/*====================================================================*
 *
 *   void error (int status, errno_t number, const char *format, ...);
 *
 *   error.h
 *
 *   custom implementation of the GNU error() function for systems
 *   that do not have it;
 *
 *.  Motley Tools by Charles Maier <cmaier@cmassoc.net>;
 *:  Published 2005 by Charles Maier Associates Limited;
 *;  Released under the GNU General Public Licence v2 and later;
 *
 *--------------------------------------------------------------------*/

#ifndef ERROR_SOURCE
#define ERROR_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#include "../tools/types.h"
#include "../tools/error.h"

#ifdef __GNUC__

__attribute__ ((format (printf, 3, 4))) 

#endif

void error (int status, errno_t number, const char *format, ...) 

{
	extern const char *program_name;
	if ((program_name) && (*program_name)) 
	{
		fprintf (stderr, "%s: ", program_name);
	}
	if ((format) && (*format)) 
	{
		va_list arglist;
		va_start (arglist, format);
		vfprintf (stderr, format, arglist);
		va_end (arglist);
	}
	if (number) 
	{
		fprintf (stderr, ": %s", strerror (number));
	}
	fprintf (stderr, "\n");
	fflush (stderr);
	if (status) 
	{
		exit (status);
	}
	return;
}

#endif

