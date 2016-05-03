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
#include "WTPFrameReceive.h"
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
CWThread thread_apMonitor;
CWThread thread_extraInfo;
CWThread thread_staInfo;
CWThread thread_ifState;
CWThread thread_snooping;
CWThread thread_sta_pkts_statistic_report;

// Manage DTLS packets
CW_THREAD_RETURN_TYPE CWWTPReceiveDtlsPacket(void *arg)
{
	if(pthread_detach(pthread_self())!=0)
	printf("##########detach error!############\n");
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
			 sleep(1);
			 continue;
		}
		
		// Clone data packet  clone a copy to add to the list		
		CW_CREATE_OBJECT_SIZE_ERR(pData, readBytes, { CWDebugLog("Out Of Memory"); return NULL; });
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
	printf("##########detach error!############\n");
	#define DST_MAC_HEN 6
	#define SRC_MAC_HEN 6
	int readBytes;
	unsigned char buf[CW_BUFFER_SIZE];
	unsigned char buf_eap[CW_BUFFER_SIZE];
	unsigned char *sendmsg;
	int sendBytes;
	int n;
	int wlanfound = 0;
	unsigned char wlan_id;
	unsigned char header_len;/*the length of capwap header*/
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
			CWWTPDebugLog("data channel error ");
			continue;
		}
#if 1
		dpf("get data from ac to sta:\n");
		CWCaptrue(readBytes,buf);
#endif 
		
		int val;
		int type = 1,T=1;
		unsigned char  rid;
		unsigned short wlan_id_bitmap;
		
		val = *(unsigned int *)buf;
		val = ntohl(val);//endian issue.
		type = CWGetField32(val,CW_TRANSPORT_HEADER_WBID_START,CW_TRANSPORT_HEADER_WBID_LEN);
		T = CWGetField32(val,CW_TRANSPORT_HEADER_T_START,CW_TRANSPORT_HEADER_T_LEN);
		rid = CWGetField32(val,CW_TRANSPORT_HEADER_RID_START,CW_TRANSPORT_HEADER_RID_LEN);
		dpf("type=wbid=%d rid=%d,T=%d,%s\n",type,rid,T,((T!=0 && type == 1)?"802.11":"802.3"));

		if(rid < 0 ||rid > wifi_count){
			CWWTPDebugLog("Rid(%d) Invalid.Discard It!",rid);
			continue;
		}

		header_len= 16;
		
		wlan_id_bitmap = *(unsigned char *)(buf+8);
		dpf("Get WlanID_BitMap:%hd.\n",wlan_id_bitmap);
		if(wlan_id_bitmap == 0){
			CWWTPDebugLog("wlan_id_bitmap == 0,Discard it.");
			continue;
		}
		wlan_id = wlan_id_bitmap;
		wlan_id = CWGetLocalWlanID(rid,wlan_id);
		if(T!=0 && type == 1)//802.11
		{
			for(ptr = wtp_wlan_list;ptr != NULL;ptr = ptr->next){
				if(ptr->radio_id == rid && ptr->wlan_id == wlan_id){
					wlan_sock = ptr->wlan_sendsock;
					eap_sock = ptr->wlan_eapsock;
					wlanfound = 1;
					dpf("802.11_get wlan_id:%d ptr->wlan_sendsock:%d for sending data to sta\n", wlan_id,ptr->wlan_sendsock);
					break;
				}				
			}
		}
		else if(T == 0)//802.3
		{
			for(ptr = wtp_wlan_list;ptr != NULL;ptr = ptr->next){
				if(ptr->radio_id == rid && ptr->wlan_id == wlan_id){
					wlan_sock = ptr->wlan_dot3sock;
					eap_sock = ptr->wlan_eapsock;
					wlanfound = 1;
					dpf("802.3_get wlan_id:%d ptr->wlan_dot3sock:%d for sending data to sta\n", wlan_id,ptr->wlan_dot3sock);
					break;
				}				
			}
		}
		else{
			CWWTPDebugLog("!802.3 && !802.11,Discard it.");
			continue;
		}
		
		dpf("%s,%d,wlan_id:%d\n", __func__, __LINE__, wlan_id);
		
		if(wlanfound == 0){
			dpf("Can't find the send wlan \n");
			continue;
		}
		
		sendmsg = (unsigned char *)(buf+header_len);
		sendBytes = readBytes-header_len;

		dpf("%s,%d,wlan_sock:%d\n", __func__, __LINE__,wlan_sock);
		n = sendto(wlan_sock, sendmsg,sendBytes, 0x0, NULL, 0); 
		if(n<0)
			CWDebugLog("data channel send error\n");
	}

	CWDebugLog("data channel receive finished ");
	
	return NULL;
}
#ifndef CW_NO_DTLS
// Manage Data DTLS packets
CW_THREAD_RETURN_TYPE CWWTPReceiveRealDtlsDataPacket(void *arg)
{
	if(pthread_detach(pthread_self())!=0)
	printf("##########detach error!############\n");
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
			 sleep(1);
		     CWLog("Receiving Dtls packets from AC error,continue");
		     continue;
		}
		
		// Clone data packet  clone a copy to add to the list		
		CW_CREATE_OBJECT_SIZE_ERR(pData, readBytes, { CWDebugLog("Out Of Memory"); return NULL; });
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
	CW_CREATE_OBJECT_SIZE_ERR(pData, readBytes, { CWDebugLog("Out Of Memory"); return; });
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
	
	CWDebugLog("Assembling WTP Event Request AP Warning...");

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
	
	CWDebugLog("WTP Event Request AP Warning Assembled");

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
		CWDebugLog("Assemble WTP Event Ap Scanning Request error!\n");
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
				CWDebugLog("Error sending message\n");
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
				CWDebugLog("Error sending message\n");
				CWFreeMessageFragments(messages, fragmentsNum);
				CW_FREE_OBJECT(messages);
				return;
			}
		}
	}
	CWDebugLog("Message Sent\n");
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
	sprintf(cmdbuf, "mkdir -p %s", gWtpdConfDir);
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
		inet_ntop(AF_INET, &sin->sin_addr, str, sizeof(str));
	}
	else
	{
		struct sockaddr_in6 *sin6 = (struct sockaddr_in6 *)sa;
		inet_ntop(AF_INET6, &sin6->sin6_addr, str, sizeof(str));
	}

	sprintf(cmdbuf, "echo %s > %s/ac_addr", str, gWtpdConfDir);
	_system(cmdbuf);

	radioCount = CWGetRadioMaxCount();
	if(radioCount == 0)
		radioCount = 2;
		
	/* radio conf */
	for(i=0;i<radioCount;i++)
	{
		sprintf(cmdbuf, "mkdir -p %s/radio%d", gWtpdConfDir, i);
		_system(cmdbuf);
		sprintf(cmdbuf, "echo %d > %s/radio%d/channel", gRadioInfoValue[i].channel, gWtpdConfDir, i);
		_system(cmdbuf);
		sprintf(cmdbuf, "echo %d > %s/radio%d/txtype", gRadioInfoValue[i].txtype, gWtpdConfDir, i);
		_system(cmdbuf);
		sprintf(cmdbuf, "echo %d > %s/radio%d/txpower", gRadioInfoValue[i].txpower, gWtpdConfDir, i);
		_system(cmdbuf);
		sprintf(cmdbuf, "echo %d > %s/radio%d/rate", gRadioInfoValue[i].rate, gWtpdConfDir, i);
		_system(cmdbuf);
		sprintf(cmdbuf, "echo %d > %s/radio%d/rateAutoEnable", gRadioInfoValue[i].rateAutoEnable, gWtpdConfDir, i);
		_system(cmdbuf);
		sprintf(cmdbuf, "echo %d > %s/radio%d/radiotype", gRadioInfoValue[i].radiotype, gWtpdConfDir, i);
		_system(cmdbuf);
		sprintf(cmdbuf, "echo %d > %s/radio%d/fragThreshold", gRadioInfoValue[i].fragThreshold, gWtpdConfDir, i);
		_system(cmdbuf);
		sprintf(cmdbuf, "echo %d > %s/radio%d/rtsThreshold", gRadioInfoValue[i].rtsThreshold, gWtpdConfDir, i);
		_system(cmdbuf);
		sprintf(cmdbuf, "echo %d > %s/radio%d/shortRetry", gRadioInfoValue[i].shortRetry, gWtpdConfDir, i);
		_system(cmdbuf);
		sprintf(cmdbuf, "echo %d > %s/radio%d/longRetry", gRadioInfoValue[i].longRetry, gWtpdConfDir, i);
		_system(cmdbuf);
		sprintf(cmdbuf, "echo %d > %s/radio%d/beaconInterval", gRadioInfoValue[i].beaconInterval, gWtpdConfDir, i);
		_system(cmdbuf);
		sprintf(cmdbuf, "echo %d > %s/radio%d/preamble", gRadioInfoValue[i].preamble, gWtpdConfDir, i);
		_system(cmdbuf);
		sprintf(cmdbuf, "echo %d > %s/radio%d/dtim", gRadioInfoValue[i].dtim, gWtpdConfDir, i);
		_system(cmdbuf);
		sprintf(cmdbuf, "echo %d > %s/radio%d/gIsRadioEnable", gRadioInfoValue[i].gIsRadioEnable, gWtpdConfDir, i);
		_system(cmdbuf);

		sprintf(cmdbuf, "rm -rf %s/radio%d/wlaninfo", gWtpdConfDir, i);
		_system(cmdbuf);
		sprintf(cmdbuf, "mkdir -p %s/radio%d/wlaninfo", gWtpdConfDir, i);
		_system(cmdbuf);
		for(ptr=wtp_wlan_list;ptr != NULL;oldptr = ptr,ptr = ptr->next)
		{
			if(ptr->radio_id == i)
			{
				radio_wlan_count[i] ++;
				sprintf(cmdbuf, "mkdir -p %s/radio%d/wlaninfo/wlan%d", gWtpdConfDir, i, radio_wlan_count[i]);
				_system(cmdbuf);
				sprintf(cmdbuf, "echo %d > %s/radio%d/wlaninfo/wlan%d/radio_id",  ptr->radio_id, gWtpdConfDir, i, radio_wlan_count[i]);
				_system(cmdbuf);
				sprintf(cmdbuf, "echo %d > %s/radio%d/wlaninfo/wlan%d/wlan_id", ptr->wlan_id, gWtpdConfDir, i, radio_wlan_count[i]);
				_system(cmdbuf);
				sprintf(cmdbuf, "echo %d > %s/radio%d/wlaninfo/wlan%d/vlan_id", ptr->vlan_id, gWtpdConfDir, i, radio_wlan_count[i]);
				_system(cmdbuf);
				sprintf(cmdbuf, "echo %d > %s/radio%d/wlaninfo/wlan%d/wlan_updown_time", ptr->wlan_updown_time, gWtpdConfDir, i, radio_wlan_count[i]);
				_system(cmdbuf);
				sprintf(cmdbuf, "echo %d > %s/radio%d/wlaninfo/wlan%d/wlan_mac_mode", ptr->wlan_mac_mode, gWtpdConfDir, i, radio_wlan_count[i]);
				_system(cmdbuf);
				sprintf(cmdbuf, "echo %d > %s/radio%d/wlaninfo/wlan%d/wlan_tunnel_mode", ptr->wlan_tunnel_mode, gWtpdConfDir, i, radio_wlan_count[i]);
				_system(cmdbuf);
			}
		}
		sprintf(cmdbuf, "echo %d > %s/radio%d/wlan_count", radio_wlan_count[i], gWtpdConfDir, i);
		_system(cmdbuf);
	}

	/* parameters whole process */
