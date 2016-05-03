/*====================================================================*
 *
 *   pcapdevs.c - enumerate ethernet devices on stdout;
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
 *	Charles Maier <charles.maier@intellon.com>
 *
 *--------------------------------------------------------------------*/

#define _GETOPT_H

/*====================================================================*
 *   system header files;
 *--------------------------------------------------------------------*/

#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <pcap.h>

#if defined (__linux__)
#elif defined (__APPLE__)
#elif defined (__OpenBSD__)
#	include <sys/types.h>
#	include <sys/socket.h>
#	include <net/if.h>
#	include <netinet/in.h>
#	include <netinet/if_ether.h>
#elif defined (WINPCAP) || defined (LIBPCAP)
#else
#error "Unknown environment"
#endif

/*====================================================================*
 *   custom header files;
 *--------------------------------------------------------------------*/

#include "../tools/getoptv.h"
#include "../tools/putoptv.h"
#include "../tools/version.h"
#include "../tools/memory.h"
#include "../tools/flags.h"
#include "../tools/types.h"
#include "../tools/error.h"
#include "../ether/ether.h"

/*====================================================================*
 *   custom source files;
 *--------------------------------------------------------------------*/

#ifndef MAKEFILE
#include "../tools/getoptv.c"
#include "../tools/putoptv.c"
#include "../tools/version.c"
#include "../tools/error.c"
#include "../tools/hexdecode.c"
#endif

#ifndef MAKEFILE
#include "../ether/gethwaddr.c"
#endif

/*====================================================================*
 *   program constants;
 *--------------------------------------------------------------------*/

#define PCAP_VERBOSE (1 << 0)
#define PCAP_SILENCE (1 << 1)
#define PCAP_DEVICES (1 << 2)
#define PCAP_NICS (1 << 3)
#define PCAP_MACS (1 << 4)

/*====================================================================*
 *
 *   void pcap_enum (flag_t flags);
 *   
 *   pcap_enum available pcap devices on stdout;
 *
 *.  Intellon INT6000 Linux Toolkit for HomePlug AV;
 *:  Published 2006-2008 by Intellon Corp. ALL RIGHTS RESERVED;
 *;  For demonstration; Not for production use;
 *
 *   Contributor(s):
 *      Nathan Houghton <nathan.houghton@intellon.com>
 *      
 *--------------------------------------------------------------------*/

void pcap_enum (flag_t flags) 

{
	char report [PCAP_ERRBUF_SIZE];
	char string [ETHER_ADDR_LEN * 3];
	byte number [ETHER_ADDR_LEN];
	pcap_if_t * device;
	pcap_if_t * devices = (pcap_if_t *)(0);
	unsigned index;
	if (pcap_findalldevs (&devices, report) == -1) 
	{
		error (1, 0, "Can't enumerate interfaces");
	}
	if (!devices) 
	{
		error (1, 0, "No interfaces available");
	}
	if (_anyset (flags, PCAP_DEVICES)) 
	{
		for (device = devices, index = 1; device; device = device->next, index++) 
		{
			gethwaddr (number, device->name);
			hexdecode (number, sizeof (number), string, sizeof (string));
			printf ("%2d %s %s", index, string, device->name);
			if (device->description) 
			{
				printf ("\t(%s)", device->description);
			}
			printf ("\n");
		}
	}
	if (_anyset (flags, PCAP_NICS)) 
	{
		for (device = devices, index = 1; device; device = device->next, index++) 
		{

#if defined (WIN32)

			printf ("ETH%d=%d", index, index);

#else

			printf ("ETH%d=%s", index, device->name);

#endif

			if (device->description) 
			{
				printf ("\t# %s", device->description);
			}
			printf ("\n");
		}
		printf ("\n");
	}
	if (_anyset (flags, PCAP_MACS)) 
	{
		for (device = devices, index = 1; device; device = device->next, index++) 
		{
			gethwaddr (number, device->name);
			hexdecode (number, sizeof (number), string, sizeof (string));
			printf ("NIC%d=%s", index, string);
			if (device->description) 
			{
				printf ("\t# %s", device->description);
			}
			printf ("\n");
		}
		printf ("\n");
	}
	pcap_freealldevs (devices);
	return;
}

/*====================================================================*
 *   
 *   int main (int argc, const char * argv []) 
 *
 *.  Intellon INT6000 Linux Toolkit for HomePlug AV;
 *:  Published 2006-2008 by Intellon Corp. ALL RIGHTS RESERVED;
 *;  For demonstration; Not for production use;
 *
 *--------------------------------------------------------------------*/

int main (int argc, const char * argv []) 

{
	static const char *optv [] = 
	{
		"hqv",
		"",
		"enumerate available pcap devices on stdout",
		"h\tprint host definitions for scripting",
		"q\tquiet",
		"v\tverbose messages",
		(const char *) (0) 
	};
	flag_t flags = PCAP_DEVICES;
	signed c;
	optind = 1;
	opterr = 1;
	optmin = 0;
	while ((c = getoptv (argc, argv, optv)) != -1) 
	{
		switch ((char) (c)) 
		{
		case 'h':
			_clrbits (flags, (PCAP_DEVICES));
			_setbits (flags, (PCAP_NICS | PCAP_MACS));
			break;
		case 'q':
			_setbits (flags, PCAP_SILENCE);
			break;
		case 'v':
			_setbits (flags, PCAP_VERBOSE);
			break;
		default:
			break;
		}
	}
	argc -= optind;
	argv += optind;
	if (argc > 0) 
	{
		error (1, 0, "Too many arguments");
	}
	pcap_enum (flags);
	return (0);
}

