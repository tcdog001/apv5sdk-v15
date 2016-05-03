
/**********************************************************************************
* Copyright (c) 2008-2011  Beijing Autelan Technology Co. Ltd.
* All rights reserved.
*
* filename: dhcp_snooping_pkt.h
* description: the header file for dhcp_snooping dhcpmessage Package
* 
*
* 
************************************************************************************/
#ifndef _DHCP_SNOOPING_PKT_H
#define _DHCP_SNOOPING_PKT_H

#define BOOTREQUEST		1
#define BOOTREPLY		2
#define CLIENT_PORT 68
#define SERVER_PORT 67
#define DHCP_MAGIC		0x63825363

typedef struct dhcpMessage {
	u_int8_t op;
	u_int8_t htype;
	u_int8_t hlen;
	u_int8_t hops;
	u_int32_t xid;
	u_int16_t secs;
	u_int16_t flags;
	u_int32_t ciaddr;
	u_int32_t yiaddr;
	u_int32_t siaddr;
	u_int32_t giaddr;
	u_int8_t chaddr[16];
	u_int8_t sname[64];
	u_int8_t file[128];
	u_int32_t cookie;
	u_int8_t options[308]; /* 312 - cookie */ 
} dhcpMessage_t;

typedef struct udp_dhcp_packet {
	struct ethhdr eth;
	struct iphdr ip;
	struct udphdr udp;
	struct dhcpMessage data;
}udp_dhcp_packet_t;

#endif  /*_DHCP_SNOOPING_PKT_H*/
