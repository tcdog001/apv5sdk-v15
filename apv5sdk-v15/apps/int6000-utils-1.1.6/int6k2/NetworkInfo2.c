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
 *   signed NetworkInfo2 (struct int6k * int6k);
 *
 *   int6k2.h
 *
 *   fetch and print network information using ihpapi_GetNetworkInfo;
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
 *      Alex Vasquez <alex.vasquez@intellon.com>
 *
 *--------------------------------------------------------------------*/

#ifndef NETWORKINFO2_SOURCE
#define NETWORKINFO2_SOURCE

#include <stdint.h>
#include <memory.h>

#include "../ihpapi/ihp.h"
#include "../ihpapi/ihpapi.h"
#include "../int6k/int6k.h"
#include "../tools/memory.h"
#include "../tools/number.h"
#include "../tools/error.h"

#define STAROLE_MAX 2

const char * StationRole [STAROLE_MAX+1] = 

{
	"STA",
	"PCO",
	"CCO" 
};

signed NetworkInfo2 (struct int6k * int6k) 

{

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
		uint8_t CCO_MACADDR [6];
		uint8_t CCO_TEI;
		uint8_t NUMSTAS;
		struct station station [0x100];
	}
	* network;
	struct __packed networks 
	{
		uint8_t networks;
		struct network network [1];
	}
	* networks;

#ifndef __GNUC__
#pragma pack (pop)
#endif

	char string [24];
	Request (int6k, "Fetch Network Information");
	int6k->packetsize = (int)(ihpapi_GetNetworkInfo (int6k->channel->host, int6k->channel->peer, sizeof (struct message), (uint8_t *)(int6k->message)));
	if (int6k->packetsize == 0) 
	{
		error ((int6k->flags & INT6K_BAILOUT), errno, "wrong packet size");
		return (-1);
	}
	if (SendMME (int6k) <= 0) 
	{
		error ((int6k->flags & INT6K_BAILOUT), errno, INT6K_CANTSEND);
		return (-1);
	}
	if (ReadMME2 (int6k) > 0) 
	{
		networks = (struct networks *) (&int6k->result->data);
		Confirm (int6k, "Found %d Network(s)\n\n", networks->networks);
		for (network = (struct network *)(&networks->network); networks->networks-- > 0; network++) 
		{
			printf ("\tnetwork->NID = %s\n", hexstring (string, sizeof (string), network->NID, sizeof (network->NID)));
			printf ("\tnetwork->SNID = %d\n", network->SNID);
			printf ("\tnetwork->TEI = %d\n", network->TEI);
			printf ("\tnetwork->ROLE = 0x%02X (%s)\n", network->ROLE, StationRole [network->ROLE]);
			printf ("\tnetwork->CCO_DA = %s\n", hexstring (string, sizeof (string), network->CCO_MACADDR, sizeof (network->CCO_MACADDR)));
			printf ("\tnetwork->CCO_TEI = %d\n", network->CCO_TEI);
			printf ("\tnetwork->STATIONS = %d\n", network->NUMSTAS);
			printf ("\n");
			for (station = (struct station *)(&network->station); network->NUMSTAS-- > 0; station++) 
			{
				printf ("\t\tstation->DA = %s\n", hexstring (string, sizeof (string), station->DA, sizeof (station->DA)));
				printf ("\t\tstation->TEI = %d\n", station->TEI);
				printf ("\t\tstation->BDA = %s\n", hexstring (string, sizeof (string), station->BDA, sizeof (station->BDA)));
				printf ("\t\tstation->AvgPHYDR_TX = %d mbps\n", station->AVGTX);
				printf ("\t\tstation->AvgPHYDR_RX = %d mbps\n", station->AVGRX);
				printf ("\n");
			}
			network = (struct network *)(station);
		}
	}
	return (0);
}

#endif

