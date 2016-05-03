/*
*                Copyright 2005, Marvell Semiconductor, Inc.
* This code contains confidential information of Marvell Semiconductor, Inc.
* No rights are granted herein under any patent, mask work right or copyright
* of Marvell or any third party.
* Marvell reserves the right at its sole discretion to request that this code
* be immediately returned to Marvell. This code is provided "as is".
* Marvell makes no warranties, express, implied or otherwise, regarding its
* accuracy, completeness or performance.
*/

#include <net/iw_handler.h>
#include <asm/processor.h>
#include <asm/uaccess.h>
#include "ap8xLnxFwcmd.h"
#include "ap8xLnxStats.h"

#ifdef CLIENT_SUPPORT
#include "linkmgt.h"
#include "mlme.h"
#include "mlmeApi.h"
#endif

#include "wllog.h"

#define IEEE80211_IS_MULTICAST(_a)  (*(_a) & 0x01)
#define IEEE80211_IS_BROADCAST(_a)              \
    ((_a)[0] == 0xff &&                         \
     (_a)[1] == 0xff &&                         \
     (_a)[2] == 0xff &&                         \
     (_a)[3] == 0xff &&                         \
     (_a)[4] == 0xff &&                         \
     (_a)[5] == 0xff)

#define MWL_SPIN_LOCK(X) spin_lock_irqsave(X, flags)
#define MWL_SPIN_UNLOCK(X)  spin_unlock_irqrestore(X, flags)

extern int new_dispatch_mac;
extern int vap_mac;

/* mic,wepfail,badkey,unencrypted*/
void Statistics_Recv_Error_packet(struct net_device *dev)
{
    struct wlprivate *wlpptr = NETDEV_PRIV_P(struct wlprivate, dev);
    vmacApInfo_t *vmacSta_p = wlpptr->vmacSta_p;
    vmacApInfo_t  *vmac_ap = NULL;

    //ath
    wlpptr->stats_info.rx_dropped++;
    wlpptr->stats_info.rx_errors++;  
    wlpptr->netDevStats.rx_dropped++;
    wlpptr->netDevStats.rx_errors++;

    Statistics_Recv_Frame_Errors(dev);

    if(vmacSta_p->master)       //wifi
    {
        vmac_ap = vmacSta_p->master;
        (NETDEV_PRIV_P(struct wlprivate, vmac_ap->dev))->stats_info.rx_dropped++;
        (NETDEV_PRIV_P(struct wlprivate, vmac_ap->dev))->stats_info.rx_errors++;
        (NETDEV_PRIV_P(struct wlprivate, vmac_ap->dev))->netDevStats.rx_dropped++;
        (NETDEV_PRIV_P(struct wlprivate, vmac_ap->dev))->netDevStats.rx_errors++;
    }
        
    return;
}

int Statistics_Recv_Frame_Errors(struct net_device *netdev)
{
    struct wlprivate *wlpptr = NETDEV_PRIV_P(struct wlprivate, netdev);
    struct net_device_stats  *stat;
    vmacApInfo_t *vmacSta_p;
    vmacApInfo_t  *vmac_ap;

    if(wlpptr == NULL)
        return -1;
                
    /*****ath****/        
    stat = &(wlpptr->netDevStats);
    vmacSta_p = wlpptr->vmacSta_p;

    stat->rx_frame_errors++;
    wlpptr->stats_info.rx_errors_frames++;

    if(vmacSta_p->master)
        vmac_ap = vmacSta_p->master;
    else
        return 0;

    /*****wifi****/
    wlpptr = NETDEV_PRIV_P(struct wlprivate, vmac_ap->dev); 
    stat = &(wlpptr->netDevStats);

    stat->rx_frame_errors++;
    wlpptr->stats_info.rx_errors_frames++;

    return 0;
}


