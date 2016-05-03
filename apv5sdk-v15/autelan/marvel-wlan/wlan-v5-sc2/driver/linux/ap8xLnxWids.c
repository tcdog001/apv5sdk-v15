/*
*                Copyright 2005, Autelan, Inc.
*
* author : guijinlin
* date    : 2012-06-12
*/
#include <linux/wireless.h>
#include <net/iw_handler.h>

#include "ap8xLnxIntf.h"
#include "ap8xLnxDesc.h"
#include "buildModes.h"
#include "wltypes.h"
#include "IEEE_types.h"
#include "mib.h"
#include "wl_macros.h"
#include "ds.h"
#include "keyMgmtCommon.h"
#include "keyMgmt.h"
#include "timer.h"
#include "tkip.h"
#include "StaDb.h"
#include "macmgmtap.h"
#include "macMgmtMlme.h"
#include "wldebug.h"
#include "wl_hal.h"
#include "IEEE_types.h"
#include "ap8xLnxWids.h"
     
/*pengruofeng add for wids 2011-5-30*/
int clear_illegaldev_history(vmacApInfo_t *vap )
{
    struct wids_detect_attacker_entry *next = NULL;
    unsigned long flag;
    
    WIDS_LOCK_IRQ(&(vap->iv_sta), flag);
    if (((vap->OpMode == WL_OP_MODE_AP) || (vap->OpMode == WL_OP_MODE_VAP)) && vap->flood_detect){
        LIST_FOREACH(next, &vap->iv_sta.attacker, attacker) {
            if(next->attack_cnt != 0){
                next->attack_cnt = 0;
            }
        }
    }
    WIDS_UNLOCK_IRQ(&(vap->iv_sta), flag);
    
    return 0;
}


int clear_attack_history(vmacApInfo_t *vap )
{
    struct wids_detect_attacker_entry *next = NULL;
    unsigned long flag;
    
    WIDS_LOCK_IRQ(&(vap->iv_sta), flag);
    if (((vap->OpMode == WL_OP_MODE_AP) || (vap->OpMode == WL_OP_MODE_VAP)) && vap->flood_detect){
        for (next = LIST_FIRST(&vap->iv_sta.attacker); next;
             next = LIST_FIRST(&vap->iv_sta.attacker)) {
            LIST_REMOVE(next, attacker);
            kfree(next);
        }
    }
    WIDS_UNLOCK_IRQ(&(vap->iv_sta), flag);

    return 0;
}


int clear_attack_stat_history(vmacApInfo_t *vap )
{
    if (((vap->OpMode == WL_OP_MODE_AP) || (vap->OpMode == WL_OP_MODE_VAP))&&vap->flood_detect){
        (vap->iv_sta).attack_assoc_req_cnt=0;
        (vap->iv_sta).attack_auth_req_cnt=0;
        (vap->iv_sta).attack_deauth_cnt=0;
        (vap->iv_sta).attack_disassoc_req_cnt=0;
        (vap->iv_sta).attack_null_data_cnt=0;
        (vap->iv_sta).attack_probe_req_cnt=0;
        (vap->iv_sta).attack_reassoc_req_cnt=0;  
        (vap->iv_sta).attack_action_cnt=0;   
    }

    return 0;
}

