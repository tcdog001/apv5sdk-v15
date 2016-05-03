/*
 * Copyright (c) 2010, Atheros Communications Inc.
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

#include <ieee80211_aponly.h>
#include <osdep.h>
#include <wbuf.h>
#include "if_media.h"
#include "ieee80211_var.h"

#include "if_athvar.h"
#include "ath_cwm.h"
#include "osif_private.h"
#include "if_athproto.h"

#ifdef ATH_SUPPORT_HTC
#include "ath_htc_wmi.h"
#include "htc_thread.h"
#endif

#ifdef ATH_TX99_DIAG
#include "ath_tx99.h"
#endif

#if ATH_SUPPORT_IBSS_NETLINK_NOTIFICATION
#include "ath_netlink.h"
#endif

#if ATH_SUPPORT_FLOWMAC_MODULE
#include <flowmac_api.h>
#endif
/*zhaoyang1 transplant from 717*/
/*Begin:added by duanmingzhe for thinap proc*/
#include "if_athproto.h"
#ifdef USE_HEADERLEN_RESV
#include "if_llc.h"
#endif
/*Begin: added by zheng*/
#ifdef APM82181
#include <linux/workqueue.h>
#endif
/*End: by zheng kun*/

#include <linux/sched.h>
extern int pro_file_sche_flag; /*lisongbai add for stop sche*/

/*Begin: added by zhengkun to  create flag  for apv6*/
extern int create_flag_file(char *, char *);
extern void kernel_restart(char *cmd);
#if 0
#ifdef APM82181
struct create_flag_work_struct {
	  void *data;
	  struct  work_struct work_queue;
   };
void create_flag_work_do_work(struct work_struct *work)
{ 
  struct create_flag_work_struct *flag_work=container_of(work,struct create_flag_work_struct,work_queue);
  create_flag_file("/jffs/kernel_reboot_log", flag_work->data);
  create_flag_file("/jffs/sysrebootflag", NULL);  
} 
#endif
/*End:add by zhengkun*/
#endif
/*End:added by duanmingzhe for thinap proc*/
extern u_int32_t rate_threshold_debug;  //added by zhanghu
/*Begin:Added by duanmingzhe for 80211 cache*/
extern int ieee80211_node_cacheq_queue(struct ieee80211_node *ni, wbuf_t wbuf,struct ieee80211com *ic);
extern void enqueue_cache_tx_tasklet_fn(struct ieee80211com *ic);
/*End:Added by duanmingzhe for 80211 cache*/

/*
 * Maximum acceptable MTU
 * MAXFRAMEBODY - WEP - QOS - RSN/WPA:
 * 2312 - 8 - 2 - 12 = 2290
 */
#define ATH_MAX_MTU     2290
#define ATH_MIN_MTU     32
int flowmac_on=0;
module_param(flowmac_on, int, 0);
MODULE_PARM_DESC(flowmac_on, "Enable Flowmac Support");
EXPORT_SYMBOL(flowmac_on);

#if ATH_DEBUG
extern int ACBKMinfree;
extern int ACBEMinfree;
extern int ACVIMinfree;
extern int ACVOMinfree;
extern int CABMinfree;
extern int UAPSDMinfree;

extern struct ath_softc_net80211 *global_scn[10];
extern int num_global_scn;
#endif

static struct ath_reg_parm ath_params = {
    .wModeSelect = MODE_SELECT_ALL,
    .NetBand = MODE_SELECT_ALL,
    .txAggrEnable = 1,
    .rxAggrEnable = 1,
    .txAmsduEnable = 1,
    .aggrLimit = IEEE80211_AMPDU_LIMIT_DEFAULT,
    .aggrSubframes = IEEE80211_AMPDU_SUBFRAME_DEFAULT,
    .aggrProtDuration = 8192,
    .aggrProtMax = 8192,
    .txRifsEnable = 0,
    .rifsAggrDiv = IEEE80211_RIFS_AGGR_DIV,
#ifdef ATH_RB
    .rxRifsEnable = ATH_RB_MODE_DETECT,
    .rxRifsTimeout = ATH_RB_DEF_TIMEOUT,
    .rxRifsSkipThresh = ATH_RB_DEF_SKIP_THRESH,
#endif
    .txChainMaskLegacy = 1,
    .rxChainMaskLegacy = 1,
    .rxChainDetectThreshA = 35,
    .rxChainDetectThreshG = 35,
    .rxChainDetectDeltaA = 30,
    .rxChainDetectDeltaG = 30,
    .calibrationTime = 30,
#if ATH_SUPPORT_LED
    .gpioPinFuncs = {GPIO_PIN_FUNC_0,GPIO_PIN_FUNC_1,GPIO_PIN_FUNC_2},
    .gpioLedCustom = ATH_LED_CUSTOMER,
#else
    .gpioPinFuncs = {1, 7, 7},
#endif
    .hwTxRetries = 4,
    .extendedChanMode = 1,
    .DmaStopWaitTime = 4,
    .swBeaconProcess = 1,
    .stbcEnable = 1,
    .ldpcEnable = 1,
    .cwmEnable = 1,
    .wpsButtonGpio = 0,
#ifdef ATH_SUPPORT_TxBF
    .TxBFSwCvTimeout = 1000 ,
#endif
#if ATH_SUPPORT_SPECTRAL
	.spectralEnable = 1,
#endif
#if ATH_SUPPORT_PAPRD
    .paprdEnable = 1,
#endif
#if ATH_TX_BUF_FLOW_CNTL
    .ACBKMinfree = 48,
    .ACBEMinfree = 32,
    .ACVIMinfree = 16,
    .ACVOMinfree = 0,
    .CABMinfree = 48,
    .UAPSDMinfree = 0,
#endif
#ifdef ATH_SWRETRY
    .numSwRetries = 2,
#endif
#if ATH_SUPPORT_FLOWMAC_MODULE
    .osnetif_flowcntrl = 0,
    .os_ethflowmac_enable = 0,
#endif
#if ATH_SUPPORT_AGGR_BURST
    .burstEnable = 0,
    .burstDur = 5100,
#endif
#if ATH_SUPPORT_FLOWMAC_MODULE
    .os_flowmac_debug = 0
#endif
};

static struct ieee80211_reg_parameters wlan_reg_params = {
    .sleepTimePwrSave = 100,         /* wake up every beacon */
    .sleepTimePwrSaveMax = 1000,     /* wake up every 10 th beacon */
    .sleepTimePerf=100,              /* station wakes after this many mS in max performance mode */
    .inactivityTimePwrSaveMax=400,   /* in max PS mode, how long (in mS) w/o Tx/Rx before going back to sleep */
    .inactivityTimePwrSave=200,      /* in normal PS mode, how long (in mS) w/o Tx/Rx before going back to sleep */
    .inactivityTimePerf=400,         /* in max perf mode, how long (in mS) w/o Tx/Rx before going back to sleep */
    .psPollEnabled=0,                /* Use PS-POLL to retrieve data frames after TIM is received */
    .wmeEnabled    = 1,
    .enable2GHzHt40Cap = 1,
    .cwmEnable = 1,
    .cwmExtBusyThreshold = IEEE80211_CWM_EXTCH_BUSY_THRESHOLD,
    .ignore11dBeacon = 1,
    .p2pGoUapsdEnable = 1,
    .extapUapsdEnable = 1,
#ifdef ATH_SUPPORT_TxBF
    .autocvupdate = 0,
#define DEFAULT_PER_FOR_CVUPDATE 30
    .cvupdateper = DEFAULT_PER_FOR_CVUPDATE,
#endif
};


/*
** Prototype for iw attach
*/

/*zhaoyang1 transplant from 717*/
/*Begin:Added by duanmingzhe for thinap proc*/
void ath_dynamic_sysctl_register_thinap(struct ath_softc_net80211 *scn);
void ath_dynamic_sysctl_unregister_thinap(struct ath_softc_net80211 *scn);
/*End:Added by duanmingzhe for thinap proc*/

/*Begin:Added by duanmingzhe for monitor process and handle fatal exception*/
static void monitor_process_timer_fn(unsigned long);
/*End:Added by duanmingzhe for monitor process and handle fatal exception*/
/*zhaoyang1 transplant end*/


#ifdef ATH_SUPPORT_LINUX_STA
#ifdef CONFIG_SYSCTL
void ath_dynamic_sysctl_register(struct ath_softc *sc);
void ath_dynamic_sysctl_unregister(struct ath_softc *sc);
#endif
#endif
#if OS_SUPPORT_ASYNC_Q
static void os_async_mesg_handler( void  *ctx, u_int16_t  mesg_type, u_int16_t  mesg_len, void  *mesg );
#endif

void ath_iw_attach(struct net_device *dev);
#if !NO_SIMPLE_CONFIG
extern int32_t unregister_simple_config_callback(char *name);
extern int32_t register_simple_config_callback (char *name, void *callback, void *arg1, void *arg2);
static irqreturn_t jumpstart_intr(int cpl, void *dev_id, struct pt_regs *regs, void *push_dur);
#endif

#ifdef ATH_TX99_DIAG
extern u_int8_t tx99_ioctl(struct net_device *dev, struct ath_softc *sc, int cmd, void *addr);
#endif

#ifndef ADF_SUPPORT
void *
OS_ALLOC_VAP(osdev_t osdev, u_int32_t len)
{
    void *netif;

    netif = OS_MALLOC(osdev, len, GFP_KERNEL);
    if (netif != NULL)
        OS_MEMZERO(netif, len);

    return netif;
}

void
OS_FREE_VAP(void *netif)
{
    OS_FREE(netif);
}

#endif

int
ath_get_netif_settings(ieee80211_handle_t ieee)
{
    struct ieee80211com *ic = NET80211_HANDLE(ieee);
    struct ath_softc_net80211 *scn = ATH_SOFTC_NET80211(ic);
    osdev_t osdev = scn->sc_osdev;
    struct net_device *dev =  osdev->netdev;
    int flags = 0;

    if (dev->flags & IFF_RUNNING)
        flags |= ATH_NETIF_RUNNING;
    if (dev->flags & IFF_PROMISC)
        flags |= ATH_NETIF_PROMISCUOUS;
    if (dev->flags & IFF_ALLMULTI)
        flags |= ATH_NETIF_ALLMULTI;

    return flags;
}

