/*====================================================================*
 *
 *   void scancntrl (SCAN * content);
 *
 *   scan.h
 *
 *.  released 2005 by charles maier associates ltd. for public use;
 *:  compiled on debian gnu/linux with gcc 2.95 compiler;
 *;  licensed under the gnu public license version two;
 *
 *--------------------------------------------------------------------*/

#ifndef SCANCNTRL_SOURCE
#define SCANCNTRL_SOURCE

#include <ctype.h>

#include "../scan/scan.h"

void scancntrl (SCAN * content) 

{
	while ((content->final < content->limit) && iscntrl (*content->final)) 
	{
		content->final++;
	}
	return;
}

#endif

