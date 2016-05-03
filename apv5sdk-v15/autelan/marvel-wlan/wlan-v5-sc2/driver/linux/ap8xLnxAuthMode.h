#ifndef _AP8X_LNX_AUTH_MODE_H_
#define _AP8X_LNX_AUTH_MODE_H_

#include <wl_mib.h>
#include <wl_hal.h>
/*
 * Authentication mode.
 * NB: the usage of auth modes NONE, AUTO are deprecated,
 * they are implemented through combinations of other auth modes
 * and cipher types. The deprecated values are preserved here to
 * maintain binary compatibility with applications like
 * wpa_supplicant and hostapd.
 */
#if 0
typedef enum _ieee80211_auth_mode {
    IEEE80211_AUTH_NONE     = 0, /* deprecated */
    IEEE80211_AUTH_OPEN     = 1, /* open */
    IEEE80211_AUTH_SHARED   = 2, /* shared-key */
    IEEE80211_AUTH_8021X    = 3, /* 802.1x */
    IEEE80211_AUTH_AUTO     = 4, /* deprecated */
    IEEE80211_AUTH_WPA      = 5, /* WPA */
    IEEE80211_AUTH_RSNA     = 6, /* WPA2/RSNA */
    IEEE80211_AUTH_CCKM     = 7, /* CCK */
    IEEE80211_AUTH_WAPI     = 8, /* WAPI */
} ieee80211_auth_mode;
#endif
int set_ieee80211_authmode(vmacApInfo_t *vmacSta_p, MIB_802DOT11 *mib, int value);
int set_80211_privacy(vmacApInfo_t *vmacSta_p, MIB_802DOT11 *mib, int value);

//yuanding add
int set_80211_keymgt_mode(vmacApInfo_t *vmacSta_p, MIB_802DOT11 *mib, int value);


#endif
