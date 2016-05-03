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

static CWBool gSuccessfulHandshake = CW_TRUE;
int gCWWaitJoin = CW_JOIN_INTERVAL_DEFAULT;
CWImageIdentifierValues *gImageIdentifier_ACSupported = NULL;
char gHostip[24];
/*__________________________________________________________*/
/*  *******************___PROTOTYPES___*******************  */
void CWWTPWaitJoinExpired(CWTimerArg arg);
CWBool CWAssembleJoinRequest(CWProtocolMessage **messagesPtr, int *fragmentsNumPtr, int PMTU, int seqNum, CWList msgElemList);
CWBool CWParseJoinResponseMessage(char *msg, int len, int seqNum, CWProtocolJoinResponseValues *valuesPtr);
CWBool CWSaveJoinResponseMessage (CWProtocolJoinResponseValues *joinResponse);
CWBool CWParseImageIdentifier (CWProtocolMessage *msgPtr, unsigned short int len, CWImageIdentifierValues *valPtr);         //pei add 0617

/*_____________________________________________________*/
/*  *******************___FUNCTIONS___*******************  */
// Manage Join State
CWStateTransition CWWTPEnterJoin() {
	CWTimerID waitJoinTimer;
	int seqNum;
	CWProtocolJoinResponseValues values;
	
	CWDebugLog("\n");
	CWDebugLog("######### Join State #########");
	CWWTPDebugLog("######### Join State #########");
	// reset Join state
	CWNetworkCloseSocket(gWTPSocket);
#ifndef CW_NO_DTLS//csc
	if(gDtlsSecurity == 1)
	{
		CWSecurityDestroySession(gWTPSession);
		CWSecurityDestroyContext(gWTPSecurityContext);
		gWTPSecurityContext = NULL;
		gWTPSession = NULL;
	}
#endif
	//Initialize gACInfoPtr
	gACInfoPtr->ACIPv4ListInfo.ACIPv4ListCount=0;
	gACInfoPtr->ACIPv4ListInfo.ACIPv4List=NULL;	
	gACInfoPtr->ACIPv6ListInfo.ACIPv6ListCount=0;
	gACInfoPtr->ACIPv6ListInfo.ACIPv6List=NULL;

	/*
	 * MAURO: allo scadere del waitJoinTimer viene invocata, in modo 
	 * asincrono, CWWTPWaitJoinExpired(0) 	
	 */

	if ((waitJoinTimer = timer_add(gCWWaitJoin, 0, CWWTPWaitJoinExpired, NULL)) == -1) {
		return CW_ENTER_DISCOVERY;
	}

	/*
	 * MAURO: gWTPForceACAddress contine l'"indirizzo" dell'AC con cui fare
	 * il JOIN nel caso che sia specificato nel file di configuarazione.
	 * Se questa var globale e' settata da WTP.c viene invocata direttamente
	 * CWWTPEnterJoin per saltare il DISCOVERY.
	 */
	if(gWTPForceACAddress != NULL) {
		CW_CREATE_OBJECT_ERR(gACInfoPtr, CWACInfoValues, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););	
		CWNetworkGetAddressForHost(gWTPForceACAddress, &(gACInfoPtr->preferredAddress));
		gACInfoPtr->security = gWTPForceSecurity;
	}
	
	// Init DTLS session
	int WTPSockPort = 32768;
#ifndef CW_NO_DTLS
	if(gDtlsSecurity == 1)
	{
		if(!CWErr(CWNetworkInitSocketClientConnect(&gWTPSocket, &(gACInfoPtr->preferredAddress), WTPSockPort)) ) {
			CWWTPDebugLog("control sock init error.SM(dtls) will jump from Join State to Discovery State.");
			CWNetworkCloseSocket(gWTPSocket);
			timer_rem(waitJoinTimer,0);
			return CW_ENTER_DISCOVERY;
		}
	}
	else
