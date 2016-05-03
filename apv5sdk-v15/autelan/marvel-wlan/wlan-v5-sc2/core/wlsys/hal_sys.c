/*
*                Copyright 2003, Marvell Semiconductor, Inc.
* This code contains confidential information of Marvell Semiconductor, Inc.
* No rights are granted herein under any patent, mask work right or copyright
* of Marvell or any third party.
* Marvell reserves the right at its sole discretion to request that this code
* be immediately returned to Marvell. This code is provided "as is".
* Marvell makes no warranties, express, implied or otherwise, regarding its
* accuracy, completeness or performance.
*/

/*!
* \file    hal_sys.c
* \brief   HAL routines for general system control
*/
#include "ap8xLnxIntf.h"
#include "ap8xLnxFwdl.h"
#include "wltypes.h"
#include "wl_macros.h"
#include "IEEE_types.h"
#include "wl_mib.h"
#include "wl_hal.h"
#include "mib.h"
#include "qos.h"
#include "wlmac.h"

/*!
* \todo Not used now. Make this into user profile struct and private MIB struct
*/



/*! \todo Document the following API change */

/*!
* The user must call wlCreateSysCfg first. We will initilize
* a default sys configuration according to operation mode (client or AP)
* The user then can adjust the sys config, and then pass it to
* wlSysInit where the hardware is initialized.
* calData can be NULL. We will just not use calibration data
* This function does not initialize any hardware. Just initialize
* internal HAL software data structures.
*/

vmacApInfo_t *wlCreateSysCfg(struct wlprivate *wlp, UINT32 opMode, MFG_CAL_DATA *calData, char *addr, int phyMacId, int vMacId)
{
	vmacApInfo_t *sSysCfg_p;
	WL_SYS_CFG_DATA *sSysCfgData_p;
	sSysCfg_p = kmalloc(sizeof(vmacApInfo_t),GFP_KERNEL);
	if(sSysCfg_p == NULL)
	{
		printk("fail to alloc memory\n");
		return NULL;
	}
	memset(sSysCfg_p, 0, sizeof(vmacApInfo_t));
	memcpy(&sSysCfg_p->VMacEntry.vmacAddr,  addr, 6);
	sSysCfg_p->VMacEntry.info_p = (UINT8*)sSysCfg_p;

    sSysCfg_p->VMacEntry.phyHwMacIndx = phyMacId;
    sSysCfgData_p = kmalloc(sizeof(WL_SYS_CFG_DATA), GFP_KERNEL);
    if(sSysCfgData_p == NULL)
    {
    	kfree(sSysCfg_p);
        printk("fail to alloc memory\n");
        return NULL;
    }
    memset(sSysCfgData_p, 0, sizeof(WL_SYS_CFG_DATA));
    sSysCfg_p->Mib802dot11 = &(sSysCfgData_p->Mib802dot11);
    sSysCfg_p->ShadowMib802dot11= &(sSysCfgData_p->ShadowMib802dot11);
    sSysCfg_p->sysCfgData = sSysCfgData_p;

	if(opMode == WL_OP_MODE_AP || opMode == WL_OP_MODE_VAP)
	{
		sSysCfg_p->VMacEntry.modeOfService = VMAC_MODE_AP;
	}
	else
	{
		sSysCfg_p->VMacEntry.modeOfService = VMAC_MODE_CLNT_INFRA;
	}
	sSysCfg_p->OpMode = opMode;
	/*
	* Initilize default MIB based on client mode or AP mode 
	*/
	mib_InitAp(sSysCfg_p->ShadowMib802dot11, addr, phyMacId, vMacId, 1);
	mib_InitAp(sSysCfg_p->Mib802dot11, addr, phyMacId, vMacId, 0);
	if (calData)			 
	{
		sSysCfg_p->CalData = calData;
	}
	else
	{
		sSysCfg_p->CalData = NULL;
	}
	return sSysCfg_p;
}

void wlDestroySysCfg(vmacApInfo_t *vmacSta_p)
{
    mib_CleanAp(vmacSta_p->ShadowMib802dot11); //yuanding add for clean the mib
	kfree(vmacSta_p->sysCfgData);
	kfree(vmacSta_p);
}


