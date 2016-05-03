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
 *   void CGILocalPLD ();
 *
 *   session.h
 *
 *
 *--------------------------------------------------------------------*/

#ifndef CGILOCALPLD_SOURCE
#define CGILOCALPLD_SOURCE

#include <stdint.h>
#include <memory.h>
#include <errno.h>

#include "../cgi/session.h"
#include "../ether/channel.h"
#include "../cgi/page.h"

void CGILocalPLD () 

{
	extern struct session session;
	extern struct channel channel;
	extern struct message message;
	extern const byte localcast [ETHER_ADDR_LEN];
	memset (&message, 0, sizeof (message));
	EthernetHeader (&message.ethernet, localcast, channel.host);
	IntellonHeader (&message.intellon, (VS_SW_VER | MMTYPE_REQ));
	if (sendpacket (&channel, &message, ETHER_MIN_LEN) <= 0) 
	{
		status (0, EIO, INT6K_CANTSEND);
	}
	for (session.bridges = 0; readpacket (&channel, &message, sizeof (message)) > 0; session.bridges++) 
	{
		memcpy (session.bridge [session.bridges].MAC, message.ethernet.OSA, ETHER_ADDR_LEN);
	}
	return;
}

#endif

