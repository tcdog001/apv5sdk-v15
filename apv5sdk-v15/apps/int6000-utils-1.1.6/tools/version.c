/*====================================================================*
 *
 *   void version();
 *
 *   version.h
 *
 *   print program and package version information on stdout; 
 *
 *.  Motley Tools by Charles Maier <cmaier@cmassoc.net>;
 *:  Published 2005 by Charles Maier Associates Limited;
 *;  Released under the GNU General Public Licence v2 and later;
 *
 *--------------------------------------------------------------------*/

#ifndef PUTOPT_SOURCE
#define PUTOPT_SOURCE

#include <stdio.h>
#include <stdlib.h>

#include "../tools/version.h"

void version () 

{
	extern const char *program_name;
	printf ("%s ", program_name);

#if 0

	printf (PACKAGE " ");

#else

	printf (ARCHIVE "-");

#endif

	printf (VERSION ".");
	printf (RELEASE " ");
	printf ("ISO C ");

#if 0

	printf (CONTACT " ");

#else

	printf (COMPANY " ");

#endif

	printf (COMPILE "\n");
	return;
}

#endif

