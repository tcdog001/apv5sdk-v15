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


#include "WTPFrameReceive.h"


#ifdef DMALLOC
#include "../dmalloc-5.5.0/dmalloc.h"
#endif
extern CWBool GetRadioId(unsigned char *mac_addr, unsigned char *radio_id);
int getMacAddr(int sock, char* interface, unsigned char* macAddr)
{
	struct ifreq ethreq;
	int i;

	memset(&ethreq, 0, sizeof(ethreq));
	strncpy(ethreq.ifr_name, interface, IFNAMSIZ);
	if (ioctl(sock, SIOCGIFHWADDR, &ethreq)==-1) 
	{
		return 0;
	}

	for (i=0; i<MAC_ADDR_LEN; i++)	
	{
		macAddr[i]=(unsigned char)ethreq.ifr_hwaddr.sa_data[i];
	}
	CWDebugLog("\n");

	return 1;
}

int extractFrameInfo(char* buffer, char* RSSI, char* SNR, int* dataRate)
{
	int signal, noise;

	*RSSI=buffer[RSSI_BYTE]-ATHEROS_CONV_VALUE;	//RSSI in dBm
	
	signal=buffer[SIGNAL_BYTE]-ATHEROS_CONV_VALUE;	
	noise=buffer[NOISE_BYTE];			
	*SNR=(char)signal-noise;			//RSN in dB
	*dataRate=(buffer[DATARATE_BYTE]/2)*10;		//Data rate in Mbps*10
	return 1;
}

int extractFrame(CWProtocolMessage** frame, unsigned char* buffer, int len)	//len: frame length including prism header
{
	CW_CREATE_OBJECT_ERR(*frame, CWProtocolMessage, return 0;);
	CWProtocolMessage *auxPtr = *frame;
	CW_CREATE_PROTOCOL_MESSAGE(*auxPtr, len-PRISMH_LEN, return 0;);
	memcpy(auxPtr->msg, buffer+PRISMH_LEN, len-PRISMH_LEN);
	auxPtr->offset=len-PRISMH_LEN;
	return 1;
}

int extractAddr(unsigned char* destAddr, unsigned char* sourceAddr, char* frame)
{
	memset(destAddr, 0, MAC_ADDR_LEN);
	memset(sourceAddr, 0, MAC_ADDR_LEN);
	memcpy(destAddr, frame+DEST_ADDR_START, MAC_ADDR_LEN);
	memcpy(sourceAddr, frame+SOURCE_ADDR_START, MAC_ADDR_LEN);

	return 1;
}
int GetTunnelModeByBssid(unsigned char bssid[MAC_ADDR_LEN])
{
	int tunnelMode = -1;
	CWWTPWlan *ptr = NULL;
	CWBool bFound = CW_FALSE;
	for(ptr = wtp_wlan_list;ptr != NULL;ptr = ptr->next){
		if(memcmp(ptr->wlan_bssid, bssid, MAC_ADDR_LEN) == 0){
			tunnelMode = ptr->wlan_tunnel_mode;
			bFound = CW_TRUE;
			break;
		}
	}
	if(bFound == CW_FALSE){
		if(debug_print)
			printf("Can't Find Tunnel Mode by [%02x-%02x-%02x-%02x-%02x-%02x]\n", bssid[0],bssid[1],bssid[2],bssid[3],bssid[4],bssid[5]);
		CWWTPDebugLog("Can't Find Tunnel Mode by  [%02x-%02x-%02x-%02x-%02x-%02x]", bssid[0],bssid[1],bssid[2],bssid[3],bssid[4],bssid[5]);
	}
#if 0
	else{
		if(debug_print)
			printf("[%02x-%02x-%02x-%02x-%02x-%02x] is %s Tunnel.\n",bssid[0],bssid[1],bssid[2],bssid[3],bssid[4],bssid[5],tunnelMode==1?"802.3":"802.11");
		CWWTPDebugLog("[%02x-%02x-%02x-%02x-%02x-%02x] is %s Tunnel.",bssid[0],bssid[1],bssid[2],bssid[3],bssid[4],bssid[5],tunnelMode==1?"802.3":"802.11");
	}
#endif
	return tunnelMode;
}

