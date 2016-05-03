#ifndef __CAPWAP_WTPDriverinteraction_HEADER__
#define __CAPWAP_WTPDriverinteraction_HEADER__
#include "CWWTP.h"

#if 0
#ifndef NO_USE_MADWIFI
#include <ath/if_athioctl.h>
#include <net80211/ieee80211_crypto.h>
#include <net80211/ieee80211.h>
#include <net80211/_ieee80211.h>
#include <net80211/ieee80211_ioctl.h>
#include <net80211/ieee80211_node.h>
#endif
#endif


#define SIOCDEVPRIVATE	0x89F0	/* to 89FF */


/* pei add for rogue AP test .  1118 */
enum interfaceType {
	ATH=0,
	ETH,
	WIFI,
};


typedef struct {  //pei add 0422
	unsigned char eth_count;
	interface_status *eth_status;
	unsigned char wifi_count;
	interface_status *wifi_status;
} ifState;


/* caisicong my  led need */
typedef struct wrcpu_op_args_s {
	unsigned long  op_addr;
	unsigned long  op_value; // ignore on read in arg, fill read value on write return value
	unsigned short op_byteorder; // destination byte order. default is bigendiana.
	unsigned short op_len;
	unsigned short op_ret; // 0 for success, other value indicate different failure.
	unsigned int num;//which gpio
	unsigned int status;/*0 off 1 on*/
	unsigned char basemac[6];
	unsigned char boardID[20];/*wtp sn*/
	unsigned int factory_reset_get;
#ifdef AQ2010
	unsigned char OEM_INFO[64];  /* rbk add 090107 */
#endif
} wrcpu_op_args;

#define WRCPU_IOC_MAGIC 10
#define WRCPU_IOC_G_  _IOWR(WRCPU_IOC_MAGIC,1,wrcpu_op_args) // read values
#define WRCPU_IOC_X_ _IOWR(WRCPU_IOC_MAGIC,2,wrcpu_op_args) // write and readout wrote value
#define WRCPU_IOC_S_ _IOWR(WRCPU_IOC_MAGIC,3,wrcpu_op_args)//set the select
#define WRCPU_IOC_D_ _IOWR(WRCPU_IOC_MAGIC,4,wrcpu_op_args)//down the blind timer
#define WRCPU_IOC_U_ _IOWR(WRCPU_IOC_MAGIC,5,wrcpu_op_args)/*up the blinding*/ 
#define WRCPU_IOC_B_ _IOWR(WRCPU_IOC_MAGIC,6,wrcpu_op_args)//blue led on 
#define WRCPU_IOC_I_ _IOWR(WRCPU_IOC_MAGIC,7,wrcpu_op_args)/*get the sysinf*/
#define WRCPU_FLASH_W_ _IOWR(WRCPU_IOC_MAGIC,8,wrcpu_op_args)/*write flash*/
#define WRCPU_WATCHDOG _IOWR(WRCPU_IOC_MAGIC,9,wrcpu_op_args)/*write watchdog*/
#define FACTORY_RESET _IOWR(WRCPU_IOC_MAGIC,10,wrcpu_op_args)/*open factory reset*/
#define WRCPU_IOC_MAXNR 15

/* peiwenhui add 0703 --netlink */
#define RTA_OK(rta,len) \
((len) > 0 && (rta)->rta_len >= sizeof(struct rtattr) && \
(rta)->rta_len <= (len))
#define RTA_NEXT(rta,attrlen) \
((attrlen) -= RTA_ALIGN((rta)->rta_len), \
(struct rtattr *) (((char *)(rta)) + RTA_ALIGN((rta)->rta_len)))

#ifndef CONFIG_NO_SOCKLEN_T_TYPEDEF
#ifndef __socklen_t_defined
typedef int socklen_t;
#endif
#endif

/*
typedef unsigned int u32;
typedef unsigned short u16;
typedef unsigned char u8;
*/

struct ifinfomsg
{
	unsigned char ifi_family;
	unsigned char __ifi_pad;
	unsigned short ifi_type;
	int ifi_index;
	unsigned ifi_flags;
	unsigned ifi_change;
};
struct nlmsghdr
{
	u32 nlmsg_len;
	u16 nlmsg_type;
	u16 nlmsg_flags;
	u32 nlmsg_seq;
	u32 nlmsg_pid;
};
struct rtattr
{
	unsigned short rta_len;
	unsigned short rta_type;
};
struct sockaddr_nl
{
	sa_family_t nl_family;
	unsigned short nl_pad;
	u32 nl_pid;
	u32 nl_groups;
};


