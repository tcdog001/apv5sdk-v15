/*====================================================================*
 *
 *   const char * xmlcontent (NODE * node);
 *
 *   nodes.h
 *
 *   return node content as a string;
 *
 *.  Motley Tools by Charles Maier <cmaier@cmassoc.net>;
 *:  Published 2006 by Charles Maier Associates Limited;
 *;  Released under the GNU General Public Licence v2 and later;
 *
 *--------------------------------------------------------------------*/

#ifndef XMLCONTENT_SOURCE
#define XMLCONTENT_SOURCE

#include <string.h>

#include "../nodes/nodes.h"

const char * xmlcontent (NODE * node) 

{
	if (node) 
	{
		node = node->below;
	}
	while (node) 
	{
		if (node->type == NODE_DATA) 
		{
			return (node->text);
		}
		node = node->after;
	}
	return ("");
}

#endif

