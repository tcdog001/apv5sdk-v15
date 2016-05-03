/*====================================================================*
 *
 *   void scanprint (SCAN * content, signed fd);
 *
 *   scan.h
 * 
 *   write the current token to the device associated with a file
 *   descriptor; this function is identical to writetoken but adds
 *   a newline character after each write;
 *
 *.  released 2005 by charles maier associates ltd. for public use;
 *:  compiled on debian gnu/linux with gcc 2.95 compiler;
 *;  licensed under the gnu public license version two;
 *
 *--------------------------------------------------------------------*/

#ifndef SCANPRINT_SOURCE
#define SCANPRINT_SOURCE

#include <unistd.h>

#include "../scan/scan.h"

void scanprint (SCAN * content, signed fd) 

{
	write (fd, content->first, content->final - content->first);
	write (fd, "\n", 1);
	return;
}

#endif

