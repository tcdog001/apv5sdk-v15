#ifndef _WTPD_MIB_H_
#define _WTPD_MIB_H_

typedef unsigned char		u_int8_t;
typedef unsigned short		u_int16_t;
typedef unsigned int 	u_int32_t;
typedef signed char 	int8_t;
typedef unsigned long long u_int64_c;

#define NUM_OF_ATH_ALL_WIFI	16
#define NUM_OF_ETH			2
#define _PATH_PROCNET_DEV               "/proc/net/dev"

#define	SIOCGATHSTATS	(SIOCDEVPRIVATE+0)
#define	IEEE80211_ADDR_LEN	6
#define	IEEE80211_RATE_VAL		0x7f
/*******************ieee80211_node.h**************************************/
/* pei add for gather station info at 090417 */
#define	IEEE80211_NODE_AUTH	0x0001		/* authorized for data */
#define	IEEE80211_NODE_QOS	0x0002		/* QoS enabled */
#define	IEEE80211_NODE_ERP	0x0004		/* ERP enabled */
/* NB: this must have the same value as IEEE80211_FC1_PWR_MGT */
#define	IEEE80211_NODE_PWR_MGT	0x0010		/* power save mode enabled */
#define	IEEE80211_NODE_AREF	0x0020		/* authentication ref held */
#define IEEE80211_NODE_UAPSD	0x0040		/* U-APSD power save enabled */
#define IEEE80211_NODE_UAPSD_TRIG 0x0080	/* U-APSD triggerable state */
#define IEEE80211_NODE_UAPSD_SP	0x0100		/* U-APSD SP in progress */
//#endif


/*******************ieee80211_ioctl.h**************************************/
#define	SIOCG80211STATS				(SIOCDEVPRIVATE+2)
#define	IEEE80211_IOCTL_GETKEY		(SIOCDEVPRIVATE+3)
#define	IEEE80211_IOCTL_GETWPAIE		(SIOCDEVPRIVATE+4)
#define	IEEE80211_IOCTL_STA_STATS	(SIOCDEVPRIVATE+5)
#define	IEEE80211_IOCTL_STA_INFO		(SIOCDEVPRIVATE+6)
#define	SIOC80211IFCREATE				(SIOCDEVPRIVATE+7)
#define	SIOC80211IFDESTROY			(SIOCDEVPRIVATE+8)
#define	IEEE80211_IOCTL_SCAN_RESULTS	(SIOCDEVPRIVATE+9)



