/*====================================================================*
 *
 *   size_t tokensize (SCAN * content);
 *
 *   scan.h
 *
 *   return the length of the current token substring; the length is
 *   computed by subtracting the start character address from
 *   the break character address;
 *
 *.  released 2005 by charles maier associates ltd. for public use;
 *:  compiled on debian gnu/linux with gcc 2.95 compiler;
 *;  licensed under the gnu public license version two;
 *
 *--------------------------------------------------------------------*/

#ifndef TOKENSIZE_SOURCE
#define TOKENSIZE_SOURCE

#include "../scan/scan.h"

size_t tokensize (SCAN * content) 

{
	return (content->final - content->first);
}

#endif

