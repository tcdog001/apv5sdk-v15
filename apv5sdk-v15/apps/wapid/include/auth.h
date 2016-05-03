/**************************************************************
* Copyright (c) 2001,西安西电捷通无线网络通信有限公司
* All rights reserved.
* 
* 文件名称：auth.h
* 摘    要：定义WAI中的结构
* 
* 当前版本：1.1
* 作    者：王月辉yhwang@iwncomm.com
* 完成日期：2005年6月10日
*
* 取代版本：1.0 
* 原作者  ：王月辉yhwang@iwncomm.com
* 完成日期：2004年1月10日
****************************************************************/

#ifndef _AUTH_H
#define _AUTH_H
//#define CGI
	#include <sys/types.h>
	#include <sys/socket.h>
	#include <netinet/in.h>
	#include <linux/if.h>
	#include <linux/netlink.h>
	//#include <linux/wireless.h>
	#include "wireless_copy.h" /*same with kernel*/
    #include "include/compat.h"

#ifdef LSDK6	
	#include "byte_order.h"
	//#include "wireless_copy.h" /*same with kernel*/
	#include "ieee80211_external.h"
	#include "ieee80211_ioctl.h"
    #include "ieee80211_defines.h"
#endif

	#include "typedef.h"
	#include "structure.h"
	#include "config.h" 
#ifdef DOT11_WAPI_MIB
#include "dot11wapiMib.h"
#endif


#ifdef LSDK6
	/*设置WAI参数的IOCTL命令字*/
	#define P80211_IOCTL_SETWAPI_INFO		IEEE80211_PARAM_SETWAPI//按照LSDK6.1修改
	#define P80211_IOCTL_WAPIREKEY_USK     IEEE80211_PARAM_WAPIREKEY_USK
	#define P80211_IOCTL_WAPIREKEY_MSK     IEEE80211_PARAM_WAPIREKEY_MSK
	#define P80211_IOCTL_WAPIREKEY_UPDATE  IEEE80211_PARAM_WAPIREKEY_UPDATE
#else
	#define P80211_IOCTL_SETWAPI_INFO		(0x8BE0 + 26)//0x8BFA
	/*设置STA状态的IOCTL命令字*/
	#define IEEE80211_IOCTL_SETMLME			(0x8BE0+6)//SIOCIWFIRSTPRIV
#endif

/*设置WAPI参数的IOCTL子命令字,表示设置WAPI状态*/
#define P80211_PACKET_WAPIFLAG			(u16)0x0001

/*设置WAPI参数的IOCTL子命令字,表示安装密钥*/
#define P80211_PACKET_SETKEY     			(u16)0x0003

#ifndef IFNAMSIZ
#define IFNAMSIZ 16
#endif

#define WLAN_ADDR_LEN  	6		/*MAC地址长度*/


/*接收缓冲区的大小*/
#define  FROM_MT_LEN 	1500		
#define  FROM_AS_LEN 	3000		
#define  FROM_CGI_LEN 	2000		

#define  MAX_AUTH_MT_SIMU  	32	/*允许进行鉴别STA的个数*/

#define MSK_LEN  	16
#define BK_LEN 16
#define BKID_LEN	16
#define CHALLENGE_LEN	32
#define PAIRKEY_LEN	16
#define MASTER_KEY_LEN	4*PAIRKEY_LEN + CHALLENGE_LEN
#define ADDID_LEN	12
#define WAI_MIC_LEN	20
#define WAI_IV_LEN	16
#define WAI_KEY_ANNOUNCE_ID_LEN	16
#define WAI_HDR	12
#define BK_TEXT  "base key expansion for key and additional nonce"
#define PSK_TEXT "preshared key expansion for authentication and key negotiation"
#define MSK_TEXT "multicast or station key expansion for station unicast and multicast and broadcast"
#define USK_TEXT "pairwise key expansion for unicast and additional keys and nonce"

#define MIN_REKEY_PERIOD	60
#define DEFAULT_REKEY_PERIOD	86400
#define REKEY_U_CHECK_INTERVAL 30

