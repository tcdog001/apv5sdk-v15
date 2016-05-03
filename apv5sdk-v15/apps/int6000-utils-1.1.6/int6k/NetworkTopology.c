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
 *   signed NetworkTopology (struct channel * channel);
 *   
 *   int6k.h
 *
 *   display network topology on stdout; the topology shows devices
 *   associated with each local device along with the tx/rx bit rates
 *   between the target device and each device and the hardware and
 *   firmware revison of each device; the target device is always 
 *   the first one listed;
 *
 *   this function is a variation on function NetworkInfo but shows
 *   more information; 
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

#ifndef NETWORKTOPOLOGY_SOURCE
#define NETWORKTOPOLOGY_SOURCE

#include <memory.h>
#include <errno.h>

#include "../int6k/int6k.h"
#include "../ether/channel.h"
#include "../tools/memory.h"
#include "../tools/error.h"
#include "../tools/flags.h"

static signed Platform (struct channel * channel, const uint8_t device []) 

{
	extern const char * chipset [CHIPSETS+1];
	struct message message;

#ifndef __GNUC__
#pragma pack (push,1)
#endif

	struct __packed vs_sw_ver_confirm 
	{
		struct header_eth ethernet;
		struct header_int intellon;
		uint8_t MSTATUS;
		uint8_t MDEVICEID;
		uint8_t MVERLENGTH;
		char MVERSION [INT6K_VERSTRING];
	}
	* confirm = (struct vs_sw_ver_confirm *) (&message);

#ifndef __GNUC__
#pragma pack (pop)
#endif

	memset (&message, 0, sizeof (message));
	EthernetHeader (&message.ethernet, device, channel->host);
	IntellonHeader (&message.intellon, (VS_SW_VER | MMTYPE_REQ));
	if (sendpacket (channel, &message, ETHER_MIN_LEN) <= 0) 
	{
//		error (1, errno, INT6K_CANTSEND);
		return (0);
	}
	if (readpacket (channel, &message, sizeof (message)) <= 0) 
	{
//		error (1, errno, INT6K_CANTREAD);
		return (0);
	}
	if (confirm->MDEVICEID > CHIPSETS) 
	{
		confirm->MDEVICEID = 0;
	}
	printf ("  %s", chipset [confirm->MDEVICEID]);
	printf ("  %s", confirm->MVERSION);
	return (0);
}

signed NetworkTopology (struct channel * channel) 

{
	extern const byte localcast [ETHER_ADDR_LEN];
	struct message message;

#ifndef __GNUC__
#pragma pack (push,1)
#endif

	struct __packed station 
	{
		uint8_t LDA [ETHER_ADDR_LEN];
		uint8_t TEI;
		uint8_t BDA [ETHER_ADDR_LEN];
		uint8_t AVGTX;
		uint8_t AVGRX;
	}
	* station;
	struct __packed network 
	{
		uint8_t NID [7];
		uint8_t SNID;
		uint8_t TEI;
		uint8_t ROLE;
		uint8_t CCO_MAC [ETHER_ADDR_LEN];
		uint8_t CCO_TEI;
		uint8_t NUMSTAS;
		struct station stations [1];
	}
	* network;
	struct __packed networks 
	{
		uint8_t NUMAVLNS;
		struct network network [1];
	}
	* networks = (struct networks *) (&message.content);

#ifndef __GNUC__
#pragma pack (pop)
#endif

	char address [ETHER_ADDR_LEN * 3];
	byte bridges [255] [ETHER_ADDR_LEN];
	unsigned bridge = 0;
	const char * role;
	const char * link;
	memset (&message, 0, sizeof (message));
	memcpy (channel->peer, localcast, sizeof (channel->peer));
	EthernetHeader (&message.ethernet, channel->peer, channel->host);
	IntellonHeader (&message.intellon, (VS_SW_VER | MMTYPE_REQ));
	if (sendpacket (channel, &message, ETHER_MIN_LEN) <= 0) 
	{
		error (1, errno, INT6K_CANTSEND);
	}
	while (readpacket (channel, &message, sizeof (message)) > 0) 
	{
		if (UnwantedPacket (&message, (VS_SW_VER | MMTYPE_CNF))) 
		{
			continue;
		}
		memcpy (bridges [bridge++], message.ethernet.OSA, ETHER_ADDR_LEN);
	}
	while (bridge--) 
	{
		memset (&message, 0, sizeof (message));
		memcpy (channel->peer, bridges [bridge], sizeof (channel->peer));
		EthernetHeader (&message.ethernet, channel->peer, channel->host);
		IntellonHeader (&message.intellon, (VS_NW_INFO | MMTYPE_REQ));
		if (sendpacket (channel, &message, ETHER_MIN_LEN) <= 0) 
		{
			error (1, errno, INT6:wqK_CANTSEND);
		}
		while (readpacket (channel, &message, sizeof (message)) > 0) 
		{
			if (UnwantedPacket (&message, (VS_NW_INFO | MMTYPE_CNF))) 
			{
				continue;
			}
			if (_allclr (channel->flags, INT6K_SILENCE)) 
			{
				printf (" P/L NET TEI ------ MAC ------ ------ BDA ------  TX  RX CHIPSET FIRMWARE\n");
			}
			network = (struct network *)(&networks->network);
			link = memcmp (message.ethernet.OSA, bridges [bridge], ETHER_ADDR_LEN)? "REM": "LOC";
			role = memcmp (message.ethernet.OSA, network->CCO_MAC, ETHER_ADDR_LEN)? "STA": "CCO";
			printf ("  %s  %s", link, role);
			printf ("  %03d", network->TEI);
			printf ("  %s", hexstring (address, sizeof (address), message.ethernet.OSA, ETHER_ADDR_LEN));
			printf ("  %s", hexstring (address, sizeof (address), message.ethernet.ODA, ETHER_ADDR_LEN));
			printf ("  n/a");
			printf ("  n/a");
			Platform (channel, message.ethernet.OSA);
			printf ("\n");
			while (networks->NUMAVLNS-- > 0) 
			{
				station = (struct station *)(&network->stations);
				while (network->NUMSTAS-- > 0) 
				{
					link = memcmp (station->BDA, bridges [bridge], ETHER_ADDR_LEN)? "REM": "LOC";
					role = memcmp (station->LDA, network->CCO_MAC, ETHER_ADDR_LEN)? "STA": "CCO";
					printf ("  %s  %s", link, role);
					printf ("  %03d", station->TEI);
					printf ("  %s", hexstring (address, sizeof (address), station->LDA, ETHER_ADDR_LEN));
					printf ("  %s", hexstring (address, sizeof (address), station->BDA, ETHER_ADDR_LEN));
					printf ("  %03d", station->AVGTX);
					printf ("  %03d", station->AVGRX);
					Platform (channel, station->LDA);
					printf ("\n");
					station++;
				}
				network = (struct network *)(station);
			}
		}
	}
	return (0);
}

#endif

