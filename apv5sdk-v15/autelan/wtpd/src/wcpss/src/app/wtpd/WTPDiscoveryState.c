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

/*________________________________________________________________*/
/*  *******************___CAPWAP VARIABLES___*******************  */
int gCWMaxDiscoveries = 5; //10;

/*_________________________________________________________*/
/*  *******************___VARIABLES___*******************  */
int gCWDiscoveryCount;
CWBool receivedDisResp = CW_FALSE;
int gACListSeqNum = 0;
extern char gHostip[24];

#ifdef CW_DEBUGGING
	int gCWDiscoveryInterval = 3; //5;
	int gCWMaxDiscoveryInterval = 4; //20;
#else
	int gCWDiscoveryInterval = 3; //5;
	int gCWMaxDiscoveryInterval = 4; //20;
#endif

/*_____________________________________________________*/
/*  *******************___MACRO___*******************  */
#define CWWTPFoundAnAC()		(gACInfoPtr != NULL /*&& gACInfoPtr->preferredAddress.ss_family != AF_UNSPEC*/)

/*__________________________________________________________*/
/*  *******************___PROTOTYPES___*******************  */
CWBool CWReceiveDiscoveryResponse();
void CWWTPEvaluateAC(CWACInfoValues *ACInfoPtr);
CWBool CWReadResponses();
CWBool CWAssembleDiscoveryRequest(CWProtocolMessage **messagesPtr, int seqNum);
CWBool CWParseDiscoveryResponseMessage(char *msg, int len, int *seqNumPtr, CWACInfoValues *ACInfoPtr);
CWBool CWRenewACList();

/*_________________________________________________________*/
/*  *******************___FUNCTIONS___*******************  */

