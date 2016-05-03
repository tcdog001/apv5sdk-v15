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
 *      Abdel Younes <younes@leacom.fr>
 *
 *--------------------------------------------------------------------*/

#define _GETOPT_H

/*====================================================================*
 *   system header files;
 *--------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
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
#include "../tools/strfbits.c"
#include "../tools/filepart.c"
#include "../tools/error.c"
#endif

#ifndef MAKEFILE
#include "../nvm/nvmpeek.c"
#endif

/*====================================================================*
 *
 *   signed chknvm2 (const void * memory, const char * title, flag_t flags) 
 *
 *   nvm.h
 *
 *   return the extent of an NVM file image in memory by walking the
 *   NVM header chain to the end;
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
 *      Abdel Younes <younes@leacom.fr>
 *
 *--------------------------------------------------------------------*/

static signed chknvm2 (const void * memory, const char * title, flag_t flags) 

{
	struct header_nvm * header_nvm;
	const byte * origin = (byte *)(memory);
	const byte * offset = (byte *)(memory);
	unsigned image = 0;
	do 
	{
		header_nvm = (struct header_nvm *)(offset);
		if (LE32TOH (header_nvm->HEADERVERSION) != NVM_HEADER_VERSION) 
		{
			error (0, errno, "Bad header version: %s (%d)", title, image);
			return (0);
		}
		if (checksum_32 (header_nvm, sizeof (struct header_nvm), 0)) 
		{
			error (0, errno, "Bad header checksum: %s (%d)", title, image);
			return (0);
		}
		offset += sizeof (struct header_nvm);
		if (_anyset (flags, NVM_VERBOSE)) 
		{
			printf ("------- %s (%d) -------\n", title, image);
			nvmpeek (header_nvm);
		}
		if (_anyset (flags, NVM_VERSION) && (header_nvm->IMAGETYPE == NVM_IMAGE_FIRMWARE)) 
		{
			printf ("%s (%d) %s\n", title, image, offset);
		}
		if (checksum_32 (offset, LE32TOH (header_nvm->IMAGELENGTH), header_nvm->IMAGECHECKSUM)) 
		{
			error (1, errno, "Bad image checksum: %s (%d)", title, image);
			return (0);
		}
		offset += header_nvm->IMAGELENGTH;
		image++;
	}
	while (header_nvm->NEXTHEADER);
	return (offset - origin);
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
		"nqrsv",
		"file [file] [...]",
		"Intellon image file validator",
		"r\tprint firmware revision string",
		"q\tquiet",
		"v\tverbose messages",
		(const char *) (0) 
	};
	void * buffer;
	signed length;
	signed extent;
	signed status = 0;
	signed fd;
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
		if ((fd = open (*argv, O_BINARY|O_RDONLY)) == -1) 
		{
			error (0, errno, "Can't open %s", *argv);
			status = 1;
		}
		else if ((length = lseek (fd, 0, SEEK_END)) == -1) 
		{
			error (0, errno, "Can't size %s", *argv);
			status = 1;
		}
		else if (!(buffer = malloc (length))) 
		{
			error (0, errno, "Can't allocate buffer");
			status = 1;
		}
		else if (lseek (fd, 0, SEEK_SET)) 
		{
			error (0, errno, "Can't home %s", *argv);
			status = 1;
		}
		else if (read (fd, buffer, length) < length) 
		{
			error (0, errno, "Can't read %s", *argv);
			status = 1;
		}
		else if (!(extent = chknvm2 (buffer, filepart (*argv), flags))) 
		{
			error (0, 0, "%s is corrupt", *argv);
			status = 1;
		}
		else if (_allclr (flags, (NVM_VERBOSE|NVM_SILENCE|NVM_VERSION|NVM_SDRAM))) 
		{
			printf ("%s looks good\n", filepart (*argv));
		}
		close (fd);
		argc--;
		argv++;
	}
	return (status);
}

