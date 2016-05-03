/**********************************************************************************
* Copyright (c) 2008-2011  Beijing Autelan Technology Co. Ltd.
* All rights reserved.
*
* filename: dhcp_snooping_if.h
* description:  implementation for the header file for dhcp_snooping
* 
*
* 
************************************************************************************/
#ifndef _DHCP_SNOOPING_IF_H
#define _DHCP_SNOOPING_IF_H

#define _PATH_PROCNET_DEV "/proc/net/dev"

struct interface {
    struct interface *next, *prev; 
    char name[IFNAMSIZ];	/* interface name        */
    short type;			/* if type               */
    short flags;		/* various flags         */
    int metric;			/* routing metric        */
    int mtu;			/* MTU value             */
    int tx_queue_len;		/* transmit queue length */
    struct ifmap map;		/* hardware setup        */
    struct sockaddr addr;	/* IP address            */
    struct sockaddr dstaddr;	/* P-P IP address        */
    struct sockaddr broadaddr;	/* IP broadcast address  */
    struct sockaddr netmask;	/* IP network mask       */
    struct sockaddr ipxaddr_bb;	/* IPX network address   */
    struct sockaddr ipxaddr_sn;	/* IPX network address   */
    struct sockaddr ipxaddr_e3;	/* IPX network address   */
    struct sockaddr ipxaddr_e2;	/* IPX network address   */
    struct sockaddr ddpaddr;	/* Appletalk DDP address */
    struct sockaddr ecaddr;	/* Econet address        */
    int has_ip;
    int has_ipx_bb;
    int has_ipx_sn;
    int has_ipx_e3;
    int has_ipx_e2;
    int has_ax25;
    int has_ddp;
    int has_econet;
    char hwaddr[32];		/* HW address            */
    int statistics_valid;
    int keepalive;		/* keepalive value for SLIP */
    int outfill;		/* outfill value for SLIP */
	int ifindex;
};

#endif //_DHCP_SNOOPING_IF_H