int Statistics_Recv_Packet_Retry(vmacApInfo_t *vmacSta_p,UINT32 len,UINT8 type)
{       
    struct wlprivate *wlpptr = NULL;
    struct net_device_stats  *stat = NULL;
    vmacApInfo_t  *vmac_ap = NULL;

    if(vmacSta_p == NULL)
    {
        return -1;
    }
        
    wlpptr = NETDEV_PRIV_P(struct wlprivate, vmacSta_p->dev);

    if(type == IEEE_TYPE_DATA)
    {
        /*****ath****/        
        stat = &(wlpptr->netDevStats);
        vmacSta_p = wlpptr->vmacSta_p;

        stat->rx_retry_packets++;
        stat->rx_retry_bytes += len;
        wlpptr->stats_info.rx_retry++;
        wlpptr->stats_info.rx_retry_bytes += len;

        if(vmacSta_p->master)
            vmac_ap = vmacSta_p->master;
        else
            return 0;

        /*****wifi****/
        wlpptr = NETDEV_PRIV_P(struct wlprivate, vmac_ap->dev); 
        stat = &(wlpptr->netDevStats);

        stat->rx_retry_packets++;
        stat->rx_retry_bytes += len;
        wlpptr->stats_info.rx_retry++;
        wlpptr->stats_info.rx_retry_bytes += len;
    }
    else if(type == IEEE_TYPE_MANAGEMENT)
    {
        /*****ath****/        
        stat = &(wlpptr->netDevStats);
        vmacSta_p = wlpptr->vmacSta_p;

        stat->rx_retry_mgmt_packets++;
        stat->rx_retry_mgmt_bytes += len;
        wlpptr->stats_info.rx_retry++;
        wlpptr->stats_info.rx_retry_bytes += len;

        if(vmacSta_p->master)
            vmac_ap = vmacSta_p->master;
        else
            return 0;

        /*****wifi****/
        wlpptr = NETDEV_PRIV_P(struct wlprivate, vmac_ap->dev); 
        stat = &(wlpptr->netDevStats);

        stat->rx_retry_mgmt_packets++;
        stat->rx_retry_mgmt_bytes += len;
        wlpptr->stats_info.rx_retry++;
        wlpptr->stats_info.rx_retry_bytes += len;

    }
        
    return 0;
}

int Statistics_Recv_Data_Packet(struct net_device *netdev,UINT8 *addr,UINT32 len)
{
    struct wlprivate *wlpptr = NETDEV_PRIV_P(struct wlprivate, netdev);
    struct net_device_stats  *stat;
    vmacApInfo_t *vmacSta_p;
    vmacApInfo_t  *vmac_ap;

    if((wlpptr == NULL) ||(addr == NULL))
        return -1;

    /*****ath****/        
    stat = &(wlpptr->netDevStats);
    vmacSta_p = wlpptr->vmacSta_p;
    if(IEEE80211_IS_BROADCAST(addr))
    {
        wlpptr->stats_info.rx_broadcast++;
        wlpptr->stats_info.rx_broadcast_bytes += len;
        stat->rx_broadcast_packets ++;
        stat->rx_broadcast_bytes += len;
    }
    else if(IEEE80211_IS_MULTICAST(addr))
    {
        wlpptr->stats_info.rx_multicast++;
        wlpptr->stats_info.rx_multicast_bytes += len;
        stat->rx_multicast_packets ++;
        stat->rx_multicast_bytes += len;
    }
    else
    {
        wlpptr->stats_info.rx_unicast++;
        wlpptr->stats_info.rx_unicast_bytes += len;
    }

    wlpptr->stats_info.rx_sum_packets++;
    wlpptr->stats_info.rx_sum_bytes += len;
    wlpptr->stats_info.rx_packets++;
    wlpptr->stats_info.rx_bytes += len;
    stat->rx_packets++;
    stat->rx_bytes += len;
        
    if(vmacSta_p->master)
        vmac_ap = vmacSta_p->master;
    else
        return 0;

    /*****wifi****/
    wlpptr = NETDEV_PRIV_P(struct wlprivate, vmac_ap->dev); 
    stat = &(wlpptr->netDevStats);
        
    if(IEEE80211_IS_BROADCAST(addr))
    {
        wlpptr->stats_info.rx_broadcast++;
        wlpptr->stats_info.rx_broadcast_bytes += len;
        stat->rx_broadcast_packets ++;
        stat->rx_broadcast_bytes += len;
    }
    else if(IEEE80211_IS_MULTICAST(addr))
    {
        wlpptr->stats_info.rx_multicast++;
        wlpptr->stats_info.rx_multicast_bytes += len;
        stat->rx_multicast_packets ++;
        stat->rx_multicast_bytes += len;
    }
    else
    {
        wlpptr->stats_info.rx_unicast++;
        wlpptr->stats_info.rx_unicast_bytes += len;
    }
        
    wlpptr->stats_info.rx_packets++;
    wlpptr->stats_info.rx_bytes += len;
    wlpptr->stats_info.rx_sum_packets++;
    wlpptr->stats_info.rx_sum_bytes += len;
    stat->rx_packets++;
    stat->rx_bytes += len;

    return 0;
}


