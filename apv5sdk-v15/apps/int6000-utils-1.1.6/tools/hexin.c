/*====================================================================*
 *
 *   size_t hexin (void * memory, size_t extent, FILE *fp);
 *
 *   memory.h
 *
 *   read a file and convert hexadecimal octets to binary bytes then 
 *   store them in consecutive memory locations up to a given length; 
 *   return the actual number of bytes stored;
 *
 *   digits may be consecutive or separated by white space consisting 
 *   of spaces, tabs, linefeeds, carriage returns, formfeeds or other
 *   characters such as punctuation; C-style or script-style comments 
 *   are treated as white space;
 *
 *.  Motley Tools by Charles Maier <cmaier@cmassoc.net>;
 *:  Published 2005 by Charles Maier Associates Limited;
 *;  Released under the GNU General Public Licence v2 and later;
 *
 *--------------------------------------------------------------------*/

#ifndef HEXIN_SOURCE
#define HEXIN_SOURCE

#include <stdio.h>
#include <stdint.h>
#include <errno.h>

#include "../tools/memory.h"
#include "../tools/error.h"

#ifndef NUMBER_HEADER
#define RADIX_HEX 16
#define RADIX_DEC 10
#endif

size_t hexin (void * memory, size_t extent, FILE *fp) 

{
	uint8_t * origin = (byte *)(memory);
	uint8_t * offset = (byte *)(memory);
	unsigned digits = 0;
	signed c;
	while (((c = getc (fp)) != EOF) && (c != ';') && (extent)) 
	{
		if (c == '#') 
		{
			do 
			{
				c = getc (fp);
			}
			while ((c != '\n') && (c != EOF));
		}
		else if (c == '/') 
		{
			c = getc (fp);
			if (c == '/') 
			{
				do 
				{
					c = getc (fp);
				}
				while ((c != '\n') && (c != EOF));
			}
			else if (c == '*') 
			{
				while ((c != '/') && (c != EOF)) 
				{
					while ((c != '*') && (c != EOF)) 
					{
						c = getc (fp);
					}
					c = getc (fp);
				}
			}
		}
		else if ((c >= '0') && (c <= '9')) 
		{
			*offset *= RADIX_HEX;
			*offset += c - '0';
			if (digits++ & 1) 
			{
				offset++;
				extent--;
			}
		}
		else if ((c >= 'A') && (c <= 'F')) 
		{
			*offset *= RADIX_HEX;
			*offset += RADIX_DEC;
			*offset += c - 'A';
			if (digits++ & 1) 
			{
				offset++;
				extent--;
			}
		}
		else if ((c >= 'a') && (c <= 'f')) 
		{
			*offset *= RADIX_HEX;
			*offset += RADIX_DEC;
			*offset += c - 'a';
			if (digits++ & 1) 
			{
				offset++;
				extent--;
			}
		}
	}
	if (digits & 1) 
	{
		error (0, ENOTSUP, "Odd number of hex digits in source");
		return (-1);
	}
	return (offset - origin);
}

#endif

