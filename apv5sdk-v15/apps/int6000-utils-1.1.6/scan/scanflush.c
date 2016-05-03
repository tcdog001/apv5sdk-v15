/*====================================================================*
 *
 *   void scanflush(SCAN * content);
 *
 *   scan.h
 * 
 *   flush the current token by aligning the first character pointer 
 *   to the input character pointer, creating in a zero-length token 
 *   string;
 *
 *.  released 2005 by charles maier associates ltd. for public use;
 *:  compiled on debian gnu/linux with gcc 2.95 compiler;
 *;  licensed under the gnu public license version two;
 *
 *--------------------------------------------------------------------*/

#ifndef SCANFLUSH_SOURCE
#define SCANFLUSH_SOURCE

#include "../scan/scan.h"

void scanflush (SCAN * content) 

{
	content->first = content->final;
	return;
}

#endif

