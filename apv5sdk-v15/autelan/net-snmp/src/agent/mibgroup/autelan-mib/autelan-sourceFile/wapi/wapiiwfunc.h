/**********************************************************************************
* Copyright (c) 2008-2011  Beijing Autelan Technology Co. Ltd.
* All rights reserved.
*
* filename: wapiiwfunc.h
* description:  implementation for the header file of wapiiwfunc.c.
* 
*
* 
************************************************************************************/

#ifndef _WAPIAPFUNC_H
#define _WAPIAPFUNC_H

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
#include <netdb.h>		
#include <net/ethernet.h>
#include <sys/time.h>
#include <unistd.h>
#include <net/if_arp.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/if_ether.h>
#include "wireless.h"
//#include "ieee80211_ioctl.h"   hyh test

typedef struct iw_statistics	iwstats;
typedef struct iw_range			iwrange;
typedef struct iw_param			iwparam;
typedef struct iw_freq			iwfreq;
typedef struct iw_quality		iwqual;
typedef struct iw_priv_args		iwprivargs;
typedef struct sockaddr			sockaddr;

#define P80211_IOCTL_PRIV  				0x8BF7
#define DEVICE_NAME 					"wlan0"
#define DEV_BR							"br0"	
#define P80211_PACKET_AP_HUNTRER 		(unsigned short)0xF601
#define IW_MAX_AP_WAPI					(unsigned short)32
#define MAX_SSID_LEN					32		
#define ELEMENT_MAX_LEN 				32

#define P80211_IOCTL_WDS_ADD			0x8BEA		
#define P80211_IOCTL_WDS_DEL            0x8BEC
#define P80211_IOCTL_WDS_GET			0x8BFD
#define IOCTL_GET_NAME_BY_MAC		 	0x8BFB
#define P80211_PACKET_B2410_SEC         (unsigned short)0xF501


#define TIMER_INTERVAL  				(unsigned long)400000

enum {
	PRISM2_PARAM_PTYPE = 1,
	PRISM2_PARAM_TXRATECTRL = 2,
	PRISM2_PARAM_BEACON_INT = 3,
	PRISM2_PARAM_PSEUDO_IBSS = 4,
	PRISM2_PARAM_ALC = 5,
	PRISM2_PARAM_TXPOWER = 6,
	PRISM2_PARAM_DUMP = 7,
	PRISM2_PARAM_OTHER_AP_POLICY = 8,
	PRISM2_PARAM_AP_MAX_INACTIVITY = 9,
	PRISM2_PARAM_AP_BRIDGE_PACKETS = 10,
	PRISM2_PARAM_DTIM_PERIOD = 11,
	PRISM2_PARAM_AP_NULLFUNC_ACK = 12,
	PRISM2_PARAM_MAX_WDS = 13,
	PRISM2_PARAM_AP_AUTOM_AP_WDS = 14,
	PRISM2_PARAM_AP_AUTH_ALGS = 15,
	PRISM2_PARAM_MONITOR_ALLOW_FCSERR = 16,
	PRISM2_PARAM_HOST_ENCRYPT = 17,
	PRISM2_PARAM_HOST_DECRYPT = 18,
	PRISM2_PARAM_BUS_MASTER_THRESHOLD_RX = 19,
	PRISM2_PARAM_BUS_MASTER_THRESHOLD_TX = 20,
	PRISM2_PARAM_HOST_ROAMING = 21,
	PRISM2_PARAM_BCRX_STA_KEY = 22,
	PRISM2_PARAM_IEEE_802_1X = 23
};


int iw_get_priv_info(int skfd, char *ifname, iwprivargs *priv);

static int set_private(int skfd, char * args[], int	count, char *ifname, char * buffer,int  *  paramnum);
  
int get_priv_int( int tok, char* ifname );

static inline int
iw_set_ext(int			skfd,		/* Socket to the kernel */
	   char *		ifname,		/* Device name */
	   int			request,	/* WE ID */
	   struct iwreq *	pwrq)		/* Fixed part of the request */
{
  /* Set device name */
  strncpy(pwrq->ifr_name, ifname, IFNAMSIZ);
  /* Do the request */
  return(ioctl(skfd, request, pwrq));
}

static inline int
iw_get_ext(int			skfd,		/* Socket to the kernel */
	   char *		ifname,		/* Device name */
	   int			request,	/* WE ID */
	   struct iwreq *	pwrq)		/* Fixed part of the request */
{
  /* Set device name */
  strncpy(pwrq->ifr_name, ifname, IFNAMSIZ);
  /* Do the request */
  return(ioctl(skfd, request, pwrq));
}


struct ioctl_drv
{
	UINT16  io_packet;
	struct  _iodata
	{
		UINT16 wDataLen;
		char pbData[96];
	}iodata;
}__attribute__ ((packed));

struct print_info_ap
{
	unsigned char ssid[MAX_SSID_LEN + 1];
	unsigned char channel[4];
	unsigned char mac[18];
	unsigned char quality[ELEMENT_MAX_LEN];
	unsigned char signal_level[ELEMENT_MAX_LEN];
	unsigned char noise_level[ELEMENT_MAX_LEN];
};

struct wapi_ap_info
{
	int ap_ssid_len;
	unsigned char ap_ssid[MAX_SSID_LEN + 1]; /* AP's ssid */
	int ap_channel;
} ;


int set_priv_int( int tok, char* ifname, long value );
int get_priv_int( int tok, char* ifname );
int wapiset_essid(u_char *var_val, size_t var_val_len, char *ifname);
int wapiset_bssid(u_char *var_val, size_t var_val_len, char *ifname);
int wapiset_mode(u_char *var_val, size_t var_val_len, char *ifname);
int wapiset_dens(u_char *var_val, size_t var_val_len, char *ifname);
int wapiset_rts(u_char *var_val, size_t var_val_len, char *ifname);
int wapiset_frag(u_char *var_val, size_t var_val_len, char *ifname);
int wapi_get_ap_info(unsigned short ioflag, struct print_info_ap *print_info);
int channel_hande_timer( unsigned long interval, int channel_used);
int set_ap_hanter_begin( );
int set_ap_hanter_end( );
int send_data(char *data_buf, int data_buf_len, const unsigned short packet_type);
int notify_to_APdriver(int ioctl_fd, unsigned short ioflag, void *buf);
void iw_print_stats(struct print_info_ap *print_info, struct iw_quality *qual, 
		iwrange *range, int has_range);
int iw_get_range_info(int	skfd,  char *	ifname,  iwrange *range);
int socket_open_for_ioctl();
char * iw_pr_ether(char *buffer,  unsigned char *	ptr);
char *iw_pr_ap_info(char *buffer,  unsigned char *	ptr);
char *free_mem(char *buffer, int buff_len);

#endif


