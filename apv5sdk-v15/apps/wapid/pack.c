/*****************************************************************
* Copyright (c) 2001,西安西电捷通无线网络通信有限公司
* All rights reserved.
* 
* 文件名称：pack.c
* 摘    要：打解包相关函数的实现
* 
* 当前版本：1.1
* 作    者：王月辉yhwang@iwncomm.com
* 完成日期：2005年6月10日
*
* 取代版本：1.0 
* 原作者  ：王月辉yhwang@iwncomm.com
* 完成日期：2004年1月10日
******************************************************************/

#include <memory.h>
#include <stdio.h>
#include <netinet/in.h>

#include "pack.h"
#include "typedef.h"
#include "structure.h"
#include "debug.h" 



inline static
void write_byte(void* buffer,u8 content,u16 site)
{
	*(((u8*)buffer)+site) = content;
}

inline static
void write_word(void* buffer,u16 content,u16 site)
{
	u8 *pos = (u8*)(buffer) + site;
	SET16(pos, content);
	//content = htons(content);memcpy ((u8*)buffer+site, &content, 2);
}

inline static
void write_dword(void* buffer,u32 content,u16 site)
{
	u8 *pos = (u8*)(buffer) + site;
	SET32(pos, content);
	//content = htonl(content);memcpy( (u8*)buffer+site, &content, 4);
}

inline static
u8 read_byte(const void* buffer,u16 site)
{
	u8 result = *(((u8*)buffer)+site);
	return result;
}

inline static
u16 read_word(const void* buffer,u16 site)
{
	u16 result;
	u8 *pos = (u8 *)buffer + site;
	GET16(pos, result);
	return result;
}

inline static
u32 read_dword(const void* buffer,u16 site)
{
	u32 result;
	u8 *pos = (u8 *)buffer + site;
	GET32(pos, result);
    	//memcpy(&result, (u8*)buffer+site, 4);    	result = ntohl(result);
	return result;
}

/* 获得包类型*/

/* 获得包子类型*/
u8  get_packet_sub_type(const void* buffer)
{
	static u16 site = 3;
	return read_byte(buffer,site);
}

/* 获得包序号*/
u16  get_packet_group_sc(const void* buffer)
{
	static u16 site = 8;
	return read_word(buffer,site);
}
/*设置包类型*/
void  set_packet_data_len(void* buffer,const u16 the_len)
{
	static u16 site = 6;
	write_word(buffer,the_len,site);
}

/*将字节串转按照长整数长度字节换成网络模式*/
void htonl_buffer(void *buffer, int len_in_byte)
{
	int i;
	unsigned int *data = (unsigned int *) buffer;
	int len = len_in_byte/sizeof(unsigned int);
	
	for (i=0; i<len; i++) data[i] = htonl(data[i]);
}

 short c_pack_nbytes(const u8* content, int n, void* buffer,u16 offset,unsigned short bufflen)
{
	const u8 *temp = content ;
	if( offset+n>bufflen ) return PACK_ERROR;
	memcpy(buffer+offset, temp, n);
	return offset+n;
}
short  c_pack_byte(const u8 content,void* buffer,u16 offset,unsigned short bufflen)
{
	if( offset+1>bufflen ) return PACK_ERROR;
	write_byte(buffer,content,offset);
	offset ++;
	return offset;
}

short  c_pack_word(const u16 content,void* buffer,u16 offset,unsigned short bufflen)
{
	if( offset+2>bufflen ) return PACK_ERROR;
	write_word(buffer,content,offset);
	offset += 2;
	return offset;
}

short  c_pack_dword(const u32 content,void* buffer,u16 offset,unsigned short bufflen)
{
	if( offset+4>bufflen ) return PACK_ERROR;
	write_dword(buffer, content,offset);
	offset +=4;
	return offset;
}
short  c_pack_16bytes(const u8* content,void* buffer,u16 offset,unsigned short bufflen)
{
	return  c_pack_nbytes(content, 16, buffer,offset, bufflen);
}
short  c_pack_32bytes(const u8* content,void* buffer,u16 offset,unsigned short bufflen)
{
	return  c_pack_nbytes(content, 32, buffer,offset, bufflen);
}

 short  c_pack_byte_data(const byte_data* pData,void* buffer,u16 offset,unsigned short bufflen)
{
	if( 	offset+pData->length+1 > bufflen) 
		return PACK_ERROR;

	offset = c_pack_byte((u8)pData->length,buffer,offset,bufflen);
	memcpy((u8*)buffer+offset,pData->data,pData->length);
	offset += pData->length;

	return offset;
}