CW_THREAD_RETURN_TYPE CWWTPReceive802_3Frame(void *arg)
{
	if(pthread_detach(pthread_self())!=0)
	printf("##########detach error!############\n");	
	int sock, n;
	unsigned char buffer[2048];
	unsigned char bssid[MAC_ADDR_LEN];
	struct sockaddr_ll addr;
	CWBindingTransportHeaderValues *bindingValuesPtr=NULL;
	CWProtocolMessage* frame = NULL;
	CWBindingDataListElement* listElement=NULL;
#ifdef WRITE_UNRECOGNIZED
	int i;
#endif
	int tosend=0;
	unsigned char wlan_id;
	char ifname[IFNAMSIZ];
	
	CWWTPWlan *wlancreating = (CWWTPWlan *)arg;

	wlan_id = wlancreating->wlan_id;
	CWDebugLog("frame receive wlan_id %d",wlan_id);
	CWWTPGetWlanName(ifname,wlancreating->radio_id,wlan_id);
	gInterfaceName = ifname;
	CWDebugLog("gInterfaceName %s",gInterfaceName);
	
#ifdef PROMODE_ON 
	struct ifreq ifr;

    /**** zhouke add ,for marvell ioctl ****/
#ifdef MARVELL_PLATFORM
    memset(&ifr, 0, sizeof(struct ifreq));
#endif
    /************ end ************/

#endif
//	CWThreadSetSignals(SIG_BLOCK, 1, SIGALRM);
	
	if ((sock = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ALL)))<0) {
		CWDebugLog("THR FRAME: Error creating socket");
		CWExitThread();
	}
	memset(&addr,'\0',sizeof(addr));
	addr.sll_family=AF_PACKET;
	addr.sll_protocol = htons(0x0030);
	addr.sll_ifindex=0;

	if ((bind(sock, (struct sockaddr*)&addr, sizeof(addr)))<0) {
		CWDebugLog("THR FRAME: Error binding socket");
		close(sock);
		CWExitThread();
	}
	CWDebugLog("pf_sock bind");
#ifdef PROMODE_ON
	/* Set the network card in promiscuos mode */
	strncpy(ifr.ifr_name,gInterfaceName,IFNAMSIZ);
	if (ioctl(sock,SIOCGIFFLAGS,&ifr)==-1) {
		CWDebugLog("THR FRAME: Error ioctl");
		EXIT_THREAD
	}
	ifr.ifr_flags|=IFF_PROMISC;
	if (ioctl(sock,SIOCSIFFLAGS,&ifr)==-1) {
		CWDebugLog("THR FRAME: Error ioctl");
		EXIT_THREAD
	}
	CWDebugLog("%s promiscuos mode set",gInterfaceName);
#endif
#ifdef FILTER_ON
	/* Attach the filter to the socket */
	if(setsockopt(sock, SOL_SOCKET, SO_ATTACH_FILTER, &Filter, sizeof(Filter))<0){
		CWDebugLog("THR FRAME: Error attaching filter");
		EXIT_THREAD
	}
