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
#ifndef AP8X_INTF_H_
#define AP8X_INTF_H_

#include <linux/version.h>
#include <linux/module.h>
#include <linux/pci.h>
#include <linux/interrupt.h>
#include <linux/spinlock.h>
#include <linux/netdevice.h>
#include <linux/etherdevice.h>
#include <linux/skbuff.h>
#include <linux/delay.h>
#include <linux/if_ether.h>   
#include <linux/if_arp.h>   
#include <linux/init.h>
#include <linux/net.h>
#include <linux/wireless.h>

#include <net/iw_handler.h>
#include "IEEE_types.h"
#include "wltypes.h"
#include "ap8xLnxDesc.h"
#include "ap8xLnxApi.h"
#include "ap8xLnxStats.h"
#include "wl_mib.h"
#include "wl_hal.h"
#include "dfs.h"
#include "buildModes.h"
#include "ap8xLnxIoctlWiress.h"

#define MAX_CARDS_SUPPORT 2
#define MAX_VMAC_INSTANCE_AP    NUMOFAPS
#define MAX_VMAC_MIB_INSTANCE   NUMOFAPS+1

#ifdef WL_KERNEL_24
#undef IRQ_NONE 
#define IRQ_NONE 0
#undef IRQ_HANDLED
#define IRQ_HANDLED 1
#ifndef irqreturn_t
#define irqreturn_t void
#endif
#endif 

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,29)
#define NETDEV_PRIV(pre, dev)  ((pre *)netdev_priv(dev))
#define NETDEV_PRIV_P(pre, dev)  ((pre *)dev->ml_priv)
#define NETDEV_PRIV_S(dev)  (dev->ml_priv)
#else
#define NETDEV_PRIV(pre, dev) ((pre *)dev->priv)
#define NETDEV_PRIV_P(pre, dev)  ((pre *)dev->priv)
#define NETDEV_PRIV_S(dev)  (dev->priv)
#endif

extern int wlInit(struct net_device *, u_int16_t);
extern int wlDeinit(struct net_device *);
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,20)
extern irqreturn_t  wlISR(int irq, void *dev_id);
#else
extern irqreturn_t wlISR(int, void *, struct pt_regs *);
#endif
extern void wlInterruptEnable(struct net_device *);
extern void wlInterruptDisable(struct net_device *);
extern void wlFwReset(struct net_device *);
extern int wlChkAdapter(struct net_device *);
extern void wlSendEvent(struct net_device *dev, int, IEEEtypes_MacAddr_t *,const char *);

#ifdef ENABLE_WLSNDEVT
#define WLSNDEVT(dev, cmd, Addr, info) wlSendEvent(dev,cmd, Addr, info)
#else
#define WLSNDEVT(dev, cmd, Addr, info)
#endif
#ifdef WDS_FEATURE
int wlstop_wds(struct net_device *netdev);
#endif

#ifdef MCAST_PS_OFFLOAD_SUPPORT
#define NUM_OF_DESCRIPTOR_DATA (4 + NUMOFAPS) 
#else
#define NUM_OF_DESCRIPTOR_DATA 4
#endif
#ifdef AP82S//WL_KERNEL_26  
#define MAX_NUM_AGGR_BUFF 128//256
#define MAX_NUM_RX_DESC        200//256
#else
#define MAX_NUM_AGGR_BUFF 256
#define MAX_NUM_RX_DESC        256
#endif
/*3839 ~ 4k*/
#define MAX_AGGR_SIZE       4096
#define MAX_NUM_TX_DESC        256
#define MIN_BYTES_HEADROOM      64
#define MAX_BYTES_HEADROOM      96
#define NUM_EXTRA_RX_BYTES     (2*MIN_BYTES_HEADROOM)

#define ENDIAN_SWAP32(_val)   (cpu_to_le32(_val))
#define ENDIAN_SWAP16(_val)   (cpu_to_le16(_val))


