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

#ifndef __CAPWAP_CWBinding_HEADER__
#define __CAPWAP_CWBinding_HEADER__

#define CW_BINDING_HLEN				4
#define CW_BINDING_WIRELESSID			1
#define CW_BINDING_DATALENGTH			6
#define CW_BINDING_MACLENGTH			6

#define NUM_QOS_PROFILES			4
#define UNUSED_QOS_VALUE			255

#define VOICE_QUEUE_INDEX			0
#define VIDEO_QUEUE_INDEX			1
#define BESTEFFORT_QUEUE_INDEX			2
#define BACKGROUND_QUEUE_INDEX			3

#define BINDING_MIN_ELEM_TYPE			1024
#define BINDING_MAX_ELEM_TYPE			2047

// Wireless ID viene preso dal campo WBID
//#define CW_TRANSPORT_HEADER_WIRELESS_ID_START	0
//#define CW_TRANSPORT_HEADER_WIRELESS_ID_LEN	8

//#define CW_TRANSPORT_HEADER_LENGTH_START	8
#define CW_TRANSPORT_HEADER_LENGTH_START	0
#define CW_TRANSPORT_HEADER_LENGTH_LEN		8

//#define CW_TRANSPORT_HEADER_RSSI_START		16
#define CW_TRANSPORT_HEADER_RSSI_START		8
#define CW_TRANSPORT_HEADER_RSSI_LEN		8

//#define CW_TRANSPORT_HEADER_SNR_START		24
#define CW_TRANSPORT_HEADER_SNR_START		16
#define CW_TRANSPORT_HEADER_SNR_LEN		8

// Poiche' nel draft 09 il campo del CAPWAP header Wireless Specific 
// Information e' stato privato del sottocampo Wireless ID con il
// conseguente shift a sx di 8 bit dei sottocampi successivi il sottocampo
// datarate del binding si trova a cavallo tra 2 word da 4 byte quindi
// vanno specificati due offset.
//#define CW_TRANSPORT_HEADER_DATARATE_START	0
//#define CW_TRANSPORT_HEADER_DATARATE_LEN	16

#define CW_TRANSPORT_HEADER_DATARATE_1_START	24
#define CW_TRANSPORT_HEADER_DATARATE_1_LEN	8

#define CW_TRANSPORT_HEADER_DATARATE_2_START	0
#define CW_TRANSPORT_HEADER_DATARATE_2_LEN	8

#define CW_TRANSPORT_HEADER_MAC_1_START	8
#define CW_TRANSPORT_HEADER_MAC_2_START	16
#define CW_TRANSPORT_HEADER_MAC_3_START	24
#define CW_TRANSPORT_HEADER_MAC_4_START	0
#define CW_TRANSPORT_HEADER_MAC_5_START	8
#define CW_TRANSPORT_HEADER_MAC_6_START	16
#define CW_TRANSPORT_HEADER_MAC_ELE_LEN	8
//#define CW_TRANSPORT_HEADER_PADDING_START	16
//#define CW_TRANSPORT_HEADER_PADDING_LEN		16
#define CW_TRANSPORT_HEADER_PADDING_START	8
#define CW_TRANSPORT_HEADER_PADDING_LEN		24


#define BINDING_MSG_ELEMENT_TYPE_ADD_WLAN   1024
#define BINDING_MSG_ELEMENT_TYPE_ASSIGNED_WTP_BSSID 1026
#define BINDING_MSG_ELEMENT_TYPE_DELETE_WLAN		1027
#define BINDING_MSG_ELEMENT_TYPE_SET_CHAN	1028
#define	BINDING_MSG_ELEMENT_TYPE_DIRECT_SEQUEUE_CONTROL	1028
#define BINDING_MSG_ELEMENT_TYPE_IEEE80211_INFO_ELEMENT 1029

#define BINDING_MSG_ELEMENT_TYPE_IEEE80211_MAC_OPERATE 1030//weiay 20080722
#define BINDING_MSG_ELEMENT_TYPE_IEEE80211_RATE_SET 1034
#define BINDING_MSG_ELEMENT_TYPE_STATION_SESSION_KEY	1038
#define BINDING_MSG_ELEMENT_TYPE_SET_TXP	1041
#define	BINDING_MSG_ELEMENT_TYPE_TX_POWER	1041
#define BINDING_MSG_ELEMENT_TYPE_WTP_QOS	1045
#define BINDING_MSG_ELEMENT_TYPE_IEEE80211_WTP_RADIO_CONFIGURATON 1046 //weiay 20080722

#define BINDING_MSG_ELEMENT_TYPE_UPDATE_WLAN 1044
#define BINDING_MSG_ELEMENT_TYPE_IEEE80211_RADIO_INFO	1048
#define	BINDING_MSG_ELEMENT_TYPE_RADIO_INFO                    1048       //pei add 0624
#define BINDING_MSG_ELEMENT_TYPE_IEEE80211_RADIO_TYPE_SET	1048 //added by weiay 20080714
#define BINDING_MSG_ELEMENT_TYPE_IEEE80211_WLAN_VLAN_INFO	1049       //pei add 0305
#define BINDING_MSG_ELEMENT_TYPE_WAPI_CER_INFO_ELEMENT	1050

#define BINDING_MSG_ELEMENT_TYPE_NEIGHBORDEAD_INTERVAL	1051

#define BINDING_MSG_ELEMENT_TYPE_WTP_RADIO_SET	2012
#define BINDING_MSG_ELEMENT_TYPE_WTP_RADIO_REPORT 2013
#define BINDING_MSG_ELEMENT_TYPE_CHANGE_WLAN_TUNNEL_MODE 2014