#endif
	CW_REPEAT_FOREVER 
	{
		if(WTPWLanReceiveThreadRun == CW_FALSE){
			break;
		}
		memset(buffer,0,sizeof(buffer));
		n = recvfrom(sock,buffer,sizeof(buffer),0,NULL,NULL);
#if 1
		printf("receive a 802.3 from Drv.\n");
		CWCaptrue(n,buffer);
#endif
		int k;
		int fragmentsNum = 0;
		CWProtocolMessage *completeMsgPtr = NULL;
		
		/* pei add for 802.3 tunnel  */
		CWWTPWlan *ptr = NULL;
		memset(bssid, 0, MAC_ADDR_LEN);
		memcpy(bssid, buffer, MAC_ADDR_LEN);
		dpf("receive a 802.3 frame from bssid:["MAC_FMT"]\n",MAC2STR(bssid));

		for(ptr = wtp_wlan_list;ptr != NULL;ptr = ptr->next){
			if(memcmp(ptr->wlan_bssid, bssid, MAC_ADDR_LEN) == 0){
				dpf("the 802.3 frame is from interface: ath.%d-%d\n", ptr->radio_id, ptr->wlan_id);
				tosend = 1;
				break;
			}
		}
		if(tosend==1){
			dpf("receive a 802.3\n");
			CW_CREATE_OBJECT_ERR(bindingValuesPtr, CWBindingTransportHeaderValues, EXIT_THREAD);
			memset(bindingValuesPtr,0,sizeof(CWBindingTransportHeaderValues));
			bindingValuesPtr->flag_w = -1;
			bindingValuesPtr->flag_m = -1;
			CW_CREATE_OBJECT_ERR(listElement, CWBindingDataListElement, EXIT_THREAD);
			CW_CREATE_OBJECT_ERR(frame, CWProtocolMessage, EXIT_THREAD);

			/*prepare RID of capwap header by diaowq@20111023*/
			memcpy(bindingValuesPtr->bssid, bssid, MAC_ADDR_LEN);
			GetRadioId(bssid,&(bindingValuesPtr->radioId));
			bindingValuesPtr->type = 0;  /* 802.3 */
			bindingValuesPtr->flag_m = 1;
			frame->msg = (char *)(buffer + MAC_ADDR_LEN); 
			frame->msgLen = n - MAC_ADDR_LEN;
			frame->offset = n- MAC_ADDR_LEN;
			listElement->frame = frame;
			listElement->bindingValues = bindingValuesPtr;
			
			if (!CWAssembleDataMessage(&completeMsgPtr, 
							   			&fragmentsNum, 
							   			gWTPPathMTU, 
							   			listElement->frame, 
							   			listElement->bindingValues,
#ifdef CW_NO_DTLS
										CW_PACKET_PLAIN
#else
								       	(gDtlsSecurity == 1)?CW_PACKET_CRYPT:CW_PACKET_PLAIN           /* 0-CW_PACKET_PLAIN, 1-CW_PACKET_CRYPT */
#endif
							  ))
			{	
				for(k = 0; k < fragmentsNum; k++){
					CW_FREE_PROTOCOL_MESSAGE(completeMsgPtr[k]);
				}
						
				CW_FREE_OBJECT(completeMsgPtr);
				CW_FREE_OBJECT(listElement->frame);
				CW_FREE_OBJECT(listElement->bindingValues);
				CW_FREE_OBJECT(listElement);
				break;
			}
											
			for (k = 0; k < fragmentsNum; k++) 
			{
	//		if (!CWNetworkSendUnsafeConnected(gWTPDataSocket, completeMsgPtr[k].msg, completeMsgPtr[k].offset)) {/*change gWTPSocket to gWTPDataSocket*/
				CWNetworkLev4Address  DataChannelAddr;
	//#ifdef IPv6
				if(gNetworkPreferredFamily == CW_IPv6)
				{
					struct sockaddr_in6 temptaddr;
					CW_COPY_NET_ADDR_PTR(&temptaddr , &(gACInfoPtr->preferredAddress));
					temptaddr.sin6_port = htons(CW_DATA_PORT);  /* pei test 1222 */
					CW_COPY_NET_ADDR_PTR(&DataChannelAddr , &temptaddr);
				}
	//#else
				else
				{
					struct sockaddr_in temptaddr;
					CW_COPY_NET_ADDR_PTR(&temptaddr , &(gACInfoPtr->preferredAddress));
					temptaddr.sin_port = htons(CW_DATA_PORT);  /* pei test 1222 */
					CW_COPY_NET_ADDR_PTR(&DataChannelAddr , &temptaddr);
				}
	//#endif

#ifndef CW_NO_DTLS
				if((gDtlsSecurity == 1)&&(gDtlsPolicy == 1))
				{
					if(!CWSecuritySend(gWTPDataSession, completeMsgPtr[k].msg, completeMsgPtr[k].offset))
					{
						CWDebugLog("Failure sending Request");
						break;
					}
				}
				else
#endif
				{
					if (!CWNetworkSendUnsafeUnconnected(gWTPDataSocket, &DataChannelAddr, completeMsgPtr[k].msg, completeMsgPtr[k].offset))
					{
						CWDebugLog("Failure sending Request");
						break;
					}
				}
			}

			for (k = 0; k < fragmentsNum; k++)
			{
				CW_FREE_PROTOCOL_MESSAGE(completeMsgPtr[k]);
			}		
			CW_FREE_OBJECT(completeMsgPtr); 			
			CW_FREE_OBJECT(listElement->frame);
			CW_FREE_OBJECT(listElement->bindingValues);
			CW_FREE_OBJECT(listElement);
		}
	}
	CWWTPDebugLog("802.3 frame receive close sock return NULL");
	close(sock);
	return(NULL);
}



