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

#ifndef _ENCRYPTAPI_H_
#define _ENCRYPTAPI_H_

#include "pbkdf2.h"
extern void hmac_sha1(const u_int8_t *text, size_t text_len, const u_int8_t *key,
    size_t key_len, u_int8_t *digest);
extern void Mrvl_PRF(unsigned char *key, int key_len, unsigned char *prefix,
                          int prefix_len, unsigned char *data, int data_len,
                          unsigned char *output, int len);

#define Mrvl_hmac_sha1 hmac_sha1
#define PKCS5_PBKDF2(pwd, ssid, slen, output) pkcs5_pbkdf2(pwd, strlen(pwd), ssid, slen, output, SHA1_DIGEST_LENGTH*2, 4096)

#endif

