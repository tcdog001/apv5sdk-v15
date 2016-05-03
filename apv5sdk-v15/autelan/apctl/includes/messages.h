#ifndef __MESSAGES_H__
#define __MESSAGES_H__

/* elements ID */
#define MSG_ELEMENT_ADD_WLAN_REQUEST_TYPE                 18
#define MSG_ELEMENT_ADD_WLAN_RESPONSE_TYPE                23
#define MSG_ELEMENT_DEL_WLAN_REQUEST_TYPE                 32
#define MSG_ELEMENT_DEL_WLAN_RESPONSE_TYPE                33

#define MSG_ELEMENT_ADD_STA_TO_BALCKLIST_REQUEST_TYPE     36
#define MSG_ELEMENT_ADD_STA_TO_BALCKLIST_RESPONSE_TYPE    37
#define MSG_ELEMENT_DEL_STA_TO_BALCKLIST_REQUEST_TYPE     38
#define MSG_ELEMENT_DEL_STA_TO_BALCKLIST_RESPONSE_TYPE    39

#define MSG_ELEMENT_UPGRADE_REQUEST_TYPE                  44
#define MSG_ELEMENT_UPGRADE_RESPONSE_TYPE                 45

#define MSG_ELEMENT_STA_ASSOCIATION_REQUEST_TYPE          32001
//#define MSG_ELEMENT_STA_ASSOCIATION_RESPONSE_TYPE         45

#define MSG_ELEMENT_SET_NTP_REQUEST_TYPE                  65539
#define MSG_ELEMENT_SET_NTP_RESPONSE_TYPE                 65540

/* Begin: Add by dongzw for station isolation 2012-11-21 */
#define MSG_ELEMENT_SET_ISOLATION_REQUEST_TYPE			  65547
#define MSG_ELEMENT_SET_ISOLATION_RESPONSE_TYPE			  65548
/* End: Add by dongzw for station isolation 2012-11-21 */

#define MSG_ELEMENT_SYS_MAINTENANCE_REQUEST_TYPE          65551
#define MSG_ELEMENT_SYS_MAINTENANCE_RESPONSE_TYPE         65552

#define MSG_ELEMENT_SET_RADIO_REQUEST_TYPE                65553
#define MSG_ELEMENT_SET_RADIO_RESPONSE_TYPE               65554

#define MSG_ELEMENT_SET_STA_STATE_REQUEST_TYPE            65555
#define MSG_ELEMENT_SET_STA_STATE_RESPONSE_TYPE           65556

#define MSG_ELEMENT_SET_DATA_TUNNEL_REQUEST_TYPE          65557
#define MSG_ELEMENT_SET_DATA_TUNNEL_RESPONSE_TYPE         65558

#define MSG_ELEMENT_SET_WLAN_INFO_REQUEST_TYPE            65563
#define MSG_ELEMENT_SET_WLAN_INFO_RESPONSE_TYPE           65564

#define MSG_ELEMENT_REPORT_AP_STATE_REQUEST_TYPE          65568

#define MSG_ELEMENT_AP_ALIVE_REQUEST_TYPE                 65569
#define MSG_ELEMENT_AP_ALIVE_RESPONSE_TYPE                65570

#define MSG_ELEMENT_AP_REPORT_RADIO_INFO_REQUEST_TYPE     65571

#define MSG_ELEMENT_AP_REPORT_STA_INFO_REQUEST_TYPE       65572

#define MSG_ELEMENT_AP_REPORT_KEY_INFO_REQUEST_TYPE       65573

#define MSG_ELEMENT_NOTICE_DISCOVERY_AC_RESULT_REQUEST_TYPE      65574
#define MSG_ELEMENT_NOTICE_JOIN_AC_RESULT_REQUEST_TYPE           65575

#define MSG_ELEMENT_SET_RADIUS_REQUEST_TYPE               65576
#define MSG_ELEMENT_SET_RADIUS_RESPONSE_TYPE              65577


#define MSG_ELEMENT_NOTICE_JOIN_AC_RESULT_REQUEST_TYPE1          4


# define MSG_ELEMENT_AP_REPORT_ETH_INFO_REQUEST_TYPE            65586
# define MSG_ELEMENT_AP_REPORT_ATH_INFO_REQUEST_TYPE            65587


#define SIOCDEVPRIVATE	0x89F0	/* to 89FF */

/*******************ieee80211_ioctl.h**************************************/
#define	SIOCG80211STATS				(SIOCDEVPRIVATE+2)
#define	IEEE80211_IOCTL_GETKEY		(SIOCDEVPRIVATE+3)
#define	IEEE80211_IOCTL_GETWPAIE		(SIOCDEVPRIVATE+4)
#define	IEEE80211_IOCTL_STA_STATS	(SIOCDEVPRIVATE+5)
#define	IEEE80211_IOCTL_STA_INFO		(SIOCDEVPRIVATE+6)
#define	SIOC80211IFCREATE				(SIOCDEVPRIVATE+7)
#define	SIOC80211IFDESTROY			(SIOCDEVPRIVATE+8)
#define	IEEE80211_IOCTL_SCAN_RESULTS	(SIOCDEVPRIVATE+9)

#define IEEE80211_IOCTL_TUNNEL_BASE_USERS (SIOCDEVPRIVATE+22)

/*******************ieee80211_ioctl.h**************************************/

/********************peiwenhui add 0716 --rate&radio type set************************/
#define IEEE80211_PARAM_MODE_11b		0x01	/* 802.11b */
#define IEEE80211_PARAM_MODE_11a		0x02	/* 802.11a */
#define IEEE80211_PARAM_MODE_11g		0x04	/* 802.11g */
#define IEEE80211_PARAM_MODE_11bg		0x05	/* 802.11bg */
#define IEEE80211_PARAM_MODE_11n_a		0x08	/* 802.11n(5G) */
#define IEEE80211_PARAM_MODE_11an_a		0x0a	/* 802.11an */
#define IEEE80211_PARAM_MODE_11n_g		0x10	/*802.11n(2.4G)*/
#define IEEE80211_PARAM_MODE_11gn_g		0x14	/*802.11gn(2.4G)*/
#define IEEE80211_PARAM_MODE_11bgn_g	0x15	/*802.11bgn(2.4G)*/
/********************peiwenhui add 0716 --rate&radio type set************************/

/********************peiwenhui add 0716 --rate&radio type set************************/
#define REPORT_IEEE80211_PARAM_MODE_11a		0x01	/* 802.11a */
#define REPORT_IEEE80211_PARAM_MODE_11b		0x02	/* 802.11b */
#define REPORT_IEEE80211_PARAM_MODE_11g		0x04	/* 802.11g */
#define REPORT_IEEE80211_PARAM_MODE_11an	0x08	/*802.11an*/
#define REPORT_IEEE80211_PARAM_MODE_11gn	0x16	/*802.11gn*/
/********************peiwenhui add 0716 --rate&radio type set************************/


/*******************ieee80211_node.h**************************************/
/* pei add for gather station info at 090417 */
#define	IEEE80211_NODE_AUTH	0x0001		/* authorized for data */
#define	IEEE80211_NODE_QOS	0x0002		/* QoS enabled */
#define	IEEE80211_NODE_ERP	0x0004		/* ERP enabled */
/* NB: this must have the same value as IEEE80211_FC1_PWR_MGT */
#define	IEEE80211_NODE_PWR_MGT	0x0010		/* power save mode enabled */
#define	IEEE80211_NODE_AREF	0x0020		/* authentication ref held */
#define IEEE80211_NODE_UAPSD	0x0040		/* U-APSD power save enabled */
#define IEEE80211_NODE_UAPSD_TRIG 0x0080	/* U-APSD triggerable state */
#define IEEE80211_NODE_UAPSD_SP	0x0100		/* U-APSD SP in progress */
//#endif
/*******************ieee80211_node.h**************************************/


/*******************if_athioctl.h**************************************/
#ifdef __linux__
#define	SIOCGATHSTATS	(SIOCDEVPRIVATE+0)
#define	SIOCGATHDIAG	(SIOCDEVPRIVATE+1)
#define SIOCGATHRADARSIG (SIOCDEVPRIVATE+2)
//#else
//#define	SIOCGATHSTATS	_IOWR('i', 137, struct ifreq)
//#define	SIOCGATHDIAG	_IOWR('i', 138, struct ath_diag)
#endif
/*******************if_athioctl.h**************************************/


/* pei add for wapi , at 090505 */
/*设置WAI参数的IOCTL命令字*/
#define P80211_IOCTL_SETWAPI_INFO		(0x8BE0 + 0)

/*设置WAPI参数的IOCTL子命令字,表示设置WAPI状态*/
#define P80211_PACKET_WAPIFLAG			 112

/* ---- if type ---- */
#define MIB_IF_TYPE_ETHERNET	6
#define MIB_IF_TYPE_WIFI		41


#define ALARM_THRESHOLD 27

#define CPU_ALARM_THRESHOLD 90
#define MEM_ALARM_THRESHOLD 90


#define MAX_PACKET_SIZE 1300

#define COUNT_INTERVAL 180
#define SAMPLING_INTERVAL 60

typedef struct {
	unsigned char value;
	unsigned char radio_id;
	unsigned char wlan_id;
	unsigned char state;  /* 1-warn, 0-no */
	/* 4 bytes reserved */
	unsigned short sta_count;
	unsigned char *sta_mac;
} terminalDisturbInfo;

typedef struct {
	unsigned char optype;
	unsigned short int len;
	unsigned char sta_mac[6];
	unsigned int frames_recv;
	unsigned int frames_send;
	unsigned long long int bytes_recv;
	unsigned long long int bytes_send;
}staThroughputsendtoAc;

/*-------------------------------------------*/
struct nlmsghdr
{
	u32 nlmsg_len;
	u16 nlmsg_type;
	u16 nlmsg_flags;
	u32 nlmsg_seq;
	u32 nlmsg_pid;
};
struct rtattr
{
	unsigned short rta_len;
	unsigned short rta_type;
};

/* peiwenhui add 0703 --netlink */
#ifndef MSG_DONTWAIT
#define MSG_DONTWAIT 0
#endif
#ifndef IFLA_WIRELESS
#define IFLA_WIRELESS 11
#endif

#define NETLINK_ROUTE 0
#define RTMGRP_LINK 1
#define RTM_BASE 0x10
#define RTM_NEWLINK (RTM_BASE + 0)
#define NLMSG_ALIGNTO 4
#define NLMSG_ALIGN(len) (((len) + NLMSG_ALIGNTO - 1) & ~(NLMSG_ALIGNTO - 1))
#define NLMSG_LENGTH(len) ((len) + NLMSG_ALIGN(sizeof(struct nlmsghdr)))
#define NLMSG_DATA(nlh) ((void*) (((char*) nlh) + NLMSG_LENGTH(0)))

#define RTA_ALIGNTO 4
#define RTA_ALIGN(len) (((len) + RTA_ALIGNTO - 1) & ~(RTA_ALIGNTO - 1))
#define RTA_OK(rta,len) \
((len) > 0 && (rta)->rta_len >= sizeof(struct rtattr) && \
(rta)->rta_len <= (len))
#define RTA_NEXT(rta,attrlen) \
((attrlen) -= RTA_ALIGN((rta)->rta_len), \
(struct rtattr *) (((char *)(rta)) + RTA_ALIGN((rta)->rta_len)))

/* for wapi attack addr redirection trap -- pei add 20100911 */
/*Driver 发送的关于关联STA的信息*/								
struct asso_mt_t
{
	//struct nlmsghdr hdr;		/**/
	u16		type;		/*消息类型*/
	u16		data_len;		/*消息长度*/
#if 1  /*czm modified,temp*/
	u8		chan_old;
	u8		chan_curr;
#endif 
	u8 		ap_mac[6];
	u8 		pad1[2];
	u8		mac[6];		/*STA 的MAC地址*/
	u8 		pad[2];
	u8		gsn[16];		/*组播数据序号*/
	u8		wie[256];		/*wapi信息元素*/
};
typedef struct asso_mt_t asso_mt;

struct ifinfomsg
{
	unsigned char ifi_family;
	unsigned char __ifi_pad;
	unsigned short ifi_type;
	int ifi_index;
	unsigned ifi_flags;
	unsigned ifi_change;
};

struct sockaddr_nl
{
	sa_family_t nl_family;
	unsigned short nl_pad;
	u32 nl_pid;
	u32 nl_groups;
};

/* Size of the Event prefix (including padding and alignement junk) */
#define IW_EV_LCP_LEN	(sizeof(struct iw_event) - sizeof(union iwreq_data))

/* iw_point events are special. First, the payload (extra data) come at
 * the end of the event, so they are bigger than IW_EV_POINT_LEN. Second,
 * we omit the pointer, so start at an offset. */
#define IW_EV_POINT_OFF (((char *) &(((struct iw_point *) NULL)->length)) - \
			  (char *) NULL)
