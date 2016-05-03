/*
 * Copyright (c) 2008-2010, Atheros Communications Inc.
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include "opt_ah.h"

#ifdef AH_SUPPORT_AR9300

#include "ah.h"
#include "ah_internal.h"

#include "ar9300/ar9300.h"

/* shorthands to compact tables for readability */
#define    OFDM    IEEE80211_T_OFDM
#define    CCK    IEEE80211_T_CCK
#define    TURBO    IEEE80211_T_TURBO
#define    XR    ATHEROS_T_XR
#define HT      IEEE80211_T_HT

HAL_RATE_TABLE ar9300_11a_table = {
    8,  /* number of rates */
    { 0 },
    {
/*                                                  short              ctrl */
/*                valid                 rateCode Preamble    dot11Rate Rate */
/*   6 Mb */ {  AH_TRUE, OFDM,    6000,     0x0b,    0x00, (0x80 | 12),   0 },
/*   9 Mb */ {  AH_TRUE, OFDM,    9000,     0x0f,    0x00,          18,   0 },
/*  12 Mb */ {  AH_TRUE, OFDM,   12000,     0x0a,    0x00, (0x80 | 24),   2 },
/*  18 Mb */ {  AH_TRUE, OFDM,   18000,     0x0e,    0x00,          36,   2 },
/*  24 Mb */ {  AH_TRUE, OFDM,   24000,     0x09,    0x00, (0x80 | 48),   4 },
/*  36 Mb */ {  AH_TRUE, OFDM,   36000,     0x0d,    0x00,          72,   4 },
/*  48 Mb */ {  AH_TRUE, OFDM,   48000,     0x08,    0x00,          96,   4 },
/*  54 Mb */ {  AH_TRUE, OFDM,   54000,     0x0c,    0x00,         108,   4 }
    },
};

HAL_RATE_TABLE ar9300_11a_half_table = {
    8,  /* number of rates */
    { 0 },
    {
/*                                                  short              ctrl */
/*                valid                 rateCode Preamble    dot11Rate Rate */
/*   6 Mb */ {  AH_TRUE, OFDM,    3000,     0x0b,    0x00, (0x80 | 6),   0 },
/*   9 Mb */ {  AH_TRUE, OFDM,    4500,     0x0f,    0x00,          9,   0 },
/*  12 Mb */ {  AH_TRUE, OFDM,    6000,     0x0a,    0x00, (0x80 | 12),   2 },
/*  18 Mb */ {  AH_TRUE, OFDM,    9000,     0x0e,    0x00,          18,   2 },
/*  24 Mb */ {  AH_TRUE, OFDM,   12000,     0x09,    0x00, (0x80 | 24),   4 },
/*  36 Mb */ {  AH_TRUE, OFDM,   18000,     0x0d,    0x00,          36,   4 },
/*  48 Mb */ {  AH_TRUE, OFDM,   24000,     0x08,    0x00,          48,   4 },
/*  54 Mb */ {  AH_TRUE, OFDM,   27000,     0x0c,    0x00,          54,   4 }
    },
};

HAL_RATE_TABLE ar9300_11a_quarter_table = {
    8,  /* number of rates */
    { 0 },
    {
/*                                                  short              ctrl */
/*                valid                 rateCode Preamble    dot11Rate Rate */
/*  6 Mb */ {  AH_TRUE, OFDM,    1500,     0x0b,    0x00, (0x80 | 3),   0 },
/*  9 Mb */ {  AH_TRUE, OFDM,    2250,     0x0f,    0x00,         4 ,   0 },
/* 12 Mb */ {  AH_TRUE, OFDM,    3000,     0x0a,    0x00, (0x80 | 6),   2 },
/* 18 Mb */ {  AH_TRUE, OFDM,    4500,     0x0e,    0x00,          9,   2 },
/* 24 Mb */ {  AH_TRUE, OFDM,    6000,     0x09,    0x00, (0x80 | 12),   4 },
/* 36 Mb */ {  AH_TRUE, OFDM,    9000,     0x0d,    0x00,          18,   4 },
/* 48 Mb */ {  AH_TRUE, OFDM,   12000,     0x08,    0x00,          24,   4 },
/* 54 Mb */ {  AH_TRUE, OFDM,   13500,     0x0c,    0x00,          27,   4 }
    },
};

