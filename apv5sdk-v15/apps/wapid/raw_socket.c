/*****************************************************************
* Copyright (c) 2001,西安西电捷通无线网络通信有限公司
* All rights reserved.
* 
* 文件名称：raw_socket.c
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

/*network include*/
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/socket.h> 
#include <linux/if.h>
#include <linux/errqueue.h>
#include "raw_socket.h"
#include "debug.h"
#include "auth.h"


/*获得设备的索引号*/
int get_device_index_by_raw_socket(char* dev_name, int sock)
{

	char * device_name = dev_name;
	struct ifreq ifr;
	int g_ifr;
	/* Get device index */
	if (strlen (device_name)>= sizeof (ifr.ifr_name)) {
		return RAW_SOCK_ERR_INVALID_DEV_NAME;
	}
	memcpy (ifr.ifr_name, device_name, strlen (device_name) +1);
	if (ioctl (sock, SIOCGIFINDEX, &ifr) != 0) {
		return RAW_SOCK_ERR_GET_IFR;
	}
	g_ifr = ifr.ifr_ifindex;
	return g_ifr;
}
/*获得设备的MAC地址*/
int get_device_mac_by_raw_socket(char* dev_name, int sock, u8 *mac_out)
{	
	
	char * device_name = dev_name;
	struct ifreq ifr;

	memset(&ifr, 0, sizeof(struct ifreq));

	if (strlen (device_name)>= sizeof (ifr.ifr_name)) {
		return RAW_SOCK_ERR_INVALID_DEV_NAME;
	}
	memcpy (ifr.ifr_name, device_name, strlen (device_name)+1 );

	/* get the dev_name MACADDR */
	if (ioctl (sock, SIOCGIFHWADDR, &ifr) != 0) {
		return RAW_SOCK_ERR_GET_MAC;
	}
	memcpy(mac_out, ifr.ifr_hwaddr.sa_data, 6);
	return 0;
}
/*获得设备的MTU值*/
int get_device_mtu_by_raw_socket(char* dev_name, int sock, u16 *mtu_out)
{	
	
	char * device_name = dev_name;
	struct ifreq ifr;
	
	memset(&ifr, 0, sizeof(struct ifreq));

	if (strlen (device_name)>= sizeof (ifr.ifr_name)) {
		return RAW_SOCK_ERR_INVALID_DEV_NAME;
	}
	memcpy (ifr.ifr_name, device_name, strlen (device_name)+1);

	/* get the dev_name MACADDR */
	if (ioctl (sock, SIOCGIFMTU, &ifr) != 0) {
		return RAW_SOCK_ERR_GET_MAC;
	}
	*mtu_out = ifr.ifr_mtu;
	return 0;
}

