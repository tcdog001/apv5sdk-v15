/*====================================================================*
 *
 *   unsigned char tokentype (SCAN * content);
 *
 *   scan.h
 *
 *   return the current token class as determined by scantoken();
 *
 *   the return value may be ' ' for spaces, 'A' for letters and '0' for
 *   numbers; they indicate that scantoken() found one or more characters
 *   of that class on its last scan;
 *
 *   any other value indicates that scantoken() collected one character of
 *   that value; for example, a return value of ';' means that scantoken()
 *   appended one semicolon;
 *
 *.  released 2005 by charles maier associates ltd. for public use;
 *:  compiled on debian gnu/linux with gcc 2.95 compiler;
 *;  licensed under the gnu public license version two;
 *
 *--------------------------------------------------------------------*/

#ifndef TOKENTYPE_SOURCE
#define TOKENTYPE_SOURCE

#include "../scan/scan.h"

unsigned tokentype (SCAN * content) 

{
	return (content->class);
}

#endif

