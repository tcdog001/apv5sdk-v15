/*====================================================================*
 *
 *   void assist (const char * name, const char * type, const struct _code_ list [], size_t size, FILE *fp);
 *
 *   symbol.h
 *   
 *   print program name followed by an error message containing the
 *   type of name expected, the incorrect name and a list of correct
 *   name;
 *   
 *.  Motley Tools by Charles Maier <cmaier@cmassoc.net>;
 *:  Published 2005 by Charles Maier Associates Limited;
 *;  Released under the GNU General Public Licence v2 and later;
 *
 *--------------------------------------------------------------------*/

#ifndef ASSIST_SOURCE
#define ASSIST_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "../tools/symbol.h"
#include "../tools/error.h"

void assist (const char * name, const char * type, const struct _code_ list [], size_t size, FILE *fp) 

{
	extern const char * program_name;
	fprintf (fp, "%s: ", program_name);
	fprintf (fp, "%s: ", strerror (ENOTSUP));
	if (name) 
	{
		fprintf (fp, "Have %s '%s' but want ", type, name);
	}
	else 
	{
		fprintf (fp, "Need %s ", type);
	}
	expect (list, size, fp);
	fprintf (fp, ".\n");
	exit (1);
}

#endif

