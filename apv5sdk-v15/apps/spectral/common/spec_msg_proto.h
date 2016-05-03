/*
 * =====================================================================================
 *
 *       Filename:  spec_msg_proto.h
 *
 *    Description:  Spectral Messaging Protocol Header file
 *
 *        Version:  1.0
 *        Created:  Thursday 13 August 2009 11:08:20  IST
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  S.Karthikeyan 
 *        Company:  Atheros Communications, Inc.
 *
 * =====================================================================================
 */

#ifndef _SPECTRAL_MSG_PROTO_H_
#define _SPECTRAL_MSG_PROTO_H_

#ifdef WIN32
#pragma pack(push, pktlog_fmt, 1)
#define __ATTRIB_PACK
#else
#ifndef __ATTRIB_PACK
#define __ATTRIB_PACK __attribute__ ((packed))
#endif
#endif

#include "spectral_types.h"

typedef enum {
    SAMP_FAST_REQUEST=1,
    SAMP_RESPONSE=2,
    SAMP_CLASSIFY_REQUEST=3,
    SAMP_REQUEST,
    LAST_SAMP_TAG_TYPE,

	//add more commands
	START_SCAN = 8,
	START_SCAN_RSP = 9,
	STOP_SCAN = 10,
	STOP_SCAN_RSP = 11,
	ERROR_RSP = 12,
	START_FULL_SCAN = 13,
	GET_CURRENT_CHANNEL = 14,
	GET_CURRENT_CHANNEL_RSP = 15
}eSAMP_TAG;

typedef enum {
    BW_20=0,
    BW_40
} eBW;

typedef enum {
    INTERF_NONE=0,
    INTERF_MW,
    INTERF_BT,
    INTERF_DECT,
    INTERF_TONE,
    INTERF_OTHER,
} eINTERF_TYPE;

struct TLV{
    u_int8_t tag;
    u_int16_t len;
    u_int8_t value[1];
}__ATTRIB_PACK;

struct FREQ_BW_REQ{
    u_int16_t freq;
    u_int8_t bw;
}__ATTRIB_PACK;

struct DATA_REQ_VAL{
    u_int16_t count;
    struct FREQ_BW_REQ data[1];
} __ATTRIB_PACK;

struct FREQ_PWR_RSP{
    u_int8_t pwr;
} __ATTRIB_PACK;

struct SAMPLE_RSP {
    u_int16_t freq;
    u_int16_t sample_count;
    struct FREQ_PWR_RSP samples[1];
} __ATTRIB_PACK;

struct INTERF_RSP{
    u_int8_t interf_type;
    u_int16_t interf_min_freq;
    u_int16_t interf_max_freq;
} __ATTRIB_PACK;

#define MAX_INTERF 10
struct INTERF_SRC_RSP{
    u_int16_t count;
    struct INTERF_RSP interf[MAX_INTERF];
} __ATTRIB_PACK;

#ifdef __ATTRIB_PACK
#undef __ATTRIB_PACK
#endif

#endif
