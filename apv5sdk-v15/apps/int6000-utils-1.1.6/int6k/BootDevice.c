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
 *   int BootDevice (struct int6k * int6k);
 *
 *   int6k.h
 *  
 *   boot device by writing firmware and parameters directly to SDRAM
 *   using function WriteMEM then starting the firmware using function
 *   StartFirmware; the Bootloader must be running when this function 
 *   is called; 
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

#ifndef BOOTDEVICE_SOURCE
#define BOOTDEVICE_SOURCE

#include <stdint.h>
#include <unistd.h>
#include <memory.h>
#include <errno.h>

#include "../int6k/int6k.h"
#include "../tools/files.h"
#include "../tools/error.h"
#include "../tools/flags.h"
#include "../nvm/nvm.h"
#include "../pib/pib.h"

int BootDevice (struct int6k * int6k) 

{
	struct header_nvm header_nvm;
	struct header_pib header_pib;
	uint32_t offset = INT6K_PIBOFFSET;
	unsigned image = 0;
	memset (&header_nvm, 0, sizeof (header_nvm));
	if (lseek (int6k->NVM.file, 0, SEEK_SET)) 
	{
		error (1, errno, "Can't home %s", filepart (int6k->NVM.name));
	}
	do 
	{
		if (lseek (int6k->NVM.file, 0, SEEK_CUR) != LE32TOH (header_nvm.NEXTHEADER)) 
		{
			error (1, 0, "Bad file position: %s (%d)", filepart (int6k->NVM.name), image);
		}
		if (read (int6k->NVM.file, &header_nvm, sizeof (header_nvm)) < sizeof (header_nvm)) 
		{
			error (1, errno, "Can't read header: %s (%d)", filepart (int6k->NVM.name), image);
		}
		if (LE32TOH (header_nvm.HEADERVERSION) != NVM_HEADER_VERSION) 
		{
			error (1, 0, "Bad header version: %s (%d)", filepart (int6k->NVM.name), image);
		}
		if (checksum_32 (&header_nvm, sizeof (struct header_nvm), 0)) 
		{
			error (1, 0, "Bad header checksum: %s (%d)", filepart (int6k->NVM.name), image);
		}
		if (!image && !header_nvm.HEADERMINORVERSION) 
		{
			error (1, ENOTSUP, "Legacy SDRAM parameters detected");
		}
		if (!header_nvm.NEXTHEADER && !header_nvm.HEADERMINORVERSION) 
		{
			error (1, ENOTSUP, "Legacy firmware image detected");
		}
		if (_anyset (LE32TOH (header_nvm.IGNOREMASK), (1 << (int6k->hardwareID - 1)))) 
		{
			if (lseek (int6k->NVM.file, LE32TOH (header_nvm.NEXTHEADER), SEEK_SET) == -1) 
			{
				error (1, errno, "Can't skip image: %s", filepart (int6k->NVM.name));
			}
		}
		else if (header_nvm.IMAGETYPE == NVM_IMAGE_MEMCTL) 
		{
			if (WriteMEM (int6k, &int6k->NVM, LE32TOH (header_nvm.IMAGEADDR), LE32TOH (header_nvm.IMAGELENGTH))) 
			{
				return (-1);
			}
			if (StartFirmware (int6k, &header_nvm)) 
			{
				return (-1);
			}
		}
		else if (header_nvm.IMAGETYPE == NVM_IMAGE_FIRMWARE) 
		{
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
		}
		else if (lseek (int6k->NVM.file, LE32TOH (header_nvm.NEXTHEADER), SEEK_SET) == -1) 
		{
			error (1, errno, "Can't skip image: %s", filepart (int6k->NVM.name));
		}
		image++;
	}
	while (header_nvm.NEXTHEADER);
	return (0);
}

#endif

