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
#ifndef AP8X_IOCTL_H_
#define AP8X_IOCTL_H_

#include <linux/version.h>
#include <linux/wireless.h>

#define WL_IOCTL_WL_PARAM               (SIOCIWFIRSTPRIV + 0)
#define WL_IOCTL_WL_GET_PARAM           (SIOCIWFIRSTPRIV + 1)
#define IEEE80211_IOCTL_SETKEY          (SIOCIWFIRSTPRIV + 2)
//#define WL_IOCTL_BSS_START            (SIOCIWFIRSTPRIV + 2)
#define WL_IOCTL_GET_VERSION            (SIOCIWFIRSTPRIV + 3)
#define WL_IOCTL_SET_TXRATE             (SIOCIWFIRSTPRIV + 4)
#define WL_IOCTL_GET_TXRATE             (SIOCIWFIRSTPRIV + 5)
//#define WL_IOCTL_SET_CIPHERSUITE      (SIOCIWFIRSTPRIV + 6)
/*add by zhanxuechao*/
#define IEEE80211_IOCTL_SETMLME         (SIOCIWFIRSTPRIV + 6)
#define WL_IOCTL_GET_CIPHERSUITE        (SIOCIWFIRSTPRIV + 7)
#define WL_IOCTL_SET_PASSPHRASE         (SIOCIWFIRSTPRIV + 8)
#define WL_IOCTL_GET_PASSPHRASE         (SIOCIWFIRSTPRIV + 9)
#define WL_IOCTL_SET_FILTERMAC          (SIOCIWFIRSTPRIV + 10)
#define WL_IOCTL_GET_FILTERMAC          (SIOCIWFIRSTPRIV + 11)
#define WL_IOCTL_SET_BSSID              (SIOCIWFIRSTPRIV + 12)
#define WL_IOCTL_GET_BSSID              (SIOCIWFIRSTPRIV + 13)
#define WL_IOCTL_SET_TXPOWER            (SIOCIWFIRSTPRIV + 14)
#define WL_IOCTL_GET_TXPOWER            (SIOCIWFIRSTPRIV + 15)
#define WL_IOCTL_SET_WMMEDCAAP          (SIOCIWFIRSTPRIV + 16)
#define WL_IOCTL_GET_WMMEDCAAP          (SIOCIWFIRSTPRIV + 17)
#define WL_IOCTL_SET_WMMEDCASTA         (SIOCIWFIRSTPRIV + 18)
#define WL_IOCTL_GET_WMMEDCASTA         (SIOCIWFIRSTPRIV + 19)
#define WL_IOCTL_SETCMD                 (SIOCIWFIRSTPRIV + 20)
#define WL_IOCTL_GETCMD                 (SIOCIWFIRSTPRIV + 25)
#define WL_IOCTL_GET_STALISTEXT         (SIOCIWFIRSTPRIV + 21)
#define WL_IOCTL_SET_APPIE              (SIOCIWFIRSTPRIV + 22)
#ifdef GENERIC_GETIE
#define WL_IOCTL_GET_IE                 (SIOCIWFIRSTPRIV + 23)
#define WL_IOCTL_GET_SCAN_BSSPROFILE    (SIOCIWFIRSTPRIV + 31)
#else
#define WL_IOCTL_GET_RSNIE              (SIOCIWFIRSTPRIV + 23)
#define WL_IOCTL_GET_WSCIE              (SIOCIWFIRSTPRIV + 31)
#endif
#define WL_IOCTL_SET_CLIENT             (SIOCIWFIRSTPRIV + 24)
#define WL_IOCTL_SET_WDS_PORT           (SIOCIWFIRSTPRIV + 26)
#define WL_IOCTL_GET_WDS_PORT           (SIOCIWFIRSTPRIV + 27)
#define WL_IOCTL_SET_WAPI               (SIOCIWFIRSTPRIV + 28)
#define WL_IOCTL_GET_STASCAN            (SIOCIWFIRSTPRIV + 29)
#define WL_IOCTL_SET_CIPHERSUITE        (SIOCIWFIRSTPRIV + 30)
#define WL_IOCTL_BSS_START              (SIOCIWFIRSTPRIV + 32)

