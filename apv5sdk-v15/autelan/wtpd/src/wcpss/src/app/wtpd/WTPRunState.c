/*******************************************************************************************
 * Copyright (c) 2006-7 Laboratorio di Sistemi di Elaborazione e Bioingegneria Informatica *
 *                      Universita' Campus BioMedico - Italy                               *
 *                                                                                         *
 * This program is free software; you can redistribute it and/or modify it under the terms *
 * of the GNU General Public License as published by the Free Software Foundation; either  *
 * version 2 of the License, or (at your option) any later version.                        *
 *                                                                                         *
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY         *
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A 	   *
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
 * Author :  Ludovico Rossi (ludo@bluepixysw.com)                                         *
 *           Del Moro Andrea (andrea_delmoro@libero.it)                                    *
 *           Giovannini Federica (giovannini.federica@gmail.com)                           *
 *           Massimo Vellucci (m.vellucci@unicampus.it)                                    *
 *           Mauro Bisson (mauro.bis@gmail.com)                                            *
 *******************************************************************************************/


#include <sys/socket.h>
#include <sys/types.h>  
#include <linux/if_ether.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <netpacket/packet.h>
#include <dirent.h>
#include "CWWTP.h"
#include "WTPDriverInteraction.h"
#ifdef DMALLOC
#include "../dmalloc-5.5.0/dmalloc.h"
#endif
extern int getifstats(const char *ifname, unsigned long *iframes, unsigned long *oframes);
extern CWBool CWGetAPOnlineTime(unsigned short *OnTime);  //pei add 0213
CWBool CWWTPManageGenericRunMessage(CWProtocolMessage *msgPtr);
void CWWTPDioTest(void);

CWBool CWWTPCheckForBindingFrame();

CWBool CWWTPCheckForWTPEventRequest();
CWBool CWParseWTPEventResponseMessage (char *msg, int len, int seqNum, void *values);
CWBool CWSaveWTPEventResponseMessage (void *WTPEventResp);

CWBool CWAssembleEchoRequest (CWProtocolMessage **messagesPtr, int *fragmentsNumPtr, int PMTU, int seqNum, CWList msgElemList);

CWBool CWParseConfigurationUpdateRequest (char *msg, int len, CWProtocolConfigurationUpdateRequestValues *valuesPtr,unsigned char  *protected_flag) ;

CWBool CWParseSTAConfigurationRequest(char *msg, int len, CWProtocolConfigurationUpdateRequestValues *valuesPtr);

CWBool CWParseIEEE80211WlanConfigurationRequest (char *msg, int len, CWProtocolConfigurationUpdateRequestValues *valuesPtr);

CWBool CWSaveConfigurationUpdateRequest(CWProtocolConfigurationUpdateRequestValues *valuesPtr, CWProtocolResultCode* resultCode, unsigned char *operat_state_cause,unsigned char radioID,unsigned char protected_flag);  //modified by pei 0724

CWBool CWSaveIEEE80211WlanConfigurationRequest(CWProtocolConfigurationUpdateRequestValues *valuesPtr, unsigned char *created_radio_id, unsigned char *created_wlan_id, CWProtocolResultCode* resultCode);

CWBool CWSaveSTAConfigurationRequest(CWProtocolConfigurationUpdateRequestValues *valuesPtr, CWProtocolResultCode* resultCode);

CWBool CWAssembleConfigurationUpdateResponse(CWProtocolMessage **messagesPtr, int *fragmentsNumPtr, int PMTU, int seqNum, CWProtocolResultCode resultCode, unsigned char radio_id, unsigned char operat_state_cause);  //modified by pei  0724

CWBool CWAssembleIEEE80211WlanConfigurationResponse(CWProtocolMessage **messagesPtr,CWProtocolConfigurationUpdateRequestValues *valuesPtr,  int *fragmentsNumPtr, int PMTU, int seqNum, unsigned char created_radio_id, unsigned char created_wlan_id,CWProtocolResultCode resultCode);

CWBool CWAssembleSTAConfigurationResponse(CWProtocolMessage **messagesPtr, int *fragmentsNumPtr, int PMTU, int seqNum, CWProtocolResultCode resultCode);
CWBool CWGetInterfaceCount(int *count, char *type);
void CWWTPNetLink(void);      //pei add 0703
CWTimerID gCWHeartBeatTimerID;
CWTimerID gCWNeighborDeadTimerID;
CWBool gNeighborDeadTimerSet=CW_FALSE;
	
int gEchoInterval = CW_ECHO_INTERVAL_DEFAULT;
int gNetLink_signal = 0;           //pei add 0703
unsigned char g_DelStaMac[6];   //pei add 0708
unsigned short gPowerOnUntilRuntime = 0;
char *gWtpdConfDir = "/tmp/conf_wtpd";
unsigned char gRunRetry=0;
wlan_stats_info gwlanStatsInfo[NUM_OF_ATH_ALL_WIFI];
wlan_stats_info gethStatsInfo[NUM_OF_ETH];
CWThread thread_wtp_throughput_info;
CWThread thread_apScanning;
/* Begin: gengzj added for wifipos 2013-11-26 */
CWThread thread_scanpos_res;
/* End: gengzj added end */
CWThread thread_apMonitor;
CWThread thread_extraInfo;
CWThread thread_staInfo;
CWThread thread_ifState;
CWThread thread_snooping;
CWThread thread_sta_pkts_statistic_report;
CWThread thread_ap_pkts_statistic_report;//caidh add 2013.02.16 for ap_pkts_statistic thread
/* Begin: gengzj added for wifipos 2013-11-26 */
CWSocket gScanPosSocket;
/* End: gengzj added end */
// Manage DTLS packets
CW_THREAD_RETURN_TYPE CWWTPReceiveDtlsPacket(void *arg)
{
	if(pthread_detach(pthread_self())!=0)
	autelan_printf("##########detach error!############\n");
	int readBytes;
	char buf[CW_BUFFER_SIZE];
	CWSocket sockDTLS = gWTPSocket;/*sockDTLS is gWTPSocket*/
	CWNetworkLev4Address addr;
	char* pData;
	CWBool recvData = CW_FALSE;

//	CWThreadSetSignals(SIG_BLOCK, 1, SIGALRM);

	CW_REPEAT_FOREVER 
	{
		bzero(buf, CW_BUFFER_SIZE);
		recvData = CW_FALSE;
		// message
		if(!CWErr(CWNetworkReceiveUnsafe(sockDTLS, buf, CW_BUFFER_SIZE - 1, 0, &addr, &readBytes))) 
		{
			if (CWErrorGetLastErrorCode() == CW_ERROR_INTERRUPTED)
				continue;
			CWLog("Receiving packets from AC error");
			 autelan_sleep(1);
			 continue;
		}
		
		// Clone data packet  clone a copy to add to the list		
		CW_CREATE_OBJECT_SIZE_ERR(pData, readBytes, { CWWTPDebugLog("Out Of Memory"); return NULL; });
		memcpy(pData, buf, readBytes);/*memcpy expend CPU*/

		CWLockSafeList(gPacketReceiveList);
		recvData = CWAddElementToSafeListTail(gPacketReceiveList, pData, readBytes);
		CWUnlockSafeList(gPacketReceiveList);
		if(recvData)
			CWSignalElementSafeList(gPacketReceiveList); /* pei add for fix bug XJ */
	}

	return NULL;
}
/***************************************************************************
 * 
 * Function:  CWWTPReceiveDtlsDataPacket
 *
 * Purpose:   receive DATA frome AC (use data channel)
 *
 * Inputs: sock pointer
 *
 * Output:    
 *
 * Returns:   
 **************************************************************************/

