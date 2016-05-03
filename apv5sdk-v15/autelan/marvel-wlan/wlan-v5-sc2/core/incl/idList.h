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
 * \file  idList.h
 * \brief aid & station id assignment to clients
 */

#if !defined(_IDLIST_H_)
#define _IDLIST_H_

void InitAidList(void);
UINT32 AssignAid(void);
void FreeAid(UINT32 Aid);

Status_e ResetAid(UINT16 StnId, UINT16 Aid);
WL_STATUS InitStnIdList(int max_stns);
UINT32 AssignStnId(void);
void FreeStnId(UINT32 StnId);

#endif /* _IDLIST_H_ */
