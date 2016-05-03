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
 *   int6kmod.c - Intellon Module Operation Utility
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
 *.  Intellon INT6000 Linux Toolkit for HomePlug AV;
 *:  Published 2006-2009 by Intellon Corp. ALL RIGHTS RESERVED;
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
#include <limits.h>

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
#include "../int6k/ReadMME.c"
#include "../int6k/SendMME.c"
#include "../int6k/EthernetHeader.c"
#include "../int6k/IntellonHeader.c"
#include "../int6k/UnwantedPacket.c"
#endif

#ifndef MAKEFILE
#include "../tools/alert.c"
#include "../tools/basespec.c"
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
#include "../tools/checksum_32.c"
#include "../tools/fdchecksum_32.c"
#include "../tools/strfbits.c"
#include "../tools/synonym.c"
#include "../tools/filepart.c"
#include "../tools/typename.c"
#include "../tools/markup.c"
#include "../tools/error.c"
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

#define INT6KMOD_OPEN_SESSION   (1 << 0)
#define INT6KMOD_WRITE_MODULE   (1 << 1)
#define INT6KMOD_COMMIT_MODULE  (1 << 2)
#define INT6KMOD_READ_MODULE    (1 << 3)

/*====================================================================*
 *   program variables;
 *--------------------------------------------------------------------*/

#ifndef __GNUC__
#pragma pack (push,1)
#endif

typedef struct __packed vs_module_operation_spec 

{
	uint16_t MODULE_ID;
	uint16_t MODULE_SUB_ID;
	uint32_t MODULE_LENGTH;
	uint32_t MODULE_CHKSUM;
}

vs_module_operation_spec;

#ifndef __GNUC__
#pragma pack (pop)
#endif

/*====================================================================*
 *
 *   signed modulefile (struct _file_ * module, struct vs_module_operation_spec * vs_module_operation_spec);
 *
 *   compute module file length and checksum; return values in struct
 *   vs_module_operation_spec for use by other modules;
 *
 *.  Intellon INT6000 Linux Toolkit for HomePlug AV;
 *:  Published 2006-2009 by Intellon Corp. ALL RIGHTS RESERVED;
 *;  For demonstration; Not for production use;
 *
 *--------------------------------------------------------------------*/

signed modulefile (struct _file_ * module, struct vs_module_operation_spec * vs_module_operation_spec) 

{
	off_t length;
	if ((length = lseek (module->file, 0, SEEK_END)) == -1) 
	{
		error (1, errno, "Can't size %s", filepart (module->name));
	}

#if 1

/*
 *	limit filesize to 1400 bytes to avoid overflowing the message 
 *	frame; this restriction will be removed once the ModuleWrite
 *	function can support multiple frames;
 */

	if (length > 1400) 
	{
		error (1, ENOTSUP, "%s exceeds %d bytes", filepart (module->name), 1400);
	}

#endif

	if (length % sizeof (uint32_t)) 
	{
		error (1, ENOTSUP, "%s not multiple of %u bytes", filepart (module->name), (unsigned)(sizeof (uint32_t)));
	}
	if (lseek (module->file, 0, SEEK_SET) == -1) 
	{
		error (1, errno, "Can't home %s", filepart (module->name));
	}
	vs_module_operation_spec->MODULE_LENGTH = length;
	vs_module_operation_spec->MODULE_CHKSUM = fdchecksum_32 (module->file, length, 0);
	if (lseek (module->file, 0, SEEK_SET) == -1) 
	{
		error (1, errno, "Can't home %s", filepart (module->name));
	}
	return (0);
}

/*====================================================================*
 *
 *   signed ModuleSession (struct int6k * int6k);
 *
 *
 *.  Intellon INT6000 Linux Toolkit for HomePlug AV;
 *:  Published 2006-2009 by Intellon Corp. ALL RIGHTS RESERVED;
 *;  For demonstration; Not for production use;
 *
 *--------------------------------------------------------------------*/

signed ModuleSession (struct int6k * int6k, uint32_t SessionID, struct vs_module_operation_spec * vs_module_operation_spec) 

