/*====================================================================*
 *   
 *   Copyright (c) 2010, Atheros Communications Inc.
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

/*====================================================================*
 *
 *   int6klist.c - list local devices on stdout;
 *
 *   this program sends and receives raw ethernet frames and so needs
 *   root priviledges; if you install it using "chmod 555" and "chown 
 *   root:root" then you must login as root to run it; otherwise, you 
 *   can install it using "chmod 4555" and "chown root:root" so that 
 *   anyone can run it; the program will refuse to run until you get
 *   things right;
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
 *.  Intellon INT6x00 Linux Toolkit for HomePlug AV; 
 *:  Published 2007 by Intellon Corp. ALL RIGHTS RESERVED;
 *;  For demonstration; Not for production use;
 *
 *   Contributor(s):
 *      Charles Maier <charles.maier@intellon.com>
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
#include "../tools/memory.h"
#include "../tools/number.h"
#include "../tools/symbol.h"
#include "../tools/types.h"
#include "../tools/flags.h"
#include "../tools/files.h"
#include "../tools/error.h"
#include "../int6k/int6k.h"

/*====================================================================*
 *   custom source files;
 *--------------------------------------------------------------------*/

#ifndef MAKEFILE
#include "../int6k/Confirm.c"
#include "../int6k/Display.c"
#include "../int6k/Failure.c"
#include "../int6k/Request.c"
#include "../int6k/ReadMME.c"
#include "../int6k/SendMME.c"
#include "../int6k/EthernetHeader.c"
#include "../int6k/IntellonHeader.c"
#include "../int6k/UnwantedPacket.c"
#include "../int6k/devices.c"
#endif

#ifndef MAKEFILE
#include "../tools/alert.c"
#include "../tools/error.c"
#include "../tools/getoptv.c"
#include "../tools/putoptv.c"
#include "../tools/version.c"
#include "../tools/uintspec.c"
#include "../tools/hexdump.c"
#include "../tools/hexencode.c"
#include "../tools/hexdecode.c"
#include "../tools/hexout.c"
#include "../tools/todigit.c"
#include "../tools/synonym.c"
#endif

#ifndef MAKEFILE
#include "../ether/openchannel.c"
#include "../ether/closechannel.c"
#include "../ether/readpacket.c"
#include "../ether/sendpacket.c"
#include "../ether/channel.c"
#endif

#ifndef MAKEFILE
#include "../mme/MMECode.c"
#endif

/*====================================================================*
 *   program constants;
 *--------------------------------------------------------------------*/

#define INT6KLIST_VERBOSE 	(1 << 0)
#define INT6KLIST_SILENCE 	(1 << 1)
#define INT6KLIST_BRIDGE 	(1 << 2)
#define INT6KLIST_REMOTE 	(1 << 3)

/*====================================================================*
 *   
 *   signed LocalDevices (struct channel * channel, const char * space);
 *
 *   print local device hardware addresses on stdout;
 *
 *.  Intellon INT6x00 Linux Toolkit for HomePlug AV; 
 *:  Published 2007 by Intellon Corp. ALL RIGHTS RESERVED;
 *;  For demonstration; Not for production use;
 *
 *--------------------------------------------------------------------*/

static signed LocalDevices (struct channel * channel, const char * space, const char * comma, flag_t flags) 

{
	struct message message;
	memset (&message, 0, sizeof (message));
	EthernetHeader (&message.ethernet, channel->peer, channel->host);
	IntellonHeader (&message.intellon, (VS_SW_VER | MMTYPE_REQ));
	if (sendpacket (channel, &message, ETHER_MIN_LEN) <= 0) 
	{
		return (-1);
	}
	while (readpacket (channel, &message, sizeof (message)) > 0) 
	{
		if (UnwantedPacket (&message, (VS_SW_VER | MMTYPE_CNF))) 
		{
			continue;
		}
		hexout (message.ethernet.OSA, sizeof message.ethernet.OSA, HEX_EXTENDER, stdout);
		if ((space) && (*space)) 
		{
			printf ("%s", space);
		}
	}
	if ((comma) && (*comma)) 
	{
		printf ("%s", comma);
	}
	return (0);
}

/*====================================================================*
 *
 *   signed RemoteDevices (struct channel * channel, const char * space);
 *
 *   print remote devices associated with a specific device;
 *
 *.  Intellon INT6x00 Linux Toolkit for HomePlug AV; 
 *:  Published 2007 by Intellon Corp. ALL RIGHTS RESERVED;
 *;  For demonstration; Not for production use;
 *
 *--------------------------------------------------------------------*/

static signed RemoteDevices (struct channel * channel, const char * space, const char * comma, flag_t flags) 