CW_THREAD_RETURN_TYPE CWWTPReceive802_11Frame(void *arg)
{
	if(pthread_detach(pthread_self())!=0)
	printf("##########detach error!############\n");	
	const unsigned char VERSION_MASK=3, TYPE_MASK=12, SUBTYPE_MASK=240;
	const unsigned char MANAGEMENT_TYPE=0, DATA_TYPE=8;//, CONTROL_TYPE=4;
	int sock, n;
	unsigned char buffer[2048];
	unsigned char bssid[MAC_ADDR_LEN];
//	unsigned char macAddr[MAC_ADDR_LEN];
//	unsigned char destAddr[MAC_ADDR_LEN];
//	unsigned char sourceAddr[MAC_ADDR_LEN];
	unsigned int prismh_len = 0;
	unsigned char byte0, version=0, type=0, subtype=0;
	struct sockaddr_ll addr;
	CWBindingTransportHeaderValues *bindingValuesPtr=NULL;
	CWProtocolMessage* frame = NULL;
	CWBindingDataListElement* listElement=NULL;
	unsigned char *srcmac = NULL;
#ifdef WRITE_UNRECOGNIZED
	int i;
#endif
	int tosend;
	unsigned char wlan_id;
	char ifname[IFNAMSIZ];
	
	CWWTPWlan *wlancreating = (CWWTPWlan *)arg;

	wlan_id = wlancreating->wlan_id;
	CWDebugLog("frame receive wlan_id %d",wlan_id);
	CWWTPGetWlanName(ifname,wlancreating->radio_id,wlan_id);
	gInterfaceName = ifname;
	CWDebugLog("gInterfaceName %s",gInterfaceName);
	
#ifdef PROMODE_ON 
	struct ifreq ifr;

    /**** zhouke add ,for marvell ioctl ****/
#ifdef MARVELL_PLATFORM
    memset(&ifr, 0, sizeof(struct ifreq));
#endif
    /************ end ************/

#endif
//	CWThreadSetSignals(SIG_BLOCK, 1, SIGALRM);
	
	if ((sock = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ALL)))<0) {
		CWDebugLog("THR FRAME: Error creating socket");
		CWExitThread();
	}
#if 0	
	strncpy(ifr.ifr_name,gInterfaceName,IFNAMSIZ);
	
	if (ioctl(sock, SIOCGIFINDEX, &ifr) == -1){	
			printf("ioctl ifindex error\n");
	}
#endif
	memset(&addr,'\0',sizeof(addr));
	addr.sll_family=AF_PACKET;
//	addr.sll_protocol=htons(ETH_P_ALL);
	addr.sll_protocol = htons(0x0019);
	addr.sll_ifindex=0;

	if ((bind(sock, (struct sockaddr*)&addr, sizeof(addr)))<0) {
		CWDebugLog("THR FRAME: Error binding socket");
		close(sock);
		CWExitThread();
	}
	CWDebugLog("pf_sock bind");
#ifdef PROMODE_ON
	/* Set the network card in promiscuos mode */
	strncpy(ifr.ifr_name,gInterfaceName,IFNAMSIZ);
	if (ioctl(sock,SIOCGIFFLAGS,&ifr)==-1) {
		CWDebugLog("THR FRAME: Error ioctl");
		EXIT_THREAD
	}
	ifr.ifr_flags|=IFF_PROMISC;
	if (ioctl(sock,SIOCSIFFLAGS,&ifr)==-1) {
		CWDebugLog("THR FRAME: Error ioctl");
		EXIT_THREAD
	}
	CWDebugLog("%s promiscuos mode set",gInterfaceName);
