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

#define PLCDEVICE "PLC"

/*====================================================================*
 *   
 *   void function (struct channel * channel);
 *   
 *   capture and record VS_ARPC messages from any Atheros powerline 
 *   device; devices must be programmed to send these messages;
 *
 *   this function has no practical use under operation conditions;
 *
 *.  Qualcomm Atheros HomePlug AV Powerline Toolkit
 *:  Published 2009-2011 by Qualcomm Atheros. ALL RIGHTS RESERVED
 *;  For demonstration and evaluation only. Not for production use
 *
 *--------------------------------------------------------------------*/

static void function (struct channel * channel) 

{
	struct message message;

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

	signed length;
	while ((length = readpacket (channel, &message, sizeof (message))) >= 0) 
	{
		if (!length) 
		{
			continue;
		}
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
		"i:qt:v",
		PUTOPTV_S_DIVINE,
		"Qualcomm Atheros Asynchronous Remote Procedure Call Monitor",

#if defined (WINPCAP) || defined (LIBPCAP)

		"i s\thost interface is (s) [" CHANNEL_ETHDEVICE "]",

#else

		"i n\thost interface is (n) [" OPTSTR (CHANNEL_ETHNUMBER) "]",

#endif

		"q\tsuppress output on stderr",
		"t n\tread timeout is (n) milliseconds [" OPTSTR (CHANNEL_TIMEOUT) "]",
		"v\tverbose messages on stdout",
		(char const *) (0)
	};
	signed c;
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
		switch ((char) (c)) 
		{
		case 'i':

#if defined (WINPCAP) || defined (LIBPCAP)

			channel.ifindex = atoi (optarg);

#else

			channel.ifname = optarg;

#endif

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

#ifndef WIN32

/*
 *      raw packets require root priviledges on linux; one does not have to be
 *      root when this program is installed setuid using 'chown root:root' and 
 *      'chmod 4555';
 */

	if (geteuid ()) 
	{
		error (1, EPERM, ERROR_NOTROOT);
	}

#endif

	openchannel (&channel);
	function (&channel);
	closechannel (&channel);
	return (0);
}

