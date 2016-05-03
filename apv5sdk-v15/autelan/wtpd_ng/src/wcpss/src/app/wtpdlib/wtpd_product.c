#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "wtpd_product.h"


extern int debug_print;
extern int gWlanIDMap[16];
extern char * trim(char *str_org);
extern __inline__ void CWWTPDebugLog(const char *format, ...);


int CWGetWlanIdxInMap(int * map, int radio_id, int wlan_id)
{
	return wlan_id;// no convert add by diaowq@20120726
	int i, found = 0, loop_low = radio_id * 8, loop_high = (radio_id + 1) * 8;
	for( i = loop_low ; i < loop_high; i++){
		if(map[i] == wlan_id){
			CWWTPDebugLog("%s,get index of ath.%d-%d is %d",__func__, radio_id, wlan_id, i);
			found = 1;
			break;
		}
	}
	return (found == 1 ? i : 0);
}

int CWCreateLocalWlanID(int radio_id, int remote_wlan_id)
{
	return remote_wlan_id;// no convert add by diaowq@20120726
	int wlan_idx = -1;
	CWWTPDebugLog("%s,radio_id:%d,original remote wlan_id:%d",__func__,radio_id,remote_wlan_id);
	wlan_idx = CWGetWlanIdxInMap(gWlanIDMap, radio_id, -1);
	gWlanIDMap[wlan_idx] = remote_wlan_id;
	CWWTPDebugLog("%s,radio_id:%d,valid local wlan_id:%d",__func__,radio_id,wlan_idx);

	return wlan_idx;
}

int CWGetLocalWlanID(int radio_id, int remote_wlan_id)
{
	return remote_wlan_id;// no convert add by diaowq@20120726
	int wlan_idx = -1;
	CWWTPDebugLog("%s,radio_id:%d,original remote wlan_id:%d",__func__,radio_id,remote_wlan_id);
	wlan_idx = CWGetWlanIdxInMap(gWlanIDMap, radio_id, remote_wlan_id);
	CWWTPDebugLog("%s,radio_id:%d,valid local wlan_id:%d",__func__,radio_id,wlan_idx);

	return wlan_idx;
}

int CWGetRemoteWlanID(int radio_id, int local_wlan_id)
{
	return local_wlan_id;// no convert add by diaowq@20120726
	int remote_wlan_id = -1;
	CWWTPDebugLog("%s,radio_id:%d,original local wlan_id:%d",__func__,radio_id,local_wlan_id);
	remote_wlan_id = gWlanIDMap[radio_id*8+local_wlan_id];
	CWWTPDebugLog("%s,radio_id:%d,valid remote wlan_id:%d",__func__,radio_id,remote_wlan_id);

	return remote_wlan_id == -1?local_wlan_id:remote_wlan_id;
}

CWBool CWGetChannel(int * channel, char *wlanname)
{
	char str[24];
	char str_tmp[24];
	char temp_str[256];

	memset(str_tmp,0,24);
	memset(temp_str, 0, 256);
	sprintf(temp_str,"iwlist %s channel |awk -F \"[()]\" '/Current/{print $2}' |awk -F \" \" '{print $2}'",wlanname);
#ifdef MARVELL_PLATFORM
    sleep(10);
#else
    sleep(5);
#endif
    PopenFile(temp_str,str_tmp, sizeof(str_tmp));
	memset(str, 0, 24);
	strcpy(str, trim(str_tmp));
	*channel=atoi(str);

	return CW_TRUE;
}
CWBool CWGetHostIP(char *hostip)
{
	char cmd_str[128];
	char str[24];
	char str_tmp[24];

	memset(cmd_str, 0, 128);
	sprintf(cmd_str, "ifconfig default | awk -F \" \" '/inet addr/ {print $2}' | awk -F \":\" '{print $2}'");
	PopenFile(cmd_str,str_tmp, sizeof(str_tmp));
	memset(str, 0, 24);
	strcpy(str, trim(str_tmp));		

	strcpy(hostip, str);
	CWWTPDebugLog("hostip: %s len:%d", hostip, (int)strlen(hostip));

	return CW_TRUE;
}

