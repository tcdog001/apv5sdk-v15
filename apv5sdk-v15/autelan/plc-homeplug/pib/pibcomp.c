/*====================================================================*
 *   
 *   Copyright (c) 2011 by Qualcomm Atheros.
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
 *   pibcomp.c - Qualcomm Atheros Parameter Block Compare Utility
 *
 *.  Atheros Powerline Toolkit for HomePlug AV
 *:  Published 2012 by Qualcomm Atheros
 *;  For demonstration and evaluation only
 *
 *   Contributor(s):
 *      Nathan Houghton <nathan.houghton@qca.qualcomm.com>
 *	Charles Maier <charles.maier@qca.qualcomm.com>
 *
 *--------------------------------------------------------------------*/

#define _GETOPT_H

/*====================================================================*
 *   system header files;
 *--------------------------------------------------------------------*/

#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <sys/stat.h>

/*====================================================================*
 *   custom header files;
 *--------------------------------------------------------------------*/

#include "../tools/getoptv.h"
#include "../tools/number.h"
#include "../tools/memory.h"
#include "../tools/error.h"
#include "../tools/flags.h"
#include "../tools/chars.h"
#include "../tools/sizes.h"
#include "../tools/files.h"
#include "../pib/pib.h"

/*====================================================================*
 *   custom source files;
 *--------------------------------------------------------------------*/

#ifndef MAKEFILE
#include "../tools/getoptv.c"
#include "../tools/putoptv.c"
#include "../tools/version.c"
#include "../tools/hexview.c"
#include "../tools/hexoffset.c"
#include "../tools/error.c"
#include "../tools/checksum32.c"
#include "../tools/fdchecksum32.c"
#endif

#ifndef MAKEFILE
#include "../pib/pibtype.c"
#include "../pib/pibseek.c"
#endif

/*====================================================================*
 *   
 *   void pibcomp (struct _file_ file [], flag_t flags);
 *   
 *   compare two files and print an object driven compare on stdout;
 *   offsets are read from stdin;
 *
 *.  Atheros Powerline Toolkit for HomePlug AV
 *:  Published 2012 by Qualcomm Atheros
 *;  For demonstration and evaluation only
 *
 *--------------------------------------------------------------------*/

static void pibcomp (struct _file_ files [], flag_t flags) 

{
	unsigned object = 0;
	unsigned lineno = 1;
	unsigned offset = 0;
	signed length = 0;
	char memory [_ADDRSIZE+1];
	char symbol [_NAMESIZE];
	char string [_LINESIZE];
	char * sp;
	signed c;
	while ((c = getc (stdin)) != EOF) 
	{
		if ((c == '#') || (c == ';')) 
		{
			do 
			{
				c = getc (stdin);
			}
			while (nobreak (c));
			lineno++;
			continue;
		}
		if (isspace (c)) 
		{
			if (c == '\n') 
			{
				lineno++;
			}
			continue;
		}
		length = 0;
		while (isdigit (c)) 
		{
			length *= 10;
			length += c - '0';
			c = getc (stdin);
		}
		while (isblank (c)) 
		{
			c = getc (stdin);
		}
		sp = symbol;
		if (isalpha (c) || (c == '_')) 
		{
			do 
			{
				*sp++ = (char)(c);
				c = getc (stdin);
			}
			while (isident (c));
		}
		while (isblank (c)) 
		{
			c = getc (stdin);
		}
		if (c == '[') 
		{
			*sp++ = (char)(c);
			c = getc (stdin);
			while (isblank (c)) 
			{
				c = getc (stdin);
			}
			while (isdigit (c)) 
			{
				*sp++ = (char)(c);
				c = getc (stdin);
			}
			while (isblank (c)) 
			{
				c = getc (stdin);
			}
			*sp = (char)(0);
			if (c != ']') 
			{
				error (1, EINVAL, "Have '%s' but need ']' on line %d", symbol, lineno);
			}
			*sp++ = (char)(c);
			c = getc (stdin);
		}
		*sp = (char)(0);
		while (isblank (c)) 
		{
			c = getc (stdin);
		}
		sp = string;
		while (nobreak (c)) 
		{
			*sp++ = (char)(c);
			c = getc (stdin);
		}
		*sp = (char)(0);
		if (length > 0) 
		{

#if defined (WIN32)

			char * buffer [2];
			buffer [0] = (char *)(emalloc (length));
			buffer [1] = (char *)(emalloc (length));

#else

			byte buffer [2][length];

#endif

			if ((read (files [0].file, buffer [0], length) == length) && (read (files [1].file, buffer [1], length) == length))
			{
				if (memcmp (buffer [0], buffer [1], length)) 
				{
					if (!object++) 
					{
						for (c = 0; c < _ADDRSIZE + 65; c++) 
						{
							putc ('-', stdout);
						}
						putc ('\n', stdout);
					}
					printf ("%s %d %s\n", hexoffset (memory, sizeof (memory), offset), length, symbol);
					for (c = 0; c < _ADDRSIZE; c++) 
					{
						putc ('-', stdout);
					}
					printf (" %s\n", files [0].name);
					hexview (buffer [0], offset, length, stdout);
					for (c = 0; c < _ADDRSIZE; c++) 
					{
						putc ('-', stdout);
					}
					printf (" %s\n", files [1].name);
					hexview (buffer [1], offset, length, stdout);
					for (c = 0; c < _ADDRSIZE + 65; c++) 
					{
						putc ('-', stdout);
					}
					putc ('\n', stdout);
				}
			}

#if defined (WIN32)

			free (buffer [0]);
			free (buffer [1]);

#endif

		}
		offset += length;
		lineno++;
	}
	if (_allclr (flags, PIB_SILENCE)) 
	{
		unsigned extent [2];
		unsigned file;
		for (file = 0; file < SIZEOF (extent); file++) 
		{
			if ((extent [file] = lseek (files [file].file, 0, SEEK_END)) == (unsigned)(-1)) 
			{
				error (1, 0, FILE_CANTSIZE, files [file].name);
			}
			if (offset < extent [file]) 
			{
				error (0, 0, "%s exceeds definition by %u bytes", files [file].name, extent [file] - offset);
			}
			if (offset > extent [file]) 
			{
				error (0, 0, "definition exceeds %s by %u bytes", files [file].name, offset - extent [file]);
			}
		}
		if (extent [0] > extent [1]) 
		{
			error (0, 0, "%s exceeds %s by %u bytes", files [0].name, files [1].name, extent [0] - extent [1]);
		}
		if (extent [1] > extent [0]) 
		{
			error (0, 0, "%s exceeds %s by %u bytes", files [1].name, files [0].name, extent [1] - extent [0]);
		}
	}
	return;
}


