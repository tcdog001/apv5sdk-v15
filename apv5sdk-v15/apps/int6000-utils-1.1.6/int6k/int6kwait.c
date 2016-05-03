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
 *   int6kwait.c - Intellon INT6000 Procrastinator;
 *
 *   wait for device events to start or finish before returning;
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
 *      Abdel Younes <younes@leacom.fr>
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
#include "../tools/getoptv.c"
#include "../tools/putoptv.c"
#include "../tools/version.c"
#include "../tools/uintspec.c"
#include "../tools/hexdump.c"
#include "../tools/hexencode.c"
#include "../tools/hexdecode.c"
#include "../tools/todigit.c"
#include "../tools/checkfilename.c"
#include "../tools/synonym.c"
#endif

#ifndef MAKEFILE
#include "../int6k/Confirm.c"
#include "../int6k/Display.c"
#include "../int6k/Failure.c"
#include "../int6k/ReadMME.c"
#include "../int6k/Request.c"
#include "../int6k/SendMME.c"
#include "../int6k/devices.c"
#include "../int6k/EthernetHeader.c"
#include "../int6k/IntellonHeader.c"
#include "../int6k/UnwantedPacket.c"
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
 *
 *   signed ResetAndWait (struct int6k * int6k);
 * 
 *   int6k.h
 *
 *   Reset the device using a VS_RS_DEV Request message; continue to
 *   request resets each second until the device accepts the request
 *   or the wait period expires;
 *
 *   See the Intellon HomePlug AV Firmware Technical Reference Manual
 *   for more information;
 *   
 *--------------------------------------------------------------------*/

signed ResetAndWait (struct int6k * int6k) 

{
	struct channel * channel = (struct channel *)(int6k->channel);
	struct message * message = (struct message *)(int6k->message);

#ifndef __GNUC__
#pragma pack (push,1)
#endif

	struct __packed vs_rs_dev_confirm 
	{
		struct header_eth ethernet;
		struct header_int intellon;
		uint8_t MSTATUS;
	}
	* confirm = (struct vs_rs_dev_confirm *) (message);

#ifndef __GNUC__
#pragma pack (pop)
#endif

	signed retry = SECONDS (int6k->timer, int6k->retry);
	Request (int6k, "Reset when Ready");
	while (retry-- > 0) 
	{
		memset (message, 0, sizeof (struct message));
		EthernetHeader (&message->ethernet, channel->peer, channel->host);
		IntellonHeader (&message->intellon, (VS_RS_DEV | MMTYPE_REQ));
		int6k->packetsize = ETHER_MIN_LEN;
		if (SendMME (int6k) <= 0) 
		{
			error ((int6k->flags & INT6K_BAILOUT), ECANCELED, INT6K_CANTSEND);
			return (-1);
		}
		if (ReadMME (int6k, (VS_RS_DEV | MMTYPE_CNF)) <= 0) 
		{
			error ((int6k->flags & INT6K_BAILOUT), ECANCELED, INT6K_CANTREAD);
			return (-1);
		}
		if (!confirm->MSTATUS) 
		{
			Confirm (int6k, "Resetting ...");
			return (0);
		}
		sleep (1);
	}
	return (-1);
}

/*====================================================================*
 *
 *   signed WaitForReset (struct int6k * int6k);
 *
 *   int6k.h
 *
 *   poll the device using VS_SW_VER messages until no confirmation 
 *   messages are received indicating that the firmware has stopped 
 *   running; return 0 if the firmware stops within the allotted time
 *   or -1 if it does not or if transmission errors occur;
 *
 *   this function cannot distinguish between a software reset and 
 *   hardware reset;
 *
 *   retry is number of times to poll the device before returniung
 *   an error to indicate that the device failed to reset; timer is
 *   the time between poll attempts;
 *
 *--------------------------------------------------------------------*/

signed WaitForReset (struct int6k * int6k) 

