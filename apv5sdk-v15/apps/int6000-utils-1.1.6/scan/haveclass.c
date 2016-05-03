/*====================================================================*
 *
 *   bool haveclass(SCAN * content, const char *charset);
 *
 *   scan.h 
 *
 *
 *.  released 2005 by charles maier associates ltd. for public use;
 *:  compiled on debian gnu/linux with gcc 2.95 compiler;
 *;  licensed under the gnu public license version two;
 *
 *--------------------------------------------------------------------*/

#ifndef HAVECLASS_SOURCE
#define HAVECLASS_SOURCE

#include "../scan/scan.h"

bool haveclass (SCAN * content, const char *charset) 

{
	if (isclass (content, charset)) 
	{
		nexttoken (content);
		return (true);
	}
	return (false);
}

#endif