//	if(gApScanningEnable)
	{
		sprintf(cmdbuf, "echo %d > %s/gApScanningEnable", gApScanningEnable, gWtpdConfDir);
		_system(cmdbuf);
		sprintf(cmdbuf, "echo %d > %s/gApScanningInterval", gApScanningInterval, gWtpdConfDir);
		_system(cmdbuf);
	}
//	if(gAPThroughputInfoCollectEnable)
	{
		sprintf(cmdbuf, "echo %d > %s/gAPThroughputInfoCollectEnable", gAPThroughputInfoCollectEnable, gWtpdConfDir);
		_system(cmdbuf);
		sprintf(cmdbuf, "echo %d > %s/gAPThroughputInfoInterval", gAPThroughputInfoInterval, gWtpdConfDir);
		_system(cmdbuf);
	}
//	if(gApMonitorEnable)
	{
		sprintf(cmdbuf, "echo %d > %s/gApMonitorEnable", gApMonitorEnable, gWtpdConfDir);
		_system(cmdbuf);
		sprintf(cmdbuf, "echo %d > %s/gApMonitorInterval", gApMonitorInterval, gWtpdConfDir);
		_system(cmdbuf);
	}
//	if(gExtraInfoEnable)
	{
		sprintf(cmdbuf, "echo %d > %s/gExtraInfoEnable", gExtraInfoEnable, gWtpdConfDir);
		_system(cmdbuf);
		sprintf(cmdbuf, "echo %d > %s/gExtraInfoInterval", gExtraInfoInterval, gWtpdConfDir);
		_system(cmdbuf);
	}
//	if(gStaInfoEnable)
	{
		sprintf(cmdbuf, "echo %d > %s/gStaInfoEnable", gStaInfoEnable, gWtpdConfDir);
		_system(cmdbuf);
		sprintf(cmdbuf, "echo %d > %s/gStaInfoInterval", gStaInfoInterval, gWtpdConfDir);
		_system(cmdbuf);
	}
//	if(gIfStateEnable)
	{
		sprintf(cmdbuf, "echo %d > %s/gIfStateEnable", gIfStateEnable, gWtpdConfDir);
		_system(cmdbuf);
		sprintf(cmdbuf, "echo %d > %s/gIfStateInterval", gIfStateInterval, gWtpdConfDir);
		_system(cmdbuf);
	}
//	if(gFloodingDetectEnable)
	{
		sprintf(cmdbuf, "echo %d > %s/gFloodingDetectEnable", gFloodingDetectEnable, gWtpdConfDir);
		_system(cmdbuf);
		sprintf(cmdbuf, "echo %d > %s/gFloodAttackCnt", gFloodAttackCnt, gWtpdConfDir);
		_system(cmdbuf);
		sprintf(cmdbuf, "echo %d > %s/gFloodProbeCnt", gFloodProbeCnt, gWtpdConfDir);
		_system(cmdbuf);
		sprintf(cmdbuf, "echo %d > %s/gFloodWidsIntval", gFloodWidsIntval, gWtpdConfDir);
		_system(cmdbuf);
	}
//	if(gSpoofingDetectEnable)
	{
		sprintf(cmdbuf, "echo %d > %s/gSpoofingDetectEnable", gSpoofingDetectEnable, gWtpdConfDir);
		_system(cmdbuf);
	}
//	if(gWeakIVDetectEnable)
	{
		sprintf(cmdbuf, "echo %d > %s/gWeakIVDetectEnable", gWeakIVDetectEnable, gWtpdConfDir);
		_system(cmdbuf);
	}
	sprintf(cmdbuf, "echo %d > %s/gEchoInterval", gEchoInterval, gWtpdConfDir);
	_system(cmdbuf);
	sprintf(cmdbuf, "echo %d > %s/gCWNeighborDeadInterval", gCWNeighborDeadInterval, gWtpdConfDir);
	system(cmdbuf);
