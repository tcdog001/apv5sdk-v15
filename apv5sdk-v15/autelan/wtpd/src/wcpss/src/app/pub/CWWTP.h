/*******************************************************************************************
 * Copyright (c) 2006-7 Laboratorio di Sistemi di Elaborazione e Bioingegneria Informatica *
 *                      Universita' Campus BioMedico - Italy                               *
 *                                                                                         *
 * This program is free software; you can redistribute it and/or modify it under the terms *
 * of the GNU General Public License as published by the Free Software Foundation; either  *
 * version 2 of the License, or (at your option) any later version.                        *
 *                                                                                         *
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY         *
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A 	       *
 * PARTICULAR PURPOSE. See the GNU General Public License for more details.                *
 *                                                                                         *
 * You should have received a copy of the GNU General Public License along with this       *
 * program; if not, write to the:                                                          *
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330, Boston,                    *
 * MA  02111-1307, USA.                                                                    *
 *                                                                                         *
 * --------------------------------------------------------------------------------------- *
 * Project:  Capwap                                                                        *
 *                                                                                         *
 * Author :  Ludovico Rossi (ludo@bluepixysw.com)                                          *  
 *           Del Moro Andrea (andrea_delmoro@libero.it)                                    *
 *           Giovannini Federica (giovannini.federica@gmail.com)                           *
 *           Massimo Vellucci (m.vellucci@unicampus.it)                                    *
 *           Mauro Bisson (mauro.bis@gmail.com)                                            *
 *******************************************************************************************/


#ifndef __CAPWAP_CWWTP_HEADER__
#define __CAPWAP_CWWTP_HEADER__

/*_______________________________________________________*/
/*  *******************___INCLUDE___*******************  */

#include "CWCommon.h"
#include "WTPProtocol.h"
#include "WTPBinding.h"
/*________________________________________________________________________*/
/*  *******************___DEFINE___*******************  */
#define LOG_FILE_NAME					"/jffs/wtp.log.txt"
#define WTP_LOG_FILE_NAME				"/tmp/wtpd.log"
#define _jffs_config_xml_				"/jffs/config.xml"
#define _tmp_config_xml_				"/tmp/config.xml"
#define MODEL_AQ1000		1000/*the model num of aq1000*/
#define MODEL_AQ1200		1200/*the model num of aq1200*/
#define MODEL_AQ3000		3000/*the model num of aq3000*/
//#define MAX_RADIO_NUM		4
#define MAX_RADIO_NUM		2////zengmin change MAX_RADIO_NUM 4 to 2 by Coverity 2013-06-09

#define MAX_WLAN_NUM_PER_RADIO        4
#define MAX_WLAN_NUM_PER_WTP		(MAX_RADIO_NUM*4)
#define MAX_WTP_STA_NUMBER  20

#define ESSID_LENGTH	32

#define MAC_ADDR_LEN		6
typedef unsigned int u32;
typedef unsigned short u16;
typedef unsigned char u8;
/********************peiwenhui add 0716 --rate&radio type set************************/
#define IEEE80211_PARAM_MODE_11b	0x01	/* 802.11b */
#define IEEE80211_PARAM_MODE_11a	0x02	/* 802.11a */
#define IEEE80211_PARAM_MODE_11g	0x04	/* 802.11g */
#define IEEE80211_PARAM_MODE_11n	0x08	/* 802.11n */
#define IEEE80211_PARAM_MODE_11an	0x10	/*802.11an*/
#define IEEE80211_PARAM_MODE_11gn	0x20	/*802.11gn*/
/********************peiwenhui add 0716 --rate&radio type set************************/


#define AUTELAN_VENDOR 31656


enum { 
	WTP_WPA_KEY_MGMT_IEEE8021X = 1,   //wpa	+1x
	WTP_WPA_KEY_MGMT_PSK = 2,              	//wpa psk	
	WTP_WPA_KEY_MGMT_NONE = 4,            	 //open 
	WTP_WPA_KEY_MGMT_IEEE8021X_NO_WPA = 8,  // only 1x 	
	WTP_WPA_KEY_MGMT_WPA_NONE = 16, 	
	WTP_WPA_KEY_MGMT_FT_IEEE8021X = 32, 
	WTP_WPA_KEY_MGMT_FT_PSK = 64,
	WTP_WPA_KEY_MGMT_SHARED =128 ,  //shared
	WTP_WPA2_KEY_MGMT_IEEE8021X = 256,   //wpa2	+1x
	WTP_WPA2_KEY_MGMT_PSK = 512,              	//wpa2 psk	
	WTP_WPA2_KEY_MGMT_FT_IEEE8021X = 1024, 
	WTP_WPA2_KEY_MGMT_FT_PSK = 2048,
	WTP_WAPI_KEY_MGMT_PSK = 4096,    //pei add 090309
	WTP_WAPI_KEY_MGMT_CER = 8192    //pei add 090309
}wtp_wpa_key_mgmt;

