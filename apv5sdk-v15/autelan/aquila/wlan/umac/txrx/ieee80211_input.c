/*
 *  Copyright (c) 2008 Atheros Communications Inc.
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include <ieee80211_txrx_priv.h>

#include <if_llc.h>
#include <if_athproto.h>
#include <if_upperproto.h>
#include <ieee80211_aow.h>

#ifdef ATH_EXT_AP
#include <ieee80211_extap.h>
#endif

#ifdef CONFIG_RCPI
#include <if_athvar.h> /* for definition of ATH_NODE_NET80211(ni) */
#endif /* CONFIG_RCPI */
#if ATH_SW_WOW
#include <ieee80211_wow.h>
#endif
#include "ieee80211_vi_dbg.h"

/*zhaoyang1 transplant from 717*/
/*Begin:Added by duanmingzhe for thinap*/
#include <osif_private.h>
#include <ieee80211_api.h>
#include "ieee80211_defines.h"

/*Begin: Added by WangJia, for traffic limit. 2012-11-02.*/
#include "ieee80211_traffic_limit.h"
/*End: Added by WangJia, for traffic limit. 2012-11-02.*/

#define OSIF_TO_NETDEV(_osif) (((osif_dev *)(_osif))->netdev)
/*End:Added by duanmingzhe for thinap*/
/*Begin:added by pengruofeng for counting of error frames 2011-5-11*/
static unsigned char is_err_frame = 0;
/*End:added by pengruofeng for counting of error frames 2011-5-11*/

#include <linux/ip.h>  /*Added by duanmingzhe for ip mac binding*/
/*zhaoyang1 transplant end*/

/*Start: Add by zhanghu to statistics of every frame by the rate*/ 
#include <if_athvar.h>
/*End: Add by zhanghu to statistics of every frame by the rate*/ 
/* Begin:gengzj added for suppressing illegal ap 2013-11-27 */
/*zhaoyang1 add start for interfering ap*/
#include "ieee80211_proto.h"
/*zhaoyang1 add end*/
/* End:gengzj added end */
typedef enum {
    FILTER_STATUS_ACCEPT = 0,
    FILTER_STATUS_REJECT
} ieee80211_privasy_filter_status;

#define IS_SNAP(_llc) ((_llc)->llc_dsap == LLC_SNAP_LSAP && \
                        (_llc)->llc_ssap == LLC_SNAP_LSAP && \
                        (_llc)->llc_control == LLC_UI)
#define RFC1042_SNAP_NOT_AARP_IPX(_llc) \
            ((_llc)->llc_snap.org_code[0] == RFC1042_SNAP_ORGCODE_0 && \
            (_llc)->llc_snap.org_code[1] == RFC1042_SNAP_ORGCODE_1 && \
            (_llc)->llc_snap.org_code[2] == RFC1042_SNAP_ORGCODE_2 \
            && !((_llc)->llc_snap.ether_type == htons(ETHERTYPE_AARP) || \
                (_llc)->llc_snap.ether_type == htons(ETHERTYPE_IPX)))
#define IS_BTEP(_llc) ((_llc)->llc_snap.org_code[0] == BTEP_SNAP_ORGCODE_0 && \
            (_llc)->llc_snap.org_code[1] == BTEP_SNAP_ORGCODE_1 && \
            (_llc)->llc_snap.org_code[2] == BTEP_SNAP_ORGCODE_2)
#define IS_ORG_BTAMP(_llc) ((_llc)->llc_snap.org_code[0] == BTAMP_SNAP_ORGCODE_0 && \
                            (_llc)->llc_snap.org_code[1] == BTAMP_SNAP_ORGCODE_1 && \
                            (_llc)->llc_snap.org_code[2] == BTAMP_SNAP_ORGCODE_2)
#define IS_ORG_AIRONET(_llc) ((_llc)->llc_snap.org_code[0] == AIRONET_SNAP_CODE_0 && \
                               (_llc)->llc_snap.org_code[1] == AIRONET_SNAP_CODE_1 && \
                               (_llc)->llc_snap.org_code[2] == AIRONET_SNAP_CODE_2)

extern struct ieee80211_autelan_addr autelan_addr[8];
static int ieee80211_qos_decap(struct ieee80211vap *vap, wbuf_t wbuf, int hdrlen);
static void ieee80211_input_data(struct ieee80211_node *ni, wbuf_t wbuf, 
                     struct ieee80211_rx_status *rs, int subtype, int dir);
static ieee80211_privasy_filter_status ieee80211_check_privacy_filters(struct ieee80211_node *ni, wbuf_t wbuf, int is_mcast);
static void ieee80211_deliver_data(struct ieee80211vap *vap, wbuf_t wbuf, struct ieee80211_node *ni, struct ieee80211_rx_status *rs,
                                          u_int32_t hdrspace, int is_mcast, u_int8_t subtype);
static wbuf_t ieee80211_decap(struct ieee80211vap *vap, wbuf_t wbuf, size_t hdrspace, struct ieee80211_rx_status *rs);

/*zhaoyang1 transplant from 717*/
/*pengruofeng--add begin for autelan private wmm 2011-6-1*/
/*
 * this function used to turn wmm priority to 1p priority.
 */
int wmm_to_vlan_map(int wmm_priority, struct ieee80211vap *vap) 
{
	if (wmm_priority > 3 || wmm_priority < 0 || vap == NULL) {
		return 0;
	}
	return (vap->priv_wmm).vlan_ingress_priority[wmm_priority];
}


/*Start: Add by zhanghu to statistics of every frame by the rate*/ 
int32_t ieee80211_ni_statistics_rate_for_rx_frame(struct ieee80211_node *ni)
{

     u_int32_t   rateKbps = ATH_NODE_NET80211(ni)->an_lastrxrate;
     u_int8_t    rateCode = ATH_NODE_NET80211(ni)->an_rxratecode;
     u_int8_t    dot11Rate = (rateKbps*2)/1000;
 
     if(rateCode & IEEE80211_MCS0){
         (ni->ni_rx_mcs_count[rateCode & ~IEEE80211_MCS0])++;
     }else if(2 == dot11Rate){
         (ni->ni_rx_rate_index[0].count)++;
     }else if(4 == dot11Rate){
         (ni->ni_rx_rate_index[1].count)++;
     }else if(11 == dot11Rate){
         (ni->ni_rx_rate_index[2].count)++;
     }else if(22 == dot11Rate){
         (ni->ni_rx_rate_index[3].count)++;
     }else{
         u_int8_t    left = 0;
         u_int8_t    right = 7;
         u_int8_t    middle = (left + right)/2;
         
         while(left <= right && left != 0xFF && right != 0xFF){            
             if(ni->ni_rx_rate_index[middle + 4].dot11Rate < dot11Rate){
                 left = middle + 1;
             }else if(ni->ni_rx_rate_index[middle + 4].dot11Rate > dot11Rate){
                 right = middle - 1;
             }else{
                 (ni->ni_rx_rate_index[middle + 4].count)++;
                 break; 
             }
                         
             middle = (left + right)/2;
         }
         
         if(left > right || left == 0xFF || right == 0xFF){
             //printk("%s:%d is not found\n", __func__, dot11Rate);
             return -1;
         }   
     }
     
     return 0;
}
 

int32_t ieee80211_vap_statistics_rate_for_rx_frame(struct ieee80211vap *vap, struct ieee80211_rx_status *rs)
{
    u_int8_t    rateCode = rs->rs_ratephy;
    u_int32_t   rateKpbs = rs->rs_datarate;
    u_int8_t    dot11Rate = (rateKpbs*2)/1000;

    if(rateCode & IEEE80211_MCS0){
        (vap->iv_rx_mcs_count[rateCode & ~IEEE80211_MCS0])++;
    }else if(2 == dot11Rate){
        (vap->iv_rx_rate_index[0].count)++;
    }else if(4 == dot11Rate){
        (vap->iv_rx_rate_index[1].count)++;
    }else if(11 == dot11Rate){
        (vap->iv_rx_rate_index[2].count)++;
    }else if(22 == dot11Rate){
        (vap->iv_rx_rate_index[3].count)++;
    }else{
        u_int8_t    left = 0;
        u_int8_t    right = 7;
        u_int8_t    middle = (left + right)/2;
        
        while(left <= right && left != 0xFF && right != 0xFF){
            if(vap->iv_rx_rate_index[middle + 4].dot11Rate < dot11Rate){
                left = middle + 1;
            }else if(vap->iv_rx_rate_index[middle + 4].dot11Rate > dot11Rate){
                right = middle - 1;
            }else{
                (vap->iv_rx_rate_index[middle + 4].count)++;
                break; 
            }
            
            middle = (left + right)/2;
        }

        if(left > right || left == 0xFF || right == 0xFF){
            printk("%s:%d is not found\n", __func__, dot11Rate);
            return -1;
        }   
    }

    return 0;
}
/*End: Add by zhanghu to statistics of every frame by the rate*/

/*
 * this function used to turn wmm priority to tos.
 */
int wmm_to_tos_map(int wmm_priority, struct ieee80211vap *vap) 
{
	if (wmm_priority > 3 || wmm_priority < 0 || vap == NULL) {
		return 0;
	}
	return (vap->priv_wmm).tos_ingress_priority[wmm_priority];
}
/*pengruofeng--add end 2011-6-1*/


/*Begin:Added by duanmingzhe for through the dhcp packet*/
int
dhcp_detect_wh(struct ieee80211vap *vap, struct sk_buff *skb)
{	
	if (vap->vap_dhcp_enable) {
		u_int8_t *wh = skb->data;
		int hdrspace = ieee80211_hdrspace(vap->iv_ic, wh);
		int ip_len = sizeof(struct iphdr);
		
		if ((skb->len > (hdrspace + LLC_SNAPFRAMELEN + ip_len)) &&
			((*(wh + hdrspace + LLC_SNAPFRAMELEN + ip_len + 1) == 67 && *(wh + hdrspace + LLC_SNAPFRAMELEN + ip_len + 3) == 68) ||
			(*(wh + hdrspace + LLC_SNAPFRAMELEN + ip_len + 1) == 68 && *(wh + hdrspace + LLC_SNAPFRAMELEN + ip_len + 3) == 67)))
			return 1;
		else
			return 0;
	}
	else
		return 0;
}

int
dhcp_detect_eth(struct ieee80211vap *vap, struct sk_buff *skb)
{
	if (vap->vap_dhcp_enable) {
		u_int8_t *eh = skb->data;
		int ip_len = sizeof(struct iphdr);
		
		if ((skb->len > (ETH_HLEN + ip_len)) &&
			((*(eh + ETH_HLEN + ip_len + 1) == 67 && *(eh + ETH_HLEN + ip_len + 3) == 68) ||
			(*(eh + ETH_HLEN + ip_len + 1) == 68 && *(eh + ETH_HLEN + ip_len + 3) == 67)))
			return 1;
		else 
			return 0;
	}
	else
		return 0;
}
/*End:Added by duanmingzhe for through the dhcp packet*/

/*zhaoyang add for through the pppoe frame*/
int
pppoe_detect_wh(struct ieee80211vap *vap, struct sk_buff *skb)
{	
	if (vap->vap_pppoe_enable)
	{
		struct ieee80211_frame *wh = (struct ieee80211_frame *)skb->data;

		if ((wh->i_fc[0] & IEEE80211_FC0_TYPE_MASK) == IEEE80211_FC0_TYPE_DATA)
		{
			int hdrspace = ieee80211_hdrspace(vap->iv_ic, wh);
			struct llc *llc_type = (struct llc *)(skb->data + hdrspace);

			if ((llc_type->llc_un.type_snap.ether_type == __constant_htons(ETH_P_PPP_DISC)) ||
				(llc_type->llc_un.type_snap.ether_type == __constant_htons(ETH_P_PPP_SES)))
				return 1;
		}
	}

	return 0;
}

int
pppoe_detect_eth(struct ieee80211vap *vap, struct sk_buff *skb)
{
	if (vap->vap_pppoe_enable)
	{
		struct ether_header *eh = (struct ether_header *)skb->data;

		if ((eh->ether_type == __constant_htons(ETH_P_PPP_DISC)) ||
			(eh->ether_type == __constant_htons(ETH_P_PPP_SES)))
			return 1;
	}

	return 0;
}
/*zhaoyang add end*/
/*zhaoyang1 transplant end*/

/*
 * check the frame against the registered  privacy flteres. 
 * returns 1 if the frame needs to be filtered out.
 * returns 0 if the frame needs to be indicated up.
 */

static ieee80211_privasy_filter_status
ieee80211_check_privacy_filters(struct ieee80211_node *ni, wbuf_t wbuf, int is_mcast)
{
    struct ieee80211vap *vap = ni->ni_vap;
    struct llc *llc;
    u_int16_t ether_type = 0;
    u_int32_t hdrspace;
    u_int32_t i;
    struct ieee80211_frame *wh;
    ieee80211_privacy_filter_packet_type packet_type;
    u_int8_t is_encrypted;

    /* Safemode must avoid the PrivacyExemptionList and ExcludeUnencrypted checking */
    if (IEEE80211_VAP_IS_SAFEMODE_ENABLED(vap)) {
        return FILTER_STATUS_ACCEPT;
    }

    wh = (struct ieee80211_frame *) wbuf_header(wbuf);

    if (IEEE80211_VAP_IS_DELIVER_80211_ENABLED(vap)) {
        /* QoS-Ctrl & Padding bytes have already been stripped off */
        hdrspace = ieee80211_hdrsize(wbuf_header(wbuf));

    } else {
        hdrspace = ieee80211_hdrspace(vap->iv_ic, wbuf_header(wbuf));
    }
    if (wbuf_get_pktlen(wbuf)  < ( hdrspace + LLC_SNAPFRAMELEN)) {
        IEEE80211_DISCARD_MAC(vap, IEEE80211_MSG_INPUT,
                            wh->i_addr2, "data",
                            "%s: too small packet 0x%x len %u \n",__func__,
                            ether_type, wbuf_get_pktlen(wbuf));
        return FILTER_STATUS_REJECT; /* filter the packet */
    }

    llc = (struct llc *)(wbuf_header(wbuf) + hdrspace);
    if (IS_SNAP(llc) && (RFC1042_SNAP_NOT_AARP_IPX(llc) || IS_ORG_BTAMP(llc) ||
        IS_ORG_AIRONET(llc))) {
        ether_type = ntohs(llc->llc_snap.ether_type);
    } else {
        ether_type = htons(wbuf_get_pktlen(wbuf) - hdrspace);
    }

    is_encrypted = (wh->i_fc[1] & IEEE80211_FC1_WEP);
    wh->i_fc[1] &= ~IEEE80211_FC1_WEP; /* XXX: we don't need WEP bit from here */
    
    if (is_mcast) {
        packet_type = IEEE80211_PRIVACY_FILTER_PACKET_MULTICAST;
    } else {
        packet_type = IEEE80211_PRIVACY_FILTER_PACKET_UNICAST;
    }

    for (i=0; i < vap->iv_num_privacy_filters; i++) {
        /* skip if the ether type does not match */
        if (vap->iv_privacy_filters[i].ether_type != ether_type)
            continue;

        /* skip if the packet type does not match */
        if (vap->iv_privacy_filters[i].packet_type != packet_type &&
            vap->iv_privacy_filters[i].packet_type != IEEE80211_PRIVACY_FILTER_PACKET_BOTH) 
            continue;
        
        if (vap->iv_privacy_filters[i].filter_type == IEEE80211_PRIVACY_FILTER_ALLWAYS) {
            /*
             * In this case, we accept the frame if and only if it was originally
             * NOT encrypted.
             */
            if (is_encrypted) {
               IEEE80211_DISCARD_MAC(vap, IEEE80211_MSG_INPUT,
                            wh->i_addr2, "data",
                            "%s: packet encrypted ether type 0x%x len %u \n",__func__,
                            ether_type, wbuf_get_pktlen(wbuf));
                return FILTER_STATUS_REJECT;
            } else {
                return FILTER_STATUS_ACCEPT;
            }
        } else if (vap->iv_privacy_filters[i].filter_type  == IEEE80211_PRIVACY_FILTER_KEY_UNAVAILABLE) {
            /*
             * In this case, we reject the frame if it was originally NOT encrypted but 
             * we have the key mapping key for this frame.
             */
            if (!is_encrypted && !is_mcast && ni->ni_ucastkey.wk_valid) {
               IEEE80211_DISCARD_MAC(vap, IEEE80211_MSG_INPUT,
                            wh->i_addr2, "data",
                            "%s: node has a key ether type 0x%x len %u \n",__func__,
                            ether_type, wbuf_get_pktlen(wbuf));
                return FILTER_STATUS_REJECT;
            } else {
                return FILTER_STATUS_ACCEPT;
            }
        } else {
            /*
             * The privacy exemption does not apply to this frame.
             */
            break;
        }
    }

    /*
     * If the privacy exemption list does not apply to the frame, check ExcludeUnencrypted.
     * if ExcludeUnencrypted is not set, or if this was oringially an encrypted frame, 
     * it will be accepted.
     */
    if (!IEEE80211_VAP_IS_DROP_UNENC(vap) || is_encrypted) {
        /*
         * if the node is not authorized 
         * reject the frame.
         */

		/*zhaoyang1 transplant from 717*/
		//zhaoyang modify for thought the dhcp packet (add !dhcp_detect_wh(vap,wbuf))
		//zhaoyang add for through the pppoe frame (add !pppoe_detect_wh(vap,wbuf))
        if (!ieee80211_node_is_authorized(ni) && !dhcp_detect_wh(vap,wbuf) && !pppoe_detect_wh(vap,wbuf)) {
		/*zhaoyang1 transplant end*/
            IEEE80211_DISCARD_MAC(vap, IEEE80211_MSG_INPUT,
                                  wh->i_addr2, "data",
                                  "unauthorized port: ether type 0x%x len %u \n",
                                  ether_type, wbuf_get_pktlen(wbuf));
            vap->iv_stats.is_rx_unauth++;
            return FILTER_STATUS_REJECT;
        }
        return FILTER_STATUS_ACCEPT;
    }

    if (!is_encrypted && IEEE80211_VAP_IS_DROP_UNENC(vap)) {
        if (is_mcast) {
            vap->iv_multicast_stats.ims_rx_unencrypted++;
            vap->iv_multicast_stats.ims_rx_decryptcrc++;
        } else {
            vap->iv_unicast_stats.ims_rx_unencrypted++;
            vap->iv_unicast_stats.ims_rx_decryptcrc++;
        }
    }

    IEEE80211_DISCARD_MAC(vap, IEEE80211_MSG_INPUT,
                            wh->i_addr2, "data",
                          "%s: ether type 0x%x len %u \n",__func__,
                             ether_type, wbuf_get_pktlen(wbuf));
    return FILTER_STATUS_REJECT;
}

