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

#include "CWWTP.h"

#ifdef DMALLOC
#include "../dmalloc-5.5.0/dmalloc.h"
#endif

CWBool CWAssembleChangeStateEventRequest(CWProtocolMessage **messagesPtr, int *fragmentsNumPtr, int PMTU, int seqNum, CWList msgElemList);
CWBool CWParseChangeStateEventResponseMessage (char *msg, int len, int seqNum, void *values);
CWBool CWSaveChangeStateEventResponseMessage (void *changeStateEventResp);
#ifndef CW_NO_DTLS
CW_THREAD_RETURN_TYPE CWWTPReceiveRealDtlsDataPacket(void *arg);
#endif
CW_THREAD_RETURN_TYPE CWWTPReceiveDtlsDataPacket(void *arg);
#if 0
// init network for client
CWBool CWNetworkInitSocketClientUnconnect(CWSocket *sockPtr, CWNetworkLev4Address *addrPtr) {  /* pei test 1222 */
	int yes = 1;
	struct sockaddr_in  sockAddr;
	int  iLen;
	char *ip;
	int port;
	// NULL addrPtr means that we don't want to connect to a specific address
	if(sockPtr == NULL) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	
#ifdef IPv6
    if(((*sockPtr)=socket((gNetworkPreferredFamily == CW_IPv4) ? AF_INET : AF_INET6, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
#else
	if(((*sockPtr)=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
#endif
		CWNetworkRaiseSystemError(CW_ERROR_CREATING);
	}

printf("--------------gWTPDataSocket:%d\n", *sockPtr);
iLen=sizeof(sockAddr); 
getpeername(*sockPtr, (struct   sockaddr  *)&sockAddr,&iLen);
getsockname(*sockPtr, (struct  sockaddr  *)&sockAddr, &iLen); 
ip=inet_ntoa(sockAddr.sin_addr); 
port= sockAddr.sin_port; 

printf("%s:%d\n",ip,port);

//	if(addrPtr != NULL) {
//		CWUseSockNtop(((struct sockaddr*)addrPtr), CWDebugLog(str););

//		if(connect((*sockPtr), ((struct sockaddr*)addrPtr), CWNetworkGetAddressSize(addrPtr)) < 0) {
//			CWNetworkRaiseSystemError(CW_ERROR_CREATING);
//		}
//	}

	// allow sending broadcast packets
	setsockopt(*sockPtr, SOL_SOCKET, SO_BROADCAST, &yes, sizeof(yes));
	
	return CW_TRUE;
}
#endif
CWBool CWNetworkInitSocketClientConnect(CWSocket *sockPtr, CWNetworkLev4Address *addrPtr, int port)   /* pei test 1222 */
{
	int yes = 1;
	int on = 1;

	CWWTPDebugLog("***WTP Sock port: %d", port);

#ifdef IPv6
	extern const struct in6_addr in6addr_any;
	struct sockaddr_in6 addr;	
	if(((*sockPtr)=socket((gNetworkPreferredFamily == CW_IPv4) ? AF_INET : AF_INET6, SOCK_DGRAM, IPPROTO_UDP)) < 0)
#else
	struct sockaddr_in addr;
	if(((*sockPtr)=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
#endif
	{
		CWDebugLog("create sock error!errno:%d,%s.",errno,strerror(errno));
		CWWTPDebugLog("create sock error!errno:%d,%s.",errno,strerror(errno));
		return CW_FALSE;
	}
	
#ifdef IPv6
	memset(&addr, 0, sizeof(struct sockaddr_in6));
	addr.sin6_family=AF_INET6;
	addr.sin6_port=htons(port);
	addr.sin6_addr = in6addr_any; /* structure assignment */
#else
	memset(&addr, 0, sizeof(struct sockaddr_in));
	addr.sin_family=AF_INET;
	addr.sin_port=htons(port);
	addr.sin_addr.s_addr = INADDR_ANY;//htonl(INADDR_ANY); //(inet_addr(0.0.0.0));
#endif

	/* pei add for sock resuse 1227 */
	setsockopt(*sockPtr,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on));

	if(bind(*sockPtr, (struct sockaddr *)&addr, sizeof(addr))==-1)
	{
		CWWTPDebugLog("bind error!%s", strerror(errno));
		return CW_FALSE;
	}
	if(addrPtr != NULL) {
		CWUseSockNtop(((struct sockaddr*)addrPtr), CWDebugLog(str););
        int res = connect((*sockPtr), ((struct sockaddr*)addrPtr), CWNetworkGetAddressSize(addrPtr));
		if(res < 0) {
			CWWTPDebugLog("gWTPSock connect error!%s.",strerror(errno));
			CWNetworkRaiseSystemError(CW_ERROR_CREATING);
		}
	}

	// allow sending broadcast packets
	setsockopt(*sockPtr, SOL_SOCKET, SO_BROADCAST, &yes, sizeof(yes));
	return CW_TRUE;
}
CWBool CWNetworkInitSocketClientUnconnect(CWSocket *sockPtr, struct sockaddr_in *addrPtr, int port, char *hostip)   /* pei test 1222 */
{
	int yes = 1;
	int on = 1;
	
	dpf("***WTP Sock port: %d\n", port);
#ifdef IPv6
	extern const struct in6_addr in6addr_any;
	struct sockaddr_in6 addr;
	if(((*sockPtr)=socket((gNetworkPreferredFamily == CW_IPv4) ? AF_INET : AF_INET6, SOCK_DGRAM, IPPROTO_UDP)) < 0)
#else
	struct sockaddr_in addr;
	if(((*sockPtr)=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
#endif
	{
		CWDebugLog("create sock error!errno:%d,%s.",errno,strerror(errno));
		CWWTPDebugLog("create sock error!errno:%d,%s.",errno,strerror(errno));
		return CW_FALSE;
	}

	memset(&addr, 0, sizeof(struct sockaddr_in));
#ifdef IPv6
	addr.sin6_family=AF_INET6;
	addr.sin6_port=htons(port);
	addr.sin6_addr = in6addr_any; /* structure assignment */
#else
	addr.sin_family=AF_INET;
	addr.sin_port=htons(port);
	addr.sin_addr.s_addr = inet_addr(hostip);//INADDR_ANY;//htonl(INADDR_ANY); //(inet_addr(0.0.0.0));
#endif

	/* pei add for sock resuse 1227 */
	setsockopt(*sockPtr,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on));

	if(bind(*sockPtr, (struct sockaddr *)&addr, sizeof(addr))==-1)
	{
		CWDebugLog("bind error!%s", strerror(errno));
		CWWTPDebugLog("bind error!%s", strerror(errno));
		return CW_FALSE;
	}
	// allow sending broadcast packets
	setsockopt(*sockPtr, SOL_SOCKET, SO_BROADCAST, &yes, sizeof(yes));
	return CW_TRUE;
}
CWStateTransition CWWTPEnterDataCheck() {
	int seqNum;
//	CWNetworkLev4Address  DataChannelAddr;
//	struct sockaddr_in temptaddr;
	
	CWDebugLog("\n");
	CWDebugLog("######### Data Check State #########");
	
	CWDebugLog("\n");
	CWDebugLog("#________ Change State Event (Data Check) ________#");

	// reset datacheck state
	//CWNetworkCloseSocket(gWTPDataSocket);
	/*
	CWSecurityDestroySession(gWTPSession);
	CWSecurityDestroyContext(gWTPSecurityContext);
	gWTPSecurityContext = NULL;
	gWTPSession = NULL;
	*/
#ifndef CW_NO_DTLS  /* pei data dtls 20100510 */
	if((gDtlsSecurity == 1)&&(gDtlsPolicy == 1))
	{
		CWSecurityDestroySession(gWTPDataSession);
		CWSecurityDestroyContext(gWTPDataSecurityContext);
		gWTPDataSecurityContext = NULL;
		gWTPDataSession = NULL;
	}
#endif

/**************************************************************************
*
*				 create the data channel
*
*
***************************************************************************/
	CWNetworkLev4Address  DataChannelAddr;
#ifdef IPv6
	struct sockaddr_in6 temptaddr;
	CW_COPY_NET_ADDR_PTR(&temptaddr , &(gACInfoPtr->preferredAddress));
	temptaddr.sin6_port = htons(CW_DATA_PORT);  /* pei test 1222 */
#else
	struct sockaddr_in temptaddr;
	CW_COPY_NET_ADDR_PTR(&temptaddr , &(gACInfoPtr->preferredAddress));
	temptaddr.sin_port = htons(CW_DATA_PORT);  /* pei test 1222 */
#endif
	CW_COPY_NET_ADDR_PTR(&DataChannelAddr , &temptaddr);
//	if(!CWErr(CWNetworkInitSocketClient(&gWTPDataSocket,&DataChannelAddr))) {

	int WTPDataSockPort = 32769;
//	if(!CWErr(CWNetworkInitSocketClientUnconnect(&gWTPDataSocket, &temptaddr, WTPDataSockPort, gHostip))) {
#ifndef CW_NO_DTLS
	if((gDtlsSecurity == 1)&&(gDtlsPolicy == 1))
	{
		if(!CWErr(CWNetworkInitSocketClientConnect(&gWTPDataSocket, &DataChannelAddr, WTPDataSockPort))) {
			CWWTPDebugLog("data sock init error. SM(dtls) will jump from DataCheck State to Discovery State.");
			CWNetworkCloseSocket(gWTPSocket);
			return CW_ENTER_DISCOVERY;
		}
	}
	else
#endif
	{
		if(!CWErr(CWNetworkInitSocketClientUnconnect(&gWTPDataSocket, NULL, WTPDataSockPort, gHostip))) {
			CWWTPDebugLog("data sock init error. SM will jump from DataCheck State to Discovery State.");
			CWNetworkCloseSocket(gWTPSocket);
			return CW_ENTER_DISCOVERY;
		}
	}
//pei add for splictmac fast-forward 081115
	char str[128];
	char cmdbuf[100];
	
	memset(str, 0, 128);
	struct sockaddr_in	*sin = (struct sockaddr_in *) ((struct sockaddr*)(&(gACInfoPtr->preferredAddress)));
	inet_ntop(AF_INET, &sin->sin_addr, str, sizeof(str));
	CWWTPDebugLog("AC addr:%#x", inet_addr(str));

	memset(cmdbuf, 0, 100);
	CWSetUdpPort();
	CWSetAC_IP(str);

#ifndef CW_NO_DTLS /* pei data dtls 20100510 */
//	if(gACInfoPtr->security == CW_X509_CERTIFICATE) {
	if((gDtlsSecurity == 1)&&(gDtlsPolicy == 1))
	{
		if(!CWErr(CWSecurityInitContext(&gWTPDataSecurityContext, "/etc/wlan/root.pem", "/etc/wlan/client.pem", "prova", CW_TRUE, NULL))) {
			CWNetworkCloseSocket(gWTPSocket);
			CWSecurityDestroyContext(gWTPSecurityContext);
			gWTPSecurityContext = NULL;
			gWTPSession = NULL;
			
			CWNetworkCloseSocket(gWTPDataSocket);
			gWTPDataSecurityContext = NULL;
			return CW_ENTER_DISCOVERY;
		}
	}
//	else { // pre-shared keys
//		if(!CWErr(CWSecurityInitContext(&gWTPDataSecurityContext, NULL, NULL, NULL, CW_TRUE, NULL))) {
//			CWNetworkCloseSocket(gWTPSocket);
//			CWSecurityDestroyContext(gWTPSecurityContext);
//			gWTPSecurityContext = NULL;
//			gWTPSession = NULL;
//			
//			CWNetworkCloseSocket(gWTPDataSocket);
//			gWTPDataSecurityContext = NULL;
//			return CW_ENTER_DISCOVERY;
//		}
//	}
#endif
	WTPDataChannelThreadRun = CW_TRUE ;
	CWThread thread_receiveData;
#ifndef CW_NO_DTLS  /* pei data dtls 20100510 */
	if((gDtlsSecurity == 1)&&(gDtlsPolicy == 1))
	{
		if(!CWErr(CWCreateThread(&thread_receiveData, CWWTPReceiveRealDtlsDataPacket, (void*)gWTPDataSocket,0)))
		{
			CWDebugLog("Error starting Thread that receive DATA DTLS packet");
			CWNetworkCloseSocket(gWTPSocket);
			CWNetworkCloseSocket(gWTPDataSocket);
	/*		CWTimerDestroy(&waitJoinTimer);
			
			CWSecurityDestroyContext(gWTPSecurityContext);
			gWTPSecurityContext = NULL;
			gWTPSession = NULL;
	*/
			return CW_ENTER_DISCOVERY;
		}
	}
	else
#endif
	{
		if(!CWErr(CWCreateThread(&thread_receiveData, CWWTPReceiveDtlsDataPacket, (void*)gWTPDataSocket,0)))
		{
			CWDebugLog("Error starting Thread that receive DATA DTLS packet");
			CWNetworkCloseSocket(gWTPSocket);
			CWNetworkCloseSocket(gWTPDataSocket);
	/*		CWTimerDestroy(&waitJoinTimer);
			
			CWSecurityDestroyContext(gWTPSecurityContext);
			gWTPSecurityContext = NULL;
			gWTPSession = NULL;
	*/
			return CW_ENTER_DISCOVERY;
		}
	}
#ifndef CW_NO_DTLS
	if((gDtlsSecurity == 1)&&(gDtlsPolicy == 1))
	{
		if(!CWErr(CWSecurityInitSessionClient(gWTPDataSocket, &DataChannelAddr, gPacketDataReceiveList, gWTPDataSecurityContext, &gWTPDataSession, &gWTPPathMTU))) { // error creating DTLS session
			
			CWNetworkCloseSocket(gWTPSocket);
			CWSecurityDestroyContext(gWTPSecurityContext);
			gWTPSecurityContext = NULL;
			gWTPSession = NULL;

			CWNetworkCloseSocket(gWTPDataSocket);
			CWSecurityDestroyContext(gWTPDataSecurityContext);
			gWTPDataSecurityContext = NULL;
			gWTPDataSession = NULL;
				
			return CW_ENTER_DISCOVERY;
		}
	}
#endif
	/* Send Change State Event Request */

	seqNum = CWGetSeqNum();
	if(!CWErr(CWStartHeartbeatTimer())) {
		CWDebugLog("CWStartHeartbeatTimer error\n");
//			return CW_ENTER_RESET;
	}
	if(!CWErr(CWWTPSendAcknowledgedPacket(seqNum, NULL, CWAssembleChangeStateEventRequest, CWParseChangeStateEventResponseMessage, CWSaveChangeStateEventResponseMessage, NULL))) 
	{
		CWDebugLog("CWWTPSendAcknowledgedPacket error\n");
#ifndef CW_NO_DTLS /* pei data dtls 20100510 */
//		CWSecurityDestroySession(gWTPDataSession);
//		CWSecurityDestroyContext(gWTPDataSecurityContext);
//		gWTPDataSecurityContext = NULL;
//		gWTPDataSession = NULL;
#endif
		CWWTPDebugLog("Enter CW_ENTER_DISCOVERY ... In DataCheck State Sending or Receiving Error.");
		return CW_ENTER_DISCOVERY;
	}

	if(!CWErr(CWStopHeartbeatTimer())) {
		CWDebugLog("CWStopHeartbeatTimer error\n");
//		return CW_ENTER_RESET;
	}

	return CW_ENTER_RUN;
}

CWBool CWAssembleChangeStateEventRequest(CWProtocolMessage **messagesPtr, int *fragmentsNumPtr, int PMTU, int seqNum, CWList msgElemList) 
{
	CWProtocolMessage *msgElems= NULL;
	const int msgElemCount = 2;
	CWProtocolMessage *msgElemsBinding= NULL;
	int msgElemBindingCount=0;

	int resultCode = CW_PROTOCOL_SUCCESS;
	int k = -1;
	
	if(messagesPtr == NULL || fragmentsNumPtr == NULL) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	
	CW_CREATE_PROTOCOL_MSG_ARRAY_ERR(msgElems, msgElemCount, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););	
		
	CWDebugLog("Assembling Change State Event Request...");	

	// Assemble Message Elements
	if (!(CWAssembleMsgElemRadioOperationalState(-1, &(msgElems[++k]))) ||
	    !(CWAssembleMsgElemResultCode(&(msgElems[++k]), resultCode))
	) 									//DA cambiare!!!!
	{	
		int i;
		for(i = 0; i <= k; i++) { CW_FREE_PROTOCOL_MESSAGE(msgElems[i]);}
		CW_FREE_OBJECT(msgElems);
		return CW_FALSE; // error will be handled by the caller
	}

	if (!(CWAssembleMessage(messagesPtr,
				fragmentsNumPtr,
				PMTU,
				seqNum,
				CW_MSG_TYPE_VALUE_CHANGE_STATE_EVENT_REQUEST,
				msgElems, msgElemCount,
				msgElemsBinding,
				msgElemBindingCount,
#ifdef CW_NO_DTLS
				CW_PACKET_PLAIN
#else
				(gDtlsSecurity == 1)?CW_PACKET_CRYPT:CW_PACKET_PLAIN           /* 0-CW_PACKET_PLAIN, 1-CW_PACKET_CRYPT */
#endif
				)))
	 	return CW_FALSE;
	
	CWDebugLog("Change State Event Request Assembled");

	 return CW_TRUE;
}

CWBool CWParseChangeStateEventResponseMessage (char *msg, int len, int seqNum, void *values) 
{
	CWControlHeaderValues controlVal;
	CWProtocolMessage completeMsg;
	
	if(msg == NULL) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	
	CWDebugLog("Parsing Change State Event Response...");
	
	completeMsg.msg = msg;
	completeMsg.offset = 0;
	
	if(!(CWParseControlHeader(&completeMsg, &controlVal))) return CW_FALSE; // error will be handled by the caller
	
	// different type
	if(controlVal.messageTypeValue != CW_MSG_TYPE_VALUE_CHANGE_STATE_EVENT_RESPONSE)
		return CWErrorRaise(CW_ERROR_INVALID_FORMAT, "Message is not Change State Event Response as Expected");
	
	if(controlVal.seqNum != seqNum) return CWErrorRaise(CW_ERROR_INVALID_FORMAT, "Different Sequence Number");
	
	controlVal.msgElemsLen -= CW_CONTROL_HEADER_OFFSET_FOR_MSG_ELEMS; // skip timestamp
	
	if(controlVal.msgElemsLen != 0 ) return CWErrorRaise(CW_ERROR_INVALID_FORMAT, "Change State Event Response must carry no message elements");

	CWDebugLog("Change State Event Response Parsed");

	return CW_TRUE;
}

CWBool CWSaveChangeStateEventResponseMessage (void *changeStateEventResp)
{
	CWDebugLog("Saving Change State Event Response...");
	CWDebugLog("Change State Event Response Saved");
	return CW_TRUE;
}
