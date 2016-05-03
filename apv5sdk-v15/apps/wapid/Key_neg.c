/*****************************************************************
* Copyright (c) 2001,西安西电捷通无线网络通信有限公司
* All rights reserved.
* 
* 文件名称：key_neg.c
* 摘    要：USK协商MSK通告相关函数的实现
* 
* 当前版本：1.1
* 作    者：王月辉yhwang@iwncomm.com
* 完成日期：2005年6月10日
*
* 取代版本：1.0 
* 原作者  ：王月辉yhwang@iwncomm.com
* 完成日期：2004年1月10日
******************************************************************/

#include <assert.h>
#include <linux/if_packet.h>
#include <linux/if_ether.h>

#include "auth.h"
#include "debug.h"
#include "cert_auth.h"
#include "wai_sta.h"
#include "alg_comm.h"
#include "key_neg.h"
#include "sms4.h"
#include "hmac.h"
#ifdef DOT11_WAPI_MIB
#include "dot11wapiMib.h"
#endif
/*单播Rekey中的单播密钥协商请求*/
void usk_rekey_handle(struct auth_sta_info_t *sta_info)
{
	int sendtoMT_len = 0;
	int sendlen = 0;
	u16 offset= 0;
	u8 sendto_asue[FROM_MT_LEN];
	u8 keyid;
	session_key_neg_request  dynamic_sessionkey;
	struct ethhdr eh;
	wai_fixdata_flag flag;
	packet_head head;

	assert(sta_info);
	
	memset(&dynamic_sessionkey, 0,sizeof(session_key_neg_request));
	memcpy(eh.h_dest, sta_info->mac, ETH_ALEN);

	sta_info->status = MT_SESSIONKEYING;
	
	/*设置uskid*/
	sta_info->usksa.dynamic_key_used = sta_info->usksa.uskid;
	sta_info->usksa.uskid =sta_info->usksa.uskid^0x01;
	keyid = sta_info->usksa.uskid;	
	DPrintf("[WAPID]: STA("MACSTR") valid keyid = %d\n", MAC2STR(sta_info->mac), keyid);
	sta_info->usksa.usk[keyid].valid_flag = 0;/*invalid*/

	/*构造单播密钥协商请求包头*/
	memset((u8 *)&head, 0, sizeof(packet_head));
	head.version = VERSIONNOW;
	head.type = WAI;
	head.sub_type = USK_NEG_REQUEST;
	head.reserved = RESERVEDDEF;
	head.data_len     = 0x0000;
	head.frame_sc = 0x00;
	head.flag = 0x00;
	head.group_sc =  sta_info->ae_group_sc;
	
	/*确定使用哪个uskid，使用当前无效的那个，经过协商
	后使之有效，而另一个成为无效*/
	memset((u8 *)&flag, 0, 1);
	/*设置USK更新标识*/
	flag = WAI_FLAG_USK_UPDATE;
	/*打包包头*/
	offset = c_pack_packet_head(&head, sendto_asue, offset, FROM_MT_LEN);
	/*打包标志*/
	offset = c_pack_byte( flag, sendto_asue, offset, FROM_MT_LEN);
	/*打包bkid*/
	offset = c_pack_16bytes((u8 *)&sta_info->bksa.bkid, sendto_asue, offset, FROM_MT_LEN);
	/*打包uskid*/
	offset = c_pack_byte((u8)sta_info->usksa.uskid, sendto_asue, offset, FROM_MT_LEN);
	/*打包addid*/
	offset = pack_mac(sta_info->pap->macaddr, sendto_asue, offset);
	offset = pack_mac(sta_info->mac, sendto_asue, offset);
	/*打包AE的挑战*/
	offset = c_pack_32bytes(sta_info->ae_nonce, sendto_asue, offset, FROM_MT_LEN);
	/*设置包的长度*/
	set_packet_data_len(sendto_asue,offset);


	sendtoMT_len = offset;

	/*设置重发缓存区*/
	set_table_item(sta_info, SENDTO_STA, 1, sendto_asue, sendtoMT_len, USK_NEG_REQUEST);
	
	/*设置STA状态*/
	if(sta_info->status == MT_GROUPNOTICEING)
		sta_info->status = MT_SESSIONGROUPING;

	/*set used key id to avoid collision,that is when we send dynamic session require, 
	   and send group notice before we receive dynamic session response, we firstly  
	   receive dynamic session response later, at the same time we change dynamic session key, 
	   and check key,  secondly receive group notice response in which we check data with raw check
	   key ,not the one we changed*/
	/*发送数据*/
	DPrintf("[WAPID]:: USK rekey start, Send '%s'  to STA("MACSTR") \n", wai_subtype_info[USK_NEG_REQUEST], MAC2STR(sta_info->mac));
	sendlen = send_rs_data(sendto_asue, sendtoMT_len, &eh, sta_info->pap);
	if(sendlen != sendtoMT_len)
	{
		DPrintf("error at %s :%d\n", __func__, __LINE__);
		return;
	}
	sta_info->ae_group_sc+= 1;
	sta_info->rekeying = 1;
	return;	
}
/*==========================end============================*/

