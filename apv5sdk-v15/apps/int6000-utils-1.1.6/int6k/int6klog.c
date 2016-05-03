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
 *   int6klog.c - Intellon INT6x00 Log File Manager;
 *
 *   Intellon INT6x00 Linux Toolkit; 
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
#include <stdint.h>
#include <stdlib.h>
#include <ctype.h>

/*====================================================================*
 *   custom header files;
 *--------------------------------------------------------------------*/

#include "../tools/getoptv.h"
#include "../tools/memory.h"
#include "../tools/number.h"
#include "../tools/symbol.h"
#include "../tools/format.h"
#include "../tools/base64.h"
#include "../tools/types.h"
#include "../tools/flags.h"
#include "../tools/files.h"
#include "../tools/error.h"
#include "../int6k/int6k.h"
#include "../ether/channel.h"
#include "../int6k/int6k.h"

/*====================================================================*
 *   custom source files;
 *--------------------------------------------------------------------*/

#ifndef MAKEFILE
#include "../int6k/devices.c"
#include "../int6k/Confirm.c"
#include "../int6k/Display.c"
#include "../int6k/Request.c"
#include "../int6k/Failure.c"
#include "../int6k/SendMME.c"
#include "../int6k/ReadMME.c"
#include "../int6k/EthernetHeader.c"
#include "../int6k/IntellonHeader.c"
#include "../int6k/UnwantedPacket.c"
#endif

#ifndef MAKEFILE
#include "../tools/alert.c"
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
#include "../tools/strfbits.c"
#include "../tools/filepart.c"
#include "../tools/typename.c"
#include "../tools/markup.c"
#include "../tools/synonym.c"
#include "../tools/b64dump.c"
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

#define MSTATUS_STATUS (7 << 0) 
#define MSTATUS_MAJORVERSION (1 << 3) 
#define MSTATUS_BUFFERISLOCKED (1 << 4) 
#define MSTATUS_AUTOLOCKONRESET (1 << 5) 
#define MSTATUS_UNSOLICITEDUPDATES (1 << 6) 
#define MSTATUS_UNSOLICITED (1 << 7) 

/*====================================================================*
 *   
 *   signed WatchdogReportXML (struct int6k * int6k);
 *   
 *   Read the watchdog reqport using VS_WD_RPT and write to file in
 *   binary or XML format; this file can be sent to Intellon Support
 *   for analysis;
 *   
 *   The XML version rquires additional information and so VS_SW_VER
 *   is used to collect it;
 *
 *   See the Intellon HomePlug AV Firmware Technical Reference Manual
 *   for more information;
 *
 *   This VW_WD_RPT message protocol returns an indication message, 
 *   not a confirm message;
 *   
 *.  Intellon INT6x00 Linux Toolkit for HomePlug AV; 
 *:  Published 2007 by Intellon Corp. ALL RIGHTS RESERVED;
 *;  For demonstration; Not for production use;
 *
 *   Contributor(s):
 *      Charles Maier <charles.maier@intellon.com>
 *
 *--------------------------------------------------------------------*/

static signed WatchdogReportXML (struct int6k * int6k, const char * version) 