/*
 * Merge multicast addresses from all vap's to form the
 * hardware filter.  Ideally we should only inspect our
 * own list and the 802.11 layer would merge for us but
 * that's a bit difficult so for now we put the onus on
 * the driver.
 */
void
ath_mcast_merge(ieee80211_handle_t ieee, u_int32_t mfilt[2])
{
    struct ieee80211com *ic = NET80211_HANDLE(ieee);
    struct ieee80211vap *vap;
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 34)
    struct netdev_hw_addr *ha;  
#else
    struct dev_mc_list *mc;
#endif
    u_int32_t val;
    u_int8_t pos;

    mfilt[0] = mfilt[1] = 0;
    /* XXX locking */
    TAILQ_FOREACH(vap, &ic->ic_vaps, iv_next) {
        struct net_device *dev = ((osif_dev *)vap->iv_ifp)->netdev;
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 34)
        netdev_for_each_mc_addr(ha, dev) {  
            /* calculate XOR of eight 6-bit values */ 
            val = LE_READ_4(ha->addr + 0); 
            pos = (val >> 18) ^ (val >> 12) ^ (val >> 6) ^ val; 
            val = LE_READ_4(ha->addr + 3); 
            pos ^= (val >> 18) ^ (val >> 12) ^ (val >> 6) ^ val; 
            pos &= 0x3f; 
            mfilt[pos / 32] |= (1 << (pos % 32)); 
        } 
#else
        for (mc = dev->mc_list; mc; mc = mc->next) {
            /* calculate XOR of eight 6bit values */
            val = LE_READ_4(mc->dmi_addr + 0);
            pos = (val >> 18) ^ (val >> 12) ^ (val >> 6) ^ val;
            val = LE_READ_4(mc->dmi_addr + 3);
            pos ^= (val >> 18) ^ (val >> 12) ^ (val >> 6) ^ val;
            pos &= 0x3f;
            mfilt[pos / 32] |= (1 << (pos % 32));
        }
#endif         
    }
}

static int
ath_netdev_open(struct net_device *dev)
{
    struct ath_softc_net80211 *scn = ath_netdev_priv(dev);

#ifdef ATH_BUS_PM
    if (scn->sc_osdev->isDeviceAsleep)
        return -EPERM;
#endif /* ATH_BUS_PM */

    dev->flags |= IFF_RUNNING;      /* we are ready to go */
    return ath_resume(scn);
}

static int
ath_netdev_stop(struct net_device *dev)
{
    struct ath_softc_net80211 *scn = ath_netdev_priv(dev);
    dev->flags &= ~IFF_RUNNING;
    return ath_suspend(scn);
}



static int
ath_netdev_hardstart_generic(struct sk_buff *skb, struct net_device *dev)
{
    struct ath_softc_net80211 *scn = ath_netdev_priv(dev);
#ifdef notyet
#ifndef ATH_SUPPORT_LINUX_STA
    struct ath_softc *sc = ATH_DEV_TO_SC(scn->sc_dev);
#endif
#endif
    struct ieee80211_cb *cb;
    struct ieee80211_node *ni;
    int error = 0;
#ifndef ATH_SUPPORT_HTC
    struct ieee80211com *ic = &scn->sc_ic;
    struct ether_header *eh = (struct ether_header *)skb->data;
    int ismulti = IEEE80211_IS_MULTICAST(eh->ether_dhost) ? 1 : 0;
    u_int16_t addba_status;
    u_int32_t txq_depth, txq_aggr_depth;
#endif

    cb = (struct ieee80211_cb *)skb->cb;
    ni = cb->ni;

    /*
     * device must be up and running
     */
    if ((dev->flags & (IFF_RUNNING|IFF_UP)) != (IFF_RUNNING|IFF_UP)) {
        error = -ENETDOWN;
        goto bad;
    }

    /*
     * NB: check for valid node in case kernel directly sends packets
     * on wifiX interface (such as broadcast packets generated by ipv6)
     */
    if (ni == NULL) {
        dev_kfree_skb(skb);
        return 0;
    }

#ifdef ATH_SUPPORT_UAPSD
    /* Limit UAPSD node queue depth to WME_UAPSD_NODE_MAXQDEPTH */
    if ((ni->ni_flags & IEEE80211_NODE_UAPSD) &&
        scn->sc_ops->uapsd_depth(ATH_NODE_NET80211(ni)->an_sta)
            >= WME_UAPSD_NODE_MAXQDEPTH)
    {
        goto bad;
    }
#endif

#ifndef ATH_SUPPORT_HTC

    txq_depth = scn->sc_ops->txq_depth(scn->sc_dev,
                    scn->sc_ac2q[skb->priority]);

    txq_aggr_depth = scn->sc_ops->txq_aggr_depth(scn->sc_dev,
                    scn->sc_ac2q[skb->priority]);

    ic->ic_addba_status(ni, cb->u_tid, &addba_status);

    /* This logic throttles legacy and unaggregated HT frames if they share
     * the hardware  queue with aggregates. This improves the transmit
     * throughput performance to  aggregation enabled nodes when they
     * coexist with legacy nodes.
     */
    /* Do not throttle EAPOL packets - this causes the REKEY packets
     * to be dropped and station disconnects.
     */
    DPRINTF(scn, ATH_DEBUG_RESET,
            "skb->priority=%d cb->u_tid=%d addba_status=%d txq_aggr_depth=%d txq_depth=%d\n",
            skb->priority, cb->u_tid, addba_status, txq_aggr_depth, txq_depth);

    if ((addba_status != IEEE80211_STATUS_SUCCESS) 
        && (txq_aggr_depth > 0)
        && !(eh->ether_type == ETHERTYPE_PAE)) 
    {
        /* Discard this packet if existing q depth is >= 25 */
        if (txq_depth >= 25) {
            goto bad;
        } 

#ifdef notyet /* This should be ported from 7.3 */
#ifndef ATH_SUPPORT_LINUX_STA
        /* Bug 38437 - If the current q depth is greater than the limit we
         * have set for legacy frames, we can put this frame in the holding
         * Q for legacy frames or discard it if the holding Q is also full
         */

        if (txq_depth >= scn->sc_limit_legacy_frames)  {

            if(scn->sc_limit_legacy_txq_count < (scn->sc_limit_legacy_frames)) {
                enq_legacy_wbuf(scn, skb);
                return 0;
            } else {
                // Holding Q is full, discard it
                goto bad;
            }
        }
        /* Bug 38437 - Holding Q contains older frames which must be sent
         * out first so Q this one and send out an older frame from the
         * holding Q
         */
        if (!(TAILQ_EMPTY(&scn->sc_limit_legacy_txq)))  {
            error = deq_legacy_wbuf(scn);
            enq_legacy_wbuf(scn, skb);
            return 0;
        }   
#endif
#endif
    }
    /*
     * Try to avoid running out of descriptors 
     */
    if (ismulti) {
        if (scn->sc_ops->txq_depth(scn->sc_dev, scn->sc_ac2q[skb->priority])
                >= (ATH_TXBUF - 40))
        {
            goto bad;
        }
    }
#endif

    error = ath_tx_send(skb);

#ifdef notyet /* This should be ported from 7.3 */
#ifndef ATH_SUPPORT_LINUX_STA
    if(scn->sc_limit_legacy_txq_count > (txq_depth)) {
        if ((addba_status == IEEE80211_STATUS_SUCCESS)
                && !(TAILQ_EMPTY(&scn->sc_limit_legacy_txq)))  {
                /* Bug 38437 - For non legacy frames, send from the holding
                 * Q if not empty this prevents starvation of the legacy
                 * node
                 */
                DPRINTF(scn, ATH_DEBUG_RESET,
                        "txq_saved=%d txq_aggr_depth=%d txq_depth=%d\n",
                        scn->sc_limit_legacy_txq_count, txq_aggr_depth,
                        txq_depth);
                deq_legacy_wbuf(scn);
        }
    } 
#endif
#endif
    
    if (error) {
        DPRINTF(scn, ATH_DEBUG_XMIT, "%s: Tx failed with error %d\n", 
            __func__, error);
    }
    return 0;  

bad:
    IEEE80211_TX_COMPLETE_WITH_ERROR(skb);
    DPRINTF(scn, ATH_DEBUG_XMIT, "%s: Tx failed with error %d\n", 
            __func__, error);
    return 0;  
}



static int
ath_netdev_hardstart(struct sk_buff *skb, struct net_device *dev)
{
    do_ath_netdev_hardstart(skb,dev);
}
static void
ath_netdev_tx_timeout(struct net_device *dev)
{
    struct ath_softc_net80211 *scn = ath_netdev_priv(dev);

    DPRINTF(scn, ATH_DEBUG_WATCHDOG, "%s: %sRUNNING\n",
            __func__, (dev->flags & IFF_RUNNING) ? "" : "!");

	if (dev->flags & IFF_RUNNING) {
        scn->sc_ops->reset_start(scn->sc_dev, 0, 0, 0);
        scn->sc_ops->reset(scn->sc_dev);
        scn->sc_ops->reset_end(scn->sc_dev, 0);
	}
}

static int
ath_netdev_set_macaddr(struct net_device *dev, void *addr)
{
    struct ath_softc_net80211 *scn = ath_netdev_priv(dev);
    struct ieee80211com *ic = &scn->sc_ic;
    struct sockaddr *mac = addr;

    if (netif_running(dev)) {
        DPRINTF(scn, ATH_DEBUG_ANY,
            "%s: cannot set address; device running\n", __func__);
        return -EBUSY;
    }
     DPRINTF(scn, ATH_DEBUG_ANY, "%s: %.2x:%.2x:%.2x:%.2x:%.2x:%.2x\n",
        __func__,
        mac->sa_data[0], mac->sa_data[1], mac->sa_data[2],
        mac->sa_data[3], mac->sa_data[4], mac->sa_data[5]);

    /* XXX not right for multiple vap's */
    IEEE80211_ADDR_COPY(ic->ic_myaddr, mac->sa_data);
    IEEE80211_ADDR_COPY(dev->dev_addr, mac->sa_data);
    scn->sc_ops->set_macaddr(scn->sc_dev, dev->dev_addr);
    return 0;
}

