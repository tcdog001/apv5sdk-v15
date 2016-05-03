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
 *   signed WriteCFG (struct int6k * int6k);
 *
 *   int6k.h
 *
 *   This plugin for program int6k writes the contents of an SDRAM
 *   confirmation file to a device using a VS_SET_SDRAM message; the
 *   CFG file in struct int6k must be opened before this function is
 *   called; the bootloader must be running for this to work; 
 *   
 *   the VS_SET_SDRAM message is recognized by the INT600 BootLoader
 *   only; the INT6400 BootLoader recognizes it but does nothing with
 *   it;
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

#ifndef WRITECFG_SOURCE
#define WRITECFG_SOURCE

#include <stdint.h>
#include <unistd.h>
#include <memory.h>

#include "../int6k/int6k.h"
#include "../tools/memory.h"
#include "../tools/error.h"
#include "../tools/files.h"
#include "../ram/sdram.h"

int WriteCFG (struct int6k * int6k) 

{

	struct channel * channel = (struct channel *)(int6k->channel);
	struct message * message = (struct message *)(int6k->message);
#ifndef __GNUC__
#pragma pack (push,1)
#endif

	struct __packed vs_set_sdram_request 
	{
		struct header_eth ethernet;
		struct header_int intellon;
		struct config_ram config_ram;
		uint32_t CHECKSUM;
	}
	* request = (struct vs_set_sdram_request *) (message);
	struct __packed vs_set_sdram_confirm 
	{
		struct header_eth ethernet;
		struct header_int intellon;
		uint8_t MSTATUS;
	}
	* confirm = (struct vs_set_sdram_confirm *) (message);

#ifndef __GNUC__
#pragma pack (pop)
#endif

	Request (int6k, "Write Configuration to Device");
	memset (message, 0, sizeof (struct message));
	EthernetHeader (&message->ethernet, channel->peer, channel->host);
	IntellonHeader (&message->intellon, (VS_SET_SDRAM | MMTYPE_REQ));
	int6k->packetsize = ETHER_MIN_LEN;
	if (lseek (int6k->CFG.file, 0, SEEK_SET)) 
	{
		error ((int6k->flags & INT6K_BAILOUT), errno, "can't rewind %s", filepart (int6k->CFG.name));
		return (-1);
	}
	if (read (int6k->CFG.file, &request->config_ram, sizeof (struct config_ram)) < sizeof (struct config_ram)) 
	{
		error ((int6k->flags & INT6K_BAILOUT), errno, "can't read %s", filepart (int6k->CFG.name));
		return (-1);
	}
	if (read (int6k->CFG.file, &request->CHECKSUM, sizeof (request->CHECKSUM)) < sizeof (request->CHECKSUM)) 
	{
		error ((int6k->flags & INT6K_BAILOUT), errno, "can't read %s checksum", filepart (int6k->CFG.name));
		return (-1);
	}
	if (SendMME (int6k) <= 0) 
	{
		error ((int6k->flags & INT6K_BAILOUT), ECANCELED, INT6K_CANTSEND);
		return (-1);
	}
	if (ReadMME (int6k, (VS_SET_SDRAM | MMTYPE_CNF)) <= 0) 
	{
		error ((int6k->flags & INT6K_BAILOUT), ECANCELED, INT6K_CANTREAD);
		return (-1);
	}
	if (confirm->MSTATUS) 
	{
		Failure (int6k, INT6K_WONTDOIT);
		return (-1);
	}
	Confirm (int6k, "Wrote %s", filepart (int6k->CFG.name));
	return (0);
}

#endif

