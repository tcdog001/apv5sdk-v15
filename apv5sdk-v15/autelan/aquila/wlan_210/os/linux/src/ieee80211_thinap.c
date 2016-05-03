/* ****************************************************************************************************
 * Filename: ieee80211_thinap.c
 * Description: a module for thin ap.
 * Project: Autelan ap 2011
 * Author: Mingzhe Duan
 * Date: 2011-03-24
 ********************************************************************************************************/
#include <linux/version.h>
#include <linux/module.h>
#include <linux/kmod.h>
#include <linux/init.h>
/*Begin:Added by duanmingzhe for thinanp*/
u_int32_t thinap = 1;	/*thin ap default*/
EXPORT_SYMBOL(thinap);
/*End:Added by duanmingzhe for thinanp*/
/*autelan - hxh: add for icmp trace print*/
u_int32_t dbg_print = 0;
EXPORT_SYMBOL(dbg_print);
u_int32_t ping_test_len = 73;
EXPORT_SYMBOL(ping_test_len);

u_int32_t ping_timeout_threshod = 500;
EXPORT_SYMBOL(ping_timeout_threshod);

typedef struct {
    struct ieee80211_node *ni;
    u_int32_t seq;
    u_int32_t flag;    
    u_int32_t timeout;
    struct timeval time_send;
	
} aute_ping_test_t;

aute_ping_test_t  ping_trace_test[16];
EXPORT_SYMBOL(ping_trace_test);

/*autelan - hxh: add for icmp trace print -end*/
/*peiwh add start for tid trace*/
u_int32_t      	 tid_period_sec;
EXPORT_SYMBOL(tid_period_sec);
u_int32_t      	 tid_period_us;
EXPORT_SYMBOL(tid_period_us);
/*peiwh add end for tid trace*/
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

/* Begin: Added by wangia, for traffic limit tasklet shedule. 2012-11-02 */
u_int64_t ath_rx_tasklet_tick = 0;
EXPORT_SYMBOL(ath_rx_tasklet_tick);
u_int64_t ath_rx_tasklet_time_slice = 0;
EXPORT_SYMBOL(ath_rx_tasklet_time_slice);
u_int64_t ath_tx_tasklet_tick = 0;
EXPORT_SYMBOL(ath_tx_tasklet_tick);
u_int64_t ath_tx_tasklet_time_slice = 0;
EXPORT_SYMBOL(ath_tx_tasklet_time_slice);
/* Begin: Added by wangia, for traffic limit tasklet shedule. 2012-11-02 */

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
//struct tasklet_struct cache_tx_tasklet_timer;		 /* tx foward tasklet */
//EXPORT_SYMBOL(cache_tx_tasklet_timer);
//struct tasklet_struct cache_tx_tasklet_enqueue;		 /* tx foward tasklet */
//EXPORT_SYMBOL(cache_tx_tasklet_enqueue);
/*End:Added by duanmingzhe for 80211 cache*/
/*Begin:Added by duanmingzhe for traffic balance*/
u_int32_t traffic_balance = 0;
EXPORT_SYMBOL(traffic_balance);
/*End:Added by duanmingzhe for traffic balance*/

/*<Begin : add for 5G priority by caizhibang  2012-09-27 */
/*zhaoyang add for priority join in 5G*/
#define IEEE80211_ADDR_LEN 6
#define IEEE80211_USERINFO_MAX 64   /* wangjia 2012-10-16 */
//#define IEEE80211_VAP_TABLE_MAX 16
#define IEEE80211_TABLE_USED 0x80 //10000000
#define IEEE80211_TABLE_SUPPORT2G 0x40 //01000000
#define IEEE80211_TABLE_SUPPORT5G 0x20 //00100000

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

/*Begin: added by wangjia for 802.11 tunnel QoS correction. 2012-12-21. */
u_int32_t tunnel_qos_correction = 1;
EXPORT_SYMBOL(tunnel_qos_correction);
/*End: added by wangjia for 802.11 tunnel QoS correction. 2012-12-21. */
/*<Begin:caizhibang add for channel utility*/
u_int32_t  channel_utility_switch = 0;
u_int32_t  channel_utility_5G = 0;
u_int32_t  channel_utility_2G = 0;
u_int32_t channel_utility_timer = 10;
EXPORT_SYMBOL(channel_utility_switch);
EXPORT_SYMBOL(channel_utility_5G);
EXPORT_SYMBOL(channel_utility_2G);
EXPORT_SYMBOL(channel_utility_timer);
/*End:caizhibang add for channel utility>*/

struct  userinfo_table
{
	u_int8_t	ni_mac[IEEE80211_ADDR_LEN];
   	u_int8_t  	ni_flags;
    unsigned int stamp_time;
    u_int8_t    count;
	u_int8_t    marked;  
    spinlock_t  userinfo_lock; /* Add irq lock. wangjia 2012-10-11 */
};

struct userinfo_table userinfo_table_t[IEEE80211_USERINFO_MAX];
EXPORT_SYMBOL(userinfo_table_t);
/*zhaoyang add end*/
/*End : add for 5G priority by caizhibang  2012-09-27> */

/*Begin:Added by zhanghu*/
u_int32_t rate_threshold_debug = 0;
EXPORT_SYMBOL(rate_threshold_debug);
/*End:Added by zhanghu*/


