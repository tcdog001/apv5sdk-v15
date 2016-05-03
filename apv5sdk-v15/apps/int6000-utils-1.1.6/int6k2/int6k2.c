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
 *   int6k2.c - Intellon Device Manager;
 *
 *   This program is essentialy the same as int6k but there are some
 *   functional differences because the API is operation oriented as
 *   opposed to message oriented;
 *
 *   Software developers are encouraged to study counterpart modules
 *   in both programs and select those that suite their needs; 
 *
 *   this program sends and receives raw ethernet frames and so needs
 *   root priviledges; if you install it using "chmod 555" and "chown 
 *   root:root" then you must login as root to run it; otherwise, you 
 *   can install it using "chmod 4555" and "chown root:root" so that 
 *   anyone can run it; the program will refuse to run until you get
 *   thing right;
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
 *      Alex Vasquez <alex.vasquez@intellon.com>
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
#include "../tools/putoptv.h"
#include "../tools/memory.h"
#include "../tools/number.h"
#include "../tools/error.h"
#include "../tools/types.h"
#include "../tools/flags.h"
#include "../tools/files.h"
#include "../crypt/HPAVKey.h"
#include "../nodes/nodes.h"
#include "../ihpapi/ihp.h"
#include "../ihpapi/ihpapi.h"
#include "../int6k/int6k.h"
#include "../crypt/keys.h"
#include "../nvm/nvm.h"
#include "../pib/pib.h"

/*====================================================================*
 *   custom source files;
 *--------------------------------------------------------------------*/

#ifndef MAKEFILE
#include "../int6k2/FactoryDefaults2.c"
#include "../int6k2/HostActionWait2.c"
#include "../int6k2/Identity2.c"
#include "../int6k2/MfgString2.c"
#include "../int6k2/NetworkInfo2.c"
#include "../int6k2/ReadMAC2.c"
#include "../int6k2/ReadMME2.c"
#include "../int6k2/ReadPIB2.c"
#include "../int6k2/ResetDevice2.c"
#include "../int6k2/RdModWrPib2.c"
#include "../int6k2/SetKey2.c"
#include "../int6k2/SetSDRAM2.c"
#include "../int6k2/Upgrade2.c"
#include "../int6k2/VersionInfo2.c"
#endif

#ifndef MAKEFILE
#include "../int6k/chipset.c"
#include "../int6k/Confirm.c"
#include "../int6k/SendMME.c"
#include "../int6k/Request.c"
#include "../int6k/Failure.c"
#endif

#ifndef MAKEFILE
#include "../ihpapi/GetManufacturerInfo.c"
#include "../ihpapi/GetNetworkInfo.c"
#include "../ihpapi/GetVersionInfo.c"
#include "../ihpapi/InitializeDevice.c"
#include "../ihpapi/ResetDevice.c"
#include "../ihpapi/ResetToFactoryDefaults.c"
#include "../ihpapi/RxFrame.c"
#include "../ihpapi/SequenceTerminate.c"
#include "../ihpapi/SetKey.c"
#include "../ihpapi/SetSDRAMConfig.c"
#include "../ihpapi/TxFrame.c"
#include "../ihpapi/UpdateDevice.c"
#include "../ihpapi/EncodeEthernetHeader.c"
#include "../ihpapi/EncodeIntellonHeader.c"
#include "../ihpapi/DecodeEthernetPHYSettings.c"
#include "../ihpapi/DecodeFlashNVM.c"
#include "../ihpapi/DecodeGetToneMap.c"
#include "../ihpapi/DecodeGetRxToneMap.c"
#include "../ihpapi/DecodeGetVersion.c"
#include "../ihpapi/DecodeHostAction.c"
#include "../ihpapi/DecodeLinkStats.c"
#include "../ihpapi/DecodeMfgString.c"
#include "../ihpapi/DecodeNetworkInfo.c"
#include "../ihpapi/DecodeResetDevice.c"
#include "../ihpapi/DecodeResetToFactoryDefaults.c"
#include "../ihpapi/DecodeSetKey.c"
#include "../ihpapi/DecodeSetSDRAMCfg.c"
#include "../ihpapi/DecodeStartMAC.c"
#include "../ihpapi/DecodeReadModule.c"
#include "../ihpapi/DecodeWriteMemory.c"
#include "../ihpapi/DecodeWriteModule.c"
#include "../ihpapi/FindFWHeader.c"
#include "../ihpapi/SequenceReadModule.c"
#include "../ihpapi/SequenceWriteMemory.c"
#include "../ihpapi/SequenceWriteModule.c"
#endif

