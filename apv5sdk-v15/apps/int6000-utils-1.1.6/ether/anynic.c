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
 *   signed anynic (char buffer [], size_t length);
 *
 *   find an available host Ethernet interface and copy the name
 *   into the designated buffer; truncate the name if necessary;
 *   return 0 on success or -1 on error;
 *
 *   if the only interface is loopback then -1 is returned since 
 *   lo is of no use;
 *
 *   this function is only available for Linux at this time and
 *   is only used by the CGI device manager;
 *
 *.  Intellon INT6000 Linux Toolkit for HomePlug AV;
 *:  Published 2006-2009 by Intellon Corp. ALL RIGHTS RESERVED;
 *;  For demonstration; Not for production use;
 *
 *   Contributor(s):
 *      Charles Maier <charles.maier@intellon.com>
 *
 *--------------------------------------------------------------------*/

#ifndef ANYNIC_SOURCE
#define ANYNIC_SOURCE

#include <sys/ioctl.h>
#include <memory.h>
#include <errno.h>

#include "../ether/ether.h"
#include "../tools/error.h"

signed anynic (char buffer [], size_t length) 

{
	char memory [1024];
	struct ifconf ifc;
	struct ifreq *ifr;
	signed fd;
	memset (buffer, 0, length);
	if ((fd = socket (AF_INET, SOCK_DGRAM, 0)) < 0) 
	{
		error (1, errno, "Can't open socket");
	}
	ifc.ifc_len = sizeof (memory);
	ifc.ifc_buf = memory;
	if (ioctl (fd, SIOCGIFCONF, &ifc) < 0) 
	{
		error (1, errno, "Can't read configuration");
	}
	ifr = ifc.ifc_req;
	if ((length) && (ifc.ifc_len > sizeof (struct ifreq))) 
	{
		memcpy (buffer, (++ifr)->ifr_name, --length);
		return (0);
	}
	return (-1);
}

#endif

