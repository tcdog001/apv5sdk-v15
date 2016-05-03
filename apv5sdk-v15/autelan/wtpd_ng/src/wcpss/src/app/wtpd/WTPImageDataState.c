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

int gCWWaitImageData = 5*CW_JOIN_INTERVAL_DEFAULT;
unsigned char gImageName[50] = "0";
int gtemp_seqNum;
/* static CWBool gImageDataSuccessfulHandshake = CW_TRUE; */
unsigned char *gtemp_values;
CWProtocolResultCode imagedata_code = CW_PROTOCOL_SUCCESS;
/*__________________________________________________________*/
/*  *******************___PROTOTYPES___*******************  */
//void CWWTPWaitImageDateExpired(CWTimerArg arg);
extern CWBool CWParseImageIdentifier (CWProtocolMessage *msgPtr, unsigned short int len, CWImageIdentifierValues *valPtr);       //pei add 0716

/*_____________________________________________________*/
/*  *******************___FUNCTIONS___*******************  */

CWBool CWAssembleImageDataRequest(CWProtocolMessage **messagesPtr, int *fragmentsNumPtr, int PMTU, int seqNum, CWList msgElemList) {
	CWProtocolMessage *msgElems= NULL;
	const int msgElemCount = 1;
	CWProtocolMessage *msgElemsBinding= NULL;
	const int msgElemBindingCount=0;

	
	if(messagesPtr == NULL || fragmentsNumPtr == NULL) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	
	CW_CREATE_PROTOCOL_MSG_ARRAY_ERR(msgElems, msgElemCount, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););	
		
	CWDebugLog("Sending ImageData Request...");
	
	// Assemble Message Elements
	if (!(CWAssembleMsgElemImageIdentifier(msgElems))) {
		CW_FREE_PROTOCOL_MESSAGE(*msgElems);
		CW_FREE_OBJECT(msgElems);
		return CW_FALSE; // error will be handled by the caller
	}
	
	CW_FREE_OBJECT(gImageIdentifier_ACSupported->values);
	CW_FREE_OBJECT(gImageIdentifier_ACSupported);

	return CWAssembleMessage(messagesPtr,
				 fragmentsNumPtr,
				 PMTU,
				 seqNum,
				 CW_MSG_TYPE_VALUE_IMAGE_DATA_REQUEST,
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


// Parse Image Data Request and return informations in *valuesPtr
CWBool CWParseImageDataRequestMessage(char *msg, int len, int seqNum, CWImageIdentifierValues *valuesPtr) {
	CWControlHeaderValues controlVal;
	int offsetTillMessages;
	
	CWProtocolMessage completeMsg;
	
	if(msg == NULL || valuesPtr == NULL){
		CWDebugLog("CWParseImageDataRequestMessage  (msg == NULL || valuesPtr == NULL)\n");
		return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	}
	
	CWDebugLog("Parsing Image Data Request...");
	
	completeMsg.msg = msg;
	completeMsg.offset = 0;
	
	if(!(CWParseControlHeader(&completeMsg, &controlVal))) return CW_FALSE; // error will be handled by the caller
	
	// different type
	if(controlVal.messageTypeValue != CW_MSG_TYPE_VALUE_IMAGE_DATA_REQUEST)
		return CWErrorRaise(CW_ERROR_INVALID_FORMAT, "Message is not Image Data Request as Expected");

	if(controlVal.seqNum != seqNum) return CWErrorRaise(CW_ERROR_INVALID_FORMAT, "Different Sequence Number");
	
	controlVal.msgElemsLen -= CW_CONTROL_HEADER_OFFSET_FOR_MSG_ELEMS; // skip timestamp
	
	offsetTillMessages = completeMsg.offset;


	// parse message elements
	while((completeMsg.offset-offsetTillMessages) < controlVal.msgElemsLen) {
		unsigned short int type=0;
		unsigned short int len=0;
		
		CWParseFormatMsgElem(&completeMsg,&type,&len);		

		CWDebugLog("Parsing Message Element: %u, len: %u", type, len);

		valuesPtr->NumOfVersion = 0;
		valuesPtr->type = 0;
		valuesPtr->length = 0;
		valuesPtr->values= NULL;

		switch(type) {
			case CW_MSG_ELEMENT_IMAGE_DATA_CW_TYPE:
//				if(!(CWParseImageIdentifier(&completeMsg, len, valuesPtr))) return CW_FALSE;
				break;
			case CW_MSG_ELEMENT_IMAGE_IDENTIFIER_CW_TYPE:                         //pei add 0616-----------------------------
				if(!(CWParseImageIdentifier(&completeMsg, len, valuesPtr))) return CW_FALSE;
				break;
			case CW_MSG_ELEMENT_IMAGE_INFO_CW_TYPE:
				
				break;
			default:
//				return CWErrorRaise(CW_ERROR_INVALID_FORMAT, "Unrecognized Message Element");
				CWWTPDebugLog("Unrecognized Message Element:%d, in Image Data Request.", type);
				completeMsg.offset += len;
				break;
		}

		//CWDebugLog("bytes: %d/%d", (completeMsg.offset-offsetTillMessages), controlVal.msgElemsLen);
	}
	
	if(completeMsg.offset != len) return CWErrorRaise(CW_ERROR_INVALID_FORMAT, "Garbage at the End of the Message");
	
	return CW_TRUE;
}

// Parse Reset Request and return informations in *valuesPtr
CWBool CWParseResetRequestMessage(char *msg, int len, int seqNum, CWImageIdentifierValues *valuesPtr) {        //pei add 0617-----------------
	CWControlHeaderValues controlVal;
	int offsetTillMessages;
	
	CWProtocolMessage completeMsg;
	
	if(msg == NULL || valuesPtr == NULL) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	
	CWDebugLog("Parsing Reset Request...");
	
	completeMsg.msg = msg;
	completeMsg.offset = 0;
	
	if(!(CWParseControlHeader(&completeMsg, &controlVal))) return CW_FALSE; // error will be handled by the caller
	
	// different type
	if(controlVal.messageTypeValue != CW_MSG_TYPE_VALUE_RESET_REQUEST)
		return CWErrorRaise(CW_ERROR_INVALID_FORMAT, "Message is not Reset Request as Expected");
	
//	if(controlVal.seqNum != seqNum) return CWErrorRaise(CW_ERROR_INVALID_FORMAT, "Different Sequence Number");
	
	controlVal.msgElemsLen -= CW_CONTROL_HEADER_OFFSET_FOR_MSG_ELEMS; // skip timestamp
	
	offsetTillMessages = completeMsg.offset;


	// parse message elements
	while((completeMsg.offset-offsetTillMessages) < controlVal.msgElemsLen) {
		unsigned short int type=0;
		unsigned short int len=0;
		
		CWParseFormatMsgElem(&completeMsg,&type,&len);		

		CWDebugLog("Parsing Message Element: %u, len: %u", type, len);

		valuesPtr->NumOfVersion = 0;
		valuesPtr->type = 0;
		valuesPtr->length = 0;
		valuesPtr->values = NULL;
	
		switch(type) {
			case CW_MSG_ELEMENT_IMAGE_IDENTIFIER_CW_TYPE:
				if(!(CWParseImageIdentifier(&completeMsg, len, valuesPtr))) return CW_FALSE;
				break;
			case CW_MSG_ELEMENT_VENDOR_SPEC_PAYLOAD_CW_TYPE:
				
				break;
			default:
//				return CWErrorRaise(CW_ERROR_INVALID_FORMAT, "Unrecognized Message Element");
				CWWTPDebugLog("Unrecognized Message Element:%d, in Reset Request after Image Data.", type);
				completeMsg.offset += len;
				break;
		}

		//CWDebugLog("bytes: %d/%d", (completeMsg.offset-offsetTillMessages), controlVal.msgElemsLen);
	}
	
	if(completeMsg.offset != len) return CWErrorRaise(CW_ERROR_INVALID_FORMAT, "Garbage at the End of the Message");
	
	gtemp_seqNum = controlVal.seqNum;            //save seqNum,send to reset state

	return CW_TRUE;
}


CWBool CWSaveImageDataRequestMessage (CWImageIdentifierValues *ImageDateRequest)
{
	char ipstr[128];
	char *AcAddressStr;
	char *ptr;
	if(ImageDateRequest == NULL) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);

	if((ImageDateRequest->NumOfVersion == 1)&&(ImageDateRequest->type == 0))
	{
		CWWTPDebugLog("ImageDateRequest->values = %s",ImageDateRequest->values);
		memcpy(gImageName, ImageDateRequest->values, ImageDateRequest->length);

		CW_FREE_OBJECT(ImageDateRequest->values);
		
		gImageName[ImageDateRequest->length] = '\0';
		
		CWDebugLog("begin to upgrade!!!!\n");
/*		CWUseSockNtop(&(gACInfoPtr->preferredAddress),
			CWLog("Preferred AC: \"%s\", at address: %s", gACInfoPtr->name, str););*/
		AcAddressStr = sock_ntop_r((struct sockaddr *)&(gACInfoPtr->preferredAddress),ipstr);
		
		for(ptr = AcAddressStr; ; ptr++)
		{
			if(*ptr == ':')
			{
				*ptr = '\0';
				break;
			}
		}
		
		if(!CWWTPUpdate(gImageName, (unsigned char *)AcAddressStr)){ //pei 0917 del warning
			CWWTPDebugLog("Firmware write error!");
			CWLog("gImagefile size error! \n");
			imagedata_code = CW_PROTOCOL_FAILURE_FIRM_WRT_ERROR;
		}
	
	}
	
	CWDebugLog("ImageData Request Saved");	
	return CW_TRUE;

 }   