enum {
 	WTP_WPA_CIPHER_NONE = 1,
	WTP_WPA_CIPHER_WEP40 = 2,
	WTP_WPA_CIPHER_WEP104 = 4,
	WTP_WPA_CIPHER_WEP128 = 8,
	WTP_WPA_CIPHER_TKIP = 16,
	WTP_WPA_CIPHER_CCMP = 32,
	WTP_WPA_CIPHER_AES_128_CMAC = 64,
	WTP_WAPI_CIPHER_SMS4 = 128    //pei add 090309
}wtp_cipher;

enum { /* pei add for XJ license, at 20100507 */
 	WTP_OWN = 0,
	WTP_XJ_POWER_NORMAL,
	WTP_XJ_POWER_HIGH,
	WTP_AQ2000M,
}wtp_manufacture_option;

/* pei add for wapi , at 090505 */
/*设置WAI参数的IOCTL命令字*/
#define P80211_IOCTL_SETWAPI_INFO		(0x8BE0 + 0)

/*设置WAPI参数的IOCTL子命令字,表示设置WAPI状态*/
#define P80211_PACKET_WAPIFLAG			 112


/*设置WAPI参数的IOCTL子命令字,表示安装密钥*/
#define P80211_PACKET_SETKEY     			(u16)0x0003

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

#define KEY_LEN 			16  	
#define MULTI_KEY_LEN  	KEY_LEN
#define WTP_BOARD_ID_LEN 64

/*_____________________________________________________*/
/*  *******************___TYPES___*******************  */
typedef struct {
	unsigned char fc[2];
	unsigned char duration[2];
	unsigned char mac1[6];
	unsigned char mac2[6];
	unsigned char mac3[6];
	unsigned char seqctl[2];
}IEEE80211_Header;
typedef struct {
	unsigned char dmac[6];
	unsigned char smac[6];
}ETH_Header;
typedef struct {
	char *address;
	CWBool received;
	int seqNum;
} CWACDescriptor;

