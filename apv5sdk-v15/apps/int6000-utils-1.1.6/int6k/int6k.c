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
 *   int6k.c - Intellon INT6x00 Device Manager;
 *
 *   Intellon INT6x00 Linux Toolkit; 
 *
 *   this program is the Intellon INT6000 Swiss Army Knife for Linux; 
 *   
 *   this program is similar to int6k2 but is implemented without the 
 *   Intellon Embedded API;
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
 *      Abdel Younes <younes@leacom.fr>
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
#include "../int6k/int6k.h"
#include "../ether/channel.h"
#include "../crypt/HPAVKey.h"
#include "../crypt/keys.h"
#include "../ram/sdram.h"
#include "../pib/pib.h"
#include "../nvm/nvm.h"

/*====================================================================*
 *   custom source files;
 *--------------------------------------------------------------------*/

#ifndef MAKEFILE
#include "../int6k/Attributes.c"
#include "../int6k/chipset.c"
#include "../int6k/NVRAMInfo.c"
#include "../int6k/SDRAMInfo.c"
#include "../int6k/NetworkInfo.c"
#include "../int6k/WriteCFG.c"
#include "../int6k/devices.c"
#include "../int6k/Confirm.c"
#include "../int6k/Display.c"
#include "../int6k/FlashDevice.c"
#include "../int6k/FlashNVM.c"
#include "../int6k/FactoryDefaults.c"
#include "../int6k/Identity.c"
#include "../int6k/PushButton.c"
#include "../int6k/StartFirmware.c"
#include "../int6k/SetNMK.c"
#include "../int6k/VersionInfo.c"
#include "../int6k/Request.c"
#include "../int6k/Failure.c"
#include "../int6k/ReadNVM.c"
#include "../int6k/ReadMME.c"
#include "../int6k/ReadMFG.c"
#include "../int6k/ReadPIB.c"
#include "../int6k/ResetDevice1.c"
#include "../int6k/SendMME.c"
#include "../int6k/WriteNVM.c"
#include "../int6k/WritePIB.c"
#include "../int6k/WaitForReset.c"
#include "../int6k/WaitForStart.c"
#include "../int6k/EthernetHeader.c"
#include "../int6k/IntellonHeader.c"
#include "../int6k/UnwantedPacket.c"
#include "../int6k/WatchdogReport.c"
#endif

#ifndef MAKEFILE
#include "../tools/synonym.c"
#include "../tools/alert.c"
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
#include "../tools/filepart.c"
#include "../tools/typename.c"
#include "../tools/markup.c"
#endif

#ifndef MAKEFILE
#include "../ether/openchannel.c"
#include "../ether/closechannel.c"
#include "../ether/readpacket.c"
#include "../ether/sendpacket.c"
#include "../ether/channel.c"
#endif

#ifndef MAKEFILE
#include "../ram/nvram.c"
#include "../ram/nvrampeek.c"
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

#ifndef MAKEFILE
#include "../mme/MMECode.c"
#endif

#ifndef MAKEFILE
#include "../crypt/keys.c"
#endif

/*====================================================================*
 *   program constants;
 *--------------------------------------------------------------------*/

#define BUTTONS 3
#define MODULES 3

/*====================================================================*
 *   program variables;
 *--------------------------------------------------------------------*/

static const struct _term_ modules [MODULES] = 

{
	{
		"nvm",
		"1" 
	},
	{
		"pib",
		"2" 
	},
	{
		"both",
		"3" 
	}
};

static const struct _term_ buttons [BUTTONS] = 

{
	{
		"join",
		"1" 
	},
	{
		"leave",
		"2" 
	},
	{
		"status",
		"2" 
	}
};

/*====================================================================*
 *   
 *   void manager (struct int6k *int6k, signed loop, signed wait);
 *   
 *   perform operations in logical order despite any order specfied 
 *   on the command line; for example read PIB before writing PIB; 
 *
 *   operation order is controlled by the order of "if" statements 
 *   shown here; the entire operation sequence can be repeated with
 *   an optional pause between each iteration;
 * 
 *.  Intellon INT6x00 Linux Toolkit for HomePlug AV; 
 *:  Published 2007 by Intellon Corp. ALL RIGHTS RESERVED;
 *;  For demonstration; Not for production use;
 *
 *--------------------------------------------------------------------*/