static void
ath_netdev_set_mcast_list(struct net_device *dev)
{
    struct ath_softc_net80211 *scn = ath_netdev_priv(dev);

#ifdef ATH_SUPPORT_HTC
    if ((!scn) ||
        (scn && scn->sc_htc_delete_in_progress)){
        printk("%s : #### delete is in progress, scn %p \n", __func__, scn);
        return;
    }

    ath_put_defer_item(scn->sc_osdev, scn->sc_ops->mc_upload, WORK_ITEM_SET_MULTICAST,
            (void *)scn->sc_dev, NULL, NULL);
#else
    scn->sc_ops->mc_upload(scn->sc_dev);
#endif
}

static int
ath_change_mtu(struct net_device *dev, int mtu)
{
    if (!(ATH_MIN_MTU < mtu && mtu <= ATH_MAX_MTU)) {
        DPRINTF((struct ath_softc_net80211 *) ath_netdev_priv(dev),
            ATH_DEBUG_ANY, "%s: invalid %d, min %u, max %u\n",
            __func__, mtu, ATH_MIN_MTU, ATH_MAX_MTU);
        return -EINVAL;
    }
    DPRINTF((struct ath_softc_net80211 *) ath_netdev_priv(dev), ATH_DEBUG_ANY,
        "%s: %d\n", __func__, mtu);

    dev->mtu = mtu;
    return 0;
}

/*
 * Diagnostic interface to the HAL.  This is used by various
 * tools to do things like retrieve register contents for
 * debugging.  The mechanism is intentionally opaque so that
 * it can change frequently w/o concern for compatiblity.
 */
static int
ath_ioctl_diag(struct ath_softc_net80211 *scn, struct ath_diag *ad)
{
    struct ath_hal *ah = (ATH_DEV_TO_SC(scn->sc_dev))->sc_ah;
    u_int id = ad->ad_id & ATH_DIAG_ID;
    void *indata = NULL;
    void *outdata = NULL;
    u_int32_t insize = ad->ad_in_size;
    u_int32_t outsize = ad->ad_out_size;
    int error = 0;
    if (ad->ad_id & ATH_DIAG_IN) {
        /*
         * Copy in data.
         */
        indata = kmalloc(insize, GFP_KERNEL);
        if (indata == NULL) {
            error = -ENOMEM;
            goto bad;
        }
        if (_copy_from_user(indata, ad->ad_in_data, insize)) {
            error = -EFAULT;
            goto bad;
        }
    }
    if (ad->ad_id & ATH_DIAG_DYN) {
        /*
         * Allocate a buffer for the results (otherwise the HAL
         * returns a pointer to a buffer where we can read the
         * results).  Note that we depend on the HAL leaving this
         * pointer for us to use below in reclaiming the buffer;
         * may want to be more defensive.
         */
        outdata = kmalloc(outsize, GFP_KERNEL);
        if (outdata == NULL) {
            error = -ENOMEM;
            goto bad;
        }
    }
    if (ath_hal_getdiagstate(ah, id, indata, insize, &outdata, &outsize)) {
        if (outsize < ad->ad_out_size)
            ad->ad_out_size = outsize;
        if (outdata &&
             _copy_to_user(ad->ad_out_data, outdata, ad->ad_out_size))
            error = -EFAULT;
    } else {
        error = -EINVAL;
    }
bad:
    if ((ad->ad_id & ATH_DIAG_IN) && indata != NULL)
        kfree(indata);
    if ((ad->ad_id & ATH_DIAG_DYN) && outdata != NULL)
        kfree(outdata);
    return error;

}

#ifdef ATH_USB
#include "usb_eth.h"
#else
extern int ath_ioctl_ethtool(struct ath_softc_net80211 *scn, int cmd, void *addr);
#endif

#if defined(ATH_SUPPORT_DFS) || defined(ATH_SUPPORT_SPECTRAL)
static int
ath_ioctl_phyerr(struct ath_softc_net80211 *scn, struct ath_diag *ad)
{
     void *indata=NULL;
     void *outdata=NULL;
     int error = -EINVAL;
     u_int32_t insize = ad->ad_in_size;
     u_int32_t outsize = ad->ad_out_size;
     u_int id= ad->ad_id & ATH_DIAG_ID;

    if (ad->ad_id & ATH_DIAG_IN) {
                /*
                 * Copy in data.
                 */
                indata = OS_MALLOC(scn->sc_osdev,insize, GFP_KERNEL);
                if (indata == NULL) {
                        error = -ENOMEM;
                        goto bad;
                }
                if (_copy_from_user(indata, ad->ad_in_data, insize)) {
                        error = -EFAULT;
                        goto bad;
                }
                id = id & ~ATH_DIAG_IN;
        }
        if (ad->ad_id & ATH_DIAG_DYN) {
                /*
                 * Allocate a buffer for the results (otherwise the HAL
                 * returns a pointer to a buffer where we can read the
                 * results).  Note that we depend on the HAL leaving this
                 * pointer for us to use below in reclaiming the buffer;
                 * may want to be more defensive.
                 */
                outdata = OS_MALLOC(scn->sc_osdev, outsize, GFP_KERNEL);
                if (outdata == NULL) {
                        error = -ENOMEM;
                        goto bad;
                }
                id = id & ~ATH_DIAG_DYN;
        }

#if ATH_SUPPORT_DFS
	error = scn->sc_ops->ath_dfs_control(
            scn->sc_dev, id, indata, insize, outdata, &outsize);
#endif

#if ATH_SUPPORT_SPECTRAL
    if (error ==  -EINVAL ) {
        error = scn->sc_ops->ath_spectral_control(
            scn->sc_dev, id, indata, insize, outdata, &outsize);
    }
#endif

         if (outsize < ad->ad_out_size)
                ad->ad_out_size = outsize;

        if (outdata &&
            _copy_to_user(ad->ad_out_data, outdata, ad->ad_out_size))
                error = -EFAULT;
bad:
        if ((ad->ad_id & ATH_DIAG_IN) && indata != NULL)
                OS_FREE(indata);
        if ((ad->ad_id & ATH_DIAG_DYN) && outdata != NULL)
                OS_FREE(outdata);

        return error;
}
#endif

#if  ATH_SUPPORT_AOW

static int
ath_ioctl_aow(struct ath_softc_net80211 *scn, struct ath_diag *ad)
{
    void *indata = NULL;
    void *outdata = NULL;
    int error = 0;

    u_int32_t insize = ad->ad_in_size;
    u_int32_t outsize = ad->ad_out_size;
    u_int   id = ad->ad_id & ATH_DIAG_ID;

    if (ad->ad_id & ATH_DIAG_IN) {
        /* copy in data */
        indata = OS_MALLOC(scn->sc_osdev, insize, GFP_KERNEL);
        if (!indata){
            error = -ENOMEM;
            goto bad;
        }
        if (_copy_from_user(indata, ad->ad_in_data, insize)) {
            error = -EFAULT;
            goto bad;
        }
    }

    /* in case of outdata, allocate space in the 
     * buffer for out data
     */
    if (ad->ad_id & ATH_DIAG_DYN) {
        outdata = OS_MALLOC(scn->sc_osdev, outsize, GFP_KERNEL);
        if (!outdata) {
            error = -ENOMEM;
            goto bad;
        }
    }        

    switch(id) {
        case IOCTL_AOW_DATA:
           {
              
              u_int64_t tsf = 0;
              u_int32_t seqno = 0;
              if (insize > MAX_AOW_PKT_SIZE) {
                  error = -EINVAL;
                  break;
              }                

              tsf = scn->sc_ops->ath_get_tsf64(scn->sc_dev);
              seqno = scn->sc_ops->ath_get_aow_seqno(scn->sc_dev);

              error = scn->sc_ops->ath_aow_control(scn->sc_dev, id, indata, insize, outdata, &outsize, seqno, tsf);
           }
           break;
        default:
           break;
    }            

bad:
    if ((ad->ad_id & ATH_DIAG_IN) && indata != NULL)
        OS_FREE(indata);
    
    if ((ad->ad_id & ATH_DIAG_DYN) && outdata != NULL)
        OS_FREE(outdata);

    return error;


}


#endif  /* ATH_SUPPORT_AOW */

extern unsigned long ath_ioctl_debug;           /* defined in ah_osdep.c  */

struct ioctl_name_tbl {
    unsigned int ioctl;
    char *name;
};

static struct ioctl_name_tbl ioctl_names[] =
{
    {SIOCGATHSTATS, "SIOCGATHSTATS"},
    {SIOCGATHSTATSCLR,"SIOCGATHSTATSCLR"},
    {SIOCGATHDIAG, "SIOCGATHDIAG"},
#if defined(ATH_SUPPORT_DFS) || defined(ATH_SUPPORT_SPECTRAL)
    {SIOCGATHPHYERR,"SIOCGATHPHYERR"},
#endif
    {SIOCETHTOOL,"SIOCETHTOOL"},
    {SIOC80211IFCREATE, "SIOC80211IFCREATE"},
#ifdef ATH_TX99_DIAG
    {SIOCIOCTLTX99, "SIOCIOCTLTX99"},
#endif
    {0, NULL},
};

static char *find_ath_std_ioctl_name(int param)
{
    int i = 0;
    for (i = 0; ioctl_names[i].ioctl; i++) {
        if (ioctl_names[i].ioctl == param)
            return(ioctl_names[i].name ? ioctl_names[i].name : "UNNAMED");
    }
    return("Unknown IOCTL");
}


