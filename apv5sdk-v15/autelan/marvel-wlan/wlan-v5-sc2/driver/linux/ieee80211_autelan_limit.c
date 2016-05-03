/* ****************************************************************************************************
 * Filename: ieee80211_autelan_limit.c
 * Description: a module for traffic limit.
 * Project: Autelan ap 2012
 * Author: Mingzhe Duan
 * Date: 2012-05-17
 ********************************************************************************************************/
//#include <ieee80211_var.h>
#include <ieee80211_autelan_limit.h>

struct timeval current_time;
u_int32_t au_pkt = 0;
u_int32_t pkt_next = 0;
u_int32_t drop_pkt = 0;

u_int32_t ls_s[3] = {0};
u_int32_t ls_us[3] = {0};
u_int32_t cur_s[3] = {0};
u_int32_t cur_us[3] = {0};
u_int32_t elapsed[3] = {0};
u_int32_t reqd[3] = {0};
u_int32_t drop[3] = {0};
u_int32_t g_pkt[3] = {0};
u_int32_t rx_reqd[3] = {0};

struct ieee80211_frame
{
	IEEEtypes_FrameCtl_t FrmCtl;
	UINT8	dur[2];
	UINT8	addr1[IEEEtypes_ADDRESS_SIZE];
	UINT8	addr2[IEEEtypes_ADDRESS_SIZE];
	UINT8	addr3[IEEEtypes_ADDRESS_SIZE];
	UINT8	seq[2];
	UINT8	addr4[IEEEtypes_ADDRESS_SIZE];
} PACK;


void autelan_traffic_limit_init(struct autelan_traffic_limit * tl)
{
	tl->traffic_limit_flag = 0;
	tl->rx_traffic_limit = 0;
	tl->rx_rate_in_us_per_byte = 0;
	tl->tx_traffic_limit = 0;
	tl->tx_rate_in_us_per_byte = 0;
}
void autelan_traffic_limit_vaptoni(struct autelan_traffic_limit * iv_tl,struct autelan_traffic_limit * ni_tl)
{
	ni_tl->traffic_limit_flag = iv_tl->traffic_limit_flag;
	ni_tl->rx_traffic_limit = iv_tl->rx_traffic_limit;
	ni_tl->rx_rate_in_us_per_byte = iv_tl->rx_rate_in_us_per_byte;
	ni_tl->tx_traffic_limit = iv_tl->tx_traffic_limit;
	ni_tl->tx_rate_in_us_per_byte = iv_tl->tx_rate_in_us_per_byte;
}

u_int32_t autelan_trafficlimit(struct autelan_traffic_limit * tl,u_int32_t wbuf_len,unsigned char flag)
{
	u_int32_t last_sec  = 0;
	u_int32_t last_usec = 0;
	u_int32_t traffic_limit = 0;
	u_int32_t rate_in_us_per_byte = 0;
	if(flag)
		au_pkt++;
	do_gettimeofday(&current_time);

	if(wbuf_len <= 0)//lhj//2012.06
		return 0;

#if 0
	if(wbuf->len > 200 && wbuf->len < 300)
	{
		printk("Begin flag = %d wbuf->len = %d----------------------------\n",flag,wbuf->len);

		int i =0;
		while (i <= wbuf->len)
		{
			if ((0 != (i%16)) || (0 == i))
			{
				printk("%02x ",wbuf->data[i]);
				i++;
				continue;
			}

			printk("\n");
			printk("%02x ",wbuf->data[i]);
			i++;					 
		}
		printk("End---------------------------------------\n\n");
	}
#endif
	//if(tl->rx_last_time.tv_sec == 0)  //lhj//2012.06//only check rx
	if(tl->rx_last_time.tv_sec == 0 || tl->tx_last_time.tv_sec == 0)  
	{
		if(flag)
		{
			tl->tx_last_time.tv_usec = current_time.tv_usec;
			tl->tx_last_time.tv_sec = current_time.tv_sec;				  
		}
		else
		{
			tl->rx_last_time.tv_usec = current_time.tv_usec;
			tl->rx_last_time.tv_sec = current_time.tv_sec;				  
		}
	}
	else
	{
		if(flag)
		{
			if(tl->tx_traffic_limit == 0)
				return 0;
			last_sec = tl->tx_last_time.tv_sec;
			last_usec = tl->tx_last_time.tv_usec;
			traffic_limit = tl->tx_traffic_limit;
			rate_in_us_per_byte = tl->tx_rate_in_us_per_byte;
		}
		else
		{
			if(tl->rx_traffic_limit == 0)
				return 0;
			last_sec = tl->rx_last_time.tv_sec;
			last_usec = tl->rx_last_time.tv_usec;				  
			traffic_limit = tl->rx_traffic_limit;
			rate_in_us_per_byte = tl->rx_rate_in_us_per_byte;
		}
  
		if(traffic_limit != 0)	
		{
			u_int32_t us_elapsed = 0; 
			u_int32_t us_reqd    = 0; 
			if(flag)
			{
				us_reqd = (rate_in_us_per_byte * wbuf_len);	
			}
			else
			{
				us_reqd = (rate_in_us_per_byte * wbuf_len);	
			}

			if((last_sec == current_time.tv_sec) ||(((current_time.tv_sec - last_sec) == 1) &&
			(current_time.tv_usec < last_usec)))  
			{		  	
				if(current_time.tv_usec < last_usec)
				{
					us_elapsed = ((current_time.tv_usec +(1000000 - last_usec)) << CAR_SCALE_FACTOR_BIT_SHIFT);
				}
				else
				{
					us_elapsed = ((current_time.tv_usec - last_usec) << CAR_SCALE_FACTOR_BIT_SHIFT);	
				}
				#if 0
				if(((au_pkt % 1000) || pkt_next != 0)&& flag)
				{
					if(pkt_next < 3)
					{
						//printk("<<%d>> last_sec = %d last_usec = %d cur_sec = %d cur_usec = %d us_elapsed = %d us_reqd = %d\n",pkt_next,last_sec,last_usec,current_time.tv_sec,current_time.tv_usec,us_elapsed,us_reqd);		
						ls_s[pkt_next] = last_sec;
						ls_us[pkt_next] = last_usec;
						cur_s[pkt_next] = current_time.tv_sec;
						cur_us[pkt_next] = current_time.tv_usec;
						elapsed[pkt_next] = us_elapsed;
						reqd[pkt_next] = us_reqd;
						drop[pkt_next] = drop_pkt;
						g_pkt[pkt_next]= au_pkt;
						pkt_next++;
					}
					else
					{
						pkt_next = 0;
					}
				}
				else
				{
				rx_reqd[0] = us_reqd;
				}
				#endif
				if (us_elapsed < us_reqd)
				{
					drop_pkt++;
					return 1; //drop this pkt
				}
				else
				{
					last_usec = last_usec + (us_reqd >> CAR_SCALE_FACTOR_BIT_SHIFT);
					if(last_usec > 1000000)
					{
						if(flag)
						{	
						tl->tx_last_time.tv_usec = last_usec - 1000000;
						tl->tx_last_time.tv_sec = current_time.tv_sec;
						}
						else
						{
							tl->rx_last_time.tv_usec = last_usec - 1000000;
							tl->rx_last_time.tv_sec = current_time.tv_sec;
						}
					}
					else
					{
						if(flag)
						{   
							tl->tx_last_time.tv_usec = last_usec;
							//tl->tx_last_time.tv_usec = current_time.tv_usec;
						}
						else
						{
							tl->rx_last_time.tv_usec = last_usec;
							//tl->rx_last_time.tv_usec = current_time.tv_usec;
						}
					}
				}
			}
			else
			{
				if(flag)
				{
					tl->tx_last_time.tv_sec = current_time.tv_sec - 1;
					tl->tx_last_time.tv_usec = current_time.tv_usec + (us_reqd >> CAR_SCALE_FACTOR_BIT_SHIFT);
				}
				else
				{
					tl->rx_last_time.tv_sec = current_time.tv_sec - 1;
					tl->rx_last_time.tv_usec = current_time.tv_usec + (us_reqd >> CAR_SCALE_FACTOR_BIT_SHIFT);
				}
			}
		}
	}
	return 0;
}

