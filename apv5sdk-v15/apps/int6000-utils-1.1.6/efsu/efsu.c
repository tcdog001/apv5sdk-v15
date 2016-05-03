/*====================================================================*
 *
 *   efsu.c - Ethernet Frame Send Utility;
 *
 *   convert hexadecimal text files to ethernet frames and transmit
 *   them over the network; basically, it is a 'send your own frame'
 *   utility; 
 *
 *   the program works like cat, sending file after file to a given
 *   interface; as each file is read, all hexadecimal octets in the
 *   file are converted to bytes and buffered; a semicolon causes a
 *   buffer transmit as does the end of file; script-style comments
 *   starting with hash (#) and c-language-style comments starting
 *   with slash-slash or slash-asterisk are consumed and discard as
 *   the file is read; the errors that can occur are non-hex digits
 *   and odd number of hex digits;
 *
 *.  Intellon INT6000 Linux Toolkit for HomePlug AV;
 *:  Published 2006-2009 by Intellon Corp. ALL RIGHTS RESERVED;
 *;  For demonstration; Not for production use;
 *
 *--------------------------------------------------------------------*/

#define _GETOPT_H

/*====================================================================*
 *   system header files;
 *--------------------------------------------------------------------*/

#include <stdlib.h>
#include <string.h>
#include <limits.h>

/*====================================================================*
 *   custom header files;
 *--------------------------------------------------------------------*/

#include "../tools/getoptv.h"
#include "../tools/putoptv.h"
#include "../tools/error.h"
#include "../tools/memory.h"
#include "../tools/number.h"
#include "../tools/symbol.h"
#include "../tools/flags.h"
#include "../ether/channel.h"

/*====================================================================*
 *   custom source files;
 *--------------------------------------------------------------------*/

#ifndef MAKEFILE
#include "../tools/getoptv.c"
#include "../tools/putoptv.c"
#include "../tools/version.c"
#include "../tools/error.c"
#include "../tools/hexencode.c"
#include "../tools/hexdump.c"
#include "../tools/hexin.c"
#include "../tools/todigit.c"
#include "../tools/uintspec.c"
#include "../tools/basespec.c"
#include "../tools/synonym.c"
#endif

#ifndef MAKEFILE
#include "../ether/openchannel.c"
#include "../ether/closechannel.c"
#include "../ether/readpacket.c"
#include "../ether/sendpacket.c"
#include "../ether/channel.c"
#endif

/*====================================================================*
 *   program contants;
 *--------------------------------------------------------------------*/

#define PLCDEVICE "PLC"
#define PROTOCOLS 2

#ifndef ETHER_CRC_LEN
#define ETHER_CRC_LEN 4
#endif

/*====================================================================*
 *   program variables;
 *--------------------------------------------------------------------*/

static const struct _term_ protocols [PROTOCOLS] =
{
	{
		"hp10",
		"887B"
	},
	{
		"hpav",
		"88E1"
	}
};

/*====================================================================*
 *   
 *   void function (struct channel * channel, void * memory, size_t extent);
 *
 *   read Ethernet frame descriptions from stdin and transmit as raw 
 *   ethernet frames; optionally, wait for a response before return;
 *
 *.  Intellon INT6000 Linux Toolkit for HomePlug AV;
 *:  Published 2006-2009 by Intellon Corp. ALL RIGHTS RESERVED;
 *;  For demonstration; Not for production use;
 *
 *--------------------------------------------------------------------*/

static void function (struct channel * channel, void * memory, size_t extent) 

{
	struct ether_header * frame = (struct ether_header *)(memory);
	size_t length;
	while ((length = hexin (memory, extent, stdin)) > 0) 
	{
		if (length < (ETHER_MIN_LEN - ETHER_CRC_LEN)) 
		{
			error (1, EINVAL, "frame size of %d is less than %d bytes", length, (ETHER_MIN_LEN - ETHER_CRC_LEN));
		}
		if (length > (ETHER_MAX_LEN - ETHER_CRC_LEN)) 
		{
			error (1, EINVAL, "frame size of %d is more than %d bytes", length, (ETHER_MAX_LEN - ETHER_CRC_LEN));
		}
		if (_anyset (channel->flags, CHANNEL_UPDATE_TARGET)) 
		{
			memcpy (frame->ether_dhost, channel->peer, sizeof (frame->ether_dhost));
		}
		if (_anyset (channel->flags, CHANNEL_UPDATE_SOURCE)) 
		{
			memcpy (frame->ether_shost, channel->host, sizeof (frame->ether_shost));
		}
		sendpacket (channel, memory, (signed)(length));
		if (_anyset (channel->flags, CHANNEL_LISTEN)) 
		{
			while (readpacket (channel, memory, (signed)(extent)) > 0);
		}
	}
	return;
}

/*====================================================================*
 *   
 *   int main (int argc, const char * argv []);
 *
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
		"d:e:hi:l:p:t:vw:",
		PUTOPTV_S_FUNNEL,
		"Ethernet Frame Send Utility",
		"d x\treplace destination address with x",
		"e x\techo return frames having ethertype x [88E1]",
		"h\treplace source address with host address",

#if defined (WINPCAP) || defined (LIBPCAP)

		"i n\tuse host interface n [2]",

#else

		"i s\tuse host interface s [" CHANNEL_ETHDEVICE "]",

#endif

		"l n\trepeat file sequence n times [1]",
		"p n\twait n seconds between files [0]",

#if defined (WINPCAP) || defined (LIBPCAP)

		"t n\tread capture time is (n) milliseconds [50]",

#else

		"t n\tread timeout is (n) milliseconds [50]",

#endif

		"v\tverbose messages",
		"w n\twait n seconds between loops [0]",
		(const char *) (0) 
	};
	byte buffer [ETHER_MAX_LEN];
	unsigned pause = 0;
	unsigned delay = 0;
	unsigned loop = 1;
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
		switch (c) 
		{
		case 'd':
			_setbits (channel.flags, CHANNEL_UPDATE_TARGET);
			if (hexencode (channel.peer, sizeof (channel.peer), optarg) == -1) 
			{
				error (1, errno, "%s", optarg);
			}
			break;
		case 'e':
			_setbits (channel.flags, CHANNEL_LISTEN);
			channel.type = (uint16_t)(basespec (synonym (optarg, protocols, PROTOCOLS), 16, sizeof (channel.type)));
			channel.type = channel.type;
			break;
		case 'h':
			_setbits (channel.flags, CHANNEL_UPDATE_SOURCE);
			break;
		case 'i':

#if defined (WINPCAP) || defined (LIBPCAP)

			channel.id = atoi (optarg);

#else

			channel.name = optarg;

#endif

			break;
		case 'l':
			loop = (unsigned)(uintspec (optarg, 0, UINT_MAX));
			break;
		case 'p':
			pause = (unsigned)(uintspec (optarg, 0, 1200));
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
		case 'w':
			delay = (unsigned)(uintspec (optarg, 0, 1200));
			break;
		default:
			break;
		}
	}
	argc -= optind;
	argv += optind;
	openchannel (&channel);
	if (!argc) 
	{
		function (&channel, buffer, sizeof (buffer));
		loop = 0;
	}
	while (loop--) 
	{
		int argn;
		for (argn = 0; argn < argc; argn++) 
		{
			if (!freopen (argv [argn], "r", stdin)) 
			{
				error (1, errno, "Can't open %s", argv [argn]);
			}
			function (&channel, buffer, sizeof (buffer));
			if (argn < argc) 
			{
				sleep (pause);
			}
		}
		if (loop) 
		{
			sleep (delay);
		}
	}
	closechannel (&channel);
	return (0);
}

