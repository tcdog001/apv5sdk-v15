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
 *   signed NetworkInformation (struct channel * channel);
 *
 *   int6k.h
 *
 *   Request network membership information for the peer device using 
 *   one VS_NW_INFO message; 
 *
 *   This function is similar to function NetworkInfo() but the output 
 *   format is different;
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

#ifndef NETWORKINFORMATION_SOURCE
#define NETWORKINFORMATION_SOURCE

#include <stdint.h>
#include <memory.h>

#include "../int6k/int6k.h"
#include "../tools/memory.h"
#include "../tools/number.h"
#include "../tools/error.h"

signed NetworkInformation (struct channel * channel) 

{
	struct message message;
	char string [24];

#ifndef __GNUC__
#pragma pack (push,1)
#endif

	struct __packed station 
	{
		uint8_t DA [ETHER_ADDR_LEN];
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
		uint8_t CCO_MACADDR [ETHER_ADDR_LEN];
		uint8_t CCO_TEI;
		uint8_t NUMSTAS;
		struct station station [1];
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

	memset (&message, 0, sizeof (message));
	EthernetHeader (&message.ethernet, channel->peer, channel->host);
	IntellonHeader (&message.intellon, (VS_NW_INFO | MMTYPE_REQ));
	if (sendpacket (channel, &message, ETHER_MIN_LEN) <= 0) 
	{
		error (1, errno, INT6K_CANTSEND);
	}
	if (readpacket (channel, &message, sizeof (message)) <= 0) 
	{
		error (1, errno, INT6K_CANTREAD);
	}
	network = (struct network *)(&networks->network);
	while (networks->NUMAVLNS-- > 0) 
	{
		printf (" NET");
		printf (" NID %20s", hexstring (string, sizeof (string), network->NID, sizeof (network->NID)));
		printf (" SNID %03d", network->SNID);
		printf ("\n");
		printf (" %s", (network->TEI == network->CCO_TEI)? "CCO": "STA");
		printf (" TEI %03d", network->TEI);
		printf (" MAC %17s", hexstring (string, sizeof (string), message.ethernet.OSA, sizeof (message.ethernet.ODA)));
		printf (" BDA %17s", hexstring (string, sizeof (string), message.ethernet.ODA, sizeof (message.ethernet.ODA)));
		printf ("\n");
		station = (struct station *)(&network->station);
		while (network->NUMSTAS-- > 0) 
		{
			printf (" %s", (station->TEI == network->CCO_TEI)? "CCO": "STA");
			printf (" TEI %03d", station->TEI);
			printf (" MAC %17s", hexstring (string, sizeof (string), station->DA, sizeof (station->DA)));
			printf (" BDA %17s", hexstring (string, sizeof (string), station->BDA, sizeof (station->BDA)));
			printf (" TX %03d", station->AVGTX);
			printf (" RX %03d", station->AVGRX);
			printf ("\n");
			station++;
		}
		network = (struct network *)(station);
	}
	return (0);
}

#endif

