/*====================================================================*
 *
 *   void scanalpha (SCAN * content);
 *
 *   scan.h
 *
 *.  released 2005 by charles maier associates ltd. for public use;
 *:  compiled on debian gnu/linux with gcc 2.95 compiler;
 *;  licensed under the gnu public license version two;
 *
 *--------------------------------------------------------------------*/

#ifndef SCANALPHA_SOURCE
#define SCANALPHA_SOURCE

#include <ctype.h>

#include "../scan/scan.h"

void scanalpha (SCAN * content) 

{
	while ((content->final < content->limit) && isalpha (*content->final)) 
	{
		content->final++;
	}
	return;
}

#endif