CWBool CWGetHostGateWay(char *gw)
{
	char str[64];
	char temp_str[256];

	memset(str,0,64);
	memset(temp_str, 0, 256);
	sprintf(temp_str, "route -n | awk -F \" \" '/UG/ {print $2}'");
	PopenFile(temp_str,str, sizeof(str));
	
	if(debug_print)
		printf("Last Connect Gateway: %s  len:%d\n", str, (int)strlen(str));

	strcpy(gw,str);
	return CW_TRUE;
}
CWBool CWSetWmmEnable(char * wlanname,int enable)
{
	char cmdbuf[128];
	memset(cmdbuf,0,128);
	sprintf(cmdbuf, "iwpriv %s wmm %d",wlanname,enable);
	return _system(cmdbuf);
}
CWBool CWSetCwmin(char * wlanname, unsigned char QosOrder,unsigned char WMMObject,unsigned short CWMin)
{
	char cmdbuf[128];
	memset(cmdbuf,0,128);
	sprintf(cmdbuf, "iwpriv %s cwmin %.*d %.*d %.*hd",wlanname,sizeof(QosOrder),QosOrder,sizeof(WMMObject),WMMObject,sizeof(CWMin),CWMin);
	return _system(cmdbuf);
}

CWBool CWSetAifs(char *wlanname, unsigned char QosOrder,unsigned char WMMObject,unsigned char aifs)
{
	char cmdbuf[128];
	memset(cmdbuf,0,128);
	sprintf(cmdbuf, "iwpriv %s aifs %.*d %.*d %.*d",wlanname,sizeof(QosOrder),QosOrder,sizeof(WMMObject),WMMObject,sizeof(aifs),aifs);
	return _system(cmdbuf);
}
CWBool CWSetTxoplimit(char * wlanname, unsigned char QosOrder,unsigned char WMMObject,unsigned short Txoplimit)
{
	char cmdbuf[128];
	memset(cmdbuf,0,128);
	sprintf(cmdbuf, "iwpriv %s txoplimit %.*d %.*d %.*hd",wlanname,sizeof(QosOrder),QosOrder,sizeof(WMMObject),WMMObject,sizeof(Txoplimit),Txoplimit);
	return _system(cmdbuf);
}
CWBool CWSetAckPolicy(char *wlanname, unsigned char QosOrder,unsigned char AckPolicy)
{
	char cmdbuf[128];
	memset(cmdbuf,0,128);
	sprintf(cmdbuf, "iwpriv %s noackpolicy %.*d 0 %.*d",wlanname,sizeof(QosOrder),QosOrder,sizeof(AckPolicy),AckPolicy);
	return _system(cmdbuf);
}
CWBool CWSetlp_flagEnable(char *wlanname,int enable)
{
	char cmdbuf[128];
	memset(cmdbuf,0,128);
	sprintf(cmdbuf, "autelan wmm 1p_flag %.*s %.*d",sizeof(wlanname),wlanname,sizeof(enable),enable);
	return _system(cmdbuf);
}
CWBool CWSet1p_ingress_map(char * wlanname,unsigned char QosOrder,unsigned char wmm_map_dot1p)
{
	char cmdbuf[128];
	memset(cmdbuf,0,128);
	sprintf(cmdbuf, "autelan wmm 1p_ingress_map %.*s %.*d %.*d",sizeof(wlanname),wlanname,sizeof(QosOrder),QosOrder,sizeof(wmm_map_dot1p),wmm_map_dot1p);
	return _system(cmdbuf);
}
CWBool CWSet1p_egress_map(char * wlanname,unsigned char QosOrder,unsigned char dot1p_map_wmm)
{	
	char cmdbuf[128];
	memset(cmdbuf,0,128);
	sprintf(cmdbuf, "autelan wmm 1p_egress_map %.*s %.*d %.*d",sizeof(wlanname),wlanname,sizeof(QosOrder),QosOrder,sizeof(dot1p_map_wmm),dot1p_map_wmm);
	return _system(cmdbuf);
}
CWBool CWSetTrafficLimitEnable(char *wlanname,unsigned char enable)
{
	char cmdbuf[128];
	memset(cmdbuf,0,128);
	sprintf(cmdbuf, "autelan traffic_limit %s set_vap_flag %d",wlanname,enable);
	return _system(cmdbuf);
}
CWBool CWSetTrafficLimitThreshold(char *wlanname,unsigned char threshold)
{
	char cmdbuf[128];
	memset(cmdbuf,0,128);
	sprintf(cmdbuf, "autelan traffic_limit %s set_vap %d",wlanname,threshold);
	return _system(cmdbuf);
}


