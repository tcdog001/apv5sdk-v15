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
#include "ap8xLnxVapIntf.h"
#include "ap8xLnxVer.h"
#include "wllog.h"
#include <linux/module.h>
#include <linux/ethtool.h>
//#include "ap8xLnxIntf.c"

extern int new_dispatch_mac;
/*zhaoyang1 transplant from 717*/
static u_int8_t wlan_units[129];     /* enough for 1027 */
/*zhaoyang1 transplant end*/

#define WL_VAP_NUM_FULL     -1
#define WL_VAP_NUM_SUCC     0

static int wl_vap_num = 0;

static int wl_vap_create_num_add(void)
{
    if (wl_vap_num > MAX_VMAC_INSTANCE_AP - 1)
    {
        return WL_VAP_NUM_FULL;
    }
    else
    {
        wl_vap_num++;
    }

    return WL_VAP_NUM_SUCC;
}

static int wl_vap_create_num_del(void)
{
    if (wl_vap_num < 1)
    {
        return WL_VAP_NUM_FULL;
    }
    else
    {
        wl_vap_num--;
    }

    return WL_VAP_NUM_SUCC;
}

static int wl_vap_create_num_get(void)
{
    return wl_vap_num;
}

int wlinit_vapmac(unsigned char *macaddr,const unsigned char *devmac,int id)
{
    unsigned int tmp_mac = 0;

    if(macaddr == NULL || devmac == NULL)
        return -1;
    
    memset(macaddr,0,6);
    memcpy(macaddr,devmac, 6);
    
    if (new_dispatch_mac == 0)
    {  
        macaddr[0] = devmac[0] |((id<<2)|0x2);
    }
    else if (new_dispatch_mac == 1)
    {
        memcpy(&tmp_mac, &(devmac[3]), 1);
        //tmp_mac = tmp_mac >> 24;
        NEW_ATH_SET_VAP_BSSID(tmp_mac, id);
        //tmp_mac = tmp_mac << 24;
        memcpy(&(macaddr[3]), &tmp_mac, 1);
    }
    else if (new_dispatch_mac == 2)
    {
        memcpy(&tmp_mac, &(devmac[3]), 3);
        //tmp_mac = tmp_mac >> 8;
        NEW_ATH_SET_VAP_BSSID(tmp_mac, (id << 16));
        //tmp_mac = tmp_mac << 8;
        memcpy(&(macaddr[3]), &tmp_mac, 3);
    }
    return 0;
}

static int ifc_name2unit(const char *name, u_int32_t *unit) /*modified by chenming*/
{
    const char *cp;
    *unit = 0;
    if(memcmp(name, "athscanwifi", 11) == 0)  // 4 scan vap
    {
        cp = name + 11;
        for(;*cp != '\0'; cp++)
        {
            if ('0' <= *cp && *cp <= '9')
                *unit = (*unit * 10) + (*cp - '0');
        }
        if (*unit > 3) 
            return -1;
        *unit += 1024;
    }
    else if(memcmp(name, "ath", 3) == 0 &&  ('0' <= *(name+3) && *(name+3) <= '3'))  // 4 wds vap
    {
        cp = name + 3;
        for(;*cp != '\0'; cp++)
        {
            if ('0' <= *cp && *cp <= '9')
                *unit = (*unit * 10) + (*cp - '0');
        }
        if (*unit > 3) 
            return -1;
        *unit += 1028;
    }
    else
    {
        for (cp = name; *cp != '\0' && !('0' <= *cp && *cp <= '9'); cp++)
            ;
        if (*cp != '\0')
        {
            int temp = 0;
            temp = *cp - '0';
            cp++;
            if (*cp == '-') 
            {
                cp++;
                for (; *cp != '\0'; cp++)
                {
                    if ('0' <= *cp && *cp <= '9')
                    *unit = (*unit * 10) + (*cp - '0');
                }
                if (*unit < 1 || *unit > 256)
                    return -1;
                *unit += (temp * 256 -1);   // 0-255
            }
            else
            {
                *unit = temp;
                for(;*cp != '\0'; cp++)
                {
                    if ('0' <= *cp && *cp <= '9')
                        *unit = (*unit * 10) + (*cp - '0');
                }
                if (*unit < 0 || *unit > 255)
                    return -1;
            }
        }
        else
            *unit = -1; /*referenced to 0xffffffff chenming note*/
    }
    return 0;
}


/*zhaoyang1 transplant from 717*/
static u_int32_t /*modified by chenming*/
/*zhaoyang1 transplant end*/
ieee80211_new_wlanunit(void)
{
#define N(a)    (sizeof(a)/sizeof(a[0]))
    u_int unit;
    u_int8_t b;
    int i;

    /* NB: covered by rtnl_lock */
    unit = 0;
    for (i = 0; i < N(wlan_units) && wlan_units[i] == 0xff; i++)
        unit += NBBY;
    if (i == N(wlan_units))
        return -1;
    for (b = wlan_units[i]; b & 1; b >>= 1)
        unit++;
    setbit(wlan_units, unit);

    return unit;
#undef N
}

/*
* Check if the specified unit number is available and, if
* so, mark it in use.  Return 1 on success, 0 on failure.
*/
static int
ieee80211_alloc_wlanunit(u_int unit)
{
    /* NB: covered by rtnl_lock */
    if (unit < sizeof(wlan_units)*NBBY && isclr(wlan_units, unit))
    {
        setbit(wlan_units, unit);
        return 1;
    }
    else
        return 0;
}

