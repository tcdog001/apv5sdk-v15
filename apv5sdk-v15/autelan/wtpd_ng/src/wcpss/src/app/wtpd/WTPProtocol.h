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

 
#ifndef __CAPWAP_WTPProtocol_HEADER__
#define __CAPWAP_WTPProtocol_HEADER__
#include "WTPBinding.h"                //pei add 0624
/*_____________________________________________________*/
/*  *******************___TYPES___*******************  */
typedef struct {
	int ACIPv4ListCount;
	int *ACIPv4List;	
}ACIPv4ListValues;

typedef struct {
	int ACIPv6ListCount;
	struct in6_addr *ACIPv6List;	
}ACIPv6ListValues;

typedef struct {
	int stations;
	int limit;
	int activeWTPs;
	int maxWTPs;
	CWAuthSecurity security;
	int RMACField;
//	int WirelessField;
	int DTLSPolicy;
	CWACVendorInfos vendorInfos;
	char *name;
	CWProtocolIPv4NetworkInterface *IPv4Addresses;
	int IPv4AddressesCount;
	CWProtocolIPv6NetworkInterface *IPv6Addresses;
	int IPv6AddressesCount;
	ACIPv4ListValues ACIPv4ListInfo;
	ACIPv6ListValues ACIPv6ListInfo;
	CWNetworkLev4Address preferredAddress;
	CWNetworkLev4Address incomingAddress;
} CWACInfoValues;

typedef struct {
	unsigned char NumOfVersion;
	unsigned char reserved;
	unsigned short imagedata_time;
	unsigned short type;
	unsigned short length;
	char *values;
}CWImageIdentifierValues;

typedef struct {
	CWACInfoValues ACInfoPtr;
	CWImageIdentifierValues ImageIdentifier;              //pei add 0617
	CWProtocolResultCode code;
	ACIPv4ListValues ACIPv4ListInfo;
	ACIPv6ListValues ACIPv6ListInfo;
} CWProtocolJoinResponseValues;

typedef struct {      //pei add 1118
	unsigned char value;
	unsigned char opstate;
	unsigned short reportinterval;
}ApScanningSetValues;

typedef struct  {
	unsigned char optype;
	unsigned short int len;
	unsigned char radioid;
	unsigned char wlanid;
	unsigned short int opstate;
	unsigned int stainterval;
	unsigned int throughput_threshold;
}StaThroughputSetValues;

typedef struct {      //pei add 0320
	unsigned char value;
	unsigned char opstate;
	unsigned short reportinterval;
}ExtraInfoValues;

typedef struct {      //pei add 0204
	unsigned char value;
	unsigned char opstate;
}ApThroughputInfoValues;

typedef struct {      //pei add 0302
	unsigned char wlanId;
	unsigned char maxThroughput;
}WlanMaxThroughputValues;
typedef struct {      //pei add 0302
	unsigned char apMaxThroughput;
	unsigned char wlanCount;
	WlanMaxThroughputValues *wlanMaxThroughput;
}ApMaxThroughputValues;

typedef struct {      //pei add 0226
	unsigned char value;
	unsigned char opstate;
	unsigned char reportinterval;
}ApMonitorSetValues;

typedef struct {
	ACIPv4ListValues ACIPv4ListInfo;
	ACIPv6ListValues ACIPv6ListInfo;
	int discoveryTimer;
	int echoRequestTimer;
	int radioOperationalInfoCount;
	CWRadioOperationalInfoValues *radioOperationalInfo;
	WTPDecryptErrorReport radiosDecryptErrorPeriod;
	int idleTimeout;
	int fallback;
	int apScanningSetCount;      //pei add 1205
	ApScanningSetValues *apScanningSet;      //pei add 1205
	int apThroughputSetCount;      //pei add 0204
	ApThroughputInfoValues *apThroughputSet;      //pei add 0204
	int maxThroughputSetCount;      //pei add 0302
	ApMaxThroughputValues *maxThroughputSet;      //pei add 0302
//	unsigned char maxThroughput;      //pei add 0209
	void * bindingValues;
} CWProtocolConfigureResponseValues;