CWBool CWSetInterfaceUp(char *ifname,int status)
{
	//return CW_TRUE;
	char cmdbuf[128];
	memset(cmdbuf,0,128);
	if(status == 1){
		sprintf(cmdbuf, "ifconfig %s up > /dev/null",ifname);
        
#ifdef MARVELL_PLATFORM
		_system(cmdbuf);
        memset(cmdbuf,0,128);
		sprintf(cmdbuf, "iwconfig %s commit > /dev/null",ifname);
#endif
        
	}
	else
		sprintf(cmdbuf,"ifconfig %s down > /dev/null",ifname);
	return _system(cmdbuf);
}
CWBool CWShell(char *cmd)
{
	return _system(cmd);
}
CWBool CWCreateWlanInterface(char *wlanname,int radio_id)
{
	char cmdbuf[128];
	memset(cmdbuf,0,128);
	sprintf(cmdbuf,"wlanconfig %s create wlandev wifi%d wlanmode ap >/dev/null",wlanname, radio_id);
	return _system(cmdbuf);
	return CW_TRUE;
}
CWBool CWDestroyWlanInterface(char *wlanname)
{
	char cmdbuf[128];
	memset(cmdbuf,0,128);
	sprintf(cmdbuf,"wlanconfig %s destroy",wlanname);
	return _system(cmdbuf);
}
CWBool CWSetQuickRoamingEnable(char *wlanname,int enable)
{
	char cmdbuf[128];
	memset(cmdbuf,0,128);
	sprintf(cmdbuf,"autelan quick_roaming %s set_flag %d",wlanname,enable);
	return _system(cmdbuf);
}
CWBool CWSetShortgi(char *wlanname,unsigned short guardinterval)
{
	char cmdbuf[128];
	memset(cmdbuf,0,128);
	sprintf(cmdbuf,"iwpriv %s shortgi %hd",wlanname,guardinterval);
	return _system(cmdbuf);
}
CWBool CWSetAuthmode(char *wlanname,int automode)
{
	char cmdbuf[128];
	memset(cmdbuf,0,128);
	sprintf(cmdbuf,"iwpriv %s authmode %d",wlanname,automode);
	return _system(cmdbuf);
}
CWBool CWSetOpenNone(char *wlanname)
{
	char cmdbuf[128];
	memset(cmdbuf,0,128);
	sprintf(cmdbuf,"iwconfig %s key off",wlanname);
	return _system(cmdbuf);
}

CWBool CWSetWepKey(char *wlanname,int isShareMode,unsigned char key_index,char *key,unsigned short key_length)
{
	char cmdbuf[128];
	memset(cmdbuf,0,128);
	char mode[16];
	if(isShareMode != 0)
		strcpy(mode,"restricted");
	else
		strcpy(mode,"open");
	if((key_length == 5)||(key_length == 13)||(key_length == 16))
		sprintf(cmdbuf,"iwconfig %s key [%d] s:%s %s", wlanname,key_index,key,mode); 
	else if((key_length == 10)||(key_length == 26)||(key_length == 32))
		sprintf(cmdbuf,"iwconfig %s key [%d] %s %s", wlanname,key_index,key,mode); 
	return _system(cmdbuf);
}
CWBool CWSetPrivacyEnable(char *wlanname,char enable)
{
	char cmdbuf[128];
	memset(cmdbuf,0,128);
	sprintf(cmdbuf,"iwpriv %s privacy %d",wlanname,enable);
	return _system(cmdbuf);
}

