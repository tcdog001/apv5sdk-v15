/*====================================================================*
 *
 *   void scanquote (const char *charset, SCAN * content);
 *
 *   scan.h
 * 
 *   append input buffer characters to the current token substring until
 *   some member of the specified character set is found; do not append
 *   that character;t marks the start of another scan operation;
 *
 *   if the escape meta character (backslash) occurs then append it and
 *   any subsequent character to the current token substringi and advance
 *   without checking for charactr set membership; this allows character
 *   set members to be ignored as scan terminators;
 *
 *   if the escape meta character (backslash) is a member of the character
 *   set then it termnates the scan, like any other member of the set;
 *
 *   this function is similar to scanuntil() except that scanuntil() does
 *   not recognize escaped character sequences;
 *
 *.  released 2005 by charles maier associates ltd. for public use;
 *:  compiled on debian gnu/linux with gcc 2.95 compiler;
 *;  licensed under the gnu public license version two;
 *
 *--------------------------------------------------------------------*/

#ifndef SCANQUOTE_SOURCE
#define SCANQUOTE_SOURCE

#include "../scan/scan.h"

void scanquote (SCAN * content, const char *charset) 

{
	while ((content->final < content->limit) && !isbreak (content, charset)) 
	{
		if (isbreak (content, "\\")) 
		{
			nextbreak (content);
		}
		nextbreak (content);
	}
	return;
}

#endif

