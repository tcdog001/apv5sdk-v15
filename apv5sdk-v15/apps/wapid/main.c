/*****************************************************************
* Copyright (c) 2001,西安西电捷通无线网络通信有限公司
* All rights reserved.
* 
* 文件名称：main.c
* 摘    要：主处理函数,完成系统初始化和鉴别过程
* 
* 当前版本：1.1
* 作    者：王月辉yhwang@iwncomm.com
* 完成日期：2005年6月10日
*
* 取代版本：1.0 
* 原作者  ：王月辉yhwang@iwncomm.com
* 完成日期：2004年1月10日
******************************************************************/

/* Standard C library includes */
#include <sys/types.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <math.h>
#include <errno.h>
#include <fcntl.h>
#include <ctype.h>
#include <string.h>
#include <syslog.h>
#include <signal.h>
#include <time.h>

#include <unistd.h>

#include <sys/reboot.h>
#include <sys/file.h>
#include <linux/if_packet.h>
#include <linux/if_ether.h>
//#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

	
#include <getopt.h>

#include "pack.h"
#include "auth.h"
#include "debug.h"
#include "proc.h"
#include "init.h"
#include "raw_socket.h"
#include "cert_auth.h"
#include "wai_sta.h"
#include "certupdate.h"
#include "ctrl_iface.h"
#include <stdlib.h>
#include <linux/netlink.h>
#include <linux/rtnetlink.h>
#ifdef DOT11_WAPI_MIB
#include "dot11wapiMib.h"
#endif

extern int  debug;
struct eloop_data eloop;
extern int errno;

static int wapid_setup_interface(apdata_info *hapd);
char *wai_subtype_info[]={
	"",
	"pre-authentication start ",
	"STAKey request",
	"authentication activation",
	"access authentication request",
	"access authentication response", 
	"certificate authentication request",
	"certificate authentication response", 
	"unicast key negotiation request",
	"unicast key negotiation response",
	"unicast key negotiation confirmation", 
	"multicast key announcement",
	"multicast key announcement response",	
	NULL,
};

#if 0
void wapid_3810_init(struct eloop_data *eloop, int asu_ip, int asu_port)
{
	struct sockaddr_in serv;

	struct sockaddr *addr;
	socklen_t addrlen;
	char abuf[50];
	int sel_sock;
	
	eloop->as_addr.sin_family = AF_INET;
	eloop->as_addr.sin_port = htons((unsigned short)atoi(asu_port));
	
	if(inet_aton(asu_ip, &(eloop.as_addr.sin_addr)) == 0)
	{
		DPrintf("ASU  IP is wrong\n");
		bzero(asu_ip, 20);
		strcpy(asu_ip, "127.0.0.1");
		inet_aton(asu_ip, &(eloop->as_addr.sin_addr));
	}
	DPrintf("[WAPID]:: Current ASU Server IP=%s\n",asu_ip);
	DPrintf("[WAPID]:: Current ASU Server PORT=%s\n",asu_port);	
	
	memset(&serv, 0, sizeof(serv));
	serv.sin_family = AF_INET;
	serv.sin_addr.s_addr = eloop->as_addr.sin_addr;
	serv.sin_port = htons(AS_AUTH_PORT);
	addr = (struct sockaddr *) &serv;
	addrlen = sizeof(serv);
	sel_sock = eloop->as_udp_sk;
	
	if (connect(sel_sock, addr, addrlen) < 0) {
		perror("connect[radius]");
		return -1;
	}

}
#endif
/*从配置文件中获取配置信息*/
int get_wapi_conf(char *file_name)
{
	prop_data properties[KEYS_MAX];
	int prop_count=0;
	char cert_index[20] = "";
	char asu_ip[20]="";
	char asu_port[20]="";
	char ctrl_interface[256]="/var/run/wapid";
	char cert_mode_str[20] = "";   //三证书

	prop_count=load_prop(SEP_EQUAL,file_name,properties);
	if(prop_count <=0 ) return -1;
	/*读取证书类型*/
	get_prop("CERT_INDEX", cert_index, properties, prop_count);
	/*读取证书文件名*/
	get_prop("CERT_NAME", eloop.cert_info.config.cert_name, properties, prop_count);
	//三证书，读取是两证书还是三证书
	get_prop("CERT_MODE", cert_mode_str, properties, prop_count);
	DPrintf("CERT_MODE =%s\n", cert_mode_str);
	/*读取ASU的IP地址和端口号*/
	get_prop("ASU_IP", asu_ip, properties, prop_count);
	get_prop("ASU_PORT", asu_port, properties, prop_count);
#ifdef CTRL_UNIX	
	get_prop("CTRL_INTERFACE", ctrl_interface, properties, prop_count);
#endif
	free_prop(properties,prop_count);
#ifdef CTRL_UNIX
	if(eloop.ctrl_interface == NULL){ 
		DPrintf("[WAPID]:: ctrl_interface=NULL\n");
		eloop.ctrl_interface = strdup(ctrl_interface);
		wapid_ctrl_iface_init(&eloop);
	}
	else{
		DPrintf("[WAPID]:: ctrl_interface=%s\n",eloop.ctrl_interface);
	}
#endif	
	eloop.cert_info.config.used_cert = (u8)atoi(cert_index);/*1:x509 2:GBW */
	eloop.cert_info.config.cert_mode = (u8)atoi(cert_mode_str);//两证书或三证书标志 
	DPrintf("[WAPID]:: Use %s certificate, path=%s\n",eloop.cert_info.config.used_cert==1?"X.509v3":"N/A",
		eloop.cert_info.config.cert_name);
	if(strlen(eloop.cert_info.config.cert_name))
	{		
		int res;
		res = register_certificate(&eloop);
		if(res !=0){
			DPrintf("[WAPID]:: Register certificate failure\n");
			save_cert_status(eloop.fileconfig, "2");
		}else {
			DPrintf("[WAPID]:: Register certificate success\n");
			eloop.has_cert = 1;
		/*获得AE的身份*/
			res = wai_fixdata_id_by_ident(&eloop.cert_info.ap_cert_obj->user_cert_st, 
							&(eloop.ae_id), 
							eloop.cert_info.config.used_cert);
			if(res != 0){
				DPrintf("error at %s:%d\n", __func__, __LINE__);
				abort();
			}
				
		}
	}
	eloop.as_addr.sin_family = AF_INET;
	eloop.as_addr.sin_port = htons((unsigned short)atoi(asu_port));
	
	if(inet_aton(asu_ip, &(eloop.as_addr.sin_addr)) == 0)
	{
		DPrintf("ASU  IP is wrong\n");
		bzero(asu_ip, 20);
		strcpy(asu_ip, "127.0.0.1");
		inet_aton(asu_ip, &(eloop.as_addr.sin_addr));
	}
	DPrintf("[WAPID]:: Current ASU Server IP=%s\n",asu_ip);
	DPrintf("[WAPID]:: Current ASU Server PORT=%s\n",asu_port);	
	return 0;
}	

