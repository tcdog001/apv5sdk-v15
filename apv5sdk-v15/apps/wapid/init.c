/*****************************************************************
* Copyright (c) 2001,西安西电捷通无线网络通信有限公司
* All rights reserved.
* 
* 文件名称：init.c
* 摘    要： 系统初始化相关函数的实现
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
#include <stdio.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>
#include <errno.h>

/* Network Includes */
#include <unistd.h>
#include <time.h>
#include <assert.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <asm/types.h>
#include <linux/netlink.h>


#ifdef MEM_DEBUG
	#include "memwatch.h"
#else
	#include <stdlib.h>
#endif

#include "pack.h"
#include "typedef.h"
#include "raw_socket.h"
#include "proc.h"
#include "debug.h"
#include "alg_comm.h"
#include "auth.h"
#include "wai_sta.h"
#include "certupdate.h"
#include "cert_auth.h"
#include "init.h"
#ifdef DOT11_WAPI_MIB
#include "dot11wapiMib.h"
#endif
/*初始化AP 的WAPI信息元素*/
int ap_initialize_wie(apdata_info *papdata)
{/*初始化WIE 参数*/
#define ADDSHORT(frm, v) do {frm[0] = (v) & 0xff;frm[1] = (v) >> 8;} while (0)
#define ADDSELECTOR(frm, sel) do {memcpy(frm, sel, 4); frm += 4;} while (0)

	u8	 *p = papdata->wie_ae, flag1 = 0, flag2 = 0, *q;
	u8 akm_haha1[4] = {0x00, 0x14, 0x72, 0x01};/*WAI证书鉴别和密钥管理*/
	u8 akm_haha2[4] = {0x00, 0x14, 0x72, 0x02};/*WAI共享密钥鉴别和密钥管理*/
	
	memset(p, 0, 255);
	*p = 68;
        p += 1;
	p += 1;/*length*/
	ADDSHORT(p,1);
	p += sizeof(u16);
	p += sizeof(u16);
	if(papdata->wai_policy & 0x08 /*bit 3:WAI*/)
	{
		flag1 = 1;
		ADDSELECTOR(p, akm_haha1);
	}
	if(papdata->wai_policy & 0x04 /*bit 2:PSK*/)
	{
		flag2 = 1;
		ADDSELECTOR(p, akm_haha2);
	}
	q = papdata->wie_ae + 4;
	ADDSHORT(q ,flag1 + flag2);

	flag1 = 0;
	flag2 = 0;
	
	p += sizeof(u16);
	if(papdata->wai_policy & 0x02 /*bit 1:单播密码套件:WPI-SMS4*/)
	{
		flag1 = 1;
		ADDSELECTOR(p, akm_haha1);
	}
	q = p -sizeof(u16) - flag1 * 4;
	ADDSHORT(q, flag1);//*(u16 *)(p -sizeof(u16) - flag1 * 4)= flag1;/*set key no*/
	
	if(papdata->wai_policy & 0x01 /*bit 0:组播密码套件:WPI-SMS4*/)
	{
		ADDSELECTOR(p, akm_haha1);
	}
	/*set capability*/
	if(papdata->wai_policy & 0x10 /*bit 4:pre-auth*/)
	{
		ADDSHORT(p , 1);
	}
	p += sizeof(u16);

	/*set wie length*/
	papdata->wie_ae[1] = p - papdata->wie_ae - 2;
	return 0;
}
#ifdef LSDK6
/*set the driver's WAPI IE through ioctl*/
int ap_setdriver_wie(apdata_info *papdata, int mgmttype)
{
#if 0        
/*WAPI IE will be appened in driver*/        
    u8 buf[256];
    struct ieee80211req_getset_appiebuf *wie;
    int ielen;
    wie = (struct ieee80211req_getset_appiebuf *)buf;
    ielen = papdata->wie_ae[1] + 2;
    memcpy(&(wie->app_buf[0]), papdata->wie_ae, ielen);
    wie->app_buflen = ielen;
    wie->app_frmtype = mgmttype;
    return wapid_ioctl(papdata, IEEE80211_IOCTL_SET_APPIEBUF, wie, 
        sizeof(struct ieee80211req_getset_appiebuf) + ielen);
#endif
    return 0;
}
#endif