#define IW_EV_POINT_LEN	(IW_EV_LCP_LEN + sizeof(struct iw_point) - \
			 IW_EV_POINT_OFF)

/* ----------------------- WIRELESS EVENTS ----------------------- */
/* Those are *NOT* ioctls, do not issue request on them !!! */
/* Most events use the same identifier as ioctl requests */

#define IWEVTXDROP	0x8C00		/* Packet dropped to excessive retry */
#define IWEVQUAL	0x8C01		/* Quality part of statistics (scan) */
#define IWEVCUSTOM	0x8C02		/* Driver specific ascii string */
#define IWEVREGISTERED	0x8C03		/* Discovered a new node (AP mode) */
#define IWEVEXPIRED	0x8C04		/* Expired a node (AP mode) */
#define IWEVTRAFFIC 0x8C0B         /*station up/down line*/
#define IWEVGENIE	0x8C05		/* Generic IE (WPA, RSN, WMM, ..)
					 * (scan results); This includes id and
					 * length fields. One IWEVGENIE may
					 * contain more than one IE. Scan
					 * results may contain one or more
					 * IWEVGENIE events. */
#define IWEVMICHAELMICFAILURE 0x8C06	/* Michael MIC failure
					 * (struct iw_michaelmicfailure)
					 */
#define IWEVASSOCREQIE	0x8C07		/* IEs used in (Re)Association Request.
					 * The data includes id and length
					 * fields and may contain more than one
					 * IE. This event is required in
					 * Managed mode if the driver
					 * generates its own WPA/RSN IE. This
					 * should be sent just before
					 * IWEVREGISTERED event for the
					 * association. */
#define IWEVASSOCRESPIE	0x8C08		/* IEs used in (Re)Association
					 * Response. The data includes id and
					 * length fields and may contain more
					 * than one IE. This may be sent
					 * between IWEVASSOCREQIE and
					 * IWEVREGISTERED events for the
					 * association. */
#define IWEVPMKIDCAND	0x8C09		/* PMKID candidate for RSN
					 * pre-authentication
					 * (struct iw_pmkid_cand) */

#define IWEVFIRST	0x8C00
#define IW_EVENT_IDX(cmd)	((cmd) - IWEVFIRST)




#define		CREATE_PROTOCOL_MESSAGE(mess, size, err)		CREATE_OBJECT_SIZE_ERR(((mess).msg), (size), err);     \
															bzero(((mess).msg), (size));						   \
															(mess).len = 0;                                        \
															(mess).offset = 0;
#define		FREE_PROTOCOL_MESSAGE(mess)				    	FREE_OBJ(((mess).msg));							   	   \
															(mess).len = 0;                                        \
															(mess).offset = 0;


#define MAX_WEP_KEY                4

#ifdef AR11N
#define RADIO_NUM				   1
#else
#define RADIO_NUM				   2
#endif

/* peiwenhui add 090205 --- for get wlan throughput info */
#define _PATH_PROCNET_DEV               "/proc/net/dev"
#define is_space(c) ((((c) == ' ') || (((unsigned int)((c) - 9)) <= (13 - 9))))
#define NUM_OF_ATH_ALL_WIFI	16
#define NUM_OF_WIFI			2
#define NUM_OF_ETH			2


enum EventRespType {
	NEIGHBOR_AP_INFO_=1,//rename
	MONITOR,
	EXTRA_INFO,
	STA_INFO_,//rename
	IF_STATE,
	ATTACK_DETECT_INFO,
	STATION_WAPI_INFO,
	ALL_IF_INFO,
	NTP_REPORT,
	ATTACK_ADDR_RD,
	CHALLENGE_REPLAY,
	TERMINAL_DISTURB,
	STATHROUGHPUT_OPTYPE = 13
};

/*向Driver发送消息的消息结构*/
struct ioctl_drv
{
	u16  io_packet;
	struct  _iodata
	{
		u16 wDataLen;
		u8 pbData[96];
	}iodata;
}__attribute__ ((packed));


/* pei add for rogue AP test .  1118 */
enum opstate
{
	NEW=1,
	DELETE,
	MODIFIED,
	REMAIN,
};

enum interfaceType
{
	ATH=0,
	ETH,
	WIFI,
};


typedef enum
{
	BIT64         = 0,
	BIT128        = 1,
	BIT152        = 2
}CIPHER_BITS;
typedef enum
{
	ASSCII        = 0,
	HEX           = 1
}ENCRYPTION_KEY_T;
typedef enum
{
	CCMP          = 0,
	TKIP          = 1,
	AUTO          = 2
}ENCRYPTION_MODE;

typedef struct
{
	u8   len;
	u8   *key;
}KEY;
typedef struct
{
	u8		encryption_type;
	ENCRYPTION_KEY_T   key_t;
	u8                 index;
	KEY                key;                
	
}WAPI;

typedef struct
{
	CIPHER_BITS        bits;
	ENCRYPTION_KEY_T   key_t;
	u8                 index;
	KEY                wep_key[MAX_WEP_KEY];                
	
}WEP_11i;
typedef struct
{
	ENCRYPTION_MODE     mode;
	ENCRYPTION_KEY_T    key_t;
	KEY                 wpa_key;
}WPA_11i;

typedef struct WLAN_INFORMATION
{
	u8         rid;                              /* radio ID */
	u8         wid;                              /* wlan ID */
	u8		   hide_SSID;						 /* is hide SSID 0-hide */
	u8         ret;                              /* setting result */
	u8         security;
	u8         sec_policy;
	u8         max_users;
	u8		   Qos_switch;
	u8		   mac_percolate_rule;
	u16		   ability_info;
	u16		   vid;
	u8		   mac_mode;
	u8		   data_packet_method;
	u8         SSID_len;
	char       *SSID;
	u8         BSSID[MAC_ADDR_LEN];
	u32        SSID_flow_limit;
	u32        user_flow_limit;
	BOOL       is_exists;
	WAPI	   sec_wapi;
	WEP_11i    sec_wep;
	WPA_11i    sec_wpa;
	u8		   wlan_updown_time;
	u32		   rx_interval_bytes;				/* 抽样时间内无线上行端口流量 */
	u32		   tx_interval_bytes;				/* 抽样时间内无线下行端口流量 */
	u32		   hostapd_port;
	struct WLAN_INFORMATION *next;
}WLAN_INFO;

extern WLAN_INFO *wlan_list;

typedef struct RENEW_WLAN_INFORMATION
{
	u16		   type;
	u16		   length;
	u8		   rid;
	u8		   wid;
	u8         state;
	u8         tunnel;
	u32        SSID_flow_limit;
	u32        user_flow_limit;
	
	struct RENEW_WLAN_INFORMATION *next;
}RENEW_WLAN_INFO;

typedef struct RENEW_WLAN_CONFIGURE
{
	u16		   ver;
	u16        type;
	u8		   seq;
	u8		   ret;
	struct RENEW_WLAN_INFORMATION *wlan_info;
}RENEW_WLAN_CONF;


typedef struct MAINTENANCE_OPRATION
{
	u16		   ver;
	u8		   seq;
	u16		   type;
	u16		   length;
	u8         value;
}MAINTENANCE_OP;

typedef struct RADIO_INFOMATION
{
	u16			type;
	u16			length;
	u8			radio_id;
	u32			radio_type;
	u16			value;
	u32			rate_mask;
	u16			*rate;
	u8			*f;
	u8			result;
	struct RADIO_INFOMATION *next;
}RADIO_INFO;

typedef struct SET_RADIO_WIRELESS_PARAMETER
{
	u16			ver;
	u8			seq;
	u8			result;
	struct RADIO_INFOMATION *radio_info;
}SET_RADIO_PARAMETER;


typedef struct DATA_TUNNEL
{
	u16		   ver;
	u8		   seq;
	u16		   type;
	u16		   length;
	//u8         I_rev_TunT;
	u8         I;
	u8         TunT;
	u8         tunnel_mode;
	u32		   IPv4;
	u16		   port;
	u8		   result;
}DATA_T;

typedef struct AC_RESULT
{
	u16		   ver;
	u8		   seq;
	u16		   type;
	u16		   length;
	u16        result_code;
	u16        error_code;
	u32		   IPv4;
}AC_RES;

typedef struct AP_ABILITY_INFOMATION
{
	char	*net_code;
	u8		eth_num;
	char	*CPU_INFO;
	char 	*MEM_TYPE;
	u32		mem_size;
	u32		flash_size;
	u32		IPv4;
	u32		Mask;
	u32		GateWay;
	char	*dev_describe;
	char	*software_name;
}AP_ABILITY_INFO;

typedef struct AP_STATE_COUNT_INFOMATION
{
	u32		CPU_use_rate;
	u32		MEM_use_rate;
	u32		AP_flow;
	u8		AP_wireless_monitor_work_mode;
	u32		BSSID_Count;
	u32		terminal_disturb_alarm_threshold;
	u32		same_frequency_disturb_alarm_threshold;
	u32		adjacent_frequency_disturb_alarm_threshold;
	u32		CPU_use_rate_alarm_threshold;
	u32		MEM_use_rate_alarm_threshold;
	u32		flash_last_space;								/* FLASH卡剩余空间容量 */

	u32		CPU_use_rate_average;
	u32		MEM_use_rate_average;
	u32		connect_count;									/* 关联总次数 */
	u32		connect_failed_count;							/* 失败关联总次数 */
	u32		connect_retry_count;							/* 重新关联总次数 */
	u32		connect_undistinguish_retry_count;				/* 由于之前的关联无法识别与转移而导致重新关联失败的总次数 */
	u32		disconnect_unnormal_count;						/* 终端异常断开连接的总次数 */
	u32		connect_poor_resource_failed_count;				/* 由于接入点资源有限而拒绝关联的总次数 */
	u32		connect_no_support_rate_failed_count;			/* 因终端不支持基本速率集要求的速率而关联失败的总次数 */
	u32		connect_low_rssi_failed_count;					/* 因RSSI值过低而关联失败的总次数 */
	u32		CPU_use_rate_max;
	u32		MEM_use_rate_max;
	u32		run_time;
}AP_STATE_COUNT_INFO;

typedef struct WIRELESS_ABILITY_INFOMATION
{
	u8		radio_id;
	char	*if_info;						/* 接口描述 */
	u8		if_type;						/* 接口类型 */
	u32		if_MTU;							/* 接口最大传输单元 */
	u32		if_bandwidth;
	u8		*if_mac;
	u8		support_wireless_mode;
	int		max_txpower;
	int		power_attenuation_area;			/* 功率衰减范围 */
	int		power_adjust_interval;			/* 功率调整步长 */
	int		antenna;						/* 天线增益 */
	struct WIRELESS_ABILITY_INFOMATION *next;
}WIRELESS_ABILITY_INFO;

