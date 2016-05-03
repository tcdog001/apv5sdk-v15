/*====================================================================*
 *
 *   unsigned tovalue (SCAN * content);
 *
 *   scan.h
 *
 *   copy the current token to a user supplied buffer of specified 
 *   length; the token is NUL terminated and truncated when needed 
 *   to fit the buffer;
 *
 *.  released 2005 by charles maier associates ltd. for public use;
 *:  compiled on debian gnu/linux with gcc 2.95 compiler;
 *;  licensed under the gnu public license version two;
 *
 *--------------------------------------------------------------------*/

#ifndef TOVALUE_SOURCE
#define TOVALUE_SOURCE

#include <ctype.h>

#include "../scan/scan.h"

unsigned tovalue (SCAN * content) 

{
	unsigned value = 0;
	content->index = content->first;
	while ((content->index < content->final) && isdigit (*content->index)) 
	{
		value *= 10;
		value += *content->index - '0';
		content->index++;
	}
	return (value);
}

#endif

