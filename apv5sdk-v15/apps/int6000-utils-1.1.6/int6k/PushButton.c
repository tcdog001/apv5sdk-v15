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
 *   signed PushButton (struct int6k * int6k);
 *   
 *   int6k.h
 * 
 *   THis plugin for program int6k emulates pushbutton functionality 
 *   using a MS_PB_ENC message;
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

#ifndef PUSHBUTTON_SOURCE
#define PUSHBUTTON_SOURCE

#include <stdint.h>
#include <memory.h>

#include "../int6k/int6k.h"
#include "../tools/error.h"
#include "../tools/memory.h"

signed PushButton (struct int6k * int6k) 

{
	struct channel * channel = (struct channel *)(int6k->channel);
	struct message * message = (struct message *)(int6k->message);

#ifndef __GNUC__
#pragma pack (push,1)
#endif

	struct __packed ms_pb_enc_request 
	{
		struct header_eth ethernet;
		struct header_std homeplug;
		uint8_t PBACTION;
	}
	* request = (struct ms_pb_enc_request *) (message);
	struct __packed ms_pb_enc_confirm 
	{
		struct header_eth ethernet;
		struct header_std homeplug;
		uint8_t MSTATUS;
		uint8_t AVLNSTAT;
	}
	* confirm = (struct ms_pb_enc_confirm *) (message);

#ifndef __GNUC__
#pragma pack (pop)
#endif

	if (int6k->pushbutton == 1) 
	{
		Request (int6k, "Join Network");
	}
	if (int6k->pushbutton == 2) 
	{
		Request (int6k, "Leave Network");
	}
	if (int6k->pushbutton == 3) 
	{
		Request (int6k, "Fetch Network Status");
	}
	memset (message, 0, sizeof (struct message));
	EthernetHeader (&message->ethernet, channel->peer, channel->host);
	request->homeplug.MMV = INTELLON_MMV;
	request->homeplug.MMTYPE = HTOLE16(MS_PB_ENC | MMTYPE_REQ);
	request->PBACTION = int6k->pushbutton;
	int6k->packetsize = ETHER_MIN_LEN;
	if (SendMME (int6k) <= 0) 
	{
		error ((int6k->flags & INT6K_BAILOUT), ECANCELED, INT6K_CANTSEND);
		return (-1);
	}
	if (ReadMFG (int6k, (MS_PB_ENC | MMTYPE_CNF)) <= 0) 
	{
		error ((int6k->flags & INT6K_BAILOUT), ECANCELED, INT6K_CANTREAD);
		return (-1);
	}
	if (confirm->MSTATUS) 
	{
		Failure (int6k, INT6K_WONTDOIT);
		return (-1);
	}
	if (int6k->pushbutton == 1) 
	{
		Confirm (int6k, "Joining ...");
		return (0);
	}
	if (int6k->pushbutton == 2) 
	{
		Confirm (int6k, "Leaving ...");
		return (0);
	}
	if (int6k->pushbutton == 3) 
	{
		Confirm (int6k, "Membership Status %d", confirm->AVLNSTAT);
		return (0);
	}
	Failure (int6k, "Unexpected pushbutton action code");
	return (-1);
}

#endif