static int
ath_ioctl(struct net_device *dev, struct ifreq *ifr, int cmd)
{
    struct ath_softc_net80211 *scn = ath_netdev_priv(dev);
#if 0
    struct ieee80211com *ic = &scn->sc_ic;
#endif
    struct ath_stats *as;
    int error = 0;

#ifdef ATH_SUPPORT_HTC
    if ((!scn) ||
        (scn && scn->sc_htc_delete_in_progress)){
        printk("%s : #### delete is in progress, scn %p \n", __func__, scn);
        return -EINVAL;
    }
#endif

    if (ath_ioctl_debug) {
        printk("***%s:  dev=%s  ioctl=0%04X  name=%s\n", __func__, dev->name, cmd, find_ath_std_ioctl_name(cmd));
    }

    switch (cmd) {
    case SIOCGATHEACS:
#if ATH_SUPPORT_SPECTRAL
        error = osif_ioctl_eacs(dev, ifr, scn->sc_osdev); 
#endif
        break;
    case SIOCGATHSTATS:
        as = scn->sc_ops->get_ath_stats(scn->sc_dev);
        if (_copy_to_user(ifr->ifr_data, as,
                         sizeof(struct ath_stats)))
            error = -EFAULT;
        else
            error = 0;
        break;
    case SIOCGATHSTATSCLR:
        as = scn->sc_ops->get_ath_stats(scn->sc_dev);
        memset(as, 0, sizeof(struct ath_stats));
        error = 0;
        break;
    case SIOCGATHDIAG:
        if (!capable(CAP_NET_ADMIN))
            error = -EPERM;
        else
            error = ath_ioctl_diag(scn, (struct ath_diag *) ifr);
        break;
#if defined(ATH_SUPPORT_DFS) || defined(ATH_SUPPORT_SPECTRAL)
    case SIOCGATHPHYERR:
        if (!capable(CAP_NET_ADMIN)) {
            error = -EPERM;
        } else {
            /*
               EV 904010 -- Make sure there is a VAP on the
               interface before the request is processed
            */
	    if ((ATH_DEV_TO_SC(scn->sc_dev))->sc_nvaps)
	        error = ath_ioctl_phyerr(scn, ifr->ifr_data);
	    else
            	error = -EINVAL;
        }
        break;
#endif
    case SIOCETHTOOL:
        if (_copy_from_user(&cmd, ifr->ifr_data, sizeof(cmd)))
            error = -EFAULT;
        else
            error = ath_ioctl_ethtool(scn, cmd, ifr->ifr_data);
        break;
    case SIOC80211IFCREATE:
        error = osif_ioctl_create_vap(dev, ifr, scn->sc_osdev); 
        break;
#if  ATH_SUPPORT_AOW
    case SIOCGATHAOW:
        error = ath_ioctl_aow(scn, ifr->ifr_data);
        break;
#endif  /* ATH_SUPPORT_AOW */

#ifdef ATH_TX99_DIAG
    case SIOCIOCTLTX99:
        printk("Call Tx99 ioctl %d \n",cmd);
        error = tx99_ioctl(dev, ATH_DEV_TO_SC(scn->sc_dev), cmd, ifr->ifr_data);
        break;
#endif
#ifdef ATH_SUPPORT_LINUX_VENDOR
    case SIOCDEVVENDOR:
        error = osif_ioctl_vendor(dev, ifr, 0);
        break;
#endif
#ifdef ATH_BUS_PM
    case SIOCSATHSUSPEND:
      {
        struct ieee80211com *ic = &scn->sc_ic;
        struct ieee80211vap *tmpvap;
        int val = 0;
        if (_copy_from_user(&val, ifr->ifr_data, sizeof(int)))
          return -EFAULT;

        if(val) {
          /* suspend only if all vaps are down */
          TAILQ_FOREACH(tmpvap, &ic->ic_vaps, iv_next) {
            struct net_device *tmpdev = ((osif_dev *)tmpvap->iv_ifp)->netdev;
            if (tmpdev->flags & IFF_RUNNING)
              return -EBUSY;
          }
          error = bus_device_suspend(scn->sc_osdev);
        }
        else
          error = bus_device_resume(scn->sc_osdev);

        if (!error)
            scn->sc_osdev->isDeviceAsleep = val;
      }
      break;
#endif /* ATH_BUS_PM */
    default:
        error = -EINVAL;
        break;
    }
    return error;
}

/*
 * Return netdevice statistics.
 */
static struct net_device_stats *
ath_getstats(struct net_device *dev)
{
    struct ath_softc_net80211 *scn = ath_netdev_priv(dev);
    struct net_device_stats *stats;
    struct ath_stats *as;
    struct ath_phy_stats *ps;
    struct ath_11n_stats *ans;
    WIRELESS_MODE wmode;
#ifdef ATH_SUPPORT_HTC
    if ((!scn) ||
        (scn && scn->sc_htc_delete_in_progress)){
        printk("%s : #### delete is in progress, scn %p \n", __func__, scn);

        /* Kernel expect return net_device_stats if *dev is not NULL. */
        if (scn)
            return (&scn->sc_osdev->devstats);
        else
            return NULL;
    }
#endif

    stats = &scn->sc_osdev->devstats;
    as = scn->sc_ops->get_ath_stats(scn->sc_dev);
    ans = scn->sc_ops->get_11n_stats(scn->sc_dev);
    /* update according to private statistics */
    stats->tx_errors = as->ast_tx_xretries
             + as->ast_tx_fifoerr
             + as->ast_tx_filtered
             ;
    stats->tx_dropped = as->ast_tx_nobuf
            + as->ast_tx_encap
            + as->ast_tx_nonode
            + as->ast_tx_nobufmgt;
    /* Add tx beacons, tx mgmt, tx, 11n tx */
    stats->tx_packets = as->ast_be_xmit
            + as->ast_tx_mgmt
            + as->ast_tx_packets
            + ans->tx_pkts;
    /* Add rx, 11n rx (rx mgmt is included) */
    stats->rx_packets = as->ast_rx_packets
            + ans->rx_pkts;

    for (wmode = 0; wmode < WIRELESS_MODE_MAX; wmode++) {
        ps = scn->sc_ops->get_phy_stats(scn->sc_dev, wmode);
        
        stats->rx_errors = ps->ast_rx_fifoerr;
        stats->rx_dropped = ps->ast_rx_tooshort;
        stats->rx_crc_errors = ps->ast_rx_crcerr;
    }
    
    return stats;
}

#ifndef ATH_SUPPORT_HTC


static void
ath_tasklet(TQUEUE_ARG data)
{
    struct net_device *dev = (struct net_device *)data;
    struct ath_softc_net80211 *scn = ath_netdev_priv(dev);

    do_ath_handle_intr(scn->sc_dev);
}
#endif



#ifndef ATH_SUPPORT_HTC
irqreturn_t
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,19)
ath_isr_generic(int irq, void *dev_id)
#else
ath_isr_generic(int irq, void *dev_id, struct pt_regs *regs)
#endif
{
    struct net_device *dev = dev_id;
    struct ath_softc_net80211 *scn = ath_netdev_priv(dev);
    int sched, needmark = 0;

    /* always acknowledge the interrupt */
    sched = scn->sc_ops->isr(scn->sc_dev);
    
    switch(sched)
    {
    case ATH_ISR_NOSCHED:
        return  IRQ_HANDLED;
        
    case ATH_ISR_NOTMINE:
        return IRQ_NONE;
        
    default:
        if ((dev->flags & (IFF_RUNNING|IFF_UP)) != (IFF_RUNNING|IFF_UP))
        {
            DPRINTF_INTSAFE(scn, ATH_DEBUG_INTR, "%s: flags 0x%x\n", __func__, dev->flags);
            scn->sc_ops->disable_interrupt(scn->sc_dev);     /* disable further intr's */
            return IRQ_HANDLED;
        }
    }
    
    /*
    ** See if the transmit queue processing needs to be scheduled
    */
    
    ATH_SCHEDULE_TQUEUE(&scn->sc_osdev->intr_tq, &needmark);
    if (needmark)
        mark_bh(IMMEDIATE_BH);

    return IRQ_HANDLED;
}
#endif


#ifndef ATH_SUPPORT_HTC
irqreturn_t
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,19)
ath_isr(int irq, void *dev_id)
{
    do_ath_isr(irq,dev_id);
}
#else
ath_isr(int irq, void *dev_id, struct pt_regs *regs)
{
    do_ath_isr(irq,dev_id,regs);
}
#endif
#endif


#if LINUX_VERSION_CODE > KERNEL_VERSION(2,6,30)
static const struct net_device_ops athdev_net_ops = {
    .ndo_open    = ath_netdev_open,
    .ndo_stop    = ath_netdev_stop,
    .ndo_start_xmit = ath_netdev_hardstart,
    .ndo_set_mac_address = ath_netdev_set_macaddr,
    .ndo_tx_timeout = ath_netdev_tx_timeout,
    .ndo_get_stats = ath_getstats,
    .ndo_change_mtu = ath_change_mtu,
    .ndo_set_multicast_list = ath_netdev_set_mcast_list,
    .ndo_do_ioctl = ath_ioctl,
};
#endif

#if ATH_SUPPORT_FLOWMAC_MODULE
int8_t
ath_flowmac_pause_wifi (struct net_device *dev, u_int8_t pause, u_int32_t duration)
{
    return 0;
}

int8_t
ath_flowmac_ratelimit_wifi(struct net_device *dev, flow_dir_t dir,
                void *rate)
{
    return 0;
}

int
ath_flowmac_notify_wifi(struct net_device *dev, flowmac_event_t event,
                void *event_data)
{
    return 0;
}

int
ath_flowmac_attach(ath_dev_t dev)
{
    flow_ops_t ops;
    struct ath_softc *sc = ATH_DEV_TO_SC(dev);

    if (!dev) {
        printk("%s dev pointer is wrong \n", __func__);
        return -EINVAL;
    }
    if (!sc->sc_ethflowmac_enable) {
        /* return without any registration */
        return 0;
    }
    /* regiser with the flow mac driver */
    ops.pause = ath_flowmac_pause_wifi;
    ops.rate_limit  = ath_flowmac_ratelimit_wifi;
    ops.notify_event = ath_flowmac_notify_wifi;

    sc->sc_flowmac_dev = flowmac_register(sc->sc_osdev->netdev, &ops, 0, 0, 0, 0);
    if (sc->sc_flowmac_dev) {
        printk("%s flowmac registration is successful %p\n",
                __func__, sc->sc_flowmac_dev);
        return 0;
    }
    return -EINVAL;
    
}
EXPORT_SYMBOL(ath_flowmac_attach);
int
ath_flowmac_detach(ath_dev_t dev)
{
    struct ath_softc *sc = ATH_DEV_TO_SC(dev);

    if (!dev) {
        printk("%s dev pointer is wrong \n", __func__);
        return -EINVAL;
    }
    if (sc->sc_flowmac_dev) {
        flowmac_unregister(sc->sc_flowmac_dev);
        sc->sc_flowmac_dev = NULL;
    }
    return 0;
}
#endif