{
	struct message message;

#ifndef __GNUC__
#pragma pack (push,1)
#endif

	struct __packed station 
	{
		uint8_t LDA [ETHER_ADDR_LEN];
		uint8_t TEI;
		uint8_t BDA [ETHER_ADDR_LEN];
		uint8_t AVGTX;
		uint8_t AVGRX;
	}
	* station;
	struct __packed network 
	{
		uint8_t NID [7];
		uint8_t SNID;
		uint8_t TEI;
		uint8_t ROLE;
		uint8_t CCO_MAC [ETHER_ADDR_LEN];
		uint8_t CCO_TEI;
		uint8_t NUMSTAS;
		struct station stations [1];
	}
	* network;
	struct __packed networks 
	{
		uint8_t NUMAVLNS;
		struct network network [1];
	}
	* networks = (struct networks *) (&message.content);

#ifndef __GNUC__
#pragma pack (pop)
#endif

	memset (&message, 0, sizeof (message));
	EthernetHeader (&message.ethernet, channel->peer, channel->host);
	IntellonHeader (&message.intellon, (VS_NW_INFO | MMTYPE_REQ));
	if (sendpacket (channel, &message, ETHER_MIN_LEN) <= 0) 
	{
		return (-1);
	}
	while (readpacket (channel, &message, sizeof (message)) > 0) 
	{
		if (UnwantedPacket (&message, (VS_NW_INFO | MMTYPE_CNF))) 
		{
			continue;
		}
		if (_anyset (flags, INT6KLIST_BRIDGE)) 
		{
			hexout (message.ethernet.OSA, sizeof message.ethernet.OSA, HEX_EXTENDER, stdout);
			if ((space) && (*space)) 
			{
				printf ("%s", space);
			}
		}
		network = (struct network *)(&networks->network);
		while (networks->NUMAVLNS-- > 0) 
		{
			station = (struct station *)(&network->stations);
			while (network->NUMSTAS-- > 0) 
			{
				if (_anyset (flags, INT6KLIST_REMOTE)) 
				{
					hexout (station->LDA, sizeof (station->LDA), HEX_EXTENDER, stdout);
					if ((space) && (*space)) 
					{
						printf ("%s", space);
					}
				}
				station++;
			}
			network = (struct network *)(station);
		}
		if ((comma) && (*comma)) 
		{
			printf ("%s", comma);
		}
	}
	return (0);
}

/*====================================================================*
 *   
 *   int main (int argc, const char * argv[]);
 *   
 *.  Intellon INT6x00 Linux Toolkit for HomePlug AV; 
 *:  Published 2007 by Intellon Corp. ALL RIGHTS RESERVED;
 *;  For demonstration; Not for production use;
 *
 *--------------------------------------------------------------------*/

int main (int argc, const char * argv []) 

{
	extern struct channel channel;
	extern const struct _term_ devices [];
	static const char *optv [] = 
	{
		"i:bnqrsv",
		"device [device] [...] [> stdout]",
		"Intellon Device Statistics Utility",
		"b\tprint bridge devices; exclude remote devices",

#if defined (WINPCAP) || defined (LIBPCAP)

		"i n\thost interface number [2]",

#else

		"i s\thost interface name [" CHANNEL_ETHDEVICE "]",

#endif

		"n\tappend newline to output",
		"q\tquiet mode",
		"r\tprint remote devices; exclude bridge devices",
		"s\tprint newlines instead of spaces",
		"v\tverbose mode",
		(const char *) (0) 
	};
	flag_t flags = (flag_t)(0);
	const char * space = " ";
	const char * comma = "\0";
	signed c;
	if (getenv (PLCDEVICE)) 
	{
#if defined (WINPCAP) || defined (LIBPCAP)
		channel.id = atoi (getenv (PLCDEVICE));
#else
		channel.name = strdup (getenv (PLCDEVICE));
#endif
	}
	optind = 1;
	opterr = 1;
	optmin = 0;
	while ((c = getoptv (argc, argv, optv)) != -1) 
	{
		switch ((char) (c)) 
		{
		case 'b':
			_setbits (flags, INT6KLIST_BRIDGE);
			break;
		case 'i':

#if defined (WINPCAP) || defined (LIBPCAP)

			channel.id = atoi (optarg);

#else

			channel.name = optarg;

#endif

			break;
		case 'n':
			comma = "\n";
			break;
		case 'r':
			_setbits (flags, INT6KLIST_REMOTE);
			break;
		case 's':
			space = "\n";
			break;
		case 'q':
			_setbits (channel.flags, CHANNEL_SILENCE);
			_setbits (flags, INT6KLIST_SILENCE);
			break;
		case 'v':
			_setbits (channel.flags, CHANNEL_VERBOSE);
			_setbits (flags, INT6KLIST_VERBOSE);
			break;
		default:
			break;
		}
	}
	argc -= optind;
	argv += optind;

#ifndef WIN32

/*
 *      raw packets require root priviledges on linux; one does not have to be
 *      root when this program is installed setuid using 'chown root:root' and 
 *      'chmod 4555';
 */

	if (geteuid ()) 
	{
		error (1, EPERM, INT6K_NEEDROOT);
	}

#endif

	if (_allclr (flags, (INT6KLIST_BRIDGE|INT6KLIST_REMOTE))) 
	{
		_setbits (flags, (INT6KLIST_BRIDGE|INT6KLIST_REMOTE));
	}
	openchannel (&channel);
	if (!argc) 
	{
		LocalDevices (&channel, space, comma, flags);
	}
	while ((argc) && (*argv)) 
	{
		if (!hexencode (channel.peer, sizeof (channel.peer), synonym (*argv, devices, DEVICES))) 
		{
			error (1, errno, INT6K_BAD_MAC, *argv);
		}
		RemoteDevices (&channel, space, comma, flags);
		argc--;
		argv++;
	}
	closechannel (&channel);
	return (0);
}