int
ieee80211_amsdu_input(struct ieee80211_node *ni, 
                      wbuf_t wbuf, struct ieee80211_rx_status *rs,
                      int is_mcast, u_int8_t subtype)
{
#define AMSDU_LLC_SIZE  (sizeof(struct ether_header) + sizeof(struct llc))
    struct ieee80211vap *vap = ni->ni_vap;
    struct ieee80211com *ic = vap->iv_ic;
    struct ieee80211_phy_stats *phy_stats;
    struct ieee80211_mac_stats *mac_stats;
    struct ether_header *subfrm_hdr;
    u_int32_t subfrm_len, subfrm_datalen, frm_len = 0;
    u_int32_t hdrsize;
    wbuf_t wbuf_new, wbuf_subfrm, wbuf_save = wbuf;
    struct ieee80211_qosframe_addr4 *wh;

    phy_stats = &ic->ic_phy_stats[ic->ic_curmode];
    mac_stats = IEEE80211_IS_MULTICAST(wbuf_header(wbuf)) ? &vap->iv_multicast_stats : &vap->iv_unicast_stats;
	/*zhaoyang1 transplant from 717*/
    /*Begin:Added by duanmingzhe for information report*/	
    if(IEEE80211_IS_BROADCAST(wbuf_header(wbuf)))
    {
    	mac_stats = &vap->iv_broadcase_stats;
    }
    /*End:Added by duanmingzhe for information report*/	
	/*zhaoyang1 transplant end*/
    hdrsize = ieee80211_hdrspace(ic, wbuf_header(wbuf));
    /* point to ieee80211 header */
    wh = (struct ieee80211_qosframe_addr4 *)wbuf_header(wbuf);
    wbuf_pull(wbuf, hdrsize);

    frm_len = wbuf_get_pktlen(wbuf);
    while (wbuf_next(wbuf) != NULL) {
        wbuf = wbuf_next(wbuf);
        frm_len += wbuf_get_pktlen(wbuf);
    }

    wbuf = wbuf_save;
    
    while (frm_len >= AMSDU_LLC_SIZE) {
        if ((wbuf_get_pktlen(wbuf)) < AMSDU_LLC_SIZE) { /* wbuf left too less */
            if ((wbuf_new = wbuf_next(wbuf)) == NULL) {
                IEEE80211_DISCARD_MAC(vap, IEEE80211_MSG_ANY,
                    ni->ni_macaddr, "amsdu", 
                    "A-MSDU: pullup failed, len %u", frm_len);
                goto err_amsdu;
            }
            /* check new wbuf for data length */
            else if (wbuf_get_pktlen(wbuf_new) < AMSDU_LLC_SIZE) {
                goto err_amsdu;
            }
            wbuf = wbuf_new;    /* update wbuf */
        }

        subfrm_hdr = (struct ether_header *)wbuf_header(wbuf);
        subfrm_datalen = ntohs(subfrm_hdr->ether_type);
        subfrm_len = subfrm_datalen + sizeof(struct ether_header);

        if (subfrm_len < sizeof(LLC_SNAPFRAMELEN)) {
            IEEE80211_DISCARD_MAC(vap, IEEE80211_MSG_INPUT, 
                subfrm_hdr->ether_shost, "amsdu", 
                "A-MSDU sub-frame too short: len %u", 
                subfrm_len);
            goto err_amsdu;
        }

        if (frm_len != subfrm_len && frm_len < roundup(subfrm_len, 4)) {
            IEEE80211_DISCARD_MAC(vap, IEEE80211_MSG_INPUT, 
                subfrm_hdr->ether_shost, "amsdu", "Short A-MSDU: len %u", frm_len);
            goto err_amsdu;
        }

        wbuf_subfrm = wbuf_alloc(ic->ic_osdev, 
                                 WBUF_RX_INTERNAL, 
                                 subfrm_datalen+hdrsize);

        if (wbuf_subfrm == NULL) {
            IEEE80211_DISCARD_MAC(vap, IEEE80211_MSG_INPUT, 
                subfrm_hdr->ether_shost, "amsdu", "%s", "amsdu A-MSDU no memory");
            goto err_amsdu;
        }

        wbuf_init(wbuf_subfrm, subfrm_datalen + hdrsize);

        switch (wh->i_fc[1] & IEEE80211_FC1_DIR_MASK) {
        case IEEE80211_FC1_DIR_NODS:
            IEEE80211_ADDR_COPY(wh->i_addr1, subfrm_hdr->ether_dhost);
            IEEE80211_ADDR_COPY(wh->i_addr2, subfrm_hdr->ether_shost);
            break;

        case IEEE80211_FC1_DIR_TODS:
            IEEE80211_ADDR_COPY(wh->i_addr2, subfrm_hdr->ether_shost);
            IEEE80211_ADDR_COPY(wh->i_addr3, subfrm_hdr->ether_dhost);
            break;

        case IEEE80211_FC1_DIR_FROMDS:
            IEEE80211_ADDR_COPY(wh->i_addr1, subfrm_hdr->ether_dhost);
            IEEE80211_ADDR_COPY(wh->i_addr3, subfrm_hdr->ether_shost);
            break;

        case IEEE80211_FC1_DIR_DSTODS:
            IEEE80211_ADDR_COPY(wh->i_addr3, subfrm_hdr->ether_dhost);
            IEEE80211_ADDR_COPY(wh->i_addr4, subfrm_hdr->ether_shost);
            break;

        default:
            wbuf_free(wbuf_subfrm);
            goto err_amsdu;
        }
        
        wbuf_set_priority(wbuf_subfrm, wbuf_get_priority(wbuf));
        if (wbuf_is_qosframe(wbuf)) {
            wbuf_set_qosframe(wbuf_subfrm);
        }

        OS_MEMCPY(wbuf_header(wbuf_subfrm), wh, hdrsize);
        OS_MEMCPY(wbuf_header(wbuf_subfrm) + hdrsize, wbuf_header(wbuf) + sizeof(struct ether_header), subfrm_datalen);

        /* update stats for received frames (all fragments) */
        phy_stats->ips_rx_packets++;
        if (is_mcast)
            phy_stats->ips_rx_multicast++; 
        mac_stats->ims_rx_packets++;
		mac_stats->ims_rx_bytes += subfrm_len;

		/*zhaoyang1 transplant from 717*/
		/*zhaoyang modify for ni rx_data and tx_data count*/
		IEEE80211_NODE_STAT(ni, rx_data);
        IEEE80211_NODE_STAT_ADD(ni, rx_bytes,subfrm_len);
		/*zhaoyang modify end*/
		/*zhaoyang1 transplant end*/
		
        if (ieee80211_check_privacy_filters(ni, wbuf_subfrm, is_mcast) == FILTER_STATUS_REJECT) {
            wbuf_free(wbuf_subfrm);
        } else {
            ieee80211_deliver_data(vap, wbuf_subfrm, ni, rs, hdrsize, is_mcast, subtype);
        }

        if (frm_len > subfrm_len) {
            wbuf_pull(wbuf, roundup(subfrm_len, 4));/* point to next e_header */
            frm_len -= roundup(subfrm_len, 4); 
        } else {
            frm_len = 0;
        }
    }
	/* we count 802.11 header into our rx_bytes */
	mac_stats->ims_rx_bytes += hdrsize;
	/*zhaoyang1 transplant from 717*/
	/*zhaoyang modify for ni rx_data and tx_data count*/
	IEEE80211_NODE_STAT_ADD(ni, rx_bytes, hdrsize);
	/*zhaoyang modify end*/
	/*zhaoyang1 transplant end*/
err_amsdu:
    while (wbuf_save) {
	wbuf = wbuf_next(wbuf_save);
        wbuf_free(wbuf_save);
	wbuf_save = wbuf;
    }

    return IEEE80211_FC0_TYPE_DATA;
}

static int
ieee80211_qos_decap(struct ieee80211vap *vap, wbuf_t wbuf, int hdrlen)
{
    struct ieee80211_frame *wh;
    u_int32_t hdrsize;

    wh = (struct ieee80211_frame *)wbuf_header(wbuf);
    if (IEEE80211_QOS_HAS_SEQ(wh)) {
        u_int16_t qoslen = sizeof(struct ieee80211_qoscntl);
        u_int8_t *qos;
#ifdef ATH_SUPPORT_TxBF
        /* Qos frame with Order bit set indicates an HTC frame */
        if (wh->i_fc[1] & IEEE80211_FC1_ORDER) {
            qoslen += sizeof(struct ieee80211_htc);
            IEEE80211_DPRINTF(vap, IEEE80211_MSG_ANY,"==>%s: receive +HTC frame\n",__func__);
        }
#endif
        if (vap->iv_ic->ic_flags & IEEE80211_F_DATAPAD) {
            hdrsize = ieee80211_hdrsize(wh);

            /* If the frame has padding, include padding length as well */
            if ((hdrsize % sizeof(u_int32_t) != 0)) {
                qoslen = roundup(qoslen, sizeof(u_int32_t));
            }
        }

        if ((wh->i_fc[1] & IEEE80211_FC1_DIR_MASK) == IEEE80211_FC1_DIR_DSTODS) {
            qos = &((struct ieee80211_qosframe_addr4 *)wh)->i_qos[0];
        } else {
            qos = &((struct ieee80211_qosframe *)wh)->i_qos[0];
        }

        /* save priority */
        wbuf_set_qosframe(wbuf);
        wbuf_set_priority(wbuf, (qos[0] & IEEE80211_QOS_TID));

        if (vap->iv_ccx_evtable && vap->iv_ccx_evtable->wlan_ccx_process_qos) {
            vap->iv_ccx_evtable->wlan_ccx_process_qos(vap->iv_ccx_arg, IEEE80211_RX_QOS_FRAME, (qos[0] & IEEE80211_QOS_TID));
        }

        /* remove QoS filed from header */
        hdrlen -= qoslen;
        memmove((u_int8_t *)wh + qoslen, wh, hdrlen);
        wh = (struct ieee80211_frame *)wbuf_pull(wbuf, qoslen);

        /* clear QoS bit */
        wh->i_fc[0] &= ~IEEE80211_FC0_SUBTYPE_QOS;

    } else {
        u_int16_t padlen = 0;

        /* Non-Qos Frames, remove padding if any */
        if (vap->iv_ic->ic_flags & IEEE80211_F_DATAPAD) {
            hdrsize = ieee80211_hdrsize(wh);
            padlen = roundup((hdrsize), sizeof(u_int32_t)) - hdrsize;
        }

        if (padlen) {
            /* remove padding from header */
            hdrlen -= padlen;
            memmove((u_int8_t *)wh + padlen, wh, hdrlen);
            wh = (struct ieee80211_frame *)wbuf_pull(wbuf, padlen);
        }
    }

    return 0;
}

static INLINE int
ieee80211_is_mcastecho(struct ieee80211vap *vap, struct ieee80211_frame *wh)
{
    u_int8_t *sender;
    u_int8_t dir, subtype;
    int mcastecho = 0;

    KASSERT(vap->iv_opmode == IEEE80211_M_STA, ("!sta mode"));

    dir = wh->i_fc[1] & IEEE80211_FC1_DIR_MASK;
    subtype = wh->i_fc[0] & IEEE80211_FC0_SUBTYPE_MASK;
    
    if (dir == IEEE80211_FC1_DIR_DSTODS) {
        if (subtype == IEEE80211_FC0_SUBTYPE_QOS) {
            struct ieee80211_qosframe_addr4 *wh4q = (struct ieee80211_qosframe_addr4 *)wh;

#ifdef IEEE80211_DEBUG_CHATTY 
            IEEE80211_DPRINTF(vap, IEEE80211_MSG_WDS, "*** MCAST QoS 4 addr *** "
                              "ADDR1 %s, ADDR2 %s, ADDR3 %s, ADDR4 %s\n",
                              ether_sprintf(wh4q->i_addr1),
                              ether_sprintf(wh4q->i_addr2),
                              ether_sprintf(wh4q->i_addr3),
                              ether_sprintf(wh4q->i_addr4));
#endif
            sender = wh4q->i_addr4;
           } else {
            struct ieee80211_frame_addr4 *wh4 = (struct ieee80211_frame_addr4 *)wh;

#ifdef IEEE80211_DEBUG_CHATTY 
            IEEE80211_DPRINTF(vap, IEEE80211_MSG_WDS, "*** MCAST 4 addr *** "
                              "ADDR1 %s, ADDR2 %s, ADDR3 %s, ADDR4 %s\n",
                              ether_sprintf(wh4->i_addr1),
                              ether_sprintf(wh4->i_addr2),
                              ether_sprintf(wh4->i_addr3),
                              ether_sprintf(wh4->i_addr4));
#endif
            sender = wh4->i_addr4;
           }
    } else {
#ifdef IEEE80211_DEBUG_CHATTY 
        IEEE80211_DPRINTF(vap, IEEE80211_MSG_WDS, "*** MCAST 3 addr  *** "
                          "ADDR1 %s, ADDR2 %s, ADDR3 %s\n",
                          ether_sprintf(wh->i_addr1),
                          ether_sprintf(wh->i_addr2),
                          ether_sprintf(wh->i_addr3));
#endif
        sender = wh->i_addr3;
    }

    
    if (IEEE80211_ADDR_EQ(sender, vap->iv_myaddr)) {
        IEEE80211_DPRINTF(vap, IEEE80211_MSG_WDS, 
                          "*** multicast echo *** sender address equals own address (%s) \n",
                          ether_sprintf(sender));
        mcastecho = 1;
        goto done;
    }
    /*
     * if it is brodcasted by me on behalf of
     * a station behind me, drop it.
     *
     */
    if (IEEE80211_VAP_IS_WDS_ENABLED(vap)) mcastecho = wds_sta_chkmcecho(&(vap->iv_ic->ic_sta), sender);
done:
    return mcastecho;
}

 /*zhaoyang add for tunnel local ctl by sta state*/
 int dhcp_detect_wh1(struct ieee80211vap *vap,struct sk_buff *skb)
 {
	 u_int8_t *wh = skb->data;
	 int hdrspace = ieee80211_hdrspace(vap->iv_ic, wh);
	 int ip_len = sizeof(struct iphdr);
	 
	 if ((skb->len > (hdrspace + LLC_SNAPFRAMELEN + ip_len)) &&
		 ((*(wh + hdrspace + LLC_SNAPFRAMELEN + ip_len + 1) == 67 && *(wh + hdrspace + LLC_SNAPFRAMELEN + ip_len + 3) == 68) ||
		 (*(wh + hdrspace + LLC_SNAPFRAMELEN + ip_len + 1) == 68 && *(wh + hdrspace + LLC_SNAPFRAMELEN + ip_len + 3) == 67)))
		 return 1;		 
	 else
		 return 0;
 }
 
 int
 dns_detect_wh(struct ieee80211vap *vap, struct sk_buff *skb)
 {	 
	 
	 u_int8_t *wh = skb->data;
	 int hdrspace = ieee80211_hdrspace(vap->iv_ic, wh);
	 int ip_len = sizeof(struct iphdr);
	 
	 if ((skb->len > (hdrspace + LLC_SNAPFRAMELEN + ip_len)) &&
		 (*(wh + hdrspace + LLC_SNAPFRAMELEN + ip_len + 1) == 53  ||
		  *(wh + hdrspace + LLC_SNAPFRAMELEN + ip_len + 3) == 53))
		 return 1;		 
	 else
		 return 0;
	 
 }
 
 int 
 arp_detect_wh(struct ieee80211vap *vap,struct sk_buff *skb)
 {
	 u_int8_t *wh = skb->data;
	 int hdrspace = ieee80211_hdrspace(vap->iv_ic, wh);
	 
	 if(skb->len > (hdrspace + LLC_SNAPFRAMELEN))
	 {	 
		 //arp llc type is 0x0806
		 if(*(wh + hdrspace + 6) == 0x08 && *(wh + hdrspace + 7) == 0x06)
			 return 1;			 
		 else 
			 return 0;
	 }
	 else 
		 return 0;
 }
 /*zhaoyang add end*/
static u_int32_t 
ieee80211_state_set(struct ieee80211_node *ni, struct ieee80211vap *vap, wbuf_t wbuf, u_int16_t hdrspace)
{
    osif_dev *osif = NULL;
    unsigned int index;
    unsigned int size = 0;
	unsigned char c_flag = 0;
	unsigned short ip_type = 0;
    unsigned short udp_len = 0;
    
    osif = (osif_dev *)vap->iv_ifp;

	if (dhcp_detect_wh1(vap, wbuf)||arp_detect_wh(vap, wbuf))
	{
		return 0;//goto local
	}
	
	size = hdrspace + LLC_SNAPFRAMELEN;
    memcpy(&ip_type, (wbuf->data + size - 2), 2);
//	printk("packet type is 0x%x\n", ip_type);
//	printk("ni->ni_localflags 0x%x\n", ni->ni_localflags);
	
	if (ni->ni_localflags)
	{          
	    //Sta passed portal authentication,all http packets going to pce will access through local.
	    if(ip_type == htons(0x0800))
	    {
	        struct ip_header * ipwh = (struct ip_header *)(wbuf->data + size);
	        if((ipwh->protocol == 17)||(ipwh->protocol == 6))
	        {
    		    struct udphdr * udp = (struct udphdr *)(wbuf->data + size + ((ipwh->version_ihl)&0xf)*4);
    		    if((udp->dest == 80)&&(ipwh->daddr == autelan_addr[index].cpe_addr))
    		    {
//    		        printk("--------pass portal-http packet-cpe addr-to local------\n");
    		        return 0;//local
    		    }		
    		}
	    }    
		return 1;//tunnel
	}
	else 
	{
//		printk("--------not pass portal------\n");
        if(ip_type == htons(0x0800))
        {
            struct iphdr * ipwh = (struct iphdr *)(wbuf->data + size);
#if 0
            if(ipwh->daddr == htonl(autelan_addr[index].cpe_addr))
            {
//                printk("--------cpe_addr,through local--------\n");
                return 0;
            }
#endif
            if((ipwh->protocol == 17)||(ipwh->protocol == 6))
            {
        		struct udphdr * udp = (struct udphdr *)(wbuf->data + size + ((ipwh->ihl)&0xf)*4);		
				if(udp->dest == 80)
                {
					if(ipwh->daddr == htonl(autelan_addr[index].safe_addr))
					{
//						printk("--------safe_addr,through local--------\n");
                        return 1;
					}	
					if(ipwh->daddr == htonl(autelan_addr[index].portal_addr))
                    {
//                        		printk("--------portal_addr,through local--------\n");
                        return 1;
                    }
					if(ipwh->daddr == htonl(autelan_addr[index].safe1_addr))
					{
						return 1;
					}
					return 0;
                }
           }
        }
//	    printk("--------other packets,through local--------\n");
		return 0;//else ,to local
	}
}
 /*
  * processes data frames.
  * ieee80211_input_data consumes the wbuf .
  */
