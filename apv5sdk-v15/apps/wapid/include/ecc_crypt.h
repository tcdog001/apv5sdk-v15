
#ifndef _ECC_CCRYPT_H
#define _ECC_CCRYPT_H
#include "ecc192_nss.h"
int ecc192_init(void);

void ecc192_deinit(void);
int ecc192_genkey_pair(uchar_t *publickey_buffer, uint_t *publickey_buffer_len,
		uchar_t *privatekey_buffer, uint_t *privatekey_buffer_len);

int ecc192_sign(const uchar_t *privatekey, uint_t privatekey_len,
	const uchar_t *data, uint_t data_len, uchar_t *sign_buffer, uint_t *signature_len);

 int ecc192_verify(const uchar_t *publickey, uint_t publickey_len,
	const uchar_t *data, uint_t data_len, const uchar_t *sign, uint_t sign_len);


int ecc192_ecdh(const uchar_t *publickey, uint_t publickey_len,
		const uchar_t *privatekey, uint_t privatekey_len,
		uchar_t *ecdhkey_buffer, uint_t *ecdhkey_buffer_len);
int ecc192_verify_key_pair(const uchar_t *publickey, uint_t publickey_len,
		const uchar_t *privatekey, uint_t privatekey_len);
int  get_RAND_bytes(unsigned char  *buffer, int len);

#endif
