/*====================================================================*
 *
 *   const char *filepart (const char *pathname);
 *
 *   files.h
 *
 *   return address of the filename portion of a pathname string;
 *
 *   this function is an alternative to the POSIX basename function
 *   for systems that do not have it;
 *
 *.  published 2005 by charles maier associates limited for internal use;
 *:  compiled for debian gnu/linux environment using gcc 2.95 compiler;
 *;  licensed according to the GNU Public License version two;
 *
 *--------------------------------------------------------------------*/

#ifndef FILEPART_SOURCE
#define FILEPART_SOURCE

#include "../tools/files.h"

const char *filepart (const char *pathname) 

{
	const char * filename = pathname;
	while (*pathname) 
	{

#if 0

		if (*pathname++ == PATH_C_EXTENDER) 
		{
			filename = pathname;
		}

#else

		if ((*pathname == '/') || (*pathname == '\\')) 
		{
			filename = pathname + 1;
		}
		pathname++;

#endif

	}
	return (filename);
}

#endif

