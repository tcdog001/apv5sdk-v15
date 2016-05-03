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
#ifndef AP8X_STATS_H_
#define AP8X_STATS_H_

#include <linux/version.h>
#include <linux/wireless.h>
#include "ap8xLnxIoctlWiress.h"
#include "ap8xLnxIntf.h"
#include "wl_hal.h"


typedef struct {  //peiwenhui add 090205
    unsigned int rx_sum_packets;                        /*sum packets*/
    unsigned int tx_sum_packets;
    unsigned int rx_errors;  //new
    unsigned int tx_errors;  //new
    unsigned long long rx_bytes;                /*data bytes*/
    unsigned long long tx_bytes;
    unsigned int rx_packets;                    /*data packet*/
    unsigned int tx_packets;
    unsigned int ast_rx_crcerr;   //pei add 0220
    unsigned int ast_rx_badcrypt;
    unsigned int ast_rx_badmic;
    unsigned int ast_rx_phyerr;
    unsigned int rx_dropped;
    unsigned int tx_dropped;
    unsigned int rx_multicast;
    unsigned int tx_multicast;
    unsigned int rx_broadcast;
    unsigned int tx_broadcast;
    unsigned int rx_unicast;
    unsigned int tx_unicast;
    unsigned int rx_multicast_bytes;
    unsigned int tx_multicast_bytes;
    unsigned int rx_broadcast_bytes;
    unsigned int tx_broadcast_bytes;
    unsigned int rx_unicast_bytes;
    unsigned int tx_unicast_bytes;
    unsigned int rx_retry_bytes;
    unsigned int tx_retry_bytes;
    unsigned int rx_retry;
    unsigned int tx_retry;  
    unsigned int rx_mgmt;   
    unsigned int tx_mgmt;
    unsigned long long rx_mgmt_bytes;
    unsigned long long tx_mgmt_bytes;
    unsigned long long rx_sum_bytes;
    unsigned long long tx_sum_bytes;
    unsigned long long rx_unicast64_bytes;
    unsigned long long tx_unicast64_bytes;
    unsigned int rx_ctrl;
    unsigned int tx_ctrl;
    unsigned int rx_ctrl_bytes;
    unsigned int tx_ctrl_bytes;
    unsigned int rx_errors_frames;  //new

    unsigned int is_disassoc_unnormal;
    unsigned int is_rx_assoc_norate;         /* rx assoc w/ no rate match */
    unsigned int is_rx_assoc_capmismatch;        /* rx assoc w/ no cap match */
    unsigned int is_rx_bad_assoc;
    unsigned int is_rx_assoc_bss;                       /* rx assoc w/ no bss match */
    unsigned int is_rx_assoc_notauth;               /* rx assoc w/o auth */
    unsigned int is_rx_assoc_badwpaie;              /* rx assoc w/ bad WPA IE */
}wlan_stats_info;

#if 0
struct ieee80211_stats {
    u_int32_t   is_rx_badversion;          /* rx frame with bad version */
    u_int32_t   is_rx_tooshort;            /* rx frame too short */
    u_int32_t   is_rx_wrongbss;            /* rx from wrong bssid */
    u_int32_t   is_rx_wrongdir;            /* rx w/ wrong direction */
    u_int32_t   is_rx_mcastecho;           /* rx discard 'cuz mcast echo */
    u_int32_t   is_rx_notassoc;            /* rx discard 'cuz sta !assoc */
    u_int32_t   is_rx_noprivacy;           /* rx w/ wep but privacy off */
    u_int32_t   is_rx_decap;               /* rx decapsulation failed */
    u_int32_t   is_rx_mgtdiscard;          /* rx discard mgt frames */
    u_int32_t   is_rx_ctl;                 /* rx discard ctrl frames */
    u_int32_t   is_rx_beacon;              /* rx beacon frames */
    u_int32_t   is_rx_rstoobig;            /* rx rate set truncated */
    u_int32_t   is_rx_elem_missing;        /* rx required element missing*/
    u_int32_t   is_rx_elem_toobig;         /* rx element too big */
    u_int32_t   is_rx_elem_toosmall;       /* rx element too small */
    u_int32_t   is_rx_elem_unknown;        /* rx element unknown */
    u_int32_t   is_rx_badchan;             /* rx frame w/ invalid chan */
    u_int32_t   is_rx_chanmismatch;        /* rx frame chan mismatch */
    u_int32_t   is_rx_nodealloc;           /* rx frame dropped */
    u_int32_t   is_rx_ssidmismatch;        /* rx frame ssid mismatch  */
    u_int32_t   is_rx_auth_unsupported;    /* rx w/ unsupported auth alg */
    u_int32_t   is_rx_auth_fail;           /* rx sta auth failure */
    u_int32_t   is_rx_auth_countermeasures;/* rx auth discard 'cuz CM */
    u_int32_t   is_rx_assoc_bss;           /* rx assoc from wrong bssid */
    u_int32_t   is_rx_assoc_notauth;       /* rx assoc w/o auth */
    u_int32_t   is_rx_assoc_capmismatch;   /* rx assoc w/ cap mismatch */
    u_int32_t   is_rx_assoc_norate;        /* rx assoc w/ no rate match */
    u_int32_t   is_rx_assoc_badwpaie;      /* rx assoc w/ bad WPA IE */

