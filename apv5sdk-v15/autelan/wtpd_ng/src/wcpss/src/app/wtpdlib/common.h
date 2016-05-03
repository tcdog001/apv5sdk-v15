#ifndef _COMMON_H_
#define _COMMON_H_


#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>


typedef __uint32_t __u32;
typedef __int32_t __s32;
typedef __uint16_t __u16;
typedef __int16_t __s16;
typedef __uint8_t __u8;

#ifndef __user
#define __user
#endif /* __user */

typedef enum {
	CW_FALSE = 0,
	CW_TRUE = 1
} CWBool;

typedef enum{
	CW_DISABLE = 0,
	CW_ENABLE = 1
}CWStatus;
#ifndef IFNAMSIZ
#define IFNAMSIZ 16/*I add*/
#endif

#define IEEE80211_PARAM_MODE_11b	0x01	/* 802.11b */
#define IEEE80211_PARAM_MODE_11a	0x02	/* 802.11a */
#define IEEE80211_PARAM_MODE_11g	0x04	/* 802.11g */
#define IEEE80211_PARAM_MODE_11n	0x08	/* 802.11n */
#define IEEE80211_PARAM_MODE_11an	0x10	/*802.11an*/
#define IEEE80211_PARAM_MODE_11gn	0x20	/*802.11gn*/


#define MAC_ADDR_LEN		6

#define CWErrorRaise(code, msg) 		_CWErrorRaise(code, msg, __FILE__, __LINE__)
#define CWErr(arg)				((arg) || _CWErrorHandleLast(__FILE__, __LINE__))
typedef int CWSocket;
typedef struct sockaddr_storage CWNetworkLev4Address;

#define		CW_FREE_OBJECT(obj_name)				{if(obj_name){free((obj_name)); (obj_name) = NULL;}}
#define		CW_FREE_OBJECTS_ARRAY(ar_name, ar_size)			{int _i = 0; for(_i = ((ar_size)-1); _i >= 0; _i--) {if(((ar_name)[_i]) != NULL){ free((ar_name)[_i]);}} free(ar_name); (ar_name) = NULL; }
#define		CW_PRINT_STRING_ARRAY(ar_name, ar_size)			{int i = 0; for(i = 0; i < (ar_size); i++) printf("[%d]: **%s**\n", i, ar_name[i]);}

// custom error
#define		CW_CREATE_OBJECT_ERR(obj_name, obj_type, on_err)	{obj_name = (obj_type*) (malloc(sizeof(obj_type))); if(!(obj_name)) {on_err}}
#define		CW_CREATE_OBJECT_SIZE_ERR(obj_name, obj_size,on_err)	{obj_name = (malloc(obj_size)); if(!(obj_name)) {on_err}}
#define		CW_CREATE_ARRAY_ERR(ar_name, ar_size, ar_type, on_err)	{ar_name = (ar_type*) (malloc(sizeof(ar_type) * (ar_size))); if(!(ar_name)) {on_err}}
#define		CW_CREATE_STRING_ERR(str_name, str_length, on_err)	{str_name = (char*) (malloc(sizeof(char) * ((str_length)+1) ) ); if(!(str_name)) {on_err}}
#define		CW_CREATE_STRING_FROM_STRING_ERR(str_name, str, on_err)	{CW_CREATE_STRING_ERR(str_name, strlen(str), on_err); strcpy((str_name), str);}

#ifdef CW_DEBUGGING
	#define		CW_CREATE_ARRAY_ERR2(ar_name, ar_size, ar_type, on_err)		{ar_name = (ar_type*) (malloc(sizeof(ar_type) * (ar_size))); if((ar_name)) {on_err}}
	#define		CW_CREATE_OBJECT_ERR2(obj_name, obj_type, on_err)		{obj_name = (obj_type*) (malloc(sizeof(obj_type))); if((obj_name)) {on_err}}
	#define		CW_CREATE_OBJECT_SIZE_ERR2(obj_name, obj_size,on_err)		{obj_name = (malloc(obj_size)); if((obj_name)) {on_err}}
	#define		CW_CREATE_STRING_ERR2(str_name, str_length, on_err)		{str_name = (char*) (malloc(sizeof(char) * ((str_length)+1) ) ); if((str_name)) {on_err}}
	#define		CW_CREATE_STRING_FROM_STRING_ERR2(str_name, str, on_err)	{CW_CREATE_STRING_ERR2(str_name, strlen(str), on_err); strcpy((str_name), str);}
#endif

#define		CW_BUFFER_SIZE					65536
#define		CW_ZERO_MEMORY					bzero
#define		CW_COPY_MEMORY(dst, src, len)			bcopy(src, dst, len)
#define		CW_REPEAT_FOREVER				while(1)

