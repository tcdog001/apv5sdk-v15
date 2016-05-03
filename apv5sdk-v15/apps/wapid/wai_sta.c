/***************************************************************
* Copyright (c) 2001,西安西电捷通无线网络通信有限公司
* All rights reserved.
* 
* 文件名称：wai_sta.c
* 摘    要：管理sta信息记录表的相关函数的实现
* 
* 当前版本：1.1
* 作    者：王月辉yhwang@iwncomm.com
* 完成日期：2005年6月10日
*
* 取代版本：1.0 
* 原作者  ：王月辉yhwang@iwncomm.com
* 完成日期：2004年1月10日
***************************************************************/

#include <time.h>
#include <errno.h>
#include <sys/ioctl.h>
 
#include "debug.h"
#include "alg_comm.h"
#include "wai_sta.h"
#include "pack.h"
#include "raw_socket.h"
#include "cert_auth.h"
#ifdef DOT11_WAPI_MIB
#include "dot11wapiMib.h"
#endif
extern int errno;
struct auth_sta_info_t * get_sta(struct eloop_data *eloop, unsigned char *mac)
{
	/*释放所有资源*/
	struct wapid_interfaces *interfaces = NULL;
	struct auth_sta_info_t * sta = NULL;
	
	if(eloop == NULL) return NULL;
	interfaces = eloop->vap_user;

	for(; interfaces; interfaces=interfaces->next){
		sta = ap_get_sta(mac, interfaces->wapid);
		if(sta != NULL)
			break;
	}
	return sta;
}

/*在STA记录表中查找STA*/
struct auth_sta_info_t * ap_get_sta(unsigned char *mac, apdata_info *pap)
{
	int i;
	struct auth_sta_info_t *sta_info = NULL;
	
	if((!pap) ||(!pap->sta_info) ||(!mac)){
		return NULL;
	}
	sta_info= pap->sta_info;

	for (i=0; i<MAX_AUTH_MT_SIMU; i++)
	{
		if (sta_info[i].status != NO_AUTH)
		{
			if((memcmp(sta_info[i].mac, mac, WLAN_ADDR_LEN)) == 0)
				break;
		}
	}
	return (i==MAX_AUTH_MT_SIMU ? NULL : &sta_info[i]);
}

/*在STA记录表中为找一个记录STA信息的位置*/
struct auth_sta_info_t *ap_get_sta_pos(u8 *mac, struct auth_sta_info_t *sta_info_table)
{
	int i = 0;
	int k = MAX_AUTH_MT_SIMU;
	for(i=0; i <MAX_AUTH_MT_SIMU; i++){
		 if(sta_info_table[i].status == NO_AUTH){
			k = i;
		}else if(memcmp((sta_info_table[i].mac), mac, WLAN_ADDR_LEN) == 0){
			k = i;	
			break;
		}
	}
	if(k<MAX_AUTH_MT_SIMU){
		return &sta_info_table[k];
	}else{
		return NULL;
	}
#if 0
	int i, k;
	k = -1;
	for (i=0; i<MAX_AUTH_MT_SIMU; i++)
	{
		if (sta_info_table[i].status != NO_AUTH)
		{
			if(memcmp((sta_info_table[i].mac), mac, WLAN_ADDR_LEN) == 0)
			{
				k = i;
				break;
			}
		}
		else
		{
			if (k == -1)
				k = i;
		}
	}
	
	return (k==-1? NULL:&sta_info_table[k]);
#endif	
}

/*向STA记录表添加STA的信息*/
struct auth_sta_info_t * ap_add_sta(asso_mt *passo_mt_info,  apdata_info *pap)
{
	struct auth_sta_info_t *sta_info = NULL;
	u8 ie_len = passo_mt_info->wie[1];
	
	sta_info = ap_get_sta_pos(passo_mt_info->mac, pap->sta_info);	

	if (sta_info  == NULL)
	{
		wapi_hex_dump("no position for", passo_mt_info->mac, 6);
		goto exit_error;
	}