int msk_rekey_handle(apdata_info *pap)
{
	int ret = -1;

	if (pap == NULL)
		return ret;


	update_gnonce(pap->gnonce,0);
	wapi_hex_dump("AP GSN", (unsigned char *)pap->gnonce, 16);
	
	/*判断组播密钥通告标识是否溢出*/
	if(overflow(pap->gnonce) == 0)
	{
		/*should not happen */
		u8 mac[WLAN_ADDR_LEN] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF} ;
		/*与所有STA解除链路验证*/
		sta_deauth(mac, IEEE80211_MLME_DEAUTH, pap);
	}
	else
	{
		/* disable msk rekey timer*/
		//pap->msk_rekey_timer = 0;
		
		/*取随机数作为组播密钥*/
		get_random(pap->msksa.msk, MSK_LEN);
		
		/*将组播通告状态置为有效,表示正在进行组播通告*/
		pap->group_status = 1;
		/*给所有鉴别成功的STA发送组播通告*/
		send_msk_announcement_to_all(pap);
		if(pap->group_No>0)
			pap->rekeying = 1;
	}

	ret = 0;
	return ret;
}


/*单播密钥协商请求*/
void	usk_negotiation_req(struct auth_sta_info_t *sta_info)
{	
	int sendtoMT_len = 0;
	int sendlen = 0;
	int   i;

	big_data data_buff;
	u16 offset= 0;

	struct ethhdr eh;
	wai_fixdata_flag flag;
	packet_head head;
	
	u8 sendto_asue[FROM_MT_LEN];
	apdata_info *pap = sta_info->pap;
	
	memcpy(eh.h_dest, sta_info->mac, ETH_ALEN);
	
	memset(&data_buff, 0, sizeof(big_data));

	/*创建单播密钥请求报文*/
	memset((u8 *)&head, 0, sizeof(packet_head));
	head.version = VERSIONNOW;
	head.type = WAI;
	head.sub_type= USK_NEG_REQUEST;
	head.reserved = RESERVEDDEF;
	head.data_len     = 0x0000;
	head.frame_sc = 0x00;
	head.flag = 0x00;
	head.group_sc = sta_info->ae_group_sc;
	/*确定使用哪个uskid，使用当前无效的那个，经过协商
	后使之有效，而另一个成为无效*/
	for(i = 0; i < 2; i++)
	{
		if(sta_info->usksa.usk[i].valid_flag == 0 /*invalid*/)
			sta_info->usksa.uskid = i;
	}
	memset((u8 *)&flag, 0, 1); /*bit4 = 0*/
	sta_info->flag &= 0xEF;/*0xEF:11101111*//*先将4位清零*/
	sta_info->flag |= flag & WAI_FLAG_USK_UPDATE;/*0x10:00010000*//*bit4:usk_update*/

	offset = c_pack_packet_head(&head, sendto_asue, offset, FROM_MT_LEN);
	offset = c_pack_byte((u8) flag, sendto_asue, offset, FROM_MT_LEN);
	offset = c_pack_16bytes(sta_info->bksa.bkid, sendto_asue, offset, FROM_MT_LEN);
	offset = c_pack_byte((u8)sta_info->usksa.uskid , sendto_asue, offset, FROM_MT_LEN);
	offset = pack_mac(pap->macaddr, sendto_asue, offset);
	offset = pack_mac(sta_info->mac, sendto_asue, offset);
	get_random(sta_info->ae_nonce, CHALLENGE_LEN);
	offset = c_pack_32bytes(sta_info->ae_nonce, sendto_asue, offset, FROM_MT_LEN);
	set_packet_data_len(sendto_asue,offset);


	sendtoMT_len = offset;
	
	/*设置重发缓冲区*/
	set_table_item(sta_info, SENDTO_STA, 1, sendto_asue, sendtoMT_len, USK_NEG_REQUEST);
	DPrintf("[WAPID]:: Send '%s'  to STA("MACSTR")\n", wai_subtype_info[USK_NEG_REQUEST], MAC2STR(sta_info->mac));
	wapi_hex_dump("Data", sendto_asue, sendtoMT_len);
	sendlen = send_rs_data(sendto_asue, sendtoMT_len, &eh, pap);
	if(sendlen != sendtoMT_len)
	{
		DPrintf("error at %s :%d\n", __func__, __LINE__);
		return; 
	}
	sta_info->ae_group_sc += 1;/*分组序号+1*/
	sta_info->status = MT_WAITING_SESSION;/*设置asue状态*/
	return;	
}



