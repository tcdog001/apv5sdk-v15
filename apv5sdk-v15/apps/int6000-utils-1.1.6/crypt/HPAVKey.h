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
 *   HPAVKey.h - Intellon HomePlug AV Key Generator;
 *
 *
 *.  Intellon Linux Toolkit; 
 *:  Published 2007 by Intellon Corp. ALL RIGHTS RESERVED;
 *;  For demonstration; Not for production use;
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
 *   Contributor(s);
 *	Charles Maier <charles.maier@intellon.com>
 *
 *--------------------------------------------------------------------*/

#ifndef HPAVKEY_HEADER
#define HPAVKEY_HEADER

/*====================================================================*
 *   system header files;
 *--------------------------------------------------------------------*/

#include <stdint.h>
#include <string.h>

/*====================================================================*
 *   constants;
 *--------------------------------------------------------------------*/

#define HPAVKEY_CHAR_MIN 0x20
#define HPAVKEY_CHAR_MAX 0x7E
#define HPAVKEY_PHRASE_MIN 12
#define HPAVKEY_PHRASE_MAX 64
#define HPAVKEY_DIGEST_LEN 32
#define HPAVKEY_SHA_LEN 32
#define HPAVKEY_DAK_LEN 16
#define HPAVKEY_NMK_LEN 16
#define HPAVKEY_NID_LEN 7

#define HPAVKEY_SHA 0
#define HPAVKEY_DAK 1
#define HPAVKEY_NMK 2
#define HPAVKEY_NID 3

#define HPAVKEY_VERBOSE (1 << 0)
#define HPAVKEY_SILENCE (1 << 1)
#define HPAVKEY_ENFORCE (1 << 2)

/*====================================================================*
 *
 *--------------------------------------------------------------------*/

void HPAVKeyNMK (uint8_t NMK [], const char * string);
void HPAVKeyDAK (uint8_t DAK [], const char * string);
void HPAVKeyNID (uint8_t NID [], const uint8_t NMK [], uint8_t mode);
void HPAVKeySHA (uint8_t digest [], const char * string);
void HPAVKeyOut (const uint8_t digest [], size_t length, const char * phrase, unsigned flags);

/*====================================================================*
 *
 *--------------------------------------------------------------------*/

#endif 

