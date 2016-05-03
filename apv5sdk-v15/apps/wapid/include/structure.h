/***************************************************************
* Copyright (c) 2001,西安西电捷通无线网络通信有限公司
* All rights reserved.
* 
* 文件名称：structure.h
* 摘    要：定义WAI分组的结构
* 
* 当前版本：1.1
* 作    者：王月辉yhwang@iwncomm.com
* 完成日期：2005年6月10日
*
* 取代版本：1.0 
* 原作者  ：王月辉yhwang@iwncomm.com
* 完成日期：2004年1月10日
***************************************************************/
#ifndef __STRUCTURE_H__
#define __STRUCTURE_H__
#include "typedef.h"

#define  VERSIONNOW			0x0001	/*当前版本号*/
#define  WAI					0x01	
#define  RESERVEDDEF		0x0000	/*reserved定义*/   

#define TRUE					(u8)1
#define FALSE				(u8)0

#define MAX_EXTENSION_TLV_ATTR   2		/*最大的扩展属性的个数*/
#define MAX_BYTE_DATA_LEN		(u16)256 /* 最大字节数据长度 */
#define COMM_DATA_LEN          		(u16)3000//1024/* 最大字节数据长度 */
#define PUBKEY_LEN             		(u16)48  /* 公钥长度 */
#define HMAC_LEN                        		20  /*hmac 长度*/

/*WAI 分组类型*/
#define  AUTHACTIVE				0x03	//鉴别激活分组
#define  STA_ACESS_REQUEST		0x04	//接入鉴别请求分组
#define  STA_ACCESS_RESPONSE	0x05	//接入鉴别响应分组
#define  AP_AUTH_REQUEST		0x06	//证书鉴别请求报文
#define  AP_AUTH_RESPONSE		0x07	//证书鉴别响应分组
#define  USK_NEG_REQUEST  		0x08	//密钥协商请求分组
#define  USK_NEG_RESPONSE		0x09	//密钥协商响应分组
#define  USK_NEG_CONFIRM		0x0A	//密钥协商响应分
#define  MSK_ANNOUNCE         		0x0B 	//广播密钥通告分组
#define  MSK_RESPONSE 			0x0C 	//广播密钥通告响应分组

/*证书鉴别结果*/
#define CERTIFVALID						0x00  /*证书正确无误*/
#define CERTIFISSUERUNKNOWN			0x01  /*颁发者错误(证书公钥和名称与本AS不同) */     
#define CERTIFUNKNOWNCA				0x02  /*证书基于不可信任的根证书*/
#define CERTIFEXPIRED					0x03  /*证书过期*/
#define CERTIFSIGNERR					0x04  /*签名错误 */
#define CERTIFREVOKED					0x05  /*证书已吊销*/
#define CERTIFBADUSE					0x06  /*证书未按规定用途使用*/
#define CERTUNKNOWNREVOKESTATUS		0x07    /* 证书吊销状态未知*/
#define CERTUNKNOWNERROR     			0x08    /* 证书错误未知*/

#define  PACK_ERROR  0xffff

/*wapi参数集合，考虑了BSS，没有考虑IBSS*/
#define MAX_AKM_NO 2
#define MAX_UNICAST_NO 2
#define MAX_MULTICAST_NO 2
#define MAX_BKID_NO 2

/*
 * 1、证书及其相关结构定义	
 * 1) 通用数据定义
 */
struct _byte_data {
	u8  length;                      			/* 长度 */
	u8  pad_value[3];                		/* 实现对齐的数据 */
	u8  data[MAX_BYTE_DATA_LEN];		/* 内容 */
} ;
typedef struct _byte_data   byte_data;

struct _comm_data{
 	u32  length;                  			 /* 长度 */
	u8  data[MAX_BYTE_DATA_LEN];  	/* 内容 */
};
typedef struct _comm_data comm_data,*pcomm_data; 

typedef comm_data tsign;				/*签名*/

