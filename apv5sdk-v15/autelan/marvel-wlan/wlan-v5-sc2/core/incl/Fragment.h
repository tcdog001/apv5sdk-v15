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

/******************** (c) Marvell Semiconductor, Inc., 2001 *******************
*
* Purpose:
*    This file contains the definitions of the fragment module
*
*****************************************************************************/
#ifndef  __FRAGMENT_H__
#define __FRAGMENT_H__
#include "StaDb.h"
//=============================================================================
//                               INCLUDE FILES
//=============================================================================

//=============================================================================
//                                DEFINITIONS
//=============================================================================
#define MIN_HEAD_ROOM  64
#define IEEE80211_SEQ_FRAG_SHIFT 0
#define wbuf_trim(_skb, _len)             skb_trim(_skb, ((_skb)->len - (_len)))
#define IEEE80211_IS_MULTICAST(_a)  (*(_a) & 0x01)
#define htole16(x)	cpu_to_le16(x)
#define wbuf_set_pktlen(_wbuf, _len)         __wbuf_set_pktlen(_wbuf, _len)
#define FRAG_SEND 1
#define FRAG_FAIL 0

//=============================================================================
//                          PUBLIC TYPE DEFINITIONS
//=============================================================================
//=============================================================================
//                    PUBLIC PROCEDURES (ANSI Prototypes)
//=============================================================================
extern struct sk_buff *DeFragPck(struct net_device *dev,struct sk_buff *skb, extStaDb_StaInfo_t **pStaInfo);
bool ieee80211_check_and_fragment(struct net_device *dev, struct sk_buff *skb, extStaDb_StaInfo_t *pStaInfo);

#endif/* __FRAGMENT_H__ */
