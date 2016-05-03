/** @file hmac.h
 *  @brief This header file contains function declarations of sha256&hmac
 *
 */

#ifndef _HMAC_
#define _HMAC_

#ifdef  __cplusplus
extern "C" {
#endif

int mhash_sha256(const unsigned char* data, unsigned length, unsigned char* digest);
int hmac_sha256(const unsigned char* text, int text_len, const unsigned char* key, unsigned key_len, unsigned char* digest, unsigned digest_length);
void KD_hmac_sha256(const unsigned char* text, unsigned text_len, const unsigned char* key, unsigned key_len, unsigned char* output, unsigned length);

#ifdef  __cplusplus
}
#endif

#endif /*_IWN_HMAC_20090216*/

/*int mhash_sha256(unsigned char* d, unsigned l, unsigned char* o);
int hmac_sha256(unsigned char* t, int tl, unsigned char* k, unsigned kl, unsigned char* o, unsigned ol);
void KD_hmac_sha256(unsigned char* t, unsigned tl, unsigned char* k, unsigned kl, unsigned char* o, unsigned ol);*/
