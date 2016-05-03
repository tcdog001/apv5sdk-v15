/*****************************************************************
* Copyright (c) 2001,西安西电捷通无线网络通信有限公司
* All rights reserved.
* 
* 文件名称：raw_socket.h
* 摘    要：使用原始套接口发送和接收数据的相关
			 函数的实现
* 
* 当前版本：1.1
* 作    者：王月辉yhwang@iwncomm.com
* 完成日期：2005年6月10日
*
* 取代版本：1.0 
* 原作者  ：王月辉yhwang@iwncomm.com
* 完成日期：2004年1月10日
******************************************************************/

#ifndef __RAW_SOCKET_H__
#define __RAW_SOCKET_H__
#include <linux/if_packet.h>
#include <linux/if_ether.h>
#include "auth.h"

#define ETH_P_WAPI  0x88B4				/*WAI协议号*/
#define MAXMSG 2048

/*错误码*/
#define RAW_SOCK_ERR_CREATE  -1
#define RAW_SOCK_ERR_INVALID_DEV_NAME -2
#define RAW_SOCK_ERR_GET_IFR  -3
#define RAW_SOCK_ERR_GET_MAC  -4
#define RAW_SOCK_ERR_SELECT   -5
#define RAW_SOCK_ERR_RECV     -6
#define RAW_SOCK_ERR_SEND     -7

int open_raw_socket(int eth_protocol_type);
int get_device_index_by_raw_socket(char* dev_name, int sock);
int get_device_mac_by_raw_socket(char* dev_name, int sock, u8 *mac_out);
int get_device_mtu_by_raw_socket(char* dev_name, int sock, u16 *mtu_out);
int handle_recverr(int sock);

int send_rs_data(const void *data, int len, struct ethhdr *eh, apdata_info *APData);

int recv_rs_data(void * data, int buflen, struct ethhdr *_eh, int sk);

#endif  

