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

const int gMaxCAPWAPHeaderSizeBinding = 16; // note: this include optional Wireless field


CWBool CWBindingCheckType(int elemType)
{
	if (elemType>=BINDING_MIN_ELEM_TYPE && elemType<=BINDING_MAX_ELEM_TYPE)
		return CW_TRUE;
	return CW_FALSE;
}

// Assemble a CAPWAP Data Packet creating Transport Header.
// completeMsgPtr is an array of fragments (can be of size 1 if the packet doesn't need fragmentation)
CWBool CWAssembleDataMessage(CWProtocolMessage **completeMsgPtr, int *fragmentsNumPtr, int PMTU, CWProtocolMessage *frame, CWBindingTransportHeaderValues *bindingValuesPtr, int is_crypted) {
	CWProtocolMessage transportHdr;
	CWProtocolTransportHeaderValues transportVal;

	if(completeMsgPtr == NULL || fragmentsNumPtr == NULL || frame == NULL || bindingValuesPtr == NULL) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	
	
	// handle fragmentation
	
	PMTU = PMTU - gMaxCAPWAPHeaderSizeBinding;
	
	if(PMTU > 0) {
		PMTU = (PMTU/8)*8; // CAPWAP fragments are made of groups of 8 bytes
		if(PMTU == 0) goto cw_dont_fragment;
		
		*fragmentsNumPtr = (frame->offset) / PMTU;
		if((frame->offset % PMTU) != 0) (*fragmentsNumPtr)++;
	} else {
	cw_dont_fragment:
		*fragmentsNumPtr = 1;
	}
	
	transportVal.bindingValuesPtr = bindingValuesPtr;
		
	if(*fragmentsNumPtr == 1) {
			
		transportVal.isFragment = transportVal.last = transportVal.fragmentOffset = transportVal.fragmentID = 0;
		transportVal.payloadType = is_crypted;		

		// Assemble Message Elements
		if(!(CWAssembleTransportHeader2(&transportHdr, &transportVal,bindingValuesPtr->radioId))) {
			CW_FREE_PROTOCOL_MESSAGE(transportHdr);
			return CW_FALSE; // will be handled by the caller
		} 
		
		CW_CREATE_OBJECT_ERR(*completeMsgPtr, CWProtocolMessage, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
		CW_CREATE_PROTOCOL_MESSAGE(((*completeMsgPtr)[0]), transportHdr.offset + frame->offset, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
		
		CWProtocolStoreMessage(&((*completeMsgPtr)[0]), &transportHdr);
		CWProtocolStoreMessage(&((*completeMsgPtr)[0]), frame);
		
		CW_FREE_PROTOCOL_MESSAGE(transportHdr);
	} else {
		int fragID = CWGetFragmentID();
		int totalSize = frame->offset;
		
		CW_CREATE_PROTOCOL_MSG_ARRAY_ERR(*completeMsgPtr, *fragmentsNumPtr, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
		frame->offset = 0;

		int i;
		for(i = 0; i < *fragmentsNumPtr; i++) { // for each fragment to assemble
			int fragSize;
			
			transportVal.isFragment = 1;
			transportVal.fragmentOffset = (frame->offset) / 8;
			transportVal.fragmentID = fragID;
			transportVal.payloadType = is_crypted; //zengmin set payloadType value by Coverity Using uninitialized value  2013-06-08	
			
			if(i < ((*fragmentsNumPtr)-1)) { // not last fragment
				fragSize = PMTU;
				transportVal.last = 0;
			} else { // last fragment
				fragSize = totalSize - (((*fragmentsNumPtr)-1) * PMTU);
				transportVal.last = 1;
			}
		
			
			// Assemble Transport Header for this fragment
			if(!(CWAssembleTransportHeader2(&transportHdr, &transportVal,bindingValuesPtr->radioId))) {
				CW_FREE_PROTOCOL_MESSAGE(transportHdr);
				CW_FREE_OBJECT(*completeMsgPtr);//zengmin add by Coverity Bad free 2013-06-08
				return CW_FALSE; // will be handled by the caller
			}
			
			CW_CREATE_PROTOCOL_MESSAGE(((*completeMsgPtr)[i]), transportHdr.offset + fragSize, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
			CWProtocolStoreMessage(&((*completeMsgPtr)[i]), &transportHdr);
			CWProtocolStoreRawBytes(&((*completeMsgPtr)[i]), &((frame->msg)[frame->offset]), fragSize);
			(frame->offset) += fragSize;
			
			CW_FREE_PROTOCOL_MESSAGE(transportHdr);
		}
	}
	return CW_TRUE;
}


CWBool CWAssembleTransportHeaderBinding(CWProtocolMessage *transportHdrPtr, CWBindingTransportHeaderValues *valuesPtr)
{
	int val = 0;
	if(valuesPtr->flag_m == 1)
	{
		CWSetField32(val,
			     CW_TRANSPORT_HEADER_LENGTH_START,
			     CW_TRANSPORT_HEADER_LENGTH_LEN,
			     CW_BINDING_MACLENGTH);

		CWSetField32(val,
			     CW_TRANSPORT_HEADER_MAC_1_START,
			     CW_TRANSPORT_HEADER_MAC_ELE_LEN,
			     valuesPtr->bssid[0]);

		CWSetField32(val,
			     CW_TRANSPORT_HEADER_MAC_2_START,
			     CW_TRANSPORT_HEADER_MAC_ELE_LEN,
			     valuesPtr->bssid[1]);

		CWSetField32(val,
			     CW_TRANSPORT_HEADER_MAC_3_START,
			     CW_TRANSPORT_HEADER_MAC_ELE_LEN,
			     valuesPtr->bssid[2]);

		CWProtocolStore32(transportHdrPtr, val);
		
		val = 0;
		
		CWSetField32(val,
				 CW_TRANSPORT_HEADER_MAC_4_START,
				 CW_TRANSPORT_HEADER_MAC_ELE_LEN,
				 valuesPtr->bssid[3]);
		
		CWSetField32(val,
				 CW_TRANSPORT_HEADER_MAC_5_START,
				 CW_TRANSPORT_HEADER_MAC_ELE_LEN,
				 valuesPtr->bssid[4]);
		
		CWSetField32(val,
				 CW_TRANSPORT_HEADER_MAC_6_START,
				 CW_TRANSPORT_HEADER_MAC_ELE_LEN,
				 valuesPtr->bssid[5]);
		
		CWSetField32(val,
				 CW_TRANSPORT_HEADER_PADDING_START,
				 CW_TRANSPORT_HEADER_PADDING_LEN,
				 0);
		
		CWProtocolStore32(transportHdrPtr, val);
	}
	

	if(valuesPtr->flag_w == 1)
	{
		val = 0;
		CWSetField32(val,
				 CW_TRANSPORT_HEADER_LENGTH_START,
				 CW_TRANSPORT_HEADER_LENGTH_LEN,
				 CW_BINDING_DATALENGTH);
	
		CWSetField32(val,
				 CW_TRANSPORT_HEADER_RSSI_START,
				 CW_TRANSPORT_HEADER_RSSI_LEN,
				 valuesPtr->RSSI);
	
		CWSetField32(val,
				 CW_TRANSPORT_HEADER_SNR_START,
				 CW_TRANSPORT_HEADER_SNR_LEN,
				 valuesPtr->SNR);
	
		/* Mauro: inserisci il byte piu' significativo del sottocampo Data */
		CWSetField32(val,
				 CW_TRANSPORT_HEADER_DATARATE_1_START,
				 CW_TRANSPORT_HEADER_DATARATE_1_LEN,
				// (valuesPtr->dataRate)>>24);
				 (valuesPtr->dataRate)>>8);//zengmin modify: dataRate is unsigned short by Coverity Out-of-bounds access 2013-06-09

		CWProtocolStore32(transportHdrPtr, val);
		
		val = 0;
		/* Mauro: inserisci il byte meno significativo del sottocampo Data */
		CWSetField32(val,
			     CW_TRANSPORT_HEADER_DATARATE_2_START,
			     CW_TRANSPORT_HEADER_DATARATE_2_LEN,
				     (valuesPtr->dataRate) & 0x00FF);//0x00FFFFFF);

		/*	CWSetField32(val,
				     CW_TRANSPORT_HEADER_DATARATE_START,
				     CW_TRANSPORT_HEADER_DATARATE_LEN,
				     valuesPtr->dataRate);
		*/
		CWSetField32(val,
			     CW_TRANSPORT_HEADER_PADDING_START,
			     CW_TRANSPORT_HEADER_PADDING_LEN,
			     0);
	
		CWProtocolStore32(transportHdrPtr, val);
	}


	return CW_TRUE;
}

CWBool CWParseTransportHeaderBinding(CWProtocolMessage *msgPtr, CWBindingTransportHeaderValues *valuesPtr){
	unsigned int val = 0;
	
	if(msgPtr == NULL || valuesPtr == NULL) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	
	val = CWProtocolRetrieve32(msgPtr);

/* Mauro: non piu' specificato nel campo Wireless Specific Information
	if(CWGetField32(val, CW_TRANSPORT_HEADER_WIRELESS_ID_START, CW_TRANSPORT_HEADER_WIRELESS_ID_LEN) != CW_BINDING_WIRELESSID)
		return CWErrorRaise(CW_ERROR_INVALID_FORMAT, "Wrong Binding Wireless ID");
*/
	if(CWGetField32(val, CW_TRANSPORT_HEADER_LENGTH_START, CW_TRANSPORT_HEADER_LENGTH_LEN) != CW_BINDING_DATALENGTH)
		return CWErrorRaise(CW_ERROR_INVALID_FORMAT, "Wrong Binding Data Field Length");

	valuesPtr->RSSI = CWGetField32(val, CW_TRANSPORT_HEADER_RSSI_START, CW_TRANSPORT_HEADER_RSSI_LEN);
	
	valuesPtr->SNR = CWGetField32(val, CW_TRANSPORT_HEADER_SNR_START, CW_TRANSPORT_HEADER_SNR_LEN);
	
/* Mauro: preleva il byte piu' significativo del sottocampo Data */
	valuesPtr->dataRate = CWGetField32(val, CW_TRANSPORT_HEADER_DATARATE_1_START, CW_TRANSPORT_HEADER_DATARATE_1_LEN);

	val = CWProtocolRetrieve32(msgPtr);

/* Mauro: preleva il byte meno significativo del sottocampo Data */
	valuesPtr->dataRate = ((valuesPtr->dataRate)<<8) | CWGetField32(val, CW_TRANSPORT_HEADER_DATARATE_1_START, CW_TRANSPORT_HEADER_DATARATE_1_LEN);

//	valuesPtr->dataRate = CWGetField32(val, CW_TRANSPORT_HEADER_DATARATE_START, CW_TRANSPORT_HEADER_DATARATE_LEN);
	
	return CW_TRUE;
}
/***************************************************************************
 * 
 * Function:  CWAssembleAssignedWTPBssid
 *
 * Purpose:  assemble the assigned WTP bssid element [binding 6.3]
 *
 * Inputs: 
 *
 * Output:    
 *
 * Returns:   
 **************************************************************************/
CWBool CWAssembleAssignedWTPBssid(CWProtocolMessage *bindPtr, char *bssid,int crete_wlan_id,int radio_id){
	CW_CREATE_PROTOCOL_MESSAGE(*bindPtr, 8, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	CWProtocolStore8(bindPtr,radio_id);
	CWProtocolStore8(bindPtr,crete_wlan_id);
	CWProtocolStoreRawBytes(bindPtr,bssid,6);
	return CWAssembleMsgElem(bindPtr, BINDING_MSG_ELEMENT_TYPE_ASSIGNED_WTP_BSSID);
}
