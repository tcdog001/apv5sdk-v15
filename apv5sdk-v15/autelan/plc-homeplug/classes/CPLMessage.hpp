/*====================================================================*
 *
 *   CPLMessage.hpp - interface for the CPLMessage class
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

#ifndef CPLMESSAGE_HEADER
#define CPLMESSAGE_HEADER

/*====================================================================*
 *  system header files;
 *--------------------------------------------------------------------*/

#include <stdint.h>
#include <cstdio>

/*====================================================================*
 *   custom header files;
 *--------------------------------------------------------------------*/

#include "../classes/stdafx.hpp"

/*====================================================================*
 *   class constants;
 *--------------------------------------------------------------------*/

#define CPLMESSAGE_BOOTLOADER

/*====================================================================*
 *   class declaration;
 *--------------------------------------------------------------------*/

class __declspec (dllexport) CPLMessage

{
public:
	CPLMessage ();
	virtual ~ CPLMessage ();
	char const * MessageName (uint16_t type) const;
	char const * MessageMode (uint16_t type) const;
	char const * MessageErrorText (uint16_t type, uint8_t code) const;
	CPLMessage & CSV ();
	CPLMessage & HTML ();
private:
	static struct name
	{
		uint16_t type;
		char const * name;
	}
	names [];
	static char const * modes [];
	static struct code
	{
		uint16_t type;
		uint8_t code;
		char const * text;
	}
	mcodes [];
};

/*====================================================================*
 *   end definition;
 *--------------------------------------------------------------------*/

#endif

