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
 *   psin.c - load prescalers into int6000 parameter file;
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
#include <string.h>
#include <unistd.h>
#include <errno.h>

// #include <sys/stat.h>
/*====================================================================*
 *   custom header files;
 *--------------------------------------------------------------------*/

#include "../tools/getoptv.h"
#include "../tools/number.h"
#include "../tools/chars.h"
#include "../tools/types.h"
#include "../tools/error.h"
#include "../tools/files.h"
#include "../crypt/HPAVKey.h"
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
#include "../tools/todigit.c"
#include "../tools/error.c"
#endif

#ifndef MAKEFILE
#include "../pib/pibfile.c"
#include "../pib/piblock.c"
#endif

#ifndef MAKEFILE
#include "../crypt/keys.c"
#endif

/*====================================================================*
 *   program constants;
 *--------------------------------------------------------------------*/

#define PSIN_VERBOSE (1 << 0)
#define PSIN_SILENCE (1 << 1)
#define PSIN_ALLTONE (1 << 2)

/*====================================================================*
 *
 *   signed psin (struct _file_ * pib, flag_t flags);
 *
 *.  Intellon INT6000 Linux Toolkit for HomePlug AV;
 *:  Published 2006-2008 by Intellon Corp. ALL RIGHTS RESERVED;
 *;  For demonstration; Not for production use;
 *
 *--------------------------------------------------------------------*/

static signed psin (struct _file_ * pib, flag_t flags) 

{
	uint32_t carrier;
	uint32_t scalar;
	signed carriers = 0;
	signed c;
	if (lseek (pib->file, PRESCALAR_OFFSET, SEEK_SET) != PRESCALAR_OFFSET) 
	{
		error (1, errno, "Can't seek %s", filepart (pib->name));
	}
	while ((c = getc (stdin)) != EOF) 
	{
		if (isspace (c)) 
		{
			continue;
		}
		if ((c == '#') || (c == ';')) 
		{
			do 
			{
				c = getc (stdin);
			}
			while (nobreak (c));
			continue;
		}
		carrier = 0;
		while (isdigit (c)) 
		{
			carrier *= 10;
			carrier += c - '0';
			c = getc (stdin);
		}
		if (carrier != carriers) 
		{
			error (1, ECANCELED, "Carrier %d/%d out of order", carrier, carriers);
		}
		if (carrier >= PRESCALAR_MEMBER) 
		{
			error (1, ECANCELED, "Too many prescalers");
		}
		while (isblank (c)) 
		{
			c = getc (stdin);
		}
		scalar = 0;
		while (isxdigit (c)) 
		{
			scalar *= 16;
			scalar += todigit (c);
			c = getc (stdin);
		}
		scalar = HTOLE32 (scalar);
		if (write (pib->file, &scalar, sizeof (scalar)) < sizeof (scalar)) 
		{
			error (1, errno, "Can't save %s", filepart (pib->name));
		}
		while (nobreak (c)) 
		{
			c = getc (stdin);
		};
		carriers++;
	}
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
		"",
		"pibfile [< scalars]",
		"load prescalers into int6000 parameter file",
		(const char *) (0) 
	};
	struct _file_ pib;
	signed state = 0;
	flag_t flags = (flag_t)(0);
	signed c;
	optind = 1;
	opterr = 1;
	while ((c = getoptv (argc, argv, optv)) != -1) 
	{
		switch ((char) (c)) 
		{
		default:
			break;
		}
	}
	argc -= optind;
	argv += optind;
	if (argc > 1) 
	{
		error (1, ECANCELED, "Only one target file allowed");
	}
	if ((argc) && (*argv)) 
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
		else if (psin (&pib, flags)) 
		{
			state = 1;
		}
		else if (piblock (&pib)) 
		{
			state = 1;
		}
		close (pib.file);
		argc--;
		argv++;
	}
	return (state);
}