typedef struct WIRELESS_STATE_COUNT_INFOMATION
{
	u32		count;
	u8		radio_id;
	u32		beacon_frame_interval;				/* 信标帧的间隔 */
	u32		DTIM_interval;						/* DTIM间隔 */
	u32		RTS_threshold;						/* RTS阀值 */
	u32		frag_threshold;						/* 分片阀值 */
	u8		preamble_length;					/* preamble长度 */
	u32		current_txpower;					/* 当前AP的发射功率 */
	u8		current_channel;					/* 当前使用的无线信道 */
	u32		max_sta_num;						/* 允许的最大关联终端数量 */
	u8		current_wireless_mode;				/* 当前的无线模式 */
	char	*current_rate;						/* 当前传输速率集合 */
	u8		switch_AMPDU;
	u8		switch_20M_40M;
	u8		switch_shortGI;
	u8		switch_11n;
	char	wifi_state;							/* 接口当前状态 */
	u32		wifi_state_continuous_time;			/* 接口当前状态进入时间 */
	char *	SSID_list_around;					/* AP周围的SSID列表 */
	u32		auto_power_control_switch;			/* 是否开启自动功率控制 */
	u32		auto_wireless_channel_switch;		/* 无线信道自动选择开关 */

/* Begin: Modified by dongzw for signal strength message report 2013-04-11 */
	s8		rx_signal_average_strength;					/* 接收的信号平均强度 */
	s8		rx_signal_max_strength;						/* 接收的信号最大强度 */
	s8		rx_signal_min_strength;						/* 接收的信号最低强度 */
/* End: Modified by dongzw for signal strength message report 2013-04-11 */
	u32		wireless_if_updown_count;					/* 无线端口updown次数 */
	u32		tx_data_package;							/* 发送的数据包数，不包含重传 */
	u32		rx_data_package;							/* 接收的数据包数，不包含重传 */
	u64		rx_data_package_bytes;						/* 接收的数据包字节数，不包含重传 */
	u64		tx_data_package_bytes;						/* 发送的数据包字节数，不包含重传 */
	u32		wireless_channel_down_retry_data_package;	/* 无线信道下行重传的数据包数 */
	u32 	rx_manage_frame;							/* 接收的管理帧数 */
	u32 	rx_ctrl_frame;								/* 接收的控制帧数 */
	u32 	rx_data_frame;								/* 接收的数据帧数 */
	u32 	tx_manage_frame;							/* 发送的管理帧数 */
	u32 	tx_ctrl_frame;								/* 发送的控制帧数 */
	u32 	tx_data_frame;								/* 发送的数据帧数 */
	u32		wireless_channel_down_retry_frame;			/* 无线信道下行重传的帧数 */
	u32		rx_error_data_package;						/* 接收的错误数据包数 */
	u32		rx_error_frame;								/* 接收的错帧数 */
	u32		rx_fcs_mac_error_frame;						/* 接收的FCS MAC错帧数 */
	u32		rx_mic_mac_error_frame;						/* 接收的MIC MAC错帧数 */
	u32		rx_dissect_failed_mac_error_frame;			/* 接收的解密失败的MAC错帧数 */

	u32		tx_authenticate_frame;						/* 发送的认证帧的数量 */
	u32		tx_associate_frame;							/* 发送的关联帧的数量 */
	u32		rx_authenticate_frame;						/* 接收的认证帧的数量 */
	u32		rx_associate_frame;							/* 接收的关联帧的数量 */
	u32		tx_signaling_packet;						/* 发送的信令包数 */
	u32		rx_signaling_packet;						/* 接收的信令包数 */
	u32		associate_sta;								/* 关联终端数 */
	u32		tx_package;									/* 发送的包数 */
	u32		rx_package;									/* 接收的包数 */
	u32		frame_section_rate;							/* 信道上帧的分段速率 */
	u64		tx_package_bytes;							/* 发送的字节数 */
	u64		rx_package_bytes;							/* 接收的字节数 */
	u32		rx_phy_errors;								/* 物理层接收的错误包数 */
	u32		rx_unicast;									/* 上行的单播包数 */
	u32		tx_unicast;									/* 下行的单播包数 */
	u32		rx_ununicast;								/* 上行的非单播包数 */
	u32		tx_ununicast;								/* 下行的非单播包数 */
	u32		wireless_rx_interval_bytes;					/* 抽样时间内无线上行端口的流量 */
	u32		wireless_tx_interval_bytes;					/* 抽样时间内无线下行端口的流量 */
	u32		err_frame_ratio;							/* 信道的错帧率 */
	u32		retry_frame_ratio;							/* 信道上帧的重传率 */
	/* 信道接收的错帧数 */
	u32		drop_frame_ratio;							/* AP无线接口丢包率 */
	u32		rx_package_all;								/* AP运行时间内信道上行总的数据帧数，包括beacon帧 */
	u32		tx_package_all;								/* AP运行时间内信道下行总的数据帧数，包括beacon帧 */
	u32		tx_error_frame;								/* AP运行时间内信道下行总的错数据帧数 */


	u64		is_rx_mgmtbytes;//just save data, don't report
	u64		is_tx_mgmtbytes;//just save data, don't report
	struct WIRELESS_STATE_COUNT_INFOMATION *next;
}WIRELESS_STATE_COUNT_INFO;

typedef struct STA_ABILITY_INFOMATION
{
	u8		*mac;
	u8		is_Qos;					/* 终端WMM属性 */
}STA_ABILITY_INFO;

typedef struct STA_STATE_COUNT_INFOMATION
{
	u32		count;
	u8		radio_id;
	u8		wlan_id;
	u8		*mac;
	u8		is_Qos;					/* 终端WMM属性 */
	u32		IPv4;					/* 终端分配的IPv4 */
	u8		wireless_mode;			/* 采用的无线模式 */
	u8		wireless_channel;		/* 无线信道 */
	u8		is_power_save;			/* 节电状态 */
	u32		vlan_id;				/* 终端所在vlan id */
	u32		rate;					/* 终端当前接入速率 */
	char	rssi;					/* AP收到的终端当前信号强度 */
	char	Noise;					/* 连接用户噪声 */
	u32		rx_rate;				/* 连接用户接收速率 */
	u32		tx_rate;				/* 连接用户发送速率 */

	char	SNR;					/* AP接收到的终端当前信噪比 */
	u32		sta_rx_packet;			/* AP发送到终端的总包数 */
	u64		sta_rx_bytes;			/* AP发送到终端的总字节数 */
	u32		sta_tx_packet;			/* AP收到终端发送的的总包数 */
	u64		sta_tx_bytes;			/* AP收到终端发送的的总字节数 */
	u32		tx_frag_packets;		/* 终端发送的的被分片的包数 */
	u32		rx_frag_packets;		/* AP向终端发送的的被分片的包数 */
	u32		rx_error;				/* 连接用户接收的错误包 */
	u32		tx_error;				/* 连接用户发送的错误包 */
	u32		rx_drop;				/* 连接用户接收丢包 */
	u32		tx_drop;				/* 连接用户发送丢包 */
}STA_STATE_COUNT_INFO;

typedef struct AP_KEY_EVENT_INFOMATION
{
	u8		radio_id;
	u8		radio_frequency_switch;	/* 射频开关 */
	u16		switch_reason;			/* 打开、关闭原因 */
	u8		wlan_id;
	u16		delete_reason;			/* 删除原因 */
	char	*BSSID;
	struct AP_KEY_EVENT_INFOMATION *next;
}AP_KEY_EVENT_INFO;

typedef struct AP_ETH_ABILITY_INFOMATION
{
	u8		eth_id;
	char	*if_info;					/* 接口描述 */
	u8		if_type;					/* 接口类型 */
	u32		if_MTU;						/* max_transport_unit */
	u32		if_bandwidth;				/* 接口带宽 */
	char	*if_mac;					/* 接口MAC地址 */
}AP_ETH_ABILITY_INFO;

typedef struct AP_ETH_STATE_COUNT_INFOMATION
{	
	u8		eth_id;
	u32		eth_num;
	u32		rx_unicast_packet;			/* 接收单播数据包数 */
	u32		rx_ununicast_packet;		/* 接收非单播数据包数 */
	u64		rx_bytes;					/* 接收总字节数 */
	u32 	rx_drop_packet;				/* 丢弃接收数据包数 */
	u32 	rx_error_packet;			/* 接收错误数据包数 */
	u32		tx_unicast_packet;			/* 发送单播数据包数 */
	u32		tx_ununicast_packet;		/* 发送非单播数据包数 */
	u64		tx_bytes;					/* 发送总字节数 */
	u32 	tx_drop_packet;				/* 丢弃发送数据包数 */
	u32 	tx_error_packet;			/* 发送错误数据包数 */
	u32		if_up_down_count;			/* 接口up down次数 */
	u32		rx_multicast;				/* 接收的组播数据包数 */
	u32		rx_broadcast;				/* 接收的广播数据包数 */
	u32		tx_multicast;				/* 发送的组播数据包数 */
	u32		tx_broadcast;				/* 发送的广播数据包数 */
}AP_ETH_STATE_COUNT_INFO;

typedef struct AP_ATH_STATE_COUNT_INFOMATION
{
	u32		count;
	u8		radio_id;
	u8		wlan_id;
	char	*SSID;
	u8		SSID_switch;				/* 是否启用该SSID */
	u8		is_hide_SSID;				/* SSID是否隐藏(是否广播) */
	u8		is_separate;				/* 接入此SSID的终端间是否隔离 */
	u8		authorize_type_802_11;		/* 802.11鉴权:opensystem(开放系统) sharedkey(共享密钥) */
	u8		safe_type;					/* 安全类型 */
	u8		authorize_mode;				/* 认证模式 */
	u8		encryption_type;			/* 加密类型 */
	u32		vlan_mark;					/* vlan标识 */
	u32		max_sta_num;				/* 允许最大关联终端数 */
	u32		sta_uplink_max_speed;			/* 终端上行最大速率 */
	u32		sta_downlink_max_speed;			/* 终端下行最大速率 */
	u8		BSSID[MAC_ADDR_LEN];

	u32		tx_packet;					/* 发送的数据包数 */
	u32		rx_packet;					/* 接收的数据包数 */
	u64		rx_bytes;					/* 接收总字节数 */
	u64		tx_bytes;					/* 发送总字节数 */
	u32		downlink_retry_packet;		/* 下行重传的包数 */
	u32		tx_signaling_packet;						/* 发送的信令包数 */
	u32		rx_signaling_packet;						/* 接收的信令包数 */
	u32		rx_unicast;									/* 上行的单播包数 */
	u32		tx_unicast;									/* 下行的单播包数 */
	u32		rx_ununicast;								/* 上行的非单播包数 */
	u32		tx_ununicast;								/* 下行的非单播包数 */
	u32		frame_section_rate;							/* 帧的分段速率 */
	u32 		rx_ctrl_frame;								/* 接收的控制帧数 */
	u32 		rx_data_frame;								/* 接收的数据帧数 */	
	u32		rx_authenticate_frame;						/* 接收的认证帧的数量 */
	u32		rx_associate_frame;							/* 接收的关联帧的数量 */
	u32 		tx_ctrl_frame;								/* 发送的控制帧数 */
	u32 		tx_data_frame;								/* 发送的数据帧数 */
	u32		tx_authenticate_frame;						/* 发送的认证帧的数量 */
	u32		tx_associate_frame;							/* 发送的关联帧的数量 */
	u32		rx_errors;						/* 用户侧接受错误包数 */
	u32		crc_error;						/* CRC错误包数 */
	u32		phy_error;						/* PHY错误包数 */
	u32		mic_error;						/* MIC错误包数 */
	u32		key_error;						/* key解密错误包数 */
	u32		err_packet_ratio;				/* 用户侧误包率 */
	u32		hollowness_resource_use_radio;	/* 无线空口资源利用率 */
	u32		err_bytes_ratio;				/* 用户侧误比特率 */
	u32		rx_interval_bytes;				/* 抽样时间内无线上行端口流量 */
	u32		tx_interval_bytes;				/* 抽样时间内无线下行端口流量 */
	u32		success_associate;				/* 用户成功接入次数 */
	u32		response_associate;				/* 响应用户次数 */
	u32		request_associate;				/* 用户接入请求次数 */
	struct AP_ATH_STATE_COUNT_INFOMATION *next;
}AP_ATH_STATE_COUNT_INFO;


typedef struct NEIGHBOR_AP_INFORMATION
{      //pei add 1118
	u8		BSSID[6];	
	u16		Rate;
	u8		Channel;
	u8		RSSI;
	u8		NOISE;
	u8		BEACON_INT;
	u8		Opstatus;
	u16		CapabilityInfo;
	u8		EssidLen;
	u8		Radio_id;
	char	*ESSID;
	u8		IeLen;
	char	*IEs_INFO;
	struct NEIGHBOR_AP_INFORMATION *next;
}NEIGHBOR_AP_INFO;
extern NEIGHBOR_AP_INFO *neighbor_apinfo_list;

typedef struct
{
	unsigned int disassoc_unnormal;
	unsigned int rx_assoc_norate;
	unsigned int rx_assoc_capmismatch;
	unsigned int assoc_invalid;
	unsigned int reassoc_deny;
	unsigned int assoc_refuse_LowRssi;
} disassoc_errors;

typedef struct INTERVAL_COUNT_MONITOR_INFOMATION
{
	u32		count;
	u32		ap_interval_bytes;							/* AP采样时长内的流量值 */
	u32		wireless_rx_interval_bytes[2];				/* 无线采样时长内上行的流量值 */
	u32		wireless_tx_interval_bytes[2];				/* 无线采样时长内下行的流量值 */
	u32		CPU_use_rate_average;
	u32		MEM_use_rate_average;
/* Begin: Modified by dongzw for signal strength message report 2013-04-11 */
	s8		rx_signal_average_strength;					/* 接收的信号平均强度 */
	s8		rx_signal_max_strength;						/* 接收的信号最大强度 */
	s8		rx_signal_min_strength;						/* 接收的信号最低强度 */
/* End: Modified by dongzw for signal strength message report 2013-04-11 */
}INTERVAL_COUNT_MONITOR_INFO;
u32 	CPU_use_rate_max;
u32 	MEM_use_rate_max;


/**********************************************************************************************/
#define MAX_BB_PANICS 3
struct ath_bb_panic_info {
    int valid;
    u32 status;
    u32 tsf;
    u32 phy_panic_wd_ctl1;
    u32 phy_panic_wd_ctl2;
    u32 phy_gen_ctrl;
    u32 rxc_pcnt;
    u32 rxf_pcnt;
    u32 txf_pcnt;
    u32 cycles;
    u32 wd;
    u32 det;
    u32 rdar;
    u32 rODFM;
    u32 rCCK;
    u32 tODFM;
    u32 tCCK;
    u32 agc;
    u32 src;
};

