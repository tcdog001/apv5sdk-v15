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

/*====================================================================*
 *   system header files;
 *--------------------------------------------------------------------*/

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <sys/signal.h>
#include <signal.h>

/*====================================================================*
 *   custom header files;
 *--------------------------------------------------------------------*/

#include "../tools/getoptv.h"
#include "../tools/putoptv.h"
#include "../tools/memory.h"
#include "../tools/number.h"
#include "../tools/types.h"
#include "../tools/flags.h"
#include "../tools/files.h"
#include "../tools/error.h"
#include "../ether/channel.h"
#include "../plc/plc.h"
#include "../mme/mme.h"

/*====================================================================*
 *   custom source files;
 *--------------------------------------------------------------------*/

#ifndef MAKEFILE
#include "../tools/getoptv.c"
#include "../tools/putoptv.c"
#include "../tools/version.c"
#include "../tools/hexdump.c"
#include "../tools/uintspec.c"
#include "../tools/todigit.c"
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
#include "../mme/ARPCPrint.c"
#endif

/*====================================================================*
 *    program constants; 
 *--------------------------------------------------------------------*/

#define ARPCD_DAEMON (1 << 0)
#define ARPCD_VERBOSE (1 << 1)
#define ARPCD_SILENCE (1 << 2)
#define ARPCD_ERROR (1 << 3)

#define PLCDEVICE "PLC"
#define ETHDEVICE "eth1"
#define ETHNUMBER 2

/*====================================================================*
 *   program variables;
 *--------------------------------------------------------------------*/

static bool loop = true;

/*====================================================================*
 *   
 *   void terminate (signo_t signal);
 *   
 *   terminate the program; we want to ensure an orgaized program 
 *   exit so that the original Ethernet adapter state is restored;
 *   
 *.  Qualcomm Atheros HomePlug AV Powerline Toolkit
 *:  Published 2009-2011 by Qualcomm Atheros. ALL RIGHTS RESERVED
 *;  For demonstration and evaluation only. Not for production use
 *
 *--------------------------------------------------------------------*/

void terminate (signo_t signal) 

{
	loop = false;
	return;
}


/*====================================================================*
 *   
 *   int main (int argc, char * argv[]);
 *   
 *.  Qualcomm Atheros HomePlug AV Powerline Toolkit
 *:  Published 2009-2011 by Qualcomm Atheros. ALL RIGHTS RESERVED
 *;  For demonstration and evaluation only. Not for production use
 *
 *--------------------------------------------------------------------*/

int main (int argc, char const * argv []) 

{
	extern struct channel channel;
	static char const *optv [] = 
	{
		"di:qt:v",
		PUTOPTV_S_DIVINE,
		"Qualcomm Atheros Asynchronous Remote Procedure Call Daemon",
		"d\trun in background as daemon",
		"i n\thost interface is (n) [" ETHDEVICE "]",
		"q\tsuppress output on stderr",
		"t n\tread timeout is (n) milliseconds [" OPTSTR (CHANNEL_TIMEOUT) "]",
		"v\tverbose messages on stdout",
		(char const *) (0)
	};
	struct message message;
	struct sigaction sa;

#ifndef __GNUC__
#pragma pack (push,1)
#endif

	struct __packed vs_arpc_ind 
	{
		struct ethernet_std ethernet;
		struct qualcomm_std qualcomm;
		uint16_t RDATALENGTH;
		uint8_t RDATAOFFSET;
		uint8_t RDATA [1];
	}
	* indicate = (struct vs_arpc_ind *)(&message);

#ifndef __GNUC__
#pragma pack (pop)
#endif

	flag_t flags;
	signed c;
	if (getenv (PLCDEVICE)) 
	{
		channel.ifname = strdup (getenv (PLCDEVICE));
	}
	optind = 1;
	while ((c = getoptv (argc, argv, optv)) != -1) 
	{
		switch ((char) (c)) 
		{
		case 'd':
			_setbits (flags, ARPCD_DAEMON);
			break;
		case 'i':
			channel.ifname = optarg;
			break;
		case 'q':
			_setbits (channel.flags, CHANNEL_SILENCE);
			break;
		case 't':
			channel.timeout = (unsigned)(uintspec (optarg, 0, UINT_MAX));
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
	if (argc) 
	{
		error (1, ENOTSUP, ERROR_TOOMANY);
	}
	if (geteuid ()) 
	{
		error (1, EPERM, ERROR_NOTROOT);
	}
	if (_anyset (flags, ARPCD_DAEMON)) 
	{
		pid_t pid = fork ();
		if (pid < 0) 
		{
			error (1, errno, "razzlefrats!");
		}
		if (pid > 0) 
		{
			exit (0);
		}
	}
	memset (&sa, 0, sizeof (struct sigaction));
	sa.sa_handler = terminate;
	sigaction (SIGTERM, &sa, (struct sigaction *)(0));
	sigaction (SIGQUIT, &sa, (struct sigaction *)(0));
	sigaction (SIGTSTP, &sa, (struct sigaction *)(0));
	sigaction (SIGINT, &sa, (struct sigaction *)(0));
	sigaction (SIGHUP, &sa, (struct sigaction *)(0));
	openchannel (&channel);
	while (loop) 
	{
		signed length = readpacket (&channel, &message, sizeof (message));
		if (length < 0) 
		{
			continue;
		}
		if (length > 0) 
		{
			if (ntohs (message.ethernet.MTYPE) != HOMEPLUG_MTYPE) 
			{
				continue;
			}
			if (message.qualcomm.MMV != 0) 
			{
				continue;
			}
			if (LE16TOH (message.qualcomm.MMTYPE) != (VS_ARPC | MMTYPE_IND)) 
			{
				continue;
			}
			ARPCPrint (stdout, &indicate->RDATA [indicate->RDATAOFFSET], LE16TOH (indicate->RDATALENGTH));
		}
	}
	closechannel (&channel);
	return (0);
}

