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


#ifndef __CAPWAP_CWAC_HEADER__
#define __CAPWAP_CWAC_HEADER__

/*_______________________________________________________*/
/*  *******************___INCLUDE___*******************  */
#include "CWCommon.h"
#include "ACMultiHomedSocket.h"
#include "ACProtocol.h"
#include "ACInterface.h"
#include "ACBinding.h"
#include "wcpss/waw.h"

/*______________________________________________________*/
/*  *******************___DEFINE___*******************  */
#define CW_MAX_WTP					1024
#define CW_CRITICAL_TIMER_EXPIRED_SIGNAL		SIGUSR2
#define CW_SOFT_TIMER_EXPIRED_SIGNAL			SIGUSR1
#define LOG_FILE_NAME					"ac.log.txt"

/*_____________________________________________________*/
/*  *******************___TYPES___*******************  */
typedef struct {			
	CWNetworkLev4Address address;
	CWThread thread;
#ifndef CW_NO_DTLS
	CWSecuritySession session;
#endif
	CWBool isNotFree;
	CWBool isRequestClose;
	CWStateTransition currentState;
	int interfaceIndex;
	CWSocket socket;
	//char buf[CW_BUFFER_SIZE];
	enum  {
		CW_DTLS_HANDSHAKE_IN_PROGRESS,
		CW_WAITING_REQUEST,
		CW_COMPLETED,
	} subState;		
	CWSafeList packetReceiveList;
	
	CWTimerID currentTimer; 	// depends on the current state: WaitJoin, NeighborDead
	CWTimerID heartbeatTimer; 
	CWList fragmentsList;
	int pathMTU;
	
	/**** ACInterface ****/
	int interfaceResult;
	CWBool interfaceCommandProgress;
	int interfaceCommand;
	CWThreadMutex interfaceSingleton;
	CWThreadMutex interfaceMutex;
	CWThreadMutex WTPThreadMutex;
	CWThreadMutex RRMThreadMutex;
	CWThreadMutex WIDSThreadMutex;
	CWThreadCondition interfaceWait;
	CWThreadCondition interfaceComplete;
	WTPQosValues* qosValues;
	/**** ACInterface ****/

	CWWTPProtocolManager WTPProtocolManager;

	CWProtocolMessage *messages;	// Retransmission
 	int messagesCount;
 	int retransmissionCount;
 	CWTimerID currentPacketTimer;
 	CWBool isRetransmitting;

	int responseType;		// expected response
	int responseSeqNum;
	unsigned int BAK_FLAG;
	//int unrecognizedMsgType;	//Unrecognized message type value
} CWWTPManager;		// Struct that describes a WTP from the AC's point of view

typedef struct {			
	CWNetworkLev4Address address;
	unsigned int WTPID;	
}CWWTPAttach;

typedef struct {
	int index;
	CWSocket sock;
	int interfaceIndex;
} CWACThreadArg_clone; // argument passed to the thread func

/*________________________________________________________________*/
/*  *******************___EXTERN VARIABLES___*******************  */
extern CWWTPManager *gWTPs;
extern CWThreadMutex gWTPsMutex;
#ifndef CW_NO_DTLS
extern CWSecurityContext gACSecurityContext;
#endif
extern int gACHWVersion;
extern int gACSWVersion;
extern char gdefaultsn[DEFAULT_SN_LENTH];
extern int gActiveStations;
extern int gActiveWTPs;
extern int gStaticWTPs;
extern int scanningWTPs;
extern CWThreadMutex gActiveWTPsMutex;
extern CWThreadMutex gAllThreadMutex;//zhanglei add for wlan op
extern CWThreadMutex gACInterfaceMutex;//zhanglei add for wlan op
extern CWThreadCondition gACInterfaceWait;//zhanglei add for wlan op
extern CWThreadCondition gInterfaceComplete;//zhanglei add for wlan op
extern CWThreadMutex gSTARoamingMutex;//zhanglei add for sta roaming op
extern CWThreadCondition gSTARoamingWait;//zhanglei add for sta roaming op
extern CWThreadMutex gACChannelMutex;//zhanglei for dynamic channel selection
extern CWThreadCondition gACChannelWait;//zhanglei for dynamic channel selection
extern CWThreadCondition gChannelComplete;//zhanglei for dynamic channel selection

extern CWThreadMutex gACTxpowerMutex;
extern CWThreadCondition gACTxpowerWait;
extern CWThreadCondition gTxpowerComplete; 
 
