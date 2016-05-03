#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ecc192_nss.h"
#if 1
static ECParams *ecparams;

void ecc192_deinit_nss(void)
{
	soft_free_ecparams(ecparams, B_TRUE);
}


int ecc192_init_nss(ECCurveName name)
{
	int  kmflag = 0;
	int rv = SECFailure;
	
	ecparams = (ECParams *)PORT_ArenaZAlloc(NULL, sizeof(ECParams), kmflag);
	if (!ecparams) {
		return SECFailure;
	}

	CHECK_SEC_OK( gf_populate_params(name, ec_field_GFp, ecparams, kmflag) );

	return ecparams->order.len;
	
cleanup:
	if (!ecparams->cofactor) {
#if EC_DEBUG
		printf("Unrecognized curve, returning NULL params\n");
#endif
	}
	ecc192_deinit_nss();
	return SECFailure;
}



int ecc192_genkey_pair_nss(uchar_t *publickey_buffer, uint_t *publickey_buffer_len,
		uchar_t *privatekey_buffer, uint_t *privatekey_buffer_len)
{

	ECPrivateKey *privKey = NULL;	
	unsigned int olen = ecparams->order.len; 
	
	if((*publickey_buffer_len < 2*olen+1) ||(*privatekey_buffer_len <olen))
	{
		printf("[NSS_ECC] :: key_buf < order length(%d)\n", ecparams->order.len);
		return SECFailure;
	}
	
	if (EC_NewKey(ecparams, &privKey, 0) != SECSuccess) {
		
		return (SECFailure);
	}
	
	memcpy(	publickey_buffer, privKey->publicValue.data, privKey->publicValue.len);
	*publickey_buffer_len = privKey->publicValue.len;
		
	memcpy(	privatekey_buffer,privKey->privateValue.data, privKey->privateValue.len);
	*privatekey_buffer_len = privKey->privateValue.len;
	
	soft_free_ecprivkey(privKey);
	return SECSuccess; 
}

int ecc192_sign_nss(const uchar_t *privatekey, uint_t privatekey_len,
	const uchar_t *digest_data, uint_t digest_len, uchar_t *sign_buffer, uint_t *signature_len)
{
	SECStatus rv = SECFailure;
	ECPrivateKey  key;	
	SECItem  digest;
	SECItem  signature; 
	
	memset(&key, 0, sizeof(key));	
	memset(&digest, 0, sizeof(digest));
	memset(&signature, 0, sizeof(signature));

	key.ecParams = *ecparams;
	key.privateValue.data = (uchar_t *)privatekey;
	key.privateValue.len = privatekey_len;
	
	digest.data = (uchar_t *)digest_data;
	digest.len = digest_len;

	signature.data = sign_buffer;
	signature.len = *signature_len;

	
	if(ECDSA_SignDigest(&key, &signature, &digest, 0) == 0){
		*signature_len = 	signature.len;
		rv = SECSuccess;
	}
	
	return rv;
	
}
int ecc192_verify_nss(const uchar_t *publickey, uint_t publickey_len,
	const uchar_t *digest_data, uint_t digest_len, const uchar_t *sign, uint_t sign_len)
{
	SECStatus rv = SECFailure;
	ECPublicKey  key;	
	SECItem  digest;
	SECItem  signature; 
	
	memset(&key, 0, sizeof(key));	
	memset(&digest, 0, sizeof(digest));
	memset(&signature, 0, sizeof(signature));

	key.ecParams = *ecparams;
	key.publicValue.data = (uchar_t *)publickey;
	key.publicValue.len = publickey_len;
	
	digest.data = (uchar_t *)digest_data;
	digest.len = digest_len;

	signature.data = (uchar_t *)sign;
	signature.len = sign_len;


	rv = ECDSA_VerifyDigest(&key,&signature, &digest, 0);
	return (rv==0?1:0);
	
}