HAL_RATE_TABLE ar9300_turbo_table = {
    8,  /* number of rates */
    { 0 },
    {
/*                                                 short              ctrl */
/*                valid                rateCode Preamble    dot11Rate Rate */
/*   6 Mb */ {  AH_TRUE, TURBO,   6000,    0x0b,    0x00, (0x80 | 12),   0 },
/*   9 Mb */ {  AH_TRUE, TURBO,   9000,    0x0f,    0x00,          18,   0 },
/*  12 Mb */ {  AH_TRUE, TURBO,  12000,    0x0a,    0x00, (0x80 | 24),   2 },
/*  18 Mb */ {  AH_TRUE, TURBO,  18000,    0x0e,    0x00,          36,   2 },
/*  24 Mb */ {  AH_TRUE, TURBO,  24000,    0x09,    0x00, (0x80 | 48),   4 },
/*  36 Mb */ {  AH_TRUE, TURBO,  36000,    0x0d,    0x00,          72,   4 },
/*  48 Mb */ {  AH_TRUE, TURBO,  48000,    0x08,    0x00,          96,   4 },
/*  54 Mb */ {  AH_TRUE, TURBO,  54000,    0x0c,    0x00,         108,   4 }
    },
};

HAL_RATE_TABLE ar9300_11b_table = {
    4,  /* number of rates */
    { 0 },
    {
/*                                                 short              ctrl */
/*                valid                rateCode Preamble    dot11Rate Rate */
/*   1 Mb */ {  AH_TRUE,  CCK,    1000,    0x1b,    0x00, (0x80 |  2),   0 },
/*   2 Mb */ {  AH_TRUE,  CCK,    2000,    0x1a,    0x04, (0x80 |  4),   1 },
/* 5.5 Mb */ {  AH_TRUE,  CCK,    5500,    0x19,    0x04, (0x80 | 11),   1 },
/*  11 Mb */ {  AH_TRUE,  CCK,   11000,    0x18,    0x04, (0x80 | 22),   1 }
    },
};


/* Venice TODO: roundUpRate() is broken when the rate table does not represent
 * rates in increasing order  e.g.  5.5, 11, 6, 9.
 * An average rate of 6 Mbps will currently map to 11 Mbps.
 */
HAL_RATE_TABLE ar9300_11g_table = {
    12,  /* number of rates */
    { 0 },
    {
/*                                                 short              ctrl */
/*                valid                rateCode Preamble    dot11Rate Rate */
/*   1 Mb */ {  AH_TRUE, CCK,     1000,    0x1b,    0x00, (0x80 |  2),   0 },
/*   2 Mb */ {  AH_TRUE, CCK,     2000,    0x1a,    0x04, (0x80 |  4),   1 },
/* 5.5 Mb */ {  AH_TRUE, CCK,     5500,    0x19,    0x04, (0x80 | 11),   2 },
/*  11 Mb */ {  AH_TRUE, CCK,    11000,    0x18,    0x04, (0x80 | 22),   3 },
/* Hardware workaround - remove rates 6, 9 from rate ctrl */
/*   6 Mb */ { AH_FALSE, OFDM,    6000,    0x0b,    0x00,          12,   4 },
/*   9 Mb */ { AH_FALSE, OFDM,    9000,    0x0f,    0x00,          18,   4 },
/*  12 Mb */ {  AH_TRUE, OFDM,   12000,    0x0a,    0x00,          24,   6 },
/*  18 Mb */ {  AH_TRUE, OFDM,   18000,    0x0e,    0x00,          36,   6 },
/*  24 Mb */ {  AH_TRUE, OFDM,   24000,    0x09,    0x00,          48,   8 },
/*  36 Mb */ {  AH_TRUE, OFDM,   36000,    0x0d,    0x00,          72,   8 },
/*  48 Mb */ {  AH_TRUE, OFDM,   48000,    0x08,    0x00,          96,   8 },
/*  54 Mb */ {  AH_TRUE, OFDM,   54000,    0x0c,    0x00,         108,   8 }
    },
};