//#ifdef NO_USE_MADWIFI
/*******************wireless_copy.h*****************************************/
#define SIOCIWFIRSTPRIV			0x8BE0

/*******************ieee80211_crypto.h**************************************/
/*
 * NB: these values are ordered carefully; there are lots of
 * of implications in any reordering.  In particular beware
 * that 4 is not used to avoid conflicting with IEEE80211_F_PRIVACY.
 */
 
/*
 * 802.11 protocol crypto-related definitions.
 */
#define	IEEE80211_KEYBUF_SIZE	16
#define	IEEE80211_MICBUF_SIZE	(8 + 8)		/* space for both TX & RX keys */
#define	IEEE80211_TID_SIZE		17		/* total number of TIDs */

/*
 * Old WEP-style key.  Deprecated.
 */
struct ieee80211_wepkey {
	u_int wk_len;				/* key length in bytes */
	u_int8_t wk_key[IEEE80211_KEYBUF_SIZE];
};
typedef u_int16_t ieee80211_keyix_t;
struct ieee80211_key {
	u_int8_t wk_keylen;		/* key length in bytes */
	u_int8_t wk_flags;
#define	IEEE80211_KEY_XMIT	0x01	/* key used for xmit */
#define	IEEE80211_KEY_RECV	0x02	/* key used for recv */
#define	IEEE80211_KEY_GROUP	0x04	/* key used for WPA group operation */
#define	IEEE80211_KEY_SWCRYPT	0x10	/* host-based encrypt/decrypt */
#define	IEEE80211_KEY_SWMIC	0x20	/* host-based enmic/demic */
	ieee80211_keyix_t wk_keyix;	/* key index */
	u_int8_t wk_key[IEEE80211_KEYBUF_SIZE+IEEE80211_MICBUF_SIZE];
#define	wk_txmic	wk_key+IEEE80211_KEYBUF_SIZE+0	/* XXX can't () right */
#define	wk_rxmic	wk_key+IEEE80211_KEYBUF_SIZE+8	/* XXX can't () right */
	u_int64_c wk_keyrsc[IEEE80211_TID_SIZE];	/* key receive sequence counter */
	u_int64_c wk_keytsc;		/* key transmit sequence counter */
	const struct ieee80211_cipher *wk_cipher;
	void *wk_private;		/* private cipher state */
};
#define	IEEE80211_KEY_COMMON 		/* common flags passed in by apps */\
	(IEEE80211_KEY_XMIT | IEEE80211_KEY_RECV | IEEE80211_KEY_GROUP)

#define	IEEE80211_CIPHER_WEP		0
#define	IEEE80211_CIPHER_TKIP		1
#define	IEEE80211_CIPHER_AES_OCB	2
#define	IEEE80211_CIPHER_AES_CCM	3
#define	IEEE80211_CIPHER_CKIP		5
#define	IEEE80211_CIPHER_NONE		6	/* pseudo value */

#define	IEEE80211_CIPHER_MAX		(IEEE80211_CIPHER_NONE + 1)

#define	IEEE80211_KEYIX_NONE	((ieee80211_keyix_t) -1)

/*******************ieee80211.h**************************************/
#define	IEEE80211_NWID_LEN			32

#define	WPA_OUI			0xf25000
#define	WPA_OUI_TYPE		0x01


#define	IEEE80211_CAPINFO_ESS			0x0001
#define	IEEE80211_CAPINFO_IBSS			0x0002
#define	IEEE80211_CAPINFO_CF_POLLABLE		0x0004
#define	IEEE80211_CAPINFO_CF_POLLREQ		0x0008
#define	IEEE80211_CAPINFO_PRIVACY		0x0010
#define	IEEE80211_CAPINFO_SHORT_PREAMBLE	0x0020
#define	IEEE80211_CAPINFO_PBCC			0x0040
#define	IEEE80211_CAPINFO_CHNL_AGILITY		0x0080
/* bits 8-9 are reserved (8 now for specturm management) */
#define IEEE80211_CAPINFO_SPECTRUM_MGMT		0x0100
#define	IEEE80211_CAPINFO_SHORT_SLOTTIME	0x0400
#define	IEEE80211_CAPINFO_RSN			0x0800
/* bit 12 is reserved */
#define	IEEE80211_CAPINFO_DSSSOFDM		0x2000
/* bits 14-15 are reserved */

