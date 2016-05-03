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
#if 1
radioInfoValues gRadioInfoValue[2]={{0,0,0,0,5,0,CW_TRUE,0,2346,2346,0,0,100,1,1,CW_TRUE},{1,0,0,0,5,0,CW_TRUE,0,2346,2346,0,0,100,1,1,CW_TRUE}};
modeNParaValues gModeNParaValue[2]={{0,1,15,0,0},{1,1,15,0,0}};//yuan modified for 11n cwmmode 0
unsigned char gCountryCode[4];
#else
unsigned char gchannel = 0;
int gtxpower = 0;
unsigned short grate = 5;         //pei add 0715
CWBool gRateAutoEnable = CW_TRUE;  //pei add 1128
unsigned int gradiotype = 0;    //pei add 0715
unsigned short gFragThreshold = 2346;  //pei add 0722
unsigned short gRtsThreshold = 2346;     //pei add 0729
unsigned char gShortRetry = 0;     //pei add 0729
unsigned char gLongRetry = 0;     //pei add 0729
unsigned short gBeaconInterval = 100;   //pei add 0722
unsigned char gPreamble = 1;                //pei add 0722
unsigned char gDtim = 1;	                   //pei add 0722
unsigned char gIsRadioEnable = 1;	        //pei add 0724
#endif
unsigned char gApScanningEnable = 0;   //pei add 1118
unsigned short gApScanningInterval = 120;   //pei add 1127
//unsigned char gApScanningThreadEnable = 0;   //pei add 1125
unsigned char gAPThroughputInfoCollectEnable = 0; //pei add 0204
unsigned short gAPThroughputInfoInterval = 10;
//unsigned char gAPThroughputInfoThreadEnable = 0; 
Radio_QosValues gRadioQosValues; //pei add 0207
unsigned char gMaxThroughput = 0;  //pei add 0209
unsigned char gQosEnable = 0;  //pei add 0210
WlanMaxThroughputValues gWlanMaxThroughputValue[16];
//void CWWTPResponseTimerExpired(void *arg, CWTimerID id);
CWBool CWAssembleConfigureRequest(CWProtocolMessage **messagesPtr, int *fragmentsNumPtr, int PMTU, int seqNum, CWList msgElemList) ;
CWBool CWParseConfigureResponseMessage (char *msg, int len, int seqNum, CWProtocolConfigureResponseValues *valuesPtr);
CWBool CWSaveConfigureResponseMessage(CWProtocolConfigureResponseValues *configureResponse);

/*_________________________________________________________*/
/*  *******************___FUNCTIONS___*******************  */

// Manage Configure State
CWStateTransition CWWTPEnterConfigure() {
	int seqNum;
	CWProtocolConfigureResponseValues values;
	
	CWDebugLog("\n");
	CWDebugLog("######### Configure State #########");
	CWWTPDebugLog("######### Configure State #########");
	
	/* AC address */
	char str[128];
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

	ConfArg confArg;
	memset(&confArg,0,sizeof(confArg));
	strcpy(confArg.conf_arg,"ac_addr");
	strcpy(confArg.conf_value,str);
	CWSaveConfigInfo(CONF_WTPD,confArg,CW_TRUE);

	int radioCount = CWGetRadioMaxCount();
	if(radioCount == 0)
		radioCount = 1;
	
	memset(&confArg,0,sizeof(confArg));
	strcpy(confArg.conf_arg,"radio_count");
	sprintf(confArg.conf_value,"%d",radioCount);
	CWSaveConfigInfo(CONF_WTPD,confArg,CW_FALSE);

	memset(&confArg,0,sizeof(confArg));
	strcpy(confArg.conf_arg,"gNetworkPreferredFamily");
	sprintf(confArg.conf_value,"%d",gNetworkPreferredFamily);
	CWSaveConfigInfo(CONF_WTPD,confArg,CW_FALSE);
	
	// send Configure Request
	seqNum = CWGetSeqNum();
	
	if(!CWErr(CWWTPSendAcknowledgedPacket(seqNum, NULL, CWAssembleConfigureRequest, (void*) CWParseConfigureResponseMessage, (void*)CWSaveConfigureResponseMessage, &values)))
	{
		//CWTimerDestroy(&responseTimer);
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
		CWWTPDebugLog("Enter CW_ENTER_DISCOVERY ... In Config State Sending or Receiving Error.");
		return CW_ENTER_DISCOVERY;
	}

	int i;
	for(i=0;i<radioCount;i++)
	{
		ConfArg confArg;
		memset(&confArg,0,sizeof(confArg));
		confArg.radioID = i;
		strcpy(confArg.conf_arg,"rateAutoEnable");
		strcpy(confArg.conf_value,gRadioInfoValue[i].rateAutoEnable == CW_TRUE?"1":"0");
		CWSaveConfigInfo(CONF_RADIO,confArg,CW_FALSE);		
	}
	
	return CW_ENTER_DATA_CHECK;
	//CWTimerDestroy(&responseTimer);
}

