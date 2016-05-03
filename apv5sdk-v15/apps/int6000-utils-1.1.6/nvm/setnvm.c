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
 *   setnvm.c - 
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
#include "../tools/number.h"
#include "../tools/flags.h"
#include "../tools/error.h"
#include "../tools/files.h"
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
#include "../tools/basespec.c"
#include "../tools/strfbits.c"
#include "../tools/todigit.c"
#include "../tools/error.c"
#include "../tools/alert.c"
#endif

#ifndef MAKEFILE
#include "../nvm/nvmpeek.c"
#endif

/*====================================================================*
 *
 *   void function (const char * filename, uint8_t type, uint16_t omit, flag_t flags) 
 *
 *   open an NVM file and change the image-type and host-mask field
 *   for every image in the file;
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

static void function (const char * filename, uint8_t type, uint16_t omit, flag_t flags) 

{
	struct header_nvm header_nvm;
	signed image = 0;
	signed ifd;
	memset (&header_nvm, 0, sizeof (header_nvm));
	if ((ifd = open (filename, O_BINARY|O_CREAT|O_RDWR)) == -1) 
	{
		error (1, errno, "Can't open %s", filename);
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
			error (1, errno, "Bad header version: %s (%d)", filename, image);
		}
		if (checksum_32 (&header_nvm, sizeof (header_nvm), 0)) 
		{
			error (1, errno, "Bad header checksum: %s (%d)", filename, image);
		}
		if (type) 
		{
			header_nvm.HEADERMINORVERSION = NVM_HEADER_MINOR_VERSION;
			header_nvm.IMAGETYPE = type;
		}
		if (omit) 
		{
			header_nvm.HEADERMINORVERSION = NVM_HEADER_MINOR_VERSION;
			header_nvm.IGNOREMASK = omit;
		}
		header_nvm.HEADERCHECKSUM = 0;
		header_nvm.HEADERCHECKSUM = checksum_32 (&header_nvm, sizeof (header_nvm), 0);
		if (_anyset (flags, NVM_VERBOSE)) 
		{
			printf ("------- %s (%d) -------\n", filename, image);
			nvmpeek (&header_nvm);
		}
		if (lseek (ifd, 0-sizeof (header_nvm), SEEK_CUR) == -1) 
		{
			error (1, errno, "Can't rewind %s", filename);
		}
		if (write (ifd, &header_nvm, sizeof (header_nvm)) < sizeof (header_nvm)) 
		{
			error (1, errno, "Can't save header: %s (%d)", filename, image);
		}
		if (fdchecksum_32 (ifd, LE32TOH (header_nvm.IMAGELENGTH), header_nvm.IMAGECHECKSUM)) 
		{
			error (1, errno, "Bad images checksum: %s (%d)", filename, image);
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
		"o:qt:v",
		"file [file] [...]",
		"Intellon image file mangler",
		"t x\timage type is (x)",
		"o x\thost mask is (x)",
		"q\tquiet mode",
		"v\tverbose mode",
		(const char *) (0) 
	};
	uint8_t type = 0;
	uint16_t omit = 0;
	flag_t flags = (flag_t)(0);
	signed c;
	optind = 1;
	opterr = 1;
	while ((c = getoptv (argc, argv, optv)) != -1) 
	{
		switch (c) 
		{
		case 't':
			type = (uint8_t)(basespec (optarg, 0, sizeof (uint8_t)));
			break;
		case 'o':
			omit = (uint16_t)(basespec (optarg, 0, sizeof (uint16_t)));
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
		function (*argv, type, omit, flags);
		argc--;
		argv++;
	}
	return (0);
}

