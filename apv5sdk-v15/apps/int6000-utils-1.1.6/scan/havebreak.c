/*====================================================================*
 *
 *   bool havebreak(SCAN * content, const char *charset);
 *
 *   scan.h
 *
 *   advance to the next input buffer character only if the current
 *   current input buffer character is in charset; this function is
 *   useful for detecting optional delimiters;
 *
 *   For example:
 *
 *   scanalpha (&content);
 *   if (havebreak (&content, "="))
 *   {
 *      scandigit (&content);
 *   }
 *
 *.  released 2005 by charles maier associates ltd. for public use;
 *:  compiled on debian gnu/linux with gcc 2.95 compiler;
 *;  licensed under the gnu public license version two;
 *
 *--------------------------------------------------------------------*/

#ifndef HAVEBREAK_SOURCE
#define HAVEBREAK_SOURCE

#include "../scan/scan.h"

bool havebreak (SCAN * content, const char *charset) 

{
	if (isbreak (content, charset)) 
	{
		nextbreak (content);
		return (true);
	}
	return (false);
}

#endif

