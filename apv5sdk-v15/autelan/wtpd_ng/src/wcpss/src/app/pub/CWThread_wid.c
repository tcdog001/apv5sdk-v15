
#include "CWCommon.h"

#include <stdio.h>
#include <time.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h> 
#include <sys/msg.h>
#include "wcpss/waw.h"
#include "wcpss/wid/WID.h" 
#include "CWAC.h"
// timers
typedef struct {
 	CWThread *requestedThreadPtr;
 	int signalToRaise;
	int WTPID;
} CWThreadTimerArg;

void CWHandleTimer(CWTimerArg arg) {
	msgq msg;	
	CWThreadTimerArg *a = (CWThreadTimerArg*)arg;
 //	CWThread requestedThreadPtr = *(a->requestedThreadPtr);
 	int signalToRaise = a->signalToRaise;
	int WTPID = a->WTPID;
	int WTPMsgqID;
	CWGetMsgQueue(&WTPMsgqID);
//	CWThreadSendSignal(requestedThreadPtr, signalToRaise);
	if(signalToRaise == SIGUSR2 )
	{
		gWTPs[WTPID].isRequestClose = CW_TRUE;	
		syslog_wtp_log(WTPID, 0, "Critical Timer Expired");
		memset((char*)&msg, 0, sizeof(msg));
		msg.mqid = WTPID%THREAD_NUM+1;
		msg.mqinfo.WTPID = WTPID;
		msg.mqinfo.type = CONTROL_TYPE;
		msg.mqinfo.subtype = WTP_S_TYPE;
		msg.mqinfo.u.WtpInfo.Wtp_Op = WTP_REBOOT;
		msg.mqinfo.u.WtpInfo.WTPID = WTPID;
		if (msgsnd(WTPMsgqID, (msgq *)&msg, sizeof(msg.mqinfo), 0) == -1)
			perror("msgsnd");
		
	}else if(signalToRaise == SIGUSR1)
	{
	
		wid_syslog_debug_debug("Soft Timer Expired for Thread: %08x", (unsigned int)CWThreadSelf());
				
		if((!gWTPs[WTPID].isRetransmitting) || (gWTPs[WTPID].messages == NULL)) {
			wid_syslog_debug_debug("Soft timer expired but we are not retransmitting");
			return;
		}
	
		(gWTPs[WTPID].retransmissionCount)++;
		
		wid_syslog_debug_debug("Retransmission Count increases to %d", gWTPs[WTPID].retransmissionCount);
		
		if(gWTPs[WTPID].retransmissionCount >= gCWMaxRetransmit) 
		{
			wid_syslog_debug_debug("Peer is Dead");
			//?? _CWCloseThread(*iPtr);
			// Request close thread
			gWTPs[WTPID].isRequestClose = CW_TRUE;
	//		CWDownWTP(*iPtr);
			memset((char*)&msg, 0, sizeof(msg));
			msg.mqid = WTPID%THREAD_NUM+1;
			msg.mqinfo.WTPID = WTPID;
			msg.mqinfo.type = CONTROL_TYPE;
			msg.mqinfo.subtype = WTP_S_TYPE;
			msg.mqinfo.u.WtpInfo.Wtp_Op = WTP_REBOOT;
			msg.mqinfo.u.WtpInfo.WTPID = WTPID;
			if (msgsnd(WTPMsgqID, (msgq *)&msg, sizeof(msg.mqinfo), 0) == -1)
				perror("msgsnd");
	//		CWSignalThreadCondition(&gWTPs[*iPtr].interfaceWait);
			return;
		}else{
			memset((char*)&msg, 0, sizeof(msg));
			msg.mqid = WTPID%THREAD_NUM+1;
			msg.mqinfo.WTPID = WTPID;
			msg.mqinfo.type = CONTROL_TYPE;
			msg.mqinfo.subtype = WTP_S_TYPE;
			msg.mqinfo.u.WtpInfo.Wtp_Op = WTP_RESEND;
			msg.mqinfo.u.WtpInfo.WTPID = WTPID;
			if (msgsnd(WTPMsgqID, (msgq *)&msg, sizeof(msg.mqinfo), 0) == -1)
				perror("msgsnd");
		}		
	}

 //	CWDebugLog("Timer Expired, Sent Signal(%d) to Thread: %d", signalToRaise, requestedThreadPtr);

//	CW_FREE_OBJECT(a->requestedThreadPtr);
	CW_FREE_OBJECT(a);

	return;
}

CWBool CWTimerRequest(int sec, CWThread *threadPtr, CWTimerID *idPtr, int signalToRaise, int WTPID) {

	CWThreadTimerArg *arg;

	CWDebugLog("Timer Request");
	if(sec < 0 || threadPtr == NULL || idPtr == NULL) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	
	CW_CREATE_OBJECT_ERR(arg, CWThreadTimerArg, return CW_FALSE;);
//	CW_CREATE_OBJECT_ERR(arg->requestedThreadPtr, CWThread, CW_FREE_OBJECT(arg); return CW_FALSE;);
 //	CW_COPY_MEMORY(arg->requestedThreadPtr, threadPtr, sizeof(CWThread));
 	arg->signalToRaise = signalToRaise;
 	arg->WTPID = WTPID;
//	CWDebugLog("Timer Request: thread(%d), signal(%d)", *(arg->requestedThreadPtr), arg->signalToRaise);
	
	if ((*idPtr = timer_add(sec, 0, &CWHandleTimer, arg)) == -1) {

		return CW_FALSE;
	}

	return CW_TRUE;
}