#ifndef MAKEFILE
#include "../tools/getoptv.c"
#include "../tools/putoptv.c"
#include "../tools/version.c"
#include "../tools/uintspec.c"
#include "../tools/alert.c"
#include "../tools/error.c"
#include "../tools/emalloc.c"
#include "../tools/fdchecksum32.c"
#include "../tools/fdchecksum_32.c"
#include "../tools/checksum32.c"
#include "../tools/checksum_32.c"
#include "../tools/hexencode.c"
#include "../tools/hexdecode.c"
#include "../tools/hexstring.c"
#include "../tools/hexdump.c"
#include "../tools/binout.c"
#include "../tools/decout.c"
#include "../tools/typename.c"
#include "../tools/checkfilename.c"
#include "../tools/strfbits.c"
#include "../tools/bclose.c"
#include "../tools/beof.c"
#include "../tools/bgetnexts.c"
#include "../tools/bgeto.c"
#include "../tools/bgets.c"
#include "../tools/binout.c"
#include "../tools/bopen.c"
#include "../tools/buffer.c"
#include "../tools/todigit.c"
#endif

#ifndef MAKEFILE

// #include "../tools/bopen.c"
// #include "../tools/bclose.c"
// #include "../tools/bgets.c"
// #include "../tools/bgetnexts.c"
// #include "../tools/bgeto.c"
// #include "../tools/beof.c"

#endif

#ifndef MAKEFILE
#include "../crypt/SHA256Reset.c"
#include "../crypt/SHA256Block.c"
#include "../crypt/SHA256Write.c"
#include "../crypt/SHA256Fetch.c"
#include "../crypt/HPAVKeyNID.c"
#include "../crypt/keys.c"
#endif

#ifndef MAKEFILE
#include "../ether/openchannel.c"
#include "../ether/closechannel.c"
#include "../ether/sendpacket.c"
#include "../ether/readpacket.c"
#include "../ether/channel.c"
#endif

#ifndef MAKEFILE
#include "../pib/pibfile.c"
#include "../pib/pibpeek.c"
#include "../pib/pibcodes.c"
#endif

#ifndef MAKEFILE
#include "../nvm/nvmfile.c"
#include "../nvm/nvmpeek.c"
#endif

#ifndef MAKEFILE
#include "../mme/MMECode.c"
#endif

#ifndef MAKEFILE
#include "../nodes/reorder.c"
#include "../nodes/xmlattribute.c"
#include "../nodes/xmlvalue.c"
#include "../nodes/xmlcontent.c"
#include "../nodes/xmlnode.c"
#include "../nodes/xmlopen.c"
#include "../nodes/xmlscan.c"
#include "../nodes/xmledit.c"
#include "../nodes/xmlfree.c"
#endif

/*====================================================================*
 *   program constants;
 *--------------------------------------------------------------------*/

#define BROADCAST "FF:FF:FF:FF:FF:FF"
#define LOCALCAST "00:B0:52:00:00:01"