/******** zhouke add ,for mode and get_mode ********/
#define	IEEE80211_IOCTL_GETMODE         (SIOCDEVPRIVATE+11)
#define	IEEE80211_IOCTL_SETMODE         (SIOCDEVPRIVATE+12)
/*********************** end ***********************/

#if 0
#define	SIOCG80211STATS                 (SIOCDEVPRIVATE+2)
/* NB: require in+out parameters so cannot use wireless extensions, yech */
#define	IEEE80211_IOCTL_GETKEY          (SIOCDEVPRIVATE+3)
#define	IEEE80211_IOCTL_GETWPAIE        (SIOCDEVPRIVATE+4)
#define	IEEE80211_IOCTL_STA_STATS       (SIOCDEVPRIVATE+5)
#define	IEEE80211_IOCTL_STA_INFO        (SIOCDEVPRIVATE+6)
#define	SIOC80211IFCREATE               (SIOCDEVPRIVATE+7)
#define	SIOC80211IFDESTROY              (SIOCDEVPRIVATE+8)
#define	IEEE80211_IOCTL_SCAN_RESULTS    (SIOCDEVPRIVATE+9)
#define	IEEE80211_IOCTL_GETWSCIE        (SIOCDEVPRIVATE+10)
#define IEEE80211_IOCTL_GETMAC          (SIOCDEVPRIVATE+11)
#define IEEE80211_IOCTL_CONFIG_GENERIC  (SIOCDEVPRIVATE+12)
#define SIOCIOCTLTX99                   (SIOCDEVPRIVATE+13)
#define IEEE80211_IOCTL_P2P_BIG_PARAM   (SIOCDEVPRIVATE+14)
#define SIOCDEVVENDOR                   (SIOCDEVPRIVATE+15)    /* Used for ATH_SUPPORT_LINUX_VENDOR */
#define	IEEE80211_IOCTL_GET_SCAN_SPACE  (SIOCDEVPRIVATE+16)
/*zhaoyang1 transplant from 717*/
/*Begin:Added by duanmingzhe for autelan ioctl*/
#define	IEEE80211_IOCTL_AUTELAN_WMM		(SIOCDEVPRIVATE+17) /*autelan private wmm configure*/
#define	IEEE80211_IOCTL_TRAFFIC_LIMIT	(SIOCDEVPRIVATE+18) /*autelan private traffic limit*/
#define	IEEE80211_IOCTL_QUICK_ROAMING	(SIOCDEVPRIVATE+19) /*autelan private wds roaming*/
#define	IEEE80211_IOCTL_MAC_BINDING		(SIOCDEVPRIVATE+20)	/*ljy--add to develop the policy of mac binding*/
/*End:Added by duanmingzhe for autelan ioctl*/
#define	IEEE80211_IOCTL_80211_CACHE		(SIOCDEVPRIVATE+21)	/*Added by duanmingzhe for 80211 cache*/
/*zhaoyang1 transplant end*/

#else

