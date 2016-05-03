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
 *   pxe.c - Intellon PIB XML Editor;
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
#include <stdint.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <errno.h>
#include <limits.h>

/*====================================================================*
 *  custom header files;
 *--------------------------------------------------------------------*/

#include "../tools/getoptv.h"
#include "../tools/files.h"
#include "../tools/number.h"
#include "../tools/memory.h"
#include "../tools/error.h"
#include "../nodes/nodes.h"
#include "../crypt/HPAVKey.h"
#include "../pib/pib.h"

/*====================================================================*
 *   custom source files;
 *--------------------------------------------------------------------*/

#ifndef MAKEFILE
#include "../tools/getoptv.c"
#include "../tools/putoptv.c"
#include "../tools/version.c"
#include "../tools/emalloc.c"
#include "../tools/fdchecksum_32.c"
#include "../tools/checksum_32.c"
#include "../tools/hexstring.c"
#include "../tools/hexdecode.c"
#include "../tools/todigit.c"
#include "../tools/alert.c"
#include "../tools/error.c"
#include "../tools/output.c"
#endif

#ifndef MAKEFILE
#include "../nodes/xmlattribute.c"
#include "../nodes/xmlcontent.c"
#include "../nodes/xmledit.c"
#include "../nodes/xmlfree.c"
#include "../nodes/xmlnode.c"
#include "../nodes/xmlopen.c"
#include "../nodes/xmlscan.c"
#include "../nodes/xmlschema.c"
#include "../nodes/xmlvalue.c"
#endif

#ifndef MAKEFILE
#include "../pib/pibfile.c"
#include "../pib/pibpeek.c"
#include "../pib/pibcodes.c"
#endif

#ifndef MAKEFILE
#include "../crypt/keys.c"
#endif

/*====================================================================*
 *
 *   int main (int argc, const char * argv []);
 *
 *
 *
 *--------------------------------------------------------------------*/

int main (int argc, const char * argv []) 

{
	static const char *optv [] = 
	{
		"oE",
		"pib-file xml-file [xml-file] [...]",
		"Intellon PIB XML Editor",
		"o\tprint XML schema on stdout",
		(const char *)(0) 
	};
	struct header_pib * header;
	struct _file_ pib;
	byte * buffer;
	signed length;
	signed c;
	optind = 1;
	opterr = 1;
	while ((c = getoptv (argc, argv, optv)) != -1) 
	{
		switch ((char)(c)) 
		{
		case 'o':
			xmlschema ();
			return (1);
		default:
			break;
		}
	}
	argc -= optind;
	argv += optind;
	if (!argc--) 
	{
		return (0);
	}
	pib.name = *argv++;
	if ((pib.file = open (pib.name, O_BINARY|O_RDONLY)) == -1) 
	{
		error (1, errno, "Can't open %s", pib.name);
	}
	if (pibfile (&pib)) 
	{
		error (1, errno, "Bad PIB file: %s", pib.name);
	}
	if ((length = lseek (pib.file, 0, SEEK_END)) == -1) 
	{
		error (1, errno, "Can't size %s", pib.name);
	}
	if (!(buffer = malloc (length))) 
	{
		error (1, errno, "Can't load %s", pib.name);
	}
	if (lseek (pib.file, 0, SEEK_SET)) 
	{
		error (1, errno, "Can't rewind %s", pib.name);
	}
	if (read (pib.file, buffer, length) < length) 
	{
		error (1, errno, "Can't read %s", pib.name);
	}
	close (pib.file);
	while ((argc) && (*argv)) 
	{
		struct node * markup = xmlopen (*argv);
		xmledit (markup, buffer, length);
		xmlfree (markup);
		argc--;
		argv++;
	}
	header = (struct header_pib *)(buffer);
	header->CHECKSUM = 0;
	header->CHECKSUM = checksum_32 (buffer, length, 0);
	if ((pib.file = open (pib.name, O_BINARY|O_CREAT|O_RDWR|O_TRUNC, FILE_FILEMODE)) == -1) 
	{
		error (1, errno, "Can't open %s", pib.name);
	}
	if (write (pib.file, buffer, length) < length) 
	{
		error (1, errno, "Can't save %s", pib.name);
	}
	close (pib.file);
	return (0);
}