struct ath_11n_stats {
    u32   tx_pkts;            /* total tx data packets */
    u32   tx_checks;          /* tx drops in wrong state */
    u32   tx_drops;           /* tx drops due to qdepth limit */
    u32   tx_minqdepth;       /* tx when h/w queue depth is low */
    u32   tx_queue;           /* tx pkts when h/w queue is busy */
    u32   tx_resetq;          /* tx reset queue instances */
    u32   tx_comps;           /* tx completions */
    u32   tx_comperror;       /* tx error completions on global failures */
    u32   tx_unaggr_comperror; /* tx error completions of unaggregted frames */
    u32   tx_stopfiltered;    /* tx pkts filtered for requeueing */
    u32   tx_qnull;           /* txq empty occurences */
    u32   tx_noskbs;          /* tx no skbs for encapsulations */
    u32   tx_nobufs;          /* tx no descriptors */
    u32   tx_badsetups;       /* tx key setup failures */
    u32   tx_normnobufs;      /* tx no desc for legacy packets */
    u32   tx_schednone;       /* tx schedule pkt queue empty */
    u32   tx_bars;            /* tx bars sent */
    u32   tx_legacy;          /* tx legacy frames sent */
    u32   txunaggr_single;    /* tx unaggregate singles sent */
    u32   txbar_xretry;       /* tx bars excessively retried */
    u32   txbar_compretries;  /* tx bars retried */
    u32   txbar_errlast;      /* tx bars last frame failed */
    u32   tx_compunaggr;      /* tx unaggregated frame completions */
    u32   txunaggr_xretry;    /* tx unaggregated excessive retries */
    u32   tx_compaggr;        /* tx aggregated completions */
    u32   tx_bawadv;          /* tx block ack window advanced */
    u32   tx_bawretries;      /* tx block ack window retries */
    u32   tx_bawnorm;         /* tx block ack window additions */
    u32   tx_bawupdates;      /* tx block ack window updates */
    u32   tx_bawupdtadv;      /* tx block ack window advances */
    u32   tx_retries;         /* tx retries of sub frames */
    u32   tx_xretries;        /* tx excessive retries of aggregates */
    u32   tx_aggregates;      /* tx aggregated pkts sent */
    u32   tx_sf_hw_xretries;  /* sub-frames excessively retried in h/w */
    u32   tx_aggr_frames;     /* tx total frames aggregated */
    u32   txaggr_noskbs;      /* tx no skbs for aggr encapsualtion */
    u32   txaggr_nobufs;      /* tx no desc for aggr */
    u32   txaggr_badkeys;     /* tx enc key setup failures */
    u32   txaggr_schedwindow; /* tx no frame scheduled: baw limited */
    u32   txaggr_single;      /* tx frames not aggregated */
    u32   txaggr_mimo;        /* tx frames aggregated for mimo */
    u32   txaggr_compgood;    /* tx aggr good completions */
    u32   txaggr_comperror;   /* tx aggr error completions */
    u32   txaggr_compxretry;  /* tx aggr excessive retries */
    u32   txaggr_compretries; /* tx aggr unacked subframes */
    u32   txunaggr_compretries; /* tx non-aggr unacked subframes */
    u32   txaggr_prepends;    /* tx aggr old frames requeued */
    u32   txaggr_filtered;    /* filtered aggr packet */
    u32   txaggr_fifo;        /* fifo underrun of aggregate */
    u32   txaggr_xtxop;       /* txop exceeded for an aggregate */
    u32   txaggr_desc_cfgerr; /* aggregate descriptor config error */
    u32   txaggr_data_urun;   /* data underrun for an aggregate */
    u32   txaggr_delim_urun;  /* delimiter underrun for an aggregate */
    u32   txaggr_errlast;     /* tx aggr: last sub-frame failed */
    u32   txunaggr_errlast;   /* tx non-aggr: last frame failed */
    u32   txaggr_longretries; /* tx aggr h/w long retries */
    u32   txaggr_shortretries;/* tx aggr h/w short retries */
    u32   txaggr_timer_exp;   /* tx aggr : tx timer expired */
    u32   txaggr_babug;       /* tx aggr : BA bug */
    u32   txrifs_single;      /* tx frames not bursted */
    u32   txrifs_babug;       /* tx rifs : BA bug */
    u32   txaggr_badtid;      /* tx aggr : Bad TID */
    u32   txrifs_compretries; /* tx rifs unacked subframes */
    u32   txrifs_bar_alloc;   /* tx rifs bars allocated */
    u32   txrifs_bar_freed;   /* tx rifs bars freed */
    u32   txrifs_compgood;    /* tx rifs good completions */
    u32   txrifs_prepends;    /* tx rifs old frames requeued */
    u32   tx_comprifs;        /* tx rifs completions */
    u32   tx_compnorifs;      /* tx not a rifs completion */
    u32   rx_pkts;            /* rx pkts */
    u32   rx_aggr;            /* rx aggregated packets */
    u32   rx_aggrbadver;      /* rx pkts with bad version */
    u32   rx_bars;            /* rx bars */
    u32   rx_nonqos;          /* rx non qos-data frames */
    u32   rx_seqreset;        /* rx sequence resets */
    u32   rx_oldseq;          /* rx old packets */
    u32   rx_bareset;         /* rx block ack window reset */
    u32   rx_baresetpkts;     /* rx pts indicated due to baw resets */
    u32   rx_dup;             /* rx duplicate pkts */
    u32   rx_baadvance;       /* rx block ack window advanced */
    u32   rx_recvcomp;        /* rx pkt completions */
    u32   rx_bardiscard;      /* rx bar discarded */
    u32   rx_barcomps;        /* rx pkts unblocked on bar reception */
    u32   rx_barrecvs;        /* rx pkt completions on bar reception */
    u32   rx_skipped;         /* rx pkt sequences skipped on timeout */
    u32   rx_comp_to;         /* rx indications due to timeout */
    u32   rx_timer_starts;    /* rx countdown timers started */
    u32   rx_timer_stops;     /* rx countdown timers stopped */ 
    u32   rx_timer_run;       /* rx timeout occurences */
    u32   rx_timer_more;      /* rx partial timeout of pending packets */
    u32   wd_tx_active;       /* watchdog: tx is active */
    u32   wd_tx_inactive;     /* watchdog: tx is not active */
    u32   wd_tx_hung;         /* watchdog: tx is hung */
    u32   wd_spurious;        /* watchdog: spurious tx hang */
    u32   tx_requeue;         /* filter & requeue on 20/40 transitions */
    u32   tx_drain_txq;       /* draining tx queue on error */
    u32   tx_drain_tid;       /* draining tid buf queue on error */
    u32   tx_cleanup_tid;     /* draining tid buf queue on node cleanup */
    u32   tx_drain_bufs;      /* buffers drained from pending tid queue */
    u32   tx_tidpaused;       /* pausing tx on tid */
    u32   tx_tidresumed;      /* resuming tx on tid */
    u32   tx_unaggr_filtered; /* unaggregated tx pkts filtered */
    u32   tx_aggr_filtered;   /* aggregated tx pkts filtered */
    u32   tx_filtered;        /* total sub-frames filtered */
    u32   rx_rb_on;           /* total rb-s on  */
    u32   rx_rb_off;          /* total rb-s off */
    u32   rx_dsstat_err;      /* rx descriptor status corrupted */
    u32   bf_stream_miss;     /* beamform stream mismatch */ 
    u32   bf_bandwidth_miss;  /* beamform bandwidth mismatch */ 
    u32   bf_destination_miss;/* beamform destination mismatch */ 
};

struct ath_stats {
    u32   ast_watchdog;                   /* device reset by watchdog */
    u32   ast_resetOnError;               /*No of resets on error*/
    u32   ast_hardware;                   /* fatal hardware error interrupts */
    u32   ast_bmiss;                      /* beacon miss interrupts */
    u32   ast_rxorn;                      /* rx overrun interrupts */
    u32   ast_rxorn_bmiss;                /* rx overrun and bmiss interrupts: indicate descriptor corruption */
    u32   ast_rxeol;                      /* rx eol interrupts */
    u32   ast_txurn;                      /* tx underrun interrupts */
    u32   ast_txto;                       /* tx timeout interrupts */
    u32   ast_cst;                        /* carrier sense timeout interrupts */
    u32   ast_mib;                        /* mib interrupts */
    u32    ast_rx;           /* rx interrupts */
    u32    ast_rxdesc;       /* rx descriptor interrupts */
    u32    ast_rxerr;        /* rx error interrupts */
    u32    ast_rxnofrm;      /* rx no frame interrupts */
    u32    ast_tx;           /* tx interrupts */
    u32    ast_txdesc;       /* tx descriptor interrupts */
    u32    ast_tim_timer;    /* tim timer interrupts */
    u32    ast_bbevent;      /* baseband event interrupts */
    u32    ast_rxphy;        /* rx phy error interrupts */
    u32    ast_rxkcm;        /* rx key cache miss interrupts */
    u32    ast_swba;         /* sw beacon alert interrupts */
    u32    ast_bnr;          /* beacon not ready interrupts */
    u32    ast_tim;          /* tim interrupts */
    u32    ast_dtim;         /* dtim interrupts */
    u32    ast_dtimsync;     /* dtimsync interrupts */
    u32    ast_gpio;         /* general purpose IO interrupts */
    u32    ast_cabend;       /* cab end interrupts */
    u32    ast_tsfoor;       /* tsf out-of-range interrupts */
    u32    ast_gentimer;     /* generic timer interrupts */
    u32    ast_gtt;          /* global transmit timeout interrupts */
    u32    ast_fatal;        /* fatal interrupts */
    u32   ast_tx_packets;                 /* packet sent on the interface */
    u32   ast_rx_packets;                 /* packet received on the interface */
    u32   ast_tx_mgmt;                    /* management frames transmitted */
    u32   ast_tx_discard;                 /* frames discarded prior to assoc */
    u32   ast_tx_invalid;                 /* frames discarded 'cuz device gone */
    u32   ast_tx_qstop;                   /* tx queue stopped 'cuz full */
    u32   ast_tx_encap;                   /* tx encapsulation failed */
    u32   ast_tx_nonode;                  /* tx failed 'cuz no node */
    u32   ast_tx_nobuf;                   /* tx failed 'cuz no tx buffer (data) */
    u32    ast_tx_stop;      /* number of times the netif_stop called*/
    u32    ast_tx_resume;    /* number of times netif_wake_queue called */
    u32    ast_tx_nobufmgt;  /* tx failed 'cuz no tx buffer (mgmt)*/
    u32    ast_tx_xretries;  /* tx failed 'cuz too many retries */
    u32    ast_tx_fifoerr;   /* tx failed 'cuz FIFO underrun */
    u32    ast_tx_filtered;  /* tx failed 'cuz xmit filtered */
    u32    ast_tx_badrate;   /* tx failed 'cuz bogus xmit rate */
    u32    ast_tx_noack;     /* tx frames with no ack marked */
    u32    ast_tx_cts;       /* tx frames with cts enabled */
    u32    ast_tx_shortpre;  /* tx frames with short preamble */
    u32    ast_tx_altrate;   /* tx frames with alternate rate */
    u32    ast_tx_protect;   /* tx frames with protection */
    u32    ast_rx_orn;       /* rx failed 'cuz of desc overrun */
    u32    ast_rx_badcrypt;  /* rx failed 'cuz decryption */
    u32    ast_rx_badmic;    /* rx failed 'cuz MIC failure */
    u32    ast_rx_nobuf;     /* rx setup failed 'cuz no skbuff */
    u32       ast_rx_hal_in_progress;
    u32       ast_rx_num_data;
    u32       ast_rx_num_mgmt;
    u32       ast_rx_num_ctl;
    u32       ast_rx_num_unknown;
    u32       ast_max_pkts_per_intr;
#define ATH_STATS_MAX_INTR_BKT  512
    u32   ast_pkts_per_intr[ATH_STATS_MAX_INTR_BKT+1]; /* counter bucket of packets handled in a single iteration */
    char      ast_tx_rssi;                    /* tx rssi of last ack */
    char      ast_tx_rssi_ctl0;               /* tx rssi of last ack [ctl, chain 0] */
    char      ast_tx_rssi_ctl1;               /* tx rssi of last ack [ctl, chain 1] */
    char      ast_tx_rssi_ctl2;               /* tx rssi of last ack [ctl, chain 2] */
    char      ast_tx_rssi_ext0;               /* tx rssi of last ack [ext, chain 0] */
    char      ast_tx_rssi_ext1;               /* tx rssi of last ack [ext, chain 1] */
    char      ast_tx_rssi_ext2;               /* tx rssi of last ack [ext, chain 2] */
    char      ast_rx_rssi;                    /* rx rssi from histogram [combined]*/
    char      ast_rx_rssi_ctl0;               /* rx rssi from histogram [ctl, chain 0] */
    char      ast_rx_rssi_ctl1;               /* rx rssi from histogram [ctl, chain 1] */
    char      ast_rx_rssi_ctl2;               /* rx rssi from histogram [ctl, chain 2] */
    char      ast_rx_rssi_ext0;               /* rx rssi from histogram [ext, chain 0] */
    char      ast_rx_rssi_ext1;               /* rx rssi from histogram [ext, chain 1] */
    char      ast_rx_rssi_ext2;               /* rx rssi from histogram [ext, chain 2] */
    u32   ast_be_xmit;	                /* beacons transmitted */
    u32   ast_be_nobuf;	                /* no skbuff available for beacon */
    u32   ast_per_cal;	                /* periodic calibration calls */
    u32   ast_per_calfail;                /* periodic calibration failed */
    u32   ast_per_rfgain;	                /* periodic calibration rfgain reset */
    u32   ast_rate_calls;	                /* rate control checks */
    u32   ast_rate_raise;	                /* rate control raised xmit rate */
    u32   ast_rate_drop;	                /* rate control dropped xmit rate */
    u32   ast_ant_defswitch;              /* rx/default antenna switches */
    u32   ast_ant_txswitch;               /* tx antenna switches */
	/*lyb add for wifi stats*/
	u32	ast_up_times;	/* wifi up times.*/
 	u32	ast_down_times;	/* wifi down times.*/
	u32 ast_rx_phyerr;	/* rx PHY error summary count */
	u32 ast_rx_crcerr;	/* rx failed 'cuz of bad CRC */
	u32  ast_rx_mgt;
	/*lyb add end*/
    u32   ast_ant_rx[8];	                /* rx frames with antenna */
    u32   ast_ant_tx[8];	                /* tx frames with antenna */
    u64   ast_rx_bytes;	                /* total number of bytes received */
    u64   ast_tx_bytes;	                /* total number of bytes transmitted */
    u32   ast_rx_num_qos_data[16];        /* per tid rx packets (includes duplicates)*/
    u32   ast_rx_num_nonqos_data;         /* non qos rx packets    */
    u32   ast_txq_packets[16];            /* perq packets sent on the interface for each category */
    u32   ast_txq_xretries[16];           /* per q tx failed 'cuz too many retries */
    u32   ast_txq_fifoerr[16];            /* per q tx failed 'cuz FIFO underrun */
    u32   ast_txq_filtered[16];           /*per q  tx failed 'cuz xmit filtered */
    u32   ast_txq_athbuf_limit[16];       /* tx dropped 'cuz of athbuf limit */
    u32   ast_txq_nobuf[16];              /* tx dropped 'cuz no athbufs */
    u8    ast_num_rxchain;                /* Number of rx chains */
    u8    ast_num_txchain;                /* Number of tx chains */
    struct ath_11n_stats ast_11n_stats;         /* 11n statistics */
    u32   ast_bb_hang;                    /* BB hang detected */
    u32   ast_mac_hang;                   /* MAC hang detected */
    u32   ast_uapsdqnulbf_unavail;        /* no qos null buffers available */
    u32   ast_uapsdqnul_pkts;             /* count of qos null frames sent */
    u32   ast_uapsdtriggers;              /* count of UAPSD triggers received */
    u32   ast_uapsdnodeinvalid;           /* count of triggers for non-UAPSD node */
    u32   ast_uapsdeospdata;              /* count of QoS Data with EOSP sent */
    u32   ast_uapsddata_pkts;             /* count of UAPSD QoS Data frames sent */
    u32   ast_uapsddatacomp;              /* count of UAPSD QoS Data frames completed */
    u32   ast_uapsdqnulcomp;              /* count of UAPSD QoS NULL frames completed */
    u32   ast_uapsddataqueued;            /* count of UAPSD QoS Data Queued */
	#define MCS_RATE 0x1f
    u8    ast_txbf;
    u8    ast_lastratecode;
    u32   ast_sounding_count;
    u32   ast_txbf_rpt_count;
    u32   ast_mcs_count[MCS_RATE+1];
	 struct ath_bb_panic_info ast_bb_panic[MAX_BB_PANICS];
};

