/*
*                Copyright 2002-2005, Marvell Semiconductor, Inc.
* This code contains confidential information of Marvell Semiconductor, Inc.
* No rights are granted herein under any patent, mask work right or copyright
* of Marvell or any third party.
* Marvell reserves the right at its sole discretion to request that this code
* be immediately returned to Marvell. This code is provided "as is".
* Marvell makes no warranties, express, implied or otherwise, regarding its
* accuracy, completeness or performance.
*/

/*
*
* Description:  Handle all the events coming in and out of the MLME State Machines
*
*/

#include "ap8xLnxIntf.h"
#include "wltypes.h"
#include "IEEE_types.h"

#include "mib.h"
#include "osif.h"
#include "timer.h"



#include "ds.h"

#include "keyMgmtCommon.h"
#include "keyMgmt.h"
#include "tkip.h"

#include "macmgmtap.h"

#include "StaDb.h"
#include "smeMain.h"
#include "qos.h"
#include "macMgmtMlme.h"

#include "mhsm.h"
#include "mlme.h"
#include "idList.h"
#include "IEEE_types.h"
#include "wldebug.h" 
#include "wllog.h" 
#include "ieee80211_autelan_limit.h" //add by lirui for traffic limit

/* Global Declaration */
#ifdef AP_URPTR
extern UINT8 mib_wbMode;
#endif

extern Status_e NotifyNewStn(IEEEtypes_MacAddr_t *MacAddr, UINT16 StnId){return FAIL;};
extern void AuthRspSrvApCtor(AuthRspSrvAp *me);
extern void AssocSrvApCtor(AssocSrvAp *me);
extern void macMgmtMlme_QoSAct(vmacApInfo_t *vmacSta_p, macmgmtQ_MgmtMsg3_t *MgmtMsg_p );
extern BOOLEAN macMgmtMlme_80211hAct(vmacApInfo_t *vmacSta_p, macmgmtQ_MgmtMsg3_t *MgmtMsg_p );
#ifdef COEXIST_20_40_SUPPORT
extern BOOLEAN macMgmtMlme_80211PublicAction(vmacApInfo_t *vmacSta_p, macmgmtQ_MgmtMsg3_t *MgmtMsg_p );
#endif
extern int wlFwSetNewStn(struct net_device *dev, u_int8_t *staaddr,u_int16_t assocId, u_int16_t stnId, u_int16_t action,	PeerInfo_t *pPeerInfo,UINT8 Qosinfo , UINT8 isQosSta);
extern int wlFwSetSecurity(struct net_device *netdev,	u_int8_t *staaddr);
extern void macMgmtRemoveSta(vmacApInfo_t *vmacSta_p,extStaDb_StaInfo_t *StaInfo_p);
extern void setStaPeerInfoApMode(struct wlprivate *wlpptr, extStaDb_StaInfo_t *pStaInfo, PeerInfo_t *pPeerInfo, UINT8 ApMode, struct wds_port *pWdsPort);

//extern MIB_AUTH_ALG *mib_AuthAlg_p_p;

/*************************************************************************
* Function:
*
* Description:
*
* Input:
*
* Output:
*
**************************************************************************/
extern void macMgtSyncSrvInit(vmacApInfo_t *vmacSta_p)
{
	/* Init the Synchronization state machines */
	SyncSrvCtor(&vmacSta_p->mgtSync);
	mhsm_initialize(&vmacSta_p->mgtSync.super,&vmacSta_p->mgtSync.sTop);

}

