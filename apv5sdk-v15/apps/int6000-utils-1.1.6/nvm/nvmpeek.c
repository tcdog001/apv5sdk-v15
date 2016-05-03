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
 *   void nvmpeek (struct header_nvm * header_nvm);
 *
 *   nvm.h
 *
 *   display an NVM image header on stdout; 
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
 *.  Intellon INT6000 Linux Toolkit for HomePlug AV;
 *:  Published 2006-2008 by Intellon Corp. ALL RIGHTS RESERVED;
 *;  For demonstration; Not for production use;
 *
 *   Contributor(s):
 *	Charles Maier <charles.maier@intellon.com>
 *
 *--------------------------------------------------------------------*/

#ifndef NVMPEEK_SOURCE
#define NVMPEEK_SOURCE

#include <stdio.h>

#include "../tools/memory.h"
#include "../tools/format.h"
#include "../nvm/nvm.h"

void nvmpeek (struct header_nvm * header_nvm) 

{

#define FUNCTIONS_COUNT 8

	static const char * platforms [] = 
	{
		"INT6000",
		"INT6300",
		"INT6400",
		(const char *)(0) 
	};
	static const char * functions []= 
	{
		"Generic Image",
		"Synopsis Configuration",
		"Denali Configuration",
		"Denali Applet",
		"Firmware Image",
		"OAS Client",
		"Custom Image",
		"Memory Control",
		"Unknown" 
	};
	char platform [100];
	strfbits (platform, sizeof (platform), platforms, "|", LE32TOH (header_nvm->IGNOREMASK));
	printf ("\tHeader Version = 0x%08X-%02X\n", LE32TOH (header_nvm->HEADERVERSION), header_nvm->HEADERMINORVERSION);
	printf ("\tHeader Checksum = 0x%08X\n", LE32TOH (header_nvm->HEADERCHECKSUM));
	printf ("\tHeader Next = 0x%08X\n", LE32TOH (header_nvm->NEXTHEADER));
	printf ("\tFlash Address = 0x%08X\n", LE32TOH (header_nvm->IMAGEROMADDR));
	printf ("\tImage Address = 0x%08X\n", LE32TOH (header_nvm->IMAGEADDR));
	printf ("\tEntry Address = 0x%08X\n", LE32TOH (header_nvm->ENTRYPOINT));
	printf ("\tImage Checksum = 0x%08X\n", LE32TOH (header_nvm->IMAGECHECKSUM));
	printf ("\tImage Size = 0x%08X (%d)\n", LE32TOH (header_nvm->IMAGELENGTH), LE32TOH (header_nvm->IMAGELENGTH));
	if (header_nvm->IMAGETYPE < FUNCTIONS_COUNT) 
	{
		printf ("\tImage Type = %s\n", functions [header_nvm->IMAGETYPE]);
	}
	else 
	{
		printf ("\tImage Type = %s\n", functions [FUNCTIONS_COUNT]);
	}
	printf ("\tImage Omit = %s\n", platform);
	return;
}

#endif

