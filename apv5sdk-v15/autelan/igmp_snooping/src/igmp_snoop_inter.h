/*********************************************************************************************
*			Copyright(c), 2008, Autelan Technology Co.,Ltd.
*						All Rights Reserved
*
**********************************************************************************************
$RCSfile:igmp_snoop_inter.h
$Author: Rock
$Revision: 1.00
$Date:2008-3-8 10:07
***********************************************************************************************/
/****************************This file is inter H file*************************************************/
#ifndef __IGMP_SNOOP_INTER_H__
#define __IGMP_SNOOP_INTER_H__

#ifdef __cplusplus
extern "C"
{
#endif
#include "igmp_snoop.h"

/*big end*/
#ifndef __BIG_ENDIAN_BITFIELD
#define __BIG_ENDIAN_BITFIELD
#endif
/*  PIM Packet header */
typedef struct pim_header
{
#ifdef __BIG_ENDIAN_BITFIELD
	UCHAR	pim_vers:4, pim_type:  4;	/* PIM message version/type*/
#else
	UCHAR	pim_type:4, pim_vers:  4;
#endif
    	UCHAR		pim_reserved;	/* Reserved			*/
    	USHORT	pim_cksum;		/* IP checksum		*/
}pim_header_t;


/**************************** multicast port state structure *****************************/
struct MC_port_state
{
	struct MC_port_state *next;	/*next port*/
	ULONG ifindex;			/*index*/
	ULONG membertimer_id;	/*timer id*/
	ULONG hosttimer_id;		/*IGMP V1 timer ID*/
	UCHAR state;				/*state*/
};

typedef struct igmp_router_entry
{
	struct igmp_router_entry *next;
	ULONG mroute_ifindex;	/*mc router port ifindex*/
	ULONG saddr;				/*mc router ip*/
	ULONG timer_id;			/*mc router port timeout */
}igmp_router_entry;

/************************************* multicast group struct ***********************************/
typedef struct MC_group
{
	struct MC_group	*next;
	struct MC_group	*prev;
	ULONG	MC_ipadd;			/*mc group ip*/
	ULONG	report_ipadd;		/*latest report ip*/
	LONG	vid;				/*VLAN ID*/
	
	USHORT	ver_flag;			/*IGMP version 1-v1, 0-v2*/
	ULONG	lifetime;			/*mc group lift time interval*/
	ULONG	lifetimer_id;		/*mc group lift timer id*/
	ULONG	resposetime;		/*report repose time interval, when timeout, send report*/
	ULONG	resposetime_id;		/*timer id*/
	
	struct	MC_port_state *portstatelist;	/*port state list*/
	ULONG	sedquerytime;		/*resend specific query packet time interval*/
	ULONG	memberinterval;		/*member ship interval*/
	ULONG	queryresposetime;	/*query resposetime interval*/
	ULONG	router_reporttimer;	/*mc router port timer id*/
}MC_group;

/**************************************** multicast group vlan structure*****************************/
typedef struct MC_group_vlan
{
	INT next;
	INT prev;
	LONG	vlan_id;
	MC_group	*firstgroup;		/*first group*/
	
	ULONG	saddr;			/*last source ip address which send general query packet*/
	ULONG	querytimeinterval;	/*query packet timeout interval*/
	ULONG	querytimer_id;		/*query timeout id*/
	ULONG	vlanlife;			/*vlan life timeout*/
	struct igmp_router_entry	*routerlist;	/*mc route port list*/
}MC_group_vlan;

/*************************************** IGMP snoop pkt structure ******************************/
typedef struct igmp_snoop_pkt
{
	USHORT	type;		/*msg type*/
	ULONG	ifindex;		/*port index*/
	LONG	vlan_id;		/*vlan index*/
	LONG	group_id;	/*group vlan index*/
	ULONG	saddr;		/*source ip*/
	ULONG	groupadd;	/*group ip*/
	USHORT	len;			/*skbuff len*/
	USHORT	retranscnt;	/*retransmit cnt*/
	ULONG	action;
}igmp_snoop_pkt;

typedef struct igmp_routerport
{
	struct igmp_routerport *next;
	LONG	vlan_id;
	ULONG	ifindex;
	USHORT	rev;	/*reserved*/
}igmp_routerport;

typedef struct igmp_queryport
{
	struct igmp_queryport *next;
	ULONG	ifindex;
}igmp_queryport;

typedef struct member_port_list
{
	struct member_port_list *next;
	ULONG	ifindex;
}member_port_list;

typedef struct igmp_reporter_port
{
	struct igmp_reporter_port	*next;
	LONG	vlan_id;
	member_port_list *portlist;
}igmp_reporter_port;

typedef struct igmp_vlan_port_list
{
	struct igmp_vlan_port_list	*next;
	ULONG	ifindex;
	ULONG	trunkflag;	/*is trunk: (=0 )---no trunck		(!=0) ---trunckifindex*/
}igmp_vlan_port_list;

typedef struct igmp_vlan_list
{
	struct igmp_vlan_list	*next;
	ULONG	vlan_id;
	igmp_vlan_port_list		*first_port;
}igmp_vlan_list;


typedef struct _l2mc_list
{
	struct _l2mc_list *next;
	struct _l2mc_list *prev;
	ULONG ulGroup;        
	ULONG ulVlanId;    
	 ULONG ulVlanIdx; 
	ULONG ulIndex;        
}l2mc_list;

typedef struct _l2mc_list_head
{
	struct _l2mc_list *stListHead;
	ULONG ulListNodeCount;
}l2mc_list_head;
/****************************************** Macro ********************************************/
#ifndef IPPROTO_IGMP
#define IPPROTO_IGMP 	2		/*IGMP protocol type*/
#endif
#ifndef IPPROTO_PIM
#define IPPROTO_PIM		103
#endif
#ifndef IPVERSION
#define IPVERSION				4
#endif


#define	IGMP_SNP_MSG_LEN	2048
#define IGMP_SNP_GRP_MAX	1023	/*max vlan count*/

/****************IGMP Packet Type*****************/
#define IGMP_MEMSHIP_QUERY	0x11	/*query*/
#define IGMP_V1_MEMSHIP_REPORT	0x12	/*V1 report*/
#define IGMP_V2_MEMSHIP_REPORT	0x16	/*V2 report*/
#define IGMP_V2_LEAVE_GROUP		0x17	/*V2 leave*/

/****************IGMP Group State*****************/
#define IGMP_SNP_GROUP_NOMEMBER		0x00	/*no member*/
#define IGMP_SNP_CHECK_MEMBER		0x01	/*checking member*/
#define IGMP_SNP_HAS_MEMBER			0x02	/*has member*/
#define IGMP_SNP_V1_MEMBER			0x04	/*has IGMP V1 member*/
#define IGMP_SNP_TEMP_MEMBER		0x08	/*send query temporary state*/

/**************** Timer Default Value ************/
#define	IGMP_ROBUST_VARIABLE		2		/*Robustness variable*/
#define	IGMP_V2_QUERY_INTERVAL		125
#define	IGMP_V2_QUERY_RESP_INTERVAL	10
#define	IGMP_V2_GROUP_QUERY_INTERVAL	1
#define IGMP_GROUP_LIFETIME			(2 * IGMP_V2_QUERY_INTERVAL)
#define	IGMP_V1_ROUTER_PRESENT_TIMEOUT	400
#define IGMP_V2_LEAVE_LATENCY		1
#define IGMP_VLAN_LIFE_FACTOR		100

#define	IGMP_V2_TIME_SCALE			100
#define IGMP_V2_SEC_2_MILL			(1000/IGMP_V2_TIME_SCALE)
#define IGMP_SNP_TICK				1000

#define IGMP_ALL_SYSTEM_ADDR	0xE0000001
#define IGMP_ALL_ROUTER_ADDR	0xE0000002

#define	IGMP_PROTOCOL	2
#define IGMP_TOS		0
#define	IGMP_TTL		1

#define IGMP_ROUTER_ALERT_LEN			4 
#define IGMP_SNOOPING_48FE_5616_PORT_BEGIN      8 
#define IGMP_SNOOPING_24FMM_5616_PORT_BEGIN     4

#define	MAX_SLOT_NUM	8

#ifndef NULL
#define NULL 0
#endif

#define ETH_HEAD_LEN_UNTAG	14
#define ETH_HEAD_LEN_TAG	18

#define IGMP_ON		1
#define IGMP_SNOOP_V1	1

#define IGMP_SNOOP_NO	0
#define IGMP_SNOOP_YES	1

#define IGMP_SNOOP_OK	0
#define IGMP_SNOOP_ERR	1
#define IGMP_SNOOP_RETURN	2	/*used in command handle*/
#define IGMP_SNOOP_DUMP	3	/*used in command handle*/
/*******macro function*********/
#define IGMP_SNP_DEBUG(format,arg...) if(igmp_snoop_debug){	printf(format,##arg);}
#define IGMP_SNP_ISENABLE()	(igmp_snoop_enable)
#define GET_VLAN_POINT_BY_INDEX(index)	\
	((((index) >=(0))&&((index) < (IGMP_GENERAL_GUERY_MAX)))?(mcgroup_vlan_queue[index]):(NULL))
#define CLEAR_VLAN_POINT_BY_INDEX(index)	\
{	\
	if(((index) >=(0))&&((index) < (IGMP_GENERAL_GUERY_MAX))	)\
		mcgroup_vlan_queue[index ] = 0;	\
}
/********************* Constant *************************/
#define IGMP_SNP_TASK_PRIORITY		150
#define ETH_FRAME_LEN_MIN			64
#define MAC_ADDR_SIZE				6
#define	IGMP_SNOOP_STR				"Igmp_Snooping"
#define IGMP_TASK_DELAY_TICK		2

/******************** Packet Type ***********************/
enum igmppkttype
{
	IGMP_TYPE_PKT_RECV,			/*receive */
	IGMP_TIMEOUT_TICK,			/*定时器超时：1sec*/
	IGMP_TIMEOUT_GROUP_LIFE,	/*组存活定时器*/
	IGMP_TIMEOUT_GEN_QUERY,		/*query定时器*/
	IGMP_TIMEOUT_GROUP_MEMBERSHIP,	/*组成员定时器*/
	IGMP_TIMEOUT_V1_HOST,		/*IGMP V1主机定时器*/
	IGMP_TIMEOUT_RXMT,			/*IGMP 转发定时器*/
	IGMP_TIMEOUT_RESP,			/*Switch响应多播路由器的query*/
	IGMP_TIMEOUT_ROUTER,		/*路由定时器*/
	IGMP_MSG_GEN_QUERY,			/*发送IGMP v2 普通query*/
	IGMP_MSG_GS_QUERY,			/*发送IGMP v2 特定query*/
	IGMP_MSG_REPORT,			/*发送IGMP v2 report*/
	IGMP_MSG_LEAVE,				/*发送IGMP v2 leave*/
	IGMP_MSG_V1_REPORT,			/*发送IGMP v1 report*/
	IGMP_VLAN_DEL_PORT,			/*VLAN删除端口*/
	IGMP_PORT_DOWN,				/*端口down*/
	IGMP_VLAN_DEL_VLAN,
	IGMP_INVALID				/*Invalid*/
};

typedef enum igmpportstate
{
	IGMP_PORT_ADD,
	IGMP_PORT_DEL,
	IGMP_PORT_QUERY
}igmpportstate;



/*直接拷贝过来，还没有修改*/
#define AWMC_IPMC_BASE				0x6E00	/*IP多播服务类消息码分区*/
#define AWMC_IPMC_PUBLIC_BASE		0x6E00	/*IP多播服务类公共消息码分区*/
#define AWMC_IPMC_PRIVATE_BASE		0x6F00	/*IP多播服务类私有消息码分区*/

#define AWMC_CLI_BASE             0xc000   /*CLI模块定义*/
#define AWMC_CLI_PUBLIC_BASE     0xc000   /*CLI模块公共消息定义*/
#define AWMC_CLI_PRIVATE_BASE    0xc100  /*CLI模块私有消息定义*/
enum enAWMsgCode_IPMC
{
    /***********************************************************************
    * IPMC公共消息码
    ***********************************************************************/
    
    AWMC_IPMC_PUBLIC_BEGIN = AWMC_IPMC_PUBLIC_BASE,
    AWMC_IPMC_PUBLIC_CHANGE_MFC,
    AWMC_IPMC_PUBLIC_DELETE_MFC,
    AWMC_IPMC_PUBLIC_WRONGIIF_TOCPU,
    AWMC_IPMC_PUBLIC_UNKNOWMD_TOCPU,
    
    AWMC_IPMC_PUBLIC_END,
    /*======================================================================*\
    * IPMC私有消息码  		                                                *
    \*======================================================================*/
    AWMC_IPMC_PRIVATE_BEGIN = AWMC_IPMC_PRIVATE_BASE,
    
    #ifdef _SWITCH_PLATFORM2_					/* Add by Lideer 03/01/2003*/
    AWMC_IPMC_PRIVATE_SNOOP_MSG_RECV,				
    AWMC_IPMC_PRIVATE_SNOOP_TIMER_EXPIRE,
    AWMC_IPMC_PRIVATE_SNOOP_HWMOD,
    AWMC_IPMC_TRUNK_SNOOP_UPDATE,
    AWMC_IPMC_SNOOP_HOT_INSERT,
    #endif
    AWMC_IPMC_IGMP_VLANPORT,
    AWMC_RTM_PUBLIC_BGP_MDT,
    AWMC_IPMC_PUBLIC_PIM_ENABLE,/* PIM任务启动时通知其它协议 */
    AWMC_IPMC_PUBLIC_PIM_DISABLE,/* PIM任务停止时通知其它协议 */
    AWMC_IPMC_PUBLIC_PIM_QUERY_BGP_MDT_REFRESH,/* PIM要求BGP刷新特定MDT的路由 */
    AWMC_IP_ROUTE_BGP_PUBLIC_MVPN_UPDATE,/* BGP MVPN Update 通知PIM */
    AWMC_IP_ROUTE_BGP_PUBLIC_MVPN_WITHDRAW,/* BGP MVPN Withdraw 通知PIM */
    AWMC_IP_ROUTE_BGP_PUBLIC_DISABLE,	/* BGP任务退出或clear ip bgp */
    AWMC_VRF_MDT_DEFAULT,	
    AWMC_IPMC_PRIVATE_END
};

/***********************************functions declare*****************************************/
/*************************************igmp_snoop_main.c*************************************/
extern INT igmp_enable_init( VOID );
extern INT igmp_snp_stop( VOID );
extern LONG Igmp_Event_DelVlan( igmp_snoop_pkt * pPkt );
extern LONG Igmp_Event_VlanDelPort( igmp_snoop_pkt * pPkt );
extern LONG Igmp_Event_PortDown( igmp_snoop_pkt * pPkt );
extern LONG igmp_searchvlangroup( LONG lVid, ULONG ulGroup,
			MC_group **ppMcGroup,
			MC_group_vlan **ppPrevGroupVlan );
extern LONG igmp_snp_searchportlist( struct MC_port_state ** ppPortList,
					ULONG usIfIndex, igmpportstate enFlag,
					struct MC_port_state **ppPortState );
extern LONG igmp_snp_searchreporterlist( LONG lVid, ULONG ulIfIndex, igmpportstate enFlag,
					igmp_reporter_port **ppReporter );
extern LONG igmp_snoop_searchrouterlist( LONG lVid, ULONG usIfIndex, igmpportstate enFlag,
									igmp_routerport **ppRouter );
extern LONG igmp_snp_delvlan( LONG vlan_id );
extern LONG igmp_snp_delgroup( MC_group_vlan * pVlan, 
							MC_group * pGroup, MC_group **ppNextGroup );
/*************************************igmp_snoop_cli.c***************************************/
extern void *create_msg_thread(void);
extern void read_config(char *line);
extern int creat_config_file(char *path);
/*************************************igmp_snoop_hw.c**************************************/
extern LONG igmp_snp_mod_addr( igmp_snoop_pkt * pPkt, ULONG ulArg );
extern INT igmp_vlanportrelation(ULONG ifindex,LONG vlan_id,ULONG *targged);
extern INT igmp_is_vlan_ifindex(LONG vlan_id, ULONG ifindex);
extern INT igmp_getvlan_addr(ULONG vlan_id, ULONG *vlan_addr);
extern void igmp_getifstatus(ULONG ifindex,ULONG *portstate);
extern void igmp_getmaxgroupcnt(ULONG ifindex,ULONG *maxgroupcnt);
extern int igmp_getifindex_byvlanid(LONG vlan_id,ULONG *first_ifindex);
extern ULONG igmp_get_nextifindex_byifindex(LONG vlan_id, ULONG ifindex);
extern LONG igmp_snp_device_event( ULONG event, dev_notify_msg * ptr );
extern LONG igmp_vlanisused( LONG vlan_id, ULONG * ret );
extern int IFM_PhyIsMerged2TrunkApi(LONG vlan_id,ULONG ifindex,ULONG *trunkifindex);
extern LONG Igmp_Snoop_IF_Recv_Is_Report_Discard( LONG lVid , ULONG ulIfIndex );
#ifdef __cplusplus
}
#endif

#endif
