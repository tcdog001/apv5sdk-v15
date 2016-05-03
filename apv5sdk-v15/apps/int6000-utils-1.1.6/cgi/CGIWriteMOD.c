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
 *   void CGIWriteMOD (uint8_t module, const void * memory, signed extent);
 *   
 *   session.h 
 *
 *   write a memory region to the current powerline device using as
 *   many VS_WR_MOD message as needed;
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

#ifndef CGIWRITEMOD_SOURCE
#define CGIWRITEMOD_SOURCE

#include <unistd.h>
#include <stdint.h>
#include <memory.h>
#include <errno.h>

#include "../int6k/int6k.h" 
#include "../ether/channel.h"
#include "../tools/memory.h"
#include "../cgi/session.h"
#include "../cgi/page.h"

void CGIWriteMOD (uint8_t module, const void * memory, signed extent) 

{
	extern struct session session;
	extern struct channel channel;
	extern struct message message;

#ifndef __GNUC__
#pragma pack (push,1)
#endif

	struct __packed vs_wr_mod_req 
	{
		struct header_eth ethernet;
		struct header_int intellon;
		uint8_t MODULEID;
		uint8_t RESERVED;
		uint16_t MLENGTH;
		uint32_t MOFFSET;
		uint32_t CHKSUM;
		uint8_t MBUFFER [INT6K_BLOCKSIZE];
	}
	* request = (struct vs_wr_mod_req *) (&message);
	struct __packed vs_wr_mod_cnf 
	{
		struct header_eth ethernet;
		struct header_int intellon;
		uint8_t MSTATUS;
		uint8_t MODULEID;
		uint8_t RESERVED;
		uint16_t MLENGTH;
		uint32_t MOFFSET;
	}
	* confirm = (struct vs_wr_mod_cnf *) (&message);

#ifndef __GNUC__
#pragma pack (pop)
#endif

	uint32_t length = INT6K_BLOCKSIZE;
	uint32_t offset = 0;
	while (extent) 
	{
		memset (&message, 0, sizeof (message));
		EthernetHeader (&request->ethernet, session.device.MAC, channel.host);
		IntellonHeader (&request->intellon, (VS_WR_MOD | MMTYPE_REQ));
		if (length > extent) 
		{
			length = extent;
		}
		memcpy (request->MBUFFER, memory + offset, length);
		request->MODULEID = module;
		request->RESERVED = 0;
		request->MLENGTH = HTOLE16 (length);
		request->MOFFSET = HTOLE32 (offset);
		request->CHKSUM = checksum_32 (request->MBUFFER, length, 0);
		if (sendpacket (&channel, &message, sizeof (message)) <= 0) 
		{
			status (0, EIO, INT6K_CANTSEND);
		}
		if (readpacket (&channel, &message, sizeof (message)) <= 0) 
		{
			status (0, EIO, INT6K_CANTREAD);
		}
		if (confirm->MSTATUS) 
		{
			status (0, EPERM, INT6K_WONTDOIT " (0x%02X)", confirm->MSTATUS);
		}

#if 1

		if (LE16TOH (confirm->MLENGTH) != length) 
		{
			status (0, EIO, INT6K_ERR_LENGTH);
		}
		if (LE32TOH (confirm->MOFFSET) != offset) 
		{
			status (0, EIO, INT6K_ERR_OFFSET);
		}

#else

		if (LE16TOH (confirm->MLENGTH) != length) 
		{
			error (0, EIO, INT6K_ERR_LENGTH);
			length = INT6K_BLOCKSIZE;
			offset = 0;
			continue;
		}
		if (LE32TOH (confirm->MOFFSET) != offset) 
		{
			error (0, EIO, INT6K_ERR_OFFSET);
			length = INT6K_BLOCKSIZE;
			offset = 0;
			continue;
		}

#endif

		extent -= length;
		offset += length;
	}
	return;
}

#endif

