/*====================================================================*
 *   
 *   bool checkfilename (const char * pathname);
 *
 *   files.h
 *   
 *   confirm that the filename portion of a pathname string contains
 *   only letters, digits, periods, underscores and hyphens; 
 *
 *   this prevents users from entering an Ethernet address where a 
 *   filename should appear on the command line; Ethernet addresses 
 *   are also valid filenames;
 *   
 *.  Motley Tools by Charles Maier <cmaier@cmassoc.net>;
 *:  Published 2005 by Charles Maier Associates Limited;
 *;  Released under the GNU General Public Licence v2 and later;
 *
 *--------------------------------------------------------------------*/

#ifndef CHECKFILENAME_SOURCE
#define CHECKFILENAME_SOURCE

#include <string.h>
#include <ctype.h>

#include "../tools/files.h"

bool checkfilename (const char * pathname) 

{
	const char * filename = pathname;
	while (*pathname) 
	{
		if (*pathname++ == PATH_C_EXTENDER) 
		{
			filename = pathname;
		}
	}
	while (isalnum (*filename) || (*filename == '.') || (*filename == '_') || (*filename == '-')) 
	{
		filename++;
	}
	return (*filename == (char) (0));
}

#endif