short  c_pack_sign_alg(const wai_fixdata_alg *pSign_alg,void* buffer,u16 offset,unsigned short bufflen)
{
	if( offset+pSign_alg->alg_length + 2>bufflen ) return PACK_ERROR;
	
	offset = c_pack_word((u16)pSign_alg->alg_length,buffer,offset,bufflen);
	offset = c_pack_byte((u8)pSign_alg->sha256_flag,buffer,offset,bufflen);
	offset = c_pack_byte((u8)pSign_alg->sign_alg,buffer,offset,bufflen);
	offset = c_pack_byte((u8)pSign_alg->sign_para.para_flag,buffer,offset,bufflen);
	offset = c_pack_word((u16)pSign_alg->sign_para.para_len,buffer,offset,bufflen);
	memcpy(buffer + offset, pSign_alg->sign_para.para_data, pSign_alg->sign_para.para_len);
	offset += pSign_alg->sign_para.para_len;
	return offset;
}

short  c_pack_packet_head( packet_head *pHead,void* buffer,u16 offset,unsigned short bufflen)
{
	if( bufflen<sizeof(packet_head) ) 
		return PACK_ERROR;
	
	offset = c_pack_word(pHead->version,buffer,offset,bufflen);
	offset = c_pack_byte(pHead->type,buffer,offset,bufflen);
	offset = c_pack_byte(pHead->sub_type,buffer,offset,bufflen);
	offset = c_pack_word(pHead->reserved,buffer,offset,bufflen);
	offset = c_pack_word(pHead->data_len,buffer,offset,bufflen);
	offset = c_pack_word(pHead->group_sc,buffer,offset,bufflen);
	offset = c_pack_byte(pHead->frame_sc,buffer,offset,bufflen);
	offset = c_pack_byte(pHead->flag,buffer,offset,bufflen);
	
	return offset;
}
short pack_mac(const u8* pData, void* buffer, short offset)
{
	memcpy((u8*)buffer+offset, pData, 6);
	offset += 6;

	return offset;
}
/* 认证激活报文的打包模块，供AP调用 */
short  pack_auth_active(const auth_active* pContent,void* buffer,unsigned short bufflen)
{
	u16 offset=0;

	packet_head head;
	memset((u8 *)&head, 0, sizeof(packet_head));
	head.version      = VERSIONNOW;
	head.type =  WAI;
	head.sub_type         = AUTHACTIVE;
	head.reserved     = RESERVEDDEF;
	head.data_len     = 0x0000;
	head.group_sc = 0x0000;
	head.frame_sc = 0x00;
	head.flag = 0x00;

	offset = c_pack_packet_head(&head,buffer,offset,bufflen);

	offset = c_pack_byte((u8)pContent->flag,buffer,offset,bufflen);
	offset = c_pack_32bytes((u8 *)&pContent->ae_auth_flag,buffer,offset,bufflen);

	offset = c_pack_word((u16)pContent->asu_id.id_flag,buffer,offset,bufflen);
	offset = c_pack_word((u16)pContent->asu_id.id_len,buffer,offset,bufflen);
	memcpy(buffer + offset, &pContent->asu_id.id_data, pContent->asu_id.id_len);
	offset +=	pContent->asu_id.id_len;
	offset = c_pack_word((u16)pContent->ae_cer.cert_flag,buffer,offset,bufflen);
	offset = c_pack_word((u16)pContent->ae_cer.cert_bin.length,buffer,offset,bufflen);
	memcpy(buffer + offset, pContent->ae_cer.cert_bin.data, pContent->ae_cer.cert_bin.length);
	offset += pContent->ae_cer.cert_bin.length;
	offset = c_pack_byte((u8)pContent->ecdh.para_flag,buffer,offset,bufflen);

	((unsigned char *)buffer)[offset]= (pContent->ecdh.para_len >> 8)&0xff;
	offset += 1;
	((unsigned char *)buffer)[offset]= (pContent->ecdh.para_len&0xff) ;
	offset += 1;

	memcpy(buffer + offset, pContent->ecdh.para_data, pContent->ecdh.para_len);
	offset += pContent->ecdh.para_len;
	
	set_packet_data_len(buffer,offset /*-sizeof(packet_head) */);
	return offset;
}


