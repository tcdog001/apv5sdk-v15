/*====================================================================*
 *
 *   NODE * xmlfree (NODE * node);
 *
 *   nodes.h
 *
 *   recursively free document nodes; minimize recursion depth by 
 *   following node->below only and not following node->after;
 *
 *.  Motley Tools by Charles Maier <cmaier@cmassoc.net>;
 *:  Published 2006 by Charles Maier Associates Limited;
 *;  Released under the GNU General Public Licence v2 and later;
 *
 *--------------------------------------------------------------------*/

#ifndef XMLFREE_SOURCE
#define XMLFREE_SOURCE

#include <stdlib.h>
#include <memory.h>

#include "../nodes/nodes.h"

void xmlfree (NODE * node) 

{
	NODE * temp;
	while (node) 
	{
		if (node->above) 
		{
			node->above->below = (NODE *)(0);
		}
		else 
		{
			free (node->text);
		}
		if (node->prior) 
		{
			node->prior->after = (NODE *)(0);
		}
		xmlfree (node->below);
		temp = node;
		node = node->after;
		memset (temp, 0, sizeof (NODE));
		free (temp);
	}
	return;
}

#endif

