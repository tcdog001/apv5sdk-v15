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
 *   signed WriteMOD (struct channel * channel, uint8_t module, const void * memory, size_t extent);
 *   
 *   int6k.h
 *
 *   This plugin writes a memory resident NVM image to device SDRAM
 *   using as many VS_WR_MOD messages as needed to complete transfer; 
 *   runtime firmware must be running for this to work; the NVM file
 *   in struct int6k must be opened before calling this function;
 *   
 *   This function is functionally similar to WriteNVM but downloads
 *   the image from memory instead of reading the image from a file;
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
 *.  Intellon INT6x00 Linux Toolkit for HomePlug AV; 
 *:  Published 2007 by Intellon Corp. ALL RIGHTS RESERVED;
 *;  For demonstration; Not for production use;
 *
 *   Contributor(s):
 *      Charles Maier <charles.maier@intellon.com>
 *      Nathan Houghton <nathan.houghton@intellon.com>
 *
 *--------------------------------------------------------------------*/

#ifndef WRITEMOD_SOURCE
#define WRITEMOD_SOURCE

#include <unistd.h>
#include <stdint.h>
#include <memory.h>

#include "../int6k/int6k.h" 
#include "../tools/memory.h"
#include "../tools/error.h"
#include "../tools/files.h"

signed WriteMOD (struct channel * channel, uint8_t module, const void * memory, size_t extent) 

{
	struct message message;

#ifndef __GNUC__
#pragma pack (push,1)
#endif

	struct __packed vs_wr_mod_request 
	{
		struct header_eth ethernet;
		struct header_int intellon;
		uint8_t MODULEID;
		uint8_t MACCESS;
		uint16_t MLENGTH;
		uint32_t MOFFSET;
		uint32_t MCHKSUM;
		uint8_t MBUFFER [INT6K_BLOCKSIZE];
	}
	* request = (struct vs_wr_mod_request *) (&message);
	struct __packed vs_wr_mod_confirm 
	{
		struct header_eth ethernet;
		struct header_int intellon;
		uint8_t MSTATUS;
		uint8_t MODULEID;
		uint8_t MACCESS;
		uint16_t MLENGTH;
		uint32_t MOFFSET;
	}
	* confirm = (struct vs_wr_mod_confirm *) (&message);

#ifndef __GNUC__
#pragma pack (pop)
#endif

	uint32_t length = INT6K_BLOCKSIZE;
	uint32_t offset = 0;
	while (extent) 
	{
		memset (&message, 0, sizeof (message));
		EthernetHeader (&message.ethernet, channel->peer, channel->host);
		IntellonHeader (&message.intellon, (VS_WR_MOD | MMTYPE_REQ));
		if (length > extent) 
		{
			length = extent;
		}
		memcpy (request->MBUFFER, (byte *)(memory) + offset, length);
		request->MODULEID = module;
		request->MACCESS = 0;
		request->MLENGTH = HTOLE16 (length);
		request->MOFFSET = HTOLE32 (offset);
		request->MCHKSUM = checksum_32 (request->MBUFFER, length, 0);
		if (sendpacket (channel, &message, sizeof (request)) <= 0) 
		{
			error (0, ECANCELED, INT6K_CANTSEND);
			return (-1);
		}
		if (readpacket (channel, &message, sizeof (message)) <= 0) 
		{
			error (0, ECANCELED, INT6K_CANTREAD);
			return (-1);
		}
		if (confirm->MSTATUS) 
		{
			error (0, ECANCELED, INT6K_WONTDOIT);
			return (-1);
		}

#if 1

		if (LE16TOH (confirm->MLENGTH) != length) 
		{
			error (0, 0, INT6K_ERR_LENGTH);
			return (-1);
		}
		if (LE32TOH (confirm->MOFFSET) != offset) 
		{
			error (0, 0, INT6K_ERR_OFFSET);
			return (-1);
		}

#else

		if (LE16TOH (confirm->MLENGTH) != length) 
		{
			error (0, 0, INT6K_ERR_LENGTH);
			length = INT6K_BLOCKSIZE;
			offset = 0;
			continue;
		}
		if (LE32TOH (confirm->MOFFSET) != offset) 
		{
			error (0, 0, INT6K_ERR_OFFSET);
			length = INT6K_BLOCKSIZE;
			offset = 0;
			continue;
		}

#endif

		extent -= length;
		offset += length;
	}
	return (0);
}

#endif

