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


#ifndef __CAPWAP_WTPBinding_HEADER__
#define __CAPWAP_WTPBinding_HEADER__

typedef enum{
	DHCP_SNOOPING = 1,
	STA_INFO_REPORT = 2,
	AMPDU_OP = 3,
	PUREN_MIXED_OP = 4,
	CHANNEL_EXTOFFSET =5,
	TX_CHAINMASK = 6,
	AMSDU_OP = 7,/*yuan add ...*/
	RX_CHAINMASK =8,/*yuan add ...*/
	ACK_TIMEOUT = 9
}WTP_RADIO_SET;

typedef enum{
	WTP = 1,
	RADIO = 2,
	STA = 3,
	KEY = 4,
	WLAN = 5
}OP_TYPE;
typedef struct {
	unsigned char radioID;
	WTPQosValues qosValues[NUM_QOS_PROFILES];
} RadioQosValues;
typedef struct
{
	unsigned char WMMObject;
	unsigned char QosOrder;
	unsigned short Length;
	unsigned char	QueueDepth;
	unsigned short CWMin;
	unsigned short CWMax;
	unsigned char	AIFS;
	unsigned short TXOPlimit;
	unsigned char	Dot1PTag;
	unsigned char	DSCPTag;
	unsigned char	ACK;
	unsigned char	mapstate;
	unsigned char	wmm_map_dot1p;
	unsigned char	dot1p_map_wmm_num;
	unsigned char	dot1p_map_wmm[8];
} WTP_QosValues;
typedef struct {
	unsigned char radioID;
	WTP_QosValues qosValues[NUM_QOS_PROFILES*2];
} Radio_QosValues;
typedef struct {
	unsigned char radio_id;
	unsigned char reserved;
	unsigned char current_channel;
	unsigned char currend_CCA;
	unsigned int energy_detect_threshold;
}DirectSequeueControlValues;
typedef struct{
	unsigned char radio_id;
	unsigned char reserved;
	unsigned short int current_power;

}TxPowerValues;

typedef struct {
	unsigned char radio_id;
	unsigned int radio_type;
}WTPRadioInfoValues;
typedef struct{
	unsigned char radio_id;
	unsigned char reserved;
	unsigned short rts_threshold;
	unsigned char short_retry;
	unsigned char long_retry;
	unsigned short fragmentation_threshold;
	unsigned int tx_msdu_lifetime;
	unsigned int rx_msdu_lifetime;
}MacOperationValues;
typedef struct{
	unsigned char radio_id;
	unsigned char is_short_preamble;
	unsigned char num_of_bssids;
	unsigned char dtim_period;
	unsigned char bssid[6];
	unsigned short beacon_period;
	unsigned char country_code[4];
}WTPRadioConfigurationValues;
typedef struct {
	unsigned char radio_id;
	unsigned char admin_state;
}AdminStateValues;

typedef struct {
	unsigned char op_type;
	unsigned char radio_id;
	unsigned char wlan_id;
	unsigned char state;
}dhcp_snooping_type;

typedef struct {
	unsigned char op_type;
	unsigned char radio_id;
	unsigned char wlan_id;
	int state;
	unsigned char current_mode;
}channel_extoffset_op_type;

typedef struct {
	unsigned char op_type;
	unsigned char radio_id;
	unsigned char wlan_id;
	unsigned char state;
}tx_chainmask_op_type ;

typedef struct {
	unsigned char op_type;
	unsigned char radio_id;
	unsigned char wlan_id;
	unsigned char state;
}rx_chainmask_op_type /*yuan add ...*/;

typedef struct {
	unsigned char op_type;
	unsigned char radio_id;
	unsigned char state;
	unsigned int  distance;
}ack_timeout_op_type;

typedef struct {
	unsigned char op_type;
	unsigned char radio_id;
	unsigned char wlan_id;
	unsigned char state;
}sta_info_repport_type;

typedef struct {
	unsigned char op_type;
	unsigned char radio_id;
	unsigned char wlan_id;
	unsigned char state;
	unsigned int ampdulimit;
	unsigned int ampduframes;/*yuan add ...*/
}ampdu_op_type;

typedef struct {
	unsigned char op_type;
	unsigned char radio_id;
	unsigned char wlan_id;
	unsigned char state;
	unsigned int amsdulimit;
	unsigned int amsduframes;
}amsdu_op_type;/*yuan add ...*/

typedef struct {
	unsigned char op_type;
	unsigned char radio_id;
	unsigned char wlan_id;
	unsigned char state;
}puren_mixed_op_type;

