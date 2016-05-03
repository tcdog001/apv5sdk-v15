/*====================================================================*
 *
 *   uint64_t basespec (const char * string, unsigned base, unsigned size);
 *
 *   number.h
 *
 *   convert a character string to an equivalent unsigned integer and
 *   return the result; terminate the program on failure;
 *
 *   the base argument is the number base to be used for conversion; 
 *   base 0 permits the number base to be determined by the string 
 *   string prefix; 0b, 0d or 0x for binary, deciaml or hex;
 *
 *   the size argument is the maximum number of bits permitted in the
 *   resulting string;
 *
 *.  Motley Tools by Charles Maier <cmaier@cmassoc.net>;
 *:  Published 2001-2006 by Charles Maier Associates Limited;
 *;  Released under the GNU General Public Licence v2 and later;
 *
 *--------------------------------------------------------------------*/

#ifndef BASESPEC_SOURCE
#define BASESPEC_SOURCE

#include <stdlib.h>
#include <ctype.h>

#include "../tools/number.h"
#include "../tools/error.h"

uint64_t basespec (const char * string, unsigned base, unsigned size) 

{
	const char * number = string;
	unsigned radix = RADIX_DEC;
	uint64_t limit = 0;
	uint64_t value = 0;
	unsigned digit = 0;
	limit = ~limit;
	if (size < sizeof (limit)) 
	{
		limit <<= size << 3;
		limit = ~limit;
	}
	if (base) 
	{
		radix = base;
	}
	if (*number == '0') 
	{
		number++;
		if ((*number == 'b') || (*number == 'B')) 
		{
			radix = RADIX_BIN;
			number++;
		}
		else if ((*number == 'd') || (*number == 'D')) 
		{
			radix = RADIX_DEC;
			number++;
		}
		else if ((*number == 'x') || (*number == 'X')) 
		{
			radix = RADIX_HEX;
			number++;
		}
	}
	if ((base) && (base != radix)) 
	{
		error (1, EINVAL, "%s is not base %d notation", string, base);
	}
	while ((digit = todigit (*number)) < radix) 
	{
		value *= radix;
		value += digit;
		if (value > limit) 
		{
			error (1, ERANGE, "%s exceeds %d bits", string, (size << 3));
		}
		number++;
	}

#ifdef WIN32

	while (isspace (*number)) 
	{
		number++;
	}

#endif

	if (*number) 
	{
		error (1, EINVAL, "%s is not base %d notation", string, radix);
	}
	return (value);
}

#endif

