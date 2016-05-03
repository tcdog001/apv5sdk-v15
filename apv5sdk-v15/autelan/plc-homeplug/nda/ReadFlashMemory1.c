/*====================================================================*
 *   
 *   Copyright (c) 2011 by Qualcomm Atheros.
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
 *   signed ReadFlashMemory1 (struct plc * plc);
 *
 *   plc.h
 *
 *   read all of flash memory and write to file;
 *   
 *.  Qualcomm Atheros HomePlug AV Powerline Toolkit
 *:  Published 2009-2011 by Qualcomm Atheros. ALL RIGHTS RESERVED
 *;  For demonstration and evaluation only. Not for production use
 *
 *   Contributor(s):
 *      Charles Maier <cmaier@qualcomm.com>
 *
 *--------------------------------------------------------------------*/

#ifndef READFLASHMEMORY1_SOURCE
#define READFLASHMEMORY1_SOURCE

#include <string.h>

#include "../tools/error.h"
#include "../tools/files.h"
#include "../tools/memory.h"
#include "../tools/endian.h"
#include "../plc/plc.h" 
#include "../nda/nda.h"

signed ReadReadFlashMemory1 (struct plc * plc) 

{
	struct channel * channel = (struct channel *)(plc->channel);
	struct message * message = (struct message *)(plc->message);

#ifndef __GNUC__
#pragma pack (push,1)
#endif

	struct __packed vs_rd_blk_nvm_request 
	{
		struct ethernet_std ethernet;
		struct qualcomm_std qualcomm;
		uint16_t MODULEID;
		uint32_t MOFFSET;
		uint32_t MLENGTH;
	}
	* request = (struct vs_rd_blk_nvm_request *) (message);
	struct __packed vs_rd_blk_nvm_confirm 
	{
		struct ethernet_std ethernet;
		struct qualcomm_std qualcomm;
		uint8_t MSTATUS;
		uint16_t MODULEID;
		uint32_t MOFFSET;
		uint32_t MLENGTH;
		uint8_t BUFFER [PLC_RECORD_SIZE];
	}
	* confirm = (struct vs_rd_blk_nvm_confirm *) (message);

#ifndef __GNUC__
#pragma pack (pop)
#endif

	unsigned offset = 0;
	unsigned length = PLC_MODULE_SIZE;
	Request (plc, "Read Flash Memory");
	while (length == PLC_MODULE_SIZE) 
	{
		memset (message, 0, sizeof (* message));
		EthernetHeader (&request->ethernet, channel->peer, channel->host, HOMEPLUG_MTYPE);
		QualcommHeader (&request->qualcomm, 0, (VS_RD_BLK_NVM | MMTYPE_REQ));
		plc->packetsize = (ETHER_MIN_LEN - ETHER_CRC_LEN);
		request->MODULEID = HTOLE16 (MID_USERPIB);
		request->MOFFSET = HTOLE32 (offset);
		request->MLENGTH = HTOLE32 (length);
		if (SendMME (plc) <= 0) 
		{
			error ((plc->flags & PLC_BAILOUT), errno, CHANNEL_CANTSEND);
			return (-1);
		}
		if (ReadMME (plc, 0, (VS_RD_BLK_NVM | MMTYPE_CNF)) <= 0) 
		{
			error ((plc->flags & PLC_BAILOUT), errno, CHANNEL_CANTREAD);
			return (-1);
		}
		if (confirm->MSTATUS) 
		{
			Failure (plc, PLC_WONTDOIT);
			return (-1);
		}
		length = LE16TOH (confirm->MLENGTH);
		offset = LE32TOH (confirm->MOFFSET);
		if (write (plc->nvm.file, confirm->BUFFER, length) != (signed)(length)) 
		{
			error ((plc->flags & PLC_BAILOUT), errno, FILE_CANTSAVE, plc->nvm.name);
			return (-1);
		}
		offset += length;
	}
	return (0);
}


#endif

