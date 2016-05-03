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

#include <ctype.h>
#include "CWWTP.h"
#include "WTPDriverInteraction.h"
#include "WTPFrameReceive.h"

#define DHCP_OPTION_AC_ADDR_FILE "/tmp/ac-addr"
#define DHCP_OPTION_AC_ADDR6_FILE "/tmp/ac-addr6"

#define WTP_BOARD_CONFIG_FILE 	"/etc/wlan/config.board"
#ifdef AQ2010  /*pei 080916*/
#define WTP_MODEL_DEFAULT     "2010"
#endif
#ifdef AQ1110T  /*pei 080916*/
#define WTP_MODEL_DEFAULT     "1110T"
#endif
#ifdef CAVIUM_PLATFORM  /*pei 081220*/
#define WTP_MODEL_DEFAULT     "2110"
#endif
#ifdef WNAP210 /*gzm 091225*/
#define WTP_MODEL_DEFAULT "210-N"
#endif
#ifdef WNAP350
#define WTP_MODEL_DEFAULT "350-N"
#endif

#define WTP_SERIAL_NUMBER_DEFAULT 654321
#define WTP_BOARD_ID_DEFAULT 654321
#define WTP_BOARD_REVISION_DEFAULT 10
#define TEMP_STR_LEN 64
#ifdef DMALLOC
#include "../dmalloc-5.5.0/dmalloc.h"
#endif
#if 1         //pei 0610
int gWatchDogTimerInterval = 5;
CWTimerID gCWWatchDogTimerID;
extern void *CWWTPWatchdog(void *arg);
CWStateTransition gtemp_state = -1;        //pei add 0616
int debug_print = 0;        //pei add 0827
char *successfile="/tmp/successful";
extern unsigned char gImageName[50];
extern CWProtocolResultCode imagedata_code;
unsigned char isBackup = 0;
#endif
int gEnabledLog;
int gMaxLogFileSize;
int gMaxWTPLogFileSize;
char gLogFileName[]=LOG_FILE_NAME;
char gWTPLogFileName[]=WTP_LOG_FILE_NAME;

// addresses of ACs for Discovery
char **gCWACAddresses;
int gCWACCount = 0;/*the count of AC*/

int gIPv4StatusDuplicate=0;
int gIPv6StatusDuplicate=0;

char *gWTPLocation = NULL;
char *gWTPName = NULL;

// if not NULL, jump Discovery and use this address for Joining
char *gWTPForceACAddress = NULL;
char *gWTPACDomainName = NULL;/*the domain name of the AC*/
char *gWTPSoftWareVersion = NULL;
char gWTPHardWareVersion[64];  //pei add for test hardversion 0214
char gWTPCodeVersion[64];  //pei add for test codeversion 090812
char versionbuf[100];
unsigned int gManuOption = WTP_OWN;
#ifndef CW_NO_DTLS
unsigned char gDtlsSecurity=0;     /* when set to 1 enable DTLS, 0 disable */
unsigned char gDtlsPolicy=0;  /* when gDtlsSecurity 0, meaningless. when gDtlsSecurity 1, 0 enable control DTLS disable data, 1 enable both */
#endif
CWAuthSecurity gWTPForceSecurity;

// network
CWSocket gWTPSocket;
//data channel
CWSocket gWTPDataSocket;

#ifndef CW_NO_DTLS//csc
CWSecurityContext gWTPSecurityContext;

CWSecuritySession gWTPSession;
CWSecurityContext gWTPDataSecurityContext; /* pei data dtls 20100510 */
CWSecuritySession gWTPDataSession; /* pei data dtls 20100510 */
#endif 
CWThreadMutex sta_table_mutex;

//
CWSafeList gFrameList;
CWSafeList gPacketReceiveList;
#ifndef CW_NO_DTLS
CWSafeList gPacketDataReceiveList;
#endif
CWThreadCondition gInterfaceWait;
CWThreadMutex gInterfaceMutex;

// infos about the ACs to discover
CWACDescriptor *gCWACList = NULL;
// infos on the better AC we discovered so far
CWACInfoValues *gACInfoPtr = NULL;


//WTP statistics timer
int gWTPStatisticsTimer=CW_STATISTIC_TIMER_DEFAULT;

WTPRebootStatisticsInfo gWTPRebootStatistics;
CWWTPRadiosInfo gRadiosInfo;

// path MTU of the current session
int gWTPPathMTU = 0;

int gWTPRetransmissionCount;

//List containg data about 802.11 Association Frame received

CWPendingRequestMessage gPendingRequestMsgs[MAX_PENDING_REQUEST_MSGS];	
// receive a message, that can be fragmented. This is useful not only for the Join State
/*****************************************
 *caosicong
 *led dev
 *
 * **************************************/
int leddev;
int blind_status;
/***************************************
*
*WTP board data this wiil get from the high sector flash lator
*now I write these in a file called config.board
*
***************************************/
char WTPModelNum[64];  //pei 0923  /* ap-code, pei 090519 */
char WTPRealModelNum[64];  /* real model num, pei 090519 */
int WTPSerialNum;
char WTPBoardID[WTP_BOARD_ID_LEN];
int WTPBoardRevesion;
unsigned char WTPBaseMAC[MAC_ADDR_LEN];
int	gIPMode;		//diaowenqin add ,to reduce the high cpu_rate caused by frequentely call CWGetIPMode() when gExtraInfoEnable = 1
/**************wlan********************/
//CWWTPWlan  wtp_wlan[MAX_WLAN_NUM_PER_WTP];
/**************************************/
CWWTPRadio *wtp_radio_list;
CWWTPWlan *wtp_wlan_list;
CWWTPSta *wtp_sta_list;

int wtp_wlan_count;/*the created wlan num */
int wtp_sta_count;
/*************************************************
*
*some signal record the statures
*
**************************************************/
CWBool CWWTPNeighborDeadTimerExpiredFlag ;
/*wireless receive frame thread num,now we just create a thread to receive all the wlans of  the ap 's frame  */
CWBool WTPWLanReceiveThreadRun ;
CWBool WTPDataChannelThreadRun ;
int receiveframethread = 0;/*if 1 the thread has created*/
char * trim(char *str_org);

CWConfigureStateRate gConfigureStateRate[2];
void dpf(const char *format, ...)
{
	if(debug_print){
		va_list args;
		va_start(args, format);
		vprintf(format,args);
		va_end(args);
	}
}

/***************************************************************************
 * 
 * Function:  CWWTPGetWlanName
 *
 * Purpose:  get the wlan name by local ID
 *
 * Inputs:  local ID
 *
 * Output:    wlan name
 *
 * Returns:   
 **************************************************************************/

void CWWTPGetWlanName(char *wlanname,unsigned char radioId, unsigned char wlanId)
{
	sprintf(wlanname,"ath.%d-%d", radioId, wlanId);
	return;
}

 CWBool PopenFile (char *cmd_str,char *str,int len )
 {
 	if(cmd_str == NULL ||str == NULL)
 		return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
 		
	 memset(str, 0, len);			
	 
	 FILE *fp=NULL;	
	 fp = popen(cmd_str, "r");	
	 if(fp)	
	 {		
		 fgets(str, len, fp);		
		 if(str[strlen(str)-1] == '\n')		
		 {			
		 	str[strlen(str)-1] = '\0';		
		 }		
		 pclose(fp);		
		 return CW_TRUE;	
	 }	
	 else
	 {		
		 CWWTPDebugLog("cmd:%s error!%s",cmd_str,strerror(errno));		
		 return CW_FALSE;	
	 }
 }

int get_file_len(FILE *tmp_fp)
{
	int tmp_len = 0;
	fseek(tmp_fp,0,SEEK_SET);
	tmp_len = ftell(tmp_fp);
	fseek(tmp_fp,0,SEEK_END);
	return tmp_len;
}

int GetOptionACIPCount(void)
{
	FILE *ip_fp;
	int i = 0;
	char tempIp[64];
	char *fileName=(gNetworkPreferredFamily == CW_IPv4)?DHCP_OPTION_AC_ADDR_FILE:DHCP_OPTION_AC_ADDR6_FILE;
	
	if((ip_fp = fopen(fileName, "r"))==NULL){
		CWWTPDebugLog("fopen %s is wrong or not exist!",fileName);
		return 0;
	}

	memset(tempIp,0,64);
	while(fgets(tempIp,sizeof(tempIp),ip_fp)){
		i++;
	}
	fclose(ip_fp);
	
	return i;
}
CWBool ReadIpFromTmp(int tmpACCount, unsigned char (*ip_addr)[64])
{
	FILE *ip_fp;
	int i = 0;
	char tempIp[64];

	if((ip_fp = fopen((gNetworkPreferredFamily == CW_IPv4)?DHCP_OPTION_AC_ADDR_FILE:DHCP_OPTION_AC_ADDR6_FILE, "r"))==NULL){
		CWWTPDebugLog("fopen Tmp/Ip file is wrong or not exist!");
		return CW_FALSE;
	}
	
	for(i=0;i<tmpACCount;i++)
	{
		memset(ip_addr[i],0,64);
	}

	i = 0;
	memset(tempIp,0,64);
	while(fgets(tempIp,sizeof(tempIp),ip_fp)){
		memcpy(ip_addr[i],tempIp,strlen(tempIp)-1);
		memset(tempIp,0,64);
		i++;
	}
	fclose(ip_fp);
	
	return CW_TRUE;
}

void CWWTPGetWifiName(char *radioname,int radio_id)
{
	sprintf(radioname,"wifi%d",radio_id);
	return;
}
/*****************************************************************

get the wlanID frome the sta MAC


*****************************************************************/
CWBool WTPGetwlanIDbyMAC(unsigned char *wlan_id,unsigned char *sta_mac)
{
	int i;
	int found = 0;
	CWWTPSta *ptr;
	if(*wlan_id != 'n')/*have got it*/{
		printf("WTPGetwlanIDbyMAC wlanid have found\n");
		return CW_TRUE;

	}
	printf("set key MAC:\n");
	for(i=0;i<6;i++){
		printf("%02x ",sta_mac[i]);
	}
	printf("\n");
	printf("wtp_sta[0].sta_mac:\n");
	for(i=0;i<6;i++){
		printf("%02x ",wtp_sta_list->sta_mac[i]);
	}
	printf("\n");
	for(ptr = wtp_sta_list ; ptr != NULL;ptr = ptr->next){
		if(macAddrCmp(sta_mac,ptr->sta_mac)==1){
			*wlan_id = ptr->wlan_id;
			printf(" wtp_sta[i].wlan_id %d\n", ptr->wlan_id);
			found = 1 ;
			break;
		}
	}
	if(found == 0)
		return CW_FALSE;
	else
		return CW_TRUE;

}
/*********************************************************
*
*   Purpose: Make wtpd to be a daemon process
*   Date   : 8/6/2011
*   Author : sundl
*
**********************************************************/
int CWWTPDaemon( int nochdir, int noclose)
{
#define	_PATH_DEVNULL	"/dev/null"
	int fd;
	switch (fork()) {
		case -1:
			return(-1);
		case 0:
			break;
		default:
			exit(0);
	}
	if (setsid() == -1)
		return(-1);

	if (fork())
		exit(0);

	if (!nochdir)
		chdir("/");
	
	umask(0);

	if (!noclose && (fd = open(_PATH_DEVNULL, O_RDWR, 0)) != -1) {
		dup2(fd, STDIN_FILENO);
		dup2(fd, STDOUT_FILENO);
		dup2(fd, STDERR_FILENO);
		if (fd > 2)
			close(fd);
	}
	return(0);
}

