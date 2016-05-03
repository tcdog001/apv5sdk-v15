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
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/net.h>
#include <linux/netdevice.h>
#include <linux/wireless.h>
#include <linux/pci.h>
#include <linux/delay.h>
#include <linux/string.h>

#include <net/iw_handler.h>
#include <asm/processor.h>
#include <asm/uaccess.h>

#include "wl.h" 
#include "wldebug.h"
#include "ap8xLnxApi.h"
#include "ap8xLnxVer.h"
#include "IEEE_types.h"
#include "wl_mib.h"
#include "wl_hal.h"
#include "ap8xLnxFwcmd.h"
#include "ap8xLnxWlLog.h"
#include "wlApi.h"
#include "qos.h"
#include "ap8xLnxIoctl.h"
#include "ap8xLnxFwdl.h"
#include "StaDb.h"
#include "domain.h" // Added by ARUN to support 802.11d
#include "wlvmac.h"
#include "macmgmtap.h"
#include "macMgmtMlme.h"
#include "idList.h"
#include "keyMgmtSta.h"
#include "bcngen.h"
#include "wlFun.h"
#ifdef EWB
#include "ewb_hash.h"
#endif

#ifdef WDS_FEATURE
#include "wds.h"
#endif

#ifdef CLIENT_SUPPORT
#include "linkmgt.h"
#include "mlme.h"
#include "mlmeApi.h"
#endif

#ifdef MPRXY
#include "ap8xLnxMPrxy.h"
#endif

#include "ap8xLnxIntf.h"
#include "ap8xLnxIoctlWiress.h"
#include "wllog.h"
#include "ap8xLnxThinap.h"

extern u_int32_t thinap;
static IEEEtypes_MacAddr_t bcastMacAddr = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

#define is_null_addr(_a)            \
                ((_a)[0] == 0x00 &&             \
                 (_a)[1] == 0x00 &&             \
                 (_a)[2] == 0x00 &&             \
                 (_a)[3] == 0x00 &&             \
                 (_a)[4] == 0x00 &&             \
                 (_a)[5] == 0x00)

int ieee80211_ioctl_setmlme(struct net_device *dev, struct iw_request_info *info,
    void *w, char *extra)
{
    struct ieee80211req_mlme *mlme = (struct ieee80211req_mlme *)extra;
    struct wlprivate *wlpptr = NETDEV_PRIV_P(struct wlprivate, dev);
    vmacApInfo_t *vmacSta_p = wlpptr->vmacSta_p;
    //MIB_802DOT11 *mib = vmacSta_p->ShadowMib802dot11;
    //MIB_802DOT11 *mib1 = vmacSta_p->Mib802dot11;
    extStaDb_StaInfo_t *pStaInfo=NULL;
    //osif_dev  *osifp = ath_netdev_priv(dev);
    //wlan_if_t vap = osifp->os_if;
#ifdef UMAC_SUPPORT_SMARTANTENNA    
    //struct ieee80211_node *ni = NULL;
#endif    
    //int force_scan = 0;

    WlLogPrint(MARVEL_DEBUG_ALL, __func__, "devname:%s setmlme", dev->name) ;
    if (NULL == vmacSta_p)
    {
        return -EINVAL;
    }

    pStaInfo = extStaDb_GetStaInfo(vmacSta_p,(IEEEtypes_MacAddr_t *)mlme->im_macaddr, 1);
    if (NULL == pStaInfo)
    {
        return -EINVAL;
    }
    
    if (!(dev->flags & IFF_UP)) 
    {
        printk(" DEVICE IS DOWN ifname=%s\n", dev->name);
        return -EINVAL;     /* XXX */
    }

    switch (mlme->im_op) 
    {
        case IEEE80211_MLME_ASSOC:
        {
            WlLogPrint(MARVEL_DEBUG_ALL, __func__, "os_opmode=%d IEEE80211_MLME_ASSOC\n", pStaInfo->AP);
            /* set dessired bssid when in STA mode accordingly */
            break;
        }
        case IEEE80211_MLME_DISASSOC:
        case IEEE80211_MLME_DEAUTH:
        {
            WlLogPrint(MARVEL_DEBUG_DEBUG, __func__, "IEEE80211_MLME_DISASSOC or IEEE80211_MLME_DEAUTH\n");
            macMgmtMlme_SendDeauthenticateMsg(vmacSta_p,&(pStaInfo->Addr),  0, IEEEtypes_REASON_CLASS3_NONASSOC);
            break;
        }
        case IEEE80211_MLME_AUTHORIZE:
        {
            WlLogPrint(MARVEL_DEBUG_DEBUG, __func__, "IEEE80211_MLME_AUTHORIZE\n");
            ieee80211_node_authorize(pStaInfo,vmacSta_p);
            WLSNDEVT(dev,IWEVCUSTOM,(IEEEtypes_MacAddr_t *)&pStaInfo->Addr, WLSYSLOG_MSG_PSK_GRPKEY_SUCCESS);
            break;
        }
        case IEEE80211_MLME_UNAUTHORIZE:
        {
            WlLogPrint(MARVEL_DEBUG_DEBUG, __func__, "IEEE80211_MLME_UNAUTHORIZE\n");
            ieee80211_node_unauthorize(pStaInfo);
            break;
        }
        case IEEE80211_MLME_CLEAR_STATS:
        {
            WlLogPrint(MARVEL_DEBUG_DEBUG, __func__, "IEEE80211_MLME_CLEAR_STATS\n");
            break;
        }
        case IEEE80211_MLME_STOP_BSS:
        {
            WlLogPrint(MARVEL_DEBUG_DEBUG, __func__, "IEEE80211_MLME_STOP_BSS\n");
            break;
        }
        default:
        {
            return -EINVAL;
        }
    }
    return 0;
}