// Manage Discovery State
CWStateTransition CWWTPEnterDiscovery() {
	int i;
	CWBool j;	

	
	// reset Discovery state
	gCWDiscoveryCount = 0;
	int DiscoveryWTPSockPort=32768;
	CWGetHostIP(gHostip);
	CWNetworkCloseSocket(gWTPSocket);/*why close first*/
//	if(!CWErr(CWNetworkInitSocketClient(&gWTPSocket, NULL,DiscoveryWTPSockPort,gHostip))) {
	if(!CWErr(CWNetworkInitSocketClientUnconnect(&gWTPSocket, NULL,DiscoveryWTPSockPort,gHostip))) {
		CWLog("%s,try discovery again.In Discovery State CWNetworkInitSocketClientUnconnect Error.",strerror(errno));
		return CW_ENTER_DISCOVERY;
	}

	for(i = 0; i < gCWACCount; i++) {
		gCWACList[i].received = CW_FALSE;
		// note: gCWACList can be freed and reallocated (reading from config file)
		// at each transition to the discovery state to save memory space
	}
	
	// wait a random time
	autelan_sleep(CWRandomIntInRange(gCWDiscoveryInterval, gCWMaxDiscoveryInterval));

	CW_REPEAT_FOREVER {
		CWBool sentSomething = CW_FALSE;
	
		if(gCWDiscoveryCount == gCWMaxDiscoveries) { // we get no responses for a very long time

			if(gACListSeqNum == (gCWACCount-1))
				gACListSeqNum = 0;
			else
				gACListSeqNum ++ ;
			return CW_ENTER_SULKING;
		}

		// send Requests to one or more ACs
		if((!gCWACList[gACListSeqNum].received)) { // this AC hasn't responded to us
			// send a Discovery Request
			CWProtocolMessage *msgPtr = NULL;
			
			// increase sequence number
			gCWACList[gACListSeqNum].seqNum = CWGetSeqNum();
			
			if(!CWErr(CWAssembleDiscoveryRequest(&msgPtr, gCWACList[gACListSeqNum].seqNum))) {
				return CW_ENTER_DISCOVERY;
			}
			
                        CW_CREATE_OBJECT_ERR(gACInfoPtr, CWACInfoValues, CWLog("malloc failed for gACInfoPtr");return CW_QUIT;);
			
			CWNetworkGetAddressForHost(gCWACList[gACListSeqNum].address, &(gACInfoPtr->preferredAddress));

			if(gWorkMode != CW_WORKMODE_FAT)
				CWUseSockNtop(&(gACInfoPtr->preferredAddress), CWWTPDebugLog("Send Discovery Request to %s",str););
			
			j = CWErr(CWNetworkSendUnsafeUnconnected(gWTPSocket, &(gACInfoPtr->preferredAddress), (*msgPtr).msg, (*msgPtr).offset)); // log eventual error and continue
			
			//CWUseSockNtop(&(gACInfoPtr->preferredAddress), CWLog("WTP sends Discovery Request to: %s", str););
							
			CW_FREE_PROTOCOL_MESSAGE(*msgPtr);
			CW_FREE_OBJECT(msgPtr);
			CW_FREE_OBJECT(gACInfoPtr);
			
			sentSomething = CW_TRUE; // we sent at least one Request in this loop (even if we got an error sending it)

		}

		if(!sentSomething && CWWTPFoundAnAC()) break;	// All AC sent the response (so we didn't send any request)
		
		gCWDiscoveryCount++;
		
		// wait for Responses
		if(CWErr(CWReadResponses()) && CWWTPFoundAnAC()) {
			// we read at least one valid Discovery Response
			//break;
			if(receivedDisResp)
				break;
//			||((1 == gACInfoPtr->IPv4AddressesCount)&&gACInfoPtr->IPv4Addresses)
			if(!CWRenewACList())
				break;
			CWWTPDebugLog("after renew AC list in discovery, gCWACCount:%d", gCWACCount);
			for(i = 0; i < gCWACCount; i++) {
				CWWTPDebugLog("AC ip list  %s", gCWACList[i].address);
			}
			receivedDisResp = CW_TRUE;
			gACListSeqNum = 0;
			return CW_ENTER_SULKING;
		}
		
	}
	
	
	if(!CWWTPFoundAnAC()) {	// critical error: here we should have received at least one Discovery Response
		return CW_ENTER_DISCOVERY;
	}
	
	// if the AC is multi homed, we select our favorite AC's interface
	CWWTPPickACInterface();
		
	CWUseSockNtop(&(gACInfoPtr->preferredAddress),
			CWWTPDebugLog("Preferred AC: \"%s\", at address: %s", gACInfoPtr->name, str);
	);
#ifndef CW_NO_DTLS
	gDtlsSecurity = gACInfoPtr->security;	/* add for dtls */
	gDtlsPolicy = gACInfoPtr->DTLSPolicy;
	CWWTPDebugLog("gDtlsSecurity:%d   gDtlsPolicy:%d", gDtlsSecurity, gDtlsPolicy);
#endif
	CWWTPDebugLog("======== get an ac ======");
#ifndef CW_NO_DTLS
	CWWTPDebugLog("gDtlsSecurity:%d   gDtlsPolicy:%d\n", gDtlsSecurity, gDtlsPolicy);
#endif
	if(gEMenuConf.e_swtich == 1){
		;
	}else{
		_system("/usr/sbin/ac-get");
	}
	
	return CW_ENTER_JOIN;
}

// Wait DiscoveryInterval time while receiving Discovery Responses
CWBool CWReadResponses() {
	CWBool result = CW_FALSE;
	
	struct timeval timeout, before, after, delta, newTimeout;
	
	timeout.tv_sec = newTimeout.tv_sec = gCWDiscoveryInterval;
	timeout.tv_usec = newTimeout.tv_usec = 0;
	
	autelan_gettimeofday(&before, NULL);

	CW_REPEAT_FOREVER {
		// check if something is available to read until newTimeout
		if(CWNetworkTimedPollRead(gWTPSocket, &newTimeout)) { // success
			// if there was no error, raise a "success error", so we can easily handle
			// all the cases in the switch
			CWErrorRaise(CW_ERROR_SUCCESS, NULL);
		}

		switch(CWErrorGetLastErrorCode()) {
			case CW_ERROR_TIME_EXPIRED:
				goto cw_time_over;
				break;
				
			case CW_ERROR_SUCCESS:
				result = (CWReceiveDiscoveryResponse());
				break; //zengmin add by Coverity 2013-06-09
			case CW_ERROR_INTERRUPTED: // something to read OR interrupted by the system
				// wait for the remaining time (NetworkPoll will be recalled with the remaining time)
			
				autelan_gettimeofday(&after, NULL);
			
				CWTimevalSubtract(&delta, &after, &before);
				if(CWTimevalSubtract(&newTimeout, &timeout, &delta) == 1) { // negative delta: time is over
					goto cw_time_over;
				}
				break;
			default:
				if(!CWErrorHandleLast())
					CWWTPDebugLog("Failure.%s %d",__func__,__LINE__);
				goto cw_error;
				break;
			
		}
	}
	
	cw_time_over:
		// time is over
	cw_error:
		return result;
}

