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
 *   signed Attributes (struct int6k * int6k);
 *   
 *   int6k.h
 *
 *   This plugin for program int6k requests device attributes using
 *   a VS_OP_ATTRIBUTES message; attributes are pre-parsed versions
 *   of information returned by VS_SW_VER and other messages; 
 * 
 *   The VS_OP_ATTRIBUTES message structure changed between FW 3.3.4
 *   and 3.3.5 and fields are ocassionally appended to the end; you  
 *   should not use this message for operational systems because the
 *   format may change again; it was originally intended for PTS use
 *   only;
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

#ifndef ATTRIBUTES_SOURCE
#define ATTRIBUTES_SOURCE

#include <stdint.h>
#include <memory.h>

#include "../int6k/int6k.h"
#include "../tools/error.h"
#include "../tools/memory.h"
#include "../tools/format.h"

#ifdef WIN32
#define snprintf _snprintf
#endif

signed Attributes (struct int6k * int6k) 

{

	struct channel * channel = (struct channel *)(int6k->channel);
	struct message * message = (struct message *)(int6k->message);
#ifndef __GNUC__
#pragma pack (push,1)
#endif

	struct __packed vs_op_attributes_request 
	{
		struct header_eth ethernet;
		struct header_int intellon;
		uint32_t COOKIE;
		uint8_t RTYPE;
	}
	* request = (struct vs_op_attributes_request *) (message);
	struct __packed vs_op_attributes_confirm 
	{
		struct header_eth ethernet;
		struct header_int intellon;
		uint16_t MSTATUS;
		uint32_t COOKIE;
		uint8_t RTYPE;
		uint16_t MLENGTH;
		uint8_t MBUFFER [ETHERMTU-9];
	}
	* confirm = (struct vs_op_attributes_confirm *) (message);
	struct __packed attributes 
	{
		uint8_t HARDWARE [16];
		uint8_t SOFTWARE [16];
		uint32_t FWVERSION [5];
		uint8_t RELEASEDATE [8];
		uint8_t RELEASETYPE [12];
		uint8_t SDRAMTYPE;
		uint8_t RESERVED;
		uint8_t FLAGS;
		uint32_t SDRAMSIZE;
	}
	* attributes = (struct attributes *)(confirm->MBUFFER);

#ifndef __GNUC__
#pragma pack (pop)
#endif

	Request (int6k, "Fetch Device Attributes");
	memset (message, 0, sizeof (struct message));
	EthernetHeader (&message->ethernet, channel->peer, channel->host);
	IntellonHeader (&message->intellon, (VS_OP_ATTRIBUTES | MMTYPE_REQ));
	int6k->packetsize = ETHER_MIN_LEN;
	request->COOKIE = 0x55555555;
	request->RTYPE = 0;
	if (SendMME (int6k) <= 0) 
	{
		error ((int6k->flags & INT6K_BAILOUT), ECANCELED, INT6K_CANTSEND);
		return (-1);
	}
	while (ReadMME (int6k, (VS_OP_ATTRIBUTES | MMTYPE_CNF)) > 0) 
	{
		char string [512];
		int length = 0;
		if (confirm->MSTATUS) 
		{
			Failure (int6k, INT6K_WONTDOIT);
			continue;
		}
		length += snprintf (string + length, sizeof (string) - length, "%s", attributes->HARDWARE);
		length += snprintf (string + length, sizeof (string) - length, "-%s", attributes->SOFTWARE);
		length += snprintf (string + length, sizeof (string) - length, "-%d", attributes->FWVERSION [0]);
		length += snprintf (string + length, sizeof (string) - length, "-%d", attributes->FWVERSION [1]);
		length += snprintf (string + length, sizeof (string) - length, "-%04d", attributes->FWVERSION [2]);
		length += snprintf (string + length, sizeof (string) - length, "-%02d", attributes->FWVERSION [3]);
		length += snprintf (string + length, sizeof (string) - length, "-%04d", attributes->FWVERSION [4]);
		length += snprintf (string + length, sizeof (string) - length, "%-8.8s", attributes->RELEASEDATE);
		length += snprintf (string + length, sizeof (string) - length, "-%s", attributes->RELEASETYPE);
		length += snprintf (string + length, sizeof (string) - length, "-%c", attributes->SDRAMTYPE);
		length += snprintf (string + length, sizeof (string) - length, " (%dmb)", attributes->SDRAMSIZE);
		Display (int6k, "%s", string);
	}
	return (0);
}

#endif

