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
 *   signed AccessLevel (struct plc * plc, unsigned level, unsigned key_type, void * key, unsigned key_len);
 *   
 *   plc.h
 * 
 *   Gain PTS level access on a QCA7420 device using a 
 *   VS_ACCESS_CONTROL_LEVEL Request Message;
 *
 *.  Qualcomm Atheros HomePlug AV Powerline Toolkit
 *:  Published 2009-2011 by Qualcomm Atheros. ALL RIGHTS RESERVED
 *;  For demonstration and evaluation only. Not for production use
 *
 *   Contributor(s):
 *      Nathaniel Houghton <nhoughto@qualcomm.com>
 *
 *--------------------------------------------------------------------*/

#ifndef ACCESSLEVEL_SOURCE
#define ACCESSLEVEL_SOURCE

#include <stdint.h>
#include <memory.h>

#include "../plc/plc.h"
#include "../tools/error.h"

#define ACCESS_CONTROL_MAX_KEY_LENGTH 1024

signed AccessLevel (struct plc * plc, unsigned level, unsigned key_type, void * key, unsigned key_len) 

{
	struct channel * channel = (struct channel *)(plc->channel);
	struct message * message = (struct message *)(plc->message);

#ifndef __GNUC__
#pragma pack (push,1)
#endif

	struct __packed vs_access_level_request 
	{
		struct ethernet_std ethernet;
		struct qualcomm_std qualcomm;
		uint32_t RSVD1;
		uint8_t ACCESS_LEVEL;
		uint16_t RSVD2;
		uint8_t ACCESS_TYPE;
		uint16_t RSVD3;
		uint16_t KEY_LENGTH;
		uint8_t KEY [1024];
	}
	* request = (struct vs_access_level_request *) (message);
	struct __packed vs_access_level_confirm 
	{
		struct ethernet_std ethernet;
		struct qualcomm_std qualcomm;
		uint16_t RSVD1;
		uint16_t MRESPONSE;
	}
	* confirm = (struct vs_access_level_confirm *) (message);

#ifndef __GNUC__
#pragma pack (pop)
#endif

	Request (plc, "Request Access Level %d", level);
	memset (message, 0, sizeof (* message));
	if (key_len > sizeof (request->KEY)) 
	{
		error (1, 0, "access control key is too long.");
	}
	EthernetHeader (&request->ethernet, channel->peer, channel->host, HOMEPLUG_MTYPE);
	QualcommHeader (&request->qualcomm, 0, (VS_ACCESS_LEVEL_CONTROL | MMTYPE_REQ));
	plc->packetsize = sizeof (*request) - sizeof (request->KEY) + key_len;
	if (plc->packetsize < ETHER_MIN_LEN - ETHER_CRC_LEN) 
	{
		plc->packetsize = (ETHER_MIN_LEN - ETHER_CRC_LEN);
	}
	memcpy (request->KEY, key, key_len);
	request->ACCESS_LEVEL = level;
	request->KEY_LENGTH = key_len;
	request->ACCESS_TYPE = key_type;
	if (SendMME (plc) <= 0) 
	{
		error ((plc->flags & PLC_BAILOUT), errno, CHANNEL_CANTSEND);
		return (-1);
	}
	if (ReadMME (plc, 0, (VS_ACCESS_LEVEL_CONTROL | MMTYPE_CNF)) <= 0) 
	{
		error ((plc->flags & PLC_BAILOUT), ECANCELED, CHANNEL_CANTREAD);
		return (-1);
	}
	if (confirm->MRESPONSE) 
	{
		Failure (plc, PLC_WONTDOIT);
		return (-1);
	}
	return (0);
}


#endif