int
__ath_attach(u_int16_t devid, struct net_device *dev, HAL_BUS_CONTEXT *bus_context, osdev_t osdev)
{
    struct ath_softc_net80211 *scn = ath_netdev_priv(dev);
    struct ieee80211com *ic = &scn->sc_ic;
    int error = 0;
    struct hal_reg_parm hal_conf_parm;
    hal_conf_parm.calInFlash = bus_context->bc_info.cal_in_flash;

#if ATH_DEBUG
    printk("%s: Set global_scn[%d]\n", __func__, num_global_scn);
    global_scn[num_global_scn++] = scn;
    ath_params.ACBKMinfree = ath_params.ACBKMinfree == ACBKMinfree ? ath_params.ACBKMinfree : ACBKMinfree;
    ath_params.ACBEMinfree = ath_params.ACBEMinfree == ACBEMinfree ? ath_params.ACBEMinfree : ACBEMinfree;
    ath_params.ACVIMinfree = ath_params.ACVIMinfree == ACVIMinfree ? ath_params.ACVIMinfree : ACVIMinfree;
    ath_params.ACVOMinfree = ath_params.ACVOMinfree == ACVOMinfree ? ath_params.ACVOMinfree : ACVOMinfree;
    ath_params.CABMinfree = ath_params.CABMinfree == CABMinfree ? ath_params.CABMinfree : CABMinfree;
    ath_params.UAPSDMinfree = ath_params.UAPSDMinfree == UAPSDMinfree ? ath_params.UAPSDMinfree : UAPSDMinfree;
    printk("ACBKMinfree = %d\n", ath_params.ACBKMinfree);
    printk("ACBEMinfree = %d\n", ath_params.ACBEMinfree);
    printk("ACVIMinfree = %d\n", ath_params.ACVIMinfree);
    printk("ACVOMinfree = %d\n", ath_params.ACVOMinfree);
    printk("CABMinfree = %d\n", ath_params.CABMinfree);
    printk("UAPSDMinfree = %d\n", ath_params.UAPSDMinfree);
#endif

    /* show that no dedicated amem instance has been created yet */
    scn->amem.handle = NULL;
    /*
     * create and initialize ath layer
     */
#if ATH_SUPPORT_FLOWMAC_MODULE
    ath_params.osnetif_flowcntrl = flowmac_on,
    ath_params.os_ethflowmac_enable = flowmac_on,
    ath_params.os_flowmac_debug = 0xf0 & flowmac_on;
#endif
    error = ath_attach(devid, bus_context, scn, osdev, &ath_params, &hal_conf_parm, &wlan_reg_params);

    if (error != 0)
        goto bad;

	/*
	 * Set bus context to osdev
	 */
	osdev->bc = *bus_context;
#if ATH_SUPPORT_IBSS_NETLINK_NOTIFICATION
    ath_adhoc_netlink_init();
#endif
#if ATH_RXBUF_RECYCLE
    ath_rxbuf_recycle_init(osdev);
#endif    

    osif_attach(dev);
    /* For STA Mode default CWM mode is Auto */	
    if ( ic->ic_opmode == IEEE80211_M_STA)
        ic->ic_cwm_set_mode(ic, IEEE80211_CWM_MODE2040);

#if 0 /* TBD: is it taken care in UMAC ? */
    /*
     * This will be used in linux client while processing 
     * country ie in 11d beacons
     */
    ic->ic_ignore_11dbeacon = 0;

    /*
     * commommode is used while determining the channel power
     * in standard client mode
     */ 
    ic->ic_commonmode = ic->ic_country.isMultidomain; 

#endif
	
    /*
     * initialize tx/rx engine
     */
    error = scn->sc_ops->tx_init(scn->sc_dev, ATH_TXBUF);
    if (error != 0)
        goto bad1;

    error = scn->sc_ops->rx_init(scn->sc_dev, ATH_RXBUF);
    if (error != 0)
        goto bad2;

    /*
     * setup net device
     */
#if LINUX_VERSION_CODE > KERNEL_VERSION(2,6,30)
    dev->netdev_ops = &athdev_net_ops;
#else
    dev->open = ath_netdev_open;
    dev->stop = ath_netdev_stop;
    dev->hard_start_xmit = ath_netdev_hardstart;
    dev->set_mac_address = ath_netdev_set_macaddr;
    dev->tx_timeout = ath_netdev_tx_timeout;
    dev->set_multicast_list = ath_netdev_set_mcast_list;
    dev->do_ioctl = ath_ioctl;
    dev->get_stats = ath_getstats;
    dev->change_mtu = ath_change_mtu;
#endif
    dev->watchdog_timeo = 5 * HZ;           /* XXX */
    dev->tx_queue_len = ATH_TXBUF-1;        /* 1 for mgmt frame */
#ifdef USE_HEADERLEN_RESV
    dev->hard_header_len += sizeof (struct ieee80211_qosframe) + sizeof(struct llc) + IEEE80211_ADDR_LEN + IEEE80211_WEP_IVLEN + IEEE80211_WEP_KIDLEN;
#ifdef ATH_SUPPORT_HTC
    dev->hard_header_len += ATH_HTC_HDRSPACE + ATH_HTC_RESERVED + ATH_HTC_TX_FRAME_HDRSPACE;
#endif
#ifdef ATH_SUPERG_FF
    dev->hard_header_len += ATH_FF_MAX_HDR;
#endif
#endif
    /*
    ** Attach the iwpriv handlers
    */
    
    ath_iw_attach(dev);

    /*
     * setup interrupt serivce routine
     */
     
#ifndef ATH_SUPPORT_HTC
#ifdef ADF_SUPPORT
    ATH_INIT_TQUEUE(&osdev->intr_tq, (adf_os_defer_fn_t)ath_tasklet, (void*)dev);
#else
   ATH_INIT_TQUEUE(&osdev->intr_tq, ath_tasklet, dev);
#endif
#endif

#if OS_SUPPORT_ASYNC_Q
   OS_MESGQ_INIT(osdev, &osdev->async_q, sizeof(os_async_q_mesg),
        OS_ASYNC_Q_MAX_MESGS,os_async_mesg_handler,  osdev,MESGQ_PRIORITY_NORMAL,MESGQ_ASYNCHRONOUS_EVENT_DELIVERY);      
#endif

	/*
     * Resolving name to avoid a crash in request_irq() on new kernels
     */ 
    
    dev_alloc_name(dev, dev->name);
 
#ifdef ATH_SUPPORT_HTC
    /*
     * do not register PCI IRQ in USB mode
     */
    if (0) {
        goto bad3;
    }
#else
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,20)
    if (request_irq(dev->irq, ath_isr, SA_SHIRQ, dev->name, dev)) {
#else

#ifdef ATH_AHB
    if (request_irq(dev->irq, ath_isr, IRQF_DISABLED, dev->name, dev)) {
#else
    if (request_irq(dev->irq, ath_isr, IRQF_SHARED, dev->name, dev)) {
#endif
    
#endif
        printk(KERN_WARNING "%s: request_irq failed\n", dev->name);
        error = -EIO;
        goto bad3;
    }
#endif
    /* Kernel 2.6.25 needs valid dev_addr before  register_netdev */
    IEEE80211_ADDR_COPY(dev->dev_addr,ic->ic_myaddr);

    /*
     * finally register netdev and ready to go
     */
    if ((error = register_netdev(dev)) != 0) {
        printk(KERN_ERR "%s: unable to register device\n", dev->name);
        goto bad4;
    }
#if !NO_SIMPLE_CONFIG
    /* Request Simple Config intr handler */
    register_simple_config_callback (dev->name, (void *) jumpstart_intr, (void *) dev,
                                     (void *)&osdev->sc_push_button_dur);
#endif

#ifdef ATH_SUPPORT_LINUX_STA
#ifdef CONFIG_SYSCTL
    ath_dynamic_sysctl_register(ATH_DEV_TO_SC(scn->sc_dev));
#endif
#endif

#if ATH_SUPPORT_FLOWMAC_MODULE
    ath_flowmac_attach(scn->sc_dev);
#endif
	/*zhaoyang1 transplant from 717*/
    /*Begin:Added by duanmingzhe for thinap proc*/
    ath_dynamic_sysctl_register_thinap(scn);
    /*End:Added by duanmingzhe for thinap proc*/
    /*Begin:Added by duanmingzhe for monitor process*/
/*    if (memcmp(dev->name, "wifi0", 5) == 0)
    {
	init_timer(&(ic->monitor_process_timer));
	ic->monitor_process_timer.function = monitor_process_timer_fn;
	ic->monitor_process_timer.data = (unsigned long) ic;
	ic->monitor_process_timer.expires = jiffies + HZ * 10;
	add_timer(&(ic->monitor_process_timer));
    }*/
    /*End:Added by duanmingzhe for monitor process*/

    /*Begin:Added by duanmingzhe for handle fatal exception*/
    ic->ic_wtpd_init_timer = 0;
    /*End:Added by duanmingzhe for handle fatal exception*/
	/*zhaoyang1 transplant end*/
	/*Begin:Added by duanmingzhe for 80211 cache*/
	tasklet_init(&ic->ic_cache_tx_tasklet_enqueue,(void *)enqueue_cache_tx_tasklet_fn,(unsigned long)ic);
	/*End:Added by duanmingzhe for 80211 cache*/
    return 0;
    
bad4:
    free_irq(dev->irq, dev);
bad3:
    scn->sc_ops->rx_cleanup(scn->sc_dev);
bad2:
    scn->sc_ops->tx_cleanup(scn->sc_dev);
bad1:
    ath_detach(scn);
bad:
    return error;
}

int
__ath_detach(struct net_device *dev)
{
    struct ath_softc_net80211 *scn = ath_netdev_priv(dev);
	/*zhaoyang1 transplant from 717*/
    struct ieee80211com *ic = &scn->sc_ic;
	/*zhaoyang1 transplant end*/

    osif_detach(dev);
#ifndef ATH_SUPPORT_HTC
    if (dev->irq)
        free_irq(dev->irq, dev);
#endif

#ifdef ATH_SUPPORT_LINUX_STA
#ifdef CONFIG_SYSCTL
    ath_dynamic_sysctl_unregister(ATH_DEV_TO_SC(scn->sc_dev));
#endif
#endif
/*zhaoyang1 transplant from 717*/
/*Begin:Added by duanmingzhe for thinap proc*/
ath_dynamic_sysctl_unregister_thinap(scn);
/*End:Added by duanmingzhe for thinap proc*/
/*Begin:Added by duanmingzhe for monitor process and handle fatal exception*/
    del_timer(&(ic->monitor_process_timer));
/*End:Added by duanmingzhe for monitor process and handle fatal exception*/
/*zhaoyang1 transplant end*/

#ifndef NO_SIMPLE_CONFIG
    unregister_simple_config_callback(dev->name);
#endif
    unregister_netdev(dev);

#if OS_SUPPORT_ASYNC_Q
   OS_MESGQ_DRAIN(&scn->sc_osdev->async_q,NULL);
   OS_MESGQ_DESTROY(&scn->sc_osdev->async_q);
#endif
    scn->sc_ops->rx_cleanup(scn->sc_dev);
    scn->sc_ops->tx_cleanup(scn->sc_dev);

#if ATH_SUPPORT_IBSS_NETLINK_NOTIFICATION
    ath_adhoc_netlink_delete();
#endif
#if ATH_RXBUF_RECYCLE
    ath_rxbuf_recycle_destroy(scn->sc_osdev);
#endif /* ATH_RXBUF_RECYCLE */
#if ATH_SUPPORT_FLOWMAC_MODULE
    ath_flowmac_detach(scn->sc_dev);
#endif
    return ath_detach(scn);
}

int
__ath_remove_interface(struct net_device *dev)
{
    struct ath_softc_net80211 *scn = ath_netdev_priv(dev);
    struct ieee80211com *ic = &scn->sc_ic;
    struct ieee80211vap *vap;
    struct ieee80211vap *vapnext;
    osif_dev  *osifp;
    struct net_device *netdev;

    vap = TAILQ_FIRST(&ic->ic_vaps);
    while (vap != NULL) {
        /* osif_ioctl_delete_vap() destroy vap->iv_next information,
        so need to store next VAP address in vapnext */
        vapnext = TAILQ_NEXT(vap, iv_next);
        osifp = (osif_dev *)vap->iv_ifp;
        netdev = osifp->netdev;
        printk("Remove interface on %s\n",netdev->name);
        rtnl_lock();
        dev_close(netdev);
        osif_ioctl_delete_vap(netdev);
        rtnl_unlock();
        vap = vapnext;
    }    
    return 0;
}

void
__ath_set_delete(struct net_device *dev)
{
#ifdef ATH_SUPPORT_HTC
    struct ath_softc_net80211 *scn = ath_netdev_priv(dev);
    struct ieee80211com *ic = &scn->sc_ic;

    ic->ic_delete_in_progress = scn->sc_htc_delete_in_progress = 1;
#endif
}

int
__ath_suspend(struct net_device *dev)
{
    return ath_netdev_stop(dev);
}

int
__ath_resume(struct net_device *dev)
{
    return ath_netdev_open(dev);
}
#if !NO_SIMPLE_CONFIG
/*
 * Handler for front panel SW jumpstart switch
 */
static irqreturn_t
jumpstart_intr (int cpl, void *dev_id, struct pt_regs *regs, void *push_time)
{
    struct net_device *dev = dev_id;
    struct ath_softc_net80211 *scn = ath_netdev_priv(dev);
    struct ieee80211com *ic = &scn->sc_ic;
    struct ieee80211vap *vap;
    u_int32_t           push_duration;

    /*
    ** Iterate through all VAPs, since any of them may have WPS enabled
    */

    vap = TAILQ_FIRST(&ic->ic_vaps);
    while (vap != NULL) {
        if (push_time) {
            push_duration = *(u_int32_t *)push_time;
        } else {
            push_duration = 0;
        }
        printk("SC Pushbutton Notify on %s for %d sec(s) and the vap %p dev %p:\n",dev->name,
                push_duration, vap, (struct net_device *)(((osif_dev *)vap->iv_ifp)->netdev));
        osif_notify_push_button ((struct net_device *)(((osif_dev *)vap->iv_ifp)->netdev), push_duration);
        vap = TAILQ_NEXT(vap, iv_next);
    }
    return IRQ_HANDLED;
}
#endif

#if OS_SUPPORT_ASYNC_Q
static void os_async_mesg_handler( void  *ctx, u_int16_t  mesg_type, u_int16_t  mesg_len, void  *mesg )
{
    if (mesg_type == OS_SCHEDULE_ROUTING_MESG_TYPE) {
        os_schedule_routing_mesg  *s_mesg = (os_schedule_routing_mesg *) mesg;
        s_mesg->routine(s_mesg->context, NULL);
    }
}
#endif

void
init_wlan(void)
{
   
}
/*zhaoyang1 transplant from 717*/
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

    /*Start:Added by zhanghu for increasing rate*/
    ATH_RATE_THRESHOLD_DEBUG = 12,
    /*End:Added by zhanghu for increasing rate*/
	/*suzhaoyu add start iwlist ath scanning modify PCAPVXN-87*/
	ATH_IWLIST_SCAN_CURRENT_CHAN_INDEX_NA = 13,
	/*suzhaoyu addend*/
	/*Begin:Added by duanmingzhe for 80211 cache*/
	ATH_80211CACHE = 14,
	ATH_80211CACHE_DEBUG = 15,
	ATH_80211CACHE_TIMER = 16,


	ATH_80211CACHE_FACTOR_WIFI0 = 17,
	ATH_80211CACHE_FACTOR_B_WIFI0 = 18,
	ATH_80211CACHE_FACTOR_G_WIFI0 = 19,
	ATH_80211CACHE_FACTOR_N_WIFI0 = 20,
	ATH_80211CACHE_FACTOR_WIFI1 = 21,
	ATH_80211CACHE_FACTOR_B_WIFI1 = 22,
	ATH_80211CACHE_FACTOR_G_WIFI1 = 23,
	ATH_80211CACHE_FACTOR_N_WIFI1 = 24,
	ATH_80211CACHE_PASS_FACTOR = 25,
	ATH_80211CACHE_MIN_SIZE = 26,
	ATH_80211CACHE_MAX_SIZE = 27,
	/*End:Added by duanmingzhe for 80211 cache*/
	/*zhaoyang modify for dusi check*/
#if ATH_HW_TXQ_STUCK_WAR
	ATH_MAX_ABNORMITY_TIME = 28,
#endif
	/*zhaoyang modify end*/
};

