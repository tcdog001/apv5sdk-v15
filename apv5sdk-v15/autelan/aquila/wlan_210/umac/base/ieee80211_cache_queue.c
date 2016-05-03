/* ****************************************************************************************************
 * Filename: ieee80211_thinap.c
 * Description: a module for 80211 cache.
 * Project: Autelan ap 2011
 * Author: Mingzhe Duan
 * Date: 2011-10-06
 ********************************************************************************************************/
#include <ieee80211_var.h>
#include <linux/ip.h>
#include <linux/timer.h>
#include <ath_internal.h>
#include <if_athvar.h>

struct timeval cache_rx_last_time;
struct timeval cache_rx_current_time;
int first_schedule = 0;
#ifndef IEEE80211_NODEQ_MAX_LEN
#define IEEE80211_CACHEQ_MAX_LEN 50
#endif
#ifndef IEEE80211_NODEQ_MAX_LEN
#define IEEE80211_CACHE_MAX 5
#endif

/*避免出现浮点运算*/
#define CAR_SCALE_FACTOR_BIT_SHIFT  8
#define CAR_SCALE_FACTOR            (1 << CAR_SCALE_FACTOR_BIT_SHIFT)

#define IEEE80211_VAP_CACHEQ_INIT(_ncq) do {             \
			  spin_lock_init(&((_ncq)->cq_lock));			   \
			  (_ncq)->cq_len=0;							   \
			  (_ncq)->cq_whead=NULL;							   \
			  (_ncq)->cq_wtail=NULL;							   \
			  (_ncq)->cq_max_len_b = 200;   \
			  (_ncq)->cq_max_len_g = 200;   \
			  (_ncq)->cq_max_len_n = 200;   \
			  (_ncq)->cq_Byte=0; \
		 } while (0)

#define IEEE80211_VAP_CACHEQ_DESTROY(_ncq) do {      \
        spin_lock_destroy(&(_ncq)->cq_lock);        \
        (_ncq)->cq_len=0;                                              \
        KASSERT(((_ncq)->cq_whead == NULL), ("node cache queue is not empty")); \
    } while (0)

#define IEEE80211_VAP_CACHE_INIT(_nc) do {   \
		 (_nc)->c_cache_enable=0;      \
		 (_nc)->c_limit=0;							  \
		 (_nc)->c_send_threshold_b = IEEE80211_CACHEQ_MAX_LEN; 						  \
		 (_nc)->c_send_threshold_g = IEEE80211_CACHEQ_MAX_LEN; 						  \
		 (_nc)->c_send_threshold_n = IEEE80211_CACHEQ_MAX_LEN; 						  \
		 (_nc)->c_priority_b = 0;   \
	     (_nc)->c_priority_g = 0;   \
		 (_nc)->c_priority_n = 0;   \
		 (_nc)->c_send_count = 0; 				  \
         (_nc)->c_drop_count = 0;				\
         (_nc)->cache_rx_time.tv_usec = 0;  \
		 IEEE80211_VAP_CACHEQ_INIT(&((_nc)->c_queue));     \
	} while (0)

#define IEEE80211_NODE_CACHEQ(_ni)     (&(_ni->ni_cache.c_queue))
#define IEEE80211_NODE_CACHEQ_LOCK(_ncq)      spin_lock(&_ncq->cq_lock)
#define IEEE80211_NODE_CACHEQ_UNLOCK(_ncq)    spin_unlock(&_ncq->cq_lock)
#define IEEE80211_NODE_CACHEQ_FULL(_ncq)     ((_ncq)->cq_len >= (_ncq)->cq_max_len)

#define IEEE80211_NODE_CACHEQ_DEQUEUE(_ncq, _w) do {   \
    _w = _ncq->cq_whead;                       \
    if (_w) {                                    \
        _ncq->cq_whead =  wbuf_next(_w);        \
        wbuf_set_next(_w, NULL);                 \
        if ( _ncq->cq_whead ==  NULL)           \
            _ncq->cq_wtail =  NULL;             \
        --_ncq->cq_len;                         \
        _ncq->cq_Byte -= _w->len; \
    }                                            \
} while (0)