#if 0
static void wlan_dump(struct wapid_interfaces *user)
{
	DPrintf("[WAPID]:: %s{\n",  user->identity);
	DPrintf("	SSID=%s\n", user->ssid);
	if(user->wapid->wai_policy &0x04){
		DPrintf("	Authenication_suit=%s\n", "WAPI-PSK");
		DPrintf("	Pre-PSK=");
		wapi_hex_dump(NULL, user->password, user->password_len);
	}else if(user->wapid->wai_policy &0x08){
		DPrintf("	Authenication_suit=%s\n", "WAPI-Cert");
	}else {
		DPrintf("	Authenication_suit=%s\n", "Open");
	}
	if(user->wapid->wai_policy &0x03){
		DPrintf("	Crypto_Suit=%s\n", "SMS4-OFB-CBC");
		DPrintf("	MSK_REKEY=%d\n", user->wapid->msk_rekey_period);
		DPrintf("	USK_REKEY=%d\n", user->wapid->msk_rekey_period);
	}
	printf("}\n");
	return;
}
#endif
static void wlan_dump(struct wapid_interfaces *user)
{
	printf("[WAPID]:: %s{\n",  user->identity);
//	printf("	SSID=%s\n", user->ssid);
	if(user->wapid->wai_policy &0x04){
		printf("	Authenication_suit=%s\n", "WAPI-PSK");
		printf("	Pre-PSK=");
		wapi_hex_dump(NULL, user->password, user->password_len);
	}else if(user->wapid->wai_policy &0x08){
		printf("	\nAuthenication_suit=%s\n", "WAPI-Cert");
	}else {
		printf("	\nAuthenication_suit=%s\n", "Open");
	}
	if(user->wapid->wai_policy &0x03){
		printf("	Crypto_Suit=%s\n", "SMS4-OFB-CBC");
		printf("	MSK_REKEY=%d\n", user->wapid->msk_rekey_period);
		printf("	USK_REKEY=%d\n", user->wapid->usk_rekey_period);
	}
	printf("}\n");
	return;
}

#if 0
void dump_all_interfaces(struct eloop_data *eloop)
{
	/*释放所有资源*/
	struct wapid_interfaces *interfaces = NULL;
	if(eloop == NULL) return;
	interfaces = eloop->vap_user;
	printf("dump all \n");
	for(; interfaces; interfaces=interfaces->next){
		wlan_dump(interfaces);
	}
}
#endif