static int
ATH_SYSCTL_DECL(ath_sysctl_halparam, ctl, write, filp, buffer, lenp, ppos)
{
        static u_int val;
        int ret=0;

        ctl->data = &val;
        ctl->maxlen = sizeof(val);
        if (write) {
		ret = ATH_SYSCTL_PROC_DOINTVEC(ctl, write, filp, buffer,
                lenp, ppos);
		if(0 == ret)
		{
			switch ((long)ctl->extra2) {
				case ATH_THINAP:
					thinap = val;
					break;
				case ATH_TRAFFIC_BALANCE:
					traffic_balance = val;
					break;
				case ATH_THINAP_STATE:
					thinap_state = val;
					break;
			    case ATH_MONITOR_PROCESS:
					monitor_process = val;
					break;
				case ATH_SYSREBOOT_FLAG:
					sysreboot_flag = val;
					break;
				case ATH_THINAP_CHECK_TIMER:
					thinap_check_timer =val;
					break;
				case ATH_THINAP_CHECK_THRESHOLD:
					thinap_check_threshold = val;
					break;
				/*suzhaoyu add start iwlist ath scanning modify PCAPVXN-87*/
				case ATH_IWLIST_SCAN_ONEBYONE:
					iwlist_scan_onebyone=val;
					break;
				case ATH_IWLIST_SCAN_CURRENT_CHAN_INDEX_NG:
				         iwlist_scan_current_chan_ng=val;
					break;
				case ATH_IWLIST_SCAN_CURRENT_CHAN_INDEX_NA:
				         iwlist_scan_current_chan_na=val;
					break;
	                        /*suzhaoyu addend*/
				/*zhaoyang add start for stop recv and send*/
				case ATH_STOP_TRANSMIT:
					stop_transmit =val;
				break;
				/*zhaoyang add end*/
				/*zhaoyang modify for dusi check*/
#if ATH_HW_TXQ_STUCK_WAR
				case ATH_MAX_ABNORMITY_TIME:
					max_abnormity_time = val;
				break;
#endif
				/*zhaoyang modify end*/
				/*Begin:Added by duanmingzhe for 80211 cache*/
				case ATH_80211CACHE:
					cache_enable = val;
				break;
				case ATH_80211CACHE_FACTOR_WIFI0:
					cache_factor_wifi0= val;
				break;
				case ATH_80211CACHE_FACTOR_B_WIFI0:
					cache_factor_b_wifi0= val;
				break;
				case ATH_80211CACHE_FACTOR_G_WIFI0:
					cache_factor_g_wifi0= val;
				break;
				case ATH_80211CACHE_FACTOR_N_WIFI0:
					cache_factor_n_wifi0= val;
				break;
				case ATH_80211CACHE_FACTOR_WIFI1:
					cache_factor_wifi1= val;
				break;
				case ATH_80211CACHE_FACTOR_B_WIFI1:
					cache_factor_b_wifi1= val;
				break;
				case ATH_80211CACHE_FACTOR_G_WIFI1:
					cache_factor_g_wifi1= val;
				break;
				case ATH_80211CACHE_FACTOR_N_WIFI1:
					cache_factor_n_wifi1= val;
				break;
				case ATH_80211CACHE_PASS_FACTOR:
					cache_pass_factor = val;
				break;
				case ATH_80211CACHE_MIN_SIZE:
					cache_min_size= val;
				break;
				case ATH_80211CACHE_MAX_SIZE:
					cache_max_size= val;
				break;
				case ATH_80211CACHE_DEBUG:
					cache_debug= val;
				break;				
				case ATH_80211CACHE_TIMER:
					cache_timer = val;
				break;

				/*End:Added by duanmingzhe for 80211 cache*/
                /*Start:Added by zhanghu for increasing rate*/
                case ATH_RATE_THRESHOLD_DEBUG:
        			if(val == 0 || val == 1 )
        				rate_threshold_debug = val;
    			break;
                /*End:Added by zhanghu for increasing rate*/

				default:
					return -EINVAL;
			}
		}
        }
	else
	{
		switch ((long)ctl->extra2) {
			case ATH_THINAP:
				val = thinap;
				break;
			case ATH_TRAFFIC_BALANCE:
				val = traffic_balance;
				break;
			case ATH_THINAP_STATE:
				val = thinap_state;				
				break;
			case ATH_MONITOR_PROCESS:
				val = monitor_process;
				break;
			case ATH_SYSREBOOT_FLAG:
				val = sysreboot_flag;
				break;
			case ATH_THINAP_CHECK_TIMER:
				val = thinap_check_timer;
				break;
			case ATH_THINAP_CHECK_THRESHOLD:
				val = thinap_check_threshold;
				break;
			/*suzhaoyu add start iwlist ath scanning modify PCAPVXN-87*/
		        case ATH_IWLIST_SCAN_ONEBYONE:
			        val = iwlist_scan_onebyone;
			        break;
		        case ATH_IWLIST_SCAN_CURRENT_CHAN_INDEX_NG:
			        val = iwlist_scan_current_chan_ng;
			        break;
		        case ATH_IWLIST_SCAN_CURRENT_CHAN_INDEX_NA:
			        val = iwlist_scan_current_chan_na;
			        break;
         		/*suzhaoyu add end*/
			/*zhaoyang add start for stop recv and send*/
			case ATH_STOP_TRANSMIT:
				val = stop_transmit ;
				break;
			/*zhaoyang add end*/
			/*zhaoyang modify for dusi check*/
#if ATH_HW_TXQ_STUCK_WAR
			case ATH_MAX_ABNORMITY_TIME:
				val = max_abnormity_time;
				break;
#endif
			/*zhaoyang modify end*/
			/*Begin:Added by duanmingzhe for 80211 cache*/
			case ATH_80211CACHE:
				val = cache_enable;
				break;
			case ATH_80211CACHE_FACTOR_WIFI0:
				val = cache_factor_wifi0;
				break;
			case ATH_80211CACHE_FACTOR_B_WIFI0:
				val = cache_factor_b_wifi0;
				break;
			case ATH_80211CACHE_FACTOR_G_WIFI0:
				val = cache_factor_g_wifi0;
				break;
			case ATH_80211CACHE_FACTOR_N_WIFI0:
				val = cache_factor_n_wifi0;
				break;
			case ATH_80211CACHE_FACTOR_WIFI1:
				val = cache_factor_wifi1;
				break;
			case ATH_80211CACHE_FACTOR_B_WIFI1:
				val = cache_factor_b_wifi1;
				break;
			case ATH_80211CACHE_FACTOR_G_WIFI1:
				val = cache_factor_g_wifi1;
				break;
			case ATH_80211CACHE_MIN_SIZE:
				val = cache_min_size;
				break;
			case ATH_80211CACHE_MAX_SIZE:
				val = cache_max_size;
				break;
			case ATH_80211CACHE_FACTOR_N_WIFI1:
				val = cache_factor_n_wifi1;
				break;
			case ATH_80211CACHE_PASS_FACTOR:
				val = cache_pass_factor;
				break;
			case ATH_80211CACHE_DEBUG:
				val = cache_debug;
				break;
			case ATH_80211CACHE_TIMER:
				val = cache_timer;
				break;

			/*End:Added by duanmingzhe for 80211 cache*/
            /*Start:Added by zhanghu for increasing rate*/
            case ATH_RATE_THRESHOLD_DEBUG:
			    val = rate_threshold_debug;
			break;
            /*End:Added by zhanghu for increasing rate*/

            default:
				return -EINVAL;
		}
                ret = ATH_SYSCTL_PROC_DOINTVEC(ctl, write, filp, buffer,
                                lenp, ppos);
	}
	return ret;
}


