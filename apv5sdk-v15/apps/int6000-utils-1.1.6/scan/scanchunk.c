/*====================================================================*
 *
 *   void scanchunk (SCAN * content, const char *literal);
 *
 *   scan the input buffer until a CR/LF character pair is detected;
 *
 *.  Released 2005 by Charles Maier Associates Ltd for private use;
 *:  Licensed under GNU General Public License version 2 and later;
 *;  Designed for CMLinux based on LFS and Debian GNU/Linux;
 *
 *--------------------------------------------------------------------*/

#ifndef SCANCHUNK_SOURCE
#define SCANCHUNK_SOURCE

#include "../scan/scan.h"

void scanchunk (SCAN * content) 

{
	while ((content->final < content->limit) && (*content->final != '\n')) 
	{
		while ((content->final < content->limit) && (*content->final != '\r')) 
		{
			content->final++;
		}
		content->final++;
	}
	if (content->final < content->limit) 
	{
		content->final++;
	}
	return;
}

#endif