/*pengruofeng add for wids alarm 2011-5-30*/
static void
ieee80211_tx_wids_alarm(unsigned long arg)
{
    vmacApInfo_t *vmacSta_p = (vmacApInfo_t *)arg;
    MIB_802DOT11 *mib = vmacSta_p->ShadowMib802dot11;
    MIB_PHY_DSSS_TABLE *PhyDSSSTable=mib->PhyDSSSTable;
    struct net_device *dev = vmacSta_p->dev;
    unsigned long flag;
    
    if((vmacSta_p->OpMode == WL_OP_MODE_AP) || (vmacSta_p->OpMode == WL_OP_MODE_VAP)){
        union iwreq_data wrq;
        struct wids_detect_attacker_entry *next = NULL;
        char buf[100],buf_sta[80];
        
        memset(buf, 0, 100);
        memset(buf_sta, 0, 80);
        memset(&wrq, 0, sizeof(wrq));
        
        
        WIDS_LOCK_IRQ(&(vmacSta_p->iv_sta), flag);
        if(vmacSta_p->flood_detect == 1){
            next = NULL;
            LIST_FOREACH(next, &vmacSta_p->iv_sta.attacker, attacker) {
                if (next->assoc_req_cnt > vmacSta_p->attack_max_cnt) {
                    printk("--------------%s--------assoc:%d , max:%lu\n",__func__,next->assoc_req_cnt,vmacSta_p->attack_max_cnt);//pengruofeng_debug
                    buf[0]=IEEE80211_WIDS_FLOOD_DETECT;
                    memcpy(buf+1,next->mac,IEEEtypes_ADDRESS_SIZE);
                    memcpy(buf+IEEEtypes_ADDRESS_SIZE+1,vmacSta_p->VMacEntry.vmacAddr,IEEEtypes_ADDRESS_SIZE);
                    buf[13]=IEEE80211_WIDS_FASSOC_REQUEST;
                    buf[14]=PhyDSSSTable->CurrChan;
                    buf[15]=next->rssi;
                    wrq.data.length = 16;
                    wireless_send_event(dev, IWEVWIDSALERM, &wrq, buf);
                    if((vmacSta_p->iv_sta).attack_assoc_req_cnt < 1000)
                        (vmacSta_p->iv_sta).attack_assoc_req_cnt++;
                    if(next->attack_cnt < 1000)
                        next->attack_cnt++;
                }

                if (next->auth_req_cnt > vmacSta_p->attack_max_cnt) {
                    buf[0]=IEEE80211_WIDS_FLOOD_DETECT;
                    memcpy(buf+1,next->mac,IEEEtypes_ADDRESS_SIZE);
                    memcpy(buf+IEEEtypes_ADDRESS_SIZE+1,vmacSta_p->VMacEntry.vmacAddr,IEEEtypes_ADDRESS_SIZE);
                    buf[13]=IEEE80211_WIDS_FAUTH_REQUEST;
                    buf[14]=PhyDSSSTable->CurrChan;
                    buf[15]=next->rssi;
                    wrq.data.length = 16;
                    wireless_send_event(dev, IWEVWIDSALERM, &wrq, buf);
                    if((vmacSta_p->iv_sta).attack_auth_req_cnt < 1000 )
                        (vmacSta_p->iv_sta).attack_auth_req_cnt++;
        
                    if(next->attack_cnt < 1000)
                        next->attack_cnt++;
                }

                if (next->reassoc_req_cnt > vmacSta_p->attack_max_cnt) {
                    buf[0]=IEEE80211_WIDS_FLOOD_DETECT;
                    memcpy(buf+1,next->mac,IEEEtypes_ADDRESS_SIZE);
                    memcpy(buf+IEEEtypes_ADDRESS_SIZE+1,vmacSta_p->VMacEntry.vmacAddr,IEEEtypes_ADDRESS_SIZE);
                    buf[13]=IEEE80211_WIDS_FREASSOC_REQUEST;
                    buf[14]=PhyDSSSTable->CurrChan;
                    buf[15]=next->rssi;
                    wrq.data.length = 16;
                    wireless_send_event(dev, IWEVWIDSALERM, &wrq, buf);
                    if((vmacSta_p->iv_sta).attack_reassoc_req_cnt < 1000 )
                        (vmacSta_p->iv_sta).attack_reassoc_req_cnt++;
                    if(next->attack_cnt < 1000)
                        next->attack_cnt++;
                }

                if (next->deauth_cnt > vmacSta_p->attack_max_cnt) {
                    buf[0]=IEEE80211_WIDS_FLOOD_DETECT;
                    memcpy(buf+1,next->mac,IEEEtypes_ADDRESS_SIZE);
                    memcpy(buf+IEEEtypes_ADDRESS_SIZE+1,vmacSta_p->VMacEntry.vmacAddr,IEEEtypes_ADDRESS_SIZE);
                    buf[13]=IEEE80211_WIDS_FDEAUTH_REQUEST;
                    buf[14]=PhyDSSSTable->CurrChan;
                    buf[15]=next->rssi;
                    wrq.data.length = 16;
                    wireless_send_event(dev, IWEVWIDSALERM, &wrq, buf);
                    if((vmacSta_p->iv_sta).attack_deauth_cnt < 1000 )
                        (vmacSta_p->iv_sta).attack_deauth_cnt++;
                    if(next->attack_cnt < 1000)
                        next->attack_cnt++;
                }

                if (next->disassoc_req_cnt > vmacSta_p->attack_max_cnt) {
                    buf[0]=IEEE80211_WIDS_FLOOD_DETECT;
                    memcpy(buf+1,next->mac,IEEEtypes_ADDRESS_SIZE);
                    memcpy(buf+IEEEtypes_ADDRESS_SIZE+1,vmacSta_p->VMacEntry.vmacAddr,IEEEtypes_ADDRESS_SIZE);
                    buf[13]=IEEE80211_WIDS_FDISASSO_REQUEST;
                    buf[14]=PhyDSSSTable->CurrChan;
                    buf[15]=next->rssi; 
                    wrq.data.length = 16;
                    wireless_send_event(dev, IWEVWIDSALERM, &wrq, buf);
                    if((vmacSta_p->iv_sta).attack_disassoc_req_cnt < 1000 )
                        (vmacSta_p->iv_sta).attack_disassoc_req_cnt++;
                    if(next->attack_cnt < 1000)
                        next->attack_cnt++;
                }

                if (next->null_data_cnt > vmacSta_p->attack_max_cnt) {
                    buf[0]=IEEE80211_WIDS_FLOOD_DETECT;
                    memcpy(buf+1,next->mac,IEEEtypes_ADDRESS_SIZE);
                    memcpy(buf+IEEEtypes_ADDRESS_SIZE+1,vmacSta_p->VMacEntry.vmacAddr,IEEEtypes_ADDRESS_SIZE);
                    buf[13]=IEEE80211_WIDS_FNULL_DATA;
                    buf[14]=PhyDSSSTable->CurrChan;
                    buf[15]=next->rssi;
                    wrq.data.length = 16;
                    wireless_send_event(dev, IWEVWIDSALERM, &wrq, buf);
                    if((vmacSta_p->iv_sta).attack_null_data_cnt < 1000 )
                        (vmacSta_p->iv_sta).attack_null_data_cnt++;
                    if(next->attack_cnt < 1000)
                        next->attack_cnt++;
                }

                if (next->probe_req_cnt > vmacSta_p->attack_max_cnt && next->probe_req_cnt > vmacSta_p->probe_attack_max_cnt) {
                    buf[0]=IEEE80211_WIDS_FLOOD_DETECT;
                    memcpy(buf+1,next->mac,IEEEtypes_ADDRESS_SIZE);
                    memcpy(buf+IEEEtypes_ADDRESS_SIZE+1,vmacSta_p->VMacEntry.vmacAddr,IEEEtypes_ADDRESS_SIZE);
                    buf[13]=IEEE80211_WIDS_FPROBE_REQUEST;
                    buf[14]=PhyDSSSTable->CurrChan;
                    buf[15]=next->rssi;
                    wrq.data.length = 16;
                    wireless_send_event(dev, IWEVWIDSALERM, &wrq, buf);
                    if((vmacSta_p->iv_sta).attack_probe_req_cnt < 1000 )
                        (vmacSta_p->iv_sta).attack_probe_req_cnt++;
                    if(next->attack_cnt < 1000)
                        next->attack_cnt++;
                }

                if (next->action_cnt > vmacSta_p->attack_max_cnt ) {
                    buf[0]=IEEE80211_WIDS_FLOOD_DETECT;
                    memcpy(buf+1,next->mac,IEEEtypes_ADDRESS_SIZE);
                    memcpy(buf+IEEEtypes_ADDRESS_SIZE+1,vmacSta_p->VMacEntry.vmacAddr,IEEEtypes_ADDRESS_SIZE);
                    buf[13]=IEEE80211_WIDS_FACTION;
                    buf[14]=PhyDSSSTable->CurrChan;
                    buf[15]=next->rssi;
                    wrq.data.length = 16;
                    wireless_send_event(dev, IWEVWIDSALERM, &wrq, buf);
                    if((vmacSta_p->iv_sta).attack_action_cnt < 1000 )
                        (vmacSta_p->iv_sta).attack_action_cnt++;
                    if(next->attack_cnt < 1000)
                        next->attack_cnt++;
                }
                next->auth_req_cnt=0;
                next->reassoc_req_cnt=0;
                next->deauth_cnt=0;
                next->disassoc_req_cnt=0;
                next->null_data_cnt=0;
                next->probe_req_cnt=0;
                next->assoc_req_cnt=0;
                next->action_cnt=0;
                
                if((vmacSta_p->iv_sta).attacker_list_node_cnt >= 50 && !next->attack_cnt){
                    LIST_REMOVE(next, attacker);
                    kfree(next);
                    (vmacSta_p->iv_sta).attacker_list_node_cnt--;
                }
            }
            
            if((vmacSta_p->iv_sta).attacker_list_node_cnt >= 50)
            {
                for (next = LIST_FIRST(&vmacSta_p->iv_sta.attacker); next;
                     next = LIST_FIRST(&vmacSta_p->iv_sta.attacker)) {
                    LIST_REMOVE(next, attacker);
                    kfree(next);
                }
                (vmacSta_p->iv_sta).attacker_list_node_cnt=0;
            }
            
        }

        if(vmacSta_p->spoof_detect == 1){
            next = NULL;
            LIST_FOREACH(next, &vmacSta_p->iv_sta.attacker, attacker) {
                if (next->spoof_deauth_cnt) {
                    printk("--------------%s--------spoof_deauth_cnt:%d\n",__func__,next->spoof_deauth_cnt);//pengruofeng_debug
                    buf[0]=IEEE80211_WIDS_SPOOF_DETECT;
                    memcpy(buf+1,next->mac,IEEEtypes_ADDRESS_SIZE);
                    memcpy(buf+IEEEtypes_ADDRESS_SIZE+1,vmacSta_p->VMacEntry.vmacAddr,IEEEtypes_ADDRESS_SIZE);
                    buf[13]=IEEE80211_WIDS_SPOOF_DEAUTH;
                    buf[14]=PhyDSSSTable->CurrChan;
                    buf[15]=next->rssi;
                    wrq.data.length = 16;
                    wireless_send_event(dev, IWEVWIDSALERM, &wrq, buf);
                    if((vmacSta_p->iv_sta).attack_spoof_deauth_cnt < 1000)
                        (vmacSta_p->iv_sta).attack_spoof_deauth_cnt++;
                    if(next->attack_cnt < 1000)
                        next->attack_cnt++;
                }
                
                if (next->spoof_disassoc_cnt) {
                    printk("--------------%s--------spoof_disassoc_cnt:%d\n",__func__,next->spoof_disassoc_cnt);//pengruofeng_debug
                    buf[0]=IEEE80211_WIDS_SPOOF_DETECT;
                    memcpy(buf+1,next->mac,IEEEtypes_ADDRESS_SIZE);
                    memcpy(buf+IEEEtypes_ADDRESS_SIZE+1,vmacSta_p->VMacEntry.vmacAddr,IEEEtypes_ADDRESS_SIZE);
                    buf[13]=IEEE80211_WIDS_SPOOF_DISASSOC;
                    buf[14]=PhyDSSSTable->CurrChan;
                    buf[15]=next->rssi;
                    wrq.data.length = 16;
                    wireless_send_event(dev, IWEVWIDSALERM, &wrq, buf);
                    if((vmacSta_p->iv_sta).attack_spoof_disassoc_cnt < 1000)
                        (vmacSta_p->iv_sta).attack_spoof_disassoc_cnt++;
                    if(next->attack_cnt < 1000)
                        next->attack_cnt++;
                }
                next->spoof_deauth_cnt=0;
                next->spoof_disassoc_cnt=0;
                
                if((vmacSta_p->iv_sta).attacker_list_node_cnt >= 50 && !next->attack_cnt){
                    LIST_REMOVE(next, attacker);
                    kfree(next);
                    (vmacSta_p->iv_sta).attacker_list_node_cnt--;
                }
                
            }
            
            if((vmacSta_p->iv_sta).attacker_list_node_cnt >= 50)
            {
                for (next = LIST_FIRST(&vmacSta_p->iv_sta.attacker); next;
                     next = LIST_FIRST(&vmacSta_p->iv_sta.attacker)) {
                    LIST_REMOVE(next, attacker);
                    kfree(next);
                }
                (vmacSta_p->iv_sta).attacker_list_node_cnt=0;
            }
            
        }

        if(vmacSta_p->weakiv_detect == 1){
            next = NULL;
            LIST_FOREACH(next, &vmacSta_p->iv_sta.attacker, attacker) {
                if (next->weakiv_cnt) {
                    buf[0]=IEEE80211_WIDS_WEAKIV_DETECT;
                    memcpy(buf+1,next->mac,IEEEtypes_ADDRESS_SIZE);
                    memcpy(buf+IEEEtypes_ADDRESS_SIZE+1,vmacSta_p->VMacEntry.vmacAddr,IEEEtypes_ADDRESS_SIZE);
                    buf[13]=IEEE80211_WIDS_WEAKIV_DETECT;
                    buf[14]=PhyDSSSTable->CurrChan;
                    buf[15]=next->rssi;
                    wrq.data.length = 16;
                    wireless_send_event(dev, IWEVWIDSALERM, &wrq, buf);
                    if((vmacSta_p->iv_sta).attack_weakiv_cnt < 1000)
                        (vmacSta_p->iv_sta).attack_weakiv_cnt++;
                    if(next->attack_cnt < 1000)
                        next->attack_cnt++;
                }
                next->weakiv_cnt=0;
                
                if((vmacSta_p->iv_sta).attacker_list_node_cnt >= 50 && !next->attack_cnt){
                    LIST_REMOVE(next, attacker);
                    kfree(next);
                    (vmacSta_p->iv_sta).attacker_list_node_cnt--;
                }
                
            }
            
            if((vmacSta_p->iv_sta).attacker_list_node_cnt >= 50)
            {
                for (next = LIST_FIRST(&vmacSta_p->iv_sta.attacker); next;
                     next = LIST_FIRST(&vmacSta_p->iv_sta.attacker)) {
                    LIST_REMOVE(next, attacker);
                    kfree(next);
                }
                (vmacSta_p->iv_sta).attacker_list_node_cnt=0;
            }
            
        }
        WIDS_UNLOCK_IRQ(&(vmacSta_p->iv_sta), flag);
                
        /*ljy--modified begin for the interval of wids detect*/
        //mod_timer(&(vmacSta_p->iv_widsalarm), jiffies + msecs_to_jiffies(1000));
        mod_timer(&(vmacSta_p->iv_widsalarm), jiffies + msecs_to_jiffies(vmacSta_p->wids_interval));
        /*ljy--modified end*/        
    }
}
/*pengruofeng add end */