{

	struct channel * channel = (struct channel *)(int6k->channel);
	struct message * message = (struct message *)(int6k->message);
#ifndef __GNUC__
#pragma pack (push,1)
#endif

	struct __packed vs_module_operation_start_request 
	{
		struct header_eth ethernet;
		struct header_int intellon;
		uint32_t RESERVED1;
		uint8_t NUM_OP_DATA;
		uint16_t MOD_OP;
		uint16_t MOD_OP_DATA_LEN;
		uint32_t MOD_OP_RSVD;
		uint32_t MOD_OP_SESSION_ID;
		uint8_t NUM_MODULES;
		struct vs_module_operation_spec MOD_OP_SPEC [1];
	}
	* request = (struct vs_module_operation_start_request *)(message);
	struct __packed vs_module_operation_start_confirm 
	{
		struct header_eth ethernet;
		struct header_int intellon;
		uint16_t MSTATUS;
		uint16_t ERR_REC_CODE;
		uint32_t RESERVED;
		uint8_t NUM_OP_DATA;
		uint16_t MOD_OP;
		uint16_t MOD_OP_DATA_LEN;
		uint32_t MOD_OP_RSVD;
		uint32_t MOD_OP_SESSION_ID;
		uint8_t NUM_MODULES;
		struct vs_module_operation_data 
		{
			uint16_t MOD_STATUS;
			uint16_t ERR_REC_CODE;
		}
		MOD_OP_DATA [1];
	}
	* confirm = (struct vs_module_operation_start_confirm *)(message);

#ifndef __GNUC__
#pragma pack (pop)
#endif

	unsigned module;
	struct vs_module_operation_spec * spec = (struct vs_module_operation_spec *)(&request->MOD_OP_SPEC);
	Request (int6k, "Request Session");
	memset (message, 0, sizeof (struct message));
	EthernetHeader (&message->ethernet, channel->peer, channel->host);
	IntellonHeader (&message->intellon, (VS_MODULE_OPERATION | MMTYPE_REQ));
	int6k->packetsize = ETHER_MIN_LEN;
	request->NUM_OP_DATA = 1;
	request->MOD_OP = HTOLE16 (0x10);
	request->MOD_OP_DATA_LEN = HTOLE16 (13 + sizeof (struct vs_module_operation_spec));
	request->MOD_OP_SESSION_ID = HTOLE32 (SessionID);
	request->NUM_MODULES = 1;
	for (module = 0; module < request->NUM_MODULES; module++) 
	{
		spec->MODULE_ID = HTOLE16 (vs_module_operation_spec->MODULE_ID);
		spec->MODULE_SUB_ID = HTOLE16 (vs_module_operation_spec->MODULE_SUB_ID);
		spec->MODULE_LENGTH = HTOLE32 (vs_module_operation_spec->MODULE_LENGTH);
		spec->MODULE_CHKSUM = vs_module_operation_spec->MODULE_CHKSUM;
	}
	if (SendMME (int6k) <= 0) 
	{
		error ((int6k->flags & INT6K_BAILOUT), ECANCELED, INT6K_CANTSEND);
		return (-1);
	}
	if (ReadMME (int6k, (VS_MODULE_OPERATION | MMTYPE_CNF)) <= 0) 
	{
		error ((int6k->flags & INT6K_BAILOUT), ECANCELED, INT6K_CANTREAD);
		return (-1);
	}
	if (confirm->MSTATUS) 
	{
		Failure (int6k, INT6K_WONTDOIT);
		return (-1);
	}
	Confirm (int6k, "Granted");
	return (0);
}

/*====================================================================*
 *
 *   signed ModuleWrite (struct int6k * int6k);
 *
 *
 *.  Intellon INT6000 Linux Toolkit for HomePlug AV;
 *:  Published 2006-2009 by Intellon Corp. ALL RIGHTS RESERVED;
 *;  For demonstration; Not for production use;
 *
 *--------------------------------------------------------------------*/

signed ModuleWrite (struct int6k * int6k, uint32_t SessionID, struct vs_module_operation_spec * vs_module_operation_spec) 

