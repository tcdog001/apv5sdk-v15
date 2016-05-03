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
 *   int WaitForStart (struct int6k *int6k, char string [], signed length);
 *
 *   int6k.h
 *
 *   poll the device using VS_SW_VER messages until it responds or 
 *   the allotted time expires; return 0 if the device responds within
 *   the allotted time or -1 if it does not or if a transmission error
 *   occurs;
 *
 *   retry is number of poll attempts before returning an error to
 *   indicate that the bootloader or firmware failed to respond in
 *   the allotted time; timer is the time between poll attempts;
 *
 *   See Intellon HomePlug AV Firmware Technical Reference Manual 
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

#ifndef WAITFORSTART_SOURCE
#define WAITFORSTART_SOURCE

#include <stdint.h>
#include <unistd.h>
#include <memory.h>
#include <string.h>

#include "../int6k/int6k.h"
#include "../tools/memory.h"
#include "../tools/error.h"

int WaitForStart (struct int6k *int6k, char string [], signed length) 

{

	struct channel * channel = (struct channel *)(int6k->channel);
	struct message * message = (struct message *)(int6k->message);
#ifndef __GNUC__
#pragma pack (push,1)
#endif

	struct __packed vs_sw_ver_confirm 
	{
		struct header_eth ethernet;
		struct header_int intellon;
		uint8_t MSTATUS;
		uint8_t MDEVICEID;
		uint8_t MVERLENGTH;
		char MVERSION [INT6K_VERSTRING];
		uint8_t UPGRADEABLE;
	}
	* confirm = (struct vs_sw_ver_confirm *) (message);

#ifndef __GNUC__
#pragma pack (pop)
#endif

	unsigned retry = int6k->retry;
	while (retry--) 
	{
		signed status;
		memset (message, 0, sizeof (struct message));
		EthernetHeader (&message->ethernet, channel->peer, channel->host);
		IntellonHeader (&message->intellon, (VS_SW_VER | MMTYPE_REQ));
		int6k->packetsize = ETHER_MIN_LEN;
		if (SendMME (int6k) <= 0) 
		{
			error ((int6k->flags & INT6K_BAILOUT), ECANCELED, INT6K_CANTSEND);
			return (-1);
		}
		status = ReadMME (int6k, (VS_SW_VER | MMTYPE_CNF));
		if (status < 0) 
		{
			error ((int6k->flags & INT6K_BAILOUT), ECANCELED, INT6K_CANTREAD);
			return (-1);
		}
		if (status > 0) 
		{
			if (confirm->MSTATUS) 
			{
				Failure (int6k, "Device will not start");
				return (-1);
			}
			int6k->hardwareID = confirm->MDEVICEID;
			strncpy (string, confirm->MVERSION, length);
			if ((int6k->hardwareID == INT6000A1) && !strcmp (string, INT6K_BOOTLOADER)) 
			{
				int6k->hardwareID = INT6300A0;
			}
			return (0);
		}
		if (retry) 
		{
			SLEEP (int6k->timer);
		}
	}
	Failure (int6k, "Device did not Start");
	return (-1);
}

#endif

