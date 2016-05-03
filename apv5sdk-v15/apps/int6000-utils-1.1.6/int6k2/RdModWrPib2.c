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
 *   signed RdModWrPIB2 (struct int6k *int6k); 
 *
 *   int6k2.h
 *
 *   Read PIB from a device using ihpapi_UpdateDevice
 *   api function call, and as many ihpapi_TxFrame and ihpapi_RxFrame
 *   as needed. Modify PIB with values from xml file, and writes back
 *   the modified PIB to the device.;
 *
 *   See the Intellon HomePlugAV Firmware Technical Reference Manual
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
 *  Contributor(s):
 *      Alex Vasquez <alex.vasquez@intellon.com>
 *
 *--------------------------------------------------------------------*/

#ifndef RDMODWRPIB2_SOURCE
#define RDMODWRPIB2_SOURCE

#include <stdint.h>
#include <stdlib.h>
#include <memory.h>
#include <errno.h>

#include "../ihpapi/ihp.h"
#include "../ihpapi/ihpapi.h"
#include "../int6k/int6k.h" 
#include "../tools/memory.h"
#include "../tools/error.h"
#include "../nodes/nodes.h"
#include "../pib/pib.h" 

static bool firstBuffer = true;
signed RdModWrPIB2 (struct int6k *int6k) 

{
	uint16_t piblen;
	uint8_t * pibbuffer = 0;
	size_t offset;
	header_pib * pibheader;
	struct node * markup;
	Request (int6k, "Read-Modify-Write PIB");
	Request (int6k, "...Reading");
	if (ihpapi_UpdateDevice (int6k->channel->host, int6k->channel->peer, IHPAPI_HAR_UPLOAD_PB) == -1) 
	{
		error ((int6k->flags & INT6K_BAILOUT), errno, "initialization error");
		return (-1);
	}
	offset = 0;
	do 
	{
		int6k->packetsize = (int)(ihpapi_TxFrame (sizeof (struct message), (uint8_t *)(int6k->message)));
		if (int6k->packetsize == 0) 
		{
			error ((int6k->flags & INT6K_BAILOUT), ECANCELED, "no more data");
			break;
		}
		if (SendMME (int6k) <= 0) 
		{
			error ((int6k->flags & INT6K_BAILOUT), ECANCELED, INT6K_CANTSEND);
			if (pibbuffer) 
			{
				free (pibbuffer);
			}
			return (-1);
		}
		if (ReadMME2 (int6k) == -1) 
		{
			error ((int6k->flags & INT6K_BAILOUT), ECANCELED, INT6K_CANTREAD);
			if (pibbuffer) 
			{
				free (pibbuffer);
			}
			return (-1);
		}
		if (int6k->result->opStatus.status) 
		{
			Failure (int6k, INT6K_WONTDOIT);
			if (pibbuffer) 
			{
				free (pibbuffer);
			}
			return (-1);
		}
		if (firstBuffer == true) 
		{
			piblen = ((header_pib *)int6k->result->data.updateDev.buffer)->PIBLENGTH;
			if ((pibbuffer = (uint8_t *)malloc (piblen)) == 0) 
			{
				error ((int6k->flags & INT6K_BAILOUT), ECANCELED, "can't allocate buffer\n");
				return (-1);
			}
			pibheader = (header_pib *)pibbuffer;
			firstBuffer = false;
		}
		memcpy (&pibbuffer [offset], int6k->result->data.updateDev.buffer, int6k->result->data.updateDev.bufferLen);
		offset += int6k->result->data.updateDev.bufferLen;
	}
	while (int6k->result->data.updateDev.eof == false || piblen > offset);
	Request (int6k, "...Checking");
	if (checksum32 ((uint32_t *)pibbuffer, (size_t)(piblen >> 2), 0)) 
	{
		error ((int6k->flags & INT6K_BAILOUT), ECANCELED, "bad PIB checksum");
		free (pibbuffer);
		return (-1);
	}
	Request (int6k, "...Modifying");
	markup = xmlopen (int6k->XML.name);
	xmledit (markup, pibbuffer, piblen);
	xmlfree (markup);
	Request (int6k, "...Writing");

#if INT6K_SAFEMODE

/*
 *      prevent attempts to write one MAC or PIB image file to multiple devices 
 *  when the caller specifies the LMA as the ODA; the operation makes
 *      no sense;
 */

	if (!memcmp (int6k->LMA, int6k->header.ethernet.ODA, sizeof (int6k->LMA))) 
	{
		error ((int6k->flags & INT6K_BAILOUT), ECANCELED, "can't multi-write PIB");
		free (pibbuffer);
		return (-1);
	}

#endif

	pibheader->CHECKSUM = 0;
	pibheader->CHECKSUM = checksum32 ((uint32_t *)pibbuffer, (size_t)(piblen >> 2), 0);
	memset (int6k->message, 0, sizeof (struct message));
	if (ihpapi_InitializeDevice (int6k->channel->host, int6k->channel->peer, 0, 0, piblen, pibbuffer, int6k->options) == -1) 
	{
		error ((int6k->flags & INT6K_BAILOUT), errno, "PIB initialization error");
		free (pibbuffer);
		return (-1);
	}
	do 
	{

/*
 *      format request message;
 */

		int6k->packetsize = (int)(ihpapi_TxFrame (sizeof (struct message), (uint8_t *)(int6k->message)));
		if (int6k->packetsize != 0) 
		{
			if (SendMME (int6k) <= 0) 
			{
				error ((int6k->flags & INT6K_BAILOUT), ECANCELED, INT6K_CANTSEND);
				free (pibbuffer);
				return (-1);
			}
		}
		if (ReadMME2 (int6k) == -1) 
		{
			error ((int6k->flags & INT6K_BAILOUT), ECANCELED, INT6K_CANTREAD);
			free (pibbuffer);
			return (-1);
		}
		if (int6k->result->opStatus.status) 
		{
			Failure (int6k, INT6K_WONTDOIT);
			free (pibbuffer);
			return (-1);
		}
	}
	while (int6k->result->opCompltCode != IHPAPI_OPCMPLTCODE_COMPLETE);
	Confirm (int6k, "Done!");
	free (pibbuffer);
	return (0);
}

#endif