#define	SIOCGATHSTATS                   (SIOCDEVPRIVATE+0)
#define	SIOCG80211STATS                 (SIOCDEVPRIVATE+2)
#define IEEE80211_IOCTL_GET_COUNTRYCODE   (SIOCDEVPRIVATE+3) // added by pengdecai for country code
#define IEEE80211_IOCTL_SET_COUNTRYCODE   (SIOCDEVPRIVATE+4) // added by pengdecai for country code
#define	IEEE80211_IOCTL_STA_STATS       (SIOCDEVPRIVATE+5)
#define	IEEE80211_IOCTL_STA_INFO		(SIOCDEVPRIVATE+6)
#define IEEE80211_IOCTL_AUTELAN_WMM     (SIOCDEVPRIVATE+7)  //(SIOCDEVPRIVATE+17)   //add for autelan wmm //lhj//2012.06
#define IEEE80211_IOCTL_TRAFFIC_LIMIT   (SIOCDEVPRIVATE+8) //(SIOCDEVPRIVATE+18)   //add for autelan traffic limit //lhj//2012.06
#define	IEEE80211_IOCTL_SCAN_RESULTS    (SIOCDEVPRIVATE+9)
/*Begin:Added by duanmingzhe for autelan ioctl*/
#define IEEE80211_IOCTL_MAC_BINDING     (SIOCDEVPRIVATE+10) /*ljy--add to develop the policy of mac binding*/
/*End:Added by duanmingzhe for autelan ioctl*/
#define SIOC80211IFCREATE               (SIOCDEVPRIVATE+13)
#define SIOC80211IFDESTROY              (SIOCDEVPRIVATE+14)

#ifdef AE5200_RT_SUPPORT
/*zhaoyang add for data forward base users*/
#define IEEE80211_IOCTL_SET_NIFLAG      (SIOCDEVPRIVATE+22)
/*zhaoyang modify end*/
#define	IEEE80211_IOCTL_ADDMAC		(SIOCDEVPRIVATE+18)        /* Add ACL MAC Address */
#define	IEEE80211_IOCTL_DELMAC		(SIOCDEVPRIVATE+16)        /* Del ACL MAC Address */

#endif

#endif

enum
{
    WL_PARAM_AUTHTYPE = 1, 
    WL_PARAM_BAND = 2,
    IEEE80211_AUTH_8021X = 3,
    WL_PARAM_HIDESSID = 4,

    /*add by zhanxuechao for cipher*/
    IEEE80211_PARAM_MCASTCIPHER = 5,        /* multicast/default cipher */
    IEEE80211_PARAM_MCASTKEYLEN = 6,        /* multicast key length */
    IEEE80211_PARAM_UCASTCIPHERS    = 7,    /* unicast cipher suites */
    
    WL_PARAM_DTIM = 8,
    WL_PARAM_FIXRATE = 9,
    /*add by zhanxuechao for cipher*/
    IEEE80211_PARAM_WPA     = 10,   /* WPA mode (0,1,2) */
    WL_PARAM_WPAWPA2MODE = 11,
    WL_PARAM_AUTHSUITE = 12,
    WL_PARAM_GROUPREKEYTIME = 13,
    WL_PARAM_WMM = 14,
    WL_PARAM_WMMACKPOLICY = 15,
    WL_PARAM_FILTER = 16,
    WL_PARAM_INTRABSS = 17,
    WL_PARAM_AMSDU = 18,
    WL_PARAM_HTBANDWIDTH = 19,
    WL_PARAM_GUARDINTERVAL = 20,
    /*add by zhanxuechao for cipher*/
    IEEE80211_PARAM_KEYMGTALGS  = 21,   /* key management algorithms */
    IEEE80211_PARAM_RSNCAPS     = 22,   /* RSN capabilities */

    /*change by zhanxuechao for access management timeout*/
    IEEE80211_PARAM_INACT=23,
    IEEE80211_PARAM_INACT_AUTH=24,
    IEEE80211_PARAM_INACT_INIT = 25,
    
