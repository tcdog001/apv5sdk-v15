#include "StaDb.h"
#include "wldebug.h"
#include "ap8xLnxFwcmd.h"
#include "wl_mib.h"

typedef signed char        A_INT8;
typedef signed short       A_INT16;
typedef signed int         A_INT32;
typedef signed long long   A_INT64;
typedef unsigned char      A_UINT8;
typedef unsigned short     A_UINT16;
typedef unsigned int       A_UINT32;
typedef unsigned long long A_UINT64;
typedef char               A_CHAR;
typedef unsigned char      A_UCHAR;

typedef    BOOLEAN         A_BOOL;
typedef    A_INT8          A_RSSI;
typedef    A_INT32         A_RSSI32;
typedef    A_UINT8         WLAN_PHY;

#define TRUE_20     0x2
#define TRUE_40     0x4
#define TRUE_2040   (TRUE_20|TRUE_40)
#define TRUE_ALL    (TRUE_2040|TRUE)

#define    WLAN_PHY_OFDM    IEEE80211_T_OFDM
#define    WLAN_PHY_TURBO   IEEE80211_T_TURBO
#define    WLAN_PHY_CCK     IEEE80211_T_DS
#define    WLAN_PHY_XR    (IEEE80211_T_TURBO+1)

#define WLAN_PHY_HT_20_SS       WLAN_RC_PHY_HT_20_SS
#define WLAN_PHY_HT_20_SS_HGI   WLAN_RC_PHY_HT_20_SS_HGI
#define WLAN_PHY_HT_20_DS       WLAN_RC_PHY_HT_20_DS
#define WLAN_PHY_HT_20_DS_HGI   WLAN_RC_PHY_HT_20_DS_HGI
#define WLAN_PHY_HT_20_TS       WLAN_RC_PHY_HT_20_TS
#define WLAN_PHY_HT_20_TS_HGI   WLAN_RC_PHY_HT_20_TS_HGI
#define WLAN_PHY_HT_40_SS       WLAN_RC_PHY_HT_40_SS
#define WLAN_PHY_HT_40_SS_HGI   WLAN_RC_PHY_HT_40_SS_HGI
#define WLAN_PHY_HT_40_DS       WLAN_RC_PHY_HT_40_DS
#define WLAN_PHY_HT_40_DS_HGI   WLAN_RC_PHY_HT_40_DS_HGI
#define WLAN_PHY_HT_40_TS       WLAN_RC_PHY_HT_40_TS
#define WLAN_PHY_HT_40_TS_HGI   WLAN_RC_PHY_HT_40_TS_HGI

/*                         One       Two      Three
 *                        strm      strm      strm
 */
#define TRUE_ALL_1_2_3  TRUE_ALL, TRUE_ALL, TRUE_ALL
#define FALSE_1_2_3        FALSE,    FALSE,    FALSE
#define TRUE_1_2_3          TRUE,     TRUE,     TRUE
#define TRUE20_1_2_3     TRUE_20,  TRUE_20,  TRUE_20
#define TRUE20_1         TRUE_20,    FALSE,    FALSE
#define TRUE20_2           FALSE,  TRUE_20,    FALSE
#define TRUE20_3           FALSE,    FALSE,  TRUE_20
#define TRUE20_1_2       TRUE_20,  TRUE_20,    FALSE
#define TRUE20_2_3         FALSE,  TRUE_20,  TRUE_20
#define TRUE40_1_2_3     TRUE_40,  TRUE_40,  TRUE_40
#define TRUE40_1         TRUE_40,    FALSE,    FALSE
#define TRUE40_2           FALSE,  TRUE_40,    FALSE
#define TRUE40_3           FALSE,    FALSE,  TRUE_40
#define TRUE40_1_2       TRUE_40,  TRUE_40,    FALSE
#define TRUE40_2_3         FALSE,  TRUE_40,  TRUE_40
#define TRUE2040_1_2_3 (TRUE_20|TRUE_40), (TRUE_20|TRUE_40) ,(TRUE_20|TRUE_40)    
#define TRUE2040       (TRUE_20|TRUE_40)



