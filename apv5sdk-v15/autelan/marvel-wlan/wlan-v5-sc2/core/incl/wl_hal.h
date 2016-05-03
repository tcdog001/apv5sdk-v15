#ifndef _WL_HAL_H_
#define _WL_HAL_H_

#include "wl_mib.h"
#include "wlvmac.h"

#include "mlme.h"

#define SZ_PHY_ADDR 6   /*!< Number of bytes in ethernet MAC address */

/* mfg data struct*/
#define SZ_BOARD_NAME       8
#define SZ_BOOT_VERSION     12
#define SZ_PRODUCT_ID       58
#define SZ_INTERNAL_PA_CFG  14
#define SZ_EXTERNAL_PA_CFG  1
#define SZ_CCA_CFG          8
#define SZ_LED              4

typedef struct _MFG_CAL_DATA
{
	UINT8 BrdDscrpt[SZ_BOARD_NAME];      /* 8 byte ASCII to descript the type and version of the board */
	UINT8 Rev;
	UINT8 PAOptions;
	UINT8 ExtPA[SZ_EXTERNAL_PA_CFG];
	UINT8 Ant;
	UINT16 IntPA[SZ_INTERNAL_PA_CFG];
	UINT8 CCA[SZ_CCA_CFG];
	UINT16 Domain;            /* Wireless domain */
	UINT16 CstmrOpts;
	UINT8 LED[SZ_LED];
	UINT16 Xosc;
	UINT8 Reserved_1[2];
	UINT16 Magic;
	UINT16 ChkSum;
	UINT8 MfgMacAddr[SZ_PHY_ADDR];     /* Mfg mac address */
	UINT8 Reserved_2[4];
	UINT8 PID[SZ_PRODUCT_ID];          /* Production ID */
	UINT8 BootVersion[SZ_BOOT_VERSION];
} MFG_CAL_DATA;
typedef enum _WL_OP_MODE
{
	WL_OP_MODE_AP,
	WL_OP_MODE_VAP,
	WL_OP_MODE_STA,
	WL_OP_MODE_VSTA
}
WL_OP_MODE;

typedef struct _WL_SYS_CFG_DATA
{
	UINT8		 Spi16AddrLen;
	UINT8		 Rsrvd[3];
	MIB_802DOT11 Mib802dot11;
	MIB_802DOT11 ShadowMib802dot11;
}
WL_SYS_CFG_DATA;
#ifdef WDS_FEATURE
struct wds_port  {
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,25)
	struct net_device        netDevWdsPriv;
#endif
	struct net_device        *netDevWds;
	UINT8                 wdsMacAddr[6];
	void                     *pWdsDevInfo;
	UINT8                 wdsPortMode;
	UINT8                 active;
	BOOLEAN               wdsPortRegistered;
};
#endif
/*
 * List declarations.
 */
#define    ATH_LIST_HEAD(name, type)                    \
struct name {                                \
    struct type *lh_first;    /* first element */            \
}

#ifndef LIST_HEAD
#define LIST_HEAD ATH_LIST_HEAD
#endif

#define    LIST_HEAD_INITIALIZER(head)                    \
    { NULL }

#define    LIST_ENTRY(type)                        \
struct {                                \
    struct type *le_next;    /* next element */            \
    struct type **le_prev;    /* address of previous next element */    \
}

/*
 * List functions.
 */

#define    LIST_EMPTY(head)    ((head)->lh_first == NULL)

#define    LIST_FIRST(head)    ((head)->lh_first)

#define    LIST_FOREACH(var, head, field)                    \
    for ((var) = LIST_FIRST((head));                \
        (var);                            \
        (var) = LIST_NEXT((var), field))

#define    LIST_FOREACH_SAFE(var, head, field, tvar)            \
    for ((var) = LIST_FIRST((head));                \
        (var) && ((tvar) = LIST_NEXT((var), field), 1);        \
        (var) = (tvar))

#define    LIST_INIT(head) do {                        \
    LIST_FIRST((head)) = NULL;                    \
} while (0)

#define    LIST_INSERT_AFTER(listelm, elm, field) do {            \
    if ((LIST_NEXT((elm), field) = LIST_NEXT((listelm), field)) != NULL)\
        LIST_NEXT((listelm), field)->field.le_prev =        \
            &LIST_NEXT((elm), field);                \
    LIST_NEXT((listelm), field) = (elm);                \
    (elm)->field.le_prev = &LIST_NEXT((listelm), field);        \
} while (0)

#define    LIST_INSERT_BEFORE(listelm, elm, field) do {            \
    (elm)->field.le_prev = (listelm)->field.le_prev;        \
    LIST_NEXT((elm), field) = (listelm);                \
    *(listelm)->field.le_prev = (elm);                \
    (listelm)->field.le_prev = &LIST_NEXT((elm), field);        \
} while (0)

