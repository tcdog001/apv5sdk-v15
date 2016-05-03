/*====================================================================*
 *
 *   odd.c - object driven dump utility;
 *
 *.  Motley Tools by Charles Maier; cmaier@cmassoc.net;
 *:  Published 2005 by Charles Maier Associates for internal use;
 *;  Released under GNU General Public License v2 and later;
 *
 *   Contributor(s);
 *      Abdel Younes <younes@leacom.fr>
 *
 *--------------------------------------------------------------------*/

#define _GETOPT_H

/*====================================================================*
 *   system header files;
 *--------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
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
#endif

/*====================================================================*
 *   program constants;
 *--------------------------------------------------------------------*/

#define _BARWIDTH 73

/*====================================================================*
 *   
 *   void function (const char * filename, flag_t flags);
 *   
 *   
 *.  Motley Tools by Charles Maier; cmaier@cmassoc.net;
 *:  Published 2005 by Charles Maier Associates for internal use;
 *;  Released under GNU General Public License v2 and later;
 *
 *--------------------------------------------------------------------*/

static void function (const char * filename, flag_t flags) 

{
	unsigned lineno = 0;
	unsigned object = 0;
	unsigned offset = 0;
	signed length = 0;
	signed extent = 0;
	byte * buffer;
	char symbol [_NAMESIZE];
	char string [_LINESIZE];
	char * sp;
	signed fd;
	signed c;
	if ((fd = open (filename, O_BINARY|O_RDONLY)) == -1) 
	{
		error (1, errno, "Can't open %s", filename);
	}
	if ((extent = lseek (fd, 0, SEEK_END)) == -1) 
	{
		error (1, errno, "Can't size %s", filename);
	}
	if (lseek (fd, 0, SEEK_SET)) 
	{
		error (1, errno, "Can't home %s", filename);
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
			buffer = emalloc (length);
			if (read (fd, buffer, length) < length) 
			{
				error (0, errno, "%s is short", filename);
				break;
			}
			if (!object++) 
			{
				for (c = 0; c < _BARWIDTH; c++) 
				{
					putc ('-', stdout);
				}
				putc ('\n', stdout);
			}
			printf ("%08X  %d %s\n", offset, length, symbol);
			hexview (buffer, offset, length, stdout);
			for (c = 0; c < _BARWIDTH; c++) 
			{
				putc ('-', stdout);
			}
			putc ('\n', stdout);
			free (buffer);
		}
		offset += length;
		lineno++;
	}
	close (fd);
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
		"file [file] [...]",
		"object driven dump",
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
	while ((argc) && (*argv)) 
	{
		rewind (stdin);
		function (*argv, flags);
		argc--;
		argv++;
	}
	return (0);
}