#endif
	{
		CWGetHostIP(gHostip);
		if(!CWErr(CWNetworkInitSocketClientUnconnect(&gWTPSocket, NULL, WTPSockPort, gHostip))) {
			CWWTPDebugLog("control sock init error.SM will jump from Join State to Discovery State.");
			CWNetworkCloseSocket(gWTPSocket);
			timer_rem(waitJoinTimer,0);
			return CW_ENTER_DISCOVERY;
		}
	}

	/* AC address */
	char str[128];
	struct sockaddr *sa = (struct sockaddr*) (&gACInfoPtr->preferredAddress);

	if(gNetworkPreferredFamily == CW_IPv4)
	{
		struct sockaddr_in	*sin = (struct sockaddr_in *)sa;
		inet_ntop(AF_INET, &sin->sin_addr, str, sizeof(str));
		/*get time from AC*/
		CWNtp_TimeSync(str);
	}
	else
	{
		struct sockaddr_in6 *sin6 = (struct sockaddr_in6 *)sa;
		inet_ntop(AF_INET6, &sin6->sin6_addr, str, sizeof(str));
		/*get time from AC*/
		CWNtp_TimeSync(str);
	}

#ifndef CW_NO_DTLS
//	if(gACInfoPtr->security == CW_X509_CERTIFICATE)
	if(gDtlsSecurity == 1)
	{
		if(!CWErr(CWSecurityInitContext(&gWTPSecurityContext, "/etc/wlan/root.pem", "/etc/wlan/client.pem", "prova", CW_TRUE, NULL))) {

			timer_rem(waitJoinTimer,0);
			CWNetworkCloseSocket(gWTPSocket);
			gWTPSecurityContext = NULL;
			return CW_ENTER_DISCOVERY;
		}
	}
//	else { // pre-shared keys
//		if(!CWErr(CWSecurityInitContext(&gWTPSecurityContext, NULL, NULL, NULL, CW_TRUE, NULL)))
//		{
//			timer_rem(waitJoinTimer,0);
//			CWNetworkCloseSocket(gWTPSocket);
//			gWTPSecurityContext = NULL;
//			return CW_ENTER_DISCOVERY;
//		}
//	}
#endif

	static CWThread thread_receiveFrame;
	if( (thread_receiveFrame != 0) && (0 == pthread_kill(thread_receiveFrame, 0) ) ){
		pthread_cancel(thread_receiveFrame);
		pthread_join(thread_receiveFrame, NULL);
	}
	if(!CWErr(CWCreateThread(&thread_receiveFrame, CWWTPReceiveDtlsPacket, (void*)gWTPSocket,0))) {
		
		CWDebugLog("Error starting Thread that receive DTLS packet");
		timer_rem(waitJoinTimer,0);
		CWNetworkCloseSocket(gWTPSocket);
#ifndef CW_NO_DTLS
		if(gDtlsSecurity == 1)
		{
			CWSecurityDestroyContext(gWTPSecurityContext);
			gWTPSecurityContext = NULL;
			gWTPSession = NULL;
		}
#endif
		return CW_ENTER_DISCOVERY;
	}
	
#ifndef CW_NO_DTLS
	if(gDtlsSecurity == 1)
	{
		if(!CWErr(CWSecurityInitSessionClient(gWTPSocket, &(gACInfoPtr->preferredAddress), gPacketReceiveList, gWTPSecurityContext, &gWTPSession, &gWTPPathMTU))) { // error creating DTLS session
			
			timer_rem(waitJoinTimer,0);		
			CWNetworkCloseSocket(gWTPSocket);
			CWSecurityDestroyContext(gWTPSecurityContext);
			gWTPSecurityContext = NULL;
			gWTPSession = NULL;
			return CW_ENTER_DISCOVERY;
		}
	}
