/*****************************************************************
* Copyright (c) 2001,西安西电捷通无线网络通信有限公司
* All rights reserved.
* 
* 文件名称：cert_auth.c
* 摘    要：证书鉴别过程相关函数定义
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
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <assert.h>
#include <linux/if_packet.h>
#include <linux/if_ether.h>

#include "cert_auth.h"
#include "proc.h"
#include "key_neg.h"
#include "typedef.h"
#include "pack.h"
#include "debug.h"
#include "raw_socket.h"
#include "wai_sta.h"
#include "alg_comm.h"
#include "certupdate.h"
#include "structure.h"
#include "auth.h"
#include "cert_auth.h"
#include "ecc_crypt.h"
#include "hmac.h"
#ifdef DOT11_WAPI_MIB
#include "dot11wapiMib.h"
#endif
static int wai_fixdata_cert_by_certificate(const struct cert_obj_st_t *cert_obj, 
										wai_fixdata_cert *fixdata_cert,
										u16 index);

/*使用证书鉴别与密钥管理套间是的BK导出方法*/
static int ap_certauthbk_derivation(struct auth_sta_info_t *sta_info);
/*使用PSK鉴别与密钥管理套间是的BK导出方法*/
static void ap_pskbkid_derivation(struct auth_sta_info_t *sta_info);
static int cmp_oid(unsigned char *in , int len);

/*比较证书公钥算法参数字段OID*/
static int cmp_oid(unsigned char *in , int len)
{
	unsigned char  pubkey_alg_para_oid_der[16] = {0x06, 0x09,0x2a,0x81,0x1c, 0xd7,0x63,0x01,0x01,0x02,0x01};

	if(len != 11) 
	{
		return -1;
	}
	if(memcmp(pubkey_alg_para_oid_der, in, 11)!=0)
	{
		return -2;
	}
	return 0;
}


/*create wai fix data of cert by inputing certificate data why@05/09/22*/
static int wai_fixdata_cert_by_certificate(const struct cert_obj_st_t *cert_obj, 
										wai_fixdata_cert *fixdata_cert,
										u16 index)
{
	if((fixdata_cert==NULL)||(cert_obj == NULL)) return -1;
	fixdata_cert->cert_flag =index; /*GBW*/
	fixdata_cert->cert_bin.data = cert_obj->user_cert_st.data;
	fixdata_cert->cert_bin.length = cert_obj->user_cert_st.length;
	return 0;

}

int wai_fixdata_id_by_ident(struct cert_bin_t  *cert_st,
								wai_fixdata_id *fixdata_id,
								u16 index)
{
	item_data_t  subject_name;
	item_data_t  issure_name;
	item_data_t  serial_no;
	const struct cert_obj_st_t *cert_obj = NULL;
	unsigned char *pos = 0;
	int rv = -2;
	
	if((!cert_st ) ||(!fixdata_id)){
		DPrintf("error at %s:%d\n", __func__, __LINE__);
		return -1;
	}
	
	/*according to the index, can call external objs if want to process asue id at the 
	same time*/
	
	cert_obj = get_cert_obj(index);
	if((cert_obj == NULL)
		||(cert_obj->get_subject == NULL)
		||(cert_obj->get_issuer == NULL)
		||(cert_obj->get_serial == NULL))	{
		DPrintf("error at %s:%d\n", __func__, __LINE__);
		return -4;
	}

	memset(&subject_name, 0, sizeof(subject_name));
	memset(&issure_name, 0, sizeof(issure_name));
	memset(&serial_no, 0, sizeof(serial_no));
	
	if((*cert_obj->get_subject)(cert_st, &subject_name) != 0){
		DPrintf("error at %s :%d\n", __func__, __LINE__);
		goto cleanup;
	}
	if((*cert_obj->get_issuer)(cert_st, &issure_name) != 0){
		DPrintf("error at %s :%d\n", __func__, __LINE__);
		goto cleanup;
	}
	if((*cert_obj->get_serial)(cert_st, &serial_no) != 0){
		DPrintf("error at %s :%d\n", __func__, __LINE__);
		goto cleanup;
	}
	if(subject_name.length + issure_name.length +serial_no.length >MAX_ID_DATA_LEN){
		DPrintf("error at %s :%d\n", __func__, __LINE__);
		goto cleanup;
	}
	pos = fixdata_id->id_data; 
	pos = MEMCPY(pos, subject_name.data, subject_name.length);
	pos = MEMCPY(pos, issure_name.data, issure_name.length);
	pos = MEMCPY(pos, serial_no.data, serial_no.length);
	fixdata_id->id_len = pos - fixdata_id->id_data;
	fixdata_id->id_flag = index;

	rv = 0;
cleanup:
	free_item(&subject_name,0);
	free_item(&issure_name,0);
	free_item(&serial_no,0);
	return rv;
}

/*构造并发送鉴别激活分组*/
int ap_activate_sta(struct auth_sta_info_t *sta_info)
{
	u8 sendto_asue[FROM_MT_LEN];
   	int sendtoMT_len = 0;
	int sendlen = 0;
	struct ethhdr eh;
	auth_active  auth_active_info;
	struct wapid_interfaces *wapid ;
	struct eloop_data *tmp_eloop ;
	apdata_info *pap;

	
	if((!sta_info) ||(!sta_info->pap)){
		DPrintf("error at %s :%d\n", __func__, __LINE__);
		return -1;
	}
	pap= sta_info->pap;
	
	wapid = (struct wapid_interfaces *)pap->user_data;

	if(wapid == NULL) {
		DPrintf("error at %s :%d\n", __func__, __LINE__);
		return -1;
	}

	tmp_eloop = (struct eloop_data *)wapid->eloop_save;

	if(tmp_eloop == NULL){
		DPrintf("error at %s :%d\n", __func__, __LINE__);
		return -1;
	}
	memset(&auth_active_info, 0, sizeof(auth_active));

	if((tmp_eloop->has_cert != 1)){
		DPrintf("error at %s :%d\n", __func__, __LINE__);
		return -1;
	}

	memcpy(eh.h_dest, sta_info->mac, WLAN_ADDR_LEN);
	
	if(sta_info->bksa.bk_update)
	{
		auth_active_info.flag |= WAI_FLAG_BK_UPDATE;
		memcpy(auth_active_info.ae_auth_flag, sta_info->bksa.ae_auth_flag, 32);
	}else{
		get_random(auth_active_info.ae_auth_flag , 32);
	}

	/*从AE的证书中取的ASU的身份字段*/
	if(wai_fixdata_id_by_ident(&tmp_eloop->cert_info.ap_cert_obj->asu_cert_st, 
							&(auth_active_info.asu_id),
							tmp_eloop->cert_info.config.used_cert)!=0)
	{
		DPrintf("error at %s :%d\n", __func__, __LINE__);
		return -1;
	}
	wapi_hex_dump("ASUID", auth_active_info.asu_id.id_data,auth_active_info.asu_id.id_len);
	wai_fixdata_cert_by_certificate(tmp_eloop->cert_info.ap_cert_obj,
								&(auth_active_info.ae_cer),
								tmp_eloop->cert_info.config.used_cert);
	wai_initialize_ecdh(&auth_active_info.ecdh);
	sendtoMT_len = pack_auth_active(&auth_active_info, sendto_asue,  FROM_MT_LEN);
	if((u16)sendtoMT_len == PACK_ERROR) 	
	{
		DPrintf("pack_auth_active PACK_ERROR");
		return -1;
	}
	/*设置flag和分组序号等*/
	*((u16 *)(sendto_asue + 8)) = htons(sta_info->ae_group_sc);
	sta_info->flag &= 0xFC;/*0xFC:11111100*//*先将0和1位清零*/
	sta_info->flag |= (auth_active_info.flag & 0x03);/*bit0:bk_update, bit1:pre_auth */
	memcpy(sta_info->bksa.ae_auth_flag , auth_active_info.ae_auth_flag, 32);
	wai_copy_ecdh(&sta_info->ecdh,&auth_active_info.ecdh);
	set_table_item(sta_info, SENDTO_STA, 0, sendto_asue, sendtoMT_len, AUTHACTIVE);

	DPrintf("[WAPID]:: Send '%s'  to STA("MACSTR")\n", wai_subtype_info[AUTHACTIVE], MAC2STR(sta_info->mac));
	sendlen = send_rs_data(sendto_asue, sendtoMT_len, &eh, pap);
	if(sendlen != sendtoMT_len) 
	{
		return -1;
	}
	sta_info->ae_group_sc += 1;
	if(sta_info->status == NO_AUTH)
	{
		sta_info->status = MT_WAITING_ACCESS_REQ;
	}
	return 0;
}