typedef struct {
	unsigned char static_IP_enable;
	unsigned int IP_addr;
	unsigned int netmask;
	unsigned int gateway;
	unsigned int firstdns;
	unsigned int secdns;
} WTPStaticIPValues;

typedef struct {
	unsigned char radio_id;
	unsigned short guard_interval;
	unsigned short mcs;
	unsigned short cwmmode;
} WTPModeNParaValues;

typedef struct {
	unsigned char radioId;
	unsigned char wlanId;
	unsigned short valueOf8021qTypeId;
	unsigned char userPriority;
	unsigned char canonicalFormat;
	unsigned short vlanId;
} WlanVlanValues;

typedef struct {
	unsigned short cerAsType;
	unsigned short cerAsLen;
	char *cerAsValue;
	unsigned short cerAeType;
	unsigned short cerAeLen;
	char *cerAeValue;
	unsigned short asType;
	unsigned short asLen;
	char *asValue;
} WapiValues;


typedef  struct {
	unsigned short  *rate;
	int count;
}CWConfigureStateRate;

typedef struct {
	int qosCount;
	Radio_QosValues *radioQosValues;
	int directSequeueControlCount;
	DirectSequeueControlValues *directSequeueControl;
	int txPowerCount;
	TxPowerValues *txPower;
	int rateCount;
	BindingRate *rateSetValues;
	int radioTypeCount;
	BindingRadioType *radioTypeSetValues;
	int macOperationCount;
	MacOperationValues *macOperation;
	int radioConfigCount;
	WTPRadioConfigurationValues *radioConfiguration;
} CWBindingConfigurationRequestValues;

typedef struct {
	int neighborDeadInterval;
	int qosCount;
	int directSequeueControlCount;
	int txPowerCount;
	int rateCount;
	int radioTypeCount;
	int macOperationCount;
	int radioConfigCount;
	int adminStateCount;
	int dhcp_snooping_count;
	int sta_info_repport_count;
	int ampdu_op_count;
	int amsdu_op_count;/*yuan add ...*/
	int puren_mixed_op_count;
	int channel_extoffset_op_count;
	int tx_chainmask_op_count;
	int rx_chainmask_op_count;
	int ack_timeout_op_count;
	Radio_QosValues *radioQosValues;
	DirectSequeueControlValues *directSequeueControlValues;
	TxPowerValues *txPowerValues;
	BindingRate *rateSetValues;
	BindingRadioType *radioTypeSetValues;
	MacOperationValues *macOperation;
	WTPRadioConfigurationValues *radioConfiguration;
	AdminStateValues *adminState;
	dhcp_snooping_type *dhcp_snooping_values;
	sta_info_repport_type *sta_info_repport_values;
	ampdu_op_type *ampdu_op_values;
	amsdu_op_type *amsdu_op_values;/*yuan add ...*/
	puren_mixed_op_type *puren_mixed_op_values;
	channel_extoffset_op_type *channel_extoffset_op_values;
	tx_chainmask_op_type *tx_chainmask_op_values;
	rx_chainmask_op_type *rx_chainmask_op_values;
	ack_timeout_op_type *ack_timeout_op_values;
} CWBindingConfigurationUpdateRequestValues;
typedef struct{
	int addwlancount;
	int infoelecount;
	int deleteWlanCout;
	int updateWlanCount;
	int wlanVlanCount;        //pei 0305
	int wapiCount;        //pei 0309
	int ChangeTunnelModeCount;
	AddWlanValues *addWlanValues;
	Ieee80211InfoEleValues *ieee80211_InfoEleValues;         //pei 0604
	DeleteWlanValues *deleteWlanValues;
	UpdateWlanValues *updateWlanValues;
	WlanVlanValues *wlanVlanValues;        //pei 0305
	WapiValues *wapiValues;          //pei 0309
	ChangeTunnelMode *changeWlanTunnelMode;
}CWBINDINGIEEE80211WlanconfigurationRequestValues;
typedef struct{
	int addstacount;
	int deletestacount;
	int stationsessionkeycount;
	int infoelecount;
	AddSTAValues *addSTAValues;
	DeleteSTAValues *deleteSTAValues;
	STASessionKeyValues *staSessionKeyValues;
	Ieee80211InfoEleValues *ieee80211_InfoEleValues;         //pei 0605
}CWBINDINGSTAConfigurationRequestValues;

typedef enum
{
	CONF_WTPD,
	CONF_RADIO,
	CONF_WLAN
}Conf_Type;

