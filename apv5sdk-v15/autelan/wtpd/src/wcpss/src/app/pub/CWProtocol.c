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


#include "CWCommon.h"

#ifdef DMALLOC
#include "../dmalloc-5.5.0/dmalloc.h"
#endif
 
static const int gCWIANATimes256 = CW_CAPWAP_NUMBER * 256;
static const int gMaxDTLSHeaderSize = 25; // see http://crypto.stanford.edu/~nagendra/papers/dtls.pdf
static const int gMaxCAPWAPHeaderSize = 8; // note: this include optional Wireless field
#ifndef CW_NO_DTLS
extern unsigned char gDtlsPolicy;  /* when gDtlsSecurity 0, meaningless. when gDtlsSecurity 1, 0 enable control DTLS disable data, 1 enable both */
#endif
// stores 8 bits in the message, increments the current offset in bytes
void CWProtocolStore8(CWProtocolMessage *msgPtr, unsigned char val) {
	CW_COPY_MEMORY(&((msgPtr->msg)[(msgPtr->offset)]), &(val), 1);
	(msgPtr->offset) += 1;
}

// stores 16 bits in the message, increments the current offset in bytes
void CWProtocolStore16(CWProtocolMessage *msgPtr, unsigned short val) {
	val = autelan_htons(val);
	CW_COPY_MEMORY(&((msgPtr->msg)[(msgPtr->offset)]), &(val), 2);
	(msgPtr->offset) += 2;
}

// stores 32 bits in the message, increments the current offset in bytes
void CWProtocolStore32(CWProtocolMessage *msgPtr, unsigned int val) {
	val = autelan_htonl(val);
	CW_COPY_MEMORY(&((msgPtr->msg)[(msgPtr->offset)]), &(val), 4);
	(msgPtr->offset) += 4;
}
void CWProtocolStore64(CWProtocolMessage *msgPtr, unsigned long long val) {
	unsigned char tmp;
	int i;
//	printf("net  transmit 64bit data: %llu   0x%llx \n",val,val);
//	printf("after shift:");
	for(i=7;i>=0;i--)
		{
		tmp = (val>>(8*i))&0xff;
//		printf("%02x", tmp);
		CW_COPY_MEMORY(&((msgPtr->msg)[(msgPtr->offset)]), &(tmp), 1);
		(msgPtr->offset) += 1;
		}
//	printf("  \n");
}

// stores a string in the message, increments the current offset in bytes. Doesn't store
// the '\0' final character.
void CWProtocolStoreStr(CWProtocolMessage *msgPtr, char *str) {
	int len = strlen(str);
	CW_COPY_MEMORY(&((msgPtr->msg)[(msgPtr->offset)]), str, len);
	(msgPtr->offset) += len;
}

// stores another message in the message, increments the current offset in bytes.
void CWProtocolStoreMessage(CWProtocolMessage *msgPtr, CWProtocolMessage *msgToStorePtr) {
	CW_COPY_MEMORY(&((msgPtr->msg)[(msgPtr->offset)]), msgToStorePtr->msg, msgToStorePtr->offset);
	(msgPtr->offset) += msgToStorePtr->offset;
}

// stores len bytes in the message, increments the current offset in bytes.
void CWProtocolStoreRawBytes(CWProtocolMessage *msgPtr, char *bytes, int len) {
	CW_COPY_MEMORY(&((msgPtr->msg)[(msgPtr->offset)]), bytes, len);
	(msgPtr->offset) += len;
}

// retrieves 8 bits from the message, increments the current offset in bytes.
unsigned char CWProtocolRetrieve8(CWProtocolMessage *msgPtr) {
	unsigned char val;
	
	CW_COPY_MEMORY(&val, &((msgPtr->msg)[(msgPtr->offset)]), 1);
	(msgPtr->offset) += 1;
	
	return val;
}

// retrieves 16 bits from the message, increments the current offset in bytes.
unsigned short CWProtocolRetrieve16(CWProtocolMessage *msgPtr) {
	unsigned short val;
	
	CW_COPY_MEMORY(&val, &((msgPtr->msg)[(msgPtr->offset)]), 2);
	(msgPtr->offset) += 2;
	
	return autelan_ntohs(val);
}

// retrieves 32 bits from the message, increments the current offset in bytes.
unsigned int CWProtocolRetrieve32(CWProtocolMessage *msgPtr) {
	unsigned int val;
	
	CW_COPY_MEMORY(&val, &((msgPtr->msg)[(msgPtr->offset)]), 4);
	(msgPtr->offset) += 4;
	
	return autelan_ntohl(val);
}

// retrieves a string (not null-terminated) from the message, increments the current offset in bytes.
// Adds the '\0' char at the end of the string which is returned
char *CWProtocolRetrieveStr(CWProtocolMessage *msgPtr, int len) {
	char *str;
	
	CW_CREATE_OBJECT_SIZE_ERR(str, (len+1), return NULL;);
	
	CW_COPY_MEMORY(str, &((msgPtr->msg)[(msgPtr->offset)]), len);
	str[len] = '\0';
	(msgPtr->offset) += len;
	
	return str;
}

// retrieves len bytes from the message, increments the current offset in bytes.
char *CWProtocolRetrieveRawBytes(CWProtocolMessage *msgPtr, int len) {
	char *bytes;
	
	CW_CREATE_OBJECT_SIZE_ERR(bytes, len, return NULL;);
	
	CW_COPY_MEMORY(bytes, &((msgPtr->msg)[(msgPtr->offset)]), len);
	(msgPtr->offset) += len;
	
	return bytes;
}

void CWProtocolDestroyMsgElemData(void *f) {
	CW_FREE_OBJECT(f);
}

// Assemble a Message Element creating the appropriate header and storing the message.
CWBool CWAssembleMsgElem(CWProtocolMessage *msgPtr, unsigned int type) {
	CWProtocolMessage completeMsg;
	
	if(msgPtr == NULL) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	
	CW_CREATE_PROTOCOL_MESSAGE(completeMsg, 6+(msgPtr->offset), return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););

	// store header
	CWProtocolStore16(&completeMsg, type);
	CWProtocolStore16(&completeMsg, msgPtr->offset); // size of the body
	
	// store body
	CWProtocolStoreMessage(&completeMsg, msgPtr);

	CW_FREE_PROTOCOL_MESSAGE(*msgPtr);

	msgPtr->msg = completeMsg.msg;
	msgPtr->offset = completeMsg.offset;

	return CW_TRUE;
}

