
#ifndef __ECC192_NSS_H__
#define __ECC192_NSS_H__

#include "ec.h"
typedef unsigned char   uchar_t;
typedef unsigned int    uint_t;

void ecc192_deinit_nss(void);
int ecc192_init_nss(ECCurveName name);
int ecc192_genkey_pair_nss(uchar_t *publickey_buffer, uint_t *publickey_buffer_len,
		uchar_t *privatekey_buffer, uint_t *privatekey_buffer_len);

int ecc192_sign_nss(const uchar_t *privatekey, uint_t privatekey_len,
	const uchar_t *digest_data, uint_t digest_len, uchar_t *sign_buffer, uint_t *signature_len);
int ecc192_verify_nss(const uchar_t *publickey, uint_t publickey_len,
	const uchar_t *digest_data, uint_t digest_len, const uchar_t *sign, uint_t sign_len);
int ecc192_ecdh_nss(const uchar_t *publickey, uint_t publickey_len,
		const uchar_t *privatekey, uint_t privatekey_len,
		uchar_t *ecdhkey_buffer, uint_t *ecdhkey_buffer_len);
int  get_randam_nss(uchar_t *buffer, int len);
#endif

