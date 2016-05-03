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

/*====================================================================*"
 *   system header files;
 *--------------------------------------------------------------------*/

#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>
#include <ctype.h>
#include <errno.h>
#include <math.h>

/*====================================================================*
 *   custom header files;
 *--------------------------------------------------------------------*/

#include "../tools/getoptv.h"
#include "../tools/putoptv.h"
#include "../tools/memory.h"
#include "../tools/number.h"
#include "../tools/symbol.h"
#include "../tools/types.h"
#include "../tools/flags.h"
#include "../tools/files.h"
#include "../tools/error.h"
#include "../tools/chars.h"
#include "../mme/mme.h"
#include "../plc/plc.h"
#include "../pib/pib.h"

/*====================================================================*
 *   custom source files;
 *--------------------------------------------------------------------*/

#ifndef MAKEFILE
#include "../plc/Devices.c"
#include "../plc/Failure.c"
#include "../plc/ReadMME.c"
#include "../plc/SendMME.c"
#include "../mme/UnwantedMessage.c"
#endif

#ifndef MAKEFILE
#include "../tools/error.c"
#include "../tools/getoptv.c"
#include "../tools/putoptv.c"
#include "../tools/version.c"
#include "../tools/uintspec.c"
#include "../tools/hexdump.c"
#include "../tools/hexencode.c"
#include "../tools/hexdecode.c"
#include "../tools/hexstring.c"
#include "../tools/todigit.c"
#include "../tools/synonym.c"
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
#include "../mme/EthernetHeader.c"
#include "../mme/FragmentHeader.c"
#endif

/*====================================================================*
 *   program constants;
 *--------------------------------------------------------------------*/

#define SNR_ANALYSE  (1 << 0)

#define DATAPOINT ",%d"
#define ERRORRATE ",%4.3f"
#define FREQUENCY "%5.2f"
#define ATTENUATION ",%5.2f"

#define ACTIVE 917
#define GROUPS 58
#define BUNCH 16

/*====================================================================*
 *   program variables;
 *--------------------------------------------------------------------*/

static uint32_t NumSNR [PLC_SLOTS];
static uint32_t BadMPDU [PLC_SLOTS];
static uint32_t LongAvgPbErrRate [PLC_SLOTS];
static uint32_t ShortAvgPbErrRate [PLC_SLOTS];
static uint32_t LongAvgBER [PLC_SLOTS];
static uint32_t ShortAvgBER [PLC_SLOTS];
static uint32_t SNRMean [PLC_SLOTS];
static uint8_t SNRMode [PLC_SLOTS];
static uint8_t TMTrigger [PLC_SLOTS];
static uint8_t ReTrigger [PLC_SLOTS];
static uint8_t AGC [PLC_SLOTS];
static uint8_t SNR [PLC_SLOTS] [AMP_CARRIERS];

/*====================================================================*
 *   program functions;
 *--------------------------------------------------------------------*/

#define db2snr(db) (((db) < 56)?(float)(0.0):(float)(db)/4-14)
#define AgcIndex2RxPgaGain(x) ((x)?2.0*(x)-14:-18)
#define snr_db(x) (((x)-56)/4)
#define snr_lin(x) (10^((x)/10))

/*====================================================================*
 *   program constants;
 *--------------------------------------------------------------------*/

#define COUPLE (sizeof (couple) / sizeof (struct _term_))

#define DEFAULT_COUPLING 0

/*====================================================================*
 *   program variables;
 *--------------------------------------------------------------------*/

static const struct _term_ couple [] = 

{
	{
		"alt",
		"1"
	},
	{
		"pri",
		"0"
	}
};


/*====================================================================*
 *
 *   signed collect (struct plc * plc, uint8_t slots, uint16_t carriers) 
 *
 *   collect SNR data for all slots and carriers; 
 *
 *--------------------------------------------------------------------*/

signed collect (struct plc * plc, uint8_t slots, uint16_t carriers) 