CWBool CWAssembleTransportHeader2(CWProtocolMessage *transportHdrPtr, CWProtocolTransportHeaderValues *valuesPtr,unsigned char radioId) {
	unsigned int val = 0;
	if(transportHdrPtr == NULL || valuesPtr == NULL) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);

	int capwap_header_len = 8;
	if(valuesPtr->bindingValuesPtr !=NULL){
		if(valuesPtr->bindingValuesPtr->flag_w == 1)
			capwap_header_len += 8;
		if(valuesPtr->bindingValuesPtr->flag_m == 1)
			capwap_header_len += 8;
	}
	
	CW_CREATE_PROTOCOL_MESSAGE(*transportHdrPtr,capwap_header_len, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	
	CWSetField32(val, 
		     CW_TRANSPORT_HEADER_VERSION_START,
		     CW_TRANSPORT_HEADER_VERSION_LEN,
		     CW_PROTOCOL_VERSION); // current version of CAPWAP

	CWSetField32(val,
		     CW_TRANSPORT_HEADER_TYPE_START,
		     CW_TRANSPORT_HEADER_TYPE_LEN,
		     (valuesPtr->payloadType == CW_PACKET_PLAIN) ? 0 : 1);
	
	CWSetField32(val,
			 CW_TRANSPORT_HEADER_HLEN_START,
			 CW_TRANSPORT_HEADER_HLEN_LEN,
			 capwap_header_len/4);


	CWSetField32(val,
		     CW_TRANSPORT_HEADER_RID_START,
		     CW_TRANSPORT_HEADER_RID_LEN,
		     radioId); // only one radio per WTP?

	if((valuesPtr->bindingValuesPtr != NULL)&&(valuesPtr->bindingValuesPtr->type == 1))  //802.11
	{
	CWSetField32(val,
		     CW_TRANSPORT_HEADER_WBID_START,
		     CW_TRANSPORT_HEADER_WBID_LEN,
		     1); // Wireless Binding ID
	}
	else   //802.3
	{
		CWSetField32(val,
			     CW_TRANSPORT_HEADER_WBID_START,
			     CW_TRANSPORT_HEADER_WBID_LEN,
			     0); // Wireless Binding ID
	}
	if((valuesPtr->bindingValuesPtr != NULL)&&(valuesPtr->bindingValuesPtr->type == 1))  //802.11
	CWSetField32(val,
		     CW_TRANSPORT_HEADER_T_START,
		     CW_TRANSPORT_HEADER_T_LEN,
			     1); // Format of the frame. 1-802.11
	else   //802.3
		CWSetField32(val,
			     CW_TRANSPORT_HEADER_T_START,
			     CW_TRANSPORT_HEADER_T_LEN,
			     0); // Format of the frame. 0-802.3

	CWSetField32(val,
		     CW_TRANSPORT_HEADER_F_START,
		     CW_TRANSPORT_HEADER_F_LEN,
		     valuesPtr->isFragment); // is fragment

	CWSetField32(val,
		     CW_TRANSPORT_HEADER_L_START,
		     CW_TRANSPORT_HEADER_L_LEN,
		     valuesPtr->last); // last fragment
	
//	if(valuesPtr->bindingValuesPtr != NULL)
//		if((valuesPtr->bindingValuesPtr != NULL)&&(valuesPtr->bindingValuesPtr->type == 1))  //802.11
	if((valuesPtr->bindingValuesPtr != NULL)&&(valuesPtr->bindingValuesPtr->flag_w == 1))  
		CWSetField32(val,
			     CW_TRANSPORT_HEADER_W_START,
			     CW_TRANSPORT_HEADER_W_LEN,
			     1); //wireless header
	else 
		CWSetField32(val,
			     CW_TRANSPORT_HEADER_W_START,
			     CW_TRANSPORT_HEADER_W_LEN,
			     0);
//		if((valuesPtr->bindingValuesPtr != NULL)&&(valuesPtr->bindingValuesPtr->type == 0))  //802.3
	if((valuesPtr->bindingValuesPtr != NULL)&&(valuesPtr->bindingValuesPtr->flag_m == 1)) 
		CWSetField32(val,
			     CW_TRANSPORT_HEADER_M_START,
			     CW_TRANSPORT_HEADER_M_LEN,
			     1); // no radio MAC address
	else   //802.11
		CWSetField32(val,
			     CW_TRANSPORT_HEADER_M_START,
			     CW_TRANSPORT_HEADER_M_LEN,
			     0); // no radio MAC address

	CWSetField32(val,
		     CW_TRANSPORT_HEADER_K_START,
		     CW_TRANSPORT_HEADER_K_LEN,
		     0); // Keep alive flag

	CWSetField32(val,
		     CW_TRANSPORT_HEADER_FLAGS_START,
		     CW_TRANSPORT_HEADER_FLAGS_LEN,
		     0); // required

	CWProtocolStore32(transportHdrPtr, val);
	// end of first 32 bits
	
	val = 0;

	CWSetField32(val,
		     CW_TRANSPORT_HEADER_FRAGMENT_ID_START,
		     CW_TRANSPORT_HEADER_FRAGMENT_ID_LEN,
		     valuesPtr->fragmentID); // fragment ID
	
	CWSetField32(val,
		     CW_TRANSPORT_HEADER_FRAGMENT_OFFSET_START,
		     CW_TRANSPORT_HEADER_FRAGMENT_OFFSET_LEN,
		     valuesPtr->fragmentOffset); // fragment offset

	CWSetField32(val,
		     CW_TRANSPORT_HEADER_RESERVED_START,
		     CW_TRANSPORT_HEADER_RESERVED_LEN,
		     0); // required

	CWProtocolStore32(transportHdrPtr, val);
	// end of second 32 bits

	if(valuesPtr->bindingValuesPtr != NULL){
		if (!CWAssembleTransportHeaderBinding(transportHdrPtr, valuesPtr->bindingValuesPtr))
			return CW_FALSE;
	}

	return CW_TRUE;
}


CWBool CWAssembleTransportHeader(CWProtocolMessage *transportHdrPtr, CWProtocolTransportHeaderValues *valuesPtr) {
	unsigned int val = 0;
	if(transportHdrPtr == NULL || valuesPtr == NULL) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	
	if(valuesPtr->bindingValuesPtr != NULL)
		{CW_CREATE_PROTOCOL_MESSAGE(*transportHdrPtr,gMaxCAPWAPHeaderSizeBinding, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););}
	else {CW_CREATE_PROTOCOL_MESSAGE(*transportHdrPtr,8 , return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););}	 // meaningful bytes of the header (no wirless header and MAC address)
	CWSetField32(val, 
		     CW_TRANSPORT_HEADER_VERSION_START,
		     CW_TRANSPORT_HEADER_VERSION_LEN,
		     CW_PROTOCOL_VERSION); // current version of CAPWAP

	CWSetField32(val,
		     CW_TRANSPORT_HEADER_TYPE_START,
		     CW_TRANSPORT_HEADER_TYPE_LEN,
		     (valuesPtr->payloadType == CW_PACKET_PLAIN) ? 0 : 1);
	
	if(valuesPtr->bindingValuesPtr != NULL)
		CWSetField32(val,
			     CW_TRANSPORT_HEADER_HLEN_START,
			     CW_TRANSPORT_HEADER_HLEN_LEN,
			     CW_BINDING_HLEN);
	else 
		CWSetField32(val,
			     CW_TRANSPORT_HEADER_HLEN_START,
			     CW_TRANSPORT_HEADER_HLEN_LEN,
			     2);

	CWSetField32(val,
		     CW_TRANSPORT_HEADER_RID_START,
		     CW_TRANSPORT_HEADER_RID_LEN,
		     0); // only one radio per WTP?
	
	CWSetField32(val,
		     CW_TRANSPORT_HEADER_WBID_START,
		     CW_TRANSPORT_HEADER_WBID_LEN,
		     1); // Wireless Binding ID
	
	CWSetField32(val,
		     CW_TRANSPORT_HEADER_T_START,
		     CW_TRANSPORT_HEADER_T_LEN,
		     1);

	CWSetField32(val,
		     CW_TRANSPORT_HEADER_F_START,
		     CW_TRANSPORT_HEADER_F_LEN,
		     valuesPtr->isFragment); // is fragment

	CWSetField32(val,
		     CW_TRANSPORT_HEADER_L_START,
		     CW_TRANSPORT_HEADER_L_LEN,
		     valuesPtr->last); // last fragment
	
	if(valuesPtr->bindingValuesPtr != NULL)
		CWSetField32(val,
			     CW_TRANSPORT_HEADER_W_START,
			     CW_TRANSPORT_HEADER_W_LEN,
			     1); //wireless header
	else 
		CWSetField32(val,
			     CW_TRANSPORT_HEADER_W_START,
			     CW_TRANSPORT_HEADER_W_LEN,
			     0);

	CWSetField32(val,
		     CW_TRANSPORT_HEADER_M_START,
		     CW_TRANSPORT_HEADER_M_LEN,
		     0); // no radio MAC address

	CWSetField32(val,
		     CW_TRANSPORT_HEADER_K_START,
		     CW_TRANSPORT_HEADER_K_LEN,
		     0); // Keep alive flag

	CWSetField32(val,
		     CW_TRANSPORT_HEADER_FLAGS_START,
		     CW_TRANSPORT_HEADER_FLAGS_LEN,
		     0); // required

	CWProtocolStore32(transportHdrPtr, val);
	// end of first 32 bits
	
	val = 0;

	CWSetField32(val,
		     CW_TRANSPORT_HEADER_FRAGMENT_ID_START,
		     CW_TRANSPORT_HEADER_FRAGMENT_ID_LEN,
		     valuesPtr->fragmentID); // fragment ID
	
	CWSetField32(val,
		     CW_TRANSPORT_HEADER_FRAGMENT_OFFSET_START,
		     CW_TRANSPORT_HEADER_FRAGMENT_OFFSET_LEN,
		     valuesPtr->fragmentOffset); // fragment offset

	CWSetField32(val,
		     CW_TRANSPORT_HEADER_RESERVED_START,
		     CW_TRANSPORT_HEADER_RESERVED_LEN,
		     0); // required

	CWProtocolStore32(transportHdrPtr, val);
	// end of second 32 bits

	if(valuesPtr->bindingValuesPtr != NULL){
		if (!CWAssembleTransportHeaderBinding(transportHdrPtr, valuesPtr->bindingValuesPtr))
			return CW_FALSE;
	}

	return CW_TRUE;
}

