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
 *   getpib.c - PIB Data Extractor
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
#include "../tools/todigit.c"
#include "../tools/hexout.c"
#include "../tools/error.c"
#endif

/*====================================================================*
 *   constants;
 *--------------------------------------------------------------------*/

#define SETPIB_VERBOSE (1 << 0)
#define SETPIB_SILENCE (1 << 1)
#define SETPIB_NEWLINE (1 << 2)

#define LENGTH_MAX 1024

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
		"qvn",
		"file offset type [size]\n\n\t  standard-length types are 'byte'|'word'|'long'|'hfid'|'mac'|'key'\n\t  variable-length types are 'data'|'text'",
		"PIB Data Extractor",
		"n\tappend newline",
		"q\tquiet mode",
		"v\tverbose mode",
		(const char *) (0) 
	};
	struct _file_ file;
	struct header_pib * header;
	signed offset = 0;
	signed length = 0;
	byte * buffer;
	byte * memory;
	byte * extent;
	const char * string;
	flag_t flags = (flag_t)(0);
	signed c;
	optind = 1;
	opterr = 1;
	while ((c = getoptv (argc, argv, optv)) != -1) 
	{
		switch (c) 
		{
		case 'n':
			_setbits (flags, SETPIB_NEWLINE);
			break;
		case 'q':
			_setbits (flags, SETPIB_SILENCE);
			break;
		case 'v':
			_setbits (flags, SETPIB_VERBOSE);
			break;
		default:
			break;
		}
	}
	argc -= optind;
	argv += optind;
	if (!argc) 
	{
		error (1, 0, "No file to read");
	}
	file.name = *argv;
	if ((file.file = open (file.name, O_BINARY|O_RDONLY)) == -1) 
	{
		error (1, errno, "Can't open %s", file.name);
	}
	if ((length = lseek (file.file, 0, SEEK_END)) == -1) 
	{
		error (1, errno, "Can't size %s", file.name);
	}
	if (!(buffer = malloc (length))) 
	{
		error (1, errno, "Can't span %s", file.name);
	}
	if (lseek (file.file, 0, SEEK_SET)) 
	{
		error (1, errno, "Can't seek %s", file.name);
	}
	if (read (file.file, buffer, length) < length) 
	{
		error (1, errno, "Can't load %s", file.name);
	}
	close (file.file);
	argc--;
	argv++;
	header = (struct header_pib *)(buffer);
	if (length != LE16TOH (header->PIBLENGTH)) 
	{
		error (1, ECANCELED, "Bad PIB filesize: %s", file.name);
	}
	if (!argc) 
	{
		error (1, ECANCELED, "Need an offset");
	}
	offset = (uint32_t)(basespec (*argv, 16, sizeof (uint32_t)));
	if (offset >= length) 
	{
		error (1, ECANCELED, "PIB offset %X exceeds PIB length %X", offset, length);
	}
	memory = buffer + offset;
	extent = buffer + length;
	argc--;
	argv++;
	if (!argc) 
	{
		_setbits (flags, SETPIB_VERBOSE);
	}
	while ((argc) && (*argv)) 
	{
		const char * object = *argv;
		argv++;
		argc--;
		if (!strcmp (object, "byte")) 
		{
			uint8_t byte;
			if (memory + sizeof (byte) > extent) 
			{
				error (1, ECANCELED, "%s exceeds PIB length %X", object, length);
			}
			memcpy (&byte, memory, sizeof (byte));
			memory += sizeof (byte);
			printf ("%u", byte);
		}
		else if (!strcmp (object, "word")) 
		{
			uint16_t word;
			if (memory + sizeof (word) > extent) 
			{
				error (1, ECANCELED, "%s exceeds PIB length %X", object, length);
			}
			memcpy (&word, memory, sizeof (word));
			memory += sizeof (word);
			printf ("%u", LE16TOH (word));
		}
		else if (!strcmp (object, "long")) 
		{
			uint32_t word;
			if (memory + sizeof (word) > extent) 
			{
				error (1, ECANCELED, "%s exceeds PIB length %X", object, length);
			}
			memcpy (&word, memory, sizeof (word));
			memory += sizeof (word);
			printf ("%u", LE32TOH (word));
		}
		else if (!strcmp (object, "xbyte")) 
		{
			uint8_t byte;
			if (memory + sizeof (byte) > extent) 
			{
				error (1, ECANCELED, "%s exceeds PIB length %X", object, length);
			}
			memcpy (&byte, memory, sizeof (byte));
			memory += sizeof (byte);
			printf ("0x%02X", byte);
		}
		else if (!strcmp (object, "xword")) 
		{
			uint16_t word;
			if (memory + sizeof (word) > extent) 
			{
				error (1, ECANCELED, "%s exceeds PIB length %X", object, length);
			}
			memcpy (&word, memory, sizeof (word));
			memory += sizeof (word);
			printf ("0x%04X", LE16TOH (word));
		}
		else if (!strcmp (object, "xlong")) 
		{
			uint32_t word;
			if (memory + sizeof (word) > extent) 
			{
				error (1, ECANCELED, "%s exceeds PIB length %X", object, length);
			}
			memcpy (&word, memory, sizeof (word));
			memory += sizeof (word);
			printf ("0x%08X", LE32TOH (word));
		}
		else if (!strcmp (object, "hfid")) 
		{
			string = (char *)(memory);
			length = PIB_HFID_LEN;
			if (memory + length > extent) 
			{
				error (1, ECANCELED, "%s exceeds PIB length %X", object, length);
			}
			memory += length;
			while (isprint (*string) && (length--)) 
			{
				putc (*string++, stdout);
			}
		}
		else if (!strcmp (object, "mac")) 
		{
			length = ETHER_ADDR_LEN;
			if (memory + length > extent) 
			{
				error (1, ECANCELED, "%s exceeds PIB length %X", object, length);
			}
			hexout (memory, length, ':', stdout);
			memory += length;
		}
		else if (!strcmp (object, "key")) 
		{
			length = HPAVKEY_DAK_LEN;
			if (memory + length > extent) 
			{
				error (1, ECANCELED, "%s exceeds PIB length %X", object, length);
			}
			hexout (memory, length, ':', stdout);
			memory += length;
		}
		else if (!strcmp (object, "data")) 
		{
			const char * size = *argv++;
			if (!size) 
			{
				error (1, EINVAL, "%s needs a length", object);
			}
			length = (unsigned)(uintspec (size, 1, LENGTH_MAX));
			if (memory + length > extent) 
			{
				error (1, ECANCELED, "%s exceeds PIB length %X", object, length);
			}
			hexout (memory, length, 0, stdout);
			memory += length;
		}
		else if (!strcmp (object, "text")) 
		{
			const char * size = *argv++;
			string = (char *)(memory);
			if (!size) 
			{
				error (1, EINVAL, "'%s' needs a length", object);
			}
			length = (unsigned)(uintspec (size, 1, LENGTH_MAX));
			if (memory + length > extent) 
			{
				error (1, ECANCELED, "%s exceeds PIB length %X", object, length);
			}
			memory += length;
			while (isprint (*string) && (length--)) 
			{
				putc (*string++, stdout);
			}
		}
		else if (!strcmp (object, "skip")) 
		{
			const char * size = *argv++;
			if (!size) 
			{
				error (1, EINVAL, "%s needs a length", object);
			}
			length = (unsigned)(uintspec (size, 1, LENGTH_MAX));
			memory += length;
			continue;
		}
		else 
		{
			error (1, ENOTSUP, "%s", *argv);
		}
		if ((argc) && (*argv)) 
		{
			putc (' ', stdout);
		}
	}
	if (_anyset (flags, SETPIB_NEWLINE)) 
	{
		putc ('\n', stdout);
	}
	return (0);
}