int ieee80211_ioctl_autelan_mac_binding(struct net_device *dev, struct iwreq *iwr)
{
    struct wlprivate *priv = NULL;
    vmacApInfo_t *vmacSta_p = NULL;
    extStaDb_StaInfo_t *pStaInfo = NULL;
    struct ieee80211_autelan_mac_binding ik;

    if (NULL == dev || NULL == iwr)
    {
        return -EFAULT;
    }
        
    priv = NETDEV_PRIV_P(struct wlprivate, dev);
    vmacSta_p = priv->vmacSta_p;

    memset(&ik, 0x00, sizeof(ik));

    if (copy_from_user(&ik, iwr->u.data.pointer, sizeof(ik))) 
    {        
        return -EFAULT;
    }

    switch (ik.type) 
    {
        case SET_DHCP_ENABLE:
        {
            priv->vap.vap_dhcp_enable = ik.arg1;
            break;
        }
        case GET_DHCP_ENABLE:
        {
            if (copy_to_user(iwr->u.data.pointer, &(priv->vap.vap_dhcp_enable), sizeof(priv->vap.vap_dhcp_enable)))
            {
                return -EFAULT;
            }
            break;
        }
        case SET_IP_AUTO_LEARNING:
        {
            priv->vap.vap_ip_auto_learning = ik.arg1;
            break;
        }
        case GET_IP_AUTO_LEARNING:
        {
            if (copy_to_user(iwr->u.data.pointer, &(priv->vap.vap_ip_auto_learning), sizeof(priv->vap.vap_ip_auto_learning)))
            {
                return -EFAULT;
            }
            break;
        }
        case SET_IP:
        {
            pStaInfo = extStaDb_GetStaInfo(vmacSta_p, (IEEEtypes_MacAddr_t *)ik.macaddr, 0);
            if (pStaInfo == NULL)
                return -EINVAL; 
            memcpy(pStaInfo->ni_ipaddr, ik.ipaddr, sizeof(pStaInfo->ni_ipaddr));
            break;
        }
        case GET_IP:
        {
            pStaInfo = extStaDb_GetStaInfo(vmacSta_p, (IEEEtypes_MacAddr_t *)ik.macaddr, 0);
            if (pStaInfo == NULL)
                return -EINVAL; 
            memcpy(ik.ipaddr, pStaInfo->ni_ipaddr, sizeof(pStaInfo->ni_ipaddr));
            if (copy_to_user(iwr->u.data.pointer, &ik, sizeof(ik)))
            {
                return -EFAULT;
            }
            break;
        }
        case SET_VLAN:
        {
            pStaInfo = extStaDb_GetStaInfo(vmacSta_p, (IEEEtypes_MacAddr_t *)ik.macaddr, 0);
            if (pStaInfo == NULL)
                return -EINVAL;
            if ((ik.arg1 >= 0) && (ik.arg1 <= 0xfff))
                pStaInfo->ni_vid = ik.arg1;
            else
                return -EINVAL;
            break;
        }
        case  GET_VLAN:
        {
            pStaInfo = extStaDb_GetStaInfo(vmacSta_p, (IEEEtypes_MacAddr_t *)ik.macaddr, 0);
            if (pStaInfo == NULL)
                return -EINVAL; 
            if (copy_to_user(iwr->u.data.pointer, &(pStaInfo->ni_vid), sizeof(pStaInfo->ni_vid)))
            {
                return -EFAULT;
            }
            break;
        }
        //sheyanbin--add to through the pppoe frame
        case SET_PPPOE_ENABLE:
        {
            priv->vap.vap_pppoe_enable = ik.arg1;
            break;
        }
        case GET_PPPOE_ENABLE:
        {
            if (copy_to_user(iwr->u.data.pointer, &(priv->vap.vap_pppoe_enable), sizeof(priv->vap.vap_pppoe_enable)))
            {
                return -EFAULT;
            }
            break;
        }
        //sheyanbin--add end
        default:
        {
            return -EFAULT;
        }
    }
    return 0;
}

