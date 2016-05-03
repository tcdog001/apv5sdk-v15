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
 *   int WriteMEM (struct int6k * int6k, struct _file_ * file, uint32_t offset, uint32_t length);
 *
 *   int6k.h
 *
 *   Write the contents of a file directly into device SDRAM using
 *   VS_WR_MEM messages; the bootloader must be running for this to
 *   work; the function is used to dowload firmware or test applets;
 *
 *   this function makes no attempt to validate the information sent
 *   to the device;
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
 *      Nathan Houghton <nathan.houghton@intellon.com>
 *      Charles Maier <charles.maier@intellon.com>
 *
 *--------------------------------------------------------------------*/

#ifndef WRITEMEM_SOURCE
#define WRITEMEM_SOURCE

#include <stdint.h>
#include <unistd.h>
#include <memory.h>

#include "../int6k/int6k.h"
#include "../tools/memory.h"
#include "../tools/error.h"
#include "../tools/files.h"

int WriteMEM (struct int6k * int6k, struct _file_ * file, uint32_t offset, uint32_t extent) 

{

	struct channel * channel = (struct channel *)(int6k->channel);
	struct message * message = (struct message *)(int6k->message);

#ifndef __GNUC__
#pragma pack (push,1)
#endif

	struct __packed vs_wr_mem_request 
	{
		struct header_eth ethernet;
		struct header_int intellon;
		uint32_t MOFFSET;
		uint32_t MLENGTH;
		uint8_t BUFFER [INT6K_BLOCKSIZE];
	}
	* request = (struct vs_wr_mem_request *) (message);
	struct __packed vs_wr_mem_confirm 
	{
		struct header_eth ethernet;
		struct header_int intellon;
		uint8_t MSTATUS;
		uint32_t MOFFSET;
		uint32_t MLENGTH;
	}
	* confirm = (struct vs_wr_mem_confirm *) (message);

#ifndef __GNUC__
#pragma pack (pop)
#endif

	signed length = INT6K_BLOCKSIZE;
	Request (int6k, "Write Memory (%08X:%d)", offset, extent);
	while (extent) 
	{
		memset (message, 0, sizeof (struct message));
		EthernetHeader (&message->ethernet, channel->peer, channel->host);
		IntellonHeader (&message->intellon, (VS_WR_MEM | MMTYPE_REQ));
		if (length > (signed)(extent)) 
		{
			length = extent;
		}
		if (read (file->file, request->BUFFER, length) < length) 
		{
			error (1, errno, "Can't read %s", filepart (file->name));
		}
		request->MLENGTH = HTOLE32 (length);
		request->MOFFSET = HTOLE32 (offset);
		int6k->packetsize = sizeof (struct vs_wr_mem_request);
		if (SendMME (int6k) <= 0) 
		{
			error ((int6k->flags & INT6K_BAILOUT), ECANCELED, INT6K_CANTSEND);
			return (-1);
		}
		if (ReadMME (int6k, (VS_WR_MEM | MMTYPE_CNF)) <= 0) 
		{
			error ((int6k->flags & INT6K_BAILOUT), ECANCELED, INT6K_CANTREAD);
			return (-1);
		}
		if (confirm->MSTATUS) 
		{
			Failure (int6k, INT6K_WONTDOIT);
			return (-1);
		}

#if 1

		if (LE32TOH (confirm->MLENGTH) != length) 
		{
			Failure (int6k, INT6K_ERR_LENGTH);
			return (-1);
		}
		if (LE32TOH (confirm->MOFFSET) != offset) 
		{
			Failure (int6k, INT6K_ERR_OFFSET);
			return (-1);
		}

#else

		if (LE32TOH (confirm->MLENGTH) != length) 
		{
			error ((int6k->flags & INT6K_BAILOUT), 0, INT6K_ERR_LENGTH);
			length = INT6K_BLOCKSIZE;
			offset = 0;
			continue;
		}
		if (LE32TOH (confirm->MOFFSET) != offset) 
		{
			error ((int6k->flags & INT6K_BAILOUT), 0, INT6K_ERR_OFFSET);
			length = INT6K_BLOCKSIZE;
			offset = 0;
			continue;
		}

#endif

		extent -= length;
		offset += length;
	}
	Confirm (int6k, "Wrote %s", filepart (file->name));
	return (0);
}

#endif