extern int gLimit;
extern int gMaxWTPs;
extern CWAuthSecurity gACDescriptorSecurity;
extern int gRMACField;
extern int gWirelessField;
extern int gDTLSPolicy;
extern CWThreadSpecific gIndexSpecific;
extern char *gACName;
extern char *gACHWVersion_char;
extern char *gACSWVersion_char;
extern int gDiscoveryTimer;
extern int gEchoRequestTimer;
extern int gIdleTimeout;
extern CWProtocolNetworkInterface *gInterfaces;
extern int gInterfacesCount;
extern int gMaxInterfacesCount;
extern char **gMulticastGroups;
extern int gMulticastGroupsCount;
extern int wAWSocket;
extern int wASDSocket;
extern int wWSMSocket;
extern int WidMsgQid;
extern int WidAllQid;
extern int ASD_WIDMSGQ;
extern int channel_state;
extern int txpower_state;
extern int tx_wtpid;
extern unsigned char control_scope;
extern unsigned char coverage_threshold;
extern unsigned char tx_power_threshold;
extern unsigned char power_constant;
extern unsigned char tx_power_max;
extern int g_AUTO_AP_LOGIN_SWITCH;
extern char* g_AUTO_AP_LOGIN_BINDING_L3_INTERFACE ;
extern int g_AUTO_AP_LOGIN_BINDING_WLANID ;
extern int g_AUTO_AP_LOGIN_SAVE_CONFIG_SWITCH ;
extern int g_interface_state ;
extern wid_auto_ap_info	g_auto_ap_login;

extern CWMultiHomedSocket gACSocket;
extern int gCOUNTRYCODE;
extern WTPQosValues* gDefaultQosValues;
extern CWWTPAttach **AC_ATTACH;
extern WID_STA STA_ROAM;
extern unsigned int receiver_signal_level;
extern wid_sample_info WID_SAMPLE_INFORMATION;

extern unsigned int g_ap_cpu_threshold;
extern unsigned int g_ap_memuse_threshold;
extern unsigned int g_ap_temp_threshold;
extern struct ifi *WID_IF;

extern struct sockaddr_in B_addr;
extern char is_secondary;
extern char secondary_AC_cmd[80];
/*__________________________________________________________*/
/*  *******************___PROTOTYPES___*******************  */

//in AC.c
void CWACInit(void);
void CWWIDInit(void);//zhanglei add
void CWACEnterMainLoop(void);
CWBool CWACSendAcknowledgedPacket(int WTPIndex, int msgType, int seqNum);
CWBool CWACResendAcknowledgedPacket(int WTPIndex);
void CWACStopRetransmission(int WTPIndex);
void CWACDestroy(void);
//CWBool CWWIDtoASDInitSocket(int *sock);
//CWBool CWWIDtoWSMInitSocket(int *sock);
CWBool CWwAWInitSocket(int *sock);
CWBool CWInitMsgQueue(int *msgqid);
CWBool CWGetMsgQueue(int *msgqid);
//in ACTest.h
CWBool ACQosTest(int WTPIndex);

//in ACRunState.h
CWBool CWACParseGenericRunMessage(int WTPIndex, CWProtocolMessage *msg, CWControlHeaderValues* controlVal);
CWBool CWBindingAssembleWlanConfigurationRequest(CWProtocolMessage **messagesPtr, int *fragmentsNumPtr, int PMTU, int seqNum, int WTPIndex, struct msgqlist * elem);
CWBool CWBindingAssembleStaConfigurationRequest(CWProtocolMessage **messagesPtr, int *fragmentsNumPtr, int PMTU, int seqNum, int WTPIndex);
CWBool CWSaveChangeStateEventRequestMessage (CWProtocolChangeStateEventRequestValues *valuesPtr, CWWTPProtocolManager *WTPProtocolManager);
CWBool CWAssembleConfigurationUpdateRequest(CWProtocolMessage **messagesPtr, int *fragmentsNumPtr, int PMTU, int seqNum);
CWBool CWAssembleConfigurationUpdateRequest_Radio(CWProtocolMessage **messagesPtr, int *fragmentsNumPtr, int PMTU, int seqNum, int WTPIndex, struct msgqlist * elem);
CWBool CWAssembleConfigurationUpdateRequest_WTP(CWProtocolMessage **messagesPtr, int *fragmentsNumPtr, int PMTU, int seqNum, int WTPIndex, struct msgqlist *elem);
CWBool CWAssembleStaConfigurationRequest(CWProtocolMessage **messagesPtr, int *fragmentsNumPtr, int PMTU, int seqNum, int WTPIndex, struct msgqlist *elem);
CWBool CWAssembleStaConfigurationRequest_key(CWProtocolMessage **messagesPtr, int *fragmentsNumPtr, int PMTU, int seqNum, int WTPIndex, struct msgqlist *elem);

