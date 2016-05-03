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
 *   int6ktest.c - Intellon Test Applet Loader;
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
#include <limits.h>
#include <errno.h>

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
#include "../int6k/int6k.h"
#include "../ether/channel.h"
#include "../ram/nvram.h"
#include "../ram/sdram.h"
#include "../nvm/nvm.h"
#include "../pib/pib.h"

/*====================================================================*
 *   custom source files;
 *--------------------------------------------------------------------*/

#ifndef MAKEFILE
#include "../int6k/Confirm.c"
#include "../int6k/Display.c"
#include "../int6k/Failure.c"
#include "../int6k/Request.c"
#include "../int6k/SendMME.c"
#include "../int6k/WriteMEM.c"
#include "../int6k/EthernetHeader.c"
#include "../int6k/IntellonHeader.c"
#include "../int6k/FirmwarePacket.c"
#include "../int6k/UnwantedPacket.c"
#include "../int6k/WaitForReset.c"
#include "../int6k/WaitForStart.c"
#include "../int6k/WaitForStop.c"
#endif

#ifndef MAKEFILE
#include "../tools/getoptv.c"
#include "../tools/putoptv.c"
#include "../tools/version.c"
#include "../tools/uintspec.c"
#include "../tools/checkfilename.c"
#include "../tools/hexdecode.c"
#include "../tools/todigit.c"
#include "../tools/hexdump.c"
#include "../tools/checksum_32.c"
#include "../tools/fdchecksum_32.c"
#include "../tools/error.c"
#include "../tools/alert.c"
#include "../tools/strfbits.c"
#include "../tools/filepart.c"
#endif

#ifndef MAKEFILE
#include "../ether/openchannel.c"
#include "../ether/closechannel.c"
#include "../ether/readpacket.c"
#include "../ether/sendpacket.c"
#include "../ether/channel.c"
#endif

#ifndef MAKEFILE
#include "../nvm/nvmfile.c"
#include "../nvm/nvmpeek.c"
#endif

#ifndef MAKEFILE
#include "../mme/MMECode.c"
#endif

/*====================================================================*
 *
 *   signed ReadMME (struct int6k * int6k, uint16_t MMTYPE);
 *
 *   int6k.h        
 *
 *   this is a custom version of ReadMME() that intercepts VS_ARPC messages for int6ktest;
 *
 *.  Intellon INT6x00 Linux Toolkit for HomePlug AV; 
 *:  Published 2007 by Intellon Corp. ALL RIGHTS RESERVED;
 *;  For demonstration; Not for production use;
 *
 *   Contributor(s):
 *      Charles Maier <charles.maier@intellon.com>
 *
 *--------------------------------------------------------------------*/

signed ReadMME (struct int6k * int6k, uint16_t MMTYPE) 

{
	struct channel * channel = (struct channel *)(int6k->channel);
	struct message * message = (struct message *)(int6k->message);
	while ((int6k->packetsize = readpacket (channel, message, sizeof (struct message))) > 0) 
	{
		if (FirmwarePacket (int6k)) 
		{
			continue;
		}
		if (UnwantedPacket (message, MMTYPE)) 
		{
			continue;
		}
		break;
	}
	return (int6k->packetsize);
}