struct ieee80211_mac_stats {
    u64   ims_tx_packets; /* frames successfully transmitted */
    u64   ims_rx_packets; /* frames successfully received */
    u64   ims_tx_bytes;	/* bytes successfully transmitted */
    u64	ims_rx_bytes;   /* bytes successfully received */

	/*pengruofeng add retry frame stats 2011-5-11*/
	u64	ims_rx_retry_packets;
	u64	ims_rx_retry_bytes;
	u64	ims_tx_retry_packets;
	u64	ims_tx_retry_bytes;
	/*pengruo add end 2011-5-11*/
	u64	ims_rx_retry_mgmt_packets;
	u64	ims_tx_retry_mgmt_packets;
	u64	ims_rx_retry_mgmt_bytes;
	u64	ims_tx_retry_mgmt_bytes;
    /* Decryption errors */
    u64   ims_rx_unencrypted; /* rx w/o wep and privacy on */
    u64   ims_rx_badkeyid;    /* rx w/ incorrect keyid */
    u64   ims_rx_decryptok;   /* rx decrypt okay */
    u64   ims_rx_decryptcrc;  /* rx decrypt failed on crc */
    u64   ims_rx_wepfail;     /* rx wep processing failed */
    u64   ims_rx_tkipreplay;  /* rx seq# violation (TKIP) */
    u64   ims_rx_tkipformat;  /* rx format bad (TKIP) */
    u64   ims_rx_tkipmic;     /* rx MIC check failed (TKIP) */
    u64   ims_rx_tkipicv;     /* rx ICV check failed (TKIP) */
    u64   ims_rx_ccmpreplay;  /* rx seq# violation (CCMP) */
    u64   ims_rx_ccmpformat;  /* rx format bad (CCMP) */
    u64   ims_rx_ccmpmic;     /* rx MIC check failed (CCMP) */
/*this file can be included by applications as 80211stats that has no such MACRO definition*/
//#if ATH_SUPPORT_WAPI 
    u64   ims_rx_wpireplay;  /* rx seq# violation (WPI) */
    u64   ims_rx_wpimic;     /* rx MIC check failed (WPI) */
//#endif
    /* Other Tx/Rx errors */
    u64   ims_tx_discard;     /* tx dropped by NIC */
    u64   ims_rx_discard;     /* rx dropped by NIC */

    u64   ims_rx_countermeasure; /* rx TKIP countermeasure activation count */
};


#ifdef AR11N
struct ieee80211_stats
{
	u32	is_rx_badversion;	/* rx frame with bad version */
	u32	is_rx_tooshort;		/* rx frame too short */
	u32	is_rx_wrongbss;		/* rx from wrong bssid */
	u32	is_rx_dup;		/* rx discard 'cuz dup */
	u32	is_rx_wrongdir;		/* rx w/ wrong direction */
	u32	is_rx_mcastecho;	/* rx discard 'cuz mcast echo */
	u32	is_rx_notassoc;		/* rx discard 'cuz sta !assoc */
	u32	is_rx_noprivacy;	/* rx w/ wep but privacy off */
	u32	is_rx_unencrypted;	/* rx w/o wep and privacy on */
	u32	is_rx_wepfail;		/* rx wep processing failed */
	u32	is_rx_decap;		/* rx decapsulation failed */
	u32	is_rx_mgtdiscard;	/* rx discard mgt frames */
	u32	is_rx_ctl;		/* rx discard ctrl frames */
	u32	is_rx_beacon;		/* rx beacon frames */
	u32	is_rx_rstoobig;		/* rx rate set truncated */
	u32	is_rx_elem_missing;	/* rx required element missing*/
	u32	is_rx_elem_toobig;	/* rx element too big */
	u32	is_rx_elem_toosmall;	/* rx element too small */
	u32	is_rx_elem_unknown;	/* rx element unknown */
	u32	is_rx_badchan;		/* rx frame w/ invalid chan */
	u32	is_rx_chanmismatch;	/* rx frame chan mismatch */
	u32	is_rx_nodealloc;	/* rx frame dropped */
	u32	is_rx_ssidmismatch;	/* rx frame ssid mismatch  */
	u32	is_rx_auth_unsupported;	/* rx w/ unsupported auth alg */
	u32	is_rx_auth_fail;	/* rx sta auth failure */
	u32	is_rx_auth_countermeasures;/* rx auth discard 'cuz CM */
	u32	is_rx_assoc_bss;	/* rx assoc from wrong bssid */
	u32	is_rx_assoc_notauth;	/* rx assoc w/o auth */
	u32	is_rx_assoc_capmismatch;/* rx assoc w/ cap mismatch */
	u32	is_rx_assoc_norate;	/* rx assoc w/ no rate match */
	u32	is_rx_assoc_badwpaie;	/* rx assoc w/ bad WPA IE */
	/*lyb add start for ath stats*/
	u32 is_rx_bad_assoc;
	u32 is_assoc_success;
	u32 is_rx_reassoc;
	u32 is_rx_reassoc_success;
	u32 is_rx_bad_reassoc;
	u32 is_rx_reassoc_bss;	
	u32 is_rx_reassoc_notauth;	
	u32 is_rx_reassoc_capmismatch;
	u32 is_rx_reassoc_norate;	
	u32 is_rx_reassoc_badwpaie;	
	u32 is_rx_reassoc_badscie;
	
	u32 is_disassoc_unnormal;
	u32 is_disassoc_state;
	u32 is_disassoc_ioctl;

	u32	is_tx_auth_assoc_fail;

