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
 *   signed SetNMK (struct int6k * int6k);
 *   
 *   int6k.h
 *
 *   set NMK on a local or remote device using a VS_SET_KEY message;
 *
 *   using this message to set the NMK on a remote device requires
 *   both the remote device address (RDA) and the remote device
 *   access key (DAK);
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

#ifndef SETNMK_SOURCE
#define SETNMK_SOURCE

#include <string.h>

#include "../int6k/int6k.h"
#include "../tools/error.h"
#include "../tools/flags.h"
#include "../tools/memory.h"
#include "../crypt/HPAVKey.h"

signed SetNMK (struct int6k * int6k) 

{

	struct channel * channel = (struct channel *)(int6k->channel);
	struct message * message = (struct message *)(int6k->message);
#ifndef __GNUC__
#pragma pack (push,1)
#endif

	struct __packed vs_set_key_request 
	{
		struct header_eth ethernet;
		struct header_int intellon;
		uint8_t EKS;
		uint8_t NMK [HPAVKEY_NMK_LEN];
		uint8_t PEKS;
		uint8_t RDA [ETHER_ADDR_LEN];
		uint8_t DAK [HPAVKEY_DAK_LEN];
	}
	* request = (struct vs_set_key_request *) (message);
	struct __packed vs_set_key_confirm 
	{
		struct header_eth ethernet;
		struct header_int intellon;
		uint8_t MSTATUS;
	}
	* confirm = (struct vs_set_key_confirm *) (message);

#ifndef __GNUC__
#pragma pack (pop)
#endif

	memset (message, 0, sizeof (struct message));
	EthernetHeader (&message->ethernet, channel->peer, channel->host);
	IntellonHeader (&message->intellon, (VS_SET_KEY | MMTYPE_REQ));
	int6k->packetsize = sizeof (struct vs_set_key_request);
	request->EKS = 0x01;
	memcpy (request->NMK, int6k->NMK, sizeof (request->NMK));
	if (_anyset (int6k->flags, INT6K_SETREMOTEKEY)) 
	{
		Request (int6k, "Set Remote Network Membership Key");
		memcpy (request->RDA, int6k->RDA, sizeof (request->RDA));
		memcpy (request->DAK, int6k->DAK, sizeof (request->DAK));
		request->PEKS = 0x00;
	}
	else 
	{
		Request (int6k, "Set Local Network Membership Key");
		memset (request->RDA, 0, sizeof (request->RDA));
		memset (request->DAK, 0, sizeof (request->DAK));
		request->PEKS = 0x0F;
	}
	if (SendMME (int6k) <= 0) 
	{
		error ((int6k->flags & INT6K_BAILOUT), ECANCELED, INT6K_CANTSEND);
		return (-1);
	}
	if (ReadMME (int6k, (VS_SET_KEY | MMTYPE_CNF)) <= 0) 
	{
		error ((int6k->flags & INT6K_BAILOUT), ECANCELED, INT6K_CANTREAD);
		return (-1);
	}
	if (confirm->MSTATUS) 
	{
		Failure (int6k, INT6K_WONTDOIT);
		return (-1);
	}
	Confirm (int6k, "Setting ...");
	return (0);
}

#endif

