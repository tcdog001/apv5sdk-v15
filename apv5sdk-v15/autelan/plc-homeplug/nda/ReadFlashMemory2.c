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

#ifndef READFLASHMEMORY2_SOURCE
#define READFLASHMEMORY2_SOURCE

#include <stdint.h>
#include <unistd.h>
#include <memory.h>

#include "../tools/error.h"
#include "../tools/files.h"
#include "../tools/endian.h"
#include "../tools/memory.h"
#include "../tools/symbol.h"
#include "../ram/nvram.h"
#include "../plc/plc.h"
#include "../nda/nda.h"

/*====================================================================*
 *   
 *   signed ReadFlash (struct plc * plc, struct _file_ * file, uint32_t offset, uint32_t extent);
 *   
 *   plc.h
 *   
 *   Read raw flash memory;
 *
 *.  Qualcomm Atheros HomePlug AV Powerline Toolkit
 *:  Published 2009-2012 by Qualcomm Atheros. ALL RIGHTS RESERVED
 *;  For demonstration and evaluation only. Not for production use
 *
 *   Contributor(s):
 *      Charles Maier <cmaier@qualcomm.com>
 *
 *--------------------------------------------------------------------*/

static signed ReadFlash (struct plc * plc, struct _file_ * file, uint32_t offset, uint32_t extent) 

{
	struct channel * channel = (struct channel *)(plc->channel);
	struct message * message = (struct message *)(plc->message);

#ifndef __GNUC__
#pragma pack (push,1)
#endif

	struct __packed vs_module_operation_read_request 
	{
		struct ethernet_std ethernet;
		struct qualcomm_std qualcomm;
		uint32_t RESERVED;
		uint8_t NUM_OP_DATA;
		struct __packed 
		{
			uint16_t MOD_OP;
			uint16_t MOD_OP_DATA_LEN;
			uint32_t MOD_OP_RSVD;
			uint16_t MODULE_ID;
			uint16_t MODULE_SUB_ID;
			uint16_t MODULE_LENGTH;
			uint32_t MODULE_OFFSET;
		}
		MODULE_SPEC;
	}
	* request = (struct vs_module_operation_read_request *)(message);
	struct __packed vs_module_operation_read_confirm 
	{
		struct ethernet_std ethernet;
		struct qualcomm_std qualcomm;
		uint16_t MSTATUS;
		uint16_t ERR_REC_CODE;
		uint32_t RESERVED;
		uint8_t NUM_OP_DATA;
		struct __packed 
		{
			uint16_t MOD_OP;
			uint16_t MOD_OP_DATA_LEN;
			uint32_t MOD_OP_RSVD;
			uint16_t MODULE_ID;
			uint16_t MODULE_SUB_ID;
			uint16_t MODULE_LENGTH;
			uint32_t MODULE_OFFSET;
		}
		MODULE_SPEC;
		uint8_t MODULE_DATA [PLC_MODULE_SIZE];
	}
	* confirm = (struct vs_module_operation_read_confirm *)(message);

#ifndef __GNUC__
#pragma pack (pop)
#endif

	unsigned length = PLC_MODULE_SIZE;
	unsigned timeout = channel->timeout;
	Request (plc, "Reading Flash Memory");
	while (extent) 
	{
		memset (message, 0, sizeof (* message));
		EthernetHeader (&request->ethernet, channel->peer, channel->host, HOMEPLUG_MTYPE);
		QualcommHeader (&request->qualcomm, 0, (VS_MODULE_OPERATION | MMTYPE_REQ));
		plc->packetsize = (ETHER_MIN_LEN - ETHER_CRC_LEN);
		if (length > extent) 
		{
			length = extent;
		}
		request->NUM_OP_DATA = 1;
		request->MODULE_SPEC.MOD_OP = HTOLE16 (PLC_MOD_OP_READ_FLASH);
		request->MODULE_SPEC.MOD_OP_DATA_LEN = HTOLE16 (sizeof (request->MODULE_SPEC));
		request->MODULE_SPEC.MOD_OP_RSVD = HTOLE32 (0);
		request->MODULE_SPEC.MODULE_ID = HTOLE16 (PLC_MODULEID_RESERVED1);
		request->MODULE_SPEC.MODULE_SUB_ID = HTOLE16 (0);
		request->MODULE_SPEC.MODULE_LENGTH = HTOLE16 (length);
		request->MODULE_SPEC.MODULE_OFFSET = HTOLE32 (offset);

#if 0
#if defined (__GNUC__)
#warning "Debug code active in module ModuleRead"
#endif

		fprintf (stderr, "----- \n");
		fprintf (stderr, "RESERVED 0x%08X\n", LE32TOH (request->RESERVED));
		fprintf (stderr, "NUM_OP_DATA %d\n", request->NUM_OP_DATA);
		fprintf (stderr, "MOD_OP 0x%02X\n", LE16TOH (request->MODULE_SPEC.MOD_OP));
		fprintf (stderr, "MOD_OP_DATA_LEN %d\n", LE16TOH (request->MODULE_SPEC.MOD_OP_DATA_LEN));
		fprintf (stderr, "RESERVED 0x%08X\n", LE32TOH (request->MODULE_SPEC.MOD_OP_RSVD));
		fprintf (stderr, "MODULE_ID 0x%04X\n", LE16TOH (request->MODULE_SPEC.MODULE_ID));
		fprintf (stderr, "MODULE_SUB_ID 0x%04X\n", LE16TOH (request->MODULE_SPEC.MODULE_SUB_ID));
		fprintf (stderr, "MODULE_LENGTH %d\n", LE16TOH (request->MODULE_SPEC.MODULE_LENGTH));
		fprintf (stderr, "MODULE_OFFSET 0x%08X\n", LE32TOH (request->MODULE_SPEC.MODULE_OFFSET));
		fprintf (stderr, "\n");

#endif

		if (SendMME (plc) <= 0) 
		{
			error ((plc->flags & PLC_BAILOUT), errno, CHANNEL_CANTSEND);
			return (-1);
		}
		channel->timeout = PLC_MODULE_READ_TIMEOUT;
		if (ReadMME (plc, 0, (VS_MODULE_OPERATION | MMTYPE_CNF)) <= 0) 
		{
			error ((plc->flags & PLC_BAILOUT), errno, CHANNEL_CANTREAD);
			return (-1);
		}
		channel->timeout = timeout;

#if 0
#if defined (__GNUC__)
#warning "Debug code active in module ModuleRead"
#endif

		fprintf (stderr, "MSTATUS 0x%04X\n", LE16TOH (confirm->MSTATUS));
		fprintf (stderr, "ERROR_REC_CODE %d\n", LE16TOH (confirm->ERR_REC_CODE));
		fprintf (stderr, "RESERVED 0x%08X\n", LE32TOH (confirm->RESERVED));
		fprintf (stderr, "NUM_OP_DATA %d\n", confirm->NUM_OP_DATA);
		fprintf (stderr, "MOD_OP 0x%02X\n", LE16TOH (confirm->MODULE_SPEC.MOD_OP));
		fprintf (stderr, "MOD_OP_DATA_LEN %d\n", LE16TOH (confirm->MODULE_SPEC.MOD_OP_DATA_LEN));
		fprintf (stderr, "RESERVED 0x%08X\n", LE32TOH (confirm->MODULE_SPEC.MOD_OP_RSVD));
		fprintf (stderr, "MODULE_ID 0x%04X\n", LE16TOH (confirm->MODULE_SPEC.MODULE_ID));
		fprintf (stderr, "MODULE_SUB_ID 0x%04X\n", LE16TOH (confirm->MODULE_SPEC.MODULE_SUB_ID));
		fprintf (stderr, "MODULE_LENGTH %d\n", LE16TOH (confirm->MODULE_SPEC.MODULE_LENGTH));
		fprintf (stderr, "MODULE_OFFSET 0x%08X\n", LE32TOH (confirm->MODULE_SPEC.MODULE_OFFSET));
		fprintf (stderr, "\n");

#endif

		if (LE16TOH (confirm->MSTATUS)) 
		{
			Failure (plc, PLC_WONTDOIT);
			return (-1);
		}
		length = LE16TOH (confirm->MODULE_SPEC.MODULE_LENGTH);
		offset = LE32TOH (confirm->MODULE_SPEC.MODULE_OFFSET);
		if (write (file->file, confirm->MODULE_DATA, length) != (signed)(length)) 
		{
			error ((plc->flags & PLC_BAILOUT), errno, FILE_CANTSAVE, file->name);
			return (-1);
		}
		offset += length;
		extent -= length;
	}
	return (0);
}


