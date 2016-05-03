/*====================================================================*
 *
 *   void scanbreakmatch (SCAN * content);
 *
 *   scan.h
 * 
 *   append the next input character to the current token if it is
 *   the same as the first token character;
 *
 *.  released 2005 by charles maier associates ltd. for public use;
 *:  compiled on debian gnu/linux with gcc 2.95 compiler;
 *;  licensed under the gnu public license version two;
 *
 *--------------------------------------------------------------------*/

#ifndef SCANBREAKMATCH_SOURCE
#define SCANBREAKMATCH_SOURCE

#include "../scan/scan.h"

void scanbreakmatch (SCAN * content) 

{
	if ((content->final < content->limit) && (*content->final != *content->first)) 
	{
		content->final++;
	}
	return;
}

#endif