CWBool CWSetEssid(char *wlanname,char *essid)
{
	char cmdbuf[128];
	memset(cmdbuf,0,128);
	sprintf(cmdbuf,"iwconfig %s essid \"%s\"",wlanname,essid);
	return _system(cmdbuf);
}
CWBool CWSetPuregEnable(char *wlanname,int enable)
{
	char cmdbuf[128];
	memset(cmdbuf,0,128);
	sprintf(cmdbuf,"iwpriv %s pureg %d",wlanname,enable);
	return _system(cmdbuf);
}
CWBool CWSetRadioType(int radio_id, int wlan_id, int radiotype)
{
	/*
		radiotype as follows from AC (and atheros)
		+-+-+-+-+-+-+-+-+
			    gn an  n  g   a   b
		+-+-+-+-+-+-+-+-+
		
		but,marvell like this
		+-+-+-+-+-+-+-+-+
				         a   n  g   b
		+-+-+-+-+-+-+-+-+
			5=802.11g/n (unused)
			7=802.11b/g/n(default)

		so,we need to transfer.
	*/
	int new_radiotype = 0;
    
	if(radiotype & IEEE80211_PARAM_MODE_11b) new_radiotype |= 0x01;	
//	if(radiotype & 0x02) new_radiotype |= 0x0f;//delete by wangxl 20121221
    if(radiotype & IEEE80211_PARAM_MODE_11a) new_radiotype |= 0x08; //add by wangxl 20121221
	if(radiotype & IEEE80211_PARAM_MODE_11g) new_radiotype |= 0x02;
	if(radiotype & IEEE80211_PARAM_MODE_11n) new_radiotype |= 0x04;
	if(radiotype & IEEE80211_PARAM_MODE_11an) new_radiotype |= 0x0c;
//	if(radiotype & 0x20) new_radiotype |= 0x05;//delete by wangxl 20121221
//  if(radiotype & 0x02) new_radiotype |= 0x0f;//add by wangxl 20121221
    if(radiotype & IEEE80211_PARAM_MODE_11gn) new_radiotype |= 0x06;
    
	char cmdbuf[128];
	memset(cmdbuf,0,128);
	sprintf(cmdbuf,"iwpriv wifi%d opmode %d",radio_id,new_radiotype);
	return _system(cmdbuf);

}
CWBool CWSetChannel(int radio_id, int wlan_id, int channel)
{
	char cmdbuf[128];
	memset(cmdbuf,0,128);
	sprintf(cmdbuf,"iwconfig wifi%d channel %d",radio_id,channel);
	return _system(cmdbuf);
}
CWBool CWSetMCSMaskAndCap(int radio_id, int mask, int capability)
{
	char cmdbuf[128];
	memset(cmdbuf, 0, 128);
	sprintf(cmdbuf,"iwpriv wifi%d set_mcs_mask 0x%x", radio_id, mask); 
	_system(cmdbuf);
	
	memset(cmdbuf, 0, 128);
	sprintf(cmdbuf,"iwpriv wifi%d set_mcs_cap 0x%x", radio_id, capability);
	_system(cmdbuf);
	return CW_TRUE;
}
CWBool CWSetRate(int radio_id, int wlan_id, int radiotype,int ratemask)
{
	char cmdbuf[128];
	memset(cmdbuf,0,128);
	sprintf(cmdbuf,"iwpriv wifi%d fixrate %d",radio_id,0);
//	return _system(cmdbuf);//so first,wait the interface below be transplated. 
	_system(cmdbuf);
	if(radiotype & IEEE80211_PARAM_MODE_11g )
	{
		sprintf(cmdbuf,"iwpriv wifi%d set_rate_mask_g 0x%x",radio_id,ratemask);
	}
	else if(radiotype & IEEE80211_PARAM_MODE_11b)
	{
		sprintf(cmdbuf,"iwpriv wifi%d set_rate_mask_b 0x%x",radio_id,ratemask);
	}
	else if (radiotype & IEEE80211_PARAM_MODE_11a)
	{
		sprintf(cmdbuf,"iwpriv wifi%d set_rate_mask_a 0x%x",radio_id,ratemask);
	}
	_system(cmdbuf);

	memset(cmdbuf,0,128);
	sprintf(cmdbuf,"iwpriv wifi%d  set_rate_cap  0x80",radio_id/*,ratemask*/);
	_system(cmdbuf);
	return CW_TRUE;
}