/*使用证书鉴别与密钥管理套间是的BK导出方法*/
static int ap_certauthbk_derivation(struct auth_sta_info_t *sta_info)
{
	const struct cert_obj_st_t *cert_obj = NULL;
	apdata_info *pap = NULL;
	int rv = -1;
	int index = 0;
	int orderlen = 0;

	struct key_bin_t tmpkey; 
	item_data_t  ecdhkey;
	item_data_t  kdbuffer;
	unsigned char *pos = NULL;
	
	memset(&tmpkey, 0, sizeof(tmpkey));
	memset(&ecdhkey, 0, sizeof(ecdhkey));
	memset(&kdbuffer, 0, sizeof(kdbuffer));

	if(!sta_info){
		DPrintf("[WAPID] :: err at %s:%d\n", __func__, __LINE__);
		goto cleanup;
	}
	pap= sta_info->pap;

	index = eloop.cert_info.config.used_cert;
	cert_obj= get_cert_obj(index);

	if((!cert_obj) || (cert_obj->genkey == NULL)){
		DPrintf("[WAPID] :: err at %s:%d\n", __func__, __LINE__);
		goto cleanup;
	}
	orderlen = cert_obj->user_key.privatekey.length;

	
	tmpkey.privatekey.data = get_buffer(orderlen);
	if(!tmpkey.privatekey.data){
		DPrintf("[WAPID] :: err at %s:%d\n", __func__, __LINE__);
		goto cleanup;;
	}
	tmpkey.privatekey.length = orderlen;
	tmpkey.publickey.data = get_buffer(cert_obj->user_key.publickey.length);
	if(!tmpkey.publickey.data){
		DPrintf("[WAPID] :: err at %s:%d\n", __func__, __LINE__);
		goto cleanup;;
	}
	tmpkey.publickey.length = cert_obj->user_key.publickey.length;
	
	if((*cert_obj->genkey)(tmpkey.publickey.data, &tmpkey.publickey.length,
					    tmpkey.privatekey.data, &tmpkey.privatekey.length)!=0){
		DPrintf("[WAPID] :: err at %s:%d\n", __func__, __LINE__);			    
		goto cleanup;;
	}

	ecdhkey.length = orderlen;
	ecdhkey.data = get_buffer(ecdhkey.length);
	if(!ecdhkey.data){
		goto cleanup;
	}
	if((*cert_obj->dh)(sta_info->asue_key_data.data, sta_info->asue_key_data.length,
			tmpkey.privatekey.data, tmpkey.privatekey.length,
			ecdhkey.data, &ecdhkey.length) !=0){
		DPrintf("[WAPID] :: err at %s:%d\n", __func__, __LINE__);	
		goto cleanup;;
	}
	wapi_hex_dump("STA publickey",sta_info->asue_key_data.data,sta_info->asue_key_data.length);
	wapi_hex_dump("AP privatekey",tmpkey.privatekey.data, tmpkey.privatekey.length);
	wapi_hex_dump("DH key",ecdhkey.data, ecdhkey.length);

	memcpy(sta_info->ae_key_data.data, tmpkey.publickey.data, tmpkey.publickey.length);
	sta_info->ae_key_data.length = tmpkey.publickey.length;
	
		
	/*计算BK*/
	kdbuffer.length =sizeof(sta_info->ae_nonce)
			    +sizeof(sta_info->asue_nonce)
			    +strlen(BK_TEXT); 
	kdbuffer.data = get_buffer(kdbuffer.length);
	if(!kdbuffer.data){
		kdbuffer.length = 0;
		goto cleanup;;
	}
	pos = kdbuffer.data;
	
	pos = MEMCPY(pos, sta_info->ae_nonce, sizeof(sta_info->ae_nonce));
	pos = MEMCPY(pos, sta_info->asue_nonce, sizeof(sta_info->asue_nonce));
	pos = MEMCPY(pos, BK_TEXT, strlen(BK_TEXT));
	wapi_hex_dump("KD_BK_buffer", kdbuffer.data, kdbuffer.length);
	
	KD_hmac_sha256(kdbuffer.data, kdbuffer.length, 
						ecdhkey.data, ecdhkey.length,
						kdbuffer.data, BK_LEN + CHALLENGE_LEN);
	wapi_hex_dump("KD_BK_out", kdbuffer.data, BK_LEN + CHALLENGE_LEN);
	
	memcpy(sta_info->bksa.bk, kdbuffer.data, BK_LEN);
	mhash_sha256(kdbuffer.data + BK_LEN, CHALLENGE_LEN, sta_info->bksa.ae_auth_flag);

	
	/*计算BKID*/
	memset(kdbuffer.data, 0, kdbuffer.length);
	memcpy(kdbuffer.data, pap->macaddr, WLAN_ADDR_LEN);
	memcpy(kdbuffer.data + WLAN_ADDR_LEN, sta_info->mac, WLAN_ADDR_LEN);
	wapi_hex_dump("KD_BKID_buffer", kdbuffer.data, ADDID_LEN);
	
	KD_hmac_sha256(kdbuffer.data, ADDID_LEN,
						sta_info->bksa.bk, BK_LEN, 
						sta_info->bksa.bkid, BKID_LEN);
	/*计算下一次鉴别标识*/
	wapi_hex_dump("in_BK",sta_info->bksa.bk,BK_LEN);
	wapi_hex_dump("out_BKID",sta_info->bksa.bkid,BKID_LEN);
	rv = 0;
cleanup:
	free_item(&tmpkey.privatekey, 0);
	free_item(&tmpkey.publickey, 0);
	free_item(&ecdhkey, 0);
	free_item(&kdbuffer, 0);
	return rv;
}

/*使用PSK鉴别与密钥管理套间时间的BKID导出方法*/
static void ap_pskbkid_derivation(struct auth_sta_info_t *sta_info)
{
	apdata_info *pap ;
	u8 input_text[ADDID_LEN] = {0,};
	if(sta_info == NULL) return;
	pap= sta_info->pap;
	/*计算BKID*/
	memcpy(input_text,pap->macaddr, 6);
	memcpy(input_text + 6, sta_info->mac, 6);
	memcpy(sta_info->bksa.bk , pap->psk.bk, 16);

	KD_hmac_sha256(input_text, ADDID_LEN, 
					pap->psk.bk, BK_LEN, 
					sta_info->bksa.bkid, BKID_LEN);
#if 1	
	DPrintf("Sta("MACSTR") BKID:\n", MAC2STR(sta_info->mac));
	wapi_hex_dump(NULL, sta_info->bksa.bkid, BKID_LEN);
#endif
}
/*使用PSK鉴别与密钥管理套间时间的BK导出方法*/
void ap_pskbk_derivation(apdata_info *pap)
{
	//u8 input_text[200] = "preshared key expansion for authentication and key negotiation";
	struct wapid_interfaces *wapid ;
	if(pap==NULL) return;
	wapid = pap->user_data;

	/*计算BK*/
	KD_hmac_sha256((u8*)PSK_TEXT, strlen(PSK_TEXT), 
					wapid->password, wapid->password_len, pap->psk.bk, BK_LEN);
#if 1	
	wapi_hex_dump("Pre-PSK", wapid->password, wapid->password_len);
	wapi_hex_dump("PSK_BK", pap->psk.bk, 16);
#endif
}
/*单播密钥导出方法*/
void ae_usk_derivation(struct auth_sta_info_t *sta_info, session_key_neg_response *key_res_buff)
{
	u8 		master_key[MASTER_KEY_LEN] = {0};
	u8		text[200] ;/*ADDID||N1||N2||"pairwise key expansion for unicast and additional keys and nonce"*/
	u8	 	uskid;
	int 		text_len;
	if((sta_info) == NULL || (key_res_buff == NULL)) return ;
	uskid = key_res_buff->uskid;//和AE比较??????
	memcpy(text, (u8 *)&(key_res_buff->addid), sizeof(wai_fixdata_addid));
	memcpy(text + sizeof(wai_fixdata_addid), sta_info->ae_nonce, 32);
	memcpy(text + sizeof(wai_fixdata_addid) + 32, sta_info->asue_nonce, 32);
	memcpy(text + sizeof(wai_fixdata_addid) + 32 + 32, USK_TEXT, strlen(USK_TEXT));
	text_len = sizeof(wai_fixdata_addid) + 32 + 32 + strlen(USK_TEXT);

	KD_hmac_sha256(text, text_len, sta_info->bksa.bk, BK_LEN,
					master_key, MASTER_KEY_LEN);
	memcpy(sta_info->usksa.usk[uskid].uek, master_key, PAIRKEY_LEN);
	memcpy(sta_info->usksa.usk[uskid].uck, master_key + PAIRKEY_LEN, PAIRKEY_LEN);
	memcpy(sta_info->usksa.usk[uskid].mck,master_key + 2*PAIRKEY_LEN, PAIRKEY_LEN);
	memcpy(sta_info->usksa.usk[uskid].kek, master_key + 3*PAIRKEY_LEN, PAIRKEY_LEN);
	wapi_hex_dump("Master key", master_key,96);
	mhash_sha256(master_key + 4*PAIRKEY_LEN, CHALLENGE_LEN, sta_info->ae_nonce);
}
/*鉴别初始化*/
int auth_initiate(asso_mt *passo_mt_info, apdata_info *pap)
{
	int akm_no;
	int active = 0;
	int psk = 0;
	int i = 0;
	u8 akm_haha1[4] = {0x00, 0x14, 0x72, 0x01};/*WAI证书鉴别和密钥管理*/
	u8 akm_haha2[4] = {0x00, 0x14, 0x72, 0x02};/*WAI共享密钥鉴别和密钥管理*/
	u8 *wie = passo_mt_info->wie;
	struct auth_sta_info_t *sta_info = NULL;

	if((passo_mt_info) == NULL || (pap == NULL)) return -1;

	sta_info = ap_add_sta(passo_mt_info, pap);

	if(sta_info == NULL)
	{
		DPrintf("error at %s :%d\n", __func__, __LINE__);
		return -1;
	}

	akm_no = ((wie[5]<<8)|wie[4]);
	for(i = 0; i < akm_no; i ++)
	{
		if(memcmp(akm_haha1, wie + 6 + i * 4, 4) == 0){
			/*ASUE使用证书鉴别与密钥管理套间*/
			active = 1;
		}else if(memcmp(akm_haha2, wie + 6 + i * 4, 4) == 0){
			/*ASUE使用PSK鉴别与密钥管理套间*/
			psk = 1;
		}	
	}
	/*记录ASUE的IE*/
	sta_info->bksa.akm_no = akm_no;
	memcpy(sta_info->bksa.akm, wie + 6 , akm_no * 4);
	
	if(active)
	{
		/*发送鉴别激活*/
		DPrintf("[WAPID]:: sta auth mode is certificate mode\n");
		ap_activate_sta(sta_info);
	}
	else if (psk)
	{
		DPrintf("[WAPID]:: sta auth mode is Pre-psk  mode\n");
		/*导出BK*/
		ap_pskbkid_derivation(sta_info);
		/*发送单播密钥协商请求*/
		usk_negotiation_req(sta_info);
#ifdef DOT11_WAPI_MIB
		memcpy(pap->mib_config.BKIDUsed,sta_info->bksa.bkid, BKID_LEN);
#endif		
	}
#ifdef DOT11_WAPI_MIB
	memcpy(pap->mib_config.AuthenticationSuiteRequested,sta_info->bksa.akm, 4);
	sta_info->SATimeout=time(0) + pap->mib_config.ConfigSATimeout;
#endif

	return 0;
}


