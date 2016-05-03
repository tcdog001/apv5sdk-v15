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

//add by lirui for sta_stats begin

#ifndef AP8X_STAT_STATS_H_
#define AP8X_STAT_STATS_H_
#include <linux/version.h>
#include <linux/wireless.h>
#include "wllog.h"

#define IEEE80211_ADDR_LEN 6
#define IEEE80211_RATE_MAXSIZE  36

struct rssi_stats{
    u_int64_t   ns_rx_data;
};


/* HT capability flags */
#define IEEE80211_HTCAP_C_ADVCODING             0x0001
#define IEEE80211_HTCAP_C_CHWIDTH40             0x0002  
#define IEEE80211_HTCAP_C_SMPOWERSAVE_STATIC    0x0000 /* Capable of SM Power Save (Static) */
#define IEEE80211_HTCAP_C_SMPOWERSAVE_DYNAMIC   0x0004 /* Capable of SM Power Save (Dynamic) */
#define IEEE80211_HTCAP_C_SM_RESERVED           0x0008 /* Reserved */
#define IEEE80211_HTCAP_C_SM_ENABLED            0x000c /* SM enabled, no SM Power Save */
#define IEEE80211_HTCAP_C_GREENFIELD            0x0010
#define IEEE80211_HTCAP_C_SHORTGI20             0x0020
#define IEEE80211_HTCAP_C_SHORTGI40             0x0040
#define IEEE80211_HTCAP_C_TXSTBC                0x0080
#define IEEE80211_HTCAP_C_RXSTBC                0x0300  /* 2 bits */
#define IEEE80211_HTCAP_C_RXSTBC_S                   8
#define IEEE80211_HTCAP_C_DELAYEDBLKACK         0x0400
#define IEEE80211_HTCAP_C_MAXAMSDUSIZE          0x0800  /* 1 = 8K, 0 = 3839B */
#define IEEE80211_HTCAP_C_DSSSCCK40             0x1000  
#define IEEE80211_HTCAP_C_PSMP                  0x2000  
#define IEEE80211_HTCAP_C_INTOLERANT40          0x4000  
#define IEEE80211_HTCAP_C_LSIGTXOPPROT          0x8000  

#define IEEE80211_HTCAP_C_SM_MASK               0x000c /* Spatial Multiplexing (SM) capabitlity bitmask */

/*yuanding add

 * Per/node (station) statistics available when operating as an AP.
 */
 typedef unsigned long long u_int64_c;

#if (defined(MODE11N)||defined(AR11N))
struct ieee80211_nodestats {
	u_int32_t	ns_rx_data;		/* rx data frames */
	u_int32_t	ns_rx_mgmt;		/* rx management frames */
	u_int32_t	ns_rx_ctrl;		/* rx control frames */
	u_int32_t	ns_rx_ucast;		/* rx unicast frames */
	u_int32_t	ns_rx_mcast;		/* rx multi/broadcast frames */
	u_int64_c	ns_rx_bytes;		/* rx data count (bytes) */
	u_int64_c	ns_rx_beacons;		/* rx beacon frames */
	u_int32_t	ns_rx_proberesp;	/* rx probe response frames */

	u_int32_t	ns_rx_dup;		/* rx discard 'cuz dup */
	u_int32_t	ns_rx_noprivacy;	/* rx w/ wep but privacy off */
	u_int32_t	ns_rx_wepfail;		/* rx wep processing failed */
	u_int32_t	ns_rx_demicfail;	/* rx demic failed */
	u_int32_t	ns_rx_decap;		/* rx decapsulation failed */
	u_int32_t	ns_rx_defrag;		/* rx defragmentation failed */
	u_int32_t	ns_rx_disassoc;		/* rx disassociation */
	u_int32_t	ns_rx_deauth;		/* rx deauthentication */
	u_int32_t   ns_rx_action;       /* rx action */
	u_int32_t	ns_rx_decryptcrc;	/* rx decrypt failed on crc */
	u_int32_t	ns_rx_unauth;		/* rx on unauthorized port */
	u_int32_t	ns_rx_unencrypted;	/* rx unecrypted w/ privacy */

	u_int32_t	ns_tx_data;		/* tx data frames */
	u_int32_t	ns_tx_mgmt;		/* tx management frames */
	u_int32_t	ns_tx_ucast;		/* tx unicast frames */
	u_int32_t	ns_tx_mcast;		/* tx multi/broadcast frames */
	u_int64_c	ns_tx_bytes;		/* tx data count (bytes) */
	u_int32_t	ns_tx_probereq;		/* tx probe request frames */
	u_int32_t	ns_tx_uapsd;		/* tx on uapsd queue */

