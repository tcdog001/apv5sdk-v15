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
 *   chknvm.c - Intellon image file validator;
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
#include <memory.h>
#include <fcntl.h>
#include <errno.h>

/*====================================================================*
 *   custom header files;
 *--------------------------------------------------------------------*/

#include "../tools/getoptv.h"
#include "../tools/memory.h"
#include "../tools/flags.h"
#include "../tools/error.h"
#include "../tools/files.h"
#include "../ram/sdram.h"
#include "../nvm/nvm.h"

/*====================================================================*
 *   custom source files;
 *--------------------------------------------------------------------*/

#ifndef MAKEFILE
#include "../tools/getoptv.c"
#include "../tools/putoptv.c"
#include "../tools/version.c"
#include "../tools/checksum_32.c"
#include "../tools/fdchecksum_32.c"
#include "../tools/alert.c"
#include "../tools/error.c"
#include "../tools/strfbits.c"
#include "../tools/filepart.c"
#endif

#ifndef MAKEFILE
#include "../ram/sdrampeek.c"
#endif

#ifndef MAKEFILE
#include "../nvm/nvmpeek.c"
#endif

/*====================================================================*
 *
 *   int chknvm (struct _file_ * file, flag_t flags);
 *
 *   nvm.h
 *
 *   open an NVM file and validate it by walking headers and images
 *   to check lengths and checksums; return 0 on success or -1 on
 *   error;
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

static signed chknvm (struct _file_ * file, flag_t flags) 

{
	struct header_nvm header_nvm;
	struct config_ram config_ram;
	unsigned image = 0;
	memset (&header_nvm, 0, sizeof (header_nvm));
	if (lseek (file->file, 0, SEEK_SET)) 
	{
		if (_allclr (flags, NVM_SILENCE)) 
		{
			error (0, errno, "Can't rewind file: %s", file->name);
		}
		return (-1);
	}
	do 
	{
		if (lseek (file->file, 0, SEEK_CUR) != LE32TOH (header_nvm.NEXTHEADER)) 
		{
			printf ("offset=%ld header=%d\n", (long)(lseek (file->file, 0, SEEK_CUR)), LE32TOH (header_nvm.NEXTHEADER));
			if (_allclr (flags, NVM_SILENCE)) 
			{
				error (0, 0, "Bad file position: %s (%d)", file->name, image);
			}
			return (-1);
		}
		if (read (file->file, &header_nvm, sizeof (struct header_nvm)) < sizeof (struct header_nvm)) 
		{
			if (_allclr (flags, NVM_SILENCE)) 
			{
				error (0, errno, "Can't read image header: %s (%d)", file->name, image);
			}
			return (-1);
		}
		if (header_nvm.HEADERVERSION != HTOLE32 (NVM_HEADER_VERSION)) 
		{
			if (_allclr (flags, NVM_SILENCE)) 
			{
				alert ("Bad header version: %s (%d)", file->name, image);
			}
			return (-1);
		}
		if (checksum_32 (&header_nvm, sizeof (struct header_nvm), 0)) 
		{
			if (_allclr (flags, NVM_SILENCE)) 
			{
				alert ("Bad header checksum: %s (%d)", file->name, image);
			}
			return (-1);
		}
		if (_anyset (flags, NVM_VERBOSE)) 
		{
			printf ("------- %s (%d) -------\n", file->name, image);
			nvmpeek (&header_nvm);
		}
		if (header_nvm.HEADERMINORVERSION) 
		{
			if (header_nvm.IMAGETYPE == NVM_IMAGE_CONFIG_SYNOPSIS) 
			{
				if (_anyset (flags, NVM_SDRAM)) 
				{
					printf ("------- %s (%d) -------\n", file->name, image);
					read (file->file, &config_ram, sizeof (config_ram));
					lseek (file->file, 0 - sizeof (config_ram), SEEK_CUR);
					sdrampeek (&config_ram);
				}
			}
			else if (header_nvm.IMAGETYPE == NVM_IMAGE_FIRMWARE) 
			{
				if (_anyset (flags, NVM_VERSION)) 
				{
					uint8_t revision [256];
					read (file->file, revision, sizeof (revision));
					lseek (file->file, 0 - sizeof (revision), SEEK_CUR);
					printf ("%s (%d) %s\n", file->name, image, revision);
				}
			}
		}
		else if (!image) 
		{
			if (_anyset (flags, NVM_SDRAM)) 
			{
				printf ("------- %s (%d) -------\n", file->name, image);
				read (file->file, &config_ram, sizeof (config_ram));
				lseek (file->file, 0 - sizeof (config_ram), SEEK_CUR);
				sdrampeek (&config_ram);
			}
		}
		else if (!header_nvm.NEXTHEADER) 
		{
			if (_anyset (flags, NVM_VERSION)) 
			{
				uint8_t revision [256];
				read (file->file, revision, sizeof (revision));
				printf ("%s (%d) %s\n", file->name, image, revision);
				lseek (file->file, 0 - sizeof (revision), SEEK_CUR);
			}
		}
		if (fdchecksum_32 (file->file, LE32TOH (header_nvm.IMAGELENGTH), header_nvm.IMAGECHECKSUM)) 
		{
			if (_allclr (flags, NVM_SILENCE)) 
			{
				alert ("Bad image checksum: %s (%d)", file->name, image);
			}
			return (-1);
		}
		image++;
	}
	while (header_nvm.NEXTHEADER);
	if (lseek (file->file, 0, SEEK_CUR) != lseek (file->file, 0, SEEK_END)) 
	{
		if (_allclr (flags, NVM_SILENCE)) 
		{
			error (0, errno, "Bad header chain: %s (%d)", file->name, image);
		}
		return (-1);
	}
	if (lseek (file->file, 0, SEEK_SET)) 
	{
		if (_allclr (flags, NVM_SILENCE)) 
		{
			error (0, errno, "Can't rewind file: %s", file->name);
		}
		return (-1);
	}
	if (_allclr (flags, (NVM_VERBOSE|NVM_SILENCE|NVM_VERSION|NVM_SDRAM))) 
	{
		alert ("%s looks good", file->name);
	}
	return (0);
}

/*====================================================================*
 *   
 *   int main (int argc, const char * argv []);
 *   
 *   
 *--------------------------------------------------------------------*/

int main (int argc, const char * argv []) 

{
	static const char *optv [] = 
	{
		"qrsv",
		"file [file] [...]",
		"Intellon image file validator",
		"r\tprint firmware revision strings",
		"s\tprint SDRAM configuration blocks",
		"q\tquiet",
		"v\tverbose messages",
		(const char *) (0) 
	};
	struct _file_ nvm;
	signed state = 0;
	flag_t flags = (flag_t)(0);
	signed c;
	optind = 1;
	opterr = 1;
	while ((c = getoptv (argc, argv, optv)) != -1) 
	{
		switch ((char) (c)) 
		{
		case 'r':
			_setbits (flags, NVM_VERSION);
			break;
		case 's':
			_setbits (flags, NVM_SDRAM);
			break;
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
		nvm.name = *argv;
		if ((nvm.file = open (nvm.name, O_BINARY|O_RDONLY)) == -1) 
		{
			error (0, errno, "Can't open %s", nvm.name);
			state = 1;
		}
		else if (chknvm (&nvm, flags)) 
		{
			state = 1;
		}
		close (nvm.file);
		argc--;
		argv++;
	}
	return (state);
}