	reset_sta_info(sta_info, pap);
	
	memcpy(sta_info->mac, passo_mt_info->mac, WLAN_ADDR_LEN);           
	sta_info->packet_type = STA_ASSOC_EVENT;		
	htonl_buffer(passo_mt_info->gsn, 16);
	memcpy(sta_info->gsn, passo_mt_info->gsn, 16);
	memcpy(sta_info->wie, passo_mt_info->wie, ie_len + 2);

#ifdef DOT11_WAPI_MIB
	wapimib_stats_init(&sta_info->mib_stats);
	memcpy(sta_info->mib_stats.StatsSTAAddress,passo_mt_info->mac, 6);
	
#endif
	
exit_error:
	return sta_info;
}

#ifdef DOT11_WAPI_MIB
int ap_get_sta_num(apdata_info *pap)
{
	int i;
	int num=0;
	struct auth_sta_info_t *sta_info = NULL;
	
	if((!pap)||(!pap->sta_info)){
		return num;
	}
	sta_info= pap->sta_info;

	for (i=0; i<MAX_AUTH_MT_SIMU; i++)
	{
		if (sta_info[i].status != NO_AUTH)
		{
			num++;
		}
	}
	return num;
}

struct auth_sta_info_t * ap_get_sta_by_index(int index, apdata_info *pap)
{
	int i;
	int num=0;
	struct auth_sta_info_t *sta_info = NULL;
	
	if((!pap) ||(!pap->sta_info) ){
		return NULL;
	}
	sta_info= pap->sta_info;

	for (i=0; i<MAX_AUTH_MT_SIMU; i++)
	{
		if (sta_info[i].status != NO_AUTH)
		{
			num++;
			if(num==index)
				break;
		}
	}
	return (i==MAX_AUTH_MT_SIMU ? NULL : &sta_info[i]);
}

void sta_usk_rekey_handle(u8 *mac, apdata_info *pap)
{
	struct auth_sta_info_t *sta_info;
	sta_info = ap_get_sta(mac, pap);
	if(sta_info != NULL)	
	{
		if(sta_info->rekeying ==1)
			return;
		
		if(sta_info->auth_mode == AUTH_MODE
			&& sta_info->status == MT_AUTHENTICATED)
		{
			usk_rekey_handle(sta_info);
		}
	}
	return ;
}

void sta_msk_rekey_handle( apdata_info *pap)
{

	if(pap->rekeying)
		return;

	if (pap->wai_policy &0x03)/*sms4*/	
	{
		msk_rekey_handle(pap);
		
	}
	return ;
}

int set_msk_rekey_pkts(apdata_info *pap)
{
	int res = 0;
#ifdef LSDK6
	int param[2];
	param[0] = P80211_IOCTL_WAPIREKEY_MSK;
	if (pap->mib_config.ConfigMulticastRekeyMethod ==REKEY_POLICY_PACKETBASED ||
		pap->mib_config.ConfigMulticastRekeyMethod ==REKEY_POLICY_TIMEPACKETBASED)	
		param[1] = pap->mib_config.ConfigMulticastRekeyPackets;
	else
		param[1] = 0;
	res=wapid_ioctl(pap, IEEE80211_IOCTL_SETPARAM, &param, sizeof(param));
#endif
	return res;
}


int set_usk_rekey_pkts(apdata_info *pap)
{
	int res = 0;
#ifdef LSDK6
	int param[2];
	param[0] = P80211_IOCTL_WAPIREKEY_USK;
	if (pap->mib_config.ConfigUnicastRekeyMethod ==REKEY_POLICY_PACKETBASED ||
		pap->mib_config.ConfigUnicastRekeyMethod ==REKEY_POLICY_TIMEPACKETBASED)	
		param[1] = pap->mib_config.ConfigUnicastRekeyPackets;
	else
		param[1] = 0;
	res=wapid_ioctl(pap, IEEE80211_IOCTL_SETPARAM, &param, sizeof(param));
#endif
	return res;
}