/*处理STA发送的帧
	a 接入鉴别请求分组
	b 密钥协商响应分组
	c 广播密钥通告响应分组
*/
int wapi_process_from_sta(u8 *read_asue, int readlen, u8 *mac, apdata_info *pap)
{
	int sendtoAS_len = 0;
	int sendlen = 0;
	int ret = 0;
	u8 buf[FROM_MT_LEN];
	u8 packID;
	u8  asue_group_sc;
	struct auth_sta_info_t *sta_info = NULL;
	struct wapid_interfaces *wapid ;
	struct eloop_data *tmp_eloop ;
	
	if((!pap) || (!read_asue) || (!mac)||(readlen < 12)) {
		DPrintf("error at %s :%d\n", __func__, __LINE__);
		return -1;
	}
	wapid = (struct wapid_interfaces *)pap->user_data;

	if(wapid == NULL){
		DPrintf("error at %s :%d\n", __func__, __LINE__);
		return -1;
	}
	
	tmp_eloop = (struct eloop_data *)wapid->eloop_save;

	if(tmp_eloop == NULL){
		DPrintf("error at %s :%d\n", __func__, __LINE__);
		return -1;
	}
	sta_info = ap_get_sta(mac, pap);

	if (sta_info == NULL)
	{
		DPrintf("error at %s :%d\n", __func__, __LINE__);
		return -1;
	}

	/* 获取WAI分组序号*/
	asue_group_sc = get_packet_group_sc(read_asue);
	
	/*分组序号错误,分组序号是递增的*/
	if(asue_group_sc < sta_info->asue_group_sc)
	{
		DPrintf("error at %s :%d\n", __func__, __LINE__);
		return -1;
	}
	
	packID = get_packet_sub_type(read_asue);

	if((packID < 1) || (packID >0x10))
	{
#ifdef DOT11_WAPI_MIB
		WAPI_MIB_STATS_INC(WAIFormatErrors);
#endif
		DPrintf("[WAPID] :: Receive Unknown WAI frame  from STA("MACSTR")\n", MAC2STR(sta_info->mac));		
		return -1;		
	}

	DPrintf("[WAPID]:: Receive '%s' from STA("MACSTR")\n", wai_subtype_info[packID], MAC2STR(sta_info->mac));		
	//wapi_hex_dump("data", read_asue, readlen);
	
	switch(packID)
	{
	case STA_ACESS_REQUEST:
		/*接入鉴别请求分组*/
		if(access_auth_req(sta_info, read_asue, readlen, buf, &sendtoAS_len) != 0)
		{
			DPrintf("error at %s :%d\n", __func__, __LINE__);
			return -1;
		}
		
		/*设置重发缓冲区*/
		set_table_item(sta_info, SENDTO_AS, 0, (u8 *)buf, sendtoAS_len, AP_AUTH_REQUEST);
		sta_info->sendinfo.timeout = 10;

		/*发送证书鉴别请求*/
    		if (tmp_eloop->as_udp_sk > 0) {
			sendlen = sendto(tmp_eloop->as_udp_sk, buf, sendtoAS_len,
					0, (struct sockaddr *)&(tmp_eloop->as_addr), 
					sizeof(struct sockaddr_in));

	    		if (sendlen != sendtoAS_len)
			{
				DPrintf("error at %s :%d\n", __func__, __LINE__);
				return -2;
			}
			DPrintf("[WAPID]:: Send '%s' to AS\n", wai_subtype_info[AP_AUTH_REQUEST]);
	    		sta_info->status = MT_WAITING_AUTH_FROM_AS;
		} else {
			sta_info->auth_result = 0;    
			sta_info->status = MT_AUTHENTICATED;
		}
		break;
		/*单播密钥响应分组*/	
	case USK_NEG_RESPONSE:
		/*密钥协商响应分组*/
		ret = usk_negotiation_res(read_asue, readlen, sta_info);
		if (ret != 0)
		{
			DPrintf("error at %s :%d\n", __func__, __LINE__);
			return -3;
		}

		if (sta_info->status == MT_SESSIONKEYING)
		{
			/*单播密钥Rekey的过程中发生了组播密钥Rekey*/
			sta_info->status = MT_AUTHENTICATED;
			sta_info->usksa.dynamic_key_used = sta_info->usksa.uskid;
			sta_info->usksa.usk[sta_info->usksa.uskid].valid_flag = 1;/*valid*/
			sta_info->usksa.usk[sta_info->usksa.uskid^0x01].valid_flag = 0;

			/*发送密钥协商确认分组*/	
			usk_negotiation_confirmation(sta_info);

			/*安装STA 的UEK UCK*/
			set_ucastkey(pap, sta_info);
			break;
		}
		else if(sta_info->status == MT_SESSIONGROUPING)
		{
			/*组播密钥Rekey的过程中发生了单播密钥Rekey*/
			sta_info->status  = MT_WAITING_DYNAMIC_GROUPING;
			sta_info->usksa.dynamic_key_used=  sta_info->usksa.uskid;
			sta_info->usksa.usk[sta_info->usksa.uskid].valid_flag = 1;/*valid*/
			sta_info->usksa.usk[sta_info->usksa.uskid^0x01].valid_flag = 0;
			usk_negotiation_confirmation(sta_info);
			set_ucastkey(pap, sta_info);
			/*restore dynamic key for msk retry*/
			sta_info->usksa.dynamic_key_used=sta_info->usksa.uskid^0x01;
		

			break;				
		}
		else if(sta_info->status == MT_WAITING_DYNAMIC_SESSION)
		{
			/*单播REKEY中的密钥协商响应分组*/
			/*change keyid?*/
			sta_info->status = MT_AUTHENTICATED;
			sta_info->usksa.dynamic_key_used = sta_info->usksa.uskid;
			sta_info->usksa.usk[sta_info->usksa.uskid].valid_flag = 0;
			sta_info->usksa.usk[sta_info->usksa.uskid^0x01].valid_flag = 1;
			usk_negotiation_confirmation(sta_info);
			set_ucastkey(pap, sta_info);
			break;
		}
		else if(sta_info->status == MT_WAITING_SESSION)
		{
			if(sta_info->auth_mode == AUTH_MODE)
			{
				/*鉴别过程中的密钥协商响应分组*/
				usk_negotiation_confirmation(sta_info);
				msk_announcement_tx(sta_info, (u8 *)buf);	
			}else{
				/*预鉴别过程中的密钥协商响应分组*/
				sta_info->status = MT_PRE_AUTH_OVER;
				//record current time and after 30 minutes it will be deleted
				sta_info->pre_auth_save_time = time(0);
			}
		}
		break;
	case MSK_RESPONSE:
			/*组播通告响应分组*/
		ret = msk_announcement_res(sta_info, read_asue, readlen);
		if (ret == 0)
		{
			if (sta_info->status == MT_GROUPNOTICEING)
			{
				/*组播Rekey 过程中的组播通告响应分组*/
				sta_info->status = MT_AUTHENTICATED;
				sta_info->usksa.dynamic_key_used = sta_info->usksa.uskid;
				pap->group_No--;//record group notice no.
				return(notify_groupnotice_to_apdriver(pap));
			} 
			else if(sta_info->status == MT_WAITING_GROUPING)
			{
				/*一次鉴别过程中的组播通告响应分组*/
				sta_info->status = MT_AUTHENTICATED;
				sta_info->usksa.dynamic_key_used = sta_info->usksa.uskid;
				//send_auth_result_to_driver(sta_info, TRUE);
				/*安装单播密钥*/
				set_ucastkey(pap, sta_info);
#ifdef DOT11_WAPI_MIB
				sta_info->SATimeout = 0;
				memcpy(pap->mib_config.AuthenticationSuiteSelected,sta_info->bksa.akm, 4);
				WAPI_MIB_STATS_SET(ControlledPortStatus,1);
#endif
			}
			else if(sta_info->status == MT_SESSIONGROUPING)
			{
				/*组播Rekey 的过程中发生了单播Rekey*/
				sta_info->status = MT_WAITING_DYNAMIC_SESSION;
				/*组播通告计数递减*/
				pap->group_No--;//record group notice no.
				return(notify_groupnotice_to_apdriver(pap));
			}
			else if(sta_info->status == MT_WAITING_DYNAMIC_GROUPING)
			{
				/*单播Rekey的过程发生了组播Rekey*/
				sta_info->status = MT_AUTHENTICATED;
				sta_info->usksa.dynamic_key_used = sta_info->usksa.uskid;
				pap->group_No--;//record group notice no.
				return(notify_groupnotice_to_apdriver(pap));
			}
		}
		break;
	default:
		DPrintf("[WAPID] :: error at %s:%d\n", __func__, __LINE__);
		break;
	}
	return 0;
}

