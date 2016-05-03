/*====================================================================*
 *   
 *   Copyright (c) 2011 by Qualcomm Atheros.
 *   
 *   Permission to use, copy, setpib, and/or distribute this software 
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

#define _GETOPT_H

/*====================================================================*"
 *   system header files;
 *--------------------------------------------------------------------*/

#include <unistd.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <ctype.h>

/*====================================================================*
 *   custom header files;
 *--------------------------------------------------------------------*/

#include "../tools/getoptv.h"
#include "../tools/putoptv.h"
#include "../tools/memory.h"
#include "../tools/number.h"
#include "../tools/error.h"
#include "../tools/types.h"
#include "../tools/flags.h"
#include "../tools/files.h"
#include "../pib/pib.h"
#include "../nvm/nvm.h"

/*====================================================================*
 *   custom source files;
 *--------------------------------------------------------------------*/

#ifndef MAKEFILE
#include "../tools/getoptv.c"
#include "../tools/putoptv.c"
#include "../tools/version.c"
#include "../tools/uintspec.c"
#include "../tools/basespec.c"
#include "../tools/dataspec.c"
#include "../tools/bytespec.c"
#include "../tools/todigit.c"
#include "../tools/hexdump.c"
#include "../tools/hexpeek.c"
#include "../tools/fdchecksum32.c"
#include "../tools/checksum32.c"
#include "../tools/memencode.c"
#include "../tools/error.c"
#endif

/*====================================================================*
 *   constants;
 *--------------------------------------------------------------------*/

#define SETPIB_VERBOSE (1 << 0)
#define SETPIB_SILENCE (1 << 1)
#define SETPIB_HEADERS (1 << 2)
#define SETPIB_CHANGED (1 << 3)
#define SETPIB_WINDOW 32

/*====================================================================*
 *   variables;
 *--------------------------------------------------------------------*/

static flag_t flags = (flag_t)(0);

/*====================================================================*
 *
 *   signed setpib (int argc, char const * argv [], byte buffer [], unsigned length, unsigned window) 
 *
 *   apply a series of edits to a memory region; edits are specified
 *   by a string vector that follows rules understood by memencode();
 *   the parameter file is not rewritten unless data changed; 
 *
 *   this implementation may not edit a parameter block having a bad
 *   checksum to start with; the user can override this feature;
 *
 *.  Qualcomm Atheros HomePlug AV Powerline Toolkit
 *:  Published 2009-2011 by Qualcomm Atheros. ALL RIGHTS RESERVED
 *;  For demonstration and evaluation only. Not for production use
 *
 *   Contributor(s):
 *	Charles Maier <cmaier@qualcomm.com>
 *
 *--------------------------------------------------------------------*/

static signed setpib (int argc, char const * argv [], byte buffer [], unsigned length, unsigned window) 

{
	unsigned origin;
	unsigned offset;
	if (!argc) 
	{
		error (1, ENOTSUP, "Need an offset");
	}
	origin = offset = (unsigned)(basespec (* argv, 16, sizeof (offset)));
	if (offset >= length) 
	{
		error (1, ECANCELED, "Offset %X exceeds file length of %d", offset, length);
	}
	argc--;
	argv++;
	if (!argc) 
	{
		_setbits (flags, SETPIB_VERBOSE);
	}
	while ((argc > 1) && (* argv)) 
	{
		_setbits (flags, SETPIB_CHANGED);
		offset += (unsigned)(memencode (buffer + offset, length - offset, argv [0], argv [1]));
		argc -= 2;
		argv += 2;
	}
	if (argc) 
	{
		error (1, ECANCELED, "%s needs a value", * argv);
	}
	if (_anyset (flags, SETPIB_VERBOSE)) 
	{
		hexpeek (buffer, origin, offset, length, window, stdout);
	}
	return (0);
}


/*====================================================================*
 *
 *   signed pibimage1 (file * pib, int argc, char const * argv [], unsigned window);
 * 
 *   read an entire flat parameter file into memory, edit it, save 
 *   it and display it;
 *
 *.  Qualcomm Atheros HomePlug AV Powerline Toolkit
 *:  Published 2009-2011 by Qualcomm Atheros. ALL RIGHTS RESERVED
 *;  For demonstration and evaluation only. Not for production use
 *
 *   Contributor(s):
 *	Charles Maier <cmaier@qualcomm.com>
 *
 *--------------------------------------------------------------------*/

static signed pibimage1 (file * pib, int argc, char const * argv [], unsigned window) 

