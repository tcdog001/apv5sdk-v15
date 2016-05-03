/*====================================================================*
 *
 *   const char * xmlattribute (NODE * node, const char * name);
 *
 *   nodes.h
 *   
 *   search node for the named attribute; return the attribute value
 *   as a string;
 *   
 *.  Motley Tools by Charles Maier <cmaier@cmassoc.net>;
 *:  Published 2006 by Charles Maier Associates Limited;
 *;  Released under the GNU General Public Licence v2 and later;
 *   
 *--------------------------------------------------------------------*/

#ifndef XMLATTRIBUTE_SOURCE
#define XMLATTRIBUTE_SOURCE

#include <string.h>

#include "../nodes/nodes.h"

const char * xmlattribute (NODE * node, const char * name) 

{
	if (node) 
	{
		node = node->below;
	}
	while (node) 
	{
		if (node->type == NODE_ATTR) 
		{
			if (!strcmp (node->text, name)) 
			{
				name = xmlvalue (node);
				return (name);
			}
		}
		node=node->after;
	}
	return ("");
}

#endif

