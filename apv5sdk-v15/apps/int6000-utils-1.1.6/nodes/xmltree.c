/*====================================================================*
 *
 *   void xmltree (NODE * node);
 *
 *   nodes.h
 *
 *   print node structure on stdout;
 *
 *.  Motley Tools by Charles Maier <cmaier@cmassoc.net>;
 *:  Published 2006 by Charles Maier Associates Limited;
 *;  Released under the GNU General Public Licence v2 and later;
 *
 *--------------------------------------------------------------------*/

#ifndef XMLTREE_SOURCE
#define XMLTREE_SOURCE

#include <stdio.h>

#include "../nodes/nodes.h"
#include "../tools/format.h"

void xmltree (NODE * node) 

{
	static signed level = 0;
	if (node) 
	{
		node = node->below;
	}
	while (node) 
	{

#if NODE_LIST

		printf ("%03d ", node->index);
		printf ("%02d ", node->level);
		printf ("%03d ", node->count);

#endif

		printf ("%03d ", node->line);
		printf ("[%c] ", node->type);
		output (level, "[%s]", node->text);
		level++;
		xmltree (node);
		level--;
		node = node->after;
	}
	return;
}

#endif

