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
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/net.h>
#include <linux/netdevice.h>
#include <linux/wireless.h>
#include <linux/pci.h>
#include <linux/delay.h>
#include <linux/string.h>
#include <linux/ipv6.h>

#include <net/iw_handler.h>
#include <asm/processor.h>
#include <asm/uaccess.h>

#include "wl.h" 
#include "wldebug.h"
#include "ap8xLnxApi.h"
#include "ap8xLnxVer.h"
#include "IEEE_types.h"
#include "wl_mib.h"
#include "wl_hal.h"
#include "ap8xLnxFwcmd.h"
#include "ap8xLnxWlLog.h"
#include "wlApi.h"
#include "qos.h"
#include "ap8xLnxIoctl.h"
#include "ap8xLnxFwdl.h"
#include "StaDb.h"
//#include "domain.h" // Added by ARUN to support 802.11d
//#include "wlvmac.h"
//#include "macmgmtap.h"
//#include "macMgmtMlme.h"
//#include "idList.h"
//#include "keyMgmtSta.h"
//#include "bcngen.h"
//#include "wlFun.h"
#ifdef EWB
#include "ewb_hash.h"
#endif

#ifdef WDS_FEATURE
#include "wds.h"
#endif

#ifdef CLIENT_SUPPORT
#include "linkmgt.h"
#include "mlme.h"
#include "mlmeApi.h"
#endif

#ifdef MPRXY
#include "ap8xLnxMPrxy.h"
#endif

#include "ap8xLnxIntf.h"
#include "ap8xLnxWmmTraffic.h"
#include "wllog.h"
#include "ieee80211_autelan_limit.h"
#include "ap8xLnxThinap.h"
#include "ap8xLnxXmit.h"

//add begin for autelan wmm,traffic_limit,etc. Moved from aquila//lhj//2012.06

extern struct timeval current_time;
extern u_int32_t au_pkt;
extern u_int32_t pkt_next;
extern u_int32_t drop_pkt;

extern u_int32_t ls_s[3];
extern u_int32_t ls_us[3];
extern u_int32_t cur_s[3];
extern u_int32_t cur_us[3];
extern u_int32_t elapsed[3];
extern u_int32_t reqd[3];
extern u_int32_t drop[3];
extern u_int32_t g_pkt[3];
extern u_int32_t rx_reqd[3];