/*====================================================================*
 *   
 *   void DeviceManager (struct int6k *int6k, signed loop, signed wait);
 *   
 *   perform operations in logical order despite any order specfied 
 *   on the command line; for example, read PIB before writing PIB; 
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
			VersionInfo2 (int6k);
		}
		if (_anyset (int6k->flags, INT6K_MANUFACTURER)) 
		{
			MfgString2 (int6k);
		}
		if (_anyset (int6k->flags, INT6K_READ_PIB_INFO)) 
		{
			Identity2 (int6k);
		}
		if (_anyset (int6k->flags, INT6K_NETWORK)) 
		{
			NetworkInfo2 (int6k);
		}
		if (_anyset (int6k->flags, INT6K_READ_PIB)) 
		{
			ReadPIB2 (int6k);
		}
		if (_anyset (int6k->flags, (INT6K_SETLOCALKEY | INT6K_SETREMOTEKEY))) 
		{
			SetKey2 (int6k);
		}
		if (_anyset (int6k->flags, INT6K_SDRAM_CONFIG)) 
		{
			SetSDRAM2 (int6k);
		}
		if (_allset (int6k->flags, (INT6K_WRITE_MAC | INT6K_WRITE_PIB))) 
		{
			Upgrade2 (int6k);
			sleep (20);
		}
		if (_allset (int6k->flags, INT6K_RD_MOD_WR_PIB)) 
		{
			RdModWrPIB2 (int6k);
		}
		if (_anyset (int6k->flags, INT6K_FACTORY_DEFAULTS)) 
		{
			FactoryDefaults2 (int6k);
		}
		if (_anyset (int6k->flags, INT6K_RESET_DEVICE)) 
		{
			ResetDevice2 (int6k);
		}
		if (int6k->index == 0) 
		{
			continue;
		}
		if (!wait) 
		{
			continue;
		}
		if (_allclr (int6k->flags, INT6K_SILENCE)) 
		{
			alert ("pause %d seconds %d more times", wait, pass);
		}
		sleep (wait);
	}
	return;
}

/*====================================================================*
 *   
 *   int main (int argc, const char * argv []);
 *   
 *   parse command line, populate int6k structure and perform selected 
 *   operations; show help summary if asked; see getoptv and putoptv
 *   to understand command line parsing and help summary display; see
 *   int6k2.h for the definition of struct int6k; 
 *
 *   the command line accepts multiple MAC addresses and the program 
 *   performs the specified operations on each address, in turn; the
 *   address order is significant but the option order is not; the
 *   default address is a local broadcast that causes all devices on
 *   the local H1 interface to respond but not those at the remote
 *   end of the powerline;
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
	static const char *optv [] = 
	{
		"D:F:hi:IJ:K:l:mMN:qp:P:rRsS:Tvw:X:x",
		"device [device] [...] [> stdout]",
		"Intellon INT6x00 Device Manager using Intellon HomePlug AV API",
		"D x\tset DAK in hex format",
		"F s\tfirmware and PIB to NVRAM option [NOINT6K_NOFW]",
		"h\twait and process requests from the device",

#if defined (WINPCAP) || defined (LIBPCAP)

		"i n\tuse host interface n [2]",

#else

		"i s\tuse host interface s [" CHANNEL_ETHDEVICE "]",

#endif

		"I\tread device identity with ihpapi_UpdateDevice()",
		"J x\tset NMK on remote device x via local device with ihpapi_SetKey()",
		"K x\tNetwork Membership Key in hex format",
		"l n\tloop n time [1]",
		"m\tnetwork information",
		"M\tset NMK on local device with ihpapi_SetKey()",
		"N f\twrite MAC from file to RAM with ihpapi_InitializeDevice()",
		"p f\tread PIB from RAM to file with ihpapi_UpdateDevice()",
		"P f\twrite PIB from file to RAM with ihpapi_InitializeDevice()",
		"q\tquiet",
		"r\tread hardware and firmware revision with ihpapi_GetVersionInfo()",
		"R\treset device with ihpapi_ResetDevice()",
		"s\tread manufacturer string with ihpapi_GetManufacturerInfo()",
		"S f\tset SDRAM configuration with ihpapi_SetSDRAMConfig()",
		"T\treset to factory defaults with ihpapi_ResetToFactoryDefaults()",
		"v\tverbose messages",
		"w n\tpause n seconds [0]",
		"X f\tread-modify-write PIB",
		"x\texit on error",
		(const char *) (0) 
	};

#include "../int6k/int6k-struct.c"

	signed loop = 1;
	signed wait = 0;
	signed c;
	optind = 1;
	opterr = 1;
	if (getenv (PLCDEVICE)) 
	{
#if defined (WINPCAP) || defined (LIBPCAP)
		channel.id = atoi (getenv (PLCDEVICE));
#else
		channel.name = strdup (getenv (PLCDEVICE));
#endif
	}
	while ((c = getoptv (argc, argv, optv)) != -1) 
	{
		switch ((char) (c)) 
		{
		case 'D':
			if (!strcmp (optarg, "key1")) 
			{
				memcpy (int6k.DAK, keys [0].DAK, sizeof (int6k.DAK));
				break;
			}
			if (!strcmp (optarg, "key2")) 
			{
				memcpy (int6k.DAK, keys [1].DAK, sizeof (int6k.DAK));
				break;
			}
			if (!hexencode (int6k.DAK, sizeof (int6k.DAK), (const char *)(optarg))) 
			{
				error (1, errno, "DAK=[%s]", optarg);
			}
			break;
		case 'F':
			if (!strcmp (optarg, "INT6K_NOFLASH")) 
			{
				int6k.options = INT6K_NOFLASH;
				break;
			}
			if (!strcmp (optarg, "NOINT6K_FW_FLASH")) 
			{
				int6k.options = NOINT6K_FW_FLASH;
				break;
			}
			if (!strcmp (optarg, "INT6K_FLASH")) 
			{
				int6k.options = INT6K_FLASH;
				break;
			}
			error (1, EINVAL, "argument %s is not one of INT6K_FLASH, INT6K_FW_FLASH or INT6K_NO_FLASH", optarg);
			break;
		case 'h':
			_setbits (int6k.flags, INT6K_HOST_ACTION);
			break;
		case 'i':

#if defined (WINPCAP) || defined (LIBPCAP)

			channel.id = atoi (optarg);

#else

			channel.name = optarg;

#endif

			break;
		case 'I':
			_setbits (int6k.flags, INT6K_READ_PIB_INFO);
			break;
		case 'J':
			if (!hexencode (int6k.RDA, sizeof (int6k.RDA), (const char *)(optarg))) 
			{
				error (1, errno, "MAC=[%s]", optarg);
			}
			_setbits (int6k.flags, INT6K_SETREMOTEKEY);
			break;
		case 'K':
			if (!strcmp (optarg, "key1")) 
			{
				memcpy (int6k.NMK, keys [0].NMK, sizeof (int6k.NMK));
				break;
			}
			if (!strcmp (optarg, "key2")) 
			{
				memcpy (int6k.NMK, keys [1].NMK, sizeof (int6k.NMK));
				break;
			}
			if (!hexencode (int6k.NMK, sizeof (int6k.NMK), (const char *)(optarg))) 
			{
				error (1, errno, "NMK=[%s]", optarg);
			}
			break;
		case 'l':
			loop = (unsigned)(uintspec (optarg, 0, UINT_MAX));
			break;
		case 'm':
			_setbits (int6k.flags, INT6K_NETWORK);
			break;
		case 'M':
			_setbits (int6k.flags, INT6K_SETLOCALKEY);
			break;
		case 'n':
			if (!checkfilename (optarg)) 
			{
				error (1, EINVAL, "%s", optarg);
			}
			if ((int6k.NVM.file = open (optarg, O_BINARY|O_CREAT|O_RDWR|O_TRUNC, FILE_FILEMODE)) == -1) 
			{
				error (1, errno, "%s", optarg);
			}
			_setbits (int6k.flags, INT6K_READ_MAC);
			int6k.NVM.name = optarg;
			break;
		case 'N':
			if (!checkfilename (optarg)) 
			{
				error (1, EINVAL, "%s", optarg);
			}
			if ((int6k.NVM.file = open (optarg, O_BINARY|O_RDONLY)) == -1) 
			{
				error (1, errno, "Can't open %s", optarg);
			}
			int6k.NVM.name = optarg;
			if (nvmfile (&int6k.NVM)) 
			{
				error (1, errno, "Bad NVM file: %s", int6k.NVM.name);
			}
			_setbits (int6k.flags, INT6K_WRITE_MAC);
			break;
		case 'p':
			if (!checkfilename (optarg)) 
			{
				error (1, EINVAL, "%s", optarg);
			}
			if ((int6k.PIB.file = open (optarg, O_BINARY|O_CREAT|O_RDWR|O_TRUNC, FILE_FILEMODE)) == -1) 
			{
				error (1, errno, "%s", optarg);
			}
			_setbits (int6k.flags, INT6K_READ_PIB);
			int6k.PIB.name = optarg;
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
		case 'q':
			_setbits (channel.flags, CHANNEL_SILENCE);
			_setbits (int6k.flags, INT6K_SILENCE);
			break;
		case 'r':
			_setbits (int6k.flags, INT6K_VERSION);
			break;
		case 'R':
			_setbits (int6k.flags, INT6K_RESET_DEVICE);
			break;
		case 's':
			_setbits (int6k.flags, INT6K_MANUFACTURER);
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
			int6k.CFG.name = optarg;
			_setbits (int6k.flags, INT6K_SDRAM_CONFIG);
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
		case 'X':
			if (!checkfilename (optarg)) 
			{
				error (1, EINVAL, "%s", optarg);
			}
			int6k.XML.name = optarg;
			_setbits (int6k.flags, INT6K_RD_MOD_WR_PIB);
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
 *      raw packets require root priviledges unless one takes special steps;
 */