#if (defined(MODE11N)||defined(AR11N))
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
    u_int32_t   ast_be_xmit;	                /* beacons transmitted */
    u_int32_t   ast_be_nobuf;	                /* no skbuff available for beacon */
    u_int32_t   ast_per_cal;	                /* periodic calibration calls */
    u_int32_t   ast_per_calfail;                /* periodic calibration failed */
    u_int32_t   ast_per_rfgain;	                /* periodic calibration rfgain reset */
    u_int32_t   ast_rate_calls;	                /* rate control checks */
    u_int32_t   ast_rate_raise;	                /* rate control raised xmit rate */
    u_int32_t   ast_rate_drop;	                /* rate control dropped xmit rate */
    u_int32_t   ast_ant_defswitch;              /* rx/default antenna switches */
    u_int32_t   ast_ant_txswitch;               /* tx antenna switches */
	/*lyb add for wifi stats*/
	u_int32_t	ast_up_times;	/* wifi up times.*/
 	u_int32_t	ast_down_times;	/* wifi down times.*/
	u_int32_t ast_rx_phyerr;	/* rx PHY error summary count */
	u_int32_t ast_rx_crcerr;	/* rx failed 'cuz of bad CRC */
	u_int32_t  ast_rx_mgt;
	/*lyb add end*/
    u_int32_t   ast_ant_rx[8];	                /* rx frames with antenna */
    u_int32_t   ast_ant_tx[8];	                /* tx frames with antenna */
    u_int64_c   ast_rx_bytes;	                /* total number of bytes received */
    u_int64_c   ast_tx_bytes;	                /* total number of bytes transmitted */
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
#else
struct ath_stats {
	u_int32_t	ast_watchdog;	/* device reset by watchdog */
	u_int32_t	ast_hardware;	/* fatal hardware error interrupts */
	u_int32_t	ast_bmiss;	/* beacon miss interrupts */
	u_int32_t	ast_rxorn;	/* rx overrun interrupts */
	u_int32_t	ast_rxeol;	/* rx eol interrupts */
	u_int32_t	ast_txurn;	/* tx underrun interrupts */
	u_int32_t	ast_mib;	/* mib interrupts */
	/*ljy--add begin to solve no txbuf can use*/
	u_int32_t	ast_swba;
	u_int32_t	ast_rx;
	u_int32_t	ast_tx;
	u_int32_t	ast_txbuf_take;
	u_int32_t	ast_txbuf_return;
	u_int32_t	ast_tx_madwifi_bug;
	/*ljy--add end*/
	/*ljy--add begin to analyze tx queue*/
	u_int32_t	ast_txq_take[10];
	u_int32_t	ast_txq_return[10];
	/*ljy--add end*/
	u_int32_t	ast_tx_packets;	/* packet sent on the interface */
	u_int32_t	ast_tx_mgmt;	/* management frames transmitted */
	u_int32_t	ast_tx_discard;	/* frames discarded prior to assoc */
	u_int32_t	ast_tx_invalid;	/* frames discarded 'cuz device gone */
	u_int32_t	ast_tx_qstop;	/* tx queue stopped 'cuz full */
	u_int32_t	ast_tx_encap;	/* tx encapsulation failed */
	u_int32_t	ast_tx_nonode;	/* tx failed 'cuz no node */
	u_int32_t	ast_tx_nobuf;	/* tx failed 'cuz no tx buffer (data) */
	u_int32_t	ast_tx_nobufmgt;/* tx failed 'cuz no tx buffer (mgmt)*/
	u_int32_t	ast_tx_xretries;/* tx failed 'cuz too many retries */
	u_int32_t	ast_tx_fifoerr;	/* tx failed 'cuz FIFO underrun */
	u_int32_t	ast_tx_filtered;/* tx failed 'cuz xmit filtered */
	u_int32_t	ast_tx_shortretry;/* tx on-chip retries (short) */
	u_int32_t	ast_tx_longretry;/* tx on-chip retries (long) */
	u_int32_t	ast_tx_badrate;	/* tx failed 'cuz bogus xmit rate */
	u_int32_t	ast_tx_noack;	/* tx frames with no ack marked */
	u_int32_t	ast_tx_rts;	/* tx frames with rts enabled */
	u_int32_t	ast_tx_cts;	/* tx frames with cts enabled */
	u_int32_t	ast_tx_shortpre;/* tx frames with short preamble */
	u_int32_t	ast_tx_altrate;	/* tx frames with alternate rate */
	u_int32_t	ast_tx_protect;	/* tx frames with protection */
	u_int32_t	ast_rx_orn;	/* rx failed 'cuz of desc overrun */
	u_int32_t	ast_rx_crcerr;	/* rx failed 'cuz of bad CRC */
	u_int32_t	ast_rx_fifoerr;	/* rx failed 'cuz of FIFO overrun */
	u_int32_t	ast_rx_badcrypt;/* rx failed 'cuz decryption */
	u_int32_t	ast_rx_badmic;	/* rx failed 'cuz MIC failure */
	u_int32_t	ast_rx_phyerr;	/* rx PHY error summary count */
	u_int32_t	ast_rx_phy[32];	/* rx PHY error per-code counts */
	u_int32_t	ast_rx_tooshort;/* rx discarded 'cuz frame too short */
	u_int32_t	ast_rx_toobig;	/* rx discarded 'cuz frame too large */
	u_int32_t	ast_rx_nobuf;	/* rx setup failed 'cuz no skbuff */
	u_int32_t	ast_rx_packets;	/* packet recv on the interface */
	u_int32_t	ast_rx_mgt;	/* management frames received */
	u_int32_t	ast_rx_ctl;	/* control frames received */
	int8_t		ast_tx_rssi;	/* tx rssi of last ack */
	int8_t		ast_rx_rssi;	/* rx rssi from histogram */
	u_int32_t	ast_be_xmit;	/* beacons transmitted */
	u_int32_t	ast_be_nobuf;	/* no skbuff available for beacon */
	u_int32_t	ast_per_cal;	/* periodic calibration calls */
	u_int32_t	ast_per_calfail;/* periodic calibration failed */
	u_int32_t	ast_per_rfgain;	/* periodic calibration rfgain reset */
	u_int32_t	ast_rate_calls;	/* rate control checks */
	u_int32_t	ast_rate_raise;	/* rate control raised xmit rate */
	u_int32_t	ast_rate_drop;	/* rate control dropped xmit rate */
	u_int32_t	ast_ant_defswitch;/* rx/default antenna switches */
	u_int32_t	ast_ant_txswitch;/* tx antenna switches */
	/*ljy--add begin for snmp*/
	u_int32_t	ast_up_times;	/* wifi up times.*/
 	u_int32_t	ast_down_times;	/* wifi down times.*/
	/*ljy--add end*/
	u_int32_t	ast_ant_rx[8];	/* rx frames with antenna */
	u_int32_t	ast_ant_tx[8];	/* tx frames with antenna */
};
#endif

