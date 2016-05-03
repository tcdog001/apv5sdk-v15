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

/*!
* \file    wlMlmeSrv.c
* \brief   the implementation of state machine service routines
*/

#include "mlme.h"
#include "IEEE_types.h"
#include "mib.h"
#include "ds.h"
#include "osif.h"
#include "keyMgmtCommon.h"
#include "keyMgmt.h"

#include "wldebug.h"
#include "tkip.h"
#include "StaDb.h"
#include "macmgmtap.h"
#include "qos.h"
#include "macMgmtMlme.h"
#include "wllog.h"


extern SINT32 mlmeAuthDoOpenSys(vmacApInfo_t *vmacSta_p, AuthRspSrvApMsg *authRspMsg_p);
extern SINT32 mlmeAuthDoSharedKeySeq1(vmacApInfo_t *vmacSta_p, AuthRspSrvApMsg *authRspMsg_p);
extern SINT32 mlmeAuthDoSharedKeySeq3(vmacApInfo_t *vmacSta_p, AuthRspSrvApMsg *authRspMsg_p);
extern void macMgmtMlme_ChannelSwitchReq(vmacApInfo_t *vmacSta_p,IEEEtypes_ChannelSwitchCmd_t *ChannelSwitchtCmd_p);
extern void macMgmtMlme_MReportReq(vmacApInfo_t *vmacSta_p,IEEEtypes_MReportCmd_t *MreportCmd_p);
extern void macMgmtMlme_MRequestReq(vmacApInfo_t *vmacSta_p,IEEEtypes_MRequestCmd_t *MrequestCmd_p);
extern void syncSrv_ScanCmd(vmacApInfo_t *vmacSta_p, IEEEtypes_ScanCmd_t *ScanCmd_p );
extern void mlmeAuthError(vmacApInfo_t *vmacSta_p,IEEEtypes_StatusCode_t statusCode, UINT16 arAlg_in, UINT8 *Addr);
/*!
* association serveice timeout handler 
*  
* @param data_p Pointer to user defined data
* @return MLME_SUCCESS, MLME_INPROCESS, MLME_FAIL
*/
int wl_MacMlme_AssocSrvTimeout( void *data_p )
{
	/*    extStaDb_StaInfo_t *StaInfo_p = (extStaDb_StaInfo_t*)data_p; */
	/*    StaInfo_p->State = AUTHENTICATED; */
	return (MLME_SUCCESS);
}

/*!
* received association request service routine 
*  
* @param data_p Pointer to user defined data
* @return MLME_SUCCESS, MLME_INPROCESS, MLME_FAIL
*/
int wl_MacMlme_AssocReq( vmacApInfo_t *vmacSta_p, void *data_p, UINT32 msgSize )
{
	macmgmtQ_MgmtMsg_t *MgmtMsg_p = (macmgmtQ_MgmtMsg_t *)data_p;
	macMgmtMlme_AssociateReq(vmacSta_p, (macmgmtQ_MgmtMsg3_t *)MgmtMsg_p, msgSize);
	return (MLME_SUCCESS);
}

/*!
* received re-association request service routine 
*  
* @param data_p Pointer to user defined data
* @return MLME_SUCCESS, MLME_INPROCESS, MLME_FAIL
*/
int wl_MacMlme_ReAssocReq(vmacApInfo_t *vmacSta_p,  void *data_p, UINT32 msgSize )
{
	macmgmtQ_MgmtMsg_t *MgmtMsg_p = (macmgmtQ_MgmtMsg_t *)data_p;
	macMgmtMlme_ReassociateReq(vmacSta_p, (macmgmtQ_MgmtMsg3_t *)MgmtMsg_p, msgSize);
	return (MLME_SUCCESS);
}

/*!
* received dis-association request service routine 
*  
* @param data_p Pointer to user defined data
* @return MLME_SUCCESS, MLME_INPROCESS, MLME_FAIL
*/
int wl_MacMlme_DisAssocReq(vmacApInfo_t *vmacSta_p, void *data_p, UINT32 msgSize )
{
	macmgmtQ_MgmtMsg_t *MgmtMsg_p = (macmgmtQ_MgmtMsg_t *)data_p;

    /*** zhouke add ,for recv disassocReq packet del sta list ***/
    macMgmtMlme_DeauthenticateMsg(vmacSta_p,(macmgmtQ_MgmtMsg3_t *)MgmtMsg_p, msgSize);
    //macMgmtMlme_DisassociateMsg(vmacSta_p,(macmgmtQ_MgmtMsg3_t *)MgmtMsg_p, msgSize);
	return (MLME_SUCCESS);
}

/*!
* ds response 
*  
* @param data_p Pointer to user defined data
* @return MLME_SUCCESS, MLME_INPROCESS, MLME_FAIL
*/
int wl_MacMlme_DsResponse( void *data_p )
{
	/*    macmgmtQ_MgmtMsg_t *MgmtMsg_p = (macmgmtQ_MgmtMsg_t *)data_p; */
	return (MLME_SUCCESS);
}