struct wldesc_data {
    dma_addr_t       pPhysTxRing;          /* ptr to first TX desc (phys.)    */
    wltxdesc_t    *pTxRing;              /* ptr to first TX desc (virt.)    */
    wltxdesc_t    *pNextTxDesc;          /* next TX desc that can be used   */
    wltxdesc_t    *pStaleTxDesc;         /* the staled TX descriptor        */
    dma_addr_t       pPhysRxRing;          /* ptr to first RX desc (phys.)    */
    wlrxdesc_t    *pRxRing;              /* ptr to first RX desc (virt.)    */
    wlrxdesc_t    *pNextRxDesc;          /* next RX desc that can be used   */
    unsigned int     wcbBase;              /* FW base offset for registers    */
    unsigned int     rxDescWrite;          /* FW descriptor write position    */
    unsigned int     rxDescRead;           /* FW descriptor read position     */
    unsigned int     rxBufSize;            /* length of the RX buffers        */
};

struct wllocks {   
    spinlock_t       xmitLock;             /* used to protect TX actions      */
    spinlock_t       fwLock;               /* used to protect FW commands     */
};

struct wlhw_data {
    u_int32_t        fwReleaseNumber;      /* MajNbr:MinNbr:SubMin:PatchLevel */
    u_int8_t         hwVersion;            /* plain number indicating version */
    u_int8_t         hostInterface;        /* plain number of interface       */
    u_int16_t        maxNumTXdesc;         /* max number of TX descriptors    */
    u_int16_t        maxNumMCaddr;         /* max number multicast addresses  */
    u_int16_t        numAntennas;          /* number antennas used            */
    u_int16_t        regionCode;           /* region (eg. 0x10 for USA FCC)   */
    unsigned char    macAddr[ETH_ALEN];    /* well known -> AA:BB:CC:DD:EE:FF */
};

struct wlpriv_stats {
    u_int32_t        skbheaderroomfailure;
    u_int32_t       tsoframecount;
	u_int32_t	weakiv_count;
	u_int32_t	weakiv_threshold_count;

};

#ifdef AMPDU_SUPPORT
#ifdef AMPDU_SUPPORT_SBA
#define MAX_SUPPORT_AMPDU_TX_STREAM      24  /** for superfly3, only 2 stream of ampdu is supported, add one more for s/w ba **/
#else
#define MAX_SUPPORT_AMPDU_TX_STREAM 2 
#endif

#define MAX_AMPDU_REORDER_BUFFER MAX_AID
#define MAX_AC 4
#define MAX_UP 8
#define MAX_AC_SEQNO 4096

/* WME stream classes */
#define WME_AC_BE   0       /* best effort */
#define WME_AC_BK   1       /* background */
#define WME_AC_VI   2       /* video */
#define WME_AC_VO   3       /* voice */

/*Begin:Added by duanmingzhe for user isolation*/
#define VAP_UNICAST_ISOLATION       0x0001
#define VAP_MULTICAST_ISOLATION     0x0002
#define WIFI_ALLPACKET_ISOLATION    0x0004
/*End:Added by duanmingzhe for user isolation*/

typedef struct
{
    vmacApInfo_t *vmacSta_p;
    UINT8 MacAddr[6];
    UINT8 AccessCat;
    UINT8 InUse;
    UINT8 DialogToken;
    Timer timer;
    UINT8 initTimer;
    UINT8 AddBaResponseReceive;
    UINT32 TimeOut;
    UINT16 start_seqno;
#ifdef DYNAMIC_BA_SUPPORT
    UINT32 txa_avgpps;
    UINT32 txa_ac;
    UINT32 txa_pkts;
    UINT32 txa_lastsample;
#endif
    UINT32 ReleaseTimestamp;
}Ampdu_tx_t;

#define TID_TO_WME_AC(_tid) (      \
    ((_tid) == 0 || (_tid) == 3) ? WME_AC_BE : \
    ((_tid) < 3) ? WME_AC_BK : \
    ((_tid) < 6) ? WME_AC_VI : \
    WME_AC_VO)


typedef struct
{
    struct sk_buff  *pFrame[MAX_UP][MAX_AMPDU_REORDER_BUFFER];
    UINT16 ExpectedSeqNo[MAX_UP][MAX_AMPDU_REORDER_BUFFER];
    UINT16 CurrentSeqNo[MAX_UP];
    UINT16 ReOrdering[MAX_UP];
    UINT8 AddBaReceive[MAX_UP];
    UINT32 Time[MAX_UP];
}Ampdu_Pck_Reorder_t;