typedef struct {
	unsigned char radio_id;
	unsigned char id;
	unsigned char updown_time;
} interface_updown_time;

typedef struct {  //pei add 0415
	unsigned char id;
	unsigned char status;  //0-not exist,1-up,2-down,3-error
} interface_status;

typedef struct {
	int cpu_use_ratio;
	unsigned int tx_mgmt;
	unsigned int rx_mgmt;
	unsigned int tx_packets;
	unsigned int tx_errors;
	unsigned int tx_retry;
	unsigned int tx_unicast; /* pei add for eth at 090624 */
	unsigned int tx_broadcast;
	unsigned int tx_multicast;
	unsigned int tx_drop;
	unsigned int rx_unicast;
	unsigned int rx_broadcast;
	unsigned int rx_multicast;
	unsigned int rx_drop; /* end. pei add for eth at 090624 */
	unsigned int wpi_replay_error;
	unsigned int wpi_decryp_error;
	unsigned int wpi_mic_error;
	unsigned int disassoc_unnormal;
	unsigned int rx_assoc_norate;
	unsigned int rx_assoc_capmismatch;
	unsigned int assoc_invalid;
	unsigned int reassoc_deny;
	unsigned char get_Ip_mode;
	unsigned short mem_total;
	unsigned char mem_use_ratio;
	unsigned short flash_total;
	int flash_free;
	unsigned char snr;
	char temperature;  //pei add 0415
	unsigned char eth_count;
	interface_updown_time *eth_updown_time;
	unsigned char ath_count;
	interface_updown_time *ath_updown_time;
	unsigned char wifi_count;  //pei add 0415
	interface_status *wifi_status;  //pei add 0415
} extra_info;
/*
 * Summary statistics.
 */
#if (defined(MODE11N)||defined(AR11N))
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
	u_int64_c	is_rx_mgmtbytes;
	u_int64_c	is_tx_mgmtbytes;        
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
#else
struct ieee80211_stats {
	u_int32_t is_rx_badversion;	/* rx frame with bad version */
	u_int32_t is_rx_tooshort;	/* rx frame too short */
	u_int32_t is_rx_wrongbss;	/* rx from wrong bssid */
	u_int32_t is_rx_dup;		/* rx discard due to it's a dup */
	u_int32_t is_rx_wrongdir;	/* rx w/ wrong direction */
	u_int32_t is_rx_mcastecho;	/* rx discard due to of mcast echo */
	u_int32_t is_rx_mcastdisabled;	/* rx discard due to of mcast disabled */
	u_int32_t is_rx_notassoc;	/* rx discard due to sta !assoc */
	u_int32_t is_rx_noprivacy;	/* rx w/ wep but privacy off */
	u_int32_t is_rx_unencrypted;	/* rx w/o wep and privacy on */
	u_int32_t is_rx_wepfail;		/* rx wep processing failed */
	u_int32_t is_rx_decap;		/* rx decapsulation failed */
	u_int32_t is_rx_mgtdiscard;	/* rx discard mgt frames */
	u_int32_t is_rx_ctl;		/* rx discard ctrl frames */
	u_int32_t is_rx_beacon;		/* rx beacon frames */
	u_int32_t is_rx_rstoobig;	/* rx rate set truncated */
	u_int32_t is_rx_elem_missing;	/* rx required element missing*/
	u_int32_t is_rx_elem_toobig;	/* rx element too big */
	u_int32_t is_rx_elem_toosmall;	/* rx element too small */
	u_int32_t is_rx_elem_unknown;	/* rx element unknown */
	u_int32_t is_rx_badchan;		/* rx frame w/ invalid chan */
	u_int32_t is_rx_chanmismatch;	/* rx frame chan mismatch */
	u_int32_t is_rx_nodealloc;	/* rx frame dropped */
	u_int32_t is_rx_ssidmismatch;	/* rx frame ssid mismatch  */
	u_int32_t is_rx_auth_unsupported;/* rx w/ unsupported auth alg */
	u_int32_t is_rx_auth_fail;	/* rx sta auth failure */
	u_int32_t is_rx_auth_countermeasures;/* rx auth discard due to CM */
	u_int32_t is_rx_assoc_bss;	/* rx assoc from wrong bssid */
	u_int32_t is_rx_assoc_notauth;	/* rx assoc w/o auth */
	u_int32_t is_rx_assoc_capmismatch;/* rx assoc w/ cap mismatch */
	u_int32_t is_rx_assoc_norate;	/* rx assoc w/ no rate match */
	u_int32_t is_rx_assoc_badwpaie;	/* rx assoc w/ bad WPA IE */
	u_int32_t is_rx_assoc_badscie;	/* rx assoc w/ bad SC IE */
	/*xmeng add for test*/
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
	/*xmeng add end*/

