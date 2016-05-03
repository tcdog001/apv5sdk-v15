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
 *   setpib.c - Command Line PIB Editor 
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
 *      Charles Maier <charles.maier@intellon.com>
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
#include "../tools/memory.h"
#include "../tools/number.h"
#include "../tools/error.h"
#include "../tools/types.h"
#include "../tools/flags.h"
#include "../tools/files.h"
#include "../pib/pib.h"

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
#include "../tools/todigit.c"
#include "../tools/hexdump.c"
#include "../tools/checksum_32.c"
#include "../tools/error.c"
#endif

/*====================================================================*
 *   constants;
 *--------------------------------------------------------------------*/

#define SETPIB_VERBOSE (1 << 0)
#define SETPIB_SILENCE (1 << 1)
#define SETPIB_HEADERS (1 << 2)
#define SETPIB_CHANGED (1 << 3)

#define SETPIB_OVERFLOW "PIB overflow at %s %s"

/*====================================================================*
 * 
 *   size_t setpib (const char * type, const char * data, byte memory [], unsigned extent);
 *
 *
 *.  Intellon INT6000 Linux Toolkit for HomePlug AV;
 *:  Published 2006-2008 by Intellon Corp. ALL RIGHTS RESERVED;
 *;  For demonstration; Not for production use;
 *
 *--------------------------------------------------------------------*/

static unsigned setpib (const char * type, const char * data, byte memory [], unsigned extent) 