static void 
ieee80211_input_data(struct ieee80211_node *ni, wbuf_t wbuf, struct ieee80211_rx_status *rs, int subtype, int dir)
{
    struct ieee80211vap *vap = ni->ni_vap;
    struct ieee80211com *ic = ni->ni_ic;
    struct ieee80211_phy_stats *phy_stats;
    struct ieee80211_mac_stats *mac_stats;
    struct ieee80211_frame *wh;
    struct ieee80211_key *key;
    u_int16_t hdrspace;
    int is_mcast, is_amsdu = 0, is_bcast;
    struct ieee80211_node *ni_wds=NULL;
    struct ieee80211_node *temp_node=NULL;
	/*zhaoyang add for data forward base users*/
	u_int32_t tunnel_local_state = -1; // 1 means tunnel,0 means local
	/*zhaoyang modify end*/
	/*zhaoyang1 transplant from 717*/
    /*Begin:Added by duanmingzhe for thinap*/
    struct net_device *dev = OSIF_TO_NETDEV(vap->iv_ifp);
    wbuf_t wbuf_capture = NULL;
/*<Begin : caizhibang modify for autelan 802.3 tunnel 2012-5-25 */			
   // wlan_ng_prism2_header *ph = NULL;
  /* End : caizhibang modify for autelan 802.3 tunnel 2012-5-25 >*/
    int sendtowtpd;
    int ret = 0;
    struct llc *llc_type = NULL;
   // u_int8_t tid;
    u_short frametype;

/* Begin:gengzj added for wifipos 2013-12-19 */
	struct ath_softc_net80211	*scn = ATH_SOFTC_NET80211(ic);
	struct ath_softc *sc = scn->sc_dev;
/* End:gengzongjie added end */
    int8_t rssithre = 0;     //added by zhanghu for avg rssi

   
   	/*zhaoyang modify for error frame count */
	int is_err =0;
	is_err_frame = 0;	
	/*zhaoyang modify end*/
	
    sendtowtpd = 0;
    frametype = 0;
    /*End:Added by duanmingzhe for thinap*/
    /*added by chenming for rssithre*/
    
    rssithre  = ic->ic_node_getrssi(ni , -1 , WLAN_RSSI_RX);     //added by zhanghu for avg rssi
    /*<Begin:caizhibang modify for PCAPVXNN-118  2013-05-27*/
    if ( (rssithre > 0) && (ni != vap->iv_bss) && (rssithre < vap->iv_rssi_threshold))    //added by zhanghu for avg rssi
    {
        if(ni->ni_rssi_thre_cnt < 3)
        {
             ni->ni_rssi_thre_cnt ++ ;
        }
        else
        {
            ni->ni_rssi_thre_cnt = 0;   
    	/*suzhaoyu add for sta leave report*/
#if AUTELAN_SOLUTION2
    	ni->ni_maintype = 3;
		ni->ni_subtype = 1;
		ieee80211_sta_leave_send_event(ni);
#endif
		/*suzhaoyu add end*/
#if AUTELAN_SOLUTION2
		//FOA just send timeout_netlink to ac.
        wlan_mlme_deauth_request(vap,ni->ni_macaddr,IEEE80211_REASON_UNSPECIFIED);
        ni->ni_vap->iv_stats.is_disassoc_ioctl++;
#endif
		//Autelan-Added-Begin:duanmingzhe for fix bug,ap reboot when open this function
		printk("%s %d a sta(%02x:%02x:%02x:%02x:%02x:%02x) average rssi(%d) is low than limit(%d), kick it out!\n", __func__,__LINE__,ni->ni_macaddr[0],ni->ni_macaddr[1],ni->ni_macaddr[2],ni->ni_macaddr[3],ni->ni_macaddr[4],ni->ni_macaddr[5],rssithre,vap->iv_rssi_threshold);
		if(kes_debug_print_handle){
	    	kes_debug_print_handle(KERN_EMERG "%s %d a sta(%02x:%02x:%02x:%02x:%02x:%02x) average rssi(%d) is low than limit(%d), kick it out!\n", __func__,__LINE__,ni->ni_macaddr[0],ni->ni_macaddr[1],ni->ni_macaddr[2],ni->ni_macaddr[3],ni->ni_macaddr[4],ni->ni_macaddr[5],rssithre,vap->iv_rssi_threshold);
		}
		goto bad; 
		//Autelan-Added-End:duanmingzhe for fix bug,ap reboot when open this function
         }
    }
    else
    {
         ni->ni_rssi_thre_cnt = 0;
    }
    /*End:caizhibang modify for PCAPVXNN-118  2013-05-27>*/
    /*ended by chenming*/
	/*zhaoyang1 transplant end*/

    wh = (struct ieee80211_frame *) wbuf_header(wbuf);
    is_mcast = (dir == IEEE80211_FC1_DIR_DSTODS ||
                dir == IEEE80211_FC1_DIR_TODS ) ?
        IEEE80211_IS_MULTICAST(IEEE80211_WH4(wh)->i_addr3) :
        IEEE80211_IS_MULTICAST(wh->i_addr1);
	/*zhaoyang1 transplant from 717*/
    /*zhaoyang modify for tunnel count CHINAMOBILE-271*/
    // is_bcast = (dir == IEEE80211_FC1_DIR_FROMDS) ? IEEE80211_IS_BROADCAST(wh->i_addr1) : FALSE;
    is_bcast = (dir == IEEE80211_FC1_DIR_FROMDS) ? IEEE80211_IS_BROADCAST(wh->i_addr1) : IEEE80211_IS_BROADCAST(IEEE80211_WH4(wh)->i_addr3);
    /*zhaoyang modify end*/
	/*zhaoyang1 transplant end*/

    phy_stats = &ic->ic_phy_stats[ic->ic_curmode];
    mac_stats = is_mcast ? &vap->iv_multicast_stats : &vap->iv_unicast_stats;
    //printk("-----------input is_mcast = %d is_bcast =%d\n",is_mcast,IEEE80211_IS_BROADCAST(wh->i_addr1));

	/*zhaoyang1 transplant from 717*/
    /*Begin:Added by duanmingzhe for information report*/	
    if(is_bcast)
    {
    	//printk("-----------input broadcase\n");
    	mac_stats = &vap->iv_broadcase_stats;
    }
    /*End:Added by duanmingzhe for information report*/	
	/*zhaoyang1 transplant end*/

    hdrspace = ieee80211_hdrspace(ic, wbuf_header(wbuf));
    if (wbuf_get_pktlen(wbuf) < hdrspace) {
        mac_stats->ims_rx_discard++;
        IEEE80211_DISCARD(vap,
                          IEEE80211_MSG_INPUT, wh,
                          "data",
                          "too small len %d ", wbuf_get_pktlen(wbuf));
		/*zhaoyang1 transplant from 717*/
		/*Begin:added by pengruofeng for counting of error frames 2011-5-11*/
		is_err =1 ;/*zhaoyang modify for error frame count */
		is_err_frame = 1;
		/*End:added by pengruofeng for counting of error frames 2011-5-11*/
		/*zhaoyang1 transplant end*/
        goto bad;
    }

    /*Begin:Added by duanmingzhe for arp filter 2013-01-07*/
    /*If switch on, we will dorp all arp request*/
	if(vap->iv_arp_filter_switch)
	{
	    struct llc *llc = NULL;
	    u_int16_t ether_type = 0;
	    u_int32_t hdrspace = 0;
		if ((wh->i_fc[0] & IEEE80211_FC0_TYPE_MASK) == IEEE80211_FC0_TYPE_DATA)
		{
	    	hdrspace = ieee80211_hdrspace(vap->iv_ic, wbuf_header(wbuf));

	    	if (unlikely(wbuf_get_pktlen(wbuf)  < (hdrspace + LLC_SNAPFRAMELEN))) {
	        	IEEE80211_DISCARD_MAC(vap, IEEE80211_MSG_INPUT,
	                            wh->i_addr2, "data",
	                            "%s: too small packet 0x%x len %u \n",__func__,
	                            ether_type, wbuf_get_pktlen(wbuf));
		       goto bad;
		    }

	    	llc = (struct llc *)(wbuf_header(wbuf) + hdrspace);
			if(llc == NULL)
			{
			    goto bad;
			}
			ether_type = ntohs(llc->llc_snap.ether_type);
			if(ether_type == ETHERTYPE_ARP)
			{
				if(is_mcast)
				{
                    /*Begin: Modified by WangJia for arp filter 2013-04-11*/
                    /*Find out whether this MAC is in excetpion list. */
					if(0 == ieee80211_arpfilter_findmac(vap, wh->i_addr2)) 
                    /*End: Modified by WangJia for arp filter 2013-04-11*/
					{
						goto bad;
					}
				}
			}
		}		
	}
    /*End:Added by duanmingzhe for arp filter*/

#if ATH_WDS_WAR_UNENCRYPTED
        /* Owl 2.2 WDS War for Non-Encrypted 4 Addr QoS Frames - Extra QoS Ctl field */
        if ((dir == IEEE80211_FC1_DIR_DSTODS) && IEEE80211_NODE_USEWDSWAR(ni) &&
            ((wh->i_fc[1] & IEEE80211_FC1_WEP) == 0) &&
            (subtype == IEEE80211_FC0_SUBTYPE_QOS)) {
            u_int8_t *header = (u_int8_t *)wh;
            int realhdrsize = ieee80211_hdrsize(wh); /* no _F_DATAPAD */
            int padsize = hdrspace - realhdrsize;

            if (padsize == 0) {
                /* IEEE80211_F_DATAPAD had no local effect,
                 * but we need to account for remote */
                padsize += roundup(sizeof(struct ieee80211_qoscntl), sizeof(u_int32_t));
            }

            if (padsize > 0) {
                memmove(header+padsize, header, realhdrsize);
                wbuf_pull(wbuf, padsize);
                /* data ptr moved */
                wh = (struct ieee80211_frame *)wbuf_header(wbuf);
            }
        }
#endif

    vap->iv_lastdata = OS_GET_TIMESTAMP();
    phy_stats->ips_rx_fragments++;
    vap->iv_txrxbytes += wbuf_get_pktlen(wbuf);
/*zhaoyang1 transplant from 717*/
/*added by chenming for rx statistics*/
    phy_stats->ips_rx_packets++;
    if (is_mcast)
        phy_stats->ips_rx_multicast++;
    mac_stats->ims_rx_packets++;
  /*cmdebug*/
	mac_stats->ims_rx_bytes += wbuf_get_pktlen(wbuf);
/*ended by chenming*/
/*zhaoyang1 transplant end*/

    /* 
     * Store timestamp for actual (non-NULL) data frames.
     * This provides other modules such as SCAN and LED with correct 
     * information about the actual data traffic in the system.
     * We don't take broadcast traffic into consideration.
     */
    if (!is_bcast && IEEE80211_CONTAIN_DATA(subtype)) {
        vap->iv_last_traffic_indication = OS_GET_TIMESTAMP();
    }

    switch (vap->iv_opmode) {
    case IEEE80211_M_STA:
        /* 
         * allow all frames with FromDS bit set .
         * allow Frames with DStoDS if the vap is WDS capable.
         */
        if (!(dir == IEEE80211_FC1_DIR_FROMDS || IEEE80211_IS_TDLS_NODE(ni) ||
              (dir == IEEE80211_FC1_DIR_DSTODS && (IEEE80211_VAP_IS_WDS_ENABLED(vap))))) {
            if (dir == IEEE80211_FC1_DIR_DSTODS) {
                IEEE80211_DISCARD(vap,
                                  IEEE80211_MSG_INPUT |
                                  IEEE80211_MSG_WDS, wh,
                                  "4-address data",
                                  "%s", "WDS not enabled");
                vap->iv_stats.is_rx_nowds++;
            } else {
                IEEE80211_DISCARD(vap,
                                  IEEE80211_MSG_INPUT, wh,
                                  "data",
                                  "invalid dir 0x%x", dir);
                vap->iv_stats.is_rx_wrongdir++;
            }
            mac_stats->ims_rx_discard++;
            goto bad;
        }
            
        /*
         * In IEEE802.11 network, multicast packet
         * sent from me is broadcasted from AP.
         * It should be silently discarded.
         */
        if (is_mcast && ieee80211_is_mcastecho(vap, wh)) {
            IEEE80211_DISCARD(vap, IEEE80211_MSG_INPUT |
                              IEEE80211_MSG_WDS,
                              wh, "data", "multicast echo dir 0x%x", dir);
            vap->iv_stats.is_rx_mcastecho++;
            goto bad;
        }

        if (is_mcast) {
            /* Report last multicast/broadcast frame to Power Save Module */
            if (!(wh->i_fc[1] & IEEE80211_FC1_MORE_DATA)) {
                if (vap->iv_txrx_event_info.iv_txrx_event_filter & IEEE80211_VAP_INPUT_EVENT_LAST_MCAST) {
                    ieee80211_vap_txrx_event evt;
                    evt.type = IEEE80211_VAP_INPUT_EVENT_LAST_MCAST;
                    ieee80211_vap_txrx_deliver_event(vap,&evt);
                }
            }
        } else {
            /* Report unicast frames to Power Save Module */
            if (vap->iv_txrx_event_info.iv_txrx_event_filter & (IEEE80211_VAP_INPUT_EVENT_UCAST | IEEE80211_VAP_INPUT_EVENT_EOSP)) {
                if (vap->iv_txrx_event_info.iv_txrx_event_filter & IEEE80211_VAP_INPUT_EVENT_UCAST) {
                    ieee80211_vap_txrx_event evt;
                    evt.type = IEEE80211_VAP_INPUT_EVENT_UCAST;
                    evt.u.more_data = wh->i_fc[1] & IEEE80211_FC1_MORE_DATA;
                    ieee80211_vap_txrx_deliver_event(vap,&evt);
                } 
                if (vap->iv_txrx_event_info.iv_txrx_event_filter & IEEE80211_VAP_INPUT_EVENT_EOSP && 
                    ( subtype == IEEE80211_FC0_SUBTYPE_QOS || subtype == IEEE80211_FC0_SUBTYPE_QOS_NULL)) {
                    if(((struct ieee80211_qosframe *)wh)->i_qos[0] & IEEE80211_QOS_EOSP) {
                        ieee80211_vap_txrx_event evt;
                        evt.type = IEEE80211_VAP_INPUT_EVENT_EOSP;
                        ieee80211_vap_txrx_deliver_event(vap,&evt);
                    }
                } 
            }
        }

	if (dir == IEEE80211_FC1_DIR_DSTODS){
	    struct ieee80211_node_table *nt;
	    struct ieee80211_frame_addr4 *wh4;
	    wh4 = (struct ieee80211_frame_addr4 *) wbuf_header(wbuf);
	    nt = &ic->ic_sta;
	    ni_wds = ieee80211_find_wds_node(nt, wh4->i_addr4);

	    if (ni_wds == NULL)
                {
		    /*
		     * In STA mode, add wds entries for hosts behind us, but
		     * not for hosts behind the rootap.
		     */
		    if (!IEEE80211_ADDR_EQ(wh4->i_addr2, vap->iv_bss->ni_bssid))
			{
			    temp_node = ieee80211_find_node(nt,wh4->i_addr4);
			    if (temp_node == NULL)
				{
					ieee80211_add_wds_addr(nt, ni, wh4->i_addr4, 
                                        IEEE80211_NODE_F_WDS_REMOTE);
				}
			    else if (!IEEE80211_ADDR_EQ(temp_node->ni_macaddr, vap->iv_myaddr))
				{
				    ieee80211_node_leave(temp_node);
				    ieee80211_free_node(temp_node);
				    ieee80211_add_wds_addr(nt, ni, wh4->i_addr4,
                                    IEEE80211_NODE_F_WDS_REMOTE);
				}
			}
                }
	    else
                {
		    ieee80211_free_node(ni_wds);
                }
	}

        break;

    case IEEE80211_M_IBSS:
        if (dir != IEEE80211_FC1_DIR_NODS) {
            mac_stats->ims_rx_discard++;
            goto bad;
        }

        /*
         * If it is a data frame from a peer, also update the receive 
         * time stamp. This can reduce false beacon miss detection.
         */
        ni->ni_beacon_rstamp = OS_GET_TIMESTAMP();
        ni->ni_probe_ticks   = 0;
        break;

    case IEEE80211_M_HOSTAP:
        /* 
         * allow all frames with ToDS bit set .
         * allow Frames with DStoDS if the vap is WDS capable.
         */
        if (!((dir == IEEE80211_FC1_DIR_TODS) || 
              (dir == IEEE80211_FC1_DIR_DSTODS && (IEEE80211_VAP_IS_WDS_ENABLED(vap))))) {
            if (dir == IEEE80211_FC1_DIR_DSTODS) {
                IEEE80211_DISCARD(vap,
                                  IEEE80211_MSG_INPUT |
                                  IEEE80211_MSG_WDS, wh,
                                  "4-address data",
                                  "%s", "WDS not enabled");
                vap->iv_stats.is_rx_nowds++;
            } else {
                IEEE80211_DISCARD(vap,
                                  IEEE80211_MSG_INPUT, wh,
                                  "data",
                                  "invalid dir 0x%x", dir);
                vap->iv_stats.is_rx_wrongdir++;
            }
			
			is_err =1;/*zhaoyang modify for error frame count */
            goto bad;
        }
        
        /* if NAWDS learning feature is enabled, add the mac to NAWDS table */
        if ((ni == vap->iv_bss) &&
            (dir == IEEE80211_FC1_DIR_DSTODS) &&
            (ieee80211_nawds_enable_learning(vap))) {
            ieee80211_nawds_learn(vap, wh->i_addr2);
            /* current node is bss node so drop it to avoid sending dis-assoc. packet */
            goto bad;
        }

        /* check if source STA is associated */
        if (ni == vap->iv_bss) {
            IEEE80211_DISCARD(vap, IEEE80211_MSG_INPUT,
                              wh, "data", "%s", "unknown src\n");
            
			/* Begin:gengzj added for rfid scan 2013-11-26 */
			if (sc->sc_locate == 0)//pengdecai  edit " rfid_scan == 0" to "vap->iv_loacte == 0"
			{
            /* NB: caller deals with reference */
            if (ieee80211_vap_ready_is_set(vap)) {
                ni = ieee80211_tmp_node(vap, wh->i_addr2);
                if (ni != NULL) {
                    ieee80211_send_deauth(ni, IEEE80211_REASON_NOT_AUTHED);

                    /* claim node immediately */
                    ieee80211_free_node(ni);
					/*zhaoyang1 transplant from 717*/
					/*pengruofeng add start for management frame stats 2011-5-9*/
					vap->iv_stats.is_deauth_unnormal++;
					/*pengruofeng add end 2011-5-9*/
					/*zhaoyang1 transplant end*/
                }
            }
			}
			/* End:gengzongjie added end */
/* End: gengzj added end */
            vap->iv_stats.is_rx_notassoc++;
			is_err =1;/*zhaoyang modify for error frame count */
            goto bad;
        }

        if (ni->ni_associd == 0) {
            IEEE80211_DISCARD(vap, IEEE80211_MSG_INPUT,
                              wh, "data", "%s", "unassoc src\n");
			ieee80211_send_disassoc(ni, IEEE80211_REASON_NOT_ASSOCED);
            vap->iv_stats.is_rx_notassoc++;
			/*zhaoyang1 transplant from 717*/
			/*pengruofeng add start for management frame stats 2011-5-9*/
				vap->iv_stats.is_disassoc_unnormal++;
			/*pengruofeng add end 2011-5-9*/
			is_err =1;/*zhaoyang modify for error frame count */
			/*zhaoyang1 transplant end*/
            goto bad;
        }
	

	/* If we're a 4 address packet, make sure we have an entry in
            the node table for the packet source address (addr4).  If not,
            add one */
	if (dir == IEEE80211_FC1_DIR_DSTODS){
	    wds_update_rootwds_table(ni,&ic->ic_sta, wbuf);
	}


#ifdef IEEE80211_DWDS
        /*
         * For 4-address packets handle WDS discovery
         * notifications.  Once a WDS link is setup frames
         * are just delivered to the WDS vap (see below).
         */
        if (dir == IEEE80211_FC1_DIR_DSTODS &&
            ni->ni_wdsvap == NULL) {
            if (!ieee80211_node_is_authorized(ni)) {
                IEEE80211_DISCARD(vap,
                                  IEEE80211_MSG_INPUT |
                                  IEEE80211_MSG_WDS, wh,
                                  "4-address data",
                                  "%s", "unauthorized port");
                vap->iv_stats.is_rx_unauth++;
                IEEE80211_NODE_STAT(ni, rx_unauth);
                goto err;
            }
            ieee80211_wds_discover(ni, m);
            goto bad;
        }
#endif
        break;

    case IEEE80211_M_WDS:
        if (dir != IEEE80211_FC1_DIR_DSTODS) {
            IEEE80211_DISCARD(vap, IEEE80211_MSG_ANY,
                              wh, "data", "invalid dir 0x%x", dir);
            vap->iv_stats.is_rx_wrongdir++;
            goto bad;
        }
#ifdef IEEE80211_WDSLEGACY
        if ((vap->iv_flags_ext & IEEE80211_FEXT_WDSLEGACY) == 0) {
            IEEE80211_DISCARD(vap, IEEE80211_MSG_ANY,
                              wh, "data", "not legacy wds, flags 0x%x",
                              vap->iv_flags_ext);
            vap->iv_stats.is_rx_nowds++;    /* XXX */
            goto bad;
        }
#endif
        break;

    default:
        break;
    }

    /*
     *  Safemode prevents us from calling decap.
     */
    if (!IEEE80211_VAP_IS_SAFEMODE_ENABLED(vap) &&
        (wh->i_fc[1] & IEEE80211_FC1_WEP)) {
        key = ieee80211_crypto_decap(ni, wbuf, hdrspace, rs);
        if (key == NULL) {
            mac_stats->ims_rx_decryptcrc++;
            IEEE80211_DISCARD (vap, IEEE80211_MSG_INPUT, wh,
                          "key is" , "%s"," null");
			/*zhaoyang1 transplant from 717*/
			/*Begin:added by pengruofeng for counting of error frames 2011-5-11*/
 			is_err =1;/*zhaoyang modify for error frame count */
			is_err_frame = 1;
			/*End:added by pengruofeng for counting of error frames 2011-5-11*/
			/*zhaoyang1 transplant end*/
            goto bad;
        } else {
            mac_stats->ims_rx_decryptok++;
        }

        wh = (struct ieee80211_frame *) wbuf_header(wbuf);
        /* NB: We clear the Protected bit later */
    } else {
        key = NULL;
    }

    /*
     * Next up, any defragmentation. A list of wbuf will be returned.
     * However, do not defrag when in safe mode.
     */
    if (!IEEE80211_VAP_IS_SAFEMODE_ENABLED(vap) && !is_mcast) {
        wbuf = ieee80211_defrag(ni, wbuf, hdrspace);
        if (wbuf == NULL) {
            /* Fragment dropped or frame not complete yet */
            IEEE80211_DISCARD(vap,
                          IEEE80211_MSG_INPUT, wh,
                          "defarg","%s",
                          "failed");
			/*zhaoyang1 transplant from 717*/
			/*Begin:added by pengruofeng for counting of error frames 2011-5-11*/
			is_err_frame = 1;
			/*End:added by pengruofeng for counting of error frames 2011-5-11*/
			/*zhaoyang1 transplant end*/
            goto out;
        }
    }

    if (subtype == IEEE80211_FC0_SUBTYPE_QOS) {
        is_amsdu = (dir != IEEE80211_FC1_DIR_DSTODS) ?
            (((struct ieee80211_qosframe *)wh)->i_qos[0] & IEEE80211_QOS_AMSDU) :
            (((struct ieee80211_qosframe_addr4 *)wh)->i_qos[0] & IEEE80211_QOS_AMSDU);
    }

    /*
     * Next strip any MSDU crypto bits.
     */
    ASSERT(!IEEE80211_VAP_IS_SAFEMODE_ENABLED(vap) || (key == NULL));
    if ((key != NULL) && !ieee80211_crypto_demic(vap, key, wbuf, hdrspace, 0, rs)) {
        IEEE80211_DISCARD_MAC(vap, IEEE80211_MSG_INPUT,
                              ni->ni_macaddr, "data", "%s", "demic error");
        /* IEEE80211_NODE_STAT(ni, rx_demicfail); */
         IEEE80211_DISCARD(vap,
                          IEEE80211_MSG_INPUT, wh,
                          "demic","%s",
                          "failed");
		is_err =1;
        goto bad;
    }

    /*
     * decapsulate the QoS header if the OS asks us to deliver standard 802.11
     * headers. if OS does not want us to deliver 802.11 header then it wants us
     * to deliver ethernet header in which case the qos header will be decapped
     * along with 802.11 header ieee80211_decap function.
     */
    if (IEEE80211_VAP_IS_DELIVER_80211_ENABLED(vap) &&
        ieee80211_qos_decap(vap, wbuf, hdrspace)) {
        IEEE80211_DISCARD_MAC(vap, IEEE80211_MSG_INPUT,
                              ni->ni_macaddr, "data", "%s", "decap error");
        vap->iv_stats.is_rx_decap++;
		is_err =0;/*zhaoyang modify for error frame count */
        goto bad;
    }

    if ((subtype == IEEE80211_FC0_SUBTYPE_QOS_NULL) || (subtype == IEEE80211_FC0_SUBTYPE_NODATA)) {
        /* no need to process the null data frames any further */
		is_err =0;/*zhaoyang modify for error frame count */
        goto bad;
    }

	/*zhaoyang1 transplant from 717*/
	/*Begin:Added by duanmingzhe for thinap*/
    /*get the frametype*/
    llc_type = (struct llc *)skb_pull(wbuf, hdrspace);
    
    if (llc_type != NULL)
    {
        if (wbuf->len >= LLC_SNAPFRAMELEN &&
        llc_type->llc_dsap == LLC_SNAP_LSAP && llc_type->llc_ssap == LLC_SNAP_LSAP &&
        llc_type->llc_control == LLC_UI && llc_type->llc_snap.org_code[0] == 0 &&
        llc_type->llc_snap.org_code[1] == 0 && llc_type->llc_snap.org_code[2] == 0) {
            frametype = llc_type->llc_un.type_snap.ether_type;
        }
        skb_push(wbuf, hdrspace);
    }
	/*End:Added by duanmingzhe for thinap*/


	/*pengruofeng--add begin for autelan private wmm 2011-6-1*/
	if (IEEE80211_QOS_HAS_SEQ(wh) &&
			(vap->priv_wmm).tos_flag && (frametype == __constant_htons(ETHERTYPE_IP)
			|| frametype == __constant_htons(0x86dd) ))   // 0x86dd is ETHERTYPE_IPv6
	{
		u_int8_t tid = ((struct ieee80211_qosframe *)wh)->i_qos[0] & IEEE80211_QOS_TID;
		struct iphdr *iph = (struct iphdr *)(wbuf->data + hdrspace + LLC_SNAPFRAMELEN);
	
		if(4 == iph->version)
		{
			if(vap->iv_debug_code & 0x00000020){
				if(printk_ratelimit())
					printk("####[%s]  %s Wmm translate Tos tid=%d  TID_TO_AC=%d\n",ether_sprintf(wh->i_addr2),__func__,tid,TID_TO_WME_AC(tid));
			}
			iph->tos = wmm_to_tos_map(TID_TO_WME_AC(tid), vap);
			
			if(vap->iv_debug_code & 0x00000020){
				if(printk_ratelimit())
					printk("####[%s]  %s Wmm translate Tos iph->tos=%d\n",ether_sprintf(wh->i_addr2),__func__,iph->tos);
			}
			
			iph->check = 0;
			iph->check = ip_fast_csum((unsigned char *)iph, iph->ihl);
		}
		else if(6 == iph->version)
		{
			unsigned char tos = 0;
			struct ipv6hdr * ipv6h = (struct ipv6hdr*)(wbuf->data + hdrspace + LLC_SNAPFRAMELEN);
			tos =  wmm_to_tos_map(TID_TO_WME_AC(tid), vap);
			ipv6h->priority = tos >> 4;
			ipv6h->flow_lbl[0] |= ((tos <<4) & 0xf0 );
		}
			
	}
	/*pengruofeng--add end 2011-6-1*/


    /*Begin:Added by pengruofeng for develop the policy of mac binding 2011-5-24*/
    if (ni != vap->iv_bss)
    {
        u_int8_t null_ip[4] = {0x0, 0x0, 0x0, 0x0};
        
        if (memcmp(ni->ni_ipaddr, null_ip, sizeof(ni->ni_ipaddr)) == 0)
            ni->ni_ip_auto_set = 0;
        else
            ni->ni_ip_auto_set = 1;
        
        if ((memcmp(ni->ni_ipaddr, null_ip, sizeof(ni->ni_ipaddr)) != 0) || ((vap->vap_ip_auto_learning == 1) && (ni->ni_ip_auto_set == 0)))
        {
            if (frametype == __constant_htons(ETHERTYPE_IP))
            {
                struct iphdr *ip_h = (struct iphdr *)(wbuf->data + hdrspace + LLC_SNAPFRAMELEN);
                
                if (memcmp(&(ip_h->saddr), ni->ni_ipaddr, sizeof(ni->ni_ipaddr)) != 0)
                {
                    if ((vap->vap_ip_auto_learning == 1) && (ni->ni_ip_auto_set == 0))
                    {
                        memcpy(ni->ni_ipaddr, &(ip_h->saddr), sizeof(ni->ni_ipaddr));
                        ni->ni_ip_auto_set = 1;
                    }
                    else
                    {
                        goto out;
                    }
                }
            }
        }
    }
    /*End:Added by pengruofeng for develop the policy of mac binding 2011-5-24*/

    /*Begin:Added by duanmingzhe for thinap*/
    if (thinap) {
        /*Begin:Modify by zhaoyang1 for data forward base users 2012-11-24 */
        #if 0
        /*<Begin : caizhibang modify for autelan 802.3 tunnel 2012-5-25 */
        if (vap->vap_splitmac == 1)
        /*End : caizhibang modify for autelan 802.3 tunnel 2012-5-25 >*/
        #endif
        /*End:Modify by zhaoyang1 for data forward base users 2012-11-24 */
		/*zhaoyang add for data forward base users*/
#if 1	//add for lte-fi 20140303
		if (vap->vap_splitmac == 3) //lte mode
		{
			if (!ieee80211_node_is_authorized(ni))
			{
				tunnel_local_state = 1;//tunnel
				printk("-------- !ieee80211_node_is_authorized(ni)--authorized for data -------\n");
			}
			else
			{
				if (ni->ni_localflags)
				{
			    	tunnel_local_state = ieee80211_state_set(ni, vap, wbuf, hdrspace);
				}
				else {
					//DHCP,ARP,DNS local ,others tunnel
//					if (dhcp_detect_wh1(vap,wbuf) || dns_detect_wh(vap,wbuf) || arp_detect_wh(vap,wbuf))
					if (dns_detect_wh(vap,wbuf))
					{	
						tunnel_local_state = 0;//local
					}
					else 
					{	
						//tunnel_local_state = 1;//tunnel
						tunnel_local_state = ieee80211_state_set(ni, vap, wbuf, hdrspace);
					}
				}
			}		
		}
#endif
		else if (vap->vap_splitmac == 4)//ubc mode
		{
			if (!ieee80211_node_is_authorized(ni))
				tunnel_local_state = 1;//tunnel
			else
			{
				if (ni->ni_localflags)
				{
					if (ni->ni_portal_ipaddr[0] == 0)// outer portal server mode
						tunnel_local_state = 0; //local
					else//inner portal server mode 
					{
						struct ip_header * ipwh= (struct ip_header *) (wbuf->data + hdrspace + LLC_SNAPFRAMELEN);
						
						if (memcmp(&(ipwh->daddr),ni->ni_portal_ipaddr,sizeof(ni->ni_portal_ipaddr)) == 0)// this frame is sent to portal server,so go to tunnel
						{
							tunnel_local_state = 1;//tunnel
							
						}
						else
						{
							tunnel_local_state = 0;//local
						}
						
					}
					
				}
				else {
					//DHCP,ARP,DNS local ,others tunnel
					if (dhcp_detect_wh1(vap,wbuf) || dns_detect_wh(vap,wbuf) || arp_detect_wh(vap,wbuf))
						tunnel_local_state = 0;//local
					else 
						tunnel_local_state = 1;//tunnel
				}
			}		
		}
		else {
			tunnel_local_state = vap->vap_splitmac;
		}

		/*zhaoyang  modify end*/
        if (tunnel_local_state == 1) {//tunnel mode
            /* pei modified at 090112 */
            wbuf_capture = wbuf;
            wbuf = NULL;
            if (ieee80211_node_is_authorized(ni)) {
                sendtowtpd = 1;
/*<Begin : caizhibang modify for autelan 802.3 tunnel 2012-5-25 */			
				//wbuf_push(wbuf_capture,sizeof(wlan_ng_prism2_header));
                //skb_push(wbuf_capture,sizeof(wlan_ng_prism2_header));
                //ph = (wlan_ng_prism2_header *) wbuf_capture->data;
                //strncpy(ph->devname, dev->name, sizeof(ph->devname));
/*End : caizhibang modify for autelan 802.3 tunnel 2012-5-25 >*/    
            }
            else {              
                if (frametype == __constant_htons(ETHERTYPE_PAE) 
                || frametype == __constant_htons(ETHERTYPE_PRE_PAE) 
                || frametype == __constant_htons(ETHERTYPE_WAI)
                || dhcp_detect_wh(vap, wbuf_capture)
                || pppoe_detect_wh(vap,wbuf_capture) )//zhaoyang add for through the pppoe frame(add pppoe_detect_wh(vap,wbuf_capture))
                {	                    
                    sendtowtpd = 1;
/*<Begin : caizhibang modify for autelan 802.3 tunnel 2012-5-25 */			
		   			//wbuf_push(wbuf_capture,sizeof(wlan_ng_prism2_header));
                    //skb_push(wbuf_capture,sizeof(wlan_ng_prism2_header));
                   // ph = (wlan_ng_prism2_header *) wbuf_capture->data;
                   // strncpy(ph->devname, dev->name, sizeof(ph->devname));
/*End : caizhibang modify for autelan 802.3 tunnel 2012-5-25 >*/    
                    if (vap->iv_eap == 1) { /*xmeng add for ac roaming eap at splitmac mode*/
                        wbuf_capture->protocol = __constant_htons(0x0020);/*ETH_P_80211_RAW*/
                    }
                }
                else {
                    wbuf_free(wbuf_capture);
                }
            }
			is_err = 0;/*zhaoyang modify for error frame count */
            goto thinapout;	
        }
        /*ljy--modified begin to optimize the process of 802.3 tunnel on 2013-1-16*/
        /*<Begin : caizhibang modify for autelan 802.3 tunnel 2012-5-25 */			
        if (tunnel_local_state == 2)
        {
            wbuf_capture = wbuf;
            wbuf = NULL;
            if (frametype == __constant_htons(ETHERTYPE_PAE) 
                || frametype == __constant_htons(ETHERTYPE_PRE_PAE) 
                || frametype == __constant_htons(ETHERTYPE_WAI))
            {
                sendtowtpd = 1;
                wbuf_capture->protocol = __constant_htons(0x0019);
            }
            else
            {
                if (ieee80211_node_is_authorized(ni) || dhcp_detect_wh(vap, wbuf_capture) ||
                    pppoe_detect_wh(vap,wbuf_capture))
                {
                    unsigned char bssid[ETHER_ADDR_LEN];
                    wh = (struct ieee80211_frame *) wbuf_header(wbuf_capture);
                    IEEE80211_ADDR_COPY(bssid,wh->i_addr1);
                    wbuf_capture = ieee80211_decap(vap, wbuf_capture, hdrspace,rs);
                    wbuf_push(wbuf_capture,6);
                    IEEE80211_ADDR_COPY(wbuf_capture->data,bssid);
                    wbuf_capture->protocol = __constant_htons(0x0030);
                    sendtowtpd = 1;
                }
                else
                {
                    wbuf_free(wbuf_capture);
                }
            }
            goto thinapout;
        }
        /*End : caizhibang modify for autelan 802.3 tunnel 2012-5-25 >*/
        /*ljy--modified end*/
        if (frametype == __constant_htons(ETHERTYPE_PAE) || frametype == __constant_htons(ETHERTYPE_PRE_PAE) || frametype == __constant_htons(ETHERTYPE_WAI)) /*ljy--modified for wapi*/
        {	
            wbuf_capture = wbuf_copy(wbuf);
            if (wbuf_capture== NULL) {
                printk("\n capture copy error \n");
                goto thinapout; 
            }
/*<Begin : caizhibang modify for autelan 802.3 tunnel 2012-5-25 */    
            //skb_push(wbuf_capture, sizeof(wlan_ng_prism2_header));/*add the prism2 header*/
  /*End : caizhibang modify for autelan 802.3 tunnel 2012-5-25 >*/    
            sendtowtpd = 2;
            if (sendtowtpd == 2 && wbuf_capture != NULL) {
		//int ret = 0;
/*<Begin : caizhibang modify for autelan 802.3 tunnel 2012-5-25 */    
                //ph = (wlan_ng_prism2_header *) wbuf_capture->data;
                //strncpy(ph->devname, dev->name, sizeof(ph->devname));
/*End : caizhibang modify for autelan 802.3 tunnel 2012-5-25 >*/    
                wbuf_capture->dev = dev ;
                #if LINUX_VERSION_CODE <= KERNEL_VERSION(2,6,20)
                wbuf_capture->mac.raw = wbuf_capture->data;
                #else
                wbuf_capture->mac_header = wbuf_capture->data;
                #endif
                wbuf_capture->ip_summed = CHECKSUM_NONE;
                wbuf_capture->pkt_type = PACKET_OTHERHOST;
                wbuf_capture->protocol = __constant_htons(0x0019);/*ETH_P_80211_RAW*/
#if 0
                /*zhaoyang--modify for receive data count*/
                if(rx_packet_flag==0)
                {
                    vap->iv_devstats.rx_packets++;
                    vap->iv_devstats.rx_bytes += skb->len;
                    rx_packet_flag++;
                }
                /*sheyanbin--add begin for multicast count*/
                if (is_multicast) {
                    vap->iv_devstats.rx_multicast_packets++;
                    vap->iv_devstats.rx_multicast_bytes += skb->len;
                }
                /*sheyanbin--add end*/
                /*zhaoyang --add begin for broadcast count*/
                if(is_broadcast){
                    //printk("receive broadcast flame\n");
                    vap->iv_devstats.rx_broadcast_packets++;
                    vap->iv_devstats.rx_broadcast_bytes += skb->len;
                }
                /*zhaoyang --add end*/					
				//zhaoyang modify for ni rx_data and tx_data count delete the following
                IEEE80211_NODE_STAT(ni, rx_data);
                IEEE80211_NODE_STAT_ADD(ni, rx_bytes, wbuf->len);
#endif
				/*added by duanmingzhe for rx statistics*/
                IEEE80211_NODE_STAT(ni, rx_data);
                IEEE80211_NODE_STAT_ADD(ni, rx_bytes, wbuf_capture->len);
				/*ended by duanmingzhe*/
				
				//ic->ic_lastdata = jiffies;
                /*sheyanbin modify for rxcount of packet dropped*/
		//printk("%s send to netif_rx protocol = 0x%x\n",__func__, wbuf_capture->protocol);
                ret = netif_rx(wbuf_capture);
                if(NET_RX_DROP == ret)
                {
                    //vap->iv_devstats.rx_dropped++;
                    goto bad;
                }
                /*sheyanbin -- add end*/
            }
        }
    }
    /*End:Added by duanmingzhe for thinap*/	
	/*zhaoyang1 transplant end*/
#if ATH_RXBUF_RECYCLE
	if (is_mcast || is_bcast) {
		wbuf_set_cloned(wbuf);
	} else {
 		wbuf_clear_cloned(wbuf);
	}
#endif
    if (!is_amsdu) {
        if (ieee80211_check_privacy_filters(ni, wbuf, is_mcast) == FILTER_STATUS_REJECT) {
             IEEE80211_DISCARD_MAC(vap,
                                   IEEE80211_MSG_INPUT, wh->i_addr2, "data",
                              "privacy filter check","%s \n",
                              "failed");
            goto bad;
        }
    } else {
        ieee80211_amsdu_input(ni, wbuf, rs, is_mcast, subtype);
        goto out;
    }



	/*zhaoyang1 transplant from 717*/
	/*zhaoyang modify for ni rx_data and tx_data count*/
	IEEE80211_NODE_STAT(ni, rx_data);
    IEEE80211_NODE_STAT_ADD(ni, rx_bytes, wbuf_get_pktlen(wbuf));
	/*zhaoyang modify end*/
	/*zhaoyang1 transplant end*/
	
    /* consumes the wbuf */
    ieee80211_deliver_data(vap, wbuf, ni, rs, hdrspace, is_mcast, subtype);

out:
	/*zhaoyang1 transplant from 717*/
	/*pengruofeng add start for management frame stats 2011-5-11*/
	if(is_err_frame)
	{
		vap->iv_stats.is_rx_error_frames++;
	}
	/*pengruofeng add end 2011-5-11*/
/*Begin:added by duanmingzhe for thinap*/
thinapout:
    if (thinap) {
        if (sendtowtpd == 1 && wbuf_capture != NULL) {
/*<Begin : caizhibang modify for autelan 802.3 tunnel 2012-5-25 */			  
         //    ph = (wlan_ng_prism2_header *)wbuf_capture->data;
        //     strncpy(ph->devname, dev->name, sizeof(ph->devname));
/*End : caizhibang modify for autelan 802.3 tunnel 2012-5-25 >*/		 
            wbuf_capture->dev = dev;
            //wbuf_capture->mac_header = wbuf_capture->data;
            #if (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,22))
            wbuf_capture->mac.raw = wbuf_capture->data;
            #else
            skb_reset_mac_header(wbuf_capture);
            #endif
            wbuf_capture->ip_summed = CHECKSUM_NONE;
            wbuf_capture->pkt_type = PACKET_OTHERHOST;    			
            //skb_capture->protocol = __constant_htons(0x0019); /* ETH_P_80211_RAW */
            /*ljy--modified begin to optimize the process of 802.3 tunnel on 2013-1-16*/
            /*<Begin : caizhibang modify for autelan 802.3 tunnel 2012-5-25 */
            if (tunnel_local_state == 1) {
                if (wbuf_capture->protocol != __constant_htons(0x0020)) {
                    wbuf_capture->protocol = __constant_htons(0x0019); /* ETH_P_80211_RAW */
                } else {
                //printk(" --a splitmac eap :0020\n");
                }
            }
            /*End : caizhibang modify for autelan 802.3 tunnel 2012-5-25 >*/	
            /*ljy--modified end*/
            //ic->ic_lastdata = jiffies;
             #if 0
            /*songchuang add begin for transmit count*/
            struct ieee80211_frame *psz_wh = NULL;
            
            psz_wh = (struct ieee80211_frame *)(wbuf_capture->data + sizeof(wlan_ng_prism2_header));
            if (NULL != psz_wh)
            {
                if ((IEEE80211_FC0_TYPE_DATA == (psz_wh->i_fc[0] & IEEE80211_FC0_TYPE_MASK))
                    && (IEEE80211_FC0_SUBTYPE_NODATA != (psz_wh->i_fc[0] & IEEE80211_FC0_SUBTYPE_MASK)))
                {
                    vap->rx_packets++;
                    vap->rx_bytes += wbuf_capture->len -IEEE80211_MAC_LLC_LEN -sizeof(wlan_ng_prism2_header);
                    /*fix me to calculate the len*/
                }
            }
            /*songchuang add end*/
           
            //sheyanbin add for transmit data count,begin
            /*zhaoyang--modify for receive data count*/
            if(type==IEEE80211_FC0_TYPE_DATA && rx_packet_flag==0 && rx_packet_err==0)
            {			   
                vap->iv_devstats.rx_packets++;
                vap->iv_devstats.rx_bytes += (wbuf_capture->len - IEEE80211_MAC_LLC_LEN - sizeof(wlan_ng_prism2_header));
            }
            /*zhaoyang--modify end*/
            
            /*sheyanbin--add begin for multicast count*/
            if (is_multicast) {
                vap->iv_devstats.rx_multicast_packets++;
                vap->iv_devstats.rx_multicast_bytes += skb_capture->len - 144 - IEEE80211_MAC_LLC_LEN;
            }
            /*sheyanbin--add end*/
            /*zhaoyang --add begin for broadcast count*/
            if (is_broadcast) {
                vap->iv_devstats.rx_broadcast_packets++;
                vap->iv_devstats.rx_broadcast_bytes += skb_capture->len - 144 - IEEE80211_MAC_LLC_LEN;
            }
            /*zhaoyang --add end*/
	        //zhaoyang modify for ni rx_data and tx_data count delete the following
            IEEE80211_NODE_STAT(ni, rx_data);
            IEEE80211_NODE_STAT_ADD(ni, rx_bytes, wbuf_capture->len);
			#endif
			if(wbuf_capture->len > 163)    //lisgonbai add for ONLINEBUG-112
			{
				IEEE80211_NODE_STAT(ni, rx_data);
				IEEE80211_NODE_STAT_ADD(ni, rx_bytes, wbuf_capture->len - 163);	/*suzhaoyu add for traffic correction*/
			}
			//else
			//	IEEE80211_NODE_STAT_ADD(ni, rx_bytes, wbuf_capture->len);
			/*sheyanbin modify for rxcount of packet droppped*/
	    //printk("%s send to netif_receive_skb wbuf_capture->protocol = 0x%x...\n",__func__,wbuf_capture->protocol);
            ret = netif_rx(wbuf_capture);	
            if(NET_RX_DROP == ret)
            {
                //vap->iv_devstats.rx_dropped++;
                goto bad;
            }
			
        }
    }
/*End:added by duanmingzhe for thinap*/
/*zhaoyang1 transplant end*/

    return;

bad:
/*  FIX ME: linux specific netdev struct iv_destats has to be replaced*/
//    vap->iv_devstats.rx_errors++; 
	/*zhaoyang1 transplant from 717*/
	/*pengruofeng add start for management frame stats 2011-5-11*/
	/*zhaoyang modify for error frame count */
	if(is_err){
		if(is_err_frame)
		{
			vap->iv_stats.is_rx_error_frames++;
		}
		else
		{
			vap->iv_stats.is_rx_errors++;
		}
	}
	/*zhaoyang modify end*/
	/*pengruofeng add start for management frame stats 2011-5-11*/
	/*zhaoyang1 transplant end*/
    wbuf_free(wbuf);
}