CW_THREAD_RETURN_TYPE CWWTPReceiveDtlsDataPacket(void *arg)
{
	if(pthread_detach(pthread_self())!=0)
	autelan_printf("##########detach error!############\n");
#define DST_MAC_HEN 6
#define SRC_MAC_HEN 6
	int readBytes;
	unsigned char buf[CW_BUFFER_SIZE];
	unsigned char buf_no_vlan[CW_BUFFER_SIZE];
	unsigned char buf_eap[CW_BUFFER_SIZE];
	unsigned char *sendmsg;
	int sendBytes;
	int n;
	int wlanfound;
	unsigned char  wlan_id;
	unsigned char header_len;
	int wlan_sock,eap_sock;
	CWWTPWlan *ptr = NULL;
	int wifi_count = CWGetRadioMaxCount();
	CW_REPEAT_FOREVER 
	{
		wlan_sock = 0;
		wlanfound = 0;
		if(WTPDataChannelThreadRun == CW_FALSE)
			break;

		bzero(buf,sizeof(buf));
		bzero(buf_eap,sizeof(buf_eap));

		CWNetworkLev4Address  DataChannelAddr;
		if(!CWErr(CWNetworkReceiveUnsafe(gWTPDataSocket, (char *)buf, CW_BUFFER_SIZE - 1, 0, &DataChannelAddr, &readBytes))) { 
			if (CWErrorGetLastErrorCode() == CW_ERROR_INTERRUPTED)
				continue;
			CWWTPDebugLog("data channel error \n");
			continue;
		}
		
		int val;
		int type = 1,T=1;
		unsigned char  rid;
		val = *(unsigned int *)buf;
		if(CWGetField32(val,CW_TRANSPORT_HEADER_K_START,CW_TRANSPORT_HEADER_K_LEN) == 1)
			continue;
		type = CWGetField32(val,CW_TRANSPORT_HEADER_WBID_START,CW_TRANSPORT_HEADER_WBID_LEN);
		T = CWGetField32(val,CW_TRANSPORT_HEADER_T_START,CW_TRANSPORT_HEADER_T_LEN);
		rid = CWGetField32(val,CW_TRANSPORT_HEADER_RID_START,CW_TRANSPORT_HEADER_RID_LEN);
		if(debug_print)
			autelan_printf("type=wbid=%d rid=%d,T=%d,%s firstbytes=%d\n",type,rid,T,(type==1?"802.11":"802.3"),val);
		//if(rid < 0 ||rid > wifi_count){
		if(rid > wifi_count){//zengmin add by Coverity 2013-06-09
			if(debug_print)
				autelan_printf("Rid(%d) Invalid.Discard It!\n",rid);
			CWWTPDebugLog("Rid(%d) Invalid.Discard It!\n",rid);
			continue;
		}
		
		int transport4BytesLen; 
		transport4BytesLen = CWGetField32(val,	CW_TRANSPORT_HEADER_HLEN_START, CW_TRANSPORT_HEADER_HLEN_LEN);
		header_len = transport4BytesLen*4;
		if(debug_print)
			autelan_printf("capwap len:%d (4-bytes word) == %d (bytes)\n",transport4BytesLen,header_len);
		header_len= 16;
		unsigned short wlan_id_bitmap;
		wlan_id_bitmap = *(unsigned char *)(buf+8);
		
#if 0
		CWCaptrue(readBytes,buf);
#endif 
		if(wlan_id_bitmap == 0)
			continue;
//				wlan_id_bitmap = 1;//it's wrong,maybe the package is given to ap,not to ath.x-x
		if(debug_print)
			autelan_printf("Get WlanID_BitMap:%hd.\n",wlan_id_bitmap);
		wlan_id = wlan_id_bitmap;

		if(T!=0 && type == 1)//802.11
		{
			for(ptr = wtp_wlan_list;ptr != NULL;ptr = ptr->next){
				if(ptr->radio_id == rid && ptr->wlan_id == wlan_id){
					wlan_sock = ptr->wlan_sendsock;
					eap_sock = ptr->wlan_eapsock;
					wlanfound = 1;
					if(debug_print)
						autelan_printf("11_get wlan_id:%d ptr->wlan_sendsock:%d for sending data to sta\n", wlan_id,ptr->wlan_sendsock);
					break;
				}				
			}
			sendmsg = (unsigned char *)(buf+header_len);
			sendBytes = readBytes-header_len;
			#if 0
			IEEE80211_Header *h;
			h = (struct IEEE80211_Header *)(buf+header_len);
			if(debug_print)
				printf("get bssid from 802.11 header:%02x:%02x:%02x:%02x:%02x:%02x\n", h->mac2[0], h->mac2[1], h->mac2[2], h->mac2[3], h->mac2[4], h->mac2[5]);
			for(ptr = wtp_wlan_list;ptr != NULL;ptr = ptr->next){
				if(memcmp(ptr->wlan_bssid, h->mac2, MAC_ADDR_LEN) == 0){
					printf("bssid:%02x:%02x:%02x:%02x:%02x:%02x\n", ptr->wlan_bssid[0], ptr->wlan_bssid[1], 
						ptr->wlan_bssid[2], ptr->wlan_bssid[3], ptr->wlan_bssid[4], ptr->wlan_bssid[5]);
					wlan_id = ptr->wlan_id;
					wlan_sock = ptr->wlan_sendsock;
					eap_sock = ptr->wlan_eapsock;
					wlanfound = 1;
					printf("11_get wlan_id:%d ptr->wlan_sendsock:%d for sending data to sta\n", wlan_id,ptr->wlan_sendsock);
					break;
				}				
			}
			#endif
		}
		else if(T == 0)//802.3
		{
			for(ptr = wtp_wlan_list;ptr != NULL;ptr = ptr->next){
				if(ptr->radio_id == rid && ptr->wlan_id == wlan_id){
					wlan_sock = ptr->wlan_dot3sock;
					eap_sock = ptr->wlan_eapsock;
					wlanfound = 1;
					if(debug_print)
						autelan_printf("2.3_get wlan_id:%d ptr->wlan_dot3sock:%d for sending data to sta\n", wlan_id,ptr->wlan_dot3sock);
					break;
				}				
			}
			int offset = header_len+DST_MAC_HEN +SRC_MAC_HEN;
			if(buf[offset] == 0x81 && buf[offset+1] == 0x00)
			{
				memcpy(buf_no_vlan,(unsigned char *)(buf+header_len),12);
				memcpy(buf_no_vlan+12,(unsigned char *)(buf+header_len+12+4),readBytes -header_len-12-4);
				sendmsg = (unsigned char *)(buf_no_vlan);
				if(debug_print)
					autelan_printf("Found Vlan ID! Discard it!\n");
				sendBytes = readBytes-header_len-4;
			}
			else
			{
				sendmsg = (unsigned char *)(buf+header_len);
				sendBytes = readBytes-header_len;
			}
			
#if 0	
//			wlan_sock = sock_for_dot_3;
//			wlanfound = 1;
			unsigned char mac[MAC_ADDR_LEN];
			int i;
			printf("Paser MAC:");
			for(i=0;i<MAC_ADDR_LEN;i++)
			{
				mac[i] = *(unsigned char *)(buf+9+i);
				printf("%02x",mac[i]);
			}
			printf("\n");
			//unsigned char mac[MAC_ADDR_LEN];//={0x00,0x1F,0x64,0xE2,0xDD,0x80};
			if(debug_print)
				printf("get bssid from capwap header:%02x:%02x:%02x:%02x:%02x:%02x\n", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
			for(ptr = wtp_wlan_list;ptr != NULL;ptr = ptr->next){
				if(memcmp(ptr->wlan_bssid, mac, MAC_ADDR_LEN) == 0){
					printf("bssid:%02x:%02x:%02x:%02x:%02x:%02x\n", ptr->wlan_bssid[0], ptr->wlan_bssid[1], 
						ptr->wlan_bssid[2], ptr->wlan_bssid[3], ptr->wlan_bssid[4], ptr->wlan_bssid[5]);
					wlan_id = ptr->wlan_id;
					wlan_sock = ptr->wlan_dot3sock;//ptr->wlan_sendsock;
					eap_sock = ptr->wlan_eapsock;
					wlanfound = 1;
					printf("2.3_get wlan_id:%d ptr->wlan_dot3sock:%d for sending data to sta\n", wlan_id,ptr->wlan_dot3sock);
					break;
				}				
			}
#endif
		}
		else
			continue;
		
		if(debug_print)
			autelan_printf("-----------wlan_id:%d\n", wlan_id);
		
		if(wlanfound == 0){
			if(debug_print)
				autelan_printf("Can't find the send wlan \n");
			continue;
		}
		

//		sendmsg=(unsigned char *) (buf + header_len);
//		ether_type = *(unsigned short int *)(buf + header_len + 30);//802.11 header(24 byte) + llc(6 byte) 
//		printf("\n eth_type  %04x\n",ether_type);
#if 0
		if(ether_type == 0x888e){/*EAP */
			ieee80211_header = (IEEE80211_Header *)(buf + header_len);
			eth_header = (ETH_Header *)buf_eap;
			memcpy(eth_header->dmac,ieee80211_header->mac1,MAC_ADDR_LEN);
			memcpy(eth_header->smac,ieee80211_header->mac2,MAC_ADDR_LEN);
			memcpy(buf_eap+12,buf+header_len+30,readBytes-30-header_len);
			printf("**************************802.3*********************************\n");
			for(i=0;i<readBytes-30;i++){
					printf("%02x ",buf_eap[i]);
				if((i+1)%10 == 0)
					printf("\n");
			}
			n = sendto(eap_sock,(unsigned char *)(buf_eap),readBytes-30,0x0,NULL,NULL);
			printf("send eapol\n");
		}
		else{
#endif
#if 1
		if(debug_print)
			autelan_printf("wlan_sock:%d\n",wlan_sock);
		n = autelan_sendto(wlan_sock, sendmsg,sendBytes, 0x0, NULL, 0); 
#else
		n = sendto(wlan_sock,(unsigned char *)(buf+header_len),(readBytes-header_len),0x0,NULL,NULL);
#endif
//			printf("send assoc response\n");
		if(n<0)
			dpf("data channel send error\n");
	}

	
	return NULL;
}
#ifndef CW_NO_DTLS
// Manage Data DTLS packets
CW_THREAD_RETURN_TYPE CWWTPReceiveRealDtlsDataPacket(void *arg)
{
	if(pthread_detach(pthread_self())!=0)
	autelan_printf("##########detach error!############\n");
	int readBytes;
	char buf[CW_BUFFER_SIZE];
	CWSocket sockDTLS = gWTPDataSocket;
	CWNetworkLev4Address addr;
	char* pData;

//	CWThreadSetSignals(SIG_BLOCK, 1, SIGALRM);

	CW_REPEAT_FOREVER 
	{
		// message
		if(!CWErr(CWNetworkReceiveUnsafe(sockDTLS, buf, CW_BUFFER_SIZE - 1, 0, &addr, &readBytes))) 
		{
			if (CWErrorGetLastErrorCode() == CW_ERROR_INTERRUPTED)
				continue;
			 autelan_sleep(1);
		     CWLog("Receiving Dtls packets from AC error,continue");
		     continue;
		}
		
		// Clone data packet  clone a copy to add to the list		
		CW_CREATE_OBJECT_SIZE_ERR(pData, readBytes, { CWWTPDebugLog("Out Of Memory"); return NULL; });
		memcpy(pData, buf, readBytes);/*memcpy expend CPU*/

		CWLockSafeList(gPacketDataReceiveList);
		CWAddElementToSafeListTail(gPacketDataReceiveList, pData, readBytes);
		CWUnlockSafeList(gPacketDataReceiveList);
		CWSignalElementSafeList(gPacketDataReceiveList); /* pei add for fix bug XJ */
	}

	return NULL;
}
#endif

void CWWTPNetLink(void)            //pei add 0703
{
	int readBytes;
	unsigned char* pData;
#if 1
	unsigned char buf[CW_BUFFER_SIZE] = /*test netlink*/
	{
		0x01,0x10,0x03,0x00,
		0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x09,       /*control message type  */
		0x04,		/*seq num*/
		0x00,0x0f,	/*msg element length*/
		0x00,		/*flags*/
		0x00,0x12,	/*message element type*/
		0x00,0x08,	/*message element length*/
		0x00,		/*radio ID*/
		0x06,		/*length*/
		0x00,0x0a,0x64,0xe0,0x00,0x88/*station mac*/
	};

	readBytes = 28;
#else
	CWProtocolMessage *messages = NULL;
	int fragmentsNum = 0;
	int seqNum;
	seqNum = CWGetSeqNum();
	if(!CWAssembleWTPEventDeleteStationRequest(&messages, &fragmentsNum, gWTPPathMTU, seqNum, NULL)) 
		printf("error:assemble wtp event delete station request error!\n");
printf("fragmentsNum = %d\n", fragmentsNum);
	readBytes = 20;
printf("readBytes = %d\n", readBytes);
#endif
	CW_CREATE_OBJECT_SIZE_ERR(pData, readBytes, { CWWTPDebugLog("Out Of Memory"); return; });
	memcpy(pData, buf, readBytes);           //memcpy expend CPU
/*
	if(debug_print)
	{
		int i=0;          //pei add 0703
		for(i = 0;i<readBytes;i++){
			printf("pData = %02x\n", pData[i]);
		}
	}
*/
	CWLockSafeList(gPacketReceiveList);
	CWAddElementToSafeListTail(gPacketReceiveList, pData, readBytes);
	CWUnlockSafeList(gPacketReceiveList);
	CW_FREE_OBJECT(pData);

	return;
}

//pei add 0213
CWBool CWAssembleWTPEventApWarningRequest (CWProtocolMessage **messagesPtr, int *fragmentsNumPtr, int PMTU, int seqNum)
{
	CWProtocolMessage *msgElems= NULL;
	const int msgElemCount = 1;
	CWProtocolMessage *msgElemsBinding= NULL;
	const int msgElemBindingCount=0;
	
	if(messagesPtr == NULL || fragmentsNumPtr == NULL) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);

	CW_CREATE_PROTOCOL_MSG_ARRAY_ERR(msgElems, msgElemCount, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););	
	

	// Assemble Message Elements
	if (!(CWAssembleMsgElemWTPOperationalStatistics(msgElems, 0))) {
		CW_FREE_OBJECT(msgElems);
		return CW_FALSE;
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
	//CWWTPDebugLog("Assemble wtp event ap warning success!");
	dpf("Assemble wtp event ap warning success!\n");

	return CW_TRUE;
}

void CWWTPSendWTPWarning(void)
{
	int i=0;

	CWProtocolMessage *messages = NULL;
	int fragmentsNum=0;
	int seqNum;
	seqNum = CWGetSeqNum();

	if(!CWAssembleWTPEventApWarningRequest(&messages, &fragmentsNum, gWTPPathMTU, seqNum))
	{
		CWWTPDebugLog("Assemble WTP Event Ap Scanning Request error!\n");
		return;
	}

	for(i = 0; i < fragmentsNum; i++) 
	{
		//if(debug_print)
		//{
		//	printf("messages[%d].offset:%d\n", i, messages[i].offset);
		//	CWCaptrue(messages[i].offset, (unsigned char *)(messages[i].msg));
		//}

#ifndef CW_NO_DTLS
		if(gDtlsSecurity == 1)
		{
			if(!CWSecuritySend(gWTPSession, messages[i].msg, messages[i].offset))
			{
				CWWTPDebugLog("Error sending message\n");
				CWFreeMessageFragments(messages, fragmentsNum);
				CW_FREE_OBJECT(messages);
				return;
			}
		}
		else
#endif
		{
			if (!CWNetworkSendUnsafeUnconnected(gWTPSocket, &(gACInfoPtr->preferredAddress), messages[i].msg, messages[i].offset))
			{
				CWWTPDebugLog("Error sending message\n");
				CWFreeMessageFragments(messages, fragmentsNum);
				CW_FREE_OBJECT(messages);
				return;
			}
		}
	}
	//CWWTPDebugLog("send wtp event ap warning success!");
	dpf("send wtp event ap warning success!\n");
	CWFreeMessageFragments(messages, fragmentsNum);
	CW_FREE_OBJECT(messages);

	return;
}
CWBool CWSaveConfigInfo_old()
{
	int i = 0;
	char cmdbuf[128];
	char str[128];
	int radioCount = 1;
	int radio_wlan_count[2] = {0, 0};
	CWWTPWlan *ptr = NULL;
	CWWTPWlan *oldptr = NULL;
	
	memset(cmdbuf, 0, 128);

	/* save parameters for backup-wtpd */
	autelan_sprintf(cmdbuf, "mkdir -p %s", gWtpdConfDir);
	_system(cmdbuf);

	/* AC address */
//	struct sockaddr *sa=(struct sockaddr*) (&gACInfoPtr->preferredAddress);
//	struct sockaddr_in	*sin = (struct sockaddr_in *) sa;
//	inet_ntop(AF_INET, &sin->sin_addr, str, 128);

	/* AC address */
	struct sockaddr *sa = (struct sockaddr*) (&gACInfoPtr->preferredAddress);
	if(gNetworkPreferredFamily == CW_IPv4)
	{
		struct sockaddr_in	*sin = (struct sockaddr_in *)sa;
		autelan_inet_ntop(AF_INET, &sin->sin_addr, str, sizeof(str));
	}
	else
	{
		struct sockaddr_in6 *sin6 = (struct sockaddr_in6 *)sa;
		autelan_inet_ntop(AF_INET6, &sin6->sin6_addr, str, sizeof(str));
	}

	autelan_sprintf(cmdbuf, "echo %s > %s/ac_addr", str, gWtpdConfDir);
	_system(cmdbuf);

	radioCount = CWGetRadioMaxCount();
	if(radioCount == 0)
		radioCount = 2;
		
	/* radio conf */
	for(i=0;i<radioCount;i++)
	{
		autelan_sprintf(cmdbuf, "mkdir -p %s/radio%d", gWtpdConfDir, i);
		autelan_system(cmdbuf);
		autelan_sprintf(cmdbuf, "echo %d > %s/radio%d/channel", gRadioInfoValue[i].channel, gWtpdConfDir, i);
		autelan_system(cmdbuf);
		autelan_sprintf(cmdbuf, "echo %d > %s/radio%d/txtype", gRadioInfoValue[i].txtype, gWtpdConfDir, i);
		autelan_system(cmdbuf);
		autelan_sprintf(cmdbuf, "echo %d > %s/radio%d/txpower", gRadioInfoValue[i].txpower, gWtpdConfDir, i);
		autelan_system(cmdbuf);
		autelan_sprintf(cmdbuf, "echo %d > %s/radio%d/rate", gRadioInfoValue[i].rate, gWtpdConfDir, i);
		autelan_system(cmdbuf);
		autelan_sprintf(cmdbuf, "echo %d > %s/radio%d/rateAutoEnable", gRadioInfoValue[i].rateAutoEnable, gWtpdConfDir, i);
		autelan_system(cmdbuf);
		autelan_sprintf(cmdbuf, "echo %d > %s/radio%d/radiotype", gRadioInfoValue[i].radiotype, gWtpdConfDir, i);
		autelan_system(cmdbuf);
		autelan_sprintf(cmdbuf, "echo %d > %s/radio%d/fragThreshold", gRadioInfoValue[i].fragThreshold, gWtpdConfDir, i);
		autelan_system(cmdbuf);
		autelan_sprintf(cmdbuf, "echo %d > %s/radio%d/rtsThreshold", gRadioInfoValue[i].rtsThreshold, gWtpdConfDir, i);
		autelan_system(cmdbuf);
		autelan_sprintf(cmdbuf, "echo %d > %s/radio%d/shortRetry", gRadioInfoValue[i].shortRetry, gWtpdConfDir, i);
		autelan_system(cmdbuf);
		autelan_sprintf(cmdbuf, "echo %d > %s/radio%d/longRetry", gRadioInfoValue[i].longRetry, gWtpdConfDir, i);
		autelan_system(cmdbuf);
		autelan_sprintf(cmdbuf, "echo %d > %s/radio%d/beaconInterval", gRadioInfoValue[i].beaconInterval, gWtpdConfDir, i);
		autelan_system(cmdbuf);
		autelan_sprintf(cmdbuf, "echo %d > %s/radio%d/preamble", gRadioInfoValue[i].preamble, gWtpdConfDir, i);
		autelan_system(cmdbuf);
		autelan_sprintf(cmdbuf, "echo %d > %s/radio%d/dtim", gRadioInfoValue[i].dtim, gWtpdConfDir, i);
		autelan_system(cmdbuf);
		autelan_sprintf(cmdbuf, "echo %d > %s/radio%d/gIsRadioEnable", gRadioInfoValue[i].gIsRadioEnable, gWtpdConfDir, i);
		autelan_system(cmdbuf);

		autelan_sprintf(cmdbuf, "rm -rf %s/radio%d/wlaninfo", gWtpdConfDir, i);
		autelan_system(cmdbuf);
		autelan_sprintf(cmdbuf, "mkdir -p %s/radio%d/wlaninfo", gWtpdConfDir, i);
		autelan_system(cmdbuf);
		for(ptr=wtp_wlan_list;ptr != NULL;oldptr = ptr,ptr = ptr->next)
		{
			if(ptr->radio_id == i)
			{
				radio_wlan_count[i] ++;
				autelan_sprintf(cmdbuf, "mkdir -p %s/radio%d/wlaninfo/wlan%d", gWtpdConfDir, i, radio_wlan_count[i]);
				autelan_system(cmdbuf);
				autelan_sprintf(cmdbuf, "echo %d > %s/radio%d/wlaninfo/wlan%d/radio_id",  ptr->radio_id, gWtpdConfDir, i, radio_wlan_count[i]);
				autelan_system(cmdbuf);
				autelan_sprintf(cmdbuf, "echo %d > %s/radio%d/wlaninfo/wlan%d/wlan_id", ptr->wlan_id, gWtpdConfDir, i, radio_wlan_count[i]);
				autelan_system(cmdbuf);
				autelan_sprintf(cmdbuf, "echo %d > %s/radio%d/wlaninfo/wlan%d/vlan_id", ptr->vlan_id, gWtpdConfDir, i, radio_wlan_count[i]);
				autelan_system(cmdbuf);
				autelan_sprintf(cmdbuf, "echo %d > %s/radio%d/wlaninfo/wlan%d/wlan_updown_time", ptr->wlan_updown_time, gWtpdConfDir, i, radio_wlan_count[i]);
				autelan_system(cmdbuf);
				autelan_sprintf(cmdbuf, "echo %d > %s/radio%d/wlaninfo/wlan%d/wlan_mac_mode", ptr->wlan_mac_mode, gWtpdConfDir, i, radio_wlan_count[i]);
				autelan_system(cmdbuf);
				autelan_sprintf(cmdbuf, "echo %d > %s/radio%d/wlaninfo/wlan%d/wlan_tunnel_mode", ptr->wlan_tunnel_mode, gWtpdConfDir, i, radio_wlan_count[i]);
				autelan_system(cmdbuf);
			}
		}
		autelan_sprintf(cmdbuf, "echo %d > %s/radio%d/wlan_count", radio_wlan_count[i], gWtpdConfDir, i);
		autelan_system(cmdbuf);
	}

	/* parameters whole process */
//	if(gApScanningEnable)
	{
		autelan_sprintf(cmdbuf, "echo %d > %s/gApScanningEnable", gApScanningEnable, gWtpdConfDir);
		autelan_system(cmdbuf);
		autelan_sprintf(cmdbuf, "echo %d > %s/gApScanningInterval", gApScanningInterval, gWtpdConfDir);
		autelan_system(cmdbuf);
	}
//	if(gAPThroughputInfoCollectEnable)
	{
		autelan_sprintf(cmdbuf, "echo %d > %s/gAPThroughputInfoCollectEnable", gAPThroughputInfoCollectEnable, gWtpdConfDir);
		autelan_system(cmdbuf);
		autelan_sprintf(cmdbuf, "echo %d > %s/gAPThroughputInfoInterval", gAPThroughputInfoInterval, gWtpdConfDir);
		autelan_system(cmdbuf);
	}
//	if(gApMonitorEnable)
	{
		autelan_sprintf(cmdbuf, "echo %d > %s/gApMonitorEnable", gApMonitorEnable, gWtpdConfDir);
		autelan_system(cmdbuf);
		autelan_sprintf(cmdbuf, "echo %d > %s/gApMonitorInterval", gApMonitorInterval, gWtpdConfDir);
		autelan_system(cmdbuf);
	}
//	if(gExtraInfoEnable)
	{
		autelan_sprintf(cmdbuf, "echo %d > %s/gExtraInfoEnable", gExtraInfoEnable, gWtpdConfDir);
		autelan_system(cmdbuf);
		autelan_sprintf(cmdbuf, "echo %d > %s/gExtraInfoInterval", gExtraInfoInterval, gWtpdConfDir);
		autelan_system(cmdbuf);
	}
//	if(gStaInfoEnable)
	{
		autelan_sprintf(cmdbuf, "echo %d > %s/gStaInfoEnable", gStaInfoEnable, gWtpdConfDir);
		autelan_system(cmdbuf);
		autelan_sprintf(cmdbuf, "echo %d > %s/gStaInfoInterval", gStaInfoInterval, gWtpdConfDir);
		autelan_system(cmdbuf);
	}
//	if(gIfStateEnable)
	{
		autelan_sprintf(cmdbuf, "echo %d > %s/gIfStateEnable", gIfStateEnable, gWtpdConfDir);
		autelan_system(cmdbuf);
		autelan_sprintf(cmdbuf, "echo %d > %s/gIfStateInterval", gIfStateInterval, gWtpdConfDir);
		autelan_system(cmdbuf);
	}
//	if(gFloodingDetectEnable)
	{
		autelan_sprintf(cmdbuf, "echo %d > %s/gFloodingDetectEnable", gFloodingDetectEnable, gWtpdConfDir);
		autelan_system(cmdbuf);
		autelan_sprintf(cmdbuf, "echo %d > %s/gFloodAttackCnt", gFloodAttackCnt, gWtpdConfDir);
		autelan_system(cmdbuf);
		autelan_sprintf(cmdbuf, "echo %d > %s/gFloodProbeCnt", gFloodProbeCnt, gWtpdConfDir);
		autelan_system(cmdbuf);
		autelan_sprintf(cmdbuf, "echo %d > %s/gFloodWidsIntval", gFloodWidsIntval, gWtpdConfDir);
		autelan_system(cmdbuf);
	}
//	if(gSpoofingDetectEnable)
	{
		autelan_sprintf(cmdbuf, "echo %d > %s/gSpoofingDetectEnable", gSpoofingDetectEnable, gWtpdConfDir);
		autelan_system(cmdbuf);
	}
//	if(gWeakIVDetectEnable)
	{
		autelan_sprintf(cmdbuf, "echo %d > %s/gWeakIVDetectEnable", gWeakIVDetectEnable, gWtpdConfDir);
		autelan_system(cmdbuf);
	}
	autelan_sprintf(cmdbuf, "echo %d > %s/gEchoInterval", gEchoInterval, gWtpdConfDir);
	autelan_system(cmdbuf);
	autelan_sprintf(cmdbuf, "echo %d > %s/gCWNeighborDeadInterval", gCWNeighborDeadInterval, gWtpdConfDir);
	autelan_system(cmdbuf);
//	sprintf(cmdbuf, "echo %d > %s/wtp_wlan_count", wtp_wlan_count, gWtpdConfDir);
//	_system(cmdbuf);

	return CW_TRUE;
}
void CWGetWlanStations(unsigned char radioId, unsigned char wlanId)
{
	unsigned char buf[24*1024];
	struct iwreq iwr;
	unsigned char *cp;
	int s, len;
	char ifname[IFNAMSIZ];

	s = socket(AF_INET, SOCK_DGRAM, 0);
	if (s < 0)
	{
		CWWTPDebugLog("create sock to get_stations error!");
		return;
	}

	CWWTPGetWlanName(ifname, radioId, wlanId);
	(void) memset(&iwr, 0, sizeof(iwr));
	(void) strncpy(iwr.ifr_name, ifname, sizeof(iwr.ifr_name));
	iwr.u.data.pointer = (void *) buf;
	iwr.u.data.length = sizeof(buf);
	if (ioctl(s, IEEE80211_IOCTL_STA_INFO, &iwr) < 0)
	{
		CWWTPDebugLog("ioctl to get_stations error!");
		autelan_close(s);
		return;
	}
	len = iwr.u.data.length;
	if (len < sizeof(struct ieee80211req_sta_info))
	{
		CWWTPDebugLog("get sta info for get_stations error!");
		autelan_close(s);
		return;
	}

	cp = buf;
	do {
		struct ieee80211req_sta_info *si;
		u_int8_t *vp;
		AddSTAValues addStaValues;
		
		si = (struct ieee80211req_sta_info *)cp;
		vp = (u_int8_t *)(si+1);
		if(si->isi_state & IEEE80211_NODE_AUTH )
		{
			memset(&addStaValues, 0, sizeof(addStaValues));
			addStaValues.radio_id = radioId;
			addStaValues.wlan_id = wlanId;
			addStaValues.mac_length = MAC_ADDR_LEN;
			CW_CREATE_OBJECT_SIZE_ERR(addStaValues.mac_addr, addStaValues.mac_length, {autelan_close(s); return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL);});
			memcpy(addStaValues.mac_addr, si->isi_macaddr, addStaValues.mac_length);
			CWWTPDebugLog("add sta to table because config backup");
			STATableAdd(&addStaValues);
			CW_FREE_OBJECT(addStaValues.mac_addr);
		}
		cp += si->isi_len;
		len -= si->isi_len;
	} while (len >= sizeof(struct ieee80211req_sta_info));

	autelan_close(s);
	return;
}
CWBool CWBackupWlanTableAdd(CWWTPWlan *wlanBackup)
{
	CWWTPWlan *wtp_wlan_node;
	CWWTPWlan *ptr = NULL;
	CWWTPWlan *oldptr = NULL;
	
	for(ptr = wtp_wlan_list;ptr != NULL; oldptr = ptr ,ptr = ptr->next)
	{
		if((wlanBackup->radio_id == ptr->radio_id)&&(wlanBackup->wlan_id == ptr->wlan_id))
		{
			CWWTPDebugLog("wlan is already in table");
			return CW_TRUE;
		}
	}
	CW_CREATE_OBJECT_ERR(wtp_wlan_node,CWWTPWlan,return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	wtp_wlan_node->radio_id = wlanBackup->radio_id;
	wtp_wlan_node->wlan_id = wlanBackup->wlan_id;
	wtp_wlan_node->vlan_id = wlanBackup->vlan_id;
	wtp_wlan_node->wlan_updown_time = wlanBackup->wlan_updown_time;
	wtp_wlan_node->wlan_mac_mode = wlanBackup->wlan_mac_mode;
	wtp_wlan_node->wlan_tunnel_mode = wlanBackup->wlan_tunnel_mode;

	/*forward insert*/
	wtp_wlan_node->next = wtp_wlan_list;
	wtp_wlan_list = wtp_wlan_node;
	wtp_wlan_count++;
	
	return CW_TRUE;
}
CWBool CWInitSockForBackup()
{
//	int i = 0;
	int WTPSockPort = 32768;
	int WTPDataSockPort = 32769;
	CWThread thread_receiveControl;
	CWThread thread_receiveData;
	struct sockaddr_in temptaddr;
	CWWTPWlan *ptr = NULL;
	CWWTPWlan *oldptr = NULL;
	CWNetworkLev4Address preferredAddress;
	memset(&preferredAddress,0,sizeof(CWNetworkLev4Address)); //  zengmin initializer preferredAddress by Coverity 2013-06-09

	CWGetHostIP(gHostip);
	CW_COPY_NET_ADDR_PTR(&temptaddr , &preferredAddress);
	if(!CWErr(CWNetworkInitSocketClientUnconnect(&gWTPSocket, &temptaddr, WTPSockPort, gHostip)))
	{
		CWNetworkCloseSocket(gWTPSocket);
		CWLog("%s,init control sock failed when hot start.",strerror(errno));
		return CW_FALSE;
	}
	if(!CWErr(CWCreateThread(&thread_receiveControl, CWWTPReceiveDtlsPacket, (void*)gWTPSocket,0)))
	{
		CWNetworkCloseSocket(gWTPSocket);
		CWLog("Error starting Thread that receive DTLS packet when hot start.");
		return CW_FALSE;
	}

	
	if(!CWErr(CWNetworkInitSocketClientUnconnect(&gWTPDataSocket, &temptaddr, WTPDataSockPort, gHostip)))
	{
		CWNetworkCloseSocket(gWTPSocket);
		CWNetworkCloseSocket(gWTPDataSocket);
		CWLog("%s,init data sock failed when hot start.",strerror(errno));
		return CW_FALSE;
	}

	WTPDataChannelThreadRun = CW_TRUE ;
	
	if(!CWErr(CWCreateThread(&thread_receiveData, CWWTPReceiveDtlsDataPacket, (void*)gWTPDataSocket,0))) {
		CWNetworkCloseSocket(gWTPSocket);
		CWNetworkCloseSocket(gWTPDataSocket);
		CWLog("Error starting Thread that receive DATA DTLS packet when hot start.");
		return CW_FALSE;
	}
	
	CWWTPCheckNetLinkForBackUp();
	for(ptr=wtp_wlan_list;ptr != NULL;oldptr = ptr,ptr = ptr->next)
	{
		if(receiveframethread == 0){
			CWWTPWlan *wlancreating;
			CW_CREATE_OBJECT_ERR(wlancreating,CWWTPWlan,return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY,NULL););
			memset(wlancreating, 0, sizeof(CWWTPWlan));
			wlancreating->radio_id = ptr->radio_id;
			wlancreating->wlan_id = ptr->wlan_id;
		
			CWThread thread_receiveFrame_dot_11;
			CWThread thread_receiveFrame_dot_3;
			WTPWLanReceiveThreadRun = CW_TRUE ;
			if(!CWErr(CWCreateThread(&thread_receiveFrame_dot_11, CWWTPReceive802_11Frame, wlancreating,0))) {
				CWWTPDebugLog("Error starting Thread that receive binding frame");
			}
			if(!CWErr(CWCreateThread(&thread_receiveFrame_dot_3, CWWTPReceive802_3Frame,wlancreating,0))) {
				CWWTPDebugLog("Error starting Thread that receive binding frame");
			}
			receiveframethread = 1;

			/* pei add for check station aging, at 080703 */
			CWThread thread_netlink;
			if(!CWErr(CWCreateThread(&thread_netlink, CWWTPCheckNetLink, NULL,0))) {
				CWWTPDebugLog("Error starting Thread that check the netlink");
			}

			CW_FREE_OBJECT(wlancreating);
		}
		/*create the send sock for wlan*/
		{
			 struct sockaddr_ll toaddr1,toaddr2;
			 struct ifreq ifr;
			 int sockd1,sockd2;// 1 data 2 eap
			 char wlanname[IFNAMSIZ];
			 CWWTPGetWlanName(wlanname, ptr->radio_id, ptr->wlan_id);
			 if((sockd1 = socket(PF_PACKET,SOCK_RAW,autelan_htons(ETH_P_ALL))) < 0){
				CWWTPDebugLog("socket init error");
			 }	
			 if((sockd2 = socket(PF_PACKET,SOCK_RAW,autelan_htons(ETH_P_ALL))) < 0){
				CWWTPDebugLog("socket init error");
			 }
			 strncpy(ifr.ifr_name,wlanname, sizeof(ifr.ifr_name));
			 if (ioctl(sockd1, SIOCGIFINDEX, &ifr) == -1){//bind to a interface
				 CWWTPDebugLog("SIOCGIFINDEX1  failed");
			 }
			 if (ioctl(sockd2, SIOCGIFINDEX, &ifr) == -1){//bind to a interface
				CWWTPDebugLog("SIOCGIFINDEX2  failed");
			 }

			 memset(&toaddr1, '\0',sizeof(toaddr1));
			 toaddr1.sll_family = AF_PACKET;
			 toaddr1.sll_protocol = autelan_htons(0x0019);
			 toaddr1.sll_ifindex = ifr.ifr_ifindex;
			 if(sockd1 >= 0){//zengmin add by Coverity 2013-06-09
				 if(autelan_bind(sockd1,(struct sockaddr *)&toaddr1,sizeof(toaddr1)) < 0){
					CWWTPDebugLog("send sock bind  error");
				 }
			 }
			 memset(&toaddr2, '\0',sizeof(toaddr2));
			 toaddr2.sll_family = AF_PACKET;
			 toaddr2.sll_protocol = autelan_htons(ETH_P_ALL);
			 toaddr2.sll_ifindex = ifr.ifr_ifindex;
			 if(sockd2 >= 0){//zengmin add by Coverity 2013-06-09
				 if(autelan_bind(sockd2,(struct sockaddr *)&toaddr2,sizeof(toaddr2)) < 0){
					CWWTPDebugLog("send sock bind  error");
				 }
			 }
			 ptr->wlan_sendsock = sockd1;
			 ptr->wlan_eapsock = sockd2;
			 
		}
		
	}

/*	if(gApScanningEnable)
	{
		const int sleepinterval = 15;

		if(!CWErr(CWCreateThread(&thread_apScanning, CWWTPSetApScanning, (void *)sleepinterval,0))) {
			CWWTPDebugLog("Error starting Thread that Scanning rogue AP");
			dpf("Error starting Thread that Scanning rogue AP\n");
		}
	}
*/
	if(gApMonitorEnable)
	{
		if(!CWErr(CWCreateThread(&thread_apMonitor, CWWTPSetApMonitor, NULL,0))) {
			CWWTPDebugLog("Error starting Thread to Monitor");
//			return CW_FALSE;
		}
	}
	if(gExtraInfoEnable)
	{
		if(!CWErr(CWCreateThread(&thread_extraInfo, CWWTPSetExtraInfo, NULL,0))) {
			CWWTPDebugLog("Error starting Thread to get ExtraInfo");
//			return CW_FALSE;
		}
	}
	if(gStaInfoEnable)
	{
		if(!CWErr(CWCreateThread(&thread_staInfo, CWWTPGetStaInfo, NULL,0))) {
			CWWTPDebugLog("Error starting Thread to get StationInfo");
//			return CW_FALSE;
		}
	}
	if(gIfStateEnable)
	{
		if(!CWErr(CWCreateThread(&thread_ifState, CWWTPGetIfState, NULL,0))) {
			CWWTPDebugLog("Error starting Thread to get InterfaceState");
//			return CW_FALSE;
		}
	}
/*	{
		if(!CWErr(CWCreateThread(&thread_snooping, CWWTPCheckMsgFromOtherPro, NULL,0))) {
			CWWTPDebugLog("Error starting Thread that check DHCPsnooping Report");
			return CW_FALSE;
		}
	}
*/
	if(gStaPktsStatisticEnable)
	{
		if(!CWErr(CWCreateThread(&thread_sta_pkts_statistic_report,CWWTPStaPktsStatisticReport,NULL,0))){
			CWWTPDebugLog("Error starting Thread that sta pkts statistic Report");
//			return CW_FALSE;
		}
	}
	if(gApPktsStatisticEnable)
	{
		if(!CWErr(CWCreateThread(&thread_ap_pkts_statistic_report,CWWTPApPktsStatisticReport,NULL,0))){
			CWWTPDebugLog("Error starting Thread that ap pkts statistic Report");
//			return CW_FALSE;
		}
	}

	return CW_TRUE;
}
CWBool CWInitConfigForBackup_old()
{
	int i = 0;
	int n = 0;
	FILE *fp;
	char filedir[128];
	char str[24];
//	int wlan_count = 0;
//	int radio_id = 0;
//	int wlan_id = 0;
	int radioCount = 1;
	int radio_wlan_count[2] = {0, 0};
	CWWTPWlan wlanCreate;
//	char temp_str[128];
	
	if(0 != access(gWtpdConfDir, 0))
	{
		CWWTPDebugLog("The config dir for wtpd is not exist!");
		return CW_FALSE;
	}

	radioCount = CWGetRadioMaxCount();
	if(radioCount == 0)
		radioCount = 2;
	for(i=0;i<radioCount;i++)
	{
		memset(filedir, 0, 128);
		autelan_sprintf(filedir, "%s/radio%d", gWtpdConfDir, i);
		if(0 == access(filedir, 0))
		{
			memset(filedir, 0, 128);
			autelan_sprintf(filedir, "%s/radio%d/channel", gWtpdConfDir, i);
			if((fp = autelan_fopen(filedir, "r")) != NULL)
			{
				memset(str, 0, 24);
				autelan_fgets(str, sizeof(str), fp);
				gRadioInfoValue[i].channel = atoi(str);
				autelan_fclose(fp);
			}

			memset(filedir, 0, 128);
			autelan_sprintf(filedir, "%s/radio%d/txtype", gWtpdConfDir, i);
			if((fp = autelan_fopen(filedir, "r")) != NULL)
			{
				memset(str, 0, 24);
				autelan_fgets(str, sizeof(str), fp);
				gRadioInfoValue[i].txtype = atoi(str);
				autelan_fclose(fp);
			}

			memset(filedir, 0, 128);
			autelan_sprintf(filedir, "%s/radio%d/txpower", gWtpdConfDir, i);
			if((fp = autelan_fopen(filedir, "r")) != NULL)
			{
				memset(str, 0, 24);
				autelan_fgets(str, sizeof(str), fp);
				gRadioInfoValue[i].txpower = atoi(str);
				autelan_fclose(fp);
			}

			memset(filedir, 0, 128);
			autelan_sprintf(filedir, "%s/radio%d/rate", gWtpdConfDir, i);
			if((fp = autelan_fopen(filedir, "r")) != NULL)
			{
				memset(str, 0, 24);
				autelan_fgets(str, sizeof(str), fp);
				gRadioInfoValue[i].rate = atoi(str);
				autelan_fclose(fp);
			}

			memset(filedir, 0, 128);
			autelan_sprintf(filedir, "%s/radio%d/rateAutoEnable", gWtpdConfDir, i);
			if((fp = autelan_fopen(filedir, "r")) != NULL)
			{
				memset(str, 0, 24);
				autelan_fgets(str, sizeof(str), fp);
				gRadioInfoValue[i].rateAutoEnable = atoi(str);
				autelan_fclose(fp);
			}

			memset(filedir, 0, 128);
			autelan_sprintf(filedir, "%s/radio%d/radiotype", gWtpdConfDir, i);
			if((fp = autelan_fopen(filedir, "r")) != NULL)
			{
				memset(str, 0, 24);
				autelan_fgets(str, sizeof(str), fp);
				gRadioInfoValue[i].radiotype = atoi(str);
				autelan_fclose(fp);
			}

			memset(filedir, 0, 128);
			autelan_sprintf(filedir, "%s/radio%d/fragThreshold", gWtpdConfDir, i);
			if((fp = autelan_fopen(filedir, "r")) != NULL)
			{
				memset(str, 0, 24);
				autelan_fgets(str, sizeof(str), fp);
				gRadioInfoValue[i].fragThreshold = atoi(str);
				autelan_fclose(fp);
			}

			memset(filedir, 0, 128);
			autelan_sprintf(filedir, "%s/radio%d/rtsThreshold", gWtpdConfDir, i);
			if((fp = autelan_fopen(filedir, "r")) != NULL)
			{
				memset(str, 0, 24);
				autelan_fgets(str, sizeof(str), fp);
				gRadioInfoValue[i].rtsThreshold = atoi(str);
				autelan_fclose(fp);
			}

			memset(filedir, 0, 128);
			autelan_sprintf(filedir, "%s/radio%d/shortRetry", gWtpdConfDir, i);
			if((fp = autelan_fopen(filedir, "r")) != NULL)
			{
				memset(str, 0, 24);
				autelan_fgets(str, sizeof(str), fp);
				gRadioInfoValue[i].shortRetry = atoi(str);
				autelan_fclose(fp);
			}

			memset(filedir, 0, 128);
			autelan_sprintf(filedir, "%s/radio%d/longRetry", gWtpdConfDir, i);
			if((fp = autelan_fopen(filedir, "r")) != NULL)
			{
				memset(str, 0, 24);
				autelan_fgets(str, sizeof(str), fp);
				gRadioInfoValue[i].longRetry = atoi(str);
				autelan_fclose(fp);
			}

			memset(filedir, 0, 128);
			autelan_sprintf(filedir, "%s/radio%d/beaconInterval", gWtpdConfDir, i);
			if((fp = autelan_fopen(filedir, "r")) != NULL)
			{
				memset(str, 0, 24);
				autelan_fgets(str, sizeof(str), fp);
				gRadioInfoValue[i].beaconInterval = atoi(str);
				autelan_fclose(fp);
			}

			memset(filedir, 0, 128);
			autelan_sprintf(filedir, "%s/radio%d/preamble", gWtpdConfDir, i);
			if((fp = autelan_fopen(filedir, "r")) != NULL)
			{
				memset(str, 0, 24);
				autelan_fgets(str, sizeof(str), fp);
				gRadioInfoValue[i].preamble = atoi(str);
				autelan_fclose(fp);
			}

			memset(filedir, 0, 128);
			autelan_sprintf(filedir, "%s/radio%d/dtim", gWtpdConfDir, i);
			if((fp = autelan_fopen(filedir, "r")) != NULL)
			{
				memset(str, 0, 24);
				autelan_fgets(str, sizeof(str), fp);
				gRadioInfoValue[i].dtim = atoi(str);
				autelan_fclose(fp);
			}

			memset(filedir, 0, 128);
			autelan_sprintf(filedir, "%s/radio%d/gIsRadioEnable", gWtpdConfDir, i);
			if((fp = autelan_fopen(filedir, "r")) != NULL)
			{
				memset(str, 0, 24);
				autelan_fgets(str, sizeof(str), fp);
				gRadioInfoValue[i].gIsRadioEnable = atoi(str);
				autelan_fclose(fp);
			}

			memset(filedir, 0, 128);
			autelan_sprintf(filedir, "%s/radio%d/wlan_count", gWtpdConfDir, i);
			if((fp = autelan_fopen(filedir, "r")) != NULL)
			{
				memset(str, 0, 24);
				autelan_fgets(str, sizeof(str), fp);
				radio_wlan_count[i] = atoi(str);
				autelan_fclose(fp);
			}
			for(n=1; n<radio_wlan_count[i]+1; n++)
			{
				memset(&wlanCreate, 0, sizeof(CWWTPWlan));
				wlanCreate.radio_id = i;
				memset(filedir, 0, 128);
				autelan_sprintf(filedir, "%s/radio%d/wlaninfo/wlan%d", gWtpdConfDir, i, n);
			//	if((fp = autelan_fopen(filedir, "r")) != NULL)
				if(0 == access(filedir, F_OK)) //zengmin fix Unused pointer value by Coverity 2013-06-08	
				{
					memset(filedir, 0, 128);
					autelan_sprintf(filedir, "%s/radio%d/wlaninfo/wlan%d/wlan_id", gWtpdConfDir, i, n);
					if((fp = autelan_fopen(filedir, "r")) != NULL)
					{
						memset(str, 0, 24);
						autelan_fgets(str, sizeof(str), fp);
						wlanCreate.wlan_id = atoi(str);
						autelan_fclose(fp);
					}
					memset(filedir, 0, 128);
					autelan_sprintf(filedir, "%s/radio%d/wlaninfo/wlan%d/vlan_id", gWtpdConfDir, i, n);
					if((fp = autelan_fopen(filedir, "r")) != NULL)
					{
						memset(str, 0, 24);
						autelan_fgets(str, sizeof(str), fp);
						wlanCreate.vlan_id = atoi(str);
						autelan_fclose(fp);
					}
					memset(filedir, 0, 128);
					autelan_sprintf(filedir, "%s/radio%d/wlaninfo/wlan%d/wlan_updown_time", gWtpdConfDir, i, n);
					if((fp = autelan_fopen(filedir, "r")) != NULL)
					{
						memset(str, 0, 24);
						autelan_fgets(str, sizeof(str), fp);
						wlanCreate.wlan_updown_time = atoi(str);
						autelan_fclose(fp);
					}
					memset(filedir, 0, 128);
					autelan_sprintf(filedir, "%s/radio%d/wlaninfo/wlan%d/wlan_mac_mode", gWtpdConfDir, i, n);
					if((fp = autelan_fopen(filedir, "r")) != NULL)
					{
						memset(str, 0, 24);
						autelan_fgets(str, sizeof(str), fp);
						wlanCreate.wlan_mac_mode = atoi(str);
						autelan_fclose(fp);
					}
					memset(filedir, 0, 128);
					autelan_sprintf(filedir, "%s/radio%d/wlaninfo/wlan%d/wlan_tunnel_mode", gWtpdConfDir, i, n);
					if((fp = autelan_fopen(filedir, "r")) != NULL)
					{
						memset(str, 0, 24);
						autelan_fgets(str, sizeof(str), fp);
						wlanCreate.wlan_tunnel_mode = atoi(str);
						autelan_fclose(fp);
					}
				}
				CWBackupWlanTableAdd(&wlanCreate);
				CWGetWlanStations(wlanCreate.radio_id, wlanCreate.wlan_id);
			}

		}
	}
/*
	memset(temp_str, 0, 128);
	sprintf(temp_str, "cat /proc/net/dev | awk -F':' '/ath/{print $1}' | wc -l");
	fp = popen(temp_str, "r");
	if(fp)
	{
		memset(str, 0, 24);
		fgets(str, sizeof(str), fp);
		str[strlen(str)-1] = '\0';
		wlan_count = atoi(str);
		pclose(fp);
	}
	for(i=0;i<wlan_count;i++)
	{
		memset(temp_str, 0, 128);
//		sprintf(temp_str, "cat /proc/net/dev | awk -F':' '/ath/{print $1}' | awk -F\"-|[ ]+\" '{if(NR==%d){print $2}}'", i+1);
		sprintf(temp_str, "cat /proc/net/dev | grep ath | awk -F '[:-]' '{if(NR==%d)print $2}' ", i+1);
		fp = popen(temp_str, "r");
		if(fp)
		{
			memset(str, 0, 24);
			fgets(str, sizeof(str), fp);
			str[strlen(str)-1] = '\0';
			wlan_id = atoi(str);
			pclose(fp);
		}
		radio_id = 0;
		CWBackupWlanTableAdd(radio_id, wlan_id);
		CWGetWlanStations(radio_id, wlan_id);
	}
*/
	memset(filedir, 0, 128);
	autelan_sprintf(filedir, "%s/gApScanningEnable", gWtpdConfDir);
	if((fp = autelan_fopen(filedir, "r")) != NULL)
	{
		memset(str, 0, 24);
		autelan_fgets(str, sizeof(str), fp);
		gApScanningEnable = atoi(str);
		autelan_fclose(fp);
	}
	memset(filedir, 0, 128);
	autelan_sprintf(filedir, "%s/gApScanningInterval", gWtpdConfDir);
	if((fp = autelan_fopen(filedir, "r")) != NULL)
	{
		memset(str, 0, 24);
		autelan_fgets(str, sizeof(str), fp);
		gApScanningInterval = atoi(str);
		autelan_fclose(fp);
	}
	
	memset(filedir, 0, 128);
	autelan_sprintf(filedir, "%s/gAPThroughputInfoCollectEnable", gWtpdConfDir);
	if((fp = autelan_fopen(filedir, "r")) != NULL)
	{
		memset(str, 0, 24);
		autelan_fgets(str, sizeof(str), fp);
		gAPThroughputInfoCollectEnable = atoi(str);
		autelan_fclose(fp);
	}
	memset(filedir, 0, 128);
	autelan_sprintf(filedir, "%s/gAPThroughputInfoInterval", gWtpdConfDir);
	if((fp = autelan_fopen(filedir, "r")) != NULL)
	{
		memset(str, 0, 24);
		autelan_fgets(str, sizeof(str), fp);
		gAPThroughputInfoInterval= atoi(str);
		autelan_fclose(fp);
	}
	memset(filedir, 0, 128);
	autelan_sprintf(filedir, "%s/gApMonitorEnable", gWtpdConfDir);
	if((fp = autelan_fopen(filedir, "r")) != NULL)
	{
		memset(str, 0, 24);
		autelan_fgets(str, sizeof(str), fp);
		gApMonitorEnable = atoi(str);
		autelan_fclose(fp);
	}
	memset(filedir, 0, 128);
	autelan_sprintf(filedir, "%s/gApMonitorInterval", gWtpdConfDir);
	if((fp = autelan_fopen(filedir, "r")) != NULL)
	{
		memset(str, 0, 24);
		autelan_fgets(str, sizeof(str), fp);
		gApMonitorInterval = atoi(str);
		autelan_fclose(fp);
	}
	memset(filedir, 0, 128);
	autelan_sprintf(filedir, "%s/gExtraInfoEnable", gWtpdConfDir);
	if((fp = autelan_fopen(filedir, "r")) != NULL)
	{
		memset(str, 0, 24);
		autelan_fgets(str, sizeof(str), fp);
		gExtraInfoEnable = atoi(str);
		autelan_fclose(fp);
	}
	memset(filedir, 0, 128);
	autelan_sprintf(filedir, "%s/gExtraInfoInterval", gWtpdConfDir);
	if((fp = autelan_fopen(filedir, "r")) != NULL)
	{
		memset(str, 0, 24);
		autelan_fgets(str, sizeof(str), fp);
		gExtraInfoInterval = atoi(str);
		autelan_fclose(fp);
	}
	memset(filedir, 0, 128);
	autelan_sprintf(filedir, "%s/gStaInfoEnable", gWtpdConfDir);
	if((fp = autelan_fopen(filedir, "r")) != NULL)
	{
		memset(str, 0, 24);
		autelan_fgets(str, sizeof(str), fp);
		gStaInfoEnable = atoi(str);
		autelan_fclose(fp);
	}
	memset(filedir, 0, 128);
	autelan_sprintf(filedir, "%s/gStaInfoInterval", gWtpdConfDir);
	if((fp = autelan_fopen(filedir, "r")) != NULL)
	{
		memset(str, 0, 24);
		autelan_fgets(str, sizeof(str), fp);
		gStaInfoInterval = atoi(str);
		autelan_fclose(fp);
	}
	memset(filedir, 0, 128);
	autelan_sprintf(filedir, "%s/gIfStateEnable", gWtpdConfDir);
	if((fp = autelan_fopen(filedir, "r")) != NULL)
	{
		memset(str, 0, 24);
		autelan_fgets(str, sizeof(str), fp);
		gIfStateEnable = atoi(str);
		autelan_fclose(fp);
	}
	memset(filedir, 0, 128);
	autelan_sprintf(filedir, "%s/gIfStateInterval", gWtpdConfDir);
	if((fp = autelan_fopen(filedir, "r")) != NULL)
	{
		memset(str, 0, 24);
		autelan_fgets(str, sizeof(str), fp);
		gIfStateInterval = atoi(str);
		autelan_fclose(fp);
	}
	memset(filedir, 0, 128);
	autelan_sprintf(filedir, "%s/gFloodingDetectEnable", gWtpdConfDir);
	if((fp = autelan_fopen(filedir, "r")) != NULL)
	{
		memset(str, 0, 24);
		autelan_fgets(str, sizeof(str), fp);
		gFloodingDetectEnable = atoi(str);
		autelan_fclose(fp);
	}
	memset(filedir, 0, 128);
	autelan_sprintf(filedir, "%s/gSpoofingDetectEnable", gWtpdConfDir);
	if((fp = autelan_fopen(filedir, "r")) != NULL)
	{
		memset(str, 0, 24);
		autelan_fgets(str, sizeof(str), fp);
		gSpoofingDetectEnable = atoi(str);
		autelan_fclose(fp);
	}
	memset(filedir, 0, 128);
	autelan_sprintf(filedir, "%s/gWeakIVDetectEnable", gWtpdConfDir);
	if((fp = autelan_fopen(filedir, "r")) != NULL)
	{
		memset(str, 0, 24);
		autelan_fgets(str, sizeof(str), fp);
		gWeakIVDetectEnable = atoi(str);
		autelan_fclose(fp);
	}
	memset(filedir, 0, 128);
	autelan_sprintf(filedir, "%s/gFloodAttackCnt", gWtpdConfDir);
	if((fp = autelan_fopen(filedir, "r")) != NULL)
	{
		memset(str, 0, 24);
		autelan_fgets(str, sizeof(str), fp);
		gFloodAttackCnt = atoi(str);
		autelan_fclose(fp);
	}
	memset(filedir, 0, 128);
	autelan_sprintf(filedir, "%s/gFloodProbeCnt", gWtpdConfDir);
	if((fp = autelan_fopen(filedir, "r")) != NULL)
	{
		memset(str, 0, 24);
		autelan_fgets(str, sizeof(str), fp);
		gFloodProbeCnt = atoi(str);
		autelan_fclose(fp);
	}
	memset(filedir, 0, 128);
	autelan_sprintf(filedir, "%s/gFloodWidsIntval", gWtpdConfDir);
	if((fp = autelan_fopen(filedir, "r")) != NULL)
	{
		memset(str, 0, 24);
		autelan_fgets(str, sizeof(str), fp);
		gFloodWidsIntval = atoi(str);
		autelan_fclose(fp);
	}

	memset(filedir, 0, 128);
	autelan_sprintf(filedir, "%s/ac_addr", gWtpdConfDir);
	if((fp = autelan_fopen(filedir, "r")) != NULL)
	{
		memset(str, 0, 24);
		autelan_fgets(str, sizeof(str), fp);
		autelan_fclose(fp);
	}
//	printf("---str:%s\n", str);
	CW_CREATE_OBJECT_ERR(gACInfoPtr, CWACInfoValues, return CW_QUIT;);
	CWNetworkGetAddressForHost(str, &(gACInfoPtr->preferredAddress));
	if(gCWForceMTU > 0) {
		gWTPPathMTU = gCWForceMTU;
	}

	memset(filedir, 0, 128);
	autelan_sprintf(filedir, "%s/gEchoInterval", gWtpdConfDir);
	if((fp = autelan_fopen(filedir, "r")) != NULL)
	{
		memset(str, 0, 24);
		autelan_fgets(str, sizeof(str), fp);
		gEchoInterval = atoi(str);
		autelan_fclose(fp);
	}
	memset(filedir, 0, 128);
	autelan_sprintf(filedir, "%s/gCWNeighborDeadInterval", gWtpdConfDir);
	if((fp = autelan_fopen(filedir, "r")) != NULL)
	{
		memset(str, 0, 24);
		autelan_fgets(str, sizeof(str), fp);
		gCWNeighborDeadInterval = atoi(str);
		autelan_fclose(fp);
	}
	
/*	memset(filedir, 0, 128);
	sprintf(filedir, "%s/wtp_wlan_count", gWtpdConfDir);
	if((fp = fopen(filedir, "r")) != NULL)
	{
		memset(str, 0, 24);
		fgets(str, sizeof(str), fp);
		wtp_wlan_count = atoi(str);
		fclose(fp);
	}
*/
	return CW_TRUE;
}
CWBool CWGetConfigFromFile(char filepath[128],char filename[64],char * str)
{
	int fd;
	char file[192];
	memset(file,0,192);
	strcat(file,filepath);
	strcat(file,filename);
	if((fd = autelan_open(file,O_RDONLY))== -1)
	{
		dpf("open file %s failed! \t Use Default Value.\n",file);	
		return CW_FALSE;
	}
	else
	{
		autelan_read(fd,str,64);
		dpf("from %s\tget %s = %s\n",file,filename,str);
		autelan_close(fd);
	}	
	return CW_TRUE;
}