{
	off_t length;
	byte * buffer;
	if ((length = lseek (pib->file, 0, SEEK_END)) == -1) 
	{
		error (1, errno, FILE_CANTSIZE, pib->name);
	}
	if (!(buffer = malloc (length))) 
	{
		error (1, errno, FILE_CANTLOAD, pib->name);
	}
	if (lseek (pib->file, 0, SEEK_SET)) 
	{
		error (1, errno, FILE_CANTHOME, pib->name);
	}
	if (read (pib->file, buffer, length) != length) 
	{
		error (1, errno, FILE_CANTREAD, pib->name);
	}
	if (lseek (pib->file, 0, SEEK_SET)) 
	{
		error (1, errno, FILE_CANTHOME, pib->name);
	}
	if (setpib (argc, argv, buffer, length, window)) 
	{
		error (1, errno, FILE_CANTEDIT, pib->name);
	}
	if (_anyset (flags, SETPIB_CHANGED)) 
	{
		struct pib_header * pib_header = (struct pib_header *)(buffer);
		pib_header->CHECKSUM = checksum32 (buffer, length, pib_header->CHECKSUM);
		if (write (pib->file, buffer, length) != length) 
		{
			error (1, errno, FILE_CANTSAVE, pib->name);
		}
		if (lseek (pib->file, (off_t)(0)-length, SEEK_CUR) == -1) 
		{
			error (1, errno, FILE_CANTHOME, pib->name);
		}
	}
	free (buffer);
	close (pib->file);
	return (0);
}


/*====================================================================*
 *
 *   signed pibchain2 (file * pib, signed argc, char const * argv [], unsigned window) 
 *
 *   search a panther/lynx image file for a PIB image then read the 
 *   entire image into memory, edit it, save it and show it;
 *
 *.  Qualcomm Atheros HomePlug AV Powerline Toolkit
 *:  Published 2009-2011 by Qualcomm Atheros. ALL RIGHTS RESERVED
 *;  For demonstration and evaluation only. Not for production use
 *   
 *   Contributor(s):
 *	Charles Maier <cmaier@qualcomm.com>
 *
 *--------------------------------------------------------------------*/

static signed pibchain2 (file * pib, signed argc, char const * argv [], unsigned window) 

{
	struct nvm_header2 nvm_header;
	unsigned module = 0;
	unsigned origin = ~0;
	unsigned offset = 0;
	do 
	{
		if (read (pib->file, &nvm_header, sizeof (nvm_header)) != sizeof (nvm_header)) 
		{
			error (1, errno, NVM_HDR_CANTREAD, pib->name, module);
		}
		if (LE16TOH (nvm_header.MajorVersion) != 1) 
		{
			error (1, 0, NVM_HDR_VERSION, pib->name, module);
		}
		if (LE16TOH (nvm_header.MinorVersion) != 1) 
		{
			error (1, 0, NVM_HDR_VERSION, pib->name, module);
		}
		if (checksum32 (&nvm_header, sizeof (nvm_header), 0)) 
		{
			error (1, 0, NVM_HDR_CHECKSUM, pib->name, module);
		}
		if (LE32TOH (nvm_header.PrevHeader) != origin) 
		{
			error (1, 0, NVM_HDR_LINK, pib->name, module);
		}
		if (LE32TOH (nvm_header.ImageType) == NVM_IMAGE_PIB) 
		{
			long length = LE32TOH (nvm_header.ImageLength);
			byte * buffer = malloc (length);
			if (!buffer) 
			{
				error (1, errno, FILE_CANTLOAD, pib->name);
			}
			if (read (pib->file, buffer, length) != length) 
			{
				error (1, errno, FILE_CANTREAD, pib->name);
			}
			if (lseek (pib->file, (off_t)(0)-length, SEEK_CUR) == -1) 
			{
				error (1, errno, FILE_CANTHOME, pib->name);
			}
			if (setpib (argc, argv, buffer, length, window)) 
			{
				error (1, errno, FILE_CANTEDIT, pib->name);
			}
			if (_anyset (flags, SETPIB_CHANGED)) 
			{
				nvm_header.ImageChecksum = checksum32 (buffer, length, 0);
				if (write (pib->file, buffer, length) != length) 
				{
					error (1, errno, FILE_CANTSAVE, pib->name);
				}
				if (lseek (pib->file, (off_t)(0)-length, SEEK_CUR) == -1) 
				{
					error (1, errno, FILE_CANTHOME, pib->name);
				}
				nvm_header.HeaderChecksum = checksum32 (&nvm_header, sizeof (nvm_header), nvm_header.HeaderChecksum);
				if (lseek (pib->file, (off_t)(0)-sizeof (nvm_header), SEEK_CUR) == -1) 
				{
					error (1, errno, FILE_CANTHOME, pib->name);
				}
				if (write (pib->file, &nvm_header, sizeof (nvm_header)) != sizeof (nvm_header)) 
				{
					error (1, errno, FILE_CANTSAVE, pib->name);
				}
				if (lseek (pib->file, (off_t)(0)-sizeof (nvm_header), SEEK_CUR) == -1) 
				{
					error (1, errno, FILE_CANTHOME, pib->name);
				}
			}
			free (buffer);
			return (0);
		}
		if (fdchecksum32 (pib->file, LE32TOH (nvm_header.ImageLength), nvm_header.ImageChecksum)) 
		{
			error (1, 0, NVM_IMG_CHECKSUM, pib->name, module);
		}
		origin = offset;
		offset = LE32TOH (nvm_header.NextHeader);
		module++;
	}
	while (~nvm_header.NextHeader);
	return (-1);
}