#if 0
int  load_vap_conf_file(char *file_name, char *be_key,char *end_key)
{
	char line[255];
	FILE *fid;
	char s1[64],s2[64],s3[64],s4[64],s5[64],s6[64];
	int i=0,ret;
	int found = -1;
	struct wapid_interfaces *user = NULL, *head = NULL;

	free_all(&eloop);
	if((fid = fopen(file_name, "r")) == NULL)
	{
		DPrintf("error at %s:%d\n", __func__, __LINE__);
		return -1;
	}
	while (!feof(fid))
	{
		
		user = NULL;
		fscanf(fid, "%[^\n]\n", line);
		if(strncmp(line, be_key,12) == 0)
		 {
			found = 0;
			continue;
		 }	
		if(strncmp(line, end_key,10) == 0)
		 {
		 	 break;
		 }	
		else if((found == 0) && (strncmp(line, end_key,10) != 0))
		{

			if(line[0]=='#') continue;
			user = malloc(sizeof(*user));
			if (user == NULL) {
				DPrintf("error at %s:%d\n", __func__, __LINE__);
				goto failed;
			}
			memset(user, 0, sizeof(*user));
				
			sscanf(line,"%s%s%s%s",s1,s2,s3,s4);
			user->identity = malloc(strlen(s1)+1);
			if (user->identity == NULL) {
				DPrintf("error at %s:%d\n", __func__, __LINE__);
				goto failed;
			}
			memcpy(user->identity, s1, strlen(s1)+1);
			//DPrintf("user->identity = %s\n", user->identity);
			user->identity_len = strlen(s1);
			user->ssid = malloc(strlen(s2)+1);
			if (user->ssid == NULL) {
				DPrintf("error at %s:%d\n", __func__, __LINE__);
				goto failed;
			}
			memcpy(user->ssid, s2, strlen(s2)+1);
			//DPrintf("user->ssid = %s\n", user->ssid);
			user->ssid_len = strlen(s2);
			user->ssid_method = atoi(s3);
			user->wapi_method = atoi(s4);
			if(user->wapi_method & 0x04)/*psk*/
			{
				sscanf(line,"%s%s%s%s%s%s",s1,s2,s3,s4,s5,s6);
				user->psk_type= atoi(s5);
				if(user->psk_type == 1)
				{
					user->password = malloc(strlen(s6) / 2);
					if (user->password == NULL) {
						DPrintf("error at %s:%d\n", __func__, __LINE__);
						goto failed;
					}
					if (str2byte((u8*)s6,strlen(s6)  ,user->password ) < 0) {
						printf("Invalid hex password \n");
						goto failed;
					}
					user->password_len = strlen(s6) / 2;
				}else
					{
					user->password = malloc(strlen(s6)+1);
					if (user->password == NULL) {
						DPrintf("error at %s:%d\n", __func__, __LINE__);
						goto failed;
					}
					memcpy(user->password,s6,strlen(s6)+1);
					user->password_len =strlen(s6);
					//DPrintf("user->password = %s\n", (char *)user->password);
				}
			}
			/*初始化wapid结构*/
			user ->wapid = malloc(sizeof(apdata_info));
			if (user ->wapid == NULL) {
				printf("VAP user allocation failed\n");
				goto failed;
			}
			user->eloop_save = &eloop;
			user->next = NULL;
			memset(user ->wapid, 0, sizeof(apdata_info));
			user ->wapid->wai_policy= user->wapi_method;
			user ->wapid->user_data = (void *)user;
			ret = ap_initialize(user ->wapid);
			if(ret == -1) goto failed;
				
			wapid_setup_interface(user ->wapid);
			if (head == NULL) {
				
				head = eloop.vap_user = user;
			} else {
				head->next = user;
				head = user;
			}
			i++;
			wlan_dump(user);
			continue;
			failed:
				if (user) 
				{
					free_one_interface(user );
					free(user);
					user= NULL;
				}
				continue;
		}
	}	
	fclose(fid);
	return 0;
}
#endif
int  load_vap_conf_file(char *file_name, char *be_key,char *end_key)
{
	char line[255];
	FILE *fid;
#ifdef DOT11_WAPI_MIB	
	char s1[64],s2[64],s3[64],s4[128], s5[64], s6[64],s7[64], s8[64];
	char s9[64],s10[64],s11[64],s12[64], s13[64], s14[64],s15[64], s16[64];
#else
	char s1[64],s2[64],s3[64],s4[128], s6[64],s9[64];
#endif
	int i=0,ret;
	int found = -1;
	struct wapid_interfaces *user = NULL, *head = NULL;

	free_all_interfaces(&eloop);
	if((fid = fopen(file_name, "r")) == NULL)
	{
		DPrintf("error at %s:%d\n", __func__, __LINE__);
		return -1;
	}
	while (!feof(fid))
	{
		
		user = NULL;
		fscanf(fid, "%[^\n]\n", line);
		if(strncmp(line, be_key,12) == 0)
		 {
			found = 0;
			continue;
		 }	
		if(strncmp(line, end_key,10) == 0)
		 {
		 	 break;
		 }	
		else if((found == 0) && (strncmp(line, end_key,10) != 0))
		{

			if(line[0]=='#') continue;
			user = malloc(sizeof(*user));
			if (user == NULL) {
				DPrintf("error at %s:%d\n", __func__, __LINE__);
				goto failed;
			}
			memset(user, 0, sizeof(*user));
#ifdef DOT11_WAPI_MIB					
			sscanf(line,"%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s",
				s1,s2,s3,s4,s5,s6,s7,s8,s9,s10,s11,s12,s13,s14,s15,s16);
#else
			sscanf(line,"%s%s%s%s%s%s",
				s1,s2,s3,s4,s6,s9);
#endif
			user->identity = malloc(strlen(s1)+1);
			if (user->identity == NULL) {
				DPrintf("error at %s:%d\n", __func__, __LINE__);
				goto failed;
			}
			memcpy(user->identity, s1, strlen(s1)+1);
			//DPrintf("user->identity = %s\n", user->identity);
			user->identity_len = strlen(s1);

			user->wapi_method = atoi(s2);
			if(user->wapi_method & 0x04)
			{
				user->psk_type= atoi(s3);
				if(user->psk_type == 1)
				{
					user->password = malloc(strlen(s4) / 2);
					if (user->password == NULL) {
						DPrintf("error at %s:%d\n", __func__, __LINE__);
						goto failed;
					}
					if (str2byte((u8*)s4,strlen(s4)  ,user->password ) < 0) {
						printf("Invalid hex password \n");
						goto failed;
					}
					user->password_len = strlen(s4) / 2;
				}else
					{
					user->password = malloc(strlen(s4)+1);
					if (user->password == NULL) {
						DPrintf("error at %s:%d\n", __func__, __LINE__);
						goto failed;
					}
					memcpy(user->password,s4,strlen(s4)+1);
					user->password_len =strlen(s4);
					//DPrintf("user->password = %s\n", (char *)user->password);
				}
			}
			/*初始化wapid结构*/
			user ->wapid = malloc(sizeof(apdata_info));
			if (user ->wapid == NULL) {
				printf("VAP user allocation failed\n");
				goto failed;
			}
			user->eloop_save = &eloop;
			user->next = NULL;
			memset(user ->wapid, 0, sizeof(apdata_info));
			user ->wapid->wai_policy= user->wapi_method & 0x1f;
			/*for rekey. #20080801*/
			user->wapid->usk_rekey_period= atoi(s6);
			if(user->wapid->usk_rekey_period<MIN_REKEY_PERIOD)
				user->wapid->usk_rekey_period= DEFAULT_REKEY_PERIOD;
			user->wapid->msk_rekey_period= atoi(s9);
			if(user->wapid->msk_rekey_period<MIN_REKEY_PERIOD)
				user->wapid->msk_rekey_period= DEFAULT_REKEY_PERIOD;

			strncpy(user->wapid->iface,user->identity,  IFNAMSIZ);
			user ->wapid->user_data = (void *)user;
			ret = ap_initialize(user ->wapid);
			if(ret == -1){ 
				printf("err at %s:%d\n", __func__, __LINE__);
				goto failed;
			}
#ifdef DOT11_WAPI_MIB
			{
				apdata_info *pap = user->wapid;
				WAPI_MIB_CONFIG_SET_FROM_STR(UnicastRekeyMethod,s5);	
				WAPI_MIB_CONFIG_SET(UnicastRekeyTime,pap->usk_rekey_period);		
				WAPI_MIB_CONFIG_SET_FROM_STR(UnicastRekeyPackets,s7);
				WAPI_MIB_CONFIG_SET_FROM_STR(MulticastRekeyMethod,s8);
				WAPI_MIB_CONFIG_SET(MulticastRekeyTime,pap->msk_rekey_period);	
				WAPI_MIB_CONFIG_SET_FROM_STR(MulticastRekeyPackets,s10);
				WAPI_MIB_CONFIG_SET_FROM_STR(CertificateUpdateCount,s11);
				WAPI_MIB_CONFIG_SET_FROM_STR(UnicastUpdateCount,s12);
				WAPI_MIB_CONFIG_SET_FROM_STR(MulticastUpdateCount,s13);
				WAPI_MIB_CONFIG_SET_FROM_STR(BKLifetime,s14);
				WAPI_MIB_CONFIG_SET_FROM_STR(BKReauthThreshold,s15);
				WAPI_MIB_CONFIG_SET_FROM_STR(SATimeout,s16);
				if (user->wapi_method & 0x20)
					WAPI_MIB_CONFIG_SET(MulticastRekeyStrict,1);
			}
#endif				
			wapid_setup_interface(user ->wapid);
			if (head == NULL) {
				
				head = eloop.vap_user = user;
			} else {
				head->next = user;
				head = user;
				user->next = NULL;
			}
			i++;
			wlan_dump(user);
			continue;
			failed:
				if (user) 
				{
					free_one_interface(user );
					free(user);
					user= NULL;
				}
				continue;
		}
	}	
	fclose(fid);
	return 0;
}