CWBool CWInitConfigForBackup()
{
	char tmp[64];
	char str[64];	
	char filepath[128];	
	char filepath_EchoInterval[128];
	CWWTPWlan wlanCreate;
	memset(str,0,64);
	memset(filepath,0,128);
	memset(filepath_EchoInterval,0,128);
	strcpy(filepath,"/tmp/conf_wtpd/");
	strcpy(filepath_EchoInterval,"/tmp/conf_wtpd/gEchoInterval");
	
	//add by sundl@20110916 for  error  [1]  killed   wtpd
	if(access(filepath_EchoInterval,0) !=0)
	{
		if(debug_print)
		{
			autelan_printf("%s does't exists,hot restart failure...sysreboot\n",filepath_EchoInterval);
		}
		kes_debug("/tmp/conf_wtpd/gEchoInterval does't exists,hot restart failure...sysreboot");
		_system("echo b > /proc/kes_debug_flag");
		_system("/sbin/sysreboot");	
	}//end
	
	if(CWGetConfigFromFile(filepath,"gCWForceMTU",str))
		gCWForceMTU = atoi(str);	
	if(CWGetConfigFromFile(filepath,"gEnabledLog",str))
		gEnabledLog = atoi(str);	
	if(CWGetConfigFromFile(filepath,"gMaxLogFileSize",str))
		gMaxLogFileSize = atoi(str);	
	if(CWGetConfigFromFile(filepath,"WTPModelNum",str))
		strcpy(WTPModelNum,str);	
	if(CWGetConfigFromFile(filepath,"WTPRealModelNum",str))
		strcpy(WTPRealModelNum,str);
	if(CWGetConfigFromFile(filepath,"gIPMode",str))
		gIPMode = atoi(str);
	if(CWGetConfigFromFile(filepath,"e_swtich",str))
		gEMenuConf.e_swtich = atoi(str);
	
	if(CWGetConfigFromFile(filepath,"gApScanningEnable",str))
		gApScanningEnable = atoi(str);	
	if(CWGetConfigFromFile(filepath,"gApScanningInterval",str))
		gApScanningInterval = atoi(str);	
	if(CWGetConfigFromFile(filepath,"gAPThroughputInfoCollectEnable",str))
		gAPThroughputInfoCollectEnable = atoi(str);	
	if(CWGetConfigFromFile(filepath,"gAPThroughputInfoInterval",str))
		gAPThroughputInfoInterval = atoi(str);	
	if(CWGetConfigFromFile(filepath,"gApMonitorEnable",str))
		gApMonitorEnable = atoi(str);	
	if(CWGetConfigFromFile(filepath,"gApMonitorInterval",str))
		gApMonitorInterval = atoi(str);	
	if(CWGetConfigFromFile(filepath,"gExtraInfoEnable",str))
		gExtraInfoEnable = atoi(str);	
	if(CWGetConfigFromFile(filepath,"gExtraInfoInterval",str))
		gExtraInfoInterval = atoi(str);	
	if(CWGetConfigFromFile(filepath,"gStaInfoEnable",str))
		gStaInfoEnable = atoi(str);	
	if(CWGetConfigFromFile(filepath,"gStaInfoInterval",str))
		gStaInfoInterval = atoi(str);	
	if(CWGetConfigFromFile(filepath,"gIfStateEnable",str))
		gIfStateEnable = atoi(str);	
	if(CWGetConfigFromFile(filepath,"gIfStateInterval",str))
		gIfStateInterval = atoi(str);	
	if(CWGetConfigFromFile(filepath,"gFloodingDetectEnable",str))
		gFloodingDetectEnable = atoi(str);	
	if(CWGetConfigFromFile(filepath,"gSpoofingDetectEnable",str))
		gSpoofingDetectEnable = atoi(str);	
	if(CWGetConfigFromFile(filepath,"gWeakIVDetectEnable",str))
		gWeakIVDetectEnable = atoi(str);	
	if(CWGetConfigFromFile(filepath,"gFloodAttackCnt",str))
		gFloodAttackCnt = atoi(str);	
	if(CWGetConfigFromFile(filepath,"gFloodProbeCnt",str))
		gFloodProbeCnt = atoi(str);	
	if(CWGetConfigFromFile(filepath,"gFloodWidsIntval",str))
		gFloodWidsIntval = atoi(str);	
	if(CWGetConfigFromFile(filepath,"gEchoInterval",str))
		gEchoInterval = atoi(str);	
	if(CWGetConfigFromFile(filepath,"gCWNeighborDeadInterval",str))
		gCWNeighborDeadInterval = atoi(str);	
	if(CWGetConfigFromFile(filepath,"gNetworkPreferredFamily",str))
		gNetworkPreferredFamily = atoi(str);
	if(CWGetConfigFromFile(filepath,"gStaPktsStatisticEnable",str))
		gStaPktsStatisticEnable = atoi(str);	
	if(CWGetConfigFromFile(filepath,"gStaPktsStatisticInterval",str))
		gStaPktsStatisticInterval = atoi(str);
	if(CWGetConfigFromFile(filepath,"gApPktsStatisticEnable",str))
		gApPktsStatisticEnable = atoi(str);	
	if(CWGetConfigFromFile(filepath,"gApPktsStatisticInterval",str))
		gApPktsStatisticInterval = atoi(str);
	if(CWGetConfigFromFile(filepath,"ac_addr",str))
	{
		CW_CREATE_OBJECT_ERR(gACInfoPtr, CWACInfoValues, CWLog("malloc failed for gACInfoPtr,hot start init failed");return CW_QUIT;);
		CWNetworkGetAddressForHost(str, &(gACInfoPtr->preferredAddress));
		if(gCWForceMTU > 0) {
			gWTPPathMTU = gCWForceMTU;
		}
	}

	int radioCount = 1;
	if(CWGetConfigFromFile(filepath,"radio_count",str))
		radioCount = atoi(str);	

	int i;
	for(i=0;i<radioCount;i++)
	{
		char tmppath[192];
		memset(tmppath,0,128);
		autelan_sprintf(tmppath,"%sradio%d/",filepath,i);
		if(debug_print && isBackup)
			autelan_printf("tmppath = %s\n",tmppath);
		if(CWGetConfigFromFile(tmppath,"channel",str))
			gRadioInfoValue[i].channel = atoi(str);			
		if(CWGetConfigFromFile(tmppath,"txtype",str))
			gRadioInfoValue[i].txtype = atoi(str);	
		if(CWGetConfigFromFile(tmppath,"txpower",str))
			gRadioInfoValue[i].txpower = atoi(str);
		if(CWGetConfigFromFile(tmppath,"rate",str))
			gRadioInfoValue[i].rate = atoi(str);
		if(CWGetConfigFromFile(tmppath,"rateAutoEnable",str))
			gRadioInfoValue[i].rateAutoEnable = atoi(str);
		if(CWGetConfigFromFile(tmppath,"radiotype",str))
			gRadioInfoValue[i].radiotype = atoi(str);
		if(CWGetConfigFromFile(tmppath,"fragThreshold",str))
			gRadioInfoValue[i].fragThreshold = atoi(str);
		if(CWGetConfigFromFile(tmppath,"rtsThreshold",str))
			gRadioInfoValue[i].rtsThreshold = atoi(str);
		if(CWGetConfigFromFile(tmppath,"shortRetry",str))
			gRadioInfoValue[i].shortRetry = atoi(str);
		if(CWGetConfigFromFile(tmppath,"longRetry",str))
			gRadioInfoValue[i].longRetry = atoi(str);
		if(CWGetConfigFromFile(tmppath,"beaconInterval",str))
			gRadioInfoValue[i].beaconInterval = atoi(str);
		if(CWGetConfigFromFile(tmppath,"dtim",str))
			gRadioInfoValue[i].dtim = atoi(str);
		if(CWGetConfigFromFile(tmppath,"gIsRadioEnable",str))
			gRadioInfoValue[i].gIsRadioEnable = atoi(str);
		if(CWGetConfigFromFile(tmppath,"preamble",str))
			gRadioInfoValue[i].preamble = atoi(str);
		//for 11n
		if(CWGetConfigFromFile(tmppath,"cwmmode",str))
			gModeNParaValue[i].cwmMode= atoi(str);
		if(CWGetConfigFromFile(tmppath,"extoffset",str))
			gModeNParaValue[i].extoffset= atoi(str);	
		int wlan_count = 0;
		if(CWGetConfigFromFile(tmppath,"wlan_count",str))
			wlan_count = atoi(str);
		if(wlan_count == 0)
		{
			int ath_count = 0; 
			memset(tmp,0,64);
			autelan_sprintf(tmp,"ath.%d-[0-9]*",i);
			CWGetInterfaceCount(&ath_count, tmp);
			if(wlan_count != ath_count)
			{
				CWLog("wlancount=%d got from config_file is unequal to athcount=%d got from /proc/net/dev",wlan_count,ath_count);
				//return CW_FALSE;
			}
		}
		else
		{
			DIR *dir;
			struct dirent *ptr;
			strcat(tmppath,"wlaninfo");
			if((dir = opendir(tmppath))==NULL)
			{
				CWLog("wlan_count=%d,but cannot find wlan info,hot start init failed!",wlan_count);
				return CW_FALSE;
			}
			else
			{
				while((ptr = readdir(dir)) != NULL)
				{
					char wlanpath[192];
					memset(wlanpath,0,192);
					if(strcmp(".",ptr->d_name)!=0&&strcmp("..",ptr->d_name)!=0)
					{
						autelan_sprintf(wlanpath,"%s/%s/",tmppath,ptr->d_name);
						dpf("get wlaninfo from %s\n",wlanpath);				
						memset(&wlanCreate, 0, sizeof(CWWTPWlan));
						if(CWGetConfigFromFile(wlanpath,"radio_id",str))
							wlanCreate.radio_id=atoi(str);
						if(CWGetConfigFromFile(wlanpath,"wlan_id",str))
							wlanCreate.wlan_id=atoi(str);
						if(wlanCreate.wlan_id== 0)
						{
							CWLog("wlan_id got from config_file is 0,invalid,hot_start init failed");
							closedir(dir);////zengmin add by Coverity 2013-06-13
							return CW_FALSE;
						}
						if(CWGetConfigFromFile(wlanpath,"vlan_id",str))
							wlanCreate.vlan_id=atoi(str);
						if(CWGetConfigFromFile(wlanpath,"wlan_updown_time",str))
							wlanCreate.wlan_updown_time=atoi(str);
						if(CWGetConfigFromFile(wlanpath,"wlan_mac_mode",str))
							wlanCreate.wlan_mac_mode=atoi(str);
						if(CWGetConfigFromFile(wlanpath,"wlan_tunnel_mode",str))
							wlanCreate.wlan_tunnel_mode=atoi(str);
						if(CWGetConfigFromFile(wlanpath,"ssid",str)){
						//	memcpy(wlanCreate.essid,str,sizeof(str));
						/*zengmin modify: memcpy out-of-bounds access by Coverity 2013-06-09*/
							memcpy(wlanCreate.essid,str,sizeof(wlanCreate.essid));
							}
						gWlanIDMap[CWGetWlanIdxInMap(gWlanIDMap, wlanCreate.radio_id, 0)] = wlanCreate.wlan_id;
						CWBackupWlanTableAdd(&wlanCreate);
						CWGetWlanStations(wlanCreate.radio_id, wlanCreate.wlan_id);
					}
					
				}
				closedir(dir);
			}
		}

	}

	return CW_TRUE;
}
int CWPingTest(char *pingfile)
{
	int lost = 0;  /* 0-ping test no lost */
	char temp_str[128];
	char str[12];
	char cmdbuf[128];

	/* AC address */
	struct sockaddr *sa=(struct sockaddr*) (&gACInfoPtr->preferredAddress);
	struct sockaddr_in	*sin = (struct sockaddr_in *) sa;
	autelan_inet_ntop(AF_INET, &sin->sin_addr, temp_str, 128);
//	printf("ac:%s\n", temp_str);
//	printf("=======before ping test=======\n");
	autelan_sprintf(cmdbuf, "date > %s && echo >> %s", pingfile, pingfile);
	_system(cmdbuf);
	autelan_sprintf(cmdbuf, "/usr/sbin/myping %s >> %s 2>&1", temp_str, pingfile);
	_system(cmdbuf);
	autelan_sprintf(cmdbuf, "echo >> %s && date >> %s", pingfile, pingfile);
	_system(cmdbuf);
//	printf("=======after ping test=======\n");
	
	
	memset(temp_str, 0, 128);
	autelan_sprintf(temp_str, "cat %s | awk -F ' ' '/lost/ {print $7}' | awk -F '%%' '{print $1}'", pingfile);
	PopenFile(temp_str, str, sizeof(str));
//		printf("str:%s\n", str);
	lost = atoi(str);
	CWWTPDebugLog("ping test, lost:%d%%", lost);
	
	return lost;
}

