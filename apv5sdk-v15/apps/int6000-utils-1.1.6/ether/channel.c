/*====================================================================*
 *
 *   channel.c - global channel structure;
 *
 *   channel.h
 *
 *   define and initialize a global channel structure; this structure
 *   is initialized for communication with Intellon devices and it is
 *   referenced by Intellon Linux Toolkit programs that do not need a 
 *   full int6k data structure;
 *
 *.  Intellon INT6000 Linux Toolkit for HomePlug AV;
 *:  Published 2006-2008 by Intellon Corp. ALL RIGHTS RESERVED;
 *;  For demonstration; Not for production use;
 *
 *   Contributor(s):
 *      Charles Maier <charles.maier@intellon.com>
 *      Nathan Houghton <nathan.houghton@intellon.com>
 *
 *--------------------------------------------------------------------*/

#ifndef CHANNEL_SOURCE
#define CHANNEL_SOURCE

#include "../ether/channel.h"
#include "../ether/ether.h"

struct channel channel = 

{
	-1,
	CHANNEL_ETHNUMBER,
	CHANNEL_ETHDEVICE,
	{
		0x00,
		0xB0,
		0x52,
		0x00,
		0x00,
		0x01 
	},
	{
		0x00,
		0x00,
		0x00,
		0x00,
		0x00,
		0x00 
	},
	CHANNEL_ETHERTYPE,

#if defined (__linux__)

#elif defined (__APPLE__) || defined (__OpenBSD__)

	0,

#elif defined (WINPCAP) || defined (LIBPCAP)

	(pcap_t *)(0),
	{
		0 
	},

#else
#error "Unknown Environment"
#endif

	CHANNEL_TIMEOUT,
	CHANNEL_OPTIONS 
};

#endif

