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


#ifndef _BCNGEN_H_
#define _BCNGEN_H_

#include "wlmac.h"
#include "macmgmtap.h"

#define SETBIT    1
#define RESETBIT  0
#define SET_ERP_PROTECTION 1
#define RESET_ERP_PROTECTION 0

extern macmgmtQ_MgmtMsg_t * BcnBuffer_p;
extern void bcngen_UpdateBeaconBuffer(vmacApInfo_t *vmacSta_p,IEEEtypes_StartCmd_t *StartCmd_p);
extern void bcngen_UpdateBitInTim(UINT16 Aid, BOOLEAN Set);
extern void bcngen_BeaconFreeIsr(void);
extern void bcngen_UpdateBeaconErpInfo(vmacApInfo_t *vmacSta_p,BOOLEAN SetFlag);
extern void bcngen_EnableBcnFreeIntr(void);

extern UINT16 AddRSN_IEWPA2_TO(IEEEtypes_RSN_IE_WPA2_t *thisStaRsnIEWPA2_p,IEEEtypes_RSN_IE_WPA2_t* pNextElement);

#ifdef WPA 
// Add RSN IE to a frame body
UINT16 AddRSN_IE(vmacApInfo_t *vmacSta_p,IEEEtypes_RSN_IE_t* pNextElement);
#endif

#ifdef IEEE80211H
void bcngen_AddChannelSwithcAnnouncement_IE(vmacApInfo_t *vmacSta_p,IEEEtypes_ChannelSwitchAnnouncementElement_t *pChannelSwitchAnnouncementIE);
void bcngen_RemoveChannelSwithcAnnouncement_IE(vmacApInfo_t *vmacSta_p);
void bcngen_AddQuiet_IE(vmacApInfo_t *vmacSta_p,IEEEtypes_QuietElement_t *pQuietIE);
void bcngen_RemoveQuiet_IE(vmacApInfo_t *vmacSta_p);
#endif /* IEEE80211H */

#ifdef UR_WPA
void InitThisStaRsnIEUr(vmacApInfo_t *vmacSta_p);
#endif
//add by guijinlin for attack detection begin
UINT16 AddHT_IE_Ext(vmacApInfo_t *vmacSta_p,IEEEtypes_HT_Element_t* pNextElement,UINT32 SupportedMCSMask);
//add by guijinlin for attack detection end
UINT16 AddHT_IE(vmacApInfo_t *vmacSta_p,IEEEtypes_HT_Element_t* pNextElement);
UINT16 AddRSN_IE_TO(IEEEtypes_RSN_IE_t *thisStaRsnIE_p, IEEEtypes_RSN_IE_t* pNextElement);
extern UINT16 AddM_IE(vmacApInfo_t *vmacSta_p, IEEEtypes_HT_Element_t* pNextElement);
extern UINT16 Add_Generic_AddHT_IE(vmacApInfo_t *vmacSta_p, IEEEtypes_Generic_Add_HT_Element_t* pNextElement);
extern UINT16 Add_Generic_HT_IE(vmacApInfo_t *vmacSta_p,IEEEtypes_Generic_HT_Element_t* pNextElement);
extern UINT16 AddAddHT_IE(vmacApInfo_t *vmacSta_p,IEEEtypes_Add_HT_Element_t* pNextElement);
extern void InitThisStaRsnIE(vmacApInfo_t *vmacSta_p);
extern UINT16 AddM_Rptr_IE(vmacApInfo_t *vmacSta_p, IEEEtypes_HT_Element_t* pNextElement);
#ifdef IEEE80211H
extern UINT8 bcn_reg_domain;
#endif /* IEEE80211H */
extern UINT16 AddRSN_IEWPA2MixedMode(vmacApInfo_t *vmacSta_p,IEEEtypes_RSN_IE_WPA2MixedMode_t* pNextElement);
UINT16 AddRSN_IEWPA2(vmacApInfo_t *vmacSta_p,IEEEtypes_RSN_IE_WPA2_t* pNextElement);
#ifdef MRVL_WPS2
UINT16 Build_AssocResp_WSCIE(vmacApInfo_t *vmacSta_p, AssocResp_WSCIE_t *pNextElement);
#endif

#endif /* _BCNGEN_H_ */