    /*zhaoyang1 transplant from 717*/
    /*pengruofeng add start for ath stats 2011-5-9*/
    u_int32_t   is_rx_bad_assoc;  
    u_int32_t   is_assoc_success;
    u_int32_t   is_rx_reassoc;
    u_int32_t   is_rx_reassoc_success;
    u_int32_t   is_rx_bad_reassoc;
    u_int32_t   is_rx_reassoc_bss;  
    u_int32_t   is_rx_reassoc_notauth;  
    u_int32_t   is_rx_reassoc_capmismatch;
    u_int32_t   is_rx_reassoc_norate;   
    u_int32_t   is_rx_reassoc_badwpaie; 
    u_int32_t   is_rx_reassoc_badscie;
    
    u_int32_t   is_disassoc_unnormal;
    u_int32_t   is_disassoc_state;
    u_int32_t   is_disassoc_ioctl;

    u_int32_t   is_deauth_ioctl;
    u_int32_t   is_deauth_unnormal;
    u_int32_t   is_deauth_state;
    u_int32_t   is_deauth_expire;
    /*pengruofeng add end 2011-5-9*/
    /*zhaoyang1 transplant end*/
    
    u_int32_t   is_rx_deauth;              /* rx deauthentication */
    u_int32_t   is_rx_disassoc;            /* rx disassociation */
    u_int32_t   is_rx_action;              /* rx action mgt */
    u_int32_t   is_rx_badsubtype;          /* rx frame w/ unknown subtype*/
    u_int32_t   is_rx_nobuf;               /* rx failed for lack of buf */
    u_int32_t   is_rx_ahdemo_mgt;          /* rx discard ahdemo mgt frame*/
    u_int32_t   is_rx_bad_auth;            /* rx bad auth request */
    u_int32_t   is_rx_unauth;              /* rx on unauthorized port */
    u_int32_t   is_rx_badcipher;           /* rx failed 'cuz key type */
    u_int32_t   is_tx_nodefkey;            /* tx failed 'cuz no defkey */
    u_int32_t   is_tx_noheadroom;          /* tx failed 'cuz no space */
    u_int32_t   is_rx_nocipherctx;         /* rx failed 'cuz key !setup */
    u_int32_t   is_rx_acl;                 /* rx discard 'cuz acl policy */
    u_int32_t   is_rx_ffcnt;               /* rx fast frames */
    u_int32_t   is_rx_badathtnl;           /* driver key alloc failed */
    u_int32_t   is_rx_nowds;               /* 4-addr packets received with no wds enabled */
    u_int32_t   is_tx_nobuf;               /* tx failed for lack of buf */
    u_int32_t   is_tx_nonode;              /* tx failed for no node */
    u_int32_t   is_tx_unknownmgt;          /* tx of unknown mgt frame */
    u_int32_t   is_tx_badcipher;           /* tx failed 'cuz key type */
    u_int32_t   is_tx_ffokcnt;             /* tx fast frames sent success */
    u_int32_t   is_tx_fferrcnt;            /* tx fast frames sent success */
    u_int32_t   is_scan_active;            /* active scans started */
    u_int32_t   is_scan_passive;           /* passive scans started */
    u_int32_t   is_node_timeout;           /* nodes timed out inactivity */
    /*zhaoyang1 transplant from 717*/
    u_int32_t   is_node_timeout_auth;       /*pengruofeng add nodes timed out inactivity at auth state 2011-5-9*/
    u_int32_t   is_node_timeout_assoc;  /* pengruofeng add nodes timed out inactivity at assoc state  2011-5-9*/
    /*zhaoyang1 transplant end*/
    u_int32_t   is_crypto_nomem;           /* no memory for crypto ctx */
    u_int32_t   is_crypto_tkip;            /* tkip crypto done in s/w */
    u_int32_t   is_crypto_tkipenmic;       /* tkip en-MIC done in s/w */
    u_int32_t   is_crypto_tkipdemic;       /* tkip de-MIC done in s/w */
    u_int32_t   is_crypto_tkipcm;          /* tkip counter measures */
    u_int32_t   is_crypto_ccmp;            /* ccmp crypto done in s/w */
    u_int32_t   is_crypto_wep;             /* wep crypto done in s/w */
    u_int32_t   is_crypto_setkey_cipher;   /* cipher rejected key */
    u_int32_t   is_crypto_setkey_nokey;    /* no key index for setkey */
    u_int32_t   is_crypto_delkey;          /* driver key delete failed */
    u_int32_t   is_crypto_badcipher;       /* unknown cipher */
    u_int32_t   is_crypto_nocipher;        /* cipher not available */
    u_int32_t   is_crypto_attachfail;      /* cipher attach failed */
    u_int32_t   is_crypto_swfallback;      /* cipher fallback to s/w */
    u_int32_t   is_crypto_keyfail;         /* driver key alloc failed */
    u_int32_t   is_crypto_enmicfail;       /* en-MIC failed */
    u_int32_t   is_ibss_capmismatch;       /* merge failed-cap mismatch */
    u_int32_t   is_ibss_norate;            /* merge failed-rate mismatch */
    u_int32_t   is_ps_unassoc;             /* ps-poll for unassoc. sta */
    u_int32_t   is_ps_badaid;              /* ps-poll w/ incorrect aid */
    u_int32_t   is_ps_qempty;              /* ps-poll w/ nothing to send */

