/*====================================================================*
 *
 *   showsta.c - Atheros Ethernet remote USERs of slave showing;
 *   
 *
 *   this program sends and receives raw ethernet frames and so needs
 *   root priviledges; if you install it using "chmod 555" and "chown 
 *   root:root" then you must login as root to run it; otherwise, you 
 *   can install it using "chmod 4555" and "chown root:root" so that 
 *   anyone can run it; the program will refuse to run until you get
 *   things right;
 *   
 *   This software and documentation is the property of Atheros 
 *   Corporation, Ocala, Florida. It is provided 'as is' without 
 *   expressed or implied warranty of any kind to anyone for any 
 *   reason. Atheros assumes no responsibility or liability for 
 *   errors or omissions in the software or documentation and 
 *   reserves the right to make changes without notification. 
 *   
 *   Atheros customers may modify and distribute the software 
 *   without obligation to Atheros. Since use of this software 
 *   is optional, users shall bear sole responsibility and 
 *   liability for any consequences of it's use. 
 *
 *. Atheros Powerline Toolkit for HomePlug AV; 
 *:  Published 2007 by Intellon Corp. ALL RIGHTS RESERVED;
 *;  For demonstration; Not for production use;
 *
 *   Contributor(s):
 *      Charles Maier <charles.maier@atheros.com>
 *
 *--------------------------------------------------------------------*/

#define _GETOPT_H

/*====================================================================*"
 *   system header files;
 *--------------------------------------------------------------------*/

#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>

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
#include "../plc/plc.h"
#include "../ether/channel.h"
#include "../mme/mme.h"

/*====================================================================*
 *   custom source files;
 *--------------------------------------------------------------------*/

#ifndef MAKEFILE
//#include "../mme/HomeplugHeader.c"
//#include "../mme/IntellonHeader.c"
#include "../mme/UnwantedHomeplugMessage.c"
#include "../int6k/Devices.c"
#endif

#ifndef MAKEFILE
//#include "../tools/alert.c"
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
#include "../tools/checkfilename.c"
#include "../tools/checksum32.c"
#include "../tools/fdchecksum32.c"
#include "../tools/strfbits.c"
#include "../tools/filepart.c"
#include "../tools/typename.c"
#include "../tools/lookup.c"
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
#endif


/*====================================================================*
 *   program variables;
 *--------------------------------------------------------------------*/

/*====================================================================*
 *
 *   signed PHYSettings (struct channel * channel, struct phy_settings * settings, flag_t flags);
 *
 *   int6k.h
 *
 *   read and display Ethernet PHY settings or write then read and
 *   display settings;
 *
 *   this program sends and receives raw ethernet frames and so needs
 *   root priviledges; if you install it using "chmod 555" and "chown 
 *   root:root" then you must login as root to run it; otherwise, you 
 *   can install it using "chmod 4555" and "chown root:root" so that 
 *   anyone can run it; the program will refuse to run until you get
 *   things right;
 *   
 *   This software and documentation is the property of Atheros 
 *   Corporation, Ocala, Florida. It is provided 'as is' without 
 *   expressed or implied warranty of any kind to anyone for any 
 *   reason. Atheros assumes no responsibility or liability for 
 *   errors or omissions in the software or documentation and 
 *   reserves the right to make changes without notification. 
 *   
 *   Atheros customers may modify and distribute the software 
 *   without obligation to Atheros. Since use of this software 
 *   is optional, users shall bear sole responsibility and 
 *   liability for any consequences of it's use. 
 *
 *. Atheros Powerline Toolkit for HomePlug AV; 
 *:  Published 2007 by Intellon Corp. ALL RIGHTS RESERVED;
 *;  For demonstration; Not for production use;
 *
 *   Contributor(s):
 *      Charles Maier <charles.maier@atheros.com>
 *
 *--------------------------------------------------------------------*/

#ifndef __GNUC__
#pragma pack (push,1)
#endif

static uint8_t VAR = 0;

typedef struct __packed brg_settings 
{
	uint8_t FMI_NUM;
	uint8_t FMSN;
	uint8_t VAR;
}
brg_settings;

uint8_t readmode = 0;
#define DM mesgdmac
uint8_t mesgdmac[ETHER_ADDR_LEN];

#ifndef __GNUC__
#pragma pack (pop)
#endif

signed PHYSettings (struct channel * channel, struct brg_settings  * settings, flag_t flags) 

