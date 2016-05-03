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
#include <net/sock.h>
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
#include "ap8xLnx_scan.h"

extern u_int32_t thinap;
extern u_int32_t traffic_balance;
extern UINT32 debug_level;

/** local definitions **/
struct ieee80211_frame
{
    IEEEtypes_FrameCtl_t FrmCtl;
    UINT8   dur[2];
    UINT8   addr1[IEEEtypes_ADDRESS_SIZE];
    UINT8   addr2[IEEEtypes_ADDRESS_SIZE];
    UINT8   addr3[IEEEtypes_ADDRESS_SIZE];
    UINT8   seq[2];
    UINT8   addr4[IEEEtypes_ADDRESS_SIZE];
} PACK;

struct llc
{
    UINT8 llc_dsap;
    UINT8 llc_ssap;
    union
    {
        struct
        {
            UINT8 control;
            UINT8 format_id;
            UINT8 class;
            UINT8 window_x2;
        } PACK type_u;
        struct
        {
            UINT8 num_snd_x2;
            UINT8 num_rcv_x2;
        } PACK type_i;
        struct
        {
            UINT8 control;
            UINT8 num_rcv_x2;
        } PACK type_s;
        struct
        {
            UINT8 control;
            /*
            * We cannot put the following fields in a structure because
            * the structure rounding might cause padding.
            */
            UINT8 frmr_rej_pdu0;
            UINT8 frmr_rej_pdu1;
            UINT8 frmr_control;
            UINT8 frmr_control_ext;
            UINT8 frmr_cause;
        } PACK type_frmr;
        struct
        {
            UINT8  control;
            UINT8  org_code[3];
            UINT16 ether_type;
        } PACK type_snap;
        struct
        {
            UINT8 control;
            UINT8 control_ext;
        } PACK type_raw;
    } llc_un /* XXX PACK ??? */;
};

/******************** zhouke add ,splitmac ***************/

#define LLC_SNAP_LSAP   0xaa
#define LLC_UI      0x03
#define llc_control     llc_un.type_u.control
#define llc_control_ext     llc_un.type_raw.control_ext
#define llc_fid         llc_un.type_u.format_id
#define llc_class       llc_un.type_u.class
#define llc_window      llc_un.type_u.window_x2
#define llc_frmrinfo        llc_un.type_frmr.frmr_rej_pdu0
#define llc_frmr_pdu0       llc_un.type_frmr.frmr_rej_pdu0
#define llc_frmr_pdu1       llc_un.type_frmr.frmr_rej_pdu1
#define llc_frmr_control    llc_un.type_frmr.frmr_control
#define llc_frmr_control_ext    llc_un.type_frmr.frmr_control_ext
#define llc_frmr_cause      llc_un.type_frmr.frmr_cause
#define llc_snap        llc_un.type_snap

#define RPTR_ETHERTYPE  0x0003

#define IEEE80211_ADDR_COPY(dst,src)    memcpy(dst,src,IEEEtypes_ADDRESS_SIZE)

#define IEEE80211_IS_BROADCAST(_a)              \
    ((_a)[0] == 0xff &&                         \
     (_a)[1] == 0xff &&                         \
     (_a)[2] == 0xff &&                         \
     (_a)[3] == 0xff &&                         \
     (_a)[4] == 0xff &&                         \
     (_a)[5] == 0xff)


#ifndef ALIGNED_POINTER
#define ALIGNED_POINTER(p,t)    1
#endif

/************************ end **************************/

//add by lirui for pppoe through transmission begin
struct  ether_header {
    UINT8   ether_dhost[IEEEtypes_ADDRESS_SIZE];
    UINT8   ether_shost[IEEEtypes_ADDRESS_SIZE];
    UINT16  ether_type;
};
//add by lirui for pppoe through transmission end

    /******************** zhouke add ,splitmac ***************/

struct llc_rptr {
    struct llc llc;
    struct ether_header eh;
} PACK;

    /************************ end **************************/

/*
 * Return the size of the 802.11 header for a management or data frame.
 */
INLINE static int ieee80211_hdrsize(const void *data)
{
    const struct ieee80211_frame *wh = (const struct ieee80211_frame *)data;
    int size = sizeof(struct ieee80211_frame);

    if (NULL == wh)
        return 0;
    
    if (!((wh->FrmCtl.ToDs==1)&&(wh->FrmCtl.FromDs==1)))
        size -= IEEE80211_ADDR_LEN;
    
    if (IEEE80211_QOS_HAS_SEQ(wh)){
        size += sizeof(u_int16_t);
    }
    return size;
}

/*
 * Return the space occupied by the 802.11 header and any
 * padding required by the driver.  This works for a
 * management or data frame.
 */
int ieee80211_hdrspace(const void *data)
{
    int size = ieee80211_hdrsize(data);

#if 1
    //if (ic->ic_flags & IEEE80211_F_DATAPAD)
        size = roundup(size, sizeof(u_int32_t));
#endif

    return size;
}


int dhcp_detect_wh(struct ieee80211vap *vap, struct sk_buff *skb)
{
    if (NULL == vap || NULL == skb)
    {
        return 0;
    }
    
    if (vap->vap_dhcp_enable) 
    {
        u_int8_t *wh = skb->data;
        int ip_len = sizeof(struct iphdr);
        int hdrspace = sizeof(struct ieee80211_frame);
        if ((skb->len > (hdrspace + LLC_SNAPFRAMELEN + ip_len)) &&
            ((*(wh + hdrspace + LLC_SNAPFRAMELEN + ip_len + 1) == 67 && *(wh + hdrspace + LLC_SNAPFRAMELEN + ip_len + 3) == 68) ||
            (*(wh + hdrspace + LLC_SNAPFRAMELEN + ip_len + 1) == 68 && *(wh + hdrspace + LLC_SNAPFRAMELEN + ip_len + 3) == 67)))
        {
            WlLogPrint(MARVEL_DEBUG_ALL, __func__, "DHCP...\n");
            return 1;
        }
        else
        {
            return 0;
        }
    }
    else
    {
        return 0;
    }
}
//modify by lirui for dhcp through transmission end


int pppoe_detect_wh(struct ieee80211vap *vap, struct sk_buff *skb)
{
    if (NULL == vap || NULL == skb)
    {
        return 0;
    }
    
    if (vap->vap_pppoe_enable)
    {
        struct ieee80211_frame *wh = (struct ieee80211_frame *)skb->data;

        if (wh->FrmCtl.Type == IEEE_TYPE_DATA)
        {
            int hdrspace = sizeof(struct ieee80211_frame);
            struct llc *llc_type = (struct llc *)(skb->data + hdrspace);

            if ((llc_type->llc_un.type_snap.ether_type == __constant_htons(ETH_P_PPP_DISC)) ||
                (llc_type->llc_un.type_snap.ether_type == __constant_htons(ETH_P_PPP_SES)))
            {
                return 1;
            }
        }
    }

    return 0;
}


#ifdef AE5200_RT_SUPPORT