/*单播密钥协商响应分组*/
int usk_negotiation_res(void *read_asue, int readlen,  struct auth_sta_info_t *sta_info)
{
	struct wapid_interfaces *tmp_wapid;
	struct eloop_data *tmp_eloop;
	session_key_neg_response   key_res_buff;
	apdata_info *pap = NULL;
	item_data_t hmac_data;
	u8	HMAC[HMAC_LEN] = {0,};
	u8	uskid;
	u16	err_pack = 0;

	if((!sta_info) ||(!sta_info->pap)){
		DPrintf("error at %s :%d\n", __func__, __LINE__);
		goto err;
	}

	memset(&hmac_data, 0 ,sizeof(hmac_data));
	
	pap = sta_info->pap;
	tmp_wapid = (struct wapid_interfaces *)pap->user_data;
	tmp_eloop = (struct eloop_data *)tmp_wapid->eloop_save;

	/*判断STA的状态*/
	if( (sta_info->status != MT_WAITING_SESSION) &&
		(sta_info->status != MT_SESSIONKEYING) &&
		(sta_info->status != MT_SESSIONGROUPING) &&
		(sta_info->status != MT_WAITING_DYNAMIC_SESSION))
	{
		DPrintf("current sta(" MACSTR ") status is %d\n", MAC2STR(sta_info->mac), sta_info->status);
		goto err;
	}
	memset(&key_res_buff, 0, sizeof(struct _session_key_neg_response));
	err_pack=unpack_ucastkey_neg_response(&key_res_buff, read_asue,readlen);

	if(err_pack == PACK_ERROR)  
	{
#ifdef DOT11_WAPI_MIB		
		WAPI_MIB_STATS_INC(WAIFormatErrors);
#endif			
		goto err;
	}
	uskid = key_res_buff.uskid;

	if(key_res_buff.flag & WAI_FLAG_USK_UPDATE)
	{
		/*检查当前有有效的USKSA 并且USKID所指USKSA无效*/
		if(sta_info->usksa.usk[uskid].valid_flag)
		{
			DPrintf("当前有有效的USKSA 并且USKID所指USKSA有效\n");
			goto err;
		}
	}
	/*比较AE的挑战*/
	if(memcmp(key_res_buff.ae_challenge , sta_info->ae_nonce, CHALLENGE_LEN) !=0)
	{
		DPrintf("AE Challenge  not same\n");
		wapi_hex_dump("Challenge from STA", key_res_buff.ae_challenge, CHALLENGE_LEN);
		wapi_hex_dump("Challenge in AP", sta_info->ae_nonce, CHALLENGE_LEN);
#ifdef DOT11_WAPI_MIB	
		wapimib_trap_keyneg_invalid_challenge();
#endif
		goto err;
	}
	memcpy(sta_info->asue_nonce , key_res_buff.asue_challenge, CHALLENGE_LEN);

	/*计算KD_HMAC_SHA256,导出usk*/
	ae_usk_derivation(sta_info, &key_res_buff);
		
	/*计算MIC,比较MIC*/
	hmac_data.data = (unsigned char *)read_asue + sizeof(packet_head);
	hmac_data.length = readlen - sizeof(packet_head) - HMAC_LEN;
	
	hmac_sha256(hmac_data.data, hmac_data.length, 
			sta_info->usksa.usk[uskid].mck, 16, 
			HMAC, HMAC_LEN);
	if(memcmp(HMAC, key_res_buff.mic, HMAC_LEN) != 0)
	{
		DPrintf("MAC is wrong\n");
		wapi_hex_dump("From STA", key_res_buff.mic, HMAC_LEN);
		wapi_hex_dump("computed by AP", HMAC, HMAC_LEN);
#ifdef DOT11_WAPI_MIB		
		WAPI_MIB_STATS_INC(WAIHMACErrors);
		wapimib_trap_keyneg_invalid_MIC();
#endif
		goto err;//?????
	}
	if((key_res_buff.flag & WAI_FLAG_USK_UPDATE )== 0)
	{
		u8 wie_len = *(key_res_buff.wie_asue+ 1);

		if(memcmp(key_res_buff.wie_asue,  sta_info->wie, wie_len + 2) != 0)
		{
			/*解除链路验证*/
			DPrintf("different WIE\n");
			wapi_hex_dump("WIE from STA", key_res_buff.wie_asue, wie_len + 2);
			wapi_hex_dump("WIE from STA association frame",sta_info->wie, wie_len+ 2);
			DPrintf("calling notify_driver_disauthenticate_sta\n");
#ifdef DOT11_WAPI_MIB		
			wapimib_trap_keyneg_invalid_WIE();
#endif			
			notify_driver_disauthenticate_sta(sta_info, __func__, __LINE__);
		}
	}
	sta_info->flag &= 0xEF;/*0xEF:11101111*//*先将4位清零*/
	sta_info->flag |= key_res_buff.flag & WAI_FLAG_USK_UPDATE;/*0x10:00010000*//*bit4:usk_update*/
	/*清除重发缓冲区*/
//	reset_table_item(sta_info); /*retry entry can't be reset, for it maybe in MSK rekeying*/
	return 0;
err:	
	DPrintf("Received a undesirable negoration response and discard it \n");
#ifdef DOT11_WAPI_MIB		
		WAPI_MIB_STATS_INC(WAIDiscardCounters);
#endif
	return -1;
}

