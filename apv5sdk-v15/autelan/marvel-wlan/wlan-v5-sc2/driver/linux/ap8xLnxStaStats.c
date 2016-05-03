
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

#include <net/iw_handler.h>
#include <asm/processor.h>
#include <asm/uaccess.h>
#include "ap8xLnxFwcmd.h"
#include "ap8xLnxStaStats.h"
#include "StaDb.h"
#include "IEEE_types.h"
#include "wllog.h"
#include "ap8xLnx_scan.h"

extern UINT16 dogetPhyRate(dbRateInfo_t *pRateTbl);

static const long frequency_list[] = {
    2412, 2417, 2422, 2427, 2432, 2437, 2442, 2447, 2452, 2457, 2462, 2467, 2472, 2484,
    4915, 4920, 4925, 4935, 4940, 4945, 4960, 4980,
    5035, 5040, 5045, 5055, 5060, 5080, 5170, 5180, 5190, 5200, 5210, 5220, 5230, 5240,
    5260, 5280, 5300, 5320, 5500, 5520, 5540, 5560, 5580, 5600, 5620, 5640, 5660, 5680,
    5700, 5745, 5765, 5785, 5805, 5825
};

static const int rates[] =
{
    2, 4, 11, 22, 12, 18, 24, 36, 48, 72, 96, 108
};



void getHtCapinfo(UINT16 *isr_capinfo,IEEEtypes_HT_Cap_t CapInfo)
{
    *isr_capinfo = 0;

    if(CapInfo.AdvCoding)
        *isr_capinfo |= IEEE80211_HTCAP_C_ADVCODING;
    if(CapInfo.SupChanWidth)
        *isr_capinfo |= IEEE80211_HTCAP_C_CHWIDTH40;
    if(CapInfo.MIMOPwSave)
        *isr_capinfo |= IEEE80211_HTCAP_C_SMPOWERSAVE_DYNAMIC;
    if(CapInfo.GreenField)
        *isr_capinfo |= IEEE80211_HTCAP_C_GREENFIELD;
    if(CapInfo.SGI20MHz)
        *isr_capinfo |= IEEE80211_HTCAP_C_SHORTGI20;
    if(CapInfo.SGI40MHz)
        *isr_capinfo |= IEEE80211_HTCAP_C_SHORTGI40;
    if(CapInfo.TxSTBC)
        *isr_capinfo |= IEEE80211_HTCAP_C_TXSTBC;
    if(CapInfo.RxSTBC)
        *isr_capinfo |= IEEE80211_HTCAP_C_RXSTBC;
    if(CapInfo.MaxAMSDUSize)
        *isr_capinfo |= IEEE80211_HTCAP_C_MAXAMSDUSIZE;
    if(CapInfo.DssCck40MHz)
        *isr_capinfo |= IEEE80211_HTCAP_C_DSSSCCK40;
    if(CapInfo.PSMP)
        *isr_capinfo |= IEEE80211_HTCAP_C_PSMP;
    if(CapInfo.FortyMIntolerant)
        *isr_capinfo |= IEEE80211_HTCAP_C_INTOLERANT40;
    if(CapInfo.LSIGTxopProc)
        *isr_capinfo |= IEEE80211_HTCAP_C_LSIGTXOPPROT;
    return;
}


int ieee80211_ioctl_getstastats(struct net_device *netdev, char *param, char *ret_str, UINT16 *ret_len)
{   
	//yuanding changed
    //struct ieee80211_stastats *tmp = &(((struct ieee80211req_sta_stats *)param)->sta_stats);
    struct ieee80211_nodestats *tmp = &(((struct ieee80211req_sta_stats *)param)->is_stats);
	
    struct wlprivate *wlpptr = NETDEV_PRIV_P(struct wlprivate, netdev);
    vmacApInfo_t *vmacSta_p = wlpptr->vmacSta_p;
    int rc = 0;
    extStaDb_StaInfo_t *StaInfo = extStaDb_GetStaInfo(vmacSta_p, (IEEEtypes_MacAddr_t *)(((struct ieee80211req_sta_stats*)param)->is_u.macaddr), 0);
    if(NULL == StaInfo)
    {
        WlLogPrint(MARVEL_DEBUG_ALL, __func__, "ethStaDb_GetStaInfo failed\n");
        return -1;
    }
     
    tmp->ns_rx_bytes = StaInfo->sta_stats.ns_rx_bytes;
    tmp->ns_tx_bytes = StaInfo->sta_stats.ns_tx_bytes;
    tmp->ns_rx_data = StaInfo->sta_stats.ns_rx_data;
    tmp->ns_tx_data = StaInfo->sta_stats.ns_tx_data;
	/*yuanding changed
    *ret_len = sizeof(struct ieee80211_stastats);
    rc = copy_to_user(&(((struct ieee80211req_sta_stats*)ret_str)->sta_stats), tmp, *ret_len);
    */
    *ret_len = sizeof(struct ieee80211_nodestats);
    rc = copy_to_user(&(((struct ieee80211req_sta_stats*)ret_str)->is_stats), tmp, *ret_len);
    return rc;

}

