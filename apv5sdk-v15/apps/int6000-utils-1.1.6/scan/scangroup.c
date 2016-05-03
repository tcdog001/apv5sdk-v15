/*====================================================================*
 *
 *   void scangroup (SCAN * content, const char *charset);
 *
 *   scan.h
 *
 *.  released 2005 by charles maier associates ltd. for public use;
 *:  compiled on debian gnu/linux with gcc 2.95 compiler;
 *;  licensed under the gnu public license version two;
 *
 *--------------------------------------------------------------------*/

#ifndef SCANGROUP_SOURCE
#define SCANGROUP_SOURCE

#include "../scan/scan.h"

void scangroup (SCAN * content, const char *charset) 

{
	while ((content->final < content->limit) && !isbreak (content, charset)) 
	{
		switch (*content->final++) 
		{
		case '{':
			scangroup (content, "}");
			nextbreak (content);
			break;
		case '(':
			scangroup (content, ")");
			nextbreak (content);
			break;
		case '[':
			scangroup (content, "]");
			nextbreak (content);
			break;
		case '\"':
			scanquote (content, "\"");
			nextbreak (content);
			break;
		case '\'':
			scanquote (content, "\'");
			nextbreak (content);
			break;
		case '\\':
			nextbreak (content);
			break;
		default:
			break;
		}
	}
	return;
}

#endif

