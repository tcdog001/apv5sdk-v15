/*====================================================================*
 *
 *   bool havematch(SCAN * content);
 *
 *   scan.h
 *
 *   return true if the input token character matches the first and
 *   advance to the next character; otherwise, return false and do
 *   nothing;
 *
 *.  released 2005 by charles maier associates ltd. for public use;
 *:  compiled on debian gnu/linux with gcc 2.95 compiler;
 *;  licensed under the gnu public license version two;
 *
 *--------------------------------------------------------------------*/

#ifndef HAVEMATCH_SOURCE
#define HAVEMATCH_SOURCE

#include "../scan/scan.h"

bool havematch (SCAN * content) 

{
	if (ismatch (content)) 
	{
		nextbreak (content);
		return (true);
	}
	return (false);
}

#endif