/*Begin:pengdecai @ record rssi states @20120427*/	
static void set_rxdata_rssi_stats(struct ieee80211_node *ni)
{
	int level;
	u_int8_t index = 0;
	
	/*
	index   level range (unit:dBm)
	0	    > -10
	1	    -10  ~ -19
	2	    -20  ~ -39
	3	    -40  ~ -49
	4	    -50  ~ -59
	5	    -60  ~ -64
	6	    -65  ~ -67
	7	    -68  ~ -70
	8	    -71  ~ -73
	9	    -74  ~ -76
	10	    -77  ~ -79
	11	    -80  ~ -82
	12	    -83  ~ -85
	13	    -86  ~ -88
	14	    -89  ~ -91
	15	    -92  ~ -94
	16	     < -94
	e.g.-19 : level >= -19 && level <= -10
	*/

	level = -95 + ni->ni_rssi;
	
	if (level >= -94 && level <= -65) {
		index = -((level + 65)/3) + 6;
	} else if (level >= -64 && level <= -60){
		index = 5;
	} else if (level >= -59 && level <= -40) {
		index = -level/10 - 1;
	} else if (level > -39 && level <= -20) {
		index = 2;
	} else if (level > -19 && level <= -10){
		index = 1;
	} else if (level > -10){
		index = 0;
	} else if (level < -94){
		index = 16;
	}
	if(index > 16 ){
		printk("rssi count  index :error\n");
		return;
	}
	ni->ni_stats.ns_rssi_stats[index].ns_rx_data ++;		
}
/*End:pengdecai @ record rssi states @20120427*/
/*Begin: Add by zhaoej for guangdong mobile vap_rssi 2013-02-21*/
static void set_vaprxdata_rssi_stats(struct ieee80211_node *ni)
{
	int level;
	u_int8_t index = 0;
    struct ieee80211vap *vap = ni->ni_vap;
	
	/*
	index   level range (unit:dBm)
	0	    > -10
	1	    -10  ~ -19
	2	    -20  ~ -39
	3	    -40  ~ -49
	4	    -50  ~ -59
	5	    -60  ~ -64
	6	    -65  ~ -67
	7	    -68  ~ -70
	8	    -71  ~ -73
	9	    -74  ~ -76
	10	    -77  ~ -79
	11	    -80  ~ -82
	12	    -83  ~ -85
	13	    -86  ~ -88
	14	    -89  ~ -91
	15	    -92  ~ -94
	16	     < -94
	e.g.-19 : level >= -19 && level <= -10
	*/

	level = -95 + ni->ni_rssi;
	
	if (level >= -94 && level <= -65) {
		index = -((level + 65)/3) + 6;
	} else if (level >= -64 && level <= -60){
		index = 5;
	} else if (level >= -59 && level <= -40) {
		index = -level/10 - 1;
	} else if (level > -39 && level <= -20) {
		index = 2;
	} else if (level > -19 && level <= -10){
		index = 1;
	} else if (level > -10){
		index = 0;
	} else if (level < -94){
		index = 16;
	}
	if(index > 16 ){
		printk("rssi count  index :error\n");
		return;
	}
    vap->iv_stats.is_rssi_stats[index].ns_rx_data++;
}
/*End: Add by zhaoej for guangdong mobile vap_rssi 2013-02-21*/


