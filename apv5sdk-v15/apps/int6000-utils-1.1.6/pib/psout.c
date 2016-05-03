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
 *   psout.c - extract prescalers from int6000 parameter file;
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
#include "../tools/chars.h"
#include "../tools/types.h"
#include "../tools/error.h"
#include "../tools/files.h"
#include "../pib/pib.h"

/*====================================================================*
 *   custom source files;
 *--------------------------------------------------------------------*/

#ifndef MAKEFILE
#include "../tools/getoptv.c"
#include "../tools/putoptv.c"
#include "../tools/version.c"
#include "../tools/fdchecksum_32.c"
#include "../tools/filepart.c"
#include "../tools/error.c"
#endif

#ifndef MAKEFILE
#include "../pib/pibfile.c"
#endif

/*====================================================================*
 *   program constants;
 *--------------------------------------------------------------------*/

#define PSOUT_VERBOSE (1 << 0)
#define PSOUT_SILENCE (1 << 1)
#define PSOUT_ALLTONE (1 << 2)

/*====================================================================*
 *
 *   static signed psout (struct _file_ * pib);
 *
 *.  Intellon INT6000 Linux Toolkit for HomePlug AV;
 *:  Published 2006-2008 by Intellon Corp. ALL RIGHTS RESERVED;
 *;  For demonstration; Not for production use;
 *
 *--------------------------------------------------------------------*/

static signed psout (struct _file_ * pib, flag_t flags) 

{
	uint32_t offset = 0;
	uint32_t scalar;
	if (lseek (pib->file, PRESCALAR_OFFSET, SEEK_SET) != PRESCALAR_OFFSET) 
	{
		error (0, errno, "Can't seek %s", filepart (pib->name));
		return (-1);
	}
	while (offset < PRESCALAR_MEMBER) 
	{
		if (read (pib->file, &scalar, sizeof (scalar)) < sizeof (scalar)) 
		{
			error (0, errno, "Can't read %s", filepart (pib->name));
			return (-1);
		}
		printf ("%08d %08X\n", offset, scalar);
		offset++;
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
		"pibfile [> scalars]",
		"extract prescalers from int6000 parameter file",
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
		error (1, ECANCELED, "Only one source file allowed");
	}
	if ((argc) && (*argv)) 
	{
		pib.name = *argv;
		if ((pib.file = open (pib.name, O_BINARY|O_RDONLY)) == -1) 
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
		else if (psout (&pib, flags)) 
		{
			state = 1;
		}
		close (pib.file);
		argc--;
		argv++;
	}
	return (state);
}

