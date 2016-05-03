/*
************************************************
* FileName:detector.h
* Athor:GengZongJie
* Date:2013-7-22
************************************************
*/

#ifndef __DETECTOR_H__
#define __DETECTOR_H__
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <linux/if_packet.h>
#include <linux/if_ether.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/time.h>
#include <errno.h>
#include <sys/socket.h>
#include <linux/netlink.h>
#include <asm/types.h>
#include <linux/rtnetlink.h>
#include <unistd.h>
#include <stdarg.h>
#include <string.h>



typedef unsigned char u_int8_t;
typedef unsigned short u_int16_t;
typedef unsigned int u_int32_t;
typedef unsigned long long u_int64_t;

#define TEST_USELESS_CODE 0
#define TEST_USEFUL_CODE 1
#define AP_SCANNER_ACL 1

#define IFNAMESIZE 16
#define MAC_ADDR_LEN 6
#define IP_ADDR_lEN 4
#define MAX_RFID_MAC_FILTER_NUM 5
#define MAX_SCAN_CHAN_NUM 13
#define MAX_LOCATE_STA_NUM 100
#define MAX_MSG_PAYLOAD 1024

#define NETLINK_GENERIC 16
#define NETLINK_DETECTOR_WTPD NETLINK_GENERIC + 4
#define NETLINK_DETECTOR_SCANNER NETLINK_GENERIC + 5
//#define NETLINK_DETECTOR_SCANNER 0
#define DETECTOR_DEBUG_PRINT 1
#define TYPE_LENGTH_LEN (1+1)

enum DetectorLogSeverity {
    DET_LOG_EMERG = 0,
    DET_LOG_ALERT = 1, 
    DET_LOG_CRIT = 2,
    DET_LOG_ERR	= 3,
    DET_LOG_WARNING = 4,
    DET_LOG_NOTICE = 5,
    DET_LOG_INFO = 6,
    DET_LOG_DEBUG = 7
};

#define RFID_FUNC_MASK 0X01
#define WIFIPOS_FUNC_MASK 0x02
#define RRM_FUNC_MASK 0x04

#define IF_NAME_SIZE 64
#define CMD_BUF_SIZE 128
//#define MAX_SERVER_NUM 4


#define MODULE_ID_DETECTOR 0x101 /* to be discussed */
#define MODULE_ID_WTPD     0x102 /* to be discussed */



#define FUNC_SWITCH_ENABLE 1
#define FUNC_SWITCH_DISABLE 0
#define RFID_SCANTYPE_AP 1
#define RFID_SCANTYPE_STA 2
#define RFID_SCANTYPE_TAG 4
#define WIFIPOS_SCANTYPE_ASSOCSAT 1
#define WIFIPOS_SCANTYPE_DISASSOCSTA 2
#define RRM_SCANTYP_AP 1
#define RRM_SCANTYPE_STA 2
#define RESULT_FORWARD_SERVER 1
#define RESULT_FORWARD_AC 2
#define REPORT_MODE_UDP 1
#define REPORT_MODE_TCP 2

//tlv message define
#define TLV_MSG_TYPE_LEN 1
#define TLV_MSG_LENGTH_LEN 1

//atnetlink header message define
#define AT_NETLINK_DOMAIN 0
#define AT_NETLINK_VERSION 0


//attribute netlink header define
enum
{
	ATTR_NLHDR_FUNCTYPE_TYPE,		//0
	ATTR_NLHDR_FUNCSUBTYPE_TYPE,	//1
	ATTR_NLHDR_VERSION_TYPE,		//2 
	ATTR_NLHDR_MSGLENGTH_TYPE,		//3
};

enum
{
	ATTR_NLHDR_FUNCTYPE_CONFIG = 1,
	ATTR_NLHDR_FUNCTYPE_RESULT 		//2
};
#define ATTR_HDR_MSG_LEN 13

//rfid attribute body define
enum
{
 	RFID_NLBD_FUNCSWITCH_TYPE,		//0
 	RFID_NLBD_SCANTYPE_TYPE,		//1
 	RFID_NLBD_RADIOID_TYPE,			//2
 	RFID_NLBD_CHANBITMAP_TYPE,		//3
 	RFID_NLBD_RESFORWARD_TYPE,		//4
 	RFID_NLBD_REPORTMODE_TYPE,		//5
 	RFID_NLBD_SERVERIP_TYPE,		//6
 	RFID_NLBD_SERVERPORT_TYPE,		//7
 	RFID_NLBD_SCANINTERVAL_TYPE,	//8
 	RFID_NLBD_LISTENTIME_TYPE,		//9
	RFID_NLBD_MACFILTSWITCH_TYPE,	//10
	RFID_NLBD_MACFILTNUM_TYPE,		//11
	RFID_NLBD_MACFILTINFO_TYPE,		//12
	RFID_NLBD_REPORTINTERVAL_TYPE	//13
};