int update_usk_rekey_pkts(u8 *mac, apdata_info *pap )
{
	int res = 0;
#ifdef LSDK6
	int param[4];
	param[0] = P80211_IOCTL_WAPIREKEY_UPDATE;
	memcpy(&param[1],mac,6);
	res=wapid_ioctl(pap, IEEE80211_IOCTL_SETPARAM, &param, sizeof(int)+6);
#endif
	return res;
}

#endif

/*STA超时处理*/
void sta_timeout_handle(u8 *mac, apdata_info *pap)
{
	struct auth_sta_info_t *sta_info;
	
	sta_info = ap_get_sta(mac, pap);
	
	if(sta_info != NULL)	
	{
		if(sta_info->auth_mode == AUTH_MODE
			||(sta_info->auth_mode ==PRE_AUTH_MODE
			&&sta_info->status == MT_AUTHENTICATED))
		{
			/*清除重发缓冲区*/
			reset_table_item( sta_info);
			memset(sta_info, 0, sizeof(struct auth_sta_info_t));
			sta_info->status = NO_AUTH;
#ifdef DOT11_WAPI_MIB
			if(pap->mib_config.ConfigMulticastRekeyStrict)
			{
				sta_msk_rekey_handle(pap);
			}
#endif
		}
	}
	return ;
}

/*向driver发送消息,set key ,set wapi */
int  wapid_ioctl(apdata_info  *pap, u16 cmd, void *buf, int buf_len)
{
	int		result = 0;
	struct	iwreq  wreq;
	struct wapid_interfaces *wapid ;
	struct eloop_data *tmp_eloop ;
	
	if(pap == NULL) return -1;
	wapid = (struct wapid_interfaces *)pap->user_data;
	if(wapid == NULL) return -1;
	tmp_eloop = (struct eloop_data *)wapid->eloop_save;
	if(tmp_eloop == NULL) return -1;
	memset(&wreq, 0, sizeof(struct iwreq));

	DPrintf("WAPID->%s\n",wapid->identity);
	wapi_hex_dump("W2Driver ", buf,buf_len);
	strcpy(wreq.ifr_ifrn.ifrn_name, wapid->identity);
#ifdef LSDK6	
	if (buf_len < IFNAMSIZ) {
		memcpy(wreq.u.name, buf, buf_len);
	} else {
		wreq.u.data.pointer = (caddr_t)buf;
		wreq.u.data.length =  buf_len;
	}
#else
	wreq.u.data.pointer = (caddr_t)buf;
	wreq.u.data.length =  buf_len;
#endif

	result = ioctl(tmp_eloop->ioctl_fd, cmd, &wreq);
	
	if ( result < 0 ) 
	{
		fprintf(stderr, "in %s:%d return(%d) :  \"%s\"........... \n", __func__, __LINE__, (result),strerror(errno));
	}
	return result;
}