/*Begin:added by duanmingzhe for 80211 cache*/
int ieee80211_ioctl_autelan_wmm(struct net_device *netdev, char *param_str, char *ret_str)
{
    int ret = 0;
    struct wlprivate *priv = NETDEV_PRIV_P(struct wlprivate, netdev);
    struct ieee80211vap *vap = &(priv->vap); 
    //printk("-------------------ieee80211_ioctl_autelan_wmm(), %s-------------------\n", priv->netDev->name);//lhj//2012.06.12
    struct ieee80211_autelan_wmm_params ik;
    memset(&ik, 0, sizeof(ik));
    memcpy(&ik, param_str, sizeof(ik)); //sizeof(ik)!! dont use param_len
    switch (ik.type) {
        case TOS_FLAG:
            (vap->priv_wmm).tos_flag = ik.arg1;
            break;

        case GET_TOS_FLAG :
            //printk("(vap->priv_wmm).tos_flag %d-%p\n", (vap->priv_wmm).tos_flag, &((vap->priv_wmm).tos_flag));    //lhj//2012.06
            ret = copy_to_user(ret_str, &((vap->priv_wmm).tos_flag), sizeof((vap->priv_wmm).tos_flag));
            break;

        case TOS_INGRESS :
#ifdef AE5200_RT_SUPPORT
			if ( ik.arg1 < 0 || ik.arg1 > 7) {//Modified by zhaoyang1 for optimize map of tos vlan & 80211e up 2013-01-28
				return -EFAULT;
			} else {
		
				(vap->priv_wmm).tos_ingress_priority[ik.arg1] = ik.arg2;
			}		
			break;
#else
            
            if ( ik.arg1 < 0 || ik.arg1 > 3) {
                return -EFAULT;
            } else {
        
                (vap->priv_wmm).tos_ingress_priority[ik.arg1] = ik.arg2;
            }       
            break;
#endif
        case GET_TOS_INGRESS :
            ret = copy_to_user(ret_str, (vap->priv_wmm).tos_ingress_priority, sizeof((vap->priv_wmm).tos_ingress_priority));
            break;

        case TOS_EGRESS :
#ifdef AE5200_RT_SUPPORT

			if (ik.arg1 < 0 || ik.arg1 > 7) {//Modified by zhaoyang1 for optimize map of tos vlan & 80211e up 2013-01-28
				
				return  -EFAULT;
			} else {
				
				(vap->priv_wmm).tos_egress_priority[ik.arg2] = ik.arg1;//Modified by zhaoyang1 for optimize map of tos vlan & 80211e up 2013-01-28
			}
			break;

#else
            if (ik.index < 0 || ik.index > 7) {
                
                return  -EFAULT;
            } else {
                
                (vap->priv_wmm).tos_egress_priority[ik.index*2] = ik.arg2;

                (vap->priv_wmm).tos_egress_priority[ik.index*2 +1] = ik.arg1;
            }
            break;
#endif
        case GET_TOS_EGRESS :
            ret = copy_to_user(ret_str, (vap->priv_wmm).tos_egress_priority, sizeof((vap->priv_wmm).tos_egress_priority));
            break;

        case VLAN_FLAG:
            (vap->priv_wmm).vlan_flag = ik.arg1;
            break;

        case GET_VLAN_FLAG :
            ret = copy_to_user(ret_str, &((vap->priv_wmm).vlan_flag), sizeof((vap->priv_wmm).vlan_flag));
            break;

        case VLAN_INGRESS :
#ifdef AE5200_RT_SUPPORT
			if ( ik.arg1 < 0 || ik.arg1 > 7 || ik.arg2 < 0 || ik.arg2 > 7) {//Modified by zhaoyang1 for optimize map of tos vlan & 80211e up 2013-01-28
				return -EFAULT;
			} else {
				((vap->priv_wmm).vlan_ingress_priority[ik.arg1]) = ik.arg2;
			}
			break;
#else
            if ( ik.arg1 < 0 || ik.arg1 > 3 || ik.arg2 > 7) {
                return -EFAULT;
            } else {
                ((vap->priv_wmm).vlan_ingress_priority[ik.arg1]) = ik.arg2;
            }
            break;
#endif
        case GET_VLAN_INGRESS :
            ret = copy_to_user(ret_str, (vap->priv_wmm).vlan_ingress_priority, sizeof((vap->priv_wmm).vlan_ingress_priority));
            break;

        case VLAN_EGRESS :

            if ( ik.arg1 < 0 || ik.arg2 > 7) {
                return -EFAULT;
            } else {
                ((vap->priv_wmm).vlan_egress_priority[ik.arg2]) = ik.arg1;
            }
            break;

        case GET_VLAN_EGRESS:
            ret = copy_to_user(ret_str, (vap->priv_wmm).vlan_egress_priority, sizeof((vap->priv_wmm).vlan_egress_priority));
            break;

        /*ljy--add begin to append the switch modified the best effort parameter*/
        case BE_FLAG:
            (vap->priv_wmm).be_flag = ik.arg1;
            break;

        case GET_BE_FLAG :
            ret = copy_to_user(ret_str, &((vap->priv_wmm).be_flag), sizeof((vap->priv_wmm).be_flag));
            break;
        /*ljy--add end*/

        default :
            return 0;
    }
    return ret;
}
/*pengruofeng--add begin for autelan private wmm 2011-6-1*/
void autelan_wmm_init(struct ieee80211vap *vap) 
{

    if (vap == NULL) {
        return;     
    }
    memset(&(vap->priv_wmm), 0x00, sizeof(struct autelan_wmm));

    (vap->priv_wmm).tos_flag = 0;
#ifdef AE5200_RT_SUPPORT
	 /*Begin:Modified by zhaoyang1 for optimize map of tos vlan & 80211e up 2013-01-28*/
	(vap->priv_wmm).tos_egress_priority[0] = 0;
	(vap->priv_wmm).tos_egress_priority[1] = 0;
	(vap->priv_wmm).tos_egress_priority[2] = 0;
	(vap->priv_wmm).tos_egress_priority[3] = 0;
	(vap->priv_wmm).tos_egress_priority[4] = 0;
	(vap->priv_wmm).tos_egress_priority[5] = 0;
	(vap->priv_wmm).tos_egress_priority[6] = 0;
	(vap->priv_wmm).tos_egress_priority[7] = 0;
	(vap->priv_wmm).tos_egress_priority[8] = 1;
	(vap->priv_wmm).tos_egress_priority[9] = 1;
	(vap->priv_wmm).tos_egress_priority[10] = 2;
	(vap->priv_wmm).tos_egress_priority[11] = 2;
	(vap->priv_wmm).tos_egress_priority[12] = 2;
	(vap->priv_wmm).tos_egress_priority[13] = 2;
	(vap->priv_wmm).tos_egress_priority[14] = 2;
	(vap->priv_wmm).tos_egress_priority[15] = 2;
	(vap->priv_wmm).tos_egress_priority[16] = 3;
	(vap->priv_wmm).tos_egress_priority[17] = 3;
	(vap->priv_wmm).tos_egress_priority[18] = 3;
	(vap->priv_wmm).tos_egress_priority[19] = 3;
	(vap->priv_wmm).tos_egress_priority[20] = 3;
	(vap->priv_wmm).tos_egress_priority[21] = 3;
	(vap->priv_wmm).tos_egress_priority[22] = 3;
	(vap->priv_wmm).tos_egress_priority[23] = 3;
	(vap->priv_wmm).tos_egress_priority[24] = 4;
	(vap->priv_wmm).tos_egress_priority[25] = 4;
	(vap->priv_wmm).tos_egress_priority[26] = 4;
	(vap->priv_wmm).tos_egress_priority[27] = 4;
	(vap->priv_wmm).tos_egress_priority[28] = 4;
	(vap->priv_wmm).tos_egress_priority[29] = 4;
	(vap->priv_wmm).tos_egress_priority[30] = 4;
	(vap->priv_wmm).tos_egress_priority[31] = 4;
	(vap->priv_wmm).tos_egress_priority[32] = 5;
	(vap->priv_wmm).tos_egress_priority[33] = 5;
	(vap->priv_wmm).tos_egress_priority[34] = 5;
	(vap->priv_wmm).tos_egress_priority[35] = 5;
	(vap->priv_wmm).tos_egress_priority[36] = 5;
	(vap->priv_wmm).tos_egress_priority[37] = 5;
	(vap->priv_wmm).tos_egress_priority[38] = 5;
	(vap->priv_wmm).tos_egress_priority[39] = 5;
	(vap->priv_wmm).tos_egress_priority[40] = 5;
	(vap->priv_wmm).tos_egress_priority[41] = 5;
	(vap->priv_wmm).tos_egress_priority[42] = 5;
	(vap->priv_wmm).tos_egress_priority[43] = 5;
	(vap->priv_wmm).tos_egress_priority[44] = 5;
	(vap->priv_wmm).tos_egress_priority[45] = 5;
	(vap->priv_wmm).tos_egress_priority[46] = 6;
	(vap->priv_wmm).tos_egress_priority[47] = 6;
	(vap->priv_wmm).tos_egress_priority[48] = 7;
	(vap->priv_wmm).tos_egress_priority[49] = 7;
	(vap->priv_wmm).tos_egress_priority[50] = 7;
	(vap->priv_wmm).tos_egress_priority[51] = 7;
	(vap->priv_wmm).tos_egress_priority[52] = 7;
	(vap->priv_wmm).tos_egress_priority[53] = 7;
	(vap->priv_wmm).tos_egress_priority[54] = 7;
	(vap->priv_wmm).tos_egress_priority[55] = 7;
	(vap->priv_wmm).tos_egress_priority[56] = 7;
	(vap->priv_wmm).tos_egress_priority[57] = 7;
	(vap->priv_wmm).tos_egress_priority[58] = 7;
	(vap->priv_wmm).tos_egress_priority[59] = 7;
	(vap->priv_wmm).tos_egress_priority[60] = 7;
	(vap->priv_wmm).tos_egress_priority[61] = 7;
	(vap->priv_wmm).tos_egress_priority[62] = 7;
	(vap->priv_wmm).tos_egress_priority[63] = 7;
		
	(vap->priv_wmm).tos_ingress_priority[0] = 0x00 >> 2;
	(vap->priv_wmm).tos_ingress_priority[1] = 0x20 >> 2;
	(vap->priv_wmm).tos_ingress_priority[2] = 0x28 >> 2;
	(vap->priv_wmm).tos_ingress_priority[3] = 0x40 >> 2;
	(vap->priv_wmm).tos_ingress_priority[4] = 0x60 >> 2;
	(vap->priv_wmm).tos_ingress_priority[5] = 0x80 >> 2;
	(vap->priv_wmm).tos_ingress_priority[6] = 0xB8 >> 2;
	(vap->priv_wmm).tos_ingress_priority[7] = 0xC0 >> 2;

	(vap->priv_wmm).vlan_flag = 0;

	(vap->priv_wmm).vlan_ingress_priority[0] = 0;
	(vap->priv_wmm).vlan_ingress_priority[1] = 1;
	(vap->priv_wmm).vlan_ingress_priority[2] = 2;
	(vap->priv_wmm).vlan_ingress_priority[3] = 3;
	(vap->priv_wmm).vlan_ingress_priority[4] = 4;
	(vap->priv_wmm).vlan_ingress_priority[5] = 5;
	(vap->priv_wmm).vlan_ingress_priority[6] = 6;
	(vap->priv_wmm).vlan_ingress_priority[7] = 7;

	(vap->priv_wmm).vlan_egress_priority[0] = 0;
	(vap->priv_wmm).vlan_egress_priority[1] = 1;
	(vap->priv_wmm).vlan_egress_priority[2] = 2;
	(vap->priv_wmm).vlan_egress_priority[3] = 3;
	(vap->priv_wmm).vlan_egress_priority[4] = 4;
	(vap->priv_wmm).vlan_egress_priority[5] = 5;
	(vap->priv_wmm).vlan_egress_priority[6] = 6;
	(vap->priv_wmm).vlan_egress_priority[7] = 7;
	 /*End:Modified by zhaoyang1 for optimize map of tos vlan & 80211e up 2013-01-28*/
#else
    (vap->priv_wmm).tos_egress_priority[0] = 0x08;
    (vap->priv_wmm).tos_egress_priority[1] = 1;
    (vap->priv_wmm).tos_egress_priority[2] = 0x20;
    (vap->priv_wmm).tos_egress_priority[3] = 1;
    (vap->priv_wmm).tos_egress_priority[4] = 0x28;
    (vap->priv_wmm).tos_egress_priority[5] = 2;
    (vap->priv_wmm).tos_egress_priority[6] = 0x30;
    (vap->priv_wmm).tos_egress_priority[7] = 3;
    (vap->priv_wmm).tos_egress_priority[8] = 0x88;
    (vap->priv_wmm).tos_egress_priority[9] = 3;
    (vap->priv_wmm).tos_egress_priority[10] = 0xa0;
    (vap->priv_wmm).tos_egress_priority[11] = 2;
    (vap->priv_wmm).tos_egress_priority[12] = 0xb8;
    (vap->priv_wmm).tos_egress_priority[13] = 3;
    (vap->priv_wmm).tos_egress_priority[14] = 0xe0;
    (vap->priv_wmm).tos_egress_priority[15] = 3;
        
    (vap->priv_wmm).tos_ingress_priority[0] = 0x00;
    (vap->priv_wmm).tos_ingress_priority[1] = 0x20;
    (vap->priv_wmm).tos_ingress_priority[2] = 0xa0;
    (vap->priv_wmm).tos_ingress_priority[3] = 0x88;


    (vap->priv_wmm).vlan_flag = 0;

    (vap->priv_wmm).vlan_ingress_priority[0] = 0;
    (vap->priv_wmm).vlan_ingress_priority[1] = 1;
    (vap->priv_wmm).vlan_ingress_priority[2] = 4;
    (vap->priv_wmm).vlan_ingress_priority[3] = 6;

    (vap->priv_wmm).vlan_egress_priority[0] = 0;
    (vap->priv_wmm).vlan_egress_priority[1] = 1;
    (vap->priv_wmm).vlan_egress_priority[2] = 1;
    (vap->priv_wmm).vlan_egress_priority[3] = 0;
    (vap->priv_wmm).vlan_egress_priority[4] = 2;
    (vap->priv_wmm).vlan_egress_priority[5] = 2;
    (vap->priv_wmm).vlan_egress_priority[6] = 3;
    (vap->priv_wmm).vlan_egress_priority[7] = 3;
#endif
    (vap->priv_wmm).be_flag = 1;    /*ljy--add to append the switch modified the best effort parameter*/

    return;
}
/*pengruofeng--add end 2011-6-1*/