static void manager (struct int6k *int6k, signed loop, signed wait) 

{
	signed pass = loop;
	while (pass-- > 0) 
	{
		if (_anyset (int6k->flags, INT6K_VERSION)) 
		{
			VersionInfo (int6k);
		}
		if (_anyset (int6k->flags, INT6K_ATTRIBUTES)) 
		{
			Attributes (int6k);
		}
		if (_anyset (int6k->flags, INT6K_WATCHDOG_REPORT)) 
		{
			WatchdogReport (int6k);
		}
		if (_anyset (int6k->flags, INT6K_NVRAM_INFO)) 
		{
			NVRAMInfo (int6k);
		}
		if (_anyset (int6k->flags, INT6K_SDRAM_INFO)) 
		{
			SDRAMInfo (int6k);
		}
		if (_anyset (int6k->flags, INT6K_READ_PIB_INFO)) 
		{
			Identity (int6k);
		}
		if (_anyset (int6k->flags, INT6K_NETWORK)) 
		{
			NetworkInfo (int6k);
		}
		if (_anyset (int6k->flags, (INT6K_SETLOCALKEY | INT6K_SETREMOTEKEY))) 
		{
			SetNMK (int6k);
		}
		if (_anyset (int6k->flags, INT6K_SDRAM_CONFIG)) 
		{
			WriteCFG (int6k);
		}
		if (_anyset (int6k->flags, INT6K_WRITE_MAC)) 
		{
			WriteNVM (int6k);
		}
		if (_anyset (int6k->flags, INT6K_READ_MAC)) 
		{
			ReadNVM (int6k);
		}
		if (_anyset (int6k->flags, INT6K_WRITE_PIB)) 
		{
			WritePIB (int6k);
		}
		if (_anyset (int6k->flags, INT6K_READ_PIB)) 
		{
			ReadPIB (int6k);
		}
		if (_anyset (int6k->flags, INT6K_PUSH_BUTTON)) 
		{
			PushButton (int6k);
		}
		if (_anyset (int6k->flags, INT6K_FACTORY_DEFAULTS)) 
		{
			FactoryDefaults (int6k);
		}
		if (_anyset (int6k->flags, INT6K_FLASH_DEVICE)) 
		{
			FlashDevice (int6k);
		}
		if (_anyset (int6k->flags, INT6K_RESET_DEVICE)) 
		{
			ResetDevice (int6k);
		}
		if (!pass || !wait) 
		{
			continue;
		}
		if (_anyset (int6k->flags, INT6K_VERBOSE)) 
		{
			alert ("pause %d seconds %d of %d times", wait, loop - pass, loop);
		}
		fflush (stdout);
		sleep (wait);
	}
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
 *.  Intellon INT6x00 Linux Toolkit for HomePlug AV; 
 *:  Published 2007 by Intellon Corp. ALL RIGHTS RESERVED;
 *;  For demonstration; Not for production use;
 *
 *--------------------------------------------------------------------*/

int main (int argc, const char * argv []) 

{
	extern struct channel channel;
	extern const struct key keys [];
	extern const struct _term_ devices [];
	static const char *optv [] = 
	{
		"aB:C:d:D:efFi:IJ:K:l:mMn:N:p:P:QqrRS:st:Tvw:x",
		"device [device] [...]",
		"Atheros INT6x00 Device Manager",
		"a\tread Device Attributes using VS_OP_ATTRIBUTES",
		"B n\tperform pushbutton action (n) using MS_PB_ENC",
		"C n\tflash NVRAM with module (n) using VS_MOD_NVM",
		"d f\tdump and clear watchdog report to file (f) using VS_WD_RPT",
		"D x\tset Device Access Key (DAK) to (x) for option -J [" DAK1 "]",
		"e\tredirect stderr messages to stdout",
		"f\tread NVRAM Configuration using VS_GET_NVM",
		"F[F]\tflash [force] NVRAM with PIB and firmware using VS_MOD_NVM",

#if defined (WINPCAP) || defined (LIBPCAP)

		"i n\thost interface number is n [2]",

#else

		"i s\thost interface name is s [" CHANNEL_ETHDEVICE "]",

#endif

		"I\tread device identity using VS_RD_MOD",
		"J x\tset NMK on remote device (x) via local device using VS_SET_KEY (see -K)",
		"K x\tset NMK to (x) for options -J and -M [" NMK1 "]",
		"l n\tloop (n) times [1]",
		"m\tread network membership information using VS_NW_INFO",
		"M\tset NMK on local device using VS_SET_KEY (see -K)",
		"n f\tread NVM from SDRAM to file (f) using VS_RD_MOD",
		"N f\twrite NVM file (f) to SDRAM using VS_WR_MOD",
		"p f\tread PIB from SDRAM to file (f) using VS_RD_MOD",
		"P f\twrite PIB file (f) to SDRAM using VS_WR_MOD",
		"q\tquiet mode",
		"Q\tquick flash (return immediately)",
		"r\tread hardware and firmware revision using VS_SW_VER",
		"R\treset device using VS_RS_DEV",
		"s\tread SDRAM Configuration using VS_RD_CBLOCK",
		"S f\twrite an SDRAM Configuration file (f) using VS_SET_SDRAM",

#if defined (WINPCAP) || defined (LIBPCAP)

		"t n\tread capture time is (n) milliseconds [50]",

#else

		"t n\tread timeout is (n) milliseconds [50]",

#endif

		"T\treset factory defaults using VS_FAC_DEFAULTS",
		"v\tverbose mode",
		"w n\tpause (n) seconds [0]",
		"x\texit on error",
		(const char *) (0) 
	};

#include "../int6k/int6k-struct.c"

	signed loop = 1;
	signed wait = 0;
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
		case 'a':
			_setbits (int6k.flags, INT6K_ATTRIBUTES);
			break;
		case 'B':
			_setbits (int6k.flags, INT6K_PUSH_BUTTON);
			int6k.pushbutton = (unsigned)(uintspec (synonym (optarg, buttons, BUTTONS), 0, UCHAR_MAX));
			break;
		case 'C':
			_setbits (int6k.flags, INT6K_FLASH_DEVICE);
			int6k.modulecode = (unsigned)(uintspec (synonym (optarg, modules, MODULES), 0, UCHAR_MAX));
			break;
		case 'd':
			_setbits (int6k.flags, INT6K_WATCHDOG_REPORT);
			if (!checkfilename (optarg)) 
			{
				error (1, EINVAL, "%s", optarg);
			}
			if ((int6k.rpt.file = open (optarg, O_BINARY|O_CREAT|O_RDWR|O_TRUNC, FILE_FILEMODE)) == -1) 
			{
				error (1, errno, "%s", optarg);
			}

#ifndef WIN32

			chown (optarg, getuid (), getgid ());

#endif

			int6k.rpt.name = optarg;
			int6k.readaction = 3;
			break;
		case 'D':
			if (!strcmp (optarg, "none")) 
			{
				memcpy (int6k.DAK, keys [0].DAK, sizeof (int6k.DAK));
				break;
			}
			if (!strcmp (optarg, "key1")) 
			{
				memcpy (int6k.DAK, keys [1].DAK, sizeof (int6k.DAK));
				break;
			}
			if (!strcmp (optarg, "key2")) 
			{
				memcpy (int6k.DAK, keys [2].DAK, sizeof (int6k.DAK));
				break;
			}
			if (!hexencode (int6k.DAK, sizeof (int6k.DAK), (const char *)(optarg))) 
			{
				error (1, errno, INT6K_BAD_DAK, optarg);
			}
			break;
		case 'e':
			dup2 (STDOUT_FILENO, STDERR_FILENO);
			break;
		case 'f':
			_setbits (int6k.flags, INT6K_NVRAM_INFO);
			break;
		case 'F':
			_setbits (int6k.modulecode, VS_MODULE_ALL);
			if (_anyset (int6k.flags, INT6K_FLASH_DEVICE)) 
			{
				_setbits (int6k.modulecode, VS_MODULE_FORCE);
			}
			_setbits (int6k.flags, INT6K_FLASH_DEVICE);
			break;
		case 'I':
			_setbits (int6k.flags, INT6K_READ_PIB_INFO);
			break;
		case 'i':

#if defined (WINPCAP) || defined (LIBPCAP)

			channel.id = atoi (optarg);

#else

			channel.name = optarg;

#endif

			break;
		case 'J':
			if (!hexencode (int6k.RDA, sizeof (int6k.RDA), (const char *)(optarg))) 
			{
				error (1, errno, INT6K_BAD_MAC, optarg);
			}
			_setbits (int6k.flags, INT6K_SETREMOTEKEY);
			break;
		case 'K':
			if (!strcmp (optarg, "none")) 
			{
				memcpy (int6k.NMK, keys [0].NMK, sizeof (int6k.NMK));
				break;
			}
			if (!strcmp (optarg, "key1")) 
			{
				memcpy (int6k.NMK, keys [1].NMK, sizeof (int6k.NMK));
				break;
			}
			if (!strcmp (optarg, "key2")) 
			{
				memcpy (int6k.NMK, keys [2].NMK, sizeof (int6k.NMK));
				break;
			}
			if (!hexencode (int6k.NMK, sizeof (int6k.NMK), (const char *)(optarg))) 
			{
				error (1, errno, INT6K_BAD_NMK, optarg);
			}
			break;
		case 'M':
			_setbits (int6k.flags, INT6K_SETLOCALKEY);
			break;
		case 'l':
			loop = (unsigned)(uintspec (optarg, 0, UINT_MAX));
			break;
		case 'm':
			_setbits (int6k.flags, INT6K_NETWORK);
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

#ifndef WIN32

			chown (optarg, getuid (), getgid ());

#endif

			_setbits (int6k.flags, INT6K_READ_MAC);
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

#ifndef WIN32

			chown (optarg, getuid (), getgid ());

#endif

			_setbits (int6k.flags, INT6K_READ_PIB);
			int6k.pib.name = optarg;
			break;
		case 'Q':
			_setbits (int6k.flags, INT6K_QUICK_FLASH);
			break;
		case 'q':
			_setbits (channel.flags, CHANNEL_SILENCE);
			_setbits (int6k.flags, INT6K_SILENCE);
			break;
		case 'R':
			_setbits (int6k.flags, INT6K_RESET_DEVICE);
			break;
		case 'r':
			_setbits (int6k.flags, INT6K_VERSION);
			break;
		case 'S':
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
				error (1, ECANCELED, "SDRAM config file %s is corrupt", optarg);
			}
			_setbits (int6k.flags, INT6K_SDRAM_CONFIG);
			int6k.CFG.name = optarg;
			break;
		case 's':
			_setbits (int6k.flags, INT6K_SDRAM_INFO);
			break;
		case 't':
			channel.timeout = (unsigned)(uintspec (optarg, 0, UINT_MAX));
			break;
		case 'T':
			_setbits (int6k.flags, INT6K_FACTORY_DEFAULTS);
			break;
		case 'v':
			_setbits (channel.flags, CHANNEL_VERBOSE);
			_setbits (int6k.flags, INT6K_VERBOSE);
			break;
		case 'w':
			wait = (unsigned)(uintspec (optarg, 0, 3600));
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
		if (int6k.pib.file != -1) 
		{
			error (1, ECANCELED, INT6K_NODEVICE);
		}
		if (int6k.rpt.file != -1) 
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
		manager (&int6k, loop, wait);
	}
	while ((argc) && (*argv)) 
	{
		if (!hexencode (channel.peer, sizeof (channel.peer), synonym (*argv, devices, DEVICES))) 
		{
			error (1, errno, INT6K_BAD_MAC, *argv);
		}
		manager (&int6k, loop, wait);
		argc--;
		argv++;
	}
	free (int6k.message);
	closechannel (&channel);
	exit (0);
}