    WL_PARAM_AUTOCHANNEL = 26,
    WL_PARAM_AMPDUFACTOR = 27,
    WL_PARAM_AMPDUDENSITY = 28,
    WL_PARAM_CARDDEVINFO = 29,
    WL_PARAM_INTEROP = 30,
    WL_PARAM_OPTLEVEL = 31,
    WL_PARAM_REGIONPWR = 32,
    WL_PARAM_ADAPTMODE = 33,
    WL_PARAM_SETKEYS = 34,
    WL_PARAM_DELKEYS = 35,
    WL_PARAM_MLME_REQ = 36,
    WL_PARAM_COUNTERMEASURES = 37,
    WL_PARAM_CSADAPTMODE = 38,
    WL_PARAM_DELWEPKEY = 39,
    WL_PARAM_WDSMODE = 40,
    WL_PARAM_STRICTWEPSHARE = 41,
    WL_PARAM_11H_CSA_CHAN = 42,
    WL_PARAM_11H_CSA_COUNT = 43,
    WL_PARAM_11H_CSA_MODE = 44,
    WL_PARAM_11H_CSA_START = 45,
    WL_PARAM_SPECTRUM_MGMT = 46,
    WL_PARAM_POWER_CONSTRAINT = 47,
    WL_PARAM_11H_DFS_MODE = 48,
    WL_PARAM_11D_MODE = 49,
    WL_PARAM_TXPWRFRACTION = 50,
    WL_PARAM_DISABLEASSOC = 51,
    WL_PARAM_PSHT_MANAGEMENTACT = 52,
    /* CLIENT_SUPPORT*/
    WL_PARAM_STAMODE = 53,
    WL_PARAM_STASCAN = 54,
    WL_PARAM_AMPDU_TX = 55,
    WL_PARAM_11HCACTIMEOUT = 56,
    WL_PARAM_11hNOPTIMEOUT = 57,
    WL_PARAM_11hDFSMODE = 58,
    WL_PARAM_MCASTPRXY = 59,
    WL_PARAM_11H_STA_MODE = 60,
    WL_PARAM_RSSI = 61,
    WL_PARAM_INTOLERANT = 62,
    WL_PARAM_TXQLIMIT = 63,
    WL_PARAM_RXINTLIMIT = 64,
    WL_PARAM_LINKSTATUS = 65,
    WL_PARAM_ANTENNATX = 66,
    WL_PARAM_RXPATHOPT = 67,
    WL_PARAM_HTGF = 68,
    WL_PARAM_HTSTBC = 69,
    WL_PARAM_3X3RATE = 70,
    WL_PARAM_AMSDU_FLUSHTIME = 71,
    WL_PARAM_AMSDU_MAXSIZE = 72,
    WL_PARAM_AMSDU_ALLOWSIZE = 73,
    WL_PARAM_AMSDU_PKTCNT = 74,
    WL_PARAM_CDD = 75,

    /*changed by zhanxuechao*/
    WL_PARAM_GETFWSTAT=76,
    WL_PARAM_AGINGTIME=77,
    WL_PARAM_ANTENNATX2 = 78,

    WL_PARAM_PREAMBLE = 79,
    WL_PARAM_GPROTECT = 80,
    WL_PARAM_BEACON = 81,

    WL_PARAM_ANTENNA = 82,

    WL_PARAM_EXTSUBCH = 83,
    WL_PARAM_HTPROTECT = 84,
 
    
    #if 0  //delete by wangxl 201211219 for the number from 86 to 93
    //add by guijinlin for attack detection begin
    WL_PARAM_MCSMASK = 85,
    WL_PARAM_FLOODDETECT = 86,
    WL_PARAM_SPOOFDETECT = 87,
    WL_PARAM_WIDS_INTERVAL = 88,
    WL_PARAM_CLEAR_ILLDEV_HISTORY = 89,
    WL_PARAM_CLEAR_ATTACK_HISTORY = 90,
    WL_PARAM_ATTACK_STATISTICS = 91,
    WL_PARAM_ATTACK_MAX_CNT = 92,
    WL_PARAM_PROBE_ATTACK_MAX_CNT = 93,  
    //add by guijinlin for attack detection end
    #endif
    //add by wangxl 20121219 begin
    WL_PARAM_RATE_MASK_CAP = 85,
    WL_PARAM_RATE_MASK_G = 86,
    WL_PARAM_RATE_MASK_B = 87,
    WL_PARAM_RATE_MASK_A = 88,
    WL_PARAM_MCS_MASK = 89,    
    WL_PARAM_MCS_CAP = 90,
    WL_PARAM_MCAST_RATE= 91,
    //add by wangxl 20121219 end
 