    /*zhaoyang1 transplant from 717*/
    /*pengruofeng add for ath stats 2011-5-9*/
    u_int32_t   is_rx_mgmt;
    u_int32_t   is_tx_mgmt;
    u_int64_t   is_rx_mgmtbytes;
    u_int64_t   is_tx_mgmtbytes;        
    u_int32_t   is_assocs;
    u_int32_t   is_reassocs;
    u_int32_t   is_rx_repack;       /*终端重传的包数*/
    u_int32_t   is_rx_rebytes;  /*终端重传的字节数*/
    
    u_int32_t   is_rx_err_pack;             /*接收到的来自终端的错误包数*/
    u_int32_t   is_tx_signaling;                /*发送到指定终端的信令包数*/
    u_int32_t   is_rx_signaling;                /*终端传送的信令包数*/
    u_int32_t   is_re_wpi;/*根据重放机制丢弃的WPI MPDU的数目*/
    u_int32_t   is_wpi_mic;  /* WPI-SMS4解密时没有有效的密钥而丢弃的MPDU数目*/
    u_int32_t   is_wpi_no_key_error;/* WPI-SMS4解密时MIC校验出错而丢弃的MPDU数目*/
    u_int32_t   is_rx_auth_try; /*add wyh@20080716 for snmp*/
    u_int32_t   is_rx_auth_ok;  /*add wyh@20080716 for snmp*/
    u_int32_t   is_rx_assoc_try;    /*add wyh@20080716 for snmp*/
    u_int32_t   is_rx_ap_too_many_sta;  /*add wyh@20080716 for snmp*/
    /*pengruofeng add for ath stats*/
    /*Begin:Added by pengruofeng for counting of error packets*/
    u_int32_t is_rx_errors;
    /*End:Added by pengruofeng for counting of error packets*/
    /*Begin:Added by pengruofeng for counting of error frames*/
    u_int32_t is_rx_error_frames;
    /*End:Added by pengruofeng for counting of error frames 2011-5-9*/
    /*zhaoyang1 transplant end*/
     u_int32_t is_refuse_LowRssi;
};
#else
struct ieee80211_stats {
	u_int32_t	is_rx_badversion;	/* rx frame with bad version */
	u_int32_t	is_rx_tooshort;		/* rx frame too short */
	u_int32_t	is_rx_wrongbss;		/* rx from wrong bssid */
	u_int32_t	is_rx_dup;		/* rx discard 'cuz dup */
	u_int32_t	is_rx_wrongdir;		/* rx w/ wrong direction */
	u_int32_t	is_rx_mcastecho;	/* rx discard 'cuz mcast echo */
	u_int32_t	is_rx_notassoc;		/* rx discard 'cuz sta !assoc */
	u_int32_t	is_rx_noprivacy;	/* rx w/ wep but privacy off */
	u_int32_t	is_rx_unencrypted;	/* rx w/o wep and privacy on */
	u_int32_t	is_rx_wepfail;		/* rx wep processing failed */
	u_int32_t	is_rx_decap;		/* rx decapsulation failed */
	u_int32_t	is_rx_mgtdiscard;	/* rx discard mgt frames */
	u_int32_t	is_rx_ctl;		/* rx discard ctrl frames */
	u_int32_t	is_rx_beacon;		/* rx beacon frames */
	u_int32_t	is_rx_rstoobig;		/* rx rate set truncated */
	u_int32_t	is_rx_elem_missing;	/* rx required element missing*/
	u_int32_t	is_rx_elem_toobig;	/* rx element too big */
	u_int32_t	is_rx_elem_toosmall;	/* rx element too small */
	u_int32_t	is_rx_elem_unknown;	/* rx element unknown */
	u_int32_t	is_rx_badchan;		/* rx frame w/ invalid chan */
	u_int32_t	is_rx_chanmismatch;	/* rx frame chan mismatch */
	u_int32_t	is_rx_nodealloc;	/* rx frame dropped */
	u_int32_t	is_rx_ssidmismatch;	/* rx frame ssid mismatch  */
	u_int32_t	is_rx_auth_unsupported;	/* rx w/ unsupported auth alg */
	u_int32_t	is_rx_auth_fail;	/* rx sta auth failure */
	u_int32_t	is_rx_auth_countermeasures;/* rx auth discard 'cuz CM */
	u_int32_t	is_rx_assoc_bss;	/* rx assoc from wrong bssid */
	u_int32_t	is_rx_assoc_notauth;	/* rx assoc w/o auth */
	u_int32_t	is_rx_assoc_capmismatch;/* rx assoc w/ cap mismatch */
	u_int32_t	is_rx_assoc_norate;	/* rx assoc w/ no rate match */
	u_int32_t	is_rx_assoc_badwpaie;	/* rx assoc w/ bad WPA IE */
	/*lyb add start for ath stats*/
	u_int32_t is_rx_bad_assoc;
	u_int32_t is_assoc_success;
	u_int32_t is_rx_reassoc;
	u_int32_t is_rx_reassoc_success;
	u_int32_t is_rx_bad_reassoc;
	u_int32_t is_rx_reassoc_bss;	
	u_int32_t is_rx_reassoc_notauth;	
	u_int32_t is_rx_reassoc_capmismatch;
	u_int32_t is_rx_reassoc_norate;	
	u_int32_t is_rx_reassoc_badwpaie;	
	u_int32_t is_rx_reassoc_badscie;
	
