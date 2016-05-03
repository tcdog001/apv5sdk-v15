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
 *   signed OptionTopology (signed margin, const char * style);
 *   
 *   page.h
 *   
 *.  Intellon INT6000 Linux Toolkit for HomePlug AV;
 *:  Published 2006-2009 by Intellon Corp. ALL RIGHTS RESERVED;
 *;  For demonstration; Not for production use;
 *
 *   Contributor(s):
 *	Charles Maier <charles.maier@intellon.com>
 *
 *--------------------------------------------------------------------*/

#ifndef OPTIONTOPOLOGY_SOURCE
#define OPTIONTOPOLOGY_SOURCE

#include <memory.h>
#include <errno.h>

#include "../int6k/int6k.h"
#include "../ether/channel.h"
#include "../tools/memory.h"
#include "../tools/format.h"
#include "../tools/error.h"
#include "../cgi/session.h"
#include "../cgi/page.h"

#define TARGET "background: yellow;"
#define LOC "LOC"
#define REM "REM"
#define CCO "CCO"
#define STA "STA"

/*====================================================================*
 *
 *   signed platform (signed margin, const char * style, const byte device []);
 *
 *
 *--------------------------------------------------------------------*/

static signed platform (signed margin, const char * style, const byte device []) 

{
	extern struct channel channel;
	extern const char * chipset [CHIPSETS+1];
	struct message message;

#ifndef __GNUC__
#pragma pack (push,1)
#endif

	struct __packed vs_sw_ver_cnf 
	{
		struct header_eth ethernet;
		struct header_int intellon;
		uint8_t MSTATUS;
		uint8_t MDEVICEID;
		uint8_t MVERLENGTH;
		char MVERSION [INT6K_VERSTRING];
	}
	* confirm = (struct vs_sw_ver_cnf *) (&message);

#ifndef __GNUC__
#pragma pack (pop)
#endif

	memset (&message, 0, sizeof (message));
	EthernetHeader (&message.ethernet, device, channel.host);
	IntellonHeader (&message.intellon, (VS_SW_VER | MMTYPE_REQ));
	if (sendpacket (&channel, &message, ETHER_MIN_LEN) <= 0) 
	{

// margin = report (margin, errno, INT6K_CANTSEND);
// return (margin);

	}
	if (readpacket (&channel, &message, sizeof (message)) <= 0) 
	{

// margin = report (margin, errno, INT6K_CANTREAD);
// return (margin);

	}
	output (margin++, "<td class='%s'>", style);
	output (margin, "%s", chipset [confirm->MDEVICEID]);
	output (margin--, "</td>");
	output (margin++, "<td class='%s'>", style);
	output (margin, "%s", confirm->MVERSION);
	output (margin--, "</td>");
	return (margin);
}

/*====================================================================*
 *
 *   signed OptionTopology (signed margin, const char * style);


 *--------------------------------------------------------------------*/

signed OptionTopology (signed margin, const char * style) 

