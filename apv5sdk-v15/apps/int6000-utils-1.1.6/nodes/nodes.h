/*====================================================================*
 *
 *   nodes.h - document node definitions and declarations;
 *
 *.  Motley Tools by Charles Maier <cmaier@cmassoc.net>;
 *:  Published 2001-2006 by Charles Maier Associates Limited;
 *;  Released under the GNU General Public Licence v2 and later;
 *
 *--------------------------------------------------------------------*/

#ifndef NODES_HEADER
#define NODES_HEADER

/*====================================================================*
 *   system headers;
 *--------------------------------------------------------------------*/

#include <stdint.h>
#include <unistd.h>

/*====================================================================*
 *   custom headers;
 *--------------------------------------------------------------------*/

#include "../tools/types.h"

/*====================================================================*
 *
 *--------------------------------------------------------------------*/

#define ISO_CHARSET "iso-8859-1"
#define ISO_CONTENT "text/html"

/*====================================================================*
 *
 *--------------------------------------------------------------------*/

#define W3C_STD "-//W3C//DTD XHTML 1.0 Strict//EN"
#define W3C_DTD "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd"

/*====================================================================*
 *   
 *--------------------------------------------------------------------*/

#define XML_VERSION "1.0"
#define XML_NAMESPACE "http://www.w3.org/2001/XMLSchema-instance" 
#define XML_CHARSET ISO_CHARSET
#define XML_LANGUAGE "en-us"
#define XML_SCHEMA "http://www.w3.org/2001/XMLSchema" 

/*====================================================================*
 *   
 *--------------------------------------------------------------------*/

#define NODE_LIST 0
#define NODE_ROOT 0
#define NODE_ELEM '<'
#define NODE_SGML '!' 
#define NODE_INST '?' 
#define NODE_ATTR ' '
#define NODE_VALU '=' 
#define NODE_ETAG '/' 
#define NODE_DATA '>' 

/*====================================================================*
 *   
 *--------------------------------------------------------------------*/

#define DATA_SCHEMA "piboffset.xsd"
#define DATA_NAME "name"
#define DATA_TEXT "text"
#define DATA_OBJECT "pib"
#define DATA_MEMBER "object"
#define DATA_OFFSET "offset"
#define DATA_LENGTH "length"
#define DATA_STRUCT "array"
#define DATA_STRING "dataString"
#define DATA_MEMORY "dataHex"
#define DATA_HUGE "dataHuge"
#define DATA_LONG "dataLong"
#define DATA_WORD "dataWord"
#define DATA_BYTE "dataByte"

/*====================================================================*
 *   variables;
 *--------------------------------------------------------------------*/

typedef struct node 

{
	struct node * above;
	struct node * prior;
	struct node * after;
	struct node * below;

#if NODE_LIST

	unsigned level;
	unsigned index;
	unsigned count;

#endif

	unsigned line;
	unsigned type;
	char * text;
}

NODE;

/*====================================================================*
 *   functions;
 *--------------------------------------------------------------------*/

#if NODE_LIST

void reorder (NODE * node);
NODE * nodes (NODE * node, unsigned index);

#endif

const char * xmlattribute (NODE * node, const char * name);
const char * xmlvalue (NODE * node);
const char * xmlcontent (NODE * node);
void xmlschema ();

/*====================================================================*
 *   
 *--------------------------------------------------------------------*/

NODE * xmlnode (NODE * node);
NODE * xmlopen (const char * filename);
signed xmlscan (NODE * node);
signed xmledit (NODE * node, void * memory, size_t extent);
void xmltree (NODE * node);
void xmlfree (NODE * node);

/*====================================================================*
 *
 *--------------------------------------------------------------------*/

#endif