#define TRUE_TS_N2              (0x01)
#define TRUE_DS_N2              (0x02)
#define TRUE_TS_DS_N2           (TRUE_TS_N2|TRUE_DS_N2)
#define TRUE_TS_N1              (0x04)
#define TRUE_DS_N1              (0x08)
#define TRUE_SS_N1              (0x10)
#define TXBF_TRUE20             (0x20)
#define TXBF_TRUE40             (0x40)
#define TXBF_TRUE20_40          (TXBF_TRUE20|TXBF_TRUE40)
#define TRUE_TS_SS_N1           (TRUE_TS_N1|TRUE_SS_N1)
#define TRUE_DS_SS_N1           (TRUE_DS_N1|TRUE_SS_N1)
#define TRUE_TS_DS_N1           (TRUE_TS_N1|TRUE_DS_N1)
#define TRUE_ALL_N1             (TRUE_TS_N1|TRUE_DS_N1|TRUE_SS_N1)


/*                          
 * TRUE_N2_A_N1_B
 * A: support under Nss=2
 * B: support under Nss=1
 *      ALL     : rate supported for all configuration of its Nss
 *      S,D,T   : rate supported for single, dual ,triple stream of its Nss
 *      F       : rate not supported
 *
 *                          (Nss2   |   Nss1)   
 */
#define TRUE_N_1_2_ALL      (TRUE_TS_DS_N2|TRUE_ALL_N1)
#define FALSE_N_ALL         (FALSE|FALSE)
#define TRUE_N2_ALL_N1_F    (TRUE_TS_DS_N2|FALSE)
#define TRUE_N2_T_N1_F      (TRUE_TS_N2|FALSE)
#define TRUE_N2_D_N1_F      (TRUE_DS_N2|FALSE)
#define TRUE_N2_ALL_N1_T    (TRUE_TS_DS_N2|TRUE_TS_N1)
#define TRUE_N2_T_N1_T      (TRUE_TS_N2|TRUE_TS_N1)
#define TRUE_N2_D_N1_T      (TRUE_DS_N2|TRUE_TS_N1)
#define TRUE_N2_F_N1_T      (FALSE|TRUE_TS_N1)
#define TRUE_N2_ALL_N1_D    (TRUE_TS_DS_N2|TRUE_DS_N1)
#define TRUE_N2_T_N1_D      (TRUE_TS_N2|TRUE_DS_N1)
#define TRUE_N2_D_N1_D      (TRUE_DS_N2|TRUE_DS_N1)
#define TRUE_N2_F_N1_D      (FALSE|TRUE_DS_N1)
#define TRUE_N2_ALL_N1_S    (TRUE_TS_DS_N2 | TRUE_SS_N1)
#define TRUE_N2_T_N1_S      (TRUE_TS_N2 | TRUE_SS_N1)
#define TRUE_N2_D_N1_S      (TRUE_DS_N2 | TRUE_SS_N1)
#define TRUE_N2_F_N1_S      (FALSE | TRUE_SS_N1)
#define TRUE_N2_ALL_N1_T_S  (TRUE_TS_DS_N2 | TRUE_TS_SS_N1)
#define TRUE_N2_T_N1_T_S    (TRUE_TS_N2 | TRUE_TS_SS_N1)
#define TRUE_N2_D_N1_T_S    (TRUE_DS_N2 | TRUE_TS_SS_N1)
#define TRUE_N2_F_N1_T_S    (FALSE | TRUE_TS_SS_N1)
#define TRUE_N2_ALL_N1_D_S  (TRUE_TS_DS_N2 | TRUE_DS_SS_N1)
#define TRUE_N2_T_N1_D_S    (TRUE_TS_N2 | TRUE_DS_SS_N1)
#define TRUE_N2_D_N1_D_S    (TRUE_DS_N2 | TRUE_DS_SS_N1)
#define TRUE_N2_F_N1_D_S    (FALSE | TRUE_DS_SS_N1)
#define TRUE_N2_ALL_N1_T_D  (TRUE_TS_DS_N2 | TRUE_TS_DS_N1)
#define TRUE_N2_T_N1_T_D    (TRUE_TS_N2 | TRUE_TS_DS_N1)
#define TRUE_N2_D_N1_T_D    (TRUE_DS_N2 | TRUE_TS_DS_N1)
#define TRUE_N2_F_N1_T_D    (FALSE | TRUE_TS_DS_N1)
#define TRUE_N2_T_N1_ALL    (TRUE_TS_N2 | TRUE_ALL_N1)
#define TRUE_N2_D_N1_ALL    (TRUE_DS_N2 | TRUE_ALL_N1)
#define TRUE_N2_F_N1_ALL    (FALSE | TRUE_ALL_N1)

