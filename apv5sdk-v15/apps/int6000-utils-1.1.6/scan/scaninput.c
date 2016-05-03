/*====================================================================*
 *
 *   void scaninput (SCAN * content, const char buffer [], signed length);
 *
 *   scan.h
 *
 *.  released 2005 by charles maier associates ltd. for public use;
 *:  compiled on debian gnu/linux with gcc 2.95 compiler;
 *;  licensed under the gnu public license version two;
 *
 *--------------------------------------------------------------------*/

#ifndef SCANINPUT_SOURCE
#define SCANINPUT_SOURCE

#include <stdlib.h>

#include "../scan/scan.h"

void scaninput (SCAN * content, const char buffer [], signed length) 

{
	content->start = buffer;
	content->first = buffer;
	content->final = buffer;
	content->index = buffer;
	content->limit = buffer + length;
	content->class = TOKEN_EMPTY;
	content->block = TOKEN_WIDTH;
	content->width = TOKEN_WIDTH;
	content->token = malloc (content->width);
	return;
}

#endif

