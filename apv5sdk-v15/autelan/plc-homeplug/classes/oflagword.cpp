/*====================================================================*
 *
 *   oflagword.cpp - oflagword class definition;
 *
 *   bitmapped flagword manager; this class can be inherited by other
 *   classes that need a flagword; 
 *
 *.  Released 2005 by Charles Maier Associates Ltd for private use;
 *:  Licensed under GNU General Public License version 2 and later;
 *;  Designed for CMLinux based on LFS and Debian GNU/Linux;
 *
 *--------------------------------------------------------------------*/

#ifndef oFLAGWORD_SOURCE
#define oFLAGWORD_SOURCE

/*====================================================================*
 *   custom header files;
 *--------------------------------------------------------------------*/

#include "../classes/oflagword.hpp"

/*====================================================================*
 *
 *   unsigned getword () const;
 *
 *   return current flagword;
 *
 *--------------------------------------------------------------------*/

unsigned oflagword::getword () const 

{
	return (this->mbits);
}

/*====================================================================*
 *
 *   unsigned getbits (unsigned bits) const;
 *
 *   return selected flagword bits;
 *
 *--------------------------------------------------------------------*/

unsigned oflagword::getbits (unsigned bits) const 

{
	return (this->mbits & bits);
}

/*====================================================================*
 *
 *   oflagword & setword (unsigned bits);
 *
 *   replace the current flagword;
 *
 *--------------------------------------------------------------------*/

oflagword & oflagword::setword (unsigned bits) 

{
	this->mbits = bits;
	return (*this);
}

/*====================================================================*
 *
 *   oflagword & setbits (unsigned bits);
 *
 *   set the specified flagword bits;
 *
 *--------------------------------------------------------------------*/

oflagword & oflagword::setbits (unsigned bits) 

{
	this->mbits |= bits;
	return (*this);
}

/*====================================================================*
 *
 *   oflagword & clearbits (unsigned bits);
 *
 *   clear the specified flagword bits;
 *
 *--------------------------------------------------------------------*/

oflagword & oflagword::clearbits (unsigned bits) 

{
	this->mbits &= ~bits;
	return (*this);
}

/*====================================================================*
 * 
 *   bool anyset (unsigned bits) const;
 * 
 *--------------------------------------------------------------------*/

bool oflagword::anyset (unsigned bits) const 

{
	return ((this->mbits & bits) != 0);
}

/*====================================================================*
 * 
 *   bool allset (unsigned bits) const;
 * 
 *--------------------------------------------------------------------*/

bool oflagword::allset (unsigned bits) const 

{
	return ((this->mbits & bits) == bits);
}

/*====================================================================*
 * 
 *   bool anyclear (unsigned bits) const;
 * 
 *--------------------------------------------------------------------*/

bool oflagword::anyclear (unsigned bits) const 

{
	return ((this->mbits & bits) != bits);
}

/*====================================================================*
 * 
 *   bool allclear (unsigned bits) const;
 * 
 *--------------------------------------------------------------------*/

bool oflagword::allclear (unsigned bits) const 

{
	return ((this->mbits & bits) == 0);
}

/*====================================================================*
 *
 *   oflagword (unsigned bits);
 *
 *--------------------------------------------------------------------*/

oflagword::oflagword (unsigned bits) 

{
	this->mbits = bits;
	return;
}

/*====================================================================*
 *
 *   oflagword ();
 *
 *--------------------------------------------------------------------*/

oflagword::oflagword () 

{
	this->mbits = 0;
	return;
}

/*====================================================================*
 *
 *   ~oflagword ();
 *
 *--------------------------------------------------------------------*/

oflagword::~oflagword () 

{
	return;
}

/*====================================================================*
 *   end definition;
 *--------------------------------------------------------------------*/

#endif