	u_int32_t is_disassoc_unnormal;
	u_int32_t is_disassoc_state;
	u_int32_t is_disassoc_ioctl;

	u_int32_t is_deauth_ioctl;
	u_int32_t is_deauth_unnormal;
	u_int32_t is_deauth_state;
	u_int32_t is_deauth_expire;
	/*lyb add end*/
	u_int32_t	is_rx_deauth;		/* rx deauthentication */
	u_int32_t	is_rx_disassoc;		/* rx disassociation */
	u_int32_t   is_rx_action;       /* rx action mgt */
	u_int32_t	is_rx_badsubtype;	/* rx frame w/ unknown subtype*/
	u_int32_t	is_rx_nobuf;		/* rx failed for lack of buf */
	u_int32_t	is_rx_decryptcrc;	/* rx decrypt failed on crc */
	u_int32_t	is_rx_ahdemo_mgt;	/* rx discard ahdemo mgt frame*/
	u_int32_t	is_rx_bad_auth;		/* rx bad auth request */
	u_int32_t	is_rx_unauth;		/* rx on unauthorized port */
	u_int32_t	is_rx_badkeyid;		/* rx w/ incorrect keyid */
	u_int32_t	is_rx_ccmpreplay;	/* rx seq# violation (CCMP) */
	u_int32_t	is_rx_ccmpformat;	/* rx format bad (CCMP) */
	u_int32_t	is_rx_ccmpmic;		/* rx MIC check failed (CCMP) */
	u_int32_t	is_rx_tkipreplay;	/* rx seq# violation (TKIP) */
	u_int32_t	is_rx_tkipformat;	/* rx format bad (TKIP) */
	u_int32_t	is_rx_tkipmic;		/* rx MIC check failed (TKIP) */
	u_int32_t	is_rx_tkipicv;		/* rx ICV check failed (TKIP) */
	u_int32_t	is_rx_badcipher;	/* rx failed 'cuz key type */
	u_int32_t	is_rx_nocipherctx;	/* rx failed 'cuz key !setup */
	u_int32_t	is_rx_acl;		/* rx discard 'cuz acl policy */
	u_int32_t	is_rx_ffcnt;		/* rx fast frames */
	u_int32_t	is_rx_badathtnl;   	/* driver key alloc failed */
	u_int32_t	is_tx_nobuf;		/* tx failed for lack of buf */
	u_int32_t	is_tx_nonode;		/* tx failed for no node */
	u_int32_t	is_tx_unknownmgt;	/* tx of unknown mgt frame */
	u_int32_t	is_tx_badcipher;	/* tx failed 'cuz key type */
	u_int32_t	is_tx_nodefkey;		/* tx failed 'cuz no defkey */
	u_int32_t	is_tx_noheadroom;	/* tx failed 'cuz no space */
	u_int32_t	is_tx_ffokcnt;		/* tx fast frames sent success */
	u_int32_t	is_tx_fferrcnt;		/* tx fast frames sent success */
	u_int32_t	is_scan_active;		/* active scans started */
	u_int32_t	is_scan_passive;	/* passive scans started */
	u_int32_t	is_node_timeout;	/* nodes timed out inactivity */
	u_int32_t is_node_timeout_auth;		/*lyb add nodes timed out inactivity at auth state*/
	u_int32_t is_node_timeout_assoc;	/* lyb add nodes timed out inactivity at assoc state */
	u_int32_t	is_crypto_nomem;	/* no memory for crypto ctx */
	u_int32_t	is_crypto_tkip;		/* tkip crypto done in s/w */
	u_int32_t	is_crypto_tkipenmic;	/* tkip en-MIC done in s/w */
	u_int32_t	is_crypto_tkipdemic;	/* tkip de-MIC done in s/w */
	u_int32_t	is_crypto_tkipcm;	/* tkip counter measures */
	u_int32_t	is_crypto_ccmp;		/* ccmp crypto done in s/w */
	u_int32_t	is_crypto_wep;		/* wep crypto done in s/w */
	u_int32_t	is_crypto_setkey_cipher;/* cipher rejected key */
	u_int32_t	is_crypto_setkey_nokey;	/* no key index for setkey */
	u_int32_t	is_crypto_delkey;	/* driver key delete failed */
	u_int32_t	is_crypto_badcipher;	/* unknown cipher */
	u_int32_t	is_crypto_nocipher;	/* cipher not available */
	u_int32_t	is_crypto_attachfail;	/* cipher attach failed */
	u_int32_t	is_crypto_swfallback;	/* cipher fallback to s/w */
	u_int32_t	is_crypto_keyfail;	/* driver key alloc failed */
	u_int32_t	is_crypto_enmicfail;	/* en-MIC failed */
	u_int32_t	is_ibss_capmismatch;	/* merge failed-cap mismatch */
	u_int32_t	is_ibss_norate;		/* merge failed-rate mismatch */
	u_int32_t	is_ps_unassoc;		/* ps-poll for unassoc. sta */
	u_int32_t	is_ps_badaid;		/* ps-poll w/ incorrect aid */
	u_int32_t	is_ps_qempty;		/* ps-poll w/ nothing to send */
	/*lyb add for ath stats*/
	u_int32_t is_rx_mgmt;
	u_int32_t is_tx_mgmt;
/* lisongbai add for count rx/tx_mgmtbytes in 2011-01-20 */
	UINT64	is_rx_mgmtbytes;
	UINT64	is_tx_mgmtbytes;        
/*lisongbai add end */
	u_int32_t is_assocs;
	u_int32_t is_reassocs;
	u_int32_t is_rx_repack;		/*终端重传的包数*/
	u_int32_t is_rx_rebytes;	/*终端重传的字节数*/
	u_int32_t is_rx_err_pack;				/*接收到的来自终端的错误包数*/
	u_int32_t is_tx_signaling;				/*发送到指定终端的信令包数*/
	u_int32_t is_rx_signaling;				/*终端传送的信令包数*/
	u_int32_t is_re_wpi;/*根据重放机制丢弃的WPI MPDU的数目*/
	u_int32_t is_wpi_mic;  /* WPI-SMS4解密时没有有效的密钥而丢弃的MPDU数目*/
	u_int32_t is_wpi_no_key_error;/* WPI-SMS4解密时MIC校验出错而丢弃的MPDU数目*/
	u_int32_t	is_rx_auth_try;	/*add wyh@20080716 for snmp*/
	u_int32_t	is_rx_auth_ok;	/*add wyh@20080716 for snmp*/
	u_int32_t	is_rx_assoc_try;	/*add wyh@20080716 for snmp*/
	u_int32_t	is_rx_ap_too_many_sta;	/*add wyh@20080716 for snmp*/
	/*lyb add for ath stats*/
	/*Begin:Added by duanmingzhe for counting of error packets 2010-09-09*/
	u_int32_t is_rx_errors;
	/*End:Added by duanmingzhe for counting of error packets 2010-09-09*/
	/*Begin:Added by duanmingzhe for counting of error frames 2010-09-20*/
	u_int32_t is_rx_error_frames;
	/*End:Added by duanmingzhe for counting of error frames 2010-09-20*/
};

