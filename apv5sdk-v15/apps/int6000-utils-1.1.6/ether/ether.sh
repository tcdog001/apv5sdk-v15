#!/bin/sh
# file: ether/ether.sh

# ====================================================================
# programs;
# --------------------------------------------------------------------

gcc -Wall -o pcapdevs pcapdevs.c -lpcap

# ====================================================================
# functions;
# --------------------------------------------------------------------

gcc -Wall -c anynic.c
gcc -Wall -c hostnics.c
gcc -Wall -c channel.c
gcc -Wall -c closechannel.c
gcc -Wall -c getifname.c
gcc -Wall -c gethwaddr.c
gcc -Wall -c openchannel.c
gcc -Wall -c readpacket.c
gcc -Wall -c sendpacket.c

# ====================================================================
# functions;
# --------------------------------------------------------------------

gcc -Wall -c pcap_nametoindex.c
gcc -Wall -c pcap_indextoname.c
gcc -Wall -c pcap_nameindex.c
gcc -Wall -c pcap_freenameindex.c