int Statistics_Recv_Mgmt_Packet(vmacApInfo_t *vmacSta_p,UINT32 len)
{
    struct wlprivate *wlpptr = NULL;
    vmacApInfo_t  *vmac_ap = NULL;

    if(vmacSta_p == NULL)
    {
        return -1;
    }

    wlpptr = NETDEV_PRIV_P(struct wlprivate, vmacSta_p->dev);
    /*****ath****/        

    wlpptr->stats_info.rx_sum_packets++;
    wlpptr->stats_info.rx_sum_bytes += len;
    wlpptr->stats_info.rx_mgmt++;
    wlpptr->stats_info.rx_mgmt_bytes += len;
        
    if(vmacSta_p->master)
        vmac_ap = vmacSta_p->master;
    else
        return 0;

    /*****wifi****/
    wlpptr = NETDEV_PRIV_P(struct wlprivate, vmac_ap->dev); 

    wlpptr->stats_info.rx_mgmt++;
    wlpptr->stats_info.rx_mgmt_bytes += len;
    wlpptr->stats_info.rx_sum_packets++;
    wlpptr->stats_info.rx_sum_bytes += len;

    return 0;
}


int Statistics_Recv_Ctl_Packet(struct net_device *netdev,UINT32 len)
{
    struct wlprivate *wlpptr = NETDEV_PRIV_P(struct wlprivate, netdev);
    vmacApInfo_t *vmacSta_p;
    vmacApInfo_t  *vmac_ap;

    if(wlpptr == NULL)
        return -1;

    /*****ath****/        
    vmacSta_p = wlpptr->vmacSta_p;

    wlpptr->stats_info.rx_sum_packets++;
    wlpptr->stats_info.rx_sum_bytes += len;
    wlpptr->stats_info.rx_ctrl++;
    wlpptr->stats_info.rx_ctrl_bytes+= len;

    if(vmacSta_p->master)
        vmac_ap = vmacSta_p->master;
    else
        return 0;

    /*****wifi****/
    wlpptr = NETDEV_PRIV_P(struct wlprivate, vmac_ap->dev); 

    wlpptr->stats_info.rx_ctrl++;
    wlpptr->stats_info.rx_ctrl_bytes+= len;
    wlpptr->stats_info.rx_sum_packets++;
    wlpptr->stats_info.rx_sum_bytes += len;

    return 0;
}