typedef struct
{
	unsigned char queueDepth;
	int cwMin;
	int cwMax;
	unsigned char  AIFS;
	unsigned char dot1PTag;
	unsigned char DSCPTag;	
} WTPQosValues;
typedef struct
{
	unsigned char radio_id;
	unsigned char wlan_id;
	unsigned short capabilities;
	unsigned char key_index;
	unsigned char key_status;	
	unsigned short key_length;
	char *key;/*legth 32*/	
	char *group_tsc;/*length 6*/	
	unsigned char qos;	
	unsigned char auth_type;	
	unsigned char mac_mode;	     
	unsigned char tunnel_mode; 
	unsigned char suppress_ssid;	
    char *ssid;	/*length 32*/
}AddWlanValues;

typedef struct{
	unsigned char radio_id;
	unsigned char wlan_id;
	unsigned char mac_mode;
	unsigned char tunnel_mode; 
}ChangeTunnelMode;

typedef struct{
 	unsigned char elem_id;  //221
 	unsigned char len;
 	unsigned char oui[3];
 	unsigned char oui_type;
 	unsigned char version[2]; /* little endian */
 	int proto;
 	int pairwise_cipher;     //alg
 	int group_cipher;
 	int key_mgmt;            //auth mode
 	int capabilities;
 	unsigned int num_pmkid;
 	//const unsigned char *pmkid;
 	int mgmt_group_cipher;
}WPA_IE;
typedef struct {                    //peiwenhui 0606
	unsigned char radio_id;
	unsigned char wlan_id;
	unsigned char flags;
	WPA_IE *wtp_ie;
}Ieee80211InfoEleValues;

typedef struct{
	unsigned char radio_id;
	unsigned char mac_length;
	unsigned char *mac_addr;
	unsigned char wlan_id;
	
}AddSTAValues;
typedef struct{
	unsigned char radio_id;
	unsigned char mac_length;
	unsigned char *mac_addr;
	unsigned char wlan_id;  //pei add 0306
}DeleteSTAValues;
typedef struct
{
	WTPQosValues* qosValues;
} bindingValues;

typedef struct
{
	unsigned char radioID;
	unsigned char channel;
	
} BindingChan;

typedef struct
{	
	unsigned char radioID;
	unsigned short TXP;
	
} BindingTXP;
//added by weiay
typedef struct
{	
	unsigned char radioID;
	unsigned short rate;
	unsigned int ratemask;
	
} BindingRate;

typedef struct
{	
	unsigned char radioID;
	unsigned int radiotype;
	
} BindingRadioType;
//added end
typedef struct
{	
	unsigned char RadioID;
	unsigned char Reserved;
	unsigned short RTSThreshold;
	unsigned char Shortretry;
	unsigned char Longretry;
	unsigned short FragThreshold;
	unsigned int TxMSDULifetime;
	unsigned int RxMSDULifetime;
	
} BindingRadioOperate;

typedef struct
{	
	unsigned char RadioID;
	unsigned char IsShortPreamble;
	unsigned char BSSIDnums;
	unsigned char DTIMPeriod;
	char BSSID[6];
	unsigned short BeaconPeriod;
	unsigned int CountryCode;
	
} BindingRadioConfiguration;

struct Support_Rate_List{
	int Rate;
	struct  Support_Rate_List *next;
};

typedef struct
{
	unsigned char radio_id;
	unsigned char wlan_id;
}DeleteWlanValues;
typedef struct 
{
	unsigned char radio_id;
	unsigned char wlan_id;
	unsigned short capabilities;
	unsigned char key_index;
	unsigned char key_status;	
	unsigned short key_length;
	char *key;/*legth 32*/	
	
}UpdateWlanValues;
typedef struct
{
	unsigned char *mac;/*6 byte*/
	unsigned short int flags;
	unsigned char *Pairwise_TSC;/*6 byte*/
	unsigned char *Pairwise_RSC;/*6 byte*/
	unsigned char *key;
		
}STASessionKeyValues;


/*---------------------------*/

typedef struct {
	unsigned char type; /*0-802.3, 1-802.11*/
	
	unsigned char flag_w;
	char RSSI;
	char SNR;
	unsigned short dataRate;

	unsigned char flag_m;
	unsigned char bssid[6];
	unsigned char radioId;
} CWBindingTransportHeaderValues;

typedef struct {
	CWProtocolMessage* frame;
	CWBindingTransportHeaderValues* bindingValues;
} CWBindingDataListElement;

typedef struct {
	unsigned char radio_id;
	unsigned char wlan_id;
	unsigned short pr_switch;
} ProbeRequestInhibition;

typedef struct {
	unsigned char radio_id;
	unsigned char wlan_id;
	unsigned short uni_muti_cast;
	unsigned int rate_value;
} CastIsolation_CastRate;

extern const int gMaxCAPWAPHeaderSizeBinding;

CWBool CWAssembleDataMessage(CWProtocolMessage **completeMsgPtr, int *fragmentsNumPtr, int PMTU, CWProtocolMessage *frame, CWBindingTransportHeaderValues *bindingValuesPtr, int is_crypted);
CWBool CWAssembleTransportHeaderBinding(CWProtocolMessage *transportHdrPtr, CWBindingTransportHeaderValues *valuesPtr);
CWBool CWBindingCheckType(int elemType);
CWBool CWParseTransportHeaderBinding(CWProtocolMessage *msgPtr, CWBindingTransportHeaderValues *valuesPtr);
CWBool CWAssembleAssignedWTPBssid(CWProtocolMessage *bindPtr, char *bssid,int crete_wlan_id,int radio_id);

#endif
