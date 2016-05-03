/*====================================================================*
 *
 *   void scanreset (SCAN * content);
 *
 *   scan.h
 *
 *   begin the scan process at the start of the token string; this
 *   collapses the token to zero length and clears the token class;
 *
 *.  released 2005 by charles maier associates ltd. for public use;
 *:  compiled on debian gnu/linux with gcc 2.95 compiler;
 *;  licensed under the gnu public license version two;
 *
 *--------------------------------------------------------------------*/

#ifndef SCANRESET_SOURCE
#define SCANRESET_SOURCE

#include "../scan/scan.h"

void scanreset (SCAN * content) 

{
	content->final = content->first;
	return;
}

#endif