	u_int32_t	ns_tx_novlantag;	/* tx discard 'cuz no tag */
	u_int32_t	ns_tx_vlanmismatch;	/* tx discard 'cuz bad tag */
	u_int32_t	ns_tx_dropblock;	/* tx discard 'cuz headline block */

	u_int32_t	ns_tx_eosplost;		/* uapsd EOSP retried out */

	u_int32_t	ns_ps_discard;		/* ps discard 'cuz of age */

	u_int32_t	ns_uapsd_triggers;	     /* uapsd triggers */
	u_int32_t	ns_uapsd_duptriggers;	 /* uapsd duplicate triggers */
	u_int32_t	ns_uapsd_ignoretriggers; /* uapsd duplicate triggers */
	u_int32_t	ns_uapsd_active;         /* uapsd duplicate triggers */
	u_int32_t	ns_uapsd_triggerenabled; /* uapsd duplicate triggers */

	/* MIB-related state */
	u_int32_t	ns_tx_assoc;		/* [re]associations */
	u_int32_t	ns_tx_assoc_fail;	/* [re]association failures */
	u_int32_t	ns_tx_auth;		/* [re]authentications */
	u_int32_t	ns_tx_auth_fail;	/* [re]authentication failures*/
	u_int32_t	ns_tx_deauth;		/* deauthentications */
	u_int32_t	ns_tx_deauth_code;	/* last deauth reason */
	u_int32_t	ns_tx_disassoc;		/* disassociations */
	u_int32_t	ns_tx_disassoc_code;	/* last disassociation reason */
	u_int32_t	ns_psq_drops;		/* power save queue drops */
	u_int32_t ns_tx_ctrl;                /* tx control frames */
};
#else
struct ieee80211_nodestats {
	u_int32_t ns_rx_data;		/* rx data frames */
	u_int32_t ns_rx_mgmt;		/* rx management frames */
	u_int32_t ns_rx_ctrl;		/* rx control frames */
	u_int32_t ns_rx_ucast;		/* rx unicast frames */
	u_int32_t ns_rx_mcast;		/* rx multi/broadcast frames */
	u_int64_c ns_rx_bytes;		/* rx data count (bytes) */
	u_int64_c ns_rx_beacons;		/* rx beacon frames */
	u_int32_t ns_rx_proberesp;	/* rx probe response frames */

	u_int32_t ns_rx_dup;		/* rx discard because it's a dup */
	u_int32_t ns_rx_noprivacy;	/* rx w/ wep but privacy off */
	u_int32_t ns_rx_wepfail;		/* rx wep processing failed */
	u_int32_t ns_rx_demicfail;	/* rx demic failed */
	u_int32_t ns_rx_decap;		/* rx decapsulation failed */
	u_int32_t ns_rx_defrag;		/* rx defragmentation failed */
	u_int32_t ns_rx_disassoc;	/* rx disassociation */
	u_int32_t ns_rx_deauth;		/* rx deauthentication */
	u_int32_t ns_rx_decryptcrc;	/* rx decrypt failed on crc */
	u_int32_t ns_rx_unauth;		/* rx on unauthorized port */
	u_int32_t ns_rx_unencrypted;	/* rx unecrypted w/ privacy */

	u_int32_t ns_tx_data;		/* tx data frames */
	u_int32_t ns_tx_mgmt;		/* tx management frames */
	u_int32_t ns_tx_ctrl; /* rx control frames added by duanmingzhe txcount of ctrl fream 2010-09-07*/
	u_int32_t ns_tx_ucast;		/* tx unicast frames */
	u_int32_t ns_tx_mcast;		/* tx multi/broadcast frames */
	u_int64_c ns_tx_bytes;		/* tx data count (bytes) */
	u_int32_t ns_tx_probereq;	/* tx probe request frames */
	u_int32_t ns_tx_uapsd;		/* tx on uapsd queue */

	u_int32_t ns_tx_novlantag;	/* tx discard due to no tag */
	u_int32_t ns_tx_vlanmismatch;	/* tx discard due to of bad tag */

	u_int32_t ns_tx_eosplost;	/* uapsd EOSP retried out */

	u_int32_t ns_ps_discard;		/* ps discard due to of age */

	u_int32_t ns_uapsd_triggers;	/* uapsd triggers */