/* 连续解包函数  */

short  c_unpack_byte(u8* content, const void* buffer,u16 offset,u16 bufflen)
{
	if(offset+1>bufflen) return PACK_ERROR;
	*content = read_byte(buffer,offset);
	offset ++;

	return offset;
}
short  c_unpack_word(u16* content, const void* buffer,u16 offset,u16 bufflen)
{
	if( offset+2>bufflen ) return PACK_ERROR;
	*content = read_word(buffer,offset);
	offset += 2;

	return offset;
}
short  c_unpack_dword(u32* content, const void* buffer,u16 offset,u16 bufflen)
{
	if( offset+4>bufflen ) return PACK_ERROR;
	*content = read_dword(buffer,offset);
	offset += 4;

	return offset;
}
short  c_unpack_16bytes(u8* content, const void* buffer,u16 offset,u16 bufflen)
{
	int i;
	if(offset+16>bufflen) return PACK_ERROR;
	for(i = 0; i < 16; i ++)
	{
		*(content + i) = read_byte(buffer,offset);
		offset ++;
	}
	return offset;
}

short  c_unpack_32bytes(u8* content, const void* buffer,u16 offset,u16 bufflen)
{
	int i;
	if(offset+32>bufflen) return PACK_ERROR;
	for(i = 0; i < 32; i ++)
	{
		*(content + i) = read_byte(buffer,offset);
		offset ++;
	}
	return offset;
}
short  c_unpack_sign_alg(wai_fixdata_alg* p_sign_alg,const void* buffer,u16 offset,u16 bufflen)
{
	
	memset((u8 *)p_sign_alg, 0, sizeof(wai_fixdata_alg));

	if( offset+2>bufflen ) return PACK_ERROR;
	p_sign_alg->alg_length = read_word(buffer,offset);
	offset += 2;

	if( offset+1>bufflen ) return PACK_ERROR;
	p_sign_alg->sha256_flag = read_byte(buffer,offset);
	offset += 1;
	
	if( offset+1>bufflen ) return PACK_ERROR;
	p_sign_alg->sign_alg = read_byte(buffer,offset);
	offset += 1;
	
	if( offset+1>bufflen ) return PACK_ERROR;
	p_sign_alg->sign_para.para_flag = read_byte(buffer,offset);
	offset += 1;

	if( offset+2>bufflen ) return PACK_ERROR;
	p_sign_alg->sign_para.para_len = read_word(buffer,offset);
	offset += 2;

	if( offset+p_sign_alg->sign_para.para_len >bufflen ) 
	{
		return PACK_ERROR;
	}
	memcpy(p_sign_alg->sign_para.para_data, buffer + offset , p_sign_alg->sign_para.para_len);
	offset += p_sign_alg->sign_para.para_len;

	return offset;
}
 
short  c_unpack_byte_data(byte_data *p_byte_data, const void* buffer, u16 offset,u16 bufflen)
{
	if(offset +1 >bufflen)	return PACK_ERROR;	
	p_byte_data->length = read_byte(buffer, offset);
	offset += 1;
	
	if( (offset+p_byte_data->length > bufflen) ||(p_byte_data->length == 0)) /*if array beyond thye mark*/ 
			return PACK_ERROR;
	memcpy(p_byte_data->data, (u8*)buffer+offset, p_byte_data->length);
	offset += p_byte_data->length;

	return offset;
}
  short  c_unpack_comm_data(comm_data *p_byte_data, const void* buffer, u16 offset,u16 bufflen)
{
	if(offset +2 >bufflen)	return PACK_ERROR;	
	p_byte_data->length = read_word(buffer, offset);
	offset += 2;
	
	if( (offset+p_byte_data->length > bufflen) ||
		(p_byte_data->length > COMM_DATA_LEN)||
		(p_byte_data->length == 0)) /*if array beyond thye mark*/ 
			return PACK_ERROR;

	memcpy(p_byte_data->data, (u8*)buffer+offset, p_byte_data->length);
	offset += p_byte_data->length;
	return offset;
}