typedef struct
{
	int radioID;
	int wlanID;
	char conf_arg[128];
	char conf_value[64];
}ConfArg;
//CWBool CWSaveConfigInfo(Conf_Type,ConfArg,CWBool);
CWBool CWSaveConfigInfo(Conf_Type conf_type,ConfArg *confArg,CWBool chkdir);

CWBool CWWTPInitBinding(int radioIndex);
CWBool CWBindingSaveConfigureResponse(void* bindingValuesPtr, CWProtocolResultCode* resultCode);
CWBool CWBindingSetQosValues(int qosCount, RadioQosValues *radioQosValues, CWProtocolResultCode *resultCode);
CWBool CWBindingSetQosValues1(int qosCount, Radio_QosValues *radioQosValues, CWProtocolResultCode *resultCode);    //pei 0207
CWBool CWBindingParseConfigurationUpdateRequest (char *msg, int len, void **valuesPtr);
CWBool CWBindingParseConfigureResponse (char *msg, int len, void **valuesPtr);
CWBool CWBindingSaveConfigurationUpdateRequest(void* bindingValuesPtr, CWProtocolResultCode* resultCode);
CWBool CWBindingParseIEEE80211WlanConfigurationRequest(char *msg, int len, void **valuesPtr);
CWBool CWBindingSaveIEEE80211WlanConfigurationRequest(void* bindingValuesPtr, unsigned char *created_radio_id, unsigned char* created_wlan_id, CWProtocolResultCode* resultCode);
CWBool CWBindingParseChangeWlanTunnelMode(CWProtocolMessage *msgPtr, int len, ChangeTunnelMode*valPtr);
CWBool CWBindingSaveChangeWlanTunnelMode(int ChangeTunnelModeCount,ChangeTunnelMode *changeTunnelMode,CWProtocolResultCode *resultCode);
CWBool CWBindingParseSTAConfigurationRequest(char *msg, int len, void **valuesPtr);   //pei add 0716
CWBool CWBindingSaveSTAConfigurationRequest(void* bindingValuesPtr, CWProtocolResultCode* resultCode); //pei add 0716
CWBool CWParseDeleteSta(CWProtocolMessage *msgPtr, int len ,DeleteSTAValues *valPtr);      //pei add 0716
CWBool CWGetRateMask(unsigned short *rate, int count,int radioid);
CWBool CWParseRateSet2(CWProtocolMessage *msgPtr, int len ,BindingRate *valPtr);	
CWBool CWParseRateSet(CWProtocolMessage *msgPtr, int len ,BindingRate *valPtr);				//pei add 0714
CWBool CWParseRadioTypeSet(CWProtocolMessage *msgPtr, int len ,BindingRadioType *valPtr);	//pei add 0714			
CWBool CWParseMacOperation(CWProtocolMessage *msgPtr, int len, MacOperationValues *valPtr);	//pei add 0722
CWBool CWParseWTPRadioConfiguration(CWProtocolMessage *msgPtr, int len ,WTPRadioConfigurationValues *valPtr);	//pei add 0722
CWBool CWParseNeighborDeadInterval(CWProtocolMessage *msgPtr, int len, CWBindingConfigurationUpdateRequestValues *valPtr);	//pei add 0714
CWBool acw_parse_ampdu_configuration(CWProtocolMessage *msgPtr, int len ,ampdu_op_type *valPtr);
CWBool acw_parse_amsdu_configuration(CWProtocolMessage *msgPtr, int len ,amsdu_op_type *valPtr);//yuan add
CWBool acw_parse_puren_mixed_op_configuration(CWProtocolMessage *msgPtr, int len ,puren_mixed_op_type *valPtr);
CWBool acw_dhcp_snooping_configuration(CWProtocolMessage *msgPtr, int len ,dhcp_snooping_type *valPtr);
CWBool acw_sta_info_report_configuration(CWProtocolMessage *msgPtr, int len ,sta_info_repport_type *valPtr);
CWBool acw_parse_channel_extoffset_configuration(CWProtocolMessage *msgPtr, int len ,channel_extoffset_op_type*valPtr);
CWBool acw_parse_tx_chainmask_configuration(CWProtocolMessage *msgPtr, int len ,tx_chainmask_op_type*valPtr);
CWBool acw_parse_rx_chainmask_configuration(CWProtocolMessage *msgPtr, int len ,rx_chainmask_op_type*valPtr);
CWBool acw_parse_ack_timeout_configuration(CWProtocolMessage *msgPtr, int len ,ack_timeout_op_type*valPtr);
CWBool GetWlanId(DeleteSTAValues *deleteSTAValues, unsigned char *wlan_id);

#endif
