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

#include "ap8xLnxIntf.h"
#include "ap8xLnxDesc.h"
#include "buildModes.h"
#include "wltypes.h"
#include "IEEE_types.h"
#include "mib.h"
#include "wl_macros.h"
#include "ds.h"
#include "keyMgmtCommon.h"
#include "keyMgmt.h"
#include "timer.h"
#include "tkip.h"
#include "StaDb.h"
#include "macmgmtap.h"
#include "macMgmtMlme.h"
#include "wldebug.h"
#include "wl_hal.h"



extern void SetupAdaptRow(void);
extern vmacApInfo_t *wlCreateSysCfg(struct wlprivate *wlp, UINT32 opMode, MFG_CAL_DATA *calData, char *addr, int, int vMacId);
extern void ampdu_Init(struct net_device *dev);
extern void wds_Init(struct net_device *netdev);
#ifdef STA_QOS
void InitStaQosParam();
#endif


UINT8 system_Init(vmacApInfo_t *sSysCfg_p, char *addr)
{

	sSysCfg_p->txPwrTblLoaded = 0;
	sSysCfg_p->regionCodeLoaded = 0;

    /* updated this from 300 to 1024 to achieve lower/zero loss higher UDP traffics 
    Further tuning increases to 2304 needed for lower PHY rate */
	sSysCfg_p->txQLimit = 2304;

	sSysCfg_p->work_to_do = MAX_NUM_RX_DESC;
	return(OS_SUCCESS);
}

/******************************************************************************
*
* Name: Ap_Init
*
* Description:
*   This routine is to initialize AP
*
* Conditions For Use:
*   None.
*
* Arguments:
*   
*
* Return Value:
*   None
*
* Notes:
*   None.
*
* PDL:
*    
* END PDL
*
****************************************************************************/

vmacApInfo_t * Mac_Init(struct wlprivate *wlp, struct net_device *dev,char *addr, UINT32 mode, int phyMacId,int index)
{
    int phyMacIndex;
	struct wlprivate *wlpptr = NETDEV_PRIV_P(struct wlprivate, dev);
	vmacApInfo_t *vmacSta_p=NULL;
	WLDBG_INFO(DBG_LEVEL_3, " Mac_Init: wireless AP Initialization. \n");

    /*********** zhouke add,for macindex and vmacindex ***********/
    if(wlp)
    {
        phyMacIndex = wlp->vmacSta_p->VMacEntry.phyHwMacIndx;
    }
    else
    {
        phyMacIndex = phyMacId;
    }
    
    //vmacSta_p = wlCreateSysCfg(wlpptr, mode, NULL, addr, phyMacId, wlp?wlp->wlpd_p->vmacIndex: 0);
    vmacSta_p = wlCreateSysCfg(wlpptr, mode, NULL, addr, phyMacIndex, wlp?index: 0);
    
    /************************* end ******************************/
	if(vmacSta_p == NULL)
		return NULL;
	vmacSta_p->dev = dev;
	if(wlp)
	{
		vmacSta_p->master = wlp->vmacSta_p;
		//vmacSta_p->VMacEntry.macId = wlp->wlpd_p->vmacIndex;
		vmacSta_p->VMacEntry.macId = index;
	}
	else
	{
		vmacSta_p->master = NULL;
		vmacSta_p->VMacEntry.macId = 0;
	}
	/* guijinlin add for low speed limitation 2012-06-29 start */
	vmacSta_p->mcs_mask = 0xFFFFFF;
	/* guijinlin add for low speed limitation 2012-06-29 end */
	/*********** zhouke add,for macindex and vmacindex ***********/
	//vmacSta_p->VMacEntry.phyHwMacIndx = phyMacId;
	vmacSta_p->VMacEntry.phyHwMacIndx = phyMacIndex;
	vmacSta_p->auth_time_in_minutes = 30;    //sta auth time = 30s;
	/************************* end ******************************/
	wlpptr->vmacSta_p= vmacSta_p;
	system_Init(vmacSta_p, addr);
	if(mode == WL_OP_MODE_AP)
	{
		extStaDb_Init(vmacSta_p,MAX_STNS);
#ifdef AMPDU_SUPPORT
		ampdu_Init(dev);
#endif
	}

	ethStaDb_Init(vmacSta_p,MAX_STNS);
	macMgmtAp_Init(vmacSta_p, MAX_AID, (IEEEtypes_MacAddr_t *)addr);
	if(mode == WL_OP_MODE_AP)
		smeMain_Init(vmacSta_p);

#ifdef MRVL_WSC
	memset(&vmacSta_p->thisbeaconIE, 0, sizeof(WSC_BeaconIE_t));
	memset(&vmacSta_p->thisprobeRespIE, 0, sizeof(WSC_ProbeRespIE_t));
	vmacSta_p->WPSOn = 0 ;
#endif
#ifdef MRVL_WAPI
	memset(&vmacSta_p->thisbeaconIEs, 0, sizeof(WAPI_BeaconIEs_t));
	memset(&vmacSta_p->thisprobeRespIEs, 0, sizeof(WAPI_ProbeRespIEs_t));
#endif
#ifdef MRVL_DFS
	if(wlp)
		wlpptr->wlpd_p->pdfsApMain = NULL ;
    vmacSta_p->dfsCacExp = 0;
#endif
	return vmacSta_p;
}