short  unpack_head(packet_head *p_head,const void* buffer,u16 bufflen)
{
	u16 offset = 0;
	
	offset = c_unpack_word(&p_head->version, buffer,offset,bufflen); 
	if(p_head->version != VERSIONNOW) 
	{
		DPrintf("Version(0x%02x) is wrong!\n", p_head->version);
		return PACK_ERROR;
	}
	offset = c_unpack_byte(&p_head->type, buffer,offset,bufflen);
	offset = c_unpack_byte(&p_head->sub_type, buffer,offset,bufflen);
	offset = c_unpack_word(&p_head->reserved, buffer,offset,bufflen);
	offset = c_unpack_word(&p_head->data_len, buffer,offset,bufflen);
	if( p_head->data_len != bufflen) 
	{
		DPrintf("p_head->data_len(%d) != receive bufflen(%d)\n", p_head->data_len, bufflen);
		return PACK_ERROR;
	}
	
	offset = c_unpack_word(&p_head->group_sc, buffer,offset,bufflen);
	offset = c_unpack_byte(&p_head->frame_sc, buffer,offset,bufflen);
	offset = c_unpack_byte(&p_head->flag, buffer,offset,bufflen);
	
	return offset;
}
short unpack_wai_fixdata_cert(wai_fixdata_cert*pcert, const void* buffer,u16 offset,u16 bufflen)
{
	/*get 证书*/
	if(offset + 2+2 >= bufflen) return PACK_ERROR;
	
	pcert->cert_flag = read_word(buffer, offset);offset  += 2;
	pcert->cert_bin.length =  read_word(buffer, offset);offset += 2;

	if(offset + pcert->cert_bin.length > bufflen) 
		return PACK_ERROR;

	pcert->cert_bin.data = (unsigned char *)buffer + offset;
	offset += pcert->cert_bin.length;
	return offset;
}

short unpack_wai_fixdata_id(wai_fixdata_id*pid, const void* buffer,u16 offset,u16 bufflen)
{
	/*get 身份列表*/
	if(offset + 2 > bufflen) return PACK_ERROR;
	offset = c_unpack_word(&pid->id_flag, buffer, offset,  bufflen);
	if(offset + 2 > bufflen) return PACK_ERROR;
	offset = c_unpack_word(&pid->id_len, buffer, offset,  bufflen);
	if(offset + pid->id_len > bufflen) return PACK_ERROR;
	memcpy(&pid->id_data, buffer + offset, pid->id_len);
	offset += pid->id_len;
	return offset;
}

short unpack_wai_attridata_cert_check_result(wai_attridata_auth_result*pid_list, const void* buffer,u16 offset,u16 bufflen)
{
	/*get 验证结果*/
	if(bufflen < offset+1+2) return PACK_ERROR;
	
	offset = c_unpack_byte(&pid_list->identifier, buffer,  offset, bufflen);
	offset = c_unpack_word(&pid_list->length, buffer,  offset, bufflen);
	if(offset + pid_list->length > bufflen) 
	{
		return PACK_ERROR;
	}
	
	if(bufflen < offset+32+32+1) return PACK_ERROR;
	offset = c_unpack_32bytes((u8 *)&pid_list->ae_challenge, buffer,  offset, bufflen);
	offset = c_unpack_32bytes((u8 *)&pid_list->asue_challenge, buffer,  offset, bufflen);
	offset = c_unpack_byte(&pid_list->auth_result1, buffer,  offset, bufflen);
	
	offset = unpack_wai_fixdata_cert(&pid_list->cert1, buffer,offset, bufflen);
	if(offset == PACK_ERROR ) return PACK_ERROR;

	if(offset + 1 > bufflen) return PACK_ERROR;
	offset = c_unpack_byte(&pid_list->auth_result2, buffer,  offset, bufflen);
	
	offset = unpack_wai_fixdata_cert(&pid_list->cert2, buffer,offset, bufflen);
	if(pid_list->length != 32 /*ae_challenge*/
					+ 32 /*asue_challenge*/
					+ 1 /*auth_result1*/
					+ 4 /*cer flag | cert len*/
					+ pid_list->cert1.cert_bin.length
					+ 1 /*auth_result2*/
					+ 4  /*cer flag | cert len*/
					+ pid_list->cert2.cert_bin.length)
	{

		return PACK_ERROR;
	}
	return offset;
}
short unpack_wai_attridata_id_list(wai_attridata_id_list*pid_list, const void* buffer,u16 offset,u16 bufflen)
{
	u16  i;

	if(offset +1+2+1+2 >bufflen  ) return PACK_ERROR;
	/*get 身份列表*/
	offset = c_unpack_byte(&pid_list->identifier, buffer,  offset, bufflen);
	offset = c_unpack_word(&pid_list->length, buffer,  offset, bufflen);
	offset = c_unpack_byte(&pid_list->rev, buffer,  offset, bufflen);
	offset = c_unpack_word(&pid_list->id_no, buffer,  offset, bufflen);
	
	for(i =0; i< pid_list->id_no; i++)/*第i 个身份*/
	{
		offset = unpack_wai_fixdata_id(&pid_list->id[i], buffer, offset, bufflen);

		if(offset == PACK_ERROR)
			break;
	}
	return offset;
}