int ieee80211_autelan_traffic_limit_tx(struct net_device *netdev, struct sk_buff *skb,extStaDb_StaInfo_t *staInfo)
{
	struct wlprivate *wlpptr = NETDEV_PRIV_P(struct wlprivate, netdev);
	struct ieee80211vap *vap = &(wlpptr->vap);
    struct ieee80211_frame *wh = (struct ieee80211_frame *)(skb->data);
	
	if((0 == wh->FrmCtl.PwrMgmt) && (IEEE_TYPE_DATA == wh->FrmCtl.Type))
	{
	    if(vap->iv_tl.traffic_limit_flag)
	    {
	    	if(autelan_trafficlimit(&(vap->iv_tl),(skb->len - 14),TX_PKT))
	    	{
	    		return LIMIT_SUCCESS;
	    	}
	    }
		if(staInfo)
		{
			if(staInfo->ni_everynode_tl.traffic_limit_flag)
		    {
		    	if(autelan_trafficlimit(&(staInfo->ni_everynode_tl),(skb->len - 14),TX_PKT))
		    	{
		    		return LIMIT_SUCCESS;
		    	}
		    }
			if(staInfo->ni_special_tl.traffic_limit_flag)
		    {
		    	if(autelan_trafficlimit(&(staInfo->ni_special_tl),(skb->len - 14),TX_PKT))
		    	{
		    		return LIMIT_SUCCESS;
		    	}
		    }
		}
	}
	return LIMIT_FAIL;
}

int ieee80211_autelan_traffic_limit_rx(struct net_device *netdev, struct sk_buff *skb, extStaDb_StaInfo_t *pStaInfo)
{
    struct wlprivate *wlpptr;
    struct ieee80211vap *vap;
    UINT32 headerspace;
    
    if(netdev == NULL || skb == NULL || pStaInfo == NULL)
    {
        return 0;
    }

	wlpptr = NETDEV_PRIV_P(struct wlprivate, netdev);
    vap = &(wlpptr->vap);
    
    headerspace = sizeof(struct ieee80211_frame);
    
	if(vap->iv_tl.traffic_limit_flag)
	{
		if(autelan_trafficlimit(&(vap->iv_tl),(skb->len - (headerspace + 6)), RX_PKT))
		{
			return LIMIT_SUCCESS;
		}
	}
	if(pStaInfo->ni_everynode_tl.traffic_limit_flag)
	{
		if(autelan_trafficlimit(&(pStaInfo->ni_everynode_tl),(skb->len - (headerspace + 6)),RX_PKT))
		{
			return LIMIT_SUCCESS;
		}
	}
	if(pStaInfo->ni_special_tl.traffic_limit_flag)
	{
		if(autelan_trafficlimit(&(pStaInfo->ni_special_tl),(skb->len - (headerspace + 6)),RX_PKT))
		{
			return LIMIT_SUCCESS;
		}
	}
	return LIMIT_FAIL;
}


