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

#ifndef _KEYMGMT_IF_H_
#define _KEYMGMT_IF_H_

#define NONCE_SIZE          32
#define EAPOL_MIC_KEY_SIZE  16
#include "wltypes.h"
#include "IEEE_types.h"
#include "wl_hal.h"
#include "ds.h"
typedef enum
{
	IDLE,
	START4WAYHSK,
	KEYTIMEOUT,
	MSGRECVDEVT,
	KEYMGMTTIMEOUTEVT,
	GRPKEYTIMEOUTEVT
}keyMgmtEvent_e;

#define KEY_INFO_KEYTYPE    0x0800
#define KEY_INFO_REQUEST    0x0008
#define KEY_INFO_ERROR      0x0004


extern UINT32 isMsgInQ;

extern void ProcessKeyMgmtData(vmacApInfo_t *vmacSta_p,void *rx_FrmPtr,
							   IEEEtypes_MacAddr_t *SourceAddr,
							   keyMgmtEvent_e evt);
void KeyMgmtInit(vmacApInfo_t *vmacSta_p);
extern void * ProcessEAPoLAp(vmacApInfo_t *vmacSta_p, IEEEtypes_8023_Frame_t *pEthFrame, IEEEtypes_MacAddr_t *pMacStaAddr);
#endif