//wifipos attribute body define
//config params
enum
{
	WIFIPOS_NLBD_FUNCSWITCH_TYPE,  		//0
	WIFIPOS_NLBD_SCANTYPE_TYPE,			//1
	WIFIPOS_NLBD_SCANINTERVAL_TYPE,		//2
	WIFIPOS_NLBD_LISTENTIME_TYPE,		//3
	WIFIPOS_NLBD_RESFORWARD_TYPE,		//4
	WIFIPOS_NLBD_REPORTMODE_TYPE,		//5
	WIFIPOS_NLBD_SERVERIP_TYPE,			//6
	WIFIPOS_NLBD_SERVERPORT_TYPE,		//7
	WIFIPOS_NLBD_STANUM_TYPE,			//8
	WIFIPOS_NLBD_RADIOID_TYPE,			//9
	WIFIPOS_NLBD_CHANBITMAP_TYPE,		//10
	WIFIPOS_NLBD_REPORTINTERVAL_TYPE,	//11
	WIFIPOS_NLBD_RSSILIMIT_TYPE			//12
};


//scan result
enum
{
	WIFIPOS_RES_NLBD_TOA_TYPE,				//0
	//WIFIPOS_NLBD_LOCATEINTERVAL_TYPE,	//1
	WIFIPOS_RES_NLBD_SRCMAC_TYPE,			//1
	WIFIPOS_RES_NLBD_RSSI_TYPE,				//2
	WIFIPOS_RES_NLBD_STACHAN_TYPE,			//3
	WIFIPOS_RES_NLBD_RADIOID_TYPE			//4
};
#define WIFIPOS_NLBD_STA_INTO_LENGTH 27


//rrm attribute body define
enum
{
	RRM_NLBD_FUNCSWITCH_TYPE,		//0
	RRM_NLBD_SCANTYPE_TYPE,			//1
	RRM_NLBD_RADIOID_TYPE,			//2
	RRM_NLBD_CHANBITMAP,			//3
	RRM_NLBD_RESFORWARD_TYPE,		//4
	RRM_NLBD_REPORTMODE_TYPE,		//5
	RRM_NLBD_SERVERIP_TYPE,			//6
	RRM_NLBD_SERVERPORT_TYPE,		//7
	RRM_NLBD_SCANINTERVAL_TYPE,		//8
	RRM_NLBD_LISTENTIME_TYPE,		//9
	RRM_NLBD_REPORTINTERVAL_TYPE,	//10
	RRM_NLBD_SCANMODE_TYPE			//11
};

//wifipos udp message define
enum
{
	WIFIPOS_UDPMSG_PROTOCOLTYPE_TYPE,	//0
	WIFIPOS_UDPMSG_PROTOCOLVER_TYPE,	//1
	WIFIPOS_UDPMSG_MSGTYPE_TYPE,		//2
	WIFIPOS_UDPMSG_APMAC_TYPE,			//3
	WIFIPOS_UDPMSG_STANUM_TYPE,			//4
	WIFIPOS_UDPMSG_MSGLEN_TYPE			//5
};

enum
{
	WIFIPOS_UDPMSG_STATYPE_DISASSOC,	//0
	WIFIPOS_UDPMSG_STATYPE_ASSOC_SELF,	//1
	WIFIPOS_UDPMSG_STATYPE_ASSOC_OTHER	//2
};

#define WIFIPOS_UDPMSG_HEADER_LEN 24
#define WIFIPOS_UDPMSG_STAINFO_LEN 37

typedef struct 
{
	struct nlmsghdr nlh;
	char buf[MAX_MSG_PAYLOAD];
}detector_nlmsg;

typedef struct 
{
	u_int8_t domain;
	u_int8_t version;
	u_int16_t attr_count;
	u_int32_t reserved;
	u_int32_t original_src_pid;
	u_int32_t original_des_pid;
}atnetlink_header;

typedef struct
{
	u_int16_t length;
	u_int8_t func_type;
	u_int8_t func_subtype;
	u_int8_t version;
	u_int8_t reserved[3];
}attribute_header;


typedef enum
{
	INIT = 0,
	CONFIG = 1,
	RUN = 2,
	IDLE = 3
}DETECTOR_STATE;


/*
common struct
*/

typedef struct
{	
	u_int8_t chan;
	u_int8_t flag;
	u_int16_t reserved;
}scan_locate_chan_info;

