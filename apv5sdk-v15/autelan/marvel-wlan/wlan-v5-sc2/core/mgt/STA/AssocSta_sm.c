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

#include "mhsm.h"
#include "mlmeSta.h" 
#include "wltypes.h"

MhsmEvent_t const *AssocSrvSta_top(AssocSrvSta *me, MhsmEvent_t *msg)
{
    #ifdef ETH_DEBUG
    eprintf("AssocSrvSta_top:: Enter\n");
    #endif /* ETH_DEBUG */
    switch (msg->event)
    {
    case MHSM_ENTER:
        mhsm_transition(&me->super, &me->Assoc_Srv_Sta);
        return 0;
    default:
        return msg;
    }
}


MhsmEvent_t const *Assoc_Srv_Sta_Handle(AssocSrvSta *me, MhsmEvent_t *msg)
{
    #ifdef ETH_DEBUG
    eprintf("Assoc_Srv_Sta_Handle:: Enter\n"); 
    #endif /* ETH_DEBUG */
    switch (msg->event)
    {
    case MHSM_ENTER:
        mhsm_transition(&me->super, &me->Assoc_Idle);
        return 0;
    default:
        return msg;
    }
}

MhsmEvent_t const *Assoc_Sta_Idle_Handle(AssocSrvSta *me, MhsmEvent_t *msg)
{
    #ifdef ETH_DEBUG
    eprintf("Assoc_Sta_Idle_Handle:: Enter\n");
    #endif /* ETH_DEBUG */
    switch (msg->event)
    {
    case MHSM_ENTER:
        return 0;
    case Timeout:
        #ifdef ETH_DEBUG
        eprintf("Assoc_Sta_Idle_Handle:: event-> Timeout\n");
        #endif /* ETH_DEBUG */
        /* House cleaning */
        wl_MacMlme_AssocSrvStaTimeout(msg->info, msg->pBody);
        mhsm_transition(&me->super, &me->Assoc_Idle);
        return 0;
    case AssocReq:
        if (wl_MacMlme_AssocCmd(msg->info, msg->pBody) == MLME_SUCCESS)
        {
            mhsm_transition(&me->super, &me->Wait_Assoc_Rsp);
        }
        return 0;
    case ReAssocReq:
        wl_MacMlme_ReAssocCmd(msg->info, msg->pBody);
        mhsm_transition(&me->super, &me->Wait_ReAssoc_Rsp);
        return 0;

	case MlmeAssoc_Req:
		if (wl_MacMlme_AssocCmd(msg->info, msg->pBody) == MLME_SUCCESS)
        {
            mhsm_transition(&me->super, &me->Wait_Assoc_Rsp);
        }
        return 0;

	case MlmeReAssoc_Req:
        wl_MacMlme_ReAssocCmd(msg->info, msg->pBody);
        mhsm_transition(&me->super, &me->Wait_ReAssoc_Rsp);
        return 0;

    case DisAssoc:
        assocSrv_RecvDisAssocMsg((vmacStaInfo_t *)msg->info, 
								 (dot11MgtFrame_t *)msg->pBody);
        mhsm_transition(&me->super, &me->Assoc_Idle);
        return 0;
    }
    return msg;
}
MhsmEvent_t const *Wait_Assoc_Sta_Rsp_Handle(AssocSrvSta *me, MhsmEvent_t *msg)
{
    #ifdef ETH_DEBUG
    eprintf("Wait_Assoc_Sta_Rsp_Handle:: Enter\n");
    #endif /* ETH_DEBUG */
    switch (msg->event)
    {
    case MHSM_ENTER:
        return 0;
    case Timeout:
        #ifdef ETH_DEBUG
        eprintf("Wait_Assoc_Sta_Rsp_Handle:: event-> Timeout\n");
        #endif /* ETH_DEBUG */
        /* House cleaning */
        wl_MacMlme_AssocSrvStaTimeout(msg->info, msg->pBody);
        mhsm_transition(&me->super, &me->Assoc_Idle);
        return 0;
    case AssocRsp:
        if (wl_MacMlme_AssocRsp(msg->info, msg->pBody) == MLME_SUCCESS)
        {
            mhsm_transition(&me->super, &me->Assoc_Idle);
        }
        return 0;

    case DisAssoc:
        assocSrv_RecvDisAssocMsg((vmacStaInfo_t *)msg->info, 
								 (dot11MgtFrame_t *)msg->pBody);
        mhsm_transition(&me->super, &me->Assoc_Idle);
        return 0;
    }
    return msg;
}
MhsmEvent_t const *Wait_ReAssoc_Sta_Rsp_Handle(AssocSrvSta *me, MhsmEvent_t *msg)
{
    #ifdef ETH_DEBUG
    eprintf("Wait_ReAssoc_Sta_Rsp_Handle:: Enter\n");
    #endif /* ETH_DEBUG */
    switch (msg->event)
    {
    case MHSM_ENTER:
        return 0;
    case Timeout:
        #ifdef ETH_DEBUG
        eprintf("Wait_ReAssoc_Sta_Rsp_Handle:: event-> Timeout\n");
        #endif /* ETH_DEBUG */
        /* House cleaning */
        wl_MacMlme_AssocSrvStaTimeout(msg->info, msg->pBody);
        mhsm_transition(&me->super, &me->Assoc_Idle);
        return 0;
    case ReAssocRsp:
        wl_MacMlme_ReAssocRsp(msg->info, msg->pBody);
        mhsm_transition(&me->super, &me->Assoc_Idle);
        return 0;

    case DisAssoc:
        assocSrv_RecvDisAssocMsg((vmacStaInfo_t *)msg->info, 
								 (dot11MgtFrame_t *)msg->pBody);
        mhsm_transition(&me->super, &me->Assoc_Idle);
        return 0;
    }
    return msg;
}

void AssocSrvStaCtor(AssocSrvSta *me)
{
    mhsm_add(&me->sTop, NULL, (MhsmFcnPtr)AssocSrvSta_top);
    mhsm_add(&me->Assoc_Srv_Sta,  
              &me->sTop, (MhsmFcnPtr)Assoc_Srv_Sta_Handle);
    mhsm_add(&me->Assoc_Idle,  &me->Assoc_Srv_Sta,
              (MhsmFcnPtr)Assoc_Sta_Idle_Handle);
    mhsm_add(&me->Wait_Assoc_Rsp, &me->Assoc_Srv_Sta,
              (MhsmFcnPtr)Wait_Assoc_Sta_Rsp_Handle);
    mhsm_add(&me->Wait_ReAssoc_Rsp,  &me->Assoc_Srv_Sta,
              (MhsmFcnPtr)Wait_ReAssoc_Sta_Rsp_Handle);
}

