 /*====================================================================*
 *
 *   CPLChannel.hpp - interface for the CPLChannel class
 *
 *   Ethernet I/O channel managment for powerline applications;
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

#ifndef CPLCHANNEL_HEADER
#define CPLCHANNEL_HEADER

/*====================================================================*
 *  system header files;
 *--------------------------------------------------------------------*/

#if defined (WINPCAP)
#	include <pcap.h>
#	include <Packet32.h>
#	include <ntddndis.h>
#endif

/*====================================================================*
 *   custom header files;
 *--------------------------------------------------------------------*/

#include "../classes/stdafx.hpp"
#include "../classes/oflagword.hpp"
#include "../classes/ointerface.hpp"
#include "../classes/ointellon.hpp"

/*====================================================================*
 *   class constants;
 *--------------------------------------------------------------------*/

#define CPLCHANNEL_FLAG_VERBOSE (1 << 0)
#define CPLCHANNEL_FLAG_SILENCE (1 << 1)

#define CPLCHANNEL_ETHERTYPE 0x88E1 /* in host byte order */
#define CPLCHANNEL_BPFDEVICE "/dev/bpf%d"
#define CPLCHANNEL_TIMEOUT 100

#define CPLCHANNEL_CANTREAD "Read timeout or network error"
#define CPLCHANNEL_CANTSEND "Send timeout or network error"
#define CPLCHANNEL_WONTDOIT "Device Refused Request"
#define CPLCHANNEL_CANTDOIT "(0x%02X) %s"

#define CPLCHANNEL_BRIDGES_MAX 0xFF
#define CPLCHANNEL_DEVICES_MAX 0xFF

/*====================================================================*
 *   class declaration;
 *--------------------------------------------------------------------*/

class __declspec (dllexport) CPLChannel: public oflagword, public oethernet, public ointerface

{
public:
	explicit CPLChannel (unsigned ifindex);
	explicit CPLChannel (char const * ifname);
	virtual ~ CPLChannel ();
	signed Descriptor (void) const;
	signed SendMessage (void const * memory, signed extent);
	signed ReadMessage (void * memory, signed extent);
	signed Bridges (void * memory, size_t extent);
	signed Neighbors (void * memory, size_t extent);
private:
	CPLChannel & init (void);
	CPLChannel & open (void);
	CPLChannel & link (void);
	CPLChannel & dump (void const * memory, size_t extent);
	signed mfd;

#if defined (__APPLE__) || defined (__OpenBSD__)

	unsigned bpf_length;

#elif defined (WINPCAP)

	pcap_t * msocket;
	char merrbuf [PCAP_ERRBUF_SIZE];

#endif
	unsigned mtimeout;

};

/*====================================================================*
 *   end definition;
 *--------------------------------------------------------------------*/

#endif

