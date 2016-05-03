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
 *   int StartDevice (struct int6k * int6k);
 *
 *   int6k.h
 *  
 *   This int6kf plugin initialize a device having no NVRAM or blank 
 *   or corrupted NVRAM; ensure Bootloader is running before starting; 
 *   write SDRAM configuration then NVM and PIB files to SDRAM and 
 *   start firmware execution; 
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
 *	Bill Wike <bill.wike@intellon.com>
 *
 *--------------------------------------------------------------------*/

#ifndef STARTDEVICE_SOURCE
#define STARTDEVICE_SOURCE

#include <stdint.h>
#include <unistd.h>
#include <memory.h>
#include <errno.h>

#include "../int6k/int6k.h"
#include "../tools/error.h"
#include "../nvm/nvm.h"
#include "../pib/pib.h"

int StartDevice (struct int6k * int6k) 

{
	struct header_nvm header_nvm;
	struct header_pib header_pib;
	uint32_t offset = INT6K_PIBOFFSET;
	if (WriteCFG (int6k)) 
	{
		return (-1);
	}
	if (lseek (int6k->NVM.file, 0, SEEK_SET)) 
	{
		error ((int6k->flags & INT6K_BAILOUT), errno, "Can't home %s", int6k->NVM.name);
		return (-1);
	}
	if (read (int6k->NVM.file, &header_nvm, sizeof (header_nvm)) < sizeof (header_nvm)) 
	{
		error ((int6k->flags & INT6K_BAILOUT), errno, "Can't read %s", int6k->NVM.name);
		return (-1);
	}
	while (header_nvm.NEXTHEADER) 
	{
		if (lseek (int6k->NVM.file, LE32TOH (header_nvm.NEXTHEADER), SEEK_SET) == -1) 
		{
			error ((int6k->flags & INT6K_BAILOUT), errno, "Can't home %s", int6k->NVM.name);
			return (-1);
		}
		if (read (int6k->NVM.file, &header_nvm, sizeof (header_nvm)) < sizeof (header_nvm)) 
		{
			error ((int6k->flags & INT6K_BAILOUT), errno, "Can't read %s", int6k->NVM.name);
			return (-1);
		}
	}
	if (WriteMEM (int6k, &int6k->NVM, LE32TOH (header_nvm.IMAGEADDR), LE32TOH (header_nvm.IMAGELENGTH))) 
	{
		return (-1);
	}
	if (lseek (int6k->NVM.file, 0, SEEK_SET)) 
	{
		error ((int6k->flags & INT6K_BAILOUT), errno, "Can't home %s", int6k->NVM.name);
		return (-1);
	}
	if (lseek (int6k->PIB.file, 0, SEEK_SET)) 
	{
		error ((int6k->flags & INT6K_BAILOUT), errno, "Can't home %s", int6k->PIB.name);
		return (-1);
	}
	if (read (int6k->PIB.file, &header_pib, sizeof (header_pib)) < sizeof (header_pib)) 
	{
		error ((int6k->flags & INT6K_BAILOUT), errno, "Can't read %s", int6k->PIB.name);
		return (-1);
	}
	if (lseek (int6k->PIB.file, 0, SEEK_SET)) 
	{
		error ((int6k->flags & INT6K_BAILOUT), errno, "can't home %s", int6k->PIB.name);
		return (-1);
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
	return (0);
}

#endif