/*
void CWWTPResponseTimerExpired(void *arg, CWTimerID id) 
{
	CWLog("WTP Response Configure Timer Expired");
	CWNetworkCloseSocket(gWTPSocket);
}
*/

// send Configure Request on the active session
CWBool CWAssembleConfigureRequest(CWProtocolMessage **messagesPtr, int *fragmentsNumPtr, int PMTU, int seqNum, CWList msgElemList) 
{
	CWProtocolMessage *msgElems= NULL;
	const int msgElemCount = 8;  //5;   //yuanlai de    pei 0623
	CWProtocolMessage *msgElemsBinding= NULL;
	const int msgElemBindingCount=0;
	int k = -1;
	
	if(messagesPtr == NULL || fragmentsNumPtr == NULL) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	
	CW_CREATE_PROTOCOL_MSG_ARRAY_ERR(msgElems, msgElemCount, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););	
		
	CWDebugLog("Assembling Configure Request...");
	
	// Assemble Message Elements
	if ((!(CWAssembleMsgElemACName(&(msgElems[++k]))))          ||
	    (!(CWAssembleMsgElemACNameWithIndex(&(msgElems[++k])))) ||
	    (!(CWAssembleMsgElemRadioAdminState(&(msgElems[++k])))) ||
	    (!(CWAssembleMsgElemStatisticsTimer(&(msgElems[++k])))) ||
	    (!(CWAssembleMsgElemWTPRebootStatistics(&(msgElems[++k]))))//)   //yuanlai pei 0623
	    ||(!(CWAssembleMsgElemIEEE80211DirectSequenceControl(&(msgElems[++k]))))    //pei add 0623
	    ||(!(CWAssembleMsgElemIEEE80211TxPower(&(msgElems[++k]))))
	    ||(!(CWAssembleMsgElemIEEE80211WTPRadioInfo(&(msgElems[++k])))))
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
				CW_MSG_TYPE_VALUE_CONFIGURE_REQUEST,
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
	
	CWDebugLog("Configure Request Assembled");
	 
	 return CW_TRUE;
}

