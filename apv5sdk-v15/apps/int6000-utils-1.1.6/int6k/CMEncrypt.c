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
 *   CMEncrypt - Send Encrypted Payload;
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
#include <stdint.h>
#include <time.h>

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
#include "../crypt/SHA256.h"

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
#include "../tools/hexdump.c"
#include "../tools/todigit.c"
#include "../tools/error.c"
#include "../tools/synonym.c"
#endif

#ifndef MAKEFILE
#include "../int6k/Confirm.c"    
#include "../int6k/Failure.c"    
#include "../int6k/Request.c"    
#include "../int6k/devices.c"
#include "../int6k/EthernetHeader.c"
#include "../int6k/StandardHeader.c"
#endif

#ifndef MAKEFILE
#include "../ether/channel.c"
#include "../ether/openchannel.c"
#include "../ether/closechannel.c"
#include "../ether/readpacket.c"
#include "../ether/sendpacket.c"
#endif

#ifndef MAKEFILE
#include "../crypt/SHA256Reset.c"
#include "../crypt/SHA256Write.c"
#include "../crypt/SHA256Block.c"
#include "../crypt/SHA256Fetch.c"
#endif

#ifndef MAKEFILE
#include "../mme/MMECode.c"
#endif

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
		"A:f:i:K:P:qv",
		"device [device] [...]",
		"Send Encrypted Payload using CM_ENCRYPTED_PAYLOAD",
		"A n\tAVLN Status [0x00]",
		"K n\tPayload Encryption Key Select (PEKS) [0x0f]",
		"P n\tProtocol Identifier (PID) [0x04]",

#if defined (WINPCAP) || defined (LIBPCAP)

		"i n\thost interface number [2]",

#else

		"i s\thost interface name [" CHANNEL_ETHDEVICE "]",

#endif

		"f f\tpayload file",
		"q\tquiet mode",
		"v\tverbose mode",
		(const char *) (0) 
	};

#ifndef __GNUC__
#pragma pack(push,1)
#endif

	struct __packed cm_encrypted_payload 
	{
		uint8_t PEKS;
		uint8_t AVLN;
		uint8_t PID;
		uint16_t PRN;
		uint8_t PMN;
		uint8_t UUID [16];
		uint16_t LEN;
	}
	template = 
	{
		0x0F,
		0x00,
		0x04,
		0x0000,
		0x00,
		{
			0x00,
			0x00,
			0x00,
			0x00,
			0x00,
			0x00,
			0x00,
			0x00,
			0x00,
			0x00,
			0x00,
			0x00,
			0x00,
			0x00,
			0x00,
			0x00 
		},
		0x0000 
	};

#ifndef __GNUC__
#pragma pack (pop)
#endif

	struct _file_ file = 
	{
		-1,
		(const char *) (0) 
	};
	struct sha256 sha256;
	byte digest [SHA256_DIGEST_LENGTH];
	time_t timer = time ((time_t *)(0));
	byte packet [ETHER_MAX_LEN];
	byte * buffer;
	signed extent;
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
		case 'f':
			if ((file.file = open (optarg, O_BINARY|O_RDONLY)) == -1) 
			{
				error (1, errno, "%s", optarg);
			}
			file.name = optarg;
			break;
		case 'P':
			template.PID = (byte)(uintspec (optarg, 0x00, 0x04));
			break;
		case 'A':
			template.AVLN = (byte)(uintspec (optarg, 0x00, 0x08));
			break;
		case 'K':
			template.PEKS = (byte)(uintspec (optarg, 0x00, 0x0F));
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
		error (1, EPERM, "Program needs root priviledges");
	}

#endif

/*
 *	load entire file into memory;
 */

	if (file.file == -1) 
	{
		error (1, ECANCELED, "No payload file given: Use -f <file>");
	}
	if ((extent = lseek (file.file, 0, SEEK_END)) == -1) 
	{
		error (1, errno, "Can't size %s", file.name);
	}
	if (!(buffer = malloc (extent))) 
	{
		error (1, errno, "Can't hold %s", file.name);
	}
	if (lseek (file.file, 0, SEEK_SET)) 
	{
		error (1, errno, "Can't home %s", file.name);
	}
	if (read (file.file, buffer, extent) < extent) 
	{
		error (1, errno, "Can't load %s", file.name);
	}
	close (file.file);
	SHA256Reset (&sha256);
	SHA256Write (&sha256, buffer, extent);
	SHA256Fetch (&sha256, digest);
	if (!argc) 
	{
		error (1, ECANCELED, "No destination given");
	}
	openchannel (&channel);
	while ((argc) && (*argv)) 
	{
		signed offset = 0;
		signed remain = extent;

#if 0

		signed length = sizeof (struct packet_ms) - sizeof (template);

#else   

		signed length = 502 - sizeof (template);

#endif

		if (!hexencode (channel.peer, sizeof (channel.peer), synonym (*argv, devices, DEVICES))) 
		{
			error (1, errno, INT6K_BAD_MAC, *argv);
		}
		template.PRN = (uint16_t)(timer);
		template.PMN = 0;
		memcpy (template.UUID, digest, sizeof (template.UUID));
		while (remain) 
		{
			byte * memory = packet;
			if (length > remain) 
			{
				length = remain;
			}
			template.PMN++;
			template.LEN = HTOLE16 (length);
			memset (memory, 0, sizeof (struct message));
			memory += EthernetHeader ((struct header_eth *)(memory), channel.peer, channel.host);
			memory += StandardHeader ((struct header_std *)(memory), CM_ENCRYPTED_PAYLOAD | MMTYPE_IND);
			memcpy (memory, &template, sizeof (template));
			memory += sizeof (template);
			memcpy (memory, buffer + offset, length);
			memory += length;
			extent = (signed)(memory - packet);
			if (extent < ETHER_MIN_LEN) 
			{
				extent = ETHER_MIN_LEN;
			}
			if (sendpacket (&channel, packet, extent) < extent) 
			{
				error (1, errno, "Can't send packet");
			}
			offset += length;
			remain -= length;
		}
		argc--;
		argv++;
	}
	closechannel (&channel);
	free (buffer);
	exit (0);
}

