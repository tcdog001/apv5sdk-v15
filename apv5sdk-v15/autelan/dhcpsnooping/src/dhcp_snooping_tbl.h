/**********************************************************************************
* Copyright (c) 2008-2011  Beijing Autelan Technology Co. Ltd.
* All rights reserved.
*
* filename: dhcp_snooping_tbl.h
* description:   the header file for dhcp_snooping table item
* 
*
* 
************************************************************************************/
#ifndef _DHCP_SNOOPING_TBL_H
#define _DHCP_SNOOPING_TBL_H


#define DHCP_SNOOPING_HASH_TABLE_SIZE  256
#define MAX_BOOTP_MAC_LEN              6

typedef enum dhcp_snoop_bind_state {
  DHCP_SNOOPING_BIND_STATE_REQUEST         = 0,
  DHCP_SNOOPING_BIND_STATE_BOUND           = 1,
} dhcp_snoop_bind_state_t;

typedef enum dhcp_snoop_bind_type {
  DHCP_SNOOPING_BIND_TYPE_DYNAMIC         = 0,
  DHCP_SNOOPING_BIND_TYPE_STATIC          = 1,
} dhcp_snoop_bind_type_t;

typedef struct dhcp_snooping_user_item {
    u_int8_t           bind_type;
    u_int8_t           state;
    u_int8_t           haddr_len;
    u_int8_t           chaddr[MAX_BOOTP_MAC_LEN];
    u_int16_t          vlanId;
    u_int32_t          ip_addr;
    u_int32_t          lease_time;
    u_int32_t          sys_escape; /*添加绑定表项时系统启动以来所过的时间 */
    u_int32_t          cur_expire;     /* 当前使用的有效的IP地址状态超时时间,仅显示时使用*/
    u_int32_t          flags;
    u_int32_t          ifindex;
}dhcp_snooping_user_item_t;

typedef struct dhcp_snooping_tbl_item {
    struct dhcp_snooping_tbl_item *next;
    u_int8_t           bind_type;
    u_int8_t           state;
    u_int8_t           haddr_len;
    u_int8_t           chaddr[MAX_BOOTP_MAC_LEN];
    u_int16_t          vlanId;
    u_int32_t          ip_addr;
    u_int32_t          lease_time;
    u_int32_t          sys_escape;
    u_int32_t          cur_expire;
    u_int32_t          ifindex;
    u_int32_t          flags;
    /*struct timer       tm_expire;*/	
}dhcp_snooping_tbl_item_t;

#endif /*_DHCP_SNOOPING_TBL_H*/