/*套接字初始化*/
void socket_initialize()
{
	eloop.max_fd = -1;
	eloop.wai_raw_sk = -1;
	eloop.as_udp_sk = -1;
#ifdef LSDK6	
	eloop.rth.fd = -1;
#else
	eloop.netlink_raw_sk = -1;
#endif
	eloop.ioctl_fd = -1;
#ifdef CTRL_UPD
	eloop.cgi_udp_sk = -1;
#endif
#ifdef CTRL_UNIX
	eloop.ctrl_sk = -1;
#endif

	eloop.socket_open = 0;
}

/*打开原始套接字*/
int open_raw_socket(int eth_protocol_type)
{
	eloop.g_eth_proto = htons(eth_protocol_type);
	eloop.wai_raw_sk = socket (PF_PACKET, SOCK_RAW, eloop.g_eth_proto);
	if (eloop.wai_raw_sk < 0) {
		return RAW_SOCK_ERR_CREATE;
	}

	return 0;
}

/*创建所有套接字*/
int creat_all_socket()
{
	if(eloop.socket_open == 0)
	{
		/*创建与ASU通信的套接字*/
		if((eloop.as_udp_sk = open_socket_for_asu()) < 0) 
		{
			DPrintf("error at %s :%d\n", __func__, __LINE__);
			abort();
		}
	
	#ifdef LSDK6
 		/*创建接收Driver信息的套接字*/
		if(rtnl_open(&(eloop.rth), RTMGRP_LINK) < 0 )
		{
			DPrintf("error at %s :%d\n", __func__, __LINE__);
			abort();
		}
	#else	
		if((eloop.netlink_raw_sk = socket_open_for_netlink()) < 0)
		{
			DPrintf("error at %s :%d\n", __func__, __LINE__);
			abort();
		}
	#endif	
		/*创建向Driver发送信息的套接字*/
		if((eloop.ioctl_fd = socket_open_for_ioctl()) < 0)
		{
			DPrintf("error at %s :%d\n", __func__, __LINE__);
			abort();
		}
		/*创建UDP套接口,并绑定到127.0.0.1:9003
		*/
#if 0
        if ((eloop.snmpd_udp_sk = open_socket_for_snmpd()) < 0)
		{
			printf("Creat udp socket using for cgi error\n");
			exit(-1);
		}
#endif
#ifdef CTRL_UDP
		/*创建UDP套接口,并绑定到127.0.0.1:9002*/
		if ((eloop.cgi_udp_sk = open_socket_for_cgi()) < 0)
		{
			DPrintf("error at %s :%d\n", __func__, __LINE__);
			abort();
		}
		DPrintf("cgi_udp_sk = %d\n", eloop.cgi_udp_sk);
#endif		
		/*创建WAPI协议的原始套接口*/
		if (open_raw_socket(ETH_P_WAPI)) {
			DPrintf("error at %s :%d\n", __func__, __LINE__);
			abort();
		}
#ifdef CTRL_UNIX
		if((eloop.ctrl_sk = open_socket_for_ctrl())<0){
			DPrintf("error at %s :%d\n", __func__, __LINE__);
		}
		DPrintf("ctrl_sk = %d\n", eloop.ctrl_sk);
#endif
		eloop.socket_open = 1;
	}
	return 0;
}
/*关闭套接字*/
void close_all_sock()
{
	if(eloop.socket_open == 1)
	{
		close(eloop.max_fd);
		close(eloop.wai_raw_sk);
		close(eloop.as_udp_sk);
		//close(eloop.cgi_udp_sk);
		//close(eloop.snmpd_udp_sk);
		close(eloop.ioctl_fd);
	#ifdef LSDK6	
		rtnl_close(&eloop.rth);
	#else
		close(eloop.netlink_raw_sk );
	#endif

	#ifdef CTRL_UDP
		close(eloop.cgi_udp_sk);
	#endif
	#ifdef CTRL_UNIX
		close(eloop.ctrl_sk);
	#endif

	}
	eloop.socket_open = 0;
}
static void eloop_init()
{
	memset(&eloop, 0, sizeof(eloop));
}
static void eloop_deinit(struct eloop_data *eloop)
{
	free_all_interfaces(eloop);
	wapid_ctrl_iface_deinit(eloop);
	close_all_sock();
}

static void eloop_sk_init()
{
	/*初始化socket*/
	socket_initialize();
	/*创建socket*/
	creat_all_socket();
}
/*更新套接字集合*/
static void update_read_fds()
{
	FD_ZERO(&(eloop.read_fds));
	FD_ZERO(&(eloop.ip_recverr_fds));
	
	if(eloop.stop == 0){
		FD_SET(eloop.as_udp_sk,  &(eloop.read_fds));
	#ifdef LSDK6	
		FD_SET(eloop.rth.fd,  &(eloop.read_fds));
	#else
		FD_SET(eloop.netlink_raw_sk,  &(eloop.read_fds));
	#endif
		FD_SET(eloop.wai_raw_sk,  &(eloop.ip_recverr_fds));
		FD_SET(eloop.wai_raw_sk, &(eloop.read_fds));
		
	}
	
#ifdef CTRL_UDP
	FD_SET(eloop.cgi_udp_sk, &(eloop.read_fds));
#endif
#ifdef CTRL_UNIX
	FD_SET(eloop.ctrl_sk, &(eloop.read_fds));
#endif

	if(eloop.stop == 0){
	#ifdef LSDK6
		eloop.max_fd = eloop.netlink_raw_sk;	
	#else
		eloop.max_fd = eloop.rth.fd;	
	#endif

		eloop.max_fd = eloop.as_udp_sk > eloop.max_fd? eloop.as_udp_sk: eloop.max_fd;
		eloop.max_fd = eloop.wai_raw_sk > eloop.max_fd ? eloop.wai_raw_sk : eloop.max_fd;
	}
	
#ifdef CTRL_UDP	
	eloop.max_fd = eloop.cgi_udp_sk>eloop.max_fd ? eloop.cgi_udp_sk : eloop.max_fd;
#endif
#ifdef CTRL_UNIX	
	eloop.max_fd = eloop.ctrl_sk>eloop.max_fd ? eloop.ctrl_sk : eloop.max_fd;
	//DPrintf(" [WAPID]:: eloop.max_fd=%d, eloop.ctrl_sk=%d\n", eloop.max_fd, eloop.ctrl_sk);
#endif

	eloop.max_fd ++;
}