int set80211priv( apdata_info *pap, int op, void *data, int len)
{
#define	N(a)	(sizeof(a)/sizeof(a[0]))
	struct iwreq iwr;
	struct wapid_interfaces *wapid ;
	struct eloop_data *tmp_eloop ;
	
	if(pap == NULL) return -1;
	wapid = (struct wapid_interfaces *)pap->user_data;
	if(wapid == NULL) return -1;
	tmp_eloop = (struct eloop_data *)wapid->eloop_save;
	if(tmp_eloop == NULL) return -1;

	memset(&iwr, 0, sizeof(iwr));
	strcpy(iwr.ifr_name, wapid->identity);
	if (len < IFNAMSIZ) {
		/*
		 * Argument data fits inline; put it there.
		 */
		memcpy(iwr.u.name, data, len);
	} else {
		/*
		 * Argument data too big for inline transfer; setup a
		 * parameter block instead; the kernel will transfer
		 * the data for the driver.
		 */
		iwr.u.data.pointer = data;
		iwr.u.data.length = len;
	}

	if (ioctl(tmp_eloop->ioctl_fd, op, &iwr) < 0) {
		static const char *opnames[] = {
			"ioctl[IEEE80211_IOCTL_SETPARAM]",
			"ioctl[IEEE80211_IOCTL_GETPARAM]",
			"ioctl[IEEE80211_IOCTL_SETKEY]",
			"ioctl[SIOCIWFIRSTPRIV+3]",
			"ioctl[IEEE80211_IOCTL_DELKEY]",
			"ioctl[SIOCIWFIRSTPRIV+5]",
			"ioctl[IEEE80211_IOCTL_SETMLME]",
			"ioctl[SIOCIWFIRSTPRIV+7]",
			"ioctl[IEEE80211_IOCTL_SETOPTIE]",
			"ioctl[IEEE80211_IOCTL_GETOPTIE]",
			"ioctl[IEEE80211_IOCTL_ADDMAC]",
			"ioctl[SIOCIWFIRSTPRIV+11]",
			"ioctl[IEEE80211_IOCTL_DELMAC]",
			"ioctl[SIOCIWFIRSTPRIV+13]",
			"ioctl[IEEE80211_IOCTL_CHANLIST]",
			"ioctl[SIOCIWFIRSTPRIV+15]",
			"ioctl[IEEE80211_IOCTL_GETRSN]",
			"ioctl[SIOCIWFIRSTPRIV+17]",
			"ioctl[IEEE80211_IOCTL_GETKEY]",
		};
		op -= SIOCIWFIRSTPRIV;
		if ((0 <= op )&& ((unsigned)op < N(opnames)))
			perror(opnames[op]);
		else
			perror("ioctl[unknown???]");
		return -1;
	}
	return 0;
#undef N
}

/*解除链路验证*/
int sta_deauth(u8 *addr, int reason_code, apdata_info *pap)
{
	struct ieee80211req_mlme mlme;
	DPrintf("[WAPID]:: deauth STA("MACSTR")\n", MAC2STR(addr));
	mlme.im_op = IEEE80211_MLME_DEAUTH;
	mlme.im_reason = reason_code;
	memcpy(mlme.im_macaddr, addr, WLAN_ADDR_LEN);
	return set80211priv(pap, IEEE80211_IOCTL_SETMLME, &mlme, sizeof(mlme));
}

int sta_authed(u8 *addr, int authorized, apdata_info *pap)
{
	struct ieee80211req_mlme mlme;

	if(authorized)
		mlme.im_op = IEEE80211_MLME_AUTHORIZE;
	else
		mlme.im_op = IEEE80211_MLME_UNAUTHORIZE;
	mlme.im_reason = 0;
	memcpy(mlme.im_macaddr, addr, WLAN_ADDR_LEN);
	return set80211priv(pap, IEEE80211_IOCTL_SETMLME, &mlme, sizeof(mlme));
}



/*重新初始化STA信息*/
void reset_sta_info(struct auth_sta_info_t *sta_info, apdata_info *pap)
{
	reset_table_item(sta_info);
	memset(sta_info, 0, sizeof(struct auth_sta_info_t));
	sta_info->status = NO_AUTH;
	sta_info->usksa.usk[0].valid_flag = 0;/*无效的key*/
	sta_info->usksa.usk[1].valid_flag = 1;/*有效的key*/
	sta_info->ae_group_sc = 1;
	sta_info->pap = pap;
#ifdef DOT11_WAPI_MIB
	sta_info->SATimeout = 0;
	sta_info->BKLifeTimeout = 0;
	sta_info->BKReauthTimeout = 0;
#endif
}