typedef struct {      //pei add 0417
	unsigned char value;
	unsigned char opstate;
	unsigned short reportinterval;
}GetStaInfoValues;

typedef struct {      //pei add 0422
	unsigned char value;
	unsigned char opstate;
	unsigned short reportinterval;
}GetIfStateValues;

typedef struct {
	unsigned char value;
	unsigned char floodingDetectEnable;
	unsigned char spoofingDetectEnable;
	unsigned char weakIVDetectEnable;
	unsigned char flood_attack_cnt;
	unsigned char flood_probe_cnt;
	unsigned char wids_intval;
}GetAttackDetectInfoValues;

typedef struct {
	unsigned char value;
	unsigned char opstate;
	unsigned short reportinterval;
}GetStaWapiInfoValues;

typedef struct {
	unsigned char value;
	unsigned char ipv4_v6;// 0--ipv4;1--ipv6
	unsigned char state;
	unsigned int ntp_intval;
	unsigned int ipv4;
	char * ipv6;
}GetNtpValues;

typedef struct {
	unsigned char value;
	unsigned char opstate;
	unsigned short terDisturbStaCnt;
	unsigned short terDisturbPkt;
	/* 4Bytes reserved*/
}GetTerminalDisturbValues;

typedef struct {
	unsigned char op_type;
	unsigned char radio_id;
	unsigned char wlan_id;
	unsigned char state;
	unsigned char value;
}GetMultiUserPerformanceValues;

typedef struct {
	unsigned char radio_id;
	unsigned int value;
}GetMcsMaskValues;

typedef struct {
	unsigned char eth_id;
	unsigned int mtu;
}set_eth_mtu_vlaues;
typedef struct {
	unsigned char optype;
	unsigned short timeinterval;
}set_all_time_vlaues;

typedef struct {
	char *username;
	char *passwd;
}WtpUserPasswdValues;


typedef struct {     
	unsigned short elem_type;
	unsigned short opstate;
	unsigned short reportinterval;
}GetStaPktsStatisticRptValues;

typedef struct {
	int timeStampCount;
	int timeStampValue;
	int adminStateCount;
	AdminStateValues *adminState;
	int WTPStaticIPCount;
	WTPStaticIPValues *WTPStaticIP;
	int WTPStaticIPV2Count;
	WTPStaticIPValues *WTPStaticIPV2;
	int WTPModeNParaCount;
	WTPModeNParaValues *WTPModeNPara;
	int apScanningSetCount;
	ApScanningSetValues *apScanningSet;
	int staThroughputSetCount;
	StaThroughputSetValues *staThroughputSet;
	int apThroughputSetCount;
	ApThroughputInfoValues *apThroughputSet;
	int maxThroughputSetCount;
	ApMaxThroughputValues *maxThroughputSet;
	unsigned char cmdLength;
	unsigned char *cmd;
	int apMonitorSetCount;
	ApMonitorSetValues *apMonitorSet;
	int extraInfoCount;
	ExtraInfoValues *extraInfo;
	int getStaInfoCount;
	GetStaInfoValues *getStaInfo;
	int getIfStateCount;
	GetIfStateValues *getIfState;
	int getAttackDetectInfoCount;
	GetAttackDetectInfoValues *getAttackDetectInfo;
	int getStaWapiInfoCount;
	GetStaWapiInfoValues *getStaWapiInfo;
	int getNtpCount;
	GetNtpValues *getNtpInfo;
	int getTerminalDisturbCount;
	GetTerminalDisturbValues *getTerminalDisturbInfo;
	int getMultiUserPerformanceCount;
	GetMultiUserPerformanceValues *getMultiUserPerformance;
	int getMcsMaskCount;
	GetMcsMaskValues *getMcsMask;
	int eth_mtu_count;
 	set_eth_mtu_vlaues *eth_mtu_info;
	int time_count;
 	set_all_time_vlaues *time_info;
	int userpasswdcount;
	WtpUserPasswdValues *userpasswdinfo;
	int ProbeRequestInhibition_count;
	ProbeRequestInhibition *ProbeRequestInhibition_value;
	int CastIsolation_CastRate_count;
	CastIsolation_CastRate *CastIsolation_CastRate_value;
	int getStaPktsStatisticRptCount;
	GetStaPktsStatisticRptValues *getStaPktsStatisticRpt;
	void * bindingValues;
} CWProtocolConfigurationUpdateRequestValues;