#endif

struct ath_11n_stats {
    u_int32_t   tx_pkts;            /* total tx data packets */
    u_int32_t   tx_checks;          /* tx drops in wrong state */
    u_int32_t   tx_drops;           /* tx drops due to qdepth limit */
    u_int32_t   tx_minqdepth;       /* tx when h/w queue depth is low */
    u_int32_t   tx_queue;           /* tx pkts when h/w queue is busy */
    u_int32_t   tx_resetq;          /* tx reset queue instances */
    u_int32_t   tx_comps;           /* tx completions */
    u_int32_t   tx_comperror;       /* tx error completions on global failures */
    u_int32_t   tx_unaggr_comperror; /* tx error completions of unaggregted frames */
    u_int32_t   tx_stopfiltered;    /* tx pkts filtered for requeueing */
    u_int32_t   tx_qnull;           /* txq empty occurences */
    u_int32_t   tx_noskbs;          /* tx no skbs for encapsulations */
    u_int32_t   tx_nobufs;          /* tx no descriptors */
    u_int32_t   tx_badsetups;       /* tx key setup failures */
    u_int32_t   tx_normnobufs;      /* tx no desc for legacy packets */
    u_int32_t   tx_schednone;       /* tx schedule pkt queue empty */
    u_int32_t   tx_bars;            /* tx bars sent */
    u_int32_t   txbar_xretry;       /* tx bars excessively retried */
    u_int32_t   txbar_compretries;  /* tx bars retried */
    u_int32_t   txbar_errlast;      /* tx bars last frame failed */
    u_int32_t   tx_compunaggr;      /* tx unaggregated frame completions */
    u_int32_t   txunaggr_xretry;    /* tx unaggregated excessive retries */
    u_int32_t   tx_compaggr;        /* tx aggregated completions */
    u_int32_t   tx_bawadv;          /* tx block ack window advanced */
    u_int32_t   tx_bawretries;      /* tx block ack window retries */
    u_int32_t   tx_bawnorm;         /* tx block ack window additions */
    u_int32_t   tx_bawupdates;      /* tx block ack window updates */
    u_int32_t   tx_bawupdtadv;      /* tx block ack window advances */
    u_int32_t   tx_retries;         /* tx retries of sub frames */
    u_int32_t   tx_xretries;        /* tx excessive retries of aggregates */
    u_int32_t   tx_aggregates;      /* tx aggregated pkts sent */
    u_int32_t   tx_sf_hw_xretries;  /* sub-frames excessively retried in h/w */
    u_int32_t   tx_aggr_frames;     /* tx total frames aggregated */
    u_int32_t   txaggr_noskbs;      /* tx no skbs for aggr encapsualtion */
    u_int32_t   txaggr_nobufs;      /* tx no desc for aggr */
    u_int32_t   txaggr_badkeys;     /* tx enc key setup failures */
    u_int32_t   txaggr_schedwindow; /* tx no frame scheduled: baw limited */
    u_int32_t   txaggr_single;      /* tx frames not aggregated */
    u_int32_t   txaggr_mimo;        /* tx frames aggregated for mimo */
    u_int32_t   txaggr_compgood;    /* tx aggr good completions */
    u_int32_t   txaggr_comperror;   /* tx aggr error completions */
    u_int32_t   txaggr_compxretry;  /* tx aggr excessive retries */
    u_int32_t   txaggr_compretries; /* tx aggr unacked subframes */
    u_int32_t   txunaggr_compretries; /* tx non-aggr unacked subframes */
    u_int32_t   txaggr_prepends;    /* tx aggr old frames requeued */
    u_int32_t   txaggr_filtered;    /* filtered aggr packet */
    u_int32_t   txaggr_fifo;        /* fifo underrun of aggregate */
    u_int32_t   txaggr_xtxop;       /* txop exceeded for an aggregate */
    u_int32_t   txaggr_desc_cfgerr; /* aggregate descriptor config error */
    u_int32_t   txaggr_data_urun;   /* data underrun for an aggregate */
    u_int32_t   txaggr_delim_urun;  /* delimiter underrun for an aggregate */
    u_int32_t   txaggr_errlast;     /* tx aggr: last sub-frame failed */
    u_int32_t   txunaggr_errlast;   /* tx non-aggr: last frame failed */
    u_int32_t   txaggr_longretries; /* tx aggr h/w long retries */
    u_int32_t   txaggr_shortretries;/* tx aggr h/w short retries */
    u_int32_t   txaggr_timer_exp;   /* tx aggr : tx timer expired */
    u_int32_t   txaggr_babug;       /* tx aggr : BA bug */
    u_int32_t   txrifs_single;      /* tx frames not bursted */
    u_int32_t   txrifs_babug;       /* tx rifs : BA bug */
    u_int32_t   txrifs_compretries; /* tx rifs unacked subframes */
    u_int32_t   txrifs_bar_alloc;   /* tx rifs bars allocated */
    u_int32_t   txrifs_bar_freed;   /* tx rifs bars freed */
    u_int32_t   txrifs_compgood;    /* tx rifs good completions */
    u_int32_t   txrifs_prepends;    /* tx rifs old frames requeued */
    u_int32_t   tx_comprifs;        /* tx rifs completions */
    u_int32_t   tx_compnorifs;      /* tx not a rifs completion */
    u_int32_t   rx_pkts;            /* rx pkts */
    u_int32_t   rx_aggr;            /* rx aggregated packets */
    u_int32_t   rx_aggrbadver;      /* rx pkts with bad version */
    u_int32_t   rx_bars;            /* rx bars */
    u_int32_t   rx_nonqos;          /* rx non qos-data frames */
    u_int32_t   rx_seqreset;        /* rx sequence resets */
    u_int32_t   rx_oldseq;          /* rx old packets */
    u_int32_t   rx_bareset;         /* rx block ack window reset */
    u_int32_t   rx_baresetpkts;     /* rx pts indicated due to baw resets */
    u_int32_t   rx_dup;             /* rx duplicate pkts */
    u_int32_t   rx_baadvance;       /* rx block ack window advanced */
    u_int32_t   rx_recvcomp;        /* rx pkt completions */
    u_int32_t   rx_bardiscard;      /* rx bar discarded */
    u_int32_t   rx_barcomps;        /* rx pkts unblocked on bar reception */
    u_int32_t   rx_barrecvs;        /* rx pkt completions on bar reception */
    u_int32_t   rx_skipped;         /* rx pkt sequences skipped on timeout */
    u_int32_t   rx_comp_to;         /* rx indications due to timeout */
    u_int32_t   rx_timer_starts;    /* rx countdown timers started */
    u_int32_t   rx_timer_stops;     /* rx countdown timers stopped */ 
    u_int32_t   rx_timer_run;       /* rx timeout occurences */
    u_int32_t   rx_timer_more;      /* rx partial timeout of pending packets */
    u_int32_t   wd_tx_active;       /* watchdog: tx is active */
    u_int32_t   wd_tx_inactive;     /* watchdog: tx is not active */
    u_int32_t   wd_tx_hung;         /* watchdog: tx is hung */
    u_int32_t   wd_spurious;        /* watchdog: spurious tx hang */
    u_int32_t   tx_requeue;         /* filter & requeue on 20/40 transitions */
    u_int32_t   tx_drain_txq;       /* draining tx queue on error */
    u_int32_t   tx_drain_tid;       /* draining tid buf queue on error */
    u_int32_t   tx_cleanup_tid;     /* draining tid buf queue on node cleanup */
    u_int32_t   tx_drain_bufs;      /* buffers drained from pending tid queue */
    u_int32_t   tx_tidpaused;       /* pausing tx on tid */
    u_int32_t   tx_tidresumed;      /* resuming tx on tid */
    u_int32_t   tx_unaggr_filtered; /* unaggregated tx pkts filtered */
    u_int32_t   tx_aggr_filtered;   /* aggregated tx pkts filtered */
    u_int32_t   tx_filtered;        /* total sub-frames filtered */
    u_int32_t   rx_rb_on;           /* total rb-s on  */
    u_int32_t   rx_rb_off;          /* total rb-s off */
    u_int32_t   rx_dsstat_err;      /* rx descriptor status corrupted */
};