#define    LIST_INSERT_HEAD(head, elm, field) do {                \
    if ((LIST_NEXT((elm), field) = LIST_FIRST((head))) != NULL)    \
        LIST_FIRST((head))->field.le_prev = &LIST_NEXT((elm), field);\
    LIST_FIRST((head)) = (elm);                    \
    (elm)->field.le_prev = &LIST_FIRST((head));            \
} while (0)

#define    LIST_NEXT(elm, field)    ((elm)->field.le_next)

#define    LIST_REMOVE(elm, field) do {                    \
    if (LIST_NEXT((elm), field) != NULL)                \
        LIST_NEXT((elm), field)->field.le_prev =         \
            (elm)->field.le_prev;                \
    *(elm)->field.le_prev = LIST_NEXT((elm), field);        \
} while (0)

/*pengruofeng add start wids 2011-5-27*/
struct sta_interfer_detect_entry{
	UINT8 mac[IEEEtypes_ADDRESS_SIZE];
	LIST_ENTRY(sta_interfer_detect_entry) sta_interfer;
	int  count;
	
};

struct sta_interfer_detect{
	struct sta_interfer_detect_entry *sta_interfer_entry;
	ATH_LIST_HEAD(, sta_interfer_detect_entry) sta_interfer;
	spinlock_t attack_lock;
	int clear_flag;
	int trap_flag;
};

struct wids_detect_attacker_entry ;
struct wids_detect_attacker;

struct wids_detect_attacker_entry {
	UINT8 mac[IEEEtypes_ADDRESS_SIZE];
	LIST_ENTRY(wids_detect_attacker_entry) attacker;
	int assoc_req_cnt;
	int auth_req_cnt;
	int deauth_cnt;
	int disassoc_req_cnt;
	int null_data_cnt;
	int probe_req_cnt;
	int reassoc_req_cnt;
	int action_cnt;
	int attack_cnt;
	int rssi;
	int spoof_deauth_cnt;
	int spoof_disassoc_cnt;
	int weakiv_cnt;
};

