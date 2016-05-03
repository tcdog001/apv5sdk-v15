/*====================================================================*
 *
 *   oerror.hpp - interface for the oerror class.
 *
 *   this class implements the useful GNU error() and error_at_line()
 *   functions with some additions;
 *
 *.  published 2003 by charles maier associates limited for internal use;
 *:  modified sun 07 sep 03 by user root on host zeus;
 *;  compiled for debian gnu/linux using gcc compiler;
 *
 *--------------------------------------------------------------------*/

#ifndef oERROR_HEADER
#define oERROR_HEADER

/*====================================================================*
 *   custom header files;
 *--------------------------------------------------------------------*/

#include <cstdio>
#include <cerrno>

/*====================================================================*
 *   custom header files;
 *--------------------------------------------------------------------*/

#include "../classes/stdafx.hpp"
#include "../tools/types.h"

/*====================================================================*
 *   class constants;
 *--------------------------------------------------------------------*/

#ifndef ECANCELED
#define ECANCELED 0
#endif

#ifndef EOVERFLOW
#define EOVERFLOW 0
#endif

#ifndef ENOTSUP
#define ENOTSUP 0
#endif

/*====================================================================*
 *   class constants;
 *--------------------------------------------------------------------*/

#define oERROR_NEEDROOT "Program requires root privileges"
#define oERROR_UNWANTED "Superfluous command line arguments"

/*====================================================================*
 *   class declarations;
 *--------------------------------------------------------------------*/

class oerror

{
public:
	oerror ();
	virtual ~ oerror ();
	static void print (char const *format, ...);
	static void error (char const *format, ...);
	static void syntax (char const * file, unsigned line, char const *format, ...);
	static void error (signed status, errno_t number, char const *format, ...);
	static void error_at_line (signed status, errno_t number, char const * file, unsigned line, char const * format, ...);
};

/*====================================================================*
 *
 *--------------------------------------------------------------------*/

#endif

