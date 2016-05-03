/*====================================================================*
 *
 *   odx.c - object driven XML dump;
 *
 *.  Motley Tools by Charles Maier; cmaier@cmassoc.net;
 *:  Published 2005 by Charles Maier Associates for internal use;
 *;  Released under GNU General Public License v2 and later;
 *
 *--------------------------------------------------------------------*/

#define _GETOPT_H

/*====================================================================*
 *   system header files;
 *--------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <errno.h>

/*====================================================================*
 *   custom header files;
 *--------------------------------------------------------------------*/

#include "../tools/getoptv.h"
#include "../tools/putoptv.h"
#include "../tools/version.h"
#include "../tools/format.h"
#include "../tools/files.h"
#include "../tools/sizes.h"
#include "../tools/chars.h"
#include "../tools/error.h"
#include "../nodes/nodes.h"

/*====================================================================*
 *   custom source files;
 *--------------------------------------------------------------------*/

#ifndef MAKEFILE
#include "../tools/getoptv.c"
#include "../tools/putoptv.c"
#include "../tools/version.c"
#include "../tools/hexview.c"
#include "../tools/output.c"
#include "../tools/error.c"
#endif

#ifndef MAKEFILE
#include "../nodes/xmlschema.c"
#endif

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

static void function (const char * filename, const char * schema, flag_t flags) 

{
	unsigned margin = 0;
	unsigned offset = 0;
	unsigned length = 0;
	unsigned extent = 0;
	unsigned lineno = 1;
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
	output (margin++, "<%s xmlns:xsi='%s' xsi:noNamespaceSchemaLocation='%s'>", DATA_OBJECT, XML_NAMESPACE, schema);
	while ((c = getc (stdin)) != EOF) 
	{
		if ((c == '#') || (c == ';')) 
		{
			do 
			{
				c = getc (stdin);
			}
			while (nobreak (c));
		}
		if (isspace (c)) 
		{
			if (c == '\n') 
			{
				lineno++;
			}
			continue;
		}
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
		*sp = (char)(0);
		while (isblank (c)) 
		{
			c = getc (stdin);
		}
		if (c == '[') 
		{

#if 0

			*sp++ = (char)(c);

#endif

			c = getc (stdin);
			while (isblank (c)) 
			{
				c = getc (stdin);
			}
			while (isdigit (c)) 
			{

#if 0

				*sp++ = (char)(c);

#endif

				c = getc (stdin);
			}
			while (isblank (c)) 
			{
				c = getc (stdin);
			}
			if (c != ']') 
			{
				error (1, EINVAL, "Have '%c' but want ']'", c);
			}

#if 0

			*sp++ = (char)(c);

#endif

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
		if (!length) 
		{
			continue;
		}
		if (!(buffer = malloc (length))) 
		{
			error (1, errno, "Can't allocate %d bytes", length);
		}
		if (read (fd, buffer, length) != length) 
		{
			error (0, ECANCELED, "File too short");
			break;
		}
		output (margin++, "<%s name='%s'>", DATA_MEMBER, symbol);

#if 0

		if (*string) 
		{
			output (margin++, "<text>");
			output (margin, "%s", string);
			output (margin--, "</text>");
		}

#endif

		output (margin++, "<%s>", DATA_OFFSET);
		output (margin, "%04X", offset);
		output (margin--, "</%s>", DATA_OFFSET);
		output (margin++, "<%s>", DATA_LENGTH);
		output (margin, "%d", length);
		output (margin--, "</%s>", DATA_LENGTH);
		output (margin++, "<%s>", DATA_MEMORY);
		for (c = 0; c < (signed)(margin); c++) 
		{
			printf ("\t");
		}
		for (c = 0; c < (signed)(length); c++) 
		{
			printf ("%02X", buffer [c]);
		}
		printf ("\n");
		output (margin--, "</%s>", DATA_MEMORY);
		output (margin--, "</%s>", DATA_MEMBER);
		offset += length;
		length = 0;
		free (buffer);
	}
	output (margin--, "</%s>", DATA_OBJECT);
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
		"f:s:x",
		"file [file] [...]",
		"object driven XML dump utility",
		"f f\tobject definition file",
		"x\tprint an XML schema on stdout",
		(const char *)(0) 
	};
	const char * schema = DATA_SCHEMA;
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
		case 's':
			schema = optarg;
			break;
		case 'x':
			xmlschema ();
			return (0);
		default:
			break;
		}
	}
	argc -= optind;
	argv += optind;
	while ((argc) && (*argv)) 
	{
		rewind (stdin);
		function (*argv, schema, flags);
		argc--;
		argv++;
	}
	return (0);
}

