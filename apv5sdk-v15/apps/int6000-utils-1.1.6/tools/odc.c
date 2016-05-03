/*====================================================================*
 *
 *   odc.c - object driven compare;
 *
 *.  Motley Tools by Charles Maier; cmaier@cmassoc.net;
 *:  Published 2005 by Charles Maier Associates for internal use;
 *;  Released under GNU General Public License v2 and later;
 *
 *   Contributor(s):
 *      Nathan Houghton <nathan@brainwerk.org>
 *      Abdel Younes <younes@leacom.fr>
 *
 *--------------------------------------------------------------------*/

#define _GETOPT_H

/*====================================================================*
 *   system header files;
 *--------------------------------------------------------------------*/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>

/*====================================================================*
 *   custom header files;
 *--------------------------------------------------------------------*/

#include "../tools/getoptv.h"
#include "../tools/putoptv.h"
#include "../tools/version.h"
#include "../tools/memory.h"
#include "../tools/error.h"
#include "../tools/files.h"
#include "../tools/sizes.h"
#include "../tools/chars.h"

/*====================================================================*
 *   custom source files;
 *--------------------------------------------------------------------*/

#ifndef MAKEFILE
#include "../tools/getoptv.c"
#include "../tools/putoptv.c"
#include "../tools/version.c"
#include "../tools/hexview.c"
#include "../tools/emalloc.c"
#include "../tools/error.c"
#include "../tools/alert.c"
#endif

/*====================================================================*
 *   program constants;
 *--------------------------------------------------------------------*/

#define _BARWIDTH 73

/*====================================================================*
 *   
 *   void function (const char * filename[], flag_t flags);
 *   
 *   read definition file to determine the size of next object;
 *   read next object from each binary file and compare; report
 *   differences;
 *   
 *.  Motley Tools by Charles Maier; cmaier@cmassoc.net;
 *:  Published 2005 by Charles Maier Associates for internal use;
 *;  Released under GNU General Public License v2 and later;
 *
 *--------------------------------------------------------------------*/

static void function (const char * filename [], flag_t flags) 

{
	unsigned lineno = 1;
	unsigned object = 0;
	unsigned offset = 0;
	unsigned length = 0;
	unsigned extent [2] = 
	{
		0,
		0 
	};
	byte * buffer [2];
	char symbol [_NAMESIZE];
	char string [_LINESIZE];
	char * sp;
	signed fd [2];
	signed c;
	signed i;
	for (i = 0; i < 2; i++) 
	{
		if ((fd [i] = open (filename [i], O_BINARY|O_RDONLY)) == -1) 
		{
			error (1, errno, "Can't open %s", filename [i]);
		}
		if ((extent [i] = lseek (fd [i], 0, SEEK_END)) == -1) 
		{
			error (1, errno, "Can't size %s", filename [i]);
		}
		if (lseek (fd [i], 0, SEEK_SET)) 
		{
			error (1, errno, "Can't home %s", filename [i]);
		}
	}
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
				error (1, EINVAL, "Have '%s' but no ']' on line %d", symbol, lineno);
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
		if (length) 
		{
			buffer [0] = emalloc (length);
			buffer [1] = emalloc (length);
			if (read (fd [0], buffer [0], length) < (signed)(length)) 
			{
				break;
			}
			if (read (fd [1], buffer [1], length) < (signed)(length)) 
			{
				break;
			}
			if (memcmp (buffer [0], buffer [1], length)) 
			{
				if (!object++) 
				{
					for (c = 0; c < _BARWIDTH; c++) 
					{
						putc ('-', stdout);
					}
					putc ('\n', stdout);
				}
				printf ("%08X  %d %s\n", offset, length, symbol);
				printf ("--------  %s\n", filename [0]);
				hexview (buffer [0], offset, length, stdout);
				printf ("--------  %s\n", filename [1]);
				hexview (buffer [1], offset, length, stdout);
				for (c = 0; c < _BARWIDTH; c++) 
				{
					putc ('-', stdout);
				}
				putc ('\n', stdout);
			}
			free (buffer [0]);
			free (buffer [1]);
		}
		offset += length;
		lineno++;
	}
	if (extent [0] > extent [1]) 
	{
		alert ("%s exceeds %s", filename [0], filename [1]);
	}
	if (extent [1] > extent [0]) 
	{
		alert ("%s exceeds %s", filename [1], filename [0]);
	}
	close (fd [0]);
	close (fd [1]);
	return;
}

/*====================================================================*
 *   
 *   int main (int argc, const char * argv []);
 *   
 *   
 *.  Motley Tools by Charles Maier; cmaier@cmassoc.net;
 *:  Published 2005 by Charles Maier Associates for internal use;
 *;  Released under GNU General Public License v2 and later;
 *
 *--------------------------------------------------------------------*/

int main (int argc, const char * argv []) 

{
	static const char *optv [] = 
	{
		"f:",
		"file file",
		"object driven compare",
		"f f\tobject definition file",
		(const char *)(0) 
	};
	flag_t flags = (flag_t)(0);
	signed c;
	optind = 1;
	opterr = 1;
	while ((c = getoptv (argc, argv, optv)) != -1) 
	{
		switch (c) 
		{
		case 'f':
			if (!freopen (optarg, "rb", stdin)) 
			{
				error (1, errno, "Can't open %s", optarg);
			}
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

