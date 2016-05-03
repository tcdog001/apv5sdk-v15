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
 *   int6keth.c - Intellon Ethernet PHY Settings;
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
 *. Intellon INT6x00 Linux Toolkit for HomePlug AV; 
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
#include "../ether/channel.h"
#include "../mme/mme.h"

/*====================================================================*
 *   custom source files;
 *--------------------------------------------------------------------*/

#ifndef MAKEFILE
#include "../int6k/EthernetHeader.c"
#include "../int6k/IntellonHeader.c"
#include "../int6k/UnwantedPacket.c"
#include "../int6k/devices.c"
#endif

#ifndef MAKEFILE
#include "../tools/alert.c"
#include "../tools/assist.c"
#include "../tools/error.c"
#include "../tools/expect.c"
#include "../tools/getoptv.c"
#include "../tools/putoptv.c"
#include "../tools/version.c"
#include "../tools/uintspec.c"
#include "../tools/hexdump.c"
#include "../tools/hexencode.c"
#include "../tools/hexdecode.c"
#include "../tools/hexstring.c"
#include "../tools/todigit.c"
#include "../tools/checkfilename.c"
#include "../tools/checksum32.c"
#include "../tools/fdchecksum32.c"
#include "../tools/strfbits.c"
#include "../tools/filepart.c"
#include "../tools/typename.c"
#include "../tools/lookup.c"
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

#define NEGOTIATE 2
#define SPEEDS 3
#define DUPLEX 2
#define CONTROL 4
#define ADVCAP 4

static const struct _code_ negotiate [NEGOTIATE] = 

{
	{
		0,
		"Off" 
	},
	{
		1,
		"On" 
	}
};

static const struct _code_ advcap [ADVCAP] = 

{
	{
		1,
		"100Full" 
	},
	{
		2,
		"100Half" 
	},
	{
		4,
		"10Full" 
	},
	{
		8,
		"10Half" 
	}
};

static const struct _code_ speeds [SPEEDS] = 

{
	{
		0,
		"10" 
	},
	{
		1,
		"100" 
	},
	{
		2,
		"1000" 
	}
};

static const struct _code_ duplex [DUPLEX] = 

{
	{
		0,
		"Half" 
	},
	{
		1,
		"Full" 
	}
};

static const struct _code_ control [CONTROL] = 

{
	{
		0,
		"Off" 
	},
	{
		1,
		"Tx" 
	},
	{
		2,
		"Rx" 
	},
	{
		3,
		"On" 
	}
};

/*====================================================================*
 *   program constants;
 *--------------------------------------------------------------------*/

#define RATES 3
#define MODES 2
#define LINKS 3
#define FLOWS 4

static const char * rates [RATES] = 

{
	"10",
	"100",
	"1000" 
};

static const char * modes [MODES] = 

{
	"Half",
	"Full" 
};

static const char * links [LINKS] = 

{
	"Unknown",
	"Off",
	"On" 
};

static const char * flows [FLOWS] = 

{
	"Off",
	"Tx",
	"Rx",
	"On" 
};

/*====================================================================*
 *   program variables;
 *--------------------------------------------------------------------*/

#ifndef __GNUC__
#pragma pack (push,1)
#endif

typedef struct __packed phy_settings 

{
	uint8_t MCONTROL;
	uint8_t AUTONEGOTIATE;
	uint8_t ADVCAPS;
	uint8_t ESPEED;
	uint8_t EDUPLEX;
	uint8_t EFLOWCONTROL;
}

phy_settings;
typedef struct __packed phy_readings 

{
	uint8_t MSTATUS;
	uint8_t ESPEED;
	uint8_t EDUPLEX;
	uint8_t ELINKSTATUS;
	uint8_t EFLOWCONTROL;
}

phy_readings;

#ifndef __GNUC__
#pragma pack (pop)
#endif