short unpack_wai_attridata_auth_payload(wai_attridata_auth_payload	*pasue_sign, 
								const void* buffer,u16 offset,
								u16 bufflen)
{
	/*get ASUE的签名*/
	if(offset + 1 > bufflen) return PACK_ERROR;
	offset = c_unpack_byte(&pasue_sign->identifier, buffer,  offset, bufflen);
	if(pasue_sign->identifier !=1) return PACK_ERROR;
	if(offset + 2 > bufflen) return PACK_ERROR;
	offset = c_unpack_word(&pasue_sign->length, buffer,  offset, bufflen);
	offset = unpack_wai_fixdata_id(&pasue_sign->id, buffer, offset, bufflen);
	if( offset==PACK_ERROR ) return PACK_ERROR;
	offset = c_unpack_sign_alg(&pasue_sign->alg, buffer, offset, bufflen);
	if( offset==PACK_ERROR ) return PACK_ERROR;
	offset = c_unpack_comm_data(&pasue_sign->sign_data, buffer,  offset, bufflen);
	if( offset==PACK_ERROR ) return PACK_ERROR;
	if(pasue_sign->length != 4 /*id flag | id len*/
						+ pasue_sign->id.id_len
						+ 2  /*alg_length len*/
						+ pasue_sign->alg.alg_length
						+ 2 /*sign data len*/
						+ pasue_sign->sign_data.length)
	{

		return PACK_ERROR;
	}
	return offset;

}
/* 接入认证请求报文解包函数，供AP调用 */
short  unpack_sta_auth_request(sta_auth_request *p_sta_auth_request,const void* buffer,u16 bufflen, u16 *no_signdata_len)
{
	u16 offset = 0 ;
	packet_head head;
	
	memset((u8 *)&head, 0, sizeof(packet_head));
	if(bufflen <= sizeof( struct _packet_head )
		+ 1/*flag*/
		+32/*auth flag*/
		+32/*asue challenge*/)
	{
		return PACK_ERROR;
	}
	
	offset = unpack_head(&head, buffer,bufflen);
	if(offset == PACK_ERROR)
		return PACK_ERROR;
	offset = c_unpack_byte((u8 *)&p_sta_auth_request->flag,buffer,offset,bufflen);   
	offset = c_unpack_32bytes((u8 *)&p_sta_auth_request->ae_auth_flag,buffer,offset,bufflen);   
	offset = c_unpack_32bytes((u8 *)&p_sta_auth_request->asue_challenge,buffer,offset,bufflen);   
	offset = c_unpack_byte_data(&p_sta_auth_request->key_data,buffer,offset,bufflen);   
	if( offset==PACK_ERROR ) 	return PACK_ERROR;
	/*get AE的身份*/
	offset =  unpack_wai_fixdata_id(&p_sta_auth_request->ae_id,  buffer, offset,  bufflen);
	if( offset==PACK_ERROR ) 	return PACK_ERROR;
	
	/*get ASUE证书*/
	offset = unpack_wai_fixdata_cert(&p_sta_auth_request->asue_cert,  buffer, offset, bufflen);
	if( offset==PACK_ERROR ) 	return PACK_ERROR;
	
	/*get ECDH参数*/
	offset = c_unpack_byte(&p_sta_auth_request->ecdh.para_flag,buffer,offset,bufflen);   
	if( offset==PACK_ERROR ) 	return PACK_ERROR;
	
	offset = c_unpack_word(&p_sta_auth_request->ecdh.para_len,buffer,offset,bufflen);   
	if( offset==PACK_ERROR ) 	return PACK_ERROR;
	
	memcpy(p_sta_auth_request->ecdh.para_data, buffer + offset, p_sta_auth_request->ecdh.para_len);
	offset += p_sta_auth_request->ecdh.para_len;
	if(*((u8 *)buffer + offset) == 3 && p_sta_auth_request->flag & WAI_FLAG_OPTION_BYTE)/*ASUE信任的ASU列表*/
	{
		offset = unpack_wai_attridata_id_list(&p_sta_auth_request->asu_id_list,  buffer, offset, bufflen);
		if( offset==PACK_ERROR ) 	return PACK_ERROR;
	}
	*no_signdata_len = offset;
	/*get ASUE的签名*/
	offset = unpack_wai_attridata_auth_payload(&p_sta_auth_request->asue_sign, buffer, offset, bufflen);
	if( offset==PACK_ERROR ) 
	{
		return PACK_ERROR;
	}
	
	return offset;
}