{
	extern uint8_t SNR [PLC_SLOTS] [AMP_CARRIERS];
	struct channel * channel = (struct channel *)(plc->channel);
	struct message * message = (struct message *)(plc->message);
	unsigned offset = 0;
	unsigned extent = 0;
	uint8_t slot = 0;

#ifndef __GNUC__
#pragma pack (push,1)
#endif

	struct __packed vs_debug_info_request 
	{
		struct ethernet_std ethernet;
		struct qualcomm_fmi qualcomm;
		uint32_t MME_LEN;
		uint16_t REQ_TYPE;
		uint32_t Reserved1;
		uint8_t MACADDR [ETHER_ADDR_LEN];
		uint8_t TMSLOT;
		uint8_t COUPLING;
	}
	* request = (struct vs_debug_info_request *) (message);
	struct __packed vs_debug_info_confirm 
	{
		struct ethernet_std ethernet;
		struct qualcomm_fmi qualcomm;
		uint8_t MSTATUS;
		uint8_t Reserved1;
		uint32_t MME_LEN;
		uint16_t REQ_TYPE;
		uint16_t Reserved3;
		uint8_t MACADDR [ETHER_ADDR_LEN];
		uint8_t TMSLOT;
		uint8_t Reserved4;
		uint32_t NUMSNR;
		uint32_t Reserved5;
		uint32_t BADMPDUCNT;
		uint8_t SNRMERGEMODE;
		uint8_t Reserved6 [3];
		uint8_t TM_TRIGGER;
		uint8_t Reserved7 [3];
		uint8_t RETRIGGERSOUNDCOMP;
		uint8_t Reserved8 [3];
		uint32_t LONGAVGPBERRRATE;
		uint32_t SHORTAVGPBERRRATE;
		uint32_t Reserved9;
		uint32_t LONGAVGBER;
		uint32_t SHORTAVGBER;
		uint8_t Reserved10 [8];
		uint8_t SNRDATA [1];
	}
	* confirm = (struct vs_debug_info_confirm *) (message);
	struct __packed vs_debug_info_fragment 
	{
		struct ethernet_std ethernet;
		struct homeplug_fmi qualcomm;
		uint8_t SNRDATA [1];
	}
	* fragment = (struct vs_debug_info_fragment *) (message);

#ifndef __GNUC__
#pragma pack (pop)
#endif

	for (offset = slot = 0; slot < slots; slot++, offset = 0) 
	{
		memset (message, 0, sizeof (struct message));
		EthernetHeader (&request->ethernet, channel->peer, channel->host, HOMEPLUG_MTYPE);
		FragmentHeader (&request->qualcomm, 1, (VS_DEBUG_INFO | MMTYPE_REQ));
		request->MME_LEN = HTOLE32 (sizeof (* request) - sizeof (request->ethernet) - sizeof (request->qualcomm) - sizeof (request->MME_LEN));
		request->REQ_TYPE = HTOLE16 (1);
		memcpy (request->MACADDR, plc->RDA, sizeof (request->MACADDR));
		request->TMSLOT = slot;
		request->COUPLING = plc->hostaction;
		plc->packetsize = (ETHER_MIN_LEN - ETHER_CRC_LEN);
		if (SendMME (plc) <= 0) 
		{
			error (1, ECANCELED, CHANNEL_CANTSEND);
		}
		if (ReadMME (plc, 1, (VS_DEBUG_INFO | MMTYPE_CNF)) <= 0) 
		{
			error (1, ECANCELED, CHANNEL_CANTSEND);
		}
		if (confirm->MSTATUS == 0x11) 
		{
			if (_allclr (plc->flags, PLC_SILENCE)) 
			{
				error ((plc->flags & PLC_BAILOUT), 0, "Slot %d has no data", slot);
			}
			continue;
		}
		if (confirm->MSTATUS) 
		{
			error (1, 0, "Device refused request for slot %d: %s", slot, MMECode (VS_DEBUG_INFO | MMTYPE_CNF, confirm->MSTATUS));
		}
		NumSNR [slot] = confirm->NUMSNR;
		BadMPDU [slot] = confirm->BADMPDUCNT;
		LongAvgPbErrRate [slot] = confirm->LONGAVGPBERRRATE;
		ShortAvgPbErrRate [slot] = confirm->SHORTAVGPBERRRATE;
		LongAvgBER [slot] = confirm->LONGAVGBER;
		ShortAvgBER [slot] = confirm->SHORTAVGBER;
		SNRMode [slot] = confirm->SNRMERGEMODE;
		TMTrigger [slot] = confirm->TM_TRIGGER;
		ReTrigger [slot] = confirm->RETRIGGERSOUNDCOMP;
		extent = LE32TOH (confirm->MME_LEN) - 64;
		if (extent > AMP_CARRIERS) 
		{
			error ((plc->flags & PLC_BAILOUT), EOVERFLOW, "Too many offsets");
			extent = AMP_CARRIERS;
		}
		plc->packetsize -= sizeof (* confirm);
		plc->packetsize += sizeof (confirm->SNRDATA);
		if (plc->packetsize > (signed)(extent)) 
		{
			plc->packetsize = extent;
		}
		memcpy (&SNR [slot] [offset], confirm->SNRDATA, plc->packetsize);
		offset += plc->packetsize;
		extent -= plc->packetsize;
		while (extent) 
		{
			if (ReadMME (plc, 1, (VS_DEBUG_INFO | MMTYPE_CNF)) <= 0) 
			{
				error (1, ECANCELED, CHANNEL_CANTSEND);
			}
			plc->packetsize -= sizeof (* fragment);
			plc->packetsize += sizeof (fragment->SNRDATA);
			if (plc->packetsize > (signed)(extent)) 
			{
				plc->packetsize = extent;
			}
			memcpy (&SNR [slot] [offset], fragment->SNRDATA, plc->packetsize);
			offset += plc->packetsize;
			extent -= plc->packetsize;
		}
	}
	return (0);
}


