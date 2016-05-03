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
 *   signed EraseFlashSector (struct plc * plc);
 *
 *   nda.h
 *
 *   erase a specific sector of flash memory when the DAK is known;
 *   the sector is passed in plc->modulecode as an 8 bit value so
 *   make sure that plc->modulecode is not being used by another
 *   function such as function Flash () in module FlashPTS; 
 *
 *   See the Atheros HomePlug AV Firmware Technical Reference Manual 
 *   for more information;
 *   
 *.  Qualcomm Atheros HomePlug AV Powerline Toolkit
 *:  Published 2009-2011 by Qualcomm Atheros. ALL RIGHTS RESERVED
 *;  For demonstration and evaluation only. Not for production use
 *
 *   Contributor(s):
 *      Charles Maier <cmaier@qualcomm.com>
 *
 *--------------------------------------------------------------------*/

#ifndef ERASEFLASHSECTOR_SOURCE
#define ERASEFLASHSECTOR_SOURCE

#include <stdint.h>
#include <memory.h>

#include "../tools/memory.h"
#include "../tools/error.h"
#include "../plc/plc.h"
#include "../nda/nda.h"

signed EraseFlashSector (struct plc * plc) 

{
	struct channel * channel = (struct channel *)(plc->channel);
	struct message * message = (struct message *)(plc->message);

#ifndef __GNUC__
#pragma pack (push,1)
#endif

	struct __packed vs_flash_erase_request 
	{
		struct ethernet_std ethernet;
		struct qualcomm_std qualcomm;
		uint8_t SECTOR;
		uint8_t DAK [HPAVKEY_DAK_LEN];
	}
	* request = (struct vs_flash_erase_request *) (message);
	struct __packed vs_flash_erase_confirm 
	{
		struct ethernet_std ethernet;
		struct qualcomm_std qualcomm;
		uint8_t STATUS;
		uint8_t SECTOR;
	}
	* confirm = (struct vs_flash_erase_confirm *) (message);

#ifndef __GNUC__
#pragma pack (pop)
#endif

	Request (plc, "Erase Flash Memory Sector");
	memset (message, 0, sizeof (* message));
	EthernetHeader (&request->ethernet, channel->peer, channel->host, HOMEPLUG_MTYPE);
	QualcommHeader (&request->qualcomm, 0, (VS_FLASH_ERASE | MMTYPE_REQ));
	request->SECTOR = plc->sector;
	memcpy (request->DAK, plc->DAK, sizeof (request->DAK));
	plc->packetsize = (ETHER_MIN_LEN - ETHER_CRC_LEN);
	if (SendMME (plc) <= 0) 
	{
		error ((plc->flags & PLC_BAILOUT), ECANCELED, CHANNEL_CANTSEND);
		return (-1);
	}
	if (ReadMME (plc, 0, (VS_FLASH_ERASE | MMTYPE_CNF)) <= 0) 
	{
		error ((plc->flags & PLC_BAILOUT), ECANCELED, CHANNEL_CANTREAD);
		return (-1);
	}
	if (confirm->STATUS) 
	{
		Failure (plc, PLC_WONTDOIT);
		return (-1);
	}
	Confirm (plc, "Erased Sector %d", confirm->SECTOR);
	return (0);
}


#endif

