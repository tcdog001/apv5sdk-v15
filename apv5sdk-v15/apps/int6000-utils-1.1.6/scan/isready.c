/*====================================================================*
 *
 *   bool isready (SCAN * content);
 *
 *   scan.h
 *
 *   return true unless the input buffer has been scanned;
 *
 *.  released 2005 by charles maier associates ltd. for public use;
 *:  compiled on debian gnu/linux with gcc 2.95 compiler;
 *;  licensed under the gnu public license version two;
 *
 *--------------------------------------------------------------------*/

#ifndef ISREADY_SOURCE
#define ISREADY_SOURCE

#include <string.h>

#include "../scan/scan.h"

bool isready (SCAN * content) 

{
	return (content->first < content->limit);
}

#endif

