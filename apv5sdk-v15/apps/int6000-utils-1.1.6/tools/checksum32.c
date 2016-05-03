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
 *   uint32_t checksum32 (const int32_t memory [], size_t extent, uint32_t checksum);
 *
 *   memory.h
 *
 *   compute the 32 bit checksum of a memory segment; the checksum
 *   is the one's complement of the XOR of all 32 bit words;
 *
 *   this checksum is returned in host byte order on the assumption
 *   that the memory segment is stored little endian;
 *
 *.  Motley Tools by Charles Maier <cmaier@cmassoc.net>;
 *:  Published 2001-2006 by Charles Maier Associates Limited;
 *;  Released under the GNU General Public Licence v2 and later;
 *
 *   Contributor(s):
 *      Charles Maier <charles.maier@intellon.com>
 *	Alex Vasquez <alex.vasquez@intellon.com>
 *
 *--------------------------------------------------------------------*/

#ifndef CHECKSUM32_SOURCE
#define CHECKSUM32_SOURCE

#include <stdint.h>

#include "../tools/memory.h"

uint32_t checksum32 (register const uint32_t memory [], register size_t extent, uint32_t checksum) 

{
	while (extent-- > 0) 
	{
		checksum ^= LE32TOH (*memory);
		memory++;
	}
	return (~checksum);
}

#endif

