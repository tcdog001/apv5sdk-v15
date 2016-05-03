/*====================================================================*
 *
 *   void hexview (const byte memory [], size_t offset, size_t extent, FILE *fp);
 *
 *   memory.h
 *
 *   print memory as a hexadecimal dump showing absolute offsets and
 *   an ASCII character display; this function is similar to but not
 *   the same as function hexdump;
 *
 *.  Motley Tools by Charles Maier <cmaier@cmassoc.net>;
 *:  Published 2005 by Charles Maier Associates Limited;
 *;  Released under the GNU General Public Licence v2 and later;
 *
 *--------------------------------------------------------------------*/

#ifndef HEXVIEW_SOURCE
#define HEXVIEW_SOURCE

#include <stdio.h>
#include <ctype.h>
#include <stdint.h>

#include "../tools/memory.h"
#include "../tools/number.h"

void hexview (const void * memory, size_t offset, size_t extent, FILE *fp) 

{
	byte * origin = (byte *)(memory);
	unsigned field = sizeof (extent) + sizeof (extent);
	unsigned block = 0x10;
	size_t lower = block * (offset / block);
	size_t upper = block + lower;
	size_t index = 0;
	char buffer [sizeof (extent) + sizeof (extent) + 72];
	char * output;
	while (lower < offset + extent) 
	{
		output = buffer + field;
		for (index = lower; output-- > buffer; index >>= 4) 
		{
			*output = DIGITS_HEX [index & 0x0F];
		}
		output = buffer + field;
		*output++ = ' ';
		for (index = lower; index < upper; index++) 
		{
			if (index < offset) 
			{
				*output++ = ' ';
				*output++ = ' ';
			}
			else if (index < offset + extent) 
			{
				*output++ = DIGITS_HEX [(origin [index-offset] >> 4) & 0x0F];
				*output++ = DIGITS_HEX [(origin [index-offset] >> 0) & 0x0F];
			}
			else 
			{
				*output++ = ' ';
				*output++ = ' ';
			}
			*output++ = ' ';
		}
		for (index = lower; index < upper; index++) 
		{
			if (index < offset) 
			{
				*output++ = ' ';
			}
			else if (index < offset + extent) 
			{
				unsigned c = origin [index-offset];
				*output++ = isprint (c)? c: '.';
			}
			else 
			{
				*output++ = ' ';
			}
		}
		*output++ = '\n';
		*output++ = '\0';
		fputs (buffer, fp);
		lower += block;
		upper += block;
	}

#if 0

	output = buffer;
	*output++ = '\n';
	*output++ = '\0';
	fputs (buffer, fp);

#endif

	return;
}

#endif