{

	struct channel * channel = (struct channel *)(int6k->channel);
	struct message * message = (struct message *)(int6k->message);
#ifndef __GNUC__
#pragma pack (push,1)
#endif

	struct __packed vs_wd_rpt_request 
	{
		struct header_eth ethernet;
		struct header_int intellon;
		uint16_t SESSIONID;
		uint8_t CLR;
	}
	* request = (struct vs_wd_rpt_request *) (message);
	struct __packed vs_wd_rpt_ind 
	{
		struct header_eth ethernet;
		struct header_int intellon;
		uint8_t MSTATUS;
		uint16_t SESSIONID;
		uint8_t NUMPARTS;
		uint8_t CURPART;
		uint16_t RDATALENGTH;
		uint8_t RDATAOFFSET;
		uint8_t RDATA [1];
	}
	* indicate = (struct vs_wd_rpt_ind *) (message);

#ifndef __GNUC__
#pragma pack (pop)
#endif

	memset (message, 0, sizeof (struct message));
	EthernetHeader (&message->ethernet, channel->peer, channel->host);
	IntellonHeader (&message->intellon, (VS_WD_RPT | MMTYPE_REQ));
	int6k->packetsize = ETHER_MIN_LEN;
	request->SESSIONID = 0xFEFE;
	request->CLR = int6k->readaction;
	if (SendMME (int6k) <= 0) 
	{
		error ((int6k->flags & INT6K_BAILOUT), ECANCELED, INT6K_CANTSEND);
		return (-1);
	}
	printf ("<WatchdogReport>");
	do 
	{
		if (ReadMME (int6k, (VS_WD_RPT | MMTYPE_IND)) <= 0) 
		{
			error ((int6k->flags & INT6K_BAILOUT), ECANCELED, INT6K_CANTREAD);
			return (-1);
		}
		if (indicate->MSTATUS) 
		{
			Failure (int6k, INT6K_WONTDOIT);
			return (-1);
		}
		printf ("<Packet>");
		printf ("<Version>%s</Version>", version);
		printf ("<OUI>%s</OUI>", "00000000");
		printf ("<Status>0</Status>");
		printf ("<SessionId>0</SessionId>");
		printf ("<NumParts>%d</NumParts>", indicate->NUMPARTS);
		printf ("<CurPart>%d</CurPart>", indicate->CURPART);
		printf ("<DataLength>%d</DataLength>", LE16TOH (indicate->RDATALENGTH));
		printf ("<DataOffset>%d</DataOffset>", indicate->RDATAOFFSET);
		printf ("<Data>");
		b64dump (indicate->RDATA, LE16TOH (indicate->RDATALENGTH), 0, stdout);
		printf ("</Data>");
		printf ("</Packet>");
	}
	while (indicate->CURPART < indicate->NUMPARTS);
	printf ("</WatchdogReport>");
	return (0);
}

/*====================================================================*
 *   
 *   signed CheckpointReportXML (struct int6k * int6k);
 *   
 *   Read the watchdog reqport using VS_WD_RPT and write to file in
 *   binary or XML format; this file can be sent to Intellon Support
 *   for analysis;
 *   
 *   The XML version rquires additional information and so VS_SW_VER
 *   is used to collect it;
 *
 *   See the Intellon HomePlug AV Firmware Technical Reference Manual
 *   for more information;
 *
 *   This VW_WD_RPT message protocol returns an indication message, 
 *   not a confirm message;
 *   
 *.  Intellon INT6x00 Linux Toolkit for HomePlug AV; 
 *:  Published 2007 by Intellon Corp. ALL RIGHTS RESERVED;
 *;  For demonstration; Not for production use;
 *
 *   Contributor(s):
 *      Charles Maier <charles.maier@intellon.com>
 *
 *--------------------------------------------------------------------*/

static signed CheckpointReportXML (struct int6k * int6k, const char * version) 