	/* MIB-related state */
	u_int32_t ns_tx_assoc;		/* [re]associations */
	u_int32_t ns_tx_assoc_fail;	/* [re]association failures */
	u_int32_t ns_tx_auth;		/* [re]authentications */
	u_int32_t ns_tx_auth_fail;	/* [re]authentication failures*/
	u_int32_t ns_tx_deauth;		/* deauthentications */
	u_int32_t ns_tx_deauth_code;	/* last deauth reason */
	u_int32_t ns_tx_disassoc;	/* disassociations */
	u_int32_t ns_tx_disassoc_code;	/* last disassociation reason */
	u_int32_t ns_psq_drops;		/* power save queue drops */
	/*lyb add start for wpi stats*/
	u_int32_t ns_re_wpi;/*???WPI MPDU?*/
	u_int32_t ns_wpi_mic;  /* WPI-SMS4??Ч?MPDU?*/
	u_int32_t ns_wpi_no_key_error;/* WPI-SMS4?MICУMPDU?*/
	/*lyb add end*/
};
#endif
/*yuanding add end*/

struct ieee80211_stastats {
    u_int32_t    ns_rx_data;             /* rx data frames */
    u_int32_t    ns_rx_mgmt;             /* rx management frames */
    u_int32_t    ns_rx_ctrl;             /* rx control frames */
    u_int32_t    ns_rx_ucast;            /* rx unicast frames */
    u_int32_t    ns_rx_mcast;            /* rx multi/broadcast frames */
    u_int64_t    ns_rx_bytes;            /* rx data count (bytes) */
    u_int64_t    ns_rx_beacons;          /* rx beacon frames */
    u_int32_t    ns_rx_proberesp;        /* rx probe response frames */

    u_int32_t    ns_rx_dup;              /* rx discard 'cuz dup */
    u_int32_t    ns_rx_noprivacy;        /* rx w/ wep but privacy off */
    u_int32_t    ns_rx_wepfail;          /* rx wep processing failed */
    u_int32_t    ns_rx_demicfail;        /* rx demic failed */
    u_int32_t    ns_rx_decap;            /* rx decapsulation failed */
    u_int32_t    ns_rx_defrag;           /* rx defragmentation failed */
    u_int32_t    ns_rx_disassoc;         /* rx disassociation */
    u_int32_t    ns_rx_deauth;           /* rx deauthentication */
    u_int32_t    ns_rx_action;           /* rx action */
    u_int32_t    ns_rx_decryptcrc;       /* rx decrypt failed on crc */
    u_int32_t    ns_rx_unauth;           /* rx on unauthorized port */
    u_int32_t    ns_rx_unencrypted;      /* rx unecrypted w/ privacy */

    u_int32_t    ns_tx_data;             /* tx data frames */
    u_int32_t    ns_tx_mgmt;             /* tx management frames */
    u_int32_t    ns_tx_ucast;            /* tx unicast frames */
    u_int32_t    ns_tx_mcast;            /* tx multi/broadcast frames */
    u_int64_t    ns_tx_bytes;            /* tx data count (bytes) */
    u_int32_t    ns_tx_probereq;         /* tx probe request frames */
    u_int32_t    ns_tx_uapsd;            /* tx on uapsd queue */

    u_int32_t    ns_tx_novlantag;        /* tx discard 'cuz no tag */
    u_int32_t    ns_tx_vlanmismatch;     /* tx discard 'cuz bad tag */

    u_int32_t    ns_tx_eosplost;         /* uapsd EOSP retried out */

    u_int32_t    ns_ps_discard;          /* ps discard 'cuz of age */

    u_int32_t    ns_uapsd_triggers;      /* uapsd triggers */
    u_int32_t    ns_uapsd_duptriggers;    /* uapsd duplicate triggers */
    u_int32_t    ns_uapsd_ignoretriggers; /* uapsd duplicate triggers */
    u_int32_t    ns_uapsd_active;         /* uapsd duplicate triggers */
    u_int32_t    ns_uapsd_triggerenabled; /* uapsd duplicate triggers */


    /* MIB-related state */
    u_int32_t    ns_tx_assoc;            /* [re]associations */
    u_int32_t    ns_tx_assoc_fail;       /* [re]association failures */
    u_int32_t    ns_tx_auth;             /* [re]authentications */
    u_int32_t    ns_tx_auth_fail;        /* [re]authentication failures*/
    u_int32_t    ns_tx_deauth;           /* deauthentications */
    u_int32_t    ns_tx_deauth_code;      /* last deauth reason */
    u_int32_t    ns_tx_disassoc;         /* disassociations */
    u_int32_t    ns_tx_disassoc_code;    /* last disassociation reason */
    u_int32_t    ns_psq_drops;           /* power save queue drops */
    