/*单播密钥协商确认分组*/
void usk_negotiation_confirmation(struct auth_sta_info_t *sta_info)
{
	struct ethhdr eh;
	apdata_info *pap =NULL;
	wai_fixdata_flag flag;
	packet_head head;
	
	u16 	offset= 0;
	int sendtoMT_len = 0;
	int sendlen = 0;
	u8  sendto_asue[FROM_MT_LEN];
	u8  dynamic_key_used = 0;
	u8  mic[20];
	u8 wie_len = 0;

	if((!sta_info) ||(!sta_info->pap)){
		DPrintf("error at %s :%d\n", __func__, __LINE__);
		return ;
	}
	
	pap = sta_info->pap;
	dynamic_key_used = sta_info->usksa.dynamic_key_used;
	
	memset((u8 *)&head, 0, sizeof(packet_head));
	memcpy(eh.h_dest, sta_info->mac, ETH_ALEN);
	/*构造包头*/
	head.version = VERSIONNOW;
	head.type = WAI;
	head.sub_type= USK_NEG_CONFIRM;
	head.reserved = RESERVEDDEF;
	head.data_len     = 0x0000;
	head.frame_sc = 0x00;
	head.flag = 0x00;
	head.group_sc = sta_info->ae_group_sc;
	
	memset((u8 *)&flag, 0, 1);
	flag  = sta_info->flag & WAI_FLAG_USK_UPDATE;
	/*打包包头*/
	offset = c_pack_packet_head(&head, sendto_asue, offset, FROM_MT_LEN);
	/*打包标志*/
	offset = c_pack_byte((u8)flag, sendto_asue, offset, FROM_MT_LEN);
	/*打包bkid*/
	offset = c_pack_16bytes((u8 *)&sta_info->bksa.bkid, sendto_asue, offset, FROM_MT_LEN);
	/*打包有效的uskid*/
	offset = c_pack_byte(dynamic_key_used, sendto_asue, offset, FROM_MT_LEN);
	/*打包addid*/
	offset = pack_mac((u8 *)&pap->macaddr, sendto_asue, offset);
	offset = pack_mac(sta_info->mac, sendto_asue, offset);
	/*打包ASUE的挑战*/
	offset = c_pack_32bytes(sta_info->asue_nonce, sendto_asue, offset, FROM_MT_LEN);
	/*打包AE的WIE*/
	wie_len =*( pap->wie_ae + 1);
	memcpy(sendto_asue + offset, pap->wie_ae, wie_len + 2);
	offset += wie_len + 2;
	/*计算MIC*/
	hmac_sha256(sendto_asue + sizeof(packet_head), offset - sizeof(packet_head), 
		sta_info->usksa.usk[dynamic_key_used].mck, PAIRKEY_LEN, mic, HMAC_LEN);

	memcpy(sendto_asue + offset, mic, HMAC_LEN);
	offset += HMAC_LEN;
	/*设置包长度*/
	set_packet_data_len(sendto_asue,offset);
	assert(offset != PACK_ERROR);
	sendtoMT_len = offset;

	/*清除重发缓冲区*/
	set_table_item(sta_info, SENDTO_STA, 1, sendto_asue, sendtoMT_len, USK_NEG_CONFIRM);

	//auth_table[asue_no].status = MT_SESSIONGROUPING;
	/*set used key id to avoid collision,that is when we send dynamic session require, 
	   and send group notice before we receive dynamic session response, we firstly  
	   receive dynamic session response later, at the same time we change dynamic session key, 
	   and check key,  secondly receive group notice response in which we check data with raw check
	   key ,not the one we changed*/
	/*发数据*/
	DPrintf("[WAPID]:: Send '%s' to STA("MACSTR")\n", wai_subtype_info[USK_NEG_CONFIRM], MAC2STR(sta_info->mac));
	DPrintf("[WAPID]:: USK negotiation completed!\n");
	sendlen = send_rs_data(sendto_asue, sendtoMT_len, &eh, pap);
	if(sendlen != sendtoMT_len)
	{
		DPrintf("error at %s :%d\n", __func__, __LINE__);
		return;
	}
	sta_info->ae_group_sc += 1;
	return;	
}/*==========================end============================*/

