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
#ifndef AP8X_5G_PRIORITY_H_
#define AP8X_5G_PRIORITY_H_

#include "ap8xLnxIntf.h"
#include "IEEE_types.h"
//#include "Wl_hal.h"

/*zhaoyang add for priority join in 5G*/

#define IEEE80211_USERINFO_MAX 64   /* wangjia 2012-10-16 */
#define IEEE80211_TABLE_USED 0x80 //10000000
#define IEEE80211_TABLE_SUPPORT2G 0x40 //01000000
#define IEEE80211_TABLE_SUPPORT5G 0x20 //00100000

//#define IEEE80211_ADDR_LEN                  6
#define IEEE80211_NWID_LEN                  32

struct  userinfo_table
{
	u_int8_t	ni_mac[IEEE80211_ADDR_LEN];     // sta mac
   	u_int8_t  	ni_flags;                       // 
    unsigned long stamp_time;                    // probe request time
    u_int8_t    count;                          // probe requeset count
	u_int8_t    marked;                         // 
    spinlock_t  userinfo_lock;                  //* Add irq lock. wangjia 2012-10-11 */
};

/*zhaoyang add end*/

/*<Begin : add for 5G priority by caizhibang  2012-09-27 */
/*zhaoyang add for priority join in 5G*/
extern u_int32_t join5g_enable;
extern u_int32_t scantime_thr;
extern u_int32_t agingtime_thr;
extern u_int32_t discard_count;
extern u_int32_t join5g_debug;
extern u_int32_t stacount_thr;
extern u_int32_t sta_signal_strength_thr; /* station's 2.4G signal strength threshold, wangjia 2012-10-16. */


int ieee80211_autelan_5G_priority_enable(vmacApInfo_t *vmacSta_p,UINT8 *sta_mac,UINT32 RSSI,UINT8 subtype);


#endif