CWBool CWReceiveMessage(CWProtocolMessage *msgPtr) {
	CWList fragments = NULL;
	int readBytes;
	char buf[CW_BUFFER_SIZE];
	
	CW_REPEAT_FOREVER {
		CW_ZERO_MEMORY(buf, CW_BUFFER_SIZE);

#ifndef CW_NO_DTLS
		if(gDtlsSecurity == 1)
		{
			if(!CWSecurityReceive(gWTPSession, buf, CW_BUFFER_SIZE, &readBytes)) {return CW_FALSE;}
		}
		else
#endif
		{
			char *pkt_buffer = NULL;

			CWLockSafeList(gPacketReceiveList);

			while (CWGetCountElementFromSafeList(gPacketReceiveList) == 0)
				CWWaitElementFromSafeList(gPacketReceiveList);

			pkt_buffer = (char*)CWRemoveHeadElementFromSafeList(gPacketReceiveList, &readBytes);

			CWUnlockSafeList(gPacketReceiveList);

			CW_COPY_MEMORY(buf, pkt_buffer, readBytes);
			CW_FREE_OBJECT(pkt_buffer);
		}

		CWBool dataFlag = CW_FALSE;
		if(!CWProtocolParseFragment(buf, readBytes, &fragments, msgPtr, &dataFlag)) {/*parse the capwap header*/
			if(CWErrorGetLastErrorCode() == CW_ERROR_NEED_RESOURCE) { // we need at least one more fragment
				continue;
			} else { // error
				CWErrorCode error;
				error=CWErrorGetLastErrorCode();
				switch(error)
				{
					case CW_ERROR_SUCCESS: {CWDebugLog("ERROR: Success"); break;}
					case CW_ERROR_OUT_OF_MEMORY: {CWDebugLog("ERROR: Out of Memory"); break;}
					case CW_ERROR_WRONG_ARG: {CWDebugLog("ERROR: Wrong Argument"); break;}
					case CW_ERROR_INTERRUPTED: {CWDebugLog("ERROR: Interrupted"); break;}
					case CW_ERROR_NEED_RESOURCE: {CWDebugLog("ERROR: Need Resource"); break;}
					case CW_ERROR_COMUNICATING: {CWDebugLog("ERROR: Comunicating"); break;}
					case CW_ERROR_CREATING: {CWDebugLog("ERROR: Creating"); break;}
					case CW_ERROR_GENERAL: {CWDebugLog("ERROR: General"); break;}
					case CW_ERROR_OPERATION_ABORTED: {CWDebugLog("ERROR: Operation Aborted"); break;}
					case CW_ERROR_SENDING: {CWDebugLog("ERROR: Sending"); break;}
					case CW_ERROR_RECEIVING: {CWDebugLog("ERROR: Receiving"); break;}
					case CW_ERROR_INVALID_FORMAT: {CWDebugLog("ERROR: Invalid Format"); break;}
					case CW_ERROR_TIME_EXPIRED: {CWDebugLog("ERROR: Time Expired"); break;}
					case CW_ERROR_NONE: {CWDebugLog("ERROR: None"); break;}
				}
				CWDebugLog("~~~~~~");
				return CW_FALSE;
			}
		} else break; // the message is fully reassembled
	}
	
	return CW_TRUE;
}
#ifndef CW_NO_DTLS
CWBool CWReceiveDataMessage(CWProtocolMessage *msgPtr)  /* pei data dtls 20100510 */
{
	CWList fragments = NULL;
	int readBytes;
	char buf[CW_BUFFER_SIZE];
	
	CW_REPEAT_FOREVER {
		CW_ZERO_MEMORY(buf, CW_BUFFER_SIZE);

		if((gDtlsSecurity == 1)&&(gDtlsPolicy == 1))
		{
			if(!CWSecurityReceive(gWTPDataSession, buf, CW_BUFFER_SIZE, &readBytes)) {return CW_FALSE;}
		}
		else
		{
			char *pkt_buffer = NULL;

			CWLockSafeList(gPacketDataReceiveList);

			while (CWGetCountElementFromSafeList(gPacketDataReceiveList) == 0)
				CWWaitElementFromSafeList(gPacketDataReceiveList);

			pkt_buffer = (char*)CWRemoveHeadElementFromSafeList(gPacketDataReceiveList, &readBytes);

			CWUnlockSafeList(gPacketDataReceiveList);

			CW_COPY_MEMORY(buf, pkt_buffer, readBytes);
			CW_FREE_OBJECT(pkt_buffer);
		}

		CWBool dataFlag = CW_TRUE;
		if(!CWProtocolParseFragment(buf, readBytes, &fragments, msgPtr, &dataFlag)) {/*parse the capwap header*/
			if(CWErrorGetLastErrorCode() == CW_ERROR_NEED_RESOURCE) { // we need at least one more fragment
				continue;
			} else { // error
				CWErrorCode error;
				error=CWErrorGetLastErrorCode();
				switch(error)
				{
					case CW_ERROR_SUCCESS: {CWDebugLog("ERROR: Success"); break;}
					case CW_ERROR_OUT_OF_MEMORY: {CWDebugLog("ERROR: Out of Memory"); break;}
					case CW_ERROR_WRONG_ARG: {CWDebugLog("ERROR: Wrong Argument"); break;}
					case CW_ERROR_INTERRUPTED: {CWDebugLog("ERROR: Interrupted"); break;}
					case CW_ERROR_NEED_RESOURCE: {CWDebugLog("ERROR: Need Resource"); break;}
					case CW_ERROR_COMUNICATING: {CWDebugLog("ERROR: Comunicating"); break;}
					case CW_ERROR_CREATING: {CWDebugLog("ERROR: Creating"); break;}
					case CW_ERROR_GENERAL: {CWDebugLog("ERROR: General"); break;}
					case CW_ERROR_OPERATION_ABORTED: {CWDebugLog("ERROR: Operation Aborted"); break;}
					case CW_ERROR_SENDING: {CWDebugLog("ERROR: Sending"); break;}
					case CW_ERROR_RECEIVING: {CWDebugLog("ERROR: Receiving"); break;}
					case CW_ERROR_INVALID_FORMAT: {CWDebugLog("ERROR: Invalid Format"); break;}
					case CW_ERROR_TIME_EXPIRED: {CWDebugLog("ERROR: Time Expired"); break;}
					case CW_ERROR_NONE: {CWDebugLog("ERROR: None"); break;}
				}
				CWDebugLog("~~~~~~");
				return CW_FALSE;
			}
		} else break; // the message is fully reassembled

	}

	return CW_TRUE;
}
#endif

CWBool CWWTPSendAcknowledgedPacket(int seqNum, CWList msgElemlist, CWBool (assembleFunc) (CWProtocolMessage **, int *, int, int, CWList), CWBool (parseFunc) (char*, int, int, void*), CWBool (saveFunc) (void*), void *valuesPtr) {
	CWProtocolMessage *messages = NULL;
	CWProtocolMessage msg;
	int fragmentsNum = 0, i;
//	struct timeval timeout, before, after, delta, newTimeout;
	struct timespec timewait;
	
	int gTimeToSleep = gCWRetransmitTimer;
	int gMaxTimeToSleep = CW_ECHO_INTERVAL_DEFAULT/2;

	msg.msg = NULL;
	
	if(!(assembleFunc(&messages, &fragmentsNum, gWTPPathMTU, seqNum, msgElemlist))) {
		CWDebugLog("assemble failer\n");
		CWLog("assemble failed.");
		goto cw_failure;
	}
	
	gWTPRetransmissionCount= 0;
	
	while(gWTPRetransmissionCount < gCWMaxRetransmit) 
	{
//		CWDebugLog("Transmission Num:%d", gWTPRetransmissionCount);
		for(i = 0; i < fragmentsNum; i++) 
		{

#ifndef CW_NO_DTLS
			if(gDtlsSecurity == 1)
			{
				if(!CWSecuritySend(gWTPSession, messages[i].msg, messages[i].offset))
				{
					CWDebugLog("Failure sending Request");
					CWLog("Failure (dtls) sending Request.");
					goto cw_failure;
				}
			}
			else
#endif
			{
				if (!CWNetworkSendUnsafeUnconnected(gWTPSocket, &(gACInfoPtr->preferredAddress), messages[i].msg, messages[i].offset))
				{
					CWDebugLog("Failure sending Request");
					CWLog("Failure sending Request.");
					goto cw_failure;
				}
			}

		}
		
		timewait.tv_sec = time(0) + gTimeToSleep;
		timewait.tv_nsec = 0;

//		gettimeofday(&before, NULL);

		CW_REPEAT_FOREVER 
		{
			// CWDebugLog("Before Select");
			// check if something is available to read until newTimeout
			//if(CWNetworkTimedPollRead(gWTPSocket, &newTimeout)) { // success
				// if there was no error, raise a "success error", so we can easily handle
				// all the cases in the switch
				// CWDebugLog("Raise Success");
			//	CWErrorRaise(CW_ERROR_SUCCESS, NULL);
			//}
	
			// Wait packet

			CWThreadMutexLock(&gInterfaceMutex);

			if (CWGetCountElementFromSafeList(gPacketReceiveList) > 0)
				CWErrorRaise(CW_ERROR_SUCCESS, NULL);
			else
			{
				if (CWErr(CWWaitThreadConditionTimeout(&gInterfaceWait, &gInterfaceMutex, &timewait)))
					CWErrorRaise(CW_ERROR_SUCCESS, NULL);
			}

			CWThreadMutexUnlock(&gInterfaceMutex);
//			printf("CWErrorGetLastErrorCode()= %d \n",CWErrorGetLastErrorCode());
			//
			switch(CWErrorGetLastErrorCode()) 
			{
				case CW_ERROR_TIME_EXPIRED:
				{
					gWTPRetransmissionCount++;
					goto cw_continue_external_loop;
					break;
				}

				case CW_ERROR_SUCCESS:
				{
					// there's something to read
//					printf("case CW_ERROR_SUCCESS: \n");
					if(!(CWReceiveMessage(&msg))) 
					{
						CW_FREE_PROTOCOL_MESSAGE(msg);
						CWDebugLog("Failure Receiving Response");
						CWLog("Failure Receiving Response.");
						goto cw_failure;
					}
					
					if(!(parseFunc(msg.msg, msg.offset, seqNum, valuesPtr))) 
					{
						if(CWErrorGetLastErrorCode() != CW_ERROR_INVALID_FORMAT) 
						{
							CW_FREE_PROTOCOL_MESSAGE(msg);
							CWDebugLog("Failure Parsing Response");
							CWLog("Failure Parsing Response.");
							goto cw_failure;
						}
						else 
						{
							CWErrorHandleLast();
							//gettimeofday(&after, NULL);
							//CWTimevalSubtract(&delta, &after, &before);	
							//if(CWTimevalSubtract(&newTimeout, &timeout, &delta) == 1) 
							{ 
								// negative delta: time is over
								gWTPRetransmissionCount++;
								goto cw_continue_external_loop;
							}

							break;
						}
					}
					
					if((saveFunc(valuesPtr))) 
					{
						goto cw_success;
					} 
					else 
					{
						if(CWErrorGetLastErrorCode() != CW_ERROR_INVALID_FORMAT) 
						{
							CW_FREE_PROTOCOL_MESSAGE(msg);
							CWDebugLog("Failure Saving Response");
							CWLog("Failure Saving Response.");
							goto cw_failure;
						} 
					}

					break;
				}

				case CW_ERROR_INTERRUPTED: // something to read OR interrupted by the system
				{
					// wait for the remaining time (NetworkPoll will be recalled with the remaining time)
					//gettimeofday(&after, NULL);
					//CWTimevalSubtract(&delta, &after, &before);
					//if(CWTimevalSubtract(&newTimeout, &timeout, &delta) == 1) 
					{ 
						// negative delta: time is over
						gWTPRetransmissionCount++;
						goto cw_continue_external_loop;
					}

					break;
				}
					
				default:
				{
					CWErrorHandleLast();
					CWDebugLog("Failure");
					CWLog("Failure:%s,case branch is default.",__func__);
					goto cw_failure;
					break;
				}
			}
		}
		
		cw_continue_external_loop:
			CWDebugLog("Retransmission time is over");
			
			gTimeToSleep<<=1;
			if ( gTimeToSleep > gMaxTimeToSleep ) gTimeToSleep = gMaxTimeToSleep;
	}

	// too many retransmissions
	return CWErrorRaise(CW_ERROR_NEED_RESOURCE, "Peer Dead");
	
cw_success:	
	for(i = 0; i < fragmentsNum; i++) {
		CW_FREE_PROTOCOL_MESSAGE(messages[i]);
	}
	
	CW_FREE_OBJECT(messages);
	CW_FREE_PROTOCOL_MESSAGE(msg);
	
	return CW_TRUE;
	
cw_failure:
	if(messages != NULL) {
		for(i = 0; i < fragmentsNum; i++) {
			CW_FREE_PROTOCOL_MESSAGE(messages[i]);
		}
		CW_FREE_OBJECT(messages);
	}
	CWDebugLog("Failure");
	return CW_FALSE;
}

