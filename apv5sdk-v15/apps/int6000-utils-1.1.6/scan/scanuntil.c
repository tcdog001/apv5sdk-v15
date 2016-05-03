/*====================================================================*
 *
 *   void scanuntil (const char *charsetm SCAN * content);
 *
 *   scan.h
 * 
 *   append input characters to the current token while they are not 
 *   members of the specified character set; stop on the first one 
 *   that is a member; 
 *
 *.  released 2005 by charles maier associates ltd. for public use;
 *:  compiled on debian gnu/linux with gcc 2.95 compiler;
 *;  licensed under the gnu public license version two;
 *
 *--------------------------------------------------------------------*/

#ifndef SCANUNTIL_SOURCE
#define SCANUNTIL_SOURCE

#include "../scan/scan.h"

void scanuntil (SCAN * content, const char *charset) 

{
	while ((content->final < content->limit) && !isbreak (content, charset)) 
	{
		content->final++;
	}
	return;
}

#endif

