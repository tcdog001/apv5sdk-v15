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
 *   nvmsplit.c - Intelon image file extractor;
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
#include <unistd.h>
#include <string.h>
#include <errno.h>

/*====================================================================*
 *   custom header files;
 *--------------------------------------------------------------------*/

#include "../tools/getoptv.h"
#include "../tools/memory.h"
#include "../tools/flags.h"
#include "../tools/files.h"
#include "../tools/error.h"
#include "../nvm/nvm.h"

/*====================================================================*
 *   custom source files;
 *--------------------------------------------------------------------*/

#ifndef MAKEFILE
#include "../tools/getoptv.c"
#include "../tools/putoptv.c"
#include "../tools/version.c"
#include "../tools/checksum_32.c"
#include "../tools/error.c"
#endif

/*====================================================================*
 *
 *   void function (const char * filename, flag_t flags);
 *
 *   open an Intellon image file and extract all components; save
 *   each one as an independent image file;
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

static void function (const char * filename, flag_t flags) 

{
	off_t offset;
	struct header_nvm header_nvm;
	byte buffer [BUFSIZ];
	signed length;
	signed image = 0;
	signed ifd;
	signed ofd;
	char savename [FILENAME_MAX];
	char * sp;
	char * cp;
	memset (&header_nvm, 0, sizeof (header_nvm));
	for (sp = cp = strcpy (savename, filename); *cp; cp++) 
	{
		if (*cp == FILE_C_EXTENDER) 
		{
			sp = cp;
		}
	}
	if (sp == savename) 
	{
		sp = cp;
	}
	if ((ifd = open (filename, O_BINARY|O_RDONLY)) == -1) 
	{
		error (1, errno, "Can't open %s", filename);
	}
	if (lseek (ifd, 0, SEEK_SET)) 
	{
		error (1, errno, "Can't rewind %s", filename);
	}
	do 
	{
		if (lseek (ifd, 0, SEEK_CUR) != LE32TOH (header_nvm.NEXTHEADER)) 
		{
			error (1, errno, "Bad file position: %s (%d)", filename, image);
		}
		if (read (ifd, &header_nvm, sizeof (header_nvm)) < sizeof (header_nvm)) 
		{
			error (1, errno, "Can't read header: %s (%d)", filename, image);
		}
		if (LE32TOH (header_nvm.HEADERVERSION) != NVM_HEADER_VERSION) 
		{
			error (1, 0, "Bad header version: %s (%d)", filename, image);
		}
		if (checksum_32 (&header_nvm, sizeof (header_nvm), 0)) 
		{
			error (1, 0, "Bad header checksum: %s (%d)", filename, image);
		}

#if defined (WIN32)

		sprintf (sp, "-%02d.nvm", image);

#else

		snprintf (sp, savename + sizeof (savename) - sp, "-%02d.nvm", image);

#endif

		if ((ofd = open (savename, O_BINARY|O_CREAT|O_RDWR|O_TRUNC, FILE_FILEMODE)) == -1) 
		{
			error (1, errno, "Can't open file: %s (%d)", savename, image);
		}
		if (_anyset (flags, NVM_VERBOSE)) 
		{
			error (0, 0, "%s --> %s", filename, savename);
		}
		offset = LE32TOH (header_nvm.NEXTHEADER);
		header_nvm.NEXTHEADER = 0;
		header_nvm.HEADERCHECKSUM = 0;
		header_nvm.HEADERCHECKSUM = checksum_32 (&header_nvm, sizeof (header_nvm), 0);
		if (write (ofd, &header_nvm, sizeof (header_nvm)) < sizeof (header_nvm)) 
		{
			error (1, errno, "Can't save header: %s (%d)", savename, image);
		}
		header_nvm.NEXTHEADER = HTOLE32 (offset);
		offset = LE32TOH (header_nvm.IMAGELENGTH);
		length = sizeof (buffer);
		while (offset) 
		{
			if (length > offset) 
			{
				length = offset;
			}
			if (read (ifd, buffer, length) < length) 
			{
				error (1, errno, "Can't read image: %s (%d)", filename, image);
			}
			if (write (ofd, buffer, length) < length) 
			{
				error (1, errno, "Can't save image: %s (%d)", savename, image);
			}
			offset -= length;
		}
		close (ofd);
		image++;
	}
	while (header_nvm.NEXTHEADER);
	close (ifd);
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
		"qv",
		"file [file] [...]",
		"Intellon image file extractor",
		"q\tsuppress messages",
		"v\tverbose messages",
		(const char *) (0) 
	};
	flag_t flags = (flag_t)(0);
	signed c;
	optind = 1;
	opterr = 1;
	while ((c = getoptv (argc, argv, optv)) != -1) 
	{
		switch (c) 
		{
		case 'q':
			_setbits (flags, NVM_SILENCE);
			break;
		case 'v':
			_setbits (flags, NVM_VERBOSE);
			break;
		default:
			break;
		}
	}
	argc -= optind;
	argv += optind;
	while ((argc) && (*argv)) 
	{
		function (*argv, flags);
		argc--;
		argv++;
	}
	return (0);
}

