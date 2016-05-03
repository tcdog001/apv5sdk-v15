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
#include "ap8xLnx_scan.h"

#ifdef CLIENT_SUPPORT
#include "linkmgt.h"
#include "mlme.h"
#include "mlmeApi.h"
#endif

#include "wllog.h"

extern int iwlist_scan_onebyone;
extern u_int16_t iwlist_scan_current_chan_na;
extern u_int16_t iwlist_scan_current_chan_ng;

size_t copy_essid(char buf[], size_t bufsize, const u_int8_t *essid, size_t essid_len)
{
    const u_int8_t *p; 
    size_t maxlen;
    int i;

    if (essid_len > bufsize)
        maxlen = bufsize;
    else
        maxlen = essid_len;
    /* determine printable or not */
    for (i = 0, p = essid; i < maxlen; i++, p++) {
        if (*p < ' ' || *p > 0x7e)
            break;
    }
    if (i != maxlen){       /* not printable, print as hex */
        if (bufsize < 3)
            return 0;
        strncpy(buf, "0x", bufsize);
        bufsize -= 2;
        p = essid;
        for (i = 0; i < maxlen && bufsize >= 2; i++) {
            sprintf(&buf[2+2*i], "%02x", *p++);
            bufsize -= 2;
        }
        maxlen = 2+2*i;
    } 
    else{           /* printable, truncate as needed */
        memcpy(buf, essid, maxlen);
    }
    if (maxlen != essid_len)
        memcpy(buf+maxlen-3, "...", 3);
    return maxlen;
}


int getmaxrate(uint8_t rates[15], uint8_t nrates)
{
    int i, maxrate = -1;

    for (i = 0; i < nrates; i++) {
        int rate = rates[i] & IEEE80211_RATE_VAL;
        if (rate > maxrate)
            maxrate = rate;
    }
    return maxrate / 2;
}


int scan_space(struct scanreq  *req)
{
    scanDescptHdr_t *curDescpt_p = NULL;
    IEEEtypes_SsIdElement_t *ssidIE_p = NULL;
    UINT16 parsedLen = 0,ielen = 0;
    UINT8 ssid_len = 0,i=0;

    if(req == NULL)
        return 0;

    for(i = 0; i < tmpNumScanDesc; i++)
    {   
        curDescpt_p = (scanDescptHdr_t *)(&tmpScanResults[0] + parsedLen);

        if((ssidIE_p = (IEEEtypes_SsIdElement_t *)smeParseIeType(SSID,
          (((UINT8 *)curDescpt_p) +sizeof(scanDescptHdr_t)),
            curDescpt_p->length + sizeof(curDescpt_p->length) - sizeof(scanDescptHdr_t))) != NULL)
        {
            ssid_len = ssidIE_p->Len;
        }

        ielen = curDescpt_p->length + sizeof(curDescpt_p->length) - sizeof(scanDescptHdr_t);
        if (ielen > 65534)
        {
            ielen = 0;
        }

        req->space += roundup(sizeof(struct ieee80211req_scan_result) + ssid_len + ielen, sizeof(SINT32));
        parsedLen += curDescpt_p->length + sizeof(curDescpt_p->length);
    }
    
    return 1;
}


void getcapinfo(UINT16 *isr_capinfo,IEEEtypes_CapInfo_t CapInfo)
{
    *isr_capinfo = 0;

    if(CapInfo.Ess)
        *isr_capinfo |= IEEE80211_CAPINFO_ESS;
    if(CapInfo.Ibss)
        *isr_capinfo |= IEEE80211_CAPINFO_IBSS;
    if(CapInfo.CfPollable)
        *isr_capinfo |= IEEE80211_CAPINFO_CF_POLLABLE;
    if(CapInfo.CfPollRqst)
        *isr_capinfo |= IEEE80211_CAPINFO_CF_POLLREQ;
    if(CapInfo.Privacy)
        *isr_capinfo |= IEEE80211_CAPINFO_PRIVACY;
    if(CapInfo.ShortPreamble)
        *isr_capinfo |= IEEE80211_CAPINFO_SHORT_PREAMBLE;
    if(CapInfo.Pbcc)
        *isr_capinfo |= IEEE80211_CAPINFO_PBCC;
    if(CapInfo.ChanAgility)
        *isr_capinfo |= IEEE80211_CAPINFO_CHNL_AGILITY;
    if(CapInfo.ShortSlotTime)
        *isr_capinfo |= IEEE80211_CAPINFO_SHORT_SLOTTIME;
    if(CapInfo.DsssOfdm)
        *isr_capinfo |= IEEE80211_CAPINFO_DSSSOFDM;
    return;
}