/*
 * Management information element payloads.
 */
enum {
	IEEE80211_ELEMID_SSID		= 0,
	IEEE80211_ELEMID_RATES		= 1,
	IEEE80211_ELEMID_FHPARMS	= 2,
	IEEE80211_ELEMID_DSPARMS	= 3,
	IEEE80211_ELEMID_CFPARMS	= 4,
	IEEE80211_ELEMID_TIM		= 5,
	IEEE80211_ELEMID_IBSSPARMS	= 6,
	IEEE80211_ELEMID_COUNTRY	= 7,
	IEEE80211_ELEMID_HOPPATRNPARMS  = 8,
	IEEE80211_ELEMID_HOPPATRNTABLE  = 9,
	IEEE80211_ELEMID_REQINFO	= 10,
	IEEE80211_ELEMID_QBSSLOAD       = 11,
	IEEE80211_ELEMID_EDCAPARAM      = 12,
	IEEE80211_ELEMID_TSPEC		= 13,
	IEEE80211_ELEMID_TRAFCLASS      = 14,
	IEEE80211_ELEMID_SCHEDULE       = 15,
	IEEE80211_ELEMID_CHALLENGE	= 16,
	/* 17-31 reserved for challenge text extension */
	IEEE80211_ELEMID_PWRCNSTR	= 32,
	IEEE80211_ELEMID_PWRCAP		= 33,
	IEEE80211_ELEMID_TPCREQ		= 34,
	IEEE80211_ELEMID_TPCREP		= 35,
	IEEE80211_ELEMID_SUPPCHAN	= 36,
	IEEE80211_ELEMID_CHANSWITCHANN	= 37,
	IEEE80211_ELEMID_MEASREQ	= 38,
	IEEE80211_ELEMID_MEASREP	= 39,
	IEEE80211_ELEMID_QUIET		= 40,
	IEEE80211_ELEMID_IBSSDFS	= 41,
	IEEE80211_ELEMID_ERP		= 42,
	IEEE80211_ELEMID_TSDELAY	= 43,
	IEEE80211_ELEMID_TCLASSPROC     = 44,
	IEEE80211_ELEMID_QOS		= 46,
	IEEE80211_ELEMID_RSN		= 48,
	/* 49 reserved */
	IEEE80211_ELEMID_XRATES		= 50,
	/*ljy--add begin for wapi*/
	IEEE80211_ELEMID_WAPI		= 68,
	/*ljy--add end*/
	/* 128-129 proprietary elements used by Agere chipsets */
	IEEE80211_ELEMID_AGERE1		= 128,
	IEEE80211_ELEMID_AGERE2		= 129,
	IEEE80211_ELEMID_TPC		= 150,
	IEEE80211_ELEMID_CCKM		= 156,
	IEEE80211_ELEMID_VENDOR		= 221,	/* vendor private */
};
/*******************_ieee80211.h**************************************/


/* * Authentication mode. */
enum ieee80211_authmode {
	IEEE80211_AUTH_NONE	= 0,	
	IEEE80211_AUTH_OPEN	= 1,		/* open */
	IEEE80211_AUTH_SHARED	= 2,		/* shared-key */	
	IEEE80211_AUTH_8021X	= 3,		/* 802.1x */	
	IEEE80211_AUTH_AUTO	= 4,		/* auto-select/accept */
	/* NB: these are used only for ioctls */
	IEEE80211_AUTH_WPA	= 5,	
	/* WPA/RSN w/ 802.1x/PSK */
};
enum ieee80211_opmode {
	IEEE80211_M_STA		= 1,	/* infrastructure station */
	IEEE80211_M_IBSS 	= 0,	/* IBSS (adhoc) station */
	IEEE80211_M_AHDEMO	= 3,	/* Old lucent compatible adhoc demo */
	IEEE80211_M_HOSTAP	= 6,	/* Software Access Point */
	IEEE80211_M_MONITOR	= 8,	/* Monitor mode */
	IEEE80211_M_WDS		= 2	/* WDS link */
};
/*******************if_athioctl.h**************************************/
#ifdef __linux__
#define	SIOCGATHDIAG	(SIOCDEVPRIVATE+1)
#define SIOCGATHRADARSIG (SIOCDEVPRIVATE+2)
//#else
//#define	SIOCGATHSTATS	_IOWR('i', 137, struct ifreq)
//#define	SIOCGATHDIAG	_IOWR('i', 138, struct ath_diag)
#endif

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
	u_int8_t ik_type;		/* key/cipher type */
	u_int8_t ik_pad;
	ieee80211_keyix_t ik_keyix;	/* key index */
	u_int8_t ik_keylen;		/* key length in bytes */
	u_int8_t ik_flags;