/* 证书认证响应报文解包函数，供AP调用 */
short  unpack_ap_auth_response(ap_auth_response *p_auth_response,
						const void* buffer,u16 bufflen,
						u16 *no_signdata_len,u16 *no_signdata_len1)
{
	u16 offset = 0, signdata_len = 0;
	packet_head head;
	memset((u8 *)&head, 0, sizeof(packet_head));

	if(bufflen < sizeof(packet_head) + 6+6) 
		 return PACK_ERROR;
	
	offset = unpack_head(&head, buffer,bufflen);
	if(offset == PACK_ERROR)
		return PACK_ERROR;
	memcpy(p_auth_response->addid.mac1, buffer + offset, 6);
	offset += 6;
	
	memcpy(p_auth_response->addid.mac2, buffer + offset, 6);
	offset += 6;
	
	offset = unpack_wai_attridata_cert_check_result(&p_auth_response->cert_result.cert_result, buffer, offset, bufflen);
	if(offset == PACK_ERROR) return PACK_ERROR;
	
	if(bufflen -offset  < 2)  return PACK_ERROR;
	memcpy(&signdata_len, buffer + offset + 1, 2);/*get sign data*/
	signdata_len = ntohs(signdata_len);

	if(p_auth_response->cert_result.cert_result.auth_result1 == CERTIFISSUERUNKNOWN )
	{
		if(head.data_len - offset - 1 - 2 - signdata_len !=0 )
		{
		 	return PACK_ERROR;
		}
	}
	if(head.data_len - offset - 1 - 2 - signdata_len > 0 )
	{
		/*为了确定是否存在ASUE信任的服务器签名这个属性
		，通过判断长度*/
		*no_signdata_len = offset;
		offset = unpack_wai_attridata_auth_payload(&p_auth_response->cert_result.asue_trust_asu_sign, buffer, offset, bufflen);
		if(offset == PACK_ERROR) return PACK_ERROR;
		
		memcpy(&signdata_len, buffer + offset + 1, 2);/*get sign data*/
		signdata_len = ntohs(signdata_len);
		*no_signdata_len1 = offset;
		offset = unpack_wai_attridata_auth_payload(&p_auth_response->cert_result.ae_trust_asu_sign, buffer, offset, bufflen);
		if(offset == PACK_ERROR) return PACK_ERROR;
	}
	else if(head.data_len - offset - 1 - 2 - signdata_len == 0)
	{
		*no_signdata_len1 = offset;
		offset = unpack_wai_attridata_auth_payload(&p_auth_response->cert_result.ae_trust_asu_sign, buffer, offset, bufflen);
		if(offset == PACK_ERROR) return PACK_ERROR;
	}
	else 
	{
		 return PACK_ERROR;
	}
	return offset;
}
/* 会话密钥协商响应报文解包函数，供AP调用 */
short  unpack_ucastkey_neg_response(session_key_neg_response *p_session_key_neg_response,const void* buffer, u16 bufflen)
{
	u16 offset = 0 , wie_len = 0;
	packet_head head;
	
	memset((u8 *)&head, 0, sizeof(packet_head));
	if(bufflen<= sizeof(packet_head)
		+1 /*flag*/
		+16/* BKID*/
		+1/*USKID*/
		+12/*ADDID*/
		+32/*ASUE challenge*/
		+32/*ASUE challenge*/
		+2/*WIE ID+WIE LENGTH*/
		)
		return PACK_ERROR;
	offset = unpack_head(&head, buffer,bufflen);
	if(offset == PACK_ERROR)
		return PACK_ERROR;
	offset = c_unpack_byte((u8 *)&p_session_key_neg_response->flag,buffer,offset,bufflen);   
	offset = c_unpack_16bytes(p_session_key_neg_response->bkid,buffer,offset,bufflen);  
	offset = c_unpack_byte(&p_session_key_neg_response->uskid, buffer, offset, bufflen);  
	
	memcpy(p_session_key_neg_response->addid.mac1, buffer + offset, 6);
	offset += 6;
	memcpy(p_session_key_neg_response->addid.mac2, buffer + offset, 6);
	offset += 6;

	offset = c_unpack_32bytes(p_session_key_neg_response->asue_challenge, buffer, offset, bufflen);
	offset = c_unpack_32bytes(p_session_key_neg_response->ae_challenge, buffer, offset, bufflen);
	
	wie_len = *((u8 *)buffer + offset + 1);
	if(offset +wie_len +2 >bufflen)
	{
		DPrintf("offset = %d, wie_len= %d, bufflen=%d\n", offset, wie_len, bufflen);
		return PACK_ERROR;
	}
	memcpy(&p_session_key_neg_response->wie_asue, buffer + offset, wie_len + 2);
	offset += wie_len + 2; 	

	 if( offset+HMAC_LEN > bufflen ) return PACK_ERROR;
	 
	memcpy(p_session_key_neg_response->mic, (u8*)buffer+offset, HMAC_LEN);
	offset += HMAC_LEN;
	return offset;
}