typedef struct wlan_info{
	unsigned char radio_id;
	unsigned char wlan_id;
	unsigned short vlan_id;  //pei add 0320
	unsigned char wlan_updown_time;  //pei add 0225
    /*Begin: caidh add for root wlan wds 2013-03-21*/
	#ifdef MESH_SWITCH
    /*End: caidh add for root wlan wds 2013-03-21*/
	unsigned char wds;
    #endif
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

/****radio struct****/
typedef struct radio_info{
	unsigned char radio_id;			/*radio id*/
	struct radio_info *next;
	char radio_name[IFNAMSIZ];		/*the name of radio EX:wifi0*/
	CWWTPWlan radio_first_wlan; 	/*the first wlan of the radio*/
	int wlancount;					/*wlan count */
	int wlan_id_table[MAX_WLAN_NUM_PER_RADIO];/*the redio's remote wlan ID array */
	CWBool status;            //radio status 1:enable  0:disable
}CWWTPRadio;
/****sta struct*****/
#if 1
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
#endif

typedef struct radio_wmm_set{
	unsigned char radio_id;
	unsigned char enable;
	int set_flag;
	struct radio_wmm_set *next;
}CWWTPWmmSetList;

#if 0
/*hostapd code*/
struct sta_info {
	unsigned char radio_id;
	unsigned char wlan_id;
	struct sta_info *next; /* next entry in sta list */
	struct sta_info *hnext; /* next entry in hash table list */
	unsigned char addr[6];
	unsigned short int aid; /* STA's unique AID (1 .. 2007) or 0 if not yet assigned */
	unsigned int flags;
	unsigned short capability;
	unsigned short listen_interval; /* or beacon_int for APs */
	//char supported_rates[WLAN_SUPP_RATES_MAX];
	char tx_supp_rates;

	enum {
		STA_NULLFUNC = 0, STA_DISASSOC, STA_DEAUTH, STA_REMOVE
	} timeout_next;

	/* IEEE 802.1X related data */
	//struct eapol_state_machine *eapol_sm;

	/* IEEE 802.11f (IAPP) related data */
	s//truct ieee80211_mgmt *last_assoc_req;

	unsigned int acct_session_id_hi;
	unsigned int acct_session_id_lo;
	time_t acct_session_start;
	int acct_session_started;
	int acct_terminate_cause; /* Acct-Terminate-Cause */
	int acct_interim_interval; /* Acct-Interim-Interval */

	unsigned long last_rx_bytes;
	unsigned long last_tx_bytes;
	unsigned char acct_input_gigawords; /* Acct-Input-Gigawords */
	unsigned char acct_output_gigawords; /* Acct-Output-Gigawords */

	unsigned char *challenge; /* IEEE 802.11 Shared Key Authentication Challenge */

	int pairwise; /* Pairwise cipher suite, WPA_CIPHER_* */
	unsigned char *wpa_ie;
	size_t wpa_ie_len;
	struct wpa_state_machine *wpa_sm;
	enum {
		WPA_VERSION_NO_WPA = 0 /* WPA not used */,
		WPA_VERSION_WPA = 1 /* WPA / IEEE 802.11i/D3.0 */,
		WPA_VERSION_WPA2 = 2 /* WPA2 / IEEE 802.11i */
	} wpa;
	int wpa_key_mgmt; /* the selected WPA_KEY_MGMT_* */
	struct rsn_pmksa_cache *pmksa;
	struct rsn_preauth_interface *preauth_iface;
	unsigned char req_replay_counter[8 /* WPA_REPLAY_COUNTER_LEN */];
	int req_replay_counter_used;
	unsigned int dot11RSNAStatsTKIPLocalMICFailures;
	unsigned int dot11RSNAStatsTKIPRemoteMICFailures;
//#ifdef JUMPSTART	
//	struct jsw_session *js_session;
//#endif /* JUMPSTART */

}
#endif

enum VendorSpecPayloadType {
	APScanning=0,
	APThroughputInfo,
	APMaxThroughput,
	ExtendCmd,
	InterfaceUpdown,
	APMonitor,
	StaInfo,
	InterfaceState,
	AttackDetectInfo,
	StaWapiInfo,
	Ntp=10,
	TerminalDisturbInfo,
	StaThroughputSet,
	UserPasswd,
	MultiUserPerformance,
	APMcsMask=15,
	LTE_Device=18,
	probe_request_inhibition = 20,
	cast_isolation = 21,
	cast_rate_setting = 22,
	VSP_T_STA_PACKET_STATISTIC_REPORT=23,
	VSP_T_SET_ACL_LIST = 24,
	VSP_T_SET_E_MENU = 25,
	DEAUTH_DISASSOC_REPORT_TO_AC_SWITCH=26,
	DETAIL_ALL_STA_INFO_TO_AC_SWITCH=27,
	ExtendLongCmdForDHCPOption=28,
	Two_radio_5G_priority_access = 29,
	VSP_T_AP_PACKET_STATISTIC_REPORT=30,//caidh add 2013.03.07,for ap packet statisitc report
	VSP_T_STA_PACKET_STATISTIC_REPORT_20=31,//caidh add for 2.0 AC ,because 2.0AC recieve packets with control channel but 1.3 AC use data channel
	/* Begin: gengzj added for wifipos 2013-11-25 */
	Set_Wifi_Scanning_and_Positioning = 51,
	/* End: gengzj added end */
};
enum LETFISubType {
	//0,//reserved
	AP_LEAVE=1,			//lte-fi quit reason
	LTE_MODE=2,			//2,//for lte mode atc -c "at\$qcsysmode"
};
enum LTEFIQuitResaon {
	//0,//for reserved
	ACC_LEAVE=1,		//ACC超时导致AP断电离线
	//2,//for extend
};
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
	EVENT_T_STA_LEAVE_REASON_DATA_REPORT=15,
	EVENT_T_AP_PACKET_STATISTIC_REPORT = 16,//caidh add 2013.03.07,for ap packet statisitc report
};

/*_____________________________________________________________*/
/*  *******************___WTP VARIABLES___*******************  */
extern char* gInterfaceName;

extern char **gCWACAddresses;
extern int gCWACCount;

extern char *gWTPLocation;
extern char *gWTPName;
extern int gIPv4StatusDuplicate;
extern int gIPv6StatusDuplicate;
extern char *gWTPForceACAddress;
extern char *gWTPACDomainName;
extern char *gWTPSoftWareVersion;
extern char gWTPHardWareVersion[64];  //pei add for test hardversion 0214
extern char gWTPCodeVersion[64];  //pei add for test codeversion 090812
extern char versionbuf[100];
extern unsigned int gManuOption;
extern int gCWWaitImageData;

extern CWAuthSecurity gWTPForceSecurity;

extern CWSocket gWTPSocket;
extern CWSocket gWTPDataSocket;

extern int gWTPPathMTU;

