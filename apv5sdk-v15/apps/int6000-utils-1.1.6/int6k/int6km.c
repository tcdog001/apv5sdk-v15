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
 *   int6km.c - Intellon INT6000 Memory Explorer 
 *
 *   read memory using one VS_RD_MEM message; limit is 1024 bytes at
 *   any offset;
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
#include <string.h>
#include <ctype.h>

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
#include "../int6k/devices.c"
#include "../int6k/EthernetHeader.c"
#include "../int6k/IntellonHeader.c"
#include "../int6k/UnwantedPacket.c"
#endif

#ifndef MAKEFILE
#include "../tools/getoptv.c"
#include "../tools/putoptv.c"
#include "../tools/version.c"
#include "../tools/basespec.c"
#include "../tools/uintspec.c"
#include "../tools/hexencode.c"
#include "../tools/hexdecode.c"
#include "../tools/todigit.c"
#include "../tools/hexdump.c"
#include "../tools/hexview.c"
#include "../tools/regview.c"
#include "../tools/synonym.c"
#include "../tools/error.c"
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
 *   program constants;
 *--------------------------------------------------------------------*/

#define INT6KM_RELATIVE (1 << 0)
#define INT6KM_ABSOLUTE (1 << 1)
#define INT6KM_REGISTER (1 << 2)
#define INT6KM_HEADINGS (1 << 3)

/*====================================================================*
 *
 *   void ReadRAW (struct channel * channel, uint32_t offset, uint32_t length, flag_t flags);
 *   
 *   
 *.  Intellon INT6x00 Linux Toolkit for HomePlug AV; 
 *:  Published 2007 by Intellon Corp. ALL RIGHTS RESERVED;
 *;  For demonstration; Not for production use;
 *
 *--------------------------------------------------------------------*/

static void ReadRAW (struct channel * channel, uint32_t offset, uint32_t length, flag_t flags) 

{
	struct message message;
	signed packetsize;

#ifndef __GNUC__
#pragma pack (push,1)
#endif

	struct __packed vs_rd_mem_request 
	{
		struct header_eth ethernet;
		struct header_int intellon;
		uint32_t MOFFSET;
		uint32_t MLENGTH;
	}
	* request = (struct vs_rd_mem_request *)(&message);
	struct __packed vs_rd_mem_confirm 
	{
		struct header_eth ethernet;
		struct header_int intellon;
		uint8_t MSTATUS;
		uint32_t MOFFSET;
		uint32_t MLENGTH;
		uint8_t MBUFFER [INT6K_BLOCKSIZE];
	}
	* confirm = (struct vs_rd_mem_confirm *)(&message);

#ifndef __GNUC__
#pragma pack (pop)
#endif

	memset (&message, 0, sizeof (message));
	EthernetHeader (&message.ethernet, channel->peer, channel->host);
	IntellonHeader (&message.intellon, (VS_RD_MEM | MMTYPE_REQ));
	request->MOFFSET = HTOLE32 (offset);
	request->MLENGTH = HTOLE32 (length);
	if (sendpacket (channel, &message, ETHER_MIN_LEN) == -1) 
	{
		error (1, ECANCELED, INT6K_CANTSEND);
	}
	while ((packetsize = readpacket (channel, &message, sizeof (message))) > 0) 
	{
		if (UnwantedPacket (&message, (VS_RD_MEM | MMTYPE_CNF))) 
		{
			continue;
		}
		if (confirm->MSTATUS) 
		{
			error (0, 0, "%s (%0X): %s", MMECode (confirm->intellon.MMTYPE, confirm->MSTATUS), confirm->MSTATUS, INT6K_WONTDOIT);
			continue;
		}
		if (_anyset (flags, INT6KM_REGISTER)) 
		{
			if (_anyset (flags, INT6KM_HEADINGS)) 
			{
				printf (REGDUMP_HEADER);
			}
			regview (confirm->MBUFFER, LE32TOH (confirm->MOFFSET), LE32TOH (confirm->MLENGTH), stdout);
			continue;
		}
		if (_anyset (flags, INT6KM_ABSOLUTE)) 
		{
			if (_anyset (flags, INT6KM_HEADINGS)) 
			{
				printf (HEXDUMP_HEADER);
			}
			hexview (confirm->MBUFFER, LE32TOH (confirm->MOFFSET), LE32TOH (confirm->MLENGTH), stdout);
			continue;
		}
		if (_anyset (flags, INT6KM_HEADINGS)) 
		{
			printf (HEXDUMP_HEADER);
		}
		hexdump (confirm->MBUFFER, 0, LE32TOH (confirm->MLENGTH), stdout);
	}
	if (packetsize == -1) 
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
		"aehi:l:o:qrv",
		"[device] [...]",
		"Intellon INT6000 Memory Explorer",
		"a\tdisplay absolute offsets",
		"e\tredirect stderr messages to stdout",
		"h\tdisplay simple headings",

#if defined (WINPCAP) || defined (LIBPCAP)

		"i n\thost interface number [2]",

#else

		"i s\thost interface name [" CHANNEL_ETHDEVICE "]",

#endif

		"l n\tmemory length in bytes [4]",
		"o n\tmemory offset [0x00000000]",
		"q\tquiet mode",
		"r\tdisplay as binary registers",
		"v\tverbose mode",
		(const char *) (0) 
	};
	flag_t flags = (flag_t) (0);
	uint32_t offset = 0;
	uint32_t length = 4;
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
			_setbits (flags, INT6KM_ABSOLUTE);
			break;
		case 'e':
			dup2 (STDOUT_FILENO, STDERR_FILENO);
			break;
		case 'h':
			_setbits (flags, INT6KM_HEADINGS);
			break;
		case 'i':

#if defined (WINPCAP) || defined (LIBPCAP)

			channel.id = atoi (optarg);

#else

			channel.name = optarg;

#endif

			break;
		case 'l':
			length = (uint32_t)(uintspec (optarg, 0, 1024));
			break;
		case 'o':
			offset = (uint32_t)(basespec (optarg, 16, sizeof (offset)));
			break;
		case 'q':
			_setbits (channel.flags, CHANNEL_SILENCE);
			break;
		case 'r':
			_setbits (flags, INT6KM_REGISTER);
			break;
		case 'v':
			_setbits (channel.flags, CHANNEL_VERBOSE);
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

	if (_anyset (flags, INT6KM_REGISTER)) 
	{
		offset &= ~(3);
		length &= ~(3);
	}
	openchannel (&channel);
	if (!argc) 
	{
		ReadRAW (&channel, offset, length, flags);
	}
	while ((argc) && (*argv)) 
	{
		if (!hexencode (channel.peer, sizeof (channel.peer), synonym (*argv, devices, DEVICES))) 
		{
			error (1, errno, INT6K_BAD_MAC, *argv);
		}
		ReadRAW (&channel, offset, length, flags);
		argv++;
		argc--;
	}
	closechannel (&channel);
	return (0);
}

