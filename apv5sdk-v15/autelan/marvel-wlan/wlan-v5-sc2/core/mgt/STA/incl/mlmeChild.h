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
/*******************************************************************************************
*
* File: mlmeChild.h
*        MLME Child Module Header
* Description:  Implementation of the MLME Child Module Services
*
*******************************************************************************************/

#ifndef MAC_MLME_CHILD
#define MAC_MLME_CHILD

#define MAX_MLME_CHILD_SESSIONS			26

/* Control Bits Map for Child Session */
#define MLME_CHILD_SET_ADDR_TO_HW		(1<<0)

extern vmacEntry_t *childSrv_StartSession(UINT8 macIndex, 
										  UINT8 *macAddr,
 										  void *callBack_fp,
                                          UINT32 controlParam);
extern SINT32 childSrv_TerminateSession(vmacId_t vMacId);
extern SINT32 childSrv_TerminateAllLinks(void);
extern halMacId_t childSrv_RegisterBlockAddress(vmacStaInfo_t * vStaInfo_p,
                                                UINT8 macIndex, 
												UINT8 *macAddr,
												UINT8 addrMask);

#endif /* MAC_MLME_CHILD */





