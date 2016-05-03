/*====================================================================*
 *   
 *   Copyright (c) 2010, Atheros Communications Inc.
 *   
 *   Permission to use, copy, modify, and/or distribute this software 
 *   for any purpose with or without fee is hereby granted, provided 
 *   that the above copyright notice and this permission notice appear 
 *   in all copies.
 *   
 *   THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL 
 *   WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED 
 *   WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL  
 *   THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR 
 *   CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM 
 *   LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, 
 *   NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN 
 *   CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *   
 *--------------------------------------------------------------------*/

/*====================================================================*
 *
 *   void HPAVKeyNID (uint8_t NID [], const uint8_t NMK [], signed level);
 *
 *   HPAVKey.h
 *   
 *   compute the HomePlugAV compliant Network Identification Key (NID)
 *   for a given Network Membership Key (NMK); return the key in buffer
 *   NID []; the key will be HPAVKEY_NID_LEN bytes as defined in 
 *   HPAVKey.h;
 *
 *   unlike the NMK, the NID is 7 bytes and includes the security level; 
 *   See the HomePlug AV Specification for more info;
 *
 *   hash the NMK then rehash the digest 4 times per HomePlug AV
 *   Specification; no salt is used;
 *   
 *   This software and documentation is the property of Intellon 
 *   Corporation, Ocala, Florida. It is provided 'as is' without 
 *   expressed or implied warranty of any kind to anyone for any 
 *   reason. Intellon assumes no responsibility or liability for 
 *   errors or omissions in the software or documentation and 
 *   reserves the right to make changes without notification. 
 *   
 *   Intellon customers may modify and distribute the software 
 *   without obligation to Intellon. Since use of this software 
 *   is optional, users shall bear sole responsibility and 
 *   liability for any consequences of it's use. 
 *
 *.  Intellon INT6000 Linux Toolkit for HomePlug AV;
 *:  Published 2006-2008 by Intellon Corp. ALL RIGHTS RESERVED;
 *;  For demonstration; Not for production use;
 *
 *   Contributor(s);
 *	Charles Maier <charles.maier@intellon.com>
 *
 *--------------------------------------------------------------------*/

#ifndef HPAVKEYNID_SOURCE
#define HPAVKEYNID_SOURCE

#include <memory.h>

#include "../crypt/HPAVKey.h"
#include "../crypt/SHA256.h"

void HPAVKeyNID (uint8_t NID [], const uint8_t NMK [], uint8_t level) 

{
	struct sha256 sha256;
	uint8_t digest [SHA256_DIGEST_LENGTH];
	int count = 4;
	SHA256Reset (&sha256);
	SHA256Write (&sha256, NMK, HPAVKEY_NMK_LEN);
	SHA256Fetch (&sha256, digest);
	while (count-- > 0) 
	{
		SHA256Reset (&sha256);
		SHA256Write (&sha256, digest, sizeof (digest));
		SHA256Fetch (&sha256, digest);
	}
	level <<= 4;
	digest [HPAVKEY_NID_LEN - 1] >>= 4;
	digest [HPAVKEY_NID_LEN - 1] |= level;
	memcpy (NID, digest, HPAVKEY_NID_LEN);
	return;
}

#endif