/* NB: IEEE80211_KEY_XMIT and IEEE80211_KEY_RECV defined elsewhere */
#define	IEEE80211_KEY_DEFAULT	0x80	/* default xmit key */
	u_int8_t ik_macaddr[MAC_ADDR_LEN];
	u_int64_c ik_keyrsc;		/* key receive sequence counter */
	u_int64_c ik_keytsc;		/* key transmit sequence counter */
	u_int8_t ik_keydata[IEEE80211_KEYBUF_SIZE+IEEE80211_MICBUF_SIZE];
};

/*
 * Delete a key either by index or address.  Set the index
 * to IEEE80211_KEYIX_NONE when deleting a unicast key.
 */
struct ieee80211req_del_key {
	/* XXX: This should be ieee80211_keyix_t, but it changes API/ABI for hostapd */
	u_int8_t idk_keyix;	/* key index */
	u_int8_t idk_macaddr[MAC_ADDR_LEN];
};
struct ieee80211_clone_params {
	char icp_name[IFNAMSIZ];		/* device name */
	u_int16_t icp_opmode;			/* operating mode */
	u_int16_t icp_flags;			/* see below */
#define	IEEE80211_CLONE_BSSID	0x0001		/* allocate unique mac/bssid */
#define	IEEE80211_NO_STABEACONS	0x0002		/* Do not setup the station beacon timers */
};
enum {
        IEEE80211_WMMPARAMS_CWMIN       = 1,
        IEEE80211_WMMPARAMS_CWMAX       = 2,
        IEEE80211_WMMPARAMS_AIFS        = 3,
        IEEE80211_WMMPARAMS_TXOPLIMIT   = 4,
        IEEE80211_WMMPARAMS_ACM         = 5,
        IEEE80211_WMMPARAMS_NOACKPOLICY = 6,
};