extern extStaDb_StaInfo_t *macMgtStaDbInit(vmacApInfo_t *vmacSta_p,IEEEtypes_MacAddr_t *staMacAddr, IEEEtypes_MacAddr_t *apMacAddr)
{
	MIB_802DOT11 *mib=vmacSta_p->Mib802dot11;
	extStaDb_StaInfo_t StaInfo;
	extStaDb_StaInfo_t *StaInfo_p = NULL;
	struct wlprivate *wlpptr = NETDEV_PRIV_P(struct wlprivate, vmacSta_p->dev);
	struct ieee80211vap *vap = &(wlpptr->vap); //ath.x-x//lhj//2012.06
	PeerInfo_t PeerInfo;

#ifdef DEBUG_PRINT
	printf("macMgtStaDbInit:: entered\n");
#endif
	WLDBG_ENTER(DBG_LEVEL_11);

	/* Station not in Stn table, hence add */
	memcpy(&StaInfo.Addr, staMacAddr, sizeof(IEEEtypes_MacAddr_t));
	memcpy(&StaInfo.Bssid, apMacAddr, sizeof(IEEEtypes_MacAddr_t));

	StaInfo.State = UNAUTHENTICATED;
	StaInfo.PwrMode = PWR_MODE_ACTIVE;
	StaInfo.StnId = AssignStnId();
	StaInfo.Aid = 0;
	StaInfo.AP = FALSE; 
#ifdef WDS_FEATURE
	StaInfo.wdsInfo = NULL;
	StaInfo.wdsPortInfo = NULL;
#endif
#ifdef CLIENT_SUPPORT
	StaInfo.Client = FALSE;
#endif
#ifdef APCFGUR
	StaInfo.UR = 0;
#endif
#ifdef STA_INFO_DB
	StaInfo.Sq1 = 0;
	StaInfo.Sq2 = 0;
	StaInfo.RSSI= 0;
	StaInfo.Rate= 0;
#endif
	StaInfo.ClientMode = 0;
	StaInfo.mib_p= mib;
	StaInfo.dev = vmacSta_p->dev;
    StaInfo.ni_assocuptime = 0;
    memset(&StaInfo.CapInfo,0,sizeof(IEEEtypes_CapInfo_t));

#ifdef AE5200_RT_SUPPORT
    StaInfo.ni_localflags = 0;
    memset(StaInfo.ni_portal_ipaddr,0,4);
#endif

    ieee80211_node_unauthorize(&StaInfo);
    
	if(*(mib->mib_ApMode)==AP_MODE_AandG)
	{
		if(memcmp(apMacAddr,vmacSta_p->macBssId,sizeof(IEEEtypes_MacAddr_t)))
		{
			StaInfo.ApMode=MIXED_MODE;
		}
		else
		{
			StaInfo.ApMode=AONLY_MODE;
		}
	}
#ifdef AP_URPTR
	if (!mib_wbMode)
#endif
	{
		if (extStaDb_AddSta(vmacSta_p,&StaInfo) != ADD_SUCCESS)
		{
			return NULL;
		}

#ifdef NEW_OSIF_POWERSAVE
		psProcessNewStn(staMacAddr, StaInfo.StnId);
#else
		NotifyNewStn(staMacAddr, StaInfo.StnId);
#endif

		if ((StaInfo_p = extStaDb_GetStaInfo(vmacSta_p,staMacAddr, 1)) == NULL)
		{
			return NULL;
		}
		setStaPeerInfoApMode(wlpptr, StaInfo_p, &PeerInfo, *(wlpptr->vmacSta_p->Mib802dot11->mib_ApMode), NULL);
		StaInfo_p->FwStaPtr = wlFwSetNewStn(vmacSta_p->dev, (u_int8_t *)staMacAddr, StaInfo_p->Aid, StaInfo_p->StnId, 0, &PeerInfo,0,0);  //add new station
		wlFwSetSecurity(vmacSta_p->dev,( u_int8_t *)staMacAddr);
		/* Init the state machines */
		/* Init Auth Request Srv SM */
		AuthReqSrvApCtor((AuthReqSrvAp *)&StaInfo_p->mgtAuthReq);
		mhsm_initialize(&StaInfo_p->mgtAuthReq.super,&StaInfo_p->mgtAuthReq.sTop);

		/* Init Auth Response Srv SM */
		AuthRspSrvApCtor((AuthRspSrvAp *)&StaInfo_p->mgtAuthRsp);
		mhsm_initialize(&StaInfo_p->mgtAuthRsp.super,&StaInfo_p->mgtAuthRsp.sTop);

		/* Init Association Srv SM */
		AssocSrvApCtor((AssocSrvAp *)&StaInfo_p->mgtAssoc);
		mhsm_initialize(&StaInfo_p->mgtAssoc.super,&StaInfo_p->mgtAssoc.sTop);


#ifdef APCFGUR
		/* Init remote control Srv SM */
		RemoteCtrlSrvCtor(&StaInfo_p->rmSrv);
		mhsm_initialize(&StaInfo_p->rmSrv.super,&StaInfo_p->rmSrv.sTop);
		StaInfo_p->rmSrv.userdata_p = (unsigned char *)StaInfo_p;
#endif
	}
	StaInfo_p->mgtAssoc.userdata_p = (unsigned char *)StaInfo_p;
	StaInfo_p->mgtAuthReq.userdata_p = (unsigned char *)StaInfo_p;
	StaInfo_p->mgtAuthRsp.userdata_p = (unsigned char *)StaInfo_p;

	//add begin for autelan traffic_limit//lhj//2012.06
	/*Begin:Added by duanmingzhe for traffic limit 2.0*/
	autelan_traffic_limit_init(&(StaInfo_p->ni_special_tl));
	autelan_traffic_limit_init(&(StaInfo_p->ni_everynode_tl));
	autelan_traffic_limit_vaptoni(&(vap->iv_everynode_tl), &(StaInfo_p->ni_everynode_tl));
    /*End:Added by duanmingzhe for traffic limit 2.0*/
	//add end for autelan traffic_limit

	WLDBG_EXIT(DBG_LEVEL_11);

	return StaInfo_p;
}

