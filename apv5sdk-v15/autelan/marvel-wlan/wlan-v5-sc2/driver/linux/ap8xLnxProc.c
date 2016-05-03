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
#include "ap8xLnxProc.h"

#include <linux/version.h>
#include <linux/module.h>
#include <linux/kmod.h>
#include <linux/init.h>
#include <linux/sched.h>
#include <linux/slab.h>

/*Begin:Added by duanmingzhe for thinanp*/
u_int32_t thinap = 1;   /*thin ap default*/
EXPORT_SYMBOL(thinap);
/*End:Added by duanmingzhe for thinanp*/
#ifdef AE5200_RT_SUPPORT
u_int32_t join5g_enable = 0;
EXPORT_SYMBOL(join5g_enable);
u_int32_t scantime_thr = 200;
EXPORT_SYMBOL(scantime_thr);
u_int32_t agingtime_thr = 10000;
EXPORT_SYMBOL(agingtime_thr);
u_int32_t discard_count = 6;
EXPORT_SYMBOL(discard_count);
u_int32_t join5g_debug = 0;
EXPORT_SYMBOL(join5g_debug);
u_int32_t stacount_thr = 30;
EXPORT_SYMBOL(stacount_thr);
/*Begin, station's 2.4G signal strength threshold, RSSI. wangjia 2012-10-16. */
u_int32_t sta_signal_strength_thr = 50;
EXPORT_SYMBOL(sta_signal_strength_thr);
/*End, wangjia 2012-10-16. */

#endif
/*Begin:Added by duanmingzhe for 80211 cache*/
u_int32_t cache_enable = 0;
EXPORT_SYMBOL(cache_enable);
u_int32_t cache_factor_wifi0 = 1;
EXPORT_SYMBOL(cache_factor_wifi0);
u_int32_t cache_factor_b_wifi0 = 0;
EXPORT_SYMBOL(cache_factor_b_wifi0);
u_int32_t cache_factor_g_wifi0 = 0;
EXPORT_SYMBOL(cache_factor_g_wifi0);
u_int32_t cache_factor_n_wifi0 = 0;
EXPORT_SYMBOL(cache_factor_n_wifi0);
u_int32_t cache_factor_wifi1 = 1;
EXPORT_SYMBOL(cache_factor_wifi1);
u_int32_t cache_factor_b_wifi1 = 0;
EXPORT_SYMBOL(cache_factor_b_wifi1);
u_int32_t cache_factor_g_wifi1 = 0;
EXPORT_SYMBOL(cache_factor_g_wifi1);
u_int32_t cache_factor_n_wifi1 = 0;
EXPORT_SYMBOL(cache_factor_n_wifi1);
u_int32_t cache_pass_factor = 256;
EXPORT_SYMBOL(cache_pass_factor);
u_int32_t cache_pass_count = 0;
EXPORT_SYMBOL(cache_pass_count);
u_int32_t cache_min_size = 256;
EXPORT_SYMBOL(cache_min_size);
u_int32_t cache_max_size = 1024;
EXPORT_SYMBOL(cache_max_size);
u_int32_t cache_size_count = 0;
EXPORT_SYMBOL(cache_size_count);
u_int32_t cache_debug = 0;
EXPORT_SYMBOL(cache_debug);
u_int32_t cache_timer = 200;
EXPORT_SYMBOL(cache_timer);
u_int32_t txok_cache_11b_count = 0;
EXPORT_SYMBOL(txok_cache_11b_count);
u_int32_t txok_cache_11g_count = 0;
EXPORT_SYMBOL(txok_cache_11g_count);
u_int32_t txok_cache_11n_count = 0;
EXPORT_SYMBOL(txok_cache_11n_count);
u_int32_t timer_cache_11b_count = 0;
EXPORT_SYMBOL(timer_cache_11b_count);
u_int32_t timer_cache_11g_count = 0;
EXPORT_SYMBOL(timer_cache_11g_count);
u_int32_t timer_cache_11n_count = 0;
EXPORT_SYMBOL(timer_cache_11n_count);
u_int32_t cache_11b_send_not_overrun = 0;
EXPORT_SYMBOL(cache_11b_send_not_overrun);
u_int32_t cache_11g_send_not_overrun = 0;
EXPORT_SYMBOL(cache_11g_send_not_overrun);
u_int32_t cache_11n_send_not_overrun = 0;
EXPORT_SYMBOL(cache_11n_send_not_overrun);
u_int32_t cache_11b_sendcount = 0;
EXPORT_SYMBOL(cache_11b_sendcount);
u_int32_t cache_11g_sendcount = 0;
EXPORT_SYMBOL(cache_11g_sendcount);
u_int32_t cache_11n_sendcount = 0;
EXPORT_SYMBOL(cache_11n_sendcount);
u_int32_t cache_schedule_taskcount = 0;
EXPORT_SYMBOL(cache_schedule_taskcount);
u_int32_t cache_11g_sendcount_timer = 0;
EXPORT_SYMBOL(cache_11g_sendcount_timer);
u_int32_t cache_not_schedule_taskcount = 0;
EXPORT_SYMBOL(cache_not_schedule_taskcount);
u_int32_t cache_11b_sendcount_tasklet = 0;
EXPORT_SYMBOL(cache_11b_sendcount_tasklet);
u_int32_t cache_11g_sendcount_tasklet = 0;
EXPORT_SYMBOL(cache_11g_sendcount_tasklet);
u_int32_t cache_11n_sendcount_tasklet = 0;
EXPORT_SYMBOL(cache_11n_sendcount_tasklet);
u_int32_t cache_11b_send_overrun = 0;
EXPORT_SYMBOL(cache_11b_send_overrun);
u_int32_t cache_11g_send_overrun = 0;
EXPORT_SYMBOL(cache_11g_send_overrun);
u_int32_t cache_11n_send_overrun = 0;
EXPORT_SYMBOL(cache_11n_send_overrun);
u_int32_t txok_cache_11b_len = 0;
EXPORT_SYMBOL(txok_cache_11b_len);
u_int32_t txok_cache_11g_len = 0;
EXPORT_SYMBOL(txok_cache_11g_len);
u_int32_t txok_cache_11n_len = 0;
EXPORT_SYMBOL(txok_cache_11n_len);
u_int32_t timerb_last_usec = 0;
EXPORT_SYMBOL(timerb_last_usec);
u_int32_t timerg_last_usec = 0;
EXPORT_SYMBOL(timerg_last_usec);
u_int32_t timern_last_usec = 0;
EXPORT_SYMBOL(timern_last_usec);
u_int32_t timer_current_usec = 0;
EXPORT_SYMBOL(timer_current_usec);
u_int32_t timer_cache_11n_len = 0;
EXPORT_SYMBOL(timer_cache_11n_len);
u_int32_t dequeue_cache_11b_deep = 0;
EXPORT_SYMBOL(dequeue_cache_11b_deep);
u_int32_t dequeue_cache_11g_deep = 0;
EXPORT_SYMBOL(dequeue_cache_11g_deep);
u_int32_t dequeue_cache_11n_deep = 0;
EXPORT_SYMBOL(dequeue_cache_11n_deep);
u_int32_t cache_11b_drop = 0;
EXPORT_SYMBOL(cache_11b_drop);
u_int32_t cache_11g_drop = 0;
EXPORT_SYMBOL(cache_11g_drop);
u_int32_t cache_11n_drop = 0;
EXPORT_SYMBOL(cache_11n_drop);
u_int32_t cache_11b_enqueue_drop = 0;
EXPORT_SYMBOL(cache_11b_enqueue_drop);
u_int32_t cache_11g_enqueue_drop = 0;
EXPORT_SYMBOL(cache_11g_enqueue_drop);
u_int32_t cache_11n_enqueue_drop = 0;
EXPORT_SYMBOL(cache_11n_enqueue_drop);

