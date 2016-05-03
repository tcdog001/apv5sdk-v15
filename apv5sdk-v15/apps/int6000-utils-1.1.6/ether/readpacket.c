/*====================================================================*
 *
 *   signed readpacket (struct channel * channel, void * memory, signed length);
 *
 *   channel.h
 *
 *   read one packet from a raw packet channel; 
 *
 *   return the packet size on success, 0 on timeout or -1 on error; 
 *   dump packets on stdout when the channel VERBOSE flag is set; 
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

#ifndef READPACKET_SOURCE
#define READPACKET_SOURCE

#include <unistd.h>
#include <memory.h>
#include <errno.h>

#include "../ether/channel.h"
#include "../tools/memory.h"
#include "../tools/error.h" 
#include "../tools/flags.h"

signed readpacket (struct channel * channel, void * memory, signed extent) 

{

#if defined (__linux__) 

#include <sys/poll.h>

	struct pollfd pollfd = 
	{
		channel->fd,
		POLLIN,
		0 
	};
	int status = poll (&pollfd, 1, channel->timeout);
	memset (memory, 0, extent);
	if (status < 0) 
	{
		error (0, errno, "%s can't poll %s", __func__, channel->name);
		return (-1);
	}
	if (status > 0) 
	{
		extent = recvfrom (channel->fd, memory, extent, 0, (struct sockaddr *) (0), (socklen_t *)(0));
		if (extent == -1) 
		{
			error (0, errno, "%s can't read from %s", __func__, channel->name);
			return (-1);
		}
		if (_anyset (channel->flags, CHANNEL_VERBOSE)) 
		{
			hexdump (memory, 0, extent, stdout);
		}
		return (extent);
	}

#elif defined (__APPLE__) 

	byte buffer [channel->bpf_length];
	struct bpf_hdr * bpf_packet = (struct bpf_hdr *)(buffer);
	memset (memory, 0, extent);
	memset (buffer, 0, sizeof (buffer));
	extent = read (channel->fd, buffer, sizeof (buffer));
	if (extent < 0) 
	{
		error (0, errno, "bpf");
		return (-1);
	}
	if (extent > 0) 
	{
		extent = bpf_packet->bh_caplen;
		memcpy (memory, buffer + bpf_packet->bh_hdrlen, bpf_packet->bh_caplen);
		if (_anyset (channel->flags, CHANNEL_VERBOSE)) 
		{
			hexdump (memory, 0, extent, stdout);
		}
		return (extent);
	}

#elif defined (__OpenBSD__)

	byte buffer [channel->bpf_length];
	struct bpf_hdr * bpf_packet = (struct bpf_hdr *)(buffer);
	memset (memory, 0, extent);
	memset (buffer, 0, sizeof (buffer));
	extent = read (channel->fd, buffer, sizeof (buffer));
	if (extent < 0) 
	{
		error (0, errno, "bpf");
		return (-1);
	}
	if (extent > 0) 
	{
		extent = bpf_packet->bh_caplen;
		memcpy (memory, buffer + bpf_packet->bh_hdrlen, bpf_packet->bh_caplen);
		if (_anyset (channel->flags, CHANNEL_VERBOSE)) 
		{
			hexdump (memory, 0, extent, stdout);
		}
		return (extent);
	}

#elif defined (WINPCAP) || defined (LIBPCAP)

	struct pcap_pkthdr * header;
	const uint8_t *data;
	int elapsed = 0;
	do 
	{
		signed status = pcap_next_ex (channel->socket, &header, &data);
		memset (memory, 0, extent);
		if (status < 0) 
		{
			error (0, errno, "pcap_next_ex");
			return (-1);
		}
		if (status > 0) 
		{
			memcpy (memory, data, header->caplen);
			extent = header->caplen;
			if (_anyset (channel->flags, CHANNEL_VERBOSE)) 
			{
				hexdump (memory, 0, extent, stdout);
			}
			return (extent);
		}
		elapsed += CHANNEL_PCAP_TIMEOUT;
	}
	while (elapsed < channel->timeout);

#else
#error "Unknown Environment"
#endif

	return (0);
}

#endif

