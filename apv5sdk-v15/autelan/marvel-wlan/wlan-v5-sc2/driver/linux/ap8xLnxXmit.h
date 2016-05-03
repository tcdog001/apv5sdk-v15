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
#ifndef AP8X_XMIT_H_
#define AP8X_XMIT_H_

#include <linux/version.h>
#include <linux/module.h>
#include <linux/interrupt.h>
#include <linux/pci.h>
#include <linux/spinlock.h>
#include <linux/netdevice.h>
#include <linux/etherdevice.h>
#include <linux/skbuff.h>
#include <linux/delay.h>
#include <linux/if_ether.h>   
#include <linux/if_arp.h>   
#include <linux/net.h>
#include <linux/wireless.h>

#include <net/iw_handler.h>

#include "ap8xLnxDesc.h"
#include "ap8xLnxApi.h"
#include "StaDb.h"

extern int wlDataTx(struct sk_buff *, struct net_device *);
extern void wlTxDone(struct net_device *);

#ifndef ETHERTYPE_IP
#define ETHERTYPE_IP        0x0800      /* IP protocol */
#define ETHERTYPE_IP_NW     0x0008      /* IP protocol network byte order*/
#endif
#ifndef ETHERTYPE_IPV6
#define ETHERTYPE_IPV6	0x86dd		/* IPv6 */
#endif
/*
 * Don't use sizeof(struct llc_un) for LLC header sizes
 */
#define LLC_ISFRAMELEN      4
#define LLC_UFRAMELEN       3
#define LLC_FRMRLEN         7
#define LLC_SNAPFRAMELEN    8

#define NBR_BYTES_FW_RX_PREPEND_LEN   2
#define NBR_BYTES_FW_TX_PREPEND_LEN   2
#define NBR_BYTES_CTRLSTATUS          2
#define NBR_BYTES_DURATION            2
#define NBR_BYTES_ADDR1               6
#define NBR_BYTES_ADDR2               6
#define NBR_BYTES_ADDR3               6
#define NBR_BYTES_SEQFRAG             2
#define NBR_BYTES_ADDR4               6
#define NBR_BYTES_TIMESTAMP           8
#define NBR_BYTES_BEACON_INTERVAL     2
#define NBR_BYTES_CAP_INFO            2
#define NBR_BYTES_FCS                 4

#define NBR_BYTES_ADD_RXFWINFO        ((NBR_BYTES_ADDR4) + \
                                       (NBR_BYTES_FW_RX_PREPEND_LEN))

#define NBR_BYTES_ADD_TXFWINFO        ((NBR_BYTES_ADDR4) + \
                                       (NBR_BYTES_FW_TX_PREPEND_LEN))

#define NBR_BYTES_COMPLETE_TXFWHEADER ((NBR_BYTES_FW_TX_PREPEND_LEN) + \
                                       (NBR_BYTES_CTRLSTATUS)        + \
                                       (NBR_BYTES_DURATION)          + \
                                       (NBR_BYTES_ADDR1)             + \
                                       (NBR_BYTES_ADDR2)             + \
                                       (NBR_BYTES_ADDR3)             + \
                                       (NBR_BYTES_SEQFRAG)           + \
                                       (NBR_BYTES_ADDR4))

#define NBR_BYTES_IEEE80211HEADER     ((NBR_BYTES_CTRLSTATUS) + \
                                       (NBR_BYTES_DURATION)   + \
                                       (NBR_BYTES_ADDR1)      + \
                                       (NBR_BYTES_ADDR2)      + \
                                       (NBR_BYTES_ADDR3)      + \
                                       (NBR_BYTES_SEQFRAG))
                                       
#define NBR_BYTES_COMPLETE_IEEE80211HEADER     ((NBR_BYTES_CTRLSTATUS) + \
                                                (NBR_BYTES_DURATION)   + \
                                                (NBR_BYTES_ADDR1)      + \
                                                (NBR_BYTES_ADDR2)      + \
                                                (NBR_BYTES_ADDR3)      + \
                                                (NBR_BYTES_SEQFRAG)    + \
                                                (NBR_BYTES_ADDR4))                                                


#define NBR_BYTES_IEEE80211COPYLEN    ((NBR_BYTES_IEEE80211HEADER) - \
                                       (NBR_BYTES_ADDR4))

#define OFFS_IEEE80211HEADER           0
#define OFFS_IEEE80211PAYLOAD          (NBR_BYTES_IEEE80211HEADER)
#define OFFS_TXFWBUFF_IEEE80211HEADER  (NBR_BYTES_FW_TX_PREPEND_LEN)
#define OFFS_TXFWBUFF_IEEE80211PAYLOAD (NBR_BYTES_COMPLETE_TXFWHEADER)
#define OFFS_RXFWBUFF_IEEE80211HEADER  (NBR_BYTES_FW_TX_PREPEND_LEN)
#define OFFS_RXFWBUFF_IEEE80211PAYLOAD (NBR_BYTES_COMPLETE_TXFWHEADER)

extern int wlxmit(struct net_device *netdev, struct sk_buff *skb, UINT8 type, extStaDb_StaInfo_t *pStaInfo, UINT32 bcast, BOOLEAN eap);
void wlDataTxHdl(struct net_device *netdev);
#endif /* AP8X_XMIT_H_ */