#define TRUE20_N_1_2_ALL        (TXBF_TRUE20|TRUE_N_1_2_ALL)
#define TRUE20_N2_ALL_N1_F      (TXBF_TRUE20|TRUE_N2_ALL_N1_F)
#define TRUE20_N2_T_N1_F        (TXBF_TRUE20|TRUE_N2_T_N1_F)
#define TRUE20_N2_D_N1_F        (TXBF_TRUE20|TRUE_N2_D_N1_F)
#define TRUE20_N2_ALL_N1_T      (TXBF_TRUE20|TRUE_N2_ALL_N1_T)
#define TRUE20_N2_T_N1_T        (TXBF_TRUE20|TRUE_N2_T_N1_T)
#define TRUE20_N2_D_N1_T        (TXBF_TRUE20|TRUE_N2_D_N1_T)
#define TRUE20_N2_F_N1_T        (TXBF_TRUE20|TRUE_N2_F_N1_T)
#define TRUE20_N2_ALL_N1_D      (TXBF_TRUE20|TRUE_N2_ALL_N1_D)
#define TRUE20_N2_T_N1_D        (TXBF_TRUE20|TRUE_N2_T_N1_D)
#define TRUE20_N2_D_N1_D        (TXBF_TRUE20|TRUE_N2_D_N1_D)
#define TRUE20_N2_F_N1_D        (TXBF_TRUE20|TRUE_N2_F_N1_D)
#define TRUE20_N2_ALL_N1_S      (TXBF_TRUE20|TRUE_N2_ALL_N1_S)
#define TRUE20_N2_T_N1_S        (TXBF_TRUE20|TRUE_N2_T_N1_S)
#define TRUE20_N2_D_N1_S        (TXBF_TRUE20|TRUE_N2_D_N1_S)
#define TRUE20_N2_F_N1_S        (TXBF_TRUE20|TRUE_N2_F_N1_S)
#define TRUE20_N2_ALL_N1_T_S    (TXBF_TRUE20|TRUE_N2_ALL_N1_T_S)
#define TRUE20_N2_T_N1_T_S      (TXBF_TRUE20|TRUE_N2_T_N1_T_S)
#define TRUE20_N2_D_N1_T_S      (TXBF_TRUE20|TRUE_N2_D_N1_T_S)
#define TRUE20_N2_F_N1_T_S      (TXBF_TRUE20|TRUE_N2_F_N1_T_S)
#define TRUE20_N2_ALL_N1_D_S    (TXBF_TRUE20|TRUE_N2_ALL_N1_D_S)
#define TRUE20_N2_T_N1_D_S      (TXBF_TRUE20|TRUE_N2_T_N1_D_S)
#define TRUE20_N2_D_N1_D_S      (TXBF_TRUE20|TRUE_N2_D_N1_D_S)
#define TRUE20_N2_F_N1_D_S      (TXBF_TRUE20|TRUE_N2_F_N1_D_S)
#define TRUE20_N2_ALL_N1_T_D    (TXBF_TRUE20|TRUE_N2_ALL_N1_T_D)
#define TRUE20_N2_T_N1_T_D      (TXBF_TRUE20|TRUE_N2_T_N1_T_D)
#define TRUE20_N2_D_N1_T_D      (TXBF_TRUE20|TRUE_N2_D_N1_T_D)
#define TRUE20_N2_F_N1_T_D      (TXBF_TRUE20|TRUE_N2_F_N1_T_D)
#define TRUE20_N2_T_N1_ALL      (TXBF_TRUE20|TRUE_N2_T_N1_ALL)
#define TRUE20_N2_D_N1_ALL      (TXBF_TRUE20|TRUE_N2_D_N1_ALL)
#define TRUE20_N2_F_N1_ALL      (TXBF_TRUE20|TRUE_N2_F_N1_ALL)

