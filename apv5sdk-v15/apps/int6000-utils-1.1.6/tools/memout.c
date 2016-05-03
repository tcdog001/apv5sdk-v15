/*====================================================================*
 *
 *   void memout (const void  * memory, size_t extent, const char * format, unsigned group, char c, FILE *fp) 
 *
 *   memory.h
 *
 *   print memory as a series of octets formatted by format string fmt,
 *   seperated by character c every mod prints;
 *
 *   for example, memout (memory, IPv4_LEN, "%d", 1, '.', stdout) would print:
 *
 *      192.168.1.1
 *
 *   another example, memout (memory, IPv6_LEN, "%02x", 2, ':', stdout) would print:
 *
 *     0032:0045:0000:0000:0000:0000:1123:4456 
 *
 *.  Intellon INT6000 Linux Toolkit for HomePlug AV;
 *:  Published 2006-2009 by Intellon Corp. ALL RIGHTS RESERVED;
 *;  For demonstration; Not for production use;
 *
 *   Contributor(s):
 *	Nathan Houghton <nathan.houghton@intellon.com>
 *
 *--------------------------------------------------------------------*/

#ifndef MEMOUT_SOURCE
#define MEMOUT_SOURCE

#include <stdio.h>

#include "../tools/memory.h"

void memout (const void * memory, size_t extent, const char * format, unsigned group, signed c, FILE *fp) 

{
	byte * offset = (byte *) memory;
	uint64_t count;
	while (extent--) 
	{
		fprintf (fp, format, *offset);
		count = (offset - (byte *)(memory)) + 1;
		if ((count % group) == 0 && extent) 
		{
			putc (c, fp);
		}
		offset++;
	}
	return;
}

#endif

