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
 *   channel.h - raw packet channel definitions and declarations;
 *
 *   the channel structure contains information needed to perform
 *   raw packet I/O on specific platforms;
 *
 *.  Intellon Linux Toolkit; 
 *:  Published 2007 by Intellon Corp. ALL RIGHTS RESERVED;
 *;  For demonstration; Not for production use;
 *
 *   Contributor(s):
 *      Charles Maier <charles.maier@intellon.com>
 *      Nathan Houghton <nathan.houghton@intellon.com>
 *	Abdel Younes <younes@leacom.fr>
 *
 *--------------------------------------------------------------------*/

#ifndef CHANNEL_HEADER
#define CHANNEL_HEADER

/*====================================================================*
 *  system header files;
 *--------------------------------------------------------------------*/

#include <stdint.h>

/*====================================================================*
 *  custom header files;
 *--------------------------------------------------------------------*/

#include "../tools/types.h"
#include "../ether/ether.h"

/*====================================================================*
 *   channel flagword bitmasks;
 *--------------------------------------------------------------------*/

#define CHANNEL_VERBOSE (1 << 0)
#define CHANNEL_SILENCE (1 << 1)
#define CHANNEL_WARNING (1 << 2)
#define CHANNEL_SUCCESS (1 << 3)
#define CHANNEL_FAILURE (1 << 4)

#define CHANNEL_UPDATE_TARGET (1 << 5) /* used by efsu only */
#define CHANNEL_UPDATE_SOURCE (1 << 6) /* used by efsu only */
#define CHANNEL_LISTEN (1 << 7)	       /* used by efsu only */

#define CHANNEL_ETHNUMBER 1
#if defined (__linux__)
#	define CHANNEL_ETHDEVICE "eth1"
#elif defined (__APPLE__)
#	define CHANNEL_ETHDEVICE "en0"
#	define CHANNEL_BPFDEVICE "/dev/bpf%d"
#elif defined (__OpenBSD__)
#	define CHANNEL_ETHDEVICE "bce0"
#	define CHANNEL_BPFDEVICE "/dev/bpf%d"
#else
#	define CHANNEL_ETHDEVICE "nic1"
#endif
#define CHANNEL_ETHERTYPE 0x88E1 /* in host byte order */
#define CHANNEL_PCAP_TIMEOUT 10
#define CHANNEL_TIMEOUT 50
#define CHANNEL_BAILOUT 0
#define CHANNEL_OPTIONS 0

/*====================================================================*
 *   communication channel structure;
 *--------------------------------------------------------------------*/

typedef struct channel 

{
	signed fd;
	signed id;
	const char * name;
	uint8_t peer [ETHER_ADDR_LEN];
	uint8_t host [ETHER_ADDR_LEN];
	uint16_t type;

#if defined (__linux__)

#elif defined (__APPLE__) 

	signed bpf_length;

#elif defined (__OpenBSD__)

	signed bpf_length;

#elif defined (WINPCAP) 

	pcap_t * socket;
	char errbuf [PCAP_ERRBUF_SIZE];

#else
#error "Unknown Environment"
#endif

	signed timeout;
	flag_t flags;
}

CHANNEL;

/*====================================================================*
 *   channel functions;
 *--------------------------------------------------------------------*/

signed openchannel (struct channel *);
signed closechannel (struct channel *);
signed sendpacket (struct channel *, void * packet, signed packetsize);
signed readpacket (struct channel *, void * packet, signed packetsize);

/*====================================================================*
 *   
 *--------------------------------------------------------------------*/

#endif