int Statistics_TX_Data_Packet(struct net_device *netdev,UINT8 bcast,UINT32 len,UINT8 count)
{
    struct wlprivate *wlpptr = NETDEV_PRIV_P(struct wlprivate, netdev);
    struct net_device_stats  *stat;
    vmacApInfo_t *vmacSta_p;
    vmacApInfo_t  *vmac_ap;

    if(wlpptr == NULL)
        return -1;

    /*****ath****/        
    vmacSta_p = wlpptr->vmacSta_p;
    stat = &(wlpptr->netDevStats);

    if(bcast == 2)      //broadcase
    {
        wlpptr->stats_info.tx_broadcast += count;
        wlpptr->stats_info.tx_broadcast_bytes += len;
        stat->tx_broadcast_packets += count;
        stat->tx_broadcast_bytes += len;
    }
    else if(bcast == 1)     //multicast
    {
        wlpptr->stats_info.tx_multicast += count;
        wlpptr->stats_info.tx_multicast_bytes+= len;
        stat->tx_multicast_packets += count;
        stat->tx_multicast_bytes += len;
    }
    else if(bcast == 0)     //unicast
    {
        wlpptr->stats_info.tx_unicast += count;
        wlpptr->stats_info.tx_unicast_bytes += len;
    }
        
    wlpptr->stats_info.tx_sum_packets += count;
    wlpptr->stats_info.tx_sum_bytes += len;
    wlpptr->stats_info.tx_packets += count;
    wlpptr->stats_info.tx_bytes += len;
    stat->tx_packets += count;
    stat->tx_bytes += len;

    if(vmacSta_p->master)
        vmac_ap = vmacSta_p->master;
    else
        return 0;

    /*****wifi****/
    wlpptr = NETDEV_PRIV_P(struct wlprivate, vmac_ap->dev); 
    stat = &(wlpptr->netDevStats);

    if(bcast == 2)      //broadcase
    {
        wlpptr->stats_info.tx_broadcast += count;
        wlpptr->stats_info.tx_broadcast_bytes += len;
        stat->tx_broadcast_packets += count;
        stat->tx_broadcast_bytes += len;
    }
    else if(bcast == 1)     //multicast
    {
        wlpptr->stats_info.tx_multicast += count;
        wlpptr->stats_info.tx_multicast_bytes+= len;
        stat->tx_multicast_packets += count;
        stat->tx_multicast_bytes += len;
    }
    else if(bcast == 0)     //unicast
    {
        wlpptr->stats_info.tx_unicast += count;
        wlpptr->stats_info.tx_unicast_bytes += len;
    }
        
    wlpptr->stats_info.tx_sum_packets += count;
    wlpptr->stats_info.tx_sum_bytes += len;
    wlpptr->stats_info.tx_packets += count;
    wlpptr->stats_info.tx_bytes += len;
    stat->tx_packets += count;
    stat->tx_bytes += len;

    return 0;
}


int Statistics_TX_Mgmt_Packet(struct net_device *netdev,UINT8 bcast,UINT32 len)
{
    struct wlprivate *wlpptr = NETDEV_PRIV_P(struct wlprivate, netdev);
    struct net_device_stats  *stat;
    vmacApInfo_t *vmacSta_p;
    vmacApInfo_t  *vmac_ap;

    if(wlpptr == NULL)
        return -1;

    /*****ath****/        
    vmacSta_p = wlpptr->vmacSta_p;
    stat = &(wlpptr->netDevStats);
        
    if(bcast == 2)      //broadcase
    {
        wlpptr->stats_info.tx_broadcast ++;
        wlpptr->stats_info.tx_broadcast_bytes += len;
        stat->tx_broadcast_packets ++;
        stat->tx_broadcast_bytes += len;
    }
    else if(bcast == 1)     //multicast
    {
        wlpptr->stats_info.tx_multicast ++;
        wlpptr->stats_info.tx_multicast_bytes+= len;
        stat->tx_multicast_packets ++;
        stat->tx_multicast_bytes += len;
    }
    else if(bcast == 0)     //unicast
    {
        wlpptr->stats_info.tx_unicast ++;
        wlpptr->stats_info.tx_unicast_bytes += len;
    }
        
    wlpptr->stats_info.tx_sum_packets ++;
    wlpptr->stats_info.tx_sum_bytes += len;
    wlpptr->stats_info.tx_mgmt++;
    wlpptr->stats_info.tx_mgmt_bytes+= len;
        
    if(vmacSta_p->master)
        vmac_ap = vmacSta_p->master;
    else
        return 0;

    /*****wifi****/
    wlpptr = NETDEV_PRIV_P(struct wlprivate, vmac_ap->dev); 
    stat = &(wlpptr->netDevStats);
        
    if(bcast == 2)      //broadcase
    {
        wlpptr->stats_info.tx_broadcast ++;
        wlpptr->stats_info.tx_broadcast_bytes += len;
        stat->tx_broadcast_packets ++;
        stat->tx_broadcast_bytes += len;
    }
    else if(bcast == 1)     //multicast
    {
        wlpptr->stats_info.tx_multicast ++;
        wlpptr->stats_info.tx_multicast_bytes+= len;
        stat->tx_multicast_packets ++;
        stat->tx_multicast_bytes += len;
    }
    else if(bcast == 0)     //unicast
    {
        wlpptr->stats_info.tx_unicast ++;
        wlpptr->stats_info.tx_unicast_bytes += len;
    }

    wlpptr->stats_info.tx_sum_packets ++;
    wlpptr->stats_info.tx_sum_bytes += len;
    wlpptr->stats_info.tx_mgmt++;
    wlpptr->stats_info.tx_mgmt_bytes+= len;

    return 0;
}


