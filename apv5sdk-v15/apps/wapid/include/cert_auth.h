/*****************************************************************
* Copyright (c) 2001,西安西电捷通无线网络通信有限公司
* All rights reserved.
* 
* 文件名称：cert_auth.c
* 摘    要：证书鉴别过程相关函数原型
* 
* 当前版本：1.1
* 作    者：王月辉yhwang@iwncomm.com
* 完成日期：2005年6月10日
*
* 取代版本：1.0 
* 原作者  ：王月辉yhwang@iwncomm.com
* 完成日期：2004年1月10日
******************************************************************/

#ifndef __CERT_AUTH_H__
#define __CERT_AUTH_H__


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <linux/if_packet.h>
#include <linux/if_ether.h>

#include "structure.h"
#include "auth.h"
#include "cert_info.h"
#include "key_neg.h"
#include "typedef.h"
//#include "prf.h"
#include "pack.h"
#include "debug.h"
#include "raw_socket.h"

void ap_pskbk_derivation(apdata_info *pap);

/*取证书中的颁发者名称,持有者名称和序列号*/
int wai_fixdata_id_by_ident(struct cert_bin_t *cert_st, wai_fixdata_id *fixdata_id, u16 index);
/*单播密钥导出方法*/
void ae_usk_derivation(struct auth_sta_info_t *sta_info, session_key_neg_response *key_res_buff);
/*鉴别初始化*/
int auth_initiate(asso_mt *passo_mt_info, apdata_info *pap);
/*构造并发送鉴别激活分组*/
int ap_activate_sta(struct auth_sta_info_t *sta_info);

/*处理STA发送的帧
	a 接入鉴别请求分组
	b 密钥协商响应分组
	c 广播密钥通告响应分组
*/
int wapi_process_from_sta(u8 *read_asue, int readlen, u8 *mac, apdata_info *pap);

/*处理AS发送的证书鉴别响应分组*/
int wapi_process_1_of_1_from_as(u8 *read_as, int readlen, apdata_info *pap);

/*接入鉴别响应*/
int access_auth_res(struct auth_sta_info_t *sta_info,	u8 *temp, int temp_len );

/*处理接入鉴别请求分组*/
int access_auth_req (struct auth_sta_info_t *sta_info, const u8 *read_asue, 
						int readlen, u8 *sendto_as, int *psendtoas_len);

/*构造证书鉴别请求"本地策略默认是通过ASU鉴别证书"*/
int certificate_auth_req (const sta_auth_request	*sta_auth_requ, 
							struct auth_sta_info_t *sta_info,
							u8 *sendto_as, int *psendtoas_len); 
	
/*处理证书鉴别响应*/
int certificate_auth_res(struct auth_sta_info_t **sta_info, u8 *read_as, int readlen , apdata_info *pap);

/*安装组播密钥*/
void set_mcastkey(apdata_info *pap, struct sta_msksa *msksa);
/*安装单播密钥*/
void set_ucastkey(apdata_info *pap,  struct auth_sta_info_t *sta_info);
/*处理Driver中消息*/
void wapi_process_for_driver(unsigned char *readapdrv, int readlen, apdata_info *pap);
int register_certificate(struct eloop_data *eloop);
#endif