/*<Begin : add for 5G priority by caizhibang  2012-09-27 */
/*zhaoyang add for priority join in 5G*/
int
ieee80211_create_sta(u_int8_t  sta_mac[IEEE80211_ADDR_LEN])
{
	int i,flag =0;
    long irq_lock_flags;   /* Temp variable, used for irq lock/unlock parameter. wangjia 2012-10-11 */
	unsigned long long min=0;
	int min_index=0,ret_index=0;
	for(i = 0; i < IEEE80211_USERINFO_MAX; i++)
	{
		if(!(userinfo_table_t[i].ni_flags & IEEE80211_TABLE_USED))// this per was using 10000000 == 00000000
		{
		    /**
		     *  Add irq lock, spin_lock_irqsave/spin_unlock_irqrestore. wangjia 2012-10-11 
		     */ 
		    spin_lock_init(&(userinfo_table_t[i].userinfo_lock));
            
		    spin_lock_irqsave(&(userinfo_table_t[i].userinfo_lock), irq_lock_flags);
			IEEE80211_ADDR_COPY(userinfo_table_t[i].ni_mac,sta_mac);
			userinfo_table_t[i].stamp_time = jiffies_to_msecs(jiffies);
			userinfo_table_t[i].ni_flags = IEEE80211_TABLE_USED;
			
			flag = 1;
			ret_index = i;

            spin_unlock_irqrestore(&(userinfo_table_t[i].userinfo_lock), irq_lock_flags);
			if(join5g_debug)
			{
				printk("create:sta mac=%x:%x:%x:%x:%x:%x,support ",userinfo_table_t[i].ni_mac[0],userinfo_table_t[i].ni_mac[1],userinfo_table_t[i].ni_mac[2],userinfo_table_t[i].ni_mac[3],userinfo_table_t[i].ni_mac[4],userinfo_table_t[i].ni_mac[5]);
				if(userinfo_table_t[i].ni_flags & IEEE80211_TABLE_SUPPORT2G)//01000000
					printk("2G ");
				if(userinfo_table_t[i].ni_flags & IEEE80211_TABLE_SUPPORT5G)//00100000
					printk("5G ");
				printk("\n");
			}
			break;
		}
	}
	
	if(flag == 0)
	{
		i=0;
        min = userinfo_table_t[i].stamp_time;
		min_index = i;
		i++; 		
        for(; i < IEEE80211_USERINFO_MAX; i++)
		{
			if(min > userinfo_table_t[i].stamp_time)
			{
				min = userinfo_table_t[i].stamp_time;
				min_index = i;
			}
		}

        /**
         *  spin_lock_irqsave/spin_unlock_irqrestore. wangjia 2012-10-11 
         */ 
        spin_lock_irqsave(&(userinfo_table_t[min_index].userinfo_lock), irq_lock_flags);
		IEEE80211_ADDR_COPY(userinfo_table_t[min_index].ni_mac,sta_mac);		
        userinfo_table_t[min_index].stamp_time = jiffies_to_msecs(jiffies);
		userinfo_table_t[min_index].ni_flags = IEEE80211_TABLE_USED;//10000000
		spin_unlock_irqrestore(&(userinfo_table_t[min_index].userinfo_lock), irq_lock_flags);
        
		if(join5g_debug)
		{
			printk("cover:sta mac=%x:%x:%x:%x:%x:%x support ",userinfo_table_t[min_index].ni_mac[0],userinfo_table_t[min_index].ni_mac[1],userinfo_table_t[min_index].ni_mac[2],userinfo_table_t[min_index].ni_mac[3],userinfo_table_t[min_index].ni_mac[4],userinfo_table_t[min_index].ni_mac[5]);
			if(userinfo_table_t[min_index].ni_flags & IEEE80211_TABLE_SUPPORT2G)//01000000
				printk("2G ");
			if(userinfo_table_t[min_index].ni_flags & IEEE80211_TABLE_SUPPORT5G)//00100000
				printk("5G ");
			printk("\n");
		}
		ret_index = min_index;
	}

	return  ret_index;
}
int 
ieee80211_has_sta(u_int8_t  sta_mac[IEEE80211_ADDR_LEN])
{
	int i,flag =0;
	for(i=0;i<IEEE80211_USERINFO_MAX;i++)
	{
		if(userinfo_table_t[i].ni_flags & IEEE80211_TABLE_USED)// this per was using 10000000
		{
			if(IEEE80211_ADDR_EQ(userinfo_table_t[i].ni_mac,sta_mac))
			{
				if(join5g_debug)
				{
					printk("%s:find a sta in userinfo_table_t,mac is %x:%x:%x:%x:%x:%x,support ",__func__,userinfo_table_t[i].ni_mac[0],userinfo_table_t[i].ni_mac[1],userinfo_table_t[i].ni_mac[2],userinfo_table_t[i].ni_mac[3],userinfo_table_t[i].ni_mac[4],userinfo_table_t[i].ni_mac[5]);
					if(userinfo_table_t[i].ni_flags & IEEE80211_TABLE_SUPPORT2G)//01000000
						printk("2G ");
					if(userinfo_table_t[i].ni_flags & IEEE80211_TABLE_SUPPORT5G)//00100000
						printk("5G ");
					printk("\n");
				}
				flag =1;
				break;
			}
		}
	}
	if(flag)
	{
		return i;
	}
	else 
	{
		return -1;
	}
}
/*zhaoyang add end*/
/*End: add for 5G priority by caizhibang  2012-09-27 >*/


/**
 *  Begin: Added by WangJia, for traffic limit. 2012-11-02.
 *  Because ieee80211_input_data is static.
 */
void 
ieee80211_input_data_for_tl(struct ieee80211_node *ni, wbuf_t wbuf, 
                         struct ieee80211_rx_status *rs, int subtype, int dir)
{
    ieee80211_input_data(ni, wbuf, rs, subtype, dir);
}
/* End: Added by WangJia, for traffic limit. 2012-11-02.*/

/*
 * Process a received frame.  The node associated with the sender
 * should be supplied.  If nothing was found in the node table then
 * the caller is assumed to supply a reference to iv_bss instead.
 * The RSSI and a timestamp are also supplied.  The RSSI data is used
 * during AP scanning to select a AP to associate with; it can have
 * any units so long as values have consistent units and higher values
 * mean ``better signal''.  
 */