/*初始化AP的签名算法参数*/
int ap_initialize_alg(apdata_info *papdata)
{/*初始化alg 参数*/
	char alg_para_oid_der[16] = {0x06, 0x09,0x2a,0x81,0x1c, 0xd7,0x63,0x01,0x01,0x02,0x01};
	
	memset((u8 *)&(papdata->sign_alg), 0, sizeof(wai_fixdata_alg));
	papdata->sign_alg.alg_length = 16;
	papdata->sign_alg.sha256_flag = 1;
	papdata->sign_alg.sign_alg = 1;
	papdata->sign_alg.sign_para.para_flag = 1;
	papdata->sign_alg.sign_para.para_len = 11;
	memcpy(papdata->sign_alg.sign_para.para_data, alg_para_oid_der, 11);
	return 0;
}

#ifdef DOT11_WAPI_MIB
int ap_initialize_mib(apdata_info *pap)
{
	struct wapid_interfaces *wapid ;
	wapid = pap->user_data;

	wapimib_config_init(&pap->mib_config);
//	hidden passwd
//	strcpy((char*)pap->mib_config.ConfigPSKPassPhrase,(char*)wapid->password);
	
	wapimib_ucipher_init(&pap->mib_uciphers);
	if(pap->wai_policy & 0x02 /*bit 1:Unicast Cipher Suite*/)
	{
		pap->mib_uciphers.ConfigUnicastCipherEnabled=1;
	}
	
	wapimib_asuites_init(pap->mib_asuites);
	if(pap->wai_policy & 0x08 /*bit 3:WAI*/)
	{
		pap->mib_asuites[0].ConfigAuthenticationSuiteEnabled=1;
	}
	if(pap->wai_policy & 0x04 /*bit 2:PSK*/)
	{
		pap->mib_asuites[1].ConfigAuthenticationSuiteEnabled=1;
	}
	wapimib_stats_init(&pap->mib_stats);
	memset(pap->mib_stats.StatsSTAAddress,0xff, 6);

	return 0;
	
}

int ap_setup_mib(apdata_info *pap)
{

	set_usk_rekey_pkts(pap);
	set_msk_rekey_pkts(pap);
	return 0;
				
}
#endif

static void free_vap(apdata_info *pap, int freeit)
{
	int i = 0;
	struct auth_sta_info_t *sta = NULL;

	if(!pap) return ;
	
	sta = pap->sta_info;
	if(sta){
		for(i=0; i<MAX_AUTH_MT_SIMU; i++){
			if(sta[i].status != NO_AUTH){
				reset_sta_info(&sta[i], pap);
			}
		}
		memset(sta, 0, sizeof(struct auth_sta_info_t)*MAX_AUTH_MT_SIMU);
		free(sta);
		pap->sta_info = NULL;
	}
	if(freeit){
		memset(pap, 0, sizeof(apdata_info));
		free(pap);
	}
}

void free_one_interface(struct wapid_interfaces *interfaces )
{
		if(interfaces->identity)
		{
			free(interfaces->identity);
			interfaces->identity = NULL;
		}
#if 0        
		if(interfaces->ssid)
		{
			free(interfaces->ssid);
			interfaces->ssid = NULL;
		}
#endif
		if(interfaces->password)
		{
			free(interfaces->password);
			interfaces->password= NULL;
		}
		if(interfaces->wapid->sta_info)
		{
			free(interfaces->wapid->sta_info);
			interfaces->wapid->sta_info= NULL;
		}	
		if(interfaces->wapid)
		{
			free_vap(interfaces->wapid, 1);
			//free(interfaces->wapid);
			interfaces->wapid= NULL;
		}
}




