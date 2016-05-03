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
 *   signed NetworkTraffic (struct int6k * int6k);
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

#ifndef NETWORKTRAFFIC_SOURCE
#define NETWORKTRAFFIC_SOURCE

#include <memory.h>
#include <errno.h>

#include "../int6k/int6k.h"
#include "../int6k/problem.h"
#include "../tools/memory.h"
#include "../tools/number.h"
#include "../tools/error.h"
#include "../tools/flags.h"

/*====================================================================*
 *   constants;
 *--------------------------------------------------------------------*/

#define DATALEN 1038

/*====================================================================*
 * 
 *   void Report (const byte device [], const byte remove []);
 *
 *   print source and target Ethernet addresses on stdout;
 *
 *.  Intellon INT6x00 Linux Toolkit for HomePlug AV; 
 *:  Published 2007 by Intellon Corp. ALL RIGHTS RESERVED;
 *;  For demonstration; Not for production use;
 *
 *--------------------------------------------------------------------*/

static void Report (struct int6k * int6k, const byte device [], const byte remote []) 

{
	char source [ETHER_ADDR_LEN * 3];
	char target [ETHER_ADDR_LEN * 3];
	if (_allclr (int6k->flags, INT6K_SILENCE))
	{
		hexdecode (remote, ETHER_ADDR_LEN, source, sizeof (source));
		hexdecode (device, ETHER_ADDR_LEN, target, sizeof (target));
		fprintf (stderr, "%s %s %s\n", int6k->channel->name, source, target);
	}
	return;
}

/*====================================================================*
 *   
 *   signed NetworkTraffic1 (struct int6k * int6k, const byte device [], const byte remote []);
 *
 *   instruct a remote powerline device to send an empty TCP frame to 
 *   another powerline device for a fixed period of time;
 *   
 *.  Intellon INT6x00 Linux Toolkit for HomePlug AV; 
 *:  Published 2007 by Intellon Corp. ALL RIGHTS RESERVED;
 *;  For demonstration; Not for production use;
 *
 *--------------------------------------------------------------------*/

static void NetworkTraffic1 (struct int6k * int6k, const byte device [], const byte remote []) 

{
	struct channel * channel = (struct channel *)(int6k->channel);
	struct message * message = (struct message *)(int6k->message);

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
		uint8_t DATA [DATALEN];
	}
	* request = (struct vs_fr_lbk_request *) (message);
	struct __packed vs_fr_lbk_confirm 
	{
		struct header_eth ethernet;
		struct header_int intellon;
		uint8_t MSTATUS;
		uint8_t DURATION;
		uint16_t LENGTH;
	}
	* confirm = (struct vs_fr_lbk_confirm *) (message);

#ifndef __GNUC__
#pragma pack (pop)
#endif

	Report (int6k, remote, device);
	memset (message, 0, sizeof (struct message));
	EthernetHeader (&message->ethernet, remote, channel->host);
	IntellonHeader (&message->intellon, (VS_FR_LBK | MMTYPE_REQ));
	request->DURATION = int6k->timeperiod;
	request->LENGTH = HTOLE16 (DATALEN);
	ProtocolHeader (&request->DATA, device, remote, ETHERTYPE_IP);
	if (sendpacket (channel, message, sizeof (struct vs_fr_lbk_request)) <= 0) 
	{
		error (1, ECANCELED, INT6K_CANTSEND);
	}
	while (readpacket (channel, message, sizeof (struct message)) > 0) 
	{
		if (UnwantedPacket (message, (VS_FR_LBK | MMTYPE_CNF))) 
		{
			continue;
		}
		if (confirm->MSTATUS) 
		{
			error (0, ECANCELED, "Device refused request");
			return;
		}
		break;
	}
	sleep (int6k->timeperiod + 1);
	return;
}

/*====================================================================*
 *   
 *   signed NetworkTraffic2 (struct int6k * int6k, const byte device [], const byte remote []);
 *   
 *   instruct a remote powerline device to instruct another powerline 
 *   device to send back an empty TCP frame for a fixed period of time;
 *
 *.  Intellon INT6x00 Linux Toolkit for HomePlug AV; 
 *:  Published 2007 by Intellon Corp. ALL RIGHTS RESERVED;
 *;  For demonstration; Not for production use;
 *
 *--------------------------------------------------------------------*/

static void NetworkTraffic2 (struct int6k * int6k, const byte device [], const byte remote []) 

{
	struct channel * channel = (struct channel *)(int6k->channel);
	struct message * message = (struct message *)(int6k->message);

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
		uint8_t DATA [DATALEN];
	}
	* request = (struct vs_fr_lbk_request *) (message);
	struct __packed vs_fr_lbk_confirm 
	{
		struct header_eth ethernet;
		struct header_int intellon;
		uint8_t MSTATUS;
		uint8_t DURATION;
		uint16_t LENGTH;
	}
	* confirm = (struct vs_fr_lbk_confirm *) (message);