#define IEEE80211_NODE_CACHEQ_ENQUEUE(_ncq, _w) do { \
			wbuf_set_next(_w, NULL);							 \
			if (_ncq->cq_wtail != NULL) {						 \
				wbuf_set_next(_ncq->cq_wtail, _w); 			 \
				_ncq->cq_wtail = _w;							 \
			} else {											 \
				_ncq->cq_whead =  _ncq->cq_wtail =  _w;		 \
			}													 \
			_ncq->cq_len++;								 \
			_ncq->cq_Byte += _w->len; \
} while (0)

#define IEEE80211_NODE_CACHEQ_DESTROY(_nsq) do {      \
		spin_lock_destroy(&(_nsq)->nsq_lock);		 \
		(_nsq)->nsq_len=0;												\
		KASSERT(((_nsq)->nsq_whead == NULL), ("node powersave queue is not empty")); \
	} while (0)


void ieee80211_node_cacheq_detach(struct ieee80211_node *ni)
{
	struct ieee80211_cache_queue *ncq;
	wbuf_t send_wbuf;

	ncq = IEEE80211_NODE_CACHEQ(ni);
	//while(ni->ni_cache.c_queue.cq_Byte> 0)
	while(ni->ni_cache.c_queue.cq_len> 0)
	{						 
		IEEE80211_NODE_CACHEQ_LOCK(ncq);				
		IEEE80211_NODE_CACHEQ_DEQUEUE(ncq,send_wbuf);
		IEEE80211_NODE_CACHEQ_UNLOCK(ncq);
		wbuf_set_status(send_wbuf, WB_STATUS_TX_ERROR);
		wbuf_complete(send_wbuf);
	}		
	spin_lock_destroy(ni->ni_cache.c_queue.cq_lock);
}

/*
 * Clear any frames queued on a node's cache queue.
 */
void
ieee80211_node_cacheq_drain(struct ieee80211_node *ni)
{
	struct ieee80211_cache_queue *ncq;
	wbuf_t wbuf;
    struct ieee80211_tx_status ts;

    ts.ts_flags = IEEE80211_TX_ERROR;

	ncq = IEEE80211_NODE_CACHEQ(ni);  

	/*
	* free all the frames.
	*/
	IEEE80211_NODE_CACHEQ_LOCK(ncq);
	IEEE80211_NODE_CACHEQ_DEQUEUE(ncq, wbuf);
	while(wbuf) {
		ieee80211_release_wbuf(ni,wbuf, &ts);
		IEEE80211_NODE_CACHEQ_DEQUEUE(ncq, wbuf);
	}
	IEEE80211_NODE_CACHEQ_UNLOCK(ncq);
}

 void ieee80211_node_cacheq_attach(struct ieee80211vap *vap,struct ieee80211_node *ni)
 {
 
	ni->ni_cache.c_cache_enable = vap->vap_cache.c_cache_enable;				
	ni->ni_cache.c_limit = vap->vap_cache.c_limit;	
	ni->ni_cache.c_send_threshold_b = vap->vap_cache.c_send_threshold_b;
	ni->ni_cache.c_send_threshold_g = vap->vap_cache.c_send_threshold_g;
	ni->ni_cache.c_send_threshold_n = vap->vap_cache.c_send_threshold_n;
	ni->ni_cache.c_priority_b = vap->vap_cache.c_priority_b;
	ni->ni_cache.c_priority_g = vap->vap_cache.c_priority_g;
	ni->ni_cache.c_priority_n = vap->vap_cache.c_priority_n;
	IEEE80211_VAP_CACHEQ_INIT(&(ni->ni_cache.c_queue));
	ni->ni_cache.c_queue.cq_max_len_b = vap->vap_cache.c_queue.cq_max_len_b;
	ni->ni_cache.c_queue.cq_max_len_g = vap->vap_cache.c_queue.cq_max_len_g;	
	ni->ni_cache.c_queue.cq_max_len_n = vap->vap_cache.c_queue.cq_max_len_n;
	ni->ni_cache.c_queue.cq_len = 0;
	ni->ni_cache.c_send_count = vap->vap_cache.c_send_count;	
	ni->ni_cache.c_drop_count = vap->vap_cache.c_drop_count;
	ni->ni_cache.cache_rx_time.tv_usec = 0;
	ni->ni_cache.rate_in_us_per_byte = ((1000*1000) * (CAR_SCALE_FACTOR * 8))/(ni->ni_cache.c_limit * 1024);
	ni->ni_cache.depth_in_us = (ni->ni_cache.c_limit * 1024 * ni->ni_cache.rate_in_us_per_byte /8);
	
 }