	u32 is_deauth_ioctl;
	u32 is_deauth_unnormal;
	u32 is_deauth_state;
	u32 is_deauth_expire;
	/*lyb add end*/
	u32	is_rx_deauth;		/* rx deauthentication */
	u32	is_rx_disassoc;		/* rx disassociation */
	u32   is_rx_action;       /* rx action mgt */
	u32	is_rx_badsubtype;	/* rx frame w/ unknown subtype*/
	u32	is_rx_nobuf;		/* rx failed for lack of buf */
	u32	is_rx_decryptcrc;	/* rx decrypt failed on crc */
	u32	is_rx_ahdemo_mgt;	/* rx discard ahdemo mgt frame*/
	u32	is_rx_bad_auth;		/* rx bad auth request */
	u32	is_rx_unauth;		/* rx on unauthorized port */
	u32	is_rx_badkeyid;		/* rx w/ incorrect keyid */
	u32	is_rx_ccmpreplay;	/* rx seq# violation (CCMP) */
	u32	is_rx_ccmpformat;	/* rx format bad (CCMP) */
	u32	is_rx_ccmpmic;		/* rx MIC check failed (CCMP) */
	u32	is_rx_tkipreplay;	/* rx seq# violation (TKIP) */
	u32	is_rx_tkipformat;	/* rx format bad (TKIP) */
	u32	is_rx_tkipmic;		/* rx MIC check failed (TKIP) */
	u32	is_rx_tkipicv;		/* rx ICV check failed (TKIP) */
	u32	is_rx_badcipher;	/* rx failed 'cuz key type */
	u32	is_rx_nocipherctx;	/* rx failed 'cuz key !setup */
	u32	is_rx_acl;		/* rx discard 'cuz acl policy */
	u32	is_rx_ffcnt;		/* rx fast frames */
	u32	is_rx_badathtnl;   	/* driver key alloc failed */
	u32	is_tx_nobuf;		/* tx failed for lack of buf */
	u32	is_tx_nonode;		/* tx failed for no node */
	u32	is_tx_unknownmgt;	/* tx of unknown mgt frame */
	u32	is_tx_badcipher;	/* tx failed 'cuz key type */
	u32	is_tx_nodefkey;		/* tx failed 'cuz no defkey */
	u32	is_tx_noheadroom;	/* tx failed 'cuz no space */
	u32	is_tx_ffokcnt;		/* tx fast frames sent success */
	u32	is_tx_fferrcnt;		/* tx fast frames sent success */
	u32	is_scan_active;		/* active scans started */
	u32	is_scan_passive;	/* passive scans started */
	u32	is_node_timeout;	/* nodes timed out inactivity */
	u32 is_node_timeout_auth;		/*lyb add nodes timed out inactivity at auth state*/
	u32 is_node_timeout_assoc;	/* lyb add nodes timed out inactivity at assoc state */
	u32	is_crypto_nomem;	/* no memory for crypto ctx */
	u32	is_crypto_tkip;		/* tkip crypto done in s/w */
	u32	is_crypto_tkipenmic;	/* tkip en-MIC done in s/w */
	u32	is_crypto_tkipdemic;	/* tkip de-MIC done in s/w */
	u32	is_crypto_tkipcm;	/* tkip counter measures */
	u32	is_crypto_ccmp;		/* ccmp crypto done in s/w */
	u32	is_crypto_wep;		/* wep crypto done in s/w */
	u32	is_crypto_setkey_cipher;/* cipher rejected key */
	u32	is_crypto_setkey_nokey;	/* no key index for setkey */
	u32	is_crypto_delkey;	/* driver key delete failed */
	u32	is_crypto_badcipher;	/* unknown cipher */
	u32	is_crypto_nocipher;	/* cipher not available */
	u32	is_crypto_attachfail;	/* cipher attach failed */
	u32	is_crypto_swfallback;	/* cipher fallback to s/w */
	u32	is_crypto_keyfail;	/* driver key alloc failed */
	u32	is_crypto_enmicfail;	/* en-MIC failed */
	u32	is_ibss_capmismatch;	/* merge failed-cap mismatch */
	u32	is_ibss_norate;		/* merge failed-rate mismatch */
	u32	is_ps_unassoc;		/* ps-poll for unassoc. sta */
	u32	is_ps_badaid;		/* ps-poll w/ incorrect aid */
	u32	is_ps_qempty;		/* ps-poll w/ nothing to send */
	/*lyb add for ath stats*/
	u32 is_rx_mgmt;
	u32 is_tx_mgmt;
/* lisongbai add for count rx/tx_mgmtbytes in 2011-01-20 */
	u64	is_rx_mgmtbytes;
	u64	is_tx_mgmtbytes;        
/*lisongbai add end */
	u32 is_assocs;
	u32 is_reassocs;
	u32 is_rx_repack;		/*终端重传的包数*/
	u32 is_rx_rebytes;	/*终端重传的字节数*/
	u32 is_rx_err_pack;				/*接收到的来自终端的错误包数*/
	u32 is_tx_signaling;				/*发送到指定终端的信令包数*/
	u32 is_rx_signaling;				/*终端传送的信令包数*/
	u32 is_re_wpi;/*根据重放机制丢弃的WPI MPDU的数目*/
	u32 is_wpi_mic;  /* WPI-SMS4解密时没有有效的密钥而丢弃的MPDU数目*/
	u32 is_wpi_no_key_error;/* WPI-SMS4解密时MIC校验出错而丢弃的MPDU数目*/
	u32	is_rx_auth_try;	/*add wyh@20080716 for snmp*/
	u32	is_rx_auth_ok;	/*add wyh@20080716 for snmp*/
	u32	is_rx_assoc_try;	/*add wyh@20080716 for snmp*/
	u32	is_rx_ap_too_many_sta;	/*add wyh@20080716 for snmp*/
	/*lyb add for ath stats*/
	/*Begin:Added by duanmingzhe for counting of error packets 2010-09-09*/
	u32 is_rx_errors;
	/*End:Added by duanmingzhe for counting of error packets 2010-09-09*/
	/*Begin:Added by duanmingzhe for counting of error frames 2010-09-20*/
	u32 is_rx_error_frames;
	/*End:Added by duanmingzhe for counting of error frames 2010-09-20*/
};
#else
struct ieee80211_stats
{
	u32			is_rx_badversion;	/* rx frame with bad version */
	u32			is_rx_tooshort;		/* rx frame too short */
	u32			is_rx_wrongbss;		/* rx from wrong bssid */
	u32			is_rx_wrongdir;		/* rx w/ wrong direction */
	u32			is_rx_mcastecho;	/* rx discard 'cuz mcast echo */
	u32			is_rx_notassoc;		/* rx discard 'cuz sta !assoc */
	u32			is_rx_noprivacy;	/* rx w/ wep but privacy off */
	u32			is_rx_decap;		/* rx decapsulation failed */
	u32			is_rx_mgtdiscard;	/* rx discard mgt frames */
	u32			is_rx_ctl;		/* rx discard ctrl frames */
	u32			is_rx_beacon;		/* rx beacon frames */
	u32			is_rx_rstoobig;		/* rx rate set truncated */
	u32			is_rx_elem_missing;	/* rx required element missing*/
	u32			is_rx_elem_toobig;	/* rx element too big */
	u32			is_rx_elem_toosmall;	/* rx element too small */
	u32			is_rx_elem_unknown;	/* rx element unknown */
	u32			is_rx_badchan;		/* rx frame w/ invalid chan */
	u32			is_rx_chanmismatch;	/* rx frame chan mismatch */
	u32			is_rx_nodealloc;	/* rx frame dropped */
	u32			is_rx_ssidmismatch;	/* rx frame ssid mismatch  */
	u32			is_rx_auth_unsupported;	/* rx w/ unsupported auth alg */
	u32			is_rx_auth_fail;	/* rx sta auth failure */
	u32			is_rx_auth_countermeasures;/* rx auth discard 'cuz CM */
	u32			is_rx_assoc_bss;	/* rx assoc from wrong bssid */
	u32			is_rx_assoc_notauth;	/* rx assoc w/o auth */
	u32			is_rx_assoc_capmismatch;/* rx assoc w/ cap mismatch */
	u32			is_rx_assoc_norate;	/* rx assoc w/ no rate match */
	u32			is_rx_assoc_badwpaie;	/* rx assoc w/ bad WPA IE */
	u32			is_rx_bad_assoc;
	u32			is_assoc_success;
	u32		 is_rx_reassoc;
	u32		 is_rx_reassoc_success;
	u32		 is_rx_bad_reassoc;
	u32		 is_rx_reassoc_bss;	
	u32		 is_rx_reassoc_notauth;	
	u32		 is_rx_reassoc_capmismatch;
	u32		 is_rx_reassoc_norate;	
	u32		 is_rx_reassoc_badwpaie;	
	u32		 is_rx_reassoc_badscie;
	
	u32		 is_disassoc_unnormal;
	u32		 is_disassoc_state;
	u32		 is_disassoc_ioctl;

    /*Begin:added  by zhanghu to statistics the number that the ap send disassoc frame to the station being on line.  2012-10-31*/
    u32		 is_tx_auth_assoc_fail;
    /*End:added  by zhanghu to statistics the number that the ap send disassoc frame to the station being on line.  2012-10-31*/

	u32		 is_deauth_ioctl;
	u32		 is_deauth_unnormal;
	u32		 is_deauth_state;
	u32		 is_deauth_expire;
	u32			is_rx_deauth;		/* rx deauthentication */
	u32			is_rx_disassoc;		/* rx disassociation */
	u32		   is_rx_action;       /* rx action mgt */
	u32			is_rx_badsubtype;	/* rx frame w/ unknown subtype*/
	u32			is_rx_nobuf;		/* rx failed for lack of buf */
	u32			is_rx_ahdemo_mgt;	/* rx discard ahdemo mgt frame*/
	u32			is_rx_bad_auth;		/* rx bad auth request */
	u32			is_rx_unauth;		/* rx on unauthorized port */
	u32			is_rx_badcipher;	/* rx failed 'cuz key type */
    u32		   is_tx_nodefkey;            /* tx failed 'cuz no defkey */
    u32		   is_tx_noheadroom;          /* tx failed 'cuz no space */
	u32			is_rx_nocipherctx;	/* rx failed 'cuz key !setup */
	u32			is_rx_acl;		/* rx discard 'cuz acl policy */
	u32			is_rx_ffcnt;		/* rx fast frames */
	u32			is_rx_badathtnl;   	/* driver key alloc failed */
    u32		   is_rx_nowds;               /* 4-addr packets received with no wds enabled */
	u32			is_tx_nobuf;		/* tx failed for lack of buf */
	u32			is_tx_nonode;		/* tx failed for no node */
	u32			is_tx_unknownmgt;	/* tx of unknown mgt frame */
	u32			is_tx_badcipher;	/* tx failed 'cuz key type */
	u32			is_tx_ffokcnt;		/* tx fast frames sent success */
	u32			is_tx_fferrcnt;		/* tx fast frames sent success */
	u32			is_scan_active;		/* active scans started */
	u32			is_scan_passive;	/* passive scans started */
	u32			is_node_timeout;	/* nodes timed out inactivity */
	u32		 is_node_timeout_auth;		/*lyb add nodes timed out inactivity at auth state*/
	u32		 is_node_timeout_assoc;	/* lyb add nodes timed out inactivity at assoc state */
	u32			is_crypto_nomem;	/* no memory for crypto ctx */
	u32			is_crypto_tkip;		/* tkip crypto done in s/w */
	u32			is_crypto_tkipenmic;	/* tkip en-MIC done in s/w */
	u32			is_crypto_tkipdemic;	/* tkip de-MIC done in s/w */
	u32			is_crypto_tkipcm;	/* tkip counter measures */
	u32			is_crypto_ccmp;		/* ccmp crypto done in s/w */
	u32			is_crypto_wep;		/* wep crypto done in s/w */
	u32			is_crypto_setkey_cipher;/* cipher rejected key */
	u32			is_crypto_setkey_nokey;	/* no key index for setkey */
	u32			is_crypto_delkey;	/* driver key delete failed */
	u32			is_crypto_badcipher;	/* unknown cipher */
	u32			is_crypto_nocipher;	/* cipher not available */
	u32			is_crypto_attachfail;	/* cipher attach failed */
	u32			is_crypto_swfallback;	/* cipher fallback to s/w */
	u32			is_crypto_keyfail;	/* driver key alloc failed */
	u32			is_crypto_enmicfail;	/* en-MIC failed */
	u32			is_ibss_capmismatch;	/* merge failed-cap mismatch */
	u32			is_ibss_norate;		/* merge failed-rate mismatch */
	u32			is_ps_unassoc;		/* ps-poll for unassoc. sta */
	u32			is_ps_badaid;		/* ps-poll w/ incorrect aid */
	u32			is_ps_qempty;		/* ps-poll w/ nothing to send */
	u32		 is_rx_mgmt;
	u32		 is_tx_mgmt;
	u64			is_rx_mgmtbytes;
	u64			is_tx_mgmtbytes;        
	u32		 is_assocs;
	u32		 is_reassocs;
	u32		 is_rx_repack;		/*终端重传的包数*/
	u32		 is_rx_rebytes;	/*终端重传的字节数*/
	u32		 is_rx_err_pack;				/*接收到的来自终端的错误包数*/
	u32		 is_tx_signaling;				/*发送到指定终端的信令包数*/
	u32		 is_rx_signaling;				/*终端传送的信令包数*/
	u32		 is_re_wpi;/*根据重放机制丢弃的WPI MPDU的数目*/
	u32		 is_wpi_mic;  /* WPI-SMS4解密时没有有效的密钥而丢弃的MPDU数目*/
	u32		 is_wpi_no_key_error;/* WPI-SMS4解密时MIC校验出错而丢弃的MPDU数目*/
	u32			is_rx_auth_try;	/*add wyh@20080716 for snmp*/
	u32			is_rx_auth_ok;	/*add wyh@20080716 for snmp*/
	u32			is_rx_assoc_try;	/*add wyh@20080716 for snmp*/
	u32			is_rx_ap_too_many_sta;	/*add wyh@20080716 for snmp*/
	u32		 is_rx_errors;
	u32		 is_rx_error_frames;
	u32		 is_refuse_LowRssi;
};
#endif

struct ieee80211_tunnel_base_users{
 
 u8 ni_macaddr[IEEE80211_ADDR_LEN];
 u8 ni_tunnel_flag;
 u16 vlan_id;
 u32 sta_sid;
};


typedef struct
{  //peiwenhui add 090205
	unsigned char type;     //0-ath, 1-eth, 2-wifi
	unsigned char radioId;
	unsigned char wlanId;
	unsigned char mac[6];
	unsigned int rx_packets;
	unsigned int tx_packets;
	unsigned int rx_errors;  //new
	unsigned int tx_errors;  //new
	unsigned long long rx_bytes;
	unsigned long long tx_bytes;
	unsigned int rx_rate;
	unsigned int tx_rate;
	unsigned int ast_rx_crcerr;   //pei add 0220
	unsigned int ast_rx_badcrypt;
	unsigned int ast_rx_badmic;
	unsigned int ast_rx_phyerr;
	unsigned int rx_dropped;
	unsigned int tx_dropped;
	unsigned int rx_multicast;
	unsigned int tx_multicast;
	unsigned int rx_broadcast;
	unsigned int tx_broadcast;
	unsigned int rx_unicast;
	unsigned int tx_unicast;
	unsigned int rx_multicast_bytes;
	unsigned int tx_multicast_bytes;
	unsigned int rx_broadcast_bytes;
	unsigned int tx_broadcast_bytes;
	unsigned int rx_unicast_bytes;
	unsigned int tx_unicast_bytes;
	unsigned int rx_retry_bytes;
	unsigned int tx_retry_bytes;
	unsigned int rx_retry;
	unsigned int tx_retry;	
	unsigned int rx_mgmt;	
	unsigned int tx_mgmt;
	unsigned long long rx_mgmt_bytes;
	unsigned long long tx_mgmt_bytes;
	unsigned long long rx_sum_bytes;
	unsigned long long tx_sum_bytes;
	unsigned long long rx_unicast64_bytes;
	unsigned long long tx_unicast64_bytes;
	unsigned int rx_ctrl;
	unsigned int tx_ctrl;
	unsigned int rx_errors_frames;  //new
}wlan_stats_info;