struct _big_data{
 	u16  length;                  			 /* 长度 */
	u16  pad_value;               			 /* 实现对齐的数据 */
	u8  data[COMM_DATA_LEN];  		/* 内容 */
};
typedef struct _big_data big_data,*pbig_data;

/*
 * 2)杂凑算法sha－256和签名算法定义在一个结构里
 */
#define MAX_PARA_LEN 20 
struct _para_alg {
	u8		para_flag;   				/*参数*/
	u8		pad[3];
	u16		para_len;  		 		/*参数长度*/
	u16		pad16;
	u8		para_data[MAX_PARA_LEN];   /*参数内容OID */
} __ATTRIBUTE_PACK__;
typedef struct _para_alg para_alg, *ppara_alg;

/*WAI协议分组数据字段的固定内容定义*/
/* a)标识FLAG*/
#define WAI_FLAG_BK_UPDATE 		0X01
#define WAI_FLAG_PRE_AUTH   		0X02
#define WAI_FLAG_AUTH_CERT 		0X04
#define WAI_FLAG_OPTION_BYTE 		0X08
#define WAI_FLAG_USK_UPDATE 	0X10	/*USK更新标识*/
#define WAI_FLAG_STAKEY_SESSION	0X20
#define WAI_FLAG_STAKEY_DEL 		0X40
#define WAI_FLAG_REV		 		0X80
#define MAX_CERT_DATA_LEN 1000
#define MAX_ID_DATA_LEN 1000

typedef  u8 wai_fixdata_flag;

struct cert_bin_t
{
	unsigned int type;
	unsigned char *data;	
	unsigned int length;
};
typedef struct cert_bin_t item_data_t, resendbuf_st;

struct key_bin_t
{
	unsigned int OrderLen;
	item_data_t publickey;
	item_data_t privatekey;
};

/*b)证书*/
typedef struct _wai_fixdata_cert {
	u16	cert_flag;
	struct cert_bin_t cert_bin;
}wai_fixdata_cert;



typedef struct _wai_fixdata_id{
	u16	id_flag;
	u16	id_len;
	u8 	id_data[MAX_ID_DATA_LEN];     /* 内容 */
}wai_fixdata_id;

/*d)地址索引*/
typedef struct _wai_fixdata_addid {
	u8	mac1[6];
	u8	mac2[6];
}wai_fixdata_addid;

/*WAI协议分组数据字段的属性内容定义*/

/*签名算法*/
typedef struct _wai_fixdata_alg {
	u16		alg_length;
	u8		sha256_flag;
	u8		sign_alg;
	para_alg	sign_para;
}wai_fixdata_alg;

/*a)签名属性*/
typedef struct _wai_attridata_auth_payload{
	u8				identifier;             	/*属性名称标识 */
	u8				pad8[3];
	u16 				length;                 	/*长度*/
	u16				pad16[2];
	wai_fixdata_id		 id;				/*身份*/
	wai_fixdata_alg	alg;				/*签名算法*/
	comm_data		sign_data;		/*签名*/
}wai_attridata_auth_payload;


/*b)证书验证结果*/
typedef struct _wai_attridata_auth_result{
	u8				identifier;				/*属性名称标识*/
	u8 				pad;
	u16 				length;                 		/*长度*/
	u8				ae_challenge[32];		/*一次性随机数*/ 
	u8  				asue_challenge[32];	/*一次性随机数*/ 
	u8				auth_result1;			/*验证结果1*/
	wai_fixdata_cert 	cert1;				/*证书1*/
	u8				auth_result2;			/*验证结果2*/
	u8 				pad8;
	wai_fixdata_cert	cert2;				/*证书2*/
}wai_attridata_auth_result;

