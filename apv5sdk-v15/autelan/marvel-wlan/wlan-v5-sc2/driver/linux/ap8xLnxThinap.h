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

#ifndef AP8X_LNX_THINAP_H_
#define AP8X_LNX_THINAP_H_

#include "ap8xLnxIntf.h"
#include "IEEE_types.h"

#define AP8X_FATAP_MODE                                     0
#define AP8X_THINAP_MODE_SKB_RELOCATE_FAIL                  1
#define AP8X_THINAP_MODE_NETIF_RX_DROP                      2
#define AP8X_THINAP_MODE_ERR                                3
#define AP8X_THINAP_MODE_THINAPOUT                          4
#define AP8X_THINAP_MODE_TUNNEL_SUCC                        5
#define AP8X_THINAP_MODE_TUNNEL_DROP                        6
#define AP8X_THINAP_MODE_SEND                               7
#define AP8X_THINAP_MODE_LOCAL_SUCC                         8

#define AP8X_80211_DATA_SUCC                                   0
#define AP8X_80211_DATA_DROP                                  -1
#define AP8X_80211_DATA_DEAUTH                                -2

#define IEEE80211_FC0_TYPE_MASK             0x0c
#define IEEE80211_FC0_TYPE_DATA             0x08
#define IEEE80211_FC0_SUBTYPE_QOS       0x80

/* does frame have QoS sequence control data */
#define IEEE80211_QOS_HAS_SEQ(wh) \
    ((((((wh->FrmCtl.Subtype) & 0xF) << 4)|(((wh->FrmCtl.Type) & 0xF) << 2)|(wh->FrmCtl.ProtocolVersion)) & \
      (IEEE80211_FC0_TYPE_MASK | IEEE80211_FC0_SUBTYPE_QOS)) == \
      (IEEE80211_FC0_TYPE_DATA | IEEE80211_FC0_SUBTYPE_QOS))


#ifdef AE5200_RT_SUPPORT
/* gengzongjie added for traffic balance to add band and rssi in probreq */
#define AUTE_OUI_BYTES		            0x00, 0x1f, 0x64
#define BAND_OUI_TYPE                   0x01
#define RSSI_OUI_SUBTYPE                0x02
/* gengzongjie added end */

typedef  struct IEEEtypes_Extend_BAND_RSSI_Element_t 
{
	UINT8 ElementId;
	IEEEtypes_Len_t Len;
	UINT8 OUI[3];
	UINT8 OUIType;
	UINT8 TypeLen;
    UINT8 TypeValue;
	UINT8 OUISubType;
	UINT8 SubTypeLen;
    UINT8 SubTypeValue;
} PACK_END IEEEtypes_Extend_BAND_RSSI_Element_t;

#endif

int ieee80211_hdrspace(const void *data);
int dhcp_detect_wh(struct ieee80211vap *vap, struct sk_buff *skb);
int pppoe_detect_wh(struct ieee80211vap *vap, struct sk_buff *skb);

#ifdef AE5200_RT_SUPPORT
int dhcp_detect_wh1(struct ieee80211vap *vap, struct sk_buff *skb);
int arp_detect_wh(struct ieee80211vap *vap, struct sk_buff *skb);
int dns_detect_wh(struct ieee80211vap *vap, struct sk_buff *skb);

int ieee80211_autelan_recv_managent_frame_thinap_process(struct sk_buff *skb, struct net_device *dev, int subtype,vmacApInfo_t * vmactem_p,UINT32 RSSI);
int ieee80211_autelan_recv_data_frame_ubc_mode(struct sk_buff **skb, struct net_device *dev, extStaDb_StaInfo_t *pStaInfo, UINT16 frametype);
#else
int ieee80211_autelan_recv_managent_frame_thinap_process(struct sk_buff *skb, struct net_device *dev, int subtype);
#endif
extern struct sk_buff *ieee80211_decap(struct net_device *dev, struct sk_buff *skb, extStaDb_StaInfo_t *pStaInfo);
int ieee80211_autelan_recv_data_frame_80211_tunnel_mode(struct sk_buff **skb, struct net_device *dev, extStaDb_StaInfo_t *pStaInfo, UINT16 frametype);
int ieee80211_autelan_recv_data_frame_8023_tunnel_mode(struct sk_buff **skb, struct net_device *dev, extStaDb_StaInfo_t *pStaInfo, UINT16 frametype);
int ieee80211_autelan_recv_data_frame_local_mode(struct sk_buff **skb, struct net_device *dev, extStaDb_StaInfo_t *pStaInfo, UINT16 frametype);
int ieee80211_autelan_send_data_frame_local_mode(struct sk_buff *skb, vmacApInfo_t *vmacSta_p, BOOLEAN *eapolPkt, int *type,extStaDb_StaInfo_t **pStaInfo_tmp);
int ieee80211_autelan_recv_data_frame_thinap_process(struct sk_buff **skb, struct net_device *dev, extStaDb_StaInfo_t *pStaInfo, UINT16 frametype);
int ieee80211_autelan_send_data_frame_thinap_process(struct sk_buff *skb, vmacApInfo_t *vmacSta_p, UINT32 tunnelmode, BOOLEAN *eapolPkt, int *type,extStaDb_StaInfo_t **pStaInfo_tmp);
UINT16 ieee80211_autelan_get_3_address_packet_llc_analysis(struct sk_buff *skb);
int ieee80211_autelan_recv_data_frame_filter_process(struct sk_buff *skb, struct net_device *dev, extStaDb_StaInfo_t *pStaInfo);
int ieee80211_autelan_send_data_ccmp_process(struct sk_buff *skb, vmacApInfo_t *vmacSta_p);
void dumppacket(struct sk_buff *skb);
#endif

