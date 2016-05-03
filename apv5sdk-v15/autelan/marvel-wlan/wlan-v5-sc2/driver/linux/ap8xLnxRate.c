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
#include "wl_mib.h"
#include "ap8xLnxRate.h"
static RATE_INFO_11N ar5416_11bRateTableInfo[] = {
     /*              Multi-strm       STBC            Valid for                                         short     dot11   ctrl  RssiAck  RssiAck  maxtx */
     /*              valid            Valid           UAPSD  TxBF                  Kbps   uKbps   RC   Preamble   Rate    Rate  ValidMin DeltaMin chain */
     /*   1 Mb */ {  TRUE_1_2_3,      FALSE_1_2_3,    TRUE,  FALSE,  WLAN_PHY_CCK,  1000,   900,  0x1b,   0x00, (0x80| 2),   0,    0,       1,     3,   0, 0},
     /*   2 Mb */ {  TRUE_1_2_3,      FALSE_1_2_3,    FALSE, FALSE,  WLAN_PHY_CCK,  2000,  1800,  0x1a,   0x04, (0x80| 4),   1,    1,       1,     3,   1, 0},
     /* 5.5 Mb */ {  TRUE_1_2_3,      FALSE_1_2_3,    FALSE, FALSE,  WLAN_PHY_CCK,  5500,  4300,  0x19,   0x04, (0x80|11),   1,    2,       2,     3,   2, 0},
     /*  11 Mb */ {  TRUE_1_2_3,      FALSE_1_2_3,    TRUE,  FALSE,  WLAN_PHY_CCK, 11000,  7100,  0x18,   0x04, (0x80|22),   1,    4,     100,     3,   3, 0},
};

static RATE_INFO_11N ar5416_11gRateTableInfo[] = {
     /*              Multi-strm       STBC            Valid for                                         short     dot11  ctrl  RssiAck  RssiAck  maxtx */
     /*              valid            Valid           UAPSD TxBF                    Kbps   uKbps   RC   Preamble  Rate   Rate  ValidMin DeltaMin chain */
     /*   1 Mb */ {  TRUE_1_2_3,      FALSE_1_2_3,    TRUE, FALSE,  WLAN_PHY_CCK,   1000,   900,  0x1b,  0x00,      2,     0,     0,       1,     3,   0, 0},
     /*   2 Mb */ {  TRUE_1_2_3,      FALSE_1_2_3,    FALSE,FALSE,  WLAN_PHY_CCK,   2000,  1900,  0x1a,  0x04,      4,     1,     1,       1,     3,   1, 0},
     /* 5.5 Mb */ {  TRUE_1_2_3,      FALSE_1_2_3,    FALSE,FALSE,  WLAN_PHY_CCK,   5500,  4900,  0x19,  0x04,     11,     2,     2,       2,     3,   2, 0},
     /*  11 Mb */ {  TRUE_1_2_3,      FALSE_1_2_3,    TRUE, FALSE,  WLAN_PHY_CCK,  11000,  8100,  0x18,  0x04,     22,     3,     3,       2,     3,   3, 0},
     /*   6 Mb */ {  FALSE_1_2_3,     FALSE_1_2_3,    FALSE,FALSE,  WLAN_PHY_OFDM,  6000,  5400,  0x0b,  0x00,     12,     4,     2,       1,     3,   4, 0},
     /*   9 Mb */ {  FALSE_1_2_3,     FALSE_1_2_3,    FALSE,FALSE,  WLAN_PHY_OFDM,  9000,  7800,  0x0f,  0x00,     18,     4,     3,       1,     3,   5, 0},
     /*  12 Mb */ {  TRUE_1_2_3,      FALSE_1_2_3,    FALSE,FALSE,  WLAN_PHY_OFDM, 12000, 10000,  0x0a,  0x00,     24,     6,     4,       1,     3,   6, 0},
     /*  18 Mb */ {  TRUE_1_2_3,      FALSE_1_2_3,    FALSE,FALSE,  WLAN_PHY_OFDM, 18000, 13900,  0x0e,  0x00,     36,     6,     6,       2,     3,   7, 0},
     /*  24 Mb */ {  TRUE_1_2_3,      FALSE_1_2_3,    TRUE, FALSE,  WLAN_PHY_OFDM, 24000, 17300,  0x09,  0x00,     48,     8,    10,       3,     3,   8, 0},
     /*  36 Mb */ {  TRUE_1_2_3,      FALSE_1_2_3,    FALSE,FALSE,  WLAN_PHY_OFDM, 36000, 23000,  0x0d,  0x00,     72,     8,    14,       3,     3,   9, 0},
     /*  48 Mb */ {  TRUE_1_2_3,      FALSE_1_2_3,    FALSE,FALSE,  WLAN_PHY_OFDM, 48000, 27400,  0x08,  0x00,     96,     8,    19,       3,     1,  10, 0},
     /*  54 Mb */ {  TRUE_1_2_3,      FALSE_1_2_3,    TRUE, FALSE,  WLAN_PHY_OFDM, 54000, 29300,  0x0c,  0x00,    108,     8,    23,       3,     1,  11, 0},
};