// Gets a datagram from network that should be a Discovery Response
CWBool CWReceiveDiscoveryResponse() {
	char buf[CW_BUFFER_SIZE];
	int i;
	CWNetworkLev4Address addr;
	CWACInfoValues *ACInfoPtr;
	int seqNum;
	int readBytes;
	
	// receive the datagram
	if(!CWErr(CWNetworkReceiveUnsafe(gWTPSocket, buf, CW_BUFFER_SIZE-1, 0, &addr, &readBytes))) {
		return CW_FALSE;
	}
	
        CW_CREATE_OBJECT_ERR(ACInfoPtr, CWACInfoValues, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
		memset(ACInfoPtr,0,sizeof(CWACInfoValues));//zengmin add for memset ACInfoPtr
	// check if it is a valid Discovery Response
	/*
	if(!CWErr(CWParseDiscoveryResponseMessage(&buf[4], readBytes - 4, &seqNum, ACInfoPtr))) {
	*/
	if(!CWErr(CWParseDiscoveryResponseMessage(buf, readBytes, &seqNum, ACInfoPtr))) {
		/*
		 * BUG ML09
		 * 19/10/2009 - Donato Capitella
		 * 14/3/2011 - Pei Wenhui
		 */
		if(ACInfoPtr->vendorInfos.vendorInfosCount > 0)
		{
			for(i = 0; i < ACInfoPtr->vendorInfos.vendorInfosCount; i++)
			{
				if((ACInfoPtr->vendorInfos.vendorInfos)[i].valuePtr != NULL)
					CW_FREE_OBJECT((ACInfoPtr->vendorInfos.vendorInfos)[i].valuePtr);
			}
		}
		if(ACInfoPtr->name != NULL)
			CW_FREE_OBJECT(ACInfoPtr->name);
		CW_FREE_OBJECT(ACInfoPtr);
		return CWErrorRaise(CW_ERROR_INVALID_FORMAT, "Received something different from a Discovery Response while in Discovery State");
	}
//	gDtlsSecurity = ACInfoPtr->security;
//	gDtlsPolicy = ACInfoPtr->DTLSPolicy;
	CW_COPY_NET_ADDR_PTR(&(ACInfoPtr->incomingAddress), &(addr));
	
	// see if this AC is better than the one we have stored
	CWWTPEvaluateAC(ACInfoPtr);
	

	// check if the sequence number we got is correct
	for(i = 0; i < gCWACCount; i++) {
		if(gCWACList[i].seqNum == seqNum) {
			gCWACList[i].received = CW_TRUE; // we received response from this address
	
			return CW_TRUE;
		}
	}
	
	return CWErrorRaise(CW_ERROR_INVALID_FORMAT, "Sequence Number of Response doesn't macth Request");
}


void CWWTPEvaluateAC(CWACInfoValues *ACInfoPtr) {
	if(ACInfoPtr == NULL) return;
	
	if(gACInfoPtr == NULL) { // this is the first AC we evaluate: so  it's the best AC we examined so far
		gACInfoPtr = ACInfoPtr;
	} else {
		/*
		 * BUG ML09
		 * 19/10/2009 - Donato Capitella
		 * 14/3/2011 - Pei Wenhui
		 */
		if(ACInfoPtr->vendorInfos.vendorInfosCount > 0)
		{
			int i=0;
			for(i = 0; i < ACInfoPtr->vendorInfos.vendorInfosCount; i++) 
			{
				if((ACInfoPtr->vendorInfos.vendorInfos)[i].valuePtr != NULL)
					CW_FREE_OBJECT((ACInfoPtr->vendorInfos.vendorInfos)[i].valuePtr);
			}
		}
		if(ACInfoPtr->name != NULL)
			CW_FREE_OBJECT(ACInfoPtr->name);
		CW_FREE_OBJECT(ACInfoPtr);
	}
	
	// ... note: we can add our favourite algorithm to pick the best AC. We can also consider to remember
	// all the Discovery Responses we received and not just the best.
}

// pick one interface of the AC (easy if there is just one interface). The current algorithm just pick the Ac with less WTP
// communicating with it. If the addresses returned by the AC in the Discovery Response don't include the address of the sender
// of the Discovery Response, we ignore the address in the Response and use the one of the sender (maybe the AC sees garbage address,
// i.e. it is behind a NAT).
void CWWTPPickACInterface() {
	int i, min;
	CWBool foundIncoming = CW_FALSE;
	if(gACInfoPtr == NULL) return;
	
	gACInfoPtr->preferredAddress.ss_family = AF_UNSPEC;
	
	if(gNetworkPreferredFamily == CW_IPv6) {
		goto cw_pick_IPv6;
	}
	
	cw_pick_IPv4:
		if(gACInfoPtr->IPv4Addresses == NULL || gACInfoPtr->IPv4AddressesCount <= 0) return;
		
		min = gACInfoPtr->IPv4Addresses[0].WTPCount;
		CW_COPY_NET_ADDR_PTR(&(gACInfoPtr->preferredAddress), &(gACInfoPtr->IPv4Addresses[0].addr));
		
		for(i = 1; i < gACInfoPtr->IPv4AddressesCount; i++) {
			if(!sock_cmp_addr((struct sockaddr*)&(gACInfoPtr->IPv4Addresses[i]), (struct sockaddr*)&(gACInfoPtr->incomingAddress), sizeof(struct sockaddr_in))) foundIncoming = CW_TRUE;

			if(gACInfoPtr->IPv4Addresses[i].WTPCount < min) {
				min = gACInfoPtr->IPv4Addresses[i].WTPCount;
				CW_COPY_NET_ADDR_PTR(&(gACInfoPtr->preferredAddress), &(gACInfoPtr->IPv4Addresses[i].addr));
			}
		}
		
		if(!foundIncoming) {
			// If the addresses returned by the AC in the Discovery Response don't include the address of the sender
			// of the Discovery Response, we ignore the address in the Response and use the one of the sender (maybe the AC sees garbage address,
			// i.e. it is behind a NAT).

			CW_COPY_NET_ADDR_PTR(&(gACInfoPtr->preferredAddress), &(gACInfoPtr->incomingAddress));
		}
		return;
		
	cw_pick_IPv6:
		CWWTPDebugLog("Pick IPv6...");
		if(gACInfoPtr->IPv6Addresses == NULL || gACInfoPtr->IPv6AddressesCount <= 0) goto cw_pick_IPv4;
		
		min = gACInfoPtr->IPv6Addresses[0].WTPCount;
		CW_COPY_NET_ADDR_PTR(&(gACInfoPtr->preferredAddress), &(gACInfoPtr->IPv6Addresses[0].addr));
		
		for(i = 1; i < gACInfoPtr->IPv6AddressesCount; i++) {
			if(!sock_cmp_addr((struct sockaddr *)&(gACInfoPtr->IPv6Addresses[i]), (struct sockaddr *)&(gACInfoPtr->incomingAddress), sizeof(struct sockaddr_in6))) foundIncoming = CW_TRUE;
			
			if(gACInfoPtr->IPv6Addresses[i].WTPCount < min) {
				min = gACInfoPtr->IPv6Addresses[i].WTPCount;
				CW_COPY_NET_ADDR_PTR(&(gACInfoPtr->preferredAddress), &(gACInfoPtr->IPv6Addresses[i].addr));
			}
		}
		
		if(!foundIncoming) {
			CW_COPY_NET_ADDR_PTR(&(gACInfoPtr->preferredAddress), &(gACInfoPtr->incomingAddress));
		}
		
		return;
}

CWBool CWAssembleDiscoveryRequest(CWProtocolMessage **messagesPtr, int seqNum) {
	CWProtocolMessage *msgElems= NULL;
	const int msgElemCount = 5;
	CWProtocolMessage *msgElemsBinding= NULL;
	const int msgElemBindingCount=0;
	int k = -1;
	int fragmentsNum;

	if(messagesPtr == NULL) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);

	CW_CREATE_PROTOCOL_MSG_ARRAY_ERR(msgElems, msgElemCount, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););	
	
	// Assemble Message Elements
	if((!(CWAssembleMsgElemDiscoveryType(&(msgElems[++k])))) ||
	   (!(CWAssembleMsgElemWTPBoardData(&(msgElems[++k]))))	 ||
	   (!(CWAssembleMsgElemWTPDescriptor(&(msgElems[++k])))) ||
	   (!(CWAssembleMsgElemWTPFrameTunnelMode(&(msgElems[++k])))) ||
	   (!(CWAssembleMsgElemWTPMACType(&(msgElems[++k])))) 
	){
		int i;
		for(i = 0; i <= k; i++) { CW_FREE_PROTOCOL_MESSAGE(msgElems[i]);}
		CW_FREE_OBJECT(msgElems);
		return CW_FALSE; // error will be handled by the caller
	}
	
	return CWAssembleMessage(messagesPtr, &fragmentsNum, 0, seqNum, CW_MSG_TYPE_VALUE_DISCOVERY_REQUEST, msgElems, msgElemCount, msgElemsBinding, msgElemBindingCount, CW_PACKET_PLAIN);
}