void free_all_interfaces(struct eloop_data *eloop)
{
	/*释放所有资源*/
	struct wapid_interfaces *interfaces = NULL,*tmp_interfaces= NULL;
	if(eloop == NULL) return;
	interfaces = eloop->vap_user;
	while(interfaces)
	{
		free_one_interface(interfaces );
		tmp_interfaces = interfaces;
		interfaces = interfaces ->next;
		memset(tmp_interfaces, 0, sizeof(*tmp_interfaces));
		free(tmp_interfaces);
		tmp_interfaces = NULL;
		eloop->vap_user = interfaces;
	}
}

/*AP初始化*/
int ap_initialize(apdata_info *pap)
{
	int ret = 0;
	int i = 0;
	
	struct wapid_interfaces *tmp_wapid;
	struct eloop_data *tmp_eloop;
	
	assert(pap!=NULL);
	tmp_wapid = (struct wapid_interfaces *)pap->user_data;
	tmp_eloop = (struct eloop_data *)tmp_wapid->eloop_save;
	/*获得设备的索引号*/

	while(i<5)		
	{
		i++;
		pap->g_ifr = get_device_index_by_raw_socket(tmp_wapid->identity, tmp_eloop->wai_raw_sk);
		if(pap->g_ifr >0)
		{
			break;
		}
		else //if(pap->g_ifr <0)
		{
			printf("get_device_index_by_raw_socket failed:dev_name=%s\n",tmp_wapid->identity);
			sleep(5);
			continue;
		}/*获得设备的MAC地址*/
	}

	if(i>=5) 
	{
		goto err;
	}
	if(get_device_mac_by_raw_socket(tmp_wapid->identity, tmp_eloop->wai_raw_sk,pap->macaddr)<0)
	{
		
		printf("get_device_mac_by_raw_socket failed \n");
		goto err;
	}
	/*获得设备的MTU值*/
	if(get_device_mtu_by_raw_socket(tmp_wapid->identity, tmp_eloop->wai_raw_sk,&(pap->mtu))<0)
	{
		printf("get_device_mtu_by_raw_socket failed\n");
		goto err;
	}

	/*for compatibility with wapi supplicants who don't support large WLAN frame */
	if(pap->mtu > 1500)
	{
		DPrintf("[WAPID]::interface %s MTU %d to 1500\n", tmp_wapid->identity, pap->mtu);
		pap->mtu = 1500;
	}

	/*初始化msksa*/
	get_random(pap->msksa.msk, MSK_LEN);
	pap->msksa.mskid= 0;
#if 1
	wapi_hex_dump("MSK", pap->msksa.msk, MSK_LEN);
#endif
	/*初始化gnonce*/
	pap->gnonce[0] = 0x5c365c36;
	pap->gnonce[1] = 0x5c365c36;
	pap->gnonce[2] = 0x5c365c36;
	pap->gnonce[3] = 0x5c365c36;
	//DPrintf("wai_policy = %d\n", pap->wai_policy);
	pap->ap_debug = debug;
	if( pap->wai_policy&0x04)
		ap_pskbk_derivation(pap);
	/*初始化AP 的WAPI信息元素*/
	ap_initialize_wie(pap);
	/*初始化AP的签名算法参数*/
	ap_initialize_alg(pap);
	pap->group_No = 0xFFFF;
	memset(pap->gsn, 0, 16);

#ifdef DOT11_WAPI_MIB
	ap_initialize_mib(pap);
#endif
	return ret;
err:
	printf("ap_initialize failed\n");
	return -1;
}

/*初始化STA信息记录表*/
void ap_initialize_sta_table(apdata_info *pap)
{
	int i = 0;
	struct auth_sta_info_t *psta_table = pap->sta_info;

	for(i = 0; i < MAX_AUTH_MT_SIMU; i++)
	{
		/*重新初始化STA信息*/
		reset_sta_info(&psta_table[i], pap);
	}
}