CWBool CWWTPDataGenericRunMessage(CWProtocolMessage *msgPtr)
{
	unsigned char header_len;/*the length of capwap header*/
	int wlan_sock,eap_sock;
	CWWTPWlan *ptr = NULL;
	unsigned char *sendmsg;
	int n;
	int wlanfound = 0;
	unsigned char radio_id, wlan_id;
	

	//dpf("------readBytes:%d  msgPtr->offset:%d------\n", readBytes, msgPtr->offset);
//	CWCaptrue(readBytes, msgPtr->msg);
	if(msgPtr == NULL) 
		return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	int readBytes = msgPtr->offset;
//	msgPtr->offset = 0;
	
	header_len = 16;
	radio_id = (unsigned char)((*(int *)msgPtr->msg>>14)&0x1f);
	wlan_id =  *(unsigned char *)(msgPtr->msg + 8);
	for(ptr = wtp_wlan_list;ptr != NULL;ptr = ptr->next){
		if((ptr->radio_id == radio_id)&&(ptr->wlan_id == wlan_id)){
			wlan_sock = ptr->wlan_sendsock;
			eap_sock = ptr->wlan_eapsock;
			wlanfound = 1;
			break;
		}				
	}
	if(wlanfound == 0){
		dpf("\n can't find the send wlan \n");
		return CW_TRUE;
	}

	dpf("send it use pf_sock  via ath.%d-%d\n", radio_id, wlan_id);

	sendmsg = (unsigned char *)(msgPtr->msg+header_len);
	n = autelan_sendto(wlan_sock, sendmsg, (readBytes-header_len), 0x0, NULL, 0); //pei 0917 del warning

//			printf("send assoc response\n");
	if(n<0)
		autelan_printf("data channel send error\n");

	CW_FREE_PROTOCOL_MESSAGE(*msgPtr);
	return CW_TRUE;
}
// Manage Run State
CWStateTransition CWWTPEnterRun() {
//	int k;
	int i ;
	int sleepinterval;
	
	if(isBackup)
	{
		CWWTPDebugLog("WTPD hot restart......In run state");
		_system("echo 1 > /proc/sys/dev/wifi0/thinap_state");
		if(!CWInitConfigForBackup())
		{
			return CW_QUIT;
		}
		if(!CWInitSockForBackup())
		{
			CWLog("init sock error when wtpd hot restart in run state, reboot");
			return CW_QUIT;
		}
	}
	else
	{
		CWWTPDebugLog("######### WTP enters in RUN State #########");
/*
  *zengmin del sta_access_limit 2013-05-02
		if(autelan_system("sta_access_limit &") == 0)//yuan add sta access limit
		{
			CWWTPDebugLog("sta_access_limit OK");
		}
*/
		/**turn off the blind annd turn the blue ligth*/
#if (defined(AQ2010))
		_system("wrcpuutil -d");
		_system("wrcpuutil -l  -n 0 -s 1");
#elif (defined( CAVIUM_PLATFORM))
		_system("avrutil -B");
#else
		_system("/usr/sbin/setled sysled enable");
#endif

#if 0
		int n=0;
		int wifiCount = 0;
		char cmdbuf[64];
		CWGetInterfaceCount(&wifiCount, "wifi[0-9]");
		/*gzm delete this limit later*/
		for(n=0;n<wifiCount;n++)
		{
			if(strcmp(WTPRealModelNum, "AQ2000-SNH") == 0){
				memset(cmdbuf, 0, 64);
				sprintf(cmdbuf, "iwpriv wifi%d txchainmask 1", n);
				_system(cmdbuf);
			}
			else{
				memset(cmdbuf, 0, 64);
				sprintf(cmdbuf,"iwpriv wifi%d txchainmask 3", n); /* 11n Set the chain masks */
				_system(cmdbuf);
				sprintf(cmdbuf,"iwpriv wifi%d rxchainmask 3", n); /* 11n Set the chain masks */
				_system(cmdbuf);
			}	
		}
#endif

		
//		for (k = 0; k < MAX_PENDING_REQUEST_MSGS; k++)
//			CWResetPendingMsgBox(gPendingRequestMsgs + k);

		/* Set last connect local IP, and touch file connect_state. */
		CWSetLastConnectLocalIP();
		CWSetLastConnectGateway();
		_system("touch /tmp/connect_state");
		_system("echo 1 > /tmp/aprebootflag");
		_system("echo 1 > /proc/sys/dev/wifi0/thinap_state");
		
		/* pei add 0213 */
		CWGetAPOnlineTime(&gPowerOnUntilRuntime);
		CWWTPSendWTPWarning();
	}
	/*gzm add gwlanStatsInfo init*/	
	for(i=0;i<NUM_OF_ATH_ALL_WIFI;i++)
	{
		memset(&gwlanStatsInfo[i], 0, sizeof(wlan_stats_info));
	}
	/* gethStatsInfo init */
	for(i=0;i<NUM_OF_ETH;i++)
	{
		memset(&gethStatsInfo[i], 0, sizeof(wlan_stats_info));
	}
	if (!CWErr(CWStartHeartbeatTimer()))
	{
		CWWTPDebugLog("CWStartHeartbeatTimer error.");
		//return CW_QUIT;
	}
	/* pei add for rogue AP 1120 */
	if(gApScanningEnable)
	{
		sleepinterval = 45;

//		gApScanningThreadEnable = 1;
		if(!CWErr(CWCreateThread(&thread_apScanning, CWWTPSetApScanning, (void *)sleepinterval,0))) {
			CWWTPDebugLog("Error starting Thread that Scanning rogue AP");
		}
	}
/* Begin: gengzj added for wifipos 2013-11-26 */
#if 0 /* liwei_chg: change start situation */
    if (!CWWTPStartDetector())
    {
        CWWTPDebugLog("Error starting Detector that does Wifi Scanning and Positioning");
        return CW_FALSE;
    }
#endif
	CWWTPDebugLog("before Init ScanPosSock CWWTPInitScanPosSock = %d", gScanPosSocket);
    if (!CWWTPInitScanPosSock(&gScanPosSocket))
    {
        CWWTPDebugLog("Scan Positon Socket init error: gScanPosSocket = %d", gScanPosSocket);
    }
#ifdef _SCANPOS_RESULT_REPORT_
    if (!CWErr(CWCreateThread(&thread_scanpos_res, CWWTPReceiveWifiScanAndPosResult, 
                              (void*)gScanPosSocket, 0)))
    {
		CWDebugLog("Error starting Thread that get Scan And Position Result from detector");
		return CW_FALSE;
	}
#endif /* _SCANPOS_RESULT_REPORT_ */
/* End: gengzj added end */
	if(gAPThroughputInfoCollectEnable)
	{
		sleepinterval = 40;
//		gAPThroughputInfoThreadEnable = 1;
		
		if(!CWErr(CWCreateThread(&thread_wtp_throughput_info, acw_wtp_throughput_info, (void *)sleepinterval,0))){
			CWWTPDebugLog("Error starting Thread to wtp_throughput_info");
		}
	}
	if(!CWErr(CWCreateThread(&thread_snooping, CWWTPCheckMsgFromOtherPro, NULL,0))) {
		CWWTPDebugLog("Error starting Thread that check DHCPsnooping Report");
		return CW_FALSE;
	}
/* for test I add a teset func CWWTPDioTest()*/
#if 0
CWWTPDioTest();
#endif

	CW_REPEAT_FOREVER
	{
		if(CWWTPNeighborDeadTimerExpiredFlag != CW_FALSE)
		{
			/* goto CWENTERRESET; */
			CWLog("CWWTPNeighborDeadTimerExpiredFlag = %d",  CWWTPNeighborDeadTimerExpiredFlag);
		}
		struct timespec timenow;
		CWBool bReceivePacket = CW_FALSE;
#ifndef CW_NO_DTLS
		CWBool bReceiveDataPacket = CW_FALSE; /* pei data dtls 20100510 */
#endif
		CWBool bReveiveBinding = CW_FALSE;
		int ret = 0;
		
		// Wait packet
		//timenow.tv_sec = time(0) + 240;	 //greater than NeighborDeadInterval
		timenow.tv_sec = autelan_time(0) + gCWNeighborDeadInterval + 240;	 //greater than NeighborDeadInterval
		timenow.tv_nsec = 0;

		CWThreadMutexLock(&gInterfaceMutex);

		/*
		 * MAURO: se non ci sono frame provenienti dalle stazioni ne'
		 * 	  pacchetti dall'AC...
		 */
		/*if the two list are null then wait for a time ( defined up timenow.tv_sec = time(0) + TIMEOUT) */
#ifndef CW_NO_DTLS
		if((gDtlsSecurity == 1)&&(gDtlsPolicy == 1))  /* pei data dtls 20100510 */
			ret = (CWGetCountElementFromSafeList(gPacketReceiveList) == 0) && (CWGetCountElementFromSafeList(gFrameList) == 0) &&(CWGetCountElementFromSafeList(gPacketDataReceiveList) == 0);
		else
#endif
			ret = (CWGetCountElementFromSafeList(gPacketReceiveList) == 0) && (CWGetCountElementFromSafeList(gFrameList) == 0);
//		if ((CWGetCountElementFromSafeList(gPacketReceiveList) == 0) && (CWGetCountElementFromSafeList(gFrameList) == 0))
		if (ret)
		{
//			CWLog("the two list are null ");
			/*
			 * MAURO: aspetta per al max 4min un frame dalle 
			 * 	  stazioni od un messaggio dall'AC...
			 */
			
			if (!CWErr(CWWaitThreadConditionTimeout(&gInterfaceWait, &gInterfaceMutex, &timenow)))
			{
				CWThreadMutexUnlock(&gInterfaceMutex);
			
				if (CWErrorGetLastErrorCode() == CW_ERROR_TIME_EXPIRED)
				{
					CWLog("No Message from AC for a long time... reboot");
					break;
				}

				continue;
			}
		}

		bReceivePacket = ((CWGetCountElementFromSafeList(gPacketReceiveList) != 0) ? CW_TRUE : CW_FALSE);
#ifndef CW_NO_DTLS
		if((gDtlsSecurity == 1)&&(gDtlsPolicy == 1))  /* pei data dtls 20100510 */
			bReceiveDataPacket = ((CWGetCountElementFromSafeList(gPacketDataReceiveList) != 0) ? CW_TRUE : CW_FALSE); /* pei data dtls 20100510 */
#endif
		bReveiveBinding = ((CWGetCountElementFromSafeList(gFrameList) != 0) ? CW_TRUE : CW_FALSE);
		CWThreadMutexUnlock(&gInterfaceMutex);

		if (bReceivePacket)/*receive  AC control message*/
		{
			CWProtocolMessage msg;

			msg.msg = NULL;
			msg.offset = 0;

			if (!(CWReceiveMessage(&msg))) 
			{
				CW_FREE_PROTOCOL_MESSAGE(msg);
				CWWTPDebugLog("%s,%d,Failure Receiving Control Msg From AC.",__func__,__LINE__);
				continue;
				//return CW_QUIT;
			}
			
			if(msg.msg == NULL)
			{
				CWWTPDebugLog("msg.msg is null,continue");
				continue;
			}
				
			if (!CWErr(CWWTPManageGenericRunMessage(&msg))) 
			{
				int ret=CWErrorGetLastErrorCode();
				if(ret == CW_ERROR_INVALID_FORMAT) 
				{
					// Log and ignore message
					//CWErrorHandleLast();
					if(!CWErrorHandleLast())//zengmin add by Coverity 2013-06-09
					{
						CWWTPDebugLog("Failure.%s %d",__func__,__LINE__);
					}
					CWWTPDebugLog("--> Received something different from a valid Run Message");
				} 
				else 
				{
					CW_FREE_PROTOCOL_MESSAGE(msg);
					CWLog("--> Critical Error Managing Generic Run Message.... ErrorCode:%d", ret);
					//return CW_QUIT;
				}
			}
		}

#ifndef CW_NO_DTLS
		if ((gDtlsSecurity == 1)&&(gDtlsPolicy == 1)&&bReceiveDataPacket)/*receive  AC data message*/  /* pei data dtls 20100510 */
		{
			CWProtocolMessage msgdata;

			msgdata.msg = NULL;
			msgdata.offset = 0;

			if (!(CWReceiveDataMessage(&msgdata))) 
			{
				CW_FREE_PROTOCOL_MESSAGE(msgdata);
				CWLog("Failure Receiving Response reboot");
				return CW_QUIT;
			}
			if (!CWErr(CWWTPDataGenericRunMessage(&msgdata))) 
			{
				if(CWErrorGetLastErrorCode() == CW_ERROR_INVALID_FORMAT) 
				{
					// Log and ignore message
					if(!CWErrorHandleLast())
						CWWTPDebugLog("--> Received something different from a valid Run Message");
				} 
				else 
				{
					CW_FREE_PROTOCOL_MESSAGE(msgdata);
					CWLog("--> Critical Error Managing Generic Run Message... reboot");
					return CW_QUIT;
				}
			}
			
		}
#endif
		if (bReveiveBinding)/*frome STA*/
			CWWTPCheckForBindingFrame();
	}
/* CWENTERRESET: */
	{
		CWLog("ac-lose reboot... In Run State.... Lose AC.");
		kes_debug("ac-lose reboot... In Run State.... Lose AC.");
		_system("echo e > /proc/kes_debug_flag");
		if(WTPGetApRebootFlag())
		{
			_system("/usr/sbin/ac-lose");
		}
		else
		{
			WTPInitConfig();
			_system("wtpd > /dev/null 2>&1 &");
			CWLog("apRebootFlag=0,so wtpd will be started by itself.");
			exit(0);
		}
//		_system("sysreboot");/*now reboot is the best (kekaoxing)*/
	}
	
	return CW_QUIT;
}
/***************************************************************************
 * 
 * Function:  CWWTPDioTest
 *
 * Purpose:   teset the control func at WTP 
 *
 * Inputs: 
 *
 * Output:    
 *
 * Returns:   
 **************************************************************************/