struct ath_stats {
    u_int32_t   ast_watchdog;                   /* device reset by watchdog */
    u_int32_t   ast_resetOnError;               /*No of resets on error*/
    u_int32_t   ast_hardware;                   /* fatal hardware error interrupts */
    u_int32_t   ast_bmiss;                      /* beacon miss interrupts */
    u_int32_t   ast_rxorn;                      /* rx overrun interrupts */
    u_int32_t   ast_rxorn_bmiss;                /* rx overrun and bmiss interrupts: indicate descriptor corruption */
    u_int32_t   ast_rxeol;                      /* rx eol interrupts */
    u_int32_t   ast_txurn;                      /* tx underrun interrupts */
    u_int32_t   ast_txto;                       /* tx timeout interrupts */
    u_int32_t   ast_cst;                        /* carrier sense timeout interrupts */
    u_int32_t   ast_mib;                        /* mib interrupts */
    u_int32_t   ast_tx_packets;                 /* packet sent on the interface */
    u_int32_t   ast_rx_packets;                 /* packet received on the interface */
    u_int32_t   ast_tx_mgmt;                    /* management frames transmitted */
    u_int32_t   ast_tx_discard;                 /* frames discarded prior to assoc */
    u_int32_t   ast_tx_invalid;                 /* frames discarded 'cuz device gone */
    u_int32_t   ast_tx_qstop;                   /* tx queue stopped 'cuz full */
    u_int32_t   ast_tx_encap;                   /* tx encapsulation failed */
    u_int32_t   ast_tx_nonode;                  /* tx failed 'cuz no node */
    u_int32_t   ast_tx_nobuf;                   /* tx failed 'cuz no tx buffer (data) */
    u_int32_t   ast_tx_nobufmgt;                /* tx failed 'cuz no tx buffer (mgmt)*/
    u_int32_t   ast_tx_xretries;                /* tx failed 'cuz too many retries */
    u_int32_t   ast_tx_fifoerr;                 /* tx failed 'cuz FIFO underrun */
    u_int32_t   ast_tx_filtered;                /* tx failed 'cuz xmit filtered */
    u_int32_t   ast_tx_badrate;                 /* tx failed 'cuz bogus xmit rate */
    u_int32_t   ast_tx_noack;                   /* tx frames with no ack marked */
    u_int32_t   ast_tx_cts;                     /* tx frames with cts enabled */
    u_int32_t   ast_tx_shortpre;                /* tx frames with short preamble */
    u_int32_t   ast_tx_altrate;                 /* tx frames with alternate rate */
    u_int32_t   ast_tx_protect;                 /* tx frames with protection */
    u_int32_t   ast_rx_orn;                     /* rx failed 'cuz of desc overrun */
    u_int32_t   ast_rx_badcrypt;                /* rx failed 'cuz decryption */
    u_int32_t   ast_rx_badmic;                  /* rx failed 'cuz MIC failure */
    u_int32_t   ast_rx_nobuf;                   /* rx setup failed 'cuz no skbuff */
    u_int32_t   ast_rx_hal_in_progress;
    u_int32_t   ast_rx_num_data;
    u_int32_t   ast_rx_num_mgmt;
    u_int32_t   ast_rx_num_ctl;
    u_int32_t   ast_rx_num_unknown;
    u_int32_t   ast_max_pkts_per_intr;
#define ATH_STATS_MAX_INTR_BKT  512
    u_int32_t   ast_pkts_per_intr[ATH_STATS_MAX_INTR_BKT+1]; /* counter bucket of packets handled in a single iteration */
    int8_t      ast_tx_rssi;                    /* tx rssi of last ack */
    int8_t      ast_tx_rssi_ctl0;               /* tx rssi of last ack [ctl, chain 0] */
    int8_t      ast_tx_rssi_ctl1;               /* tx rssi of last ack [ctl, chain 1] */
    int8_t      ast_tx_rssi_ctl2;               /* tx rssi of last ack [ctl, chain 2] */
    int8_t      ast_tx_rssi_ext0;               /* tx rssi of last ack [ext, chain 0] */
    int8_t      ast_tx_rssi_ext1;               /* tx rssi of last ack [ext, chain 1] */
    int8_t      ast_tx_rssi_ext2;               /* tx rssi of last ack [ext, chain 2] */
    int8_t      ast_rx_rssi;                    /* rx rssi from histogram [combined]*/
    int8_t      ast_rx_rssi_ctl0;               /* rx rssi from histogram [ctl, chain 0] */
    int8_t      ast_rx_rssi_ctl1;               /* rx rssi from histogram [ctl, chain 1] */
    int8_t      ast_rx_rssi_ctl2;               /* rx rssi from histogram [ctl, chain 2] */
    int8_t      ast_rx_rssi_ext0;               /* rx rssi from histogram [ext, chain 0] */
    int8_t      ast_rx_rssi_ext1;               /* rx rssi from histogram [ext, chain 1] */
    int8_t      ast_rx_rssi_ext2;               /* rx rssi from histogram [ext, chain 2] */
    u_int32_t   ast_be_xmit;                    /* beacons transmitted */
    u_int32_t   ast_be_nobuf;                   /* no skbuff available for beacon */
    u_int32_t   ast_per_cal;                    /* periodic calibration calls */
    u_int32_t   ast_per_calfail;                /* periodic calibration failed */
    u_int32_t   ast_per_rfgain;                 /* periodic calibration rfgain reset */
    u_int32_t   ast_rate_calls;                 /* rate control checks */
    u_int32_t   ast_rate_raise;                 /* rate control raised xmit rate */
    u_int32_t   ast_rate_drop;                  /* rate control dropped xmit rate */
    u_int32_t   ast_ant_defswitch;              /* rx/default antenna switches */
    u_int32_t   ast_ant_txswitch;               /* tx antenna switches */
    /*lyb add for wifi stats*/
    u_int32_t   ast_up_times;   /* wifi up times.*/
    u_int32_t   ast_down_times; /* wifi down times.*/
    u_int32_t ast_rx_phyerr;    /* rx PHY error summary count */
    u_int32_t ast_rx_crcerr;    /* rx failed 'cuz of bad CRC */
    u_int32_t  ast_rx_mgt;
    /*lyb add end*/
    u_int32_t   ast_ant_rx[8];                  /* rx frames with antenna */
    u_int32_t   ast_ant_tx[8];                  /* tx frames with antenna */
    UINT64 ast_rx_bytes;                    /* total number of bytes received */
    UINT64   ast_tx_bytes;                  /* total number of bytes transmitted */
    u_int32_t   ast_rx_num_qos_data[16];        /* per tid rx packets (includes duplicates)*/
    u_int32_t   ast_rx_num_nonqos_data;         /* non qos rx packets    */
    u_int32_t   ast_txq_packets[16];            /* perq packets sent on the interface for each category */
    u_int32_t   ast_txq_xretries[16];           /* per q tx failed 'cuz too many retries */
    u_int32_t   ast_txq_fifoerr[16];            /* per q tx failed 'cuz FIFO underrun */
    u_int32_t   ast_txq_filtered[16];           /*per q  tx failed 'cuz xmit filtered */
    u_int32_t   ast_txq_athbuf_limit[16];       /* tx dropped 'cuz of athbuf limit */
    u_int32_t   ast_txq_nobuf[16];              /* tx dropped 'cuz no athbufs */
    struct ath_11n_stats ast_11n_stats;         /* 11n statistics */
    u_int32_t   ast_bb_hang;                    /* BB hang detected */
    u_int32_t   ast_mac_hang;                   /* MAC hang detected */
    u_int32_t   ast_uapsdqnulbf_unavail;        /* no qos null buffers available */
    u_int32_t   ast_uapsdqnul_pkts;             /* count of qos null frames sent */
    u_int32_t   ast_uapsdtriggers;              /* count of UAPSD triggers received */
    u_int32_t   ast_uapsdnodeinvalid;           /* count of triggers for non-UAPSD node */
    u_int32_t   ast_uapsdeospdata;              /* count of QoS Data with EOSP sent */
    u_int32_t   ast_uapsddata_pkts;             /* count of UAPSD QoS Data frames sent */
    u_int32_t   ast_uapsddatacomp;              /* count of UAPSD QoS Data frames completed */
    u_int32_t   ast_uapsdqnulcomp;              /* count of UAPSD QoS NULL frames completed */
    u_int32_t   ast_uapsddataqueued;            /* count of UAPSD QoS Data Queued */
};