/* sysctl entries: /proc/sys/dev/wifiN/... */
static const ctl_table ath_sysctl_template[] = {
        { .ctl_name     = CTL_AUTO,
          .procname     = "thinap",
          .mode         = 0644,
          .proc_handler = ath_sysctl_halparam,
          .extra2       = (void *)ATH_THINAP,
        },
        { .ctl_name     = CTL_AUTO,
          .procname     = "traffic_balance",
          .mode         = 0644,
          .proc_handler = ath_sysctl_halparam,
          .extra2       = (void *)ATH_TRAFFIC_BALANCE,
        },
        { .ctl_name     = CTL_AUTO,
          .procname     = "thinap_state",
          .mode         = 0644,
          .proc_handler = ath_sysctl_halparam,
          .extra2       = (void *)ATH_THINAP_STATE,
        },
        { .ctl_name     = CTL_AUTO,
          .procname     = "monitor_process",
          .mode         = 0644,
          .proc_handler = ath_sysctl_halparam,
          .extra2       = (void *)ATH_MONITOR_PROCESS,
        },
        { .ctl_name     = CTL_AUTO,
          .procname     = "sysreboot_flag",
          .mode         = 0644,
          .proc_handler = ath_sysctl_halparam,
          .extra2       = (void *)ATH_SYSREBOOT_FLAG,
        },
        { .ctl_name     = CTL_AUTO,
          .procname     = "thinap_check_timer",
          .mode         = 0644,
          .proc_handler = ath_sysctl_halparam,
          .extra2       = (void *)ATH_THINAP_CHECK_TIMER,
        },
        { .ctl_name	= CTL_AUTO,
	  .procname     = "thinap_check_threshold",
	  .mode         = 0644,
	  .proc_handler = ath_sysctl_halparam,
	  .extra2	= (void *)ATH_THINAP_CHECK_THRESHOLD,
	},
	/*suzhaoyu add start iwlist ath scanning modify PCAPVXN-87*/
        { .ctl_name     = CTL_AUTO,
          .procname     = "iwlist_scan_onebyone",
          .mode         = 0644,
          .proc_handler = ath_sysctl_halparam,
          .extra2       = (void *)ATH_IWLIST_SCAN_ONEBYONE,
        },
        { .ctl_name     = CTL_AUTO,
          .procname     = "iwlist_scan_current_chan_ng",
          .mode         = 0644,
          .proc_handler = ath_sysctl_halparam,
          .extra2       = (void *)ATH_IWLIST_SCAN_CURRENT_CHAN_INDEX_NG,
        },
        { .ctl_name     = CTL_AUTO,
          .procname     = "iwlist_scan_current_chan_na",
          .mode         = 0644,
          .proc_handler = ath_sysctl_halparam,
          .extra2       = (void *)ATH_IWLIST_SCAN_CURRENT_CHAN_INDEX_NA,
        },
        /*suzhaoyu add end*/
	/*zhaoyang add start for stop recv and send*/
	{ .ctl_name         = CTL_AUTO,
      .procname         = "stop_transmit",
      .mode             = 0644,
      .proc_handler     = ath_sysctl_halparam,
   	  .extra2           = (void *)ATH_STOP_TRANSMIT,  
    },
    /*zhaoyang add end*/
	/*zhaoyang modify for dusi check*/
#if ATH_HW_TXQ_STUCK_WAR
	{ .ctl_name         = CTL_AUTO,
      .procname         = "max_abnormity_time",
      .mode             = 0644,
      .proc_handler     = ath_sysctl_halparam,
   	  .extra2           = (void *)ATH_MAX_ABNORMITY_TIME,  
    },
#endif
    /*zhaoyang add end*/
	/*Begin:Added by duanmingzhe for 80211 cache*/
	{ .ctl_name 		= CTL_AUTO,
	  .procname 		= "cache_enable",
	  .mode 			= 0644,
	  .proc_handler 	= ath_sysctl_halparam,
	  .extra2			= (void *)ATH_80211CACHE,  
	},	
	{ .ctl_name 		= CTL_AUTO,
	  .procname 		= "cache_factor_wifi0",
	  .mode 			= 0644,
	  .proc_handler 	= ath_sysctl_halparam,
	  .extra2			= (void *)ATH_80211CACHE_FACTOR_WIFI0,  
	},	
	{ .ctl_name 		= CTL_AUTO,
	  .procname 		= "cache_factor_b_wifi0",
	  .mode 			= 0644,
	  .proc_handler 	= ath_sysctl_halparam,
	  .extra2			= (void *)ATH_80211CACHE_FACTOR_B_WIFI0,  
	},
	{ .ctl_name 		= CTL_AUTO,
	  .procname 		= "cache_factor_g_wifi0",
	  .mode 			= 0644,
	  .proc_handler 	= ath_sysctl_halparam,
	  .extra2			= (void *)ATH_80211CACHE_FACTOR_G_WIFI0,  
	},
	{ .ctl_name 		= CTL_AUTO,
	  .procname 		= "cache_factor_n_wifi0",
	  .mode 			= 0644,
	  .proc_handler 	= ath_sysctl_halparam,
	  .extra2			= (void *)ATH_80211CACHE_FACTOR_N_WIFI0,  
	},
	{ .ctl_name 		= CTL_AUTO,
	  .procname 		= "cache_factor_wifi1",
	  .mode 			= 0644,
	  .proc_handler 	= ath_sysctl_halparam,
	  .extra2			= (void *)ATH_80211CACHE_FACTOR_WIFI1,  
	},	
	{ .ctl_name 		= CTL_AUTO,
	  .procname 		= "cache_factor_b_wifi1",
	  .mode 			= 0644,
	  .proc_handler 	= ath_sysctl_halparam,
	  .extra2			= (void *)ATH_80211CACHE_FACTOR_B_WIFI1,  
	},
	{ .ctl_name 		= CTL_AUTO,
	  .procname 		= "cache_factor_g_wifi1",
	  .mode 			= 0644,
	  .proc_handler 	= ath_sysctl_halparam,
	  .extra2			= (void *)ATH_80211CACHE_FACTOR_G_WIFI1,  
	},
	{ .ctl_name 		= CTL_AUTO,
	  .procname 		= "cache_factor_n_wifi1",
	  .mode 			= 0644,
	  .proc_handler 	= ath_sysctl_halparam,
	  .extra2			= (void *)ATH_80211CACHE_FACTOR_N_WIFI1,  
	},
	{ .ctl_name 		= CTL_AUTO,
	  .procname 		= "cache_pass_factor",
	  .mode 			= 0644,
	  .proc_handler 	= ath_sysctl_halparam,
	  .extra2			= (void *)ATH_80211CACHE_PASS_FACTOR,  
	},
	{ .ctl_name 		= CTL_AUTO,
	  .procname 		= "cache_min_size",
	  .mode 			= 0644,
	  .proc_handler 	= ath_sysctl_halparam,
	  .extra2			= (void *)ATH_80211CACHE_MIN_SIZE,  
	},
	{ .ctl_name 		= CTL_AUTO,
	  .procname 		= "cache_max_size",
	  .mode 			= 0644,
	  .proc_handler 	= ath_sysctl_halparam,
	  .extra2			= (void *)ATH_80211CACHE_MAX_SIZE,  
	},
	{ .ctl_name 		= CTL_AUTO,
	  .procname 		= "cache_debug",
	  .mode 			= 0644,
	  .proc_handler 	= ath_sysctl_halparam,
	  .extra2			= (void *)ATH_80211CACHE_DEBUG,  
	},
	{ .ctl_name 		= CTL_AUTO,
	  .procname 		= "cache_timer",
	  .mode 			= 0644,
	  .proc_handler 	= ath_sysctl_halparam,
	  .extra2			= (void *)ATH_80211CACHE_TIMER,  
	},

	/*End:Added by duanmingzhe for 80211 cache*/

        
    /*Begin: added by zhanghu for increasing for rate*/
    { .ctl_name = CTL_AUTO,
      .procname     = "rate_threshold_debug",
      .mode         = 0644,
      .proc_handler = ath_sysctl_halparam,
      .extra2   = (void *)ATH_RATE_THRESHOLD_DEBUG,
    },
    /*End: added by zhanghu for increasing for rate*/


	/* may add more in the future */
        { 0 }
};