//diaowq add@20120413,for sta pkts statistic report ,via data tunnel.
#define STA_DATA_RATE_COUNT			44			//base rate + support mcs count
#define SIGNAL_STRENGTH_REGION 		17
#define MAC_ADDR_LEN 					6
#define WIRELESS_HEADER_LEN			24
#define LLC_LEN							8
#define MCS_COUNT_PER_STREAM		8
#define CURRENT_SUPPORT_STREAM_COUNT 	3	//we support 2 stream,current drv complete 3.
#define CURRENT_SUPPORT_MCS_COUNT	 MCS_COUNT_PER_STREAM*CURRENT_SUPPORT_STREAM_COUNT
/*the statistic info of one sta*/
typedef struct {
	unsigned char sta_mac[MAC_ADDR_LEN];
	unsigned char bssid[MAC_ADDR_LEN];
	unsigned int StaTxDataRatePkts[STA_DATA_RATE_COUNT];
	unsigned int StaRxDataRatePkts[STA_DATA_RATE_COUNT];
	unsigned int StaTxSignalStrengthPkts[SIGNAL_STRENGTH_REGION];
}StaPktsStatistic;
/*the statistic info of one wlan*/
typedef struct {
	unsigned int sta_count;					//sum sta count,assiciated the wlan.
	StaPktsStatistic *pStaPktsStatistic;		
}WlanStaPktsStatisticRpt;


typedef struct {  //peiwenhui add 090205
	unsigned int tx_unicast;
	unsigned int tx_broadcast;
	unsigned int tx_multicast;
	unsigned int tx_drop;
	unsigned int rx_unicast;
	unsigned int rx_broadcast;
	unsigned int rx_multicast;
	unsigned int rx_drop;
}eth_package_info;


/*__________________________________________________________*/
/*  *******************___PROTOTYPES___*******************  */
CWBool CWAssembleMsgElemACName(CWProtocolMessage *msgPtr);				// 4
CWBool CWAssembleMsgElemACNameWithIndex(CWProtocolMessage *msgPtr);			// 5
CWBool CWAssembleMsgElemDiscoveryType(CWProtocolMessage *msgPtr);			//20
CWBool CWAssembleMsgElemDuplicateIPv4Address(CWProtocolMessage *msgPtr);		//21
CWBool CWAssembleMsgElemLocationData(CWProtocolMessage *msgPtr);			//27
CWBool CWAssembleMsgElemStatisticsTimer(CWProtocolMessage *msgPtr);			//33
CWBool CWAssembleMsgElemWTPBoardData(CWProtocolMessage *msgPtr);			//35
CWBool CWAssembleMsgElemWTPDescriptor(CWProtocolMessage *msgPtr);			//36
CWBool CWAssembleMsgElemWTPFrameTunnelMode(CWProtocolMessage *msgPtr);			//38
CWBool CWAssembleMsgElemWTPIPv4Address(CWProtocolMessage *msgPtr);			//39
CWBool CWAssembleMsgElemWTPMACType(CWProtocolMessage *msgPtr);				//40
CWBool CWAssembleMsgElemWTPName(CWProtocolMessage *msgPtr);				//41
CWBool CWAssembleMsgElemWTPOperationalStatistics(CWProtocolMessage *msgPtr,int radio);	//42
CWBool CWAssembleMsgElemWTPRadioStatistics(CWProtocolMessage *msgPtr,int radio);	//43
CWBool CWAssembleMsgElemWTPRebootStatistics(CWProtocolMessage *msgPtr);			//44
CWBool CWAssembleMsgElemDeleteStation(CWProtocolMessage *msgPtr, DeleteSTAValues *valuesPtr);             //pei add 0703--------------
CWBool CWAssembleMsgElemIEEE80211DirectSequenceControl(CWProtocolMessage *msgPtr);        //pei add 0716
CWBool CWAssembleMsgElemIEEE80211TxPower(CWProtocolMessage *msgPtr);               			//pei add 0716
CWBool CWAssembleMsgElemIEEE80211WTPRadioInfo(CWProtocolMessage *msgPtr);               		//pei add 0716
CWBool CWAssembleMsgElemImageIdentifier(CWProtocolMessage *msgPtr);
//CWBool CWAssembleMsgElemWTPStaticIPInfo(CWProtocolMessage *msgPtr);			//45