u_int32_t cache_all_pkt = 0;
EXPORT_SYMBOL(cache_all_pkt);
u_int32_t all_pkt = 0;
EXPORT_SYMBOL(all_pkt);
u_int32_t cache_11b_all_pkt = 0;
EXPORT_SYMBOL(cache_11b_all_pkt);
u_int32_t cache_11g_all_pkt = 0;
EXPORT_SYMBOL(cache_11g_all_pkt);
u_int32_t cache_11n_all_pkt = 0;
EXPORT_SYMBOL(cache_11n_all_pkt);
u_int32_t ieee80211_send_wbuf_internal_count = 0;
EXPORT_SYMBOL(ieee80211_send_wbuf_internal_count);
u_int32_t ath_netdev_hardstart_generic_count = 0;
EXPORT_SYMBOL(ath_netdev_hardstart_generic_count);
u_int32_t ath_tx_send_count = 0;
EXPORT_SYMBOL(ath_tx_send_count);
u_int32_t ath_tx_start_count = 0;
EXPORT_SYMBOL(ath_tx_start_count);
u_int32_t cache_timer_duration1 = 0;
EXPORT_SYMBOL(cache_timer_duration1);
u_int32_t cache_timer_duration2 = 0;
EXPORT_SYMBOL(cache_timer_duration2);
//struct tasklet_struct cache_tx_tasklet_timer;      /* tx foward tasklet */
//EXPORT_SYMBOL(cache_tx_tasklet_timer);
//struct tasklet_struct cache_tx_tasklet_enqueue;        /* tx foward tasklet */
//EXPORT_SYMBOL(cache_tx_tasklet_enqueue);
/*End:Added by duanmingzhe for 80211 cache*/
/*Begin:Added by duanmingzhe for traffic balance*/
u_int32_t traffic_balance = 0;
EXPORT_SYMBOL(traffic_balance);
/*End:Added by duanmingzhe for traffic balance*/
/*zhaoyang modify for send arp active when wireless to wireless*/
u_int32_t arp_send_active= 0;
EXPORT_SYMBOL(arp_send_active);
/*zhaoyang modify end*/
/*zhaoyang modify for tunnel data frame fast forward*/
u_int32_t tunnel_fast_forward =0;
EXPORT_SYMBOL(tunnel_fast_forward);
/*zhaoyang modify end*/
/*Begin: Added by zhanghu for increasing the rate */
u_int32_t rate_threshold = 0;
EXPORT_SYMBOL(rate_threshold);
u_int32_t rate_threshold_a = 0;
EXPORT_SYMBOL(rate_threshold_a);
u_int32_t rate_threshold_b = 0;
EXPORT_SYMBOL(rate_threshold_b);
u_int32_t rate_threshold_g = 0;
EXPORT_SYMBOL(rate_threshold_g);
u_int32_t rate_threshold_enable = 0;
EXPORT_SYMBOL(rate_threshold_enable);
u_int32_t rate_threshold_single = 0;
EXPORT_SYMBOL(rate_threshold_single);
u_int32_t rate_threshold_double = 0;
EXPORT_SYMBOL(rate_threshold_double);
/*End: Added by zhanghu for increasing the rate*/

