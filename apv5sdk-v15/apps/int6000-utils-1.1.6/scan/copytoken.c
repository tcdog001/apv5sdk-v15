/*====================================================================*
 *
 *   void copytoken (SCAN * content, char buffer [], signed length);
 *
 *   scan.h
 *
 *   copy the current token to a user supplied buffer of specified 
 *   length; the token is truncated and NUL terminated when needed 
 *   to fit the buffer;
 *
 *.  released 2005 by charles maier associates ltd. for public use;
 *:  compiled on debian gnu/linux with gcc 2.95 compiler;
 *;  licensed under the gnu public license version two;
 *
 *--------------------------------------------------------------------*/

#ifndef COPYTOKEN_SOURCE
#define COPYTOKEN_SOURCE

# include "../scan/scan.h"

void copytoken (SCAN * content, char buffer [], signed length) 

{
	if (length-- > 0) 
	{
		content->index = content->first;
		while ((length-- > 0) && (content->index < content->final)) 
		{
			*buffer++ = *content->index++;
		}
		*buffer = (char)(0);
	}
	return;
}

#endif