typedef struct _scan_locate_chan_node
{
	scan_locate_chan_info node;
	struct _scan_locate_chan_node *next;	
}scan_locate_chan_node;

typedef struct
{
	u_int8_t main_switch;
	u_int8_t parm_update;
	u_int8_t chan_num;
	u_int8_t reserved;	
	u_int16_t scan_period_each_chan;
	u_int16_t listen_time_each_chan;
	scan_locate_chan_node *chan_list;	//channel chain head
}scan_locate_parm_detector_common;

/*
rfid scan struct
*/
typedef struct
{
	u_int8_t mac_len;
	u_int8_t reserved;
	u_int8_t mac[MAC_ADDR_LEN];
}rfid_filter_mac;

typedef struct
{
	u_int8_t server_type;	//ac,server?
	u_int8_t deliver_mode;	//tcp,udp?
	u_int16_t server_port;
	u_int32_t server_ip;
}server_config;

typedef struct
{
	u_int8_t func_switch;
	u_int8_t scan_type;
	u_int16_t report_interval;
	u_int8_t mac_filter_switch;
	server_config server;
	rfid_filter_mac *mac_filter_list_head;	//mac filter chain head
	u_int8_t mac_filter_num;
	//u_int8_t radio_id;
	u_int8_t reserved[3];
}scan_locate_parm_detector_rfid;

/*
wifipos struct
*/
typedef struct
{
	u_int8_t sta_chan;
	u_int8_t mac_len;
	u_int8_t mac[MAC_ADDR_LEN];
}locate_sta_info;


typedef struct
{
	u_int8_t func_switch;
	u_int8_t scan_type;
	u_int16_t report_interval;
	u_int8_t locate_sta_num;
	server_config server;
	locate_sta_info *sta_list_head;	//locate sta chain head
	//u_int8_t radio_id;
	u_int8_t rssilimit;
	u_int8_t reserved[3];
}scan_locate_parm_detector_wifipos;

/*
rrm scan struct
*/
typedef struct
{
	u_int8_t func_switch;
	u_int8_t scan_type;
	u_int16_t report_interval;
	u_int8_t scan_mode;
	server_config server;
	//u_int8_t radio_id;
	//u_int8_t reserved[3];
}scan_locate_parm_detector_rrm;



typedef struct
{
	u_int8_t main_switch;
	u_int8_t parm_update;
	u_int16_t reserved;
	u_int16_t scan_period_each_chan;
	u_int16_t listen_time_each_chan;
	scan_locate_chan_info *channel;
}scan_locate_parm_driver_common;

typedef struct
{
	u_int8_t func_switch;
	u_int8_t scan_type;
	u_int8_t mac_filter_switch;
	u_int8_t mac_filter_num;
	rfid_filter_mac *mac_filter_list_head;	//mac filter chain head
}scan_locate_parm_driver_rfid;

typedef struct 
{
	locate_sta_info *sta_list_head;		//locate sta chain head
	u_int8_t func_switch;
	u_int8_t scan_type;
	u_int8_t locate_sta_num;
	u_int8_t reserved;
}scan_locate_parm_driver_wifipos;

typedef struct
{
	u_int8_t func_switch;
	u_int8_t scan_typed;
	u_int8_t scan_mode;
	u_int8_t reserved;
}scan_locate_parm_driver_rrm;

typedef struct
{
	u_int32_t msg_len;
	u_int8_t *msg;
}assemble_message;


typedef struct
{
	u_int32_t value_tag_status;
	u_int8_t src_mac[MAC_ADDR_LEN];
	u_int16_t value_seq_num;
	u_int8_t rssi;
	u_int8_t value_tag_type;
	u_int16_t reserved;
}scan_locate_res_rfid_node;

typedef struct
{
	struct timeval toa;
	u_int64_t locate_interval;
	u_int8_t src_mac[MAC_ADDR_LEN];
	u_int8_t rssi;
	u_int8_t sta_chan;
	u_int8_t radio_id;
}scan_locate_res_wifipos_node;


/*
* liujf: linux 有通用的链表结构(struct list_head), 就别自己写了
*/

typedef struct 
{
	scan_locate_res_rfid_node node;
	struct scan_locate_res_rfid_chain_node *next;
}scan_locate_res_rfid_chain_node;


typedef struct _scan_locate_res_wifipos_chain_node 
{
	scan_locate_res_wifipos_node node;
	struct _scan_locate_res_wifipos_chain_node *next;
}scan_locate_res_wifipos_chain_node;