/*初始化鉴别与密钥管理套间*/
int ap_initialize_akm(apdata_info *apdata)
{
#define BIT(x) (1 << (x))
	struct wapid_interfaces *wapid ;
	struct eloop_data *tmp_eloop ;

	wapid = (struct wapid_interfaces *)apdata->user_data;
	if(wapid == NULL) return -1;
	tmp_eloop = (struct eloop_data *)wapid->eloop_save;
	if(tmp_eloop == NULL) return -1;
	/*判断AP是否启用鉴别与密钥管理套间*/
	if(!(apdata->wai_policy &(BIT(2) |BIT(3))))
	{
		return 0;
	}
	/*设置MEK, MAK*/
	set_mcastkey(apdata, &apdata->msksa);
	return 0;
#undef BIT
}

									 
/*创建与ASU通信的套接字*/
 int open_socket_for_asu() 
{
	int sock;
 	sock = socket(PF_INET, SOCK_DGRAM, 17);
	
	if (sock == -1 ) 
	{
		DPrintf("error at %s :%d\n", __func__, __LINE__);
 	}
	return sock;
}

/*创建向Driver发送信息的套接字*/
int socket_open_for_ioctl()
{
  	int inet_sock = -1; 			/* INET socket					*/

 	inet_sock=socket(AF_INET, SOCK_DGRAM, 0);
	return inet_sock;
 }

#ifdef LSDK6
void rtnl_close(struct rtnl_handle *rth)
{
	close(rth->fd);
}

int rtnl_open(struct rtnl_handle *rth, unsigned subscriptions)
{
	int addr_len;

	memset(rth, 0, sizeof(rth));

	rth->fd = socket(PF_NETLINK, SOCK_RAW, NETLINK_ROUTE);
	if (rth->fd < 0) {
		perror("Cannot open netlink socket");
		return -1;
	}

	memset(&rth->local, 0, sizeof(rth->local));
	rth->local.nl_family = AF_NETLINK;
	rth->local.nl_groups = subscriptions;

	if (bind(rth->fd, (struct sockaddr*)&rth->local, sizeof(rth->local)) < 0) {
		perror("Cannot bind netlink socket");
		return -1;
	}
	addr_len = sizeof(rth->local);
	if (getsockname(rth->fd, (struct sockaddr*)&rth->local,
			(socklen_t *) &addr_len) < 0) {
		perror("Cannot getsockname");
		return -1;
	}
	if (addr_len != sizeof(rth->local)) {
		fprintf(stderr, "Wrong address length %d\n", addr_len);
		return -1;
	}
	if (rth->local.nl_family != AF_NETLINK) {
		fprintf(stderr, "Wrong address family %d\n", rth->local.nl_family);
		return -1;
	}
	rth->seq = time(NULL);
	return 0;
}
#endif

#ifndef LSDK6
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
		nlskaddr.nl_groups =  COMMTYPE_GROUP;
	}
	else
	{
		DPrintf("error at %s :%d\n", __func__, __LINE__);
		return -1;
	}
	bindsock = bind(sock,(struct sockaddr *)&nlskaddr,sizeof(nlskaddr));
	if(bindsock!=0)
	{
		DPrintf("error at %s :%d\n", __func__, __LINE__);
 		return -1;
	}
	return sock;
}
#endif

/*设置鉴别与密钥管理套间*/
int set_wapi(apdata_info *pap)
{
#ifdef LSDK6
	int param[2];
#endif 
	u8 buffer[128]={0,};
	int res = 0;
	u8 *pos = NULL;
	
#ifdef LSDK6	
	param[0] = P80211_IOCTL_SETWAPI_INFO;
#endif

	pos = buffer;
	
	SET16(pos, P80211_PACKET_WAPIFLAG); pos += 2;
	SET16((buffer+2), 1); pos += 2;
	*pos = pap->wai_policy;
	
	printf("%s: send command, param at %p with len %d\n", __func__, buffer, sizeof(buffer));
#ifdef LSDK6	
	param[1] = (int)pap->wai_policy;
	res=wapid_ioctl(pap, IEEE80211_IOCTL_SETPARAM, &param, 2*sizeof(int));
#else
	res=wapid_ioctl(pap, P80211_IOCTL_SETWAPI_INFO, buffer, sizeof(buffer));
#endif
	return res;
}

