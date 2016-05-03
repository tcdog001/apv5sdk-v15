/*====================================================================*
 *
 *   signed openchannel (struct channel * channel);
 *
 *   channel.h
 *
 *   open a raw ethernet channel;
 *
 *.  Intellon Linux Toolkit; 
 *:  Published 2007 by Intellon Corp. ALL RIGHTS RESERVED;
 *;  For demonstration; Not for production use;
 *
 *   Contributor(s):
 *	Charles Maier <charles.maier@intellon.com>
 *	Nathan Houghton <nathan.houghton@intellon.com>
 *
 *--------------------------------------------------------------------*/

#ifndef OPENCHANNEL_SOURCE
#define OPENCHANNEL_SOURCE

#include <unistd.h>
#include <memory.h>
#include <errno.h>

#if defined (__linux__)
#	include <net/if_arp.h>
#	include <netpacket/packet.h>
#	include <sys/ioctl.h>
#elif defined (__APPLE__)
#	include <fcntl.h>
#	include <sys/ioctl.h>
#elif defined (__OpenBSD__)
#	include <sys/ioctl.h>
#elif defined (WINPCAP) || defined (LIBPCAP)
#	include <string.h>
#ifndef _MSC_VER 
#	include "../ether/getifname.c"
#	include "../ether/gethwaddr.c"
#endif
#else
#error "Unknown environment"
#endif

#include "../ether/channel.h"
#include "../tools/memory.h"
#include "../tools/error.h"

#if defined (__APPLE__) || defined (__OpenBSD__)
#include "../ether/gethwaddr.c"
#endif

signed openchannel (struct channel * channel) 