apdata_info *search_ap_by_mac(const u8 *mac_in)
{
	struct wapid_interfaces *interfaces = eloop.vap_user;
	apdata_info *tmp_ap = NULL;
	
	if(mac_in == NULL) return NULL;
	while(interfaces)
	{
		tmp_ap = interfaces->wapid;
		if((tmp_ap) && (memcmp(tmp_ap->macaddr, mac_in, 6) == 0))
		{
			//DPrintf("finding sta("MACSTR")in interfaces %s\n", MAC2STR(mac_in), interfaces->identity);
			return tmp_ap;
		}else{
			interfaces = interfaces->next;
		}
	}
	return NULL;
}

/*超时重发处理*/
void timeout_retry()
{
	int i;
	int waiting_groupkey = 0;
	time_t current_time = time(0);
	struct ethhdr eh;
	struct auth_sta_info_t  *sta_info=NULL;
	struct wapid_interfaces *interfaces = eloop.vap_user;
	//struct eloop_data *tmp_eloop ;
	apdata_info *pap;

	while(interfaces)
	{
		pap = interfaces->wapid;
		if(pap == NULL) return ;
		waiting_groupkey = 0;
		
		for (i=0; i<MAX_AUTH_MT_SIMU; i++) 
		{
			sta_info = &(pap->sta_info[i]);
		#if 0	
			if(sta_info->status != NO_AUTH){
				//DPrintf("[WAPID]:: Sta("MACSTR") status = %u\n", MAC2STR(sta_info->mac), sta_info->status);
				//DPrintf("[WAPID]:: sta_info->sendinfo.send_time= %d\n", sta_info->sendinfo.send_time);
				//DPrintf("[WAPID]:: sta_info->sendinfo.timeout= %d\n", sta_info->sendinfo.timeout);
				//DPrintf("[WAPID]:: current_time= %ld\n", current_time);
			}
		#endif	
#ifdef DOT11_WAPI_MIB
			if (sta_info->status == NO_AUTH)
				continue;
//#if BK_REAUTH			
			if (sta_info->status == MT_AUTHENTICATED)
			{	
				if (sta_info->BKReauthTimeout && sta_info->BKReauthTimeout< (u32)current_time)
				{
					DPrintf("[WAPID]:: Start BK update\n");
					sta_info->status = NO_AUTH;
					sta_info->bksa.bk_update = 1;
					ap_activate_sta(sta_info);
					continue;
				}
			}

//#endif
			if((sta_info->SATimeout && sta_info->SATimeout< (u32)current_time)
//#if BK_REAUTH
		         ||(sta_info->BKLifeTimeout  && sta_info->BKLifeTimeout < (u32)current_time)
//#endif
			)
			{
				DPrintf("SATimeout or BKLifeTimeout\n");
				if(sta_info->status == MT_GROUPNOTICEING
					||sta_info->status == MT_WAITING_DYNAMIC_GROUPING
					||sta_info->status == MT_SESSIONGROUPING)//if msk rereking...
				{
					waiting_groupkey++ ;
					pap->group_No--;//record group notice no.
				}
				if(sta_info->auth_mode == AUTH_MODE)
				{

					notify_driver_disauthenticate_sta(sta_info, __func__, __LINE__);
					DPrintf("notify driver to disauthenticate this sta\n");
				}
				reset_sta_info(sta_info, pap);

			}
#endif
			if (sta_info->status == NO_AUTH
				||sta_info->status == MT_AUTHENTICATED
				||sta_info->sendinfo.cur_count  == 0){
				continue;
			}
			/*检查发送是否超时*/
			if (current_time - sta_info->sendinfo.send_time >= sta_info->sendinfo.timeout) 
			{
				/*检测是否达到最大重发次数*/
				if (sta_info->sendinfo.cur_count >= sta_info->sendinfo.max_count) 
				{
					DPrintf("[WAPID] retry  counter is bigger than %d\n",sta_info->sendinfo.max_count);
					if(sta_info->status == MT_GROUPNOTICEING
						||sta_info->status == MT_WAITING_DYNAMIC_GROUPING
						||sta_info->status == MT_SESSIONGROUPING)
					{
						waiting_groupkey++ ;
						pap->group_No--;//record group notice no.
					}
#ifdef DOT11_WAPI_MIB	
					if(sta_info->status == MT_WAITING_AUTH_FROM_AS)
					{
						WAPI_MIB_STATS_INC(WAICertificateHandshakeFailures);
					}		

					if(sta_info->status == MT_WAITING_SESSION)
					{
						WAPI_MIB_STATS_INC(WAIUnicastHandshakeFailures);
					}
				
					if(sta_info->status == MT_WAITING_GROUPING)
					{
						WAPI_MIB_STATS_INC(WAIMulticastHandshakeFailures);
					}
#endif					
					/*检查鉴别模式*/
					if(sta_info->auth_mode == AUTH_MODE)
					{
						notify_driver_disauthenticate_sta(sta_info, __func__, __LINE__);
						DPrintf("notify driver to disauthenticate this sta\n");
					}
					/*清除重发缓冲区*/
					reset_sta_info(sta_info, pap);
				}
				else{
#ifdef DOT11_WAPI_MIB		
					WAPI_MIB_STATS_INC(WAITimeoutCounters);
#endif
					memcpy (eh.h_dest, sta_info->mac,  ETH_ALEN);
					if ((sta_info->buf0.length> 0) && (sta_info->buf0.data !=NULL))
					{	
					#if 0
						DPrintf("Timeout. (%d) status=%d  send_count=%d \n", 
						i,  sta_info->status, sta_info->sendinfo.cur_count);

						DPrintf("\nresend buf0. len=%d\n", sta_info->buf0.len);
					#endif	
						/*检查发送的反向*/
						if (sta_info->sendinfo.direction == SENDTO_AS)
						{
							DPrintf("[WAPID]::  %d retry '%s' to to AS \n", sta_info->sendinfo.cur_count,
								wai_subtype_info[sta_info->buf0.type]);
                           				if (eloop.as_udp_sk > 0)
    								sendto( eloop.as_udp_sk, sta_info->buf0.data, sta_info->buf0.length, 
									0, (struct sockaddr *)&(eloop.as_addr), sizeof(struct sockaddr_in));
						}
						else
						{
							DPrintf("[WAPID]::  %d retry '%s' to to STA( "MACSTR")\n", sta_info->sendinfo.cur_count,
								wai_subtype_info[sta_info->buf0.type], MAC2STR(sta_info->mac));
							/*发送数据*/
							send_rs_data(sta_info->buf0.data, sta_info->buf0.length, &eh, pap);
						}
					}
		
					if ((sta_info->buf1.length > 0) && (sta_info->buf1.data !=NULL)) 
					{				
						DPrintf("[WAPID]:: %d retry '%s' to to STA( "MACSTR")\n", sta_info->sendinfo.cur_count,
							wai_subtype_info[sta_info->buf1.type], MAC2STR(sta_info->mac));
						send_rs_data(sta_info->buf1.data, sta_info->buf1.length, &eh, pap);	
					}
					sta_info->sendinfo.send_time = current_time;
					(sta_info->sendinfo.cur_count) ++;
				}
			}
		}
		if(	(pap->group_status == 1) && 
			(pap->group_No == 0)&&
			(waiting_groupkey != 0))/*组播密钥更新完成*/
		{
			DPrintf("in process_timeout()\n");
			pap->group_status = 0;
			pap->rekeying = 0;
			set_mcastkey(pap, &pap->msksa);
		}
		interfaces = interfaces->next;
	}
}