#ifndef WIN32

	if (geteuid ()) 
	{
		error (1, EPERM, "Program needs root priviledges.");
	}

#endif

/*
 *   if no hardware addresses or multiple hardware addresses are 
 *   specified on the command line and the user elected to read 
 *   either the MAC or PIB from RAM then terminate to prevent 
 *   accidental overwriting of either file; no hardware addresses
 *   means local broadcast and many device could respond;
 */

	if (argc != 1) 
	{
		if (int6k.nvm.file != -1) 
		{
			error (1, 0, INT6K_NODEVICE);
		}
		if (int6k.pib.file != -1) 
		{
			error (1, 0, INT6K_NODEVICE);
		}
	}

/*
 *   open a layer 2 socket and bind it to the Ethernet interface named
 *   in channel.name on unix-like systems or numbered by channel.id if
 *   using LibPcap or WinPcap;
 */

	openchannel (int6k.channel);

/*
 *   allocate an Ethernet frame buffer and point channel.message to it; 
 *   the API uses this buffer to store all outgoing and incoming Ethernet
 *   frames; frame fields are accessed using message structure members;
 */

	if (!(int6k.message = malloc (sizeof (struct message)))) 
	{
		error (1, errno, INT6K_ERR_MEMORY);
	}

/*
 *   allocate an API workspace and point int6k.result to it; workspace
 *   structure is explained in the API Programmer's Guide; a workspace
 *   is not needed unless you are calling API functions; 
 */

	if (!(int6k.result = malloc (sizeof (ihpapi_result_t)))) 
	{
		error (1, errno, INT6K_ERR_MEMORY);
	}

/*
 *   operate on the default device named in channel.peer when no 
 *   device addresses appear on the command line;
 */

	if (!argc) 
	{
		manager (&int6k, loop, wait);
	}

/*
 *   for each hardware address, replace the channel.peer address and
 *   perform all indicated operations on each device; replace symbolic
 *   addresses ("all", "broadcast" and "local") with literal addresses 
 *   for convenience; operations are indicated by bits in int6k.flags;
 */

	while ((argc--) && (*argv)) 
	{
		if (!strcmp (*argv, "all")) 
		{
			*argv = BROADCAST;
		}
		else if (!strcmp (*argv, "broadcast")) 
		{
			*argv = BROADCAST;
		}
		else if (!strcmp (*argv, "local")) 
		{
			*argv = LOCALCAST;
		}
		if (!hexencode (channel.peer, sizeof (channel.peer), *argv)) 
		{
			error (1, errno, "MAC=[%s]", *argv);
		}
		manager (&int6k, loop, wait);
		argv++;
	}

/*
 *   release Ethernet frame buffer and API workspace; close socket;
 */

	free (int6k.message);
	free (int6k.result);
	closechannel (int6k.channel);
	exit (0);
}

