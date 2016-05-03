/*
 *
 * Copyright (c) 2009 Atheros Communications Inc.
 * All rights reserved.
 *
 */

#include "includes.h"

#include "common.h"
#include "wireless_copy.h"
#include "wpa.h"
#include "eloop.h"
#include "wpa_supplicant.h"
#include "config.h"
#include "l2_packet.h"
#include "wpa_supplicant_i.h"
#include "ctrl_iface.h"
#include "version.h"
#include "wpa_ctrl.h"
#include "wapi.h"

struct wpa_supplicant *g_wpa_s;

void
wapi_supplicant_deinit()
{
    WIFI_lib_exit();
    g_wpa_s = NULL;
}

void
wapi_supplicant_deauthenticate(struct wpa_supplicant *wpa_s,
                               int reason_code)
{
    /* TODO: Reset SM of WAPI here, need check about later*/
    u8 bssid[ETH_ALEN];
    MAC_ADDRESS bssid_s;
    MAC_ADDRESS own_s;

    wpa_drv_get_bssid(wpa_s, bssid);
    memset(&bssid_s, 0, sizeof(bssid_s));
    memset(&own_s, 0, sizeof(own_s));

    memcpy(bssid_s.v, bssid, sizeof(bssid_s.v));
    memcpy(own_s.v, wpa_s->own_addr, sizeof(own_s.v));
    WIFI_WAI_IE_set(NULL, 0);
    WAI_Msg_Input(CONN_DISASSOC, &bssid_s, &own_s, NULL, 0);
    wpa_printf(MSG_DEBUG, "WAI_Msg_Input  success");
}

void
wapi_supplicant_rx_wai(void *ctx, const u8 *src_addr, const u8 *buf, size_t len)
{
    WAI_RX_packets_indication(buf, len);
}

struct wpa_supplicant *
wapi_supplicant_init(struct wpa_supplicant *wpa_s)
{
    g_wpa_s = wpa_s;

    for (;;) {
        wpa_s->l2_wapi = l2_packet_init(
                                        wpa_s->ifname,
                                        wpa_drv_get_mac_addr(wpa_s),
                                        ETH_P_WAI,
                                        wapi_supplicant_rx_wai,
                                        wpa_s,
                                        0);
        wpa_printf(MSG_ERROR, "%s: Init WAI packet %s\n", __func__,
                   wpa_s->ifname);
        if (wpa_s->l2_wapi) {
            break;
        }
        sleep(5);
    }

    if (l2_packet_get_own_addr(wpa_s->l2_wapi, wpa_s->own_addr)) {
        fprintf(stderr, "Failed to get own L2 address\n");
        return NULL;
    }

    wpa_printf(MSG_DEBUG, "Own MAC address: " MACSTR,
               MAC2STR(wpa_s->own_addr));

    WIFI_lib_init();

    return wpa_s;
}

void
wapi_supplicant_disassociate(struct wpa_supplicant *wpa_s,
                             int reason_code)
{
    wpa_drv_set_wapi(wpa_s, 0);
    /* TODO: Reset SM of WAPI here, need check about later*/

    wpa_printf(MSG_DEBUG, "%s: In\n", __func__);

    u8 bssid[ETH_ALEN];
    MAC_ADDRESS bssid_s;
    MAC_ADDRESS own_s;

    wpa_drv_get_bssid(wpa_s, bssid);
    memset(&bssid_s, 0, sizeof(bssid_s));
    memset(&own_s, 0, sizeof(own_s));

    memcpy(bssid_s.v, bssid, sizeof(bssid_s.v));
    memcpy(own_s.v, wpa_s->own_addr, sizeof(own_s.v));
    WIFI_WAI_IE_set(NULL, 0);
    WAI_Msg_Input(CONN_DISASSOC, &bssid_s, &own_s, NULL, 0);
    wpa_printf(MSG_DEBUG, "WAI_Msg_Input  success");
}

int wapi_conf_read_certfile(const char *cert_file, unsigned char *cert_buf, int buf_len)
{
    FILE *file = NULL;
    int i = 0;

    if (NULL == cert_file) {
        wpa_printf(MSG_ERROR, "Error: try to open a null certificate");
        return -1;
    }
    file = fopen((const char*)cert_file, "rb");
    if (NULL == file) {
        wpa_printf(MSG_ERROR, "Open file: %s Error", cert_file);
        perror("Open cert file Error");
        return -1;
    }
    while (!feof(file)) {
        cert_buf[i++] = (char)fgetc(file);
        if (i >= buf_len) {
            wpa_printf(MSG_ERROR, "cert buf too small, buf_len = %d, i = %d", buf_len, i);
            fclose(file);
            return -1;
        }
    }
    fclose(file);
    return i;
}

