/*====================================================================*
 *
 *   signed sendpacket (struct channel * channel, void * packet, signed length);
 *
 *   channel.h
 *   
 *   send one packet over a raw packet channel; return the number of 
 *   bytes sent or -1 on error; dump outgoing packets on stdout when 
 *   the VERBOSE flag is set; 
 *
 *.  Intellon INT6000 Linux Toolkit for HomePlug AV;
 *:  Published 2006-2008 by Intellon Corp. ALL RIGHTS RESERVED;
 *;  For demonstration; Not for production use;
 *
 *   Contributor(s):
 *	Charles Maier <charles.maier@intellon.com>
 *	Nathan Houghton <nathan.houghton@intellon.com>
 *
 *--------------------------------------------------------------------*/

#ifndef SENDPACKET_SOURCE
#define SENDPACKET_SOURCE

#include <unistd.h>

#include "../ether/channel.h"
#include "../tools/memory.h"
#include "../tools/flags.h"

signed sendpacket (struct channel * channel, void * memory, signed extent) 

{
	if (_anyset (channel->flags, CHANNEL_VERBOSE)) 
	{
		hexdump (memory, 0, extent, stdout);
	}

#if defined (__linux__)

	extent = sendto (channel->fd, memory, extent, 0, (struct sockaddr *) (0), (socklen_t) (0));

#elif defined (__APPLE__)

	extent = write (channel->fd, memory, extent);

#elif defined (__OpenBSD__)

	extent = write (channel->fd, memory, extent);

#elif defined (WINPCAP) || defined (LIBPCAP)

	if (pcap_sendpacket (channel->socket, memory, extent)) 
	{
		extent = -1;
	}

#else
#error "Unknown Environment"
#endif

	return (extent);
}

#endif