//CWBool CWAssembleMsgElemWTPRadioInformation(CWProtocolMessage *msgPtr);	

//---------------------------------------------------------/
CWBool CWParseACDescriptor(CWProtocolMessage *msgPtr, int len, CWACInfoValues *valPtr);					// 1
CWBool CWParseACIPv4List(CWProtocolMessage *msgPtr, int len, ACIPv4ListValues *valPtr);					// 2
CWBool CWParseACIPv6List(CWProtocolMessage *msgPtr, int len, ACIPv6ListValues *valPtr);					// 3
CWBool CWParseCWControlIPv4Addresses(CWProtocolMessage *msgPtr, int len, CWProtocolIPv4NetworkInterface *valPtr);	//10 
CWBool CWParseCWControlIPv6Addresses(CWProtocolMessage *msgPtr, int len, CWProtocolIPv6NetworkInterface *valPtr);	//11 
CWBool CWParseCWTimers (CWProtocolMessage *msgPtr, int len, CWProtocolConfigureResponseValues *valPtr);			//12 
CWBool CWParseDecryptErrorReportPeriod (CWProtocolMessage *msgPtr, int len, WTPDecryptErrorReportValues *valPtr);	//16 
CWBool CWParseIdleTimeout (CWProtocolMessage *msgPtr, int len, CWProtocolConfigureResponseValues *valPtr);		//26 
CWBool CWParseWTPFallback (CWProtocolMessage *msgPtr, int len, CWProtocolConfigureResponseValues *valPtr);		//37 
CWBool CWParseIeee80211DirectSequenceControl (CWProtocolMessage *msgPtr, int len, DirectSequeueControlValues *valPtr);        //pei add 0623
CWBool CWParseIeee80211TxPower (CWProtocolMessage *msgPtr, int len, TxPowerValues *valPtr);        //pei add 0623
CWBool CWParseRadioAdminState (CWProtocolMessage *msgPtr, int len, AdminStateValues *valPtr);        //pei add 0724
CWBool CWParseWTPStaticIP (CWProtocolMessage *msgPtr, int len, WTPStaticIPValues *valPtr);        //pei add 0214
CWBool CWParseWTPStaticIPV2 (CWProtocolMessage *msgPtr, int len, WTPStaticIPValues *valPtr);
CWBool acwparse_eth_config_mtu (CWProtocolMessage *msgPtr, int len, set_eth_mtu_vlaues*valPtr);
CWBool acwparse_time_config(CWProtocolMessage *msgPtr, int len, set_all_time_vlaues*valPtr);
CWBool CWParseWTPModeNPara(CWProtocolMessage *msgPtr, int len, WTPModeNParaValues *valPtr);
CWBool CWParseApScanningSet (CWProtocolMessage *msgPtr, int len, ApScanningSetValues *valPtr);        //pei add 1118
CWBool CWParseStaThroughputSet(CWProtocolMessage *msgPtr, int len, StaThroughputSetValues *valPtr);
CWBool CWParseCollectApThroughputInfoSet (CWProtocolMessage *msgPtr, int len, ApThroughputInfoValues *valPtr);        //pei add 0204
CWBool CWParseMaxThroughput (CWProtocolMessage *msgPtr, int len, ApMaxThroughputValues *valPtr);        //pei add 0204
CWBool CWParseCmdFromAC (CWProtocolMessage *msgPtr, int len, CWProtocolConfigurationUpdateRequestValues *valPtr);        //pei add 0204
CWBool CWParseInterfaceUpdownTime (CWProtocolMessage *msgPtr, int len, ExtraInfoValues *valPtr);        //pei add 0224
CWBool CWParseMonitorSet (CWProtocolMessage *msgPtr, int len, ApMonitorSetValues *valPtr);        //pei add 0226
CWBool CWParseGetStaInfo (CWProtocolMessage *msgPtr, int len, GetStaInfoValues *valPtr);        //pei add 0417
CWBool CWParseGetIfState (CWProtocolMessage *msgPtr, int len, GetIfStateValues *valPtr);        //pei add 0422
CWBool CWParseGetAttackDetectInfo (CWProtocolMessage *msgPtr, int len, GetAttackDetectInfoValues *valPtr);        //pei add 0622
CWBool CWParseGetStaWapiInfo (CWProtocolMessage *msgPtr, int len, GetStaWapiInfoValues *valPtr);
CWBool CWParseGetNtpInfo (CWProtocolMessage *msgPtr, int len, GetNtpValues *valPtr);
CWBool CWParseGetTerminalDisturbInfo (CWProtocolMessage *msgPtr, int len, GetTerminalDisturbValues *valPtr);
CWBool CWSetApMaxThroughput(ApMaxThroughputValues *apMaxThroughput,unsigned char radioId);  //pei add 0209
CWBool CWWTPSetModeNPara(WTPModeNParaValues *wtpModeNParaValues);
CWBool acw_set_eth_config_mtu(set_eth_mtu_vlaues *set_eth_values);
CWBool acw_set_time_config(set_all_time_vlaues *set_time_values);
CWBool Txpowerget(CWProtocolResultCode *resultCode,unsigned char radio_id, char *wlanname,int type);//gzm add 0929
CWBool Channelget(CWProtocolResultCode *resultCode,unsigned char radio_id, char *wlanname);
CWBool CWParseGetUserandPasswdinfo (CWProtocolMessage *msgPtr, int len, WtpUserPasswdValues *valPtr);