enum {
	IEEE80211_PARAM_TURBO			= 1,	/* turbo mode */
	IEEE80211_PARAM_MODE			= 2,	/* phy mode (11a, 11b, etc.) */
	IEEE80211_PARAM_AUTHMODE		= 3,	/* authentication mode */
	IEEE80211_PARAM_PROTMODE		= 4,	/* 802.11g protection */
	IEEE80211_PARAM_MCASTCIPHER		= 5,	/* multicast/default cipher */
	IEEE80211_PARAM_MCASTKEYLEN		= 6,	/* multicast key length */
	IEEE80211_PARAM_UCASTCIPHERS		= 7,	/* unicast cipher suites */
	IEEE80211_PARAM_UCASTCIPHER		= 8,	/* unicast cipher */
	IEEE80211_PARAM_UCASTKEYLEN		= 9,	/* unicast key length */
	IEEE80211_PARAM_WPA			= 10,	/* WPA mode (0,1,2) */
	IEEE80211_PARAM_ROAMING			= 12,	/* roaming mode */
	IEEE80211_PARAM_PRIVACY			= 13,	/* privacy invoked */
	IEEE80211_PARAM_COUNTERMEASURES		= 14,	/* WPA/TKIP countermeasures */
	IEEE80211_PARAM_DROPUNENCRYPTED		= 15,	/* discard unencrypted frames */
	IEEE80211_PARAM_DRIVER_CAPS		= 16,	/* driver capabilities */
	IEEE80211_PARAM_MACCMD			= 17,	/* MAC ACL operation */
	IEEE80211_PARAM_WMM			= 18,	/* WMM mode (on, off) */
	IEEE80211_PARAM_HIDESSID		= 19,	/* hide SSID mode (on, off) */
	IEEE80211_PARAM_APBRIDGE    		= 20,   /* AP inter-sta bridging */
	IEEE80211_PARAM_KEYMGTALGS		= 21,	/* key management algorithms */
	IEEE80211_PARAM_RSNCAPS			= 22,	/* RSN capabilities */
	IEEE80211_PARAM_INACT			= 23,	/* station inactivity timeout */
	IEEE80211_PARAM_INACT_AUTH		= 24,	/* station auth inact timeout */
	IEEE80211_PARAM_INACT_INIT		= 25,	/* station init inact timeout */
	IEEE80211_PARAM_ABOLT			= 26,	/* Atheros Adv. Capabilities */
	IEEE80211_PARAM_DTIM_PERIOD		= 28,	/* DTIM period (beacons) */
	IEEE80211_PARAM_BEACON_INTERVAL		= 29,	/* beacon interval (ms) */
	IEEE80211_PARAM_DOTH			= 30,	/* 11.h is on/off */
	IEEE80211_PARAM_PWRTARGET		= 31,	/* Current Channel Pwr Constraint */
	IEEE80211_PARAM_GENREASSOC		= 32,	/* Generate a reassociation request */
	IEEE80211_PARAM_COMPRESSION		= 33,	/* compression */
	IEEE80211_PARAM_FF			= 34,	/* fast frames support  */
	IEEE80211_PARAM_XR			= 35,	/* XR support */
	IEEE80211_PARAM_BURST			= 36,	/* burst mode */
	IEEE80211_PARAM_PUREG			= 37,	/* pure 11g (no 11b stations) */
	IEEE80211_PARAM_AR			= 38,	/* AR support */
	IEEE80211_PARAM_WDS			= 39,	/* Enable 4 address processing */
	IEEE80211_PARAM_BGSCAN			= 40,	/* bg scanning (on, off) */
	IEEE80211_PARAM_BGSCAN_IDLE		= 41,	/* bg scan idle threshold */
	IEEE80211_PARAM_BGSCAN_INTERVAL		= 42,	/* bg scan interval */
	IEEE80211_PARAM_MCAST_RATE		= 43,	/* Multicast Tx Rate */
	IEEE80211_PARAM_COVERAGE_CLASS		= 44,	/* coverage class */
	IEEE80211_PARAM_COUNTRY_IE		= 45,	/* enable country IE */
	IEEE80211_PARAM_SCANVALID		= 46,	/* scan cache valid threshold */
	IEEE80211_PARAM_ROAM_RSSI_11A		= 47,	/* rssi threshold in 11a */
	IEEE80211_PARAM_ROAM_RSSI_11B		= 48,	/* rssi threshold in 11b */
	IEEE80211_PARAM_ROAM_RSSI_11G		= 49,	/* rssi threshold in 11g */
	IEEE80211_PARAM_ROAM_RATE_11A		= 50,	/* tx rate threshold in 11a */
	IEEE80211_PARAM_ROAM_RATE_11B		= 51,	/* tx rate threshold in 11b */
	IEEE80211_PARAM_ROAM_RATE_11G		= 52,	/* tx rate threshold in 11g */
	IEEE80211_PARAM_UAPSDINFO		= 53,	/* value for qos info field */
	IEEE80211_PARAM_SLEEP			= 54,	/* force sleep/wake */
	IEEE80211_PARAM_QOSNULL			= 55,	/* force sleep/wake */
	IEEE80211_PARAM_PSPOLL			= 56,	/* force ps-poll generation (sta only) */
	IEEE80211_PARAM_EOSPDROP		= 57,	/* force uapsd EOSP drop (ap only) */
	IEEE80211_PARAM_MARKDFS			= 58,	/* mark a dfs interference channel when found */
	IEEE80211_PARAM_REGCLASS		= 59,	/* enable regclass ids in country IE */
	IEEE80211_PARAM_DROPUNENC_EAPOL		= 60,	/* drop unencrypted eapol frames */
 	IEEE80211_PARAM_SHPREAMBLE		= 61,	/* Short Preamble */
	IEEE80211_PARAM_DUMPREGS		= 62,   /* Pretty printed dump of Atheros hardware registers */
#ifdef CAVIUM_PLATFORM
	IEEE80211_PARAM_DOTH_ALGORITHM		= 63,	/* spectrum management algorithm */
	IEEE80211_PARAM_DOTH_MINCOM   		= 64,	/* minimum number of common channels */
	IEEE80211_PARAM_DOTH_SLCG		= 65,	/* permil of Stations Lost per Channel Gained */
	IEEE80211_PARAM_DOTH_SLDG		= 66,	/* permil of Stations Lost per rssi Db Gained */
	IEEE80211_PARAM_TXCONT			= 67,	/* continuous transmit mode (boolean) */
	IEEE80211_PARAM_TXCONT_RATE		= 68,	/* continuous transmit mode data rate (in mbit/sec) - will use closest match from current rate table */
	IEEE80211_PARAM_TXCONT_POWER		= 69,	/* power level in units of 0.5dBm */
	IEEE80211_PARAM_DFS_TESTMODE		= 70,	/* do not perform DFS actions (i.e. markng DFS and channel change on interference), just report them via debug. */
	IEEE80211_PARAM_DFS_CACTIME		= 71,	/* how long do we wait for chan availability
							   scans ?
							   FCC requires 60s, so that is the default. */
	IEEE80211_PARAM_DFS_EXCLPERIOD		= 72,	/* DFS no-occupancy limit - how long do we stay
							   off a channel once radar is detected?
							   FCC requires 30m, so that is the default. */
	IEEE80211_PARAM_BEACON_MISS_THRESH	= 73,	/* Beacon miss threshold (in beacons) */
	IEEE80211_PARAM_BEACON_MISS_THRESH_MS	= 74,	/* Beacon miss threshold (in ms) */
	IEEE80211_PARAM_MAXRATE			= 75,	/* Maximum rate (by table index) */
	IEEE80211_PARAM_MINRATE			= 76,	/* Maximum rate (by table index) */
	IEEE80211_PARAM_PROTMODE_RSSI		= 77,	/* RSSI Threshold for enabling protection mode */
	IEEE80211_PARAM_PROTMODE_TIMEOUT	= 78,	/* Timeout for expiring protection mode */
	IEEE80211_PARAM_BGSCAN_THRESH		= 79,	/* bg scan rssi threshold */
	IEEE80211_PARAM_RSSI_DIS_THR	= 80,	/* rssi threshold for disconnection */
	IEEE80211_PARAM_RSSI_DIS_COUNT	= 81,	/* counter for rssi threshold */
	IEEE80211_PARAM_WDS_SEP			= 82,	/* move wds stations into separate interfaces */
	IEEE80211_PARAM_SWITCH = 83,/*rbk new add switch*/
#endif
};
/*
 * Scan result data returned for IEEE80211_IOC_SCAN_RESULTS.
 */
 struct ieee80211req_scan_result {
	u_int16_t	isr_len;		/* length (mult of 4) */
	u_int16_t	isr_freq;		/* MHz */
	u_int32_t	isr_flags;		/* channel flags */
	u_int8_t	isr_noise;
	u_int8_t	isr_rssi;
	u_int8_t	isr_intval;		/* beacon interval */
	u_int16_t	isr_capinfo;		/* capabilities */
	u_int8_t	isr_erp;		/* ERP element */
	u_int8_t	isr_bssid[IEEE80211_ADDR_LEN];
	u_int8_t	isr_nrates;
	u_int8_t	isr_rates[IEEE80211_RATE_MAXSIZE];
	u_int8_t	isr_ssid_len;		/* SSID length */
	u_int16_t	isr_ie_len;		/* IE length */
	u_int8_t	isr_pad[4];
	/* variable length SSID followed by IE data */
};
/*
 * MLME state manipulation request.  IEEE80211_MLME_ASSOC
 * only makes sense when operating as a station.  The other
 * requests can be used when operating as a station or an
 * ap (to effect a station).
 */