int dhcp_detect_wh1(struct ieee80211vap *vap,struct sk_buff *skb)
{
    u_int8_t *wh = skb->data;
    int hdrspace = sizeof(struct ieee80211_frame);
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
    int hdrspace = sizeof(struct ieee80211_frame);
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
    int hdrspace = sizeof(struct ieee80211_frame);

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


UINT8 Build_Autelan_Report_EXIE(IEEEtypes_Extend_BAND_RSSI_Element_t *Element,vmacApInfo_t * vmactem_p,UINT32 RSSI)
{
    MIB_802DOT11 *mib;
    MIB_PHY_DSSS_TABLE *PhyDSSSTable;
    UINT8 oui[3] = { AUTE_OUI_BYTES };
    
    if((Element == NULL) || (vmactem_p == NULL))
    {
        return 1;
    }

    mib = vmactem_p->Mib802dot11;
    PhyDSSSTable=mib->PhyDSSSTable;
    
    memset(Element,0,sizeof(IEEEtypes_Extend_BAND_RSSI_Element_t));

    Element->ElementId = IEEE80211_ELEMID_VENDOR;
    Element->Len = sizeof(IEEEtypes_Extend_BAND_RSSI_Element_t) - 2;
    memcpy(Element->OUI,oui,sizeof(oui));
    Element->OUIType = BAND_OUI_TYPE;
    Element->TypeLen = 1;
    Element->TypeValue = (PhyDSSSTable->CurrChan<=14)?0:1;
    Element->OUISubType = RSSI_OUI_SUBTYPE;
    Element->SubTypeLen = 1;
    Element->SubTypeValue = RSSI;

	return 0;
}


UINT8 ieee80211_autelan_get_4_address_packet_eapol_type_analysis(struct sk_buff *skb)
{
    UINT8 eapol_type = 0;
    struct llc *llc;
    struct llc_rptr *llc_rptr = NULL;

    llc = (struct llc *)&(skb->data[sizeof(struct ieee80211_frame)]);

    if (skb->len >= sizeof(struct llc) &&
    llc->llc_dsap == LLC_SNAP_LSAP && llc->llc_ssap == LLC_SNAP_LSAP &&
    llc->llc_control == LLC_UI && 
    ((llc->llc_snap.org_code[0] == 0x00 && llc->llc_snap.org_code[1] == 0x40 && llc->llc_snap.org_code[2] == 0x96) ||
    (llc->llc_snap.org_code[0] == 0x00 && llc->llc_snap.org_code[1] == 0x00 && llc->llc_snap.org_code[2] == 0x00)))
    {

        if (ntohs(llc->llc_un.type_snap.ether_type) == RPTR_ETHERTYPE)
        {
            llc_rptr = (struct llc_rptr *)llc;

            eapol_type = skb->data[sizeof(struct ieee80211_frame) + sizeof(struct llc_rptr) + 1];
        } 
        else 
        {
            eapol_type = skb->data[sizeof(struct ieee80211_frame) + sizeof(struct llc) + 1];
        }
    }

    return eapol_type;
}

#endif

#if 0
/***************** zhouke add ,capwap_split_fast ***************/
struct sk_buff * ieee80211_autelan_recv_data_80211_splitmac(struct sk_buff *skb)
{
    int need_headroom;
    WlLogPrint(MARVEL_DEBUG_ALL, __func__," 802.11 splitmac");

    if(skb == NULL)
    {
        WlLogPrint(MARVEL_DEBUG_WARNING, __func__,"netdev is NULL,recv packet error\n");
        return NULL;
    }

    skb = skb_unshare(skb, GFP_ATOMIC);

    if (skb == NULL)
    {
        WlLogPrint(MARVEL_DEBUG_ERROR, __func__,"SKB unshare operation failed!\n");
    }

    need_headroom = 80;     // capwap_split_fast need 80 bytes;

    if (skb_headroom(skb) < need_headroom)
    {
        struct sk_buff *tmp;

        tmp = skb_realloc_headroom(skb, need_headroom);
        if (tmp == NULL)
        {
            WlLogPrint(MARVEL_DEBUG_ERROR, __func__,"SKB headroom not enough --- reallocate headroom!\n");
        }

        if ((skb)->sk)
            skb_set_owner_w(tmp, (skb)->sk);

        dev_kfree_skb_any(skb);        
        skb = tmp;
    }

    return skb;
}
#endif


/**** 80211 head Transform 8023 head ****/
struct sk_buff * ieee80211_autelan_recv_data_8023_splitmac(struct sk_buff *skb)
{

    struct ether_header *eh;
    struct ieee80211_frame wh;
    struct llc *llc;
    struct llc_rptr *llc_rptr = NULL;
    UINT16 ether_type = 0;
    UINT8 *BSSID = NULL;
    int need_headroom;

    if(skb == NULL)
    {
        WlLogPrint(MARVEL_DEBUG_WARNING, __func__,"netdev is NULL,recv packet error\n");
        return NULL;
    }

    WlLogPrint(MARVEL_DEBUG_ALL, __func__," 802.3 splitmac");

    memcpy(&wh, skb->data, sizeof(struct ieee80211_frame));

    llc = (struct llc *)skb_pull(skb, sizeof(struct ieee80211_frame) - IEEEtypes_ADDRESS_SIZE);
    
    if (skb->len >= sizeof(struct llc) &&
        llc->llc_dsap == LLC_SNAP_LSAP && 
        llc->llc_ssap == LLC_SNAP_LSAP &&
        llc->llc_control == LLC_UI && 
        ((llc->llc_snap.org_code[0] == 0x00 && llc->llc_snap.org_code[1] == 0x40 && llc->llc_snap.org_code[2] == 0x96) ||
         (llc->llc_snap.org_code[0] == 0x00 && llc->llc_snap.org_code[1] == 0x00 && llc->llc_snap.org_code[2] == 0x00)))
    {
        if (ntohs(llc->llc_un.type_snap.ether_type) == RPTR_ETHERTYPE) 
        {
            llc_rptr = (struct llc_rptr *)llc;
            ether_type = llc_rptr->eh.ether_type;
            skb_pull(skb,sizeof(struct llc_rptr));
            llc = NULL;
        } 
        else
        {
            ether_type = llc->llc_un.type_snap.ether_type;
            skb_pull(skb, sizeof(struct llc));
            llc = NULL;
        }
    }

    eh = (struct ether_header *)skb_push(skb,sizeof(struct ether_header));

    if(wh.FrmCtl.ToDs)
    {
        if (llc_rptr)
        {
            IEEE80211_ADDR_COPY(eh->ether_dhost, llc_rptr->eh.ether_dhost);
            IEEE80211_ADDR_COPY(eh->ether_shost, llc_rptr->eh.ether_shost);
            llc_rptr = NULL;
        } 
        else 
        {
            IEEE80211_ADDR_COPY(eh->ether_dhost, wh.addr3);
            IEEE80211_ADDR_COPY(eh->ether_shost, wh.addr2);
        }
    } 
    else
    {
        WlLogPrint(MARVEL_DEBUG_DEBUG, __func__,"FromDS = %i, ToDs = %i", wh.FrmCtl.FromDs, wh.FrmCtl.ToDs);
        dev_kfree_skb_any(skb);
        return NULL;
    }

    if (!ALIGNED_POINTER(skb->data + sizeof(*eh), u_int32_t))
    {
        struct sk_buff *n;

        n = skb_copy(skb, GFP_ATOMIC);
        n->protocol = skb->protocol;
        n->dev = skb->dev;
        dev_kfree_skb_any(skb);
        if (n == NULL)
            return NULL;
        skb = n;
        eh = (struct ether_header *) skb->data;
    }
    
    if (llc != NULL)
        eh->ether_type = htons(skb->len - sizeof(*eh));
    else
        eh->ether_type = ether_type;

    BSSID = (UINT8 *)skb_push(skb,IEEEtypes_ADDRESS_SIZE);

    IEEE80211_ADDR_COPY(BSSID, wh.addr1);

    skb = skb_unshare(skb, GFP_ATOMIC);

    if (skb == NULL)
    {
        WlLogPrint(MARVEL_DEBUG_ERROR, __func__,"SKB unshare operation failed!\n");
    }

    need_headroom = 80;     // capwap_split_fast need 80 bytes;

    if (skb_headroom(skb) < need_headroom)
    {
        struct sk_buff *tmp;

        tmp = skb_realloc_headroom(skb, need_headroom);
        if (tmp == NULL)
        {
            WlLogPrint(MARVEL_DEBUG_ERROR, __func__,"SKB headroom not enough --- reallocate headroom!\n");
        }

        if ((skb)->sk)
            skb_set_owner_w(tmp, (skb)->sk);

        dev_kfree_skb_any(skb);
        skb = tmp;
    }

    return skb;
}
/*********************** end **************************/

#ifdef AE5200_RT_SUPPORT
int ieee80211_autelan_recv_managent_frame_thinap_process(struct sk_buff *skb, struct net_device *dev, int subtype,vmacApInfo_t * vmactem_p,UINT32 RSSI)
#else
int ieee80211_autelan_recv_managent_frame_thinap_process(struct sk_buff *skb, struct net_device *dev, int subtype)
#endif
{
    struct sk_buff *wbuf_capture = NULL;
    UINT8 tmpbuf[NBR_BYTES_IEEE80211HEADER];
    int ret = AP8X_FATAP_MODE;

    if(skb == NULL || NULL == dev)
    {
        WlLogPrint(MARVEL_DEBUG_WARNING, __func__,"netdev is NULL");
        return AP8X_THINAP_MODE_NETIF_RX_DROP;
    }

    if (thinap) 
    {
        /**********************************************************************
        * divide by subtype
        * one to capture
        * one to manage recv
        * csc
        *
        *
        * ********************************************************************/
        switch (subtype) 
        {
            case IEEE_MSG_PROBE_RQST:
            {
                /*probe req*/
                if (traffic_balance) 
                {   
                    /*xmeng add for thin ap traffic balance*/
                    ;
                }
                else 
                {
                    break;
                }
            }
            case IEEE_MSG_ASSOCIATE_RQST:
                /*asso_req*/
            case IEEE_MSG_REASSOCIATE_RQST:
                /*reassoc*/
            case IEEE_MSG_DISASSOCIATE:
                /*disassoc*/
            case IEEE_MSG_AUTHENTICATE:
                /*authentication*/
            case IEEE_MSG_DEAUTHENTICATE:
            {
                /*deauthentication*/
                wbuf_capture = skb_copy(skb, GFP_ATOMIC);
                if (wbuf_capture == NULL) 
                {
                    WlLogPrint(MARVEL_DEBUG_ERROR, __func__,"capture malloc error\n");
                    ret = AP8X_THINAP_MODE_SKB_RELOCATE_FAIL;
                    break;
                }
                /* add by zhanxuechao for 4 address chenge to 3 address */
                memcpy(tmpbuf, wbuf_capture->data, NBR_BYTES_IEEE80211HEADER);
                skb_pull(wbuf_capture, 6);
                memcpy(wbuf_capture->data, tmpbuf, NBR_BYTES_IEEE80211HEADER);
                /*send management frame to wtpd*/

#ifdef AE5200_RT_SUPPORT
                if (traffic_balance) 
                {   
                    if(subtype == IEEE_MSG_PROBE_RQST)
                    {
                        struct ieee80211_frame *wh;
                        wh = (struct ieee80211_frame *)wbuf_capture->data;
                        if (IEEE80211_IS_BROADCAST(wh->addr1)){
                            IEEE80211_ADDR_COPY(wh->addr1,&vmactem_p->macStaAddr);
                        } 
                    }
                    
                    if((subtype == IEEE_MSG_PROBE_RQST)||(subtype == IEEE_MSG_ASSOCIATE_RQST)\
                       ||(subtype == IEEE_MSG_AUTHENTICATE))
                    {
                        if(skb_tailroom(wbuf_capture) > sizeof(IEEEtypes_Extend_BAND_RSSI_Element_t))
                        {
                            IEEEtypes_Extend_BAND_RSSI_Element_t *IE_p = NULL;
                            //We have enough room at the tail
                            IE_p = (IEEEtypes_Extend_BAND_RSSI_Element_t *)skb_put(wbuf_capture, sizeof(IEEEtypes_Extend_BAND_RSSI_Element_t));
                            if (IE_p != NULL)
                                Build_Autelan_Report_EXIE(IE_p,vmactem_p,RSSI);
                        }
                        else
                        {
                            WlLogPrint(MARVEL_DEBUG_ERROR, __func__,"not enough space for Autelan Report EXIE\n");
                        }   
                    }
                }
#endif
                
                wbuf_capture->dev = dev;
            #if (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,22))
                wbuf_capture->mac.raw = wbuf_capture->data;
            #else
                skb_reset_mac_header(wbuf_capture);
            #endif
                wbuf_capture->ip_summed = CHECKSUM_NONE;
                wbuf_capture->pkt_type = PACKET_OTHERHOST;              
                wbuf_capture->protocol = __constant_htons(0x0019); /* ETH_P_80211_RAW */
                ret = netif_rx(wbuf_capture);
                if (ret == NET_RX_DROP)
                {
                    if (wbuf_capture != NULL)
                    {
                        dev_kfree_skb_any(wbuf_capture);
                    }
                    ret = AP8X_THINAP_MODE_NETIF_RX_DROP;
                }
                break;
            }            
            default:
            {
                break;
            }
        }
    }

    return ret;
}

int ieee80211_autelan_recv_data_frame_thinap_process(struct sk_buff **skb, struct net_device *dev, extStaDb_StaInfo_t *pStaInfo, UINT16 frametype)
{
    struct wlprivate *wlpptr = NULL;
    int ret = AP8X_FATAP_MODE;
    int tunnel_local_state = -1; // 1 means tunnel,0 means local

    wlpptr = NETDEV_PRIV_P(struct wlprivate, dev);

    if(*skb == NULL || NULL == dev || NULL == pStaInfo)
    {
        WlLogPrint(MARVEL_DEBUG_WARNING, __func__,"netdev is NULL,recv packet error\n");
        return AP8X_THINAP_MODE_TUNNEL_DROP;
    }

    /**** printk recv packet ****/
    if(debug_level == MARVEL_DEBUG_RECV_PACKET)
    {
        CWCaptrue((*skb)->len,(*skb)->data);
    }
    
    if (thinap)
    {
        tunnel_local_state = wlpptr->vap.vap_splitmac;
        
#ifdef AE5200_RT_SUPPORT
		/*zhaoyang add for data forward base users*/
		if (wlpptr->vap.vap_splitmac == 3)  //ubc mode
		{       
            tunnel_local_state = ieee80211_autelan_recv_data_frame_ubc_mode(skb, dev, pStaInfo, frametype);

		}
#endif
        if(tunnel_local_state == 0)         /* LOCAL mode */
        {
            ret = ieee80211_autelan_recv_data_frame_local_mode(skb, dev, pStaInfo, frametype);
        }
        else if (tunnel_local_state == 1)   /* 80211 TUNNEL mode */
        {
            ret = ieee80211_autelan_recv_data_frame_80211_tunnel_mode(skb, dev, pStaInfo, frametype);
        }
        if (tunnel_local_state == 2)        /* 8023 TUNNEL mode */
        {
            ret = ieee80211_autelan_recv_data_frame_8023_tunnel_mode(skb, dev, pStaInfo, frametype);
        }
    }
    else
    {
        WlLogPrint(MARVEL_DEBUG_ALL, __func__, "FAT AP Mode:Receive Data dev name : %s frametype: %02x\n", dev->name, frametype);
    }
    
    return ret;
}

int ieee80211_autelan_send_data_frame_thinap_process(struct sk_buff *skb, vmacApInfo_t *vmacSta_p, UINT32 tunnelmode, BOOLEAN *eapolPkt, int *type,extStaDb_StaInfo_t **pStaInfo_tmp)
{
    int ret = AP8X_FATAP_MODE;

    if(skb == NULL || NULL == vmacSta_p)
    {
        WlLogPrint(MARVEL_DEBUG_WARNING, __func__,"netdev is NULL,send packet error\n");
        return AP8X_THINAP_MODE_ERR;
    }
    
    if (thinap) 
    {        

        /**** printk send packet ****/
        if(debug_level == MARVEL_DEBUG_SEND_PACKET)
        {
            CWCaptrue(skb->len,skb->data);
        }
    
        if (skb->protocol == htons(0x0019))
        {
            ret = ieee80211_autelan_send_data_frame_local_mode(skb,vmacSta_p,eapolPkt,type,pStaInfo_tmp);
        }
        else if(skb->protocol == htons(0x0030))     // 8023 packet goto fatap mode;
        {
            WlLogPrint(MARVEL_DEBUG_ALL, __func__,"packet protocol = 0x0030"); 
            *type = IEEE_TYPE_DATA;
            return AP8X_THINAP_MODE_TUNNEL_SUCC;
        }
        else if (tunnelmode == 1 || tunnelmode == 2)
        {
#ifdef AE5200_RT_SUPPORT    //hostapd send 4-way handshake
            struct ether_header *pEth;

            pEth = (struct ether_header *) skb->data;

            if(pEth->ether_type == IEEE_ETHERTYPE_PAE 
                || pEth->ether_type == IEEE_ETHERTYPE_PRE_PAE)
            {
                WlLogPrint(MARVEL_DEBUG_ALL, __func__,"AE5200_RT TUNNEL MODE Send ether_type:%02x Packet to STA\n",pEth->ether_type);
                ret = AP8X_FATAP_MODE;
                *type = IEEE_TYPE_DATA;
                return ret;
            }
            else
#endif
                ret = AP8X_THINAP_MODE_ERR;
        }
#ifdef AE5200_RT_SUPPORT
        else if (tunnelmode == 3)   //ubc mode
        {
            WlLogPrint(MARVEL_DEBUG_ALL, __func__,"AE5200_RT UBC MODE Send Packet to STA\n");
            *type = IEEE_TYPE_DATA;
            ret = AP8X_FATAP_MODE;
            return ret;
        }
#endif
        else if(*pStaInfo_tmp != NULL)  // thinap mode,send packet to sta;
        {

#ifdef AE5200_RT_SUPPORT    //hostapd send 4-way handshake

            struct ether_header *pEth;

            pEth = (struct ether_header *) skb->data;

            if(pEth->ether_type == IEEE_ETHERTYPE_PAE 
                || pEth->ether_type == IEEE_ETHERTYPE_PRE_PAE)
            {
                WlLogPrint(MARVEL_DEBUG_ALL, __func__,"AE5200_RT LOCAL MODE Send ether_type:%02x Packet to STA,len = %d\n",\
                 pEth->ether_type,skb->len);
                ret = AP8X_FATAP_MODE;
                *type = IEEE_TYPE_DATA;
                return ret;
            }
            else
#endif
            if(ieee80211_node_is_authorize(*pStaInfo_tmp) != IEEE80211_NODE_AUTHORIZE_SUCC)
            {
                WlLogPrint(MARVEL_DEBUG_WARNING, __func__, "Send Packet to sta,But Sta is not authorized\n");
                ret = AP8X_THINAP_MODE_ERR;
            }
        }
    }

    return ret;
}


int ieee80211_autelan_recv_data_frame_filter_process(struct sk_buff *skb, struct net_device *dev, extStaDb_StaInfo_t *pStaInfo)
{
    struct wlprivate *priv = NULL;
    vmacApInfo_t *vmacSta_p = NULL;
    MIB_802DOT11 *mib = NULL;
    struct ieee80211_frame *wh;

#ifdef CLIENT_SUPPORT
    vmacEntry_t  *VMacEntry_p;
    UINT8 AssociatedFlag = 0;
    UINT8 bssId[6];    
#endif

    if (NULL == skb || NULL == dev || NULL == pStaInfo)
    {
        WlLogPrint(MARVEL_DEBUG_WARNING, __func__,"netdev is NULL,recv packet error\n");
        return AP8X_80211_DATA_DROP;
    }

    priv = NETDEV_PRIV_P(struct wlprivate, dev);
    vmacSta_p = priv->vmacSta_p;
    mib = vmacSta_p->Mib802dot11;
    
    if(skb->len < sizeof(struct ieee80211_frame))
    {
        WlLogPrint(MARVEL_DEBUG_WARNING, __func__,"packet len < %d(min_len),recv packet error\n",sizeof(struct ieee80211_frame));
        return AP8X_80211_DATA_DROP;
    }

    wh = (struct ieee80211_frame *)skb->data;
    
#ifdef CLIENT_SUPPORT

    if (skb->protocol & WL_WLAN_TYPE_STA)
    {
        if ((VMacEntry_p = sme_GetParentVMacEntry(vmacSta_p->VMacEntry.phyHwMacIndx)) == NULL)
        {
            return AP8X_80211_DATA_DROP;
        }
        smeGetStaLinkInfo(VMacEntry_p->id,
            &AssociatedFlag,
            &bssId[0]);

        if(!AssociatedFlag || memcmp(bssId, wh->addr2, sizeof(IEEEtypes_MacAddr_t)))
        {
            return AP8X_80211_DATA_DROP;
        }
        /* Check to see if broadcast packet from AP. */ 
        if (IS_GROUP((UINT8 *) &(wh->addr1)))
        {
            /* Verify that broadcast src address is not client */
            if ((VMacEntry_p = vmacGetVMacEntryByAddr(wh->addr3)) != NULL)
            {
                return AP8X_80211_DATA_DROP;
            }
        }
        else
        {
            /* Unicast check if for client */
            if ((VMacEntry_p = vmacGetVMacEntryByAddr(wh->addr1)) == NULL)
            {
                return AP8X_80211_DATA_DROP;
            }
        }
    }
    else
#endif
    {
        if(memcmp(wh->addr1,vmacSta_p->macStaAddr,6) )
        {
            //dev_kfree_skb_any(skb);
            WlLogPrint(MARVEL_DEBUG_DEBUG, __func__,"BSSID is not match!");
            return AP8X_80211_DATA_DROP;
        }
#ifdef WDS_FEATURE
        if (pStaInfo == NULL)
        {
            return AP8X_80211_DATA_DEAUTH;
        }
        else if ((pStaInfo->State != ASSOCIATED) && !pStaInfo->AP)
        {
            return AP8X_80211_DATA_DEAUTH;
        }
#else
        if ((pStaInfo == NULL) || (pStaInfo->State != ASSOCIATED))
        {
            //WLDBG_INFO(DBG_LEVEL_9, "class3 frame from %x %d\n", pStaInfo, pStaInfo?pStaInfo->State:0);
            return AP8X_80211_DATA_DEAUTH;
        }
#endif
    }

    if((*(mib->mib_STAMacCloneEnable) == 2) && IS_GROUP((UINT8 *) &(wh->addr1)) && wh->FrmCtl.FromDs) {
        if (ethStaDb_GetStaInfo(vmacSta_p, &(wh->addr3), 1) != NULL ) {                                                          
            return AP8X_80211_DATA_DROP;
        }
    }
    return AP8X_80211_DATA_SUCC;
}

#if 0
int ieee80211_autelan_send_data_ccmp_process(struct sk_buff *skb, vmacApInfo_t *vmacSta_p)
{
    MIB_802DOT11 *mib = NULL;
    struct ieee80211_frame *wh = NULL;
    UINT8 state2 = 0;
    int ret = 0;

    if (skb == NULL || vmacSta_p == NULL)
    {
        return ret;
    }

    mib = vmacSta_p->Mib802dot11;
    
    if (thinap)
    {
        if (skb->protocol == htons(0x0019))
        {
            if ((mib->RSNConfig->MulticastCipher[3] == 0x04) && 
                (mib->UnicastCiphers->UnicastCipher[3] == 0x04) && 
                (*(mib->mib_wpaWpa2Mode) == 1))
            {
                wh = (struct ieee80211_frame *)skb->data;
                state2 = skb->data[1];
                if (state2 == 0x42)
                {
                    ret = 1;
                }
            }
        }
    }

    return ret;
}
#endif

int ieee80211_autelan_send_data_frame_local_mode(struct sk_buff *skb, vmacApInfo_t *vmacSta_p, BOOLEAN *eapolPkt, int *type,extStaDb_StaInfo_t **pStaInfo_tmp)
{
    struct ieee80211_frame *wh = NULL;
    UINT16 ether_type = 0;
    UINT8 state2 = 0;
    int ret = AP8X_FATAP_MODE;
    UINT8 tmpbuf[NBR_BYTES_IEEE80211HEADER];
    
    /*ljy--modified begin to drop the management frame from AC by wtpd*/
    wh = (struct ieee80211_frame *)skb->data;

    if (wh->FrmCtl.Type == IEEE_TYPE_DATA)
    {

        int qos_addr4_padding = 2 + IEEEtypes_ADDRESS_SIZE;

        if(IS_GROUP(wh->addr1))
        {
            WlLogPrint(MARVEL_DEBUG_ALL, __func__,"Group Data Packet will send to sta,protocol is 0x0019\n");
            if((vmacSta_p->master == NULL)||(vmacSta_p->master->StaCtl == NULL)||(vmacSta_p->master->StaCtl->StaList.cnt == 0))
            {
                WlLogPrint(MARVEL_DEBUG_DEBUG, __func__,"No Find Sta,Group packet will drop\n");
                return AP8X_THINAP_MODE_ERR;
            }
        }
        else
        {
            WlLogPrint(MARVEL_DEBUG_ALL, __func__,"Unicast Data Packet will send to sta,protocol is 0x0019\n");

            (*pStaInfo_tmp) = extStaDb_GetStaInfo(vmacSta_p,(IEEEtypes_MacAddr_t *)&wh->addr1, 1);
            if ((*pStaInfo_tmp) == NULL)
            {
                WlLogPrint(MARVEL_DEBUG_WARNING, __func__,"Not Find STA %02X:%02X:%02X:%02X:%02X:%02X\n",\
                    wh->addr1[0],wh->addr1[1],wh->addr1[2],wh->addr1[3],wh->addr1[4],wh->addr1[5]);
                return AP8X_THINAP_MODE_ERR;
            }

            if (((*pStaInfo_tmp)->State != ASSOCIATED) && !(*pStaInfo_tmp)->AP)
            {
                WlLogPrint(MARVEL_DEBUG_WARNING, __func__,"STA Not Assocoated %02X:%02X:%02X:%02X:%02X:%02X\n",\
                    wh->addr1[0],wh->addr1[1],wh->addr1[2],wh->addr1[3],wh->addr1[4],wh->addr1[5]);
                return AP8X_THINAP_MODE_ERR;
            }
        }

        ether_type = ieee80211_autelan_get_3_address_packet_llc_analysis(skb);

        if(wh->FrmCtl.Subtype == QoS_DATA)
        {
            qos_addr4_padding -= roundup(2, 4);     
        }

        /* add by zhanxuechao for 3 address change to 4 address */
        if(skb_headroom(skb) < qos_addr4_padding)
        {
            //the code should not come here, need to find out why this happens!!!!
            WlLogPrint(MARVEL_DEBUG_ERROR, __func__,"skb_headroom error\n");
            ret = AP8X_THINAP_MODE_ERR;
            return ret;
        }
        
        if (ether_type == IEEE_ETHERTYPE_PAE || ether_type == IEEE_ETHERTYPE_WAI)
        {
            *eapolPkt = TRUE;
        }
        else
        {
            *eapolPkt = FALSE;
        }

        state2 = skb->data[1];

        memcpy(tmpbuf, skb->data, NBR_BYTES_IEEE80211HEADER);
        skb_push(skb, qos_addr4_padding);
        memcpy(skb->data, tmpbuf, NBR_BYTES_IEEE80211HEADER);
        memset(&skb->data[NBR_BYTES_IEEE80211HEADER], 0, qos_addr4_padding);

        if (ether_type == IEEE_ETHERTYPE_PAE)
        {
            if (state2 == 0x42)
            {
                *eapolPkt = FALSE;
            }
        }

        WlLogPrint(MARVEL_DEBUG_ALL,__func__,"Send 0x0019 Packet to STA. ether_type:%02x eapolPkt:%d len: %d\n", ether_type, *eapolPkt,skb->len);
        
        /*printk("send a 802.11 frame\n");*/
        ret = AP8X_THINAP_MODE_SEND;
        *type = IEEE_TYPE_DATA;
    }
    else if (wh->FrmCtl.Type == IEEE_TYPE_MANAGEMENT) 
    {
        
        /*Begin:Added by duanmingzhe for traffic balance*/
        if(traffic_balance)
        {
            WlLogPrint(MARVEL_DEBUG_ALL, __func__,"traffic_balance is open,management packet subtype = %d will send\n",wh->FrmCtl.Subtype);

            if(skb_headroom(skb) < 8)
            {
                //the code should not come here, need to find out why this happens!!!!
                WlLogPrint(MARVEL_DEBUG_ERROR, __func__,"skb_headroom error \n");
                ret = AP8X_THINAP_MODE_ERR;
                return ret;
            }

#ifdef AE5200_RT_SUPPORT

            if (wh->FrmCtl.Subtype == IEEE_MSG_ASSOCIATE_RSP) 
            {
                UINT16 status,status_len;
                (*pStaInfo_tmp) = extStaDb_GetStaInfo(vmacSta_p,(IEEEtypes_MacAddr_t *)&wh->addr1, 1);
                if ((*pStaInfo_tmp) == NULL)
                {
                    WlLogPrint(MARVEL_DEBUG_WARNING, __func__,"Not Find STA %02X:%02X:%02X:%02X:%02X:%02X\n",\
                        wh->addr1[0],wh->addr1[1],wh->addr1[2],wh->addr1[3],wh->addr1[4],wh->addr1[5]);
                    return AP8X_THINAP_MODE_ERR;
                }

                status_len = sizeof(struct ieee80211_frame)- IEEEtypes_ADDRESS_SIZE + 2;
                status = le16_to_cpu(*((UINT16 *)&skb->data[status_len]));

                WlLogPrint(MARVEL_DEBUG_DEBUG, __func__,"ASSOCIATE_RSP status = %d\n",status);

                macMgmtMlme_SendAssocicateMsg(vmacSta_p,(*pStaInfo_tmp),status,0);
                return AP8X_THINAP_MODE_ERR;
            }
#endif
            /* add by zhanxuechao for 3 address chenge to 4 address */
            memcpy(tmpbuf, skb->data, NBR_BYTES_IEEE80211HEADER);
            skb_push(skb, 6);
            memcpy(skb->data, tmpbuf, NBR_BYTES_IEEE80211HEADER);
            memset(&skb->data[NBR_BYTES_IEEE80211HEADER], 0, 6);

#if 0
            ether_type = ieee80211_autelan_eapol_llc_analysis(skb);
            if (ether_type == IEEE_ETHERTYPE_PAE || ether_type == IEEE_ETHERTYPE_WAI)
            {
                *eapolPkt = TRUE;
            }
            else
            {
                *eapolPkt = FALSE;
            }

            state2 = skb->data[1];

            memcpy(tmpbuf, skb->data, NBR_BYTES_IEEE80211HEADER);
            skb_push(skb, 6);
            memcpy(skb->data, tmpbuf, NBR_BYTES_IEEE80211HEADER);
            memset(&skb->data[NBR_BYTES_IEEE80211HEADER], 0, 6);

            if (ether_type == IEEE_ETHERTYPE_PAE)
            {
                if (state2 == 0x42)
                {
                    *eapolPkt = FALSE;
                }
            }

            WlLogPrint(MARVEL_DEBUG_ALL, __func__, "Send 0x0019 Packet to STA. ether_type:%02x eapolPkt:%d\n", ether_type, *eapolPkt);
#endif

            WlLogPrint(MARVEL_DEBUG_ALL, __func__, "Send management Packet to STA.\n");
            //printk("this is  management frame from ac!\n");
            //vap->iv_stats.is_tx_mgmt++;/*sheyanbin add for management frame count*/
            //vap->iv_stats.is_tx_mgmtbytes += wbuf->len;  //lisongbai add for tx_mgmtbytes count in 2011-01-22
            ret = AP8X_THINAP_MODE_SEND;
            *type = IEEE_TYPE_MANAGEMENT;
        }
        else
        {
            ret = AP8X_THINAP_MODE_ERR;
        }
        /*End:Added by duanmingzhe for traffic balance*/
    }
    else
    {
        ret = AP8X_THINAP_MODE_ERR;
    }

    return ret;
}


int ieee80211_autelan_recv_data_frame_ubc_mode(struct sk_buff **skb, struct net_device *dev, extStaDb_StaInfo_t *pStaInfo, UINT16 frametype)
{

#define TUNNEL_MODE 1
#define LOCAL_MODE 0

    struct sk_buff *wbuf_capture = NULL;
    struct wlprivate *wlpptr = NULL;
    int ret = -1;

    wlpptr = NETDEV_PRIV_P(struct wlprivate, dev);
    wbuf_capture = *skb;

    WlLogPrint(MARVEL_DEBUG_ALL, __func__, "UBC Mode:Receive Data dev name : %s frametype: %02x len: %d\n", dev->name, frametype,wbuf_capture->len);

    if((ieee80211_node_is_authorize(pStaInfo) != IEEE80211_NODE_AUTHORIZE_SUCC))
    {
        ret = TUNNEL_MODE;        
    }
    else
    {
        if (pStaInfo->ni_localflags)
        {
            if (pStaInfo->ni_portal_ipaddr[0] == 0)// outer portal server mode
                ret = LOCAL_MODE; //local
            else//inner portal server mode 
            {
                struct iphdr *ipwh = (struct iphdr *)((*skb)->data + sizeof(struct ieee80211_frame) + LLC_SNAPFRAMELEN);

                if (memcmp(&(ipwh->daddr),pStaInfo->ni_portal_ipaddr,4) == 0)// this frame is sent to portal server,so go to tunnel
                {
                    ret = TUNNEL_MODE;
                }
                else
                {
                    ret = LOCAL_MODE; //local
                }
            }
        }
		else
        {
            //DHCP,ARP,DNS local ,others tunnel
            if (dhcp_detect_wh1(&(wlpptr->vap),wbuf_capture) || dns_detect_wh(&(wlpptr->vap),wbuf_capture) || arp_detect_wh(&(wlpptr->vap),wbuf_capture))
                ret = LOCAL_MODE;       //local
            else 
                ret = TUNNEL_MODE;      //tunnel
        }
    }
    
#undef TUNNEL_MODE
#undef LOCAL_MODE
    
    return ret;
}

int ieee80211_autelan_recv_data_frame_80211_tunnel_mode(struct sk_buff **skb, struct net_device *dev, extStaDb_StaInfo_t *pStaInfo, UINT16 frametype)
{
    struct sk_buff *wbuf_capture = NULL;
    struct wlprivate *wlpptr = NULL;
    UINT8 tmpbuf[NBR_BYTES_IEEE80211HEADER];
    int ret = AP8X_FATAP_MODE;

    wlpptr = NETDEV_PRIV_P(struct wlprivate, dev);
    wbuf_capture = *skb;

    WlLogPrint(MARVEL_DEBUG_ALL, __func__, "80211 TUNNEL Mode:Receive Data dev name : %s frametype: %02x len: %d\n", dev->name, frametype,wbuf_capture->len);

    if((ieee80211_node_is_authorize(pStaInfo) == IEEE80211_NODE_AUTHORIZE_SUCC))
    {
        if((frametype == IEEE_ETHERTYPE_PAE) && \
            (ieee80211_autelan_get_4_address_packet_eapol_type_analysis(*skb) == 0x03))
        {
            return ret;
        }
    
        /* add by zhanxuechao for 4 address chenge to 3 address */
        memcpy(tmpbuf, wbuf_capture->data, NBR_BYTES_IEEE80211HEADER);
        skb_pull(wbuf_capture, 6);
        memcpy(wbuf_capture->data, tmpbuf, NBR_BYTES_IEEE80211HEADER);
    }
#if 0
    else if(frametype == IEEE_ETHERTYPE_PAE || frametype == IEEE_ETHERTYPE_PRE_PAE
     || dhcp_detect_wh(&(wlpptr->vap), wbuf_capture)
     || pppoe_detect_wh(&(wlpptr->vap), wbuf_capture)//zhaoyang add for through the pppoe frame(add pppoe_detect_wh(vap,wbuf_capture))
#ifndef AE5200_RT_SUPPORT
     || frametype == IEEE_ETHERTYPE_WAI)
    {
#else
    )
    {
        char eapol_type = 0;
        eapol_type = ieee80211_autelan_get_4_address_packet_eapol_type_analysis(*skb);

        if(eapol_type == 0x03)  //EAPOL KEY PACKET will send hostapd(4-way Handshake)
        {
            WlLogPrint(MARVEL_DEBUG_ALL, __func__, "Receive EALOL TYPE %d to HOSTAPD\n",eapol_type);
            return ret;
        }
    
        WlLogPrint(MARVEL_DEBUG_ALL, __func__, "Receive EALOL TYPE %d to WTPD\n",eapol_type);
#endif
#else
    else if(frametype == IEEE_ETHERTYPE_PAE || frametype == IEEE_ETHERTYPE_PRE_PAE
     || dhcp_detect_wh(&(wlpptr->vap), wbuf_capture)
     || pppoe_detect_wh(&(wlpptr->vap), wbuf_capture)//zhaoyang add for through the pppoe frame(add pppoe_detect_wh(vap,wbuf_capture))
     || frametype == IEEE_ETHERTYPE_WAI)
    {
#ifdef AE5200_RT_SUPPORT
        char eapol_type = 0;
        eapol_type = ieee80211_autelan_get_4_address_packet_eapol_type_analysis(*skb);

        if(eapol_type == 0x03)  //EAPOL KEY PACKET will send hostapd(4-way Handshake)
        {
            WlLogPrint(MARVEL_DEBUG_ALL, __func__, "Receive EALOL TYPE %d to HOSTAPD\n",eapol_type);
            return ret;
        }
    
        WlLogPrint(MARVEL_DEBUG_ALL, __func__, "Receive EALOL TYPE %d to WTPD\n",eapol_type);
#endif
#endif
        /* add by zhanxuechao for 4 address chenge to 3 address */
        memcpy(tmpbuf, wbuf_capture->data, NBR_BYTES_IEEE80211HEADER);
        skb_pull(wbuf_capture, 6);
        memcpy(wbuf_capture->data, tmpbuf, NBR_BYTES_IEEE80211HEADER);
        
        wbuf_capture->protocol = __constant_htons(0x0020); /* ETH_P_80211_RAW */

    }
    else
    {
        WlLogPrint(MARVEL_DEBUG_WARNING, __func__, "Tunnel Mode:Receive Packet,But Sta is not authorized\n");
        ret = AP8X_THINAP_MODE_TUNNEL_DROP;
        return ret;
    }

    *skb = NULL;
    wbuf_capture->dev = dev;

    wbuf_capture = skb_unshare(wbuf_capture, GFP_ATOMIC);

    if (wbuf_capture == NULL)
    {
        WlLogPrint(MARVEL_DEBUG_ERROR, __func__,"SKB unshare operation failed!\n");
    }

    if (skb_headroom(wbuf_capture) < 80)    // capwap_split_fast need 80 bytes;
    {
        struct sk_buff *tmp;

        tmp = skb_realloc_headroom(wbuf_capture, 80);
        if (tmp == NULL)
        {
            WlLogPrint(MARVEL_DEBUG_ERROR, __func__,"SKB headroom not enough --- reallocate headroom!\n");
        }

        if ((wbuf_capture)->sk)
            skb_set_owner_w(tmp, (wbuf_capture)->sk);

        dev_kfree_skb_any(wbuf_capture);        
        wbuf_capture = tmp;
    }
    
    //wbuf_capture->mac_header = wbuf_capture->data;
#if (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,22))
    wbuf_capture->mac.raw = wbuf_capture->data;
#else
    skb_reset_mac_header(wbuf_capture);
#endif

    wbuf_capture->ip_summed = CHECKSUM_NONE;
    wbuf_capture->pkt_type = PACKET_OTHERHOST;

    if (wbuf_capture->protocol != __constant_htons(0x0020)) 
    {
        wbuf_capture->protocol = __constant_htons(0x0019); /* ETH_P_80211_RAW */
    }

    if(wbuf_capture->data[0] & IEEE80211_FC0_SUBTYPE_QOS)   /* clear QoS bit */
        wbuf_capture->data[0] &= ~IEEE80211_FC0_SUBTYPE_QOS;
        
    WlLogPrint(MARVEL_DEBUG_ALL, __func__, "80211 Tunnel Mode: Send Packet to Linux kernel. \n");
    ret = netif_rx(wbuf_capture);
    if(NET_RX_DROP == ret)
    {
        if (wbuf_capture != NULL)
        {
            dev_kfree_skb_any(wbuf_capture);
            ret = AP8X_THINAP_MODE_NETIF_RX_DROP; 
        }
    }
    else
    {
        ret = AP8X_THINAP_MODE_TUNNEL_SUCC;
    }
    
    return ret;
}


int ieee80211_autelan_recv_data_frame_8023_tunnel_mode(struct sk_buff **skb, struct net_device *dev, extStaDb_StaInfo_t *pStaInfo, UINT16 frametype)
{
    struct sk_buff *wbuf_capture = NULL;
    struct wlprivate *wlpptr = NULL;
    //vmacApInfo_t *vmacSta_p = NULL;
    //MIB_802DOT11 *mib = NULL;
    UINT8 tmpbuf[NBR_BYTES_IEEE80211HEADER];
    struct ieee80211_frame *wh;
    int ret = AP8X_FATAP_MODE;
    int packet_type = 0;   //1:8203_packet  2:80211_packet
    
    wlpptr = NETDEV_PRIV_P(struct wlprivate, dev);
    wbuf_capture = *skb;
    wh = (struct ieee80211_frame *)wbuf_capture->data;

    WlLogPrint(MARVEL_DEBUG_ALL, __func__, "8023 TUNNEL Mode:Receive Data dev name : %s frametype: %02x len: %d\n", dev->name, frametype,wbuf_capture->len);

    if((ieee80211_node_is_authorize(pStaInfo) == IEEE80211_NODE_AUTHORIZE_SUCC))
    {
        /* add by zhanxuechao for 4 address chenge to 3 address */
        //memcpy(tmpbuf, wbuf_capture->data, NBR_BYTES_IEEE80211HEADER);
        //skb_pull(wbuf_capture, 6);
        //memcpy(wbuf_capture->data, tmpbuf, NBR_BYTES_IEEE80211HEADER);
        packet_type = 1;
        wbuf_capture->protocol = __constant_htons(0x0030); /* ETH_P_80211_RAW */
    }
#if 0
    else if(frametype == IEEE_ETHERTYPE_PAE || frametype == IEEE_ETHERTYPE_PRE_PAE
     || dhcp_detect_wh(&(wlpptr->vap), wbuf_capture)
     || pppoe_detect_wh(&(wlpptr->vap), wbuf_capture)//zhaoyang add for through the pppoe frame(add pppoe_detect_wh(vap,wbuf_capture))
#ifndef AE5200_RT_SUPPORT
     || frametype == IEEE_ETHERTYPE_WAI)
    {
#else
    )
    {
        char eapol_type = 0;
        eapol_type = ieee80211_autelan_get_4_address_packet_eapol_type_analysis(*skb);

        if(eapol_type == 0x03)  //EAPOL KEY PACKET will send hostapd(4-way Handshake)
        {
            WlLogPrint(MARVEL_DEBUG_ALL, __func__, "Receive EALOL TYPE %d to HOSTAPD\n",eapol_type);
            return ret;
        }
    
        WlLogPrint(MARVEL_DEBUG_ALL, __func__, "Receive EALOL TYPE %d to WTPD\n",eapol_type);
#endif
#else
    else if(frametype == IEEE_ETHERTYPE_PAE || frametype == IEEE_ETHERTYPE_PRE_PAE
     || dhcp_detect_wh(&(wlpptr->vap), wbuf_capture)
     || pppoe_detect_wh(&(wlpptr->vap), wbuf_capture)//zhaoyang add for through the pppoe frame(add pppoe_detect_wh(vap,wbuf_capture))
     || frametype == IEEE_ETHERTYPE_WAI)
    {
#ifdef AE5200_RT_SUPPORT
        char eapol_type = 0;
        eapol_type = ieee80211_autelan_get_4_address_packet_eapol_type_analysis(*skb);

        if(eapol_type == 0x03)  //EAPOL KEY PACKET will send hostapd(4-way Handshake)
        {
            WlLogPrint(MARVEL_DEBUG_ALL, __func__, "Receive EALOL TYPE %d to HOSTAPD\n",eapol_type);
            return ret;
        }
    
        WlLogPrint(MARVEL_DEBUG_ALL, __func__, "Receive EALOL TYPE %d to WTPD\n",eapol_type);
#endif
#endif
        /* add by zhanxuechao for 4 address chenge to 3 address */
        memcpy(tmpbuf, wbuf_capture->data, NBR_BYTES_IEEE80211HEADER);
        skb_pull(wbuf_capture, 6);
        memcpy(wbuf_capture->data, tmpbuf, NBR_BYTES_IEEE80211HEADER);
        packet_type = 2;
        wbuf_capture->protocol = __constant_htons(0x0019); /* ETH_P_80211_RAW */

    }
    else
    {
        WlLogPrint(MARVEL_DEBUG_WARNING, __func__, "Tunnel Mode:Receive Packet,But Sta is not authorized\n");
        ret = AP8X_THINAP_MODE_TUNNEL_DROP;
        return ret;
    }

    *skb = NULL;

    if (packet_type == 1)   // 80211 transform 8023
    {
        UINT8 *BSSID = NULL;
        wbuf_capture = ieee80211_decap(dev,wbuf_capture,pStaInfo);
        
        if (wbuf_capture == NULL)
        {
            WlLogPrint(MARVEL_DEBUG_ERROR, __func__,"ieee80211_decap wbuf_capture == NULL\n");
            return AP8X_THINAP_MODE_NETIF_RX_DROP;
        }

        BSSID = (UINT8 *)skb_push(wbuf_capture,IEEEtypes_ADDRESS_SIZE);
        IEEE80211_ADDR_COPY(BSSID, wh->addr1);
    }
    else if (packet_type == 2)  //del qos
    {
        if(wbuf_capture->data[0] & IEEE80211_FC0_SUBTYPE_QOS)   /* clear QoS bit */
            wbuf_capture->data[0] &= ~IEEE80211_FC0_SUBTYPE_QOS;
    }

    wbuf_capture->dev = dev;

    wbuf_capture = skb_unshare(wbuf_capture, GFP_ATOMIC);

    if (wbuf_capture == NULL)
    {
        WlLogPrint(MARVEL_DEBUG_ERROR, __func__,"SKB unshare operation failed!\n");
        return AP8X_THINAP_MODE_NETIF_RX_DROP;
    }

    if (skb_headroom(wbuf_capture) < 80)    // capwap_split_fast need 80 bytes;
    {
        struct sk_buff *tmp;

        tmp = skb_realloc_headroom(wbuf_capture, 80);
        if (tmp == NULL)
        {
            WlLogPrint(MARVEL_DEBUG_ERROR, __func__,"SKB headroom not enough --- reallocate headroom!\n");
            return AP8X_THINAP_MODE_NETIF_RX_DROP;
        }

        if ((wbuf_capture)->sk)
            skb_set_owner_w(tmp, (wbuf_capture)->sk);

        dev_kfree_skb_any(wbuf_capture);        
        wbuf_capture = tmp;
    }

#if (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,22))
    wbuf_capture->mac.raw = wbuf_capture->data;
#else
    skb_reset_mac_header(wbuf_capture);
#endif

    wbuf_capture->ip_summed = CHECKSUM_NONE;
    wbuf_capture->pkt_type = PACKET_OTHERHOST;

    WlLogPrint(MARVEL_DEBUG_ALL, __func__, "8023 Tunnel Mode: Send Packet to Linux kernel. \n");
    ret = netif_rx(wbuf_capture);
    if(NET_RX_DROP == ret)
    {
        if (wbuf_capture != NULL)
        {
            dev_kfree_skb_any(wbuf_capture);
            ret = AP8X_THINAP_MODE_NETIF_RX_DROP; 
        }
    }
    else
    {
        ret = AP8X_THINAP_MODE_TUNNEL_SUCC;
    }

    return ret;
}


int ieee80211_autelan_recv_data_frame_local_mode(struct sk_buff **skb, struct net_device *dev, extStaDb_StaInfo_t *pStaInfo, UINT16 frametype)
{
    struct sk_buff *wbuf_capture = NULL;
    int ret = AP8X_FATAP_MODE;
    UINT8 tmpbuf[NBR_BYTES_IEEE80211HEADER];
    
    WlLogPrint(MARVEL_DEBUG_ALL, __func__, "Local Mode:Receive Data dev name : %s frametype: %02x len: %d\n", dev->name, frametype,(*skb)->len);

    if((ieee80211_node_is_authorize(pStaInfo) == IEEE80211_NODE_AUTHORIZE_SUCC))
    {
        return ret;
    }
#if 0   
    else if(frametype == IEEE_ETHERTYPE_PAE || frametype == IEEE_ETHERTYPE_PRE_PAE
#ifndef AE5200_RT_SUPPORT
     || frametype == IEEE_ETHERTYPE_WAI) 
    {
#else
    )
    {
        char eapol_type = 0;
        eapol_type = ieee80211_autelan_get_4_address_packet_eapol_type_analysis(*skb);

        if(eapol_type == 0x03)  //EAPOL KEY PACKET will send hostapd(4-way Handshake)
        {
            WlLogPrint(MARVEL_DEBUG_ALL, __func__, "Receive EALOL TYPE %d to HOSTAPD\n",eapol_type);
            return ret;
        }
    
        WlLogPrint(MARVEL_DEBUG_ALL, __func__, "Receive EALOL TYPE %d to WTPD\n",eapol_type);

#endif
#else
    else if(frametype == IEEE_ETHERTYPE_PAE || frametype == IEEE_ETHERTYPE_PRE_PAE
     || frametype == IEEE_ETHERTYPE_WAI) 
    {
    
#ifdef AE5200_RT_SUPPORT
        char eapol_type = 0;
        eapol_type = ieee80211_autelan_get_4_address_packet_eapol_type_analysis(*skb);

        if(eapol_type == 0x03)  //EAPOL KEY PACKET will send hostapd(4-way Handshake)
        {
            WlLogPrint(MARVEL_DEBUG_ALL, __func__, "Receive EALOL TYPE %d to HOSTAPD\n",eapol_type);
            return ret;
        }
    
        WlLogPrint(MARVEL_DEBUG_ALL, __func__, "Receive EALOL TYPE %d to WTPD\n",eapol_type);
#endif
#endif
        /*ljy--modified for wapi*/
        wbuf_capture = skb_copy(*skb, GFP_ATOMIC);
        if (wbuf_capture== NULL) 
        {
            WlLogPrint(MARVEL_DEBUG_WARNING, __func__, "capture copy error \n");
            ret = AP8X_THINAP_MODE_SKB_RELOCATE_FAIL;
            return ret;
        }

        /* add by zhanxuechao for 4 address chenge to 3 address */
        memcpy(tmpbuf, wbuf_capture->data, NBR_BYTES_IEEE80211HEADER);
        skb_pull(wbuf_capture, 6);
        memcpy(wbuf_capture->data, tmpbuf, NBR_BYTES_IEEE80211HEADER);

        //skb_push(wbuf_capture, sizeof(wlan_ng_prism2_header));/*add the prism2 header*/

        wbuf_capture->dev = dev ;
#if LINUX_VERSION_CODE <= KERNEL_VERSION(2,6,20)
        wbuf_capture->mac.raw = wbuf_capture->data;
#else
        wbuf_capture->mac_header = wbuf_capture->data;
#endif
        wbuf_capture->ip_summed = CHECKSUM_NONE;
        wbuf_capture->pkt_type = PACKET_OTHERHOST;
        wbuf_capture->protocol = __constant_htons(0x0019);/*ETH_P_80211_RAW*/
        
        if(wbuf_capture->data[0] & IEEE80211_FC0_SUBTYPE_QOS)   /* clear QoS bit */
            wbuf_capture->data[0] &= ~IEEE80211_FC0_SUBTYPE_QOS;

        WlLogPrint(MARVEL_DEBUG_ALL, __func__, "Local Mode:888E Send Packet to WTPD. \n");

        ret = netif_rx(wbuf_capture);
        
        if(NET_RX_DROP == ret)
        {
            if (wbuf_capture != NULL)
            {
                dev_kfree_skb_any(wbuf_capture);
                ret = AP8X_THINAP_MODE_NETIF_RX_DROP; 
            }                        
        }
        else
        {
            ret = AP8X_THINAP_MODE_TUNNEL_SUCC;
        }
    }
    else
    {
        WlLogPrint(MARVEL_DEBUG_WARNING, __func__, "Local Mode:Receive Packet,But Sta is not authorized\n");
        ret = AP8X_THINAP_MODE_NETIF_RX_DROP;
    }

    return ret;
}