void
 notify_driver_disauthenticate_sta(struct auth_sta_info_t *sta_info, const char *func, int line)
 {
	
	func = func; line = line;
	apdata_info *pap = sta_info->pap;
	/*解除链路认证*/
	DPrintf("notify_driver_disauthenticate_sta\n");
	sta_deauth(sta_info->mac, IEEE80211_MLME_DEAUTH, sta_info->pap);
	/*clear information about this STA in the auth table*/
	/*清除sta的信息*/
	reset_sta_info(sta_info, pap);
}

/*清除重发缓冲区*/
void reset_table_item(struct auth_sta_info_t *sta_info)
{
       sta_info->buf0.length= 0;
	sta_info->buf1.length = 0;
	if(sta_info->buf0.data)
	{
		free(sta_info->buf0.data);
		sta_info->buf0.data = NULL;
		sta_info->buf0.type = 0;
	}
	if(sta_info->buf1.data)
	{
		free(sta_info->buf1.data);
		sta_info->buf1.data = NULL;
		sta_info->buf0.type = 0;
	}
/*	memset(sta_info->buf0.data, 0 ,MAX_RESEND_BUF);
	memset(sta_info->buf1.data, 0 ,MAX_RESEND_BUF);*/	
	sta_info->sendinfo.cur_count = 0;
	sta_info->sendinfo.max_count = 0;
	sta_info->sendinfo.direction = SENDTO_STA;
 }
/*设置重发缓冲区*/
void set_table_item(struct auth_sta_info_t *sta_info, u16 direction, 
					u16 flag, u8*frame, int frame_len, 
					unsigned int subtype)
{
	/*Confirm  don't need retry, just clear the buffer for USK(data1)*/
	if (subtype ==USK_NEG_CONFIRM)
	{
		if(sta_info->buf1.data)
		{
			free(sta_info->buf1.data);
			sta_info->buf1.data = NULL;
			sta_info->buf0.type = 0;
			sta_info->buf1.length = 0;
		}
		return ;
	}
	
	// fill sendinfo. used to check timeout
	sta_info->sendinfo.send_time = time(0);
	sta_info->sendinfo.cur_count = 1;
	sta_info->sendinfo.max_count = MAX_RESEND_COUNT;
	sta_info->sendinfo.timeout = TIMEOUT;
	sta_info->sendinfo.direction = direction;


#ifdef DOT11_WAPI_MIB
	switch(subtype){
		case AUTHACTIVE:
		case AP_AUTH_REQUEST:
		case STA_ACCESS_RESPONSE:
			sta_info->sendinfo.max_count = 
				sta_info->pap->mib_config.ConfigCertificateUpdateCount;
			break;
		case USK_NEG_REQUEST:
		case USK_NEG_CONFIRM:
			sta_info->sendinfo.max_count = 
				sta_info->pap->mib_config.ConfigUnicastUpdateCount;
			break;
		case MSK_ANNOUNCE:
			sta_info->sendinfo.max_count = 
				sta_info->pap->mib_config.ConfigMulticastUpdateCount;
			break;
		}
#endif	

	if(flag == 0)
	{
		if(sta_info->buf0.data !=NULL)
		{
			free((void *)(sta_info->buf0.data));
			sta_info->buf0.data = NULL;
		}
		sta_info->buf0.data = malloc(frame_len);
		if(sta_info->buf0.data == NULL) return ;
		memset(sta_info->buf0.data, 0, frame_len);
		memcpy(sta_info->buf0.data, frame, frame_len);
		sta_info->buf0.length= frame_len;
		sta_info->buf0.type= subtype;
	}
	if(flag == 1)
	{
		if(sta_info->buf1.data !=NULL)
		{
			free((void *)(sta_info->buf1.data));
			sta_info->buf1.data = NULL;
		}
		sta_info->buf1.data = malloc(frame_len);
		if(sta_info->buf1.data == NULL) return;
		memset(sta_info->buf1.data, 0, frame_len);
		memcpy(sta_info->buf1.data, frame, frame_len);
		sta_info->buf1.length= frame_len;
		sta_info->buf1.type = subtype;
	}
}
									 

