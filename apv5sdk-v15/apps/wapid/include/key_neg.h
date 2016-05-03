/*****************************************************************
* Copyright (c) 2001,西安西电捷通无线网络通信有限公司
* All rights reserved.
* 
* 文件名称：key_neg.c
* 摘    要：USK协商MSK通告相关函数的原型
* 
* 当前版本：1.1
* 作    者：王月辉yhwang@iwncomm.com
* 完成日期：2005年6月10日
*
* 取代版本：1.0 
* 原作者  ：王月辉yhwang@iwncomm.com
* 完成日期：2004年1月10日
******************************************************************/

#ifndef __KEY_NEG_H__
#define __KEY_NEG_H__


//#include "cert_auth.h"
//#include "prf.h"
//#include "ecc192_x962.h"
//#include "sha256_api.h"
#include "auth.h"
#include "structure.h"

/*单播Rekey中的单播密钥协商请求*/
void usk_rekey_handle(struct auth_sta_info_t *sta_info);
int msk_rekey_handle(apdata_info *pap);

/*单播密钥协商请求*/
void	usk_negotiation_req(struct auth_sta_info_t *sta_info);
/*单播密钥协商响应分组*/
int usk_negotiation_res(void *read_asue, int readlen,  struct auth_sta_info_t *sta_info);
/*单播密钥协商确认分组*/
void usk_negotiation_confirmation(struct auth_sta_info_t *sta_info);
/*组播通告*/
void msk_announcement_tx(struct auth_sta_info_t *sta_info, u8 *sendto_asue);
/*组播通告响应*/
int  msk_announcement_res(struct auth_sta_info_t *sta_info, u8 *read_asue, int readlen);
/*向所有STA发送组播通告*/
void send_msk_announcement_to_all(apdata_info *pap);
/*向Driver 发送组播通告结果*/
int notify_groupnotice_to_apdriver(apdata_info *pap);
/*ECDH算法初始化*/
int wai_initialize_ecdh(para_alg *ecdh);
int wai_copy_ecdh(para_alg *ecdh_a, para_alg *ecdh_b);
int wai_compare_ecdh(para_alg *ecdh_a, para_alg *ecdh_b);


#endif
