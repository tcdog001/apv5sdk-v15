/*====================================================================*
 *
 *   bool isclass (SCAN * content, const char *charset);
 *
 *   scan.h
 *
 *   return true if the current class character is a member of the given
 *   character set; comparison is case sensitive;
 *
 *.  released 2005 by charles maier associates ltd. for public use;
 *:  compiled on debian gnu/linux with gcc 2.95 compiler;
 *;  licensed under the gnu public license version two;
 *
 *--------------------------------------------------------------------*/

#ifndef ISCLASS_SOURCE
#define ISCLASS_SOURCE

#include <string.h>

#include "../scan/scan.h"

bool isclass (SCAN * content, const char *charset) 

{
	if (content->class) 
	{
		return ((bool)(strchr (charset, content->class)));
	}
	return (false);
}

#endif

