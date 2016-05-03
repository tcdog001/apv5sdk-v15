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
 *   int6kid.c - Intellon INT6000 Device Identity 
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
#include <limits.h>
#include <string.h>
#include <ctype.h>

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
#include "../ram/nvram.h"
#include "../ram/sdram.h"
#include "../nvm/nvm.h"
#include "../pib/pib.h"
#include "../mme/mme.h"

/*====================================================================*
 *   custom source files;
 *--------------------------------------------------------------------*/

#ifndef MAKEFILE
#include "../tools/getoptv.c"
#include "../tools/putoptv.c"
#include "../tools/version.c"
#include "../tools/uintspec.c"
#include "../tools/hexencode.c"
#include "../tools/hexdecode.c"
#include "../tools/todigit.c"
#include "../tools/hexdump.c"
#include "../tools/hexout.c"
#include "../tools/error.c"
#include "../tools/synonym.c"
#endif

#ifndef MAKEFILE
#include "../int6k/EthernetHeader.c"
#include "../int6k/IntellonHeader.c"
#include "../int6k/UnwantedPacket.c"
#include "../int6k/Display.c"
#include "../int6k/devices.c"
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

#define INT6KID_VERBOSE (1 << 0)
#define INT6KID_SILENCE (1 << 1)
#define INT6KID_NEWLINE (1 << 2)

#define INT6KID_DAK 0
#define INT6KID_NMK 1
#define INT6KID_MAC 2

/*====================================================================*
 *
 *   void ReadKey (struct channel * channel, unsigned c, int key);
 *   
 *   read the first block of the PIB from a device then echo one of 
 *   several parameters on stdout as a string; program output can be 
 *   used in scripts to define variables or compare strings;
 *
 *   this function is an abridged version of ReadPIB(); it reads only
 *   the first 1024 bytes of the PIB then stops; most parameters of
 *   general interest occur in that block;
 *   
 *.  Intellon INT6x00 Linux Toolkit for HomePlug AV; 
 *:  Published 2007 by Intellon Corp. ALL RIGHTS RESERVED;
 *;  For demonstration; Not for production use;
 *
 *--------------------------------------------------------------------*/

static void ReadKey (struct channel * channel, unsigned c, int key) 

{
	struct message message;
	static signed count = 0;
	signed packetsize;

#ifndef __GNUC__
#pragma pack (push,1)
#endif

	struct __packed vs_rd_mod_request 
	{
		struct header_eth ethernet;
		struct header_int intellon;
		uint8_t MODULEID;
		uint8_t RESERVED;
		uint16_t MLENGTH;
		uint32_t MOFFSET;
		uint8_t DAK [16];
	}
	* request = (struct vs_rd_mod_request *)(&message);
	struct __packed vs_rd_mod_confirm 
	{
		struct header_eth ethernet;
		struct header_int intellon;
		uint8_t MSTATUS;
		uint8_t RESERVED1 [3];
		uint8_t MODULEID;
		uint8_t RESERVED2;
		uint16_t MLENGTH;
		uint32_t MOFFSET;
		uint32_t MCHKSUM;
		struct simple_pib pib;
	}
	* confirm = (struct vs_rd_mod_confirm *)(&message);

#ifndef __GNUC__
#pragma pack (pop)
#endif

	memset (&message, 0, sizeof (message));
	EthernetHeader (&message.ethernet, channel->peer, channel->host);
	IntellonHeader (&message.intellon, (VS_RD_MOD | MMTYPE_REQ));
	request->MODULEID = VS_MODULE_PIB;
	request->MLENGTH = HTOLE16 (INT6K_BLOCKSIZE);
	request->MOFFSET = HTOLE32 (0);
	if (sendpacket (channel, &message, ETHER_MIN_LEN) < 0) 
	{
		error (1, ECANCELED, INT6K_CANTSEND);
	}
	while ((packetsize = readpacket (channel, &message, sizeof (message))) > 0) 
	{
		if (UnwantedPacket (&message, (VS_RD_MOD | MMTYPE_CNF))) 
		{
			continue;
		}
		if (confirm->MSTATUS) 
		{
			error (0, 0, "%s (%0X): ", MMECode (confirm->intellon.MMTYPE, confirm->MSTATUS), confirm->MSTATUS);
			continue;
		}
		if (count++ > 0) 
		{
			putc (c, stdout);
		}
		if (key == INT6KID_MAC) 
		{
			hexout (confirm->pib.MAC, sizeof (confirm->pib.MAC), HEX_EXTENDER, stdout);
			continue;
		}
		if (key == INT6KID_DAK) 
		{
			hexout (confirm->pib.DAK, sizeof (confirm->pib.DAK), HEX_EXTENDER, stdout);
			continue;
		}
		if (key == INT6KID_NMK) 
		{
			hexout (confirm->pib.NMK, sizeof (confirm->pib.NMK), HEX_EXTENDER, stdout);
			continue;
		}
	}
	if (packetsize < 0) 
	{
		error (1, ECANCELED, INT6K_CANTREAD);
	}
	return;
}

/*====================================================================*
 *
 *   int main (int argc, const char * argv []);
 *   
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
		"Ac:Dei:Mnqv",
		"device",
		"Intellon INT6000 Device Identity",
		"A\tEthernet address (MAC)",
		"c c\tcharacter delimiter is (c)",
		"D\tDevice Access Key (DAK)",
		"e\tredirect stderr messages to stdout",

#if defined (WINPCAP) || defined (LIBPCAP)

		"i n\thost interface number [2]",

#else

		"i s\thost interface name [" CHANNEL_ETHDEVICE "]",

#endif

		"M\tNetwork Membership Key (NMK)",
		"n\tappend newline on output",
		"q\tquiet mode",
		"v\tverbose mode",
		(const char *) (0) 
	};
	signed newline = '\n';
	signed key = INT6KID_DAK;
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
	optmin = 1;
	while ((c = getoptv (argc, argv, optv)) != -1) 
	{
		switch ((char) (c)) 
		{
		case 'A':
			key = INT6KID_MAC;
			break;
		case 'c':
			newline = * optarg;
			break;
		case 'D':
			key = INT6KID_DAK;
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
		case 'M':
			key = INT6KID_NMK;
			break;
		case 'n':
			_setbits (flags, INT6KID_NEWLINE);
			break;
		case 'q':
			_setbits (channel.flags, CHANNEL_SILENCE);
			_setbits (flags, INT6K_SILENCE);
			break;
		case 'v':
			_setbits (channel.flags, CHANNEL_VERBOSE);
			_setbits (flags, INT6K_VERBOSE);
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
 *      root when this program is installed; setuid using 'chown root:root' and 
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
		ReadKey (&channel, newline, key);
		if (_anyset (flags, INT6KID_NEWLINE)) 
		{
			putc (newline, stdout);
		}
	}
	while ((argc) && (*argv)) 
	{
		if (!hexencode (channel.peer, sizeof (channel.peer), synonym (*argv, devices, DEVICES))) 
		{
			error (1, errno, INT6K_BAD_MAC, *argv);
		}
		ReadKey (&channel, newline, key);
		if (_anyset (flags, INT6KID_NEWLINE)) 
		{
			putc (newline, stdout);
		}
		argv++;
		argc--;
	}
	closechannel (&channel);
	return (0);
}