#endif
	if(gCWForceMTU > 0) {
		gWTPPathMTU = gCWForceMTU;
	}
	
	CWDebugLog("Path MTU for this Session: %d", gWTPPathMTU);
	
	// send Join Request
	seqNum = CWGetSeqNum();

	if(!CWErr(CWWTPSendAcknowledgedPacket(seqNum, NULL, CWAssembleJoinRequest, (void*) CWParseJoinResponseMessage, (void*) CWSaveJoinResponseMessage, &values))) {
cw_join_err:
		
		timer_rem(waitJoinTimer,0);
		CWNetworkCloseSocket(gWTPSocket);
#ifndef CW_NO_DTLS
		if(gDtlsSecurity == 1)
		{
			CWSecurityDestroySession(gWTPSession);
			CWSecurityDestroyContext(gWTPSecurityContext);
			gWTPSecurityContext = NULL;
			gWTPSession = NULL;
		}
#endif
		CWWTPDebugLog("Enter CW_ENTER_DISCOVERY ... In Join State Sending or Receiving Error.");
		return CW_ENTER_DISCOVERY;
	}
	
#if 1
	if(gtemp_state == CW_ENTER_IMAGE_DATA)                                             //pei add 0616--------------------------
	{
		gtemp_state = -1;
		timer_rem(waitJoinTimer,0);
		CWDebugLog("entering image data state!!!\n");
		return CW_ENTER_IMAGE_DATA;
	}
	gtemp_state = -1;
#endif
	timer_rem(waitJoinTimer,0);
	
	if(!gSuccessfulHandshake) { // timer expired
		goto cw_join_err;
	}
	
	CWDebugLog("Join Completed");

	
	return CW_ENTER_CONFIGURE;
}

void CWWTPWaitJoinExpired(CWTimerArg arg) {
	CWDebugLog("WTP Wait Join Expired");

	// kill session 
	/*
	if((r=SSL_shutdown(gWTPSession)) == 1) {
		CWDebugLog("Shutdown Completed");
	} else if(r == 0) {
		CWDebugLog("Partial Shutdown");
		if((r=SSL_shutdown(gWTPSession)) == 1) {
			CWDebugLog("Shutdown Completed");
		}
	}
	*/
	
	// TO-DO: at this point we can:
	// 1) try with a different address (interface) for this AC
	// 2) if we tried all the addresses mark this AC as non-responsive and try with another AC we discovered
	// At the moment, what we do is:
	// Repeat Discovery
	gSuccessfulHandshake = CW_FALSE;
	CWNetworkCloseSocket(gWTPSocket);
}

CWBool CWAssembleJoinRequest(CWProtocolMessage **messagesPtr, int *fragmentsNumPtr, int PMTU, int seqNum, CWList msgElemList) {
	CWProtocolMessage *msgElems= NULL;
	const int msgElemCount = 8;
	CWProtocolMessage *msgElemsBinding= NULL;
	const int msgElemBindingCount=0;
	int k = -1;
	
	if(messagesPtr == NULL || fragmentsNumPtr == NULL) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	
	CW_CREATE_PROTOCOL_MSG_ARRAY_ERR(msgElems, msgElemCount, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););	
		
	CWDebugLog("Sending Join Request...");
	
	// Assemble Message Elements
	if ((!(CWAssembleMsgElemLocationData(&(msgElems[++k])))) ||
	     (!(CWAssembleMsgElemWTPBoardData(&(msgElems[++k])))) ||
	     (!(CWAssembleMsgElemWTPDescriptor(&(msgElems[++k])))) ||
	     (!(CWAssembleMsgElemWTPIPv4Address(&(msgElems[++k])))) ||
	     (!(CWAssembleMsgElemWTPName(&(msgElems[++k])))) ||
	     (!(CWAssembleMsgElemSessionID(&(msgElems[++k]), CWWTPGetSessionID()))) ||
	     (!(CWAssembleMsgElemWTPFrameTunnelMode(&(msgElems[++k])))) ||
	     (!(CWAssembleMsgElemWTPMACType(&(msgElems[++k]))))
	) {
		int i;
		for(i = 0; i <= k; i++) {
			CW_FREE_PROTOCOL_MESSAGE(msgElems[i]);
			}
		CW_FREE_OBJECT(msgElems);
		return CW_FALSE; // error will be handled by the caller
	}
	
	return CWAssembleMessage(messagesPtr,
				 fragmentsNumPtr,
				 PMTU,
				 seqNum,
				 CW_MSG_TYPE_VALUE_JOIN_REQUEST,
				 msgElems,
				 msgElemCount,
				 msgElemsBinding,
				 msgElemBindingCount,
#ifdef CW_NO_DTLS
				 CW_PACKET_PLAIN
#else
				(gDtlsSecurity == 1)?CW_PACKET_CRYPT:CW_PACKET_PLAIN           /* 0-CW_PACKET_PLAIN, 1-CW_PACKET_CRYPT */
#endif
				 );
}


