/*====================================================================*
 *
 *   types.h - custom data type definitions and declarations; 
 *
 *   this file is a subset of the original that includes only those
 *   definitions and declaration needed for toolkit programs;
 *
 *.  Motley Tools by Charles Maier <cmaier@cmassoc.net>;
 *:  Published 2005 by Charles Maier Associates Limited;
 *;  Released under the GNU General Public Licence v2 and later;
 *
 *--------------------------------------------------------------------*/

#ifndef TYPES_HEADER
#define TYPES_HEADER

/*====================================================================*
 *   system header files; 
 *--------------------------------------------------------------------*/

#include <stdint.h>

/*====================================================================*
 *   new POSIX types; 
 *--------------------------------------------------------------------*/

typedef signed errno_t;
typedef signed signo_t;
typedef unsigned char byte;

/*====================================================================*
 *   define C++ style true and false for use in standard C programs;
 *--------------------------------------------------------------------*/

#ifndef __cplusplus

typedef enum 

{
	false,
	true 
}

bool;

#endif

/*====================================================================*
 *   cope with structure packing vagaries;
 *--------------------------------------------------------------------*/

#ifndef __packed
#ifdef __GNUC__
#define __packed __attribute__ ((packed))
#else
#define __packed
#endif
#endif

/*====================================================================*
 *   define flagword (bitmap) variable type for clarity;
 *--------------------------------------------------------------------*/

typedef signed file_t;
typedef signed sock_t;
typedef signed code_t;
typedef unsigned char byte_t;
typedef unsigned type_t;
typedef unsigned flag_t;
typedef struct _file_ 

{
	file_t file;
	const char *name;
}

file;
typedef struct _term_ 

{
	const char * term;
	const char * text;
}

TERM;
typedef struct _type_ 

{
	type_t type;
	const char *name;
}

TYPE;
typedef struct _code_ 

{
	code_t code;
	const char *name;
}

CODE;

/*====================================================================*
 *   end definitions and declarations;
 *--------------------------------------------------------------------*/

#endif