/*====================================================================*
 *
 *   void analyse (uint8_t slots, uint16_t carriers);
 *
 *   print SNR data for each slot and carrier;
 *
 *--------------------------------------------------------------------*/

static void analyse (uint8_t slots, uint16_t carriers) 

{
	uint32_t mean;
	uint16_t carrier;
	uint8_t slot;
	printf ("Value");
	for (slot = 0; slot < PLC_SLOTS; slot++) 
	{
		printf (",Slot%d", slot);
	}
	printf (",Mean\n");
	printf ("NumSNR");
	for (slot = mean = 0; slot < PLC_SLOTS; slot++) 
	{
		printf (DATAPOINT, NumSNR [slot]);
		if (slot < slots) 
		{
			mean += NumSNR [slot];
		}
	}
	printf (DATAPOINT "\n", (mean/slots));
	printf ("BadMPDU");
	for (slot = mean = 0; slot < PLC_SLOTS; slot++) 
	{
		printf (DATAPOINT, BadMPDU [slot]);
		if (slot < slots) 
		{
			mean += BadMPDU [slot];
		}
	}
	printf (DATAPOINT "\n", (mean/slots));
	printf ("LPbErr");
	for (slot = mean = 0; slot < PLC_SLOTS; slot++) 
	{
		printf (ERRORRATE, (float)(LongAvgPbErrRate [slot])/10);
		if (slot < slots) 
		{
			mean += LongAvgPbErrRate [slot];
		}
	}
	printf (ERRORRATE "\n", (float)(mean)/(slots*10));
	printf ("SPbErr");
	for (slot = mean = 0; slot < PLC_SLOTS; slot++) 
	{
		printf (ERRORRATE, (float)(ShortAvgPbErrRate [slot])/10);
		if (slot < slots) 
		{
			mean += ShortAvgPbErrRate [slot];
		}
	}
	printf (ERRORRATE "\n", (float)(mean)/(slots*10));
	printf ("LBER");
	for (slot = mean = 0; slot < PLC_SLOTS; slot++) 
	{
		printf (ERRORRATE, (float)(LongAvgBER [slot])/10);
		if (slot < slots) 
		{
			mean += LongAvgBER [slot];
		}
	}
	printf (ERRORRATE "\n", (float)(mean)/(slots*10));
	printf ("SBER");
	for (slot = mean = 0; slot < PLC_SLOTS; slot++) 
	{
		printf (ERRORRATE, (float)(ShortAvgBER [slot])/10);
		if (slot < slots) 
		{
			mean += ShortAvgBER [slot];
		}
	}
	printf (ERRORRATE "\n", (float)(mean)/(slots*10));
	printf ("SNRMode");
	for (slot = mean = 0; slot < PLC_SLOTS; slot++) 
	{
		printf (DATAPOINT, SNRMode [slot]);
		if (slot < slots) 
		{
			mean += SNRMode [slot];
		}
	}
	printf (DATAPOINT "\n", (mean/slots));
	printf ("TMTrig");
	for (slot = mean = 0; slot < PLC_SLOTS; slot++) 
	{
		printf (DATAPOINT, TMTrigger [slot]);
		if (slot < slots) 
		{
			mean += TMTrigger [slot];
		}
	}
	printf (DATAPOINT "\n", (mean/slots));
	printf ("ReTrig");
	for (slot = mean = 0; slot < PLC_SLOTS; slot++) 
	{
		printf (DATAPOINT, ReTrigger [slot]);
		if (slot < slots) 
		{
			mean += ReTrigger [slot];
		}
	}
	printf (DATAPOINT "\n", (mean/slots));
	printf ("Freq");
	for (slot = 0; slot < PLC_SLOTS; slot++) 
	{
		printf (",Slot%d", slot);
	}
	printf (",Mean\n");
	for (carrier = 0; carrier < carriers; carrier++) 
	{
		printf (FREQUENCY, INDEX_TO_FREQ (carrier));
		for (slot = mean = 0; slot < PLC_SLOTS; slot++) 
		{
			printf (ATTENUATION, db2snr (SNR [slot][carrier]));
			if (slot < slots) 
			{
				SNRMean [slot] += SNR [slot] [carrier];
				mean += SNR [slot][carrier];
			}
		}
		printf (ATTENUATION "\n", db2snr (mean/slots));
	}
	printf ("     ");
	for (slot = mean = 0; slot < PLC_SLOTS; slot++) 
	{
		printf (ATTENUATION, db2snr (SNRMean [slot]/carriers));
		if (slot < slots) 
		{
			mean += SNRMean [slot];
		}
	}
	printf (ATTENUATION "\n", db2snr (mean/slots/carriers));
	return;
}


