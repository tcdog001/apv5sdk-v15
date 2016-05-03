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
 *   signed ReadMME2 (struct int6k * int6k);
 *
 *   int6k.h
 *
 *   receive frames over Ethernet and return frame length or 0 on
 *   timeout or -1 on error; return unwanted or unexpected frames 
 *   as an error; 
 *
 *   the components of struct int6k used here are:
 *
 *      int6k->channel
 *      int6k->packet
 *      int6k->packetsize
 *      int6k->result  
 *
 *   see SendMME for the send counterpart to this functions;
 *
 *   ReadMME2 is similar to ReadConfirm but uses the Intellon Embedded
 *   API;
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
 *      Alex Vasquez <alex.vasquez@intellon.com>
 *
 *--------------------------------------------------------------------*/

#ifndef READCONFIRM2_SOURCE
#define READCONFIRM2_SOURCE

#include <stdint.h>
#include <memory.h>

#include "../ihpapi/ihp.h"
#include "../ihpapi/ihpapi.h"
#include "../int6k/int6k.h"
#include "../tools/error.h"

signed ReadMME2 (struct int6k * int6k) 

{
	while ((int6k->packetsize = readpacket (int6k->channel, int6k->message, sizeof (struct message))) > 0) 
	{
		if (ihpapi_RxFrame (int6k->packetsize, (uint8_t *)(int6k->message), int6k->result) == -1) 
		{
			switch (errno) 
			{
			case EFAULT:
				error ((int6k->flags & INT6K_BAILOUT), errno, "invalid packet or result");
				break;
			case EBADMSG:
				error ((int6k->flags & INT6K_BAILOUT), errno, "wrong message version or vendor");
				break;
			case ENOSYS:
				error ((int6k->flags & INT6K_BAILOUT), errno, "wrong message response");
				break;
			case EPROTONOSUPPORT:
				error ((int6k->flags & INT6K_BAILOUT), errno, "wrong ethernet protocol");
				break;
			default:
				error ((int6k->flags & INT6K_BAILOUT), errno, "unknown RxFrame error");
				break;
			}
			continue;
		}
		break;
	}
	return (int6k->packetsize);
}

#endif