/*
 * Convert IEEE channel number to MHz frequency.
 */
unsigned int ieee80211_ieee2mhz(unsigned int chan)
{
    if (chan > 0 && chan < 14)   /* 2GHz band */
        return 2407 + chan*5;
    else if (chan == 14)
        return 2484;
    else
        return 5000 + (chan*5);
}

int ieee80211_ioctl_getscanresults(struct net_device *netdev,char *ret_str, UINT16 *ret_len)
{
    //struct wlprivate *priv = NETDEV_PRIV_P(struct wlprivate, netdev);
    //vmacApInfo_t *vmacSta_p = priv->vmacSta_p;
    //MIB_802DOT11 *mib = vmacSta_p->ShadowMib802dot11;

    scanDescptHdr_t *curDescpt_p = NULL;
    IEEEtypes_SsIdElement_t *ssidIE_p = NULL;
    IEEEtypes_DsParamSet_t *dsPSetIE_p = NULL;
    IEEEtypes_SuppRatesElement_t *PeerSupportedRates_p = NULL;
    IEEEtypes_ExtSuppRatesElement_t *PeerExtSupportedRates_p = NULL;
    struct ieee80211req_scan_result *sr = NULL;
    UINT16 parsedLen = 0,nr = 0, nxr = 0;
    UINT8 i = 0,intval = 0;
    UINT8 ssid[14];
    struct scanreq req;
    UINT8 *cp,*buf;
    void *p;

    if(netdev == NULL||ret_str == NULL)
        return -ENXIO;

    if(NETDEV_PRIV_P(struct wlprivate, netdev)->vmacSta_p->busyScanning == 1)
    {
        WlLogPrint(MARVEL_DEBUG_DEBUG, __func__,"%s is scanning,please wait\n",netdev->name);
        return 0;
    }
    memset(&req,0,sizeof(struct scanreq));

    scan_space(&req);
    WlLogPrint(MARVEL_DEBUG_ALL, __func__,"space_size = %d scan_num = %d\n",req.space,tmpNumScanDesc);

    if (req.space > 0)
    {
        size_t space;
        space = req.space;
        p = (void *)kmalloc(space, GFP_KERNEL);
        if (p == NULL)
            return -ENOMEM;

        req.sr = p;
    }
    else
    {
        *ret_len = 0;
        return 0;
    }

    WlLogPrint(MARVEL_DEBUG_ALL, __func__,"%-14.14s  %-17.17s  %4s  %4s  %-5s %3s %4s\n"
        , "SSID"
        , "BSSID"
        , "CHAN"
        , "RATE"
        , "S:N"
        , "INT"
        , "CAPS"
    );

    for(i = 0; i < tmpNumScanDesc;i++)
    {
        curDescpt_p = (scanDescptHdr_t *)(&tmpScanResults[0] + parsedLen);
        sr = (struct ieee80211req_scan_result *)p;
        memset(sr,0,sizeof(struct ieee80211req_scan_result));
        memset(ssid,0,sizeof(ssid));
        nr = 0;
        nxr = 0;

        /*** bssid ***/
        memcpy(sr->isr_bssid, curDescpt_p->bssId, ETH_ALEN);

        /*** frequency ***/
        if ((dsPSetIE_p = (IEEEtypes_DsParamSet_t *)smeParseIeType(DS_PARAM_SET,
                                        (((UINT8 *)curDescpt_p) +sizeof(scanDescptHdr_t)),
                                         curDescpt_p->length + sizeof(curDescpt_p->length) - sizeof(scanDescptHdr_t))) != NULL)
        {
            sr->isr_freq = ieee80211_ieee2mhz(dsPSetIE_p->CurrentChan);
            sr->isr_flags = 0;
        }

        /*** quality statistics ***/
        sr->isr_rssi = curDescpt_p->rssi;
        sr->isr_noise = (__u8) le16_to_cpu(-0x95);

        /*** beacon interval ***/
        //sr->isr_intval = (UINT8)curDescpt_p->BcnInterval;
        intval = (UINT8)curDescpt_p->BcnInterval;
        sr->isr_intval = intval;
        sr->isr_erp = 1;

        /*** capinfo ***/
        getcapinfo(&sr->isr_capinfo,curDescpt_p->CapInfo);

        /*** rates ***/
        PeerSupportedRates_p = (IEEEtypes_SuppRatesElement_t*)smeParseIeType(SUPPORTED_RATES,
                                        (((UINT8 *)curDescpt_p) +sizeof(scanDescptHdr_t)),
                                           curDescpt_p->length + sizeof(curDescpt_p->length) - sizeof(scanDescptHdr_t));

        PeerExtSupportedRates_p = (IEEEtypes_ExtSuppRatesElement_t *)smeParseIeType(EXT_SUPPORTED_RATES,
                                        (((UINT8 *)curDescpt_p) + sizeof(scanDescptHdr_t)),
                                           curDescpt_p->length + sizeof(curDescpt_p->length) - sizeof(scanDescptHdr_t));
        if (PeerSupportedRates_p)
        {
            nr = min((int)PeerSupportedRates_p->Len, IEEE80211_RATE_MAXSIZE);
            memcpy(sr->isr_rates,PeerSupportedRates_p->Rates, nr);
        }
        if (PeerExtSupportedRates_p)
        {
            nxr = min((int)PeerExtSupportedRates_p->Len, IEEE80211_RATE_MAXSIZE - nr);
            memcpy(sr->isr_rates + nr, PeerExtSupportedRates_p->Rates, nxr);
        }
        sr->isr_nrates = nr + nxr;

        /*** ssid ***/
        cp = (UINT8 *)(sr+1);
        if ((ssidIE_p = (IEEEtypes_SsIdElement_t *)smeParseIeType(SSID,
                                        (((UINT8 *)curDescpt_p) +sizeof(scanDescptHdr_t)),
                                           curDescpt_p->length + sizeof(curDescpt_p->length) - sizeof(scanDescptHdr_t))) != NULL)
        {
            sr->isr_ssid_len = ssidIE_p->Len;
            memcpy(cp,ssidIE_p->SsId, sr->isr_ssid_len);
        }

        /**** pad ****/
        sr->isr_pad[0] = 0xaa;
        sr->isr_pad[1] = 0xbb;
        sr->isr_pad[2] = 0xcc;
        sr->isr_pad[3] = 0xdd;

        WlLogPrint(MARVEL_DEBUG_ALL, __func__,"%-14.*s  %02x:%02x:%02x:%02x:%02x:%02x  %3d  %3dM %2d:%-2d  %3d %-4.4d"
                , copy_essid(ssid, sizeof(ssid), cp, sr->isr_ssid_len)
                , ssid
                , sr->isr_bssid[0],sr->isr_bssid[1],sr->isr_bssid[2],sr->isr_bssid[3],sr->isr_bssid[4],sr->isr_bssid[5]
                ,sr->isr_freq 
                , getmaxrate(sr->isr_rates, sr->isr_nrates)
                , (int8_t)sr->isr_rssi, sr->isr_noise
                , sr->isr_intval
                ,sr->isr_capinfo);

        /*** IE ***/
        cp += sr->isr_ssid_len;
        sr->isr_ie_len = curDescpt_p->length + sizeof(curDescpt_p->length) - sizeof(scanDescptHdr_t);

        if(sr->isr_ie_len){
            memcpy(cp,(((UINT8 *)curDescpt_p) +sizeof(scanDescptHdr_t)),sr->isr_ie_len);
        }
        sr->isr_len = roundup(sizeof(struct ieee80211req_scan_result) + sr->isr_ssid_len + sr->isr_ie_len, sizeof(SINT32));

        parsedLen += curDescpt_p->length + sizeof(curDescpt_p->length);
        p = (struct ieee80211req_scan_result *)(((UINT8 *)p) + sr->isr_len);
    }
        
    *ret_len = req.space;
    buf = (UINT8 *)req.sr;

    if (copy_to_user(ret_str,buf,*ret_len))
    {
        kfree(req.sr);
        return -EFAULT;
    }

    kfree(req.sr);

    return 0;
}


