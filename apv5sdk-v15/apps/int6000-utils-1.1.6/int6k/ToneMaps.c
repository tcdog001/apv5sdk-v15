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
 *   signed ToneMaps (struct int6k * int6k);
 *
 *   int6k.h
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
 *      Lluis Paulet Gimbert <dimat.com>
 *
 *--------------------------------------------------------------------*/

#ifndef TONEMAPS_SOURCE
#define TONEMAPS_SOURCE

#include <stdio.h>
#include <stdint.h>

#include "../int6k/int6k.h"
#include "../tools/flags.h"
#include "../tools/error.h"
#include "../tools/endian.h"

#define SLOTS 6
#define CARRIERS 1156

signed ToneMaps (struct int6k * int6k) 

{
	struct channel * channel = (struct channel *)(int6k->channel);
	struct message * message = (struct message *)(int6k->message);
	byte result [SLOTS] [CARRIERS/2];
	unsigned carriers = 0;
	unsigned carrier = 0;
	unsigned slots = 0;
	unsigned slot = 0;

#ifndef __GNUC__
#pragma pack (push,1)
#endif

	struct __packed vs_tone_slot_char_request 
	{
		struct header_eth ethernet;
		struct header_int intellon;
		uint8_t MACADDRESS [ETHER_ADDR_LEN];
		uint8_t TMSLOT;
	}
	* request = (struct vs_tone_slot_char_request *) (message);
	struct __packed vs_tone_slot_char_confirm 
	{
		struct header_eth ethernet;
		struct header_int intellon;
		uint8_t MSTATUS;
		uint8_t TMSLOT;
		uint8_t NUMTMS;
		uint16_t TMNUMACTCARRIERS;
		uint8_t MOD_CARRIER [CARRIERS/2];
		uint8_t GIL;
		uint8_t AGC;
	}
	* confirm = (struct vs_tone_slot_char_confirm *) (message);

#ifndef __GNUC__
#pragma pack (pop)
#endif

	Request (int6k, "Request Tone Maps");
	memset (message, 0, sizeof (struct message));
	EthernetHeader (&message->ethernet, channel->peer, channel->host);
	IntellonHeader (&message->intellon, (VS_RX_TONE_MAP_CHAR | MMTYPE_REQ));
	int6k->packetsize = ETHER_MIN_LEN;
	memcpy (request->MACADDRESS, int6k->RDA, sizeof (request->MACADDRESS));
	request->TMSLOT = 0;
	if (SendMME (int6k) <= 0) 
	{
		error ((int6k->flags & INT6K_BAILOUT), ECANCELED, INT6K_CANTSEND);
		return (-1);
	}
	if (ReadMME (int6k, (VS_RX_TONE_MAP_CHAR | MMTYPE_CNF)) <= 0) 
	{
		error ((int6k->flags & INT6K_BAILOUT), ECANCELED, INT6K_CANTREAD);
		return (-1);
	}
	carriers = LE16TOH (confirm->TMNUMACTCARRIERS);
	slots = confirm->NUMTMS;
	if (!slots) 
	{
		error ((int6k->flags & INT6K_BAILOUT), ECANCELED, "No Tone Maps Available");
		return (-1);
	}
	memset (result, 0, sizeof (result));
	for (slot = 0; slot < slots; slot++) 
	{
		EthernetHeader (&message->ethernet, channel->peer, channel->host);
		IntellonHeader (&message->intellon, (VS_RX_TONE_MAP_CHAR | MMTYPE_REQ));
		memcpy (request->MACADDRESS, int6k->RDA, sizeof (request->MACADDRESS));
		request->TMSLOT = slot;
		int6k->packetsize = ETHER_MIN_LEN;
		if (SendMME (int6k) <= 0) 
		{
			error ((int6k->flags & INT6K_BAILOUT), ECANCELED, INT6K_CANTSEND);
			return (-1);
		}
		if (ReadMME (int6k, (VS_RX_TONE_MAP_CHAR | MMTYPE_CNF)) <= 0) 
		{
			error ((int6k->flags & INT6K_BAILOUT), ECANCELED, INT6K_CANTREAD);
			return (-1);
		}
		if (confirm->MSTATUS) 
		{
			Failure (int6k, INT6K_WONTDOIT);
			return (-1);
		}
		memcpy (&result [slot][0], confirm->MOD_CARRIER, sizeof (confirm->MOD_CARRIER));
	}
	while (carrier < carriers) 
	{
		unsigned value = 0;
		unsigned scale = 0;
		unsigned index = carrier >> 1;
		printf ("% 04d", carrier);
		for (slot = 0; slot < SLOTS; slot++) 
		{
			value = result [slot][index];
			if ((carrier & 1)) 
			{
				value >>= 4;
			}
			value &= 0x0F;
			printf (" %02x", value);
			value *= value;
			scale += value;
		}
		if (slots) 
		{
			scale /= slots;
		}
		printf (" %02d", scale);
		if (_anyset (int6k->flags, INT6K_GRAPH)) 
		{
			printf (" ");
			while (scale--) 
			{
				printf ("#");
			}
		}
		printf ("\n");
		carrier++;
	}
	return (0);
}

#endif