/*====================================================================*
 *
 *   void display (uint8_t slots, uint16_t carriers);
 *
 *   print raw SNR data for each slot and carrier; this is similar
 *   to function analyse () but displays data without converion or
 *   averaging;
 *
 *--------------------------------------------------------------------*/

static void display (uint8_t slots, uint16_t carriers) 

{
	uint16_t carrier;
	for (carrier = 0; carrier < carriers; carrier++) 
	{
		uint8_t slot;
		printf ("%04d", carrier);
		for (slot = 0; slot < slots; slot++) 
		{
			printf (",%03d", SNR [slot][carrier]);
		}
		printf ("\n");
	}
	return;
}


/*====================================================================*
 *   
 *   int main (int argc, char const * argv[]);
 *   
 *   parse command line, populate plc structure and perform selected 
 *   operations; show help summary if asked; see getoptv and putoptv
 *   to understand command line parsing and help summary display; see
 *   plc.h for the definition of struct plc; 
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
 *.  Qualcomm Atheros HomePlug AV Powerline Toolkit
 *:  Published 2009-2011 by Qualcomm Atheros. ALL RIGHTS RESERVED
 *;  For demonstration and evaluation only. Not for production use
 *
 *--------------------------------------------------------------------*/

int main (int argc, char const * argv []) 

