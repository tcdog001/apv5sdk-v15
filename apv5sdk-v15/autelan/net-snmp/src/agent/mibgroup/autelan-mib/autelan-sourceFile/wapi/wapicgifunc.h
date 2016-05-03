/**********************************************************************************
* Copyright (c) 2008-2011  Beijing Autelan Technology Co. Ltd.
* All rights reserved.
*
* filename: wapicgifunc.h
* description:  implementation for the header file of wapicgifunc.c.
* 
*
* 
************************************************************************************/

#ifndef __WAPICGIFUNC_H
#define __WAPICGIFUNC_H

#define  TABLE_SIZE 2
#define  AS_START_SCR       "/usr/local/boa/htmls/script/as_scr"
#define  AS_PORT   9001
#define  WLAN_IFNAME 			"wlan0"
#define  GEN_AS_PARAM_CONF              "/etc/wapi/config/AS.conf"
#define  AP_BASE_CONF     		"/etc/wapi/config/basic.conf"
#define  AP_BASE_SCR        		"/usr/local/boa/htmls/script/apply_basic_scr"
#define  AP_REBOOT_SCR      		"/usr/local/boa/htmls/script/reboot_scr"
#define  AP_DEFCON_SCR      		"/usr/local/boa/htmls/script/defcon_scr"

#define  AP_SYSLOG_SCR      		"/usr/local/boa/htmls/script/syslog_scr"

#define  WAPI_CONF          		"/etc/wapi/config/wapi.conf"
#define  MAC_CONF           		"/etc/network/mac.conf"
#define  WIRELESS_CONF                  "/etc/wapi/config/wireless.conf"
#define  WIRELESS_SCR                   "/usr/local/boa/htmls/script/wireless_scr"
#define  IPTABLE_FILTER_SCR                   "/usr/local/boa/htmls/script/trapip_filter"
#define  VIRTURE_CONF                   "/etc/wapi/config/virture_server.conf"
#define  PPPOE_CONF                     "/etc/wapi/config/ppp/pppoe.conf"  
#define  PAP_SECRETS                     "/etc/wapi/config/ppp/pap-secrets"
#define  CHAP_SECRETS            	"/etc/wapi/config/ppp/chap-secrets"
#define DHCPD_CONF                      "/etc/wapi/config/udhcpd.conf"
#define APPLY_DHCPD_SCRIPT               "/usr/local/boa/htmls/script/dhcpd_scr"
#define HOST_BLOCK_CONF                  "/etc/wapi/config/firewall.conf" 
#define  BASIC_CONF                     "/etc/wapi/config/basic.conf"
#define PRE_AUTH_CONFIG                  "/etc/wapi/config/pre_auth.conf" 
#define AP_MACFIL_CONF                  "/etc/wapi/config/macfilter.conf"
#define AP_WAPI_FLAG_CONF               "/etc/wapi/config/wapi.conf"
#define AG_SNMP_CONF                    "/etc/snmp/snmp_agent.conf"
#define SYSLOG_CONF_FILE		"/etc/snmp/syslog.conf"
#define NTP_CONF                        "/etc/wapi/config/ntp.conf"
#define WLAN_CONF                       "/usr/sbin/wlan.conf"
/*******************************************************************************/
#define GEN_OP_MODE_CONF	  	"/etc/wapi/op_mode.conf"
#define GEN_ADDR_CONF			"/etc/wapi/addr.conf"
#define GEN_CERT_FLAG_CONF		"/etc/wapi/ap_cert_flag.conf"
#define GEN_AS_ENTRY_CONF		"/etc/wapi/as_entry.conf"

#define AP_MACFIL_CONF     	 	"/etc/wapi/config/macfilter.conf"
#define AP_WAPI_FLAG_CONF		"/etc/wapi/config/wapi.conf"
#define BR_BASE_CONF			"/etc/wapi/B2410/base.conf"
#define BR_WDS_SECURE_CONF              "/etc/wapi/B2410/wds_secure.conf"
#define BR_WDS_CONF		  	"/etc/wapi/B2410/wds.conf"


#define SNMPD_CONF	  		"/etc/snmp/snmpd.conf"
#define SNMP_AGENT_CONF	  		"/etc/snmp/snmp_agent.conf"
#define APPARA_CONF       		"/etc/wapi/snmp/appara.conf"

#define AP_QOS_SCR 			"/usr/local/boa/htmls/A2410/script/qos_scr"
#define AP_QOS_FILTER_SCR		"/usr/local/boa/htmls/A2410/script/qos_filter_scr"
#define AP_WAPI_SCR 			"/usr/local/boa/htmls/script/wapi_scr"
#define AP_AS_SYSTEM 			"/usr/local/boa/htmls/A2410/script/as_scr"
#define AP_EX_PARAM_SCRIPT 		"/usr/local/boa/htmls/A2410/script/ex_param_scr"
#define AP_LED_SCRIPT 			"/usr/local/boa/htmls/A2410/script/led_scr"

