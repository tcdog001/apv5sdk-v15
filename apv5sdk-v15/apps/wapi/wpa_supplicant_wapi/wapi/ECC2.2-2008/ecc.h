
#ifndef _ECC_H_
#define _ECC_H_
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#ifdef _cplusplus
 extern "C" {
#endif
typedef unsigned char BYTE;
typedef unsigned int DWord;
typedef unsigned int dword;


typedef struct {
	unsigned int *x;                
	unsigned int *y;                
} Point;
typedef Point *PointTable;


int ECC_Init(void);
#ifndef ASUE
 int Generate_PubKey(unsigned int *piPrivateKey,int piLenOfPriKey,
				 Point *poPublicKey);
 int Sign_With_Private_Key(BYTE *pbSignOut,const BYTE *pbData,int iLenIn,
				const unsigned int *piPrivateKey,int iLenOfPriKey);
 int Verify_With_Public_Key(const BYTE *pbData,int iDataLen,
			 const BYTE *pbSignIn,int iSignInLen,const Point oPubPoint);

 int PriKeyToOctetString(unsigned int *piPrivateKey,int piLenOfPriKey,
				unsigned int OSBuffSize,
				unsigned int *OSLen,unsigned char *DString);
 int OctetStringToPriKey(const unsigned char *OString, unsigned int OSLen,
				unsigned int *piPrivateKey,int *piLenOfPriKey);
 
 int PubKeyToOctetString(Point *poPublicKey,unsigned int OSBuffSize,
				 unsigned int *OSLen,unsigned char *DString);
#endif
 int ecc192_genkey(unsigned char *priv_key, unsigned char *pub_key);
 int  ecc192_sign(const unsigned char *priv_key, const unsigned char *in, int in_len, unsigned char *out);
 int   ecc192_verify(const unsigned char *pub_key, const unsigned char *in ,  int in_len, const unsigned char *sign,int sign_len);
 int ecc192_ecdh(const unsigned char * priv_key, const unsigned char *pub_key, unsigned char * ecdhkey);
#ifdef _cplusplus
 }
#endif
#endif
