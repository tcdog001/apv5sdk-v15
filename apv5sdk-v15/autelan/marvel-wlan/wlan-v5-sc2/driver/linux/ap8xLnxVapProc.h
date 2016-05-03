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

#ifndef AP8X_LNX_VAP_PROC_H_
#define AP8X_LNX_VAP_PROC_H_

#include "ap8xLnxIntf.h"
#include <stdarg.h>

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,32))
#define IEEE80211_SYSCTL_DECL(f, ctl, write, filp, buffer, lenp, ppos) \
    f(ctl_table *ctl, int write, void *buffer,\
        size_t *lenp, loff_t *ppos)
#else
#define IEEE80211_SYSCTL_DECL(f, ctl, write, filp, buffer, lenp, ppos) \
    f(ctl_table *ctl, int write, struct file *filp, void *buffer,\
        size_t *lenp, loff_t *ppos)
#endif
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,32))
#define IEEE80211_SYSCTL_PROC_DOINTVEC(ctl, write, filp, buffer, lenp, ppos) \
    proc_dointvec(ctl, write, buffer, lenp, ppos)
#else
#define IEEE80211_SYSCTL_PROC_DOINTVEC(ctl, write, filp, buffer, lenp, ppos) \
    proc_dointvec(ctl, write, filp, buffer, lenp, ppos)
#endif
#define IEEE80211_SYSCTL_PROC_DOSTRING(ctl, write, filp, buffer, lenp, ppos) \
    proc_dostring(ctl, write, filp, buffer, lenp, ppos)

extern void ap8x_dynamic_vap_sysctl_register_thinap(struct wlprivate *wlp);
extern void ap8x_dynamic_vap_sysctl_unregister_thinap(struct wlprivate *wlp);

#endif