/*pengruofeng--add begin for autelan private wmm 2011-6-1*/
/*
 * this function turn vlan priority to wmm ptiority
 */
int vlan_to_wmm_map (int vlan_priority, struct ieee80211vap *vap)
{
    if (vlan_priority > 7 || vlan_priority < 0) {
        return 1;       //reture 0-->1//lhj//2012.06
    }
    return (vap->priv_wmm).vlan_egress_priority[vlan_priority];
}

/*
 * this function turn tos to wmm priority
 */
int tos_to_wmm_map (int tos_priority, struct ieee80211vap *vap) 
{

#ifdef AE5200_RT_SUPPORT
    
    tos_priority = tos_priority << 5;
    tos_priority = tos_priority >> 2;

	if (tos_priority > 63 || tos_priority < 0) {
		return 0;
	}
	return (vap->priv_wmm).tos_egress_priority[tos_priority];

#else
    int i;
    for (i = 0; i < 16; i += 2) {
        if ((vap->priv_wmm).tos_egress_priority[i] == (tos_priority<<5) ) {
            return (vap->priv_wmm).tos_egress_priority[i+1];
        }
    }
    return 1;       //reture 0-->1//lhj//2012.06
#endif
}
/*
 * this function used to turn wmm priority to 1p priority.
 */