int Statistics_TX_Error_Packet(struct net_device *netdev,UINT8 count)
{
    struct wlprivate *wlpptr = NETDEV_PRIV_P(struct wlprivate, netdev);
    vmacApInfo_t *vmacSta_p;
    vmacApInfo_t  *vmac_ap;

    if(wlpptr == NULL)
        return -1;

    /*****ath****/        
    vmacSta_p = wlpptr->vmacSta_p;

    wlpptr->stats_info.tx_errors += count;

    if(vmacSta_p->master)
        vmac_ap = vmacSta_p->master;
    else
        return 0;

    /*****wifi****/
    wlpptr = NETDEV_PRIV_P(struct wlprivate, vmac_ap->dev); 

    wlpptr->stats_info.tx_errors += count;
    wlpptr->netDevStats.tx_errors += count;

    return 0;
}


int Statistics_TX_Drop_Packet(struct net_device *netdev)
{
    struct wlprivate *wlpptr = NETDEV_PRIV_P(struct wlprivate, netdev);
    vmacApInfo_t *vmacSta_p;
    vmacApInfo_t  *vmac_ap;

    if(wlpptr == NULL)
        return -1;

    /*****ath****/        
    vmacSta_p = wlpptr->vmacSta_p;

    wlpptr->stats_info.tx_dropped++;
        
    if(vmacSta_p->master)
        vmac_ap = vmacSta_p->master;
    else
        return 0;

    /*****wifi****/
    wlpptr = NETDEV_PRIV_P(struct wlprivate, vmac_ap->dev); 

    wlpptr->stats_info.tx_dropped++;
    wlpptr->netDevStats.tx_dropped ++;

    return 0;
}


int Statistics_Disassoc_Errors(struct net_device *netdev,UINT16 flag)
{
    struct wlprivate *wlpptr = NETDEV_PRIV_P(struct wlprivate, netdev);
    vmacApInfo_t *vmacSta_p;
    vmacApInfo_t  *vmac_ap;

    if(wlpptr == NULL)
        return -1;

    /*****ath****/        
    vmacSta_p = wlpptr->vmacSta_p;

    switch(flag)
    {
        case IEEEtypes_STATUS_UNSPEC_FAILURE:
        {
            wlpptr->stats_info.is_disassoc_unnormal++;
            break;
        }

        case IEEEtypes_STATUS_ASSOC_DENIED_RATES:
        {
            wlpptr->stats_info.is_rx_assoc_norate++;
            break;
        }        

        case IEEEtypes_STATUS_CAPS_UNSUPPORTED:
        {
            wlpptr->stats_info.is_rx_assoc_capmismatch++;
            break;
        }

        case IEEEtypes_STATUS_REASSOC_NO_ASSOC:
        {
            wlpptr->stats_info.is_rx_bad_assoc++;
            break;
        }        

        case IEEEtypes_STATUS_ASSOC_DENIED_UNSPEC:
        {
            wlpptr->stats_info.is_rx_assoc_bss++;
            break;
        }        

        case IEEEtypes_STATUS_AUTH_TIMEOUT:
        {
            wlpptr->stats_info.is_rx_assoc_notauth++;
            break;
        }        

        case IEEEtypes_REASON_INVALID_IE:
        {
            wlpptr->stats_info.is_rx_assoc_badwpaie++;
            break;
        }        

        default:
            break;
    }

    if(vmacSta_p->master)
        vmac_ap = vmacSta_p->master;
    else
        return 0;

    /*****wifi****/
    wlpptr = NETDEV_PRIV_P(struct wlprivate, vmac_ap->dev); 

    switch(flag)
    {
        case IEEEtypes_STATUS_UNSPEC_FAILURE:
        {
            wlpptr->stats_info.is_disassoc_unnormal++;
            break;
        }

        case IEEEtypes_STATUS_ASSOC_DENIED_RATES:
        {
            wlpptr->stats_info.is_rx_assoc_norate++;
            break;
        }        

        case IEEEtypes_STATUS_CAPS_UNSUPPORTED:
        {
            wlpptr->stats_info.is_rx_assoc_capmismatch++;
            break;
        }        

        case IEEEtypes_STATUS_REASSOC_NO_ASSOC:
        {
            wlpptr->stats_info.is_rx_bad_assoc++;
            break;
        }        

        case IEEEtypes_STATUS_ASSOC_DENIED_UNSPEC:
        {
            wlpptr->stats_info.is_rx_assoc_bss++;
            break;
        }        

        case IEEEtypes_STATUS_AUTH_TIMEOUT:
        {
            wlpptr->stats_info.is_rx_assoc_notauth++;
            break;
        }        

        case IEEEtypes_REASON_INVALID_IE:
        {
            wlpptr->stats_info.is_rx_assoc_badwpaie++;
            break;        
        }        

        default:
            break;
    }

    return 0;
}


