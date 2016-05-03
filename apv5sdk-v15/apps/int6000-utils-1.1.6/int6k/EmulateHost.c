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
 *   int EmulateHost (struct int6k * int6k);
 *  
 *   int6k.h
 *
 *   wait indefinitely for VS_HST_ACTION messages; service requests
 *   as they arrive; this function is for demonstration and testing
 *   only; it stops dead - like a bug! - on error;
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
 *
 *--------------------------------------------------------------------*/

#ifndef EMULATEHOST_SOURCE
#define EMULATEHOST_SOURCE

#include <unistd.h>
#include <memory.h>

#include "../int6k/int6k.h"
#include "../tools/error.h"
#include "../tools/flags.h"
#include "../tools/files.h"

#include "../nvm/nvm.h"
#include "../pib/pib.h"

int EmulateHost (struct int6k * int6k) 

{
	struct channel * channel = (struct channel *)(int6k->channel);
	struct message * message = (struct message *)(int6k->message);
	static const char * hostaction [] = 
	{
		"initialize device",
		"store firmware",
		"store parameters",
		"update host",
		"configure memory",
		"restore defaults",
		"unknown" 
	};

#ifndef __GNUC__
#pragma pack (push,1)
#endif

	struct __packed vs_host_action_ind 
	{
		struct header_eth ethernet;
		struct header_int intellon;
		uint8_t MACTION;
	}
	* indicate = (struct vs_host_action_ind *) (message);

#ifndef __GNUC__
#pragma pack (pop)
#endif

	struct header_nvm header_nvm;
	struct header_pib header_pib;
	uint32_t offset = INT6K_PIBOFFSET;
	const char * PIB = int6k->PIB.name;
	const char * NVM = int6k->NVM.name;
	unsigned timeout = channel->timeout;
	channel->timeout = -1;
	Request (int6k, "Waiting for Host Action");
	while (ReadMME (int6k, (VS_HST_ACTION | MMTYPE_IND)) > 0) 
	{
		Confirm (int6k, "Host Action Request is (%d) %s.", indicate->MACTION, hostaction [indicate->MACTION]);
		memcpy (&channel->peer, indicate->ethernet.OSA, sizeof (channel->peer));
		channel->timeout = timeout;
		if (indicate->MACTION == 0x00) 
		{
			if (HostActionResponse (int6k)) 
			{
				return (-1);
			}
			if (lseek (int6k->NVM.file, 0, SEEK_SET)) 
			{
				error (1, errno, "Can't homed %s", filepart (int6k->NVM.name));
			}
			if (read (int6k->NVM.file, &header_nvm, sizeof (header_nvm)) < sizeof (header_nvm)) 
			{
				error (1, errno, "Can't read %s", filepart (int6k->NVM.name));
			}
			while (header_nvm.NEXTHEADER) 
			{
				lseek (int6k->NVM.file, LE32TOH (header_nvm.NEXTHEADER), SEEK_SET);
				if (read (int6k->NVM.file, &header_nvm, sizeof (header_nvm)) < sizeof (header_nvm)) 
				{
					error (1, errno, "Can't read %s", filepart (int6k->NVM.name));
				}
			}
			if (WriteMEM (int6k, &int6k->NVM, LE32TOH (header_nvm.IMAGEADDR), LE32TOH (header_nvm.IMAGELENGTH))) 
			{
				return (-1);
			}
			if (lseek (int6k->PIB.file, 0, SEEK_SET)) 
			{
				error (1, errno, "Can't home %s", filepart (int6k->PIB.name));
			}
			if (read (int6k->PIB.file, &header_pib, sizeof (header_pib)) < sizeof (header_pib)) 
			{
				error (1, errno, "Can't read %s", filepart (int6k->PIB.name));
			}
			if (lseek (int6k->PIB.file, 0, SEEK_SET)) 
			{
				error (1, errno, "Can't home %s", filepart (int6k->PIB.name));
			}
			if (ntohs (*(uint16_t *)(&header_pib)) < 0x0305) 
			{
				offset = LEGACY_PIBOFFSET;
			}
			if (WriteMEM (int6k, &int6k->PIB, offset, LE16TOH (header_pib.PIBLENGTH))) 
			{
				return (-1);
			}
			if (StartFirmware (int6k, &header_nvm)) 
			{
				return (-1);
			}
			if (_anyset (int6k->flags, INT6K_FLASH_DEVICE)) 
			{
				if (WriteNVM (int6k)) 
				{
					return (-1);
				}
				if (WritePIB (int6k)) 
				{
					return (-1);
				}
				if (Flash (int6k)) 
				{
					return (-1);
				}
			}
		}
		else if (indicate->MACTION == 0x01) 
		{
			if (HostActionResponse (int6k)) 
			{
				return (-1);
			}
			close (int6k->NVM.file);
			if (ReadNVM (int6k)) 
			{
				return (-1);
			}
			int6k->NVM.name = int6k->nvm.name;
			if ((int6k->NVM.file = open (int6k->NVM.name, O_BINARY|O_RDONLY)) == -1) 
			{
				error (1, errno, "%s", filepart (int6k->NVM.name));
			}
			if (ResetDevice (int6k)) 
			{
				return (-1);
			}
		}
		else if (indicate->MACTION == 0x02) 
		{
			if (HostActionResponse (int6k)) 
			{
				return (-1);
			}
			close (int6k->PIB.file);
			if (ReadPIB (int6k)) 
			{
				return (-1);
			}
			int6k->PIB.name = int6k->pib.name;
			if ((int6k->PIB.file = open (int6k->PIB.name, O_BINARY|O_RDONLY)) == -1) 
			{
				error (1, errno, "%s", filepart (int6k->PIB.name));
			}
			if (ResetDevice (int6k)) 
			{
				return (-1);
			}
		}
		else if (indicate->MACTION == 0x03) 
		{
			if (HostActionResponse (int6k)) 
			{
				return (-1);
			}
			close (int6k->NVM.file);
			if (ReadNVM (int6k)) 
			{
				return (-1);
			}
			int6k->NVM.name = int6k->nvm.name;
			if ((int6k->NVM.file = open (int6k->NVM.name, O_BINARY|O_RDONLY)) == -1) 
			{
				error (1, errno, "%s", filepart (int6k->NVM.name));
			}
			close (int6k->PIB.file);
			if (ReadPIB (int6k)) 
			{
				return (-1);
			}
			int6k->PIB.name = int6k->pib.name;
			if ((int6k->PIB.file = open (int6k->PIB.name, O_BINARY|O_RDONLY)) == -1) 
			{
				error (1, errno, "%s", filepart (int6k->PIB.name));
			}
			if (ResetDevice (int6k)) 
			{
				return (-1);
			}
		}
		else if (indicate->MACTION == 0x04) 
		{

#if 0

/*
 *	Due to an omission in the INT6300 BootLoader, responding to this VS_HST_ACTION 
 *      indication will suppress subsequent VS_HST_ACTION messages and the device will 
 *     	not request firmware and parameters; this may be corrected on the INT6400; 
 */

			if (HostActionResponse (int6k)) 
			{
				return (-1);
			}

#endif

			if (WriteCFG (int6k)) 
			{
				return (-1);
			}

/*
 *	At this point, one could download firmware and parameters without waiting for 
 *	further  requests from the device; however, we elect to wait for them since it 
 *	is 'good form'; a device should send code 0x00 within 10 seconds of this one; 
 */

		}
		else if (indicate->MACTION == 0x05) 
		{
			if (HostActionResponse (int6k)) 
			{
				return (-1);
			}
			close (int6k->NVM.file);
			int6k->NVM.name = NVM;
			if ((int6k->NVM.file = open (int6k->NVM.name, O_BINARY|O_RDONLY)) == -1) 
			{
				error (1, errno, "%s", filepart (int6k->NVM.name));
			}
			close (int6k->PIB.file);
			int6k->PIB.name = PIB;
			if ((int6k->PIB.file = open (int6k->PIB.name, O_BINARY|O_RDONLY)) == -1) 
			{
				error (1, errno, "%s", filepart (int6k->PIB.name));
			}
			if (ResetDevice (int6k)) 
			{
				return (-1);
			}
		}
		channel->timeout = -1;
		printf ("\n");
	}
	return (0);
}

#endif