HAL_RATE_TABLE ar9300_xr_table = {
    13,        /* number of rates */
    { 0 },
    {
/*                                                 short              ctrl */
/*                valid                rateCode Preamble    dot11Rate Rate */
/* 0.25 Mb */{  AH_TRUE,   XR,    250,     0x03,    0x00, (0x80 |  1),   0, 612, 612 },
/* 0.5 Mb */ {  AH_TRUE,   XR,    500,     0x07,    0x00, (0x80 |  1),   0, 457, 457},
/*   1 Mb */ {  AH_TRUE,   XR,   1000,     0x02,    0x00, (0x80 |  2),   1, 228, 228 },
/*   2 Mb */ {  AH_TRUE,   XR,   2000,     0x06,    0x00, (0x80 |  4),   2, 160, 160,},
/*   3 Mb */ {  AH_TRUE,   XR,   3000,     0x01,    0x00, (0x80 |  6),   3, 140, 140 },
/*   6 Mb */ {  AH_TRUE, OFDM,    6000,    0x0b,    0x00, (0x80 | 12),   4, 60,  60  },
/*   9 Mb */ {  AH_TRUE, OFDM,    9000,    0x0f,    0x00,          18,   4, 60,  60  },
/*  12 Mb */ {  AH_TRUE, OFDM,   12000,    0x0a,    0x00, (0x80 | 24),   6, 48,  48  },
/*  18 Mb */ {  AH_TRUE, OFDM,   18000,    0x0e,    0x00,          36,   6, 48,  48  },
/*  24 Mb */ {  AH_TRUE, OFDM,   24000,    0x09,    0x00,          48,   8, 44,  44  },
/*  36 Mb */ {  AH_TRUE, OFDM,   36000,    0x0d,    0x00,          72,   8, 44,  44  },
/*  48 Mb */ {  AH_TRUE, OFDM,   48000,    0x08,    0x00,          96,   8, 44,  44  },
/*  54 Mb */ {  AH_TRUE, OFDM,   54000,    0x0c,    0x00,         108,   8, 44,  44  }
    },
};

HAL_RATE_TABLE ar9300_11ng_table = {

    36,  /* number of rates */
    { 0 },
    {
/*                                                 short              ctrl */
/*                valid                rateCode Preamble    dot11Rate Rate */
/*   1 Mb */ {  AH_TRUE, CCK,     1000,    0x1b,    0x00, (0x80 |  2),   0 },
/*   2 Mb */ {  AH_TRUE, CCK,     2000,    0x1a,    0x04, (0x80 |  4),   1 },
/* 5.5 Mb */ {  AH_TRUE, CCK,     5500,    0x19,    0x04, (0x80 | 11),   2 },
/*  11 Mb */ {  AH_TRUE, CCK,    11000,    0x18,    0x04, (0x80 | 22),   3 },
/* Hardware workaround - remove rates 6, 9 from rate ctrl */
/*   6 Mb */ { AH_FALSE, OFDM,    6000,    0x0b,    0x00,          12,   4 },
/*   9 Mb */ { AH_FALSE, OFDM,    9000,    0x0f,    0x00,          18,   4 },
/*  12 Mb */ {  AH_TRUE, OFDM,   12000,    0x0a,    0x00,          24,   6 },
/*  18 Mb */ {  AH_TRUE, OFDM,   18000,    0x0e,    0x00,          36,   6 },
/*  24 Mb */ {  AH_TRUE, OFDM,   24000,    0x09,    0x00,          48,   8 },
/*  36 Mb */ {  AH_TRUE, OFDM,   36000,    0x0d,    0x00,          72,   8 },
/*  48 Mb */ {  AH_TRUE, OFDM,   48000,    0x08,    0x00,          96,   8 },
/*  54 Mb */ {  AH_TRUE, OFDM,   54000,    0x0c,    0x00,         108,   8 },
/* 6.5 Mb */ {  AH_TRUE, HT,      6500,    0x80,    0x00,           0,   4 },
/*  13 Mb */ {  AH_TRUE, HT,     13000,    0x81,    0x00,           1,   6 },
/*19.5 Mb */ {  AH_TRUE, HT,     19500,    0x82,    0x00,           2,   6 },
/*  26 Mb */ {  AH_TRUE, HT,     26000,    0x83,    0x00,           3,   8 },
/*  39 Mb */ {  AH_TRUE, HT,     39000,    0x84,    0x00,           4,   8 },
/*  52 Mb */ {  AH_TRUE, HT,     52000,    0x85,    0x00,           5,   8 },
/*58.5 Mb */ {  AH_TRUE, HT,     58500,    0x86,    0x00,           6,   8 },
/*  65 Mb */ {  AH_TRUE, HT,     65000,    0x87,    0x00,           7,   8 },
/*  13 Mb */ {  AH_TRUE, HT,     13000,    0x88,    0x00,           8,   4 },
/*  26 Mb */ {  AH_TRUE, HT,     26000,    0x89,    0x00,           9,   6 },
/*  39 Mb */ {  AH_TRUE, HT,     39000,    0x8a,    0x00,          10,   6 },
/*  52 Mb */ {  AH_TRUE, HT,     52000,    0x8b,    0x00,          11,   8 },
/*  78 Mb */ {  AH_TRUE, HT,     78000,    0x8c,    0x00,          12,   8 },
/* 104 Mb */ {  AH_TRUE, HT,    104000,    0x8d,    0x00,          13,   8 },
/* 117 Mb */ {  AH_TRUE, HT,    117000,    0x8e,    0x00,          14,   8 },
/* 130 Mb */ {  AH_TRUE, HT,    130000,    0x8f,    0x00,          15,   8 },
/*19.5 Mb */ {  AH_TRUE, HT,     19500,    0x90,    0x00,          16,   4 },
/*  39 Mb */ {  AH_TRUE, HT,     39000,    0x91,    0x00,          17,   6 },
/*58.5 Mb */ {  AH_TRUE, HT,     58500,    0x92,    0x00,          18,   6 },
/*  78 Mb */ {  AH_TRUE, HT,     78000,    0x93,    0x00,          19,   8 },
/* 117 Mb */ {  AH_TRUE, HT,    117000,    0x94,    0x00,          20,   8 },
/* 156 Mb */ {  AH_TRUE, HT,    156000,    0x95,    0x00,          21,   8 },
/*175.5Mb */ {  AH_TRUE, HT,    175500,    0x96,    0x00,          22,   8 },
/* 195 Mb */ {  AH_TRUE, HT,    195000,    0x97,    0x00,          23,   8 },
    },
};

