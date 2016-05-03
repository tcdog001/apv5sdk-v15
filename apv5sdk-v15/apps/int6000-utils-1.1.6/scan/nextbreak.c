/*====================================================================*
 *
 *   void nextbreak(SCAN * content);
 *
 *   scan.h
 *
 *   append the current input buffer character to the current token 
 *   by incrementing the input character pointer but do not advance
 *   past the end of the input buffer;
 *
 *.  released 2005 by charles maier associates ltd. for public use;
 *:  compiled on debian gnu/linux with gcc 2.95 compiler;
 *;  licensed under the gnu public license version two;
 *
 *--------------------------------------------------------------------*/

#ifndef NEXTBREAK_SOURCE
#define NEXTBREAK_SOURCE

#include "../scan/scan.h"

void nextbreak (SCAN * content) 

{
	if (content->final < content->limit) 
	{
		content->final++;
	}
	return;
}

#endif