void setPskIncorrect(){
	    struct wpa_supplicant *wpa_s = g_wpa_s;
		
		if(wpa_s){
 			wpa_msg(wpa_s, MSG_INFO, "WPA: 4-Way Handshake failed - " "pre-shared key may be incorrect");
		}
}


void
wapi_supplicant_associate(struct wpa_supplicant *wpa_s,
                          struct wpa_scan_result *bss, struct wpa_ssid *ssid)
{
    struct wpa_driver_associate_params params;
    CNTAP_PARA ap_param;
    wpa_cipher cipher_pairwise, cipher_group;
    size_t len;
    int ret = -1;

    wpa_printf(MSG_DEBUG, "[%s]", __func__);

	wpa_supplicant_set_state(wpa_s,WPA_ASSOCIATING);

    memset(&ap_param, 0, sizeof(CNTAP_PARA));
    if (ssid->key_mgmt & WPA_KEY_MGMT_WAPI_PSK) {
        ap_param.authType = AUTH_TYPE_WAPI_PSK;
        if (ssid->wapi_psk == NULL) {
            wpa_printf(MSG_ERROR, "[%s] No WAPI PSK\n", __func__);
            return;
        }
        if (ssid->wapi_key_type == WAPI_KEY_TYPE_HEX) {
            ap_param.para.kt = KEY_TYPE_HEX;
            ap_param.para.kl = PMK_LEN;
            os_memcpy(ap_param.para.kv, ssid->wapi_psk, PMK_LEN);
        } else {
            ap_param.para.kt = KEY_TYPE_ASCII;
            len = os_strlen(ssid->wapi_psk);
            ap_param.para.kl = len;
            os_memcpy(ap_param.para.kv, ssid->wapi_psk, len);
        }
    } else if (ssid->key_mgmt & WPA_KEY_MGMT_WAPI_CERT) {
        ap_param.authType = AUTH_TYPE_WAPI;
        ret = wapi_conf_read_certfile((const char *)ssid->as_cert_file,
            ap_param.para.as, sizeof(ap_param.para.as));
        if (ret < 0) {
            wpa_printf(MSG_ERROR, "ERROR wapi_conf_read_certfile %s", ssid->as_cert_file);
            return;
        }
        ret = wapi_conf_read_certfile((const char *)ssid->user_cert_file,
            ap_param.para.user, sizeof(ap_param.para.user));
        if (ret < 0) {
            wpa_printf(MSG_ERROR, "ERROR wapi_conf_read_certfile %s", ssid->user_cert_file);
            return;
        }
    } else {
        ap_param.authType = AUTH_TYPE_NONE_WAPI;
    }

    ret = WAI_CNTAPPARA_SET(&ap_param);

    if(!ret){

        cipher_pairwise = CIPHER_SMS4;
        cipher_group = CIPHER_SMS4;

        wpa_hexdump_ascii(MSG_DEBUG, " curent ssid:", (u8 *)ssid->ssid, ssid->ssid_len);
        memset(&params, 0, sizeof(params));
        if (bss) {
            params.ssid = bss->ssid;
            params.ssid_len = bss->ssid_len; //strlen((char *)ssid);
            params.bssid = bss->bssid;
        } else {
            params.ssid = ssid->ssid;
            params.ssid_len = ssid->ssid_len;
        }
        params.mode = IW_MODE_AUTO;

        wpa_s->ap_wapi_ie_len = bss->wapi_ie_len;
        if(bss->wapi_ie_len) {
            memcpy(wpa_s->ap_wapi_ie, bss->wapi_ie, bss->wapi_ie_len);
        }

        params.wpa_ie_len = wpa_s->assoc_wapi_ie_len;
        params.wpa_ie = wpa_s->assoc_wapi_ie;
        params.freq = bss->freq;
        params.pairwise_suite = cipher_pairwise;
        params.group_suite = cipher_group;
        wpa_supplicant_set_state(wpa_s, WPA_ASSOCIATING);

        wpa_printf(MSG_ERROR, "wapi return value:%d\n", wpa_drv_set_wapi(wpa_s, 1));

        if (!memcmp(wpa_s->bssid, "\x00\x00\x00\x00\x00\x00", ETH_ALEN)) { /*2006-11-13*/
            /* Timeout for IEEE 802.11 authentication and association */
            wpa_supplicant_req_auth_timeout(wpa_s, 10000, 0);

            wpa_printf(MSG_DEBUG, "wpa ie len = %d\n", params.wpa_ie_len);
            wpa_hexdump(MSG_DEBUG, "wapi IE: ", params.wpa_ie, params.wpa_ie_len);
            if(wpa_drv_associate(wpa_s, &params)) {
                printf("wapi_supplicant_associate failed\n");
                wpa_supplicant_set_state(wpa_s, WPA_DISCONNECTED);
            }
        }
    }else{
    /* TODO: add wpa_msg to notify upper to change the certificate or psk*/
	    setPskIncorrect();
	    wpa_supplicant_set_state(wpa_s,WPA_DISCONNECTED);
    }
}