CWBool CWSaveResetRequestMessage (CWImageIdentifierValues *ResetRequest)
{
	if(ResetRequest == NULL) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);

	/* 14/3/2011 Pei Wenhui */
   	if(ResetRequest->values != NULL)
		CW_FREE_OBJECT(ResetRequest->values);
	CWDebugLog("Reset Request Saved");
	return CW_TRUE;
}


CWBool CWAssembleImageDataResponse(CWProtocolMessage **messagesPtr, int *fragmentsNumPtr, int PMTU, int seqNum, CWList msgElemList) {
	CWProtocolMessage *msgElems= NULL;
	const int msgElemCount = 1;
	CWProtocolMessage *msgElemsBinding= NULL;
	const int msgElemBindingCount=0;
	
	if(messagesPtr == NULL || fragmentsNumPtr == NULL) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	
	CW_CREATE_PROTOCOL_MSG_ARRAY_ERR(msgElems, msgElemCount, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););	
		
	CWDebugLog("Sending ImageData Response...");
	
	// Assemble Message Elements
	//if (!(CWAssembleMsgElemResultCode(msgElems, CW_PROTOCOL_SUCCESS))) {
	if(imagedata_code != CW_PROTOCOL_FAILURE_FIRM_WRT_ERROR)
		imagedata_code = CW_PROTOCOL_SUCCESS;
	if (!(CWAssembleMsgElemResultCode(msgElems, imagedata_code))) {
		CW_FREE_PROTOCOL_MESSAGE(*msgElems);
		CW_FREE_OBJECT(msgElems);
		return CW_FALSE; // error will be handled by the caller
	}

	return CWAssembleMessage(messagesPtr,
				 fragmentsNumPtr,
				 PMTU,
				 seqNum,
				 CW_MSG_TYPE_VALUE_IMAGE_DATA_RESPONSE,
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

void CWWTPWaitImageDataExpired(CWTimerArg arg) {
	CWLog("WTP Wait Image Data Expired,reboot");

	dpf("tftp get image-file is timeout,reboot\n");
	_system("sysreboot");
}

// Manage ImageDate State
CWStateTransition CWWTPEnterImageData() {
	CWTimerID waitImageDataTimer;
	int seqNum;
	CWImageIdentifierValues values;
	
	CWDebugLog("\n");
	CWDebugLog("######### ImageDate State #########");
	CWWTPDebugLog("######### ImageDate State #########");
	
	if ((waitImageDataTimer = timer_add(gCWWaitImageData, 0, CWWTPWaitImageDataExpired, NULL)) == -1) {
		return CW_ENTER_DISCOVERY;
	}

	/* Set last connect local IP, and touch file connect_state. */
	CWSetLastConnectLocalIP();
	_system("touch /tmp/connect_state");
	_system("echo 2 > /proc/sys/dev/wifi0/thinap_state");

	// send ImageData Request
	seqNum = CWGetSeqNum();
	if(!CWErr(CWWTPSendAcknowledgedPacket(seqNum, NULL, CWAssembleImageDataRequest, (void*) CWParseImageDataRequestMessage, (void*) CWSaveImageDataRequestMessage, &values)))
	{
		timer_rem(waitImageDataTimer,0);
		//CWNetworkCloseSocket(gWTPSocket);

		CWLog("WTP Enter Reset State ImageDataReq/Resp  reboot... In Image Data State.");
		return CW_ENTER_RESET;   //pei change CW_ENTER_DISCOVERY to CW_ENTER_RESET ath 090223
	}

	// send ImageData Response
	if(!CWErr(CWWTPSendAcknowledgedPacket(seqNum, NULL, CWAssembleImageDataResponse, (void*) CWParseResetRequestMessage, (void*) CWSaveResetRequestMessage, &values)))
	{
		timer_rem(waitImageDataTimer,0);
		//CWNetworkCloseSocket(gWTPSocket);

		CWLog("WTP Enter Reset State ResetReq  reboot... In Image Data State.");
		return CW_ENTER_RESET;          /* pei add for 3ceng update 1205 */
	}

	timer_rem(waitImageDataTimer,0);
#if 0	
	if(!gImageDataSuccessfulHandshake) { // timer expired
		timer_rem(waitImageDataTimer,0);
		//CWNetworkCloseSocket(gWTPSocket);

		CWLog("WTP Enter Reset State timer expired  reboot... In Image Data State.");
		return CW_ENTER_RESET;          /* pei add for 3ceng update 1205 */
	}
#endif
	
	CWLog("ImageData Completed");
//	CWLog("update OK! reboot!");
	CWWTPDebugLog("ImageData Completed!");
	return CW_ENTER_RESET;
}



