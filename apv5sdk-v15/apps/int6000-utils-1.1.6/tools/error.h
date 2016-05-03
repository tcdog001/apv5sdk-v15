/*====================================================================*
 *
 *   error.h - error function definitions and declarations;
 *
 *   this file is an alterantive to the GNU header file of the same
 *   name; in addition to standard GNU error function declarations,
 *   some additional functions are declared; 
 *
 *.  Motley Tools by Charles Maier <cmaier@cmassoc.net>;
 *:  Published 2005 by Charles Maier Associates Limited;
 *;  Released under the GNU General Public Licence v2 and later;
 *
 *--------------------------------------------------------------------*/

#ifndef ERROR_HEADER
#define ERROR_HEADER

/*====================================================================*
 *   system header files; 
 *--------------------------------------------------------------------*/

#include <errno.h>

/*====================================================================*
 *   custom header files; 
 *--------------------------------------------------------------------*/

#include "../tools/types.h"

/*====================================================================*
 *   
 *--------------------------------------------------------------------*/

#ifndef ECANCELED
#define ECANCELED 125
#endif
#ifndef ENOTSUP
#define ENOTSUP 124
#endif
#ifndef EBADMSG
#define EBADMSG 123
#endif
#ifndef EPROTONOSUPPORT
#define EPROTONOSUPPORT 122
#endif
#ifndef ENODATA
#define ENODATA 0
#endif

/*====================================================================*
 *   declare GNU error() and error_at_line() functions;  
 *--------------------------------------------------------------------*/

#ifdef __GNUC__

__attribute__ ((format (printf, 3, 4))) 

#endif

void error (int status, errno_t number, const char *format, ...);

#ifdef __GNUC__

__attribute__ ((format (printf, 5, 6))) 

#endif

void error_at_line (int status, errno_t number, const char *filename, unsigned lineno, const char *format, ...);

/*====================================================================*
 *   declare GNU error() function alternatives;  
 *--------------------------------------------------------------------*/

#ifdef __GNUC__

__attribute__ ((format (printf, 1, 2))) 

#endif

void alert (const char *format, ...);

#ifdef __GNUC__

__attribute__ ((format (printf, 3, 4))) 

#endif

void event (int status, signo_t number, const char *format, ...);

/*====================================================================*
 *   end definitions and declarations;
 *--------------------------------------------------------------------*/

#endif