void
ath_dynamic_sysctl_register_thinap(struct ath_softc_net80211 *scn)
{
        int i, space;
	char *devname = scn->sc_osdev->netdev->name ;

        space = 5*sizeof(struct ctl_table) + sizeof(ath_sysctl_template);
        scn->sc_sysctls = kmalloc(space, GFP_KERNEL);
        if (scn->sc_sysctls == NULL) {
                printk("%s: no memory for sysctl table!\n", __func__);
                return;
        }

        /* setup the table */
        memset(scn->sc_sysctls, 0, space);
        scn->sc_sysctls[0].ctl_name = CTL_DEV;
        scn->sc_sysctls[0].procname = "dev";
        scn->sc_sysctls[0].mode = 0555;
        scn->sc_sysctls[0].child = &scn->sc_sysctls[2];
        /* [1] is NULL terminator */
        scn->sc_sysctls[2].ctl_name = CTL_AUTO;
        scn->sc_sysctls[2].procname = devname ;
        scn->sc_sysctls[2].mode = 0555;
        scn->sc_sysctls[2].child = &scn->sc_sysctls[4];
        /* [3] is NULL terminator */
        /* copy in pre-defined data */
        memcpy(&scn->sc_sysctls[4], ath_sysctl_template,
                sizeof(ath_sysctl_template));

        /* add in dynamic data references */
        for (i = 4; scn->sc_sysctls[i].procname; i++)
                if (scn->sc_sysctls[i].extra1 == NULL)
                        scn->sc_sysctls[i].extra1 = scn;

        /* and register everything */
#if LINUX_VERSION_CODE > KERNEL_VERSION(2,6,20)
        scn->sc_sysctl_header = register_sysctl_table(scn->sc_sysctls);
#else
        scn->sc_sysctl_header = register_sysctl_table(scn->sc_sysctls, 1);
#endif
        if (!scn->sc_sysctl_header) {
                printk("%s: failed to register sysctls!\n", devname);
                kfree(scn->sc_sysctls);
                scn->sc_sysctls = NULL;
        }
}

void
ath_dynamic_sysctl_unregister_thinap(struct ath_softc_net80211 * scn)
{
        if (scn->sc_sysctl_header) {
                unregister_sysctl_table(scn->sc_sysctl_header);
                scn->sc_sysctl_header = NULL;
        }
        if (scn->sc_sysctls) {
                kfree(scn->sc_sysctls);
                scn->sc_sysctls = NULL;
        }
}

EXPORT_SYMBOL(ath_dynamic_sysctl_register_thinap);
EXPORT_SYMBOL(ath_dynamic_sysctl_unregister_thinap);

/*End:Added by duanmingzhe for thinap proc*/

/*Begin:Added by duanmingzhe for monitor process and handle fatal exception*/
/*wangyu add the stop_watchdog function for APV5*/
#ifdef PC018
extern struct timer_list dog_timer;
extern int (*stop_ap2400_watchdog)(void);
#endif
/*wangyu add end */
/*zhengkun add stop_watchdog APV6*/
#ifdef APM82181
extern struct timer_list dog_timer;
extern int (*stop_ap2400_watchdog)(void);
#endif
/*zhengkun add end*/
static void monitor_process_timer_fn(unsigned long data)
{
	struct ieee80211com *ic = (struct ieee80211com *) data;
	int disable_watchdog = 0;
	char reboot_log[128];
	memset(reboot_log, 0, 128);
	strcpy(reboot_log, "wifi: fatal exception ");

	if (monitor_process)
	{
		if (!find_process(monitor_process))
		{
			printk(KERN_EMERG "Ap monitor is down!\n");
			disable_watchdog++;
		}
	}

	if (sysreboot_flag)
	{
		printk(KERN_EMERG "Sysreboot is called!\n");
		strcat(reboot_log,"Sysreboot is called!\n");
		disable_watchdog++;
	}

	if (thinap)
	{
		if (thinap_state == 0)
		{
			ic->ic_wtpd_init_timer++;
			thinap_check_timer = 0;

			if (ic->ic_wtpd_init_timer >= 66)  //Modified by duanmingzhe change 60 to 66
			{
				printk(KERN_EMERG "Wtpd is not run after initializtion 10 minutes!\n");
				strcat(reboot_log,"Wtpd or ap_monitor is not run after initializtion 10 minutes!\n");
				disable_watchdog++;
			}
		}
		else if (thinap_state == 1)
		{
			ic->ic_wtpd_init_timer = 0;
			thinap_check_timer++;

			if (thinap_check_timer >= thinap_check_threshold)
			{
				printk(KERN_EMERG "Wtpd is not receive echo message in 2 minutes!\n");
				strcat(reboot_log,"Wtpd or ap_monitor is not receive echo message in several minutes!\n");
				disable_watchdog++;
			}
		
		}
		else if (thinap_state == 2)
		{
			ic->ic_wtpd_init_timer = 0;
			thinap_check_timer = 0;
		}
	}
	else
	{
		ic->ic_wtpd_init_timer = 0;
		thinap_check_timer = 0;
	}

	if (disable_watchdog)
	{
#if 0
	/*Begin:added by zhengkun
  * change the former  strategy.We use the timer to create or modify 
  * the correspond file in /jffs to record the reason why sys goes to die and  time.
  * But in apv6,we can't do that like before as in the driver of spi flash,it needs to be scheduled;
  *which is not premmited in the soft interupt of a timer.
  *So, we would like to add a work queue to commplete this task.
  */
		#ifdef APM82181
		    del_timer(&dog_timer);
			stop_ap2400_watchdog = NULL;
     		struct create_flag_work_struct create_flag_work;
		    INIT_WORK(&(create_flag_work.work_queue),(void(*)(void *))create_flag_work_do_work);
           	create_flag_work.data=reboot_log;
    		schedule_work(&create_flag_work.work_queue);
		#else
		#endif
	/*End : by zhengkun*/
#endif
		/*wangyu add the stop_watchdog function for APV5*/
		#if defined(PC018) || defined(APM82181)
			del_timer(&dog_timer);
			stop_ap2400_watchdog = NULL;
		#endif
		/*wangyu add end */
	
		pro_file_sche_flag = 1;    /*lisongbai add for stop sche*/
		create_flag_file("/jffs/kernel_reboot_log", reboot_log);
		create_flag_file("/jffs/sysrebootflag", NULL);
#ifndef APM82181
#if defined(ATH_AHB) || defined(WNDAP350) || defined(AP93) || defined(XJPB44_02) || defined(TDPB44_02) || defined(AQ2000N) 
		kernel_restart("wifi: fatal exception");
#else
		disable_watchdog_irq();
#endif
#endif
	}

	mod_timer(&(ic->monitor_process_timer), jiffies + HZ * 10);
}
/*End:Added by duanmingzhe for monitor process and handle fatal exception*/
/*zhaoyang1 transplant end*/

#ifdef ATH_USB

#define IS_UP(_dev) \
    (((_dev)->flags & (IFF_RUNNING|IFF_UP)) == (IFF_RUNNING|IFF_UP))

static int ath_reinit_interface(struct net_device *dev)
{
    struct ath_softc_net80211 *scn = ath_netdev_priv(dev);
    struct ieee80211com *ic = &scn->sc_ic;
    struct ieee80211vap *vap;
    struct ieee80211vap *vapnext;
    osif_dev  *osifp;
    struct net_device *netdev;

    vap = TAILQ_FIRST(&ic->ic_vaps);
    while (vap != NULL) {
        vapnext = TAILQ_NEXT(vap, iv_next);
        osifp = (osif_dev *)vap->iv_ifp;
        netdev = osifp->netdev;
        rtnl_lock();
        if (IS_UP(netdev)) {
            dev_close(netdev);
            msleep(50);
            dev_open(netdev);
        }
        rtnl_unlock();
        vap = vapnext;
    }

    return 0;
}

void ath_usb_vap_restart(struct net_device *dev)
{
    struct ath_softc_net80211 *scn = ath_netdev_priv(dev);

    cmpxchg((int*)(&(scn->sc_dev_enabled)), 1, 0);
    ath_netdev_open(dev);
    ath_reinit_interface(dev);
}

EXPORT_SYMBOL(__ath_detach);
EXPORT_SYMBOL(init_wlan);
EXPORT_SYMBOL(__ath_remove_interface);
EXPORT_SYMBOL(__ath_attach);
EXPORT_SYMBOL(ath_usb_vap_restart);
EXPORT_SYMBOL(__ath_set_delete);
#endif

#ifndef REMOVE_PKT_LOG

struct ath_softc * 
ath_get_softc(struct net_device *dev)                                         
{           
    struct ath_softc_net80211 *scn = ath_netdev_priv(dev);                    
    return (struct ath_softc *)scn->sc_dev;                                   
}    
EXPORT_SYMBOL(ath_get_softc);

#endif