void ieee80211_vap_cache_attach(struct ieee80211vap *vap)
{
	IEEE80211_VAP_CACHE_INIT(&(vap->vap_cache));
}

int ieee80211_node_cacheq_classify(struct ieee80211_node *ni, wbuf_t wbuf,struct ieee80211com *ic)
{
	struct ieee80211_cache_queue *ncq;
	u_int32_t maxlen = 0;
	//struct timeval cache_rx_now_time;
	u_int32_t cache_factor = 0;
	u_int32_t cache_factor_b= 0;
	u_int32_t cache_factor_g = 0;
	u_int32_t cache_factor_n = 0;
	if(ni->ni_mode == IEEE80211_STA_MODE_11B)
	{
		maxlen = ni->ni_cache.c_queue.cq_max_len_b;
	}
	else if(ni->ni_mode == IEEE80211_STA_MODE_11G)
	{
		maxlen = ni->ni_cache.c_queue.cq_max_len_g;
	}
	else if(ni->ni_mode == IEEE80211_STA_MODE_11N)
	{
		maxlen = ni->ni_cache.c_queue.cq_max_len_n;
	}

	//cache_rx_last_time.tv_usec		
	
	if(ni->ni_cache.c_cache_enable)
	{
		if(ic->ic_osdev->netdev->name[4] == '0')
		{
			cache_factor = cache_factor_wifi0;
			cache_factor_b = cache_factor_b_wifi0;
			cache_factor_g = cache_factor_g_wifi0;
			cache_factor_n = cache_factor_n_wifi0;
		}
		else
		{
			cache_factor = cache_factor_wifi1;
			cache_factor_b = cache_factor_b_wifi1;
			cache_factor_g = cache_factor_g_wifi1;
			cache_factor_n = cache_factor_n_wifi1;		
		}
		cache_all_pkt++;
		if(!first_schedule)
		{
			do_gettimeofday(&cache_rx_last_time);
			first_schedule++;
			tasklet_schedule(&ic->ic_cache_tx_tasklet_enqueue);
		}
		else
		{
			//timer_last_usec = cache_rx_last_time.tv_usec;
			do_gettimeofday(&cache_rx_current_time);
			timer_current_usec = cache_rx_current_time.tv_usec;
			if(cache_rx_current_time.tv_usec < cache_rx_last_time.tv_usec)
			{
				if(((1000000 - cache_rx_last_time.tv_usec) + cache_rx_current_time.tv_usec) >= cache_factor)
				{
					cache_rx_last_time.tv_usec = cache_rx_current_time.tv_usec;
					cache_schedule_taskcount++;
					tasklet_schedule(&ic->ic_cache_tx_tasklet_enqueue);
				}
				else
				{
					cache_not_schedule_taskcount++;
				}
			}
			else if((cache_rx_current_time.tv_usec - cache_rx_last_time.tv_usec) >= cache_factor)
			{
			    cache_rx_last_time.tv_usec = cache_rx_current_time.tv_usec;
				cache_schedule_taskcount++;
				tasklet_schedule(&ic->ic_cache_tx_tasklet_enqueue);
			}
			else
			{
				cache_not_schedule_taskcount++;
			}
		}
		//if((cache_all_pkt % cache_factor) == 1)
		//	tasklet_schedule(&ic->ic_cache_tx_tasklet_enqueue);
		//if(ni->ni_cache.c_queue.cq_Byte <= maxlen)
		if(ni->ni_cache.c_queue.cq_len <= maxlen)
		{	

			
			ni->ni_cache.c_send_count++;
			ncq = IEEE80211_NODE_CACHEQ(ni);
			//do_gettimeofday(&cache_rx_now_time);
#if 1
			if(wbuf->len <= cache_pass_factor)
			{
				IEEE80211_NODE_CACHEQ_LOCK(ncq);
				IEEE80211_NODE_CACHEQ_ENQUEUE(ncq,wbuf);
				IEEE80211_NODE_CACHEQ_UNLOCK(ncq);
				cache_pass_count++;
				return 1;
			}
#endif
			if(ni->ni_mode == IEEE80211_STA_MODE_11B)
			{
				timerb_last_usec = ni->ni_cache.cache_rx_time.tv_usec;
				if(ni->ni_cache.cache_rx_time.tv_sec == 0)
				{
					ni->ni_cache.cache_rx_time.tv_usec = cache_rx_last_time.tv_usec;
					ni->ni_cache.cache_rx_time.tv_sec = cache_rx_last_time.tv_sec;					
				}
				else
				{
					/*rate limit based by packet number*/
					if(cache_factor_b != 0)
					{
						/*反转发生*/
						if(cache_rx_current_time.tv_usec < ni->ni_cache.cache_rx_time.tv_usec)
						{
							if(((1000000 - ni->ni_cache.cache_rx_time.tv_usec) + cache_rx_current_time.tv_usec) < cache_factor_b)
							{
								//ni->ni_cache.cache_rx_time.tv_usec = cache_rx_current_time.tv_usec;
								cache_11b_enqueue_drop++;
								return -2;
							}
						}	
						else if((cache_rx_current_time.tv_usec - ni->ni_cache.cache_rx_time.tv_usec) < cache_factor_b)
						{
						    //ni->ni_cache.cache_rx_time.tv_usec = cache_rx_current_time.tv_usec;
							cache_11b_enqueue_drop++;
							return -2;
						}
						
						/*if not drop current packet, then update the time*/
						ni->ni_cache.cache_rx_time.tv_usec = ni->ni_cache.cache_rx_time.tv_usec + cache_factor_b; //cache_rx_current_time.tv_usec;
						if(ni->ni_cache.cache_rx_time.tv_usec > 1000000)
						{
							ni->ni_cache.cache_rx_time.tv_usec = ni->ni_cache.cache_rx_time.tv_usec - 1000000;
						}
					}

					/*rate limit based on kbps*/
					if(ni->ni_cache.c_limit != 0)
					{
						u_int32_t us_elapsed = 0; /*两个报文间隔的时常*/
						u_int32_t us_reqd    = 0; /*发送这个报文需要多少时常*/

						us_reqd    = (ni->ni_cache.rate_in_us_per_byte * wbuf->len); 

						/*间隔时间不超过1秒的采样周期*/
						if((ni->ni_cache.cache_rx_time.tv_sec == cache_rx_current_time.tv_sec) ||
							(((cache_rx_current_time.tv_sec - ni->ni_cache.cache_rx_time.tv_sec) == 1) &&
							(cache_rx_current_time.tv_usec < ni->ni_cache.cache_rx_time.tv_usec)))
						{
							if(cache_rx_current_time.tv_usec < ni->ni_cache.cache_rx_time.tv_usec)
								us_elapsed = ((cache_rx_current_time.tv_usec +(1000000 - ni->ni_cache.cache_rx_time.tv_usec)) << CAR_SCALE_FACTOR_BIT_SHIFT);
							else
								us_elapsed = ((cache_rx_current_time.tv_usec - ni->ni_cache.cache_rx_time.tv_usec) << CAR_SCALE_FACTOR_BIT_SHIFT);	

							if (us_elapsed < us_reqd)
							{
								cache_11b_enqueue_drop++;
								return -2;
							}
							else/*发送报文*/
							{
								/*更新为发送成功后的时间戳*/
								ni->ni_cache.cache_rx_time.tv_usec = ni->ni_cache.cache_rx_time.tv_usec + (us_reqd >> CAR_SCALE_FACTOR_BIT_SHIFT);
								if(ni->ni_cache.cache_rx_time.tv_usec > 1000000)/*反转*/
								{
									ni->ni_cache.cache_rx_time.tv_usec = ni->ni_cache.cache_rx_time.tv_usec - 1000000;
									ni->ni_cache.cache_rx_time.tv_sec = cache_rx_current_time.tv_sec;
								}
							}
						}
						else
						{
							ni->ni_cache.cache_rx_time.tv_sec = cache_rx_current_time.tv_sec - 1;
							ni->ni_cache.cache_rx_time.tv_usec = cache_rx_current_time.tv_usec + (us_reqd >> CAR_SCALE_FACTOR_BIT_SHIFT);
						}
					}
				}
				
				cache_11b_all_pkt++;
				if(ni->ni_cache.c_priority_b != 0)
				{
	                if(ni->ni_cache.c_priority_b == 1)
	                { 	
	                	wbuf_set_priority(wbuf,WME_AC_BK);
	                    wbuf_set_tid(wbuf,0);
	                }
	                if(ni->ni_cache.c_priority_b == 2)
	                { 		
	                 	wbuf_set_priority(wbuf,WME_AC_VI);
	                    wbuf_set_tid(wbuf,5);
	                }
	                if(ni->ni_cache.c_priority_b == 3)
	                { 		
	                	wbuf_set_priority(wbuf,WME_AC_VO);
	                	wbuf_set_tid(wbuf,6);
	                }
				}
			}else if(ni->ni_mode == IEEE80211_STA_MODE_11G)
			{
				timerg_last_usec = ni->ni_cache.cache_rx_time.tv_usec;
				if(ni->ni_cache.cache_rx_time.tv_sec == 0)
				{
					ni->ni_cache.cache_rx_time.tv_usec = cache_rx_last_time.tv_usec;
					ni->ni_cache.cache_rx_time.tv_sec = cache_rx_last_time.tv_sec;					
				}
				else
				{
					/*rate limit based by packet number*/
					if(cache_factor_g != 0)
					{
						/*反转发生*/
						if(cache_rx_current_time.tv_usec < ni->ni_cache.cache_rx_time.tv_usec)
						{
							if(((1000000 - ni->ni_cache.cache_rx_time.tv_usec) + cache_rx_current_time.tv_usec) < cache_factor_g)
							{
								//ni->ni_cache.cache_rx_time.tv_usec = cache_rx_current_time.tv_usec;
								cache_11g_enqueue_drop++;
								return -2;
							}
						}	
						else if((cache_rx_current_time.tv_usec - ni->ni_cache.cache_rx_time.tv_usec) < cache_factor_g)
						{
						    //ni->ni_cache.cache_rx_time.tv_usec = cache_rx_current_time.tv_usec;
							cache_11g_enqueue_drop++;
							return -2;
						}
						
						/*if not drop current packet, then update the time*/
						ni->ni_cache.cache_rx_time.tv_usec = ni->ni_cache.cache_rx_time.tv_usec + cache_factor_g; //cache_rx_current_time.tv_usec;
						if(ni->ni_cache.cache_rx_time.tv_usec > 1000000)
						{
							ni->ni_cache.cache_rx_time.tv_usec = ni->ni_cache.cache_rx_time.tv_usec - 1000000;
						}
					}

					/*rate limit based on kbps*/
					if(ni->ni_cache.c_limit != 0)
					{
						u_int32_t us_elapsed = 0; /*两个报文间隔的时常*/
						u_int32_t us_reqd    = 0; /*发送这个报文需要多少时常*/

						us_reqd    = (ni->ni_cache.rate_in_us_per_byte * wbuf->len); 

						/*间隔时间不超过1秒的采样周期*/
						if((ni->ni_cache.cache_rx_time.tv_sec == cache_rx_current_time.tv_sec) ||
							(((cache_rx_current_time.tv_sec - ni->ni_cache.cache_rx_time.tv_sec) == 1) &&
							(cache_rx_current_time.tv_usec < ni->ni_cache.cache_rx_time.tv_usec)))
						{
							if(cache_rx_current_time.tv_usec < ni->ni_cache.cache_rx_time.tv_usec)
								us_elapsed = ((cache_rx_current_time.tv_usec +(1000000 - ni->ni_cache.cache_rx_time.tv_usec)) << CAR_SCALE_FACTOR_BIT_SHIFT);
							else
								us_elapsed = ((cache_rx_current_time.tv_usec - ni->ni_cache.cache_rx_time.tv_usec) << CAR_SCALE_FACTOR_BIT_SHIFT);	

							if (us_elapsed < us_reqd)
							{
								cache_11g_enqueue_drop++;
								return -2;
							}
							else/*发送报文*/
							{
								/*更新为发送成功后的时间戳*/
								ni->ni_cache.cache_rx_time.tv_usec = ni->ni_cache.cache_rx_time.tv_usec + (us_reqd >> CAR_SCALE_FACTOR_BIT_SHIFT);
								if(ni->ni_cache.cache_rx_time.tv_usec > 1000000)/*反转*/
								{
									ni->ni_cache.cache_rx_time.tv_usec = ni->ni_cache.cache_rx_time.tv_usec - 1000000;
									ni->ni_cache.cache_rx_time.tv_sec = cache_rx_current_time.tv_sec;
								}
							}
						}
						else
						{
							ni->ni_cache.cache_rx_time.tv_sec = cache_rx_current_time.tv_sec - 1;
							ni->ni_cache.cache_rx_time.tv_usec = cache_rx_current_time.tv_usec + (us_reqd >> CAR_SCALE_FACTOR_BIT_SHIFT);
						}
					}
				}

				cache_11g_all_pkt++;
				if(ni->ni_cache.c_priority_g != 0)
				{
	                if(ni->ni_cache.c_priority_g == 1)
	                { 	
	                	wbuf_set_priority(wbuf,WME_AC_BK);
	                    wbuf_set_tid(wbuf,0);
	                }
	                if(ni->ni_cache.c_priority_g == 2)
	                { 		
	                 	wbuf_set_priority(wbuf,WME_AC_VI);
	                    wbuf_set_tid(wbuf,5);
	                }
	                if(ni->ni_cache.c_priority_g == 3)
	                { 		
	                	wbuf_set_priority(wbuf,WME_AC_VO);
	                	wbuf_set_tid(wbuf,6);
	                }
				}
			}else if(ni->ni_mode == IEEE80211_STA_MODE_11N)
			{

				timern_last_usec = ni->ni_cache.cache_rx_time.tv_usec;
				if(ni->ni_cache.cache_rx_time.tv_sec == 0)
				{
					ni->ni_cache.cache_rx_time.tv_usec = cache_rx_last_time.tv_usec;
					ni->ni_cache.cache_rx_time.tv_sec = cache_rx_last_time.tv_sec;					
				}
				else
				{
					/*rate limit based by packet number*/
					if(cache_factor_n != 0)
					{
						/*反转发生*/
						if(cache_rx_current_time.tv_usec < ni->ni_cache.cache_rx_time.tv_usec)
						{
							if(((1000000 - ni->ni_cache.cache_rx_time.tv_usec) + cache_rx_current_time.tv_usec) < cache_factor_n)
							{
								//ni->ni_cache.cache_rx_time.tv_usec = cache_rx_current_time.tv_usec;
								cache_11n_enqueue_drop++;
								return -2;
							}
						}	
						else if((cache_rx_current_time.tv_usec - ni->ni_cache.cache_rx_time.tv_usec) < cache_factor_n)
						{
						    //ni->ni_cache.cache_rx_time.tv_usec = cache_rx_current_time.tv_usec;
							cache_11n_enqueue_drop++;
							return -2;
						}
						
						/*if not drop current packet, then update the time*/
						ni->ni_cache.cache_rx_time.tv_usec = ni->ni_cache.cache_rx_time.tv_usec + cache_factor_n; //cache_rx_current_time.tv_usec;
						if(ni->ni_cache.cache_rx_time.tv_usec > 1000000)
						{
							ni->ni_cache.cache_rx_time.tv_usec = ni->ni_cache.cache_rx_time.tv_usec - 1000000;
						}
					}

					/*rate limit based on kbps*/
					if(ni->ni_cache.c_limit != 0)
					{
						u_int32_t us_elapsed = 0; /*两个报文间隔的时常*/
						u_int32_t us_reqd    = 0; /*发送这个报文需要多少时常*/

						us_reqd    = (ni->ni_cache.rate_in_us_per_byte * wbuf->len); 

						/*间隔时间不超过1秒的采样周期*/
						if((ni->ni_cache.cache_rx_time.tv_sec == cache_rx_current_time.tv_sec) ||
							(((cache_rx_current_time.tv_sec - ni->ni_cache.cache_rx_time.tv_sec) == 1) &&
							(cache_rx_current_time.tv_usec < ni->ni_cache.cache_rx_time.tv_usec)))
						{
							if(cache_rx_current_time.tv_usec < ni->ni_cache.cache_rx_time.tv_usec)
								us_elapsed = ((cache_rx_current_time.tv_usec +(1000000 - ni->ni_cache.cache_rx_time.tv_usec)) << CAR_SCALE_FACTOR_BIT_SHIFT);
							else
								us_elapsed = ((cache_rx_current_time.tv_usec - ni->ni_cache.cache_rx_time.tv_usec) << CAR_SCALE_FACTOR_BIT_SHIFT);	

							if (us_elapsed < us_reqd)
							{
								cache_11n_enqueue_drop++;
								return -2;
							}
							else/*发送报文*/
							{
								/*更新为发送成功后的时间戳*/
								ni->ni_cache.cache_rx_time.tv_usec = ni->ni_cache.cache_rx_time.tv_usec + (us_reqd >> CAR_SCALE_FACTOR_BIT_SHIFT);
								if(ni->ni_cache.cache_rx_time.tv_usec > 1000000)/*反转*/
								{
									ni->ni_cache.cache_rx_time.tv_usec = ni->ni_cache.cache_rx_time.tv_usec - 1000000;
									ni->ni_cache.cache_rx_time.tv_sec = cache_rx_current_time.tv_sec;
								}
							}
						}
						else
						{
							ni->ni_cache.cache_rx_time.tv_sec = cache_rx_current_time.tv_sec - 1;
							ni->ni_cache.cache_rx_time.tv_usec = cache_rx_current_time.tv_usec + (us_reqd >> CAR_SCALE_FACTOR_BIT_SHIFT);
						}
					}
				}

				cache_11n_all_pkt++;
				if(ni->ni_cache.c_priority_n != 0)
				{
	                if(ni->ni_cache.c_priority_n == 1)
	                { 	
	                	wbuf_set_priority(wbuf,WME_AC_BK);
	                    wbuf_set_tid(wbuf,0);
	                }
	                if(ni->ni_cache.c_priority_n == 2)
	                { 		
	                 	wbuf_set_priority(wbuf,WME_AC_VI);
	                    wbuf_set_tid(wbuf,5);
	                }
	                if(ni->ni_cache.c_priority_n == 3)
	                { 		
	                	wbuf_set_priority(wbuf,WME_AC_VO);
	                	wbuf_set_tid(wbuf,6);
	                }
				}
			}
			if(wbuf->len > cache_min_size && wbuf->len < cache_max_size)
			{
				cache_size_count++;
			}
			IEEE80211_NODE_CACHEQ_LOCK(ncq);
			IEEE80211_NODE_CACHEQ_ENQUEUE(ncq,wbuf);
			IEEE80211_NODE_CACHEQ_UNLOCK(ncq);

			if(ic == NULL)
			{
				printk("%s ic is null\n",__func__);
				return -1;
			}
			
			return 1;
		}
		else
		{
			ni->ni_cache.c_drop_count++;
			if(ni->ni_mode == IEEE80211_STA_MODE_11B) //11b
	 		{
	 			cache_11b_drop++;
			}
			else if(ni->ni_mode == IEEE80211_STA_MODE_11G) //11g
	 		{
	 			cache_11g_drop++;
			}
			else if(ni->ni_mode == IEEE80211_STA_MODE_11N) //11g
	 		{
	 			cache_11n_drop++;
			}
			return -2; //drop frame
		}
	}
	return -1;
}