/*------------------------------------------------------------------*/
/*
 * Print out all Wireless Events part of the RTNetlink message
 * Most often, there will be only one event per message, but
 * just make sure we read everything...
 */
#ifdef LSDK6 
static inline int handle_event_stream(  char *data,  int	len)
{
  	asso_mt *asso_mt_info ;
	apdata_info *pap = NULL;
	if (len < (int)IW_EV_POINT_LEN )
	{
		DPrintf("[WAPID]:: Receive invallid netlink event  LEN=%d,IW_EV_POINT_LEN=%d\n",len,IW_EV_POINT_LEN);
		return 0;
	}
	asso_mt_info = (asso_mt *)(data+IW_EV_POINT_LEN);
	//DPrintf("[WAPID]:: Receive netlink event  LEN=%d,IW_EV_POINT_LEN=%d\n",IW_EV_LCP_LEN,IW_EV_POINT_LEN);
	//wapi_hex_dump("Receive netlink event",  asso_mt_info,len-IW_EV_POINT_LEN);
	pap = search_ap_by_mac(asso_mt_info->ap_mac);
	if(pap){
		wapi_process_for_driver((unsigned char*)data+IW_EV_POINT_LEN, asso_mt_info->data_len+2, pap);
	}
	return(0);
}
#endif
/*********************** RTNETLINK EVENT DUMP***********************/
/*
 * Dump the events we receive from rtnetlink
 * This code is mostly from Casey
 */

/*------------------------------------------------------------------*/
/*
 * Respond to a single RTM_NEWLINK event from the rtnetlink socket.
 */\
#ifdef LSDK6
static int LinkCatcher(struct nlmsghdr *nlh)
{
	struct ifinfomsg* ifi;

	ifi = NLMSG_DATA(nlh);

	/* Check for attributes */
	if (nlh->nlmsg_len > NLMSG_ALIGN(sizeof(struct ifinfomsg))){
		int attrlen = nlh->nlmsg_len - NLMSG_ALIGN(sizeof(struct ifinfomsg));
		
		struct rtattr *attr = (void *) ((char *) ifi + NLMSG_ALIGN(sizeof(struct ifinfomsg)));

		while (RTA_OK(attr, attrlen)){
			/* Check if the Wireless kind */
			if(attr->rta_type == IFLA_WIRELESS){
				/* Go to display it */
				handle_event_stream( (char *) attr + RTA_ALIGN(sizeof(struct rtattr)), attr->rta_len - RTA_ALIGN(sizeof(struct rtattr)));
			}
			attr = RTA_NEXT(attr, attrlen);
		}
	}

	return 0;
}
#endif


#ifdef LSDK6
static void handle_netlink_events(struct rtnl_handle *rth)
{
	struct sockaddr_nl sanl;
	socklen_t sanllen = sizeof(struct sockaddr_nl);

	struct nlmsghdr *h;
	int amt;
	char buf[8192];

	amt = recvfrom(rth->fd, buf, sizeof(buf), MSG_DONTWAIT, (struct sockaddr*)&sanl, &sanllen);
	if(amt>0)
	{
		h = (struct nlmsghdr*)buf;
		while(amt >= (int)sizeof(*h))
		{
			int len = h->nlmsg_len;
			int l = len - sizeof(*h);

			if(l < 0 || len > amt)
			{
				fprintf(stderr, "%s: malformed netlink message: len=%d\n", __PRETTY_FUNCTION__, len);
				break;
			}
			LinkCatcher(h);
			len = NLMSG_ALIGN(len);
			amt -= len;
			h = (struct nlmsghdr*)((char*)h + len);
		}
	}
	if(amt > 0)
		fprintf(stderr, "%s: remnant of size %d on netlink\n", __PRETTY_FUNCTION__, amt);
}
#endif

/*处理命令行参数*/
static void parse_opts (int argc, char **argv)
{
	char	opt;
	opterr = 0;
	while((opt = getopt(argc, argv, "i:c:d:")) > 0) 
	{
		switch(opt) 
		{
		case 'c':
			strncpy(eloop.fileconfig, optarg, 255);
			break;
		case 'd':
			//printf("%s\n", optarg);
			//debug ++;
			debug  = atoi(optarg);
			//printf("debug = %d\n", debug);
			eloop.debug++;
			break;
		default:
			DPrintf("opt is 0x%02x\n", opt);
			break;
		}
	}
}

static int wapid_setup_interface(apdata_info *hapd)
{
	int ret = 0;
	
	if(hapd == NULL)  return -1;
	if(hapd->sta_info == NULL)
	{	
		hapd->sta_info = malloc(sizeof(struct auth_sta_info_t)*MAX_AUTH_MT_SIMU);
		if (hapd->sta_info == NULL) {
			struct wapid_interfaces *wapid = NULL ;
			struct eloop_data *tmp_eloop = NULL; ;
			wapid =(struct wapid_interfaces *)(hapd->user_data);
			if(wapid)
			{
				tmp_eloop = wapid->eloop_save;
				if(tmp_eloop)
					free_all_interfaces(tmp_eloop);	
			}
			printf("wapid_setup_interface malloc failed\n");
			exit(1);
		}
	}
	memset(hapd->sta_info, 0, sizeof(struct auth_sta_info_t)*MAX_AUTH_MT_SIMU);

	/*设置Driver中的鉴别与密钥管理套间*/
	set_wapi(hapd);

	/*初始化鉴别与密钥管理套间*/
	ap_initialize_akm(hapd);
	
	/*初始化STA表*/
	ap_initialize_sta_table(hapd);

#ifdef LSDK6	
	/*send IE buf to driver via ioctl*/
	ap_setdriver_wie(hapd, IEEE80211_APPIE_FRAME_BEACON);
	ap_setdriver_wie(hapd, IEEE80211_APPIE_FRAME_PROBE_RESP);
	//    ap_setdriver_wie(hapd, IEEE80211_APPIE_FRAME_ASSOC_RESP);
#endif
#ifdef DOT11_WAPI_MIB
	ap_setup_mib(hapd);
#endif
	return ret;
}