UINT8 ieee80211_Control_Scan_Chan(vmacApInfo_t *vmacSta_p,UINT8 *mainChnlList,UINT8 *chnlScanList,UINT8 *scanType)
{
    UINT8 currChnlIndex = 0,chnlListLen = 0,i = 0;

    if((vmacSta_p == NULL)||(mainChnlList == NULL)||(chnlScanList == NULL))
        return 0;

    /********************** sta mode ***************************/
    if((*(vmacSta_p->Mib802dot11->mib_STAMode) == CLIENT_MODE_AUTO) || 
       (*(vmacSta_p->Mib802dot11->mib_STAMode) == CLIENT_MODE_N))
    {
        WlLogPrint(MARVEL_DEBUG_ALL, __func__,"STAMode = AUTO\n");

        *scanType = SCAN_ACTIVE;
        for (i = 0; i < IEEEtypes_MAX_CHANNELS; i++)
        {
            if (mainChnlList[i] > 0)
            {
                chnlScanList[currChnlIndex] = mainChnlList[i];
                currChnlIndex ++;
            }
        }

        for (i=0; i < IEEEtypes_MAX_CHANNELS_A; i++)
        {
            if (mainChnlList[i+IEEEtypes_MAX_CHANNELS] > 0)
            {
                chnlScanList[currChnlIndex] = mainChnlList[i+IEEEtypes_MAX_CHANNELS];
                currChnlIndex ++;
            }
        }
        chnlListLen = currChnlIndex;
        return chnlListLen;
    }
    else if(*(vmacSta_p->Mib802dot11->mib_STAMode) == CLIENT_MODE_N_24)
    {
        WlLogPrint(MARVEL_DEBUG_ALL, __func__,"STAMode = 2.4G\n");
        *scanType = SCAN_ACTIVE;
        for (i = 0; i < IEEEtypes_MAX_CHANNELS;i++)
        {
            chnlScanList[i] = mainChnlList[i];
        }
        chnlScanList[i] = 0;
        chnlListLen = IEEEtypes_MAX_CHANNELS;
        return chnlListLen;
    }
    else if (*(vmacSta_p->Mib802dot11->mib_STAMode) == CLIENT_MODE_N_5)
    {
        WlLogPrint(MARVEL_DEBUG_ALL, __func__,"STAMode = 5G\n");
        *scanType = SCAN_ACTIVE;
        for (i = 0; i < IEEEtypes_MAX_CHANNELS_A;i++)
        {
            chnlScanList[i] = mainChnlList[i+IEEEtypes_MAX_CHANNELS];
        }
        chnlScanList[i] = 0;
        chnlListLen = IEEEtypes_MAX_CHANNELS_A;
        return chnlListLen;
    }
    else if(*(vmacSta_p->Mib802dot11->mib_STAMode) == CLIENT_MODE_DISABLE)
    {
        /********************** ap mode *********************/
        *scanType = SCAN_PASSIVE;
        if((*(vmacSta_p->Mib802dot11->mib_ApMode) == AP_MODE_N_ONLY) || 
            (*(vmacSta_p->Mib802dot11->mib_ApMode) == AP_MODE_BandN) || 
            (*(vmacSta_p->Mib802dot11->mib_ApMode) == AP_MODE_GandN) ||
            (*(vmacSta_p->Mib802dot11->mib_ApMode) == AP_MODE_AandG) ||
            (*(vmacSta_p->Mib802dot11->mib_ApMode) == AP_MODE_AandN) ||
            (*(vmacSta_p->Mib802dot11->mib_ApMode) == AP_MODE_BandGandN))
        {
            WlLogPrint(MARVEL_DEBUG_ALL, __func__,"ApMode = AUTO\n");
            WlLogPrint(MARVEL_DEBUG_ALL, __func__,"onebyone:%d ng:%d na:%d\n",\
            iwlist_scan_onebyone,iwlist_scan_current_chan_ng,iwlist_scan_current_chan_na);
            if(iwlist_scan_onebyone == 1)
            {
                if(iwlist_scan_current_chan_ng > 0)
                {
                    for (i = 0; i < IEEEtypes_MAX_CHANNELS; i++)
                    {
                        if(mainChnlList[i] == iwlist_scan_current_chan_ng)
                        {
                            chnlScanList[currChnlIndex] = mainChnlList[i];
                            currChnlIndex++;
                            if(i+1 < IEEEtypes_MAX_CHANNELS)
                            {
                                iwlist_scan_current_chan_ng = mainChnlList[i+1];
                            }
                            else
                            {
                                iwlist_scan_current_chan_ng = 0;
                                iwlist_scan_current_chan_na = mainChnlList[IEEEtypes_MAX_CHANNELS];
                            }
                            break;
                        }
                    }
                }
                else if(iwlist_scan_current_chan_na > 0)
                {
                    for (i=0; i < IEEEtypes_MAX_CHANNELS_A; i++)
                    {
                        if (mainChnlList[i+IEEEtypes_MAX_CHANNELS] == iwlist_scan_current_chan_na)
                        {
                            chnlScanList[currChnlIndex] = mainChnlList[i+IEEEtypes_MAX_CHANNELS];
                            currChnlIndex ++;
                            if(i+1 < IEEEtypes_MAX_CHANNELS_A)
                            {
                                iwlist_scan_current_chan_na = mainChnlList[i+IEEEtypes_MAX_CHANNELS+1];
                            }
                            else
                            {
                                iwlist_scan_current_chan_na = 0;
                            }
                            break;
                        }
                    }
                }
                else
                {
                    chnlScanList[currChnlIndex] = mainChnlList[0];
                    currChnlIndex ++;
                    iwlist_scan_current_chan_ng = mainChnlList[1];
                    return currChnlIndex;
                }
                chnlListLen = currChnlIndex;
            }
            else        //onebyone == 0;
            {
                for (i = 0;i < IEEEtypes_MAX_CHANNELS;i++)
                {
                    if(mainChnlList[i] > 0)
                    {
                        chnlScanList[currChnlIndex] = mainChnlList[i];
                        currChnlIndex ++;
                    }
                }

                for (i = 0;i < IEEEtypes_MAX_CHANNELS_A;i++)
                {
                    if (mainChnlList[i+IEEEtypes_MAX_CHANNELS] > 0)
                    {
                        chnlScanList[currChnlIndex] = mainChnlList[i+IEEEtypes_MAX_CHANNELS];
                        currChnlIndex ++;
                    }
                }
                chnlListLen = currChnlIndex;
            }
            return chnlListLen;
        }
        else if((*(vmacSta_p->Mib802dot11->mib_ApMode) == AP_MODE_B_ONLY)||
            (*(vmacSta_p->Mib802dot11->mib_ApMode) == AP_MODE_G_ONLY) || 
            (*(vmacSta_p->Mib802dot11->mib_ApMode) == AP_MODE_MIXED))
        {
            WlLogPrint(MARVEL_DEBUG_ALL, __func__,"ApMode = 2.4G\n");
            WlLogPrint(MARVEL_DEBUG_ALL, __func__,"onebyone:%d ng:%d na:%d\n",\
            iwlist_scan_onebyone,iwlist_scan_current_chan_ng,iwlist_scan_current_chan_na);
            if(iwlist_scan_onebyone == 1)
            {
                if(iwlist_scan_current_chan_ng > 0)
                {
                    for (i = 0;i < IEEEtypes_MAX_CHANNELS;i++)
                    {
                        if(mainChnlList[i] == iwlist_scan_current_chan_ng)
                        {
                            chnlScanList[currChnlIndex] = mainChnlList[i];
                            currChnlIndex ++;
                            if(i+1 < IEEEtypes_MAX_CHANNELS)
                            {
                                iwlist_scan_current_chan_ng = mainChnlList[i+1];
                            }
                            else
                            {
                                iwlist_scan_current_chan_ng = 0;
                            }
                            break;
                        }
                    }
                }
                else
                {
                    chnlScanList[0] = mainChnlList[0];
                    chnlScanList[1] = 0;
                    currChnlIndex ++;
                    iwlist_scan_current_chan_ng = mainChnlList[1];
                }
                chnlListLen = currChnlIndex;
            }
            else
            {
                for (i = 0; i < IEEEtypes_MAX_CHANNELS;i++)
                {
                    chnlScanList[i] = mainChnlList[i];
                }
                chnlScanList[i] = 0;
                chnlListLen = IEEEtypes_MAX_CHANNELS;
            }
            return chnlListLen;
        }
        else if( (*(vmacSta_p->Mib802dot11->mib_ApMode) == AP_MODE_A_ONLY) ||
            (*(vmacSta_p->Mib802dot11->mib_ApMode) == AP_MODE_5GHZ_N_ONLY))
        {
            WlLogPrint(MARVEL_DEBUG_ALL, __func__,"ApMode = 5G\n");
            WlLogPrint(MARVEL_DEBUG_ALL, __func__,"onebyone:%d ng:%d na:%d\n",\
            iwlist_scan_onebyone,iwlist_scan_current_chan_ng,iwlist_scan_current_chan_na);
            if(iwlist_scan_onebyone == 1)
            {
                if(iwlist_scan_current_chan_na > 0)
                {
                    for (i = 0;i < IEEEtypes_MAX_CHANNELS_A;i++)
                    {
                        if (mainChnlList[i+IEEEtypes_MAX_CHANNELS] == iwlist_scan_current_chan_na)
                        {
                            chnlScanList[currChnlIndex] = mainChnlList[i+IEEEtypes_MAX_CHANNELS];
                            currChnlIndex ++;
                            if(i+1 < IEEEtypes_MAX_CHANNELS_A)
                            {
                                iwlist_scan_current_chan_na = mainChnlList[i+IEEEtypes_MAX_CHANNELS+1];
                            }
                            else
                            {
                                iwlist_scan_current_chan_na = 0;
                            }
                            break;
                        }
                    }
                }
                else
                {
                    chnlScanList[0] = mainChnlList[IEEEtypes_MAX_CHANNELS];
                    chnlScanList[1] = 0;
                    currChnlIndex ++;
                    iwlist_scan_current_chan_na = mainChnlList[IEEEtypes_MAX_CHANNELS+1];
                }
                chnlListLen = currChnlIndex;
            }
            else
            {
                for (i = 0;i < IEEEtypes_MAX_CHANNELS_A;i++)
                {
                    chnlScanList[i] = mainChnlList[i+IEEEtypes_MAX_CHANNELS];
                }
                chnlScanList[i] = 0;
                chnlListLen = IEEEtypes_MAX_CHANNELS_A;
            }
            return chnlListLen;
        }
    }

    return chnlListLen;
}


