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
 *   int StartFirmware (struct int6k * int6k, struct header_nvm * header_nvm) 
 *
 *   int6k.h
 *
 *   This plugin starts firmware execution using VS_ST_MAC; despite
 *   an affirmative respond from the device, there is no guarantee
 *   that the firmware will actually start or continue to run so we
 *   wait for a response and report the result;
 *
 *   struct header_nvm must be little-endian order and ready to send 
 *   to the device;
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

#ifndef STARTFIRMWARE_SOURCE
#define STARTFIRMWARE_SOURCE

#include <stdint.h>
#include <memory.h>

#include "../int6k/int6k.h"
#include "../tools/error.h"
#include "../tools/memory.h"

int StartFirmware (struct int6k * int6k, struct header_nvm * header_nvm) 

{
	char firmware [INT6K_VERSTRING];
	struct channel * channel = (struct channel *)(int6k->channel);
	struct message * message = (struct message *)(int6k->message);

#ifndef __GNUC__
#pragma pack (push,1)
#endif

	struct __packed vs_st_mac_request 
	{
		struct header_eth ethernet;
		struct header_int intellon;
		uint8_t MODULEID;
		uint8_t RESERVED [3];
		uint32_t IMAGELOAD;
		uint32_t IMAGELENGTH;
		uint32_t IMAGECHECKSUM;
		uint32_t IMAGESTART;
	}
	* request = (struct vs_st_mac_request *) (message);
	struct __packed vs_st_mac_confirm 
	{
		struct header_eth ethernet;
		struct header_int intellon;
		uint8_t MSTATUS;
		uint8_t MODULEID;
	}
	* confirm = (struct vs_st_mac_confirm *) (message);

#ifndef __GNUC__
#pragma pack (pop)
#endif

	memset (message, 0, sizeof (struct message));
	EthernetHeader (&message->ethernet, channel->peer, channel->host);
	IntellonHeader (&message->intellon, (VS_ST_MAC | MMTYPE_REQ));
	int6k->packetsize = ETHER_MIN_LEN;
	request->MODULEID = VS_MODULE_MAC;
	request->IMAGELOAD = header_nvm->IMAGEADDR;
	request->IMAGELENGTH = header_nvm->IMAGELENGTH;
	request->IMAGECHECKSUM = header_nvm->IMAGECHECKSUM;
	request->IMAGESTART = header_nvm->ENTRYPOINT;
	if (SendMME (int6k) <= 0) 
	{
		error ((int6k->flags & INT6K_BAILOUT), ECANCELED, INT6K_CANTSEND);
		return (-1);
	}
	if (ReadMME (int6k, (VS_ST_MAC | MMTYPE_CNF)) <= 0) 
	{
		error ((int6k->flags & INT6K_BAILOUT), ECANCELED, INT6K_CANTREAD);
		return (-1);
	}
	if (confirm->MSTATUS) 
	{
		Failure (int6k, "Firmware will not Start.");
		return (-1);
	}
	if (WaitForStart (int6k, firmware, sizeof (firmware))) 
	{
		return (-1);
	}
	Confirm (int6k, "Started %s", firmware);
	return (0);
}

#endif