/* serach the rekey unicast sta */
static void  wapid_rekey_usk(void)
{
	apdata_info *pap    = NULL;
	struct auth_sta_info_t *sta_table    = NULL;
	struct wapid_interfaces *interfaces =NULL;
	time_t current_time;
	int i = 0;
 	
	/* get the system current time */
	
	
	/* search the first wireless network card */
	interfaces = eloop.vap_user;
	while (interfaces)
	{
		pap = interfaces->wapid;
		sta_table = pap->sta_info;
#ifdef DOT11_WAPI_MIB
		if (pap->mib_config.ConfigUnicastRekeyMethod !=REKEY_POLICY_TIMEBASED &&
			pap->mib_config.ConfigUnicastRekeyMethod !=REKEY_POLICY_TIMEPACKETBASED)
		{
			interfaces = interfaces->next;
			continue;
		}
#endif		
		for (i = 0; i < MAX_AUTH_MT_SIMU; i++)
		{
			current_time = time(NULL);
			if (sta_table[i].status != MT_AUTHENTICATED)
				continue;
			if(sta_table[i].rekeying == 1)
				continue;
			if(current_time >= (sta_table[i].usk_rekey_timer + pap->usk_rekey_period))
			{
				/* find the result */
				DPrintf("[WAPID]:: find USK rekey sta \n");
				//DPrintf("	usk_rekey_timer=%ld, current_time = %ld\n", sta_table[i].usk_rekey_timer, current_time);
				DPrintf("	STA("MACSTR") USK rekey start\n", MAC2STR(sta_table[i].mac));
				
				usk_rekey_handle(&sta_table[i]);
#ifdef DOT11_WAPI_MIB
				if (pap->mib_config.ConfigUnicastRekeyMethod ==REKEY_POLICY_TIMEPACKETBASED)
					update_usk_rekey_pkts(sta_table[i].mac, pap);
#endif				
			}
		#if 0	
			else{
				DPrintf("current_time= %ld\n",current_time);
				DPrintf("sta_table[i].usk_rekey_timer = %ld\n", sta_table[i].usk_rekey_timer);
				DPrintf("eloop.usk_rekey_period= %d\n", pap->usk_rekey_period);
				DPrintf("sta_table[%d].usk_rekey_timer(%ld) + eloop.usk_rekey_period(%d) = %ld\n", i,
					sta_table[i].usk_rekey_timer , pap->usk_rekey_period,
					(sta_table[i].usk_rekey_timer + pap->usk_rekey_period));
			}
		#endif	
		}
		interfaces = interfaces->next;
	}
	return ;
}


/* serach the rekey group sta */
static void wapid_rekey_msk(void)
{
	apdata_info *pap    = NULL;
	struct wapid_interfaces *interfaces =NULL;
	time_t current_time;
 
	/* get the system current time */
	
	/* search the first wireless network card */
	interfaces = eloop.vap_user;
	for (;interfaces; interfaces = interfaces->next)
	{
		current_time = time(NULL);
		pap = interfaces->wapid;
#ifdef DOT11_WAPI_MIB
		if (pap->mib_config.ConfigMulticastRekeyMethod !=REKEY_POLICY_TIMEBASED &&
			pap->mib_config.ConfigMulticastRekeyMethod !=REKEY_POLICY_TIMEPACKETBASED)
			continue;
#endif		
		if(pap->rekeying)
			continue;
		if ((pap->wai_policy &0x03)&&
				(current_time >= (pap->msk_rekey_timer + pap->msk_rekey_period)))
		{
			/* find the result */
			DPrintf("[WAPID]:: find MSK rekey VAP \n");
			//DPrintf("msk_rekey_timer=%ld current_time = %ld,  \n", pap->msk_rekey_timer, current_time);
			DPrintf("[WAPID]:: VAP("MACSTR") MSK rekey start\n", MAC2STR(pap->macaddr));
			msk_rekey_handle(pap);
#ifdef DOT11_WAPI_MIB
			if (pap->mib_config.ConfigMulticastRekeyMethod ==REKEY_POLICY_TIMEPACKETBASED )
				set_msk_rekey_pkts(pap); 
#endif
		}
	}
	return ;
}

static void rekey_handle()
{
	wapid_rekey_msk();
	wapid_rekey_usk();
}	
static void wapid_terminate(int sig)
{
	DPrintf("Signal %d received - terminating\n", sig);
	eloop.quit = 1;
}