int ieee80211_ioctl_getathstats(struct net_device *netdev,char *ret_str, UINT16 *ret_len)
{
    struct wlprivate *wlpptr = NETDEV_PRIV_P(struct wlprivate, netdev);
    struct ath_stats total;
    struct HWstats buff;

    memset(&buff,0,sizeof(struct HWstats));
    memset(&total,0,sizeof(struct ath_stats));

    total.ast_tx_mgmt = wlpptr->stats_info.tx_mgmt;
    total.ast_rx_mgt = wlpptr->stats_info.rx_mgmt;
    total.ast_tx_packets = wlpptr->stats_info.tx_packets;
    total.ast_tx_xretries = wlpptr->stats_info.rx_retry_bytes;

    wlFwGetHwStats(netdev,NULL,&buff);

    total.ast_rx_crcerr = wlpptr->netDevStats.rx_crc_errors;
    total.ast_rx_badcrypt = wlpptr->netDevStats.rx_errors;
    //total.ast_rx_badcrypt = wlpptr->stats_info.ast_rx_badcrypt;
    total.ast_rx_badmic = buff.RxWAPIMICErrors;
    total.ast_rx_phyerr = buff.RxErrors;

    *ret_len = sizeof(struct ath_stats);

    if (copy_to_user(ret_str,&total,*ret_len))
    {
        return -EFAULT;
    }
        
    return 0;        
}


int ieee80211_ioctl_get_ieee80211_stats(struct net_device *netdev,char *ret_str, UINT16 *ret_len)
{
    struct wlprivate *wlpptr = NETDEV_PRIV_P(struct wlprivate, netdev);
    struct ieee80211_stats stats;
    struct HWstats buff;

    memset(&buff,0,sizeof(struct HWstats));
    memset(&stats,0,sizeof(struct ieee80211_stats));

    stats.is_rx_mgmt = wlpptr->stats_info.rx_mgmt;
    stats.is_tx_mgmt = wlpptr->stats_info.tx_mgmt;
    stats.is_rx_mgmtbytes = wlpptr->stats_info.rx_mgmt_bytes;
    stats.is_tx_mgmtbytes = wlpptr->stats_info.tx_mgmt_bytes;

    wlFwGetHwStats(netdev,NULL,&buff);

    stats.is_re_wpi = buff.RxWAPIPNErrors;   //Uncertain!
    stats.is_wpi_no_key_error = buff.WAPINoKeyErrors;   //Uncertain!
    stats.is_wpi_mic = buff.RxWAPIMICErrors;

    stats.is_disassoc_unnormal = wlpptr->stats_info.is_disassoc_unnormal;
    stats.is_rx_assoc_norate = wlpptr->stats_info.is_rx_assoc_norate;
    stats.is_rx_assoc_capmismatch = wlpptr->stats_info.is_rx_assoc_capmismatch;
    stats.is_rx_bad_assoc = wlpptr->stats_info.is_rx_bad_assoc;
    stats.is_rx_assoc_bss = wlpptr->stats_info.is_rx_assoc_bss;
    stats.is_rx_assoc_notauth = wlpptr->stats_info.is_rx_assoc_notauth;
    stats.is_rx_assoc_badwpaie = wlpptr->stats_info.is_rx_assoc_badwpaie;

    *ret_len = sizeof(struct ieee80211_stats);

    if (copy_to_user(ret_str,&stats,*ret_len))
    {
        return -EFAULT;
    }
        
    return 0;
}

