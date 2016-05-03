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

void CWResetPendingMsgBox(CWPendingRequestMessage *pendingRequestMsgs) {
	pendingRequestMsgs->msgType = UNUSED_MSG_TYPE;
	pendingRequestMsgs->seqNum = 0;
	pendingRequestMsgs->retransmission = 0;
	pendingRequestMsgs->timer_sec = 0;
	pendingRequestMsgs->timer_hdl = NULL;

	CW_FREE_OBJECT(pendingRequestMsgs->timer_arg); /* fix bug, capwap 0.93.3 */
	pendingRequestMsgs->timer_arg = NULL;

	timer_rem(pendingRequestMsgs->timer,0);
	
	int i;
	for(i=0; i<(pendingRequestMsgs->fragmentsNum); i++){
		CW_FREE_PROTOCOL_MESSAGE((pendingRequestMsgs->msgElems)[i]);
	}
	CW_FREE_OBJECT(pendingRequestMsgs->msgElems);

	pendingRequestMsgs->fragmentsNum = 0;
	
	return;
}

int CWFindFreePendingMsgBox(CWPendingRequestMessage *pendingRequestMsgs, const int length) {
	int k;

	for(k=0; k<length; k++){
		if(pendingRequestMsgs[k].msgType == UNUSED_MSG_TYPE){
			CWResetPendingMsgBox(pendingRequestMsgs+k);
			return k;
		}
	}
	return -1;
}

CWBool CWUpdatePendingMsgBox(CWPendingRequestMessage *pendingRequestMsgs, 
			     unsigned char msgType,
			     int seqNum,
			     int timer_sec,
			     CWTimerArg timer_arg,
			     void (*timer_hdl)(CWTimerArg),
			     int retransmission,
			     CWProtocolMessage *msgElems,
			     int fragmentsNum){

	if(pendingRequestMsgs == NULL) return CW_FALSE;
	if(pendingRequestMsgs->msgType != UNUSED_MSG_TYPE) return CW_TRUE;

	pendingRequestMsgs->msgType = msgType;
	pendingRequestMsgs->seqNum = seqNum;
	pendingRequestMsgs->retransmission = retransmission;
	pendingRequestMsgs->msgElems = msgElems;
	pendingRequestMsgs->fragmentsNum = fragmentsNum;
	pendingRequestMsgs->timer_sec = timer_sec;
	pendingRequestMsgs->timer_hdl = timer_hdl;
	pendingRequestMsgs->timer_arg = timer_arg;
	if((pendingRequestMsgs->timer = timer_add(timer_sec, 0, timer_hdl, timer_arg))) {
		return CW_FALSE;
	}	

	return CW_TRUE;
}

int CWFindPendingRequestMsgsBox(CWPendingRequestMessage *pendingRequestMsgs, const int length, const int msgType, const int seqNum){
	if(pendingRequestMsgs == NULL) return -1;
	int k;
	for(k=0; k<length; k++){
		if((pendingRequestMsgs[k].seqNum == seqNum) && (pendingRequestMsgs[k].msgType == msgType)){

			timer_rem(pendingRequestMsgs[k].timer,0);	
			return k;
		}
	}
	return -1;

}


int CWSendPendingRequestMessage(CWPendingRequestMessage *pendingRequestMsgs, CWProtocolMessage *messages, int fragmentsNum) {
	int pendingReqIndex = -1;

	if(messages == NULL || fragmentsNum <0) {
		return -1;
	}

	pendingReqIndex = CWFindFreePendingMsgBox(pendingRequestMsgs, MAX_PENDING_REQUEST_MSGS);

	if(pendingReqIndex < 0) {
		return -1;
	}
	
	int i;
	for(i = 0; i < fragmentsNum; i++) {
#ifndef CW_NO_DTLS
		if(gDtlsSecurity == 1)
		{
			if(!CWSecuritySend(gWTPSession, messages[i].msg, messages[i].offset))
				return -1;
		}
		else
#endif
		{
			if (!CWNetworkSendUnsafeUnconnected(gWTPSocket, &(gACInfoPtr->preferredAddress), messages[i].msg, messages[i].offset))
				return -1;
		}
	}

	return pendingReqIndex;
}