{
	if (!strcmp (type, "byte")) 
	{
		uint8_t * number = (uint8_t *)(memory);
		if (extent < sizeof (uint8_t)) 
		{
			error (1, ECANCELED, "Overflow at %s %s", type, data);
		}
		*number = (uint8_t)(basespec (data, 0, sizeof (uint8_t)));
		return (sizeof (uint8_t));
	}
	if (!strcmp (type, "word")) 
	{
		uint16_t * number = (uint16_t *)(memory);
		if (extent < sizeof (uint16_t)) 
		{
			error (1, ECANCELED, "Overflow at %s %s", type, data);
		}
		* number = (uint16_t)(basespec (data, 0, sizeof (uint16_t)));

#if __BYTE_ORDER == BIG_ENDIAN

		endian (number, sizeof (uint16_t));

#endif

		return (sizeof (uint16_t));
	}
	if (!strcmp (type, "long")) 
	{
		uint32_t * number = (uint32_t *)(memory);
		if (extent < sizeof (uint32_t)) 
		{
			error (1, ECANCELED, "Overflow at %s %s", type, data);
		}
		* number = (uint32_t)(basespec (data, 0, sizeof (uint32_t)));

#if __BYTE_ORDER == BIG_ENDIAN

		endian (number, sizeof (uint32_t));

#endif

		return (sizeof (uint32_t));
	}
	if (!strcmp (type, "huge")) 
	{
		uint64_t * number = (uint64_t *)(memory);
		if (extent < sizeof (uint64_t)) 
		{
			error (1, ECANCELED, "Overflow at %s %s", type, data);
		}
		* number = (uint64_t)(basespec (data, 0, sizeof (uint64_t)));

#if __BYTE_ORDER == BIG_ENDIAN

		endian (number, sizeof (uint64_t));

#endif

		return (sizeof (uint64_t));
	}
	if (!strcmp (type, "hfid")) 
	{
		char * string = (char *)(memory);
		if (extent < PIB_HFID_LEN) 
		{
			error (1, ECANCELED, "Overflow at %s %s", type, data);
		}
		memset (string, 0, PIB_HFID_LEN);
		strncpy (string, data, PIB_HFID_LEN - 1);
		return (PIB_HFID_LEN);
	}
	if (!strcmp (type, "key")) 
	{
		extent = (unsigned)(bytespec (data, memory, 16));
		return (extent);
	}
	if (!strcmp (type, "hex")) 
	{
		extent = (unsigned)(dataspec (data, memory, extent));
		return (extent);
	}
	if (!strcmp (type, "data")) 
	{
		extent = (unsigned)(dataspec (data, memory, extent));
		return (extent);
	}
	if (!strcmp (type, "zero")) 
	{
		extent = (unsigned)(uintspec (data, 0, extent));
		memset (memory, 0, extent);
		return (extent);
	}
	if (!strcmp (type, "fill")) 
	{
		extent = (unsigned)(uintspec (data, 0, extent));
		memset (memory, ~0, extent);
		return (extent);
	}
	if (!strcmp (type, "skip")) 
	{
		extent = (unsigned)(uintspec (data, 0, extent));
		return (extent);
	}
	error (1, ENOTSUP, "%s", type);
	return (0);
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
		"hqvw:x",
		"file offset [format data] [format data] [...]\n\n\t  where types are 'byte'|'word'|'long'|'huge'|'data'|'hfid'|'zero'|'fill'|'skip'",
		"PIB File Editor",
		"q\tquiet mode",
		"v[v]\tverbose mode",
		"w n\twindow size is n [32]",
		"x\trepair checksum",
		(const char *) (0) 
	};
	struct _file_ file;
	struct header_pib * header;
	unsigned window = 32;
	unsigned offset = 0;
	unsigned extent = 0;
	byte * buffer;
	byte * memory;
	flag_t flags = (flag_t)(0);
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
	if (!argc) 
	{
		error (1, ENOTSUP, "Need a file to edit");
	}
	file.name = *argv;
	if ((file.file = open (file.name, O_BINARY|O_RDONLY)) == -1) 
	{
		error (1, errno, "%s", file.name);
	}
	if ((extent = lseek (file.file, 0, SEEK_END)) == -1) 
	{
		error (1, errno, "%s", file.name);
	}
	if (!(buffer = malloc (extent))) 
	{
		error (1, errno, "%s", file.name);
	}
	if (lseek (file.file, 0, SEEK_SET)) 
	{
		error (1, errno, "%s", file.name);
	}
	if (read (file.file, buffer, extent) < (signed)(extent)) 
	{
		error (1, errno, "%s", file.name);
	}
	close (file.file);
	argc--;
	argv++;
	header = (struct header_pib *)(buffer);
	if (extent != LE16TOH (header->PIBLENGTH)) 
	{
		error (1, ECANCELED, "Bad PIB filesize: %s", file.name);
	}
	if (_allclr (flags, SETPIB_CHANGED)) 
	{
		if (checksum_32 (buffer, LE16TOH (header->PIBLENGTH), 0)) 
		{
			error (1, ECANCELED, "Bad PIB checksum: %s", file.name);
		}
	}
	if (!argc) 
	{
		error (1, ENOTSUP, "Need an offset");
	}
	offset = (uint32_t)(basespec (*argv, 16, sizeof (uint32_t)));
	if (offset >= extent) 
	{
		error (1, ECANCELED, "PIB offset %X exceeds PIB extent %X", offset, extent);
	}
	memory = buffer + offset;
	argc--;
	argv++;
	if (!argc) 
	{
		_setbits (flags, SETPIB_VERBOSE);
	}
	while ((argc > 1) && (*argv)) 
	{
		_setbits (flags, SETPIB_CHANGED);
		memory += setpib (argv [0], argv [1], memory, (unsigned)(buffer + extent - memory));
		argc -= 2;
		argv += 2;
	}
	if (argc) 
	{
		error (1, ECANCELED, "%s needs a value", *argv);
	}
	if (_anyset (flags, SETPIB_CHANGED)) 
	{
		header->CHECKSUM = 0;
		header->CHECKSUM = checksum_32 (buffer, extent, 0);
		if ((file.file = open (file.name, O_BINARY|O_CREAT|O_RDWR|O_TRUNC, FILE_FILEMODE)) == -1) 
		{
			error (1, errno, "%s", file.name);
		}
		if (write (file.file, buffer, extent) < (signed)(extent)) 
		{
			error (1, errno, "%s", file.name);
		}
		close (file.file);
	}
	if (_anyset (flags, SETPIB_VERBOSE)) 
	{
		if ((memory - buffer) == offset) 
		{
			memory = buffer + offset + 1;
		}
		if (window < offset) 
		{
			offset -= window;
		}
		else 
		{
			offset = 0;
		}
		if (memory - buffer + window < extent) 
		{
			extent = (unsigned)(memory - buffer) + window;
		}
		if (_anyset (flags, SETPIB_HEADERS)) 
		{
			printf (HEXDUMP_HEADER);
		}
		hexdump (buffer, offset, extent, stdout);
	}
	return (0);
}