/*====================================================================*
 *
 *   signed PHYSettings (struct channel * channel, struct phy_settings * settings, flag_t flags);
 *
 *   int6k.h
 *
 *   read and display Ethernet PHY settings or write then read and
 *   display settings;
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
 *. Intellon INT6x00 Linux Toolkit for HomePlug AV; 
 *:  Published 2007 by Intellon Corp. ALL RIGHTS RESERVED;
 *;  For demonstration; Not for production use;
 *
 *   Contributor(s):
 *      Charles Maier <charles.maier@intellon.com>
 *
 *--------------------------------------------------------------------*/

signed PHYSettings (struct channel * channel, struct phy_settings * settings, flag_t flags) 

{
	struct message message;
	signed packetsize;

#ifndef __GNUC__
#pragma pack (push,1)
#endif

	struct __packed vs_enet_settings_request 
	{
		struct header_eth ethernet;
		struct header_int intellon;
		uint8_t MCONTROL;
		uint8_t AUTONEGOTIATE;
		uint8_t ADVCAPS;
		uint8_t ESPEED;
		uint8_t EDUPLEX;
		uint8_t EFLOWCONTROL;
	}
	* request = (struct vs_enet_settings_request *) (&message);
	struct __packed vs_enet_settings_confirm 
	{
		struct header_eth ethernet;
		struct header_int intellon;
		uint8_t MSTATUS;
		uint8_t ESPEED;
		uint8_t EDUPLEX;
		uint8_t ELINKSTATUS;
		uint8_t EFLOWCONTROL;
	}
	* confirm = (struct vs_enet_settings_confirm *) (&message);

#ifndef __GNUC__
#pragma pack (pop)
#endif

	char address [ETHER_ADDR_LEN * 3];
	memset (&message, 0, sizeof (message));
	EthernetHeader (&message.ethernet, channel->peer, channel->host);
	IntellonHeader (&message.intellon, (VS_ENET_SETTINGS | MMTYPE_REQ));
	request->MCONTROL = settings->MCONTROL;
	request->AUTONEGOTIATE = settings->AUTONEGOTIATE;
	request->ADVCAPS = settings->ADVCAPS;
	request->ESPEED = settings->ESPEED;
	request->EDUPLEX = settings->EDUPLEX;
	request->EFLOWCONTROL = settings->EFLOWCONTROL;
	if (sendpacket (channel, &message, ETHER_MIN_LEN) < 0) 
	{
		error (1, ECANCELED, INT6K_CANTSEND);
	}
	while ((packetsize = readpacket (channel, &message, sizeof (message))) > 0) 
	{
		if (UnwantedPacket (&message, (VS_ENET_SETTINGS | MMTYPE_CNF))) 
		{
			continue;
		}
		if ((confirm->MSTATUS == 1) || (confirm->MSTATUS == 3)) 
		{
			error (0, 0, "%s (%0X): ", MMECode (confirm->intellon.MMTYPE, confirm->MSTATUS), confirm->MSTATUS);
			continue;
		}
		if (_anyset (flags, INT6K_ANALYSE)) 
		{
			printf ("Bits Mode Link Flow\n");
			printf ("%4d ", confirm->ESPEED);
			printf ("%4d ", confirm->EDUPLEX);
			printf ("%4d ", confirm->ELINKSTATUS);
			printf ("%4d\n", confirm->EFLOWCONTROL);
		}
		else 
		{
			printf ("%s %s ", channel->name, hexstring (address, sizeof (address), channel->host, sizeof (channel->host)));
			printf ("Speed=%s ", rates [confirm->ESPEED]);
			printf ("Duplex=%s ", modes [confirm->EDUPLEX]);
			printf ("LinkStatus=%s ", links [confirm->ELINKSTATUS]);
			printf ("FlowControl=%s\n", flows [confirm->EFLOWCONTROL]);
		}
	}
	if (packetsize < 0) 
	{
		error (1, ECANCELED, INT6K_CANTREAD);
	}
	return (0);
}