{
	struct channel * channel = (struct channel *)(int6k->channel);
	struct message * message = (struct message *)(int6k->message);
	unsigned retry = 0;
	Request (int6k, "Allow %d seconds for Reset", SECONDS (int6k->timer, int6k->retry));
	while (retry < int6k->retry) 
	{
		memset (message, 0, sizeof (struct message));
		EthernetHeader (&message->ethernet, channel->peer, channel->host);
		IntellonHeader (&message->intellon, (VS_SW_VER | MMTYPE_REQ));
		int6k->packetsize = ETHER_MIN_LEN;
		if (SendMME (int6k) <= 0) 
		{
			error ((int6k->flags & INT6K_BAILOUT), ECANCELED, INT6K_CANTSEND);
			return (-1);
		}
		if (ReadMME (int6k, (VS_SW_VER | MMTYPE_CNF)) == -1) 
		{
			error ((int6k->flags & INT6K_BAILOUT), ECANCELED, INT6K_CANTREAD);
			return (-1);
		}
		if (!int6k->packetsize) 
		{
			if (_allset (int6k->flags, (INT6K_WAITFORRESET | INT6K_ANALYSE))) 
			{
				Confirm (int6k, "Waited %d seconds for Reset", SECONDS (int6k->timer, retry));
			}
			return (0);
		}
		if (++retry < int6k->retry) 
		{
			SLEEP (int6k->timer);
		}
	}
	if (_allset (int6k->flags, (INT6K_WAITFORRESET | INT6K_ANALYSE))) 
	{
		Confirm (int6k, "Waited %d seconds for Reset", SECONDS (int6k->timer, retry));
	}
	return (-1);
}

/*====================================================================*
 *
 *   signed WaitForStart (struct int6k *int6k, char string [], signed length);
 *
 *   int6k.h
 *
 *   poll the device using VS_SW_VER messages until it responds or 
 *   the allotted time expires; return 0 if the device responds within
 *   the allotted time or -1 if it does not or if a transmission error
 *   occurs;
 *
 *   retry is number of poll attempts before returning an error to
 *   indicate that the bootloader or firmware failed to respond in
 *   the allotted time; timer is the time between poll attempts;
 *
 *--------------------------------------------------------------------*/

signed WaitForStart (struct int6k *int6k, char string [], signed length) 

{
	struct channel * channel = (struct channel *)(int6k->channel);
	struct message * message = (struct message *)(int6k->message);

#ifndef __GNUC__
#pragma pack (push,1)
#endif

	struct __packed vs_sw_ver_confirm 
	{
		struct header_eth ethernet;
		struct header_int intellon;
		uint8_t MSTATUS;
		uint8_t MDEVICEID;
		uint8_t MVERLENGTH;
		char MVERSION [INT6K_VERSTRING];
		uint8_t UPGRADEABLE;
	}
	* confirm = (struct vs_sw_ver_confirm *) (message);

#ifndef __GNUC__
#pragma pack (pop)
#endif

	unsigned retry = 0;
	Request (int6k, "Allow %d seconds for Start", SECONDS (int6k->timer, int6k->retry));
	while (retry < int6k->retry) 
	{
		signed status;
		memset (message, 0, sizeof (struct message));
		EthernetHeader (&message->ethernet, channel->peer, channel->host);
		IntellonHeader (&message->intellon, (VS_SW_VER | MMTYPE_REQ));
		int6k->packetsize = ETHER_MIN_LEN;
		if (SendMME (int6k) <= 0) 
		{
			error ((int6k->flags & INT6K_BAILOUT), ECANCELED, INT6K_CANTSEND);
			return (-1);
		}
		status = ReadMME (int6k, (VS_SW_VER | MMTYPE_CNF));
		if (status < 0) 
		{
			error ((int6k->flags & INT6K_BAILOUT), ECANCELED, INT6K_CANTREAD);
			return (-1);
		}
		if (status > 0) 
		{
			if (confirm->MSTATUS) 
			{
				Failure (int6k, INT6K_WONTDOIT);
				return (-1);
			}
			if (_allset (int6k->flags, (INT6K_WAITFORSTART | INT6K_ANALYSE))) 
			{
				Confirm (int6k, "Waited %d seconds for Start", SECONDS (int6k->timer, retry));
			}
			strncpy (string, confirm->MVERSION, length);
			return (0);
		}
		if (++retry < int6k->retry) 
		{
			SLEEP (int6k->timer);
		}
	}
	if (_allset (int6k->flags, (INT6K_WAITFORSTART | INT6K_ANALYSE))) 
	{
		Confirm (int6k, "Waited %d seconds for Start", SECONDS (int6k->timer, retry));
	}
	return (-1);
}

