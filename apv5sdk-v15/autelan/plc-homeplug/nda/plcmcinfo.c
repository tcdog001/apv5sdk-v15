/*====================================================================*
 *   
 *   Copyright (c) 2011 by Qualcomm Atheros.
 *   
 *   Permission to use, copy, modify, and/or distribute this software 
 *   for any purpose with or without fee is hereby granted, provided 
 *   that the above copyright notice and this permission notice appear 
 *   in all copies.
 *   
 *   THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL 
 *   WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED 
 *   WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL  
 *   THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR 
 *   CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM 
 *   LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, 
 *   NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN 
 *   CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *   
 *--------------------------------------------------------------------*/

#define _GETOPT_H

/*====================================================================*"
 *   system header files;
 *--------------------------------------------------------------------*/

#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>

/*====================================================================*
 *   custom header files;
 *--------------------------------------------------------------------*/

#include "../tools/getoptv.h"
#include "../tools/putoptv.h"
#include "../tools/memory.h"
#include "../tools/number.h"
#include "../tools/symbol.h"
#include "../tools/types.h"
#include "../tools/flags.h"
#include "../tools/files.h"
#include "../tools/error.h"
#include "../ether/channel.h"
#include "../key/HPAVKey.h"
#include "../key/keys.h"
#include "../ram/sdram.h"
#include "../pib/pib.h"
#include "../nvm/nvm.h"
#include "../plc/plc.h"

/*====================================================================*
 *   custom source files;
 *--------------------------------------------------------------------*/

#ifndef MAKEFILE
#include "../plc/Confirm.c"
#include "../plc/Devices.c"
#include "../plc/Display.c"
#include "../plc/Failure.c"
#include "../plc/ReadFMI.c"
#include "../plc/ReadMME.c"
#include "../plc/SendMME.c"
#include "../plc/Request.c"
#include "../plc/chipset.c"
#endif

#ifndef MAKEFILE
#include "../nda/MulticastInfo1.c"
#include "../nda/MulticastInfo2.c"
#endif

#ifndef MAKEFILE
#include "../tools/error.c"
#include "../tools/getoptv.c"
#include "../tools/putoptv.c"
#include "../tools/synonym.c"
#include "../tools/uintspec.c"
#include "../tools/basespec.c"
#include "../tools/version.c"
#include "../tools/hexdump.c"
#include "../tools/hexencode.c"
#include "../tools/hexdecode.c"
#include "../tools/hexstring.c"
#include "../tools/decstring.c"
#include "../tools/decdecode.c"
#include "../tools/hexout.c"
#include "../tools/todigit.c"
#include "../tools/checksum32.c"
#include "../tools/strfbits.c"
#include "../tools/typename.c"
#endif

#ifndef MAKEFILE
#include "../ether/channel.c"
#include "../ether/openchannel.c"
#include "../ether/closechannel.c"
#include "../ether/readpacket.c"
#include "../ether/sendpacket.c"
#endif

#ifndef MAKEFILE
#include "../ram/nvram.c"
#include "../ram/sdramfile.c"
#include "../ram/sdrampeek.c"
#endif

#ifndef MAKEFILE
#include "../mme/EthernetHeader.c"
#include "../mme/QualcommHeader.c"
#include "../mme/FragmentHeader.c"
#include "../mme/UnwantedMessage.c"
#include "../mme/MMECode.c"
#endif

/*====================================================================*
 *   program constants;
 *--------------------------------------------------------------------*/

#define PLCMCINFO_ADDRESS_TYPE 0
#define PLCMCINFO_REQUEST_TYPE 0

/*====================================================================*
 *   program variables;
 *--------------------------------------------------------------------*/

static struct _term_ const addresses [] = 

{
	{
		"IPv4",
		"0x00"
	},
	{
		"IPv6",
		"0x01"
	},
	{
		"MAC",
		"0x03"
	}
};