/*====================================================================*
 *
 *   signed function (int argc, char const * argv [], unsigned window);
 *
 *   call an appropriate parameter edit function based on the file 
 *   header;
 *
 *   older parameter files are flat with their own header; newer ones
 *   are image chains where one of image contains the parameter block;
 *
 *.  Qualcomm Atheros HomePlug AV Powerline Toolkit
 *:  Published 2009-2011 by Qualcomm Atheros. ALL RIGHTS RESERVED
 *;  For demonstration and evaluation only. Not for production use
 *   
 *   Contributor(s):
 *	Charles Maier <cmaier@qualcomm.com>
 *
 *--------------------------------------------------------------------*/

static signed function (int argc, char const * argv [], unsigned window) 

{
	struct _file_ pib;
	uint32_t version;
	signed status;
	pib.name = * argv;
	if ((pib.file = open (pib.name, O_BINARY|O_RDWR)) == -1) 
	{
		error (1, errno, FILE_CANTOPEN, pib.name);
	}
	if (read (pib.file, &version, sizeof (version)) != sizeof (version)) 
	{
		error (1, errno, FILE_CANTREAD, pib.name);
	}
	if (lseek (pib.file, 0, SEEK_SET)) 
	{
		error (1, errno, FILE_CANTHOME, pib.name);
	}
	argc--;
	argv++;
	if (LE32TOH (version) == 0x00010001) 
	{
		status = pibchain2 (&pib, argc, argv, window);
	}
	else 
	{
		status = pibimage1 (&pib, argc, argv, window);
	}
	close (pib.file);
	return (status);
}


/*====================================================================*
 *
 *   int main (int argc, char const * argv []);
 *   
 *.  Qualcomm Atheros HomePlug AV Powerline Toolkit
 *:  Published 2009-2011 by Qualcomm Atheros. ALL RIGHTS RESERVED
 *;  For demonstration and evaluation only. Not for production use
 *
 *--------------------------------------------------------------------*/

int main (int argc, char const * argv []) 

{
	static char const * optv [] = 
	{
		"qvw:x",
		"file base [type data] [type data] [...]\n\n\ttype :== 'byte'|'word'|'long'|'huge'|'text'|'data'|'zero'|'fill'|'skip'|'hfid'|'mac'|'key'",
		"Qualcomm Atheros PIB File Editor",
		"q\tquiet mode",
		"v[v]\tverbose mode",
		"w n\twindow size is (n) [" OPTSTR (SETPIB_WINDOW) "]",
		"x\trepair checksum",
		(char const *) (0)
	};
	unsigned window = SETPIB_WINDOW;
	signed c;
	optind = 1;
	opterr = 1;
	while ((c = getoptv (argc, argv, optv)) != -1) 
	{
		switch (c) 
		{
		case 'q':
			_setbits (flags, SETPIB_SILENCE);
			break;
		case 'v':
			if (_anyset (flags, SETPIB_VERBOSE)) 
			{
				_setbits (flags, SETPIB_HEADERS);
			}
			_setbits (flags, SETPIB_VERBOSE);
			break;
		case 'w':
			window = (unsigned)(uintspec (optarg, 0, UINT_MAX));
			_setbits (flags, SETPIB_VERBOSE);
			break;
		case 'x':
			_setbits (flags, SETPIB_CHANGED);
			break;
		default:
			break;
		}
	}
	argc -= optind;
	argv += optind;
	if ((argc) && (* argv)) 
	{
		function (argc, argv, window);
	}
	return (0);
}

