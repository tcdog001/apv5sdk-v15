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
 *   nvm.h - nvm file format definitions and declarations;
 *
 *   The Boot ROM reads a linked list of NVM headers containing 
 *   information about the size, location and execution address 
 *   of various software images;
 *
 *   See INT6300 Boot ROM Software Design Specification for more 
 *   information; the document may not be available to some
 *   customers so don't push it;
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
 *	Charles Maier <charles.maier@intellon.com>
 *
 *--------------------------------------------------------------------*/

#ifndef NVM_HEADER
#define NVM_HEADER

/*====================================================================*
 *   system header files;
 *--------------------------------------------------------------------*/

#include <stdint.h>

/*====================================================================*
 *   custom header files;
 *--------------------------------------------------------------------*/

#include "../tools/types.h"

/*====================================================================*
 *   nvm constants;
 *--------------------------------------------------------------------*/

#define NVM_LEGACY 1
#define NVM_LATEST 1

#define NVM_SILENCE    	(1 << 0)
#define NVM_VERBOSE   	(1 << 1)
#define NVM_BASENAME   	(1 << 2)
#define NVM_TRACE 	(1 << 3)
#define NVM_SDRAM 	(1 << 4)
#define NVM_VERSION 	(1 << 5)
#define NVM_FINAL 	(1 << 6)

/*====================================================================*
 *   nvm constants;
 *--------------------------------------------------------------------*/

#define NVM_HEADER_VERSION 0x60000000
#define NVM_HEADER_MINOR_VERSION 1 

#define NVM_CHIPSET_INT6000 (1 << 0)
#define NVM_CHIPSET_INT6300 (1 << 1)
#define NVM_CHIPSET_INT6400 (1 << 2)

#define NVM_IMAGE_GENERIC 0
#define NVM_IMAGE_CONFIG_SYNOPSIS 1
#define NVM_IMAGE_CONFIG_DENALI 2
#define NVM_IMAGE_APPLET_DENALI 3
#define NVM_IMAGE_FIRMWARE 4
#define NVM_IMAGE_OASCLIENT 5
#define NVM_IMAGE_CUSTOM 6
#define NVM_IMAGE_MEMCTL 7

/*====================================================================*
 *   nvm variables; 
 *--------------------------------------------------------------------*/

#ifndef __GNUC__
#pragma pack (push,1)
#endif

typedef struct __packed header_nvm 

{
	uint32_t HEADERVERSION;
	uint32_t IMAGEROMADDR;
	uint32_t IMAGEADDR;
	uint32_t IMAGELENGTH;
	uint32_t IMAGECHECKSUM;
	uint32_t ENTRYPOINT;
	uint8_t HEADERMINORVERSION;
	uint8_t IMAGETYPE;
	uint16_t IGNOREMASK;
	uint32_t RESERVED [2];
	uint32_t NEXTHEADER;
	uint32_t HEADERCHECKSUM;
}

header_nvm;

#ifndef __GNUC__
#pragma pack (pop)
#endif

/*====================================================================*
 *   nvm functions;
 *--------------------------------------------------------------------*/

signed nvmspan (const void * memory);
void nvmpeek (struct header_nvm * header_nvm);
signed nvmfile (struct _file_ *file);

/*====================================================================*
 *   
 *--------------------------------------------------------------------*/

#endif

