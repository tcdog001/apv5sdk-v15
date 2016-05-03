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
#ifndef AP8X_SCAN_H_
#define AP8X_SCAN_H_

#include <linux/version.h>
#include <linux/wireless.h>
#include "ap8xLnxIoctlWiress.h"

#define IEEE80211_CAPINFO_ESS           0x0001
#define IEEE80211_CAPINFO_IBSS          0x0002
#define IEEE80211_CAPINFO_CF_POLLABLE       0x0004
#define IEEE80211_CAPINFO_CF_POLLREQ        0x0008
#define IEEE80211_CAPINFO_PRIVACY           0x0010
#define IEEE80211_CAPINFO_SHORT_PREAMBLE    0x0020
#define IEEE80211_CAPINFO_PBCC                  0x0040
#define IEEE80211_CAPINFO_CHNL_AGILITY      0x0080
/* bits 8-9 are reserved (8 now for specturm management) */
#define IEEE80211_CAPINFO_SPECTRUM_MGMT     0x0100
#define IEEE80211_CAPINFO_SHORT_SLOTTIME        0x0400
#define IEEE80211_CAPINFO_RSN                   0x0800
/* bit 12 is reserved */
#define IEEE80211_CAPINFO_DSSSOFDM              0x2000

#define IEEE80211_RATE_VAL      0x7f
#define IEEE80211_RATE_MAXSIZE  36      /* max rates we'll handle */

//#define min(a,b) ((a) < (b) ? (a) : (b))
//#define roundup(x, y)   ((((x)+((y)-1))/(y))*(y))

/*
 * Scan result data returned for IEEE80211_IOC_SCAN_RESULTS.
 */
struct ieee80211req_scan_result{
    u_int16_t   isr_len;        /* length (mult of 4) */
    u_int16_t   isr_freq;       /* MHz */
    u_int32_t   isr_flags;      /* channel flags */
    u_int8_t    isr_noise;
    u_int8_t    isr_rssi;
    u_int8_t    isr_intval;     /* beacon interval--modified by chenming to U32 */
    u_int16_t   isr_capinfo;        /* capabilities */
    u_int8_t    isr_erp;        /* ERP element */
    u_int8_t    isr_bssid[IEEE80211_ADDR_LEN];
    u_int8_t    isr_nrates;
    u_int8_t    isr_rates[IEEE80211_RATE_MAXSIZE];
    u_int8_t    isr_ssid_len;       /* SSID length */
    u_int16_t   isr_ie_len;     /* IE length */
    u_int8_t    isr_pad[4];
    /* variable length SSID followed by IE data */
};

struct scanreq
{
    struct ieee80211req_scan_result *sr;
    size_t space;
};

enum
{
    IEEE80211_ELEMID_SSID       = 0,
    IEEE80211_ELEMID_RATES      = 1,
    IEEE80211_ELEMID_FHPARMS    = 2,
    IEEE80211_ELEMID_DSPARMS    = 3,
    IEEE80211_ELEMID_CFPARMS    = 4,
    IEEE80211_ELEMID_TIM        = 5,
    IEEE80211_ELEMID_IBSSPARMS  = 6,
    IEEE80211_ELEMID_COUNTRY    = 7,
    IEEE80211_ELEMID_CHALLENGE  = 16,
    IEEE80211_ELEMID_ERP        = 42,
    IEEE80211_ELEMID_RSN        = 48,
    IEEE80211_ELEMID_XRATES     = 50,
    IEEE80211_ELEMID_TPC        = 150,
    IEEE80211_ELEMID_CCKM       = 156,
    IEEE80211_ELEMID_VENDOR     = 221,  
};
#define WPA_OUI         0xf25000
#define WPA_OUI_TYPE        0x01

/* unaligned little endian access */
#define LE_READ_4(p)                    \
    ((u_int32_t)                    \
     ((((const u_int8_t *)(p))[0]      ) |      \
      (((const u_int8_t *)(p))[1] <<  8) |      \
      (((const u_int8_t *)(p))[2] << 16) |      \
      (((const u_int8_t *)(p))[3] << 24)))


unsigned int ieee80211_ieee2mhz(unsigned int chan);
void getcapinfo(UINT16 *isr_capinfo,IEEEtypes_CapInfo_t CapInfo);
int scan_space(struct scanreq  *req);
int ieee80211_ioctl_getscanresults(struct net_device *netdev,char *ret_str, UINT16 *ret_len);
UINT8 ieee80211_Control_Scan_Chan(vmacApInfo_t *vmacSta_p,UINT8 *mainChnlList,UINT8 *chnlScanList,UINT8 *scanType);
int getmaxrate(uint8_t rates[15], uint8_t nrates);
UINT8 ieee80211_Control_Get_Support_Chan(vmacApInfo_t *vmacSta_p,UINT8 *mainChnlList,UINT8 *chnlScanList);
size_t copy_essid(char buf[], size_t bufsize, const u_int8_t *essid, size_t essid_len);

#endif /* AP8X_SCAN_H_ */

