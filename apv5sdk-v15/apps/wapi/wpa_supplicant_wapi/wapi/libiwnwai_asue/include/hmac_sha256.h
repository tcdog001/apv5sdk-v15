/** @file hmac_sha256.h
 *  @brief This header file contains data structures and function declarations of sha256&hmac
 *
 */

#ifndef _SHA256_H_
#define _SHA256_H_

/* SHA256 */
typedef unsigned int word32;
typedef unsigned short word16;
typedef unsigned char word8;
typedef word8 byte;
typedef word32 dword;


#define SHA256_DIGEST_SIZE 32
#define SHA256_DATA_SIZE 64

/* Digest is kept internally as 8 32-bit words. */
#define _SHA256_DIGEST_LENGTH 8

typedef struct sha256_ctx
{
  unsigned int state[_SHA256_DIGEST_LENGTH];    /* State variables */
  unsigned int count_low, count_high;           /* 64-bit block count */
  unsigned char block[SHA256_DATA_SIZE];          /* SHA256 data buffer */
  unsigned int index;                       /* index into buffer */
} SHA256_CTX;

typedef struct contxt
{
	unsigned char *buff;
	unsigned int length;
} CONTX;




#ifndef EXTRACT_UCHAR
#define EXTRACT_UCHAR(p)  (*(unsigned char *)(p))
#endif

#define STRING2INT(s) ((((((EXTRACT_UCHAR(s) << 8)    \
			 | EXTRACT_UCHAR(s+1)) << 8)  \
			 | EXTRACT_UCHAR(s+2)) << 8)  \
			 | EXTRACT_UCHAR(s+3))
			 
int hash_sha256(const CONTX *contx, unsigned length, byte *digest);


/*-----------------orgin in hmac.h-------------*/
void KD_hmac_sha256(unsigned char *text,unsigned text_len,unsigned char *key,unsigned key_len,unsigned char *output,unsigned length);
int __hmac_sha256(unsigned char * text, int text_len, byte * key, unsigned key_len, byte * digest, unsigned digest_length);
#define hmac_sha256(_t, _tl, _k, _kl, _d, _dl) \
    __hmac_sha256(_t, _tl, _k, _kl, _d, _dl)
int mhash_sha256(unsigned char *data, unsigned length, unsigned char *digest);

#endif /* _SHA256_H_ */
