/*====================================================================*
 *
 *   unistd.h - substitute unistd.h file for Windows;
 *
 *   this is an important POSIX header that Microsoft ommits; we are
 *   able to obtain many of the same definitions from Microsoft io.h;
 *
 *   Contributor(s):
 *	Charles Maier <charles.maier@intellon.com>
 *	Nathan Houghton <nathan.houghton@intellon.com>
 *	Abdel Younes <younes@leacom.fr>
 *
 *--------------------------------------------------------------------*/

#ifndef UNISTD_HEADER
#define UNISTD_HEADER
 
/*====================================================================*
 *
 *--------------------------------------------------------------------*/

#define STDIN_FILENO 0
#define STDOUT_FILENO 1
#define STDERR_FILENO 2

/*====================================================================*
 *
 *--------------------------------------------------------------------*/

#ifdef WIN32
#include <io.h>
#endif
 
/*====================================================================*
 *
 *--------------------------------------------------------------------*/

#ifdef WIN32
#define sleep(x) Sleep(1000*(x))
#define strcasecmp(a,b) stricmp(a,b)
typedef signed ssize_t;
#endif
 
#if !defined _MSC_VER || _MSC_VER < 1500
#define vsnprintf _vsnprintf
#endif

/*====================================================================*
 *
 *--------------------------------------------------------------------*/

#endif
 