int ieee80211_ioctl_setkey(struct net_device *dev, struct iw_request_info *info,
                           void *w, char *extra)
{
#ifndef CLIENT_SUPPORT
    #define GetParentStaBSSID(x) NULL
#endif
    struct ieee80211req_key *wk = (struct ieee80211req_key *)extra;
    extStaDb_StaInfo_t *pStaInfo=NULL;
#ifdef CLIENT_SUPPORT
    vmacEntry_t  *vmacEntry_p = NULL;
    STA_SECURITY_MIBS * pStaSecurityMibs = NULL;
    keyMgmtInfoSta_t  *pKeyMgmtInfoSta = NULL;
#endif
    struct wlprivate *priv = NULL;
    vmacApInfo_t *vmacSta_p = NULL;
    MIB_802DOT11 *mib = NULL;
    MIB_802DOT11 *mib1 = NULL;
    int rc = 0;
    //int debug_i = 0;
    int ret_value = 0;

    int index = 0;
    int inx   = 0;
    UCHAR tmpWEPKey[16];

    if (NULL == dev || NULL == wk)
    {
        return -EFAULT;
    }
    
    priv = NETDEV_PRIV_P(struct wlprivate, dev);
    vmacSta_p = priv->vmacSta_p;
    mib = vmacSta_p->ShadowMib802dot11;
    mib1 = vmacSta_p->Mib802dot11;

    WlLogPrint(MARVEL_DEBUG_ALL, __func__, "wk->ik_keyix:%d dev->name:%s\n", wk->ik_keyix, dev->name);

#ifdef MRVL_WPS_CLIENT
    if( vmacSta_p->VMacEntry.modeOfService == VMAC_MODE_CLNT_INFRA )
    {
        if( (vmacEntry_p = sme_GetParentVMacEntry(vmacSta_p->VMacEntry.phyHwMacIndx)) == NULL)
        {
            rc = -EFAULT;
            return rc;
        }
    }

    if( vmacSta_p->VMacEntry.modeOfService == VMAC_MODE_CLNT_INFRA )
    {
        pStaSecurityMibs = sme_GetStaSecurityMibsPtr(vmacEntry_p);
        pKeyMgmtInfoSta = sme_GetKeyMgmtInfoStaPtr(vmacEntry_p);
        if( pStaSecurityMibs == NULL || pKeyMgmtInfoSta == NULL )
        {
            WlLogPrint(MARVEL_DEBUG_ALL, __func__, "pStaSecurityMibs NULL or pKeyMgmtInfoSta NULL\n");
            rc = -EINVAL;
            return rc;
        }
    }
#endif //MRVL_WPS_CLIENT

    if (wk->ik_keyix == WL_KEYIX_NONE)
    {
        WlLogPrint(MARVEL_DEBUG_ALL, __func__, "\nMAC:%02x-%02x-%02x-%02x-%02x-%02x flag:%d ik_type:%d ik_pad:%d ik_keyix:%d ik_keylen:%d ik_keyrsc:%lld ik_keytsc:%lld\n", 
                        wk->ik_macaddr[0], wk->ik_macaddr[1], wk->ik_macaddr[2], 
                        wk->ik_macaddr[3], wk->ik_macaddr[4], wk->ik_macaddr[5], wk->ik_flags, wk->ik_type, wk->ik_pad, wk->ik_keyix, wk->ik_keylen ,wk->ik_keyrsc, wk->ik_keytsc);
        if (wk->ik_type == WL_IEEE80211_WAPI_PSK)
        {
            int gkey=0;
            struct wlreq_wapi_key wapi_key;

            memset(&wapi_key, 0, sizeof(struct wlreq_wapi_key));

            /* for mcst key, use bssid to replace bcast MAC */
            if ( memcmp(wk->ik_macaddr, bcastMacAddr, 6) == 0 ) {
                memcpy(wapi_key.ik_macaddr, vmacSta_p->macBssId, 6);
                gkey=1;
            }
            else
            {
                memcpy(wapi_key.ik_macaddr, wk->ik_macaddr, 6);
            }

            memcpy(wapi_key.ik_keydata, wk->ik_keydata, 32);
            wapi_key.ik_flags = 1;
            wapi_key.ik_keyid = 0;
            wlPrintData(DBG_LEVEL_1|DBG_CLASS_DATA,  __FUNCTION__, wapi_key.ik_keydata, 32, NULL);

            /* appie->appBuf = wapid's pbData: MAC + 1 + Keyindex + key + key-mic */
            if( vmacSta_p->VMacEntry.modeOfService == VMAC_MODE_AP )
            {
                wlFwSetWapiKey(dev, &wapi_key, gkey);
            }

        #ifdef MRVL_WAPI
            if (mib1->Privacy->WAPIEnabled)
            {
                pStaInfo = extStaDb_GetStaInfo(vmacSta_p,
                            vmacSta_p->VMacEntry.modeOfService == VMAC_MODE_CLNT_INFRA ?
                            (IEEEtypes_MacAddr_t *)GetParentStaBSSID((vmacEntry_p)->phyHwMacIndx) :
                            (IEEEtypes_MacAddr_t *)wapi_key.ik_macaddr, 
                            1);
                if (pStaInfo != NULL && vmacSta_p != NULL)
                {
                    ieee80211_node_authorize(pStaInfo,vmacSta_p);
                    //WLSNDEVT(dev,IWEVCUSTOM,(IEEEtypes_MacAddr_t *)&pStaInfo->Addr, WLSYSLOG_MSG_PSK_SUCCESS);
                }
            }
        #endif
            
        #ifdef CLIENT_SUPPORT
            /* to do */
        #endif
        }
        
/*************** zhouke add start ,for 8021x + WEP key set********************/
        else if(wk->ik_type == WL_IEEE80211_CIPHER_WEP)
        {       
            WlLogPrint(MARVEL_DEBUG_ALL, __func__, "WL_IEEE80211_CIPHER_WEP\n");

            if ((pStaInfo = extStaDb_GetStaInfo(
                vmacSta_p,
                vmacSta_p->VMacEntry.modeOfService == VMAC_MODE_CLNT_INFRA ?
                (IEEEtypes_MacAddr_t *)GetParentStaBSSID((vmacEntry_p)->phyHwMacIndx) :
                (IEEEtypes_MacAddr_t *)wk->ik_macaddr, 
                1)) == NULL)
            {
                rc = -EOPNOTSUPP;
                return rc;
            }


            *(mib1->mib_defaultkeyindex) = index;            

            memset(mib1->WepDefaultKeys[index].WepDefaultKeyValue, 0, 13);
            memcpy(tmpWEPKey, wk->ik_keydata, wk->ik_keylen);
            memcpy(mib1->WepDefaultKeys[index].WepDefaultKeyValue, tmpWEPKey, wk->ik_keylen);

            if ( ! WL_FUN_SetWEPKey((void *)priv, index, 1, tmpWEPKey))
            {
                rc = -EIO;
            }
                
            //wlFwSetNewStn(dev,pStaInfo->Addr, 0, 0, 0, NULL,0,0);  //add new station
            wlFwSetSecurity(dev,pStaInfo->Addr);
            
            for(inx = 0; inx <4 ; inx ++)
            {
            WlLogPrint(MARVEL_DEBUG_ALL, __func__,"wep key = %x %x %x %x %x %x %x %x %x %x %x %x %x \n",
                    mib1->WepDefaultKeys[inx].WepDefaultKeyValue[0],
                    mib1->WepDefaultKeys[inx].WepDefaultKeyValue[1],
                    mib1->WepDefaultKeys[inx].WepDefaultKeyValue[2],
                    mib1->WepDefaultKeys[inx].WepDefaultKeyValue[3],
                    mib1->WepDefaultKeys[inx].WepDefaultKeyValue[4],
                    mib1->WepDefaultKeys[inx].WepDefaultKeyValue[5],
                    mib1->WepDefaultKeys[inx].WepDefaultKeyValue[6],
                    mib1->WepDefaultKeys[inx].WepDefaultKeyValue[7],
                    mib1->WepDefaultKeys[inx].WepDefaultKeyValue[8],
                    mib1->WepDefaultKeys[inx].WepDefaultKeyValue[9],
                    mib1->WepDefaultKeys[inx].WepDefaultKeyValue[10],
                    mib1->WepDefaultKeys[inx].WepDefaultKeyValue[11],
                    mib1->WepDefaultKeys[inx].WepDefaultKeyValue[12]);
            }
        }
/********************** zhouke add end **********************************/

        else
        {        
            if (extStaDb_SetRSNPwkAndDataTraffic(vmacSta_p,
                vmacSta_p->VMacEntry.modeOfService == VMAC_MODE_CLNT_INFRA ? 
                (IEEEtypes_MacAddr_t *)GetParentStaBSSID((vmacEntry_p)->phyHwMacIndx) : 
                (IEEEtypes_MacAddr_t *)wk->ik_macaddr, 
                &wk->ik_keydata[0],
                (UINT32*)&wk->ik_keydata[16],
                (UINT32*)&wk->ik_keydata[24]) != STATE_SUCCESS)
            {
                rc = -EOPNOTSUPP;
                return rc;
            }
            if (extStaDb_SetPairwiseTSC(vmacSta_p,
                vmacSta_p->VMacEntry.modeOfService == VMAC_MODE_CLNT_INFRA ?
                (IEEEtypes_MacAddr_t *)GetParentStaBSSID((vmacEntry_p)->phyHwMacIndx) :
                (IEEEtypes_MacAddr_t *)wk->ik_macaddr, 
                0, 0x0001) != STATE_SUCCESS)
            {
                rc = -EOPNOTSUPP;
                return rc;
            }

            if ((pStaInfo = extStaDb_GetStaInfo(
                vmacSta_p,
                vmacSta_p->VMacEntry.modeOfService == VMAC_MODE_CLNT_INFRA ?
                (IEEEtypes_MacAddr_t *)GetParentStaBSSID((vmacEntry_p)->phyHwMacIndx) :
                (IEEEtypes_MacAddr_t *)wk->ik_macaddr, 
                1)) == NULL)
            {
                rc = -EOPNOTSUPP;
                return rc;
            }

            //extStaDb_SetRSNDataTrafficEnabled(vmacSta_p,(IEEEtypes_MacAddr_t *)wk->ik_macaddr, FALSE);
#ifdef CLIENT_SUPPORT
            if( vmacSta_p->VMacEntry.modeOfService == VMAC_MODE_CLNT_INFRA)
            {
                if (!pStaSecurityMibs->mib_RSNConfigWPA2_p->WPA2OnlyEnabled
                    && !pStaSecurityMibs->mib_RSNConfigWPA2_p->WPA2Enabled)
                {
                    //WPA
                    AddRSN_IE_TO(pStaSecurityMibs->thisStaRsnIE_p,(IEEEtypes_RSN_IE_t*)(&pStaInfo->keyMgmtStateInfo.RsnIEBuf[0]) );
                    if (pStaSecurityMibs->mib_RSNConfigUnicastCiphers_p->UnicastCipher[3] == 2)
                    {
                        // TKIP
                        wlFwSetWpaTkipMode_STA(dev,  (UINT8 *)&pStaInfo->Addr);
                    }
                    else if ((pStaSecurityMibs->mib_RSNConfigUnicastCiphers_p->UnicastCipher[3] == 4))
                    {
                        // AES
                        wlFwSetWpaAesMode_STA(dev,  (UINT8 *)&pStaInfo->Addr);
                    }
                }
                else
                {
                    // WPA2
                    AddRSN_IEWPA2_TO(pStaSecurityMibs->thisStaRsnIEWPA2_p,(IEEEtypes_RSN_IE_WPA2_t*)(&pStaInfo->keyMgmtStateInfo.RsnIEBuf[0]) );
                    if (pStaSecurityMibs->mib_RSNConfigWPA2UnicastCiphers_p->UnicastCipher[3] == 4)
                    {
                        // AES
                        wlFwSetWpaAesMode_STA(dev,  (UINT8 *)&pStaInfo->Addr);
                    }
                    else
                    {
                        // TKIP
                        //Not sure if this is correct setting for firmware in this case????
                        wlFwSetMixedWpaWpa2Mode_STA(dev,  (UINT8 *)&pStaInfo->Addr);
                    }
                }

                wlFwSetWpaWpa2PWK_STA(dev, pStaInfo);
                if( pKeyMgmtInfoSta )
                    pKeyMgmtInfoSta->pKeyData->RSNDataTrafficEnabled = 1 ;
                if( pStaInfo )
                    pStaInfo->keyMgmtStateInfo.RSNDataTrafficEnabled = 1 ;
            }
            else
#endif 
            {
                if( vmacSta_p->VMacEntry.modeOfService == VMAC_MODE_AP )
                {
                    ret_value = wlFwSetWpaWpa2PWK(dev, pStaInfo);
                    WlLogPrint(MARVEL_DEBUG_ALL, __func__, "WL_KEYIX_NONE VMAC_MODE_AP ret_value:%d\n", ret_value);
                    //WLSNDEVT(dev,IWEVCUSTOM, wk->ik_macaddr, WLSYSLOG_MSG_PSK_SUCCESS);
                }
            }
        }       
    }
    else if ((0 < wk->ik_keyix) &&  (wk->ik_keyix< 4))
    {
        WlLogPrint(MARVEL_DEBUG_ALL, __func__, "\nMAC:%02x-%02x-%02x-%02x-%02x-%02x flag:%d ik_type:%d ik_pad:%d ik_keyix:%d ik_keylen:%d ik_keyrsc:%lld ik_keytsc:%lld\n", 
                        wk->ik_macaddr[0], wk->ik_macaddr[1], wk->ik_macaddr[2], 
                        wk->ik_macaddr[3], wk->ik_macaddr[4], wk->ik_macaddr[5], wk->ik_flags, wk->ik_type, wk->ik_pad, wk->ik_keyix, wk->ik_keylen ,wk->ik_keyrsc, wk->ik_keytsc);
        //if (wk->ik_type == WL_CIPHER_TKIP)
        if (wk->ik_type == WL_IEEE80211_CIPHRASE_TKIP)
        {
            WlLogPrint(MARVEL_DEBUG_ALL, __func__, "WL_CIPHER_TKIP\n");
            if( vmacSta_p->VMacEntry.modeOfService == VMAC_MODE_AP )
            {
                WlLogPrint(MARVEL_DEBUG_ALL, __func__, "WL_CIPHER_TKIP VMAC_MODE_AP\n");
                memcpy(mib1->mib_MrvlRSN_GrpKey->EncryptKey, &wk->ik_keydata[0], 16);
                memcpy(mib1->mib_MrvlRSN_GrpKey->TxMICKey, &wk->ik_keydata[16], 8);
                memcpy(mib1->mib_MrvlRSN_GrpKey->RxMICKey, &wk->ik_keydata[24], 8);
                mib1->mib_MrvlRSN_GrpKey->g_IV16 = 0x0001;
                mib1->mib_MrvlRSN_GrpKey->g_IV32 = 0;
                mib1->mib_MrvlRSN_GrpKey->g_KeyIndex = 1/*(UINT8)wk->ik_keyix*/;
                ret_value = wlFwSetWpaTkipGroupK(dev, mib1->mib_MrvlRSN_GrpKey->g_KeyIndex);
                //need to update shadow mib, when directly modify run-time mib.
                memcpy(mib->mib_MrvlRSN_GrpKey, 
                    mib1->mib_MrvlRSN_GrpKey, sizeof(MRVL_MIB_RSN_GRP_KEY));
                WlLogPrint(MARVEL_DEBUG_ALL, __func__, "mib1->mib_MrvlRSN_GrpKey->g_KeyIndex:%d ret_value:%d\n", mib1->mib_MrvlRSN_GrpKey->g_KeyIndex, ret_value);
            }
#ifdef CLIENT_SUPPORT
            else if( vmacSta_p->VMacEntry.modeOfService == VMAC_MODE_CLNT_INFRA )
            {
                ENCR_TKIPSEQCNT TkipTsc;
                WlLogPrint(MARVEL_DEBUG_ALL, __func__, "WL_CIPHER_TKIP VMAC_MODE_CLNT_INFRA\n");

                memcpy( mib_MrvlRSN_GrpKeyUr1[vmacEntry_p->phyHwMacIndx].EncryptKey,
                    &wk->ik_keydata[0], TK_SIZE );
                memcpy(mib_MrvlRSN_GrpKeyUr1[vmacEntry_p->phyHwMacIndx].TxMICKey, 
                    &wk->ik_keydata[16], 8 );
                memcpy(mib_MrvlRSN_GrpKeyUr1[vmacEntry_p->phyHwMacIndx].RxMICKey, 
                    &wk->ik_keydata[24], 8);
                mib_MrvlRSN_GrpKeyUr1[vmacEntry_p->phyHwMacIndx].g_IV16 = 0x0001;
                mib_MrvlRSN_GrpKeyUr1[vmacEntry_p->phyHwMacIndx].g_IV32 = 0;

                TkipTsc.low = mib_MrvlRSN_GrpKeyUr1[vmacEntry_p->phyHwMacIndx].g_IV16;
                TkipTsc.high = mib_MrvlRSN_GrpKeyUr1[vmacEntry_p->phyHwMacIndx].g_IV32;

                wlFwSetWpaTkipGroupK_STA(dev,
                    GetParentStaBSSID(vmacEntry_p->phyHwMacIndx),
                    &mib_MrvlRSN_GrpKeyUr1[vmacEntry_p->phyHwMacIndx].EncryptKey[0],
                    TK_SIZE,
                    (UINT8 *)&mib_MrvlRSN_GrpKeyUr1[vmacEntry_p->phyHwMacIndx].RxMICKey,
                    MIC_KEY_LENGTH,
                    (UINT8 *)&mib_MrvlRSN_GrpKeyUr1[vmacEntry_p->phyHwMacIndx].TxMICKey,
                    MIC_KEY_LENGTH,
                    TkipTsc, wk->ik_keyix);

                if( pKeyMgmtInfoSta )
                    pKeyMgmtInfoSta->pKeyData->RSNSecured = 1 ;
            }
#endif
        }
        //else if (wk->ik_type == WL_CIPHER_CCMP)
        else if (wk->ik_type == WL_IEEE80211_CIPHRASE_AES_CCM)
        {
            WlLogPrint(MARVEL_DEBUG_ALL, __func__, "WL_CIPHER_CCMP\n");
            if( vmacSta_p->VMacEntry.modeOfService == VMAC_MODE_AP )
            {
                WlLogPrint(MARVEL_DEBUG_ALL, __func__, "WL_CIPHER_CCMP VMAC_MODE_AP\n");
                memcpy(mib1->mib_MrvlRSN_GrpKey->EncryptKey, &wk->ik_keydata[0], 16);
                mib1->mib_MrvlRSN_GrpKey->g_KeyIndex = (UINT8)wk->ik_keyix;
                wlFwSetWpaAesGroupK(dev, mib1->mib_MrvlRSN_GrpKey->g_KeyIndex);
                //need to update shadow mib, when directly modify run-time mib.
                memcpy(mib->mib_MrvlRSN_GrpKey, 
                    mib1->mib_MrvlRSN_GrpKey, sizeof(MRVL_MIB_RSN_GRP_KEY));
            }
#ifdef CLIENT_SUPPORT
            else if( vmacSta_p->VMacEntry.modeOfService == VMAC_MODE_CLNT_INFRA )
            {
                WlLogPrint(MARVEL_DEBUG_ALL, __func__, "WL_CIPHER_CCMP VMAC_MODE_CLNT_INFRA\n");
                memcpy( mib_MrvlRSN_GrpKeyUr1[vmacEntry_p->phyHwMacIndx].EncryptKey,
                    &wk->ik_keydata[0], TK_SIZE );
                wlFwSetWpaAesGroupK_STA(dev,
                    GetParentStaBSSID(vmacEntry_p->phyHwMacIndx),
                    &mib_MrvlRSN_GrpKeyUr1[vmacEntry_p->phyHwMacIndx].EncryptKey[0],
                    TK_SIZE);
            }
#endif
        }

/********************** zhouke add start **********************************/
        else if(wk->ik_type == WL_IEEE80211_CIPHER_WEP)
        {
        #if 0
            WlLogPrint(MARVEL_DEBUG_ALL, __func__, "WL_IEEE80211_CIPHER_WEP GROUP\n");

        mib1->Privacy->RSNEnabled = 0;
        mib1->RSNConfigWPA2->WPA2Enabled = 0;
        mib1->RSNConfigWPA2->WPA2OnlyEnabled = 0;

        mib1->AuthAlg->Enable = 1;
        mib1->StationConfig->PrivOption = 1;
        mib1->Privacy->PrivInvoked = 1;
        if (WL_FUN_SetPrivacyOption((void *)priv, 1))
        {
        }else
            rc = -EIO;
        int index = wk->ik_keyix;
            int wep_type = 1;
            UCHAR tmpWEPKey[16];

    
        *(mib1->mib_defaultkeyindex) = index;
        mib1->AuthAlg->Type = 0;

            if(wk->ik_keylen== 5)
            {
                wep_type =1;
                mib1->WepDefaultKeys[index].WepType = wep_type;
            }
            else if(wk->ik_keylen == 13 )
            {
                wep_type = 2;
                mib1->WepDefaultKeys[index].WepType = wep_type;
            }
                
            memset(mib1->WepDefaultKeys[index].WepDefaultKeyValue, 0, 13);
            memcpy(tmpWEPKey, wk->ik_keydata, wk->ik_keylen);
            memcpy(mib1->WepDefaultKeys[index].WepDefaultKeyValue, tmpWEPKey, wk->ik_keylen);

            if (WL_FUN_SetWEPKey((void *)priv, index, wep_type, tmpWEPKey))
            {
                WlLogPrint(MARVEL_DEBUG_ALL, __func__,"wlset_encode: WL_FUN_SetWEPKey TRUE length = %d index = %d type = %d\n", wk->ik_keylen, index, wep_type);
                WlLogPrint(MARVEL_DEBUG_ALL, __func__,"wep key = %x %x %x %x %x %x %x %x %x %x %x %x %x \n",
                    mib1->WepDefaultKeys[index].WepDefaultKeyValue[0],
                    mib1->WepDefaultKeys[index].WepDefaultKeyValue[1],
                    mib1->WepDefaultKeys[index].WepDefaultKeyValue[2],
                    mib1->WepDefaultKeys[index].WepDefaultKeyValue[3],
                    mib1->WepDefaultKeys[index].WepDefaultKeyValue[4],
                    mib1->WepDefaultKeys[index].WepDefaultKeyValue[5],
                    mib1->WepDefaultKeys[index].WepDefaultKeyValue[6],
                    mib1->WepDefaultKeys[index].WepDefaultKeyValue[7],
                    mib1->WepDefaultKeys[index].WepDefaultKeyValue[8],
                    mib1->WepDefaultKeys[index].WepDefaultKeyValue[9],
                    mib1->WepDefaultKeys[index].WepDefaultKeyValue[10],
                    mib1->WepDefaultKeys[index].WepDefaultKeyValue[11],
                    mib1->WepDefaultKeys[index].WepDefaultKeyValue[12]);

            }else
                rc = -EIO;

            wlFwSetNewStn(dev,vmacSta_p->macStaAddr, 0, 0, 0, NULL,0,0);  //add new station
            wlFwSetSecurity(dev,vmacSta_p->macStaAddr);


            int inx = 0;
            for(inx = 0; inx <4 ; inx ++)
            {
            WlLogPrint(MARVEL_DEBUG_ALL, __func__,"~~~~~group wep key = %x %x %x %x %x %x %x %x %x %x %x %x %x \n",
                    mib1->WepDefaultKeys[inx].WepDefaultKeyValue[0],
                    mib1->WepDefaultKeys[inx].WepDefaultKeyValue[1],
                    mib1->WepDefaultKeys[inx].WepDefaultKeyValue[2],
                    mib1->WepDefaultKeys[inx].WepDefaultKeyValue[3],
                    mib1->WepDefaultKeys[inx].WepDefaultKeyValue[4],
                    mib1->WepDefaultKeys[inx].WepDefaultKeyValue[5],
                    mib1->WepDefaultKeys[inx].WepDefaultKeyValue[6],
                    mib1->WepDefaultKeys[inx].WepDefaultKeyValue[7],
                    mib1->WepDefaultKeys[inx].WepDefaultKeyValue[8],
                    mib1->WepDefaultKeys[inx].WepDefaultKeyValue[9],
                    mib1->WepDefaultKeys[inx].WepDefaultKeyValue[10],
                    mib1->WepDefaultKeys[inx].WepDefaultKeyValue[11],
                    mib1->WepDefaultKeys[inx].WepDefaultKeyValue[12]);
            }
        #endif    
        }

/********************** zhouke add end **********************************/
        else
        {
            WlLogPrint(MARVEL_DEBUG_ALL, __func__, "Unknown......\n");
            rc = -ENOTSUPP;
        }
    }
    else if (0 == wk->ik_keyix) //WAPI
    {
        int gkey=0;
        struct wlreq_wapi_key wapi_key;

        memset(&wapi_key, 0, sizeof(struct wlreq_wapi_key));

        /* for mcst key, use bssid to replace bcast MAC */
        if (memcmp(wk->ik_macaddr, bcastMacAddr, 6) == 0 ) 
        {
            memcpy(wapi_key.ik_macaddr, vmacSta_p->macBssId, 6);
            gkey=1;
        }
        else
        {
            memcpy(wapi_key.ik_macaddr, wk->ik_macaddr, 6);
        }

        memcpy(wapi_key.ik_keydata, wk->ik_keydata, 32);
        wapi_key.ik_flags = 1;
        wapi_key.ik_keyid = 0;
        wlPrintData(DBG_LEVEL_1|DBG_CLASS_DATA,  __FUNCTION__, wapi_key.ik_keydata, 32, NULL);

        /* appie->appBuf = wapid's pbData: MAC + 1 + Keyindex + key + key-mic */
        if( vmacSta_p->VMacEntry.modeOfService == VMAC_MODE_AP )
        {
            wlFwSetWapiKey(dev, &wapi_key, gkey);
        }
#ifdef CLIENT_SUPPORT
        /* to do */
#endif
    }
    else
    {
        WlLogPrint(MARVEL_DEBUG_ALL, __func__, "Error......\n");
        rc = -ENOTSUPP;
    }

    return rc;
}