/*处理AS发送的证书鉴别响应分组*/
int wapi_process_1_of_1_from_as(u8 *read_as, int readlen, apdata_info *pap)
{
	struct auth_sta_info_t *sta_info = NULL; 
	u8		packID;
	int hlen = sizeof(packet_head);
	int idlen = sizeof(wai_fixdata_addid);
	int auth_result = -3;

	if((pap == NULL) || (read_as == NULL)||(readlen < 12)) return -1;

	packID = get_packet_sub_type(read_as);
	
	if (packID != AP_AUTH_RESPONSE)
	{
#ifdef DOT11_WAPI_MIB
		WAPI_MIB_STATS_INC(WAIFormatErrors);
#endif
		return -1;/*报文错误*/
	}
	DPrintf("[WAPID]:: Receive '%s'  from AS\n", wai_subtype_info[AP_AUTH_RESPONSE]);
	auth_result = certificate_auth_res(&sta_info, read_as, readlen, pap);
	switch(auth_result)
	{
		case 0:
		case -1:
		case -2:
			access_auth_res(sta_info, read_as + hlen + idlen, readlen - hlen - idlen);
			break;
		case -3:
#ifdef DOT11_WAPI_MIB			
			WAPI_MIB_STATS_INC(WAIFormatErrors);
#endif
			break;
		case -4:
#ifdef DOT11_WAPI_MIB		
			WAPI_MIB_STATS_INC(WAIDiscardCounters);
#endif
			break;
		default:
			break;
	}

	if((auth_result == -1) ||(auth_result == -2  ))
	{
		usleep(10);
		notify_driver_disauthenticate_sta(sta_info, __func__, __LINE__);
		DPrintf("notify_driver_disauthenticate_sta");
	}
	else if(auth_result == 0)
	{
		/*构造并发送单播密钥协商请求*/
		usk_negotiation_req(sta_info);

	}
	return 0;
}


/*处理Driver中消息*/
void wapi_process_for_driver(unsigned char *readapdrv, int readlen, apdata_info *pap)
{
#define MIN_ASSOC_EVT_LEN  (4 + 6+2+16+24)
#define MIN_MSG_EVT_LEN  (4 + 6+2)
 	asso_mt *asso_mt_info = NULL;

	do{	
		if(readlen < MIN_MSG_EVT_LEN){
			break;
		}
		asso_mt_info = (asso_mt *)readapdrv;

		switch (asso_mt_info->type)
		{
			case STA_ASSOC_EVENT:
				/*STA关联成功消息*/
				DPrintf("[WAPDI]:: Receive assioation event\n");
				wapi_hex_dump("event", readapdrv,readlen);
				
				if(readlen < MIN_ASSOC_EVT_LEN){
					DPrintf("error at %s :%d\n", __func__, __LINE__);
					break;
				}
				
				/*鉴别初始化*/
				if(auth_initiate(asso_mt_info, pap) != 0){
					DPrintf("error at %s :%d\n", __func__, __LINE__);
					break;
				}
				break;

			case STA_AGING_EVENT:
				/*STA 超时消息*/
				DPrintf("[WAPID]:: Receive STA("MACSTR") aging event\n", MAC2STR(asso_mt_info->mac));
				wapi_hex_dump("event", readapdrv,readlen);

				/*STA超时处理*/
				sta_timeout_handle(asso_mt_info->mac, pap);
				break;
#ifdef DOT11_WAPI_MIB
			case UNICAST_REKEY_EVENT:
				DPrintf("[WAPID]:: Receive STA("MACSTR") urekey event\n", MAC2STR(asso_mt_info->mac));
				sta_usk_rekey_handle(asso_mt_info->mac, pap);
				break;

			case MULTI_REKEY_EVENT:
				DPrintf("[WAPID]:: Receive STA("MACSTR") mrekey event\n", MAC2STR(asso_mt_info->mac));
				sta_msk_rekey_handle(pap);
				break;
#endif
		
		}
	}while(0);
#undef MIN_ASSOC_EVT_LEN
#undef MIN_MSG_EVT_LEN
}


