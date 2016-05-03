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
 *   int ihpapi_RxFrame (size_t length, uint8_t buffer [], ihpapi_result_t result);
 *
 *   ihpapi.h
 *
 *   This function decodes recived frames and provides the results to the application.
 *   The information provided in the results structure allows the application to
 *   futher process data or allows the API to continue sequencing an operation in
 *   progress.
 *
 *   EFAULT if buffer/result address or size are invalid or illegal;
 *   message version or vendor identifier is wrong and so decoding cannot 
 *   be trusted;
 *
 *   EPERM means the frame is HomePlug protocol and is not
 *   supported;
 *
 *   ENOSYS means unrecognized MME function or type;
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
 *.  Intellon HomePlug AV Application Programming Interface;
 *:  Published 2007 by Intellon Corp. ALL RIGHTS RESERVED;
 *;  For demonstration and evaluation only; Not for production use;
 *
 *   Contributor(s): 
 *	Alex Vasquez, alex.vasquez@intellon.com
 *	Charles Maier, charles.maier@intellon.com
 *
 *--------------------------------------------------------------------*/

#ifndef FRAMERECEIVED_SOURCE
#define FRAMERECEIVED_SOURCE

#include <stdint.h>
#include <string.h>
#include <errno.h>

#include "../tools/memory.h"
#include "ihpapi.h"
#include "ihp.h"
 
char ihpapi_version [] = IHPAPI_VERSION;

int ihpapi_RxFrame (size_t length, uint8_t buffer [], ihpapi_result_t * result) 

