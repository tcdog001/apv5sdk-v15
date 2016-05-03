/*====================================================================*
 *
 *   void endian ( void * memory, size_t extent);
 *
 *   reverse the order of bytes within a multi-byte memory region; 
 *   return no value;
 *
 *.  Motley Tools by Charles Maier <cmaier@cmassoc.net>;
 *:  Published 2005 by Charles Maier Associates Limited;
 *;  Released under the GNU General Public Licence v2 and later;
 *
 *--------------------------------------------------------------------*/

#ifndef ENDIAN_SOURCE
#define ENDIAN_SOURCE

#include "../tools/memory.h"
 
void endian ( void * memory, size_t extent) 

{
	register byte * first = (byte *)(memory);
	register byte * final = first + extent;
	while (first < final) 
	{
		register byte byte = *first;
		*first++ = *--final;
		*final = byte;
	}
	return;
}

#endif
 

