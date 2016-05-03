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
 *   void SessionPeek ();
 *
 *   session.h
 *
 *   send session variable information to HTML client for development
 *   and debugging purposes; 
 *
 *.  Intellon INT6000 Linux Toolkit for HomePlug AV;
 *:  Published 2006-2009 by Intellon Corp. ALL RIGHTS RESERVED;
 *;  For demonstration; Not for production use;
 *
 *   Contributor(s):
 *	Charles Maier <charles.maier@intellon.com>
 *
 *--------------------------------------------------------------------*/

#ifndef SESSIONPEEK_SOURCE
#define SESSIONPEEk_SOURCE

#include "../cgi/page.h"
#include "../cgi/session.h"
#include "../tools/memory.h"
#include "../tools/format.h"

void SessionPeek () 

{
	extern struct session session;
	char address [ETHER_ADDR_LEN * 3];
	signed margin = 0;
	unsigned bridge = 0;
	output (margin++, "<div class ='%s' style='background:yellow;'>", STYLE_PEEK);
	output (margin++, "<table>");
	output (margin++, "<tr>");
	output (margin++, "<td>");
	printf ("%s=%s<br/>", SUBMIT_OPTION, session.option);
	printf ("%s=%s<br/>", SUBMIT_ACTION, session.action);
	printf ("%s=%s<br/>", SUBMIT_SOURCE, session.source);
	while (bridge < session.bridges)
	{
		printf ("bridge=%s<br/>", hexstring (address, sizeof (address), session.bridge[bridge++].MAC, ETHER_ADDR_LEN));
	}
	printf ("device=%s<br/>", hexstring (address, sizeof (address), session.device.MAC, ETHER_ADDR_LEN));
	printf ("DAK=%s DPW='%s'<br/>", hexstring (address, sizeof (address), session.device.DAK, sizeof (session.device.DAK)), session.DPW);
	printf ("NMK=%s NPW='%s'<br/>", hexstring (address, sizeof (address), session.device.NMK, sizeof (session.device.NMK)), session.NPW);
	printf ("NID=%s<br/>", hexstring (address, sizeof (address), session.device.NID, sizeof (session.device.NID)));
	printf ("method=%d,%d<br/>", session.QoS.method [0], session.QoS.method [1]);
	output (margin--, "</td>");
	output (margin--, "</tr>");
	output (margin--, "</table>");
	output (margin--, "</div>");
	return;
}

#endif

