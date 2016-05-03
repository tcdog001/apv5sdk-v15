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
 *   message.h - Ethernet message definitions and declarations;
 *
 *.  Intellon INT6x00 Linux Toolkit for HomePlug AV; 
 *:  Published 2007 by Intellon Corp. ALL RIGHTS RESERVED;
 *;  For demonstration; Not for production use;
 *
 *   Contributor(s):
 *      Charles Maier <charles.maier@intellon.com>
 *
 *--------------------------------------------------------------------*/

#ifndef MESSAGE_HEADER
#define MESSAGE_HEADER

/*====================================================================*
 *   system header files;
 *--------------------------------------------------------------------*/

#include <stdint.h>

/*====================================================================*
 *   custom header files;
 *--------------------------------------------------------------------*/

#include "../ether/ether.h"

/*====================================================================*
 *   cope with structure packing vagaries;
 *--------------------------------------------------------------------*/

#ifndef __packed
#ifdef __GNUC__
#define __packed __attribute__ ((packed))
#else
#define __packed
#endif
#endif

/*====================================================================*
 *   Ethernet, HomePlug and Intellon frame headers;
 *--------------------------------------------------------------------*/

#ifndef __GNUC__
#pragma pack (push, 1)
#endif

typedef struct __packed header_std 

{
	uint8_t MMV;
	uint16_t MMTYPE;
}

header_std;

#ifndef IHP_HEADER

typedef struct __packed header_eth 

{
	uint8_t ODA [ETHER_ADDR_LEN];
	uint8_t OSA [ETHER_ADDR_LEN];
	uint16_t MTYPE;
}

header_eth;
typedef struct __packed header_int 

{
	uint8_t MMV;
	uint16_t MMTYPE;
	uint8_t OUI [ETHER_ADDR_LEN >> 1];
}

header_int;
typedef struct __packed header_mme 

{
	uint8_t MMV;
	uint16_t MMTYPE;
	uint16_t FMI;
}

header_mme;

#endif

#ifndef __GNUC__
#pragma pack (pop)
#endif

/*====================================================================*
 *   Intellon message;
 *--------------------------------------------------------------------*/

#ifndef __GNUC__
#pragma pack (push, 1)
#endif

typedef struct __packed message 

{
	struct header_eth ethernet;
	struct header_int intellon;
	uint8_t content [ETHERMTU - sizeof (struct header_int)];
}

MESSAGE;

#ifndef __GNUC__
#pragma pack (pop)
#endif

/*====================================================================*
 *   header encode functions;
 *--------------------------------------------------------------------*/

signed ProtocolHeader (void * memory, const uint8_t peer [], const uint8_t host [], uint16_t protocol);
signed EthernetHeader (struct header_eth *, const uint8_t peer [], const uint8_t host []);
signed FragmentHeader (struct header_mme *, uint16_t MMTYPE, uint16_t number);
signed StandardHeader (struct header_std *, uint16_t MMTYPE);
signed IntellonHeader (struct header_int *, uint16_t MMTYPE);
signed UnwantedPacket (const void * memory, uint16_t MMTYPE);

/*====================================================================*
 *
 *--------------------------------------------------------------------*/

#endif