struct user_net_device_stats
{
	unsigned long long rx_packets;	/* total packets received       */
	unsigned long long tx_packets;	/* total packets transmitted    */
	unsigned long long rx_bytes;	/* total bytes received         */
	unsigned long long tx_bytes;	/* total bytes transmitted      */
	unsigned long rx_errors;	/* bad packets received         */
	unsigned long tx_errors;	/* packet transmit problems     */
	unsigned long rx_dropped;	/* no space in linux buffers    */
	unsigned long tx_dropped;	/* no space available in linux  */
	unsigned long rx_multicast;	/* multicast packets received   */
	unsigned long rx_compressed;
	unsigned long tx_compressed;
	unsigned long collisions;
	unsigned long rx_broadcast;  /* pei add 090624 */
	unsigned long rx_unicast;
	unsigned long tx_multicast;
	unsigned long tx_broadcast;
	unsigned long tx_unicast;
	
	/* detailed rx_errors: */
	unsigned long rx_length_errors;
	unsigned long rx_over_errors;	/* receiver ring buff overflow  */
	unsigned long rx_crc_errors;	/* recved pkt with crc error    */
	unsigned long rx_frame_errors;	/* recv'd frame alignment error */
	unsigned long rx_fifo_errors;	/* recv'r fifo overrun          */
	unsigned long rx_missed_errors;	/* receiver missed packet     */
	/* detailed tx_errors */
	unsigned long tx_aborted_errors;
	unsigned long tx_carrier_errors;
	unsigned long tx_fifo_errors;
	unsigned long tx_heartbeat_errors;
	unsigned long tx_window_errors;
	unsigned long rx_multicast_bytes;
	unsigned long tx_multicast_bytes;
	unsigned long rx_broadcast_bytes;
	unsigned long tx_broadcast_bytes;
	unsigned long long rx_unicast_bytes;
	unsigned long long tx_unicast_bytes;
	unsigned long rx_retry_bytes;
	unsigned long tx_retry_bytes;
	unsigned long rx_retry;
	unsigned long tx_retry;	
	unsigned long rx_error_frames;		/* bad frames received		*/
};

struct ieee80211_nodestats
{
	u32	ns_rx_data;		/* rx data frames */
	u32	ns_rx_mgmt;		/* rx management frames */
	u32	ns_rx_ctrl;		/* rx control frames */
	u32	ns_rx_ucast;		/* rx unicast frames */
	u32	ns_rx_mcast;		/* rx multi/broadcast frames */
	u64	ns_rx_bytes;		/* rx data count (bytes) */
	u64	ns_rx_beacons;		/* rx beacon frames */
	u32	ns_rx_proberesp;	/* rx probe response frames */

	u32	ns_rx_dup;		/* rx discard 'cuz dup */
	u32	ns_rx_noprivacy;	/* rx w/ wep but privacy off */
	u32	ns_rx_wepfail;		/* rx wep processing failed */
	u32	ns_rx_demicfail;	/* rx demic failed */
	u32	ns_rx_decap;		/* rx decapsulation failed */
	u32	ns_rx_defrag;		/* rx defragmentation failed */
	u32	ns_rx_disassoc;		/* rx disassociation */
	u32	ns_rx_deauth;		/* rx deauthentication */
	u32   ns_rx_action;       /* rx action */
	u32	ns_rx_decryptcrc;	/* rx decrypt failed on crc */
	u32	ns_rx_unauth;		/* rx on unauthorized port */
	u32	ns_rx_unencrypted;	/* rx unecrypted w/ privacy */

	u32	ns_tx_data;		/* tx data frames */
	u32	ns_tx_mgmt;		/* tx management frames */
	u32	ns_tx_ucast;		/* tx unicast frames */
	u32	ns_tx_mcast;		/* tx multi/broadcast frames */
	u64	ns_tx_bytes;		/* tx data count (bytes) */
	u32	ns_tx_probereq;		/* tx probe request frames */
	u32	ns_tx_uapsd;		/* tx on uapsd queue */

	u32	ns_tx_novlantag;	/* tx discard 'cuz no tag */
	u32	ns_tx_vlanmismatch;	/* tx discard 'cuz bad tag */

	u32	ns_tx_eosplost;		/* uapsd EOSP retried out */

	u32	ns_ps_discard;		/* ps discard 'cuz of age */

	u32	ns_uapsd_triggers;	     /* uapsd triggers */
	u32	ns_uapsd_duptriggers;	 /* uapsd duplicate triggers */
	u32	ns_uapsd_ignoretriggers; /* uapsd duplicate triggers */
	u32	ns_uapsd_active;         /* uapsd duplicate triggers */
	u32	ns_uapsd_triggerenabled; /* uapsd duplicate triggers */

	/* MIB-related state */
	u32	ns_tx_assoc;		/* [re]associations */
	u32	ns_tx_assoc_fail;	/* [re]association failures */
	u32	ns_tx_auth;		/* [re]authentications */
	u32	ns_tx_auth_fail;	/* [re]authentication failures*/
	u32	ns_tx_deauth;		/* deauthentications */
	u32	ns_tx_deauth_code;	/* last deauth reason */
	u32	ns_tx_disassoc;		/* disassociations */
	u32	ns_tx_disassoc_code;	/* last disassociation reason */
	u32	ns_psq_drops;		/* power save queue drops */
	u32	ns_tx_dropblock;	/* tx discard 'cuz headline block */
	u32 ns_tx_ctrl;                /* tx control frames */
	/*zhaoyang modify for wapi error frame count*/
	u32 ns_re_wpi;/*根据重放机制丢弃的WPI MPDU的数目*/
	u32 ns_wpi_mic;  /* WPI-SMS4解密时没有有效的密钥而丢弃的MPDU数目*/
	u32 ns_wpi_no_key_error;/* WPI-SMS4解密时MIC校验出错而丢弃的MPDU数目*/
	/*zhaoyang modify end*/
	u32 ns_tx_retry_packets;
	u32 ns_tx_retry_bytes;
	u32 ns_rx_retry_packets;
	u32 ns_rx_retry_bytes;
	u64	ns_rx_bytes_l;            /*  rx data count (bytes) of last time */
	u64	ns_sum_bytes;            /*  rx and tx data sum (bytes) of last time */
	
	u32 tx_frag_packets;
	u32 rx_frag_packets;
	unsigned long  ns_time_remain;      /*  rx data count (bytes) of last time */ 
};

typedef struct {
	unsigned char value;
	unsigned char count;
	unsigned char is_flood_attack;
	unsigned char flood_detect_type;  // 0
	unsigned char flood_sta_mac[6];
	unsigned char flood_vap_mac[6];
	unsigned char flood_frame_type;
	unsigned char flood_channel;
	unsigned char flood_rssi;
	unsigned char is_spoof_attack;
	unsigned char spoof_detect_type;  // 1
	unsigned char spoof_sta_mac[6];
	unsigned char spoof_vap_mac[6];
	unsigned char spoof_frame_type;
	unsigned char spoof_channel;
	unsigned char spoof_rssi;
	unsigned char is_weakiv_attack;
	unsigned char weakiv_detect_type;  // 2
	unsigned char weakiv_sta_mac[6];
	unsigned char weakiv_vap_mac[6];
	unsigned char weakiv_payload;
	unsigned char weakiv_channel;
	unsigned char weakiv_rssi;
} attackDetectInfo;


struct ieee80211req_sta_stats
{
	union
	{
		/* NB: explicitly force 64-bit alignment */
		u8 macaddr[IEEE80211_ADDR_LEN];
		u64 pad;
	} is_u;
	struct ieee80211_nodestats is_stats;
};

typedef __uint32_t __u32;
typedef __int32_t __s32;
typedef __uint16_t __u16;
typedef __int16_t __s16;
typedef __uint8_t __u8;

#ifndef __user
#define __user
#endif /* __user */

/*
 *	For all data larger than 16 octets, we need to use a
 *	pointer to memory allocated in user space.
 */
struct	iw_point
{
  void __user	*pointer;	/* Pointer to the data	(in user space) */
  __u16 	length; 	/* number of fields or size in bytes */
  __u16 	flags;		/* Optional params */
};

/* --------------------------- SUBTYPES --------------------------- */
/*
 *	Generic format for most parameters that fit in an int
 */
struct	iw_param
{
  __s32 	value;		/* The value of the parameter itself */
  __u8		fixed;		/* Hardware should not use auto select */
  __u8		disabled;	/* Disable the feature */
  __u16 	flags;		/* Various specifc flags (if any) */
};


/*
 *	A frequency
 *	For numbers lower than 10^9, we encode the number in 'm' and
 *	set 'e' to 0
 *	For number greater than 10^9, we divide it by the lowest power
 *	of 10 to get 'm' lower than 10^9, with 'm'= f / (10^'e')...
 *	The power of 10 is in 'e', the result of the division is in 'm'.
 */
struct	iw_freq
{
	__s32		m;		/* Mantissa */
	__s16		e;		/* Exponent */
	__u8		i;		/* List index (when in range struct) */
	__u8		flags;		/* Flags (fixed/auto) */
};

/*
 *	Quality of the link
 */
struct	iw_quality
{
	__u8		qual;		/* link quality (%retries, SNR,
					   %missed beacons or better...) */
	__u8		level;		/* signal level (dBm) */
	__u8		noise;		/* noise level (dBm) */
	__u8		updated;	/* Flags to know if updated */
};


union	iwreq_data
{
	/* Config - generic */
	char		name[16];
	/* Name : used to verify the presence of  wireless extensions.
	 * Name of the protocol/provider... */

	struct iw_point	essid;		/* Extended network name */
	struct iw_param	nwid;		/* network id (or domain - the cell) */
	struct iw_freq	freq;		/* frequency or channel :
					 * 0-1000 = channel
					 * > 1000 = frequency in Hz */

	struct iw_param	sens;		/* signal level threshold */
	struct iw_param	bitrate;	/* default bit rate */
	struct iw_param	txpower;	/* default transmit power */
	struct iw_param	rts;		/* RTS threshold threshold */
	struct iw_param	frag;		/* Fragmentation threshold */
	u32		mode;		/* Operation mode */
	struct iw_param	retry;		/* Retry limits & lifetime */

	struct iw_point	encoding;	/* Encoding stuff : tokens */
	struct iw_param	power;		/* PM duration/timeout */
	struct iw_quality qual;		/* Quality part of statistics */

	struct sockaddr	ap_addr;	/* Access point address */
	struct sockaddr	addr;		/* Destination address (hw/mac) */

	struct iw_param	param;		/* Other small parameters */
	struct iw_point	data;		/* Other large parameters */
};

struct iw_event
{
	u16		len;			/* Real length of this stuff */
	u16		cmd;			/* Wireless IOCTL */
	union iwreq_data	u;		/* IOCTL fixed payload */
};

struct	iwreq 
{
	union
	{
		char	ifrn_name[IFNAMSIZ];	/* if name, e.g. "eth0" */
	} ifr_ifrn;

	/* Data part (defined just above) */
	union	iwreq_data	u;
};

struct ieee80211req_sta_info {
	u16		isi_len;		/* length (mult of 4) */
	u16		isi_freq;		/* MHz */
	u32		isi_flags;      /* channel flags */
	u16		isi_state;		/* state flags */
	u8		isi_authmode;		/* authentication algorithm */
	char	isi_rssi;
	u16		isi_capinfo;		/* capabilities */
	u8		isi_athflags;		/* Atheros capabilities */
	u8		isi_erp;		/* ERP element */
	u8		isi_macaddr[IEEE80211_ADDR_LEN];
	u8		isi_nrates;
						/* negotiated rates */
	u8		isi_rates[IEEE80211_RATE_MAXSIZE];
	u8		isi_txrate;		/* index to isi_rates[] */
	u32		isi_txratekbps;         /* tx rate in kbps */
	u32		isi_rxratekbps;         /* rx rate in kbps */
	u16		isi_ie_len;		/* IE length */
	u16		isi_associd;		/* assoc response */
	u16		isi_txpower;		/* current tx power */
	u16		isi_vlan;		/* vlan tag */
	u16		isi_txseqs[17];		/* seq to be transmitted */
	u16		isi_rxseqs[17];		/* seq previous for qos frames*/
	u16		isi_inact;		/* inactivity timer */
	u8		isi_uapsd;		/* UAPSD queues */
	u8		isi_opmode;		/* sta operating mode */
	u8		isi_cipher;
	u32		isi_assoc_time;	/* sta association time */

	u16		isi_htcap;		/* HT capabilities */
	/* XXX frag state? */
	/* variable length IE data */
};
/**************************************************************************************************/
typedef enum
{
	OPEN_SYSTEM		= 0,
	SHARED_KEY		= 1
}authorize_type_802_11;