int ieee80211_ioctl_getstainfo(struct net_device *netdev, char *param, char *ret_str, UINT16 *ret_len)
{
    struct ieee80211req_sta_info *tmp = (struct ieee80211req_sta_info *)param;
    struct wlprivate *wlpptr = NETDEV_PRIV_P(struct wlprivate, netdev);
    vmacApInfo_t *vmacSta_p = wlpptr->vmacSta_p;

    /******************************************/ 
    /********** yuanding add start*************/
    vmacApInfo_t *vmac_p = NULL;
    ExtStaInfoItem_t *tmp_search = NULL;

    /*********  yuanding add end **************/ 
    /******************************************/ 

    extStaDb_StaInfo_t *StaInfo = NULL;
    ListItem *search = NULL;
    int i = 0;
    int retn = 0;
  /*  if(vmacSta_p->master)
        vmacSta_p = vmacSta_p->master;*/

    /***************************************************/
    /********** yuanding add    start******************/
    if(vmacSta_p->master)
    {
        vmac_p = vmacSta_p;           //vmac_p    : vap
        vmacSta_p = vmacSta_p->master;//vmacSta_p : wifi interface
    }
    else
    {
        return 0;
    }
    /**********  yuanding add    end *******************/
    /***************************************************/
    if(!vmacSta_p->StaCtl->Initialized)
    {
        return 0;
    }
    search = vmacSta_p->StaCtl->StaList.head;
    
    while(search)
    {
        /***************************************************/
        /********** yuanding add   start *******************/
        tmp_search = (ExtStaInfoItem_t *)search;

        if( (tmp_search->StaInfo.AP == false)
            && (!(tmp_search->StaInfo.State == AUTHENTICATED)||
                !(tmp_search->StaInfo.State == ASSOCIATED))
            && memcmp( &vmac_p->macBssId , &tmp_search->StaInfo.Bssid ,sizeof(IEEEtypes_MacAddr_t) ))
        {
            search = search->nxt;
            continue;
        }
        /********** yuanding add   end *******************/
        /***************************************************/	
        memset(tmp, 0, sizeof(struct ieee80211req_sta_info));
        StaInfo = &(((ExtStaInfoItem_t *)search)->StaInfo);
        memcpy(tmp->isi_macaddr, StaInfo->Addr, IEEEtypes_ADDRESS_SIZE);
        tmp->isi_rssi = StaInfo->RSSI;
        if(StaInfo->mib_p->PhyDSSSTable->CurrChan <= 14)
        {
            tmp->isi_freq = frequency_list[StaInfo->mib_p->PhyDSSSTable->CurrChan - 1];
            if(StaInfo->ClientMode == BONLY_MODE)
            {
                tmp->isi_nrates = 4;
            }
            else if(StaInfo->ClientMode == MIXED_MODE)
            {
                tmp->isi_nrates = 12;
            }
        }
        else
        {
            tmp->isi_freq = StaInfo->mib_p->PhyDSSSTable->CurrChan * 5 + 5000;
        } 
        
        tmp->isi_txratekbps = StaInfo->Rate/2 * 1000;
        
        if(0 == tmp->isi_txratekbps)
        { 
            if(StaInfo->ClientMode == BONLY_MODE)
            {
                tmp->isi_txrate = *(StaInfo->mib_p->mib_txDataRate);
            }
            else if(StaInfo->ClientMode == GONLY_MODE || StaInfo->ClientMode == MIXED_MODE)
            {
                tmp->isi_txrate = *(StaInfo->mib_p->mib_txDataRateG);
            }
            else if(StaInfo->ClientMode == AONLY_MODE)
            {
                tmp->isi_txrate = *(StaInfo->mib_p->mib_txDataRateA);
            }
            else if(StaInfo->ClientMode == NONLY_MODE)
            {
                tmp->isi_txrate = *(StaInfo->mib_p->mib_txDataRateN);
            }
            tmp->isi_rates[tmp->isi_txrate] = rates[tmp->isi_txrate] / 2 * 1000;
        }

        tmp->isi_associd = StaInfo->Aid;

        tmp->isi_inact = StaInfo->TimeStamp * AGING_TIMER_VALUE_IN_SECONDS;

        tmp->isi_rxseqs[0] = StaInfo->sta_stats.ns_rx_data;     //?

        tmp->isi_txseqs[0] = StaInfo->sta_stats.ns_tx_data;     //?

        getcapinfo(&tmp->isi_capinfo,StaInfo->CapInfo);

        tmp->isi_erp = 0;

        tmp->isi_assoc_time = jiffies - StaInfo->ni_assocuptime;

        tmp->isi_rxratekbps = dogetPhyRate(&(StaInfo->RateInfo)) * 1000;

        getHtCapinfo(&tmp->isi_htcap,StaInfo->PeerHTCapabilitiesInfo);

        tmp->isi_state = StaInfo->ni_flags;
        tmp->isi_len  = sizeof(struct ieee80211req_sta_info);
        search = search->nxt;
        tmp = tmp + 1;
        i++;
    }
    if(i == 0)
        return -1;
    *ret_len = i * sizeof(struct ieee80211req_sta_info);
    retn = copy_to_user(ret_str, param, *ret_len);
     
    return retn;
}

