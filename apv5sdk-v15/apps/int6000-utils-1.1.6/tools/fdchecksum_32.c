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
 *   uint32_t fdchecksum_32 (int fd, size_t extent, uint32_t checksum);
 *
 *   memory.h
 *
 *   return the 32-bit checksum of a file region starting from the
 *   surrent file position; extent is specified in bytes but will be
 *   rounded to a multiple of four bytes;
 *
 *.  Motley Tools by Charles Maier <cmaier@cmassoc.net>;
 *:  Published 2001-2006 by Charles Maier Associates Limited;
 *;  Released under the GNU General Public Licence v2 and later;
 *
 *--------------------------------------------------------------------*/

#ifndef FDCHECKSUM_32_SOURCE
#define FDCHECKSUM_32_SOURCE

#include <stdio.h>
#include <unistd.h>
#include <errno.h>

#include "../tools/memory.h"

uint32_t fdchecksum_32 (int fd, register size_t extent, register uint32_t checksum) 

{
	uint32_t memory;
	while (extent >= sizeof (memory)) 
	{
		if (read (fd, &memory, sizeof (memory)) != sizeof (memory)) 
		{
			return (-1);
		}
		extent -= sizeof (memory);
		checksum ^= memory;
	}
	return (~checksum);
}

#endif