static RATE_INFO_11N ar5416_11ngRateTableInfo[] = {
     /*                               Triple(T)                                                                                                                                                   */
     /*              Triple(T)        Dual(D)         Valid    Valid                                                                                                                              */
     /*              Dual(D)          Single(S)       for      for                                                                short   dot11 ctrl RssiAck  RssiAck  maxtx base cw40   sgi   ht   4ms tx */
     /*              Single(S) strm   strm STBC       UAPSD    TxBF                                          Kbps    uKbps   RC   Preamb  Rate  Rate ValidMin DeltaMin chain Idx  Idx    Idx   Idx  limit  */
     /*    1 Mb */ {  TRUE_ALL_1_2_3, TRUE_ALL_1_2_3, TRUE,    FALSE,               WLAN_PHY_CCK,            1000,    900,  0x1b,  0x00,    2,   0,    0,       1,     3,     0,    0,    0,    0,      0},
     /*    2 Mb */ {  TRUE_ALL_1_2_3, TRUE_ALL_1_2_3, FALSE,   FALSE,               WLAN_PHY_CCK,            2000,   1900,  0x1a,  0x04,    4,   1,    1,       1,     3,     1,    1,    1,    1,      0},
     /*  5.5 Mb */ {  TRUE_ALL_1_2_3, TRUE_ALL_1_2_3, FALSE,   FALSE,               WLAN_PHY_CCK,            5500,   4900,  0x19,  0x04,   11,   2,    2,       2,     3,     2,    2,    2,    2,      0},
     /*   11 Mb */ {  TRUE_ALL_1_2_3, TRUE_ALL_1_2_3, TRUE,    FALSE,               WLAN_PHY_CCK,           11000,   8100,  0x18,  0x04,   22,   3,    3,       2,     3,     3,    3,    3,    3,      0},
     /*    6 Mb */ {  FALSE_1_2_3,    FALSE_1_2_3,    FALSE,   FALSE,               WLAN_PHY_OFDM,           6000,   5200,  0x0b,  0x00,   12,   4,    2,       1,     3,     4,    4,    4,    4,      0},
     /*    9 Mb */ {  FALSE_1_2_3,    FALSE_1_2_3,    FALSE,   FALSE,               WLAN_PHY_OFDM,           9000,   7500,  0x0f,  0x00,   18,   4,    3,       1,     3,     5,    5,    5,    5,      0},
     /*   12 Mb */ {  TRUE_1_2_3,     TRUE_1_2_3,     FALSE,   FALSE,               WLAN_PHY_OFDM,          12000,   9700,  0x0a,  0x00,   24,   6,    4,       1,     3,     6,    6,    6,    6,      0},
     /*   18 Mb */ {  TRUE_1_2_3,     TRUE_1_2_3,     FALSE,   FALSE,               WLAN_PHY_OFDM,          18000,  13600,  0x0e,  0x00,   36,   6,    6,       2,     3,     7,    7,    7,    7,      0},
     /*   24 Mb */ {  TRUE_1_2_3,     TRUE_1_2_3,     TRUE,    FALSE,               WLAN_PHY_OFDM,          24000,  17000,  0x09,  0x00,   48,   8,   10,       3,     3,     8,    8,    8,    8,      0},
     /*   36 Mb */ {  TRUE_1_2_3,     TRUE_1_2_3,     FALSE,   FALSE,               WLAN_PHY_OFDM,          36000,  22700,  0x0d,  0x00,   72,   8,   14,       3,     3,     9,    9,    9,    9,      0},
     /*   48 Mb */ {  TRUE_1_2_3,     TRUE_1_2_3,     FALSE,   FALSE,               WLAN_PHY_OFDM,          48000,  27100,  0x08,  0x00,   96,   8,   20,       3,     1,    10,   10,   10,   10,      0},
     /*   54 Mb */ {  TRUE_1_2_3,     TRUE_1_2_3,     TRUE,    FALSE,               WLAN_PHY_OFDM,          54000,  28900,  0x0c,  0x00,  108,   8,   23,       3,     1,    11,   11,   11,   11,      0},
     /*  6.5 Mb */ {  FALSE_1_2_3,    FALSE_1_2_3,    TRUE,    TRUE20_N_1_2_ALL,    WLAN_PHY_HT_20_SS,       6500,   5800,  0x80,  0x00,    0,   4,    2,       3,     3,    12,   42,   12,   42,   3210},
     /*   13 Mb */ {  TRUE20_1_2_3,   TRUE20_1_2_3,   TRUE,    TRUE20_N_1_2_ALL,    WLAN_PHY_HT_20_SS,      13000,  11700,  0x81,  0x00,    1,   6,    4,       3,     3,    13,   43,   13,   43,   6430},
     /* 19.5 Mb */ {  TRUE20_1_2_3,   TRUE20_1_2_3,   FALSE,   TRUE20_N_1_2_ALL,    WLAN_PHY_HT_20_SS,      19500,  17600,  0x82,  0x00,    2,   6,    6,       3,     3,    14,   44,   14,   44,   9650},
     /*   26 Mb */ {  TRUE20_1_2,     TRUE20_1_2,     FALSE,   TRUE20_N_1_2_ALL,    WLAN_PHY_HT_20_SS,      26000,  23500,  0x83,  0x00,    3,   8,   10,       3,     3,    15,   45,   15,   45,  12880},
     /*   39 Mb */ {  TRUE20_1_2,     TRUE20_1_2,     TRUE,    TRUE20_N_1_2_ALL,    WLAN_PHY_HT_20_SS,      39000,  35300,  0x84,  0x00,    4,   8,   14,       3,     3,    16,   46,   16,   46,  19320},
     /*   52 Mb */ {  TRUE20_1,       TRUE20_1_2,     FALSE,   TRUE20_N2_F_N1_D_S,  WLAN_PHY_HT_20_SS,      52000,  47100,  0x85,  0x00,    5,   8,   20,       3,     1,    17,   47,   17,   47,  25760},
     /* 58.5 Mb */ {  TRUE20_1,       TRUE20_1_2,     FALSE,   TRUE20_N2_F_N1_D_S,  WLAN_PHY_HT_20_SS,      58500,  52900,  0x86,  0x00,    6,   8,   23,       3,     1,    18,   48,   18,   48,  28980},
     /*   65 Mb */ {  TRUE20_1,       TRUE20_1,       TRUE,    TRUE20_N2_F_N1_S,    WLAN_PHY_HT_20_SS,      65000,  58800,  0x87,  0x00,    7,   8,   25,       3,     1,    19,   49,   20,   50,  32200},
     /*   75 Mb */ {  TRUE20_1,       TRUE20_1,       TRUE,    TRUE20_N2_F_N1_S,    WLAN_PHY_HT_20_SS_HGI,  72200,  65400,  0x87,  0x00,    7,   8,   25,       3,     2,    19,   49,   20,   50,  35780},
     /*   13 Mb */ {  FALSE_1_2_3,    FALSE_1_2_3,    TRUE,    FALSE,               WLAN_PHY_HT_20_DS,      13000,  11600,  0x88,  0x00,    8,   4,    2,       3,     3,    20,   51,   21,   51,   6430},
     /*   26 Mb */ {  TRUE20_3,       TRUE20_3,       FALSE,   FALSE,               WLAN_PHY_HT_20_DS,      26000,  23400,  0x89,  0x00,    9,   6,    4,       3,     3,    21,   52,   22,   52,  12860},
     /*   39 Mb */ {  TRUE20_3,       TRUE20_3,       TRUE,    FALSE,               WLAN_PHY_HT_20_DS,      39000,  35200,  0x8a,  0x00,   10,   6,    6,       3,     3,    22,   53,   23,   53,  19300},
     /*   52 Mb */ {  TRUE20_2_3,     TRUE20_3,       FALSE,   TRUE20_N2_ALL_N1_T,  WLAN_PHY_HT_20_DS,      52000,  47000,  0x8b,  0x00,   11,   8,   10,       3,     3,    23,   54,   24,   54,  25730},
     /*   78 Mb */ {  TRUE20_2_3,     TRUE20_2_3,     TRUE,    TRUE20_N2_ALL_N1_T_D,WLAN_PHY_HT_20_DS,      78000,  70500,  0x8c,  0x00,   12,   8,   14,       3,     3,    24,   55,   25,   55,  38600},
     /*  104 Mb */ {  TRUE20_2_3,     TRUE20_2_3,     FALSE,   TRUE20_N2_ALL_N1_T_D,WLAN_PHY_HT_20_DS,     104000,  94000,  0x8d,  0x00,   13,   8,   20,       3,     2,    25,   56,   26,   56,  51470},
     /*  117 Mb */ {  TRUE20_2_3,     TRUE20_2_3,     FALSE,   TRUE20_N2_ALL_N1_T_D,WLAN_PHY_HT_20_DS,     117000, 105200,  0x8e,  0x00,   14,   8,   23,       3,     2,    26,   57,   27,   57,  57910},
     /*  130 Mb */ {  TRUE20_2_3,     TRUE20_2_3,     TRUE,    TRUE20_N2_ALL_N1_T_D,WLAN_PHY_HT_20_DS,     130000, 116100,  0x8f,  0x00,   15,   8,   25,       3,     2,    27,   58,   29,   59,  64340},
     /*144.4 Mb */ {  TRUE20_2_3,     TRUE20_2_3,     TRUE,    TRUE20_N2_ALL_N1_T_D,WLAN_PHY_HT_20_DS_HGI, 144400, 128100,  0x8f,  0x00,   15,   8,   25,       3,     2,    27,   58,   29,   59,  71490},
     /* 19.5 Mb */ {  FALSE_1_2_3,    FALSE_1_2_3,    FALSE,   FALSE,               WLAN_PHY_HT_20_TS,      19500,  17400,  0x90,  0x00,   16,   4,   25,       3,     3,    28,   60,   30,   60,   9630},
     /*   39 Mb */ {  FALSE_1_2_3,    FALSE_1_2_3,    FALSE,   FALSE,               WLAN_PHY_HT_20_TS,      39000,  35100,  0x91,  0x00,   17,   6,   25,       3,     3,    29,   61,   31,   61,  19260},
     /* 58.5 Mb */ {  FALSE_1_2_3,    FALSE_1_2_3,    FALSE,   FALSE,               WLAN_PHY_HT_20_TS,      58500,  52600,  0x92,  0x00,   18,   6,   25,       3,     3,    30,   62,   32,   62,  28890},
     /*   78 Mb */ {  FALSE_1_2_3,    FALSE_1_2_3,    FALSE,   FALSE,               WLAN_PHY_HT_20_TS,      78000,  70400,  0x93,  0x00,   19,   8,   25,       3,     3,    31,   63,   33,   63,  38520},
     /*  117 Mb */ {  FALSE_1_2_3,    FALSE_1_2_3,    FALSE,   FALSE,               WLAN_PHY_HT_20_TS,     117000, 104900,  0x94,  0x00,   20,   8,   25,       3,     3,    32,   64,   35,   65,  57790},
     /*  130 Mb */ {  FALSE_1_2_3,    FALSE_1_2_3,    FALSE,   FALSE,               WLAN_PHY_HT_20_TS_HGI, 130000, 115800,  0x94,  0x00,   20,   8,   25,       3,     3,    32,   64,   35,   65,  64210},
     /*  156 Mb */ {  TRUE20_3,       TRUE20_3,       TRUE,    TRUE20_N2_T_N1_T,    WLAN_PHY_HT_20_TS,     156000, 137200,  0x95,  0x00,   21,   8,   25,       3,     3,    33,   66,   37,   67,  77060},
     /*173.3 Mb */ {  TRUE20_3,       TRUE20_3,       TRUE,    TRUE20_N2_T_N1_T,    WLAN_PHY_HT_20_TS_HGI, 173300, 151100,  0x95,  0x00,   21,   8,   25,       3,     3,    33,   66,   37,   67,  85620},
     /*175.5 Mb */ {  TRUE20_3,       TRUE20_3,       FALSE,   TRUE20_N2_T_N1_T,    WLAN_PHY_HT_20_TS,     175500, 152800,  0x96,  0x00,   22,   8,   25,       3,     3,    34,   68,   39,   69,  86690},
     /*  195 Mb */ {  TRUE20_3,       TRUE20_3,       FALSE,   TRUE20_N2_T_N1_T,    WLAN_PHY_HT_20_TS_HGI, 195000, 168400,  0x96,  0x00,   22,   8,   25,       3,     3,    34,   68,   39,   69,  96320},
     /*  195 Mb */ {  TRUE20_3,       TRUE20_3,       TRUE,    TRUE20_N2_T_N1_T,    WLAN_PHY_HT_20_TS,     195000, 168400,  0x97,  0x00,   23,   8,   25,       3,     3,    35,   70,   41,   71,  96320},
     /*216.7 Mb */ {  TRUE20_3,       TRUE20_3,       TRUE,    TRUE20_N2_T_N1_T,    WLAN_PHY_HT_20_TS_HGI, 216700, 185000,  0x97,  0x00,   23,   8,   25,       3,     3,    35,   70,   41,   71, 107030},
     /* 13.5 Mb */ {  TRUE40_1_2_3,   TRUE40_1_2_3,   TRUE,    TRUE40_N_1_2_ALL,    WLAN_PHY_HT_40_SS,      13500,  12100,  0x80,  0x00,    0,   8,    2,       3,     3,    12,   42,   42,   42,   6680},
     /* 27.0 Mb */ {  TRUE40_1_2_3,   TRUE40_1_2_3,   TRUE,    TRUE40_N_1_2_ALL,    WLAN_PHY_HT_40_SS,      27000,  24300,  0x81,  0x00,    1,   8,    4,       3,     3,    13,   43,   43,   43,  13370},
     /* 40.5 Mb */ {  TRUE40_1_2_3,   TRUE40_1_2_3,   FALSE,   TRUE40_N_1_2_ALL,    WLAN_PHY_HT_40_SS,      40500,  36500,  0x82,  0x00,    2,   8,    6,       3,     3,    14,   44,   44,   44,  20060},
     /*   54 Mb */ {  TRUE40_1_2,     TRUE40_1_2,     FALSE,   TRUE40_N_1_2_ALL,    WLAN_PHY_HT_40_SS,      54000,  48900,  0x83,  0x00,    3,   8,   10,       3,     3,    15,   45,   45,   45,  26750},
     /*   81 Mb */ {  TRUE40_1_2,     TRUE40_1_2,     TRUE,    TRUE40_N_1_2_ALL,    WLAN_PHY_HT_40_SS,      81000,  73300,  0x84,  0x00,    4,   8,   14,       3,     3,    16,   46,   46,   46,  40130},
     /*  108 Mb */ {  TRUE40_1,       TRUE40_1_2,     FALSE,   TRUE40_N2_F_N1_D_S,  WLAN_PHY_HT_40_SS,     108000,  97500,  0x85,  0x00,    5,   8,   20,       3,     1,    17,   47,   47,   47,  53510},
     /* 121.5Mb */ {  TRUE40_1,       TRUE40_1_2,     FALSE,   TRUE40_N2_F_N1_D_S,  WLAN_PHY_HT_40_SS,     121500, 109100,  0x86,  0x00,    6,   8,   23,       3,     1,    18,   48,   48,   48,  60200},
     /*  135 Mb */ {  TRUE40_1,       TRUE40_1,       TRUE,    TRUE40_N2_F_N1_S,    WLAN_PHY_HT_40_SS,     135000, 120400,  0x87,  0x00,    7,   8,   23,       3,     1,    19,   49,   50,   50,  66880},
     /*  150 Mb */ {  TRUE40_1,       TRUE40_1,       TRUE,    TRUE40_N2_F_N1_S,    WLAN_PHY_HT_40_SS_HGI, 150000, 133000,  0x87,  0x00,    7,   8,   25,       3,     1,    19,   49,   50,   50,  74320},
     /*   27 Mb */ {  FALSE_1_2_3,    FALSE_1_2_3,    TRUE,    FALSE,               WLAN_PHY_HT_40_DS,      27000,  24100,  0x88,  0x00,    8,   8,    2,       3,     3,    20,   51,   51,   51,  13360},
     /*   54 Mb */ {  TRUE40_3,       TRUE40_3,       FALSE,   FALSE,               WLAN_PHY_HT_40_DS,      54000,  48700,  0x89,  0x00,    9,   8,    4,       3,     3,    21,   52,   52,   52,  26720},
     /*   81 Mb */ {  TRUE40_3,       TRUE40_3,       TRUE,    FALSE,               WLAN_PHY_HT_40_DS,      81000,  73000,  0x8a,  0x00,   10,   8,    6,       3,     3,    22,   53,   53,   53,  40090},
     /*  108 Mb */ {  TRUE40_2_3,     TRUE40_3,       FALSE,   TRUE40_N2_ALL_N1_T,  WLAN_PHY_HT_40_DS,     108000,  97400,  0x8b,  0x00,   11,   8,   10,       3,     3,    23,   54,   54,   54,  53450},
     /*  162 Mb */ {  TRUE40_2_3,     TRUE40_2_3,     TRUE,    TRUE40_N2_ALL_N1_T_D,WLAN_PHY_HT_40_DS,     162000, 142400,  0x8c,  0x00,   12,   8,   14,       3,     3,    24,   55,   55,   55,  80180},
     /*  216 Mb */ {  TRUE40_2_3,     TRUE40_2_3,     FALSE,   TRUE40_N2_ALL_N1_T_D,WLAN_PHY_HT_40_DS,     216000, 185300,  0x8d,  0x00,   13,   8,   20,       3,     2,    25,   56,   56,   56, 106910},
     /*  243 Mb */ {  TRUE40_2_3,     TRUE40_2_3,     FALSE,   TRUE40_N2_ALL_N1_T_D,WLAN_PHY_HT_40_DS,     243000, 206000,  0x8e,  0x00,   14,   8,   23,       3,     2,    26,   57,   57,   57, 120280},
     /*  270 Mb */ {  TRUE40_2_3,     TRUE40_2_3,     TRUE,    TRUE40_N2_ALL_N1_T_D,WLAN_PHY_HT_40_DS,     270000, 225800,  0x8f,  0x00,   15,   8,   23,       3,     2,    27,   58,   59,   59, 133640},
     /*  300 Mb */ {  TRUE40_2_3,     TRUE40_2_3,     TRUE,    TRUE40_N2_ALL_N1_T_D,WLAN_PHY_HT_40_DS_HGI, 300000, 247800,  0x8f,  0x00,   15,   8,   25,       3,     2,    27,   58,   59,   59, 148490},
     /* 40.5 Mb */ {  FALSE_1_2_3,    FALSE_1_2_3,    FALSE,   FALSE,               WLAN_PHY_HT_40_TS,      40500,  36100,  0x90,  0x00,   16,   8,   25,       3,     3,    28,   60,   60,   60,  20000},
     /*   81 Mb */ {  FALSE_1_2_3,    FALSE_1_2_3,    FALSE,   FALSE,               WLAN_PHY_HT_40_TS,      81000,  72900,  0x91,  0x00,   17,   8,   25,       3,     3,    29,   61,   61,   61,  40010},
     /*121.5 Mb */ {  FALSE_1_2_3,    FALSE_1_2_3,    FALSE,   FALSE,               WLAN_PHY_HT_40_TS,     121500, 108300,  0x92,  0x00,   18,   8,   25,       3,     3,    30,   62,   62,   62,  60010},
     /*  162 Mb */ {  FALSE_1_2_3,    FALSE_1_2_3,    FALSE,   FALSE,               WLAN_PHY_HT_40_TS,     162000, 142000,  0x93,  0x00,   19,   8,   25,       3,     3,    31,   63,   63,   63,  80020},
     /*  243 Mb */ {  FALSE_1_2_3,    FALSE_1_2_3,    FALSE,   FALSE,               WLAN_PHY_HT_40_TS,     243000, 205100,  0x94,  0x00,   20,   8,   25,       3,     3,    32,   64,   65,   65, 120030},
     /*  270 Mb */ {  FALSE_1_2_3,    FALSE_1_2_3,    FALSE,   FALSE,               WLAN_PHY_HT_40_TS_HGI, 270000, 224700,  0x94,  0x00,   20,   8,   25,       3,     3,    32,   64,   65,   65, 133370},
     /*  324 Mb */ {  TRUE40_3,       TRUE40_3,       TRUE,    TRUE40_N2_T_N1_T,    WLAN_PHY_HT_40_TS,     324000, 263100,  0x95,  0x00,   21,   8,   25,       3,     3,    33,   66,   67,   67, 160050},
     /*  360 Mb */ {  TRUE40_3,       TRUE40_3,       TRUE,    TRUE40_N2_T_N1_T,    WLAN_PHY_HT_40_TS_HGI, 360000, 288000,  0x95,  0x00,   21,   8,   25,       3,     3,    33,   66,   67,   67, 177830},
     /*364.5 Mb */ {  TRUE40_3,       TRUE40_3,       FALSE,   TRUE40_N2_T_N1_T,    WLAN_PHY_HT_40_TS,     364500, 290700,  0x96,  0x00,   22,   8,   25,       3,     3,    34,   68,   69,   69, 180060},
     /*  405 Mb */ {  TRUE40_3,       TRUE40_3,       FALSE,   TRUE40_N2_T_N1_T,    WLAN_PHY_HT_40_TS_HGI, 405000, 317200,  0x96,  0x00,   22,   8,   25,       3,     3,    34,   68,   69,   69, 200060},
     /*  405 Mb */ {  TRUE40_3,       TRUE40_3,       TRUE,    TRUE40_N2_T_N1_T,    WLAN_PHY_HT_40_TS,     405000, 317200,  0x97,  0x00,   23,   8,   25,       3,     3,    35,   70,   71,   71, 200060},
     /*  450 Mb */ {  TRUE40_3,       TRUE40_3,       TRUE,    TRUE40_N2_T_N1_T,    WLAN_PHY_HT_40_TS_HGI, 450000, 346400,  0x97,  0x00,   23,   8,   25,       3,     3,    35,   70,   71,   71, 222290},
};

