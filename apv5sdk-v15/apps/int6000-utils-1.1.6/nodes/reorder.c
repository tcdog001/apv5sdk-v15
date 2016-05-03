/*====================================================================*
 *   
 *   void reorder (NODE * node, unsigned index, unsigned level);
 *
 *   nodes.h
 *   
 *   
 *.  Motley Tools by Charles Maier <cmaier@cmassoc.net>;
 *:  Published 2006 by Charles Maier Associates Limited;
 *;  Released under the GNU General Public Licence v2 and later;
 *
 *--------------------------------------------------------------------*/

#ifndef REORDER_SOURCE
#define REORDER_SOURCE

#include "../nodes/nodes.h"

#if NODE_LIST

void reorder (NODE * node) 

{
	static unsigned index = 0;
	static unsigned level = 0;
	while (node) 
	{
		node->index = index++;
		node->level = level;
		node->count = index;
		level++;
		reorder (node->below);
		level--;
		node->count = index - node->count;
		node = node->after;
	}
	return;
}

#endif
#endif