#endif
#ifdef FILTER_ON
	/* Attach the filter to the socket */
	if(setsockopt(sock, SOL_SOCKET, SO_ATTACH_FILTER, &Filter, sizeof(Filter))<0){
		CWDebugLog("THR FRAME: Error attaching filter");
		EXIT_THREAD
	}
#endif
	
	CW_REPEAT_FOREVER 
	{
		if(WTPWLanReceiveThreadRun == CW_FALSE){
			break;
		}
		tosend = 0;
		memset(buffer,0,sizeof(buffer));
		n = recvfrom(sock,buffer,sizeof(buffer),0,NULL,NULL);
#if 1
		CWWTPDebugLog("\nreceive a 802.11 from wireless drv.");
		CWCaptrue(n,buffer);
#endif

		if (CHECK_PRISM_HEADER || CHECK_CRYPTO_PRISM_HEADER){
			prismh_len = PRISMH_LEN;
		}
		else {
			prismh_len = 0;
		}
		if (n>prismh_len){
			byte0=*(buffer+prismh_len);
			version = byte0 & VERSION_MASK;
			type = byte0 & TYPE_MASK;
			subtype = byte0 & SUBTYPE_MASK;
			subtype>>=4;
			int k;
	 		int fragmentsNum = 0;
			CWProtocolMessage *completeMsgPtr = NULL;

				DeleteSTAValues *deleeStaValues = NULL;
				IEEE80211_Header *wh = NULL;
			wh = (IEEE80211_Header *)(buffer + prismh_len);
			srcmac = wh->mac2;
			
/*			if(version == (unsigned char) VERSION) {CWDebugLog("Version OK\n");}
			else {CWDebugLog("Wrong Version");}
*/
			if(type == (unsigned char) MANAGEMENT_TYPE){/*Management Frame*/
			switch (subtype){
				case ASSOCIATION_REQUEST_SUBTYPE:{
					CWDebugLog("Association Request received\n");
					CWWTPDebugLog("Association Request received:%x-%x-%x-%x-%x-%x",srcmac[0],srcmac[1],srcmac[2],srcmac[3],srcmac[4],srcmac[5]);
					if(debug_print)
						printf("Association Request received:%x-%x-%x-%x-%x-%x\n",srcmac[0],srcmac[1],srcmac[2],srcmac[3],srcmac[4],srcmac[5]);
					tosend = 1;
					break;
				}
				case AUTHENTICATION_SUBTYPE: {
					CWWTPDebugLog("Authentication received:%x-%x-%x-%x-%x-%x",srcmac[0],srcmac[1],srcmac[2],srcmac[3],srcmac[4],srcmac[5]);
					if(debug_print)
						printf("Authentication received:%x-%x-%x-%x-%x-%x\n",srcmac[0],srcmac[1],srcmac[2],srcmac[3],srcmac[4],srcmac[5]);
					tosend = 1;
					break;
				}
				case PROBE_REQUEST_SUBTYPE :{
//					CWLog("\nProbe Request received\n");
//					CWDebugLog("\nProbe Request received\n");
					//printf("\nProbe Request received\n");
					tosend = 1;
					break;
				}
		 		case ASSOCIATION_RESPONSE_SUBTYPE: {
//					CWDebugLog("Association Response\n");
					//printf("Association Response\n");
					tosend = 1;
					break;
				}
		 		case REASSOCIATION_REQUEST_SUBTYPE: {
//					CWDebugLog("Reassociation Request\n");
					//printf("Reassociation Request\n");
					tosend = 1;
					break;
				}
		 		case REASSOCIATION_RESPONSE_SUBTYPE: {
//					CWDebugLog("Reassociation Response\n"); 
					//printf("Reassociation Response\n"); 
					tosend = 1;
					break;
				}
			    case PROBE_RESPONSE_SUBTYPE: {
//					CWDebugLog("Probe Response\n");
					//printf("Probe Response\n");
					tosend = 1; 
					break;
				}
				case RESERVED6_SUBTYPE: {
//					CWDebugLog("Reserved\n");
					tosend = 0; 
					break;
				}
		 		case RESERVED7_SUBTYPE: {
//					CWDebugLog("Reserved\n");
					tosend = 0;
					break;
				}
		 		case BEACON_SUBTYPE: {//CWDebugLog("Beacon\n");tosend = 1;
//					CWDebugLog("Beacon\n");
		 			break;
				}
		 		case ATIM_SUBTYPE: {
//					CWDebugLog("ATIM\n");
					tosend = 1; 
					break;
				}
		 		case DISASSOCIATION_SUBTYPE: {
//					CWDebugLog("Disassociation\n");
					CWWTPDebugLog("Disassociation:%x-%x-%x-%x-%x-%x",srcmac[0],srcmac[1],srcmac[2],srcmac[3],srcmac[4],srcmac[5]);
					tosend = 1;           
					CW_CREATE_OBJECT_ERR(deleeStaValues,DeleteSTAValues,CWWTPDebugLog("CW_ERROR_OUT_OF_MEMORY\n"););
					deleeStaValues->mac_addr = wh->mac2;
					STATableDelete(deleeStaValues);
					deleeStaValues->mac_addr = NULL;
					CW_FREE_OBJECT(deleeStaValues);
					break;
				}
		 		case DEAUTHENTICATION_SUBTYPE: {
//					CWDebugLog("Deauthentication\n");
					CWWTPDebugLog("Deauthentication:%x-%x-%x-%x-%x-%x",srcmac[0],srcmac[1],srcmac[2],srcmac[3],srcmac[4],srcmac[5]);
					tosend = 1;          
					CW_CREATE_OBJECT_ERR(deleeStaValues,DeleteSTAValues,CWWTPDebugLog("CW_ERROR_OUT_OF_MEMORY\n"););
					deleeStaValues->mac_addr = wh->mac2;
					STATableDelete(deleeStaValues);
					deleeStaValues->mac_addr = NULL;
					CW_FREE_OBJECT(deleeStaValues);
					break;
		 		}
			   default: {CWDebugLog("Unrecognized management Frame\n");}
		  }
		  }
		  else if (type == (unsigned char)DATA_TYPE){
//			CWDebugLog("\n!!!!!!!!!!!!!!!!a EAP in wtpd!!!!!!!!!!!!!!!!!!!!!!\n");
			/*CWCaptrue(n,buffer);*/
			/*now it's only EAP*/
			CWWTPDebugLog("recv 802.11 data frame from drv.");
			//printf("\n!!!!!!!!!!!!!!!!a EAP in wtpd--from STA!!!!!!!!!!!!!!!!!!!!!!\n");
			tosend = 1;
		 }
		 else{
			CWDebugLog("OTHER frame");
			CWWTPDebugLog("recv error frame from drv.");
		 }
			
			if(tosend == 1)
			{
				CW_CREATE_OBJECT_ERR(bindingValuesPtr, CWBindingTransportHeaderValues, EXIT_THREAD);
				if (PRISMH_LEN == prismh_len) {
					extractFrameInfo((char*)buffer, &(bindingValuesPtr->RSSI),  &(bindingValuesPtr->SNR), (int *)&(bindingValuesPtr->dataRate));/*from prism2*/
				}else{
					bindingValuesPtr->RSSI = 0xa1;
					bindingValuesPtr->SNR = 0xa1;
					bindingValuesPtr->dataRate = 0;
				}
				
				memset(bssid, 0, MAC_ADDR_LEN);
				memcpy(bssid, wh->mac1, MAC_ADDR_LEN);
				GetRadioId(bssid,&(bindingValuesPtr->radioId));
				
				CW_CREATE_OBJECT_ERR(listElement, CWBindingDataListElement, EXIT_THREAD);
				CW_CREATE_OBJECT_ERR(frame, CWProtocolMessage, EXIT_THREAD);
				frame->msg = (char *)(buffer + prismh_len); //pei 0917 del warning
				frame->msgLen = n - prismh_len;
				frame->offset = n- prismh_len;
				listElement->frame = frame;
				bindingValuesPtr->type = 1;  
				bindingValuesPtr->flag_w = 1;
				listElement->bindingValues = bindingValuesPtr;
			
			if (!CWAssembleDataMessage(&completeMsgPtr, 
							   			&fragmentsNum, 
							   			gWTPPathMTU, 
							   			listElement->frame, 
							   			listElement->bindingValues,
#ifdef CW_NO_DTLS
										CW_PACKET_PLAIN
#else
								       	(gDtlsSecurity == 1)?CW_PACKET_CRYPT:CW_PACKET_PLAIN           /* 0-CW_PACKET_PLAIN, 1-CW_PACKET_CRYPT */
#endif
							  ))
			{	
				for(k = 0; k < fragmentsNum; k++){
					CW_FREE_PROTOCOL_MESSAGE(completeMsgPtr[k]);
				}
						
				CW_FREE_OBJECT(completeMsgPtr);
				//diaowq del it @20110726, it's wrong to free a 'stack-buf'
				//CW_FREE_PROTOCOL_MESSAGE(*(listElement->frame));
				CW_FREE_OBJECT(listElement->frame);
				CW_FREE_OBJECT(listElement->bindingValues);
				CW_FREE_OBJECT(listElement);
				break;
			}
											
			for (k = 0; k < fragmentsNum; k++) 
			{
	//							if (!CWNetworkSendUnsafeConnected(gWTPDataSocket, completeMsgPtr[k].msg, completeMsgPtr[k].offset)) {/*change gWTPSocket to gWTPDataSocket*/
				CWNetworkLev4Address  DataChannelAddr;
	//#ifdef IPv6
				if(gNetworkPreferredFamily == CW_IPv6)
				{
					struct sockaddr_in6 temptaddr;
					CW_COPY_NET_ADDR_PTR(&temptaddr , &(gACInfoPtr->preferredAddress));
					temptaddr.sin6_port = htons(CW_DATA_PORT);  /* pei test 1222 */
					CW_COPY_NET_ADDR_PTR(&DataChannelAddr , &temptaddr);
				}
	//#else
				else
				{
					struct sockaddr_in temptaddr;
					CW_COPY_NET_ADDR_PTR(&temptaddr , &(gACInfoPtr->preferredAddress));
					temptaddr.sin_port = htons(CW_DATA_PORT);  /* pei test 1222 */
					CW_COPY_NET_ADDR_PTR(&DataChannelAddr , &temptaddr);
				}
	//#endif

#ifndef CW_NO_DTLS
				if((gDtlsSecurity == 1)&&(gDtlsPolicy == 1))
				{
					if(!CWSecuritySend(gWTPDataSession, completeMsgPtr[k].msg, completeMsgPtr[k].offset))
					{
						CWDebugLog("Failure sending Request");
						break;
					}
				}
				else
#endif
				{
					if (!CWNetworkSendUnsafeUnconnected(gWTPDataSocket, &DataChannelAddr, completeMsgPtr[k].msg, completeMsgPtr[k].offset))
					{
						CWDebugLog("Failure sending Request");
						break;
					}
				}
			}

			for (k = 0; k < fragmentsNum; k++)
			{
				CW_FREE_PROTOCOL_MESSAGE(completeMsgPtr[k]);
			}		
			CW_FREE_OBJECT(completeMsgPtr); 			
			CW_FREE_OBJECT(listElement->frame);
			CW_FREE_OBJECT(listElement->bindingValues);
			CW_FREE_OBJECT(listElement);
		}
	 	}
		else {
#ifdef WRITE_UNRECOGNIZED
			for (i=0;i<n;i++){
				if (i%16==0) CWDebugLog("\n%04x:   ", i);
				CWDebugLog("%02x  ", buffer[i]);
			}
			CWDebugLog("\n");	
#endif
		}
	}

	CWDebugLog("frame receive close sock return NULL");
	close(sock);
	return(NULL);
}