/*====================================================================*
 *
 *   signed ConfigureMemory (struct int6k * int6k, struct config_ram * config_ram);
 *
 *   download SDRAM configuration parameters using a VS_SET_SDRAM 
 *   message; this function is deprecated in favor of the memctl.nvm 
 *   applet but may still be needed on INT6000 devices;
 *
 *   See the Intellon HomePlug AV Firmware Technical Reference Manual 
 *   for more information;
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

#if 0

static signed ConfigureMemory (struct int6k * int6k, struct config_ram * config_ram) 

{
	struct channel * channel = (struct channel *)(int6k->channel);
	struct message * message = (struct message *)(int6k->message);

#ifndef __GNUC__
#pragma pack (push,1)
#endif

	struct __packed vs_set_sdram_request 
	{
		struct header_eth ethernet;
		struct header_int intellon;
		struct config_ram config_ram;
		uint32_t CHECKSUM;
	}
	* request = (struct vs_set_sdram_request *) (message);
	struct __packed vs_set_sdram_confirm 
	{
		struct header_eth ethernet;
		struct header_int intellon;
		uint8_t MSTATUS;
	}
	* confirm = (struct vs_set_sdram_confirm *) (message);

#ifndef __GNUC__
#pragma pack (pop)
#endif

	memset (message, 0, sizeof (struct message));
	EthernetHeader (&message->ethernet, channel->peer, channel->host);
	IntellonHeader (&message->intellon, (VS_SET_SDRAM | MMTYPE_REQ));
	int6k->packetsize = ETHER_MIN_LEN;
	memcpy (&request->config_ram, config_ram, sizeof (struct config_ram));
	request->CHECKSUM = checksum_32 (&request->config_ram, sizeof (struct config_ram), 0);
	if (SendMME (int6k) <= 0) 
	{
		error ((int6k->flags & INT6K_BAILOUT), ECANCELED, INT6K_CANTSEND);
		return (-1);
	}
	if (ReadMME (int6k, (VS_SET_SDRAM | MMTYPE_CNF)) <= 0) 
	{
		error ((int6k->flags & INT6K_BAILOUT), ECANCELED, INT6K_CANTREAD);
		return (-1);
	}
	if (confirm->MSTATUS) 
	{
		Failure (int6k, "Memory will not Configure.");
		return (-1);
	}
	return (0);
}

#endif

/*====================================================================*
 *
 *   signed ExecuteApplet (struct int6k * int6k, struct header_nvm * header_nvm, unsigned image) 
 *
 *   int6k.h
 *
 *   start applet execution using VS_ST_MAC and wait for execution
 *   to complete;
 *
 *   See the Intellon HomePlug AV Firmware Technical Reference Manual 
 *   for more information;
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

signed ExecuteApplet (struct int6k * int6k, struct header_nvm * header_nvm, unsigned image) 

{

	struct channel * channel = (struct channel *)(int6k->channel);
	struct message * message = (struct message *)(int6k->message);
#ifndef __GNUC__
#pragma pack (push,1)
#endif

	struct __packed vs_st_mac_request 
	{
		struct header_eth ethernet;
		struct header_int intellon;
		uint8_t MODULEID;
		uint8_t RESERVED [3];
		uint32_t IMAGELOAD;
		uint32_t IMAGELENGTH;
		uint32_t IMAGECHECKSUM;
		uint32_t IMAGESTART;
	}
	* request = (struct vs_st_mac_request *) (message);
	struct __packed vs_st_mac_confirm 
	{
		struct header_eth ethernet;
		struct header_int intellon;
		uint8_t MSTATUS;
		uint8_t MODULEID;
	}
	* confirm = (struct vs_st_mac_confirm *) (message);

#ifndef __GNUC__
#pragma pack (pop)
#endif

	if (_anyset (LE32TOH (header_nvm->IGNOREMASK), (1 << (int6k->hardwareID - 1)))) 
	{
		return (0);
	}
	if (WriteMEM (int6k, &int6k->NVM, LE32TOH (header_nvm->IMAGEADDR), LE32TOH (header_nvm->IMAGELENGTH))) 
	{
		return (-1);
	}
	Request (int6k, "Start %s (%d)", filepart (int6k->NVM.name), image);
	memset (message, 0, sizeof (struct message));
	EthernetHeader (&message->ethernet, channel->peer, channel->host);
	IntellonHeader (&message->intellon, (VS_ST_MAC | MMTYPE_REQ));
	int6k->packetsize = ETHER_MIN_LEN;
	request->MODULEID = VS_MODULE_MAC;
	request->IMAGELOAD = header_nvm->IMAGEADDR;
	request->IMAGELENGTH = header_nvm->IMAGELENGTH;
	request->IMAGECHECKSUM = header_nvm->IMAGECHECKSUM;
	request->IMAGESTART = header_nvm->ENTRYPOINT;
	if (SendMME (int6k) <= 0) 
	{
		error ((int6k->flags & INT6K_BAILOUT), ECANCELED, INT6K_CANTSEND);
		return (-1);
	}
	if (ReadMME (int6k, (VS_ST_MAC | MMTYPE_CNF)) <= 0) 
	{
		error ((int6k->flags & INT6K_BAILOUT), ECANCELED, INT6K_CANTREAD);
		return (-1);
	}
	if (confirm->MSTATUS) 
	{
		Failure (int6k, "Applet did not Start.");
		return (-1);
	}
	if (WaitForStop (int6k)) 
	{
		return (-1);
	}
	if (int6k->state) 
	{
		Failure (int6k, "Test Failed: %s (%d)", filepart (int6k->NVM.name), image);
	}
	return (0);
}

/*====================================================================*
 *  
 *   signed ExecuteApplets (struct int6k * int6k);
 *
 *   int6k.h
 *  
 *   download and execute all image file applets in turn regardless 
 *   of image type; users should make sure that image files contain
 *   nothing but executable image or results may be unpredictable;
 *
 *   DO NOT USE THIS FUNCTION TO DOWNLOAD AND EXECUTE RUNTIME FIRMWARE;
 *
 *   See the Intellon HomePlug AV Firmware Technical Reference Manual 
 *   for more information;
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

static signed ExecuteApplets (struct int6k * int6k) 

{
	struct header_nvm header_nvm;
	signed image = 0;
	memset (&header_nvm, 0, sizeof (header_nvm));
	if (lseek (int6k->NVM.file, 0, SEEK_SET)) 
	{
		error ((int6k->flags & INT6K_BAILOUT), errno, "Can't home %s", filepart (int6k->NVM.name));
		return (-1);
	}
	do 
	{
		if (lseek (int6k->NVM.file, 0, SEEK_CUR) != LE32TOH (header_nvm.NEXTHEADER)) 
		{
			if (_allclr (int6k->flags, INT6K_SILENCE)) 
			{
				error (0, 0, "Bad file position: %s (%d)", filepart (int6k->NVM.name), image);
			}
			return (-1);
		}
		if (read (int6k->NVM.file, &header_nvm, sizeof (struct header_nvm)) < sizeof (struct header_nvm)) 
		{
			if (_allclr (int6k->flags, INT6K_SILENCE)) 
			{
				error (0, errno, "Can't read image header: %s (%d)", filepart (int6k->NVM.name), image);
			}
			return (-1);
		}
		if (LE32TOH (header_nvm.HEADERVERSION) != NVM_HEADER_VERSION) 
		{
			if (_allclr (int6k->flags, INT6K_SILENCE)) 
			{
				error (0, 0, "Bad header version: %s (%d)", filepart (int6k->NVM.name), image);
			}
			return (-1);
		}
		if (checksum_32 (&header_nvm, sizeof (struct header_nvm), 0)) 
		{
			if (_allclr (int6k->flags, INT6K_SILENCE)) 
			{
				error (0, 0, "Bad header checksum: %s (%d)", filepart (int6k->NVM.name), image);
			}
			return (-1);
		}
		if (ExecuteApplet (int6k, &header_nvm, image)) 
		{
			return (-1);
		}
		image++;
	}
	while (header_nvm.NEXTHEADER);
	if (lseek (int6k->NVM.file, 0, SEEK_CUR) != lseek (int6k->NVM.file, 0, SEEK_END)) 
	{
		if (_allclr (int6k->flags, INT6K_SILENCE)) 
		{
			error (0, errno, "Bad header chain: %s (%d)", filepart (int6k->NVM.name), image);
		}
		return (-1);
	}
	return (0);
}

/*====================================================================*
 *   
 *   int main (int argc, const char * argv[]);
 *   
 *   parse command line, populate int6k structure and perform selected 
 *   operations; show help summary when asked; see getoptv and putoptv
 *   to understand command line parsing and help summary display; see
 *   int6k.h for the definition of struct int6k; 
 *
 *   the default interface is eth1 because most people use eth0 as 
 *   their principle network connection; you can specify another 
 *   interface with -i or define environment string PLC to make
 *   that the default interface and save typing;
 *   
 *.  Intellon INT6x00 Linux Toolkit for HomePlug AV; 
 *:  Published 2007 by Intellon Corp. ALL RIGHTS RESERVED;
 *;  For demonstration; Not for production use;
 *
 *--------------------------------------------------------------------*/