{

	struct channel * channel = (struct channel *)(int6k->channel);
	struct message * message = (struct message *)(int6k->message);
#ifndef __GNUC__
#pragma pack (push,1)
#endif

	struct __packed vs_cp_rpt_request 
	{
		struct header_eth ethernet;
		struct header_int intellon;
		uint16_t SESSIONID;
		uint8_t CLR;
	}
	* request = (struct vs_cp_rpt_request *) (message);
	struct __packed vs_cp_rpt_ind 
	{
		struct header_eth ethernet;
		struct header_int intellon;
		uint8_t MSTATUS;
		uint8_t MAJORVERSION;
		uint8_t MINORVERSION;
		uint8_t RESERVED [14];
		uint16_t SESSIONID;
		uint32_t TOTALBUFFERSIZE;
		uint32_t BLOCKOFFSET;
		uint32_t BYTEINDEXOFNEXTPOSITION;
		uint8_t NUMPARTS;
		uint8_t CURPART;
		uint16_t RDATALENGTH;
		uint8_t RDATAOFFSET;
		uint8_t RDATA [1];
	}
	* indicate = (struct vs_cp_rpt_ind *) (message);

#ifndef __GNUC__
#pragma pack (pop)
#endif

	memset (message, 0, sizeof (struct message));
	EthernetHeader (&request->ethernet, channel->peer, channel->host);
	IntellonHeader (&request->intellon, (VS_CP_RPT | MMTYPE_REQ));
	int6k->packetsize = ETHER_MIN_LEN;
	request->SESSIONID = 0xEFEF;
	request->CLR = int6k->readaction;
	if (SendMME (int6k) <= 0) 
	{
		error ((int6k->flags & INT6K_BAILOUT), ECANCELED, INT6K_CANTSEND);
		return (-1);
	}
	printf ("<CheckpointReport>");
	do 
	{
		if (ReadMME (int6k, (VS_CP_RPT | MMTYPE_IND)) <= 0) 
		{
			error ((int6k->flags & INT6K_BAILOUT), ECANCELED, INT6K_CANTREAD);
			return (-1);
		}
		if (_anyset (indicate->MSTATUS, MSTATUS_STATUS)) 
		{
			Failure (int6k, INT6K_WONTDOIT);
			return (-1);
		}
		printf ("<Packet>");
		printf ("<Version>%s</Version>", version);
		printf ("<Status>0</Status>");
		printf ("<MajorVersionBit>%d</MajorVersionBit>", indicate->MSTATUS & MSTATUS_MAJORVERSION? 1:0);
		printf ("<BufferIsLocked>%d</BufferIsLocked>", indicate->MSTATUS & MSTATUS_BUFFERISLOCKED? 1:0);
		printf ("<AutoLockOnResetIsOn>%d</AutoLockOnResetIsOn>", indicate->MSTATUS & MSTATUS_AUTOLOCKONRESET? 1:0);
		printf ("<UnsolicitedUpdatesIsOn>%d</UnsolicitedUpdatesIsOn>", indicate->MSTATUS & MSTATUS_UNSOLICITEDUPDATES? 1:0);
		printf ("<Unsolicited>%d</Unsolicited>", indicate->MSTATUS & MSTATUS_UNSOLICITED? 1:0);
		printf ("<MajorVersion>%d</MajorVersion>", indicate->MAJORVERSION);
		printf ("<MinorVersion>%d</MinorVersion>", indicate->MINORVERSION);
		printf ("<Reserved1>0</Reserved1>");
		printf ("<Reserved2>0</Reserved2>");
		printf ("<Reserved3>0</Reserved3>");
		printf ("<Reserved4>0</Reserved4>");
		printf ("<SessionId>1</SessionId>");
		printf ("<TotalBufferSize>%d</TotalBufferSize>", indicate->TOTALBUFFERSIZE);
		printf ("<BlockOffset>%d</BlockOffset>", indicate->BLOCKOFFSET);
		printf ("<ByteIndexOfNextPos>%d</ByteIndexOfNextPos>", indicate->BYTEINDEXOFNEXTPOSITION);
		printf ("<NumParts>%d</NumParts>", indicate->NUMPARTS);
		printf ("<CurPart>%d</CurPart>", indicate->CURPART);
		printf ("<DataLength>%d</DataLength>", LE16TOH (indicate->RDATALENGTH));
		printf ("<DataOffset>%d</DataOffset>", indicate->RDATAOFFSET);
		printf ("<Data>");
		b64dump (indicate->RDATA, LE16TOH (indicate->RDATALENGTH), 0, stdout);
		printf ("</Data>");
		printf ("</Packet>");
	}
	while (indicate->CURPART < indicate->NUMPARTS);
	printf ("</CheckpointReport>");
	return (0);
}

/*====================================================================*
 *   
 *   static signed Diagnostics (struct int6k *int6k);
 *   
 *   read the firmware version string from a device before reading
 *   and writing the watchdog and checkpoint reports in XML format;
 *
 *.  Intellon INT6x00 Linux Toolkit for HomePlug AV; 
 *:  Published 2007 by Intellon Corp. ALL RIGHTS RESERVED;
 *;  For demonstration; Not for production use;
 *
 *--------------------------------------------------------------------*/

static signed Diagnostics (struct int6k *int6k) 

