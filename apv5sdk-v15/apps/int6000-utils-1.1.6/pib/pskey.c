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
 *   psfp.c - prescaler finger print; 
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
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>

/*====================================================================*
 *   custom header files;
 *--------------------------------------------------------------------*/

#include "../tools/getoptv.h"
#include "../tools/number.h"
#include "../tools/types.h"
#include "../tools/error.h"
#include "../tools/files.h"
#include "../tools/flags.h"
#include "../crypt/HPAVKey.h"
#include "../crypt/SHA256.h"
#include "../pib/pib.h"

/*====================================================================*
 *   custom source files;
 *--------------------------------------------------------------------*/

#ifndef MAKEFILE
#include "../tools/getoptv.c"
#include "../tools/putoptv.c"
#include "../tools/version.c"
#include "../tools/fdchecksum_32.c"
#include "../tools/hexdecode.c"
#include "../tools/hexstring.c"
#include "../tools/filepart.c"
#include "../tools/hexout.c"
#include "../tools/error.c"
#endif

#ifndef MAKEFILE
#include "../pib/pibfile.c"
#endif

#ifndef MAKEFILE
#include "../crypt/SHA256Reset.c"
#include "../crypt/SHA256Write.c"
#include "../crypt/SHA256Block.c"
#include "../crypt/SHA256Fetch.c"
#include "../crypt/keys.c"
#endif

/*====================================================================*
 *   program constants;
 *--------------------------------------------------------------------*/

#define PSKEY_VERBOSE (1 << 0)
#define PSKEY_SILENCE (1 << 1)

#define PS_OFFSET 0x0A10
#define PS_MEMBER 1155

/*====================================================================*
 *
 *   static signed pskey (struct _file_ * pib);
 *
 *   compute the SHA256 digest of the PIB file prescalers and print on
 *   stdout; the digest act like a fingerprint;
 *
 *.  Intellon INT6000 Linux Toolkit for HomePlug AV;
 *;  For demonstration; Not for production use;
 *;  For demonstration; Not for production use;
 *
 *--------------------------------------------------------------------*/

static signed pskey (struct _file_ * pib, flag_t flags) 

{
	struct sha256 sha256;
	byte digest [SHA256_DIGEST_LENGTH];
	uint32_t buffer [PS_MEMBER];
	if (lseek (pib->file, PS_OFFSET, SEEK_SET) != PS_OFFSET) 
	{
		error (0, errno, "Can't seek %s", filepart (pib->name));
		return (-1);
	}
	if (read (pib->file, buffer, sizeof (buffer)) < sizeof (buffer)) 
	{
		error (0, errno, "Can't read %s", filepart (pib->name));
		return (-1);
	}
	SHA256Reset (&sha256);
	SHA256Write (&sha256, (uint8_t *)(buffer), sizeof (buffer));
	SHA256Fetch (&sha256, digest);
	hexout (digest, sizeof (digest), '\0', stdout);
	if (_allclr (flags, PSKEY_SILENCE)) 
	{
		printf (" %s", filepart (pib->name));
	}
	printf ("\n");
	return (0);
}

/*====================================================================*
 *   
 *   int main (int argc, const char *argv []) 
 *   
 *.  Intellon INT6000 Linux Toolkit for HomePlug AV;
 *:  Published 2006-2008 by Intellon Corp. ALL RIGHTS RESERVED;
 *;  For demonstration; Not for production use;
 *
 *--------------------------------------------------------------------*/

int main (int argc, const char *argv []) 

{
	static const char *optv [] = 
	{
		"lqrsv",
		"file [file] [...]",
		"print prescaler finger-print on stdout",
		"q\tquiet mode",
		"v\tverbose mode",
		(const char *) (0) 
	};
	struct _file_ pib;
	signed state = 0;
	flag_t flags = (flag_t) (0);
	signed c;
	optind = 1;
	opterr = 1;
	while ((c = getoptv (argc, argv, optv)) != -1) 
	{
		switch ((char) (c)) 
		{
		case 'q':
			_setbits (flags, PSKEY_SILENCE);
			break;
		case 'v':
			_setbits (flags, PSKEY_VERBOSE);
			break;
		default:
			break;
		}
	}
	argc -= optind;
	argv += optind;
	while ((argc) && (*argv)) 
	{
		pib.name = *argv;
		if ((pib.file = open (pib.name, O_BINARY|O_RDWR)) == -1) 
		{
			error (0, errno, "Can't open %s", pib.name);
			state = 1;
			errno = 0;
		}
		else if (pibfile (&pib)) 
		{
			error (0, errno, "Bad PIB file: %s", pib.name);
			state = 1;
		}
		else if (pskey (&pib, flags)) 
		{
			state = 1;
		}
		close (pib.file);
		argc--;
		argv++;
	}
	return (state);
}