int main (int argc, const char * argv []) 

{
	extern struct channel channel;
	char hardware [INT6K_VERSTRING];
	static const char *optv [] = 
	{
		"c:ei:p:qt:vx",
		"file [file] [...]",
		"Intellon Test Applet Loader",
		"c n\tretry count until failure",
		"e\tredirect stderr messages to stdout",

#if defined (WINPCAP) || defined (LIBPCAP)

		"i n\thost interface number [2]",

#else

		"i s\thost interface name [" CHANNEL_ETHDEVICE "]",

#endif

		"p n\tpolls per second second",
		"q\tquiet mode",

#if defined (WINPCAP) || defined (LIBPCAP)

		"t n\tread capture time is (n) milliseconds [50]",

#else

		"t n\tread timeout is (n) milliseconds [50]",

#endif

		"v\tverbose mode",
		"x\texit on error",
		(const char *) (0) 
	};

#include "../int6k/int6k-struct.c"

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
		case 'c':
			int6k.retry = (unsigned)(uintspec (optarg, 1, INT_MAX));
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
		case 'p':
			int6k.timer = _TIMER/(unsigned)(uintspec (optarg, 1, 10));
			break;
		case 'q':
			_setbits (channel.flags, CHANNEL_SILENCE);
			_setbits (int6k.flags, INT6K_SILENCE);
			break;
		case 't':
			channel.timeout = (unsigned)(uintspec (optarg, 0, UINT_MAX));
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
	if (!(int6k.message = malloc (sizeof (struct message)))) 
	{
		error (1, errno, INT6K_ERR_MEMORY);
	}
	if (WaitForStart (&int6k, hardware, sizeof (hardware))) 
	{
		error (1, ECANCELED, "Device must be connected");
	}
	if (strcmp (hardware, INT6K_BOOTLOADER)) 
	{
		error (1, ECANCELED, "BootLoader must be running");
	}
	while ((argc) && (*argv)) 
	{
		int6k.NVM.name = *argv;
		if ((int6k.NVM.file = open (int6k.NVM.name, O_BINARY|O_RDONLY)) == -1) 
		{
			error (0, errno, "%s", int6k.NVM.name);
		}
		else if (nvmfile (&int6k.NVM)) 
		{
			error (0, errno, "Won't load %s", int6k.NVM.name);
		}
		else 
		{
			ExecuteApplets (&int6k);
		}
		close (int6k.NVM.file);
		argc--;
		argv++;
	}
	free (int6k.message);
	closechannel (&channel);
	return (int6k.state);
}