extern CWACDescriptor *gCWACList;
extern CWACInfoValues *gACInfoPtr;
extern int leddev;
extern int blind_status;
extern int gEchoInterval;
extern int gWTPStatisticsTimer;
extern WTPRebootStatisticsInfo gWTPRebootStatistics;
extern CWWTPRadiosInfo gRadiosInfo;
#ifndef CW_NO_DTLS//csc
extern CWSecurityContext gWTPSecurityContext;
extern CWSecuritySession gWTPSession;
extern CWSecurityContext gWTPDataSecurityContext; /* pei data dtls 20100510 */
extern CWSecuritySession gWTPDataSession; /* pei data dtls 20100510 */
#endif
extern CWThreadMutex sta_table_mutex;
extern CWThreadMutex wmm_set_mutex;
extern CWPendingRequestMessage gPendingRequestMsgs[MAX_PENDING_REQUEST_MSGS];

extern CWSafeList gPacketReceiveList;
#ifndef CW_NO_DTLS
extern CWSafeList gPacketDataReceiveList;/*receive data packet*/
#endif
extern CWSafeList gFrameList;
extern CWThreadCondition gInterfaceWait;
extern CWThreadMutex gInterfaceMutex;
extern char WTPModelNum[64];  //pei 0923  /* ap-code, pei 090519 */
extern char WTPRealModelNum[64]; /* ap real model num, pei 090519 */
extern int WTPSerialNum;
extern char WTPBoardID[WTP_BOARD_ID_LEN];
extern CWStateTransition gtemp_state;   //pei add 0618
extern int debug_print;        //pei add 0827-----------
extern CWImageIdentifierValues *gImageIdentifier_ACSupported;        //pei add 0618
extern int gtemp_seqNum;        //0619
extern CWBool isFirstWlan;        	//pei add 0624
extern CWBool receivedDisResp;
extern int gACListSeqNum;
extern char gHostip[24];
extern unsigned char isBackup;
extern unsigned char gRunRetry;
extern int gIPMode;		//diaowenqin add 110226
#ifndef CW_NO_DTLS
extern unsigned char gDtlsSecurity;     /* when set to 1 enable DTLS, 0 disable */
extern unsigned char gDtlsPolicy;  /* when gDtlsSecurity 0, meaningless. when gDtlsSecurity 1, 0 enable control DTLS disable data, 1 enable both */
#endif
/* Begin: gengzj added for wifipos 2013-11-25 */
extern CWSocket gScanPosSocket;
/* End: gengzj added end */
#if 1
typedef struct {
	unsigned char radioId;
	unsigned char channel;
	unsigned char txtype;
	int txpower;
	unsigned short rate;
	unsigned int ratemask;
	CWBool rateAutoEnable;
	unsigned int radiotype;
	unsigned short fragThreshold;
	unsigned short rtsThreshold;
	unsigned char shortRetry;
	unsigned char longRetry;
	unsigned short beaconInterval;
	unsigned char preamble;
	unsigned char dtim;
	CWBool gIsRadioEnable;
} radioInfoValues;
extern radioInfoValues gRadioInfoValue[2];
extern unsigned char gCountryCode[4];
#else
extern unsigned char gchannel;        //pei add 0624
extern int gtxpower;        //pei add 0624
extern unsigned short grate; 			        //pei add 0715
extern CWBool gRateAutoEnable;  //pei add 1128
extern unsigned int gradiotype;         		//pei add 0715
extern unsigned short gFragThreshold;  //pei add 0722
extern unsigned short gRtsThreshold;    //pei add 0729
extern unsigned char gShortRetry;     //pei add 0729
extern unsigned char gLongRetry;     //pei add 0729
extern unsigned short gBeaconInterval;   //pei add 0722
extern unsigned char gPreamble;                //pei add 0722
extern unsigned char gDtim;	                 //pei add 0722
extern unsigned char gIsRadioEnable;	        //pei add 0724
#endif
typedef struct {
	unsigned char radioId;
	unsigned short guardInterval;
	unsigned short mcs;
	unsigned short cwmMode;
       int extoffset;
} modeNParaValues;
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
typedef struct {
	unsigned char type;// 1--dhcpsnooping_report;2--ntp_report
	unsigned char result;
}ntp_report_type;
extern CWConfigureStateRate gConfigureStateRate[2];
extern modeNParaValues gModeNParaValue[2];
extern unsigned char gApScanningEnable;   //pei add 1118
extern unsigned short gApScanningInterval;   //pei add 1127
//extern unsigned char gApScanningThreadEnable;   //pei add 1125
extern unsigned char gAPThroughputInfoCollectEnable; //pei add 0204
extern unsigned short gAPThroughputInfoInterval;
extern unsigned char gStaPktsStatisticEnable;	//diaowq add@20120413
extern unsigned short gStaPktsStatisticInterval;
extern unsigned char gApPktsStatisticEnable;	//caidh add 2013.02.16,ap packet statisitc report opstate 
extern unsigned short gApPktsStatisticInterval;//ap packet statistic report interval
//extern unsigned char gAPThroughputInfoThreadEnable;
extern unsigned char gApMonitorEnable;
extern unsigned char gApMonitorInterval;
extern unsigned char gExtraInfoEnable;
extern unsigned short gExtraInfoInterval;
extern unsigned char gStaInfoEnable;
extern unsigned short gStaInfoInterval;
extern unsigned char gIfStateEnable;
extern unsigned short gIfStateInterval;
extern unsigned char gFloodingDetectEnable;
extern unsigned char gSpoofingDetectEnable;
extern unsigned char gWeakIVDetectEnable;
extern unsigned char gFloodAttackCnt;
extern unsigned char gFloodProbeCnt;
extern unsigned char gFloodWidsIntval;
extern unsigned char gDhcpsnoopingReportEnable;

