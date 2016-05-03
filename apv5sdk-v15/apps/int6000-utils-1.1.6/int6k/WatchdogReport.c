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
 *   signed WatchdogReport (struct int6k * int6k);
 *   
 *   int6k.h
 * 
 *   Read the watchdog report using VS_WD_RPT and write it to a file
 *   in binary format; this file can be sent to Intellon Support for
 *   analysis;
 *   
 *   The VS_WD_RPT message returns an indication, not a confirmation
 *    message;
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

#ifndef WATCHDOGREPORT_SOURCE
#define WATCHDOGREPORT_SOURCE

#include <stdint.h>
#include <unistd.h>
#include <memory.h>

#include "../int6k/int6k.h"
#include "../tools/error.h"
#include "../tools/flags.h"
#include "../tools/memory.h"
#include "../tools/format.h"

signed WatchdogReport (struct int6k * int6k) 

{
	struct channel * channel = (struct channel *)(int6k->channel);
	struct message * message = (struct message *)(int6k->message);

#ifndef __GNUC__
#pragma pack (push,1)
#endif

	struct __packed vs_wd_rpt_request 
	{
		struct header_eth ethernet;
		struct header_int intellon;
		uint16_t SESSIONID;
		uint8_t CLR;
	}
	* request = (struct vs_wd_rpt_request *) (message);
	struct __packed vs_wd_rpt_ind 
	{
		struct header_eth ethernet;
		struct header_int intellon;
		uint8_t MSTATUS;
		uint16_t SESSIONID;
		uint8_t NUMPARTS;
		uint8_t CURPART;
		uint16_t RDATALENGTH;
		uint8_t RDATAOFFSET;
		uint8_t RDATA [1];
	}
	* indicate = (struct vs_wd_rpt_ind *) (message);

#ifndef __GNUC__
#pragma pack (pop)
#endif

	Request (int6k, "Read Watchdog Report");
	memset (message, 0, sizeof (struct message));
	EthernetHeader (&message->ethernet, channel->peer, channel->host);
	IntellonHeader (&message->intellon, (VS_WD_RPT | MMTYPE_REQ));
	request->SESSIONID = 0xFEFE;
	request->CLR = int6k->readaction;
	int6k->packetsize = ETHER_MIN_LEN;
	if (SendMME (int6k) <= 0) 
	{
		error ((int6k->flags & INT6K_BAILOUT), ECANCELED, INT6K_CANTSEND);
		return (-1);
	}
	do 
	{
		if (ReadMME (int6k, (VS_WD_RPT | MMTYPE_IND)) <= 0) 
		{
			error ((int6k->flags & INT6K_BAILOUT), ECANCELED, INT6K_CANTREAD);
			return (-1);
		}
		if (indicate->MSTATUS) 
		{
			Failure (int6k, INT6K_WONTDOIT);
			return (-1);
		}
		if (write (int6k->rpt.file, indicate->RDATA + indicate->RDATAOFFSET, LE16TOH (indicate->RDATALENGTH)) < LE16TOH (indicate->RDATALENGTH)) 
		{
			Failure (int6k, "Can't write %s", int6k->rpt.name);
			return (-1);
		}
	}
	while (indicate->CURPART < indicate->NUMPARTS);
	Confirm (int6k, "Read %s", int6k->rpt.name);
	return (0);
}

#endif

