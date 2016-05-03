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
 *   signed OptionIdentity (signed margin, const char * style);
 *
 *   page.h
 *
 *   print an identity tab on stdout;
 *
 *.  Intellon INT6000 Linux Toolkit for HomePlug AV;
 *:  Published 2006-2009 by Intellon Corp. ALL RIGHTS RESERVED;
 *;  For demonstration; Not for production use;
 *
 *   Contributor(s):
 *	Charles Maier <charles.maier@intellon.com>
 *
 *--------------------------------------------------------------------*/

#ifndef OPTIONIDENTITY_SOURCE
#define OPTIONIDENTITY_SOURCE

#include <errno.h>

#include "../int6k/int6k.h"
#include "../ether/channel.h"
#include "../tools/memory.h"
#include "../tools/format.h"
#include "../tools/error.h"
#include "../cgi/session.h"
#include "../cgi/page.h"
#include "../pib/pib.h"

signed OptionIdentity (signed margin, const char * style) 

{
	extern struct channel channel;
	extern struct session session;
	extern struct message message;
	extern const char * chipset [CHIPSETS+1];
	extern const char * CCoMode [PIB_CCOMODES];
	extern const char * MDURole [PIB_MDUROLES];

#ifndef __GNUC__
#pragma pack (push,1)
#endif

	struct __packed vs_rd_mod_req 
	{
		struct header_eth ethernet;
		struct header_int intellon;
		uint8_t MODULEID;
		uint8_t RESERVED;
		uint16_t LENGTH;
		uint32_t OFFSET;
	}
	* request = (struct vs_rd_mod_req *) (&message);
	struct __packed vs_sw_ver_cnf 
	{
		struct header_eth header_eth;
		struct header_int header_int;
		uint8_t MSTATUS;
		uint8_t MDEVICEID;
		uint8_t MVERLENGTH;
		char MVERSION [128];
		uint8_t UPGRADEABLE;
		uint32_t MCOOKIE;
	}
	* version = (struct vs_sw_ver_cnf *) (&message);
	struct __packed vs_rd_mod_cnf 
	{
		struct header_eth ethernet;
		struct header_int intellon;
		uint8_t MSTATUS;
		uint8_t RES [3];
		uint8_t MODULEID;
		uint8_t RESERVED;
		uint16_t LENGTH;
		uint32_t OFFSET;
		uint32_t CHKSUM;
		uint8_t BUFFER [INT6K_BLOCKSIZE];
	}
	* confirm = (struct vs_rd_mod_cnf *) (&message);

#ifndef __GNUC__
#pragma pack (pop)
#endif

	struct PIB1_5 * pib = (struct PIB1_5 *)(&confirm->BUFFER);
	char firmware [INT6K_VERSTRING];
	byte platform;
	char buffer [48];
	memset (&message, 0, sizeof (message));
	EthernetHeader (&request->ethernet, session.device.MAC, channel.host);
	IntellonHeader (&request->intellon, (VS_SW_VER | MMTYPE_REQ));
	if (sendpacket (&channel, &message, ETHER_MIN_LEN) <= 0) 
	{
		margin = report (margin, ECANCELED, INT6K_CANTREAD);
		return (margin);
	}
	if (readpacket (&channel, &message, sizeof (message)) <= 0) 
	{
		margin = report (margin, ENODATA, INT6K_CANTSEND);
		return (margin);
	}
	platform = version->MDEVICEID;
	memcpy (firmware, version->MVERSION, sizeof (firmware));
	memset (&message, 0, sizeof (message));
	EthernetHeader (&request->ethernet, session.device.MAC, channel.host);
	IntellonHeader (&request->intellon, (VS_RD_MOD | MMTYPE_REQ));
	request->MODULEID = VS_MODULE_PIB;
	request->LENGTH = INT6K_BLOCKSIZE;
	request->OFFSET = 0;
	if (sendpacket (&channel, &message, ETHER_MIN_LEN) <= 0) 
	{
		margin = report (margin, ECANCELED, INT6K_CANTREAD);
		return (margin);
	}
	if (readpacket (&channel, &message, sizeof (message)) <= 0) 
	{
		margin = report (margin, ENODATA, INT6K_CANTSEND);
		return (margin);
	}
	output (margin++, "<div class='%s'>", style);
	output (margin++, "<pre class='%s'>", style);
	output (1, " ");
	output (1, "Hardware %s", chipset [platform]);
	output (1, "Firmware %s", firmware);
	output (1, " ");
	output (1, "PIB %d-%d %d bytes", pib->VersionHeader.FWVersion, pib->VersionHeader.PIBVersion, LE16TOH (pib->VersionHeader.PIBLength));
	output (1, "MAC %s", hexstring (buffer, sizeof (buffer), pib->LocalDeviceConfig.MAC, sizeof (pib->LocalDeviceConfig.MAC)));
	output (1, "DAK %s", hexstring (buffer, sizeof (buffer), pib->LocalDeviceConfig.DAK, sizeof (pib->LocalDeviceConfig.DAK)));
	output (1, "NMK %s", hexstring (buffer, sizeof (buffer), pib->LocalDeviceConfig.NMK, sizeof (pib->LocalDeviceConfig.NMK)));
	output (1, "NID %s", hexstring (buffer, sizeof (buffer), pib->LocalDeviceConfig.PreferredNID, sizeof (pib->LocalDeviceConfig.PreferredNID)));
	output (1, "NET %s", pib->LocalDeviceConfig.NET);
	output (1, "MFG %s", pib->LocalDeviceConfig.MFG);
	output (1, "USR %s", pib->LocalDeviceConfig.USR);
	output (1, "CCo %s", CCoMode [pib->LocalDeviceConfig.CCoSelection]);
	output (1, "MDU %s", pib->LocalDeviceConfig.MDUConfiguration? MDURole [pib->LocalDeviceConfig.MDURole]: "N/A");
	output (margin--, "</pre>");
	output (margin--, "</div>");
	return (margin);
}

#endif

