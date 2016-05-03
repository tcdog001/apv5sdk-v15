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
 *   signed DivisionDevice (signed margin, const char * style);
 *
 *   page.h
 *
 *.  Intellon INT6000 Linux Toolkit for HomePlug AV;
 *:  Published 2006-2009 by Intellon Corp. ALL RIGHTS RESERVED;
 *;  For demonstration; Not for production use;
 *
 *--------------------------------------------------------------------*/

#ifndef UNIT_SOURCE
#define UNIT_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <errno.h>

#include "../int6k/int6k.h"
#include "../tools/memory.h"
#include "../tools/format.h"
#include "../tools/error.h"
#include "../ether/channel.h"
#include "../ether/ether.h"
#include "../cgi/session.h"
#include "../cgi/page.h"

/*====================================================================*
 *
 *   signed select_source (signed margin, const char * style);
 *   
 *   display a drop-down list of host network interfaces and a submit
 *   buttion;
 *
 *--------------------------------------------------------------------*/

static signed select_source (signed margin, const char * style) 

{
	extern struct session session;
	struct if_nameindex * interfaces;
	struct if_nameindex * interface;
	output (margin++, "<th class='%s'>", style);
	output (margin, "<input class='%s' type='submit' name='%s' value='%s' title=' Select this Network Interface '>", style, SUBMIT_ACTION, ACTION_SOURCE);
	output (margin--, "</th>");
	output (margin++, "<td class='%s'>", style);
	output (margin++, "<select class='%s' name='%s'>", style, SUBMIT_SOURCE);
	for (interface = interfaces = if_nameindex (); interface->if_index; interface++) 
	{
		if (!strcmp (interface->if_name, session.source)) 
		{
			output (margin++, "<option class='%s' value='%s' selected>", style, interface->if_name);
		}
		else 
		{
			output (margin++, "<option class='%s' value='%s'>", style, interface->if_name);
		}
		output (margin, "%*s", (signed)(sizeof (interface->if_name)), interface->if_name);
		output (margin--, "</option>");
	}
	if_freenameindex (interfaces);
	output (margin--, "</select>");
	output (margin--, "</td>");
	return (margin);
}

/*====================================================================*
 *
 *   signed select_device (signed margin, const char * style);
 *   
 *   display a drop-down list of accessible powerline devices and an associated submit buttion;
 *
 *--------------------------------------------------------------------*/

static signed select_device (signed margin, const char * style) 

{
	extern struct channel channel;
	extern struct session session;
	extern struct message message;
	extern const byte localcast [ETHER_ADDR_LEN];

#ifndef __GNUC__
#pragma pack (push,1)
#endif

	struct __packed station 
	{
		uint8_t LDA [ETHER_ADDR_LEN];
		uint8_t TEI;
		uint8_t BDA [ETHER_ADDR_LEN];
		uint8_t AVGTX;
		uint8_t AVGRX;
	}
	* station;
	struct __packed network 
	{
		uint8_t NID [7];
		uint8_t SNID;
		uint8_t TEI;
		uint8_t ROLE;
		uint8_t CCO_MAC [ETHER_ADDR_LEN];
		uint8_t CCO_TEI;
		uint8_t NUMSTAS;
		struct station station [1];
	}
	* network;
	struct __packed networks 
	{
		uint8_t NUMAVLNS;
		struct network network [1];
	}
	* networks = (struct networks *) (&message.content);

#ifndef __GNUC__
#pragma pack (pop)
#endif

	output (margin++, "<th class='%s'>", style);
	output (margin, "<input class='%s' type='submit' name='%s' value='%s' title=' Select this Powerline Device '>", style, SUBMIT_ACTION, ACTION_DEVICE);
	output (margin--, "</th>");
	output (margin++, "<td class='%s'>", style);
	output (margin++, "<select class='%s' name='%s'>", style, SUBMIT_DEVICE);
	memset (&message, 0, sizeof (message));
	EthernetHeader (&message.ethernet, localcast, channel.host);
	IntellonHeader (&message.intellon, (VS_NW_INFO | MMTYPE_REQ));
	if (sendpacket (&channel, &message, ETHER_MIN_LEN) <= 0) 
	{
		error (1, ECANCELED, INT6K_CANTSEND);
	}
	while (readpacket (&channel, &message, sizeof (message)) > 0) 
	{
		char address [ETHER_ADDR_LEN * 3];
		if (UnwantedPacket (&message, (VS_NW_INFO | MMTYPE_CNF))) 
		{
			continue;
		}
		hexdecode (message.ethernet.OSA, sizeof (message.ethernet.OSA), address, sizeof (address));
		if (!memcmp (message.ethernet.OSA, session.device.MAC, ETHER_ADDR_LEN)) 
		{
			output (margin++, "<option class='%s' value='%s' selected>", style, address);
		}
		else 
		{
			output (margin++, "<option class='%s' value='%s'>", style, address);
		}
		output (margin, "L %s", address);
		output (margin--, "</option>");
		network = (struct network *)(networks->network);
		while (networks->NUMAVLNS-- > 0) 
		{
			station = (struct station *)(network->station);
			while (network->NUMSTAS-- > 0) 
			{
				hexdecode (station->LDA, sizeof (station->LDA), address, sizeof (address));
				if (!memcmp (station->LDA, session.device.MAC, ETHER_ADDR_LEN)) 
				{
					output (margin++, "<option class='%s' value='%s' selected>", style, address);
				}
				else 
				{
					output (margin++, "<option class='%s' value='%s'>", style, address);
				}
				output (margin, "R %s", address);
				output (margin--, "</option>");
				station++;
			}
			network = (struct network *)(station);
		}
	}
	output (margin--, "</select>");
	output (margin--, "</td>");
	return (margin);
}

/*====================================================================*
 *
 *   signed DivisionDevice (signed margin, const char * style);
 *
 *   print the source/device selection division on stdout;
 *
 *--------------------------------------------------------------------*/

signed DivisionDevice (signed margin, const char * style) 

{
	output (margin++, "<div class='%s'>", style);
	output (margin++, "<table class='%s'>", style);
	output (margin++, "<tr class='%s'>", style);
	margin = select_source (margin, STYLE_SOURCE);
	margin = select_device (margin, STYLE_DEVICE);
	output (margin--, "</tr>");
	output (margin--, "</table>");
	output (margin--, "</div>");
	return (margin);
}

#endif

