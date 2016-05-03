/*====================================================================*
 *
 *   void scanblock (SCAN * content, const char *literal);
 *
 *   collect input characters until an inverted pair is detected; the 
 *   pair is specified in reverse order;
 *
 *   this method collects text between inverted delimiter pairs like:
 *   
 *   "{* ... *}" or "-- ... --"
 *
 *.  Released 2005 by Charles Maier Associates Ltd for private use;
 *:  Licensed under GNU General Public License version 2 and later;
 *;  Designed for CMLinux based on LFS and Debian GNU/Linux;
 *
 *--------------------------------------------------------------------*/

#ifndef SCANBLOCK_SOURCE
#define SCANBLOCK_SOURCE

#include "../scan/scan.h"

void scanblock (SCAN * content, const char *literal) 

{
	while ((content->final < content->limit) && (*content->final != *literal)) 
	{
		literal++;
		scanuntil (content, literal);
		scanbreak (content, literal);
		literal--;
	}
	nextbreak (content);
	return;
}

#endif

