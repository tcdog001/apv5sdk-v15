/*====================================================================*
 *
 *   signed lookup (const char * name, const struct _code_ list [], size_t size);
 *
 *   symbol.h
 *   
 *   search a name list and return the associated name; return the
 *   corresponding code on success or -1 on failure; the search is
 *   case insensitive;
 *
 *.  Motley Tools by Charles Maier <cmaier@cmassoc.net>;
 *:  Published 2005 by Charles Maier Associates Limited;
 *;  Released under the GNU General Public Licence v2 and later;
 *
 *--------------------------------------------------------------------*/

#ifndef LOOKUP_SOURCE
#define LOOKUP_SOURCE

#include <unistd.h>
#include <string.h>

#include "../tools/symbol.h"

signed lookup (const char * name, const struct _code_ list [], size_t size) 

{
	const struct _code_ * item = list;
	if ((name) && (*name)) while ((unsigned)(item - list) < size) 
	{
		if (!strcasecmp (item->name, name)) 
		{
			return (item->code);
		}
		item++;
	}
	return (-1);
}

#endif

