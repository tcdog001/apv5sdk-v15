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
 *   signed ReadPIB (struct int6k *int6k); 
 *
 *   int6k.h
 *
 *   Read the PIB Image from an INT6000 using as many VS_RD_MOD
 *   messages as needed; Write image blocks to file as they are 
 *   read;
 *
 *   the objective here is to read the PIB module in 1024 byte blocks 
 *   until the module offset equals or exceeds the PIB length; we do
 *   not know the PIB length until we have read the header contained
 *   in the first block read from OFFSET 0;
 *
 *   MOFFSET and MLENGTH fields occupy different offsets in REQ and CNF 
 *   messages; we exploit that by initializing them using CNF message
 *   offsets then copying values into REQ message offsets before each
 *   read; this works because confirmation messages always return all 
 *   the data we request or nothing at all;
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
 *      Lluis Paulet Gimbert <dimat.com>
 *
 *--------------------------------------------------------------------*/

#ifndef READPIB_SOURCE
#define READPIB_SOURCE

#include <stdint.h>
#include <unistd.h>
#include <memory.h>

#include "../int6k/int6k.h" 
#include "../tools/error.h"
#include "../tools/files.h"
#include "../pib/pib.h" 

signed ReadPIB (struct int6k *int6k) 

{

	struct channel * channel = (struct channel *)(int6k->channel);
	struct message * message = (struct message *)(int6k->message);
#ifndef __GNUC__
#pragma pack (push,1)
#endif

	struct __packed vs_rd_mod_request 
	{
		struct header_eth ethernet;
		struct header_int intellon;
		uint8_t MODULEID;
		uint8_t MACCESS;
		uint16_t MLENGTH;
		uint32_t MOFFSET;
		uint8_t MSECRET [16];
	}
	* request = (struct vs_rd_mod_request *) (message);
	struct __packed vs_rd_mod_confirm 
	{
		struct header_eth ethernet;
		struct header_int intellon;
		uint8_t MSTATUS;
		uint8_t RESERVED1 [3];
		uint8_t MODULEID;
		uint8_t RESERVED;
		uint16_t MLENGTH;
		uint32_t MOFFSET;
		uint32_t CHKSUM;
		uint8_t BUFFER [INT6K_BLOCKSIZE];
	}
	* confirm = (struct vs_rd_mod_confirm *) (message);

#ifndef __GNUC__
#pragma pack (pop)
#endif

	uint32_t extent = 0;
	uint32_t offset = 0;
	signed length = INT6K_BLOCKSIZE;
	Request (int6k, "Read Parameters from Device");
	if (lseek (int6k->pib.file, 0, SEEK_SET)) 
	{
		error ((int6k->flags & INT6K_BAILOUT), errno, "Can't rewind %s", filepart (int6k->pib.name));
		return (1);
	}
	memset (message, 0, sizeof (struct message));
	do 
	{
		EthernetHeader (&message->ethernet, channel->peer, channel->host);
		IntellonHeader (&message->intellon, (VS_RD_MOD | MMTYPE_REQ));
		int6k->packetsize = ETHER_MIN_LEN;
		request->MODULEID = VS_MODULE_PIB;
		request->MLENGTH = HTOLE16 (length);
		request->MOFFSET = HTOLE32 (offset);
		if (SendMME (int6k) <= 0) 
		{
			error ((int6k->flags & INT6K_BAILOUT), ECANCELED, INT6K_CANTSEND);
			return (-1);
		}
		if (ReadMME (int6k, (VS_RD_MOD | MMTYPE_CNF)) <= 0) 
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

		if (LE16TOH (confirm->MLENGTH) != length) 
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

		if (LE16TOH (confirm->MLENGTH) != length) 
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

		length = LE16TOH (confirm->MLENGTH);
		offset = LE32TOH (confirm->MOFFSET);
		if (checksum_32 (confirm->BUFFER, length, confirm->CHKSUM)) 
		{
			error ((int6k->flags & INT6K_BAILOUT), ECANCELED, "Bad Packet Checksum");
			return (-1);
		}
		if (offset == extent) 
		{
			struct header_pib * header_pib = (struct header_pib *) (confirm->BUFFER);
			extent = header_pib->PIBLENGTH;
		}
		if ((offset + length) > extent) 
		{
			length = extent - offset;
		}
		if (lseek (int6k->pib.file, offset, SEEK_SET) != offset) 
		{
			error ((int6k->flags & INT6K_BAILOUT), errno, "can't seek %s", filepart (int6k->pib.name));
			return (-1);
		}
		if (write (int6k->pib.file, confirm->BUFFER, length) < length) 
		{
			error ((int6k->flags & INT6K_BAILOUT), errno, "can't save %s", filepart (int6k->pib.name));
			return (-1);
		}
		offset += length;
	}
	while (offset < extent);
	Confirm (int6k, "Read %s", int6k->pib.name);
	return (0);
}

#endif

