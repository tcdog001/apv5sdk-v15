/**********************************************************************************
* Copyright (c) 2008-2011  Beijing Autelan Technology Co. Ltd.
* All rights reserved.
*
* filename: wapid.c
* description:  implementation for socket information communicating with the driver. 
* 
*
* 
************************************************************************************/

#include <linux/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <linux/netlink.h>
#define COMMTYPE_GROUP	8
#define BUF_LEN	3000

typedef unsigned char u8;
typedef unsigned short u16;

struct asso_mt_t
{
	struct nlmsghdr hdr;		/**/
	u16		type;		/*消息类型*/
	u16		data_len;		/*消息长度*/
	u8 		wifiFlag;  /*无线网卡标记，wifi0:1 , wifi1:2 */
	u8 		users;     /*无线网卡上所接的用户数*/
	u8      reserve1;   /*保留*/
	u8      reserve2;   /*保留*/
};
typedef struct asso_mt_t asso_mt;

int netlink_raw_sk=-1;
int socket_open_for_netlink();
void snmpd_process_for_driver();
int  send_trap_users(asso_mt *p) ;
void close_netlink_sock();


/*创建接收Driver信息的套接字*/
int socket_open_for_netlink()
{
	int sock = -1; 
	int bindsock = -1;
	struct sockaddr_nl nlskaddr;
	
	sock = socket(AF_NETLINK,SOCK_RAW,NETLINK_USERSOCK);
	
	if(sock > 0)
	{
		memset ( &nlskaddr, 0 , sizeof( nlskaddr ));
		nlskaddr.nl_family = (sa_family_t)AF_NETLINK;
		nlskaddr.nl_pid = (__u32)getpid();
		nlskaddr.nl_groups = COMMTYPE_GROUP;
	}
	else    return -1;
	
	bindsock = bind(sock,(struct sockaddr *)&nlskaddr,sizeof(nlskaddr));
	
	if(bindsock!=0)
	{
		printf("bind failure!\n");
		return -1;
    }
	return sock;
}

void close_netlink_sock()
{
		close(netlink_raw_sk );
}

const u16 SNMP_USERS = 0x00E1; /*接收类型定义，用户数*/
void snmpd_process_for_driver()
{
	  u8 recv_buf[BUF_LEN] = {0,};
    /*** receive Data from netlink_raw_sk *****/ 
		asso_mt *asso_mt_info ;
		u16 ret=0;
		int readlen=-1;
		readlen = recvfrom(netlink_raw_sk, recv_buf, BUF_LEN, 0, NULL, NULL);

		if(readlen >0)
		{		    
				asso_mt_info = (asso_mt *)recv_buf;
				ret=asso_mt_info->type;
				if( (ret==SNMP_USERS) && (asso_mt_info->data_len==4) ){
				  send_trap_users(asso_mt_info);
			  }
			  else{
			      printf("reverve error from driver in snmpd_process_for_driver().");	
			  }
	  }

}

int  send_trap_users(asso_mt *p)
{

    char trapStr[128]="users trap: ";
    char tmpstr[10];
    char ap_parameter[30];
    AAT aat;
    time_t nowTimes;
    int  ret=-1;
    bzero(ap_parameter,30);
    bzero(tmpstr,10);
    memset(&aat,0,sizeof(aat));
    time(&nowTimes);
    
    sprintf(tmpstr,"%d",p->users);
    strcat(trapStr,tmpstr);
    
    ret=p->wifiFlag  ;  
    if(ret==1){
		set_alarm( & aat,ALARM_ID_WIFI0_USER_NUM,trapStr);           
        send_trap_string_notification(&aat);
    }
    
    if(ret==2){
		set_alarm( & aat,ALARM_ID_WIFI1_USER_NUM,trapStr);           
        send_trap_string_notification(&aat);
    }
    else{
    	printf("wifiFlag error!");
    }
	
}