// Parse Join Response and return informations in *valuesPtr
CWBool CWParseJoinResponseMessage(char *msg, int len, int seqNum, CWProtocolJoinResponseValues *valuesPtr) {
	CWControlHeaderValues controlVal;
	int offsetTillMessages;
	
	CWProtocolMessage completeMsg;
#if 0
	len = 92;
	int SEQNUM;
//	sprintf(SEQNUM,"0x%02x",seqNum);
	if(seqNum == 1)
		SEQNUM = 0x01;
	else if(seqNum == 2)
		SEQNUM = 0x02;
	else if(seqNum == 3)
		SEQNUM = 0x03;
	else if(seqNum == 4)
		SEQNUM = 0x04;
	else if(seqNum == 5)
		SEQNUM = 0x05;
	unsigned char my_buf1[CW_BUFFER_SIZE] = /*test join response*/
	{
		0x00,0x00,0x00,0x04,         /* capwap head */
		SEQNUM,
//		0x00,0x46,        /* length */
		0x00,0x57,
		0x00,
		
		0x00,0x01,                 /* type 1 for AC Descriptor */
		0x00,0x24,
		0x00,0x00,0x00,0xc8,0x00,0x01,0x00,0x0f,0x01,0x00,0x00,0x02,0x00,0x00,0xff,0x98,0x00,0x04,
		0x00,0x04,0x00,0x12,0xda,0xc8,0x00,0x00,0xff,0x98,0x00,0x05,0x00,0x04,0x00,0x31,0xb2,0x98,
		
		0x00,0x04,                 /* type 4 for AC Name */
		0x00,0x05,
		0x4d,0x79,0x20,0x41,0x43,

		0x00,0x0a,                 /* type 10 for CAPWAP Control IPv4 Address */
		0x00,0x06,
		0x7f,0x00,0x00,0x01,0x00,0x01,

		0x00,0x21,                 /* type 33 for Result Code */
		0x00,0x04,
		0x00,0x00,0x00,0x00,

		0x00,0x19,                 /* type 25 for Image Identifier */
		0x00,0x0d,
		0x01,                    /* Num of version */
		0x00,0x00,0x00,    /* reserved */
		0x00,0x01,
		0x00,0x05,
		0x31,0x2e,0x30,0x2e,0x32
		
	};

	CWCaptrue(110, msg);
#endif	
	if(msg == NULL || valuesPtr == NULL) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	
	CWDebugLog("Parsing Join Response...");
	
	completeMsg.msg = msg;
	completeMsg.offset = 0;
	
	if(!(CWParseControlHeader(&completeMsg, &controlVal))) return CW_FALSE; // error will be handled by the caller
	
	// different type
	if(controlVal.messageTypeValue != CW_MSG_TYPE_VALUE_JOIN_RESPONSE)
		return CWErrorRaise(CW_ERROR_INVALID_FORMAT, "Message is not Join Response as Expected");
	
	if(controlVal.seqNum != seqNum) return CWErrorRaise(CW_ERROR_INVALID_FORMAT, "Different Sequence Number");
	
	controlVal.msgElemsLen -= CW_CONTROL_HEADER_OFFSET_FOR_MSG_ELEMS; // skip timestamp
	
	offsetTillMessages = completeMsg.offset;

	/* Mauro */
	valuesPtr->ACInfoPtr.IPv4AddressesCount = 0;
	valuesPtr->ACInfoPtr.IPv6AddressesCount = 0;

	// parse message elements
	while((completeMsg.offset-offsetTillMessages) < controlVal.msgElemsLen) {
		unsigned short int type=0;
		unsigned short int len=0;
		
		CWParseFormatMsgElem(&completeMsg,&type,&len);		

		CWDebugLog("Parsing Message Element: %u, len: %u", type, len);
/*
		valuesPtr->ACInfoPtr.IPv4AddressesCount = 0;
		valuesPtr->ACInfoPtr.IPv6AddressesCount = 0;
*/
		valuesPtr->ACIPv4ListInfo.ACIPv4ListCount=0;
		valuesPtr->ACIPv4ListInfo.ACIPv4List=NULL;
		valuesPtr->ACIPv6ListInfo.ACIPv6ListCount=0;
		valuesPtr->ACIPv6ListInfo.ACIPv6List=NULL;
	
		switch(type) {
			case CW_MSG_ELEMENT_AC_DESCRIPTOR_CW_TYPE:
				if(!(CWParseACDescriptor(&completeMsg, len, &(valuesPtr->ACInfoPtr)))) return CW_FALSE; // will be handled by the caller
				break;
			case CW_MSG_ELEMENT_AC_IPV4_LIST_CW_TYPE:
				if(!(CWParseACIPv4List(&completeMsg, len, &(valuesPtr->ACIPv4ListInfo)))) return CW_FALSE;
				break;
			case CW_MSG_ELEMENT_AC_IPV6_LIST_CW_TYPE:
				if(!(CWParseACIPv6List(&completeMsg, len, &(valuesPtr->ACIPv6ListInfo)))) return CW_FALSE;
				break;
			case CW_MSG_ELEMENT_RESULT_CODE_CW_TYPE:
				if(!(CWParseResultCode(&completeMsg, len, &(valuesPtr->code)))) return CW_FALSE;
				break;
			case CW_MSG_ELEMENT_AC_NAME_CW_TYPE:
				if(!(CWParseACName(&completeMsg, len, &(valuesPtr->ACInfoPtr.name)))) return CW_FALSE; // will be handled by the caller
				break;
			case CW_MSG_ELEMENT_CW_CONTROL_IPV4_ADDRESS_CW_TYPE:
				valuesPtr->ACInfoPtr.IPv4AddressesCount++; // just count how many interfacess we have, so we can allocate the array
				completeMsg.offset += len;
				break;
			case CW_MSG_ELEMENT_CW_CONTROL_IPV6_ADDRESS_CW_TYPE:
				valuesPtr->ACInfoPtr.IPv6AddressesCount++; // just count how many interfacess we have, so we can allocate the array
				completeMsg.offset += len;
				break;
			case CW_MSG_ELEMENT_IMAGE_IDENTIFIER_CW_TYPE:                         //pei add 0617-----------------------------
				if(!(CWParseImageIdentifier(&completeMsg, len, &(valuesPtr->ImageIdentifier)))) return CW_FALSE; // will be handled by the caller
				break;
// 			case CW_MSG_ELEMENT_SESSION_ID_CW_TYPE:
// 				if(!(CWParseSessionID(&completeMsg, len, valuesPtr))) return CW_FALSE;
// 				break;	
			default:
//				return CWErrorRaise(CW_ERROR_INVALID_FORMAT, "Unrecognized Message Element");
				CWWTPDebugLog("Unrecognized Message Element:%d, in Join Response.", type);
				completeMsg.offset += len;
				break;
		}

		//CWDebugLog("bytes: %d/%d", (completeMsg.offset-offsetTillMessages), controlVal.msgElemsLen);
	}
	
	if(completeMsg.offset != len) return CWErrorRaise(CW_ERROR_INVALID_FORMAT, "Garbage at the End of the Message");
	
	// actually read each interface info
	CW_CREATE_ARRAY_ERR(valuesPtr->ACInfoPtr.IPv4Addresses, valuesPtr->ACInfoPtr.IPv4AddressesCount, CWProtocolIPv4NetworkInterface,
							return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	
	if(valuesPtr->ACInfoPtr.IPv6AddressesCount > 0) {
		CW_CREATE_ARRAY_ERR(valuesPtr->ACInfoPtr.IPv6Addresses, valuesPtr->ACInfoPtr.IPv6AddressesCount, CWProtocolIPv6NetworkInterface,
							return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	}

	int i = 0;
	int j = 0;
	
	completeMsg.offset = offsetTillMessages;
	while((completeMsg.offset-offsetTillMessages) < controlVal.msgElemsLen) {
		unsigned short int type=0;// = CWProtocolRetrieve32(&completeMsg);
		unsigned short int len=0;// = CWProtocolRetrieve16(&completeMsg);
		
		CWParseFormatMsgElem(&completeMsg,&type,&len);		
		
		switch(type) {
			case CW_MSG_ELEMENT_CW_CONTROL_IPV4_ADDRESS_CW_TYPE:
				if(!(CWParseCWControlIPv4Addresses(&completeMsg, len, &(valuesPtr->ACInfoPtr.IPv4Addresses[i])))) return CW_FALSE; // will be handled by the caller
				i++;
				break;
			case CW_MSG_ELEMENT_CW_CONTROL_IPV6_ADDRESS_CW_TYPE:
				if(!(CWParseCWControlIPv6Addresses(&completeMsg, len, &(valuesPtr->ACInfoPtr.IPv6Addresses[j])))) return CW_FALSE; // will be handled by the caller
				j++;
				break;
			default:
				completeMsg.offset += len;
				break;
		}
	}

	return CW_TRUE;
}

CWBool CWSaveJoinResponseMessage (CWProtocolJoinResponseValues *joinResponse)
{
   if(joinResponse == NULL) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
   if((joinResponse->code == CW_PROTOCOL_SUCCESS) || (joinResponse->code == CW_PROTOCOL_SUCCESS_NAT))
   {
	if(gACInfoPtr == NULL) return CWErrorRaise(CW_ERROR_NEED_RESOURCE, NULL);
	
	gACInfoPtr->stations = (joinResponse->ACInfoPtr).stations;
	gACInfoPtr->limit = (joinResponse->ACInfoPtr).limit;
	gACInfoPtr->activeWTPs = (joinResponse->ACInfoPtr).activeWTPs;
	gACInfoPtr->maxWTPs = (joinResponse->ACInfoPtr).maxWTPs;
	gACInfoPtr->security = (joinResponse->ACInfoPtr).security;
	gACInfoPtr->RMACField = (joinResponse->ACInfoPtr).RMACField;

	/* BUG-ML07
         * Before overwriting the field vendorInfos we'd better
         * free it (it was allocated during the Discovery State by
         * the function CWParseACDescriptor()).
         *
         * 19/10/2009 - Donato Capitella
         */
	int i;
	for(i = 0; i < gACInfoPtr->vendorInfos.vendorInfosCount; i++) {
		CW_FREE_OBJECT(gACInfoPtr->vendorInfos.vendorInfos[i].valuePtr);
	}
	CW_FREE_OBJECT(gACInfoPtr->vendorInfos.vendorInfos);


	gACInfoPtr->vendorInfos = (joinResponse->ACInfoPtr).vendorInfos;
	
	if(joinResponse->ACIPv4ListInfo.ACIPv4ListCount >0)
	{
		gACInfoPtr->ACIPv4ListInfo.ACIPv4ListCount = joinResponse->ACIPv4ListInfo.ACIPv4ListCount; 
		gACInfoPtr->ACIPv4ListInfo.ACIPv4List = joinResponse->ACIPv4ListInfo.ACIPv4List; 
	}
	
	if(joinResponse->ACIPv6ListInfo.ACIPv6ListCount >0)
	{
		gACInfoPtr->ACIPv6ListInfo.ACIPv6ListCount = joinResponse->ACIPv6ListInfo.ACIPv6ListCount; 
		gACInfoPtr->ACIPv6ListInfo.ACIPv6List = joinResponse->ACIPv6ListInfo.ACIPv6List; 
	}
	if(joinResponse->ImageIdentifier.imagedata_time == 0)
		gCWWaitImageData = 300;
	else
		gCWWaitImageData = joinResponse->ImageIdentifier.imagedata_time;
	CWWTPDebugLog("New gCWWaitImageData:%d", gCWWaitImageData);
	if(joinResponse->ImageIdentifier.NumOfVersion > 0)                                 //pei add 0616
	{
		CW_CREATE_OBJECT_ERR(gImageIdentifier_ACSupported, CWImageIdentifierValues, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
		CW_CREATE_STRING_ERR(gImageIdentifier_ACSupported->values, joinResponse->ImageIdentifier.length, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););

		gImageIdentifier_ACSupported->NumOfVersion = joinResponse->ImageIdentifier.NumOfVersion;
		gImageIdentifier_ACSupported->length = joinResponse->ImageIdentifier.length;
//pei modified 090717
#ifdef AQ1110T
		gImageIdentifier_ACSupported->type = 2000;
#else
		gImageIdentifier_ACSupported->type = 1;
#endif
		memcpy(gImageIdentifier_ACSupported->values, joinResponse->ImageIdentifier.values, gImageIdentifier_ACSupported->length);

		if(memcmp(joinResponse->ImageIdentifier.values, gWTPSoftWareVersion, joinResponse->ImageIdentifier.length) == 0)
			gtemp_state = CW_ENTER_CONFIGURE;
		else
			gtemp_state = CW_ENTER_IMAGE_DATA;
	
		CW_FREE_OBJECT(joinResponse->ImageIdentifier.values);
	
	}
	/* 
         * This field name was allocated for storing the AC name; however, it
         * doesn't seem to be used and it is certainly lost when we exit
         * CWWTPEnterJoin() as joinResponse is actually a local variable of that
         * function.
         *
         * Thus, it seems good to free it now.   
         * 
         * BUG ML03  
         * 16/10/2009 - Donato Capitella
         */
	CW_FREE_OBJECT(joinResponse->ACInfoPtr.name);
        /* BUG ML08 */
	CW_FREE_OBJECT(joinResponse->ACInfoPtr.IPv4Addresses);
	if(joinResponse->ACInfoPtr.IPv6AddressesCount > 0)
		CW_FREE_OBJECT(joinResponse->ACInfoPtr.IPv6Addresses);	
	CWDebugLog("Join Response Saved");	
	return CW_TRUE;
   }
   else {
	   CWDebugLog("Join Response said \"Failure\"");
	   return CW_FALSE;
  }   
}
CWBool CWParseImageIdentifier (CWProtocolMessage *msgPtr, unsigned short int len, CWImageIdentifierValues *valPtr)	                //pei add 0616--------------
{
	CWParseMessageElementStart();
	
	valPtr->NumOfVersion = CWProtocolRetrieve8(msgPtr);
	valPtr->reserved = CWProtocolRetrieve8(msgPtr);
	valPtr->imagedata_time = CWProtocolRetrieve16(msgPtr);
	valPtr->type = CWProtocolRetrieve16(msgPtr);
	valPtr->length = CWProtocolRetrieve16(msgPtr);

	CW_CREATE_STRING_ERR(valPtr->values, valPtr->length, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	valPtr->values = CWProtocolRetrieveStr(msgPtr, valPtr->length);
	
	if(valPtr == NULL) return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL);
	
	CWParseMessageElementEnd();
}
