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

#include <linux/version.h>
#include <linux/ip.h>
#include <linux/igmp.h>
#include <linux/if_vlan.h>
#include "Fragment.h"

#include "keyMgmt_if.h"
#include "wldebug.h"
#include "ap8xLnxRegs.h"
#include "ap8xLnxDesc.h"
#include "ap8xLnxIntf.h"
#include "ap8xLnxRecv.h"
#include "ap8xLnxXmit.h"
#include "ap8xLnxFwcmd.h"
#include "ap8xLnxWlLog.h"
#include "wltypes.h"
#include "wl_macros.h"
#include "IEEE_types.h"
#include "wl_mib.h"
#include "wl_hal.h"
#include "StaDb.h"
#include "ds.h"
#include "ap8xLnxDma.h"
#include "buildModes.h"
#include "macmgmtap.h"
#include "macMgmtMlme.h"
#ifdef DYNAMIC_BA_SUPPORT
#include "wltypes.h"
#include "osif.h"
#include "timer.h"
#endif
#include "wds.h"
#include "mlmeApi.h"
#include "keyMgmtSta.h"
#include "linkmgt.h"
#include "ap8xLnxIoctlWiress.h"
#include "wllog.h"
#include "ap8xLnxThinap.h"
#include "ap8xLnxMacBinding.h"
#define	IEEE80211_ADDR_LEN	6
struct ieee80211_frame {
	u_int8_t	i_fc[2];
	u_int8_t	i_dur[2];
	u_int8_t	i_addr1[IEEE80211_ADDR_LEN];
	u_int8_t	i_addr2[IEEE80211_ADDR_LEN];
	u_int8_t	i_addr3[IEEE80211_ADDR_LEN];
	u_int8_t	i_seq[2];
	u_int8_t	i_addr4[IEEE80211_ADDR_LEN];
} PACK;


int ieee80211_autelan_ip_mac_binding_process(struct sk_buff *skb, extStaDb_StaInfo_t *pStaInfo, struct net_device *dev, UINT16 frametype)
{
    UINT16 hdrspace;
    struct wlprivate *wlpptr = NETDEV_PRIV_P(struct wlprivate, dev);

    if (skb == NULL || pStaInfo == NULL)
    {
        WlLogPrint(MARVEL_DEBUG_WARNING, __func__,"netdev is NULL,recv packet error\n");
        return 0;
    }

    hdrspace = sizeof(struct ieee80211_frame);
    
    if((pStaInfo->State != ASSOCIATED) && !pStaInfo->AP)
    {
        if (pStaInfo->ni_ipaddr[0] == 0x00 && pStaInfo->ni_ipaddr[1] == 0x00 && pStaInfo->ni_ipaddr[2] == 0x00 && pStaInfo->ni_ipaddr[3] == 0x00)
        {
            pStaInfo->ni_ip_auto_set = 0;
        }
        else
        {
            pStaInfo->ni_ip_auto_set = 1;
        }

        if ((pStaInfo->ni_ip_auto_set == 1)
         || ((wlpptr->vap.vap_ip_auto_learning == 1) && (pStaInfo->ni_ip_auto_set == 0)))
        {
            if (frametype == ETHERTYPE_IP)
            {
                struct iphdr *ip_h = (struct iphdr *)(skb->data + hdrspace + LLC_SNAPFRAMELEN);
                if (memcmp(&(ip_h->saddr), pStaInfo->ni_ipaddr, 4) != 0)
                {
                    if ((wlpptr->vap.vap_ip_auto_learning == 1) && (pStaInfo->ni_ip_auto_set == 0))
                    {
                        memcpy(pStaInfo->ni_ipaddr, &(ip_h->saddr), 4);
                        pStaInfo->ni_ip_auto_set = 1;
                    }
                    else
                    {
                        WlLogPrint(MARVEL_DEBUG_WARNING, __func__,"ip mac binding process error\n");
                        return -1;
                    }
                }
            }
        }
    }

    return 0;
}

int ieee80211_autelan_vlan_recv_mac_binding_process(struct sk_buff *skb, extStaDb_StaInfo_t *pStaInfo)
{
    int ret = 0;
    
    if (skb == NULL || pStaInfo == NULL)
    {
        //WlLogPrint(MARVEL_DEBUG_ALL, __func__, "Error\n");
        return ret;
    }

    if((pStaInfo->State != ASSOCIATED) && !pStaInfo->AP)
    {
        if ((pStaInfo->ni_vid > 0) && (pStaInfo->ni_vid <= VLAN_VID_MASK))
        {
            skb = vlan_put_tag(skb, pStaInfo->ni_vid);
            if (!skb)
            {
                WlLogPrint(MARVEL_DEBUG_WARNING, __func__, "skb is NULL,recv packet error\n");
                ret = -1;
            }
        }
    }

    return ret;
}


int ieee80211_autelan_vlan_send_mac_binding_process(struct sk_buff *skb, extStaDb_StaInfo_t *pStaInfo)
{
    int ret = 0;

    if (skb == NULL || pStaInfo == NULL)
    {
        //WlLogPrint(MARVEL_DEBUG_ALL, __func__, "Error\n");
        return ret;
    }

    if((pStaInfo->State != ASSOCIATED) && !pStaInfo->AP)
    {
        if (((pStaInfo->ni_vid > 0) && (pStaInfo->ni_vid <= VLAN_VID_MASK))/* || !(pStaInfo->AP)*/)
        {
            unsigned char *rawp = NULL;
            struct vlan_ethhdr *veth = (struct vlan_ethhdr *)(skb->data);
            unsigned short vid;
            unsigned short vlan_TCI;
            __be16 proto;

            if (veth->h_vlan_proto == ETH_P_8021Q)
            {
                vlan_TCI = ntohs(veth->h_vlan_TCI);
                vid = (vlan_TCI & VLAN_VID_MASK);

                if ((vid != pStaInfo->ni_vid) && !(pStaInfo->AP))
                {
                    ret = -1;
                }

                rcu_read_lock();

                /* Take off the VLAN header (4 bytes currently) */
                skb_pull(skb, VLAN_HLEN); /*lyb add for wndap350 vlan bug*/

                /* Need to correct hardware checksum */
                //skb_postpull_rcsum(wbuf, vhdr, VLAN_HLEN); /*lyb add for wndap350 vlan bug*/

                /*  Was a VLAN packet, grab the encapsulated protocol, which the layer
                 * three protocols care about.
                 */
                proto = veth->h_vlan_encapsulated_proto;

                skb->protocol = proto;
                if (ntohs(proto) < 1536)
                {
                    rawp = skb->data + ETH_HLEN;
                    if (*(unsigned short *)rawp == 0xFFFF)
                    {
                        skb->protocol = __constant_htons(ETH_P_802_3);
                    }
                    else
                    {
                        skb->protocol = __constant_htons(ETH_P_802_2);
                    }
                }

                if (skb_shared(skb) || skb_cloned(skb)) {
                    struct sk_buff *nskb = skb_copy(skb, GFP_ATOMIC);
                    kfree_skb(skb);
                    skb = nskb;
                }
                if (skb) {
                    memmove(skb->data,skb->data - VLAN_HLEN, 12);
                    #if LINUX_VERSION_CODE <= KERNEL_VERSION(2,6,20)
                    skb->mac.raw += VLAN_HLEN;
                    #else
                    skb->mac_header += VLAN_HLEN;
                    #endif
                }

                rcu_read_unlock();
            }
            else if (!(pStaInfo->AP))
            {
                ret = -1;
            }
        }
    }

    return ret;
}