struct wids_detect_attacker {
	struct wids_detect_attacker_entry *attacker_entry;
	ATH_LIST_HEAD(, wids_detect_attacker_entry) attacker;
	spinlock_t attack_lock;
	int attack_assoc_req_cnt;
	int attack_auth_req_cnt;
	int attack_deauth_cnt;
	int attack_disassoc_req_cnt;
	int attack_null_data_cnt;
	int attack_probe_req_cnt;
	int attack_reassoc_req_cnt;	
	int attack_action_cnt;
	int attack_spoof_deauth_cnt;
	int attack_spoof_disassoc_cnt;
	int attack_weakiv_cnt;
	int attacker_list_node_cnt;
};
/*pengruofeng add end 2011-5-27*/
//add by guijinlin for attack detection end
typedef struct vmacApInfo_t
{
	vmacEntry_t VMacEntry;
	UINT32 OpMode; /*!< Mode of operation: client or access point */
	UINT32 CpuFreq; /* CPU frequency */
	MIB_802DOT11 *Mib802dot11; /* Initial 802.11 MIB settings */
	MIB_802DOT11 *ShadowMib802dot11; /* Initial 802.11 MIB settings */
	MFG_CAL_DATA *CalData; /*!< Calibration data */
#ifdef NEWCALDATA
	MFG_HW_INFO  *mfgHwData;
	UINT8	hwInfoRev;
#endif
	struct net_device *dev;
	WL_SYS_CFG_DATA *sysCfgData;
	Timer AgingTimer;
	Timer KeepAliveTimer;	
	Timer monTimer;
	Timer scaningTimer;
	Timer reqMeasurementTimer;
	Timer MicTimer;
	Timer GrpKeytimer;
#ifdef COEXIST_20_40_SUPPORT
	Timer CoexistTimer;
#endif
	SyncSrvAp mgtSync;
	BOOLEAN download;
	IEEEtypes_MacAddr_t macStaAddr;
	IEEEtypes_MacAddr_t macStaAddr2;
	IEEEtypes_SsIdElement_t macSsId;
	IEEEtypes_SsIdElement_t macSsId2;
	IEEEtypes_MacAddr_t macBssId,macBssId2;
#ifdef AUTOCHANNEL
	UINT32 autochannel[IEEEtypes_MAX_CHANNELS+IEEEtypes_MAX_CHANNELS_A];
	UINT8 autochannelstarted;
	int StopTraffic;
#endif
#ifdef WDS_FEATURE
	struct wds_port 		 wdsPort[6];
	BOOLEAN wdsActive[6];
	PeerInfo_t wdsPeerInfo[6];
	int CurrFreeWdsPort;
#endif
	BOOLEAN keyMgmtInitDone;
	UINT8  busyScanning;
	UINT8 gUserInitScan;
	UINT32	NumScanChannels;
	SINT32	ChanIdx;
	IEEEtypes_ScanCmd_t  ScanParams;
	UINT32 PwrSaveStnCnt;
	struct STADB_CTL *StaCtl;	
	UINT8 SmeState;
	IEEEtypes_CapInfo_t macCapInfo;
	UINT32 bOnlyStnCnt;
	UINT32 monitorcnt;
	UINT32 g_IV32; // = 0;
	UINT16 g_IV16; // = 0x0001;
	Timer MIC_Errortimer;
	UINT8 MIC_Errorstatus;
	BOOLEAN MICCounterMeasureEnabled;//indicates if counter Measures is enabled
	UINT32 MIC_ErrordisableStaAsso;//1
	UINT8 numClients;
	UINT8 nClients;
	UINT8 legClients;
	UINT8 n40MClients;
	UINT8 n20MClients;					
	UINT8 gaClients;
	UINT8 bClients;
	UINT8 txPwrTblLoaded;
	UINT8 regionCodeLoaded;
	UINT32 work_to_do;
	UINT32 txQLimit;
	UINT8 Ampdu_Rx_Disable_Flag;
#ifdef MRVL_WSC
	WSC_BeaconIE_t thisbeaconIE ;
	WSC_ProbeRespIE_t thisprobeRespIE ;
	UINT8 WPSOn ;
#endif
#ifdef MRVL_WAPI
	WAPI_BeaconIEs_t thisbeaconIEs ;
	WAPI_ProbeRespIEs_t thisprobeRespIEs ;
#endif
	struct vmacApInfo_t *master;
	UINT8 NonGFSta;
	struct ETHSTADB_CTL *EthStaCtl;
	UINT8 dfsCacExp;
/* guijinlin add for low speed limitation 2012-06-29 start */		
	UINT32 mcs_mask;
/* guijinlin add for low speed limitation 2012-06-29 end */
//add by guijinlin for attack detection begin
	/*pengruofeng add for wids 2011-5-27*/
    struct timer_list iv_widsalarm;
    struct wids_detect_attacker iv_sta;
    UINT32 localap;
    UINT32 attack_max_cnt;
    UINT32 probe_attack_max_cnt;
    UINT32 flood_detect;
    UINT32 spoof_detect;
    UINT32 weakiv_detect;
    UINT32 clear_illegaldev_history;
    UINT32 clear_attack_history;
    UINT32 clear_attack_statistics;
	UINT32 sta_interferece_detect;
	UINT32 sta_interferece_count;
	struct sta_interfer_detect iv_sta_interferece;

    UINT32 wids_interval;
    UINT32 iv_weakiv_threshold;

    UINT32 iv_select_with_weight;
    UINT32 iv_select_with_near_rssi;
    UINT8 iv_select_with_channel;
    /*pengruofeng add end 2011-5-27*/
//add by guijinlin for attack detection end
	IEEEtypes_SuppRatesElement_t SuppRateSet;
	IEEEtypes_ExtSuppRatesElement_t ExtSuppRateSet;
#ifdef MRVL_WAPI
    UINT8 wapiPN[16];
    UINT8 wapiPN_mc[16];
#endif
    UINT32 auth_time_in_minutes;           //zhouke add ,auth time = 30s
}
vmacApInfo_t;

typedef  struct _WLAN_RX_INFO
{
	UINT8    resvd0;           /* reserved */
	UINT8    Sq2;              /* Signal Quality 2 */
	UINT16   resvd1;           /* reserved */
	UINT8    resvd2;           /* reserved */
	UINT8    Sq1;              /* Signal Quality 1 */
	UINT8    Rate;             /* rate at which frame was received */
	UINT8    RSSI;             /* RF Signal Strength Indicator */
	UINT16   QosControl;       /* QoS Control field */
	UINT16   resvd3;           /* reserved */
}
PACK_END WLAN_RX_INFO;


typedef enum _MCU_OP_MODE
{
	MCU_MODE_AP,
	MCU_MODE_STA_INFRA,
	MCU_MODE_STA_ADHOC
}
MCU_OP_MODE;
extern BOOLEAN wlSetRFChan(vmacApInfo_t *vmacSta_p,UINT32 chan);
extern BOOLEAN wlSetOpModeMCU(vmacApInfo_t *vmacSta_p,UINT32 mode);
void wlDestroySysCfg(vmacApInfo_t *vmacSta_p);
#endif