//si trova in CWProtocol.h
//CWBool CWParseACName(CWProtocolMessage *msgPtr, int len, char **valPtr);						// 4

//---------------------------------------------------------/
void CWWTPResetRebootStatistics(WTPRebootStatisticsInfo *rebootStatistics);

int CWWTPGetDiscoveryType(void);
int CWWTPGetMaxRadios(void);
int CWWTPGetRadiosInUse(void);
int CWWTPGetEncCapabilities(void);
CWBool CWWTPGetBoardData(CWWTPVendorInfos *valPtr);
CWBool CWWTPGetVendorInfos(CWWTPVendorInfos *valPtr);
int CWWTPGetMACType(void);
char *CWWTPGetLocation(void);
int CWWTPGetSessionID(void);
int CWWTPGetIPv4Address(void);
int CWWTPGetIPv4StatusDuplicate(void);
int CWWTPGetIPv6StatusDuplicate(void);
char *CWWTPGetName(void);
CWBool CWWTPGetRadiosInformation(CWRadiosInformation *valPtr);
int CWWTPGetACIndex();
char* CWWTPGetACName();
int CWWTPGetFrameTunnelMode();
CWBool CWGetWTPRadiosOperationalState(int radioID, CWRadiosOperationalInfo *valPtr);
CWBool CWAssembleMsgElemDecryptErrorReport(CWProtocolMessage *msgPtr, int radioID);
CWBool CWAssembleMsgElemDuplicateIPv6Address(CWProtocolMessage *msgPtr);

//---------------------------------------------------------/
void CWWTPDestroyVendorInfos(CWWTPVendorInfos *valPtr);
CWBool CWSetRadioAdminState(AdminStateValues *adminStateValues, unsigned char *operat_state_cause);

CWBool CWParseGetMultiUserPerformance (CWProtocolMessage *msgPtr, int len, GetMultiUserPerformanceValues *valPtr);
CWBool CWParseGetMcsMask (CWProtocolMessage *msgPtr, int len, GetMcsMaskValues *valPtr);
CWBool CWParseGetStaPktsStatisticRpt(CWProtocolMessage *msgPtr, int len, GetStaPktsStatisticRptValues *valPtr);



#endif
