#ifndef _WTPD_PRODUCT_H_
#define _WTPD_PRODUCT_H_
#include "common.h"


/*
add by diaowq@20120613 for marvell platform
use ath.x-7 as scan interface

#define _iwlist_scan_if_name_for_wifi0_	"ath.0-7"
#define _iwlist_scan_if_name_for_wifi1_	"ath.1-7"
#define _wtpd_scan_if_name_for_wifi0_	"ath.0-7"
#define _wtpd_scan_if_name_for_wifi1_	"ath.1-7"
*/
#define _iwlist_scan_if_name_for_wifi0_	"ath.0-7"
#define _iwlist_scan_if_name_for_wifi1_	"ath.1-7"
#define _wtpd_scan_if_name_for_wifi0_	"ath.0-7"
#define _wtpd_scan_if_name_for_wifi1_	"ath.1-7"


extern CWBool PopenFile (char *cmd_str,char *str,int len );

CWBool CWGetChannel(int * channel, char *wlanname);
CWBool CWGetHostIP(char *hostip);
CWBool CWGetHostGateWay(char *gw);

//radio configuration
CWBool CWSetRadioType(int radio_id, int wlan_id, int radiotype);




//QoS configuration
CWBool CWSetWmmEnable(char * wlanname,int enable);
CWBool CWSetCwmin(char * wlanname, unsigned char QosOrder,unsigned char WMMObject,unsigned short CWMin);
CWBool CWSetCwmax(char *wlanname, unsigned char QosOrder,unsigned char WMMObject,unsigned short CWMax);
CWBool CWSetAifs(char *wlanname, unsigned char QosOrder,unsigned char WMMObject,unsigned char aifs);
CWBool CWSetTxoplimit(char *wlanname, unsigned char QosOrder,unsigned char WMMObject,unsigned short Txoplimit);
CWBool CWSetAckPolicy(char *wlanname, unsigned char QosOrder,unsigned char AckPolicy);
CWBool CWSetlp_flagEnable(char *wlanname,int enable);
CWBool CWSet1p_ingress_map(char * wlanname,unsigned char QosOrder,unsigned char wmm_map_dot1p);
CWBool CWSet1p_egress_map(char * wlanname,unsigned char QosOrder,unsigned char dot1p_map_wmm);
CWBool CWSetTrafficLimitEnable(char *wlanname,unsigned char enable);
CWBool CWSetTrafficLimitThreshold(char *wlanname,unsigned char threshold);


//wlan configuration
CWBool CWSetInterfaceUp(char *ifname,int status);
CWBool CWShell(char *cmd);
CWBool CWCreateWlanInterface(char *wlanname,int radio_id);
CWBool CWDestroyWlanInterface(char *wlanname);
CWBool CWSetQuickRoamingEnable(char *wlanname,int enable);
CWBool CWSetShortgi(char *wlanname,unsigned short guardinterval);
CWBool CWSetAuthmode(char *wlanname,int automode);
CWBool CWSetWepKey(char *wlanname,int isShareMode,unsigned char key_index,char *key,unsigned short key_length);
CWBool CWSetPrivacyEnable(char *wlanname,char enable);
CWBool CWSetOpenNone(char *wlanname);



//set wireless parameters of vap
CWBool CWSetEssid(char *wlanname,char *essid);
CWBool CWSetPuregEnable(char *wlanname,int enable);
CWBool CWSetChannel(int radio_id, int wlan_id, int channel);
CWBool CWSetFragmentThreshold(char *wlanname,unsigned short  threshold);
CWBool CWSetMCSMaskAndCap(int radio_id, int mask, int capability);
CWBool CWSetRate(int radio_id, int wlan_id, int radio_type,int ratemask);
CWBool CWSetTxpower(int radio_id, int wlan_id, unsigned char txtype, int txpower);
CWBool CWSetRtsThreshold(int radio_id, int wlan_id,unsigned short rtsThreshold);
CWBool CWSetBeaconInterval(int radio_id, int wlan_id,unsigned short beaconInterval);
CWBool CWSetDtim(char *wlanname,unsigned char dtim);
CWBool CWSetPreamble(char *wlanname,unsigned char preamble);
CWBool CWSetExtoffset(char *wlanname,char extoffset);
//CWBool CWSetAr11nMode(char *wlanname,char *mode,char mcs,unsigned short isShortgi);
CWBool CWSetTx_ChainMask(char *wlanname,unsigned char state);
CWBool CWSetRx_ChainMask(char *wlanname,unsigned char state);
CWBool CWSetTxChainMask(char *wlanname,unsigned char state);
CWBool CWSetRxChainMask(char *wlanname,unsigned char state);
CWBool CWSetAmpdu(char *wlanname ,unsigned char state ,int ampdu_limit,int ampdu_frames);
CWBool CWSetWifiAmpdu(char *wlanname,int state,int apmdu_limit,int ampdu_frames);
CWBool CWSetAmsdu(char *wlanname ,unsigned char state ,int amsdu_limit);
CWBool CWSetWifiAmsdu(char *wlanname ,unsigned char state ,int amsdu_limit);
CWBool CWSetForBiasAuto(int radio_id,char enable);
CWBool CWSetPuren(char *wlanname,int state);
CWBool CWSetHideSsid(char *wlanname,unsigned char hide_ssid);

//
CWBool CWDelWapiset(char *wlanname);

//wid configuration
CWBool CWSetWidFloodAttackCnt(char *wlanname,unsigned char gFloodAttackCnt);
CWBool CWSetWidFloodProbeCnt(char *wlanname,unsigned char gFloodProbeCnt);
CWBool CWSetWidFloodInterval(char *wlanname,unsigned char gFloodWidsInterval);
CWBool CWSetWidSpoofDetectEnable(char *wlanname,int enable);
CWBool CWSetWidFloodDetectEnable(char *wlanname,int enable);
CWBool CWSetWidWeakIVDetectEnable(char *wlanname,int enable);


//vlan configuration



//other
int CWCreateLocalWlanID(int radio_id, int wlan_id);

    /**** zhouke add ,for marvell iwconfig wifi commit ****/
#ifdef MARVELL_PLATFORM

CWBool CWSetWifiCommit(int radio_id);

#endif
    /********************** end *************************/

#endif