struct ieee80211req_mlme {
	u_int8_t im_op;			/* operation to perform */
#define	IEEE80211_MLME_ASSOC		1	/* associate station */
#define	IEEE80211_MLME_DISASSOC		2	/* disassociate station */
#define	IEEE80211_MLME_DEAUTH		3	/* deauthenticate station */
#define	IEEE80211_MLME_AUTHORIZE	4	/* authorize station */
#define	IEEE80211_MLME_UNAUTHORIZE	5	/* unauthorize station */
#define	IEEE80211_MLME_STOP_BSS		6	/* stop bss */
#define IEEE80211_MLME_CLEAR_STATS	7	/* clear station statistic */
	u_int8_t im_ssid_len;		/* length of optional ssid */
	u_int16_t im_reason;		/* 802.11 reason code */
	u_int8_t im_macaddr[MAC_ADDR_LEN];
	u_int8_t im_ssid[IEEE80211_NWID_LEN];    /* pei modified 081218 */
};

#define	IEEE80211_IOCTL_SETPARAM	(SIOCIWFIRSTPRIV+0)
#define	IEEE80211_IOCTL_GETPARAM	(SIOCIWFIRSTPRIV+1)
#define	IEEE80211_IOCTL_SETKEY		(SIOCIWFIRSTPRIV+2)
#define	IEEE80211_IOCTL_SETWMMPARAMS	(SIOCIWFIRSTPRIV+3)
#define	IEEE80211_IOCTL_DELKEY		(SIOCIWFIRSTPRIV+4)
#define	IEEE80211_IOCTL_GETWMMPARAMS	(SIOCIWFIRSTPRIV+5)
#define	IEEE80211_IOCTL_SETMLME		(SIOCIWFIRSTPRIV+6)
#define	IEEE80211_IOCTL_GETCHANINFO	(SIOCIWFIRSTPRIV+7)
#define	IEEE80211_IOCTL_SETOPTIE	(SIOCIWFIRSTPRIV+8)
#define	IEEE80211_IOCTL_GETOPTIE	(SIOCIWFIRSTPRIV+9)
#define	IEEE80211_IOCTL_ADDMAC		(SIOCIWFIRSTPRIV+10)
#define	IEEE80211_IOCTL_DELMAC		(SIOCIWFIRSTPRIV+12)
#define	IEEE80211_IOCTL_GETCHANLIST	(SIOCIWFIRSTPRIV+13)
#define	IEEE80211_IOCTL_SETCHANLIST	(SIOCIWFIRSTPRIV+14)
#define IEEE80211_IOCTL_KICKMAC		(SIOCIWFIRSTPRIV+15)
#define	IEEE80211_IOCTL_CHANSWITCH	(SIOCIWFIRSTPRIV+16)
#define	IEEE80211_IOCTL_GETMODE		(SIOCIWFIRSTPRIV+17)
#define	IEEE80211_IOCTL_SETMODE		(SIOCIWFIRSTPRIV+18)
#define IEEE80211_IOCTL_GET_APPIEBUF    (SIOCIWFIRSTPRIV+19)
#define IEEE80211_IOCTL_SET_APPIEBUF    (SIOCIWFIRSTPRIV+20)
#define IEEE80211_IOCTL_SET_ACPARAMS    (SIOCIWFIRSTPRIV+21)
#define IEEE80211_IOCTL_FILTERFRAME     (SIOCIWFIRSTPRIV+22)
#define IEEE80211_IOCTL_SET_RTPARAMS    (SIOCIWFIRSTPRIV+23)
#define IEEE80211_IOCTL_SENDADDBA       (SIOCIWFIRSTPRIV+24)
#define IEEE80211_IOCTL_GETADDBASTATUS  (SIOCIWFIRSTPRIV+25)
#define IEEE80211_IOCTL_SENDDELBA       (SIOCIWFIRSTPRIV+26)
#define IEEE80211_IOCTL_GET_IQUECONFIG  (SIOCIWFIRSTPRIV+27)
#define IEEE80211_IOCTL_SET_ADDBARESP   (SIOCIWFIRSTPRIV+28)
#define IEEE80211_IOCTL_GET_MACADDR     (SIOCIWFIRSTPRIV+29)
#define IEEE80211_IOCTL_SET_HBRPARAMS	(SIOCIWFIRSTPRIV+30)


