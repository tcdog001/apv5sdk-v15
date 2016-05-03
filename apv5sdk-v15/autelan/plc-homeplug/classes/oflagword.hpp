/*====================================================================*
 *
 *   oflagword.hpp - interface for the oflagword class;
 *
 *   bitmapped flagword manager; this class can be inherited by other
 *   classes that need a flagword; 
 *
 *.  Released 2005 by Charles Maier Associates Ltd for private use;
 *:  Licensed under GNU General Public License version 2 and later;
 *;  Designed for CMLinux based on LFS and Debian GNU/Linux;
 *
 *--------------------------------------------------------------------*/

#ifndef oFLAGWORD_HEADER
#define oFLAGWORD_HEADER

/*====================================================================*
 *   custom header files;
 *--------------------------------------------------------------------*/

#include "../classes/stdafx.hpp"

/*====================================================================*
 *   class declaration;
 *--------------------------------------------------------------------*/

class __declspec (dllexport) oflagword 

{
public:
	oflagword ();
	oflagword (unsigned);
	virtual ~ oflagword ();
	unsigned getword () const;
	unsigned getbits (unsigned bits) const;
	oflagword & setword (unsigned bits);
	oflagword & setbits (unsigned bits);
	oflagword & clearbits (unsigned bits);
	bool anyset (unsigned bits) const;
	bool allset (unsigned bits) const;
	bool anyclear (unsigned bits) const;
	bool allclear (unsigned bits) const;
private:
	unsigned mbits;
};

/*====================================================================*
 *   end declaration;
 *--------------------------------------------------------------------*/

#endif

