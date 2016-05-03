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
 *   hpav2.c - Intellon Packet Listener;
 *   
 *   this is an example program that prints Intellon Vendor Specific
 *   HomePlug AV packets exchanged between hosts and Intellon devices 
 *   connected to the local network;
 *
 *   this program receives raw ethernet frames and so requires root
 *   priviledges; if you install it using "chmod 555" and "chown 
 *   root:root" then you must login as root to run it; otherwise, you 
 *   can install it using "chmod 4555" and "chown root:root" so that 
 *   anyone can run it; the program will refuse to run until you get
 *   thing right;
 *
 *.  Intellon INT6000 Linux Toolkit for HomePlug AV;
 *:  Published 2006-2009 by Intellon Corp. ALL RIGHTS RESERVED;
 *;  For demonstration; Not for production use;
 *
 *   Contributor(s):
 *	C. Maier, charles.maier@intellon.com
 *
 *--------------------------------------------------------------------*/

#define _GETOPT_H

/*====================================================================*
 *   system header files;
 *--------------------------------------------------------------------*/

#include <fcntl.h>
#include <stdio.h>
#include <ctype.h>
#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#ifdef __linux__
#include <net/if.h>
#include <net/if_arp.h>
#include <netpacket/packet.h>
#include <signal.h>
#endif

/*====================================================================*
 *   custom header files;
 *--------------------------------------------------------------------*/

#include "../tools/getoptv.h"
#include "../tools/putoptv.h"
#include "../tools/memory.h"
#include "../tools/number.h"
#include "../tools/types.h"
#include "../tools/flags.h"
#include "../tools/error.h"
#include "../ether/channel.h"
#include "../int6k/intellon.h"
#include "../int6k/homeplug.h"
#include "../int6k/message.h"
#include "../int6k/problem.h"
#include "../mme/mme.h"

/*====================================================================*
 *   custom source files;
 *--------------------------------------------------------------------*/

#ifndef MAKEFILE
#include "../tools/getoptv.c"
#include "../tools/putoptv.c"
#include "../tools/version.c"
#include "../tools/hexdump.c"
#include "../tools/hexdecode.c"
#include "../tools/hexstring.c"
#include "../tools/error.c"
#endif

#ifndef MAKEFILE
#include "../ether/channel.c"
#include "../ether/openchannel.c"
#include "../ether/closechannel.c"
#include "../ether/sendpacket.c"
#include "../ether/readpacket.c"
#endif

#ifndef MAKEFILE
#include "../mme/MMEPeek.c"
#include "../mme/MMEName.c"
#include "../mme/MMEMode.c"
#endif

/*====================================================================*
 *   program constants;
 *--------------------------------------------------------------------*/

#define PLCDEVICE "PLC"

#define HPAV_VERBOSE (1 << 0)
#define HPAV_SILENCE (1 << 1)

/*====================================================================*
 *   
 *   void analyse (void * memory, signed length, flag_t flags);
 *   
 *.  Intellon INT6000 Linux Toolkit for HomePlug AV;
 *:  Published 2006-2009 by Intellon Corp. ALL RIGHTS RESERVED;
 *;  For demonstration; Not for production use;
 *
 *--------------------------------------------------------------------*/

static void function (struct channel * channel, flag_t flags) 

{
	struct message message;
	signed length;
	while ((length = readpacket (channel, &message, sizeof (message))) >= 0) 
	{
		if (!length) 
		{
			continue;
		}
		if (_allclr (flags, HPAV_SILENCE)) 
		{
			MMEPeek (&message, length, stdout);
		}
		if (_anyset (flags, HPAV_VERBOSE)) 
		{
			hexdump (&message, 0, length, stdout);
		}
	}
	return;
}

/*====================================================================*
 *   
 *   int main (int argc, char * argv[]);
 *   
 *.  Intellon INT6000 Linux Toolkit for HomePlug AV;
 *:  Published 2006-2009 by Intellon Corp. ALL RIGHTS RESERVED;
 *;  For demonstration; Not for production use;
 *
 *--------------------------------------------------------------------*/

int main (int argc, const char * argv []) 

{
	extern struct channel channel;
	static const char *optv [] = 
	{
		"i:qv",
		PUTOPTV_S_DIVINE,
		"HomePlug AV Packet Listener",

#if defined (WINPCAP) || defined (LIBPCAP)

		"i s\tuse host interface s [" CHANNEL_ETHDEVICE "]",

#else

		"i n\tuse host interface n [2]",

#endif

		"q\tsuppress normal output",
		"v\tverbose messages on stdout",
		(const char *) (0) 
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
	optmin = 0;
	while ((c = getoptv (argc, argv, optv)) != -1) 
	{
		switch ((char) (c)) 
		{
		case 'i':

#if defined (WIN32)

			channel.id = atoi (optarg);

#else

			channel.name = optarg;

#endif

			break;
		case 'q':
			_setbits (flags, HPAV_SILENCE);
			break;
		case 'v':
			_setbits (flags, HPAV_VERBOSE);
			break;
		default:
			break;
		}
	}
	argc -= optind;
	argv += optind;

#ifndef WIN32

	if (geteuid ()) 
	{
		error (1, EPERM, INT6K_NEEDROOT);
	}

#endif

	openchannel (&channel);
	function (&channel, flags);
	closechannel (&channel);
	return (0);
}