static HAL_RATE_TABLE ar9300_11na_table = {

    32,  /* number of rates */
    { 0 },
    {
/*                                                 short              ctrl */
/*                valid                rateCode Preamble    dot11Rate Rate */
/*   6 Mb */ {  AH_TRUE, OFDM,    6000,    0x0b,    0x00, (0x80 | 12),   0 },
/*   9 Mb */ {  AH_TRUE, OFDM,    9000,    0x0f,    0x00,          18,   0 },
/*  12 Mb */ {  AH_TRUE, OFDM,   12000,    0x0a,    0x00, (0x80 | 24),   2 },
/*  18 Mb */ {  AH_TRUE, OFDM,   18000,    0x0e,    0x00,          36,   2 },
/*  24 Mb */ {  AH_TRUE, OFDM,   24000,    0x09,    0x00, (0x80 | 48),   4 },
/*  36 Mb */ {  AH_TRUE, OFDM,   36000,    0x0d,    0x00,          72,   4 },
/*  48 Mb */ {  AH_TRUE, OFDM,   48000,    0x08,    0x00,          96,   4 },
/*  54 Mb */ {  AH_TRUE, OFDM,   54000,    0x0c,    0x00,         108,   4 },
/* 6.5 Mb */ {  AH_TRUE, HT,      6500,    0x80,    0x00,           0,   0 },
/*  13 Mb */ {  AH_TRUE, HT,     13000,    0x81,    0x00,           1,   2 },
/*19.5 Mb */ {  AH_TRUE, HT,     19500,    0x82,    0x00,           2,   2 },
/*  26 Mb */ {  AH_TRUE, HT,     26000,    0x83,    0x00,           3,   4 },
/*  39 Mb */ {  AH_TRUE, HT,     39000,    0x84,    0x00,           4,   4 },
/*  52 Mb */ {  AH_TRUE, HT,     52000,    0x85,    0x00,           5,   4 },
/*58.5 Mb */ {  AH_TRUE, HT,     58500,    0x86,    0x00,           6,   4 },
/*  65 Mb */ {  AH_TRUE, HT,     65000,    0x87,    0x00,           7,   4 },
/*  13 Mb */ {  AH_TRUE, HT,     13000,    0x88,    0x00,           8,   0 },
/*  26 Mb */ {  AH_TRUE, HT,     26000,    0x89,    0x00,           9,   2 },
/*  39 Mb */ {  AH_TRUE, HT,     39000,    0x8a,    0x00,          10,   2 },
/*  52 Mb */ {  AH_TRUE, HT,     52000,    0x8b,    0x00,          11,   4 },
/*  78 Mb */ {  AH_TRUE, HT,     78000,    0x8c,    0x00,          12,   4 },
/* 104 Mb */ {  AH_TRUE, HT,    104000,    0x8d,    0x00,          13,   4 },
/* 117 Mb */ {  AH_TRUE, HT,    117000,    0x8e,    0x00,          14,   4 },
/* 130 Mb */ {  AH_TRUE, HT,    130000,    0x8f,    0x00,          15,   4 },
/*19.5 Mb */ {  AH_TRUE, HT,     19500,    0x90,    0x00,          16,   0 },
/*  39 Mb */ {  AH_TRUE, HT,     39000,    0x91,    0x00,          17,   2 },
/*58.5 Mb */ {  AH_TRUE, HT,     58500,    0x92,    0x00,          18,   2 },
/*  78 Mb */ {  AH_TRUE, HT,     78000,    0x93,    0x00,          19,   4 },
/* 117 Mb */ {  AH_TRUE, HT,    117000,    0x94,    0x00,          20,   4 },
/* 156 Mb */ {  AH_TRUE, HT,    156000,    0x95,    0x00,          21,   4 },
/*175.5Mb */ {  AH_TRUE, HT,    175500,    0x96,    0x00,          22,   4 },
/* 195 Mb */ {  AH_TRUE, HT,    195000,    0x97,    0x00,          23,   4 },
    },
};