/*构造组播通告分组*/
static short  pack_msk_announcement(struct auth_sta_info_t *sta_info, 
									u8 *sendto_asue, u8 stakey_flag, 
									BOOL if_multi_rekey)
{
	u8 *hmac_data = NULL;
	u8 *hmac_tar_data =NULL;
	u8 used_key = sta_info->usksa.dynamic_key_used;;
	u8 iv[16] = {0,};
	wai_fixdata_flag flag ;
	packet_head head;
	u16 offset = 0;
	u16	hmac_data_len = 0;
	apdata_info *pap = sta_info->pap;
	memset((u8 *)&flag, 0, 1);
	memset((u8 *)&head, 0, sizeof(packet_head));
	head.version      = VERSIONNOW;
	head.type = WAI;
	head.sub_type		= MSK_ANNOUNCE;
	head.reserved     = RESERVEDDEF;
	head.data_len     = 0x0000;
	head.frame_sc = 0x00;
	head.flag = 0x00;
	head.group_sc = sta_info->ae_group_sc;
	
	switch(stakey_flag) {
		case 0:/*bit5-0,bit6-0*/
			flag &= 0x9F ;/*10011111*/
			break;
		case 1:/*bit5-0,bit6-1*/
			flag &= 0xDF ;/*11011111*/
			break;
		case 2:/*bit5-1,bit6-0*/
			flag &= 0xBF ;/*10111111*/
			break;
		case 3:/*bit5-1,bit6-1*/
			flag &= 0xFF ;/*11111111*/
			break;
		default:
			break;
	}
	sta_info->flag &= 0x9F;/*0x9F:10011111*//*先将5和6位清零*/
	sta_info->flag |= flag & 0x60;/*0x60:01100000*//*bit5:stakey-session bit6:stakey-del*/
	
	offset = c_pack_packet_head(&head,sendto_asue,offset,FROM_MT_LEN);
	offset = c_pack_byte((u8)flag, sendto_asue, offset, FROM_MT_LEN);
	offset = c_pack_byte((u8)pap->msksa.mskid, sendto_asue, offset, FROM_MT_LEN);
	offset = c_pack_byte((u8)sta_info->usksa.dynamic_key_used, sendto_asue, offset, FROM_MT_LEN);
	offset = pack_mac(pap->macaddr, sendto_asue, offset);
	offset = pack_mac(sta_info->mac, sendto_asue, offset);
	if(if_multi_rekey == FALSE)
	{
		offset = c_pack_16bytes(sta_info->gsn, sendto_asue, offset, FROM_MT_LEN);
	}
	else{
		u8 gsn[16] = {0x5c,0x36,0x5c,0x36,0x5c,0x36,0x5c,0x36,
					   0x5c,0x36,0x5c,0x36,0x5c,0x36,0x5c,0x36};		/*组播数据序号*/
		offset = c_pack_16bytes(gsn, sendto_asue, offset, FROM_MT_LEN);
	}
	offset = c_pack_dword((u32)pap->gnonce[0], sendto_asue, offset, FROM_MT_LEN);
	offset = c_pack_dword((u32)pap->gnonce[1], sendto_asue, offset, FROM_MT_LEN);
	offset = c_pack_dword((u32)pap->gnonce[2], sendto_asue, offset, FROM_MT_LEN);
	offset = c_pack_dword((u32)pap->gnonce[3], sendto_asue, offset, FROM_MT_LEN);
	
	*(sendto_asue + offset) = MSK_LEN;/*密钥通告数据长度*/
	offset += 1;

	/*加密组播密钥*/
	memcpy(iv, (u8 *)pap->gnonce, MSK_LEN);
	htonl_buffer(iv, MSK_LEN);
	wpi_encrypt(iv, pap->msksa.msk,	MSK_LEN,
			sta_info->usksa.usk[used_key].kek,
			sendto_asue+offset);

	offset += MSK_LEN;

	/*计算MIC*/
	hmac_data = sendto_asue+sizeof(packet_head);
	hmac_data_len = offset - sizeof(packet_head);
	hmac_tar_data = (unsigned char *)sendto_asue + offset;
	hmac_sha256(hmac_data, hmac_data_len, 
			sta_info->usksa.usk[used_key].mck, PAIRKEY_LEN, 
			hmac_tar_data, HMAC_LEN);
	offset += HMAC_LEN;
	
	set_packet_data_len(sendto_asue,offset);
	return offset;
}
/*组播通告响应分组*/
int  msk_announcement_res(struct auth_sta_info_t *sta_info, u8 *read_asue, int readlen)
{
	apdata_info *pap = sta_info->pap;
	groupkey_notice_response  group_res_buff;
	u16	err_pack;
	u16 	hmac_data_len = 0;
	u8 	HMAC[20];
	u8 	*hmac_data = NULL;
	u8 	dynamic_key_used = sta_info->usksa.dynamic_key_used;


	
	/*检查ASUE的状态*/
	if((	sta_info->status !=MT_WAITING_GROUPING) && 
		(sta_info->status !=MT_GROUPNOTICEING)&& 
		(sta_info->status !=MT_SESSIONGROUPING) && 
		(sta_info->status !=MT_WAITING_DYNAMIC_GROUPING))
	{
		DPrintf("AP is not in a waiting group notice response\n");
		DPrintf("current sta's mac " MACSTR,MAC2STR(sta_info->mac));
		DPrintf("\n");
		DPrintf("current status = %d\n", sta_info->status );
		return -1;
	}
	
	memset(&group_res_buff, 0, sizeof(groupkey_notice_response));

	/*组播通告响应解包*/
	err_pack = unpack_msk_announcement_res(&group_res_buff, read_asue, readlen);

	if(err_pack == PACK_ERROR)  
	{
		DPrintf("unpack_groupkey_notice_response Error\n");
#ifdef DOT11_WAPI_MIB		
		WAPI_MIB_STATS_INC(WAIFormatErrors);
#endif		
		goto err;
	}

	/*计算MIC校验*/
	hmac_data = (unsigned char *)read_asue + sizeof(packet_head);
	hmac_data_len = readlen - sizeof(packet_head) - HMAC_LEN;
	hmac_sha256(hmac_data, hmac_data_len, 
			sta_info->usksa.usk[dynamic_key_used].mck, 16, HMAC, HMAC_LEN);

	/*与该分组中的MIC相比较*/	
	if(memcmp(HMAC, group_res_buff.mic, HMAC_LEN) !=0) 
	{
		DPrintf("Error Hmac\n");
		DPrintf("USKID= %d\n ",dynamic_key_used);
		wapi_hex_dump("STA MCK", sta_info->usksa.usk[dynamic_key_used].mck, 16);
		wapi_hex_dump("HMAC from STA",group_res_buff.mic, 20);
		wapi_hex_dump("HMAC computed",HMAC, 20);
#ifdef DOT11_WAPI_MIB		
		WAPI_MIB_STATS_INC(WAIHMACErrors);
#endif
		goto err;
	}

	/*比较标识,MSKID, USKID, ADDID, 组播通告标识*/
	if (	(group_res_buff.flag & 0x60)!= (sta_info->flag & 0x60)/*bit5 and bit 6*/||
		group_res_buff.notice_keyid!= pap->msksa.mskid ||
		group_res_buff.uskid != sta_info->usksa.dynamic_key_used||
		memcmp(group_res_buff.addid.mac1, pap->macaddr, 6) !=0 ||
		memcmp(group_res_buff.addid.mac2, sta_info->mac, 6) !=0 ||
		group_res_buff.g_nonce[0]!= pap->gnonce[0] ||
		group_res_buff.g_nonce[1]!= pap->gnonce[1] ||
		group_res_buff.g_nonce[2]!= pap->gnonce[2] ||
		group_res_buff.g_nonce[3]!= pap->gnonce[3]){
		DPrintf("[WAPID]:: Error msksa.mskid or Gnonce\n");
		DPrintf("	  	from sta		in AP\n"); 
		DPrintf("	  flag	%d	%d\n", group_res_buff.flag ,sta_info->flag);
		DPrintf("	  apmac	"MACSTR"	"MACSTR"\n", MAC2STR(group_res_buff.addid.mac1), 
			MAC2STR(pap->macaddr));
		DPrintf("	  stamac	"MACSTR"	"MACSTR"\n", MAC2STR(group_res_buff.addid.mac2), 
			MAC2STR(sta_info->mac));
	{
		int i = 0;
		for(i=0; i<4; i++)
			DPrintf("	  pn%d		%04X	%04X\n", i, group_res_buff.g_nonce[i], pap->gnonce[i]);
	}	
		goto err;
	}

	/*清除重发缓冲区*/
	reset_table_item(sta_info);
	DPrintf("[WAPID]:: MSK announcement completed!\n");
	DPrintf("[WAPID]:: OK!wapi Competed!!\n");
	return 0;

err:	
	printf("Received a undesirable group notice response and discard it!!\n\n");
	
#ifdef DOT11_WAPI_MIB		
		WAPI_MIB_STATS_INC(WAIDiscardCounters);
#endif
	return -1;
}

