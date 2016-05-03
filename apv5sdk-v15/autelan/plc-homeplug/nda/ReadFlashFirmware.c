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
 *   signed ReadFlashFirmware (struct plc *plc);
 *   
 *   nda.h
 *   
 *.  Qualcomm Atheros HomePlug AV Powerline Toolkit
 *:  Published 2009-2011 by Qualcomm Atheros. ALL RIGHTS RESERVED
 *;  For demonstration and evaluation only. Not for production use
 *
 *   Contributor(s):
 *      Charles Maier <cmaier@qualcomm.com>
 *
 *--------------------------------------------------------------------*/

#ifndef READFLASHFIRMWARE_SOURCE
#define READFLASHFIRMWARE_SOURCE

#include <stdint.h>
#include <unistd.h>
#include <memory.h>

#include "../tools/error.h"
#include "../tools/files.h"
#include "../nvm/nvm.h"
#include "../plc/plc.h"
#include "../nda/nda.h"

signed ReadFlashFirmware (struct plc *plc) 

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

	uint32_t header = 0;
	uint32_t extent = 0;
	uint32_t offset = 0;
	uint32_t length = PLC_RECORD_SIZE;
	Request (plc, "Read Firmware from Flash");
	if (lseek (plc->nvm.file, 0, SEEK_SET)) 
	{
		error ((plc->flags & PLC_BAILOUT), errno, FILE_CANTHOME, plc->nvm.name);
		return (1);
	}
	do 
	{
		memset (message, 0, sizeof (* message));
		EthernetHeader (&request->ethernet, channel->peer, channel->host, HOMEPLUG_MTYPE);
		QualcommHeader (&request->qualcomm, 0, (VS_RD_BLK_NVM | MMTYPE_REQ));
		plc->packetsize = (ETHER_MIN_LEN - ETHER_CRC_LEN);
		request->MODULEID = HTOLE16 (MID_FIRMWARE);
		request->MLENGTH = HTOLE32 (length);
		request->MOFFSET = HTOLE32 (offset);
		if (SendMME (plc) <= 0) 
		{
			error ((plc->flags & PLC_BAILOUT), ECANCELED, CHANNEL_CANTSEND);
			return (-1);
		}
		if (ReadMME (plc, 0, (VS_RD_BLK_NVM | MMTYPE_CNF)) <= 0) 
		{
			error ((plc->flags & PLC_BAILOUT), ECANCELED, CHANNEL_CANTREAD);
			return (-1);
		}
		if (confirm->MSTATUS) 
		{
			Failure (plc, PLC_WONTDOIT);
			return (-1);
		}
		if (LE32TOH (confirm->MOFFSET) != offset) 
		{
			Failure (plc, PLC_ERR_OFFSET);
			return (-1);
		}
		if (LE32TOH (confirm->MLENGTH) != length) 
		{
			Failure (plc, PLC_ERR_LENGTH);
			return (-1);
		}
		offset = LE32TOH (confirm->MOFFSET);
		length = LE32TOH (confirm->MLENGTH);
		if (offset == extent) 
		{
			struct nvm_header1 * nvm_header = (struct nvm_header1 *)(confirm->BUFFER);
			if (checksum32 (nvm_header, sizeof (* nvm_header), 0)) 
			{
				error ((plc->flags & PLC_BAILOUT), ECANCELED, "Bad Header Checksum");
				return (-1);
			}
			if (LE32TOH (nvm_header->HEADERVERSION) != 0x60000000) 
			{
				error ((plc->flags & PLC_BAILOUT), ECANCELED, "Bad Header Version");
				return (-1);
			}
			extent += sizeof (* nvm_header);
			extent += LE32TOH (nvm_header->IMAGELENGTH);
			header = LE16TOH (nvm_header->NEXTHEADER);
		}
		if ((offset + length) > extent) 
		{
			length = extent - offset;
		}
		if (lseek (plc->nvm.file, offset, SEEK_SET) != (signed)(offset)) 
		{
			error ((plc->flags & PLC_BAILOUT), errno, "Can't seek %s", plc->nvm.name);
			return (-1);
		}
		if (write (plc->nvm.file, confirm->BUFFER, length) < (signed)(length)) 
		{
			error ((plc->flags & PLC_BAILOUT), errno, "Can't save %s", plc->nvm.name);
			return (-1);
		}
		offset += length;
		length = 1024;
	}
	while ((header) || (offset < extent));
	return (0);
}


#endif