int wids_init(vmacApInfo_t *vap)
{
    /* pengruofeng add for wids 2011-5-30*/        
    if((vap->OpMode == WL_OP_MODE_AP) || (vap->OpMode == WL_OP_MODE_VAP)) {
        init_timer(&(vap->iv_widsalarm));
        vap->iv_widsalarm.function = ieee80211_tx_wids_alarm;
        vap->iv_widsalarm.data = (unsigned long) vap;
        vap->wids_interval = 1000;
        mod_timer(&(vap->iv_widsalarm), jiffies + msecs_to_jiffies(vap->wids_interval));
                
        WIDS_LOCK_INIT(&(vap->iv_sta), "wids");
                
        LIST_INIT(&vap->iv_sta.attacker);
        (vap->iv_sta).attacker_entry = NULL;
        (vap->iv_sta).attack_assoc_req_cnt=0;
        (vap->iv_sta).attack_auth_req_cnt=0;
        (vap->iv_sta).attack_deauth_cnt=0;
        (vap->iv_sta).attack_disassoc_req_cnt=0;
        (vap->iv_sta).attack_null_data_cnt=0;
        (vap->iv_sta).attack_probe_req_cnt=0;
        (vap->iv_sta).attack_reassoc_req_cnt=0;
        (vap->iv_sta).attack_action_cnt=0;
        (vap->iv_sta).attack_spoof_deauth_cnt=0;
        (vap->iv_sta).attack_spoof_disassoc_cnt=0;
        (vap->iv_sta).attack_weakiv_cnt=0;
        (vap->iv_sta).attacker_list_node_cnt=0;
        vap->attack_max_cnt = 30;
        vap->probe_attack_max_cnt = 0;
            
    }
    /* pengruofeng add end 2011-5-30*/

    return 0;
}

