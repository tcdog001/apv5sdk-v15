/*====================================================================*
 *
 *   oversion.hpp - declarations of the oversion class.
 *
 *   implement a standard means of program version identification; 
 *
 *.  Motley Tools by Charles Maier <cmaier@cmassoc.net>;
 *:  Published 2001-2006 by Charles Maier Associates Limited;
 *;  Released under the GNU General Public Licence v2 and later;
 *
 *--------------------------------------------------------------------*/

#ifndef oVERSION_HEADER
#define oVERSION_HEADER

/*====================================================================*
 *   custom header files; 
 *--------------------------------------------------------------------*/

#include "../classes/stdafx.hpp"

/*====================================================================*
 *   class constants;
 *--------------------------------------------------------------------*/

#define COMPANY "Qualcomm Atheros, Ocala FL USA"
#define PROJECT "Qualcomm Atheros Powerline Toolkit"
#define PACKAGE "plc-utils"
#define VERSION "2.1"
#define RELEASE "1-dev"
#define COMPILE __TIME__ " " __DATE__
#define CONTACT "cmaier@qualcomm.com"
#define LICENSE "Restricted License"

/*====================================================================*
 *   class declaration;
 *--------------------------------------------------------------------*/

class __declspec (dllexport) oversion 

{
public:
	oversion ();
	virtual ~ oversion ();
	oversion & print ();
private:
};

/*====================================================================*
 *   end declaration;
 *--------------------------------------------------------------------*/

#endif

