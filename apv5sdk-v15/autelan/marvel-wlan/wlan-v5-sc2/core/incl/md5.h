/*
*                Copyright 2002-2005, Marvell Semiconductor, Inc.
* This code contains confidential information of Marvell Semiconductor, Inc.
* No rights are granted herein under any patent, mask work right or copyright
* of Marvell or any third party.
* Marvell reserves the right at its sole discretion to request that this code
* be immediately returned to Marvell. This code is provided "as is".
* Marvell makes no warranties, express, implied or otherwise, regarding its
* accuracy, completeness or performance.
*/

#ifndef _MD5_H_
#define _MD5_H_

typedef struct{
    unsigned long state[4];/* state (ABCD) */
    unsigned long count[2];/* number of bits, modulo 2^64 (lsb first) */
    unsigned char buffer[64];/* input buffer */
}Mrvl_MD5_CTX; 

void wpa_MD5Init (Mrvl_MD5_CTX *context);
void wpa_MD5Update (Mrvl_MD5_CTX *context, UINT8 *input, UINT32 inputLen);
void wpa_MD5Final (unsigned char digest[16], Mrvl_MD5_CTX *context);
void Mrvl_hmac_md5(UINT8 *text, int text_len, UINT8 *key, int key_len,
              void * digest);
//void wpa_MD5Transform(UINT32 *, unsigned int *);

#endif
