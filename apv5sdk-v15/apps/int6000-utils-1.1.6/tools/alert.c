/*====================================================================*
 *
 *   void alert (const char *format, ...);
 *
 *   error.h
 *
 *   alternative to the GNU error() function without the exitcode or
 *   errno argument; it never exits and so always returns; output is
 *   always to stderr;
 *
 *.  Motley Tools by Charles Maier <cmaier@cmassoc.net>;
 *:  Published 2005 by Charles Maier Associates Limited;
 *;  Released under the GNU General Public Licence v2 and later;
 *
 *--------------------------------------------------------------------*/

#ifndef ALERT_SOURCE
#define ALERT_SOURCE

#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#include "../tools/error.h"

#ifdef __GNUC__

__attribute__ ((format (printf, 1, 2))) 

#endif

void alert (const char *format, ...) 

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
	fprintf (stderr, "\n");
	fflush (stderr);
	return;
}

#endif

