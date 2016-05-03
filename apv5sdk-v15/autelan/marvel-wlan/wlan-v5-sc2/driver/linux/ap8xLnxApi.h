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
#ifndef	AP8X_API_H_
#define	AP8X_API_H_

enum ieee80211_phymode {
    IEEE80211_MODE_AUTO             = 0,    /* autoselect */
    IEEE80211_MODE_11A              = 1,    /* 5GHz, OFDM */
    IEEE80211_MODE_11B              = 2,    /* 2GHz, CCK */
    IEEE80211_MODE_11G              = 3,    /* 2GHz, OFDM */
    IEEE80211_MODE_FH               = 4,    /* 2GHz, GFSK */
    IEEE80211_MODE_TURBO_A          = 5,    /* 5GHz, OFDM, 2x clock dynamic turbo */
    IEEE80211_MODE_TURBO_G          = 6,    /* 2GHz, OFDM, 2x clock  dynamic turbo*/
    IEEE80211_MODE_11NA_HT20        = 15,    /* 5Ghz, HT20 */
    IEEE80211_MODE_11NG_HT20        = 16,    /* 2Ghz, HT20 */
    IEEE80211_MODE_11NA_HT40PLUS    = 17,    /* 5Ghz, HT40 (ext ch +1) */
    IEEE80211_MODE_11NA_HT40MINUS   = 18,   /* 5Ghz, HT40 (ext ch -1) */
    IEEE80211_MODE_11NG_HT40PLUS    = 19,   /* 2Ghz, HT40 (ext ch +1) */
    IEEE80211_MODE_11NG_HT40MINUS   = 20,   /* 2Ghz, HT40 (ext ch -1) */
    IEEE80211_MODE_11NG_HT40        = 21,   /* 2Ghz, Auto HT40 */
    IEEE80211_MODE_11NA_HT40        = 22,   /* 2Ghz, Auto HT40 */
};

int wldo_ioctl(struct net_device *dev , struct ifreq  *rq, int cmd);
struct iw_statistics *wlGetStats(struct net_device *dev);

extern int wlIoctl(struct net_device *dev , struct ifreq  *rq, int cmd);
extern int wlSetupWEHdlr(struct net_device *netdev);

#ifdef AE5200_RT_SUPPORT
/*zhaoyang add for data forward base users*/
struct ieee80211_autelan_tunnel_ctl{

#define SET_NIFLAG	        1
/*Begin: zhaoyang1 add for getting infomation of niflag 2013-03-23*/
#define GET_NIFLAG			2
/*End: zhaoyang1 add for getting infomation of niflag 2013-03-23*/
	unsigned char   type;  			/* request type*/
	u_int8_t		ni_macaddr[6];
	u_int16_t       ni_localflags;
	u_int8_t		ni_portal_ipaddr[4]; //record the portal server IP addr 
};
/*zhaoyang modify end*/

#endif

#endif /* AP8X_API_H_ */


