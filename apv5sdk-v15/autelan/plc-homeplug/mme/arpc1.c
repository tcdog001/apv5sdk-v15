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
#include "../plc/plc.h"
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
 *   void hexwrite1 (signed fd, const void * memory, size_t extent);
 *
 *   memory.h
 *
 *   write a memory region as hexadecimal character stream of space
 *   delimited octets; terminate the stream with a newline; this is
 *   used for logging unwanted or unknown Ethernet frames for later
 *   analysis;
 *   
 *.  Qualcomm Atheros HomePlug AV Powerline Toolkit
 *:  Published 2009-2011 by Qualcomm Atheros. ALL RIGHTS RESERVED
 *;  For demonstration and evaluation only. Not for production use
 *
 *--------------------------------------------------------------------*/

void hexwrite1 (signed fd, const void * memory, size_t extent) 

{
	uint8_t * offset = (uint8_t *)(memory);
	while (extent--) 
	{
		uint8_t byte = * offset;
		write (fd, &DIGITS_HEX [(byte >> 4) & 0x0F], sizeof (char));
		write (fd, &DIGITS_HEX [(byte >> 0) & 0x0F], sizeof (char));
		offset++;
	}
	write (fd, "\n", sizeof (char));
	return;
}


/*====================================================================*
 *
 *   unsigned ARPCPrint1 (FILE * fp, void const * memory, size_t extent);
 *
 *   print formatted VS_ARPC payload on the specified output stream;
 *   return the value of the first character of the format string as
 *   the function value;
 *
 *
 *.  Qualcomm Atheros HomePlug AV Powerline Toolkit
 *:  Published 2009-2011 by Qualcomm Atheros. ALL RIGHTS RESERVED
 *;  For demonstration and evaluation only. Not for production use
 *
 *--------------------------------------------------------------------*/

unsigned ARPCPrint1 (FILE * fp, void const * memory, size_t extent) 

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
 *   capture and record VS_ARPC messages from any Atheros powerline 
 *   device; devices must be programmed to send these messages; the
 *   first payload character determines message disposition; 
 *
 *   this function has no practical use under operational conditions
 *   because of the frame logging overhead; see manjunath krishnam
 *   about the details; it's all his fault;
 *
 *.  Qualcomm Atheros HomePlug AV Powerline Toolkit
 *:  Published 2009-2011 by Qualcomm Atheros. ALL RIGHTS RESERVED
 *;  For demonstration and evaluation only. Not for production use
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
				hexwrite1 (fd1, &message, length);
			}
			continue;
		}
		if (message.qualcomm.MMV == HOMEPLUG_MMV) 
		{
			if (message.qualcomm.MMTYPE == HTOLE16 (VS_SNIFFER | MMTYPE_IND)) 
			{
				if (fd3 > 0) 
				{
					hexwrite1 (fd3, &message, length);
				}
				continue;
			}
			if (fd1 > 0) 
			{
				hexwrite1 (fd1, &message, length);
			}
			continue;
		}
		if (message.qualcomm.MMV != 0) 
		{
			if (fd1 > 0) 
			{
				hexwrite1 (fd1, &message, length);
			}
			continue;
		}
		if (message.qualcomm.MMTYPE == HTOLE16 (VS_SNIFFER | MMTYPE_IND)) 
		{
			if (fd3 > 0) 
			{
				hexwrite1 (fd3, &message, length);
			}
			continue;
		}
		if (message.qualcomm.MMTYPE != HTOLE16 (VS_ARPC | MMTYPE_IND)) 
		{
			if (fd1 > 0) 
			{
				hexwrite1 (fd1, &message, length);
			}
			continue;
		}
		state = ARPCPrint1 (stdout, &indicate->RDATA [indicate->RDATAOFFSET], LE16TOH (indicate->RDATALENGTH));
		if (state == ARPC_ASCII_SOME) 
		{
			static unsigned count = 0;
			if ((count++ % period)) 
			{
				continue;
			}
			ARPCPrint1 (stderr, &indicate->RDATA [indicate->RDATAOFFSET], LE16TOH (indicate->RDATALENGTH));
			continue;
		}
		if (state == ARPC_ASCII_EACH) 
		{
			ARPCPrint1 (stderr, &indicate->RDATA [indicate->RDATAOFFSET], LE16TOH (indicate->RDATALENGTH));
			continue;
		}
		if (state == ARPC_ASCII_EXIT) 
		{
			ARPCPrint1 (stderr, &indicate->RDATA [indicate->RDATAOFFSET], LE16TOH (indicate->RDATALENGTH));
			break;
		}
		ARPCPrint1 (stderr, &indicate->RDATA [indicate->RDATAOFFSET], LE16TOH (indicate->RDATALENGTH));
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

#define DEFAULT_COUNT 10

int main (int argc, char const * argv []) 

{
	extern struct channel channel;
	static char const *optv [] = 
	{
		"a:b:c:i:qt:u:v",
		PUTOPTV_S_DIVINE,
		"Atheros Asynchronous Remote Procedure Call Logger",
		"a s\tlog intellon sniffer frames to file (s)",
		"b s\tlog homeplug channel estimation frames to file (s)",
		"c n\tprint every (n) messages [" OPTSTR (DEFAULT_COUNT) "]",

#if defined (WINPCAP) || defined (LIBPCAP)

		"i s\thost interface is (s) [" CHANNEL_ETHDEVICE "]",

#else

		"i n\thost interface is (n) [" OPTSTR (CHANNEL_ETHNUMBER) "]",

#endif

		"q\tsuppress output on stderr",
		"t n\tread timeout is (n) milliseconds [" OPTSTR (CHANNEL_TIMEOUT) "]",
		"u s\tlog unwanted frames to file (s)",
		"v\tverbose messages on stdout",
		(char const *) (0)
	};
	unsigned count = DEFAULT_COUNT;
	signed fd1 = -1;
	signed fd2 = -1;
	signed fd3 = -1;
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

			channel.ifindex = atoi (optarg);

#else

			channel.ifname = optarg;

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
		error (1, EPERM, ERROR_NOTROOT);
	}

#endif

	openchannel (&channel);
	function (&channel, count, fd1, fd2, fd3);
	closechannel (&channel);
	return (0);
}