/*
* Reclaim the specified unit number.
*/
/*zhaoyang1 transplant from 717*/
static void
ieee80211_delete_wlanunit(u_int32_t unit) /*modified by chenming*/
/*zhaoyang1 transplant end*/
{
    /* NB: covered by rtnl_lock */
    KASSERT(unit < sizeof(wlan_units)*NBBY, ("invalid wlan unit %u", unit));
    KASSERT(isset(wlan_units, unit), ("wlan unit %u not allocated", unit));
    clrbit(wlan_units, unit);
    return;
}

int ieee80211_ioctl_create_ath(struct net_device *netdev,struct ifreq *ifr)
{
    struct ieee80211_clone_params cp;
    int error;
    /*zhaoyang1 transplant from 717*/
    u_int32_t unit; /*modified by chenming*/
    /*zhaoyang1 transplant end*/
    unsigned char name[IFNAMSIZ];
    unsigned char macaddr[6];
    int i;
    struct wlprivate *wlpptr ;

    if((netdev == NULL) || (ifr == NULL))
    {
        WlLogPrint(MARVEL_DEBUG_PANIC, __func__,"netdev is NULL");
        return -1;
    }

    if (wl_vap_create_num_get() > MAX_VMAC_INSTANCE_AP - 1)
    {
        printk("Create vap interface number is full, can only create 16 vap interface.\n");
        return -1;
    }

    wlpptr = NETDEV_PRIV_P(struct wlprivate, netdev);
    
    if (copy_from_user(&cp, ifr->ifr_data, sizeof(cp)))
    {
        WlLogPrint(MARVEL_DEBUG_ERROR, __func__,"copy_from_user is error");
        return -1;
    }
        
    error = ifc_name2unit(cp.icp_name, &unit);
    if (error)
    {
        WlLogPrint(MARVEL_DEBUG_ERROR, __func__,"vapname is error");
        return error;
    }

    if (unit == -1)
    {
        unit = ieee80211_new_wlanunit();
        if (unit == -1)
            return -EIO;        /* XXX */
        /* XXX verify name fits */
        snprintf(name, sizeof(name), "%s%d", cp.icp_name, unit);
    }
    else
    {
        if (!ieee80211_alloc_wlanunit(unit))
            return -1;
        strncpy(name, cp.icp_name, sizeof(name));
    }

    for( i = 0 ;i < MAX_VMAC_INSTANCE_AP;i++)
    {
        if(wlpptr->vdev[i] == NULL)
        {
            break;
        }
    }

    wlinit_vapmac(macaddr,wlpptr->hwData.macAddr,i);

    WlLogPrint(MARVEL_DEBUG_DEBUG, __func__,"%02x:%02x:%02x:%02x:%02x:%02x\n",\
    macaddr[0],macaddr[1],macaddr[2],macaddr[3],macaddr[4],macaddr[5]);
    WlLogPrint(MARVEL_DEBUG_DEBUG, __func__,"devname = %s\n",name);

    if(wlInit_vap(wlpptr,macaddr,name,i) != 0)
    {
        WlLogPrint(MARVEL_DEBUG_ERROR, __func__,"Create the vap %s interface fail.\n", name);
        return -1;
    }

    wl_vap_create_num_add();

    WlLogPrint(MARVEL_DEBUG_DEBUG, __func__,"Create the vap interface OK.\n");
    return 0;
}

int ieee80211_ioctl_destroy_ath(struct net_device *netdev)
{
    /*zhaoyang1 transplant from 717*/
    u_int32_t unit; /*modified by chenming*/
    /*zhaoyang1 transplant end*/
    int error;
    vmacApInfo_t  *vmac_ap ;
    struct wlprivate *wlpptr = NETDEV_PRIV_P(struct wlprivate, netdev) ;
    vmacApInfo_t *vmacSta_p = wlpptr->vmacSta_p;
    
    if(netdev == NULL)
        return -1;

    if(vmacSta_p->master)
        vmac_ap = vmacSta_p->master;
    else
        return -1;

    error = ifc_name2unit(netdev->name,&unit);
    if (error)
        return error;

    WlLogPrint(MARVEL_DEBUG_DEBUG, __func__,"%02x:%02x:%02x:%02x:%02x:%02x\n",\
    netdev->dev_addr[0],netdev->dev_addr[1],netdev->dev_addr[2],netdev->dev_addr[3],netdev->dev_addr[4],netdev->dev_addr[5]);
    WlLogPrint(MARVEL_DEBUG_DEBUG, __func__,"devname = %s\n",netdev->name);

    if (wl_vap_create_num_get() <= 0)
    {
        printk("No vap interface need delete.\n");
        return -1;
    }

    if(wlDeinit_vap(vmac_ap->dev,netdev) != 0)
    {
        WlLogPrint(MARVEL_DEBUG_ERROR, __func__,"Delete the vap %s interface fail.\n", netdev->name);
        return -1;
    }

    wl_vap_create_num_del();

    ieee80211_delete_wlanunit(unit);

    WlLogPrint(MARVEL_DEBUG_DEBUG, __func__,"Del the vap interface OK.\n");

    return 0;
}