/*====================================================================*
 *   
 *   signed WaitForAssoc (struct int6k * int6k);
 *
 *   int6k.h
 *
 *   poll the device using VS_NW_INFO messages and examine confirmation
 *   messages until the device reports that a network exists and has at
 *   least one station present on that network; return 0 if the device
 *   associates within the allotted time or -1 if it does not or if 
 *   transmission errors occur;
 *
 *   retry is number of times to poll the device before returing an 
 *   error to indicate that the device di not associate in the allotted
 *   time; timer is the time to wait in between poll attempts;
 *
 *   See the Intellon HomePlug AV Firmware Technical Reference Manual
 *   for more information;
 *   
 *--------------------------------------------------------------------*/

signed WaitForAssoc (struct int6k * int6k) 

{
	struct channel * channel = (struct channel *)(int6k->channel);
	struct message * message = (struct message *)(int6k->message);

#ifndef __GNUC__
#pragma pack (push,1)
#endif

	struct __packed station 
	{
		uint8_t DA [ETHER_ADDR_LEN];
		uint8_t TEI;
		uint8_t BDA [ETHER_ADDR_LEN];
		uint8_t AVGTX;
		uint8_t AVGRX;
	}
	* station;
	struct __packed network 
	{
		uint8_t NID [HPAVKEY_NID_LEN];
		uint8_t SNID;
		uint8_t TEI;
		uint8_t ROLE;
		uint8_t CCO_MACADDR [6];
		uint8_t CCO_TEI;
		uint8_t NUMSTAS;
		struct station station [1];
	}
	* network;
	struct __packed networks 
	{
		uint8_t NUMAVLNS;
		struct network network [1];
	}
	* networks = (struct networks *) (&message->content);

#ifndef __GNUC__
#pragma pack (pop)
#endif

	static byte broadcast [ETHER_ADDR_LEN] = 
	{
		0xFF,
		0xFF,
		0xFF,
		0xFF,
		0xFF,
		0xFF 
	};
	unsigned retry = 0;
	signed networkcount;
	signed stationcount;
	Request (int6k, "Allow %d seconds for Assoc", SECONDS (int6k->timer, int6k->retry));
	while (retry < int6k->retry) 
	{
		memset (message, 0, sizeof (struct message));
		EthernetHeader (&message->ethernet, channel->peer, channel->host);
		IntellonHeader (&message->intellon, (VS_NW_INFO | MMTYPE_REQ));
		int6k->packetsize = ETHER_MIN_LEN;
		if (SendMME (int6k) <= 0) 
		{
			error ((int6k->flags & INT6K_BAILOUT), ECANCELED, INT6K_CANTSEND);
			return (-1);
		}
		while (ReadMME (int6k, (VS_NW_INFO | MMTYPE_CNF)) > 0) 
		{
			network = (struct network *)(&networks->network);
			networkcount = networks->NUMAVLNS;
			while (networkcount-- > 0) 
			{
				station = (struct station *)(&network->station);
				stationcount = network->NUMSTAS;
				while (stationcount-- > 0) 
				{
					if (memcmp (station->DA, broadcast, sizeof (broadcast))) 
					{
						if (_allset (int6k->flags, (INT6K_WAITFORASSOC | INT6K_ANALYSE))) 
						{
							Confirm (int6k, "Waited %d seconds for Assoc", SECONDS (int6k->timer, retry));
						}
						return (0);
					}
					station++;
				}
				network = (struct network *)(station);
			}
		}
		if (++retry < int6k->retry) 
		{
			SLEEP (int6k->timer);
		}
	}
	if (_allset (int6k->flags, (INT6K_WAITFORASSOC | INT6K_ANALYSE))) 
	{
		Confirm (int6k, "Waited %d seconds for Assoc", SECONDS (int6k->timer, retry));
	}
	return (-1);
}

/*====================================================================*
 *   
 *   void function (struct int6k *int6k, const char * firmware);
 *
 *   perform operations in a logical order;
 *   
 *.  Intellon INT6x00 Linux Toolkit for HomePlug AV; 
 *:  Published 2007 by Intellon Corp. ALL RIGHTS RESERVED;
 *;  For demonstration; Not for production use;
 *
 *--------------------------------------------------------------------*/

static void function (struct int6k *int6k, const char * firmware) 

