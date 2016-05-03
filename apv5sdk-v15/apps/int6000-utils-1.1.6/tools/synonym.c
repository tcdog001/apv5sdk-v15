/*====================================================================*
 *
 *   const char * synonym (const char * term, const struct _term_ list [], size_t size);
 *
 *   symbol.h  
 *
 *   lookup term and return corresponding text; return the original
 *   term if lookup fails; the list must be in lexographic order by
 *   term or lookups may fail; struct _term_ is defined in types.h;
 * 
 *.  Motley Tools by Charlie Maier; cmaier@cmassoc.net;
 *:  Released 2007 by Intellon Corporation, Ocala FL USA;
 *;  Licensed under the GNU General Public License verion 2 or later;
 *
 *--------------------------------------------------------------------*/

#ifndef SYNONYM_SOURCE
#define SYNONYM_SOURCE

#include <string.h>

#include "../tools/types.h"

const char * synonym (const char * term, const struct _term_ list [], size_t size) 

{
	size_t lower = 0;
	size_t upper = size;
	while (lower < upper) 
	{
		size_t index = (lower + upper) >> 1;
		signed order = strcmp (term, list [index].term);
		if (order < 0) 
		{
			upper = index - 0;
			continue;
		}
		if (order > 0) 
		{
			lower = index + 1;
			continue;
		}
		return (list [index].text);
	}
	return (term);
}

#endif