int
ieee80211_input(struct ieee80211_node *ni, wbuf_t wbuf, struct ieee80211_rx_status *rs)
{
#define QOS_NULL   (IEEE80211_FC0_TYPE_DATA | IEEE80211_FC0_SUBTYPE_QOS_NULL)
#define HAS_SEQ(type, subtype)   (((type & 0x4) == 0) && ((type | subtype) != QOS_NULL))
    struct ieee80211com *ic = ni->ni_ic;
/* Begin:gengzj added for wifipos */
    struct ath_softc_net80211   *scn = ATH_SOFTC_NET80211(ic);
    struct ath_softc *sc = scn->sc_dev;
/* End:gengzongjie added end */
    struct ieee80211_frame *wh;
    struct ieee80211vap *vap = ni->ni_vap;
    struct ieee80211_phy_stats *phy_stats;
    struct ieee80211_mac_stats *mac_stats;
    struct ieee80211_stats *vap_stats;
    int type = -1, subtype, dir;
    u_int16_t rxseq;
    u_int8_t *bssid;
	/*zhaoyang1 transplant from 717*/
    /*Begin:Added by duanmingzhe for thinap*/
    struct net_device *dev = OSIF_TO_NETDEV(vap->iv_ifp);
    wbuf_t wbuf_capture = NULL;
/*<Begin : caizhibang modify for autelan 802.3 tunnel 2012-5-25*/	
   // wlan_ng_prism2_header *ph = NULL;/*zhaoyang modify for del the prism2 header*/
 /*End : caizhibang modify for autelan 802.3 tunnel 2012-5-25 >*/  
    int sendtowtpd;
    int ret = 0;
   // struct llc *llc_type = NULL;
    u_int8_t tid = 0; //pengruofeng add tid for spoof attack 2011-5-27
    u_short frametype;
    sendtowtpd = 0;
    frametype = 0;
    /*End:Added by duanmingzhe for thinap*/
	/*zhaoyang1 transplant end*/
   
    KASSERT((wbuf_get_pktlen(wbuf) >= ic->ic_minframesize),
            ("frame length too short: %u", wbuf_get_pktlen(wbuf)));

    wbuf_set_node(wbuf, ni);


    if (wbuf_get_pktlen(wbuf) < ic->ic_minframesize) {
		/*zhaoyang1 transplant from 717*/
		/*zhaoyang modify for error frame count CHINAMOBILE-105*/
		vap->iv_stats.is_rx_tooshort++;
		/*zhaoyang modify end*/
		/*zhaoyang1 transplant end*/
        goto bad1;
    }

    wh = (struct ieee80211_frame *) wbuf_header(wbuf);
    if ((wh->i_fc[0] & IEEE80211_FC0_VERSION_MASK) != IEEE80211_FC0_VERSION_0) {
        /* XXX: no stats for it. */
        goto bad1;
    }

    type = wh->i_fc[0] & IEEE80211_FC0_TYPE_MASK;
    subtype = wh->i_fc[0] & IEEE80211_FC0_SUBTYPE_MASK;
    dir = wh->i_fc[1] & IEEE80211_FC1_DIR_MASK;

	/*zhaoyang1 transplant from 717*/
	/*zhaoyang add start for stop recv and send*/
	//zhaoyang disable this function at 2012-04-28
//	if(stop_transmit &&(vap->iv_opmode == IEEE80211_M_HOSTAP)&&(subtype!=IEEE80211_FC0_SUBTYPE_BEACON)){
		//printk("%s:line %d,error frame recv after down\n",__func__,__LINE__);
//		goto bad1;
//	}
	/*zhaoyang add end*/
	/*zhaoyang1 transplant end*/

    if (OS_ATOMIC_CMPXCHG(&vap->iv_rx_gate, 0, 1) != 0) {
        goto bad1;
    }

    if (!ieee80211_vap_active_is_set(vap)) {
        if (vap->iv_input_mgmt_filter && type == IEEE80211_FC0_TYPE_MGT && IEEE80211_ADDR_EQ(wh->i_addr1, vap->iv_myaddr) ) {
              vap->iv_input_mgmt_filter(ni,wbuf,subtype,rs) ;
        } 
        IEEE80211_DPRINTF(vap, IEEE80211_MSG_INPUT, "vap in not active, %s \n", "discard the frame");
        goto bad;
    }
	/* Begin: gengzj added for wifipos 2013-11-26 */
	  dir = wh->i_fc[1] & IEEE80211_FC1_DIR_MASK;
	/* End: gengzj added end */
	/* Mark node as WDS */
    if (dir == IEEE80211_FC1_DIR_DSTODS)
    {	
	/* Begin: gengzj added for wifipos 2013-11-26 */
		if(vap->iv_sl_debug){
			struct ieee80211_frame_addr4* wh = (struct ieee80211_frame_addr4*)wbuf_header(wbuf);
	    	printk("wh->i_fc[0] = 0x%02x,wh->i_fc[1] = 0x%02x,wh->i_dur[0] = 0x%02x,wh->i_dur[1] = 0x%02x \n",wh->i_fc[0],wh->i_fc[1],wh->i_dur[0],wh->i_dur[1]);
			printk("wh2:%02x:%02x:%02x:%02x:%02x:%02x\n",wh->i_addr2[0],wh->i_addr2[1],wh->i_addr2[2],wh->i_addr2[3],wh->i_addr2[4],wh->i_addr2[5]);
			printk("wh4:%02x:%02x:%02x:%02x:%02x:%02x\n",wh->i_addr4[0],wh->i_addr4[1],wh->i_addr4[2],wh->i_addr4[3],wh->i_addr4[4],wh->i_addr4[5]);
   		}
	/* End: gengzj added end */
	    ni->ni_flags |= IEEE80211_NODE_WDS;
    }
/* Begin: gengzj added for wifipos 2013-11-26 */
	/*AUTELAN-Added-begin:Added by pengdecai for wifi scan locate function*/
	if(vap->iv_locate) {
		ieee80211_scan_locate_nlsend(ni,wbuf,subtype,rs);	
	    goto bad;
	}
	//else if(vap->iv_sl_asinfo && iv_scan_notlocate.sl_sock)
	else if(sc->sc_locate && iv_scan_notlocate.sl_sock)	//change vap->iv_sl_asinfo to sc->sc_locate
	{
		ieee80211_scan_locate_nlsend(ni,wbuf,subtype,rs);	
	}
	/*AUTELAN-Added-end:Added by pengdecai for wifi scan locate function*/	
/* End: gengzj added end */
    /*
     * deliver 802.11 frame if the OS is interested in it.
     * if os returns a non zero value, drop the frame .
     */
    if (vap->iv_evtable && vap->iv_evtable->wlan_receive_filter_80211) {
        if (vap->iv_evtable->wlan_receive_filter_80211(vap->iv_ifp, wbuf, type, subtype, rs)) {
            goto bad;
        }
    }

    phy_stats = &ic->ic_phy_stats[ic->ic_curmode];
    mac_stats = IEEE80211_IS_MULTICAST(wh->i_addr1) ? &vap->iv_multicast_stats : &vap->iv_unicast_stats;
    vap_stats = &vap->iv_stats;

    /*
     * XXX Validate received frame if we're not scanning.
     * why do we receive only data frames when we are scanning and 
     * current (foreign channel) channel is the bss channel ?
     * should we simplify this to if (vap->iv_bsschan == ic->ic_curchan) ?
     */
    if ((ieee80211_scan_in_home_channel(ic->ic_scanner)) ||
        ((vap->iv_bsschan == ic->ic_curchan) && (type == IEEE80211_FC0_TYPE_DATA))) {
        switch (vap->iv_opmode) {
        case IEEE80211_M_STA:
            bssid = wh->i_addr2;
            if (ni != vap->iv_bss && dir != IEEE80211_FC1_DIR_DSTODS)
                bssid = wh->i_addr3; 

            if (!IEEE80211_TDLS_ENABLED(vap) &&
                !IEEE80211_ADDR_EQ(bssid, ieee80211_node_get_bssid(ni))) {
                /*
                 * If this is a beacon from APs other than the one we are connected, update scan entry
                 * to have fast roaming response in case we need to. Note, this only helps when both APs
                 * are in the same channel.
                 */
                if ((type == IEEE80211_FC0_TYPE_MGT) && 
                    ((subtype == IEEE80211_FC0_SUBTYPE_BEACON) ||
                     (subtype == IEEE80211_FC0_SUBTYPE_PROBE_RESP))) {
                    ieee80211_update_beacon(ni, wbuf, wh, subtype, rs);
                }
                
                mac_stats->ims_rx_discard++;
                goto bad;
            }

            /*
             * WAR for excessive beacon miss on SoC.
             * Reset bmiss counter when we receive a non-probe request
             * frame from our home AP, and save the time stamp.
             */
            if ((ieee80211_vap_ready_is_set(vap)) &&
                (!((type == IEEE80211_FC0_TYPE_MGT) &&
                   (subtype == IEEE80211_FC0_SUBTYPE_PROBE_REQ)))) {

                if (vap->iv_bmiss_count > 0) {
#ifdef ATH_SWRETRY                     
                    /* Turning on the sw retry mechanism. This should not
                     * produce issues even if we are in the middle of 
                     * cleaning sw retried frames
                     */
                    ic->ic_set_swretrystate(vap->iv_bss, TRUE);
#endif                    
                    IEEE80211_DPRINTF(vap, IEEE80211_MSG_SCAN,
                                      "clear beacon miss. frm type=%02x, subtype=%02x\n",
                                      type, subtype);
                    ieee80211_mlme_reset_bmiss(vap);    
                }

                /* 
                 * Beacon timestamp will be set when beacon is processed.
                 * Set directed frame timestamp if frame is not multicast or
                 * broadcast.
                 */
                if (! IEEE80211_IS_MULTICAST(wh->i_addr1)) {
                    vap->iv_last_directed_frame = OS_GET_TIMESTAMP();
                }
            }
            break;

        case IEEE80211_M_IBSS:
        case IEEE80211_M_AHDEMO:
            if (dir != IEEE80211_FC1_DIR_NODS)
                bssid = wh->i_addr1;
            else if (type == IEEE80211_FC0_TYPE_CTL)
                bssid = wh->i_addr1;
            else {
                if (wbuf_get_pktlen(wbuf) < sizeof(struct ieee80211_frame)) {
                    mac_stats->ims_rx_discard++;
					/*zhaoyang1 transplant from 717*/
					/*zhaoyang modify for error frame count CHINAMOBILE-105*/
					vap->iv_stats.is_rx_tooshort++;
					/*zhaoyang modify end*/
					/*zhaoyang1 transplant end*/
                    goto bad;
                }
                bssid = wh->i_addr3;
            }

            if (type != IEEE80211_FC0_TYPE_DATA)
                break;

            /*
             * Data frame, validate the bssid.
             */
            if (!IEEE80211_ADDR_EQ(bssid, ieee80211_node_get_bssid(vap->iv_bss)) &&
                !IEEE80211_ADDR_EQ(bssid, IEEE80211_GET_BCAST_ADDR(ic)) &&
                subtype != IEEE80211_FC0_SUBTYPE_BEACON) {
                /* not interested in */
                mac_stats->ims_rx_discard++;
                goto bad;
            }
            break;

        case IEEE80211_M_HOSTAP:
            if (dir != IEEE80211_FC1_DIR_NODS)
                bssid = wh->i_addr1;
            else if (type == IEEE80211_FC0_TYPE_CTL)
                bssid = wh->i_addr1;
            else {
                if (wbuf_get_pktlen(wbuf) < sizeof(struct ieee80211_frame)) {
                    mac_stats->ims_rx_discard++;
					/*zhaoyang1 transplant from 717*/
					/*zhaoyang modify for error frame count CHINAMOBILE-105*/
					vap->iv_stats.is_rx_tooshort++;
					/*zhaoyang modify end*/
					/*zhaoyang1 transplant end*/
                    goto bad;
                }
                bssid = wh->i_addr3;
            }
            if (type != IEEE80211_FC0_TYPE_DATA)
                break;
            /*
             * Data frame, validate the bssid.
             */
            if (!IEEE80211_ADDR_EQ(bssid, ieee80211_node_get_bssid(vap->iv_bss)) &&
                !IEEE80211_ADDR_EQ(bssid, IEEE80211_GET_BCAST_ADDR(ic)) &&
                subtype != IEEE80211_FC0_SUBTYPE_BEACON) {
                /*
                 * NAWDS repeaters send ADDBA with the BSSID 
                 * set to their BSSID and not ours 
                 * We should not drop those frames
                 * However these are control frames with 
                 * type = IEEE80211_FC0_TYPE_MGT &
                 * subtype = IEEE80211_FC0_SUBTYPE_ACTION
                 * So will not reach here.
                 * Any packet reaching here, not passing the bssid
                 * validation needs to be dropped.
                 */
                 mac_stats->ims_rx_discard++;
                 goto bad;
            }
            break;

        case IEEE80211_M_WDS:
            if (wbuf_get_pktlen(wbuf) < sizeof(struct ieee80211_frame_addr4)) {
                if ((type != IEEE80211_FC0_TYPE_MGT) && 
                    (subtype != IEEE80211_FC0_SUBTYPE_DEAUTH)) {
                    vap_stats->is_rx_tooshort++;
                    goto bad;
                }
            }
            bssid = wh->i_addr1;
            if (!IEEE80211_ADDR_EQ(bssid, vap->iv_bss->ni_bssid) &&
                !IEEE80211_ADDR_EQ(bssid, IEEE80211_GET_BCAST_ADDR(ic))) {
                /* not interested in */
                IEEE80211_DPRINTF(vap, IEEE80211_MSG_INPUT, "*** WDS *** "
                                  "WDS ADDR1 %s, BSSID %s\n",
                                  ether_sprintf(bssid),
                                  ether_sprintf(ieee80211_node_get_bssid(vap->iv_bss)));
                vap_stats->is_rx_wrongbss++;
                goto bad; 
            }
            break;
            
        case IEEE80211_M_BTAMP:
            break;
            
        default:
            goto bad;
        }

        ni->ni_rssi = rs->rs_rssi;
        
        /*Start: Add by zhanghu to statistics of every frame by the rate*/         
        if( IEEE80211_FC0_TYPE_DATA  == type && \
            !(wh->i_fc[1] & IEEE80211_FC1_RETRY)){
            ieee80211_ni_statistics_rate_for_rx_frame(ni);        

            ieee80211_vap_statistics_rate_for_rx_frame(vap, rs);


            /*Begin:pengdecai @ record rssi states @20120427*/	
            set_rxdata_rssi_stats(ni);
		    /*End:pengdecai@20120427*/

            /*Begin: Add by zhaoej for guangdong mobile vap_rssi 2013-02-21*/
            set_vaprxdata_rssi_stats(ni);
            /*End: Add by zhaoej for guangdong mobile 2013-02-21*/

        }
        
        /*End: Add by zhanghu to statistics of every frame by the rate*/

        /* Check duplicates */
        if (HAS_SEQ(type, subtype)) {
        /*zhaoyang1 transplant from 717*/
        //u_int8_t tid; pengruofeng del tid for spoof attack 2011-5-27
        /*zhaoyang1 transplant end*/
            if (IEEE80211_QOS_HAS_SEQ(wh)) {
                if (dir == IEEE80211_FC1_DIR_DSTODS) {
                    tid = ((struct ieee80211_qosframe_addr4 *)wh)->
                        i_qos[0] & IEEE80211_QOS_TID;
                } else {
                    tid = ((struct ieee80211_qosframe *)wh)->
                        i_qos[0] & IEEE80211_QOS_TID;
                }
                if (TID_TO_WME_AC(tid) >= WME_AC_VI)
                    ic->ic_wme.wme_hipri_traffic++;
            } else {
                if (type == IEEE80211_FC0_TYPE_MGT)
                    tid = IEEE80211_TID_SIZE; /* use different pool for rx mgt seq number */
                else
                    tid = IEEE80211_NON_QOS_SEQ;
            }

            rxseq = le16toh(*(u_int16_t *)wh->i_seq);
            if ((wh->i_fc[1] & IEEE80211_FC1_RETRY) &&
                (rxseq == ni->ni_rxseqs[tid])) {
                phy_stats->ips_rx_dup++;
				/*zhaoyang1 transplant from 717*/
				/*zhaoyang modify for retry packet count ISSUES-665*/
				if(ieee80211_node_is_authorized(ni))
				{
					if(type == IEEE80211_FC0_TYPE_DATA)
					{
						//vap->iv_stats.is_rx_repack++;
						//vap->iv_stats.is_rx_rebytes += wbuf->len;
						vap->iv_unicast_stats.ims_rx_retry_packets++;
						vap->iv_unicast_stats.ims_rx_retry_bytes += wbuf->len;
						/*zhaoyang modify for ns retry count PCAPVXN-136*/						
						IEEE80211_NODE_STAT(ni, rx_retry_packets);
        				IEEE80211_NODE_STAT_ADD(ni, rx_retry_bytes,wbuf->len);
						/*zhaoyang modify end*/
					}
					else 
					{
						//vap->iv_stats.is_rx_mgmt_repack++;
						//vap->iv_stats.is_rx_mgmt_rebytes += wbuf->len;
						vap->iv_unicast_stats.ims_rx_retry_mgmt_packets++;
						vap->iv_unicast_stats.ims_rx_retry_mgmt_bytes += wbuf->len;
					}
				}
				/*zhaoyang modify end*/
				/*zhaoyang1 transplant end*/
			             
                if (ni->ni_last_rxseqs[tid] == ni->ni_rxseqs[tid]) {
                    phy_stats->ips_rx_mdup++;
                }
                ni->ni_last_rxseqs[tid] = ni->ni_rxseqs[tid];
                goto bad;
            }
            ni->ni_rxseqs[tid] = rxseq;

#if ATH_SUPPORT_AOW
            rs->rs_rxseq = rxseq >> IEEE80211_SEQ_SEQ_SHIFT;
#endif
        }
    }

    /*
     * Check for power save state change.
     */
    if ((vap->iv_opmode == IEEE80211_M_HOSTAP) && 
        (ni != vap->iv_bss) &&
        !(type == IEEE80211_FC0_TYPE_MGT && subtype == IEEE80211_FC0_SUBTYPE_PROBE_REQ))
    {
        if ((wh->i_fc[1] & IEEE80211_FC1_PWR_MGT) ^
            (ni->ni_flags & IEEE80211_NODE_PWR_MGT)) {
            ieee80211_mlme_node_pwrsave(ni, wh->i_fc[1] & IEEE80211_FC1_PWR_MGT);
        }
    }
    /*zhaoyang1 transplant from 717*/
    /*Begin:Added by duanmingzhe for capwap_spilt_fast*/
    //wbuf->splitmac = vap->vap_splitmac; //del by duanmingzhe
    /*End:Added by duanmingzhe for capwap_spilt_fast*/
    /*zhaoyang1 transplant end*/
    if (type == IEEE80211_FC0_TYPE_DATA) {
        if (!ieee80211_vap_ready_is_set(vap)) {
            goto bad;
        }
        /* ieee80211_input_data consumes the wbuf */
		 /*zhaoyang modify for do not dropped when unauth*/
		if(ieee80211_node_is_authorized(ni))
			ieee80211_node_activity(ni); /* node has activity */
		/*zhaoyang modify end*/
#if ATH_SW_WOW
        if (wlan_get_wow(vap)) {
            ieee80211_wow_magic_parser(ni, wbuf);
        }
#endif
		/*zhaoyang1 transplant from 717*/
		/*pengruofeng add for spoofing attack of data frame 2011-5-27*/
		if (vap->spoof_detect)
		{
			int seq = 0;
			/*Begin:Added by duanmingzhe for fixing bug of spoof detect 2011-02-16*/
			if(ni->ni_rxseqs_last[tid] == 0)
			{
				ni->ni_rxseqs_last[tid] = ni->ni_rxseqs[tid];
			}
			seq = (((ni->ni_rxseqs[tid] >> IEEE80211_SEQ_SEQ_SHIFT) % 4090) - (((ni->ni_rxseqs_last[tid] >> IEEE80211_SEQ_SEQ_SHIFT) + 1) % 4090));
			/*End:Added by duanmingzhe for fixing bug of spoof detect 2011-02-16*/
			if ( seq > vap->iv_seq_detect_num || seq < 0)
			{
				goto bad;
			}
			/*Begin:Added by duanmingzhe for fixing bug of spoof detect 2011-02-16*/
			ni->ni_rxseqs_last[tid] = ni->ni_rxseqs[tid];
			/*End:Added by duanmingzhe for fixing bug of spoof detect 2011-02-16*/
		}
		/*pengruofeng -- add end 2011-5-27*/

		/*pengruofeng add for wids 2011-5-30*/
		if (vap->iv_opmode == IEEE80211_M_HOSTAP && vap->flood_detect && subtype == IEEE80211_FC0_SUBTYPE_NODATA){
			struct wids_detect_attacker_entry *match, *next;
			match = next = NULL;

			WIDS_LOCK_IRQ(&(vap->iv_sta));
			LIST_FOREACH(next, &vap->iv_sta.attacker, attacker) {
				if (!memcmp(next->mac, wh->i_addr2, IEEE80211_ADDR_LEN)) {
					match = next;
					match->rssi = rs->rs_rssi;
					break;
				}
			}

			if(match == NULL){
				match= kmalloc(sizeof(*match), GFP_KERNEL);
				if (match == NULL)
					panic("sta_add: can't malloc memory");
				
				match->assoc_req_cnt=0;
				match->auth_req_cnt=0;
				match->deauth_cnt=0;
				match->disassoc_req_cnt=0;
				match->null_data_cnt=0;
				match->probe_req_cnt=0;
				match->reassoc_req_cnt=0;
				match->action_cnt=0;
				match->attack_cnt=0;
				match->rssi=0;
				match->spoof_deauth_cnt=0;
				match->spoof_disassoc_cnt=0;
				match->weakiv_cnt=0;
				memcpy(match->mac,wh->i_addr2,IEEE80211_ADDR_LEN);
				LIST_INSERT_HEAD(&vap->iv_sta.attacker, match, attacker);
				(vap->iv_sta).attacker_list_node_cnt++;
			}
			
			match->null_data_cnt++;
			WIDS_UNLOCK_IRQ(&(vap->iv_sta));
			
		}
		/*pengruofeng add end 2011-5-30*/
		/*zhaoyang1 transplant end*/
		
		/* Begin: Added by wangia, for traffic limit. 2012-11-02 */
        if((IEEE80211_TL_ENABLE == vap->vap_tl_vap_enable) &&          // Vap
           (vap->vap_tl_up_srtcm_vap.sr_cir > 0))
        {
            ieee80211node_pause(ni); 
            ret = ieee80211_tl_vap_cache_enqueue_rx(vap, wbuf, rs, subtype, dir, ic);
            ieee80211node_unpause(ni);
            
            if(ret == IEEE80211_TL_ENQUEUE_OK)
            {
                (void) OS_ATOMIC_CMPXCHG(&vap->iv_rx_gate, 1, 0);
                return type;
            }
            else if(ret == IEEE80211_TL_ENQUEUE_IS_FULL)
            {
                goto bad;
            }
            // In these two situations, we receive it right away.
            // else if(ret == IEEE80211_TL_ENQUEUE_PACKET_LEN)
            // else if(ret == IEEE80211_TL_ENQUEUE_CACHE_IS_DISABLE)
        }
        else if ((IEEE80211_TL_ENABLE == ni->ni_tl_sp_enable && ni->ni_tl_up_srtcm_sp.sr_cir > 0) ||    // Specific node 
                 (IEEE80211_TL_ENABLE == ni->ni_tl_ev_enable && ni->ni_tl_up_srtcm_ev.sr_cir > 0))      // Everynode
        {
            /**
             *  pause it to make sure that no one else unpaused it after 
             *  the node_is_paused check above, pause operation is ref counted 
             */  
            ieee80211node_pause(ni); 
            ret = ieee80211_tl_node_cache_enqueue_rx(ni, wbuf, rs, subtype, dir, ic);
            ieee80211node_unpause(ni); /* unpause it if we are the last one, the frame will be flushed out */  

            if(ret == IEEE80211_TL_ENQUEUE_OK)
            {
                (void) OS_ATOMIC_CMPXCHG(&vap->iv_rx_gate, 1, 0);
                return type;
            }
            else if(ret == IEEE80211_TL_ENQUEUE_IS_FULL)
            {
                goto bad;
            }
            // In these two situations, we receive it right away.
            // else if(ret == IEEE80211_TL_ENQUEUE_PACKET_LEN)
            // else if(ret == IEEE80211_TL_ENQUEUE_CACHE_IS_DISABLE)
        }  
        /*End: Added by WangJia, for traffic limit. 2012-11-02.*/
		
        ieee80211_input_data(ni, wbuf, rs, subtype, dir); 
    } else if (type == IEEE80211_FC0_TYPE_MGT) {
        /* ieee80211_recv_mgmt does not consume the wbuf */
        if (subtype != IEEE80211_FC0_SUBTYPE_PROBE_REQ) {
            ieee80211_node_activity(ni); /* node has activity */
        }
		/*<Begin : add for 5G priority by caizhibang  2012-09-27 */
        /*Begin, only deal with Probe Request and Authentication frames. wangjia 2012-10-16 */
		if(join5g_enable && (IEEE80211_FC0_SUBTYPE_PROBE_REQ == subtype || IEEE80211_FC0_SUBTYPE_AUTH == subtype))
        /*End, wangjia 2012-10-16 */
        {   
	        /*Begin, 2.4G signal strength threshold, if current strength greater than threshold, 
	          then carry out 5G priority procedure.  wangjia 2012-10-16 */
	        if(join5g_debug)
	        {
                printk("=====5G Priority: Sta[XX:XX:XX:XX:%02X:%02X] RSSI: %d, IC-%s, ",
                    wh->i_addr2[4], wh->i_addr2[5], ni->ni_rssi, 
                    IEEE80211_IS_CHAN_5GHZ(ic->ic_curchan)?"5.8G":"2.4G");
	        }
	        if(IEEE80211_IS_CHAN_5GHZ(ic->ic_curchan) || 
               (IEEE80211_IS_CHAN_2GHZ(ic->ic_curchan) && ni->ni_rssi >= sta_signal_strength_thr))
            /*End, wangjia 2012-10-16 */
            {   
                if(join5g_debug && IEEE80211_IS_CHAN_2GHZ(ic->ic_curchan))
    	        {
                    printk("2.4G signal is enough!\r\n");
    	        }
                struct net_device *tmp_dev = NULL;
                if (memcmp(ic->ic_osdev->netdev->name, "wifi0", 5) == 0) 
                {
                    #if LINUX_VERSION_CODE <= KERNEL_VERSION(2,6,22)
                          tmp_dev = dev_get_by_name("wifi1");
                    #else
                          tmp_dev = dev_get_by_name(&init_net,"wifi1");
                    #endif
                }
                else if (memcmp(ic->ic_osdev->netdev->name, "wifi1", 5) == 0)
                {
                    #if LINUX_VERSION_CODE <= KERNEL_VERSION(2,6,22)
                    tmp_dev = dev_get_by_name("wifi0");
                    #else
                    tmp_dev = dev_get_by_name(&init_net,"wifi0");
                    #endif
                }

                if (tmp_dev)
                { 
                    int sta_index;
                    long irq_lock_flags;  /* Temp variable, used for irq lock/unlock parameter. wangjia 2012-10-11 */
                    struct ieee80211vap  *vap_tmp = NULL;
                    struct ieee80211com  *ic_tmp = (struct ieee80211com *)ath_netdev_priv(tmp_dev); 
                    
                    if (ic_tmp)
                    {
                        /* Begin, Add for the judgement of 5G priority sta threshold. wangjia 2012-10-11 */
                        if((IEEE80211_IS_CHAN_5GHZ(ic_tmp->ic_curchan) && (ic_tmp->ic_sta_assoc < stacount_thr)) ||
                           (IEEE80211_IS_CHAN_5GHZ(ic->ic_curchan) && (ic->ic_sta_assoc < stacount_thr)))
                        {
                        /* End, Add for the judgement of 5G priority sta threshold. wangjia 2012-10-11 */
                            TAILQ_FOREACH(vap_tmp, &ic_tmp->ic_vaps, iv_next)
                            if((vap_tmp->iv_bss->ni_esslen == vap->iv_bss->ni_esslen) &&
                               (memcmp(vap_tmp->iv_bss->ni_essid, vap->iv_bss->ni_essid, vap->iv_bss->ni_esslen) == 0))
                            {
                                unsigned int current_time;
                                sta_index = ieee80211_has_sta(wh->i_addr2);
                                if(subtype == IEEE80211_FC0_SUBTYPE_PROBE_REQ)
                                {
                                	if(sta_index == -1)
                                	{
                                		sta_index = ieee80211_create_sta(wh->i_addr2);
                                	}
                                    current_time = jiffies_to_msecs(jiffies);
                                    if(IEEE80211_IS_CHAN_2GHZ(ic->ic_curchan))
                                	{                  
                                	    if(userinfo_table_t[sta_index].marked == 0)
                                    	{
                                    	    if(current_time - userinfo_table_t[sta_index].stamp_time <= scantime_thr) 
                                    		{   /**
                                    		     *  spin_lock_irqsave/spin_unlock_irqrestore. wangjia 2012-10-11 
                                    		     */ 
                                    		    spin_lock_irqsave(&(userinfo_table_t[sta_index].userinfo_lock), irq_lock_flags);
                                                userinfo_table_t[sta_index].ni_flags |= IEEE80211_TABLE_SUPPORT2G;// set the 2g flag
                                                userinfo_table_t[sta_index].count ++;
                                                userinfo_table_t[sta_index].stamp_time= current_time; 
                                                if(userinfo_table_t[sta_index].count >= discard_count)
                                                {
                                                    userinfo_table_t[sta_index].marked = 1;
                                                }
                                                spin_unlock_irqrestore(&(userinfo_table_t[sta_index].userinfo_lock), irq_lock_flags);
                                                dev_put(tmp_dev); /* Release reference to device. wangjia 2012-10-11 */
                                                goto bad;
                                    		}
                                    		else
                                    		{
                                    		    spin_lock_irqsave(&(userinfo_table_t[sta_index].userinfo_lock), irq_lock_flags);
                                    		    userinfo_table_t[sta_index].ni_flags &= ~(IEEE80211_TABLE_SUPPORT2G | IEEE80211_TABLE_SUPPORT5G);
                                                userinfo_table_t[sta_index].ni_flags |= IEEE80211_TABLE_SUPPORT2G;// set the 2g flag
                                    			userinfo_table_t[sta_index].stamp_time= current_time;
                                    			userinfo_table_t[sta_index].count = 1;
                                                spin_unlock_irqrestore(&(userinfo_table_t[sta_index].userinfo_lock), irq_lock_flags);
                                                dev_put(tmp_dev);
                                    			goto bad;
                                    		}
                                    	}
                                	    else
                                    	{
                                            if(current_time - userinfo_table_t[sta_index].stamp_time <= agingtime_thr) 
                                            {
                                                if(userinfo_table_t[sta_index].ni_flags & IEEE80211_TABLE_SUPPORT5G)
                                                {
                                                    dev_put(tmp_dev);
                                                    goto bad;
                                                }		   
                                            }
                                    		else
                                    		{
                                    		    spin_lock_irqsave(&(userinfo_table_t[sta_index].userinfo_lock), irq_lock_flags);
                                    		    userinfo_table_t[sta_index].ni_flags &= ~(IEEE80211_TABLE_SUPPORT2G | IEEE80211_TABLE_SUPPORT5G);
                                                userinfo_table_t[sta_index].ni_flags |= IEEE80211_TABLE_SUPPORT2G;// set the 2g flag
                                    			userinfo_table_t[sta_index].stamp_time= current_time;
                                    			userinfo_table_t[sta_index].count = 1;
                                    			userinfo_table_t[sta_index].marked = 0;
                                                spin_unlock_irqrestore(&(userinfo_table_t[sta_index].userinfo_lock), irq_lock_flags);
                                                dev_put(tmp_dev);
                                    			goto bad;
                                    		}
                                    	}
                                    }
                                    else if(IEEE80211_IS_CHAN_5GHZ(ic->ic_curchan))
                                    {
                                        /**
                            		     *  spin_lock_irqsave/spin_unlock_irqrestore. wangjia 2012-10-11 
                            		     */ 
                                        spin_lock_irqsave(&(userinfo_table_t[sta_index].userinfo_lock), irq_lock_flags);
                                        userinfo_table_t[sta_index].ni_flags |= IEEE80211_TABLE_SUPPORT5G;// set the 5g flag	
                                        userinfo_table_t[sta_index].marked =1;
                                        userinfo_table_t[sta_index].stamp_time = current_time;
                                        spin_unlock_irqrestore(&(userinfo_table_t[sta_index].userinfo_lock), irq_lock_flags);
                                    }
                                }
                                else if(subtype == IEEE80211_FC0_SUBTYPE_AUTH)	
                                {
                                    if(sta_index != -1)
                                    {
                                        if(IEEE80211_IS_CHAN_2GHZ(ic->ic_curchan))
                                        {
                                    		if(userinfo_table_t[sta_index].marked == 0)
                                    		{ 
                                    		    dev_put(tmp_dev);
                                    		    goto bad;
                                    		}
                                    		else
                                    		{
                                                if(userinfo_table_t[sta_index].ni_flags & IEEE80211_TABLE_SUPPORT5G)
                                                {
                                                    dev_put(tmp_dev);
                                                    goto bad;
                                                }
                                    		}
                                        }
                                    }
                                }
                            }	
                        }
                    }	
                    dev_put(tmp_dev); /* Release reference to device. wangjia 2012-10-11 */
                }
            }
            else if(join5g_debug)
	        {
                printk("2.4G signal is NOT enough!\r\n");
	        }
         }
		/*End: add for 5G priority by caizhibang  2012-09-27 >*/
        if (vap->iv_input_mgmt_filter == NULL || 
            (vap->iv_input_mgmt_filter && vap->iv_input_mgmt_filter(ni,wbuf,subtype,rs) == 0)) {
	        /*
             * if no filter function is installed (0)
             * if not filtered by the filter function
             * then process  management frame.
             */
	/*zhaoyang1 transplant from 717*/
				/*zhaoyang add for crypto decap*/
				
				/*
			     * can only happen for HOST AP mode .
			     */
			    if (wh->i_fc[1] & IEEE80211_FC1_WEP) {
					if(subtype == IEEE80211_FC0_SUBTYPE_AUTH){
				        struct ieee80211_key *key;
					    key = ieee80211_crypto_decap(ni, wbuf, ieee80211_hdrspace(ni->ni_ic,wh), rs);
				        if (key) {						
				            wh = (struct ieee80211_frame *) wbuf_header(wbuf);
				            wh->i_fc[1] &= ~IEEE80211_FC1_WEP;
				        }
				        /*
				         * if crypto decap fails (key == null) let the 
				         * ieee80211_mlme_recv_auth will discard the request(algo and/or
				         * auth seq # is going to be bogus) and also handle any stattion
				         * ref count. 
				         */
					}
		    	}
			/*zhaoyang add end*/
		/*add-chenming*/
    /*modified by chenming for  statistics*/
    if(vap->iv_rssi_threshold && (ni->ni_rssi < vap->iv_rssi_threshold)) //AUTELAN-Added:modified by duanmingzhe for necessary check 
    {
      if(type == IEEE80211_FC0_TYPE_MGT && \
        (subtype == IEEE80211_FC0_SUBTYPE_ASSOC_REQ || \
         subtype == IEEE80211_FC0_SUBTYPE_REASSOC_REQ || \
         subtype == IEEE80211_FC0_SUBTYPE_AUTH \
		 /* Begin: Added by wangjia, add support for Probe Request. 2012-10-15 */
		 || subtype == IEEE80211_FC0_SUBTYPE_PROBE_REQ) 
		 /* End: Added by wangjia, 2012-10-15. */
        )
        {
        	vap->iv_stats.is_refuse_LowRssi ++;
      	}
      goto bad;
    }

		/*end-chenming*/
			
            if (thinap) {
                /**********************************************************************
                * divide by subtype
                * one to capture
                * one to manage recv
                * csc
                *
                *
                * ********************************************************************/
                wh = (struct ieee80211_frame *)wbuf->data;
                switch ((wh->i_fc[0] & IEEE80211_FC0_SUBTYPE_MASK)
                            >> IEEE80211_FC0_SUBTYPE_SHIFT) {
                    case 4:
                    /*probe req*/
                        if (traffic_balance) {	/*xmeng add for thin ap traffic balance*/
                            /* Begin, Add by linrongqi, 2012-10-16. */
                            IEEE80211_ADDR_COPY(wh->i_addr3,vap->iv_myaddr);
                            /* End, Add by linrongqi, 2012-10-16. */
                        } else {
                            break;
                        }
                    case 0:
                    /*asso_req*/
                    case 2:
                    /*reassoc*/
                    case 10:
                    /*disassoc*/
                    case 11:
                    /*authentication*/
                    case 12:
                    /*deauthentication*/
					/*suzhaoyu add for sta leave report*/
#if AUTELAN_SOLUTION2
					if(!pass_lvframe)
					{
						int type = (wh->i_fc[0] & IEEE80211_FC0_SUBTYPE_MASK)
								>> IEEE80211_FC0_SUBTYPE_SHIFT;
						if(type == 10 || type == 12)
							break;
					}
#endif
					/*suzhaoyu add end*/
                    wbuf_capture = wbuf_copy(wbuf);
                    if (wbuf_capture == NULL) {
                        printk("capture malloc error\n");
                        goto bad;
                    }
/*<Begin : caizhibang modify for autelan 802.3 tunnel 2012-5-25*/						
            //        wbuf_push(wbuf_capture, sizeof(wlan_ng_prism2_header));
 /*End : caizhibang modify for autelan 802.3 tunnel 2012-5-25>*/	              
				    sendtowtpd = 3;	/*ljy--modified for sending management frames to wtpd before handle them*/
                    break;
                }

				/*Begin:Add by linrongqi for recv_mgmt before netif_rx 2012-11-7*/
				if(subtype == IEEE80211_FC0_SUBTYPE_ASSOC_REQ 
		        || subtype == IEEE80211_FC0_SUBTYPE_REASSOC_REQ 
		        || subtype == IEEE80211_FC0_SUBTYPE_AUTH ){
		           if(0 != ieee80211_recv_mgmt(ni, wbuf, subtype, rs))
		        	{
						wbuf_free(wbuf_capture);
						wbuf_capture=NULL;
					}
				}
           		/*End:Add by linrongqi for recv_mgmt before netif_rx 2012-11-7*/
        	

				
                /*ljy--add begin for sending management frames to wtpd before handle them*/
                if (sendtowtpd == 3 && wbuf_capture != NULL) {
/*<Begin : caizhibang modify for autelan 802.3 tunnel 2012-5-25*/						
                    //ph = (wlan_ng_prism2_header *)wbuf_capture->data;
                    //strncpy(ph->devname, dev->name, sizeof(ph->devname));
 /*End : caizhibang modify for autelan 802.3 tunnel 2012-5-25>*/	              
                    wbuf_capture->dev = dev;
                    //wbuf_capture->mac_header = wbuf_capture->data;
                    #if (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,22))
                    wbuf_capture->mac.raw = wbuf_capture->data;
                    #else
                    skb_reset_mac_header(wbuf_capture);
                    #endif
                    wbuf_capture->ip_summed = CHECKSUM_NONE;
                    wbuf_capture->pkt_type = PACKET_OTHERHOST;				
                    wbuf_capture->protocol = __constant_htons(0x0019); /* ETH_P_80211_RAW */
                    //ic->ic_lastdata = jiffies;	
                    //sheyanbin modify for receive data count,begin
                    /*sheyanbin modify for rxcount of packet droppped*/
                    ret = netif_rx(wbuf_capture);
                    if(NET_RX_DROP == ret)
                    {
                        //vap->iv_devstats.rx_dropped++;

                        goto bad;
                    }

                    /*sheyanbin -- add end*/
                }
                //sheyanbin end
				/*ljy--add end*/


				/*Begin:Add by linrongqi for recv_mgmt before netif_rx 2012-11-7*/
				if(subtype != IEEE80211_FC0_SUBTYPE_ASSOC_REQ 
           		&& subtype != IEEE80211_FC0_SUBTYPE_REASSOC_REQ 
           		&& subtype != IEEE80211_FC0_SUBTYPE_AUTH )
           		{       
                ieee80211_recv_mgmt(ni,wbuf,subtype,rs);
				//ic->ic_recv_mgmt(ni, skb, subtype, rs->rs_rssi, rs->rs_tstamp.tsf, auth_encrypted);
				}
				/*End:Add by linrongqi for recv_mgmt before netif_rx 2012-11-7*/

				
            }
            else
            {
                ieee80211_recv_mgmt(ni, wbuf, subtype, rs);
            }
        }
	/*zhaoyang1 transplant end*/
	
        /*
         * deliver the frame to the os. the handler cosumes the wbuf.
         */
        if (vap->iv_evtable) {
            vap->iv_evtable->wlan_receive(vap->iv_ifp, wbuf, type, subtype, rs);
        }
    } else if (type == IEEE80211_FC0_TYPE_CTL) {
        vap->iv_stats.is_rx_ctl++;
        ieee80211_recv_ctrl(ni, wbuf, subtype, rs);
        /*
         * deliver the frame to the os. the handler cosumes the wbuf.
         */
        if (vap->iv_evtable) {
            vap->iv_evtable->wlan_receive(vap->iv_ifp, wbuf, type, subtype, rs);
        }
    } else {
        goto bad;
    }

    (void) OS_ATOMIC_CMPXCHG(&vap->iv_rx_gate, 1, 0);
    return type;

bad:
    (void) OS_ATOMIC_CMPXCHG(&vap->iv_rx_gate, 1, 0);
bad1:
    wbuf_free(wbuf);
    return type; 
#undef HAS_SEQ
#undef QOS_NULL
}