extern char *wai_subtype_info[];
extern struct eloop_data eloop;

/*鉴别模式*/
#define AUTH_MODE 		(u8)0
#define PRE_AUTH_MODE 	(u8)1

/* ASUE状态定义*/
#define NO_AUTH		  					(u16)0		/*未鉴别*/
#define MT_WAITING_ACCESS_REQ 			(u16)1		/*等待接入鉴别请求*/
#define MT_WAITING_AUTH_FROM_AS 		(u16)2		/*等待证书鉴别响应*/
#define MT_WAITING_SESSION		 		(u16)3		/*等待密钥协商*/
#define MT_WAITING_GROUPING	 		(u16)4		/*等待组播通告*/
#define MT_SESSIONKEYING  				(u16)5		/*单播密钥REKEY*/
#define MT_GROUPNOTICEING    			(u16)6		/*组播密钥REKEY*/
#define MT_SESSIONGROUPING 				(u16)7		/*发送了组播通告再发动态密钥协商请求*/
#define MT_WAITING_DYNAMIC_SESSION		(u16)8		/*单播密钥REKEY过程中的单播密钥响应*/
#define MT_WAITING_DYNAMIC_GROUPING	(u16)9		/*等待在组播密钥REKEY过程中组播密钥响应*/
#define MT_AUTHENTICATED  				(u16)10 	/*鉴别成功*/
#define MT_PRE_AUTH_OVER  				(u16)11 	/**/


/*Driver 发送的消息的类型*/
#define STA_ASSOC_EVENT					(u16)0x00F1		
#define STA_AGING_EVENT					(u16)0x00F3
#define UNICAST_REKEY_EVENT          			(u16)0x00F2
#define MULTI_REKEY_EVENT          			(u16)0x00F4

#ifndef LSDK6
	/*80211协议中sta 状态*/
	#define	IEEE80211_MLME_ASSOC			1	/* associate station */
	#define	IEEE80211_MLME_DISASSOC		2	/* disassociate station */
	#define	IEEE80211_MLME_DEAUTH			3	/* deauthenticate station */
	#define	IEEE80211_MLME_AUTHORIZE		4	/* authorize station */
	#define	IEEE80211_MLME_UNAUTHORIZE	5	/* unauthorize station */
#endif

/*WAI相关*/
//#define MAX_AP_NODE 				20	/*预鉴别最大节点数*/
//#define PRE_AUTH_SAVE_TIME			1800 /*预鉴别超时时间*/
#define TIMEOUT						1	/*WAI分组超时时间*/
#define MAX_RESEND_COUNT 			3	/*WAI分组最大重传次数*/
//#define GROUP_NOTICE_RESULT 		0xFF/*组播密钥通告结果标识*/
#define WAI_HLEN						12	/*WAI分组头长度*/
#define AS_AUTH_PORT				3810
#ifndef LSDK6                            	
/*设置80211 协议中的sta状态 */
struct ieee80211req_mlme {
	u8	 	im_op;		/* operation to perform */
	u16	im_reason;	/* 802.11 reason code */
	u8 	im_macaddr[WLAN_ADDR_LEN];
};
#endif

/*Driver 发送的关于关联STA的信息*/								
struct asso_mt_t
{
#ifndef LSDK6 
	struct nlmsghdr hdr;		/**/
#endif
	u16		type;		/*消息类型*/
	u16		data_len;		/*消息长度*/
	u8 		ap_mac[6];
	u8 		pad1[2];
	u8		mac[6];		/*STA 的MAC地址*/
	u8 		pad[2];
	u8		gsn[16];		/*组播数据序号*/
	u8		wie[256];		/*wapi信息元素*/
};
typedef struct asso_mt_t asso_mt;
struct rtnl_handle
{
	int			fd;
	struct sockaddr_nl	local;
	struct sockaddr_nl	peer;
	__u32			seq;
	__u32			dump;
};


/*  resendinfo_st->direction   */
#define SENDTO_STA	0
#define SENDTO_AS	1