/*接入鉴别响应*/
int access_auth_res(struct auth_sta_info_t *sta_info,	
							u8 *temp,/*记录证书鉴别响应分组中的除了地址索引外的其他字段*/
							int temp_len )
{
	struct ethhdr eh;
	struct wapid_interfaces *wapid ;
	struct eloop_data *tmp_eloop ;
	apdata_info *pap ;
	packet_head head;
	wai_fixdata_flag flag;
	tsign	 sign;
	item_data_t sign_data;
	
	u8 sendto_asue[FROM_AS_LEN];
	int sendlen = 0;
	u16 sign_len_pos = 0;
	//u16 sign_val_len = 48;
	u16 offset = 0;

	if(sta_info == NULL) return -1;
	pap= sta_info->pap;

	if(pap == NULL) return -1;
	wapid = (struct wapid_interfaces *)pap->user_data;
	if(wapid == NULL) return -1;
	tmp_eloop = (struct eloop_data *)wapid->eloop_save;
	if(tmp_eloop == NULL) return -1;

	memset(&sign, 0, sizeof(sign));
	sign.length = sizeof(sign.data);
	
	memset(&sign_data, 0, sizeof(sign_data));
	memset((u8 *)&flag, 0, 1);
	memset((u8 *)&head, 0, sizeof(packet_head));
	head.version      = VERSIONNOW;
	head.type = WAI;
	head.sub_type= STA_ACCESS_RESPONSE;
	head.reserved     = RESERVEDDEF;
	head.data_len     = 0x0000;
	head.group_sc = sta_info->ae_group_sc;
	
	flag =  sta_info->flag & 0x03;/*bit0:bk_update, bit1:pre_auth */

	if(sta_info->flag & WAI_FLAG_AUTH_CERT)/*bit2  ASUE 要求验证AE证书*/
		flag |= WAI_FLAG_OPTION_BYTE;/*bit3*/

	offset = c_pack_packet_head(&head,sendto_asue,offset,FROM_AS_LEN);
	offset = c_pack_byte( flag,sendto_asue,offset,FROM_AS_LEN);
	offset = c_pack_32bytes((u8 *)&sta_info->asue_nonce,sendto_asue,offset,FROM_AS_LEN);
	offset = c_pack_32bytes((u8 *)&sta_info->ae_nonce,sendto_asue,offset,FROM_AS_LEN);
	offset = c_pack_byte(sta_info->auth_result,sendto_asue,offset,FROM_AS_LEN);
	offset = c_pack_byte_data(&sta_info->asue_key_data,sendto_asue,offset,FROM_AS_LEN);
	if(offset == PACK_ERROR) return -1;
	offset = c_pack_byte_data(&sta_info->ae_key_data,sendto_asue,offset,FROM_AS_LEN);
	if(offset == PACK_ERROR) return -1;
	/*AE身份*/
	offset = c_pack_word((u16)tmp_eloop->ae_id.id_flag,sendto_asue,offset,FROM_AS_LEN);
	offset = c_pack_word((u16)tmp_eloop->ae_id.id_len,sendto_asue,offset,FROM_AS_LEN);
	memcpy(sendto_asue + offset, tmp_eloop->ae_id.id_data, tmp_eloop->ae_id.id_len);
	offset += tmp_eloop->ae_id.id_len;
	
	/*ASUE身份*/
	offset = c_pack_word((u16)sta_info->asue_id.id_flag,sendto_asue,offset,FROM_AS_LEN);
	offset = c_pack_word((u16)sta_info->asue_id.id_len,sendto_asue,offset,FROM_AS_LEN);
	memcpy(sendto_asue + offset, sta_info->asue_id.id_data, sta_info->asue_id.id_len);
	//c_pack_htons_id(sendto_asue + offset);
	offset +=  sta_info->asue_id.id_len;

	if(flag & WAI_FLAG_OPTION_BYTE)
	{
		/*copy复合得证书验证结果*/
		memcpy(sendto_asue + offset, temp, temp_len);
		offset += temp_len;
	}
	
	/*计算AE签名*/
	/*对上面的数据签名，如何确定鉴别载荷里的长度值*/
	sign_data.length = offset - sizeof(packet_head);
	sign_data.data = sendto_asue + sizeof(packet_head);
	
	if((*tmp_eloop->cert_info.ap_cert_obj->sign)(
		tmp_eloop->cert_info.ap_cert_obj->user_key.privatekey.data,
		tmp_eloop->cert_info.ap_cert_obj->user_key.privatekey.length,
		sign_data.data, sign_data.length, sign.data, &sign.length)!=0){
		DPrintf("err in %s:%d\n", __func__, __LINE__);
		return -1;	
	}
	if(sign.length == 0)
	{
		DPrintf("err in %s:%d\n", __func__, __LINE__);
		return -1;
	}
	/*构造签名属性*********************************8*/
	*(sendto_asue + offset) = 1;	/*类型*/
	offset += 1;		
	sign_len_pos = offset;				
	offset += 2;/*length*/

	/*AE身份*/
	offset = c_pack_word((u16)tmp_eloop->ae_id.id_flag,sendto_asue,offset,FROM_AS_LEN);
	offset = c_pack_word((u16)tmp_eloop->ae_id.id_len,sendto_asue,offset,FROM_AS_LEN);
	memcpy(sendto_asue + offset, &tmp_eloop->ae_id.id_data, tmp_eloop->ae_id.id_len);
	offset += tmp_eloop->ae_id.id_len;
	
	/*签名算法,没有参数*/
	offset = c_pack_sign_alg(&pap->sign_alg, sendto_asue,offset,FROM_AS_LEN);
	offset = c_pack_word((u16)sign.length,sendto_asue,offset,FROM_AS_LEN);
	
	memcpy(sendto_asue + offset, sign.data, sign.length);
	offset += sign.length;

	/*设置签名属性长度*/
	sendto_asue[sign_len_pos] = ((offset - sign_len_pos - 2)>>8)&0xff;
	sendto_asue[sign_len_pos+1] = ((offset - sign_len_pos - 2))&0xff;
	/*构造签名属性*********************************8*/
	
	set_packet_data_len(sendto_asue,offset );
	//(*sendtoMT_len) = offset;
	
	/*设置重发缓冲区*/
	set_table_item(sta_info, SENDTO_STA, 0, sendto_asue, offset, STA_ACCESS_RESPONSE);
	memcpy(eh.h_dest, sta_info->mac, ETH_ALEN);
	DPrintf("[WAPID]:: Send '%s' to STA("MACSTR")\n", 
		wai_subtype_info[AP_AUTH_RESPONSE], MAC2STR(sta_info->mac));
	sendlen = send_rs_data(sendto_asue, offset, &eh, pap);
	sta_info->ae_group_sc += 1;
	return sendlen;
}

/*处理接入鉴别请求分组*/
int access_auth_req (struct auth_sta_info_t *sta_info, const u8 *read_asue, 
						int readlen, u8 *sendto_as, int *psendtoas_len)
{
	const struct cert_obj_st_t  *cert_obj = NULL;
	struct wapid_interfaces *wapid ;
	struct eloop_data *tmp_eloop ;
	sta_auth_request sta_auth_requ;
	apdata_info *pap;
	item_data_t vsign_data;
	item_data_t pubkey;
	u16	no_signdata_len = 0;
	u16	offset_mt=0;
	u16	cert_type;
	int res = -1;
	
	memset(&sta_auth_requ, 0, sizeof(sta_auth_request));
	memset(&vsign_data, 0, sizeof(vsign_data));
	memset(&pubkey, 0, sizeof(pubkey));


	if(sta_info== NULL){
		DPrintf("error at %s :%d\n", __func__, __LINE__);
		return -1;
	}

	pap = sta_info->pap;

	if(pap == NULL){
		DPrintf("error at %s :%d\n", __func__, __LINE__);
		return -1;
	}

	wapid = (struct wapid_interfaces *)pap->user_data;

	if(wapid == NULL){
		DPrintf("error at %s :%d\n", __func__, __LINE__);
		return -1;
	}
	tmp_eloop = (struct eloop_data *)wapid->eloop_save;
	if(tmp_eloop == NULL) return -1;
	
	
	
	switch (sta_info->status )
	{	
		case MT_WAITING_AUTH_FROM_AS:/*重传帧*/
			/*AE接收到接入鉴别请求分组并向ASU发送证书鉴别请求分组后，
				在证书鉴别请求分组超时时间里不对ASUE发送得接入鉴别
				请求分组进行处理*/
			return -1;
		default :
			break;
	}
	/*对接入鉴别请求分组解包*/
	offset_mt = unpack_sta_auth_request(&sta_auth_requ, read_asue,  readlen,&no_signdata_len);
	if( offset_mt == PACK_ERROR )	
	{
		DPrintf("error at %s :%d\n", __func__, __LINE__);
#ifdef DOT11_WAPI_MIB		
			WAPI_MIB_STATS_INC(WAIFormatErrors);
#endif	
		return -1;		
	}

	switch (sta_info->status )
	{
		case MT_WAITING_ACCESS_REQ:
			if(memcmp(sta_auth_requ.ae_auth_flag , sta_info->bksa.ae_auth_flag, 32)!=0
				||(sta_auth_requ.flag & 0x03) != (sta_info->flag & 0x03))
			{
				/*比较鉴别标示,分组中的标示字段策略域是否相同*/
				DPrintf("[WAPID]:: ae_auth_flag ,bk_update,pre_auth not same\n");
				wapi_hex_dump("Auth Flag From STA", sta_auth_requ.ae_auth_flag, 32);
				wapi_hex_dump("Auth Flag in AP", sta_info->bksa.ae_auth_flag, 32);
				DPrintf("[WAPID]:: current sta's flag.bk_update and pre auth= %d, saved =%d\n ",
					sta_auth_requ.flag & 0x03, sta_info->flag & 0x03);
#ifdef DOT11_WAPI_MIB		
			WAPI_MIB_STATS_INC(WAIDiscardCounters);
#endif						
				return -1;
			}
			break;
		
		default :
			if(memcmp(sta_auth_requ.ae_auth_flag, sta_info->bksa.ae_auth_flag,32) != 0)
			{
				/*AE没有发送鉴别激活分组*/
				DPrintf("[WAPID]:: AP not send active frame to STA("MACSTR"), sta Status is %d\n", MAC2STR(sta_info->mac),sta_info->status);
#ifdef DOT11_WAPI_MIB		
			WAPI_MIB_STATS_INC(WAIDiscardCounters);
#endif				
				return -1;
			}
			break;
	}
	/*比较AE身份*/
	if(memcmp((u8 *)&(sta_auth_requ.ae_id), (u8 *)&(tmp_eloop->ae_id), tmp_eloop->ae_id.id_len + 4) !=0)
	{
		DPrintf("[WAPID]:: AEID is not same\n");
		wapi_hex_dump("AEID from sta", (void *)&sta_auth_requ.ae_id, sta_auth_requ.ae_id.id_len + 4);
		wapi_hex_dump("AEID from sta", (void *)&tmp_eloop->ae_id,  tmp_eloop->ae_id.id_len + 4);
#ifdef DOT11_WAPI_MIB		
			WAPI_MIB_STATS_INC(WAIDiscardCounters);
#endif			
		return -1;
	}
	/*比较ECDH*/
	if(wai_compare_ecdh(&sta_auth_requ.ecdh, &sta_info->ecdh)!=0)
	{
	DPrintf("[WAPID]:: ecdh param is not same\n");

		DPrintf("ecdh param from STA:{\n");
		DPrintf("	flag=%d\n",sta_auth_requ.ecdh.para_flag);
		DPrintf("	param_len=%d\n",sta_auth_requ.ecdh.para_len);
		DPrintf("	data:\n");
		wapi_hex_dump(NULL, sta_auth_requ.ecdh.para_data, sta_auth_requ.ecdh.para_len);
		DPrintf("	}\n");
	
		DPrintf("ecdh param in AP:{\n");
		DPrintf("	flag=%d\n",sta_auth_requ.ecdh.para_flag);
		DPrintf("	param_len=%d\n",sta_auth_requ.ecdh.para_len);
		DPrintf("	data:\n");
		wapi_hex_dump(NULL, sta_auth_requ.ecdh.para_data, sta_auth_requ.ecdh.para_len);
		DPrintf("	}\n");
#ifdef DOT11_WAPI_MIB		
			WAPI_MIB_STATS_INC(WAIDiscardCounters);
#endif		
		return -1;
	}
	
	/*验证ASUE 签名*/
	vsign_data.data = (u8*)(read_asue + sizeof(packet_head));
	vsign_data.length = no_signdata_len - sizeof(packet_head);
	/*取证书的类型 1:X509, 2:GBW*/
	cert_type = sta_auth_requ.asue_cert.cert_flag;
	if(cert_type != 1)
	{
		/*None X509证书*/
		DPrintf("none X509 certificate\n");
		return -2;
	}
	/*根据证书类型取证书中的公钥*/
	cert_obj = get_cert_obj(cert_type);

	if((cert_obj == NULL) ||(cert_obj->get_public_key == NULL))	{
		DPrintf("error at %s :%d\n", __func__, __LINE__);
		return -4;
	}
	
	if( (*cert_obj->get_public_key)(&sta_auth_requ.asue_cert.cert_bin, &pubkey)!=0){
		DPrintf("error at %s :%d\n", __func__, __LINE__);
		goto errexit;
	}
		
	
#if 1
	wapi_hex_dump("STA Publickey", pubkey.data, pubkey.length);
#endif
	/*验证证书*/
	res = (*cert_obj->verify)(pubkey.data, pubkey.length,
							vsign_data.data,vsign_data.length,
							sta_auth_requ.asue_sign.sign_data.data, 
							sta_auth_requ.asue_sign.sign_data.length);
	if(!res)
	{
		DPrintf("Verify ASUE sign  error\n");
#ifdef DOT11_WAPI_MIB		
			WAPI_MIB_STATS_INC(WAISignatureErrors);
#endif	
		res = -3;//bug:20090811
		goto errexit;
	}
	
	/*copy 密钥数据*/
	memcpy(sta_info->asue_nonce , sta_auth_requ.asue_challenge, CHALLENGE_LEN);
	memcpy(sta_info->asue_key_data.data, sta_auth_requ.key_data.data, sta_auth_requ.key_data.length);
	sta_info->asue_key_data.length = sta_auth_requ.key_data.length;

	/*根据证书生成身份*/
	wai_fixdata_id_by_ident(&sta_auth_requ.asue_cert.cert_bin, &(sta_info->asue_id),cert_type);

	/*记录ASUE 证书的hash值，用以和证书鉴别响应中的证书做对比*/
	mhash_sha256(sta_auth_requ.asue_cert.cert_bin.data, 
				sta_auth_requ.asue_cert.cert_bin.length,
				sta_info->asue_cert_hash);

	/*记录flag的比特2*/
	sta_info->flag &= 0xFB;/*0xFB:11111011*//*先将2位清零*/
	sta_info->flag |= sta_auth_requ.flag & 0x04;/*0x04:00000100*//*bit2:auth_cert*/

	/*根据本地鉴别策略构造证书鉴别请求发往ASU*/
	certificate_auth_req(&sta_auth_requ, sta_info, sendto_as, psendtoas_len);

	/*good negoration response and not nessary to resend negoration require */
	/*释放ASUE重发缓冲区*/
	reset_table_item(sta_info);
	res = 0;
errexit:
	free_item(&pubkey, 0);
	/*释放存放公钥和证书内存*/
	return res;
}