{

	struct channel * channel = (struct channel *)(int6k->channel);
	struct message * message = (struct message *)(int6k->message);
#ifndef __GNUC__
#pragma pack (push,1)
#endif

	struct __packed vs_module_operation_write_request 
	{
		struct header_eth ethernet;
		struct header_int intellon;
		uint32_t RESERVED;
		uint8_t NUM_OP_DATA;
		uint16_t MOD_OP;
		uint16_t MOD_OP_DATA_LEN;
		uint32_t MOD_OP_RSVD;
		uint32_t MOD_OP_SESSION_ID;
		uint8_t MODULE_IDX;
		uint16_t MODULE_ID;
		uint16_t MODULE_SUB_ID;
		uint16_t MODULE_LENGTH;
		uint32_t MODULE_OFFSET;
		uint8_t MODULE_DATA [1400];
	}
	* request = (struct vs_module_operation_write_request *)(message);
	struct __packed vs_module_operation_write_confirm 
	{
		struct header_eth ethernet;
		struct header_int intellon;
		uint16_t MSTATUS;
		uint16_t ERR_REC_CODE;
		uint32_t RESERVED;
		uint8_t NUM_OP_DATA;
		uint16_t MOD_OP;
		uint16_t MOD_OP_DATA_LEN;
		uint32_t MOD_OP_RSVD;
		uint32_t MOD_OP_SESSION_ID;
		uint8_t MODULE_IDX;
		uint16_t MODULE_ID;
		uint16_t MODULE_SUB_ID;
		uint32_t MODULE_LENGTH;
		uint32_t MODULE_OFFSET;
	}
	* confirm = (struct vs_module_operation_write_confirm *)(message);

#ifndef __GNUC__
#pragma pack (pop)
#endif

	Request (int6k, "Write %s", filepart (int6k->NVM.name));
	memset (message, 0, sizeof (struct message));
	EthernetHeader (&message->ethernet, channel->peer, channel->host);
	IntellonHeader (&message->intellon, (VS_MODULE_OPERATION | MMTYPE_REQ));
	int6k->packetsize = sizeof (struct vs_module_operation_write_request);
	request->NUM_OP_DATA = 1;
	request->MOD_OP = HTOLE16 (0x11);
	request->MOD_OP_DATA_LEN = HTOLE16 (23 + vs_module_operation_spec->MODULE_LENGTH);
	request->MOD_OP_SESSION_ID = HTOLE32 (SessionID);
	request->MODULE_IDX = 0;
	request->MODULE_ID = HTOLE16 (vs_module_operation_spec->MODULE_ID);
	request->MODULE_SUB_ID = HTOLE16 (vs_module_operation_spec->MODULE_SUB_ID);
	request->MODULE_LENGTH = HTOLE16 ((uint16_t)(vs_module_operation_spec->MODULE_LENGTH));
	request->MODULE_OFFSET = HTOLE32 (0);
	read (int6k->NVM.file, request->MODULE_DATA, vs_module_operation_spec->MODULE_LENGTH);
	if (SendMME (int6k) <= 0) 
	{
		error ((int6k->flags & INT6K_BAILOUT), ECANCELED, INT6K_CANTSEND);
		return (-1);
	}
	if (ReadMME (int6k, (VS_MODULE_OPERATION | MMTYPE_CNF)) <= 0) 
	{
		error ((int6k->flags & INT6K_BAILOUT), ECANCELED, INT6K_CANTREAD);
		return (-1);
	}
	if (confirm->MSTATUS) 
	{
		Failure (int6k, INT6K_WONTDOIT);
		return (-1);
	}
	Confirm (int6k, "Written");
	return (0);
}

/*====================================================================*
 *
 *   signed ModuleCommit (struct int6k * int6k, uint32_t SessionID, struct vs_module_operation_spec * vs_module_operation_spec);
 *
 *
 *.  Intellon INT6000 Linux Toolkit for HomePlug AV;
 *:  Published 2006-2009 by Intellon Corp. ALL RIGHTS RESERVED;
 *;  For demonstration; Not for production use;
 *
 *--------------------------------------------------------------------*/

signed ModuleCommit (struct int6k * int6k, uint32_t SessionID, struct vs_module_operation_spec * vs_module_operation_spec) 