// Parse Discovery Response and return informations in *ACInfoPtr
CWBool CWParseDiscoveryResponseMessage(char *msg, int len, int *seqNumPtr, CWACInfoValues *ACInfoPtr) {
	CWControlHeaderValues controlVal;
	CWProtocolTransportHeaderValues transportVal;
	int offsetTillMessages, i, j;
	
	CWProtocolMessage completeMsg;
	
	if(msg == NULL || seqNumPtr == NULL || ACInfoPtr == NULL) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	
	
	completeMsg.msg = msg;
	completeMsg.offset = 0;
	
	CWBool dataFlag = CW_FALSE;
	if(!(CWParseTransportHeader(&completeMsg, &transportVal, &dataFlag))) return CW_FALSE; // will be handled by the caller
	if(!(CWParseControlHeader(&completeMsg, &controlVal))) return CW_FALSE; // will be handled by the caller
	
	// different type
	if(controlVal.messageTypeValue != CW_MSG_TYPE_VALUE_DISCOVERY_RESPONSE)
		return CWErrorRaise(CW_ERROR_INVALID_FORMAT, "Message is not Discovery Response as Expected");
	
	*seqNumPtr = controlVal.seqNum;
	
	controlVal.msgElemsLen -= CW_CONTROL_HEADER_OFFSET_FOR_MSG_ELEMS; // skip timestamp
	
	offsetTillMessages = completeMsg.offset;
	
	ACInfoPtr->IPv4AddressesCount = 0;
	ACInfoPtr->IPv6AddressesCount = 0;
	// parse message elements
	while((completeMsg.offset-offsetTillMessages) < controlVal.msgElemsLen) {
		unsigned short int type=0;// = CWProtocolRetrieve32(&completeMsg);
		unsigned short int len=0;// = CWProtocolRetrieve16(&completeMsg);
		
		CWParseFormatMsgElem(&completeMsg,&type,&len);
		
		switch(type) {
			case CW_MSG_ELEMENT_AC_DESCRIPTOR_CW_TYPE:
				if(!(CWParseACDescriptor(&completeMsg, len, ACInfoPtr))) return CW_FALSE; // will be handled by the caller
				break;
			case CW_MSG_ELEMENT_AC_NAME_CW_TYPE:
				if(!(CWParseACName(&completeMsg, len, &(ACInfoPtr->name)))) return CW_FALSE; // will be handled by the caller
				break;
			case CW_MSG_ELEMENT_CW_CONTROL_IPV4_ADDRESS_CW_TYPE:
				ACInfoPtr->IPv4AddressesCount++; // just count how many interfacess we have, so we can allocate the array
				completeMsg.offset += len;
				break;
			case CW_MSG_ELEMENT_CW_CONTROL_IPV6_ADDRESS_CW_TYPE:
				ACInfoPtr->IPv6AddressesCount++; // just count how many interfacess we have, so we can allocate the array
				completeMsg.offset += len;
				break;
			default:
//				return CWErrorRaise(CW_ERROR_INVALID_FORMAT, "Unrecognized Message Element");
				CWWTPDebugLog("Unrecognized Message Element:%d, in Discovery Response", type);
				completeMsg.offset += len;
				break;
		}

	}
	
	if(completeMsg.offset != len) return CWErrorRaise(CW_ERROR_INVALID_FORMAT, "Garbage at the End of the Message");
	
	// actually read each interface info
	CW_CREATE_ARRAY_ERR(ACInfoPtr->IPv4Addresses, ACInfoPtr->IPv4AddressesCount, CWProtocolIPv4NetworkInterface,
							return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	
	if(ACInfoPtr->IPv6AddressesCount > 0) {
		CW_CREATE_ARRAY_ERR(ACInfoPtr->IPv6Addresses, ACInfoPtr->IPv6AddressesCount, CWProtocolIPv6NetworkInterface,
							return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	}
	i = 0, j = 0;
	
	completeMsg.offset = offsetTillMessages;
	while((completeMsg.offset-offsetTillMessages) < controlVal.msgElemsLen) {
		unsigned short int type=0;// = CWProtocolRetrieve32(&completeMsg);
		unsigned short int len=0;// = CWProtocolRetrieve16(&completeMsg);
		
		CWParseFormatMsgElem(&completeMsg,&type,&len);		
		
		switch(type) {
			case CW_MSG_ELEMENT_CW_CONTROL_IPV4_ADDRESS_CW_TYPE:
				if(!(CWParseCWControlIPv4Addresses(&completeMsg, len, &(ACInfoPtr->IPv4Addresses[i])))) return CW_FALSE; // will be handled by the caller
				i++;
				break;
			case CW_MSG_ELEMENT_CW_CONTROL_IPV6_ADDRESS_CW_TYPE:
				if(!(CWParseCWControlIPv6Addresses(&completeMsg, len, &(ACInfoPtr->IPv6Addresses[j])))) return CW_FALSE; // will be handled by the caller
				j++;
				break;
			default:
				completeMsg.offset += len;
				break;
		}
	}

	return CW_TRUE;
}

CWBool CWRenewACList()
{
	CWACDescriptor *tmpACList;
	int temACCount = 0;
	int i;
	int j;
#if 0
	if(1 == gACInfoPtr->IPv4AddressesCount)
	{
		char *AcAddressStr;
		char *ptr;
		char ipstr[128];
		AcAddressStr = sock_ntop_r(&(gACInfoPtr->IPv4Addresses[0].addr),ipstr);
		for(ptr = AcAddressStr; ; ptr++)
		{
			if(*ptr == ':')
			{
				*ptr = '\0';
				break;
			}
		}
		dpf("ac ip list :%s\n", AcAddressStr);
		if(0 == memcmp(AcAddressStr, "0.0.0.0", strlen(AcAddressStr)))
		{
			dpf("AC ip from discovery response is 0.0.0.0, invalid... return.\n");
			return CW_FALSE;
		}
	}
#else
	if(gACInfoPtr->IPv4AddressesCount>0)
	{
		if(gACInfoPtr->IPv4Addresses[0].WTPCount != 9999)
		{
			CWWTPDebugLog("There is no AC IP list in discovery response... return");
			return CW_FALSE;
		}
	}
#endif
	temACCount = gCWACCount;
	gCWACCount += gACInfoPtr->IPv4AddressesCount;
	CW_CREATE_ARRAY_ERR(tmpACList, gCWACCount, CWACDescriptor, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	for(i=0;i<gACInfoPtr->IPv4AddressesCount;i++)
	{
		char *AcAddressStr;
		char *ptr;
		char ipstr[128];
		AcAddressStr = sock_ntop_r((struct sockaddr *)&(gACInfoPtr->IPv4Addresses[i].addr),ipstr);
		for(ptr = AcAddressStr; ; ptr++)
		{
			if(*ptr == ':')
			{
				*ptr = '\0';
				break;
			}
		}
		CWWTPDebugLog("ac ip list :%s", AcAddressStr);
		CW_CREATE_STRING_FROM_STRING_ERR(tmpACList[i].address, AcAddressStr, {for(j=i-1;j>=0;j--){CW_FREE_OBJECT(tmpACList[j].address);}CW_FREE_OBJECT(tmpACList);return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL);});
		tmpACList[i].received = CW_FALSE;

		CWWTPDebugLog("Receiving discovery response, now AC addr:%s", tmpACList[i].address);
	}

	for(i=gACInfoPtr->IPv4AddressesCount;i<gCWACCount;i++)
	{
		CW_CREATE_STRING_FROM_STRING_ERR(tmpACList[i].address, gCWACList[i-gACInfoPtr->IPv4AddressesCount].address, {for(j=i-1;j>=0;j--){CW_FREE_OBJECT(tmpACList[j].address);}CW_FREE_OBJECT(tmpACList);return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL);});
		tmpACList[i].received = gCWACList[i-gACInfoPtr->IPv4AddressesCount].received;
		tmpACList[i].seqNum = gCWACList[i-gACInfoPtr->IPv4AddressesCount].seqNum;
		CWWTPDebugLog("Receiving discovery response, Old AC addr:%s", tmpACList[i].address);
	}
	for(i=0;i<temACCount;i++)
		CW_FREE_OBJECT(gCWACList[i].address);
	CW_FREE_OBJECT(gCWACList);
	gCWACList = tmpACList;
	
	return CW_TRUE;
}

