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
#include <err.h>
#include "WTPDriverInteraction.h"
#include "CWWTP.h"

#ifdef DMALLOC
#include "../dmalloc-5.5.0/dmalloc.h"
#endif
#define IOCTL 1
#define BIT(x) (1 << (x))         //pei 0605
CWBool isWPAfirstKey;
extern wlan_stats_info gwlanStatsInfo[NUM_OF_ATH_ALL_WIFI];
extern wlan_stats_info gethStatsInfo[NUM_OF_ETH];
unsigned char gApMonitorEnable = 0;   //pei add 0226
unsigned char gApMonitorInterval = 20;   //pei add 0226
unsigned char gExtraInfoEnable = 0;  //pei add 0224
unsigned short gExtraInfoInterval = 20;   //pei add 0226
unsigned char gStaInfoEnable = 0;  //pei add 0224
unsigned short gStaInfoInterval = 30;   //pei add 0226
unsigned char gIfStateEnable = 0;  //pei add 0224
unsigned short gIfStateInterval = 20;   //pei add 0226
unsigned char gStaWapiInfoEnable = 0;
unsigned short gStaWapiInfoInterval = 30;
unsigned char gFloodingDetectEnable = 0;
unsigned char gSpoofingDetectEnable = 0;
unsigned char gWeakIVDetectEnable = 0;
unsigned char gFloodAttackCnt = 30;
unsigned char gFloodProbeCnt = 30;
unsigned char gFloodWidsIntval = 1;
unsigned char gTerminalDisturbEnable = 0;
unsigned char gDhcpsnoopingReportEnable = 0;
unsigned char gSetScanWifiRadioType = 0; //sundaolian add ror athscanwifi1
unsigned char gStaPktsStatisticEnable = 0;	//diaowq add@20120413,for sta packets statistic report
unsigned short gStaPktsStatisticInterval = 50;
int gWlanIDMap[16] = {-1};
static int get_eth_fields(char *ifname, eth_package_info *ethPackageInfo);
int getifstats(const char *ifname, unsigned long *iframes, unsigned long *oframes);
extern void CWCaptrue(int n ,unsigned char *buffer);
extern CWBool CWWTPManageGenericRunMessage(CWProtocolMessage *msgPtr);
extern CWBool CWGetInterfaceCount(int *count, char *type);
extern unsigned int BitReverse32(unsigned int aNum);

/****************************************************************
 *
 *
 *
 * ************************************************************/
int Check_Interface_State(char *ifname)
{
	int sockfd;
	struct ifreq ifr;
	
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    /**** zhouke add ,for marvell ioctl ****/
#ifdef MARVELL_PLATFORM
    memset(&ifr, 0, sizeof(struct ifreq));
#endif
    /************ end ************/
	strncpy(ifr.ifr_name,ifname, sizeof(ifr.ifr_name));	
	
	if(ioctl(sockfd, SIOCGIFINDEX, &ifr) == -1){//bind to a interface 
		CWDebugLog("SIOCGIFINDEX error\n");
		CWWTPDebugLog("SIOCGIFINDEX error, %s is not exist !", ifname);
		close(sockfd);
		return -1;
	 }
/*		
	if (ioctl(sockfd, SIOCGIFFLAGS, &ifr) == -1){
		printf("SIOCGIFFLAGS error\n");
		close(sockfd);
		return -1;
	}
	
	if(ifr.ifr_flags & IFF_UP){
		//printf("interface UP\n");
	}else{
		//printf("interface DOWN\n");
		close(sockfd);
		return -1;
	}
*/
	CWWTPDebugLog("%s is already exist !", ifname);
	close(sockfd);
	return 0;
}
/*pei add for wapid ioctl. 090505*/
/*向driver发送消息,set key ,set wapi */
int  wapid_ioctl(char *ifname, unsigned short cmd, char *buf, int buf_len)
{
	int result = 0;
	int sock;
	struct iwreq  wreq;

	sock = socket(AF_INET, SOCK_DGRAM, 0);
	memset(&wreq, 0, sizeof(struct iwreq));
	strcpy(wreq.ifr_ifrn.ifrn_name, ifname);
	if (buf_len < IFNAMSIZ) {
		memcpy(wreq.u.name, buf, buf_len);
	} else {
		wreq.u.data.pointer = (caddr_t)buf;
		wreq.u.data.length =  buf_len;
	}

	CWWTPDebugLog("cmd: %x", cmd);
	result = ioctl(sock, cmd, &wreq);
	
	if (result < 0) 
	{
		fprintf(stderr, "in %s:%d return(%d) :  \"%s\"........... \n", __func__, __LINE__, (result),strerror(errno));
	}
	close(sock);
	
	return result;
}
/*************************wlanconfig*******************************/
/*------------------------addwlan--------------------------------*/

int create_wlan(int sock, AddWlanValues *addWlanValues, WPA_IE *wpa_ie, WlanVlanValues *wlanVlanValues, WapiValues *wapiValues,int sameradio)
{
	char cmdbuf[100];
	char wlanname[IFNAMSIZ];
	char radioname[IFNAMSIZ];
	int wpa_enable =0;
	unsigned int ratemask = 0;
	int tunnel_mode =  0; //0-local bridge;    1-802.11 tunnel;   2-802.3 tunnel(for future use)
	int rid = addWlanValues->radio_id, wid = addWlanValues->wlan_id;

	CWWTPGetWifiName(radioname,addWlanValues->radio_id);
	CWDebugLog("IN wlan create");
	CWWTPDebugLog("IN wlan create");

	
	CWDebugLog("createwlan->wlan_localid %d",addWlanValues->wlan_id);
	CWWTPGetWlanName(wlanname,addWlanValues->radio_id,addWlanValues->wlan_id);

//	if(Check_Interface_State(wlanname) == 0) //pei add for no recreate wlan
//		return 1;
	

	CWCreateWlanInterface(wlanname, addWlanValues->radio_id);
	if((addWlanValues->tunnel_mode == CW_802_DOT_11_TUNNEL))//||(addWlanValues->tunnel_mode == CW_802_IPIP_TUNNEL))
	{
		CWDebugLog("splitmac\n");
		CWWTPDebugLog("splitmac");
		dpf("CW_802_DOT_11_TUNNEL-split mac\n");
		tunnel_mode = 1;
		CWSetWlanTunnelMode(wlanname,tunnel_mode);
		/* pei add fast-forward for splitmac 081115 */
		CWEnableIpForward(CW_ENABLE);
#ifndef CW_NO_DTLS
		if((gDtlsSecurity == 1)&&(gDtlsPolicy == 1))
		{
			CWSetSpfastDown(CW_DISABLE);			
			CWSetSpfastUp(CW_DISABLE);			
		}
		else
#endif
		{
			CWSetSpfastDown(CW_ENABLE);			
			CWSetSpfastUp(CW_ENABLE);			
		}
	}
	else if (addWlanValues->tunnel_mode == CW_802_DOT_3_TUNNEL)
	{
		dpf("CW_802_DOT_3_TUNNEL-split mac\n");
		tunnel_mode = 2;
		CWSetWlanTunnelMode(wlanname,tunnel_mode);
		
		CWEnableIpForward(CW_ENABLE);
		
		CWSetSpfastDown(CW_ENABLE);			
		CWSetSpfastUp(CW_ENABLE);			
	}
	else if(addWlanValues->tunnel_mode == CW_802_IPIP_TUNNEL)
	{
		CWWTPDebugLog("IP in IP");
		CWEnableIPinIP(wlanname,CW_ENABLE);
	}
	else
		printf("local !802.11tunnel && !802.3tunnel\n");
	isWPAfirstKey = CW_TRUE;
	/* set ssid */
	CWSetEssid(wlanname,addWlanValues->ssid);
	CWDebugLog("wtpcreatewlan->essid:%s",addWlanValues->ssid);
	/* set security type */
	char cmd[64] = {0};
	sprintf(cmd,"iwconfig ath.%d-%d key off",rid,wid);
	_system(cmd);
	if( (wpa_ie->key_mgmt == WTP_WPA_KEY_MGMT_PSK) || (wpa_ie->key_mgmt == WTP_WPA2_KEY_MGMT_PSK))  /*WPA mode*/
	{
		
		if(wpa_ie->key_mgmt == WTP_WPA_KEY_MGMT_PSK){
			CWDebugLog("wpa 1 psk mode");
			CWWTPDebugLog("wpa 1 psk mode");
			wpa_enable = 1;
		}
		else if (wpa_ie->key_mgmt == WTP_WPA2_KEY_MGMT_PSK){
			CWDebugLog("wpa 2 psk mode");
			CWWTPDebugLog("wpa 2 psk mode");
			wpa_enable =2;
			wpa_ie->key_mgmt = WTP_WPA_KEY_MGMT_PSK;/*the same as wpa 1*/
		}
		madwifi_set_ieee8021x(sock, wpa_ie, addWlanValues->radio_id,addWlanValues->wlan_id,wpa_enable);	
	}
	else if(wpa_ie->key_mgmt == WTP_WPA_KEY_MGMT_IEEE8021X_NO_WPA)  /*ieee802.1x mode*/
	{
		CWWTPDebugLog("802.1x 3 mode");
		CWSetAuthmode(wlanname,3);

		/*xmeng add for 802.1x privacy*/
		memset(cmdbuf, 0x00, sizeof(cmdbuf));
		CWSetPrivacyEnable(wlanname,1);
	}
	/*csc 080716*/
	else if((wpa_ie->key_mgmt == WTP_WPA_KEY_MGMT_IEEE8021X) || (wpa_ie->key_mgmt == WTP_WPA2_KEY_MGMT_IEEE8021X))  /*ieee802.1x  +wpa mode*/
	{
		if(wpa_ie->key_mgmt == WTP_WPA_KEY_MGMT_IEEE8021X){
			CWDebugLog("wpa 1 802.1x mode");
			CWWTPDebugLog("wpa 1 802.1x mode");
			wpa_enable =1;
		}
		else if (wpa_ie->key_mgmt == WTP_WPA2_KEY_MGMT_IEEE8021X){
			CWDebugLog("wpa 2 802.1x mode");
			CWWTPDebugLog("wpa 2 802.1x mode");
			wpa_enable =2;
			wpa_ie->key_mgmt = WTP_WPA_KEY_MGMT_IEEE8021X;
		}
		madwifi_set_ieee8021x(sock, wpa_ie, addWlanValues->radio_id,addWlanValues->wlan_id,wpa_enable);	
	}
	else if(wpa_ie->key_mgmt == WTP_WAPI_KEY_MGMT_PSK){  //pei add 090309
		CWWTPDebugLog("wapi-psk mode");
		int res = 0;
		int param[2];
		param[0] = P80211_PACKET_WAPIFLAG;
		param[1] = 0x07;
		CWWTPDebugLog("sizeof(struct ioctl_drv):%u", sizeof(struct ioctl_drv));
		res=wapid_ioctl(wlanname, P80211_IOCTL_SETWAPI_INFO, (char *)&param, 2*sizeof(int));
	}
	else if(wpa_ie->key_mgmt == WTP_WAPI_KEY_MGMT_CER){  //pei add 090309
		CWWTPDebugLog("wapi-cer mode");
		
	/*	struct ioctl_drv ioctl_drv_data;  */
		int res = 0;
		int param[2];
		param[0] = P80211_PACKET_WAPIFLAG;
		param[1] = 0x0b;
		res=wapid_ioctl(wlanname, P80211_IOCTL_SETWAPI_INFO, (char *)&param, 2*sizeof(int));
	}
	else if(wpa_ie->key_mgmt == WTP_WPA_KEY_MGMT_SHARED){
		CWWTPDebugLog("shared-wep mode");
		CWWTPDebugLog("key2 length:%d key2:%s", addWlanValues->key_length, addWlanValues->key);
		CWWTPDebugLog("key_index : %d", addWlanValues->key_index);
		if(addWlanValues->key_length>0)
		{
			CWSetWepKey( wlanname,1,addWlanValues->key_index,addWlanValues->key,addWlanValues->key_length);
			CWSetAuthmode(wlanname,2);/*authmode shared key*/
			sprintf(cmd,"iwconfig ath.%d-%d key on",rid,wid);
			_system(cmd);
		}
	}
	else{/*open mode*/
		CWWTPDebugLog("key2 length:%d key2:%s", addWlanValues->key_length, addWlanValues->key);
		CWWTPDebugLog("key_index : %d", addWlanValues->key_index);
		if(addWlanValues->key_length>0)
		{
			CWSetWepKey(wlanname,0,addWlanValues->key_index,addWlanValues->key,addWlanValues->key_length);
			CWWTPDebugLog("open-wep mode");
			sprintf(cmd,"iwconfig ath.%d-%d key on",rid,wid);
			_system(cmd);
		}
		else{
			CWSetOpenNone(wlanname);
			CWWTPDebugLog("open mode");
		}
	}
	if(wpa_ie->key_mgmt == WTP_WPA_KEY_MGMT_PSK||wpa_ie->key_mgmt == WTP_WPA2_KEY_MGMT_PSK ||
		wpa_ie->key_mgmt == WTP_WPA_KEY_MGMT_IEEE8021X || wpa_ie->key_mgmt == WTP_WPA2_KEY_MGMT_IEEE8021X )  /*WPA mode*/
	{
		madwifi_set_privacy(sock,addWlanValues->radio_id, addWlanValues->wlan_id, 1);
	}

 	/* set radiotype */
	dpf("set radio  mode in create_wlan !\n"); 
 	CWSetRadioType(addWlanValues->radio_id, addWlanValues->wlan_id,gRadioInfoValue[addWlanValues->radio_id].radiotype);

	if( gModeNParaValue[addWlanValues->radio_id].cwmMode > 1) 
			sprintf(cmdbuf, "iwpriv %s disablecoext 1 > /dev/null", wlanname);
	else
			sprintf(cmdbuf, "iwpriv %s disablecoext 0 > /dev/null", wlanname);		
	_system(cmdbuf);

	 /* config some radio parameter when create the first wlan on the radio */
 	if(sameradio==0)
	{
		CWSetBeaconInterval(rid,wid, gRadioInfoValue[rid].beaconInterval);       //set the beacon-interval
		CWSetPreamble(wlanname, gRadioInfoValue[rid].preamble);         //set the preamble
		
		/* set wmm */
		CWSetWmmEnable(wlanname,gQosEnable); 
		/*gzm add for fist set txpower must up down interface*/	
		CWSetInterfaceUp(wlanname,1);
		CWSetInterfaceUp(wlanname,0);
		/*delete up down interface because channel scanning cannot work nomoal firsttime and without it txpower can work nomoal*/
		
		CWWTPDebugLog("txtype:%d  txpower:%d", gRadioInfoValue[addWlanValues->radio_id].txtype, gRadioInfoValue[addWlanValues->radio_id].txpower);
		CWSetTxpower(rid,wid,gRadioInfoValue[rid].txtype,gRadioInfoValue[addWlanValues->radio_id].txpower);
 	}
	
	/* set channel */
	if(0==gRadioInfoValue[addWlanValues->radio_id].channel){
		gRadioInfoValue[addWlanValues->radio_id].channel=1;
		if(gRadioInfoValue[addWlanValues->radio_id].radiotype&IEEE80211_PARAM_MODE_11a){
			gRadioInfoValue[addWlanValues->radio_id].channel=149;
			if(-1==gModeNParaValue[addWlanValues->radio_id].extoffset)
				gRadioInfoValue[addWlanValues->radio_id].channel=153;
			if(-1!=gModeNParaValue[addWlanValues->radio_id].extoffset)
				gRadioInfoValue[addWlanValues->radio_id].channel=149;
		}
	}
	CWSetChannel(rid,wid, gRadioInfoValue[addWlanValues->radio_id].channel);

	/*setting rate*/
	ratemask = gRadioInfoValue[addWlanValues->radio_id].ratemask;
	if(ratemask!= 0)
		CWSetRate(rid,wid,gRadioInfoValue[rid].radiotype,ratemask);

	CWSetHideSsid(wlanname, addWlanValues->suppress_ssid);         //set the hide-ssid
	CWSetFragmentThreshold(wlanname, gRadioInfoValue[addWlanValues->radio_id].fragThreshold);         //set the fragment-threshold
	CWSetRtsThreshold(rid, wid, gRadioInfoValue[addWlanValues->radio_id].rtsThreshold);         //set the rts-threshold

	CWSetDtim(wlanname, gRadioInfoValue[addWlanValues->radio_id].dtim);       	//set the dtim-period
	
	if((addWlanValues->tunnel_mode != CW_802_DOT_11_TUNNEL) &&(addWlanValues->tunnel_mode != CW_802_DOT_3_TUNNEL)){
		CWWTPDebugLog("bridge mode");
		int i=0;
		int eth_nu=0;	

#if (defined( AQ2000N )|| defined(XHPB44)||defined(XJAP93V1))
		eth_nu=1;
#endif
    
		if(wlanVlanValues->vlanId!= 0)
		{
			sprintf(cmdbuf, "br%d", wlanVlanValues->vlanId);
			if(Check_Interface_State(cmdbuf)!=0)
			{
				sprintf(cmdbuf, "brctl addbr br%d", wlanVlanValues->vlanId);
				_system(cmdbuf);
				sprintf(cmdbuf, "/sbin/ifconfig br%d 0.0.0.0 up", wlanVlanValues->vlanId);
				_system(cmdbuf);
			}
			sprintf(cmdbuf, "eth%d.%d", eth_nu,wlanVlanValues->vlanId);
			if(Check_Interface_State(cmdbuf)!=0)
			{
				sprintf(cmdbuf, "vconfig add eth%d %d",eth_nu, wlanVlanValues->vlanId);
				_system(cmdbuf);
				sprintf(cmdbuf, "brctl addif br%d eth%d.%d", wlanVlanValues->vlanId,eth_nu, wlanVlanValues->vlanId);
				_system(cmdbuf);
				sprintf(cmdbuf, "/sbin/ifconfig eth%d.%d up",eth_nu, wlanVlanValues->vlanId);
				_system(cmdbuf);
				sprintf(cmdbuf, "/sbin/ifconfig eth%d mtu 1504",eth_nu);
				_system(cmdbuf);
			}
#ifdef CAVIUM_PLATFORM
			int ethCount = 1;
			CWGetInterfaceCount(&ethCount, "eth[0-9]");
			if(ethCount == 2)
			{
				sprintf(cmdbuf, "eth1.%d", wlanVlanValues->vlanId);
				if(Check_Interface_State(cmdbuf)!=0)
				{
					sprintf(cmdbuf, "vconfig add eth1 %d", wlanVlanValues->vlanId);
					_system(cmdbuf);
					sprintf(cmdbuf, "brctl addif br%d eth1.%d", wlanVlanValues->vlanId, wlanVlanValues->vlanId);
					_system(cmdbuf);
					sprintf(cmdbuf, "/sbin/ifconfig eth1.%d up", wlanVlanValues->vlanId);
					_system(cmdbuf);
					_system("/sbin/ifconfig eth1 mtu 1504");
				}
			}
#endif
			sprintf(cmdbuf,"brctl addif br%d %s", wlanVlanValues->vlanId, wlanname);
			_system(cmdbuf);
			if(wlanVlanValues->userPriority!=0)
			{
				sprintf(cmdbuf, "vconfig set_flag eth%d.%d 1 1",eth_nu,wlanVlanValues->vlanId);
				_system(cmdbuf);
				for(i=1;i<8;i++)
				{
					sprintf(cmdbuf, "vconfig set_egress_map eth%d.%d %d %d",eth_nu, wlanVlanValues->vlanId, i, wlanVlanValues->userPriority);
					_system(cmdbuf);
					sprintf(cmdbuf, "vconfig set_ingress_map eth%d.%d %d %d",eth_nu, wlanVlanValues->vlanId, i, wlanVlanValues->userPriority);
					_system(cmdbuf);
				}
#ifdef CAVIUM_PLATFORM
				if(ethCount == 2)
				{
					sprintf(cmdbuf, "vconfig set_flag eth1.%d 1 1", wlanVlanValues->vlanId);
					_system(cmdbuf);
					for(i=1;i<8;i++)
					{
						sprintf(cmdbuf, "vconfig set_egress_map eth1.%d %d %d", wlanVlanValues->vlanId, i, wlanVlanValues->userPriority);
						_system(cmdbuf);
						sprintf(cmdbuf, "vconfig set_ingress_map eth1.%d %d %d", wlanVlanValues->vlanId, i, wlanVlanValues->userPriority);
						_system(cmdbuf);
					}
				}
#endif
			}
		}
		else
		{

			sprintf(cmdbuf,"brctl addif default %s", wlanname);
			_system(cmdbuf);
		}
		CWDebugLog("local bridge brctl addif br-lan ath%d ",addWlanValues->wlan_id);
	}

	if(addWlanValues->capabilities == 1)  //pei add 0218
	{
		CWSetQuickRoamingEnable(wlanname,1);
	}

	/* set flood (probe) attack cnt */
	if(gFloodingDetectEnable)
	{
		CWSetWidFloodAttackCnt(wlanname, gFloodAttackCnt);
		CWSetWidFloodProbeCnt(wlanname, gFloodProbeCnt);
		CWSetWidFloodInterval(wlanname, gFloodWidsIntval*1000);
	}
	
	CWSetAntennaNone(wlanname);

	return 1;
}
int delete_wlan(DeleteWlanValues *value)
{
	struct ifreq ifr;
	char cmdbuf[100];
	memset(&ifr, 0, sizeof(ifr));
	char wlanname[IFNAMSIZ];
	CWWTPWlan *ptr = NULL;
	CWWTPWlan *oldptr = NULL;
	unsigned short vlanId = 0;
	int vlan_found = 0;
	int eth_nu=0;	
//#ifdef AQ2000N
       //eth_nu=1;
//#endif
#if (defined( AQ2000N )|| defined(XHPB44)||defined(XJAP93V1))
       eth_nu=1;
#endif

	
	CWWTPGetWlanName(wlanname, value->radio_id, value->wlan_id);
	for(ptr=wtp_wlan_list;ptr != NULL;oldptr = ptr,ptr = ptr->next)
	{
		if((ptr->radio_id == value->radio_id)&&(ptr->wlan_id == value->wlan_id)&&(ptr->vlan_id != 0))
		{
			vlanId = ptr->vlan_id;
			break;
		}
	}
	if(vlanId)
	{
		for(ptr=wtp_wlan_list;ptr != NULL;oldptr = ptr,ptr = ptr->next)
		{
			if(ptr->vlan_id == vlanId)
			{
				if((ptr->radio_id!= value->radio_id)||(ptr->wlan_id != value->wlan_id))
				{
					vlan_found = 1;
					break;
				}
			}
		}
		CWSetInterfaceUp(wlanname,0);
		sprintf(cmdbuf,"brctl delif br%d %s", vlanId, wlanname);
		system(cmdbuf);
		CWDestroyWlanInterface(wlanname);

		if(!vlan_found)
		{
			sprintf(cmdbuf,"brctl delif br%d eth%d.%d", vlanId, eth_nu, vlanId);
			_system(cmdbuf);
			sprintf(cmdbuf,"/sbin/ifconfig eth%d.%d down",eth_nu, vlanId);
			_system(cmdbuf);
			sprintf(cmdbuf, "vconfig rem eth%d.%d",eth_nu, vlanId);
			_system(cmdbuf);
#ifdef CAVIUM_PLATFORM
			int ethCount = 1;
			CWGetInterfaceCount(&ethCount, "eth[0-9]");
			if(ethCount == 2)
			{
				sprintf(cmdbuf,"brctl delif br%d eth1.%d", vlanId, vlanId);
				_system(cmdbuf);
				sprintf(cmdbuf,"/sbin/ifconfig eth1.%d down", vlanId);
				_system(cmdbuf);
				sprintf(cmdbuf, "vconfig rem eth1.%d", vlanId);
				_system(cmdbuf);
			}
#endif
			sprintf(cmdbuf,"/sbin/ifconfig br%d down", vlanId);
			_system(cmdbuf);
			sprintf(cmdbuf, "brctl delbr br%d", vlanId);
			_system(cmdbuf);
		}
	}
	else
	{
#ifdef AQ1110T
		CWSetInterfaceUp(wlanname,0);
		sprintf(cmdbuf,"brctl delif br-lan %s", wlanname);
		_system(cmdbuf);
		CWDestroyWlanInterface(wlanname);
#else
		CWSetInterfaceUp(wlanname,0);
		CWDelWapiset(wlanname);  
		sprintf(cmdbuf,"brctl delif default %s", wlanname);
		_system(cmdbuf);
		CWDestroyWlanInterface(wlanname);
#endif
	}

	if(wtp_wlan_list == NULL){
		_system("echo 0 > /proc/sys/net/capwap/spfast_down");   /* pei add 1227 */
		_system("echo 0 > /proc/sys/net/capwap/spfast_up");   /* pei add 1227 */
		CWDebugLog("turn back to local bridge mode\n");
		//sprintf(cmdbuf,"sysctl -w dev.wifi0.splitmac=0");
		//system(cmdbuf);
		isFirstWlan = CW_TRUE;
		isWPAfirstKey = CW_TRUE;
	}
	return 1;
}


/**************************** iwconfig ****************************/
/*--------------------------- Frequency ---------------------------*/
int set_freq(int sock, struct iwreq wrq, int value)
{
	wrq.u.freq.m=value;		//in Ghz/10
	wrq.u.freq.e=1;			
      	if(ioctl(sock, SIOCSIWFREQ, &wrq) < 0)
	{
		perror("Ioctl error");
		return(0);
	}		
	CWDebugLog("Frequenza impostata a: %d\n", wrq.u.freq.m);
	return 1;
}

int get_freq(int sock, struct iwreq* wrq)
{
      	if(ioctl(sock, SIOCGIWFREQ, wrq) < 0)
	{
		perror("Ioctl error");
		return(0);
	}	

	CWDebugLog("\nFrequenza: %d\n", wrq->u.freq.m);

	return 1;
}

/*--------------------------- Bit rate ---------------------------*/
int set_bitrate(int sock, struct iwreq wrq, int value)
{
	wrq.u.bitrate.value=value;
	wrq.u.bitrate.fixed=1;
	
      	if(ioctl(sock, SIOCSIWRATE, &wrq) < 0)
	{
		perror("Ioctl error");
		return(0);
	}	

	CWDebugLog("\nBit rate impostato a: %d\n", wrq.u.bitrate.value);

	return 1;
}

int get_bitrate(int sock, struct iwreq* wrq)
{
      	if(ioctl(sock, SIOCGIWRATE, wrq) < 0)
	{
		perror("Ioctl error");
		return(0);
	}	

	CWDebugLog("\nBit rate: %d\n", wrq->u.bitrate.value);

	return 1;
}

/*--------------------------- RTS/CTS Threshold ---------------------------*/
int set_rts_cts(int sock, struct iwreq wrq, int value)
{
	if (value!=0) {wrq.u.rts.value=value;}
	else {wrq.u.rts.disabled=1;}	

      	if(ioctl(sock, SIOCSIWRTS, &wrq) < 0)
	{
		perror("Ioctl error");
		return(0);
	}	

	CWDebugLog("\nRTS/CTS threshold impostato a: %d\n", wrq.u.rts.value);

	return 1;
}

int get_rts_cts(int sock, struct iwreq* wrq)
{
      	if(ioctl(sock, SIOCGIWRTS, wrq) < 0)
	{
		perror("Ioctl error");
		return(0);
	}	
	
	if (wrq->u.rts.disabled!=1) {
		CWDebugLog("\nRTS/CTS threshold: %d\n", wrq->u.rts.value);
	}
	else {
		CWDebugLog("\nRTS/CTS threshold off\n");
	}

	return 1;
}

/*--------------------------- Fragmentation Threshold ---------------------------*/
int set_frag(int sock, struct iwreq wrq, int value)
{
	if (value!=0) {wrq.u.frag.value=value;}
	else {wrq.u.frag.disabled=1;}	

      	if(ioctl(sock, SIOCSIWFRAG, &wrq) < 0)
	{
		perror("Ioctl error");
		return(0);
	}	

	CWDebugLog("\nFragmentation threshold impostato a: %d\n", wrq.u.frag.value);

	return 1;
}

int get_frag(int sock, struct iwreq* wrq)
{
      	if(ioctl(sock, SIOCGIWFRAG, wrq) < 0)
	{
		perror("Ioctl error");
		return(0);
	}	
	
	if (wrq->u.frag.disabled!=1) {
		CWDebugLog("\nFragmentation threshold: %d\n", wrq->u.frag.value);
	}
	else {
		CWDebugLog("\nFragmentation threshold off\n");
	}

	return 1;
}

/*--------------------------- Transmit Power ---------------------------*/
/*remeember that fox =1 you can change the tx power you have to change it to 0 after change*/
int set_txpower(int sock, struct iwreq wrq, int value)
{
	wrq.u.txpower.value=value; 
	wrq.u.txpower.fixed=1;

      	if(ioctl(sock, SIOCSIWTXPOW, &wrq) < 0)
	{
		perror("Ioctl error");
		return(0);
	}
		
	wrq.u.txpower.fixed=0;/*i changed 1 to 0*/
	if(ioctl(sock, SIOCSIWTXPOW, &wrq) < 0)
	{
		perror("Ioctl error");
		return(0);
	}	

	CWDebugLog("\nTransmit power impostato a: %d\n", wrq.u.txpower.value);

	return 1;
}

int set_channel_cmd(int radio_id, int wlan_id, int channel){
	char cmdbuf[128];
	sprintf(cmdbuf,"iwconfig wifi%d channel %d",radio_id,channel);
	_system(cmdbuf);
	return 1;
}
int set_puren_cmd(char *wlanname ,unsigned char state){
	char cmdbuf[128];
	sprintf(cmdbuf,"iwpriv %s puren %d",wlanname,state);
	_system(cmdbuf);
	return 1;
}
int set_channel_extoffset_cmd(char *wlanname ,char *mode ,int extoffset,unsigned short cwmmode){
	char cmdbuf[128];
	sprintf(cmdbuf,"/usr/sbin/set_mode %s %s %d %d",wlanname,mode,extoffset,cwmmode);
	_system(cmdbuf);
	dpf("set extoffset cmd :%s \n",cmdbuf);
	return 1;
}
int set_tx_chainmask_cmd(char *wlanname ,unsigned char state ){
	char cmdbuf[128];
	sprintf(cmdbuf,"iwpriv %s tx_chainmask %d",wlanname,state);
	_system(cmdbuf);
	return 1;
}
int set_rx_chainmask_cmd(char *wlanname ,unsigned char state ){
	char cmdbuf[128];
	sprintf(cmdbuf,"iwpriv %s rx_chainmask %d",wlanname,state);
	_system(cmdbuf);
	return 1;
}
int set_wifi_tx_chainmask_cmd(char *wlanname ,unsigned char state ){
	/*gzm delete this limit later*/
	if(strcmp(WTPRealModelNum, "AQ2000-SNH") == 0){
		CWSetTxChainMask(wlanname,1);
	}
	/*gzm delete this limit later*/
	else{
	CWSetTxChainMask(wlanname,state);
	}
	return 1;
}
int set_wifi_rx_chainmask_cmd(char *wlanname ,unsigned char state ){
	/*gzm delete this limit later*/
	if(strcmp(WTPRealModelNum, "AQ2000-SNH") == 0){
		CWSetRxChainMask(wlanname,1);
	}
	/*gzm delete this limit later*/
	else{
	CWSetRxChainMask(wlanname,state);
	}
	return 1;
}
#if 0
int set_sta_report_by_dhcpsnooping(unsigned char state)
{
	CWWTPDebugLog("got msg to set AP DHCPsnooping Report, begin to set...");
	dpf("got msg to set AP DHCPsnooping Report, begin to set...\n");

	if(state == 0)
	{
		
		CWWTPDebugLog("set AP DHCPsnooping Report disable");
		dpf("set AP DHCPsnooping Report disable\n");
		if(gDhcpsnoopingReportEnable == 1)
		{
			if( (thread_snooping != 0) && (0 == pthread_kill(thread_snooping, 0) ) ) {
				pthread_cancel(thread_snooping);
			}
			gDhcpsnoopingReportEnable= 0;
			CWWTPDebugLog("set OK!");
			dpf("set OK!\n");
		}

	}
	else if(state == 1)
	{
		CWWTPDebugLog("set AP Scanning enable");
		dpf("set AP Scanning enable\n");

		if(gDhcpsnoopingReportEnable == 0)
		{
			gDhcpsnoopingReportEnable = 1;
			CWWTPDebugLog("set OK!");
			dpf("set OK!\n");
			if( (thread_snooping != 0) && (0 == pthread_kill(thread_snooping, 0) ) ) {
				pthread_cancel(thread_snooping);
//				pthread_join(thread_snooping, NULL);
			}
			if(!CWErr(CWCreateThread(&thread_snooping, CWWTPCheckMsgFromOtherPro, NULL,0))) {
				CWDebugLog("Error starting Thread that check DHCPsnooping Report");
				return CW_FALSE;
			}
		}
	}
	
	return CW_TRUE;
}
#endif

int set_ampdu_cmd(char *wlanname ,unsigned char state ,int ampdu_limit,int ampdu_frames){
	char cmdbuf[128];
	sprintf(cmdbuf,"iwpriv %s ampdu %d > /dev/null", wlanname,state);
       _system(cmdbuf);
       sprintf(cmdbuf,"iwpriv %s ampdulimit %d > /dev/null", wlanname,ampdu_limit); 
	_system(cmdbuf);
       sprintf(cmdbuf,"iwpriv %s ampdusframes %d > /dev/null", wlanname,ampdu_frames); /*yuan add ...*/
	_system(cmdbuf);
	return 1;
}

int set_wifi_ampdu_cmd(char *wlanname ,unsigned char state ,int ampdu_limit,int ampdu_frames){/*yuan add wifi ampdu...*/
	char cmdbuf[128];
	sprintf(cmdbuf,"iwpriv %s AMPDU %d > /dev/null", wlanname,state);
       _system(cmdbuf);
       sprintf(cmdbuf,"iwpriv %s AMPDULim %d > /dev/null", wlanname,ampdu_limit); 
	_system(cmdbuf);
       sprintf(cmdbuf,"iwpriv %s AMPDUFrames %d > /dev/null", wlanname,ampdu_frames); 
	_system(cmdbuf);
	return 1;
}

/*yuan add for amsdu*/
int set_amsdu_cmd(char *wlanname ,unsigned char state ,int amsdu_limit){
	char cmdbuf[128];
	sprintf(cmdbuf,"iwpriv %s amsdu %d > /dev/null", wlanname,state);
       _system(cmdbuf);
       sprintf(cmdbuf,"iwpriv %s amsdulimit %d > /dev/null", wlanname,amsdu_limit); 
	_system(cmdbuf);
       //sprintf(cmdbuf,"iwpriv %s amsdusframes %d > /dev/null", wlanname,amsdu_frames); 
	//system(cmdbuf);
	return 1;
}

int set_wifi_amsdu_cmd(char *wlanname ,unsigned char state ,int amsdu_limit){
	char cmdbuf[128];
	sprintf(cmdbuf,"iwpriv %s AMSDU %d > /dev/null", wlanname,state);
       _system(cmdbuf);
       sprintf(cmdbuf,"iwpriv %s AMSDULim %d > /dev/null", wlanname,amsdu_limit); 
	_system(cmdbuf);
       //sprintf(cmdbuf,"iwpriv %s AMSDUFrames %d > /dev/null", wlanname,amsdu_frames); 
	//system(cmdbuf);
	return 1;
}

int set_txpower_cmd(char *wlanname, unsigned char txtype, int txpower)
{
	char cmdbuf[128];
//	dpf("Set  %s  txpower: %d\n", wlanname, txpower);
	
	memset(cmdbuf, 0, 128);
	if (txtype == 1){
		txpower=txpower*(-1); // for cheap card
		sprintf(cmdbuf,"/usr/sbin/settxpower2 %s %d",wlanname, txpower);
	}
	if(txtype==0){
		if(txpower==100)
			sprintf(cmdbuf,"/usr/sbin/auto_power2 %s inc",wlanname);
		else
			sprintf(cmdbuf,"/usr/sbin/settxpower %s %d",wlanname, txpower);
	}
	_system(cmdbuf);

	return 1;
}

int set_rate_cmd(char *wlanname, unsigned int radioid,unsigned int ratemask)         //pei add 0716
{
 	char cmdbuf[128]={0};
	if(gRadioInfoValue[radioid].radiotype & IEEE80211_PARAM_MODE_11g )
	{
		sprintf(cmdbuf,"iwpriv wifi%d set_rate_mask_g 0x%x",radioid,ratemask);
	}
	else if(gRadioInfoValue[radioid].radiotype & IEEE80211_PARAM_MODE_11b)
	{
		sprintf(cmdbuf,"iwpriv wifi%d set_rate_mask_b 0x%x",radioid,ratemask);
	}
	else if (gRadioInfoValue[radioid].radiotype & IEEE80211_PARAM_MODE_11a)
	{
		sprintf(cmdbuf,"iwpriv wifi%d set_rate_mask_a 0x%x",radioid,ratemask);
	}
	_system(cmdbuf);

	memset(cmdbuf,0,128);
	sprintf(cmdbuf,"iwpriv wifi%d  set_rate_cap  0x80",radioid);
	_system(cmdbuf);

	return 1;
}

CWBool get_radio_mode(int radiotype,char **mode)
{
	char *p = NULL;
	
	CW_CREATE_OBJECT_SIZE_ERR(p, sizeof(char)*20, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY,NULL););
	memset(p,0,20);
	*mode = p;
	
	if ((radiotype&IEEE80211_PARAM_MODE_11n)&&(radiotype&IEEE80211_PARAM_MODE_11b)&&(radiotype&IEEE80211_PARAM_MODE_11g)){
		strcpy(p,"11bgng");
	}
	else if ((radiotype&IEEE80211_PARAM_MODE_11n)&&(radiotype&IEEE80211_PARAM_MODE_11g)&&(radiotype&IEEE80211_PARAM_MODE_11gn)){
		strcpy(p,"11gng");
	}
	else if ((radiotype&IEEE80211_PARAM_MODE_11n)&&(radiotype&IEEE80211_PARAM_MODE_11g)){
		strcpy(p,"11ng");
	}
	else if ((radiotype&IEEE80211_PARAM_MODE_11a)&&(radiotype&IEEE80211_PARAM_MODE_11n)&&(radiotype&IEEE80211_PARAM_MODE_11an)) {
		strcpy(p,"11ana");
	}
	else if ((radiotype&IEEE80211_PARAM_MODE_11a)&&(radiotype&IEEE80211_PARAM_MODE_11n)) {
		strcpy(p,"11na");
	}
	else if (radiotype&IEEE80211_PARAM_MODE_11n) {
		strcpy(p,"11n");
	}
	else if ((radiotype&IEEE80211_PARAM_MODE_11b)&&(radiotype&IEEE80211_PARAM_MODE_11g)){
		strcpy(p,"11bg");
	}
	else if (radiotype&IEEE80211_PARAM_MODE_11g)	{
		strcpy(p,"11g");
	}
	else if (radiotype&IEEE80211_PARAM_MODE_11b)	{
		strcpy(p,"11b");
	}
	else if (radiotype&IEEE80211_PARAM_MODE_11a){				//mode 11a
		strcpy(p,"11a");
	}
	else{
		CWDebugLog("Unknown Radio Type!");
		CWWTPDebugLog("Unknown Radio Type!");
		return CW_FALSE;
	}
	return CW_TRUE;
	
}

int set_radiotype_cmd(unsigned char radioId, unsigned char wlanId, int radiotype)         //pei add 0716
{
	char cmdbuf[128];
	char wlanname[IFNAMSIZ];
	char *mode = NULL;
	int extoffset;
	unsigned short cwmmode;

	cwmmode=gModeNParaValue[radioId].cwmMode;
	extoffset=gModeNParaValue[radioId].extoffset;

	memset(wlanname, 0, IFNAMSIZ);
	CWWTPGetWlanName(wlanname, radioId, wlanId);
	if(wlanId ==0){
		sprintf(wlanname,"%s",_iwlist_scan_if_name_for_wifi0_);
	}
	if( gSetScanWifiRadioType ){
		sprintf(wlanname,"%s",_iwlist_scan_if_name_for_wifi1_);
	}
	/* should set channel to 0(auto) before set mode
	 * pei modified at 081117 and default mode is b/g/n or a/n
	 * */
	memset(cmdbuf, 0, 128);
	CWSetChannel(radioId, wlanId,0);
	CWSetPuregEnable(wlanname,0);
	sprintf(cmdbuf,"iwpriv %s puren 0", wlanname);
	_system(cmdbuf);
	sprintf(cmdbuf,"iwpriv %s chwidth 0", wlanname);
	_system(cmdbuf);
	CWSetExtoffset(wlanname,0);
	
//frag must be set before mode changing  to 11n,11b/g/n,11a/n
	if(radiotype&IEEE80211_PARAM_MODE_11n)
	{
		if(debug_print)
				printf("set %s fragmatation off value %d\n",wlanname,2346);
			set_fragthreshold_cmd(wlanname,2346);
	}
	
	if(!get_radio_mode(radiotype,&mode))
	{
		CW_FREE_OBJECT(mode);
		CWWTPDebugLog("get radio mode error!");
		return 0;
	}
	
	CWWTPDebugLog("config wlan---mode is %s",mode);
	
	memset(cmdbuf, 0, 128);
	sprintf(cmdbuf,"/usr/sbin/set_mode %s %s %d %d",wlanname,mode,extoffset,cwmmode);
	_system(cmdbuf);
	

	if(!(radiotype&IEEE80211_PARAM_MODE_11n)) // not 11n ,set frag 
   	{
	 	if(debug_print)
	 		printf("set %s fragmatation value %d\n",wlanname,gRadioInfoValue[radioId].fragThreshold);
	 	set_fragthreshold_cmd(wlanname,gRadioInfoValue[radioId].fragThreshold);
 	}

	CW_FREE_OBJECT(mode);
	ConfArg confArg;
	memset(&confArg,0,sizeof(ConfArg));
	confArg.radioID = radioId;
	strcpy(confArg.conf_arg,"radiotype");
	sprintf(confArg.conf_value,"%d",radiotype);
	CWSaveConfigInfo(CONF_RADIO,confArg,CW_TRUE);
	
	return 1;
}
int set_radiotype_cmd1(unsigned char radioId, char *ifname, int radiotype)         //pei add 0716
{
	char cmdbuf[128];
	char wlanname[IFNAMSIZ];
	char *mode = NULL;
	int extoffset;
	unsigned short cwmmode;

	cwmmode=gModeNParaValue[radioId].cwmMode;
	extoffset=gModeNParaValue[radioId].extoffset;

	memset(wlanname, 0, IFNAMSIZ);
	memcpy(wlanname, ifname, strlen(ifname));
	/* should set channel to 0(auto) before set mode
	 * pei modified at 081117 and default mode is b/g/n or a/n
	 * */
	memset(cmdbuf, 0, 128);
	//CWSetChannel(wlanname,0); temp del by diaowq@20120530
	CWSetPuregEnable(wlanname,0);	
	
	//frag must be set before mode changing  to 11n,11b/g/n,11a/n
	if(radiotype&IEEE80211_PARAM_MODE_11n)
	{
		if(debug_print)
				printf("set %s fragmatation off value %d\n",wlanname,2346);
			set_fragthreshold_cmd(wlanname,2346);
	}

	if(!get_radio_mode(radiotype,&mode))
	{
		CW_FREE_OBJECT(mode);
		CWWTPDebugLog("get radio mode error!");
		return 0;
	}
	
	CWWTPDebugLog("config wlan---mode is %s",mode);
	
	memset(cmdbuf, 0, 128);
	sprintf(cmdbuf,"/usr/sbin/set_mode %s %s %d %d",wlanname,mode,extoffset,cwmmode);
	_system(cmdbuf);
	

	if(!(radiotype&IEEE80211_PARAM_MODE_11n)) // 11b 11b/g 11a or not 11n set frag 
   	{
	 	if(debug_print)
	 		printf("set %s fragmatation value %d\n",wlanname,gRadioInfoValue[radioId].fragThreshold);
	 	set_fragthreshold_cmd(wlanname,gRadioInfoValue[radioId].fragThreshold);
 	}

	CW_FREE_OBJECT(mode);
	
	ConfArg confArg;
	memset(&confArg,0,sizeof(ConfArg));
	confArg.radioID = radioId;
	strcpy(confArg.conf_arg,"radiotype");
	sprintf(confArg.conf_value,"%d",radiotype);
	CWSaveConfigInfo(CONF_RADIO,confArg,CW_TRUE);
	return 1;
}

int set_fragthreshold_cmd(char *wlanname, unsigned short fragThreshold)         //pei add 0722
{
	char cmdbuf[100];
	CWDebugLog("fragThreshold = %d\n", fragThreshold);
	sprintf(cmdbuf,"iwconfig %s frag %d",wlanname, fragThreshold);
	_system(cmdbuf);
	return 1;
}
int set_rtsthreshold_cmd(char *wlanname, unsigned short rtsThreshold)         //pei add 0729
{
	char cmdbuf[100];
	CWDebugLog("rtsThreshold = %d\n", rtsThreshold);
	sprintf(cmdbuf,"iwconfig %s rts %d",wlanname, rtsThreshold);
	_system(cmdbuf);
	return 1;
}
int set_radioconfig_cmd(unsigned char radioId, unsigned char wlanId, unsigned char preamble, unsigned char dtim, unsigned short beaconInterval)         //pei add 0722
{
	char wlanname[IFNAMSIZ];
	CWWTPGetWlanName(wlanname, radioId, wlanId);
	
	CWDebugLog("beaconInterval = %d dtim = %d preamble = %d\n", beaconInterval, dtim, preamble);
	CWSetBeaconInterval(radioId, wlanId, beaconInterval);
	CWSetPreamble(wlanname, preamble);
	CWSetDtim(wlanname, dtim);
	return 1;
}
int get_txpower(int sock, struct iwreq* wrq)
{
      	if(ioctl(sock, SIOCGIWTXPOW, wrq) < 0)
	{
		perror("Ioctl error");
		return(0);
	}	
	
	if (wrq->u.txpower.disabled!=1) {
		CWDebugLog("\nTransmit power: %d\n", wrq->u.txpower.value);
	}
	else {
		CWDebugLog("\nTransmit power off\n");
	}

	return 1;
}
/**************************** iwpriv ****************************/
/*--------------------------- CWMIN ---------------------------*/
int set_cwmin(int sock, struct iwreq wrq, int acclass, int sta, int value)
{
	int buffer[3];

	wrq.u.mode=IEEE80211_WMMPARAMS_CWMIN;
	buffer[0]=acclass;
	buffer[1]=sta;
	buffer[2]=value;
	memcpy(wrq.u.name + sizeof(int), buffer, sizeof(buffer));

      	if(ioctl(sock, IEEE80211_IOCTL_SETWMMPARAMS, &wrq) < 0)
	{
		perror("Ioctl error");
		return(0);
	}	

	CWDebugLog("\nCWMIN impostato a: %d\n", value);

	return 1;
}

int get_cwmin(int sock, struct iwreq* wrq, int acclass, int sta)
{
	int buffer[2];

	wrq->u.mode=IEEE80211_WMMPARAMS_CWMIN;
	buffer[0]=acclass;
	buffer[1]=sta;
	memcpy(wrq->u.name + sizeof(int), buffer, sizeof(buffer));

    	if(ioctl(sock, IEEE80211_IOCTL_GETWMMPARAMS, wrq) < 0)
	{
		perror("Ioctl error");
		return(0);
	}	
	
//	printf("\nCWMIN: %d\n", wrq->u.param.value);

	return 1;
}

/*--------------------------- CWMAX ---------------------------*/
int set_cwmax(int sock, struct iwreq wrq, int acclass, int sta, int value)
{
	int buffer[3];

	wrq.u.mode=IEEE80211_WMMPARAMS_CWMAX;
	buffer[0]=acclass;
	buffer[1]=sta;
	buffer[2]=value;
	memcpy(wrq.u.name + sizeof(int), buffer, sizeof(buffer));

      	if(ioctl(sock, IEEE80211_IOCTL_SETWMMPARAMS, &wrq) < 0)
	{
		perror("Ioctl error");
		return(0);
	}	

	CWDebugLog("\nCWMAX impostato a: %d\n", value);

	return 1;
}

int get_cwmax(int sock, struct iwreq* wrq, int acclass, int sta)
{
	int buffer[2];

	wrq->u.mode=IEEE80211_WMMPARAMS_CWMAX;
	buffer[0]=acclass;
	buffer[1]=sta;
	memcpy(wrq->u.name + sizeof(int), buffer, sizeof(buffer));

      	if(ioctl(sock, IEEE80211_IOCTL_GETWMMPARAMS, wrq) < 0)
	{
		perror("Ioctl error");
		return(0);
	}	
	
	//printf("\nCWMAX: %d\n", wrq->u.param.value);

	return 1;
}

/*--------------------------- AIFS ---------------------------*/
int set_aifs(int sock, struct iwreq wrq, int acclass, int sta, int value)
{
	int buffer[3];

	wrq.u.mode=IEEE80211_WMMPARAMS_AIFS;
	buffer[0]=acclass;
	buffer[1]=sta;
	buffer[2]=value;
	memcpy(wrq.u.name + sizeof(int), buffer, sizeof(buffer));

      	if(ioctl(sock, IEEE80211_IOCTL_SETWMMPARAMS, &wrq) < 0)
	{
		perror("Ioctl error");
		return(0);
	}	

	CWDebugLog("\nAIFS impostato a: %d\n", value);

	return 1;
}

int get_aifs(int sock, struct iwreq* wrq, int acclass, int sta)
{
	int buffer[2];

	wrq->u.mode=IEEE80211_WMMPARAMS_AIFS;
	buffer[0]=acclass;
	buffer[1]=sta;
	memcpy(wrq->u.name + sizeof(int), buffer, sizeof(buffer));

     	if(ioctl(sock, IEEE80211_IOCTL_GETWMMPARAMS, wrq) < 0)
	{
		perror("Ioctl error");
		return(0);
	}	
	
	//printf("\nAIFS: %d\n", wrq->u.param.value);

	return 1;
}
#if 0
int add_sta(int sock,AddSTAValues *addStaValues)
{
	int i;
	CWDebugLog("**************************************into add_sta******************************************");
	struct iwreq wrq;
	struct ieee80211req_mlme mlme;
	char wlanname[IFNAMSIZ];
	memset(&mlme, 0, sizeof(mlme));
	mlme.im_op = IEEE80211_MLME_AUTHORIZE;
	CWDebugLog("mlme.im_op %d",mlme.im_op);
	mlme.im_reason = 0;
	memcpy(mlme.im_macaddr, addStaValues->mac_addr, MAC_ADDR_LEN);
		printf("\nMAC:\n");
#if 1		
	for(i=0;i<6;i++){
		printf("%02x ",mlme.im_macaddr[i]);
	}
#endif	
	memset(&wrq, 0, sizeof(wrq));
	CWWTPGetWlanName(wlanname,addStaValues->wlan_id);
	strncpy(wrq.ifr_name, wlanname, IFNAMSIZ);
	CWDebugLog("iwr.ifr_name:%s",wrq.ifr_name);
	memcpy(wrq.u.name,&mlme,sizeof(mlme));
	//wrq.u.data.pointer = &mlme;
	//wrq.u.data.length = (int)sizeof(mlme);
	//wrq.u.mode = IEEE80211_IOCTL_SETMLME;
	//wrq.u.data.flags = 0;
	printf("wrq.u.data.length:%d\n",wrq.u.data.length);
	if(ioctl(sock,IEEE80211_IOCTL_SETMLME,&wrq)<0){
		printf("ioctl add sta error\n");
		printf("ioctl %X error\n",IEEE80211_IOCTL_SETMLME);
		return 0;
	}
	return 1;
}
#endif
int add_sta(int sock,AddSTAValues *addStaValues)
{
	int i;
	int ret;
	CWDebugLog("**************************************into add_sta******************************************");
//	struct iwreq wrq;
	struct ieee80211req_mlme mlme;
	char wlanname[IFNAMSIZ];
	memset(&mlme, 0, sizeof(mlme));
	mlme.im_op = IEEE80211_MLME_AUTHORIZE;
	CWDebugLog("mlme.im_op %d",mlme.im_op);
	mlme.im_reason = 0;
	memcpy(mlme.im_macaddr, addStaValues->mac_addr, MAC_ADDR_LEN);

	if(debug_print)
	{
		printf("**************************************into add_sta******************************************");
		printf("\nMAC:");
		for(i=0;i<6;i++){
			printf("%02x ",mlme.im_macaddr[i]);
		}
		printf("\n");
	}

	CWWTPDebugLog("into add_sta:[%02x-%02x-%02x-%02x-%02x-%02x]", mlme.im_macaddr[0], mlme.im_macaddr[1],mlme.im_macaddr[2],
		mlme.im_macaddr[3],mlme.im_macaddr[4],mlme.im_macaddr[5]);

	CWWTPGetWlanName(wlanname,addStaValues->radio_id,addStaValues->wlan_id);
	CWDebugLog("wlanname%s\n",wlanname);
	ret = set80211priv(sock,wlanname,IEEE80211_IOCTL_SETMLME, &mlme, sizeof(mlme));
	return ret;
}
int del_sta(int sock, DeleteSTAValues *deleteSTAValues, unsigned char *wlan_id)
{
	int i;
	int ret;
	CWDebugLog("**************************************into del_sta******************************************");
	struct ieee80211req_mlme mlme;
	char wlanname[IFNAMSIZ];
	memset(&mlme, 0, sizeof(mlme));
//	mlme.im_op = IEEE80211_MLME_UNAUTHORIZE;
	mlme.im_op = IEEE80211_MLME_DISASSOC;
	CWDebugLog("mlme.im_op %d",mlme.im_op);
	mlme.im_reason = 1;
	memcpy(mlme.im_macaddr, deleteSTAValues->mac_addr, MAC_ADDR_LEN);

	if(debug_print)
	{
		printf("**************************************into del_sta******************************************");
		printf("\nMAC:\n");
		for(i=0;i<6;i++){
			printf("%02x ",mlme.im_macaddr[i]);
		}
		printf("\n");
	}
	CWWTPDebugLog("into del_sta:[%02x-%02x-%02x-%02x-%02x-%02x]", mlme.im_macaddr[0], mlme.im_macaddr[1],mlme.im_macaddr[2],
		mlme.im_macaddr[3],mlme.im_macaddr[4],mlme.im_macaddr[5]);

	CWWTPGetWlanName(wlanname, deleteSTAValues->radio_id,*wlan_id);
	CWWTPDebugLog("***delete sta--- wlan_id:%d", *wlan_id);
	CWDebugLog("wlanname%s\n",wlanname);
	ret = set80211priv(sock,wlanname,IEEE80211_IOCTL_SETMLME, &mlme, sizeof(mlme));
	return 1;
}
int authmode(int sock){
	return 0;
}
int
set80211priv(int sock,char *ifname, int op, void *data, int len)
{
#define	N(a)	(sizeof(a)/sizeof(a[0]))
	struct iwreq iwr;
	memset(&iwr, 0, sizeof(iwr));
	strncpy(iwr.ifr_name, ifname, IFNAMSIZ);
	if (len < IFNAMSIZ) {
		/*
		 * Argument data fits inline; put it there.
		 */
		memcpy(iwr.u.name, data, len);
	} else {
		/*
		 * Argument data too big for inline transfer; setup a
		 * parameter block instead; the kernel will transfer
		 * the data for the driver.
		 */
		iwr.u.data.pointer = data;
		iwr.u.data.length = len;
	}

	if (ioctl(sock, op, &iwr) < 0) {
		CWWTPDebugLog("iwpriv failed");
		const char *opnames[] = {
			"ioctl[IEEE80211_IOCTL_SETPARAM]",
			"ioctl[IEEE80211_IOCTL_GETPARAM]",
			"ioctl[IEEE80211_IOCTL_SETKEY]",
			"ioctl[SIOCIWFIRSTPRIV+3]",
			"ioctl[IEEE80211_IOCTL_DELKEY]",
			"ioctl[SIOCIWFIRSTPRIV+5]",
			"ioctl[IEEE80211_IOCTL_SETMLME]",
			"ioctl[SIOCIWFIRSTPRIV+7]",
			"ioctl[IEEE80211_IOCTL_SETOPTIE]",
			"ioctl[IEEE80211_IOCTL_GETOPTIE]",
			"ioctl[IEEE80211_IOCTL_ADDMAC]",
			"ioctl[SIOCIWFIRSTPRIV+11]",
			"ioctl[IEEE80211_IOCTL_DELMAC]",
			"ioctl[SIOCIWFIRSTPRIV+13]",
			"ioctl[IEEE80211_IOCTL_CHANLIST]",
			"ioctl[SIOCIWFIRSTPRIV+15]",
			"ioctl[IEEE80211_IOCTL_GETRSN]",
			"ioctl[SIOCIWFIRSTPRIV+17]",
			"ioctl[IEEE80211_IOCTL_GETKEY]",
		};
		op -= SIOCIWFIRSTPRIV;
		if (0 <= op && op < N(opnames))
			perror(opnames[op]);
		else
			perror("ioctl[unknown???]");
		return -1;
	}
	return 0;
#undef N
}

int madwifi_set_key(int sock,STASessionKeyValues *staSessionKey,int alg,unsigned char radioId,unsigned char wlan_id)      /* peiwenhui 0605 */
{
	unsigned int key_idx;
	unsigned long int key_len;
	unsigned char *addr;
	int i;     //pei add 0605
	char ifname[IFNAMSIZ];

	CWWTPDebugLog("madwifi_set_key wlanid %d",wlan_id);
	CWWTPGetWlanName(ifname,radioId,wlan_id);
	CWWTPDebugLog("madwifi_set_key ifname %s",ifname);
	addr = staSessionKey->mac;
	key_idx = (unsigned int)((staSessionKey->flags>>12)&~(0xfffC));
	key_len = (long unsigned int)((staSessionKey->flags>>4)&~(0xff00));

	struct ieee80211req_key wk;
	unsigned int cipher;
/*
	if (strcmp(alg, "none") == 0)
		return madwifi_del_key(priv, addr, key_idx);
*/
	if ((alg == WTP_WPA_CIPHER_WEP40)||(alg == WTP_WPA_CIPHER_WEP104))
		cipher = IEEE80211_CIPHER_WEP;
	else if (alg == WTP_WPA_CIPHER_TKIP)
		cipher = IEEE80211_CIPHER_TKIP;
	else if (alg == WTP_WPA_CIPHER_CCMP)
		cipher = IEEE80211_CIPHER_AES_CCM;
	else {
		CWDebugLog("%d: unknown/unsupported algorithm %d\n",
				__func__, alg);
		return -1;
	}

	if (key_len > sizeof(wk.ik_keydata)) {
		CWDebugLog("%s: key length %lu too big\n", __func__, key_len);
		return -3;
	}

	memset(&wk, 0, sizeof(wk));
	wk.ik_type = cipher;
	wk.ik_flags = IEEE80211_KEY_RECV | IEEE80211_KEY_XMIT;
	if (addr == NULL) {
		memset(wk.ik_macaddr, 0xff, MAC_ADDR_LEN);
		wk.ik_keyix = key_idx;
		wk.ik_flags |= IEEE80211_KEY_DEFAULT;
	} else {
		memcpy(wk.ik_macaddr, addr, MAC_ADDR_LEN);
		wk.ik_keyix = IEEE80211_KEYIX_NONE;
	}
	wk.ik_keylen = key_len;
	memcpy(wk.ik_keydata, staSessionKey->key, key_len);

	if(addr == NULL)
		CWWTPDebugLog("wk.ik_flags = %d",wk.ik_flags);
	CWWTPDebugLog("wk.ik_type = %d  wk.ik_keyix = %d", wk.ik_type,wk.ik_keyix);
	CWWTPDebugLog("wk.ik_macaddr:[%02x-%02x-%02x-%02x-%02x-%02x]", wk.ik_macaddr[0], wk.ik_macaddr[1],
		wk.ik_macaddr[2], wk.ik_macaddr[3], wk.ik_macaddr[4], wk.ik_macaddr[5]);
	CWWTPDebugLog("wk.ik_keylen = %d",wk.ik_keylen);
#if 1
	if(debug_print)
	{
		if(addr == NULL)
		{
			printf("wk.ik_flags = %d\n",wk.ik_flags);
		}
		//printf("staSessionKey->key = %x\nalg = %d\n",staSessionKey->key,alg);
		printf("wk.ik_type = %d\nwk.ik_keyix = %d\n",
			wk.ik_type,wk.ik_keyix);
		printf("wk.ik_macaddr = ");
		for(i=0;i<6;i++)
		{
			printf("%02x ",wk.ik_macaddr[i]);
		}
		printf("\nwk.ik_keylen = %d\n",wk.ik_keylen);
		printf("wk.ik_keydata = ");
		for(i=0;i<wk.ik_keylen;i++)
		{
			printf("%x",wk.ik_keydata[i]);
		}
		printf("\n");
	}
#endif
	return set80211priv(sock,ifname, IEEE80211_IOCTL_SETKEY, &wk, sizeof(wk));
}
char * strop(int op)
{
	static char s[128];
	memset(s,0,128);
	switch(op){
		case IEEE80211_PARAM_WPA:
			strcpy(s,"IEEE80211_PARAM_WPA");
			break;
		case IEEE80211_PARAM_MCASTCIPHER:
			strcpy(s,"IEEE80211_PARAM_MCASTCIPHER");
			break;
		case IEEE80211_PARAM_MCASTKEYLEN:
			strcpy(s,"IEEE80211_PARAM_MCASTKEYLEN");
			break;
		case IEEE80211_PARAM_UCASTCIPHERS:
			strcpy(s,"IEEE80211_PARAM_UCASTCIPHERS");
			break;
		case IEEE80211_PARAM_KEYMGTALGS:
			strcpy(s,"IEEE80211_PARAM_KEYMGTALGS");
			break;
		case IEEE80211_PARAM_RSNCAPS:
			strcpy(s,"IEEE80211_PARAM_RSNCAPS");
			break;
		case 3:
			strcpy(s,"authmode");
			break;
		case IEEE80211_PARAM_PRIVACY:
			strcpy(s,"IEEE80211_PARAM_PRIVACY");
			break;
	}
	return s;
}
int set80211param(int sock, char *ifname, int op, int arg)
{
	CWWTPDebugLog("op:%d,str(op):%s",op,strop(op));

	struct iwreq iwr;

	memset(&iwr, 0, sizeof(iwr));
	strncpy(iwr.ifr_name, ifname, IFNAMSIZ);
	iwr.u.mode = op;
	memcpy(iwr.u.name+sizeof(__u32), &arg, sizeof(arg));

	if (ioctl(sock, IEEE80211_IOCTL_SETPARAM, &iwr) < 0) {
		perror("ioctl[IEEE80211_IOCTL_SETPARAM]");
		return -1;
	}
	return 0;
}

int madwifi_set_ieee8021x(int sock, WPA_IE *wpa_ie, unsigned char radioId,unsigned char wlan_id, int wpa_enable)
{
	char ifname[IFNAMSIZ];
	int enabled = 1;
	
	CWWTPGetWlanName(ifname,radioId,wlan_id);
	CWWTPDebugLog("%s: enabled=%d\n", __func__, enabled);

	if (!enabled) {
		/* XXX restore state */
		return set80211param(sock, ifname, 3,
			IEEE80211_AUTH_AUTO);
	}
	if ((wpa_ie->key_mgmt != WTP_WPA_KEY_MGMT_PSK)
		&&(wpa_ie->key_mgmt != WTP_WPA_KEY_MGMT_IEEE8021X_NO_WPA)
		&&(wpa_ie->key_mgmt != WTP_WPA_KEY_MGMT_IEEE8021X)) {
		CWWTPDebugLog("No 802.1x or WPA enabled!");
		return -1;
	}

	if (((wpa_ie->key_mgmt == WTP_WPA_KEY_MGMT_PSK) || (wpa_ie->key_mgmt == WTP_WPA_KEY_MGMT_IEEE8021X))&& madwifi_configure_wpa(sock, wpa_ie, ifname,wpa_enable) != 0) {
		CWWTPDebugLog("Error configuring WPA state!");
		return -1;
	}

	if (set80211param(sock, ifname, 3,/*auth mode*/
		((wpa_ie->key_mgmt == WTP_WPA_KEY_MGMT_PSK || wpa_ie->key_mgmt == WTP_WPA_KEY_MGMT_IEEE8021X )?  IEEE80211_AUTH_WPA : IEEE80211_AUTH_8021X))) {
		CWWTPDebugLog("Error enabling WPA/802.1x!");
		return -1;
	}
	
	return 0;
//	return madwifi_set_iface_flags(sock, ifname, 1);
}

int madwifi_set_privacy(int sock,unsigned char radioId, unsigned char wlan_id, int enabled)
{
	char ifname[IFNAMSIZ];
	CWWTPGetWlanName(ifname,radioId,wlan_id);
	CWDebugLog("%s: enabled=%d\n", __func__, enabled);

	return set80211param(sock, ifname, IEEE80211_PARAM_PRIVACY, enabled);
}
int madwifi_configure_wpa(int sock, WPA_IE *wpa_ie, char *ifname, int wpa_enable)
{
	CWWTPDebugLog("%s: enable WPA= 0x%x,ifname = %s\n", __func__, wpa_enable,ifname);
	if (set80211param(sock, ifname, IEEE80211_PARAM_WPA/*10*/, wpa_enable)) {
		CWWTPDebugLog("Unable to set WPA to %u\n", wpa_enable);
		return -1;
	}
	
	int v;

	switch (wpa_ie->group_cipher) {
	case WTP_WPA_CIPHER_CCMP:
		v = IEEE80211_CIPHER_AES_CCM;
		break;
	case WTP_WPA_CIPHER_TKIP:
		v = IEEE80211_CIPHER_TKIP;
		break;
	case WTP_WPA_CIPHER_WEP104:
		v = IEEE80211_CIPHER_WEP;
		break;
	case WTP_WPA_CIPHER_WEP40:
		v = IEEE80211_CIPHER_WEP;
		break;
	case WTP_WPA_CIPHER_NONE:
		v = IEEE80211_CIPHER_NONE;
		break;
	default:
		CWWTPDebugLog("Unknown group key cipher %u",
				wpa_ie->group_cipher);
		return -1;
	}
	CWWTPDebugLog("%s: group key cipher=%d", __func__, v);
	if (set80211param(sock, ifname, IEEE80211_PARAM_MCASTCIPHER/*5*/, v)) {
		CWWTPDebugLog("Unable to set group key cipher to %u", v);
		return -1;
	}
	if (v == IEEE80211_CIPHER_WEP) {
		/* key length is done only for specific ciphers */
		v = (wpa_ie->group_cipher == WTP_WPA_CIPHER_WEP104 ? 13 : 5);
		if (set80211param(sock, ifname, IEEE80211_PARAM_MCASTKEYLEN/*6*/, v)) {
			CWWTPDebugLog("Unable to set group key length to %u", v);
			return -1;
		}
	}

	v = 0;
	if (wpa_ie->pairwise_cipher & WTP_WPA_CIPHER_CCMP)
		v |= 1<<IEEE80211_CIPHER_AES_CCM;
	if (wpa_ie->pairwise_cipher & WTP_WPA_CIPHER_TKIP)
		v |= 1<<IEEE80211_CIPHER_TKIP;
	if (wpa_ie->pairwise_cipher & WTP_WPA_CIPHER_NONE)
		v |= 1<<IEEE80211_CIPHER_NONE;
	CWWTPDebugLog("%s: pairwise key ciphers=0x%x", __func__, v);
	if (set80211param(sock, ifname, IEEE80211_PARAM_UCASTCIPHERS/*7*/, v)) {
		CWWTPDebugLog("Unable to set pairwise key ciphers to 0x%x\n", v);
		return -1;
	}

	CWWTPDebugLog("%s: key management algorithms=0x%x",
			__func__, wpa_ie->key_mgmt);
	if (set80211param(sock, ifname, IEEE80211_PARAM_KEYMGTALGS/*21*/, wpa_ie->key_mgmt)) {
		CWWTPDebugLog("Unable to set key management algorithms to 0x%x",
				wpa_ie->key_mgmt);
		return -1;
	}

	v = 0;
	if (wpa_ie->capabilities)
		v |= BIT(0);
	CWWTPDebugLog("%s: rsn capabilities=0x%x", __func__, wpa_ie->capabilities);
	if (set80211param(sock, ifname, IEEE80211_PARAM_RSNCAPS/*22*/, v)) {
		CWWTPDebugLog("Unable to set RSN capabilities to 0x%x\n", v);
		return -1;
	}

	return 0;
}

int madwifi_del_key(int sock, char* ifname, unsigned char *addr, int key_idx)
{
	struct ieee80211req_del_key wk;
	memset(&wk, 0, sizeof(wk));
	if (addr != NULL) {
		memcpy(wk.idk_macaddr, addr, MAC_ADDR_LEN);
		wk.idk_keyix = (unsigned char)IEEE80211_KEYIX_NONE;  //pei 0917 del warning
	} else {
		wk.idk_keyix = key_idx;
	}	
	return set80211priv(sock, ifname, IEEE80211_IOCTL_DELKEY, &wk, sizeof(wk));
}


CWBool CWAssembleMsgElemAttackDetectInfo(CWProtocolMessage *msgPtr, attackDetectInfo *attackDetectInfoValues)    //pei add 0622
{
//	int k;
//	unsigned char value = ATTACK_DETECT_INFO;
	int vendor_specific_payload_length = 2;
	if(attackDetectInfoValues->is_flood_attack)
		vendor_specific_payload_length+=16;
	if(attackDetectInfoValues->is_spoof_attack)
		vendor_specific_payload_length+=16;
	if(attackDetectInfoValues->is_weakiv_attack)
		vendor_specific_payload_length+=16;

	if(msgPtr == NULL) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	
	CW_CREATE_PROTOCOL_MESSAGE(*msgPtr, vendor_specific_payload_length, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	
	CWProtocolStore8(msgPtr, attackDetectInfoValues->value);
	CWProtocolStore8(msgPtr, attackDetectInfoValues->count);
	dpf("value:%d count:%d\n", attackDetectInfoValues->value, attackDetectInfoValues->count);
	if(attackDetectInfoValues->is_flood_attack)
	{
		CWProtocolStore8(msgPtr, attackDetectInfoValues->flood_detect_type);
		CWProtocolStoreRawBytes(msgPtr, (char *)attackDetectInfoValues->flood_sta_mac, 6);
		CWProtocolStoreRawBytes(msgPtr, (char *)attackDetectInfoValues->flood_vap_mac, 6);
		CWProtocolStore8(msgPtr, attackDetectInfoValues->flood_frame_type);
		CWProtocolStore8(msgPtr, attackDetectInfoValues->flood_channel);
		CWProtocolStore8(msgPtr, attackDetectInfoValues->flood_rssi);
		dpf("flood_detect_type:%d  sta_mac: %02x:%02x:%02x:%02x:%02x:%02x  vap_mac: %02x:%02x:%02x:%02x:%02x:%02x frame_type:%d channel:%d rssi:%d\n", 
			attackDetectInfoValues->flood_detect_type, attackDetectInfoValues->flood_sta_mac[0], attackDetectInfoValues->flood_sta_mac[1], 
			attackDetectInfoValues->flood_sta_mac[2], attackDetectInfoValues->flood_sta_mac[3], attackDetectInfoValues->flood_sta_mac[4], 
			attackDetectInfoValues->flood_sta_mac[5], attackDetectInfoValues->flood_vap_mac[0], attackDetectInfoValues->flood_vap_mac[1], 
			attackDetectInfoValues->flood_vap_mac[2], attackDetectInfoValues->flood_vap_mac[3], attackDetectInfoValues->flood_vap_mac[4], 
			attackDetectInfoValues->flood_vap_mac[5], attackDetectInfoValues->flood_frame_type, attackDetectInfoValues->flood_channel, 
			attackDetectInfoValues->flood_rssi);
	}
	if(attackDetectInfoValues->is_spoof_attack)
	{
		CWProtocolStore8(msgPtr, attackDetectInfoValues->spoof_detect_type);
		CWProtocolStoreRawBytes(msgPtr, (char *)attackDetectInfoValues->spoof_sta_mac, 6);
		CWProtocolStoreRawBytes(msgPtr, (char *)attackDetectInfoValues->spoof_vap_mac, 6);
		CWProtocolStore8(msgPtr, attackDetectInfoValues->spoof_frame_type);
		CWProtocolStore8(msgPtr, attackDetectInfoValues->spoof_channel);
		CWProtocolStore8(msgPtr, attackDetectInfoValues->spoof_rssi);
		dpf("spoof_detect_type:%d  sta_mac: %02x:%02x:%02x:%02x:%02x:%02x  vap_mac: %02x:%02x:%02x:%02x:%02x:%02x frame_type:%d channel:%d rssi:%d\n", 
			attackDetectInfoValues->spoof_detect_type, attackDetectInfoValues->spoof_sta_mac[0], attackDetectInfoValues->spoof_sta_mac[1], 
			attackDetectInfoValues->spoof_sta_mac[2], attackDetectInfoValues->spoof_sta_mac[3], attackDetectInfoValues->spoof_sta_mac[4], 
			attackDetectInfoValues->spoof_sta_mac[5], attackDetectInfoValues->spoof_vap_mac[0], attackDetectInfoValues->spoof_vap_mac[1], 
			attackDetectInfoValues->spoof_vap_mac[2], attackDetectInfoValues->spoof_vap_mac[3], attackDetectInfoValues->spoof_vap_mac[4], 
			attackDetectInfoValues->spoof_vap_mac[5], attackDetectInfoValues->spoof_frame_type, attackDetectInfoValues->spoof_channel, 
			attackDetectInfoValues->spoof_rssi);
	}
	if(attackDetectInfoValues->is_weakiv_attack)
	{
		CWProtocolStore8(msgPtr, attackDetectInfoValues->weakiv_detect_type);
		CWProtocolStoreRawBytes(msgPtr, (char *)attackDetectInfoValues->weakiv_sta_mac, 6);
		CWProtocolStoreRawBytes(msgPtr, (char *)attackDetectInfoValues->weakiv_vap_mac, 6);
		CWProtocolStore8(msgPtr, attackDetectInfoValues->weakiv_payload);
		CWProtocolStore8(msgPtr, attackDetectInfoValues->weakiv_channel);
		CWProtocolStore8(msgPtr, attackDetectInfoValues->weakiv_rssi);
		dpf("weakiv_detect_type:%d sta_mac: %02x:%02x:%02x:%02x:%02x:%02x  vap_mac: %02x:%02x:%02x:%02x:%02x:%02x frame_type:%d channel:%d rssi:%d\n", 
			attackDetectInfoValues->weakiv_detect_type, attackDetectInfoValues->weakiv_sta_mac[0], attackDetectInfoValues->weakiv_sta_mac[1], 
			attackDetectInfoValues->weakiv_sta_mac[2], attackDetectInfoValues->weakiv_sta_mac[3], attackDetectInfoValues->weakiv_sta_mac[4], 
			attackDetectInfoValues->weakiv_sta_mac[5], attackDetectInfoValues->weakiv_vap_mac[0], attackDetectInfoValues->weakiv_vap_mac[1], 
			attackDetectInfoValues->weakiv_vap_mac[2], attackDetectInfoValues->weakiv_vap_mac[3], attackDetectInfoValues->weakiv_vap_mac[4], 
			attackDetectInfoValues->weakiv_vap_mac[5], attackDetectInfoValues->weakiv_payload, attackDetectInfoValues->weakiv_channel, 
			attackDetectInfoValues->weakiv_rssi);
	}
	
	return CWAssembleMsgElem(msgPtr, CW_MSG_ELEMENT_VENDOR_SPEC_PAYLOAD_CW_TYPE);
}
CWBool CWAssembleWTPEventAttackDetectInfoRequest (CWProtocolMessage **messagesPtr, int *fragmentsNumPtr, int PMTU, int seqNum, attackDetectInfo *attackDetectInfoValues) 	//pei add 0422
{
	CWProtocolMessage *msgElems= NULL;
	const int msgElemCount = 1;
	CWProtocolMessage *msgElemsBinding= NULL;
	const int msgElemBindingCount=0;
	
	if(messagesPtr == NULL || fragmentsNumPtr == NULL) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);

	CW_CREATE_PROTOCOL_MSG_ARRAY_ERR(msgElems, msgElemCount, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););	
	
	CWDebugLog("Assembling WTP Event Request AP attack Detect Info...");

	// Assemble Message Elements
	if (!(CWAssembleMsgElemAttackDetectInfo(msgElems, attackDetectInfoValues))) {
		CW_FREE_OBJECT(msgElems);
		return CW_FALSE; // error will be handled by the caller
	}
	
	if(!(CWAssembleMessage(messagesPtr,
			       fragmentsNumPtr,
			       PMTU,
			       seqNum,
			       CW_MSG_TYPE_VALUE_WTP_EVENT_REQUEST,
			       msgElems,
			       msgElemCount,
			       msgElemsBinding,
			       msgElemBindingCount,
#ifdef CW_NO_DTLS
			       CW_PACKET_PLAIN
#else
			       (gDtlsSecurity == 1)?CW_PACKET_CRYPT:CW_PACKET_PLAIN           /* 0-CW_PACKET_PLAIN, 1-CW_PACKET_CRYPT */
#endif
			       ))) 
		return CW_FALSE;
	
	CWDebugLog("WTP Event Request attack Detect Info Assembled");

	return CW_TRUE;
}
CWBool CWWTPGetAttackDetectInfo(attackDetectInfo *attackDetectInfoValues)
{
	int i=0;
	CWProtocolMessage *messages = NULL;
	int fragmentsNum=0;
	int seqNum;
	seqNum = CWGetSeqNum();

	/* assemble WTP Event request element with info of AP AttackDetectInfo */
	if(!CWAssembleWTPEventAttackDetectInfoRequest(&messages, &fragmentsNum, gWTPPathMTU, seqNum, attackDetectInfoValues))
	{
		CWWTPDebugLog("Assemble WTP Event Ap AttackDetectInfo Request error!");
		return CW_FALSE;
	}
	
	/* send AttackDetectInfo info */
	for(i = 0; i < fragmentsNum; i++) 
	{
//		dpf("messages[%d].offset:%d\n", i, messages[i].offset);
//		CWCaptrue(messages[i].offset, (unsigned char *)(messages[i].msg));

#ifndef CW_NO_DTLS
		if(gDtlsSecurity == 1)
		{
			if(!CWSecuritySend(gWTPSession, messages[i].msg, messages[i].offset))
			{
				CWDebugLog("Error sending message\n");
				printf("Error sending message\n");
				break;  //gzm change countinue to break 091029
			}
		}
		else
#endif
		{
			if (!CWNetworkSendUnsafeUnconnected(gWTPSocket, &(gACInfoPtr->preferredAddress), messages[i].msg, messages[i].offset))
			{
				CWDebugLog("Error sending message\n");
				printf("Error sending message\n");
				break;  //gzm change countinue to break 091029
			}
		}
	}
	CWDebugLog("Message Sent\n");
	CWFreeMessageFragments(messages, fragmentsNum);
	CW_FREE_OBJECT(messages);

	return CW_TRUE;
}

CWBool CWGetAttackDetectInfo(GetAttackDetectInfoValues *getAttackDetectInfoValues)
{
	CWWTPDebugLog("got msg to get AttackDetectInfo, begin to set...");
	if(getAttackDetectInfoValues->value!=AttackDetectInfo)
		return CW_FALSE;

	char wlanname[IFNAMSIZ];
	CWWTPWlan *ptr = NULL;
	CWWTPWlan *oldptr = NULL;

	if(wtp_wlan_count == 0)
	{
		CWWTPDebugLog("There is no wlan, when set attack detect");
		return CW_TRUE;
	}
	
/*	for(ptr=wtp_wlan_list;ptr != NULL;oldptr = ptr,ptr = ptr->next)
	{
		CWWTPGetWlanName(wlanname, ptr->radio_id, ptr->wlan_id);
		break;
	}
*/
	/* set flooding detect */
	if(!getAttackDetectInfoValues->floodingDetectEnable && gFloodingDetectEnable)
	{
		gFloodingDetectEnable = 0;
		for(ptr=wtp_wlan_list;ptr != NULL;oldptr = ptr,ptr = ptr->next)
		{
			CWWTPGetWlanName(wlanname, ptr->radio_id, ptr->wlan_id);
			CWSetWidFloodDetectEnable(wlanname,0);
		}
		
		CWWTPDebugLog("set AP floodingDetect disable");
	}
	else if(getAttackDetectInfoValues->floodingDetectEnable && !gFloodingDetectEnable)
	{
		gFloodingDetectEnable = 1;
		gFloodAttackCnt = getAttackDetectInfoValues->flood_attack_cnt;
		gFloodProbeCnt = getAttackDetectInfoValues->flood_probe_cnt;
		gFloodWidsIntval = getAttackDetectInfoValues->wids_intval;
		for(ptr=wtp_wlan_list;ptr != NULL;oldptr = ptr,ptr = ptr->next)
		{
			CWWTPGetWlanName(wlanname, ptr->radio_id, ptr->wlan_id);
			CWSetWidFloodDetectEnable(wlanname,1);
			CWSetWidFloodAttackCnt(wlanname, gFloodAttackCnt);
			CWSetWidFloodProbeCnt(wlanname, gFloodProbeCnt);
			CWSetWidFloodInterval(wlanname, gFloodWidsIntval*1000);
		}
		
		CWWTPDebugLog("set AP floodingDetect enable");
		
		ConfArg confArg;
		memset(&confArg,0,sizeof(confArg));
		strcpy(confArg.conf_arg,"gFloodAttackCnt");
		sprintf(confArg.conf_value,"%d",gFloodAttackCnt);
		CWSaveConfigInfo(CONF_WTPD,confArg,CW_TRUE);
		
		memset(&confArg,0,sizeof(confArg));
		strcpy(confArg.conf_arg,"gFloodProbeCnt");
		sprintf(confArg.conf_value,"%d",gFloodProbeCnt);
		CWSaveConfigInfo(CONF_WTPD,confArg,CW_FALSE);
		
		memset(&confArg,0,sizeof(confArg));		
		strcpy(confArg.conf_arg,"gFloodWidsIntval");
		sprintf(confArg.conf_value,"%d",gFloodWidsIntval);
		CWSaveConfigInfo(CONF_WTPD,confArg,CW_FALSE);	
	}

	/* set spoofing detect */
	if(!getAttackDetectInfoValues->spoofingDetectEnable && gSpoofingDetectEnable)
	{
		gSpoofingDetectEnable = 0;
		for(ptr=wtp_wlan_list;ptr != NULL;oldptr = ptr,ptr = ptr->next)
		{
			CWWTPGetWlanName(wlanname, ptr->radio_id, ptr->wlan_id);
			CWSetWidSpoofDetectEnable(wlanname,0);
		}
		
		CWWTPDebugLog("set AP spoofingDetect disable");
	}
	else if(getAttackDetectInfoValues->spoofingDetectEnable && !gSpoofingDetectEnable)
	{
		gSpoofingDetectEnable = 1;
		for(ptr=wtp_wlan_list;ptr != NULL;oldptr = ptr,ptr = ptr->next)
		{
			CWWTPGetWlanName(wlanname, ptr->radio_id, ptr->wlan_id);
			CWSetWidSpoofDetectEnable(wlanname,1);
		}
		
		CWWTPDebugLog("set AP spoofingDetect enable");
	}

	/* set weak IV detect */
	if(!getAttackDetectInfoValues->weakIVDetectEnable && gWeakIVDetectEnable)
	{
		gWeakIVDetectEnable = 0;
		for(ptr=wtp_wlan_list;ptr != NULL;oldptr = ptr,ptr = ptr->next)
		{
			CWWTPGetWlanName(wlanname, ptr->radio_id, ptr->wlan_id);
			CWSetWidWeakIVDetectEnable(wlanname,0);
		}
		
		CWWTPDebugLog("set AP weakIVDetect disable");
	}
	else if(getAttackDetectInfoValues->weakIVDetectEnable && !gWeakIVDetectEnable)
	{
		gWeakIVDetectEnable = 1;
		for(ptr=wtp_wlan_list;ptr != NULL;oldptr = ptr,ptr = ptr->next)
		{
			CWWTPGetWlanName(wlanname, ptr->radio_id, ptr->wlan_id);
			CWSetWidWeakIVDetectEnable(wlanname,1);
		}
		
		CWWTPDebugLog("set AP weakIVDetect enable");
	}
	
	ConfArg confArg;
	memset(&confArg,0,sizeof(confArg));
	strcpy(confArg.conf_arg,"gFloodingDetectEnable");
	sprintf(confArg.conf_value,"%d",gFloodingDetectEnable);
	CWSaveConfigInfo(CONF_WTPD,confArg,CW_TRUE);
	
	strcpy(confArg.conf_arg,"gSpoofingDetectEnable");
	sprintf(confArg.conf_value,"%d",gSpoofingDetectEnable);
	CWSaveConfigInfo(CONF_WTPD,confArg,CW_FALSE);
	
	strcpy(confArg.conf_arg,"gWeakIVDetectEnable");
	sprintf(confArg.conf_value,"%d",gWeakIVDetectEnable);
	CWSaveConfigInfo(CONF_WTPD,confArg,CW_FALSE);	
	return CW_TRUE;
}
CWBool CWSetNtp(GetNtpValues *NtpValues)
{
	char cmd_ntp[128];
	char str[128];
	char ntp_ip[128];
	struct sockaddr *sa=(struct sockaddr*) (&gACInfoPtr->preferredAddress);
	struct sockaddr_in	*sin = (struct sockaddr_in *) sa;
	inet_ntop(AF_INET, &sin->sin_addr, str, sizeof(str));
	/*get time from AC*/
	memset(ntp_ip, 0, 128);
	ip_int2str(NtpValues->ipv4, (unsigned char *)ntp_ip);
	if((NtpValues->ipv4==0)||(memcmp(ntp_ip, "255.255.255.255", strlen("255.255.255.255"))==0)){
		memset(ntp_ip, 0, 128);
		strcpy(ntp_ip,str);
	}
	if(NtpValues->state==0){
		sprintf(cmd_ntp,"/usr/sbin/set_crontab ntpclient m%d \"-h %s -s\" >/dev/null 2>&1 &",((NtpValues->ntp_intval)<60)?1:(NtpValues->ntp_intval)/60,ntp_ip);
		_system(cmd_ntp);
	}else{
		sprintf(cmd_ntp,"/usr/sbin/set_crontab ntpclient m0 \"-h %s -s\" >/dev/null 2>&1 &",ntp_ip);
		_system(cmd_ntp);
	}

	return CW_TRUE;
}
CWBool CWSetTerminalDisturb(GetTerminalDisturbValues *TerminalDisturbValues)
{
	char wlanname[IFNAMSIZ];
	CWWTPWlan *ptr = NULL;
	CWWTPWlan *oldptr = NULL;
	char cmdbuf[128];
	
	CWWTPDebugLog("got msg to get TerminalDisturbInfo, begin to set...");
	if(TerminalDisturbValues->value!=TerminalDisturbInfo)
		return CW_FALSE;

	if(wtp_wlan_count == 0)
	{
		CWWTPDebugLog("There is no wlan, when set Terminal Disturb");
		return CW_TRUE;
	}
	
	if(!TerminalDisturbValues->opstate && gTerminalDisturbEnable)
	{
		gTerminalDisturbEnable = 0;
		for(ptr=wtp_wlan_list;ptr != NULL;oldptr = ptr,ptr = ptr->next)
		{
			CWWTPGetWlanName(wlanname, ptr->radio_id, ptr->wlan_id);
			sprintf(cmdbuf, "echo 0 > /proc/sys/dev/wifi0/sta_interferce");
			_system(cmdbuf);
			sprintf(cmdbuf, "iwpriv %s sta_interfer 0", wlanname);
			_system(cmdbuf);
		}
		
		CWWTPDebugLog("set AP TerminalDisturb disable");
	}
	else if(TerminalDisturbValues->opstate && !gTerminalDisturbEnable)
	{
		gTerminalDisturbEnable = 1;
		for(ptr=wtp_wlan_list;ptr != NULL;oldptr = ptr,ptr = ptr->next)
		{
			CWWTPGetWlanName(wlanname, ptr->radio_id, ptr->wlan_id);
			sprintf(cmdbuf, "echo 1 > /proc/sys/dev/wifi0/sta_interferce");
			_system(cmdbuf);
			sprintf(cmdbuf, "iwpriv %s interfer_count %d", wlanname, TerminalDisturbValues->terDisturbStaCnt);
			_system(cmdbuf);
			sprintf(cmdbuf, "iwpriv %s sta_interfer %d", wlanname, TerminalDisturbValues->terDisturbPkt);
			_system(cmdbuf);
			sprintf(cmdbuf, "iwpriv %s wids_intval 60000", wlanname);
			_system(cmdbuf);
		}
		
		CWWTPDebugLog("set AP TerminalDisturb enable");
	}
	else if(TerminalDisturbValues->opstate && gTerminalDisturbEnable)
	{
		for(ptr=wtp_wlan_list;ptr != NULL;oldptr = ptr,ptr = ptr->next)
		{
			CWWTPGetWlanName(wlanname, ptr->radio_id, ptr->wlan_id);
			sprintf(cmdbuf, "iwpriv %s interfer_count %d", wlanname, TerminalDisturbValues->terDisturbStaCnt);
			_system(cmdbuf);
			sprintf(cmdbuf, "iwpriv %s sta_interfer %d", wlanname, TerminalDisturbValues->terDisturbPkt);
			_system(cmdbuf);
		}
		
		CWWTPDebugLog("TerminalDisturb is already enable, set parameter");
	}
	
	return CW_TRUE;
}

CWBool isescapeletter(char c)
{
	if ( (c>32 && c< 48) || (c>57 && c<65) || (c>90 && c<97) || (c>122 && c<127))
	 	return CW_TRUE;
	else
		return CW_FALSE;
}
	
CWBool CWSetUserandPasswdinfo (WtpUserPasswdValues *valPtr)
{
	char cmdbuf[256] = {0};
	char usermem[128]={0};
	char passwdmem[128]={0};
	int i = 0,j;
	char *p = NULL;
	int userlength = 0;
	int pwdlength = 0;

	userlength = strlen(valPtr->username);
	pwdlength =  strlen(valPtr->passwd);
	p = valPtr->username;
	for(j=0;j<userlength;j++){
		if(isescapeletter(*p))
			usermem[i++]='\\';
		usermem[i++]=*(p++);
	}
	p= valPtr->passwd;
	i=0,j=0;
	while(j++<pwdlength){
		if(isescapeletter(*p))  
			passwdmem[i++]='\\';
		passwdmem[i++]=*(p++);
	}
	
	CWWTPDebugLog("username = %s passwd = %s", valPtr->username,valPtr->passwd);
	sprintf(cmdbuf,"/usr/sbin/set_user_passwd %s %s &", usermem,passwdmem);
	_system(cmdbuf);
	
	return CW_TRUE;
}

CWBool CWSetMultiUserPerformance(GetMultiUserPerformanceValues *MultiUserPerformanceValues)
{
	char wlanname[IFNAMSIZ];
	CWBool radio_found = CW_FALSE;
	CWWTPWlan *ptr = NULL;
	char cmdbuf[128];
	memset(wlanname, 0, IFNAMSIZ);
	if(MultiUserPerformanceValues == NULL) {return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);}
	
	for(ptr=wtp_wlan_list;ptr != NULL;ptr = ptr->next){
		if(ptr->radio_id == MultiUserPerformanceValues->radio_id){
			radio_found = CW_TRUE;
			break;
		}
	}
	
	if(radio_found == CW_FALSE){
		CWDebugLog("The WTP dosen't have radio %d",MultiUserPerformanceValues->radio_id);
		return CW_TRUE;
	}
	
	if(MultiUserPerformanceValues->op_type==WLAN){
		CWWTPGetWlanName(wlanname,MultiUserPerformanceValues->radio_id,MultiUserPerformanceValues->wlan_id);
		CWWTPDebugLog("wlanname=%s MultiUserPerformanceValues->state=%d MultiUserPerformanceValues->value=%d !\n",wlanname,MultiUserPerformanceValues->state, MultiUserPerformanceValues->value);
		sprintf(cmdbuf,"autelan 80211_cache %s set_cache_enable %d", wlanname, MultiUserPerformanceValues->value); 
		_system(cmdbuf);
	}
	CWDebugLog("%s MultiUserPerformanceValues set OK the MultiUserPerformanceValues  now is :%d  ",wlanname,MultiUserPerformanceValues->value);
	return CW_TRUE;
}

CWBool CWSetMcsMask(GetMcsMaskValues *McsMaskValues)
{
	CWBool radio_found = CW_FALSE;
	CWWTPWlan *ptr = NULL;
	char cmdbuf[128];
	unsigned int value;
	if(McsMaskValues == NULL) {return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);}
	
	for(ptr=wtp_wlan_list;ptr != NULL;ptr = ptr->next){
		if(ptr->radio_id == McsMaskValues->radio_id){
			radio_found = CW_TRUE;
			break;
		}
	}
	
	if(radio_found == CW_FALSE){
		CWWTPDebugLog("The WTP dosen't have radio %d", McsMaskValues->radio_id);
		return CW_TRUE;
	}	

	CWWTPDebugLog("McsMaskValues->value=0x%x !", McsMaskValues->value);
	
	value = BitReverse32(McsMaskValues->value);
	value = value&0x00FFFFFF;

	memset(cmdbuf, 0, 128);
	sprintf(cmdbuf,"iwpriv wifi%d set_mcs_mask 0x%x", McsMaskValues->radio_id, value); 
	_system(cmdbuf);
	
	memset(cmdbuf, 0, 128);
	sprintf(cmdbuf,"iwpriv wifi%d set_mcs_cap 0x%x", McsMaskValues->radio_id, 0x80);
	_system(cmdbuf);
	
	CWWTPDebugLog("wifi%d McsMaskValues set OK the McsMaskValues  now is 0x:%x  ", McsMaskValues->radio_id, value);
	return CW_TRUE;
}

CWBool CWAssembleMsgElemAttackAddrRedirection(CWProtocolMessage *msgPtr, attackAddrRedirectionInfo *attackAddrRedirectionInfoValues)    //pei add 0622
{
//	int i = 0;
	int vendor_specific_payload_length = 16;

	if(msgPtr == NULL) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	
	CW_CREATE_PROTOCOL_MESSAGE(*msgPtr, vendor_specific_payload_length, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	
	CWProtocolStore8(msgPtr, attackAddrRedirectionInfoValues->value);
	CWProtocolStore32(msgPtr, 0); /* for future use */
	CWProtocolStore16(msgPtr, attackAddrRedirectionInfoValues->count);
	CWProtocolStore8(msgPtr, attackAddrRedirectionInfoValues->radio_id);
	attackAddrRedirectionInfoValues->wlan_id = CWGetRemoteWlanID(attackAddrRedirectionInfoValues->radio_id, attackAddrRedirectionInfoValues->wlan_id);
	CWProtocolStore8(msgPtr, attackAddrRedirectionInfoValues->wlan_id);
	CWProtocolStore8(msgPtr, attackAddrRedirectionInfoValues->state);
	CWProtocolStoreRawBytes(msgPtr, (char *)attackAddrRedirectionInfoValues->sta_mac, 6);
	
	dpf("value:%d count:%d radio_id:%d wlan_id:%d state:%d sta_mac: %02x:%02x:%02x:%02x:%02x:%02x\n", 
		attackAddrRedirectionInfoValues->value, attackAddrRedirectionInfoValues->count, attackAddrRedirectionInfoValues->radio_id,
		attackAddrRedirectionInfoValues->wlan_id, attackAddrRedirectionInfoValues->state, attackAddrRedirectionInfoValues->sta_mac[0], 
		attackAddrRedirectionInfoValues->sta_mac[1], attackAddrRedirectionInfoValues->sta_mac[2], attackAddrRedirectionInfoValues->sta_mac[3], 
		attackAddrRedirectionInfoValues->sta_mac[4], attackAddrRedirectionInfoValues->sta_mac[5]);
	
	return CWAssembleMsgElem(msgPtr, CW_MSG_ELEMENT_VENDOR_SPEC_PAYLOAD_CW_TYPE);
}

CWBool CWAssembleMsgElemstaThroughputSendtoAcValues(CWProtocolMessage *msgPtr, staThroughputsendtoAc *staThroughputSendtoAcValues)
{
	int vendor_specific_payload_length = 36;

	if(msgPtr == NULL) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	
	CW_CREATE_PROTOCOL_MESSAGE(*msgPtr, vendor_specific_payload_length, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	
	CWProtocolStore8(msgPtr, staThroughputSendtoAcValues->optype);
	CWProtocolStore16(msgPtr, staThroughputSendtoAcValues->len);
	CWProtocolStore8(msgPtr, 0); /* for future use */
	CWProtocolStoreRawBytes(msgPtr, (char *)staThroughputSendtoAcValues->sta_mac, 6);
	CWProtocolStore32(msgPtr, staThroughputSendtoAcValues->frames_recv);
	CWProtocolStore32(msgPtr, staThroughputSendtoAcValues->frames_send);
	CWProtocolStore64(msgPtr, staThroughputSendtoAcValues->bytes_recv);
	CWProtocolStore64(msgPtr, staThroughputSendtoAcValues->bytes_send);
	CWProtocolStore16(msgPtr, 0); /* for fture use */
	
	if(debug_print)
		printf("optype:%d len:%d sta_mac: %02x:%02x:%02x:%02x:%02x:%02x frames_recv:%u frames_send:%u bytes_recv:%llu bytes_send:%llu\n", 
		staThroughputSendtoAcValues->optype, staThroughputSendtoAcValues->len, staThroughputSendtoAcValues->sta_mac[0], staThroughputSendtoAcValues->sta_mac[1], staThroughputSendtoAcValues->sta_mac[2], 
		staThroughputSendtoAcValues->sta_mac[3], staThroughputSendtoAcValues->sta_mac[4], 
		staThroughputSendtoAcValues->sta_mac[5],staThroughputSendtoAcValues->frames_recv,
		staThroughputSendtoAcValues->frames_send, staThroughputSendtoAcValues->bytes_recv, 
		staThroughputSendtoAcValues->bytes_send);
	
	return CWAssembleMsgElem(msgPtr, CW_MSG_ELEMENT_VENDOR_SPEC_PAYLOAD_CW_TYPE);
}

CWBool CWAssembleMsgElemChallengeReplay(CWProtocolMessage *msgPtr, challengeReplayInfo *challengeReplayInfoValues)    //pei add 0622
{
//	int i = 0;
	int vendor_specific_payload_length = 16;

	if(msgPtr == NULL) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	
	CW_CREATE_PROTOCOL_MESSAGE(*msgPtr, vendor_specific_payload_length, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	
	CWProtocolStore8(msgPtr, challengeReplayInfoValues->value);
	CWProtocolStore32(msgPtr, 0); /* for future use */
	CWProtocolStore16(msgPtr, challengeReplayInfoValues->count);
	CWProtocolStore8(msgPtr, challengeReplayInfoValues->radio_id);
	challengeReplayInfoValues->wlan_id = CWGetRemoteWlanID(challengeReplayInfoValues->radio_id, challengeReplayInfoValues->wlan_id);
	CWProtocolStore8(msgPtr, challengeReplayInfoValues->wlan_id);
	CWProtocolStore8(msgPtr, challengeReplayInfoValues->state);
	CWProtocolStoreRawBytes(msgPtr, (char *)challengeReplayInfoValues->sta_mac, 6);
	
	dpf("value:%d count:%d radio_id:%d wlan_id:%d state:%d sta_mac: %02x:%02x:%02x:%02x:%02x:%02x\n", 
		challengeReplayInfoValues->value, challengeReplayInfoValues->count, challengeReplayInfoValues->radio_id,
		challengeReplayInfoValues->wlan_id, challengeReplayInfoValues->state, challengeReplayInfoValues->sta_mac[0], 
		challengeReplayInfoValues->sta_mac[1], challengeReplayInfoValues->sta_mac[2], challengeReplayInfoValues->sta_mac[3], 
		challengeReplayInfoValues->sta_mac[4], challengeReplayInfoValues->sta_mac[5]);
	
	return CWAssembleMsgElem(msgPtr, CW_MSG_ELEMENT_VENDOR_SPEC_PAYLOAD_CW_TYPE);
}
CWBool CWAssembleMsgElemTerminalDisturb(CWProtocolMessage *msgPtr, terminalDisturbInfo *terminalDisturbInfoValues)
{
	int i = 0;
	int vendor_specific_payload_length = 10+6*terminalDisturbInfoValues->sta_count;

	if(msgPtr == NULL) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	
	CW_CREATE_PROTOCOL_MESSAGE(*msgPtr, vendor_specific_payload_length, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	
	CWProtocolStore8(msgPtr, terminalDisturbInfoValues->value);
	CWProtocolStore8(msgPtr, terminalDisturbInfoValues->radio_id);
	terminalDisturbInfoValues->wlan_id = CWGetRemoteWlanID(terminalDisturbInfoValues->radio_id, terminalDisturbInfoValues->wlan_id);
	CWProtocolStore8(msgPtr, terminalDisturbInfoValues->wlan_id);
	CWProtocolStore8(msgPtr, terminalDisturbInfoValues->state);
	CWProtocolStore32(msgPtr, 0); /* for future use */
	CWProtocolStore16(msgPtr, terminalDisturbInfoValues->sta_count);
	CWProtocolStoreRawBytes(msgPtr, (char *)terminalDisturbInfoValues->sta_mac, 6*terminalDisturbInfoValues->sta_count);
	
	if(debug_print)
	{
		printf("value:%d radio_id:%d wlan_id:%d state:%d sta_count:%d\n", terminalDisturbInfoValues->value, terminalDisturbInfoValues->radio_id, 
			terminalDisturbInfoValues->wlan_id, terminalDisturbInfoValues->state, terminalDisturbInfoValues->sta_count);
		for(i=0; i<terminalDisturbInfoValues->sta_count; i++)
			printf("sta_mac[%d]: %02x:%02x:%02x:%02x:%02x:%02x\n", i, *(terminalDisturbInfoValues->sta_mac+i*6), *(terminalDisturbInfoValues->sta_mac+6*i+1), *(terminalDisturbInfoValues->sta_mac+6*i+2), 
				*(terminalDisturbInfoValues->sta_mac+6*i+3), *(terminalDisturbInfoValues->sta_mac+6*i+4), *(terminalDisturbInfoValues->sta_mac+6*i+5));
	}
	
	return CWAssembleMsgElem(msgPtr, CW_MSG_ELEMENT_VENDOR_SPEC_PAYLOAD_CW_TYPE);
}

CWBool CWAssembleWTPEventAttackAddrRedirectionRequest (CWProtocolMessage **messagesPtr, int *fragmentsNumPtr, int PMTU, int seqNum, attackAddrRedirectionInfo *attackAddrRedirectionInfoValues) 	//pei add 0422
{
	CWProtocolMessage *msgElems= NULL;
	const int msgElemCount = 1;
	CWProtocolMessage *msgElemsBinding= NULL;
	const int msgElemBindingCount=0;
	
	if(messagesPtr == NULL || fragmentsNumPtr == NULL) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);

	CW_CREATE_PROTOCOL_MSG_ARRAY_ERR(msgElems, msgElemCount, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););	
	
	CWDebugLog("Assembling WTP Event Request AP attack Addr Redirection...");

	// Assemble Message Elements
	if (!(CWAssembleMsgElemAttackAddrRedirection(msgElems, attackAddrRedirectionInfoValues))) {
		CW_FREE_OBJECT(msgElems);
		return CW_FALSE; // error will be handled by the caller
	}
	
	if(!(CWAssembleMessage(messagesPtr,
			       fragmentsNumPtr,
			       PMTU,
			       seqNum,
			       CW_MSG_TYPE_VALUE_WTP_EVENT_REQUEST,
			       msgElems,
			       msgElemCount,
			       msgElemsBinding,
			       msgElemBindingCount,
#ifdef CW_NO_DTLS
			       CW_PACKET_PLAIN
#else
			       (gDtlsSecurity == 1)?CW_PACKET_CRYPT:CW_PACKET_PLAIN           /* 0-CW_PACKET_PLAIN, 1-CW_PACKET_CRYPT */
#endif
			       ))) 
		return CW_FALSE;
	
	CWDebugLog("WTP Event Request attack Detect Info Assembled");

	return CW_TRUE;
}

CWBool CWAssembleWTPEventStaThroughputSendtoAcValuesRequest(CWProtocolMessage **messagesPtr, int *fragmentsNumPtr, int PMTU, int seqNum, staThroughputsendtoAc *staThroughputSendtoAcValues)
{
	CWProtocolMessage *msgElems= NULL;
	const int msgElemCount = 1;
	CWProtocolMessage *msgElemsBinding= NULL;
	const int msgElemBindingCount=0;
	
	if(messagesPtr == NULL || fragmentsNumPtr == NULL) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);

	CW_CREATE_PROTOCOL_MSG_ARRAY_ERR(msgElems, msgElemCount, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););	
	
	CWDebugLog("Assembling WTP Event Request staThroughputSendtoAcValues...");

	// Assemble Message Elements
	if (!(CWAssembleMsgElemstaThroughputSendtoAcValues(msgElems, staThroughputSendtoAcValues))) {
		CW_FREE_OBJECT(msgElems);
		return CW_FALSE; // error will be handled by the caller
	}

	if(!(CWAssembleMessage(messagesPtr,
			       fragmentsNumPtr,
			       PMTU,
			       seqNum,
			       CW_MSG_TYPE_VALUE_WTP_EVENT_REQUEST,
			       msgElems,
			       msgElemCount,
			       msgElemsBinding,
			       msgElemBindingCount,
#ifdef CW_NO_DTLS
			       CW_PACKET_PLAIN
#else
			       (gDtlsSecurity == 1)?CW_PACKET_CRYPT:CW_PACKET_PLAIN           /* 0-CW_PACKET_PLAIN, 1-CW_PACKET_CRYPT */
#endif
			       ))) 
		return CW_FALSE;
	
	CWDebugLog("WTP Event Request staThroughputSendtoAcValues Assembled");

	return CW_TRUE;
	
}

CWBool CWAssembleWTPEventChallengeReplayRequest (CWProtocolMessage **messagesPtr, int *fragmentsNumPtr, int PMTU, int seqNum, challengeReplayInfo *challengeReplayInfoValues) 	//pei add 0422
{
	CWProtocolMessage *msgElems= NULL;
	const int msgElemCount = 1;
	CWProtocolMessage *msgElemsBinding= NULL;
	const int msgElemBindingCount=0;
	
	if(messagesPtr == NULL || fragmentsNumPtr == NULL) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);

	CW_CREATE_PROTOCOL_MSG_ARRAY_ERR(msgElems, msgElemCount, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););	
	
	CWDebugLog("Assembling WTP Event Request AP Challenge Replay...");

	// Assemble Message Elements
	if (!(CWAssembleMsgElemChallengeReplay(msgElems, challengeReplayInfoValues))) {
		CW_FREE_OBJECT(msgElems);
		return CW_FALSE; // error will be handled by the caller
	}
	
	if(!(CWAssembleMessage(messagesPtr,
			       fragmentsNumPtr,
			       PMTU,
			       seqNum,
			       CW_MSG_TYPE_VALUE_WTP_EVENT_REQUEST,
			       msgElems,
			       msgElemCount,
			       msgElemsBinding,
			       msgElemBindingCount,
#ifdef CW_NO_DTLS
			       CW_PACKET_PLAIN
#else
			       (gDtlsSecurity == 1)?CW_PACKET_CRYPT:CW_PACKET_PLAIN           /* 0-CW_PACKET_PLAIN, 1-CW_PACKET_CRYPT */
#endif
			       ))) 
		return CW_FALSE;
	
	CWDebugLog("WTP Event Request Challenge Replay Info Assembled");

	return CW_TRUE;
}
CWBool CWAssembleWTPEventTerminalDisturbRequest (CWProtocolMessage **messagesPtr, int *fragmentsNumPtr, int PMTU, int seqNum, terminalDisturbInfo *terminalDisturbInfoValues)
{
	CWProtocolMessage *msgElems= NULL;
	const int msgElemCount = 1;
	CWProtocolMessage *msgElemsBinding= NULL;
	const int msgElemBindingCount=0;
	
	if(messagesPtr == NULL || fragmentsNumPtr == NULL) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);

	CW_CREATE_PROTOCOL_MSG_ARRAY_ERR(msgElems, msgElemCount, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););	
	
	CWDebugLog("Assembling WTP Event Request AP Terminal Disturb...");

	// Assemble Message Elements
	if (!(CWAssembleMsgElemTerminalDisturb(msgElems, terminalDisturbInfoValues))) {
		CW_FREE_OBJECT(msgElems);
		return CW_FALSE; // error will be handled by the caller
	}
	
	if(!(CWAssembleMessage(messagesPtr,
			       fragmentsNumPtr,
			       PMTU,
			       seqNum,
			       CW_MSG_TYPE_VALUE_WTP_EVENT_REQUEST,
			       msgElems,
			       msgElemCount,
			       msgElemsBinding,
			       msgElemBindingCount,
#ifdef CW_NO_DTLS
			       CW_PACKET_PLAIN
#else
			       (gDtlsSecurity == 1)?CW_PACKET_CRYPT:CW_PACKET_PLAIN           /* 0-CW_PACKET_PLAIN, 1-CW_PACKET_CRYPT */
#endif
			       ))) 
		return CW_FALSE;
	
	CWDebugLog("WTP Event Request Challenge Replay Info Assembled");

	return CW_TRUE;
}
CWBool CWWTPGetAttackAddrRedirection(attackAddrRedirectionInfo *attackAddrRedirectionInfoValues)
{
	int i=0;
	CWProtocolMessage *messages = NULL;
	int fragmentsNum=0;
	int seqNum;
	seqNum = CWGetSeqNum();

	/* assemble WTP Event request element with info of AP attackAddrRedirectionInfo */
	if(!CWAssembleWTPEventAttackAddrRedirectionRequest(&messages, &fragmentsNum, gWTPPathMTU, seqNum, attackAddrRedirectionInfoValues))
	{
		CWWTPDebugLog("Assemble WTP Event Ap AttackAddrRedirection Request error!");
		return CW_FALSE;
	}
	
	/* send attackAddrRedirectionInfo info */
	for(i = 0; i < fragmentsNum; i++) 
	{
//		dpf("messages[%d].offset:%d\n", i, messages[i].offset);
//		CWCaptrue(messages[i].offset, (unsigned char *)(messages[i].msg));

#ifndef CW_NO_DTLS
		if(gDtlsSecurity == 1)
		{
			if(!CWSecuritySend(gWTPSession, messages[i].msg, messages[i].offset))
			{
				CWDebugLog("Error sending message\n");
				printf("Error sending message\n");
				break;  //gzm change countinue to break 091029
			}
		}
		else
#endif
		{
			if (!CWNetworkSendUnsafeUnconnected(gWTPSocket, &(gACInfoPtr->preferredAddress), messages[i].msg, messages[i].offset))
			{
				CWDebugLog("Error sending message\n");
				printf("Error sending message\n");
				break;  //gzm change countinue to break 091029
			}
		}
	}
	CWDebugLog("Message Sent\n");
	CWFreeMessageFragments(messages, fragmentsNum);
	CW_FREE_OBJECT(messages);

	return CW_TRUE;
}

CWBool CWWTPSendStaThoughputToAC(staThroughputsendtoAc *staThroughputSendtoAcValues)
{
	int i=0;
	CWProtocolMessage *messages = NULL;
	int fragmentsNum=0;
	int seqNum;
	seqNum = CWGetSeqNum();

	/* assemble WTP Event request element with info of staThroughputsendtoAcvalues */
	if(!CWAssembleWTPEventStaThroughputSendtoAcValuesRequest(&messages, &fragmentsNum, gWTPPathMTU, seqNum, staThroughputSendtoAcValues))
	{
		CWWTPDebugLog("Assemble WTP Event staThroughputSendtoAcValues Request error!");
		return CW_FALSE;
	}

	/* send staThroughputsendtoAc */
	for(i = 0; i < fragmentsNum; i++) 
	{
//		if(debug_print)
//		{
//				printf("messages[%d].offset:%d\n", i, messages[i].offset);
//				CWCaptrue(messages[i].offset, (unsigned char *)(messages[i].msg));
//		}

#ifndef CW_NO_DTLS
		if(gDtlsSecurity == 1)
		{
			if(!CWSecuritySend(gWTPSession, messages[i].msg, messages[i].offset))
			{
				CWDebugLog("Error sending message\n");
				printf("Error sending message\n");
				break;  //gzm change countinue to break 091029
			}
		}
		else
#endif
		{
			if (!CWNetworkSendUnsafeUnconnected(gWTPSocket, &(gACInfoPtr->preferredAddress), messages[i].msg, messages[i].offset))
			{
				CWDebugLog("Error sending message\n");
				printf("Error sending message\n");
				break;  //gzm change countinue to break 091029
			}
		}
	}
	CWDebugLog("Message Sent\n");
	CWFreeMessageFragments(messages, fragmentsNum);
	CW_FREE_OBJECT(messages);

	return CW_TRUE;
}

CWBool CWWTPGetChallengeReplay(challengeReplayInfo *challengeReplayInfoValues)
{
	int i=0;
	CWProtocolMessage *messages = NULL;
	int fragmentsNum=0;
	int seqNum;
	seqNum = CWGetSeqNum();

	/* assemble WTP Event request element with info of AP ChallengeReplay */
	if(!CWAssembleWTPEventChallengeReplayRequest(&messages, &fragmentsNum, gWTPPathMTU, seqNum, challengeReplayInfoValues))
	{
		CWWTPDebugLog("Assemble WTP Event Ap ChallengeReplay Request error!");
		return CW_FALSE;
	}
	
	/* send ChallengeReplay info */
	for(i = 0; i < fragmentsNum; i++) 
	{
//		dpf("messages[%d].offset:%d\n", i, messages[i].offset);
//		CWCaptrue(messages[i].offset, (unsigned char *)(messages[i].msg));

#ifndef CW_NO_DTLS
		if(gDtlsSecurity == 1)
		{
			if(!CWSecuritySend(gWTPSession, messages[i].msg, messages[i].offset))
			{
				CWDebugLog("Error sending message\n");
				printf("Error sending message\n");
				break;  //gzm change countinue to break 091029
			}
		}
		else
#endif
		{
			if (!CWNetworkSendUnsafeUnconnected(gWTPSocket, &(gACInfoPtr->preferredAddress), messages[i].msg, messages[i].offset))
			{
				CWDebugLog("Error sending message\n");
				printf("Error sending message\n");
				break;  //gzm change countinue to break 091029
			}
		}
	}
	CWDebugLog("Message Sent\n");
	CWFreeMessageFragments(messages, fragmentsNum);
	CW_FREE_OBJECT(messages);

	return CW_TRUE;
}
CWBool CWWTPGetTerminalDisturb(terminalDisturbInfo *terminalDisturbInfoValues)
{
	int i=0;
	CWProtocolMessage *messages = NULL;
	int fragmentsNum=0;
	int seqNum;
	seqNum = CWGetSeqNum();

	/* assemble WTP Event request element with info of AP terminalDisturbInfoValues */
	if(!CWAssembleWTPEventTerminalDisturbRequest(&messages, &fragmentsNum, gWTPPathMTU, seqNum, terminalDisturbInfoValues))
	{
		CWWTPDebugLog("Assemble WTP Event Ap terminalDisturb Request error!");
		return CW_FALSE;
	}
	
	/* send terminalDisturbInfoValues info */
	for(i = 0; i < fragmentsNum; i++) 
	{
//		dpf("messages[%d].offset:%d\n", i, messages[i].offset);
//		CWCaptrue(messages[i].offset, (unsigned char *)(messages[i].msg));

#ifndef CW_NO_DTLS
		if(gDtlsSecurity == 1)
		{
			if(!CWSecuritySend(gWTPSession, messages[i].msg, messages[i].offset))
			{
				CWDebugLog("Error sending message\n");
				printf("Error sending message\n");
				break;  //gzm change countinue to break 091029
			}
		}
		else
#endif
		{
			if (!CWNetworkSendUnsafeUnconnected(gWTPSocket, &(gACInfoPtr->preferredAddress), messages[i].msg, messages[i].offset))
			{
				CWDebugLog("Error sending message\n");
				printf("Error sending message\n");
				break;  //gzm change countinue to break 091029
			}
		}
	}
	CWDebugLog("Message Sent\n");
	CWFreeMessageFragments(messages, fragmentsNum);
	CW_FREE_OBJECT(messages);

	return CW_TRUE;
}
static void driver_madwifi_wireless_event_wireless(char *data, int len, unsigned char *mac)
{	
	char *str = NULL;
	
	NetLink_Type netLink_Type;
	
	str = CWParseNetLinkBuffer(&netLink_Type,data,len,mac); //malloc
	
	if(netLink_Type == NT_T_AttackAddrRedirection)
	{
		CWWTPGetAttackAddrRedirection((attackAddrRedirectionInfo *)str);
		CW_FREE_OBJECT(str); //free
	}
	else if (netLink_Type == NT_T_ChallengeReplay)
	{
		CWWTPGetAttackAddrRedirection((/*challengeReplayInfo--del warning*/attackAddrRedirectionInfo *)str);
		CW_FREE_OBJECT(str); //free
	}
	else if (netLink_Type == NT_T_TerminalDisturbDetect)
	{
		terminalDisturbInfo * str2 = (terminalDisturbInfo*)str;
		CWWTPGetTerminalDisturb(str2);
		CW_FREE_OBJECT(str2->sta_mac);
		CW_FREE_OBJECT(str2);
	}
	else if (netLink_Type == NT_T_TerminalDisturbClear )
	{
		terminalDisturbInfo *str2 = (terminalDisturbInfo *)str;
		CWWTPGetTerminalDisturb(str2);
		CW_FREE_OBJECT(str2->sta_mac);
		CW_FREE_OBJECT(str2);
	}
	else if (netLink_Type == NT_T_AttackDetect )
	{
		attackDetectInfo *str2 = (attackDetectInfo *)str;
		if(str2->count != 0)
			CWWTPGetAttackDetectInfo(str2);
		CW_FREE_OBJECT(str2);
	}
	else if (netLink_Type == NT_T_StationAging)
	{
		return ;
	}
	else if (netLink_Type == NT_T_Sta_Traffic_Limit)
	{
		staThroughputsendtoAc *str2 = (staThroughputsendtoAc *)str;
		CWWTPSendStaThoughputToAC(str2);
		CW_FREE_OBJECT(str2);
	}
	else
		return;
}
#if 0
static void driver_madwifi_wireless_event_wireless_old(char *data, int len, unsigned char *mac)
{
	struct iw_event iwe_buf, *iwe = &iwe_buf;
	char *pos, *end, *custom;
	int i = 0;
	unsigned char vap_mac[MAC_ADDR_LEN];
	terminalDisturbInfo terminalDisturbValues;
	unsigned char wireless_type = 0;
	unsigned char tbuf[128];
	
	/* pei add start, for wapi attack addr redirection trap */
	CWWTPWlan *ptr = NULL;
	CWWTPSta *staptr = NULL;
	char sta_mac[6];
	asso_mt *asso_mt_info ;
	attackAddrRedirectionInfo attackAddrRdValues;
	staThroughputsendtoAc staThpvalues;
	challengeReplayInfo challengeReplayValues;
	const u16 ADDR_RD_TRAP = 0x00F8;  /* 地址重定向*/
	const u16 CHALLENGE_REPLAY_TRAP = 0x00FF;
	asso_mt_info = (asso_mt *)(data+IW_EV_POINT_LEN);

	for(ptr=wtp_wlan_list;ptr != NULL;ptr = ptr->next)
	{
		if ((0 == memcmp(ptr->wlan_bssid, asso_mt_info->ap_mac, 6))&&(asso_mt_info->type == ADDR_RD_TRAP))
		{
			dpf("attack addr redirection [%02x-%02x-%02x-%02x-%02x-%02x]\n", asso_mt_info->mac[0], 
				asso_mt_info->mac[1], asso_mt_info->mac[2], asso_mt_info->mac[3], asso_mt_info->mac[4],
				asso_mt_info->mac[5]);
			attackAddrRdValues.value = ATTACK_ADDR_RD;
			attackAddrRdValues.count = 1;
			attackAddrRdValues.radio_id = ptr->radio_id;
			attackAddrRdValues.wlan_id = ptr->wlan_id;
			attackAddrRdValues.state = 1;
			memcpy(attackAddrRdValues.sta_mac, asso_mt_info->mac, MAC_ADDR_LEN);
			CWWTPGetAttackAddrRedirection(&attackAddrRdValues);
		}
		if ((0 == memcmp(ptr->wlan_bssid, asso_mt_info->ap_mac, 6))&&(asso_mt_info->type == CHALLENGE_REPLAY_TRAP))
		{
			dpf("challenge replay [%02x-%02x-%02x-%02x-%02x-%02x]\n", asso_mt_info->mac[0], 
				asso_mt_info->mac[1], asso_mt_info->mac[2], asso_mt_info->mac[3], asso_mt_info->mac[4],
				asso_mt_info->mac[5]);
			challengeReplayValues.value = CHALLENGE_REPLAY;
			challengeReplayValues.count = 1;
			challengeReplayValues.radio_id = ptr->radio_id;
			challengeReplayValues.wlan_id = ptr->wlan_id;
			challengeReplayValues.state = 1;
			memcpy(challengeReplayValues.sta_mac, asso_mt_info->mac, MAC_ADDR_LEN);
			CWWTPGetChallengeReplay(&challengeReplayValues);
		}
	}
	if(access("/jffs/attack_ad_rd", R_OK)==0)
	{
		dpf("simulate attack addr redirection...\n");
		attackAddrRdValues.value = ATTACK_ADDR_RD;
		attackAddrRdValues.count = 1;
		attackAddrRdValues.radio_id = 0;
		attackAddrRdValues.wlan_id = 1;
		attackAddrRdValues.state = 1;
		if(wtp_sta_list)
		{
			for(staptr = wtp_sta_list; staptr != NULL; staptr = staptr->next)
			{
				memcpy(sta_mac, staptr->sta_mac, 6);
				memcpy(attackAddrRdValues.sta_mac, staptr->sta_mac, 6);
			}
		}
		else
		{
			attackAddrRdValues.sta_mac[0] = 0x00;
			attackAddrRdValues.sta_mac[1] = 0x1f;
			attackAddrRdValues.sta_mac[2] = 0x64;
			attackAddrRdValues.sta_mac[3] = 0x11;
			attackAddrRdValues.sta_mac[4] = 0x22;
			attackAddrRdValues.sta_mac[5] = 0x33;
		}
		CWWTPGetAttackAddrRedirection(&attackAddrRdValues);
	}
	if(access("/jffs/attack_ad_rd1", R_OK)==0)
	{
		dpf("simulate1 attack addr redirection...\n");
		attackAddrRdValues.value = ATTACK_ADDR_RD;
		attackAddrRdValues.count = 1;
		attackAddrRdValues.radio_id = 0;
		attackAddrRdValues.wlan_id = 1;
		attackAddrRdValues.state = 0;
		if(wtp_sta_list)
		{
			memcpy(attackAddrRdValues.sta_mac, sta_mac, 6);
		}
		else
		{
			attackAddrRdValues.sta_mac[0] = 0x00;
			attackAddrRdValues.sta_mac[1] = 0x1f;
			attackAddrRdValues.sta_mac[2] = 0x64;
			attackAddrRdValues.sta_mac[3] = 0x11;
			attackAddrRdValues.sta_mac[4] = 0x22;
			attackAddrRdValues.sta_mac[5] = 0x33;
		}
		CWWTPGetAttackAddrRedirection(&attackAddrRdValues);
	}
	/* pei add end, for wapi attack addr redirection trap */
#if 0
	/* pei add start, for terminal disturb trap */
	char sta[MAC_ADDR_LEN];
	terminalDisturbInfo terminalDisturbValues;
	if(access("/jffs/terminal_disturb1", R_OK)==0)
	{
		terminalDisturbValues.value = TERMINAL_DISTURB;
		terminalDisturbValues.radio_id = 0;
		terminalDisturbValues.wlan_id = 1;
		terminalDisturbValues.state = 1;
		terminalDisturbValues.sta_count = 2;
		sta[0] = 0x00;
		sta[1] = 0x1f;
		sta[2] = 0x64;
		sta[3] = 0x11;
		sta[4] = 0x22;
		sta[5] = 0x33;
		CW_CREATE_ARRAY_ERR(terminalDisturbValues.sta_mac, 6*terminalDisturbValues.sta_count, char, CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
		memcpy(terminalDisturbValues.sta_mac, sta, MAC_ADDR_LEN);
		sta[5] = 0x44;
		memcpy(terminalDisturbValues.sta_mac+6, sta, MAC_ADDR_LEN);
		CWWTPGetTerminalDisturb(&terminalDisturbValues);
		CW_FREE_OBJECT(terminalDisturbValues.sta_mac);
	}
	if(access("/jffs/terminal_disturb2", R_OK)==0)
	{
		terminalDisturbValues.value = TERMINAL_DISTURB;
		terminalDisturbValues.radio_id = 0;
		terminalDisturbValues.wlan_id = 1;
		terminalDisturbValues.state = 0;
		terminalDisturbValues.sta_count = 2;
		sta[0] = 0x00;
		sta[1] = 0x1f;
		sta[2] = 0x64;
		sta[3] = 0x11;
		sta[4] = 0x22;
		sta[5] = 0x33;
		CW_CREATE_ARRAY_ERR(terminalDisturbValues.sta_mac, 6*terminalDisturbValues.sta_count, char, CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
		memcpy(terminalDisturbValues.sta_mac, sta, MAC_ADDR_LEN);
		sta[5] = 0x44;
		memcpy(terminalDisturbValues.sta_mac+6, sta, MAC_ADDR_LEN);
		CWWTPGetTerminalDisturb(&terminalDisturbValues);
		CW_FREE_OBJECT(terminalDisturbValues.sta_mac);
	}
	/* pei add end, for terminal disturb trap */
#endif
	pos = data;
	end = data + len;

//	printf("-----entering the driver_madwifi_wireless_event_wireless()\n");
	while (pos + IW_EV_LCP_LEN <= end) {
		/* Event data may be unaligned, so make a local, aligned copy
		 * before processing. */
		memcpy(&iwe_buf, pos, IW_EV_LCP_LEN);
		CWDebugLog("Wireless event: "
			      "cmd=0x%x len=%d\n", iwe->cmd, iwe->len);
		if (iwe->len <= IW_EV_LCP_LEN)
			return;

		custom = pos + IW_EV_POINT_LEN;
		
		memcpy(&iwe_buf, pos, sizeof(struct iw_event));
		custom += IW_EV_POINT_OFF;

		switch (iwe->cmd) {
			case IWEVEXPIRED:
				CWDebugLog("station aging!\n");
				memcpy(mac, (unsigned char *)iwe->u.addr.sa_data, 6);
//				madwifi_del_sta(drv, (u8 *) iwe->u.addr.sa_data);
			break;
			case IWEVTRAFFIC:
				CWWTPDebugLog("station up/down line");
				memset(&staThpvalues,0,sizeof(staThpvalues));
				staThpvalues.optype = STATHROUGHPUT_OPTYPE;
				staThpvalues.len = 33;
				memcpy(staThpvalues.sta_mac, (unsigned char *)iwe->u.addr.sa_data, 6);
				if(!(get_sta_throughput_info(&staThpvalues)))
					break;
				CWWTPSendStaThoughputToAC(&staThpvalues);
			break;
			case 0x8c0a:  /* pei add for attack detect, at 090625 */
				if(debug_print)
				{
					printf("iwe->len:%d   data-len:%d\n", iwe->len, iwe->u.data.length);
					printf("data:");
					for(i=0;i<iwe->len;i++)
						printf("%02x ", (unsigned char)*(pos+i));
					printf("\n");
				}
				wireless_type = (unsigned char)*(pos+IW_EV_POINT_LEN);
				if(debug_print)
					printf("wireless_type:%d\n", wireless_type);
				switch(wireless_type)
				{
					case IEEE80211_STA_INTERFER_DETECT:
						memset(vap_mac, 0, MAC_ADDR_LEN);
						memset(&terminalDisturbValues, 0, sizeof(terminalDisturbInfo));
						terminalDisturbValues.value = TERMINAL_DISTURB;
						terminalDisturbValues.state = 1;
						terminalDisturbValues.sta_count = (unsigned char)*(pos+IW_EV_POINT_LEN+2);
						CW_CREATE_ARRAY_ERR(terminalDisturbValues.sta_mac, MAC_ADDR_LEN*terminalDisturbValues.sta_count, unsigned char, CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
						memcpy(terminalDisturbValues.sta_mac, pos+IW_EV_POINT_LEN+3, MAC_ADDR_LEN*terminalDisturbValues.sta_count);
						memcpy(vap_mac, pos+IW_EV_POINT_LEN+3+MAC_ADDR_LEN*terminalDisturbValues.sta_count, MAC_ADDR_LEN);
						for(ptr=wtp_wlan_list;ptr != NULL;ptr = ptr->next)
						{
							if (0 == memcmp(ptr->wlan_bssid, vap_mac, 6))
							{
								terminalDisturbValues.radio_id = ptr->radio_id;
								terminalDisturbValues.wlan_id = ptr->wlan_id;
							}
						}
						memset(tbuf, 0, 128);
						tbuf[0] = terminalDisturbValues.sta_count;
						memcpy(tbuf+1, pos+IW_EV_POINT_LEN+3, MAC_ADDR_LEN*terminalDisturbValues.sta_count);
						for(i=0;i<MAC_ADDR_LEN*tbuf[0]+1;i++)
						{
							printf("%02x", tbuf[i]);
						}
						printf("\n");
						CWWTPGetTerminalDisturb(&terminalDisturbValues);
						CW_FREE_OBJECT(terminalDisturbValues.sta_mac);
					break;
					case IEEE80211_STA_INTERFER_CLEAR:
						memset(vap_mac, 0, MAC_ADDR_LEN);
						memset(&terminalDisturbValues, 0, sizeof(terminalDisturbInfo));
						terminalDisturbValues.value = TERMINAL_DISTURB;
						terminalDisturbValues.state = 0;
						for(i=0;i<MAC_ADDR_LEN*tbuf[0]+1;i++)
						{
							printf("%02x", tbuf[i]);
						}
						printf("\n");
						terminalDisturbValues.sta_count = tbuf[0];
						CW_CREATE_ARRAY_ERR(terminalDisturbValues.sta_mac, MAC_ADDR_LEN*terminalDisturbValues.sta_count, unsigned char, CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
						memcpy(terminalDisturbValues.sta_mac, tbuf+1, MAC_ADDR_LEN*terminalDisturbValues.sta_count);
						memcpy(vap_mac, pos+IW_EV_POINT_LEN+2, MAC_ADDR_LEN);
						for(ptr=wtp_wlan_list;ptr != NULL;ptr = ptr->next)
						{
							if (0 == memcmp(ptr->wlan_bssid, vap_mac, 6))
							{
								terminalDisturbValues.radio_id = ptr->radio_id;
								terminalDisturbValues.wlan_id = ptr->wlan_id;
							}
						}
						CWWTPGetTerminalDisturb(&terminalDisturbValues);
						CW_FREE_OBJECT(terminalDisturbValues.sta_mac);
					break;
					default:
					break;
				}

				if(debug_print)
				{
					printf("iwe->len:%d   data-len:%d\n", iwe->len, iwe->u.data.length);
					printf("data:");
					//for(i=0;i<iwe->len;i++)
					//	printf("%02x ", (unsigned char)*(pos+i));
					//printf("\n");
				}
				unsigned char buf[16];
				for(i=0;i<16;i++)
				{
					buf[i] = (unsigned char)*(pos+iwe->len-16+i);
					if(debug_print)
						printf("%02x ", (unsigned char)*(pos+iwe->len-16+i));
				}
				if(debug_print)
					printf("\n");
				CWWTPDebugLog("data-len:%d", iwe->u.data.length);
				CWWTPDebugLog("data:%02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x",
					buf[0], buf[1], buf[2], buf[3], buf[4], buf[5], buf[6], buf[7], buf[8], buf[9], buf[10], buf[11], buf[12], buf[13], buf[14], buf[15]);
				

				{
					attackDetectInfo attackDetectInfoValues;
					attackDetectInfoValues.value = ATTACK_DETECT_INFO;
					attackDetectInfoValues.count = 0;
					attackDetectInfoValues.is_flood_attack = 0;
					attackDetectInfoValues.is_spoof_attack = 0;
					attackDetectInfoValues.is_weakiv_attack = 0;
					//if(gFloodingDetectEnable||gSpoofingDetectEnable||gWeakIVDetectEnable)
					if(gFloodingDetectEnable&&(buf[0]) == 1)
					{
						attackDetectInfoValues.count += 1;
						attackDetectInfoValues.is_flood_attack = 1;
						attackDetectInfoValues.flood_detect_type = IEEE80211_WIDS_FLOOD_DETECT;
						memcpy(attackDetectInfoValues.flood_sta_mac, &buf[1], 6);
						memcpy(attackDetectInfoValues.flood_vap_mac, &buf[7], 6);
						attackDetectInfoValues.flood_frame_type = buf[13];
						attackDetectInfoValues.flood_channel = buf[14];
						attackDetectInfoValues.flood_rssi = buf[15];
					}
					if(gSpoofingDetectEnable&&(buf[0]) == 2)
					{
						attackDetectInfoValues.count += 1;
						attackDetectInfoValues.is_spoof_attack = 1;
						attackDetectInfoValues.spoof_detect_type = IEEE80211_WIDS_SPOOF_DETECT;
						memcpy(attackDetectInfoValues.spoof_sta_mac, &buf[1], 6);
						memcpy(attackDetectInfoValues.spoof_vap_mac, &buf[7], 6);
						attackDetectInfoValues.spoof_frame_type = buf[13];
						attackDetectInfoValues.spoof_channel = buf[14];
						attackDetectInfoValues.spoof_rssi = buf[15];
					}
					if(gWeakIVDetectEnable&&(buf[0]) == 3)
					{
						attackDetectInfoValues.count += 1;
						attackDetectInfoValues.is_weakiv_attack = 1;
						attackDetectInfoValues.weakiv_detect_type = IEEE80211_WIDS_WEAKIV_DETECT;
						memcpy(attackDetectInfoValues.weakiv_sta_mac, &buf[1], 6);
						memcpy(attackDetectInfoValues.weakiv_vap_mac, &buf[7], 6);
						attackDetectInfoValues.weakiv_payload = buf[13];
						attackDetectInfoValues.weakiv_channel = buf[14];
						attackDetectInfoValues.weakiv_rssi = buf[15];
					}
					if(attackDetectInfoValues.count != 0)
						CWWTPGetAttackDetectInfo(&attackDetectInfoValues);
				}
#if 0
				if(gFloodingDetectEnable||gSpoofingDetectEnable||gWeakIVDetectEnable)
				{
					printf("attacking detect is enabled.......\n");
					attackDetectInfo attackDetectInfoValues;
					attackDetectInfoValues.value = ATTACK_DETECT_INFO;
					attackDetectInfoValues.count = 0;
					attackDetectInfoValues.is_flood_attack = 0;
					attackDetectInfoValues.is_spoof_attack = 0;
					attackDetectInfoValues.is_weakiv_attack = 0;
					if(access("/jffs/flood_detect", R_OK)==0)
					{
						attackDetectInfoValues.count += 1;
						attackDetectInfoValues.is_flood_attack = 1;
						attackDetectInfoValues.flood_detect_type = IEEE80211_WIDS_FLOOD_DETECT;
						//attackDetectInfoValues.flood_sta_mac = {0x11,0x22,0x33,0x44,0x55,0x66};
						attackDetectInfoValues.flood_sta_mac[0] = 0x11;
						attackDetectInfoValues.flood_sta_mac[1] = 0x22;
						attackDetectInfoValues.flood_sta_mac[2] = 0x33;
						attackDetectInfoValues.flood_sta_mac[3] = 0x44;
						attackDetectInfoValues.flood_sta_mac[4] = 0x55;
						attackDetectInfoValues.flood_sta_mac[5] = 0x66;
						attackDetectInfoValues.flood_vap_mac[0] = 0x01;
						attackDetectInfoValues.flood_vap_mac[1] = 0x12;
						attackDetectInfoValues.flood_vap_mac[2] = 0x23;
						attackDetectInfoValues.flood_vap_mac[3] = 0x34;
						attackDetectInfoValues.flood_vap_mac[4] = 0x45;
						attackDetectInfoValues.flood_vap_mac[5] = 0x56;
						attackDetectInfoValues.flood_frame_type = 2;
						attackDetectInfoValues.flood_channel = 2;
						attackDetectInfoValues.flood_rssi = 2;
					}
					if(access("/jffs/spoof_detect", R_OK)==0)
					{
						attackDetectInfoValues.count += 1;
						attackDetectInfoValues.is_spoof_attack = 1;
						attackDetectInfoValues.spoof_detect_type = IEEE80211_WIDS_SPOOF_DETECT;
						//attackDetectInfoValues.spoof_sta_mac = {0x11,0x22,0x33,0x44,0x55,0x66};
						attackDetectInfoValues.spoof_sta_mac[0] = 0x01;
						attackDetectInfoValues.spoof_sta_mac[1] = 0x12;
						attackDetectInfoValues.spoof_sta_mac[2] = 0x23;
						attackDetectInfoValues.spoof_sta_mac[3] = 0x34;
						attackDetectInfoValues.spoof_sta_mac[4] = 0x45;
						attackDetectInfoValues.spoof_sta_mac[5] = 0x56;
						attackDetectInfoValues.spoof_vap_mac[0] = 0x11;
						attackDetectInfoValues.spoof_vap_mac[1] = 0x22;
						attackDetectInfoValues.spoof_vap_mac[2] = 0x33;
						attackDetectInfoValues.spoof_vap_mac[3] = 0x44;
						attackDetectInfoValues.spoof_vap_mac[4] = 0x55;
						attackDetectInfoValues.spoof_vap_mac[5] = 0x66;
						attackDetectInfoValues.spoof_frame_type = 3;
						attackDetectInfoValues.spoof_channel = 3;
						attackDetectInfoValues.spoof_rssi = 3;
					}
					if(access("/jffs/weakiv_detect", R_OK)==0)
					{
						attackDetectInfoValues.count += 1;
						attackDetectInfoValues.is_weakiv_attack = 1;
						attackDetectInfoValues.weakiv_detect_type = IEEE80211_WIDS_WEAKIV_DETECT;
						//attackDetectInfoValues.weakiv_sta_mac = {0x11,0x22,0x33,0x44,0x55,0x66};
						attackDetectInfoValues.weakiv_sta_mac[0] = 0x21;
						attackDetectInfoValues.weakiv_sta_mac[1] = 0x32;
						attackDetectInfoValues.weakiv_sta_mac[2] = 0x43;
						attackDetectInfoValues.weakiv_sta_mac[3] = 0x54;
						attackDetectInfoValues.weakiv_sta_mac[4] = 0x65;
						attackDetectInfoValues.weakiv_sta_mac[5] = 0x06;
						attackDetectInfoValues.weakiv_vap_mac[0] = 0x11;
						attackDetectInfoValues.weakiv_vap_mac[1] = 0x22;
						attackDetectInfoValues.weakiv_vap_mac[2] = 0x33;
						attackDetectInfoValues.weakiv_vap_mac[3] = 0x44;
						attackDetectInfoValues.weakiv_vap_mac[4] = 0x55;
						attackDetectInfoValues.weakiv_vap_mac[5] = 0x66;
						attackDetectInfoValues.weakiv_payload = 4;
						attackDetectInfoValues.weakiv_channel = 4;
						attackDetectInfoValues.weakiv_rssi = 4;
					}
					if(attackDetectInfoValues.count != 0)
						CWWTPGetAttackDetectInfo(&attackDetectInfoValues);
				}
#endif
				
			break;
			default:
			break;
#if 0/*csc add 080716*/
			case IWEVREGISTERED:
//				madwifi_delete_key();
//				memcpy(mac, (unsigned char *) iwe->u.addr.sa_data, 6);
				printf("new station \n");		
				int i,ret;
				char wlanname[IFNAMSIZ];
				int sock = -1;
				CWWTPWlan *ptr = NULL;
				sock = socket(AF_INET, SOCK_DGRAM, 0);
				if (sock < 0) {
					CWDebugLog("Error Creating Socket for ioctl"); 
					return CWErrorRaise(CW_ERROR_GENERAL, NULL);;
				}
				struct ieee80211req_mlme mlme;
				memset(&mlme, 0, sizeof(mlme));
				mlme.im_op = IEEE80211_MLME_UNAUTHORIZE;
				CWDebugLog("mlme.im_op %d",mlme.im_op);
				mlme.im_reason = 0;
				memcpy(mlme.im_macaddr, (unsigned char *) iwe->u.addr.sa_data, MAC_ADDR_LEN);			
#if 1		
	printf("\nMAC:\n");
	for(i=0;i<6;i++){
		printf("%02x ",mlme.im_macaddr[i]);
	}
#endif	
			for(ptr=wtp_wlan_list;ptr != NULL;ptr = ptr->next){
				CWWTPGetWlanName(wlanname,ptr->wlan_id);
				CWDebugLog("unauthorize wlanname %s\n",wlanname);
				ret = madwifi_del_key(sock,wlanname,(unsigned char *) iwe->u.addr.sa_data,0);
				ret = set80211priv(sock,wlanname,IEEE80211_IOCTL_SETMLME, &mlme, sizeof(mlme));
				break;/*any ath id OK*/
			}
			
			break;
#endif
		}

		pos += iwe->len;
	}
}
#endif
static void driver_madwifi_wireless_event_rtm_newlink(struct nlmsghdr *h, int len, unsigned char *mac)
{
	struct ifinfomsg *ifi;
	int attrlen, nlmsg_len, rta_len;
	struct rtattr * attr;
//	printf("-----entering the driver_madwifi_wireless_event_rtm_newlink()\n");
	if (len < (int) sizeof(*ifi))
		return;

	ifi = NLMSG_DATA(h);

//	if (ifi->ifi_index != drv->ifindex)
//		return;

	nlmsg_len = NLMSG_ALIGN(sizeof(struct ifinfomsg));

	attrlen = h->nlmsg_len - nlmsg_len;

	if (attrlen < 0)
		return;

	attr = (struct rtattr *) (((char *) ifi) + nlmsg_len);

	rta_len = RTA_ALIGN(sizeof(struct rtattr));
	while (RTA_OK(attr, attrlen)) {
		if (attr->rta_type == IFLA_WIRELESS) {
//			printf("attr->rta_type == IFLA_WIRELESS\n");
			driver_madwifi_wireless_event_wireless(
				((char *) attr) + rta_len,
				attr->rta_len - rta_len, mac);
		}
		attr = RTA_NEXT(attr, attrlen);
	}
}


static void driver_madwifi_wireless_event_receive(int sock, unsigned char *mac)
{
	char buf[256];
	int left;
	struct sockaddr_nl from;
	socklen_t fromlen;
	struct nlmsghdr *h;
	int ret;
	fd_set myset;

	fromlen = sizeof(from);
//	printf("-----entering the driver_madwifi_wireless_event_receive()\n");
//	printf("fromlen = %d\n", fromlen);

	FD_ZERO(&myset);
	FD_SET(sock,&myset);
	ret = select((sock+1), &myset, NULL, NULL, NULL);
	if(ret<0){
		CWDebugLog("select error\n");
		return;
	}
	else
	{
		if(FD_ISSET(sock, &myset))
		{
			left = recvfrom(sock, buf, sizeof(buf), MSG_DONTWAIT,
					(struct sockaddr *) &from, &fromlen);
//			printf("left = %d\n", left);
			if (left < 0) {
//				printf("errno = %d\n", errno);
				if (errno != EINTR && errno != EAGAIN)
					perror("recvfrom(netlink)");
				return;
			}
//			printf("(int) sizeof(*h) = %d\n", (int) sizeof(*h));
			h = (struct nlmsghdr *) buf;
			while (left >= (int) sizeof(*h)) {
				int len, plen;
				len = h->nlmsg_len;
				plen = len - sizeof(*h);
				if (len > left || plen < 0) {
					CWDebugLog("Malformed netlink message: "
						       "len=%d left=%d plen=%d\n",
						       len, left, plen);
					break;
				}

				switch (h->nlmsg_type) {
				case RTM_NEWLINK:
					driver_madwifi_wireless_event_rtm_newlink(h, plen, mac);
					break;
				}

				len = NLMSG_ALIGN(len);
				left -= len;
				h = (struct nlmsghdr *) ((char *) h + len);
			}

			if (left > 0) {
				CWDebugLog("%d extra bytes in the end of netlink message\n", left);
				}
		}
	}
	return;
}

CWBool CWWTPCheckNetLinkForBackUp(void)
{
	int radioid = 0 ;
	char pathbuf[128];
	FILE *fd = NULL;
	unsigned char buf[64];
	unsigned int mac_addr[6];
	CWWTPSta *ptr = NULL;
	int ToSend  ;
	char mactmp [64];
	
 
	for (radioid=0;radioid <= 1;radioid ++)
	{
		memset(pathbuf,0,128);
		sprintf(pathbuf,"%s%d/sta_mac",CONF_RADIO_PATH,radioid);	
		if((fd=fopen(pathbuf,"r"))==NULL)
		{
			dpf("-----------open file %s failed!\n",pathbuf);
			continue;
		}
		else
		{
			memset(buf,0,64);
			while (fgets((char *)buf, sizeof(buf),fd))
			{	
				dpf("-------------------------------------------------\n");
				memset(mac_addr,0,6);
				ToSend=1;
				buf[strlen((char *)buf)-1] = '\0';
				sscanf((char *)buf, "%x:%x:%x:%x:%x:%x",&mac_addr[0],&mac_addr[1],&mac_addr[2],&mac_addr[3],&mac_addr[4],&mac_addr[5]);
				dpf("the buf mac is %02x:%02x:%02x:%02x:%02x:%02x\n",mac_addr[0],mac_addr[1],mac_addr[2],mac_addr[3],mac_addr[4],mac_addr[5]);

				if(wtp_sta_list != NULL)
				{
					for(ptr = wtp_sta_list;ptr != NULL; ptr = ptr->next)
					{	
						memset(mactmp,0,64);
						sprintf(mactmp,"%02x:%02x:%02x:%02x:%02x:%02x",ptr->sta_mac[0],ptr->sta_mac[1],ptr->sta_mac[2]
																	,ptr->sta_mac[3],ptr->sta_mac[4],ptr->sta_mac[5]);
						if(strcmp(mactmp,(char *)buf)==0)
						{	
							dpf("the same mac_addr is %02x:%02x:%02x:%02x:%02x:%02x\n", mac_addr[0],mac_addr[1],mac_addr[2],mac_addr[3],mac_addr[4],mac_addr[5]);
							dpf("the sta  is not quit,the mac is %s\n",mactmp);
							ToSend = 0 ;
						}
					}
				}
				if(ToSend == 1)
				{
					
					int seqNum;
					int i = 0;
					DeleteSTAValues values;
					CWProtocolMessage *messages = NULL;
					int fragmentsNum=0;
					values.mac_length = 6;
					CW_CREATE_OBJECT_SIZE_ERR(values.mac_addr, values.mac_length, {CWWTPDebugLog("create memory error in CWWTPCheckNetLink");return CW_FALSE;});
					for(i = 0;i<6;i++)
					{
						values.mac_addr[i] = mac_addr[i];
					}
					dpf("-------the delete mac is %02x:%02x:%02x:%02x:%02x:%02x \n",values.mac_addr[0],values.mac_addr[1],values.mac_addr[2],values.mac_addr[3],values.mac_addr[4],values.mac_addr[5]);
					values.radio_id = radioid;
					seqNum = CWGetSeqNum();
					if(!CWAssembleWTPEventDeleteStationRequest(&messages, &fragmentsNum, gWTPPathMTU, seqNum, &values))
					{
						CWWTPDebugLog("Station aging AssembleWTPEventDeleteStationRequest error!");
						return CW_FALSE;
					}
					CW_FREE_OBJECT(values.mac_addr);
			
					for(i = 0; i < fragmentsNum; i++) 
					{
			//			if(debug_print)
			//			{
			//				printf("messages[%d].offset:%d\n", i, messages[i].offset);
			//				CWCaptrue(messages[i].offset, (unsigned char *)(messages[i].msg));
			//			}

#ifndef CW_NO_DTLS
						if(gDtlsSecurity == 1)
						{
							if(!CWSecuritySend(gWTPSession, messages[i].msg, messages[i].offset))
							{
								CWDebugLog("Error sending message\n");
								CWWTPDebugLog("Error sending deleted sta info message");
								break;
							}
						}
						else
#endif
						{
							if (!CWNetworkSendUnsafeUnconnected(gWTPSocket, &(gACInfoPtr->preferredAddress), messages[i].msg, messages[i].offset))
							{
								CWDebugLog("Error sending message\n");
								CWWTPDebugLog("Error sending deleted sta info message");
								break;
							}
						}
					}

					CWDebugLog("Message Sent\n");
					CWFreeMessageFragments(messages, fragmentsNum);
					CW_FREE_OBJECT(messages);
				}
				memset(buf,0,64);
				dpf("-------------------------------------------------\n");
			}
			fclose(fd);
		}
		STAMacRefreshForBackup(radioid);
	}
	return 0;
}

CW_THREAD_RETURN_TYPE CWWTPCheckNetLink(void *arg)
{
	if(pthread_detach(pthread_self())!=0)
	printf("##########detach error!############\n");

	int sock;
	struct sockaddr_nl local;
	unsigned char mac[6];

	sock = socket(PF_NETLINK, SOCK_RAW, NETLINK_ROUTE);

	if (sock < 0) {
		CWWTPDebugLog("%s,create socket failed.",__func__);
		perror("socket(PF_NETLINK,SOCK_RAW,NETLINK_ROUTE)");
		CWExitThread();
	}

	memset(&local, 0, sizeof(local));
	local.nl_family = AF_NETLINK;
	local.nl_groups = RTMGRP_LINK;
	if (bind(sock, (struct sockaddr *) &local, sizeof(local)) < 0) {
		perror("bind(netlink)");
		close(sock);
		CWExitThread();
	}
	
	memset(g_DelStaMac, 0, 6);
	CW_REPEAT_FOREVER
	{
		memset(mac, 0, 6);
		driver_madwifi_wireless_event_receive(sock, mac);
		if((mac[0] == 0)&&(mac[1] == 0)&&(mac[2] == 0)&&(mac[3] == 0)&&(mac[4] == 0)&&(mac[5] ==0))
		{
		}
		else
		{
			int i;
			CWWTPDebugLog("there is a station aging:[%02x-%02x-%02x-%02x-%02x-%02x]", mac[0],mac[1],mac[2],mac[3],mac[4],mac[5]);
			int seqNum;
			DeleteSTAValues values;
			CWProtocolMessage *messages = NULL;
			int fragmentsNum=0;

			CWWTPSta *ptr;
			CWWTPSta *oldptr;
			oldptr = ptr = NULL;
			CWThreadMutexLock(&sta_table_mutex);
			for(ptr = wtp_sta_list;ptr != NULL; oldptr = ptr ,ptr = ptr->next){
				if(macAddrCmp(ptr->sta_mac,mac) == 1){
					values.radio_id = ptr->radio_id;
					values.wlan_id = ptr->wlan_id;
				}
			}
			//values.radio_id = 0;
			values.mac_length = 6;
			CW_CREATE_OBJECT_SIZE_ERR(values.mac_addr, values.mac_length, {CWWTPDebugLog("create memory error in CWWTPCheckNetLink");close(sock);CWExitThread();});
			for(i = 0;i<6;i++)
			{
				values.mac_addr[i] = mac[i];
			}

			CWThreadMutexUnlock(&sta_table_mutex);
			STATableDelete(&values);
			seqNum = CWGetSeqNum();
			if(!CWAssembleWTPEventDeleteStationRequest(&messages, &fragmentsNum, gWTPPathMTU, seqNum, &values)){
				CWWTPDebugLog("Station aging AssembleWTPEventDeleteStationRequest error!");
			}
			CW_FREE_OBJECT(values.mac_addr);
			
			for(i = 0; i < fragmentsNum; i++) 
			{
//				CWCaptrue(messages[i].offset, messages[i].msg);

#ifndef CW_NO_DTLS
				if(gDtlsSecurity == 1)
				{
					if(!CWSecuritySend(gWTPSession, messages[i].msg, messages[i].offset))
					{
						CWDebugLog("Error sending message");
						CWWTPDebugLog("Error sending message Station aging!");
						break; //gzm change countinue to break 091029
					}
				}
				else
#endif
				{
					if (!CWNetworkSendUnsafeUnconnected(gWTPSocket, &(gACInfoPtr->preferredAddress), messages[i].msg, messages[i].offset))
					{
						CWDebugLog("Error sending message");
						CWWTPDebugLog("Error sending message Station aging!");
						break; //gzm change countinue to break 091029
					}
				}
			}

			CWDebugLog("Message Sent\n");
			CWFreeMessageFragments(messages, fragmentsNum);
			CW_FREE_OBJECT(messages);
		}

	}

	return(NULL);
}

CWBool acw_parse_ntp_report(CWProtocolMessage *msgPtr, int len ,ntp_report_type *valPtr)	
{

	CWDebugLog("Parse ntp_report message values");
	CWParseMessageElementStart();
//	valPtr->type= atoi(CWProtocolRetrieveStr(msgPtr,1));// 1--dhcpsnooping;2--ntp
//	valPtr->result= atoi(CWProtocolRetrieveStr(msgPtr,1));
	void *ptr = NULL;
	ptr = CWProtocolRetrieveStr(msgPtr,1);
	valPtr->type = atoi(ptr);	// 1--dhcpsnooping;2--ntp
	CW_FREE_OBJECT(ptr);

	ptr = CWProtocolRetrieveStr(msgPtr,1);
	valPtr->result = atoi(ptr);
	CW_FREE_OBJECT(ptr);

	CWWTPDebugLog("ntp_report ( type = %d result = %d  )",valPtr->type,valPtr->result);
	CWParseMessageElementEnd();
}

CWBool CWAssembleMsgElemDhcpsnoopingReport(CWProtocolMessage *msgPtr, dhcpsnooping_report_type* valuesPtr)             //pei add 0703
{
	const int dhcpsnooping_report_length = 19;
	
	if(msgPtr == NULL) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	
	CW_CREATE_PROTOCOL_MESSAGE(*msgPtr, dhcpsnooping_report_length, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	CWProtocolStore8(msgPtr, WTP);
	CWProtocolStore8(msgPtr, valuesPtr->subtype);
	CWProtocolStore8(msgPtr, valuesPtr->radio_id);
	valuesPtr->wlan_id = CWGetRemoteWlanID(valuesPtr->radio_id, valuesPtr->wlan_id);
	CWProtocolStore8(msgPtr, valuesPtr->wlan_id);
	CWProtocolStore32(msgPtr, valuesPtr->vlanid);
	CWProtocolStoreRawBytes(msgPtr, (char *)(valuesPtr->mac), 6); 
	CWProtocolStore8(msgPtr, valuesPtr->ip_length);
	CWProtocolStore32(msgPtr, valuesPtr->ip);
	return CWAssembleMsgElem(msgPtr, BINDING_MSG_ELEMENT_TYPE_WTP_RADIO_REPORT);
}
CWBool CWAssembleMsgElemNtpReport(CWProtocolMessage *msgPtr, ntp_report_type* valuesPtr)           
{
	const int ntp_report_length = 4;
	
	if(msgPtr == NULL) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	
	CW_CREATE_PROTOCOL_MESSAGE(*msgPtr, ntp_report_length, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	CWProtocolStore8(msgPtr, NTP_REPORT);
	CWProtocolStore8(msgPtr, valuesPtr->result);
	CWProtocolStore8(msgPtr, 0);
	CWProtocolStore8(msgPtr, 0);
	return CWAssembleMsgElem(msgPtr, CW_MSG_ELEMENT_VENDOR_SPEC_PAYLOAD_CW_TYPE);
}

CWBool CWAssembleWTPEventStationSnoopingInfoRequest (CWProtocolMessage **messagesPtr, int *fragmentsNumPtr, int PMTU, int seqNum, dhcpsnooping_report_type*valuesPtr) 	//pei add 0703
{
	CWProtocolMessage *msgElems= NULL;
	const int msgElemCount = 1;
	CWProtocolMessage *msgElemsBinding= NULL;
	const int msgElemBindingCount=0;
	
	if(messagesPtr == NULL || fragmentsNumPtr == NULL) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);

	CW_CREATE_PROTOCOL_MSG_ARRAY_ERR(msgElems, msgElemCount, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););	
	CWDebugLog("Assembling WTP Event Request Sta info by dhcpsnooping...");
	// Assemble Message Elements
	if (!(CWAssembleMsgElemDhcpsnoopingReport(msgElems, valuesPtr))) {
		CW_FREE_OBJECT(msgElems);
		return CW_FALSE; // error will be handled by the caller
	}
	
	if(!(CWAssembleMessage(messagesPtr,
			       fragmentsNumPtr,
			       PMTU,
			       seqNum,
			       CW_MSG_TYPE_VALUE_WTP_EVENT_REQUEST,
			       msgElems,
			       msgElemCount,
			       msgElemsBinding,
			       msgElemBindingCount,
#ifdef CW_NO_DTLS
			       CW_PACKET_PLAIN
#else
			       (gDtlsSecurity == 1)?CW_PACKET_CRYPT:CW_PACKET_PLAIN           /* 0-CW_PACKET_PLAIN, 1-CW_PACKET_CRYPT */
#endif
			       ))) 
		return CW_FALSE;
	
	CWDebugLog("WTP Event Request Sta info by dhcpsnooping Assembled");
	

	return CW_TRUE;
}
CWBool CWAssembleWTPEventNtpInfoRequest (CWProtocolMessage **messagesPtr, int *fragmentsNumPtr, int PMTU, int seqNum, ntp_report_type*valuesPtr) 	
{
	CWProtocolMessage *msgElems= NULL;
	const int msgElemCount = 1;
	CWProtocolMessage *msgElemsBinding= NULL;
	const int msgElemBindingCount=0;
	
	if(messagesPtr == NULL || fragmentsNumPtr == NULL) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);

	CW_CREATE_PROTOCOL_MSG_ARRAY_ERR(msgElems, msgElemCount, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););	
	CWDebugLog("Assembling WTP Event Request ntp info ...");
	// Assemble Message Elements
	if (!(CWAssembleMsgElemNtpReport(msgElems, valuesPtr))) {
		CW_FREE_OBJECT(msgElems);
		return CW_FALSE; // error will be handled by the caller
	}
	
	if(!(CWAssembleMessage(messagesPtr,
			       fragmentsNumPtr,
			       PMTU,
			       seqNum,
			       CW_MSG_TYPE_VALUE_WTP_EVENT_REQUEST,
			       msgElems,
			       msgElemCount,
			       msgElemsBinding,
			       msgElemBindingCount,
#ifdef CW_NO_DTLS
			       CW_PACKET_PLAIN
#else			       
			       CW_PACKET_CRYPT
#endif
			       ))) 
		return CW_FALSE;
	
	CWDebugLog("WTP Event Request ntp info Assembled");
	

	return CW_TRUE;
}
CW_THREAD_RETURN_TYPE CWWTPCheckMsgFromOtherPro(void *arg)
{
	char sManagerID[64];
	memset(sManagerID,0,64);
	sprintf(sManagerID,"echo %d > /tmp/manager_id",getppid());
	_system(sManagerID);
	_system("touch /tmp/run_state");
	
	if(pthread_detach(pthread_self())!=0)
	{
		CWWTPDebugLog("###detach error in CWWTPCheckMsgFromOtherPro!###\n");
	}
	int i=0;
	int sock;
	int readBytes = 0;
	char s_buff[CW_BUFFER_SIZE];
	struct sockaddr_in local;
	struct sockaddr_in server_addr;
	
//	CWThreadSetSignals(SIG_BLOCK, 1, SIGALRM);
	
	sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (sock < 0) {
		perror("Failed to create socket");
		CWWTPDebugLog("Failed to create socket in CWWTPCheckMsgFromOtherPro, goto exit_thread");
		goto  exit_thread;
	}

	memset(&local, 0, sizeof(local));
	local.sin_family = AF_INET;
	local.sin_port = htons(3333);
	local.sin_addr.s_addr = htonl(INADDR_ANY);
	int on=1;
	setsockopt(sock,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on));
	if(bind(sock, (struct sockaddr *)&local, sizeof(local)) == -1)
	{
		perror("bind error");
		CWWTPDebugLog("bind error, goto exit_thread\n");
		close(sock);
		goto  exit_thread;
	}
	
	CW_REPEAT_FOREVER
	{
		bzero(s_buff, CW_BUFFER_SIZE);
		if(!CWErr(CWNetworkReceiveUnsafe(sock, s_buff, CW_BUFFER_SIZE, 0, (CWNetworkLev4Address *)&server_addr, &readBytes))) 
		{
			if (CWErrorGetLastErrorCode() == CW_ERROR_INTERRUPTED)
				continue;
		}
		/*if(!gDhcpsnoopingReportEnable)
		{
			CWWTPDebugLog("close dhcpsnooping report, goto exit_thread");
			close(sock);
			goto  exit_thread;
		}*/

//		CWCaptrue(readBytes, s_buff);

		CWProtocolMessage completeMsg;
		completeMsg.msg = s_buff;
		completeMsg.offset = 0;
		dhcpsnooping_report_type *auxdhcp_report;
		ntp_report_type *ntp_report;
		int type;
		int seqNum;
		CWProtocolMessage *messages = NULL;
		int fragmentsNum=0;

		seqNum = CWGetSeqNum();
		
//		type= atoi(CWProtocolRetrieveStr(&completeMsg,1));
		/*
		 * BUG ML09
		 * 19/10/2009 - Donato Capitella
		 * 14/3/2011 - Pei Wenhui
		 */
		void *ptr = NULL;
		ptr = CWProtocolRetrieveStr(&completeMsg,1);
		type = atoi(ptr);
		CW_FREE_OBJECT(ptr);
		switch(type){
			case 1:// 1--dhcpsnooping_report;2--ntp_report
				completeMsg.offset = 0;
				CW_CREATE_OBJECT_ERR(auxdhcp_report, dhcpsnooping_report_type, {close(sock);CWWTPDebugLog("CW_ERROR_OUT_OF_MEMORY");printf("CW_ERROR_OUT_OF_MEMORY\n");return (NULL);});
				acw_parse_dhcpsnooping_report(&completeMsg,readBytes,auxdhcp_report);
				if(!CWAssembleWTPEventStationSnoopingInfoRequest(&messages, &fragmentsNum, gWTPPathMTU, seqNum,auxdhcp_report)){
					CW_FREE_OBJECT(auxdhcp_report);
					CWWTPDebugLog("AssembleWTPEventStationSnoopingInfoRequest error!");
					break; //gzm ??? 
				}
				CW_FREE_OBJECT(auxdhcp_report);
				break;
			case 2:// 1--dhcpsnooping_report;2--ntp_report
				completeMsg.offset = 0;
				CW_CREATE_OBJECT_ERR(ntp_report, ntp_report_type, {close(sock);CWWTPDebugLog("CW_ERROR_OUT_OF_MEMORY");printf("CW_ERROR_OUT_OF_MEMORY\n");return (NULL);});
				acw_parse_ntp_report(&completeMsg,readBytes,ntp_report);
				if(!CWAssembleWTPEventNtpInfoRequest(&messages, &fragmentsNum, gWTPPathMTU, seqNum,ntp_report)){
					CW_FREE_OBJECT(ntp_report);
					CWWTPDebugLog("AssembleWTPEventNtpInfoRequest error!");
					break;  
				}
				CW_FREE_OBJECT(ntp_report);
				break;
			default:
				continue;
		}
			
		for(i = 0; i < fragmentsNum; i++) 
		{
			//CWCaptrue(messages[i].offset, messages[i].msg);

#ifndef CW_NO_DTLS
			if(gDtlsSecurity == 1)
			{
				if(!CWSecuritySend(gWTPSession, messages[i].msg, messages[i].offset))
				{
					CWDebugLog("Error sending message");
					CWWTPDebugLog("Error sending message dhcpsnooping or ntp report!");
					break; //gzm change countinue to break 091029
				}
			}
			else
#endif
			{
				if (!CWNetworkSendUnsafeUnconnected(gWTPSocket, &(gACInfoPtr->preferredAddress), messages[i].msg, messages[i].offset))
				{
					CWDebugLog("Error sending message");
					CWWTPDebugLog("Error sending message dhcpsnooping or ntp report!");
					break; //gzm change countinue to break 091029
				}
			}
		}

		CWDebugLog("Message Sent\n");
		CWFreeMessageFragments(messages, fragmentsNum);
		CW_FREE_OBJECT(messages);

	}
exit_thread:
	CWExitThread();
	return NULL;
}
#if 1  /* pei add for rogue AP scanning. at 1118*/
static int
getsocket(void)
{
	int s = -1;

	if (s < 0) {
		s = socket(AF_INET, SOCK_DGRAM, 0);
		if (s < 0)
			err(1, "socket(SOCK_DRAGM)");
	}
	return s;
}

static int
do80211priv(struct iwreq *iwr, const char *ifname, int op, void *data, u_int32_t len)
{
#define	N(a)	(sizeof(a)/sizeof(a[0]))

	memset(iwr, 0, sizeof(iwr));
	strncpy(iwr->ifr_name, ifname, IFNAMSIZ);
	if (len < IFNAMSIZ) {
		/*
		 * Argument data fits inline; put it there.
		 */
		memcpy(iwr->u.name, data, len);
	} else {
		/*
		 * Argument data too big for inline transfer; setup a
		 * parameter block instead; the kernel will transfer
		 * the data for the driver.
		 */
		iwr->u.data.pointer = data;
		iwr->u.data.length = len;
	}

	int s = getsocket();
	if(s<0)
		return -1;
	if (ioctl(s, op, iwr) < 0) {
		const char *opnames[] = {
			"ioctl[IEEE80211_IOCTL_SETPARAM]",
			"ioctl[IEEE80211_IOCTL_GETPARAM]",
			"ioctl[IEEE80211_IOCTL_SETKEY]",
			"ioctl[SIOCIWFIRSTPRIV+3]",
			"ioctl[IEEE80211_IOCTL_DELKEY]",
			"ioctl[SIOCIWFIRSTPRIV+5]",
			"ioctl[IEEE80211_IOCTL_SETMLME]",
			"ioctl[SIOCIWFIRSTPRIV+7]",
			"ioctl[IEEE80211_IOCTL_SETOPTIE]",
			"ioctl[IEEE80211_IOCTL_GETOPTIE]",
			"ioctl[IEEE80211_IOCTL_ADDMAC]",
			"ioctl[SIOCIWFIRSTPRIV+11]",
			"ioctl[IEEE80211_IOCTL_DELMAC]",
			"ioctl[SIOCIWFIRSTPRIV+13]",
			"ioctl[IEEE80211_IOCTL_CHANLIST]",
			"ioctl[SIOCIWFIRSTPRIV+15]",
			"ioctl[IEEE80211_IOCTL_GETRSN]",
			"ioctl[SIOCIWFIRSTPRIV+17]",
			"ioctl[IEEE80211_IOCTL_GETKEY]",
		};
		op -= SIOCIWFIRSTPRIV;
		if (0 <= op && op < N(opnames))
			perror(opnames[op]);
		else
			perror("ioctl[unknown???]");
		close(s);
		return -1;
	}
	close(s);
	return 0;
#undef N
}

static int
get80211priv(const char *ifname, int op, void *data, u_int32_t len)
{
	struct iwreq iwr;

	if (do80211priv(&iwr, ifname, op, data, len) < 0)
		return -1;
	if (len < IFNAMSIZ)
		memcpy(data, iwr.u.name, len);
	return iwr.u.data.length;
}
#if 0
/*
 * Copy the ssid string contents into buf, truncating to fit.  If the
 * ssid is entirely printable then just copy intact.  Otherwise convert
 * to hexadecimal.  If the result is truncated then replace the last
 * three characters with "...".
 */
static u_int32_t
copy_essid(char buf[], u_int32_t bufsize, const u_int8_t *essid, u_int32_t essid_len)
{
	const u_int8_t *p; 
	u_int32_t maxlen;
	int i;

	if (essid_len > bufsize)
		maxlen = bufsize;
	else
		maxlen = essid_len;
	/* determine printable or not */
	for (i = 0, p = essid; i < maxlen; i++, p++) {
		if (*p < ' ' || *p > 0x7e)
			break;
	}
	if (i != maxlen) {		/* not printable, print as hex */
		if (bufsize < 3)
			return 0;
#if 0
		strlcpy(buf, "0x", bufsize);
#else
		strncpy(buf, "0x", bufsize);
#endif
		bufsize -= 2;
		p = essid;
		for (i = 0; i < maxlen && bufsize >= 2; i++) {
			sprintf(&buf[2+2*i], "%02x", *p++);
			bufsize -= 2;
		}
		maxlen = 2+2*i;
	} else {			/* printable, truncate as needed */
		memcpy(buf, essid, maxlen);
	}
	if (maxlen != essid_len)
		memcpy(buf+maxlen-3, "...", 3);
	return maxlen;
}

static const char *
ieee80211_ntoa(const u_int8_t mac[IEEE80211_ADDR_LEN])
{
	static char a[18];
	int i;

	i = snprintf(a, sizeof(a), "%02x:%02x:%02x:%02x:%02x:%02x",
		mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
	return (i < 17 ? NULL : a);
}
#endif
/*gzm change 15 to 30  20100106*/
static int
getmaxrate(u_int8_t rates[IEEE80211_RATE_MAXSIZE], u_int8_t nrates)
{
	int i, maxrate = -1;

	for (i = 0; i < nrates; i++) {
		int rate = rates[i] & IEEE80211_RATE_VAL;
		if (rate > maxrate)
			maxrate = rate;
	}
	return maxrate / 2;
}

static void
getcaps(int capinfo, char *capstring)
{
	char *cp = capstring;

	if (capinfo & IEEE80211_CAPINFO_ESS)
		*cp++ = 'E';
	if (capinfo & IEEE80211_CAPINFO_IBSS)
		*cp++ = 'I';
	if (capinfo & IEEE80211_CAPINFO_CF_POLLABLE)
		*cp++ = 'c';
	if (capinfo & IEEE80211_CAPINFO_CF_POLLREQ)
		*cp++ = 'C';
	if (capinfo & IEEE80211_CAPINFO_PRIVACY)
		*cp++ = 'P';
	if (capinfo & IEEE80211_CAPINFO_SHORT_PREAMBLE)
		*cp++ = 'S';
	if (capinfo & IEEE80211_CAPINFO_PBCC)
		*cp++ = 'B';
	if (capinfo & IEEE80211_CAPINFO_CHNL_AGILITY)
		*cp++ = 'A';
	if (capinfo & IEEE80211_CAPINFO_SHORT_SLOTTIME)
		*cp++ = 's';
	if (capinfo & IEEE80211_CAPINFO_RSN)
		*cp++ = 'R';
	if (capinfo & IEEE80211_CAPINFO_DSSSOFDM)
		*cp++ = 'D';
	*cp = '\0';
}
/* unaligned little endian access */
#define LE_READ_4(p)					\
	((u_int32_t)					\
	 ((((const u_int8_t *)(p))[0]      ) |		\
	  (((const u_int8_t *)(p))[1] <<  8) |		\
	  (((const u_int8_t *)(p))[2] << 16) |		\
	  (((const u_int8_t *)(p))[3] << 24)))
static __inline int
iswpaoui(const u_int8_t *frm)
{
	return frm[1] > 3 && LE_READ_4(frm + 2) == ((WPA_OUI_TYPE << 24) | WPA_OUI);
}

static void getencryption(const u_int8_t *vp, int ielen, char *buf, int *buf_len)
{
	while (ielen > 0) {
		switch (vp[0]) {
		case IEEE80211_ELEMID_VENDOR:
			if (iswpaoui(vp))
			{
				*buf_len = 3;
				memcpy(buf, "WPA", strlen("WPA"));
				return;
			}
			else
				*buf_len = 0;
			break;
		case IEEE80211_ELEMID_RSN:
			*buf_len = 3;
			memcpy(buf, "RSN", strlen("RSN"));
			return;
		default:
			*buf_len = 0;
			break;
		}
		ielen -= 2 + vp[1];
		vp += 2 + vp[1];
	}
	return;
}

/*pei test for neighbor AP info list. 1125*/
CWBool CWAssembleMsgElemNeighborAPInfos(CWProtocolMessage *msgPtr, unsigned int *neighborApCount, Neighbor_AP_INFO *valuesPtr,unsigned char radioId)             //pei add 0703
{
	int value = NEIGHBOR_AP_INFO;
	int neighbor_apinfos_length = 3;
	unsigned char disturb_warning = 0;
	Neighbor_AP_INFO *p = valuesPtr;
	int nNewElementCount = 0;

	while(p!=NULL)
	{
		if( p->Opstatus == REMAIN || p->Radio_id != radioId)   /* pei changed 2 to REMAIN 1202 */
		{
			p = p->next;
			continue;
		}
		neighbor_apinfos_length+=17;
		neighbor_apinfos_length+=p->EssidLen;
		neighbor_apinfos_length+=p->IeLen;
		p = p->next;
		nNewElementCount++;
	}

	if(msgPtr == NULL) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	CWWTPDebugLog("Assemble Msg Elem Neighbor AP count:[%d] %d",radioId ,nNewElementCount);
	CW_CREATE_PROTOCOL_MESSAGE(*msgPtr, neighbor_apinfos_length, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););

	int fd;
	char disturb_index[10];
	int size = 0;
	fd = open ("/tmp/disturb_warning", O_RDONLY);
	if(fd!=-1)
	{
		memset(disturb_index, 0, 10);
		size = read(fd, disturb_index, sizeof(disturb_index));
		disturb_warning = atoi(disturb_index);
		dpf("disturb_warning:%d\n", disturb_warning);
	}
	close(fd);

	CWProtocolStore8(msgPtr, value);
	CWProtocolStore8(msgPtr, disturb_warning);  //pei add 0214
	CWProtocolStore8(msgPtr, nNewElementCount); 

	p = valuesPtr;

	while(p!=NULL)
	{
		if( p->Opstatus == REMAIN || p->Radio_id != radioId)   /* pei changed 2 to REMAIN 1202 */
		{
			p = p->next;
			continue;
		}

		if(debug_print)
		{
			int n=0;
			printf("\nBSSID:");
			for(n=0;n<6;n++)
			{
				printf("%02x ", p->BSSID[n]);
			}
			printf("Rate:%d Channel:%d RSSI:%d NOISE:%d BEACON_INT:%d Opstatus:%d CapabilityInfo:%d\n",
				p->Rate, p->Channel, (signed char)p->RSSI, p->NOISE, 
				p->BEACON_INT, p->Opstatus, p->CapabilityInfo);
			printf("EssidLen:%d ESSID:%s", p->EssidLen, p->ESSID);
			printf("\nIeLen:%d IEs_INFO:%s Radio_ID:%02x\n", p->IeLen, p->IEs_INFO,p->Radio_id);
		}

		CWProtocolStoreRawBytes(msgPtr, (char *)(p->BSSID), 6);
		CWProtocolStore16(msgPtr, p->Rate);
		CWProtocolStore8(msgPtr, p->Channel);
		CWProtocolStore8(msgPtr, p->RSSI);
		CWProtocolStore8(msgPtr, p->NOISE);
		CWProtocolStore8(msgPtr, p->BEACON_INT);
		CWProtocolStore8(msgPtr, p->Opstatus);
		CWProtocolStore16(msgPtr, p->CapabilityInfo);
		CWProtocolStore8(msgPtr, p->EssidLen);
		CWProtocolStoreRawBytes(msgPtr, p->ESSID, p->EssidLen);
		CWProtocolStore8(msgPtr, p->IeLen);
		CWProtocolStoreRawBytes(msgPtr, p->IEs_INFO, p->IeLen);
		p = p->next;
	}
	return CWAssembleMsgElem(msgPtr, CW_MSG_ELEMENT_VENDOR_SPEC_PAYLOAD_CW_TYPE);
}

CWBool CWAssembleWTPEventApScanningRequest (CWProtocolMessage **messagesPtr, int *fragmentsNumPtr, int PMTU, int seqNum, unsigned int *neighborApCount, Neighbor_AP_INFO *valuesPtr, unsigned char radioId) 	//pei add 0703
{
	CWProtocolMessage *msgElems= NULL;
	const int msgElemCount = 1;
	CWProtocolMessage *msgElemsBinding= NULL;
	const int msgElemBindingCount=0;
	
	if(messagesPtr == NULL || fragmentsNumPtr == NULL) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);

	CW_CREATE_PROTOCOL_MSG_ARRAY_ERR(msgElems, msgElemCount, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););	
	
	CWDebugLog("Assembling WTP Event Request AP Scanning...");

	// Assemble Message Elements
	if (!(CWAssembleMsgElemNeighborAPInfos(msgElems, neighborApCount, valuesPtr,radioId))) {
		CW_FREE_OBJECT(msgElems);
		return CW_FALSE; // error will be handled by the caller
	}
	
	if(!(CWAssembleMessageCheckRadio(messagesPtr,
			       fragmentsNumPtr,
			       PMTU,
			       seqNum,
			       CW_MSG_TYPE_VALUE_WTP_EVENT_REQUEST,
			       msgElems,
			       msgElemCount,
			       msgElemsBinding,
			       msgElemBindingCount,
#ifdef CW_NO_DTLS
			       CW_PACKET_PLAIN
#else
			       (gDtlsSecurity == 1)?CW_PACKET_CRYPT:CW_PACKET_PLAIN           /* 0-CW_PACKET_PLAIN, 1-CW_PACKET_CRYPT */
#endif
					, radioId
			       ))) 
		return CW_FALSE;
	
	CWDebugLog("WTP Event Request AP Scanning Assembled");

	return CW_TRUE;
}

CWBool CWAssembleMsgElemAPMonitor(CWProtocolMessage *msgPtr, int eth_throughput, int wifi_throughput)             //pei add 0703
{
	int value = MONITOR;
	int ap_monitor_length = 9;

	if(msgPtr == NULL) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	
	CW_CREATE_PROTOCOL_MESSAGE(*msgPtr, ap_monitor_length, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););

	CWProtocolStore8(msgPtr, value);
	CWProtocolStore32(msgPtr, eth_throughput);
	CWProtocolStore32(msgPtr, wifi_throughput);

	dpf("value:%d eth_throughput:%d wifi_throughput:%d\n", value, eth_throughput, wifi_throughput);

	return CWAssembleMsgElem(msgPtr, CW_MSG_ELEMENT_VENDOR_SPEC_PAYLOAD_CW_TYPE);
}

CWBool CWAssembleWTPEventApMonitorRequest (CWProtocolMessage **messagesPtr, int *fragmentsNumPtr, int PMTU, int seqNum, int eth_throughput, int wifi_throughput) 	//pei add 0703
{
	CWProtocolMessage *msgElems= NULL;
	const int msgElemCount = 1;
	CWProtocolMessage *msgElemsBinding= NULL;
	const int msgElemBindingCount=0;
	
	if(messagesPtr == NULL || fragmentsNumPtr == NULL) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);

	CW_CREATE_PROTOCOL_MSG_ARRAY_ERR(msgElems, msgElemCount, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););	
	
	CWDebugLog("Assembling WTP Event Request AP Monitor...");

	// Assemble Message Elements
	if (!(CWAssembleMsgElemAPMonitor(msgElems, eth_throughput, wifi_throughput))) {
		CW_FREE_OBJECT(msgElems);
		return CW_FALSE; // error will be handled by the caller
	}
	
	if(!(CWAssembleMessage(messagesPtr,
			       fragmentsNumPtr,
			       PMTU,
			       seqNum,
			       CW_MSG_TYPE_VALUE_WTP_EVENT_REQUEST,
			       msgElems,
			       msgElemCount,
			       msgElemsBinding,
			       msgElemBindingCount,
#ifdef CW_NO_DTLS
			       CW_PACKET_PLAIN
#else
			       (gDtlsSecurity == 1)?CW_PACKET_CRYPT:CW_PACKET_PLAIN           /* 0-CW_PACKET_PLAIN, 1-CW_PACKET_CRYPT */
#endif
			       ))) 
		return CW_FALSE;
	
	CWDebugLog("WTP Event Request AP Monitor Assembled");

	return CW_TRUE;
}

CWBool CWGetEthUpdownTime(extra_info *extraInfoValues)
{
	int i = 0;
//	CWWTPWlan *ptr = NULL;
	
	for(i=0;i<extraInfoValues->eth_count;i++)
	{
		extraInfoValues->eth_updown_time[i].id = i;
//		extraInfoValues->eth_updown_time[i].updown_time = 1;
		char str[20];
		int ifUpTime = 0;
		int ifDownTime = 0;
		
		CWGetEthUpTime(&ifUpTime,i);
		dpf("ifUpTime: %s len:%d ifUpTime:%d\n", str, (int)strlen(str), ifUpTime);
		
		CWGetEthdownTime(&ifDownTime,i);
		dpf("ifDownTime: %s len:%d ifDownTime:%d\n", str, (int)strlen(str), ifDownTime);
		if((ifUpTime+ifDownTime) == 0)
			extraInfoValues->eth_updown_time[i].updown_time = 1;
		else
			extraInfoValues->eth_updown_time[i].updown_time = ifUpTime+ifDownTime;
	}
	
	return CW_TRUE;
}
CWBool CWGetMemUseRatio(unsigned short *memTotal, unsigned char *memUseRatio)
{
	char str[20];
	int mem_Total = 0;
	int memFree = 0;
//	int memUseRatio = 0;

	mem_Total = CWGetTotalMem();
	dpf("memTotal: %s len:%d memTotal:%d\n", str, (int)strlen(str), mem_Total);

	memFree = CWGetFreeMem();
//		dpf"memFree mode: %s len:%d memFree:%d\n", str, (int)strlen(str), memFree);
	if(mem_Total == 0)  //pei add 0416
	{
#ifdef CAVIUM_PLATFORM
		mem_Total = 249216;
#else
#ifdef AP2400_IFM
		mem_Total=65536;
#else
#ifdef WNAP350
		mem_Total=131072;
#else
#ifdef WNAP210
		mem_Total=32768;
#else
		mem_Total = 13836;
#endif
#endif
#endif
#endif
	  }
	memFree/=1024;
	*memUseRatio = 100-memFree*100/mem_Total;
	*memTotal = mem_Total;

	dpf("memTotal:%dMB memUseRatio:%d%%\n", *memTotal, *memUseRatio);

	return CW_TRUE;
}
void CWGetFlashSizeInfo(unsigned short *flashTotal, int *flashFree)
{
	int flashConfigFree;
	char temp_str[100];
	char str[20];
	
	flashConfigFree = CWGetFlashConfigSize();
	memset(temp_str, 0, 100);
	sprintf(temp_str, "/usr/sbin/get_hardinfo | awk -F \"[:M]\" '/flash_size/ {print $2}'");
	PopenFile(temp_str, str, sizeof(str));	
	*flashTotal = atoi(str);
	*flashFree = flashConfigFree*8;
	if(debug_print)
		printf("flashTotal:%dMB flashFree:%dKb\n", *flashTotal, *flashFree);
	
	return;
}
unsigned char CWGetSnr2(void)
{
	unsigned char snr = 0;
	unsigned char wifi0Snr = 0;
	unsigned char wifi1Snr = 0;
	CWWTPWlan *ptr = NULL;
	CWWTPWlan *oldptr = NULL;
	unsigned char radioId = 0;
	unsigned char wlanId = 0;
	CWBool wifi0WlanFound = CW_FALSE;
	CWBool wifi1WlanFound = CW_FALSE;
	char ifname[IFNAMSIZ];
	char temp_str[100];
	char str[20];
	int wifi_count=0;

	wifi_count = CWGetRadioMaxCount();
	for(ptr=wtp_wlan_list;ptr != NULL;oldptr = ptr,ptr = ptr->next)
	{
		if((ptr->radio_id == 0)&&(ptr->wlan_id != 0))
		{
			radioId = ptr->radio_id;
			wlanId = ptr->wlan_id;
			wifi0WlanFound = CW_TRUE;
			break;
		}
	}
	if(wifi0WlanFound == CW_TRUE)
	{
		memset(ifname,0,IFNAMSIZ);
		CWWTPGetWlanName(ifname,radioId, wlanId);
		memset(temp_str, 0, 100);
		sprintf(temp_str, "iwconfig %s | awk -F \"[=/]\" '/Link Quality/ {print $2}'", ifname);
		PopenFile(temp_str, str, sizeof(str));	
		wifi0Snr = atoi(str);
	}
	snr = wifi0Snr;
	if(wifi_count == 2)
	{
		for(ptr=wtp_wlan_list;ptr != NULL;oldptr = ptr,ptr = ptr->next)
		{
			if((ptr->radio_id == 1)&&(ptr->wlan_id != 0))
			{
				radioId = ptr->radio_id;
				wlanId = ptr->wlan_id;
				wifi1WlanFound = CW_TRUE;
				break;
			}
		}
		if(wifi1WlanFound ==CW_TRUE)
		{
			memset(ifname,0,IFNAMSIZ);
			CWWTPGetWlanName(ifname,radioId, wlanId);
			memset(temp_str, 0, 100);
			sprintf(temp_str, "iwconfig %s | awk -F \"[=/]\" '/Link Quality/ {print $2}'", ifname);
			PopenFile(temp_str, str, sizeof(str));	
			wifi1Snr = atoi(str);
		}
		if(wifi0Snr == 0)
			snr = wifi1Snr;
	}
	dpf("AP get Snr:%d\n", snr);
	
	return snr;
}
char CWGetTemperature2(void)
{
	char temperature = 0;
	char temp_str[100];
	char str[20];
	
	memset(temp_str, 0, 100);
#ifdef CAVIUM_PLATFORM
	sprintf(temp_str, "avrutil -T | awk -F \" \" '/Temperature/ {print $3}'");
#else
	sprintf(temp_str,"pro_ctl_util -M | awk -F \"=\" '{print $2}'");
#endif
	PopenFile(temp_str, str,sizeof(str));
	temperature = atoi(str);
	dpf("AP get temperature:%d\n", temperature);
	
	return temperature;
}
CWBool CWGetWpiErrors2(char *ifname, wpi_errors *wpi_error_values)
{
	struct ieee80211_stats *stats=NULL;
	struct ifreq ifr;
	int s;
	
	s = socket(AF_INET, SOCK_DGRAM, 0);
	if (s < 0)
	{
		CWWTPDebugLog("create sock to get 80211stats error!");
		return CW_FALSE;
	}

    /**** zhouke add ,for marvell ioctl ****/
#ifdef MARVELL_PLATFORM
    memset(&ifr, 0, sizeof(struct ifreq));
#endif
    /************ end ************/
    
	strncpy(ifr.ifr_name, ifname, sizeof (ifr.ifr_name));
	stats = malloc(sizeof(struct ieee80211_stats)+ 2* sizeof(struct ieee80211_mac_stats));
	memset(stats,0,sizeof(struct ieee80211_stats)+ 2* sizeof(struct ieee80211_mac_stats));
	ifr.ifr_data = (caddr_t) stats;
	if(debug_print)
		printf("WriteAddr :%08x  MemLength:%d\n",(unsigned int)ifr.ifr_data,sizeof(struct ieee80211_stats)+  2* sizeof(struct ieee80211_mac_stats));
	if (ioctl(s, SIOCG80211STATS, &ifr) < 0)
	{
		CWWTPDebugLog("ioctl to get 80211stats error!");
		close(s);
		free(stats);
		stats=NULL;
		return CW_FALSE;
	}
	wpi_error_values->wpi_replay_error = stats->is_re_wpi;
	wpi_error_values->wpi_decryp_error = stats->is_wpi_no_key_error;
	wpi_error_values->wpi_mic_error = stats->is_wpi_mic;

	close(s);
	free(stats);
	stats = NULL;
	return CW_TRUE;
}
CWBool CWGetDisassocErrors2(char *ifname, disassoc_errors *disassoc_error_values)
{
	struct ieee80211_stats *stats=NULL;
	struct ifreq ifr;
	int s;
	
	s = socket(AF_INET, SOCK_DGRAM, 0);
	if (s < 0)
	{
		CWWTPDebugLog("create sock to get 80211stats error!");
		return CW_FALSE;
	}

    /**** zhouke add ,for marvell ioctl ****/
#ifdef MARVELL_PLATFORM
    memset(&ifr, 0, sizeof(struct ifreq));
#endif
    /************ end ************/
    
	strncpy(ifr.ifr_name, ifname, sizeof (ifr.ifr_name));
	stats = malloc(sizeof(struct ieee80211_stats)+ 2* sizeof(struct ieee80211_mac_stats));
	memset(stats,0,sizeof(struct ieee80211_stats)+ 2* sizeof(struct ieee80211_mac_stats));
	ifr.ifr_data = (caddr_t) stats;
	if (ioctl(s, SIOCG80211STATS, &ifr) < 0)
	{
		CWWTPDebugLog("ioctl to get 80211stats error!");
		close(s);
		free(stats);
		stats = NULL;
		return CW_FALSE;
	}
//	printstats(stdout, &stats);
	disassoc_error_values->disassoc_unnormal = stats->is_disassoc_unnormal;
	disassoc_error_values->rx_assoc_norate = stats->is_rx_assoc_norate;
	disassoc_error_values->rx_assoc_capmismatch = stats->is_rx_assoc_capmismatch;
	disassoc_error_values->assoc_invalid = stats->is_rx_bad_assoc;
	disassoc_error_values->reassoc_deny = stats->is_rx_assoc_bss + stats->is_rx_assoc_notauth + stats->is_rx_assoc_capmismatch + stats->is_rx_assoc_norate 
					+ stats->is_rx_assoc_badwpaie ;
	close(s);
	free(stats);
	stats = NULL;
	return CW_TRUE;
}
CWBool CWGetExtraInfo2(char *wifiname, extra_info *extraInfoValues)
{
	int s;
	struct ifreq ifr;
	struct ath_stats total;

	s = socket(AF_INET, SOCK_DGRAM, 0);
	if (s < 0)
	{
		CWWTPDebugLog("create sock to get extra info error!");
		return CW_FALSE;
	}

    /**** zhouke add ,for marvell ioctl ****/
#ifdef MARVELL_PLATFORM
    memset(&ifr, 0, sizeof(struct ifreq));
#endif
    /************ end ************/
    
	strncpy(ifr.ifr_name, wifiname, sizeof (ifr.ifr_name));

	ifr.ifr_data = (caddr_t) &total;
	if (ioctl(s, SIOCGATHSTATS, &ifr) < 0)
	{
		CWWTPDebugLog("ioctl to get extra info error!");
		close(s);
		return CW_FALSE;
		//err(1, ifr.ifr_name);
	}
/*	
	if (!getifstats(ifr.ifr_name, &itot, &otot))
	{
		CWWTPDebugLog("getifstats to get extra info error!");
		dpf("getifstats to get extra info error!\n");
		close(s);
		return CW_FALSE;
		//err(1, ifr.ifr_name);
	}
*/
	extraInfoValues->tx_mgmt += total.ast_tx_mgmt;
	extraInfoValues->rx_mgmt += total.ast_rx_mgt;
	extraInfoValues->tx_packets += total.ast_tx_packets;
	extraInfoValues->tx_errors += total.ast_tx_xretries;
	extraInfoValues->tx_retry += total.ast_tx_xretries;
		
/*	printf("%8u %8u %7u %7u %7u %6u %6u %6u %7u\n"
		, itot - total.ast_rx_mgt
		, otot
		, total.ast_tx_altrate
		, total.ast_tx_shortretry
		, total.ast_tx_longretry
		, total.ast_tx_xretries
		, total.ast_rx_crcerr
		, total.ast_rx_badcrypt
		, total.ast_rx_phyerr
	);
*/
	close(s);
	return CW_TRUE;
}
CWBool CWAssembleMsgElemExtraInfo(CWProtocolMessage *msgPtr, extra_info *extraInfoValues)             //pei add 0224
{
	int k;
	unsigned char value = EXTRA_INFO;
	const int vendor_specific_payload1_length = 84+20+3*extraInfoValues->ath_count+2*(extraInfoValues->eth_count+extraInfoValues->wifi_count);

	if(msgPtr == NULL) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	
	CW_CREATE_PROTOCOL_MESSAGE(*msgPtr, vendor_specific_payload1_length, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	
	CWProtocolStore8(msgPtr, value);
	CWProtocolStore32(msgPtr, extraInfoValues->cpu_use_ratio);
	CWProtocolStore32(msgPtr, extraInfoValues->tx_mgmt);
	CWProtocolStore32(msgPtr, extraInfoValues->rx_mgmt);
	CWProtocolStore32(msgPtr, extraInfoValues->tx_packets);
	CWProtocolStore32(msgPtr, extraInfoValues->tx_errors);
	CWProtocolStore32(msgPtr, extraInfoValues->tx_retry);
	CWProtocolStore32(msgPtr, extraInfoValues->tx_unicast);  /* pei add for eth at 090624 */
	CWProtocolStore32(msgPtr, extraInfoValues->tx_broadcast);
	CWProtocolStore32(msgPtr, extraInfoValues->tx_multicast);
	CWProtocolStore32(msgPtr, extraInfoValues->tx_drop);
	CWProtocolStore32(msgPtr, extraInfoValues->rx_unicast);
	CWProtocolStore32(msgPtr, extraInfoValues->rx_broadcast);
	CWProtocolStore32(msgPtr, extraInfoValues->rx_multicast);
	CWProtocolStore32(msgPtr, extraInfoValues->rx_drop);  /* end. pei add for eth at 090624 */
	CWProtocolStore32(msgPtr, extraInfoValues->wpi_replay_error);
	CWProtocolStore32(msgPtr, extraInfoValues->wpi_decryp_error);
	CWProtocolStore32(msgPtr, extraInfoValues->wpi_mic_error);
	CWProtocolStore32(msgPtr, extraInfoValues->disassoc_unnormal);  /* pei add for disassoc_errors at 090803 */
	CWProtocolStore32(msgPtr, extraInfoValues->rx_assoc_norate);
	CWProtocolStore32(msgPtr, extraInfoValues->rx_assoc_capmismatch);
	CWProtocolStore32(msgPtr, extraInfoValues->assoc_invalid);
	CWProtocolStore32(msgPtr, extraInfoValues->reassoc_deny);  /* end. pei add for disassoc_errors at 090803 */
	CWProtocolStore8(msgPtr, extraInfoValues->get_Ip_mode);
	CWProtocolStore16(msgPtr, extraInfoValues->mem_total);
	CWProtocolStore8(msgPtr, extraInfoValues->mem_use_ratio);
	CWProtocolStore16(msgPtr, extraInfoValues->flash_total);
	CWProtocolStore32(msgPtr, extraInfoValues->flash_free);
	CWProtocolStore8(msgPtr, extraInfoValues->snr);
	CWProtocolStore8(msgPtr, extraInfoValues->temperature);
	CWProtocolStore8(msgPtr, extraInfoValues->eth_count);
	dpf("value:%d \ncpu_use_ratio:%d \ntx_mgmt:%d \nrx_mgmt:%d \ntx_packets:%d \ntx_errors:%d \ntx_retry:%d \nwpi_replay_error:%d\
		 \nwpi_decryp_error:%d \nwpi_mic_error:%d \ndisassoc_unnormal:%d \nrx_assoc_norate:%d \nrx_assoc_capmismatch:%d\
		 \nassoc_invalid:%d \nreassoc_deny:%d \ntemperature:%d\neth_count:%d", 
			value, extraInfoValues->cpu_use_ratio, extraInfoValues->tx_mgmt, extraInfoValues->rx_mgmt,
			extraInfoValues->tx_packets, extraInfoValues->tx_errors, extraInfoValues->tx_retry, extraInfoValues->wpi_replay_error,
			extraInfoValues->wpi_decryp_error, extraInfoValues->wpi_mic_error, extraInfoValues->disassoc_unnormal, 
			extraInfoValues->rx_assoc_norate, extraInfoValues->rx_assoc_capmismatch, extraInfoValues->assoc_invalid, 
			extraInfoValues->reassoc_deny, extraInfoValues->temperature, extraInfoValues->eth_count);
	for(k=0;k<extraInfoValues->eth_count;k++)
	{
		CWProtocolStore8(msgPtr, extraInfoValues->eth_updown_time[k].id);
		CWProtocolStore8(msgPtr, extraInfoValues->eth_updown_time[k].updown_time);
		dpf("ethId:%d updowntime:%d\n", extraInfoValues->eth_updown_time[k].id, extraInfoValues->eth_updown_time[k].updown_time);
	}
	CWProtocolStore8(msgPtr, extraInfoValues->ath_count);
	dpf("ath_count:%d\n", extraInfoValues->ath_count);
	for(k=0;k<extraInfoValues->ath_count;k++)
	{
		CWProtocolStore8(msgPtr, extraInfoValues->ath_updown_time[k].radio_id);
		CWProtocolStore8(msgPtr, extraInfoValues->ath_updown_time[k].id);
		CWProtocolStore8(msgPtr, extraInfoValues->ath_updown_time[k].updown_time);
		dpf("athId:%d updowntime:%d\n", extraInfoValues->ath_updown_time[k].id, extraInfoValues->ath_updown_time[k].updown_time);
	}
	CWProtocolStore8(msgPtr, extraInfoValues->wifi_count);
	dpf("wifi_count:%d\n", extraInfoValues->wifi_count);
	for(k=0;k<extraInfoValues->wifi_count;k++)
	{
		CWProtocolStore8(msgPtr, extraInfoValues->wifi_status[k].id);
		CWProtocolStore8(msgPtr, extraInfoValues->wifi_status[k].status);
		dpf("wifiId:%d status:%d\n", extraInfoValues->wifi_status[k].id, extraInfoValues->wifi_status[k].status);
	}
	dpf("\n");
	
	return CWAssembleMsgElem(msgPtr, CW_MSG_ELEMENT_VENDOR_SPEC_PAYLOAD_CW_TYPE);
}
CWBool CWAssembleWTPEventExtraInfoRequest (CWProtocolMessage **messagesPtr, int *fragmentsNumPtr, int PMTU, int seqNum, extra_info *extraInfoValues) 	//pei add 0703
{
	CWProtocolMessage *msgElems= NULL;
	const int msgElemCount = 1;
	CWProtocolMessage *msgElemsBinding= NULL;
	const int msgElemBindingCount=0;
	
	if(messagesPtr == NULL || fragmentsNumPtr == NULL) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);

	CW_CREATE_PROTOCOL_MSG_ARRAY_ERR(msgElems, msgElemCount, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););	
	
	CWDebugLog("Assembling WTP Event Request AP extra_info...");

	// Assemble Message Elements
	if (!(CWAssembleMsgElemExtraInfo(msgElems, extraInfoValues))) {
		CW_FREE_OBJECT(msgElems);
		return CW_FALSE; // error will be handled by the caller
	}
	
	if(!(CWAssembleMessage(messagesPtr,
			       fragmentsNumPtr,
			       PMTU,
			       seqNum,
			       CW_MSG_TYPE_VALUE_WTP_EVENT_REQUEST,
			       msgElems,
			       msgElemCount,
			       msgElemsBinding,
			       msgElemBindingCount,
#ifdef CW_NO_DTLS
			       CW_PACKET_PLAIN
#else
			       (gDtlsSecurity == 1)?CW_PACKET_CRYPT:CW_PACKET_PLAIN           /* 0-CW_PACKET_PLAIN, 1-CW_PACKET_CRYPT */
#endif
			       ))) 
		return CW_FALSE;
	
	CWDebugLog("WTP Event Request AP extra_info Assembled");

	return CW_TRUE;
}
CWBool CWAssembleMsgElemStaInfo(CWProtocolMessage *msgPtr, int staCount, StationInfo *stationInfoValues)             //pei add 0224
{
	int k;
	unsigned char value = STA_INFO;
	const int vendor_specific_payload_length = 2+(23+42)*staCount;

	if(msgPtr == NULL) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	
	CW_CREATE_PROTOCOL_MESSAGE(*msgPtr, vendor_specific_payload_length, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	
	CWProtocolStore8(msgPtr, value);
	CWProtocolStore8(msgPtr, staCount);

	dpf("value:%d staCount:%d\n", value, staCount);
	if(staCount!=0)
	{
		for(k=0;k<staCount;k++)
		{
			CWProtocolStore8(msgPtr, stationInfoValues[k].radioId);
			stationInfoValues[k].wlanId = CWGetRemoteWlanID(stationInfoValues[k].radioId, stationInfoValues[k].wlanId);
			CWProtocolStore8(msgPtr, stationInfoValues[k].wlanId);
			CWProtocolStoreRawBytes(msgPtr, (char *)stationInfoValues[k].mac, 6);
			CWProtocolStore8(msgPtr, stationInfoValues[k].mode);
			CWProtocolStore8(msgPtr, stationInfoValues[k].channel);
			CWProtocolStore8(msgPtr, stationInfoValues[k].rssi);
			CWProtocolStore16(msgPtr, stationInfoValues[k].tx_Rate);
			CWProtocolStore8(msgPtr, stationInfoValues[k].isPowerSave);
			CWProtocolStore8(msgPtr, stationInfoValues[k].isQos);
			CWProtocolStore32(msgPtr, stationInfoValues[k].rx_bytes);
			CWProtocolStore32(msgPtr, stationInfoValues[k].tx_bytes);
			CWProtocolStore64(msgPtr, stationInfoValues[k].rx_data_bytes);
			CWProtocolStore64(msgPtr, stationInfoValues[k].tx_data_bytes);
			CWProtocolStore32(msgPtr, stationInfoValues[k].rx_data_frames);
			CWProtocolStore32(msgPtr, stationInfoValues[k].tx_data_frames);
			CWProtocolStore32(msgPtr, stationInfoValues[k].rx_frames);
			CWProtocolStore32(msgPtr, stationInfoValues[k].tx_frames);
			CWProtocolStore32(msgPtr, stationInfoValues[k].rx_frag_packets);
			CWProtocolStore32(msgPtr, stationInfoValues[k].tx_frag_packets);
			CWProtocolStore16(msgPtr, stationInfoValues[k].rx_Rate);
			dpf("radioId:%d wlanId:%d mac=%02x:%02x:%02x:%02x:%02x:%02x mode:%d channel:%d rssi:%d tx_Rate:%d isPowerSave:%d isQos:%d rx_bytes:%d tx_bytes:%d rx_data_bytes:%llu tx_data_bytes:%llu rx_data_frames:%d tx_data_frames:%d rx_frames:%d tx_frames:%d rx_frag_packets:%d tx_frag_packets:%d rx_Rate:%d\n", 
					stationInfoValues[k].radioId, stationInfoValues[k].wlanId, stationInfoValues[k].mac[0], stationInfoValues[k].mac[1],
					stationInfoValues[k].mac[2], stationInfoValues[k].mac[3], stationInfoValues[k].mac[4], stationInfoValues[k].mac[5],
					stationInfoValues[k].mode, stationInfoValues[k].channel, stationInfoValues[k].rssi, stationInfoValues[k].tx_Rate, 
					stationInfoValues[k].isPowerSave, stationInfoValues[k].isQos, stationInfoValues[k].rx_bytes, stationInfoValues[k].tx_bytes, 
					stationInfoValues[k].rx_data_bytes, stationInfoValues[k].tx_data_bytes, stationInfoValues[k].rx_data_frames,
					stationInfoValues[k].tx_data_frames, stationInfoValues[k].rx_frames, stationInfoValues[k].tx_frames, 
					stationInfoValues[k].rx_frag_packets, stationInfoValues[k].tx_frag_packets, stationInfoValues[k].rx_Rate);
		}
	}
	
	return CWAssembleMsgElem(msgPtr, CW_MSG_ELEMENT_VENDOR_SPEC_PAYLOAD_CW_TYPE);
}
CWBool CWAssembleWTPEventStaInfoRequest (CWProtocolMessage **messagesPtr, int *fragmentsNumPtr, int PMTU, int seqNum, int staCount, StationInfo *stationInfoValues) 	//pei add 0703
{
	CWProtocolMessage *msgElems= NULL;
	const int msgElemCount = 1;
	CWProtocolMessage *msgElemsBinding= NULL;
	const int msgElemBindingCount=0;
	
	if(messagesPtr == NULL || fragmentsNumPtr == NULL) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);

	CW_CREATE_PROTOCOL_MSG_ARRAY_ERR(msgElems, msgElemCount, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););	
	
	CWDebugLog("Assembling WTP Event Request AP station_info...");

	// Assemble Message Elements
	if (!(CWAssembleMsgElemStaInfo(msgElems, staCount, stationInfoValues))) {
		CW_FREE_OBJECT(msgElems);
		return CW_FALSE; // error will be handled by the caller
	}
	
	if(!(CWAssembleMessage(messagesPtr,
			       fragmentsNumPtr,
			       PMTU,
			       seqNum,
			       CW_MSG_TYPE_VALUE_WTP_EVENT_REQUEST,
			       msgElems,
			       msgElemCount,
			       msgElemsBinding,
			       msgElemBindingCount,
#ifdef CW_NO_DTLS
			       CW_PACKET_PLAIN
#else
			       (gDtlsSecurity == 1)?CW_PACKET_CRYPT:CW_PACKET_PLAIN           /* 0-CW_PACKET_PLAIN, 1-CW_PACKET_CRYPT */
#endif
			       ))) 
		return CW_FALSE;
	
	CWDebugLog("WTP Event Request station_info Assembled");

	return CW_TRUE;
}

CWBool CWAssembleMsgElemIfState(CWProtocolMessage *msgPtr, ifState *ifStateValues)             //pei add 0422
{
	int k;
	unsigned char value = IF_STATE;
	const int vendor_specific_payload_length = 3+2*(ifStateValues->eth_count+ifStateValues->wifi_count);

	if(msgPtr == NULL) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	
	CW_CREATE_PROTOCOL_MESSAGE(*msgPtr, vendor_specific_payload_length, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	
	CWProtocolStore8(msgPtr, value);
	CWProtocolStore8(msgPtr, ifStateValues->eth_count);

	dpf("value:%d eth_count:%d\n", value, ifStateValues->eth_count);
	if(ifStateValues->eth_count!=0)
	{
		for(k=0;k<ifStateValues->eth_count;k++)
		{
			CWProtocolStore8(msgPtr, ifStateValues->eth_status[k].id);
			CWProtocolStore8(msgPtr, ifStateValues->eth_status[k].status);
			
			dpf("id:%d status:%d\n", ifStateValues->eth_status[k].id, ifStateValues->eth_status[k].status);
		}
	}
	
	CWProtocolStore8(msgPtr, ifStateValues->wifi_count);
	dpf("wifi_count:%d\n", ifStateValues->wifi_count);
	if(ifStateValues->wifi_count!=0)
	{
		for(k=0;k<ifStateValues->wifi_count;k++)
		{
			CWProtocolStore8(msgPtr, ifStateValues->wifi_status[k].id);
			CWProtocolStore8(msgPtr, ifStateValues->wifi_status[k].status);
			
			dpf("id:%d status:%d\n", ifStateValues->wifi_status[k].id, ifStateValues->wifi_status[k].status);
		}
	}
	
	return CWAssembleMsgElem(msgPtr, CW_MSG_ELEMENT_VENDOR_SPEC_PAYLOAD_CW_TYPE);
}
CWBool CWAssembleWTPEventIfStateRequest (CWProtocolMessage **messagesPtr, int *fragmentsNumPtr, int PMTU, int seqNum, ifState *ifStateValues) 	//pei add 0422
{
	CWProtocolMessage *msgElems= NULL;
	const int msgElemCount = 1;
	CWProtocolMessage *msgElemsBinding= NULL;
	const int msgElemBindingCount=0;
	
	if(messagesPtr == NULL || fragmentsNumPtr == NULL) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);

	CW_CREATE_PROTOCOL_MSG_ARRAY_ERR(msgElems, msgElemCount, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););	
	
	CWDebugLog("Assembling WTP Event Request AP interface state...");

	// Assemble Message Elements
	if (!(CWAssembleMsgElemIfState(msgElems, ifStateValues))) {
		CW_FREE_OBJECT(msgElems);
		return CW_FALSE; // error will be handled by the caller
	}
	
	if(!(CWAssembleMessage(messagesPtr,
			       fragmentsNumPtr,
			       PMTU,
			       seqNum,
			       CW_MSG_TYPE_VALUE_WTP_EVENT_REQUEST,
			       msgElems,
			       msgElemCount,
			       msgElemsBinding,
			       msgElemBindingCount,
#ifdef CW_NO_DTLS
			       CW_PACKET_PLAIN
#else
			       (gDtlsSecurity == 1)?CW_PACKET_CRYPT:CW_PACKET_PLAIN           /* 0-CW_PACKET_PLAIN, 1-CW_PACKET_CRYPT */
#endif
			       ))) 
		return CW_FALSE;
	
	CWDebugLog("WTP Event Request interface state Assembled");

	return CW_TRUE;
}

CWBool CWAssembleMsgElemStaWapiInfo(CWProtocolMessage *msgPtr, int staCount, StationWapiInfo *staWapiInfoValues)
{
	int k;
	unsigned char value = STATION_WAPI_INFO;
	const int vendor_specific_payload_length = 2+29*staCount;

	if(msgPtr == NULL) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	
	CW_CREATE_PROTOCOL_MESSAGE(*msgPtr, vendor_specific_payload_length, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	
	CWProtocolStore8(msgPtr, value);
	CWProtocolStore8(msgPtr, staCount);

	dpf("value:%d staCount:%d\n", value, staCount);
	if(staCount!=0)
	{
		for(k=0;k<staCount;k++)
		{
			CWProtocolStore8(msgPtr, staWapiInfoValues[k].radioId);
			staWapiInfoValues[k].wlanId = CWGetRemoteWlanID(staWapiInfoValues[k].radioId, staWapiInfoValues[k].wlanId);
			CWProtocolStore8(msgPtr, staWapiInfoValues[k].wlanId);
			CWProtocolStoreRawBytes(msgPtr, (char *)staWapiInfoValues[k].mac, 6);
			CWProtocolStore32(msgPtr, staWapiInfoValues[k].version);
			CWProtocolStore8(msgPtr, staWapiInfoValues[k].controlledPortStatus);
			CWProtocolStore32(msgPtr, staWapiInfoValues[k].selectedUnicastCipher);
			CWProtocolStore32(msgPtr, staWapiInfoValues[k].wpiReplayCounters);
			CWProtocolStore32(msgPtr, staWapiInfoValues[k].wpiDecryptableErrors);
			CWProtocolStore32(msgPtr, staWapiInfoValues[k].wpiMICErrors);
			
			dpf("radioId:%d wlanId:%d mac:%02x-%02x-%02x-%02x-%02x-%02x version:%d controlledPortStatus:%d selectedUnicastCipher:%d wpiReplayCounters:%d wpiDecryptableErrors:%d wpiMICErrors:%d\n", 
				staWapiInfoValues[k].radioId, staWapiInfoValues[k].wlanId, staWapiInfoValues[k].mac[0], staWapiInfoValues[k].mac[1],
				staWapiInfoValues[k].mac[2], staWapiInfoValues[k].mac[3], staWapiInfoValues[k].mac[4], staWapiInfoValues[k].mac[5],
				staWapiInfoValues[k].version, staWapiInfoValues[k].controlledPortStatus, staWapiInfoValues[k].selectedUnicastCipher, 
				staWapiInfoValues[k].wpiReplayCounters, staWapiInfoValues[k].wpiDecryptableErrors, staWapiInfoValues[k].wpiMICErrors);
		}
	}
	
	return CWAssembleMsgElem(msgPtr, CW_MSG_ELEMENT_VENDOR_SPEC_PAYLOAD_CW_TYPE);
}
CWBool CWAssembleWTPEventStaWapiInfoRequest (CWProtocolMessage **messagesPtr, int *fragmentsNumPtr, int PMTU, int seqNum, int staCount, StationWapiInfo *staWapiInfoValues)
{
	CWProtocolMessage *msgElems= NULL;
	const int msgElemCount = 1;
	CWProtocolMessage *msgElemsBinding= NULL;
	const int msgElemBindingCount=0;
	
	if(messagesPtr == NULL || fragmentsNumPtr == NULL) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);

	CW_CREATE_PROTOCOL_MSG_ARRAY_ERR(msgElems, msgElemCount, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););	
	
	CWDebugLog("Assembling WTP Event Request AP station wapi info...");

	// Assemble Message Elements
	if (!(CWAssembleMsgElemStaWapiInfo(msgElems, staCount, staWapiInfoValues))) {
		CW_FREE_OBJECT(msgElems);
		return CW_FALSE; // error will be handled by the caller
	}
	
	if(!(CWAssembleMessage(messagesPtr,
			       fragmentsNumPtr,
			       PMTU,
			       seqNum,
			       CW_MSG_TYPE_VALUE_WTP_EVENT_REQUEST,
			       msgElems,
			       msgElemCount,
			       msgElemsBinding,
			       msgElemBindingCount,
#ifdef CW_NO_DTLS
			       CW_PACKET_PLAIN
#else
			       (gDtlsSecurity == 1)?CW_PACKET_CRYPT:CW_PACKET_PLAIN           /* 0-CW_PACKET_PLAIN, 1-CW_PACKET_CRYPT */
#endif
			       ))) 
		return CW_FALSE;
	
	CWDebugLog("WTP Event Request station wapi info Assembled");

	return CW_TRUE;
}
CWBool CWGetRadioScanResult_UseWTPD(int radio_id, char *ifname,unsigned int *_neighborApCount,Neighbor_AP_INFO **neighborApInfo)
{
	char cmdbuf[128];
	Neighbor_AP_INFO *s = NULL;
	unsigned char buf[24*1024];
	int i = 0;
	unsigned char *cp;
	int len = 0;
	unsigned int buf_size = 0, neighborApCount = 0;
	*_neighborApCount = 0;

	if(0 == Check_Interface_State(ifname))
	{
		CWSetInterfaceUp(ifname,1);
		sleep(5);
	}

	dpf("ap scanning......using %s\n", ifname);
	sprintf(cmdbuf, "iwlist %s scanning > /dev/null", ifname);
	_system(cmdbuf);
	sleep(5);
	dpf("after sleep of wifi%d scanning\n",radio_id);
	/* begin to gather rogue AP info */
	s = NULL;
	len = 0;
	buf_size = 0;
	i = 0;
	memset(buf, 0, 24*1024);

	len = get80211priv(ifname, IEEE80211_IOCTL_SCAN_RESULTS,
				buf, sizeof(buf));
	buf_size = len;
	printf("buf_size=%d\n",buf_size);
	if (len == -1)
		errx(1, "unable to get scan results");
	if (len < sizeof(struct ieee80211req_scan_result))
	{
		CWWTPDebugLog("---wifi1 get no vap in scanning");

		sleep(5);
		if(0 == Check_Interface_State(ifname))
		{
			CWSetInterfaceUp(ifname,0);
		}
		return CW_TRUE;//continue;
	}
	else
	{
		cp = buf;
		
		/* get num of neighbor AP */
		do {
			struct ieee80211req_scan_result *sr;

			sr = (struct ieee80211req_scan_result *) cp;
			cp += sr->isr_len, len -= sr->isr_len;
			if (gRadioInfoValue[1].radiotype&IEEE80211_PARAM_MODE_11a)
			{
				if(ieee80211_mhz2ieee(sr->isr_freq) < 14)
					continue;
			}
			else
			{
				if(ieee80211_mhz2ieee(sr->isr_freq) > 13)
					continue;
			}
			
			if(sr->isr_len == 0 || neighborApCount == 254){
				CWWTPDebugLog("%s,may be isr_len == 0, break",__func__);
				break;
			}
			
			neighborApCount++;
		} while (len >= sizeof(struct ieee80211req_scan_result));
		dpf("get the neighborApCount:%d\n", neighborApCount);
		
		if(neighborApCount == 0){
			CWWTPDebugLog("neighborApCount == 0,skip once to report, continue gather.");
			return CW_TRUE;//continue;
		}

		*_neighborApCount = neighborApCount;

		/* malloc the mem for neighbor AP info */
		CW_CREATE_ARRAY_ERR((*neighborApInfo), neighborApCount, Neighbor_AP_INFO, {CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL);printf("---6---goto exit_thread\n");return CW_FALSE;});
		for(i=0;i<neighborApCount;i++)
		{
			memset(&(*neighborApInfo)[i], 0, sizeof(Neighbor_AP_INFO));
		}

		len = buf_size;
		cp = buf;
		i = 0;
		do {
			char capstring[32];
			char temp_buf[32];
			int buf_len = 0;
			struct ieee80211req_scan_result *sr;
			u_int8_t *vp;
			sr = (struct ieee80211req_scan_result *) cp;
			vp = (u_int8_t *)(sr+1);
			if (gRadioInfoValue[1].radiotype&IEEE80211_PARAM_MODE_11a)
			{
				if(ieee80211_mhz2ieee(sr->isr_freq) < 14)
				{
					cp += sr->isr_len, len -= sr->isr_len;
					continue;
				}
			}
			else
			{
				if(ieee80211_mhz2ieee(sr->isr_freq) > 13)
				{
					cp += sr->isr_len, len -= sr->isr_len;
					continue;
				}
			}
			memcpy((*neighborApInfo)[i].BSSID, sr->isr_bssid, 6);
			(*neighborApInfo)[i].Rate = (unsigned short)getmaxrate(sr->isr_rates, sr->isr_nrates);
			(*neighborApInfo)[i].Channel = (unsigned char)ieee80211_mhz2ieee(sr->isr_freq);
			(*neighborApInfo)[i].RSSI = (unsigned char) sr->isr_rssi;
			(*neighborApInfo)[i].NOISE = sr->isr_noise;
			(*neighborApInfo)[i].BEACON_INT = sr->isr_intval;
			(*neighborApInfo)[i].Opstatus = NEW;
			(*neighborApInfo)[i].CapabilityInfo = sr->isr_capinfo;
			(*neighborApInfo)[i].EssidLen = sr->isr_ssid_len;

			CW_CREATE_STRING_ERR((*neighborApInfo)[i].ESSID, (*neighborApInfo)[i].EssidLen, {CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL);printf("---4---goto exit_thread\n");return CW_FALSE;});
			memset((*neighborApInfo)[i].ESSID, 0 , (*neighborApInfo)[i].EssidLen+1);
			memcpy((*neighborApInfo)[i].ESSID, vp, sr->isr_ssid_len);
	//			printf("1---ESSID:%s\n", neighborApInfo[i].ESSID);
	//			neighborApInfo[i].IeLen = sr->isr_ie_len;
			memset(capstring,0,sizeof(capstring));
			getcaps(sr->isr_capinfo, capstring);
			(*neighborApInfo)[i].IeLen = strlen(capstring);

			getencryption(vp + sr->isr_ssid_len, sr->isr_ie_len, temp_buf, &buf_len);
			(*neighborApInfo)[i].IeLen += buf_len;
			
			CW_CREATE_STRING_ERR((*neighborApInfo)[i].IEs_INFO, (*neighborApInfo)[i].IeLen, {CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL);printf("---5---goto exit_thread\n");return CW_FALSE;});
			memset((*neighborApInfo)[i].IEs_INFO, 0 , (*neighborApInfo)[i].IeLen+1);
			memcpy((*neighborApInfo)[i].IEs_INFO, capstring, (*neighborApInfo)[i].IeLen);
			strcat((*neighborApInfo)[i].IEs_INFO, temp_buf);
			
			(*neighborApInfo)[i].next = NULL;

			i++;
			if(i == neighborApCount ||i == 254){
				CWWTPDebugLog("%s,may be isr_len == 0, Skip once.");
				break;
			}
			cp += sr->isr_len, len -= sr->isr_len;
		} while (len >= sizeof(struct ieee80211req_scan_result));
	}
	return CW_TRUE;
}

CWBool CWRadioScan_UseWTPD(int radio_id)
{
	CWBool isFirstTime = CW_TRUE;
	Neighbor_AP_INFO *head = NULL;  /* for wifi1 */
	Neighbor_AP_INFO *tail = NULL;  /* for wifi1 */
	Neighbor_AP_INFO *p = NULL;
	char scanifname[32]={0};
	unsigned int neighborApCount = 0;
	Neighbor_AP_INFO *s = NULL;
	int i = 0;
	Neighbor_AP_INFO *neighborApInfo;
	CWProtocolMessage *messages = NULL;
	int fragmentsNum=0;
	int seqNum;

	if(radio_id == 0){
		memcpy(scanifname,_wtpd_scan_if_name_for_wifi0_,sizeof(_wtpd_scan_if_name_for_wifi0_));
	}else if (radio_id == 1){
		memcpy(scanifname,_wtpd_scan_if_name_for_wifi1_,sizeof(_wtpd_scan_if_name_for_wifi1_));
	}
	
	if(CW_FALSE == CWGetRadioScanResult_UseWTPD(radio_id, scanifname, &neighborApCount, &neighborApInfo))
	{
		CWWTPDebugLog("CWGetRadioScanResult_UseWTPD(rid=%d) return false. go exit_thread.",radio_id);
		return CW_FALSE;//goto exit_thread;
	}else if(0 == neighborApCount){
		CWWTPDebugLog("CWGetRadioScanResult_UseWTPD(rid=%d) return true. but rrm count = 0,continue to gather.",radio_id);
		return CW_TRUE;//continue;
	};

	/* add for create a list for saving neighbor AP info */
	dpf("Begin to set link list for saving wifi%d neighbor AP info!\n",radio_id);
	if(isFirstTime||(head == NULL))
	{
		for(i=0;i<neighborApCount;i++)
		{
			s = NULL;
			CW_CREATE_OBJECT_ERR(s, Neighbor_AP_INFO, {CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL);printf("---6---goto exit_thread\n");return CW_FALSE;});
			memset(s, 0, sizeof(Neighbor_AP_INFO));
			s->ESSID = NULL;
			s->IEs_INFO = NULL;
			s->next = NULL;
			memcpy(s, &neighborApInfo[i], sizeof(Neighbor_AP_INFO));
			CW_CREATE_STRING_ERR(s->ESSID, neighborApInfo[i].EssidLen, {CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL);printf("---7---goto exit_thread\n");return CW_FALSE;});
			CW_CREATE_STRING_ERR(s->IEs_INFO, neighborApInfo[i].IeLen, {CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL);printf("---8---goto exit_thread\n");return CW_FALSE;});
			memset(s->ESSID, 0, neighborApInfo[i].EssidLen+1);
			memset(s->IEs_INFO, 0, neighborApInfo[i].IeLen+1);
			memcpy(s->ESSID, neighborApInfo[i].ESSID, neighborApInfo[i].EssidLen);
			memcpy(s->IEs_INFO, neighborApInfo[i].IEs_INFO, neighborApInfo[i].IeLen);
			
			if(i == 0)
			{
				head = s;
				p = s;
			}
			else
			{
				p->next = s;
				p = s;
			}
		}
		tail = s;
	}
	else
	{
	//			printf("is not the first time\n");
		p = head;
		while(p!=NULL)
		{
			p->Opstatus = DELETE;
			p = p->next;
		}
		for(i=0;i<neighborApCount;i++)
		{
			neighborApInfo[i].Opstatus = NEW;
			p = head;
			while(p!=NULL)
			{
				if(memcmp(p->BSSID, neighborApInfo[i].BSSID, 6)==0)
				{
					if((p->Channel == neighborApInfo[i].Channel)
						&&((p->RSSI > (neighborApInfo[i].RSSI-5))&&(p->RSSI < (neighborApInfo[i].RSSI+5)))
						&&(p->NOISE == neighborApInfo[i].NOISE)
						&&(p->EssidLen == neighborApInfo[i].EssidLen)
						&&(strcmp(p->ESSID, neighborApInfo[i].ESSID)==0))
					{
						p->Opstatus = REMAIN;
						neighborApInfo[i].Opstatus = REMAIN;
					}
					else
					{
						p->Opstatus = MODIFIED;
						neighborApInfo[i].Opstatus = MODIFIED;

						Neighbor_AP_INFO *temp = p->next;
						CW_FREE_OBJECT(p->ESSID);
						CW_FREE_OBJECT(p->IEs_INFO);
						memcpy(p, &neighborApInfo[i], sizeof(Neighbor_AP_INFO));
						CW_CREATE_STRING_ERR(p->ESSID, neighborApInfo[i].EssidLen, {CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL);printf("---9---goto exit_thread\n");return CW_FALSE;});
						CW_CREATE_STRING_ERR(p->IEs_INFO, neighborApInfo[i].IeLen, {CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL);printf("---10---goto exit_thread\n");return CW_FALSE;});
						memset(p->ESSID, 0, neighborApInfo[i].EssidLen+1);
						memset(p->IEs_INFO, 0, neighborApInfo[i].IeLen+1);
						memcpy(p->ESSID, neighborApInfo[i].ESSID, neighborApInfo[i].EssidLen);
						memcpy(p->IEs_INFO, neighborApInfo[i].IEs_INFO, neighborApInfo[i].IeLen);
						p->next = temp;
					}
					break;
				}
				p = p->next;
			}
		}

		for(i=0;i<neighborApCount;i++)
		{
			if(neighborApInfo[i].Opstatus == NEW)
			{
				CW_CREATE_OBJECT_ERR(s, Neighbor_AP_INFO, {CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL);printf("---11---goto exit_thread\n");return CW_FALSE;});
				memset(s, 0, sizeof(Neighbor_AP_INFO));
				s->ESSID = NULL;
				s->IEs_INFO = NULL;
				s->next = NULL;
				memcpy(s, &neighborApInfo[i], sizeof(Neighbor_AP_INFO));
				CW_CREATE_STRING_ERR(s->ESSID, neighborApInfo[i].EssidLen, {CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL);printf("---12---goto exit_thread\n");return CW_FALSE;});
				CW_CREATE_STRING_ERR(s->IEs_INFO, neighborApInfo[i].IeLen, {CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL);printf("---13---goto exit_thread\n");return CW_FALSE;});
				memset(s->ESSID, 0, neighborApInfo[i].EssidLen+1);
				memset(s->IEs_INFO, 0, neighborApInfo[i].IeLen+1);
				memcpy(s->ESSID, neighborApInfo[i].ESSID, neighborApInfo[i].EssidLen);
				memcpy(s->IEs_INFO, neighborApInfo[i].IEs_INFO, neighborApInfo[i].IeLen);
				s->next = NULL;    /* pei add to test the bug 1208 */
				if(tail != NULL)
					tail->next = s;
				tail = s;
			}
		}

	}

	dpf("Free the neighborApInfo%d[]!\n",radio_id);
	for(i = 0; i < neighborApCount; i++)
	{
		CW_FREE_OBJECT(neighborApInfo[i].ESSID);
		CW_FREE_OBJECT(neighborApInfo[i].IEs_INFO);
	}
	CW_FREE_OBJECT(neighborApInfo);  //pei modified 0227

	if(!isFirstTime)
	{
		/* pei add 1125, if is not the first time to test neighbor AP, the neighbor AP info list should be changed, so recount the neighbor AP transmit to AC */
		p=head;
		neighborApCount = 0;
		while(p!=NULL)
		{
			if((p->Opstatus == NEW)||(p->Opstatus == DELETE)||(p->Opstatus == MODIFIED))
				neighborApCount++;
			p = p->next;
		}
		dpf("---recount the num of wifi%d neighbor AP send to AC:%d\n", radio_id, neighborApCount);
	}

	seqNum = CWGetSeqNum();

	if(!CWAssembleWTPEventApScanningRequest(&messages, &fragmentsNum, gWTPPathMTU, seqNum, &neighborApCount, head, radio_id))	/*pei test for neighbor AP info list. 1125*/
	{
		CWDebugLog("Assemble WTP Event Ap Scanning Request error!\n");
		dpf("---wifi%d-14---goto exit_thread\n",radio_id);
		return CW_FALSE;
	}

	/* send rogue AP info */
	dpf("Ready to send ap wifi%d scanning infos\n",radio_id);

	for(i = 0; i < fragmentsNum; i++) 
	{
		dpf("wifi%d messages[%d].offset:%d\n", radio_id, i, messages[i].offset);
	//						CWCaptrue(messages[i].offset, (unsigned char *)(messages[i].msg));
#ifndef CW_NO_DTLS
		if(gDtlsSecurity == 1)
		{
			if(!CWSecuritySend(gWTPSession, messages[i].msg, messages[i].offset))
			{
				CWDebugLog("Error sending message\n");
				CWFreeMessageFragments(messages, fragmentsNum);
				CW_FREE_OBJECT(messages);
				CWWTPDebugLog("Error sending ap scanning message");
				return CW_FALSE;
			}
		}
		else
#endif
		{
			if (!CWNetworkSendUnsafeUnconnected(gWTPSocket, &(gACInfoPtr->preferredAddress), messages[i].msg, messages[i].offset))
			{
				CWDebugLog("Error sending message\n");
				CWFreeMessageFragments(messages, fragmentsNum);
				CW_FREE_OBJECT(messages);
				CWWTPDebugLog("Error sending ap scanning message");
				return CW_FALSE;
			}
		}
	}
	CWDebugLog("Message Sent\n");
	CWFreeMessageFragments(messages, fragmentsNum);
	CW_FREE_OBJECT(messages);

	/* pei add 1125, if is not the first time to test neighbor AP, the neighbor AP info list should be changed, so update the neighbor AP list */
	if(!isFirstTime)
	{
		dpf("Update the link list saving neighbor AP info!\n");
		p=head;
		Neighbor_AP_INFO *last;
		while(p!=NULL)
		{
			if(debug_print)
			{
				int n=0;
				printf("\nwifi%d update---BSSID:",radio_id);
				for(n=0;n<6;n++)
				{
					printf("%02x ", p->BSSID[n]);
				}
			}

			if(p->Opstatus == DELETE)
			{
				if(p == head)
				{
					if(p->next == NULL)
					{
						tail = head = NULL;	 /* pei change tail=head; to tail=head=NULL; test the bug 1208 */
						CW_FREE_OBJECT(p->ESSID);
						CW_FREE_OBJECT(p->IEs_INFO);
						p->next = NULL;
						CW_FREE_OBJECT(p);
						break;
					}
					else
					{
						head=head->next;
						CW_FREE_OBJECT(p->ESSID);
						CW_FREE_OBJECT(p->IEs_INFO);
						p->next = NULL;
						CW_FREE_OBJECT(p);
						p=head;
						continue;	 /* pei add to test the bug 1208 */
					}
				}
				else
				{
					if(p->next == NULL)
					{
						tail = last;
						last->next = NULL;
						CW_FREE_OBJECT(p->ESSID);
						CW_FREE_OBJECT(p->IEs_INFO);
						p->next = NULL;
						CW_FREE_OBJECT(p);
						break;
					}
					else
					{
						last->next = p->next;
						CW_FREE_OBJECT(p->ESSID);
						CW_FREE_OBJECT(p->IEs_INFO);
						p->next = NULL;
						CW_FREE_OBJECT(p);
						p = last;
					}
				}
			}
			last = p;
			p = p->next;
		}
	}

	if(debug_print)
	{
		p = head;
		while(p!=NULL)
		{
			int n=0;
			printf("\nwifi1updated-BSSID:");
			for(n=0;n<6;n++)
			{
				printf("%02x ", p->BSSID[n]);
			}
			p = p->next;
		}
		printf("\n");
	}

	return CW_TRUE;
}

void dpf_rrm(unsigned char* cp)
{
	char capstring[32];
	char temp_buf[32];
	int buf_len = 0;
	Neighbor_AP_INFO n;
	dpf("\n%s:\n",__func__);
	struct ieee80211req_scan_result *sr;
	u_int8_t *vp;
	sr = (struct ieee80211req_scan_result *) cp;
	//CWCaptrue(sr->isr_len,cp);
	dpf("n.isr_len:%d ",sr->isr_len);
	vp = (u_int8_t *)(sr+1);
	memcpy(n.BSSID, sr->isr_bssid, 6);
	dpf("BSSID:"MAC_FMT" ",MAC2STR(n.BSSID));
	n.Rate = (unsigned short)getmaxrate(sr->isr_rates, sr->isr_nrates);
	dpf("n.Rate:%04x ",n.Rate);
	n.Channel = (unsigned char)ieee80211_mhz2ieee(sr->isr_freq);
	dpf("n.Channel:%02x ",n.Channel);
	n.RSSI = (unsigned char) sr->isr_rssi;
	dpf("n.RSSI:%02x ",n.RSSI);
	n.NOISE = sr->isr_noise;
	dpf("n.NOISE:%02x ",n.NOISE);
	n.BEACON_INT = sr->isr_intval;
	dpf("n.BEACON_INT:%02x ",n.BEACON_INT);
	n.Opstatus = NEW;
//	dpf("n.Opstatus:%02x ",n.Opstatus);
	n.CapabilityInfo = sr->isr_capinfo;
	dpf("n.CapabilityInfo:%04x ",n.CapabilityInfo);
	n.EssidLen = sr->isr_ssid_len;
	dpf("n.EssidLen:%d ",n.EssidLen);

	CW_CREATE_STRING_ERR(n.ESSID, n.EssidLen, {CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL);CWWTPDebugLog("create memory error");;});
	memset(n.ESSID, 0 , n.EssidLen+1);
	memcpy(n.ESSID, vp, sr->isr_ssid_len);
	memset(capstring,0,sizeof(capstring));
	getcaps(sr->isr_capinfo, capstring);
	n.IeLen = strlen(capstring);
	dpf("n.IeLen:%d ",n.IeLen);

	getencryption(vp + sr->isr_ssid_len, sr->isr_ie_len, temp_buf, &buf_len);
	n.IeLen += buf_len;
	dpf("n.IeLen:%d ",n.IeLen);
	CW_CREATE_STRING_ERR(n.IEs_INFO, n.IeLen, {CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL);CWWTPDebugLog("create memory error");;});
	memset(n.IEs_INFO, 0 , n.IeLen+1);
	memcpy(n.IEs_INFO, capstring, n.IeLen);
	strcat(n.IEs_INFO, temp_buf);
//	dpf(" n.Rate:%d,n.Channel:%d,n.RSSI:%d,n.EssidLen:%d,n.ESSID:%s,n.IeLen:%d\n",n.Rate,n.Channel,n.RSSI,n.EssidLen,n.ESSID,n.IeLen);
//	dpf(" n.Rate:%02x,n.Channel:%02x,n.RSSI:%02x,n.EssidLen:%02x,n.IeLen:%02x\n",n.Rate,n.Channel,n.RSSI,n.EssidLen,n.IeLen);
	n.next = NULL;
	CW_FREE_OBJECT(n.ESSID);
	CW_FREE_OBJECT(n.IEs_INFO);

}

CW_THREAD_RETURN_TYPE CWWTPSetApScanning(void *arg)
{
	if(pthread_detach(pthread_self())!=0)
	printf("##########detach error!############\n");
	CWBool isFirstTime = CW_TRUE;
	CWBool isWifi1 = CW_FALSE;  /* for wifi1 */
	unsigned short reportintval;
	char ifname[IFNAMSIZ];
	char ifname1[IFNAMSIZ];  /* for wifi1 */
	Neighbor_AP_INFO *head = NULL;
	Neighbor_AP_INFO *head1 = NULL;  /* for wifi1 */
	Neighbor_AP_INFO *tail = NULL;
	Neighbor_AP_INFO *p = NULL;
	char cmdbuf[128];
	unsigned char usewtpdscan = 0;
	CWWTPWlan *ptr = NULL;
	int radiocount = 1;
	int radionum = 0;
	char wifi0mode[10]={0};
	char wifi1mode[10]={0};
	
	//reportintval = (unsigned short)arg;
	
	if(arg!=NULL)
	{
		CWWTPDebugLog("into run---start the ap scanning thread---config in config state---sleep first");
		sleep((int)arg);
	}
	else
		sleep(5);

	if(0 ==access("/jffs/use_wtpd_scan", 0))
		usewtpdscan = 1;
	if(usewtpdscan)
	{
		if(Check_Interface_State("wifi1")==0)
		{
			isWifi1 = CW_TRUE;
		}
	}
	else
	{
		if(Check_Interface_State("wifi1")==0)
		{
			isWifi1 = CW_TRUE;
			radiocount += 1;
		}
		_system("killall iwlist_scan > /dev/null 2>&1");
		
		if(0 != Check_Interface_State(_iwlist_scan_if_name_for_wifi0_))
		{
			for(ptr = wtp_wlan_list ; ptr != NULL ; ptr = ptr->next){
				if(ptr->radio_id == 0)
				{
					ptr->wlan_updown_time++;  //pei add 0225
					CWWTPGetWlanName(ifname,ptr->radio_id,ptr->wlan_id);
					CWSetInterfaceUp(ifname,0);
				}
			}
			strcpy(ifname, _iwlist_scan_if_name_for_wifi0_);
			CWCreateWlanInterface(ifname,0);
			CWSetHideSsid(ifname,1);
			CWSetRadioType(0,0,gRadioInfoValue[0].radiotype);
			for(ptr = wtp_wlan_list ; ptr != NULL ; ptr = ptr->next){
				if(ptr->radio_id == 0)
				{
					ptr->wlan_updown_time++;  //pei add 0225
					CWWTPGetWlanName(ifname,ptr->radio_id,ptr->wlan_id);
					CWSetInterfaceUp(ifname,1);;
				}
			}
			CWSetInterfaceUp(ifname,1);
		}

		if(isWifi1)
		{	
			if(0 != Check_Interface_State(_iwlist_scan_if_name_for_wifi1_))
			{
				gSetScanWifiRadioType = 1;
				
				for(ptr = wtp_wlan_list ; ptr != NULL ; ptr = ptr->next){
					if(ptr->radio_id == 1)
					{
						ptr->wlan_updown_time++;  //pei add 0225
						CWWTPGetWlanName(ifname,ptr->radio_id,ptr->wlan_id);
						CWSetInterfaceUp(ifname,0);
					}
				}
				strcpy(ifname, _iwlist_scan_if_name_for_wifi1_);
				CWCreateWlanInterface(ifname,0);
				CWSetHideSsid(ifname,1);
				CWSetRadioType(1,1,gRadioInfoValue[1].radiotype);
				
				for(ptr = wtp_wlan_list ; ptr != NULL ; ptr = ptr->next){
					if(ptr->radio_id == 1) 
					{
						ptr->wlan_updown_time++;  
						CWWTPGetWlanName(ifname,ptr->radio_id,ptr->wlan_id);
						CWSetInterfaceUp(ifname,1);
					}
				}
				CWSetInterfaceUp(ifname,1);
				CWWTPDebugLog("Create %s OK",ifname);
				gSetScanWifiRadioType = 0;
			}
		}
		
	 	memset(cmdbuf, 0, 128);
		if((gRadioInfoValue[0].radiotype&IEEE80211_PARAM_MODE_11a)!=0)
			memcpy(wifi0mode,"11a",sizeof("11a"));
		else
			memcpy(wifi0mode,"11bg",sizeof("11bg"));
		isWifi1 = 0;
		if(isWifi1)
		{
			if((gRadioInfoValue[1].radiotype&IEEE80211_PARAM_MODE_11a)!=0)
				memcpy(wifi1mode,"11a",sizeof("11a"));
			else
				memcpy(wifi1mode,"11bg",sizeof("11bg"));
			sprintf(cmdbuf, "/usr/sbin/iwlist_scan -m %s -c %s -I %d -i 5 -n 2 > /dev/null 2>&1 &",wifi0mode,wifi1mode,gApScanningInterval/3>60?gApScanningInterval/3:60);
		}
		else 
			sprintf(cmdbuf, "/usr/sbin/iwlist_scan -m %s  -I %d -i 5 -n 1 > /dev/null 2>&1 &",wifi0mode,gApScanningInterval/3>60?gApScanningInterval/3:60);
		_system(cmdbuf);
	}
	CW_REPEAT_FOREVER 
	{
		/* pei add 1125, if disable the neighbor AP scanning, the neighbor AP info list should be free, and exit the thread. */
		if(!gApScanningEnable)
		{
			CWWTPDebugLog("disable ap scanning, exit thread");
			goto exit_thread;
		}
		reportintval = gApScanningInterval;
		dpf("reportintval:%d\n", reportintval);
				
		if(usewtpdscan)
		{
			CWWTPWlan *ptr = NULL;
			unsigned char found_wlan_in_fir_radio = 0;
			unsigned char found_wlan_in_sec_radio = 0;

			for(ptr=wtp_wlan_list;ptr != NULL;ptr = ptr->next)
			{
				if((ptr->wlan_id != 0)&&(ptr->radio_id == 0))
				{
					CWWTPGetWlanName(ifname, ptr->radio_id, ptr->wlan_id);
					found_wlan_in_fir_radio = 1;
					if(0 == Check_Interface_State(_wtpd_scan_if_name_for_wifi0_))
					{
						CWSetInterfaceUp(_wtpd_scan_if_name_for_wifi0_,0);
						CWDestroyWlanInterface(_wtpd_scan_if_name_for_wifi0_);
					}
				}
			}
			if(0 == found_wlan_in_fir_radio)
			{
				memset(ifname, 0, IFNAMSIZ);
				sprintf(ifname, _wtpd_scan_if_name_for_wifi0_);
				if(Check_Interface_State(ifname)!=0)
				{
					CWCreateWlanInterface(ifname,0);
					CWSetEssid(ifname,"*#$_$#*");
					CWSetHideSsid(ifname,1);
				}
				
			}
			if(isWifi1)
			{
				for(ptr=wtp_wlan_list;ptr != NULL;ptr = ptr->next)
				{
					if((ptr->wlan_id != 0)&&(ptr->radio_id == 1))
					{
						CWWTPGetWlanName(ifname1, ptr->radio_id, ptr->wlan_id);
						found_wlan_in_sec_radio = 1;
						if(0 == Check_Interface_State(_wtpd_scan_if_name_for_wifi1_))
						{
							CWSetInterfaceUp(_wtpd_scan_if_name_for_wifi1_,0);
							CWDestroyWlanInterface(_wtpd_scan_if_name_for_wifi1_);
						}
					}
				}
				if(0 == found_wlan_in_sec_radio)
				{
					memset(ifname1, 0, IFNAMSIZ);
					sprintf(ifname1, _wtpd_scan_if_name_for_wifi1_);
					if(Check_Interface_State(ifname1)!=0)
					{
						CWCreateWlanInterface(ifname1,1);
						CWSetEssid(ifname,"*#$_$#*1");
						CWSetHideSsid(ifname1,1);
						
						printf("no wlan in second radio, set radiotype!\n");
						set_radiotype_cmd1(1, ifname1, gRadioInfoValue[1].radiotype);
					}
				}
			}
			
		}
			
		if(!gApScanningEnable)
		{
			CWWTPDebugLog("disable ap scanning, exit thread");
			goto exit_thread;
		}
		
		/* begin to gather rogue AP info */
		unsigned int neighborApCount = 0;
		Neighbor_AP_INFO *s = NULL;
		unsigned char buf[24*1024];
		int i = 0;
		Neighbor_AP_INFO *neighborApInfo;
		CWProtocolMessage *messages = NULL;
		int fragmentsNum=0;
		int seqNum;
		
		if(!usewtpdscan)
		{
			/* get iwlist ap info from other process, for example, iwlist_scan */
			int sockfd = -1;
			int readBytes = 0;
			int type = 0;
			struct timeval timeout;
			fd_set readfds;
			CWProtocolMessage msgPtr;
			struct sockaddr_in addr_from;
			struct sockaddr_in serv_addr;
			
			sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
			if(sockfd < 0)
			{
				CWWTPDebugLog("Failed to create socket in ap scanning, continue");
				continue;
			}

			memset(&serv_addr, 0, sizeof(serv_addr));
			serv_addr.sin_family = AF_INET;
			serv_addr.sin_port = htons(5248);
			serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

			int nRet = 0;
			if((nRet = sendto(sockfd, &buf[0], 1, 0,(struct sockaddr *)&serv_addr, sizeof(struct sockaddr_in))) == -1)
			{
				CWWTPDebugLog("send error\n");
				close(sockfd);
				continue;
			}
			dpf("send request ok....................nRet:%d\n",nRet);
			
			FD_ZERO(&readfds);
			FD_SET(sockfd, &readfds);
			timeout.tv_sec = 15;
			timeout.tv_usec = 0;
			int ret = select(sockfd+1, &readfds, NULL, NULL, &timeout);
			if(ret < 0)
			{
				close(sockfd);
				CWWTPDebugLog("select error");
			}
			else if(ret == 0)
			{
				close(sockfd);
				CWWTPDebugLog("receive time out");
			}
			else
			{
				if(!CWErr(CWNetworkReceiveUnsafe(sockfd, (char *)buf, 24*1024, 0, (CWNetworkLev4Address *)&addr_from, &readBytes))) 
				{
					if (CWErrorGetLastErrorCode() == CW_ERROR_INTERRUPTED)
					{
						close(sockfd);
						continue;
					}
				}
				close(sockfd);
				dpf("receive a msg....................\n");
	//			CWCaptrue(readBytes, buf);
				
				msgPtr.msg = (char *)buf;
				msgPtr.offset = 0;
				type= CWProtocolRetrieve8(&msgPtr);
				neighborApCount = CWProtocolRetrieve8(&msgPtr);
				dpf("type:%d  All neighborapcount:%d\n", type, neighborApCount);
				if(neighborApCount != 0)
				{
					CW_CREATE_ARRAY_ERR(neighborApInfo, neighborApCount, Neighbor_AP_INFO, {CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL);printf("---6---goto exit_thread\n");goto exit_thread;});
					for(i=0;i<neighborApCount;i++)
					{
						memset(&neighborApInfo[i], 0, sizeof(Neighbor_AP_INFO));
						void *ptr = NULL;
						ptr =  (unsigned char *)CWProtocolRetrieveRawBytes(&msgPtr,6);
						CW_COPY_MEMORY(neighborApInfo[i].BSSID, ptr, 6);
						CW_FREE_OBJECT(ptr);
						neighborApInfo[i].Channel = CWProtocolRetrieve8(&msgPtr);
						neighborApInfo[i].RSSI = CWProtocolRetrieve8(&msgPtr);
						neighborApInfo[i].NOISE = CWProtocolRetrieve8(&msgPtr);
						neighborApInfo[i].BEACON_INT = CWProtocolRetrieve8(&msgPtr);
						neighborApInfo[i].Opstatus = NEW;
						msgPtr.offset +=1;
						neighborApInfo[i].EssidLen = CWProtocolRetrieve8(&msgPtr);

						CW_CREATE_STRING_ERR(neighborApInfo[i].ESSID, neighborApInfo[i].EssidLen, {CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL);CWWTPDebugLog("create memory error");goto exit_thread;});
						memset(neighborApInfo[i].ESSID, 0 , neighborApInfo[i].EssidLen+1);
						ptr = NULL;
						ptr =  CWProtocolRetrieveRawBytes(&msgPtr, neighborApInfo[i].EssidLen);
						CW_COPY_MEMORY(neighborApInfo[i].ESSID, ptr, neighborApInfo[i].EssidLen);
						CW_FREE_OBJECT(ptr);
						//printf("%d---ESSID:%s\n", i, neighborApInfo[i].ESSID);
						
						neighborApInfo[i].IeLen = CWProtocolRetrieve8(&msgPtr);
						CW_CREATE_STRING_ERR(neighborApInfo[i].IEs_INFO, neighborApInfo[i].IeLen, {CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL);CWWTPDebugLog("create memory error");goto exit_thread;});
						memset(neighborApInfo[i].IEs_INFO, 0 , neighborApInfo[i].IeLen+1);
						ptr = NULL;
						ptr =  CWProtocolRetrieveRawBytes(&msgPtr, neighborApInfo[i].IeLen);
						CW_COPY_MEMORY(neighborApInfo[i].IEs_INFO, ptr, neighborApInfo[i].IeLen);
						CW_FREE_OBJECT(ptr);
						//printf("%d---IEs_INFO:%s\n", i, neighborApInfo[i].IEs_INFO);
						
						neighborApInfo[i].Rate = CWProtocolRetrieve16(&msgPtr);
						neighborApInfo[i].CapabilityInfo = CWProtocolRetrieve16(&msgPtr);
						neighborApInfo[i].Radio_id = CWProtocolRetrieve8(&msgPtr);
						neighborApInfo[i].next = NULL;
					}
				}
			}
		}
		else
		{	
			/* use wtpd self to scan, and get rrm result */
			if(CW_FALSE == CWGetRadioScanResult_UseWTPD(0, _wtpd_scan_if_name_for_wifi0_, &neighborApCount, &neighborApInfo))
			{
				CWWTPDebugLog("CWGetRadioScanResult_UseWTPD return false. go exit_thread.");
				goto exit_thread;
			}else if(0 == neighborApCount){
				CWWTPDebugLog("CWGetRadioScanResult_UseWTPD return true. but rrm count = 0,continue to gather.");
				continue;
			};
		}
		/* add for create a list for saving neighbor AP info */
		dpf("Begin to set link list for saving neighbor AP info!\n");
		if(isFirstTime||(head == NULL))
		{
			for(i=0;i<neighborApCount;i++)
			{
				s = NULL;
				CW_CREATE_OBJECT_ERR(s, Neighbor_AP_INFO, {CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL);CWWTPDebugLog("create memory error");goto exit_thread;});
				memset(s, 0, sizeof(Neighbor_AP_INFO));
				s->ESSID = NULL;
				s->IEs_INFO = NULL;
				s->next = NULL;
				memcpy(s, &neighborApInfo[i], sizeof(Neighbor_AP_INFO));
				CW_CREATE_STRING_ERR(s->ESSID, neighborApInfo[i].EssidLen, {CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL);CWWTPDebugLog("create memory error");goto exit_thread;});
				CW_CREATE_STRING_ERR(s->IEs_INFO, neighborApInfo[i].IeLen, {CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL);CWWTPDebugLog("create memory error");goto exit_thread;});
				memset(s->ESSID, 0, neighborApInfo[i].EssidLen+1);
				memset(s->IEs_INFO, 0, neighborApInfo[i].IeLen+1);
				memcpy(s->ESSID, neighborApInfo[i].ESSID, neighborApInfo[i].EssidLen);
				memcpy(s->IEs_INFO, neighborApInfo[i].IEs_INFO, neighborApInfo[i].IeLen);
				s->Radio_id 		=  neighborApInfo[i].Radio_id;
				if(i == 0)
				{
					head = s;
					p = s;
				}
				else
				{
					p->next = s;
					p = s;
				}
			}
			tail = s;
		}
		else
		{
//			printf("is not the first time\n");
			p = head;
			while(p!=NULL)
			{
				p->Opstatus = DELETE;
				p = p->next;
			}
			for(i=0;i<neighborApCount;i++)
			{
				neighborApInfo[i].Opstatus = NEW;
				p = head;
				while(p!=NULL)
				{
					if(memcmp(p->BSSID, neighborApInfo[i].BSSID, 6)==0)
					{
						if((p->Channel == neighborApInfo[i].Channel)
							&&((p->RSSI > (neighborApInfo[i].RSSI-5))&&(p->RSSI < (neighborApInfo[i].RSSI+5)))
							&&(p->NOISE == neighborApInfo[i].NOISE)
							&&(p->EssidLen == neighborApInfo[i].EssidLen)
							&&(strcmp(p->ESSID, neighborApInfo[i].ESSID)==0)
							&&(p->Radio_id == neighborApInfo[i].Radio_id)
						)
						{
							p->Opstatus = REMAIN;
							neighborApInfo[i].Opstatus = REMAIN;
						}
						else
						{
							p->Opstatus = MODIFIED;
							neighborApInfo[i].Opstatus = MODIFIED;
		
							Neighbor_AP_INFO *temp = p->next;
							CW_FREE_OBJECT(p->ESSID);
							CW_FREE_OBJECT(p->IEs_INFO);
							memcpy(p, &neighborApInfo[i], sizeof(Neighbor_AP_INFO));
							CW_CREATE_STRING_ERR(p->ESSID, neighborApInfo[i].EssidLen, {CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL);CWWTPDebugLog("create memory error");goto exit_thread;});
							CW_CREATE_STRING_ERR(p->IEs_INFO, neighborApInfo[i].IeLen, {CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL);CWWTPDebugLog("create memory error");goto exit_thread;});
							memset(p->ESSID, 0, neighborApInfo[i].EssidLen+1);
							memset(p->IEs_INFO, 0, neighborApInfo[i].IeLen+1);
							memcpy(p->ESSID, neighborApInfo[i].ESSID, neighborApInfo[i].EssidLen);
							memcpy(p->IEs_INFO, neighborApInfo[i].IEs_INFO, neighborApInfo[i].IeLen);
							p->Radio_id 		= neighborApInfo[i].Radio_id;
							p->next = temp;
						}
						break;
					}
					p = p->next;
				}
			}

			for(i=0;i<neighborApCount;i++)
			{
				if(neighborApInfo[i].Opstatus == NEW)
				{
					CW_CREATE_OBJECT_ERR(s, Neighbor_AP_INFO, {CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL);CWWTPDebugLog("create memory error");goto exit_thread;});
					memset(s, 0, sizeof(Neighbor_AP_INFO));
					s->ESSID = NULL;
					s->IEs_INFO = NULL;
					s->next = NULL;
					memcpy(s, &neighborApInfo[i], sizeof(Neighbor_AP_INFO));
					CW_CREATE_STRING_ERR(s->ESSID, neighborApInfo[i].EssidLen, {CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL);CWWTPDebugLog("create memory error");goto exit_thread;});
					CW_CREATE_STRING_ERR(s->IEs_INFO, neighborApInfo[i].IeLen, {CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL);CWWTPDebugLog("create memory error");goto exit_thread;});
					memset(s->ESSID, 0, neighborApInfo[i].EssidLen+1);
					memset(s->IEs_INFO, 0, neighborApInfo[i].IeLen+1);
					memcpy(s->ESSID, neighborApInfo[i].ESSID, neighborApInfo[i].EssidLen);
					memcpy(s->IEs_INFO, neighborApInfo[i].IEs_INFO, neighborApInfo[i].IeLen);
					s->Radio_id		= neighborApInfo[i].Radio_id;
					s->next = NULL;    /* pei add to test the bug 1208 */
					if(tail!=NULL)
						tail->next = s;
					tail = s;
				}
			}

		}
		
		dpf("Free the neighborApInfo[]!\n");
		for(i = 0; i < neighborApCount; i++)
		{
			CW_FREE_OBJECT(neighborApInfo[i].ESSID);
			CW_FREE_OBJECT(neighborApInfo[i].IEs_INFO);
		}
		CW_FREE_OBJECT(neighborApInfo);  //pei modified 0227

		if(!isFirstTime)
		{
			/* pei add 1125, if is not the first time to test neighbor AP, the neighbor AP info list should be changed, so recount the neighbor AP transmit to AC */
			p=head;
			neighborApCount = 0;
			while(p!=NULL)
			{
				if((p->Opstatus == NEW)||(p->Opstatus == DELETE)||(p->Opstatus == MODIFIED))
					neighborApCount++;
				p = p->next;
			}
			dpf("---recount the num of All neighbor AP send to AC:%d\n", neighborApCount);
		}

		for(radionum = 0;radionum<radiocount;radionum++)
		{
				messages = NULL;
				fragmentsNum = 0;
				seqNum = CWGetSeqNum();

				if(!CWAssembleWTPEventApScanningRequest(&messages, &fragmentsNum, gWTPPathMTU, seqNum, &neighborApCount, head, (unsigned char)radionum))  /*pei test for neighbor AP info list. 1125*/
				{
					CWDebugLog("Assemble WTP Event Ap Scanning Request error!\n");
					CWWTPDebugLog("Assemble WTP Event Ap Scanning Request error!");
					goto exit_thread;
				}

				/* send rogue AP info */
		dpf("Ready to send ap scanning infos\n");

		for(i = 0; i < fragmentsNum; i++) 
		{
//			dpf("messages[%d].offset:%d\n", i, messages[i].offset);
//			CWCaptrue(messages[i].offset, (unsigned char *)(messages[i].msg));

#ifndef CW_NO_DTLS
					if(gDtlsSecurity == 1)
					{
						if(!CWSecuritySend(gWTPSession, messages[i].msg, messages[i].offset))
						{
							CWDebugLog("Error sending message\n");
							CWFreeMessageFragments(messages, fragmentsNum);
							CW_FREE_OBJECT(messages);
							CWWTPDebugLog("Error sending ap scanning message");
							goto exit_thread;
						}
					}
					else
#endif
					{
						if (!CWNetworkSendUnsafeUnconnected(gWTPSocket, &(gACInfoPtr->preferredAddress), messages[i].msg, messages[i].offset))
						{
							CWDebugLog("Error sending message\n");
							CWFreeMessageFragments(messages, fragmentsNum);
							CW_FREE_OBJECT(messages);
							CWWTPDebugLog("Error sending ap scanning message");
							goto exit_thread;
						}
					}
				}
				CWDebugLog("Radio %d Message be Sent",radionum);
				dpf("Radio %d Message be Sent\n",radionum);
				CWFreeMessageFragments(messages, fragmentsNum);
				CW_FREE_OBJECT(messages);
		}
		
		
		/* pei add 1125, if is not the first time to test neighbor AP, the neighbor AP info list should be changed, so update the neighbor AP list */
		if(!isFirstTime)
		{
			dpf("Update the link list saving neighbor AP info!\n");
			p=head;
			Neighbor_AP_INFO *last;
			while(p!=NULL)
			{
				if(debug_print)
				{
					int n=0;
					printf("\nupdate---BSSID:");
					for(n=0;n<6;n++)
					{
						printf("%02x ", p->BSSID[n]);
					}
				}

				if(p->Opstatus == DELETE)
				{
					if(p == head)
					{
						if(p->next == NULL)
						{
							tail = head = NULL;    /* pei change tail=head; to tail=head=NULL; test the bug 1208 */
							CW_FREE_OBJECT(p->ESSID);
							CW_FREE_OBJECT(p->IEs_INFO);
							p->next = NULL;
							CW_FREE_OBJECT(p);
							break;
						}
						else
						{
							head=head->next;
							CW_FREE_OBJECT(p->ESSID);
							CW_FREE_OBJECT(p->IEs_INFO);
							p->next = NULL;
							CW_FREE_OBJECT(p);
							p=head;
							continue;    /* pei add to test the bug 1208 */
						}
					}
					else
					{
						if(p->next == NULL)
						{
							tail = last;
							last->next = NULL;
							CW_FREE_OBJECT(p->ESSID);
							CW_FREE_OBJECT(p->IEs_INFO);
							p->next = NULL;
							CW_FREE_OBJECT(p);
							break;
						}
						else
						{
							last->next = p->next;
							CW_FREE_OBJECT(p->ESSID);
							CW_FREE_OBJECT(p->IEs_INFO);
							p->next = NULL;
							CW_FREE_OBJECT(p);
							p = last;
						}
					}
				}
				last = p;
				p = p->next;
			}
		}

		if(debug_print)
		{
			p = head;
			while(p!=NULL)
			{
				int n=0;
				printf("\nupdated-BSSID:");
				for(n=0;n<6;n++)
				{
					printf("%02x ", p->BSSID[n]);
				}
				p = p->next;
			}
			printf("\n");
		}

#if 1 /* add scanning for wifi1 */
		if(usewtpdscan && isWifi1)
		{
			if(!CWRadioScan_UseWTPD(1)){
				CWWTPDebugLog("CWRadioScan_UseWTPD(rid=1) return false. go exit_thread.");
				goto exit_thread;
			}else{
				continue;
			};
		}
#endif

		/* pei add 1128, if disable the neighbor AP scanning, the neighbor AP info list should be free, and exit the thread. */
		if(!gApScanningEnable)
		{
			CWWTPDebugLog("disable ap scanning, exit thread");
			goto exit_thread;
		}
		if(usewtpdscan)
		{
			if((0 == strcmp(ifname, _wtpd_scan_if_name_for_wifi0_))&&(0 == Check_Interface_State(ifname)))
			{
				CWSetInterfaceUp(ifname,0);
			}
			if(isWifi1)
			{
				if((0 == strcmp(ifname1, _wtpd_scan_if_name_for_wifi1_))&&(0 == Check_Interface_State(ifname1)))
				{
					CWSetInterfaceUp(ifname1,0);
				}
				sleep(reportintval-25);
			}
			else
			{
				sleep(reportintval-25);
			}
		}
		else
			sleep(reportintval);

		isFirstTime = CW_FALSE;
	}

exit_thread:
	p = head;
	while(p!=NULL)
	{
		head = p->next;
		CW_FREE_OBJECT(p->ESSID);
		CW_FREE_OBJECT(p->IEs_INFO);
		p->next = NULL;
		CW_FREE_OBJECT(p);
		p = head;
	}
	if(isWifi1)
	{
		p = head1;
		while(p!=NULL)
		{
			head1 = p->next;
			CW_FREE_OBJECT(p->ESSID);
			CW_FREE_OBJECT(p->IEs_INFO);
			p->next = NULL;
			CW_FREE_OBJECT(p);
			p = head1;
		}
	}
	CWWTPDebugLog("Exit the thread for ap scan!");
	if((0 == strcmp(ifname, _wtpd_scan_if_name_for_wifi0_))&&(0 == Check_Interface_State(ifname)))
	{
		CWSetInterfaceUp(ifname,0);
		CWDestroyWlanInterface(ifname);
	}
	if(isWifi1)
	{
		if((0 == strcmp(ifname1, _wtpd_scan_if_name_for_wifi1_))&&(0 == Check_Interface_State(ifname1)))
		{
			CWSetInterfaceUp(ifname1,0);
			CWDestroyWlanInterface(ifname1);
		}
	}
	gApScanningEnable = 0;
	CWExitThread();
	return NULL;
}

CWBool CWSetApScanning(ApScanningSetValues *apScanningSetValues)
{
	char ifname[IFNAMSIZ];
	CWWTPWlan *ptr = NULL;
	char cmdbuf[128];
	memset(cmdbuf, 0, 128);
	CWWTPDebugLog("got msg to set AP scanning, begin to set...");
	if(apScanningSetValues->value!=APScanning)
	{
		return CW_FALSE;
	}

	if(apScanningSetValues->opstate == 0)
	{
		CWWTPDebugLog("set AP Scanning disable");
		if(gApScanningEnable == 1)
		{
			if(0 == Check_Interface_State(_wtpd_scan_if_name_for_wifi0_))
			{
				CWSetInterfaceUp(_wtpd_scan_if_name_for_wifi0_,0);
				CWDestroyWlanInterface(_wtpd_scan_if_name_for_wifi0_);
			}
			if(0 == Check_Interface_State(_wtpd_scan_if_name_for_wifi1_))
			{
				CWSetInterfaceUp(_wtpd_scan_if_name_for_wifi1_,0);
				CWDestroyWlanInterface(_wtpd_scan_if_name_for_wifi1_);
			}
			for(ptr = wtp_wlan_list ; ptr != NULL ; ptr = ptr->next){
				if(ptr->radio_id == 0)
				{
					ptr->wlan_updown_time++;  //pei add 0225
					CWWTPGetWlanName(ifname,ptr->radio_id,ptr->wlan_id);
					CWSetInterfaceUp(ifname,0);
				}
			}
			_system("killall iwlist_scan > /dev/null 2>&1");
			if(0 == Check_Interface_State(_iwlist_scan_if_name_for_wifi0_))
		 	{
				CWSetInterfaceUp(_iwlist_scan_if_name_for_wifi0_,0);
				CWDestroyWlanInterface(_iwlist_scan_if_name_for_wifi0_);
			}	
			for(ptr = wtp_wlan_list ; ptr != NULL ; ptr = ptr->next){
				if(ptr->radio_id == 0)
				{
					ptr->wlan_updown_time++;  //pei add 0225
					CWWTPGetWlanName(ifname,ptr->radio_id,ptr->wlan_id);
					 CWSetInterfaceUp(ifname,1);
				}
			}
			if(0 == Check_Interface_State(_iwlist_scan_if_name_for_wifi1_))
			{
				CWSetInterfaceUp(_iwlist_scan_if_name_for_wifi1_,0);
				CWDestroyWlanInterface(_iwlist_scan_if_name_for_wifi1_);
			}
			
			if( (thread_apScanning != 0) && (0 == pthread_kill(thread_apScanning, 0) ) ) {
				pthread_cancel(thread_apScanning);
			}
//			gApScanningThreadEnable = 0;
			gApScanningEnable = 0;
			CWWTPDebugLog("set OK!");
		}
		else
		{
			CWWTPDebugLog("AP Scanning is already disable");
		}
	}
	else if(apScanningSetValues->opstate == 1)
	{
		CWWTPDebugLog("set AP Scanning enable");
		if(gApScanningEnable)
		{
//			gApScanningThreadEnable = 1;
			if(debug_print)
			{
				if(gApScanningInterval != apScanningSetValues->reportinterval)
					printf("set AP scanning interval changed %d to %d\n", gApScanningInterval, apScanningSetValues->reportinterval);
			}
			CWWTPDebugLog("AP Scanning is already enable, set interval %d to %d", gApScanningInterval, apScanningSetValues->reportinterval);
			gApScanningInterval = apScanningSetValues->reportinterval;
		}
		else
		{
			gApScanningEnable = 1;
//			gApScanningThreadEnable = 1;
			gApScanningInterval = apScanningSetValues->reportinterval;
			CWWTPDebugLog("set OK!");
			if( (thread_apScanning != 0) && (0 == pthread_kill(thread_apScanning, 0) ) ) {
				pthread_cancel(thread_apScanning);
//				pthread_join(thread_apScanning,NULL);
			}
			if(!CWErr(CWCreateThread(&thread_apScanning, CWWTPSetApScanning, NULL,0))) {  //pei change (void *)apScanningSetValues->reportinterval  to NULL 1127
				CWDebugLog("Error starting Thread that Scanning rogue AP");
				CWWTPDebugLog("Error starting Thread that Scanning rogue AP");
				return CW_FALSE;
			}
		}
		
	}
	
	ConfArg confArg;
	memset(&confArg,0,sizeof(confArg));
	strcpy(confArg.conf_arg,"gApScanningEnable");
	sprintf(confArg.conf_value,"%d",gApScanningEnable);
	CWSaveConfigInfo(CONF_WTPD,confArg,CW_TRUE);
	
	memset(&confArg,0,sizeof(confArg));
	strcpy(confArg.conf_arg,"gApScanningInterval");
	sprintf(confArg.conf_value,"%hd",gApScanningInterval);
	CWSaveConfigInfo(CONF_WTPD,confArg,CW_FALSE);
	return CW_TRUE;
}

CWBool CWSetStaThroughput(StaThroughputSetValues *staThroughputSetValues)
{
	char cmdbuf[128];
	
	if(staThroughputSetValues->opstate == 0)
	{
		memset(cmdbuf,0,128);
		sprintf(cmdbuf,"iwpriv ath.%d-%d set_lowtraf_en %d",staThroughputSetValues->radioid,staThroughputSetValues->wlanid,staThroughputSetValues->opstate);
		_system(cmdbuf);
	}
	else
	{
		memset(cmdbuf,0,128);
#ifdef MARVELL_PLATFORM     //zhouke add ,for marvell ioclt no support 16bytes;

		sprintf(cmdbuf,"iwpriv ath.%d-%d set_lowtraf_tim %d",staThroughputSetValues->radioid,staThroughputSetValues->wlanid,staThroughputSetValues->stainterval);
		_system(cmdbuf);
		memset(cmdbuf,0,128);
		sprintf(cmdbuf,"iwpriv ath.%d-%d set_lowtraf_thr %d",staThroughputSetValues->radioid,staThroughputSetValues->wlanid,staThroughputSetValues->throughput_threshold);
		_system(cmdbuf);
        
#else

		sprintf(cmdbuf,"iwpriv ath.%d-%d set_lowtraf_time %d",staThroughputSetValues->radioid,staThroughputSetValues->wlanid,staThroughputSetValues->stainterval);
		_system(cmdbuf);
		memset(cmdbuf,0,128);
		sprintf(cmdbuf,"iwpriv ath.%d-%d set_lowtraf_thre %d",staThroughputSetValues->radioid,staThroughputSetValues->wlanid,staThroughputSetValues->throughput_threshold);
		_system(cmdbuf);
        
#endif
		memset(cmdbuf,0,128);
		sprintf(cmdbuf,"iwpriv ath.%d-%d set_lowtraf_en %d",staThroughputSetValues->radioid,staThroughputSetValues->wlanid,staThroughputSetValues->opstate);
		_system(cmdbuf);
	}
	return CW_TRUE;
}

CW_THREAD_RETURN_TYPE CWWTPSetApMonitor(void *arg)  //pei add 0226
{
	if(pthread_detach(pthread_self())!=0)
	printf("##########detach error!############\n");

	unsigned char reportintval;
	
//	reportintval = (unsigned short)arg;
	
	CW_REPEAT_FOREVER 
	{
		int i=0;
		char wlanname[IFNAMSIZ];
		int ethCount = 0;
		int wifiCount = 0;
//		int monitorApCount = 0;
		int eth_throughput = 0;
		int wifi_throughput = 0;
		wlan_stats_info *wlanStatsInfo;
		
		/* if disable the neighbor AP Monitor, exit the thread. */
		if(!gApMonitorEnable)
		{
			CWWTPDebugLog("disable ap monitor, exit thread");
			goto exit_thread;
		}
		reportintval = gApMonitorInterval;
		dpf("reportintval:%d\n", reportintval);

		sleep(reportintval);
		
		/* get eth and wifi interface throughput info */
		CWGetInterfaceCount(&ethCount, "eth[0-9]");
		//CWGetInterfaceCount(&wifiCount, "wifi[0-9]");
		wifiCount = CWGetRadioMaxCount();
		if(wifiCount == 0)
			wifiCount = 1;
//		monitorApCount = ethCount+wifiCount;
//		wlan_stats_info wlanStatsInfo[ethCount+wifiCount];
		CW_CREATE_ARRAY_ERR(wlanStatsInfo, ethCount+wifiCount, wlan_stats_info, CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
		
		for(i=0;i<ethCount;i++)
		{
			memset(wlanname, 0, IFNAMSIZ);
			sprintf(wlanname, "eth%d", i);
			memset(&wlanStatsInfo[i], 0, sizeof(wlan_stats_info));
			wlanStatsInfo[i].type = ETH;
			wlanStatsInfo[i].radioId = 0;
			wlanStatsInfo[i].wlanId = i;
			get_interface_throughput_fields(wlanname, &wlanStatsInfo[i]);
			eth_throughput+=wlanStatsInfo[i].rx_bytes;
			eth_throughput+=wlanStatsInfo[i].tx_bytes;
		}
		for(i=0;i<wifiCount;i++)
		{
			memset(wlanname, 0, IFNAMSIZ);
			sprintf(wlanname, "wifi%d", i);
			memset(&wlanStatsInfo[ethCount+i], 0, sizeof(wlan_stats_info));
			wlanStatsInfo[ethCount+i].type = WIFI;
			wlanStatsInfo[ethCount+i].radioId = 0;
			wlanStatsInfo[ethCount+i].wlanId = i;
			get_interface_throughput_fields(wlanname, &wlanStatsInfo[ethCount+i]);
			wifi_throughput+=wlanStatsInfo[ethCount+i].rx_bytes;
			wifi_throughput+=wlanStatsInfo[ethCount+i].tx_bytes;
		}

		/* assemble WTP Event request element with info of AP monitor */
		CWProtocolMessage *messages = NULL;
		int fragmentsNum=0;
		int seqNum;
		seqNum = CWGetSeqNum();

		if(!CWAssembleWTPEventApMonitorRequest(&messages, &fragmentsNum, gWTPPathMTU, seqNum, eth_throughput, wifi_throughput))
		{
			CWDebugLog("Assemble WTP Event Ap Monitor Request error!\n");
			CWWTPDebugLog("Assemble WTP Event Ap Monitor Request error!");
			CW_FREE_OBJECT(wlanStatsInfo);
			goto exit_thread;
		}
		CW_FREE_OBJECT(wlanStatsInfo);
		
		/* send Monitor AP info */
		for(i = 0; i < fragmentsNum; i++) 
		{
//			dpf("messages[%d].offset:%d\n", i, messages[i].offset);
//			CWCaptrue(messages[i].offset, (unsigned char *)(messages[i].msg));

#ifndef CW_NO_DTLS
			if(gDtlsSecurity == 1)
			{
				if(!CWSecuritySend(gWTPSession, messages[i].msg, messages[i].offset))
				{
					CWDebugLog("Error sending message\n");
					CWFreeMessageFragments(messages, fragmentsNum);
					CW_FREE_OBJECT(messages);
					CWWTPDebugLog("Error sending message ap monitor");
					goto exit_thread;
				}
			}
			else
#endif
			{
				if (!CWNetworkSendUnsafeUnconnected(gWTPSocket, &(gACInfoPtr->preferredAddress), messages[i].msg, messages[i].offset))
				{
					CWDebugLog("Error sending message\n");
					CWFreeMessageFragments(messages, fragmentsNum);
					CW_FREE_OBJECT(messages);
					CWWTPDebugLog("Error sending message ap monitor");
					goto exit_thread;
				}
			}
		}
		CWDebugLog("Message Sent\n");
		CWFreeMessageFragments(messages, fragmentsNum);
		CW_FREE_OBJECT(messages);

		/* if disable the AP Monitor, exit the thread. */
		if(!gApMonitorEnable)
		{
			CWWTPDebugLog("disable ap monitor, exit thread");
			goto exit_thread;
		}
		
	}

exit_thread:
	CWWTPDebugLog("Exit the thread of ap monitor");
	CWExitThread();
	return NULL;
}
CWBool CWSetApMonitor(ApMonitorSetValues *apMonitorSetValues)
{
	CWWTPDebugLog("got msg to set AP Monitor, begin to set...");
	if(apMonitorSetValues->value!=APMonitor)
		return CW_FALSE;

	if(apMonitorSetValues->opstate == 0)
	{
		CWWTPDebugLog("set AP Monitor disable");
		if(gApMonitorEnable == 1)
		{
			if( (thread_apMonitor != 0) && (0 == pthread_kill(thread_apMonitor, 0) ) ) {
				pthread_cancel(thread_apMonitor);
			}
			gApMonitorEnable = 0;
			CWWTPDebugLog("set OK!");
		}
	}
	else if(apMonitorSetValues->opstate == 1)
	{
		CWWTPDebugLog("set AP Monitor enable");
		if(gApMonitorEnable)
		{
			if(debug_print)
			{
				if(gApMonitorInterval != apMonitorSetValues->reportinterval)
					printf("set AP Monitor interval changed %d to %d\n", gApMonitorInterval, apMonitorSetValues->reportinterval);
			}
			CWWTPDebugLog("AP Monitor is already enable, interval %d to %d", gApMonitorInterval, apMonitorSetValues->reportinterval);
			gApMonitorInterval = apMonitorSetValues->reportinterval;
		}
		else
		{
			gApMonitorInterval = apMonitorSetValues->reportinterval;
			gApMonitorEnable = 1;
			if( (thread_apMonitor != 0) && (0 == pthread_kill(thread_apMonitor, 0) ) ) {
				pthread_cancel(thread_apMonitor);
//				pthread_join(thread_apMonitor,NULL);
			}			
			if(!CWErr(CWCreateThread(&thread_apMonitor, CWWTPSetApMonitor, NULL,0))) {
				CWDebugLog("Error starting Thread to Monitor");
				return CW_FALSE;
			}
		}
		
	}
	
	ConfArg confArg;
	memset(&confArg,0,sizeof(confArg));
	strcpy(confArg.conf_arg,"gApMonitorEnable");
	sprintf(confArg.conf_value,"%d",gApMonitorEnable);
	CWSaveConfigInfo(CONF_WTPD,confArg,CW_TRUE);
	
	memset(&confArg,0,sizeof(confArg));
	strcpy(confArg.conf_arg,"gApMonitorInterval");
	sprintf(confArg.conf_value,"%d",gApMonitorInterval);
	CWSaveConfigInfo(CONF_WTPD,confArg,CW_FALSE);
	return CW_TRUE;
}
CW_THREAD_RETURN_TYPE CWWTPSetExtraInfo(void *arg)  //pei add 0226
{
	if(pthread_detach(pthread_self())!=0)
	printf("##########detach error!############\n");
	unsigned short reportintval;
	int j;
	int eth_count;
	_system("/usr/sbin/set_crontab /usr/sbin/cpu_util_rate m0 >/dev/null 2>&1 &");
	_system("/usr/sbin/set_crontab /usr/sbin/cpu_util_rate m1 30 >/dev/null 2>&1 &");
	_system("killall linkcheck");
	CWGetInterfaceCount(&eth_count, "eth[0-9]");
//	reportintval = (unsigned short)arg;
	for(j=0;j<eth_count;j++)		//diaowq add,to reduce the high cpu_rate caused by linkcheck when testing throughout,110301
	{
		CWEnable_linkcheck(j);
	}
	
	CW_REPEAT_FOREVER 
	{
		int i=0;
		extra_info extraInfoValues;
		eth_package_info ethPackageInfoValues;
//		int ath_count;
		char ifname[IFNAMSIZ];
		CWWTPWlan *ptr = NULL;
		
		/* if disable the AP ExtraInfo, exit the thread. */
		if(!gExtraInfoEnable)
		{
			CWWTPDebugLog("disable extra info");
			goto exit_thread;
		}
		reportintval = gExtraInfoInterval;
		dpf("reportintval:%d\n", reportintval);
		
		sleep(reportintval);
		
		/* get eth and wifi interface throughput info */
		memset(&extraInfoValues, 0, sizeof(extra_info));
		extraInfoValues.eth_count = eth_count;
		extraInfoValues.wifi_count = CWGetRadioMaxCount();
		if(extraInfoValues.wifi_count == 0)
		{
			extraInfoValues.wifi_count = 1;
#ifdef CAVIUM_PLATFORM
			extraInfoValues.wifi_count = 2;
#else
#ifdef AP2400_IFM
			extraInfoValues.wifi_count = 2;
#endif
#endif
		}
		CW_CREATE_ARRAY_ERR(extraInfoValues.eth_updown_time, extraInfoValues.eth_count, interface_updown_time, CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
		CW_CREATE_ARRAY_ERR(extraInfoValues.wifi_status, extraInfoValues.wifi_count, interface_status, CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););

		for(i=0;i<extraInfoValues.eth_count;i++)
			memset(&extraInfoValues.eth_updown_time[i], 0, sizeof(interface_updown_time));
		
		for(i=0;i<extraInfoValues.wifi_count;i++)
			memset(&extraInfoValues.wifi_status[i], 0, sizeof(interface_status));
		CWGetEthUpdownTime(&extraInfoValues);
		CWGetWifiStatus(&extraInfoValues); //pei add 0415

		for(i=0;i<extraInfoValues.wifi_count;i++)
		{
			memset(ifname, 0, IFNAMSIZ);
			sprintf(ifname, "wifi%d", i);
			CWGetExtraInfo(ifname, &extraInfoValues);
		}

		extraInfoValues.get_Ip_mode = gIPMode;
		CWGetMemUseRatio(&(extraInfoValues.mem_total), &(extraInfoValues.mem_use_ratio));
		CWGetFlashSizeInfo(&(extraInfoValues.flash_total), &(extraInfoValues.flash_free));
		CWGetCpuUseRatio(&(extraInfoValues.cpu_use_ratio));
		extraInfoValues.snr = CWGetSnr();
		extraInfoValues.temperature = CWGetTemperature();

		for(i=0;i<extraInfoValues.eth_count;i++) /* pei add for eth at 090624 */
		{
			memset(ifname, 0, IFNAMSIZ);
			sprintf(ifname, "eth%d", i);
//#ifdef CAVIUM_PLATFORM //yuanjh delete for ap of other platforms.
			get_eth_fields(ifname, &ethPackageInfoValues);
			dpf("ifname:%s tx_unicast:%d tx_broadcast:%d tx_multicast:%d tx_drop:%d rx_unicast:%d rx_broadcast:%d rx_multicast:%d rx_drop:%d\n", 
				ifname, ethPackageInfoValues.tx_unicast, ethPackageInfoValues.tx_broadcast, ethPackageInfoValues.tx_multicast, ethPackageInfoValues.tx_drop, 
				ethPackageInfoValues.rx_unicast, ethPackageInfoValues.rx_broadcast, ethPackageInfoValues.rx_multicast, ethPackageInfoValues.rx_drop);
//#endif
			extraInfoValues.tx_unicast += ethPackageInfoValues.tx_unicast;
			extraInfoValues.tx_broadcast += ethPackageInfoValues.tx_broadcast;
			extraInfoValues.tx_multicast += ethPackageInfoValues.tx_multicast;
			extraInfoValues.tx_drop += ethPackageInfoValues.tx_drop;
			extraInfoValues.rx_unicast += ethPackageInfoValues.rx_unicast;
			extraInfoValues.rx_broadcast += ethPackageInfoValues.rx_broadcast;
			extraInfoValues.rx_multicast += ethPackageInfoValues.rx_multicast;
			extraInfoValues.rx_drop += ethPackageInfoValues.rx_drop;
		}
		/* get ath info */
//		CWGetInterfaceCount(&ath_count, "ath.[0-9]-[0-9]*");
		wpi_errors *wpi_error_values;
		disassoc_errors *disassoc_error_values;
		
		extraInfoValues.ath_count = wtp_wlan_count;
		CW_CREATE_ARRAY_ERR(extraInfoValues.ath_updown_time, extraInfoValues.ath_count, interface_updown_time, CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
		CW_CREATE_ARRAY_ERR(wpi_error_values, extraInfoValues.ath_count, wpi_errors, CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
		CW_CREATE_ARRAY_ERR(disassoc_error_values, extraInfoValues.ath_count, disassoc_errors, CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
		for(i=0;i<extraInfoValues.ath_count;i++)
		{
			memset(&extraInfoValues.ath_updown_time[i], 0, sizeof(interface_updown_time));
			memset(&wpi_error_values[i], 0, sizeof(wpi_errors));
			memset(&disassoc_error_values[i], 0, sizeof(disassoc_errors));
		}
		i = 0;
		for(ptr = wtp_wlan_list ; ptr != NULL ; ptr = ptr->next){
			extraInfoValues.ath_updown_time[i].radio_id = ptr->radio_id;
			extraInfoValues.ath_updown_time[i].id = ptr->wlan_id;
			extraInfoValues.ath_updown_time[i].updown_time = ptr->wlan_updown_time;

			memset(ifname, 0, IFNAMSIZ);
			CWWTPGetWlanName(ifname, ptr->radio_id, ptr->wlan_id);

			CWGetWpiErrors(ifname, &wpi_error_values[i]);
			CWGetDisassocErrors(ifname, &disassoc_error_values[i]);

			extraInfoValues.wpi_replay_error += wpi_error_values[i].wpi_replay_error;
			extraInfoValues.wpi_decryp_error += wpi_error_values[i].wpi_decryp_error;
			extraInfoValues.wpi_mic_error += wpi_error_values[i].wpi_mic_error;
			extraInfoValues.disassoc_unnormal += disassoc_error_values[i].disassoc_unnormal;
			extraInfoValues.rx_assoc_norate += disassoc_error_values[i].rx_assoc_norate;
			extraInfoValues.rx_assoc_capmismatch += disassoc_error_values[i].rx_assoc_capmismatch;
			extraInfoValues.assoc_invalid += disassoc_error_values[i].assoc_invalid;
			extraInfoValues.reassoc_deny += disassoc_error_values[i].reassoc_deny;
			i++;
		}
		/* assemble WTP Event request element with info of AP ExtraInfo */
		CWProtocolMessage *messages = NULL;
		int fragmentsNum=0;
		int seqNum;
		seqNum = CWGetSeqNum();

		if(!CWAssembleWTPEventExtraInfoRequest(&messages, &fragmentsNum, gWTPPathMTU, seqNum, &extraInfoValues))  /*pei test for neighbor AP info list. 1125*/
		{
			CWDebugLog("Assemble WTP Event Ap ExtraInfo Request error!\n");
			CWWTPDebugLog("Assemble WTP Event Ap ExtraInfo Request error!");
			CW_FREE_OBJECT(extraInfoValues.eth_updown_time);
			CW_FREE_OBJECT(extraInfoValues.ath_updown_time);
			CW_FREE_OBJECT(extraInfoValues.wifi_status);
			CW_FREE_OBJECT(wpi_error_values);
			CW_FREE_OBJECT(disassoc_error_values);
			goto exit_thread;
		}
		CW_FREE_OBJECT(extraInfoValues.eth_updown_time);
		CW_FREE_OBJECT(extraInfoValues.ath_updown_time);
		CW_FREE_OBJECT(extraInfoValues.wifi_status);
		CW_FREE_OBJECT(wpi_error_values);
		CW_FREE_OBJECT(disassoc_error_values);
		
		/* send ExtraInfo info */
		for(i = 0; i < fragmentsNum; i++) 
		{
//			dpf("messages[%d].offset:%d\n", i, messages[i].offset);
//			CWCaptrue(messages[i].offset, (unsigned char *)(messages[i].msg));

#ifndef CW_NO_DTLS
			if(gDtlsSecurity == 1)
			{
				if(!CWSecuritySend(gWTPSession, messages[i].msg, messages[i].offset))
				{
					CWDebugLog("Error sending message\n");
					CWFreeMessageFragments(messages, fragmentsNum);
					CW_FREE_OBJECT(messages);
					CWWTPDebugLog("Error sending extra info message");
					goto exit_thread;
				}
			}
			else
#endif
			{
				if (!CWNetworkSendUnsafeUnconnected(gWTPSocket, &(gACInfoPtr->preferredAddress), messages[i].msg, messages[i].offset))
				{
					CWDebugLog("Error sending message\n");
					CWFreeMessageFragments(messages, fragmentsNum);
					CW_FREE_OBJECT(messages);
					CWWTPDebugLog("Error sending extra info message");
					goto exit_thread;
				}
			}
		}
		CWDebugLog("Message Sent\n");
		CWFreeMessageFragments(messages, fragmentsNum);
		CW_FREE_OBJECT(messages);

		/* if disable the AP ExtraInfo, exit the thread. */
		if(!gExtraInfoEnable)
		{
			CWWTPDebugLog("disable extra info");
			goto exit_thread;
		}
		
	}

exit_thread:
	CWWTPDebugLog("Exit the thread of extra info!");
	CWExitThread();
	return NULL;
}
CWBool CWSetApExtraInfo(ExtraInfoValues *extraInfoValues)
{
	CWWTPDebugLog("got msg to set AP ExtraInfo, begin to set...");
	if(extraInfoValues->value!=InterfaceUpdown)
		return CW_FALSE;

	if(extraInfoValues->opstate == 0)
	{
		CWWTPDebugLog("set AP ExtraInfo disable");
		if(gExtraInfoEnable == 1)
		{
			_system("/usr/sbin/set_crontab /usr/sbin/cpu_util_rate m0 >/dev/null 2>&1 &");
			if( (thread_extraInfo != 0) && (0 == pthread_kill(thread_extraInfo, 0) ) ) {
				pthread_cancel(thread_extraInfo);
			}
			_system("killall linkcheck");
			gExtraInfoEnable = 0;
			CWWTPDebugLog("set OK!");
		}
	}
	else if(extraInfoValues->opstate == 1)
	{
		CWWTPDebugLog("set AP ExtraInfo enable");
		if(gExtraInfoEnable)
		{
			if(debug_print)
			{
				if(gExtraInfoInterval!= extraInfoValues->reportinterval)
					printf("set AP ExtraInfo interval changed %d to %d\n", gExtraInfoInterval, extraInfoValues->reportinterval);
			}
			CWWTPDebugLog("AP ExtraInfo is already enable, interval %d to %d", gExtraInfoInterval, extraInfoValues->reportinterval);
			gExtraInfoInterval = extraInfoValues->reportinterval;
		}
		else
		{
			gExtraInfoInterval = extraInfoValues->reportinterval;
			gExtraInfoEnable = 1;
			if( (thread_extraInfo != 0) && (0 == pthread_kill(thread_extraInfo, 0) ) ) {
				pthread_cancel(thread_extraInfo);		
//				pthread_join(thread_extraInfo,NULL);
			}
			if(!CWErr(CWCreateThread(&thread_extraInfo, CWWTPSetExtraInfo, NULL,0))) {
				CWDebugLog("Error starting Thread to get ExtraInfo");
				return CW_FALSE;
			}
		}
		
	}
	
	ConfArg confArg;
	memset(&confArg,0,sizeof(confArg));
	strcpy(confArg.conf_arg,"gExtraInfoEnable");
	sprintf(confArg.conf_value,"%d",gExtraInfoEnable);
	CWSaveConfigInfo(CONF_WTPD,confArg,CW_TRUE);
	
	memset(&confArg,0,sizeof(confArg));
	strcpy(confArg.conf_arg,"gExtraInfoInterval");
	sprintf(confArg.conf_value,"%hd",gExtraInfoInterval);
	CWSaveConfigInfo(CONF_WTPD,confArg,CW_FALSE);
	return CW_TRUE;
}

CWBool CWWTPGetAddedStaInfo(AddSTAValues *addStaValues)
{
	
	int i=0;
	int sta_count = 0;
	char ifname[IFNAMSIZ];
	CWWTPSta *ptr = NULL;
	CWWTPSta *oldptr = NULL;
	StationInfo *staInfoValues;
	

	/* get station count */
	sta_count = wtp_sta_count;
	sta_count = 1;
	CW_CREATE_ARRAY_ERR(staInfoValues, sta_count, StationInfo, CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	/* get station count */
	if((sta_count != 0)&&(wtp_sta_list != NULL))
	{
		/* get station radio_id,wlan_id,mac */
		i = 0;
		for(ptr = wtp_sta_list;ptr != NULL; oldptr = ptr ,ptr = ptr->next){
			if(macAddrCmp(ptr->sta_mac, addStaValues->mac_addr))
			{
				memset(staInfoValues, 0, sizeof(StationInfo));
				(*staInfoValues).radioId = ptr->radio_id;
				(*staInfoValues).wlanId = ptr->wlan_id;
				memcpy((*staInfoValues).mac, ptr->sta_mac, 6);
				break;
			}
			i++;
		}
		CWWTPGetWlanName(ifname, (*staInfoValues).radioId, (*staInfoValues).wlanId);
		/* get station info */
		get_stations(ifname, sta_count, staInfoValues);
		//get_sta_stats(staInfoValues,sta_count);
	
		/* assemble WTP Event request element with info of AP StationInfo */
		CWProtocolMessage *messages = NULL;
		int fragmentsNum=0;
		int seqNum;
		seqNum = CWGetSeqNum();

		if(!CWAssembleWTPEventStaInfoRequest(&messages, &fragmentsNum, gWTPPathMTU, seqNum, sta_count, staInfoValues))  /*pei test for neighbor AP info list. 1125*/
		{
			CWDebugLog("Assemble WTP Event Ap Sta Info Request error!\n");
			CWWTPDebugLog("Assemble WTP Event Ap Sta Info Request error!");
			CW_FREE_OBJECT(staInfoValues);
		}
		CW_FREE_OBJECT(staInfoValues);
		
		/* send StationInfo info */
		for(i = 0; i < fragmentsNum; i++) 
		{

#ifndef CW_NO_DTLS
			if(gDtlsSecurity == 1)
			{
				if(!CWSecuritySend(gWTPSession, messages[i].msg, messages[i].offset))
				{
					CWDebugLog("Error sending message\n");
					CWFreeMessageFragments(messages, fragmentsNum);
					CW_FREE_OBJECT(messages);
					CWWTPDebugLog("Error sending msg of sta info");
				}
			}
			else
#endif
			{
				if (!CWNetworkSendUnsafeUnconnected(gWTPSocket, &(gACInfoPtr->preferredAddress), messages[i].msg, messages[i].offset))
				{
					CWDebugLog("Error sending message\n");
					CWFreeMessageFragments(messages, fragmentsNum);
					CW_FREE_OBJECT(messages);
					CWWTPDebugLog("Error sending msg of sta info");
				}
			}
		}
		CWDebugLog("Message Sent\n");
		CWFreeMessageFragments(messages, fragmentsNum);
		CW_FREE_OBJECT(messages);
	}
	
	return CW_FALSE;
}

CW_THREAD_RETURN_TYPE CWWTPGetStaInfo(void *arg)
{
	if(pthread_detach(pthread_self())!=0)
	printf("##########detach error!############\n");
	unsigned short reportintval;
	
	CW_REPEAT_FOREVER 
	{
		int i=0;
		int sta_count = 0;
		char ifname[IFNAMSIZ];
		CWWTPSta *ptr = NULL;
		CWWTPSta *oldptr = NULL;
		StationInfo *staInfoValues;
		
		reportintval = gStaInfoInterval;

		sleep(reportintval);
		
		/* if disable the AP StationInfo, exit the thread. */
		if(!gStaInfoEnable)
		{
			CWWTPDebugLog("disable sta info, exit thread");
			goto exit_thread;
		}

		/* get station count */
		CWThreadMutexLock(&sta_table_mutex);
		sta_count = wtp_sta_count;
		dpf("There are %d stations!\n", sta_count);
		if((sta_count == 0)||(wtp_sta_list == NULL))
			CWThreadMutexUnlock(&sta_table_mutex);
		else{
			CW_CREATE_ARRAY_ERR(staInfoValues, sta_count, StationInfo, CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
			/* get station radio_id,wlan_id,mac */
			i = 0;
			for(ptr = wtp_sta_list;ptr != NULL; oldptr = ptr ,ptr = ptr->next){
				memset(&staInfoValues[i], 0, sizeof(StationInfo));
				staInfoValues[i].radioId = ptr->radio_id;
				staInfoValues[i].wlanId = ptr->wlan_id;
				memcpy(&(staInfoValues[i].mac), &(ptr->sta_mac), 6);
				i++;
			}
			CWThreadMutexUnlock(&sta_table_mutex);
			
			/* get station info */
			CWWTPWlan *ptr_wlan = NULL;
			CWWTPWlan *oldptr_wlan = NULL;
			for(ptr_wlan=wtp_wlan_list;ptr_wlan != NULL;oldptr_wlan = ptr_wlan,ptr_wlan = ptr_wlan->next)
			{
				CWWTPGetWlanName(ifname, ptr_wlan->radio_id, ptr_wlan->wlan_id);
				get_stations(ifname, sta_count, staInfoValues);
			}
			get_sta_stats(staInfoValues,sta_count);
		
			/* assemble WTP Event request element with info of AP StationInfo */
			CWProtocolMessage *messages = NULL;
			int fragmentsNum=0;
			int seqNum;
			seqNum = CWGetSeqNum();

			if(!CWAssembleWTPEventStaInfoRequest(&messages, &fragmentsNum, gWTPPathMTU, seqNum, sta_count, staInfoValues))  /*pei test for neighbor AP info list. 1125*/
			{
				CWDebugLog("Assemble WTP Event Ap Sta Info Request error!\n");
				CWWTPDebugLog("Assemble WTP Event Ap Sta Info Request error!");
				CW_FREE_OBJECT(staInfoValues);
				goto exit_thread;
			}
			CW_FREE_OBJECT(staInfoValues);
			
			/* send StationInfo info */
			for(i = 0; i < fragmentsNum; i++) 
			{
//				if(debug_print)
//				{
//					printf("messages[%d].offset:%d\n", i, messages[i].offset);
//					CWCaptrue(messages[i].offset, (unsigned char *)(messages[i].msg));
//				}

#ifndef CW_NO_DTLS
				if(gDtlsSecurity == 1)
				{
					if(!CWSecuritySend(gWTPSession, messages[i].msg, messages[i].offset))
					{
						CWDebugLog("Error sending message\n");
						CWFreeMessageFragments(messages, fragmentsNum);
						CW_FREE_OBJECT(messages);
						CWWTPDebugLog("Error sending msg of sta info");
						goto exit_thread;
					}
				}
				else
#endif
				{
					if (!CWNetworkSendUnsafeUnconnected(gWTPSocket, &(gACInfoPtr->preferredAddress), messages[i].msg, messages[i].offset))
					{
						CWDebugLog("Error sending message\n");
						CWFreeMessageFragments(messages, fragmentsNum);
						CW_FREE_OBJECT(messages);
						CWWTPDebugLog("Error sending msg of sta info");
						goto exit_thread;
					}
				}
			}
			CWDebugLog("Message Sent\n");
			CWFreeMessageFragments(messages, fragmentsNum);
			CW_FREE_OBJECT(messages);
		}
		
		/* if disable the AP StationInfo, exit the thread. */
		if(!gStaInfoEnable)
		{
			CWWTPDebugLog("disable sta info, exit thread");
			goto exit_thread;
		}
		
	}

exit_thread:
	CWWTPDebugLog("Exit the thread of sta info!");
	CWExitThread();
	return NULL;
}

CWBool CWGetStaInfo(GetStaInfoValues *getStaInfoValues)
{
	CWWTPDebugLog("got msg to get StationInfo, begin to set...");
	if(getStaInfoValues->value!=StaInfo)
		return CW_FALSE;

	if(getStaInfoValues->opstate == 0)
	{
		CWWTPDebugLog("set AP StationInfo disable");
		if(gStaInfoEnable == 1)
		{
			if( (thread_staInfo != 0) && (0 == pthread_kill(thread_staInfo, 0) ) ) {
				pthread_cancel(thread_staInfo);
			}
			gStaInfoEnable = 0;
			CWWTPDebugLog("set OK!");
		}
	}
	else if(getStaInfoValues->opstate == 1)
	{
		CWWTPDebugLog("set AP StationInfo enable");
		if(gStaInfoEnable)
		{
			if(debug_print)
			{
				if(gStaInfoInterval!= getStaInfoValues->reportinterval)
					printf("set AP StationInfo interval changed %d to %d\n", gStaInfoInterval, getStaInfoValues->reportinterval);
			}
			CWWTPDebugLog("AP StationInfo is already enable, interval %d to %d", gStaInfoInterval, getStaInfoValues->reportinterval);
			gStaInfoInterval = getStaInfoValues->reportinterval;
		}
		else
		{
			gStaInfoInterval = getStaInfoValues->reportinterval;
			gStaInfoEnable = 1;
			if( (thread_staInfo != 0) && (0 == pthread_kill(thread_staInfo, 0) ) ) {
				pthread_cancel(thread_staInfo);
//				pthread_join(thread_staInfo,NULL);
			}			
			if(!CWErr(CWCreateThread(&thread_staInfo, CWWTPGetStaInfo, NULL,0))) {
				CWDebugLog("Error starting Thread to get StationInfo");
				return CW_FALSE;
			}
		}
		
	}
	
	ConfArg confArg;
	memset(&confArg,0,sizeof(confArg));
	strcpy(confArg.conf_arg,"gStaInfoEnable");
	sprintf(confArg.conf_value,"%d",gStaInfoEnable);
	CWSaveConfigInfo(CONF_WTPD,confArg,CW_TRUE);
	
	memset(&confArg,0,sizeof(confArg));
	strcpy(confArg.conf_arg,"gStaInfoInterval");
	sprintf(confArg.conf_value,"%hd",gStaInfoInterval);
	CWSaveConfigInfo(CONF_WTPD,confArg,CW_FALSE);
	return CW_TRUE;
}

CW_THREAD_RETURN_TYPE CWWTPGetIfState(void *arg)
{
	if(pthread_detach(pthread_self())!=0)
	printf("##########detach error!############\n");
	unsigned short reportintval;
	
//	reportintval = (unsigned short)arg;
	
	CW_REPEAT_FOREVER 
	{
		int i=0;
		int eth_count = 0;
		int wifi_count = 0;
		char ifname[IFNAMSIZ];
		ifState ifStateValues;
		
		/* if disable the AP InterfaceState, exit the thread. */
		if(!gIfStateEnable)
		{
			CWWTPDebugLog("disable if state, exit thread");
			goto exit_thread;
		}
		reportintval = gIfStateInterval;
		dpf("reportintval:%d\n", reportintval);

		sleep(reportintval);
		
		/* get if state */
		memset(&ifStateValues, 0, sizeof(ifState));
		CWGetInterfaceCount(&eth_count, "eth[0-9]");
		//CWGetInterfaceCount(&wifi_count, "wifi[0-9]");
		wifi_count = CWGetRadioMaxCount();
		if(wifi_count == 0)
			wifi_count = 1;
		
		ifStateValues.eth_count = eth_count;
		ifStateValues.wifi_count = wifi_count;
		CW_CREATE_ARRAY_ERR(ifStateValues.eth_status, ifStateValues.eth_count, interface_status, CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
		CW_CREATE_ARRAY_ERR(ifStateValues.wifi_status, ifStateValues.wifi_count, interface_status, CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
		for(i=0;i<eth_count;i++)
		{
			sprintf(ifname, "eth%d", i);
			ifStateValues.eth_status[i].id = i;
			CWGetInterfaceStatus(ifname, &(ifStateValues.eth_status[i].status));
		}
		for(i=0;i<wifi_count;i++)
		{
			sprintf(ifname, "wifi%d", i);
			ifStateValues.wifi_status[i].id = i;
			CWGetInterfaceStatus(ifname, &(ifStateValues.wifi_status[i].status));
		}

		/* assemble WTP Event request element with info of AP InterfaceState */
		CWProtocolMessage *messages = NULL;
		int fragmentsNum=0;
		int seqNum;
		seqNum = CWGetSeqNum();

		if(!CWAssembleWTPEventIfStateRequest(&messages, &fragmentsNum, gWTPPathMTU, seqNum, &ifStateValues))  /*pei test for neighbor AP info list. 1125*/
		{
			CW_FREE_OBJECT(ifStateValues.eth_status);
			CW_FREE_OBJECT(ifStateValues.wifi_status);
			CWDebugLog("Assemble WTP Event Ap InterfaceState Request error!\n");
			CWWTPDebugLog("Assemble WTP Event Ap InterfaceState Request error!");
			goto exit_thread;
		}
		CW_FREE_OBJECT(ifStateValues.eth_status);
		CW_FREE_OBJECT(ifStateValues.wifi_status);

		/* send InterfaceState info */
		for(i = 0; i < fragmentsNum; i++) 
		{
//			if(debug_print)
//			{
//				printf("messages[%d].offset:%d\n", i, messages[i].offset);
//				CWCaptrue(messages[i].offset, (unsigned char *)(messages[i].msg));
//			}

#ifndef CW_NO_DTLS
			if(gDtlsSecurity == 1)
			{
				if(!CWSecuritySend(gWTPSession, messages[i].msg, messages[i].offset))
				{
					CWDebugLog("Error sending message\n");
					CWFreeMessageFragments(messages, fragmentsNum);
					CW_FREE_OBJECT(messages);
					CWWTPDebugLog("Error sending msg of if state");
					goto exit_thread;
				}
			}
			else
#endif
			{
				if (!CWNetworkSendUnsafeUnconnected(gWTPSocket, &(gACInfoPtr->preferredAddress), messages[i].msg, messages[i].offset))
				{
					CWDebugLog("Error sending message\n");
					CWFreeMessageFragments(messages, fragmentsNum);
					CW_FREE_OBJECT(messages);
					CWWTPDebugLog("Error sending msg of if state");
					goto exit_thread;
				}
			}
		}
		CWDebugLog("Message Sent\n");
		CWFreeMessageFragments(messages, fragmentsNum);
		CW_FREE_OBJECT(messages);

		/* if disable the AP interface state, exit the thread. */
		if(!gIfStateEnable)
		{
			CWWTPDebugLog("disable if state, exit thread");
			goto exit_thread;
		}
		
	}

exit_thread:
	CWWTPDebugLog("Exit the thread of if state!");
	CWExitThread();
	return NULL;
}
CWBool CWGetIfState(GetIfStateValues *getIfStateValues)
{
	CWWTPDebugLog("got msg to get InterfaceState, begin to set...");
	if(getIfStateValues->value!=InterfaceState)
		return CW_FALSE;

	if(getIfStateValues->opstate == 0)
	{
		CWWTPDebugLog("set AP InterfaceState disable");
		if(gIfStateEnable == 1)
		{
			if( (thread_ifState != 0) && (0 == pthread_kill(thread_ifState, 0) ) ) {
				pthread_cancel(thread_ifState);
			}
			gIfStateEnable = 0;
			CWWTPDebugLog("set OK!");
		}
	}
	else if(getIfStateValues->opstate == 1)
	{
		CWWTPDebugLog("set AP InterfaceState enable");
		if(gIfStateEnable)
		{
			if(debug_print)
			{
				if(gIfStateInterval!= getIfStateValues->reportinterval)
					printf("set AP InterfaceState interval changed %d to %d\n", gIfStateInterval, getIfStateValues->reportinterval);
			}
			CWWTPDebugLog("AP StationInfo is already enable, interval %d to %d", gIfStateInterval, getIfStateValues->reportinterval);
			gIfStateInterval = getIfStateValues->reportinterval;
		}
		else
		{
			gIfStateInterval = getIfStateValues->reportinterval;
			gIfStateEnable = 1;
			if( (thread_ifState != 0) && (0 == pthread_kill(thread_ifState, 0) ) ) {
				pthread_cancel(thread_ifState);
//				pthread_join(thread_ifState,NULL);
			}
			if(!CWErr(CWCreateThread(&thread_ifState, CWWTPGetIfState, NULL,0))) {
				CWDebugLog("Error starting Thread to get InterfaceState");
				return CW_FALSE;
			}
		}
		
	}
	
	ConfArg confArg;
	memset(&confArg,0,sizeof(confArg));
	strcpy(confArg.conf_arg,"gIfStateEnable");
	sprintf(confArg.conf_value,"%d",gIfStateEnable);
	CWSaveConfigInfo(CONF_WTPD,confArg,CW_TRUE);
	
	memset(&confArg,0,sizeof(confArg));
	strcpy(confArg.conf_arg,"gIfStateInterval");
	sprintf(confArg.conf_value,"%hd",gIfStateInterval);
	CWSaveConfigInfo(CONF_WTPD,confArg,CW_FALSE);
	return CW_TRUE;
}
CWBool CWGetStaPktsStatisticRpt(GetStaPktsStatisticRptValues *getStaPktsStatisticRptValues)
{
	GetStaPktsStatisticRptValues *g = getStaPktsStatisticRptValues;
	CWWTPDebugLog("got msg to set StaPktsStatisticRpt, begin to set...");
	if(debug_print)
		printf("got msg to set StaPktsStatisticRpt, begin to set...\n");
	if(g->elem_type!=VSP_T_STA_PACKET_STATISTIC_REPORT)
		return CW_FALSE;

	if(g->opstate == 0)
	{
		CWWTPDebugLog("set AP StaPktsStatisticRpt disable");
		if(debug_print)
			printf("set AP StaPktsStatisticRpt disable\n");
		if(gStaPktsStatisticEnable == 1)
		{
			if( (thread_sta_pkts_statistic_report != 0) && (0 == pthread_kill(thread_sta_pkts_statistic_report, 0) ) ) {
				pthread_cancel(thread_sta_pkts_statistic_report);
			}
			gStaPktsStatisticEnable = 0;
			CWWTPDebugLog("set OK!");
			if(debug_print)
				printf("set OK!\n");
		}
	}
	else if(g->opstate == 1)
	{
		CWWTPDebugLog("set AP StaPktsStatisticRpt enable");
		if(debug_print)
			printf("set AP StaPktsStatisticRpt enable\n");
		if(gStaPktsStatisticEnable)
		{
			if(debug_print)
			{
				printf("AP StaPktsStatisticRpt is already enable\n");
				if(gStaPktsStatisticInterval!= g->reportinterval)
					printf("set AP StaPktsStatisticRpt interval changed %d to %d\n", gStaPktsStatisticInterval, g->reportinterval);
			}
			CWWTPDebugLog("AP StaPktsStatisticRpt is already enable, interval %d to %d", gStaPktsStatisticInterval, g->reportinterval);
			gStaPktsStatisticInterval = g->reportinterval;
		}
		else
		{
			gStaPktsStatisticInterval = g->reportinterval;
			gStaPktsStatisticEnable = 1;
			if( (thread_sta_pkts_statistic_report != 0) && (0 == pthread_kill(thread_sta_pkts_statistic_report, 0) ) ) {
				pthread_cancel(thread_sta_pkts_statistic_report);
			}
			if(!CWErr(CWCreateThread(&thread_sta_pkts_statistic_report, CWWTPStaPktsStatisticReport, NULL,0))) {
				CWDebugLog("Error starting Thread to get InterfaceState");
				return CW_FALSE;
			}
		}
		
	}
	
	ConfArg confArg;
	memset(&confArg,0,sizeof(confArg));
	strcpy(confArg.conf_arg,"gStaPktsStatisticEnable");
	sprintf(confArg.conf_value,"%d",gStaPktsStatisticEnable);
	CWSaveConfigInfo(CONF_WTPD,confArg,CW_TRUE);
	
	memset(&confArg,0,sizeof(confArg));
	strcpy(confArg.conf_arg,"gStaPktsStatisticInterval");
	sprintf(confArg.conf_value,"%hd",gStaPktsStatisticInterval);
	CWSaveConfigInfo(CONF_WTPD,confArg,CW_FALSE);
	return CW_TRUE;
}

int getWapiStaCount(void)
{
	CWWTPWlan *ptr_wlan = NULL;
	CWWTPWlan *oldptr_wlan = NULL;
	CWWTPSta *ptr = NULL;
	CWWTPSta *oldptr = NULL;
	int staCount = 0;

	for(ptr_wlan=wtp_wlan_list;ptr_wlan != NULL;oldptr_wlan = ptr_wlan,ptr_wlan = ptr_wlan->next)
	{
		if((ptr_wlan->wlan_key_mgmt == WTP_WAPI_KEY_MGMT_CER)||(ptr_wlan->wlan_key_mgmt == WTP_WAPI_KEY_MGMT_PSK))
//		if(ptr_wlan->wlan_key_mgmt == WTP_WPA_KEY_MGMT_NONE)  /*test*/
		{
			for(ptr = wtp_sta_list;ptr != NULL; oldptr = ptr ,ptr = ptr->next){
				if((ptr->radio_id == ptr_wlan->radio_id)&&(ptr->wlan_id == ptr_wlan->wlan_id))
					staCount++;
			}
		}
	}
	dpf("sta count :%d\n", staCount);
	return staCount;
}
void getWapiStaMac(StationWapiInfo *StaWapiInfo)
{
	CWWTPWlan *ptr_wlan = NULL;
	CWWTPWlan *oldptr_wlan = NULL;
	CWWTPSta *ptr = NULL;
	CWWTPSta *oldptr = NULL;
	int i = 0;

	for(ptr_wlan=wtp_wlan_list;ptr_wlan != NULL;oldptr_wlan = ptr_wlan,ptr_wlan = ptr_wlan->next)
	{
		if((ptr_wlan->wlan_key_mgmt == WTP_WAPI_KEY_MGMT_CER)||(ptr_wlan->wlan_key_mgmt == WTP_WAPI_KEY_MGMT_PSK))
//		if(ptr_wlan->wlan_key_mgmt == WTP_WPA_KEY_MGMT_NONE)  /*test*/
		{
			for(ptr = wtp_sta_list;ptr != NULL; oldptr = ptr ,ptr = ptr->next){
				if((ptr->radio_id == ptr_wlan->radio_id)&&(ptr->wlan_id == ptr_wlan->wlan_id))
				{
					StaWapiInfo[i].radioId = ptr->radio_id;
					StaWapiInfo[i].wlanId = ptr->wlan_id;
					memcpy(&(StaWapiInfo[i].mac), &(ptr->sta_mac), 6);
					i++;
				}
			}
		}
	}
	
	return;
}
CW_THREAD_RETURN_TYPE CWWTPGetStaWapiInfo(void *arg)
{
	if(pthread_detach(pthread_self())!=0)
	printf("##########detach error!############\n");
	unsigned short reportintval;
		
	CW_REPEAT_FOREVER 
	{
		int i = 0;
		int sta_count = 0;
		char ifname[IFNAMSIZ];
		CWWTPSta *ptr = NULL;
		CWWTPSta *oldptr = NULL;
		wpi_errors wpi_error_values;
		
		/* if disable the AP StaWapiInfo, exit the thread. */
		if(!gStaWapiInfoEnable)
		{
			printf("exit_thread\n");
			goto exit_thread;
		}
		reportintval = gStaWapiInfoInterval;
		dpf("reportintval:%d\n", reportintval);

		sleep(reportintval);
		
		/* get wapi station count */
		sta_count = getWapiStaCount();

		if(sta_count == 0)
		{
			dpf("wapi station count is :0, continue\n");
			continue;
		}
		
		/* get station wapi info */
		//StationWapiInfo staWapiInfo[sta_count];
		//memset(staWapiInfo, 0, sizeof(StationWapiInfo)*sta_count);
		StationWapiInfo *staWapiInfo;
		CW_CREATE_ARRAY_ERR(staWapiInfo, sta_count, StationWapiInfo, CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
		for(i=0;i<sta_count;i++)
			memset(&staWapiInfo[i], 0, sizeof(StationWapiInfo));
		
		getWapiStaMac(staWapiInfo);
		getStaWapiInfos(sta_count, staWapiInfo);
		for(ptr = wtp_sta_list;ptr != NULL; oldptr = ptr ,ptr = ptr->next){
			for(i=0;i<sta_count;i++)
			{
				if(1 == macAddrCmp(staWapiInfo[i].mac, ptr->sta_mac))
				{
					memset(ifname, 0, IFNAMSIZ);
					CWWTPGetWlanName(ifname, ptr->radio_id, ptr->wlan_id);
					CWGetWpiErrors(ifname, &wpi_error_values);
					staWapiInfo[i].wpiReplayCounters = wpi_error_values.wpi_replay_error;
					staWapiInfo[i].wpiDecryptableErrors = wpi_error_values.wpi_decryp_error;
					staWapiInfo[i].wpiMICErrors = wpi_error_values.wpi_mic_error;
				}
			}
		}
		
		/* assemble WTP Event request element with info of AP StaWapiInfo */
		CWProtocolMessage *messages = NULL;
		int fragmentsNum=0;
		int seqNum;
		seqNum = CWGetSeqNum();

		if(!CWAssembleWTPEventStaWapiInfoRequest(&messages, &fragmentsNum, gWTPPathMTU, seqNum, sta_count, staWapiInfo))
		{
			CWDebugLog("Assemble WTP Event Ap StaWapiInfo Request error!\n");
			printf("--Assemble WTP Event Ap StaWapiInfo Request error!-goto exit_thread\n");
			CW_FREE_OBJECT(staWapiInfo);
			goto exit_thread;
		}
		
		/* send StaWapiInfo info */
		for(i = 0; i < fragmentsNum; i++) 
		{
//			if(debug_print)
//			{
//				printf("messages[%d].offset:%d\n", i, messages[i].offset);
//				CWCaptrue(messages[i].offset, (unsigned char *)(messages[i].msg));
//			}

#ifndef CW_NO_DTLS
			if(gDtlsSecurity == 1)
			{
				if(!CWSecuritySend(gWTPSession, messages[i].msg, messages[i].offset))
				{
					CWDebugLog("Error sending message\n");
					printf("Error sending message\n");
					sleep(reportintval);
					break;//gzm change countinue to break 091029
				}
			}
			else
#endif
			{
				if (!CWNetworkSendUnsafeUnconnected(gWTPSocket, &(gACInfoPtr->preferredAddress), messages[i].msg, messages[i].offset))
				{
					CWDebugLog("Error sending message\n");
					printf("Error sending message\n");
					sleep(reportintval);
					break;//gzm change countinue to break 091029
				}
			}
		}
		CWDebugLog("Message Sent\n");
		CW_FREE_OBJECT(staWapiInfo);
		CWFreeMessageFragments(messages, fragmentsNum);
		CW_FREE_OBJECT(messages);

		/* if disable the AP StaWapiInfo, exit the thread. */
		if(!gStaWapiInfoEnable)
		{
			printf("exit_thread\n");
			goto exit_thread;
		}
	}

exit_thread:
	dpf("Exit the thread!\n");
	CWExitThread();
	return NULL;
}
CWBool CWGetStaWapiInfo(GetStaWapiInfoValues *getStaWapiInfoValues)
{
	dpf("got msg to get StaWapiInfo, begin to set...\n");
	if(getStaWapiInfoValues->value!=StaWapiInfo)
		return CW_FALSE;

	if(getStaWapiInfoValues->opstate == 0)
	{
		dpf("set AP StaWapiInfo disable\n");
		if(gStaWapiInfoEnable == 1)
		{
			gStaWapiInfoEnable = 0;
			dpf("set OK!\n");
		}
	}
	else if(getStaWapiInfoValues->opstate == 1)
	{
		dpf("set AP StaWapiInfo enable\n");
		if(gStaWapiInfoEnable)
		{
			if(debug_print)
			{
				printf("AP StaWapiInfo is already enable\n");
				if(gStaWapiInfoInterval!= getStaWapiInfoValues->reportinterval)
					printf("set AP StaWapiInfo interval changed %d to %d\n", gStaWapiInfoInterval, getStaWapiInfoValues->reportinterval);
			}
			gStaWapiInfoInterval = getStaWapiInfoValues->reportinterval;
		}
		else
		{
			gStaWapiInfoInterval = getStaWapiInfoValues->reportinterval;
			gStaWapiInfoEnable = 1;
			CWThread thread_ifState;
			if(!CWErr(CWCreateThread(&thread_ifState, CWWTPGetStaWapiInfo, NULL,0))) {
				CWDebugLog("Error starting Thread to get StaWapiInfo");
				return CW_FALSE;
			}
		}
		
	}
	
	return CW_TRUE;
}
#endif

#if 0
CWBool CWSetApThroughputInfoCollect(ApThroughputInfoValues *apThroughputInfoCollectValues)
{
	if(apThroughputInfoCollectValues->opstate == 1)
	{
		gAPThroughputInfoCollectEnable = 1;
		CWWTPDebugLog("Set AP Throughput Info Collect Enable!");
		dpf("Set AP Throughput Info Collect Enable!\n");
	}
	else if(apThroughputInfoCollectValues->opstate == 0)
	{
		gAPThroughputInfoCollectEnable = 0;
		CWWTPDebugLog("Set AP Throughput Info Collect Disable!");
		dpf("Set AP Throughput Info Collect Disable!\n");
	}
	return CW_TRUE;
}
#endif
static inline int procnetdev_version(char *buf)
{
	if (strstr(buf, "compressed"))
		return 2;
	if (strstr(buf, "bytes"))
		return 1;
	return 0;
}

static char *get_name(char *name, char *p)
{
	/* Extract <name> from nul-terminated p where p matches
	   <name>: after leading whitespace.
	   If match is not made, set name empty and return unchanged p */
	int namestart=0, nameend=0;
	while (is_space(p[namestart]))
		namestart++;
	nameend=namestart;
	while (p[nameend] && p[nameend]!=':' && !is_space(p[nameend]))
		nameend++;
	if (p[nameend]==':') {
		if ((nameend-namestart)<IFNAMSIZ) {
			memcpy(name,&p[namestart],nameend-namestart);
			name[nameend-namestart]='\0';
			p=&p[nameend];
		} else {
			/* Interface name too large */
			name[0]='\0';
		}
	} else {
		/* trailing ':' not found - return empty */
		name[0]='\0';
	}
	return p + 1;
}


int get_interface_throughput_fields(char *wlanname, wlan_stats_info *wlanStatsInfo)
{
	FILE *fh;
	char buf[512];
	struct user_net_device_stats wlan_stats;
	int procnetdev_vsn;

	fh = fopen(_PATH_PROCNET_DEV, "r");
	if (!fh) {
		CWWTPDebugLog("warning: cannot open %s, limiting output", _PATH_PROCNET_DEV);
		return 1;
	}
	fgets(buf, sizeof buf, fh);	/* eat line */
	fgets(buf, sizeof buf, fh);

	procnetdev_vsn = procnetdev_version(buf);

	while (fgets(buf, sizeof buf, fh)) {
		char *s, name[128];

		s = get_name(name, buf);
		if(strstr(buf, wlanname))
		{
			if(strncmp(name, wlanname, strlen(name)) == 0)
			{
				get_dev_fields(s, &wlan_stats, procnetdev_vsn);
				wlanStatsInfo->rx_packets = wlan_stats.rx_packets;
				wlanStatsInfo->tx_packets = wlan_stats.tx_packets;
				wlanStatsInfo->rx_errors = wlan_stats.rx_errors;
				wlanStatsInfo->tx_errors = wlan_stats.tx_errors;
				wlanStatsInfo->rx_bytes = wlan_stats.rx_bytes/1024;
				wlanStatsInfo->tx_bytes = wlan_stats.tx_bytes/1024;
				wlanStatsInfo->rx_rate = 0;  //need to modify
				wlanStatsInfo->tx_rate = 0;  //need to modify
			}
		}
	}
	fclose(fh);
	return 0;
}

static int get_wlan_throughput_fields(char *wlanname, wlan_stats_info *wlanStatsInfo)
{
	FILE *fh;
	char buf[512];
	struct user_net_device_stats wlan_stats;
	int procnetdev_vsn;

	fh = fopen(_PATH_PROCNET_DEV, "r");
	if (!fh) {
		CWWTPDebugLog("warning: cannot open %s, limiting output", _PATH_PROCNET_DEV);
		return 1;
	}
	fgets(buf, sizeof(buf), fh);	/* eat line */
	fgets(buf, sizeof(buf), fh);

	procnetdev_vsn = procnetdev_version(buf);

	while (fgets(buf, sizeof(buf), fh)) {
		char *s, name[128];

		s = get_name(name, buf);
		if(strstr(buf, wlanname))
		{
			if(strncmp(name, wlanname, strlen(name)) == 0)
			{
				get_dev_fields(s, &wlan_stats, procnetdev_vsn);
				wlanStatsInfo->rx_packets = wlan_stats.rx_packets;
				wlanStatsInfo->tx_packets = wlan_stats.tx_packets;
				wlanStatsInfo->rx_errors = wlan_stats.rx_errors;
				wlanStatsInfo->tx_errors = wlan_stats.tx_errors;
				wlanStatsInfo->rx_bytes = wlan_stats.rx_bytes;
				wlanStatsInfo->tx_bytes = wlan_stats.tx_bytes;
				wlanStatsInfo->rx_dropped= wlan_stats.rx_dropped;
				wlanStatsInfo->tx_dropped= wlan_stats.tx_dropped;
				wlanStatsInfo->rx_rate = 0;  //need to modify
				wlanStatsInfo->tx_rate = 0;  //need to modify
				wlanStatsInfo->tx_multicast = wlan_stats.tx_multicast;
				wlanStatsInfo->rx_multicast = wlan_stats.rx_multicast;
				wlanStatsInfo->tx_broadcast = wlan_stats.tx_broadcast;
				wlanStatsInfo->rx_broadcast = wlan_stats.rx_broadcast;
				wlanStatsInfo->tx_unicast = wlan_stats.tx_unicast;
				wlanStatsInfo->rx_unicast = wlan_stats.rx_unicast;
				wlanStatsInfo->rx_multicast_bytes = wlan_stats.rx_multicast_bytes;
				wlanStatsInfo->tx_multicast_bytes = wlan_stats.tx_multicast_bytes;
				if(wlan_stats.rx_unicast_bytes != 0)
					wlanStatsInfo->rx_unicast_bytes = wlan_stats.rx_unicast_bytes/1024;
				if(wlan_stats.tx_unicast_bytes != 0)
					wlanStatsInfo->tx_unicast_bytes = wlan_stats.tx_unicast_bytes/1024;
				wlanStatsInfo->rx_retry_bytes = wlan_stats.rx_retry_bytes;
				wlanStatsInfo->tx_retry_bytes = wlan_stats.tx_retry_bytes;
				wlanStatsInfo->rx_retry= wlan_stats.rx_retry;
				wlanStatsInfo->tx_retry= wlan_stats.tx_retry;
				wlanStatsInfo->rx_sum_bytes = wlan_stats.rx_bytes;
				wlanStatsInfo->tx_sum_bytes = wlan_stats.tx_bytes;
				wlanStatsInfo->rx_unicast64_bytes = wlan_stats.rx_unicast_bytes;
				wlanStatsInfo->tx_unicast64_bytes = wlan_stats.tx_unicast_bytes;
				wlanStatsInfo->rx_errors_frames = wlan_stats.rx_error_frames;
			}
		}
	}
	fclose(fh);
	return 0;
}
static int get_eth_fields(char *ifname, eth_package_info *ethPackageInfo)
{
	FILE *fh;
	char buf[512];
	struct user_net_device_stats wlan_stats;
	int procnetdev_vsn;

	fh = fopen(_PATH_PROCNET_DEV, "r");
	if (!fh) {
		CWWTPDebugLog("warning: cannot open %s, limiting output", _PATH_PROCNET_DEV);
		return 1;
	}
	fgets(buf, sizeof buf, fh);	/* eat line */
	fgets(buf, sizeof buf, fh);

	procnetdev_vsn = procnetdev_version(buf);

	while (fgets(buf, sizeof buf, fh)) {
		char *s, name[128];

		s = get_name(name, buf);
		if(strstr(buf, ifname))
		{
			if(strncmp(name, ifname, strlen(name)) == 0)
			{
				get_dev_fields(s, &wlan_stats, procnetdev_vsn);
				ethPackageInfo->tx_unicast = wlan_stats.tx_unicast;
				ethPackageInfo->tx_broadcast = wlan_stats.tx_broadcast;
				ethPackageInfo->tx_multicast = wlan_stats.tx_multicast;
				ethPackageInfo->tx_drop = wlan_stats.tx_dropped;
				ethPackageInfo->rx_unicast = wlan_stats.rx_unicast;
				ethPackageInfo->rx_broadcast = wlan_stats.rx_broadcast;
				ethPackageInfo->rx_multicast = wlan_stats.rx_multicast;
				ethPackageInfo->rx_drop = wlan_stats.rx_dropped;
			}
		}
	}
	fclose(fh);
	return 0;
}

int getifstats(const char *ifname, unsigned long *iframes, unsigned long *oframes)
{
	FILE * fd = fopen("/proc/net/dev", "r");
	if (fd != NULL) {
		char line[256];
		while (fgets(line, sizeof(line), fd)) {
			char *cp, *tp;

			for (cp = line; is_space(*cp); cp++)
				;
			if (cp[0] != ifname[0])
				continue;
			for (tp = cp; *tp != ':' && *tp; tp++)
				;
			if (*tp == ':') {
				*tp++ = '\0';
				if (strcmp(cp, ifname) != 0)
					continue;
//				sscanf(tp, "%*llu %lu %*u %*u %*u %*u %*u %*u %*llu %lu",
//					iframes, oframes);
				fclose(fd);
				return 1;
			}
		}
		fclose(fd);
	}
	return 0;
}

CWBool get_if_dropped(char *iftype, unsigned int *dropped_recv, unsigned int *dropped_send)
{
	char temp_str[128];
	char str[32];
	
	memset(temp_str, 0, 128);
	sprintf(temp_str, "cat /tmp/pkt/drop_recv_%s 2>/dev/null", iftype);
	PopenFile(temp_str, str, sizeof(str));	
	*dropped_recv = atoll(str);
	memset(temp_str, 0, 128);
	sprintf(temp_str, "cat /tmp/pkt/drop_send_%s 2>/dev/null", iftype);
	PopenFile(temp_str, str, sizeof(str));	
	*dropped_send = atoll(str);
	dpf("iftype:%s  dropped_recv:%u   dropped_send:%u\n", iftype, *dropped_recv, *dropped_send);

	return CW_TRUE;
}
CWBool get_if_errors(char *iftype, unsigned int *errors_recv, unsigned int *errors_send, unsigned int *errors_frames_recv)
{
	char temp_str[128];
	char str[32];
	
	memset(temp_str, 0, 128);
	sprintf(temp_str, "cat /tmp/pkt/errors_recv_%s 2>/dev/null", iftype);
	PopenFile(temp_str, str, sizeof(str));	
	*errors_recv = atoll(str);
	memset(temp_str, 0, 128);
	sprintf(temp_str, "cat /tmp/pkt/errors_send_%s 2>/dev/null", iftype);
	PopenFile(temp_str, str,sizeof(str));
	*errors_send = atoll(str);
	memset(temp_str, 0, 128);
	sprintf(temp_str, "cat /tmp/pkt/errframes_recv_%s 2>/dev/null", iftype);
	PopenFile(temp_str, str, sizeof(str));	
	*errors_frames_recv = atoll(str);
	dpf("iftype:%s  errors_recv:%u   errors_send:%u  errframes_recv:%u\n", iftype, *errors_recv, *errors_send, *errors_frames_recv);

	return CW_TRUE;
}
CWBool get_if_baderror(char *iftype, unsigned int *badmic_recv, unsigned int *badcrypt_recv)
{
	char temp_str[128];
	char str[32];
	
	memset(temp_str, 0, 128);
	sprintf(temp_str, "cat /tmp/pkt/badmic_recv_%s 2>/dev/null", iftype);
	PopenFile(temp_str, str, sizeof(str));	
	*badmic_recv = atoll(str);
	memset(temp_str, 0, 128);
	sprintf(temp_str, "cat /tmp/pkt/badcrypt_recv_%s 2>/dev/null", iftype);
	PopenFile(temp_str, str, sizeof(str));	
	*badcrypt_recv = atoll(str);
	dpf("iftype:%s  badmic_recv:%u  badcrypt_recv:%u\n", iftype, *badmic_recv, *badcrypt_recv);

	return CW_TRUE;
}
CWBool CWGetAPThroughputInfo(int wlanCount, int ethCount, int wifiCount, wlan_stats_info *wlanStatsInfo)
{
	int i = 0,k = 0;
	char wlanname[IFNAMSIZ];
	CWWTPWlan *ptr = NULL;
	unsigned char bssid[MAC_ADDR_LEN] = {0};
	int sock;
	unsigned char isLocal = 0;
	struct ifreq ifr;
	if(wtp_wlan_list == NULL){
		CWWTPDebugLog("wtp_wlan_count is 0,there is no wlan");
	}
	else{
		k = 0;
		for(ptr = wtp_wlan_list;ptr != NULL;ptr = ptr->next)
		{
			//printf("k:%d\n", k);
			if(k == wlanCount)
				break;
			wlanStatsInfo[k].type = ATH;
			wlanStatsInfo[k].radioId = ptr->radio_id;
			wlanStatsInfo[k].wlanId = ptr->wlan_id;
			/* get wlan mac */
			memset(wlanname, 0, IFNAMSIZ);
			CWWTPGetWlanName(wlanname, ptr->radio_id,ptr->wlan_id);
			dpf("wlan name:%s\n", wlanname);

            /**** zhouke add ,for marvell ioctl ****/
#ifdef MARVELL_PLATFORM
            memset(&ifr, 0, sizeof(struct ifreq));
#endif
            /************ end ************/
            
			strncpy(ifr.ifr_name, wlanname, IFNAMSIZ);
			memset(bssid, 0, 6);
			memset(wlanStatsInfo[k].mac, 0, 6);
			sock = socket(AF_INET, SOCK_DGRAM, 0);
			if (sock < 0) {
				CWWTPDebugLog("Error Creating Socket for ioctl");
//				CWThreadMutexUnlock(&wlan_list_mutex);
//				return CW_FALSE;
			}
			if (ioctl(sock, SIOCGIFHWADDR, &ifr)==-1) {
				CWWTPDebugLog("get wlan mAC error");
				close(sock);
//				CWThreadMutexUnlock(&wlan_list_mutex);
//				return CW_FALSE;
			}
			else{
				for (i=0; i<MAC_ADDR_LEN; i++){
					bssid[i]=(unsigned char)ifr.ifr_hwaddr.sa_data[i];
					//printf("%02x ", bssid[i]);
					wlanStatsInfo[k].mac[i] = bssid[i];
				}
				close(sock);
				//printf("\n");
			}
			/* get wlan throughput */
			get_wlan_throughput_fields(wlanname, &wlanStatsInfo[k]);
			get_stations_ctrl(&wlanStatsInfo[k]);
			get_athinfo_from_80211stats(wlanname, &wlanStatsInfo[k]);
			wlanStatsInfo[k].rx_sum_bytes = wlanStatsInfo[k].rx_bytes + wlanStatsInfo[k].rx_mgmt_bytes;
			wlanStatsInfo[k].tx_sum_bytes = wlanStatsInfo[k].tx_bytes + wlanStatsInfo[k].tx_mgmt_bytes;
			/* get wlan rate */
			for(i=0;i<NUM_OF_ATH_ALL_WIFI;i++)
			{
				if((wlanStatsInfo[k].radioId == gwlanStatsInfo[i].radioId)&&(wlanStatsInfo[k].wlanId == gwlanStatsInfo[i].wlanId))
				{
					wlanStatsInfo[k].rx_rate = (wlanStatsInfo[k].rx_sum_bytes-gwlanStatsInfo[i].rx_sum_bytes)/gAPThroughputInfoInterval;
					wlanStatsInfo[k].tx_rate = (wlanStatsInfo[k].tx_sum_bytes-gwlanStatsInfo[i].tx_sum_bytes)/gAPThroughputInfoInterval;
				}
			}
			//printf("get_wlan_throughput_fields succeed\n");
			/* simulate ath/eth pkt send/receive */
			//get_if_errors("ath", &(wlanStatsInfo[k].rx_errors), &(wlanStatsInfo[k].tx_errors), &(wlanStatsInfo[k].rx_errors_frames));
			//get_if_dropped("ath", &(wlanStatsInfo[k].rx_dropped), &(wlanStatsInfo[k].tx_dropped));
			k++;
		}
		/* renew the gwlanStatsInfo */
		for(i=0;i<NUM_OF_ATH_ALL_WIFI;i++)
		{
			memset(&gwlanStatsInfo[i], 0, sizeof(wlan_stats_info));
		}
		for(i=0;i<wlanCount;i++)
		{
			memcpy(&gwlanStatsInfo[i], &wlanStatsInfo[i], sizeof(wlan_stats_info));
		}
	}
	/* get eth and wifi interface throughput info */
	for(i=0;i<ethCount;i++)
	{
		memset(wlanname, 0, IFNAMSIZ);
		sprintf(wlanname, "eth%d", i);
		wlanStatsInfo[wlanCount+i].type = ETH;
		wlanStatsInfo[wlanCount+i].radioId = 0;
		wlanStatsInfo[wlanCount+i].wlanId = i;
		if(ethCount == 2)
		{
#if (defined( AQ2000N )|| defined(XHPB44)||defined(XJAP93V1))
			if(i == 0)
				continue;
#else
			if(i == 1)
				continue;
#endif			
		}
		get_wlan_throughput_fields(wlanname, &wlanStatsInfo[wlanCount+i]);
		for(ptr = wtp_wlan_list;ptr != NULL;ptr = ptr->next)
		{
			if(ptr->wlan_tunnel_mode!=CW_802_DOT_11_TUNNEL)
			{
				isLocal = 1;
				break;
			}
		}
		dpf("isLocal:%d\n", isLocal);
		if(isLocal == 0)
		{
			unsigned long long bytes_recv = 0;
			unsigned long long bytes_send = 0;
			bytes_recv = CWGet_tunnel_bytes_recv();
			wlanStatsInfo[wlanCount+i].rx_bytes = bytes_recv;
			bytes_send = CWGet_tunnel_bytes_send();
			wlanStatsInfo[wlanCount+i].tx_bytes = bytes_send;
			
			if(debug_print)
			{
				printf("bytes_recv:%llu   bytes_send:%llu\n", bytes_recv, bytes_send);
				printf("recv:%llu   send:%llu\n", wlanStatsInfo[wlanCount+i].rx_bytes, wlanStatsInfo[wlanCount+i].tx_bytes);
			}			
		}
		/* simulate ath/eth pkt send/receive */
		//get_if_errors("eth", &(wlanStatsInfo[wlanCount+i].rx_errors), &(wlanStatsInfo[wlanCount+i].tx_errors), &(wlanStatsInfo[wlanCount+i].rx_errors_frames));
		//get_if_dropped("eth", &(wlanStatsInfo[wlanCount+i].rx_dropped), &(wlanStatsInfo[wlanCount+i].tx_dropped));
		//get_if_baderror("eth", &(wlanStatsInfo[wlanCount+i].ast_rx_badmic), &(wlanStatsInfo[wlanCount+i].ast_rx_badcrypt));
		/* get eth rate */
		for(k=0;k<NUM_OF_ETH;k++)
		{
			if((wlanStatsInfo[wlanCount+i].type == gethStatsInfo[k].type)&&(wlanStatsInfo[wlanCount+i].radioId == gethStatsInfo[k].radioId)&&(wlanStatsInfo[wlanCount+i].wlanId == gethStatsInfo[k].wlanId))
			{
				wlanStatsInfo[wlanCount+i].rx_rate = (wlanStatsInfo[wlanCount+i].rx_sum_bytes-gethStatsInfo[k].rx_sum_bytes)/gAPThroughputInfoInterval;
				wlanStatsInfo[wlanCount+i].tx_rate = (wlanStatsInfo[wlanCount+i].tx_sum_bytes-gethStatsInfo[k].tx_sum_bytes)/gAPThroughputInfoInterval;
			}
		}
	}
	/* renew the gethStatsInfo */
	for(i=0;i<NUM_OF_ETH;i++)
	{
		memset(&gethStatsInfo[i], 0, sizeof(wlan_stats_info));
	}
	for(i=0;i<ethCount;i++)
	{
		memcpy(&gethStatsInfo[i], &wlanStatsInfo[wlanCount+i], sizeof(wlan_stats_info));
	}
	for(i=0;i<wifiCount;i++)
	{
		memset(wlanname, 0, IFNAMSIZ);
		sprintf(wlanname, "wifi%d", i);
		wlanStatsInfo[wlanCount+ethCount+i].type = WIFI;
		wlanStatsInfo[wlanCount+ethCount+i].radioId = 0;
		wlanStatsInfo[wlanCount+ethCount+i].wlanId = i;
//		get_wlan_throughput_fields(wlanname, &wlanStatsInfo[wlanCount+ethCount+i]);
/*********** pei add for shanghai dianxin test at 090410 **********/
		//wlanStatsInfo[wlanCount+ethCount+i].rx_dropped = 0;
		//wlanStatsInfo[wlanCount+ethCount+i].tx_dropped = 0;
		//wlanStatsInfo[wlanCount+ethCount+i].rx_errors = 0;
		//wlanStatsInfo[wlanCount+ethCount+i].tx_errors = 0;
		//wlanStatsInfo[wlanCount+ethCount+i].rx_errors_frames = 0;
		int j=0;
		for(j=0;j<wlanCount;j++){
			if(wlanStatsInfo[wlanCount+ethCount+i].wlanId == wlanStatsInfo[j].radioId)
			{
				wlanStatsInfo[wlanCount+ethCount+i].rx_packets += wlanStatsInfo[j].rx_packets;
				wlanStatsInfo[wlanCount+ethCount+i].tx_packets += wlanStatsInfo[j].tx_packets;
				wlanStatsInfo[wlanCount+ethCount+i].rx_bytes += wlanStatsInfo[j].rx_bytes;
				wlanStatsInfo[wlanCount+ethCount+i].tx_bytes += wlanStatsInfo[j].tx_bytes;
				wlanStatsInfo[wlanCount+ethCount+i].rx_unicast += wlanStatsInfo[j].rx_unicast;
				wlanStatsInfo[wlanCount+ethCount+i].tx_unicast += wlanStatsInfo[j].tx_unicast;
				wlanStatsInfo[wlanCount+ethCount+i].rx_unicast64_bytes += wlanStatsInfo[j].rx_unicast64_bytes;
				wlanStatsInfo[wlanCount+ethCount+i].tx_unicast64_bytes += wlanStatsInfo[j].tx_unicast64_bytes;
				wlanStatsInfo[wlanCount+ethCount+i].rx_errors += wlanStatsInfo[j].rx_errors;
				wlanStatsInfo[wlanCount+ethCount+i].tx_errors += wlanStatsInfo[j].tx_errors;
				wlanStatsInfo[wlanCount+ethCount+i].rx_sum_bytes += wlanStatsInfo[j].rx_sum_bytes;
				wlanStatsInfo[wlanCount+ethCount+i].tx_sum_bytes += wlanStatsInfo[j].tx_sum_bytes;

				wlanStatsInfo[wlanCount+ethCount+i].rx_rate+= wlanStatsInfo[j].rx_rate;
				wlanStatsInfo[wlanCount+ethCount+i].tx_rate+= wlanStatsInfo[j].tx_rate;
				wlanStatsInfo[wlanCount+ethCount+i].rx_multicast += wlanStatsInfo[j].rx_multicast;
				wlanStatsInfo[wlanCount+ethCount+i].tx_multicast += wlanStatsInfo[j].tx_multicast;
				wlanStatsInfo[wlanCount+ethCount+i].rx_multicast_bytes+= wlanStatsInfo[j].rx_multicast_bytes;
				wlanStatsInfo[wlanCount+ethCount+i].tx_multicast_bytes+= wlanStatsInfo[j].tx_multicast_bytes;
				wlanStatsInfo[wlanCount+ethCount+i].rx_broadcast += wlanStatsInfo[j].rx_broadcast;
				wlanStatsInfo[wlanCount+ethCount+i].tx_broadcast += wlanStatsInfo[j].tx_broadcast;
				wlanStatsInfo[wlanCount+ethCount+i].rx_broadcast_bytes+= wlanStatsInfo[j].rx_broadcast_bytes;
				wlanStatsInfo[wlanCount+ethCount+i].tx_broadcast_bytes+= wlanStatsInfo[j].tx_broadcast_bytes;
				wlanStatsInfo[wlanCount+ethCount+i].rx_retry += wlanStatsInfo[j].rx_retry;
				wlanStatsInfo[wlanCount+ethCount+i].tx_retry += wlanStatsInfo[j].tx_retry;
				wlanStatsInfo[wlanCount+ethCount+i].rx_retry_bytes+= wlanStatsInfo[j].rx_retry_bytes;
				wlanStatsInfo[wlanCount+ethCount+i].tx_retry_bytes+= wlanStatsInfo[j].tx_retry_bytes;
				wlanStatsInfo[wlanCount+ethCount+i].rx_dropped += wlanStatsInfo[j].rx_dropped;
				wlanStatsInfo[wlanCount+ethCount+i].tx_dropped += wlanStatsInfo[j].tx_dropped;
				wlanStatsInfo[wlanCount+ethCount+i].rx_mgmt += wlanStatsInfo[j].rx_mgmt;
				wlanStatsInfo[wlanCount+ethCount+i].tx_mgmt += wlanStatsInfo[j].tx_mgmt;
				wlanStatsInfo[wlanCount+ethCount+i].rx_mgmt_bytes += wlanStatsInfo[j].rx_mgmt_bytes;
				wlanStatsInfo[wlanCount+ethCount+i].tx_mgmt_bytes += wlanStatsInfo[j].tx_mgmt_bytes;
				wlanStatsInfo[wlanCount+ethCount+i].rx_ctrl+= wlanStatsInfo[j].rx_ctrl;
				wlanStatsInfo[wlanCount+ethCount+i].tx_ctrl+= wlanStatsInfo[j].tx_ctrl;
				wlanStatsInfo[wlanCount+ethCount+i].rx_errors_frames+= wlanStatsInfo[j].rx_errors_frames;
			}
		}
/*********************************************************/

		get_wifi_info(wlanname, &wlanStatsInfo[wlanCount+ethCount+i]);
	}
	/*yuanjh modified for get rx_crcerr,rx_badcrypt,rx_badmic,rx_phyerr for the first ath of the wifi.*/
	for(i=0;i<wifiCount;i++)
	{
		int j=0;
		for(j=0;j<wlanCount;j++)
		{
			if(wlanStatsInfo[j].radioId== i)
			{
				wlanStatsInfo[j].ast_rx_crcerr = wlanStatsInfo[wlanCount+ethCount+i].ast_rx_crcerr;
				wlanStatsInfo[j].ast_rx_badcrypt = wlanStatsInfo[wlanCount+ethCount+i].ast_rx_badcrypt;
				wlanStatsInfo[j].ast_rx_badmic = wlanStatsInfo[wlanCount+ethCount+i].ast_rx_badmic;
				wlanStatsInfo[j].ast_rx_phyerr = wlanStatsInfo[wlanCount+ethCount+i].ast_rx_phyerr;
				break;
			}
		}
	}
	/*yuanjh add end*/

	return CW_TRUE;
}
CWBool CWSetApMaxThroughput(ApMaxThroughputValues *apMaxThroughput,unsigned char radioId)
{
	unsigned char wlanId;
	int i = 0;
	char wlanname[IFNAMSIZ];
#if 0
	char cmdbuf[100];
	CWWTPWlan *ptr = NULL;

	if((wtp_wlan_list!=NULL)&&(apMaxThroughput!=NULL))
	{
		for(ptr = wtp_wlan_list;ptr != NULL;ptr = ptr->next)
		{
			i = 0;
			if(apMaxThroughput->wlanMaxThroughput[i].wlanId == ptr->wlan_id)
			{
				/* get wlan mac */
				CWWTPGetWlanName(wlanname, ptr->wlan_id);
				sprintf(cmdbuf,"autelan traffic_limit %s set_vap_flag 1", wlanname);
				system(cmdbuf);
				sprintf(cmdbuf,"autelan traffic_limit %s set_vap %d", wlanname, (apMaxThroughput->wlanMaxThroughput[i].maxThroughput)*1024);
				system(cmdbuf);
				gMaxThroughput = apMaxThroughput->apMaxThroughput;
			}
			i++;
		}
	}
#else
	gMaxThroughput = apMaxThroughput->apMaxThroughput;
	for(i=0;i<16;i++)
	{
		memset(&gWlanMaxThroughputValue[i], 0, sizeof(WlanMaxThroughputValues));
	}
	
	for(i=0;i<apMaxThroughput->wlanCount;i++)
	{
		wlanId = apMaxThroughput->wlanMaxThroughput[i].wlanId;
		/* get wlan mac */
		CWWTPGetWlanName(wlanname, radioId,wlanId);
		CWSetTrafficLimitEnable(wlanname,1);
		CWSetTrafficLimitThreshold(wlanname, (apMaxThroughput->wlanMaxThroughput[i].maxThroughput)*1024);
		gWlanMaxThroughputValue[wlanId-1].wlanId = wlanId;
		gWlanMaxThroughputValue[wlanId-1].maxThroughput = apMaxThroughput->wlanMaxThroughput[i].maxThroughput;
	}
#endif
	return CW_TRUE;
}

CWBool CWAssembleMsgElem_wtp_throughput_info(CWProtocolMessage *msgPtr, int interface_count,wlan_stats_info *wtp_throughput_info_values)             //pei add 0224
{
	int k;
	const int vendor_specific_payload_length = 1+1+(153+16+16+16+8+4)*interface_count;
	unsigned int old_rx_bytes, old_tx_bytes;
	if(msgPtr == NULL) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	
	CW_CREATE_PROTOCOL_MESSAGE(*msgPtr, vendor_specific_payload_length, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););

	CWProtocolStore8(msgPtr, ALL_IF_INFO);
	CWProtocolStore8(msgPtr, interface_count);
	for(k=0;k<interface_count;k++)
	{
		CWProtocolStore8(msgPtr, wtp_throughput_info_values[k].type);
		CWProtocolStore8(msgPtr, wtp_throughput_info_values[k].radioId);
		wtp_throughput_info_values[k].wlanId = CWGetRemoteWlanID(wtp_throughput_info_values[k].radioId, wtp_throughput_info_values[k].wlanId);
		CWProtocolStore8(msgPtr, wtp_throughput_info_values[k].wlanId);
		CWProtocolStoreRawBytes(msgPtr, (char *)(wtp_throughput_info_values[k].mac), 6);
		CWProtocolStore32(msgPtr, wtp_throughput_info_values[k].rx_packets);
		CWProtocolStore32(msgPtr, wtp_throughput_info_values[k].tx_packets);
		CWProtocolStore32(msgPtr, wtp_throughput_info_values[k].rx_errors);
		CWProtocolStore32(msgPtr, wtp_throughput_info_values[k].tx_errors);
		old_rx_bytes = wtp_throughput_info_values[k].rx_bytes/1024;
		old_tx_bytes = wtp_throughput_info_values[k].tx_bytes/1024;
		CWProtocolStore32(msgPtr, old_rx_bytes);
		CWProtocolStore32(msgPtr, old_tx_bytes);
		CWProtocolStore32(msgPtr, wtp_throughput_info_values[k].rx_rate);
		CWProtocolStore32(msgPtr, wtp_throughput_info_values[k].tx_rate);
		CWProtocolStore32(msgPtr, wtp_throughput_info_values[k].ast_rx_crcerr);
		CWProtocolStore32(msgPtr, wtp_throughput_info_values[k].ast_rx_badcrypt);
		CWProtocolStore32(msgPtr, wtp_throughput_info_values[k].ast_rx_badmic);
		CWProtocolStore32(msgPtr, wtp_throughput_info_values[k].ast_rx_phyerr);
		CWProtocolStore32(msgPtr, wtp_throughput_info_values[k].rx_dropped); 
		CWProtocolStore32(msgPtr, wtp_throughput_info_values[k].tx_dropped);
		CWProtocolStore32(msgPtr, wtp_throughput_info_values[k].rx_multicast);
		CWProtocolStore32(msgPtr, wtp_throughput_info_values[k].tx_multicast);
		CWProtocolStore32(msgPtr, wtp_throughput_info_values[k].rx_broadcast);
		CWProtocolStore32(msgPtr, wtp_throughput_info_values[k].tx_broadcast);
		CWProtocolStore32(msgPtr, wtp_throughput_info_values[k].rx_unicast);
		CWProtocolStore32(msgPtr, wtp_throughput_info_values[k].tx_unicast);
		CWProtocolStore32(msgPtr, wtp_throughput_info_values[k].rx_multicast_bytes);
		CWProtocolStore32(msgPtr, wtp_throughput_info_values[k].tx_multicast_bytes);
		CWProtocolStore32(msgPtr, wtp_throughput_info_values[k].rx_broadcast_bytes); /*save for broadcast_bytes*/
		CWProtocolStore32(msgPtr, wtp_throughput_info_values[k].tx_broadcast_bytes);
		CWProtocolStore32(msgPtr, wtp_throughput_info_values[k].rx_unicast_bytes);
		CWProtocolStore32(msgPtr, wtp_throughput_info_values[k].tx_unicast_bytes);
		CWProtocolStore32(msgPtr, wtp_throughput_info_values[k].rx_retry_bytes);
		CWProtocolStore32(msgPtr, wtp_throughput_info_values[k].tx_retry_bytes);
		CWProtocolStore32(msgPtr, wtp_throughput_info_values[k].rx_retry);
		CWProtocolStore32(msgPtr, wtp_throughput_info_values[k].tx_retry);
		CWProtocolStore32(msgPtr, wtp_throughput_info_values[k].rx_mgmt);
		CWProtocolStore32(msgPtr, wtp_throughput_info_values[k].tx_mgmt);
		CWProtocolStore64(msgPtr, wtp_throughput_info_values[k].rx_bytes);
		CWProtocolStore64(msgPtr, wtp_throughput_info_values[k].tx_bytes);
		CWProtocolStore64(msgPtr, wtp_throughput_info_values[k].rx_mgmt_bytes);
		CWProtocolStore64(msgPtr, wtp_throughput_info_values[k].tx_mgmt_bytes);
		CWProtocolStore64(msgPtr, wtp_throughput_info_values[k].rx_sum_bytes);
		CWProtocolStore64(msgPtr, wtp_throughput_info_values[k].tx_sum_bytes);
		CWProtocolStore64(msgPtr, wtp_throughput_info_values[k].rx_unicast64_bytes);
		CWProtocolStore64(msgPtr, wtp_throughput_info_values[k].tx_unicast64_bytes);
		CWProtocolStore32(msgPtr, wtp_throughput_info_values[k].rx_ctrl);
		CWProtocolStore32(msgPtr, wtp_throughput_info_values[k].tx_ctrl);
		CWProtocolStore32(msgPtr, wtp_throughput_info_values[k].rx_errors_frames);
		if(debug_print)
		{
			int i=0;
			printf("---assembling WTP Event Request Vendor Specific Payload Request.\n");
			printf("type:%d  (0--ATH, 1--ETH, 2--WIFI)\n", wtp_throughput_info_values[k].type);
			printf("radioId:%d\n", wtp_throughput_info_values[k].radioId);
			printf("wlanId:%d\n", wtp_throughput_info_values[k].wlanId);
			printf("mac:");
			for (i = 0; i < 6; i++){
				printf("%02x ", wtp_throughput_info_values[k].mac[i]);
			}
			printf("\n");
			printf("rx_packets:%d\n", wtp_throughput_info_values[k].rx_packets);
			printf("tx_packets:%d\n", wtp_throughput_info_values[k].tx_packets);
			printf("32bit_rx_bytes:%u\n", old_rx_bytes);
			printf("32bit_tx_bytes:%u\n", old_tx_bytes);
			printf("rx_errors:%d\n", wtp_throughput_info_values[k].rx_errors);
			printf("tx_errors:%d\n", wtp_throughput_info_values[k].tx_errors);
			printf("rx_rate:%d\n", wtp_throughput_info_values[k].rx_rate);
			printf("tx_rate:%d\n", wtp_throughput_info_values[k].tx_rate);
			printf("ast_rx_crcerr:%d\n", wtp_throughput_info_values[k].ast_rx_crcerr);
			printf("ast_rx_badcrypt:%d\n", wtp_throughput_info_values[k].ast_rx_badcrypt);
			printf("ast_rx_badmic:%d\n", wtp_throughput_info_values[k].ast_rx_badmic);
			printf("ast_rx_phyerr:%d\n", wtp_throughput_info_values[k].ast_rx_phyerr);
			printf("rx_dropped:%d\n", wtp_throughput_info_values[k].rx_dropped);
			printf("tx_dropped:%d\n", wtp_throughput_info_values[k].tx_dropped);			
			printf("rx_multicast:%d\n", wtp_throughput_info_values[k].rx_multicast);
			printf("tx_multicast:%d\n", wtp_throughput_info_values[k].tx_multicast);
			printf("rx_broadcast:%d\n", wtp_throughput_info_values[k].rx_broadcast);			
			printf("tx_broadcast:%d\n", wtp_throughput_info_values[k].tx_broadcast);
			printf("rx_unicast:%d\n", wtp_throughput_info_values[k].rx_unicast);
			printf("tx_unicast:%d\n", wtp_throughput_info_values[k].tx_unicast);
			printf("rx_multicast_bytes:%d\n", wtp_throughput_info_values[k].rx_multicast_bytes);
			printf("tx_multicast_bytes:%d\n", wtp_throughput_info_values[k].tx_multicast_bytes);
			printf("rx_broadcast_bytes:%d\n", wtp_throughput_info_values[k].rx_broadcast_bytes);
			printf("tx_broadcast_bytes:%d\n", wtp_throughput_info_values[k].tx_broadcast_bytes);
			printf("rx_unicast_bytes:%d\n", wtp_throughput_info_values[k].rx_unicast_bytes);
			printf("tx_unicast_bytes:%d\n", wtp_throughput_info_values[k].tx_unicast_bytes);
			printf("rx_retry_bytes:%d\n", wtp_throughput_info_values[k].rx_retry_bytes);
			printf("tx_retry_bytes:%d\n", wtp_throughput_info_values[k].tx_retry_bytes);
			printf("rx_retry:%d\n", wtp_throughput_info_values[k].rx_retry);
			printf("tx_retry:%d\n", wtp_throughput_info_values[k].tx_retry);	
			printf("rx_mgmt:%d\n", wtp_throughput_info_values[k].rx_mgmt);
			printf("tx_mgmt:%d\n", wtp_throughput_info_values[k].tx_mgmt);
			printf("64bit_rx_bytes:%llu\n", wtp_throughput_info_values[k].rx_bytes);
			printf("64bit_tx_bytes:%llu\n", wtp_throughput_info_values[k].tx_bytes);
			printf("64bit_rx_mgmt_bytes:%llu\n", wtp_throughput_info_values[k].rx_mgmt_bytes);
			printf("64bit_tx_mgmt_bytes:%llu\n", wtp_throughput_info_values[k].tx_mgmt_bytes);
			printf("64bit_rx_sum_bytes:%llu\n", wtp_throughput_info_values[k].rx_sum_bytes);
			printf("64bit_tx_sum_bytes:%llu\n", wtp_throughput_info_values[k].tx_sum_bytes);
			printf("64bit_rx_unicastbytes:%llu\n", wtp_throughput_info_values[k].rx_unicast64_bytes);
			printf("64bit_tx_unicastbytes:%llu\n", wtp_throughput_info_values[k].tx_unicast64_bytes);
			printf("rx_ctrl_frames:%d\n", wtp_throughput_info_values[k].rx_ctrl);
			printf("tx_ctrl_frames:%d\n", wtp_throughput_info_values[k].tx_ctrl);
			printf("rx_errors_frames:%d\n\n", wtp_throughput_info_values[k].rx_errors_frames);
		}
	}
	return CWAssembleMsgElem(msgPtr, CW_MSG_ELEMENT_VENDOR_SPEC_PAYLOAD_CW_TYPE);
}

CWBool CWAssemble_wtp_throughput_Info_report (CWProtocolMessage **messagesPtr, int *fragmentsNumPtr, int PMTU, int seqNum, int interface_count,wlan_stats_info *wtp_throughput_info) 	//pei add 0703
{
	CWProtocolMessage *msgElems= NULL;
	const int msgElemCount = 1;
	CWProtocolMessage *msgElemsBinding= NULL;
	const int msgElemBindingCount=0;
	
	if(messagesPtr == NULL || fragmentsNumPtr == NULL) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);

	CW_CREATE_PROTOCOL_MSG_ARRAY_ERR(msgElems, msgElemCount, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););	
	
	CWDebugLog("Assembling WTP Event Request AP extra_info...");

	// Assemble Message Elements
	if (!(CWAssembleMsgElem_wtp_throughput_info(msgElems,interface_count, wtp_throughput_info))) {
		CW_FREE_OBJECT(msgElems);
		return CW_FALSE; // error will be handled by the caller
	}
	
	if(!(CWAssembleMessage(messagesPtr,
			       fragmentsNumPtr,
			       PMTU,
			       seqNum,
			       CW_MSG_TYPE_VALUE_WTP_EVENT_REQUEST,
			       msgElems,
			       msgElemCount,
			       msgElemsBinding,
			       msgElemBindingCount,
#ifdef CW_NO_DTLS
			       CW_PACKET_PLAIN
#else			       
			       CW_PACKET_CRYPT
#endif
			       ))) 
		return CW_FALSE;
	
	CWDebugLog("WTP Event Request AP extra_info Assembled");

	return CW_TRUE;
}
CWBool CWget_wtp_throughput_Info(ApThroughputInfoValues *apThroughputInfoCollectValues)
{
	CWWTPDebugLog("got msg to set_throughput_Info report, begin to set...");
	if(apThroughputInfoCollectValues->value!=APThroughputInfo)
	{
		return CW_FALSE;
	}

	if(apThroughputInfoCollectValues->opstate == 0)
	{
		CWWTPDebugLog("set wtp_throughput_Info disable");
		if(gAPThroughputInfoCollectEnable == 1)
		{
			if( (thread_wtp_throughput_info != 0) && (0 == pthread_kill(thread_wtp_throughput_info, 0) ) ) {
				pthread_cancel(thread_wtp_throughput_info);
			}
//			gAPThroughputInfoThreadEnable = 0;
			gAPThroughputInfoCollectEnable = 0;
			CWWTPDebugLog("set OK!");
		}
		else
		{
			CWWTPDebugLog("wtp_throughput_Info is already disable");
		}
	}
	else if(apThroughputInfoCollectValues->opstate == 1)
	{
		CWWTPDebugLog("set wtp_throughput_Info enable");
		if(gAPThroughputInfoCollectEnable)
		{
//			gAPThroughputInfoThreadEnable = 1;
			CWWTPDebugLog("wtp_throughput_Info is already enable");
		}
		else
		{
			gAPThroughputInfoCollectEnable = 1;
//			gAPThroughputInfoThreadEnable = 1;
			CWWTPDebugLog("set OK!");
			if( (thread_wtp_throughput_info != 0) && (0 == pthread_kill(thread_wtp_throughput_info, 0) ) ) {
				pthread_cancel(thread_wtp_throughput_info);
			}
			if(!CWErr(CWCreateThread(&thread_wtp_throughput_info, acw_wtp_throughput_info, NULL,0))) {  //pei change (void *)apScanningSetValues->reportinterval  to NULL 1127
				CWDebugLog("Error starting Thread to get wtp_throughput_Info");
				CWWTPDebugLog("Error starting Thread to get wtp_throughput_Info");
				return CW_FALSE;
			}
		}
		
	}
	
	ConfArg confArg;
	memset(&confArg,0,sizeof(confArg));
	strcpy(confArg.conf_arg,"gAPThroughputInfoCollectEnable");
	sprintf(confArg.conf_value,"%d",gAPThroughputInfoCollectEnable);
	CWSaveConfigInfo(CONF_WTPD,confArg,CW_TRUE);
	
	memset(&confArg,0,sizeof(confArg));
	strcpy(confArg.conf_arg,"gAPThroughputInfoInterval");
	sprintf(confArg.conf_value,"%hd",gAPThroughputInfoInterval);
	CWSaveConfigInfo(CONF_WTPD,confArg,CW_FALSE);
	return CW_TRUE;
}
CW_THREAD_RETURN_TYPE acw_wtp_throughput_info(void *arg)  
{
	unsigned short reportintval;
	if(arg!=NULL)
	{
		CWWTPDebugLog("into run----start the wtp_throughput_info thread---enable in config state---sleep first\n");
		sleep((int)arg);
	}
	
//	reportintval = (unsigned short)arg;
	
	CW_REPEAT_FOREVER 
	{
		/* if disable the wtp_throughput_info, exit the thread. */
		if(!gAPThroughputInfoCollectEnable)
		{
			printf("exit_thread\n");
			goto exit_thread;
		}
		reportintval = gAPThroughputInfoInterval;
		dpf("reportintval:%d\n", reportintval);

		int i=0;
		int interface_count = 0;
		int wlan_count=0;
		int eth_count = 0;
		int wifi_count = 0;
		wlan_stats_info *valuesPtr;
		
		wlan_count = wtp_wlan_count;
		if(wlan_count>16){
			goto exit_thread;
		}

		/* get eth and wifi count */
		CWGetInterfaceCount(&eth_count, "eth[0-9]");
		//CWGetInterfaceCount(&wifi_count, "wifi[0-9]");
		wifi_count = CWGetRadioMaxCount();
		if(wifi_count == 0)
			wifi_count = 1;
		interface_count = wlan_count+eth_count+wifi_count;
		dpf("get interface_count:%d\n", interface_count);
		
		CW_CREATE_ARRAY_ERR(valuesPtr, interface_count, wlan_stats_info, CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
		for(i=0;i<interface_count;i++)
		{
			memset(&valuesPtr[i], 0, sizeof(wlan_stats_info));
		}	
		if(!CWGetAPThroughputInfo(wlan_count, eth_count, wifi_count, valuesPtr)){
			CWWTPDebugLog("get wlan throughput info error, no wlan exist");
			CW_FREE_OBJECT(valuesPtr);
			goto exit_thread;
		}
		
		CWProtocolMessage *messages = NULL;
		int fragmentsNum=0;
		int seqNum;
		seqNum = CWGetSeqNum();

		if(!CWAssemble_wtp_throughput_Info_report(&messages, &fragmentsNum, gWTPPathMTU, seqNum, interface_count,valuesPtr))  /*pei test for neighbor AP info list. 1125*/
		{
			CWDebugLog("Assemble WTP Event wtp throughput_Info report  error!");
			CWWTPDebugLog("Assemble WTP Event wtp throughput_Info report  error!");
			CW_FREE_OBJECT(valuesPtr);
			goto exit_thread;
		}
		CW_FREE_OBJECT(valuesPtr);
		
			/* send ExtraInfo info */
		for(i = 0; i < fragmentsNum; i++) 
		{
//			if(debug_print)
//			{
//				printf("messages[%d].offset:%d\n", i, messages[i].offset);
//				CWCaptrue(messages[i].offset, (unsigned char *)(messages[i].msg));
//			}

#ifndef CW_NO_DTLS
			if(gDtlsSecurity == 1)
			{
				if(!CWSecuritySend(gWTPSession, messages[i].msg, messages[i].offset))
				{
					CWDebugLog("Error sending message");
					CWFreeMessageFragments(messages, fragmentsNum);
					CW_FREE_OBJECT(messages);
					CWWTPDebugLog("Error sending msg of ap throughput info");
					goto exit_thread;
				}
			}
			else
#endif
			{
				if (!CWNetworkSendUnsafeUnconnected(gWTPSocket, &(gACInfoPtr->preferredAddress), messages[i].msg, messages[i].offset))
				{
					CWDebugLog("Error sending message");
					CWFreeMessageFragments(messages, fragmentsNum);
					CW_FREE_OBJECT(messages);
					CWWTPDebugLog("Error sending msg of ap throughput info");
					goto exit_thread;
				}
			}
		}
		CWDebugLog("Message Sent\n");
		CWFreeMessageFragments(messages, fragmentsNum);
		CW_FREE_OBJECT(messages);

		/* if disable the AP ExtraInfo, exit the thread. */
		if(!gAPThroughputInfoCollectEnable)
		{
			CWWTPDebugLog("disable ap throughput info, exit_thread");
			goto exit_thread;
		}
		sleep(reportintval);
	}

exit_thread:
	CWWTPDebugLog("Exit the thread!");
	gAPThroughputInfoCollectEnable = 0;
	CWExitThread();
	return NULL;
}
/*
*my order of base rate is different from the data of drv.
*my order is  2, 4, 11, 12, 18,22,  24, 36, 48, 72, 96, 108
*drv order is 2, 4, 11, 22, 12, 18, 24, 36, 48, 72, 96, 108
*map it to suitable.
*/
static CWBool MapBaseRateCountsFromDrv(unsigned int base_rate_count[12],struct rate_info rateinfo)
{
	int i;
	//int base[12]={2, 4, 11, 22, 12, 18, 24, 36, 48, 72, 96, 108};
	int base_rate[12]={2, 4, 11, 12, 18,22,  24, 36, 48, 72, 96, 108};
	for(i = 0; i < 12 ;i++){
		if(base_rate[i] == rateinfo.dot11Rate){
			base_rate_count[i] = rateinfo.count;
			return CW_TRUE;
		}
	}
	return CW_FALSE;
}
CWBool CWGetStaPktsStatisticRptInfo(int *wlan_count,WlanStaPktsStatisticRpt **rpt)
{
	int idx = 0;
	int s = -1;
	CWWTPWlan *pWlan = wtp_wlan_list;

	if((s=socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP)) < 0){
		CWWTPDebugLog("%s,create socket failed.%s",__func__,strerror(errno));
		return CW_FALSE;
	}
	
	*wlan_count = wtp_wlan_count;
	if(*wlan_count == 0){
		dpf("%s,*wlan_count=%d,no sta info to report\n",__func__,*wlan_count);
		return CW_TRUE;
	}
	dpf("%s,%d,*wlan_count=%d\n",__func__,__LINE__,*wlan_count);
	CW_CREATE_ARRAY_ERR(*rpt,(*wlan_count)*sizeof(WlanStaPktsStatisticRpt),WlanStaPktsStatisticRpt,return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY,NULL););
	memset(*rpt,0,(*wlan_count)*sizeof(WlanStaPktsStatisticRpt));

	for(;pWlan !=NULL;pWlan=pWlan->next)
	{
		struct iwreq wrq;
		char bssid[IFNAMSIZ];
		char wlanname[IFNAMSIZ];
		struct iw_ni_rate_frame_req reqopt;

		memset(bssid,0,IFNAMSIZ);
		memcpy(bssid,pWlan->wlan_bssid,IFNAMSIZ);
		memset(wlanname,0,IFNAMSIZ);
		CWWTPGetWlanName(wlanname,pWlan->radio_id,pWlan->wlan_id);
		dpf("%s,wlanname:%s\n",__func__,wlanname);
		
		memset(&wrq,0,sizeof(wrq));
		strncpy(wrq.ifr_name,wlanname,IFNAMSIZ);
		memset(&reqopt,0,sizeof(reqopt));
		wrq.u.data.pointer = (caddr_t)&(reqopt);
		wrq.u.data.length = sizeof(reqopt);
		wrq.u.data.flags = 0;
		if(ioctl(s,SIOCGIWNIRATEFRAME,&wrq) < 0){
			dpf("%s,ioctl failed.%s\n",__func__,strerror(errno));
			CWWTPDebugLog("%s,ioctl failed.%s",__func__,strerror(errno));
			CW_FREE_OBJECT(*rpt);
			return CW_FALSE;
		}else{
			int i = 0;
			int sta_count = reqopt.ni_count;
			struct ni_info *ni = reqopt.ni_data;
			if(sta_count == 0){
				dpf("%s,%s has no sta.\n",__func__,wlanname);
				continue;
			}
			if(sta_count  > NI_COUNT){
				dpf("%s,too many sta(%d).we have at most %d sta.\n",__func__,sta_count,wtp_sta_count);
				CWWTPDebugLog("%s,too many sta(%d).we have at most %d sta.",__func__,sta_count,wtp_sta_count);
			}
			CW_CREATE_ARRAY_ERR((*rpt)[idx].pStaPktsStatistic,sta_count*sizeof(StaPktsStatistic),StaPktsStatistic,return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY,NULL););
			memset((*rpt)[idx].pStaPktsStatistic,0,sta_count*sizeof(StaPktsStatistic));
			
			(*rpt)[idx].sta_count = sta_count;
			dpf("%s,%s has %d sta.\n",__func__,wlanname,sta_count);
			for(i=0;i < sta_count ;i++){
				memcpy((*rpt)[idx].pStaPktsStatistic[i].sta_mac,ni[i].mac,MAC_ADDR_LEN);
				memcpy((*rpt)[idx].pStaPktsStatistic[i].bssid,bssid,MAC_ADDR_LEN);
				int j = 0,jj = 0 ;
				for(;j<12;j++){
					MapBaseRateCountsFromDrv((*rpt)[idx].pStaPktsStatistic[i].StaRxDataRatePkts,ni[i].ni_rx_data.rate[j]);
					//(*rpt)[idx].pStaPktsStatistic[i].StaRxDataRatePkts[j] = ni[i].ni_rx_data.rate[j].count;
				}
				for(jj = 0;j<CURRENT_SUPPORT_MCS_COUNT;j++,jj++){
					(*rpt)[idx].pStaPktsStatistic[i].StaRxDataRatePkts[j] = ni[i].ni_rx_data.mcs[jj];
				}
				for(j=0;j<12;j++){
					MapBaseRateCountsFromDrv((*rpt)[idx].pStaPktsStatistic[i].StaTxDataRatePkts,ni[i].ni_tx_data.rate[j]);
					//(*rpt)[idx].pStaPktsStatistic[i].StaTxDataRatePkts[j] = ni[i].ni_tx_data.rate[j].count;
				}
				for(jj = 0;j<CURRENT_SUPPORT_MCS_COUNT;j++,jj++){
					(*rpt)[idx].pStaPktsStatistic[i].StaTxDataRatePkts[j] = ni[i].ni_tx_data.mcs[jj];
				}
				for(j=0;j<SIGNAL_STRENGTH_REGION;j++){
					(*rpt)[idx].pStaPktsStatistic[i].StaTxSignalStrengthPkts[j] = ni[i].ni_rssi_stats[j].ns_rx_data;
				}
			}
		}
		idx++;
	}
	close(s);
	return CW_TRUE;
}
CWBool CWAssembleMsgElem_StaPktsStatisticRpt(CWProtocolMessage *msgPtr,const WlanStaPktsStatisticRpt wlanStaPktsStatisticRpt)             
{
	int k;
	WlanStaPktsStatisticRpt s = wlanStaPktsStatisticRpt;
	const int vendor_specific_payload_length = 4			//vendor id(31656)
										+2			//elem_type
										+2			//elem_len
										+2			//sta_count
										+(
										   +6			//sta_mac
										   +44*4		//StaTxDataRatePkts*4
										   +44*4		//StaRxDataRatePkts*4
										   +17*4		//StaTxSignalStrengthPkts*4
										   )*s.sta_count;
	
	if(msgPtr == NULL) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);

	CW_CREATE_PROTOCOL_MESSAGE(*msgPtr, vendor_specific_payload_length, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););

	int vendor_id = AUTELAN_VENDOR;
	CWProtocolStore32(msgPtr,/*1<<31+*/vendor_id/*AUTELAN_VENDOR*/);
	CWProtocolStore16(msgPtr, EVENT_T_STA_PACKET_STATISTIC_REPORT);
	CWProtocolStore16(msgPtr, vendor_specific_payload_length -8);
	CWProtocolStore16(msgPtr, s.sta_count);
	for(k=0;k<s.sta_count;k++)
	{
		CWProtocolStoreRawBytes(msgPtr,(char *)s.pStaPktsStatistic[k].sta_mac,MAC_ADDR_LEN);
		CWProtocolStoreRawBytes(msgPtr,(char *)s.pStaPktsStatistic[k].StaTxDataRatePkts,STA_DATA_RATE_COUNT*4);
		CWProtocolStoreRawBytes(msgPtr,(char *)s.pStaPktsStatistic[k].StaRxDataRatePkts,STA_DATA_RATE_COUNT*4);
		CWProtocolStoreRawBytes(msgPtr,(char *)s.pStaPktsStatistic[k].StaTxSignalStrengthPkts,SIGNAL_STRENGTH_REGION*4);
	}
	if(debug_print)
	{
		char strRate[12][6]={"1M","2M","5.5M","6M","9M","11M","12M","18M","24M","36M","48M","54M"};//1-12
		char strSignalStrengthRegion[17][8]={">-10","-10~-19","-20~-39","-40~-49","-50~-59","-60~-64","-65~-67","-68~-70","-71~-73",
			"-74~-76","-77~-79","-80~-82","-83~-85","-86~-88","-89~-91","-92~-94","<-94"};
		for(k=0;k<s.sta_count;k++)
		{
			int m,mcs_idx;
			unsigned char mac[6];
			unsigned int *pTx	=	s.pStaPktsStatistic[k].StaTxDataRatePkts;
			unsigned int *pRx	=	s.pStaPktsStatistic[k].StaRxDataRatePkts;
			unsigned int *pTs	=	s.pStaPktsStatistic[k].StaTxSignalStrengthPkts;
			memcpy(mac,s.pStaPktsStatistic[k].sta_mac,6);
			printf("----------------StaPktsStatisticReport begin to report sta info to AC-------------\n");
			printf("Sta(%02x:%02x:%02x:%02x:%02x:%02x)TxDataRatePkts:\n",mac[0],mac[1],mac[2],mac[3],mac[4],mac[5]);
			for(m=0,mcs_idx=0;m<STA_DATA_RATE_COUNT;m++)
			{
				if(m<12)
					printf("%5s:%-10u  %s",strRate[m],pTx[m],(m%5==4)?"\n":"");
				else
					printf("MCS%02d:%-10u  %s",mcs_idx++,pTx[m],(m%5==4)?"\n":"");
			}
			printf("\nSta(%02x:%02x:%02x:%02x:%02x:%02x)RxDataRatePkts:\n",mac[0],mac[1],mac[2],mac[3],mac[4],mac[5]);
			for(m=0,mcs_idx=0;m<STA_DATA_RATE_COUNT;m++)
			{
				if(m<12)
					printf("%5s:%-10u  %s",strRate[m],pRx[m],(m%5==4)?"\n":"");
				else
					printf("MCS%02d:%-10u  %s",mcs_idx++,pRx[m],(m%5==4)?"\n":"");
			}
			printf("\nSta(%02x:%02x:%02x:%02x:%02x:%02x)TxSignalStrengthPkts:\n",mac[0],mac[1],mac[2],mac[3],mac[4],mac[5]);
			for(m=0;m<SIGNAL_STRENGTH_REGION;m++)
			{
				printf("[%7s]:%-10u  %s",strSignalStrengthRegion[m],pTs[m],(m%5==4)?"\n":"");
			}
			printf("\n----------------StaPktsStatisticReport end to report sta info to AC---------------\n");
		}
	}
	return CWAssembleMsgElem(msgPtr, CW_MSG_ELEMENT_VENDOR_SPEC_PAYLOAD_CW_TYPE);
}

CWBool CWAssemble_StaPktsStatisticRpt(CWProtocolMessage **messagesPtr, int *fragmentsNumPtr, int PMTU, int seqNum,WlanStaPktsStatisticRpt staPktsStatisticRpt) 
{
	CWProtocolMessage *msgElems= NULL;
	const int msgElemCount = 1;
	CWProtocolMessage *msgElemsBinding= NULL;
	const int msgElemBindingCount=0;
	
	if(messagesPtr == NULL || fragmentsNumPtr == NULL) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);

	CW_CREATE_PROTOCOL_MSG_ARRAY_ERR(msgElems, msgElemCount, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););	
	
	CWDebugLog("WTP Event Request AP StaPktsStatisticRpt .....");

	// Assemble Message Elements
	if (!(CWAssembleMsgElem_StaPktsStatisticRpt(msgElems,staPktsStatisticRpt))) {
		CW_FREE_OBJECT(msgElems);
		return CW_FALSE; // error will be handled by the caller
	}

	/*
	*simulate a wireless header of manage frame ,
	*which subtype is RESERVED6_SUBTYPE,
	*transfer from STA to AP
	*by diaowq @20120416
	*/
	char wh[WIRELESS_HEADER_LEN+LLC_LEN];
	memset(&wh,0,sizeof(wh));
	wh[0] = 0x60;	//version=0,type=manage(00),subtype=6(RESERVED6_SUBTYPE)
	wh[1] = 0x01;	//toDS=1,other fields=0.
	memcpy(wh+4,staPktsStatisticRpt.pStaPktsStatistic[0].bssid,MAC_ADDR_LEN);
	memcpy(wh+4+MAC_ADDR_LEN,staPktsStatisticRpt.pStaPktsStatistic[0].sta_mac,MAC_ADDR_LEN);

	if(!(CWAssembleMessageV2(messagesPtr,
			       fragmentsNumPtr,
			       PMTU,
			       seqNum,
			       CW_MSG_TYPE_VALUE_WTP_EVENT_REQUEST,
			       msgElems,
			       msgElemCount,
			       msgElemsBinding,
			       msgElemBindingCount,
#ifdef CW_NO_DTLS
			       CW_PACKET_PLAIN,
#else			       
			       CW_PACKET_CRYPT,
#endif
			       wh,
			       sizeof(wh)))) 
		return CW_FALSE;
	
	CWDebugLog("WTP Event Request AP StaPktsStatisticRpt Assembled");

	return CW_TRUE;
}
/*
*start a thread,which continuing to report sta packets's rate statistic info to AC, 
*via the DATA tunnel (Notice:here transfer control msgs, not using control tunnel)
*statistic info include StaTxDataRatePkts,StaRxDataRatePkts,StaTxSignalStrengthPkts
*/
CW_THREAD_RETURN_TYPE CWWTPStaPktsStatisticReport(void *arg)
{
	unsigned short reportinterval;
	if(arg != NULL){
		CWWTPDebugLog("start the sta_pkts_statistic thread\n");
		if(debug_print)
			printf("start the wtp_throughput_info thread\n");
		sleep((unsigned int)arg);
	}
	
	CW_REPEAT_FOREVER
	{
		if(!gStaPktsStatisticEnable)
			goto exit_thread;
		reportinterval = gStaPktsStatisticInterval;
		dpf("sta_pkts_statistic_report interval:%d\n",reportinterval);
		
		int i=0;
		int wlan_count = 0;
		WlanStaPktsStatisticRpt *wlanStaPktsStatisticRpt;
		//CW_CREATE_ARRAY_ERR(wlanStaPktsStatisticRpt,wtp_wlan_count*sizeof(WlanStaPktsStatisticRpt),WlanStaPktsStatisticRpt,CWErrorRaise(CW_ERROR_OUT_OF_MEMORY,NULL););
		if(!CWGetStaPktsStatisticRptInfo(&wlan_count,&wlanStaPktsStatisticRpt)){
			dpf("GetStaPktsStatisticRptInfo failed.just have a break.\n");
			CWWTPDebugLog("GetStaPktsStatisticRptInfo failed.just have a break.");
			goto have_a_sleep;
		};
		
		int k;
		dpf("%s,%d,wlan_count:%d\n",__func__,__LINE__,wlan_count);
		for(k=0;k<wlan_count;k++)
		{
			int kk = 0;
			dpf("%s,%d,wlanStaPktsStatisticRpt[%d].sta_count:%d\n",__func__,__LINE__,k,wlanStaPktsStatisticRpt[k].sta_count);
			for(;kk<wlanStaPktsStatisticRpt[k].sta_count;kk++)
			{
				WlanStaPktsStatisticRpt justRptOneByOne;
				justRptOneByOne.sta_count = 1;
				justRptOneByOne.pStaPktsStatistic = &(wlanStaPktsStatisticRpt[k].pStaPktsStatistic[kk]);

				CWProtocolMessage *messages = NULL;
				int fragmentsNum=0;
				int seqNum;
				
				seqNum = CWGetSeqNum();
				if(!CWAssemble_StaPktsStatisticRpt(&messages, &fragmentsNum, gWTPPathMTU, seqNum,justRptOneByOne))  
				{
					CWDebugLog("Assemble WTP  StaPktsStatisticReport error!");
					CWWTPDebugLog("Assemble WTP StaPktsStatisticReport error!");
					dpf("Assemble WTP StaPktsStatisticReport error!exit_thread\n");
					goto exit_thread;
				}
				
				for(i = 0; i < fragmentsNum; i++) 
				{
					CWNetworkLev4Address  DataChannelAddr;
#ifdef IPv6
					struct sockaddr_in6 temptaddr;
					CW_COPY_NET_ADDR_PTR(&temptaddr , &(gACInfoPtr->preferredAddress));
					temptaddr.sin6_port = htons(CW_DATA_PORT);	
#else
					struct sockaddr_in temptaddr;
					CW_COPY_NET_ADDR_PTR(&temptaddr , &(gACInfoPtr->preferredAddress));
					temptaddr.sin_port = htons(CW_DATA_PORT); 
#endif
					CW_COPY_NET_ADDR_PTR(&DataChannelAddr , &temptaddr);
	
#ifndef CW_NO_DTLS
					if((gDtlsSecurity == 1)&&(gDtlsPolicy == 1))
					{
						if(!CWSecuritySend(gWTPDataSession, messages[i].msg, messages[i].offset))
						{
							CWWTPDebugLog("%s,Failure dtls sending",__func__);
							break;
						}
					}
					else
#endif
					{
						if (!CWNetworkSendUnsafeUnconnected(gWTPDataSocket, &DataChannelAddr, messages[i].msg, messages[i].offset))
						{
							CWWTPDebugLog("%s,Failure sending",__func__);
							break;
						}
					}
				}
				CWDebugLog("Message Sent\n");
				CWFreeMessageFragments(messages, fragmentsNum);
				CW_FREE_OBJECT(messages);
			}
			if(wlanStaPktsStatisticRpt[k].sta_count > 0)
				CW_FREE_OBJECT(wlanStaPktsStatisticRpt[k].pStaPktsStatistic);

		}
		if(wlan_count > 0)
			CW_FREE_OBJECT(wlanStaPktsStatisticRpt);
have_a_sleep:
		sleep(reportinterval);
	}
exit_thread:
	CWWTPDebugLog("exit the sta_pkts_statistic thread\n");
	dpf("exit the wtp_throughput_info thread\n");
	gStaPktsStatisticEnable = 0;
	CWExitThread();
	return NULL;
}


CWBool Txpowerget(CWProtocolResultCode *resultCode,unsigned char radio_id, char *wlanname,int type)	
{
	int Retxpower; 
	char temp_str[128];
	char str_tmp[24];
	char str[24];
	memset(temp_str, 0, 128);
	memset(str_tmp, 0 ,24);
	sprintf(temp_str,"/usr/sbin/gettxpower %s |awk  '{print $0}'",wlanname);
	PopenFile(temp_str, str_tmp, sizeof(str_tmp));
	memset(str, 0, 24);
	strcpy(str, trim(str_tmp));
//		dpf("txpower:%s len:%d\n", str, (int)strlen(str));

	CWGetTxpower(&Retxpower,wlanname);
//	printf("Retxpower=%d\n",Retxpower);
	type = type<<4;
	*resultCode=radio_id|type;
//	printf("1resultcode=%d\n",*resultCode);
	*resultCode=*resultCode<<16;
//	printf("2resultcode=%d\n",*resultCode);
	*resultCode=*resultCode|Retxpower;
//	printf("3resultcode=%d\n",*resultCode);
	CWWTPDebugLog("radio_id:%d txpower:%d  resultCode:%d", radio_id, Retxpower, *resultCode);
	
	return CW_TRUE;
}

CWBool Channelget(CWProtocolResultCode *resultCode,unsigned char radio_id, char *wlanname)
{
	int Rechannel;
	int type = 1;
	
	CWGetChannel(&Rechannel,wlanname);

	gRadioInfoValue[radio_id].channel = Rechannel;
//	printf("Rechannel=%d\n",Rechannel);
	type = type<<4;
	*resultCode=radio_id|type;
//	printf("1resultcode=%d\n",*resultCode);
	*resultCode=*resultCode<<16;
//	printf("2resultcode=%d\n",*resultCode);
	*resultCode=*resultCode|Rechannel;
//	printf("3resultcode=%d\n",*resultCode);
	CWWTPDebugLog("type:%d radio_id:%d channel:%d  resultCode:%d", type, radio_id, Rechannel, *resultCode);
	
	return CW_TRUE;
}

void CWSetLastConnectGateway(void)
{
	char *local_gw_file = "/jffs/last_connect_gw";
	char temp_str[256];
	char cmdbuf[128];
	char str[64];
	
	memset(temp_str, 0, 256);
	sprintf(temp_str, "route -n | awk -F \" \" '/UG/ {print $2}'");
	PopenFile(temp_str, str, sizeof(str));
	
	dpf("Last Connect Gateway: %s  len:%d\n", str, (int)strlen(str));
	
	if(strlen(str))
	{
		memset(cmdbuf, 0, 128);
		sprintf(cmdbuf, "echo %s > %s", str, local_gw_file);
		_system(cmdbuf);
	}
	
	return;
}

void WTPInitConfig()
{
	char cmdbuf[100];
	char wlanname[IFNAMSIZ];
	CWWTPWlan *ptr = NULL,*ptr_t= NULL;
	CWWTPWlan *oldptr = NULL,*oldptr_t = NULL;
	int vlan_found = 0;
	int eth_nu=0;	
	
#if (defined( AQ2000N )|| defined(XHPB44)||defined(XJAP93V1))
       eth_nu=1;
#endif

	for(ptr=wtp_wlan_list;ptr != NULL;oldptr = ptr,ptr = ptr->next)
	{	
		CWWTPGetWlanName(wlanname, ptr->radio_id, ptr->wlan_id);
		if(ptr->vlan_id)
		{
			for(ptr_t=wtp_wlan_list;ptr_t != NULL;oldptr_t = ptr_t,ptr_t = ptr_t->next)
			{
				if(ptr-> vlan_id == ptr_t->vlan_id)
				{
					if((ptr->radio_id!= ptr_t->radio_id)||(ptr->wlan_id != ptr_t->wlan_id))
					{
						vlan_found = 1;
						break;
					}
				}
			}
			sprintf(cmdbuf,"/sbin/ifconfig %s down", wlanname);
			_system(cmdbuf);
			sprintf(cmdbuf,"brctl delif br%d %s", ptr->vlan_id, wlanname);
			_system(cmdbuf);
			sprintf(cmdbuf,"wlanconfig %s destroy", wlanname);
			_system(cmdbuf);

			if(!vlan_found)
			{
				sprintf(cmdbuf,"brctl delif br%d eth%d.%d", ptr->vlan_id, eth_nu, ptr->vlan_id);
				_system(cmdbuf);
				sprintf(cmdbuf,"/sbin/ifconfig eth%d.%d down",eth_nu, ptr->vlan_id);
				_system(cmdbuf);
				sprintf(cmdbuf, "vconfig rem eth%d.%d",eth_nu, ptr->vlan_id);
				_system(cmdbuf);
#ifdef CAVIUM_PLATFORM
				int ethCount = 1;
				CWGetInterfaceCount(&ethCount, "eth[0-9]");
				if(ethCount == 2)
				{
					sprintf(cmdbuf,"brctl delif br%d eth1.%d", ptr->vlan_id, ptr->vlan_id);
					_system(cmdbuf);
					sprintf(cmdbuf,"/sbin/ifconfig eth1.%d down", ptr->vlan_id);
					_system(cmdbuf);
					sprintf(cmdbuf, "vconfig rem eth1.%d", ptr->vlan_id);
					_system(cmdbuf);
				}
#endif
				sprintf(cmdbuf,"/sbin/ifconfig br%d down", ptr->vlan_id);
				_system(cmdbuf);
				sprintf(cmdbuf, "brctl delbr br%d", ptr->vlan_id);
				_system(cmdbuf);
			}
		}
		else
		{
			sprintf(cmdbuf,"/sbin/ifconfig %s down", wlanname);
			_system(cmdbuf);
			sprintf(cmdbuf, "wapiset wlan %s del > /dev/null 2>&1", wlanname);  
			_system(cmdbuf);
			sprintf(cmdbuf,"brctl delif default %s", wlanname);
			_system(cmdbuf);
			sprintf(cmdbuf,"wlanconfig %s destroy", wlanname);
			_system(cmdbuf);
		}
		vlan_found = 0;
	}

	if(wtp_wlan_list == NULL){
		_system("echo 0 > /proc/sys/net/capwap/spfast_down");  
		_system("echo 0 > /proc/sys/net/capwap/spfast_up");  
	}

	_system("echo 0 > /proc/sys/dev/wifi0/thinap_state");	
	_system("echo 0 > /proc/sys/dev/wifi0/thinap_check_timer");
	
	_system("stop_monitor");
	_system("rm -f /tmp/run_state");
	_system("rm -f /tmp/manager_id");
	
	/* clear */
	_system("killall linkcheck");
	_system("killall iwlist_scan");
	_system("killall cpu_util_stat");
	_system("killall crond");
	return;
}

CWBool WTPGetApRebootFlag()
{
	char *ap_reboot_flag_file = "/tmp/aprebootflag";
	char temp_str[128];
	char str_tmp[12];
	char str[12];
	int apRebootFlag = 0;

	if(access(ap_reboot_flag_file, 0)!=0)
	{
		//printf("The file is not exist!\n");
		return CW_TRUE;
	}
	
	memset(temp_str, 0, 128);
	sprintf(temp_str, "cat %s", ap_reboot_flag_file);
	PopenFile(temp_str, str_tmp, sizeof(str_tmp));
	memset(str, 0, 12);
	strcpy(str, trim(str_tmp));
	apRebootFlag = atoi(str);
//		dpf("ath:%s current channel:%s len:%d\n", wlanname, str, (int)strlen(str));

	
	dpf("apRebootFlag: %d\n", apRebootFlag);

	if(apRebootFlag == 0)
		return CW_FALSE;
	
	return CW_TRUE;
}
// static void

CWBool printstats(FILE *fd, struct ieee80211_stats *stats)
{
#define	N(a)	(sizeof(a) / sizeof(a[0]))
#define	STAT(x,fmt) \
	if (stats->is_##x) fprintf(fd, "%u " fmt "\n", stats->is_##x)
	STAT(rx_badversion,	"rx frame with bad version");
	STAT(rx_tooshort,	"rx frame too short");
	STAT(rx_wrongbss,	"rx from wrong bssid");
	STAT(rx_wrongdir,	"rx w/ wrong direction");
	STAT(rx_mcastecho,	"rx discard 'cuz mcast echo");
	STAT(rx_notassoc,	"rx discard 'cuz sta !assoc");
	STAT(rx_noprivacy,	"rx w/ wep but privacy off");
	STAT(rx_decap,		"rx decapsulation failed");
	STAT(rx_mgtdiscard,	"rx discard mgt frames");
	STAT(rx_ctl,		"rx discard ctrl frames");
	STAT(rx_beacon,		"rx beacon frames");
	STAT(rx_rstoobig,	"rx rate set truncated");
	STAT(rx_elem_missing,	"rx required element missing");
	STAT(rx_elem_toobig,	"rx element too big");
	STAT(rx_elem_toosmall,	"rx element too small");
	STAT(rx_elem_unknown,	"rx element unknown");
	STAT(rx_badchan,	"rx frame w/ invalid chan");
	STAT(rx_chanmismatch,	"rx frame chan mismatch");
	STAT(rx_nodealloc,	"nodes allocated (rx)");
	STAT(rx_ssidmismatch,	"rx frame ssid mismatch");
	STAT(rx_auth_unsupported,"rx w/ unsupported auth alg");
	STAT(rx_auth_fail,	"rx sta auth failure");
	STAT(rx_auth_countermeasures,
		"rx sta auth failure 'cuz of TKIP countermeasures");
	STAT(rx_assoc_bss,	"rx assoc from wrong bssid");
	STAT(rx_assoc_notauth,	"rx assoc w/o auth");
	STAT(rx_assoc_capmismatch,"rx assoc w/ cap mismatch");
	STAT(rx_assoc_norate,	"rx assoc w/ no rate match");
	STAT(rx_assoc_badwpaie,	"rx assoc w/ bad WPA IE");
	STAT(rx_deauth,		"rx deauthentication");
	STAT(rx_disassoc,	"rx disassociation");
	STAT(rx_action,		"rx action mgt");
	STAT(rx_badsubtype,	"rx frame w/ unknown subtype");
	STAT(rx_nobuf,		"rx failed for lack of sk_buffer");
	STAT(rx_ahdemo_mgt,
		"rx discard mgmt frame received in ahdoc demo mode");
	STAT(rx_bad_auth,	"rx bad authentication request");
	STAT(rx_unauth,		"rx discard 'cuz port unauthorized");
	STAT(rx_badcipher,	"rx failed 'cuz bad cipher/key type");
	STAT(tx_nodefkey, "tx failed 'cuz no defkey");
	STAT(tx_noheadroom,"tx failed 'cuz no space");
	STAT(rx_nocipherctx,	"rx failed 'cuz key/cipher ctx not setup");
	STAT(rx_acl,		"rx discard 'cuz acl policy");
	STAT(rx_ffcnt,		"rx fast frames");
	STAT(rx_badathtnl,   	"rx fast frame failed 'cuz bad tunnel header");
	//STAT(rx_nowds,"4-addr packets received with no wds enabled");
	STAT(tx_nobuf,		"tx failed for lack of sk_buffer");
	STAT(tx_nonode,		"tx failed for no node");
	STAT(tx_unknownmgt,	"tx of unknown mgt frame");
	STAT(tx_badcipher,	"tx failed 'cuz bad ciper/key type");
	STAT(tx_ffokcnt,	"tx atheros fast frames successful");
	STAT(tx_fferrcnt,	"tx atheros fast frames failed");
	STAT(scan_active,	"active scans started");
	STAT(scan_passive,	"passive scans started");
	STAT(node_timeout,	"nodes timed out inactivity");
	STAT(crypto_nomem,	"cipher context malloc failed");
	STAT(crypto_tkip,	"tkip crypto done in s/w");
	STAT(crypto_tkipenmic,	"tkip tx MIC done in s/w");
	STAT(crypto_tkipdemic,	"tkip rx MIC done in s/w");
	STAT(crypto_tkipcm,	"tkip dropped frames 'cuz of countermeasures");
	STAT(crypto_ccmp,	"ccmp crypto done in s/w");
	STAT(crypto_wep,	"wep crypto done in s/w");
	STAT(crypto_setkey_cipher,"setkey failed 'cuz cipher rejected data");
	STAT(crypto_setkey_nokey,"setkey failed 'cuz no key index");
	STAT(crypto_delkey,	"driver key delete failed");
	STAT(crypto_badcipher,	"setkey failed 'cuz unknown cipher");
	STAT(crypto_nocipher,	"setkey failed 'cuz cipher module unavailable");
	STAT(crypto_attachfail,	"setkey failed 'cuz cipher attach failed");
	STAT(crypto_swfallback,	"crypto fell back to s/w implementation");
	STAT(crypto_keyfail,	"setkey faied 'cuz driver key alloc failed");
/*lyb add for management frame stats*/
	STAT(rx_mgmt,	"rx management");
	STAT(tx_mgmt,	"tx management");
/*lyb add end*/
#undef STAT
#undef N
/*lyb add for frame stats*/
	printf("auth_invaild: %d\n", stats->is_rx_auth_unsupported + stats->is_rx_bad_auth);
	printf("auth_timeout: %d\n", stats->is_node_timeout_auth);
	printf("auth_deny: %d\n", stats->is_rx_acl + stats->is_rx_auth_fail);
	printf("auth_others: %d\n",  stats->is_rx_auth_countermeasures);
	printf("assoc_invaild: %d\n", stats->is_rx_bad_assoc);
	printf("assoc_timeout: %d\n", stats->is_node_timeout_assoc);
	printf("assoc_deny: %d\n", stats->is_rx_assoc_bss + stats->is_rx_assoc_notauth + stats->is_rx_assoc_capmismatch + stats->is_rx_assoc_norate 
					+ stats->is_rx_assoc_badwpaie );
	printf("assoc_others: 0\n");
	printf("reassoc_times: %d\n", stats->is_rx_reassoc);
	printf("reassoc_success: %d\n", stats->is_rx_reassoc_success);
	printf("reassoc_invaild: %d\n", stats->is_rx_bad_reassoc);
	printf("reassoc_timeout: %d\n", stats->is_node_timeout_assoc);
	printf("reassoc_deny: %d\n", stats->is_rx_reassoc_bss + stats->is_rx_reassoc_notauth + stats->is_rx_reassoc_capmismatch 
					+ stats->is_rx_reassoc_norate + stats->is_rx_reassoc_badwpaie + stats->is_rx_reassoc_badscie);
	printf("reassoc_others: 0\n");
	printf("deauth_all: %d\n", stats->is_rx_deauth + stats->is_deauth_unnormal + stats->is_deauth_expire + stats->is_deauth_state + stats->is_deauth_ioctl);
	printf("deauth_user_leave: %d\n", stats->is_rx_deauth);
	printf("deauth_ap_ability: 0\n");
	printf("deauth_unnormal: %d\n", stats->is_deauth_unnormal + stats->is_deauth_expire);
	printf("deauth_others: %d\n", stats->is_deauth_state + stats->is_deauth_ioctl);
	printf("disassoc_all: %d\n", stats->is_rx_disassoc + stats->is_disassoc_unnormal  
				+ stats->is_disassoc_state + stats->is_disassoc_ioctl);
	printf("disassoc_user_leave: %d\n", stats->is_rx_disassoc);
	printf("disassoc_ap_ability: 0\n");
	printf("disassoc_unnormal: %d\n", stats->is_disassoc_unnormal );
	printf("disassoc_others: %d\n", stats->is_disassoc_state + stats->is_disassoc_ioctl);
	printf("identify_times: %d\n", stats->is_assoc_success);
	printf("identify_sucessful:%d\n", stats->is_assoc_success - (stats->is_rx_deauth + stats->is_deauth_unnormal + stats->is_deauth_expire + stats->is_deauth_state + stats->is_deauth_ioctl));
	printf("identify_password_deny: %d\n", stats->is_rx_auth_fail);
	printf("identify_deny: %d\n", stats->is_rx_acl);
	printf("identify_timeout: %d\n", stats->is_node_timeout_auth);
	printf("identify_invalid: %d\n", stats->is_rx_auth_unsupported + stats->is_rx_bad_auth);
	printf("identify_others: 0\n");
	printf("node_timeout: %d\n", stats->is_node_timeout);
	return CW_TRUE;
/*lyb add end*/
}
CWBool get_sta_throughput_info(staThroughputsendtoAc *staThroughputsendtoAcinfo)
{
	int s;
	struct iwreq iwr;
	struct ieee80211req_sta_stats stats;
	const struct ieee80211_nodestats *ns = &stats.is_stats;
	char ifname[IFNAMSIZ];
	CWWTPSta *ptr = NULL;
	CWWTPSta *oldptr = NULL;
	unsigned char radioId = 0, wlanId = 0;
	int flags = 0;
	
	s = socket(AF_INET, SOCK_DGRAM, 0);
	if (s < 0)
	{
		CWWTPDebugLog("create sock to get_stations error!");
		close(s);
		return CW_FALSE;
	}

	for(ptr = wtp_sta_list;ptr != NULL; oldptr = ptr,ptr = ptr->next)
	{
		if((macAddrCmp(ptr->sta_mac,staThroughputsendtoAcinfo->sta_mac) == 1))
		{
				radioId = ptr->radio_id;
				wlanId = ptr->wlan_id;
				flags = 1;
				break;
		}
	}
	if(flags == 0)
	{
		close(s);
		return CW_FALSE;
	}
	
	(void) memset(&iwr, 0, sizeof(iwr));
	memset(ifname,0,sizeof(ifname));
	CWWTPGetWlanName(ifname, radioId, wlanId);
	(void) strncpy(iwr.ifr_name, ifname,IFNAMSIZ);
	iwr.u.data.pointer = (void *) &stats;
	iwr.u.data.length = sizeof(stats);

	memcpy(stats.is_u.macaddr, staThroughputsendtoAcinfo->sta_mac, IEEE80211_ADDR_LEN);
	if (ioctl(s, IEEE80211_IOCTL_STA_STATS, &iwr) < 0){
		CWWTPDebugLog("ioctl to get_sta_stats error!");
		close(s);
		return CW_FALSE;
	}

	staThroughputsendtoAcinfo->bytes_recv= ns->ns_rx_bytes;
	staThroughputsendtoAcinfo->bytes_send= ns->ns_tx_bytes;
	staThroughputsendtoAcinfo->frames_recv= ns->ns_rx_data;
	staThroughputsendtoAcinfo->frames_send= ns->ns_tx_data;
#if 0
	if(debug_print)
		printf("bytes_recv:%u,  bytes_send:%u,  frames_recv:%llu,  frame_send:%llu\n",
			staThroughputsendtoAcinfo->frames_recv,staThroughputsendtoAcinfo->frames_send,
			staThroughputsendtoAcinfo->bytes_recv,staThroughputsendtoAcinfo->bytes_send);
#endif
	close(s);
	return CW_TRUE;
}