int wmm_to_vlan_map(int wmm_priority, struct ieee80211vap *vap) 
{
#ifdef AE5200_RT_SUPPORT
	if (wmm_priority > 7 || wmm_priority < 0 || vap == NULL) {//Begin:Modified by zhaoyang1 for optimize map of tos vlan & 80211e up 2013-01-28
		return 0;
	}
	return (vap->priv_wmm).vlan_ingress_priority[wmm_priority];
#else
    if (wmm_priority > 3 || wmm_priority < 0 || vap == NULL) {
        return 0;
    }
    return (vap->priv_wmm).vlan_ingress_priority[wmm_priority];
#endif
}
/*
 * this function turn tos to wmm priority
 */
int wmm_to_tos_map(int wmm_priority, struct ieee80211vap *vap) 
{
#ifdef AE5200_RT_SUPPORT

	if (wmm_priority > 7 || wmm_priority < 0 || vap == NULL) {//Begin:Modified by zhaoyang1 for optimize map of tos vlan & 80211e up 2013-01-28*/
		return 0;
	}
	return (vap->priv_wmm).tos_ingress_priority[wmm_priority];

#else
    if (wmm_priority > 3 || wmm_priority < 0 || vap == NULL) {
        return 0;
    }
    return (((vap->priv_wmm).tos_ingress_priority[wmm_priority])>>5);
#endif
}
/*pengruofeng--add end 2011-6-1*/