int wlan_get_rsn_cipher_param(struct ieee80211_rsnparms *rsn, ieee80211_rsn_param type)
{
    int ret = 0;

    if (NULL == rsn)
    {
        return -1;
    }

    switch (type) 
    {
        case IEEE80211_UCAST_CIPHER_LEN:
        {
            ret = rsn->rsn_ucastkeylen;
            break;
        }
        case IEEE80211_MCAST_CIPHER_LEN:
        {
            ret = rsn->rsn_mcastkeylen;
            break;
        }
        case IEEE80211_KEYMGT_ALGS:
        {
            ret = rsn->rsn_keymgmtset;
            break;
        }
        case IEEE80211_RSN_CAPS:
        {
            ret = rsn->rsn_caps;
            break;
        }
        default:
        {
            ret = -EINVAL;
            WlLogPrint(MARVEL_DEBUG_ALL, __func__, "unsupported rsn parameter\n");
            break;
        }
    }   
    
    return ret;
}

int wlan_set_rsn_cipher_param(struct ieee80211_rsnparms *rsn, ieee80211_rsn_param type, int value)
{
    if (NULL == rsn)
    {
        return -1;
    }
    
    switch (type) 
    {
        case IEEE80211_UCAST_CIPHER_LEN:
        {
            rsn->rsn_ucastkeylen = value;
            break;
        }            
        case IEEE80211_MCAST_CIPHER_LEN:
        {
            rsn->rsn_mcastkeylen = value;
            break;
        }            
        case IEEE80211_KEYMGT_ALGS:
        {
            rsn->rsn_keymgmtset = value;
            break;
        }            
        case IEEE80211_RSN_CAPS:
        {
            rsn->rsn_caps = value;
            break;
        }            
        default:
        {
            WlLogPrint(MARVEL_DEBUG_ALL, __func__, "unsupported rsn parameter\n");
            return -EINVAL;
        }
    }

    return 0;
}

