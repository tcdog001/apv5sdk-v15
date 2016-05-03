/*====================================================================*
 *
 *   uint64_t uintspec (const char *number, uint64_t minimum, uint64_t maximum);
 *
 *   number.h
 *
 *   convert a numeric string to an unsigned integer; confirm that
 *   the result does not exceed the specified range; report errors 
 *   and terminate the program on error;
 *
 *.  Motley Tools by Charles Maier <cmaier@cmassoc.net>;
 *:  Published 2005 by Charles Maier Associates Limited;
 *;  Released under the GNU General Public Licence v2 and later;
 *
 *--------------------------------------------------------------------*/

#ifndef UINTSPEC_SOURCE
#define UINTSPEC_SOURCE

#include <stdlib.h>
#include <ctype.h>
#include <errno.h>

#include "../tools/number.h"
#include "../tools/error.h"

uint64_t uintspec (const char * string, uint64_t minimum, uint64_t maximum) 

{
	const char * number = string;
	unsigned radix = RADIX_DEC;
	uint64_t value = 0;
	unsigned digit;
	if (*number == '0') 
	{
		number++;
		if ((*number == 'b') || (*number == 'B')) 
		{
			radix = RADIX_BIN;
			number++;
		}
		else if ((*number == 'x') || (*number == 'X')) 
		{
			radix = RADIX_HEX;
			number++;
		}
	}
	while ((digit = todigit (*number)) < radix) 
	{
		value *= radix;
		value += digit;
		number++;
	}
	if (*number) 
	{
		error (1, EINVAL, "Have '%s' but want an unsigned integer", string);
	}
	if ((value < minimum) || (value > maximum)) 
	{
		error (1, ERANGE, "Have '%s' but want %llu thru %llu", string, minimum, maximum);
	}
	return (value);
}

#endif