/*组播通告分组*/
void msk_announcement_tx(struct auth_sta_info_t *sta_info, u8 *sendto_asue)
{
	int sendtoMT_len = 0;
	int sendlen = 0;
	struct ethhdr eh;
	apdata_info *pap = sta_info->pap;
	
	memcpy(eh.h_dest, sta_info->mac, ETH_ALEN);

	/*构造组播通告分组*/
	sendtoMT_len = pack_msk_announcement(sta_info, sendto_asue, 0,FALSE);
	/*设置重发缓冲区*/
	set_table_item(sta_info, SENDTO_STA, 0, sendto_asue, sendtoMT_len, MSK_ANNOUNCE);
	/*发送数据*/
	DPrintf("[WAPID]:: Send '%s' to STA("MACSTR")\n", wai_subtype_info[MSK_ANNOUNCE], MAC2STR(sta_info->mac));
	sendlen = send_rs_data(sendto_asue, sendtoMT_len, &eh, pap);
	if(sendlen != sendtoMT_len)
	{
		DPrintf("error at %s :%d\n", __func__, __LINE__);
		return; 
	}
	sta_info->ae_group_sc += 1;
	sta_info->status = MT_WAITING_GROUPING;
	return;	
}

/*给所有鉴别成功的STA发送组播通告*/
void send_msk_announcement_to_all(apdata_info *pap)
{
	u8 sendto_asue[FROM_MT_LEN];
	int sendtoMT_len =0;
	int sendlen = 0;
	int i = 0;
	struct ethhdr eh;
	struct auth_sta_info_t *sta_info_table = pap->sta_info;
	
	pap->msksa.mskid = pap->msksa.mskid^0x01;
	pap->group_No = 0;
	for (i=0; i<MAX_AUTH_MT_SIMU; i++)
	{
		if (sta_info_table[i].status == MT_AUTHENTICATED
			||sta_info_table[i].status == MT_SESSIONKEYING)
		{
			/*构造组播通告分组*/
			sendtoMT_len = pack_msk_announcement(&sta_info_table[i], sendto_asue, 0, TRUE);

			memcpy(eh.h_dest, sta_info_table[i].mac, ETH_ALEN);
			/*发送*/
			DPrintf("[WAPID]:: Send '%s' to STA("MACSTR")\n", wai_subtype_info[MSK_ANNOUNCE], MAC2STR(sta_info_table[i].mac));
			sendlen = send_rs_data(sendto_asue, sendtoMT_len, &eh, pap);
			if(sendlen != sendtoMT_len)
			{
				DPrintf("error at %s:%d\n", __func__, __LINE__);
				return; 
			}
			
			if(sta_info_table[i].status == MT_AUTHENTICATED){
				sta_info_table[i].status = MT_GROUPNOTICEING;
			}else if(sta_info_table[i].status == MT_SESSIONKEYING){
				sta_info_table[i].status = MT_SESSIONGROUPING;
			}
			
			set_table_item(&sta_info_table[i], SENDTO_STA, 0, sendto_asue, sendtoMT_len, MSK_ANNOUNCE);
			pap->group_No++;
			sta_info_table[i].ae_group_sc += 1;
		}
	}
	/*如果没有STA鉴别成功*/
	if((pap->group_status ==1) && (pap->group_No==0))
	{
		DPrintf("[WAPID]:: tell driver the gkeyid although now there is no any STA associated with AP\n");
		/*安装组播密钥*/
		set_mcastkey(pap, &pap->msksa);
	}
	return;
}