extern Radio_QosValues gRadioQosValues; //pei add 0207
extern unsigned char gMaxThroughput;  //pei add 0209
extern WlanMaxThroughputValues gWlanMaxThroughputValue[16];
extern unsigned char gQosEnable;  //pei add 0210
extern unsigned char gQosNewState; //sun add 
extern unsigned short gPowerOnUntilRuntime;  //pei add 0214
extern unsigned char g_DelStaMac[6];
extern int WTPBoardRevesion;
extern unsigned char WTPBaseMAC[6];
extern int gNetLink_signal;
extern CWBool CWWTPNeighborDeadTimerExpiredFlag;
extern CWBool WTPWLanReceiveThreadRun ;
extern CWBool WTPDataChannelThreadRun ;
extern int gRadioCount;
extern int gWlanIDMap[16];
extern unsigned char gWorkMode;
extern EMenuConf gEMenuConf;

//extern CWWTPWlan  wtp_wlan[MAX_WLAN_NUM_PER_WTP];
extern CWWTPRadio *wtp_radio_list;
extern CWWTPWlan *wtp_wlan_list;
extern CWWTPSta *wtp_sta_list;
extern CWRtransMsgList gRtransMsgList;
extern CWWTPWmmSetList *g_wmm_list;

extern int echo_retran_count;

extern int wtp_wlan_count;/*the wlan number of the wtp*/
extern int wtp_sta_count;/*the sta count of the wtp*/
extern int receiveframethread;

extern CWThread thread_wtp_throughput_info;
extern CWThread thread_apScanning;
extern CWThread thread_apMonitor;
extern CWThread thread_extraInfo;
extern CWThread thread_staInfo;
extern CWThread thread_ifState;
extern CWThread thread_snooping;
extern CWThread thread_sta_pkts_statistic_report;
extern CWThread thread_ap_pkts_statistic_report;//caidh add 2013.02.16,new thread for ap packet statistic report 

/*__________________________________________________________*/
/*  *******************___PROTOTYPES___*******************  */

//in WTP.c
void CWWTPGetWlanName(char *wlanname,unsigned char radioId, unsigned char wlanId);
void CWWTPGetRadioName(char *radioame,int radio_id);

CWBool CWWTPLoadConfiguration();
CWBool CWWTPLoadBoardConfiguration();

CWBool CWWTPInitConfiguration();
void CWWTPResetRadioStatistics(WTPRadioStatisticsInfo *radioStatistics);
CWBool CWReceiveMessage(CWProtocolMessage *msgPtr);
CWBool CWWTPSendAcknowledgedPacket(int seqNum, CWList msgElemlist, CWBool (assembleFunc) (CWProtocolMessage **, int *, int, int, CWList), CWBool (parseFunc) (char*, int, int, void*), CWBool (saveFunc) (void*), void *valuesPtr);
void CWWTPDestroy();
int macAddrCmp (unsigned char* addr1, unsigned char* addr2);/*compare two MAC*/
CWBool CWWTPUpdate(unsigned char *imgname,unsigned char *ip);
CWBool CWAssembleResetResponse(CWProtocolMessage **messagesPtr, int *fragmentsNumPtr, int PMTU, int seqNum, CWProtocolResultCode resultCode) ;

CWBool apservicerestart(void);
unsigned int ip_int2str(unsigned int ipAddress,unsigned char *buff);  //pei add 0214

CWBool WlanTableAdd(CWWTPWlan *addWlanValues, unsigned short vlanId);     //pei add 0624
CWBool WLanTableDelete(DeleteWlanValues *deleteWlanValues);
CWBool STATableAdd(AddSTAValues *addSTAValues);     //pei add 0708
CWBool STATableDelete(DeleteSTAValues *deleteSTAValues);     //pei add 0708
CWBool CWWTPLoadVersionConfiguration();
int inet_atoi(char *cp, unsigned int *ap);