{

	struct channel * channel = (struct channel *)(int6k->channel);
	struct message * message = (struct message *)(int6k->message);
#ifndef __GNUC__
#pragma pack (push,1)
#endif

	struct __packed vs_module_operation_commit_request 
	{
		struct header_eth ethernet;
		struct header_int intellon;
		uint32_t RESERVED;
		uint8_t NUM_OP_DATA;
		uint16_t MOD_OP;
		uint16_t MOD_OP_DATA_LEN;
		uint32_t MOD_OP_RSVD;
		uint32_t MOD_OP_SESSION_ID;
		uint32_t COMMIT_CODE;
		uint8_t RSVD [20];
	}
	* request = (struct vs_module_operation_commit_request *)(message);
	struct __packed vs_module_operation_commit_confirm 
	{
		struct header_eth ethernet;
		struct header_int intellon;
		uint16_t MSTATUS;
		uint16_t ERR_REC_CODE;
		uint32_t RESERVED1;
		uint8_t NUM_OP_DATA;
		uint16_t MOD_OP;
		uint16_t MOD_OP_DATA_LEN;
		uint32_t MOD_OP_RSVD;
		uint32_t MOD_OP_SESSION_ID;
		uint32_t COMMIT_CODE;
		uint8_t NUM_MODULES;
		struct 
		{
			uint16_t MOD_STATUS;
			uint16_t ERR_REC_CODE;
		}
		MOD_OP_DATA [1];
	}
	* confirm = (struct vs_module_operation_commit_confirm *)(message);

#ifndef __GNUC__
#pragma pack (pop)
#endif

	unsigned timeout;
	Request (int6k, "Commit Modules");
	memset (message, 0, sizeof (struct message));
	EthernetHeader (&message->ethernet, channel->peer, channel->host);
	IntellonHeader (&message->intellon, (VS_MODULE_OPERATION | MMTYPE_REQ));
	int6k->packetsize = ETHER_MIN_LEN;
	request->NUM_OP_DATA = 1;
	request->MOD_OP = HTOLE16 (0x12);
	request->MOD_OP_DATA_LEN = HTOLE16 (36);
	request->MOD_OP_SESSION_ID = HTOLE32 (SessionID);
	request->COMMIT_CODE = HTOLE32 (0);
	if (SendMME (int6k) <= 0) 
	{
		error ((int6k->flags & INT6K_BAILOUT), ECANCELED, INT6K_CANTSEND);
		return (-1);
	}
	timeout = channel->timeout;
	channel->timeout = 5000;
	if (ReadMME (int6k, (VS_MODULE_OPERATION | MMTYPE_CNF)) <= 0) 
	{
		error ((int6k->flags & INT6K_BAILOUT), ECANCELED, INT6K_CANTREAD);
		return (-1);
	}
	channel->timeout = timeout;
	if (confirm->MSTATUS) 
	{
		Failure (int6k, INT6K_WONTDOIT);
		return (-1);
	}
	Confirm (int6k, "Committed");
	return (0);
}

/*====================================================================*
 *
 *   void Manager (struct int6k * int6k, uint32_t SessionID, struct vs_module_operation_spec * vs_module_operation_spec);
 *
 *
 *.  Intellon INT6000 Linux Toolkit for HomePlug AV;
 *:  Published 2006-2009 by Intellon Corp. ALL RIGHTS RESERVED;
 *;  For demonstration; Not for production use;
 *
 *--------------------------------------------------------------------*/

static void Manager (struct int6k * int6k, uint32_t SessionID, struct vs_module_operation_spec * vs_module_operation_spec) 