CWBool CWParseConfigureResponseMessage (char *msg, int len, int seqNum, CWProtocolConfigureResponseValues *valuesPtr) 
{
	CWControlHeaderValues controlVal;
	int offsetTillMessages;
	CWProtocolMessage completeMsg;
	CWBool bindingMsgElemFound = CW_FALSE;
	int i=0;
	int j=0;
	int k=0;
	int l=0; //pei add 0204
	
	if(msg == NULL || valuesPtr == NULL) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	
	CWDebugLog("Parsing Configure Response...");
	
	completeMsg.msg = msg;
	completeMsg.offset = 0;
	
	valuesPtr->echoRequestTimer=0;
	valuesPtr->radioOperationalInfoCount=0;
	valuesPtr->radiosDecryptErrorPeriod.radiosCount=0;
	valuesPtr->bindingValues = NULL;
	valuesPtr->ACIPv4ListInfo.ACIPv4ListCount=0;
	valuesPtr->ACIPv4ListInfo.ACIPv4List=NULL;
	valuesPtr->ACIPv6ListInfo.ACIPv6ListCount=0;
	valuesPtr->ACIPv6ListInfo.ACIPv6List=NULL;
	valuesPtr->apScanningSetCount = 0;   //pei add 1205
	valuesPtr->apScanningSet = NULL;   //pei add 1205
	valuesPtr->apThroughputSetCount = 0;   //pei add 0204
	valuesPtr->apThroughputSet = NULL;   //pei add 0204
	valuesPtr->maxThroughputSetCount = 0;   //pei add 0204
	valuesPtr->maxThroughputSet = NULL;   //pei add 0204

	if(!(CWParseControlHeader(&completeMsg, &controlVal))) return CW_FALSE; // error will be handled by the caller
	
	// different type
	if(controlVal.messageTypeValue != CW_MSG_TYPE_VALUE_CONFIGURE_RESPONSE)
		return CWErrorRaise(CW_ERROR_INVALID_FORMAT, "Message is not Configure Response as Expected");
	
	if(controlVal.seqNum != seqNum) return CWErrorRaise(CW_ERROR_INVALID_FORMAT, "Different Sequence Number");
	
	controlVal.msgElemsLen -= CW_CONTROL_HEADER_OFFSET_FOR_MSG_ELEMS; // skip timestamp

	offsetTillMessages = completeMsg.offset;	
	
	// parse message elements
	while((completeMsg.offset-offsetTillMessages) < controlVal.msgElemsLen) {
		unsigned short int type=0;// = CWProtocolRetrieve32(&completeMsg);
		unsigned short int len=0;// = CWProtocolRetrieve16(&completeMsg);
		
		CWParseFormatMsgElem(&completeMsg,&type,&len);
//		CWDebugLog("Parsing Message Element: %u, len: %u", type, len);
		
#if 1
		if(CWBindingCheckType(type))
		{
			bindingMsgElemFound=CW_TRUE;
			completeMsg.offset += len;
			continue;	
		}
#endif
		CWDebugLog("Parsing Message Element: %u, len: %u", type, len);
		switch(type) {
			case CW_MSG_ELEMENT_AC_IPV4_LIST_CW_TYPE:
				if(!(CWParseACIPv4List(&completeMsg, len, &(valuesPtr->ACIPv4ListInfo)))) return CW_FALSE;
				break;
			case CW_MSG_ELEMENT_AC_IPV6_LIST_CW_TYPE:
				if(!(CWParseACIPv6List(&completeMsg, len, &(valuesPtr->ACIPv6ListInfo)))) return CW_FALSE;
				break;
			case CW_MSG_ELEMENT_CW_TIMERS_CW_TYPE:
				if(!(CWParseCWTimers(&completeMsg, len, valuesPtr))) return CW_FALSE;
				break;
			case CW_MSG_ELEMENT_RADIO_OPERAT_STATE_CW_TYPE:
				valuesPtr->radioOperationalInfoCount++; // just count how many radios we have, so we can allocate the array
				completeMsg.offset += len;
				break;
			case CW_MSG_ELEMENT_CW_DECRYPT_ER_REPORT_PERIOD_CW_TYPE:
				valuesPtr->radiosDecryptErrorPeriod.radiosCount++;
				completeMsg.offset += len;
				break;
			case CW_MSG_ELEMENT_IDLE_TIMEOUT_CW_TYPE:
				if(!(CWParseIdleTimeout(&completeMsg, len, valuesPtr))) return CW_FALSE;
				break;
			case CW_MSG_ELEMENT_WTP_FALLBACK_CW_TYPE:
				if(!(CWParseWTPFallback(&completeMsg, len, valuesPtr))) return CW_FALSE;
				break;
			case CW_MSG_ELEMENT_VENDOR_SPEC_PAYLOAD_CW_TYPE:{      //pei add 1205
				unsigned char value;
				value = (unsigned char)CWProtocolRetrieve8(&completeMsg);
				CWDebugLog("ConfigState for count the num: value:%d\n", value);
				switch(value){
					case APScanning:
						(completeMsg.offset)-=1;
						(valuesPtr->apScanningSetCount)++; //just count
						completeMsg.offset +=len;
					break;
					case APThroughputInfo:
						(completeMsg.offset)-=1;
						(valuesPtr->apThroughputSetCount)++; //just count
						completeMsg.offset +=len;
					break;
					case APMaxThroughput:
						(completeMsg.offset)-=1;
						(valuesPtr->maxThroughputSetCount)++; //just count
						completeMsg.offset +=len;
					break;
					default:
						(completeMsg.offset)-=1;
						completeMsg.offset +=len;
					break;
					}
				break;
				}
			default:
//				return CWErrorRaise(CW_ERROR_INVALID_FORMAT, "Unrecognized Message Element");
				CWWTPDebugLog("Unrecognized Message Element:%d, in Configure Response", type);
				completeMsg.offset += len;
				break;
		}
	}
	
	if(completeMsg.offset != len) return CWErrorRaise(CW_ERROR_INVALID_FORMAT, "Garbage at the End of the Message");
	
	CW_CREATE_ARRAY_ERR((*valuesPtr).radioOperationalInfo, (*valuesPtr).radioOperationalInfoCount, CWRadioOperationalInfoValues, 
		return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
		
	CW_CREATE_ARRAY_ERR((*valuesPtr).radiosDecryptErrorPeriod.radios, (*valuesPtr).radiosDecryptErrorPeriod.radiosCount, WTPDecryptErrorReportValues, 
		return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););

	CW_CREATE_ARRAY_ERR((*valuesPtr).apScanningSet, (*valuesPtr).apScanningSetCount, ApScanningSetValues, 
		return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););      //pei add 1205

	CW_CREATE_ARRAY_ERR((*valuesPtr).apThroughputSet, (*valuesPtr).apThroughputSetCount, ApThroughputInfoValues, 
		return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););      //pei add 0204

	CW_CREATE_ARRAY_ERR((*valuesPtr).maxThroughputSet, (*valuesPtr).maxThroughputSetCount, ApMaxThroughputValues, 
		return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););      //pei add 0302
	
	completeMsg.offset = offsetTillMessages;
	while(completeMsg.offset-offsetTillMessages < controlVal.msgElemsLen) {
		unsigned short int type=0;// = CWProtocolRetrieve32(&completeMsg);
		unsigned short int len=0;// = CWProtocolRetrieve16(&completeMsg);
		
		CWParseFormatMsgElem(&completeMsg,&type,&len);
		
		switch(type) {
			case CW_MSG_ELEMENT_RADIO_OPERAT_STATE_CW_TYPE:
				if(!(CWParseWTPRadioOperationalState(&completeMsg, len, &(valuesPtr->radioOperationalInfo[i])))) return CW_FALSE; // will be handled by the caller
				i++;
				break;
			
			case CW_MSG_ELEMENT_CW_DECRYPT_ER_REPORT_PERIOD_CW_TYPE:	
				if(!(CWParseDecryptErrorReportPeriod(&completeMsg, len, &(valuesPtr->radiosDecryptErrorPeriod.radios[j])))) return CW_FALSE;
				j++;
				break;
			case CW_MSG_ELEMENT_VENDOR_SPEC_PAYLOAD_CW_TYPE:{      //pei add 1205
				unsigned char value;
				value = (unsigned char)CWProtocolRetrieve8(&completeMsg);
				switch(value){
					case APScanning:
						CWWTPDebugLog("ConfigState get the AP scanning set message:");
						(completeMsg.offset)-=1;
						if(!(CWParseApScanningSet(&completeMsg, len, &(valuesPtr->apScanningSet[k])))) return CW_FALSE;
						k++;
					break;
					case APThroughputInfo:
						CWWTPDebugLog("ConfigState get the collecting AP throughput info message:");
						(completeMsg.offset)-=1;
						if(!(CWParseCollectApThroughputInfoSet(&completeMsg, len, &(valuesPtr->apThroughputSet[l])))) return CW_FALSE;
						l++;
					break;
					case APMaxThroughput:
						CWWTPDebugLog("ConfigState get the max throughput message:");
						(completeMsg.offset)-=1;
						if(!(CWParseMaxThroughput(&completeMsg, len, valuesPtr->maxThroughputSet)))
						{
							CWWTPDebugLog("ConfigState CWParseMaxThroughput error");
							CW_FREE_OBJECT(valuesPtr->maxThroughputSet->wlanMaxThroughput);
							CW_FREE_OBJECT(valuesPtr->maxThroughputSet);
							return CW_FALSE; // will be handled by the caller
						}
					break;
					default:
						(completeMsg.offset)-=1;
						completeMsg.offset += len;
					break;
				}
				break;
			}
			default:
				completeMsg.offset += len;
				break;
		}
	}
	
	if(bindingMsgElemFound){
		if(!CWBindingParseConfigureResponse(msg+offsetTillMessages, len-offsetTillMessages, &(valuesPtr->bindingValues))){return CW_FALSE;}	
	}
	
	CWDebugLog("Configure Response Parsed");
	
	return CW_TRUE;
}

