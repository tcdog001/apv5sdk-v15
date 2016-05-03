/*====================================================================*
 *
 *   char * tokentext (SCAN * content);
 *
 *   scan.h
 *
 *   return the address of a constant string containing a copy of 
 *   the current token; calling applications may duplicate it, if 
 *   needed; the string content will, and the address may, change 
 *   with each call to tokentext;
 *
 *   the token buffer size expands, as needed, using a fibonacci 
 *   expansion; don't ask; it's magic;
 *
 *.  released 2005 by charles maier associates ltd. for public use;
 *:  compiled on debian gnu/linux with gcc 2.95 compiler;
 *;  licensed under the gnu public license version two;
 *
 *--------------------------------------------------------------------*/

#ifndef TOKENTEXT_SOURCE
#define TOKENTEXT_SOURCE

#include <stdlib.h>
#include <memory.h>

#include "../scan/scan.h"

char * tokentext (SCAN * content) 

{
	while (content->width <= (content->final - content->first)) 
	{
		free (content->token);
		content->width = content->width + content->block;
		content->block = content->width - content->block;
		content->token = malloc (content->width);
	}
	memcpy (content->token, content->first, content->final - content->first);
	content->token [content->final - content->first] = (char)(0);
	return (content->token);
}

#endif