/*************************************************************************
* Function:
*
* Description:
*
* Input:
*
* Output:
*
**************************************************************************/
extern IEEEtypes_MacAddr_t macAddrZero;
#ifdef AP_MAC_LINUX
extern SINT8 evtDot11MgtMsg(vmacApInfo_t *vmacSta_p,UINT8 *message, struct sk_buff *skb)
#endif
{
	MIB_AUTH_ALG  *mib_AuthAlg_p=vmacSta_p->Mib802dot11->AuthAlg;
	MhsmEvent_t smMsg;
	macmgmtQ_MgmtMsg3_t *MgmtMsg_p;
	extStaDb_StaInfo_t *StaInfo_p;

	if (message == NULL)
	{
		return 1;
	}
	WLDBG_ENTER(DBG_LEVEL_11);
	MgmtMsg_p = (macmgmtQ_MgmtMsg3_t *)message;

#ifdef FILTER_BSSID
	if ( ( memcmp(MgmtMsg_p->Hdr.DestAddr, vmacSta_p->macStaAddr,sizeof(IEEEtypes_MacAddr_t)) || 
		memcmp(MgmtMsg_p->Hdr.BssId, vmacSta_p->macBssId,sizeof(IEEEtypes_MacAddr_t)) )
		&& ( MgmtMsg_p->Hdr.FrmCtl.Subtype != IEEE_MSG_PROBE_RQST ) )
#else
	if (memcmp(MgmtMsg_p->Hdr.DestAddr, vmacSta_p->macStaAddr,sizeof(IEEEtypes_MacAddr_t)) &&
		memcmp(MgmtMsg_p->Hdr.DestAddr, vmacSta_p->macStaAddr2,sizeof(IEEEtypes_MacAddr_t)) &&
		(MgmtMsg_p->Hdr.FrmCtl.Subtype != IEEE_MSG_PROBE_RQST) )
#endif
	{
		WLDBG_ENTER_INFO(DBG_LEVEL_11,"mgt frame %d rxved %2x-%2x-%2x-%2x-%2x-%2x\n",MgmtMsg_p->Hdr.FrmCtl.Subtype,
			MgmtMsg_p->Hdr.DestAddr[0],
			MgmtMsg_p->Hdr.DestAddr[1],
			MgmtMsg_p->Hdr.DestAddr[2],
			MgmtMsg_p->Hdr.DestAddr[3],
			MgmtMsg_p->Hdr.DestAddr[4],
			MgmtMsg_p->Hdr.DestAddr[5]);
		return 1;
	}
	if (isMacAccessList(vmacSta_p, &(MgmtMsg_p->Hdr.SrcAddr)) != SUCCESS)
	{
		return 1;
	}
	
	if ((StaInfo_p = extStaDb_GetStaInfo(vmacSta_p,&MgmtMsg_p->Hdr.SrcAddr,  0)) == NULL)
	{
		if(MgmtMsg_p->Hdr.FrmCtl.Subtype == IEEE_MSG_AUTHENTICATE)
		{
			//added call to check other VAP's StaInfo_p
			if ((StaInfo_p = extStaDb_GetStaInfo(vmacSta_p,&MgmtMsg_p->Hdr.SrcAddr,  2)))
			{
				macMgmtRemoveSta(vmacSta_p, StaInfo_p);
			}
			if ((StaInfo_p = macMgtStaDbInit(vmacSta_p,&MgmtMsg_p->Hdr.SrcAddr,&MgmtMsg_p->Hdr.DestAddr)) == NULL)
			{
				WlLogPrint(MARVEL_DEBUG_WARNING, __func__,"init data base fail\n");
				return 1;
			}
		}
		else if ((MgmtMsg_p->Hdr.FrmCtl.Subtype != IEEE_MSG_PROBE_RQST))
		{
			if ((MgmtMsg_p->Hdr.FrmCtl.Subtype != IEEE_MSG_DEAUTHENTICATE) && 
				(MgmtMsg_p->Hdr.FrmCtl.Subtype !=  IEEE_MSG_PROBE_RSP) )

			{

				macMgmtMlme_SendDeauthenticateMsg(vmacSta_p,&MgmtMsg_p->Hdr.SrcAddr,
					0,
					IEEEtypes_REASON_CLASS2_NONAUTH);

			} 
			return 1;
		}
	}
#ifdef AP_MAC_LINUX
	smMsg.devinfo = (void *) vmacSta_p;
#endif

        /*****************zhouke add, statistics mgmt frame************/
        if(MgmtMsg_p->Hdr.FrmCtl.Subtype != IEEE_MSG_BEACON)
        {	
            Statistics_Recv_Mgmt_Packet(vmacSta_p,MgmtMsg_p->Hdr.FrmBodyLen);
        }

        if((ieee80211_node_is_authorize(StaInfo_p))&&(MgmtMsg_p->Hdr.FrmCtl.Retry== 1)){
                Statistics_Recv_Packet_Retry(vmacSta_p,MgmtMsg_p->Hdr.FrmBodyLen,IEEE_TYPE_MANAGEMENT);
        }

        /*********************end*******************************/

	switch (MgmtMsg_p->Hdr.FrmCtl.Subtype)
	{
	case IEEE_MSG_AUTHENTICATE:
		{
			AuthRspSrvApMsg authRspMsg;
			WlLogPrint(MARVEL_DEBUG_WARNING, __func__,"received AUTHENTICATE message from %02X:%02X:%02X:%02X:%02X:%02X\n",\
            MgmtMsg_p->Hdr.SrcAddr[0],MgmtMsg_p->Hdr.SrcAddr[1],MgmtMsg_p->Hdr.SrcAddr[2],MgmtMsg_p->Hdr.SrcAddr[3],\
            MgmtMsg_p->Hdr.SrcAddr[4],MgmtMsg_p->Hdr.SrcAddr[5]);
			memcpy(authRspMsg.rspMac, MgmtMsg_p->Hdr.SrcAddr,6);
			authRspMsg.arAlg_in = MgmtMsg_p->Body.Auth.AuthAlg;
			{
				if (mib_AuthAlg_p->Type == AUTH_OPEN_OR_SHARED_KEY)
				{
					authRspMsg.arAlg = authRspMsg.arAlg_in;
				}
				else
				{
					authRspMsg.arAlg = mib_AuthAlg_p->Type;
				}
			}
			authRspMsg.mgtMsg = (UINT8 *)MgmtMsg_p;
			smMsg.event = AuthOdd;
			smMsg.pBody = (unsigned char *)&authRspMsg;
#ifdef AP_MAC_LINUX
			smMsg.info = (void *) skb;
#endif
			mhsm_send_event((Mhsm_t *)&StaInfo_p->mgtAuthRsp.super, &smMsg);
		}     
		break;

	case IEEE_MSG_ASSOCIATE_RQST:
		{
		    WlLogPrint(MARVEL_DEBUG_WARNING, __func__,"received ASSOCIATE message from %02X:%02X:%02X:%02X:%02X:%02X\n",\
            MgmtMsg_p->Hdr.SrcAddr[0],MgmtMsg_p->Hdr.SrcAddr[1],MgmtMsg_p->Hdr.SrcAddr[2],MgmtMsg_p->Hdr.SrcAddr[3],\
            MgmtMsg_p->Hdr.SrcAddr[4],MgmtMsg_p->Hdr.SrcAddr[5]);
			smMsg.event = AssocReq;
			smMsg.pBody = (unsigned char *)MgmtMsg_p;
#ifdef AP_MAC_LINUX
			smMsg.info = (void *) skb;
#endif
			mhsm_send_event((Mhsm_t *)&StaInfo_p->mgtAssoc.super,&smMsg);
		}
		break;

	case IEEE_MSG_REASSOCIATE_RQST:
		{
		    WlLogPrint(MARVEL_DEBUG_WARNING, __func__,"received REASSOCIATE message from %02X:%02X:%02X:%02X:%02X:%02X\n",\
            MgmtMsg_p->Hdr.SrcAddr[0],MgmtMsg_p->Hdr.SrcAddr[1],MgmtMsg_p->Hdr.SrcAddr[2],MgmtMsg_p->Hdr.SrcAddr[3],\
            MgmtMsg_p->Hdr.SrcAddr[4],MgmtMsg_p->Hdr.SrcAddr[5]);
			smMsg.event = ReAssocReq;
			smMsg.pBody = (unsigned char *)MgmtMsg_p;
#ifdef AP_MAC_LINUX
			smMsg.info = (void *) skb;
#endif
			mhsm_send_event((Mhsm_t *)&StaInfo_p->mgtAssoc.super, &smMsg);
		}
		break;

	case IEEE_MSG_DISASSOCIATE:
		{
		    WlLogPrint(MARVEL_DEBUG_WARNING, __func__,"received DISASSOCIATE message from %02X:%02X:%02X:%02X:%02X:%02X\n",\
            MgmtMsg_p->Hdr.SrcAddr[0],MgmtMsg_p->Hdr.SrcAddr[1],MgmtMsg_p->Hdr.SrcAddr[2],MgmtMsg_p->Hdr.SrcAddr[3],\
            MgmtMsg_p->Hdr.SrcAddr[4],MgmtMsg_p->Hdr.SrcAddr[5]);
			smMsg.event = DisAssoc;
			smMsg.pBody = (unsigned char *)MgmtMsg_p;
#ifdef AP_MAC_LINUX
			smMsg.info = (void *) skb;
#endif
			mhsm_send_event((Mhsm_t *)&StaInfo_p->mgtAssoc.super, &smMsg);
		}
		break;

	case IEEE_MSG_DEAUTHENTICATE:
		{
		    WlLogPrint(MARVEL_DEBUG_WARNING, __func__,"received DEAUTHENTICATE message from %02X:%02X:%02X:%02X:%02X:%02X\n",\
            MgmtMsg_p->Hdr.SrcAddr[0],MgmtMsg_p->Hdr.SrcAddr[1],MgmtMsg_p->Hdr.SrcAddr[2],MgmtMsg_p->Hdr.SrcAddr[3],\
            MgmtMsg_p->Hdr.SrcAddr[4],MgmtMsg_p->Hdr.SrcAddr[5]);
			smMsg.event = DeAuth;
			smMsg.pBody = (unsigned char *)MgmtMsg_p;
#ifdef AP_MAC_LINUX
			smMsg.info = (void *) skb;
#endif
			mhsm_send_event((Mhsm_t *)&StaInfo_p->mgtAuthRsp.super, &smMsg);
		}
		break;

		/* Could be handled by HW */
	case IEEE_MSG_PROBE_RQST:
		{
			SyncSrvApMsg syncMsg;
		    WlLogPrint(MARVEL_DEBUG_WARNING, __func__,"received PROBE_RQST message from %02X:%02X:%02X:%02X:%02X:%02X\n",\
            MgmtMsg_p->Hdr.SrcAddr[0],MgmtMsg_p->Hdr.SrcAddr[1],MgmtMsg_p->Hdr.SrcAddr[2],MgmtMsg_p->Hdr.SrcAddr[3],\
            MgmtMsg_p->Hdr.SrcAddr[4],MgmtMsg_p->Hdr.SrcAddr[5]);
			syncMsg.opMode = infrastructure;
			syncMsg.mgtMsg = (UINT8 *)MgmtMsg_p;
			smMsg.event = ProbeReq;
			smMsg.pBody = (unsigned char *)&syncMsg;
#ifdef AP_MAC_LINUX
			smMsg.info = (void *) skb;
#endif
			mhsm_send_event((Mhsm_t *)&vmacSta_p->mgtSync.super, &smMsg);
		}
		break;

#if defined(QOS_FEATURE)||defined(IEEE80211H)
	case IEEE_MSG_QOS_ACTION:
		{
		    WlLogPrint(MARVEL_DEBUG_WARNING, __func__,"received QOS_ACTION message from %02X:%02X:%02X:%02X:%02X:%02X\n",\
            MgmtMsg_p->Hdr.SrcAddr[0],MgmtMsg_p->Hdr.SrcAddr[1],MgmtMsg_p->Hdr.SrcAddr[2],MgmtMsg_p->Hdr.SrcAddr[3],\
            MgmtMsg_p->Hdr.SrcAddr[4],MgmtMsg_p->Hdr.SrcAddr[5]);

			if(MgmtMsg_p->Body.Action.Category == HT_CATEGORY)
			{
				switch (MgmtMsg_p->Body.Action.Action)
				{
				case ACTION_SMPS:
					{
						extern int wlFwSetMimoPsHt(struct net_device *netdev, UINT8 *addr, UINT8 enable, UINT8 mode);
						IEEEtypes_SM_PwrCtl_t *p =  (IEEEtypes_SM_PwrCtl_t *)&MgmtMsg_p->Body.Act.Field.SmPwrCtl;
						WLDBG_INFO(DBG_LEVEL_11, "IEEE_MSG_QOS_ACTION MIMO PS HT message received. \n");
						wlFwSetMimoPsHt(vmacSta_p->dev, (UINT8 *)MgmtMsg_p->Hdr.SrcAddr, p->Enable, p->Mode);    
						break;
					}
#ifdef INTOLERANT40
				case ACTION_INFOEXCH:
					{
						extern void RecHTIntolerant(vmacApInfo_t *vmacSta_p,UINT8 enable);
						IEEEtypes_InfoExch_t *p =  (IEEEtypes_InfoExch_t *)&MgmtMsg_p->Body.Act.Field.InfoExch;
						WLDBG_INFO(DBG_LEVEL_11, "IEEE_MSG_QOS_ACTION Info Exch HT message received. \n");

						RecHTIntolerant(vmacSta_p,p->FortyMIntolerant);
						break;
					}	
#endif //#ifdef INTOLERANT40
				default:
					break;
				}
				break;
			}
#ifdef IEEE80211H         
			if(TRUE == macMgmtMlme_80211hAct(vmacSta_p,(macmgmtQ_MgmtMsg3_t *) MgmtMsg_p)) /* if it's mine frame, then return true */
				break;
#endif /* IEEE80211H */        
#ifdef COEXIST_20_40_SUPPORT
			if(TRUE == macMgmtMlme_80211PublicAction(vmacSta_p,(macmgmtQ_MgmtMsg3_t *) MgmtMsg_p)) /* if it's mine frame, then return true */
				break;
#endif  
#ifdef QOS_FEATURE    
			/*smMsg.event = QoSAction;
			smMsg.pBody = MgmtMsg_p;
			mhsm_send_event((Mhsm_t *)&wlpptr->vmacSta_p->mgtSync.super, &smMsg);*/
			macMgmtMlme_QoSAct(vmacSta_p,(macmgmtQ_MgmtMsg3_t *) MgmtMsg_p);
			break;
#endif /* QOS_FEATURE */ 
		}   
#endif
	default:
		break;
	}
	WLDBG_EXIT(DBG_LEVEL_11);

	return 0;
}