#define TRUE40_N_1_2_ALL        (TXBF_TRUE40|TRUE_N_1_2_ALL)
#define TRUE40_N2_ALL_N1_F      (TXBF_TRUE40|TRUE_N2_ALL_N1_F)
#define TRUE40_N2_T_N1_F        (TXBF_TRUE40|TRUE_N2_T_N1_F)
#define TRUE40_N2_D_N1_F        (TXBF_TRUE40|TRUE_N2_D_N1_F)
#define TRUE40_N2_ALL_N1_T      (TXBF_TRUE40|TRUE_N2_ALL_N1_T)
#define TRUE40_N2_T_N1_T        (TXBF_TRUE40|TRUE_N2_T_N1_T)
#define TRUE40_N2_D_N1_T        (TXBF_TRUE40|TRUE_N2_D_N1_T)
#define TRUE40_N2_F_N1_T        (TXBF_TRUE40|TRUE_N2_F_N1_T)
#define TRUE40_N2_ALL_N1_D      (TXBF_TRUE40|TRUE_N2_ALL_N1_D)
#define TRUE40_N2_T_N1_D        (TXBF_TRUE40|TRUE_N2_T_N1_D)
#define TRUE40_N2_D_N1_D        (TXBF_TRUE40|TRUE_N2_D_N1_D)
#define TRUE40_N2_F_N1_D        (TXBF_TRUE40|TRUE_N2_F_N1_D)
#define TRUE40_N2_ALL_N1_S      (TXBF_TRUE40|TRUE_N2_ALL_N1_S)
#define TRUE40_N2_T_N1_S        (TXBF_TRUE40|TRUE_N2_T_N1_S)
#define TRUE40_N2_D_N1_S        (TXBF_TRUE40|TRUE_N2_D_N1_S)
#define TRUE40_N2_F_N1_S        (TXBF_TRUE40|TRUE_N2_F_N1_S)
#define TRUE40_N2_ALL_N1_T_S    (TXBF_TRUE40|TRUE_N2_ALL_N1_T_S)
#define TRUE40_N2_T_N1_T_S      (TXBF_TRUE40|TRUE_N2_T_N1_T_S)
#define TRUE40_N2_D_N1_T_S      (TXBF_TRUE40|TRUE_N2_D_N1_T_S)
#define TRUE40_N2_F_N1_T_S      (TXBF_TRUE40|TRUE_N2_F_N1_T_S)
#define TRUE40_N2_ALL_N1_D_S    (TXBF_TRUE40|TRUE_N2_ALL_N1_D_S)
#define TRUE40_N2_T_N1_D_S      (TXBF_TRUE40|TRUE_N2_T_N1_D_S)
#define TRUE40_N2_D_N1_D_S      (TXBF_TRUE40|TRUE_N2_D_N1_D_S)
#define TRUE40_N2_F_N1_D_S      (TXBF_TRUE40|TRUE_N2_F_N1_D_S)
#define TRUE40_N2_ALL_N1_T_D    (TXBF_TRUE40|TRUE_N2_ALL_N1_T_D)
#define TRUE40_N2_T_N1_T_D      (TXBF_TRUE40|TRUE_N2_T_N1_T_D)
#define TRUE40_N2_D_N1_T_D      (TXBF_TRUE40|TRUE_N2_D_N1_T_D)
#define TRUE40_N2_F_N1_T_D      (TXBF_TRUE40|TRUE_N2_F_N1_T_D)
#define TRUE40_N2_T_N1_ALL      (TXBF_TRUE40|TRUE_N2_T_N1_ALL)
#define TRUE40_N2_D_N1_ALL      (TXBF_TRUE40|TRUE_N2_D_N1_ALL)
#define TRUE40_N2_F_N1_ALL      (TXBF_TRUE40|TRUE_N2_F_N1_ALL)

