/*====================================================================*
 *
 *   void scanlimit (SCAN * content);
 *
 *   scan.h
 * 
 *   expand the token to include the balance of the input buffer 
 *   by setting the charcter pointer to the limit pointer;
 *
 *.  released 2005 by charles maier associates ltd. for public use;
 *:  compiled on debian gnu/linux with gcc 2.95 compiler;
 *;  licensed under the gnu public license version two;
 *
 *--------------------------------------------------------------------*/

#ifndef SCANLIMIT_SOURCE
#define SCANLIMIT_SOURCE

#include "../scan/scan.h"

void scanlimit (SCAN * content) 

{
	content->final = content->limit;
	return;
}

#endif