/*
ic_rate_mask(V5):
0               1               2               3               4               5               6               7               8
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
| |     CAP     |         basic_G       |        rate_G         |               |basic_B| rate_B|    basic_A    |     rate_A    |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
6463   6059585756                       44                      32              24      20      16              8               0 
switch  n g b a(rate select)



BssBasicRateMask and NotBssBasicRateMask (marvell)
0               1               2               3               4
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
| |     CAP     |                     |               | |rate_B |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
3231            24                    13              5 4       0 
 switch                          {108,96,72,48,36,24,18,12,44,22,11,4,2}(bit)
 
*/


int ieee80211_get_current_rate(struct net_device *dev,struct iw_request_info *info, struct iw_param *bitrate, char *extra)
{

    RATE_INFO_11N *hwRateTable ;
    RATE_INFO_11N rateinfor;
    struct wlprivate *priv = NETDEV_PRIV_P(struct wlprivate, dev);
    vmacApInfo_t *vmacSta_p = priv->vmacSta_p;
    MIB_802DOT11 *mib = vmacSta_p->ShadowMib802dot11;
    int rate_num,i;


    WLDBG_ENTER_INFO(DBG_LEVEL_1, "");
    
    rateinfor.validSS =0x0; 
    rateinfor.validDS =0x0;
    rateinfor.validTS =0x0;
    rateinfor.phy =0x0;
    rate_num = 0; 
    i = 0;
    switch (*(mib->mib_ApMode))
    {
        case AP_MODE_B_ONLY:
            hwRateTable= ar5416_11bRateTableInfo; 
            rate_num = sizeof(ar5416_11bRateTableInfo)/sizeof(RATE_INFO_11N); /* number of rates */
            for (i=0;i<rate_num-1;i++)
            {
                hwRateTable++;
            }
            bitrate->value = hwRateTable->rateKbps;
            break;
  
        case AP_MODE_G_ONLY:
        case AP_MODE_MIXED :
        case AP_MODE_A_ONLY:
        case AP_MODE_AandG :    
            hwRateTable= ar5416_11gRateTableInfo; 
            rate_num = sizeof(ar5416_11gRateTableInfo)/sizeof(RATE_INFO_11N); /* number of rates */
            for (i=0;i<rate_num-1;i++)
            {
                hwRateTable++;
            }
            bitrate->value = hwRateTable->rateKbps;
            break;

        case AP_MODE_N_ONLY:
        case AP_MODE_BandN:
        case AP_MODE_GandN:
        case AP_MODE_BandGandN:
        case AP_MODE_AandN:
            hwRateTable= ar5416_11ngRateTableInfo; 
            rate_num = sizeof(ar5416_11ngRateTableInfo)/sizeof(RATE_INFO_11N); /* number of rates */
            for (i=0;i<rate_num-1;i++)
            {
                hwRateTable++;
            }
            bitrate->value = hwRateTable->rateKbps;
            /* mib_rxAntenna 0:ABC(Auto), 1:A, 5:B, 4:C 2:AB, 3:ABC, 6:BC, 7:AC */
            if(*(mib->mib_rxAntenna) == 0 || *(mib->mib_rxAntenna) == 3)
            {
                if((mib->PhyDSSSTable->Chanflag.ChnlWidth) == CH_40_MHz_WIDTH ||(mib->PhyDSSSTable->Chanflag.ChnlWidth) == CH_AUTO_WIDTH)
                { 
                    if(*mib->mib_guardInterval==1 || *mib->mib_guardInterval==0) // shortgi
                    {
                        rateinfor.validTS = 0x4;
                        rateinfor.phy = WLAN_PHY_HT_40_TS_HGI;
                    }
                    else if(*mib->mib_guardInterval==2 )
                    {
                        rateinfor.validTS = 0x4;
                        rateinfor.phy = WLAN_PHY_HT_40_TS;
                    }
                    else
                        goto error;
                }
                else if(mib->PhyDSSSTable->Chanflag.ChnlWidth == CH_20_MHz_WIDTH) 
                {
                    if(*mib->mib_guardInterval==1|| *mib->mib_guardInterval==0) // shortgi
                    {
                        rateinfor.validTS = 0x2;
                        rateinfor.phy = WLAN_PHY_HT_20_TS_HGI;
                    }
                    else if(*mib->mib_guardInterval==2 )
                    {
                        rateinfor.validTS = 0x2;
                        rateinfor.phy = WLAN_PHY_HT_20_TS;
                    }
                    else
                        goto error;
                }
                else
                    goto error;
            }
            else if(*mib->mib_rxAntenna == 2|| *(mib->mib_rxAntenna) == 6|| *(mib->mib_rxAntenna) == 7)
            {
                if(mib->PhyDSSSTable->Chanflag.ChnlWidth == CH_40_MHz_WIDTH||(mib->PhyDSSSTable->Chanflag.ChnlWidth) == CH_AUTO_WIDTH)
                { 
                    if(*mib->mib_guardInterval==1|| *mib->mib_guardInterval==0) // shortgi
                    {
                        rateinfor.validDS = 0x4;
                        rateinfor.validTS = 0x4;
                        rateinfor.phy = WLAN_PHY_HT_40_DS_HGI;
                    }
                    else if(*mib->mib_guardInterval==2 )
                    {
                        rateinfor.validDS = 0x4;
                        rateinfor.validTS = 0x4;
                        rateinfor.phy = WLAN_PHY_HT_40_DS;
                    }
                    else
                        goto error;
                }
                else if(mib->PhyDSSSTable->Chanflag.ChnlWidth == CH_20_MHz_WIDTH) 
                {
                    if(*mib->mib_guardInterval==1|| *mib->mib_guardInterval==0) // shortgi
                    {
                        rateinfor.validDS = 0x2;
                        rateinfor.validTS = 0x2;
                        rateinfor.phy = WLAN_PHY_HT_20_DS_HGI;
                    }
                    else if(*mib->mib_guardInterval==2 )
                    {
                        rateinfor.validDS = 0x2;
                        rateinfor.validTS = 0x2;
                        rateinfor.phy = WLAN_PHY_HT_20_DS;
                    }
                    else
                        goto error;
                }
                else
                    goto error;
            }
            else if(*mib->mib_rxAntenna == 1|| *(mib->mib_rxAntenna) == 4|| *(mib->mib_rxAntenna) == 5)
            {
                if(mib->PhyDSSSTable->Chanflag.ChnlWidth == CH_40_MHz_WIDTH||(mib->PhyDSSSTable->Chanflag.ChnlWidth) == CH_AUTO_WIDTH)
                { 
                    if(*mib->mib_guardInterval==1|| *mib->mib_guardInterval==0) // shortgi
                    {
                        rateinfor.validSS = 0x4;
                        rateinfor.phy = WLAN_PHY_HT_40_SS_HGI;
                    }
                    else if(*mib->mib_guardInterval==2 )
                    {
                        rateinfor.validSS = 0x4;
                        rateinfor.phy = WLAN_PHY_HT_40_SS;
                    }
                    else
                        goto error;
                }
                else if(mib->PhyDSSSTable->Chanflag.ChnlWidth == CH_20_MHz_WIDTH) 
                {
                    if(*mib->mib_guardInterval==1|| *mib->mib_guardInterval==0) // shortgi
                    {
                        rateinfor.validSS = 0x2;
                        rateinfor.phy = WLAN_PHY_HT_20_SS_HGI;
                    }
                    else if(*mib->mib_guardInterval==2 )
                    {
                        rateinfor.validSS = 0x2;
                        rateinfor.phy = WLAN_PHY_HT_20_SS;
                    }
                    else
                        goto error;
                }
            }
            else
                goto error;
              
            for (i=rate_num-1;i>0;i--)
            {
                if((hwRateTable->validSS ==rateinfor.validSS )&&
                    (hwRateTable->validDS==rateinfor.validDS )&&
                    (hwRateTable->validTS==rateinfor.validTS )&&
                    (hwRateTable->phy==rateinfor.phy))
                    break;
                else
                    hwRateTable--;

            }
            
            if (i<15) //if i<15 that's mean wo got a error value
                goto error;
            else
                bitrate->value = hwRateTable->rateKbps;
            
            break;

        default:
            bitrate->value = 0;
            break;
    }

    bitrate->value *=1000;
    
    WLDBG_EXIT(DBG_LEVEL_1); 
    return 0;
    
error:
    bitrate->value =0;
    return 0;
}