/*************************************************************************
* Function:
*
* Description:
*
* Input:
*
* Output:
*
**************************************************************************/
extern SINT8 evtSmeCmdMsg(vmacApInfo_t *vmacSta_p,UINT8 *message)
{
	MhsmEvent_t smMsg;
	macmgmtQ_SmeCmd_t * smeCmd_p;
	extStaDb_StaInfo_t *StaInfo_p;

	WLDBG_ENTER(DBG_CLASS_INFO);
	if (message == NULL)
	{
		return 1;
	}
#ifdef AP_MAC_LINUX
	smMsg.devinfo = (void *) vmacSta_p;
#endif
	smeCmd_p = (macmgmtQ_SmeCmd_t *)message;
	switch (smeCmd_p->CmdType)
	{

	case SME_CMD_DISASSOCIATE:
		{
			WLDBG_INFO(DBG_LEVEL_11, "evtSmeCmdMsg: SME_CMD_DISASSOCIATE message received. \n");
			if ((StaInfo_p = extStaDb_GetStaInfo(vmacSta_p,&smeCmd_p->Body.AssocCmd.PeerStaAddr, 1)) == NULL)
			{
				return 1;
			}
			smMsg.event = MlmeDisAssoc_Req;
			smMsg.pBody = (UINT8 *)&(smeCmd_p->Body.AssocCmd);
			mhsm_send_event((Mhsm_t *)&StaInfo_p->mgtAssoc.super, &smMsg);
		}
		break;

	case SME_CMD_START:
		{
			SyncSrvApMsg syncMsg;
			WLDBG_INFO(DBG_LEVEL_11, "evtSmeCmdMsg: SME_CMD_START message received. \n");
			syncMsg.opMode = infrastructure;
			syncMsg.mgtMsg = (UINT8 *)&(smeCmd_p->Body.StartCmd);
			smMsg.event = MlmeStart_Req;
			smMsg.pBody = (unsigned char *)&syncMsg;
			mhsm_send_event((Mhsm_t *)&vmacSta_p->mgtSync.super, &smMsg);
		}
		break;

	case SME_CMD_RESET:
		{
			SyncSrvApMsg syncMsg;
			WLDBG_INFO(DBG_LEVEL_11, "evtSmeCmdMsg: SME_CMD_RESET message received. \n");
			syncMsg.mgtMsg = (UINT8 *)&(smeCmd_p->Body.ResetCmd);
			smMsg.event = ResetMAC;
			smMsg.pBody = (unsigned char *)&syncMsg;
			mhsm_send_event((Mhsm_t *)&vmacSta_p->mgtSync.super, &smMsg);
		}
		break;

#if defined(AP_SITE_SURVEY) || defined(AUTOCHANNEL)
	case SME_CMD_SCAN:
		{
			SyncSrvApMsg syncMsg;
			WLDBG_INFO(DBG_LEVEL_11, "evtSmeCmdMsg: SME_CMD_SCAN message received. \n");            
			syncMsg.mgtMsg = (UINT8 *)&(smeCmd_p->Body.ScanCmd);
			smMsg.event = MlmeScan_Req;
			smMsg.pBody = (unsigned char *)&syncMsg;
			mhsm_send_event((Mhsm_t *)&vmacSta_p->mgtSync.super, &smMsg);
		}
		break;
#endif /* AP_SITE_SURVEY */

#ifdef IEEE80211H
	case SME_CMD_MREQUEST:
		{   
			SyncSrvApMsg syncMsg;
			WLDBG_INFO(DBG_LEVEL_11, "evtSmeCmdMsg: SME_CMD_MREQUEST message received. \n");
			if (!IS_BROADCAST(&smeCmd_p->Body.MrequestCmd.PeerStaAddr))
			{
				if ((StaInfo_p = extStaDb_GetStaInfo(vmacSta_p,&smeCmd_p->Body.MrequestCmd.PeerStaAddr, 1)) == NULL)
				{
					WLDBG_INFO(DBG_LEVEL_11, "evtSmeCmdMsg: SME_CMD_MREQUEST - no station found %x:%x:%x:%x:%x:%x] \n", 
						smeCmd_p->Body.MrequestCmd.PeerStaAddr[0],
						smeCmd_p->Body.MrequestCmd.PeerStaAddr[1],
						smeCmd_p->Body.MrequestCmd.PeerStaAddr[2],
						smeCmd_p->Body.MrequestCmd.PeerStaAddr[3],
						smeCmd_p->Body.MrequestCmd.PeerStaAddr[4],
						smeCmd_p->Body.MrequestCmd.PeerStaAddr[5]);
					return 1;
				}
			}
			syncMsg.mgtMsg = (UINT8 *)&(smeCmd_p->Body.MrequestCmd);
			smMsg.event = MlmeMrequest_Req;
			smMsg.pBody = (unsigned char *)&syncMsg;
			mhsm_send_event((Mhsm_t *)&vmacSta_p->mgtSync.super, &smMsg);
		}
		break;

	case SME_CMD_MREPORT:
		{   
			SyncSrvApMsg syncMsg;
			WLDBG_INFO(DBG_LEVEL_11, "evtSmeCmdMsg: SME_CMD_MREPORT message received. \n");

			if ((StaInfo_p = extStaDb_GetStaInfo(vmacSta_p,&smeCmd_p->Body.MrequestCmd.PeerStaAddr, 1)) == NULL)
			{
				WLDBG_INFO(DBG_LEVEL_11, "evtSmeCmdMsg: SME_CMD_MREPORT - no station found %x:%x:%x:%x:%x:%x] \n", 
					smeCmd_p->Body.MrequestCmd.PeerStaAddr[0],
					smeCmd_p->Body.MrequestCmd.PeerStaAddr[1],
					smeCmd_p->Body.MrequestCmd.PeerStaAddr[2],
					smeCmd_p->Body.MrequestCmd.PeerStaAddr[3],
					smeCmd_p->Body.MrequestCmd.PeerStaAddr[4],
					smeCmd_p->Body.MrequestCmd.PeerStaAddr[5]);

				return 1;
			}

			syncMsg.mgtMsg = (UINT8 *)&(smeCmd_p->Body.MreportCmd);
			smMsg.event = MlmeMreport_Req;
			smMsg.pBody = (unsigned char *)&syncMsg;
			mhsm_send_event((Mhsm_t *)&vmacSta_p->mgtSync.super, &smMsg);
		}
		break;   

	case SMC_CMD_CHANNELSWITCH_REQ:
		{   
			SyncSrvApMsg syncMsg;
			WLDBG_INFO(DBG_LEVEL_11, "evtSmeCmdMsg: SMC_CMD_CHANNELSWITCH_REQ message received. \n");            
			syncMsg.mgtMsg = (UINT8 *)&(smeCmd_p->Body.ChannelSwitchCmd);
			smMsg.event = MlmeChannelSwitch_Req;
			smMsg.pBody = (unsigned char *)&syncMsg;            
			mhsm_send_event((Mhsm_t *)&vmacSta_p->mgtSync.super, &smMsg);
		}
		break;     
#endif /* IEEE80211H */
	default:
		break;
	}
	WLDBG_EXIT(DBG_CLASS_INFO);
	return 0;
}

