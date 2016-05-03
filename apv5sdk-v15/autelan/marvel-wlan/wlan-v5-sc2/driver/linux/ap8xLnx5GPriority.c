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

#include <linux/jiffies.h>
#include "ap8xLnx5GPriority.h"
#include "wllog.h"
#include "StaDb.h"

/*<Begin : add for 5G priority by caizhibang  2012-09-27 */
/*zhaoyang add for priority join in 5G*/

struct userinfo_table userinfo_table_t[IEEE80211_USERINFO_MAX];
EXPORT_SYMBOL(userinfo_table_t);
#define IEEE80211_ADDR_COPY(dst,src)    memcpy(dst,src,IEEEtypes_ADDRESS_SIZE)
#define IEEE80211_ADDR_EQ(a1,a2)        (memcmp(a1, a2, IEEE80211_ADDR_LEN) == 0)

#define IS_BROADCAST(macaddr) ((*(UINT32*)macaddr == 0xffffffff ) &&  \
                          *(UINT16 *)((UINT8*)macaddr+4) == 0xffff)

vmacApInfo_t *ieee80211_get_master_vmacApInfo(vmacApInfo_t *vmacSta_p)
{
    vmacApInfo_t *vmacSta_master;

    if(vmacSta_p == NULL)
        return NULL;

    if(vmacSta_p->master)
    {
        vmacSta_master = vmacSta_p->master;
    }
    else
    {
        vmacSta_master = vmacSta_p;
    }

    return vmacSta_master;
}

int ieee80211_get_same_essid_netdev(vmacApInfo_t *dst_vmacSta_p,vmacApInfo_t *src_vmacSta_p)
{
    int i;
    vmacApInfo_t *dst_vmacSta_master,*tmp_vmacSta_p;
    struct wlprivate *wlpptr,*tmp_wlpptr;
    MIB_802DOT11 *src_mib,*tmp_mib;
    UINT32 src_SsidLen,tmp_SsidLen;

    dst_vmacSta_master = ieee80211_get_master_vmacApInfo(dst_vmacSta_p);
    
    if((dst_vmacSta_master == NULL) || (src_vmacSta_p == NULL) || (src_vmacSta_p->master == NULL))
    {
        return 2;
    }

    src_mib = src_vmacSta_p->ShadowMib802dot11;
    src_SsidLen = strlen(&(src_mib->StationConfig->DesiredSsId[0]));

    wlpptr = NETDEV_PRIV_P(struct wlprivate,dst_vmacSta_master->dev);

#if defined(SINGLE_DEV_INTERFACE) && !defined(CLIENTONLY)
    for (i = 1; i < MAX_VMAC_INSTANCE_AP; i++)
#else
    for (i = 0; i < MAX_VMAC_INSTANCE_AP; i++)
#endif
    {
        if(wlpptr->vdev[i] == NULL)
        {
            continue;
        }

        tmp_wlpptr = NETDEV_PRIV_P(struct wlprivate,wlpptr->vdev[i]);
        tmp_vmacSta_p = tmp_wlpptr->vmacSta_p;
        tmp_mib = tmp_vmacSta_p->ShadowMib802dot11;
        tmp_SsidLen = strlen(&(tmp_mib->StationConfig->DesiredSsId[0]));

        if((tmp_SsidLen == src_SsidLen) && \
           (memcmp(&(src_mib->StationConfig->DesiredSsId[0]),&(tmp_mib->StationConfig->DesiredSsId[0]),src_SsidLen) == 0))
        {
            return 0;
        }
    }
    return 1;
}

int 
ieee80211_has_5G_priority_sta(u_int8_t  sta_mac[IEEE80211_ADDR_LEN])
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