// Assembles the Transport Header  /* add for 2 radio rrm, pei add at 20100911 */
CWBool CWAssembleTransportHeaderCheckRadio(CWProtocolMessage *transportHdrPtr, CWProtocolTransportHeaderValues *valuesPtr, unsigned char radioId) {
	unsigned int val = 0;
	if(transportHdrPtr == NULL || valuesPtr == NULL) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	
	if(valuesPtr->bindingValuesPtr != NULL)
		{CW_CREATE_PROTOCOL_MESSAGE(*transportHdrPtr,gMaxCAPWAPHeaderSizeBinding, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););}
	else {CW_CREATE_PROTOCOL_MESSAGE(*transportHdrPtr,8 , return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););}	 // meaningful bytes of the header (no wirless header and MAC address)
	CWSetField32(val, 
		     CW_TRANSPORT_HEADER_VERSION_START,
		     CW_TRANSPORT_HEADER_VERSION_LEN,
		     CW_PROTOCOL_VERSION); // current version of CAPWAP

	CWSetField32(val,
		     CW_TRANSPORT_HEADER_TYPE_START,
		     CW_TRANSPORT_HEADER_TYPE_LEN,
		     (valuesPtr->payloadType == CW_PACKET_PLAIN) ? 0 : 1);
	
	if(valuesPtr->bindingValuesPtr != NULL)
		CWSetField32(val,
			     CW_TRANSPORT_HEADER_HLEN_START,
			     CW_TRANSPORT_HEADER_HLEN_LEN,
			     CW_BINDING_HLEN);
	else 
		CWSetField32(val,
			     CW_TRANSPORT_HEADER_HLEN_START,
			     CW_TRANSPORT_HEADER_HLEN_LEN,
			     2);

	CWSetField32(val,
		     CW_TRANSPORT_HEADER_RID_START,
		     CW_TRANSPORT_HEADER_RID_LEN,
		     radioId); // only one radio per WTP?
	
	CWSetField32(val,
		     CW_TRANSPORT_HEADER_WBID_START,
		     CW_TRANSPORT_HEADER_WBID_LEN,
		     1); // Wireless Binding ID
	
	CWSetField32(val,
		     CW_TRANSPORT_HEADER_T_START,
		     CW_TRANSPORT_HEADER_T_LEN,
		     1);

	CWSetField32(val,
		     CW_TRANSPORT_HEADER_F_START,
		     CW_TRANSPORT_HEADER_F_LEN,
		     valuesPtr->isFragment); // is fragment

	CWSetField32(val,
		     CW_TRANSPORT_HEADER_L_START,
		     CW_TRANSPORT_HEADER_L_LEN,
		     valuesPtr->last); // last fragment
	
	if(valuesPtr->bindingValuesPtr != NULL)
		CWSetField32(val,
			     CW_TRANSPORT_HEADER_W_START,
			     CW_TRANSPORT_HEADER_W_LEN,
			     1); //wireless header
	else 
		CWSetField32(val,
			     CW_TRANSPORT_HEADER_W_START,
			     CW_TRANSPORT_HEADER_W_LEN,
			     0);

	CWSetField32(val,
		     CW_TRANSPORT_HEADER_M_START,
		     CW_TRANSPORT_HEADER_M_LEN,
		     0); // no radio MAC address

	CWSetField32(val,
		     CW_TRANSPORT_HEADER_K_START,
		     CW_TRANSPORT_HEADER_K_LEN,
		     0); // Keep alive flag

	CWSetField32(val,
		     CW_TRANSPORT_HEADER_FLAGS_START,
		     CW_TRANSPORT_HEADER_FLAGS_LEN,
		     0); // required

	CWProtocolStore32(transportHdrPtr, val);
	// end of first 32 bits
	
	val = 0;

	CWSetField32(val,
		     CW_TRANSPORT_HEADER_FRAGMENT_ID_START,
		     CW_TRANSPORT_HEADER_FRAGMENT_ID_LEN,
		     valuesPtr->fragmentID); // fragment ID
	
	CWSetField32(val,
		     CW_TRANSPORT_HEADER_FRAGMENT_OFFSET_START,
		     CW_TRANSPORT_HEADER_FRAGMENT_OFFSET_LEN,
		     valuesPtr->fragmentOffset); // fragment offset

	CWSetField32(val,
		     CW_TRANSPORT_HEADER_RESERVED_START,
		     CW_TRANSPORT_HEADER_RESERVED_LEN,
		     0); // required

	CWProtocolStore32(transportHdrPtr, val);
	// end of second 32 bits

	if(valuesPtr->bindingValuesPtr != NULL){
		if (!CWAssembleTransportHeaderBinding(transportHdrPtr, valuesPtr->bindingValuesPtr))
			return CW_FALSE;
	}

	return CW_TRUE;
}

