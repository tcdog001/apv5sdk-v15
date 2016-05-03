/* ****************************************************************************************************
 * Filename: ieee80211_autelan_limit.c
 * Description: a module for traffic limit.
 * Project: Autelan ap 2012
 * Author: Mingzhe Duan
 * Date: 2012-05-17
 ********************************************************************************************************/

#ifndef _IEEE80211_AUTELAN_LIMIT_H_
#define _IEEE80211_AUTELAN_LIMIT_H_

#include <linux/ip.h>
#include <linux/timer.h>
#include <ap8xLnxIntf.h>
#include <IEEE_types.h>
#include <wl_hal.h>
#include <StaDb.h>
#include "ap8xLnxThinap.h"

#define LIMIT_SUCCESS 1
#define LIMIT_FAIL 0
#define RX_PKT 0
#define TX_PKT 1

#define CAR_SCALE_FACTOR_BIT_SHIFT	8
#define CAR_SCALE_FACTOR			(1 << CAR_SCALE_FACTOR_BIT_SHIFT) /**/

void autelan_traffic_limit_init(struct autelan_traffic_limit * tl);
void autelan_traffic_limit_vaptoni(struct autelan_traffic_limit * iv_tl,struct autelan_traffic_limit * ni_tl);
u_int32_t autelan_trafficlimit(struct autelan_traffic_limit * tl,u_int32_t wbuf_len,unsigned char flag);
int ieee80211_autelan_traffic_limit_tx(struct net_device *netdev, struct sk_buff *skb,extStaDb_StaInfo_t *staInfo);
int ieee80211_autelan_traffic_limit_rx(struct net_device *netdev, struct sk_buff *skb, extStaDb_StaInfo_t *pStaInfo);


#endif