#ifdef IEEE80211_DWDS
/*
 * Handle WDS discovery on receipt of a 4-address frame in
 * ap mode.  Queue the frame and post an event for someone
 * to plumb the necessary WDS vap for this station.  Frames
 * received prior to the vap set running will then be reprocessed
 * as if they were just received.
 */
static void
ieee80211_wds_discover(struct ieee80211_node *ni, wbuf_t wbuf)
{
#ifdef IEEE80211_DEBUG
    struct ieee80211vap *vap = ni->ni_vap;
#endif
    struct ieee80211com *ic = ni->ni_ic;
    int qlen, age;

    IEEE80211_NODE_WDSQ_LOCK(ni);
    if (_IF_QFULL(&ni->ni_wdsq)) {
        _IF_DROP(&ni->ni_wdsq);
        IEEE80211_NODE_WDSQ_UNLOCK(ni);
        IEEE80211_DISCARD(vap, IEEE80211_MSG_INPUT,
                          (struct ieee80211_frame *)wbuf_header(wbuf), "wds data",
                          "wds pending q overflow, drops %d (size %d)",
                          ni->ni_wdsq.ifq_drops, IEEE80211_PS_MAX_QUEUE);
#ifdef IEEE80211_DEBUG
        if (ieee80211_msg_dumppkts(vap))
            ieee80211_dump_pkt(ic, (u_int8_t*)wbuf_header(wbuf), wbuf_get_pktlen(wbuf), -1, -1);
#endif
        wbuf_free(wbuf);
        return;
    }
    /*
     * Tag the frame with it's expiry time and insert
     * it in the queue.  The aging interval is 4 times
     * the listen interval specified by the station. 
     * Frames that sit around too long are reclaimed
     * using this information.
     */
    /* XXX handle overflow? */
    /* XXX per/vap beacon interval? */
    age = ((ni->ni_intval * ic->ic_lintval) << 2) / 1024; /* TU -> secs */
    _IEEE80211_NODE_WDSQ_ENQUEUE(ni, wbuf, qlen, age);
    IEEE80211_NODE_WDSQ_UNLOCK(ni);

    IEEE80211_NOTE(vap, IEEE80211_MSG_WDS, ni,
                   "save frame, %u now queued", qlen);

    ieee80211_notify_wds_discover(ni);
}
#endif

