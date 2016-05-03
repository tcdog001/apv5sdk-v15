#ifndef __IWLIST_SCAN_H
#define __IWLIST_SCAN_H

#ifdef __cplusplus
extern "C" {
#endif 

/* Fixup to be able to include kernel includes in userspace.
 * Basically, kill the sparse annotations... Jean II */
#ifndef __user
#define __user
#endif

#include <sys/types.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <math.h>
#include <errno.h>
#include <fcntl.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>		/* gethostbyname, getnetbyname */
#include <net/ethernet.h>	/* struct ether_addr */
#include <sys/time.h>		/* struct timeval */
#include <unistd.h>

#include <signal.h>
#include <pthread.h>

/* Glibc systems headers are supposedly less problematic than kernel ones */
#include <sys/socket.h>			/* for "struct sockaddr" et al	*/
#include <linux/wireless.h>
#define mode_11a 1 
#define mode_11bg   2 
#define CMD_LEN 256
#define DEV_LEN 64
#define RES_LEN 128
//#define IFNAMSIZ 16
#define SSID_LEN 6
#define SCAN_INTERVAL 2
#define ESSID_LEN 64
#define IES_INFO_LEN 32
#define IEEE80211_ADDR_LEN 6
#if (defined(MODE11N)||defined(AR11N))
//#define IEEE80211_RATE_MAXSIZE 30
#define IEEE80211_RATE_MAXSIZE 36

#else
#define IEEE80211_RATE_MAXSIZE 15 
#endif

#define	IEEE80211_ADDR_LEN	6
#define	IEEE80211_RATE_VAL		0x7f

#define	IEEE80211_CAPINFO_ESS			0x0001
#define	IEEE80211_CAPINFO_IBSS			0x0002
#define	IEEE80211_CAPINFO_CF_POLLABLE		0x0004
#define	IEEE80211_CAPINFO_CF_POLLREQ		0x0008
#define	IEEE80211_CAPINFO_PRIVACY		0x0010
#define	IEEE80211_CAPINFO_SHORT_PREAMBLE	0x0020
#define	IEEE80211_CAPINFO_PBCC			0x0040
#define	IEEE80211_CAPINFO_CHNL_AGILITY		0x0080
/* bits 8-9 are reserved (8 now for specturm management) */
#define IEEE80211_CAPINFO_SPECTRUM_MGMT		0x0100
#define	IEEE80211_CAPINFO_SHORT_SLOTTIME	0x0400
#define	IEEE80211_CAPINFO_RSN			0x0800
/* bit 12 is reserved */
#define	IEEE80211_CAPINFO_DSSSOFDM		0x2000

#define	WPA_OUI			0xf25000
#define	WPA_OUI_TYPE		0x01

#define STAT_NEW 2
#define STAT_OLD 1
#define STAT_EPT 0

#define SIOCDEVPRIVATE	0x89F0	/* to 89FF */
#define	SIOCG80211STATS				(SIOCDEVPRIVATE+2)
#define	IEEE80211_IOCTL_GETKEY		(SIOCDEVPRIVATE+3)
#define	IEEE80211_IOCTL_GETWPAIE		(SIOCDEVPRIVATE+4)
#define	IEEE80211_IOCTL_STA_STATS	(SIOCDEVPRIVATE+5)
#define	IEEE80211_IOCTL_STA_INFO		(SIOCDEVPRIVATE+6)
#define	SIOC80211IFCREATE				(SIOCDEVPRIVATE+7)
#define	SIOC80211IFDESTROY			(SIOCDEVPRIVATE+8)
#define	IEEE80211_IOCTL_SCAN_RESULTS	(SIOCDEVPRIVATE+9)


/*
 * Management information element payloads.
 */
enum {
	IEEE80211_ELEMID_SSID		= 0,
	IEEE80211_ELEMID_RATES		= 1,
	IEEE80211_ELEMID_FHPARMS	= 2,
	IEEE80211_ELEMID_DSPARMS	= 3,
	IEEE80211_ELEMID_CFPARMS	= 4,
	IEEE80211_ELEMID_TIM		= 5,
	IEEE80211_ELEMID_IBSSPARMS	= 6,
	IEEE80211_ELEMID_COUNTRY	= 7,
	IEEE80211_ELEMID_HOPPATRNPARMS  = 8,
	IEEE80211_ELEMID_HOPPATRNTABLE  = 9,
	IEEE80211_ELEMID_REQINFO	= 10,
	IEEE80211_ELEMID_QBSSLOAD       = 11,
	IEEE80211_ELEMID_EDCAPARAM      = 12,
	IEEE80211_ELEMID_TSPEC		= 13,
	IEEE80211_ELEMID_TRAFCLASS      = 14,
	IEEE80211_ELEMID_SCHEDULE       = 15,
	IEEE80211_ELEMID_CHALLENGE	= 16,
	/* 17-31 reserved for challenge text extension */
	IEEE80211_ELEMID_PWRCNSTR	= 32,
	IEEE80211_ELEMID_PWRCAP		= 33,
	IEEE80211_ELEMID_TPCREQ		= 34,
	IEEE80211_ELEMID_TPCREP		= 35,
	IEEE80211_ELEMID_SUPPCHAN	= 36,
	IEEE80211_ELEMID_CHANSWITCHANN	= 37,
	IEEE80211_ELEMID_MEASREQ	= 38,
	IEEE80211_ELEMID_MEASREP	= 39,
	IEEE80211_ELEMID_QUIET		= 40,
	IEEE80211_ELEMID_IBSSDFS	= 41,
	IEEE80211_ELEMID_ERP		= 42,
	IEEE80211_ELEMID_TSDELAY	= 43,
	IEEE80211_ELEMID_TCLASSPROC     = 44,
	IEEE80211_ELEMID_QOS		= 46,
	IEEE80211_ELEMID_RSN		= 48,
	/* 49 reserved */
	IEEE80211_ELEMID_XRATES		= 50,
	/*ljy--add begin for wapi*/
	IEEE80211_ELEMID_WAPI		= 68,
	/*ljy--add end*/
	/* 128-129 proprietary elements used by Agere chipsets */
	IEEE80211_ELEMID_AGERE1		= 128,
	IEEE80211_ELEMID_AGERE2		= 129,
	IEEE80211_ELEMID_TPC		= 150,
	IEEE80211_ELEMID_CCKM		= 156,
	IEEE80211_ELEMID_VENDOR		= 221,	/* vendor private */
};

#if (defined(MODE11N)||defined(AR11N))
struct ieee80211req_scan_result {
	unsigned short	isr_len;		/* length (mult of 4) */
	unsigned short	isr_freq;		/* MHz */
	unsigned int	isr_flags;		/* channel flags */
	unsigned char	isr_noise;
	unsigned char	isr_rssi;
	unsigned char	isr_intval;		/* beacon interval */
	unsigned short	isr_capinfo;		/* capabilities */
	unsigned char	isr_erp;		/* ERP element */
	unsigned char	isr_bssid[IEEE80211_ADDR_LEN];
	unsigned char	isr_nrates;
    /*<Begin : caizhibang modify for WBS-4 */
	//unsigned char	isr_rates[IEEE80211_RATE_MAXSIZE];
	unsigned short isr_rates[IEEE80211_RATE_MAXSIZE];
    /*End : caizhibang modify for WBS-4> */
	unsigned char	isr_ssid_len;		/* SSID length */
	unsigned short	isr_ie_len;		/* IE length */
	unsigned char	isr_pad[4];
	/* variable length SSID followed by IE data */
};
#else
struct ieee80211req_scan_result {
	unsigned short	isr_len;		/* length (mult of 4) */
	unsigned short	isr_freq;		/* MHz */
	unsigned short	isr_flags;		/* channel flags */
	unsigned char	isr_noise;
	unsigned char	isr_rssi;
	unsigned char	isr_intval;		/* beacon interval */
	unsigned short	isr_capinfo;		/* capabilities */
	unsigned char	isr_erp;		/* ERP element */
	unsigned char	isr_bssid[IEEE80211_ADDR_LEN];
	unsigned char	isr_nrates;
    /*<Begin : caizhibang modify for WBS-4 */
	//unsigned char	isr_rates[IEEE80211_RATE_MAXSIZE];
	unsigned short	isr_rates[IEEE80211_RATE_MAXSIZE];
    /*End : caizhibang modify for WBS-4> */
	unsigned char	isr_ssid_len;		/* SSID length */
	unsigned char	isr_ie_len;		/* IE length */
	unsigned char	isr_pad[5];
	/* variable length SSID followed by IE data */
};
#endif

enum opstate {
	NEW=1,
	DELETE,
	MODIFIED,
	REMAIN,
};
enum interfaceType {
	ATH=0,
	ETH,
	WIFI,
};

typedef struct ST_S_LIST
{
    struct ST_S_LIST *next;
}S_LIST;

typedef struct ST_S_LIST_HEAD
{
    S_LIST head;
    unsigned int node_count;
}S_LIST_HEAD;


typedef struct ST_AP_INFO { 
	unsigned char BSSID[SSID_LEN];	
	unsigned char Channel;
	unsigned char RSSI;
	unsigned char NOISE;
	unsigned char BEACON_INT;
	unsigned char Opstatus;
	unsigned char EssidLen;
	char ESSID[ESSID_LEN];
	unsigned char IeLen;
	char IEs_INFO[IES_INFO_LEN];
    unsigned short Rate;
    unsigned short CapabilityInfo;
	unsigned char radio_id;
}AP_INFO;


typedef struct ST_NODE_INFO
{
    unsigned int node_stat;
    AP_INFO st_ap_info;
    
}NODE_INFO;

typedef struct ST_AP_NODE
{
    S_LIST st_list;
    NODE_INFO st_ap_stat;
}AP_NODE;

typedef struct iwscan_state
{
  /* State */
  int			ap_num;		/* Access Point number 1->N */
  int			val_index;	/* Value in table 0->(N-1) */
} iwscan_state;

#ifdef __cplusplus
}
#endif 

#endif