static int eloop_run()
{
	#define BUF_LEN	FROM_AS_LEN
	u8 recv_buf[BUF_LEN] = {0,};
	int ret = 0;
	socklen_t i = 0;
	int readlen = 0;
	int sel_err = 0;
	int create_ptv = 0;
	struct ethhdr eh;
	struct sockaddr_in s_addr, c_addr;
	struct timeval tv, *ptv= NULL;
	struct auth_sta_info_t *sta_table ;
	struct wapid_interfaces *interfaces =NULL;
	apdata_info *pap = NULL;
	memset(&eh, 0, sizeof(struct ethhdr));
	memset(&s_addr, 0, sizeof(struct sockaddr_in));
	memset(&c_addr, 0, sizeof(struct sockaddr_in));
	
	while (!eloop.quit)
	{
		update_read_fds();
		interfaces = eloop.vap_user;
		create_ptv = 0;
		ptv = NULL;

		for(;interfaces; interfaces=  interfaces->next)
		{
			sta_table = interfaces->wapid->sta_info;
			for (i=0; i<MAX_AUTH_MT_SIMU; i++){
				if (	(sta_table[i].status != NO_AUTH) &&
					(sta_table[i].status != MT_AUTHENTICATED)){
					
					tv.tv_sec = TIMEOUT;
					tv.tv_usec = 0;
					ptv = &tv;
					create_ptv = 1;
					DPrintf("[WAPID]:: Install timer (timeout=%ld)\n", tv.tv_sec);
					break;
				}
			}
		}
		if(create_ptv == 0){
			tv.tv_sec = REKEY_U_CHECK_INTERVAL;
			tv.tv_usec = 0;
			ptv = &tv;
			DPrintf("[WAPID]:: Install timer (timeout=%ld)\n", tv.tv_sec);
		}
		sel_err = select(eloop.max_fd, &(eloop.read_fds), NULL,  &(eloop.ip_recverr_fds), ptv);
				
		if (sel_err < 0)
		{
			continue;
		}
		
		if (sel_err > 0)
		{
		
			if(FD_ISSET(eloop.wai_raw_sk, &(eloop.ip_recverr_fds)))
			{
				if(eloop.stop) continue;
				printf("RECEIVE ERROR FROM wai_raw_sk\n");
				ret = handle_recverr(eloop.wai_raw_sk);
				if(ret < 0) continue;
			}
		#ifdef LSDK6	
			if (FD_ISSET(eloop.rth.fd, &(eloop.read_fds)))	
			{
				/*** receive Data from netlink_raw_sk *****/ 
				if(eloop.stop) continue;
				handle_netlink_events(&eloop.rth);
			}
		#else	
			if (FD_ISSET(eloop.netlink_raw_sk, &(eloop.read_fds)))	
			{
				asso_mt *asso_mt_info ;
				if(eloop.stop) continue;
				readlen = recvfrom(eloop.netlink_raw_sk, recv_buf, BUF_LEN, 0, NULL, NULL);

				if(readlen >0)
				{
					asso_mt_info = (asso_mt *)recv_buf;
					pap = search_ap_by_mac(asso_mt_info->ap_mac);
					if(pap){
						wapi_process_for_driver(recv_buf, readlen, pap);
					}
				}
			}
		#endif	
			else if (FD_ISSET(eloop.as_udp_sk, &(eloop.read_fds))) 
			{
				/*** receive Data from as_udp_sk handle****/
				if(eloop.stop) continue;
				i = sizeof(s_addr);
				readlen = recvfrom(eloop.as_udp_sk, recv_buf, BUF_LEN, 0,
									(struct sockaddr *)&s_addr, &i);
				if(readlen > WAI_HLEN/*WAI head*/)
				{
					pap = search_ap_by_mac(recv_buf + sizeof(packet_head));
					if(pap){
						wapi_process_1_of_1_from_as(recv_buf, readlen, pap);
					}
				}
			}
			else if (FD_ISSET(eloop.wai_raw_sk, &(eloop.read_fds)))
			{
				/*** receive Data from wai_raw_sk****/
				if(eloop.stop) continue;
				readlen = recv_rs_data(recv_buf, BUF_LEN, &eh, eloop.wai_raw_sk);

				if (readlen < 0)
				{
					continue;
				}
				pap = search_ap_by_mac(eh.h_dest);
				if(pap == NULL) continue;
				/*未启用WAPI*/
				if((pap->wai_policy &  0x0C) == 0 /*not wai or psk*/)
				{
					continue;
				}
				wapi_process_from_sta(recv_buf, readlen, eh.h_source, pap);
			}
#ifdef CTRL_UDP
			else if (FD_ISSET(eloop.cgi_udp_sk, &(eloop.read_fds)))
			{
				int sendlen = 0;
				int cmd = 0;
				
				if(eloop.stop) continue;
				DPrintf("[WAPID]:: Receive ctrl cmd from cgi\n");
				i = sizeof(c_addr);
				readlen = recvfrom(eloop.cgi_udp_sk, recv_buf, BUF_LEN,
									0, (struct sockaddr *)&c_addr, &i);
				if (readlen < 0){
					DPrintf("error at %s :%d\n", __func__, __LINE__);
					continue;
				}else{				
					sendlen = Process_CTRL_message(recv_buf,  readlen, &cmd);
					if(sendlen >0)
					{
						readlen = sendto(eloop.cgi_udp_sk, recv_buf, sendlen, 0, (struct sockaddr *)&c_addr, i);
					}
					if(cmd == CGI_CMD_RELOAD)
						return CGI_CMD_RELOAD;
				}
			}
#endif
#ifdef CTRL_UNIX 
			else if (FD_ISSET(eloop.ctrl_sk, &(eloop.read_fds)))
			{
				ret  = wapid_ctrl_handle(eloop.ctrl_sk, &eloop);
				if(ret == CGI_CMD_RELOAD){
						return CGI_CMD_RELOAD;
				}
			}
#endif			
		}/**** end if (sel_err>0) ***/
		if (create_ptv == 1)
		{
			DPrintf("[WAPID]:: Retry handle\n");
			timeout_retry( );
		}
		rekey_handle();
	}
	return 0;
}

int main(int argc, char *argv[])
{
	int ret = 0;
	/*初始化openssl*/
	/*处理命令行参数*/
	opterr = 0;
		/*处理命令行参数*/
	DPrintf("\%s start......................................\n", argv[0]); 
	DPrintf("version = MAC layer WAPI AUTH CTRL 1.3\n");


	signal(SIGINT, wapid_terminate);
	signal(SIGTERM, wapid_terminate);

	
	eloop_init();

	parse_opts(argc, argv);

	/*初始化证书对象*/
	if(ecc192_init() >0){
		DPrintf("[WAPID]:: ecc192 initilize success\n");
	}else{
		DPrintf("[WAPID]:: ecc192 initilize failure\n");
	}
	X509_init();
	eloop_sk_init();
reload:
	
	if(get_wapi_conf(eloop.fileconfig) != 0)	{
		DPrintf("[WAPID]:: initilize %s failure\n",eloop.fileconfig);
		goto cleanup;
	}
	
	eloop.card_id = 0;
	DPrintf("AP has %d wireless card\n",eloop.card_id+1);

	if(load_vap_conf_file(eloop.fileconfig,"[WLAN_BEGIN]","[WLAN_END]") != 0){
		goto cleanup;
	}
	/*进入鉴别主程序*/
	ret = eloop_run();
	
	/*WAI规则被修改,重新初始化*/
	if (ret == CGI_CMD_RELOAD) {
		goto reload;
	}
cleanup:
	/*释放所有资源*/	
	eloop_deinit(&eloop);
	X509_exit();
	ecc192_deinit();
	return 0;
}

