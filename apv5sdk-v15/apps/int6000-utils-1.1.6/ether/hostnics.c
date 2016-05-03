/*====================================================================*
 *
 *   unsigned hostnics (struct nic list [], size_t size);
 *   
 *.  Intellon Linux Toolkit; 
 *:  Published 2007 by Intellon Corp. ALL RIGHTS RESERVED;
 *;  For demonstration; Not for production use;
 *
 *   Contributor(s):
 *	Charles Maier <charles.maier@intellon.com>
 *
 *--------------------------------------------------------------------*/

#ifndef HOSTNICS_SOURCE
#define HOSTNICS_SOURCE

#if defined (__linux__)
#	include <net/if.h>
#	include <net/ethernet.h>
#	include <sys/ioctl.h>
#elif defined (__APPLE__)
#	incluse <sys/types.h>
#	include <ifaddrs.h>
#	include <net/if_dl.h>
#endif
#include <memory.h>
#include <errno.h>

#include "../ether/ether.h"
#include "../tools/error.h"

unsigned hostnics (struct nic list [], size_t size) 

{
#if defined (__linux__)

	char buffer [1024];
	struct ifconf ifc;
	struct ifreq * ifr;
	signed count;
	signed fd;
	memset (list, 0, size * sizeof (struct nic));
	if ((fd = socket (AF_INET, SOCK_DGRAM, 0)) < 0) 
	{
		error (1, errno, "Can't open socket");
	}
	ifc.ifc_len = sizeof (buffer);
	ifc.ifc_buf = buffer;
	if (ioctl (fd, SIOCGIFCONF, &ifc) < 0) 
	{
		error (1, errno, "Can't read interface configuration");
	}
	ifr = ifc.ifc_req;
	count = ifc.ifc_len / sizeof (struct ifreq);
	if (count > size) 
	{
		count = size;
	}
	if (count < size) 
	{
		size = count;
	}
	while (count--) 
	{
		memcpy (list [count].name, ifr->ifr_name, IF_NAMESIZE);
		memcpy (list [count].internet, &((struct sockaddr_in *)(&ifr->ifr_addr))->sin_addr, 4);
		if (ioctl (fd, SIOCGIFHWADDR, ifr) < 0) 
		{
			error (1, errno, "Can't read %s hardware address", ifr->ifr_name);
		}
		memcpy (list [count].ethernet, ((struct ether_addr *)(ifr->ifr_hwaddr.sa_data)), ETHER_ADDR_LEN);
		ifr++;
	}
#elif defined (__APPLE__)

	struct ifaddrs *ifaddrs;
	struct ifaddrs *ifaddr;
	signed count = 0;
	if (getifaddrs (&ifaddrs) == -1) 
	{
		error (1, errno, "No interfaces available");
	}
	for (ifaddr = ifaddrs; ifaddr && (count < size); ifaddr = ifaddr->ifa_next) 
	{
		if (!ifaddr->ifa_addr) 
		{
			continue;
		}
		if (ifaddr->ifa_addr->sa_family == AF_LINK) 
		{
			struct sockaddr_dl * sockaddr = (struct sockaddr_dl *) (ifaddr->ifa_addr);
			memcpy (list [count++].ethernet, LLADDR (sockaddr), ETHER_ADDR_LEN);
			continue;
		}
	}
	freeifaddrs (ifaddrs);

#endif
	return (size);
}

/*====================================================================*
 *   test/demo program;
 *--------------------------------------------------------------------*/

#if 0
#include <sys/ioctl.h>
#include <net/if.h>
#include <net/ethernet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <errno.h>
#include <memory.h>

#include "../ether/ether.h"
#include "../ether/hostnics.c"

#include "../tools/hexdecode.c"
#include "../tools/decdecode.c"
#include "../tools/hexstring.c"
#include "../tools/decstring.c"

int main (int argc, const char * argv []) 

{
	char address [ETHER_ADDR_LEN * 3];
	struct nic list [10];
	struct nic * item = list;
	unsigned size = sizeof (list) /sizeof (struct nic);
	size = hostnics (list, size);
	while (size--) 
	{
		printf (" %s", hexstring (address, sizeof (address), item->ethernet, sizeof (item->ethernet)));
		printf (" %s", decstring (address, sizeof (address), item->internet, sizeof (item->internet)));
		printf (" %s", item->name);
		printf ("\n");
		item++;
	}
	return (0);
}

#endif

/*====================================================================*
 *
 *--------------------------------------------------------------------*/

#endif