{
	ModuleSession (int6k, SessionID, vs_module_operation_spec);
	ModuleWrite (int6k, SessionID, vs_module_operation_spec);
	ModuleCommit (int6k, SessionID, vs_module_operation_spec);
	return;

#if 0

	if (_anyset (int6k->flags, INT6K_OPEN_SESSION)) 
	{
		ModuleSession (int6k, SessionID, vs_module_operation_spec);
	}
	if (_anyset (int6k->flags, INT6K_WRITE_MODULE)) 
	{
		ModuleWrite (int6k, SessionID, vs_module_operation_spec);
	}
	if (_anyset (int6k->flags, INT6K_COMMIT_MODULE)) 
	{
		ModuleCommit (int6k, SessionID, vs_module_operation_spec);
	}
	if (_anyset (int6k->flags, INT6K_READ_MODULE)) 
	{
		ModuleRead (int6k, SessionID, vs_module_operation_spec);
	}

#endif

	return;
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
 *.  Intellon INT6000 Linux Toolkit for HomePlug AV;
 *:  Published 2006-2009 by Intellon Corp. ALL RIGHTS RESERVED;
 *;  For demonstration; Not for production use;
 *
 *--------------------------------------------------------------------*/

int main (int argc, const char * argv []) 

{
	extern struct channel channel;
	extern const struct _term_ devices [];
	static const char *optv [] = 
	{
		"ei:m:M:qs:S:t:vx",
		"module [module] [...]",
		"Intellon Module Management Utility",
		"e\tredirect stderr messages to stdout",

#if defined (WINPCAP) || defined (LIBPCAP)

		"i n\thost interface number is n [2]",

#else

		"i s\thost interface name is s [" CHANNEL_ETHDEVICE "]",

#endif

		"m f\tread module from NVRAM to file f (not implemented)",
		"M f\twrite module file f to NVRAM",
		"s n\tmodule sub-ID",
		"S n\tsession ID",
		"t n\tmodule ID",
		"q\tquiet mode",
		"v\tverbose mode",
		(const char *) (0) 
	};

#include "../int6k/int6k-struct.c"

	uint32_t SessionID = 0x78563412;
	struct vs_module_operation_spec vs_module_operation_spec = 
	{
		0 
	};
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
		case 'e':
			dup2 (STDOUT_FILENO, STDERR_FILENO);
			break;
		case 'C':
			_setbits (int6k.flags, INT6K_COMMIT_MODULE);
			break;
		case 'i':

#if defined (WINPCAP) || defined (LIBPCAP)

			channel.id = atoi (optarg);

#else

			channel.name = optarg;

#endif

			break;
		case 'M':
			_setbits (int6k.flags, INT6K_WRITE_MODULE);
			if (!checkfilename (optarg)) 
			{
				error (1, EINVAL, "%s", optarg);
			}
			if ((int6k.NVM.file = open (optarg, O_BINARY|O_RDONLY)) == -1) 
			{
				error (1, errno, "%s", optarg);
			}
			int6k.NVM.name = optarg;
			if (modulefile (&int6k.NVM, &vs_module_operation_spec) == -1) 
			{
				error (1, errno, "%s", optarg);
			}
			break;
		case 'm':
			_setbits (int6k.flags, INT6K_READ_MODULE);
			if (!checkfilename (optarg)) 
			{
				error (1, EINVAL, "%s", optarg);
			}
			if ((int6k.nvm.file = open (optarg, O_BINARY|O_CREAT|O_RDWR|O_TRUNC, FILE_FILEMODE)) == -1) 
			{
				error (1, errno, "%s", optarg);
			}

#ifndef WIN32

			chown (optarg, getuid (), getgid ());

#endif

			int6k.nvm.name = optarg;
			break;
		case 't':
			vs_module_operation_spec.MODULE_ID = (uint16_t)(basespec (optarg, 0, sizeof (uint16_t)));
			break;
		case 's':
			vs_module_operation_spec.MODULE_SUB_ID = (uint16_t)(basespec (optarg, 0, sizeof (uint16_t)));
			break;
		case 'S':
			SessionID = (uint32_t)(basespec (optarg, 0, sizeof (uint32_t)));
			break;
		case 'q':
			_setbits (channel.flags, CHANNEL_SILENCE);
			_setbits (int6k.flags, INT6K_SILENCE);
			break;
		case 'v':
			_setbits (channel.flags, CHANNEL_VERBOSE);
			_setbits (int6k.flags, INT6K_VERBOSE);
			break;
		case 'x':
			_setbits (int6k.flags, INT6K_BAILOUT);
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
		if (int6k.nvm.file != -1) 
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
		Manager (&int6k, SessionID, &vs_module_operation_spec);
	}
	while ((argc) && (*argv)) 
	{
		if (!hexencode (channel.peer, sizeof (channel.peer), synonym (*argv, devices, DEVICES))) 
		{
			error (1, errno, INT6K_BAD_MAC, *argv);
		}
		Manager (&int6k, SessionID, &vs_module_operation_spec);
		argc--;
		argv++;
	}
	free (int6k.message);
	closechannel (&channel);
	exit (0);
}