struct HWstats{
    
    u_int32_t RxWAPIMICErrors;
    u_int32_t RxWAPIPNErrors;
    u_int32_t WAPINoKeyErrors;
    u_int32_t RxErrors;
};

int ieee80211_ioctl_getathstats(struct net_device *netdev,char *ret_str, UINT16 *ret_len);
int ieee80211_ioctl_get_ieee80211_stats(struct net_device *netdev,char *ret_str, UINT16 *ret_len);
int Statistics_Recv_Data_Packet(struct net_device *netdev,UINT8 *addr,UINT32 len);
void Statistics_Recv_Error_packet(struct net_device *dev);
int Statistics_Recv_Frame_Errors(struct net_device *netdev);
int Statistics_Recv_Packet_Retry(vmacApInfo_t *vmacSta_p,UINT32 len,UINT8 type);
int Statistics_Recv_Mgmt_Packet(vmacApInfo_t *vmacSta_p,UINT32 len);
int Statistics_Recv_Ctl_Packet(struct net_device *netdev,UINT32 len);
int Statistics_TX_Data_Packet(struct net_device *netdev,UINT8 bcast,UINT32 len,UINT8 count);
int Statistics_TX_Mgmt_Packet(struct net_device *netdev,UINT8 bcast,UINT32 len);
int Statistics_TX_Error_Packet(struct net_device *netdev,UINT8 count);
int Statistics_TX_Drop_Packet(struct net_device *netdev);
int Statistics_Disassoc_Errors(struct net_device *netdev,UINT16 flag);

#endif
