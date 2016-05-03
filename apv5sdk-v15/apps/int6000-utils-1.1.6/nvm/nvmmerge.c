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
 *   nvmmerge.c - Intellon image file splicer;
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
 *   void function (const char * filename, signed index, flag_t flags);
 *
 *   concatenate Intellon image files;
 *
 *   open an NVM file and validate it by walking the header and image
 *   chain and validating all checksums; rewind valid files; return 0
 *   on success or -1 on error;
 *
 *   the checksum of the entire header, including header checksum, is
 *   always 0 for valid headers; similarly, the checksum of the image
 *   and image checksum is always 0 for valid images;
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

static void function (const char * filename, signed index, flag_t flags) 

{
	struct header_nvm header_nvm;
	byte buffer [BUFSIZ];
	signed offset = 0;
	signed length;
	signed image = 0;
	signed ifd;
	memset (&header_nvm, 0, sizeof (header_nvm));
	if ((ifd = open (filename, O_BINARY|O_RDONLY)) == -1) 
	{
		error (1, errno, "Can't open %s", filename);
	}
	if (_anyset (flags, NVM_VERBOSE)) 
	{
		error (0, 0, "%s", filename);
	}
	do 
	{
		if (lseek (ifd, 0, SEEK_CUR) != LE32TOH (header_nvm.NEXTHEADER)) 
		{
			error (1, 0, "Bad file position: %s (%d)", filename, image);
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
		offset = LE32TOH (header_nvm.NEXTHEADER);
		if ((index) || (offset)) 
		{
			uint32_t offset = 0;
			offset += lseek (STDOUT_FILENO, 0, SEEK_CUR);
			offset += sizeof (header_nvm);
			offset += LE32TOH (header_nvm.IMAGELENGTH);
			header_nvm.NEXTHEADER = HTOLE32 (offset);
		}
		header_nvm.HEADERCHECKSUM = 0;
		header_nvm.HEADERCHECKSUM = checksum_32 (&header_nvm, sizeof (header_nvm), 0);
		if (write (STDOUT_FILENO, &header_nvm, sizeof (header_nvm)) < sizeof (header_nvm)) 
		{
			error (1, errno, "Can't save header: %s (%d)", filename, image);
		}
		header_nvm.NEXTHEADER = HTOLE32 (offset);
		offset = LE32TOH (header_nvm.IMAGELENGTH);
		length = sizeof (buffer);
		while (offset) 
		{
			if (length > (signed)(offset)) 
			{
				length = offset;
			}
			if ((length = read (ifd, buffer, length)) < length) 
			{
				error (1, errno, "Can't read image: %s (%d)", filename, image);
			}
			if (write (STDOUT_FILENO, buffer, length) < length) 
			{
				error (1, errno, "Can't save image: %s (%d)", filename, image);
			}
			offset -= length;
		}
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
		"file [file] [...] [> file]",
		"Intellon image file splicer",
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
		switch ((char) (c)) 
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

#ifdef WIN32

	setmode (fileno (stdin), O_BINARY);
	setmode (fileno (stdout), O_BINARY);

#endif

	while ((argc) && (*argv)) 
	{
		function (*argv, argc-1, flags);
		argv++;
		argc--;
	}
	return (0);
}

