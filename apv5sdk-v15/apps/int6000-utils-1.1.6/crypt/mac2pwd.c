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
 *   mac2pw.c - Intellon device password generator;
 *
 *   read one or more text files containing device address strings 
 *   and print a stream of address/password pairs; device addresses 
 *   must be separated by white space; 
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
 *   Contributor(s):
 *	Charles Maier <charles.maier@intellon.com>
 *
 *--------------------------------------------------------------------*/

#define _GETOPT_H

/*====================================================================*
 *   system header files;
 *--------------------------------------------------------------------*/

#include <stdio.h>
#include <ctype.h>
#include <errno.h>
#include <limits.h>

/*====================================================================*
 *   custom header files;
 *--------------------------------------------------------------------*/

#include "../tools/getoptv.h"
#include "../tools/putoptv.h"
#include "../tools/memory.h"
#include "../tools/number.h"
#include "../tools/types.h"
#include "../tools/flags.h"
#include "../tools/error.h"
#include "../crypt/keys.h"

/*====================================================================*
 *   custom source files;
 *--------------------------------------------------------------------*/

#ifndef MAKEFILE
#include "../tools/getoptv.c"
#include "../tools/putoptv.c"
#include "../tools/version.c"
#include "../tools/todigit.c"
#include "../tools/uintspec.c"
#include "../tools/memincr.c"
#include "../tools/error.c"
#endif

#ifndef MAKEFILE
#include "../crypt/MACPasswords.c"
#endif

/*====================================================================*
 *   program constants;  
 *--------------------------------------------------------------------*/

#ifndef ETHER_ADDR_LEN
#define ETHER_ADDR_LEN 6 /* normally defined in ethernet.h or if_ether.h */
#endif

/*====================================================================*
 *
 *   void function (const char * file, const char * secret, unsigned count, unsigned bunch, flag_t flags)
 *
 *
 *.  Intellon INT6000 Linux Toolkit for HomePlug AV;
 *:  Published 2006-2008 by Intellon Corp. ALL RIGHTS RESERVED;
 *;  For demonstration; Not for production use;
 *
 *--------------------------------------------------------------------*/

static void function (const char * file, const char * secret, unsigned count, unsigned bunch, unsigned space, flag_t flags) 

{
	unsigned line = 1;
	unsigned radix = 0x10;
	unsigned width;
	unsigned digit;
	signed c = getc (stdin);
	while (c != EOF) 
	{
		uint32_t vendor = 0;
		uint32_t device = 0;
		while (isspace (c)) 
		{
			if (c == '\n') 
			{
				line++;
			}
			c = getc (stdin);
		}
		if ((c == '#') || (c == ';')) 
		{
			do 
			{
				c = getc (stdin);
			}
			while ((c != '\n') && (c != EOF));
			continue;
		}
		for (width = 0; width < ETHER_ADDR_LEN; width++) 
		{
			if ((digit = todigit (c)) < radix) 
			{
				vendor *= radix;
				vendor += digit;
				c = getc (stdin);
				continue;
			}
			error (1, EINVAL, "%s(%d): Illegal vendor", file, line);
		}
		if (!vendor) 
		{
			error (1, EPERM, "%s(%d): Vendor can't be zero", file, line);
		}
		for (width = 0; width < ETHER_ADDR_LEN; width++) 
		{
			if ((digit = todigit (c)) < radix) 
			{
				device *= radix;
				device += digit;
				c = getc (stdin);
				continue;
			}
			error (1, EINVAL, "%s(%d): Illegal device", file, line);
		}
		if (!device) 
		{
			error (1, EPERM, "%s(%d): Device can't be zero", file, line);
		}
		while (isspace (c)) 
		{
			if (c == '\n') 
			{
				line++;
			}
			c = getc (stdin);
		}
		MACPasswords (vendor, device, 1, count, bunch, space, flags);
	}
	return;
}

/*====================================================================*
 *   
 *   int main (int argc, const char * argv []);
 *
 *.  Intellon INT6000 Linux Toolkit for HomePlug AV;
 *:  Published 2006-2008 by Intellon Corp. ALL RIGHTS RESERVED;
 *;  For demonstration; Not for production use;
 *
 *--------------------------------------------------------------------*/

int main (int argc, const char * argv []) 

{
	static const char *optv [] = 
	{
		"b:l:qs:uv",
		PUTOPTV_S_FUNNEL,
		"Intellon device password generator",
		"b n\tbunching factor [4]",
		"l n\tpassword letters [16]",
		"q\tomit device address on output",
		"v\tprepend PTS flag on output",
		(const char *)(0) 
	};
	const char * secret = (const char *)(0);
	unsigned count = 16;
	unsigned bunch = 4;
	unsigned space = '-';
	flag_t flags = (flag_t)(0);
	signed c;
	optind = 1;
	opterr = 1;
	while ((c = getoptv (argc, argv, optv)) != -1) 
	{
		switch ((char)(c)) 
		{
		case 'b':
			bunch = (unsigned)(uintspec (optarg, 0, UCHAR_MAX));
			break;
		case 'l':
			count = (unsigned)(uintspec (optarg, 12, 64));
			break;
		case 'q':
			_setbits (flags, PASSWORD_SILENCE);
			break;
		case 's':
			secret = optarg;
			break;
		case 'u':
		case 'v':
			_setbits (flags, PASSWORD_VERBOSE);
			break;
		default:
			break;
		}
	}
	argc -= optind;
	argv += optind;
	if (!argc) 
	{
		function ("stdin", secret, count, bunch, space, flags);
	}
	while ((argv) && (*argv)) 
	{
		if (!freopen (*argv, "rb", stdin)) 
		{
			error (1, EINVAL, "Can't open %s", *argv);
		}
		function (*argv, secret, count, bunch, space, flags);
		argc--;
		argv++;
	}
	return (0);
}