//9350
int system_new(char *cmd);
CWBool CWDeleteStaInfoInTable_StaGetOff(CWRtransmMsg *retransmsg,CWProtocolMessage *completeMsg,DeleteSTAValues *deleteSTAValues);
CWBool CWDeleteStaInfoInTable_StaLeave(unsigned char *sta_lev_data,CWProtocolMessage *protocolMsg,DeleteSTAValues *deleteSTAValues);
CWBool CWDeleteStaInfoInTableBy(int type,DeleteSTAValues *deleteSTAValues, int notice_driver);
CWBool Check_DeleteInfo_In_StaTable(unsigned char *sta_lev_data,CWProtocolMessage *protocolMsg);



//in WTPRunState.c
CWBool CWAssembleWTPEventRequest(CWProtocolMessage **messagesPtr, int *fragmentsNumPtr, int PMTU, int seqNum, CWList msgElemList);
CWBool CWAssembleWTPEventDeleteStationRequest (CWProtocolMessage **messagesPtr, int *fragmentsNumPtr, int PMTU, int seqNum, DeleteSTAValues *valuesPtr); 	//pei add 0703
CWBool CWParseWTPEventRequestDeleteStationMessage(char *msgPtr, int len, DeleteSTAValues *valuesPtr);   //pei add 0703
CWBool CWParseTwoRadio5gPriorityAccess(CWProtocolMessage *msgPtr, int len, TwoRadio5GPriorityAccess*valPtr);
CWBool CWParseGetApPktsStatisticRpt(CWProtocolMessage *msgPtr, int len, GetApPktsStatisticRptValues *valPtr);
CWBool CWSaveTwoRadio5gPriorityAccess(TwoRadio5GPriorityAccess*twoRadio5GPriorityAccess);
CWBool CWSetApPktsStatisticRpt(GetApPktsStatisticRptValues *getApPktsStatisticRptValues);

CW_THREAD_RETURN_TYPE CWWTPReceiveDtlsPacket(void *arg);
CWBool CWWTPCheckForBindingFrame();

//in WTPProtocol_User.c
CWBool CWWTPGetACNameWithIndex (CWACNamesWithIndex *ACsInfo);
int CWWTPGetStatisticsTimer ();
int CWGetRadioMaxCount();
void CWWTPGetIPv6Address(struct sockaddr_in6* myAddr);
CWBool CWGetWTPRadiosAdminState(CWRadiosAdminInfo *valPtr);
CWBool CWGetDecryptErrorReport(int radioID, CWDecryptErrorReportInfo *valPtr);

//in WTPRetransmission.c
int CWSendPendingRequestMessage(CWPendingRequestMessage *pendingRequestMsgs, CWProtocolMessage *messages, int fragmentsNum);
int CWFindPendingRequestMsgsBox(CWPendingRequestMessage *pendingRequestMsgs, const int length, const int msgType, const int seqNum);
void CWResetPendingMsgBox(CWPendingRequestMessage *pendingRequestMsgs);
CWBool CWUpdatePendingMsgBox(CWPendingRequestMessage *pendingRequestMsgs, unsigned char msgType, int seqNum,  int timer_sec, CWTimerArg timer_arg, void (*timer_hdl)(CWTimerArg), int retransmission, CWProtocolMessage *msgElems,int fragmentsNum);

//in WTPDriverInteraction.c
int set_cwmin(int sock, struct iwreq wrq, int acclass, int sta, int value);
int get_cwmin(int sock, struct iwreq* wrq, int acclass, int sta);
int set_cwmax(int sock, struct iwreq wrq, int acclass, int sta, int value);
int get_cwmax(int sock, struct iwreq* wrq, int acclass, int sta);
int set_aifs(int sock, struct iwreq wrq, int acclass, int sta, int value);
int get_aifs(int sock, struct iwreq* wrq, int acclass, int sta);
int Check_Interface_State(char *ifname);
CWBool CWGetInterfaceStatus(char *ifname, unsigned char *status);
void CWSetLastConnectLocalIP(void);
void CWSetLastConnectGateway(void);
int CWGetIPMode(void);
void WTPInitConfig();
CWBool WTPGetApRebootFlag();
extern int CWGetWlanIdxInMap(int * map, int radio_id, int wlan_id);
//CWBool printstats(FILE *fd,  struct ieee80211_stats *stats);