UINT8 ieee80211_Control_Get_Support_Chan(vmacApInfo_t *vmacSta_p,UINT8 *mainChnlList,UINT8 *chnlScanList)
{
    UINT8 currChnlIndex = 0,chnlListLen = 0,i = 0;

    if((vmacSta_p == NULL)||(mainChnlList == NULL)||(chnlScanList == NULL))
        return 0;

    /********************** sta mode ***************************/
    if((*(vmacSta_p->Mib802dot11->mib_STAMode) == CLIENT_MODE_AUTO) || 
    (*(vmacSta_p->Mib802dot11->mib_STAMode) == CLIENT_MODE_N))
    {
        WlLogPrint(MARVEL_DEBUG_ALL, __func__,"STAMode = AUTO\n");

        for (i = 0; i < IEEEtypes_MAX_CHANNELS; i++)
        {
            if (mainChnlList[i] > 0)
            {
                chnlScanList[currChnlIndex] = mainChnlList[i];
                currChnlIndex ++;
            }
        }

        for (i=0; i < IEEEtypes_MAX_CHANNELS_A; i++)
        {
            if (mainChnlList[i+IEEEtypes_MAX_CHANNELS] > 0)
            {
                chnlScanList[currChnlIndex] = mainChnlList[i+IEEEtypes_MAX_CHANNELS];
                currChnlIndex ++;
            }
        }
        chnlListLen = currChnlIndex;
        return chnlListLen;
    }
    else if(*(vmacSta_p->Mib802dot11->mib_STAMode) == CLIENT_MODE_N_24)
    {
        WlLogPrint(MARVEL_DEBUG_ALL, __func__,"STAMode = 2.4G\n");
        for (i = 0; i < IEEEtypes_MAX_CHANNELS;i++)
        {
            chnlScanList[i] = mainChnlList[i];
        }
        chnlScanList[i] = 0;
        chnlListLen = IEEEtypes_MAX_CHANNELS;
        return chnlListLen;
    }
    else if (*(vmacSta_p->Mib802dot11->mib_STAMode) == CLIENT_MODE_N_5)
    {
        WlLogPrint(MARVEL_DEBUG_ALL, __func__,"STAMode = 5G\n");
        for (i = 0; i < IEEEtypes_MAX_CHANNELS_A;i++)
        {
            chnlScanList[i] = mainChnlList[i+IEEEtypes_MAX_CHANNELS];
        }
        chnlScanList[i] = 0;
        chnlListLen = IEEEtypes_MAX_CHANNELS_A;
        return chnlListLen;
    }
    else if(*(vmacSta_p->Mib802dot11->mib_STAMode) == CLIENT_MODE_DISABLE)
    {
        /********************** ap mode *********************/
        if((*(vmacSta_p->Mib802dot11->mib_ApMode) == AP_MODE_N_ONLY) || 
            (*(vmacSta_p->Mib802dot11->mib_ApMode) == AP_MODE_BandN) || 
            (*(vmacSta_p->Mib802dot11->mib_ApMode) == AP_MODE_GandN) ||
            (*(vmacSta_p->Mib802dot11->mib_ApMode) == AP_MODE_AandG) ||
            (*(vmacSta_p->Mib802dot11->mib_ApMode) == AP_MODE_AandN) ||
            (*(vmacSta_p->Mib802dot11->mib_ApMode) == AP_MODE_BandGandN))
        {
            WlLogPrint(MARVEL_DEBUG_ALL, __func__,"ApMode = AUTO\n");

            for (i = 0;i < IEEEtypes_MAX_CHANNELS;i++)
            {
                if(mainChnlList[i] > 0)
                {
                    chnlScanList[currChnlIndex] = mainChnlList[i];
                    currChnlIndex ++;
                }
            }

            for (i = 0;i < IEEEtypes_MAX_CHANNELS_A;i++)
            {
                if (mainChnlList[i+IEEEtypes_MAX_CHANNELS] > 0)
                {
                    chnlScanList[currChnlIndex] = mainChnlList[i+IEEEtypes_MAX_CHANNELS];
                    currChnlIndex ++;
                }
            }
            chnlListLen = currChnlIndex;

            return chnlListLen;
        }
        else if((*(vmacSta_p->Mib802dot11->mib_ApMode) == AP_MODE_B_ONLY)||
            (*(vmacSta_p->Mib802dot11->mib_ApMode) == AP_MODE_G_ONLY) || 
            (*(vmacSta_p->Mib802dot11->mib_ApMode) == AP_MODE_MIXED))
        {
            WlLogPrint(MARVEL_DEBUG_ALL, __func__,"ApMode = 2.4G\n");

            for (i = 0; i < IEEEtypes_MAX_CHANNELS;i++)
            {
                chnlScanList[i] = mainChnlList[i];
            }
            chnlScanList[i] = 0;
            chnlListLen = IEEEtypes_MAX_CHANNELS;
            return chnlListLen;
        }
        else if( (*(vmacSta_p->Mib802dot11->mib_ApMode) == AP_MODE_A_ONLY) ||
            (*(vmacSta_p->Mib802dot11->mib_ApMode) == AP_MODE_5GHZ_N_ONLY))
        {
            WlLogPrint(MARVEL_DEBUG_ALL, __func__,"ApMode = 5G\n");

            for (i = 0;i < IEEEtypes_MAX_CHANNELS_A;i++) 
            {
                chnlScanList[i] = mainChnlList[i+IEEEtypes_MAX_CHANNELS];
            }
            chnlScanList[i] = 0;
            chnlListLen = IEEEtypes_MAX_CHANNELS_A;

            return chnlListLen;
        }
    }

    return chnlListLen;
}