/*zhaoyang add start for stop recv and send*/
u_int32_t stop_transmit = 0;
EXPORT_SYMBOL(stop_transmit);
/*zhaoyang add end*/
/*yanggs transpant for cts*/
/*lyb add start*/
u_int32_t noise_sched = 0;
EXPORT_SYMBOL(noise_sched);
/*lyb add end*/
/*yanggs transpant end*/
/*yanggs transplant for cts*/
u_int32_t cts_duration = 3000;
EXPORT_SYMBOL(cts_duration);
/*yanggs transplant end*/
/*AUTELAN-Begin:zhaoenjuan added for drv monitor enable*/
u_int32_t autelan_drv_monitor_enable = 1;
EXPORT_SYMBOL(autelan_drv_monitor_enable);
/*AUTELAN-End:zhaoenjuan added for drv monitor enable*/

/*AUTELAN-Added-Begin:Added by duanmingzhe for EIFS & REG(9e24)*/
u_int32_t set_eifs = 0;
EXPORT_SYMBOL(set_eifs);
u_int32_t set_reg9e24 = 0;
EXPORT_SYMBOL(set_reg9e24);
/*AUTELAN-Added-End:Added by duanmingzhe for EIFS & REG(9e24)*/



/*Begin:added by duanmingzhe for monitor process*/
int monitor_process = 0;
EXPORT_SYMBOL(monitor_process);
/*End:added by duanmingzhe for monitor process*/

/*Begin:added by duanmingzhe for handle fatal exception*/
int sysreboot_flag = 0;
EXPORT_SYMBOL(sysreboot_flag);
int thinap_state = 0;	/*0-wtpd not run, 1-wtpd run, 2-software upgrade*/
EXPORT_SYMBOL(thinap_state);
int thinap_check_timer = 0;
EXPORT_SYMBOL(thinap_check_timer);
/*End:added by duanmingzhe for handle fatal exception*/
/*Begin:Added by duanmingzhe for wtpd echo threshold*/
int thinap_check_threshold = 12;
EXPORT_SYMBOL(thinap_check_threshold);
/*End:Added by duanmingzhe for wtpd echo threshold*/

/*suzhaoyu add start iwlist ath scanning modify PCAPVXN-87*/
int iwlist_scan_onebyone = 0;
EXPORT_SYMBOL(iwlist_scan_onebyone);
u_int16_t iwlist_scan_chan_num=0;
EXPORT_SYMBOL(iwlist_scan_chan_num);
u_int16_t iwlist_scan_current_chan_ng=255;
EXPORT_SYMBOL(iwlist_scan_current_chan_ng);
u_int16_t iwlist_scan_current_chan_na=255;
EXPORT_SYMBOL(iwlist_scan_current_chan_na);
/*suzhaoyu addend*/
/*Begin:Added by duanmingzhe for print txq info*/
u_int16_t txqprint=0;
EXPORT_SYMBOL(txqprint);
u_int16_t printdevid=0;
EXPORT_SYMBOL(printdevid);

/*End:Added by duanmingzhe for print txq info*/
/*suzhaoyu add for sta leave report*/
#if AUTELAN_SOLUTION2
bool ioctl_del_sta = 1;
EXPORT_SYMBOL(ioctl_del_sta);
bool pass_lvframe = 0;
EXPORT_SYMBOL(pass_lvframe);
bool report_detail = 0;
EXPORT_SYMBOL(report_detail);
#endif
/*suzhaoyu add end*/
/*AUTELAN-Added-Begin:duanmingzhe for when check too many bmiss reboot the device */
u_int32_t bmiss_check = 60;
EXPORT_SYMBOL(bmiss_check);
/*AUTELAN-Added-End:duanmingzhe for when check too many bmiss reboot the device */

//AUTELAN--drv monitor proc varible
int bstuck_enable = 1;
EXPORT_SYMBOL(bstuck_enable);

//AUTELAN:zhaoenjuan added  for bb hang & mac hang check
int aute_bb_hang_check = 2;
EXPORT_SYMBOL(aute_bb_hang_check);
int aute_mac_hang_check = 2;
EXPORT_SYMBOL(aute_mac_hang_check);


/*
0 wait to start
1 start test
2 disable the monitor
*/
u_int32_t aute_latency_idle_enable = 0;//After cpe online, wait 120 sec, then start the test, lut add
EXPORT_SYMBOL(aute_latency_idle_enable);
u_int32_t aute_air_bad_threshhod = 30;
EXPORT_SYMBOL(aute_air_bad_threshhod);

u_int32_t aute_air_bad_num = 0;
EXPORT_SYMBOL(aute_air_bad_num);
u_int32_t autelan_drv_monitor_reboot = 1;
EXPORT_SYMBOL(autelan_drv_monitor_reboot);

u_int32_t aute_air_drop_num = 0;
EXPORT_SYMBOL(aute_air_drop_num);

u_int32_t rx_hp_cnt = 0;
EXPORT_SYMBOL(rx_hp_cnt);
u_int32_t rx_lp_cnt = 0;
EXPORT_SYMBOL(rx_lp_cnt);
//AUTELAN--drv monitor proc varible