#define SEP_EQUAL '='
#define SEP_SPACE ' '
#define SEP_TAB (char)0x9

#define QOS_BEGIN	"[QOS_LIST_BEGIN]"
#define QOS_END		"[QOS_LIST_END]"

#define LENGTH 255
#define KEYS_MAX 255

#define PUBKEY_LEN  48 
#define RECVFROM_LEN 20000
#define RECETIMEOUT  10 
#define DIS_CERT_NUM 64	

typedef unsigned int   UINT;
typedef unsigned char  UCHAR;

typedef unsigned short  WORD;
typedef unsigned long   DWORD;
typedef char BYTE;


#define RECVTIMEOUT		2
#define 	WAI_WEB_PORT	9002
#define 	VERSIONNOW	0x0001	
#define 	AP_RELOAD	0x0212
#define	AP_RELOAD_RESPONSE	0x0213
#define	CHECK_CERT		0x0214
#define	CHECK_CERT_RESPNOSE	0x0215

#define P80211_IF_AUTH_FLAG	0x8BF7
#define PACKET_AUTH_FLAG	(UINT16)(0xF301)
#define PACKET_ENCRYPT_POLICY	(UINT16)(0xF302)

typedef struct prop_data
{
  char *key;
  char *value;
} prop_data;

typedef struct mac_buf
{
  unsigned char macaddr[6];
} mac_buf;

typedef struct mac_ip
{
  char mac[20]; 
  char ip[20];
} mac_ip;
typedef struct mac_ip_name
{
  char mac[20];
  char ip[20];
  char name[32];
} mac_ip_name;
typedef struct _aCert_query  
{
	WORD ver;  
	WORD packe_type;  
	DWORD preserve;  
	WORD query_id;  
	WORD max_num; 
	WORD from; 
	WORD cpreserve; 
	struct Tdata  
	{
		char pbData[32]; 
		int wDataLen;	
	}Data;
}aCert_query;
typedef struct _tCertItem
{
	DWORD sn;
	BYTE  name[32];
	DWORD starTime;
	DWORD endTime;
	BYTE certState;
	BYTE certTtype;
}tCertItem;
typedef struct _aCert_info 
{
	WORD ver; 
	WORD function_id;
	DWORD validTimes;
	DWORD result; 
	DWORD AS_times; 
	char *pData; 
}aCert_info;

typedef struct _aCert 
{
	WORD ver; 
	WORD packe_type; 
	DWORD preserve;  
	char username[32]; 
	DWORD period; 
	DWORD cert_type; 
}aCert;

typedef struct _aCert_act
{
        WORD ver;
        WORD packe_type; 
        DWORD preserve; 
	char * date;		
}aCert_act; 

struct  trap_info_t
{
	char ipaddr[32];
	char community[32];
	int  port; 
};

struct srv_info 
{
	int fd; 
	int port;
	struct sockaddr_in addr; 
};

struct _head_info  
{
	unsigned short ver;
	unsigned short cmd;
	unsigned short reserve;
	unsigned short data_len;
};

struct _packet_reset_srv 
{
	struct _head_info head;	
	unsigned char data[2000];
};

int set_var_string(char* msg, int did, char *wlanname, u_char* value, size_t varlen1, long* varlen2);
int get_var_string(char * msg,int did,char *wlanname,unsigned char * str,size_t * varlen);
int get_var_long(char * msg,int did,char *wlanname,long * value);
int set_var_long(char* msg, int did, char *wlanname, u_char* value);
int save_global_conf(char type, char* filename, char *command, char *key, char *value);
int free_prop (prop_data properties[], int count);
char *get_prop (char *key, char *result, prop_data properties[], int count);
int load_prop (char type, char *file_name, prop_data properties[]);
int get_pr (char type, char *line, prop_data *prop);
char *strip (char *string, char delimiter);
int wapiget_ip(char *filename, char sep, char *key, char *outstr);
int kill_proc (char *proc_name, int signal);
int get_as_info(struct sockaddr_in *as_addr, char *loop, int as_port);
int open_as_socket();
int fd_timed_read(int fd, char *buf, int length,
   		int microseconds);
int pack_Cert_info(char *buf, int buf_len, aCert_info *Cert_info);
int macstr2addr(char *macstr, unsigned char *addr);
int save_cert(char *filename, char *buff, int bufflen);
int save_mac1(char *filename, char *macaddr, char *startkey, char *endkey);
int del_mac1(char *filename, char *macaddr, char *startkey, char *endkey);
int del_macall(char *filename, char *startkey, char *endkey);
int  load_conf_file(char *file_name, char *be_key,char *end_key);
int download_cert(char *filename, char* buff, int *bufflen);

/***************lhh2007**********************************/
int save_certificate(const char *fname, char *fcontent, int flen);
int send_wapi_info(struct srv_info *WAI_srv, struct _packet_reset_srv *packet_reset_srv);
int recv_wapi_info(struct srv_info *WAI_srv, struct _packet_reset_srv *recv_from_srv);

#endif
