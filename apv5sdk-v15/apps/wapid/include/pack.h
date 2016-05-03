/*****************************************************************
* Copyright (c) 2001,西安西电捷通无线网络通信有限公司
* All rights reserved.
* 
* 文件名称：pack.h
* 摘    要：打解包相关函数的原型
* 
* 当前版本：1.1
* 作    者：王月辉yhwang@iwncomm.com
* 完成日期：2005年6月10日
*
* 取代版本：1.0 
* 原作者  ：王月辉yhwang@iwncomm.com
* 完成日期：2004年1月10日
******************************************************************/

#ifndef _PACK_H
#define _PACK_H

#include "structure.h"
#include "typedef.h"

#define	GET16(frm, v) do { (v) = (((frm[0]) <<8) | (frm[1]))& 0xffff;} while (0)
#define	GET32(frm, v) do { (v) = (((frm[0]) <<24) |((frm[1]) <<16) |((frm[2]) <<8) |(frm[3]))& 0xffffffff;} while (0)
#define	SET16(frm, v) do{\
	(frm[0])=((v)>>8)&0xff;\
	(frm[1])=((v))&0xff;\
	}while(0)

#define	SET32(frm, v) do{\
	(frm[0])=((v)>>24)&0xff;\
	(frm[1])=((v)>>16)&0xff;\
	(frm[2])=((v)>>8)&0xff;\
	(frm[3])=((v))&0xff;\
	}while(0)

void htonl_buffer(void *buffer, int len_in_byte);

short  c_pack_byte(const u8 content,void* buffer,u16 offset,unsigned short bufflen);
short  c_pack_word(const u16 content,void* buffer,u16 offset,unsigned short bufflen);
short  c_pack_dword(const u32 content,void* buffer,u16 offset,unsigned short bufflen);
short  c_pack_16bytes(const u8* content,void* buffer,u16 offset,unsigned short bufflen);
short  c_pack_32bytes(const u8* content,void* buffer,u16 offset,unsigned short bufflen);
short pack_mac(const u8* pData, void* buffer, short offset);
short  c_pack_sign_alg(const wai_fixdata_alg *pSign_alg,void* buffer,u16 offset,unsigned short bufflen);
short  c_unpack_sign_alg(wai_fixdata_alg* p_sign_alg,const void* buffer,u16 offset,u16 bufflen);
short  c_pack_byte_data(const byte_data* pData,void* buffer,u16 offset,unsigned short bufflen);
short  c_pack_packet_head( packet_head *pHead,void* buffer,u16 offset,unsigned short bufflen);
short c_pack_packed_head(void *buffer, unsigned short bufflen);
short  c_unpack_word(u16* content, const void* buffer,u16 offset,	u16 bufflen);

u8  get_packet_sub_type(const void* buffer);
void  set_packet_data_len(void* buffer,const u16 the_len);
u16  get_packet_group_sc(const void* buffer);

	
/* 认证激活报文的打包模块，供AP调用 */
short  pack_auth_active(const auth_active* pContent,void* buffer,unsigned short bufflen);
short  unpack_head(packet_head* p_head,const void* buffer,unsigned short bufflen);
/* 接入认证请求报文解包函数，供sta调用 */
short  unpack_sta_auth_request(sta_auth_request *p_sta_auth_request,const void* buffer,u16 bufflen, u16 *no_signdata_len);
/* 证书认证响应报文解包函数，供AP调用 */
short  unpack_ap_auth_response(ap_auth_response *p_auth_response, 
										const void* buffer,u16 bufflen,
										u16 *no_signdata_len,u16 *no_signdata_len1);

/* 会话密钥协商响应报文解包函数，供响应方AP调用 */
short  unpack_ucastkey_neg_response(session_key_neg_response *p_session_key_neg_response,const void* buffer,u16 bufflen);
 /* 组播密钥响应报文的解包函数，供AP调用 */
short unpack_msk_announcement_res(groupkey_notice_response *p_groupkey_notice_response, const void* buffer, u16 bufflen);
#endif
