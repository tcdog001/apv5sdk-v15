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
 *   void CGIRestore ();
 *
 *   session.h
 *
 *   restore factory default settings;
 *
 *   this module is for CGI applications and not for general use;
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
 *.  Intellon INT6000 Linux Toolkit for HomePlug AV;
 *:  Published 2006-2009 by Intellon Corp. ALL RIGHTS RESERVED;
 *;  For demonstration; Not for production use;
 *
 *   Contributor(s):
 *      Charles Maier <charles.maier@intellon.com>
 *
 *--------------------------------------------------------------------*/

#ifndef CGIRESTORE_SOURCE
#define CGIRESTORE_SOURCE

#include <stdint.h>
#include <memory.h>
#include <errno.h>

#include "../int6k/intellon.h"
#include "../ether/channel.h"
#include "../tools/memory.h"
#include "../cgi/session.h"
#include "../cgi/page.h"

void CGIRestore () 

{
	extern struct session session;
	extern struct channel channel;
	extern struct message message;

#ifndef __GNUC__
#pragma pack (push,1)
#endif

	struct __packed vs_fac_def_cnf 
	{
		struct header_eth ethernet;
		struct header_int intellon;
		uint8_t MSTATUS;
	}
	* confirm = (struct vs_fac_def_cnf *) (&message);

#ifndef __GNUC__
#pragma pack (pop)
#endif

	memset (&message, 0, sizeof (message));
	EthernetHeader (&message.ethernet, session.device.MAC, channel.host);
	IntellonHeader (&message.intellon, (VS_FAC_DEFAULT | MMTYPE_REQ));
	if (sendpacket (&channel, &message, ETHER_MIN_LEN) <= 0) 
	{
		status (0, EIO, INT6K_CANTSEND);
	}
	if (readpacket (&channel, &message, sizeof (message)) <= 0) 
	{
		status (0, EIO, INT6K_CANTREAD);
	}
	if (confirm->MSTATUS) 
	{
		status (0, EPERM, INT6K_WONTDOIT);
	}
	return;
}

#endif