void
wapi_supplicant_event(struct wpa_supplicant *wpa_s, wpa_event_type event,
                      void *data)
{
    u8 bssid[ETH_ALEN];

    wpa_printf(MSG_DEBUG, "%s: event = %d\n", __func__, event);
    switch (event) {
    case EVENT_ASSOC:
        {
            MAC_ADDRESS bssid_s;
            MAC_ADDRESS own_s;

            wpa_drv_get_bssid(wpa_s, bssid);
            memset(&bssid_s, 0, sizeof(bssid_s));
            memset(&own_s, 0, sizeof(own_s));

            memcpy(bssid_s.v, bssid, sizeof(bssid_s.v));
            memcpy(own_s.v, wpa_s->own_addr, sizeof(own_s.v));
            wpa_hexdump(MSG_DEBUG,"bssid",bssid, sizeof(bssid));
            wpa_hexdump(MSG_DEBUG,"own mac",wpa_s->own_addr, 6);
            if(wpa_s->ap_wapi_ie_len) {
                wpa_printf(MSG_DEBUG, "Call CONN_ASSOC Hong\n");
                WAI_Msg_Input(CONN_ASSOC, &bssid_s, &own_s, wpa_s->ap_wapi_ie,
                              wpa_s->ap_wapi_ie_len);
            } else {
                WAI_Msg_Input(CONN_ASSOC, &bssid_s, &own_s, NULL, 0);
            }
            break;
        }
    case EVENT_DISASSOC:
        wpa_printf(MSG_DEBUG, "Disconnect event ");

        MAC_ADDRESS bssid_s;
        MAC_ADDRESS own_s;

        wpa_drv_get_bssid(wpa_s, bssid);
        memset(&bssid_s, 0, sizeof(bssid_s));
        memset(&own_s, 0, sizeof(own_s));

        memcpy(bssid_s.v, bssid, sizeof(bssid_s.v));
        memcpy(own_s.v, wpa_s->own_addr, sizeof(own_s.v));
        WAI_Msg_Input(CONN_DISASSOC, &bssid_s, &own_s, NULL, 0);
        wpa_printf(MSG_DEBUG, "WAI_Msg_Input  success");
        break;
    default:
        wpa_printf(MSG_INFO, "Unknown event %d", event);
        break;
    }
}

unsigned char
WIFI_get_rand_byte()
{
    return (unsigned char)(rand() & 0xFF);
}


void WIFI_Action_Deauth()
{
    struct wpa_supplicant *wpa_s = g_wpa_s;

    wpa_supplicant_disassociate(wpa_s, 15);
}

unsigned long
WIFI_TX_packet(const char* pbuf, int length)
{
    struct wpa_supplicant *wpa_s = g_wpa_s;
    int ret;

    ret = l2_packet_send(wpa_s->l2_wapi, wpa_s->bssid, ETH_P_WAI,
                         (const u8*)pbuf, (unsigned  int)length);

    if (ret >= 0) {
        return 0;
    } else {
        return (unsigned long)ret;
    }
}


int
WIFI_group_key_set(const unsigned char* pKeyValue,
                   int keylength,
                   int key_idx,
                   const unsigned char* keyIV)
{
    struct wpa_supplicant *wpa_s = g_wpa_s;
    int alg;
    u8 msk_len;

    wpa_hexdump(MSG_DEBUG, "WIFI_group_key_set: KEYSC", keyIV, 16);
    printf("[%s: %d] keylength = %d key_index = %d\n", __FUNCTION__, __LINE__, keylength, key_idx);
    alg = WPA_ALG_SMS4;
    msk_len = 32;


    if (wpa_drv_set_key(wpa_s,
                        alg,
                        (u8 *) "\xff\xff\xff\xff\xff\xff",
                        key_idx,
                        1,
                        keyIV,
                        16,
                        pKeyValue,
                        keylength) < 0)
    {
        wpa_printf(MSG_WARNING, "WPA: Failed to set MSK to "
                   "the driver.");
        return -1;
    }

    return 0;
}


