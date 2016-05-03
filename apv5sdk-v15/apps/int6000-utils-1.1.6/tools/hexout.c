/*====================================================================*
 *
 *   void hexout (const void * memory, size_t extent, signed c, FILE *fp);
 *
 *   memory.h
 *
 *   print memory as a series of hexadecimal octets seperated by 
 *   character c; normally, character c will be HEX_EXTENDER as 
 *   defined in number.h;
 *
 *   for example, hexout (memory, 6, ':', stdout) would print:
 *
 *      00:B0:52:00:00:01
 *
 *.  Motley Tools by Charles Maier <cmaier@cmassoc.net>;
 *:  Published 2005 by Charles Maier Associates Limited;
 *;  Released under the GNU General Public Licence v2 and later;
 *
 *--------------------------------------------------------------------*/

#ifndef HEXOUT_SOURCE
#define HEXOUT_SOURCE

#include <stdio.h>
#include <ctype.h>

#include "../tools/memory.h"
#include "../tools/number.h"

void hexout (const void * memory, size_t extent, signed c, FILE *fp) 

{
	byte * offset = (byte *)(memory);
	while (extent--) 
	{
		putc (DIGITS_HEX [(* offset >> 4) & 0x0F], fp);
		putc (DIGITS_HEX [(* offset >> 0) & 0x0F], fp);
		if ((extent) && isprint (c)) 
		{
			putc (c, fp);
		}
		offset++;
	}
	return;
}

#endif

