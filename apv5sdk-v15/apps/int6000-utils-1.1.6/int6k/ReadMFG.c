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
 *   signed ReadMFG (struct int6k * int6k, uint16_t MMTYPE);
 *
 *   int6k.h
 *
 *   read a manufacturer specific management message of the given
 *   type in platform independent manner; return the number of bytes 
 *   read, 0 on timeout or -1 on error; 
 *
 *   see SendMME for the send counterpart to this function; 
 *
 *   see ReadMME for the vendor specific version of this function;
 *
 *   readpacket behaves like the read function but there are several 
 *   readpacket functions in the toolkit and each performs raw packet 
 *   i/o differently depending on environment; they all use a channel
 *   structure;
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
 *      Nathan Houghton <nathan.houghton@intellon.com>
 *
 *--------------------------------------------------------------------*/

#ifndef READMFG_SOURCE
#define READMFG_SOURCE

#include <memory.h>

#include "../int6k/int6k.h"
#include "../ether/channel.h"
#include "../tools/memory.h" 
#include "../tools/flags.h"

signed ReadMFG (struct int6k * int6k, uint16_t MMTYPE) 

{
	struct channel * channel = (struct channel *)(int6k->channel);
	struct message * message = (struct message *)(int6k->message);
	memset (message, 0, sizeof (struct message));
	while ((int6k->packetsize = readpacket (channel, message, sizeof (struct message))) > 0) 
	{
		if (message->intellon.MMV != INTELLON_MMV) 
		{
			continue;
		}
		if (message->intellon.MMTYPE != HTOLE16 (MMTYPE)) 
		{
			continue;
		}
		break;
	}
	return (int6k->packetsize);
}

#endif