int ieee80211_ioctl_autelan_traffic_limit(struct net_device *dev, char *param_str, char *ret_str)
{
    int ret = 0;
    struct wlprivate *priv = NETDEV_PRIV_P(struct wlprivate, dev);
    struct ieee80211vap *vap = &(priv->vap); 
    vmacApInfo_t *vmacSta_p = priv->vmacSta_p;
    extStaDb_StaInfo_t *ni = NULL;
    unsigned int rate = 0;
    struct ieee80211_autelan_traffic_limit ik;

    memset(&ik, 0x00, sizeof(ik));
    memcpy(&ik, param_str, sizeof(ik)); //sizeof(ik)!! dont use param_len

    switch (ik.type) {
        case SET_VAP_TRAFFIC_LIMIT_FLAG:
            //vap->vap_traffic_limit = ik.arg1;
            vap->iv_tl.traffic_limit_flag = ik.arg1;
            break;
        case GET_VAP_TRAFFIC_LIMIT_FLAG:
            ret = copy_to_user(ret_str, &(vap->iv_tl.traffic_limit_flag), sizeof(vap->iv_tl.traffic_limit_flag));
            break;
        case SET_VAP_TRAFFIC_LIMIT:
            //vap->vap_packet_max_number = ik.arg1 * 1024 / 8;
            vap->iv_tl.rx_traffic_limit = ik.arg1;
            if(ik.arg1)
                vap->iv_tl.rx_rate_in_us_per_byte = (1024 * (CAR_SCALE_FACTOR * 8))/(vap->iv_tl.rx_traffic_limit);
            break;
        case GET_VAP_TRAFFIC_LIMIT :
            //rate = (vap->vap_packet_max_number) * 8 / 1024;
            rate = vap->iv_tl.rx_traffic_limit;
            ret = copy_to_user(ret_str, &rate, sizeof(rate));
            break;
        case SET_VAP_TRAFFIC_LIMIT_SEND:
            //vap->vap_packet_max_number_send = ik.arg1 * 1024 / 8;         
            vap->iv_tl.tx_traffic_limit = ik.arg1;
            if(ik.arg1)
                vap->iv_tl.tx_rate_in_us_per_byte = (1024* (CAR_SCALE_FACTOR * 8))/(vap->iv_tl.tx_traffic_limit);
            break;
        case GET_VAP_TRAFFIC_LIMIT_SEND:
            rate = (vap->iv_tl.tx_traffic_limit);
            ret = copy_to_user(ret_str, &rate, sizeof(rate));
            break;

        case SET_SPECIFIC_NODE_TRAFFIC_LIMIT_FLAG:
            ni = extStaDb_GetStaInfo(vmacSta_p, (IEEEtypes_MacAddr_t *)ik.macaddr, 1);
            if (ni == NULL)
            {
                printk("%s ni is null.\n",__func__);
                return -EINVAL;
            }
            ni->ni_special_tl.traffic_limit_flag = ik.arg1;
            //printk("%s ni->ni_special_tl.traffic_limit_flag = %d\n",__func__,ni->ni_special_tl.traffic_limit_flag);
            break;

        case  GET_SPECIFIC_NODE_TRAFFIC_LIMIT_FLAG:
            ni = extStaDb_GetStaInfo(vmacSta_p, (IEEEtypes_MacAddr_t *)ik.macaddr, 1);
            if (ni == NULL)
            {
                printk("%s ni is null.\n",__func__);
                return -EINVAL; 
            }
            ret = copy_to_user(ret_str, &(ni->ni_special_tl.traffic_limit_flag), sizeof(ni->ni_special_tl.traffic_limit_flag));
            break;

        case SET_NODE_TRAFFIC_LIMIT:
            ni = extStaDb_GetStaInfo(vmacSta_p, (IEEEtypes_MacAddr_t *)ik.macaddr, 1);
            if (ni == NULL)
                return -EINVAL; 
            //ni->ni_packet_max_number = ik.arg1 * 1024 / 8;
            ni->ni_special_tl.rx_traffic_limit = ik.arg1;
            if(ik.arg1)
                ni->ni_special_tl.rx_rate_in_us_per_byte = (1024* (CAR_SCALE_FACTOR * 8))/(ni->ni_special_tl.rx_traffic_limit);
            break;

        case GET_NODE_TRAFFIC_LIMIT :
            ni = extStaDb_GetStaInfo(vmacSta_p, (IEEEtypes_MacAddr_t *)ik.macaddr, 1);
            if (ni == NULL)
                return -EINVAL; 
            //rate = (ni->ni_packet_max_number) * 8 /1024;
            rate = ni->ni_special_tl.rx_traffic_limit;
            ret = copy_to_user(ret_str, &rate, sizeof(rate));
            break;
            
        case SET_NODE_TRAFFIC_LIMIT_SEND:
            ni = extStaDb_GetStaInfo(vmacSta_p, (IEEEtypes_MacAddr_t *)ik.macaddr, 1);
            if (ni == NULL)
                return -EINVAL; 
            //ni->ni_packet_max_number_send = ik.arg1 * 1024 / 8;
            ni->ni_special_tl.tx_traffic_limit = ik.arg1;
            if(ik.arg1)
                ni->ni_special_tl.tx_rate_in_us_per_byte = (1024* (CAR_SCALE_FACTOR * 8))/(ni->ni_special_tl.tx_traffic_limit);
            break;

        case GET_NODE_TRAFFIC_LIMIT_SEND:
            ni = extStaDb_GetStaInfo(vmacSta_p, (IEEEtypes_MacAddr_t *)ik.macaddr, 1);
            if (ni == NULL)
                return -EINVAL; 
            //rate = (ni->ni_packet_max_number_send) * 8 / 1024;
            rate = ni->ni_special_tl.tx_traffic_limit;
            ret = copy_to_user(ret_str, &rate, sizeof(rate));
            break;

        case SET_EVERY_NODE_TRAFFIC_LIMIT:
            //vap->every_node_packet_max_number = ik.arg1 * 1024 / 8;
            vap->iv_everynode_tl.rx_traffic_limit = ik.arg1;
            if(ik.arg1)
                vap->iv_everynode_tl.rx_rate_in_us_per_byte = (1024 * (CAR_SCALE_FACTOR * 8))/(vap->iv_everynode_tl.rx_traffic_limit);
            //printk("%s rx_traffic_limit = %d rx_rate_in_us_per_byte = %d\n",__func__,vap->iv_everynode_tl.rx_traffic_limit,vap->iv_everynode_tl.rx_rate_in_us_per_byte);
            break;

        case GET_EVERY_NODE_TRAFFIC_LIMIT :
            //rate = (vap->every_node_packet_max_number) * 8 / 1024;
            rate = vap->iv_everynode_tl.rx_traffic_limit;
            ret = copy_to_user(ret_str, &rate, sizeof(rate));
            au_pkt = 0;
            drop_pkt = 0;
            break;

        case SET_EVERY_NODE_TRAFFIC_LIMIT_FLAG:
            //vap->every_node_traffic_limit = ik.arg1;
            vap->iv_everynode_tl.traffic_limit_flag = ik.arg1;
            break;

        case GET_EVERY_NODE_TRAFFIC_LIMIT_FLAG:
            ret = copy_to_user(ret_str, &(vap->iv_everynode_tl.traffic_limit_flag), sizeof(vap->iv_everynode_tl.traffic_limit_flag));
            #if 0
            {
                int i = 0;
                for(i = 0;i < 3 ;i++)
                {
                    printk("<<%d>> gpkt = %d drop_pkt = %d last_sec = %d last_usec = %d cur_sec = %d cur_usec = %d us_elapsed = %d us_reqd = %d rx_reqd = %d\n",i,g_pkt[i],drop[i],ls_s[i],ls_us[i],cur_s[i],cur_us[i],elapsed[i],reqd[i],rx_reqd[0]);      
                }
            }
            #endif          
            break;

        case SET_EVERY_NODE_TRAFFIC_LIMIT_SEND:
            //vap->every_node_packet_max_number_send = ik.arg1 * 1024 / 8;
            vap->iv_everynode_tl.tx_traffic_limit = ik.arg1;
            if(ik.arg1)
                vap->iv_everynode_tl.tx_rate_in_us_per_byte = (1024* (CAR_SCALE_FACTOR * 8))/(vap->iv_everynode_tl.tx_traffic_limit);
            //printk("%s tx_traffic_limit = %d tx_rate_in_us_per_byte = %d \n",__func__,vap->iv_everynode_tl.tx_traffic_limit,vap->iv_everynode_tl.tx_rate_in_us_per_byte);
            //printk("((1024*1024) * (CAR_SCALE_FACTOR * 8)) = %d (vap->iv_tl.tx_traffic_limit * 1024) = %d\n",((1024*1024) * (CAR_SCALE_FACTOR * 8)),(vap->iv_everynode_tl.tx_traffic_limit * 1024));
            break;

        case GET_EVERY_NODE_TRAFFIC_LIMIT_SEND:
            //rate = (vap->every_node_packet_max_number_send) * 8 / 1024;
            rate = vap->iv_everynode_tl.tx_traffic_limit;
            ret = copy_to_user(ret_str, &rate, sizeof(rate));
            //printk("pkt = %d drop_pkt = %d\n",au_pkt,drop_pkt);
            break;
        /*ljy--add end*/

        default :
            return -EFAULT;
    }
    switch (ik.type) {
        case SET_EVERY_NODE_TRAFFIC_LIMIT_FLAG:
        case SET_EVERY_NODE_TRAFFIC_LIMIT:
        case SET_EVERY_NODE_TRAFFIC_LIMIT_SEND: 
            {   
                extStaDb_StaInfo_t *pStaInfo;
                UCHAR *sta_buf, *show_buf;
                int i, entries;
                entries = extStaDb_entries(vmacSta_p,0);
                sta_buf = kmalloc(entries *64,GFP_KERNEL);
                if (sta_buf == NULL) 
                {
                    break; 
                }
                extStaDb_list(vmacSta_p,sta_buf, 1);
                show_buf = sta_buf;
                
                for(i=0; i<entries; i++)
                {
                    if ( (pStaInfo = extStaDb_GetStaInfo(vmacSta_p,(IEEEtypes_MacAddr_t *)show_buf, 0)) == NULL )
                    {
                        break;
                    }
                
                    pStaInfo->ni_everynode_tl.traffic_limit_flag = vap->iv_everynode_tl.traffic_limit_flag;
                    pStaInfo->ni_everynode_tl.tx_traffic_limit = vap->iv_everynode_tl.tx_traffic_limit;
                    pStaInfo->ni_everynode_tl.tx_rate_in_us_per_byte = vap->iv_everynode_tl.tx_rate_in_us_per_byte;
                    pStaInfo->ni_everynode_tl.rx_traffic_limit = vap->iv_everynode_tl.rx_traffic_limit;
                    pStaInfo->ni_everynode_tl.rx_rate_in_us_per_byte = vap->iv_everynode_tl.rx_rate_in_us_per_byte;

                    show_buf += sizeof(STA_INFO);
                }
                kfree(sta_buf);
            }
            break;
        default:
            return 0;
    }
    return ret;
}

