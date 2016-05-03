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
 *   arpc.c - Intellon ARPC Logger;
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
#include "../int6k/intellon.h"
#include "../int6k/homeplug.h"
#include "../int6k/message.h"
#include "../int6k/problem.h"
#include "../ether/channel.h"
#include "../mme/mme.h"

/*====================================================================*
 *   custom source files;
 *--------------------------------------------------------------------*/

#ifndef MAKEFILE
#include "../tools/getoptv.c"
#include "../tools/putoptv.c"
#include "../tools/version.c"
#include "../tools/hexdump.c"
#include "../tools/hexwrite.c"
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

/*====================================================================*
 *    program constants; 
 *--------------------------------------------------------------------*/

#define PLCDEVICE "PLC"
#define ARPC_ASCII_EACH '2'
#define ARPC_ASCII_SOME '1'
#define ARPC_ASCII_EXIT '0'

/*====================================================================*
 *
 *   unsigned ARPCLog (void const * memory, size_t extent, FILE *fp);
 *
 *   print formatted VS_ARPC payload on the specified output stream;
 *
 *.  Intellon INT6000 Linux Toolkit for HomePlug AV;
 *:  Published 2006-2009 by Intellon Corp. ALL RIGHTS RESERVED;
 *;  For demonstration; Not for production use;
 *
 *--------------------------------------------------------------------*/

unsigned ARPCLog (void const * memory, size_t extent, FILE *fp) 

{

#ifndef __GNUC__
#pragma pack (push,1)
#endif

	struct __packed vs_arpc_data 
	{
		uint32_t BYPASS;
		uint16_t ARPCID;
		uint16_t DATALENGTH;
		uint8_t DATAOFFSET;
		uint8_t RESERVED [3];
		uint16_t ARGOFFSET;
		uint16_t STROFFSET;
		uint16_t ARGLENGTH;
		uint16_t STRLENGTH;
		uint8_t LIST [1];
	}
	* data = (struct vs_arpc_data *)(memory);

#ifndef __GNUC__
#pragma pack (pop)
#endif

#if 0

	fprintf (fp, "BYPASS=%d ", data->BYPASS);
	fprintf (fp, "ARPCID=%d ", data->ARPCID);
	fprintf (fp, "DATALENGTH=%d ", LE16TOH (data->DATALENGTH));
	fprintf (fp, "DATAOFFSET=%d ", data->DATAOFFSET);
	fprintf (fp, "ARGOFFSET=%d ", LE16TOH (data->ARGOFFSET));
	fprintf (fp, "STROFFSET=%d ", LE16TOH (data->STROFFSET));
	fprintf (fp, "ARGLENGTH=%d ", LE16TOH (data->ARGLENGTH));
	fprintf (fp, "STRLENGTH=%d ", LE16TOH (data->STRLENGTH));
	fprintf (fp, "FORMAT=[%s]\n", &data->LIST [data->STROFFSET]);

#endif

	vfprintf (fp, (char *)(&data->LIST [data->STROFFSET]), (void *)(&data->LIST [data->ARGOFFSET]));
	fprintf (fp, "\n");
	return (data->LIST [data->STROFFSET]);
}

/*====================================================================*
 *   
 *   void function (struct channel * channel, unsigned period, signed fo);
 *   
 *   capture and record VS_ARPC messages from any Intellon powerline 
 *   device; devices must be programmed to send these messages;
 *   the first payload character determines message disposition;
 *
 *   this function has no practical use under operation conditions;
 *
 *.  Intellon INT6000 Linux Toolkit for HomePlug AV;
 *:  Published 2006-2009 by Intellon Corp. ALL RIGHTS RESERVED;
 *;  For demonstration; Not for production use;
 *
 *--------------------------------------------------------------------*/

static void function (struct channel * channel, unsigned period, signed fd1, signed fd2, signed fd3) 

