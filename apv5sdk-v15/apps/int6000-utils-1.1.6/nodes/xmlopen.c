/*====================================================================*
 *
 *   NODE * xmlopen (const char * filename);
 *
 *   nodes.h
 *
 *   open an XML file and return the parse tree root;     
 *   
 *   the entire file is read into a buffer associated with the text
 *   member in the root node; the buffer is then split into strings
 *   referenced by child nodes, forming a hierarchial string vector;
 *
 *.  Motley Tools by Charles Maier <cmaier@cmassoc.net>;
 *:  Published 2006 by Charles Maier Associates Limited;
 *;  Released under the GNU General Public Licence v2 and later;
 *
 *--------------------------------------------------------------------*/

#ifndef XMLOPEN_SOURCE
#define XMLOPEN_SOURCE

#include <unistd.h>
#include <memory.h>
#include <fcntl.h>
#include <errno.h>

#include "../nodes/nodes.h"
#include "../tools/memory.h"
#include "../tools/files.h"
#include "../tools/error.h"

NODE * xmlopen (const char * filename) 

{
	NODE * node;
	signed length;
	signed fd;

#if defined (WIN32)

	fd = open (filename, O_RDONLY|O_BINARY);

#else

	fd = open (filename, O_RDONLY);

#endif

	if (fd == -1) 
	{
		error (1, errno, "can't open %s", filename);
	}
	length = lseek (fd, 0, SEEK_END);
	if (length == -1) 
	{
		error (1, errno, "can't access %s", filename);
	}
	node = NEW (NODE);
	memset (node, 0, sizeof (NODE));
	if (lseek (fd, 0, SEEK_SET) == -1) 
	{
		error (1, errno, "can't rewind %s", filename);
	}
	node->text = STR (length);
	if (read (fd, node->text, length) < length) 
	{
		error (1, errno, "can't read %s", filename);
	}
	close (fd);
	node->text [length] = (char)(0);
	xmlscan (node);

#if NODE_LIST

	reorder (node);

#endif

	return (node);
}

#endif