/*
*diaowq add @2010/05/04
*for open/close debug_print,when wtpd is running.
*/
#define SIG_DEBUG_SWITCH SIGUSR1+1
void change_debug_print_status(int sig_num,siginfo_t *info,void *myact)
{
	int sigval_int = info->si_int;
	int logon_type_serial = sigval_int >= 10 ?0:1;
	sigval_int = sigval_int >= 10 ?sigval_int-10:sigval_int;

	printf("\nwtpd get signal to change debug_print status.\n");
	printf("old debug_print=%d\nsiginfo->si_int:%d\n",debug_print,sigval_int);
	if(debug_print != 0){
		//freopen("/dev/null","w",stdout);   
		debug_print = 0;
		printf("new debug_print=%d\n",debug_print);
		printf("debug_print switch is closed.\n");
	}else{
		char str_dev[32];
		if(logon_type_serial)
			sprintf(str_dev,"/dev/ttyS%d",sigval_int);
		else
			sprintf(str_dev,"/dev/pts/%d",sigval_int);
		debug_print = 1;
		printf("new debug_print=%d\n",debug_print);
		printf("debug_print switch is opened.\nstdout is redirected to %s\n",str_dev);
		freopen(str_dev,"w",stdout);   
	}
}

/***************************************************************************
 * 
 * Function:   main
 *
 * Purpose:    capwap main func
 *
 * Inputs:     ??
 *
 * Output:    
 *
 * Returns:   
 **************************************************************************/

int main (int argc, char * argv[]) {

	//signal(SIG_DEBUG_SWITCH,func_change_debug_print); 
/*	struct sigaction sig_act;
	sigemptyset(&sig_act.sa_mask);
	sig_act.sa_sigaction = change_debug_print_status;
	sig_act.sa_flags = SA_SIGINFO;
	if(sigaction(SIG_DEBUG_SWITCH,&sig_act,NULL)<0){
		printf("install signal failed.");
	}
*/	
	CWStateTransition nextState = CW_ENTER_DISCOVERY;
#ifdef AQ2010
	blind_status =1;
#endif
	CWWTPNeighborDeadTimerExpiredFlag = CW_FALSE;
	WTPWLanReceiveThreadRun = CW_FALSE;
	WTPDataChannelThreadRun = CW_FALSE;
	
	gMaxLogFileSize = 3000;
	gMaxWTPLogFileSize = 300000; 
	memset(gWlanIDMap,-1,sizeof(gWlanIDMap));
	
	int opt = 0;
	//use option "ebug",del warning,when start like "wtpd -debug &"
	while((opt = getopt(argc,argv,"debugVB"))  !=  -1){
		switch(opt)
		{
			case 'd':
				debug_print = 1;
				break;
			case 'V':
				gEnabledLog = 1;//keep log info of hot-start is fully.
				isBackup = 1;
				break;
			case 'B':
				daemon(0, 0);
				break;
			default://for option "ebug" 
				break;
		}

	}
	
	dpf("debug_print has been open!\n");

	if(!isBackup)
	{
	/* turn on the blind and turn the blue ligth */
#if (defined(AQ2010)) /*pei 080903  because aq1200, down led control*/
	if(blind_status == 0){
		 	_system("wrcpuutil -u");
	}
#elif (defined(CAVIUM_PLATFORM))
		_system("avrutil -t");
#else
		_system("/usr/sbin/setled sysled blink");
#endif
	}

	if(!isBackup)
		CWSetThinapState(0);
	else
		CWSetThinapCheckTimer();

	if(!isBackup)
	{
#ifdef CW_NO_DTLS
		/*csc load config.wtp file */
		if( !CWErr(CWWTPLoadConfiguration()) )
#else
		if( !CWErr(CWSecurityInitLib())	|| !CWErr(CWWTPLoadConfiguration()) )
#endif
		{
			CWLog("CWWTPLoadConfiguration error Can't start WTP... Lose AC.");
			{
				CWac_lose();
				CWSysReboot();
			}
		}
	}

	/* init log file */
	CWLogInitFile(LOG_FILE_NAME);
	CWWTPLogInitFile(WTP_LOG_FILE_NAME);
	#ifndef CW_SINGLE_THREAD
		CWDebugLog("Use Threads");
	#else
		CWDebugLog("Don't Use Threads");
	#endif

	CWErrorHandlingInitLib();

	/* Capwap receive packets list */
	if (!CWErr(CWCreateSafeList(&gPacketReceiveList)))
	{
		CWLog("init receving list of control packets failed.");
		{
			CWac_lose();
			CWSysReboot();
		}
	}
#ifndef CW_NO_DTLS
	if (!CWErr(CWCreateSafeList(&gPacketDataReceiveList))) /* pei data dtls 20100510 */
	{
		CWLog("init receving list of data packets failed.");
		{
			CWac_lose();
			CWSysReboot();
		}
	}
#endif
	/* wireless frame list truly we do'nt need it now */
	if (!CWErr(CWCreateSafeList(&gFrameList)))
	{
		CWLog("Can't start WTP... Lose AC.");
		{
			CWac_lose();
			CWSysReboot();
		}
	}
	//
	CWCreateThreadMutex(&gInterfaceMutex);
	CWSetMutexSafeList(gPacketReceiveList, &gInterfaceMutex);
#ifndef CW_NO_DTLS
	CWSetMutexSafeList(gPacketDataReceiveList, &gInterfaceMutex); /* pei data dtls 20100510 */
#endif
	CWSetMutexSafeList(gFrameList, &gInterfaceMutex);
	CWCreateThreadCondition(&gInterfaceWait);
	CWSetConditionSafeList(gPacketReceiveList, &gInterfaceWait);
#ifndef CW_NO_DTLS
	CWSetConditionSafeList(gPacketDataReceiveList, &gInterfaceWait); /* pei data dtls 20100510 */
#endif
	CWSetConditionSafeList(gFrameList, &gInterfaceWait);
	//
	
	CWCreateThreadMutex(&sta_table_mutex);
	CWRandomInitLib();

	CWThreadSetSignals(SIG_BLOCK, 1, SIGALRM);

	if (timer_init() == 0) {
		CWLog("Can't init timer module... Lose AC.");
		{
			CWac_lose();
			CWSysReboot();
		}
	}

	if(isBackup)
		CWWTPDebugLog("WTPD hot restart......");
	
	if(!isBackup)
	/* load WTP board configuration */	
	{
		if(!CWErr(CWWTPLoadBoardConfiguration()) ){
			CWLog("WTP board configuration Can't load!... Lose AC.");
			{
				CWac_lose();
				CWSysReboot();
			}
		}


		/* load version */
		if(!CWErr(CWWTPLoadVersionConfiguration()) ){
			CWLog("WTP load version error... Lose AC.");
			{
				CWac_lose();
				CWSysReboot();
			}
		}
		
		ConfArg confArg;
		memset(&confArg,0,sizeof(ConfArg));
		strcpy(confArg.conf_arg,"gCWForceMTU");
		sprintf(confArg.conf_value,"%d",gCWForceMTU);
		CWSaveConfigInfo(CONF_WTPD,confArg,CW_TRUE);
		
		memset(&confArg,0,sizeof(ConfArg));
		strcpy(confArg.conf_arg,"gEnabledLog");
		sprintf(confArg.conf_value,"%d",gEnabledLog);
		CWSaveConfigInfo(CONF_WTPD,confArg,CW_TRUE);

		memset(&confArg,0,sizeof(ConfArg));
		strcpy(confArg.conf_arg,"gMaxLogFileSize");
		sprintf(confArg.conf_value,"%d",gMaxLogFileSize);
		CWSaveConfigInfo(CONF_WTPD,confArg,CW_TRUE);

		memset(&confArg,0,sizeof(ConfArg));
		strcpy(confArg.conf_arg,"WTPModelNum");
		sprintf(confArg.conf_value,"%s",WTPModelNum);
		CWSaveConfigInfo(CONF_WTPD,confArg,CW_TRUE);

		memset(&confArg,0,sizeof(ConfArg));
		strcpy(confArg.conf_arg,"WTPRealModelNum");
		sprintf(confArg.conf_value,"%s",WTPRealModelNum);
		CWSaveConfigInfo(CONF_WTPD,confArg,CW_TRUE);

		memset(&confArg,0,sizeof(ConfArg));
		strcpy(confArg.conf_arg,"gIPMode");
		sprintf(confArg.conf_value,"%d",gIPMode);
		CWSaveConfigInfo(CONF_WTPD,confArg,CW_FALSE);		
		
	}
	
	/* init radio table */
	wtp_wlan_count = 0;
	wtp_sta_count = 0;
	wtp_sta_list = NULL;
	wtp_wlan_list = NULL;
	wtp_radio_list = NULL;
	if(!isBackup){
		CWDebugLog("WTPModelNum:%s", WTPModelNum);  //pei 0923
		CWWTPDebugLog("AP Code :%s", WTPModelNum);
	}
#ifndef CAVIUM_PLATFORM
	/* pei add for close bug process(Z) at 090624 */
	{
		struct sigaction sa;
		sa.sa_handler = SIG_IGN;
			#ifdef SA_NOCLDWAIT
		sa.sa_flags = SA_NOCLDWAIT;
			#else
			sa.sa_flags = 0;
			#endif
		sigemptyset(&sa.sa_mask);
		sigaction(SIGCHLD, &sa, NULL);
	}
#endif

	
/**************************************************
**************init radio******************************
****bind  the ath interface of a ap to the wtpd program *****/	
/**************************************************
	init configuration include the bind configuration
***************************************************/
/*
	CWDebugLog("Init WTP Radio Info");
	if(!CWWTPInitConfiguration())
	{
		CWLog("Error Init Configuration");
		exit(1);
	}
*/

	if(isBackup)
	{
		nextState = CW_ENTER_RUN;
		CWWTPDebugLog("WTPD hot restart......Enter run state");
	}
	else
	{
		if(gWTPForceACAddress != NULL) {
			// jump Discovery and use this address for Joining
			nextState = CW_ENTER_JOIN;
		}
		CWLog("THE AP started for a time");
		CWWTPDebugLog("THE AP started for a time");
	}
	/* start CAPWAP state machine */
	CW_REPEAT_FOREVER {

		switch(nextState) {
			case CW_ENTER_DISCOVERY:
				nextState = CWWTPEnterDiscovery();
				break;
			case CW_ENTER_SULKING:
				nextState = CWWTPEnterSulking();
				break;
			case CW_ENTER_JOIN:
				nextState = CWWTPEnterJoin();
				break;
			case CW_ENTER_IMAGE_DATA:                           //pei add 0616
				nextState = CWWTPEnterImageData();
				break;
			case CW_ENTER_CONFIGURE:
				nextState = CWWTPEnterConfigure();
				break;	
			case CW_ENTER_DATA_CHECK:
				nextState = CWWTPEnterDataCheck();
				break;	
			case CW_ENTER_RUN:
				nextState = CWWTPEnterRun();
				break;
			case CW_ENTER_RESET:
				CWDebugLog("######### WTP enters in RESET State #########");
				nextState = CWWTPEnterReset();
				break;
			case CW_QUIT:
				CWLog("enter CW_QUIT reboot... Lose AC.");
				CWWTPDebugLog("enter CW_QUIT reboot... Lose AC.");
				{
					printf("=== lose ac ===\n");
					if(WTPGetApRebootFlag())
					{
						CWac_lose();
					}
					else
					{
						WTPInitConfig();
						_system("wtpd > /dev/null 2>&1 &");
						exit(0);
					}					
				}
				CWWTPDestroy();
				return 0;
			default:
				CWDebugLog("Error state!");
		}
	}
}