/*重发缓冲区信息结构*/
struct _resendinfo_st {
	u32		send_time;		/*发送时间*/
	u32		cur_count;		/*发送次数计数*/
	u32		timeout;			/*超时时间*/
	u32		direction;			/* 发送方向( STA or AS)*/
	u32		max_count;
};
typedef struct _resendinfo_st	 resendinfo_st;

/*密钥结构*/
struct _sta_key
{
	u8  uek[16]; 			/*单播加密密钥*/
	u8  uck[16];				/*单播完整性校验密钥*/
	u8  kek[16];				/*密钥加密密钥*/
	u8  mck[16];			/*消息鉴别密钥*/
	u32  valid_flag;			/*密钥是否有效的标志*/
};

/*PSK鉴别与密钥管理套间中AE的BKSA*/
struct psk_bksa
{
	u8 psk_password[32];
	u8 bk[16];
};

/*ASUE的BKSA*/
struct sta_bksa
{
	u8 bkid[16];
	u8 bk[16];
	u8 ae_auth_flag[32];		/*下一次证书鉴别的鉴别标识*/
	u16	bk_update;			/*表示是否定义BK更新策略*/
	u16	akm_no;				/*鉴别与密钥管理套间计数*/
	u8		akm[4*MAX_AKM_NO];	/*鉴别与密钥管理套*/
};

/*ASUE的USKSA*/
struct sta_usksa
{
	u8 uskid;				/*Key ID 0,1之间翻转*/
	u8 dynamic_key_used;	/**/
	u8 pad[2];
	struct _sta_key usk[2];	/*密钥*/
};

/*AE的MSKSA*/
struct sta_msksa
{
	u8		mskid;		/*Key ID 0,1之间翻转*/
	u8		pad[3];
	u8		msk[16];		/*组播密钥*/
};
typedef struct _wapi_stats_Entry{
	u32		wai_sign_errors;
	u32		wai_hamc_errors;
	u32		wai_auth_res_fail;
	u32		wai_discard;
	u32		wai_timeout;
	u32		wai_format_erros;
	u32 		wai_cert_handshake_fail;
	u32		wai_unicast_handshake_fail;
	u32		wai_multi_handshake_fail;
}wapi_stats_Entry;

/*ap 信息记录表*/
struct _apdata_info {
/*记录上级的wapid_interfaces结构*/
	void *user_data;				/*wapid_interfaces*/
	u8  wai_policy;				/*ap 的WAI策略*/
	u8  pre_auth_policy;			/*ap 的预鉴别策略*/
	u16	mtu;					/*无线卡的mtu*/

	u8 macaddr[6];				/*无线卡的mac 地址*/
	u8 pad;

	u8 	gsn[16];					/*组播数据序号*/
	u32	gnonce[4];				/*组播密钥通告标识*/

	int 		g_ifr;					/**/
	
	int		group_No;				/*组播通告计数*/
	int		group_status;				/*组播通告状态*/
	int 		msk_rekey_period;
	time_t 	msk_rekey_timer;			/*MSK rekey timer*/
	int		usk_rekey_period;
	
	int		rekeying;
	u8 		wie_ae[255];				/*WIE*/
	u8	 	ae_nonce[32];				/*AE 的挑战*/

	struct psk_bksa 	psk;				/*预共享密钥关联*/
	struct sta_msksa	msksa;			/*组播密钥关联*/
	
	wai_fixdata_alg 	sign_alg;			/*签名算法*/
	struct auth_sta_info_t *sta_info;		/**/
	char iface[IFNAMSIZ+1];
	int ap_debug;
#ifdef DOT11_WAPI_MIB
	struct Gb15629dot11wapiConfigEntry mib_config;
	struct Gb15629dot11wapiConfigUnicastCiphersEntry mib_uciphers;
	struct Gb15629dot11wapiConfigAuthenticationSuitesEntry mib_asuites[2];
	struct Gb15629dot11wapiStatsEntry mib_stats;
#endif
};
typedef struct _apdata_info apdata_info;


/*sta 信息记录表*/
struct auth_sta_info_t
{
	u16		packet_type;			/*接入控制消息类型*/
	u16		status;				/*ASUE状态*/

