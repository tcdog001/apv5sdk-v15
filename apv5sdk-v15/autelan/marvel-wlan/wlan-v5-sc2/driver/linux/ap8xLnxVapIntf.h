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

#ifndef AP8X_VAPINIT_
#define AP8X_VAPINIT_

#include <linux/if.h>
#include <linux/version.h>
#include <linux/wireless.h>
#include "ap8xLnxIoctlWiress.h"

#define NEW_ATH_SET_VAP_BSSID(bssid, id)                    \
        do {                            \
            if (id)                     \
                bssid += id;            \
        } while (0)

#define KASSERT(exp, msg) do {          \
    if (unlikely(!(exp))) {         \
        printk msg;         \
        BUG();              \
    }                   \
} while (0)

#define IEEE80211_F_NOBRIDGE             0x10000000  /* CONF: disable internal bridge */
#define IEEE80211_FEXT_PURE11N             0x80000000  /* CONF: pure 11n mode */


#define IEEE80211_VAP_NOBRIDGE_ENABLE(_v)           ((_v).iv_flags |= IEEE80211_F_NOBRIDGE)
#define IEEE80211_VAP_NOBRIDGE_DISABLE(_v)          ((_v).iv_flags &= ~IEEE80211_F_NOBRIDGE)
#define IEEE80211_VAP_IS_NOBRIDGE_ENABLED(_v)       ((_v).iv_flags & IEEE80211_F_NOBRIDGE)

#define IEEE80211_VAP_PURE11N_ENABLE(_v)            ((_v).iv_flags_ext |= IEEE80211_FEXT_PURE11N)
#define IEEE80211_VAP_PURE11N_DISABLE(_v)           ((_v).iv_flags_ext &= ~IEEE80211_FEXT_PURE11N)
#define IEEE80211_VAP_IS_PURE11N_ENABLED(_v)        ((_v).iv_flags_ext & IEEE80211_FEXT_PURE11N)


#define setbit(a,i) ((a)[(i)/NBBY] |= 1<<((i)%NBBY))
#define clrbit(a,i) ((a)[(i)/NBBY] &= ~(1<<((i)%NBBY)))
#define isset(a,i)  ((a)[(i)/NBBY] & (1<<((i)%NBBY)))
#define isclr(a,i) (((a)[(i)/NBBY] & (1<<((i)%NBBY))) == 0)
#define NBBY    8           /* number of bits/byte */

struct ieee80211_clone_params {
    char icp_name[IFNAMSIZ];        /* device name */
    unsigned short icp_opmode;          /* operating mode */
    unsigned short icp_flags;           /* see below */
#define IEEE80211_CLONE_BSSID   0x0001      /* allocate unique mac/bssid */
#define IEEE80211_NO_STABEACONS 0x0002      /* Do not setup the station beacon timers */
};


int snprintf(char *str, size_t size, const char *format, ...);
int ieee80211_ioctl_create_ath(struct net_device *netdev,struct ifreq *ifr);
int ieee80211_ioctl_destroy_ath(struct net_device *netdev);
int wlinit_vapmac(unsigned char *macaddr,const unsigned char *devmac,int id);
int load_txpower_table(struct net_device *netdev,const char* pwrtable);
#endif