{
	signed packetsize;

#ifndef __GNUC__
#pragma pack (push,1)
#endif

	typedef struct __packed FMI
	{
		uint8_t NFFN;
		uint8_t FMSN;
		
	}FMI;
	
	typedef struct __packed homeplug_std
	{
		uint8_t ODA[ETHER_ADDR_LEN];
		uint8_t OSA[ETHER_ADDR_LEN];
		uint16_t MTYPE;
		uint8_t MMV;
		uint16_t MMTYPE;
		struct FMI FMI;

	}homeplug_std;

	typedef struct __packed homeplug_message
	{
		struct homeplug_std homeplugdata;
		uint8_t MMENTRY[VAR];
		uint8_t MME_PAD[46-VAR];
		uint8_t BDA[ETHER_ADDR_LEN * 64];

	}homeplug_message;

	struct homeplug_message message;


	typedef struct __packed brgInfo
	{
		uint8_t BTEI;
		uint8_t NBDA;
		uint8_t BDA[ETHER_ADDR_LEN * 64];
		
	}brgInfo;

	typedef struct __packed brg_response
	{
		uint8_t BSF;
		struct brgInfo BIVF; 
	}brg_response;

	struct __packed vs_brg_readings_confirm 
	{
		struct homeplug_std homeplugdata;
		struct brg_response brgresponse;
	}
	* confirm = (struct vs_brg_readings_confirm *) (&message) ;

#ifndef __GNUC__
#pragma pack (pop)
#endif

	uint8_t tmp_address[ETHER_ADDR_LEN*3];
	char address [ETHER_ADDR_LEN * 3];
	char listaddress [ETHER_ADDR_LEN * 3];
	memset (&message, 0, sizeof (message));
	
	memcpy(message.homeplugdata.OSA ,channel->host, ETHER_ADDR_LEN);
	memcpy(message.homeplugdata.ODA, mesgdmac, ETHER_ADDR_LEN);
	message.homeplugdata.MTYPE = htons (HOMEPLUG_MTYPE);
	message.homeplugdata.MMV = 1;
	message.homeplugdata.MMTYPE = HTOLE16(CM_BRG_INFO | MMTYPE_REQ);
	
	message.homeplugdata.FMI.NFFN = settings->FMI_NUM;
	message.homeplugdata.FMI.FMSN = settings->FMSN;				
	
	if (sendpacket (channel, &message, (ETHER_MIN_LEN - ETHER_CRC_LEN)) <= 0) 
	{
		error (1, ECANCELED, CHANNEL_CANTSEND);
	}
	//printf("send byte %d\n",temp);	
	static uint8_t  frag = 0;

	hexstring((char *)tmp_address, sizeof(tmp_address), message.homeplugdata.ODA, sizeof(message.homeplugdata.OSA));	
	if(strcmp((char *)tmp_address, "00:B0:52:00:00:01") == 0 || strcmp((char *)tmp_address, "FF:FF:FF:FF:FF:FF") == 0)
	{		
		while ((packetsize = readpacket (channel, &message, sizeof(message))) > 0  ) 
		{
			
			if (UnwantedHomeplugMessage (&message, confirm->homeplugdata.MMV, (CM_BRG_INFO | MMTYPE_CNF), channel)) 
			{
				continue;
			}
			if ((confirm->brgresponse.BSF == 0)) 
			{
				error (0, 0, "%s (%0X): ", MMECode (confirm->brgresponse.BSF, 1), 1);
				continue;
			}
		
			if(((confirm->homeplugdata.FMI.NFFN >> 4 )& 0x0f)>confirm->homeplugdata.FMI.FMSN)
			{
				printf("error:the number of fragment is mistaken, send the request again!\n");
				exit(1);
			}
			if((confirm->homeplugdata.FMI.NFFN & 0x0f )!= frag)
			{
				printf("error:the sequence of fragment is mistaken, send the request again!\n");
				frag ++;
				exit(1);
			}
			
			printf ("%s %s ", channel->ifname,	hexstring (address, sizeof(address), message.homeplugdata.OSA, sizeof(mesgdmac)));
			printf("	SUM %d	  STEI %d\n", confirm->brgresponse.BIVF.NBDA, confirm->brgresponse.BIVF.BTEI);
			if(confirm->brgresponse.BIVF.NBDA != 0)
			{
				uint8_t *p = confirm->brgresponse.BIVF.BDA;
				uint8_t SUM = confirm->brgresponse.BIVF.NBDA;
				uint8_t num = 0;
				while((num++ < SUM) && (1 == readmode))
				{
					printf("No.%d %s\n", num, hexstring (listaddress, sizeof(listaddress), p, sizeof(mesgdmac)));
					p = p + 6;
				}
			}
		}
	}
	else 
	while ((packetsize = readpacket (channel, &message, sizeof(message))) > 0  ) 
	{
		
		if (UnwantedHomeplugMessage (&message, confirm->homeplugdata.MMV, (CM_BRG_INFO | MMTYPE_CNF), channel)) 
		{
			continue;
		}
		if ((confirm->brgresponse.BSF == 0)) 
		{
			error (0, 0, "%s (%0X): ", MMECode (confirm->brgresponse.BSF, 1), 1);
			continue;
		}

		if(((confirm->homeplugdata.FMI.NFFN >> 4 )& 0x0f)>confirm->homeplugdata.FMI.FMSN)
		{
			printf("error:the number of fragment is mistaken, send the request again!\n");
			exit(1);
		}
		if((confirm->homeplugdata.FMI.NFFN & 0x0f )!= frag)
		{
			printf("error:the sequence of fragment is mistaken, send the request again!\n");
			frag ++;
			exit(1);
		}
		
		printf ("%s %s ", channel->ifname,	hexstring (address, sizeof(address), message.homeplugdata.OSA, sizeof(mesgdmac)));
		printf("	SUM %d	  STEI %d\n", confirm->brgresponse.BIVF.NBDA, confirm->brgresponse.BIVF.BTEI);
		if(confirm->brgresponse.BIVF.NBDA != 0)
		{
			uint8_t *p = confirm->brgresponse.BIVF.BDA;
			uint8_t SUM = confirm->brgresponse.BIVF.NBDA;
			uint8_t num = 0;
			while((num++ < SUM) && (1 == readmode))
			{
				printf("No.%d %s\n", num, hexstring (listaddress, sizeof(listaddress), p, sizeof(mesgdmac)));
				p = p + 6;
			}
		}
		break;
	}
	if (packetsize < 0) 
	{
		error (1, ECANCELED, CHANNEL_CANTREAD);
	}


		
	return (0);
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
 *.  Atheros Powerline Toolkit for HomePlug AV; 
 *:  Published 2007 by Intellon Corp. ALL RIGHTS RESERVED;
 *;  For demonstration; Not for production use;
 *
 *--------------------------------------------------------------------*/

int main (int argc, const char * argv []) 

{
	extern struct channel channel;
	static const char *optv [] = 
	{
		"a:d:i:rv",
		"device [device] [...] [> stdout]",
		"Atheros Ethernet PHY Settings",

#if defined (WINPCAP) || defined (LIBPCAP)

		"i n\thost interface number [2]",

#else

		"i s\thost interface name [" CHANNEL_ETHDEVICE "]",

#endif

		"d s\tdestination MAC ",
		"r\tread settings from device",
		"v\tverbose mode",
		(const char *) (0)
	};
	struct brg_settings settings = 
	{
		0,
		0,
		0
	};
	flag_t flags = (flag_t)(0);
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

		case 'i':

#if defined (WINPCAP) || defined (LIBPCAP)

			channel.ifindex = atoi (optarg);

#else

			channel.ifname = optarg;

#endif

			break;
		case 'r':
			readmode = 1;
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

	if (!argc || !argv) 
	{
		error (1, ECANCELED, "No dest address given");
	}
	if (!hexencode (mesgdmac, ETHER_ADDR_LEN, synonym (* argv, devices, SIZEOF (devices)))) 
	{
		error (1, errno, PLC_BAD_MAC, *argv);
	}
	argc--;
	argv++;
	
	openchannel (&channel);
	if (!argc) 
	{
		PHYSettings (&channel, &settings, flags);
	}
	while ((argc) && (*argv)) 
	{
		if (!hexencode (channel.peer, sizeof (channel.peer), synonym (* argv, devices, SIZEOF (devices)))) 
		{
			error (1, errno, PLC_BAD_MAC, *argv);
		}
		PHYSettings (&channel, &settings, flags);
		argc--;
		argv++;
	}
	closechannel (&channel);
	exit (0);
}