/*====================================================================*
 *   
 *   int main (int argc, const char * argv[]);
 *   
 *   parse command line, populate int6k structure and perform selected 
 *   operations; show help summary if asked; see getoptv and putoptv
 *   to understand command line parsing and help summary display; see
 *   int6k.h for the definition of struct int6k; 
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
 *.  Intellon INT6x00 Linux Toolkit for HomePlug AV; 
 *:  Published 2007 by Intellon Corp. ALL RIGHTS RESERVED;
 *;  For demonstration; Not for production use;
 *
 *--------------------------------------------------------------------*/

int main (int argc, const char * argv []) 

{
	extern struct channel channel;
	static const char *optv [] = 
	{
		"a:d:ef:i:n:qrs:tvw",
		"device [device] [...] [> stdout]",
		"Intellon Ethernet PHY Settings",
		"a s\tadvertise capabilities as s ['100Full'|'100Half'|10Full'|'10Half']",

#if defined (WINPCAP) || defined (LIBPCAP)

		"i n\thost interface number [2]",

#else

		"i s\thost interface name [" CHANNEL_ETHDEVICE "]",

#endif

		"f s\tflow control is s ['on'|'tx'|'rx'|'off']",
		"d s\tduplex setting is s ['half'|'full']",
		"n s\tauto-negotiate mode is s ['on'|'off']",
		"q\tquiet mode",
		"r\tread settings from device",
		"s s\ttransmission speed in mbps is s ['10'|'100'|'1000']",
		"v\tverbose mode",
		"w\twrite settings to device",
		(const char *) (0) 
	};
	struct phy_settings settings = 
	{
		0,
		1,
		0,
		0,
		0,
		0 
	};
	flag_t flags = (flag_t)(0);
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
	while ((c = getoptv (argc, argv, optv)) != -1) 
	{
		switch ((char) (c)) 
		{
		case 'a':
			if ((c = lookup (optarg, advcap, ADVCAP)) == -1) 
			{
				assist (optarg, "capability", advcap, ADVCAP, stderr);
			}
			settings.ADVCAPS |= (uint8_t)(c);
			break;
		case 'd':
			if ((c = lookup (optarg, duplex, DUPLEX)) == -1) 
			{
				assist (optarg, "duplex", duplex, DUPLEX, stderr);
			}
			settings.EDUPLEX = (uint8_t)(c);
			break;
		case 'e':
			dup2 (STDOUT_FILENO, STDERR_FILENO);
			break;
		case 'f':
			if ((c = lookup (optarg, control, CONTROL)) == -1) 
			{
				assist (optarg, "control", control, CONTROL, stderr);
			}
			settings.EFLOWCONTROL = (uint8_t)(c);
			break;
		case 'n':
			if ((c = lookup (optarg, negotiate, NEGOTIATE)) == -1) 
			{
				assist (optarg, "auto-negotiate", negotiate, NEGOTIATE, stderr);
			}
			settings.AUTONEGOTIATE = (uint8_t)(c);
			break;
		case 's':
			if ((c = lookup (optarg, speeds, SPEEDS)) == -1) 
			{
				assist (optarg, "speed", speeds, SPEEDS, stderr);
			}
			settings.ESPEED = (uint8_t)(c);
			break;
		case 't':
			_setbits (flags, INT6K_ANALYSE);
			break;
		case 'i':

#if defined (WINPCAP) || defined (LIBPCAP)

			channel.id = atoi (optarg);

#else

			channel.name = optarg;

#endif

			break;
		case 'q':
			_setbits (channel.flags, CHANNEL_SILENCE);
			break;
		case 'r':
			settings.MCONTROL = 0;
			break;
		case 'v':
			_setbits (channel.flags, CHANNEL_VERBOSE);
			break;
		case 'w':
			settings.MCONTROL = 1;
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

	openchannel (&channel);
	if (!argc) 
	{
		PHYSettings (&channel, &settings, flags);
	}
	while ((argc) && (*argv)) 
	{
		if (!hexencode (channel.peer, sizeof (channel.peer), synonym (*argv, devices, DEVICES))) 
		{
			error (1, errno, INT6K_BAD_MAC, *argv);
		}
		PHYSettings (&channel, &settings, flags);
		argc--;
		argv++;
	}
	closechannel (&channel);
	exit (0);
}

