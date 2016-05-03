/*====================================================================*
 *
 *   bool isbreak (SCAN * content, const char *charset);
 *
 *   scan.h
 *
 *   return true if the current input character is a member of the 
 *   specified character set; comparison is case sensitive;
 *
 *.  released 2005 by charles maier associates ltd. for public use;
 *:  compiled on debian gnu/linux with gcc 2.95 compiler;
 *;  licensed under the gnu public license version two;
 *
 *--------------------------------------------------------------------*/

#ifndef ISBREAK_SOURCE
#define ISBREAK_SOURCE

#include "../scan/scan.h"

bool isbreak (SCAN * content, const char *charset) 

{
	if (content->final < content->limit) while (*charset) 
	{
		if (*charset == *content->final) 
		{
			return (true);
		}
		charset++;
	}
	return (false);
}

#endif