{
	char version [INT6K_VERSTRING];
	struct channel * channel = (struct channel *)(int6k->channel);
	struct message * message = (struct message *)(int6k->message);

#ifndef __GNUC__
#pragma pack (push,1)
#endif

	struct __packed vs_sw_ver_request 
	{
		struct header_eth ethernet;
		struct header_int intellon;
		uint8_t MSTATUS;
		uint8_t MDEVICEID;
		uint8_t MVERLENGTH;
		char MVERSION [INT6K_VERSTRING];
		uint8_t UPGRADEABLE;
		uint32_t MCOOKIE;
	}
	* request = (struct vs_sw_ver_request *) (message);
	struct __packed vs_sw_ver_confirm 
	{
		struct header_eth ethernet;
		struct header_int intellon;
		uint8_t MSTATUS;
		uint8_t MDEVICEID;
		uint8_t MVERLENGTH;
		char MVERSION [INT6K_VERSTRING];
		uint8_t UPGRADEABLE;
		uint32_t MCOOKIE;
	}
	* confirm = (struct vs_sw_ver_confirm *) (message);

#ifndef __GNUC__
#pragma pack (pop)
#endif

	memset (message, 0, sizeof (struct message));
	EthernetHeader (&request->ethernet, channel->peer, channel->host);
	IntellonHeader (&request->intellon, (VS_SW_VER | MMTYPE_REQ));
	int6k->packetsize = ETHER_MIN_LEN;
	if (SendMME (int6k) <= 0) 
	{
		error ((int6k->flags & INT6K_BAILOUT), ECANCELED, INT6K_CANTSEND);
		return (-1);
	}
	if (ReadMME (int6k, (VS_SW_VER | MMTYPE_CNF)) <= 0) 
	{
		error ((int6k->flags & INT6K_BAILOUT), ECANCELED, INT6K_CANTREAD);
		return (-1);
	}
	if (confirm->MSTATUS) 
	{
		Failure (int6k, INT6K_WONTDOIT);
		return (-1);
	}
	memcpy (version, confirm->MVERSION, sizeof (version));
	printf ("<?xml version='1.0' encoding='utf-8' standalone='yes'?>");
	printf ("<Diagnostics>");
	WatchdogReportXML (int6k, version);
	CheckpointReportXML (int6k, version);
	printf ("</Diagnostics>");
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
	extern const struct _term_ devices [];
	static const char *optv [] = 
	{
		"cei:qv",
		"device [device] [...] [> stdout]",
		"Intellon INT6x00 Log File Manager",
		"c\tclear Watchdog and Checkpoint Reports after reading",
		"e\tredirect stderr messages to stdout",

#if defined (WINPCAP) || defined (LIBPCAP)

		"i n\thost interface number is n [2]",

#else

		"i s\thost interface name is s [" CHANNEL_ETHDEVICE "]",

#endif

		"q\tquiet mode",
		"v\tverbose mode",
		(const char *) (0) 
	};

#include "../int6k/int6k-struct.c"

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
	int6k.readaction = 1;
	while ((c = getoptv (argc, argv, optv)) != -1) 
	{
		switch ((char) (c)) 
		{
		case 'c':
			int6k.readaction |= 2;
			break;
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

/*
 *   if no hardware addresses or multiple hardware addresses are 
 *   specified on the command line and the user elected to read 
 *   either the MAC or PIB from RAM then terminate the program to 
 *   prevent accidental overwriting of files; no addresse defaults
 *   to the local management address which could cause many devices
 *   to respond; note that you can specify a broadcast address to
 *   bypass this interlock.
 */

	if (argc != 1) 
	{
		if (int6k.rpt.file != -1) 
		{
			error (1, ECANCELED, INT6K_NODEVICE);
		}
	}

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
		Diagnostics (&int6k);
	}
	while ((argc) && (*argv)) 
	{
		if (!hexencode (channel.peer, sizeof (channel.peer), synonym (*argv, devices, DEVICES))) 
		{
			error (1, errno, INT6K_BAD_MAC, *argv);
		}
		Diagnostics (&int6k);
		argc--;
		argv++;
	}
	free (int6k.message);
	closechannel (&channel);
	exit (0);
}