int ieee80211_node_cacheq_queue(struct ieee80211_node *ni, wbuf_t wbuf,struct ieee80211com *ic)
{
    int ret = -1;
   	ret = ieee80211_node_cacheq_classify(ni,wbuf,ic);
    return ret;
}

void enqueue_cache_tx_tasklet_fn(struct ieee80211com *ic)
{
	struct ieee80211_node_table *nt = &ic->ic_sta; 
	struct ieee80211_node * ni = NULL;	
	struct ieee80211_cache_queue *ncq = NULL;
	wbuf_t send_wbuf;
	if(nt == NULL)
	{
		printk("%s nt is NULL\n",__func__);
		return;
	}
	
	cache_timer_duration1 = jiffies_to_msecs(jiffies);
	TAILQ_FOREACH(ni, &nt->nt_node, ni_list){
		
			//if(ni != ni->ni_vap->iv_bss)
 			{
	 			if(ni->ni_cache.c_cache_enable)
	 			{
	 				if(ni->ni_mode == IEEE80211_STA_MODE_11B) //11b
	 				{
		 				//if(ni->ni_cache.c_queue.cq_Byte >= ni->ni_cache.c_send_threshold_b)
						{
							int count = 0;
							ncq = IEEE80211_NODE_CACHEQ(ni);
							if(ncq == NULL)
							{
								continue;
							}
							if(ni->ni_cache.c_queue.cq_len)
							{
								dequeue_cache_11b_deep = ni->ni_cache.c_queue.cq_len;
							}
							if(cache_debug == 1)
							{
								printk("timer_fn 11b q_len = %d\n",ni->ni_cache.c_queue.cq_len);								
								printk("timer_fn 11b c_send_count = %d\n",ni->ni_cache.c_send_count);
							}
							while(ni->ni_cache.c_queue.cq_len > 0)
							{		
								if(count >= ni->ni_cache.c_send_threshold_b/* || ni->ni_cache.c_queue.cq_len == 0*/)
								{
									cache_11b_send_overrun++;
									break;
								}
								IEEE80211_NODE_CACHEQ_LOCK(ncq);				
								IEEE80211_NODE_CACHEQ_DEQUEUE(ncq,send_wbuf);
								IEEE80211_NODE_CACHEQ_UNLOCK(ncq);
								count++;
								ieee80211_send_wbuf(ni->ni_vap,ni,send_wbuf); 
								cache_11b_sendcount++;
							}
							if(ni->ni_cache.c_queue.cq_len == 0)
							{
								cache_11b_send_not_overrun++;
							}
							
							if(cache_debug == 1)
							{
								printk("timer_fn 11b cache send = %d\n",count);								
							}
						}
	 				}
					else if(ni->ni_mode == IEEE80211_STA_MODE_11G) //11g
					{
						//if(ni->ni_cache.c_queue.cq_Byte >= ni->ni_cache.c_send_threshold_g)
						{
							int count = 0;
							ncq = IEEE80211_NODE_CACHEQ(ni);
							if(ncq == NULL)
							{
								continue;
							}
							if(ni->ni_cache.c_queue.cq_len)
							{
								dequeue_cache_11g_deep = ni->ni_cache.c_queue.cq_len;
							}							
							if(cache_debug == 1)
							{
								printk("timer_fn 11g q_len = %d\n",ni->ni_cache.c_queue.cq_len);								
								printk("timer_fn 11g c_send_count = %d\n",ni->ni_cache.c_send_count);
							}
							while(ni->ni_cache.c_queue.cq_len > 0)
							{		
								if(count >= ni->ni_cache.c_send_threshold_g/* || ni->ni_cache.c_queue.cq_len == 0*/)
								{
									cache_11g_send_overrun++;
									break;
								}
								IEEE80211_NODE_CACHEQ_LOCK(ncq);				
								IEEE80211_NODE_CACHEQ_DEQUEUE(ncq,send_wbuf);
								IEEE80211_NODE_CACHEQ_UNLOCK(ncq);
								count++;
								ieee80211_send_wbuf(ni->ni_vap,ni,send_wbuf); 
								cache_11g_sendcount++;
							}	
							if(ni->ni_cache.c_queue.cq_len == 0)
							{
								cache_11g_send_not_overrun++;
							}
							
							if(cache_debug == 1)
							{
								printk("timer_fn 11g cache send = %d\n",count);								
							}
						}
					}
					else if(ni->ni_mode == IEEE80211_STA_MODE_11N) //11n
					{
						//if(ni->ni_cache.c_queue.cq_len >= ni->ni_cache.c_send_threshold_n)
						{
							int count = 0;
							ncq = IEEE80211_NODE_CACHEQ(ni);
							if(ncq == NULL)
							{
								continue;
							}
							if(ni->ni_cache.c_queue.cq_len)
							{
								dequeue_cache_11n_deep = ni->ni_cache.c_queue.cq_len;
							}
							
							if(cache_debug == 1)
							{
								printk("timer_fn 11n q_len = %d\n",ni->ni_cache.c_queue.cq_len);								
								printk("timer_fn 11n c_send_count = %d\n",ni->ni_cache.c_send_count);
							}
							while(ni->ni_cache.c_queue.cq_len > 0)
							{		
								if(count >= ni->ni_cache.c_send_threshold_n/* || ni->ni_cache.c_queue.cq_len == 0*/)
								{
									cache_11n_send_overrun++;
									break;
								}
								IEEE80211_NODE_CACHEQ_LOCK(ncq);				
								IEEE80211_NODE_CACHEQ_DEQUEUE(ncq,send_wbuf);
								IEEE80211_NODE_CACHEQ_UNLOCK(ncq);
								count++;
								ieee80211_send_wbuf(ni->ni_vap,ni,send_wbuf); 
								cache_11n_sendcount++;
							}			
							if(ni->ni_cache.c_queue.cq_len == 0)
							{
								cache_11n_send_not_overrun++;
							}
							
							if(cache_debug == 1)
							{
								printk("timer_fn 11n cache send = %d\n",count);								
							}
						}
					}
	 			}
 			}
	}
#if 1
	if(cache_debug == 2)
	{
		cache_debug = 1;
	}else if(cache_debug == 1)
	{
		cache_debug = 0;
	}
#endif
}

EXPORT_SYMBOL(enqueue_cache_tx_tasklet_fn);




