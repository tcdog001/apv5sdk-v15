/*====================================================================*
 *
 *   void scanwrite (SCAN * content, signed fd);
 *
 *   scan.h
 * 
 *   write the current token to the device associated with a file
 *   descriptor;
 *
 *.  released 2005 by charles maier associates ltd. for public use;
 *:  compiled on debian gnu/linux with gcc 2.95 compiler;
 *;  licensed under the gnu public license version two;
 *
 *--------------------------------------------------------------------*/

#ifndef SCANWRITE_SOURCE
#define SCANWRITE_SOURCE

#include <unistd.h>

#include "../scan/scan.h"

void scanwrite (SCAN * content, signed fd) 

{
	write (fd, content->first, content->final - content->first);
	return;
}

#endif