    /* IQUE-HBR related state */
/*zhaoyang1 transplant from 717*/
    /*zhaoyang modify for ctrl frame count*/
//#ifdef ATH_SUPPORT_IQUE
    u_int32_t   ns_tx_dropblock;    /* tx discard 'cuz headline block */
//#endif
    u_int32_t   ns_tx_ctrl;             /* tx control frames */
    /*zhaoyang modify end*/
    /*zhaoyang modify for wapi error frame count*/
    u_int32_t ns_re_wpi;/*根据重放机制丢弃的WPI MPDU的数目*/
    u_int32_t ns_wpi_mic;  /* WPI-SMS4解密时没有有效的密钥而丢弃的MPDU数目*/
    u_int32_t ns_wpi_no_key_error;/* WPI-SMS4解密时MIC校验出错而丢弃的MPDU数目*/
    /*zhaoyang modify end*/

    /*zhaoyang modify for ns retry count PCAPVXN-136*/
    u_int32_t ns_tx_retry_packets;
    u_int32_t ns_tx_retry_bytes;
    u_int32_t ns_rx_retry_packets;
    u_int32_t ns_rx_retry_bytes;
    /*zhaoyang modify end*/
    /*suzhaoyu add for customer online-traffic limit*/
    u_int64_t    ns_rx_bytes_l;            /*  rx data count (bytes) of last time */
    u_int64_t    ns_sum_bytes;            /*  rx and tx data sum (bytes) of last time */
    /*suzhaoyu addend*/

    /*zhaoyang modify for frag count*/
    u_int32_t ns_tx_frag;
    u_int32_t ns_rx_frag;
    /*zhaoyang modify end*/
    /*suzhaoyu add for customer online-traffic limit*/
    u_long ns_time_remain;            /*  rx data count (bytes) of last time */
    /*suzhaoyu addend*/

    /*Begin:pengdecai @ record rssi states @20120427*/
#ifndef RSSI_RANGE_NUM
    #define RSSI_RANGE_NUM  17
#endif
    struct rssi_stats ns_rssi_stats[RSSI_RANGE_NUM];
    /*End:pengdecai @ record rssi states @20120427*/
};

struct ieee80211req_sta_stats {
    union {
        /* NB: explicitly force 64-bit alignment */
        u_int8_t    macaddr[IEEE80211_ADDR_LEN];
        u_int64_t   pad;
    } is_u;
//yuanding changed
//  struct ieee80211_stastats sta_stats;
  	struct ieee80211_nodestats is_stats;	
};

struct ieee80211req_sta_info {
    u_int16_t   isi_len;        /* length (mult of 4) */
    u_int16_t   isi_freq;       /* MHz */
    u_int32_t   isi_flags;      /* channel flags */
    u_int16_t   isi_state;      /* state flags */
    u_int8_t    isi_authmode;       /* authentication algorithm */
    int8_t          isi_rssi;
    u_int16_t   isi_capinfo;        /* capabilities */
    u_int8_t    isi_athflags;       /* Atheros capabilities */
    u_int8_t    isi_erp;        /* ERP element */
    u_int8_t    isi_macaddr[IEEE80211_ADDR_LEN];
    u_int8_t    isi_nrates;
                        /* negotiated rates */
    u_int8_t    isi_rates[IEEE80211_RATE_MAXSIZE];
    u_int8_t    isi_txrate;     /* index to isi_rates[] */
    u_int32_t   isi_txratekbps;         /* tx rate in kbps */
    u_int32_t   isi_rxratekbps;         /* rx rate in kbps */
    u_int16_t   isi_ie_len;     /* IE length */
    u_int16_t   isi_associd;        /* assoc response */
    u_int16_t   isi_txpower;        /* current tx power */
    u_int16_t   isi_vlan;       /* vlan tag */
    u_int16_t   isi_txseqs[17];     /* seq to be transmitted */
    u_int16_t   isi_rxseqs[17];     /* seq previous for qos frames*/
    u_int16_t   isi_inact;      /* inactivity timer */
    u_int8_t    isi_uapsd;      /* UAPSD queues */
    u_int8_t    isi_opmode;     /* sta operating mode */
    u_int8_t    isi_cipher;
    u_int32_t   isi_assoc_time; /* sta association time */

    u_int16_t   isi_htcap;      /* HT capabilities */
    /* XXX frag state? */
    /* variable length IE data */
};

int ieee80211_ioctl_getstastats(struct net_device *dev, char *param, char *ret_str, UINT16 *ret_len);

int ieee80211_ioctl_getstainfo(struct net_device *dev, char *param, char *ret_str, UINT16 *ret_len);
#endif
//add by lirui for stastats end