	u_int32_t is_rx_deauth;		/* rx deauthentication */
	u_int32_t is_rx_disassoc;	/* rx disassociation */
	u_int32_t is_rx_badsubtype;	/* rx frame w/ unknown subtype*/
	u_int32_t is_rx_nobuf;		/* rx failed for lack of buf */
	u_int32_t is_rx_decryptcrc;	/* rx decrypt failed on crc */
	u_int32_t is_rx_ahdemo_mgt;	/* rx discard ahdemo mgt frame*/
	u_int32_t is_rx_bad_auth;	/* rx bad auth request */
	u_int32_t is_rx_unauth;		/* rx on unauthorized port */
	u_int32_t is_rx_badkeyid;	/* rx w/ incorrect keyid */
	u_int32_t is_rx_ccmpreplay;	/* rx seq# violation (CCMP) */
	u_int32_t is_rx_ccmpformat;	/* rx format bad (CCMP) */
	u_int32_t is_rx_ccmpmic;		/* rx MIC check failed (CCMP) */
	u_int32_t is_rx_tkipreplay;	/* rx seq# violation (TKIP) */
	u_int32_t is_rx_tkipformat;	/* rx format bad (TKIP) */
	u_int32_t is_rx_tkipmic;		/* rx MIC check failed (TKIP) */
	u_int32_t is_rx_tkipicv;		/* rx ICV check failed (TKIP) */
	u_int32_t is_rx_badcipher;	/* rx failed due to of key type */
	u_int32_t is_rx_nocipherctx;	/* rx failed due to key !setup */
	u_int32_t is_rx_acl;		/* rx discard due to of acl policy */
	u_int32_t is_rx_ffcnt;		/* rx fast frames */
	u_int32_t is_rx_badathtnl;   	/* driver key alloc failed */
	u_int32_t is_tx_nobuf;		/* tx failed for lack of buf */
	u_int32_t is_tx_nonode;		/* tx failed for no node */
	u_int32_t is_tx_unknownmgt;	/* tx of unknown mgt frame */
	u_int32_t is_tx_badcipher;	/* tx failed due to of key type */
	u_int32_t is_tx_nodefkey;	/* tx failed due to no defkey */
	u_int32_t is_tx_noheadroom;	/* tx failed due to no space */
	u_int32_t is_tx_ffokcnt;		/* tx fast frames sent success */
	u_int32_t is_tx_fferrcnt;	/* tx fast frames sent success */
	u_int32_t is_scan_active;	/* active scans started */
	u_int32_t is_scan_passive;	/* passive scans started */
	u_int32_t is_node_timeout;	/* nodes timed out inactivity */
	u_int32_t is_node_timeout_auth;		/*xmeng add nodes timed out inactivity at auth state*/
	u_int32_t is_node_timeout_assoc;	/* xmeng add nodes timed out inactivity at assoc state */
	u_int32_t is_node_fdisassoc;	/* forced node disassociation */
	u_int32_t is_crypto_nomem;	/* no memory for crypto ctx */
	u_int32_t is_crypto_tkip;	/* tkip crypto done in s/w */
	u_int32_t is_crypto_tkipenmic;	/* tkip en-MIC done in s/w */
	u_int32_t is_crypto_tkipdemic;	/* tkip de-MIC done in s/w */
	u_int32_t is_crypto_tkipcm;	/* tkip counter measures */
	u_int32_t is_crypto_ccmp;	/* ccmp crypto done in s/w */
	u_int32_t is_crypto_wep;		/* wep crypto done in s/w */
	u_int32_t is_crypto_setkey_cipher;/* cipher rejected key */
	u_int32_t is_crypto_setkey_nokey;/* no key index for setkey */
	u_int32_t is_crypto_delkey;	/* driver key delete failed */
	u_int32_t is_crypto_badcipher;	/* unknown cipher */
	u_int32_t is_crypto_nocipher;	/* cipher not available */
	u_int32_t is_crypto_attachfail;	/* cipher attach failed */
	u_int32_t is_crypto_swfallback;	/* cipher fallback to s/w */
	u_int32_t is_crypto_keyfail;	/* driver key alloc failed */
	u_int32_t is_crypto_enmicfail;	/* en-MIC failed */
	u_int32_t is_ibss_capmismatch;	/* merge failed-cap mismatch */
	u_int32_t is_ibss_norate;	/* merge failed-rate mismatch */
	u_int32_t is_ps_unassoc;		/* ps-poll for unassoc. sta */
	u_int32_t is_ps_badaid;		/* ps-poll w/ incorrect aid */
	u_int32_t is_ps_qempty;		/* ps-poll w/ nothing to send */

