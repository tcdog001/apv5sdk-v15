/*====================================================================*
 *
 *   CPLNetwork.hpp - interface for the CPLNetwork class  
 *
 *   powerline station enumerator;
 *
 *   the collection of CPLStations on a single powerline network;
 *
 *   This software and documentation is the property of Intellon 
 *   Corporation, Ocala, Florida. It is provided 'as is' without 
 *   expressed or implied warranty of any kind to anyone for any 
 *   reason. Intellon assumes no responsibility or liability for 
 *   errors or omissions in the software or documentation and 
 *   reserves the right to make changes without notification. 
 *   
 *   Intellon customers may modify and distribute the software 
 *   without obligation to Intellon. Since use of this software 
 *   is optional, users shall bear sole responsibility and 
 *   liability for any consequences of it's use. 
 *
 *.  Intellon HomePlug AV Application Programming Package;
 *:  Published 2007-2009 by Intellon Corp. ALL RIGHTS RESERVED;
 *;  For demonstration and evaluation only; Not for production use;
 *
 *   Contributor(s): 
 *	    Charles Maier <charles.maier@intellon.com>
 *
 *--------------------------------------------------------------------*/

#ifndef CPLNETWORK_HEADER
#define CPLNETWORK_HEADER

/*====================================================================*
 *   custom header files;
 *--------------------------------------------------------------------*/

#include "../classes/stdafx.hpp"
#include "../classes/CPLChannel.hpp"
#include "../classes/CPLStation.hpp"

/*====================================================================*
 *   class declaration;
 *--------------------------------------------------------------------*/

class __declspec (dllexport) CPLNetwork 

{
public:
	explicit CPLNetwork (CPLChannel * channel);
	virtual ~ CPLNetwork ();
	bool Empty (void) const;
	bool End (void) const;
	unsigned Count (void) const;
	unsigned Index (void) const;
	CPLNetwork & Select (unsigned);
	CPLNetwork & SelectPrev (void);
	CPLNetwork & SelectNext (void);
	CPLNetwork & SelectFirst (void);
	CPLNetwork & SelectFinal (void);
	CPLNetwork & Enumerate (void);
	CPLNetwork & operator = (unsigned);
	CPLStation & Selected (void) const;
	CPLStation & Station (void) const;
	CPLStation & operator [] (unsigned);
private:
	static void platform (CPLChannel * channel, CPLStation * station);
	CPLStation ** mtable;
	unsigned mcount;
	unsigned mindex;
};

/*====================================================================*
 *   end declaration;
 *--------------------------------------------------------------------*/

#endif