//add end for autelan wmm,traffic_limit,etc. Moved from aquila//lhj//2012.06


#ifdef AE5200_RT_SUPPORT
struct ieee80211_qosframe
{
    IEEEtypes_FrameCtl_t FrmCtl;
    UINT8   dur[2];
    UINT8   addr1[IEEEtypes_ADDRESS_SIZE];
    UINT8   addr2[IEEEtypes_ADDRESS_SIZE];
    UINT8   addr3[IEEEtypes_ADDRESS_SIZE];
    UINT8   seq[2];
    UINT8   qos[2];
    UINT8   addr4[IEEEtypes_ADDRESS_SIZE];
} PACK;

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


int ieee_autelan_qos_decap(struct sk_buff **skb, struct net_device *dev, UINT16 frametype)
{

    //struct sk_buff *wbuf_capture = NULL;
    struct wlprivate *wlpptr = NULL;
    vmacApInfo_t *vmacSta_p = NULL;
    MIB_802DOT11 *mib = NULL;
    struct ieee80211_qosframe *wh = NULL;
    struct ieee80211vap *vap = NULL;
    
    wlpptr = NETDEV_PRIV_P(struct wlprivate, dev);
    vmacSta_p = wlpptr->vmacSta_p;
    mib = vmacSta_p->Mib802dot11;
    vap = &(wlpptr->vap); 
    
    wh = (struct ieee80211_qosframe *)(*skb)->data;

    if (IEEE80211_QOS_HAS_SEQ(wh) && (vap->priv_wmm).tos_flag)
    {
        //(*skb)->priority = wh->qos[0] & IEEE80211_QOS_TID;
        //wh->FrmCtl[0] &= ~IEEE80211_FC0_SUBTYPE_QOS;
        if(frametype == __constant_htons(ETHERTYPE_IP) || frametype == __constant_htons(0x86dd))
        {
            UINT8 tid = wh->qos[0] & IEEE80211_QOS_TID;
            struct iphdr *iph = (struct iphdr *)((*skb)->data + sizeof(struct ieee80211_frame) + LLC_SNAPFRAMELEN);

    		if(4 == iph->version)
    		{
                WlLogPrint(MARVEL_DEBUG_ALL, __func__,"IPV4 Wmm translate Tos tid=%d  TID_TO_AC=%d\n",tid,TID_TO_WME_AC(tid));

    			iph->tos = wmm_to_tos_map(tid, vap) << 2;//Begin:Modified by zhaoyang1 for optimize map of tos vlan & 80211e up 2013-01-28

                WlLogPrint(MARVEL_DEBUG_ALL, __func__,"Wmm translate Tos iph->tos=%d\n",iph->tos);
    			iph->check = 0;
    			iph->check = ip_fast_csum((unsigned char *)iph, iph->ihl);
    		}
    		else if(6 == iph->version)
    		{
    			unsigned char tos = 0;
    			struct ipv6hdr * ipv6h = (struct ipv6hdr*)((*skb)->data + sizeof(struct ieee80211_frame) + LLC_SNAPFRAMELEN);
    			tos =  wmm_to_tos_map(tid, vap) << 2;   //Begin:Modified by zhaoyang1 for optimize map of tos vlan & 80211e up 2013-01-28
    			ipv6h->priority = tos >> 4;
    			ipv6h->flow_lbl[0] |= ((tos <<4) & 0xf0 );
    		}
        }
    }
    return 0;
}


#endif