#define		CWParseMessageElementStart()				int oldOffset;												\
									if(msgPtr == NULL || valPtr == NULL) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);	\
									oldOffset = msgPtr->offset
						
#define		CWParseMessageElementEnd()				CWDebugLog(NULL);											\
									return ((msgPtr->offset) - oldOffset) == len ? CW_TRUE :	\
									CWErrorRaise(CW_ERROR_INVALID_FORMAT, "Message Element Malformed");

typedef enum {
	CW_ERROR_SUCCESS = 1,
	CW_ERROR_OUT_OF_MEMORY,
	CW_ERROR_WRONG_ARG,
	CW_ERROR_INTERRUPTED,
	CW_ERROR_NEED_RESOURCE,
	CW_ERROR_COMUNICATING,
	CW_ERROR_CREATING,
	CW_ERROR_GENERAL,
	CW_ERROR_OPERATION_ABORTED,
	CW_ERROR_SENDING,
	CW_ERROR_RECEIVING,
	CW_ERROR_INVALID_FORMAT,
	CW_ERROR_TIME_EXPIRED,
	CW_ERROR_NONE
} CWErrorCode;


typedef struct {  //peiwenhui add 090205
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


/****************************** TYPES ******************************/

/* --------------------------- SUBTYPES --------------------------- */
/*
 *	Generic format for most parameters that fit in an int
 */
struct	iw_param
{
  __s32		value;		/* The value of the parameter itself */
  __u8		fixed;		/* Hardware should not use auto select */
  __u8		disabled;	/* Disable the feature */
  __u16		flags;		/* Various specifc flags (if any) */
};

/*
 *	For all data larger than 16 octets, we need to use a
 *	pointer to memory allocated in user space.
 */
struct	iw_point
{
  void __user	*pointer;	/* Pointer to the data  (in user space) */
  __u16		length;		/* number of fields or size in bytes */
  __u16		flags;		/* Optional params */
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

/* ------------------------ IOCTL REQUEST ------------------------ */
/*
 * This structure defines the payload of an ioctl, and is used 
 * below.
 *
 * Note that this structure should fit on the memory footprint
 * of iwreq (which is the same as ifreq), which mean a max size of
 * 16 octets = 128 bits. Warning, pointers might be 64 bits wide...
 * You should check this when increasing the structures defined
 * above in this file...
 */
union	iwreq_data
{
	/* Config - generic */
	char		name[IFNAMSIZ];
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
	__u32		mode;		/* Operation mode */
	struct iw_param	retry;		/* Retry limits & lifetime */

	struct iw_point	encoding;	/* Encoding stuff : tokens */
	struct iw_param	power;		/* PM duration/timeout */
	struct iw_quality qual;		/* Quality part of statistics */

	struct sockaddr	ap_addr;	/* Access point address */
	struct sockaddr	addr;		/* Destination address (hw/mac) */

	struct iw_param	param;		/* Other small parameters */
	struct iw_point	data;		/* Other large parameters */
};

/*
 * The structure to exchange data for ioctl.
 * This structure is the same as 'struct ifreq', but (re)defined for
 * convenience...
 * Do I need to remind you about structure size (32 octets) ?
 */
struct	iwreq 
{
	union
	{
		char	ifrn_name[IFNAMSIZ];	/* if name, e.g. "eth0" */
	} ifr_ifrn;

	/* Data part (defined just above) */
	union	iwreq_data	u;
};

typedef struct {      //pei add 0417
	unsigned char radioId;
	unsigned char wlanId;
	unsigned char mac[6];
	unsigned char mode;  //11b-0x01,11a-0x02,11g-0x04,11n-0x08,
	unsigned char channel;
	unsigned char rssi;
	unsigned short tx_Rate;
	unsigned short rx_Rate;
	unsigned char isPowerSave;
	unsigned char isQos;
	unsigned int  rx_bytes;
	unsigned int  tx_bytes;
	unsigned long long rx_data_bytes;
	unsigned long long tx_data_bytes;
	unsigned int rx_data_frames;
	unsigned int tx_data_frames;
	unsigned int rx_frames;
	unsigned int tx_frames;
	unsigned int rx_frag_packets;
	unsigned int tx_frag_packets;
} StationInfo;
typedef struct sta_info{
	unsigned char radio_id;
	unsigned char wlan_id;
	struct sta_info *next;
	unsigned char sta_mac[6];
	unsigned char rate;
	unsigned char authentication_type;
	unsigned short int aid; /* STA's unique AID (1 .. 2007) or 0 if not yet assigned */
	unsigned short capability;
	unsigned short listen_interval; /* or beacon_int for APs */
}CWWTPSta;

typedef struct {
	char *msg;
	int offset;
	int msgLen;
} CWProtocolMessage;

typedef struct wlan_info{
	unsigned char radio_id;
	unsigned char wlan_id;
	unsigned short vlan_id;  //pei add 0320
	unsigned char wlan_updown_time;  //pei add 0225
	unsigned char wlan_bssid[MAC_ADDR_LEN];
	struct wlan_info *next;
	unsigned short int capabilities;/*the capabilities information*/
	char essid[32];
	int wlan_key_mgmt;
	unsigned char wlan_mac_mode;
	unsigned char wlan_tunnel_mode;
	unsigned short key_length;
	char key[32];/*legth 32*/	
	int wlan_sendsock;
	int wlan_eapsock;
	int wlan_dot3sock;
}CWWTPWlan;
/* ----------------------- WIRELESS EVENTS ----------------------- */
/*
 * Wireless events are carried through the rtnetlink socket to user
 * space. They are encapsulated in the IFLA_WIRELESS field of
 * a RTM_NEWLINK message.
 */

/*
 * A Wireless Event. Contains basically the same data as the ioctl...
 */
struct iw_event
{
	__u16		len;			/* Real length of this stuff */
	__u16		cmd;			/* Wireless IOCTL */
	union iwreq_data	u;		/* IOCTL fixed payload */
};

/* Size of the Event prefix (including padding and alignement junk) */
#define IW_EV_LCP_LEN	(sizeof(struct iw_event) - sizeof(union iwreq_data))
/* Size of the various events */
#define IW_EV_CHAR_LEN	(IW_EV_LCP_LEN + IFNAMSIZ)
#define IW_EV_UINT_LEN	(IW_EV_LCP_LEN + sizeof(__u32))
#define IW_EV_FREQ_LEN	(IW_EV_LCP_LEN + sizeof(struct iw_freq))
#define IW_EV_PARAM_LEN	(IW_EV_LCP_LEN + sizeof(struct iw_param))
#define IW_EV_ADDR_LEN	(IW_EV_LCP_LEN + sizeof(struct sockaddr))
#define IW_EV_QUAL_LEN	(IW_EV_LCP_LEN + sizeof(struct iw_quality))

/* iw_point events are special. First, the payload (extra data) come at
 * the end of the event, so they are bigger than IW_EV_POINT_LEN. Second,
 * we omit the pointer, so start at an offset. */
#define IW_EV_POINT_OFF (((char *) &(((struct iw_point *) NULL)->length)) - \
			  (char *) NULL)
#define IW_EV_POINT_LEN	(IW_EV_LCP_LEN + sizeof(struct iw_point) - \
			 IW_EV_POINT_OFF)
	