    //modify by lirui 2012-07-05 begin
    WL_PARAM_AUTH_AGINGTIME=94,     //zhouke add ,auth ageing time;
    /*Begin:Added by duanmingzhe for user isolation*/

    //add by lirui for set authmode begin
    IEEE80211_PARAM_PRIVACY=95,
    WL_PARAM_REGIONCODE = 96,//modify by lirui 3 to 97
    //add by lirui for set authmode end
    IEEE80211_PARAM_APBRIDGE = 97,  //zhouke add,AP inter-sta bridging
    WL_PARAM_WAPIMODE = 99,
    //add by zhanxuechao for WAPI begin

    IEEE80211_PARAM_PUREN		= 100,	/* pure 11n (no 11bg/11a stations) */
    IEEE80211_PARAM_COEXT_DISABLE    = 124, //zhouke add,for setcmd "intolerant40 0" 
#ifdef MRVL_WAPI
    IEEE80211_PARAM_SETWAPI=112,
    IEEE80211_PARAM_WAPIREKEY_USK=191,
    IEEE80211_PARAM_WAPIREKEY_MSK=192,
    IEEE80211_PARAM_WAPIREKEY_UPDATE=193,
#endif
    //add by zhanxuechao for WAPI end
    
	WL_PARAM_MAX_AMPDU_LEN = 101,       //77->101
	WL_PARAM_MAX_AMPDU_NUM = 102,       //78->102
	WL_PARAM_MAX_PACKET_LIFETIME = 103,	//79->103
    IEEE80211_PARAM_SWITCH = 231,
    /*End:Added by duanmingzhe for user isolation*/
       
    /*Begin:Added by pengdecai for country ID*/
    WL_PARAM_COUNTRYID  = 232,
    /*End :Added by pengdecai for country ID*/

    //add by wangxl20121219 for attack detection begin
    WL_PARAM_MCSMASK = 238,
    WL_PARAM_WIDS_INTERVAL = 239,
    WL_PARAM_FLOODDETECT = 240,
    WL_PARAM_SPOOFDETECT = 241,
    WL_PARAM_CLEAR_ILLDEV_HISTORY = 243,
    WL_PARAM_CLEAR_ATTACK_HISTORY = 244,
    WL_PARAM_ATTACK_STATISTICS = 245,
    WL_PARAM_ATTACK_MAX_CNT = 246,
    WL_PARAM_PROBE_ATTACK_MAX_CNT = 249,  
    //add by wangxl20121219 for attack detection end

    /*suzhaoyu add for customer online-traffic limit*/
    IEEE80211_PARAM_LOWEST_TRA_LIM = 256,
    IEEE80211_PARAM_LOWEST_TRA_TIMELENGTH = 257,
    IEEE80211_PARAM_LOWEST_TRA_THESHOLD = 258,
    /*suzhaoyu addend*/

    /*Begin:added by chenming for rssithre*/
    IEEE80211_PARAM_RSSITHRE_DATA_FRAME = 259,
    /*End:added by chenming for rssithre*/

    /*Begin:added by pengdecai for rxchainmask and txchainmask */
	IEEE80211_PARAM_RXCHAINMASK = 260, 
	IEEE80211_PARAM_TXCHAINMASK = 261, 
    /*End:added by pengdecai for rxchainmask and txchainmask */
    
    /*yuanding add for probe request restrict*/
    IEEE80211_PARAM_PROBE_REQUEST = 262,
    /*yuanding add end*/

    ATH_PARAM_ACKTIMEOUT = 263,
    ATH_PARAM_AMPDU_LIMIT = 264,
    ATH_PARAM_AMPDU_SUBFRAMES = 265,

#ifdef AE5200_RT_SUPPORT
    IEEE80211_PARAM_MACCMD = 266,
#endif

};

