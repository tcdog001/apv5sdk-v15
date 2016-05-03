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

#ifndef AP8X_LNX_PROC_H_
#define AP8X_LNX_PROC_H_

#include "ap8xLnxIntf.h"
#include <stdarg.h>

/*Begin:Added by duanmingzhe for thinap proc*/
enum {
    ATH_THINAP = 1,
    ATH_TRAFFIC_BALANCE = 2,
    /*Begin:Added by duanmingzhe for monitor process*/
    ATH_MONITOR_PROCESS = 3,
    /*End:Added by duanmingzhe for monitor process*/

    /*Begin:Added by duanmingzhe for handle fatal exception*/
    ATH_SYSREBOOT_FLAG = 4,
    ATH_THINAP_STATE = 5,
    ATH_THINAP_CHECK_TIMER = 6,
    /*End:Added by duanmingzhe for handle fatal exception*/
    /*Begin:Added by duanmingzhe for wtpd echo threshhold*/
    ATH_THINAP_CHECK_THRESHOLD =7,
    /*End:Added by duanmingzhe for wtpd echo threshhold*/
    /*Begin:Added by chenming for rssi threshhold*/
    ATH_THINAP_RSSI_THRESHOLD = 8,
    /*End:Added by chenming for rssi threshhold*/
    /*zhaoyang add start for stop recv and send*/
    ATH_STOP_TRANSMIT = 9,
    /*zhaoyang add end*/
    /*suzhaoyu add start iwlist ath scanning modify PCAPVXN-87*/
    ATH_IWLIST_SCAN_ONEBYONE= 10,
    ATH_IWLIST_SCAN_CURRENT_CHAN_INDEX_NG = 11,
    /*suzhaoyu addend*/
    /*Begin:Added by duanmingzhe for 80211 cache*/
    ATH_80211CACHE = 12,
    ATH_80211CACHE_DEBUG = 13,
    ATH_80211CACHE_TIMER = 14,
    /*End:Added by duanmingzhe for 80211 cache*/
    /*suzhaoyu add start iwlist ath scanning modify PCAPVXN-87*/
    ATH_IWLIST_SCAN_CURRENT_CHAN_INDEX_NA = 15,
    /*suzhaoyu addend*/
    DISABLE_PROBE_RESPONSE = 16, /*added by chenming for disable probe response*/
    /*Begin:Added by zhanghu for increasing for rate*/
    ATH_RATE_THRESHOLD = 17,
    ATH_RATE_THRESHOLD_A = 18,
    ATH_RATE_THRESHOLD_B = 19,
    ATH_RATE_THRESHOLD_G = 20,
    ATH_RATE_THRESHOLD_ENABLE = 21,
    ATH_RATE_THRESHOLD_SINGLE = 22,
    ATH_RATE_THRESHOLD_DOUBLE = 23,
    /*End:Added by zhanghu for increasing for rate*/
    /*suzhaoyu add for suppressing illegal sta begin*/
    ATH_STA_INTERFERCE =24,
    /*suzhaoyu add end */


    /*Begin:Added by duanmingzhe for change data priority*/
    ATH_DATA_PRIORITY = 25,
    /*End:Added by duanmingzhe for change data priority*/
    /*lyb add start*/
    ATH_NUM_PER_TIMER = 26,
    ATH_PACKET_RATE = 27,
    ATH_TIMER_SCHED = 28,
    ATH_SIGNAL_DECISION = 29,
    ATH_NUM_PER_TIMER_STEP = 30,
    ATH_PACKET_NUM_FOR_TIME = 31,
    ATH_PACKET_NUM_FOR_TIME_STEP = 32,
    ATH_INTERFER_OR_RECV = 33,
    ATH_TWICE_NUM_PER_TIMER = 34,
    ATH_FORCE_MANUAL = 35,
    /*lyb add end*/
    /*Begin:Added by duanmingzhe for print txq info*/
    ATH_TXQPRINT = 36,
    ATH_PRINTDEVID = 37,
    /*End:Added by duanmingzhe for print txq info*/ 
    /*Begin:Added by duanmingzhe for 80211 cache*/
    ATH_80211CACHE_FACTOR_WIFI0 = 38,
    ATH_80211CACHE_FACTOR_B_WIFI0 = 39,
    ATH_80211CACHE_FACTOR_G_WIFI0 = 40,
    ATH_80211CACHE_FACTOR_N_WIFI0 = 41,
    ATH_80211CACHE_FACTOR_WIFI1 = 42,
    ATH_80211CACHE_FACTOR_B_WIFI1 = 43,
    ATH_80211CACHE_FACTOR_G_WIFI1 = 44,
    ATH_80211CACHE_FACTOR_N_WIFI1 = 45,
    ATH_80211CACHE_PASS_FACTOR = 46,
    ATH_80211CACHE_MIN_SIZE = 47,
    ATH_80211CACHE_MAX_SIZE = 48,
    /*End:Added by duanmingzhe for 80211 cache*/
    /*zhaoyang modify for send arp active when wireless to wireless*/
    ATH_ARP_SEND_ACTIVE=49,
    /*zhaoyang modify end*/
    /*zhaoyang modify for tunnel data frame fast forward*/
    ATH_TUNNEL_FAST_FORWARD=50,
    /*zhaoyang modify end*/

    /*zhanxuechao modify for debug*/
    ATH_DEBUG_LEVEL=51,

#ifdef AE5200_RT_SUPPORT
    /*zhaoyang add for priority join in 5G*/
    ATH_JOIN5G_ENABLE = 52,
	/*<Begin : add for 5G priority by caizhibang  2012-09-27 */
	ATH_SCANTIME_THR = 53,
	ATH_AGINGTIME_THR = 54,
	ATH_DISCARD_COUNT = 55,
	ATH_JOIN5G_DEBUG = 56,
	ATH_STA_COUNT = 578,
	ATH_STA_SIGNAL_STRENGTH_THR = 58,   /* add for 2.4g signal strength threshold, wangjia 2012-10-16. */
	/*zhaoyang add end*/
	/*End : add for 5G priority by caizhibang  2012-09-27 >*/
	ATH_MCAST_QUEUE_DEBUG = 59,	/*  added by huangtao for mcast queue 2013-5-24*/

#endif
    
};

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,32))
#define ATH_SYSCTL_DECL(f, ctl, write, filp, buffer, lenp, ppos) \
    f(ctl_table *ctl, int write, void *buffer,\
        size_t *lenp, loff_t *ppos)
#else
#define ATH_SYSCTL_DECL(f, ctl, write, filp, buffer, lenp, ppos) \
    f(ctl_table *ctl, int write, struct file *filp, void *buffer,\
        size_t *lenp, loff_t *ppos)
#endif
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,32))
#define ATH_SYSCTL_PROC_DOINTVEC(ctl, write, filp, buffer, lenp, ppos) \
    proc_dointvec(ctl, write, buffer, lenp, ppos)
#else
#define ATH_SYSCTL_PROC_DOINTVEC(ctl, write, filp, buffer, lenp, ppos) \
    proc_dointvec(ctl, write, filp, buffer, lenp, ppos)
#endif
#define ATH_SYSCTL_PROC_DOSTRING(ctl, write, filp, buffer, lenp, ppos) \
    proc_dostring(ctl, write, filp, buffer, lenp, ppos)

extern void ap8x_dynamic_sysctl_register_thinap(struct wlprivate *wlp);
extern void ap8x_dynamic_sysctl_unregister_thinap(struct wlprivate *wlp);

#endif