{
	char revision [INT6K_VERSTRING];
	if (_anyset (int6k->flags, INT6K_RESET_DEVICE)) 
	{
		if (ResetAndWait (int6k)) 
		{
			Failure (int6k, "Device did not Reset.");
		}
	}
	if (_anyset (int6k->flags, INT6K_WAITFORRESET)) 
	{
		if (WaitForReset (int6k)) 
		{
			Failure (int6k, "Device did not Reset.");
		}
	}
	if (_anyset (int6k->flags, INT6K_WAITFORSTART)) 
	{
		if (WaitForStart (int6k, revision, sizeof (revision))) 
		{
			Failure (int6k, "Device did not Start.");
		}
		if ((firmware) && (*firmware) && strcmp (firmware, revision)) 
		{
			Failure (int6k, "Started wrong firmware");
		}
	}
	if (_anyset (int6k->flags, INT6K_WAITFORASSOC)) 
	{
		if (WaitForAssoc (int6k)) 
		{
			Failure (int6k, "Device did not Assoc.");
		}
	}
	if (int6k->pause) 
	{
		Request (int6k, "Pause %d seconds", int6k->pause);
		sleep (int6k->pause);
	}
	return;
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
	extern const struct _term_ devices [];
	static const char *optv [] = 
	{
		"ac:ef:i:p:qrRstvxw:",
		"device [device] [...] [> stdout]",
		"Intellon INT6000 Procrastinator",
		"a\twait for device assoc",
		"c n\tpolling retry count",
		"e\tredirect stderr messages to stdout",
		"f s\tconfirm firmware is revision s",

#if defined (WINPCAP) || defined (LIBPCAP)

		"i n\thost interface number [2]",

#else

		"i s\thost interface name [" CHANNEL_ETHDEVICE "]",

#endif

		"p n\tpoll frequency per second [5]",
		"q\tquiet mode",
		"v\tverbose mode",
		"R\treset device and wait",
		"r\twait for device reset",
		"s\twait for device start",
		"t\tshow wait times",
		"w n\twait n seconds",
		"x\texit on error",
		(const char *) (0) 
	};

#include "../int6k/int6k-struct.c"

	const char * firmware = "";
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
			_setbits (int6k.flags, INT6K_WAITFORASSOC);
			break;
		case 'c':
			int6k.retry = (unsigned)(uintspec (optarg, 1, INT_MAX));
			break;
		case 'e':
			dup2 (STDOUT_FILENO, STDERR_FILENO);
			break;
		case 'f':
			firmware = optarg;
			break;
		case 'i':

#if defined (WINPCAP) || defined (LIBPCAP)

			channel.id = atoi (optarg);

#else

			channel.name = optarg;

#endif

			break;
		case 'p':
			int6k.timer = _TIMER/(unsigned)(uintspec (optarg, 1, 10));
			break;
		case 'q':
			_setbits (channel.flags, CHANNEL_SILENCE);
			_setbits (int6k.flags, INT6K_SILENCE);
			break;
		case 'r':
			_setbits (int6k.flags, INT6K_WAITFORRESET);
			break;
		case 'R':
			_setbits (int6k.flags, INT6K_RESET_DEVICE);
			break;
		case 's':
			_setbits (int6k.flags, INT6K_WAITFORSTART);
			break;
		case 't':
			_setbits (int6k.flags, INT6K_ANALYSE);
			break;
		case 'v':
			_setbits (channel.flags, CHANNEL_VERBOSE);
			_setbits (int6k.flags, INT6K_VERBOSE);
			break;
		case 'w':
			int6k.pause = (unsigned)(uintspec (optarg, 0, 3600));
			break;
		case 'x':
			_setbits (int6k.flags, INT6K_BAILOUT);
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
	if (!(int6k.message = malloc (sizeof (struct message)))) 
	{
		error (1, errno, INT6K_ERR_MEMORY);
	}
	if (!argc) 
	{
		function (&int6k, firmware);
	}
	while ((argc) && (*argv)) 
	{
		if (!hexencode (channel.peer, sizeof (channel.peer), synonym (*argv, devices, DEVICES))) 
		{
			error (1, errno, INT6K_BAD_MAC, *argv);
		}
		function (&int6k, firmware);
		argv++;
		argc--;
	}
	free (int6k.message);
	closechannel (&channel);
	exit (0);
}