#define WL_KEY_XMIT         0x01    /* key used for xmit */
#define WL_KEY_RECV         0x02    /* key used for recv */
#define WL_KEY_GROUP        0x04    /* key used for WPA group operation */
#define WL_KEY_DEFAULT      0x80    /* default xmit key */
#define WL_KEYIX_NONE       ((u_int16_t) - 1)

#define WL_CIPHER_NONE      0x00
#define WL_CIPHER_WEP40     0x01
#define WL_CIPHER_TKIP      0x02
#define WL_CIPHER_WRAP      0x03
#define WL_CIPHER_CCMP      0x04
#define WL_CIPHER_WEP104    0x05

#define WL_IEEE80211_CIPHER_WEP         0x0
#define WL_IEEE80211_CIPHRASE_TKIP      0x1
#define WL_IEEE80211_CIPHRASE_AES_CCM   0x3
#define WL_IEEE80211_WAPI_PSK           0x4

#ifdef GENERIC_GETIE
struct wlreq_ie {
    u_int8_t    macAddr[6];
    u_int8_t    IEtype;
    u_int8_t    IELen;
    u_int8_t    IE[64];
};
#else
struct wlreq_rsnie {
    u_int8_t    macAddr[6];
    u_int8_t    rsnIE[64];
};
#endif

#ifdef MRVL_WSC
struct wlreq_wscie {
    u_int8_t    macAddr[6];
    u_int8_t    wscIE[280];
};
#endif

struct wlreq_key {
    u_int8_t ik_type;       /* key/cipher type */
    u_int8_t ik_pad;
    u_int16_t ik_keyix;     /* key index */
    u_int8_t ik_keylen;     /* key length in bytes */
    u_int8_t ik_flags;
    u_int8_t ik_macaddr[6];
    u_int64_t ik_keyrsc;        /* key receive sequence counter */
    u_int64_t ik_keytsc;        /* key transmit sequence counter */
    u_int8_t ik_keydata[16+8+8];
};

struct wlreq_del_key {
    u_int8_t idk_keyix;     /* key index */
    u_int8_t idk_macaddr[6];
};

#define WL_MLME_ASSOC       1   /* associate station */
#define WL_MLME_DISASSOC        2   /* disassociate station */
#define WL_MLME_DEAUTH      3   /* deauthenticate station */
#define WL_MLME_AUTHORIZE   4   /* authorize station */
#define WL_MLME_UNAUTHORIZE 5   /* unauthorize station */
#define WL_MLME_CLEAR_STATS 6   /* clear station statistic */

struct wlreq_mlme {
    u_int8_t im_op;         /* operation to perform */
    u_int8_t im_ssid_len;       /* length of optional ssid */
    u_int16_t im_reason;        /* 802.11 reason code */
    u_int8_t im_macaddr[6];
    u_int8_t im_ssid[32];
};

#ifdef MRVL_WSC
#define WL_APPIE_FRAMETYPE_BEACON           1
#define WL_APPIE_FRAMETYPE_PROBE_RESP       2

struct wlreq_set_appie {
    u_int32_t    appFrmType;
    u_int32_t   appBufLen;
    u_int8_t    appBuf[504]; /*total size of 512 bytes */
}__attribute__ ((packed));

#endif //MRVL_WSC

#ifdef MRVL_WAPI
/* come from wapid, 1 and 2 is useed as beacon/probe-resp */
#define P80211_PACKET_WAPIFLAG          0
#define P80211_PACKET_SETKEY            3

#define KEY_LEN             16      
/* from wapid */
struct wlreq_wapi_key {
    u_int8_t ik_macaddr[6];        /* sta mac, all "ff" for mcastkey */
    u_int8_t ik_flags;             /* always = 1 */
    u_int8_t ik_keyid;             /* key index */
    u_int8_t ik_keydata[KEY_LEN*2];/* mcastkey: 32 byte key; ucastkey: uek (16 byte) + uck (16 byte) */
};
#endif

#endif /* AP8X_IOCTL_H_ */