{
	struct message message;

#ifndef __GNUC__
#pragma pack (push,1)
#endif

	struct __packed vs_arpc_ind 
	{
		struct header_eth ethernet;
		struct header_int intellon;
		uint16_t RDATALENGTH;
		uint8_t RDATAOFFSET;
		uint8_t RDATA [1];
	}
	* indicate = (struct vs_arpc_ind *)(&message);

#ifndef __GNUC__
#pragma pack (pop)
#endif

	unsigned state = 0;
	signed length;
	while ((length = readpacket (channel, &message, sizeof (message))) >= 0) 
	{
		if (!length) 
		{
			continue;
		}
		if (message.ethernet.MTYPE != htons (HOMEPLUG_MTYPE)) 
		{
			if (fd1 > 0) 
			{
				hexwrite (fd1, &message, length);
			}
			continue;
		}
		if (message.intellon.MMV == HOMEPLUG_MMV) 
		{
			if (message.intellon.MMTYPE == HTOLE16 (VS_SNIFFER | MMTYPE_IND)) 
			{
				if (fd3 > 0) 
				{
					hexwrite (fd3, &message, length);
				}
				continue;
			}
			if (fd1 > 0) 
			{
				hexwrite (fd1, &message, length);
			}
			continue;
		}
		if (message.intellon.MMV != INTELLON_MMV) 
		{
			if (fd1 > 0) 
			{
				hexwrite (fd1, &message, length);
			}
			continue;
		}
		if (message.intellon.MMTYPE == HTOLE16 (VS_SNIFFER | MMTYPE_IND)) 
		{
			if (fd3 > 0) 
			{
				hexwrite (fd3, &message, length);
			}
			continue;
		}
		if (message.intellon.MMTYPE != HTOLE16 (VS_ARPC | MMTYPE_IND)) 
		{
			if (fd1 > 0) 
			{
				hexwrite (fd1, &message, length);
			}
			continue;
		}
		state = ARPCLog (&indicate->RDATA [indicate->RDATAOFFSET], indicate->RDATALENGTH, stdout);
		if (state == ARPC_ASCII_SOME) 
		{
			static unsigned count = 0;
			if ((count++ % period)) 
			{
				continue;
			}
			ARPCLog (&indicate->RDATA [indicate->RDATAOFFSET], indicate->RDATALENGTH, stderr);
			continue;
		}
		if (state == ARPC_ASCII_EACH) 
		{
			ARPCLog (&indicate->RDATA [indicate->RDATAOFFSET], indicate->RDATALENGTH, stderr);
			continue;
		}
		if (state == ARPC_ASCII_EXIT) 
		{
			ARPCLog (&indicate->RDATA [indicate->RDATAOFFSET], indicate->RDATALENGTH, stderr);
			break;
		}
		ARPCLog (&indicate->RDATA [indicate->RDATAOFFSET], indicate->RDATALENGTH, stderr);
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
		"a:b:c:i:qu:v",
		PUTOPTV_S_DIVINE,
		"Intellon ARPC Logger",
		"a s\tlog intellon sniffer frames to file (s)",
		"b s\tlog homeplug channel estimation frames to file (s)",
		"c n\tprint every (n) messages [10]",

#if defined (WINPCAP) || defined (LIBPCAP)

		"i s\tuse host interface s [" CHANNEL_ETHDEVICE "]",

#else

		"i n\tuse host interface n [2]",

#endif

		"q\tsuppress output on stderr",
		"t n\ttimeout is (n) milliseconds [50]",
		"u s\tlog unwanted frames to file (s)",
		"v\tverbose messages on stdout",
		(const char *) (0) 
	};
	unsigned count = 10;
	signed fd1 = -1;
	signed fd2 = -1;
	signed fd3 = -1;
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
		case 'a':
			if ((fd3 = open (optarg, O_BINARY|O_CREAT|O_WRONLY|O_TRUNC, FILE_FILEMODE)) == -1) 
			{
				error (1, errno, "%s", optarg);
			}
			break;
		case 'b':
			if ((fd2 = open (optarg, O_BINARY|O_CREAT|O_WRONLY|O_TRUNC, FILE_FILEMODE)) == -1) 
			{
				error (1, errno, "%s", optarg);
			}
			break;
		case 'c':
			count = (unsigned)(uintspec (optarg, 1, 1024));
			break;
		case 't':
			channel.timeout = (unsigned)(uintspec (optarg, 0, UINT_MAX));
			break;
		case 'u':
			if ((fd1 = open (optarg, O_BINARY|O_CREAT|O_WRONLY|O_TRUNC, FILE_FILEMODE)) == -1) 
			{
				error (1, errno, "%s", optarg);
			}
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
 *      root when this program is installed setuid using 'chown root:root' and 
 *      'chmod 4555';
 */

	if (geteuid ()) 
	{
		error (1, EPERM, INT6K_NEEDROOT);
	}

#endif

	openchannel (&channel);
	function (&channel, count, fd1, fd2, fd3);
	closechannel (&channel);
	return (0);
}