typedef enum
{
	SAFE_NONE		= 1,
	SAFE_WPA		= 2,
	SAFE_WPA2		= 3,
	SAFE_WAPI		= 4
}safe_type;

typedef enum
{
	AUTHORIZE_NONE		= 0,
	AUTHORIZE_PSK		= 1,
	AUTHORIZE_EAP		= 2,
	AUTHORIZE_WAPI_CER	= 3
}authorize_mode;

typedef enum
{
	ENCRYPTION_NONE		= 0,
	ENCRYPTION_WEP_40	= 1,
	ENCRYPTION_WEP_104	= 2,
	ENCRYPTION_TKIP		= 3,
	ENCRYPTION_AES_CCMP	= 4,
	ENCRYPTION_WPI_SMS4	= 5
}encryption_type;




typedef enum
{
	WAPI_OPEN		= 0,
	WAPI_PSK		= 1,
	WAPI_CER		= 2
}SECURITY_POLICY_WAPI;

typedef enum
{
	OPEN          = 0,
	WEP           = 1,
	EAP_WPA_WPA2  = 2,
	PSK_WPA_WPA2  = 3,
	Certificate	  = 4,
	/* no use below */
	PSK_WPA       = 5,
	PSK_WPA2      = 6,
	EAP_WPA       = 7,
	EAP_WPA2      = 8
}SECURITY_POLICY;
typedef enum
{
	SECURITY_WAPI			= 1,
	SECURITY_802_11i        = 2
}SECURITY;
typedef enum
{
	LOCAL_MAC		= 0,
	SPLIT_MAC		= 1
}MAC_MODE;
typedef enum
{
	LOCAL_BRIDGE		= 0,
	TUNNEL_802_3		= 1,
	TUNNEL_802_11		= 2
}TUNNEL_MODE;

typedef enum
{
	STA_OPEN			= 0,
	STA_WHITE_LIST		= 1,
	STA_BLACK_LIST		= 2
}MAC_FILTRATION_RULE;

typedef enum
{
	EAP_DEFAULT		= 0,
	MD5_Challenge	= 4,
	EAP_TLS			= 13,
	EAP_SIM			= 18,
	PEAP			= 25,
	MS_CHAP_V2		= 29
}EAP_TYPE;

/* */
typedef struct COMMAND 
{
	u32 msgtype;
	void *cmdmsg;
}TCMD;
typedef struct COMMAND_LIST
{
	TCMD cmd;
	struct COMMAND_LIST *next;
}CMD_LST;
typedef struct MAC_LIST
{
	u8   ret;
	u8   *mac;
}MAC_LST;
typedef struct BLACK_LIST
{
	u8   num;
	MAC_LST *maclst;
}BLACK_LST;

typedef struct UPGRADE_FIREWARE
{
	u16      type;
	u16      len;
	u8       transproto;
	u32      serverip;
	u8       *ver;	
	u8       *url;
	u8       *username;
	u8       *passwd;	
}UPGRADE_ARG;

typedef enum
{
	UPGRADE_SUCCESSED           = 0,
	UPGRADE_MATCH_VERSION       = 1,
	UPGRADE_INVALIDED_VER       = 2,
	UPGRADE_TIMEOUT             = 3,
	UPGRADE_FTP_SUCCESSED       = 4,
	UPGRADE_FAILED              = 5
}UPGRADE_RET;

typedef struct STA_INFOMATION
{
	u8		rid;
	u8		wid;
	u8		*MAC;
	u8		*BSSID;
	char	*SSID;
	u8		encryption_type;
	u32		time_stamp;
	u8		ssid_length;
	u16		type;
	u16		length;
	u32		value;
	u32		ip;
	u8    	*PMK;
	struct STA_INFOMATION *next;
}STA_INFO;
extern STA_INFO *sta_list;


typedef struct NTP_SETTING
{
	u16      ver;
	u8       seq;
	u8       ret;
	u32      serverip;
	u16      syninterval;
}NTP_SET;

/* Begin: Add by dongzw for station isolation 2012-11-21 */
typedef struct ISOLATION_SETTING
{
	u16		   ver;
	u8		   seq;
	u16		   type;
	u16		   length;
	u8         value;
	u8		   ret;
} ISOLATION_SET;
/* End: Add by dongzw for station isolation 2012-11-21 */

typedef struct AC_SET_STA_STATE
{
	u16  ver;
	u8   seq;
	u8   ret;
	u16  type;
	u8   rid;
	u8   wid;
	u8   mac[MAC_ADDR_LEN];
	u16  state;
	u16  code;
	u16  value;
	u8   act;
	u32  sid;
	u16	 PMKtype;
	u16  PMKlen;
	u8   *PMK;
}SET_STA_STATE;
typedef struct 
{
	u8 mac[MAC_ADDR_LEN];
	u16 on_off_line;
}STA_ON_OFF_LINE;
/* for radius */
typedef struct RADIUS_CONFIG 
{
	u8   type;
	u8   pri;
	u32  ipaddr;
	u16  port;
	u16  pwd_len;
	char *pwd;
	struct RADIUS_CONFIG *next;
}RADIUS_CONF;
typedef struct MESSAE2
{
	u16  ver;
	u8   seq;
	u8   ret;
	void *element;
}MSG2;

extern u32           ntpserverip;
extern u16           ntpinterval;



typedef struct
{
	unsigned char radioId;
	unsigned char channel;
	unsigned char txtype;
	unsigned char auto_txpower;
	int txpower;
	unsigned short rate;
	BOOL rateAutoEnable;
	unsigned int radiotype;
	unsigned short fragThreshold;
	unsigned short rtsThreshold;
	unsigned char shortRetry;
	unsigned char longRetry;
	unsigned short beaconInterval;
	unsigned char preamble;
	unsigned char dtim;
	BOOL gIsRadioEnable;
	BOOL radioState;
	unsigned long wifi_start_time;
	unsigned long wifi_current_time;
	unsigned char channel_width;	/* 0:standard_20 1:auto_20/40 2:down_40 3:up_40 */
	/* */
	u32 rate_mask;
	u16 MCS;
	u8  chianmask;
	u8  AMPDU;
	u8  shortGI;
	u8  HTMixed;
	u8  AMSDU;
	
} radioInfoValues;
typedef struct 
{
	u8  flag;
	int count;
	u8  *f;
	u16 *rate;
}RATE_SET_T;

typedef struct {
	unsigned char type;// 1--dhcpsnooping_report;2--ntp_report
	unsigned char subtype;
	unsigned char mac[6];
	unsigned char radio_id;
	unsigned char wlan_id;
	unsigned int vlanid ;
	unsigned char ip_length;
	unsigned int ip ;
}dhcpsnooping_report_type;

typedef struct mac_address_filtration
{
	u8 mac[6];
	struct mac_address_filtration *next;
}macAddrFiltration;

extern macAddrFiltration *StaBlackList;
extern radioInfoValues gRadioInfoValue[2];
extern RADIO_INFO *hri, *tri;
extern RATE_SET_T rs;
extern BOOL local_tunnel;
extern RADIUS_CONF *radius_list;
extern INTERVAL_COUNT_MONITOR_INFO *interval_count_monitor_info;
extern int overtime_count;
extern BOOL	traffic_vap_up_switch;
extern BOOL traffic_vap_down_switch;
extern BOOL	traffic_node_up_switch;
extern BOOL traffic_node_down_switch;


extern BOOL ResetTimers();

extern BOOL get_ap_ability_info(AP_ABILITY_INFO **ability_info);
extern BOOL get_ap_state_count_info(AP_STATE_COUNT_INFO **state_count_info);
extern BOOL get_wireless_ability_info(WIRELESS_ABILITY_INFO **ability_info);
extern BOOL get_wireless_state_count_info(WIRELESS_STATE_COUNT_INFO **state_count_info);
extern BOOL get_sta_state_count_info(STA_STATE_COUNT_INFO **state_count_info);
extern BOOL get_ap_eth_ability_info(AP_ETH_ABILITY_INFO **ability_info);
extern BOOL get_ap_eth_state_count_info(AP_ETH_STATE_COUNT_INFO **state_count_info);
extern BOOL get_ap_ath_state_count_info(AP_ATH_STATE_COUNT_INFO **state_count_info);

extern BOOL assemble_report_ap_ability_request(void *arg, TMSG *msg);
extern BOOL assemble_report_ap_state_count_request(void *arg, TMSG *msg);
extern BOOL assemble_report_wireless_ability_request(void *arg, TMSG *msg);
extern BOOL assemble_report_wireless_state_count_request(void *arg, TMSG *msg);
extern BOOL assemble_report_sta_count_request(void *arg, TMSG *msg);
extern BOOL assemble_report_ap_eth_ability_request(void *arg, TMSG *msg);
extern BOOL assemble_report_ap_eth_state_count_request(void *arg, TMSG *msg);
extern BOOL assemble_report_ap_ath_state_count_request(void *arg, TMSG *msg);

extern BOOL parse_messages_header(TMSG *msg, MSG_HEADER *msgh);
extern BOOL parse_add_wlan_requset_msg(TMSG *msg);
extern BOOL manage_add_wlan_request_msg(WLAN_INFO *wlanconf);
extern BOOL parse_del_wlan_requset_msg(TMSG *msg);
extern BOOL manage_del_wlan_request_msg(WLAN_INFO *wlan);
extern BOOL parse_add_sta_to_blacklist_request_msg(TMSG *msg);
extern BOOL manage_add_sta_to_blacklist_request_msg(BLACK_LST *bl);
extern BOOL parse_del_sta_to_blacklist_request_msg(TMSG *msg);
extern BOOL manage_del_sta_to_blacklist_request_msg(BLACK_LST *bl);
extern BOOL parse_upgrade_request_msg(TMSG *msg);
extern BOOL manage_upgrade_request_msg(UPGRADE_ARG *uparg);
extern BOOL parse_set_ntp_request_msg(TMSG *msg);
extern BOOL manage_set_ntp_request_msg(NTP_SET *ntp);
/* Begin: Add by dongzw for station isolation 2012-12-21 */
extern BOOL parse_set_isolation_request_msg(TMSG *msg);
extern BOOL manage_set_isolation_request_msg(ISOLATION_SET *isolation);
/* Begin: Add by dongzw for station isolation 2012-12-21 */
extern BOOL parse_sys_maintenance_requset_msg(TMSG *msg);
extern BOOL manage_sys_maintenance_requset_msg(MAINTENANCE_OP *maintenance);
extern BOOL parse_set_radio_requset_msg(TMSG *msg);
extern BOOL manage_set_radio_requset_msg(SET_RADIO_PARAMETER *radio);
extern BOOL parse_set_sta_state_request_msg(TMSG *msg);
extern BOOL manage_set_sta_state_request_msg(SET_STA_STATE *sta);
extern BOOL parse_set_data_tunnel_requset_msg(TMSG *msg);
extern BOOL manage_set_data_tunnel_requset_msg(DATA_T *data);
extern BOOL parse_set_wlan_info_requset_msg(TMSG *msg);
extern BOOL manage_set_wlan_info_request_msg(RENEW_WLAN_CONF *wlanconf);
extern BOOL parse_notice_discovery_ac_result_requset_msg(TMSG *msg);
extern BOOL manage_notice_discovery_ac_result_requset_msg(AC_RES *result);
extern BOOL parse_notice_join_ac_result_requset_msg1(TMSG *msg);
extern BOOL manage_notice_join_ac_result_requset_msg1(AC_RES *result);
extern BOOL parse_notice_join_ac_result_requset_msg(TMSG *msg);
extern BOOL manage_notice_join_ac_result_requset_msg(AC_RES *result);
extern BOOL set_radio_info_no_wlan(u8 rid);
extern BOOL parse_set_radius_requset_msg(TMSG *msg);
extern BOOL manage_set_radius_resquest_msg(MSG2 *radius_msg);
extern BOOL start_heartbeat_timer();
extern void *check_sta_on_off_line(void *arg);
extern int set_dhcp_snooping(unsigned char state);//char *if_name
extern void *CheckMsgFromOtherPro(void *arg);
extern void *interval_count_monitor(void *arg);
extern BOOL parse_echo_response_msg(TMSG *msg);
extern BOOL manage_echo_response_msg(MSG2 *msg2);
extern BOOL send_msg_to_hostapd(char *mac, u16 PMK_Len, char *PMK, u32 port);
extern unsigned int ip_int2str(unsigned int ipAddress, unsigned char *buff);
extern BOOL set_sta_mac_addr_filtration(u8 rid, u8 wid);
extern void delete_vlan_from_bridge(int rid,int wid);
extern int macAddrCmp (unsigned char* addr1, unsigned char* addr2);
extern BOOL GetInterfaceStatus(char *ifname, unsigned char *status);
extern void * GetApScanningInfo(void *arg);
#endif
