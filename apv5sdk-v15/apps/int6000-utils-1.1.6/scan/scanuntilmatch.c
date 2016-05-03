/*====================================================================*
 *
 *   void scanuntil (SCAN * content);
 *
 *   scan.h
 *
 *   append input buffer characters to the current token substring while
 *   they are not member of the specified character set; stop on the first
 *   one that is a member; it marks the start of another scan;
 *
 *.  released 2005 by charles maier associates ltd. for public use;
 *:  compiled on debian gnu/linux with gcc 2.95 compiler;
 *;  licensed under the gnu public license version two;
 *
 *--------------------------------------------------------------------*/

#ifndef SCANUNTILMATCH_SOURCE
#define SCANUNTILMATCH_SOURCE

#include "../scan/scan.h"

void scanuntilmatch (SCAN * content) 

{
	while ((content->final < content->limit) && (*content->final != *content->first)) 
	{
		content->final++;
	}
	return;
}

#endif