/*====================================================================*
 *
 *   signed function (char const * filename [], flag_t flags);
 *
 *   open a named file and determine if it is a valid thunderbolt, 
 *   lightning, panther or lynx PIB; panther and lynx PIBs are just
 *   image chain files containing a legacy pib;
 *
 *.  Atheros Powerline Toolkit for HomePlug AV
 *:  Published 2012 by Qualcomm Atheros
 *;  For demonstration and evaluation only
 *   
 *--------------------------------------------------------------------*/

static signed function (char const * filename [], flag_t flags) 

{
	struct _file_ files [2];
	unsigned file;
	for (file = 0; file < SIZEOF (files); file++) 
	{
		files [file].name = filename [file];
		if ((files [file].file = open (files [file].name, O_BINARY|O_RDONLY)) == -1) 
		{
			error (1, errno, "%s", files [file].name);
		}
		if (pibtype (&files [file], flags) == -1)
		{
			error (1, ENOTSUP, "%s", files [file].name);
		}
	}
	pibcomp (files, flags);
	for (file = 0; file < SIZEOF (files); file++) 
	{
		close (files [file].file);
	}
	return (0);
}


/*====================================================================*
 *   
 *   int main (int argc, char const * argv []);
 *   
 *   
 *.  Atheros Powerline Toolkit for HomePlug AV
 *:  Published 2012 by Qualcomm Atheros
 *;  For demonstration and evaluation only
 *
 *--------------------------------------------------------------------*/

int main (int argc, char const * argv []) 

{
	static char const * optv [] = 
	{
		"f:qv",
		"file file",
		"Qualcomm Atheros Parameter Block Compare Utility",
		"f f\tobject definition file",
		"q\tquiet mode",
		"v\tverbose mode",
		(char const *)(0)
	};
	flag_t flags = (flag_t)(0);
	signed c;
	optind = 1;
	while ((c = getoptv (argc, argv, optv)) != -1) 
	{
		switch (c) 
		{
		case 'f':
			if (!freopen (optarg, "rb", stdin)) 
			{
				error (1, errno, "%s", optarg);
			}
			break;
		case 'q':
			_setbits (flags, PIB_SILENCE);
			break;
		case 'v':
			_setbits (flags, PIB_VERBOSE);
			break;
		default:
			break;
		}
	}
	argc -= optind;
	argv += optind;
	if (argc != 2) 
	{
		error (1, 0, "Need two files to compare.");
	}
	function (argv, flags);
	return (0);
}