/*!
* disassociation command service routine 
*  
* @param data_p Pointer to user defined data
* @return MLME_SUCCESS, MLME_INPROCESS, MLME_FAIL
*/
int wl_MacMlme_DisAssocCmd( vmacApInfo_t *vmacSta_p, void *data_p )
{
	IEEEtypes_DisassocCmd_t *DisassocCmd_p = (IEEEtypes_DisassocCmd_t *)data_p;
	macMgmtMlme_DisassociateCmd(vmacSta_p, DisassocCmd_p);
	return (MLME_SUCCESS);
}

/*!
* received authenticate request service routine 
*  
* @param data_p Pointer to user defined data
* @return MLME_SUCCESS, MLME_INPROCESS, MLME_FAIL
*/
int wl_MacMlme_AuthReq( void *data_p )
{
	return (MLME_SUCCESS);
}

/*!
* received authenticate sequence even service routine 
*  
* @param data_p Pointer to user defined data
* @return MLME_SUCCESS, MLME_INPROCESS, MLME_FAIL
*/
int wl_MacMlme_AuthEven(vmacApInfo_t *vmacSta_p, void *data_p )
{
	AuthRspSrvApMsg *authRspMsg_p = (AuthRspSrvApMsg *)data_p;
	int share_key = 1;
	if ( share_key )
	{
		/*mlmeAuthDoSharedKey(authRspMsg_p); */
		return (MLME_INPROCESS);
	} else
	{
		mlmeAuthDoOpenSys(vmacSta_p,authRspMsg_p);
	}
	return (MLME_SUCCESS);
}

/*!
* received authenticate sequence 1 service routine 
*  
* @param data_p Pointer to user defined data
* @return MLME_SUCCESS, MLME_INPROCESS, MLME_FAIL
*/
int wl_MacMlme_AuthOdd1(vmacApInfo_t *vmacSta_p, void *data_p )
{
	AuthRspSrvApMsg *authRspMsg = (AuthRspSrvApMsg *)data_p;

	if ( authRspMsg->arAlg_in == shared_key  && authRspMsg->arAlg == shared_key)
	{
		WlLogPrint(MARVEL_DEBUG_ALL, __func__,"mlmeAuthDoSharedKeySeq\n");
		if ( mlmeAuthDoSharedKeySeq1(vmacSta_p,authRspMsg)==MLME_SUCCESS )
		{
			return (MLME_INPROCESS);
		} else
		{
			return (MLME_FAILURE);
		}
	}
	else if (authRspMsg->arAlg_in == open_system  && authRspMsg->arAlg == open_system)
	{
		WlLogPrint(MARVEL_DEBUG_ALL, __func__,"mlmeAuthDoOpenSys\n");
		mlmeAuthDoOpenSys(vmacSta_p,authRspMsg);
	}
	else    
	{ 
		macmgmtQ_MgmtMsg3_t  *MgmtMsg_p;
		WlLogPrint(MARVEL_DEBUG_WARNING, __func__,"unsupported authalg\n");
		MgmtMsg_p = (macmgmtQ_MgmtMsg3_t *) authRspMsg->mgtMsg;
		mlmeAuthError(vmacSta_p, IEEEtypes_STATUS_UNSUPPORTED_AUTHALG, 
			authRspMsg->arAlg,
			(UINT8 *)&MgmtMsg_p->Hdr.SrcAddr);
		return (MLME_FAILURE);
	}


	return (MLME_SUCCESS);
}

/*!
* received authenticate sequence 3 service routine 
*  
* @param data_p Pointer to user defined data
* @return MLME_SUCCESS, MLME_INPROCESS, MLME_FAIL
*/
int wl_MacMlme_AuthOdd3(vmacApInfo_t *vmacSta_p,void *data_p )
{
	AuthRspSrvApMsg *authRspMsg = (AuthRspSrvApMsg *)data_p;

	return (mlmeAuthDoSharedKeySeq3(vmacSta_p,authRspMsg));
}

/*!
* received deauthentication service routine 
*  
* @param data_p Pointer to user defined data
* @return MLME_SUCCESS, MLME_INPROCESS, MLME_FAIL
*/
int wl_MacMlme_DeAuth(vmacApInfo_t *vmacSta_p, void *data_p, UINT32 msgSize )
{
	macmgmtQ_MgmtMsg_t *MgmtMsg_p = (macmgmtQ_MgmtMsg_t *)data_p;
	macMgmtMlme_DeauthenticateMsg(vmacSta_p, (macmgmtQ_MgmtMsg3_t *)MgmtMsg_p , msgSize);
	return (MLME_SUCCESS);
}


/*!
* authenticate service timeout handler 
*  
* @param data_p Pointer to user defined data
* @return MLME_SUCCESS, MLME_INPROCESS, MLME_FAIL
*/
int wl_MacMlme_AuthSrvTimeout( void *data_p )
{
	extStaDb_StaInfo_t *StaInfo_p = (extStaDb_StaInfo_t*)data_p;
	StaInfo_p->State = UNAUTHENTICATED;
	return (MLME_SUCCESS);
}

