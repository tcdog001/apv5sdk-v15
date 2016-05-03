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

#ifndef AP8X_WIDS_
#define AP8X_WIDS_


enum {
	WIDS_DETECT_TYPE_FLOOD,
	WIDS_DETECT_TYPE_SPOOF,
	WIDS_DETECT_TYPE_WEAKIV,
};

/*pengruofeng  add for wids 2011-5-30*/
#define IEEE80211_WIDS_FLOOD_DETECT 0x01
#define IEEE80211_WIDS_SPOOF_DETECT 0x02
#define IEEE80211_WIDS_WEAKIV_DETECT 0x03

#define IEEE80211_WIDS_FPROBE_REQUEST 0x01
#define IEEE80211_WIDS_FAUTH_REQUEST 0x02
#define IEEE80211_WIDS_FASSOC_REQUEST 0x03
#define IEEE80211_WIDS_FREASSOC_REQUEST 0x04
#define IEEE80211_WIDS_FDEAUTH_REQUEST 0x05
#define IEEE80211_WIDS_FDISASSO_REQUEST 0x06
#define IEEE80211_WIDS_FNULL_DATA 0x07
#define IEEE80211_WIDS_FACTION 0x08

#define IEEE80211_STA_INTERFER_DETECT 0x09
#define IEEE80211_STA_INTERFER_CLEAR 0x0a
#define IEEE80211_WIDS_SPOOF_DEAUTH 0x01
#define IEEE80211_WIDS_SPOOF_DISASSOC 0x02
/*pengruofeng add end 2011-5-30*/

/*pengruofeng--add for wids 2011-5-30*/
#define	WIDS_LOCK_INIT(_WIDS, _name)					\
	spin_lock_init(&(_WIDS)->attack_lock)
#define	WIDS_LOCK_DESTROY(_WIDS)
#define	WIDS_LOCK_IRQ(_WIDS, FLAG) 	spin_lock_irqsave(&(_WIDS)->attack_lock, (FLAG))
#define	WIDS_UNLOCK_IRQ(_WIDS, FLAG)	 spin_unlock_irqrestore(&(_WIDS)->attack_lock, (FLAG))
#define	WIDS_UNLOCK_IRQ_EARLY(_WIDS, FLAG)	  spin_unlock_irqrestore(&(_WIDS)->attack_lock, FLAG)
/*pengruofeng--add end*/

/*
 * 802.11 protocol definitions.
 */

/* is 802.11 address multicast/broadcast? */
#define IEEE80211_IS_MULTICAST(_a)  (*(_a) & 0x01)
#define IEEE80211_IS_BROADCAST(_a)              \
    ((_a)[0] == 0xff &&                         \
     (_a)[1] == 0xff &&                         \
     (_a)[2] == 0xff &&                         \
     (_a)[3] == 0xff &&                         \
     (_a)[4] == 0xff &&                         \
     (_a)[5] == 0xff)

void disable_wids_timer(vmacApInfo_t *vap);

#endif