int ecc192_ecdh_nss(const uchar_t *publickey, uint_t publickey_len,
		const uchar_t *privatekey, uint_t privatekey_len,
		uchar_t *ecdhkey_buffer, uint_t *ecdhkey_buffer_len)
 {
	int rv = SECFailure;
	SECItem   privateValue;
	SECItem   publicValue;
	SECItem   secret_item;
	unsigned int olen = ecparams->order.len; 

	memset(&privateValue, 0, sizeof(privateValue));
	memset(&publicValue, 0, sizeof(publicValue));
	memset(&secret_item, 0, sizeof(secret_item));

	

	if((!ecdhkey_buffer)||( *ecdhkey_buffer_len < olen)){
		printf("err %s:%d,  *ecdhkey_buffer_len= %d< olen=%d\n", __func__, __LINE__,
			*ecdhkey_buffer_len, olen);
		goto cleanup;
	}

	privateValue.data = (uchar_t *)privatekey;
	privateValue.len = privatekey_len;

	publicValue.data = (uchar_t *)publickey;
	publicValue.len = publickey_len;
	

	if(ECDH_Derive(&publicValue, ecparams, &privateValue, B_FALSE, &secret_item, 0) ==0){
		memcpy(ecdhkey_buffer, secret_item.data, secret_item.len);
		*ecdhkey_buffer_len = secret_item.len;
		rv = SECSuccess;
	}
cleanup:
	SECITEM_FreeItem(&secret_item, B_FALSE);
	return rv;

}

int  get_randam_nss(uchar_t *buffer, int len)
{
	return RNG_GenerateGlobalRandomBytes(buffer ,len);
}


#if 0
static int ecc192_genkey_pair_by_randprivkey_nss(uchar_t *publickey_buffer, uint_t *publickey_buffer_len,
		uchar_t *privatekey_buffer, uint_t *privatekey_buffer_len, 
		const uchar_t  * privKeyBytes, uint_t privKeyBytes_len);
static int ecc192_sign_test_nss(const uchar_t *privatekey, uint_t privatekey_len,
	const uchar_t *digest_data, uint_t digest_len, uchar_t *sign_buffer, uint_t *signature_len,
	const uchar_t *kb, uint_t kb_len);


static void hex_dump(char *name, void *_str,int len)
{
	int i;
	unsigned char *str = (unsigned char *)_str;

	if(name != NULL)
		printf("[%s]:: (length=%d) : \n", name, len);
	for(i=0;i<len;i++)
	{
		printf("%02X ",*str++);
		if(len >16){
			if(((i+1)%16==0) && ((i+1) <len))
				printf("\n");
		}
	}
	printf("\n");
}


static int ecc192_genkey_pair_by_randprivkey_nss(uchar_t *publickey_buffer, uint_t *publickey_buffer_len,
		uchar_t *privatekey_buffer, uint_t *privatekey_buffer_len, 
		const uchar_t  * privKeyBytes, uint_t privKeyBytes_len)
{
	SECStatus rv = SECFailure;
	ECPrivateKey *privKey = NULL;	
	unsigned int olen = ecparams->order.len; 

	if( privKeyBytes_len != ecparams->order.len)
	{
		printf("[NSS_ECC]::  rand privKeyBytes < order length(%d)\n", ecparams->order.len);
		return SECFailure;
	}
	if((*publickey_buffer_len < 2*olen+1) ||(*privatekey_buffer_len <olen))
	{
		printf("[NSS_ECC] :: key buffer < order length(%d)\n", ecparams->order.len);
		return SECFailure;
	}
	
	CHECK_SEC_OK( ec_NewKey(ecparams, &privKey, privKeyBytes, privKeyBytes_len, 0) );

	
	memcpy(	(void*)publickey_buffer,privKey->publicValue.data, privKey->publicValue.len);
	*publickey_buffer_len = privKey->publicValue.len;
		
	memcpy(	privatekey_buffer,privKey->privateValue.data, privKey->privateValue.len);
	*privatekey_buffer_len = privKey->privateValue.len;

cleanup:

#if EC_DEBUG
    printf("EC_NewKey returning %s\n", 
	(rv == SECSuccess) ? "success" : "failure");
#endif
	if(privKey)
		soft_free_ecprivkey(privKey);
	return rv; 
}

