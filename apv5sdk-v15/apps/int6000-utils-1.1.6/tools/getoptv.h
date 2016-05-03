/*====================================================================*
 *
 *   getoptv.h - getopt related definitions and declarations;
 *
 *.  Motley Tools by Charles Maier <cmaier@cmassoc.net>;
 *:  Published 2005 by Charles Maier Associates Limited;
 *;  Released under the GNU General Public Licence v2 and later;
 *
 *--------------------------------------------------------------------*/

#ifndef GETOPTV_HEADER
#define GETOPTV_HEADER

/*====================================================================*
 *   constant definitions;
 *--------------------------------------------------------------------*/

#define GETOPTV_VERSION 1
#define GETOPTV_SUMMARY 1

#define GETOPTV_C_OPTION  ('-')
#define GETOPTV_C_OPERAND (':') 
#define GETOPTV_C_ILLEGAL ('?') 
#define GETOPTV_C_VERSION ('!') 
#define GETOPTV_C_SUMMARY ('?') 

/*====================================================================*
 *   variable declarations;
 *--------------------------------------------------------------------*/

extern char * optarg;
extern signed optopt;
extern signed optind;
extern signed opterr;
extern signed optmin;

/*====================================================================*
 *   function declarations;
 *--------------------------------------------------------------------*/

signed getoptv (int argc, const char * argv [], const char * optv []);

/*====================================================================*
 *   end definitions;
 *--------------------------------------------------------------------*/

#endif