#endif
struct wlprivate_data {
    dma_addr_t          pPhysCmdBuf;      /* pointer to CmdBuf (physical) */
    struct timer_list       Timer; /* timer tick for Timer.c     */
    Bool_t              isMtuChanged;     /* change may interact with open*/
    Bool_t              isTxTimeout;      /* timeout may collide with scan*/
    Bool_t              inReset;          /* is chip currently resetting  */
    Bool_t              inResetQ;         /* is chip currently resetting  */
    struct wllocks          locks;          /* various spinlocks            */
    struct wlpriv_stats     privStats;      /* wireless statistic data      */
    struct iw_statistics        wStats;       /* wireless statistic data      */
    struct sk_buff_head     aggreQ;
    struct sk_buff_head     txQ[NUM_OF_DESCRIPTOR_DATA];
    struct wldesc_data  descData[NUM_OF_DESCRIPTOR_DATA];       /* various descriptor data      */
#ifdef WL_KERNEL_26
    UINT8 isTxTaskScheduled;            /*To keep scheduling status of a tx task*/
#ifdef USE_TASKLET
    struct tasklet_struct txtask;
#else
    struct work_struct      txtask;
#endif

#ifdef USE_TASKLET
    struct tasklet_struct rxtask;
#else
    struct work_struct      rxtask;
#endif
    struct work_struct      resettask;
#ifdef MRVL_DFS
    struct work_struct dfstask;
    struct work_struct csatask;
#endif
	struct work_struct 		kickstatask;		
#endif
    int             SDRAMSIZE_Addr;
    int             CardDeviceInfo;
    int             fwDescCnt[NUM_OF_DESCRIPTOR_DATA];/* number of descriptors owned by fw at any one time */
    int             txDoneCnt;/* number of tx packet to call wlTXDONE() */
    int             vmacIndex;
    Bool_t          inSendCmd;
    vmacApInfo_t        *vmacampdurxap_p;
    UINT8           ampdurxmacaddrset;
#ifdef AMPDU_SUPPORT
    Ampdu_Pck_Reorder_t AmpduPckReorder[MAX_AID+1];
    Ampdu_tx_t Ampdu_tx[MAX_SUPPORT_AMPDU_TX_STREAM];
    UINT8 Global_DialogToken;
#endif
    struct wlprivate *masterwlp;
#ifdef MRVL_DFS
    DfsAp *pdfsApMain ;
#endif //MRVL_DFS
    UINT8 TxGf;
    UINT8 NonGFSta; 
    UINT8 BcnAddHtOpMode;
    UINT8 legClients;
    UINT8 n20MClients;                  
    UINT8 nClients; 
    UINT8 legAPCount;
#ifdef COEXIST_20_40_SUPPORT
    UINT8 BcnAddHtAddChannel;
#endif

    /*Begin:Added by duanmingzhe for monitor process*/
    struct timer_list monitor_process_timer;
    /*End:Added by duanmingzhe for monitor process*/
    int ic_wtpd_init_timer;

    struct net_device   *rootdev;
};

//add begin for autelan wmm//lhj//2012.06
#ifdef AE5200_RT_SUPPORT
struct autelan_wmm {
	unsigned char tos_flag;
	unsigned char vlan_flag;
	/*Begin:Modified by zhaoyang1 for optimize map of tos vlan & 80211e up 2013-01-28*/
	unsigned char vlan_ingress_priority[8];
	unsigned char vlan_egress_priority[8];
	unsigned char tos_ingress_priority[8];
	unsigned char tos_egress_priority[64];
	/*End:Modified by zhaoyang1 for optimize map of tos vlan & 80211e up 2013-01-28*/
	unsigned char be_flag;	/*ljy--add to append the switch modified the best effort parameter*/
};

#else

struct autelan_wmm {
    UINT8 tos_flag;
    UINT8 vlan_flag;
    UINT8 vlan_ingress_priority[4];
    UINT8 vlan_egress_priority[8];
    UINT8 tos_ingress_priority[4];
    UINT8 tos_egress_priority[16];
    UINT8 be_flag;  /*ljy--add to append the switch modified the best effort parameter*/
};
#endif
//add end for autelan wmm

//modify by lirui for traffic limit begin 
struct autelan_traffic_limit
{
    u_int32_t       traffic_limit_flag;             //traffic limit switch
    u_int32_t       tx_traffic_limit;               //lmit rate
    u_int32_t       rx_traffic_limit;               //lmit rate
    struct timeval  rx_last_time;           
    struct timeval  tx_last_time;           
    u_int32_t       rx_rate_in_us_per_byte; 
    u_int32_t       tx_rate_in_us_per_byte; 
};
//modify by lirui for traffic limit begin 