void disable_wids_timer(vmacApInfo_t *vap)
{
    //MIB_802DOT11 *mib = vmacSta_p->Mib802dot11;
    del_timer(&(vap->iv_widsalarm));
    vap->iv_widsalarm.function = NULL;
    return;
}

/*pengruofeng add end 2011-5-30*/
int attack_detection(vmacApInfo_t *vap, IEEEtypes_Frame_t *wh, void *arg)
{
    struct wids_detect_attacker_entry *match = NULL, *next = NULL;
    unsigned long flag;
    
    if((vap->OpMode == WL_OP_MODE_AP) || (vap->OpMode == WL_OP_MODE_VAP))
    {   
        if(vap->spoof_detect == 1)
        {
            if(wh->Hdr.FrmCtl.Type == IEEE_TYPE_MANAGEMENT)
            {
                switch(wh->Hdr.FrmCtl.Subtype)
                {
                    case IEEE_MSG_DEAUTHENTICATE:
                    case IEEE_MSG_DISASSOCIATE:
                        if (IEEE80211_IS_MULTICAST(wh->Hdr.Addr1) || IEEE80211_IS_MULTICAST(wh->Hdr.Addr2) || IEEE80211_IS_MULTICAST(wh->Hdr.Addr3))
                        {
                            WIDS_LOCK_IRQ(&(vap->iv_sta), flag);
                            
                            LIST_FOREACH(next, &vap->iv_sta.attacker, attacker) {
                                if (!memcmp(next->mac, wh->Hdr.Addr2, IEEEtypes_ADDRESS_SIZE)) {
                                    match = next;
                                    match->rssi=(int)arg;
                                    break;
                                }
                            }
                    
                            if(match == NULL){
                                WIDS_UNLOCK_IRQ(&(vap->iv_sta), flag);
                                match= kmalloc(sizeof(*match), GFP_KERNEL);
                                if (match == NULL)
                                    panic("sta_add: can't malloc memory");
                                
                                match->assoc_req_cnt=0;
                                match->auth_req_cnt=0;
                                match->deauth_cnt=0;
                                match->disassoc_req_cnt=0;
                                match->null_data_cnt=0;
                                match->probe_req_cnt=0;
                                match->reassoc_req_cnt=0;
                                match->action_cnt=0;
                                match->attack_cnt=0;
                                match->rssi=(int)arg;
                                match->spoof_deauth_cnt=0;
                                match->spoof_disassoc_cnt=0;
                                match->weakiv_cnt=0;
                                memcpy(match->mac,wh->Hdr.Addr2,IEEEtypes_ADDRESS_SIZE);
                                WIDS_LOCK_IRQ(&(vap->iv_sta), flag);
                                LIST_INSERT_HEAD(&vap->iv_sta.attacker, match, attacker);
                                (vap->iv_sta).attacker_list_node_cnt++;
                            }

                            if(wh->Hdr.FrmCtl.Subtype == IEEE_MSG_DEAUTHENTICATE)
                                match->spoof_deauth_cnt++;
                            else
                                match->spoof_disassoc_cnt++;
                    
                            WIDS_UNLOCK_IRQ(&(vap->iv_sta), flag);
                            
                            printk("%s: (m)deauth spoofing.\n", __func__);
                        }                       
                        break;
                        
                    default:
                        break;
                }
            }           
        }

        if(vap->flood_detect)
        {
            if(wh->Hdr.FrmCtl.Type == IEEE_TYPE_MANAGEMENT)
            {
                switch(wh->Hdr.FrmCtl.Subtype)
                {
                    case IEEE_MSG_PROBE_RQST:
                    case IEEE_MSG_AUTHENTICATE:
                    case IEEE_MSG_ASSOCIATE_RQST:
                    case IEEE_MSG_REASSOCIATE_RQST:
                    case IEEE_MSG_DEAUTHENTICATE:
                    case IEEE_MSG_DISASSOCIATE:
                    case IEEE_MSG_ACTION:

                    if(match == NULL) {
                        WIDS_LOCK_IRQ(&(vap->iv_sta), flag);
                        LIST_FOREACH(next, &vap->iv_sta.attacker, attacker) {
                            if (!memcmp(next->mac, wh->Hdr.Addr2, IEEEtypes_ADDRESS_SIZE)) {
                                match = next;
                                match->rssi=(int)arg;
                                break;
                            }
                        }
                        if(match == NULL){
                            WIDS_UNLOCK_IRQ(&(vap->iv_sta), flag);
                            match= kmalloc(sizeof(*match), GFP_KERNEL);
                            if (match == NULL)
                                panic("sta_add: can't malloc memory");
                            
                            match->assoc_req_cnt=0;
                            match->auth_req_cnt=0;
                            match->deauth_cnt=0;
                            match->disassoc_req_cnt=0;
                            match->null_data_cnt=0;
                            match->probe_req_cnt=0;
                            match->reassoc_req_cnt=0;
                            match->action_cnt=0;
                            match->attack_cnt=0;
                            match->rssi=(int)arg;
                            match->spoof_deauth_cnt=0;
                            match->spoof_disassoc_cnt=0;
                            match->weakiv_cnt=0;
                            memcpy(match->mac,wh->Hdr.Addr2,IEEEtypes_ADDRESS_SIZE);
                            WIDS_LOCK_IRQ(&(vap->iv_sta), flag);
                            LIST_INSERT_HEAD(&vap->iv_sta.attacker, match, attacker);
                            (vap->iv_sta).attacker_list_node_cnt++;
                        }                       
                    }
                    else
                    {
                        WIDS_LOCK_IRQ(&(vap->iv_sta), flag);
                    }

                    if(wh->Hdr.FrmCtl.Subtype == IEEE_MSG_PROBE_RQST)
                        match->probe_req_cnt++;
                    else if(wh->Hdr.FrmCtl.Subtype == IEEE_MSG_AUTHENTICATE)
                        match->auth_req_cnt++;
                    else if(wh->Hdr.FrmCtl.Subtype == IEEE_MSG_ASSOCIATE_RQST)
                        match->assoc_req_cnt++;
                    else if(wh->Hdr.FrmCtl.Subtype == IEEE_MSG_REASSOCIATE_RQST)
                        match->reassoc_req_cnt++;
                    else if(wh->Hdr.FrmCtl.Subtype == IEEE_MSG_DEAUTHENTICATE)
                        match->deauth_cnt++;
                    else if(wh->Hdr.FrmCtl.Subtype == IEEE_MSG_DISASSOCIATE)
                        match->disassoc_req_cnt++;
                    else if(wh->Hdr.FrmCtl.Subtype == IEEE_MSG_ACTION)
                        match->action_cnt++;    
                    
                    WIDS_UNLOCK_IRQ(&(vap->iv_sta), flag);
                    break;

                    default:
                        break;                  
                }
            }
            else if(wh->Hdr.FrmCtl.Type == IEEE_TYPE_DATA)
            {
                if(wh->Hdr.FrmCtl.Subtype == NULL_DATA)
                {
                    if(match == NULL) {
                        WIDS_LOCK_IRQ(&(vap->iv_sta), flag);
                        LIST_FOREACH(next, &vap->iv_sta.attacker, attacker) {
                            if (!memcmp(next->mac, wh->Hdr.Addr2, IEEEtypes_ADDRESS_SIZE)) {
                                match = next;
                                match->rssi = (int)arg;
                                break;
                            }
                        }

                        if(match == NULL){
                            WIDS_UNLOCK_IRQ(&(vap->iv_sta), flag);
                            match= kmalloc(sizeof(*match), GFP_KERNEL);
                            if (match == NULL)
                                panic("sta_add: can't malloc memory");
                            
                            match->assoc_req_cnt=0;
                            match->auth_req_cnt=0;
                            match->deauth_cnt=0;
                            match->disassoc_req_cnt=0;
                            match->null_data_cnt=0;
                            match->probe_req_cnt=0;
                            match->reassoc_req_cnt=0;
                            match->action_cnt=0;
                            match->attack_cnt=0;
                            match->rssi=(int)arg;
                            match->spoof_deauth_cnt=0;
                            match->spoof_disassoc_cnt=0;
                            match->weakiv_cnt=0;
                            memcpy(match->mac,wh->Hdr.Addr2,IEEEtypes_ADDRESS_SIZE);
                            WIDS_LOCK_IRQ(&(vap->iv_sta), flag);
                            LIST_INSERT_HEAD(&vap->iv_sta.attacker, match, attacker);
                            (vap->iv_sta).attacker_list_node_cnt++;
                        }
                    }
                    else
                    {
                        WIDS_LOCK_IRQ(&(vap->iv_sta), flag);
                    }
                    
                    match->null_data_cnt++;
                    WIDS_UNLOCK_IRQ(&(vap->iv_sta), flag);
                }
            }
        }   

        if (vap->weakiv_detect)
        {
            WlLogPrint(MARVEL_DEBUG_ALL, __func__,"weak IV detection not supported");
        }
    }

    return 0;
}

