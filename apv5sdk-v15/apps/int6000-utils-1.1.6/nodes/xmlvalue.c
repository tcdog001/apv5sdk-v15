/*====================================================================*
 *
 *   const char * xmlvalue (NODE * node);
 *
 *   nodes.h
 *
 *   return node value as a string;
 *
 *.  Motley Tools by Charles Maier <cmaier@cmassoc.net>;
 *:  Published 2006 by Charles Maier Associates Limited;
 *;  Released under the GNU General Public Licence v2 and later;
 *
 *--------------------------------------------------------------------*/

#ifndef XMLVALUE_SOURCE
#define XMLVALUE_SOURCE

#include <string.h>

#include "../nodes/nodes.h"

const char * xmlvalue (NODE * node) 

{
	if (node) 
	{
		node = node->below;
	}
	while (node) 
	{
		if (node->type == NODE_VALU) 
		{
			return (node->text);
		}
		node = node->after;
	}
	return ("");
}

#endif

