/*
*                Copyright 2012, Marvell Semiconductor, Inc.
* This code contains confidential information of Marvell Semiconductor, Inc.
* No rights are granted herein under any patent, mask work right or copyright
* of Marvell or any third party.
* Marvell reserves the right at its sole discretion to request that this code
* be immediately returned to Marvell. This code is provided "as is".
* Marvell makes no warranties, express, implied or otherwise, regarding its
* accuracy, completeness or performance.
*/

#ifndef AP8X_LNX_MAC_BINDING_H_
#define AP8X_LNX_MAC_BINDING_H_

int ieee80211_autelan_ip_mac_binding_process(struct sk_buff *skb, extStaDb_StaInfo_t *pStaInfo, struct net_device *dev, UINT16 frametype);
int ieee80211_autelan_vlan_recv_mac_binding_process(struct sk_buff *skb, extStaDb_StaInfo_t *pStaInfo);
int ieee80211_autelan_vlan_send_mac_binding_process(struct sk_buff *skb, extStaDb_StaInfo_t *pStaInfo);

#endif
