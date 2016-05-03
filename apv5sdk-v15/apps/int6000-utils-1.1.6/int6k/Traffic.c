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
 *   signed Traffic (struct channel * channel, unsigned period, unsigned repeat);
 *   
 *   int6k.h
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

#ifndef TRAFFIC_SOURCE
#define TRAFFIC_SOURCE

#include <memory.h>
#include <errno.h>

#include "../int6k/message.h"
#include "../int6k/intellon.h"
#include "../int6k/homeplug.h"
#include "../int6k/problem.h"
#include "../ether/channel.h"
#include "../tools/memory.h"
#include "../tools/error.h"
#include "../tools/flags.h"

/*====================================================================*
 *
 *   signed Traffic1 (struct channel * channel, const byte remote [], unsigned count);
 *
 *   send send a given number of empty TCP frames to some powerline 
 *   device; 
 *
 *.  Intellon INT6x00 Linux Toolkit for HomePlug AV; 
 *:  Published 2007 by Intellon Corp. ALL RIGHTS RESERVED;
 *;  For demonstration; Not for production use;
 *
 *--------------------------------------------------------------------*/

static string1 [24];
static string2 [24];

static signed Traffic1 (struct channel * channel, const byte remote [], unsigned count) 

{
	struct message message;	printf ("send %d frames from %s to %s\n", count, hexstring (string1, sizeof (string1), channel->host, sizeof (channel->host)), hexstring (string2, sizeof (string2), remote, 6));

	memset (&message, 0, sizeof (message));
	memcpy (&message.ethernet.ODA, remote, sizeof (message.ethernet.ODA));
	memcpy (&message.ethernet.OSA, channel->host, sizeof (message.ethernet.OSA));
	message.ethernet.MTYPE = htons (ETHERTYPE_IP);
	printf ("send %s to %s\n", count, hexstring (string1, sizeof (string1), channel->host, sizeof (channel->host)), hexstring (string2, sizeof (string2), remote, 6));
	while (count--) 
	{
		if (sendpacket (channel, &message, sizeof (message)) <= 0) 
		{
			error (1, ECANCELED, INT6K_CANTSEND);
		}
	}
	return (0);
}

/*====================================================================*
 *
 *   signed Traffic2 (struct channel * channel, const byte remote [], unsigned period);
 *
 *   instruct a remote powerline device to echo empty TCP frames to
 *   the local device for a given period of time in seconds;
 *
 *.  Intellon INT6x00 Linux Toolkit for HomePlug AV; 
 *:  Published 2007 by Intellon Corp. ALL RIGHTS RESERVED;
 *;  For demonstration; Not for production use;
 *
 *--------------------------------------------------------------------*/

static signed Traffic2 (struct channel * channel, const byte remote [], unsigned period) 

{
	struct message message;

#ifndef __GNUC__
#pragma pack (push,1)
#endif

	struct __packed vs_fr_lbk_request 
	{
		struct header_eth ethernet;
		struct header_int intellon;
		uint8_t DURATION;
		uint8_t RESERVED;
		uint16_t LENGTH;
		uint8_t PACKET [1038];
	}
	* request = (struct vs_fr_lbk_request *) (&message);
	struct header_eth * header_eth = (struct header_eth *)(&request->PACKET);

#ifndef __GNUC__
#pragma pack (pop)
#endif

	memset (&message, 0, sizeof (message));
	printf ("send frames %s to %s\n", hexstring (string1, sizeof (string1), channel->host, sizeof (channel->host)), hexstring (string2, sizeof (string2), remote, 6));
	EthernetHeader (&message.ethernet, remote, channel->host);
	IntellonHeader (&message.intellon, (VS_FR_LBK | MMTYPE_REQ));
	request->DURATION = period;
	request->LENGTH = HTOLE16 (sizeof (request->PACKET));
	memcpy (&header_eth->ODA, channel->peer, sizeof (header_eth->ODA));
	memcpy (&header_eth->OSA, remote, sizeof (header_eth->OSA));
	header_eth->MTYPE = htons (ETHERTYPE_IP);
	if (sendpacket (channel, &message, sizeof (message)) <= 0) 
	{
		error (1, ECANCELED, INT6K_CANTSEND);
	}
	while (readpacket (channel, &message, sizeof (message)) > 0) 
	{
		if (UnwantedPacket (&message, (VS_FR_LBK | MMTYPE_REQ))) 
		{
			continue;
		}
		break;
	}
	return (0);
}

/*====================================================================*
 *
 *   signed Traffic (struct channel * channel, unsigned period, unsigned repeat);
 *   
 *   generate bi-directional traffic between a local device and all
 *   remote devices on a powerline network; remote devices need not
 *   be connected; no traffic is generated between remote devices;
 *
 *   this function is similar to NetworkTraffic but takes less time
 *   since not all powerline paths are exercised and those that are
 *   can be exercised in parallel; 
 *   
 *.  Intellon INT6x00 Linux Toolkit for HomePlug AV; 
 *:  Published 2007 by Intellon Corp. ALL RIGHTS RESERVED;
 *;  For demonstration; Not for production use;
 *
 *--------------------------------------------------------------------*/

signed Traffic (struct channel * channel, unsigned period, unsigned repeat) 

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

	byte bridges [255] [ETHER_ADDR_LEN];
	unsigned bridge = 0;
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
		if (!UnwantedPacket (&message, (VS_SW_VER | MMTYPE_CNF))) 
		{
			memcpy (bridges [bridge++], message.ethernet.OSA, ETHER_ADDR_LEN);
		}
	}
	while (bridge--) 
	{
		memset (&message, 0, sizeof (message));
		memcpy (channel->peer, bridges [bridge], sizeof (channel->peer));
		EthernetHeader (&message.ethernet, channel->peer, channel->host);
		IntellonHeader (&message.intellon, (VS_NW_INFO | MMTYPE_REQ));
		if (sendpacket (channel, &message, ETHER_MIN_LEN) <= 0) 
		{
			error (1, errno, INT6K_CANTSEND);
		}
		while (readpacket (channel, &message, sizeof (message)) > 0) 
		{
			if (UnwantedPacket (&message, (VS_NW_INFO | MMTYPE_CNF))) 
			{
				continue;
			}
			network = (struct network *)(&networks->network);
			while (networks->NUMAVLNS-- > 0) 
			{
				station = (struct station *)(&network->stations);
				while (network->NUMSTAS-- > 0) 
				{
					Traffic1 (channel, station->LDA, repeat);
					Traffic2 (channel, station->LDA, period);
					station++;
				}
				network = (struct network *)(station);
			}
		}
		sleep (period + 1);
	}
	return (0);
}

#endif

