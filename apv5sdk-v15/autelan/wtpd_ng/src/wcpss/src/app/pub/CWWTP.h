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
#define MODEL_AQ1000		1000/*the model num of aq1000*/
#define MODEL_AQ1200		1200/*the model num of aq1200*/
#define MODEL_AQ3000		3000/*the model num of aq3000*/
#define MAX_RADIO_NUM		4
#define MAX_WLAN_NUM_PER_RADIO        4
#define MAX_WLAN_NUM_PER_WTP		(MAX_RADIO_NUM*4)
#define MAX_WTP_STA_NUMBER  20

#define ESSID_LENGTH	32


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
	probe_request_inhibition = 20,
	cast_isolation = 21,
	cast_rate_setting = 22,
	VSP_T_STA_PACKET_STATISTIC_REPORT=23,
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
extern int gWlanIDMap[16];

//extern CWWTPWlan  wtp_wlan[MAX_WLAN_NUM_PER_WTP];
extern CWWTPRadio *wtp_radio_list;
extern CWWTPWlan *wtp_wlan_list;
extern CWWTPSta *wtp_sta_list;



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

/*__________________________________________________________*/
/*  *******************___PROTOTYPES___*******************  */

//in WTP.c
void CWWTPGetWlanName(char *wlanname,unsigned char radioId, unsigned char wlanId);
void CWWTPGetWifiName(char *radioame,int radio_id);

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



//in WTPRunState.c
CWBool CWAssembleWTPEventRequest(CWProtocolMessage **messagesPtr, int *fragmentsNumPtr, int PMTU, int seqNum, CWList msgElemList);
CWBool CWAssembleWTPEventDeleteStationRequest (CWProtocolMessage **messagesPtr, int *fragmentsNumPtr, int PMTU, int seqNum, DeleteSTAValues *valuesPtr); 	//pei add 0703
CWBool CWParseWTPEventRequestDeleteStationMessage(char *msgPtr, int len, DeleteSTAValues *valuesPtr);   //pei add 0703

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
int set_channel_cmd(int radio_id, int wlan_id,int channel);
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
int set_radioconfig_cmd(unsigned char radioId, unsigned char wlanId, unsigned char preamble, unsigned char dtim, unsigned short beaconInterval);         //pei add 0722
CWBool get_radio_mode(int radiotype,char **mode);
CW_THREAD_RETURN_TYPE CWWTPCheckNetLink(void *arg);      //pei add 0703
CW_THREAD_RETURN_TYPE CWWTPSetApScanning(void *arg);  //pei test for rogue AP 1120
CW_THREAD_RETURN_TYPE CWWTPSetApMonitor(void *arg);      //pei add 0226
CW_THREAD_RETURN_TYPE CWWTPSetExtraInfo(void *arg);  //pei add 0226
CW_THREAD_RETURN_TYPE CWWTPGetStaInfo(void *arg);
CW_THREAD_RETURN_TYPE CWWTPGetIfState(void *arg);
CW_THREAD_RETURN_TYPE CWWTPReceive802_11Frame(void *arg);
CW_THREAD_RETURN_TYPE CWWTPReceive802_3Frame(void *arg);
CW_THREAD_RETURN_TYPE CWWTPCheckMsgFromOtherPro(void *arg);
CW_THREAD_RETURN_TYPE acw_wtp_throughput_info(void *arg) ;
CW_THREAD_RETURN_TYPE CWWTPStaPktsStatisticReport(void *arg);
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
CWBool PopenFile (char *cmd_str,char *str,int len );
#define CONF_WTPD_PATH 		"/tmp/conf_wtpd"
#define CONF_RADIO_PATH 	"/tmp/conf_wtpd/radio"

char * trim(char *);
CWBool CWGetHostIP(char *);
CWBool CWWTPCheckNetLinkForBackUp(void);
CWBool STAMacRefreshForBackup(int);
CWBool CWParseProbeRequestInhibition(CWProtocolMessage *msgPtr,int len, ProbeRequestInhibition *valPtr);
CWBool CWSaveProbeRequestInhibition(ProbeRequestInhibition *valPtr);
CWBool CWParseCastIsolation(CWProtocolMessage *msgPtr, int len, CastIsolation_CastRate *valPtr);
CWBool CWParseCastRate(CWProtocolMessage *msgPtr, int len, CastIsolation_CastRate *valPtr);
CWBool CWSaveCastIsolation_CastRate(CastIsolation_CastRate *valPtr);







#endif