#define TRUE2040_N_1_2_ALL      (TXBF_TRUE20_40|TRUE_N_1_2_ALL)
#define TRUE2040_N2_ALL_N1_F    (TXBF_TRUE20_40|TRUE_N2_ALL_N1_F)
#define TRUE2040_N2_T_N1_F      (TXBF_TRUE20_40|TRUE_N2_T_N1_F)
#define TRUE2040_N2_D_N1_F      (TXBF_TRUE20_40|TRUE_N2_D_N1_F)
#define TRUE2040_N2_ALL_N1_T    (TXBF_TRUE20_40|TRUE_N2_ALL_N1_T)
#define TRUE2040_N2_T_N1_T      (TXBF_TRUE20_40|TRUE_N2_T_N1_T)
#define TRUE2040_N2_D_N1_T      (TXBF_TRUE20_40|TRUE_N2_D_N1_T)
#define TRUE2040_N2_F_N1_T      (TXBF_TRUE20_40|TRUE_N2_F_N1_T)
#define TRUE2040_N2_ALL_N1_D    (TXBF_TRUE20_40|TRUE_N2_ALL_N1_D)
#define TRUE2040_N2_T_N1_D      (TXBF_TRUE20_40|TRUE_N2_T_N1_D)
#define TRUE2040_N2_D_N1_D      (TXBF_TRUE20_40|TRUE_N2_D_N1_D)
#define TRUE2040_N2_F_N1_D      (TXBF_TRUE20_40|TRUE_N2_F_N1_D)
#define TRUE2040_N2_ALL_N1_S    (TXBF_TRUE20_40|TRUE_N2_ALL_N1_S)
#define TRUE2040_N2_T_N1_S      (TXBF_TRUE20_40|TRUE_N2_T_N1_S)
#define TRUE2040_N2_D_N1_S      (TXBF_TRUE20_40|TRUE_N2_D_N1_S)
#define TRUE2040_N2_F_N1_S      (TXBF_TRUE20_40|TRUE_N2_F_N1_S)
#define TRUE2040_N2_ALL_N1_T_S  (TXBF_TRUE20_40|TRUE_N2_ALL_N1_T_S)
#define TRUE2040_N2_T_N1_T_S    (TXBF_TRUE20_40|TRUE_N2_T_N1_T_S)
#define TRUE2040_N2_D_N1_T_S    (TXBF_TRUE20_40|TRUE_N2_D_N1_T_S)
#define TRUE2040_N2_F_N1_T_S    (TXBF_TRUE20_40|TRUE_N2_F_N1_T_S)
#define TRUE2040_N2_ALL_N1_D_S  (TXBF_TRUE20_40|TRUE_N2_ALL_N1_D_S)
#define TRUE2040_N2_T_N1_D_S    (TXBF_TRUE20_40|TRUE_N2_T_N1_D_S)
#define TRUE2040_N2_D_N1_D_S    (TXBF_TRUE20_40|TRUE_N2_D_N1_D_S)
#define TRUE2040_N2_F_N1_D_S    (TXBF_TRUE20_40|TRUE_N2_F_N1_D_S)
#define TRUE2040_N2_ALL_N1_T_D  (TXBF_TRUE20_40|TRUE_N2_ALL_N1_T_D)
#define TRUE2040_N2_T_N1_T_D    (TXBF_TRUE20_40|TRUE_N2_T_N1_T_D)
#define TRUE2040_N2_D_N1_T_D    (TXBF_TRUE20_40|TRUE_N2_D_N1_T_D)
#define TRUE2040_N2_F_N1_T_D    (TXBF_TRUE20_40|TRUE_N2_F_N1_T_D)
#define TRUE2040_N2_T_N1_ALL    (TXBF_TRUE20_40|TRUE_N2_T_N1_ALL)
#define TRUE2040_N2_D_N1_ALL    (TXBF_TRUE20_40|TRUE_N2_D_N1_ALL)
#define TRUE2040_N2_F_N1_ALL    (TXBF_TRUE20_40|TRUE_N2_F_N1_ALL)


