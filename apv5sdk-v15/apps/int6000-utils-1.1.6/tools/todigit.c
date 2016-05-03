/*====================================================================*
 *
 *   unsigned todigit (unsigned c);
 *
 *   number.h
 *   
 *   return the unsigned integer equivalent of an ASCII digit or the
 *   value UCHAR_MAX on error;
 *
 *.  Motley Tools by Charles Maier <cmaier@cmassoc.net>;
 *:  Published 2005 by Charles Maier Associates Limited;
 *;  Released under the GNU General Public Licence v2 and later;
 *
 *--------------------------------------------------------------------*/

#ifndef TODIGIT_SOURCE
#define TODIGIT_SOURCE

#include <limits.h>

#include "../tools/number.h"

unsigned todigit (unsigned c) 

{
	if ((c >= '0') && (c <= '9')) 
	{
		return (c - '0');
	}
	if ((c >= 'A') && (c <= 'Z')) 
	{
		return (c - 'A' + 10);
	}
	if ((c >= 'a') && (c <= 'z')) 
	{
		return (c - 'a' + 10);
	}
	return (UCHAR_MAX);
}

#endif