{
	extern struct SeqCB scb;
	int rc = -1;
	uint8_t OUI [3] = 
	{
		0x00,
		0xB0,
		0x52 
	};
	TxInfo *tcb = &scb.tcb;
	BlockInfo block;
	struct header_cnf * header = (struct header_cnf *)(buffer);
	memset (result, 0, sizeof (* result));
	result->opCode = IHPAPI_OPCODE_NOOP;
	result->opCompltCode = IHPAPI_OPCMPLTCODE_NOOP;
	result->opStatus.type = XX_MMTYPE_BAD;
	result->opStatus.status = 0;
	result->validData = false;
	result->dataLen = 0;

#if INTELLON_SAFEMODE
 
	if (buffer == (uint8_t *)(0)) 
	{
		result->opStatus.status = errno = EFAULT;
		return (-1);
	}
	if (result == (ihpapi_result_t *)(0)) 
	{
		result->opStatus.status = errno = EFAULT;
		return (-1);
	}

#endif
 
	if (ntohs (header->ethernet.MTYPE) != HOMEPLUG_MTYPE) 
	{
		result->opStatus.status = errno = EPERM;
		return (-1);
	}
	if (header->intellon.MMV != INTELLON_MMV) 
	{
		result->opStatus.status = errno = EFAULT;
		return (-1);
	}
	if (memcmp (header->intellon.OUI, OUI, sizeof (OUI))) 
	{
		result->opStatus.status = errno = EFAULT;
		return (-1);
	}
	result->opStatus.type = intohs(header->intellon.MMTYPE);
	memcpy (result->ODA, header->ethernet.ODA, IHPAPI_ETHER_ADDR_LEN);
	memcpy (result->OSA, header->ethernet.OSA, IHPAPI_ETHER_ADDR_LEN);
	switch (result->opStatus.type) 
	{
	case VS_HST_ACTION | MMTYPE_IND:
		rc = ihp_DecodeHostAction (buffer, length, result);
		memset (tcb, 0, sizeof (TxInfo));
		tcb->txok = true;
		memcpy (tcb->ODA, result->OSA, IHPAPI_ETHER_ADDR_LEN);
		memcpy (tcb->OSA, result->ODA, IHPAPI_ETHER_ADDR_LEN);
		if (!rc) 
		{
			tcb->HARRSPBLK.STATUS = 0x00;
		}
		else 
		{
			tcb->HARRSPBLK.STATUS = 0x01;
		}
		tcb->MMTYPE = VS_HST_ACTION | MMTYPE_RSP;
		tcb->hdrlen = sizeof (HARRspBlock);
		result->opCode = IHPAPI_OPCODE_HOST_ACTION_REQUEST;
		result->opCompltCode = IHPAPI_OPCMPLTCODE_TRANSMIT;
		break;
	case VS_SW_VER | MMTYPE_CNF:
		rc = ihp_DecodeGetVersion (buffer, length, result);
		result->opCode = IHPAPI_OPCODE_GET_VERSION_INFO;
		result->opCompltCode = IHPAPI_OPCMPLTCODE_COMPLETE;
		break;
	case VS_RS_DEV | MMTYPE_CNF:
		rc = ihp_DecodeResetDevice (buffer, length, result);
		result->opCode = IHPAPI_OPCODE_RESET_DEVICE;
		result->opCompltCode = IHPAPI_OPCMPLTCODE_COMPLETE;
		memset (&scb, 0, sizeof (scb));
		break;
	case VS_FAC_DEFAULT | MMTYPE_CNF:
		rc = ihp_DecodeResetToFactoryDefaults (buffer, length, result);
		result->opCode = IHPAPI_OPCODE_RESET_TO_FACTORY_DEFAULTS;
		result->opCompltCode = IHPAPI_OPCMPLTCODE_COMPLETE;
		memset (&scb, 0, sizeof (scb));
		break;
	case VS_SET_KEY | MMTYPE_CNF:
		rc = ihp_DecodeSetKey (buffer, length, result);
		result->opCode = IHPAPI_OPCODE_SET_KEY;
		result->opCompltCode = IHPAPI_OPCMPLTCODE_COMPLETE;
		break;
	case VS_MFG_STRING | MMTYPE_CNF:
		rc = ihp_DecodeMfgString (buffer, length, result);
		result->opCode = IHPAPI_OPCODE_GET_MANUFACTURER_INFO;
		result->opCompltCode = IHPAPI_OPCMPLTCODE_COMPLETE;
		break;
	case VS_LNK_STATS | MMTYPE_CNF:
		rc = ihp_DecodeLinkStats (buffer, length, result);
		result->opCode = IHPAPI_OPCODE_GET_CONNECTION_INFO;
		result->opCompltCode = IHPAPI_OPCMPLTCODE_COMPLETE;
		break;
	case VS_NW_INFO | MMTYPE_CNF:
		rc = ihp_DecodeNetworkInfo (buffer, length, result);
		result->opCode = IHPAPI_OPCODE_GET_NETWORK_INFO;
		result->opCompltCode = IHPAPI_OPCMPLTCODE_COMPLETE;
		break;
	case VS_TONE_MAP_CHAR | MMTYPE_CNF:
		rc = ihp_DecodeGetToneMapInfo (buffer, length, result);
		result->opCode = IHPAPI_OPCODE_GET_TONE_MAP_INFO;
		result->opCompltCode = IHPAPI_OPCMPLTCODE_COMPLETE;
		break;
	case VS_RX_TONE_MAP_CHAR | MMTYPE_CNF:
		rc = ihp_DecodeGetRxToneMapInfo (buffer, length, result);
		result->opCode = IHPAPI_OPCODE_GET_RX_TONE_MAP_INFO;
		result->opCompltCode = IHPAPI_OPCMPLTCODE_COMPLETE;
		break;
	case VS_ENET_SETTINGS | MMTYPE_CNF:
		rc = ihp_DecodeEthernetPHYSettings (buffer, length, result);
		result->opCode = IHPAPI_OPCODE_MANAGE_ENET_PHY;
		result->opCompltCode = IHPAPI_OPCMPLTCODE_COMPLETE;
		break;
	case VS_SET_SDRAM | MMTYPE_CNF:
		rc = ihp_DecodeSetSDRAMCfg (buffer, length, result);
		result->opCode = IHPAPI_OPCODE_SET_SDRAM_CONFIG;
		result->opCompltCode = IHPAPI_OPCMPLTCODE_COMPLETE;
		break;
	case VS_WR_MEM | MMTYPE_CNF:
		rc = ihp_SequenceWriteMemory (buffer, length, result);
		break;
	case VS_WR_MOD | MMTYPE_CNF:
		rc = ihp_SequenceWriteModule (buffer, length, result);
		break;
	case VS_RD_MOD | MMTYPE_CNF:
		rc = ihp_SequenceReadModule (buffer, length, result);
		break;
	case VS_MOD_NVM | MMTYPE_CNF:
		rc = ihp_DecodeFlashNVM (buffer, length, result, &block);
		result->opCode = scb.opcode;
		result->opCompltCode = IHPAPI_OPCMPLTCODE_COMPLETE;
		memset (&scb, 0, sizeof (scb));
		break;
	case VS_ST_MAC | MMTYPE_CNF:
		rc = ihp_DecodeStartMAC (buffer, length, result, &block);
		result->opCode = scb.opcode;
		result->opCompltCode = IHPAPI_OPCMPLTCODE_COMPLETE;
		memset (&scb, 0, sizeof (scb));
		break;
	default:
		result->opStatus.status = errno = ENOSYS;
		result->opStatus.type = XX_MMTYPE_BAD;
	}
	return (rc);
}

#endif
 

