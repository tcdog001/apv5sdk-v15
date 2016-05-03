/*********************************************************************************************
*			Copyright(c), 2008, Autelan Technology Co.,Ltd.
*						All Rights Reserved
*
**********************************************************************************************
$RCSfile:igmp_snoop.h
$Author: Rock
$Revision: 1.00
$Date:2008-3-8 10:07
***********************************************************************************************/

#ifndef __IGMP_SNOOP_H__
#define __IGMP_SNOOP_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>
#include <sys/un.h>
#include <sys/stat.h>
#include <linux/if_ether.h>
#include <netinet/ip.h>
#include <netinet/igmp.h>

#define _IGMP_SNOOPING_

#ifndef ULONG
#define ULONG unsigned long
#endif

#ifndef LONG
#define LONG long 
#endif

#ifndef USHORT
#define USHORT unsigned short
#endif

#ifndef UCHAR
#define UCHAR unsigned char
#endif

#ifndef CHAR
#define CHAR char
#endif

#ifndef  SHORT
#define SHORT short
#endif

#ifndef	UINT
#define	UINT	unsigned int
#endif

#ifndef	INT
#define INT		int
#endif

#ifndef	VOID
#define	VOID	void
#endif
#if 0
/* Internet address. */
struct in_addr {
	UINT		s_addr;
};

typedef struct igmp_header
{
	UCHAR	igmp_type;		/*IGMP消息类型*/
	UCHAR	igmp_code;		/*igmp消息代码*/
	USHORT	igmp_cksum;		/*IP checksum*/
	struct in_addr	igmp_group;	/*组地址*/
}igmp_header;
#endif

struct nlmsghdr
{
	UINT		nlmsg_len;	/* Length of message including header */
	USHORT		nlmsg_type;	/* Message content */
	USHORT		nlmsg_flags;	/* Additional flags */
	UINT		nlmsg_seq;	/* Sequence number */
	UINT		nlmsg_pid;	/* Sending process PID */
};

/*IGMP Snoop kernel message type*/
#define	IGMP_SNP_TYPE_MIN				0
#define	IGMP_SNP_TYPE_NOTIFY_MSG		1	/*notify message*/
#define	IGMP_SNP_TYPE_PACKET_MSG		2	/*Packet message*/
#define 	IGMP_SNP_TYPE_DEVICE_EVENT	3	/*device message*/
#define	IGMP_SNP_TYPE_MAX				9

/*IGMP Snoop message flag*/
#define	IGMP_SNP_FLAG_MIN				0
#define	IGMP_SNP_FLAG_PKT_UNKNOWN	1	/*Unknown packet*/
#define	IGMP_SNP_FLAG_PKT_IGMP		2	/*IGMP,PIM packet*/
#define	IGMP_SNP_FLAG_ADDR_MOD		3	/*notify information for modify address*/
#define	IGMP_SNP_FLAG_MAX				9

struct igmp_skb{
		struct nlmsghdr nlh;
		char buf[1024];
} ;

enum igmpmodaddtype
{
	IGMP_ADDR_ADD,
	IGMP_ADDR_DEL,
	IGMP_ADDR_RMV,
	IGMP_SYS_SET,
	IGMP_TRUNK_UPDATE
};

#define IGMP_SYS_SET_INIT	1
#define IGMP_SYS_SET_STOP	2

typedef struct igmp_notify_mod_pkt{
	ULONG	mod_type;
	LONG	vlan_id;
	ULONG	ifindex;
	ULONG	groupadd;
	ULONG	reserve;
}igmp_notify_mod_pkt;

struct igmp_info{
	unsigned int ifindex;
	unsigned int vlan_id;
	struct iphdr *ip_hdr;
	struct igmp *igmp_hdr;
	struct igmp_skb *data;
};

/*Device notify message*/
typedef struct dev_notify_msg
{
	ULONG	event;
	LONG	vlan_id;
	ULONG	ifindex;
	ULONG	reserve;
}dev_notify_msg;
/***********************************device notify event**************************************/
#define EVENT_DEV_UP                	(0x0001)
#define EVENT_DEV_DOWN			(0x0002)
#define EVENT_DEV_REGISTER		(0x0003)
#define EVENT_DEV_UNREGISTER	(0x0004)
#define EVENT_DEV_VLANADDIF	(0x0005)
#define EVENT_DEV_VLANDELIF		(0x0006)


/********************************cmd structure***********************************************/
#if 1
typedef struct
{
	UCHAR	msgtype;			/*message type*/
	ULONG	msgcode;
	ULONG	msgext;				/*reserve*/
	ULONG	msglen;
}cmd_msg;

#define	CMD_MSG_TYPE_BEGIN	0
#define	CMD_MSG_TYPE_CONF	1
#define	CMD_MSG_TYPE_KERNEL	2
#define	CMD_MSG_TYPE_SHOW	3
#define	CMD_MSG_TYPE_NOTIFY	4
#define	CMD_MSG_TYPE_REQUEST	5
#define	CMD_MSG_TYPE_ACK		6

#define	CMD_MSG_TYPE_END		20

#define	CMD_CONF_BEGIN		0
#define	CMD_CONF_ENABLE		1
#define	CMD_CONF_DISABLE		2
#define	CMD_CONF_DEBUG_ON	3
#define	CMD_CONF_DEBUG_OFF	4
#define	CMD_CONF_DEL_SPEC		5
#define	CMD_CONF_DEL_ALL		6
#define	CMD_CONF_SET_QUERYTIME	7
#define	CMD_CONF_SET_ROBUSTTIME	8
#define	CMD_CONF_SET_GROUPLIFE	9
#define	CMD_CONF_SET_VLANLIFE		10
#define	CMD_CONF_ADD_MCROUTE	11
#define	CMD_CONF_DEL_MCROUTE		12

#define	CMD_CONF_END			255

#define	CMD_SHOW_BEGIN			0
#define	CMD_SHOW_GROUPCNT		1	/*show group count*/
#define	CMD_SHOW_HOSTTIMEOUT	2	/*show host time interval*/
#define	CMD_SHOW_QUERYTIMEOUT	3
#define	CMD_SHOW_RESPTIMEOUT	4
#define	CMD_SHOW_ROBUSTTIMEOUT	5
#define	CMD_SHOW_GROUPLIFETIME	6
#define	CMD_SHOW_VLANLIFETIME	7
#define	CMP_SHOW_MCROUTE			8

#define	CMD_SHOW_END				20

#define	CMD_EXT_RETURN_SUCCESS	1	/*command excute return value, success*/
#define	CMD_EXT_RETURN_FAILED	2	/*command excute return value, failed*/

typedef struct cmd_opt_info{
	LONG	vlan_id;
	ULONG	ifindex;
	ULONG	groupadd;
	ULONG	reserve;
}cmd_opt_info;
#endif





#define	IGMP_SNOOP_CONFIG_PATH	"/home/igmp/.config"		/*config file path*/
#define	IGMP_SNOOP_MSG_SOCK		"/home/igmp/msg.sock"		/*messageocket*/
#define	IGMP_SNOOP_SKB_SOCK		"/home/igmp/skb.sock"		/*skb socket*/

#define	IGMP_MSG_MAX_SIZE			1024
#define	IGMP_GENERAL_GUERY_MAX	4096
#ifdef __cplusplus
}
#endif

#endif