void CWWTPDioTest(void)
{
	if(debug_print)
	{
		autelan_printf("*******************************************************************************************************\n");
		autelan_printf("*******************************************Begin to test***********************************************\n");
		autelan_printf("*******************************************************************************************************\n");
	}
#ifdef CW_DEBUGGING
	CWProtocolMessage testmsg0;/*I create a test message myself*/
#if 0
	CWProtocolMessage testmsg1;/*I create a test message myself*/
	CWProtocolMessage testmsg2;/*I create a test message myself*/
	CWProtocolMessage testmsg3;/*I create a test message myself*/
	CWProtocolMessage testmsg4;/*I create a test message myself*/
	CWProtocolMessage testmsg5;/*I create a test message myself*/
	CWProtocolMessage testmsg6;/*I create a test message myself*/
	CWProtocolMessage testmsg7;/*I create a test message myself*/
#endif
#if 1
/* Begin: gengzj added for wifipos 2013-11-26 */
	unsigned char pos_buf[200] =
	{
		0x00, 0x00, 0x00, 0x07, 0x12, 0x00, 0x50, 0x00,  /* control header */
		0x00, 0x25, 0x00, 0x49, 0x80, 0x00, 0x7b, 0xa8,  /* elem header */
		0x00, 0x33, 0x00, 0x41, 0x00, 0x01, 0x01, 0x01, 
		0x01, 0x02, 0x02, 0x01, 0x00,                    /* attribute header */
		0x03, 0x02, 0x00, 0x34, 0x00, 0x01, 0x00, 0x01,
		0x01, 0x03, 0x02, 0x02, 0x00, 0x00, 0x03, 0x02, 0x00,
		0xc8, 0x04, 0x01, 0x00, 0x05, 0x01, 0x00, 0x06,  0x04, 0xc0, 0xa8,
		0x01, 0x01, 0x07, 0x02, 0x2a, 
		0xf8, 0x08, 0x01, 0x00, 0x09, 0x01, 0x00, 0x0a,  0x08, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 
		0xff, 0x0b, 0x02, 0x01, 0x0c, 0x01, 0x05 
	};
#endif
#if 0
/* End: gengzj added end */
	unsigned char my_buf0[CW_BUFFER_SIZE] = /*test netlink*/
	{
//		0x01,0x10,0x03,0x00,
//		0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x09,       /*control message type  */
		0x04,		/*seq num*/
		0x00,0x0f,	/*msg element length*/
		0x00,		/*flags*/
		0x00,0x12,	/*message element type*/
		0x00,0x08,	/*message element length*/
		0x00,		/*radio ID*/
		0x06,		/*length*/
		0x00,0x11,0x22,0x33,0x44,0x55/*station mac*/
	};

#endif
	
#if 0

	unsigned char my_buf1[CW_BUFFER_SIZE] = 
	{
		0x00,0x33,0xdd,0x00,               /*control message type  */
		0x04,/*seq num*/
//		0x00,0x5a,/*msg element length*/
		0x00,0x85,
		0x00,/*flags*/
		0x04,0x00,/*message element type   1024*/
		0x00,0x53,/*message element length*/
		0x00,/*radio ID*/
		0x01,/*wlan ID*/
		0x00,0x05,/*capabilities*/
		0x00,/*key index*/
		0x00,/*key status*/
		0x01,0x00,/*key length*/
		0x88,0x5b,0xa0,0x00,0x00,0x00,0x04,0x00,
		0x01,0xe2,0x40,0x00,0x01,0x00,0x04,0x00,
		0x01,0xe2,0x40,0x00,0x27,0x00,0x28,0x01,
		0x00,0x0a,0x09,0x00,0x00,0x5b,0xa0,0x00,/*key*/
		0x00,0x00,0x04,0x00,0x01,0xe2,/*group tsc*/
		0xa0,/*qos*/
		0x00,/*auth type*/
		0x00,/*mac mode 0 local 1 split 2*/
		0x00,/*tunnel mode 0 local bridge 1 802.3 2 802.11 2*/
		0x04,/*supress ssid*/
		0x41,0x55,0x54,0x45,0x4c,0x41,0x4e,0x21,	/*ssid*/
		0x41,0x55,0x54,0x45,0x4c,0x41,0x4e,0x21,
		0x41,0x55,0x54,0x45,0x4c,0x41,0x4e,0x21,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		//0x41,0x55,0x54,0x45,0x4c,0x41,0x4e,0x21,

#if 0
		0x04,0x05,/*message element type*/                               //peiwenhui add 0605
		0x00,0x27,/*message element length*/
		0x00,/*radio_id*/
		0x01,/*wlan_id*/
		0x00,/*flags*/
		0xdd,/*elem_id*/
		0x00,/*len*/
		0x00,0x00,0x00,/*oui[3]*/
		0x00,/*oui_type*/
		0x00,0x00,/*version[2]*/
		0x00,0x00,0x00,0x00,/*proto*/
		0x00,0x00,0x00,0x08,/*pairwise_cipher ---alg*/
		0x00,0x00,0x00,0x08,/*group_cipher*/
		0x00,0x00,0x00,0x02,/*key_mgmt ---auth mode*/
		0x00,0x00,0x00,0x00,/*capabilities*/
		0x00,0x00,0x00,0x00,/*num_pmkid*/
		//0x00,/* *pmkid */
		0x00,0x00,0x00,0x00/*mgmt_group_cipher*/
#endif
	};/*used for create wlan*/
#endif
#if 0
	unsigned char my_buf2[CW_BUFFER_SIZE] = /*test set channel*/
	{
		0x00,0x00,0x00,0x07,               /*control message type  */
		0x04,/*seq num*/
		0x00,0x0f,/*msg element length*/
		0x00,/*flags*/
		0x04,0x04,/*message element type*/
		0x00,0x08,/*message element length*/
		0x00,/*radio ID*/
		0x00,/*reserved*/
		0x0a,/*current channel*/
		0x00,/*CCA*/
		0x00,0x00,0x00,0x00/*energy_detect_threshold*/
	};
#endif
#if 0
	unsigned char my_buf3[CW_BUFFER_SIZE] = /*test set tx power*/
	{
		0x00,0x00,0x00,0x07,               /*control message type  */
		0x04,/*seq num*/
		0x00,0x0b,/*msgs element length*/
		0x00,/*flags*/
		0x04,0x11,/*message element type*/
		0x00,0x04,/*message element length*/
		0x00,/*radio ID*/
		0x00,/*reserved*/
		0x00,0x0a /*current tx power*/
	};
#endif
#if 0
	unsigned char my_buf4[CW_BUFFER_SIZE] =
	{
		0x00,0x33,0xdd,0x00,               /*control message type  */
		0x04,/*seq num*/
		0x00,0x09,/*msg elements length*/
		0x00,/*flags*/
		0x04,0x03,/*message element type*/
		0x00,0x02,/*message element length*/
		0x00,/*radio ID*/
		0x01 /*wlan ID*/
	};
#endif
#if 0/*add sta*/
	unsigned char my_buf5[CW_BUFFER_SIZE] = 
	{
		0x00,0x00,0x00,0x19,               /*control message type  25 */
		0x04,/*seq num*/
		0x00,0x10,/*msgs element length*/
		0x00,/*flags*/
		0x00,0x08,/*message element type 8*/
		0x00,0x09,/*message element length*/	
		0x00,/*radio ID*/
		0x06,/*mac length*/
		0x00,0x19,0xe0,0x81,0x42,0xa0,/*mac addr*/
		0x01 /*wlan ID*/
	};
#endif
#if 0/*set session key*/
	unsigned char my_buf6[CW_BUFFER_SIZE] = 
	{
		0x00,0x00,0x00,0x19,               /*control message type  25 */
		0x04,/*seq num*/
//		0x00,0x20,/*msgs element length*/
		0x00,0x66,/*msgs element length*/
		0x00,/*flags*/
		0x04,0x0e,/*message element type 1038*/
		0x00,0x34,/*message element length*/
		0x00,0x00,0x00,0x00,0x00,0x00,/*mac addr*/
		0x02,0x00,/*flags*/
		0x00,0x00,0x00,0x00,0x00,0x00,/*mac addr*/
		0x00,0x00,0x00,0x00,0x00,0x00,/*mac addr*/
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x42,/*key*/
#if 1
		0x04,0x05,/*message element type        1029*/                       //peiwenhui add 0605
		0x00,0x27,/*message element length*/
		0x00,/*radio_id*/
		0x01,/*wlan_id*/
		0x00,/*flags*/
		0xdd,/*elem_id*/
		0x00,/*len*/
		0x00,0x00,0x00,/*oui[3]*/
		0x00,/*oui_type*/
		0x00,0x00,/*version[2]*/
		0x00,0x00,0x00,0x00,/*proto*/
		0x00,0x00,0x00,0x08,/*pairwise_cipher ---alg*/
		0x00,0x00,0x00,0x08,/*group_cipher*/
		0x00,0x00,0x00,0x02,/*key_mgmt ---auth mode*/
		0x00,0x00,0x00,0x00,/*capabilities*/
		0x00,0x00,0x00,0x00,/*num_pmkid*/
		//0x00,/* *pmkid */
		0x00,0x00,0x00,0x00/*mgmt_group_cipher*/
#endif
	};
#endif
#if 0/*delete sta*/
	unsigned char my_buf7[CW_BUFFER_SIZE] = 
	{
		0x00,0x00,0x00,0x19,               /*control message type  25 */
		0x04,/*seq num*/
		0x00,0x0f,/*msgs element length*/
		0x00,/*flags*/
		0x00,0x12,/*message element type 18*/
		0x00,0x08,/*message element length*/	
		0x00,/*radio ID*/
		0x06,/*mac length*/
		0x00,0x19,0xe0,0x81,0x42,0xa0,/*mac addr*/
	};
#endif

#if 1
/* Begin: gengzj added for wifipos 2013-11-26 */
	testmsg0.msg = pos_buf;
	testmsg0.offset =0;
#endif
#if 0
/* End: gengzj added end */
	testmsg0.msg = my_buf0;
	testmsg0.offset =0;
#endif
#if 0
	testmsg1.msg = my_buf1;
	testmsg1.offset =0;
#endif
#if 0
	testmsg2.msg = my_buf2;
	testmsg2.offset =0;
#endif

#if 0
	testmsg3.msg = my_buf3;
	testmsg3.offset =0;
#endif
#if 0
	testmsg4.msg = my_buf4;
	testmsg4.offset =0;
#endif
#if 0
	testmsg5.msg = my_buf5;
	testmsg5.offset =0;
#endif
#if 0
	testmsg6.msg = my_buf6;
	testmsg6.offset =0;
#endif
#if 0
	testmsg7.msg = my_buf7;
	testmsg7.offset =0;
#endif



#if 1
/* Begin: gengzj added for wifipos 2013-11-26 */
	CWWTPDebugLog("call CWWTPManageGenericRunMessage");
	if (!CWErr(CWWTPManageGenericRunMessage(&testmsg0))) 
	{
				if(CWErrorGetLastErrorCode() == CW_ERROR_INVALID_FORMAT) 
				{
					if(!CWErrorHandleLast())
						CWWTPDebugLog("--> Received something different from a valid Run Message");
				} 
				else 
				{
					CW_FREE_PROTOCOL_MESSAGE(testmsg0);
					CWWTPDebugLog("--> Critical Error Managing Generic Run Message... we enter RESET State");
				}
	}
#endif
#if 0
/* End: gengzj added end */
	/************************WTP Event request del sta***********************/
	if (!CWErr(CWWTPManageGenericRunMessage(&testmsg0))) 
	{
				if(CWErrorGetLastErrorCode() == CW_ERROR_INVALID_FORMAT) 
				{
					// Log and ignore message
					if(!CWErrorHandleLast())
						CWWTPDebugLog("--> Received something different from a valid Run Message");
				} 
				else 
				{
					CW_FREE_PROTOCOL_MESSAGE(testmsg0);
					CWWTPDebugLog("--> Critical Error Managing Generic Run Message... we enter RESET State");
				}
	}
#endif

#if 0

	/************************create wlan***********************/
	if (!CWErr(CWWTPManageGenericRunMessage(&testmsg1))) 
	{
				if(CWErrorGetLastErrorCode() == CW_ERROR_INVALID_FORMAT) 
				{
					// Log and ignore message
					CWErrorHandleLast();
				} 
				else 
				{
					CW_FREE_PROTOCOL_MESSAGE(testmsg1);
				}
	}
#endif
#if 0
	/*************************set channel************************/
	if (!CWErr(CWWTPManageGenericRunMessage(&testmsg2))) 
	{
				if(CWErrorGetLastErrorCode() == CW_ERROR_INVALID_FORMAT) 
				{
					// Log and ignore message
					CWErrorHandleLast();
				} 
				else 
				{
					CW_FREE_PROTOCOL_MESSAGE(testmsg2);
				}
	}
#endif
#if 0
	/*************************set tx power**********************/
	if (!CWErr(CWWTPManageGenericRunMessage(&testmsg3))) 
	{
				if(CWErrorGetLastErrorCode() == CW_ERROR_INVALID_FORMAT) 
				{
					// Log and ignore message
					CWErrorHandleLast();
				} 
				else 
				{
					CW_FREE_PROTOCOL_MESSAGE(testmsg3);
				}
	}
#endif
#if 0
/****************************delete wlan********************************/
	if (!CWErr(CWWTPManageGenericRunMessage(&testmsg4))) 
	{
				if(CWErrorGetLastErrorCode() == CW_ERROR_INVALID_FORMAT) 
				{
					// Log and ignore message
					CWErrorHandleLast();
				} 
				else 
				{
					CW_FREE_PROTOCOL_MESSAGE(testmsg4);
				}
	}
#endif
#if 0
/****************************add sta********************************/
	if (!CWErr(CWWTPManageGenericRunMessage(&testmsg5))) 
	{
				if(CWErrorGetLastErrorCode() == CW_ERROR_INVALID_FORMAT) 
				{
					// Log and ignore message
					CWErrorHandleLast();
				} 
				else 
				{
					CW_FREE_PROTOCOL_MESSAGE(testmsg5);
				}
	}
#endif
#if 0
/****************************set sta session key********************************/
	if (!CWErr(CWWTPManageGenericRunMessage(&testmsg6))) 
	{
				if(CWErrorGetLastErrorCode() == CW_ERROR_INVALID_FORMAT) 
				{
					// Log and ignore message
					CWErrorHandleLast();
				} 
				else 
				{
					CW_FREE_PROTOCOL_MESSAGE(testmsg6);
				}
	}
#endif
#if 0
/****************************delete sta********************************/
	if (!CWErr(CWWTPManageGenericRunMessage(&testmsg7))) 
	{
				if(CWErrorGetLastErrorCode() == CW_ERROR_INVALID_FORMAT) 
				{
					// Log and ignore message
					CWErrorHandleLast();
				} 
				else 
				{
					CW_FREE_PROTOCOL_MESSAGE(testmsg7);
				}
	}
#endif



#endif
}
CWBool CWWTPManageGenericRunMessage(CWProtocolMessage *msgPtr)
{
	CWControlHeaderValues controlVal;
	unsigned char created_radio_id,created_wlan_id; 
//	int i;/*for debug*/
	static unsigned char run_seqnum = 0;
	
	if(msgPtr == NULL) 
		return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	
	msgPtr->offset = 0;
	
	if(!(CWParseControlHeader(msgPtr, &controlVal))) 
		return CW_FALSE; // will be handled by the caller	
	echo_retran_count=0;

	int len=controlVal.msgElemsLen-CW_CONTROL_HEADER_OFFSET_FOR_MSG_ELEMS;
	gRunRetry = 0;
	if(run_seqnum==controlVal.seqNum){
		CWWTPDebugLog("###RUN state recv same msg        msg type=%d! ",controlVal.messageTypeValue);
		gRunRetry = 1;
	}
	else{
		if(controlVal.messageTypeValue !=10 && controlVal.messageTypeValue !=14)//for gRunRetry, except "echo response" and "wtp event response" 
			run_seqnum=controlVal.seqNum;
	}
	int pendingMsgIndex = -1;
//	pendingMsgIndex = CWFindPendingRequestMsgsBox(gPendingRequestMsgs, MAX_PENDING_REQUEST_MSGS, controlVal.messageTypeValue, controlVal.seqNum);
	if(pendingMsgIndex <0) //## we have received a new Request or an Echo Response
	{
		CWProtocolMessage *messages = NULL;
		int fragmentsNum=0;
		CWBool toSend=CW_FALSE;
		
	
		switch(controlVal.messageTypeValue) 
		{
			case CW_MSG_TYPE_VALUE_CONFIGURE_UPDATE_REQUEST:
			{				
				CWWTPDebugLog("received configure update request,Message Type: 7");
				CWProtocolResultCode resultCode = CW_PROTOCOL_FAILURE;
				CWProtocolConfigurationUpdateRequestValues values;				
				memset(&values,0,sizeof(CWProtocolConfigurationUpdateRequestValues)); //  zengmin initializer values by Coverity 2013-06-09
				unsigned char radio_id;
				unsigned char oprat_state_cause;
				unsigned char protected_flag = 0;
				
				CWResetTimers();

				if(!CWParseConfigurationUpdateRequest((msgPtr->msg)+(msgPtr->offset), len, &values,&protected_flag)) 
					return CW_FALSE;
				
				radio_id = values.adminState->radio_id;
				
				if(!CWSaveConfigurationUpdateRequest(&values, &resultCode, &oprat_state_cause,radio_id,protected_flag))
					return CW_FALSE;

				if(!CWAssembleConfigurationUpdateResponse(&messages, &fragmentsNum, gWTPPathMTU, controlVal.seqNum, resultCode, radio_id, oprat_state_cause)) 
					return CW_FALSE;

				toSend=CW_TRUE;
				//if(0 == gRunRetry)
					//CWSaveConfigInfo();
				break;
			}

			case CW_MSG_TYPE_VALUE_CLEAR_CONFIGURATION_REQUEST:
			{				
				CWWTPDebugLog("received clear configure request,Message Type: 23");
				CWResetTimers();
				break;
			}

			case CW_MSG_TYPE_VALUE_STATION_CONFIGURATION_REQUEST:
			{
				CWWTPDebugLog("received station configure request,Message Type: 25");
				CWProtocolResultCode resultCode = CW_PROTOCOL_FAILURE;
				CWProtocolConfigurationUpdateRequestValues values;/*now use this struct because it works ok I wiil change it'name later*/				
				memset(&values,0,sizeof(CWProtocolConfigurationUpdateRequestValues)); //  zengmin initializer values by Coverity 2013-06-13
				CWResetTimers();
				if(!CWParseSTAConfigurationRequest((msgPtr->msg)+(msgPtr->offset), len, &values))
					return CW_FALSE;
				if(!CWSaveSTAConfigurationRequest(&values, &resultCode))
					return CW_FALSE;

				if(!CWAssembleSTAConfigurationResponse(&messages,&fragmentsNum, gWTPPathMTU, controlVal.seqNum,resultCode)) 
					return CW_FALSE;
				toSend = CW_TRUE;

				break;
			}

			case CW_MSG_TYPE_VALUE_ECHO_RESPONSE:
			{
				//CWWTPDebugLog("received echo response,Message Type: 14");
				dpf("received echo response,Message Type: 14\n");
				CWResetTimers();
				break;
			}
			case CW_MSG_TYPE_VALUE_IEEE80211_WLAN_CONFIGURATION_REQUEST:
			{
				CWWTPDebugLog("received wlan configuration request,Message Type: 3398912");
				CWProtocolResultCode resultCode = CW_PROTOCOL_FAILURE;
				CWProtocolConfigurationUpdateRequestValues values;/*I wiil add a new struct if it needs*/
				memset(&values,0,sizeof(CWProtocolConfigurationUpdateRequestValues)); //  zengmin initializer values by Coverity 2013-06-13
				CWResetTimers();
				if(!CWParseIEEE80211WlanConfigurationRequest((msgPtr->msg)+(msgPtr->offset), len, &values))
					return CW_FALSE;
				/*
				*zengmin del sta_access_limit 2013-05-02
				_system("killall sta_access_limit");//yuan add sta access limit
				_system("sta_access_limit &");//yuan add sta access limit
				*/
				if(!CWSaveIEEE80211WlanConfigurationRequest(&values, &created_radio_id, &created_wlan_id,&resultCode))
					return CW_FALSE;

				if(!CWAssembleIEEE80211WlanConfigurationResponse(&messages, &values ,&fragmentsNum, gWTPPathMTU, controlVal.seqNum, created_radio_id, created_wlan_id,resultCode)) 
					return CW_FALSE;

				toSend = CW_TRUE;
				//if(0 == gRunRetry)
//					CWSaveConfigInfo_old();
				break;
			}
			
#if 0     //pei add 0703
			case CW_MSG_TYPE_VALUE_WTP_EVENT_REQUEST:
			{
				int seqNum;
				DeleteSTAValues values;
				CWResetTimers();

				if(!CWParseWTPEventRequestDeleteStationMessage((msgPtr->msg)+(msgPtr->offset), len, &values))
					return CW_FALSE;
				seqNum = CWGetSeqNum();
				if(!CWAssembleWTPEventDeleteStationRequest(&messages, &fragmentsNum, gWTPPathMTU, seqNum, &values)) 
					return CW_FALSE;
//				CWCaptrue(28, messages->msg);
				toSend = CW_TRUE;
				break;

			}
#endif
			case CW_MSG_TYPE_VALUE_WTP_EVENT_RESPONSE:
			{
				//CWWTPDebugLog("received wtp event response,Message Type: 10");
				dpf("received wtp event response,Message Type: 10\n");
				CWResetTimers();
#ifdef AUTELAN_SOLUTION2  //zengmin add  20121230				
				CWDeleteRtransmMsgList(&gRtransMsgList,controlVal.seqNum,1);
#endif
				break;
			}

			default:
				CWWTPDebugLog("received unknow request,Message Type: %u",controlVal.messageTypeValue);
				//## We can't recognize the received Request so we have to send
				//## a corresponding response containing a failure result code
				CWResetTimers();
				if(!(CWAssembleUnrecognizedMessageResponse(&messages, &fragmentsNum, gWTPPathMTU, controlVal.seqNum, controlVal.messageTypeValue+1))) 
					return CW_FALSE;
				toSend = CW_TRUE;
				//return CWErrorRaise(CW_ERROR_INVALID_FORMAT, "Received Message not valid in Run State");
		}
		
		if(toSend)
		{
//			CWLog("SEND A RESPONSE IN RUN STATE");
			int i;
			for(i = 0; i < fragmentsNum; i++) 
			{

#ifndef CW_NO_DTLS
				if(gDtlsSecurity == 1)
				{
					if(!CWSecuritySend(gWTPSession, messages[i].msg, messages[i].offset))
					{
						CWFreeMessageFragments(messages, fragmentsNum);
						CW_FREE_OBJECT(messages);
						return CW_FALSE;
					}
				}
				else
#endif
				{
					if (!CWNetworkSendUnsafeUnconnected(gWTPSocket, &(gACInfoPtr->preferredAddress), messages[i].msg, messages[i].offset))
					{
						CWFreeMessageFragments(messages, fragmentsNum);
						CW_FREE_OBJECT(messages);
						return CW_FALSE;
					}
				}
			}
			CWWTPDebugLog("send control msg response success");
			CWFreeMessageFragments(messages, fragmentsNum);
			CW_FREE_OBJECT(messages);
		}	
	} 
//zengmin add by Coverity 2013-06-09	
#if 0	
	else //## we have received a Response
	{
		CWResetTimers();
		switch(controlVal.messageTypeValue) 
		{
			case CW_MSG_TYPE_VALUE_CHANGE_STATE_EVENT_RESPONSE:
				break;
		
			case CW_MSG_TYPE_VALUE_WTP_EVENT_RESPONSE:
				break;
	
			case CW_MSG_TYPE_VALUE_DATA_TRANSFER_RESPONSE:
				break;

			default:
				//## We can't recognize the received Response: we ignore the
				//## message and log the event.
				return CWErrorRaise(CW_ERROR_INVALID_FORMAT, "Received Message not valid in Run State");
		}
	
//		CWResetPendingMsgBox(&(gPendingRequestMsgs[pendingMsgIndex]));
	}
#endif
	CW_FREE_PROTOCOL_MESSAGE(*msgPtr);
	return CW_TRUE;
}


/*______________________________________________________________*/
/*  *******************___TIMER HANDLERS___*******************  */
void CWWTPHeartBeatTimerExpiredHandler(void *arg) 
{
	CWList msgElemList = NULL;
	CWProtocolMessage *messages = NULL;
	int fragmentsNum = 0;
	int seqNum;
	int send_echo_error = 0;

	if(!gNeighborDeadTimerSet) {
		if (!CWStartNeighborDeadTimer()) {
			CWStopHeartbeatTimer();  /* fix bug, capwap0.93.3 */
			CWStopNeighborDeadTimer();  /* fix bug, capwap0.93.3 */
			return;
		}
	}
	if(!CWStartHeartbeatTimer()) {
		CWWTPDebugLog("CWStartHeartbeatTimer failed");
		return;
	}


	/* Send WTP Event Request */
	seqNum = CWGetSeqNum();

	if(!CWAssembleEchoRequest(&messages, &fragmentsNum, gWTPPathMTU, seqNum, msgElemList)){
		int i;
		CWWTPDebugLog("Assemble echo requst failed!");
		if(messages)
			for(i = 0; i < fragmentsNum; i++) {
				CW_FREE_PROTOCOL_MESSAGE(messages[i]);
			}	
		CW_FREE_OBJECT(messages);
		return;
	}
	
	int i;
	for(i = 0; i < fragmentsNum; i++) {

#ifndef CW_NO_DTLS
		if(gDtlsSecurity == 1)
		{
			if(!CWSecuritySend(gWTPSession, messages[i].msg, messages[i].offset))
			{
				int k;
				for(k = 0; k < fragmentsNum; k++) {
					CW_FREE_PROTOCOL_MESSAGE(messages[k]);
				}	
				CW_FREE_OBJECT(messages);
				send_echo_error = 1;
				break;
			}
		}
		else
#endif
		{
			if (!CWNetworkSendUnsafeUnconnected(gWTPSocket, &(gACInfoPtr->preferredAddress), messages[i].msg, messages[i].offset))
			{
				int k;
				for(k = 0; k < fragmentsNum; k++) {
					CW_FREE_PROTOCOL_MESSAGE(messages[k]);
				}	
				CW_FREE_OBJECT(messages);
				send_echo_error = 1;
				break;
			}
		}
	}
	if(send_echo_error != 1){
		int k;
		for(k = 0; k < fragmentsNum; k++) {
			CW_FREE_PROTOCOL_MESSAGE(messages[k]);
		}
		if(echo_retran_count)
			CWWTPDebugLog("send echo requst success!,echo no response count:%d",echo_retran_count+1);
		else
			dpf("send echo requst success!\n");
		
		echo_retran_count++;
		CW_FREE_OBJECT(messages);
	}
	else
	{
		CWWTPDebugLog("send echo requst failed!");
		echo_retran_count++;
	}

}

/*Begin:zeng add for save wtpd log when wtpd restart 2013-01-10*/
void CWSaveWtpLog(void)    
{       
    char cmdbuf[128];       
    if(0 != access("/jffs/last_log", F_OK))       
    {       
        if(0 != mkdir("/jffs/last_log",S_IRWXU))
			CWWTPDebugLog("create path:/jffs/last_log error!");
    }       
    memset(cmdbuf,0,128);
    autelan_sprintf(cmdbuf,"tail -100 /tmp/wtpd.log > /jffs/last_log/last_wtpd_log");       
    _system(cmdbuf);       
           
    memset(cmdbuf,0,128);
    autelan_sprintf(cmdbuf,"cp /tmp/echo_time /jffs/last_log/last_echo_time");       
    _system(cmdbuf);       
        
    if(access("/tmp/cpu_ratio", 0) == 0)       
    {       
        memset(cmdbuf,0,128);       
        autelan_sprintf(cmdbuf,"cp /tmp/cpu_ratio /jffs/last_log/last_cpu_ratio");       
        _system(cmdbuf);                   
    }       
    if(access("/proc/net/capwap_record", 0) == 0)       
    {       
        memset(cmdbuf,0,128);       
        autelan_sprintf(cmdbuf,"cp /proc/net/capwap_record /jffs/last_log/last_capwap_record");       
        _system(cmdbuf);                   
    }      
	
    return;       
}
/*Begin:zeng add for save wtpd log when wtpd restart 2013-01-10*/

