/*====================================================================*
 *
 *   CPLNetworks.hpp - interface for the CPLNetworks class  
 *
 *   powerline network enumerator;
 *
 *   the collection of CPLNetworks on a single host interface;
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
 *.  Intellon INT6000 Linux Toolkit for HomePlug AV;
 *:  Published 2006-2009 by Intellon Corp. ALL RIGHTS RESERVED;
 *;  For demonstration; Not for production use;
 *
 *   Contributor(s): 
 *	    Charles Maier <charles.maier@intellon.com>
 *
 *--------------------------------------------------------------------*/

#ifndef CPLNETWORKS_HEADER
#define CPLNETWORKS_HEADER

/*====================================================================*
 *   custom header files;
 *--------------------------------------------------------------------*/

#include "../classes/stdafx.hpp"
#include "../classes/CPLChannel.hpp"
#include "../classes/CPLNetwork.hpp"

/*====================================================================*
 *   class declaration;
 *--------------------------------------------------------------------*/

class __declspec (dllexport) CPLNetworks 

{
public:
	explicit CPLNetworks (char const * ifname);
	virtual ~ CPLNetworks ();
	bool Empty (void) const;
	bool End (void) const;
	unsigned Count (void) const;
	unsigned Index (void) const;
	CPLNetworks & Select (unsigned);
	CPLNetworks & SelectPrev (void);
	CPLNetworks & SelectNext (void);
	CPLNetworks & SelectFirst (void);
	CPLNetworks & SelectFinal (void);
	CPLNetworks & Enumerate (void);
	CPLNetworks & operator = (unsigned);
	CPLNetwork & Selected (void) const;
	CPLNetwork & Network (void) const;
	CPLNetwork & operator [] (unsigned);
private:
	CPLNetwork ** mtable;
	unsigned mcount;
	unsigned mindex;
};

/*====================================================================*
 *   end declaration;
 *--------------------------------------------------------------------*/

#endif