/*检查组播更新结果*/
static int check_groupkey_response_result(struct auth_sta_info_t *sta_info)
{
	int i, k;
	for (i=k=0; i<MAX_AUTH_MT_SIMU; i++)
	{
		if (sta_info[i].status == MT_GROUPNOTICEING
			||sta_info[i].status == MT_SESSIONGROUPING
			||sta_info[i].status == MT_WAITING_DYNAMIC_GROUPING)
		{
			k = 1;	// some STA has noticed but not response
			break;
		}

	}
	return k;
}
/*向Driver通知组播更新结果*/
int notify_groupnotice_to_apdriver(apdata_info *pap)
{
	/*检查是否所有asue组播通告都已经完成*/
	if (check_groupkey_response_result(pap->sta_info) == 0)
	{
		/*安装组播密钥*/
		set_mcastkey(pap, &pap->msksa);
		pap->rekeying = 0;
	}
	return 0;
}
/*初始化ECDH算法参数*/
int wai_initialize_ecdh(para_alg *ecdh)
{
	char alg_para_oid_der[16] = {0x06, 0x09,0x2a,0x81,0x1c, 0xd7,0x63,0x01,0x01,0x02,0x01};
	
	memset((u8 *)ecdh, 0, sizeof(para_alg));
	ecdh->para_flag= 1;
	ecdh->para_len = 11;
	memcpy(ecdh->para_data, alg_para_oid_der, 11);
	return 0;
}

int wai_copy_ecdh(para_alg *ecdh_a, para_alg *ecdh_b)
{
	memset((u8 *)ecdh_a, 0, sizeof(para_alg));
	
	ecdh_a->para_flag = ecdh_b->para_flag;
	ecdh_a->para_len = ecdh_b->para_len;
	memcpy(ecdh_a->para_data, ecdh_b->para_data, ecdh_b->para_len);
	return 0;
}

int wai_compare_ecdh(para_alg *ecdh_a, para_alg *ecdh_b)
{
	if(ecdh_a == NULL || ecdh_b == NULL) {
		return -1;
	}else if((ecdh_a->para_flag!= ecdh_b->para_flag) || 
			(ecdh_a->para_len !=  ecdh_b->para_len)|| 
			(memcmp(ecdh_a->para_data, ecdh_b->para_data,  ecdh_b->para_len)!=0)){
		return -1;
	}
	return 0;
}