/**/
/*************************wlanconfig*******************************/
/*------------------------addwlan--------------------------------*/
int wapid_ioctl(char *ifname, unsigned short cmd, char *buf, int buf_len); //pei add 090505
int create_wlan(int sock, AddWlanValues *addWlanValues, WPA_IE *wpa_ie, WlanVlanValues *wlanVlanValues, WapiValues *wapiValues,int sameradio);   //pei add 0624
int delete_wlan(DeleteWlanValues *value);
int add_sta(int sock,AddSTAValues *addStaValues);
int del_sta(int sock, DeleteSTAValues *deleteSTAValues, unsigned char *wlan_id);
int madwifi_set_key(int sock,STASessionKeyValues *staSessionKey,int alg,unsigned char radioId,unsigned char wlan_id);
int madwifi_set_ieee8021x(int sock, WPA_IE *wpa_ie, unsigned char radioId,unsigned char wlan_id, int wpa_enable);
int madwifi_set_privacy(int sock, unsigned char radioId,unsigned char wlan_id, int enabled);
int set80211priv(int sock,char *ifname, int op, void *data, int len);
int madwifi_configure_wpa(int sock, WPA_IE *wpa_ie, char *ifname, int wpa_enable);
int set_channel_cmd(char *wlanname ,int channel);
int set_puren_cmd(char *wlanname ,unsigned char state);
int set_channel_extoffset_cmd(char *wlanname ,char *mode ,int extoffset,unsigned short cwmmode);
int set_ampdu_cmd(char *wlanname ,unsigned char state ,int ampdu_limit,int ampdu_frames);
int set_wifi_ampdu_cmd(char *wlanname ,unsigned char state ,int ampdu_limit,int ampdu_frames);
int set_amsdu_cmd(char *wlanname ,unsigned char state ,int amsdu_limit);/*yuan add ...*/
int set_wifi_amsdu_cmd(char *wlanname ,unsigned char state ,int amsdu_limit);/*yuan add ...*/
int set_tx_chainmask_cmd(char *wlanname ,unsigned char state );
int set_wifi_tx_chainmask_cmd(char *wlanname ,unsigned char state );
int set_rx_chainmask_cmd(char *wlanname ,unsigned char state );
int set_wifi_rx_chainmask_cmd(char *wlanname ,unsigned char state );
int set_dhcp_snooping(unsigned char state);
int set_sta_report_by_dhcpsnooping(unsigned char state);
int set_txpower_cmd(char *wlanname, unsigned char txtype, int txpower);
int set_rate_cmd(char *wlanname, unsigned int radioid,unsigned int ratemask);   //pei add 0716
int set_radiotype_cmd(unsigned char radioId, unsigned char wlanId, int radiotype);  //pei add 0716
int set_fragthreshold_cmd(char *wlanname, unsigned short fragThreshold);         //pei add 0722
int set_rtsthreshold_cmd(char *wlanname, unsigned short rtsThreshold);         //pei add 0729
int set_radioconfig_cmd(char *wlanname, unsigned char preamble, unsigned char dtim, unsigned short beaconInterval);         //pei add 0722
CWBool get_radio_mode(int radiotype,char **mode);
CW_THREAD_RETURN_TYPE CWWTPCheckNetLink(void *arg);      //pei add 0703
CW_THREAD_RETURN_TYPE CWWTPSetApScanning(void *arg);  //pei test for rogue AP 1120
/* Begin: gengzj added for wifipos 2013-11-25 */
#ifdef _SCANPOS_RESULT_REPORT_
CW_THREAD_RETURN_TYPE CWWTPReceiveWifiScanAndPosResult(void* arg);
#endif /* _SCANPOS_RESULT_REPORT_ */
/* End: gengzj added end */
CW_THREAD_RETURN_TYPE CWWTPSetApMonitor(void *arg);      //pei add 0226
CW_THREAD_RETURN_TYPE CWWTPSetExtraInfo(void *arg);  //pei add 0226
CW_THREAD_RETURN_TYPE CWWTPGetStaInfo(void *arg);
CW_THREAD_RETURN_TYPE CWWTPGetIfState(void *arg);
CW_THREAD_RETURN_TYPE CWWTPReceive802_11Frame(void *arg);
CW_THREAD_RETURN_TYPE CWWTPReceive802_3Frame(void *arg);
CW_THREAD_RETURN_TYPE CWWTPCheckMsgFromOtherPro(void *arg);
CW_THREAD_RETURN_TYPE acw_wtp_throughput_info(void *arg) ;
CW_THREAD_RETURN_TYPE CWWTPStaPktsStatisticReport(void *arg);
CW_THREAD_RETURN_TYPE CWWTPApPktsStatisticReport(void *arg);//caidh add 2013.03.07
CWBool CWSetApScanning(ApScanningSetValues *apScanningSetValues);  //pei test for rogue AP 1120
CWBool CWSetApMonitor(ApMonitorSetValues *apMonitorSetValues);      //pei add 0226
CWBool CWSetApExtraInfo(ExtraInfoValues *extraInfoValues);
CWBool CWGetStaInfo(GetStaInfoValues *getStaInfoValues);
CWBool CWGetIfState(GetIfStateValues *getIfStateValues);
CWBool CWGetAttackDetectInfo(GetAttackDetectInfoValues *getAttackDetectInfoValues);
CWBool CWGetStaWapiInfo(GetStaWapiInfoValues *getStaWapiInfoValues);
CWBool CWSetNtp(GetNtpValues *NtpValues);
CWBool CWSetTerminalDisturb(GetTerminalDisturbValues *TerminalDisturbValues);
CWBool CWSetApThroughputInfoCollect(ApThroughputInfoValues *apThroughputInfoCollectValues);  //pei add 0204
CWBool CWget_wtp_throughput_Info(ApThroughputInfoValues *apThroughputInfoCollectValues);
CWBool CWGetAPThroughputInfo(int wlanCount, int ethCount, int wifiCount, wlan_stats_info *wlanStatsInfo);  //pei add 0205
CWBool CWAssembleMsgElemVendorSpecificPayload(CWProtocolMessage *msgPtr, int interface_count, wlan_stats_info *valuesPtr);     //pei add 0205
CWBool isescapeletter(char c);
CWBool CWSetUserandPasswdinfo (WtpUserPasswdValues *valPtr);
CWBool CWSetACLConfigForNewWlan(int radio_id, int wlan_id);
CWBool CWSaveGetDisauthDisassocvalues(DisassocDeauthValues *disassocDeauthValues);
CWBool CWSaveGetAllstaLeavDetailIValues(DetailAllStaInfoValues *detailAllStaInfoValues);

