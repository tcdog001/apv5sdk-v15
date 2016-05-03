/*====================================================================*
 *
 *   size_t strfbits (char  buffer [], size_t length, const char *operands [], const char * operator, unsigned flagword);
 *
 *   format.h
 *
 *   format buffer with an enumerated list of the bits in a flagword; 
 *   each flagword bit position corresponds to a string in operands[]
 *   and operator is the string separating formatted operands;
 *
 *   enumeration continues until all bits are enumerated or operands
 *   are exhausted or the buffer fills;
 *
 *   for example, the following formats buffer with the literal string
 *   "one, three, five, six" since those bits are set;
 *
 *   char buffer[100];
 *   const char operator = ", ";
 *   const char *operands[] = 
 *   { 
 *      "zero", 
 *      "one", 
 *      "two", 
 *      "three", 
 *      "four", 
 *      "five", 
 *      "six", 
 *      "seven", 
 *      "eight",
 *      "nine",
 *      "ten",
 *      (char *)(0) 
 *   };
 *   flag_t flags = 0x006C;
 *
 *   strfbits (buffer, sizeof(buffer), operator, operands, flags);
 *
 *   we decrement length before starting to reserve room for the NUL
 *   terminator; not room ... no write; we then add length to buffer
 *   before to compute the terminator address then subtract it after
 *   to compute the buffer start; this minimizes indexing and offset 
 *   calculations within the loop; 
 *
 *.  Released 2005 by Charles Maier Associates Ltd for private use;
 *:  Licensed under GNU General Public License version 2 and later;
 *;  Designed for CMLinux based on LFS and Debian GNU/Linux;
 *
 *--------------------------------------------------------------------*/

#ifndef STRFBITS_SOURCE
#define STRFBITS_SOURCE

#include <unistd.h>

#include "../tools/memory.h"
#include "../tools/flags.h"

size_t strfbits (char  buffer [], size_t length, const char * operands [], const char * operator, unsigned flagword) 

{
	char * string = (char *)(buffer);
	const char *separator = "";

#ifdef INTELLON_SAFEMODE

	if (buffer == (char *) (0)) 
	{
		return (0);
	}
	if (operands == (const char **) (0)) 
	{
		return (0);
	}
	if (operator == (char *) (0)) 
	{
		operator = ", ";
	}

#endif

	if (length-- > 0) 
	{
		buffer += length;
		while ((flagword) && (*operands)) 
		{
			if (flagword & 1) 
			{
				const char *symbol;
				for (symbol = separator; (*symbol) && (string < buffer); symbol++) 
				{
					*string++ = *symbol;
				}
				for (symbol = *operands; (*symbol) && (string < buffer); symbol++) 
				{
					*string++ = *symbol;
				}
				separator = operator;
			}
			flagword >>= 1;
			operands++;
		}
		*string = (char) (0);
		buffer -= length;
	}
	return (string - (char *)(buffer));
}

#endif