enum EventRespType {
	NEIGHBOR_AP_INFO=1,
	MONITOR,
	EXTRA_INFO,
	STA_INFO,
	IF_STATE,
	ATTACK_DETECT_INFO,
	STATION_WAPI_INFO,
	ALL_IF_INFO,
	NTP_REPORT,
	ATTACK_ADDR_RD,
	CHALLENGE_REPLAY,
	TERMINAL_DISTURB,
	STATHROUGHPUT_OPTYPE = 13,
	EVENT_T_STA_PACKET_STATISTIC_REPORT = 14,
};

typedef struct {
	unsigned char static_IP_enable;
	unsigned int IP_addr;
	unsigned int netmask;
	unsigned int gateway;
	unsigned int firstdns;
	unsigned int secdns;
} WTPStaticIPValues;
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


#define is_space(c) ((((c) == ' ') || (((unsigned int)((c) - 9)) <= (13 - 9))))
#define	IEEE80211_RATE_MAXSIZE	36

unsigned char CWProtocolRetrieve8(CWProtocolMessage *msgPtr);
char *CWProtocolRetrieveRawBytes(CWProtocolMessage *msgPtr, int len);
CWBool _system(char *cmdbuf);
int CWGetWlanIdxInMap(int * map, int radio_id, int wlan_id);
int CWGetLocalWlanID(int radio_id, int remote_wlan_id);
int CWGetRemoteWlanID(int radio_id, int local_wlan_id);
int CWCreateLocalWlanID(int radio_id, int remote_wlan_id);
void CWCaptrue(int n ,unsigned char *buffer);
CWBool spf_cmd(const char *format,...);
void dpf(const char *format, ...);

//for mac debug print, add by diaowq@20120531
#define MAC_FMT "%02x:%02x:%02x:%02x:%02x:%02x"
#define MAC2STR(a) (a)[0], (a)[1], (a)[2], (a)[3], (a)[4], (a)[5]


#endif
