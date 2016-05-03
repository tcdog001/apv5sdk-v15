/*
*                Copyright 2009, Marvell Semiconductor, Inc.
* This code contains confidential information of Marvell Semiconductor, Inc.
* No rights are granted herein under any patent, mask work right or copyright
* of Marvell or any third party.
* Marvell reserves the right at its sole discretion to request that this code
* be immediately returned to Marvell. This code is provided "as is".
* Marvell makes no warranties, express, implied or otherwise, regarding its
* accuracy, completeness or performance.
*/


#ifndef _WDS_H_

#include "IEEE_types.h"
#include "mib.h"
#include "StaDb.h"
#include "idList.h"
#include "ap8xLnxIntf.h"
#include "ap8xLnxVer.h"
#include "hostcmd.h"
#include "wldebug.h"

extern BOOLEAN validWdsIndex(UINT8 wdsIndex);
extern BOOLEAN setWdsPort(struct net_device *netdev, UINT8 *pMacAddr, UINT8 wdsIndex, UINT8 wdsPortMode);
extern void getWdsModeStr(char *wdsModeStr, UINT8 wdsPortMode);
extern void AP_InitWdsPorts(struct wlprivate *wlpptr);
extern void wlprobeInitWds(struct wlprivate *wlpptr);
extern BOOLEAN wdsPortActive(struct net_device *netdev,UINT8 wdsIndex);
extern void wds_wlDeinit(struct net_device *netdev);
extern void setWdsPortMacAddr(struct net_device *netdev, UINT8 *pMacAddr);
extStaDb_StaInfo_t *updateWds(struct net_device *netdev);
extern struct wds_port *getWdsPortFromNetDev(struct wlprivate *wlpptr, struct net_device *netdev);
#endif
