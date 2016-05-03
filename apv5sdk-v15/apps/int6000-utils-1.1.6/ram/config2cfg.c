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
 *   config2cfg.c - convert a .config file to a .cfg file;
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
 *.  Intellon INT6x00 Linux Toolkit for HomePlug AV; 
 *:  Published 2007 by Intellon Corp. ALL RIGHTS RESERVED;
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
#include <fcntl.h>
#include <errno.h>

/*====================================================================*
 *   custom header files;
 *--------------------------------------------------------------------*/

#include "../tools/getoptv.h"
#include "../tools/putoptv.h"
#include "../tools/version.h"
#include "../tools/number.h"
#include "../tools/flags.h"
#include "../tools/memory.h"
#include "../tools/files.h"
#include "../tools/error.h"
#include "../ram/sdram.h"

/*====================================================================*
 *   custom source files;
 *--------------------------------------------------------------------*/

#ifndef MAKEFILE
#include "../tools/getoptv.c"
#include "../tools/putoptv.c"
#include "../tools/version.c"
#include "../tools/checksum_32.c"
#include "../tools/todigit.c"
#include "../tools/hexencode.c"
#endif

/*====================================================================*
 *   
 *--------------------------------------------------------------------*/

int main (int argc, const char * argv []) 

{
	static const char *optv [] = 
	{
		"qv",
		"file [file] [...]",
		"convert ASCII SDRAM configuration files (DM) to binary (toolkit) format",
		"q\tquiet mode",
		"v\tverbose mode",
		(const char *) (0) 
	};
	struct config_ram config_ram;
	char string [(sizeof (config_ram) << 1) + 1];
	uint32_t checksum;
	flag_t flags = (flag_t)(0);
	signed state = 1;
	signed fd;
	signed c;
	optind = 1;
	opterr = 1;
	while ((c = getoptv (argc, argv, optv)) != -1) 
	{
		switch ((char) (c)) 
		{
		case 'q':
			_setbits (flags, SDRAM_SILENCE);
			break;
		case 'v':
			_setbits (flags, SDRAM_VERBOSE);
			break;
		default:
			break;
		}
	}
	argc -= optind;
	argv += optind;
	while ((argc-- > 0) && (*argv != (const char *)(0))) 
	{

#if 0

		const char * pathname;
		const char * filename;
		const char * extender;
		for (pathname = filename = * argv; *pathname; pathname++) 
		{
			if ((*pathname == '/') || (*pathname == '\\')) 
			{
				filename = pathname + 1;
			}
		}
		for (pathname = extender = filename; *pathname; pathname++) 
		{
			if (*pathname == '.') 
			{
				extender = pathname;
			}
		}
		if (extender == filename) 
		{
			extender = pathname;
		}

#endif

		if ((fd = open (*argv, O_BINARY|O_RDONLY)) == -1) 
		{
			error (0, errno, "can't open %s for input", *argv);
			state = 1;
		}
		else if (read (fd, &string, sizeof (string)) < (sizeof (string) - 1)) 
		{
			error (0, errno, "can't read %s", *argv);
			state = 1;
		}
		else 
		{
			close (fd);
			if (hexencode ((uint8_t *) (&config_ram), sizeof (config_ram), string) == sizeof (config_ram)) 
			{
				error (1, errno, "%s is suspect", *argv);
			}
			checksum = checksum_32 (&config_ram, sizeof (config_ram), 0);
			if ((fd = open (*argv, O_BINARY|O_CREAT|O_RDWR|O_TRUNC)) == -1) 
			{
				error (1, errno, "can't open %s for output", *argv);
			}
			write (fd, &config_ram, sizeof (config_ram));
			write (fd, &checksum, sizeof (checksum));
		}
		close (fd);
		argv++;
	}
	return (0);
}

