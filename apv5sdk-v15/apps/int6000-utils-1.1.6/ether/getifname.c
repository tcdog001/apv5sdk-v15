/*====================================================================*
 *
 *   char * getifname (signed number);
 *
 *   ether.h
 *
 *   return the PCAP interface name for a given interface number; this
 *   function is only needed when using LIBPCAP or WINPCAP libraries;
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
 *      Nathan Houghton <nathan.houghton@intellon.com>
 *      Charles Maier <charles.maier@intellon.com>
 *      
 *--------------------------------------------------------------------*/

#ifndef GETIFNAME_SOURCE
#define GETIFNAME_SOURCE

#include <string.h>
#include <pcap.h>

#include "../ether/ether.h"
#include "../tools/error.h"

char * getifname (signed index) 

{
	char * name = (char *)(0);
#if defined (WINPCAP) || defined (LIBPCAP)
	char buffer [PCAP_ERRBUF_SIZE];
	pcap_if_t *devices = (pcap_if_t *)(0);
	pcap_if_t *device;
	signed count;
	if (pcap_findalldevs (&devices, buffer) == -1) 
	{
		error (1, errno, "can't enumerate pcap devices");
	}
	for (device = devices, count = 0; device; device = device->next) 
	{
		if (++count == index) 
		{
			name = strdup (device->name);
			break;
		}
	}
	if (!device) 
	{
		error (1, EINVAL, "invalid interface: %d", index);
	}
	pcap_freealldevs (devices);
#endif
	return (name);
}

#endif