{
	extern struct channel channel;
	extern const struct _term_ devices [];
	static char const *optv [] = 
	{
		"abc:ei:p:qs:t:vx",
		"node peer [> stdout]",
		"Qualcomm Atheros AR7x00 SNR Monitor",
		"a\tformat output for spreadsheet analysis",
		"b\tbrief settings (-s5 -c2690)",
		"c n\tlimit to first (n) carriers [" OPTSTR (AMP_CARRIERS) "]",
		"e\tredirect stderr messages to stdout",

#if defined (WINPCAP) || defined (LIBPCAP)

		"i n\thost interface is (n) [" OPTSTR (CHANNEL_ETHNUMBER) "]",

#else

		"i s\thost interface is (s) [" CHANNEL_ETHDEVICE "]",

#endif

		"p n\tcoupling [" OPTSTR (DEFAULT_COUPLING) "]",
		"q\tquiet mode",
		"s n\tlimit to first (n) slots [" OPTSTR (PLC_SLOTS) "]",
		"t n\tread timeout is (n) milliseconds [" OPTSTR (CHANNEL_TIMEOUT) "]",
		"v\tverbose mode",
		"x\texit on error",
		(char const *) (0)
	};

#include "../plc/plc.c"

	void (* function) (uint8_t, uint16_t) = display;
	uint16_t carriers = AMP_CARRIERS;
	uint8_t slots = PLC_SLOTS;
	signed c;
	plc.hostaction = DEFAULT_COUPLING;
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
		switch (c) 
		{
		case 'a':
			function = analyse;
			break;
		case 'b':
			carriers = 2690;
			slots = 5;
			break;
		case 'c':
			carriers = (uint16_t)(uintspec (optarg, 0, AMP_CARRIERS));
			break;
		case 'e':
			dup2 (STDOUT_FILENO, STDERR_FILENO);
			break;
		case 'i':

#if defined (WINPCAP) || defined (LIBPCAP)

			channel.ifindex = atoi (optarg);

#else

			channel.ifname = optarg;

#endif

			break;
		case 'p':
			plc.hostaction = (unsigned)(uintspec (synonym (optarg, couple, COUPLE), 0, UCHAR_MAX));
			break;
		case 'q':
			_setbits (channel.flags, CHANNEL_SILENCE);
			_setbits (plc.flags, PLC_SILENCE);
			break;
		case 's':
			slots = (byte)(uintspec (optarg, 1, PLC_SLOTS));
			break;
		case 't':
			channel.timeout = (unsigned)(uintspec (optarg, 0, UINT_MAX));
			break;
		case 'v':
			_setbits (channel.flags, CHANNEL_VERBOSE);
			_setbits (plc.flags, PLC_VERBOSE);
			break;
		case 'x':
			_setbits (plc.flags, PLC_BAILOUT);
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

	if (!argc || !argv) 
	{
		error (1, ECANCELED, "No node address given");
	}
	if (!hexencode (channel.peer, sizeof (channel.peer), synonym (*argv, devices, DEVICES))) 
	{
		error (1, errno, PLC_BAD_MAC, *argv);
	}
	argc--;
	argv++;
	if ((!argc) || (!argv)) 
	{
		error (1, ECANCELED, "No peer address given");
	}
	if (!hexencode (plc.RDA, sizeof (plc.RDA), synonym (*argv, devices, DEVICES))) 
	{
		error (1, errno, PLC_BAD_MAC, *argv);
	}
	argc--;
	argv++;
	openchannel (&channel);
	if (!(plc.message = malloc (sizeof (struct message)))) 
	{
		error (1, errno, PLC_NOMEMORY);
	}
	memset (NumSNR, 0, sizeof (NumSNR));
	memset (BadMPDU, 0, sizeof (BadMPDU));
	memset (LongAvgPbErrRate, 0xFE, sizeof (LongAvgPbErrRate));
	memset (ShortAvgPbErrRate, 0, sizeof (ShortAvgPbErrRate));
	memset (LongAvgBER, 0, sizeof (LongAvgBER));
	memset (ShortAvgBER, 0, sizeof (ShortAvgBER));
	memset (SNRMode, 0, sizeof (SNRMode));
	memset (SNRMean, 0, sizeof (SNRMean));
	memset (TMTrigger, 0, sizeof (TMTrigger));
	memset (ReTrigger, 0, sizeof (ReTrigger));
	memset (AGC, 0, sizeof (AGC));
	memset (SNR, 0, sizeof (SNR));
	collect (&plc, slots, carriers);
	function (slots, carriers);
	free (plc.message);
	closechannel (&channel);
	return (0);
}

