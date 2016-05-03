/*====================================================================*
 *
 *   void scanbreak (SCAN * content, const char *charset);
 *
 *   scan.h
 * 
 *   append the next input character to the current token if it is
 *   in charaset;
 *
 *.  released 2005 by charles maier associates ltd. for public use;
 *:  compiled on debian gnu/linux with gcc 2.95 compiler;
 *;  licensed under the gnu public license version two;
 *
 *--------------------------------------------------------------------*/

#ifndef SCANBREAK_SOURCE
#define SCANBREAK_SOURCE

#include "../scan/scan.h"

void scanbreak (SCAN * content, const char *charset) 

{
	if ((content->final < content->limit) && isbreak (content, charset)) 
	{
		content->final++;
	}
	return;
}

#endif