/*
 * Summary statistics.
 */
struct ieee80211_mac_stats {
    u_int64_t   ims_tx_packets; /* frames successfully transmitted */
    u_int64_t   ims_rx_packets; /* frames successfully received */
    u_int64_t   ims_tx_bytes;	/* bytes successfully transmitted */
    u_int64_t	ims_rx_bytes;   /* bytes successfully received */

	/*pengruofeng add retry frame stats 2011-5-11*/
	u_int64_t	ims_rx_retry_packets;
	u_int64_t	ims_rx_retry_bytes;
	u_int64_t	ims_tx_retry_packets;
	u_int64_t	ims_tx_retry_bytes;
	/*pengruo add end 2011-5-11*/
	u_int64_t	ims_rx_retry_mgmt_packets;
	u_int64_t	ims_tx_retry_mgmt_packets;
	u_int64_t	ims_rx_retry_mgmt_bytes;
	u_int64_t	ims_tx_retry_mgmt_bytes;
    /* Decryption errors */
    u_int64_t   ims_rx_unencrypted; /* rx w/o wep and privacy on */
    u_int64_t   ims_rx_badkeyid;    /* rx w/ incorrect keyid */
    u_int64_t   ims_rx_decryptok;   /* rx decrypt okay */
    u_int64_t   ims_rx_decryptcrc;  /* rx decrypt failed on crc */
    u_int64_t   ims_rx_wepfail;     /* rx wep processing failed */
    u_int64_t   ims_rx_tkipreplay;  /* rx seq# violation (TKIP) */
    u_int64_t   ims_rx_tkipformat;  /* rx format bad (TKIP) */
    u_int64_t   ims_rx_tkipmic;     /* rx MIC check failed (TKIP) */
    u_int64_t   ims_rx_tkipicv;     /* rx ICV check failed (TKIP) */
    u_int64_t   ims_rx_ccmpreplay;  /* rx seq# violation (CCMP) */
    u_int64_t   ims_rx_ccmpformat;  /* rx format bad (CCMP) */
    u_int64_t   ims_rx_ccmpmic;     /* rx MIC check failed (CCMP) */
/*this file can be included by applications as 80211stats that has no such MACRO definition*/
//#if ATH_SUPPORT_WAPI 
    u_int64_t   ims_rx_wpireplay;  /* rx seq# violation (WPI) */
    u_int64_t   ims_rx_wpimic;     /* rx MIC check failed (WPI) */
//#endif
    /* Other Tx/Rx errors */
    u_int64_t   ims_tx_discard;     /* tx dropped by NIC */
    u_int64_t   ims_rx_discard;     /* rx dropped by NIC */