int certificate_auth_req (const struct _sta_auth_request *sta_auth_requ, 
							struct auth_sta_info_t *sta_info,
							u8 *sendto_as, int *psendtoas_len) /*本地策略默认是通过ASU鉴别证书*/
{
	u16 offset=0;
	packet_head head;
	struct wapid_interfaces *wapid ;
	struct eloop_data *tmp_eloop ;
	struct cert_obj_st_t *ap_cert_obj ;
	apdata_info *pap;
	int i;

	if(sta_info== NULL) return -1;
	pap= sta_info->pap;
	if(pap == NULL) return -1;
	wapid = (struct wapid_interfaces *)pap->user_data;
	if(wapid == NULL) return -1;
	tmp_eloop = (struct eloop_data *)wapid->eloop_save;
	if(tmp_eloop == NULL) return -1;
	ap_cert_obj =tmp_eloop 	->cert_info.ap_cert_obj;
	
	
	memset((u8 *)&head, 0, sizeof(packet_head));
	head.version      = VERSIONNOW;
	head.type =  WAI;
	head.sub_type= AP_AUTH_REQUEST;
	head.reserved     = RESERVEDDEF;
	head.data_len     = 0x0000;
	head.group_sc	= 1;
	head.frame_sc = 0;

	/*构造分组头*/
	offset = c_pack_packet_head(&head,sendto_as,offset,FROM_MT_LEN);

	/*构造ADDID*/
	offset = pack_mac(pap->macaddr, sendto_as, offset);
	offset = pack_mac(sta_info->mac, sendto_as, offset);
	
	/*构造AE和ASUE的挑战*/
	get_random(sta_info->ae_nonce , CHALLENGE_LEN);
	smash_random(sta_info->ae_nonce, CHALLENGE_LEN);
	offset = c_pack_32bytes(sta_info->ae_nonce, sendto_as, offset, FROM_MT_LEN);
	offset = c_pack_32bytes(sta_info->asue_nonce, sendto_as, offset, FROM_MT_LEN);

	/*ASUE certificate*/
	offset = c_pack_word((u16)sta_auth_requ->asue_cert.cert_flag,sendto_as ,offset,FROM_MT_LEN);
	offset = c_pack_word((u16)sta_auth_requ->asue_cert.cert_bin.length,sendto_as ,offset,FROM_MT_LEN);
	memcpy(sendto_as + offset, sta_auth_requ->asue_cert.cert_bin.data, sta_auth_requ->asue_cert.cert_bin.length);
	offset += sta_auth_requ->asue_cert.cert_bin.length;

	/*AE certificate*/
	//DPrintf("process_accessrequest:cert_index=%d\n",tmp_eloop->cert_info.config.used_cert);
	offset = c_pack_word((u16)tmp_eloop->cert_info.config.used_cert, sendto_as ,offset,FROM_MT_LEN);
	offset = c_pack_word((u16)ap_cert_obj->user_cert_st.length,sendto_as,offset,FROM_MT_LEN);
	memcpy(sendto_as + offset, ap_cert_obj->user_cert_st.data, ap_cert_obj->user_cert_st.length);
	offset += tmp_eloop->cert_info.ap_cert_obj->user_cert_st.length;

	if(sta_auth_requ->asu_id_list.identifier == 3)
	{
#if 0
		/*说明填充了ASUE信任的ASU列表*/
		DPrintf("push asue trust lists\n");
		memcpy(sendto_as + offset, (u8 *)&(sta_auth_requ->asu_id_list), sta_auth_requ->asu_id_list.id_no * 4 + 6);
		offset += sta_auth_requ->asu_id_list.id_no * 4 + 6;
#endif
		offset = c_pack_byte(sta_auth_requ->asu_id_list.identifier,
			               sendto_as, offset,FROM_MT_LEN);
		
		offset = c_pack_word(sta_auth_requ->asu_id_list.length,
		                                  sendto_as,offset,FROM_MT_LEN);
        offset = c_pack_byte(sta_auth_requ->asu_id_list.rev,
			               sendto_as, offset,FROM_MT_LEN);
			
        offset = c_pack_word(sta_auth_requ->asu_id_list.id_no,
		                                  sendto_as,offset,FROM_MT_LEN);
		for(i = 0; i < sta_auth_requ->asu_id_list.id_no; i ++)
        {
		   	//身份标识
			 offset = c_pack_word(sta_auth_requ->asu_id_list.id[i].id_flag,
	                                                   sendto_as,offset,FROM_MT_LEN);
			//身份长度
			 offset = c_pack_word(sta_auth_requ->asu_id_list.id[i].id_len,
		                                                sendto_as,offset,FROM_MT_LEN);
			//身份数据
			 offset = c_pack_nbytes((u8 *)&(sta_auth_requ->asu_id_list.id[i].id_data), 
			 	                                sta_auth_requ->asu_id_list.id[i].id_len, sendto_as,
			 	                                offset,FROM_MT_LEN);	
	    }
	}
	
	assert(offset !=PACK_ERROR);
	/*设置包的长度*/
	set_packet_data_len(sendto_as, offset);
	*psendtoas_len = offset;
	return 0;
}	