int ecc192_sign_test_nss(const uchar_t *privatekey, uint_t privatekey_len,
	const uchar_t *digest_data, uint_t digest_len, uchar_t *sign_buffer, uint_t *signature_len,
	const uchar_t *kb, uint_t kb_len)
{
	SECStatus rv = SECFailure;
	ECPrivateKey  key;	
	SECItem  digest;
	SECItem  signature; 

	if ((!kb) || (!privatekey) || (!digest_data) || (!sign_buffer)) {
		printf("error at %s:%d\n", __func__, __LINE__);
		return SECFailure;
	}

	memset(&key, 0, sizeof(key));	
	memset(&digest, 0, sizeof(digest));
	memset(&signature, 0, sizeof(signature));

	key.ecParams = *ecparams;
	key.privateValue.data = ( uchar_t *)privatekey;
	key.privateValue.len = privatekey_len;
	
	digest.data = ( uchar_t *)digest_data;
	digest.len = digest_len;

	signature.data = sign_buffer;
	signature.len = *signature_len;

    
 	if(kb_len != key.ecParams.order.len){
		printf("error at %s:%d\n", __func__, __LINE__);
		printf("kb_len = %d,  key.ecParams.order.len = %d\n",kb_len, key.ecParams.order.len);
		return SECFailure;
	}

	/* Generate ECDSA signature with the specified k value */
	rv = ECDSA_SignDigestWithSeed(&key, &signature, &digest, kb, kb_len, 0);

#if EC_DEBUG
	printf("ECDSA signing %s\n",
	(rv == SECSuccess) ? "succeeded" : "failed");
#endif

	return rv;
}


