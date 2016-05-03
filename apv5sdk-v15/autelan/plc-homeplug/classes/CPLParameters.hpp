/*====================================================================*
 *
 *   CPLParameters.hpp - interface for the CPLParameters class
 *
 *   read/write a parameter block image from/to a device/file;
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

#ifndef CPLPARAMETERS_HEADER
#define CPLPARAMETERS_HEADER

/*====================================================================*
 *   system header files;
 *--------------------------------------------------------------------*/

#include <stdint.h>
#include <cstddef>

/*====================================================================*
 *   custom header files;
 *--------------------------------------------------------------------*/

#include "../classes/stdafx.hpp"
#include "../classes/CPLChannel.hpp"
#include "../classes/CPLMessage.hpp"

/*====================================================================*
 *   class declaration;
 *--------------------------------------------------------------------*/

class __declspec (dllexport) CPLParameters:public CPLMessage

{
public:
	CPLParameters ();
	explicit CPLParameters (char const * filename);
	explicit CPLParameters (CPLChannel * channel);
	virtual ~ CPLParameters ();
	void * Data () const;
	size_t Size () const;
	bool IsValid () const;
	signed Flash (CPLChannel * channel);
	signed Write (CPLChannel * channel);
	signed Write (char const * filename);
	signed Read (CPLChannel *);
	signed Read (char const * filename);
	CPLParameters & SetCAPPriority (int CAPpriority);
	int Write_QoS_block (int selectPIB);
	CPLParameters & Lock ();
private:
//    uint8_t * mbuffer;
	CPLParameters & Init ();
	uint16_t mextent;

#ifndef __GNUC__
#pragma pack (push, 1)
#endif

	typedef struct __packed 
	{
		uint8_t FWVERSION;
		uint8_t PIBVERSION;
		uint16_t RESERVED1;
		uint16_t PIBLENGTH;
		uint16_t RESERVED2;
		uint32_t CHECKSUM;
	}
	Header;
	CPLParameters::Header * mheader;

protected:

	uint8_t * mbuffer;

#ifndef __GNUC__
#pragma pack (pop)
#endif

};

/*====================================================================*
 *   end definition;
 *--------------------------------------------------------------------*/

#endif