/*处理证书鉴别响应*/
int certificate_auth_res(struct auth_sta_info_t **sta_info0, u8 *read_as, int readlen , apdata_info *pap)//???
{
	struct auth_sta_info_t *sta_info = NULL;
	const struct cert_obj_st_t  *cert_obj = NULL;
	struct wapid_interfaces *wapid ;
	struct eloop_data *tmp_eloop ;
	ap_auth_response	auth_res_buff;
	wai_fixdata_id 	as_id;
	wai_fixdata_id 	ca_id;
	item_data_t  *pubkey = NULL;
	item_data_t   sign_data;
	
	char  *cert_invalid = "2";
	u8 errorcode_toASUE=0;
	u8 errorcode_toAE=0;
	u8  hash_value[32]={0,};

	int 	auth_error = 0;
	u16	no_signdata_len = 0;
	u16	no_signdata_len1 = 0;
	u16 	cert_type;
	u16 	offset = 0;	
	int use_ca_cert=0;

	if(pap == NULL) return -1;
	wapid = (struct wapid_interfaces *)pap->user_data;
	if(wapid == NULL) return -1;
	tmp_eloop = (struct eloop_data *)wapid->eloop_save;
	if(tmp_eloop == NULL) return -1;
	
	memset(&auth_res_buff, 0, sizeof(ap_auth_response));
	memset(&sign_data, 0, sizeof(sign_data));
	
	/*解包证书鉴别响应分组*/
	offset = unpack_ap_auth_response(&auth_res_buff,  read_as,  readlen, 
									&no_signdata_len, &no_signdata_len1);
	if(offset == PACK_ERROR) 
	{
		auth_error = -1;
		DPrintf("error at %s :%d\n", __func__, __LINE__);
		return -3;
	}
	/*查找STA*/
	sta_info = ap_get_sta(auth_res_buff.addid.mac2, pap);
	if(sta_info == NULL) 
	{
		DPrintf("error at %s :%d\n", __func__, __LINE__);
		return -4;
	}
	*sta_info0 = sta_info;
	/*检查ASUE的状态是否为等待证书鉴别响应*/
	if(sta_info->status != MT_WAITING_AUTH_FROM_AS)
	{
		DPrintf("process_accessauthentificate_ans: not MT_WAITING_AUTH_FROM_AS\n");
		DPrintf("error at %s :%d\n", __func__, __LINE__);
		return -4;
	}
	
	/*比较AE的挑战*/
	if(memcmp(sta_info->ae_nonce,auth_res_buff.cert_result.cert_result.ae_challenge,32)!=0)
	{
		DPrintf("AE Challenge  no match\n");
		wapi_hex_dump("From AS", auth_res_buff.cert_result.cert_result.ae_challenge,32);
		wapi_hex_dump("In AP", sta_info->ae_nonce,32);
		return -4;
	}
	
	/*检查是不是来自AE信任的服务器*/
	memset(&as_id, 0, sizeof(wai_fixdata_id ));
	if(wai_fixdata_id_by_ident(&tmp_eloop->cert_info.ap_cert_obj->asu_cert_st, 
							&(as_id), tmp_eloop->cert_info.config.used_cert)!=0)
	{
		DPrintf("error at %s :%d\n", __func__, __LINE__);
		return -4;
	}
	/*三证书 获取CA 身份*/
	memset(&ca_id, 0, sizeof(wai_fixdata_id ));
	if(tmp_eloop->cert_info.config.cert_mode==2)    //用CA证书验证
    {
	    if(wai_fixdata_id_by_ident(&tmp_eloop->cert_info.ap_cert_obj->ca_cert_st, 
							&(ca_id), tmp_eloop->cert_info.config.used_cert)!=0)
	    {
		    return -4;
	    }
	}
	/*比较ASU的身份*/
	if((auth_res_buff.cert_result.ae_trust_asu_sign.id.id_len != as_id.id_len)||
		(memcmp(auth_res_buff.cert_result.ae_trust_asu_sign.id.id_data,
			as_id.id_data, as_id.id_len)!=0))
	{
          /*比较CA的身份－三证书*/
    	if((auth_res_buff.cert_result.ae_trust_asu_sign.id.id_len == ca_id.id_len)&&
		    (memcmp(auth_res_buff.cert_result.ae_trust_asu_sign.id.id_data,
			ca_id.id_data, ca_id.id_len)==0))
	    {	       
	        use_ca_cert=1;
	    }
        else        
        {    
		    DPrintf("error at %s :%d\n", __func__, __LINE__);
    		DPrintf("not trust as\n");
	    	wapi_hex_dump("ASID from AS", auth_res_buff.cert_result.ae_trust_asu_sign.id.id_data, 
		    	auth_res_buff.cert_result.ae_trust_asu_sign.id.id_len);
    		wapi_hex_dump("ASID in AP", as_id.id_data, as_id.id_len);
	    	return -4;
        }
	}
	
	/*HASH 鉴别响应分组中ASUE证书*/
	mhash_sha256(auth_res_buff.cert_result.cert_result.cert1.cert_bin.data, 
				auth_res_buff.cert_result.cert_result.cert1.cert_bin.length, hash_value);
	/*和当前ASUE 证书的hash做对比，检查是不是正确的ASUE证书*/
	if(memcmp(sta_info->asue_cert_hash, hash_value, 32)!=0)
	{
		DPrintf("[WAPID]:: receive auth response with mismatch asue cert\n");
		return -4;
	}
	
	/*检查签名算法OID*/
	if(cmp_oid(auth_res_buff.cert_result.ae_trust_asu_sign.alg.sign_para.para_data,
		auth_res_buff.cert_result.ae_trust_asu_sign.alg.sign_para.para_len)!=0){
		return -4;
	}
	/*验证AS签名*/
	sign_data.length = no_signdata_len1 - sizeof(packet_head) - sizeof(wai_fixdata_addid);
	sign_data.data = read_as + sizeof(packet_head) + sizeof(wai_fixdata_addid);

	cert_type = tmp_eloop->cert_info.config.used_cert;
	cert_obj = get_cert_obj(cert_type);
	if((cert_obj == NULL) ||(cert_obj->verify == NULL))
	{
		DPrintf("error at %s :%d\n", __func__, __LINE__);
		return -4;
	}
    if(use_ca_cert)    //用CA证书验证
    {
        pubkey = &tmp_eloop->cert_info.ap_cert_obj->ca_pubkey;
	    wapi_hex_dump("ca public_key",pubkey->data, pubkey->length);
	    if(!(*cert_obj->verify)(pubkey->data, pubkey->length,
							sign_data.data,sign_data.length,
							auth_res_buff.cert_result.ae_trust_asu_sign.sign_data.data, 
							auth_res_buff.cert_result.ae_trust_asu_sign.sign_data.length))
	    {
		    DPrintf("CA sign error!!!\n");
    		errorcode_toASUE = CERTIFSIGNERR;
	    	auth_error = -1;
#ifdef DOT11_WAPI_MIB		
		WAPI_MIB_STATS_INC(WAISignatureErrors);
#endif				
    		return -4;
	    }
        /*
         * XXX Do we need to free this buffer for memory leakage?
         */
#if 0	    
        free_buffer(pubkey, sizeof(tkey));
#endif
    }
    else {          //用asu证书验证
        pubkey = &tmp_eloop->cert_info.ap_cert_obj->asu_publickey;

	    wapi_hex_dump("asu public_key",pubkey->data, pubkey->length);
    	if(!(*cert_obj->verify)(pubkey->data, pubkey->length,
							sign_data.data,sign_data.length,
							auth_res_buff.cert_result.ae_trust_asu_sign.sign_data.data, 
							auth_res_buff.cert_result.ae_trust_asu_sign.sign_data.length))
	    {
		    DPrintf("ASU sign error!!!\n");
    		errorcode_toASUE = CERTIFSIGNERR;
	    	auth_error = -1;
#ifdef DOT11_WAPI_MIB		
		WAPI_MIB_STATS_INC(WAISignatureErrors);
#endif			
    		return -4;
	    }
        /*
         * XXX Do we need to free this buffer for memory leakage?
         */
#if 0	    
        free_buffer(pubkey, sizeof(tkey));
#endif
    }
	sta_info->auth_result = 0;
	errorcode_toASUE = auth_res_buff.cert_result.cert_result.auth_result1; 
	errorcode_toAE = auth_res_buff.cert_result.cert_result.auth_result2;

	DPrintf("[WAPID]:: STA  Certificate is %s \n", errorcode_toASUE==0?"valid":"invalid");
	DPrintf("[WAPID]:: AP  Certificate is %s \n", errorcode_toAE==0?"valid":"invalid");

	if(errorcode_toASUE)
	{
		switch(errorcode_toASUE) 
		{
		case CERTIFVALID:
			DPrintf("Cert valid!\n");
			auth_error = 0;
			sta_info->auth_result = 0;
			break;
		case CERTIFISSUERUNKNOWN:
			DPrintf("Cert Issuer unknow!\n");
			auth_error = -1;
			sta_info->auth_result = 1;
			break;
		case CERTIFUNKNOWNCA:
			DPrintf("Cert is based on untrust root cert!\n");
			auth_error = -2;
			sta_info->auth_result = 2;
			break;
		case CERTIFEXPIRED:
			DPrintf("Cert Expired!\n");
			auth_error = -2;
			sta_info->auth_result = 2;
			break;
		case CERTIFSIGNERR:
			DPrintf("Cert sign error!\n");
			auth_error = -2;
			sta_info->auth_result = 2;
			break;
		case CERTIFREVOKED:
			DPrintf("Cert Revoked!\n");
			auth_error = -2;
			sta_info->auth_result = 2;
			break;
		case CERTIFBADUSE:
			DPrintf("Cert not used legally!\n");
			auth_error = -2;
			sta_info->auth_result = 2;
			break;
		case CERTUNKNOWNREVOKESTATUS:
			DPrintf("Cert unknown revoke-status!\n");
			auth_error = -2;
			sta_info->auth_result = 2;
			break;
		case CERTUNKNOWNERROR:
			DPrintf("Cert unknown error!\n");
			auth_error = -2;
			sta_info->auth_result = 2;
			break;
		default:
			DPrintf("Other Error!\n");
			auth_error = -2;
			sta_info->auth_result = 2;
			break;
		}
		
		goto auth_err;
	}

	if(errorcode_toASUE == 0)
	{
#ifdef DOT11_WAPI_MIB
		u32 cur_time;
#endif
		/*生成BK和BKID和下一次证书鉴别过程的鉴别标识*/
		ap_certauthbk_derivation(sta_info);
#ifdef DOT11_WAPI_MIB
		cur_time=time(0);
		memcpy(pap->mib_config.BKIDUsed,sta_info->bksa.bkid, BKID_LEN);
		sta_info->BKLifeTimeout=cur_time + pap->mib_config.ConfigBKLifetime;
		sta_info->BKReauthTimeout=cur_time +
			pap->mib_config.ConfigBKLifetime*pap->mib_config.ConfigBKReauthThreshold/100;
#endif		
	}
#ifdef DOT11_WAPI_MIB	
	else
	{
		wapimib_trap_auth_invalid_ASUE_cert();
	}
#endif
	
	reset_table_item(sta_info);

	/*更新AE证书状态*/
	if(errorcode_toAE != 0)
	{
		save_cert_status(tmp_eloop->fileconfig, cert_invalid);
	}
	else
	{
		update_cert_status(tmp_eloop->fileconfig) ;
	}
	DPrintf("[WAPID]:: Certificate authenication completed!\n");
	return auth_error;
auth_err:
	//DPrintf("STA  unauthenticated \n");
 	sta_info->auth_result = 2;
	
#ifdef DOT11_WAPI_MIB		
	WAPI_MIB_STATS_INC(WAIAuthenticationResultFailures);
#endif		
	return auth_error;
}