/*zhaoyang add start for stop recv and send*/
u_int32_t stop_transmit = 0;
EXPORT_SYMBOL(stop_transmit);
/*zhaoyang add end*/
/*zhaoyang modify for txpower ctl*/
u_int32_t aid = 0;
EXPORT_SYMBOL(aid);
/*zhaoyang modify end*/

/*lyb add start*/
u_int32_t noise_sched = 0;
EXPORT_SYMBOL(noise_sched);
u_int32_t noiselevel = 0;
EXPORT_SYMBOL(noiselevel);
u_int32_t twice_noiselevel = 0;
EXPORT_SYMBOL(twice_noiselevel);
u_int32_t packet_rate = 0;
EXPORT_SYMBOL(packet_rate);
u_int32_t noiselevel_step = 0;
EXPORT_SYMBOL(noiselevel_step);

u_int32_t packet_number = 0;
EXPORT_SYMBOL(packet_number);
u_int32_t packet_num_stats = 0;
EXPORT_SYMBOL(packet_num_stats);
u_int32_t packet_last_rate = 0;
EXPORT_SYMBOL(packet_last_rate);
u_int32_t packet_rx_rate = 0;
EXPORT_SYMBOL(packet_rx_rate);
u_int32_t noiseFloorThresh = 1;
EXPORT_SYMBOL(noiseFloorThresh);
u_int32_t interfer_or_recv = 2;
EXPORT_SYMBOL(interfer_or_recv);
u_int32_t packet_num_for_time_step = 0;
EXPORT_SYMBOL(packet_num_for_time_step);
u_int32_t packet_num_stats_count = 0;
EXPORT_SYMBOL(packet_num_stats_count);
u_int32_t packet_num_for_time = 150;
EXPORT_SYMBOL(packet_num_for_time);
u_int32_t noiseImmunity = 0;
EXPORT_SYMBOL(noiseImmunity);
/*lyb add end*/

/*Begin:added by duanmingzhe for monitor process*/
int monitor_process = 0;
EXPORT_SYMBOL(monitor_process);
/*End:added by duanmingzhe for monitor process*/

/*Begin:added by duanmingzhe for handle fatal exception*/
int sysreboot_flag = 0;
EXPORT_SYMBOL(sysreboot_flag);
int thinap_state = 0;   /*0-wtpd not run, 1-wtpd run, 2-software upgrade*/
EXPORT_SYMBOL(thinap_state);
int thinap_check_timer = 0;
EXPORT_SYMBOL(thinap_check_timer);
/*End:added by duanmingzhe for handle fatal exception*/
/*Begin:Added by duanmingzhe for wtpd echo threshold*/
int thinap_check_threshold = 12;
EXPORT_SYMBOL(thinap_check_threshold);
/*End:Added by duanmingzhe for wtpd echo threshold*/

/* add rssi threshold for station association restriction - chenming*/
int rssi_threshold = 0;     // default to 0
EXPORT_SYMBOL(rssi_threshold);
/*suzhaoyu add start iwlist ath scanning modify PCAPVXN-87*/
int iwlist_scan_onebyone = 0;
EXPORT_SYMBOL(iwlist_scan_onebyone);
u_int16_t iwlist_scan_chan_num=0;
EXPORT_SYMBOL(iwlist_scan_chan_num);
u_int16_t iwlist_scan_current_chan_ng=0;
EXPORT_SYMBOL(iwlist_scan_current_chan_ng);
u_int16_t iwlist_scan_current_chan_na=0;
EXPORT_SYMBOL(iwlist_scan_current_chan_na);
/*suzhaoyu addend*/
/*Begin:Added by duanmingzhe for print txq info*/
u_int16_t txqprint=0;
EXPORT_SYMBOL(txqprint);
u_int16_t printdevid=0;
EXPORT_SYMBOL(printdevid);

/*End:Added by duanmingzhe for print txq info*/

/*Begin:Added by duanmingzhe for change data priority*/
u_int16_t data_priority=0;
EXPORT_SYMBOL(data_priority);
/*End:Added by duanmingzhe for change priority of data frame*/
int disable_probe_response = 0;     /*default to 0 added by chenming for disable probe response */
EXPORT_SYMBOL(disable_probe_response);
/*suzhaoyu add for suppressing illegal sta begin*/
int ath_sta_interfere = 0 ;
EXPORT_SYMBOL(ath_sta_interfere);
/*suzhaoyu end*/

/*zhanxuechao add for debug*/
u_int32_t debug_level = 3;
EXPORT_SYMBOL(debug_level);

