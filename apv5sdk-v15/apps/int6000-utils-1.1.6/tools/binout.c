/*====================================================================*
 *
 *   void binout (const void * memory, size_t extent, signed c, FILE *fp) 
 *
 *   memory.h
 *
 *   print memory as a series of binary bytes separated by character
 *   c; normally, character c will be BIN_EXTENDER as defined 
 *   in number.h; 
 *
 *   for example, binout (memory, 6, '-', stdout) would print
 *
 *      00000000-11010000-01010010-00000000-00000000-00000001
 *
 *
 *.  Motley Tools by Charles Maier <cmaier@cmassoc.net>;
 *:  Published 2005 by Charles Maier Associates Limited;
 *;  Released under the GNU General Public Licence v2 and later;
 *
 *--------------------------------------------------------------------*/

#ifndef BINOUT_SOURCE
#define BINOUT_SOURCE

#include <stdio.h>
#include <ctype.h>

#include "../tools/memory.h"
#include "../tools/number.h"

void binout (const void * memory, size_t extent, signed c, FILE *fp) 

{
	byte * offset = (byte *)(memory);
	while (extent--) 
	{
		unsigned bits = 8;
		while (bits--) 
		{
			putc (DIGITS_BIN [(* offset >> bits) & 1], fp);
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

