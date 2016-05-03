/*====================================================================*
 *
 *   void decout (const byte memory [], size_t extent, signed c, FILE *fp);
 *
 *   memory.h
 *
 *   print a memory region as a series of decimal octets separated  
 *   by character c; normally, c will be DEC_EXTENDER as defined in 
 *   number.h; 
 *
 *   for example, decout (memory, 4, '.', stdout) would print
 *
 *      192.168.101.002
 *
 *.  Motley Tools by Charles Maier <cmaier@cmassoc.net>;
 *:  Published 2005 by Charles Maier Associates Limited;
 *;  Released under the GNU General Public Licence v2 and later;
 *
 *--------------------------------------------------------------------*/

#ifndef DECOUT_SOURCE
#define DECOUT_SOURCE

#include <stdio.h>
#include <ctype.h>

#include "../tools/memory.h"
#include "../tools/number.h"

void decout (const void * memory, size_t extent, signed c, FILE *fp) 

{
	byte * offset = (byte *)(memory);
	while (extent--) 
	{
		unsigned order = 100;
		while (order) 
		{
			putc (DIGITS_DEC [(* offset / order) % RADIX_DEC], fp);
			order /= RADIX_DEC;
		}
		if ((extent) && isprint (c)) 
		{
			putc (c, fp);
		}
		offset++;
	}
	return;
}

#endif