/*====================================================================*
 *  
 *   signed ReadFlashMemory2 (struct plc * plc);
 *
 *   plc.h
 *  
 *   determine the overall size of flash memory with VS_GET_NVM then
 *   erase flash memory by writing 0xFF to all of flash memory using 
 *   VS_MODULE_OPERATION messages; force flash but do not reset;
 *
 *   struct vs_module_spec is defined in plc.h;
 *
 *.  Qualcomm Atheros HomePlug AV Powerline Toolkit
 *:  Published 2009-2012 by Qualcomm Atheros. ALL RIGHTS RESERVED
 *;  For demonstration and evaluation only. Not for production use
 *
 *   Contributor(s):
 *      Charles Maier <cmaier@qualcomm.com>
 *
 *--------------------------------------------------------------------*/

signed ReadFlashMemory2 (struct plc * plc) 

{
	struct channel * channel = (struct channel *)(plc->channel);
	struct message * message = (struct message *)(plc->message);

#ifndef __GNUC__
#pragma pack (push,1)
#endif

	struct __packed vs_get_nvm_request 
	{
		struct ethernet_std ethernet;
		struct qualcomm_std qualcomm;
	}
	* request = (struct vs_get_nvm_request *) (message);
	struct __packed vs_get_nvm_confirm 
	{
		struct ethernet_std ethernet;
		struct qualcomm_std qualcomm;
		uint8_t MSTATUS;
		struct config_nvram config_nvram;
	}
	* confirm = (struct vs_get_nvm_confirm *) (message);

#ifndef __GNUC__
#pragma pack (pop)
#endif

	memcpy (channel->peer, message->ethernet.OSA, sizeof (channel->peer));
	Request (plc, "Probing Flash Memory");
	memset (message, 0, sizeof (* message));
	EthernetHeader (&request->ethernet, channel->peer, channel->host, HOMEPLUG_MTYPE);
	QualcommHeader (&request->qualcomm, 0, (VS_GET_NVM | MMTYPE_REQ));
	plc->packetsize = (ETHER_MIN_LEN - ETHER_CRC_LEN);
	if (SendMME (plc) <= 0) 
	{
		error ((plc->flags & PLC_BAILOUT), errno, CHANNEL_CANTSEND);
		return (-1);
	}
	if (ReadMME (plc, 0, (VS_GET_NVM | MMTYPE_CNF)) <= 0) 
	{
		error ((plc->flags & PLC_BAILOUT), errno, CHANNEL_CANTSEND);
		return (-1);
	}
	if (confirm->MSTATUS) 
	{
		Failure (plc, PLC_WONTDOIT);
		return (-1);
	}
	if (ReadFlash (plc, &plc->nvm, 0, LE32TOH (confirm->config_nvram.NVRAMSIZE))) 
	{
		return (-1);
	}
	return (0);
}


#endif

