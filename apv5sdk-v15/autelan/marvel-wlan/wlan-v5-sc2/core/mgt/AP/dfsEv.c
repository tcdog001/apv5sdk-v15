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
* Description:  Handle all the events coming in and out of the DFS State Machines
*
*/
#ifdef MRVL_DFS

#include "wltypes.h"
#include "IEEE_types.h"

#include "mib.h"
#include "osif.h"
#include "timer.h"
#include "dfsMgmt.h"
#include "dfs.h"



#include "ds.h"
#include "smeMain.h"

#include "mhsm.h"
#include "IEEE_types.h"
#include "wldebug.h"
#include "dfsMgmt.h"
#include "dfs.h"
#include "ap8xLnxIntf.h"


void DfsInit(struct net_device  *pNetDev, struct wlprivate_data *wlpd_p)
{
	DfsAp *pdfsApMain = NULL ;

	if( !wlpd_p )
	{
		return ;
	}
	/* Init the DFS state machines */
	if(( pdfsApMain = (DfsAp *)kmalloc( sizeof( DfsAp ), GFP_KERNEL )) == NULL )
	{
		printk("Cannot allocate memory for DFS SM\n");
		return ;
	}
	wlpd_p->pdfsApMain = (DfsAp *)pdfsApMain ;
	DFSApCtor(pNetDev, pdfsApMain);
	mhsm_initialize(&pdfsApMain->super,&pdfsApMain->sTop);
	return ;
}

void DfsDeInit(struct wlprivate_data *wlpd_p)
{
	/* DeInit the DFS state machines */
	if( !wlpd_p )
	{
		return ;
	}
	if( !wlpd_p->pdfsApMain )
	{
		return ;
	}
	DFSApReset(wlpd_p->pdfsApMain);	
	kfree(wlpd_p->pdfsApMain);
	wlpd_p->pdfsApMain = NULL ;
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
SINT8 evtDFSMsg(struct net_device *dev, UINT8 *message)
{
	MhsmEvent_t dfsMsg;
	DfsCmd_t *dfsCmd_p;
	struct wlprivate *priv = NETDEV_PRIV_P(struct wlprivate, dev);
	struct wlprivate_data       *wlpd_p = priv->wlpd_p ;

	WLDBG_ENTER(DBG_CLASS_INFO);
	if (message == NULL)
	{
		return 1;
	}
	if( !wlpd_p->pdfsApMain )
	{
		return 1 ;
	}
	dfsCmd_p = (DfsCmd_t *)message;
	switch (dfsCmd_p->CmdType)
	{
	case DFS_CMD_CHANNEL_CHANGE :
		{
			DfsApMsg dfsApMsg ;

			WLDBG_INFO(DBG_LEVEL_15, "evtDFSMsg: DFS_CMD_CHANNEL_CHANGE message received. \n");
			dfsApMsg.mgtMsg = (UINT8 *)(&(dfsCmd_p->Body.chInfo));
			dfsMsg.event = CHANNEL_CHANGE_EVT;
			dfsMsg.pBody = (unsigned char *)&dfsApMsg ;
			mhsm_send_event(&wlpd_p->pdfsApMain->super, &dfsMsg);
		}
		break ;
	case DFS_CMD_RADAR_DETECTION:
		{
			DfsApMsg dfsApMsg ;

			WLDBG_INFO(DBG_LEVEL_15, "evtDFSMsg: DFS_CMD_RADAR_DETECTION message received. \n");
			dfsApMsg.mgtMsg = (UINT8 *)(&(dfsCmd_p->Body.chInfo));
			dfsMsg.event = RADAR_EVT;
			dfsMsg.pBody = (unsigned char *)&dfsApMsg ;
			mhsm_send_event(&wlpd_p->pdfsApMain->super, &dfsMsg);
		}
		break ;
	case DFS_CMD_WL_RESET:
		{
			DfsApMsg dfsApMsg ;

			WLDBG_INFO(DBG_LEVEL_15, "evtDFSMsg: DFS_CMD_WL_RESET message received. \n");
//			dfsApMsg.mgtMsg = (UINT8 *)(&(dfsCmd_p->Body.chInfo));
			dfsMsg.event = WL_RESET_EVT;
			dfsMsg.pBody = (unsigned char *)&dfsApMsg ;
			mhsm_send_event(&wlpd_p->pdfsApMain->super, &dfsMsg);
		}
		break ;

	default :
		return 1;
	}
	return 1;
}

#endif
