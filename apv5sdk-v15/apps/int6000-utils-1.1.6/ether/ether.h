/*====================================================================*
 *
 *   ether.h - Ethernet definitions and declarations;
 *
 *   include or exclude various ethernet related definitions based 
 *   platform and environment;
 *
 *.  Intellon Linux Toolkit; 
 *:  Published 2007 by Intellon Corp. ALL RIGHTS RESERVED;
 *;  For demonstration; Not for production use;
 *
 *--------------------------------------------------------------------*/

#ifndef ETHER_HEADER
#define ETHER_HEADER

/*====================================================================*
 *   system header files;
 *--------------------------------------------------------------------*/

#include <stdint.h>
#if defined (__linux__)
#	include <net/if.h>
#	include <net/ethernet.h>
#	include <arpa/inet.h>
#elif defined (__APPLE__)
#	include <sys/types.h>
#	include <sys/socket.h>
#	include <net/if.h>
#	include <net/ethernet.h>
#	include <arpa/inet.h>
#	include <net/bpf.h>
#elif defined (__OpenBSD__)
#	include <fcntl.h>
#	include <sys/types.h>
#	include <sys/socket.h>
#	include <net/if.h>
#	include <netinet/in.h>
#	include <netinet/if_ether.h>
#	include <net/bpf.h>
#elif defined (WINPCAP)
#	include <pcap.h>
#	include <pcap/Packet32.h>
#	include <pcap/Ntddndis.h>
#	include <net/ethernet.h>
#	include <net/if.h>
#else
#error "Unknown environment"
#endif

#ifndef ETH_CRC_LEN
#define ETH_CRC_LEN 4 /* normally defined in ethernet.h or if_ether.h */
#endif

/*====================================================================*
 *   custom header files;
 *--------------------------------------------------------------------*/

#include "../tools/types.h"

/*====================================================================*
 *   sort out the raw socket mess;
 *--------------------------------------------------------------------*/

#if defined (__linux__)
#ifdef WINPCAP
#	error "Don't enable winpcap on Linux. It won't work."
#	endif
#ifdef LIBPCAP
#	error "Don't enable libpcap on Linux. You don't need it."
#	endif
#elif defined (__APPLE__) 
#ifdef WINPCAP
#	error "Don't enable winpcap on OSX. It won't work."
#	endif
#ifdef LIBPCAP
#	error "Don't enable libpcap on OSX. You don't need it."
#	endif
#elif defined (__OpenBSD__) 
#ifdef WINPCAP
#	error "Don't enable winpcap on OpenBSD. It won't work."
#	endif
#ifdef LIBPCAP
#	error "Don't enable libpcap on OpenBSD. You don't need it."
#	endif
#elif defined (WINPCAP)
#ifndef WINPCAP
#	error "Define preprocessor constant WINPCAP on Windows."
#	endif
#ifdef LIBPCAP
#	error "Don't enable libpcap on Windows. It won't work."
#	endif
#else
#	error "Unknown Environment"
#endif

/*====================================================================*
 *   variables;
 *--------------------------------------------------------------------*/

struct nic 

{
	char name [IF_NAMESIZE];
	char text [255];
	byte ethernet [ETHER_ADDR_LEN];
	byte internet [4];
}

NIC;

/*====================================================================*
 *   functions;
 *--------------------------------------------------------------------*/

char * getifname (signed number);
signed gethwaddr (void * memory, char const * device);
signed anynic (char buffer [], size_t length);
unsigned hostnics (struct nic list [], size_t size);

/*====================================================================*
 *
 *--------------------------------------------------------------------*/

#endif

