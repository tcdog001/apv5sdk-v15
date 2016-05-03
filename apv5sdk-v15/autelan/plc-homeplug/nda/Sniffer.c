/*====================================================================*
 *   
 *   Copyright (c) 2011 by Qualcomm Atheros.
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
 *   signed Sniffer (struct plc * plc);
 *   
 *   nda.h
 *
 *   enable/disable sniffer mode on a powerline device;
 *
 *
 *.  Qualcomm Atheros HomePlug AV Powerline Toolkit
 *:  Published 2009-2011 by Qualcomm Atheros. ALL RIGHTS RESERVED
 *;  For demonstration and evaluation only. Not for production use
 *
 *   Contributor(s):
 *      Charles Maier <cmaier@qualcomm.com>
 *
 *--------------------------------------------------------------------*/

#ifndef SNIFFER_SOURCE
#define SNIFFER_SOURCE

#include <memory.h>
#include <errno.h>

#include "../tools/memory.h"
#include "../tools/error.h"
#include "../tools/flags.h"
#include "../ether/channel.h"
#include "../mme/mme.h"
#include "../plc/plc.h"

signed Sniffer (struct plc * plc) 

{
	struct channel * channel = (struct channel *)(plc->channel);
	struct message * message = (struct message *)(plc->message);

#ifndef __GNUC__
#pragma pack (push,1)
#endif

	struct __packed vs_sniffer_request 
	{
		struct ethernet_std ethernet;
		struct qualcomm_std qualcomm;
		uint8_t SNIFFCTRL;
	}
	* request = (struct vs_sniffer_request *) (message);
	struct __packed vs_sniffer_confirm 
	{
		struct ethernet_std ethernet;
		struct qualcomm_std qualcomm;
		uint8_t MSTATUS;
		uint8_t SNIFSTATE;
		uint8_t SNIFFER_DA [ETHER_ADDR_LEN];
	}
	* confirm = (struct vs_sniffer_confirm *) (message);

#ifndef __GNUC__
#pragma pack (pop)
#endif
	ssize_t packetsize;

	memset (message, 0, sizeof (* message));
	EthernetHeader (&request->ethernet, channel->peer, channel->host, HOMEPLUG_MTYPE);
	QualcommHeader (&request->qualcomm, 0, (VS_SNIFFER | MMTYPE_REQ));
	request->SNIFFCTRL = plc->action;
	if (sendpacket (channel, message, (ETHER_MIN_LEN - ETHER_CRC_LEN)) <= 0) 
	{
		error (1, ECANCELED, CHANNEL_CANTSEND);
	}
	while ((packetsize = readpacket (channel, message, sizeof (* message))) > 0) 
	{
		if (!UnwantedMessage (message, packetsize, 0, (VS_SNIFFER | MMTYPE_CNF))) 
		{
			Display (plc, "SNIFFER %s", (confirm->SNIFSTATE)? "ON": "OFF");
			break;
		}
	}
	return (0);
}


#endif

