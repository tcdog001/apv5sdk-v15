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
 *   signed EthernetHeader (struct header_eth * header, const uint8_t ODA [], const uint8_t OSA []);
 *
 *   message.h
 *
 *   encode buffer with a standard Ethernet header having a specified
 *   source address (OSA) and destination address (ODA) address and a
 *   HomePlug AV ethertype (HOMEPLUG_MTYPE);
 *
 *   return the number of bytes encoded or 0 on encode error; set
 *   errno to the appropriate POSIX error number;
 *
 *   OSA is the hardware (MAC) address of the source device;
 *
 *   ODA is the hardware (MAC) address of the desintation device;
 *
 *   MTYPE is the HomePlug AV Ethertype;
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

#ifndef ETHERNETHEADER_SOURCE
#define ETHERNETHEADER_SOURCE

#include <stdint.h>
#include <memory.h>
#include <arpa/inet.h>

#include "../int6k/homeplug.h"
#include "../int6k/message.h"

const uint8_t broadcast [ETHER_ADDR_LEN] = 

{
	0xFF,
	0xFF,
	0xFF,
	0xFF,
	0xFF,
	0xFF 
};

signed EthernetHeader (struct header_eth * header, const uint8_t ODA [], const uint8_t OSA []) 

{
	memcpy (header->ODA, ODA, sizeof (header->ODA));
	memcpy (header->OSA, OSA, sizeof (header->OSA));
	header->MTYPE = htons (HOMEPLUG_MTYPE);
	return (sizeof (struct header_eth));
}

#endif