static wbuf_t
ieee80211_decap(struct ieee80211vap *vap, wbuf_t wbuf, size_t hdrspace, struct ieee80211_rx_status *rs)
{
    struct ieee80211_qosframe_addr4 wh;    /* max size address frame */
    struct ether_header *eh;
    struct llc *llc;
    u_int16_t ether_type = 0;

    if (wbuf_get_pktlen(wbuf) < (hdrspace + sizeof(*llc))) {
		/*zhaoyang1 transplant from 717*/
		/*zhaoyang modify for error frame count CHINAMOBILE-105*/
		vap->iv_stats.is_rx_tooshort++;
		/*zhaoyang modify end*/
		/*zhaoyang1 transplant end*/
        /* XXX stat, msg */
        wbuf_free(wbuf);
        wbuf = NULL;
        goto done;
    }
    OS_MEMCPY(&wh, wbuf_header(wbuf), hdrspace);
    llc = (struct llc *)(wbuf_header(wbuf) + hdrspace);

    if (IS_SNAP(llc) && RFC1042_SNAP_NOT_AARP_IPX(llc)) {
       /* leave ether_tyep in  in network order */
        ether_type = llc->llc_un.type_snap.ether_type;
        wbuf_pull(wbuf, (u_int16_t) (hdrspace + sizeof(struct llc) - sizeof(*eh)));
        llc = NULL;
    } else if (IS_SNAP(llc) && IS_BTEP(llc)) {
        /* for bridge-tunnel encap, remove snap and 802.11 headers, keep llc ptr for type */
        wbuf_pull(wbuf,
                  (u_int16_t) (hdrspace + sizeof(struct llc) - sizeof(*eh)));
    } else {
        wbuf_pull(wbuf, (u_int16_t) (hdrspace - sizeof(*eh)));
    }
    eh = (struct ether_header *)(wbuf_header(wbuf));

    ieee80211_rptplacement_input(vap, wbuf, eh);
    ieee80211_smartantenna_input(vap, wbuf, eh, rs);

    switch (wh.i_fc[1] & IEEE80211_FC1_DIR_MASK) {
    case IEEE80211_FC1_DIR_NODS:
        /* If not from BSS then its from TDLS node to WDS */
        if (IEEE80211_TDLS_ENABLED(vap) &&
            !IEEE80211_ADDR_EQ(vap->iv_bss->ni_macaddr, wh.i_addr3)) {
            IEEE80211_ADDR_COPY(eh->ether_dhost, wh.i_addr3);
        }
        else  {
            IEEE80211_ADDR_COPY(eh->ether_dhost, wh.i_addr1);
        }
        IEEE80211_ADDR_COPY(eh->ether_shost, wh.i_addr2);
        break;
    case IEEE80211_FC1_DIR_TODS:
        IEEE80211_ADDR_COPY(eh->ether_dhost, wh.i_addr3);
        IEEE80211_ADDR_COPY(eh->ether_shost, wh.i_addr2);
        break;
    case IEEE80211_FC1_DIR_FROMDS:
        IEEE80211_ADDR_COPY(eh->ether_dhost, wh.i_addr1);
        IEEE80211_ADDR_COPY(eh->ether_shost, wh.i_addr3);
        break;
    case IEEE80211_FC1_DIR_DSTODS:
        IEEE80211_ADDR_COPY(eh->ether_dhost, wh.i_addr3);
        IEEE80211_ADDR_COPY(eh->ether_shost, wh.i_addr4);
        break;
    } 
#ifdef ATH_EXT_AP
    if ((vap->iv_opmode == IEEE80211_M_STA) &&
        IEEE80211_VAP_IS_EXT_AP_ENABLED(vap)) {
        if (ieee80211_extap_input(vap, eh)) {
            dev_kfree_skb(wbuf);
            return NULL;
        }
    } else {
#ifdef EXTAP_DEBUG
        extern char *arps[];
        eth_arphdr_t *arp = (eth_arphdr_t *)(eh + 1);
        if (eh->ether_type == ETHERTYPE_ARP) {
            printk("InP %s\t" eaistr "\t" eamstr "\t" eaistr "\t" eamstr "\n"
                   "s: " eamstr "\td: " eamstr "\n",
                arps[arp->ar_op],
                eaip(arp->ar_sip), eamac(arp->ar_sha),
                eaip(arp->ar_tip), eamac(arp->ar_tha),
                eamac(eh->ether_shost), eamac(eh->ether_dhost));
        }
#endif
    }
#endif /* ATH_EXT_AP */

    if (llc != NULL) {
        if (IS_BTEP(llc)) {
            /* leave ether_tyep in  in network order */
            eh->ether_type = llc->llc_snap.ether_type; 
        } else {
            eh->ether_type = htons(wbuf_get_pktlen(wbuf) - sizeof(*eh));
        }
    }
    else {
        eh->ether_type = ether_type;
    }
done:
    return wbuf;
}

/* 
 * delivers the data to the OS .
 *  will deliver standard 802.11 frames (with qos control removed)
 *  if IEEE80211_DELIVER_80211 param is set. 
 *  will deliver ethernet frames (with 802.11 header decapped)
 *  if IEEE80211_DELIVER_80211 param is not set. 
 *  this funcction consumes the  passed in wbuf.
 */
static void 
ieee80211_deliver_data(struct ieee80211vap *vap, wbuf_t wbuf, struct ieee80211_node *ni, struct ieee80211_rx_status *rs,
                       u_int32_t hdrspace, int is_mcast, u_int8_t subtype) 
{
    if (!IEEE80211_VAP_IS_DELIVER_80211_ENABLED(vap)) {
        /*
         * if the OS is interested in ethernet frame,
         * decap the 802.11 frame and convert into 
         * ethernet frame.
         */
        wbuf = ieee80211_decap(vap, wbuf, hdrspace, rs);
        if (!wbuf) {
         IEEE80211_DPRINTF(vap,
                          IEEE80211_MSG_INPUT,
                          "decap %s",
                          "failed");
            return;
        }

        /*
         * If IQUE is not enabled, the ops table is NULL and the following
         * steps will be skipped;
         * If IQUE is enabled, the packet will be checked to see whether it
         * is an IGMP packet or not, and update the mcast snoop table if necessary
         */
        if (vap->iv_ique_ops.me_inspect) {
            vap->iv_ique_ops.me_inspect(vap, ni, wbuf);
        }
    }


    if (IEEE80211_ENAB_AOW(ic)) {
        ieee80211_audio_receive(vap, wbuf, rs);
    }
    
    /* perform as a bridge within the AP */
    if ((vap->iv_opmode == IEEE80211_M_HOSTAP) &&
        !IEEE80211_VAP_IS_NOBRIDGE_ENABLED(vap)) {
        wbuf_t wbuf_cpy = NULL;

		/*Begin:Added by zhaoyang1 for optimizing vlan setting based on user 2013-01-22*/
		#if 0
		/*zhaoyang1 transplant from 717*/
		/*pengruofeng--add begin to develop the policy of mac binding vlan_bind*/
		if ((ni->ni_vid > 0) && (ni->ni_vid <= VLAN_VID_MASK))
		{
			wbuf = vlan_put_tag(wbuf, ni->ni_vid);
			if (!wbuf)
				return;
		}
		/*pengruofeng--add end*/
		/*zhaoyang1 transplant end*/
		#endif
		/*End:Added by zhaoyang1 for optimizing vlan setting based on user 2013-01-22*/
		

        if (is_mcast) {
			/*zhaoyang1 transplant from 717*/
            if ((vap->iv_switch & VAP_MULTICAST_ISOLATION) == 0) {	/*Added by duanmingzhe check method for user isolation*/
			/*zhaoyang1 transplant end*/
                wbuf_cpy = wbuf_clone(vap->iv_ic->ic_osdev, wbuf);
#if ATH_RXBUF_RECYCLE
			wbuf_set_cloned(wbuf_cpy);
#endif
            }
        } else {
       		/*zhaoyang1 transplant from 717*/
            if ((vap->iv_switch & VAP_UNICAST_ISOLATION) == 0) {	/*Added by duanmingzhe check method for user isolation*/

	            struct ieee80211_node *ni1;
	            /*
	             * Check if destination is associated with the
	             * same vap and authorized to receive traffic.
	             * Beware of traffic destined for the vap itself;
	             * sending it will not work; just let it be
	             * delivered normally.
	             */
	            if (IEEE80211_VAP_IS_DELIVER_80211_ENABLED(vap)) {
	                struct ieee80211_frame *wh = (struct ieee80211_frame *) wbuf_header(wbuf);
	                ni1 = ieee80211_find_node(&vap->iv_ic->ic_sta, wh->i_addr3);
	            } else {
	                struct ether_header *eh= (struct ether_header *) wbuf_header(wbuf);
	                ni1 = ieee80211_find_node(&vap->iv_ic->ic_sta, eh->ether_dhost);
	                if (ni1 == NULL) {
	                   ni1 = ieee80211_find_wds_node(&vap->iv_ic->ic_sta, eh->ether_dhost);
	                }
	            }
	            if (ni1 != NULL) {
	                if (ni1->ni_vap == vap &&
	                    ieee80211_node_is_authorized(ni1) &&
	                    ni1 != vap->iv_bss) {
	                    wbuf_cpy = wbuf;
	                    wbuf = NULL;
	                }
	                ieee80211_free_node(ni1);
	            }
            }
			/*zhaoyang1 transplant end*/
        }
        if (wbuf_cpy != NULL) {
            /*
             * send the frame copy back to the interface.
             * this frame is either multicast frame. or unicast frame
             * to one of the stations.
             */
            vap->iv_evtable->wlan_vap_xmit_queue(vap->iv_ifp, wbuf_cpy);
        }
    }
    if (wbuf != NULL) {
#ifdef IEEE80211_WDS
        if (is_mcast) {
            IEEE80211_DPRINTF(vap, IEEE80211_MSG_WDS,
                "%s: RX MCAST VAP from SA %s\n", __func__, 
                ether_sprintf(eh->ether_shost));

            if (vap->iv_opmode == IEEE80211_M_HOSTAP)
            {
                ieee80211_internal_send_dwds_multicast(vap, m);
            }
        }
#endif
        ieee80211_vap_pause_update_recv_stats(vap,wbuf); /* update the stats for vap pause module */

#if UMAC_SUPPORT_TDLS
        /* deliver the data frame to TDLS handler, don't cosume wbuf as os_handler will do */
        vap->iv_ic->tdls_recv_mgmt(vap->iv_ic, ni, wbuf);
#endif /* UMAC_SUPPORT_TDLS */

#if UMAC_SUPPORT_VI_DBG
        ieee80211_vi_dbg_input(vap, wbuf);
#endif
        /*
         * deliver the data frame to the os. the handler cosumes the wbuf.
         */
        vap->iv_evtable->wlan_receive(vap->iv_ifp, wbuf, IEEE80211_FC0_TYPE_DATA, subtype, rs);
    }
}

struct ieee80211_iter_input_all_arg {
    wbuf_t wbuf;
    struct ieee80211_rx_status *rs;
    int type;
};

static INLINE void
ieee80211_iter_input_all(void *arg, struct ieee80211vap *vap, bool is_last_vap)
{
    struct ieee80211_iter_input_all_arg *params = (struct ieee80211_iter_input_all_arg *) arg; 
    struct ieee80211_node *ni;
    wbuf_t wbuf1;

    if (!ieee80211_vap_active_is_set(vap) && !vap->iv_input_mgmt_filter ) {
        return;
    }

    if (!is_last_vap) {
        wbuf1 = wbuf_clone(vap->iv_ic->ic_osdev, params->wbuf);
        if (wbuf1 == NULL) {
            /* XXX stat+msg */
            return;
        }
    } else {
        wbuf1 = params->wbuf;
        params->wbuf = NULL;
    }

    ni = vap->iv_bss;
    params->type = ieee80211_input(ni, wbuf1, params->rs);
}

int
ieee80211_input_all(struct ieee80211com *ic,
    wbuf_t wbuf, struct ieee80211_rx_status *rs)
{
    struct ieee80211_iter_input_all_arg params; 
    u_int32_t num_vaps;

    params.wbuf = wbuf;
    params.rs = rs;
    params.type = -1;

    ieee80211_iterate_vap_list_internal(ic,ieee80211_iter_input_all,(void *)&params,num_vaps);
    if (params.wbuf != NULL)        /* no vaps, reclaim wbuf */
        wbuf_free(params.wbuf);
    return params.type;
}

static INLINE void
ieee80211_input_monitor_iter_func(void *arg, struct ieee80211vap *vap, bool is_last_vap)
{
    struct ieee80211_iter_input_all_arg *params = (struct ieee80211_iter_input_all_arg *) arg;

    if (!params->wbuf)
        return;

    /*
     * deliver the frame to the os.
     */
    if (vap->iv_opmode == IEEE80211_M_MONITOR && ieee80211_vap_ready_is_set(vap)) {
        /* remove padding from header */
        u_int8_t *header = (u_int8_t *)wbuf_header(params->wbuf);
        u_int32_t hdrspace = ieee80211_anyhdrspace(vap->iv_ic, header);
        u_int32_t realhdrsize = ieee80211_anyhdrsize(header); /* no _F_DATAPAD */
        u_int32_t padsize = hdrspace - realhdrsize;

        if (padsize > 0) {
            memmove(header+padsize, header, realhdrsize);
            wbuf_pull(params->wbuf, padsize);
        }

        vap->iv_evtable->wlan_receive_monitor_80211(vap->iv_ifp, params->wbuf, params->rs);

        /* For now, only allow one vap to be monitoring */
        params->wbuf = NULL;
    }
}

/*
 * this should be called only if there exists a monitor mode vap.
 */
int
ieee80211_input_monitor(struct ieee80211com *ic, wbuf_t wbuf, struct ieee80211_rx_status *rs)
{
    u_int32_t num_vaps;
    struct ieee80211_iter_input_all_arg params;

    params.wbuf = wbuf;
    params.rs = rs;
    params.type = -1;

    ieee80211_iterate_vap_list_internal(ic,ieee80211_input_monitor_iter_func,(void *)&params,num_vaps);
    if (params.wbuf)
        wbuf_free(params.wbuf);
    return 0;
}

int ieee80211_vap_txrx_unregister_event_handler(ieee80211_vap_t vap,ieee80211_vap_txrx_event_handler evhandler, void *arg)
{
    int i,j;
    ieee80211_txrx_event_info *info = &vap->iv_txrx_event_info;
    IEEE80211_VAP_LOCK(vap);
    for (i=0;i<IEEE80211_MAX_VAP_TXRX_EVENT_HANDLERS; ++i) {
        if ( info->iv_txrx_event_handler[i] == evhandler  && info->iv_txrx_event_handler_arg[i] == arg) {
            info->iv_txrx_event_handler[i] = NULL; 
            info->iv_txrx_event_handler_arg[i] = NULL;
            /* recompute event filters */
            info->iv_txrx_event_filter = 0;
            for (j=0;j<IEEE80211_MAX_VAP_TXRX_EVENT_HANDLERS; ++j) {
              if ( info->iv_txrx_event_handler[j]) {
                info->iv_txrx_event_filter |= info->iv_txrx_event_filters[j] ;
              }
            }
            IEEE80211_VAP_UNLOCK(vap);
            return EOK;
        }
    }
    IEEE80211_VAP_UNLOCK(vap);
    return -EEXIST;
}


int ieee80211_vap_txrx_register_event_handler(ieee80211_vap_t vap,ieee80211_vap_txrx_event_handler evhandler, void *arg, u_int32_t event_filter)
{
    int i;
    ieee80211_txrx_event_info *info = &vap->iv_txrx_event_info;
    ieee80211_vap_txrx_unregister_event_handler(vap,evhandler,arg);
    IEEE80211_VAP_LOCK(vap);
    for (i=0;i<IEEE80211_MAX_VAP_TXRX_EVENT_HANDLERS; ++i) {
        if ( info->iv_txrx_event_handler[i] == NULL) {
            info->iv_txrx_event_handler[i] = evhandler; 
            info->iv_txrx_event_handler_arg[i] = arg;
            info->iv_txrx_event_filters[i] = event_filter;
            info->iv_txrx_event_filter |= event_filter;
            IEEE80211_VAP_UNLOCK(vap);
            return EOK;
        }
    }
    IEEE80211_VAP_UNLOCK(vap);
    return -ENOMEM;
}

void ieee80211_vap_txrx_deliver_event(ieee80211_vap_t vap,ieee80211_vap_txrx_event *evt)
{
    ieee80211_txrx_event_info *info = &vap->iv_txrx_event_info;
    int i;

    IEEE80211_VAP_LOCK(vap);
    for(i=0;i<IEEE80211_MAX_VAP_TXRX_EVENT_HANDLERS; ++i) {   
        if (info->iv_txrx_event_handler[i] && (info->iv_txrx_event_filters[i] & evt->type) ) { 
            ieee80211_vap_txrx_event_handler evhandler = info->iv_txrx_event_handler[i]; 
            void *arg = info->iv_txrx_event_handler_arg[i]; 
        
            IEEE80211_VAP_UNLOCK(vap);
            (* evhandler)                                      
                (vap, evt, arg);                   
            IEEE80211_VAP_LOCK(vap);
        }                                                                              
    }                                                                                   
    IEEE80211_VAP_UNLOCK(vap);
}

#if ATH_SUPPORT_IWSPY
int ieee80211_input_iwspy_update_rssi(struct ieee80211com *ic, u_int8_t *address, int8_t rssi)
{
	struct ieee80211vap *vap = TAILQ_FIRST(&ic->ic_vaps);
	
	if (vap)
	{
		vap->iv_evtable->wlan_iwspy_update(vap->iv_ifp, address, rssi);
	}
		
	return 0;
}
#endif

