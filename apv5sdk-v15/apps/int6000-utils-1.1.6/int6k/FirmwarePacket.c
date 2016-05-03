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
 *   bool FirmwarePacket (struct int6k * int6k);
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
 *   liability for any consequences of its use. 
 *   
 *.  Intellon INT6x00 Linux Toolkit for HomePlug AV; 
 *:  Published 2007 by Intellon Corp. ALL RIGHTS RESERVED;
 *;  For demonstration; Not for production use;
 *
 *   Contributor(s):
 *      Charles Maier <charles.maier@intellon.com>
 *      Nathan Houghton <nathan.houghton@intellon.com>
 *
 *--------------------------------------------------------------------*/

#ifndef FIRMWAREPACKET_SOURCE
#define FIRMWAREPACKET_SOURCE

#include <stdint.h>
#include <memory.h>

#include "../int6k/int6k.h"
#include "../int6k/message.h"
#include "../ether/channel.h"
#include "../tools/memory.h" 
#include "../tools/flags.h"

bool FirmwarePacket (struct int6k * int6k) 

{

	struct message * message = (struct message *)(int6k->message);
#ifndef __GNUC__
#pragma pack (push,1)
#endif

	struct __packed header_int header_arpc = 
	{
		INTELLON_MMV,
		HTOLE16 (VS_ARPC | MMTYPE_IND),
		{
			0x00,
			0xB0,
			0x52 
		}
	};
	struct __packed vs_arpc_ind 
	{
		struct header_eth ethernet;
		struct header_int intellon;
		uint16_t RDATALENGTH;
		uint8_t RDATAOFFSET;
		uint8_t RDATA [1];
	}
	* indicate = (struct vs_arpc_ind *)(message);

#ifndef __GNUC__
#pragma pack (pop)
#endif

	if (!memcmp (&indicate->intellon, &header_arpc, sizeof (header_arpc))) 
	{
		indicate->RDATA [indicate->RDATALENGTH - indicate->RDATAOFFSET] = (char)(0);
		Display (int6k, "0x%02X %s", indicate->RDATA [0], &indicate->RDATA [5]);
		return (-1);
	}
	return (0);
}

#endif

