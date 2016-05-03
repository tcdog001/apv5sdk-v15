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
 *   coqos_info.c - Retrieve MCMSA stream info
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
 *	Bill Wike <bill.wike@intellon.com>
 *	Charles Maier <charles.maier@intellon.com>
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
#include "../coqos/coqos.h"

/*====================================================================*
 *   custom source files;
 *--------------------------------------------------------------------*/

#ifndef MAKEFILE
#include "../tools/getoptv.c"
#include "../tools/putoptv.c"
#include "../tools/version.c"
#include "../tools/basespec.c"
#include "../tools/hexdump.c"
#include "../tools/hexencode.c"
#include "../tools/bytespec.c"
#include "../tools/ipv4spec.c"
#include "../tools/ipv6spec.c"
#include "../tools/hexdecode.c"
#include "../tools/todigit.c"
#include "../tools/typename.c"
#include "../tools/endian.c"
#include "../tools/error.c"
#include "../tools/synonym.c"
#include "../tools/lookup.c"
#include "../tools/assist.c"
#include "../tools/expect.c"
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
#include "../ether/channel.c"
#include "../ether/openchannel.c"
#include "../ether/closechannel.c"
#include "../ether/readpacket.c"
#include "../ether/sendpacket.c"
#endif

#ifndef MAKEFILE
#include "../mme/MMECode.c"
#endif

/*====================================================================*
 *   constants;    
 *--------------------------------------------------------------------*/

#define CONTROLS 6
#define CONTROL "control"

/*====================================================================*
 *   program variables;
 *--------------------------------------------------------------------*/

#define CONTROL_STA 0x00
#define CONTROL_CID 0x01
#define CONTROL_RSVD 0x02
#define CONTROL_VIC 0x03
#define CONTROL_BW 0x04
#define CONTROL_BWC 0xFF

static struct _code_ controls [CONTROLS] = 

{
	{
		CONTROL_STA,
		"STA" 
	},
	{
		CONTROL_CID,
		"CID" 
	},
	{
		CONTROL_RSVD,
		"RSVD" 
	},
	{
		CONTROL_VIC,
		"VIC" 
	},
	{
		CONTROL_BW,
		"BW" 
	},
	{
		CONTROL_BWC,
		"BWC" 
	}
};

/*====================================================================*
 *
 *   signed con_info (struct int6k * int6k, uint8_t TYPE, uint16_t CID, uint16_t TOT_BW_USED);
 *
 *
 *.  Intellon INT6x00 Linux Toolkit for HomePlug AV; 
 *:  Published 2007 by Intellon Corp. ALL RIGHTS RESERVED;
 *;  For demonstration; Not for production use;
 *
 *   Contributor(s):
 *	Bill Wike <bill.wike@intellon.com>
 *
 *--------------------------------------------------------------------*/

static signed con_info (struct int6k * int6k, uint8_t TYPE, uint16_t CID, uint16_t TOT_BW_USED) 