// Assembles the Control Header
CWBool CWAssembleControlHeader(CWProtocolMessage *controlHdrPtr, CWControlHeaderValues *valPtr) {
	if(controlHdrPtr == NULL || valPtr == NULL) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	
	CW_CREATE_PROTOCOL_MESSAGE(*controlHdrPtr, 8,	 // meaningful bytes of the header
						return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	
	CWProtocolStore32(controlHdrPtr, valPtr->messageTypeValue); //  Message Type Value
	CWProtocolStore8(controlHdrPtr, valPtr->seqNum);
	CWProtocolStore16(controlHdrPtr, (CW_CONTROL_HEADER_OFFSET_FOR_MSG_ELEMS+(valPtr->msgElemsLen))); // 7 is for the next 8+32+16 bits (= 7 bytes), MessageElementsLength+flags + timestamp 
	CWProtocolStore8(controlHdrPtr, 0); // flags
	//CWProtocolStore32(controlHdrPtr, ((unsigned int)(time(NULL))) ); // timestamp
	
	return CW_TRUE;
}
CWBool CWAssembleMsgElemResultCode(CWProtocolMessage *msgPtr, CWProtocolResultCode code) {
	if(msgPtr == NULL) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	
	// create message
	CW_CREATE_PROTOCOL_MESSAGE(*msgPtr, 4, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	
	CWProtocolStore32(msgPtr, code);
				
	return CWAssembleMsgElem(msgPtr, CW_MSG_ELEMENT_RESULT_CODE_CW_TYPE);
}

// Assemble a CAPWAP Control Packet, with the given Message Elements, Sequence Number and Message Type. Create Transport and Control Headers.
// completeMsgPtr is an array of fragments (can be of size 1 if the packet doesn't need fragmentation
CWBool CWAssembleMessage(CWProtocolMessage **completeMsgPtr, int *fragmentsNumPtr, int PMTU, int seqNum, int msgTypeValue, CWProtocolMessage *msgElems, const int msgElemNum, CWProtocolMessage *msgElemsBinding, const int msgElemBindingNum, int is_crypted) {
	CWProtocolMessage transportHdr, controlHdr, msg;
	int msgElemsLen = 0;
	int i;
	
	CWProtocolTransportHeaderValues transportVal;
	CWControlHeaderValues controlVal;
	
	if(completeMsgPtr == NULL || fragmentsNumPtr == NULL || (msgElems == NULL && msgElemNum > 0) || (msgElemsBinding == NULL && msgElemBindingNum > 0)) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	
	//Calculate the whole size of the Msg Elements	
	for(i = 0; i < msgElemNum; i++) msgElemsLen += msgElems[i].offset;
	for(i = 0; i < msgElemBindingNum; i++) msgElemsLen += msgElemsBinding[i].offset;

	//Assemble Control Header
	controlVal.messageTypeValue = msgTypeValue;
	controlVal.msgElemsLen = msgElemsLen;
	controlVal.seqNum = seqNum;
	
	if(!(CWAssembleControlHeader(&controlHdr, &controlVal))) {
		CW_FREE_PROTOCOL_MESSAGE(controlHdr);
		for(i = 0; i < msgElemNum; i++) { CW_FREE_PROTOCOL_MESSAGE(msgElems[i]);}
		CW_FREE_OBJECT(msgElems);
		for(i = 0; i < msgElemBindingNum; i++) { CW_FREE_PROTOCOL_MESSAGE(msgElemsBinding[i]);}
		CW_FREE_OBJECT(msgElemsBinding);
		return CW_FALSE; // will be handled by the caller
	}
	
	// assemble the message putting all the data consecutively
	CW_CREATE_PROTOCOL_MESSAGE(msg, controlHdr.offset + msgElemsLen, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	
	CWProtocolStoreMessage(&msg, &controlHdr);
	for(i = 0; i < msgElemNum; i++) { // store in the request all the Message Elements
		CWProtocolStoreMessage(&msg, &(msgElems[i]));
	}
	for(i = 0; i < msgElemBindingNum; i++) { // store in the request all the Message Elements
		CWProtocolStoreMessage(&msg, &(msgElemsBinding[i]));
	}

	//Free memory not needed anymore
	CW_FREE_PROTOCOL_MESSAGE(controlHdr);
	for(i = 0; i < msgElemNum; i++) { CW_FREE_PROTOCOL_MESSAGE(msgElems[i]);}
	CW_FREE_OBJECT(msgElems);
	for(i = 0; i < msgElemBindingNum; i++) { CW_FREE_PROTOCOL_MESSAGE(msgElemsBinding[i]);}
	CW_FREE_OBJECT(msgElemsBinding);
	
	
	// handle fragmentation
	PMTU = PMTU - gMaxDTLSHeaderSize - gMaxCAPWAPHeaderSize;
	
	if(PMTU > 0) {
		PMTU = (PMTU/8)*8; // CAPWAP fragments are made of groups of 8 bytes
		if(PMTU == 0) goto cw_dont_fragment;
		
		*fragmentsNumPtr = msg.offset / PMTU;
		if((msg.offset % PMTU) != 0) (*fragmentsNumPtr)++;
	} else {
	cw_dont_fragment:
		*fragmentsNumPtr = 1;
	}
	
	transportVal.bindingValuesPtr = NULL;
		
	if(*fragmentsNumPtr == 1) {

		CW_CREATE_OBJECT_ERR(*completeMsgPtr, CWProtocolMessage, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
		
		transportVal.isFragment = transportVal.last = transportVal.fragmentOffset = transportVal.fragmentID = 0;
		transportVal.payloadType = is_crypted;
//		transportVal.last = 1;

		// Assemble Message Elements
		if	(!(CWAssembleTransportHeader(&transportHdr, &transportVal))) {
			CW_FREE_PROTOCOL_MESSAGE(msg);
			CW_FREE_PROTOCOL_MESSAGE(transportHdr);
			CW_FREE_OBJECT(*completeMsgPtr);//zengmin add by Coverity Bad free 2013-06-08
			return CW_FALSE; // will be handled by the caller
		} 
	
		// assemble the message putting all the data consecutively
		CW_CREATE_PROTOCOL_MESSAGE(((*completeMsgPtr)[0]), transportHdr.offset + msg.offset, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
		
		CWProtocolStoreMessage(&((*completeMsgPtr)[0]), &transportHdr);
		CWProtocolStoreMessage(&((*completeMsgPtr)[0]), &msg);
		
		CW_FREE_PROTOCOL_MESSAGE(transportHdr);
		CW_FREE_PROTOCOL_MESSAGE(msg);
	} else {
		int fragID = CWGetFragmentID();
		int totalSize = msg.offset;

		CW_CREATE_PROTOCOL_MSG_ARRAY_ERR(*completeMsgPtr, *fragmentsNumPtr, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
		msg.offset = 0;
	
		for(i = 0; i < *fragmentsNumPtr; i++) { // for each fragment to assemble
			int fragSize;
			
			transportVal.isFragment = 1;
			transportVal.fragmentOffset = msg.offset/8;
			transportVal.fragmentID = fragID;
			transportVal.payloadType = is_crypted;

			if(i < ((*fragmentsNumPtr)-1)) { // not last fragment
				fragSize = PMTU;
				transportVal.last = 0;
			} else { // last fragment
				fragSize = totalSize - (((*fragmentsNumPtr)-1) * PMTU);
				transportVal.last = 1;
			}

			
			// Assemble Transport Header for this fragment
			if(!(CWAssembleTransportHeader(&transportHdr, &transportVal))) {
				CW_FREE_PROTOCOL_MESSAGE(msg);
				CW_FREE_PROTOCOL_MESSAGE(transportHdr);
				CW_FREE_OBJECT(*completeMsgPtr);//zengmin add by Coverity Bad free 2013-06-08
				return CW_FALSE; // will be handled by the caller
			}

			CW_CREATE_PROTOCOL_MESSAGE(((*completeMsgPtr)[i]), transportHdr.offset + fragSize, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
			
			CWProtocolStoreMessage(&((*completeMsgPtr)[i]), &transportHdr);
			CWProtocolStoreRawBytes(&((*completeMsgPtr)[i]), &((msg.msg)[msg.offset]), fragSize);
			msg.offset += fragSize;
			
			CW_FREE_PROTOCOL_MESSAGE(transportHdr);
		}
		CW_FREE_PROTOCOL_MESSAGE(msg);
	}
	
	return CW_TRUE;
}

/*
*	diaowq capsulate packet like the following @20120416:
*
*	capwap header (16bytes) + 802.11 header (24bytes) + LLC (8bytes) + 802.11 data (here is capwap msg elems=vsp)
*
*	details:
*		capwap header = transfer header(8bytes) + wirelesss special info(8bytes)	/////note:no control header here
*		802.11 header = manage bytes(subtype=6) & ToDs=1,include sta_mac and bssid
*		LLC = all zero
*		802.11 data = capwap msg elems(verdor special payload)
*
*/
CWBool CWAssembleMessageV2(CWProtocolMessage **completeMsgPtr, int *fragmentsNumPtr, int PMTU, int seqNum, int msgTypeValue, CWProtocolMessage *msgElems, const int msgElemNum, CWProtocolMessage *msgElemsBinding, const int msgElemBindingNum, int is_crypted,char* wh,int str_len) {
	CWProtocolMessage transportHdr, controlHdr, msg;
	int msgElemsLen = 0;
	int i;
	
	CWProtocolTransportHeaderValues transportVal;
	CWControlHeaderValues controlVal;
	
	if(completeMsgPtr == NULL || fragmentsNumPtr == NULL || (msgElems == NULL && msgElemNum > 0) || (msgElemsBinding == NULL && msgElemBindingNum > 0)) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	
	//Calculate the whole size of the Msg Elements	
	for(i = 0; i < msgElemNum; i++) msgElemsLen += msgElems[i].offset;
	for(i = 0; i < msgElemBindingNum; i++) msgElemsLen += msgElemsBinding[i].offset;

	//Assemble Control Header
	controlVal.messageTypeValue = msgTypeValue;
	controlVal.msgElemsLen = msgElemsLen;
	controlVal.seqNum = seqNum;
	
	if(!(CWAssembleControlHeader(&controlHdr, &controlVal))) {
		CW_FREE_PROTOCOL_MESSAGE(controlHdr);
		for(i = 0; i < msgElemNum; i++) { CW_FREE_PROTOCOL_MESSAGE(msgElems[i]);}
		CW_FREE_OBJECT(msgElems);
		for(i = 0; i < msgElemBindingNum; i++) { CW_FREE_PROTOCOL_MESSAGE(msgElemsBinding[i]);}
		CW_FREE_OBJECT(msgElemsBinding);
		return CW_FALSE; // will be handled by the caller
	}
	
	// assemble the message putting all the data consecutively
	CW_CREATE_PROTOCOL_MESSAGE(msg, /*controlHdr.offset + */str_len  + msgElemsLen, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););

//	CWProtocolStoreMessage(&msg, &controlHdr);		//diaowq del control header in data tunnel @20120413
	CWProtocolStoreRawBytes(&msg,wh,str_len);						//diaowq add dot11 header + LLC in data tunnel @20120413
	for(i = 0; i < msgElemNum; i++) { // store in the request all the Message Elements
		CWProtocolStoreMessage(&msg, &(msgElems[i]));
	}
	for(i = 0; i < msgElemBindingNum; i++) { // store in the request all the Message Elements
		CWProtocolStoreMessage(&msg, &(msgElemsBinding[i]));
	}

	//Free memory not needed anymore
	CW_FREE_PROTOCOL_MESSAGE(controlHdr);
	for(i = 0; i < msgElemNum; i++) { CW_FREE_PROTOCOL_MESSAGE(msgElems[i]);}
	CW_FREE_OBJECT(msgElems);
	for(i = 0; i < msgElemBindingNum; i++) { CW_FREE_PROTOCOL_MESSAGE(msgElemsBinding[i]);}
	CW_FREE_OBJECT(msgElemsBinding);
	
	// handle fragmentation
	PMTU = PMTU - gMaxDTLSHeaderSize - gMaxCAPWAPHeaderSize;
	
	if(PMTU > 0) {
		PMTU = (PMTU/8)*8; // CAPWAP fragments are made of groups of 8 bytes
		if(PMTU == 0) goto cw_dont_fragment;
		
		*fragmentsNumPtr = msg.offset / PMTU;
		if((msg.offset % PMTU) != 0) (*fragmentsNumPtr)++;
	} else {
	cw_dont_fragment:
		*fragmentsNumPtr = 1;
	}

	/*diaowq add "wireless special info" in capwap header @20120416,
	    for AC's wifi module to check,
	    which care about weather the size of capwap header is 16,
	    but not the detail of "wireless special info"
	*/
	CWBindingTransportHeaderValues bindingValues;
	bindingValues.dataRate=0;
	bindingValues.RSSI=0xa1;
	bindingValues.SNR=0xa1;
	transportVal.bindingValuesPtr = &bindingValues;
		
	if(*fragmentsNumPtr == 1) {

		CW_CREATE_OBJECT_ERR(*completeMsgPtr, CWProtocolMessage, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
		
		transportVal.isFragment = transportVal.last = transportVal.fragmentOffset = transportVal.fragmentID = 0;
		transportVal.payloadType = is_crypted;
//		transportVal.last = 1;

		// Assemble Message Elements
		if	(!(CWAssembleTransportHeader(&transportHdr, &transportVal))) {
			CW_FREE_PROTOCOL_MESSAGE(msg);
			CW_FREE_PROTOCOL_MESSAGE(transportHdr);
			CW_FREE_OBJECT(*completeMsgPtr);//zengmin add by Coverity Bad free 2013-06-08
			return CW_FALSE; // will be handled by the caller
		} 
	
		// assemble the message putting all the data consecutively
		CW_CREATE_PROTOCOL_MESSAGE(((*completeMsgPtr)[0]), transportHdr.offset + msg.offset, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
		
		CWProtocolStoreMessage(&((*completeMsgPtr)[0]), &transportHdr);
		CWProtocolStoreMessage(&((*completeMsgPtr)[0]), &msg);
		
		CW_FREE_PROTOCOL_MESSAGE(transportHdr);
		CW_FREE_PROTOCOL_MESSAGE(msg);
	} else {
		int fragID = CWGetFragmentID();
		int totalSize = msg.offset;

		CW_CREATE_PROTOCOL_MSG_ARRAY_ERR(*completeMsgPtr, *fragmentsNumPtr, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
		msg.offset = 0;
	
		for(i = 0; i < *fragmentsNumPtr; i++) { // for each fragment to assemble
			int fragSize;
			
			transportVal.isFragment = 1;
			transportVal.fragmentOffset = msg.offset/8;
			transportVal.fragmentID = fragID;
			transportVal.payloadType = is_crypted;

			if(i < ((*fragmentsNumPtr)-1)) { // not last fragment
				fragSize = PMTU;
				transportVal.last = 0;
			} else { // last fragment
				fragSize = totalSize - (((*fragmentsNumPtr)-1) * PMTU);
				transportVal.last = 1;
			}

			
			// Assemble Transport Header for this fragment
			if(!(CWAssembleTransportHeader(&transportHdr, &transportVal))) {
				CW_FREE_PROTOCOL_MESSAGE(msg);
				CW_FREE_PROTOCOL_MESSAGE(transportHdr);
				CW_FREE_OBJECT(*completeMsgPtr);//zengmin add by Coverity Bad free 2013-06-08
				return CW_FALSE; // will be handled by the caller
			}

			CW_CREATE_PROTOCOL_MESSAGE(((*completeMsgPtr)[i]), transportHdr.offset + fragSize, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
			
			CWProtocolStoreMessage(&((*completeMsgPtr)[i]), &transportHdr);
			CWProtocolStoreRawBytes(&((*completeMsgPtr)[i]), &((msg.msg)[msg.offset]), fragSize);
			msg.offset += fragSize;
			
			CW_FREE_PROTOCOL_MESSAGE(transportHdr);
		}
		CW_FREE_PROTOCOL_MESSAGE(msg);
	}
	
	return CW_TRUE;
}

	/* add for 2 radio rrm, pei add at 20100911 */
// Assemble a CAPWAP Control Packet, with the given Message Elements, Sequence Number and Message Type. Create Transport and Control Headers.
// completeMsgPtr is an array of fragments (can be of size 1 if the packet doesn't need fragmentation
CWBool CWAssembleMessageCheckRadio(CWProtocolMessage **completeMsgPtr, int *fragmentsNumPtr, int PMTU, int seqNum, int msgTypeValue, CWProtocolMessage *msgElems, const int msgElemNum, CWProtocolMessage *msgElemsBinding, const int msgElemBindingNum, int is_crypted, unsigned char radioId) {
	CWProtocolMessage transportHdr, controlHdr, msg;
	int msgElemsLen = 0;
	int i;
	
	CWProtocolTransportHeaderValues transportVal;
	CWControlHeaderValues controlVal;
	
	if(completeMsgPtr == NULL || fragmentsNumPtr == NULL || (msgElems == NULL && msgElemNum > 0) || (msgElemsBinding == NULL && msgElemBindingNum > 0)) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	
	//Calculate the whole size of the Msg Elements	
	for(i = 0; i < msgElemNum; i++) msgElemsLen += msgElems[i].offset;
	for(i = 0; i < msgElemBindingNum; i++) msgElemsLen += msgElemsBinding[i].offset;

	//Assemble Control Header
	controlVal.messageTypeValue = msgTypeValue;
	controlVal.msgElemsLen = msgElemsLen;
	controlVal.seqNum = seqNum;
	
	if(!(CWAssembleControlHeader(&controlHdr, &controlVal))) {
		CW_FREE_PROTOCOL_MESSAGE(controlHdr);
		for(i = 0; i < msgElemNum; i++) { CW_FREE_PROTOCOL_MESSAGE(msgElems[i]);}
		CW_FREE_OBJECT(msgElems);
		for(i = 0; i < msgElemBindingNum; i++) { CW_FREE_PROTOCOL_MESSAGE(msgElemsBinding[i]);}
		CW_FREE_OBJECT(msgElemsBinding);
		return CW_FALSE; // will be handled by the caller
	}
	
	// assemble the message putting all the data consecutively
	CW_CREATE_PROTOCOL_MESSAGE(msg, controlHdr.offset + msgElemsLen, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	
	CWProtocolStoreMessage(&msg, &controlHdr);
	for(i = 0; i < msgElemNum; i++) { // store in the request all the Message Elements
		CWProtocolStoreMessage(&msg, &(msgElems[i]));
	}
	for(i = 0; i < msgElemBindingNum; i++) { // store in the request all the Message Elements
		CWProtocolStoreMessage(&msg, &(msgElemsBinding[i]));
	}

	//Free memory not needed anymore
	CW_FREE_PROTOCOL_MESSAGE(controlHdr);
	for(i = 0; i < msgElemNum; i++) { CW_FREE_PROTOCOL_MESSAGE(msgElems[i]);}
	CW_FREE_OBJECT(msgElems);
	for(i = 0; i < msgElemBindingNum; i++) { CW_FREE_PROTOCOL_MESSAGE(msgElemsBinding[i]);}
	CW_FREE_OBJECT(msgElemsBinding);
	
	
	// handle fragmentation
	PMTU = PMTU - gMaxDTLSHeaderSize - gMaxCAPWAPHeaderSize;
	
	if(PMTU > 0) {
		PMTU = (PMTU/8)*8; // CAPWAP fragments are made of groups of 8 bytes
		if(PMTU == 0) goto cw_dont_fragment;
		
		*fragmentsNumPtr = msg.offset / PMTU;
		if((msg.offset % PMTU) != 0) (*fragmentsNumPtr)++;
	} else {
	cw_dont_fragment:
		*fragmentsNumPtr = 1;
	}
	
	transportVal.bindingValuesPtr = NULL;
		
	if(*fragmentsNumPtr == 1) {

		CW_CREATE_OBJECT_ERR(*completeMsgPtr, CWProtocolMessage, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
		
		transportVal.isFragment = transportVal.last = transportVal.fragmentOffset = transportVal.fragmentID = 0;
		transportVal.payloadType = is_crypted;
//		transportVal.last = 1;

		// Assemble Message Elements
		if	(!(CWAssembleTransportHeaderCheckRadio(&transportHdr, &transportVal, radioId))) {
			CW_FREE_PROTOCOL_MESSAGE(msg);
			CW_FREE_PROTOCOL_MESSAGE(transportHdr);
			CW_FREE_OBJECT(*completeMsgPtr);//zengmin add by Coverity Bad free 2013-06-08
			return CW_FALSE; // will be handled by the caller
		} 
	
		// assemble the message putting all the data consecutively
		CW_CREATE_PROTOCOL_MESSAGE(((*completeMsgPtr)[0]), transportHdr.offset + msg.offset, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
		
		CWProtocolStoreMessage(&((*completeMsgPtr)[0]), &transportHdr);
		CWProtocolStoreMessage(&((*completeMsgPtr)[0]), &msg);
		
		CW_FREE_PROTOCOL_MESSAGE(transportHdr);
		CW_FREE_PROTOCOL_MESSAGE(msg);
	} else {
		int fragID = CWGetFragmentID();
		int totalSize = msg.offset;

		CW_CREATE_PROTOCOL_MSG_ARRAY_ERR(*completeMsgPtr, *fragmentsNumPtr, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
		msg.offset = 0;
	
		for(i = 0; i < *fragmentsNumPtr; i++) { // for each fragment to assemble
			int fragSize;
			
			transportVal.isFragment = 1;
			transportVal.fragmentOffset = msg.offset/8;
			transportVal.fragmentID = fragID;
			transportVal.payloadType = is_crypted;

			if(i < ((*fragmentsNumPtr)-1)) { // not last fragment
				fragSize = PMTU;
				transportVal.last = 0;
			} else { // last fragment
				fragSize = totalSize - (((*fragmentsNumPtr)-1) * PMTU);
				transportVal.last = 1;
			}

			
			// Assemble Transport Header for this fragment
			if(!(CWAssembleTransportHeaderCheckRadio(&transportHdr, &transportVal, radioId))) {
				CW_FREE_PROTOCOL_MESSAGE(msg);
				CW_FREE_PROTOCOL_MESSAGE(transportHdr);
				CW_FREE_OBJECT(*completeMsgPtr);//zengmin add by Coverity Bad free 2013-06-08
				return CW_FALSE; // will be handled by the caller
			}

			CW_CREATE_PROTOCOL_MESSAGE(((*completeMsgPtr)[i]), transportHdr.offset + fragSize, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
			
			CWProtocolStoreMessage(&((*completeMsgPtr)[i]), &transportHdr);
			CWProtocolStoreRawBytes(&((*completeMsgPtr)[i]), &((msg.msg)[msg.offset]), fragSize);
			msg.offset += fragSize;
			
			CW_FREE_PROTOCOL_MESSAGE(transportHdr);
		}
		CW_FREE_PROTOCOL_MESSAGE(msg);
	}
	
	return CW_TRUE;
}

void CWProtocolDestroyFragment(void *f) {
	CW_FREE_OBJECT(((CWProtocolFragment*)f)->data);
	CW_FREE_OBJECT(f);
}

CWBool CWCompareFragment(void *newFrag, void *oldFrag)
{
	CWProtocolFragment *newEl = (CWProtocolFragment *) newFrag;
	CWProtocolFragment *oldEl = (CWProtocolFragment *) oldFrag;

	if((newEl->transportVal.fragmentID == oldEl->transportVal.fragmentID) &&
	(newEl->transportVal.fragmentOffset == oldEl->transportVal.fragmentOffset))
	{return CW_TRUE;}

	return CW_FALSE;
}

// parse a sigle fragment. If it is the last fragment we need or the only fragment, return the reassembled message in
// *reassembleMsg. If we need at lest one more fragment, save this fragment in the list. You then call this function again
// with a new fragment and the same list untill we got all the fragments.
CWBool CWProtocolParseFragment(char *buf, int readBytes, CWList *fragmentsListPtr, CWProtocolMessage *reassembledMsg, CWBool *dataFlagPtr) {
	CWProtocolTransportHeaderValues values;
	CWProtocolMessage msg;
	int totalSize;
	
	msg.msg = buf;
	msg.offset = 0;

#ifndef CW_NO_DTLS
	if(gDtlsPolicy != 1)
#endif
		*dataFlagPtr = CW_FALSE;
	
	if(!CWParseTransportHeader(&msg, &values, dataFlagPtr)) return CW_FALSE;
	if(values.isFragment == 0) { // single fragment
	/*	if(*fragmentsListPtr != NULL) { // we are receiving another fragmented message,
			return CWErrorRaise(CW_ERROR_INVALID_FORMAT, "Received Fragment with Different ID"); // discard this packet
		}
	*/

#ifndef CW_NO_DTLS
		if(*dataFlagPtr == CW_TRUE)
		{
			CW_CREATE_PROTOCOL_MESSAGE(*reassembledMsg, readBytes, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
			CWProtocolStoreRawBytes(reassembledMsg, buf, readBytes);
		}
		else
#endif
		{
			CW_CREATE_PROTOCOL_MESSAGE(*reassembledMsg, (readBytes-msg.offset), return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
			CWProtocolStoreRawBytes(reassembledMsg, &(buf[msg.offset]), (readBytes-msg.offset));
		}
			
		return CW_TRUE;
	} else {
		CWListElement *el;
		CWProtocolFragment *fragPtr;
		int currentSize;

		CW_CREATE_OBJECT_ERR(fragPtr, CWProtocolFragment, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););

		fragPtr->transportVal.fragmentID = values.fragmentID;
		fragPtr->transportVal.fragmentOffset = values.fragmentOffset;
		fragPtr->transportVal.last = values.last;


#ifndef CW_NO_DTLS
		if(*dataFlagPtr == CW_TRUE)
			fragPtr->dataLen = readBytes;
		else
#endif
			fragPtr->dataLen = (readBytes-msg.offset);

		if( *fragmentsListPtr == NULL ||  // empty list
		  (((CWProtocolFragment*)((*fragmentsListPtr)->data))->transportVal.fragmentID) == fragPtr->transportVal.fragmentID) // this fragment is in the set of fragments we are receiving
/*		{
			CW_CREATE_OBJECT_SIZE_ERR(fragPtr->data, fragPtr->dataLen, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
			CW_COPY_MEMORY(fragPtr->data, &(buf[msg.offset]), fragPtr->dataLen);
	
			if(!CWAddElementToList(fragmentsListPtr, fragPtr)) {
				CWDeleteList(fragmentsListPtr, CWProtocolDestroyFragment);
				CW_FREE_OBJECT(fragPtr);
				return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL);
			}
		}*/
		{
			CWListElement *aux = NULL;
			aux = CWSearchInList(*fragmentsListPtr, fragPtr, CWCompareFragment);
			if(aux == NULL) 
			{
			/*zengmin add free pointer when malloc error by Coverity Resource leak  2013-06-08*/
				CW_CREATE_OBJECT_SIZE_ERR(fragPtr->data, fragPtr->dataLen, {CW_FREE_OBJECT(fragPtr);return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL);});
#ifndef CW_NO_DTLS
				if(*dataFlagPtr == CW_TRUE)
					CW_COPY_MEMORY(fragPtr->data, buf, fragPtr->dataLen);
				else
#endif
					CW_COPY_MEMORY(fragPtr->data, &(buf[msg.offset]), fragPtr->dataLen);
	
				if(!CWAddElementToList(fragmentsListPtr, fragPtr)) {
					CWDeleteList(fragmentsListPtr, CWProtocolDestroyFragment);
					CW_FREE_OBJECT(fragPtr);
					return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL);
				}
			}
			else{
				CW_FREE_OBJECT(fragPtr);
				return CWErrorRaise(CW_ERROR_NEED_RESOURCE, NULL);
			}	
		} 
		else { 
			CWDeleteList(fragmentsListPtr, CWProtocolDestroyFragment);
			/*zengmin add free pointer when malloc error by Coverity Resource leak  2013-06-08*/
			CW_CREATE_OBJECT_SIZE_ERR(fragPtr->data, fragPtr->dataLen, {CW_FREE_OBJECT(fragPtr);return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL);});
#ifndef CW_NO_DTLS
			if(*dataFlagPtr == CW_TRUE)
				CW_COPY_MEMORY(fragPtr->data, buf, fragPtr->dataLen);
			else
#endif
				CW_COPY_MEMORY(fragPtr->data, &(buf[msg.offset]), fragPtr->dataLen);
			if(!CWAddElementToList(fragmentsListPtr, fragPtr)) {
				CWDeleteList(fragmentsListPtr, CWProtocolDestroyFragment);
				CW_FREE_OBJECT(fragPtr);
				return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL);
			}
		}

		// check if we have all the fragments
		for(el = *fragmentsListPtr, totalSize = 0; el != NULL; el = el->next) {
			if( (((CWProtocolFragment*)(el->data))->transportVal.last) == 1) { // last element
				totalSize = (((CWProtocolFragment*)(el->data))->transportVal.fragmentOffset) * 8;
				totalSize += (((CWProtocolFragment*)(el->data))->dataLen);
			}
		}

		if(totalSize == 0) { // we haven't the last fragment
			return CWErrorRaise(CW_ERROR_NEED_RESOURCE, NULL); // we need at least one more fragment
		}
		
		// calculate the size of all the fragments we have so far
		for(el = *fragmentsListPtr, currentSize = 0; el != NULL; el = el->next) {
			currentSize += (((CWProtocolFragment*)(el->data))->dataLen);
		}
		
		
		if(currentSize != totalSize) {
			return CWErrorRaise(CW_ERROR_NEED_RESOURCE, NULL); // we need at least one mpre fragment
		} else {
			int currentOffset = 0;
		
			
			CW_CREATE_PROTOCOL_MESSAGE(*reassembledMsg, (totalSize), return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););

			CW_REPEAT_FOREVER {
				CWBool found = CW_FALSE;
				
				// find the fragment in the list with the currend offset
				for(el = *fragmentsListPtr, currentSize = 0; el != NULL; el = el->next) {
					if( (((CWProtocolFragment*)(el->data))->transportVal.fragmentOffset) == currentOffset) {
						found = CW_TRUE;
						break;
					}
				}
			
				if(!found) { // mmm... we should have all the fragment, but we haven't a fragment for the current offset
					CWDeleteList(fragmentsListPtr, CWProtocolDestroyFragment);
					CW_FREE_PROTOCOL_MESSAGE(*reassembledMsg);
					return CWErrorRaise(CW_ERROR_INVALID_FORMAT, "Bad Fragmented Messsage");
				}
				
				CWProtocolStoreRawBytes(reassembledMsg, (((CWProtocolFragment*)(el->data))->data), (((CWProtocolFragment*)(el->data))->dataLen));

				if( (((CWProtocolFragment*)(el->data))->transportVal.last) == 1) { // last fragment
					CWDeleteList(fragmentsListPtr, CWProtocolDestroyFragment);
					return CW_TRUE;
				}

				currentOffset += ((((CWProtocolFragment*)(el->data))->dataLen) / 8);
			}
		}
	}
}



/*************************************************
*
*2008-04-26    luoxun
*
*This function is similar to CWProtocolParseFragment()
*The only difference is that a parameter is added to get CAPWAP head
*information back
*
**************************************************/
// parse a sigle fragment. If it is the last fragment we need or the only fragment, return the reassembled message in
// *reassembleMsg. If we need at lest one more fragment, save this fragment in the list. You then call this function again
// with a new fragment and the same list untill we got all the fragments.
CWBool CWProtocolParseFragment_GetCAPWAPheadInfo(char *buf, int readBytes, CWList *fragmentsListPtr, CWProtocolMessage *reassembledMsg, CWBool *dataFlagPtr, CWProtocolTransportHeaderValues *pCAPWAPheadvalue) {
	CWProtocolTransportHeaderValues values;
	CWBindingTransportHeaderValues wireless_opt;
	CWProtocolMessage msg;
	int totalSize;
	
	msg.msg = buf;
	msg.offset = 0;
	*dataFlagPtr = CW_FALSE;
	/*zengmin add initialization value by Coverity Using uninitialized value  2013-06-08*/
	bzero(&wireless_opt, sizeof(CWBindingTransportHeaderValues));
	bzero(&values, sizeof(CWProtocolTransportHeaderValues));
	values.bindingValuesPtr = &wireless_opt;
	
	if(!CWParseTransportHeader(&msg, &values, dataFlagPtr)) return CW_FALSE;

	//get CAPWAP head information
	memcpy(pCAPWAPheadvalue, &values, sizeof(CWProtocolTransportHeaderValues));//luoxun
	
	if(values.isFragment == 0) { // single fragment
	/*	if(*fragmentsListPtr != NULL) { // we are receiving another fragmented message,
			return CWErrorRaise(CW_ERROR_INVALID_FORMAT, "Received Fragment with Different ID"); // discard this packet
		}
	*/

	
//		CW_CREATE_PROTOCOL_MESSAGE(*reassembledMsg, (readBytes-msg.offset), return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
//		CW_CREATE_PROTOCOL_MESSAGE(*reassembledMsg, (readBytes-msg.offset+12), return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
		//luoxun
//		CW_CREATE_PROTOCOL_MESSAGE() is turned away to avoid free() error.
//		The offset of 12 Bytes is used to construct a capwap head.
#if 0
		reassembledMsg->msg = (char *)malloc((readBytes-msg.offset+20));
		if( NULL == reassembledMsg->msg )
			{
				printf("malloc memory failure");
				return CW_FALSE;
			}
		bzero(reassembledMsg->msg, (readBytes-msg.offset+20));
		reassembledMsg->offset = 20;

		CWProtocolStoreRawBytes(reassembledMsg, &(buf[msg.offset]), (readBytes-msg.offset));
#endif
		//Guo Xuebin add, for split mac.
		bzero(reassembledMsg->msg, msg.offset);
		reassembledMsg->msgLen = readBytes-msg.offset;
		reassembledMsg->offset = msg.offset;

		return CW_TRUE;
	} else {
		CWListElement *el;
		CWProtocolFragment *fragPtr;
		int currentSize;

		CW_CREATE_OBJECT_ERR(fragPtr, CWProtocolFragment, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););

		fragPtr->transportVal.fragmentID = values.fragmentID;
		fragPtr->transportVal.fragmentOffset = values.fragmentOffset;
		fragPtr->transportVal.last = values.last;

	
		fragPtr->dataLen = (readBytes-msg.offset);

		if( *fragmentsListPtr == NULL ||  // empty list
		  (((CWProtocolFragment*)((*fragmentsListPtr)->data))->transportVal.fragmentID) == fragPtr->transportVal.fragmentID) // this fragment is in the set of fragments we are receiving
/*		{
			CW_CREATE_OBJECT_SIZE_ERR(fragPtr->data, fragPtr->dataLen, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
			CW_COPY_MEMORY(fragPtr->data, &(buf[msg.offset]), fragPtr->dataLen);
	
			if(!CWAddElementToList(fragmentsListPtr, fragPtr)) {
				CWDeleteList(fragmentsListPtr, CWProtocolDestroyFragment);
				CW_FREE_OBJECT(fragPtr);
				return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL);
			}
		}*/
		{
			CWListElement *aux = NULL;
			aux = CWSearchInList(*fragmentsListPtr, fragPtr, CWCompareFragment);
			if(aux == NULL) 
			{
			/*zengmin add free pointer when malloc error by Coverity Resource leak  2013-06-08*/
				CW_CREATE_OBJECT_SIZE_ERR(fragPtr->data, fragPtr->dataLen, {CW_FREE_OBJECT(fragPtr);return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL);});
				CW_COPY_MEMORY(fragPtr->data, &(buf[msg.offset]), fragPtr->dataLen);
	
				if(!CWAddElementToList(fragmentsListPtr, fragPtr)) {
					CWDeleteList(fragmentsListPtr, CWProtocolDestroyFragment);
					CW_FREE_OBJECT(fragPtr);
					return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL);
				}
			}
			else{
				CW_FREE_OBJECT(fragPtr);
				return CWErrorRaise(CW_ERROR_NEED_RESOURCE, NULL);
			}	
		} 
		else { 
			CWDeleteList(fragmentsListPtr, CWProtocolDestroyFragment);
		/*zengmin add free pointer when malloc error by Coverity Resource leak  2013-06-08*/
		//	CW_CREATE_OBJECT_SIZE_ERR(fragPtr->data, fragPtr->dataLen, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
			CW_CREATE_OBJECT_SIZE_ERR(fragPtr->data, fragPtr->dataLen, {CW_FREE_OBJECT(fragPtr);return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL);});
			CW_COPY_MEMORY(fragPtr->data, &(buf[msg.offset]), fragPtr->dataLen);
			if(!CWAddElementToList(fragmentsListPtr, fragPtr)) {
				CWDeleteList(fragmentsListPtr, CWProtocolDestroyFragment);
				CW_FREE_OBJECT(fragPtr);
				return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL);
			}
		}

		// check if we have all the fragments
		for(el = *fragmentsListPtr, totalSize = 0; el != NULL; el = el->next) {
			if( (((CWProtocolFragment*)(el->data))->transportVal.last) == 1) { // last element
				totalSize = (((CWProtocolFragment*)(el->data))->transportVal.fragmentOffset) * 8;
				totalSize += (((CWProtocolFragment*)(el->data))->dataLen);
			}
		}

		if(totalSize == 0) { // we haven't the last fragment
			return CWErrorRaise(CW_ERROR_NEED_RESOURCE, NULL); // we need at least one more fragment
		}
		
		// calculate the size of all the fragments we have so far
		for(el = *fragmentsListPtr, currentSize = 0; el != NULL; el = el->next) {
			currentSize += (((CWProtocolFragment*)(el->data))->dataLen);
		}
		
		
		if(currentSize != totalSize) {
			return CWErrorRaise(CW_ERROR_NEED_RESOURCE, NULL); // we need at least one mpre fragment
		} else {
			int currentOffset = 0;
		
			
//			CW_CREATE_PROTOCOL_MESSAGE(*reassembledMsg, (totalSize), return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
//			CW_CREATE_PROTOCOL_MESSAGE(*reassembledMsg, (totalSize+12), return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
			//luoxun
//			CW_CREATE_PROTOCOL_MESSAGE() is turned away to avoid free() error.
//			The offset of 12 Bytes is used to construct a capwap head.
			reassembledMsg->msg = (char *)malloc((totalSize+20));
			if( NULL == reassembledMsg->msg )
				{
					autelan_printf("malloc memory failure");
					return CW_FALSE;
				}
			bzero(reassembledMsg->msg, (totalSize+20));
			reassembledMsg->offset = 20;



			CW_REPEAT_FOREVER {
				CWBool found = CW_FALSE;
				
				// find the fragment in the list with the currend offset
				for(el = *fragmentsListPtr, currentSize = 0; el != NULL; el = el->next) {
					if( (((CWProtocolFragment*)(el->data))->transportVal.fragmentOffset) == currentOffset) {
						found = CW_TRUE;
						break;
					}
				}
			
				if(!found) { // mmm... we should have all the fragment, but we haven't a fragment for the current offset
					CWDeleteList(fragmentsListPtr, CWProtocolDestroyFragment);
					CW_FREE_PROTOCOL_MESSAGE(*reassembledMsg);
					return CWErrorRaise(CW_ERROR_INVALID_FORMAT, "Bad Fragmented Messsage");
				}
				
				CWProtocolStoreRawBytes(reassembledMsg, (((CWProtocolFragment*)(el->data))->data), (((CWProtocolFragment*)(el->data))->dataLen));

				if( (((CWProtocolFragment*)(el->data))->transportVal.last) == 1) { // last fragment
					CWDeleteList(fragmentsListPtr, CWProtocolDestroyFragment);
					return CW_TRUE;
				}

				currentOffset += ((((CWProtocolFragment*)(el->data))->dataLen) / 8);
			}
		}
	}
}
// Parse Transport Header
CWBool CWParseTransportHeader(CWProtocolMessage *msgPtr, CWProtocolTransportHeaderValues *valuesPtr, CWBool *dataFlagPtr) {
	int transport4BytesLen; 
	int val;
	int optionalWireless = 0;
	int version, rid;

	if(msgPtr == NULL || valuesPtr == NULL) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	
	val = CWProtocolRetrieve32(msgPtr);
	
	if(CWGetField32(val, CW_TRANSPORT_HEADER_VERSION_START, CW_TRANSPORT_HEADER_VERSION_LEN) != CW_PROTOCOL_VERSION)
		return CWErrorRaise(CW_ERROR_INVALID_FORMAT, "Wrong Protocol Version");
		
	version = CWGetField32(val, CW_TRANSPORT_HEADER_VERSION_START, CW_TRANSPORT_HEADER_VERSION_LEN);
	
	valuesPtr->payloadType = CWGetField32(val, CW_TRANSPORT_HEADER_TYPE_START, CW_TRANSPORT_HEADER_TYPE_LEN);
	
	transport4BytesLen = CWGetField32(val,	CW_TRANSPORT_HEADER_HLEN_START, CW_TRANSPORT_HEADER_HLEN_LEN);

	rid = CWGetField32(val, CW_TRANSPORT_HEADER_RID_START, CW_TRANSPORT_HEADER_RID_LEN);
	
	
	valuesPtr->type = CWGetField32(val, CW_TRANSPORT_HEADER_T_START, CW_TRANSPORT_HEADER_T_LEN);
	
	valuesPtr->isFragment = CWGetField32(val, CW_TRANSPORT_HEADER_F_START, CW_TRANSPORT_HEADER_F_LEN);

	valuesPtr->last = CWGetField32(val, CW_TRANSPORT_HEADER_L_START, CW_TRANSPORT_HEADER_L_LEN);
	
	optionalWireless = CWGetField32(val, CW_TRANSPORT_HEADER_W_START, CW_TRANSPORT_HEADER_W_LEN);
	
	valuesPtr->keepAlive = CWGetField32(val, CW_TRANSPORT_HEADER_K_START, CW_TRANSPORT_HEADER_K_LEN);

	val = CWProtocolRetrieve32(msgPtr);

	valuesPtr->fragmentID = CWGetField32(val, CW_TRANSPORT_HEADER_FRAGMENT_ID_START, CW_TRANSPORT_HEADER_FRAGMENT_ID_LEN);

	valuesPtr->fragmentOffset = CWGetField32(val, CW_TRANSPORT_HEADER_FRAGMENT_OFFSET_START, CW_TRANSPORT_HEADER_FRAGMENT_OFFSET_LEN);


	/* Modified by GuoXuebin
	*
	* In there, function CW_CREATE_OBJECT_ERR would memory leak.
	*
	*/
	//valuesPtr->bindingValuesPtr = NULL;
	if(transport4BytesLen == 4 && optionalWireless == 1){
		*dataFlagPtr = CW_TRUE;
		//CW_CREATE_OBJECT_ERR( valuesPtr->bindingValuesPtr, CWBindingTransportHeaderValues, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY,NULL););
		if (!CWParseTransportHeaderBinding(msgPtr, valuesPtr->bindingValuesPtr)){
			CW_FREE_OBJECT(valuesPtr->bindingValuesPtr);
			return CW_FALSE;
		}
	}


#ifndef CW_NO_DTLS
	if(*dataFlagPtr == CW_TRUE)
		return CW_TRUE;
	else
#endif
		return (transport4BytesLen == 2 || (transport4BytesLen == 4 && optionalWireless == 1)) ? CW_TRUE : CWErrorRaise(CW_ERROR_INVALID_FORMAT, "Malformed Transport Header"); //TEMP?
}

// Parse Control Header
CWBool CWParseControlHeader(CWProtocolMessage *msgPtr, CWControlHeaderValues *valPtr) {	
	unsigned char flags=0;

	if(msgPtr == NULL|| valPtr == NULL) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);

	valPtr->messageTypeValue = CWProtocolRetrieve32(msgPtr);
	
	valPtr->seqNum = CWProtocolRetrieve8(msgPtr);

	valPtr->msgElemsLen = CWProtocolRetrieve16(msgPtr);
	
	flags=CWProtocolRetrieve8(msgPtr);
	
//	valPtr->timestamp = CWProtocolRetrieve32(msgPtr);

	
	return CW_TRUE;
}

//## Assemble a Message Response containing a Failure (Unrecognized Message) Result Code
CWBool CWAssembleUnrecognizedMessageResponse(CWProtocolMessage **messagesPtr, int *fragmentsNumPtr, int PMTU, int seqNum, int msgType)
{
	CWProtocolMessage *msgElems= NULL;
	const int msgElemCount=1;
	CWProtocolMessage *msgElemsBinding= NULL;
	int msgElemBindingCount=0;
	
	if(messagesPtr == NULL || fragmentsNumPtr == NULL) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	
	
	CW_CREATE_OBJECT_ERR(msgElems, CWProtocolMessage, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););

	if (!(CWAssembleMsgElemResultCode(msgElems,CW_PROTOCOL_FAILURE_UNRECOGNIZED_REQ))) {
		CW_FREE_OBJECT(msgElems);
		return CW_FALSE;
	}
	
	if(!(CWAssembleMessage(messagesPtr, fragmentsNumPtr, PMTU, seqNum, msgType, msgElems, msgElemCount, msgElemsBinding, msgElemBindingCount, CW_PACKET_CRYPT))) 
		return CW_FALSE;
	
	CWWTPDebugLog("Assemble unrecognized message response success!");
	return CW_TRUE;
}


CWBool CWAssembleMsgElemSessionID(CWProtocolMessage *msgPtr, int sessionID) {
	if(msgPtr == NULL) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	
	// create message
	CW_CREATE_PROTOCOL_MESSAGE(*msgPtr, 4, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	
	CWProtocolStore32(msgPtr, sessionID);
				
	return CWAssembleMsgElem(msgPtr, CW_MSG_ELEMENT_SESSION_ID_CW_TYPE);
}

CWBool CWParseACName(CWProtocolMessage *msgPtr, int len, char **valPtr) {	
	CWParseMessageElementStart();
	
	*valPtr = CWProtocolRetrieveStr(msgPtr, len);
	if(valPtr == NULL) return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL);
	
	CWParseMessageElementEnd();
}

CWBool CWParseWTPRadioAdminState (CWProtocolMessage *msgPtr, int len, CWRadioAdminInfoValues *valPtr)
{
	CWParseMessageElementStart();
	
	valPtr->ID = CWProtocolRetrieve8(msgPtr);
	valPtr->state = CWProtocolRetrieve8(msgPtr);
	//valPtr->cause = CWProtocolRetrieve8(msgPtr);
	
	
	CWParseMessageElementEnd();
}

CWBool CWParseWTPRadioOperationalState (CWProtocolMessage *msgPtr, int len, CWRadioOperationalInfoValues *valPtr)
{
	CWParseMessageElementStart();
	
	valPtr->ID = CWProtocolRetrieve8(msgPtr);
	valPtr->state = CWProtocolRetrieve8(msgPtr);
	valPtr->cause = CWProtocolRetrieve8(msgPtr);
	
	
	CWParseMessageElementEnd();
}

CWBool CWParseFormatMsgElem(CWProtocolMessage *completeMsg,unsigned short int *type,unsigned short int *len)
{
	*type = CWProtocolRetrieve16(completeMsg);
	*len = CWProtocolRetrieve16(completeMsg);
	return CW_TRUE;
}

CWBool CWParseResultCode(CWProtocolMessage *msgPtr, int len, CWProtocolResultCode *valPtr) {
	CWParseMessageElementStart();
	
	*valPtr = CWProtocolRetrieve32(msgPtr);
	
	CWParseMessageElementEnd();
}

void CWWTPResetRadioStatistics(WTPRadioStatisticsInfo *radioStatistics)
{	
	radioStatistics->lastFailureType= UNKNOWN_TYPE;
	radioStatistics->resetCount= 0;
	radioStatistics->SWFailureCount= 0;
	radioStatistics->HWFailuireCount= 0;
	radioStatistics->otherFailureCount= 0;
	radioStatistics->unknownFailureCount= 0;
	radioStatistics->configUpdateCount= 0;
	radioStatistics->channelChangeCount= 0;
	radioStatistics->bandChangeCount= 0;
	radioStatistics->currentNoiseFloor= 0;
}

void CWFreeMessageFragments(CWProtocolMessage* messages, int fragmentsNum)
{
	int i;

	for(i=0; i<fragmentsNum; i++)
	{
		CW_FREE_PROTOCOL_MESSAGE(messages[i]);
	}
}


