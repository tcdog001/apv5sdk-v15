/***************************************************************
* Copyright (c) 2001,西安西电捷通无线网络通信有限公司
* All rights reserved.
* 
* 文件名称：wai_sta.h
* 摘    要：管理sta信息记录表的相关函数的原型
* 
* 当前版本：1.1
* 作    者：王月辉yhwang@iwncomm.com
* 完成日期：2005年6月10日
*
* 取代版本：1.0 
* 原作者  ：王月辉yhwang@iwncomm.com
* 完成日期：2004年1月10日
***************************************************************/

#ifndef __WAI_STA_H__
#define __WAI_STA_H__
#define DEL_DEBUG
#include "auth.h"
struct auth_sta_info_t * get_sta(struct eloop_data *eloop, unsigned char *mac);
struct auth_sta_info_t * ap_add_sta(asso_mt *passo_mt_info,  apdata_info *pap);
struct auth_sta_info_t * ap_get_sta(unsigned char *mac, apdata_info *pap);
struct auth_sta_info_t * ap_get_sta_pos(u8 *mac, struct auth_sta_info_t *sta_info_table);
void sta_timeout_handle(u8 *mac, apdata_info *pap);
#ifdef DOT11_WAPI_MIB
void sta_usk_rekey_handle(u8 *mac, apdata_info *pap);
void sta_msk_rekey_handle(apdata_info *pap);
int set_msk_rekey_pkts(apdata_info *pap);
int set_usk_rekey_pkts(apdata_info *pap);
int update_usk_rekey_pkts(u8 *mac, apdata_info *pap );
#endif
int  wapid_ioctl(apdata_info  *pap, u16 cmd, void *buf, int buf_len);
int  set80211priv( apdata_info *pap, int op, void *data, int len);

int sta_deauth(u8 *addr, int reason_code, apdata_info *pap);
int sta_authed(u8 *addr, int authorized, apdata_info *pap);

/*重新初始化STA信息*/
void reset_sta_info(struct auth_sta_info_t *sta_info, apdata_info*pap);
void  notify_driver_disauthenticate_sta(struct auth_sta_info_t *sta_info, const char *func, int line);
/*清除重发缓冲区*/
void reset_table_item(struct auth_sta_info_t *sta_info);
void set_table_item(struct auth_sta_info_t *sta_info, u16 direction, u16 flag, 	u8*sendto_asue, int sendto_len,unsigned int subtype);
#endif

