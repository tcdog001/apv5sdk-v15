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
 *   signed Loopback (struct int6k * int6k, void * memory, size_t extent);
 *   
 *   int6k.h
 * 
 *   instruct a powerline device to echo a given Etherenet frame for
 *   a fixed period of time using the VS_FR_LNK message;
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

#ifndef LOOPBACK_SOURCE
#define LOOPBACK_SOURCE

#include <stdint.h>
#include <memory.h>

#include "../int6k/int6k.h"
#include "../tools/error.h"
#include "../tools/memory.h"

signed Loopback (struct int6k * int6k, void * memory, size_t extent) 

{

	struct channel * channel = (struct channel *)(int6k->channel);
	struct message * message = (struct message *)(int6k->message);
#define DATALEN 1038
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

	memset (message, 0, sizeof (struct message));
	EthernetHeader (&message->ethernet, channel->peer, channel->host);
	IntellonHeader (&message->intellon, (VS_FR_LBK | MMTYPE_REQ));
	if (extent > DATALEN) 
	{
		extent = DATALEN;
	}
	memcpy (request->DATA, memory, extent);
	if (extent < ETHER_MIN_LEN) 
	{
		extent = ETHER_MIN_LEN;
	}
	request->DURATION = int6k->timeperiod;
	request->LENGTH = HTOLE16 ((uint16_t)(extent));
	extent += sizeof (struct header_eth);
	extent += sizeof (struct header_int);
	extent += 4;
	int6k->packetsize = (uint16_t)(extent);
	if (SendMME (int6k) <= 0) 
	{
		error (1, ECANCELED, INT6K_CANTSEND);
	}
	while (ReadMME (int6k, (VS_FR_LBK | MMTYPE_CNF)) > 0) 
	{
		if (confirm->MSTATUS) 
		{
			Failure (int6k, INT6K_WONTDOIT);
			continue;
		}
	}
	return (0);
}

#endif