/******** GET rates container,return num*********/
unsigned int ieee80211_get_rates_mask(vmacApInfo_t *vmacSta_p,int flag)
{

    MIB_802DOT11 *mib;
    unsigned int rateMask;

    if(vmacSta_p == NULL)
    {
        WlLogPrint(MARVEL_DEBUG_WARNING, __func__,"netdev is NULL");
        return 0;
    }

    mib = vmacSta_p->ShadowMib802dot11;
    rateMask = 0;

    if(flag == 1)       //basic
        rateMask = *(mib->BssBasicRateMask);
    else if(flag == 2)  //notBasic
        rateMask = *(mib->NotBssBasicRateMask);
    else if(flag == 3)  //basic | notbasic
        rateMask = *(mib->BssBasicRateMask) | *(mib->NotBssBasicRateMask);

    if(*(mib->BssBasicRateMask)&(unsigned int)1<<(8*sizeof(unsigned int)-1))    //enable cap switch
    {

        switch(*(mib->mib_ApMode))
        {
            case AP_MODE_B_ONLY:
                rateMask &= MRVL_BSSBASICRATEMASK_BGN;  //get B rate bit;
                break;

            case AP_MODE_MIXED:
            case AP_MODE_BandN:
            case AP_MODE_BandGandN:
                rateMask &= 0x1FEF;     //get B/G/N rate bit;
                break;

            default:
                rateMask &= MRVL_NOTBSSBASICRATEMASK_BGN;   //get G/N rate bit;
                break;
        }
    }
    else    //disable cap switch ,ratemask is rate container;
    {
        switch(*(mib->mib_ApMode))
        {
            case AP_MODE_B_ONLY:
                if(flag == 1)
                    rateMask = MRVL_BSSBASICRATEMASK_B;  //get B rate bit;
                else if(flag == 2)
                    rateMask = MRVL_NOTBSSBASICRATEMASK_B;
                else if(flag == 3)
                    rateMask = MRVL_BSSBASICRATEMASK_B | MRVL_NOTBSSBASICRATEMASK_B;
                break;

            case AP_MODE_MIXED:
            case AP_MODE_BandN:
            case AP_MODE_BandGandN:
                if(flag == 1)
                    rateMask = MRVL_BSSBASICRATEMASK_BGN;  //get B rate bit;
                else if(flag == 2)
                    rateMask = MRVL_NOTBSSBASICRATEMASK_BGN;
                else if(flag == 3)
                    rateMask = MRVL_BSSBASICRATEMASK_BGN | MRVL_NOTBSSBASICRATEMASK_BGN;
                break;

            default:
                if(flag == 1)
                    rateMask = MRVL_BSSBASICRATEMASK_G;  //get B rate bit;
                else if(flag == 2)
                    rateMask = MRVL_NOTBSSBASICRATEMASK_G;
                else if(flag == 3)
                    rateMask = MRVL_BSSBASICRATEMASK_G | MRVL_NOTBSSBASICRATEMASK_G;
                break;
        }
    }

    //WlLogPrint(MARVEL_DEBUG_ALL, __func__,"mask = %x\n",rateMask);
    return rateMask;
}