CWBool CWSetFragmentThreshold(char *wlanname,unsigned short  threshold)
{
	char cmdbuf[128];
	memset(cmdbuf,0,128);
	sprintf(cmdbuf,"iwconfig %s frag %d",wlanname,threshold);
	return _system(cmdbuf);
}
CWBool CWSetTxpower(int radio_id, int wlan_id, unsigned char txtype, int txpower)
{
	char cmdbuf[128];
	
	memset(cmdbuf, 0, 128);
	sprintf(cmdbuf,"iwpriv wifi%d txpower %d",radio_id,txpower);

	return _system(cmdbuf);
}
CWBool CWSetRtsThreshold(int radio_id, int wlan_id,unsigned short rtsThreshold)
{
	char cmdbuf[100];
	memset(cmdbuf,0,100);
	sprintf(cmdbuf,"iwconfig wifi%d rts %d",radio_id, rtsThreshold);
	return _system(cmdbuf);
}
CWBool CWSetBeaconInterval(int radio_id, int wlan_id,unsigned short beaconInterval)
{
	char cmdbuf[100];
	memset(cmdbuf,0,100);
	sprintf(cmdbuf,"iwpriv wifi%d bcninterval %hd",radio_id, beaconInterval);
	return _system(cmdbuf);
}
CWBool CWSetDtim(char *wlanname,unsigned char dtim)
{
	char cmdbuf[100];
	memset(cmdbuf,0,100);
	sprintf(cmdbuf,"iwpriv %s dtim %d",wlanname, dtim);
	return _system(cmdbuf);
}
CWBool CWSetPreamble(char *wlanname,unsigned char preamble)
{
	char cmdbuf[100];
	memset(cmdbuf,0,100);
	sprintf(cmdbuf,"iwpriv %s preamble %d",wlanname, preamble);
	return _system(cmdbuf);
}
CWBool CWSetExtoffset(char *wlanname,char extoffset)
{
	char cmdbuf[100];
	memset(cmdbuf,0,100);
	sprintf(cmdbuf,"iwpriv %s extoffset %d",wlanname, extoffset);
	return _system(cmdbuf);
}
/*CWBool CWSetAr11nMode(char *wlanname,char *mode,char mcs,unsigned short isShortgi)
{
	char cmdbuf[100];
	memset(cmdbuf,0,100);
	sprintf(cmdbuf,"/usr/sbin/ar11nmode %.*s %.*s %.*d %.*d",sizeof(wlanname),wlanname,sizeof(mode),mode,sizeof(mcs),mcs,sizeof(isShortgi),isShortgi);
	return _system(cmdbuf);
}*/
CWBool CWSetTx_ChainMask(char *wlanname,unsigned char state)
{
	char cmdbuf[128];
	memset(cmdbuf,0,128);
	sprintf(cmdbuf,"iwpriv %s tx_chainmask %d",wlanname,state);
	return _system(cmdbuf);
}
CWBool CWSetRx_ChainMask(char *wlanname,unsigned char state)
{
	char cmdbuf[128];
	memset(cmdbuf,0,128);
	sprintf(cmdbuf,"iwpriv %s rx_chainmask %d",wlanname,state);
	return _system(cmdbuf);
}
CWBool CWSetTxChainMask(char *wlanname,unsigned char state)
{
	char cmdbuf[128];
	memset(cmdbuf,0,128);
	sprintf(cmdbuf,"iwpriv %s txchainmask %d",wlanname,state);
	return _system(cmdbuf);
}
CWBool CWSetRxChainMask(char *wlanname,unsigned char state)
{
	char cmdbuf[128];
	memset(cmdbuf,0,128);
	sprintf(cmdbuf,"iwpriv %s rxchainmask %d",wlanname,state);
	return _system(cmdbuf);
}
CWBool CWSetAmpdu(char *wlanname ,unsigned char state ,int ampdu_limit,int ampdu_frames)
{
	char cmdbuf[128];
	memset(cmdbuf,0,128);
	sprintf(cmdbuf,"iwpriv %s ampdu %d > /dev/null", wlanname,state);
        system(cmdbuf);
	memset(cmdbuf,0,128);
        sprintf(cmdbuf,"iwpriv %s ampdulimit %d > /dev/null", wlanname,ampdu_limit); 
	system(cmdbuf);
	memset(cmdbuf,0,128);
        sprintf(cmdbuf,"iwpriv %s ampdusframes %d > /dev/null", wlanname,ampdu_frames); 
	system(cmdbuf);
	return 1;
}
CWBool CWSetWifiAmpdu(char *wlanname,int state,int apmdu_limit,int ampdu_frames)
{
	char cmdbuf[128];
	memset(cmdbuf,0,128);
	sprintf(cmdbuf,"iwpriv %s AMPDU %d > /dev/null", wlanname,state);
        system(cmdbuf);
	memset(cmdbuf,0,128);
        sprintf(cmdbuf,"iwpriv %s AMPDULim %d > /dev/null", wlanname,apmdu_limit); 
	system(cmdbuf);
	memset(cmdbuf,0,128);
        sprintf(cmdbuf,"iwpriv %s AMPDUFrames %d > /dev/null", wlanname,ampdu_frames); 
	system(cmdbuf);
	return 1;
}
CWBool CWSetAmsdu(char *wlanname ,unsigned char state ,int amsdu_limit)
{
	char cmdbuf[128];
	memset(cmdbuf,0,128);
	sprintf(cmdbuf,"iwpriv %s amsdu %d > /dev/null", wlanname,state);
       	system(cmdbuf);
	memset(cmdbuf,0,128);
       	sprintf(cmdbuf,"iwpriv %s amsdulimit %d > /dev/null", wlanname,amsdu_limit); 
	system(cmdbuf);
	return 1;
}
CWBool CWSetWifiAmsdu(char *wlanname ,unsigned char state ,int amsdu_limit)
{
	char cmdbuf[128];
	sprintf(cmdbuf,"iwpriv %s AMSDU %d > /dev/null", wlanname,state);
       	system(cmdbuf);
       	sprintf(cmdbuf,"iwpriv %s AMSDULim %d > /dev/null", wlanname,amsdu_limit); 
	system(cmdbuf);
       	return 1;
}
CWBool CWSetForBiasAuto(int radio_id,char enable)
{
	char cmdbuf[128];
	memset(cmdbuf,0,128);
	sprintf(cmdbuf,"iwpriv wifi%d ForBiasAuto %d",radio_id,enable);
	return _system(cmdbuf);
}
CWBool CWSetPuren(char *wlanname,int state)
{
	char cmdbuf[128];
	memset(cmdbuf,0,128);
	sprintf(cmdbuf,"iwpriv %s puren %d",wlanname,state);
	return _system(cmdbuf);
}
CWBool CWSetHideSsid(char *wlanname,unsigned char hide_ssid)
{
	char cmdbuf[128];
	memset(cmdbuf,0,128);
	sprintf(cmdbuf,"iwpriv %s hidessid %d",wlanname,hide_ssid);
	return _system(cmdbuf);
}
CWBool CWSetWidFloodAttackCnt(char *wlanname,unsigned char gFloodAttackCnt)
{
	char cmdbuf[128];
	memset(cmdbuf,0,128);
	sprintf(cmdbuf,"iwpriv %s attack_cnt %d",wlanname,gFloodAttackCnt);
	return _system(cmdbuf);
}
CWBool CWSetWidFloodProbeCnt(char *wlanname,unsigned char gFloodProbeCnt)
{
	char cmdbuf[128];
	memset(cmdbuf,0,128);
	sprintf(cmdbuf,"iwpriv %s prb_atk_cnt %d",wlanname,gFloodProbeCnt);
	return _system(cmdbuf);
}
CWBool CWSetWidFloodInterval(char *wlanname,unsigned char gFloodWidsInterval)
{
	char cmdbuf[128];
	memset(cmdbuf,0,128);
	sprintf(cmdbuf,"iwpriv %s wids_intval %d",wlanname,gFloodWidsInterval);
	return _system(cmdbuf);
}
CWBool CWSetWidSpoofDetectEnable(char *wlanname,int enable)
{
	char cmdbuf[128];
	memset(cmdbuf,0,128);
	sprintf(cmdbuf,"iwpriv %s spoof_detect %d",wlanname,enable);
	return _system(cmdbuf);
}
CWBool CWSetWidFloodDetectEnable(char *wlanname,int enable)
{
	char cmdbuf[128];
	memset(cmdbuf,0,128);
	sprintf(cmdbuf,"iwpriv %s flood_detect %d",wlanname,enable);
	return _system(cmdbuf);
}
CWBool CWSetWidWeakIVDetectEnable(char *wlanname,int enable)
{
	char cmdbuf[128];
	memset(cmdbuf,0,128);
	sprintf(cmdbuf,"iwpriv %s wk_iv_detect %d",wlanname,enable);
	return _system(cmdbuf);
}
CWBool CWDelWapiset(char *wlanname)
{
	char cmdbuf[128];
	memset(cmdbuf,0,128);
	sprintf(cmdbuf,"wapiset wlan %s del",wlanname);
	return _system(cmdbuf);
}


    /**** zhouke add ,for marvell iwconfig wifi commit ****/
#ifdef MARVELL_PLATFORM

CWBool CWSetWifiCommit(int radio_id)
{

    //printf("#####radio_id = %d\n",radio_id);

	char cmdbuf[128];
	memset(cmdbuf,0,128);
	sprintf(cmdbuf,"iwconfig wifi%d commit > /dev/null",0);
    _system(cmdbuf);
	usleep(1000);
	return 1;
}

#endif
    /********************** end *************************/


