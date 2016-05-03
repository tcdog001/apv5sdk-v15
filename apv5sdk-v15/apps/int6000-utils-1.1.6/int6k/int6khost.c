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
 *   int6khost.c - Intellon INT6300 Host Emulator
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
#include "../int6k/FactoryDefaults.c"
#include "../int6k/FlashDevice.c"
#include "../int6k/FlashNVM.c"
#include "../int6k/HostActionResponse.c"
#include "../int6k/EmulateHost.c"
#include "../int6k/Request.c"
#include "../int6k/ResetDevice1.c"
#include "../int6k/ReadMME.c"
#include "../int6k/SendMME.c"
#include "../int6k/StartFirmware.c"
#include "../int6k/WaitForReset.c"
#include "../int6k/WaitForStart.c"
#include "../int6k/WriteMEM.c"
#include "../int6k/WriteNVM.c"
#include "../int6k/WritePIB.c"
#include "../int6k/WriteCFG.c"
#include "../int6k/ReadNVM.c"
#include "../int6k/ReadPIB.c"
#include "../int6k/StartDevice.c"
#include "../int6k/EthernetHeader.c"
#include "../int6k/IntellonHeader.c"
#include "../int6k/UnwantedPacket.c"
#endif

#ifndef MAKEFILE
#include "../tools/getoptv.c"
#include "../tools/putoptv.c"
#include "../tools/version.c"
#include "../tools/uintspec.c"
#include "../tools/checkfilename.c"
#include "../tools/hexdecode.c"
#include "../tools/hexstring.c"
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
#include "../mme/MMECode.c"
#endif

#ifndef MAKEFILE
#include "../crypt/keys.c"
#endif

#ifndef MAKEFILE
#include "../ram/sdramfile.c"
#include "../ram/sdrampeek.c"
#endif

#ifndef MAKEFILE
#include "../nvm/nvmfile.c"
#include "../nvm/nvmpeek.c"
#endif

#ifndef MAKEFILE
#include "../pib/pibcodes.c"
#include "../pib/pibfile.c"
#include "../pib/pibpeek.c"
#endif

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
	static const char *optv [] = 
	{
		"C:eFi:n:N:p:P:qt:vx",
		"-N file -P file [-n file] [-p file]",
		"Intellon INT6300 Host Emulator",
		"C f\twrite NVM file to device using VS_SET_SDRAM",
		"e\tredirect stderr messages to stdout",
		"F[F]\tflash (force) NVRAM using VS_MOD_NVM",

#if defined (WINPCAP) || defined (LIBPCAP)

		"i n\thost interface number [2]",

#else

		"i s\thost interface name [" CHANNEL_ETHDEVICE "]",

#endif

		"n f\tread NVM from device into file using VS_RD_MOD",
		"N f\twrite NVM file to device using VS_WR_MEM",
		"p f\tread PIB from device into file using VS_RD_MOD",
		"P f\twrite PIB file to device using VS_WR_MEM",
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
		case 'C':
			if (!checkfilename (optarg)) 
			{
				error (1, EINVAL, "%s", optarg);
			}
			if ((int6k.CFG.file = open (optarg, O_BINARY|O_RDONLY)) == -1) 
			{
				error (1, errno, "%s", optarg);
			}
			if (sdramfile (int6k.CFG.file, optarg, int6k.flags)) 
			{
				error (1, ECANCELED, "CFG file %s is corrupt", optarg);
			}
			_setbits (int6k.flags, INT6K_SDRAM_CONFIG);
			int6k.CFG.name = optarg;
			break;
		case 'e':
			dup2 (STDOUT_FILENO, STDERR_FILENO);
			break;
		case 'F':
			_setbits (int6k.modulecode, (VS_MODULE_MAC | VS_MODULE_PIB));
			if (_anyset (int6k.flags, INT6K_FLASH_DEVICE)) 
			{
				_setbits (int6k.modulecode, VS_MODULE_FORCE);
			}
			_setbits (int6k.flags, INT6K_FLASH_DEVICE);
			break;
		case 'i':

#if defined (WINPCAP) || defined (LIBPCAP)

			channel.id = atoi (optarg);

#else

			channel.name = optarg;

#endif

			break;
		case 'N':
			if (!checkfilename (optarg)) 
			{
				error (1, EINVAL, "%s", optarg);
			}
			if ((int6k.NVM.file = open (optarg, O_BINARY|O_RDONLY)) == -1) 
			{
				error (1, errno, "%s", optarg);
			}
			int6k.NVM.name = optarg;
			if (nvmfile (&int6k.NVM)) 
			{
				error (1, errno, "Bad NVM file: %s", int6k.NVM.name);
			}
			_setbits (int6k.flags, INT6K_WRITE_MAC);
			break;
		case 'n':
			if (!checkfilename (optarg)) 
			{
				error (1, EINVAL, "%s", optarg);
			}
			if ((int6k.nvm.file = open (optarg, O_BINARY|O_CREAT|O_RDWR|O_TRUNC, FILE_FILEMODE)) == -1) 
			{
				error (1, errno, "%s", optarg);
			}
			int6k.nvm.name = optarg;
			break;
		case 'P':
			if (!checkfilename (optarg)) 
			{
				error (1, EINVAL, "%s", optarg);
			}
			if ((int6k.PIB.file = open (optarg, O_BINARY|O_RDONLY)) == -1) 
			{
				error (1, errno, "%s", optarg);
			}
			int6k.PIB.name = optarg;
			if (pibfile (&int6k.PIB)) 
			{
				error (1, errno, "Bad PIB file: %s", int6k.PIB.name);
			}
			_setbits (int6k.flags, INT6K_WRITE_PIB);
			break;
		case 'p':
			if (!checkfilename (optarg)) 
			{
				error (1, EINVAL, "%s", optarg);
			}
			if ((int6k.pib.file = open (optarg, O_BINARY|O_CREAT|O_RDWR|O_TRUNC, FILE_FILEMODE)) == -1) 
			{
				error (1, errno, "%s", optarg);
			}
			int6k.pib.name = optarg;
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

/*
 *      cancel operation if the user omitted a file or entered to extra argments 
 *      on the command line;
 */

	if (argc) 
	{
		error (1, ECANCELED, "Too many arguments");
	}
	if (int6k.CFG.file == -1) 
	{
		error (1, ECANCELED, "No host CFG file named");
	}
	if (int6k.NVM.file == -1) 
	{
		error (1, ECANCELED, "No host NVM file named");
	}
	if (int6k.PIB.file == -1) 
	{
		error (1, ECANCELED, "No host PIB file named");
	}
	if (int6k.nvm.file == -1) 
	{
		error (1, ECANCELED, "No user NVM file named");
	}
	if (int6k.pib.file == -1) 
	{
		error (1, ECANCELED, "No user PIB file named");
	}

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
	EmulateHost (&int6k);
	free (int6k.message);
	closechannel (&channel);
	exit (0);
}