/*c)身份列表*/
#define MAX_ID_NO 10
typedef struct _wai_attridata_id_list{
	u8		identifier;             	/* 属性名称标识 */
	u16 		length;                 	/* 长度*/
	u8		rev;		
	u16  		id_no;			/*身份个数*/
	wai_fixdata_id	id[MAX_ID_NO];	/*身份*/
}wai_attridata_id_list;


 /* 2、STA与AP之间的鉴别结构定义*/
 /* 1) 通用数据头部定义*/
 struct _packet_head {
	u16  version; 		/* 版本号 */
	u8	type;			/*WAI类型*/
	u8  	sub_type;		/* 鉴别分组类型 */
	u16  	reserved;		/* 保留 */
	u16  	data_len;		/* 数据长度 */ 
	u16 	group_sc;		/*分组序号*/
	u8   	frame_sc;		/*分片序号*/
	u8 	flag;				/*分片标志*/
} ;
typedef struct _packet_head packet_head, *ppacket_head;

/*
 2) 鉴别激活分组
*/
/*
 * b) 鉴别激活分组
 */
 struct _auth_active {
 	wai_fixdata_flag	flag;				/*标识字段*/
	u8				pad_vlue[3];      	/* 实现对齐的数据  */ 
	u8				ae_auth_flag[32];	/*鉴别标识*/
	wai_fixdata_id		asu_id;			/*ASU的身份*/
	wai_fixdata_cert 	ae_cer;              	/* AE 的证书 */
	para_alg			ecdh;			/*ecdh算法*/
};
typedef struct _auth_active    auth_active,  *pauth_active;

/*
 * 3) 接入鉴别请求分组
 */
struct _sta_auth_request {
	wai_fixdata_flag	flag;				/*标识字段*/
	u8				ae_auth_flag[32];	/*鉴别标识*/
	u8				asue_challenge[32];/*asue 挑战*/
	byte_data			key_data;		/*asue 密钥数据*/
	wai_fixdata_id		ae_id;			/*ae 身份*/
	wai_fixdata_cert	asue_cert;		/*asue 证书*/
	wai_attridata_id_list	asu_id_list;		/*asue 信任的asu列表*/
	para_alg			ecdh;
	wai_attridata_auth_payload	asue_sign;/*asue 签名*/
} ;
typedef struct _sta_auth_request    sta_auth_request;

/*
 * 4) 接入鉴别响应分组
*/
struct _cert_check_result {
	wai_attridata_auth_result 		cert_result;			/*鉴别结果*/	
	wai_attridata_auth_payload	asue_trust_asu_sign;	/*ASUE 信任的ASU签名*/
	wai_attridata_auth_payload	ae_trust_asu_sign;	/*AE 信任的ASU签名*/
};
typedef struct _cert_check_result cert_check_result;

struct _auth_response {
	wai_fixdata_flag	flag;				/*标识字段*/
	u8				pad_vlue[3];      	/* 实现对齐的数据  */ 
	u32				asue_challenge;	/*asue 挑战*/
	u32				ae_challenge;		/*ae挑战*/
	u8				access_result;	/*接入结果*/
	u8				pad_vlue1[3];      	/* 实现对齐的数据  */ 
	byte_data			asue_key_data;	/*asue 密钥数据*/
	byte_data			ae_key_data;		/*ae 密钥数据*/
	wai_fixdata_id		ae_id;			/*ae 身份*/
	wai_fixdata_id		asue_id;			/*asue 身份*/
	cert_check_result 	cert_result;		/*复合的证书验证结果*/
	wai_attridata_auth_payload	ae_sign;	/*ae 签名*/
} ;
typedef struct _auth_response auth_response;

/*
 * 5) 密钥协商请求分组
 */
struct _session_key_neg_request {
	wai_fixdata_flag	flag;		/*标识字段*/
	u8		pad_vlue[3];		/* 实现对齐的数据*/ 
	u8		bkid[16];			/*基密钥索引 */
	u8		uskid;			/*密钥标识ID*/
	u8		pad_vlue1[3];		/* 实现对齐的数据*/
	wai_fixdata_addid	addid;	/*地址索引 */
	u8		ae_challenge[32];	/*ae挑战 */
} ;
typedef struct _session_key_neg_request session_key_neg_request;

/*
 * 6) 密钥协商响应分组
 */
