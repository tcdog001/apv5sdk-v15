/*
 *  Copyright (c) 2010 Atheros Communications Inc.
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
 
#ifndef _DOT11WAPIMIB_H_
#define _DOT11WAPIMIB_H_

#ifndef u32
#define u32 unsigned int
#endif
#ifndef u8
#define u8 unsigned char
#endif

#define REKEY_POLICY_DIABLE 1
#define REKEY_POLICY_TIMEBASED 2
#define REKEY_POLICY_PACKETBASED 3
#define REKEY_POLICY_TIMEPACKETBASED 4

enum{
	WAPIMIB_CONFENTRY_VERSION=1,
	WAPIMIB_CONFENTRY_AUTHCTL=2,
	WAPIMIB_CONFENTRY_PORTCTL=3,
	WAPIMIB_CONFENTRY_OPTIONAL_IMP=4,
	WAPIMIB_CONFENTRY_PREAUTH_IMP=5,
	WAPIMIB_CONFENTRY_ENABLE=6,
	WAPIMIB_CONFENTRY_PREAUTH_ENABLE=7,
	WAPIMIB_CONFENTRY_UKEY_SUPPORT=8,
	WAPIMIB_CONFENTRY_UREKEY_METHOD=9,
	WAPIMIB_CONFENTRY_UREKEY_TIME=10,
	WAPIMIB_CONFENTRY_UREKEY_PACKETS=11,
	WAPIMIB_CONFENTRY_MCAST_CIPHER=12,
	WAPIMIB_CONFENTRY_MREKEY_METHOD=13,
	WAPIMIB_CONFENTRY_MREKEY_TIME=14,
	WAPIMIB_CONFENTRY_MREKEY_PACKETS=15,	
	WAPIMIB_CONFENTRY_MREKEY_STRICT=16,
	WAPIMIB_CONFENTRY_PSK_VALUE=17,
	WAPIMIB_CONFENTRY_PSK_PASSPHRASE=18,
	WAPIMIB_CONFENTRY_CERT_UPDATECNT=19,
	WAPIMIB_CONFENTRY_MCAST_UPDATECNT=20,
	WAPIMIB_CONFENTRY_UCAST_UPDATECNT=21,
	WAPIMIB_CONFENTRY_MCAST_CIPHER_SIZE=22,
	WAPIMIB_CONFENTRY_BK_LIFETIME=23,
	WAPIMIB_CONFENTRY_BK_REAUTH_THRESH=24,
	WAPIMIB_CONFENTRY_SA_TIMEOUT=25,
	WAPIMIB_CONFENTRY_AUTHSUITE_SEL=26,
	WAPIMIB_CONFENTRY_UCIPHER_SEL=27,
	WAPIMIB_CONFENTRY_MCIPHER_SEL=28,
	WAPIMIB_CONFENTRY_BKID_USED=29,
	WAPIMIB_CONFENTRY_AUTHSUITE_REQ=30,
	WAPIMIB_CONFENTRY_UCIPHER_REQ=31,
	WAPIMIB_CONFENTRY_MCIPHER_REQ=32

};

enum{
	WAPIMIB_CONFIG_UCIPHER_INDEX=1,
	WAPIMIB_CONFIG_UCIPHER=2,
	WAPIMIB_CONFIG_UCIPHER_ENABLED=3,
	WAPIMIB_CONFIG_UCIPHER_SIZE=4,
	
};

enum{
	WAPIMIB_CONFIG_AUTHSUITE_INDEX=1,
	WAPIMIB_CONFIG_AUTHSUITE_SUITE=2,
	WAPIMIB_CONFIG_AUTHSUITE_ENABLED=3
};

enum {
	WAPIMIB_STATS_INDEX=1,
	WAPIMIB_STATS_STAADDRESS=2,
	WAPIMIB_STATS_VERSION=3,
	WAPIMIB_STATS_CTLPORT_STATUS=4,
	WAPIMIB_STATS_UNICAST_CIPHER=5,
	WAPIMIB_STATS_WPI_REPLAYCNT=6,
	WAPIMIB_STATS_WPI_DECRYPTERROR=7,
	WAPIMIB_STATS_WPI_MICERROR=8,
	WAPIMIB_STATS_WAI_SIGERROR=9,
	WAPIMIB_STATS_WAI_HMACERROR=10,
	WAPIMIB_STATS_WAI_AUTHFAILURE=11,
	WAPIMIB_STATS_WAI_DISCARDCNT=12,
	WAPIMIB_STATS_WAI_TIMEOUTCNT=13,
	WAPIMIB_STATS_WAI_FORMATERROR=14,
	WAPIMIB_STATS_WAI_CERTFAILURE=15,
	WAPIMIB_STATS_WAI_UNICASTFAILURE=16,
	WAPIMIB_STATS_WAI_MULTICASTFAILURE=17
};

struct Gb15629dot11wapiConfigEntry{
	int ConfigVersion;               //RO
	int ControlledAuthControl;  //RO
	int ControlledPortControl;  //RO
	int OptionalImplemented;   //RO
	int PreauthenticationImplemented; //RO
	int Enabled;                                   //RW
	int PreauthenticationEnabled;        //RW
	u32 ConfigUnicastKeysSupported;  //RO
	int ConfigUnicastRekeyMethod;     //RW disabled(1),timeBased(2),packetBased(3),timepacket-Based(4)
	u32 ConfigUnicastRekeyTime;        //RW
	u32 ConfigUnicastRekeyPackets;    //RW
	u8 ConfigMulticastCipher[4];           //RW
	int ConfigMulticastRekeyMethod;   //RW disabled(1),timeBased(2),packetBased(3),timepacket-Based(4)
	u32 ConfigMulticastRekeyTime;      //RW
	u32 ConfigMulticastRekeyPackets;  //RW
	int ConfigMulticastRekeyStrict;       //RW
	u8 ConfigPSKValue[32];                  //RW
	u8 ConfigPSKPassPhrase[128];         //RW
	u32 ConfigCertificateUpdateCount; //RW
	u32 ConfigMulticastUpdateCount;   //RW
	u32 ConfigUnicastUpdateCount;     //RW
	u32 ConfigMulticastCipherSize;      //RO
	u32 ConfigBKLifetime;                   //RW
	u32 ConfigBKReauthThreshold;     //RW
	u32 ConfigSATimeout;                   //RW
	u8 AuthenticationSuiteSelected[4]; //RO
	u8 UnicastCipherSelected[4];         //RO
	u8 MulticastCipherSelected[4];      //RO
	u8 BKIDUsed[16];                         //RO
	u8 AuthenticationSuiteRequested[4];  //RO
	u8 UnicastCipherRequested[4];    //RO
	u8 MulticastCipherRequested[4];  //RO
};



struct Gb15629dot11wapiConfigUnicastCiphersEntry{
	u32 ConfigUnicastCipherIndex;  //NA
	u8 ConfigUnicastCipher[4];        //RO
	int ConfigUnicastCipherEnabled;  //RW
	u32 ConfigUnicastCipherSize;      //RO
};



struct Gb15629dot11wapiConfigAuthenticationSuitesEntry{
	u32 ConfigAuthenticationSuiteIndex; //NA
	u8 ConfigAuthenticationSuite[4];       //RO
	int ConfigAuthenticationSuiteEnabled;  //RW
};


//should support entry FF-FF-FF-FF-FF-FF
struct Gb15629dot11wapiStatsEntry{
	u32 StatsIndex; 				//NA
	u8 StatsSTAAddress[6];		//RO
	u32 StatsVersion;				//RO
	int StatsControlledPortStatus;   //RO
	u8 StatsSelectedUnicastCipher[4];  //RO
	u32 StatsWPIReplayCounters;    //RO from driver
	u32 StatsWPIDecryptableErrors; //RO from driver
	u32 StatsWPIMICErrors;            //RO frome driver
	u32 StatsWAISignatureErrors;    //RO
	u32 StatsWAIHMACErrors;         //RO
	u32 StatsWAIAuthenticationResultFailures;  //RO
	u32 StatsWAIDiscardCounters;     //RO
	u32 StatsWAITimeoutCounters;    //RO
	u32 StatsWAIFormatErrors;
	u32 StatsWAICertificateHandshakeFailures;  //RO
	u32 StatsWAIUnicastHandshakeFailures;       //RO
	u32 StatsWAIMulticastHandshakeFailures;      //RO
};

#define WAPI_MIB_STATS_INC( counter) \
{	\
	if(pap) \
		pap->mib_stats.Stats##counter ++;  \
	if(sta_info) \
		sta_info->mib_stats.Stats##counter ++; \
}

#define WAPI_MIB_STATS_SET( entry, val)   \
	sta_info->mib_stats.Stats##entry = val;

#define WAPI_MIB_CONFIG_SET( entry, val)   \
	pap->mib_config.Config##entry = val;

#define 	WAPI_MIB_CONFIG_SET_FROM_STR(entry, str)	\
{                                   \
	int value = atoi(str); \
	if (value)                  \
		WAPI_MIB_CONFIG_SET(entry,value); \
}

#define WAPI_MIB_CMD_GET 1
#define WAPI_MIB_CMD_REPLY      2   

#define WAPI_MIB_RESULT_OK 0
#define WAPI_MIB_RESULT_FAIL 1

#define WAPI_MIB_ENTRY_CONFIG  1
#define WAPI_MIB_ENTRY_UCIPHER 2
#define WAPI_MIB_ENTRY_ASUITE   3
#define WAPI_MIB_ENTRY_STATS     4
/*sta num is not real mib, just for convience */
#define WAPI_MIB_ENTRY_STANUM  5

struct wapimib_msg_hdr{
	unsigned char type;//request or reply
	unsigned char mibEntry;
	unsigned short subEntry;
	int ifindex;
	short  result;
	unsigned short  datalen;//data length
	char data[0];
};


void wapimib_config_init(struct Gb15629dot11wapiConfigEntry* entry);
void wapimib_ucipher_init(struct Gb15629dot11wapiConfigUnicastCiphersEntry* entry);
void wapimib_asuites_init(struct Gb15629dot11wapiConfigAuthenticationSuitesEntry* entry);
void wapimib_stats_init(struct Gb15629dot11wapiStatsEntry* entry);

void wapimib_trap_auth_invalid_ASUE_cert();
void wapimib_trap_keyneg_invalid_challenge();
void wapimib_trap_keyneg_invalid_MIC();
void wapimib_trap_keyneg_invalid_WIE();

#endif