    u_int64_t   ims_rx_countermeasure; /* rx TKIP countermeasure activation count */
};



/*Start: Add by zhanghu to statistics of every frame by the rate*/
#ifndef RATE_FRAME_INFO
#define RATE_FRAME_INFO

struct rate_info{
    u_int64_t    count;
    u_int8_t    dot11Rate;
};

struct mac_rate_frame_info{
    struct rate_info    rate[12];
    u_int64_t   mcs[24];
};


struct iw_vap_rate_frame_req{    
    u_int8_t    mac[6];
    struct mac_rate_frame_info vap_rx_data;    
    struct mac_rate_frame_info vap_tx_data;
};
struct rssi_stats{
  u_int64_t   ns_rx_data;
};

struct ni_info {    
    u_int8_t    mac[6];
    struct  mac_rate_frame_info ni_rx_data;
    struct  mac_rate_frame_info ni_tx_data;

#ifndef RSSI_RANGE_NUM
#define RSSI_RANGE_NUM  17
#endif

    struct rssi_stats   ni_rssi_stats[RSSI_RANGE_NUM];
};



struct iw_ni_rate_frame_req{
    u_int32_t   ni_count;

#ifndef NI_COUNT
#define NI_COUNT    128
#endif

    struct ni_info ni_data[128];
};

#endif

/*End: Add by zhanghu to statistics of every frame by the rate*/

/*Start: Add by zhanghu to statistics of every frame by the rate*/
#define SIOCSIWNIRATEFRAME   0x8B3A    /*statistics of every frame by the rate*/    
#define SIOCGIWNIRATEFRAME   0x8B3B
/*End: Add by zhanghu to statistics of every frame by the rate*/


CWBool CWSetStaThroughput(StaThroughputSetValues *staThroughputSetValues);
CWBool CWSetMultiUserPerformance(GetMultiUserPerformanceValues *MultiUserPerformanceValues);
CWBool CWSetMcsMask(GetMcsMaskValues *McsMaskValues);
CWBool CWGetStaPktsStatisticRpt(GetStaPktsStatisticRptValues *getStaPktsStatisticRptValues);
int get_interface_throughput_fields(char *wlanname, wlan_stats_info *wlanStatsInfo);


#endif

