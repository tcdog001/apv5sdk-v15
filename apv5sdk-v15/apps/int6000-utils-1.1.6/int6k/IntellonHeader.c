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
 *   signed IntellonHeader (struct header_int * header, uint16_t MMTYPE);
 *
 *   message.h
 *
 *   Encode memory with an Intellon vendor specific message header 
 *   having HomePlug message protocol version (INTELLON_MMV) and 
 *   Intellon message type (MMTYPE);
 *
 *   return the number of bytes actually encoded or 0 on encode error; 
 *   the error code is stored in errno;
 *
 *   see the INT6000 Firmware Technical Reference Manual for more
 *   about MME headers and message types; the Intellon OUI is 
 *   implicit in this function;
 *
 *   MMV is the version number of the MME command set; currently, 
 *   there is only one command set for Intellon MMEs; 
 *
 *   MMTYPE indicates the desired Intellon device operation taken
 *   from the TRM; some operations are undocumented and should not
 *   be used;
 *
 *   OUI is the Organizationally Unique Identifier resgistered with
 *   the IEEE by the vendor and is a constant for Intellon Devices; 
 *
 *   There is no need to flush the header since this function writes
 *   to all locations unless there is an error; the caller may elect
 *   to flush the buffer before calling this function;
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
 *	Charles Maier, charles.maier@intellon.com
 *
 *--------------------------------------------------------------------*/

#ifndef INTELLONHEADER_SOURCE
#define INTELLONHEADER_SOURCE

#include <stdint.h>
#include <memory.h>

#include "../int6k/intellon.h"
#include "../int6k/message.h"
#include "../tools/memory.h"

const uint8_t localcast [ETHER_ADDR_LEN] = 

{
	0x00,
	0xB0,
	0x52,
	0x00,
	0x00,
	0x01 
};

signed IntellonHeader (struct header_int * header, uint16_t MMTYPE) 

{
	extern const uint8_t localcast [ETHER_ADDR_LEN];
	header->MMV = INTELLON_MMV;
	header->MMTYPE = HTOLE16 (MMTYPE);
	memcpy (header->OUI, localcast, ETHER_ADDR_LEN >> 1);
	return (sizeof (struct header_int));
}

#endif