/* 密钥协商响应分组结构 */
struct _session_key_neg_response {
	wai_fixdata_flag	flag;			/*标识字段*/
	u8				pad_vlue[3]; 	/* 实现对齐的数据  */ 
	u8      			bkid[16];		/*基密钥索引 */
	u8				uskid; 		/*密钥标识ID*/
	u8				pad_vlue1[3];	/* 实现对齐的数据  */
	wai_fixdata_addid	addid;		/*地址索引 */
	u8				asue_challenge[32];  /*asue挑战 */
	u8				ae_challenge[32];  /*ae挑战 */
	u8				wie_asue[255];
	u8				mic[20];
} ;
typedef struct _session_key_neg_response session_key_neg_response;

/*套件*/
typedef struct _suite_haha {
	u8		oui[3];
	u8		suite_type;
}suite_haha;
/*能力信息字段*/
typedef struct _wapi_capability {
	u16	pre_auth:1;
	u16 	reserved:15;
}wapi_capability;

/*WAPI信息元素*/
typedef struct	_wapi_para_index { 
	u8			element_id;				/*ID*/
	u8			len;						/*长度*/
	u16			ver;						/*版本*/
	u16			akm_no;					/*AKM计数*/
	suite_haha	akm_suite[MAX_AKM_NO];	/*AKM*/
	u16			unicast_key_no;			/*单播密钥套间计数*/
	suite_haha	unicast_key_suite[MAX_UNICAST_NO];/*单播密钥套*/
	suite_haha	multicast_key_suite;		/*组播密钥套间*/	
	wapi_capability	capability;				/*能力信息*/
	u16		bkid_no;					/*BKID计数*/
	u16		bkid_list[MAX_BKID_NO];	/*BKID列表*/
}wapi_para_index;

/*
 * 7) 密钥协商确认分组
 */
 struct _session_key_neg_confirm {
	wai_fixdata_flag	flag;			/*标识字段*/
	u8				pad_vlue[3];	/* 实现对齐的数据  */ 
	u8     			bkid[16];		/*基密钥索引 */
	u8				uskid;		/*密钥标识ID*/
	u8				pad_vlue1[3];	/* 实现对齐的数据  */
	wai_fixdata_addid	addid;		/*地址索引 */
	u8				asue_challenge[32];  /*asue挑战 */
	wapi_para_index	wie_ae;
	u8				mic[20];
} ;
typedef struct _session_key_neg_confirm session_key_neg_confirm;

/*
 *8)组播密钥通告分组
 */
struct _groupkey_notice {
	wai_fixdata_flag	flag;				/*标识字段*/
	u8				notice_keyid;		/*通告密钥索引，在0和1之间翻转*/
	u8				uskid;              	 /*密钥标识ID*/
	u8				pad_vlue[1];      	/* 实现对齐的数据  */ 
	wai_fixdata_addid	addid;			/*地址索引 */
	u8				g_sn[16];  		/*数据序号 */
	u32				g_nonce[4];		/*密钥通告标识 */
	byte_data			negotiate_data;	/*密钥通告数据 */
	u8				mic[20];  
} ;
typedef struct _groupkey_notice groupkey_notice;

/*
*9)组播密钥通告响应分组
*/
struct _groupkey_notice_response {
	wai_fixdata_flag	flag;				/*标识字段*/
	u8				notice_keyid;		/*通告密钥索引，在0和1之间翻转*/
	u8				uskid;               	/*密钥标识ID*/
	u8				pad_vlue[1];      	/* 实现对齐的数据  */ 
	wai_fixdata_addid	addid;			/*地址索引 */
	u32			g_nonce[4];		/*密钥通告标识 */
	u8				mic[20];     
};
typedef struct _groupkey_notice_response groupkey_notice_response;


/*
 * 3) 证书鉴别响应报文：
 */
 struct _ap_auth_response { 
	wai_fixdata_addid	addid;		/*地址索引 */
	cert_check_result	cert_result;	/*接入鉴别响应分组*/	
} ;
typedef struct _ap_auth_response ap_auth_response;
#endif