int ieee80211_create_5G_priority_sta(u_int8_t  sta_mac[IEEE80211_ADDR_LEN])
{
	int i,flag =0;
    unsigned long irq_lock_flags;   /* Temp variable, used for irq lock/unlock parameter. wangjia 2012-10-11 */
	unsigned long min=0;
	int min_index,ret_index;

    min_index = 0;
    ret_index = 0;
    
	for(i = 0; i < IEEE80211_USERINFO_MAX; i++)
	{
		if(!(userinfo_table_t[i].ni_flags & IEEE80211_TABLE_USED))// this per was using 10000000 == 00000000
		{
		    /**
		     *  Add irq lock, spin_lock_irqsave/spin_unlock_irqrestore. wangjia 2012-10-11 
		     */ 
		    spin_lock_init(&(userinfo_table_t[i].userinfo_lock));
            
		    spin_lock_irqsave(&(userinfo_table_t[i].userinfo_lock), irq_lock_flags);


            memset(&userinfo_table_t[i],0,sizeof(struct userinfo_table));
            IEEE80211_ADDR_COPY(userinfo_table_t[i].ni_mac,sta_mac);
			userinfo_table_t[i].stamp_time = jiffies_to_msecs(jiffies);
			userinfo_table_t[i].ni_flags = IEEE80211_TABLE_USED;
			
			flag = 1;
			ret_index = i;

            spin_unlock_irqrestore(&(userinfo_table_t[i].userinfo_lock), irq_lock_flags);
			if(1)
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

        memset(&userinfo_table_t[i],0,sizeof(struct userinfo_table));
        IEEE80211_ADDR_COPY(userinfo_table_t[min_index].ni_mac,sta_mac);		
        userinfo_table_t[min_index].stamp_time = jiffies_to_msecs(jiffies);
		userinfo_table_t[min_index].ni_flags = IEEE80211_TABLE_USED;//10000000
		spin_unlock_irqrestore(&(userinfo_table_t[min_index].userinfo_lock), irq_lock_flags);
        
		if(1)
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


int ieee80211_autelan_5G_priority_enable(vmacApInfo_t *vmacSta_p,UINT8 *sta_mac,UINT32 RSSI,UINT8 subtype)
{
    int sta_index;
    unsigned int current_time,CurrChan;
    unsigned long irq_lock_flags;  /* Temp variable, used for irq lock/unlock parameter. wangjia 2012-10-11 */

    if((vmacSta_p == NULL) || (sta_mac == NULL))
    {
        WlLogPrint(MARVEL_DEBUG_WARNING, __func__, "dev is null\n");
        return -1;
    }

    if(IS_GROUP(sta_mac))
    {
        return 1;
    }

    CurrChan = vmacSta_p->Mib802dot11->PhyDSSSTable->CurrChan;

    WlLogPrint(MARVEL_DEBUG_DEBUG, __func__, "Sta[%02X:%02X:%02X:%02X:%02X:%02X] RSSI: %d CurrChan: %d\n",\
        sta_mac[0], sta_mac[1],sta_mac[2], sta_mac[3],sta_mac[4], sta_mac[5],RSSI,CurrChan);

    if(((CurrChan >= 1 && CurrChan <= 14) && (RSSI >= sta_signal_strength_thr)) || \
    (CurrChan > 14))
    {
        struct net_device *tmp_dev = NULL;

        vmacApInfo_t *vmacSta_p_master = ieee80211_get_master_vmacApInfo(vmacSta_p);
        
        if (memcmp(vmacSta_p_master->dev->name, "wifi0", 5) == 0) 
        {
#if LINUX_VERSION_CODE <= KERNEL_VERSION(2,6,22)
            tmp_dev = dev_get_by_name("wifi1");
#else
            tmp_dev = dev_get_by_name(&init_net,"wifi1");
#endif
        }
        else if (memcmp(vmacSta_p_master->dev->name, "wifi1", 5) == 0)
        {
#if LINUX_VERSION_CODE <= KERNEL_VERSION(2,6,22)
            tmp_dev = dev_get_by_name("wifi0");
#else
            tmp_dev = dev_get_by_name(&init_net,"wifi0");
#endif
        }

        if (tmp_dev)
        {
            struct wlprivate *tmp_wlpptr = NETDEV_PRIV_P(struct wlprivate, tmp_dev);
            vmacApInfo_t *tmp_vmacSta_p = tmp_wlpptr->vmacSta_p;
            UINT32 tmp_entries,entries;
            UINT32 tmp_CurrChan = tmp_vmacSta_p->Mib802dot11->PhyDSSSTable->CurrChan;
            entries = extStaDb_entries(vmacSta_p, 0);
            tmp_entries = extStaDb_entries(tmp_vmacSta_p, 0);

            /* Begin, Add for the judgement of 5G priority sta threshold. wangjia 2012-10-11 */
            if(((CurrChan > 14) && (entries < stacount_thr)) ||
               ((tmp_CurrChan >= 14) && (tmp_entries < stacount_thr)))
            {
                if(ieee80211_get_same_essid_netdev(tmp_vmacSta_p,vmacSta_p) == 0)
                {

                    WlLogPrint(MARVEL_DEBUG_ALL, __func__,"find same essid from other netdev\n");

                    sta_index = ieee80211_has_5G_priority_sta(sta_mac);

                    if(subtype == IEEE_MSG_PROBE_RQST)
                    {

                        WlLogPrint(MARVEL_DEBUG_DEBUG, __func__,"AP Recv Probe Request\n");
                    	if(sta_index == -1)
                    	{
                    		sta_index = ieee80211_create_5G_priority_sta(sta_mac);
                    	}

                        current_time = jiffies_to_msecs(jiffies);
                        
                        if(CurrChan >= 1 && CurrChan <= 14)
                        {
                    	    if(userinfo_table_t[sta_index].marked == 0)
                        	{
                                WlLogPrint(MARVEL_DEBUG_ALL, __func__,"scantime = %d ,scantime_thr = %d\n",current_time - userinfo_table_t[sta_index].stamp_time,scantime_thr);
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
                                        WlLogPrint(MARVEL_DEBUG_ALL, __func__,"finish sta scan!!!\n");
                                        userinfo_table_t[sta_index].marked = 1;
                                    }
                                    spin_unlock_irqrestore(&(userinfo_table_t[sta_index].userinfo_lock), irq_lock_flags);

                                    WlLogPrint(MARVEL_DEBUG_ALL, __func__,"recv probe request count = %d\n",userinfo_table_t[sta_index].count);
                                    dev_put(tmp_dev); /* Release reference to device. wangjia 2012-10-11 */
                                    return 0;
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
                                    WlLogPrint(MARVEL_DEBUG_ALL, __func__,"scan time expire,reset sta scan\n");
                        			return 0;
                        		}
                        	}
                    	    else
                        	{
                                if(current_time - userinfo_table_t[sta_index].stamp_time <= agingtime_thr) 
                                {
                                    if(userinfo_table_t[sta_index].ni_flags & IEEE80211_TABLE_SUPPORT5G)
                                    {
                                        WlLogPrint(MARVEL_DEBUG_DEBUG, __func__,"sta support 5.8G,but CurrChan is 2.4G,no send Probe Response\n");
                                        dev_put(tmp_dev);
                                        return 0;
                                    }
                                    WlLogPrint(MARVEL_DEBUG_WARNING, __func__,"sta no support 5.8G,will send Probe Response\n");
                                    return 1;
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
                                    WlLogPrint(MARVEL_DEBUG_ALL, __func__,"ageing time expire,reset sta scan\n");
                                    return 0;
                        		}
                        	}
                        }
                        else if (CurrChan > 14)
                        {
                            WlLogPrint(MARVEL_DEBUG_WARNING, __func__, "CurrChan is 5.8G,will send Probe Response\n");
                            /**
                		     *  spin_lock_irqsave/spin_unlock_irqrestore. wangjia 2012-10-11 
                		     */ 
                            spin_lock_irqsave(&(userinfo_table_t[sta_index].userinfo_lock), irq_lock_flags);
                            userinfo_table_t[sta_index].ni_flags |= IEEE80211_TABLE_SUPPORT5G;// set the 5g flag	
                            userinfo_table_t[sta_index].marked =1;
                            userinfo_table_t[sta_index].stamp_time = current_time;
                            spin_unlock_irqrestore(&(userinfo_table_t[sta_index].userinfo_lock), irq_lock_flags);
                            return 1;
                        }

                    }
                    else if(subtype == IEEE_MSG_AUTHENTICATE)	
                    {
                        WlLogPrint(MARVEL_DEBUG_DEBUG, __func__,"AP Recv Auth Request\n");
                        if(sta_index != -1)
                        {
                            if(CurrChan >= 1 && CurrChan <= 14)
                            {
                        		if(userinfo_table_t[sta_index].marked == 0)
                        		{ 
                        		    WlLogPrint(MARVEL_DEBUG_DEBUG, __func__,"sta no finish scan,no send Auth Response\n");
                        		    dev_put(tmp_dev);
                        		    return 0;
                        		}
                        		else
                        		{
                                    if(userinfo_table_t[sta_index].ni_flags & IEEE80211_TABLE_SUPPORT5G)
                                    {
                                        WlLogPrint(MARVEL_DEBUG_DEBUG, __func__,"sta support 5.8G,no send Auth Response\n");
                                        dev_put(tmp_dev);
                                        return 0;
                                    }
                                    WlLogPrint(MARVEL_DEBUG_WARNING, __func__,"sta no support 5.8G,will send Auth Response\n");
                        		}
                            }
                        }
                        else
                        {
                            WlLogPrint(MARVEL_DEBUG_WARNING, __func__,"sta no support 5.8G Priority,will send Probe Response\n");
                        }
                    }
                    else if(subtype == IEEE_MSG_ASSOCIATE_RQST)
                    {
                        if(sta_index != -1)
                        {
                            userinfo_table_t[sta_index].ni_flags &= ~(IEEE80211_TABLE_USED);
                        }
                    }
                }
                else
                {
                    WlLogPrint(MARVEL_DEBUG_WARNING, __func__, "no find same essid from other Card\n");
                }
            }
            else
            {
                WlLogPrint(MARVEL_DEBUG_WARNING, __func__, "sta count greater than 5.8G traffic balance threshold\n");
            }
            dev_put(tmp_dev); /* Release reference to device. wangjia 2012-10-11 */
        }
        else
        {
            WlLogPrint(MARVEL_DEBUG_WARNING, __func__, "AP is Single Card device,no support 5.8G Priority function!\n");
        }
    }
    else
    {
        WlLogPrint(MARVEL_DEBUG_WARNING, __func__, "RSSI less than sta_signal_strength_thr or CurrChan is 5.8G\n");
    }

    return 1;
}