//in WTPDiscoveryState.c
CWStateTransition CWWTPEnterDiscovery();
void CWWTPPickACInterface();

CWStateTransition CWWTPEnterSulking();
CWStateTransition CWWTPEnterJoin();
CWStateTransition CWWTPEnterImageData();       //pei add 0716
CWStateTransition CWWTPEnterConfigure();
CWStateTransition CWWTPEnterDataCheck();
CWStateTransition CWWTPEnterRun();
CWStateTransition CWWTPEnterReset();


CWBool CWStartHeartbeatTimer();
CWBool CWStopHeartbeatTimer();
CWBool CWStartNeighborDeadTimer();
CWBool CWStopNeighborDeadTimer();
CWBool CWResetTimers();

void CWWTPHeartBeatTimerExpiredHandler(void *arg); 
void CWWTPRetransmitTimerExpiredHandler(CWTimerArg arg);
/*in WTPBinding.c*/


CWBool CWNetworkInitSocketClientUnconnect(CWSocket *sockPtr, struct sockaddr_in *addrPtr, int port, char *hostip);   /* pei test 1222 */

/**************************** iwconfig ****************************/
/*--------------------------- Frequency ---------------------------*/
int set_freq(int sock, struct iwreq wrq, int value);
/*--------------------------- Transmit Power ---------------------------*/
/*remeember that fox =1 you can change the tx power you have to change it to 0 after change*/
int set_txpower(int sock, struct iwreq wrq, int value);
/*watchdog*/
CWBool CWWTPStartWatchdog();
CWBool CWStartWatchDogTimer();
void CWWTPWatchDogTimerExpiredHandler(void *arg);

/******************************  WTPFrameReceive.c  **************************/
int getMacAddr(int sock, char* interface, unsigned char* macAddr);
CWBool CWGetInterfaceCount(int *count, char *type);
CWBool PopenFile (char *cmd_str,char *str,int len );
#define CONF_WTPD_PATH 		"/tmp/conf_wtpd"
#define CONF_RADIO_PATH 	"/tmp/conf_wtpd/radio"
CWBool _system(char *cmd);
void dpf(const char *format, ...);
/* Begin: gengzj added for wifipos 2013-11-25 */
void printmsg(int n ,unsigned char *buffer);
/* End: gengzj added end */
CWBool kes_debug(char *buf);

char * trim(char *);
CWBool CWGetHostIP(char *);
CWBool CWWTPCheckNetLinkForBackUp(void);
CWBool CWWTPGetAddedStaInfo(AddSTAValues *);

CWBool STAMacRefreshForBackup(int);

CWBool CWParseProbeRequestInhibition(CWProtocolMessage *msgPtr,int len, ProbeRequestInhibition *valPtr);
CWBool CWSaveProbeRequestInhibition(ProbeRequestInhibition *valPtr);
CWBool CWParseCastIsolation(CWProtocolMessage *msgPtr, int len, CastIsolation_CastRate *valPtr);
CWBool CWParseCastRate(CWProtocolMessage *msgPtr, int len, CastIsolation_CastRate *valPtr);
CWBool CWSaveCastIsolation_CastRate(CastIsolation_CastRate *valPtr);
CWBool CWXmlWriteWTPDParam(char *file, char *arg_name, char *arg_val);
CWBool CWClearEMenuConf();




CWBool CWSaveWmmInlist(unsigned char radio_id,unsigned char qos_enable);



#endif