#undef    OFDM
#undef    CCK
#undef    TURBO
#undef    XR
#undef    HT
#undef    HT_HGI

const HAL_RATE_TABLE *
ar9300GetRateTable(struct ath_hal *ah, u_int mode)
{
    struct ath_hal_private *ahpriv = AH_PRIVATE(ah);
    HAL_CAPABILITIES *pCap = &ahpriv->ah_caps;
    HAL_RATE_TABLE *rt;

    switch (mode) {
    case HAL_MODE_11A:
        rt = &ar9300_11a_table;
        break;
    case HAL_MODE_11A_HALF_RATE:
        if (pCap->halChanHalfRate) {
            rt = &ar9300_11a_half_table;
            break;
        }
        return AH_NULL;
    case HAL_MODE_11A_QUARTER_RATE:
        if (pCap->halChanQuarterRate) {
            rt = &ar9300_11a_quarter_table;
            break;
        }
        return AH_NULL;
    case HAL_MODE_11B:
        rt = &ar9300_11b_table;
        break;
    case HAL_MODE_11G:
        rt =  &ar9300_11g_table;
        break;
    case HAL_MODE_TURBO:
    case HAL_MODE_108G:
        rt =  &ar9300_turbo_table;
        break;
    case HAL_MODE_XR:
        rt = &ar9300_xr_table;
        break;
    case HAL_MODE_11NG_HT20:
    case HAL_MODE_11NG_HT40PLUS:
    case HAL_MODE_11NG_HT40MINUS:
        rt = &ar9300_11ng_table;
        break;
    case HAL_MODE_11NA_HT20:
    case HAL_MODE_11NA_HT40PLUS:
    case HAL_MODE_11NA_HT40MINUS:
        rt = &ar9300_11na_table;
        break;
    default:
        HDPRINTF(ah, HAL_DBG_CHANNEL,
            "%s: invalid mode 0x%x\n", __func__, mode);
        return AH_NULL;
    }
    ath_hal_setupratetable(ah, rt);
    return rt;
}

int16_t
ar9300GetRateTxPower(struct ath_hal *ah, u_int mode, u_int8_t rate_index,
                     u_int8_t chainmask)
{
    struct ath_hal_9300 *ahp = AH9300(ah);
    int numChains;

    switch (chainmask) {
    case OSPREY_1_CHAINMASK:
        numChains = 1;
        break;
    case OSPREY_2LOHI_CHAINMASK:
    case OSPREY_2LOMID_CHAINMASK:
        numChains = 2;
        break;
    case OSPREY_3_CHAINMASK:
        numChains = 3;
        break;
    default:
        numChains = 3;
    }
    
    return ahp->txPower[rate_index][numChains-1];
}
#endif /* AH_SUPPORT_AR9300 */