/*!
* reset command service routine 
*  
* @param data_p Pointer to user defined data
* @return MLME_SUCCESS, MLME_INPROCESS, MLME_FAIL
*/
int wl_MacMlme_ResetCmd(vmacApInfo_t *vmacSta_p, void *data_p )
{
	SyncSrvApMsg *syncMsg = (SyncSrvApMsg *)data_p;
	macMgmtMlme_ResetCmd(vmacSta_p, (IEEEtypes_ResetCmd_t *)syncMsg->mgtMsg);
	return (MLME_SUCCESS);
}

/*!
* synchronization service timeout handler 
*  
* @param data_p Pointer to user defined data
* @return MLME_SUCCESS, MLME_INPROCESS, MLME_FAIL
*/
int wl_MacMlme_SyncSrvTimeout( void *data_p )
{
	return (MLME_SUCCESS);
}

/*!
* start command service routine 
*  
* @param data_p Pointer to user defined data
* @return MLME_SUCCESS, MLME_INPROCESS, MLME_FAIL
*/
int wl_MacMlme_StartCmd(vmacApInfo_t *vmacSta_p, void *data_p )
{
	SyncSrvApMsg *syncMsg = (SyncSrvApMsg *)data_p;
	macMgmtMlme_StartCmd(vmacSta_p,(IEEEtypes_StartCmd_t *)syncMsg->mgtMsg );
	return (MLME_SUCCESS);
}

/*!
* received probe request service routine 
*  
* @param data_p Pointer to user defined data
* @return MLME_SUCCESS, MLME_INPROCESS, MLME_FAIL
*/
int wl_MacMlme_ProbeReq(vmacApInfo_t *vmacSta_p,void *data_p )
{
	SyncSrvApMsg *syncMsg = (SyncSrvApMsg *)data_p;
	macMgmtMlme_ProbeRqst(vmacSta_p, (macmgmtQ_MgmtMsg3_t *)syncMsg->mgtMsg );
	return (MLME_SUCCESS);
}
#if defined(AP_SITE_SURVEY) || defined(AUTOCHANNEL)
/********************* Added for Site Survey on AP *******************************/
/*!
* Scan Request for Site Survey 
*  
* @param data_p Pointer to user defined data
* @return MLME_SUCCESS, MLME_INPROCESS, MLME_FAIL
*/
int wl_MacMlme_ScanReq(vmacApInfo_t *vmacSta_p, void *data_p )
{
	SyncSrvApMsg *syncMsg = (SyncSrvApMsg *)data_p;
	syncSrv_ScanCmd(vmacSta_p, (IEEEtypes_ScanCmd_t *)syncMsg->mgtMsg );
	return (MLME_SUCCESS);
}
#endif /* AP_SITE_SURVEY */

#ifdef IEEE80211H
/********************* Support IEEE 802.11h *******************************/
/*!
* MREQUEST Request 
*  
* @param data_p Pointer to user defined data
* @return MLME_SUCCESS, MLME_INPROCESS, MLME_FAIL
*/
int wl_MacMlme_MRequestReq(vmacApInfo_t *vmacSta_p,void *data_p )
{   
	SyncSrvApMsg *syncMsg = (SyncSrvApMsg *)data_p;
	macMgmtMlme_MRequestReq(vmacSta_p,(IEEEtypes_MRequestCmd_t *)syncMsg->mgtMsg );
	return (MLME_SUCCESS);
}

/*!
* MREPORT Request 
*  
* @param data_p Pointer to user defined data
* @return MLME_SUCCESS, MLME_INPROCESS, MLME_FAIL
*/
int wl_MacMlme_MReportReq(vmacApInfo_t *vmacSta_p, void *data_p )
{   
	SyncSrvApMsg *syncMsg = (SyncSrvApMsg *)data_p;
	macMgmtMlme_MReportReq(vmacSta_p,(IEEEtypes_MReportCmd_t *)syncMsg->mgtMsg );
	return (MLME_SUCCESS);
}

/*!
* CHANNELSWITCH Request 
*  
* @param data_p Pointer to user defined data
* @return MLME_SUCCESS, MLME_INPROCESS, MLME_FAIL
*/
int wl_MacMlme_ChannelswitchReq(vmacApInfo_t *vmacSta_p,void *data_p)
{   
	SyncSrvApMsg *syncMsg = (SyncSrvApMsg *)data_p;
	macMgmtMlme_ChannelSwitchReq(vmacSta_p,(IEEEtypes_ChannelSwitchCmd_t *)syncMsg->mgtMsg );
	return (MLME_SUCCESS);
}
#endif /* IEEE80211H */
#ifdef APCFGUR
int RmSrv_Timeout( UINT8 *data_p, UINT32 ptr )
{
	UINT8 *data;
	extStaDb_StaInfo_t *StaInfo_p = (extStaDb_StaInfo_t*)data_p; 
	data =(UINT8 *)ptr; 
	SendApiDataTo(&StaInfo_p->Addr, data);
	return (MLME_SUCCESS);
}
#endif