CWBool CWSaveConfigureResponseMessage(CWProtocolConfigureResponseValues *configureResponse)
{
	if(configureResponse == NULL) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	if(gACInfoPtr == NULL) return CWErrorRaise(CW_ERROR_NEED_RESOURCE, NULL);
	
	CWDebugLog("Saving Configure Response...");
	CWDebugLog("###A");
	CWDebugLog("###Count:%d", (configureResponse->ACIPv4ListInfo).ACIPv4ListCount);
	if((gACInfoPtr->ACIPv4ListInfo).ACIPv4List==NULL)
	{CWDebugLog("###NULL");}

	if((configureResponse->ACIPv4ListInfo).ACIPv4ListCount >0)
	{
		CW_FREE_OBJECT((gACInfoPtr->ACIPv4ListInfo).ACIPv4List);
		(gACInfoPtr->ACIPv4ListInfo).ACIPv4ListCount = (configureResponse->ACIPv4ListInfo).ACIPv4ListCount;
		(gACInfoPtr->ACIPv4ListInfo).ACIPv4List = (configureResponse->ACIPv4ListInfo).ACIPv4List;	
	}
	CWDebugLog("###B");
	if((configureResponse->ACIPv6ListInfo).ACIPv6ListCount >0)
	{
		CW_FREE_OBJECT((gACInfoPtr->ACIPv6ListInfo).ACIPv6List);
		(gACInfoPtr->ACIPv6ListInfo).ACIPv6ListCount = (configureResponse->ACIPv6ListInfo).ACIPv6ListCount;
		(gACInfoPtr->ACIPv6ListInfo).ACIPv6List = (configureResponse->ACIPv6ListInfo).ACIPv6List;	
	}
	if(configureResponse->apScanningSetCount > 0)
	{
		gApScanningEnable = configureResponse->apScanningSet->opstate;
		gApScanningInterval = configureResponse->apScanningSet->reportinterval;
		
		ConfArg confArg;
		memset(&confArg,0,sizeof(confArg));
		strcpy(confArg.conf_arg,"gApScanningEnable");
		//strcpy(confArg.conf_value,gApScanningEnable=='0'?"0":"1");
		sprintf(confArg.conf_value,"%d",gApScanningEnable);
		CWSaveConfigInfo(CONF_WTPD,confArg,CW_TRUE);

		memset(&confArg,0,sizeof(confArg));
		strcpy(confArg.conf_arg,"gApScanningInterval");
		sprintf(confArg.conf_value,"%hd",gApScanningInterval);
		CWSaveConfigInfo(CONF_WTPD,confArg,CW_FALSE);
		CWWTPDebugLog("ConfigState:   gApScanningEnable:%d  gApScanningInterval:%d", gApScanningEnable, gApScanningInterval);
		CW_FREE_OBJECT(configureResponse->apScanningSet);
	}
	if(configureResponse->apThroughputSetCount > 0)  //pei add 0204
	{
		gAPThroughputInfoCollectEnable = configureResponse->apThroughputSet->opstate;

		ConfArg confArg;
		memset(&confArg,0,sizeof(confArg));
		strcpy(confArg.conf_arg,"gAPThroughputInfoCollectEnable");
		sprintf(confArg.conf_value,"%d",gAPThroughputInfoCollectEnable);
		CWSaveConfigInfo(CONF_WTPD,confArg,CW_TRUE);
		
		memset(&confArg,0,sizeof(confArg));
		strcpy(confArg.conf_arg,"gAPThroughputInfoInterval");
		sprintf(confArg.conf_value,"%hd",gAPThroughputInfoInterval);
		CWSaveConfigInfo(CONF_WTPD,confArg,CW_FALSE);
		CWWTPDebugLog("ConfigState:   gAPThroughputInfoCollectEnable:%d", gAPThroughputInfoCollectEnable);
		CW_FREE_OBJECT(configureResponse->apThroughputSet);
	}
	if(configureResponse->maxThroughputSetCount > 0)
	{
		int i = 0;
		int wlanId = 0;
		gMaxThroughput = configureResponse->maxThroughputSet->apMaxThroughput;
		CWWTPDebugLog("ConfigState:   gMaxThroughput:%d", gMaxThroughput);
		for(i=0;i<16;i++)
		{
			memset(&gWlanMaxThroughputValue[i], 0, sizeof(WlanMaxThroughputValues));
		}
		for(i=0;i<configureResponse->maxThroughputSet->wlanCount;i++)
		{
			wlanId = configureResponse->maxThroughputSet->wlanMaxThroughput[i].wlanId;
			gWlanMaxThroughputValue[wlanId-1].wlanId = configureResponse->maxThroughputSet->wlanMaxThroughput[i].wlanId;
			gWlanMaxThroughputValue[wlanId-1].maxThroughput = configureResponse->maxThroughputSet->wlanMaxThroughput[i].maxThroughput;
			CWWTPDebugLog("wlanId:%d maxThroughput:%d", gWlanMaxThroughputValue[wlanId-1].wlanId, gWlanMaxThroughputValue[wlanId-1].maxThroughput);
		}
		CW_FREE_OBJECT(configureResponse->maxThroughputSet->wlanMaxThroughput);
		CW_FREE_OBJECT(configureResponse->maxThroughputSet);
	}

	if(configureResponse->echoRequestTimer>0)
	{
		gEchoInterval=configureResponse->echoRequestTimer;
		ConfArg confArg;
		memset(&confArg,0,sizeof(ConfArg));
		strcpy(confArg.conf_arg,"gEchoInterval");
		sprintf(confArg.conf_value,"%d",gEchoInterval);
		CWSaveConfigInfo(CONF_WTPD,confArg,CW_TRUE);
	}
	
	if(configureResponse->bindingValues!=NULL)
	{
		CWProtocolResultCode resultCode;

		if(!CWBindingSaveConfigureResponse(configureResponse->bindingValues, &resultCode))
		{
			CW_FREE_OBJECT(configureResponse->bindingValues);	
			return CW_FALSE;
		}	
	}

	/*
	""need to be added""
	
	int discoveryTimer;
	int radioOperationalInfoCount;
	CWRadioOperationalInfoValues *radioOperationalInfo;
	WTPDecryptErrorReport radiosDecryptErrorPeriod;
	int idleTimeout;
	int fallback;
	*/

 	/*
         * It is not clear to me what the original developers intended to
         * accomplish. One thing's for sure: radioOperationalInfo, radiosDecryptErrorPeriod.radios,
         * and bidingValues get allocated and are never freed, 
         * so we do it here...
         * 
         * BUGs ML02-ML04-ML05
         * 16/10/2009 - Donato Capitella
         */
	CW_FREE_OBJECT(configureResponse->radioOperationalInfo);
	CW_FREE_OBJECT(configureResponse->radiosDecryptErrorPeriod.radios);
	CW_FREE_OBJECT(configureResponse->bindingValues);

	CWDebugLog("Configure Response Saved");
	return CW_TRUE;	
}