	u8			mac[6];			
	u8			auth_mode;			/*鉴别模式0正常模式,1预鉴别模式*/
	u8			auth_result;			/*鉴别结果*/
	
	u32		timestamp;			/*时标*/
	u32		pre_auth_save_time;	/*预鉴别时标*/

	resendinfo_st	sendinfo;				/*重发缓冲区属性*/
	resendbuf_st	buf0;				/*重发缓冲区0*/
	resendbuf_st	buf1;				/*重发缓冲区1*/
	
	u8 		gsn[16];				/*组播数据序号*/
	u8			asue_nonce[32];		/*ASUE挑战*/
	u8			asue_cert_hash[32];	/*ASUE 证书HASH*/
	u8			ae_nonce[32];			/*AE挑战*/
	u8			wie[256];				/*WAPI信息元素*/
	byte_data		asue_key_data;		/*ASUE密钥数据*/
	byte_data		ae_key_data;			/*AE密钥数据*/
	para_alg  		ecdh;				/*ECDH算法参数*/
	wai_fixdata_flag flag;				/*WAI 标识*/
	wai_fixdata_id	asue_id;				/*ASUE 身份*/
	struct sta_bksa 	bksa;			/*STA BKSA*/
	struct sta_usksa 	usksa;			/*STA USKSA*/
	u16		ae_group_sc;			/*AE发送的WAI 分组序号*/
	u16		asue_group_sc;		/*ASUE发送的WAI 分组序号*/
	apdata_info *pap;
	time_t  usk_rekey_timer;
	int	rekeying;
#ifdef DOT11_WAPI_MIB
	struct Gb15629dot11wapiStatsEntry mib_stats;
	u32 SATimeout;
	u32 BKLifeTimeout;
	u32 BKReauthTimeout;
#endif
};
	
/*向Driver发送消息的消息结构*/
struct ioctl_drv
{
	u16  io_packet;
	struct  _iodata
	{
		u16 wDataLen;
		u8  pbData[96];
	}iodata;
};//__attribute__ ((packed));

struct param_element_t
{
	int debug;
	char iface[20];
	char fileconfig[256];
};
struct wapid_interfaces {
	struct wapid_interfaces *next;
/*每个设备的配置信息*/
	char *identity;
	size_t identity_len;
#if 0	
    u8 *ssid;
	size_t ssid_len;
	int	ssid_method;
#endif
	u8	wapi_method;
	u8	psk_type;
	u8 *password;
	size_t password_len;
/*多个vap结构用于存储认证信息等*/
	apdata_info *wapid; 
/*记录上级的eloop结构*/
	void *eloop_save;
};

struct eloop_data
{
/*配置文件*/
	char fileconfig[256];
	int card_id;
	int debug;
	int stop;
	int quit;
/*套接字设置*/
	int 		socket_open;				/*套接子的状态*/
	u16		g_eth_proto;				/*协议*/
	int 		wai_raw_sk;				/*发送接收0x88b4帧的套接子*/
	int 		netlink_raw_sk ; 			/*接收driver消息的套接子*/
	struct rtnl_handle rth;
	int 		cgi_udp_sk ;				/*接收CGI消息的套接子*/
	int 		snmpd_udp_sk ;				/*接收CGI消息的套接子*/
	int 		as_udp_sk ;				/*与ASU通信的套接子*/
	int 		ioctl_fd ;					/*使用IOCTL 系统调用时的套接子*/
	char		*ctrl_interface;
	int		ctrl_sk;					/*WAPD----WAPITALK, Unix socket*/
	int		max_fd;
	fd_set 	read_fds;
	fd_set 	ip_recverr_fds;
/*证书和认证服务器设置*/
	struct sockaddr_in 	as_addr;			/*asu 的IP地址*/
	struct _apcert_info 	cert_info;			/*AE的证书*/
	wai_fixdata_cert 	ae_cert;			/**/
	wai_fixdata_id  	ae_id;			/*AE的身份*/
/*所有设备接口*/
	struct wapid_interfaces *vap_user;
	u8 has_cert;					/*ap是否有证书*/
};

#endif
