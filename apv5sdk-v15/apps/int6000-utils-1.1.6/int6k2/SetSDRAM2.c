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
 *   signed SetSDRAM2 (struct int6k * int6k);
 *   
 *   int6k2.h
 *
 *   Use ihpapi_SetSDRAMConfig api function call and print all responses;
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
 *      Charles Maier <charles.maier@intellon.com>
 *      Alex Vasquez <alex.vasquez@intellon.com>
 *
 *--------------------------------------------------------------------*/

#ifndef SETSDRAM2_SOURCE
#define SETSDRAM2_SOURCE

#include <unistd.h>
#include <stdlib.h>
#include <memory.h>

#include "../ihpapi/ihp.h"
#include "../ihpapi/ihpapi.h"
#include "../int6k/int6k.h"
#include "../tools/memory.h"
#include "../tools/error.h"

signed SetSDRAM2 (struct int6k * int6k) 

{
	uint32_t sdlen;
	uint8_t * sdbuffer;
	if (_anyset (int6k->flags, INT6K_HOST_ACTION)) 
	{
		int6k->hostaction = IHPAPI_HAR_INITIALIZE_DEVICE_SDRAMCFG;
		HostActionWait2 (int6k);
		int6k->hostaction = (uint8_t)(-1);
	}
	Request (int6k, "Set SDRAM Configuration");
	sdlen = lseek (int6k->CFG.file, 0, SEEK_END);
	if (sdlen == 0) 
	{
		error ((int6k->flags & INT6K_BAILOUT), errno, "%s failed to calculate SDRAM file length", int6k->CFG.name);
		return (-1);
	}
	sdbuffer = (uint8_t *)malloc (sdlen + sdlen/2);
	if (sdbuffer == 0) 
	{
		error ((int6k->flags & INT6K_BAILOUT), ECANCELED, "failed get SDRAM memory buffers");
		return (-1);
	}
	lseek (int6k->CFG.file, 0, SEEK_SET);
	if (read (int6k->CFG.file, sdbuffer, sdlen) != sdlen) 
	{
		error ((int6k->flags & INT6K_BAILOUT), ECANCELED, "%s failed to read SDRAM file", int6k->CFG.name);
		free (sdbuffer);
		return (-1);
	}
	memset (int6k->message, 0, sizeof (struct message));
	if (sdlen <= 64) 
	{
		if (!hexencode (&sdbuffer [sdlen], sdlen/2, (char *)sdbuffer)) 
		{
			error ((int6k->flags & INT6K_BAILOUT), ECANCELED, "%s failed to hexencode SDRAM file", int6k->CFG.name);
			free (sdbuffer);
			return (-1);
		}
		int6k->packetsize = (int)(ihpapi_SetSDRAMConfig (int6k->channel->host, int6k->channel->peer, sizeof (struct message), (uint8_t *)(int6k->message), sdlen/2, &sdbuffer [sdlen]));
		if (int6k->packetsize == 0) 
		{
			error ((int6k->flags & INT6K_BAILOUT), errno, "wrong packet size");
			free (sdbuffer);
			return (-1);
		}
		if (SendMME (int6k) <= 0) 
		{
			free (sdbuffer);
			error ((int6k->flags & INT6K_BAILOUT), errno, INT6K_CANTSEND);
			return (-1);
		}
		if (ReadMME2 (int6k) <= 0) 
		{
			free (sdbuffer);
			error ((int6k->flags & INT6K_BAILOUT), errno, INT6K_CANTREAD);
			return (-1);
		}
		if (int6k->result->opStatus.status) 
		{
			free (sdbuffer);
			Failure (int6k, INT6K_WONTDOIT);
			return (-1);
		}
	}
	else 
	{
		int6k->packetsize = (int)(ihpapi_SetSDRAMConfig (int6k->channel->host, int6k->channel->peer, sizeof (struct message), (uint8_t *)(int6k->message), sdlen, sdbuffer));
		do 
		{

/*
 *      format request message;
 */

			if (int6k->packetsize != 0) 
			{
				if (SendMME (int6k) <= 0) 
				{
					error ((int6k->flags & INT6K_BAILOUT), ECANCELED, INT6K_CANTSEND);
					free (sdbuffer);
					return (-1);
				}
			}
			if (ReadMME2 (int6k) == -1) 
			{
				error ((int6k->flags & INT6K_BAILOUT), ECANCELED, INT6K_CANTREAD);
				free (sdbuffer);
				return (-1);
			}
			if (int6k->result->opStatus.status) 
			{
				Failure (int6k, INT6K_WONTDOIT);
				free (sdbuffer);
				return (-1);
			}
			int6k->packetsize = (int)(ihpapi_TxFrame (sizeof (struct message), (uint8_t *)(int6k->message)));
		}
		while (int6k->result->opCompltCode != IHPAPI_OPCMPLTCODE_COMPLETE);
	}
	Confirm (int6k, "Written.");
	free (sdbuffer);
	return (0);
}

#endif