#ifdef LSDK6
static int del_key(apdata_info *pap, const u8 *addr, int key_idx, int ismcast)
{
	struct ieee80211req_del_key wk;
	int ret;

	DPrintf("[WAPID]:: delete key %d) \n", key_idx);

	memset(&wk, 0, sizeof(wk));
	if (!ismcast) {
		memcpy(wk.idk_macaddr, addr, IEEE80211_ADDR_LEN);
		wk.idk_keyix = (u8) IEEE80211_KEYIX_NONE;
	} else {
		wk.idk_keyix = key_idx;
	}

	ret = set80211priv(pap, IEEE80211_IOCTL_DELKEY, &wk, sizeof(wk));
	if (ret < 0) {
		DPrintf("error at %s :%d\n", __func__, __LINE__);
	}

	return ret;
}


static int set_key( apdata_info *pap, const char *alg,
		const u8 *addr, int key_idx,
		const u8 *key, size_t key_len, int txkey, int ismcast)
{
#define	IEEE80211_KEY_XMIT	0x01	/* key used for xmit */
#define	IEEE80211_KEY_RECV	0x02	/* key used for recv */
#define	IEEE80211_KEY_DEFAULT	0x80	/* default xmit key */

	struct ieee80211req_key wk;
	u8 cipher;
	int ret = -1;


	memset(&wk, 0, sizeof(wk));
	
	if (strcmp(alg, "none") == 0)
		return del_key(pap, addr, key_idx, ismcast);

	DPrintf("[WAPID]:: %s: alg=%s  key_idx=%d\n",  __func__, alg, key_idx);

	if (strcmp(alg, "WPI-SMS4") == 0)
		cipher = IEEE80211_CIPHER_WAPI;
	else {
		printf("%s: unknown/unsupported algorithm %s\n", __func__, alg);
		return -1;
	}

	if (key_len > sizeof(wk.ik_keydata)) {
		printf("%s: key length %lu too big\n", __func__, (unsigned long) key_len);
		return -3;
	}

	memset(&wk, 0, sizeof(wk));
	wk.ik_type = cipher;
	wk.ik_flags = IEEE80211_KEY_RECV | IEEE80211_KEY_XMIT;
	
	if (ismcast) {
		wk.ik_keyix = key_idx;
		wk.ik_pad = key_idx;//add by rococo@20090730
		wk.ik_flags |= IEEE80211_KEY_DEFAULT;
		memset(wk.ik_macaddr, 0xFF, WLAN_ADDR_LEN);
	} else {
		wk.ik_keyix = IEEE80211_KEYIX_NONE;
		wk.ik_pad = key_idx;//add by rococo@20090730
		memcpy(wk.ik_macaddr, addr, WLAN_ADDR_LEN);

	}
	wk.ik_keylen = key_len;
	memcpy(wk.ik_keydata, key, key_len);

	ret = set80211priv(pap, IEEE80211_IOCTL_SETKEY, &wk, sizeof(wk));
	if (ret < 0) {
		DPrintf("[WAPID]:: %s: Failed to set key (addr "MACSTR	"key_idx %d alg '%s' key_len %d txkey %d)",
			   __func__, MAC2STR(wk.ik_macaddr), key_idx,  alg,  key_len, txkey);
	}
	return ret;
}

#endif //LSDK6

/*安装组播密钥*/
void set_mcastkey(apdata_info *pap, struct sta_msksa *msksa)
{

	u8 *pos = NULL;
	u8 ioctl_buffer[128] = {0,};

	if((pap == NULL) ||(msksa == NULL)) 
		return;
	pos = ioctl_buffer;
#ifndef LSDK6
	SET16(pos, P80211_PACKET_SETKEY); pos += 2;
	SET16(pos, (WLAN_ADDR_LEN + 2+2*MSK_LEN)); pos += 2;
	
	memset(pos, 0xFF, WLAN_ADDR_LEN);   pos += WLAN_ADDR_LEN;
	*pos =  0x1; pos += 1;
	*pos =  msksa->mskid; pos += 1;
#endif

	KD_hmac_sha256((u8 *)MSK_TEXT, strlen(MSK_TEXT), msksa->msk, MSK_LEN, pos, 2*MSK_LEN);

#ifndef LSDK6
	wapid_ioctl(pap, P80211_IOCTL_SETWAPI_INFO, ioctl_buffer, sizeof(ioctl_buffer));
#endif

#ifdef LSDK6
	set_key(pap, 
			"WPI-SMS4",
			NULL, msksa->mskid, 
			pos, 2*PAIRKEY_LEN, 1, 1);
#endif

	wapi_hex_dump("Set MEK", pos, MSK_LEN);
	wapi_hex_dump("Set MCK", pos+MSK_LEN, MSK_LEN);
	pap->msk_rekey_timer = time(NULL);
}

/*安装单播密钥*/
void set_ucastkey(apdata_info *pap,  struct auth_sta_info_t *sta_info)
{
	struct sta_usksa *usksa;
#ifndef LSDK6
	u8 *pos = NULL;
	u8 ioctl_buffer[128] = {0,};

	pos = ioctl_buffer;
#endif

	if((pap == NULL) ||(sta_info == NULL)) 
		return;
	usksa = &sta_info->usksa;
	
#ifndef LSDK6
	SET16(pos, P80211_PACKET_SETKEY); pos += 2;
	SET16(pos, (WLAN_ADDR_LEN+2+2* PAIRKEY_LEN)); pos += 2;
	memcpy( pos, sta_info->mac, WLAN_ADDR_LEN); pos += WLAN_ADDR_LEN;
	*pos = 1;	pos+=1;
	*pos = usksa->uskid; pos+=1;
	
	memcpy(pos, usksa->usk[usksa->uskid].uek , PAIRKEY_LEN); pos += PAIRKEY_LEN;
	memcpy(pos, usksa->usk[usksa->uskid].uck , PAIRKEY_LEN); pos += PAIRKEY_LEN;

	wapid_ioctl(pap, P80211_IOCTL_SETWAPI_INFO, ioctl_buffer, sizeof(ioctl_buffer));
#endif

#ifdef LSDK6
	set_key(pap, 
			"WPI-SMS4",
			sta_info->mac, usksa->uskid, 
			(u8 *)&usksa->usk[usksa->uskid], 2*PAIRKEY_LEN, 1, 0);
	sta_authed(sta_info->mac, 1, pap);
#endif
	wapi_hex_dump("Set UEK", usksa->usk[usksa->uskid].uek, 16);
	wapi_hex_dump("Set UCK", usksa->usk[usksa->uskid].uck, 16);
	/*rekey timer init 20090728*/
	sta_info->usk_rekey_timer = time(NULL);
	sta_info->rekeying = 0;
#ifdef TEST
	sta_deauth(sta_info->mac, IEEE80211_MLME_DEAUTH, pap);
#endif
}