/*发送以太网原始数据包*/
int send_rs_data(const void *data, int len, struct ethhdr *eh, apdata_info *pap)
{
	int i = 0;
	int  slen = 0;
	int frame_no = 0;
	int frame_len = 0;
	int mtu = pap->mtu;
	u8 packet_type;
	u8 *frame_data = NULL;
	struct sockaddr_ll eth_addr;
	struct wapid_interfaces *wapid ;
	struct eloop_data *tmp_eloop ;

	if(pap == NULL) return -1;
	wapid = (struct wapid_interfaces *)pap->user_data;
	if(wapid == NULL) return -1;
	tmp_eloop = (struct eloop_data *)wapid->eloop_save;
	if(tmp_eloop == NULL) return -1;
	
	packet_type = ((char *)data)[3];
	/*构造以太网头*/
	memset(&eth_addr, 0, sizeof(struct sockaddr_ll));
	eth_addr.sll_family = AF_PACKET;
	eth_addr.sll_protocol = tmp_eloop->g_eth_proto;
	eth_addr.sll_ifindex =pap->g_ifr;
	eth_addr.sll_halen = ETH_ALEN;
	eh->h_proto = tmp_eloop->g_eth_proto;
	memcpy (eh->h_source, pap->macaddr,  ETH_ALEN);
	if(len + ETH_HLEN <= mtu)/*不需要分片*/
	{
		frame_data = malloc(len + ETH_HLEN);
		memcpy(frame_data, eh,  ETH_HLEN);
		memcpy(frame_data + ETH_HLEN, data, len);
		frame_len = len + ETH_HLEN;
		slen = sendto(tmp_eloop->wai_raw_sk, frame_data, frame_len, 0, (struct sockaddr*)&eth_addr, sizeof(eth_addr));
		slen -= ETH_HLEN;
		if (slen <= 0) {
			return RAW_SOCK_ERR_SEND;
		}
	}else{
		/*分片处理*/
		
		if(((len - sizeof(packet_head))%(mtu - sizeof(packet_head) -ETH_HLEN ))!=0)
			frame_no = (len - sizeof(packet_head))/(mtu - sizeof(packet_head) -ETH_HLEN ) + 1;
		else			
			frame_no = (len - sizeof(packet_head))/(mtu - sizeof(packet_head) -ETH_HLEN );
		
		frame_data = malloc(frame_no * mtu);
		memset(frame_data , 0x11, sizeof(frame_no * mtu));
		for(i = 0; i < frame_no; i++)
		{	
			int send_frame_len =0;
			u8 * ethernet_header = frame_data + i * mtu;
			packet_head *wai_frame_header = (packet_head *)( ethernet_header + ETH_HLEN );
			/*set ethernet header*/
			memcpy(ethernet_header, eh,  ETH_HLEN);
			/*Copy WAI header*/
			memcpy(wai_frame_header , data, sizeof(packet_head));
			/*set frame sequence*/
			wai_frame_header->frame_sc = i;
			if(i != frame_no -1) /*not the last frame*/
			{	
				/*set frame flag*/
				wai_frame_header->flag |= 0x01;
				/*Copy WAI data*/
				memcpy((u8 *)wai_frame_header + sizeof(packet_head), 
					data + sizeof(packet_head)+ i* (mtu - sizeof(packet_head) -ETH_HLEN ),
					mtu - sizeof(packet_head) -ETH_HLEN );
				/*set WAI len*/
				wai_frame_header->data_len =htons(mtu -ETH_HLEN) ;
				/*set send data len*/
				frame_len = mtu; 
			}else{
				/*the last frame*/
				u16 temp_len = 0;
				memcpy((u8 *)wai_frame_header + sizeof(packet_head), 
					data + sizeof(packet_head)+ i* (mtu - sizeof(packet_head) -ETH_HLEN ),
					len - sizeof(packet_head) -i* (mtu - sizeof(packet_head) -ETH_HLEN )) ;
				/*set WAI len*/
				temp_len = len  -i* (mtu - sizeof(packet_head) -ETH_HLEN );
				wai_frame_header->data_len =htons(temp_len) ;
				/*set send data len*/
				frame_len =temp_len + ETH_HLEN ; 
			}
			send_frame_len = sendto(tmp_eloop->wai_raw_sk, ethernet_header, frame_len, 0, (struct sockaddr*)&eth_addr, sizeof(eth_addr));
			send_frame_len -= ETH_HLEN ;
			DPrintf("[WAPID]:: Send fragment %d to sta; MTU=%d, datalen=%d\n", i, mtu, frame_len);
			if (send_frame_len <= 0) {
				DPrintf("error at %s :%d\n", __func__, __LINE__);
				return RAW_SOCK_ERR_SEND;
			}
			slen = slen + send_frame_len ;/*many headers length*/
		}
		slen -=(frame_no - 1)*sizeof(packet_head) ;
	}
	
	free(frame_data);
	return slen;
}
/*接收数据时的错误处理*/
int handle_recverr(int sock)
{
	//BYTE recverr_buf[BUF_LEN] = {0,};
	
	char *msg = NULL;
	struct msghdr mdr;
	struct iovec iov;
	union{
		struct cmsghdr cmsg;
		char control[CMSG_SPACE(sizeof(struct sock_extended_err))];
	}control_un;
	
	struct cmsghdr *cmsg = NULL;
	int  errdata_len = 0;
	
	msg = (char *)malloc(MAXMSG);
	if(msg == NULL)
	{
		printf("malloc failure\n");
		return -1;
	}
	memset(msg, 0, MAXMSG);
	iov.iov_base = msg;
	iov.iov_len = MAXMSG;
	mdr.msg_name = NULL;
	mdr.msg_namelen = 0;
	mdr.msg_iov = &iov;
	mdr.msg_iovlen = 1;
	mdr.msg_control = &control_un.control;
	mdr.msg_controllen = sizeof(control_un.control);
	
	errdata_len = recvmsg(sock, &mdr, MSG_ERRQUEUE);	

	if(errdata_len < 0) 
	{
		memset(msg, 0, MAXMSG);
		free(msg);
		return -1;
	}
	
	DPrintf("Received %d bytes from\n ", errdata_len); 
	for(	cmsg = CMSG_FIRSTHDR(&mdr); cmsg != NULL; 
		cmsg = CMSG_NXTHDR(&mdr, cmsg))
	{
		if(	(cmsg->cmsg_level == SOL_IP) && 
			(cmsg->cmsg_type == IP_RECVERR))
		{
			struct sock_extended_err  *perr_data = NULL;
			perr_data = (struct sock_extended_err *)CMSG_DATA(cmsg);
			wapi_hex_dump("DUMP sock_extended_err ", perr_data, sizeof(struct sock_extended_err));
			DPrintf("sock_extended_err is %08x\n", perr_data->ee_errno);
			DPrintf("sock_extended_err is %02x\n", perr_data->ee_type);
			DPrintf("sock_extended_err is %02x\n", perr_data->ee_code);
			DPrintf("sock_extended_err is %02x\n", perr_data->ee_origin);
			DPrintf("sock_extended_err is %08x\n", perr_data->ee_info);
			DPrintf("sock_extended_err is %08x\n", perr_data->ee_data);
		}
	}
	memset(msg, 0, MAXMSG);
	free(msg);
	return 0;
	
}

int recv_rs_data(void * data, int buflen, struct ethhdr *_eh, int sk)
{
	int recv_len = 0;

	/*接收数据*/
	recv_len = recvfrom (sk, data, buflen, 0, NULL, NULL);

	/*检查包头*/
	if(recv_len >= ETH_HLEN+12)
	{
		/*copy以太网头*/
		memcpy(_eh, data, ETH_HLEN);
		_eh->h_proto = ntohs (_eh->h_proto);
		recv_len -= ETH_HLEN;
		memmove(data, (char *)data+ETH_HLEN, recv_len);
		return recv_len;

	}
	else if (recv_len <= 0) 
	{
		return RAW_SOCK_ERR_RECV;
	}
	else	if (recv_len <= ETH_HLEN) 
	{
		return RAW_SOCK_ERR_RECV;
	}
	else	if (recv_len <= ETH_HLEN+12/*WAI head*/) 
	{
		printf("WAI msg  too short (%d)\n", recv_len);
		return RAW_SOCK_ERR_RECV;
	}
	return RAW_SOCK_ERR_RECV;
}

