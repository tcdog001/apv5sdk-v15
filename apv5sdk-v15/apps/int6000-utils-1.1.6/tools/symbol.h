/*====================================================================*
 *
 *   symbol.h - symbol table definitions and declarations;
 *
 *   this file is a subset of the original with some additions not in
 *   the original;
 *
 *.  Motley Tools by Charles Maier <cmaier@cmassoc.net>;
 *:  Published 2005 by Charles Maier Associates Limited;
 *;  Released under the GNU General Public Licence v2 and later;
 *
 *--------------------------------------------------------------------*/

#ifndef SYMBOL_HEADER
#define SYMBOL_HEADER

/*====================================================================*
 *   system header files;
 *--------------------------------------------------------------------*/

#include <stdio.h>

/*====================================================================*
 *   custom header files;
 *--------------------------------------------------------------------*/

#include "../tools/types.h"

/*====================================================================*
 *   functions;             
 *--------------------------------------------------------------------*/

const char * synonym (const char * term, const struct _term_ list [], size_t size);
void assist (const char * name, const char * type, const struct _code_ list [], size_t size, FILE *fp);
signed lookup (const char * name, const struct _code_ list [], size_t size);
const char * reword (code_t code, const struct _code_ list [], size_t size);
void expect (const struct _code_ list [], size_t size, FILE *fp);
const char * typename (const struct _type_ list [], size_t size, type_t type, const char * name);
const char * codename (const struct _code_ list [], size_t size, code_t code, const char * name);
size_t typeload (struct _type_ list [], size_t size);
size_t codeload (struct _code_ list [], size_t size);
void typesave (struct _type_ list [], size_t size, const char * name);
void codesave (struct _code_ list [], size_t size, const char * name);

/*====================================================================*
 *   
 *--------------------------------------------------------------------*/

#endif

