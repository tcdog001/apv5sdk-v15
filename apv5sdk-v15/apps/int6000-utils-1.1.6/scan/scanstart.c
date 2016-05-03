/*====================================================================*
 *
 *   void scanstart (SCAN * content);
 *
 *   scan.h
 *
 *   start the scan process at the start of the input buffer; this
 *   collapses the token to zero length and clears the token class;
 *
 *.  released 2005 by charles maier associates ltd. for public use;
 *:  compiled on debian gnu/linux with gcc 2.95 compiler;
 *;  licensed under the gnu public license version two;
 *
 *--------------------------------------------------------------------*/

#ifndef SCANSTART_SOURCE
#define SCANSTART_SOURCE

#include "../scan/scan.h"

void scanstart (SCAN * content) 

{
	content->first = content->start;
	content->final = content->start;
	content->index = content->start;
	return;
}

#endif

