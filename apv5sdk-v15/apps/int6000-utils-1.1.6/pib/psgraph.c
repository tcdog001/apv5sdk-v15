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
 *   psgraph.c - print PIB prescalar graph;
 *
 *.  Intellon INT6000 Linux Toolkit for HomePlug AV;
 *:  Published 2006-2008 by Intellon Corp. ALL RIGHTS RESERVED;
 *;  For demonstration; Not for production use;
 *
 *--------------------------------------------------------------------*/

#define _GETOPT_H

/*====================================================================*
 *   system header files;
 *--------------------------------------------------------------------*/

#include <unistd.h>
#include <limits.h>
#include <errno.h>

/*====================================================================*
 *   custom header files;
 *--------------------------------------------------------------------*/

#include "../tools/getoptv.h"
#include "../tools/putoptv.h"
#include "../tools/number.h"
#include "../tools/error.h"
#include "../tools/files.h"

/*====================================================================*
 *   custom source files;
 *--------------------------------------------------------------------*/

#ifndef MAKEFILE
#include "../tools/getoptv.c"
#include "../tools/putoptv.c"
#include "../tools/version.c"
#include "../tools/uintspec.c"
#include "../tools/todigit.c"
#include "../tools/error.c"
#endif

/*====================================================================*
 *   program constants;
 *--------------------------------------------------------------------*/

#define TM_VERBOSE (1 << 0)
#define TM_SILENCE (1 << 1)

#define OFFSET 0x0A10
#define LENGTH 0x0483

#define INDEX_TO_FREQ(index) ((float)(index + 74)/40.96) 
#define FREQ_TO_INDEX(freq)  ((unsigned)(40.96 * freq)-74)

/*====================================================================*
 *
 *   int main (int argc, const char * argv []);
 *
 *
 *.  Intellon INT6000 Linux Toolkit for HomePlug AV;
 *:  Published 2006-2008 by Intellon Corp. ALL RIGHTS RESERVED;
 *;  For demonstration; Not for production use;
 *
 *--------------------------------------------------------------------*/

int main (int argc, const char *argv []) 

{
	static const char *optv [] = 
	{
		"s:",
		"file [file] [...] [> stdout]",
		"print PIB prescalar graph" "amplitude map display",
		"s n\tscale",
		(const char *) (0) 
	};
	signed index = 0;
	signed scale = 5;
	uint32_t table [LENGTH];
	file_t fd;
	signed c;
	optind = 1;
	opterr = 1;
	while ((c = getoptv (argc, argv, optv)) != -1) 
	{
		switch (c) 
		{
		case 's':
			scale = (unsigned)(uintspec (optarg, 1, UCHAR_MAX));
			break;
		default:
			break;
		}
	}
	argc -= optind;
	argv += optind;
	while ((argc) && (*argv)) 
	{
		if ((fd = open (*argv, O_BINARY|O_RDONLY)) == -1) 
		{
			error (0, errno, "Can't open %s", *argv);
		}
		else if (lseek (fd, OFFSET, SEEK_SET) != OFFSET) 
		{
			error (0, errno, "Can't seek %s", *argv);
			close (fd);
		}
		else if (read (fd, table, sizeof (table)) < sizeof (table)) 
		{
			error (0, errno, "Can't read %s", *argv);
			close (fd);
		}
		else 
		{
			for (index = 0; index < LENGTH; index++) 
			{
				int32_t value = table [index];
				printf (" %04d %6d %6.3f ", index, value, INDEX_TO_FREQ (index));
				while (value > (scale >> 1)) 
				{
					printf ("#");
					value -= scale;
				}
				printf ("\n");
			}
			close (fd);
		}
		argc--;
		argv++;
	}
	return (0);
}

