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
#ifndef AP8X_LNX_IOCTL_WIRESS_H_
#define AP8X_LNX_IOCTL_WIRESS_H_

#include <linux/version.h>
#include <linux/wireless.h>

#define IEEE80211_ADDR_LEN                  6
#define IEEE80211_NWID_LEN                  32

#define IEEE80211_MLME_ASSOC        1   /* associate station */
#define IEEE80211_MLME_DISASSOC     2   /* disassociate station */
#define IEEE80211_MLME_DEAUTH       3   /* deauthenticate station */
#define IEEE80211_MLME_AUTHORIZE    4   /* authorize station */
#define IEEE80211_MLME_UNAUTHORIZE  5   /* unauthorize station */
#define IEEE80211_MLME_STOP_BSS     6   /* stop bss */
#define IEEE80211_MLME_CLEAR_STATS  7   /* clear station statistic */

/* crypto related defines*/
#define IEEE80211_KEYBUF_SIZE   16
#define IEEE80211_MICBUF_SIZE   (8+8)   /* space for both tx+rx keys */

#define WPA_ASE_NONE                    0x00
#define WPA_ASE_8021X_UNSPEC            0x01
#define WPA_ASE_8021X_PSK               0x02
#define WPA_ASE_CCKM                    0x80

/*
 * Cipher types.
 * NB: The values are preserved here to maintain binary compatibility
 * with applications like wpa_supplicant and hostapd.
 */
typedef enum _ieee80211_cipher_type {
    IEEE80211_CIPHER_WEP        = 0,
    IEEE80211_CIPHER_TKIP       = 1,
    IEEE80211_CIPHER_AES_OCB    = 2,
    IEEE80211_CIPHER_AES_CCM    = 3,
    IEEE80211_CIPHER_WAPI       = 4,
    IEEE80211_CIPHER_CKIP       = 5,
    IEEE80211_CIPHER_NONE       = 6,
} ieee80211_cipher_type;

#define IEEE80211_CIPHER_MAX    (IEEE80211_CIPHER_NONE+1)

/*
 * MLME state manipulation request.  IEEE80211_MLME_ASSOC
 * only makes sense when operating as a station.  The other
 * requests can be used when operating as a station or an
 * ap (to effect a station).
 */
struct ieee80211req_mlme {
    u_int8_t    im_op;      /* operation to perform */
    u_int8_t    im_ssid_len;    /* length of optional ssid */
    u_int16_t   im_reason;  /* 802.11 reason code */
    u_int8_t    im_macaddr[IEEE80211_ADDR_LEN];
    u_int8_t    im_ssid[IEEE80211_NWID_LEN];
};

/*
 * WPA/RSN get/set key request.  Specify the key/cipher
 * type and whether the key is to be used for sending and/or
 * receiving.  The key index should be set only when working
 * with global keys (use IEEE80211_KEYIX_NONE for ``no index'').
 * Otherwise a unicast/pairwise key is specified by the bssid
 * (on a station) or mac address (on an ap).  They key length
 * must include any MIC key data; otherwise it should be no
 more than IEEE80211_KEYBUF_SIZE.
 */
struct ieee80211req_key {
    u_int8_t    ik_type;    /* key/cipher type */
    u_int8_t    ik_pad;
    u_int16_t   ik_keyix;   /* key index */
    u_int8_t    ik_keylen;  /* key length in bytes */
    u_int8_t    ik_flags;
/* NB: IEEE80211_KEY_XMIT and IEEE80211_KEY_RECV defined elsewhere */
#define IEEE80211_KEY_DEFAULT   0x80    /* default xmit key */
    u_int8_t    ik_macaddr[IEEE80211_ADDR_LEN];
    u_int64_t   ik_keyrsc;  /* key receive sequence counter */
    u_int64_t   ik_keytsc;  /* key transmit sequence counter */
    u_int8_t    ik_keydata[IEEE80211_KEYBUF_SIZE+IEEE80211_MICBUF_SIZE];
};

typedef enum _ieee80211_rsn_param {
    IEEE80211_UCAST_CIPHER_LEN,
    IEEE80211_MCAST_CIPHER_LEN,
    IEEE80211_KEYMGT_ALGS,
    IEEE80211_RSN_CAPS
} ieee80211_rsn_param;

struct ieee80211_rsnparms {
    u_int32_t               rsn_authmodeset;        /* authentication mode set */
    u_int32_t               rsn_ucastcipherset;     /* unicast cipher set */
    u_int32_t               rsn_mcastcipherset;     /* mcast/group cipher set */
    u_int8_t                rsn_ucastkeylen;        /* unicast key length */
    u_int8_t                rsn_mcastkeylen;        /* mcast key length */
    u_int8_t                rsn_keymgmtset;         /* key mangement algorithms */
    u_int16_t               rsn_caps;               /* capabilities */
};

struct ieee80211_autelan_mac_binding {
#define SET_DHCP_ENABLE         1
#define GET_DHCP_ENABLE         2
#define SET_IP_AUTO_LEARNING    3
#define GET_IP_AUTO_LEARNING    4
#define SET_IP                  5
#define GET_IP                  6
#define SET_VLAN                7
#define GET_VLAN                8
/*sheyanbin--add to through the pppoe frame  */
#define SET_PPPOE_ENABLE        9
#define GET_PPPOE_ENABLE        10
/*sheyanbin--acd end*/

    unsigned char   type;           /* request type*/
    unsigned int    arg1;
    u_int8_t macaddr[IEEE80211_ADDR_LEN];
    u_int8_t ipaddr[4];
};

int ieee80211_ioctl_setmlme(struct net_device *dev, struct iw_request_info *info,
    void *w, char *extra);
int ieee80211_ioctl_setkey(struct net_device *dev, struct iw_request_info *info,
    void *w, char *extra);
int ieee80211_ioctl_autelan_mac_binding(struct net_device *dev, struct iwreq *iwr);

int wlan_get_rsn_cipher_param(struct ieee80211_rsnparms *rsn, ieee80211_rsn_param type);
int wlan_set_rsn_cipher_param(struct ieee80211_rsnparms *rsn, ieee80211_rsn_param type, int value);
#endif