int listening_sockets(int nlsock_to_scanner, int nlsock_to_wtpd);
int nlsock_create_to_wtpd(void);
int parse_netlink_msg(u_int8_t *buf);
int parse_ath_config_parm(char *ptr, char subtype, char len);
int parse_server_config_parm(char *ptr, char subtype, char len);
int parse_func_config_parm(u_int8_t *ptr, u_int8_t subtype, u_int16_t len);
int parse_rfid_config_parm(u_int8_t *ptr, u_int16_t len);
int parse_rfid_config_chan_info(char *cur_ptr, u_int8_t common_parm_chan_num, u_int8_t tmp_chan_num);
int parse_rfid_config_mac_info(char *cur_ptr, u_int8_t rfid_mac_filter_num, u_int8_t tmp_mac_filter_num);
int parse_wifipos_config_parm(u_int8_t *ptr, u_int16_t len);
int parse_wifipos_sta_info(char *cur_ptr, char wifipos_locate_sta_num, char tmp_locate_sta_num);
int parse_rrm_config_parm(u_int8_t *ptr, u_int16_t len);
int parse_rrm_config_chan_info(char *cur_ptr, u_int8_t common_parm_chan_num, u_int8_t tmp_chan_num);
int wifipos_udp_sock_create();
int hand_wifipos_report_task(int sockfd);
int nlsock_create_to_scanner(void);
int assemble_config_msg(u_int8_t *bug);
int hand_scan_task(u_int8_t chan_to_scan, u_int16_t listen_time, u_int8_t radio_id);
int take_scan_result(void);
int setup_scan_vap(u_int8_t radio_id);
int set_func_flag_to_scanner(u_int8_t switch_flag, u_int8_t radio_id);
void recv_scan_locate_result(int nlsock_to_scanner);
int parse_scan_locate_result(u_int8_t *ptr, u_int8_t func_subtype, u_int16_t length);
int scan_locate_system_start(u_int8_t radio_id);
void wifipos_result_chain_delete(void);
int rfid_result_chain_delete(void);
int rrm_result_chain_delete(void);
int assemble_msg(assemble_message *msg);
int get_ap_mac_addr(void);
int chain_delete(void);
void printmsg_to_screen(int n ,unsigned char *buffer);
void printmsg_to_logfile(int n ,unsigned char *buffer);
void printmsg(int n ,unsigned char *buffer);
int parse_config_chan_bitmap(char *cur_ptr, char radio_id, char func_type);
char switch_channel_id(char chan_id);
int common_parm_chan_add(u_int8_t tmp_chan, char radio_id, char func_type);
int common_parm_chan_delete(u_int8_t tmp_chan,char radio_id,char func_type);
void parse_wifipos_result(u_int8_t *ptr, u_int16_t length);
void* deal_with_scan_task_11g(void *arg);
void* deal_with_scan_task_11a(void *arg);
void* send_udp_msg_periodic(void *arg);
void scan_locate_system_exit(u_int8_t radio_id);
int scan_vap_destroy(u_int8_t radio_id);
int detector_pthread_create(pthread_t *new_thread, void *thread_func, void *arg); 
int sub_thread_kill(void);
int des_wlan_all(void);

/* liwei_add: log file interface */
void detector_log_init_file(void);
void detector_log_close_file(void);
int check_detector_logfile_size(void);
inline void detector_vlog(const char *format, va_list args);
void detector_debug(int debug_lvl, const char* fmt, ...);


#define bit(x) (0x1ULL<<(x))
#define DETECTOR_FD_SET(fd, fd_set)    {if (fd >= 0) {FD_SET(fd, fd_set);}}

#if 0 /* liwei_del */
#ifdef DETECTOR_DEBUG_PRINT
#define detector_debug(debug_lvl, fmt, args...) if (detector_debug_lvl > debug_lvl) printf(fmt, ##args)	//detector_debug_lvl should be a pro variable.
#else
#define detector_debug(debug_lvl, fmt, args...)
#endif
#endif /* liwei_del */
int write_counter_to_tmpfile(const char* filename, unsigned long counter);
int det_init_det_to_svr_sock(scan_locate_parm_detector_wifipos detector_wifipos_parm);
int det_exec_scan_locate_param(void);

int det_get_param_bakupinfo(void);
int det_set_param_bakupinfo(u_int8_t radio_id);

int det_get_bak_state(void);
int det_set_bak_state(void);

int det_get_bakupinfo(void);
int det_get_bakup_11g(void);
int det_get_bakup_11a(void);
int det_set_bakupinfo(void);
int det_del_bakupinfo(void);
int set_bakinfo_11g(FILE* fp);
int set_bakinfo_11a(FILE* fp);
int detector_system(char *cmdbuf);

#endif


