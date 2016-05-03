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
 *   hpavkey.c - HomePlug AV key generator;
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

#define _GETOPT_H

/*====================================================================*
 *   system header files;
 *--------------------------------------------------------------------*/

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <unistd.h>

/*====================================================================*
 *   custom header files;
 *--------------------------------------------------------------------*/

#include "../tools/getoptv.h"
#include "../tools/putoptv.h"
#include "../tools/version.h"
#include "../tools/number.h"
#include "../tools/types.h"
#include "../tools/flags.h"
#include "../tools/error.h"
#include "../crypt/HPAVKey.h"
#include "../crypt/SHA256.h"

/*====================================================================*
 *   custom source files;
 *--------------------------------------------------------------------*/

#ifndef MAKEFILE
#include "../tools/getoptv.c"
#include "../tools/putoptv.c"
#include "../tools/version.c"
#include "../tools/uintspec.c"
#include "../tools/todigit.c"
#include "../tools/hexout.c"
#include "../tools/error.c"
#endif

#ifndef MAKEFILE
#include "../crypt/HPAVKeyDAK.c"
#include "../crypt/HPAVKeyNMK.c"
#include "../crypt/HPAVKeyNID.c"
#include "../crypt/HPAVKeySHA.c"
#include "../crypt/HPAVKeyOut.c"
#include "../crypt/SHA256.c"
#endif

/*====================================================================*
 *   
 *   int main (int argc, const char * argv []);
 *
 *
 *
 *--------------------------------------------------------------------*/

int main (int argc, const char * argv []) 

{
	static const char *optv [] = 
	{
		"DeL:MNqv",
		"phrase [phrase] [...]",
		"HomePlug AV key generator",
		"D\tconvert password to Device Access Key",
		"e\tenforce HomePlug AV password rules",
		"M\tconvert password to Network Membership Key",
		"N\tconvert password to Network Identifier",
		"L n\tSecurity Level is (n) [0]",
		"q\tquiet mode",
		"v\tverbose mode",
		(const char *) (0) 
	};
	byte digest [SHA256_DIGEST_LENGTH];
	const char * phrase;
	const char * sp;
	signed type = 0;
	signed level = 0;
	flag_t flags = (flag_t)(0);
	signed c;
	optind = 1;
	opterr = 1;
	optmin = 1;
	while ((c = getoptv (argc, argv, optv)) != -1) 
	{
		switch ((char) (c)) 
		{
		case 'D':
			type = HPAVKEY_DAK;
			break;
		case 'M':
			type = HPAVKEY_NMK;
			break;
		case 'N':
			type = HPAVKEY_NID;
			break;
		case 'L':
			level = (signed)(uintspec (optarg, 0, 1));
			break;
		case 'q':
			_setbits (flags, HPAVKEY_SILENCE);
			break;
		case 'v':
			_setbits (flags, HPAVKEY_VERBOSE);
			break;
		case 'e':
			_setbits (flags, HPAVKEY_ENFORCE);
			break;
		default:
			break;
		}
	}
	argc -= optind;
	argv += optind;
	while ((argc--) && (*argv)) 
	{
		sp = phrase = *argv++;
		while (isprint (*sp)) 
		{
			sp++;
		}
		if (_anyset (flags, HPAVKEY_ENFORCE)) 
		{
			if (*sp) 
			{
				error (0, ENOTSUP, "Phrase \"%s\" contains illegal characters", phrase);
				continue;
			}
			if ((sp - phrase) < HPAVKEY_PHRASE_MIN) 
			{
				error (0, ENOTSUP, "Phrase \"%s\" less than %d characters", phrase, HPAVKEY_PHRASE_MIN);
				continue;
			}
			if ((sp - phrase) > HPAVKEY_PHRASE_MAX) 
			{
				error (0, ENOTSUP, "Phrase \"%s\" more than %d characters", phrase, HPAVKEY_PHRASE_MAX);
				continue;
			}
		}
		if (type == HPAVKEY_DAK) 
		{
			HPAVKeyDAK (digest, phrase);
			HPAVKeyOut (digest, HPAVKEY_DAK_LEN, phrase, flags);
			continue;
		}
		if (type == HPAVKEY_NMK) 
		{
			HPAVKeyNMK (digest, phrase);
			HPAVKeyOut (digest, HPAVKEY_NMK_LEN, phrase, flags);
			continue;
		}
		if (type == HPAVKEY_NID) 
		{
			HPAVKeyNMK (digest, phrase);
			HPAVKeyNID (digest, digest, level);
			HPAVKeyOut (digest, HPAVKEY_NID_LEN, phrase, flags);
			continue;
		}
		HPAVKeySHA (digest, phrase);
		HPAVKeyOut (digest, HPAVKEY_SHA_LEN, phrase, flags);
	}
	return (0);
}

