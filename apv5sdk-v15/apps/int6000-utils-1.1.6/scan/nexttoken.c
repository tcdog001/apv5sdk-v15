/*====================================================================*
 *
 *   void nexttoken(SCAN * content);
 *
 *   scan.h
 *
 *   collect the next non-space token;
 *
 *.  released 2005 by charles maier associates ltd. for public use;
 *:  compiled on debian gnu/linux with gcc 2.95 compiler;
 *;  licensed under the gnu public license version two;
 *
 *--------------------------------------------------------------------*/

#ifndef NEXTTOKEN_SOURCE
#define NEXTTOKEN_SOURCE

#include "../scan/scan.h"

void nexttoken (SCAN * content) 

{
	scanspace (content);
	scanflush (content);
	scantoken (content);
	return;
}

#endif

