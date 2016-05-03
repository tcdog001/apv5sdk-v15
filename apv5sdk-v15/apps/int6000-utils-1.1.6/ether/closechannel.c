/*====================================================================*
 *
 *   signed closechannel (struct channel * channel);
 *
 *   channel.h
 *
 *   close ethernet raw packet channel;
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

#ifndef CLOSECHANNEL_SOURCE
#define CLOSECHANNEL_SOURCE

#include <unistd.h>

#include "../ether/channel.h"

signed closechannel (struct channel * channel) 

{

#if defined (__linux__) 

	return (close (channel->fd));

#elif defined (__APPLE__) || (__OpenBSD__)

	return (close (channel->fd));

#elif defined (WINPCAP) || defined (LIBPCAP)

	pcap_close (channel->socket);
	return (0);

#else
#error "Unknown Environment"
#endif

}

#endif

