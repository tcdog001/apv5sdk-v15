#ifndef __APCTL_H__
#define __APCTL_H__

/* include header files */

#define ECHO_TIME    10
#define ECHO_OVERTIME 4320 //12 hours instead of 5
#define BUFFER_SIZE  65536    
#define DEVINFO_PATH   "/tmp/device_info.xml"

#define IWLIST_SCAN_SHORT_INTERVAL 120
#define IWLIST_SCAN_LONG_INTERVAL 3600
#define GET_AP_SCANNING_INFO_INTERVAL 1800

/* define variables */

/* define structures */
typedef struct PACKET_LIST 
{
	char *pkt;
	int len;
	struct PACKET_LIST *next;
}PKT_LST;
typedef struct MESSAGE
{
	u16 offset;
	u16 len;
	u8  *msg;
}TMSG;
typedef struct MESSAGE_HEADER
{
	u32 type;
	u32 len;
}MSG_HEADER;


typedef struct 
{
	enum
	{
		T_INTEGER,
		T_STRING,
		T_STRING_ARRAY
	} type;
	union
	{
		int int_value;
		char *str_value;
		char **str_array_value;
	} value;
	char *code;
	char *endCode;
	int count;
} AP_CONFIG;

typedef struct
{
	char *addr;	
}T_AC_ADDR;
typedef enum
{
	METHOD_RESERVED   = 0,
	METHOD_STATIC     = 1,
	METHOD_DNS        = 2,
	METHOD_OPTION43   = 3,	
	METHOD_BOARDCAST  = 4,
	METHOD_MULTICAST  = 5
}DISCOVER_AC_METHOD;

typedef enum
{
	CHAR,
	INT,
	SHORT,
	UL,//unsigned long
	STR
}Value_Type;
typedef enum
{
	CONF_APCTL,
	CONF_RADIO,
	CONF_WLAN
}Conf_Type;
typedef struct
{
	int radioID;
	int wlanID;
	char conf_arg[128];
	char conf_value[64];
}ConfArg;
#define CONF_APCTL_PATH 		"/tmp/conf_apctl"
#define CONF_RADIO_PATH 		"/tmp/conf_apctl/radio"

/* declare variables */
extern pthread_mutex_t  wlan_mutex;
extern pthread_mutex_t  sta_mutex;

extern int tmp_log_max;
extern int log_max;
extern int message_report_max;	// Add by dongzw for station association message report log 2013-04-09
extern char ifname[32];
extern char hostip[32];
extern u8   DEV_MAC[MAC_ADDR_LEN];
extern char COMPANY[64];
extern char HARDWARE_VER[32];
extern char SOFTWARE_VER[32];
extern u8   RADIO_COUNT;

extern char ReportEnable;
/* Begin: Add by dongzw for ap/wireless/eth information report time 2012-12-21 */
extern int ap_report_switch;
extern int eth_report_switch;
/* End: Add by dongzw for ap/wireless/eth information report time 2012-12-21 */

/* declare functions */
extern BOOL get_bssid(u8 rid, u8 wid, u8 *bssid);
extern BOOL add_cmd_to_list_tail(u32 msgtype, void *cmd);
extern u32 ap_report_interval;

BOOL save_apctl_level_config(char *conf_arg,  int value_type, void *conf_value, BOOL chkdir);
BOOL save_radio_level_config(int radio_id, char *conf_arg, int value_type, void *conf_value, BOOL chkdir);
BOOL save_wlan_level_config(int radio_id, int wlan_id, char *conf_arg, int value_type, void *conf_value, BOOL chkdir);
BOOL get_wlan_stations(unsigned char radioId, unsigned char wlanId);
void sigstop_handle(int x);
BOOL sta_limit(int radio_id);

#endif