struct ieee80211vap
{
/*Begin:Added by duanmingzhe for thinap*/
    UINT32 vap_splitmac;
    UINT8   iv_eap;
/*Begin:Added by duanmingzhe for user isolation*/
    UINT32 iv_switch;   /*broadcasts and unicast switch*/
/*End:Added by duanmingzhe for user isolation*/
    UINT8 vap_dhcp_enable;
    u_int8_t vap_ip_auto_learning;
    UINT8 vap_pppoe_enable;
/*End:Added by duanmingzhe for thinap*/
/*Begin:added by chenming for rssithre*/
    UINT8 iv_rssi_threshold;
/*End:added by chenming for rssithre*/
/*suzhaoyu add for customer online-traffic limit*/
    int         lowest_traffic_limit_switch;
    UINT32      lowest_traffic_limit_timelength;
    UINT64  lowest_traffic_limit_threshold;
/*suzhaoyu addend*/
    UINT32 iv_flags;   /* state flags */
    UINT32 iv_flags_ext;   /* extension of state flags */
/*Begin:Added by zhanxuechao for wpa*/
    struct ieee80211_rsnparms         iv_rsn;         /* rsn information */
/*End:Added by zhanxuechao for wpa*/
    struct autelan_wmm      priv_wmm;               //add for autelan wmm //lhj//2012.06
    struct autelan_traffic_limit iv_tl;             //Added by duanmingzhe for traffic limit 2.0//lhj//2012.06
    struct autelan_traffic_limit iv_everynode_tl;   //Added by duanmingzhe for traffic limit 2.0//lhj//2012.06

    UINT8 iv_probe_request; //add by yuanding for probe request restrict
};

struct wlprivate {
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,25)
    struct net_device               netDevPriv;          /* the net_device struct        */
#endif
    struct net_device               *netDev;          /* the net_device struct        */
#ifdef NAPI
    struct napi_struct napi;
#endif
    struct net_device_stats     netDevStats;     /* net_device statistics        */
    struct pci_dev                  *pPciDev;         /* for access to pci cfg space  */
    void                                *ioBase0;         /* MEM Base Address Register 0  */
    void                                *ioBase1;         /* MEM Base Address Register 1  */
    unsigned short                  *pCmdBuf;         /* pointer to CmdBuf (virtual)  */
    struct wlhw_data        hwData;         /* Adapter HW specific info     */
    vmacApInfo_t            *vmacSta_p;
#ifdef CLIENT_SUPPORT
    void                                *clntParent_priv_p;
#endif /* CLIENT_SUPPORT */
    int (*wlreset)(struct net_device *netdev);
    struct net_device           *master;
    struct net_device           *vdev[MAX_VMAC_INSTANCE_AP+1]; //+1 station
    struct wlprivate_data       *wlpd_p;
    UINT8 calTbl[200];
    UINT8 *FwPointer;
    UINT32 FwSize;  
    UINT8 mfgEnable;
    UINT32 cmdFlags;  /* Command flags */
    struct net_device *txNetdev_p;
    UINT32 nextBarNum;
    UINT32 chipversion;
    UINT32 mfgLoaded;
    wlan_stats_info stats_info;              // zhouke add, for interface statistics;

/*Begin:Added by zhanxuechao for thinap proc*/
    struct ctl_table_header *sc_sysctl_header;
    struct ctl_table        *sc_sysctls;
    struct ctl_table_header *sc_wl_sysctl_header;
    struct ctl_table        *sc_wl_sysctls;
/*End:Added by zhanxuechao for thinap proc*/

/*Begin:Added by zhanxuechao for wiress proc*/
    struct ieee80211vap vap;
/*End:Added by zhanxuechao for wiress proc*/
};

extern struct net_device *mainNetdev_p[NUM_OF_WLMACS];
extern int wlinitcnt;
extern UINT8   tmpScanResults[MAX_SCAN_BUF_SIZE];
extern UINT8   tmpNumScanDesc;
extern int wlResetTask(struct net_device *dev);
extern void wlLinkMgt(struct net_device *netdev, UINT8 phyIndex);
int wlInit_vap(struct wlprivate *wlp,unsigned char *macaddr,unsigned char *name,unsigned int index);
int wlDeinit_vap(struct net_device *netdev,struct net_device *vapdev);

#endif /* AP8X_INTF_H_ */