	/*ljy--add begin to add the count of management via vap*/
	u_int32_t is_rx_mgmt;
	u_int64_c is_rx_mgmtbytes;
	u_int32_t is_tx_mgmt;
	u_int64_c is_tx_mgmtbytes;
	/*ljy--add end*/

	/*ljy--add begin for snmp*/
	u_int32_t is_assocs;
	u_int32_t is_reassocs;
	u_int32_t is_rx_repack;		/*终端重传的包数*/
	u_int32_t is_rx_rebytes;	/*终端重传的字节数*/
	/*ljy--add end*/
	
	/*ljy--add begin for wapi*/
	u_int32_t is_rx_err_pack;				/*接收到的来自终端的错误包数*/
	u_int32_t is_tx_signaling;				/*发送到指定终端的信令包数*/
	u_int32_t is_rx_signaling;				/*终端传送的信令包数*/
	u_int32_t is_re_wpi;/*根据重放机制丢弃的WPI MPDU的数目*/
	u_int32_t is_wpi_mic;  /* WPI-SMS4解密时没有有效的密钥而丢弃的MPDU数目*/
	u_int32_t is_wpi_no_key_error;/* WPI-SMS4解密时MIC校验出错而丢弃的MPDU数目*/
	/*ljy--add end*/
	
	/*ljy--add begin for snmp*/
	u_int32_t	is_rx_auth_try;	/*add wyh@20080716 for snmp*/
	u_int32_t	is_rx_auth_ok;	/*add wyh@20080716 for snmp*/
	u_int32_t	is_rx_assoc_try;	/*add wyh@20080716 for snmp*/
	u_int32_t	is_rx_ap_too_many_sta;	/*add wyh@20080716 for snmp*/
	/*ljy--add end*/
	/*Begin:Added by duanmingzhe for counting of error packets 2010-09-09*/
	u_int32_t is_rx_errors;
	/*End:Added by duanmingzhe for counting of error packets 2010-09-09*/
	/*Begin:Added by duanmingzhe for counting of error frames 2010-09-20*/
	u_int32_t is_rx_error_frames;
	/*End:Added by duanmingzhe for counting of error frames 2010-09-20*/

};
#endif

typedef struct {
	unsigned int wpi_replay_error;
	unsigned int wpi_decryp_error;
	unsigned int wpi_mic_error;
} wpi_errors;

typedef struct {
	unsigned int disassoc_unnormal;
	unsigned int rx_assoc_norate;
	unsigned int rx_assoc_capmismatch;
	unsigned int assoc_invalid;
	unsigned int reassoc_deny;
} disassoc_errors;
/*
 * Per/node (station) statistics available when operating as an AP.
 */
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
	u_int32_t ns_re_wpi;/*根据重放机制丢弃的WPI MPDU的数目*/
	u_int32_t ns_wpi_mic;  /* WPI-SMS4解密时没有有效的密钥而丢弃的MPDU数目*/
	u_int32_t ns_wpi_no_key_error;/* WPI-SMS4解密时MIC校验出错而丢弃的MPDU数目*/
	/*lyb add end*/
};
#endif
/*
 * Retrieve per-node statistics.
 */
struct ieee80211req_sta_stats {
	union {
		/* NB: explicitly force 64-bit alignment */
		u_int8_t macaddr[IEEE80211_ADDR_LEN];
		u_int64_c pad;
	} is_u;
	struct ieee80211_nodestats is_stats;
};

/*******************_ieee80211.h**************************************/

#if (defined(MODE11N)||defined(AR11N))
struct ieee80211req_sta_info {
	u_int16_t	isi_len;		/* length (mult of 4) */
	u_int16_t	isi_freq;		/* MHz */
	u_int32_t   isi_flags;      /* channel flags */
	u_int16_t	isi_state;		/* state flags */
	u_int8_t	isi_authmode;		/* authentication algorithm */
	int8_t	    	isi_rssi;
	u_int16_t	isi_capinfo;		/* capabilities */
	u_int8_t	isi_athflags;		/* Atheros capabilities */
	u_int8_t	isi_erp;		/* ERP element */
	u_int8_t	isi_macaddr[IEEE80211_ADDR_LEN];
	u_int8_t	isi_nrates;
						/* negotiated rates */
	u_int8_t	isi_rates[IEEE80211_RATE_MAXSIZE];
	u_int8_t	isi_txrate;		/* index to isi_rates[] */
	u_int32_t	isi_txratekbps;         /* tx rate in kbps */
	u_int32_t	isi_rxratekbps;         /* rx rate in kbps */
	u_int16_t	isi_ie_len;		/* IE length */
	u_int16_t	isi_associd;		/* assoc response */
	u_int16_t	isi_txpower;		/* current tx power */
	u_int16_t	isi_vlan;		/* vlan tag */
	u_int16_t	isi_txseqs[17];		/* seq to be transmitted */
	u_int16_t	isi_rxseqs[17];		/* seq previous for qos frames*/
	u_int16_t	isi_inact;		/* inactivity timer */
	u_int8_t	isi_uapsd;		/* UAPSD queues */
	u_int8_t	isi_opmode;		/* sta operating mode */
	u_int8_t	isi_cipher;
	u_int32_t	isi_assoc_time;	/* sta association time */