/********* store rate cap bit ***********/
int ieee80211_reset_rate_mask(MIB_802DOT11 *mib)
{

    if((mib == NULL)||(mib->mib_ApMode == NULL)||(mib->BssBasicRateMask == NULL )||(mib->NotBssBasicRateMask== NULL))
    {
        WlLogPrint(MARVEL_DEBUG_WARNING, __func__,"mib is NULL");
        return -1;
    }

    switch (*(mib->mib_ApMode))
    {
        case AP_MODE_B_ONLY:
            *(mib->BssBasicRateMask) &= 0x80000000;
            *(mib->NotBssBasicRateMask) &= 0x80000000;
            *(mib->BssBasicRateMask) |= MRVL_BSSBASICRATEMASK_B;
            *(mib->NotBssBasicRateMask) |= MRVL_NOTBSSBASICRATEMASK_B;
            *(mib->mib_shortSlotTime) = FALSE;
            *(mib->mib_ManagementRate) = 2;
            break;

        case AP_MODE_G_ONLY:
        case AP_MODE_N_ONLY://add by wangxl 20121218       
        case AP_MODE_GandN ://add by wangxl 20121218
            *(mib->BssBasicRateMask) &= 0x80000000;
            *(mib->NotBssBasicRateMask) &= 0x80000000;
            *(mib->BssBasicRateMask) |= MRVL_BSSBASICRATEMASK_G;
            *(mib->NotBssBasicRateMask) |= MRVL_NOTBSSBASICRATEMASK_G;
            *(mib->mib_shortSlotTime) = TRUE;
            *(mib->mib_ManagementRate) = 12;
            break;

        case AP_MODE_A_ONLY:
        case AP_MODE_AandN:
        case AP_MODE_5GHZ_N_ONLY:
            *(mib->BssBasicRateMask) &= 0x80000000;
            *(mib->NotBssBasicRateMask) &= 0x80000000;
            *(mib->BssBasicRateMask) |= MRVL_BSSBASICRATEMASK_A;
            *(mib->NotBssBasicRateMask) |= MRVL_NOTBSSBASICRATEMASK_A;
            *(mib->mib_shortSlotTime) = TRUE;
            *(mib->mib_ManagementRate) = 12;
            break;

      //case AP_MODE_N_ONLY:delete by wangxl 20121218
        case AP_MODE_MIXED:
        default:
            *(mib->BssBasicRateMask) &= 0x80000000;
            *(mib->NotBssBasicRateMask) &= 0x80000000;
            *(mib->BssBasicRateMask) |= MRVL_BSSBASICRATEMASK_BGN;
            *(mib->NotBssBasicRateMask) |= MRVL_NOTBSSBASICRATEMASK_BGN;
            *(mib->mib_shortSlotTime) = TRUE;
            *(mib->mib_ManagementRate) = 2;
            break;
    }

    return 0;
}


