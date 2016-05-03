/*====================================================================*
 *
 *   bool istoken (SCAN * content, const char *literal);
 *
 *   scan.h
 *
 *   return true if the current token substring matches the given literal
 *   string; comparison is case sensitive;
 *
 *.  released 2005 by charles maier associates ltd. for public use;
 *:  compiled on debian gnu/linux with gcc 2.95 compiler;
 *;  licensed under the gnu public license version two;
 *
 *--------------------------------------------------------------------*/

#ifndef ISTOKEN_SOURCE
#define ISTOKEN_SOURCE

#include <ctype.h>

#include "../scan/scan.h"

bool istoken (SCAN * content, const char *literal) 

{
	const char * current = content->first;
	while ((current < content->final) && (*current == *literal)) 
	{
		current++;
		literal++;
	}
	return (*literal == (char)(0));
}

#endif

