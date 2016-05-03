/*====================================================================*
 *
 *   void hexwrite (signed fd, const void * memory, size_t extent);
 *
 *   memory.h
 *
 *   write a memory region as hexadecimal character stream of space
 *   delimited octets; terminate the stream with a newline; this is
 *   used for logging unwanted or unknown Ethernet frames for later
 *   analysis;
 *   
 *.  Motley Tools by Charles Maier <cmaier@cmassoc.net>;
 *:  Published 2001-2006 by Charles Maier Associates Limited;
 *;  Released under the GNU General Public Licence v2 and later;
 *
 *--------------------------------------------------------------------*/

#include <unistd.h>

#include "../tools/number.h"
#include "../tools/memory.h"

void hexwrite (signed fd, const void * memory, size_t extent) 

{
	byte * offset = (byte *)(memory);
	while (extent--) 
	{
		byte c = * offset;
		write (fd, &DIGITS_HEX [(c >> 4) & 0x0F], sizeof (char));
		write (fd, &DIGITS_HEX [(c >> 0) & 0x0F], sizeof (char));
		if (extent) 
		{
			write (fd, " ", sizeof (char));
		}
		offset++;
	}
	write (fd, "\n", sizeof (char));
	return;
}