{

#ifndef __GNUC__
#pragma pack (push,1)
#endif

	struct __packed vs_con_info_req 
	{
		struct header_eth ethernet;
		struct header_int intellon;
		uint8_t REQ_TYPE;
		uint32_t RSVD;
		uint16_t CSPEC_VER;
		uint16_t CID;
		uint16_t TOT_BW_USED;
	}
	* request = (struct vs_con_info_req *)(int6k->message);
	struct __packed vs_con_info_cnf 
	{
		struct header_eth ethernet;
		struct header_int intellon;
		uint8_t REQ_TYPE;
		uint8_t MSTATUS;
		uint16_t ERR_REC_CODE;
		uint32_t RSVD;
		uint8_t NUM_CONN;
		struct conn_info CONN_INFO [64];
	}
	* confirm = (struct vs_con_info_cnf *)(int6k->message);
	struct conn_info * conn_info = (struct conn_info*)(confirm->CONN_INFO);
	struct vs_con_info_cnf * indicate = (struct vs_con_info_cnf *)(int6k->message);

#ifndef __GNUC__
#pragma pack (pop)
#endif

	int i;
	Request (int6k, "COQOS connection information");
	memset (int6k->message, 0, sizeof (struct message));
	EthernetHeader (&int6k->message->ethernet, int6k->channel->peer, int6k->channel->host);
	IntellonHeader (&int6k->message->intellon, (VS_CON_INFO | MMTYPE_REQ));
	int6k->packetsize = ETHER_MIN_LEN;
	request->REQ_TYPE = TYPE;
	request->CID = CID;
	request->TOT_BW_USED = TOT_BW_USED;
	request->CSPEC_VER = 0x01;
	if (SendMME (int6k) <= 0) 
	{
		error ((int6k->flags & INT6K_BAILOUT), errno, INT6K_CANTSEND);
		return (-1);
	}
	while (ReadMME (int6k, (VS_CON_INFO | MMTYPE_CNF)) <= 0);
	if (confirm->MSTATUS) 
	{
		Failure (int6k, "Error requesting information.");
		return (-1);
	}

/* BW exceeded request and "cancel all requests" do not send indicate */

	if ((TYPE == CONTROL_BW) || (TYPE == CONTROL_BWC)) 
	{
		Confirm (int6k, "Success");
		return (0);
	}
	Confirm (int6k, "Received confirm, waiting for indicate...");
	while (ReadMME (int6k, (VS_CON_INFO | MMTYPE_IND)) <= 0);
	if (indicate->MSTATUS) 
	{
		Failure (int6k, "Error requesting information.");
		return (-1);
	}
	printf ("Number of Connections: %d\n", (int)indicate->NUM_CONN);
	conn_info = (struct conn_info *)(indicate->CONN_INFO);
	for (i = 0; i < indicate->NUM_CONN; i++) 
	{
		printf ("SNID: %02X ", conn_info->SNID);
		printf ("CID: %04X ", conn_info->CID);
		printf ("STEI: %02X ", conn_info->STEI);
		printf ("DTEI: %02X ", conn_info->DTEI);
		printf ("LLID: %02X ", conn_info->LLID);
		printf ("CSPEC_VERSION: %04X ", conn_info->CSPECVERSION);
		printf ("CONN_CAP: %02d ", conn_info->CONN_CAP);
		printf ("CONN_COQOS_PRIO: %02d ", conn_info->CONN_COQOS_PRIO);
		printf ("CONN_RATE: %d ", conn_info->CONN_RATE);
		printf ("CONN_TTL: %d ", conn_info->CONN_TTL);
		printf ("BW_USED: %d \n", conn_info->BW_USED);
		conn_info++;
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
		"ei:qv",
		"sta,cid,rsvd,vic,bw,bwc [limit] [device] [...]",
		"CoQos Stream Utility",
		"e\tredirect stderr messages to stdout",

#if defined (WINPCAP) || defined (LIBPCAP)

		"i n\thost interface number [2]",

#else

		"i s\thost interface name [" CHANNEL_ETHDEVICE "]",

#endif

		"q\tquiet - suppress routine messages",
		"v\tverbose messages",
		(const char *) (0) 
	};

#include "../int6k/int6k-struct.c"

	uint16_t cid = 0;
	uint16_t limit;
	uint8_t type;
	signed code;
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
		case 'e':
			dup2 (STDOUT_FILENO, STDERR_FILENO);
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
			_setbits (int6k.flags, INT6K_SILENCE);
			break;
		case 'v':
			_setbits (channel.flags, CHANNEL_VERBOSE);
			_setbits (int6k.flags, INT6K_VERBOSE);
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

	if ((code = lookup (*argv++, controls, CONTROLS)) == -1) 
	{
		assist (*--argv, CONTROL, controls, CONTROLS, stderr);
	}
	type = (uint8_t)(code);
	argc--;
	if (type == CONTROL_CID) 
	{
		if (!argc) 
		{
			error (1, errno, "Expected Connection Identifier");
		}
		if (!hexencode ((uint8_t *)(&cid), sizeof (cid), *argv++)) 
		{
			error (1, errno, "Invalid CID");
		}
		cid = htons (cid);
		argc--;
	}
	else if (type == CONTROL_BW) 
	{
		if (!argc) 
		{
			error (1, errno, "Expected Bandwidth Limit");
		}
		limit = atoi (*argv++);
		argc--;
	}
	openchannel (&channel);
	if (!(int6k.message = malloc (sizeof (struct message)))) 
	{
		error (1, errno, INT6K_ERR_MEMORY);
	}
	if (!argc) 
	{
		con_info (&int6k, type, cid, limit);
	}
	while ((argc) && (*argv)) 
	{
		if (!hexencode (channel.peer, sizeof (channel.peer), synonym (*argv, devices, DEVICES))) 
		{
			error (1, errno, INT6K_BAD_MAC, *argv);
		}
		con_info (&int6k, type, cid, limit);
		argc--;
		argv++;
	}
	free (int6k.message);
	closechannel (&channel);
	exit (0);
}