__inline__ unsigned int CWGetSeqNum() {
	static unsigned int seqNum = 0;
	
	if (seqNum==CW_MAX_SEQ_NUM) seqNum=0;
	else seqNum++;
	return seqNum;
}

__inline__ int CWGetFragmentID() {
	static int fragID = 0;
	return fragID++;
}
CWBool CWGetBcast(char *bcast)
{
	char cmd_str[128];
	char str[24];
	char str_tmp[24];

	memset(cmd_str, 0, 128);
	sprintf(cmd_str, "/sbin/ifconfig default | awk -F \" \" '/Bcast/ {print $3}' | awk -F \":\" '{print $2}'");
	PopenFile(cmd_str,str_tmp,sizeof(str_tmp));
	memset(str, 0, 24);
	strcpy(str, trim(str_tmp));
//		dpf("str: %s len:%d\n", str, (int)strlen(str));
	strcpy(bcast, str);
	CWWTPDebugLog("Bcast: %s len:%d", bcast, (int)strlen(bcast));
	return CW_TRUE;
}
// parses config file and inits WTP configuration
CWBool CWWTPLoadConfiguration() {
	int i,j,m=0;
	char **DNSACAddresses;
	char **TmpIpAddress;
	int DNSACCount = 0;
	struct hostent *host;
	char **h_addr_list;
	char charip[64];
	int TmpACCount = 0;
	char str[64];
	//unsigned long int ip;
	//unsigned long int *x;
	char Bcast[24];
	CWDebugLog("WTP Loads Configuration");
	
	if(!CWErr(CWParseConfigFile())) { // get saved preferences
		CWDebugLog("Can't Read Config File");
		CWLog("Can't Read Config File");
		exit(1);
	}

	if(!isBackup)
	{
		/* wait for dhcp return for getting option43 AC-IP from /tmp/ac-addr, check /tmp/dhcpflag is exist or not. */
		gIPMode = CWGetIPMode();
		if(gIPMode)
		{
			while(access("/tmp/dhcpflag", R_OK)==-1)
			{
				sleep(3);
				CWWTPDebugLog("Dhcp hasn't return!Waiting...");
			}
		}
	}
/**************************************************************
*	2008-5-8 caosicong
*	add the AClist a address
*	we have got AC address from config.wtp
*     we add a dns addr of the ac to the acList
*
***************************************************************/
	if((gWTPACDomainName != NULL) && (isBackup == 0))
	{
		CWDebugLog("get the ac domain name: %s", gWTPACDomainName);
		CWWTPDebugLog("get the ac domain name: %s", gWTPACDomainName);

		if(gNetworkPreferredFamily == CW_IPv6)
		{
			CWSaveACAddrFromDNS(gWTPACDomainName);
			DNSACCount = CWGetACCountFromDNS(gWTPACDomainName);
			CWWTPDebugLog("the ipv6 DNSACCount is :%d",DNSACCount);
			if(DNSACCount!=0){
				CW_CREATE_ARRAY_ERR(DNSACAddresses, DNSACCount, char*, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
			}
			
			for(i=0;i<DNSACCount;i++)
			{
				CWGetACAddrFromDNS(gWTPACDomainName,i+1,str);
				strcpy(charip, trim(str));
				CW_CREATE_STRING_FROM_STRING_ERR(DNSACAddresses[i], charip, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
				CWWTPDebugLog("Init Configuration by DNS for AC at IPv6 : %s", DNSACAddresses[i]);
			}
		}
		else
		{
			host = gethostbyname(gWTPACDomainName);
			if(host==NULL) {	
				CWDebugLog("DNS No Information found\n");
				CWWTPDebugLog("DNS No Information found");
			}
			else{
				CWDebugLog("host find \n");
				CWWTPDebugLog("official hostname:%s", host->h_name);
				h_addr_list = host->h_addr_list;
				for (i = 0; h_addr_list[i]; i++){
					DNSACCount++;
				}
				CW_CREATE_ARRAY_ERR(DNSACAddresses, DNSACCount, char*, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
			
				for (i = 0;i<DNSACCount; i++){
#if 1 /* pei add 090203 */
					memset(str, 0, 64);
					sprintf(charip, "%s", inet_ntop(host->h_addrtype, h_addr_list[i], str, sizeof(str)));
					CWWTPDebugLog("dns-ACAddress:%s", charip);
#else
					x = (unsigned long int *)h_addr_list[i];
					ip = ntohl(*x);
					split[0] = (ip & 0xff000000) >> 24;
					split[1] = (ip & 0x00ff0000) >> 16;
					split[2] = (ip & 0x0000ff00) >> 8;
					split[3] = (ip & 0x000000ff);
					CWDebugLog("\n%d.%d.%d.%d\n", split[0], split[1], split[2], split[3]);
					sprintf(charip,"%d.%d.%d.%d", split[0], split[1], split[2], split[3]);
#endif
					CW_CREATE_STRING_FROM_STRING_ERR(DNSACAddresses[i], charip, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
		/*
					CW_CREATE_STRING_FROM_STRING_ERR(gCWACList[gCWACCount].address, charip, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
					CWDebugLog("Init Configuration for AC at %s", gCWACList[gCWACCount].address);
		*/
				}		
			}
		}
	}
	CWDebugLog("gCWACCount %d",gCWACCount);

	TmpACCount = GetOptionACIPCount();
	CWWTPDebugLog("Dhcp Option AC IP Count is :%d", TmpACCount);
	if(TmpACCount != 0)
	{
		FILE *ip_fp;
		CW_CREATE_ARRAY_ERR(TmpIpAddress, TmpACCount, char*, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););

		if((ip_fp = fopen((gNetworkPreferredFamily == CW_IPv4)?DHCP_OPTION_AC_ADDR_FILE:DHCP_OPTION_AC_ADDR6_FILE, "r"))==NULL){
			CWLog("OptionACcount=%d,but cannot open option43 ac_addr file.",TmpACCount);
 			return CW_FALSE;
		}
		i = 0;
		memset(charip, 0, 64);
		while(fgets(charip, sizeof(charip), ip_fp)){
			if(charip[strlen(charip)-1] == '\n')
			{
				charip[strlen(charip)-1] = '\0';
			}
			CW_CREATE_STRING_FROM_STRING_ERR(TmpIpAddress[i], charip, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
			memset(charip, 0, 64);
			i++;
		}
		fclose(ip_fp);

		CW_CREATE_ARRAY_ERR(gCWACList, gCWACCount+DNSACCount+TmpACCount, CWACDescriptor, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
		for(m=0;m<TmpACCount;m++){
			CW_CREATE_STRING_FROM_STRING_ERR(gCWACList[m].address, TmpIpAddress[m], return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
			CWWTPDebugLog("Init Configuration by Dhcp for AC at %s",gCWACList[m].address);
		}
	}
	else
		CW_CREATE_ARRAY_ERR(gCWACList, gCWACCount+DNSACCount, CWACDescriptor, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););

	for(j = 0; j < DNSACCount; j++) {
		CWDebugLog("Init Configuration for AC at %s", DNSACAddresses[j]);
		CWWTPDebugLog("Init Configuration by DNS for AC at %s", DNSACAddresses[j]);
		
		CW_CREATE_STRING_FROM_STRING_ERR(gCWACList[m+j].address, DNSACAddresses[j], return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	}
	for(i = 0; i < gCWACCount; i++) {
		CWDebugLog("Init Configuration by config.wtp for AC at %s", gCWACAddresses[i]);
		CWWTPDebugLog("Init Configuration by config.wtp for AC at %s", gCWACAddresses[i]);
		
		CW_CREATE_STRING_FROM_STRING_ERR(gCWACList[m+j+i].address, gCWACAddresses[i], return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	}

#if 0
	/*******rbk new add for dhcp getting AC addr*******/
	TmpACCount = ReadIpFromTmp(TmpIpAddress);
	if(TmpACCount!=0){
		CW_CREATE_ARRAY_ERR(gCWACList, gCWACCount+DNSACCount+TmpACCount, CWACDescriptor, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
		TmpIpAddressTag = 1;
	}
	else{
		CW_CREATE_ARRAY_ERR(gCWACList, gCWACCount+DNSACCount, CWACDescriptor, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	}
	/*******end*******/
	
	for(i = 0; i < gCWACCount; i++) {
		CWDebugLog("Init Configuration for AC at %s", gCWACAddresses[i]);
		dpf("Init Configuration for AC at %s\n", gCWACAddresses[i]);
		
		CW_CREATE_STRING_FROM_STRING_ERR(gCWACList[i].address, gCWACAddresses[i], return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	}
	for(j = 0; j < DNSACCount; j++) {
		CWDebugLog("Init Configuration for AC at %s", DNSACAddresses[j]);
		dpf("Init Configuration by DNS for AC at %s\n", DNSACAddresses[j]);
		
		CW_CREATE_STRING_FROM_STRING_ERR(gCWACList[i+j].address, DNSACAddresses[j], return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	}

	/*******rbk new add for dhcp getting AC addr*******/
	if(TmpIpAddressTag == 1){
		for(m=0;m<TmpACCount;m++){
			CW_CREATE_STRING_FROM_STRING_ERR(gCWACList[i+j+m].address, TmpIpAddress[m], return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
			dpf("Init Configuration by Dhcp for AC at %s\n",gCWACList[i+j+m].address);
		}
		
	}else{
		dpf("ip file is can't open\n");
	}
	/*******end*******/
#endif
	if(gCWACCount>0)
		CW_FREE_OBJECTS_ARRAY(gCWACAddresses, gCWACCount);
	if(DNSACCount>0)
		CW_FREE_OBJECTS_ARRAY(DNSACAddresses, DNSACCount);
	if(TmpACCount>0)
		CW_FREE_OBJECTS_ARRAY(TmpIpAddress, TmpACCount);
	gCWACCount += DNSACCount+TmpACCount;
/*	if(TmpIpAddressTag == 1){
		gCWACCount += TmpACCount;//rbk new  add
	}*/

//add bcast after insert ac address to ACList.  yuan add ... 
	for(i=0;i<gCWACCount;i++){
		int is_add=0;
		if(strcmp(gCWACList[i].address,"255.255.255.255")==0){
			int k;
			CWACDescriptor *re_gCWACList=NULL;
			CWGetBcast(Bcast);
			CW_CREATE_ARRAY_ERR(re_gCWACList,gCWACCount+1, CWACDescriptor, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
			CW_CREATE_STRING_FROM_STRING_ERR(re_gCWACList[i].address, Bcast, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
			CWWTPDebugLog("Add Bcast to AC list:%s",Bcast);
			gCWACCount+=1;
			for(k=0;k<i;k++){
				CW_CREATE_STRING_FROM_STRING_ERR(re_gCWACList[k].address, gCWACList[k].address, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
			}
			for(k=i+1;k<gCWACCount;k++){
				CW_CREATE_STRING_FROM_STRING_ERR(re_gCWACList[k].address, gCWACList[k-1].address, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
			}
			for(i = 0; i < gCWACCount-1; i++) {
			CW_FREE_OBJECT(gCWACList[i].address);
			}
			CW_FREE_OBJECT(gCWACList);
			gCWACList=re_gCWACList;
			re_gCWACList=NULL;
			is_add=1;
		}
		if(is_add==1)
			break;
	}
	CWDebugLog("gCWACCount %d",gCWACCount);
	CWWTPDebugLog("Init Configuration Get AC Sum Count:%d.", gCWACCount);
	if(gCWACCount == 0)
	{
		gCWACCount = 1;
		char DefaultACAddr[20]="255.255.255.255";
		CW_CREATE_STRING_FROM_STRING_ERR(gCWACList[0].address,DefaultACAddr, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
		CWWTPDebugLog("not ac-addr in the file of config.wtp,wtpd will use the default addr 255.255.255.255");
	}
		//return CWErrorRaise(CW_ERROR_NEED_RESOURCE, "No AC Configured");

	for(i = 0; i < gCWACCount; i++) {
		CWWTPDebugLog("  %d\t%s",i+1,gCWACList[i].address);
		gCWACList[i].received = CW_FALSE;
	}
	
	return CW_TRUE;
}

/*****************************************
*caosicong 2008 05 20
*get the sysinfo frome the flash
*
*****************************************/

char * trim(char *str_org)
{
	char *str_dst1 = str_org;
	char *str_dst2 = str_org+strlen(str_org)-1; 
	if(NULL == str_org)
	{
		CWWTPDebugLog("parameter is NULL");
	}
	while( *str_dst1 == ' ' || *str_dst1 == '\t' )
	{
		str_dst1++;
		if(*str_dst1 == '\0')
		return NULL;
	}
	while( *str_dst2 == ' ' || *str_dst2 == '\t' )
	{
		str_dst2--;
	}
	*(str_dst2+1) = '\0';
	return str_dst1;
}

CWBool CWWTPLoadBoardConfiguration() {
#if (defined(AQ2010))
	char temp_str[100];
	char str_tmp[64];
	unsigned char str[TEMP_STR_LEN];
	int a,b,c,d,e,f;
	memset(temp_str, 0, 100);
	memset(str_tmp, 0 ,64);
#ifdef NETGEAR
	sprintf(temp_str, "/usr/sbin/showsysinfo | awk -F \":\" '/MAC/ {print $2\":\"$3\":\"$4\":\"$5\":\"$6\":\"$7}'");
#else
#ifdef WG103
	sprintf(temp_str, "/usr/sbin/showsysinfo | awk -F \":\" '/MAC/ {print $2\":\"$3\":\"$4\":\"$5\":\"$6\":\"$7}'");
#else
	sprintf(temp_str, "/usr/sbin/showsysinfo | awk -F \":\" '/SN/ {print $2}'");
#endif
#endif
	PopenFile(temp_str, str_tmp, sizeof(str_tmp));	
	memset(str, 0, TEMP_STR_LEN);
	strcpy(str, trim(str_tmp));
	CWWTPDebugLog("WTPBoardID:%s len:%d", str, strlen(str));
	memcpy(WTPBoardID, str, WTP_BOARD_ID_LEN);	

get_base_mac_atheros:
	memset(temp_str, 0, 100);
	sprintf(temp_str, "/usr/sbin/showsysinfo | awk -F \":\" '/MAC/ {print $2\":\"$3\":\"$4\":\"$5\":\"$6\":\"$7}'");
	PopenFile(temp_str, str_tmp, sizeof(str_tmp));
	memset(str, 0, TEMP_STR_LEN);
	strcpy(str, trim(str_tmp));
	sscanf(str, "%x:%x:%x:%x:%x:%x", &a,&b,&c,&d,&e,&f);	
	if((0 == a)&&(0 == b)&&(0 == c))
	{
		sleep(1);
		goto get_base_mac_atheros;
	}
	
	WTPBaseMAC[0]=a;
	WTPBaseMAC[1]=b;
	WTPBaseMAC[2]=c;
	WTPBaseMAC[3]=d;
	WTPBaseMAC[4]=e;
	WTPBaseMAC[5]=f;

	/* get ap-code, pei 090706 */
#ifdef NETGEAR
	strcpy(WTPModelNum, "102-500-H");  //pei 0923
	WTPModelNum[strlen(WTPModelNum)] = '\0';  //pei 0923
#else
	memset(temp_str, 0, 100);
	sprintf(temp_str, "wrcpuutil -i | awk -F \":\" '/product type/ {print substr($2,3)}'");
	PopenFile(temp_str, str, sizeof(str));
	CWWTPDebugLog("temp_APcode:%s len:%d", str, strlen(str));
	memcpy(WTPModelNum, str, strlen(str));
#endif
	
	/* get ap real model, 090519 */
	memset(temp_str, 0, 100);
	sprintf(temp_str, "/usr/sbin/showsysinfo | awk -F \":\" '/Device Type/ {print $2}'");
	PopenFile(temp_str, str_tmp, sizeof(str_tmp));
	memset(str, 0, TEMP_STR_LEN);
	strcpy(str, trim(str_tmp));
	CWWTPDebugLog("temp_WTPRealModelNum:%s len:%d", str, strlen(str));
	memcpy(WTPRealModelNum, str, strlen(str));

#elif (defined(CAVIUM_PLATFORM))

	char temp_str[100];
	char str_tmp[64];
	char str[TEMP_STR_LEN], mac[20];
	int a,b,c,d,e,f;
	
	memset(temp_str, 0, 100);
	sprintf(temp_str, "/usr/sbin/showsysinfo | awk -F \":\" '/SN/ {print $2}'");
	PopenFile(temp_str, str_tmp, sizeof(str_tmp));
	memset(str, 0, TEMP_STR_LEN);
	strcpy(str, trim(str_tmp));
	CWWTPDebugLog("WTPBoardID:%s len:%d", str, strlen(str));
	memcpy(WTPBoardID, str, WTP_BOARD_ID_LEN);

get_base_mac_cavium:
	memset(temp_str, 0, 100);
	sprintf(temp_str, "/usr/sbin/showsysinfo | awk -F \":\" '/BASE MAC/ {print $2}'");
	PopenFile(temp_str, str_tmp, sizeof(str_tmp));
	memset(str, 0, TEMP_STR_LEN);
	strcpy(str, trim(str_tmp));
	//printf("str:%s\n", str);
	memset(mac, 0, 20);
	mac[0] = str[0];
	mac[1] = str[1];
	mac[2] = ':';
	mac[3] = str[2];
	mac[4] = str[3];
	mac[5] = ':';
	mac[6] = str[4];
	mac[7] = str[5];
	mac[8] = ':';
	mac[9] = str[6];
	mac[10] = str[7];
	mac[11] = ':';
	mac[12] = str[8];
	mac[13] = str[9];
	mac[14] = ':';
	mac[15] = str[10];
	mac[16] = str[11];
	printf("mac:%s\n", mac);
	sscanf(mac, "%x:%x:%x:%x:%x:%x", &a,&b,&c,&d,&e,&f);
	if((0 == a)&&(0 == b)&&(0 == c))
	{
		sleep(1);
		CWWTPDebugLog("get base mac all 0, retry");
		goto get_base_mac_cavium;
	}

	WTPBaseMAC[0]=a;
	WTPBaseMAC[1]=b;
	WTPBaseMAC[2]=c;
	WTPBaseMAC[3]=d;
	WTPBaseMAC[4]=e;
	WTPBaseMAC[5]=f;
	
	/* get ap model */ /* ap-code, pei 090519 */
	memset(temp_str, 0, 100);
//	sprintf(temp_str, "/usr/sbin/showsysinfo | awk -F \":\" '/Device Type/ {print $2}'");
	sprintf(temp_str, "bmutil -p | awk -F \":\" '/product name/ {print substr($2,3)}'");
	PopenFile(temp_str, str_tmp, sizeof(str_tmp));
	memset(str, 0, TEMP_STR_LEN);
	strcpy(str, trim(str_tmp));		
	CWWTPDebugLog("temp_APcode:%s len:%d", str, strlen(str));

	memcpy(WTPModelNum, str, strlen(str));
	
	/* get ap real model, 090519 */
	memset(temp_str, 0, 100);
	sprintf(temp_str, "/usr/sbin/showsysinfo | awk -F \":\" '/Device Type/ {print $2}'");
	PopenFile(temp_str, str_tmp, sizeof(str_tmp));
	memset(str, 0, TEMP_STR_LEN);
	strcpy(str, trim(str_tmp));
	CWWTPDebugLog("temp_WTPRealModelNum:%s len:%d", str, (int)strlen(str));

	memcpy(WTPRealModelNum, str, strlen(str));

#else

	char temp_str[100];
	char str_tmp[64];
	unsigned char str[TEMP_STR_LEN];
	int a,b,c,d,e,f;
	memset(temp_str, 0, 100);
	memset(str_tmp, 0 ,64);
	sprintf(temp_str, "/usr/sbin/showsysinfo | awk '/SN/ {print substr($1,4)}'");
	PopenFile(temp_str, str_tmp, sizeof(str_tmp));
	memset(str, 0, TEMP_STR_LEN);
	strcpy((char *)str, trim(str_tmp));
		if(debug_print)
			printf("WTPBoardID:%s len:%d\n", str, strlen((char *)str));
		memcpy(WTPBoardID, str, WTP_BOARD_ID_LEN);

get_base_mac_atheros:
	memset(temp_str, 0, 100);
	sprintf(temp_str, "/usr/sbin/showsysinfo | awk -F \":\" '/MAC/ {print $2\":\"$3\":\"$4\":\"$5\":\"$6\":\"$7}'");
	PopenFile(temp_str, str_tmp, sizeof(str_tmp));
	memset(str, 0, TEMP_STR_LEN);
	strcpy((char *)str, trim(str_tmp));
	sscanf((char *)str, "%x:%x:%x:%x:%x:%x", &a,&b,&c,&d,&e,&f);
	if((0 == a)&&(0 == b)&&(0 == c))
	{
		sleep(1);
		goto get_base_mac_atheros;
	}

	WTPBaseMAC[0]=a;
	WTPBaseMAC[1]=b;
	WTPBaseMAC[2]=c;
	WTPBaseMAC[3]=d;
	WTPBaseMAC[4]=e;
	WTPBaseMAC[5]=f;

	/* get ap-code, pei 090706 */
	memset(temp_str, 0, 100);

	#if (defined(XHPB44)||defined(XHAP93))
		sprintf(temp_str, "get_syscode | awk '{print $0}'");
	#else
		sprintf(temp_str, "pro_ctl_util -i | awk -F \":\" '/product type/ {print substr($2,3)}'");
	#endif

	PopenFile(temp_str, (char *)str,sizeof(str));
	if(debug_print)
		printf("temp_APcode:%s len:%d\n", str, (int)strlen((char *)str));
	memcpy(WTPModelNum, str, strlen((char *)str));
	
	/* get ap real model, 090519 */
	memset(temp_str, 0, 100);
	sprintf(temp_str, "/usr/sbin/showsysinfo | awk -F \":\" '/Device Type/ {print $2}'");
	PopenFile(temp_str, str_tmp, sizeof(str_tmp));
	memset(str, 0, TEMP_STR_LEN);
	strcpy((char *)str, trim(str_tmp));
#ifdef TDPB44_02
	memcpy(WTPRealModelNum, str, strlen(str));
	if(debug_print)
		printf("WTPModelNum:%s len:%d\n", str, strlen(str));
	memset(str, 0, TEMP_STR_LEN);
	strcpy(str,"TD-PB44-IND");
	memset(WTPModelNum,0,64);
	memcpy(WTPModelNum, str, strlen(str));
#else
	if(debug_print)
		printf("temp_WTPRealModelNum:%s len:%d\n", str, strlen((char *)str));
	memcpy(WTPRealModelNum, str, strlen((char *)str));
#endif
	/*gzm add 20100224*/
	if(strcmp((char *)str, "WNDAP350") == 0)
	strcpy(WTPModelNum, "350-N"); 
	if(strcmp((char *)str, "WNAP210") == 0)
	strcpy(WTPModelNum, "210-N"); 

#ifdef WNAP550
	strncpy(WTPModelNum, str+2, strlen(str)-2);
#endif
#endif

#ifdef WRITE_IN_SOFTWARE
	strcpy(WTPModelNum, WTP_MODEL_DEFAULT);  //pei 0923
	WTPModelNum[strlen(WTP_MODEL_DEFAULT)] = '\0';  //pei 0923
#endif

#if(defined(APXJ)||defined(AP2400_IFM)||defined(XJPB44_02))
	gManuOption = 1;
#endif
#if(defined(XHAP93)||defined(XHPB44))
	gManuOption = 4;
#endif
#ifdef TDPB44_02
	gManuOption = 7;	// sdl add for TDPB44_02 license code 7
#endif
	CWWTPDebugLog("###WTP Loads Board Configuration:");
	CWWTPDebugLog("Base mac : [%02x:%02x:%02x:%02x:%02x:%02x]", WTPBaseMAC[0], WTPBaseMAC[1], WTPBaseMAC[2],
		WTPBaseMAC[3], WTPBaseMAC[4], WTPBaseMAC[5]);
	CWWTPDebugLog("AP Code:%s", WTPModelNum);
	CWWTPDebugLog("Model:%s", WTPRealModelNum);
	CWWTPDebugLog("WTP manufacture option:%d", gManuOption);
	int i;
	dpf("SN:");
	for(i=0;i<strlen(WTPBoardID);i++){
		dpf("%c",WTPBoardID[i]);
	}
	dpf("\n");

	return CW_TRUE;
}

CWBool CWWTPLoadVersionConfiguration() {
	CWDebugLog("WTP Loads Version Configuration");
#if (defined(AQ2010))
	char temp_str[100];
	char str_tmp[64];
	char str[64];
	int k=0;

	/* get ap sn */
	memset(temp_str, 0, 100);
	sprintf(temp_str, "/usr/sbin/showsysinfo | awk -F \":\" '/Software Version/ {print $2}'");
	PopenFile(temp_str, str_tmp, sizeof(str_tmp));
	memset(str, 0, 64);
	strcpy(str, trim(str_tmp));
	CWWTPDebugLog("temp_SoftwareVersion:%s len:%d", str, strlen(str));
	for(k=0;k<strlen(str);k++)
	{
		if(str[k]=='#')
			str[k] = '\0';
	}
	memcpy(versionbuf, str, strlen(str));

	/* pei add for hardversion 0214 */
	char hardversion_str[100];
	memset(hardversion_str, 0, 100);
	sprintf(hardversion_str, "/usr/sbin/showsysinfo | awk -F \":\" '/Hardware Version/ {print substr($0,18)}'");
	PopenFile(hardversion_str, gWTPHardWareVersion, sizeof(gWTPHardWareVersion));
	CWWTPDebugLog("gWTPHardWareVersion:%s len:%d", gWTPHardWareVersion, strlen(gWTPHardWareVersion));
#elif (defined(CAVIUM_PLATFORM))
	char temp_str[100];
	char str[64];
	char str_tmp[64];
	int k=0;

	/* get ap sn */
	memset(temp_str, 0, 100);
	sprintf(temp_str, "/usr/sbin/showsysinfo | awk -F \":\" '/Software Version/ {print $2}'");
	PopenFile(temp_str, str_tmp, sizeof(str_tmp));
	memset(str, 0, 64);
	strcpy(str, trim(str_tmp));		
	CWWTPDebugLog("temp_SoftwareVersion:%s len:%d", str, (int)strlen(str));
	for(k=0;k<strlen(str);k++)
	{
		if(str[k]=='#')
			str[k] = '\0';
	}
	memcpy(versionbuf, str, strlen(str));

	/* pei add for hardversion 0214 */
	char hardversion_str[100];
	memset(hardversion_str, 0, 100);
	sprintf(hardversion_str, "/usr/sbin/showsysinfo | awk -F \":\" '/Hardware Version/ {print $2}'");
	PopenFile(hardversion_str, gWTPHardWareVersion,sizeof(gWTPHardWareVersion));
	CWWTPDebugLog("gWTPHardWareVersion:%s len:%d", gWTPHardWareVersion, (int)strlen(gWTPHardWareVersion));

#else

	char temp_str[100];
       char str_tmp[64];
	char str[64];
	int k=0;

	/* get ap sn */
	memset(temp_str, 0, 100);
	sprintf(temp_str, "/usr/sbin/showsysinfo | awk -F \":\" '/Software Version/ {print $2}'");
	PopenFile(temp_str, str_tmp, sizeof(str_tmp));
	memset(str, 0, 64);
	strcpy(str, trim(str_tmp));
	if(debug_print)
		printf("temp_SoftwareVersion:%s len:%d\n", str, strlen(str));
	for(k=0;k<strlen(str);k++)
	{
		if(str[k]=='#')
			str[k] = '\0';
	}
	memcpy(versionbuf, str, strlen(str));
	/* pei add for hardversion 0214 */
	char hardversion_str[100];
	memset(hardversion_str, 0, 100);
	sprintf(hardversion_str, "/usr/sbin/showsysinfo | awk -F \":\" '/Hardware Version/ {print $2}'");
	PopenFile(hardversion_str, gWTPHardWareVersion, sizeof(gWTPHardWareVersion));
	if(debug_print)
		printf("gWTPHardWareVersion:%s len:%d\n", gWTPHardWareVersion, (int)strlen(gWTPHardWareVersion));

#endif

	/* pei add for codeversion 090812 */
	char codeversion_str[100];
	memset(codeversion_str, 0, 100);
	sprintf(codeversion_str, "/usr/sbin/showver | awk -F \".\" '// {print $1\".\"$2\".\"$3}'");
	PopenFile(codeversion_str,gWTPCodeVersion, sizeof(gWTPCodeVersion));
	CWWTPDebugLog("gWTPCodeVersion:%s len:%d", gWTPCodeVersion, (int)strlen(gWTPCodeVersion));
	gWTPSoftWareVersion = versionbuf;
	CWDebugLog("/etc/version/version %s end len %d", gWTPSoftWareVersion, (int)strlen(gWTPSoftWareVersion));
	CWWTPDebugLog("###WTP Loads Version Configuration:");
	CWWTPDebugLog("WTPSoftWareVersion:%s end len %d", gWTPSoftWareVersion, (int)strlen(gWTPSoftWareVersion));
	CWWTPDebugLog("WTPCodeVersion:%s end len %d", gWTPCodeVersion, (int)strlen(gWTPCodeVersion));
	return CW_TRUE; 
}


void CWWTPDestroy() {
	int i;
	
	CWDebugLog("Destroy WTP");
	
	for(i = 0; i < gCWACCount; i++) {
		CW_FREE_OBJECT(gCWACList[i].address);
	}
	
	timer_destroy();

	CW_FREE_OBJECT(gCWACList);
	CW_FREE_OBJECT(gRadiosInfo.radiosInfo);
}


CWBool CWWTPInitConfiguration()
{
	int i;

	CWWTPResetRebootStatistics(&gWTPRebootStatistics);

	//gRadiosInfo.radioCount = CWWTPGetMaxRadios();
	CW_CREATE_ARRAY_ERR(gRadiosInfo.radiosInfo, gRadiosInfo.radioCount, CWWTPRadioInfoValues, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	gRadiosInfo.radiosInfo[0].radioID= 0;
	//gRadiosInfo.radiosInfo[0].numEntries = 0;
	gRadiosInfo.radiosInfo[0].decryptErrorMACAddressList = NULL;
	gRadiosInfo.radiosInfo[0].reportInterval= CW_REPORT_INTERVAL_DEFAULT;
	gRadiosInfo.radiosInfo[0].adminState= ENABLED;
	gRadiosInfo.radiosInfo[0].adminCause= AD_NORMAL;
	gRadiosInfo.radiosInfo[0].operationalState= ENABLED;
	gRadiosInfo.radiosInfo[0].operationalCause= OP_NORMAL;
	gRadiosInfo.radiosInfo[0].TxQueueLevel= 0;
	gRadiosInfo.radiosInfo[0].wirelessLinkFramesPerSec= 0;
	CWWTPResetRadioStatistics(&(gRadiosInfo.radiosInfo[0].statistics));
	//if(!CWWTPInitBinding(0)) {return CW_FALSE;}
	
	for (i=1; i<gRadiosInfo.radioCount; i++)
	{
		gRadiosInfo.radiosInfo[i].radioID= i;
		//gRadiosInfo.radiosInfo[i].numEntries = 0;
		gRadiosInfo.radiosInfo[i].decryptErrorMACAddressList = NULL;
		gRadiosInfo.radiosInfo[i].reportInterval= CW_REPORT_INTERVAL_DEFAULT;
		gRadiosInfo.radiosInfo[i].adminState= ENABLED; //Default value for CAPWAP
		gRadiosInfo.radiosInfo[i].adminCause= AD_NORMAL;
		gRadiosInfo.radiosInfo[i].operationalState= DISABLED;
		gRadiosInfo.radiosInfo[i].operationalCause= OP_NORMAL;
		gRadiosInfo.radiosInfo[i].TxQueueLevel= 0;
		gRadiosInfo.radiosInfo[i].wirelessLinkFramesPerSec= 0;
		CWWTPResetRadioStatistics(&(gRadiosInfo.radiosInfo[i].statistics));
		//if(!CWWTPInitBinding(i)) {return CW_FALSE;}
	}
	return CW_TRUE;
}
void CWDestroyWlan(void){
#if 0
	int i;
	printf("CWDestroyWlan\n");
	for(i=0;i<wtp_wlan_count;i++)
	{
		CWNetworkCloseSocket(wtp_wlan[i].wlan_sendsock);
		CWNetworkCloseSocket(wtp_wlan[i].wlan_eapsock);
		
	}
#endif
}
CWBool WlanTableAdd(CWWTPWlan *addWlanValues, unsigned short vlanId)
{
	CWWTPWlan *wtp_wlan_node;
	CWWTPWlan *ptr = NULL;
	CWWTPWlan *oldptr = NULL;
	CWDebugLog("wlan table add\n");
	for(ptr = wtp_wlan_list;ptr != NULL; oldptr = ptr ,ptr = ptr->next)
	{
		if((addWlanValues->radio_id == ptr->radio_id)&&(addWlanValues->wlan_id == ptr->wlan_id))
		{
			CWDebugLog("wlan is already in table\n");
			CWWTPDebugLog("wlan is already in table");
			if(debug_print)
				printf("wlan is already in table\n");
			return CW_TRUE;
		}
	}
	CW_CREATE_OBJECT_ERR(wtp_wlan_node,CWWTPWlan,return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
	wtp_wlan_node->radio_id = addWlanValues->radio_id;
	wtp_wlan_node->wlan_id = addWlanValues->wlan_id;
	wtp_wlan_node->vlan_id = vlanId;
	wtp_wlan_node->wlan_updown_time = 1;  //pei add 0225
	wtp_wlan_node->wlan_mac_mode = addWlanValues->wlan_mac_mode;
	wtp_wlan_node->wlan_tunnel_mode = addWlanValues->wlan_tunnel_mode;
	strncpy(wtp_wlan_node->essid,addWlanValues->essid,32);
	wtp_wlan_node->wlan_key_mgmt = addWlanValues->wlan_key_mgmt;
	wtp_wlan_node->wlan_sendsock = addWlanValues->wlan_sendsock;
	wtp_wlan_node->wlan_eapsock = addWlanValues->wlan_eapsock;
	wtp_wlan_node->wlan_dot3sock = addWlanValues->wlan_dot3sock;
	/*forward insert*/
	//wtp_wlan_node->next = wtp_wlan_list;
	//wtp_wlan_list = wtp_wlan_node;
	/*behind insert*/
	wtp_wlan_node->next = NULL;
	if(wtp_wlan_list == NULL)
		wtp_wlan_list=wtp_wlan_node;
	else
	{
		ptr = wtp_wlan_list;
		while(ptr->next != NULL)
			ptr = ptr->next;
		ptr->next = wtp_wlan_node;
	}
	wtp_wlan_count++;
	CWWTPDebugLog("add wlan(ath.%d-%d) to wlan_table successfully.",addWlanValues->radio_id,addWlanValues->wlan_id);
	return CW_TRUE;
}
CWBool WLanTableDelete(DeleteWlanValues *deleteWlanValues)
{
	CWWTPWlan *ptr = NULL;
	CWWTPWlan *oldptr = NULL;
	for(ptr=wtp_wlan_list;ptr != NULL;oldptr = ptr,ptr = ptr->next){
		if((ptr->wlan_id == deleteWlanValues->wlan_id)&&(ptr->radio_id == deleteWlanValues->radio_id)){
			if(oldptr != NULL){
				oldptr->next = ptr->next;
				close(ptr->wlan_sendsock);
				close(ptr->wlan_eapsock);
				close(ptr->wlan_dot3sock);
				CW_FREE_OBJECT(ptr);
			}
			else{
				wtp_wlan_list = wtp_wlan_list->next;
				CWDebugLog("delete the wlan's sock");
				close(ptr->wlan_sendsock);
				close(ptr->wlan_eapsock);/*don't need now*/
				close(ptr->wlan_dot3sock);
				CW_FREE_OBJECT(ptr);
			}
		wtp_wlan_count--;
		return CW_TRUE;
		}
	}
	
	CWDebugLog("can't found the delete wlan\n");
	return CW_FALSE;
	
}
CWBool RadioTableAdd()
{
	return CW_TRUE;
}
CWBool RadioTableDelete()
{
	return CW_TRUE;
}

CWBool STAMacSaveForBackup(AddSTAValues *addSTAValues)
{		
		char pathbuf[128];
		
		memset(pathbuf,0,128);
		sprintf(pathbuf,"%s%d",CONF_RADIO_PATH,addSTAValues->radio_id);		
		if(0 != access(pathbuf, F_OK))
		{
			mkdir(pathbuf,S_IRWXU);
		}			
		
		FILE* fd=NULL;
		char filepath[128];
		char mac [16];
		
		memset(mac,0,16);
		memset(filepath,0,128);
		sprintf(filepath,"%s/sta_mac",pathbuf);
		printf("%s\n",filepath);
		if((fd = fopen(filepath,"a+"))==NULL)
		{ 
			printf("create file %s failed!\n",filepath);
			return CW_FALSE;
		}
		else
		{
		
			sprintf(mac,"%02x:%02x:%02x:%02x:%02x:%02x\n",addSTAValues->mac_addr[0],addSTAValues->mac_addr[1],addSTAValues->mac_addr[2]
													,addSTAValues->mac_addr[3],addSTAValues->mac_addr[4],addSTAValues->mac_addr[5]);
			fputs(mac, fd);   
			fclose(fd);
			printf("add the mac to the file for backup,the mac is %s",mac);
			return CW_TRUE;
		}
		
}

CWBool STAMacRefreshForBackup(int radio_id)
{
	FILE* fd=NULL;
	char filepath[256];
	char mac [64];
	CWWTPSta *p = NULL;
	memset(mac,0,64);
	memset(filepath,0,256);
	sprintf(filepath,"%s%d/sta_mac",CONF_RADIO_PATH,radio_id); 
	//dpf("%s\n",filepath);
	if((fd = fopen(filepath,"w+"))==NULL)
	{ 
		printf("open file %s failed!\n",filepath);
		return CW_FALSE;
	}
	else
	{
		for(p = wtp_sta_list;p != NULL; p = p->next)
		{
			if(p->radio_id == radio_id){
				memset(mac,0,64);
				sprintf(mac,"%02x:%02x:%02x:%02x:%02x:%02x\n",p->sta_mac[0],p->sta_mac[1],p->sta_mac[2]
														,p->sta_mac[3],p->sta_mac[4],p->sta_mac[5]);
				fputs(mac, fd);   
			}
		}
		fclose(fd);
	//	dpf("refresh the mac file for backup\n");
		return CW_TRUE;
	}
}

CWBool STATableAdd(AddSTAValues *addSTAValues)
{/*head insert*/
	CWWTPSta *wtp_sta_node;
	CWWTPSta *ptr = NULL;
	CWWTPSta *oldptr = NULL;
	unsigned char *mac = addSTAValues->mac_addr;
	CWDebugLog("sta table add\n");
	CWThreadMutexLock(&sta_table_mutex);
	for(ptr = wtp_sta_list;ptr != NULL; oldptr = ptr ,ptr = ptr->next)
	{
		if(macAddrCmp(ptr->sta_mac,addSTAValues->mac_addr) == 1)
		{
			CWDebugLog("sta is already in table\n");
			CWWTPDebugLog("sta(%02x:%02x:%02x:%02x:%02x:%02x) is already in table",mac[0],mac[1],mac[2],mac[3],mac[4],mac[5]);
			CWThreadMutexUnlock(&sta_table_mutex);
			return CW_TRUE;
		}
	}
	CW_CREATE_OBJECT_ERR(wtp_sta_node, CWWTPSta, {CWThreadMutexUnlock(&sta_table_mutex);return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL);});
	wtp_sta_node->radio_id = addSTAValues->radio_id;
	wtp_sta_node->wlan_id = addSTAValues->wlan_id;
	memcpy(wtp_sta_node->sta_mac,addSTAValues->mac_addr,MAC_ADDR_LEN);
	wtp_sta_node->next = wtp_sta_list;
	wtp_sta_list = wtp_sta_node;
	wtp_sta_count ++;
	CWWTPDebugLog("add sta(%02x:%02x:%02x:%02x:%02x:%02x) to table successfully.",mac[0],mac[1],mac[2],mac[3],mac[4],mac[5]);
	CWThreadMutexUnlock(&sta_table_mutex);
	return CW_TRUE;
}
CWBool STATableDelete(DeleteSTAValues *deleteSTAValues)
{
	int radioid=0;
	CWWTPSta *ptr;
	CWWTPSta *oldptr;
	unsigned char *mac = deleteSTAValues->mac_addr;
	CWThreadMutexLock(&sta_table_mutex);
	
	if(wtp_sta_list == NULL || deleteSTAValues == NULL){
		CWWTPDebugLog("sta table delete, but no sta left");
		CWThreadMutexUnlock(&sta_table_mutex);
		return CW_FALSE;
	}
	oldptr = ptr = NULL;
	for(ptr = wtp_sta_list;ptr != NULL; oldptr = ptr ,ptr = ptr->next){
		if(macAddrCmp(ptr->sta_mac,deleteSTAValues->mac_addr) == 1){
			radioid=ptr->radio_id;
			if(oldptr != NULL){
				oldptr->next = ptr->next;
				CW_FREE_OBJECT(ptr);
			}
			else{/*head*/
				wtp_sta_list = ptr->next;
				CW_FREE_OBJECT(ptr);
				
			}
		wtp_sta_count--;
		STAMacRefreshForBackup(radioid);
		CWWTPDebugLog("del sta(%02x:%02x:%02x:%02x:%02x:%02x) from table successfully.",mac[0],mac[1],mac[2],mac[3],mac[4],mac[5]);
		CWThreadMutexUnlock(&sta_table_mutex);
		return CW_TRUE;
			
		}
	}
	CWWTPDebugLog("cannot find sta(%02x:%02x:%02x:%02x:%02x:%02x) in table,so no need to del.",mac[0],mac[1],mac[2],mac[3],mac[4],mac[5]);
	CWDebugLog("can't found the delete sta\n");
	CWThreadMutexUnlock(&sta_table_mutex);
	return CW_FALSE;
}

int macAddrCmp (unsigned char* addr1, unsigned char* addr2)
{
	int i, ok=1;
#if 0
	printf("Address 1:");
	for (i=0; i<MAC_ADDR_LEN; i++)
	{
		printf("%02x ", addr1[i]);
	}
	
	printf("\nAddress 2:");
	for (i=0; i<MAC_ADDR_LEN; i++)
	{
		printf("%02x ", addr2[i]);
	}
	printf("\n");
#endif	
	for (i=0; i<MAC_ADDR_LEN; i++)
	{
		if (addr1[i]!=addr2[i])
		{ok=0;}
	}

	if (ok==1) {CWDebugLog("MAC Address test: OK\n");}
	else {CWDebugLog("MAC Address test: Failed\n");}
	
	return ok;
}

void CWCaptrue(int n ,unsigned char *buffer){
		int t=0;
		while((n-t)>=16)
		{
			int i;
			printf("[");
			for(i=0;i<16;i++)
				printf("%02x ",buffer[t+i]);
			printf("]\t[");
			for(i=0;i<16;i++)
			{
				char ch=buffer[t+i];
				if(isalnum(ch))
					printf("%c",ch);
				else
					printf(".");
			}
			printf("]\n");
			t+=16;
		}

		if(n>t)
		{
			int i=t;
			printf("[");
			while(i<n)
				printf("%02x ",buffer[i++]);
			printf("]");
			i=n-t;
			i=16-i;
			while(i--)
				printf("   ");
			printf("\t[");
			i=t;
			while(i<n)
			{
				char ch=buffer[i++];
				if(isalnum(ch))
					printf("%c",ch);
				else
					printf(".");
			}
			printf("]\n");
		}
		printf("\n\n");
}

unsigned int get_file_size(const char *filename)
{
	unsigned int size;

	FILE* fp = fopen( filename, "rb" );

	if(fp==NULL)
	{
		CWWTPDebugLog("%s,Open file %s failed.",strerror(errno), filename);
		return 0;
	}

	fseek( fp, SEEK_SET, SEEK_END);
	size=ftell(fp);
	fclose(fp);
	CWWTPDebugLog("Size of file %s is %dB", filename, size);
	
	return size;
}

#if 0
CWBool CWWTPUpdate(unsigned char *imgname,unsigned char *ip)
{
	char *successfile="/tmp/successful";
	char cmdbuf[100];

/* pei add 0823 */
	_system("/usr/sbin/clearmemory");
	dpf("clearmemory OK!\n");

	sprintf(cmdbuf,"cd /tmp && tftp -g -r %s %s && touch %s",imgname,ip,successfile);
	_system((cmdbuf);
	if(access(successfile, 0)!=0)
	{
		dpf("tftp get imge error!\n");
		return CW_FALSE;
	}
	else
	{
		dpf("tftp get imge OK\n");
		sprintf(cmdbuf, "rm %s", successfile);
		_system((cmdbuf);
		
#ifdef AQ2010
		_system("cp /usr/sbin/sysupgrade /tmp/");
		sprintf(cmdbuf,"cd /tmp && /tmp/sysupgrade %s",imgname);   /* pei modified at 081229 */ 
		if(system(cmdbuf)==0){
			return CW_TRUE;
		}
		else
			return CW_FALSE;
#endif
#ifdef AQ1110T
		sprintf(cmdbuf, "cd /tmp && /usr/sbin/thinap1110T_upgrade %s",imgname);  //pei add 1107
		if(system(cmdbuf)==0){
			return CW_TRUE;
		}
		else
			return CW_FALSE;
#endif
#ifdef CAVIUM_PLATFORM  /* pei add 1227 */
		dpf("CAVIUM_PLATFORM upgrade\n");

		/* test for write flash error. at 090514 */
		int imgsize = 0;
		char filename[50];
		sprintf(filename, "/tmp/%s", imgname);
		printf("Img filename is %s\n", filename);
		imgsize = get_file_size(filename);

		if(imgsize>20*1024*1024)
		{
			printf("The image file is too big, upgrade failed!!!\n");
			return CW_FALSE;
		}

		//sprintf(cmdbuf,"cd /tmp && dd if=%s of=/dev/mtdblock2 bs=10",imgname);
		sprintf(cmdbuf,"cd /tmp && /usr/sbin/sysupgrade %s",imgname);   /* pei modified at 090724 */ 
		if(system(cmdbuf)==0){
			return CW_TRUE;
		}
		else
			return CW_FALSE;
		
#endif
	}
}
#else
CWBool CWWTPUpdate(unsigned char *imgname,unsigned char *ip)
{
/* pei add 0823 */
	CWClearMemory();
	CWWTPDebugLog("clearmemory OK!");
	if(debug_print)
		printf("clearmemory OK!\n");

	CWGetUpgradeImage(imgname,ip,successfile);
	
//#ifdef CAVIUM_PLATFORM  /* pei add 1227 */
		dpf("upgrade...\n");

		/* test for write flash error. at 090514 */
		int imgsize = 0;
		char filename[50];
		sprintf(filename, "/tmp/%s", imgname);
		CWWTPDebugLog("Img filename is %s", filename);
		imgsize = get_file_size(filename);

#ifdef CAVIUM_PLATFORM  /* pei add 1227 */ 
		if(imgsize>20*1024*1024)
#else
        if(imgsize>16*1024*1024) /*image size:  AR11n:8M      APXJ:6M      AQ2010: latest v12,not v13 */
#endif
		{
			CWLog("The image file is too big(%dB), upgrade failed!!!",imgsize);
			CWWTPDebugLog("The image file is too big(%dB), upgrade failed!!!",imgsize);
			return CW_FALSE;
		}	

	return CW_TRUE;
}
CWBool CWWTPUpgrade()
{
	char cmdbuf[256];

	if(access(successfile, 0)!=0)
	{
		CWWTPDebugLog("tftp get imgefile %s error!",gImageName);
		CWLog("tftp get imgefile %s error !",gImageName);
		return CW_FALSE;
	}
	else
	{
		CWWTPDebugLog("tftp get imgefile %s OK.",gImageName);
		CWLog("tftp get imgefile %s OK.",gImageName);
		sprintf(cmdbuf, "rm %s", successfile);
		_system(cmdbuf);
		
		CWWTPDebugLog("system upgrade");
		if(CWSysUpgrade((char*)gImageName)==CW_TRUE){
			CWWTPDebugLog("upgrade firmware OK");
			CWLog("upgrade firmware OK.");
			return CW_TRUE;
		}
		else{
			CWWTPDebugLog("upgrade firmware failed.");
			CWLog("upgrade firmware failed.");
			return CW_FALSE;
		}

	}
}
#endif
#if 1 /*reset*/
CWStateTransition CWWTPEnterReset() {
	CWProtocolMessage *messages = NULL;
	int fragmentsNum=0;
	CWProtocolResultCode resultCode = CW_PROTOCOL_SUCCESS;
	int i;
	
	if(!CWAssembleResetResponse(&messages, &fragmentsNum, gWTPPathMTU, gtemp_seqNum/*seq num*/, resultCode)) {
		CWDebugLog("CWAssembleResetResponse error\n");
	}


	for(i = 0; i < fragmentsNum; i++) {

#ifndef CW_NO_DTLS
		if(gDtlsSecurity == 1)
		{
			if(!CWSecuritySend(gWTPSession, messages[i].msg, messages[i].offset))
			{
				CWDebugLog("Error sending message");
				CWFreeMessageFragments(messages, fragmentsNum);
				CW_FREE_OBJECT(messages);
				CWLog("Enter CW_QUIT reboot... In Reset State  CWNetworkSendUnsafeConnected Error.");
				return CW_QUIT;
			}
		}
		else
#endif
		{
			if (!CWNetworkSendUnsafeUnconnected(gWTPSocket, &(gACInfoPtr->preferredAddress), messages[i].msg, messages[i].offset))
			{
				CWDebugLog("Error sending message");
				CWFreeMessageFragments(messages, fragmentsNum);
				CW_FREE_OBJECT(messages);
				CWLog("Enter CW_QUIT reboot... In Reset State  CWNetworkSendUnsafeConnected Error.");
				return CW_QUIT;
			}
		}
	}

	CWFreeMessageFragments(messages, fragmentsNum);
	CW_FREE_OBJECT(messages);

	if(imagedata_code == CW_PROTOCOL_SUCCESS)
	{
		if(CWWTPUpgrade()==CW_TRUE)
              CWLog("WTP Upgrade OK reboot... In Reset State."); 
        else
              CWLog("WTP Upgrade Error reboot... In Reset State.");
	}
	{
		CWLog("WTP Upgrade OK reboot... In Reset State.... Lose AC.");
		CWac_lose();
		CWSysReboot();
	}

	return CW_QUIT;
}
CWBool CWAssembleResetResponse(CWProtocolMessage **messagesPtr, int *fragmentsNumPtr, int PMTU, int seqNum, CWProtocolResultCode resultCode) 
{
	CWProtocolMessage *msgElems= NULL;
	const int msgElemCount = 1;
	CWProtocolMessage *msgElemsBinding= NULL;
	const int msgElemBindingCount=0;
	
	if(messagesPtr == NULL || fragmentsNumPtr == NULL) return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);
	
	CWDebugLog("CWAssembleResetResponse");
	
	CW_CREATE_OBJECT_ERR(msgElems, CWProtocolMessage, return CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););

	if (!(CWAssembleMsgElemResultCode(msgElems,resultCode))) {
		CW_FREE_OBJECT(msgElems);
		return CW_FALSE;
	}

	if(!(CWAssembleMessage(messagesPtr,
			       fragmentsNumPtr,
			       PMTU,
			       seqNum,
			       CW_MSG_TYPE_VALUE_RESET_RESPONSE,
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
	
	CWDebugLog("CWAssembleResetResponse");
	
	return CW_TRUE;
}
CWBool apservicerestart(void){
	char cmdbuf[100];
	CWWTPWlan *ptr = NULL;
	for(ptr = wtp_wlan_list ; ptr != NULL ; ptr = ptr->next){
		CWDebugLog("restart ath%d\n",ptr->wlan_id);
		sprintf(cmdbuf,"/sbin/ifconfig ath%d down",ptr->wlan_id);
		_system(cmdbuf);
		sprintf(cmdbuf,"/sbin/ifconfig ath%d up",ptr->wlan_id);
		_system(cmdbuf);
	}
	return CW_TRUE;
}
#endif/*reset end */

unsigned int ip_int2str(unsigned int ipAddress, unsigned char *buff)
 {
	 unsigned int cnt;
	 unsigned char *tmpPtr = buff;
 
	 cnt = sprintf((char*)tmpPtr,"%d.%d.%d.%d",(ipAddress>>24) & 0xFF, \
			 (ipAddress>>16) & 0xFF,(ipAddress>>8) & 0xFF,ipAddress & 0xFF);
	 
	 return cnt;
 }

int inet_atoi(char *cp, unsigned int *ap)
{
	int dots = 0;
	unsigned int acc = 0;
	unsigned int addr = 0;
	char *cc = cp;
	
	do {
		switch (*cc) {
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
				acc = acc * 10 + (*cc - '0');
			break;

			case '.':
				if (++dots > 3) {
					return 0;
				}
				if (acc > 255) {
					return 0;
				}
				addr = addr << 8 | acc;
				acc = 0;
			break;
/*			case '\0':
				if (acc > 255) {
					return 0;
				}
				addr = addr << 8 | acc;
				printf("2acc:%d\n", acc);
				acc = 0;
			break;
*/
			default:
			break;
		}
		cc++;
	} while (*cc != '\0');
	if (acc > 255) {
		return 0;
	}
	addr = addr << 8 | acc;
	acc = 0;

	// Normalize the address 
	if (dots < 3) {
		addr <<= 8 * (3 - dots);
	}

	*ap = addr;

	return 1;    
}
