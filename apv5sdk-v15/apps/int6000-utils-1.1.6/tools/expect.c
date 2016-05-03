/*====================================================================*
 *
 *   void expect (const struct _code_ list [], size_t size, FILE *fp);
 *
 *   symbol.h
 *   
 *   print a list of expected symbols on stderr;
 *   
 *.  Motley Tools by Charles Maier <cmaier@cmassoc.net>;
 *:  Published 2005 by Charles Maier Associates Limited;
 *;  Released under the GNU General Public Licence v2 and later;
 *
 *--------------------------------------------------------------------*/

#include <stdio.h>

#include "../tools/symbol.h"

void expect (const struct _code_ list [], size_t size, FILE *fp) 

{
	const struct _code_ * item = list;
	if (list) while ((size_t)(item - list) < size) 
	{
		if (item > list) 
		{
			fprintf (fp, "|");
		}
		fprintf (fp, "'%s'", item->name);
		item++;
	}
	return;
}

