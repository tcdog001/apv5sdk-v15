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

#include "ap8xLnxIntf.h"
#include <stdarg.h>
#include "ap8xLnxFwcmd.h"
#include "ap8xLnxVapProc.h"

#include <linux/version.h>
#include <linux/module.h>
#include <linux/delay.h>
#include <linux/netdevice.h>
#include <linux/utsname.h>
#include <linux/if_arp.h>       /* XXX for ARPHRD_ETHER */
#include <net/iw_handler.h>
#include <linux/slab.h>

/*add by zhanxuechao*/
u_int32_t vap_splitmac = 0;
EXPORT_SYMBOL(vap_splitmac);

static int
IEEE80211_SYSCTL_DECL(ieee80211_sysctl_vap_splitmac, ctl, write, filp, buffer,
    lenp, ppos)
{
    struct wlprivate *wlp = ctl->extra1;
    u_int val;
    int ret;

    ctl->data = &val;
    ctl->maxlen = sizeof(val);
    if (write) {
        ret = IEEE80211_SYSCTL_PROC_DOINTVEC(ctl, write, filp, buffer,
            lenp, ppos);
        if (ret == 0)
            wlp->vap.vap_splitmac = val;
    } else {
        val = wlp->vap.vap_splitmac;
        ret = IEEE80211_SYSCTL_PROC_DOINTVEC(ctl, write, filp, buffer,
            lenp, ppos);
    }
    return ret;
}

extern u_int32_t aid;
static int
IEEE80211_SYSCTL_DECL(ieee80211_sysctl_txpower_offset, ctl, write, filp, buffer,
    lenp, ppos)
{
#if 0
    struct ieee80211vap *vap = ctl->extra1;
    u_int val;
    int ret;

    struct txpoweroffsetreq req;
    req.aid = aid;
    req.sta_count = 0;
    ctl->data = &val;
    ctl->maxlen = sizeof(val);

    if (write) {
        ret = IEEE80211_SYSCTL_PROC_DOINTVEC(ctl, write, filp, buffer,
            lenp, ppos);
        if (ret == 0) { 
            req.txpower_offset = val;
            wlan_iterate_station_list(vap, set_txpower_offset, &req);
            if (req.sta_count == 0)
                printk("the sta which aid is %u is not exist.\n", req.aid);
        }
    } else {
        wlan_iterate_station_list(vap, get_txpower_offset, &req);
        val = req.txpower_offset;
        ret = IEEE80211_SYSCTL_PROC_DOINTVEC(ctl, write, filp, buffer,
            lenp, ppos);
    }
    return ret;
#endif
    return 0;
}

static int
IEEE80211_SYSCTL_DECL(ieee80211_sysctl_aid, ctl, write, filp, buffer,
    lenp, ppos)
{
    u_int val;
    int ret;
    ctl->data = &val;
    ctl->maxlen = sizeof(val);

    if (write) {
        ret = IEEE80211_SYSCTL_PROC_DOINTVEC(ctl, write, filp, buffer,
            lenp, ppos);
        if (ret == 0) { 
            aid = val;
            printk("set aid %d\n",aid);         
        }
    } else {
        val = aid;
        ret = IEEE80211_SYSCTL_PROC_DOINTVEC(ctl, write, filp, buffer,
            lenp, ppos);
    }
    return ret;
}

static int
IEEE80211_SYSCTL_DECL(ieee80211_sysctl_show, ctl, write, filp, buffer,
    lenp, ppos)
{
#if 0
    struct ieee80211vap *vap = ctl->extra1;
    u_int val;
    int ret;
    struct txpoweroffsetreq req;
    ctl->data = &val;
    ctl->maxlen = sizeof(val);

    if (write) {
        ret = IEEE80211_SYSCTL_PROC_DOINTVEC(ctl, write, filp, buffer,
            lenp, ppos);
        if (ret == 0) { 
            req.sta_count = 0;
            wlan_iterate_station_list(vap, show_txpower_offset, &req);
            printk("sta count is %d\n", req.sta_count);
        }
    } else {
        val = 1;
        ret = IEEE80211_SYSCTL_PROC_DOINTVEC(ctl, write, filp, buffer,
            lenp, ppos);
    }
    return ret;
#endif
    return 0;
}