{
	extern struct channel channel;
	extern struct message message;
	extern struct session session;

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

	unsigned bridge = session.bridges;
	const char * mark;
	const char * link;
	const char * role;
	output (margin++, "<div class='%s'>", style);
	output (margin++, "<table class='%s'>", style);
	while (bridge--) 
	{
		memset (&message, 0, sizeof (message));
		memcpy (channel.peer, session.bridge [bridge].MAC, sizeof (channel.peer));
		EthernetHeader (&message.ethernet, channel.peer, channel.host);
		IntellonHeader (&message.intellon, (VS_NW_INFO | MMTYPE_REQ));
		if (sendpacket (&channel, &message, ETHER_MIN_LEN) <= 0) 
		{
			margin = report (margin, ENODATA, INT6K_CANTSEND);
			return (margin);
		}
		while (readpacket (&channel, &message, sizeof (message)) > 0) 
		{
			char address [ETHER_ADDR_LEN * 3];
			if (UnwantedPacket (&message, (VS_NW_INFO | MMTYPE_CNF))) 
			{
				continue;
			}
			output (margin++, "<tr class='%s'>", style);
			output (margin++, "<th class='%s'>", style);
			output (margin, "P/L");
			output (margin--, "</th>");
			output (margin++, "<th class='%s'>", style);
			output (margin, "NET");
			output (margin--, "</th>");
			output (margin++, "<th class='%s'>", style);
			output (margin, "%s", "TEI");
			output (margin--, "</th>");
			output (margin++, "<th class='%s'>", style);
			output (margin, "%s", "------ MAC ------");
			output (margin--, "</th>");
			output (margin++, "<th class='%s'>", style);
			output (margin, "%s", "------ BDA ------");
			output (margin--, "</th>");
			output (margin++, "<th class='%s'>", style);
			output (margin, "%s", "TX");
			output (margin--, "</th>");
			output (margin++, "<th class='%s'>", style);
			output (margin, "%s", "RX");
			output (margin--, "</th>");
			output (margin++, "<th class='%s'>", style);
			output (margin, "%s", "CHIPSET");
			output (margin--, "</th>");
			output (margin++, "<th class='%s' style='width:100%%; text-align: left;'>", style);
			output (margin, "%s", "FIRMWARE");
			output (margin--, "</th>");
			output (margin--, "</tr>");
			network = (struct network *)(networks->network);
			mark = memcmp (message.ethernet.OSA, session.device.MAC, ETHER_ADDR_LEN)? "none": TARGET;
			link = memcmp (message.ethernet.OSA, session.bridge [bridge].MAC, ETHER_ADDR_LEN)? REM: LOC;
			role = memcmp (message.ethernet.OSA, network->CCO_MAC, ETHER_ADDR_LEN)? STA: CCO;
			output (margin++, "<tr class='%s' style='%s'>", style, mark);
			output (margin++, "<td class='%s'>", style);
			output (margin, "LOC");
			output (margin--, "</td>");
			output (margin++, "<td class='%s'>", style);
			output (margin, "%s", role);
			output (margin--, "</td>");
			output (margin++, "<td class='%s'>", style);
			output (margin, "%03d", network->TEI);
			output (margin--, "</td>");
			output (margin++, "<td class='%s'>", style);
			output (margin, "%s", hexstring (address, sizeof (address), message.ethernet.OSA, sizeof (message.ethernet.OSA)));
			output (margin--, "</td>");
			output (margin++, "<td class='%s'>", style);
			output (margin, "%s", hexstring (address, sizeof (address), message.ethernet.ODA, sizeof (message.ethernet.ODA)));
			output (margin--, "</td>");
			output (margin++, "<td class='%s'>", style);
			output (margin, "n/a");
			output (margin--, "</td>");
			output (margin++, "<td class='%s'>", style);
			output (margin, "n/a");
			output (margin--, "</td>");
			margin = platform (margin, style, message.ethernet.OSA);
			output (margin--, "</tr>");
			while (networks->NUMAVLNS-- > 0) 
			{
				station = (struct station *)(network->station);
				while (network->NUMSTAS-- > 0) 
				{
					mark = memcmp (station->LDA, session.device.MAC, ETHER_ADDR_LEN)? "none": TARGET;
					link = memcmp (station->BDA, session.bridge [bridge].MAC, ETHER_ADDR_LEN)? REM: LOC;
					role = memcmp (station->LDA, network->CCO_MAC, ETHER_ADDR_LEN)? STA: CCO;
					output (margin++, "<tr class='%s' style='%s'>", style, mark);
					output (margin++, "<td class='%s'>", style);
					output (margin, "REM");
					output (margin--, "</td>");
					output (margin++, "<td class='%s'>", style);
					output (margin, "%s", role);
					output (margin--, "</td>");
					output (margin++, "<td class='%s'>", style);
					output (margin, "%03d", station->TEI);
					output (margin--, "</td>");
					output (margin++, "<td class='%s'>", style);
					output (margin, "%s", hexstring (address, sizeof (address), station->LDA, sizeof (station->LDA)));
					output (margin--, "</td>");
					output (margin++, "<td class='%s'>", style);
					output (margin, "%s", hexstring (address, sizeof (address), station->BDA, sizeof (station->BDA)));
					output (margin--, "</td>");
					output (margin++, "<td class='%s'>", style);
					output (margin, "%03d", station->AVGTX);
					output (margin--, "</td>");
					output (margin++, "<td class='%s'>", style);
					output (margin, "%03d", station->AVGRX);
					output (margin--, "</td>");
					margin = platform (margin, style, station->LDA);
					output (margin--, "</tr>");
					station++;
				}
				network = (struct network *)(station);
			}
		}
	}
	output (margin--, "</table>");
	output (margin--, "</div>");
	return (margin);
}

#endif

