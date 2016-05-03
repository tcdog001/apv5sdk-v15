

#include "ecc_crypt.h"
#include "hmac.h"
#include "debug.h"

int ecc192_init( )
{
	return ecc192_init_nss(ECCurve_X9_62_PRIME_192V4);
}

void ecc192_deinit(void)
{
	ecc192_deinit_nss();
}

int ecc192_genkey_pair(uchar_t *publickey_buffer, uint_t *publickey_buffer_len,
		uchar_t *privatekey_buffer, uint_t *privatekey_buffer_len)
{

	return  ecc192_genkey_pair_nss(publickey_buffer, 
				publickey_buffer_len,
				privatekey_buffer, 
				privatekey_buffer_len);
}

int ecc192_sign(const uchar_t *privatekey, uint_t privatekey_len,
	const uchar_t *data, uint_t data_len, uchar_t *sign_buffer, uint_t *signature_len)
{

	uchar_t digest_data[64]= {0,};
	int digest_len = 0;
	
	digest_len = mhash_sha256(data, data_len, digest_data);

	return ecc192_sign_nss(privatekey, privatekey_len, digest_data, digest_len, sign_buffer, signature_len);
}

 int ecc192_verify(const uchar_t *publickey, uint_t publickey_len,
	const uchar_t *data, uint_t data_len, const uchar_t *sign_value, uint_t sign_value_len)
{

	uchar_t digest_data[64]= {0,};
	int digest_len = 0;
	
	digest_len = mhash_sha256(data, data_len, digest_data);

	return ecc192_verify_nss(publickey, publickey_len, digest_data, digest_len, sign_value,sign_value_len);
}


int ecc192_ecdh(const uchar_t *publickey, uint_t publickey_len,
		const uchar_t *privatekey, uint_t privatekey_len,
		uchar_t *ecdhkey_buffer, uint_t *ecdhkey_buffer_len)
 {
	return ecc192_ecdh_nss(publickey, publickey_len,
				privatekey, privatekey_len,
				ecdhkey_buffer, ecdhkey_buffer_len);
}

int ecc192_verify_key_pair(const uchar_t *publickey, uint_t publickey_len,
		const uchar_t *privatekey, uint_t privatekey_len)
{
	const uchar_t digest_vector[]={
		0x24, 0x8D, 0x6A, 0x61, 0xD2, 0x06, 0x38, 0xB8, 
		0xE5, 0xC0, 0x26, 0x93, 0x0C, 0x3E, 0x60, 0x39,
		0xA3, 0x3C, 0xE4, 0x59, 0x64, 0xFF, 0x21, 0x67, 
		0xF6, 0xEC, 0xED, 0xD4, 0x19, 0xDB, 0x06, 0xC1
	};	
	uchar_t  sign_buffer[128] = {0,};
	uint_t    sign_len = sizeof(sign_buffer);
	int rv = -1;
	if(ecc192_sign_nss(privatekey, privatekey_len, 
		digest_vector, sizeof(digest_vector), 
		sign_buffer, &sign_len) !=0){
		DPrintf("error at %s :%d\n", __func__, __LINE__);
		goto cleanup;
	}
	if(ecc192_verify_nss(publickey, publickey_len, 
		digest_vector, sizeof(digest_vector), 
		sign_buffer, sign_len) !=1){
		DPrintf("error at %s :%d\n", __func__, __LINE__);
		goto cleanup;
	}
	rv = 0;		
cleanup:

	return rv;
}

int  get_RAND_bytes(unsigned char  *buffer, int len)
{
	return get_randam_nss(buffer, len);
}

#if 0
int ecc192_genkey_pair_test(uchar_t *publickey_buffer, uint_t *publickey_buffer_len,
		uchar_t *privatekey_buffer, uint_t *privatekey_buffer_len)
{
	const uchar_t privkey_vector0[]={
		0x3A, 0xC0, 0xE7, 0x17, 0xEB, 0x61, 0x60, 0x2E, 
		0xFC, 0xBB, 0x1D, 0xE8, 0x1A, 0xA1, 0x44, 0xA2, 
		0x72, 0xB4, 0x4B, 0xA1, 0xF1, 0x69, 0x36, 0xAC
	};

	
	return ecc192_genkey_pair_by_randprivkey_nss(publickey_buffer, 
						publickey_buffer_len,
						privatekey_buffer, 
						privatekey_buffer_len,
						privkey_vector0, 
						sizeof(privkey_vector0));
}


#endif

