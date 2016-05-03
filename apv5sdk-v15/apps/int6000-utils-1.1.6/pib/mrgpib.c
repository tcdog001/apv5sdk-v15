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
 *   mrgpib.c - overlay one PIB with another;
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
#include <stdlib.h>
#include <unistd.h>
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
#include "../pib/pib.h"

/*====================================================================*
 *   custom source files;
 *--------------------------------------------------------------------*/

#ifndef MAKEFILE
#include "../tools/getoptv.c"
#include "../tools/putoptv.c"
#include "../tools/version.c"
#include "../tools/fdchecksum_32.c"
#include "../tools/error.c"
#endif

#ifndef MAKEFILE
#include "../pib/pibfile.c"
#endif

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
		"qv",
		"target source",
		"overlay target PIB with source PIB",
		"q\tquiet",
		"v\tverbose messages",
		(const char *) (0) 
	};
	struct _file_ new;
	struct _file_ old;
	struct VersionHeader target;
	struct VersionHeader source;
	byte * buffer;
	signed length;
	flag_t flags = (flag_t)(0);
	signed c;
	optind = 1;
	opterr = 1;
	while ((c = getoptv (argc, argv, optv)) != -1) 
	{
		switch ((char) (c)) 
		{
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
	if (!argc--) 
	{
		error (1, ECANCELED, "No target file given");
	}
	new.name = *argv++;
	if ((new.file = open (new.name, O_BINARY|O_CREAT|O_RDWR)) == -1) 
	{
		error (1, errno, "Can't open %s", new.name);
	}
	if (pibfile (&new)) 
	{
		error (1, errno, "Bad target file: %s", new.name);
	}
	if (read (new.file, &target, sizeof (target)) < sizeof (target)) 
	{
		error (1, errno, "Can't read header: %s", new.name);
	}
	if (!argc--) 
	{
		error (1, ECANCELED, "No source file given");
	}
	old.name = *argv++;
	if ((old.file = open (old.name, O_BINARY|O_RDONLY)) == -1) 
	{
		error (1, errno, "Can't open %s", old.name);
	}
	if (pibfile (&old)) 
	{
		error (1, errno, "Bad source file: %s", old.name);
	}
	if (read (old.file, &source, sizeof (source)) < sizeof (source)) 
	{
		error (1, errno, "Can't read header: %s", old.name);
	}
	if (LE16TOH (target.PIBLength) < LE16TOH (source.PIBLength)) 
	{
		length = LE16TOH (target.PIBLength) - sizeof (target);
	}
	else 
	{
		length = LE16TOH (source.PIBLength) - sizeof (source);
	}
	if (!(buffer = malloc (length))) 
	{
		error (1, errno, "Need (%d) bytes: %s", length, old.name);
	}
	if (read (old.file, buffer, length) < length) 
	{
		error (1, errno, "Can't read %s body", old.name);
	}
	if (write (new.file, buffer, length) < length) 
	{
		error (1, errno, "Can't save %s body", new.name);
	}
	free (buffer);
	close (old.file);
	target.Checksum = 0;
	if (lseek (new.file, 0, SEEK_SET)) 
	{
		error (1, errno, "Can't rewind %s", new.name);
	}
	if (write (new.file, &target, sizeof (target)) < sizeof (target)) 
	{
		error (1, errno, "Can't update %s header", new.name);
	}
	if (lseek (new.file, 0, SEEK_SET)) 
	{
		error (1, errno, "Can't rewind %s", new.name);
	}
	target.Checksum = fdchecksum_32 (new.file, LE16TOH (target.PIBLength), 0);
	if (lseek (new.file, 0, SEEK_SET)) 
	{
		error (1, errno, "Can't rewind %s", new.name);
	}
	if (write (new.file, &target, sizeof (target)) < sizeof (target)) 
	{
		error (1, errno, "Can't update %s header", new.name);
	}
	close (new.file);
	exit (0);
}