/*************************************************************************
* Function:
*
* Description:
*
* Input:
*
* Output:
*
**************************************************************************/
#ifdef NEW_OSIF_POWERSAVE
/* This is part of group NEW_OSIF_POWERSAVE */
extern SINT8 evtMlmePwMgmtMsg(UINT8 *dataFrameHeader, extStaDb_StaInfo_t *StaInfo_p)
{
	IEEEtypes_PwrMgmtMode_e PwrMode;
	IEEEtypes_GenHdr_t *Hdr_p;

	Hdr_p = (IEEEtypes_GenHdr_t *)dataFrameHeader;
	/*if ((StaInfo_p = extStaDb_GetStaInfo(vmacSta_p,&Hdr_p->Addr2, 0)) != NULL )*/
	{
		PwrMode = StaInfo_p->PwrMode;

		if ( PwrMode == PWR_MODE_ACTIVE )
		{
			if ( Hdr_p->FrmCtl.PwrMgmt == 0)
			{
				/* no change in mode */
				return 1;
			}
			else
			{
				MhsmEvent_t msg;
				msg.event = PsIndicate;
				msg.pBody = (unsigned char *)StaInfo_p;
				StaInfo_p->PwrMode = PWR_MODE_PWR_SAVE;
                StaInfo_p->ni_flags |= IEEE80211_NODE_PWR_MGT;
				mhsm_send_event((Mhsm_t *)&StaInfo_p->pwrSvMon.super,&msg);
				return 1;

			}
		}
		else
		{
			if (Hdr_p->FrmCtl.PwrMgmt == 1)
			{
				/* no change in power mode */
				return 1;
			}
			else
			{
				MhsmEvent_t msg;
				msg.event = PsIndicate;
				msg.pBody = (unsigned char *)StaInfo_p;
				StaInfo_p->PwrMode = PWR_MODE_ACTIVE;
				mhsm_send_event((Mhsm_t *)&StaInfo_p->pwrSvMon.super, &msg);
				return 1;

			}
		}
	}
	return 0;
}
#endif /* NEW_OSIF_POWERSAVE */