	u_int16_t	isi_htcap;		/* HT capabilities */
	/* XXX frag state? */
	/* variable length IE data */
};
#else
struct ieee80211req_sta_info {
	u_int16_t isi_len;		/* length (mult of 4) */
	u_int16_t isi_freq;		/* MHz */
	u_int16_t isi_flags;		/* channel flags */
	u_int16_t isi_state;		/* state flags */
	u_int8_t isi_authmode;		/* authentication algorithm */
	u_int8_t isi_rssi;
	int8_t isi_noise;
	u_int16_t isi_capinfo;		/* capabilities */
	u_int8_t isi_athflags;		/* Atheros capabilities */
	u_int8_t isi_erp;		/* ERP element */
	u_int8_t isi_macaddr[IEEE80211_ADDR_LEN];
	u_int8_t isi_nrates;		/* negotiated rates */
	u_int8_t isi_rates[IEEE80211_RATE_MAXSIZE];
	u_int8_t isi_txrate;		/* index to isi_rates[] */
	u_int16_t isi_ie_len;		/* IE length */
	u_int16_t isi_associd;		/* assoc response */
	u_int16_t isi_txpower;		/* current tx power */
	u_int16_t isi_vlan;		/* vlan tag */
	u_int16_t isi_txseqs[17];	/* seq to be transmitted */
	u_int16_t isi_rxseqs[17];	/* seq previous for qos frames*/
	u_int16_t isi_inact;		/* inactivity timer */
	u_int8_t isi_uapsd;		/* UAPSD queues */
	u_int8_t isi_opmode;		/* sta operating mode */

	/* XXX frag state? */
	/* variable length IE data */
};
#endif

enum opstate {
	NEW=1,
	DELETE,
	MODIFIED,
	REMAIN,
};

typedef struct node {      //pei add 1118
	unsigned char BSSID[6];	
	unsigned short Rate;
	unsigned char Channel;
	unsigned char RSSI;
	unsigned char	NOISE;
	unsigned char BEACON_INT;
	unsigned char Opstatus;
	unsigned short CapabilityInfo;
	unsigned char EssidLen;
	unsigned char Radio_id;
	char * ESSID;
	unsigned char IeLen;
	char * IEs_INFO;
	struct node *next;
} Neighbor_AP_INFO;

struct user_net_device_stats {
	unsigned long long rx_packets;	/* total packets received       */
	unsigned long long tx_packets;	/* total packets transmitted    */
	unsigned long long rx_bytes;	/* total bytes received         */
	unsigned long long tx_bytes;	/* total bytes transmitted      */
	unsigned long rx_errors;	/* bad packets received         */
	unsigned long tx_errors;	/* packet transmit problems     */
	unsigned long rx_dropped;	/* no space in linux buffers    */
	unsigned long tx_dropped;	/* no space available in linux  */
	unsigned long rx_multicast;	/* multicast packets received   */
	unsigned long rx_compressed;
	unsigned long tx_compressed;
	unsigned long collisions;
	unsigned long rx_broadcast;  /* pei add 090624 */
	unsigned long rx_unicast;
	unsigned long tx_multicast;
	unsigned long tx_broadcast;
	unsigned long tx_unicast;
	
	/* detailed rx_errors: */
	unsigned long rx_length_errors;
	unsigned long rx_over_errors;	/* receiver ring buff overflow  */
	unsigned long rx_crc_errors;	/* recved pkt with crc error    */
	unsigned long rx_frame_errors;	/* recv'd frame alignment error */
	unsigned long rx_fifo_errors;	/* recv'r fifo overrun          */
	unsigned long rx_missed_errors;	/* receiver missed packet     */
	/* detailed tx_errors */
	unsigned long tx_aborted_errors;
	unsigned long tx_carrier_errors;
	unsigned long tx_fifo_errors;
	unsigned long tx_heartbeat_errors;
	unsigned long tx_window_errors;
	unsigned long rx_multicast_bytes;
	unsigned long tx_multicast_bytes;
	unsigned long rx_broadcast_bytes;
	unsigned long tx_broadcast_bytes;
	unsigned long long rx_unicast_bytes;
	unsigned long long tx_unicast_bytes;
	unsigned long rx_retry_bytes;
	unsigned long tx_retry_bytes;
	unsigned long rx_retry;
	unsigned long tx_retry;	
	unsigned long rx_error_frames;		/* bad frames received		*/
};