void CWWTPNeighborDeadTimerExpired(void *arg) 
{	
	if(gEMenuConf.e_swtich == 1){
		CWLog("WTP NeighborDead Timer Expired ... e-menu is enable, switch to Fat.");
		CWPingTest("/jffs/pingtest");
		CWSaveWtpLog();	
		WTPInitConfig();
		_system("sys_ToFat &");
		exit(0);
	}
	else if(gEMenuConf.e_swtich == 0){
		int ping_lost = 0;
		ping_lost = CWPingTest("/jffs/pingtest1");
		if(ping_lost == 0){	
			CWLog("WTP NeighborDead Timer Expired ... ping test result:lost %d%% ,wtpd restart",ping_lost);
			_system("touch /tmp/sysrebootflag"); //zeng add for fix report cold boot bug 2013-01-10
			CWSaveWtpLog(); //zeng add for save log 2013-01-10
			WTPInitConfig();			
			_system("/usr/sbin/wtpd_check &");
			_system("wtpd > /dev/null 2>&1 &");
			_system("/usr/sbin/ap_monitor &");			
			exit(0);
		}
		else
		{		
			CWLog("WTP NeighborDead Timer Expired ... ping test result:lost %d%% ,ap sysreboot",ping_lost);
			kes_debug("WTP NeighborDead Timer Expired...sysreboot");
			_system("echo a > /proc/kes_debug_flag");
			CWSaveWtpLog();
			_system("/usr/sbin/ac-lose");
			_system("sysreboot");
		}
	}
	return;  /* add, when compare with capwap 0.93.3 */
}


CWBool CWStartHeartbeatTimer(){
	if ((gCWHeartBeatTimerID = timer_add(gEchoInterval, 0, &CWWTPHeartBeatTimerExpiredHandler, NULL)) == -1 ) {
		return CW_FALSE;
	}
	

	return CW_TRUE;
}


CWBool CWStopHeartbeatTimer(){
	
	timer_rem(gCWHeartBeatTimerID,0);

	
	return CW_TRUE;
}


CWBool CWStartNeighborDeadTimer(){
	
	if ((gCWNeighborDeadTimerID = timer_add(gCWNeighborDeadInterval, 0, &CWWTPNeighborDeadTimerExpired, NULL)) == -1) {
		return CW_FALSE;
	}
	gNeighborDeadTimerSet = CW_TRUE;
	return CW_TRUE;
}


CWBool CWStopNeighborDeadTimer(){
	
	timer_rem(gCWNeighborDeadTimerID,0);


	gNeighborDeadTimerSet = CW_FALSE;
	return CW_TRUE;
}