#ifndef __GNUC__
#pragma pack (pop)
#endif

	Report (int6k, device, remote);
	memset (message, 0, sizeof (struct message));
	EthernetHeader (&message->ethernet, remote, channel->host);
	IntellonHeader (&message->intellon, (VS_FR_LBK | MMTYPE_REQ));
	request->DURATION = 1;
	request->LENGTH = HTOLE16 (DATALEN);
	request = (struct vs_fr_lbk_request *)(&request->DATA);
	EthernetHeader (&request->ethernet, device, remote);
	IntellonHeader (&request->intellon, (VS_FR_LBK | MMTYPE_REQ));
	request->DURATION = int6k->timeperiod;
	request->LENGTH = HTOLE16 (DATALEN-24);
	ProtocolHeader (&request->DATA, remote, device, ETHERTYPE_IP);
	if (sendpacket (channel, message, sizeof (struct vs_fr_lbk_request)) <= 0) 
	{
		error (1, ECANCELED, INT6K_CANTSEND);
	}
	while (readpacket (channel, message, sizeof (struct message)) > 0) 
	{
		if (UnwantedPacket (message, (VS_FR_LBK | MMTYPE_CNF))) 
		{
			continue;
		}
		if (confirm->MSTATUS) 
		{
			error (0, ECANCELED, "Device refused request");
			return;
		}
		break;
	}
	sleep (int6k->timeperiod);
	return;
}

/*====================================================================*
 *
 *   signed NetworkTraffic (struct int6k * int6k);
 *   
 *   generate network traffic between all powerline device on all 
 *   accessible powerline networks;
 *   
 *.  Intellon INT6x00 Linux Toolkit for HomePlug AV; 
 *:  Published 2007 by Intellon Corp. ALL RIGHTS RESERVED;
 *;  For demonstration; Not for production use;
 *
 *--------------------------------------------------------------------*/

signed NetworkTraffic (struct int6k * int6k) 

{
	extern const byte localcast [ETHER_ADDR_LEN];
	struct channel * channel = (struct channel *)(int6k->channel);
	struct message * message = (struct message *)(int6k->message);

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
	* networks = (struct networks *) (&message->content);

#ifndef __GNUC__
#pragma pack (pop)
#endif

	byte bridgelist [255] [ETHER_ADDR_LEN];
	byte devicelist [255] [ETHER_ADDR_LEN];
	unsigned bridges = 0;
	unsigned devices = 0;
	unsigned bridge = 0;
	unsigned device = 0;
	unsigned remote = 0;
	memset (message, 0, sizeof (struct message));
	EthernetHeader (&message->ethernet, localcast, channel->host);
	IntellonHeader (&message->intellon, (VS_SW_VER | MMTYPE_REQ));
	if (sendpacket (channel, message, ETHER_MIN_LEN) <= 0) 
	{
		error (1, errno, INT6K_CANTSEND);
	}
	while (readpacket (channel, message, sizeof (struct message)) > 0) 
	{
		if (!UnwantedPacket (message, (VS_SW_VER | MMTYPE_CNF))) 
		{
			memcpy (bridgelist [bridges++], message->ethernet.OSA, ETHER_ADDR_LEN);
		}
	}
	for (bridge = 0; bridge < bridges; bridge++) 
	{
		memset (message, 0, sizeof (struct message));
		EthernetHeader (&message->ethernet, bridgelist [bridge], channel->host);
		IntellonHeader (&message->intellon, (VS_NW_INFO | MMTYPE_REQ));
		if (sendpacket (channel, message, ETHER_MIN_LEN) <= 0) 
		{
			error (1, errno, INT6K_CANTSEND);
		}
		while (readpacket (channel, message, sizeof (struct message)) > 0) 
		{
			if (UnwantedPacket (message, (VS_NW_INFO | MMTYPE_CNF))) 
			{
				continue;
			}
			memcpy (devicelist [devices++], message->ethernet.OSA, ETHER_ADDR_LEN);
			network = (struct network *)(&networks->network);
			while (networks->NUMAVLNS-- > 0) 
			{
				station = (struct station *)(&network->stations);
				while (network->NUMSTAS-- > 0) 
				{
					memcpy (devicelist [devices++], station->LDA, sizeof (station->LDA));
					station++;
				}
				network = (struct network *)(station);
			}
		}
		for (remote = 1; remote < devices; remote++) 
		{
			NetworkTraffic1 (int6k, devicelist [0], devicelist [remote]);
			NetworkTraffic2 (int6k, devicelist [0], devicelist [remote]);
		}
		for (device = 1; device < devices; device++) 
		{
			for (remote = 1; remote < devices; remote++) 
			{
				if (remote == device) 
				{
					continue;
				}
				NetworkTraffic1 (int6k, devicelist [device], devicelist [remote]);
	
			}
		}
	}
	return (0);
}

#endif