typedef struct {
	unsigned int netmask;
	unsigned int gateway;
	unsigned int dns;
	unsigned int vice_dns;
}APIPInfoValues;
typedef struct {
	char WTPCPUTYPE[32];
	char WTPFLASHTYPE[32];
	unsigned short flashsize;
	char WTPMEMTYPE[32];
	unsigned short memsize;
	unsigned char eth_num;
	unsigned int eth_up_speed;
}ApHardInfoValues;
typedef struct {
	unsigned char radioId;
	unsigned char wlanId;
	unsigned char mac[6];
	unsigned int version;
	unsigned char controlledPortStatus;
	unsigned int selectedUnicastCipher;
	unsigned int wpiReplayCounters;
	unsigned int wpiDecryptableErrors;
	unsigned int wpiMICErrors;
} StationWapiInfo;
typedef struct {
	unsigned char value;
	unsigned char count;
	unsigned char is_flood_attack;
	unsigned char flood_detect_type;  // 0
	unsigned char flood_sta_mac[6];
	unsigned char flood_vap_mac[6];
	unsigned char flood_frame_type;
	unsigned char flood_channel;
	unsigned char flood_rssi;
	unsigned char is_spoof_attack;
	unsigned char spoof_detect_type;  // 1
	unsigned char spoof_sta_mac[6];
	unsigned char spoof_vap_mac[6];
	unsigned char spoof_frame_type;
	unsigned char spoof_channel;
	unsigned char spoof_rssi;
	unsigned char is_weakiv_attack;
	unsigned char weakiv_detect_type;  // 2
	unsigned char weakiv_sta_mac[6];
	unsigned char weakiv_vap_mac[6];
	unsigned char weakiv_payload;
	unsigned char weakiv_channel;
	unsigned char weakiv_rssi;
} attackDetectInfo;

typedef struct {
	unsigned char value;
	/* 4 bytes reserved */
	unsigned short count;
	unsigned char radio_id;
	unsigned char wlan_id;
	unsigned char state;  /* 1-warn, 0-no */
	unsigned char sta_mac[6];
} attackAddrRedirectionInfo;

typedef struct {
	unsigned char value;
	/* 4 bytes reserved */
	unsigned short count;
	unsigned char radio_id;
	unsigned char wlan_id;
	unsigned char state;  /* 1-warn, 0-no */
	unsigned char sta_mac[6];
} challengeReplayInfo;

typedef struct {
	unsigned char value;
	unsigned char radio_id;
	unsigned char wlan_id;
	unsigned char state;  /* 1-warn, 0-no */
	/* 4 bytes reserved */
	unsigned short sta_count;
	unsigned char *sta_mac;
} terminalDisturbInfo;

typedef struct {
	unsigned char optype;
	unsigned short int len;
	unsigned char sta_mac[6];
	unsigned int frames_recv;
	unsigned int frames_send;
	unsigned long long int bytes_recv;
	unsigned long long int bytes_send;
}staThroughputsendtoAc;

/* peiwenhui add 0703 --netlink */
#define IWEVEXPIRED	0x8C04		/* Expired a node (AP mode) */
#define IWEVTRAFFIC 	0x8C0B         /*station up/down line*/

#ifndef MSG_DONTWAIT
#define MSG_DONTWAIT 0
#endif

#ifndef IFLA_WIRELESS
#define IFLA_WIRELESS 11
#endif
#define NETLINK_ROUTE 0
#define RTMGRP_LINK 1
#define RTM_BASE 0x10
#define RTM_NEWLINK (RTM_BASE + 0)
#define NLMSG_ALIGNTO 4
#define NLMSG_ALIGN(len) (((len) + NLMSG_ALIGNTO - 1) & ~(NLMSG_ALIGNTO - 1))
#define NLMSG_LENGTH(len) ((len) + NLMSG_ALIGN(sizeof(struct nlmsghdr)))
#define NLMSG_DATA(nlh) ((void*) (((char*) nlh) + NLMSG_LENGTH(0)))

#define RTA_ALIGNTO 4
#define RTA_ALIGN(len) (((len) + RTA_ALIGNTO - 1) & ~(RTA_ALIGNTO - 1))
#define RTA_OK(rta,len) \
((len) > 0 && (rta)->rta_len >= sizeof(struct rtattr) && \
(rta)->rta_len <= (len))
#define RTA_NEXT(rta,attrlen) \
((attrlen) -= RTA_ALIGN((rta)->rta_len), \
(struct rtattr *) (((char *)(rta)) + RTA_ALIGN((rta)->rta_len)))

