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
 *   signed Upgrade2 (struct int6k *int6k);
 *   
 *   int6k2.h
 *
 *   Write MAC Firmware and PIB images to device using 
 *   ihpapi_InitializeDevice, and as many ihpapi_TxFrame and 
 *   ihpapi_RxFrames as needed.
 *   
 *   read the int6k->NVM and int6k->PIB files into buffers;
 *
 *   See the HomePlugAV Firmware Technical Reference Manual for more 
 *   information;
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
 *      Alex Vasquez <alex.vasquez@intellon.com>
 *      Charles Maier <charles.maier@intellon.com>
 *
 *--------------------------------------------------------------------*/

#ifndef UPGRADE2_SOURCE
#define UPGRADE2_SOURCE

#include <unistd.h>
#include <stdlib.h>
#include <memory.h>
#include <errno.h>

#include "../ihpapi/ihpapi.h"
#include "../int6k/int6k.h" 
#include "../tools/flags.h"
#include "../tools/files.h"
#include "../tools/error.h"
#include "../nvm/nvm.h" 
#include "../pib/pib.h"

signed Upgrade2 (struct int6k *int6k) 

{
	uint32_t fwlen;
	uint32_t piblen;
	uint8_t *fwbuffer;
	uint8_t *pibbuffer;
	if (_anyset (int6k->flags, INT6K_HOST_ACTION)) 
	{
		int6k->hostaction = IHPAPI_HAR_INITIALIZE_DEVICE;
		HostActionWait2 (int6k);
		int6k->hostaction = (uint8_t)(-1);
	}

/*
 *      get and validate the MAC and PIB lengths;
 */

	fwlen = lseek (int6k->NVM.file, 0, SEEK_END);
	if (fwlen <= 0) 
	{
		error ((int6k->flags & INT6K_BAILOUT), errno, "%s failed to calculate MAC file length", int6k->NVM.name);
		return (-1);
	}
	piblen = lseek (int6k->PIB.file, 0, SEEK_END);
	if (piblen <= 0) 
	{
		error ((int6k->flags & INT6K_BAILOUT), errno, "%s failed to calculate PIB file length", int6k->NVM.name);
		return (-1);
	}
	Request (int6k, "Writing MAC and PIB to Device");

/*
 *      validate then rewind NVM file before starting;
 */

	if (lseek (int6k->NVM.file, 0, SEEK_SET)) 
	{
		error ((int6k->flags & INT6K_BAILOUT), errno, "can't rewind %s", int6k->NVM.name);
		return (-1);
	}
	fwbuffer = (uint8_t *)malloc (fwlen);
	if (fwbuffer == 0) 
	{
		error ((int6k->flags & INT6K_BAILOUT), ECANCELED, "failed get MAC memory buffers");
		return (-1);
	}
	if (read (int6k->NVM.file, fwbuffer, fwlen) != fwlen) 
	{
		error ((int6k->flags & INT6K_BAILOUT), ECANCELED, "%s failed to read MAC file", int6k->NVM.name);
		free (fwbuffer);
		return (-1);
	}

/*
 *      validate then rewind PIB file before starting;
 */

	if (lseek (int6k->PIB.file, 0, SEEK_SET)) 
	{
		error ((int6k->flags & INT6K_BAILOUT), errno, "can't rewind %s", int6k->PIB.name);
		return (-1);
	}
	pibbuffer = (uint8_t *)malloc (piblen);
	if (pibbuffer == 0) 
	{
		error ((int6k->flags & INT6K_BAILOUT), ECANCELED, "failed get PIB memory buffers");
		free (fwbuffer);
		return (-1);
	}
	if (read (int6k->PIB.file, pibbuffer, piblen) != piblen) 
	{
		error ((int6k->flags & INT6K_BAILOUT), ECANCELED, "%s failed to read PIB file", int6k->PIB.name);
		free (fwbuffer);
		free (pibbuffer);
		return (-1);
	}

#if INT6K_SAFEMODE

/*
 *      prevent attempts to write one MAC or PIB image file to multiple devices 
 *  when the caller specifies the LMA as the ODA; the operation makes
 *      no sense;
 */

	if (!memcmp (int6k->LMA, int6k->header.ethernet.ODA, sizeof (int6k->LMA))) 
	{
		error ((int6k->flags & INT6K_BAILOUT), ECANCELED, "can't multi-write MAC images");
		free (fwbuffer);
		free (pibbuffer);
		return (-1);
	}

#endif

	memset (int6k->message, 0, sizeof (struct message));
	if (ihpapi_InitializeDevice (int6k->channel->host, int6k->channel->peer, fwlen, fwbuffer, piblen, pibbuffer, int6k->options) == -1) 
	{
		error ((int6k->flags & INT6K_BAILOUT), errno, "initialization error");
		free (fwbuffer);
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
				free (fwbuffer);
				free (pibbuffer);
				return (-1);
			}
		}
		if (ReadMME2 (int6k) == -1) 
		{
			error ((int6k->flags & INT6K_BAILOUT), ECANCELED, INT6K_CANTREAD);
			free (fwbuffer);
			free (pibbuffer);
			return (-1);
		}
		if (int6k->result->opStatus.status) 
		{
			Failure (int6k, INT6K_WONTDOIT);
			free (fwbuffer);
			free (pibbuffer);
			return (-1);
		}
	}
	while (int6k->result->opCompltCode != IHPAPI_OPCMPLTCODE_COMPLETE);
	Confirm (int6k, "Done.");
	free (fwbuffer);
	free (pibbuffer);
	return (0);
}

/*====================================================================*
 *
 *--------------------------------------------------------------------*/

#endif

