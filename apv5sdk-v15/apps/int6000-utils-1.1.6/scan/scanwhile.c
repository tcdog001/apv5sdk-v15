/*====================================================================*
 *
 *   void scanwhile (SCAN * content, const char *charset);
 *
 *   scan.h
 * 
 *   append input characters to the current token while they are 
 *   members of the specified character set; stop on the first one
 *   that is not a member; 
 *
 *.  released 2005 by charles maier associates ltd. for public use;
 *:  compiled on debian gnu/linux with gcc 2.95 compiler;
 *;  licensed under the gnu public license version two;
 *
 *--------------------------------------------------------------------*/

#ifndef SCANWHILE_SOURCE
#define SCANWHILE_SOURCE

#include "../scan/scan.h"

void scanwhile (SCAN * content, const char *charset) 

{
	while ((content->final < content->limit) && isbreak (content, charset)) 
	{
		content->final++;
	}
	return;
}

#endif