//in ACDiscoveryState.c
CWBool CWAssembleDiscoveryResponse(CWProtocolMessage **messagesPtr, int seqNum);
CWBool CWParseDiscoveryRequestMessage(char *msg, int len, int *seqNumPtr, CWDiscoveryRequestValues *valuesPtr);

CWBool CWAssembleMsgElemWTPVersion(CWProtocolMessage *msgPtr,char *version);

//in ACRetransmission.c
CWBool CWACSendFragments(int WTPIndex);

//in ACRunStateCheck.c
CWBool CWACCheckForConfigurationUpdateRequest(int WTPIndex);

//in ACProtocol_User.c
CWBool CWACGetVendorInfos(CWACVendorInfos *valPtr);
int CWACGetRMACField();
int CWACGetWirelessField();
int CWACGetDTLSPolicy();
void CWACDestroyVendorInfos(CWACVendorInfos *valPtr);

//in ACMainLoop.c
void CWACManageIncomingPacket(CWSocket sock, char *buf, int len, int incomingInterfaceIndex,int BindingSystemIndex, CWNetworkLev4Address *addrPtr);
void *CWManageWTP(void *arg);
void CWCloseThread();

// in CWSecurity.c
#ifndef CW_NO_DTLS
CWBool CWSecurityInitSessionServer(CWWTPManager* pWtp, CWSocket sock, CWSecurityContext ctx, CWSecuritySession *sessionPtr, int *PMTUPtr);
#endif
CWBool ACEnterJoin(int WTPIndex, CWProtocolMessage *msgPtr);
CWBool ACEnterConfigure(int WTPIndex, CWProtocolMessage *msgPtr);
CWBool ACEnterImageData(int WTPIndex, CWProtocolMessage *msgPtr);
CWBool ACEnterDataCheck(int WTPIndex, CWProtocolMessage *msgPtr);
CWBool ACEnterRun(int WTPIndex, CWProtocolMessage *msgPtr, CWBool dataFlag);
CWBool ACEnterReset(int WTPIndex, CWProtocolMessage *msgPtr);


CW_THREAD_RETURN_TYPE CWInterface(void* arg);
CW_THREAD_RETURN_TYPE CWInterface1(void* arg);
CW_THREAD_RETURN_TYPE CWDynamicChannelSelection(void * arg);
CW_THREAD_RETURN_TYPE CWTransmitPowerControl(void * arg);
CW_THREAD_RETURN_TYPE CWSTARoamingOP(void* arg);
//void CWTimerExpiredHandler(int arg);

CWBool AsdWsm_RadioOp(unsigned int WTPID, Operate op);
CWBool AsdWsm_WLANOp(unsigned char WlanID, Operate op, int both);
CWBool AsdWsm_BSSOp(unsigned int BSSIndex, Operate op, int both);
CWBool AsdWsm_DataChannelOp(unsigned int WTPID, Operate op);
CWBool wid_to_wsm_bss_pkt_info_get(unsigned int wtpindex);
CWBool AsdWsm_StationOp(unsigned int WTPID, CWStationInfoValues *valuesPtr,Operate op);//added by weiay 20080702
CWBool WidAsd_StationInfoUpdate(unsigned int WTPID,WIDStationInfo * valuesPtr);
CWBool AsdWsm_WTP_Channelchange_Op(unsigned int WtpID,unsigned int radioid,Operate op);

CW_THREAD_RETURN_TYPE CWWawInforUpdate(void * arg);
int wid_sem_creat();
int p(int semid);
int v(int semid);
int wid_new_sem();
void wait_v(int semid);
void _CWCloseThread(int i);
void CWCaptrue(int n ,unsigned char *buffer);
void str2higher(char **str);
CWBool get_sock_descriper(int isystemindex, int* sockdes);
void ACInterfaceReInit();
void syslog_wtp_log(int WTPIndex, int login, char *note);
int RadioNumCheck(int WTPIndex);
CWBool check_ascii_32_to126(const char * str);
void WID_WTP_INIT(void *arg);

#endif