typedef struct {
        A_UINT32  validSS      : 4, /* Valid for use in rate control for single stream operation */
                  validDS      : 4, /* Valid for use in rate control for dual stream operation */
                  validTS      : 4, /* Valid for use in rate control for triple stream operation */
                  validSTBC_SS : 4, /* Valid for use in rate control for STBC operation on 1 stream products */
                  validSTBC_DS : 4, /* Valid for use in rate control for STBC operation on 2 stream products */
                  validSTBC_TS : 4, /* Valid for use in rate control for STBC operation on 3 stream products */
                  validUAPSD   : 1, /* Valid for use in rate control for UAPSD operation */
                  validTxBF    : 7; /* Valid for use in rate control for Beamforming operation */
        WLAN_PHY  phy;                /* CCK/OFDM/TURBO/XR */
        A_UINT32  rateKbps;           /* Rate in Kbits per second */
        A_UINT32  userRateKbps;       /* User rate in KBits per second */
        A_UINT8   rateCode;           /* rate that goes into hw descriptors */
        A_UINT8   shortPreamble;      /* Mask for enabling short preamble in rate code for CCK */
        A_UINT8   dot11Rate;          /* Value that goes into supported rates info element of MLME */
        A_UINT8   controlRate;        /* Index of next lower basic rate, used for duration computation */
        A_RSSI    rssiAckValidMin;    /* Rate control related information */
        A_RSSI    rssiAckDeltaMin;    /* Rate control related information */
        A_UINT8   maxTxChains;        /* maximum number of tx chains to use */
        A_UINT8   baseIndex;          /* base rate index */
        A_UINT8   cw40Index;          /* 40cap rate index */
        A_UINT8   sgiIndex;           /* shortgi rate index */
        A_UINT8   htIndex;            /* shortgi rate index */
        A_UINT32  max4msframelen;     /* Maximum frame length(bytes) for 4ms tx duration */
} RATE_INFO_11N;

enum ieee80211_phytype {
    IEEE80211_T_DS,                 /* direct sequence spread spectrum */
    IEEE80211_T_FH,                 /* frequency hopping */
    IEEE80211_T_OFDM,               /* frequency division multiplexing */
    IEEE80211_T_TURBO,              /* high rate OFDM, aka turbo mode */
    IEEE80211_T_HT,                 /* HT - full GI */
    IEEE80211_T_ERP,
    IEEE80211_T_ANY,
    IEEE80211_T_MAX
};

enum {
    WLAN_RC_PHY_CCK,
    WLAN_RC_PHY_OFDM,
    WLAN_RC_PHY_TURBO,
    WLAN_RC_PHY_XR,
    WLAN_RC_PHY_HT_20_SS,
    WLAN_RC_PHY_HT_20_DS,
    WLAN_RC_PHY_HT_20_TS,
    WLAN_RC_PHY_HT_40_SS,
    WLAN_RC_PHY_HT_40_DS,
    WLAN_RC_PHY_HT_40_TS,
    WLAN_RC_PHY_HT_20_SS_HGI,
    WLAN_RC_PHY_HT_20_DS_HGI,
    WLAN_RC_PHY_HT_20_TS_HGI,
    WLAN_RC_PHY_HT_40_SS_HGI,
    WLAN_RC_PHY_HT_40_DS_HGI,
    WLAN_RC_PHY_HT_40_TS_HGI,
    WLAN_RC_PHY_MAX
};

#define IEEE80211_RATE_XRATE_SIZE   13

int ieee80211_get_current_rate(struct net_device *dev,struct iw_request_info *info, struct iw_param *bitrate, char *extra);
unsigned int ieee80211_get_rates_mask(vmacApInfo_t *vmacSta_p,int flag);
int ieee80211_reset_rate_mask(MIB_802DOT11 *mib);


