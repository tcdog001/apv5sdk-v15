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
 *   void HPAVKeyDAK (uint8_t DAK [], const char * string);
 *
 *   HPAVKey.h
 *   
 *   compute the HomePlugAV compliant Device Access Key (DAK) for a
 *   NUL terminated string; return the key in buffer DAK[]; the key
 *   will be HPAVKEY_DAK_LEN bytes as defined in HPAVKey.h;
 *
 *   seed the digest then salt the digest then rehash the digest 999 
 *   times per HomePlug AV Specification; 
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

#ifndef HPAVKEYDAK_SOURCE
#define HPAVKEYDAK_SOURCE

#include <memory.h>

#include "../crypt/HPAVKey.h"
#include "../crypt/SHA256.h"

void HPAVKeyDAK (uint8_t DAK [], const char * string) 

{
	struct sha256 sha256;
	uint8_t digest [SHA256_DIGEST_LENGTH];
	const uint8_t secret [] = 
	{
		0x08,
		0x85,
		0x6D,
		0xAF,
		0x7C,
		0xF5,
		0x81,
		0x85 
	};
	int count = 999;
	SHA256Reset (&sha256);
	SHA256Write (&sha256, (uint8_t *)(string), strlen (string));
	SHA256Write (&sha256, secret, sizeof (secret));
	SHA256Fetch (&sha256, digest);
	while (count-- > 0) 
	{
		SHA256Reset (&sha256);
		SHA256Write (&sha256, digest, sizeof (digest));
		SHA256Fetch (&sha256, digest);
	}
	memcpy (DAK, digest, HPAVKEY_DAK_LEN);
	return;
}

#endif