static int
ATH_SYSCTL_DECL(ath_sysctl_halparam, ctl, write, filp, buffer, lenp, ppos)
{
    static u_int val;
    int ret=0;

    ctl->data = &val;
    ctl->maxlen = sizeof(val);
    if (write) 
    {
        ret = ATH_SYSCTL_PROC_DOINTVEC(ctl, write, filp, buffer,
                                       lenp, ppos);
        if(0 == ret)
        {
            switch ((long)ctl->extra2) 
            {
                case ATH_THINAP:
                    thinap = val;
                    break;
#ifdef AE5200_RT_SUPPORT
				/*zhaoyang add for priority join in 5G*/
				case ATH_JOIN5G_ENABLE:
					join5g_enable = val;
				break;
				case ATH_SCANTIME_THR:
					scantime_thr = val;
				break;
				case ATH_AGINGTIME_THR:
					 agingtime_thr = val;
					 break;
			      case ATH_DISCARD_COUNT:
				  	discard_count = val;
					break;
				case ATH_JOIN5G_DEBUG:
					join5g_debug = val;
					break;
				case ATH_STA_COUNT:
					stacount_thr = val;
					break;
                /*Begin, station's 2.4G signal strength threshold, wangjia 2012-10-16. */
                case ATH_STA_SIGNAL_STRENGTH_THR:
                    /*Begin, modified by wangjia, for parameter value validation check. 2012-12-28 */
                    {
                        int signal_strength = (int)val;
                        if(signal_strength >= -95 && signal_strength <= 0)
                        {
                            //sta_signal_strength_thr = signal_strength - ATH_DEFAULT_NOISE_FLOOR; /* Convert dBm to RSSI. */
                            sta_signal_strength_thr = signal_strength - (-95);
                        }
                    }
                    /*End, modified by wangjia, for parameter value validation check. 2012-12-28 */
                    break;
                /*End, wangjia 2012-10-16. */     
				/*End: add for 5G priority by caizhibang  2012-09-27> */
#endif                    
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
                /*add-chenming*/
                case ATH_THINAP_RSSI_THRESHOLD:
                    rssi_threshold = val;
                    break;
                /*end-chenming*/
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
                /*suzhaoyu add for suppressing illegal sta begin*/
                case ATH_STA_INTERFERCE:
                    ath_sta_interfere = val;
                    break;
                /*suzhaoyu add end */
                case ATH_STOP_TRANSMIT:
                    stop_transmit =val;
                    break;
                /*zhaoyang add end*/
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
                /*          
                case ATH_80211PRISM_ENABLE:
                    prism_enable = val;
                    break;
                case ATH_80211MGMT_ENABLE:
                    mgmt_enable = val;
                    break;
                */
                /*added by chenming for disable probe response */
                case DISABLE_PROBE_RESPONSE:  
                    disable_probe_response = val;
                    break;
                /*ended by chenming*/
                /*End:Added by duanmingzhe for 80211 cache*/
                /*Begin:Added by duanmingzhe for print txq info*/               
                case ATH_TXQPRINT:
                    txqprint = val;
                    break;
                case ATH_PRINTDEVID:
                    printdevid= val;
                    break;
                /*End:Added by duanmingzhe for print txq info*/             
                /*Begin:Added by duanmingzhe for change data priority*/
                case ATH_DATA_PRIORITY:
                    data_priority= val;
                    break;
                /*End:Added by duanmingzhe for change data priority*/
                /*Begin:Added by zhanghu for increasing rate*/
                case ATH_RATE_THRESHOLD_A:
                if(val == 0 || val == 6000 || val == 9000 \
                    ||val == 12000 || val == 18000 ||val == 24000 \
                    || val == 36000 ||val == 48000 || val == 54000 )
                    rate_threshold_a = val;
                    break;
                case ATH_RATE_THRESHOLD_B:
                if(val == 0 || val == 1000 || val == 2000 ||val == 5500 || val == 11000)
                    rate_threshold_b = val;
                    break;
                case ATH_RATE_THRESHOLD_G:
                if(val == 0 || val == 1000 || val == 2000 ||val == 5500  \
                    || val == 11000 ||val == 6000 || val == 9000 \
                    ||val == 12000 || val == 18000 ||val == 24000 \
                    || val == 36000 ||val == 48000 || val == 54000 )
                    rate_threshold_g = val;
                    break;
                case ATH_RATE_THRESHOLD_ENABLE:
                if(val == 0 || val == 1 )
                    rate_threshold_enable = val;
                    break;
                case ATH_RATE_THRESHOLD_SINGLE:
                    if(0 <= val && val < 8)
                        rate_threshold_single = val;
                    break;
                case ATH_RATE_THRESHOLD_DOUBLE:
                    if(0 <= val && val < 16)
                        rate_threshold_double = val;
                    break;
                /*End:Added by zhanghu for increasing rate*/
                /*lyb add start*/
                case ATH_TIMER_SCHED:
                    noise_sched =val;
                    break;
                case ATH_NUM_PER_TIMER:
                    noiselevel =val;
                    break;
                case ATH_TWICE_NUM_PER_TIMER:
                    twice_noiselevel =val;
                    break;
                case ATH_PACKET_RATE:
                    packet_rate =val;
                    break;
                case ATH_SIGNAL_DECISION:
                    noiseFloorThresh =val;
                    break;
                case ATH_NUM_PER_TIMER_STEP:
                    noiselevel_step =val;
                    break;  
                case ATH_PACKET_NUM_FOR_TIME_STEP:
                    packet_num_for_time_step =val;
                    break;
                case ATH_PACKET_NUM_FOR_TIME:
                    packet_num_for_time =val;
                    break;
                case ATH_INTERFER_OR_RECV:
                    interfer_or_recv=val;
                    break;
                case ATH_FORCE_MANUAL:
                    noiseImmunity=val;
                    if(noiseImmunity)
                    {
                        noiseFloorThresh = 1;
                        noise_sched = 1;
                        noiselevel = 8;
                        packet_rate = 0;
                    }
                    break;
                /*lyb add end*/
                /*zhaoyang modify for send arp active when wireless to wireless*/
                case ATH_ARP_SEND_ACTIVE:
                    arp_send_active=val;
                    break;
                /*zhaoyang modify end*/
                /*zhaoyang modify for tunnel data frame fast forward*/
                case ATH_TUNNEL_FAST_FORWARD:
                    tunnel_fast_forward=val;
                    break;
                /*zhaoyang modify end*/

                /*zhanxuechao add for debug*/
                case ATH_DEBUG_LEVEL:
                    debug_level=val;
                    break;
                    
                default:
                    return -EINVAL;
            }
        }
    }
    else
    {
        switch ((long)ctl->extra2) 
        {
            case ATH_THINAP:
                val = thinap;
                break;
#ifdef AE5200_RT_SUPPORT
			/*zhaoyang add for priority join in 5G*/
			case ATH_JOIN5G_ENABLE:
				val = join5g_enable;
			break;
			case ATH_SCANTIME_THR:
				val = scantime_thr;
			break;
			case ATH_AGINGTIME_THR:
				val = agingtime_thr;
				break;
			case ATH_DISCARD_COUNT:
				val = discard_count;
				break;
			case ATH_JOIN5G_DEBUG:
				val = join5g_debug;
			      break;
			case ATH_STA_COUNT:
				val = stacount_thr;
				break;
            /*Begin, station's 2.4G signal strength threshold, wangjia 2012-10-16. */
            case ATH_STA_SIGNAL_STRENGTH_THR:
                //val = sta_signal_strength_thr + ATH_DEFAULT_NOISE_FLOOR; /* Convert RSSI to dBm. */;
                val = sta_signal_strength_thr + (-95); /* Convert RSSI to dBm. */;
                break;
            /*End, wangjia 2012-10-16. */
			/*zhaoyang add end*/
			/*End : add for 5G priority by caizhibang  2012-09-27> */
#endif
                
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
            /*add-chenming*/
            case ATH_THINAP_RSSI_THRESHOLD:
                val = rssi_threshold;
                break;
            /*end-chenming*/
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
            /*suzhaoyu add for suppressing illegal sta begin*/
            case ATH_STA_INTERFERCE:
                val = ath_sta_interfere;
                break;
            /*suzhaoyu add end */
            case ATH_STOP_TRANSMIT:
                val = stop_transmit ;
                break;
            /*zhaoyang add end*/            
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
            /*  
            case ATH_80211PRISM_ENABLE:
                val = prism_enable;
                break;
            case ATH_80211MGMT_ENABLE:
                val = mgmt_enable;
                break;
            */
            /*End:Added by duanmingzhe for 80211 cache*/

            /*Begin:Added by duanmingzhe for print txq info*/               
            case ATH_TXQPRINT:
                val = txqprint;
                break;
            case ATH_PRINTDEVID:
                val = printdevid;
                break;
            /*End:Added by duanmingzhe for print txq info*/

            /*Begin:Added by duanmingzhe for change data priority*/
            case ATH_DATA_PRIORITY:
                val = data_priority;
                break;
            /*End:Added by duanmingzhe for change data priority*/
            case DISABLE_PROBE_RESPONSE:  /*added by chenming for disable probe response */
                val = disable_probe_response ;
                break;

            /*Begin:Added by zhanghu for increasing rate*/
            case ATH_RATE_THRESHOLD_A:
                val = rate_threshold_a ;
                break;
            case ATH_RATE_THRESHOLD_B:
                val = rate_threshold_b ;
                break;
            case ATH_RATE_THRESHOLD_G:
                val = rate_threshold_g ;
                break;
            case ATH_RATE_THRESHOLD_ENABLE:
                val = rate_threshold_enable;
                break;
            case ATH_RATE_THRESHOLD_SINGLE:
                val = rate_threshold_single;
                break;
            case ATH_RATE_THRESHOLD_DOUBLE:
                val = rate_threshold_double;
                break;
            /*End:Added by zhanghu for increasing rate*/
            /*lyb add start*/
            case ATH_TIMER_SCHED:
                val = noise_sched ;
                break;
            case ATH_TWICE_NUM_PER_TIMER:
                val = twice_noiselevel;
                break;
            case ATH_NUM_PER_TIMER:
                val = noiselevel ;
                break;
            case ATH_PACKET_RATE:
                val = packet_rate ;
                break;
            case ATH_SIGNAL_DECISION:
                val = noiseFloorThresh ;
                break;
            case ATH_NUM_PER_TIMER_STEP:
                val = noiselevel_step ;
                break;
            case ATH_PACKET_NUM_FOR_TIME:
                val = packet_num_for_time ;
                break;
            case ATH_PACKET_NUM_FOR_TIME_STEP:
                val = packet_num_for_time_step ;
                break;
            case ATH_INTERFER_OR_RECV:
                val = interfer_or_recv;
                break;
            case ATH_FORCE_MANUAL:
                val = noiseImmunity;
                break;
            /*lyb add end*/
                /*zhaoyang modify for send arp active when wireless to wireless*/
            case ATH_ARP_SEND_ACTIVE:
                val = arp_send_active;
                break;
            /*zhaoyang modify end*/
            /*zhaoyang modify for tunnel data frame fast forward*/
            case ATH_TUNNEL_FAST_FORWARD:
                val = tunnel_fast_forward;
                break;
            /*zhaoyang modify end*/

            /*zhanxuechao add for debug*/
            case ATH_DEBUG_LEVEL:
                val = debug_level;
                break;
                
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
    {
        .procname     = "thinap",
        .mode         = 0644,
        .proc_handler = ath_sysctl_halparam,
        .extra2       = (void *)ATH_THINAP,
    },
#ifdef AE5200_RT_SUPPORT
	/*zhaoyang add for priority join in 5G*/
	{
      .procname         = "join5g_enable",
      .mode             = 0644,
      .proc_handler     = ath_sysctl_halparam,
   	  .extra2           = (void *)ATH_JOIN5G_ENABLE,  
    },
    {
      .procname         = "scantime_thr",
      .mode             = 0644,
      .proc_handler     = ath_sysctl_halparam,
   	  .extra2           = (void *)ATH_SCANTIME_THR,  
    },
    {
      .procname         = "agingtime_thr",
      .mode             = 0644,
      .proc_handler     = ath_sysctl_halparam,
   	  .extra2           = (void *)ATH_AGINGTIME_THR,  
    },
    {
      .procname         = "discard_count",
      .mode             = 0644,
      .proc_handler     = ath_sysctl_halparam,
   	  .extra2           = (void *)ATH_DISCARD_COUNT,  
    },
    {
      .procname         = "join5g_debug",
      .mode             = 0644,
      .proc_handler     = ath_sysctl_halparam,
   	  .extra2           = (void *)ATH_JOIN5G_DEBUG,  
    },
    {
      .procname         = "stacount_thr",
      .mode             = 0644,
      .proc_handler     = ath_sysctl_halparam,
   	  .extra2           = (void *)ATH_STA_COUNT,  
    },
    /*Begin, station's 2.4G signal strength threshold, wangjia 2012-10-16. */
    {
      .procname         = "sta_signal_strength_thr",
      .mode             = 0644, 
      .proc_handler     = ath_sysctl_halparam,
   	  .extra2           = (void *)ATH_STA_SIGNAL_STRENGTH_THR,  
    },
    /*End, wangjia 2012-10-16. */
/*End : add for 5G priority by caizhibang  2012-09-27> */	

#endif
    {
        .procname     = "traffic_balance",
        .mode         = 0644,
        .proc_handler = ath_sysctl_halparam,
        .extra2       = (void *)ATH_TRAFFIC_BALANCE,
    },
    {
        .procname     = "thinap_state",
        .mode         = 0644,
        .proc_handler = ath_sysctl_halparam,
        .extra2       = (void *)ATH_THINAP_STATE,
    },
    {
        .procname     = "monitor_process",
        .mode         = 0644,
        .proc_handler = ath_sysctl_halparam,
        .extra2       = (void *)ATH_MONITOR_PROCESS,
    },
    {
        .procname     = "sysreboot_flag",
        .mode         = 0644,
        .proc_handler = ath_sysctl_halparam,
        .extra2       = (void *)ATH_SYSREBOOT_FLAG,
    },
    {
        .procname     = "thinap_check_timer",
        .mode         = 0644,
        .proc_handler = ath_sysctl_halparam,
        .extra2       = (void *)ATH_THINAP_CHECK_TIMER,
    },
    {
        .procname     = "thinap_check_threshold",
        .mode         = 0644,
        .proc_handler = ath_sysctl_halparam,
        .extra2 = (void *)ATH_THINAP_CHECK_THRESHOLD,
    },
    /*add-chenming*/
    {
        .procname   = "rssi_threshold",
        .mode       = 0644,
        .proc_handler   = ath_sysctl_halparam,
        .extra2     = (void*)ATH_THINAP_RSSI_THRESHOLD,
    },
    /*end-chenming*/
    /*suzhaoyu add start iwlist ath scanning modify PCAPVXN-87*/
    {
        .procname     = "iwlist_scan_onebyone",
        .mode         = 0644,
        .proc_handler = ath_sysctl_halparam,
        .extra2       = (void *)ATH_IWLIST_SCAN_ONEBYONE,
    },
    {
        .procname     = "iwlist_scan_current_chan_ng",
        .mode         = 0644,
        .proc_handler = ath_sysctl_halparam,
        .extra2       = (void *)ATH_IWLIST_SCAN_CURRENT_CHAN_INDEX_NG,
    },
    {
        .procname     = "iwlist_scan_current_chan_na",
        .mode         = 0644,
        .proc_handler = ath_sysctl_halparam,
        .extra2       = (void *)ATH_IWLIST_SCAN_CURRENT_CHAN_INDEX_NA,
    },
    /*suzhaoyu add end*/
    /*zhaoyang add start for stop recv and send*/
    {
        .procname         = "stop_transmit",
        .mode             = 0644,
        .proc_handler     = ath_sysctl_halparam,
        .extra2           = (void *)ATH_STOP_TRANSMIT,  
    },
    /*zhaoyang add end*/
    /*Begin:Added by duanmingzhe for 80211 cache*/
    {
        .procname       = "cache_enable",
        .mode           = 0644,
        .proc_handler   = ath_sysctl_halparam,
        .extra2         = (void *)ATH_80211CACHE,  
    },  
    {
        .procname       = "cache_factor_wifi0",
        .mode           = 0644,
        .proc_handler   = ath_sysctl_halparam,
        .extra2         = (void *)ATH_80211CACHE_FACTOR_WIFI0,  
    },  
    {
        .procname       = "cache_factor_b_wifi0",
        .mode           = 0644,
        .proc_handler   = ath_sysctl_halparam,
        .extra2         = (void *)ATH_80211CACHE_FACTOR_B_WIFI0,  
    },
    {
        .procname       = "cache_factor_g_wifi0",
        .mode           = 0644,
        .proc_handler   = ath_sysctl_halparam,
        .extra2         = (void *)ATH_80211CACHE_FACTOR_G_WIFI0,  
    },
    {
        .procname       = "cache_factor_n_wifi0",
        .mode           = 0644,
        .proc_handler   = ath_sysctl_halparam,
        .extra2         = (void *)ATH_80211CACHE_FACTOR_N_WIFI0,  
    },
    {
        .procname       = "cache_factor_wifi1",
        .mode           = 0644,
        .proc_handler   = ath_sysctl_halparam,
        .extra2         = (void *)ATH_80211CACHE_FACTOR_WIFI1,  
    },  
    {
        .procname       = "cache_factor_b_wifi1",
        .mode           = 0644,
        .proc_handler   = ath_sysctl_halparam,
        .extra2         = (void *)ATH_80211CACHE_FACTOR_B_WIFI1,  
    },
    {
        .procname       = "cache_factor_g_wifi1",
        .mode           = 0644,
        .proc_handler   = ath_sysctl_halparam,
        .extra2         = (void *)ATH_80211CACHE_FACTOR_G_WIFI1,  
    },
    {
        .procname       = "cache_factor_n_wifi1",
        .mode           = 0644,
        .proc_handler   = ath_sysctl_halparam,
        .extra2         = (void *)ATH_80211CACHE_FACTOR_N_WIFI1,  
    },
    {
        .procname       = "cache_pass_factor",
        .mode           = 0644,
        .proc_handler   = ath_sysctl_halparam,
        .extra2         = (void *)ATH_80211CACHE_PASS_FACTOR,  
    },
    {
        .procname       = "cache_min_size",
        .mode           = 0644,
        .proc_handler   = ath_sysctl_halparam,
        .extra2         = (void *)ATH_80211CACHE_MIN_SIZE,  
    },
    {
        .procname       = "cache_max_size",
        .mode           = 0644,
        .proc_handler   = ath_sysctl_halparam,
        .extra2         = (void *)ATH_80211CACHE_MAX_SIZE,  
    },
    {
        .procname       = "cache_debug",
        .mode           = 0644,
        .proc_handler   = ath_sysctl_halparam,
        .extra2         = (void *)ATH_80211CACHE_DEBUG,  
    },
    {
        .procname       = "cache_timer",
        .mode           = 0644,
        .proc_handler   = ath_sysctl_halparam,
        .extra2         = (void *)ATH_80211CACHE_TIMER,  
    },
    /*  
    { 
        .procname       = "prism_enable",
        .mode           = 0644,
        .proc_handler   = ath_sysctl_halparam,
        .extra2         = (void *)ATH_80211PRISM_ENABLE,  
    },
    {
        .procname       = "mgmt_enable",
        .mode           = 0644,
        .proc_handler   = ath_sysctl_halparam,
        .extra2         = (void *)ATH_80211MGMT_ENABLE,  
    },
    */
    /*End:Added by duanmingzhe for 80211 cache*/
    /*added by chenming for disable probe response */
    {
        .procname   = "disable_probe_response",
        .mode       = 0644,
        .proc_handler   = ath_sysctl_halparam,
        .extra2     = (void*)DISABLE_PROBE_RESPONSE,
    },
    /*ended by chenming*/

    /*Begin: added by zhanghu for increasing for rate*/
    {
        .procname     = "rate_threshold_a",
        .mode         = 0644,
        .proc_handler = ath_sysctl_halparam,
        .extra2   = (void *)ATH_RATE_THRESHOLD_A,
    },
    {
        .procname     = "rate_threshold_b",
        .mode         = 0644,
        .proc_handler = ath_sysctl_halparam,
        .extra2   = (void *)ATH_RATE_THRESHOLD_B,
    },
    {
        .procname     = "rate_threshold_g",
        .mode         = 0644,
        .proc_handler = ath_sysctl_halparam,
        .extra2   = (void *)ATH_RATE_THRESHOLD_G,
    },
    {
        .procname     = "rate_threshold_enable",
        .mode         = 0644,
        .proc_handler = ath_sysctl_halparam,
        .extra2   = (void *)ATH_RATE_THRESHOLD_ENABLE,
    },
    {
        .procname     = "rate_threshold_single",
        .mode         = 0644,
        .proc_handler = ath_sysctl_halparam,
        .extra2   = (void *)ATH_RATE_THRESHOLD_SINGLE,
    },
    {
        .procname     = "rate_threshold_double",
        .mode         = 0644,
        .proc_handler = ath_sysctl_halparam,
        .extra2   = (void *)ATH_RATE_THRESHOLD_DOUBLE,
    },
    /*End: added by zhanghu for increasing for rate*/
    /*Begin:Added by duanmingzhe for print txq info*/
    {
        .procname   = "txqprint",
        .mode       = 0644,
        .proc_handler = ath_sysctl_halparam,
        .extra2 = (void *)ATH_TXQPRINT,
    },      
    {
        .procname   = "printdevid",
        .mode       = 0644,
        .proc_handler = ath_sysctl_halparam,
        .extra2 = (void *)ATH_PRINTDEVID,
    },  
    /*End:Added by duanmingzhe for print txq info*/
    /*Begin:Added by duanmingzhe for change data priority*/
    {
        .procname   = "data_priority",
        .mode       = 0644,
        .proc_handler = ath_sysctl_halparam,
        .extra2 = (void *)ATH_DATA_PRIORITY,
    },  
    /*End:Added by duanmingzhe for change data priority*/
    /*suzhaoyu add for suppressing illegal sta begin*/
    {
        .procname     = "ath_sta_interfere",
        .mode         = 0644,
        .proc_handler = ath_sysctl_halparam,
        .extra2       = (void *)ATH_STA_INTERFERCE,
    },
    /*suzhaoyu add end */
    /*zhaoyang modify for send arp active when wireless to wireless*/
    {
        .procname         = "arp_send_active",
        .mode             = 0644,
        .proc_handler     = ath_sysctl_halparam,
        .extra2           = (void *)ATH_ARP_SEND_ACTIVE,  
    },
    /*zhaoyang modify end*/
    /*zhaoyang modify for tunnel data frame fast forward*/
    {
        .procname         = "tunnel_fast_forward",
        .mode             = 0644,
        .proc_handler     = ath_sysctl_halparam,
        .extra2           = (void *)ATH_TUNNEL_FAST_FORWARD,  
    },
    /*zhaoyang modify end*/
    /*lyb add start*/
    {
        .procname         = "noise_sched",
        .mode             = 0644,
        .proc_handler     = ath_sysctl_halparam,
        .extra2           = (void *)ATH_TIMER_SCHED,  
    },
    {
        .procname         = "noiselevel",
        .mode             = 0644,
        .proc_handler     = ath_sysctl_halparam,
        .extra2           = (void *)ATH_NUM_PER_TIMER,  
    },
    {
        .procname         = "twice_noiselevel",
        .mode             = 0644,
        .proc_handler     = ath_sysctl_halparam,
        .extra2           = (void *)ATH_TWICE_NUM_PER_TIMER,  
    },
    {
        .procname         = "packet_rate",
        .mode             = 0644,
        .proc_handler     = ath_sysctl_halparam,
        .extra2           = (void *)ATH_PACKET_RATE,  
    },
    {
        .procname         = "noiseFloorThresh",
        .mode             = 0644,
        .proc_handler     = ath_sysctl_halparam,
        .extra2           = (void *)ATH_SIGNAL_DECISION,  
    },
    {
        .procname         = "noiselevel_step",
        .mode             = 0644,
        .proc_handler     = ath_sysctl_halparam,
        .extra2           = (void *)ATH_NUM_PER_TIMER_STEP,  
    },
    {
        .procname         = "packet_num_for_time",
        .mode             = 0644,
        .proc_handler     = ath_sysctl_halparam,
        .extra2           = (void *)ATH_PACKET_NUM_FOR_TIME,  
    },
    {
        .procname         = "interfer_or_recv",
        .mode             = 0644,
        .proc_handler     = ath_sysctl_halparam,
        .extra2           = (void *)ATH_INTERFER_OR_RECV,  
    },
    {
        .procname         = "packet_num_for_time_step",
        .mode             = 0644,
        .proc_handler     = ath_sysctl_halparam,
        .extra2           = (void *)ATH_PACKET_NUM_FOR_TIME_STEP,  
    },
    {
        .procname         = "noiseImmunity",
        .mode             = 0644,
        .proc_handler     = ath_sysctl_halparam,
        .extra2           = (void *)ATH_FORCE_MANUAL,  
    },
    /*lyb add end*/
    /* may add more in the future */

    /*zhanxuechao add for debug*/
    {
        .procname         = "debug_level",
        .mode             = 0644,
        .proc_handler     = ath_sysctl_halparam,
        .extra2           = (void *)ATH_DEBUG_LEVEL,  
    },
    
    { 0 }
};

void ap8x_dynamic_sysctl_register_thinap(struct wlprivate *wlp)
{
    int i, space;
    char *devname = wlp->netDev->name ;

    space = 5*sizeof(struct ctl_table) + sizeof(ath_sysctl_template);
    wlp->sc_sysctls = kmalloc(space, GFP_KERNEL);
    if (wlp->sc_sysctls == NULL) 
    {
        printk("%s: no memory for sysctl table!\n", __func__);
        return;
    }

    /* setup the table */
    memset(wlp->sc_sysctls, 0, space);
    wlp->sc_sysctls[0].procname = "dev";
    wlp->sc_sysctls[0].mode = 0555;
    wlp->sc_sysctls[0].child = &wlp->sc_sysctls[2];
    /* [1] is NULL terminator */
    wlp->sc_sysctls[2].procname = devname ;
    wlp->sc_sysctls[2].mode = 0555;
    wlp->sc_sysctls[2].child = &wlp->sc_sysctls[4];
    /* [3] is NULL terminator */
    /* copy in pre-defined data */
    memcpy(&wlp->sc_sysctls[4], ath_sysctl_template,
            sizeof(ath_sysctl_template));

    /* add in dynamic data references */
    for (i = 4; wlp->sc_sysctls[i].procname; i++)
        if (wlp->sc_sysctls[i].extra1 == NULL)
            wlp->sc_sysctls[i].extra1 = wlp;

    /* and register everything */
#if LINUX_VERSION_CODE > KERNEL_VERSION(2,6,20)
    wlp->sc_sysctl_header = register_sysctl_table(wlp->sc_sysctls);
#else
    wlp->sc_sysctl_header = register_sysctl_table(wlp->sc_sysctls, 1);
#endif
    if (!wlp->sc_sysctl_header) 
    {
        printk("%s: failed to register sysctls!\n", devname);
        kfree(wlp->sc_sysctls);
        wlp->sc_sysctls = NULL;
    }
}

void ap8x_dynamic_sysctl_unregister_thinap(struct wlprivate *wlp)
{
    if (wlp->sc_sysctl_header) 
    {
        unregister_sysctl_table(wlp->sc_sysctl_header);
        wlp->sc_sysctl_header = NULL;
    }
    if (wlp->sc_sysctls) 
    {
        kfree(wlp->sc_sysctls);
        wlp->sc_sysctls = NULL;
    }

    return;
}

EXPORT_SYMBOL(ap8x_dynamic_sysctl_register_thinap);
EXPORT_SYMBOL(ap8x_dynamic_sysctl_unregister_thinap);