int
WIFI_unicast_key_set(const char* pKeyValue,
                     int keylength,
                     int key_idx)
{
    struct wpa_supplicant *wpa_s = g_wpa_s;

    int alg, keylen, rsclen;
    u8 *key_rsc;
    u8 null_rsc[16] = { 0, 0, 0, 0, 0, 0, 0, 0 };


    wpa_printf(MSG_WARNING, "[%s: %d] keylength = %d key_index = %d\n", __FUNCTION__, __LINE__, keylength, key_idx);
    wpa_printf(MSG_WARNING, "[%s: %d] bssid = %02X:%02X:%02X:%02X:%02X:%02X \n", __FUNCTION__, __LINE__,wpa_s->bssid[0],
wpa_s->bssid[1], wpa_s->bssid[2], wpa_s->bssid[3], wpa_s->bssid[4], wpa_s->bssid[5]);

    alg = WPA_ALG_SMS4;
    keylen = 32;
    rsclen = 16;

    key_rsc = null_rsc;

    if (wpa_drv_set_key(wpa_s,

                        alg,
                        wpa_s->bssid,
                        -1,
                        1,
                        key_rsc,
                        rsclen,
                        (u8 *) pKeyValue,
                        keylen) < 0)
    {
        wpa_printf(MSG_WARNING, "WIFI_unicast_key_set: Failed to set PTK to the driver");
        return -1;
    }

    return 0;
}

int
WIFI_WAI_IE_set(const u8* pbuf, int length)
{
    struct wpa_supplicant *wpa_s = g_wpa_s;
    u32 temp;

    if (length == 0) {
        wpa_drv_set_wpa_ie(wpa_s, (u8 *)&temp, length);
    } else {
        wpa_drv_set_wpa_ie(wpa_s, pbuf, length);
    }
    wpa_s->assoc_wapi_ie_len = length;

    if (length != 0) {
        os_memcpy(wpa_s->assoc_wapi_ie, pbuf, length);
    } else {
        os_memset(wpa_s->assoc_wapi_ie, 0, sizeof(wpa_s->assoc_wapi_ie));
    }

    return 0;
}


void
wapi_supplicant_key_negotiation_state_report(wpa_states state)
{
    struct wpa_supplicant *wpa_s = g_wpa_s;

    if (wpa_s != NULL) {
        wpa_supplicant_set_state(wpa_s, state);
        if (state == WPA_COMPLETED) {
            wpa_supplicant_cancel_auth_timeout(wpa_s);
        }
    }
}

static void
init_sigaction(int signum, OS_timer_expired handle)
{
    signal(signum, handle);
}

void*
OS_timer_setup(int deltaTimer, int repeated, OS_timer_expired pfunction,
               const void* pdata)
{
    struct itimerval value;
    int sec = 0, usec = 0;
    int repsec = 0, repusec = 0;


    if (deltaTimer < 0 || repeated < 0 || pfunction == NULL) {
        return NULL;
    }

    init_sigaction(SIGALRM, pfunction);

    sec  = deltaTimer/(1000);
    usec = (deltaTimer%(1000)) * 1000;

    repsec  = repeated/(1000);
    repusec = (repeated%(1000)) * 1000;

    memset(&value, 0, sizeof(value));

    value.it_interval.tv_sec   = repsec;
    value.it_interval.tv_usec = repusec;

    value.it_value.tv_sec   = sec;
    value.it_value.tv_usec = usec;

    setitimer(ITIMER_REAL, &value, NULL);

    return (void *)pfunction;
}

#ifndef CONFIG_NATIVE_WINDOWS
static void
eloop_handle_alarm(int sig)
{
    exit(1);
}
#endif // CONFIG_NATIVE_WINDOWS

void
OS_timer_clean(void* pTimer)
{
    struct itimerval value;

    memset(&value, 0, sizeof(value));
    setitimer(ITIMER_REAL, &value, NULL);

#ifndef CONFIG_NATIVE_WINDOWS
    signal(SIGALRM, eloop_handle_alarm);
#endif
}
