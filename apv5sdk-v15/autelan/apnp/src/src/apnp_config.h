#ifndef _APNP_CONFIG_H
#define _APNP_CONFIG_H
#include "apnp_list.h"

typedef struct apnp_ap_s* 	 apnp_ap;
typedef struct apnp_radio_s* apnp_radio;
typedef struct apnp_wlan_s*  apnp_wlan;

char* read_mem_xml(char *udpbuff,int udpsize);


struct apnp_ap_s{
    char*			ip;
    char*			mac;
	char*			model;
	char*			sn;
	char*			name;
	char*			apname;
	char*			soft_version;
	char*			radio_count;
	char*			radioif;
	char*		 	stacount;
	struct list_head head;
};

struct apnp_radio_s{
    char*			id;
    char*			channel;
	char* 			mode;
	char*		 	rate;
	char*			power;
	char*		 	wlancount;
	char*		 	maxassoc;
	char*			wlanif;
	struct list_head radio_list;
	struct list_head wlan_list;
};

struct apnp_wlan_s{
	char*		name;
	char*		essid;
	char*		securitytype;
	char*		securitykey;
	struct list_head wlan_list;
};
#endif