int ecc192_test_nss(void)
{
	int order_len = 0;
	ECPrivateKey privkey;
	ECPublicKey   publickey;
	SECItem signature;
	SECItem ecdhkey;
	int rv = -1;
	
	const uchar_t privkey_vector0[]={
		0x3A, 0xC0, 0xE7, 0x17, 0xEB, 0x61, 0x60, 0x2E, 
		0xFC, 0xBB, 0x1D, 0xE8, 0x1A, 0xA1, 0x44, 0xA2, 
		0x72, 0xB4, 0x4B, 0xA1, 0xF1, 0x69, 0x36, 0xAC
	};
			
	const uchar_t pubkey_vector0[]={
		0x04,
		0x7E, 0x19, 0x69, 0xFD, 0x0B, 0x00, 0x18, 0x10, 
		0xA4, 0xE7, 0xF4, 0x14, 0xC2, 0x3F, 0x2B, 0xAD, 
		0xF6, 0xB2, 0xDE, 0x96, 0xAE, 0x6B, 0x78, 0x56, 
		0x29, 0x42, 0x67, 0x71, 0xED, 0xD3, 0x00, 0x1F, 
		0x4A, 0x42, 0x53, 0xD8, 0xEE, 0xB9, 0xFF, 0xC1, 
		0x86, 0x84, 0xC6, 0xC0, 0xB4, 0x3A, 0xCA, 0x08
	};

	const uchar_t privkey_vector1[]={
		0x25, 0xFB, 0xB3, 0x2E, 0xFB, 0xEC, 0x6E, 0xCB, 
		0x13, 0x14, 0x33, 0x2A, 0x02, 0x65, 0x82, 0xDB, 
		0x7B, 0xE0, 0x0C, 0x05, 0x1C, 0xF2, 0xFA, 0x80
	};

	const uchar_t pubkey_vector1[]={
		0x04,
		0x06, 0x21, 0xD8, 0xAD, 0xAB, 0x09, 0x52, 0x75, 
		0x2E, 0xBE, 0xAE, 0x50, 0x07, 0xF6, 0xAE, 0x45, 
		0x5C, 0x61, 0x86, 0x0D, 0x1C, 0xEA, 0xDB, 0x25,
		0x6A, 0x58, 0xD5, 0xD5, 0x50, 0x87, 0x32, 0x5D, 
		0xAC, 0x43, 0x4C, 0x0D, 0xD2, 0x8A, 0x9F, 0x81, 
		0x59, 0x07, 0x0C, 0x8A, 0xAE, 0xCD, 0x21, 0xD8
	};	

	const uchar_t digest_vector[]={
		0x24, 0x8D, 0x6A, 0x61, 0xD2, 0x06, 0x38, 0xB8, 
		0xE5, 0xC0, 0x26, 0x93, 0x0C, 0x3E, 0x60, 0x39,
		0xA3, 0x3C, 0xE4, 0x59, 0x64, 0xFF, 0x21, 0x67, 
		0xF6, 0xEC, 0xED, 0xD4, 0x19, 0xDB, 0x06, 0xC1
	};	

	const uchar_t rand_privkey_vector[]={
		0x2D, 0x8E, 0xEC, 0x49, 0xE2, 0x68, 0x4E, 0xE6,
		0xE0, 0x9A, 0xC6, 0x80, 0x8A, 0x16, 0x39, 0xED, 
		0xC4, 0x8F, 0x12, 0xBE, 0xFB, 0x6C, 0xE9, 0x6F
	};
	
	const  uchar_t sign_vector[]={
		0x23, 0x46, 0x0E, 0xF8, 0x47, 0xE2, 0xBE, 0x62, 
		0x45, 0x40, 0x00, 0x10, 0xE9, 0x23, 0x42, 0xD4, 
		0xAE, 0x30, 0x31, 0x8A, 0x03, 0xFE, 0x9F, 0xA5,
		0x18, 0x7F, 0x1E, 0x57, 0xF9, 0xB9, 0x22, 0x02, 
		0xEF, 0x4F, 0xD0, 0xC7, 0x80, 0x67, 0xE6, 0x27, 
		0x2A, 0xB3, 0xA1, 0x6F, 0xB9, 0x8B, 0x03, 0x56
	};	

	const uchar_t dhkey_vector[]={
		0x3A, 0x74, 0xDD, 0xFA, 0x30, 0x80, 0xF6, 0xB5, 
		0xA1, 0x68, 0x8C, 0x6E, 0xB7, 0xB0, 0x98, 0x24, 
		0x0B, 0x5A, 0xFC, 0x67, 0x24, 0x50, 0xA4, 0x25
	};
	
	memset(&privkey, 0, sizeof(privkey));
	memset(&publickey, 0, sizeof(publickey));
	memset(&signature, 0, sizeof(signature));
	memset(&ecdhkey, 0, sizeof(ecdhkey));

	order_len = ecc192_init_nss(ECCurve_X9_62_PRIME_192V4);
	if( order_len < 0){
		printf("ecc192_init failure\n");	
		goto cleanup;
	}

	
	SECITEM_AllocItem(NULL, &privkey.privateValue, order_len, 0);
	if(!privkey.privateValue.data){
		printf("[NSS_ECC]::  allocItem failure, error at %s:%d\n", __func__, __LINE__);
		goto cleanup;
	}

	SECITEM_AllocItem(NULL, &publickey.publicValue, 2*order_len+1, 0);
	if(!publickey.publicValue.data){
		printf("[NSS_ECC]::  allocItem failure, error at %s:%d\n", __func__, __LINE__);
		goto cleanup;
	}
	
	SECITEM_AllocItem(NULL, &signature, 2*order_len, 0);
	if(!signature.data){
		printf("[NSS_ECC]::  allocItem failure, error at %s:%d\n", __func__, __LINE__);
		goto cleanup;
	}

	SECITEM_AllocItem(NULL, &ecdhkey, order_len, 0);
	if(!ecdhkey.data){
		printf("[NSS_ECC]::  allocItem failure, error at %s:%d\n", __func__, __LINE__);
		goto cleanup;
	}
	
	printf("[ECC192_test]:: generate pairkey  test\n");
	if(ecc192_genkey_pair_by_randprivkey_nss(publickey.publicValue.data, 
		&publickey.publicValue.len, 
		privkey.privateValue.data, 
		&privkey.privateValue.len, 
		privkey_vector0, 
		sizeof(privkey_vector0)) != 0){
		printf("[NSS_ECC]::  error at %s\n", __func__);
		goto cleanup;
	}
	printf("Genkey success\n");
	hex_dump("privatekey", privkey.privateValue.data, privkey.privateValue.len);
	hex_dump("privatekey", publickey.publicValue.data, publickey.publicValue.len); 

	if((privkey.privateValue.len == sizeof(privkey_vector0)) 
		&&(memcmp(privkey.privateValue.data, privkey_vector0, privkey.privateValue.len) == 0)
		&&(publickey.publicValue.len == sizeof(pubkey_vector0))
		&&(memcmp(publickey.publicValue.data, pubkey_vector0, publickey.publicValue.len) == 0)){

		printf("Pairkey test success\n");
	}else{
		printf("Pairkey not match\n");
		goto cleanup;
	}

	printf("[ECC192_test]:: sign  test\n");
	
	if(0!= ecc192_sign_test_nss(privkey_vector0,	
		sizeof(privkey_vector0), 
		digest_vector, 
		sizeof(digest_vector), 
		signature.data, 
		&signature.len, 
		rand_privkey_vector, 
		sizeof(rand_privkey_vector)
		)){
		printf("[NSS_ECC]::  error at %s:%d\n", __func__, __LINE__);
		goto cleanup;
	}

	if((signature.len == sizeof(sign_vector)) \
		&& (memcmp(signature.data, sign_vector, signature.len) == 0)){
		printf("ECC192 sign test  success\n");
	}else{
		printf("signatuer not match\n");
		goto cleanup;
	}

	printf("[ECC192_test]:: Verify  test\n");

	if(ecc192_verify_nss(publickey.publicValue.data, publickey.publicValue.len, 
		digest_vector, sizeof(digest_vector), signature.data, signature.len)){
		printf("ECC192 Verify test success\n");
	}else{
		printf("ECC192 Verify test failure\n");
		goto cleanup;
	}

	printf("[ECC192_test]:: ECDH  test 1\n");
		
	if(ecc192_ecdh_nss(pubkey_vector0, sizeof(pubkey_vector0), 
			privkey_vector1, sizeof(privkey_vector1), 
			ecdhkey.data, &ecdhkey.len) != 0){
		printf("ECC192 ECDH test failure\n");
		goto cleanup;
	}

	if((ecdhkey.len == sizeof(dhkey_vector)) && (memcmp(ecdhkey.data, dhkey_vector, ecdhkey.len) == 0)){
		printf("ECC192 ECDH test1  success\n");
	}else{
		printf("ECC192 Verify test1  failure\n");
		goto cleanup;
	}

	printf("[ECC192_test]:: ECDH  test 2\n");
		
	if(ecc192_ecdh_nss(pubkey_vector1, sizeof(pubkey_vector1), 
			privkey_vector0, sizeof(privkey_vector0), 
			ecdhkey.data, &ecdhkey.len) != 0){
		printf("ECC192 ECDH test failure\n");
		goto cleanup;
	}

	if((ecdhkey.len == sizeof(dhkey_vector)) && (memcmp(ecdhkey.data, dhkey_vector, ecdhkey.len) == 0)){
		printf("ECC192 ECDH test2  success\n");
	}else{
		printf("ECC192 Verify test2  failure\n");
		goto cleanup;
	}
	
cleanup:
	SECITEM_FreeItem(&privkey.privateValue, B_FALSE);
	SECITEM_FreeItem(&publickey.publicValue, B_FALSE);
	SECITEM_FreeItem(&signature, B_FALSE);
	SECITEM_FreeItem(&ecdhkey, B_FALSE);
	ecc192_deinit_nss();
	return rv;
}
#endif
#endif