//	sprintf(cmdbuf, "echo %d > %s/wtp_wlan_count", wtp_wlan_count, gWtpdConfDir);
//	system(cmdbuf);

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
		close(s);
		return;
	}
	len = iwr.u.data.length;
	if (len < sizeof(struct ieee80211req_sta_info))
	{
		CWWTPDebugLog("get sta info for get_stations error!");
		close(s);
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
			CW_CREATE_OBJECT_SIZE_ERR(addStaValues.mac_addr, addStaValues.mac_length, {close(s); CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL);});
			memcpy(addStaValues.mac_addr, si->isi_macaddr, addStaValues.mac_length);
			STATableAdd(&addStaValues);
			CW_FREE_OBJECT(addStaValues.mac_addr);
		}
		cp += si->isi_len;
		len -= si->isi_len;
	} while (len >= sizeof(struct ieee80211req_sta_info));

	close(s);
	return;
}
CWBool CWBackupWlanTableAdd(CWWTPWlan *wlanBackup)
{
	CWWTPWlan *wtp_wlan_node;
	CWWTPWlan *ptr = NULL;
	CWWTPWlan *oldptr = NULL;
	CWDebugLog("wlan table add\n");
	
	for(ptr = wtp_wlan_list;ptr != NULL; oldptr = ptr ,ptr = ptr->next)
	{
		if((wlanBackup->radio_id == ptr->radio_id)&&(wlanBackup->wlan_id == ptr->wlan_id))
		{
			CWDebugLog("wlan is already in table\n");
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
		CWDebugLog("Error starting Thread that receive DTLS packet");
		CWNetworkCloseSocket(gWTPSocket);
		CWLog("Error starting Thread that receive DTLS packet when hot start.");
		return CW_FALSE;
	}

	
	if(!CWErr(CWNetworkInitSocketClientUnconnect(&gWTPDataSocket, &temptaddr, WTPDataSockPort, gHostip)))
	{
		CWDebugLog("data sock init error\n");
		CWNetworkCloseSocket(gWTPSocket);
		CWNetworkCloseSocket(gWTPDataSocket);
		CWLog("%s,init data sock failed when hot start.",strerror(errno));
		return CW_FALSE;
	}

	WTPDataChannelThreadRun = CW_TRUE ;
	
	if(!CWErr(CWCreateThread(&thread_receiveData, CWWTPReceiveDtlsDataPacket, (void*)gWTPDataSocket,0))) {
		CWDebugLog("Error starting Thread that receive DATA DTLS packet");
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
			memset(wlancreating, 0, sizeof(wlancreating));
			wlancreating->radio_id = ptr->radio_id;
			wlancreating->wlan_id = ptr->wlan_id;
		
			CWThread thread_receiveFrame_dot_11;
			CWThread thread_receiveFrame_dot_3;
			WTPWLanReceiveThreadRun = CW_TRUE ;
			if(!CWErr(CWCreateThread(&thread_receiveFrame_dot_11, CWWTPReceive802_11Frame, wlancreating,0))) {
				CWDebugLog("Error starting Thread that receive binding frame");
			}
			if(!CWErr(CWCreateThread(&thread_receiveFrame_dot_3, CWWTPReceive802_3Frame,wlancreating,0))) {
				CWDebugLog("Error starting Thread that receive binding frame");
			}
			receiveframethread = 1;

			/* pei add for check station aging, at 080703 */
			CWThread thread_netlink;
			CWDebugLog("start to create the thread check netlink");
			if(!CWErr(CWCreateThread(&thread_netlink, CWWTPCheckNetLink, NULL,0))) {
				CWDebugLog("Error starting Thread that check the netlink");
			}
			CWDebugLog("already create the thread check netlink");

			CW_FREE_OBJECT(wlancreating);
		}
		/*create the send sock for wlan*/
		{
			 struct sockaddr_ll toaddr1,toaddr2;
			 struct ifreq ifr;
			 int sockd1,sockd2;// 1 data 2 eap
			 char wlanname[IFNAMSIZ];
			 CWWTPGetWlanName(wlanname, ptr->radio_id, ptr->wlan_id);
			 if((sockd1 = socket(PF_PACKET,SOCK_RAW,htons(ETH_P_ALL))) < 0){
				CWDebugLog("socket init error");
			 }	
			 if((sockd2 = socket(PF_PACKET,SOCK_RAW,htons(ETH_P_ALL))) < 0){
				CWDebugLog("socket init error");
			 }
             
            /**** zhouke add ,for marvell ioctl ****/
#ifdef MARVELL_PLATFORM
            memset(&ifr, 0, sizeof(struct ifreq));
#endif
            /************ end ************/

			 strncpy(ifr.ifr_name,wlanname, sizeof(ifr.ifr_name));
			 if (ioctl(sockd1, SIOCGIFINDEX, &ifr) == -1){//bind to a interface
				 CWDebugLog("SIOCGIFINDEX1  failed");
			 }
			 if (ioctl(sockd2, SIOCGIFINDEX, &ifr) == -1){//bind to a interface
				CWDebugLog("SIOCGIFINDEX2  failed");
			 }

			 memset(&toaddr1, '\0',sizeof(toaddr1));
			 toaddr1.sll_family = AF_PACKET;
			 toaddr1.sll_protocol = htons(0x0019);
			 toaddr1.sll_ifindex = ifr.ifr_ifindex;
			 if(bind(sockd1,(struct sockaddr *)&toaddr1,sizeof(toaddr1)) < 0){
				CWDebugLog("send sock bind  error");
			 }
			 memset(&toaddr2, '\0',sizeof(toaddr2));
			 toaddr2.sll_family = AF_PACKET;
			 toaddr2.sll_protocol = htons(ETH_P_ALL);
			 toaddr2.sll_ifindex = ifr.ifr_ifindex;
			 if(bind(sockd2,(struct sockaddr *)&toaddr2,sizeof(toaddr2)) < 0){
				CWDebugLog("send sock bind  error");
			 }
			 ptr->wlan_sendsock = sockd1;
			 ptr->wlan_eapsock = sockd2;
			 
			CWDebugLog("wlancreating wlan_sendsock %d\n", ptr->wlan_sendsock);
			CWDebugLog("wlancreating wlan_eapsock %d\n", ptr->wlan_eapsock);
			CWDebugLog("send sock bind  OK\n");
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
			CWDebugLog("Error starting Thread to Monitor");
//			return CW_FALSE;
		}
	}
	if(gExtraInfoEnable)
	{
		if(!CWErr(CWCreateThread(&thread_extraInfo, CWWTPSetExtraInfo, NULL,0))) {
			CWDebugLog("Error starting Thread to get ExtraInfo");
//			return CW_FALSE;
		}
	}
	if(gStaInfoEnable)
	{
		if(!CWErr(CWCreateThread(&thread_staInfo, CWWTPGetStaInfo, NULL,0))) {
			CWDebugLog("Error starting Thread to get StationInfo");
//			return CW_FALSE;
		}
	}
	if(gIfStateEnable)
	{
		if(!CWErr(CWCreateThread(&thread_ifState, CWWTPGetIfState, NULL,0))) {
			CWDebugLog("Error starting Thread to get InterfaceState");
//			return CW_FALSE;
		}
	}
/*	{
		if(!CWErr(CWCreateThread(&thread_snooping, CWWTPCheckMsgFromOtherPro, NULL,0))) {
			CWDebugLog("Error starting Thread that check DHCPsnooping Report");
			return CW_FALSE;
		}
	}
*/
	if(gStaPktsStatisticEnable)
	{
		if(!CWErr(CWCreateThread(&thread_sta_pkts_statistic_report,CWWTPStaPktsStatisticReport,NULL,0))){
			CWDebugLog("Error starting Thread that sta pkts statistic Report");
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
		sprintf(filedir, "%s/radio%d", gWtpdConfDir, i);
		if(0 == access(filedir, 0))
		{
			memset(filedir, 0, 128);
			sprintf(filedir, "%s/radio%d/channel", gWtpdConfDir, i);
			if((fp = fopen(filedir, "r")) != NULL)
			{
				memset(str, 0, 24);
				fgets(str, sizeof(str), fp);
				gRadioInfoValue[i].channel = atoi(str);
				fclose(fp);
			}

			memset(filedir, 0, 128);
			sprintf(filedir, "%s/radio%d/txtype", gWtpdConfDir, i);
			if((fp = fopen(filedir, "r")) != NULL)
			{
				memset(str, 0, 24);
				fgets(str, sizeof(str), fp);
				gRadioInfoValue[i].txtype = atoi(str);
				fclose(fp);
			}

			memset(filedir, 0, 128);
			sprintf(filedir, "%s/radio%d/txpower", gWtpdConfDir, i);
			if((fp = fopen(filedir, "r")) != NULL)
			{
				memset(str, 0, 24);
				fgets(str, sizeof(str), fp);
				gRadioInfoValue[i].txpower = atoi(str);
				fclose(fp);
			}

			memset(filedir, 0, 128);
			sprintf(filedir, "%s/radio%d/rate", gWtpdConfDir, i);
			if((fp = fopen(filedir, "r")) != NULL)
			{
				memset(str, 0, 24);
				fgets(str, sizeof(str), fp);
				gRadioInfoValue[i].rate = atoi(str);
				fclose(fp);
			}

			memset(filedir, 0, 128);
			sprintf(filedir, "%s/radio%d/rateAutoEnable", gWtpdConfDir, i);
			if((fp = fopen(filedir, "r")) != NULL)
			{
				memset(str, 0, 24);
				fgets(str, sizeof(str), fp);
				gRadioInfoValue[i].rateAutoEnable = atoi(str);
				fclose(fp);
			}

			memset(filedir, 0, 128);
			sprintf(filedir, "%s/radio%d/radiotype", gWtpdConfDir, i);
			if((fp = fopen(filedir, "r")) != NULL)
			{
				memset(str, 0, 24);
				fgets(str, sizeof(str), fp);
				gRadioInfoValue[i].radiotype = atoi(str);
				fclose(fp);
			}

			memset(filedir, 0, 128);
			sprintf(filedir, "%s/radio%d/fragThreshold", gWtpdConfDir, i);
			if((fp = fopen(filedir, "r")) != NULL)
			{
				memset(str, 0, 24);
				fgets(str, sizeof(str), fp);
				gRadioInfoValue[i].fragThreshold = atoi(str);
				fclose(fp);
			}

			memset(filedir, 0, 128);
			sprintf(filedir, "%s/radio%d/rtsThreshold", gWtpdConfDir, i);
			if((fp = fopen(filedir, "r")) != NULL)
			{
				memset(str, 0, 24);
				fgets(str, sizeof(str), fp);
				gRadioInfoValue[i].rtsThreshold = atoi(str);
				fclose(fp);
			}

			memset(filedir, 0, 128);
			sprintf(filedir, "%s/radio%d/shortRetry", gWtpdConfDir, i);
			if((fp = fopen(filedir, "r")) != NULL)
			{
				memset(str, 0, 24);
				fgets(str, sizeof(str), fp);
				gRadioInfoValue[i].shortRetry = atoi(str);
				fclose(fp);
			}

			memset(filedir, 0, 128);
			sprintf(filedir, "%s/radio%d/longRetry", gWtpdConfDir, i);
			if((fp = fopen(filedir, "r")) != NULL)
			{
				memset(str, 0, 24);
				fgets(str, sizeof(str), fp);
				gRadioInfoValue[i].longRetry = atoi(str);
				fclose(fp);
			}

			memset(filedir, 0, 128);
			sprintf(filedir, "%s/radio%d/beaconInterval", gWtpdConfDir, i);
			if((fp = fopen(filedir, "r")) != NULL)
			{
				memset(str, 0, 24);
				fgets(str, sizeof(str), fp);
				gRadioInfoValue[i].beaconInterval = atoi(str);
				fclose(fp);
			}

			memset(filedir, 0, 128);
			sprintf(filedir, "%s/radio%d/preamble", gWtpdConfDir, i);
			if((fp = fopen(filedir, "r")) != NULL)
			{
				memset(str, 0, 24);
				fgets(str, sizeof(str), fp);
				gRadioInfoValue[i].preamble = atoi(str);
				fclose(fp);
			}

			memset(filedir, 0, 128);
			sprintf(filedir, "%s/radio%d/dtim", gWtpdConfDir, i);
			if((fp = fopen(filedir, "r")) != NULL)
			{
				memset(str, 0, 24);
				fgets(str, sizeof(str), fp);
				gRadioInfoValue[i].dtim = atoi(str);
				fclose(fp);
			}

			memset(filedir, 0, 128);
			sprintf(filedir, "%s/radio%d/gIsRadioEnable", gWtpdConfDir, i);
			if((fp = fopen(filedir, "r")) != NULL)
			{
				memset(str, 0, 24);
				fgets(str, sizeof(str), fp);
				gRadioInfoValue[i].gIsRadioEnable = atoi(str);
				fclose(fp);
			}

			memset(filedir, 0, 128);
			sprintf(filedir, "%s/radio%d/wlan_count", gWtpdConfDir, i);
			if((fp = fopen(filedir, "r")) != NULL)
			{
				memset(str, 0, 24);
				fgets(str, sizeof(str), fp);
				radio_wlan_count[i] = atoi(str);
				fclose(fp);
			}
			for(n=1; n<radio_wlan_count[i]+1; n++)
			{
				memset(&wlanCreate, 0, sizeof(CWWTPWlan));
				wlanCreate.radio_id = i;
				memset(filedir, 0, 128);
				sprintf(filedir, "%s/radio%d/wlaninfo/wlan%d", gWtpdConfDir, i, n);
				if((fp = fopen(filedir, "r")) != NULL)
				{
					memset(filedir, 0, 128);
					sprintf(filedir, "%s/radio%d/wlaninfo/wlan%d/wlan_id", gWtpdConfDir, i, n);
					if((fp = fopen(filedir, "r")) != NULL)
					{
						memset(str, 0, 24);
						fgets(str, sizeof(str), fp);
						wlanCreate.wlan_id = atoi(str);
						fclose(fp);
					}
					memset(filedir, 0, 128);
					sprintf(filedir, "%s/radio%d/wlaninfo/wlan%d/vlan_id", gWtpdConfDir, i, n);
					if((fp = fopen(filedir, "r")) != NULL)
					{
						memset(str, 0, 24);
						fgets(str, sizeof(str), fp);
						wlanCreate.vlan_id = atoi(str);
						fclose(fp);
					}
					memset(filedir, 0, 128);
					sprintf(filedir, "%s/radio%d/wlaninfo/wlan%d/wlan_updown_time", gWtpdConfDir, i, n);
					if((fp = fopen(filedir, "r")) != NULL)
					{
						memset(str, 0, 24);
						fgets(str, sizeof(str), fp);
						wlanCreate.wlan_updown_time = atoi(str);
						fclose(fp);
					}
					memset(filedir, 0, 128);
					sprintf(filedir, "%s/radio%d/wlaninfo/wlan%d/wlan_mac_mode", gWtpdConfDir, i, n);
					if((fp = fopen(filedir, "r")) != NULL)
					{
						memset(str, 0, 24);
						fgets(str, sizeof(str), fp);
						wlanCreate.wlan_mac_mode = atoi(str);
						fclose(fp);
					}
					memset(filedir, 0, 128);
					sprintf(filedir, "%s/radio%d/wlaninfo/wlan%d/wlan_tunnel_mode", gWtpdConfDir, i, n);
					if((fp = fopen(filedir, "r")) != NULL)
					{
						memset(str, 0, 24);
						fgets(str, sizeof(str), fp);
						wlanCreate.wlan_tunnel_mode = atoi(str);
						fclose(fp);
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
	sprintf(filedir, "%s/gApScanningEnable", gWtpdConfDir);
	if((fp = fopen(filedir, "r")) != NULL)
	{
		memset(str, 0, 24);
		fgets(str, sizeof(str), fp);
		gApScanningEnable = atoi(str);
		fclose(fp);
	}
	memset(filedir, 0, 128);
	sprintf(filedir, "%s/gApScanningInterval", gWtpdConfDir);
	if((fp = fopen(filedir, "r")) != NULL)
	{
		memset(str, 0, 24);
		fgets(str, sizeof(str), fp);
		gApScanningInterval = atoi(str);
		fclose(fp);
	}
	
	memset(filedir, 0, 128);
	sprintf(filedir, "%s/gAPThroughputInfoCollectEnable", gWtpdConfDir);
	if((fp = fopen(filedir, "r")) != NULL)
	{
		memset(str, 0, 24);
		fgets(str, sizeof(str), fp);
		gAPThroughputInfoCollectEnable = atoi(str);
		fclose(fp);
	}
	memset(filedir, 0, 128);
	sprintf(filedir, "%s/gAPThroughputInfoInterval", gWtpdConfDir);
	if((fp = fopen(filedir, "r")) != NULL)
	{
		memset(str, 0, 24);
		fgets(str, sizeof(str), fp);
		gAPThroughputInfoInterval= atoi(str);
		fclose(fp);
	}
	memset(filedir, 0, 128);
	sprintf(filedir, "%s/gApMonitorEnable", gWtpdConfDir);
	if((fp = fopen(filedir, "r")) != NULL)
	{
		memset(str, 0, 24);
		fgets(str, sizeof(str), fp);
		gApMonitorEnable = atoi(str);
		fclose(fp);
	}
	memset(filedir, 0, 128);
	sprintf(filedir, "%s/gApMonitorInterval", gWtpdConfDir);
	if((fp = fopen(filedir, "r")) != NULL)
	{
		memset(str, 0, 24);
		fgets(str, sizeof(str), fp);
		gApMonitorInterval = atoi(str);
		fclose(fp);
	}
	memset(filedir, 0, 128);
	sprintf(filedir, "%s/gExtraInfoEnable", gWtpdConfDir);
	if((fp = fopen(filedir, "r")) != NULL)
	{
		memset(str, 0, 24);
		fgets(str, sizeof(str), fp);
		gExtraInfoEnable = atoi(str);
		fclose(fp);
	}
	memset(filedir, 0, 128);
	sprintf(filedir, "%s/gExtraInfoInterval", gWtpdConfDir);
	if((fp = fopen(filedir, "r")) != NULL)
	{
		memset(str, 0, 24);
		fgets(str, sizeof(str), fp);
		gExtraInfoInterval = atoi(str);
		fclose(fp);
	}
	memset(filedir, 0, 128);
	sprintf(filedir, "%s/gStaInfoEnable", gWtpdConfDir);
	if((fp = fopen(filedir, "r")) != NULL)
	{
		memset(str, 0, 24);
		fgets(str, sizeof(str), fp);
		gStaInfoEnable = atoi(str);
		fclose(fp);
	}
	memset(filedir, 0, 128);
	sprintf(filedir, "%s/gStaInfoInterval", gWtpdConfDir);
	if((fp = fopen(filedir, "r")) != NULL)
	{
		memset(str, 0, 24);
		fgets(str, sizeof(str), fp);
		gStaInfoInterval = atoi(str);
		fclose(fp);
	}
	memset(filedir, 0, 128);
	sprintf(filedir, "%s/gIfStateEnable", gWtpdConfDir);
	if((fp = fopen(filedir, "r")) != NULL)
	{
		memset(str, 0, 24);
		fgets(str, sizeof(str), fp);
		gIfStateEnable = atoi(str);
		fclose(fp);
	}
	memset(filedir, 0, 128);
	sprintf(filedir, "%s/gIfStateInterval", gWtpdConfDir);
	if((fp = fopen(filedir, "r")) != NULL)
	{
		memset(str, 0, 24);
		fgets(str, sizeof(str), fp);
		gIfStateInterval = atoi(str);
		fclose(fp);
	}
	memset(filedir, 0, 128);
	sprintf(filedir, "%s/gFloodingDetectEnable", gWtpdConfDir);
	if((fp = fopen(filedir, "r")) != NULL)
	{
		memset(str, 0, 24);
		fgets(str, sizeof(str), fp);
		gFloodingDetectEnable = atoi(str);
		fclose(fp);
	}
	memset(filedir, 0, 128);
	sprintf(filedir, "%s/gSpoofingDetectEnable", gWtpdConfDir);
	if((fp = fopen(filedir, "r")) != NULL)
	{
		memset(str, 0, 24);
		fgets(str, sizeof(str), fp);
		gSpoofingDetectEnable = atoi(str);
		fclose(fp);
	}
	memset(filedir, 0, 128);
	sprintf(filedir, "%s/gWeakIVDetectEnable", gWtpdConfDir);
	if((fp = fopen(filedir, "r")) != NULL)
	{
		memset(str, 0, 24);
		fgets(str, sizeof(str), fp);
		gWeakIVDetectEnable = atoi(str);
		fclose(fp);
	}
	memset(filedir, 0, 128);
	sprintf(filedir, "%s/gFloodAttackCnt", gWtpdConfDir);
	if((fp = fopen(filedir, "r")) != NULL)
	{
		memset(str, 0, 24);
		fgets(str, sizeof(str), fp);
		gFloodAttackCnt = atoi(str);
		fclose(fp);
	}
	memset(filedir, 0, 128);
	sprintf(filedir, "%s/gFloodProbeCnt", gWtpdConfDir);
	if((fp = fopen(filedir, "r")) != NULL)
	{
		memset(str, 0, 24);
		fgets(str, sizeof(str), fp);
		gFloodProbeCnt = atoi(str);
		fclose(fp);
	}
	memset(filedir, 0, 128);
	sprintf(filedir, "%s/gFloodWidsIntval", gWtpdConfDir);
	if((fp = fopen(filedir, "r")) != NULL)
	{
		memset(str, 0, 24);
		fgets(str, sizeof(str), fp);
		gFloodWidsIntval = atoi(str);
		fclose(fp);
	}

	memset(filedir, 0, 128);
	sprintf(filedir, "%s/ac_addr", gWtpdConfDir);
	if((fp = fopen(filedir, "r")) != NULL)
	{
		memset(str, 0, 24);
		fgets(str, sizeof(str), fp);
		fclose(fp);
	}
//	printf("---str:%s\n", str);
	CW_CREATE_OBJECT_ERR(gACInfoPtr, CWACInfoValues, return CW_QUIT;);
	CWNetworkGetAddressForHost(str, &(gACInfoPtr->preferredAddress));
	CWUseSockNtop(&(gACInfoPtr->preferredAddress), CWDebugLog(str););
	if(gCWForceMTU > 0) {
		gWTPPathMTU = gCWForceMTU;
	}

	memset(filedir, 0, 128);
	sprintf(filedir, "%s/gEchoInterval", gWtpdConfDir);
	if((fp = fopen(filedir, "r")) != NULL)
	{
		memset(str, 0, 24);
		fgets(str, sizeof(str), fp);
		gEchoInterval = atoi(str);
		fclose(fp);
	}
	memset(filedir, 0, 128);
	sprintf(filedir, "%s/gCWNeighborDeadInterval", gWtpdConfDir);
	if((fp = fopen(filedir, "r")) != NULL)
	{
		memset(str, 0, 24);
		fgets(str, sizeof(str), fp);
		gCWNeighborDeadInterval = atoi(str);
		fclose(fp);
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
	if((fd = open(file,O_RDONLY))== -1)
	{
		dpf("open file %s failed! \t Use Default Value.\n",file);	
		return CW_FALSE;
	}
	else
	{
		read(fd,str,64);
		dpf("from %s\tget %s = %s\n",file,filename,str);
		close(fd);
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
			printf("%s does't exists,hot restart failure...sysreboot\n",filepath_EchoInterval);
		}
		CWDebugLog("%s does't exists,hot restart failure...sysreboot",filepath_EchoInterval);
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
	if(CWGetConfigFromFile(filepath,"ac_addr",str))
	{
		CW_CREATE_OBJECT_ERR(gACInfoPtr, CWACInfoValues, CWLog("malloc failed for gACInfoPtr,hot start init failed");return CW_QUIT;);
		CWNetworkGetAddressForHost(str, &(gACInfoPtr->preferredAddress));
		CWUseSockNtop(&(gACInfoPtr->preferredAddress), CWDebugLog(str););
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
		sprintf(tmppath,"%sradio%d/",filepath,i);
		if(debug_print && isBackup)
			printf("tmppath = %s\n",tmppath);
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
		if(CWGetConfigFromFile(tmppath,"cwmMode",str))
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
			sprintf(tmp,"ath.%d-[0-9]*",i);
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
						sprintf(wlanpath,"%s/%s/",tmppath,ptr->d_name);
						dpf("get wlaninfo from %s\n",wlanpath);				
						memset(&wlanCreate, 0, sizeof(CWWTPWlan));
						if(CWGetConfigFromFile(wlanpath,"radio_id",str))
							wlanCreate.radio_id=atoi(str);
						if(CWGetConfigFromFile(wlanpath,"wlan_id",str))
							wlanCreate.wlan_id=atoi(str);
						if(wlanCreate.wlan_id== 0)
						{
							CWLog("wlan_id got from config_file is 0,invalid,hot_start init failed");
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
						
						wlanCreate.wlan_id = CWCreateLocalWlanID(wlanCreate.radio_id,wlanCreate.wlan_id);
						
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

	/* AC address */
	struct sockaddr *sa=(struct sockaddr*) (&gACInfoPtr->preferredAddress);
	struct sockaddr_in	*sin = (struct sockaddr_in *) sa;
	inet_ntop(AF_INET, &sin->sin_addr, temp_str, 128);
//	printf("ac:%s\n", temp_str);
//	printf("=======before ping test=======\n");
	CWPingTestAndSave(temp_str,pingfile);
//	printf("=======after ping test=======\n");
	
	
	memset(temp_str, 0, 128);
	sprintf(temp_str, "cat %s | awk -F ' ' '/lost/ {print $7}' | awk -F '%%' '{print $1}'", pingfile);
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
	int readBytes = msgPtr->offset;

	dpf("------readBytes:%d  msgPtr->offset:%d------\n", readBytes, msgPtr->offset);
//	CWCaptrue(readBytes, msgPtr->msg);
	if(msgPtr == NULL) 
		return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	
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
	n = sendto(wlan_sock, sendmsg, (readBytes-header_len), 0x0, NULL, 0); //pei 0917 del warning

//			printf("send assoc response\n");
	if(n<0)
		printf("data channel send error\n");

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
		CWSetThinapState(1);
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

		if(CWSta_Access_limit() == CW_TRUE)
		{
			CWWTPDebugLog("sta_access_limit OK");
		}

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
				system(cmdbuf);
			}
			else{
				memset(cmdbuf, 0, 64);
				sprintf(cmdbuf,"iwpriv wifi%d txchainmask 3", n); /* 11n Set the chain masks */
				system(cmdbuf);
				sprintf(cmdbuf,"iwpriv wifi%d rxchainmask 3", n); /* 11n Set the chain masks */
				system(cmdbuf);
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
		CWSetThinapState(1);
		
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
	if(gAPThroughputInfoCollectEnable)
	{
		sleepinterval = 40;
//		gAPThroughputInfoThreadEnable = 1;
		
		if(!CWErr(CWCreateThread(&thread_wtp_throughput_info, acw_wtp_throughput_info, (void *)sleepinterval,0))){
			CWDebugLog("Error starting Thread to wtp_throughput_info");
		}
	}
	if(!CWErr(CWCreateThread(&thread_snooping, CWWTPCheckMsgFromOtherPro, NULL,0))) {
		CWDebugLog("Error starting Thread that check DHCPsnooping Report");
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
		timenow.tv_sec = time(0) + gCWNeighborDeadInterval + 240;	 //greater than NeighborDeadInterval
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
				CWDebugLog("CWThreadMutexUnlock ");
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

			if (!CWErr(CWWTPManageGenericRunMessage(&msg))) 
			{
				int ret=CWErrorGetLastErrorCode();
				if(ret == CW_ERROR_INVALID_FORMAT) 
				{
					// Log and ignore message
					CWErrorHandleLast();
					CWDebugLog("--> Received something different from a valid Run Message");
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
					CWErrorHandleLast();
					CWDebugLog("--> Received something different from a valid Run Message");
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
		if(WTPGetApRebootFlag())
		{
			CWac_lose();
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
		printf("*******************************************************************************************************\n");
		printf("*******************************************Begin to test***********************************************\n");
		printf("*******************************************************************************************************\n");
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

	/************************WTP Event request del sta***********************/
	if (!CWErr(CWWTPManageGenericRunMessage(&testmsg0))) 
	{
				if(CWErrorGetLastErrorCode() == CW_ERROR_INVALID_FORMAT) 
				{
					// Log and ignore message
					CWErrorHandleLast();
					CWDebugLog("--> Received something different from a valid Run Message");
				} 
				else 
				{
					CW_FREE_PROTOCOL_MESSAGE(testmsg0);
					CWDebugLog("--> Critical Error Managing Generic Run Message... we enter RESET State");
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
					CWDebugLog("--> Received something different from a valid Run Message");
				} 
				else 
				{
					CW_FREE_PROTOCOL_MESSAGE(testmsg1);
					CWDebugLog("--> Critical Error Managing Generic Run Message... we enter RESET State");
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
					CWDebugLog("--> Received something different from a valid Run Message");
				} 
				else 
				{
					CW_FREE_PROTOCOL_MESSAGE(testmsg2);
					CWDebugLog("--> Critical Error Managing Generic Run Message... we enter RESET State");
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
					CWDebugLog("--> Received something different from a valid Run Message");
				} 
				else 
				{
					CW_FREE_PROTOCOL_MESSAGE(testmsg3);
					CWDebugLog("--> Critical Error Managing Generic Run Message... we enter RESET State");
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
					CWDebugLog("--> Received something different from a valid Run Message");
				} 
				else 
				{
					CW_FREE_PROTOCOL_MESSAGE(testmsg4);
					CWDebugLog("--> Critical Error Managing Generic Run Message... we enter RESET State");
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
					CWDebugLog("--> Received something different from a valid Run Message");
				} 
				else 
				{
					CW_FREE_PROTOCOL_MESSAGE(testmsg5);
					CWDebugLog("--> Critical Error Managing Generic Run Message... we enter RESET State");
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
					CWDebugLog("--> Received something different from a valid Run Message");
				} 
				else 
				{
					CW_FREE_PROTOCOL_MESSAGE(testmsg6);
					CWDebugLog("--> Critical Error Managing Generic Run Message... we enter RESET State");
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
					CWDebugLog("--> Received something different from a valid Run Message");
				} 
				else 
				{
					CW_FREE_PROTOCOL_MESSAGE(testmsg7);
					CWDebugLog("--> Critical Error Managing Generic Run Message... we enter RESET State");
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
				CWProtocolResultCode resultCode = CW_PROTOCOL_FAILURE;
				CWProtocolConfigurationUpdateRequestValues values;
				unsigned char radio_id;
				unsigned char oprat_state_cause;
				unsigned char protected_flag = 0;
				
				CWResetTimers();
				CWDebugLog("Configuration Update Request received");

				if(!CWParseConfigurationUpdateRequest((msgPtr->msg)+(msgPtr->offset), len, &values,&protected_flag)) 
					return CW_FALSE;
				
				radio_id = values.adminState->radio_id;
				CWDebugLog("values.adminState->radio_id = %d\n", values.adminState->radio_id);
				
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
				CWResetTimers();
				CWDebugLog("Clear Configuration Request received");
				break;
			}

			case CW_MSG_TYPE_VALUE_STATION_CONFIGURATION_REQUEST:
			{
				CWProtocolResultCode resultCode = CW_PROTOCOL_FAILURE;
				CWProtocolConfigurationUpdateRequestValues values;/*now use this struct because it works ok I wiil change it'name later*/
				CWResetTimers();
				CWDebugLog("Station Configuration Request received");
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
				CWResetTimers();
				CWDebugLog("Echo Response received");
				break;
			}
			case CW_MSG_TYPE_VALUE_IEEE80211_WLAN_CONFIGURATION_REQUEST:
			{
				CWProtocolResultCode resultCode = CW_PROTOCOL_FAILURE;
				CWProtocolConfigurationUpdateRequestValues values;/*I wiil add a new struct if it needs*/
				CWResetTimers();
				CWDebugLog("wlan configuration request received");
				if(!CWParseIEEE80211WlanConfigurationRequest((msgPtr->msg)+(msgPtr->offset), len, &values))
					return CW_FALSE;
				CWSta_Access_limit();
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
				CWResetTimers();
				CWDebugLog("CW_MSG_TYPE_VALUE_WTP_EVENT_RESPONSE received");
				break;
			}

			default:
				//## We can't recognize the received Request so we have to send
				//## a corresponding response containing a failure result code
				CWDebugLog("--> Not valid Request in Run State... we send a failure Response");
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
						CWDebugLog("Error sending message");
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
						CWDebugLog("Error sending message");
						CWFreeMessageFragments(messages, fragmentsNum);
						CW_FREE_OBJECT(messages);
						return CW_FALSE;
					}
				}
			}
			CWDebugLog("Message Sent\n");
			CWFreeMessageFragments(messages, fragmentsNum);
			CW_FREE_OBJECT(messages);
		}	
	} 
	else //## we have received a Response
	{
		CWResetTimers();
		switch(controlVal.messageTypeValue) 
		{
			case CW_MSG_TYPE_VALUE_CHANGE_STATE_EVENT_RESPONSE:
				CWDebugLog("Change State Event Response received");
				break;
		
			case CW_MSG_TYPE_VALUE_WTP_EVENT_RESPONSE:
				CWDebugLog("WTP Event Response received");	
				break;
	
			case CW_MSG_TYPE_VALUE_DATA_TRANSFER_RESPONSE:
				CWDebugLog("Data Transfer Response received");
				break;

			default:
				//## We can't recognize the received Response: we ignore the
				//## message and log the event.
				CWDebugLog("Received Message not valid in Run State");
				return CWErrorRaise(CW_ERROR_INVALID_FORMAT, "Received Message not valid in Run State");
		}
	
//		CWResetPendingMsgBox(&(gPendingRequestMsgs[pendingMsgIndex]));
	}
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
		CWDebugLog("CWStartHeartbeatTimer failed\n");
		CWWTPDebugLog("CWStartHeartbeatTimer failed\n");
		return;
	}

	CWDebugLog("\n");
	CWDebugLog("#________ Echo Request Message (Run) ________#");

	/* Send WTP Event Request */
	seqNum = CWGetSeqNum();

	if(!CWAssembleEchoRequest(&messages, &fragmentsNum, gWTPPathMTU, seqNum, msgElemList)){
		int i;
		CWDebugLog("Failure Assembling Echo Request");
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
				CWDebugLog("Failure sending Request");
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
				CWDebugLog("Failure sending Request");
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
		CW_FREE_OBJECT(messages);
	}

}

void CWWTPNeighborDeadTimerExpired(void *arg) 
{
	CWLog("WTP NeighborDead Timer Expired ... wtpd reboot");
	WTPInitConfig();
	CWPingTest("/jffs/pingtest1");
	_system("wtpd > /dev/null 2>&1 &");
	_system("/usr/sbin/ap_monitor &");
	exit(0);
	return;  /* add, when compare with capwap 0.93.3 */
}


CWBool CWStartHeartbeatTimer(){
//	CWDebugLog("gEchoInterval %d ",gEchoInterval);
	if ((gCWHeartBeatTimerID = timer_add(gEchoInterval, 0, &CWWTPHeartBeatTimerExpiredHandler, NULL)) == -1 ) {
		return CW_FALSE;
	}
	
	CWDebugLog("Heartbeat Timer Started");

	return CW_TRUE;
}


CWBool CWStopHeartbeatTimer(){
	
	timer_rem(gCWHeartBeatTimerID,0);

//	CWDebugLog("Heartbeat Timer Stopped");
	
	return CW_TRUE;
}


CWBool CWStartNeighborDeadTimer(){
	
	if ((gCWNeighborDeadTimerID = timer_add(gCWNeighborDeadInterval, 0, &CWWTPNeighborDeadTimerExpired, NULL)) == -1) {
		return CW_FALSE;
	}
	CWDebugLog("NeighborDead Timer Started");
	gNeighborDeadTimerSet = CW_TRUE;
	return CW_TRUE;
}


CWBool CWStopNeighborDeadTimer(){
	
	timer_rem(gCWNeighborDeadTimerID,0);

	CWDebugLog("NeighborDead Timer Stopped");

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
	system(cmdbuf);
#endif
	
	if(0 ==access("/tmp/neighbordead", 0))
	{
		CWLog("WTP receive a msg from AC, we consider WTP works well.");
		_system("rm -rf /tmp/neighbordead");
	}
	
	CWSetThinapCheckTimer();
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
		CWDebugLog("Assembling Echo Request...");

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
	
//	CWLog("Echo Request Assembled");
	
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
	
	CWDebugLog("Assembling WTP Event Request Del Sta...");

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
	
	CWDebugLog("WTP Event Request Del Sta Assembled");
	
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
		
	CWDebugLog("Assembling WTP Event Request...");

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

	CWDebugLog("WTP Event Request Assembled");
	
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
//	CWDebugLog("Result Code: %d", code);
				
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
	
	CWDebugLog("Assembling Configuration Update Response...");
	
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
	
	CWDebugLog("Configuration Update Response Assembled");
	
	return CW_TRUE;
}
/*_______________________________________________________________*/
/*  *******************___PARSE FUNCTIONS___*******************  */
CWBool CWParseConfigurationUpdateRequest (char *msg, int len, CWProtocolConfigurationUpdateRequestValues *valuesPtr,unsigned char *protected_flag) 
{
	CWBool bindingMsgElemFound=CW_FALSE;
	CWProtocolMessage completeMsg;
	
	if(msg == NULL || valuesPtr == NULL) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	
	CWDebugLog("Parsing Configuration Update Request...");
	
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
//	CWDebugLog("completeMsg.offset %d",completeMsg.offset);
//	CWDebugLog("len%d",len);

	// parse message elements
	while(completeMsg.offset < len) {
		unsigned short int elemType=0;// = CWProtocolRetrieve32(&completeMsg);
		unsigned short int elemLen=0;// = CWProtocolRetrieve16(&completeMsg);
		
		CWParseFormatMsgElem(&completeMsg,&elemType,&elemLen);		

		CWDebugLog("Parsing Message Element: %u, elemLen: %u", elemType, elemLen);
		
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
	//i=0;
	completeMsg.offset = 0;
	while(completeMsg.offset < len) {
		unsigned short int type=0;
		unsigned short int elemLen=0;
		
		CWParseFormatMsgElem(&completeMsg,&type,&elemLen);		

		switch(type) {
			case CW_MSG_ELEMENT_TIMESTAMP_CW_TYPE:{	//pei add 0218
				CWDebugLog("get the time stamp message");
				valuesPtr->timeStampValue = CWProtocolRetrieve32(&completeMsg);
				CWWTPDebugLog("time stamp value:%d", valuesPtr->timeStampValue);
				CWDebugLog("CW_MSG_ELEMENT_TIMESTAMP_CW_TYPE Parsed");
				break;
			}
			case CW_MSG_ELEMENT_RADIO_ADMIN_STATE_CW_TYPE:{	//pei add 0724
				CWDebugLog("get the WTP radio administrative state message");
				if(!(CWParseRadioAdminState(&completeMsg, elemLen,valuesPtr->adminState)))
				{
					CW_FREE_OBJECT(valuesPtr->adminState);
					return CW_FALSE; // will be handled by the caller
				}
				CWDebugLog("CW_MSG_ELEMENT_RADIO_ADMIN_STATE_CW_TYPE Parsed");
				break;
			}
			case CW_MSG_ELEMENT_WTP_STATIC_IP_CW_TYPE:{	//pei add 0214
				CWDebugLog("get the set WTP static IP message");
				if(!(CWParseWTPStaticIP(&completeMsg, elemLen,valuesPtr->WTPStaticIP)))
				{
					CW_FREE_OBJECT(valuesPtr->WTPStaticIP);
					return CW_FALSE; // will be handled by the caller
				}
				CWDebugLog("CW_MSG_ELEMENT_WTP_STATIC_IP_CW_TYPE Parsed");
				break;
			}
			case CW_MSG_ELEMENT_WTP_STATIC_IP_CW_TYPE_V2:{	//pei add 0214
				CWDebugLog("get the set WTP static IP V2 message");
				if(!(CWParseWTPStaticIPV2(&completeMsg, elemLen,valuesPtr->WTPStaticIPV2)))
				{
					CW_FREE_OBJECT(valuesPtr->WTPStaticIPV2);
					return CW_FALSE; // will be handled by the caller
				}
				CWDebugLog("CW_MSG_ELEMENT_WTP_STATIC_IP_CW_TYPE_V2 Parsed");
				break;
			}
			case CW_MSG_ELEMENT_ETH_SET_CW_TYPE:{
				CWDebugLog("get the set eth config message");
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
				CWDebugLog("CW_MSG_ELEMENT_ETH_SET_CW_TYPE Parsed");
				break;
			}
			case CW_MSG_ELEMENT_ALL_TIME_SET_CW_TYPE:{
				CWDebugLog("get the set time config message");
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
				CWDebugLog("CW_MSG_ELEMENT_ALL_TIME_SET_CW_TYPE Parsed");
				break;
			}
			case CW_MSG_ELEMENT_WTP_11N_MCS_CWMODE_GUARDINTERVAL:{
				(*protected_flag)++;
				CWDebugLog("get the set WTP Mode 11N parameter message");
				if(!(CWParseWTPModeNPara(&completeMsg, elemLen,valuesPtr->WTPModeNPara)))
				{
					CW_FREE_OBJECT(valuesPtr->WTPModeNPara);
					return CW_FALSE; // will be handled by the caller
				}
				CWDebugLog("CW_MSG_ELEMENT_WTP_11N_MCS_CWMODE_GUARDINTERVAL Parsed");
				break;
			}
			case CW_MSG_ELEMENT_VENDOR_SPEC_PAYLOAD_CW_TYPE:{	//pei add 1118
				unsigned char value;
				value = CWProtocolRetrieve8(&completeMsg);
				CWWTPDebugLog("value:%d", value);
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
								printf("runState CWParseMaxThroughput error\n");
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
							if(!CWParseGetStaPktsStatisticRpt(&completeMsg, element_len, valuesPtr->getStaPktsStatisticRpt))
							{
								CW_FREE_OBJECT(valuesPtr->getStaPktsStatisticRpt);
								return CW_FALSE;
							}
							break;
						default:
							completeMsg.offset += element_len;
							break;
					}
				}
				//CWWTPDebugLog("CW_MSG_ELEMENT_VENDOR_SPEC_PAYLOAD_CW_TYPE Parsed");
				break;
			}
			default:
				completeMsg.offset += elemLen;
				break;
		}
	}
	
//	CWDebugLog("completeMsg.offset %d",completeMsg.offset);
//	CWDebugLog("len%d",len);
	
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
	
	CWDebugLog("Parsing WTP Event Response...");
	
	completeMsg.msg = msg;
	completeMsg.offset = 0;
	
	if(!(CWParseControlHeader(&completeMsg, &controlVal))) return CW_FALSE; // error will be handled by the caller
	
	// different type
	if(controlVal.messageTypeValue != CW_MSG_TYPE_VALUE_WTP_EVENT_RESPONSE)
		return CWErrorRaise(CW_ERROR_INVALID_FORMAT, "Message is not WTP Event Response as Expected");
	
	if(controlVal.seqNum != seqNum) return CWErrorRaise(CW_ERROR_INVALID_FORMAT, "Different Sequence Number");
	
	controlVal.msgElemsLen -= CW_CONTROL_HEADER_OFFSET_FOR_MSG_ELEMS; // skip timestamp
	
	if(controlVal.msgElemsLen != 0 ) return CWErrorRaise(CW_ERROR_INVALID_FORMAT, "WTP Event Response must carry no message element");

	CWDebugLog("WTP Event Response Parsed...");

	return CW_TRUE;
}


/*______________________________________________________________*/
/*  *******************___SAVE FUNCTIONS___*******************  */
CWBool CWSaveWTPEventResponseMessage (void *WTPEventResp)
{
	CWDebugLog("Saving WTP Event Response...");
	CWDebugLog("WTP Response Saved");
	return CW_TRUE;
}

CWBool CWSaveConfigurationUpdateRequest(CWProtocolConfigurationUpdateRequestValues *valuesPtr, CWProtocolResultCode* resultCode, unsigned char *operat_state_cause,unsigned char radioId,unsigned char protected_flag)
{
	char cmdbuf[256];
	CWDebugLog("CWSaveConfigurationUpdateRequest ");
	*resultCode=CW_PROTOCOL_SUCCESS;
    
    /**** zhouke add ,for marvell iwconfig wifi commit ****/
#ifdef MARVELL_PLATFORM
    int marvell_flag = 0;
#endif
    /********************** end *************************/

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
		int i,j,done = 0;

		//done = 1 ;
		goto _exit_lable;//demp add by diaowq@20120706
		
		char wlanname[32] = {0},str[32] = {0};
		
		for(i = 0; i < 2 ; i++)
		{
			for(j = i*8; j < 8/*vap count per radio*/; j++)
			{
				if(gWlanIDMap[j] != -1){
					CWWTPGetWlanName(wlanname, i, gWlanIDMap[j]);
					sprintf(cmdbuf, "echo \"%s\" |awk '{if(match($0,\"%s\")){print 1}else{print 0}}' ", valuesPtr->cmd, wlanname );
					PopenFile(cmdbuf, str,sizeof(str));
					CWWTPDebugLog("popen cmd:%s\npopen return value:%s", cmdbuf, str);
					if(atoi(str) == 1){
						spf_cmd("echo \"%s\" | sed 's/%s/ath.%d-%d/g'",valuesPtr->cmd, wlanname, i, j);
						done = 1;
						goto _exit_lable;
					}
				}
				else
					break;
			}
		}
	_exit_lable:
		if(done == 0){
			spf_cmd("%s",valuesPtr->cmd);
		}
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
        
#ifdef MARVELL_PLATFORM
        marvell_flag = 1;
#endif
        
		CW_FREE_OBJECT(valuesPtr->getMcsMask);
	}
	
	if(valuesPtr->getStaPktsStatisticRptCount>0)  
	{
		if(!CWGetStaPktsStatisticRpt(valuesPtr->getStaPktsStatisticRpt))
		{
			CW_FREE_OBJECT(valuesPtr->getStaPktsStatisticRpt);
			return CW_FALSE;
		}
		CW_FREE_OBJECT(valuesPtr->getStaPktsStatisticRpt);
	}
	
	if(protected_flag != 0 )
	{
		_system("killall sta_access_limit");
		if(system("sta_access_limit &") == 0){
			CWWTPDebugLog("sta_access_limit OK");
		}
		if(gApScanningEnable == 1){
			memset(cmdbuf, 0, 128);
			_system("killall iwlist_scan > /dev/null 2>&1");
			CWSetInterfaceUp(_iwlist_scan_if_name_for_wifi0_,0);
			if(0==Check_Interface_State(_iwlist_scan_if_name_for_wifi1_))
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

#ifdef MARVELL_PLATFORM
        marvell_flag = 1;
#endif
		CW_FREE_OBJECT(valuesPtr->WTPModeNPara);
	}
	if (valuesPtr->bindingValues!=NULL) 
	{
		if(!CWBindingSaveConfigurationUpdateRequest(valuesPtr->bindingValues, resultCode)) 
			return CW_FALSE;
#ifdef MARVELL_PLATFORM
        marvell_flag = 1;
#endif
    }

    /**** zhouke add ,for marvell iwconfig wifi commit ****/
#ifdef MARVELL_PLATFORM
    if(marvell_flag)
    {
        CWSetWifiCommit(radioId);
    }
#endif
    /********************** end *************************/
    
	if( protected_flag !=0)	
	{
		if(gApScanningEnable == 1){
			char wifi0mode[10]={0};
			char wifi1mode[10]={0};
			CWSetInterfaceUp(_iwlist_scan_if_name_for_wifi0_,1);
			if(0==Check_Interface_State(_iwlist_scan_if_name_for_wifi1_))
				CWSetInterfaceUp(_iwlist_scan_if_name_for_wifi1_,1);
			memset(cmdbuf, 0, 128);
			if((gRadioInfoValue[0].radiotype&IEEE80211_PARAM_MODE_11a)!=0)
				memcpy(wifi0mode,"11a",sizeof("11a"));
			else memcpy(wifi0mode,"11bg",sizeof("11bg"));
			
			if(0==Check_Interface_State(_iwlist_scan_if_name_for_wifi1_)){
				if((gRadioInfoValue[1].radiotype&IEEE80211_PARAM_MODE_11a)!=0)
					memcpy(wifi1mode,"11a",sizeof("11a"));
				else
					memcpy(wifi1mode,"11bg",sizeof("11bg"));
				sprintf(cmdbuf, "/usr/sbin/iwlist_scan -m %s -c %s -I %d -i 5 -n 2 > /dev/null 2>&1 &",wifi0mode,wifi1mode, gApScanningInterval/3>60?gApScanningInterval/3:60);
			}
			else
				sprintf(cmdbuf, "/usr/sbin/iwlist_scan -m %s -I %d -i 5 -n 1 > /dev/null 2>&1 &",wifi0mode,gApScanningInterval/3>60?gApScanningInterval/3:60);
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

	CWDebugLog("Change State Event Response Parsed");
	CWDebugLog("#########################");
	CWDebugLog("###### STO DENTRO #######");
	CWDebugLog("#########################");

	return CW_TRUE;
}
*/

CWBool CWParseIEEE80211WlanConfigurationRequest(char *msg, int len, CWProtocolConfigurationUpdateRequestValues *valuesPtr) 
{
	CWBool bindingMsgElemFound=CW_FALSE;
	CWProtocolMessage completeMsg;
	
	if(msg == NULL || valuesPtr == NULL) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	
	CWDebugLog("Parsing IEEE80211 wlan configuration Request");
	
	completeMsg.msg = msg;
	completeMsg.offset = 0;

	valuesPtr->bindingValues = NULL;

	// parse message elements
	//CWDebugLog("len:%d",len);
	//CWDebugLog("ofoset:%d",completeMsg.offset);
	while(completeMsg.offset < len) {
		unsigned short int elemType=0;// = CWProtocolRetrieve32(&completeMsg);
		unsigned short int elemLen=0;// = CWProtocolRetrieve16(&completeMsg);
		
		CWParseFormatMsgElem(&completeMsg,&elemType,&elemLen);		

		CWDebugLog("Parsing Message Element: %u, elemLen: %u", elemType, elemLen);

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
	
	CWDebugLog("Assembling wlan configuration Response...");
	CW_CREATE_OBJECT_ERR(msgElems, CWProtocolMessage, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	CWDebugLog("msg create OK");
	CW_CREATE_OBJECT_ERR(msgElemsBinding, CWProtocolMessage, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	CWDebugLog("msgBinding create OK");
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
		CWWTPDebugLog("%s,wlan name %s",__func__,wlanname);
        
        /**** zhouke add ,for marvell ioctl ****/
#ifdef MARVELL_PLATFORM
        memset(&ifr, 0, sizeof(struct ifreq));
#endif
        /************ end ************/

		strncpy(ifr.ifr_name,wlanname,IFNAMSIZ);
		if (ioctl(sock, SIOCGIFHWADDR, &ifr)==-1) {
			CWDebugLog("get wlan mAC error\n");
		}
		else{
			for (i=0; i<MAC_ADDR_LEN; i++)	{
				bssid[i]=(unsigned char)ifr.ifr_hwaddr.sa_data[i];
				dpf("%02x ",bssid[i]);
//				dpf("%02x ",bssid[i]);
			}
			dpf("\n");
//			printf("\n");
			for(ptr = wtp_wlan_list;ptr != NULL;ptr = ptr->next){  //pei add for format capwap header at 090413
				if((ptr->radio_id == created_radio_id)&&(ptr->wlan_id == created_wlan_id)){
					memcpy(ptr->wlan_bssid, bssid, MAC_ADDR_LEN);
					break;
				}
			}
		}
		{
			CWSetInterfaceUp(wlanname,1); 
		}
	}
	else{
		radio_id = 2; //pei modified 1 to 2 at 090326 for 2radio
		CWDebugLog("wlan delete response do not need bssid");
	}
	if(sock != -1)
		close(sock);
	//created_wlan_id = gWlanIDMap[created_wlan_id]; // no convert add by diaowq@20120726
	CWWTPDebugLog("%s,created_wlan_id:%d",__func__,created_wlan_id);
	if (!(CWAssembleMsgElemResultCode(msgElems,resultCode))||
		!(CWAssembleAssignedWTPBssid(msgElemsBinding, (char *)bssid, created_wlan_id, radio_id))) { //pei 0917 del warning
		CWDebugLog("assembleIEEE80211WlanCOnfigurationResponse failed");
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
	
	CWDebugLog("Configuration wlan response Assembled");

	return CW_TRUE;
}
CWBool CWParseSTAConfigurationRequest(char *msg, int len, CWProtocolConfigurationUpdateRequestValues *valuesPtr)
{
	CWBool bindingMsgElemFound=CW_FALSE;
	CWProtocolMessage completeMsg;
	
	if(msg == NULL || valuesPtr == NULL) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	
	CWDebugLog("Parsing STA configuration Request");
	
	completeMsg.msg = msg;
	completeMsg.offset = 0;

	valuesPtr->bindingValues = NULL;
// parse message elements
	CWDebugLog("len:%d",len);
	CWDebugLog("ofoset:%d",completeMsg.offset);
	while(completeMsg.offset < len) {
		unsigned short int elemType=0;// = CWProtocolRetrieve32(&completeMsg);
		unsigned short int elemLen=0;// = CWProtocolRetrieve16(&completeMsg);
		
		CWParseFormatMsgElem(&completeMsg,&elemType,&elemLen);		

		CWDebugLog("Parsing Message Element: %u, elemLen: %u", elemType, elemLen);

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
	CWDebugLog("wlan configuration request parsed return CW_true");
	
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
	
	CWDebugLog("Assembling station config Response...");
	
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
	
	CWDebugLog("Configuration Update Response Assembled");
	
	return CW_TRUE;

}


CWBool CWParseWTPEventRequestDeleteStationMessage(char *msgPtr, int len, DeleteSTAValues *valuesPtr) //pei add 0703
{
//	int i;
	CWProtocolMessage completeMsg;
	
	if(msgPtr == NULL || valuesPtr == NULL) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	
	CWDebugLog("Parsing WTP Event Request Delete Station Request");
	
	completeMsg.msg = msgPtr;
	completeMsg.offset = 0;
	
	unsigned short int elemType = 0;
	unsigned short int elemLen = 0;
	
	CWParseFormatMsgElem(&completeMsg,&elemType,&elemLen);
	
	CWDebugLog("Parsing Message Element: %u, elemLen: %u", elemType, elemLen);
	
	CW_CREATE_OBJECT_SIZE_ERR(valuesPtr->mac_addr, valuesPtr->mac_length, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););

	if(!(CWParseDeleteSta(&completeMsg, elemLen, valuesPtr))){
				 CWDebugLog("parse delete sta failed");}

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
	CWDebugLog("WTP Event Request Delete Station Request Parsed");
	
	return CW_TRUE;
}



