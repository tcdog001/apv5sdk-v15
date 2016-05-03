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

#include <linux/string.h>
#include "sha1.h"
#include "encryptapi.h"

/*
* PRF -- Length of output is in octets rather than bits
*     since length is always a multiple of 8 output array is
*     organized so first N octets starting from 0 contains PRF output
*
*     supported inputs are 16, 32, 48, 64
*     output array must be 80 octets to allow for sha1 overflow
*/
void Mrvl_PRF(unsigned char *key, int key_len,
			  unsigned char *prefix, int prefix_len,
			  unsigned char *data, int data_len, 
			  unsigned char *output, int len)
{
	int i;
	unsigned char input[1024]; /* concatenated input */
	int currentindex = 0;
	int total_len;

	memcpy(input, prefix, prefix_len);
	input[prefix_len] = 0; /* single octet 0 */
	memcpy(&input[prefix_len+1], data, data_len);
	total_len = prefix_len + 1 + data_len;
	input[total_len] = 0; /* single octet count, starts at 0 */
	total_len++;
	for (i = 0; i < (len+19)/20; i++)
	{
		Mrvl_hmac_sha1(input, total_len, key, key_len,
			&output[currentindex]);
		currentindex += 20; /* next concatenation location */
		input[total_len-1]++; /* increment octet count */
	}
}