/*====================================================================*
 *   
 *   int main (int argc, char const * argv[]);
 *   
 *   parse command line, populate plc structure and perform selected 
 *   operations; show help summary if asked; see getoptv and putoptv
 *   to understand command line parsing and help summary display; see
 *   plc.h for the definition of struct plc; 
 *
 *   the command line accepts multiple MAC addresses and the program 
 *   performs the specified operations on each address, in turn; the
 *   address order is significant but the option order is not; the
 *   default address is a local broadcast that causes all devices on
 *   the local H1 interface to respond but not those at the remote
 *   end of the powerline;
 *
 *   the default address is 00:B0:52:00:00:01; omitting the address
 *   will automatically address the local device; some options will
 *   cancel themselves if this makes no sense;
 *
 *   the default interface is eth1 because most people use eth0 as 
 *   their principle network connection; you can specify another 
 *   interface with -i or define environment string PLC to make
 *   that the default interface and save typing;
 *   
 *.  Qualcomm Atheros HomePlug AV Powerline Toolkit
 *:  Published 2009-2011 by Qualcomm Atheros. ALL RIGHTS RESERVED
 *;  For demonstration and evaluation only. Not for production use
 *
 *--------------------------------------------------------------------*/

int main (int argc, char const * argv []) 

{
	extern struct channel channel;
	static char const * optv [] = 
	{
		"a:egi:qt:v",
		"device [device] [...]",
		"Qualcomm Atheros Multicast Information Probe",
		"a n\taddress type is (n) [" OPTSTR (PLCMCINFO_ADDRESS_TYPE) "]",
		"e\tredirect stderr to stdout",

#if defined (WINPCAP) || defined (LIBPCAP)

		"i n\thost interface is (n) [" OPTSTR (CHANNEL_ETHNUMBER) "]",

#else

		"i s\thost interface is (s) [" OPTSTR (CHANNEL_ETHDEVICE) "]",

#endif
		"q\tquiet mode",
		"t n\trequest type is (n) [" OPTSTR (PLCMCINFO_REQUEST_TYPE) "]",
		"v\tverbose mode",
		(char const *) (0)
	};

#include "../plc/plc.c"

		signed c;
	plc.action = PLCMCINFO_ADDRESS_TYPE;
	if (getenv (PLCDEVICE)) 
	{

#if defined (WINPCAP) || defined (LIBPCAP)

		channel.ifindex = atoi (getenv (PLCDEVICE));

#else

		channel.ifname = strdup (getenv (PLCDEVICE));

#endif

	}
	optind = 1;
	while ((c = getoptv (argc, argv, optv)) != -1) 
	{
		switch (c) 
		{
		case 'a':
			plc.sector = (unsigned)(uintspec (synonym (optarg, addresses, SIZEOF (addresses)), 0, UCHAR_MAX));
			error (1, ENOTSUP, "Address Type");
			break;
		case 'e':
			dup2 (STDOUT_FILENO, STDERR_FILENO);
			break;
		case 'i':

#if defined (WINPCAP) || defined (LIBPCAP)

			channel.ifindex = atoi (optarg);

#else

			channel.ifname = optarg;

#endif

			break;
		case 't':
			plc.action = (unsigned)(uintspec (optarg, 0, UCHAR_MAX));
			error (1, ENOTSUP, "Request Type");
			break;
		case 'v':
			_setbits (channel.flags, CHANNEL_VERBOSE);
			_setbits (plc.flags, PLC_VERBOSE);
			break;
		default:
			break;
		}
	}
	argc -= optind;
	argv += optind;
	openchannel (&channel);
	if (!(plc.message = malloc (sizeof (* plc.message)))) 
	{
		error (1, errno, PLC_NOMEMORY);
	}
	if (!argc) 
	{

#if 0

		MulticastInfo1 (&plc);

#else

		MulticastInfo2 (&plc);

#endif

	}
	while ((argc) && (* argv)) 
	{
		if (!hexencode (channel.peer, sizeof (channel.peer), synonym (* argv, devices, SIZEOF (devices)))) 
		{
			error (1, errno, PLC_BAD_MAC, * argv);
		}

#if 0

		MulticastInfo1 (&plc);

#else

		MulticastInfo2 (&plc);

#endif

		argc--;
		argv++;
	}
	free (plc.message);
	closechannel (&channel);
	exit (0);
}