CWBool CWResetTimers(){
#if 0
	time_t now;
	char *nowReadable = NULL;
	char cmdbuf[64];
	
	now = time(NULL);
	nowReadable = ctime(&now);	
	nowReadable[strlen(nowReadable)-1] = '\0';
	sprintf(cmdbuf, "echo 'receive last msg at: [%s]' > /jffs/lastmsgtime", nowReadable);
	_system(cmdbuf);
#endif
	
	if(0 ==access("/tmp/neighbordead", 0))
	{
		CWLog("WTP receive a msg from AC, we consider WTP works well.");
		_system("rm -rf /tmp/neighbordead");
	}
	
	autelan_system("echo 0 > /proc/sys/dev/wifi0/thinap_check_timer");
	time_now();
	if(gNeighborDeadTimerSet) {
		if (!CWStopNeighborDeadTimer()) return CW_FALSE;
	}
	if(!CWStopHeartbeatTimer()) {
		CWWTPDebugLog("WTP Stop HeartbeatTime error!");
		return CW_FALSE;
	}
	if(!CWStartHeartbeatTimer()) {
		CWWTPDebugLog("WTP Start HeartbeatTime error!");
		return CW_FALSE;
	}
	return CW_TRUE;
}
CWBool CWGetInterfaceCount(int *count, char *type){
	char cmd_n[100];
	char bfr[10];
	
	memset(cmd_n, 0, 100);
	autelan_sprintf(cmd_n, "cat /proc/net/dev | awk -F \" \" '/%s:/' | wc -l", type);
	PopenFile(cmd_n, bfr, sizeof(bfr));
	*count = atoi(bfr);
	return CW_TRUE;
}
/*__________________________________________________________________*/
/*  *******************___ASSEMBLE FUNCTIONS___*******************  */
CWBool CWAssembleEchoRequest (CWProtocolMessage **messagesPtr, int *fragmentsNumPtr, int PMTU, int seqNum, CWList msgElemList) 
{
	CWProtocolMessage *msgElems= NULL;
#if 1  //pei add 0205
	int msgElemCount = 0;
#else
	const int msgElemCount = 0;
#endif
	CWProtocolMessage *msgElemsBinding= NULL;
	const int msgElemBindingCount=0;
	
	if(messagesPtr == NULL || fragmentsNumPtr == NULL) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	
//	CWLog("Assembling Echo Request...");
#if 0  //pei add 0205
	if(gAPThroughputInfoCollectEnable)
	{
		int i=0;
		int interface_count = 0;
		int wlan_count=0;
		int eth_count = 0;
		int wifi_count = 0;
		wlan_stats_info *valuesPtr;
		
		wlan_count = wtp_wlan_count;
		if(wlan_count>16){
			goto exit;
		}

		/* get eth and wifi count */
		CWGetInterfaceCount(&eth_count, "eth[0-9]");
		CWGetInterfaceCount(&wifi_count, "wifi[0-9]");
		interface_count = wlan_count+eth_count+wifi_count;
		dpf("get interface_count:%d\n", interface_count);
		
		CW_CREATE_ARRAY_ERR(valuesPtr, interface_count, wlan_stats_info, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
		for(i=0;i<interface_count;i++)
		{
			memset(&valuesPtr[i], 0, sizeof(wlan_stats_info));
		}

		if(!CWGetAPThroughputInfo(wlan_count, eth_count, wifi_count, valuesPtr)){
			printf("get wlan throughput info error, no wlan exit\n");
			CW_FREE_OBJECT(valuesPtr);
			goto exit;
		}

		msgElemCount = 1;
		CW_CREATE_PROTOCOL_MSG_ARRAY_ERR(msgElems, msgElemCount, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););	

		// Assemble Message Elements
		if (!(CWAssembleMsgElemVendorSpecificPayload(msgElems, interface_count, valuesPtr))) {
			printf("Assemble MsgElem Vendor Specific Payload error\n");
			CW_FREE_OBJECT(valuesPtr);
			CW_FREE_OBJECT(msgElems);
			msgElemCount = 0;
			goto exit;
		}
		CW_FREE_OBJECT(valuesPtr);
	}
exit:
#endif
	if(!(CWAssembleMessage(messagesPtr,
			       fragmentsNumPtr,
			       PMTU,
			       seqNum,
			       CW_MSG_TYPE_VALUE_ECHO_REQUEST,
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
	if(echo_retran_count)
		CWWTPDebugLog("Assemble echo requst success!");
	else
		dpf("Assemble echo requst success!\n");
	return CW_TRUE;
}

CWBool CWAssembleWTPEventDeleteStationRequest (CWProtocolMessage **messagesPtr, int *fragmentsNumPtr, int PMTU, int seqNum, DeleteSTAValues *valuesPtr) 	//pei add 0703
{
	CWProtocolMessage *msgElems= NULL;
	const int msgElemCount = 1;
	CWProtocolMessage *msgElemsBinding= NULL;
	const int msgElemBindingCount=0;
	
	if(messagesPtr == NULL || fragmentsNumPtr == NULL) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);

	CW_CREATE_PROTOCOL_MSG_ARRAY_ERR(msgElems, msgElemCount, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););	
	

	// Assemble Message Elements
	if (!(CWAssembleMsgElemDeleteStation(msgElems, valuesPtr))) {
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
	
	dpf("Assemble wtp event netlink del sta info to AC success!\n");
	
	return CW_TRUE;
}

CWBool CWAssembleWTPEventRequest(CWProtocolMessage **messagesPtr, int *fragmentsNumPtr, int PMTU, int seqNum, CWList msgElemList)
{
	CWProtocolMessage *msgElems= NULL;
	int msgElemCount = 0;
	CWProtocolMessage *msgElemsBinding= NULL;
	const int msgElemBindingCount=0;
	int i;
	CWListElement *current;
	int k = -1;

	if(messagesPtr == NULL || fragmentsNumPtr == NULL || msgElemList == NULL) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	
	msgElemCount = CWCountElementInList(msgElemList);

	if (msgElemCount > 0) {
		CW_CREATE_PROTOCOL_MSG_ARRAY_ERR(msgElems, msgElemCount, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	} 
	else msgElems = NULL;
		

	current=msgElemList;
	for (i=0; i<msgElemCount; i++)
	{
		switch (((CWMsgElemData *) current->data)->type)
		{
			case CW_MSG_ELEMENT_CW_DECRYPT_ER_REPORT_CW_TYPE:
				if (!(CWAssembleMsgElemDecryptErrorReport(&(msgElems[++k]), ((CWMsgElemData *) current->data)->value)))
					goto cw_assemble_error;	
				break;
			case CW_MSG_ELEMENT_DUPLICATE_IPV4_ADDRESS_CW_TYPE:
				if (!(CWAssembleMsgElemDuplicateIPv4Address(&(msgElems[++k]))))
					goto cw_assemble_error;
				break;
			case CW_MSG_ELEMENT_DUPLICATE_IPV6_ADDRESS_CW_TYPE:
				if (!(CWAssembleMsgElemDuplicateIPv6Address(&(msgElems[++k]))))
					goto cw_assemble_error;
				break;
			case CW_MSG_ELEMENT_WTP_OPERAT_STATISTICS_CW_TYPE:
				if (!(CWAssembleMsgElemWTPOperationalStatistics(&(msgElems[++k]), ((CWMsgElemData *) current->data)->value)))
					goto cw_assemble_error;
				break;
			case CW_MSG_ELEMENT_WTP_RADIO_STATISTICS_CW_TYPE:
				if (!(CWAssembleMsgElemWTPRadioStatistics(&(msgElems[++k]), ((CWMsgElemData *) current->data)->value)))
					goto cw_assemble_error;
				break;
			case CW_MSG_ELEMENT_WTP_REBOOT_STATISTICS_CW_TYPE:
				if (!(CWAssembleMsgElemWTPRebootStatistics(&(msgElems[++k]))))
					goto cw_assemble_error;	
				break;
			default:
				goto cw_assemble_error;
				break;	
		}

		current = current->next;	
	}

	if (!(CWAssembleMessage(messagesPtr,
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

	
	return CW_TRUE;

cw_assemble_error:
	{
		int i;
		for(i = 0; i <= k; i++) { CW_FREE_PROTOCOL_MESSAGE(msgElems[i]);}
		CW_FREE_OBJECT(msgElems);
		return CW_FALSE; // error will be handled by the caller
	}
}

CWBool CWAssembleMsgElemRadioOperState(CWProtocolMessage *msgPtr, unsigned char radio_id, unsigned char operat_state_cause)
{
	if(msgPtr == NULL) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	
	// create message
	CW_CREATE_PROTOCOL_MESSAGE(*msgPtr, 3, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	
	CWProtocolStore8(msgPtr, radio_id);
	CWProtocolStore8(msgPtr, gRadioInfoValue[radio_id].gIsRadioEnable);
	CWProtocolStore8(msgPtr, operat_state_cause);
//	dpf("radio_id = %d gIsRadioEnable = %d operat_state_cause = %d\n", radio_id, gRadioInfoValue[radio_id].gIsRadioEnable, operat_state_cause);
				
	return CWAssembleMsgElem(msgPtr, CW_MSG_ELEMENT_RADIO_OPERAT_STATE_CW_TYPE);
}

CWBool CWAssembleConfigurationUpdateResponse(CWProtocolMessage **messagesPtr, int *fragmentsNumPtr, int PMTU, int seqNum, CWProtocolResultCode resultCode, unsigned char radio_id, unsigned char operat_state_cause) 
{
	CWProtocolMessage *msgElems= NULL;
	const int msgElemCount = 2;
	CWProtocolMessage *msgElemsBinding= NULL;
	const int msgElemBindingCount=0;
	int k = -1;
	
	if(messagesPtr == NULL || fragmentsNumPtr == NULL) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	
	
	CW_CREATE_PROTOCOL_MSG_ARRAY_ERR(msgElems, msgElemCount, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););

	if (!(CWAssembleMsgElemRadioOperState(&(msgElems[++k]), radio_id, operat_state_cause))||
		!(CWAssembleMsgElemResultCode(&(msgElems[++k]), resultCode))) {
		int i;
		for(i = 0; i <= k; i++) { CW_FREE_PROTOCOL_MESSAGE(msgElems[i]);}
		CW_FREE_OBJECT(msgElems);
		return CW_FALSE;
	}

	if(!(CWAssembleMessage(messagesPtr,
			       fragmentsNumPtr,
			       PMTU,
			       seqNum,
			       CW_MSG_TYPE_VALUE_CONFIGURE_UPDATE_RESPONSE,
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
	
	CWWTPDebugLog("Assemble configure update response success!");
	return CW_TRUE;
}
/*_______________________________________________________________*/
/*  *******************___PARSE FUNCTIONS___*******************  */
CWBool CWParseConfigurationUpdateRequest (char *msg, int len, CWProtocolConfigurationUpdateRequestValues *valuesPtr,unsigned char *protected_flag) 
{
	CWBool bindingMsgElemFound=CW_FALSE;
	CWProtocolMessage completeMsg;
	
	if(msg == NULL || valuesPtr == NULL) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	
	
	completeMsg.msg = msg;
	completeMsg.offset = 0;

	valuesPtr->timeStampCount = 0;
	valuesPtr->timeStampValue = 0;
	valuesPtr->adminStateCount = 0;
	valuesPtr->adminState = NULL;
	valuesPtr->WTPStaticIPCount = 0;
	valuesPtr->WTPStaticIP = NULL;
	valuesPtr->WTPStaticIPV2Count = 0;
	valuesPtr->WTPStaticIPV2 = NULL;
	valuesPtr->WTPModeNParaCount =0;
	valuesPtr->WTPModeNPara = NULL;
	valuesPtr->apScanningSetCount = 0;
	valuesPtr->apScanningSet = NULL;
	valuesPtr->staThroughputSetCount = 0;
	valuesPtr->staThroughputSet = NULL;
	valuesPtr->apThroughputSetCount = 0;
	valuesPtr->apThroughputSet = NULL;
	valuesPtr->maxThroughputSetCount = 0;
	valuesPtr->maxThroughputSet = NULL;
	valuesPtr->cmdLength = 0;
	valuesPtr->cmd = NULL;
	valuesPtr->apMonitorSetCount = 0;
	valuesPtr->apMonitorSet = NULL;
	valuesPtr->extraInfoCount = 0;
	valuesPtr->extraInfo = NULL;
	valuesPtr->getStaInfoCount = 0;
	valuesPtr->getStaInfo = NULL;
	valuesPtr->getIfStateCount = 0;
	valuesPtr->getIfState = NULL;
	valuesPtr->getAttackDetectInfoCount = 0;
	valuesPtr->getAttackDetectInfo = NULL;
	valuesPtr->getStaWapiInfoCount = 0;
	valuesPtr->getStaWapiInfo = NULL;
	valuesPtr->getNtpCount = 0;      
	valuesPtr->getNtpInfo = NULL;
	valuesPtr->getTerminalDisturbCount = 0;      
	valuesPtr->getTerminalDisturbInfo = NULL;
	valuesPtr->getMultiUserPerformanceCount = 0;      
	valuesPtr->getMultiUserPerformance = NULL;
	valuesPtr->getMcsMaskCount = 0;      
	valuesPtr->getMcsMask = NULL;
	valuesPtr->eth_mtu_count = 0;
	valuesPtr->eth_mtu_info = NULL;
	valuesPtr->time_count= 0;
	valuesPtr->time_info= NULL;
	valuesPtr->bindingValues = NULL;
	valuesPtr->ProbeRequestInhibition_count = 0;
	valuesPtr->ProbeRequestInhibition_value = NULL;
	valuesPtr->CastIsolation_CastRate_count = 0;
	valuesPtr->CastIsolation_CastRate_value =NULL;
	valuesPtr->getStaPktsStatisticRptCount = 0;
	valuesPtr->getStaPktsStatisticRpt = NULL;
	valuesPtr->getACLList_count = 0;
	valuesPtr->getACLList = NULL;
	valuesPtr->eMenuConf_count = 0;
	valuesPtr->eMenuConf = NULL;
	valuesPtr->deauth_disassoc_report_count  = 0;
	valuesPtr->disassocDeauthValues = NULL;
	valuesPtr->detail_all_sta_info_count = 0;
	valuesPtr->detailAllStaInfoValues = NULL;
	valuesPtr->two_radio_5g_priority_access = 0;
	valuesPtr->twoRadio5GPriorityAccess = NULL;
	valuesPtr->getApPktsStatisticRptCount = 0;//caidh add 2013.02.16
	valuesPtr->getApPktsStatisticRpt = NULL;//caidh add 2013.02.16
/* Begin: gengzj added for wifipos 2013-11-26 */
	valuesPtr->WifiScanAndPosCount = 0;
	valuesPtr->WifiScanAndPosVal = NULL;
/* End: gengzj added end */

	// parse message elements
	while(completeMsg.offset < len) {
		unsigned short int elemType=0;// = CWProtocolRetrieve32(&completeMsg);
		unsigned short int elemLen=0;// = CWProtocolRetrieve16(&completeMsg);
		
		CWParseFormatMsgElem(&completeMsg,&elemType,&elemLen);		

		
		if(CWBindingCheckType(elemType))
		{
			bindingMsgElemFound=CW_TRUE;
			completeMsg.offset += elemLen;
			continue;	
		}
		
		switch(elemType) {
			case CW_MSG_ELEMENT_TIMESTAMP_CW_TYPE:      //pei add 0218
				(valuesPtr->timeStampCount)++; // just count 
				completeMsg.offset += elemLen;
				break;
			case CW_MSG_ELEMENT_RADIO_ADMIN_STATE_CW_TYPE:      //pei add 0724
				(valuesPtr->adminStateCount)++; // just count 
				completeMsg.offset += elemLen;
				break;
			case CW_MSG_ELEMENT_WTP_STATIC_IP_CW_TYPE:      //pei add 0214
				(valuesPtr->WTPStaticIPCount)++; // just count 
				completeMsg.offset += elemLen;
				break;
			case CW_MSG_ELEMENT_WTP_STATIC_IP_CW_TYPE_V2:      //pei add 0214
				(valuesPtr->WTPStaticIPV2Count)++; // just count 
				completeMsg.offset += elemLen;
				break;
			case CW_MSG_ELEMENT_ETH_SET_CW_TYPE: {
				unsigned char eth_type1;
				eth_type1 = CWProtocolRetrieve8(&completeMsg);
				switch(eth_type1){
					case 1:
						(completeMsg.offset)-=1;
						(valuesPtr->eth_mtu_count)++;// just count 
						completeMsg.offset += elemLen;
						break;
					default:
						(completeMsg.offset)-=1;
						completeMsg.offset +=elemLen;
						break;
					}
				break;
				}
			case CW_MSG_ELEMENT_ALL_TIME_SET_CW_TYPE: {
				unsigned char time_type;
				time_type = CWProtocolRetrieve8(&completeMsg);
				switch(time_type){
					case 1:
						(completeMsg.offset)-=1;
						(valuesPtr->time_count)++;// just count 
						completeMsg.offset += elemLen;
						break;
					default:
						(completeMsg.offset)-=1;
						completeMsg.offset +=elemLen;
						break;
					}
				break;
				}
			case CW_MSG_ELEMENT_WTP_11N_MCS_CWMODE_GUARDINTERVAL:
				(valuesPtr->WTPModeNParaCount)++; // just count 
				completeMsg.offset += elemLen;
				break;
			case CW_MSG_ELEMENT_VENDOR_SPEC_PAYLOAD_CW_TYPE:{      //pei add 1118
				unsigned char value;
				value = (unsigned char)CWProtocolRetrieve8(&completeMsg);
				if((value >> 7) != 1)
				{
//					if(debug_print)
//					printf("for count the num: value:%d\n", value);
					switch(value){
						case APScanning:
							(valuesPtr->apScanningSetCount)++; //just count
							break;
						case StaThroughputSet:
							(valuesPtr->staThroughputSetCount)++;//just count
							break;
						case APThroughputInfo:
							(valuesPtr->apThroughputSetCount)++; //just count
							break;
						case APMaxThroughput:
							(valuesPtr->maxThroughputSetCount)++; //just count
							break;
						case ExtendCmd:
							break;
						case InterfaceUpdown:
							(valuesPtr->extraInfoCount)++; //just count
							break;
						case APMonitor:
							(valuesPtr->apMonitorSetCount)++; //just count
							break;
						case StaInfo:
							(valuesPtr->getStaInfoCount)++; //just count
							break;
						case InterfaceState:
							(valuesPtr->getIfStateCount)++; //just count
							break;
						case AttackDetectInfo:
							(valuesPtr->getAttackDetectInfoCount)++; //just count
							break;
						case StaWapiInfo:
							(valuesPtr->getStaWapiInfoCount)++; //just count
							break;
						case Ntp:
							(valuesPtr->getNtpCount)++; //just count
							break;
						case TerminalDisturbInfo:
							(valuesPtr->getTerminalDisturbCount)++; //just count
							break;
						case UserPasswd:
							(valuesPtr->userpasswdcount)++; //just count
							break;
						default:
							break;
					}
					(completeMsg.offset)-=1;
					completeMsg.offset +=elemLen;
				}
				else
				{
					completeMsg.offset += 3;
					unsigned short elementid;
					elementid = CWProtocolRetrieve16(&completeMsg);
					switch (elementid){
						case MultiUserPerformance:
							valuesPtr->getMultiUserPerformanceCount ++;
							break;							
						case APMcsMask:
							valuesPtr->getMcsMaskCount ++;
							break;
						case probe_request_inhibition :
							valuesPtr->ProbeRequestInhibition_count ++;
							break;
						case cast_isolation:
							valuesPtr->CastIsolation_CastRate_count ++;
							break;
						case cast_rate_setting:
							valuesPtr->CastIsolation_CastRate_count ++;
							break;
						case VSP_T_STA_PACKET_STATISTIC_REPORT:
							valuesPtr->getStaPktsStatisticRptCount ++;
							break;
						case VSP_T_STA_PACKET_STATISTIC_REPORT_20:
							valuesPtr->getStaPktsStatisticRptCount ++;
							break;
						case VSP_T_SET_ACL_LIST:
							valuesPtr->getACLList_count++;
							break;
						case VSP_T_SET_E_MENU:
							valuesPtr->eMenuConf_count++;
							break;
						case DEAUTH_DISASSOC_REPORT_TO_AC_SWITCH:
							(valuesPtr->deauth_disassoc_report_count) ++;
							break;
						case DETAIL_ALL_STA_INFO_TO_AC_SWITCH:
							(valuesPtr->detail_all_sta_info_count) ++;
							break;
						case ExtendLongCmdForDHCPOption:
							break;
						case Two_radio_5G_priority_access:
							(valuesPtr->two_radio_5g_priority_access)++;
							break;							
						/* Begin: gengzj added for wifipos 2013-11-26 */
						case Set_Wifi_Scanning_and_Positioning:
						    CWWTPDebugLog("set wifi scanning and positioning param count!");
						    (valuesPtr->WifiScanAndPosCount)++;
							break;
						/* End: gengzj added end */

						case VSP_T_AP_PACKET_STATISTIC_REPORT:
							(valuesPtr->getApPktsStatisticRptCount) ++;//caidh add 2013.02.16,get 30 message from AC
							break;
						default:
							break;
					}
					completeMsg.offset +=(elemLen-6);
				}
			}
			break;
				
			default:
//				return CWErrorRaise(CW_ERROR_INVALID_FORMAT, "Unrecognized Message Element");
				CWWTPDebugLog("Unrecognized Message Element:%d, in Configuration Update Request.", elemType);
				completeMsg.offset += elemLen;
				break;
		}
	}
	

/* Begin: gengzj added for wifipos 2013-11-26 */
	CWWTPDebugLog("completeMsg.offset %d",completeMsg.offset);
	CWWTPDebugLog("len%d",len);
/* End: gengzj added end */
	if(completeMsg.offset != len) return CWErrorRaise(CW_ERROR_INVALID_FORMAT, "Garbage at the End of the Message");
	
	// actually read each radio info
	
	CW_CREATE_ARRAY_ERR(valuesPtr->adminState, valuesPtr->adminStateCount, AdminStateValues, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	CW_CREATE_ARRAY_ERR(valuesPtr->WTPStaticIP, valuesPtr->WTPStaticIPCount, WTPStaticIPValues, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	CW_CREATE_ARRAY_ERR(valuesPtr->WTPStaticIPV2, valuesPtr->WTPStaticIPV2Count, WTPStaticIPValues, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	CW_CREATE_ARRAY_ERR(valuesPtr->eth_mtu_info, valuesPtr->eth_mtu_count, set_eth_mtu_vlaues, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	CW_CREATE_ARRAY_ERR(valuesPtr->time_info, valuesPtr->time_count, set_all_time_vlaues, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	CW_CREATE_ARRAY_ERR(valuesPtr->WTPModeNPara, valuesPtr->WTPModeNParaCount, WTPModeNParaValues, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	CW_CREATE_ARRAY_ERR(valuesPtr->apScanningSet, valuesPtr->apScanningSetCount, ApScanningSetValues, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	CW_CREATE_ARRAY_ERR(valuesPtr->staThroughputSet, valuesPtr->staThroughputSetCount, StaThroughputSetValues, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	CW_CREATE_ARRAY_ERR(valuesPtr->apThroughputSet, valuesPtr->apThroughputSetCount, ApThroughputInfoValues, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	CW_CREATE_ARRAY_ERR(valuesPtr->maxThroughputSet, valuesPtr->maxThroughputSetCount, ApMaxThroughputValues, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	CW_CREATE_ARRAY_ERR(valuesPtr->apMonitorSet, valuesPtr->apMonitorSetCount, ApMonitorSetValues, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	CW_CREATE_ARRAY_ERR(valuesPtr->extraInfo, valuesPtr->extraInfoCount, ExtraInfoValues, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	CW_CREATE_ARRAY_ERR(valuesPtr->getStaInfo, valuesPtr->getStaInfoCount, GetStaInfoValues, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	CW_CREATE_ARRAY_ERR(valuesPtr->getIfState, valuesPtr->getIfStateCount, GetIfStateValues, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	CW_CREATE_ARRAY_ERR(valuesPtr->getAttackDetectInfo, valuesPtr->getAttackDetectInfoCount, GetAttackDetectInfoValues, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	CW_CREATE_ARRAY_ERR(valuesPtr->getStaWapiInfo, valuesPtr->getStaWapiInfoCount, GetStaWapiInfoValues, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	CW_CREATE_ARRAY_ERR(valuesPtr->getNtpInfo, valuesPtr->getNtpCount, GetNtpValues, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	CW_CREATE_ARRAY_ERR(valuesPtr->getTerminalDisturbInfo, valuesPtr->getTerminalDisturbCount, GetTerminalDisturbValues, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	CW_CREATE_ARRAY_ERR(valuesPtr->userpasswdinfo, valuesPtr->userpasswdcount, WtpUserPasswdValues, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	CW_CREATE_ARRAY_ERR(valuesPtr->ProbeRequestInhibition_value, valuesPtr->ProbeRequestInhibition_count, ProbeRequestInhibition, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL);)
	CW_CREATE_ARRAY_ERR(valuesPtr->CastIsolation_CastRate_value, valuesPtr->CastIsolation_CastRate_count, CastIsolation_CastRate, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL);)
	CW_CREATE_ARRAY_ERR(valuesPtr->getMultiUserPerformance, valuesPtr->getMultiUserPerformanceCount, GetMultiUserPerformanceValues, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	CW_CREATE_ARRAY_ERR(valuesPtr->getMcsMask, valuesPtr->getMcsMaskCount, GetMcsMaskValues, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	CW_CREATE_ARRAY_ERR(valuesPtr->getStaPktsStatisticRpt,valuesPtr->getStaPktsStatisticRptCount,GetStaPktsStatisticRptValues, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	CW_CREATE_ARRAY_ERR(valuesPtr->getACLList,valuesPtr->getACLList_count,GetACLList,return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	CW_CREATE_ARRAY_ERR(valuesPtr->eMenuConf,valuesPtr->eMenuConf_count,EMenuConf,return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	CW_CREATE_ARRAY_ERR(valuesPtr->disassocDeauthValues,valuesPtr->deauth_disassoc_report_count,DisassocDeauthValues, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	CW_CREATE_ARRAY_ERR(valuesPtr->detailAllStaInfoValues,valuesPtr->detail_all_sta_info_count,DetailAllStaInfoValues, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	CW_CREATE_ARRAY_ERR(valuesPtr->twoRadio5GPriorityAccess,valuesPtr->two_radio_5g_priority_access,TwoRadio5GPriorityAccess, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	CW_CREATE_ARRAY_ERR(valuesPtr->getApPktsStatisticRpt,valuesPtr->getApPktsStatisticRptCount,GetApPktsStatisticRptValues, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););//caidh add 2013.02.16 ,malloc memory
/* Begin: gengzj added for wifipos 2013-11-26 */
	CW_CREATE_ARRAY_ERR(valuesPtr->WifiScanAndPosVal,valuesPtr->WifiScanAndPosCount, WifiScanAndPosParam, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
/* End: gengzj added end */
	//i=0;
	completeMsg.offset = 0;
	while(completeMsg.offset < len) {
		unsigned short int type=0;
		unsigned short int elemLen=0;
		
		CWParseFormatMsgElem(&completeMsg,&type,&elemLen);		

		switch(type) {
			case CW_MSG_ELEMENT_TIMESTAMP_CW_TYPE:{	//pei add 0218
				valuesPtr->timeStampValue = CWProtocolRetrieve32(&completeMsg);
				CWWTPDebugLog("time stamp value:%d", valuesPtr->timeStampValue);
				break;
			}
			case CW_MSG_ELEMENT_RADIO_ADMIN_STATE_CW_TYPE:{	//pei add 0724
				if(!(CWParseRadioAdminState(&completeMsg, elemLen,valuesPtr->adminState)))
				{
					CW_FREE_OBJECT(valuesPtr->adminState);
					return CW_FALSE; // will be handled by the caller
				}
				break;
			}
			case CW_MSG_ELEMENT_WTP_STATIC_IP_CW_TYPE:{	//pei add 0214
				if(!(CWParseWTPStaticIP(&completeMsg, elemLen,valuesPtr->WTPStaticIP)))
				{
					CW_FREE_OBJECT(valuesPtr->WTPStaticIP);
					return CW_FALSE; // will be handled by the caller
				}
				break;
			}
			case CW_MSG_ELEMENT_WTP_STATIC_IP_CW_TYPE_V2:{	//pei add 0214
				if(!(CWParseWTPStaticIPV2(&completeMsg, elemLen,valuesPtr->WTPStaticIPV2)))
				{
					CW_FREE_OBJECT(valuesPtr->WTPStaticIPV2);
					return CW_FALSE; // will be handled by the caller
				}
				break;
			}
			case CW_MSG_ELEMENT_ETH_SET_CW_TYPE:{
				unsigned char eth_type;
				eth_type = CWProtocolRetrieve8(&completeMsg);
				CWWTPDebugLog("eth_type:%d", eth_type);
				switch(eth_type){
					case 1:
						(completeMsg.offset)-=1;
						if(!(acwparse_eth_config_mtu(&completeMsg, elemLen,valuesPtr->eth_mtu_info)))
						{
							CW_FREE_OBJECT(valuesPtr->eth_mtu_info);
							return CW_FALSE; // will be handled by the caller
						}
						break;
					default:
						(completeMsg.offset)-=1;
						completeMsg.offset += elemLen;
						break;
				}
				break;
			}
			case CW_MSG_ELEMENT_ALL_TIME_SET_CW_TYPE:{
				unsigned char time_type;
				time_type = CWProtocolRetrieve8(&completeMsg);
				dpf("time_type:%d\n", time_type);
				switch(time_type){
					case 1:
						(completeMsg.offset)-=1;
						if(!(acwparse_time_config(&completeMsg, elemLen,valuesPtr->time_info)))
						{
							CW_FREE_OBJECT(valuesPtr->time_info);
							return CW_FALSE; // will be handled by the caller
						}
						break;
					default:
						(completeMsg.offset)-=1;
						completeMsg.offset +=elemLen;						
						break;
				}
				break;
			}
			case CW_MSG_ELEMENT_WTP_11N_MCS_CWMODE_GUARDINTERVAL:{
				(*protected_flag)++;
				if(!(CWParseWTPModeNPara(&completeMsg, elemLen,valuesPtr->WTPModeNPara)))
				{
					CW_FREE_OBJECT(valuesPtr->WTPModeNPara);
					return CW_FALSE; // will be handled by the caller
				}
				break;
			}
			case CW_MSG_ELEMENT_VENDOR_SPEC_PAYLOAD_CW_TYPE:{	//pei add 1118
				unsigned char value;
				value = CWProtocolRetrieve8(&completeMsg);
				if((value >> 7) != 1)
				{
					switch(value){
						case APScanning:
							CWWTPDebugLog("get the AP scanning set message");
							(completeMsg.offset)-=1;
							if(!(CWParseApScanningSet(&completeMsg, elemLen,valuesPtr->apScanningSet)))
							{
								CW_FREE_OBJECT(valuesPtr->apScanningSet);
								return CW_FALSE; // will be handled by the caller
							}
							break;
						case StaThroughputSet:
							CWWTPDebugLog("get the STA throughput set message");
							(completeMsg.offset)-=1;
							if(!(CWParseStaThroughputSet(&completeMsg, elemLen,valuesPtr->staThroughputSet)))
							{
								CW_FREE_OBJECT(valuesPtr->staThroughputSet);
								return CW_FALSE; // will be handled by the caller
							}
							break;
						case APThroughputInfo:
							CWWTPDebugLog("get the msg to collect AP throughput info");
							(completeMsg.offset)-=1;
							if(!(CWParseCollectApThroughputInfoSet(&completeMsg, elemLen,valuesPtr->apThroughputSet)))
							{
								CW_FREE_OBJECT(valuesPtr->apThroughputSet);
								return CW_FALSE; // will be handled by the caller
							}
							break;
						case APMaxThroughput:
							CWWTPDebugLog("get the msg to set AP max throughput");
							(completeMsg.offset)-=1;
							if(!(CWParseMaxThroughput(&completeMsg, elemLen, valuesPtr->maxThroughputSet)))
							{
								autelan_printf("runState CWParseMaxThroughput error\n");
								CW_FREE_OBJECT(valuesPtr->maxThroughputSet->wlanMaxThroughput);
								CW_FREE_OBJECT(valuesPtr->maxThroughputSet);
								return CW_FALSE; // will be handled by the caller
							}
							break;
						case ExtendCmd:
							CWWTPDebugLog("get the msg to do the cmd from AC");
							(completeMsg.offset)-=1;
							if(!(CWParseCmdFromAC(&completeMsg, elemLen, valuesPtr)))
							{
								CW_FREE_OBJECT(valuesPtr->cmd);
								return CW_FALSE; // will be handled by the caller
							}
							break;
						case InterfaceUpdown:
							CWWTPDebugLog("get the msg to gather the interface updown time");
							(completeMsg.offset)-=1;
							if(!(CWParseInterfaceUpdownTime(&completeMsg, elemLen, valuesPtr->extraInfo)))
							{
								CW_FREE_OBJECT(valuesPtr->extraInfo);
								return CW_FALSE; // will be handled by the caller
							}
							break;
						case APMonitor:
							CWWTPDebugLog("get the msg to enable monitor");
							(completeMsg.offset)-=1;
							if(!(CWParseMonitorSet(&completeMsg, elemLen, valuesPtr->apMonitorSet)))
							{
								CW_FREE_OBJECT(valuesPtr->apMonitorSet);
								return CW_FALSE; // will be handled by the caller
							}
							break;
						case StaInfo:
							CWWTPDebugLog("get the msg to enable stationInfo");
							(completeMsg.offset)-=1;
							if(!(CWParseGetStaInfo(&completeMsg, elemLen, valuesPtr->getStaInfo)))
							{
								CW_FREE_OBJECT(valuesPtr->getStaInfo);
								return CW_FALSE; // will be handled by the caller
							}
							break;
						case InterfaceState:
							CWWTPDebugLog("get the msg to enable InterfaceState");
							(completeMsg.offset)-=1;
							if(!(CWParseGetIfState(&completeMsg, elemLen, valuesPtr->getIfState)))
							{
								CW_FREE_OBJECT(valuesPtr->getIfState);
								return CW_FALSE; // will be handled by the caller
							}
							break;
						case AttackDetectInfo:
							CWWTPDebugLog("get the msg to enable AttackDetectInfo");
							(completeMsg.offset)-=1;
							if(!(CWParseGetAttackDetectInfo(&completeMsg, elemLen, valuesPtr->getAttackDetectInfo)))
							{
								CW_FREE_OBJECT(valuesPtr->getAttackDetectInfo);
								return CW_FALSE; // will be handled by the caller
							}
							break;
						case StaWapiInfo:
							CWWTPDebugLog("get the msg to enable StaWapiInfo");
							(completeMsg.offset)-=1;
							if(!(CWParseGetStaWapiInfo(&completeMsg, elemLen, valuesPtr->getStaWapiInfo)))
							{
								CW_FREE_OBJECT(valuesPtr->getStaWapiInfo);
								return CW_FALSE; // will be handled by the caller
							}
							break;
						case Ntp:
							CWWTPDebugLog("get the msg to enable Ntp");
							(completeMsg.offset)-=1;
							if(!(CWParseGetNtpInfo(&completeMsg, elemLen, valuesPtr->getNtpInfo)))
							{
								CW_FREE_OBJECT(valuesPtr->getNtpInfo);
								return CW_FALSE; // will be handled by the caller
							}
							break;
						case TerminalDisturbInfo:
							CWWTPDebugLog("get the msg to enable TerminalDisturbInfo");
							(completeMsg.offset)-=1;
							if(!(CWParseGetTerminalDisturbInfo(&completeMsg, elemLen, valuesPtr->getTerminalDisturbInfo)))
							{
								CW_FREE_OBJECT(valuesPtr->getTerminalDisturbInfo);
								return CW_FALSE; // will be handled by the caller
							}
							break;
						case UserPasswd:
							CWWTPDebugLog("get message to set wtp user and passwd");
							(completeMsg.offset)-=1;
							if(!CWParseGetUserandPasswdinfo(&completeMsg,elemLen,valuesPtr->userpasswdinfo))
							{
								CW_FREE_OBJECT(valuesPtr->userpasswdinfo->username);
								CW_FREE_OBJECT(valuesPtr->userpasswdinfo->passwd);
								return CW_FALSE;
							}
							break;
						default:
							(completeMsg.offset)-=1;
							completeMsg.offset += elemLen;
							break;
					
					}
				}
				else
				{
					completeMsg.offset += 3;
					unsigned short element_id, element_len;
					element_id =  CWProtocolRetrieve16(&completeMsg);
					element_len = CWProtocolRetrieve16(&completeMsg);
					switch (element_id){
						case MultiUserPerformance:
							CWWTPDebugLog("get the msg to enable/disable MultiUserPerformance");
							if(!(CWParseGetMultiUserPerformance(&completeMsg, element_len, valuesPtr->getMultiUserPerformance)))
							{
								CW_FREE_OBJECT(valuesPtr->getMultiUserPerformance);
								return CW_FALSE; // will be handled by the caller
							}
							break;
						case APMcsMask:
							CWWTPDebugLog("get the msg to set APMcsMask");
							if(!(CWParseGetMcsMask(&completeMsg, element_len, valuesPtr->getMcsMask)))
							{
								CW_FREE_OBJECT(valuesPtr->getMcsMask);
								return CW_FALSE; // will be handled by the caller
							}
							break;
						case probe_request_inhibition:
							if(!(CWParseProbeRequestInhibition(&completeMsg, element_len, valuesPtr->ProbeRequestInhibition_value)))
							{
								CW_FREE_OBJECT(valuesPtr->ProbeRequestInhibition_value);
								return CW_FALSE; // will be handled by the caller
							}
							break;
						case cast_isolation:
							if(!CWParseCastIsolation(&completeMsg, element_len, valuesPtr->CastIsolation_CastRate_value))
								{
								CW_FREE_OBJECT(valuesPtr->CastIsolation_CastRate_value);
								return CW_FALSE;
							}
							break;
						case cast_rate_setting:
							if(!CWParseCastRate(&completeMsg, element_len, valuesPtr->CastIsolation_CastRate_value))
								{
								CW_FREE_OBJECT(valuesPtr->CastIsolation_CastRate_value);
								return CW_FALSE;
							}
							break;
						case VSP_T_STA_PACKET_STATISTIC_REPORT:
							if(!CWParseGetStaPktsStatisticRpt(&completeMsg, element_len, valuesPtr->getStaPktsStatisticRpt,element_id))
							{
								CW_FREE_OBJECT(valuesPtr->getStaPktsStatisticRpt);
								return CW_FALSE;
							}
							break;
						case VSP_T_STA_PACKET_STATISTIC_REPORT_20:
							if(!CWParseGetStaPktsStatisticRpt(&completeMsg, element_len, valuesPtr->getStaPktsStatisticRpt,element_id))
							{
								CW_FREE_OBJECT(valuesPtr->getStaPktsStatisticRpt);
								return CW_FALSE;
							}
							break;
						case VSP_T_SET_ACL_LIST:
							if(!CWParseGetACLList(&completeMsg, element_len, valuesPtr->getACLList))
							{
								CW_FREE_OBJECT(valuesPtr->getACLList->mac_list);
								CW_FREE_OBJECT(valuesPtr->getACLList);
								return CW_FALSE;
							}
							break;
						case VSP_T_SET_E_MENU:
							if(!CWParseEMenuConf(&completeMsg, element_len, valuesPtr->eMenuConf))
							{
								CW_FREE_OBJECT(valuesPtr->eMenuConf);
								return CW_FALSE;
							}
							break;
						case DEAUTH_DISASSOC_REPORT_TO_AC_SWITCH:
							if(!CWParseGetDisauthDisassocvalues(&completeMsg,element_len,valuesPtr->disassocDeauthValues))
							{
								CW_FREE_OBJECT(valuesPtr->disassocDeauthValues);
								return CW_FALSE; 
							}
							break;
						case DETAIL_ALL_STA_INFO_TO_AC_SWITCH:
							if(!CWParseGetAllstaLeavDetailIValues(&completeMsg,element_len,valuesPtr->detailAllStaInfoValues))
							{
								CW_FREE_OBJECT(valuesPtr->detailAllStaInfoValues);
								return CW_FALSE; 
							}
							break;
						case ExtendLongCmdForDHCPOption:
							CWWTPDebugLog("get the msg to do the long cmd for dhcp from AC");
							if(!(CWParseLongCmdForDHCPOption60FromAC(&completeMsg, element_len, valuesPtr)))
							{
								CW_FREE_OBJECT(valuesPtr->cmd);
								return CW_FALSE; 
							}
							break;
						case Two_radio_5G_priority_access:
							if(!CWParseTwoRadio5gPriorityAccess(&completeMsg,element_len,valuesPtr->twoRadio5GPriorityAccess))
							{
								CW_FREE_OBJECT(valuesPtr->twoRadio5GPriorityAccess);
								return CW_FALSE; 
							}
							break;
						/*Begin:caidh add we receive message 30 from AC 2013.02.16*/
						case VSP_T_AP_PACKET_STATISTIC_REPORT:
							if(!CWParseGetApPktsStatisticRpt(&completeMsg, element_len, valuesPtr->getApPktsStatisticRpt))
							{
								CW_FREE_OBJECT(valuesPtr->getApPktsStatisticRpt);
								return CW_FALSE;
							}
							break;
						/*End:caidh add  2013.02.16*/
						/* Begin: gengzj added for wifipos 2013-11-26 */
						case Set_Wifi_Scanning_and_Positioning:
						    CWWTPDebugLog("get the msg Set_Wifi_Scanning_and_Positioning from AC");
							if(!CWParseWifiScanAndPosParam(&completeMsg, element_len, valuesPtr->WifiScanAndPosVal))
							{
								CWWTPDebugLog("Parse error");
								CWReleaseWifiScanAndPosParam(&valuesPtr->WifiScanAndPosVal);
								return CW_FALSE; 
							}
							break;
						/* End: gengzj added end */
						default:
							completeMsg.offset += element_len;
							break;
					}
				}
				break;
			}
			default:
				completeMsg.offset += elemLen;
				break;
		}
	}
	
	
/* Begin: gengzj added for wifipos 2013-11-26 */
    CWWTPDebugLog("completeMsg.offset %d",completeMsg.offset);
	CWWTPDebugLog("len%d",len);
/* End: gengzj added end */
	if(completeMsg.offset != len) return CWErrorRaise(CW_ERROR_INVALID_FORMAT, "Garbage at the End of the Message");
	
	if(bindingMsgElemFound)
	{
		(*protected_flag)++;
		if(!(CWBindingParseConfigurationUpdateRequest (msg, len, &(valuesPtr->bindingValues))))
		{
			return CW_FALSE;
		}
	}

//	CWLog("Configure Update Request Parsed");
	
	return CW_TRUE;
}

CWBool CWParseWTPEventResponseMessage (char *msg, int len, int seqNum, void *values) 
{
	CWControlHeaderValues controlVal;
	CWProtocolMessage completeMsg;
	
	
	if(msg == NULL) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	
	
	completeMsg.msg = msg;
	completeMsg.offset = 0;
	
	if(!(CWParseControlHeader(&completeMsg, &controlVal))) return CW_FALSE; // error will be handled by the caller
	
	// different type
	if(controlVal.messageTypeValue != CW_MSG_TYPE_VALUE_WTP_EVENT_RESPONSE)
		return CWErrorRaise(CW_ERROR_INVALID_FORMAT, "Message is not WTP Event Response as Expected");
	
	if(controlVal.seqNum != seqNum) return CWErrorRaise(CW_ERROR_INVALID_FORMAT, "Different Sequence Number");
	
	controlVal.msgElemsLen -= CW_CONTROL_HEADER_OFFSET_FOR_MSG_ELEMS; // skip timestamp
	
	if(controlVal.msgElemsLen != 0 ) return CWErrorRaise(CW_ERROR_INVALID_FORMAT, "WTP Event Response must carry no message element");


	return CW_TRUE;
}


/*______________________________________________________________*/
/*  *******************___SAVE FUNCTIONS___*******************  */
CWBool CWSaveWTPEventResponseMessage (void *WTPEventResp)
{
	return CW_TRUE;
}

CWBool CWSaveConfigurationUpdateRequest(CWProtocolConfigurationUpdateRequestValues *valuesPtr, CWProtocolResultCode* resultCode, unsigned char *operat_state_cause,unsigned char radioId,unsigned char protected_flag)
{
//	char cmdbuf[256];
	char cmdbuf[512];
	*resultCode=CW_PROTOCOL_SUCCESS;

	if(valuesPtr==NULL) {return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);}


	if(valuesPtr->ProbeRequestInhibition_count > 0){
		if(!CWSaveProbeRequestInhibition(valuesPtr->ProbeRequestInhibition_value)){
			CW_FREE_OBJECT(valuesPtr->ProbeRequestInhibition_value);
			return CW_FALSE;
		}
		CW_FREE_OBJECT(valuesPtr->ProbeRequestInhibition_value);
	}
	
	if(valuesPtr->CastIsolation_CastRate_count > 0){
		if(!CWSaveCastIsolation_CastRate(valuesPtr->CastIsolation_CastRate_value)){
			CW_FREE_OBJECT(valuesPtr->CastIsolation_CastRate_value);
			return CW_FALSE;
		}
		CW_FREE_OBJECT(valuesPtr->CastIsolation_CastRate_value);
	}

	if(valuesPtr->timeStampCount>0)
	{
		CWWTPDebugLog("save time stamp :%d", valuesPtr->timeStampValue);
	}

	if(valuesPtr->adminStateCount>0)
	{
		if(!CWSetRadioAdminState(valuesPtr->adminState, operat_state_cause))
		{
			CW_FREE_OBJECT(valuesPtr->adminState);
			return CW_FALSE;
		}
		CW_FREE_OBJECT(valuesPtr->adminState);
	}

	if(valuesPtr->WTPStaticIPCount > 0)
	{
		if(!CWWTPSetStaticIP(valuesPtr->WTPStaticIP))
		{
			CW_FREE_OBJECT(valuesPtr->WTPStaticIP);
			return CW_FALSE;
		}
		CW_FREE_OBJECT(valuesPtr->WTPStaticIP);
	}

	if(valuesPtr->WTPStaticIPV2Count > 0)
	{
		if(!CWWTPSetStaticIPV2(valuesPtr->WTPStaticIPV2))
		{
			CW_FREE_OBJECT(valuesPtr->WTPStaticIPV2);
			return CW_FALSE;
		}
		CW_FREE_OBJECT(valuesPtr->WTPStaticIPV2);
	}
	if(valuesPtr->eth_mtu_count> 0)
	{
		if(!acw_set_eth_config_mtu(valuesPtr->eth_mtu_info))
		{
			CW_FREE_OBJECT(valuesPtr->eth_mtu_info);
			return CW_FALSE;
		}
		CW_FREE_OBJECT(valuesPtr->eth_mtu_info);
	}
	if(valuesPtr->time_count> 0)
	{
		if(!acw_set_time_config(valuesPtr->time_info))
		{
			CW_FREE_OBJECT(valuesPtr->time_info);
			return CW_FALSE;
		}
		CW_FREE_OBJECT(valuesPtr->time_info);
	}
	if(valuesPtr->apScanningSetCount>0)  //pei add 1118
	{
		if(!CWSetApScanning(valuesPtr->apScanningSet))
		{
			CW_FREE_OBJECT(valuesPtr->apScanningSet);
			return CW_FALSE;
		}
		CW_FREE_OBJECT(valuesPtr->apScanningSet);
	}

	if(valuesPtr->staThroughputSetCount>0)
	{
		if(!CWSetStaThroughput(valuesPtr->staThroughputSet))
		{
			CW_FREE_OBJECT(valuesPtr->staThroughputSet);
			return CW_FALSE;
		}
		CW_FREE_OBJECT(valuesPtr->staThroughputSet);
	}

	if(valuesPtr->apThroughputSetCount>0)  //pei add 0204
	{
#if 0
		if(!CWSetApThroughputInfoCollect(valuesPtr->apThroughputSet))
		{
			CW_FREE_OBJECT(valuesPtr->apThroughputSet);
			return CW_FALSE;
		}
#endif
		if(!CWget_wtp_throughput_Info(valuesPtr->apThroughputSet))
		{
			CW_FREE_OBJECT(valuesPtr->apThroughputSet);
			return CW_FALSE;
		}
		CW_FREE_OBJECT(valuesPtr->apThroughputSet);
	}
	if(valuesPtr->maxThroughputSetCount>0)
	{
		if(!CWSetApMaxThroughput(valuesPtr->maxThroughputSet,radioId))
		{
			CW_FREE_OBJECT(valuesPtr->maxThroughputSet->wlanMaxThroughput);
			CW_FREE_OBJECT(valuesPtr->maxThroughputSet);
			return CW_FALSE;
		}
		CW_FREE_OBJECT(valuesPtr->maxThroughputSet->wlanMaxThroughput);
		CW_FREE_OBJECT(valuesPtr->maxThroughputSet);
	}
	if(valuesPtr->cmdLength>0)
	{
		autelan_sprintf(cmdbuf, "%s", valuesPtr->cmd);
		_system(cmdbuf);
		CW_FREE_OBJECT(valuesPtr->cmd);
	}
	//gExtraInfoEnable = valuesPtr->extraInfoEnable;  //pei add 0224
	if(valuesPtr->apMonitorSetCount>0)  //pei add 1118
	{
		if(!CWSetApMonitor(valuesPtr->apMonitorSet))
		{
			CW_FREE_OBJECT(valuesPtr->apMonitorSet);
			return CW_FALSE;
		}
		CW_FREE_OBJECT(valuesPtr->apMonitorSet);
	}
	if(valuesPtr->extraInfoCount>0)  //pei add 1118
	{
		if(!CWSetApExtraInfo(valuesPtr->extraInfo))
		{
			CW_FREE_OBJECT(valuesPtr->extraInfo);
			return CW_FALSE;
		}
		CW_FREE_OBJECT(valuesPtr->extraInfo);
	}
	if(valuesPtr->getStaInfoCount>0)  //pei add 0417
	{
		if(!CWGetStaInfo(valuesPtr->getStaInfo))
		{
			CW_FREE_OBJECT(valuesPtr->getStaInfo);
			return CW_FALSE;
		}
		CW_FREE_OBJECT(valuesPtr->getStaInfo);
	}
	if(valuesPtr->getIfStateCount>0)  //pei add 0422
	{
		if(!CWGetIfState(valuesPtr->getIfState))
		{
			CW_FREE_OBJECT(valuesPtr->getIfState);
			return CW_FALSE;
		}
		CW_FREE_OBJECT(valuesPtr->getIfState);
	}
	if(valuesPtr->getAttackDetectInfoCount>0)
	{
		if(!CWGetAttackDetectInfo(valuesPtr->getAttackDetectInfo))
		{
			CW_FREE_OBJECT(valuesPtr->getAttackDetectInfo);
			return CW_FALSE;
		}
		CW_FREE_OBJECT(valuesPtr->getAttackDetectInfo);
	}
	if(valuesPtr->getStaWapiInfoCount>0)
	{
		if(!CWGetStaWapiInfo(valuesPtr->getStaWapiInfo))
		{
			CW_FREE_OBJECT(valuesPtr->getStaWapiInfo);
			return CW_FALSE;
		}
		CW_FREE_OBJECT(valuesPtr->getStaWapiInfo);
	}
	if(valuesPtr->getNtpCount>0)  
	{
		if(!CWSetNtp(valuesPtr->getNtpInfo))
		{
			CW_FREE_OBJECT(valuesPtr->getNtpInfo);
			return CW_FALSE;
		}
		CW_FREE_OBJECT(valuesPtr->getNtpInfo);
	}
	if(valuesPtr->getTerminalDisturbCount>0)  
	{
		if(!CWSetTerminalDisturb(valuesPtr->getTerminalDisturbInfo))
		{
			CW_FREE_OBJECT(valuesPtr->getTerminalDisturbInfo);
			return CW_FALSE;
		}
		CW_FREE_OBJECT(valuesPtr->getTerminalDisturbInfo);
	}
	if(valuesPtr->userpasswdcount >0)
	{
		if(!CWSetUserandPasswdinfo(valuesPtr->userpasswdinfo))
		{
			CW_FREE_OBJECT(valuesPtr->userpasswdinfo->username);
			CW_FREE_OBJECT(valuesPtr->userpasswdinfo->passwd);
			return CW_FALSE;
		}
		CW_FREE_OBJECT(valuesPtr->userpasswdinfo->username);
		CW_FREE_OBJECT(valuesPtr->userpasswdinfo->passwd);
	}
	if(valuesPtr->getMultiUserPerformanceCount>0)  
	{
		if(!CWSetMultiUserPerformance(valuesPtr->getMultiUserPerformance))
		{
			CW_FREE_OBJECT(valuesPtr->getMultiUserPerformance);
			return CW_FALSE;
		}
		CW_FREE_OBJECT(valuesPtr->getMultiUserPerformance);
	}
	if(valuesPtr->getMcsMaskCount>0)  
	{
		if(!CWSetMcsMask(valuesPtr->getMcsMask))
		{
			CW_FREE_OBJECT(valuesPtr->getMcsMask);
			return CW_FALSE;
		}
		CW_FREE_OBJECT(valuesPtr->getMcsMask);
	}
	
	if(valuesPtr->getStaPktsStatisticRptCount>0)  
	{
		if(!CWSetStaPktsStatisticRpt(valuesPtr->getStaPktsStatisticRpt))
		{
			CW_FREE_OBJECT(valuesPtr->getStaPktsStatisticRpt);
			return CW_FALSE;
		}
		CW_FREE_OBJECT(valuesPtr->getStaPktsStatisticRpt);
	}

	if(valuesPtr->getACLList_count>0)  
	{
		if(!CWSetACLList(valuesPtr->getACLList))
		{
			CW_FREE_OBJECT(valuesPtr->getACLList->mac_list);
			CW_FREE_OBJECT(valuesPtr->getACLList);
			return CW_FALSE;
		}
		CW_FREE_OBJECT(valuesPtr->getACLList->mac_list);
		CW_FREE_OBJECT(valuesPtr->getACLList);
	}
	
	if(valuesPtr->eMenuConf_count>0)  
	{
		if(!CWSetEMenuConf(valuesPtr->eMenuConf))
		{
			CW_FREE_OBJECT(valuesPtr->eMenuConf);
			return CW_FALSE;
		}
		CW_FREE_OBJECT(valuesPtr->eMenuConf);
	}
	if(valuesPtr->deauth_disassoc_report_count>0)
	{
		if(!CWSaveGetDisauthDisassocvalues(valuesPtr->disassocDeauthValues))
		{
			CW_FREE_OBJECT(valuesPtr->disassocDeauthValues);
			return CW_FALSE; 
		}
		CW_FREE_OBJECT(valuesPtr->disassocDeauthValues);
	}
	if(valuesPtr->detail_all_sta_info_count>0)
	{
		if(!CWSaveGetAllstaLeavDetailIValues(valuesPtr->detailAllStaInfoValues))
		{
			CW_FREE_OBJECT(valuesPtr->detailAllStaInfoValues);
			return CW_FALSE; 
		}
		CW_FREE_OBJECT(valuesPtr->detailAllStaInfoValues);
	}

	if(valuesPtr->two_radio_5g_priority_access > 0)
	{
		if(!CWSaveTwoRadio5gPriorityAccess(valuesPtr->twoRadio5GPriorityAccess))
		{
			CW_FREE_OBJECT(valuesPtr->detailAllStaInfoValues);
			return CW_FALSE; 
		}
		CW_FREE_OBJECT(valuesPtr->twoRadio5GPriorityAccess);
	}
	/*Begin:this function is to set ap packet statistic report thread caidh add 2013.02.16*/
	if(valuesPtr->getApPktsStatisticRptCount > 0)
	{
		if(!CWSetApPktsStatisticRpt(valuesPtr->getApPktsStatisticRpt))
		{
			CW_FREE_OBJECT(valuesPtr->getApPktsStatisticRpt);
			return CW_FALSE;
		}
		CW_FREE_OBJECT(valuesPtr->getApPktsStatisticRpt);
	}	
	/*End:caidh add 2013.02.16*/
/* Begin: gengzj added for wifipos 2013-11-26 */
	if (valuesPtr->WifiScanAndPosCount > 0)
	{
	    if (!CWSetWifiScanAndPosParam(valuesPtr->WifiScanAndPosVal))
	    {
	        CWReleaseWifiScanAndPosParam(&valuesPtr->WifiScanAndPosVal);
			return CW_FALSE;
	    }
		CWReleaseWifiScanAndPosParam(&valuesPtr->WifiScanAndPosVal);
	}
/* End: gengzj added end */	
	if(protected_flag != 0 )
	{
	/*
	  *zengmin del sta_access_limit 2013-05-02
		_system("killall sta_access_limit");
		if(autelan_system("sta_access_limit &") == 0){
			CWWTPDebugLog("sta_access_limit OK");
		}
		*/
		if(gApScanningEnable == 1){
			memset(cmdbuf, 0, 128);
			_system("killall iwlist_scan > /dev/null 2>&1");
			_system("/sbin/ifconfig athscanwifi0 down > /dev/null 2>&1 &");
			if(0==Check_Interface_State("athscanwifi1"))
				_system("/sbin/ifconfig athscanwifi1 down > /dev/null 2>&1 &");
		}
	}
	if(valuesPtr->WTPModeNParaCount > 0)
	{
		if(!CWWTPSetModeNPara(valuesPtr->WTPModeNPara))
		{
			CW_FREE_OBJECT(valuesPtr->WTPModeNPara);
			return CW_FALSE;
		}
		CW_FREE_OBJECT(valuesPtr->WTPModeNPara);
	}
	if (valuesPtr->bindingValues!=NULL) 
	{
		if(!CWBindingSaveConfigurationUpdateRequest(valuesPtr->bindingValues, resultCode)) 
			return CW_FALSE;
	}
	if( protected_flag !=0)	
	{
		if(gApScanningEnable == 1){
			char wifi0mode[10]={0};
			char wifi1mode[10]={0};
			_system("/sbin/ifconfig athscanwifi0 up > /dev/null 2>&1 &");
			if(0==Check_Interface_State("athscanwifi1"))
				_system("ifconfig athscanwifi1 up >/dev/null 2>&1 &");
			memset(cmdbuf, 0, 128);
			if((gRadioInfoValue[0].radiotype&IEEE80211_PARAM_MODE_11a)!=0)
				memcpy(wifi0mode,"11a",sizeof("11a"));
			else memcpy(wifi0mode,"11bg",sizeof("11bg"));
			
			if(0==Check_Interface_State("athscanwifi1")){
				if((gRadioInfoValue[1].radiotype&IEEE80211_PARAM_MODE_11a)!=0)
					memcpy(wifi1mode,"11a",sizeof("11a"));
				else
					memcpy(wifi1mode,"11bg",sizeof("11bg"));
				autelan_sprintf(cmdbuf, "/usr/sbin/iwlist_scan -m %s -c %s -I %d -i 5 -n 2 > /dev/null 2>&1 &",wifi0mode,wifi1mode, gApScanningInterval/3>60?gApScanningInterval/3:60);
			}
			else
				autelan_sprintf(cmdbuf, "/usr/sbin/iwlist_scan -m %s -I %d -i 5 -n 1 > /dev/null 2>&1 &",wifi0mode,gApScanningInterval/3>60?gApScanningInterval/3:60);
			_system(cmdbuf);
		}
	}
	return CW_TRUE;
}

/*
CWBool CWWTPManageACRunRequest(char *msg, int len)
{
	CWControlHeaderValues controlVal;
	CWProtocolMessage completeMsg;
	
	if(msg == NULL) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	
	completeMsg.msg = msg;
	completeMsg.offset = 0;
	
	if(!(CWParseControlHeader(&completeMsg, &controlVal))) return CW_FALSE; // error will be handled by the caller
	
	switch(controlVal.messageTypeValue) {
		case CW_MSG_TYPE_VALUE_CONFIGURE_UPDATE_REQUEST:
			break;
		case CW_MSG_TYPE_VALUE_ECHO_REQUEST:
			break;
		case CW_MSG_TYPE_VALUE_CLEAR_CONFIGURATION_REQUEST:
			break;
		case CW_MSG_TYPE_VALUE_STATION_CONFIGURATION_REQUEST:
			break;
		default:
			return CWErrorRaise(CW_ERROR_INVALID_FORMAT, "Message is not Change State Event Response as Expected");
	}

	
	
	//if(controlVal.seqNum != seqNum) return CWErrorRaise(CW_ERROR_INVALID_FORMAT, "Different Sequence Number");
	
	controlVal.msgElemsLen -= CW_CONTROL_HEADER_OFFSET_FOR_MSG_ELEMS; // skip timestamp
	
	if(controlVal.msgElemsLen != 0 ) return CWErrorRaise(CW_ERROR_INVALID_FORMAT, "Change State Event Response must carry no message elements");


	return CW_TRUE;
}
*/

CWBool CWParseIEEE80211WlanConfigurationRequest(char *msg, int len, CWProtocolConfigurationUpdateRequestValues *valuesPtr) 
{
	CWBool bindingMsgElemFound=CW_FALSE;
	CWProtocolMessage completeMsg;
	
	if(msg == NULL || valuesPtr == NULL) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	
	
	completeMsg.msg = msg;
	completeMsg.offset = 0;

	valuesPtr->bindingValues = NULL;

	// parse message elements
	while(completeMsg.offset < len) {
		unsigned short int elemType=0;// = CWProtocolRetrieve32(&completeMsg);
		unsigned short int elemLen=0;// = CWProtocolRetrieve16(&completeMsg);
		
		CWParseFormatMsgElem(&completeMsg,&elemType,&elemLen);		


		if(CWBindingCheckType(elemType))
		{
			bindingMsgElemFound=CW_TRUE;
			completeMsg.offset += elemLen;
			continue;	
		}
		switch(elemType) {			
			default:
				CWWTPDebugLog("Unrecognized Message Element:%d, in IEEE80211 Wlan Configuration Request ", elemType);
				completeMsg.offset += elemLen;
				break;
//				return CWErrorRaise(CW_ERROR_INVALID_FORMAT, "Unrecognized Message Element");
		}
	}

	if(completeMsg.offset != len) return CWErrorRaise(CW_ERROR_INVALID_FORMAT, "Garbage at the End of the Message");
	
	if(bindingMsgElemFound)
	{
		if(!(CWBindingParseIEEE80211WlanConfigurationRequest(msg, len, &(valuesPtr->bindingValues))))
		{
			return CW_FALSE;
		}
	}
	//CWLog("wlan configuration request parsed return CW_true");
	
	return CW_TRUE;
}

CWBool CWSaveIEEE80211WlanConfigurationRequest(CWProtocolConfigurationUpdateRequestValues *valuesPtr, unsigned char *created_radio_id, unsigned char *created_wlan_id, CWProtocolResultCode* resultCode)
{
	*resultCode=CW_TRUE;

	if(valuesPtr==NULL) {return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);}

	if (valuesPtr->bindingValues!=NULL) 
	{
		if(!CWBindingSaveIEEE80211WlanConfigurationRequest(valuesPtr->bindingValues,created_radio_id,created_wlan_id,resultCode)) 
			return CW_FALSE;
	}
	return CW_TRUE;

}

CWBool CWAssembleIEEE80211WlanConfigurationResponse(CWProtocolMessage **messagesPtr,CWProtocolConfigurationUpdateRequestValues *valuesPtr, int *fragmentsNumPtr, int PMTU, int seqNum,unsigned char created_radio_id,unsigned char created_wlan_id,CWProtocolResultCode resultCode)
{
	CWProtocolMessage *msgElems= NULL;
	const int msgElemCount = 1;
	CWProtocolMessage *msgElemsBinding= NULL;
	const int msgElemBindingCount = 1;
	unsigned char bssid[MAC_ADDR_LEN] = {0};
	int sock = -1;
	struct ifreq ifr;
	int radio_id;
	CWBool radiofound = CW_FALSE;
	int i;
	char wlanname[IFNAMSIZ];
	CWWTPWlan *ptr = NULL;
	
	if(messagesPtr == NULL || fragmentsNumPtr == NULL) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	
	CW_CREATE_OBJECT_ERR(msgElems, CWProtocolMessage, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	CW_CREATE_OBJECT_ERR(msgElemsBinding, CWProtocolMessage, {CW_FREE_OBJECT(msgElems);return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL);});
	sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock < 0) {
		CWWTPDebugLog("%s,Error Creating Socket for ioctl",__func__); 
		resultCode = CW_PROTOCOL_FAILURE;
	}
	for(ptr = wtp_wlan_list;ptr != NULL;ptr = ptr->next){
		if((ptr->radio_id == created_radio_id)&&(ptr->wlan_id == created_wlan_id)){
			radio_id = ptr->radio_id;
			radiofound = CW_TRUE;
			break;
		}
	}
	if(radiofound){
		CWWTPGetWlanName(wlanname, created_radio_id, created_wlan_id);
		strncpy(ifr.ifr_name,wlanname,IFNAMSIZ);
		if (ioctl(sock, SIOCGIFHWADDR, &ifr)==-1) {
			CWWTPDebugLog("get wlan mAC error");
		}
		else{
			for (i=0; i<MAC_ADDR_LEN; i++)	{
				bssid[i]=(unsigned char)ifr.ifr_hwaddr.sa_data[i];
//				dpf("%02x ",bssid[i]);
			}
//			printf("\n");
			for(ptr = wtp_wlan_list;ptr != NULL;ptr = ptr->next){  //pei add for format capwap header at 090413
				if((ptr->radio_id == created_radio_id)&&(ptr->wlan_id == created_wlan_id)){
					memcpy(ptr->wlan_bssid, bssid, MAC_ADDR_LEN);
					break;
				}
			}
		}
		{
			CWWTPDebugLog("wlan %s bssid %02x:%02x:%02x:%02x:%02x:%02x",wlanname,bssid[0],bssid[1],bssid[2],bssid[3],bssid[4],bssid[5]);
			char cmdbuf[100];
			autelan_sprintf(cmdbuf,"/sbin/ifconfig %s up > /dev/null", wlanname);
			autelan_system(cmdbuf);
		}
	}
	else{
		radio_id = 2; //pei modified 1 to 2 at 090326 for 2radio
	}
	if(sock != -1)
		autelan_close(sock);
	if (!(CWAssembleMsgElemResultCode(msgElems,resultCode))||
		!(CWAssembleAssignedWTPBssid(msgElemsBinding, (char *)bssid, created_wlan_id, radio_id))) { //pei 0917 del warning
		CW_FREE_OBJECT(msgElems);
		CW_FREE_OBJECT(msgElemsBinding);
		return CW_FALSE; // error will be handled by the caller
	}

	if(!(CWAssembleMessage(messagesPtr,
				   fragmentsNumPtr,
				   PMTU,
				   seqNum,
				   CW_MSG_TYPE_VALUE_IEEE80211_WLAN_CONFIG_RESPONSE,
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
	
	CWWTPDebugLog("Assemble wlan configure response success!");

	return CW_TRUE;
}
CWBool CWParseSTAConfigurationRequest(char *msg, int len, CWProtocolConfigurationUpdateRequestValues *valuesPtr)
{
	CWBool bindingMsgElemFound=CW_FALSE;
	CWProtocolMessage completeMsg;
	
	if(msg == NULL || valuesPtr == NULL) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	
	
	completeMsg.msg = msg;
	completeMsg.offset = 0;

	valuesPtr->bindingValues = NULL;
// parse message elements
	while(completeMsg.offset < len) {
		unsigned short int elemType=0;// = CWProtocolRetrieve32(&completeMsg);
		unsigned short int elemLen=0;// = CWProtocolRetrieve16(&completeMsg);
		
		CWParseFormatMsgElem(&completeMsg,&elemType,&elemLen);		


		if(elemType > 0)
		{
			bindingMsgElemFound=CW_TRUE;
			completeMsg.offset += elemLen;
			continue;	
		}
		switch(elemType) {			
			default:
				CWWTPDebugLog("Unrecognized Message Element:%d, in STA Configuration Request", elemType);
				completeMsg.offset += elemLen;
				break;
//				return CWErrorRaise(CW_ERROR_INVALID_FORMAT, "Unrecognized Message Element");
		}
	}

	if(completeMsg.offset != len) return CWErrorRaise(CW_ERROR_INVALID_FORMAT, "Garbage at the End of the Message");
	
	if(bindingMsgElemFound)
	{
		if(!(CWBindingParseSTAConfigurationRequest(msg, len, &(valuesPtr->bindingValues))))
		{
			return CW_FALSE;
		}
	}
	
	return CW_TRUE;		

}
CWBool CWSaveSTAConfigurationRequest(CWProtocolConfigurationUpdateRequestValues *valuesPtr, CWProtocolResultCode* resultCode)
{
	*resultCode=CW_TRUE;

	if(valuesPtr==NULL) {return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);}

	if (valuesPtr->bindingValues!=NULL) 
	{
		if(!CWBindingSaveSTAConfigurationRequest(valuesPtr->bindingValues,resultCode)) 
			return CW_FALSE;
	}
	return CW_TRUE;

}
CWBool CWAssembleSTAConfigurationResponse(CWProtocolMessage **messagesPtr, int *fragmentsNumPtr, int PMTU, int seqNum, CWProtocolResultCode resultCode)
{
	CWProtocolMessage *msgElems= NULL;
	const int msgElemCount = 1;
	CWProtocolMessage *msgElemsBinding= NULL;
	const int msgElemBindingCount=0;
	
	if(messagesPtr == NULL || fragmentsNumPtr == NULL) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	
	
	CW_CREATE_OBJECT_ERR(msgElems, CWProtocolMessage, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););

	if (!(CWAssembleMsgElemResultCode(msgElems,resultCode))) {
		CW_FREE_OBJECT(msgElems);
		return CW_FALSE;
	}

	if(!(CWAssembleMessage(messagesPtr,
			       fragmentsNumPtr,
			       PMTU,
			       seqNum,
			       CW_MSG_TYPE_VALUE_STATION_CONFIGURATION_RESPONSE,
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
	
	CWWTPDebugLog("Assemble sta configure response success!");
	return CW_TRUE;

}


CWBool CWParseWTPEventRequestDeleteStationMessage(char *msgPtr, int len, DeleteSTAValues *valuesPtr) //pei add 0703
{
//	int i;
	CWProtocolMessage completeMsg;
	
	if(msgPtr == NULL || valuesPtr == NULL) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	
	
	completeMsg.msg = msgPtr;
	completeMsg.offset = 0;
	
	unsigned short int elemType = 0;
	unsigned short int elemLen = 0;
	
	CWParseFormatMsgElem(&completeMsg,&elemType,&elemLen);
	
	
	CW_CREATE_OBJECT_SIZE_ERR(valuesPtr->mac_addr, valuesPtr->mac_length, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););

	if(!(CWParseDeleteSta(&completeMsg, elemLen, valuesPtr))){
				 CWWTPDebugLog("parse delete sta failed");}

/*
	if(debug_print)
	{
		printf("valuesPtr->radio_id = %d\n", valuesPtr->radio_id);
		printf("valuesPtr->mac_length = %d\n", valuesPtr->mac_length);
		printf("valuesPtr->mac_addr = ");
		for (i = 0; i < 6; i++)
		{
			printf("%02x", valuesPtr->mac_addr[i]);
		}
		printf("\n");
	}
*/	
	
	return CW_TRUE;
}