static const ctl_table ieee80211_sysctl_template[] = {
    /* NB: must be last entry before NULL */
    {
        .procname   = "%parent",
        .maxlen = IFNAMSIZ,
        .mode       = 0444,
        .proc_handler   = proc_dostring
    },

    /*lyb add start for thinap*/
    {
        .procname = "vap_splitmac",
        .mode   = 0644,
        .proc_handler = ieee80211_sysctl_vap_splitmac
    },
    /*lyb add end*/
    /*Begin:Added by duanmingzhe for auto-transmit
    {
        .procname   = "transmit_switch",
        .mode       = 0644,
        .proc_handler   = ieee80211_sysctl_transmit_switch
    },*/
    /*  {
        .procname   = "transmit_rate",
        .mode       = 0644,
        .proc_handler   = ieee80211_sysctl_transmit_rate
    },
    End:Added by duanmingzhe for auto-transmit*/
    /*zhaoyang modify for txpower ctl*/
    {
        .procname = "txpower_offset",
        .mode     = 0644,
        .proc_handler = ieee80211_sysctl_txpower_offset
    },
    {
        .procname = "aid",
        .mode     = 0644,
        .proc_handler = ieee80211_sysctl_aid
    },
    {
        .procname = "show",
        .mode     = 0644,
        .proc_handler = ieee80211_sysctl_show
    },
    /*zhaoyang modify end*/

    {0}
};

void ap8x_dynamic_vap_sysctl_register_thinap(struct wlprivate *wlp)
{
    int i, space;
    char *devname = wlp->netDev->name ;

    space = 5*sizeof(struct ctl_table) + sizeof(ieee80211_sysctl_template);
    wlp->sc_wl_sysctls = kmalloc(space, GFP_KERNEL);
    if (wlp->sc_wl_sysctls == NULL) 
    {
        printk("%s: no memory for sysctl table!\n", __func__);
        return;
    }

    /* setup the table */
    memset(wlp->sc_wl_sysctls, 0, space);
    wlp->sc_wl_sysctls[0].procname = "net";
    wlp->sc_wl_sysctls[0].mode = 0555;
    wlp->sc_wl_sysctls[0].child = &wlp->sc_wl_sysctls[2];
    /* [1] is NULL terminator */
    wlp->sc_wl_sysctls[2].procname = devname ;
    wlp->sc_wl_sysctls[2].mode = 0555;
    wlp->sc_wl_sysctls[2].child = &wlp->sc_wl_sysctls[4];
    /* [3] is NULL terminator */
    /* copy in pre-defined data */
    memcpy(&wlp->sc_wl_sysctls[4], ieee80211_sysctl_template,
            sizeof(ieee80211_sysctl_template));

    /* add in dynamic data references */
    for (i = 4; wlp->sc_wl_sysctls[i].procname; i++)
        if (wlp->sc_wl_sysctls[i].extra1 == NULL)
            wlp->sc_wl_sysctls[i].extra1 = wlp;

    /* and register everything */
#if LINUX_VERSION_CODE > KERNEL_VERSION(2,6,20)
    wlp->sc_wl_sysctl_header = register_sysctl_table(wlp->sc_wl_sysctls);
#else
    wlp->sc_wl_sysctl_header = register_sysctl_table(wlp->sc_wl_sysctls, 1);
#endif
    if (!wlp->sc_wl_sysctl_header) 
    {
        printk("%s: failed to register sysctls!\n", devname);
        kfree(wlp->sc_wl_sysctls);
        wlp->sc_wl_sysctls = NULL;
    }
}

void ap8x_dynamic_vap_sysctl_unregister_thinap(struct wlprivate *wlp)
{
    if (wlp->sc_wl_sysctl_header) 
    {
        unregister_sysctl_table(wlp->sc_wl_sysctl_header);
        wlp->sc_wl_sysctl_header = NULL;
    }
    if (wlp->sc_wl_sysctls) 
    {
        kfree(wlp->sc_wl_sysctls);
        wlp->sc_wl_sysctls = NULL;
    }

    return;
}

    /************** zhouke add ,splitmac **************/
struct ieee80211vap * get_vap_from_netdev(struct net_device * dev)  
{
    return (&(NETDEV_PRIV_P(struct wlprivate, dev)->vap)); 
}
EXPORT_SYMBOL(get_vap_from_netdev);
    /********************* end ************************/

EXPORT_SYMBOL(ap8x_dynamic_vap_sysctl_register_thinap);
EXPORT_SYMBOL(ap8x_dynamic_vap_sysctl_unregister_thinap);
