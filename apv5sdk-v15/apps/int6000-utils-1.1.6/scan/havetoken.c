/*====================================================================*
 *
 *   bool havetoken(SCAN * content, const char *literal);
 *
 *   scan.h
 *
 *
 *.  released 2005 by charles maier associates ltd. for public use;
 *:  compiled on debian gnu/linux with gcc 2.95 compiler;
 *;  licensed under the gnu public license version two;
 *
 *--------------------------------------------------------------------*/

#ifndef HAVETOKEN_SOURCE
#define HAVETOKEN_SOURCE

#include "../scan/scan.h"

bool havetoken (SCAN * content, const char *literal) 

{
	if (istoken (content, literal)) 
	{
		nexttoken (content);
		return (true);
	}
	return (false);
}

#endif