/* for wapi attack addr redirection trap -- pei add 20100911 */
/*Driver 发送的关于关联STA的信息*/	
typedef unsigned int u32;
typedef unsigned short u16;
typedef unsigned char u8;
struct asso_mt_t
{
	//struct nlmsghdr hdr;		/**/
	u16		type;		/*消息类型*/
	u16		data_len;		/*消息长度*/
#if 1  /*czm modified,temp*/
	u8		chan_old;
	u8		chan_curr;
#endif 
	u8 		ap_mac[6];
	u8 		pad1[2];
	u8		mac[6];		/*STA 的MAC地址*/
	u8 		pad[2];
	u8		gsn[16];		/*组播数据序号*/
	u8		wie[256];		/*wapi信息元素*/
};
typedef struct asso_mt_t asso_mt;
/* end */

/* peiwenhui add 090625 --- for get attack detect info */
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

#define IEEE80211_WIDS_SPOOF_DEAUTH 0x01
#define IEEE80211_WIDS_SPOOF_DISASSOC 0x02

#define IEEE80211_WIDS_WEAKIV_DETECT 0x03
/* end peiwenhui add 090625 --- for get attack detect info */
#define IEEE80211_STA_INTERFER_DETECT 0x09
#define IEEE80211_STA_INTERFER_CLEAR 0x0a


u_int32_t ieee80211_mhz2ieee(u_int32_t freq);

//station information report
void get_stations(char *ifname, int staCount, StationInfo *staInfoValues);
void get_sta_stats( StationInfo *staInfoValues,int sta_count);

//interface information report
CWBool CWGetInterfaceStatus(char *ifname, unsigned char *status);

//statistic report
void get_stations_ctrl(wlan_stats_info *wlanStatsInfo);
void get_dev_fields(char *bp, struct user_net_device_stats *wlan_stats, int procnetdev_vsn);
CWBool get_athinfo_from_80211stats(char *ifname, wlan_stats_info *wlanStatsInfo);
unsigned long long  CWGet_tunnel_bytes_recv();
unsigned long long  CWGet_tunnel_bytes_send();
CWBool get_wifi_info(char *wifiname, wlan_stats_info *wlanStatsInfo);

//RRM report
Neighbor_AP_INFO * CWGetApScanningResult(CWBool *bRet,unsigned int *neighborApCount);

//ExtraInfo report
CWBool CWGetExtraInfo(char *wifiname, extra_info *extraInfoValues);
CWBool CWGetInterfaceCount(int *count, char *type);
int CWGetRadioMaxCount();
int CWGetIPMode(void);
CWBool CWGetWifiStatus(extra_info *extraInfoValues);
int CWGetTotalMem();
int CWGetFreeMem();
int CWGetFlashConfigSize();
int CWGetFlashTotalSize();
CWBool CWGetCpuUseRatio(int *cpu_use_ratio);
char CWGetTemperature(void);
CWBool CWGetWpiErrors(char *ifname, wpi_errors *wpi_error_values);
CWBool CWGetDisassocErrors(char *ifname, disassoc_errors *disassoc_error_values);
CWBool CWGetEthUpTime(int * ifUpTime,int eth_num);
CWBool CWGetEthdownTime(int * ifDownTime,int eth_num);
unsigned char CWGetSnr(void);

//just enter RunState to report
CWBool CWGetAPOnlineTime(unsigned short *OnTime);
CWBool CWGetAPIsColdBoot(unsigned char *IsColdBoot);
CWBool CWGetAPIPInfo(APIPInfoValues *ipInfo);
CWBool CWGetAPhardInfo(ApHardInfoValues *hardInfo);

//StaWapiInfo report
void getStaWapiInfos(int staCount, StationWapiInfo *StaWapiInfo);


//netlink module

typedef enum
{
	NT_T_TerminalDisturbDetect,
	NT_T_TerminalDisturbClear,
	NT_T_AttackAddrRedirection,
	NT_T_ChallengeReplay,
	NT_T_AttackDetect,
	NT_T_StationAging,
	NT_T_Sta_Traffic_Limit
}NetLink_Type;


void * CWParseNetLinkBuffer(NetLink_Type *netLink_Type,char * data,int len,unsigned char *mac);
extern CWBool get_sta_throughput_info(staThroughputsendtoAc *staThroughputsendtoAcinfo);
extern CWBool PopenFile (char *cmd_str,char *str,int len );


#endif
