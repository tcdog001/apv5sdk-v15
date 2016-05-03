/*====================================================================*
 *
 *   void markup (signed fd, signed indent, char const * format, ...);
 *
 *   format.h
 *
 *   write an indented and formatted wtring to the specified device;
 *
 *.  released 2005 by charles maier associates ltd. for public use;
 *:  compiled on debian gnu/linux with gcc 2.95 compiler;
 *;  licensed under the gnu public license version two;
 *
 *--------------------------------------------------------------------*/

#ifndef MARKUP_SOURCE
#define MARKUP_SOURCE

#include <stdio.h>
#include <stdarg.h>
#include <unistd.h>
#include <string.h>

#include "../tools/format.h"

#ifdef __GNUC__

__attribute__ ((format (printf, 3, 4))) 

#endif

void markup (signed fd, signed indent, char const * format, ...) 

{
	static char tab = '\t';
	static char end = '\n';
	char buffer [1024];
	signed length = 0;
	while (indent-- > 0) 
	{
		write (fd, &tab, sizeof (tab));
	}
	if ((format) && (*format)) 
	{
		va_list arglist;
		va_start (arglist, format);
		length = vsnprintf (buffer, sizeof (buffer), format, arglist);
		va_end (arglist);
	}
	write (fd, buffer, length);
	write (fd, &end, sizeof (end));
	return;
}

#endif