/* 组播密钥响应报文的解包函数，供AP调用 */
short unpack_msk_announcement_res(groupkey_notice_response *p_groupkey_notice_response, const void* buffer, u16 bufflen)
{
	u16 offset = 0;
	packet_head head;
	memset((u8 *)&head, 0, sizeof(packet_head));

	if(offset 
		+sizeof(packet_head)
		+1 /*flag*/
		+1/*MSKID*/
		+1/*USKID*/
		+12/*ADDID*/
		+16/*PN*/
		+20/*MIC*/
		>bufflen)
		return PACK_ERROR;


	offset = unpack_head(&head, buffer,bufflen);
	if(offset == PACK_ERROR)
		return PACK_ERROR;
	offset = c_unpack_byte((u8 *)&p_groupkey_notice_response->flag, buffer, offset, bufflen);
	offset = c_unpack_byte(&p_groupkey_notice_response->notice_keyid, buffer, offset, bufflen);
	offset = c_unpack_byte(&p_groupkey_notice_response->uskid, buffer, offset, bufflen);
	
	memcpy(p_groupkey_notice_response->addid.mac1, buffer + offset, 6);
	offset += 6;

	memcpy(p_groupkey_notice_response->addid.mac2, buffer + offset, 6);
	offset += 6;

	offset = c_unpack_dword(&p_groupkey_notice_response->g_nonce[0], buffer, offset, bufflen);
	offset = c_unpack_dword(&p_groupkey_notice_response->g_nonce[1], buffer, offset, bufflen);
	offset = c_unpack_dword(&p_groupkey_notice_response->g_nonce[2], buffer, offset, bufflen);
	offset = c_unpack_dword(&p_groupkey_notice_response->g_nonce[3], buffer, offset, bufflen);

	memcpy(p_groupkey_notice_response->mic, (u8*)buffer+offset, HMAC_LEN);
	offset += HMAC_LEN;
	
	return offset;
}