{

#if defined (__linux__) 

	struct ifreq ifreq;
	struct sockaddr_ll sockaddr_ll = 
	{
		PF_PACKET,
		0x0000,
		0x0000,
		ARPHRD_ETHER,
		PACKET_OTHERHOST,
		ETHER_ADDR_LEN,
		{
			0x00,
			0x00,
			0x00,
			0x00,
			0x00,
			0x00,
			0x00,
			0x00 
		}
	};
	memset (&ifreq, 0, sizeof (ifreq));
	sockaddr_ll.sll_protocol = htons (channel->type);
	if ((channel->fd = socket (sockaddr_ll.sll_family, SOCK_RAW, sockaddr_ll.sll_protocol)) == -1) 
	{
		error (1, errno, "Can't open %s", channel->name);
	}
	memcpy (ifreq.ifr_name, channel->name, sizeof (ifreq.ifr_name));
	if (ioctl (channel->fd, SIOCGIFINDEX, &ifreq) == -1) 
	{
		error (1, errno, "Can't read index: %s", ifreq.ifr_name);
	}
	channel->id = sockaddr_ll.sll_ifindex = ifreq.ifr_ifindex;
	if (ioctl (channel->fd, SIOCGIFHWADDR, &ifreq) == -1) 
	{
		error (1, errno, "Can't read address: %s", ifreq.ifr_name);
	}
#if 0
	memcpy (channel->host, ifreq.ifr_ifru.ifru_hwaddr.sa_data, sizeof (channel->host));
#else
	memcpy (sockaddr_ll.sll_addr, ifreq.ifr_ifru.ifru_hwaddr.sa_data, sizeof (sockaddr_ll.sll_addr));
#endif
	if (bind (channel->fd, (struct sockaddr *) (&sockaddr_ll), sizeof (sockaddr_ll)) == -1) 
	{
		error (1, errno, "Can't bind socket: %s", ifreq.ifr_name);
	}

#if 1
	memcpy (channel->host, sockaddr_ll.sll_addr, sizeof (channel->host));
#endif

#if 1

	if (ioctl (channel->fd, SIOCGIFFLAGS, &ifreq) == -1) 
	{
		error (1, errno, "Can't read state: %s", ifreq.ifr_name);
	}
	ifreq.ifr_flags |= (IFF_UP | IFF_BROADCAST | IFF_MULTICAST);
	ifreq.ifr_flags &= ~IFF_ALLMULTI;
	if (ioctl (channel->fd, SIOCSIFFLAGS, &ifreq) == -1) 
	{
		error (1, errno, "Can't save state: %s", ifreq.ifr_name);
	}

#endif

#else

	struct bpf_program bpf_program;
	static struct bpf_insn bpf_insn [] = 
	{
		{ BPF_LD + BPF_H + BPF_ABS, 0, 0, 12 },
		{ BPF_JMP + BPF_JEQ + BPF_K, 0, 25, 0 },
		{ BPF_LD + BPF_B+BPF_ABS, 0, 0, 0 },
		{ BPF_JMP + BPF_JEQ + BPF_K, 0, 10, 0 },
		{ BPF_LD + BPF_B + BPF_ABS, 0, 0, 1 },
		{ BPF_JMP + BPF_JEQ + BPF_K, 0, 8, 0 },
		{ BPF_LD + BPF_B + BPF_ABS, 0, 0, 2 },
		{ BPF_JMP + BPF_JEQ + BPF_K, 0, 6, 0 },
		{ BPF_LD + BPF_B + BPF_ABS, 0, 0, 3 },
		{ BPF_JMP + BPF_JEQ + BPF_K, 0, 4, 0 },
		{ BPF_LD + BPF_B + BPF_ABS, 0, 0, 4 },
		{ BPF_JMP + BPF_JEQ + BPF_K, 0, 2, 0 },
		{ BPF_LD + BPF_B + BPF_ABS, 0, 0, 5 },
		{ BPF_JMP + BPF_JEQ + BPF_K, 12, 1, 0 },
		{ BPF_LD + BPF_B+BPF_ABS, 0, 0, 0 },
		{ BPF_JMP + BPF_JEQ + BPF_K, 0, 11, 0xFF },
		{ BPF_LD + BPF_B+BPF_ABS, 0, 0, 1 },
		{ BPF_JMP + BPF_JEQ + BPF_K, 0, 9, 0xFF },
		{ BPF_LD + BPF_B+BPF_ABS, 0, 0, 2 },
		{ BPF_JMP + BPF_JEQ + BPF_K, 0, 7, 0xFF },
		{ BPF_LD + BPF_B+BPF_ABS, 0, 0, 3 },
		{ BPF_JMP + BPF_JEQ + BPF_K, 0, 5, 0xFF },
		{ BPF_LD + BPF_B+BPF_ABS, 0, 0, 4 },
		{ BPF_JMP + BPF_JEQ + BPF_K, 0, 3, 0xFF },
		{ BPF_LD + BPF_B+BPF_ABS, 0, 0, 5 },
		{ BPF_JMP + BPF_JEQ + BPF_K, 0, 1, 0xFF },
		{ BPF_RET + BPF_K, 0, 0, 4096 },
		{ BPF_RET + BPF_K, 0, 0, 0 }
	};

#if defined (__APPLE__) || defined (__OpenBSD__)

	struct ifreq ifreq;
	struct timeval timeout = 
	{
		0,
		0 
	};
	char filename [sizeof (CHANNEL_BPFDEVICE) + 1];
	unsigned count;
	unsigned state;
	for (count = 0; count < 100; count++) 
	{
		snprintf (filename, sizeof (filename), CHANNEL_BPFDEVICE, count);
		if ((channel->fd = open (filename, O_RDWR)) != -1) 
		{
			break;
		}
	}
	if (channel->fd == -1) 
	{
		error (1, ECANCELED, "No free bpf devices");
	}
	memcpy (ifreq.ifr_name, channel->name, sizeof (ifreq.ifr_name));
	if (ioctl (channel->fd, BIOCSETIF, &ifreq) == -1) 
	{
		error (1, errno, "%s", ifreq.ifr_name);
	}
	if (ioctl (channel->fd, BIOCGBLEN, &channel->bpf_length) == -1) 
	{
		error (1, errno, "Can't determine buffer length: %s", ifreq.ifr_name);
	}
	state = true;
	if (ioctl (channel->fd, BIOCIMMEDIATE, &state) == -1) 
	{
		error (1, errno, "Can't set immediate mode: %s", ifreq.ifr_name);
	}
#if defined (__APPLE__)
	state = false;
	if (ioctl (channel->fd, BIOCSSEESENT, &state) == -1) 
	{
		error (1, errno, "Can't hide outgoing frames: %s", ifreq.ifr_name);
	}
#elif defined (__OpenBSD__)
	state = BPF_DIRECTION_OUT;
	if (ioctl (channel->fd, BIOCSDIRFILT, &state) == -1) 
	{	
		error (0, errno, "Can't hide outgoing frames");
	}
#else
#error "Abandon all hope"
#endif
	timeout.tv_usec = channel->timeout * 1000;
	if (ioctl (channel->fd, BIOCSRTIMEOUT, &timeout) == -1) 
	{
		error (1, errno, "Can't set timeout: %s", ifreq.ifr_name);
	}

#if 1

	gethwaddr (channel->host, channel->name);

#else

	if (ioctl (channel->fd, SIOCGIFADDR, &ifreq) > 0) 
	{
		error (1, errno, "%s", ifreq.ifr_name);
	}
	memcpy (channel->host, ifreq.ifr_ifru.ifru_addr.sa_data, sizeof (channel->host));

#endif

	bpf_program.bf_len = sizeof (bpf_insn)/sizeof (struct bpf_insn);
	bpf_program.bf_insns = bpf_insn;
	bpf_insn [1].k = channel->type;
	bpf_insn [3].k = channel->host [0];
	bpf_insn [5].k = channel->host [1];
	bpf_insn [7].k = channel->host [2];
	bpf_insn [9].k = channel->host [3];
	bpf_insn [11].k = channel->host [4];
	bpf_insn [13].k = channel->host [5];
	if (ioctl (channel->fd, BIOCSETF, &bpf_program) == -1) 
	{
		error (1, errno, "Can't use filter program");
	}

#elif defined (WINPCAP) || defined (LIBPCAP)

	channel->name = getifname (channel->id);
	gethwaddr (channel->host, channel->name);
	channel->socket = pcap_open_live (channel->name, 65536, 0, CHANNEL_PCAP_TIMEOUT, channel->errbuf);
	snprintf ((char *)(channel->name), strlen (channel->name), "nic%d", channel->id);
	if (!channel->socket) 
	{
		error (1, errno, "Can't open adaptor: %s", channel->name);
	}
	bpf_program.bf_len = sizeof (bpf_insn)/sizeof (struct bpf_insn);
	bpf_program.bf_insns = bpf_insn;
	bpf_insn [1].k = channel->type;
	bpf_insn [3].k = channel->host [0];
	bpf_insn [5].k = channel->host [1];
	bpf_insn [7].k = channel->host [2];
	bpf_insn [9].k = channel->host [3];
	bpf_insn [11].k = channel->host [4];
	bpf_insn [13].k = channel->host [5];
	if (pcap_setfilter (channel->socket, &bpf_program) < 0) 
	{
		error (1, errno, "Can't install pcap filter: %s", channel->name);
	}
	if (pcap_setmintocopy (channel->socket, ETHER_MAX_LEN) < 0) 
	{
		error (1, errno, "Can't set pcap mintocopy: %s", channel->name);
	}

#else
#error "Unknown Environment"
#endif
#endif

	return (0);
}

#endif

