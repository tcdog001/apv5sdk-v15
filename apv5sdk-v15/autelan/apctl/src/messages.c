#include "global.h"

int Check_Interface_State(char *ifname);
int macAddrCmp (unsigned char* addr1, unsigned char* addr2);


#define UPGRADE_TIMEOUT_TIME              300
#if 0
	#define HOSTAPD_PROFILE_PATH            "../.."
#else
#define HOSTAPD_PROFILE_PATH            "/jffs/hostapd"
#endif

int heartbeat_timer;
int echotime;
u32           ntpserverip;
u16           ntpinterval;
pthread_t     tid_upgrade;
u32   time_upgrade;

WLAN_INFO *wlan_list;
STA_INFO *sta_list;
RADIUS_CONF *radius_list;
INTERVAL_COUNT_MONITOR_INFO *interval_count_monitor_info;

#ifdef AR11N
u32 wireless_if_updown_count[RADIO_NUM] = {1};
#else
u32 wireless_if_updown_count[RADIO_NUM] = {1,1};
#endif

extern char Sta_ReportEnable;
extern char Wlan_ReportEnable;

extern unsigned long AP_START_TIME;
extern unsigned long AP_CURRENT_TIME;

u32 flash_size = 16*1024;
pthread_t thread_sta_tid;
int CheckNetLinkThreadRun = FALSE;
int IntervalCountMonitorOnce = FALSE;

NEIGHBOR_AP_INFO *neighbor_apinfo_list = NULL;

#ifdef AR11N
radioInfoValues gRadioInfoValue[2]={{0,0,0,DISABLED,20,5,TRUE,21,2346,2346,0,0,100,1,1,TRUE,UP,0,0,1, 0xFFFFFF,0x00FF,1,0,0,0,0},
									{1,0,0,DISABLED,20,5,TRUE,10,2346,2346,0,0,100,1,1,TRUE,UP,0,0,1, 0xFFFFFF,0x00FF,1,0,0,0,0}};
#else
radioInfoValues gRadioInfoValue[2]={{0,0,0,DISABLED,20,5,TRUE,21,2346,2346,0,0,100,1,1,TRUE,UP,0,0,1, 0xFFFFFF,0xFFFF,2,0,0,0,0},
									{1,0,0,DISABLED,20,5,TRUE,10,2346,2346,0,0,100,1,1,TRUE,UP,0,0,1, 0xFFFFFF,0xFFFF,2,0,0,0,0}};
#endif

macAddrFiltration *StaBlackList;


RADIO_INFO  *hri, *tri;
RATE_SET_T rs;
BOOL local_tunnel;
int overtime_count;

unsigned char gFloodingDetectEnable = 0;
unsigned char gSpoofingDetectEnable = 0;
unsigned char gWeakIVDetectEnable = 0;


pthread_mutex_t neighbor_apinfo_list_mutex;
pthread_mutex_t siocg_80211_stats;
pthread_mutex_t path_procnet_dev;

extern BOOL set_tunnel_for_all_wlan(BOOL state);
/* ------------------------------------------------------------------------------------------------------------------ */
/* about protocol */
/* store values */
/* stores 8 bits in the message, increments the current offset in bytes */
void protocol_store8(TMSG *msgPtr, u8 val) 
{
	COPY_MEMORY(&((msgPtr->msg)[(msgPtr->offset)]), &(val), 1);
	(msgPtr->offset) += 1;
}

/* stores 16 bits in the message, increments the current offset in bytes */
void protocol_store16(TMSG *msgPtr, u16 val) 
{
	val = htons(val);
	COPY_MEMORY(&((msgPtr->msg)[(msgPtr->offset)]), &(val), 2);
	(msgPtr->offset) += 2;
}

/* stores 32 bits in the message, increments the current offset in bytes */
void protocol_store32(TMSG *msgPtr, u32 val) 
{
	val = htonl(val);
	COPY_MEMORY(&((msgPtr->msg)[(msgPtr->offset)]), &(val), 4);
	(msgPtr->offset) += 4;
}
void protocol_store64(TMSG *msgPtr, u64 val) 
{
	u8 tmp;
	int i = 0;
	for(i=7;i>=0;i--)
	{
		tmp = (val>>(8*i))&0xff;
		COPY_MEMORY(&((msgPtr->msg)[(msgPtr->offset)]), &(tmp), 1);
		(msgPtr->offset) += 1;
	}
}

/* stores a string in the message, increments the current offset in bytes. Doesn't store
 * the '\0' final character.
 */
void protocol_store_str(TMSG *msgPtr, char *str) 
{
	int len = strlen(str);
	COPY_MEMORY(&((msgPtr->msg)[(msgPtr->offset)]), str, len);
	(msgPtr->offset) += len;
}

/* stores another message in the message, increments the current offset in bytes. */
void protocol_store_message(TMSG *msgPtr, TMSG *msgToStorePtr) 
{
	COPY_MEMORY(&((msgPtr->msg)[(msgPtr->offset)]), msgToStorePtr->msg, msgToStorePtr->offset);
	(msgPtr->offset) += msgToStorePtr->offset;
}
/* stores len bytes in the message, increments the current offset in bytes. */
void protocol_store_rawbytes(TMSG *msgPtr, char *bytes, int len) 
{
	COPY_MEMORY(&((msgPtr->msg)[(msgPtr->offset)]), bytes, len);
	(msgPtr->offset) += len;
}
/* get values */
/* retrieves 8 bits from the message, increments the current offset in bytes. */
u8 protocol_retrieve8(TMSG *msgPtr)
{
	u8 val;
	
	COPY_MEMORY(&val, &((msgPtr->msg)[(msgPtr->offset)]), 1);
	(msgPtr->offset) += 1;
	
	return val;
}

/* retrieves 16 bits from the message, increments the current offset in bytes. */
u16 protocol_retrieve16(TMSG *msgPtr) 
{
	unsigned short val;
	
	COPY_MEMORY(&val, &((msgPtr->msg)[(msgPtr->offset)]), 2);
	(msgPtr->offset) += 2;
	
	return ntohs(val);
}
u32 protocol_retrieve32(TMSG *msgPtr) 
{
	u32 val = 0;
	
	COPY_MEMORY(&val, &((msgPtr->msg)[(msgPtr->offset)]), 4);
	(msgPtr->offset) += 4;
	
	return ntohl(val);
}
/* retrieves a string (not null-terminated) from the message, increments the current offset in bytes.
 * Adds the '\0' char at the end of the string which is returned
 */
char *protocol_retrieve_str(TMSG *msgPtr, int len) 
{
	char *str = NULL;
	
	CREATE_OBJECT_SIZE_ERR(str, (len+1), return NULL;);
	
	COPY_MEMORY(str, &((msgPtr->msg)[(msgPtr->offset)]), len);
	str[len] = '\0';
	(msgPtr->offset) += len;
	
	return str;
}

/* retrieves len bytes from the message, increments the current offset in bytes. */
char *protocol_retrieve_raw_bytes(TMSG *msgPtr, int len) 
{
	char *bytes = NULL;
	
	CREATE_OBJECT_SIZE_ERR(bytes, len, return NULL;);
	COPY_MEMORY(bytes, &((msgPtr->msg)[(msgPtr->offset)]), len);
	(msgPtr->offset) += len;
	
	return bytes;
}
char *protocol_retrieve_hex_to_str(TMSG *msgPtr, int len)
{
	char *bytes = NULL;
	char str[8] = {0};
	int i = 0;
	CREATE_OBJECT_SIZE_ERR(bytes, 2*len+1, return NULL;);
	for(i = 0; i < len; i++)
	{
		sprintf(str, "%02X", (msgPtr->msg)[(msgPtr->offset)]);
		strncat(bytes, str, 2);
		msgPtr->offset += 1;
	}
	return bytes;
}
/* ------------------------------------------------------------------------------------------------------------------ */
/* Assemble a Message Element creating the appropriate header and storing the message. */
BOOL assemble_msg_elem(TMSG *msgPtr, u32 type) 
{
	TMSG completeMsg;
	
	if(msgPtr == NULL) 
		return RAISE_ERROR(ERROR_WRONG_ARG, NULL);
	
	CREATE_PROTOCOL_MESSAGE(completeMsg, 8+(msgPtr->offset), return RAISE_ERROR(ERROR_OUT_OF_MEMORY, NULL););

	/* store header */
	protocol_store32(&completeMsg, type);
	protocol_store32(&completeMsg, msgPtr->offset); /* size of the body */
	
	/* store body */
	protocol_store_message(&completeMsg, msgPtr);

	FREE_PROTOCOL_MESSAGE(*msgPtr);

	msgPtr->msg = completeMsg.msg;
	msgPtr->offset = completeMsg.offset;

	return TRUE;
}
/* ------------------------------------------------------------------------------------------------------------------ */
/* Assemble a request Message. */

BOOL assemble_sta_association_request(void *arg, TMSG *msg)
{
	STA_INFO *sta = (STA_INFO *)arg;
	if(sta == NULL)
		RAISE_SYS_ERR(ERROR_WRONG_ARG);
	sta->ssid_length = strlen((char *)sta->SSID);
	CREATE_PROTOCOL_MESSAGE(*msg, 18+sta->ssid_length+5*3, RAISE_SYS_ERR(ERROR_NEED_RESOURCE););
	protocol_store_rawbytes(msg, (char *)sta->MAC, MAC_ADDR_LEN);
	protocol_store_rawbytes(msg, (char *)sta->BSSID, MAC_ADDR_LEN);
	protocol_store8(msg, sta->encryption_type);
	protocol_store32(msg, sta->time_stamp);
	protocol_store8(msg, sta->ssid_length);
	protocol_store_rawbytes(msg, (char *)sta->SSID, sta->ssid_length);
	protocol_store16(msg, 2);
	protocol_store16(msg, 1);
	protocol_store8(msg, sta->value);
	protocol_store16(msg, 3);
	protocol_store16(msg, 1);
	protocol_store8(msg, sta->wid);
	protocol_store16(msg, 6);
	protocol_store16(msg, 1);
	protocol_store8(msg, sta->rid);
	if(assemble_msg_elem(msg, MSG_ELEMENT_STA_ASSOCIATION_REQUEST_TYPE) == FALSE)
	{
		return RAISE_ERROR(ERROR_NEED_RESOURCE, "assemble sta association request failed!");
	}
	return TRUE;
}

BOOL assemble_report_ap_ability_request(void *arg, TMSG *msg)
{
	int len = 0;
	AP_ABILITY_INFO *ability = (AP_ABILITY_INFO *)arg;
	if(ability == NULL)
		RAISE_SYS_ERR(ERROR_WRONG_ARG);
	len = 4+4+5+(4+strlen(ability->CPU_INFO))+(4+strlen(ability->MEM_TYPE))+8*5
		+(4+strlen(ability->dev_describe))+(4+strlen(ability->software_name));
	CREATE_PROTOCOL_MESSAGE(*msg, len, RAISE_SYS_ERR(ERROR_NEED_RESOURCE););
	u16	ver = 0;
	u8	seq = 0;
	protocol_store16(msg, ver);
	protocol_store8(msg, seq);
	protocol_store8(msg, 0);

	protocol_store16(msg, 20);	//type
	protocol_store16(msg, 0);	//length
	//protocol_store_str(msg, ability->net_code);

	protocol_store16(msg, 21);	//type
	protocol_store16(msg, 1);	//length
	protocol_store8(msg, ability->eth_num);

	protocol_store16(msg, 22);	//type
	protocol_store16(msg, strlen(ability->CPU_INFO));	//length
	protocol_store_str(msg, ability->CPU_INFO);
	//(msg->offset) += 64 - strlen(ability->CPU_INFO);

	protocol_store16(msg, 23);	//type
	protocol_store16(msg, strlen(ability->MEM_TYPE));	//length
	protocol_store_str(msg, ability->MEM_TYPE);
	//(msg->offset) += 64 - strlen(ability->MEM_TYPE);

	protocol_store16(msg, 24);	//type
	protocol_store16(msg, 4);	//length
	protocol_store32(msg, ability->mem_size);

	protocol_store16(msg, 25);	//type
	protocol_store16(msg, 4);	//length
	protocol_store32(msg, ability->flash_size);

	protocol_store16(msg, 26);	//type
	protocol_store16(msg, 4);	//length
	protocol_store32(msg, ability->IPv4);

	protocol_store16(msg, 27);	//type
	protocol_store16(msg, 4);	//length
	protocol_store32(msg, ability->Mask);
	
	protocol_store16(msg, 28);	//type
	protocol_store16(msg, 4);	//length
	protocol_store32(msg, ability->GateWay);

	protocol_store16(msg, 29);	//type
	protocol_store16(msg, strlen(ability->dev_describe));	//length
	protocol_store_str(msg, ability->dev_describe);

	protocol_store16(msg, 30);	//type
	protocol_store16(msg, strlen(ability->software_name));	//length
	protocol_store_str(msg, ability->software_name);

	if(debugf){
		printf("\n%s:\n",__func__);
		printf("eth_num:%d\t",ability->eth_num);
		printf("CPU_INFO:%s\t",ability->CPU_INFO);
		printf("MEM_TYPE:%s\t",ability->MEM_TYPE);
		printf("mem_size:%d\t",ability->mem_size);
		printf("flash_size:%d\n",ability->flash_size);
		char ip_str[24]={0};
		ip_int2str(ability->IPv4, (unsigned char *)ip_str);
		printf("IPv4:%s\t",ip_str);
		ip_int2str(ability->Mask, (unsigned char *)ip_str);
		printf("Mask:%s\t",ip_str);
		ip_int2str(ability->GateWay, (unsigned char *)ip_str);
		printf("GateWay:%s\t\t",ip_str);
		printf("dev_describe:%s\t",ability->dev_describe);
		printf("software_name:%s\n\n",ability->software_name);
	}
	if(assemble_msg_elem(msg, MSG_ELEMENT_REPORT_AP_STATE_REQUEST_TYPE) == FALSE)
	{
		return RAISE_ERROR(ERROR_NEED_RESOURCE, "assemble report ap ability request failed!");
	}
	return TRUE;
}

BOOL assemble_report_ap_state_count_request(void *arg, TMSG *msg)
{
	int len = 0;
	AP_STATE_COUNT_INFO *state_count = (AP_STATE_COUNT_INFO *)arg;
	if(state_count == NULL)
		RAISE_SYS_ERR(ERROR_WRONG_ARG);
	len = 4+5+8*21;
	if(state_count->same_frequency_disturb_alarm_threshold > ALARM_THRESHOLD)
		len += 8;
	if(state_count->adjacent_frequency_disturb_alarm_threshold > ALARM_THRESHOLD)
		len += 8;

	CREATE_PROTOCOL_MESSAGE(*msg, len, RAISE_SYS_ERR(ERROR_NEED_RESOURCE););
	u16	ver = 0;
	u8	seq = 0;
	protocol_store16(msg, ver);
	protocol_store8(msg, seq);
	protocol_store8(msg, 0);

	protocol_store16(msg, 50);	//type
	protocol_store16(msg, 4);	//length
	protocol_store32(msg, state_count->CPU_use_rate);

	protocol_store16(msg, 51);	//type
	protocol_store16(msg, 4);	//length
	protocol_store32(msg, state_count->MEM_use_rate);

	protocol_store16(msg, 52);	//type
	protocol_store16(msg, 4);	//length
	protocol_store32(msg, state_count->AP_flow);

	protocol_store16(msg, 53);	//type
	protocol_store16(msg, 1);	//length
	protocol_store8(msg, state_count->AP_wireless_monitor_work_mode);

	protocol_store16(msg, 54);	//type
	protocol_store16(msg, 4);	//length
	protocol_store32(msg, state_count->BSSID_Count);

	protocol_store16(msg, 55);	//type
	protocol_store16(msg, 4);	//length
	protocol_store32(msg, state_count->terminal_disturb_alarm_threshold);

	if(state_count->same_frequency_disturb_alarm_threshold > ALARM_THRESHOLD)
	{
		protocol_store16(msg, 56);	//type
		protocol_store16(msg, 4);	//length
		protocol_store32(msg, state_count->same_frequency_disturb_alarm_threshold);
	}

	if(state_count->adjacent_frequency_disturb_alarm_threshold > ALARM_THRESHOLD)
	{
		protocol_store16(msg, 57);	//type
		protocol_store16(msg, 4);	//length
		protocol_store32(msg, state_count->adjacent_frequency_disturb_alarm_threshold);
	}

	protocol_store16(msg, 58);	//type
	protocol_store16(msg, 4);	//length
	protocol_store32(msg, state_count->CPU_use_rate_alarm_threshold);

	protocol_store16(msg, 59);	//type
	protocol_store16(msg, 4);	//length
	protocol_store32(msg, state_count->MEM_use_rate_alarm_threshold);

	protocol_store16(msg, 60);	//type
	protocol_store16(msg, 4);	//length
	protocol_store32(msg, state_count->flash_last_space);

	protocol_store16(msg, 100);	//type
	protocol_store16(msg, 4);	//length
	protocol_store32(msg, state_count->CPU_use_rate_average);

	protocol_store16(msg, 101);	//type
	protocol_store16(msg, 4);	//length
	protocol_store32(msg, state_count->MEM_use_rate_average);

	protocol_store16(msg, 105);	//type
	protocol_store16(msg, 4);	//length
	protocol_store32(msg, state_count->connect_count);

	protocol_store16(msg, 106);	//type
	protocol_store16(msg, 4);	//length
	protocol_store32(msg, state_count->connect_failed_count);

	protocol_store16(msg, 107);	//type
	protocol_store16(msg, 4);	//length
	protocol_store32(msg, state_count->connect_retry_count);

	protocol_store16(msg, 108);	//type
	protocol_store16(msg, 4);	//length
	protocol_store32(msg, state_count->connect_undistinguish_retry_count);

	protocol_store16(msg, 109);	//type
	protocol_store16(msg, 4);	//length
	protocol_store32(msg, state_count->disconnect_unnormal_count);

	protocol_store16(msg, 110);	//type
	protocol_store16(msg, 4);	//length
	protocol_store32(msg, state_count->connect_poor_resource_failed_count);

	protocol_store16(msg, 111);	//type
	protocol_store16(msg, 4);	//length
	protocol_store32(msg, state_count->connect_no_support_rate_failed_count);

	protocol_store16(msg, 112);	//type
	protocol_store16(msg, 4);	//length
	protocol_store32(msg, state_count->connect_low_rssi_failed_count);

	protocol_store16(msg, 113);	//type
	protocol_store16(msg, 4);	//length
	protocol_store32(msg, state_count->CPU_use_rate_max);

	protocol_store16(msg, 114);	//type
	protocol_store16(msg, 4);	//length
	protocol_store32(msg, state_count->MEM_use_rate_max);

	protocol_store16(msg, 115);	//type
	protocol_store16(msg, 4);	//length
	protocol_store32(msg, state_count->run_time);

	if(debugf){
		printf("\n%s:\n",__func__);
		printf("CPU_use_rate:%d\t",state_count->CPU_use_rate);
		printf("MEM_use_rate:%d\t\t",state_count->MEM_use_rate);
		printf("AP_flow:%d\t",state_count->AP_flow);
		printf("BSSID_Count:%d\t",state_count->BSSID_Count);
		printf("AP_wireless_monitor_work_mode:%d\n",state_count->AP_wireless_monitor_work_mode);
		printf("terminal_disturb_alarm_threshold:%d\t",state_count->terminal_disturb_alarm_threshold);
		printf("CPU_use_rate_alarm_threshold:%d\t\t",state_count->CPU_use_rate_alarm_threshold);
		printf("MEM_use_rate_alarm_threshold:%d\n",state_count->MEM_use_rate_alarm_threshold);
		printf("flash_last_space:%d\t",state_count->flash_last_space);
		printf("CPU_use_rate_average:%d\t",state_count->CPU_use_rate_average);
		printf("MEM_use_rate_average:%d\t",state_count->MEM_use_rate_average);
		printf("connect_undistinguish_retry_count:%d\n",state_count->connect_undistinguish_retry_count);
		printf("connect_count:%d\t",state_count->connect_count);
		printf("connect_failed_count:%d\t",state_count->connect_failed_count);
		printf("connect_retry_count:%d\t",state_count->connect_retry_count);
		printf("disconnect_unnormal_count:%d\n",state_count->disconnect_unnormal_count);
		printf("connect_poor_resource_failed_count:%d\t",state_count->connect_poor_resource_failed_count);
		printf("connect_no_support_rate_failed_count:%d\t",state_count->connect_no_support_rate_failed_count);
		printf("connect_low_rssi_failed_count:%d\n",state_count->connect_low_rssi_failed_count);
		printf("CPU_use_rate_max:%d\t",state_count->CPU_use_rate_max);
		printf("MEM_use_rate_max:%d\t",state_count->MEM_use_rate_max);
		printf("run_time:%d\n",state_count->run_time);
		if(state_count->same_frequency_disturb_alarm_threshold > ALARM_THRESHOLD)
		{
			printf("same_frequency_disturb_alarm_threshold:%d\t",state_count->same_frequency_disturb_alarm_threshold);
		}	
		if(state_count->adjacent_frequency_disturb_alarm_threshold > ALARM_THRESHOLD)
		{
			printf("adjacent_frequency_disturb_alarm_threshold:%d\t",state_count->adjacent_frequency_disturb_alarm_threshold);
		}	
		printf("\n");
	}
	/* Begin: Add by dongzw for station association message report log 2013-04-09 */
	message_report_log("connect_undistinguish_retry_count:%d\n",state_count->connect_undistinguish_retry_count);
	message_report_log("connect_count:%d\n",state_count->connect_count);
	message_report_log("connect_failed_count:%d\n",state_count->connect_failed_count);
	message_report_log("connect_retry_count:%d\n",state_count->connect_retry_count);
	message_report_log("disconnect_unnormal_count:%d\n",state_count->disconnect_unnormal_count);
	message_report_log("connect_poor_resource_failed_count:%d\n",state_count->connect_poor_resource_failed_count);
	message_report_log("connect_no_support_rate_failed_count:%d\n",state_count->connect_no_support_rate_failed_count);
	message_report_log("connect_low_rssi_failed_count:%d\n",state_count->connect_low_rssi_failed_count);
	/* End: Add by dongzw for station association message report log 2013-04-09 */

	if(assemble_msg_elem(msg, MSG_ELEMENT_REPORT_AP_STATE_REQUEST_TYPE) == FALSE)
	{
		return RAISE_ERROR(ERROR_NEED_RESOURCE, "assemble report ap state count request failed!");
	}
	return TRUE;
}

BOOL assemble_report_wireless_ability_request(void *arg, TMSG *msg)
{
	WIRELESS_ABILITY_INFO *radio = (WIRELESS_ABILITY_INFO *)arg;
	WIRELESS_ABILITY_INFO *ptr = NULL;
	int count = 0;
	int size = 0;
	u16	ver = 0;
	u8	seq = 0;
	if(radio == NULL)
		RAISE_SYS_ERR(ERROR_WRONG_ARG);
	for(ptr = radio; ptr!= NULL; ptr = ptr->next)
		count++;
	size = 4+(5+strlen(radio->if_info)+11+2*6+6*9)*count;
	CREATE_PROTOCOL_MESSAGE(*msg, size, RAISE_SYS_ERR(ERROR_NEED_RESOURCE););
	
	protocol_store16(msg, ver);
	protocol_store8(msg, seq);
	protocol_store8(msg, 0);

	for(ptr = radio; ptr!= NULL; ptr = ptr->next)
	{
		protocol_store16(msg, 20);	//type
		protocol_store16(msg, strlen(radio->if_info)+1);	//length
		protocol_store8(msg, ptr->radio_id);
		protocol_store_str(msg, ptr->if_info);

		protocol_store16(msg, 21);	//type
		protocol_store16(msg, 2);	//length
		protocol_store8(msg, ptr->radio_id);
		protocol_store8(msg, ptr->if_type);


		protocol_store16(msg, 22);	//type
		protocol_store16(msg, 5);	//length
		protocol_store8(msg, ptr->radio_id);
		protocol_store32(msg, ptr->if_MTU);

		protocol_store16(msg, 23);	//type
		protocol_store16(msg, 5);	//length
		protocol_store8(msg, ptr->radio_id);
		protocol_store32(msg, ptr->if_bandwidth);

		protocol_store16(msg, 24);	//type
		protocol_store16(msg, 7);	//length
		protocol_store8(msg, ptr->radio_id);
		protocol_store_rawbytes(msg, (char *)ptr->if_mac, MAC_ADDR_LEN);

		protocol_store16(msg, 25);	//type
		protocol_store16(msg, 2);	//length
		protocol_store8(msg, ptr->radio_id);
		protocol_store8(msg, ptr->support_wireless_mode);

		protocol_store16(msg, 26);	//type
		protocol_store16(msg, 5);	//length
		protocol_store8(msg, ptr->radio_id);
		protocol_store32(msg, ptr->max_txpower);

		protocol_store16(msg, 27);	//type
		protocol_store16(msg, 5);	//length
		protocol_store8(msg, ptr->radio_id);
		protocol_store32(msg, ptr->power_attenuation_area);
		
		protocol_store16(msg, 28);	//type
		protocol_store16(msg, 5);	//length
		protocol_store8(msg, ptr->radio_id);
		protocol_store32(msg, ptr->power_adjust_interval);

		protocol_store16(msg, 29);	//type
		protocol_store16(msg, 5);	//length
		protocol_store8(msg, ptr->radio_id);
		protocol_store32(msg, ptr->antenna);

		if(debugf){
			printf("\n%s:\n",__func__);
			printf("radio_id:%d\t",ptr->radio_id);
			printf("if_info:%s\t",ptr->if_info);
			printf("if_type:%d\t",ptr->if_type);
			printf("if_MTU:%d\t",ptr->if_MTU);
			printf("if_bandwidth:%d\n",ptr->if_bandwidth);
			printf("if_mac:"MAC_FMT"\t",MAC2STR(ptr->if_mac));
			printf("max_txpower:%d\t",ptr->max_txpower);
			printf("antenna:%d\t",ptr->antenna);
			printf("support_wireless_mode:%d\t\n",ptr->support_wireless_mode);
			printf("power_attenuation_area:%d\t\t",ptr->power_attenuation_area);
			printf("power_adjust_interval:%d\n\n",ptr->power_adjust_interval);
		}
	}
	if(assemble_msg_elem(msg, MSG_ELEMENT_AP_REPORT_RADIO_INFO_REQUEST_TYPE) == FALSE)
	{
		return RAISE_ERROR(ERROR_NEED_RESOURCE, "assemble report wireless ability request failed!");
	}
	return TRUE;
}

BOOL assemble_report_wireless_state_count_request(void *arg, TMSG *msg)
{
	WIRELESS_STATE_COUNT_INFO *radio = (WIRELESS_STATE_COUNT_INFO *)arg;
	WIRELESS_STATE_COUNT_INFO *ptr = NULL;
	int count = 0;
	int size = 4;
	int i = 0;
	if(radio == NULL)
		RAISE_SYS_ERR(ERROR_WRONG_ARG);
	count = radio->count;
	if(count == 0)
		return TRUE;
	ptr = radio;
	for(i = count; ((ptr != NULL) && (i > 0)); ptr = ptr->next, i--)
	{
		size += 9*42+6*11+13*4;
		if(ptr->current_rate != NULL)
			size += 5+strlen(ptr->current_rate);
		if(ptr->SSID_list_around != NULL)
			size += 5+strlen(ptr->SSID_list_around);
	}
	CREATE_PROTOCOL_MESSAGE(*msg, size, RAISE_SYS_ERR(ERROR_NEED_RESOURCE););
	u16	ver = 0;
	u8	seq = 0;
	
	protocol_store16(msg, ver);
	protocol_store8(msg, seq);
	protocol_store8(msg, 0);
	ptr = radio;
	for(i = count; ((ptr != NULL) && (i > 0)); ptr = ptr->next, i--)
	{	
		protocol_store16(msg, 50);	//type
		protocol_store16(msg, 5);	//length
		protocol_store8(msg, ptr->radio_id);
		protocol_store32(msg, ptr->beacon_frame_interval);

		protocol_store16(msg, 51);	//type
		protocol_store16(msg, 5);	//length
		protocol_store8(msg, ptr->radio_id);
		protocol_store32(msg, ptr->DTIM_interval);
		
		protocol_store16(msg, 52);	//type
		protocol_store16(msg, 5);	//length
		protocol_store8(msg, ptr->radio_id);
		protocol_store32(msg, ptr->RTS_threshold);

		protocol_store16(msg, 53);	//type
		protocol_store16(msg, 5);	//length
		protocol_store8(msg, ptr->radio_id);
		protocol_store32(msg, ptr->frag_threshold);

		protocol_store16(msg, 54);	//type
		protocol_store16(msg, 2);	//length
		protocol_store8(msg, ptr->radio_id);
		protocol_store8(msg, ptr->preamble_length);

		protocol_store16(msg, 55);	//type
		protocol_store16(msg, 5);	//length
		protocol_store8(msg, ptr->radio_id);
		protocol_store32(msg, ptr->current_txpower);

		protocol_store16(msg, 56);	//type
		protocol_store16(msg, 2);	//length
		protocol_store8(msg, ptr->radio_id);
		protocol_store8(msg, ptr->current_channel);

		protocol_store16(msg, 57);	//type
		protocol_store16(msg, 5);	//length
		protocol_store8(msg, ptr->radio_id);
		protocol_store32(msg, ptr->max_sta_num);

		protocol_store16(msg, 58);	//type
		protocol_store16(msg, 2);	//length
		protocol_store8(msg, ptr->radio_id);
		protocol_store8(msg, ptr->current_wireless_mode);

		if(radio->current_rate != NULL)
		{
			protocol_store16(msg, 59);	//type
			protocol_store16(msg, 1+strlen(ptr->current_rate));	//length
			protocol_store8(msg, ptr->radio_id);
			protocol_store_str(msg, ptr->current_rate);
		}

		protocol_store16(msg, 60);	//type
		protocol_store16(msg, 2);	//length
		protocol_store8(msg, ptr->radio_id);
		protocol_store8(msg, ptr->switch_AMPDU);

		protocol_store16(msg, 61);	//type
		protocol_store16(msg, 2);	//length
		protocol_store8(msg, ptr->radio_id);
		protocol_store8(msg, ptr->switch_20M_40M);

		protocol_store16(msg, 62);	//type
		protocol_store16(msg, 2);	//length
		protocol_store8(msg, ptr->radio_id);
		protocol_store8(msg, ptr->switch_shortGI);

		protocol_store16(msg, 63);	//type
		protocol_store16(msg, 2);	//length
		protocol_store8(msg, ptr->radio_id);
		protocol_store8(msg, ptr->switch_11n);

		protocol_store16(msg, 64);	//type
		protocol_store16(msg, 2);	//length
		protocol_store8(msg, ptr->radio_id);
		protocol_store8(msg, ptr->wifi_state);

		protocol_store16(msg, 65);	//type
		protocol_store16(msg, 5);	//length
		protocol_store8(msg, ptr->radio_id);
		protocol_store32(msg, ptr->wifi_state_continuous_time);

		if(radio->SSID_list_around != NULL)
		{
			protocol_store16(msg, 66);	//type
			protocol_store16(msg, 1+strlen(ptr->SSID_list_around)); //length
			protocol_store8(msg, ptr->radio_id);
			protocol_store_str(msg, ptr->SSID_list_around);
		}

		protocol_store16(msg, 67);	//type
		protocol_store16(msg, 5);	//length
		protocol_store8(msg, ptr->radio_id);
		protocol_store32(msg, ptr->auto_power_control_switch);

		protocol_store16(msg, 68);	//type
		protocol_store16(msg, 5);	//length
		protocol_store8(msg, ptr->radio_id);
		protocol_store32(msg, ptr->auto_wireless_channel_switch);

		protocol_store16(msg, 100);	//type
		protocol_store16(msg, 2);	//length
		protocol_store8(msg, ptr->radio_id);
		protocol_store8(msg, ptr->rx_signal_average_strength);

		protocol_store16(msg, 101);	//type
		protocol_store16(msg, 2);	//length
		protocol_store8(msg, ptr->radio_id);
		protocol_store8(msg, ptr->rx_signal_max_strength);

		protocol_store16(msg, 102);	//type
		protocol_store16(msg, 2);	//length
		protocol_store8(msg, ptr->radio_id);
		protocol_store8(msg, ptr->rx_signal_min_strength);

		protocol_store16(msg, 103);	//type
		protocol_store16(msg, 5);	//length
		protocol_store8(msg, ptr->radio_id);
		protocol_store32(msg, ptr->wireless_if_updown_count);

		protocol_store16(msg, 104);	//type
		protocol_store16(msg, 5);	//length
		protocol_store8(msg, ptr->radio_id);
		protocol_store32(msg, ptr->tx_data_package);

		protocol_store16(msg, 105);	//type
		protocol_store16(msg, 5);	//length
		protocol_store8(msg, ptr->radio_id);
		protocol_store32(msg, ptr->rx_data_package);

		protocol_store16(msg, 106);	//type
		protocol_store16(msg, 9);	//length
		protocol_store8(msg, ptr->radio_id);
		protocol_store64(msg, ptr->rx_data_package_bytes);

		protocol_store16(msg, 107);	//type
		protocol_store16(msg, 9);	//length
		protocol_store8(msg, ptr->radio_id);
		protocol_store64(msg, ptr->tx_data_package_bytes);

		protocol_store16(msg, 108);	//type
		protocol_store16(msg, 5);	//length
		protocol_store8(msg, ptr->radio_id);
		protocol_store32(msg, ptr->wireless_channel_down_retry_data_package);

		protocol_store16(msg, 109);	//type
		protocol_store16(msg, 5);	//length
		protocol_store8(msg, ptr->radio_id);
		protocol_store32(msg, ptr->rx_manage_frame);

		protocol_store16(msg, 110);	//type
		protocol_store16(msg, 5);	//length
		protocol_store8(msg, ptr->radio_id);
		protocol_store32(msg, ptr->rx_ctrl_frame);

		protocol_store16(msg, 111);	//type
		protocol_store16(msg, 5);	//length
		protocol_store8(msg, ptr->radio_id);
		protocol_store32(msg, ptr->rx_data_frame);

		protocol_store16(msg, 112);	//type
		protocol_store16(msg, 5);	//length
		protocol_store8(msg, ptr->radio_id);
		protocol_store32(msg, ptr->tx_manage_frame);

		protocol_store16(msg, 113);	//type
		protocol_store16(msg, 5);	//length
		protocol_store8(msg, ptr->radio_id);
		protocol_store32(msg, ptr->tx_ctrl_frame);

		protocol_store16(msg, 114);	//type
		protocol_store16(msg, 5);	//length
		protocol_store8(msg, ptr->radio_id);
		protocol_store32(msg, ptr->tx_data_frame);

		protocol_store16(msg, 115);	//type
		protocol_store16(msg, 5);	//length
		protocol_store8(msg, ptr->radio_id);
		protocol_store32(msg, ptr->wireless_channel_down_retry_data_package);

		protocol_store16(msg, 116);	//type
		protocol_store16(msg, 5);	//length
		protocol_store8(msg, ptr->radio_id);
		protocol_store32(msg, ptr->rx_error_data_package);

		protocol_store16(msg, 117);	//type
		protocol_store16(msg, 5);	//length
		protocol_store8(msg, ptr->radio_id);
		protocol_store32(msg, ptr->rx_error_frame);

		protocol_store16(msg, 118);	//type
		protocol_store16(msg, 5);	//length
		protocol_store8(msg, ptr->radio_id);
		protocol_store32(msg, ptr->rx_fcs_mac_error_frame);

		protocol_store16(msg, 119);	//type
		protocol_store16(msg, 5);	//length
		protocol_store8(msg, ptr->radio_id);
		protocol_store32(msg, ptr->rx_mic_mac_error_frame);

		protocol_store16(msg, 120);	//type
		protocol_store16(msg, 5);	//length
		protocol_store8(msg, ptr->radio_id);
		protocol_store32(msg, ptr->rx_dissect_failed_mac_error_frame);

		/* ...... */

		protocol_store16(msg, 123);	//type
		protocol_store16(msg, 5);	//length
		protocol_store8(msg, ptr->radio_id);
		protocol_store32(msg, ptr->rx_authenticate_frame);

		protocol_store16(msg, 124);	//type
		protocol_store16(msg, 5);	//length
		protocol_store8(msg, ptr->radio_id);
		protocol_store32(msg, ptr->rx_associate_frame);

		protocol_store16(msg, 125);	//type
		protocol_store16(msg, 5);	//length
		protocol_store8(msg, ptr->radio_id);
		protocol_store32(msg, ptr->tx_signaling_packet);
		

		/* ...... */

		protocol_store16(msg, 128);	//type
		protocol_store16(msg, 5);	//length
		protocol_store8(msg, ptr->radio_id);
		protocol_store32(msg, ptr->associate_sta);

		protocol_store16(msg, 129);	//type
		protocol_store16(msg, 5);	//length
		protocol_store8(msg, ptr->radio_id);
		protocol_store32(msg, ptr->tx_package);

		protocol_store16(msg, 130);	//type
		protocol_store16(msg, 5);	//length
		protocol_store8(msg, ptr->radio_id);
		protocol_store32(msg, ptr->rx_package);

		protocol_store16(msg, 131);	//type
		protocol_store16(msg, 9);	//length
		protocol_store8(msg, ptr->radio_id);
		protocol_store64(msg, ptr->tx_package_bytes);

		protocol_store16(msg, 132);	//type
		protocol_store16(msg, 9);	//length
		protocol_store8(msg, ptr->radio_id);
		protocol_store64(msg, ptr->rx_package_bytes);

		protocol_store16(msg, 133);	//type
		protocol_store16(msg, 5);	//length
		protocol_store8(msg, ptr->radio_id);
		protocol_store32(msg, ptr->rx_phy_errors);

		protocol_store16(msg, 134);	//type
		protocol_store16(msg, 5);	//length
		protocol_store8(msg, ptr->radio_id);
		protocol_store32(msg, ptr->rx_unicast);

		protocol_store16(msg, 135);	//type
		protocol_store16(msg, 5);	//length
		protocol_store8(msg, ptr->radio_id);
		protocol_store32(msg, ptr->tx_unicast);

		protocol_store16(msg, 136);	//type
		protocol_store16(msg, 5);	//length
		protocol_store8(msg, ptr->radio_id);
		protocol_store32(msg, ptr->rx_ununicast);

		protocol_store16(msg, 137);	//type
		protocol_store16(msg, 5);	//length
		protocol_store8(msg, ptr->radio_id);
		protocol_store32(msg, ptr->tx_ununicast);

		protocol_store16(msg, 138);	//type
		protocol_store16(msg, 5);	//length
		protocol_store8(msg, ptr->radio_id);
		protocol_store32(msg, ptr->wireless_rx_interval_bytes);

		protocol_store16(msg, 139);	//type
		protocol_store16(msg, 5);	//length
		protocol_store8(msg, ptr->radio_id);
		protocol_store32(msg, ptr->wireless_tx_interval_bytes);

		protocol_store16(msg, 140);	//type
		protocol_store16(msg, 5);	//length
		protocol_store8(msg, ptr->radio_id);
		protocol_store32(msg, ptr->err_frame_ratio);

		/* ...... */

		protocol_store16(msg, 144);	//type
		protocol_store16(msg, 5);	//length
		protocol_store8(msg, ptr->radio_id);
		protocol_store32(msg, ptr->rx_package_all);

		protocol_store16(msg, 145);	//type
		protocol_store16(msg, 5);	//length
		protocol_store8(msg, ptr->radio_id);
		protocol_store32(msg, ptr->tx_package_all);

		protocol_store16(msg, 146);	//type
		protocol_store16(msg, 5);	//length
		protocol_store8(msg, ptr->radio_id);
		protocol_store32(msg, ptr->tx_error_frame);

		if(debugf){
			printf("\n%s:\n",__func__);
			printf("radio_id:%d\t", ptr->radio_id - 1);
			printf("beacon_frame_interval:%d\t", ptr->beacon_frame_interval);
			printf("DTIM_interval:%d\t", ptr->DTIM_interval);
			printf("RTS_threshold:%d\t", ptr->RTS_threshold);
			printf("frag_threshold:%d\t", ptr->frag_threshold);
			printf("preamble_length:%d\n", ptr->preamble_length);
			printf("current_txpower:%d\t", ptr->current_txpower);
			printf("current_channel:%d\t", ptr->current_channel);
			printf("max_sta_num:%d\t", ptr->max_sta_num);
			printf("current_wireless_mode:%d\t", ptr->current_wireless_mode);
			printf("switch_AMPDU:%d\n", ptr->switch_AMPDU);
			printf("switch_20M_40M:%d\t", ptr->switch_20M_40M);
			printf("switch_shortGI:%d\t", ptr->switch_shortGI);
			printf("switch_11n:%d\t", ptr->switch_11n);
			printf("wifi_state:%d\t\t", ptr->wifi_state);
			printf("wifi_state_continuous_time:%d\n", ptr->wifi_state_continuous_time);
			printf("auto_power_control_switch:%d\t", ptr->auto_power_control_switch);
			printf("auto_wireless_channel_switch:%d\t", ptr->auto_wireless_channel_switch);
			printf("rx_signal_average_strength:%d\n", ptr->rx_signal_average_strength);
			printf("rx_signal_max_strength:%d\t", ptr->rx_signal_max_strength);
			printf("rx_signal_min_strength:%d\t", ptr->rx_signal_min_strength);
			printf("wireless_if_updown_count:%d\n", ptr->wireless_if_updown_count);
			printf("tx_data_package:%d\t", ptr->tx_data_package);
			printf("rx_data_package:%d\t", ptr->rx_data_package);
			printf("rx_data_package_bytes:%llu\t\t", ptr->rx_data_package_bytes);
			printf("tx_data_package_bytes:%llu\n", ptr->tx_data_package_bytes);
			printf("wireless_channel_down_retry_data_package:%d\t", ptr->wireless_channel_down_retry_data_package);
			printf("wireless_channel_down_retry_data_frame:%d\n", ptr->wireless_channel_down_retry_data_package);
			printf("rx_manage_frame:%d\t", ptr->rx_manage_frame);
			printf("rx_ctrl_frame:%d\t\t", ptr->rx_ctrl_frame);
			printf("rx_data_frame:%d\n", ptr->rx_data_frame);
			printf("tx_manage_frame:%d\t", ptr->tx_manage_frame);
			printf("tx_ctrl_frame:%d\t\t", ptr->tx_ctrl_frame);
			printf("tx_data_frame:%d\n", ptr->tx_data_frame);
			printf("rx_error_data_package:%d\t", ptr->rx_error_data_package);
			printf("rx_error_frame:%d\t", ptr->rx_error_frame);
			printf("rx_fcs_mac_error_frame:%d\t", ptr->rx_fcs_mac_error_frame);
			printf("rx_mic_mac_error_frame:%d\t", ptr->rx_mic_mac_error_frame);
			printf("rx_dissect_failed_mac_error_frame:%d\n", ptr->rx_dissect_failed_mac_error_frame);
			printf("rx_authenticate_frame:%d\t", ptr->rx_authenticate_frame);
			printf("rx_associate_frame:%d\t", ptr->rx_associate_frame);
			printf("tx_signaling_packet:%d\t", ptr->tx_signaling_packet);
			printf("associate_sta:%d\n", ptr->associate_sta);
			printf("tx_package:%d\t", ptr->tx_package);
			printf("rx_package:%d\t\t", ptr->rx_package);
			printf("tx_package_bytes:%llu\t", ptr->tx_package_bytes);
			printf("rx_package_bytes:%llu\n", ptr->rx_package_bytes);
			printf("rx_phy_errors:%d\t", ptr->rx_phy_errors);
			printf("rx_unicast:%d\t", ptr->rx_unicast);
			printf("tx_unicast:%d\t", ptr->tx_unicast);
			printf("rx_ununicast:%d\t", ptr->rx_ununicast);
			printf("tx_ununicast:%d\n", ptr->tx_ununicast);
			printf("wireless_rx_interval_bytes:%d\t", ptr->wireless_rx_interval_bytes);
			printf("wireless_tx_interval_bytes:%d\n", ptr->wireless_tx_interval_bytes);
			printf("err_frame_ratio:%d\t", ptr->err_frame_ratio);
			printf("rx_package_all:%d\t", ptr->rx_package_all);
			printf("tx_package_all:%d\t", ptr->tx_package_all);
			printf("tx_error_frame:%d\n", ptr->tx_error_frame);
		}
	}
	printf_d("\n");
	if(assemble_msg_elem(msg, MSG_ELEMENT_AP_REPORT_RADIO_INFO_REQUEST_TYPE) == FALSE)
	{
		return RAISE_ERROR(ERROR_NEED_RESOURCE, "assemble report wireless state count request failed!");
	}
	return TRUE;
}

BOOL assemble_report_sta_ability_request(void *arg, TMSG *msg)
{
	STA_ABILITY_INFO *ability = (STA_ABILITY_INFO *)arg;
	if(ability == NULL)
		RAISE_SYS_ERR(ERROR_WRONG_ARG);	
	CREATE_PROTOCOL_MESSAGE(*msg, 4+11, RAISE_SYS_ERR(ERROR_NEED_RESOURCE););
	u16	ver = 0;
	u8	seq = 0;
	protocol_store16(msg, ver);
	protocol_store8(msg, seq);
	protocol_store8(msg, 0);

	protocol_store16(msg, 20);	//type
	protocol_store16(msg, MAC_ADDR_LEN+1);	//length
	protocol_store_rawbytes(msg, (char *)ability->mac, MAC_ADDR_LEN);
	protocol_store8(msg, ability->is_Qos);
	if(debugf){
		printf("\n%s,sta("MAC_FMT"),is_QoS:%d\n",__func__,MAC2STR(ability->mac),ability->is_Qos);
	}
	if(assemble_msg_elem(msg, MSG_ELEMENT_AP_REPORT_STA_INFO_REQUEST_TYPE) == FALSE)
	{
		return RAISE_ERROR(ERROR_NEED_RESOURCE, "assemble report sta ability request failed!");
	}
	return TRUE;
}


BOOL set_mode2(u8 rid)
{
	char cmd[128] = {0};
	int extoffset = 0, cwmmode = 0;
	char wlanname[32] = {0};
	
	sprintf(wlanname, "athscanwifi%d", rid);
	if(gRadioInfoValue[rid].channel_width == 0)
	{
		extoffset = 0;
		cwmmode = 0;
	}
	else if(gRadioInfoValue[rid].channel_width == 1)
	{
		extoffset = 0;
		cwmmode = 1;
	}
	else if(gRadioInfoValue[rid].channel_width == 2)
	{
		extoffset = -1;
		cwmmode = 2;
	}
	else if(gRadioInfoValue[rid].channel_width == 3)
	{
		extoffset = 1;
		cwmmode = 2;
	}
	if(gRadioInfoValue[rid].radiotype == IEEE80211_PARAM_MODE_11b)	//11b(2.4G)
		sprintf(cmd,"set_mode %s 11b", wlanname);
	else if(gRadioInfoValue[rid].radiotype == IEEE80211_PARAM_MODE_11a)	//11a(5G)
		sprintf(cmd,"set_mode %s 11a", wlanname);
	else if(gRadioInfoValue[rid].radiotype == IEEE80211_PARAM_MODE_11g)	//11g(2.4G)
		sprintf(cmd,"set_mode %s 11g", wlanname);
	else if(gRadioInfoValue[rid].radiotype == IEEE80211_PARAM_MODE_11bg)	//11b(2.4G) & 11g(2.4G)
		sprintf(cmd,"set_mode %s 11bg", wlanname);
	else if(gRadioInfoValue[rid].radiotype == IEEE80211_PARAM_MODE_11n_a) //11an(5G)
		sprintf(cmd,"set_mode %s 11na %d %d", wlanname, extoffset, cwmmode);
	else if(gRadioInfoValue[rid].radiotype == IEEE80211_PARAM_MODE_11an_a)	//11a(5G) & 11an(5G)
		sprintf(cmd,"set_mode %s 11ana %d %d", wlanname, extoffset, cwmmode);
	else if(gRadioInfoValue[rid].radiotype == IEEE80211_PARAM_MODE_11n_g)	//11gn(2.4G)
		sprintf(cmd,"set_mode %s 11ng %d %d", wlanname, extoffset, cwmmode);
	else if(gRadioInfoValue[rid].radiotype == IEEE80211_PARAM_MODE_11gn_g)	//11g(2.4G) & 11gn(2.4G)
		sprintf(cmd,"set_mode %s 11gng %d %d", wlanname, extoffset, cwmmode);
	else if(gRadioInfoValue[rid].radiotype == IEEE80211_PARAM_MODE_11bgn_g)	//11b(2.4G) & 11g(2.4G) & 11gn(2.4G)
		sprintf(cmd,"set_mode %s 11bgng %d %d", wlanname, extoffset, cwmmode);
	else
	{
		printf_d("radio->radio_type = %d\n", gRadioInfoValue[rid].radiotype);
		return FALSE;
	}
	SYSTEM(cmd);
	sprintf(cmd, "iwconfig %s channel %d", wlanname, gRadioInfoValue[rid].channel);
	SYSTEM(cmd);
	
	return TRUE;
}

BOOL assemble_report_sta_count_request(void *arg, TMSG *msg)
{
	int i = 0;
	int len = 0;
	STA_STATE_COUNT_INFO *state_count_info = (STA_STATE_COUNT_INFO *)arg;
	if(state_count_info == NULL)
		RAISE_SYS_ERR(ERROR_WRONG_ARG);
	if(state_count_info->count == 0)
		return TRUE;
	len = 4+(11*6+14*13+18*2)*state_count_info->count;

	CREATE_PROTOCOL_MESSAGE(*msg, len, RAISE_SYS_ERR(ERROR_NEED_RESOURCE););
	u16	ver = 0;
	u8	seq = 0;
	protocol_store16(msg, ver);
	protocol_store8(msg, seq);
	protocol_store8(msg, 0);

	for(i = 0; i < state_count_info->count; i++)
	{
		protocol_store16(msg, 50);	//type
		protocol_store16(msg, MAC_ADDR_LEN+4);	//length
		protocol_store_rawbytes(msg, (char *)state_count_info[i].mac, MAC_ADDR_LEN);
		protocol_store32(msg, state_count_info[i].IPv4);

		protocol_store16(msg, 51);	//type
		protocol_store16(msg, MAC_ADDR_LEN+1);	//length
		protocol_store_rawbytes(msg, (char *)state_count_info[i].mac, MAC_ADDR_LEN);
		protocol_store8(msg, state_count_info[i].wireless_mode);

		protocol_store16(msg, 52);	//type
		protocol_store16(msg, MAC_ADDR_LEN+1);	//length
		protocol_store_rawbytes(msg, (char *)state_count_info[i].mac, MAC_ADDR_LEN);
		protocol_store8(msg, state_count_info[i].wireless_channel);

		protocol_store16(msg, 53);	//type
		protocol_store16(msg, MAC_ADDR_LEN+1);	//length
		protocol_store_rawbytes(msg, (char *)state_count_info[i].mac, MAC_ADDR_LEN);
		protocol_store8(msg, state_count_info[i].is_power_save);

		protocol_store16(msg, 54);	//type
		protocol_store16(msg, MAC_ADDR_LEN+4);	//length
		protocol_store_rawbytes(msg, (char *)state_count_info[i].mac, MAC_ADDR_LEN);
		protocol_store32(msg, state_count_info[i].vlan_id);

		protocol_store16(msg, 55);	//type
		protocol_store16(msg, MAC_ADDR_LEN+4);	//length
		protocol_store_rawbytes(msg, (char *)state_count_info[i].mac, MAC_ADDR_LEN);
		protocol_store32(msg, state_count_info[i].rate);

		protocol_store16(msg, 56);	//type
		protocol_store16(msg, MAC_ADDR_LEN+1);	//length
		protocol_store_rawbytes(msg, (char *)state_count_info[i].mac, MAC_ADDR_LEN);
		protocol_store8(msg, state_count_info[i].rssi);

		protocol_store16(msg, 57);	//type
		protocol_store16(msg, MAC_ADDR_LEN+1);	//length
		protocol_store_rawbytes(msg, (char *)state_count_info[i].mac, MAC_ADDR_LEN);
		protocol_store8(msg, state_count_info[i].Noise);

		protocol_store16(msg, 58);	//type
		protocol_store16(msg, MAC_ADDR_LEN+4);	//length
		protocol_store_rawbytes(msg, (char *)state_count_info[i].mac, MAC_ADDR_LEN);
		protocol_store32(msg, state_count_info[i].rx_rate);

		protocol_store16(msg, 59);	//type
		protocol_store16(msg, MAC_ADDR_LEN+4);	//length
		protocol_store_rawbytes(msg, (char *)state_count_info[i].mac, MAC_ADDR_LEN);
		protocol_store32(msg, state_count_info[i].tx_rate);

		protocol_store16(msg, 100);	//type
		protocol_store16(msg, MAC_ADDR_LEN+1);	//length
		protocol_store_rawbytes(msg, (char *)state_count_info[i].mac, MAC_ADDR_LEN);
		protocol_store8(msg, state_count_info[i].SNR);

		protocol_store16(msg, 101);	//type
		protocol_store16(msg, MAC_ADDR_LEN+4);	//length
		protocol_store_rawbytes(msg, (char *)state_count_info[i].mac, MAC_ADDR_LEN);
		protocol_store32(msg, state_count_info[i].sta_rx_packet);

		protocol_store16(msg, 102);	//type
		protocol_store16(msg, MAC_ADDR_LEN+8);	//length
		protocol_store_rawbytes(msg, (char *)state_count_info[i].mac, MAC_ADDR_LEN);
		protocol_store64(msg, state_count_info[i].sta_rx_bytes);

		protocol_store16(msg, 103);	//type
		protocol_store16(msg, MAC_ADDR_LEN+4);	//length
		protocol_store_rawbytes(msg, (char *)state_count_info[i].mac, MAC_ADDR_LEN);
		protocol_store32(msg, state_count_info[i].sta_tx_packet);

		protocol_store16(msg, 104);	//type
		protocol_store16(msg, MAC_ADDR_LEN+8);	//length
		protocol_store_rawbytes(msg, (char *)state_count_info[i].mac, MAC_ADDR_LEN);
		protocol_store64(msg, state_count_info[i].sta_tx_bytes);

		protocol_store16(msg, 105);	//type
		protocol_store16(msg, MAC_ADDR_LEN+4);	//length
		protocol_store_rawbytes(msg, (char *)state_count_info[i].mac, MAC_ADDR_LEN);
		protocol_store32(msg, state_count_info[i].tx_frag_packets);

		protocol_store16(msg, 106);	//type
		protocol_store16(msg, MAC_ADDR_LEN+4);	//length
		protocol_store_rawbytes(msg, (char *)state_count_info[i].mac, MAC_ADDR_LEN);
		protocol_store32(msg, state_count_info[i].rx_frag_packets);

		protocol_store16(msg, 107);	//type
		protocol_store16(msg, MAC_ADDR_LEN+4);	//length
		protocol_store_rawbytes(msg, (char *)state_count_info[i].mac, MAC_ADDR_LEN);
		protocol_store32(msg, state_count_info[i].rx_error);

		protocol_store16(msg, 108);	//type
		protocol_store16(msg, MAC_ADDR_LEN+4);	//length
		protocol_store_rawbytes(msg, (char *)state_count_info[i].mac, MAC_ADDR_LEN);
		protocol_store32(msg, state_count_info[i].tx_error);

		protocol_store16(msg, 109);	//type
		protocol_store16(msg, MAC_ADDR_LEN+4);	//length
		protocol_store_rawbytes(msg, (char *)state_count_info[i].mac, MAC_ADDR_LEN);
		protocol_store32(msg, state_count_info[i].rx_drop);

		protocol_store16(msg, 110);	//type
		protocol_store16(msg, MAC_ADDR_LEN+4);	//length
		protocol_store_rawbytes(msg, (char *)state_count_info[i].mac, MAC_ADDR_LEN);
		protocol_store32(msg, state_count_info[i].tx_drop);

		if(debugf){
			printf("\n%s,\t",__func__);
			printf("stamac("MAC_FMT")\t",MAC2STR(state_count_info[i].mac));

			char ip_str[24] = {0};
			ip_int2str(state_count_info[i].IPv4, (unsigned char *)ip_str);
			printf("IPv4:%s\n",ip_str);
			
			printf("wireless_mode:%d\t",state_count_info[i].wireless_mode);
			printf("wireless_channel:%d\t",state_count_info[i].wireless_channel);
			printf("is_power_save:%d\t",state_count_info[i].is_power_save);
			printf("vlan_id:%d\t",state_count_info[i].vlan_id);
			printf("rate:%d\n",state_count_info[i].rate);
			printf("rssi:%d\t",state_count_info[i].rssi);
			printf("Noise:%d\t",state_count_info[i].Noise);
			printf("rx_rate:%d\t",state_count_info[i].rx_rate);
			printf("tx_rate:%d\t",state_count_info[i].tx_rate);
			printf("SNR:%d\n",state_count_info[i].SNR);
			printf("sta_rx_packet:%d\t",state_count_info[i].sta_rx_packet);
			printf("sta_rx_bytes:%llu\t",state_count_info[i].sta_rx_bytes);
			printf("sta_tx_packet:%d\t",state_count_info[i].sta_tx_packet);
			printf("sta_tx_bytes:%llu\n",state_count_info[i].sta_tx_bytes);
			printf("tx_frag_packets:%d\t",state_count_info[i].tx_frag_packets);
			printf("rx_frag_packets:%d\n",state_count_info[i].rx_frag_packets);
			printf("rx_error:%d\t",state_count_info[i].rx_error);
			printf("tx_error:%d\t",state_count_info[i].tx_error);
			printf("rx_drop:%d\t",state_count_info[i].rx_drop);
			printf("tx_drop:%d\n\n",state_count_info[i].tx_drop);
		}
	}
	if(assemble_msg_elem(msg, MSG_ELEMENT_AP_REPORT_STA_INFO_REQUEST_TYPE) == FALSE)
	{
		return RAISE_ERROR(ERROR_NEED_RESOURCE, "assemble report sta ability request failed!");
	}
	return TRUE;
}


BOOL assemble_report_key_event_radio_frequency_request(void *arg, TMSG *msg)
{
	int count = 0;
	AP_KEY_EVENT_INFO *key = (AP_KEY_EVENT_INFO *)arg;
	AP_KEY_EVENT_INFO *ptr = NULL;
	if(key == NULL)
		RAISE_SYS_ERR(ERROR_WRONG_ARG);	
	for(ptr = key; ptr!= NULL; ptr = ptr->next)
		count++;
	CREATE_PROTOCOL_MESSAGE(*msg, 4+8*count, RAISE_SYS_ERR(ERROR_NEED_RESOURCE););

	u16	ver = 0;
	u8	seq = 0;
	protocol_store16(msg, ver);
	protocol_store8(msg, seq);
	protocol_store8(msg, 0);	//reserve
	for(ptr = key; ptr!= NULL; ptr = ptr->next)
	{
		protocol_store16(msg, 2);	//type
		protocol_store16(msg, 4);	//length
		protocol_store8(msg, key->radio_id);
		protocol_store8(msg, key->radio_frequency_switch);
		protocol_store16(msg, key->switch_reason);
	}

	if(assemble_msg_elem(msg, MSG_ELEMENT_AP_REPORT_KEY_INFO_REQUEST_TYPE) == FALSE)
	{
		return RAISE_ERROR(ERROR_NEED_RESOURCE, "assemble report ap key event radio frequency request failed!");
	}
	return TRUE;
}

BOOL assemble_report_key_event_wlan_disable_request(void *arg, TMSG *msg)
{
	int count = 0;
	AP_KEY_EVENT_INFO *key = (AP_KEY_EVENT_INFO *)arg;
	AP_KEY_EVENT_INFO *ptr = NULL;
	if(key == NULL)
		RAISE_SYS_ERR(ERROR_WRONG_ARG);	
	for(ptr = key; ptr!= NULL; ptr = ptr->next)
		count++;
	CREATE_PROTOCOL_MESSAGE(*msg, 4+8*count, RAISE_SYS_ERR(ERROR_NEED_RESOURCE););
	u16	ver = 0;
	u8	seq = 0;
	protocol_store16(msg, ver);
	protocol_store8(msg, seq);
	protocol_store8(msg, 0);	//reserve

	for(ptr = key; ptr!= NULL; ptr = ptr->next)
	{
		protocol_store16(msg, 5);	//type
		protocol_store16(msg, 4);	//length
		protocol_store8(msg, key->radio_id);
		protocol_store8(msg, key->wlan_id);
		protocol_store16(msg, key->delete_reason);
	}

	if(assemble_msg_elem(msg, MSG_ELEMENT_AP_REPORT_KEY_INFO_REQUEST_TYPE) == FALSE)
	{
		return RAISE_ERROR(ERROR_NEED_RESOURCE, "assemble report key event wlan enable request failed!");
	}
	return TRUE;
}

BOOL assemble_report_key_event_wlan_enable_request(void *arg, TMSG *msg)
{
	int count = 0;
	AP_KEY_EVENT_INFO *key = (AP_KEY_EVENT_INFO *)arg;
	AP_KEY_EVENT_INFO *ptr = NULL;
	if(key == NULL)
		RAISE_SYS_ERR(ERROR_WRONG_ARG);	
	for(ptr = key; ptr!= NULL; ptr = ptr->next)
		count++;
	CREATE_PROTOCOL_MESSAGE(*msg, 4+12*count, RAISE_SYS_ERR(ERROR_NEED_RESOURCE););
	u16	ver = 0;
	u8	seq = 0;
	protocol_store16(msg, ver);
	protocol_store8(msg, seq);
	protocol_store8(msg, 0);	//reserve

	for(ptr = key; ptr!= NULL; ptr = ptr->next)
	{
		protocol_store16(msg, 6);	//type
		protocol_store16(msg, 8);	//length
		protocol_store8(msg, key->radio_id);
		protocol_store8(msg, key->wlan_id);
		protocol_store_rawbytes(msg, key->BSSID, MAC_ADDR_LEN);
	}

	if(assemble_msg_elem(msg, MSG_ELEMENT_AP_REPORT_KEY_INFO_REQUEST_TYPE) == FALSE)
	{
		return RAISE_ERROR(ERROR_NEED_RESOURCE, "assemble report key event wlan disable request failed!");
	}
	return TRUE;
}

BOOL assemble_report_ap_eth_ability_request(void *arg, TMSG *msg)
{
	AP_ETH_ABILITY_INFO *ability = (AP_ETH_ABILITY_INFO *)arg;
	if(ability == NULL)
		RAISE_SYS_ERR(ERROR_WRONG_ARG);	
	CREATE_PROTOCOL_MESSAGE(*msg, 4+5+strlen(ability->if_info)+6+9*2+11, RAISE_SYS_ERR(ERROR_NEED_RESOURCE););
	u16	ver = 0;
	u8	seq = 0;
	protocol_store16(msg, ver);
	protocol_store8(msg, seq);
	protocol_store8(msg, 0);	//reserve

	protocol_store16(msg, 20);	//type
	protocol_store16(msg, 1+strlen(ability->if_info));	//length
	protocol_store8(msg, ability->eth_id);	//eth id
	protocol_store_str(msg, ability->if_info);
	//(msg->offset) += 64 - strlen(ability->if_info);

	protocol_store16(msg, 21);	//type
	protocol_store16(msg, 2);	//length
	protocol_store8(msg, ability->eth_id);	//eth id
	protocol_store8(msg, ability->if_type);

	protocol_store16(msg, 22);	//type
	protocol_store16(msg, 5);	//length
	protocol_store8(msg, ability->eth_id);	//eth id
	protocol_store32(msg, ability->if_MTU);

	protocol_store16(msg, 23);	//type
	protocol_store16(msg, 5);	//length
	protocol_store8(msg, ability->eth_id);	//eth id
	protocol_store32(msg, ability->if_bandwidth);

	protocol_store16(msg, 24);	//type
	protocol_store16(msg, 7);	//length
	protocol_store8(msg, ability->eth_id);	//eth id
	protocol_store_rawbytes(msg, (char *)ability->if_mac, MAC_ADDR_LEN);

	if(debugf){
		printf("\n%s:\n",__func__);
		printf("eth_id:%d\t",ability->eth_id);
		printf("if_info:%s\t",ability->if_info);
		printf("if_type:%d\t",ability->if_type);
		printf("if_MTU:%d\n",ability->if_MTU);
		printf("if_bandwidth:%d\t",ability->if_bandwidth);
		printf("if_mac:"MAC_FMT"\n\n",MAC2STR((u8*)(ability->if_mac)));
	}
	if(assemble_msg_elem(msg, MSG_ELEMENT_AP_REPORT_ETH_INFO_REQUEST_TYPE) == FALSE)
	{
		return RAISE_ERROR(ERROR_NEED_RESOURCE, "assemble report ap eth ability request failed!");
	}
	return TRUE;
}

BOOL assemble_report_ap_eth_state_count_request(void *arg, TMSG *msg)
{
	AP_ETH_STATE_COUNT_INFO *eth_count = (AP_ETH_STATE_COUNT_INFO *)arg;
	if(eth_count == NULL)
		RAISE_SYS_ERR(ERROR_WRONG_ARG);	

	CREATE_PROTOCOL_MESSAGE(*msg, 4+9*13+13*2, RAISE_SYS_ERR(ERROR_NEED_RESOURCE););
	u16	ver = 0;
	u8	seq = 0;
	protocol_store16(msg, ver);
	protocol_store8(msg, seq);
	protocol_store8(msg, 0);	//reserve

	protocol_store16(msg, 100);	//type
	protocol_store16(msg, 5);	//length
	protocol_store8(msg, eth_count->eth_id);	//eth id
	protocol_store32(msg, eth_count->rx_unicast_packet);

	protocol_store16(msg, 101);	//type
	protocol_store16(msg, 5);	//length
	protocol_store8(msg, eth_count->eth_id);	//eth id
	protocol_store32(msg, eth_count->rx_ununicast_packet);

	protocol_store16(msg, 102);	//type
	protocol_store16(msg, 9);	//length
	protocol_store8(msg, eth_count->eth_id);	//eth id
	protocol_store64(msg, eth_count->rx_bytes);

	protocol_store16(msg, 103);	//type
	protocol_store16(msg, 5);	//length
	protocol_store8(msg, eth_count->eth_id);	//eth id
	protocol_store32(msg, eth_count->rx_drop_packet);

	protocol_store16(msg, 104);	//type
	protocol_store16(msg, 5);	//length
	protocol_store8(msg, eth_count->eth_id);	//eth id
	protocol_store32(msg, eth_count->rx_error_packet);

	protocol_store16(msg, 105);	//type
	protocol_store16(msg, 5);	//length
	protocol_store8(msg, eth_count->eth_id);	//eth id
	protocol_store32(msg, eth_count->tx_unicast_packet);

	protocol_store16(msg, 106);	//type
	protocol_store16(msg, 5);	//length
	protocol_store8(msg, eth_count->eth_id);	//eth id
	protocol_store32(msg, eth_count->tx_ununicast_packet);

	protocol_store16(msg, 107);	//type
	protocol_store16(msg, 9);	//length
	protocol_store8(msg, eth_count->eth_id);	//eth id
	protocol_store64(msg, eth_count->tx_bytes);

	protocol_store16(msg, 108);	//type
	protocol_store16(msg, 5);	//length
	protocol_store8(msg, eth_count->eth_id);	//eth id
	protocol_store32(msg, eth_count->tx_drop_packet);

	protocol_store16(msg, 109);	//type
	protocol_store16(msg, 5);	//length
	protocol_store8(msg, eth_count->eth_id);	//eth id
	protocol_store32(msg, eth_count->tx_error_packet);

	protocol_store16(msg, 110);	//type
	protocol_store16(msg, 5);	//length
	protocol_store8(msg, eth_count->eth_id);	//eth id
	protocol_store32(msg, eth_count->if_up_down_count);

	protocol_store16(msg, 111);	//type
	protocol_store16(msg, 5);	//length
	protocol_store8(msg, eth_count->eth_id);	//eth id
	protocol_store32(msg, eth_count->rx_multicast);

	protocol_store16(msg, 112);	//type
	protocol_store16(msg, 5);	//length
	protocol_store8(msg, eth_count->eth_id);	//eth id
	protocol_store32(msg, eth_count->rx_broadcast);

	protocol_store16(msg, 113);	//type
	protocol_store16(msg, 5);	//length
	protocol_store8(msg, eth_count->eth_id);	//eth id
	protocol_store32(msg, eth_count->tx_multicast);

	protocol_store16(msg, 114);	//type
	protocol_store16(msg, 5);	//length
	protocol_store8(msg, eth_count->eth_id);	//eth id
	protocol_store32(msg, eth_count->tx_broadcast);

	if(debugf){
		printf("\n%s,\t",__func__);
		printf("eth_id:%d\n",eth_count->eth_id);
		printf("if_up_down_count:%d\t",eth_count->if_up_down_count);
		printf("rx_unicast_packet:%d\t",eth_count->rx_unicast_packet);
		printf("rx_ununicast_packet:%d\t",eth_count->rx_ununicast_packet);
		printf("rx_bytes:%llu\t",eth_count->rx_bytes);
		printf("rx_drop_packet:%d\n",eth_count->rx_drop_packet);
		printf("rx_error_packet:%d\t",eth_count->rx_error_packet);
		printf("tx_unicast_packet:%d\t",eth_count->tx_unicast_packet);
		printf("tx_ununicast_packet:%d\t",eth_count->tx_ununicast_packet);
		printf("tx_bytes:%llu\t",eth_count->tx_bytes);
		printf("tx_drop_packet:%d\n",eth_count->tx_drop_packet);
		printf("tx_error_packet:%d\t",eth_count->tx_error_packet);
		printf("rx_multicast:%d\t",eth_count->rx_multicast);
		printf("rx_broadcast:%d\t",eth_count->rx_broadcast);
		printf("tx_multicast:%d\t",eth_count->tx_multicast);
		printf("tx_broadcast:%d\n\n",eth_count->tx_broadcast);
	}
	if(assemble_msg_elem(msg, MSG_ELEMENT_AP_REPORT_ETH_INFO_REQUEST_TYPE) == FALSE)
	{
		return RAISE_ERROR(ERROR_NEED_RESOURCE, "assemble report ap eth count request failed!");
	}
	return TRUE;
}

BOOL assemble_report_ap_ath_state_count_request(void *arg, TMSG *msg)
{
	int count = 0;
	int i = 0;
	AP_ATH_STATE_COUNT_INFO *state_count = (AP_ATH_STATE_COUNT_INFO *)arg;
	AP_ATH_STATE_COUNT_INFO *ptr = NULL;
	if(state_count == NULL)
		RAISE_SYS_ERR(ERROR_WRONG_ARG);	
	count = state_count->count;
	CREATE_PROTOCOL_MESSAGE(*msg, 4+(6+strlen(state_count->SSID)+10*19+7*7+14*2+12)*count, RAISE_SYS_ERR(ERROR_NEED_RESOURCE););
	u16	ver = 0;
	u8	seq = 0;
	protocol_store16(msg, ver);
	protocol_store8(msg, seq);
	protocol_store8(msg, 0);	//reserve

	for(ptr = state_count, i = count; ((ptr!= NULL) && (i > 0)); ptr = ptr->next, i--)
	{
		protocol_store16(msg, 50);	//type
		protocol_store16(msg, 2+strlen(state_count->SSID));	//length
		protocol_store8(msg, state_count->radio_id);
		protocol_store8(msg, state_count->wlan_id);
		protocol_store_str(msg, state_count->SSID);

		protocol_store16(msg, 51);	//type
		protocol_store16(msg, 3);	//length
		protocol_store8(msg, state_count->radio_id);
		protocol_store8(msg, state_count->wlan_id);
		protocol_store8(msg, state_count->SSID_switch);

		protocol_store16(msg, 52);	//type
		protocol_store16(msg, 3);	//length
		protocol_store8(msg, state_count->radio_id);
		protocol_store8(msg, state_count->wlan_id);
		protocol_store8(msg, state_count->is_hide_SSID);

		protocol_store16(msg, 53);	//type
		protocol_store16(msg, 3);	//length
		protocol_store8(msg, state_count->radio_id);
		protocol_store8(msg, state_count->wlan_id);
		protocol_store8(msg, state_count->is_separate);

		protocol_store16(msg, 54);	//type
		protocol_store16(msg, 3);	//length
		protocol_store8(msg, state_count->radio_id);
		protocol_store8(msg, state_count->wlan_id);
		protocol_store8(msg, state_count->authorize_type_802_11);

		protocol_store16(msg, 55);	//type
		protocol_store16(msg, 3);	//length
		protocol_store8(msg, state_count->radio_id);
		protocol_store8(msg, state_count->wlan_id);
		protocol_store8(msg, state_count->safe_type);

		protocol_store16(msg, 56);	//type
		protocol_store16(msg, 3);	//length
		protocol_store8(msg, state_count->radio_id);
		protocol_store8(msg, state_count->wlan_id);
		protocol_store8(msg, state_count->authorize_mode);

		protocol_store16(msg, 57);	//type
		protocol_store16(msg, 3);	//length
		protocol_store8(msg, state_count->radio_id);
		protocol_store8(msg, state_count->wlan_id);
		protocol_store8(msg, state_count->encryption_type);

		protocol_store16(msg, 58);	//type
		protocol_store16(msg, 6);	//length
		protocol_store8(msg, state_count->radio_id);
		protocol_store8(msg, state_count->wlan_id);
		protocol_store32(msg, state_count->vlan_mark);

		protocol_store16(msg, 59);	//type
		protocol_store16(msg, 6);	//length
		protocol_store8(msg, state_count->radio_id);
		protocol_store8(msg, state_count->wlan_id);
		protocol_store32(msg, state_count->max_sta_num);

		protocol_store16(msg, 60);	//type
		protocol_store16(msg, 6);	//length
		protocol_store8(msg, state_count->radio_id);
		protocol_store8(msg, state_count->wlan_id);
		protocol_store32(msg, state_count->sta_uplink_max_speed);

		protocol_store16(msg, 61);	//type
		protocol_store16(msg, 6);	//length
		protocol_store8(msg, state_count->radio_id);
		protocol_store8(msg, state_count->wlan_id);
		protocol_store32(msg, state_count->sta_downlink_max_speed);

		protocol_store16(msg, 62);	//type
		protocol_store16(msg, 8);	//length
		protocol_store8(msg, state_count->radio_id);
		protocol_store8(msg, state_count->wlan_id);
		protocol_store_rawbytes(msg, (char *)state_count->BSSID, MAC_ADDR_LEN);

		protocol_store16(msg, 100);	//type
		protocol_store16(msg, 6);	//length
		protocol_store8(msg, state_count->radio_id);
		protocol_store8(msg, state_count->wlan_id);
		protocol_store32(msg, state_count->tx_packet);

		protocol_store16(msg, 101);	//type
		protocol_store16(msg, 6);	//length
		protocol_store8(msg, state_count->radio_id);
		protocol_store8(msg, state_count->wlan_id);
		protocol_store32(msg, state_count->rx_packet);

		protocol_store16(msg, 102);	//type
		protocol_store16(msg, 10);	//length
		protocol_store8(msg, state_count->radio_id);
		protocol_store8(msg, state_count->wlan_id);
		protocol_store64(msg, state_count->rx_bytes);	//接收 102

		protocol_store16(msg, 103);	//type
		protocol_store16(msg, 10);	//length
		protocol_store8(msg, state_count->radio_id);
		protocol_store8(msg, state_count->wlan_id);
		protocol_store64(msg, state_count->tx_bytes);	//发送 103

		protocol_store16(msg, 104);	//type
		protocol_store16(msg, 6);	//length
		protocol_store8(msg, state_count->radio_id);
		protocol_store8(msg, state_count->wlan_id);
		protocol_store32(msg, state_count->downlink_retry_packet);

		/* ... */
		protocol_store16(msg, 112); //type
		protocol_store16(msg, 6);	//length
		protocol_store8(msg, state_count->radio_id);
		protocol_store8(msg, state_count->wlan_id);
		protocol_store32(msg, state_count->rx_ctrl_frame);

		protocol_store16(msg, 113); //type
		protocol_store16(msg, 6);	//length
		protocol_store8(msg, state_count->radio_id);
		protocol_store8(msg, state_count->wlan_id);
		protocol_store32(msg, state_count->rx_data_frame);

		protocol_store16(msg, 114); //type
		protocol_store16(msg, 6);	//length
		protocol_store8(msg, state_count->radio_id);
		protocol_store8(msg, state_count->wlan_id);
		protocol_store32(msg, state_count->rx_authenticate_frame);

		protocol_store16(msg, 115); //type
		protocol_store16(msg, 6);	//length
		protocol_store8(msg, state_count->radio_id);
		protocol_store8(msg, state_count->wlan_id);
		protocol_store32(msg, state_count->rx_associate_frame);

		protocol_store16(msg, 116); //type
		protocol_store16(msg, 6);	//length
		protocol_store8(msg, state_count->radio_id);
		protocol_store8(msg, state_count->wlan_id);
		protocol_store32(msg, state_count->tx_ctrl_frame);

		protocol_store16(msg, 117); //type
		protocol_store16(msg, 6);	//length
		protocol_store8(msg, state_count->radio_id);
		protocol_store8(msg, state_count->wlan_id);
		protocol_store32(msg, state_count->tx_data_frame);

		/* ... */
		
		protocol_store16(msg, 120);	//type
		protocol_store16(msg, 6);	//length
		protocol_store8(msg, state_count->radio_id);
		protocol_store8(msg, state_count->wlan_id);
		protocol_store32(msg, state_count->rx_errors);

		/* ... */

		protocol_store16(msg, 125);	//type
		protocol_store16(msg, 6);	//length
		protocol_store8(msg, state_count->radio_id);
		protocol_store8(msg, state_count->wlan_id);
		protocol_store32(msg, state_count->err_packet_ratio);

		/* ... */

		protocol_store16(msg, 128);	//type
		protocol_store16(msg, 6);	//length
		protocol_store8(msg, state_count->radio_id);
		protocol_store8(msg, state_count->wlan_id);
		protocol_store32(msg, state_count->rx_interval_bytes);

		protocol_store16(msg, 129);	//type
		protocol_store16(msg, 6);	//length
		protocol_store8(msg, state_count->radio_id);
		protocol_store8(msg, state_count->wlan_id);
		protocol_store32(msg, state_count->tx_interval_bytes);

		protocol_store16(msg, 130);	//type
		protocol_store16(msg, 6);	//length
		protocol_store8(msg, state_count->radio_id);
		protocol_store8(msg, state_count->wlan_id);
		protocol_store32(msg, state_count->success_associate);

		/* 131 */

		protocol_store16(msg, 132);	//type
		protocol_store16(msg, 6);	//length
		protocol_store8(msg, state_count->radio_id);
		protocol_store8(msg, state_count->wlan_id);
		protocol_store32(msg, state_count->request_associate);

		if(debugf){
			printf("\n%s:\n",__func__);
			printf("radio_id:%d\t",state_count->radio_id-1);
			printf("BSSID("MAC_FMT")\t",MAC2STR(state_count->BSSID));
			printf("SSID:%s\t",state_count->SSID);
			printf("SSID_switch:%d\t",state_count->SSID_switch);
			printf("is_hide_SSID:%d\n",state_count->is_hide_SSID);
			printf("is_separate:%d\t",state_count->is_separate);
			printf("authorize_type_802_11:%d\t\t",state_count->authorize_type_802_11);
			printf("safe_type:%d\t",state_count->safe_type);
			printf("authorize_mode:%d\t",state_count->authorize_mode);
			printf("encryption_type:%d\n",state_count->encryption_type);
			printf("vlan_mark:%d\t",state_count->vlan_mark);
			printf("max_sta_num:%d\t",state_count->max_sta_num);
			printf("rx_errors:%d\t",state_count->rx_errors);
			printf("err_packet_ratio:%d\t",state_count->err_packet_ratio);
			printf("rx_interval_bytes:%d\n",state_count->rx_interval_bytes);
			printf("tx_packet:%d\t",state_count->tx_packet);
			printf("rx_packet:%d\t",state_count->rx_packet);
			printf("tx_bytes:%llu\t",state_count->tx_bytes);
			printf("rx_bytes:%llu\t",state_count->rx_bytes);
			printf("downlink_retry_packet:%d\n",state_count->downlink_retry_packet);
			printf("rx_ctrl_frame:%d\t",state_count->rx_ctrl_frame);
			printf("rx_data_frame:%d\t",state_count->rx_data_frame);
			printf("sta_uplink_max_speed:%d\t",state_count->sta_uplink_max_speed);
			printf("sta_downlink_max_speed:%d\t",state_count->sta_downlink_max_speed);
			printf("rx_authenticate_frame:%d\n",state_count->rx_authenticate_frame);
			printf("rx_associate_frame:%d\t",state_count->rx_associate_frame);
			printf("tx_ctrl_frame:%d\t",state_count->tx_ctrl_frame);
			printf("tx_data_frame:%d\t",state_count->tx_data_frame);
			printf("tx_interval_bytes:%d\t",state_count->tx_interval_bytes);
			printf("success_associate:%d\t",state_count->success_associate);
			printf("request_associate:%d\n",state_count->request_associate);
		}
	}

	if(assemble_msg_elem(msg, MSG_ELEMENT_AP_REPORT_ATH_INFO_REQUEST_TYPE) == FALSE)
	{
		return RAISE_ERROR(ERROR_NEED_RESOURCE, "assemble report ap ath info request failed!");
	}
	return TRUE;
}

/* ------------------------------------------------------------------------------------------------------------------ */
/* Assemble a response Message. */
BOOL assemble_add_wlan_response(void *arg, TMSG *msg)
{
	WLAN_INFO *wlanconf = (WLAN_INFO *)arg;
	WLAN_INFO *ptr = wlanconf;
	int count = 0;
	if(wlanconf == NULL)
		RAISE_SYS_ERR(ERROR_WRONG_ARG);
	while(wlanconf != NULL)
	{
		count++;
		wlanconf = wlanconf->next;
	}
	CREATE_PROTOCOL_MESSAGE(*msg,count*9+1, RAISE_SYS_ERR(ERROR_NEED_RESOURCE););
	wlanconf = ptr;
	protocol_store8(msg, count);
	while(wlanconf != NULL)
	{
		protocol_store8(msg, wlanconf->rid);
		protocol_store8(msg, wlanconf->wid);
		protocol_store8(msg, wlanconf->ret);
		protocol_store_rawbytes(msg, (char *)wlanconf->BSSID, MAC_ADDR_LEN);
		wlanconf = wlanconf->next;
	}
	if(assemble_msg_elem(msg, MSG_ELEMENT_ADD_WLAN_RESPONSE_TYPE) == FALSE)
	{
		return RAISE_ERROR(ERROR_NEED_RESOURCE, "assemble add wlan response failed!");
	}
	return TRUE;
}
BOOL assemble_del_wlan_response(void *arg, TMSG *msg)
{
	WLAN_INFO *wlan = (WLAN_INFO *)arg;
	if(wlan == NULL)
		RAISE_SYS_ERR(ERROR_WRONG_ARG);
	CREATE_PROTOCOL_MESSAGE(*msg, 3, RAISE_SYS_ERR(ERROR_NEED_RESOURCE););
	protocol_store8(msg, wlan->rid);
	protocol_store8(msg, wlan->wid);
	protocol_store8(msg, wlan->ret);
	if(assemble_msg_elem(msg, MSG_ELEMENT_DEL_WLAN_RESPONSE_TYPE) == FALSE)
	{
		return RAISE_ERROR(ERROR_NEED_RESOURCE, "assemble delete wlan response failed!");
	}
	return TRUE;
}
BOOL assemble_add_del_sta_to_blacklist_response(void *arg, TMSG *msg)
{
	BLACK_LST *bl = (BLACK_LST *)arg;
	int i = 0;
	CREATE_PROTOCOL_MESSAGE(*msg, (bl->num)*(MAC_ADDR_LEN+1), RAISE_SYS_ERR(ERROR_NEED_RESOURCE););
	protocol_store8(msg, bl->num);
	for(i = 0; i < bl->num; i++)
	{
		protocol_store_rawbytes(msg, (char *)bl->maclst[i].mac, MAC_ADDR_LEN);
		protocol_store8(msg, bl->maclst[i].ret);
	}
	if(assemble_msg_elem(msg, MSG_ELEMENT_ADD_STA_TO_BALCKLIST_RESPONSE_TYPE) == FALSE)
	{
		return RAISE_ERROR(ERROR_NEED_RESOURCE, "assemble add del sta to blacklist response failed!");
	}
	return TRUE;
}
BOOL assemble_upgrade_response(void *arg, TMSG *msg)
{
	u8 *ret = (u8 *)arg;
	if(ret == NULL)
		RAISE_SYS_ERR(ERROR_WRONG_ARG);
	CREATE_PROTOCOL_MESSAGE(*msg, 1, RAISE_SYS_ERR(ERROR_NEED_RESOURCE););
	protocol_store8(msg, *ret);
	if(assemble_msg_elem(msg, MSG_ELEMENT_UPGRADE_RESPONSE_TYPE) == FALSE)
	{
		return RAISE_ERROR(ERROR_NEED_RESOURCE, "assemble upgrade response failed!");
	}
	return TRUE;
}

BOOL assemble_set_ntp_response(void *arg, TMSG *msg)
{
	NTP_SET *ntp = (NTP_SET *)arg;
	if(ntp == NULL)
		RAISE_SYS_ERR(ERROR_WRONG_ARG);
	CREATE_PROTOCOL_MESSAGE(*msg, 9, RAISE_SYS_ERR(ERROR_NEED_RESOURCE););
	protocol_store16(msg, ntp->ver);
	protocol_store8(msg, ntp->seq);
	protocol_store8(msg, 0);
	protocol_store16(msg, 1);
	protocol_store16(msg, 1);
	protocol_store8(msg, ntp->ret);
	if(assemble_msg_elem(msg, MSG_ELEMENT_SET_NTP_RESPONSE_TYPE) == FALSE)
	{
		return RAISE_ERROR(ERROR_NEED_RESOURCE, "assemble set ntp response failed!");
	}
	return TRUE;
}

/* Begin: Add by dongzw for station isolation 2012-11-21 */
BOOL assemble_set_isolation_response(void *arg, TMSG *msg)
{
	ISOLATION_SET *isolation = (ISOLATION_SET *)arg;
	if(isolation == NULL)
		RAISE_SYS_ERR(ERROR_WRONG_ARG);
	CREATE_PROTOCOL_MESSAGE(*msg, 9, RAISE_SYS_ERR(ERROR_NEED_RESOURCE););
	protocol_store16(msg, isolation->ver);
	protocol_store8(msg, isolation->seq);
	protocol_store8(msg, 0);
	protocol_store16(msg, 1);
	protocol_store16(msg, 1);
	protocol_store8(msg, isolation->ret);
	if(assemble_msg_elem(msg, MSG_ELEMENT_SET_ISOLATION_RESPONSE_TYPE) == FALSE) {
		return RAISE_ERROR(ERROR_NEED_RESOURCE, "assemble set isolation response failed!");
	}

	return TRUE;
}
/* End: Add by dongzw for station isolation 2012-11-21 */

BOOL assemble_echo_request(void *arg, TMSG *msg)
{
	CREATE_PROTOCOL_MESSAGE(*msg, 4, RAISE_SYS_ERR(ERROR_NEED_RESOURCE););
		
	protocol_store16(msg, 0);
	protocol_store8(msg, get_sequeue_num());
	protocol_store8(msg, 0);
	
	if(assemble_msg_elem(msg, MSG_ELEMENT_AP_ALIVE_REQUEST_TYPE) == FALSE)
	{
		return RAISE_ERROR(ERROR_NEED_RESOURCE, "assemble ap alive request failed!");
	}
	return TRUE;
}
BOOL assemble_sys_maintenance_response(void *arg, TMSG *msg)
{
	MAINTENANCE_OP *maintenance = (MAINTENANCE_OP *)arg;
	if(maintenance == NULL)
		RAISE_SYS_ERR(ERROR_WRONG_ARG);
	CREATE_PROTOCOL_MESSAGE(*msg, 9, RAISE_SYS_ERR(ERROR_NEED_RESOURCE););
	protocol_store16(msg, maintenance->ver);
	protocol_store8(msg, maintenance->seq);
	protocol_store8(msg, 0);
	protocol_store16(msg, maintenance->type);
	protocol_store16(msg, 1);
	protocol_store8(msg, maintenance->value);
	if(assemble_msg_elem(msg, MSG_ELEMENT_SYS_MAINTENANCE_RESPONSE_TYPE) == FALSE)
	{
		return RAISE_ERROR(ERROR_NEED_RESOURCE, "assemble sys maintenance response failed!");
	}
	return TRUE;
}

BOOL assemble_set_radio_response(void *arg, TMSG *msg)
{
	SET_RADIO_PARAMETER *radio = (SET_RADIO_PARAMETER *)arg;
	if(radio == NULL)
		RAISE_SYS_ERR(ERROR_WRONG_ARG);
	CREATE_PROTOCOL_MESSAGE(*msg, 9, RAISE_SYS_ERR(ERROR_NEED_RESOURCE););
	protocol_store16(msg, radio->ver);
	protocol_store8(msg, radio->seq);
	protocol_store8(msg, 0);
	protocol_store16(msg, 1);
	protocol_store16(msg, 1);
	protocol_store8(msg, radio->result);
	if(assemble_msg_elem(msg, MSG_ELEMENT_SET_RADIO_RESPONSE_TYPE) == FALSE)
	{
		return RAISE_ERROR(ERROR_NEED_RESOURCE, "assemble set radio response failed!");
	}
	return TRUE;
}
BOOL assemble_set_sta_state_response(void *arg, TMSG *msg)
{
	SET_STA_STATE *sta = (SET_STA_STATE *)arg;
	CREATE_PROTOCOL_MESSAGE(*msg, 9, RAISE_SYS_ERR(ERROR_NEED_RESOURCE););
	protocol_store16(msg, 0);
	protocol_store8(msg, sta->seq);
	protocol_store8(msg, 0);
	protocol_store16(msg, 1);
	protocol_store16(msg, 1);
	if(sta->ret == TRUE)
		protocol_store8(msg, 0);
	else
		protocol_store8(msg, 1);
	if(assemble_msg_elem(msg, MSG_ELEMENT_SET_STA_STATE_RESPONSE_TYPE) == FALSE)
	{
		return RAISE_ERROR(ERROR_NEED_RESOURCE, "assemble set sta state failed!");
	}
	return TRUE;
}


BOOL assemble_data_tunnel_response(void *arg, TMSG *msg)
{
	DATA_T *data = (DATA_T *)arg;
	if(data == NULL)
		RAISE_SYS_ERR(ERROR_WRONG_ARG);
	CREATE_PROTOCOL_MESSAGE(*msg, 9, RAISE_SYS_ERR(ERROR_NEED_RESOURCE););
	protocol_store16(msg, data->ver);
	protocol_store8(msg, data->seq);
	protocol_store8(msg, 0);
	protocol_store16(msg, data->type);
	protocol_store16(msg, 1);
	protocol_store8(msg, data->result);
	if(assemble_msg_elem(msg, MSG_ELEMENT_SET_DATA_TUNNEL_RESPONSE_TYPE) == FALSE)
	{
		return RAISE_ERROR(ERROR_NEED_RESOURCE, "assemble data tunnel response failed!");
	}
	return TRUE;
}


BOOL assemble_set_wlan_info_response(void *arg, TMSG *msg)
{
	RENEW_WLAN_CONF *wlanconf = (RENEW_WLAN_CONF *)arg;
	if(wlanconf == NULL)
		RAISE_SYS_ERR(ERROR_WRONG_ARG);
	CREATE_PROTOCOL_MESSAGE(*msg, 9, RAISE_SYS_ERR(ERROR_NEED_RESOURCE););
	protocol_store16(msg, wlanconf->ver);
	protocol_store8(msg, wlanconf->seq);
	protocol_store8(msg, 0);
	protocol_store16(msg, wlanconf->type);
	protocol_store16(msg, 1);
	protocol_store8(msg, wlanconf->ret);
	if(assemble_msg_elem(msg, MSG_ELEMENT_SET_WLAN_INFO_RESPONSE_TYPE) == FALSE)
	{
		return RAISE_ERROR(ERROR_NEED_RESOURCE, "assemble set wlan info response failed!");
	}
	return TRUE;
}
/* set radius response */
BOOL assemble_set_radius_response(void *arg, TMSG *msg)
{
	MSG2 *radius_msg = (MSG2 *)arg;
	if(radius_msg == NULL)
		RAISE_SYS_ERR(ERROR_WRONG_ARG);
	if(radius_msg->element == NULL)
		return FALSE;
	CREATE_PROTOCOL_MESSAGE(*msg, 9, RAISE_SYS_ERR(ERROR_NEED_RESOURCE););
	protocol_store16(msg, radius_msg->ver);
	protocol_store8(msg, radius_msg->seq);
	protocol_store8(msg, 0);
	protocol_store16(msg, ((RADIUS_CONF *)(radius_msg->element))->type);
	protocol_store16(msg, 1);
	protocol_store8(msg, radius_msg->ret);
	if(assemble_msg_elem(msg, MSG_ELEMENT_SET_RADIUS_RESPONSE_TYPE) == FALSE)
	{
		return RAISE_ERROR(ERROR_NEED_RESOURCE, "assemble set radius response failed!");
	}
	return TRUE;
}

/* ------------------------------------------------------------------------------------------------------------------ */
char *neighbor_SSID_get()
{
	char *SSID_LIST = NULL;
	int SSID_LIST_len = 0;
	NEIGHBOR_AP_INFO *str = NULL;
	char ESSID_tmp[512];
	printf_d("---------------neighbor_apinfo_list-------------\n");
	pthread_mutex_lock(&neighbor_apinfo_list_mutex);
	for(str = neighbor_apinfo_list; str != NULL; str = str->next)
	{
		if(str->ESSID != NULL)
		{
			if(SSID_LIST == NULL)
			{
				SSID_LIST_len = 0;
				SSID_LIST_len += strlen(str->ESSID)+3;
				SSID_LIST = (char *)malloc(SSID_LIST_len);
				//CREATE_STRING_ERR(SSID_LIST,SSID_LIST_len,MALLOC_ERR_RET(NULL));
				memset(SSID_LIST, 0, SSID_LIST_len);
				memset(ESSID_tmp, 0, 512);
				sprintf(ESSID_tmp, "<%s>", str->ESSID);
				strcpy(SSID_LIST, ESSID_tmp);	
			}
			else
			{
				SSID_LIST_len += strlen(str->ESSID)+3;
				SSID_LIST = (char *)realloc(SSID_LIST, SSID_LIST_len);
				memset(ESSID_tmp, 0, 512);
				sprintf(ESSID_tmp, "<%s>", str->ESSID);
				strcat(SSID_LIST, ESSID_tmp);
			}
		}
	}
	pthread_mutex_unlock(&neighbor_apinfo_list_mutex);
	return SSID_LIST;
}

char *rate_get(int mode, int mask)
{
	int i;
	char bit = 0;
	int rate_string_len = 0;
	char *rate = NULL;
	char support_rate_g[12][4]={"1","2","5.5","11","6","9","12","18","24","36","48","54"}; 
	char support_rate_b[4][4]={"1","2","5.5","11"};//4
	char support_rate_a[8][3]={"6","9","12","18","24","36","48","54"};
	if(mode == IEEE80211_PARAM_MODE_11b)
	{
		
		for(i = 0; i < 4; i++)
		{
			bit = mask&0x01;
			if(bit == 1)
			{
				if(rate == NULL)
				{
					rate_string_len = strlen(support_rate_b[i])+1;
					rate = malloc(rate_string_len);
					memset(rate, 0, rate_string_len);
					strcat(rate, support_rate_b[i]);
				}
				else
				{
					rate_string_len += strlen(support_rate_b[i]) + 2;
					rate = (char *)realloc(rate, rate_string_len);
					strcat(rate, ",");
					strcat(rate, support_rate_b[i]);
				}
			}
			mask = mask >> 1;
		}
		return rate;
	}
	else if(mode == IEEE80211_PARAM_MODE_11a)
	{
		
		for(i = 0; i < 8; i++)
		{
			bit = mask&0x01;
			if(bit == 1)
			{
				if(rate == NULL)
				{
					rate_string_len = strlen(support_rate_a[i])+1;
					rate = malloc(rate_string_len);
					memset(rate, 0, rate_string_len);
					strcat(rate, support_rate_a[i]);
				}
				else
				{
					rate_string_len += strlen(support_rate_a[i]) + 2;
					rate = (char *)realloc(rate, rate_string_len);
					strcat(rate, ",");
					strcat(rate, support_rate_a[i]);
				}
			}
			mask = mask >> 1;
		}
		return rate;
	}
	else if(mode == IEEE80211_PARAM_MODE_11g)
	{
		
		for(i = 0; i < 12; i++)
		{
			bit = mask&0x01;
			if(bit == 1)
			{
				if(rate == NULL)
				{
					rate_string_len = strlen(support_rate_g[i])+1;
					rate = malloc(rate_string_len);
					memset(rate, 0, rate_string_len);
					strcat(rate, support_rate_g[i]);
				}
				else
				{
					rate_string_len += strlen(support_rate_g[i]) + 2;
					rate = (char *)realloc(rate, rate_string_len);
					strcat(rate, ",");
					strcat(rate, support_rate_g[i]);
				}
			}
			mask = mask >> 1;
		}
		return rate;
	}
	return rate;
}

BOOL rate_set(int count, RADIO_INFO *radio)
{
	int i, m;
	unsigned char rate;
	unsigned int ratemask = 0;
	unsigned short  *ptr = NULL;
	unsigned int basic_rate_mask = 0, basic_rate_offset = 0,basic_rate_offset_skip = 0;
	unsigned short support_rate_g[12]={10,20,55,110,60,90,120,180,240,360,480,540}; 
	unsigned short support_rate_b[4]={10,20,55,110};//4
	unsigned short support_rate_a[8]={60,90,120,180,240,360,480,540};
	
	rate = radio->rate[0];
	for(i=1;i<count;i++)
	{	
		if(rate > radio->rate[i]) 
		{
			rate = radio->rate[i]; 	//find the smallest one rate  as a basic rate
		}
	}
	
	if(gRadioInfoValue[radio->radio_id-1].radiotype & IEEE80211_PARAM_MODE_11g){
		ptr=support_rate_g;
		basic_rate_offset_skip = 12; //12bits
	//	basic_rate_mask_all = 0x015F;  //	{ *,   24 ,   *,      12，*，6，11，5.5，2，1}
		radio->radio_type = gRadioInfoValue[radio->radio_id].radiotype;
	}
	else if(gRadioInfoValue[radio->radio_id-1].radiotype & IEEE80211_PARAM_MODE_11b){
		ptr=support_rate_b;
		basic_rate_offset_skip = 4; //4bits
	//	basic_rate_mask_all = 0xF; //{11，5.5，2，1}
		radio->radio_type = gRadioInfoValue[radio->radio_id-1].radiotype;
	}
	else if (gRadioInfoValue[radio->radio_id-1].radiotype & IEEE80211_PARAM_MODE_11a){
		ptr=support_rate_a;
		basic_rate_offset_skip = 8;//8bits
	//	basic_rate_mask_all = 0x015;//{*,24,*,12,*,6}
		radio->radio_type = gRadioInfoValue[radio->radio_id-1].radiotype;
	}
	for(i=0;i<count;i++)
	{
		m = 0;
		printf_d("%d,",radio->rate[i]);
		while(m<basic_rate_offset_skip)
		{
			if(radio->rate[i] == ptr[m]) //found
			{
				ratemask = ratemask | (0x01 << m);	//each rate offset
				if(radio->f[i] == 1)
					basic_rate_mask = basic_rate_mask | (0x01 << m);   //(basic_rate_offset_skip + m))
			}		
			if(rate == ptr[m])
				basic_rate_offset = m;  			 //basicoffset,the smallest one rate  as a basic rate	
			m++;
		}
	}
	printf_d("\n");
	basic_rate_mask = ratemask & basic_rate_mask;/*if the rate don't support,then basic rate invalid*/
	if(basic_rate_mask != 0)
	{ //basic rateset mask
		ratemask = ratemask | (basic_rate_mask << basic_rate_offset_skip);
	}
	else 
	{// the smalleast rate can be as  a basic rate 
		basic_rate_mask = (basic_rate_mask |0x01) << (basic_rate_offset + basic_rate_offset_skip);  
		ratemask = ratemask | basic_rate_mask;
	}
	printf_d("parse ratemask:0x%08X,basic_rate_offset:%d basic_rate_offset_skip:%d\n",ratemask,basic_rate_offset,basic_rate_offset_skip);

	printf_d("radio %d,rate mask:0x%08X\n", radio->radio_id-1, ratemask);
	radio->rate_mask = ratemask;
	return TRUE;
}

char *mcs_get(u32 mcs_mask)
{
	u32 mask = mcs_mask;
	char *mcs = NULL;
	char tmp[8];
	int i = 0, len = 0;
	int mcs_string_len = 0;
	while(mask != 0)
	{
		if((mask & 0x01) == 1)
		{
			if(i <= 9)
				mcs_string_len = 7;
			else
				mcs_string_len = 8;
			len += mcs_string_len;
			if(mcs == NULL)
			{
				mcs = malloc(mcs_string_len);
				memset(mcs, 0, mcs_string_len);
				sprintf(tmp,"MCS %d", i);
				strcat(mcs, tmp);
			}
			else
			{		
				mcs = (char *)realloc(mcs, len);
				sprintf(tmp,",MCS %d", i);
				strcat(mcs, tmp);
			}
		}
		mask = mask>>1;
		i++;
	}
	return mcs;
}

BOOL GetInterfaceStatus(char *ifname, unsigned char *status)
{
	int sockfd;
	struct ifreq ifr;
	
	*status = 0;
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	strncpy(ifr.ifr_name,ifname, sizeof(ifr.ifr_name));
	if(ioctl(sockfd, SIOCGIFINDEX, &ifr) == -1)
	{//bind to a interface 
		APVLog("%s is not exist !\n", ifname);
		printf_d("%s is not exist !\n", ifname);
		close(sockfd);
		*status = 3;
		return TRUE;
	}
	
	if (ioctl(sockfd, SIOCGIFFLAGS, &ifr) == -1)
	{
		APVLog("SIOCGIFFLAGS error\n");
		printf_d("SIOCGIFFLAGS error\n");
		close(sockfd);
		*status = 3;
		return TRUE;
	}
	if(ifr.ifr_flags & IFF_UP)
	{
		*status = 1;
	}
	else
	{
		*status = 2;
	}

	close(sockfd);
	return TRUE;
}

/* ------------------------------------------------------------------------------------------------------------------ */
/* get information to report */
BOOL GetCpuUseRatio(u32 *cpu_use_ratio)/* 实际值*10000 */
{
	char temp_str[128];
	char str[24];
	
	memset(temp_str, 0, 128);
	sprintf(temp_str, "cat /tmp/cpu_ratio |awk 'NR==2 {print $0}'");
	popen_fgets(temp_str, str, sizeof(str));	
	*cpu_use_ratio = atoi(str);
	printf_d("cpu use ratio:%d\n", *cpu_use_ratio);

	return TRUE;
}

BOOL GetMemUseRatio(u32 *memUseRatio)/* 实际值*10000 */
{
	char temp_str[100];
	char str[20];
	int mem_Total = 0;
	int memFree = 0;
	
	memset(temp_str, 0, 100);
	//sprintf(temp_str, "/usr/sbin/get_hardinfo | awk -F \"[:M]\" '/mem_size/ {print $2}'");
	sprintf(temp_str, "cat /proc/meminfo | awk -F \" \" '/MemTotal/ {print $2}'");
	popen_fgets(temp_str, str, sizeof(str));	
	mem_Total = atoi(str);
	printf_d("memTotal: %s len:%d memTotal:%d\n", str, (int)strlen(str), mem_Total);

	memset(temp_str, 0, 100);
	sprintf(temp_str, "cat /proc/meminfo | awk -F \" \" '/MemFree/ {print $2}'");
	popen_fgets(temp_str, str, sizeof(str));	
	memFree = atoi(str);
	printf_d("memFree mode: %s len:%d memFree:%d\n", str, (int)strlen(str), memFree);
	if(mem_Total == 0)  //pei add 0416
	{
		mem_Total = 113308;
	}
	*memUseRatio = 100-(memFree*100)/mem_Total;
	printf_d("memTotal:%dMB memUseRatio:%d%%\n", mem_Total, *memUseRatio);
	*memUseRatio = *memUseRatio*100;
	return TRUE;
}

#if 1
void * GetApScanningInfo(void *arg)
{
	NEIGHBOR_AP_INFO *neighborApInfo = NULL;
	NEIGHBOR_AP_INFO *head = NULL;
	NEIGHBOR_AP_INFO *tail = NULL;
	NEIGHBOR_AP_INFO *p = NULL;
	NEIGHBOR_AP_INFO *s = NULL;
	char ifname[16];
	char cmdbuf[128];
	int radiocount = 1;
	WLAN_INFO *ptr = NULL;
	char wifi0mode[10]={0};
	char wifi1mode[10]={0};
	BOOL isWifi1 = FALSE;  /* for wifi1 */
	BOOL isFirstTime = TRUE;
	int  scan_cnt = 0;
	
	if(arg!=NULL)
	{
		sleep((int)arg);
	}
	
	if(Check_Interface_State("wifi1")==0)
	{
		isWifi1 = TRUE;
		radiocount += 1;
	}
	SYSTEM("killall -9 iwlist_scan > /dev/null 2>&1");
	pthread_mutex_lock(&wlan_mutex);
	pthread_mutex_lock(&sta_mutex);
	if(0 != Check_Interface_State("athscanwifi0"))
	{
		for(ptr = wlan_list; ptr != NULL; ptr = ptr->next){
			if(ptr->rid == 1)
			{
				sprintf(ifname,"ath.%d-%d", ptr->rid-1, ptr->wid);
				sprintf(cmdbuf,"ifconfig %s down > /dev/null", ifname);
				SYSTEM(cmdbuf);
			}
		}
		strcpy(ifname, "athscanwifi0");
		sprintf(cmdbuf,"wlanconfig %s create wlandev wifi0 wlanmode ap >/dev/null",ifname);
		SYSTEM(cmdbuf);
		memset(cmdbuf, 0, 128);
		sprintf(cmdbuf, "/usr/sbin/iwpriv athscanwifi0 hide_ssid 1");
		SYSTEM(cmdbuf);
		set_mode2(0);
		//set_radiotype_cmd(0,0,gRadioInfoValue[0].radiotype);
		for(ptr = wlan_list ; ptr != NULL ; ptr = ptr->next){
			if(ptr->rid == 1)
			{
				sprintf(ifname,"ath.%d-%d", ptr->rid-1, ptr->wid);
				sprintf(cmdbuf,"/sbin/ifconfig %s up > /dev/null", ifname);
				SYSTEM(cmdbuf);
			}
		}
		SYSTEM("/sbin/ifconfig athscanwifi0 up >/dev/null &");

	}

	if(isWifi1)
	{	
		if(0 != Check_Interface_State("athscanwifi1"))
		{
			for(ptr = wlan_list ; ptr != NULL ; ptr = ptr->next)
			{
				if(ptr->rid == 2)
				{
					sprintf(ifname,"ath.%d-%d", ptr->rid-1, ptr->wid);
					sprintf(cmdbuf,"ifconfig %s down > /dev/null", ifname);
					SYSTEM(cmdbuf);
				}
			}
			strcpy(ifname, "athscanwifi1");
			sprintf(cmdbuf,"wlanconfig %s create wlandev wifi1 wlanmode ap >/dev/null",ifname);
			sleep(2);
			APVLog("Create athscanwifi1,sleep 2\n");
			printf_d("Create athscanwifi1,sleep 2\n");
			SYSTEM(cmdbuf);
			memset(cmdbuf, 0, 128);
			sprintf(cmdbuf, "/usr/sbin/iwpriv athscanwifi1 hide_ssid 1");
			SYSTEM(cmdbuf);
			set_mode2(1);
			//set_radiotype_cmd(1,1,gRadioInfoValue[1].radiotype);
			for(ptr = wlan_list ; ptr != NULL ; ptr = ptr->next)
			{
				if(ptr->rid == 2) 
				{
					sprintf(ifname,"ath.%d-%d", ptr->rid-1, ptr->wid);
					sprintf(cmdbuf,"ifconfig %s up > /dev/null", ifname);
					SYSTEM(cmdbuf);
				}
			}
			SYSTEM("ifconfig athscanwifi1 up >/dev/null &");
		}
	}
	pthread_mutex_unlock(&sta_mutex);
	pthread_mutex_unlock(&wlan_mutex);
	memset(cmdbuf, 0, 128);
	if((gRadioInfoValue[0].radiotype&0x02)!=0)	//IEEE80211_PARAM_MODE_11a 0x02
		memcpy(wifi0mode,"11a",sizeof("11a"));
	else
		memcpy(wifi0mode,"11bg",sizeof("11bg"));
	if(isWifi1)
	{
		
		if((gRadioInfoValue[1].radiotype&0x02)!=0)	//IEEE80211_PARAM_MODE_11a 0x02
			memcpy(wifi1mode,"11a",sizeof("11a"));
		else
			memcpy(wifi1mode,"11bg",sizeof("11bg"));
		sprintf(cmdbuf, "/usr/sbin/iwlist_scan -m %s -c %s -I %d -i %d -n 2 > /dev/null 2>&1 &",wifi0mode,wifi1mode,IWLIST_SCAN_SHORT_INTERVAL,10);
	}
	else 
		sprintf(cmdbuf, "/usr/sbin/iwlist_scan -m %s  -I %d -i %d  > /dev/null 2>&1 &",wifi0mode,IWLIST_SCAN_SHORT_INTERVAL,10);
	SYSTEM(cmdbuf);

	/* begin to gather rogue AP info */
	u32 neighborApCount = 0;
	u8 buf[24*1024];
	int i = 0;
	
	/* get iwlist ap info from other process */
	while(1)
	{
		if(scan_cnt > 2){
			SYSTEM("killall -9 iwlist_scan");
			if(isWifi1)
			{
				sprintf(cmdbuf, "/usr/sbin/iwlist_scan -m %s -c %s -I %d -i %d -n 2 > /dev/null 2>&1 &",wifi0mode,wifi1mode,IWLIST_SCAN_LONG_INTERVAL,10);
			}
			else 
				sprintf(cmdbuf, "/usr/sbin/iwlist_scan -m %s  -I %d -i %d  > /dev/null 2>&1 &",wifi0mode,IWLIST_SCAN_LONG_INTERVAL,10);
			SYSTEM(cmdbuf);
			scan_cnt = -1;//stop increase scan_cnt;
			printf_d("%s,%d,restart iwlist_scan\n",__func__,__LINE__);
			APVLog("%s,%d,restart iwlist_scan\n",__func__,__LINE__);
		}
		if(scan_cnt != -1){
			scan_cnt++;
			printf_d("%s,%d,scan_cnt:%d\n",__func__,__LINE__,scan_cnt);
			APVLog("%s,%d,scan_cnt:%d\n",__func__,__LINE__,scan_cnt);
		}
		if(ReportEnable == DISABLED)
		{
			APVLog("disable ap scanning, exit thread\n");
			printf_d("disable ap scanning, exit thread\n");
			goto exit_thread;
		}
		int sockfd = -1;
		int readBytes = 0;
		int type = 0;
		struct timeval timeout;
		fd_set readfds;
		TMSG msgPtr;
		struct sockaddr_in addr_from;
		struct sockaddr_in serv_addr;
		
		sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
		if(sockfd < 0)
		{
			APVLog("Failed to create socket in ap scanning, continue\n");
			continue;
		}

		memset(&serv_addr, 0, sizeof(serv_addr));
		serv_addr.sin_family = AF_INET;
		serv_addr.sin_port = htons(5248);
		serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

		if(sendto(sockfd, &buf[0], 1, 0,(struct sockaddr *)&serv_addr, sizeof(struct sockaddr_in)) == -1)
		{
			APVLog("send error\n");
			close(sockfd);
			continue;
		}
		printf_d("send request ok....................\n");
		
		FD_ZERO(&readfds);
		FD_SET(sockfd, &readfds);
		timeout.tv_sec = 15;
		timeout.tv_usec = 0;
		int ret = select(sockfd+1, &readfds, NULL, NULL, &timeout);
		if(ret < 0)
		{
			close(sockfd);
			printf_d("select error\n");
		}
		else if(ret == 0)
		{
			close(sockfd);
			printf_d("receive time out\n");
		}
		else
		{
			if(network_receive_unsafe(sockfd, (char *)buf, 24*1024, 0, (SOCKADDR_STORAGE *)&addr_from, &readBytes) != FALSE) 
			{
				/*if (CWErrorGetLastErrorCode() == CW_ERROR_INTERRUPTED)
				{
					close(sockfd);
					continue;
				}*/
				printf_d("receive failed\n");
			}
			close(sockfd);
			printf_d("receive a msg....................\n");
			
			msgPtr.msg = buf;
			msgPtr.offset = 0;
			type= protocol_retrieve8(&msgPtr);
			neighborApCount = protocol_retrieve8(&msgPtr);
			printf_d("type:%d  All neighborapcount:%d\n", type, neighborApCount);
			if(neighborApCount != 0)
			{
				CREATE_ARRAY_ERR(neighborApInfo, neighborApCount, NEIGHBOR_AP_INFO, {RAISE_ERROR(ERROR_OUT_OF_MEMORY, NULL);printf_d("---6---goto exit_thread\n");/*goto exit_thread;*/});// {CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL);printf("---6---goto exit_thread\n");goto exit_thread;});
				for(i=0;i<neighborApCount;i++)
				{
					memset(&neighborApInfo[i], 0, sizeof(NEIGHBOR_AP_INFO));
					
					void *ptr = NULL;
					ptr =  (unsigned char *)protocol_retrieve_str(&msgPtr,6);
					COPY_MEMORY(neighborApInfo[i].BSSID, ptr, 6);
					FREE_OBJ(ptr);
					neighborApInfo[i].Channel = protocol_retrieve8(&msgPtr);
					neighborApInfo[i].RSSI = protocol_retrieve8(&msgPtr);
					neighborApInfo[i].NOISE = protocol_retrieve8(&msgPtr);
					neighborApInfo[i].BEACON_INT = protocol_retrieve8(&msgPtr);
					neighborApInfo[i].Opstatus = NEW;
					msgPtr.offset +=1;
					neighborApInfo[i].EssidLen = protocol_retrieve8(&msgPtr);

					CREATE_STRING_ERR(neighborApInfo[i].ESSID, neighborApInfo[i].EssidLen, {RAISE_ERROR(ERROR_OUT_OF_MEMORY, NULL);APVLog("create memory error\n");});
					memset(neighborApInfo[i].ESSID, 0 , neighborApInfo[i].EssidLen+1);

					ptr = NULL;
					ptr =  protocol_retrieve_str(&msgPtr, neighborApInfo[i].EssidLen);
					COPY_MEMORY(neighborApInfo[i].ESSID, ptr, neighborApInfo[i].EssidLen);
					FREE_OBJ(ptr);
					
					neighborApInfo[i].IeLen = protocol_retrieve8(&msgPtr);
					CREATE_STRING_ERR(neighborApInfo[i].IEs_INFO, neighborApInfo[i].IeLen, {RAISE_ERROR(ERROR_OUT_OF_MEMORY, NULL);APVLog("create memory error\n");});
					memset(neighborApInfo[i].IEs_INFO, 0 , neighborApInfo[i].IeLen+1);

					ptr = NULL;
					ptr =  protocol_retrieve_str(&msgPtr, neighborApInfo[i].IeLen);
					COPY_MEMORY(neighborApInfo[i].IEs_INFO, ptr, neighborApInfo[i].IeLen);
					FREE_OBJ(ptr);
					
					neighborApInfo[i].Rate = protocol_retrieve16(&msgPtr);
					neighborApInfo[i].CapabilityInfo = protocol_retrieve16(&msgPtr);
					neighborApInfo[i].Radio_id = protocol_retrieve8(&msgPtr);
					neighborApInfo[i].next = NULL;
				}
			}

			printf_d("Begin to set link list for saving neighbor AP info!\n");
			//if(isFirstTime||(head == NULL))
			pthread_mutex_lock(&neighbor_apinfo_list_mutex);
			p = neighbor_apinfo_list;
			if(neighbor_apinfo_list != NULL)
			{
				for(s = p->next; s != NULL;s = s->next)
				{
					FREE_OBJ(p->ESSID);
					FREE_OBJ(p->IEs_INFO);
					FREE_OBJ(p);
					p = s;
				}
				FREE_OBJ(p->ESSID);
				FREE_OBJ(p->IEs_INFO);
				FREE_OBJ(p);
				neighbor_apinfo_list = NULL;
			}
			s = neighbor_apinfo_list;
			if(s == NULL)
			{
				for(i=0;i<neighborApCount;i++)
				{
					s = NULL;
					CREATE_OBJECT_ERR(s, NEIGHBOR_AP_INFO, {RAISE_ERROR(ERROR_OUT_OF_MEMORY, NULL);APVLog("create memory error\n");goto exit_thread;});
					memset(s, 0, sizeof(NEIGHBOR_AP_INFO));
					s->ESSID = NULL;
					s->IEs_INFO = NULL;
					s->next = NULL;
					memcpy(s, &neighborApInfo[i], sizeof(NEIGHBOR_AP_INFO));
					CREATE_STRING_ERR(s->ESSID, neighborApInfo[i].EssidLen, {RAISE_ERROR(ERROR_OUT_OF_MEMORY, NULL);APVLog("create memory error\n");goto exit_thread;});
					CREATE_STRING_ERR(s->IEs_INFO, neighborApInfo[i].IeLen, {RAISE_ERROR(ERROR_OUT_OF_MEMORY, NULL);APVLog("create memory error\n");goto exit_thread;});
					memset(s->ESSID, 0, neighborApInfo[i].EssidLen+1);
					memset(s->IEs_INFO, 0, neighborApInfo[i].IeLen+1);
					memcpy(s->ESSID, neighborApInfo[i].ESSID, neighborApInfo[i].EssidLen);
					memcpy(s->IEs_INFO, neighborApInfo[i].IEs_INFO, neighborApInfo[i].IeLen);
					s->Radio_id 		=  neighborApInfo[i].Radio_id;
					if(i == 0)
					{
						head = s;
						neighbor_apinfo_list = head;
						p = s;
					}
					else
					{
						p->next = s;
						p = s;
					}
				}
				tail = s;
			}
			printf_d("Free the neighborApInfo[]!\n");
			for(i = 0; i < neighborApCount; i++)
			{
				FREE_OBJ(neighborApInfo[i].ESSID);
				FREE_OBJ(neighborApInfo[i].IEs_INFO);
			}
			FREE_OBJ(neighborApInfo);
		}
		if(ReportEnable == DISABLED)
		{
			APVLog("disable ap scanning, exit thread\n");
			printf_d("disable ap scanning, exit thread\n");
			goto exit_thread;
		}
		isFirstTime = FALSE;
		pthread_mutex_unlock(&neighbor_apinfo_list_mutex);
		if (scan_cnt == -1)
			sleep(GET_AP_SCANNING_INFO_INTERVAL);
		else
			sleep(ap_report_interval);
	}
exit_thread:
	pthread_mutex_unlock(&neighbor_apinfo_list_mutex);
	p = head;
	while(p!=NULL)
	{
		head = p->next;
		FREE_OBJ(p->ESSID);
		FREE_OBJ(p->IEs_INFO);
		p->next = NULL;
		FREE_OBJ(p);
		p = head;
	}
	neighbor_apinfo_list = NULL;
	
	APVLog("Exit the thread for ap scan!\n");
	printf_d("Exit the thread!\n");
	if(0 == Check_Interface_State("athscanwifi0"))
	{
		sprintf(cmdbuf, "ifconfig %s down > /dev/null &", "athscanwifi0");
		SYSTEM(cmdbuf);
		sprintf(cmdbuf, "wlanconfig %s destroy > /dev/null &", "athscanwifi0");
		SYSTEM(cmdbuf);
	}
	if(isWifi1)
	{
		if(0 == Check_Interface_State("athscanwifi1"))
		{
			sprintf(cmdbuf, "ifconfig %s down > /dev/null &", "athscanwifi1");
			SYSTEM(cmdbuf);
			sprintf(cmdbuf, "wlanconfig %s destroy > /dev/null &", "athscanwifi1");
			SYSTEM(cmdbuf);
		}
	}
	pthread_exit((void *) 0);
	return NULL;
}
#endif

BOOL GetDisassocErrors(char *ifname, AP_STATE_COUNT_INFO *state_count_info)
{
	struct ieee80211_stats *stats=NULL;
	struct ifreq ifr;
	int s;
	
	s = socket(AF_INET, SOCK_DGRAM, 0);
	if (s < 0)
	{
		APVLog("create sock to get 80211stats error!\n");
		printf_d("create sock to get 80211stats error!\n");
		return FALSE;
	}
	
	strncpy(ifr.ifr_name, ifname, sizeof (ifr.ifr_name));

	stats = malloc(sizeof(struct ieee80211_stats)+ 2* sizeof(struct ieee80211_mac_stats));
	memset(stats,0,sizeof(struct ieee80211_stats)+ 2* sizeof(struct ieee80211_mac_stats));

	ifr.ifr_data = (caddr_t) stats;
	pthread_mutex_lock(&siocg_80211_stats);
	#if 1
	if (ioctl(s, SIOCG80211STATS, &ifr) < 0)
	{
		APVLog("ioctl to get 80211stats error!\n");
		printf_d("ioctl to get 80211stats error!\n");
		close(s);
		FREE_OBJ(stats);
		pthread_mutex_unlock(&siocg_80211_stats);
		return FALSE;
	}
	#endif
	/*state_count_info->connect_retry_count += stats->is_rx_assoc_bss + stats->is_rx_assoc_notauth + stats->is_rx_assoc_capmismatch + stats->is_rx_assoc_norate 
					+ stats->is_rx_assoc_badwpaie ;//拒绝的重关联次数*/
	state_count_info->connect_retry_count += stats->is_rx_reassoc + stats->is_rx_reassoc_bss;//拒绝的重关联次数
	state_count_info->connect_undistinguish_retry_count += stats->is_rx_bad_assoc;//因无效而导致用户链路验证失败的次数
	state_count_info->disconnect_unnormal_count +=stats->is_deauth_expire;//stats->is_rx_deauth去掉，原因是用户主动下线会发此帧，不属于异常因异常情况而导致解链路验证的次数;stats->is_disassoc_ioctl去掉，原因是踢用户下线会发此帧，不属于异常情况而导致解链路验证的次数
	state_count_info->connect_poor_resource_failed_count += stats->is_rx_ap_too_many_sta;//由于接入点资源有限而关联失败的次数
	state_count_info->connect_no_support_rate_failed_count += stats->is_rx_assoc_norate;//终端不支持基本速率级所要求的全部速率而关联失败的总次数
#ifdef AR11N
	state_count_info->connect_low_rssi_failed_count = 0;
#else
	state_count_info->connect_low_rssi_failed_count += stats->is_refuse_LowRssi;//因RSSI过低而关联失败的总次数
#endif
	state_count_info->connect_failed_count += stats->is_tx_auth_assoc_fail;

	state_count_info->connect_count += stats->is_assoc_success + state_count_info->connect_failed_count + state_count_info->connect_retry_count;
//	printstats(stdout, &stats);
	close(s);
	FREE_OBJ(stats);
	pthread_mutex_unlock(&siocg_80211_stats);
	return TRUE;
}



static inline int procnetdev_version(char *buf)
{
	if (strstr(buf, "compressed"))
		return 2;
	if (strstr(buf, "bytes"))
		return 1;
	return 0;
}

static char *get_name(char *name, char *p)
{
	/* Extract <name> from nul-terminated p where p matches
	   <name>: after leading whitespace.
	   If match is not made, set name empty and return unchanged p */
	int namestart=0, nameend=0;
	while (is_space(p[namestart]))
		namestart++;
	nameend=namestart;
	while (p[nameend] && p[nameend]!=':' && !is_space(p[nameend]))
		nameend++;
	if (p[nameend]==':') {
		if ((nameend-namestart)<16) {
			memcpy(name,&p[namestart],nameend-namestart);
			name[nameend-namestart]='\0';
			p=&p[nameend];
		} else {
			/* Interface name too large */
			name[0]='\0';
		}
	} else {
		/* trailing ':' not found - return empty */
		name[0]='\0';
	}
	return p + 1;
}


#if INT_MAX == LONG_MAX   /*in cavium INT != LONG*/
static const char * const ss_fmt[] = {
	"%n%llu%u%u%u%u%n%n%n%llu%u%u%u%u%u",
	"%llu%llu%u%u%u%u%n%n%llu%llu%u%u%u%u%u",
	"%llu%llu%u%u%u%u%u%u%llu%llu%u%u%u%u%u%u%u%u%u%u%u%u%u%llu%u%llu%u%u%u%u%u"
};
#else
static const char * const ss_fmt[] = {
	"%n%llu%lu%lu%lu%lu%n%n%n%llu%lu%lu%lu%lu%lu",
	"%llu%llu%lu%lu%lu%lu%n%n%llu%llu%lu%lu%lu%lu%lu",
	"%llu%llu%lu%lu%lu%lu%lu%lu%llu%llu%lu%lu%lu%lu%lu%lu%lu%lu%lu%lu%lu%lu%lu%llu%lu%llu%lu%lu%lu%lu%lu"
};
#endif


static void get_dev_fields(char *bp, struct user_net_device_stats *wlan_stats, int procnetdev_vsn)
{
	memset(wlan_stats, 0, sizeof(struct user_net_device_stats));

	sscanf(bp, ss_fmt[procnetdev_vsn],
		&wlan_stats->rx_bytes, /* missing for 0 */
		&wlan_stats->rx_packets,
		&wlan_stats->rx_errors,
		&wlan_stats->rx_dropped,
		&wlan_stats->rx_fifo_errors,
		&wlan_stats->rx_frame_errors,
		&wlan_stats->rx_compressed, /* missing for <= 1 */
		&wlan_stats->rx_multicast, /* missing for <= 1 */
		&wlan_stats->tx_bytes, /* missing for 0 */
		&wlan_stats->tx_packets,
		&wlan_stats->tx_errors,
		&wlan_stats->tx_dropped,
		&wlan_stats->tx_fifo_errors,
		&wlan_stats->collisions,
		&wlan_stats->tx_carrier_errors,
		&wlan_stats->tx_compressed /* missing for <= 1 */
		,&wlan_stats->rx_multicast,  /* pei add for eth 090624 */
		&wlan_stats->rx_broadcast,
		&wlan_stats->rx_unicast,
		&wlan_stats->tx_multicast,
		&wlan_stats->tx_broadcast,
		&wlan_stats->tx_unicast,
		&wlan_stats->rx_multicast_bytes,
		&wlan_stats->rx_unicast_bytes,	
		&wlan_stats->tx_multicast_bytes,				
		&wlan_stats->tx_unicast_bytes,
		&wlan_stats->rx_retry,
		&wlan_stats->rx_retry_bytes,   
		&wlan_stats->tx_retry,
		&wlan_stats->tx_retry_bytes,
		&wlan_stats->rx_error_frames
		   );

	if (procnetdev_vsn <= 1) {
		if (procnetdev_vsn == 0) {
			wlan_stats->rx_bytes = 0;
			wlan_stats->tx_bytes = 0;
		}
		wlan_stats->rx_multicast = 0;
		wlan_stats->rx_compressed = 0;
		wlan_stats->tx_compressed = 0;
	}
}

static int get_eth_fields(char *ifname, AP_ETH_STATE_COUNT_INFO *ethPackageInfo)
{
	FILE *fh;
	char buf[512];
	struct user_net_device_stats wlan_stats;
	int procnetdev_vsn;
	pthread_mutex_lock(&siocg_80211_stats);
	fh = fopen(_PATH_PROCNET_DEV, "r");
	if (!fh)
	{
		APVLog("warning: cannot open %s, limiting output\n", _PATH_PROCNET_DEV);
		printf_d("warning: cannot open %s, limiting output\n", _PATH_PROCNET_DEV);
		pthread_mutex_unlock(&siocg_80211_stats);
		return 1;
	}
	fgets(buf, sizeof buf, fh);	/* eat line */
	fgets(buf, sizeof buf, fh);

	procnetdev_vsn = procnetdev_version(buf);

	while(fgets(buf, sizeof buf, fh))
	{
		char *s, name[128];

		s = get_name(name, buf);
		if(strstr(buf, ifname))
		{
			if(strncmp(name, ifname, strlen(name)) == 0)
			{
				get_dev_fields(s, &wlan_stats, procnetdev_vsn);
				ethPackageInfo->tx_unicast_packet = wlan_stats.tx_unicast;
				ethPackageInfo->tx_ununicast_packet = wlan_stats.tx_broadcast + wlan_stats.tx_multicast;
				ethPackageInfo->tx_bytes = wlan_stats.tx_bytes;
				ethPackageInfo->tx_drop_packet = wlan_stats.tx_dropped;
				ethPackageInfo->tx_error_packet = wlan_stats.tx_errors;
				ethPackageInfo->rx_unicast_packet = wlan_stats.rx_unicast;
				ethPackageInfo->rx_multicast = wlan_stats.rx_multicast;
				ethPackageInfo->rx_broadcast = wlan_stats.rx_broadcast;
				ethPackageInfo->tx_multicast = wlan_stats.tx_multicast;	
				ethPackageInfo->tx_broadcast = wlan_stats.tx_broadcast;
				
				ethPackageInfo->rx_ununicast_packet = wlan_stats.rx_broadcast + wlan_stats.rx_multicast;
				ethPackageInfo->rx_bytes = wlan_stats.rx_bytes;
				ethPackageInfo->rx_drop_packet = wlan_stats.rx_dropped;
				ethPackageInfo->rx_error_packet = wlan_stats.rx_errors;
			}
		}
	}
	fclose(fh);
	pthread_mutex_unlock(&siocg_80211_stats);
	return 0;
}

BOOL get_sta_throughput_info(staThroughputsendtoAc *staThroughputsendtoAcinfo)
{
	int s;
	struct iwreq iwr;
	struct ieee80211req_sta_stats stats;
	const struct ieee80211_nodestats *ns = &stats.is_stats;
	char ifname[IFNAMSIZ];
	STA_INFO *ptr = NULL;
	STA_INFO *oldptr = NULL;
	unsigned char radioId = 0, wlanId = 0;
	int flags = 0;
	
	s = socket(AF_INET, SOCK_DGRAM, 0);
	if (s < 0)
	{
		APVLog("create sock to get_stations error!\n");
		printf_d("create sock to get_stations error!\n");
		close(s);
		return FALSE;
	}
	pthread_mutex_lock(&sta_mutex);
	for(ptr = sta_list;ptr != NULL; oldptr = ptr,ptr = ptr->next)
	{
		if((macAddrCmp(ptr->MAC, staThroughputsendtoAcinfo->sta_mac) == 1))
		{
				radioId = ptr->rid-1;
				wlanId = ptr->wid;
				flags = 1;
				break;
		}
	}
	pthread_mutex_unlock(&sta_mutex);
	if(flags == 0)
	{
		close(s);
		return FALSE;
	}
	
	(void) memset(&iwr, 0, sizeof(iwr));
	memset(ifname,0,sizeof(ifname));
	sprintf(ifname, "ath.%d-%d", radioId, wlanId);
	(void) strncpy(iwr.ifr_name, ifname,IFNAMSIZ);
	iwr.u.data.pointer = (void *) &stats;
	iwr.u.data.length = sizeof(stats);

	memcpy(stats.is_u.macaddr, staThroughputsendtoAcinfo->sta_mac, IEEE80211_ADDR_LEN);
	if (ioctl(s, IEEE80211_IOCTL_STA_STATS, &iwr) < 0){
		APVLog("ioctl to get_sta_stats error!\n");
		printf_d("ioctl to get_sta_stats error!\n");
		close(s);
		return FALSE;
	}

	staThroughputsendtoAcinfo->bytes_recv= ns->ns_rx_bytes;
	staThroughputsendtoAcinfo->bytes_send= ns->ns_tx_bytes;
	staThroughputsendtoAcinfo->frames_recv= ns->ns_rx_data;
	staThroughputsendtoAcinfo->frames_send= ns->ns_tx_data;
#if 0
	if(debug_print)
		printf("bytes_recv:%u,  bytes_send:%u,  frames_recv:%llu,  frame_send:%llu\n",
			staThroughputsendtoAcinfo->frames_recv,staThroughputsendtoAcinfo->frames_send,
			staThroughputsendtoAcinfo->bytes_recv,staThroughputsendtoAcinfo->bytes_send);
#endif
	close(s);
	return TRUE;
}

static int get_wlan_throughput_fields(char *wlanname, wlan_stats_info *wlanStatsInfo)	/* 接口统计信息 */
{
	FILE *fh;
	char buf[512];
	struct user_net_device_stats wlan_stats;
	int procnetdev_vsn;
	pthread_mutex_lock(&siocg_80211_stats);
	fh = fopen(_PATH_PROCNET_DEV, "r");
	if (!fh)
	{
		APVLog("warning: cannot open %s, limiting output\n", _PATH_PROCNET_DEV);
		printf_d("warning: cannot open %s, limiting output\n", _PATH_PROCNET_DEV);
		pthread_mutex_unlock(&siocg_80211_stats);
		return 1;
	}
	fgets(buf, sizeof(buf), fh);	/* eat line */
	fgets(buf, sizeof(buf), fh);

	procnetdev_vsn = procnetdev_version(buf);

	while (fgets(buf, sizeof(buf), fh))
	{
		char *s, name[128];
		s = get_name(name, buf);
		if(strstr(buf, wlanname))
		{
			if(strncmp(name, wlanname, strlen(name)) == 0)
			{
				get_dev_fields(s, &wlan_stats, procnetdev_vsn);
				wlanStatsInfo->rx_packets = wlan_stats.rx_packets;
				wlanStatsInfo->tx_packets = wlan_stats.tx_packets;
				wlanStatsInfo->rx_errors = wlan_stats.rx_errors;
				wlanStatsInfo->tx_errors = wlan_stats.tx_errors;
				wlanStatsInfo->rx_bytes = wlan_stats.rx_bytes;
				wlanStatsInfo->tx_bytes = wlan_stats.tx_bytes;
				wlanStatsInfo->rx_dropped= wlan_stats.rx_dropped;
				wlanStatsInfo->tx_dropped= wlan_stats.tx_dropped;				
#ifdef AR11N
				wlanStatsInfo->rx_rate = 150000;  //need to modify
				wlanStatsInfo->tx_rate = 150000;  //need to modify
#else
				wlanStatsInfo->rx_rate = 300000;  //need to modify
				wlanStatsInfo->tx_rate = 300000;  //need to modify
#endif
				wlanStatsInfo->tx_multicast = wlan_stats.tx_multicast;
				wlanStatsInfo->rx_multicast = wlan_stats.rx_multicast;
				wlanStatsInfo->tx_broadcast = wlan_stats.tx_broadcast;
				wlanStatsInfo->rx_broadcast = wlan_stats.rx_broadcast;
				wlanStatsInfo->tx_unicast = wlan_stats.tx_unicast;
				wlanStatsInfo->rx_unicast = wlan_stats.rx_unicast;
				wlanStatsInfo->rx_multicast_bytes = wlan_stats.rx_multicast_bytes;
				wlanStatsInfo->tx_multicast_bytes = wlan_stats.tx_multicast_bytes;
				if(wlan_stats.rx_unicast_bytes != 0)
					wlanStatsInfo->rx_unicast_bytes = wlan_stats.rx_unicast_bytes/1024;
				if(wlan_stats.tx_unicast_bytes != 0)
					wlanStatsInfo->tx_unicast_bytes = wlan_stats.tx_unicast_bytes/1024;
				wlanStatsInfo->rx_retry_bytes = wlan_stats.rx_retry_bytes;
				wlanStatsInfo->tx_retry_bytes = wlan_stats.tx_retry_bytes;
				wlanStatsInfo->rx_retry= wlan_stats.rx_retry;
				wlanStatsInfo->tx_retry= wlan_stats.tx_retry;
				wlanStatsInfo->rx_sum_bytes = wlan_stats.rx_bytes;
				wlanStatsInfo->tx_sum_bytes = wlan_stats.tx_bytes;
				wlanStatsInfo->rx_unicast64_bytes = wlan_stats.rx_unicast_bytes;
				wlanStatsInfo->tx_unicast64_bytes = wlan_stats.tx_unicast_bytes;
				wlanStatsInfo->rx_errors_frames = wlan_stats.rx_error_frames;
			}
		}
	}
	fclose(fh);
	pthread_mutex_unlock(&siocg_80211_stats);
	return 0;
}

static int get_wlan_bytes(char *wlanname, u32 *rx_bytes, u32 *tx_bytes, u32 *bytes)
{
	FILE *fh;
	char buf[512];
	struct user_net_device_stats wlan_stats;
	int procnetdev_vsn;
	pthread_mutex_lock(&siocg_80211_stats);
	fh = fopen(_PATH_PROCNET_DEV, "r");
	if (!fh)
	{
		APVLog("warning: cannot open %s, limiting output\n", _PATH_PROCNET_DEV);
		printf_d("warning: cannot open %s, limiting output\n", _PATH_PROCNET_DEV);
		pthread_mutex_unlock(&siocg_80211_stats);
		return 1;
	}
	fgets(buf, sizeof(buf), fh);	/* eat line */
	fgets(buf, sizeof(buf), fh);

	procnetdev_vsn = procnetdev_version(buf);

	while (fgets(buf, sizeof(buf), fh))
	{
		char *s, name[128];
		s = get_name(name, buf);
		if(strstr(buf, wlanname))
		{
			if(strncmp(name, wlanname, strlen(name)) == 0)
			{
				get_dev_fields(s, &wlan_stats, procnetdev_vsn);
				*rx_bytes = wlan_stats.rx_bytes;
				*tx_bytes = wlan_stats.tx_bytes;
				*bytes = wlan_stats.rx_bytes + wlan_stats.tx_bytes;
			}
		}
	}
	fclose(fh);
	pthread_mutex_unlock(&siocg_80211_stats);
	return 0;
}

BOOL get_wifiinfo_from_80211stats(char *ifname, WIRELESS_STATE_COUNT_INFO *wifiStatsInfo)
{
	struct ieee80211_stats *stats=NULL;
	struct ifreq ifr;
	int s;
	pthread_mutex_lock(&siocg_80211_stats);
	s = socket(AF_INET, SOCK_DGRAM, 0);
	if (s < 0)
	{
		APVLog("create sock to get 80211stats error!\n");
		printf_d("create sock to get 80211stats error!\n");
		pthread_mutex_unlock(&siocg_80211_stats);
		return FALSE;
	}
	
	strncpy(ifr.ifr_name, ifname, sizeof (ifr.ifr_name));

	stats = malloc(sizeof(struct ieee80211_stats) + 2* sizeof(struct ieee80211_mac_stats));
	memset(stats,0,sizeof(struct ieee80211_stats) + 2* sizeof(struct ieee80211_mac_stats));

	ifr.ifr_data = (caddr_t)stats;
	#if 1
	if (ioctl(s, SIOCG80211STATS, &ifr) < 0)
	{
		APVLog("ioctl to get 80211stats error!\n");
		printf_d("ioctl to get 80211stats error!\n");
		close(s);
		FREE_OBJ(stats);
		pthread_mutex_unlock(&siocg_80211_stats);
		return FALSE;
	}
	#endif
	wifiStatsInfo->rx_manage_frame += stats->is_rx_mgmt;
	wifiStatsInfo->is_rx_mgmtbytes += stats->is_rx_mgmtbytes;
	wifiStatsInfo->is_tx_mgmtbytes += stats->is_tx_mgmtbytes;
	wifiStatsInfo->tx_manage_frame += stats->is_tx_mgmt;

	wifiStatsInfo->rx_authenticate_frame += stats->is_rx_auth_try;
	wifiStatsInfo->rx_associate_frame += stats->is_rx_assoc_try;
	close(s);
	FREE_OBJ(stats);

	pthread_mutex_unlock(&siocg_80211_stats);
	return TRUE;
}


BOOL get_athinfo_from_80211stats(char *ifname, AP_ATH_STATE_COUNT_INFO *wlanStatsInfo)
{
	struct ieee80211_stats *stats=NULL;
	struct ifreq ifr;
	int s;
	pthread_mutex_lock(&siocg_80211_stats);
	s = socket(AF_INET, SOCK_DGRAM, 0);
	if (s < 0)
	{
		APVLog("create sock to get 80211stats error!\n");
		printf_d("create sock to get 80211stats error!\n");
		pthread_mutex_unlock(&siocg_80211_stats);
		return FALSE;
	}
	strncpy(ifr.ifr_name, ifname, sizeof (ifr.ifr_name));

	stats = malloc(sizeof(struct ieee80211_stats) + 2* sizeof(struct ieee80211_mac_stats));
	memset(stats,0,sizeof(struct ieee80211_stats) + 2* sizeof(struct ieee80211_mac_stats));

	ifr.ifr_data = (caddr_t)stats;
	#if 1
	if (ioctl(s, SIOCG80211STATS, &ifr) < 0)
	{
		APVLog("ioctl to get 80211stats error!\n");
		printf_d("ioctl to get 80211stats error!\n");
		close(s);
		FREE_OBJ(stats);
		pthread_mutex_unlock(&siocg_80211_stats);
		return FALSE;
	}
	#endif
#ifdef AR11N
	wlanStatsInfo->crc_error = stats->is_rx_decryptcrc;
	wlanStatsInfo->phy_error = 0;
	wlanStatsInfo->key_error = stats->is_rx_badkeyid;
	wlanStatsInfo->mic_error = stats->is_rx_ccmpmic + stats->is_rx_tkipmic;
#else
	wlanStatsInfo->crc_error = 0;/* !!!wrong data */
	wlanStatsInfo->phy_error = 0;/* !!!wrong data */
	wlanStatsInfo->key_error = 0;/* !!!wrong data */
	wlanStatsInfo->mic_error = 0;/* !!!wrong data */
#endif

	wlanStatsInfo->rx_authenticate_frame += stats->is_rx_auth_try;
	wlanStatsInfo->rx_associate_frame += stats->is_rx_assoc_try;
	
	wlanStatsInfo->success_associate = stats->is_assoc_success;
	wlanStatsInfo->response_associate = stats->is_assoc_success;/* !!!wrong data */
	wlanStatsInfo->request_associate = stats->is_assocs;
	printf_d("wlanStatsInfo->success_associate == %d\n", wlanStatsInfo->success_associate);
	printf_d("wlanStatsInfo->response_associate == %d\n", wlanStatsInfo->response_associate);
	printf_d("wlanStatsInfo->request_associate == %d\n", wlanStatsInfo->request_associate);


	close(s);
	FREE_OBJ(stats);

	pthread_mutex_unlock(&siocg_80211_stats);
	return TRUE;
}

#if 0
static void get_stations_ctrl(WIRELESS_STATE_COUNT_INFO *wlanStatsInfo, unsigned char rid, unsigned char wid)
{
	int s;
	unsigned int tx_ctrl = 0;
	unsigned int rx_ctrl = 0;
	
	struct iwreq iwr;
	struct ieee80211req_sta_stats stats;
	const struct ieee80211_nodestats *ns = &stats.is_stats;
	char ifname[16];
	STA_INFO *ptr = NULL;
	
	s = socket(AF_INET, SOCK_DGRAM, 0);
	if (s < 0)
	{
		APVLog("create sock to get_stations error!\n");
		printf_d("create sock to get_stations error!\n");
		return;
	}
	pthread_mutex_lock(&sta_mutex);
	for(ptr = sta_list;ptr != NULL;ptr = ptr->next)
	{
		if((ptr->rid-1 == rid)&&(ptr->wid == wid))
		{
			(void) memset(&iwr, 0, sizeof(iwr));
			sprintf(ifname, "ath.%d-%d", ptr->rid-1, ptr->wid);
			(void) strncpy(iwr.ifr_name, ifname,16);
			iwr.u.data.pointer = (void *) &stats;
			iwr.u.data.length = sizeof(stats);

			memcpy(stats.is_u.macaddr, ptr->MAC, MAC_ADDR_LEN);
			if (ioctl(s, IEEE80211_IOCTL_STA_STATS, &iwr) < 0)
			{
				APVLog("ioctl to get_sta_ctrl error!\n");
				printf_d("ioctl to get_sta_ctrl error!\n");
				close(s);
				pthread_mutex_unlock(&sta_mutex);
				return;
			}
			rx_ctrl += ns->ns_rx_ctrl;
			tx_ctrl += ns->ns_tx_ctrl;
		}
		
		wlanStatsInfo->rx_ctrl_frame = rx_ctrl;
		wlanStatsInfo->tx_ctrl_frame = tx_ctrl;
	}
	pthread_mutex_unlock(&sta_mutex);
	close(s);
	return;
}
#endif

/*
 * Convert MHz frequency to IEEE channel number.
 */
static unsigned int ieee80211_mhz2ieee(unsigned int freq)
{
	if (freq == 2484)
		return 14;
	if (freq < 2484)
		return (freq - 2407) / 5;
	if (freq < 5000)
		return 15 + ((freq - 2512) / 20);
	return (freq - 5000) / 5;
}

int macAddrCmp (unsigned char* addr1, unsigned char* addr2)
{
	int i, ok=1;
	for (i=0; i<MAC_ADDR_LEN; i++)
	{
		if (addr1[i]!=addr2[i])
		{ok=0;}
	}
	//if (ok==1) {printf_d("MAC Address test: OK\n");}
	//else {printf_d("MAC Address test: Failed\n");}
	return ok;
}

int get_sta_wmm(char *ifname, u8 *sta_mac)
{
	unsigned char buf[24*1024];
	struct iwreq iwr;
	unsigned char *cp;
	int s, len;
	int is_Qos;
	s = socket(AF_INET, SOCK_DGRAM, 0);
	if (s < 0)
	{
		APVLog("create sock to get_stations error!\n");
		printf_d("create sock to get_stations error!\n");
		return 1;
	}

	(void) memset(&iwr, 0, sizeof(iwr));
	(void) strncpy(iwr.ifr_name, ifname, sizeof(iwr.ifr_name));
	printf_d("get_stations ifr.iwr_name:%s\n", iwr.ifr_name);
	iwr.u.data.pointer = (void *) buf;
	iwr.u.data.length = sizeof(buf);
	if (ioctl(s, IEEE80211_IOCTL_STA_INFO, &iwr) < 0)
	{
		APVLog("ioctl to get_stations error! in get_stations\n");
		printf_d("ioctl to get_stations error! in get_stations\n");
		close(s);
		return 1;
	}
	len = iwr.u.data.length;
	if (len < sizeof(struct ieee80211req_sta_info))
	{
		printf_d("get sta info for get_stations error! in get_stations\n");
		close(s);
		return 1;
	}

	cp = buf;
	do {
		struct ieee80211req_sta_info *si;
		u8 *vp;

		si = (struct ieee80211req_sta_info *) cp;
		vp = (u8 *)(si+1);
		
			if(macAddrCmp(si->isi_macaddr, sta_mac))
			{		
				is_Qos = 0;
				if(si->isi_state & IEEE80211_NODE_QOS)
					is_Qos = 1;
			}
			
		cp += si->isi_len;
		len -= si->isi_len;
	} while (len >= sizeof(struct ieee80211req_sta_info));
	close(s);
	return is_Qos;
}


static void get_stations(char *ifname, int staCount, STA_STATE_COUNT_INFO *staInfoValues)
{
	unsigned char buf[24*1024];
	struct iwreq iwr;
	unsigned char *cp;
	int s, len;

	s = socket(AF_INET, SOCK_DGRAM, 0);
	if (s < 0)
	{
		APVLog("create sock to get_stations error!\n");
		printf_d("create sock to get_stations error!\n");
		return;
	}

	(void) memset(&iwr, 0, sizeof(iwr));
	(void) strncpy(iwr.ifr_name, ifname, sizeof(iwr.ifr_name));
	printf_d("get_stations ifr.iwr_name:%s\n", iwr.ifr_name);
	iwr.u.data.pointer = (void *) buf;
	iwr.u.data.length = sizeof(buf);
	if (ioctl(s, IEEE80211_IOCTL_STA_INFO, &iwr) < 0)
	{
		APVLog("ioctl to get_stations error! in get_stations\n");
		printf_d("ioctl to get_stations error! in get_stations\n");
		close(s);
		return;
	}
	len = iwr.u.data.length;
	if (len < sizeof(struct ieee80211req_sta_info))
	{
		APVLog("%s info of %s from drv failed\n",__func__,ifname);
		printf_d("%s info of %s from drv failed\n",__func__,ifname);
		close(s);
		return;
	}

	cp = buf;
	do {
		struct ieee80211req_sta_info *si;
		u8 *vp;

		si = (struct ieee80211req_sta_info *) cp;
		vp = (u8 *)(si+1);
		
		char str_tmp[64] = {0};
		char cmd_str[128] = {0};
		memset(cmd_str, 0, 128);
		sprintf(cmd_str, "iwconfig %s |awk '/%s/, /Noise level/'|grep 'Noise level'|awk -F '[= ]' '{print $22}'", ifname,ifname);
		memset(str_tmp, 0, 64);
		popen_fgets(cmd_str, str_tmp, sizeof(str_tmp));
		staInfoValues->Noise = atoi(str_tmp);

	//	for(i=0;i<staCount;i++)
		{
			if(macAddrCmp(si->isi_macaddr, staInfoValues->mac))
			{
				staInfoValues->wireless_channel = ieee80211_mhz2ieee(si->isi_freq);
				/* Begin: Modified by dongzw for sta rssi&snr report message error 2013-04-12 */
				//staInfoValues->rssi = -(si->isi_rssi*10);
				//staInfoValues->SNR = (-(si->isi_rssi) - staInfoValues->Noise)*10;/* noise < 0 */
				/* Begin: Modified by dongzw for sta rssi&snr report message error 2013-05-02 */
				// staInfoValues->rssi = -(si->isi_rssi);
				staInfoValues->rssi = si->isi_rssi - 95;
				/* End: Modified by dongzw for sta rssi&snr report message error 2013-05-02 */
				staInfoValues->SNR = -(si->isi_rssi) - staInfoValues->Noise;
				/* End: Modified by dongzw for sta rssi&snr report message error 2013-04-12 */

				staInfoValues->vlan_id = si->isi_vlan;
				
				if(si->isi_freq>2000&&si->isi_freq<3000)
				{
					if(si->isi_nrates==4)
						staInfoValues->wireless_mode = REPORT_IEEE80211_PARAM_MODE_11b;
					else if(si->isi_nrates==12)
						staInfoValues->wireless_mode = REPORT_IEEE80211_PARAM_MODE_11b|REPORT_IEEE80211_PARAM_MODE_11g;
					else
						staInfoValues->wireless_mode = REPORT_IEEE80211_PARAM_MODE_11g;
				}
				else
					staInfoValues->wireless_mode = REPORT_IEEE80211_PARAM_MODE_11a;

				/* !!! current rate need to modify */
				if(si->isi_txratekbps != 0)
					staInfoValues->rate = si->isi_txratekbps/1000;
				else
					staInfoValues->rate = (si->isi_rates[si->isi_txrate] & IEEE80211_RATE_VAL)/2;
				if(staInfoValues->rate == 0)
					staInfoValues->rate = 10;
				
				if(si->isi_txratekbps != 0)
					staInfoValues->tx_rate = si->isi_txratekbps/1000;
				else
					staInfoValues->tx_rate = (si->isi_rates[si->isi_txrate] & IEEE80211_RATE_VAL)/2;
				if(si->isi_rxratekbps != 0)
					staInfoValues->rx_rate = si->isi_rxratekbps/1000;
				else
					staInfoValues->rx_rate = 0;
				
				/*
				if(si->isi_txratekbps != 0)
					staInfoValues[i].tx_Rate = si->isi_txratekbps *10/1000;
				else
					staInfoValues[i].tx_Rate = (si->isi_rates[si->isi_txrate] & IEEE80211_RATE_VAL)*10/2;
				if(si->isi_rxratekbps != 0)
					staInfoValues[i].rx_Rate = si->isi_rxratekbps *10/1000;
				else
					staInfoValues[i].rx_Rate =0;
				*/
				if(si->isi_htcap!=0)
				{
					if(si->isi_freq>2000&&si->isi_freq<3000)
						staInfoValues->wireless_mode = REPORT_IEEE80211_PARAM_MODE_11b|REPORT_IEEE80211_PARAM_MODE_11gn;
					else
						staInfoValues->wireless_mode = REPORT_IEEE80211_PARAM_MODE_11an;//11b-0x01,11a-0x02,11g-0x04,11n-0x08,
				}
				else
				{
					if(si->isi_freq>2000&&si->isi_freq<3000)
					{
						if(si->isi_nrates==4)
							staInfoValues->wireless_mode = REPORT_IEEE80211_PARAM_MODE_11b;
						else if(si->isi_nrates==12)
							staInfoValues->wireless_mode = REPORT_IEEE80211_PARAM_MODE_11b|REPORT_IEEE80211_PARAM_MODE_11g;
						else
							staInfoValues->wireless_mode = REPORT_IEEE80211_PARAM_MODE_11g;
					}
					else
						staInfoValues->wireless_mode = REPORT_IEEE80211_PARAM_MODE_11a;
				}
				staInfoValues->is_power_save = 0;
				staInfoValues->is_Qos = 0;
				if(si->isi_state & IEEE80211_NODE_PWR_MGT)
					staInfoValues->is_power_save = 1;
				if(si->isi_state & IEEE80211_NODE_QOS)
					staInfoValues->is_Qos = 1;
			}
		}
		
		cp += si->isi_len;
		len -= si->isi_len;
	} while (len >= sizeof(struct ieee80211req_sta_info));
	close(s);
	return;
}


void delete_vlan_from_bridge(int rid,int wid)
{
	char cmdbuf[100];
	char wlanname[16] = {0};
	char sub_eth_name[16] = {0};
	WLAN_INFO *ptr = NULL,*ptr_t= NULL;
	int vlan_found = 0;
	int eth_nu=0;	

#if (defined( AR11N ))
		eth_nu=1;
#endif

	for(ptr=wlan_list;ptr != NULL;ptr = ptr->next)
	{	
		if((ptr->rid == rid) && (ptr->wid == wid))
		{
			sprintf(wlanname,"ath.%d-%d",ptr->rid -1, ptr->wid);

			sprintf(cmdbuf,"brctl delif br%d %s", ptr->vid, wlanname);
			SYSTEM(cmdbuf);
			
			for(ptr_t=wlan_list;ptr_t != NULL;ptr_t = ptr_t->next)
			{
				if(ptr->vid == ptr_t->vid)
				{
					if((ptr->wid != ptr_t->wid) && (ptr_t->data_packet_method == LOCAL_BRIDGE))
					{
						vlan_found = 1;
						break;
					}
				}
			}
			if(!vlan_found)
			{
				sprintf(sub_eth_name,"eth%d.%d",eth_nu, ptr->vid);

				sprintf(cmdbuf,"brctl delif br%d %s", ptr->vid, sub_eth_name);
				SYSTEM(cmdbuf);
				sprintf(cmdbuf,"/sbin/ifconfig %s down",sub_eth_name);
				SYSTEM(cmdbuf);
				sprintf(cmdbuf, "vconfig rem %s",sub_eth_name);
				SYSTEM(cmdbuf);
				sprintf(cmdbuf,"/sbin/ifconfig br%d down", ptr->vid);
				SYSTEM(cmdbuf);
				sprintf(cmdbuf, "brctl delbr br%d", ptr->vid);
				SYSTEM(cmdbuf);

				set_dhcp_snooping(ENABLED);
			}
			vlan_found = 0;
		}		
	}
	return;
}


static void get_sta_max_rate(char *ifname, AP_ATH_STATE_COUNT_INFO *staInfoValues)
{
	unsigned char buf[24*1024];
	struct iwreq iwr;
	unsigned char *cp;
	int s, len;
	int rate = 0;
	
	s = socket(AF_INET, SOCK_DGRAM, 0);
	if (s < 0)
	{
		APVLog("create sock to get_stations error!\n");
		printf_d("create sock to get_stations error!\n");
		return;
	}

	(void) memset(&iwr, 0, sizeof(iwr));
	(void) strncpy(iwr.ifr_name, ifname, sizeof(iwr.ifr_name));
	iwr.u.data.pointer = (void *) buf;
	iwr.u.data.length = sizeof(buf);
	if (ioctl(s, IEEE80211_IOCTL_STA_INFO, &iwr) < 0)
	{
		APVLog("ioctl to get_stations error! in get_sta_max_rate\n");
		printf_d("ioctl to get_stations error! in get_sta_max_rate\n");
		close(s);
		return;
	}
	len = iwr.u.data.length;
	if (len < sizeof(struct ieee80211req_sta_info))
	{
		//APVLog("%s via %s from drv failed\n",__func__,ifname);
		printf_d("%s via %s from drv failed\n",__func__,ifname);
		close(s);
		return;
	}

	cp = buf;

	staInfoValues->sta_downlink_max_speed = 0;
	staInfoValues->sta_uplink_max_speed = 0;
	do {
		struct ieee80211req_sta_info *si;
		u8 *vp;

		si = (struct ieee80211req_sta_info *) cp;
		vp = (u8 *)(si+1); 
		if(si->isi_txratekbps != 0)
		{
			rate = si->isi_txratekbps *10/1000;
			if(rate > staInfoValues->sta_downlink_max_speed)
				staInfoValues->sta_downlink_max_speed = rate;
		}
		else 
		{
			rate = (si->isi_rates[si->isi_txrate] & IEEE80211_RATE_VAL)*10/2;
			if(rate > staInfoValues->sta_downlink_max_speed)
				staInfoValues->sta_downlink_max_speed = rate;
		}
 
		if(si->isi_rxratekbps != 0)
		{
			rate = si->isi_rxratekbps *10/1000;
			if(rate > staInfoValues->sta_uplink_max_speed)
				staInfoValues->sta_uplink_max_speed = rate;
		} 
		cp += si->isi_len;
		len -= si->isi_len;
	} while (len >= sizeof(struct ieee80211req_sta_info));
	close(s);
	return;
}

int sta_compare(char *ifname, STA_INFO *sta)
{
	unsigned char buf[24*1024] = {0};
	struct iwreq iwr;
	unsigned char *cp;
	int s, len;

	s = socket(AF_INET, SOCK_DGRAM, 0);
	if (s < 0)
	{
		APVLog("create sock to get_stations error!\n");
		printf_d("create sock to get_stations error!\n");
		return -1;
	}

	(void) memset(&iwr, 0, sizeof(iwr));
	(void) strncpy(iwr.ifr_name, ifname, sizeof(iwr.ifr_name));
	iwr.u.data.pointer = (void *) buf;
	iwr.u.data.length = sizeof(buf);
	if (ioctl(s, IEEE80211_IOCTL_STA_INFO, &iwr) < 0)
	{
		APVLog("ioctl to get_stations error! in sta_compare\n");
		printf_d("ioctl to get_stations error! in sta_compare\n");
		close(s);
		return -1;
	}
	len = iwr.u.data.length;
	if (len < sizeof(struct ieee80211req_sta_info))
	{
		//APVLog("%s, get sta("MAC_FMT") info via %s from drv failed,so sta not in %s\n",__func__,MAC2STR(sta->MAC),ifname,ifname);
		printf_d("%s, get sta("MAC_FMT") info via %s from drv failed,so sta not in %s\n",__func__,MAC2STR(sta->MAC),ifname,ifname);
		close(s);
		return -1;
	}

	cp = buf;
	do {
		struct ieee80211req_sta_info *si;

		si = (struct ieee80211req_sta_info *) cp;

		if(macAddrCmp(si->isi_macaddr, sta->MAC))
		{
			sta->time_stamp = si->isi_assoc_time;/* get sta time stamp */
			APVLog("%s, get sta("MAC_FMT") info via %s from drv ok,so sta in %s\n",__func__,MAC2STR(sta->MAC),ifname,ifname);
			printf_d("%s, get sta("MAC_FMT") info via %s from drv ok,so sta in %s\n",__func__,MAC2STR(sta->MAC),ifname,ifname);
			close(s);/*peiwh add at 20121004*/
			return 0;
		}
		
		cp += si->isi_len;
		len -= si->isi_len;
	} while (len >= sizeof(struct ieee80211req_sta_info));
	close(s);
	return 1;
}

static void get_sta_stats(STA_STATE_COUNT_INFO *staInfoValues, int sta_count)
{
	int s,i ;
	struct iwreq iwr;
	struct ieee80211req_sta_stats stats;
	const struct ieee80211_nodestats *ns = &stats.is_stats;
	char ifname[16];
	
	s = socket(AF_INET, SOCK_DGRAM, 0);
	if (s < 0)
	{
		APVLog("create sock to get_sta_stats error!\n");
		printf_d("create sock to get_sta_stats error!\n");
		return;
	}
	for(i=0;i<sta_count;i++){
		(void) memset(&iwr, 0, sizeof(iwr));
		sprintf(ifname,"ath.%d-%d", staInfoValues[i].radio_id-1, staInfoValues[i].wlan_id);
		(void) strncpy(iwr.ifr_name, ifname,16);
		iwr.u.data.pointer = (void *) &stats;
		iwr.u.data.length = sizeof(stats);

		memcpy(stats.is_u.macaddr, staInfoValues[i].mac, IEEE80211_ADDR_LEN);
		if (ioctl(s, IEEE80211_IOCTL_STA_STATS, &iwr) < 0){
			APVLog("ioctl to get_sta_stats error!\n");
			printf_d("ioctl to get_sta_stats error!\n");
			close(s);
			return;
		}
		staInfoValues[i].sta_rx_bytes = ns->ns_rx_bytes;
		staInfoValues[i].sta_tx_bytes = ns->ns_tx_bytes;
		staInfoValues[i].tx_frag_packets = ns->tx_frag_packets;
		staInfoValues[i].rx_frag_packets = ns->rx_frag_packets;
		staInfoValues[i].sta_rx_packet = ns->ns_rx_data+ns->ns_rx_mgmt+ns->ns_rx_ctrl;
		staInfoValues[i].sta_tx_packet = ns->ns_tx_data+ns->ns_tx_mgmt+ns->ns_tx_ctrl;
		printf_d("staInfoValues[i].tx_frag_packets == %d\n", staInfoValues[i].tx_frag_packets);
		printf_d("staInfoValues[i].rx_frag_packets == %d\n", staInfoValues[i].rx_frag_packets);
	}
	close(s);
	return;
}

BOOL GetInterfaceCount(int *count, char *type)
{
	char cmd_n[100];
	char bfr[10];
	
	memset(cmd_n, 0, 100);
	sprintf(cmd_n, "cat /proc/net/dev | awk -F \" \" '/%s:/' | wc -l", type);
	popen_fgets(cmd_n, bfr, sizeof(bfr));
	*count = atoi(bfr);
	return TRUE;
}

BOOL GetEthUpdownTime(int eth_id, int *eth_updown_time)
{
		*eth_updown_time = 1;
		char temp_str[100];
		char str[20];
		int ifUpTime = 0;
		int ifDownTime = 0;
		
		memset(temp_str, 0, 100);
		sprintf(temp_str, "cat /tmp/link_count/link_up_count_eth%d", eth_id);
		popen_fgets(temp_str, str, sizeof(str));
		ifUpTime = atoi(str);
		printf_d("ifUpTime: %s len:%d ifUpTime:%d\n", str, (int)strlen(str), ifUpTime);
		memset(temp_str, 0, 100);
		sprintf(temp_str, "cat /tmp/link_count/link_down_count_eth%d", eth_id);
		popen_fgets(temp_str, str, sizeof(str));			
		ifDownTime = atoi(str);
		printf_d("ifDownTime: %s len:%d ifDownTime:%d\n", str, (int)strlen(str), ifDownTime);
		if((ifUpTime+ifDownTime) == 0)
			*eth_updown_time = 1;
		else
			*eth_updown_time = ifUpTime+ifDownTime;
	return TRUE;
}

BOOL GetMACErrorInfo(char *wifiname, WIRELESS_STATE_COUNT_INFO *state_count_info)
{
	int s;
	struct ifreq ifr;
	struct ath_stats total;

	s = socket(AF_INET, SOCK_DGRAM, 0);
	if (s < 0)
	{
		APVLog("create sock to get extra info error!\n");
		printf_d("create sock to get extra info error!\n");
		return FALSE;
	}
	strncpy(ifr.ifr_name, wifiname, sizeof (ifr.ifr_name));
	ifr.ifr_data = (caddr_t) &total;
	if (ioctl(s, SIOCGATHSTATS, &ifr) < 0)
	{
		APVLog("ioctl to get extra info error!\n");
		printf_d("ioctl to get extra info error!\n");
		close(s);
		return FALSE;
	}
	state_count_info->rx_fcs_mac_error_frame = total.ast_rx_crcerr;
	state_count_info->rx_mic_mac_error_frame = total.ast_rx_badmic;
	state_count_info->rx_dissect_failed_mac_error_frame = total.ast_rx_badcrypt;
	state_count_info->rx_phy_errors = total.ast_rx_phyerr;
	state_count_info->tx_signaling_packet = total.ast_be_xmit;
	close(s);
	return TRUE;
}
BOOL GetExtraInfo(char *wifiname, AP_ATH_STATE_COUNT_INFO *extraInfoValues)
{
	int s;
	struct ifreq ifr;
	struct ath_stats total;

	s = socket(AF_INET, SOCK_DGRAM, 0);
	if (s < 0)
	{
		APVLog("create sock to get extra info error!\n");
		printf_d("create sock to get extra info error!\n");
		return FALSE;
	}
	strncpy(ifr.ifr_name, wifiname, sizeof (ifr.ifr_name));
	ifr.ifr_data = (caddr_t) &total;
	if (ioctl(s, SIOCGATHSTATS, &ifr) < 0)
	{
		APVLog("ioctl to get extra info error!\n");
		printf_d("ioctl to get extra info error!\n");
		close(s);
		return FALSE;
	}
/*	
	if (!getifstats(ifr.ifr_name, &itot, &otot))
	{
		CWWTPDebugLog("getifstats to get extra info error!");
		if(debug_print)
			printf("getifstats to get extra info error!\n");
		close(s);
		return CW_FALSE;
		//err(1, ifr.ifr_name);
	}
*/
	extraInfoValues->tx_packet += total.ast_tx_packets;
	extraInfoValues->rx_packet += total.ast_rx_packets;
	extraInfoValues->tx_bytes += total.ast_tx_bytes;
	extraInfoValues->rx_bytes += total.ast_tx_bytes;
		
/*	printf("%8u %8u %7u %7u %7u %6u %6u %6u %7u\n"
		, itot - total.ast_rx_mgt
		, otot
		, total.ast_tx_altrate
		, total.ast_tx_shortretry
		, total.ast_tx_longretry
		, total.ast_tx_xretries
		, total.ast_rx_crcerr
		, total.ast_rx_badcrypt
		, total.ast_rx_phyerr
	);
*/
	close(s);
	return TRUE;
}

////////////////////////////////////////////////////////////////
BOOL get_ap_ability_info(AP_ABILITY_INFO **ability_info)
{
	char str_tmp[64];
	char cmd_str[128];

	//*ability_info = (AP_ABILITY_INFO *)malloc(sizeof(AP_ABILITY_INFO));	
	CREATE_OBJECT_ERR(*ability_info,AP_ABILITY_INFO,MALLOC_ERR_RET(FALSE));
	if(*ability_info == NULL)
		return RAISE_ERROR(ERROR_OUT_OF_MEMORY, NULL);

	(*ability_info)->net_code = NULL;
	(*ability_info)->CPU_INFO = NULL;
	(*ability_info)->MEM_TYPE = NULL;
	#if 0
	/* !!! net_code	<64 */
	char *net_code = "BJ_BJ_01";
	//(*ability_info)->net_code = (char *)malloc(strlen(net_code)+1);
	CREATE_STRING_ERR((*ability_info)->net_code,strlen(net_code),MALLOC_ERR_RET(FALSE));
	strcpy((*ability_info)->net_code, net_code);
	#endif

	memset(cmd_str, 0, 128);
	sprintf(cmd_str,  "/usr/sbin/get_hardinfo | awk -F \":\" '/eth_num/ {print $2}'");
	memset(str_tmp, 0, 64);
	popen_fgets(cmd_str, str_tmp, sizeof(str_tmp));
	(*ability_info)->eth_num = atoi(str_tmp);

	memset(cmd_str, 0, 128);
	//sprintf(cmd_str, "/usr/sbin/get_hardinfo | grep cpu_type");
	sprintf(cmd_str, "/usr/sbin/get_hardinfo | awk -F \": \" '/cpu_type/ {print $2}'");
	memset(str_tmp, 0, 64);
	popen_fgets(cmd_str, str_tmp, sizeof(str_tmp));
#if 1
	//(*ability_info)->CPU_INFO = (char *)malloc(strlen(str_tmp)+1);
	CREATE_STRING_ERR((*ability_info)->CPU_INFO,strlen(str_tmp),MALLOC_ERR_RET(FALSE));
	strcpy((*ability_info)->CPU_INFO,str_tmp);
#else
	char cmd_str1[128];
	memset(cmd_str1, 0, 128);
	strcpy(cmd_str1, str_tmp);

	char *string = "| BogoMIPS: ";
	strncat(cmd_str1, string, strlen(string));
	memset(cmd_str, 0, 128);
	sprintf(cmd_str, "cat /proc/cpuinfo | grep BogoMIPS|awk -F \":\" '{print $2}'");
	memset(str_tmp, 0, 64);
	popen_fgets(cmd_str, str_tmp, sizeof(str_tmp));
	strncat(cmd_str1, str_tmp, strlen(str_tmp));
	//(*ability_info)->CPU_INFO = (char *)malloc(strlen(cmd_str1)+1);
	CREATE_STRING_ERR((*ability_info)->CPU_INFO,strlen(cmd_str1),MALLOC_ERR_RET(FALSE));
	strcpy((*ability_info)->CPU_INFO,cmd_str1);
#endif
	memset(cmd_str, 0, 128);
	sprintf(cmd_str,  "/usr/sbin/get_hardinfo | awk -F \":\" '/mem_type/ {print $2}'");
	memset(str_tmp, 0, 64);
	popen_fgets(cmd_str, str_tmp, sizeof(str_tmp));
	//(*ability_info)->MEM_TYPE = (char *)malloc(strlen(str_tmp)+1);
	CREATE_STRING_ERR((*ability_info)->MEM_TYPE, strlen(str_tmp), MALLOC_ERR_RET(FALSE));
	strcpy((*ability_info)->MEM_TYPE,str_tmp);

	memset(cmd_str, 0, 128);
	sprintf(cmd_str, "/usr/sbin/get_hardinfo|awk -F \":\" '/mem_size/ {print $2}'|awk -F \"M\" '{print $1}'");
	memset(str_tmp, 0, 64);
	popen_fgets(cmd_str, str_tmp, sizeof(str_tmp));
	(*ability_info)->mem_size = atoi(str_tmp)*1024;

	memset(cmd_str, 0, 128);
	sprintf(cmd_str, "/usr/sbin/get_hardinfo|awk -F \":\" '/flash_size/ {print $2}'|awk -F \"M\" '{print $1}'");
	memset(str_tmp, 0, 64);
	popen_fgets(cmd_str, str_tmp, sizeof(str_tmp));
	flash_size = atoi(str_tmp)*1024;
	(*ability_info)->flash_size = flash_size;

	memset(cmd_str, 0, 128);
	sprintf(cmd_str, "/sbin/ifconfig default|awk -F \" \" '/inet addr/ {print $2}'|awk -F \":\" '{print $2}'");
	memset(str_tmp, 0, 64);
	popen_fgets(cmd_str, str_tmp, sizeof(str_tmp));
	(*ability_info)->IPv4 = inet_strtoint(str_tmp);
	
	memset(cmd_str, 0, 128);
	sprintf(cmd_str, "/sbin/ifconfig default|awk -F \" \" '/Mask/ {print $4}'|awk -F \":\" '{print $2}'");
	memset(str_tmp, 0, 64);
	popen_fgets(cmd_str, str_tmp, sizeof(str_tmp));
	(*ability_info)->Mask = inet_strtoint(str_tmp);
	
	memset(cmd_str, 0, 128);
	sprintf(cmd_str, "route -n | awk -F \" \" '/UG/ {print $2}'");
	memset(str_tmp, 0, 64);
	popen_fgets(cmd_str, str_tmp, sizeof(str_tmp));
	(*ability_info)->GateWay = inet_strtoint(str_tmp);

	memset(cmd_str, 0, 128);
	sprintf(cmd_str, "Manufactor %s\nSoftware version %s\nHardware version %s\n", COMPANY, SOFTWARE_VER, HARDWARE_VER);
	//(*ability_info)->dev_describe = (char *)malloc(strlen(cmd_str)+1);
	CREATE_STRING_ERR((*ability_info)->dev_describe,strlen(cmd_str),MALLOC_ERR_RET(FALSE));
	strcpy((*ability_info)->dev_describe, cmd_str);

	memset(cmd_str, 0, 128);
	sprintf(cmd_str,  "AOS");
	//(*ability_info)->software_name = (char *)malloc(strlen(cmd_str)+1);
	CREATE_STRING_ERR((*ability_info)->software_name,strlen(cmd_str),MALLOC_ERR_RET(FALSE));
	strcpy((*ability_info)->software_name, cmd_str);
	return TRUE;	
}
BOOL get_ap_run_time(u32 *run_time)
{	
	char str_tmp[64] = {0},cmd_str[128] = {0};
	sprintf(cmd_str, "cat /proc/uptime | awk -F ' ' '{print $1}' | awk -F '.' '{print $1}'");
	popen_fgets(cmd_str, str_tmp, sizeof(str_tmp));
	/* Begin: Modified by dongzw for fix report system run time error 2013-04-10 */
	printf_d("%s, %d, runtime: %d seconds, %d centiseconds.\n", __func__, __LINE__, atoi(str_tmp), atoi(str_tmp) * 100);
	*run_time = atoi(str_tmp) * 100;
	/* End: Modified by dongzw for fix report system run time error 2013-04-10 */
	return TRUE;
}
BOOL get_ap_state_count_info(AP_STATE_COUNT_INFO **state_count_info)
{
	/* first create the GetApScanningInfo thread to get neighbor ap info */
	char str_tmp[64];
	char cmd_str[128];
	char wlanname[16];
	int i,count;
	int CPU_use_rate_average = 0;
	int MEM_use_rate_average = 0;
	int interval_bytes = 0;

	//*state_count_info = (AP_STATE_COUNT_INFO *)malloc(sizeof(AP_STATE_COUNT_INFO));	
	CREATE_OBJECT_ERR(*state_count_info,AP_STATE_COUNT_INFO,MALLOC_ERR_RET(FALSE));
	if(*state_count_info == NULL)
		return RAISE_ERROR(ERROR_OUT_OF_MEMORY, NULL);

	system("/usr/sbin/cpu_util_rate 1  > /dev/null");
	GetCpuUseRatio(&((*state_count_info)->CPU_use_rate));
	GetMemUseRatio(&((*state_count_info)->MEM_use_rate));
	(*state_count_info)->CPU_use_rate = ((*state_count_info)->CPU_use_rate)/100;
	(*state_count_info)->MEM_use_rate = ((*state_count_info)->MEM_use_rate)/100;
	(*state_count_info)->AP_wireless_monitor_work_mode = 1;
	/* !!! normal,没有下发iwscan开关报文所以暂时不支持 monitor semiMonitor 
	 1: normal(1)
	 2: monitor(2)
	 3: semiMonitor(3)
	*/

	memset(cmd_str, 0, 128);
	sprintf(cmd_str, "cat /proc/net/dev | awk -F':' '/ath/{print $1}' | wc -l");
	memset(str_tmp, 0, 64);
	popen_fgets(cmd_str, str_tmp, sizeof(str_tmp));
	(*state_count_info)->BSSID_Count = atoi(str_tmp);	//wlan count
	(*state_count_info)->terminal_disturb_alarm_threshold = 0;
	(*state_count_info)->same_frequency_disturb_alarm_threshold = 27;
	(*state_count_info)->adjacent_frequency_disturb_alarm_threshold = 27;
	(*state_count_info)->CPU_use_rate_alarm_threshold = CPU_ALARM_THRESHOLD;
	(*state_count_info)->MEM_use_rate_alarm_threshold = MEM_ALARM_THRESHOLD;

	count = interval_count_monitor_info->count;
	interval_bytes += interval_count_monitor_info[0].ap_interval_bytes;/* 采样时长内的流量值 */
	for(i = 0; i < count; i++)
	{
		CPU_use_rate_average += interval_count_monitor_info[i].CPU_use_rate_average;
		MEM_use_rate_average += interval_count_monitor_info[i].MEM_use_rate_average;
	}
	interval_bytes = interval_bytes*8;
	interval_bytes = interval_bytes/1024;
	(*state_count_info)->AP_flow = interval_bytes/SAMPLING_INTERVAL;/* kbps */
	if(count != 0)
	{
		(*state_count_info)->CPU_use_rate_average = CPU_use_rate_average/count;
		(*state_count_info)->MEM_use_rate_average = MEM_use_rate_average/count;
	}
	(*state_count_info)->CPU_use_rate_max = CPU_use_rate_max;
	(*state_count_info)->MEM_use_rate_max = MEM_use_rate_max;

	AP_CURRENT_TIME = info_report_get_current_time();
	get_ap_run_time(&((*state_count_info)->run_time));
	//(*state_count_info)->run_time = AP_CURRENT_TIME - AP_START_TIME;
	if(wlan_list == NULL)
	{
		APVLog("there are no wlan, no count info to report!\n");
		printf_d("there are no wlan, no count info to report!\n");
		return TRUE;
	}
	WLAN_INFO *ptr = NULL;
	/* init */
	(*state_count_info)->connect_count = 0;
	(*state_count_info)->connect_failed_count  = 0;
	(*state_count_info)->connect_retry_count = 0;
	(*state_count_info)->connect_undistinguish_retry_count = 0;
	(*state_count_info)->disconnect_unnormal_count = 0;
	(*state_count_info)->connect_poor_resource_failed_count = 0;
	(*state_count_info)->connect_no_support_rate_failed_count = 0;
	(*state_count_info)->connect_low_rssi_failed_count = 0;
	pthread_mutex_lock(&wlan_mutex);
	for(ptr = wlan_list; ptr != NULL; ptr = ptr->next)
	{
		sprintf(wlanname,"ath.%d-%d", ptr->rid-1, ptr->wid);
		GetDisassocErrors(wlanname, (*state_count_info));
	}
	pthread_mutex_unlock(&wlan_mutex);
	
	int flash_use_size = 0;
	memset(cmd_str, 0, 128);
	sprintf(cmd_str,  "du -sk /");
	memset(str_tmp, 0, 64);
	popen_fgets(cmd_str, str_tmp, sizeof(str_tmp));
	flash_use_size = atoi(str_tmp);
	(*state_count_info)->flash_last_space = flash_size - flash_use_size;
	
	return TRUE;
}

BOOL get_wireless_ability_info(WIRELESS_ABILITY_INFO **ability_info)
{
	char str_tmp[64];
	char cmd_str[128];
	char if_name[16];
	WIRELESS_ABILITY_INFO *head = NULL;
	
	int i;
	for(i = 0; i < RADIO_NUM; i++)
	{
		if((*ability_info) == NULL)
		{		
			//*ability_info = (WIRELESS_ABILITY_INFO *)malloc(sizeof(WIRELESS_ABILITY_INFO));	
			CREATE_OBJECT_ERR(*ability_info, WIRELESS_ABILITY_INFO, MALLOC_ERR_RET(FALSE));
			(*ability_info)->next = NULL;
			head = (*ability_info);
		}
		else
		{
			//(*ability_info)->next = (WIRELESS_ABILITY_INFO *)malloc(sizeof(WIRELESS_ABILITY_INFO));
			CREATE_OBJECT_ERR((*ability_info)->next, WIRELESS_ABILITY_INFO, MALLOC_ERR_RET(FALSE));
			(*ability_info) = (*ability_info)->next;
			(*ability_info)->next = NULL;
		}
		if((*ability_info) == NULL)
				return RAISE_ERROR(ERROR_OUT_OF_MEMORY, NULL);
		
		sprintf(if_name, "wifi%d", i);
		(*ability_info)->radio_id = i+1;
		//(*ability_info)->if_info = (char *)malloc(strlen(if_name)+1);
		CREATE_STRING_ERR((*ability_info)->if_info,strlen(if_name),MALLOC_ERR_RET(FALSE));
		memcpy((*ability_info)->if_info,if_name,strlen(if_name));
		(*ability_info)->if_info[strlen(if_name)] = '\0';
 
		(*ability_info)->if_type = MIB_IF_TYPE_WIFI;
		
		memset(cmd_str, 0, 128);
		sprintf(cmd_str, "ifconfig | awk '/%s/,/MTU/'|grep MTU|awk -F \"MTU:\" '{print $2}'|awk -F \" \" '{print $1}'", if_name);
		memset(str_tmp, 0, 64);
		popen_fgets(cmd_str, str_tmp, sizeof(str_tmp));
		(*ability_info)->if_MTU = atoi(str_tmp);
		if((*ability_info)->if_MTU == 0)
			(*ability_info)->if_MTU = 1500;

#ifdef AR11N
		(*ability_info)->if_bandwidth = 150;
#else
		(*ability_info)->if_bandwidth = 300;
#endif
		
		(*ability_info)->if_mac = malloc(6);
		memset(cmd_str, 0, 128);
		sprintf(cmd_str, "ifconfig | grep %s | awk -F \" \" '{print $5}'",if_name);
		memset(str_tmp, 0, 64);
		popen_fgets(cmd_str, str_tmp, sizeof(str_tmp));
		sscanf(str_tmp,"%02hhX:%02hhX:%02hhX:%02hhX:%02hhX:%02hhX",
			&(*ability_info)->if_mac[0], &(*ability_info)->if_mac[1], &(*ability_info)->if_mac[2],
			&(*ability_info)->if_mac[3], &(*ability_info)->if_mac[4], &(*ability_info)->if_mac[5]);
			
		(*ability_info)->support_wireless_mode = 0x1F;/* support all mode */
		
		memset(cmd_str, 0, 128);
		sprintf(cmd_str,"cat /tmp/device_info.xml|awk '/%s/, /\\/%s/'|grep \"max_power\"|awk -F '[<>]' '{print $3}'", if_name, if_name);
		memset(str_tmp, 0, 64);
		popen_fgets(cmd_str, str_tmp, sizeof(str_tmp));
		(*ability_info)->max_txpower = atoi(str_tmp)*10;
		
		(*ability_info)->power_attenuation_area = (*ability_info)->max_txpower/10;
		(*ability_info)->power_adjust_interval = 1;
		(*ability_info)->antenna = 3;
	}
	(*ability_info) = head;
	return TRUE;
}

BOOL get_wireless_state_count_info(WIRELESS_STATE_COUNT_INFO **state_count_info)
{
	char str_tmp[64];
	char cmd_str[128];
	char wlanname[16];
	WLAN_INFO *ptr = NULL;
	int i, j;
	u32 rate_mask = 0;
	u32 current_mode = 0;
	char *string = NULL;
	WIRELESS_STATE_COUNT_INFO *head = NULL;

	wlan_stats_info *wlanStatsInfo = NULL;
	//wlanStatsInfo = (wlan_stats_info *)malloc(sizeof(wlan_stats_info));
	CREATE_OBJECT_ERR(wlanStatsInfo,wlan_stats_info,MALLOC_ERR_RET(FALSE));
	pthread_mutex_lock(&wlan_mutex);
	
	if(wlan_list == NULL)
	{
		APVLog("there are no wlan, nothing to report!\n");
		printf_d("there are no wlan, nothing to report!\n");
		pthread_mutex_unlock(&wlan_mutex);
		return FALSE;
	}
	
	for(i = 0; i < RADIO_NUM; i++)
	{
		if((*state_count_info) == NULL)
		{
			//*state_count_info = (WIRELESS_STATE_COUNT_INFO *)malloc(sizeof(WIRELESS_STATE_COUNT_INFO)); 
			CREATE_OBJECT_ERR(*state_count_info,WIRELESS_STATE_COUNT_INFO,MALLOC_ERR_RET(FALSE));
			(*state_count_info)->next = NULL;
			head = *state_count_info;
		}
		else
		{
			//(*state_count_info)->next = (WIRELESS_STATE_COUNT_INFO *)malloc(sizeof(WIRELESS_STATE_COUNT_INFO));
			CREATE_OBJECT_ERR((*state_count_info)->next,WIRELESS_STATE_COUNT_INFO,MALLOC_ERR_RET(FALSE));
			(*state_count_info) = (*state_count_info)->next;
			(*state_count_info)->next = NULL;
		}
		if(*state_count_info == NULL)
		{
			pthread_mutex_unlock(&wlan_mutex);
			return RAISE_ERROR(ERROR_OUT_OF_MEMORY, NULL);
		}
		(*state_count_info)->radio_id = i+1;
		/* iwpriv get */
		/* !!! gRadioInfoValue中数值不正常 */
		(*state_count_info)->beacon_frame_interval = gRadioInfoValue[(*state_count_info)->radio_id-1].beaconInterval;
		(*state_count_info)->DTIM_interval = gRadioInfoValue[(*state_count_info)->radio_id-1].dtim;
		(*state_count_info)->RTS_threshold = gRadioInfoValue[(*state_count_info)->radio_id-1].rtsThreshold;
		(*state_count_info)->frag_threshold = gRadioInfoValue[(*state_count_info)->radio_id-1].fragThreshold;
		/* !!! */
		(*state_count_info)->preamble_length = gRadioInfoValue[(*state_count_info)->radio_id-1].preamble;
		(*state_count_info)->current_txpower = gRadioInfoValue[(*state_count_info)->radio_id-1].txpower*10;
		(*state_count_info)->current_channel = gRadioInfoValue[(*state_count_info)->radio_id-1].channel;

		if(gRadioInfoValue[(*state_count_info)->radio_id-1].channel == 0)
			(*state_count_info)->auto_wireless_channel_switch = ENABLED;
		else
			(*state_count_info)->auto_wireless_channel_switch = DISABLED;
		
		(*state_count_info)->auto_power_control_switch = gRadioInfoValue[(*state_count_info)->radio_id-1].auto_txpower;

		//memset(cmd_str, 0, 128);
		//sprintf(cmd_str,"cat /proc/sys/net/%s/maxaid", wlanname);
		//popen_fgets(cmd_str, str_tmp, sizeof(str_tmp));
		//(*state_count_info)->max_sta_num = atoi(str_tmp);
		(*state_count_info)->max_sta_num = MAX_STA_NUM;
		
		current_mode = gRadioInfoValue[(*state_count_info)->radio_id-1].radiotype;
		/* init */
		(*state_count_info)->current_wireless_mode = 0;
		(*state_count_info)->current_rate = NULL;
		(*state_count_info)->wifi_state = gRadioInfoValue[(*state_count_info)->radio_id-1].radioState;

		gRadioInfoValue[(*state_count_info)->radio_id-1].wifi_current_time = info_report_get_current_time();
		int rid = (*state_count_info)->radio_id-1;
		save_radio_level_config(rid,"wifi_current_time",UL,(unsigned long*)&gRadioInfoValue[rid].wifi_current_time,1);
		(*state_count_info)->wifi_state_continuous_time = 
			gRadioInfoValue[(*state_count_info)->radio_id-1].wifi_current_time - gRadioInfoValue[(*state_count_info)->radio_id-1].wifi_start_time;

		if(current_mode == 0)
			;
		else
		{
			if(current_mode & IEEE80211_PARAM_MODE_11g)
			{
				memset(cmd_str, 0, 128);
				sprintf(cmd_str, "iwpriv wifi%d get_rate_mask_g|awk -F \":\" '{print $2}'", (*state_count_info)->radio_id-1);
				memset(str_tmp, 0, 64);
				popen_fgets(cmd_str, str_tmp, sizeof(str_tmp));
				rate_mask = atoi(str_tmp);//65535 FFFF
				(*state_count_info)->current_rate = rate_get(IEEE80211_PARAM_MODE_11g, rate_mask);
			}
			else if(current_mode & IEEE80211_PARAM_MODE_11b)
			{
				memset(cmd_str, 0, 128);
				sprintf(cmd_str, "iwpriv wifi%d get_rate_mask_b|awk -F \":\" '{print $2}'", (*state_count_info)->radio_id-1);
				memset(str_tmp, 0, 64);
				popen_fgets(cmd_str, str_tmp, sizeof(str_tmp));
				rate_mask = atoi(str_tmp);//5631 15FF
				(*state_count_info)->current_rate = rate_get(IEEE80211_PARAM_MODE_11b, rate_mask);
			}
			else if(current_mode & IEEE80211_PARAM_MODE_11a)
			{
				memset(cmd_str, 0, 128);
				sprintf(cmd_str, "iwpriv wifi%d get_rate_mask_a|awk -F \":\" '{print $2}'", (*state_count_info)->radio_id-1);
				memset(str_tmp, 0, 64);
				popen_fgets(cmd_str, str_tmp, sizeof(str_tmp));
				rate_mask = atoi(str_tmp);//255 FF
				(*state_count_info)->current_rate = rate_get(IEEE80211_PARAM_MODE_11a, rate_mask);
			}

			if(current_mode & IEEE80211_PARAM_MODE_11n_a || current_mode & IEEE80211_PARAM_MODE_11n_g)
			{
				memset(cmd_str, 0, 128);
				sprintf(cmd_str, "iwpriv wifi%d get_mcs_mask|awk -F \":\" '{print $2}'", (*state_count_info)->radio_id-1);
				memset(str_tmp, 0, 64);
				popen_fgets(cmd_str, str_tmp, sizeof(str_tmp));
				rate_mask = atoi(str_tmp);
				string = mcs_get(rate_mask);
				printf_d("string:\n%s\n", string);
				printf_d("strlen(string):%d\n", strlen(string));
				if((*state_count_info)->current_rate == NULL)
				{
					//(*state_count_info)->current_rate = (char *)malloc(strlen(string)+1);
					CREATE_STRING_ERR((*state_count_info)->current_rate,strlen(string),MALLOC_ERR_RET(FALSE));
					strcpy((*state_count_info)->current_rate, string);
				}
				else
				{
					printf("strlen((*state_count_info)->current_rate):%d\n", strlen((*state_count_info)->current_rate));
					(*state_count_info)->current_rate = (char *)realloc((*state_count_info)->current_rate, strlen((*state_count_info)->current_rate) + strlen(string)+2);
					strcat((*state_count_info)->current_rate, ",");
					strcat((*state_count_info)->current_rate, string);
				}				
				FREE_OBJ(string);
			}
			if(current_mode & IEEE80211_PARAM_MODE_11g)
				(*state_count_info)->current_wireless_mode = (*state_count_info)->current_wireless_mode | REPORT_IEEE80211_PARAM_MODE_11g;
			if(current_mode & IEEE80211_PARAM_MODE_11b)
				(*state_count_info)->current_wireless_mode = (*state_count_info)->current_wireless_mode | REPORT_IEEE80211_PARAM_MODE_11b;
			if(current_mode & IEEE80211_PARAM_MODE_11a)
				(*state_count_info)->current_wireless_mode = (*state_count_info)->current_wireless_mode | REPORT_IEEE80211_PARAM_MODE_11a;
			if(current_mode & IEEE80211_PARAM_MODE_11n_a)
				(*state_count_info)->current_wireless_mode = (*state_count_info)->current_wireless_mode | REPORT_IEEE80211_PARAM_MODE_11an;
			if(current_mode & IEEE80211_PARAM_MODE_11n_g)
				(*state_count_info)->current_wireless_mode = (*state_count_info)->current_wireless_mode | REPORT_IEEE80211_PARAM_MODE_11gn;
		}
		memset(cmd_str, 0, 128);
		sprintf(cmd_str,"iwpriv wifi%d getAMPDU | awk -F \":\" '{print $2}'", (*state_count_info)->radio_id-1);
		memset(str_tmp, 0, 64);
		popen_fgets(cmd_str, str_tmp, sizeof(str_tmp));
		(*state_count_info)->switch_AMPDU = atoi(str_tmp);

		int rx_signal_average_strength = 0;
		int rx_signal_max_strength = 0;
		int rx_signal_min_strength = 0;
		int count;
		count = interval_count_monitor_info->count;
		for(j = 0; j < count; j++)
		{
			rx_signal_average_strength += interval_count_monitor_info[j].rx_signal_average_strength;
			rx_signal_max_strength += interval_count_monitor_info[j].rx_signal_max_strength;
			rx_signal_min_strength += interval_count_monitor_info[j].rx_signal_min_strength;
		}
		if(count != 0)
		{
			(*state_count_info)->rx_signal_average_strength = rx_signal_average_strength/count;
			(*state_count_info)->rx_signal_max_strength = rx_signal_max_strength/count;
			(*state_count_info)->rx_signal_min_strength = rx_signal_min_strength/count;
		}

		int rx_bytes = 0, tx_bytes = 0;
		rx_bytes = interval_count_monitor_info[0].wireless_rx_interval_bytes[i];
		tx_bytes = interval_count_monitor_info[0].wireless_tx_interval_bytes[i];
		rx_bytes = rx_bytes/1024;
		tx_bytes = tx_bytes/1024;
		(*state_count_info)->wireless_rx_interval_bytes = rx_bytes/SAMPLING_INTERVAL;/* kBps */
		(*state_count_info)->wireless_tx_interval_bytes = tx_bytes/SAMPLING_INTERVAL;/* kBps */
		printf_d("(*state_count_info)->wireless_rx_interval_bytes == %d\n", (*state_count_info)->wireless_rx_interval_bytes);
		printf_d("(*state_count_info)->wireless_tx_interval_bytes == %d\n", (*state_count_info)->wireless_tx_interval_bytes);

		(*state_count_info)->wireless_if_updown_count = wireless_if_updown_count[(*state_count_info)->radio_id-1];
		(*state_count_info)->SSID_list_around = neighbor_SSID_get();
		/* init */
		(*state_count_info)->tx_data_package = 0;
		(*state_count_info)->rx_data_package = 0;
		(*state_count_info)->rx_data_package_bytes = 0;
		(*state_count_info)->tx_data_package_bytes = 0;
		(*state_count_info)->wireless_channel_down_retry_data_package = 0;	/* 无线信道下行重传的数据包数 */
		(*state_count_info)->rx_manage_frame = 0;							/* 接收的管理帧数 */
		(*state_count_info)->rx_ctrl_frame = 0;								/* 接收的控制帧数 */
		(*state_count_info)->rx_data_frame = 0;								/* 接收的数据帧数 */
		(*state_count_info)->tx_manage_frame = 0;							/* 发送的管理帧数 */
		(*state_count_info)->tx_ctrl_frame = 0;								/* 发送的控制帧数 */
		(*state_count_info)->tx_data_frame = 0;								/* 发送的数据帧数 */
		(*state_count_info)->wireless_channel_down_retry_frame= 0;			/* 无线信道下行重传的帧数 */
		(*state_count_info)->rx_error_data_package = 0;						/* 接收的错误数据包数 */
		(*state_count_info)->rx_error_frame = 0;								/* 接收的错帧数 */
		(*state_count_info)->tx_error_frame = 0;
		(*state_count_info)->rx_fcs_mac_error_frame = 0;						/* 接收的FCS MAC错帧数 */
		(*state_count_info)->rx_mic_mac_error_frame = 0;						/* 接收的MIC MAC错帧数 */
		(*state_count_info)->rx_dissect_failed_mac_error_frame = 0;			/* 接收的解密失败的MAC错帧数 */
		(*state_count_info)->tx_authenticate_frame = 0;						/* 发送的认证帧的数量 */
		(*state_count_info)->tx_associate_frame = 0;							/* 发送的关联帧的数量 */
		(*state_count_info)->rx_authenticate_frame = 0;						/* 接收的认证帧的数量 */
		(*state_count_info)->rx_associate_frame = 0;							/* 接收的关联帧的数量 */
		(*state_count_info)->tx_signaling_packet = 0;						/* 发送的信令包数 */
		(*state_count_info)->rx_signaling_packet = 0;						/* 接收的信令包数 */
		/* ...... */
		(*state_count_info)->tx_package = 0;									/* 发送的包数 */
		(*state_count_info)->rx_package = 0;									/* 接收的包数 */
		(*state_count_info)->tx_package_bytes = 0;						/* 发送的字节数 */
		(*state_count_info)->rx_package_bytes = 0;						/* 接收的字节数 */
		(*state_count_info)->rx_unicast = 0;
		(*state_count_info)->tx_unicast = 0;
		(*state_count_info)->rx_ununicast = 0;
		(*state_count_info)->tx_ununicast = 0;
		for(ptr = wlan_list; ptr != NULL; ptr = ptr->next)
		{
			if((*state_count_info)->radio_id == ptr->rid)
			{
				sprintf(wlanname,"ath.%d-%d", ptr->rid-1, ptr->wid);
				memset(cmd_str, 0, 128);
				sprintf(cmd_str,"iwpriv %s get_chwidth | awk -F \":\" '{print $2}'", wlanname);
				memset(str_tmp, 0, 64);
				popen_fgets(cmd_str, str_tmp, sizeof(str_tmp));
				(*state_count_info)->switch_20M_40M = atoi(str_tmp);

				memset(cmd_str, 0, 128);
				sprintf(cmd_str,"iwpriv %s get_shortgi | awk -F \":\" '{print $2}'", wlanname);
				memset(str_tmp, 0, 64);
				popen_fgets(cmd_str, str_tmp, sizeof(str_tmp));
				(*state_count_info)->switch_shortGI = atoi(str_tmp);

				memset(cmd_str, 0, 128);
				sprintf(cmd_str,"iwpriv %s get_puren | awk -F \":\" '{print $2}'", wlanname);
				memset(str_tmp, 0, 64);
				popen_fgets(cmd_str, str_tmp, sizeof(str_tmp));
				(*state_count_info)->switch_11n = atoi(str_tmp);/* get_puren=1 HT-Greefield, 11n true=1 | get_puren=0 HT-Mixed, 11n false=0 */
				
				get_wlan_throughput_fields(wlanname, wlanStatsInfo);
				(*state_count_info)->rx_data_package += wlanStatsInfo->rx_packets;
				(*state_count_info)->tx_data_package += wlanStatsInfo->tx_packets;
				(*state_count_info)->rx_data_frame += wlanStatsInfo->rx_packets;//same to rx_data_package
				(*state_count_info)->tx_data_frame += wlanStatsInfo->tx_packets;//same to tx_data_package
				(*state_count_info)->rx_data_package_bytes += wlanStatsInfo->rx_bytes;
				(*state_count_info)->tx_data_package_bytes += wlanStatsInfo->tx_bytes;
				
				(*state_count_info)->wireless_channel_down_retry_data_package += wlanStatsInfo->tx_retry;
				(*state_count_info)->wireless_channel_down_retry_frame += wlanStatsInfo->tx_retry;//same to wireless_channel_down_retry_data_package		
				(*state_count_info)->rx_error_data_package += wlanStatsInfo->rx_errors;
				(*state_count_info)->rx_error_frame += wlanStatsInfo->rx_errors;//rx_errors_frames;
				(*state_count_info)->tx_error_frame += wlanStatsInfo->tx_errors;
				(*state_count_info)->rx_unicast += wlanStatsInfo->rx_unicast;
				(*state_count_info)->tx_unicast += wlanStatsInfo->tx_unicast;
				(*state_count_info)->rx_ununicast += wlanStatsInfo->rx_broadcast+wlanStatsInfo->rx_multicast;
				(*state_count_info)->tx_ununicast += wlanStatsInfo->tx_broadcast+wlanStatsInfo->tx_multicast;
				
				get_wifiinfo_from_80211stats(wlanname, (*state_count_info));
				//get_stations_ctrl((*state_count_info), (*state_count_info)->radio_id-1, ptr->wid);

				(*state_count_info)->rx_ctrl_frame += wlanStatsInfo->rx_ctrl;
				(*state_count_info)->rx_ctrl_frame += wlanStatsInfo->tx_ctrl;
				printf_d("____ath -- wlanStatsInfo->rx_ctrl == %d____\n", wlanStatsInfo->rx_ctrl);
				printf_d("____ath -- wlanStatsInfo->tx_ctrl == %d____\n", wlanStatsInfo->tx_ctrl);
			}
		}
		
		memset(ifname, 0, 32);
		sprintf(ifname, "wifi%d", (*state_count_info)->radio_id-1);
		GetMACErrorInfo(ifname, (*state_count_info));

		(*state_count_info)->rx_package += wlanStatsInfo->rx_bytes + wlanStatsInfo->rx_ctrl
			+ (*state_count_info)->rx_manage_frame - wlanStatsInfo->rx_retry;
		(*state_count_info)->tx_package += wlanStatsInfo->rx_bytes + wlanStatsInfo->tx_ctrl
			+ (*state_count_info)->tx_manage_frame - wlanStatsInfo->tx_retry;
		(*state_count_info)->rx_package_all += (*state_count_info)->rx_package + (*state_count_info)->rx_signaling_packet
			+ (*state_count_info)->rx_manage_frame + (*state_count_info)->rx_signaling_packet;
		(*state_count_info)->tx_package_all += (*state_count_info)->tx_package + (*state_count_info)->tx_signaling_packet
			+ (*state_count_info)->tx_manage_frame + (*state_count_info)->tx_signaling_packet;
		(*state_count_info)->rx_package_bytes += wlanStatsInfo->rx_bytes
			+ (*state_count_info)->is_rx_mgmtbytes - wlanStatsInfo->rx_retry_bytes;//!!! no rx_ctl_bytes
		(*state_count_info)->tx_package_bytes += wlanStatsInfo->tx_bytes
			+ (*state_count_info)->is_tx_mgmtbytes - wlanStatsInfo->tx_retry_bytes;//!!! no tx_ctl_bytes

		(*state_count_info)->err_frame_ratio = 0;
		if(((*state_count_info)->rx_package_all+(*state_count_info)->tx_package_all) != 0)
			(*state_count_info)->err_frame_ratio = ((wlanStatsInfo->rx_errors+wlanStatsInfo->tx_errors)*100)/((*state_count_info)->rx_package_all+(*state_count_info)->tx_package_all);
		
		pthread_mutex_lock(&sta_mutex);
		STA_INFO *psta = NULL;
		(*state_count_info)->associate_sta = 0;
		for(psta = sta_list; psta != NULL; psta = psta->next)
			(*state_count_info)->associate_sta++;
		pthread_mutex_unlock(&sta_mutex);
	}
	FREE_OBJ(wlanStatsInfo);
	*state_count_info = head;
	pthread_mutex_unlock(&wlan_mutex);
	return TRUE;
}

BOOL get_sta_state_count_info(STA_STATE_COUNT_INFO **state_count_info)
{
	char wlanname[16];
	STA_INFO *ptr = NULL;
	int i = 0;
	int count = 0;
	
	pthread_mutex_lock(&sta_mutex);
	if(sta_list == NULL)
	{
		APVLog("there are no sta, nothing to report!\n");
		printf_d("there are no sta, nothing to report!\n");
		pthread_mutex_unlock(&sta_mutex);
		return FALSE;
	}
	for(ptr = sta_list; ptr!= NULL; ptr = ptr->next)
		count++;
	//*state_count_info = (STA_STATE_COUNT_INFO *)malloc(sizeof(STA_STATE_COUNT_INFO)*count);
	CREATE_ARRAY_ERR(*state_count_info,count,STA_STATE_COUNT_INFO,MALLOC_ERR_RET(FALSE));
	if(*state_count_info == NULL)
	{
		pthread_mutex_unlock(&sta_mutex);
		return RAISE_ERROR(ERROR_OUT_OF_MEMORY, NULL);
	}
	for(i = 0,ptr = sta_list; ptr != NULL; ptr = ptr->next)
	{
		(*state_count_info)[i].count = count;
		(*state_count_info)[i].radio_id = ptr->rid;
		(*state_count_info)[i].wlan_id = ptr->wid;
		(*state_count_info)[i].mac = (u8 *)malloc(MAC_ADDR_LEN);
		memcpy((*state_count_info)[i].mac, ptr->MAC, MAC_ADDR_LEN);
		sprintf(wlanname,"ath.%d-%d", ptr->rid-1, ptr->wid);	
		(*state_count_info)[i].IPv4 = ptr->ip;
		get_stations(wlanname, count, &(*state_count_info)[i]);
		i++;
	}
	get_sta_stats((*state_count_info), count);
	pthread_mutex_unlock(&sta_mutex);
	
	return TRUE;	
}

BOOL get_key_event_radio_frequency(AP_KEY_EVENT_INFO **ap_key_event, u8 rid, u8 state)
{
	AP_KEY_EVENT_INFO *ap_key_event_head = NULL;
	ap_key_event_head = *ap_key_event;

	if((*ap_key_event) == NULL)
	{
		//*ap_key_event = (AP_KEY_EVENT_INFO *)malloc(sizeof(AP_KEY_EVENT_INFO));
		CREATE_OBJECT_ERR(*ap_key_event,AP_KEY_EVENT_INFO,MALLOC_ERR_RET(FALSE));
		if((*ap_key_event) == NULL)
			return RAISE_ERROR(ERROR_OUT_OF_MEMORY, NULL);
		ap_key_event_head = *ap_key_event;
	}
	else
	{
		while((*ap_key_event)->next != NULL)
			*ap_key_event = (*ap_key_event)->next;
		//(*ap_key_event)->next = (AP_KEY_EVENT_INFO *)malloc(sizeof(AP_KEY_EVENT_INFO));
		CREATE_OBJECT_ERR((*ap_key_event)->next,AP_KEY_EVENT_INFO,MALLOC_ERR_RET(FALSE));
		if((*ap_key_event)->next == NULL)
			return RAISE_ERROR(ERROR_OUT_OF_MEMORY, NULL);
		*ap_key_event = (*ap_key_event)->next;
	}
	(*ap_key_event)->next = NULL;
	(*ap_key_event)->BSSID = NULL;
	(*ap_key_event)->radio_id = rid;
	(*ap_key_event)->radio_frequency_switch = state;
	(*ap_key_event)->switch_reason = 1;/* AC force delete */
	
	(*ap_key_event) = ap_key_event_head;
	return TRUE;
}


BOOL get_key_event_wlan_disable(AP_KEY_EVENT_INFO **ap_key_event, WLAN_INFO *wlan)
{
	AP_KEY_EVENT_INFO *ap_key_event_head = NULL;
	ap_key_event_head = *ap_key_event;

	if((*ap_key_event) == NULL)
	{
		//*ap_key_event = (AP_KEY_EVENT_INFO *)malloc(sizeof(AP_KEY_EVENT_INFO));
		CREATE_OBJECT_ERR(*ap_key_event,AP_KEY_EVENT_INFO,MALLOC_ERR_RET(FALSE));
		if((*ap_key_event) == NULL)
			return RAISE_ERROR(ERROR_OUT_OF_MEMORY, NULL);
		ap_key_event_head = *ap_key_event;
	}
	else
	{
		while((*ap_key_event)->next != NULL)
			*ap_key_event = (*ap_key_event)->next;
		//(*ap_key_event)->next = (AP_KEY_EVENT_INFO *)malloc(sizeof(AP_KEY_EVENT_INFO));
		CREATE_OBJECT_ERR((*ap_key_event)->next,AP_KEY_EVENT_INFO,MALLOC_ERR_RET(FALSE));
		if((*ap_key_event)->next == NULL)
			return RAISE_ERROR(ERROR_OUT_OF_MEMORY, NULL);
		*ap_key_event = (*ap_key_event)->next;
	}
	(*ap_key_event)->next = NULL;
	(*ap_key_event)->BSSID = NULL;
	(*ap_key_event)->radio_id = wlan->rid;
	(*ap_key_event)->wlan_id = wlan->wid;
	(*ap_key_event)->delete_reason = 1;/* AC force delete */
	
	(*ap_key_event) = ap_key_event_head;
	return TRUE;
}


BOOL get_key_event_wlan_enable(AP_KEY_EVENT_INFO **ap_key_event, WLAN_INFO *wlan)
{
	AP_KEY_EVENT_INFO *ap_key_event_head = NULL;
	ap_key_event_head = *ap_key_event;

	if((*ap_key_event) == NULL)
	{
		//*ap_key_event = (AP_KEY_EVENT_INFO *)malloc(sizeof(AP_KEY_EVENT_INFO));
		CREATE_OBJECT_ERR(*ap_key_event,AP_KEY_EVENT_INFO,MALLOC_ERR_RET(FALSE));
		if((*ap_key_event) == NULL)
			return RAISE_ERROR(ERROR_OUT_OF_MEMORY, NULL);
		ap_key_event_head = *ap_key_event;
	}
	else
	{
		while((*ap_key_event)->next != NULL)
			*ap_key_event = (*ap_key_event)->next;
		//(*ap_key_event)->next = (AP_KEY_EVENT_INFO *)malloc(sizeof(AP_KEY_EVENT_INFO));
		CREATE_OBJECT_ERR((*ap_key_event)->next,AP_KEY_EVENT_INFO,MALLOC_ERR_RET(FALSE));
		if((*ap_key_event)->next == NULL)
			return RAISE_ERROR(ERROR_OUT_OF_MEMORY, NULL);
		*ap_key_event = (*ap_key_event)->next;
	}
	(*ap_key_event)->next = NULL;
	(*ap_key_event)->BSSID = NULL;
	(*ap_key_event)->radio_id = wlan->rid;
	(*ap_key_event)->wlan_id = wlan->wid;
	(*ap_key_event)->BSSID = (char *)malloc(sizeof(wlan->BSSID));
	memcpy((*ap_key_event)->BSSID, wlan->BSSID, sizeof(wlan->BSSID));
	
	(*ap_key_event) = ap_key_event_head;
	return TRUE;
}

BOOL get_ap_eth_ability_info(AP_ETH_ABILITY_INFO **ability_info)
{
	char str_tmp[64];
	char cmd_str[128];
	char if_name[32];
	//GetInterfaceCount(&eth_count, "eth[0-9]");
	//*ability_info = (AP_ETH_ABILITY_INFO *)malloc(sizeof(AP_ETH_ABILITY_INFO));
	CREATE_OBJECT_ERR(*ability_info,AP_ETH_ABILITY_INFO,MALLOC_ERR_RET(FALSE));
	if(*ability_info == NULL)
		return RAISE_ERROR(ERROR_OUT_OF_MEMORY, NULL);

	memset(if_name, 0, 32);
	sprintf(if_name, "eth%d", 1);
	(*ability_info)->eth_id = 1;
	(*ability_info)->if_info = (char *)malloc(strlen(if_name)+1);
	memset((*ability_info)->if_info, 0, strlen(if_name)+1);
	memcpy((*ability_info)->if_info,if_name,strlen(if_name));
	(*ability_info)->if_type = MIB_IF_TYPE_ETHERNET;
#ifdef AR11N
	sprintf(if_name, "eth%d", 1);
#else
	sprintf(if_name, "eth%d", 0);
#endif
	memset(cmd_str, 0, 128);
	sprintf(cmd_str, "ifconfig | awk '/%s/,/MTU/'|grep MTU|awk -F \"MTU:\" '{print $2}'|awk -F \" \" '{print $1}'", if_name);
	memset(str_tmp, 0, 64);
	popen_fgets(cmd_str, str_tmp, sizeof(str_tmp));
	(*ability_info)->if_MTU = atoi(str_tmp);

	memset(cmd_str, 0, 128);
	sprintf(cmd_str, "get_hardinfo | awk -F \":\" '/up_speed/{print $2}' | awk -F \"M\" '{print $1}'");
	memset(str_tmp, 0, 64);
	popen_fgets(cmd_str, str_tmp, sizeof(str_tmp));
	(*ability_info)->if_bandwidth = atoi(str_tmp);

	//(*ability_info)->if_mac = malloc(MAC_ADDR_LEN);
	CREATE_ARRAY_ERR((*ability_info)->if_mac,MAC_ADDR_LEN,char,MALLOC_ERR_RET(FALSE));
	memset(cmd_str, 0, 128);
	
	sprintf(cmd_str, "ifconfig | grep %s | awk -F \" \" '{print $5}'",if_name);
	popen_fgets(cmd_str, str_tmp, sizeof(str_tmp));
	sscanf(str_tmp, "%02hhX:%02hhX:%02hhX:%02hhX:%02hhX:%02hhX", 
		   &((*ability_info)->if_mac[0]), &((*ability_info)->if_mac[1]), &((*ability_info)->if_mac[2]),
		   &((*ability_info)->if_mac[3]), &((*ability_info)->if_mac[4]), &((*ability_info)->if_mac[5]));

	return TRUE;
}

BOOL get_ap_eth_state_count_info(AP_ETH_STATE_COUNT_INFO **state_count_info)
{
	char wlanname[16];
	int ethid = 0;
	system("killall -9 linkcheck");
#ifdef AR11N
	ethid = 1;
#else
	ethid = 0;
#endif

	//GetInterfaceCount(&eth_count, "eth[0-9]");
	char cmdbuf[64];
	memset(cmdbuf,0,64);
	sprintf(cmdbuf,"/usr/sbin/linkcheck eth%d >/dev/null 2>&1 &",ethid);
	system(cmdbuf);

	//*state_count_info = (AP_ETH_STATE_COUNT_INFO *)malloc(sizeof(AP_ETH_STATE_COUNT_INFO));
	CREATE_OBJECT_ERR(*state_count_info,AP_ETH_STATE_COUNT_INFO,MALLOC_ERR_RET(FALSE));
	if(*state_count_info == NULL)
		return RAISE_ERROR(ERROR_OUT_OF_MEMORY, NULL);

	memset(wlanname, 0, 16);
	sprintf(wlanname, "eth%d", ethid);
	(*state_count_info)->eth_id = 1;
	get_eth_fields(wlanname, *state_count_info);
	GetEthUpdownTime(ethid, (int *)&(*state_count_info)->if_up_down_count);
	return TRUE;	
}

BOOL get_ap_ath_state_count_info(AP_ATH_STATE_COUNT_INFO **state_count_info)
{
	char wlanname[16];
	char if_name[16];
	char str_tmp[64];
	char cmd_str[128];
	WLAN_INFO *ptr = NULL;
	WLAN_INFO *ptr1 = NULL;
	AP_ATH_STATE_COUNT_INFO *head = NULL;

	wlan_stats_info * wlanStatsInfo = NULL;
	//wlanStatsInfo = (wlan_stats_info *)malloc(sizeof(wlan_stats_info));
	CREATE_OBJECT_ERR(wlanStatsInfo,wlan_stats_info,MALLOC_ERR_RET(FALSE));
	if(wlanStatsInfo == NULL)
		return RAISE_ERROR(ERROR_OUT_OF_MEMORY, NULL);
	pthread_mutex_lock(&wlan_mutex);
	ptr1 = wlan_list;
	if(wlan_list == NULL)
	{
		APVLog("there are no wlan, nothing to report!\n");
		printf_d("there are no wlan, nothing to report!\n");
		pthread_mutex_unlock(&wlan_mutex);
		FREE_OBJ(wlanStatsInfo);//diao add@20120929 for memory leak
		return FALSE;
	}
	for(ptr = wlan_list; ptr != NULL; ptr = ptr->next)
	{
		if((*state_count_info) == NULL)
		{
			//*state_count_info = (AP_ATH_STATE_COUNT_INFO *)malloc(sizeof(AP_ATH_STATE_COUNT_INFO));	
			CREATE_OBJECT_ERR(*state_count_info,AP_ATH_STATE_COUNT_INFO,MALLOC_ERR_RET(FALSE));
			(*state_count_info)->next = NULL;
			head = *state_count_info;
		}
		else
		{
			//(*state_count_info)->next = (AP_ATH_STATE_COUNT_INFO *)malloc(sizeof(AP_ATH_STATE_COUNT_INFO));
			CREATE_OBJECT_ERR((*state_count_info)->next,AP_ATH_STATE_COUNT_INFO,MALLOC_ERR_RET(FALSE));
			*state_count_info = (*state_count_info)->next;
			(*state_count_info)->next = NULL;
		}
		if(*state_count_info == NULL)
		{
			pthread_mutex_unlock(&wlan_mutex);
			FREE_OBJ(wlanStatsInfo);//diao add@20120929 for memory leak
			return RAISE_ERROR(ERROR_OUT_OF_MEMORY, NULL);
		}
		memset(*state_count_info, 0, sizeof(AP_ATH_STATE_COUNT_INFO));//diaowq add@20120927
		(*state_count_info)->radio_id = ptr->rid;
		(*state_count_info)->wlan_id = ptr->wid;

		(*state_count_info)->SSID = malloc(strlen(ptr->SSID)+1);
		memset((*state_count_info)->SSID, 0, strlen(ptr->SSID)+1);
		memcpy((*state_count_info)->SSID, ptr->SSID, strlen(ptr->SSID));
		
		sprintf(wlanname, "ath.%d-%d", ptr->rid-1, ptr->wid);
		GetInterfaceStatus(wlanname, &(*state_count_info)->SSID_switch);//!!! up:1 down:2

		(*state_count_info)->is_hide_SSID = ptr->hide_SSID;
	
		memset(cmd_str, 0, 128);
		sprintf(cmd_str, "iwpriv %s get_ap_bridge | awk -F \":\" '{print $2}'", wlanname);
		memset(str_tmp, 0, 64);
		popen_fgets(cmd_str, str_tmp, sizeof(str_tmp));
		(*state_count_info)->is_separate = atoi(str_tmp);
		
		if(ptr->security == SECURITY_802_11i)
		{
			(*state_count_info)->authorize_type_802_11 = OPEN_SYSTEM;
			if(ptr->sec_policy == OPEN)
			{
				(*state_count_info)->safe_type = SAFE_NONE;
				(*state_count_info)->authorize_mode = AUTHORIZE_NONE;
				(*state_count_info)->encryption_type = ENCRYPTION_NONE;
			}
			else if(ptr->sec_policy == WEP)
			{
				(*state_count_info)->safe_type = SAFE_NONE;
				(*state_count_info)->authorize_mode = AUTHORIZE_NONE;/* !!! */
				if(ptr->sec_wep.bits == BIT64)
					(*state_count_info)->encryption_type = ENCRYPTION_WEP_40;
				else if(ptr->sec_wep.bits == BIT128)
					(*state_count_info)->encryption_type = ENCRYPTION_WEP_104;
			}
			else if(ptr->sec_policy == EAP_WPA_WPA2)
			{
				(*state_count_info)->safe_type = SAFE_WPA;
				(*state_count_info)->authorize_mode = AUTHORIZE_EAP;
				if(ptr->sec_wpa.mode == CCMP)
					(*state_count_info)->encryption_type = ENCRYPTION_AES_CCMP;
				else if(ptr->sec_wpa.mode == TKIP)
					(*state_count_info)->encryption_type = ENCRYPTION_TKIP;
			}
			else if(ptr->sec_policy == PSK_WPA_WPA2)
			{
				(*state_count_info)->safe_type = SAFE_WPA;
				(*state_count_info)->authorize_mode = AUTHORIZE_PSK;
				if(ptr->sec_wpa.mode == CCMP)
					(*state_count_info)->encryption_type = ENCRYPTION_AES_CCMP;
				else if(ptr->sec_wpa.mode == TKIP)
					(*state_count_info)->encryption_type = ENCRYPTION_TKIP;
			}
		}
		else if(ptr->security == SECURITY_WAPI)
		{
			(*state_count_info)->authorize_type_802_11 = SHARED_KEY;
			(*state_count_info)->safe_type = SAFE_WAPI;
			if(ptr->sec_policy == WAPI_OPEN)
			{
				(*state_count_info)->authorize_mode = AUTHORIZE_NONE;
				(*state_count_info)->encryption_type = ENCRYPTION_NONE;
			}
			else if(ptr->sec_policy == WAPI_PSK)
			{
				(*state_count_info)->authorize_mode = AUTHORIZE_PSK;
				if(ptr->sec_wapi.encryption_type == 0)
					(*state_count_info)->encryption_type = ENCRYPTION_WPI_SMS4;
			}
			else if(ptr->sec_policy == WAPI_CER)
			{
				(*state_count_info)->authorize_mode = AUTHORIZE_WAPI_CER;
				if(ptr->sec_wapi.encryption_type == 0)
					(*state_count_info)->encryption_type = ENCRYPTION_WPI_SMS4;
			}
		}
		
		(*state_count_info)->vlan_mark = ptr->vid;
		(*state_count_info)->max_sta_num = ptr->max_users;
		get_sta_max_rate(wlanname, (*state_count_info));

		get_bssid((*state_count_info)->radio_id-1, (*state_count_info)->wlan_id, (*state_count_info)->BSSID);
		/* Begin: Modified by dongzw for fix ap report ath state count information error 2013-2-16 */
		//sprintf(if_name, "wifi%d", ptr->rid-1);
		//GetExtraInfo(if_name, (*state_count_info));
		get_wlan_throughput_fields(wlanname, wlanStatsInfo);

		(*state_count_info)->tx_packet = wlanStatsInfo->tx_packets;
		(*state_count_info)->rx_packet = wlanStatsInfo->rx_packets;
		(*state_count_info)->tx_bytes = wlanStatsInfo->tx_bytes;
		(*state_count_info)->rx_bytes = wlanStatsInfo->rx_bytes;
		/* End: Modified by dongzw for fix ap report ath state count information error 2013-2-16 */
		
		(*state_count_info)->rx_data_frame = wlanStatsInfo->rx_packets;
		(*state_count_info)->tx_data_frame = wlanStatsInfo->tx_packets;
		
		(*state_count_info)->downlink_retry_packet = wlanStatsInfo->tx_retry;
		(*state_count_info)->rx_errors = wlanStatsInfo->rx_errors;
		printf_d("(*state_count_info)->rx_errors = %d\n", (*state_count_info)->rx_errors);	
		get_athinfo_from_80211stats(wlanname, (*state_count_info));

		(*state_count_info)->err_packet_ratio = 0;
		if((wlanStatsInfo->rx_packets + wlanStatsInfo->tx_packets) != 0)
		{
			(*state_count_info)->err_packet_ratio = ((wlanStatsInfo->rx_errors+wlanStatsInfo->tx_errors)*100)
				/(wlanStatsInfo->rx_packets + wlanStatsInfo->tx_packets);
		}
		//(*state_count_info->err_bytes_ratio = (wlanStatsInfo->rx_errors)*100/(wlanStatsInfo->rx_bytes);

		(*state_count_info)->rx_ctrl_frame += wlanStatsInfo->rx_ctrl;
		(*state_count_info)->rx_ctrl_frame += wlanStatsInfo->tx_ctrl;

		(*state_count_info)->rx_interval_bytes = ptr->rx_interval_bytes/1024;/* kBps */
		(*state_count_info)->tx_interval_bytes = ptr->tx_interval_bytes/1024;/* kBps */
		(*state_count_info)->rx_interval_bytes = (*state_count_info)->rx_interval_bytes/SAMPLING_INTERVAL;
		(*state_count_info)->tx_interval_bytes = (*state_count_info)->tx_interval_bytes/SAMPLING_INTERVAL;
	}
	*state_count_info = head;
	pthread_mutex_unlock(&wlan_mutex);
	FREE_OBJ(wlanStatsInfo);//diao add@20120929 for memory leak
	return TRUE;	
}


/* ------------------------------------------------------------------------------------------------------------------ */
/* parse messages */
BOOL parse_messages_header(TMSG *msg, MSG_HEADER *msgh)
{
	msgh->type = protocol_retrieve32(msg);
	printf_d("msgh->type:%d\n", msgh->type);
	msgh->len  = protocol_retrieve32(msg);
	return TRUE;
}

BOOL free_wlan_info(WLAN_INFO *wlan)
{
	if(wlan->ret == FAILED)
	{
		if(wlan->security == SECURITY_WAPI)
		{
			switch(wlan->sec_policy)
			{
				case WAPI_PSK:
					FREE_OBJ(wlan->sec_wapi.key.key);
					break;
				default:
					break;
			}
		}
		else if(wlan->security == SECURITY_802_11i)
		{
			switch(wlan->sec_policy)
			{
				case WEP:
					FREE_OBJ(wlan->sec_wep.wep_key[0].key);
					FREE_OBJ(wlan->sec_wep.wep_key[1].key);
					FREE_OBJ(wlan->sec_wep.wep_key[2].key);
					FREE_OBJ(wlan->sec_wep.wep_key[3].key);
					break;
				case PSK_WPA:
				case PSK_WPA2:
				case EAP_WPA:
				case EAP_WPA2:
					FREE_OBJ(wlan->sec_wpa.wpa_key.key);
					break;
				default:
					break;
			}
		}
		FREE_OBJ(wlan->SSID);
	}
	return TRUE;
}
BOOL parse_security_wapi(TMSG *msg, WLAN_INFO *wlan)
{
	switch(wlan->sec_policy)
	{
		case WAPI_OPEN://open
			break;
		case WAPI_PSK://wapi-psk
			wlan->sec_wapi.encryption_type	= protocol_retrieve8(msg);
			wlan->sec_wapi.key_t         	= protocol_retrieve8(msg);
			wlan->sec_wapi.key.len   	= protocol_retrieve8(msg);
			wlan->sec_wapi.key.key  	= (u8 *)protocol_retrieve_raw_bytes(msg, wlan->sec_wpa.wpa_key.len);
			break;
		case WAPI_CER://Certificate
			wlan->sec_wapi.encryption_type	= protocol_retrieve8(msg);
			break;
		default:
			break;
	}
	return TRUE;
}

BOOL parse_security_11i(TMSG *msg, WLAN_INFO *wlan)
{
	switch(wlan->sec_policy)
	{
		case OPEN:
			break;
		case WEP:
			wlan->sec_wep.bits           = protocol_retrieve8(msg);
			wlan->sec_wep.key_t          = protocol_retrieve8(msg);
			wlan->sec_wep.index          = protocol_retrieve8(msg);
			if(wlan->sec_wep.key_t == ASSCII)
			{
				wlan->sec_wep.wep_key[0].len = protocol_retrieve8(msg);
				wlan->sec_wep.wep_key[0].key = (u8 *)protocol_retrieve_str(msg, wlan->sec_wep.wep_key[0].len);
				wlan->sec_wep.wep_key[1].len = protocol_retrieve8(msg);
				wlan->sec_wep.wep_key[1].key = (u8 *)protocol_retrieve_str(msg, wlan->sec_wep.wep_key[1].len);
				wlan->sec_wep.wep_key[2].len = protocol_retrieve8(msg);
				wlan->sec_wep.wep_key[2].key = (u8 *)protocol_retrieve_str(msg, wlan->sec_wep.wep_key[2].len);
				wlan->sec_wep.wep_key[3].len = protocol_retrieve8(msg);
				wlan->sec_wep.wep_key[3].key = (u8 *)protocol_retrieve_str(msg, wlan->sec_wep.wep_key[3].len);
			}
			else if(wlan->sec_wep.key_t == HEX)
			{
				wlan->sec_wep.wep_key[0].len = protocol_retrieve8(msg);
				wlan->sec_wep.wep_key[0].key = (u8 *)protocol_retrieve_hex_to_str(msg, wlan->sec_wep.wep_key[0].len);
				wlan->sec_wep.wep_key[1].len = protocol_retrieve8(msg);
				wlan->sec_wep.wep_key[1].key = (u8 *)protocol_retrieve_hex_to_str(msg, wlan->sec_wep.wep_key[1].len);
				wlan->sec_wep.wep_key[2].len = protocol_retrieve8(msg);
				wlan->sec_wep.wep_key[2].key = (u8 *)protocol_retrieve_hex_to_str(msg, wlan->sec_wep.wep_key[2].len);
				wlan->sec_wep.wep_key[3].len = protocol_retrieve8(msg);
				wlan->sec_wep.wep_key[3].key = (u8 *)protocol_retrieve_hex_to_str(msg, wlan->sec_wep.wep_key[3].len);
			}
			else
			{
				wlan->sec_wep.wep_key[0].len = protocol_retrieve8(msg);
				wlan->sec_wep.wep_key[0].key = (u8 *)protocol_retrieve_str(msg, wlan->sec_wep.wep_key[0].len);
				wlan->sec_wep.wep_key[1].len = protocol_retrieve8(msg);
				wlan->sec_wep.wep_key[1].key = (u8 *)protocol_retrieve_str(msg, wlan->sec_wep.wep_key[1].len);
				wlan->sec_wep.wep_key[2].len = protocol_retrieve8(msg);
				wlan->sec_wep.wep_key[2].key = (u8 *)protocol_retrieve_str(msg, wlan->sec_wep.wep_key[2].len);
				wlan->sec_wep.wep_key[3].len = protocol_retrieve8(msg);
				wlan->sec_wep.wep_key[3].key = (u8 *)protocol_retrieve_str(msg, wlan->sec_wep.wep_key[3].len);
			}
			break;
		case EAP_WPA_WPA2:
		case EAP_WPA:
		case EAP_WPA2:
			wlan->sec_wpa.mode          = protocol_retrieve8(msg);
			break;
		case PSK_WPA_WPA2:
			wlan->sec_wpa.mode          = protocol_retrieve8(msg);
			wlan->sec_wpa.key_t         = protocol_retrieve8(msg);
			wlan->sec_wpa.wpa_key.len   = protocol_retrieve8(msg);
			wlan->sec_wpa.wpa_key.key   = (u8 *)protocol_retrieve_str(msg, wlan->sec_wpa.wpa_key.len);
			break;
		case PSK_WPA:
		case PSK_WPA2:
			wlan->sec_wpa.mode          = protocol_retrieve8(msg);
			wlan->sec_wpa.wpa_key.len   = protocol_retrieve8(msg);
			wlan->sec_wpa.wpa_key.key   = (u8 *)protocol_retrieve_str(msg, wlan->sec_wpa.wpa_key.len);
			break;
		case Certificate:
			break;
		default:
			printf_d("unrecongnized security policy\n");
			APVLog("unrecongnized security policy\n");
			break;
	}
	return TRUE;
}
/*pei add for wapid ioctl. 090505*/
/*向driver发送消息,set key ,set wapi */
int  wapid_ioctl(char *ifname, unsigned short cmd, char *buf, int buf_len)
{
	int result = 0;
	int sock;
	struct iwreq  wreq;

	sock = socket(AF_INET, SOCK_DGRAM, 0);
	memset(&wreq, 0, sizeof(struct iwreq));
	strcpy(wreq.ifr_ifrn.ifrn_name, ifname);
	if (buf_len < IFNAMSIZ) {
		memcpy(wreq.u.name, buf, buf_len);
	} else {
		wreq.u.data.pointer = (caddr_t)buf;
		wreq.u.data.length =  buf_len;
	}

	APVLog("cmd: %x\n", cmd);
	printf_d("cmd: %x\n", cmd);
	result = ioctl(sock, cmd, &wreq);
	
	if (result < 0) 
	{
		fprintf(stderr, "in %s:%d return(%d) :  \"%s\"........... \n", __func__, __LINE__, (result),strerror(errno));
	}
	close(sock);
	
	return result;
}

/****************************************************************
 *
 *
 *
 * ************************************************************/
int check_interface_state(char * ifname)
{
	int sockfd;
	struct ifreq ifr;
	
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	
	strncpy(ifr.ifr_name,ifname, sizeof(ifr.ifr_name));	
	
	if(ioctl(sockfd, SIOCGIFINDEX, &ifr) == -1){
		close(sockfd);
		return -1;
	 }
	close(sockfd);
	return 0;
}

int code_convert(char *from_charset,char *to_charset,char *inbuf,int inlen,char *outbuf,int outlen) 
{ 
	iconv_t cd; 
	char **pin = &inbuf; 
	char **pout = &outbuf; 
	cd = iconv_open(to_charset,from_charset); 
	if (cd==0) return -1; 
	memset(outbuf,0,outlen); 
	if (iconv(cd,pin,&inlen,pout,&outlen)==-1) return -1; 
	iconv_close(cd); 
	return 0; 
} 
  //UNICODE码转为GB2312码 
  int u2g(char *inbuf,int inlen,char *outbuf,int outlen) 
  { 
 	 return code_convert("utf-8","gb2312",inbuf,inlen,outbuf,outlen); 
  } 
  //GB2312码转为UNICODE码 
  int g2u(char *inbuf,size_t inlen,char *outbuf,size_t outlen) 
  { 
	 return code_convert("gb2312","utf-8",inbuf,inlen,outbuf,outlen); 
  } 

/* parse add wlan message */
BOOL parse_add_wlan_requset_msg(TMSG *msg)
{
	WLAN_INFO *wlanconf = NULL;
	WLAN_INFO *ptr = NULL;
	wlanconf = (WLAN_INFO *)malloc(sizeof(WLAN_INFO));

	if(wlanconf == NULL)
		return RAISE_ERROR(ERROR_OUT_OF_MEMORY, NULL);
	memset(wlanconf, 0, sizeof(WLAN_INFO));
	ptr = wlanconf;
	wlanconf->next = NULL;
	
	u32 wlan_count = 0;
	#if 0
	wlan.sec_wep.wepkey[0].key = NULL;
	wlan.sec_wep.wepkey[1].key = NULL;
	wlan.sec_wep.wepkey[2].key = NULL;
	wlan.sec_wep.wepkey[3].key = NULL;
	wlan.sec_psk.psk_key.key   = NULL;
	wlan.SSID                  = NULL;
	#endif
	while(msg->offset < msg->len)
	{
		wlanconf->rid					= protocol_retrieve8(msg);
		wlanconf->wid					= protocol_retrieve8(msg);
		wlanconf->hide_SSID				= protocol_retrieve8(msg);
		wlanconf->security				= protocol_retrieve8(msg);
		wlanconf->sec_policy			= protocol_retrieve8(msg);
		wlanconf->max_users				= protocol_retrieve8(msg);
		wlanconf->Qos_switch			= protocol_retrieve8(msg);
		wlanconf->mac_percolate_rule	= protocol_retrieve8(msg);
		wlanconf->ability_info			= protocol_retrieve16(msg);
		wlanconf->vid  	          		= protocol_retrieve16(msg);
		wlanconf->mac_mode				= protocol_retrieve8(msg);
		wlanconf->data_packet_method	= protocol_retrieve8(msg);
		wlanconf->SSID_flow_limit = protocol_retrieve32(msg);
		wlanconf->user_flow_limit = protocol_retrieve32(msg);
		wlanconf->SSID_len        = protocol_retrieve8(msg);
		wlanconf->SSID            = protocol_retrieve_str(msg, wlanconf->SSID_len);
		APVLog("%s,%d,create wlan essid,wlanconf->SSID:%s, ssid_len:%d\n", __func__, __LINE__, wlanconf->SSID, wlanconf->SSID_len);

		//convert utf8 to gb2312
		u8 ssid[128]={0};
		u2g(wlanconf->SSID,wlanconf->SSID_len,ssid,sizeof(ssid));
		wlanconf->SSID_len = strlen(ssid);
		FREE_OBJ(wlanconf->SSID);
		CREATE_STRING_FROM_STRING_ERR(wlanconf->SSID,ssid,{return FALSE;});
		APVLog("%s,%d,create wlan essid,after converted,wlanconf->SSID:%s, ssid_len:%d\n", __func__, __LINE__, wlanconf->SSID, wlanconf->SSID_len);
		
		if(wlanconf->security == SECURITY_WAPI)
			parse_security_wapi(msg, wlanconf);
		else if(wlanconf->security == SECURITY_802_11i)
			parse_security_11i(msg, wlanconf);
		else
		{
			while(ptr != NULL)
			{
				
				wlanconf = ptr;
				ptr = ptr->next;
				free_wlan_info(wlanconf);
				FREE_OBJ(wlanconf);
			}		
			RAISE_SYS_ERR(ERROR_INVALID_FORMAT);
		}
		
		wlan_count++;
		if(wlan_count >= MAX_WLAN_NUM)
		{
			while(ptr != NULL)
			{
				wlanconf = ptr;
				free_wlan_info(wlanconf);
				ptr = ptr->next;
				FREE_OBJ(wlanconf);
			}
			RAISE_SYS_ERR(ERROR_INVALID_FORMAT);
		}
		if(msg->offset < msg->len)
		{
			//wlanconf->next = (WLAN_INFO *)malloc(sizeof(WLAN_INFO));
			CREATE_OBJECT_ERR(wlanconf->next,WLAN_INFO,MALLOC_ERR_RET(FALSE));
			wlanconf->next->next = NULL;
			wlanconf = wlanconf->next;
		}
	}
	
	wlanconf = ptr;
	
	if(msg->offset != msg->len)
	{
		while(ptr != NULL)
		{
			wlanconf = ptr;
			free_wlan_info(wlanconf);
			ptr = ptr->next;
			FREE_OBJ(wlanconf);
		}
		RAISE_SYS_ERR(ERROR_INVALID_FORMAT);
	}
	return add_cmd_to_list_tail(MSG_ELEMENT_ADD_WLAN_REQUEST_TYPE, wlanconf);
}

BOOL copy_wlanconf_to_sencond_wifi(WLAN_INFO *src,WLAN_INFO **wlanconf)
{

	WLAN_INFO *p1		 = NULL,
			  *temp		 = NULL,
			  *head		 = NULL;
	int i = 0;
	if(src==NULL || wlanconf==NULL)
		return FALSE;

	while(src != NULL)
	{
		p1 = (WLAN_INFO *)malloc(sizeof(WLAN_INFO));
		if(p1==NULL)
			return RAISE_ERROR(ERROR_OUT_OF_MEMORY, NULL);
		memset(p1,0,sizeof(WLAN_INFO));	
		memcpy(p1,src,sizeof(WLAN_INFO));
		p1->SSID = (char *)malloc(src->SSID_len+1);
		if(p1->SSID==NULL)
		{
			return RAISE_ERROR(ERROR_OUT_OF_MEMORY, NULL);
		}
		memset(p1->SSID,0,src->SSID_len+1);
		memcpy(p1->SSID,src->SSID,src->SSID_len);

		if(src->sec_wapi.key.key!=NULL)
		{
			p1->sec_wapi.key.key = (u8*)malloc(src->sec_wapi.key.len+1);
			if(p1->sec_wapi.key.key==NULL)
			{	
				FREE_OBJ(p1->SSID);
				return RAISE_ERROR(ERROR_OUT_OF_MEMORY, NULL);
			}
			memset(p1->sec_wapi.key.key ,0,src->sec_wapi.key.len+ 1);
			memcpy(p1->sec_wapi.key.key,src->sec_wapi.key.key,src->sec_wapi.key.len);
		}
		if(src->sec_wep.wep_key[0].key!=NULL)
		{
			for(i=0;i<MAX_WEP_KEY;i++)
			{
				p1->sec_wep.wep_key[i].key=(u8 *)malloc(src->sec_wep.wep_key[i].len + 1);
				if(p1->sec_wep.wep_key[i].key==NULL)
				{
					FREE_OBJ(p1->SSID);
					FREE_OBJ(p1->sec_wapi.key.key );
					return RAISE_ERROR(ERROR_OUT_OF_MEMORY, NULL);
				}
				memset(p1->sec_wep.wep_key[i].key,0,src->sec_wep.wep_key[i].len + 1);
				memcpy(p1->sec_wep.wep_key[i].key,src->sec_wep.wep_key[i].key,src->sec_wep.wep_key[i].len);
			}
		}
		if(src->sec_wpa.wpa_key.key!=NULL)
		{
			p1->sec_wpa.wpa_key.key = (u8 *)malloc(src->sec_wpa.wpa_key.len + 1);
			if(p1->sec_wpa.wpa_key.key==NULL)
			{	
				FREE_OBJ(p1->SSID);
				FREE_OBJ(p1->sec_wapi.key.key);
				for(i=0;i<MAX_WEP_KEY;i++)
				{
					FREE_OBJ(p1->sec_wep.wep_key[i].key);
				}
				return RAISE_ERROR(ERROR_OUT_OF_MEMORY, NULL);
			}
			memset(p1->sec_wpa.wpa_key.key,0,src->sec_wpa.wpa_key.len + 1);
			memcpy(p1->sec_wpa.wpa_key.key,src->sec_wpa.wpa_key.key,src->sec_wpa.wpa_key.len);
		}
		
		p1->rid = 2;
		p1->next = NULL;

		if(head==NULL)
		{
			head = p1;
			temp = head;
		}
		else
		{
			temp->next=p1;
			temp = p1;
		}
			
		src = src->next;
 
	}

	*wlanconf = head;
	return TRUE;
}

/*
void kill_hostapd(char *filename)
{
	char cmd[256] = {0};	
	sprintf(cmd, "ps | grep \"hostapd\" | awk -F \" \" '/_%s /{print $1}' | xargs kill -9", filename);
	SYSTEM(cmd);
	printf("kill_hostapd==================\n");
}
*/

void kill_hostapd_by_wlanname(char *wlanname)
{
	char cmd[256] = {0};
	/*
	  * Behind "%s" have a space, the space is to avoid filter error.
	  * For example, "ath.0-1"  & "ath.0-11".
	  */
	sprintf(cmd, "ps | grep \"hostapd\" | awk -F \" \" '/_%s /{print $1}' | xargs kill -9", wlanname);
	SYSTEM(cmd);

}

/*
void kill_hostapd_by_wlan(WLAN_INFO *wlan)
{
	char wlanname[64] = {0};
	//char filename[64] = {0};	

	sprintf(wlanname, "ath.%d-%d", wlan->rid-1, wlan->wid);
	kill_hostapd_by_wlanname(wlanname);
	
	if(wlan->sec_policy == EAP_WPA_WPA2)
		snprintf(filename,sizeof(filename), "eap_%s", wlanname);
	else if(wlan->sec_policy == EAP_WPA)
		snprintf(filename,sizeof(filename), "eap_wpa_%s", wlanname);
	else if(wlan->sec_policy == EAP_WPA2)
		snprintf(filename,sizeof(filename), "eap_wpa2_%s", wlanname);
	else if(wlan->sec_policy == PSK_WPA_WPA2)
		snprintf(filename,sizeof(filename), "psk_%s", wlanname);
	else if(wlan->sec_policy == PSK_WPA)
		snprintf(filename,sizeof(filename), "psk_wpa_%s", wlanname);
	else if(wlan->sec_policy == PSK_WPA2)
		snprintf(filename,sizeof(filename), "psk_wpa2_%s", wlanname);	
	else
		return;
	//kill_hostapd(filename);
	
	return;
}
*/

BOOL set_sta_mac_addr_filtration(u8 rid, u8 wid)
{
	char cmd[128];
	memset(cmd, 0, 128);
	macAddrFiltration *ptr = NULL;
	if(StaBlackList == NULL)
		return TRUE;
	for(ptr = StaBlackList; ptr != NULL; ptr = ptr->next)
	{
		snprintf(cmd, sizeof(cmd), "iwpriv ath.%d-%d addmac %02X:%02X:%02X:%02X:%02X:%02X", rid-1, wid, ptr->mac[0], ptr->mac[1], ptr->mac[2], ptr->mac[3], ptr->mac[4], ptr->mac[5]);
		SYSTEM(cmd);
	}
	return TRUE;
}

BOOL set_security_mode_wapi(WLAN_INFO *wlan)
{
	char wlanname[IFNAMSIZ];
	sprintf(wlanname,"ath.%d-%d", wlan->rid-1, wlan->wid);
	if(wlan->sec_policy == WAPI_PSK)
	{  //pei add 090309
		APVLog("wapi-psk mode\n");
		printf_d("wapi-psk mode\n");
		int res = 0;
		int param[2];
		param[0] = P80211_PACKET_WAPIFLAG;
		param[1] = 0x07;
		APVLog("sizeof(struct ioctl_drv):%u\n", sizeof(struct ioctl_drv));
		printf_d("sizeof(struct ioctl_drv):%u\n", sizeof(struct ioctl_drv));
		res=wapid_ioctl(wlanname, P80211_IOCTL_SETWAPI_INFO, (char *)&param, 2*sizeof(int));
	}
	else if(wlan->sec_policy == WAPI_CER)
	{  //pei add 090309
		APVLog("wapi-cer mode\n");
		printf_d("wapi-cer mode\n");
		
	/*	struct ioctl_drv ioctl_drv_data;  */
		int res = 0;
		int param[2];
		param[0] = P80211_PACKET_WAPIFLAG;
		param[1] = 0x0b;
		res=wapid_ioctl(wlanname, P80211_IOCTL_SETWAPI_INFO, (char *)&param, 2*sizeof(int));
	}
	return TRUE;
}
BOOL set_security_mode(WLAN_INFO *wlan)
{
	char wlanname[32] = {0};
	char filename[64] = {0};
	char bridgename[16] = {0};
	char cmd[512] = {0};
	char ip[32] = {0};
	FILE *ofp = NULL;
	RADIUS_CONF *radius = NULL;
	radius = radius_list;
	sprintf(wlanname, "ath.%d-%d", wlan->rid-1, wlan->wid);
	sprintf(bridgename, "br%d", wlan->vid);
	/* set default values*/
	snprintf(cmd, sizeof(cmd), "ifconfig %s down", wlanname);
	SYSTEM(cmd);
	/*
	if(wlan->sec_policy == EAP_WPA_WPA2)
		snprintf(filename,sizeof(filename), "eap_%s", wlanname);	
	else if(wlan->sec_policy == EAP_WPA)
		snprintf(filename,sizeof(filename), "eap_wpa_%s", wlanname);		
	else if(wlan->sec_policy == EAP_WPA2)
		snprintf(filename,sizeof(filename), "eap_wpa2_%s", wlanname);
	else if(wlan->sec_policy == PSK_WPA_WPA2)
		snprintf(filename,sizeof(filename), "psk_%s", wlanname);
	else if(wlan->sec_policy == PSK_WPA)
		snprintf(filename,sizeof(filename), "psk_wpa_%s", wlanname);
	else if(wlan->sec_policy == PSK_WPA2)
		snprintf(filename,sizeof(filename), "psk_wpa2_%s", wlanname);		
	else if(wlan->sec_policy == WEP)
		;
	else
	{
		goto quit;
	}
	kill_hostapd(filename);
	*/
	
	kill_hostapd_by_wlanname(wlanname);
	if (wlan->sec_policy == OPEN)
		goto quit;
	snprintf(cmd, sizeof(cmd), "iwconfig %s key off", wlanname);
	SYSTEM(cmd);
	snprintf(cmd, sizeof(cmd), "iwpriv %s authmode 1", wlanname);
	SYSTEM(cmd);
	/* */
	if((wlan->sec_policy == EAP_WPA_WPA2)||(wlan->sec_policy == EAP_WPA)||(wlan->sec_policy == EAP_WPA2))  /* WPA/WPA2 EAP */
	{
		if(wlan->sec_policy == EAP_WPA_WPA2)
			snprintf(filename,sizeof(filename), "%s/eap_%s", HOSTAPD_PROFILE_PATH, wlanname);
		else if(wlan->sec_policy == EAP_WPA)
			snprintf(filename,sizeof(filename), "%s/eap_wpa_%s", HOSTAPD_PROFILE_PATH, wlanname);
		else if(wlan->sec_policy == EAP_WPA2)
			snprintf(filename,sizeof(filename), "%s/eap_wpa2_%s", HOSTAPD_PROFILE_PATH, wlanname);
		ofp = fopen(filename, "w");
		if(ofp == NULL)
			goto err;
		fprintf(ofp, "interface=%s\n", wlanname); 
		fprintf(ofp, "bridge=%s\n", wlan->data_packet_method==LOCAL_BRIDGE?bridgename:"default"); 
		#ifdef AR11N
		fprintf(ofp, "driver=%s\n", "madwifi"); 
		#endif
		fprintf(ofp, "logger_syslog=%d\n", -1); 
		fprintf(ofp, "logger_syslog_level=%d\n", 2); 
		fprintf(ofp, "logger_stdout=%d\n", -1); 
		fprintf(ofp, "logger_stdout_level=%d\n", 2); 
		//fprintf(ofp, "debug=%d\n", 0); 
		fprintf(ofp, "dump_file=%s\n", "/tmp/hostapd.dump"); 
		fprintf(ofp, "ssid=%s\n", wlan->SSID);
		fprintf(ofp, "ieee8021x=%d\n", 1);             /* */
		fprintf(ofp, "eapol_key_index_workaround=%d\n", 0);
		fprintf(ofp, "own_ip_addr=%s\n", hostip);
		if(radius_list == NULL)
		{
			fprintf(ofp, "auth_server_addr=%s\n", "192.168.1.1");
			fprintf(ofp, "auth_server_port=%d\n", 1812);
			fprintf(ofp, "auth_server_shared_secret=%s\n", "test");
			fprintf(ofp, "acct_server_addr=%s\n", "192.168.1.1");
			fprintf(ofp, "acct_server_port=%d\n", 1813);
			fprintf(ofp, "acct_server_shared_secret=%s\n", "test");
			fprintf(ofp, "auth_server_addr=%s\n", "192.168.1.1");
			fprintf(ofp, "auth_server_port=%d\n", 1812);
			fprintf(ofp, "auth_server_shared_secret=%s\n", "test");
			fprintf(ofp, "acct_server_addr=%s\n", "192.168.1.1");
			fprintf(ofp, "acct_server_port=%d\n", 1813);
			fprintf(ofp, "acct_server_shared_secret=%s\n", "test");
		}
		else
		{
			while(radius != NULL)
			{
				inet_ntop(AF_INET, &(radius->ipaddr), ip, sizeof(ip));
				if(radius->type == 1)
				{
					fprintf(ofp, "auth_server_addr=%s\n", ip);
					fprintf(ofp, "auth_server_port=%d\n", radius->port);
					fprintf(ofp, "auth_server_shared_secret=%s\n", radius->pwd);
				}
				else if(radius->type == 2)
				{
					inet_ntop(AF_INET, &(radius->ipaddr), ip, sizeof(ip));
					fprintf(ofp, "acct_server_addr=%s\n", ip);
					fprintf(ofp, "acct_server_port=%d\n", radius->port);
					fprintf(ofp, "acct_server_shared_secret=%s\n", radius->pwd);
				}
				radius = radius->next;
				#if 0
				fprintf(ofp, "auth_server_addr=%s\n", "192.168.1.1");
				fprintf(ofp, "auth_server_port=%d\n", 1812);
				fprintf(ofp, "auth_server_shared_secret=%s\n", "test");
				fprintf(ofp, "acct_server_addr=%s\n", "192.168.1.1");
				fprintf(ofp, "acct_server_port=%d\n", 1813);
				fprintf(ofp, "acct_server_shared_secret=%s\n", "test");
				#endif
			}
		}
		
		if(wlan->sec_policy == EAP_WPA)
			fprintf(ofp, "wpa=%d\n", 1);
		else if(wlan->sec_policy == EAP_WPA2)
			fprintf(ofp, "wpa=%d\n", 2);
		else if(wlan->sec_policy == EAP_WPA_WPA2)
			fprintf(ofp, "wpa=%d\n", 3);
		else
		{
			goto err;
		}
		fprintf(ofp, "wpa_key_mgmt=%s\n", "WPA-EAP");
		if(wlan->sec_wpa.mode == TKIP)
			fprintf(ofp, "wpa_pairwise=%s\n", "TKIP");
		else if(wlan->sec_wpa.mode == CCMP)
			fprintf(ofp, "wpa_pairwise=%s\n", "CCMP");
		else
			goto err;
		
		if(ofp != NULL)
			fclose(ofp);	
		
		snprintf(cmd, sizeof(cmd), "hostapd %s &", filename);
		SYSTEM(cmd);
	}
	else if((wlan->sec_policy == PSK_WPA_WPA2)||(wlan->sec_policy == PSK_WPA)||(wlan->sec_policy == PSK_WPA2))  /* WPA/WPA2 PSK */
	{
		if(wlan->sec_policy == PSK_WPA_WPA2)
			snprintf(filename,sizeof(filename), "%s/psk_%s", HOSTAPD_PROFILE_PATH, wlanname);
		else if(wlan->sec_policy == PSK_WPA)
			snprintf(filename,sizeof(filename), "%s/psk_wpa_%s", HOSTAPD_PROFILE_PATH, wlanname);
		else if(wlan->sec_policy == PSK_WPA2)
			snprintf(filename,sizeof(filename), "%s/psk_wpa2_%s", HOSTAPD_PROFILE_PATH, wlanname);
		ofp = fopen(filename, "w");
		if(ofp == NULL)
			goto err;
		fprintf(ofp, "interface=%s\n", wlanname); 
		fprintf(ofp, "bridge=%s\n", wlan->data_packet_method==LOCAL_BRIDGE?bridgename:"default"); 
		#ifdef AR11N
		fprintf(ofp, "driver=%s\n", "madwifi"); 
		#endif
		fprintf(ofp, "logger_syslog=%d\n", -1); 
		fprintf(ofp, "logger_syslog_level=%d\n", 2); 
		fprintf(ofp, "logger_stdout=%d\n", -1); 
		fprintf(ofp, "logger_stdout_level=%d\n", 2); 
		fprintf(ofp, "dump_file=%s\n", "/tmp/hostapd.dump"); 
		fprintf(ofp, "ssid=%s\n", wlan->SSID);
		fprintf(ofp, "eapol_key_index_workaround=%d\n", 0);
		fprintf(ofp, "own_ip_addr=%s\n", hostip);
		
		if(wlan->sec_policy == PSK_WPA)
			fprintf(ofp, "wpa=%d\n", 1);
		else if(wlan->sec_policy == PSK_WPA2)
			fprintf(ofp, "wpa=%d\n", 2);
		else if(wlan->sec_policy == PSK_WPA_WPA2)
			fprintf(ofp, "wpa=%d\n", 3);
		else
		{
			goto err;
		}
		fprintf(ofp, "wpa_passphrase=%s\n", wlan->sec_wpa.wpa_key.key);
		fprintf(ofp, "wpa_key_mgmt=%s\n", "WPA-PSK");
		if(wlan->sec_wpa.mode == TKIP)
			fprintf(ofp, "wpa_pairwise=%s\n", "TKIP");
		else if(wlan->sec_wpa.mode == CCMP)
			fprintf(ofp, "wpa_pairwise=%s\n", "CCMP");
		else
			goto err;
		
		if(ofp != NULL)
			fclose(ofp);
		snprintf(cmd, sizeof(cmd), "hostapd %s &", filename);
		SYSTEM(cmd);
	}
	else if(wlan->sec_policy == WEP)                                                  /* wep */
	{
		snprintf(cmd, sizeof(cmd), "iwpriv %s authmode 1", wlanname);                 /*authmode shared key*/
		SYSTEM(cmd);
		if(wlan->sec_wep.key_t == ASSCII)
		{
			snprintf(cmd, sizeof(cmd), "iwconfig %s key [%d] s:%s", wlanname, wlan->sec_wep.index, wlan->sec_wep.wep_key[wlan->sec_wep.index-1].key);
			SYSTEM(cmd);
		}
		else if(wlan->sec_wep.key_t == HEX)
		{
			snprintf(cmd, sizeof(cmd), "iwconfig %s key [%d] %s", wlanname, wlan->sec_wep.index, wlan->sec_wep.wep_key[wlan->sec_wep.index-1].key);
			SYSTEM(cmd);
		}
		else
			goto err;
	}
	snprintf(cmd, sizeof(cmd), "ifconfig %s up", wlanname);
	SYSTEM(cmd);
	
	return TRUE;
err:
	snprintf(cmd, sizeof(cmd), "ifconfig %s up", wlanname);
	SYSTEM(cmd);
	if(ofp != NULL)
		fclose(ofp);
	return FALSE;
quit:
	snprintf(cmd, sizeof(cmd), "ifconfig %s up", wlanname);
	SYSTEM(cmd);
	if(ofp != NULL)
		fclose(ofp);
	return TRUE;
}
BOOL set_security_policy()
{
	char cmd[128] = {0};
	WLAN_INFO *wlan = NULL;
	int ret = FALSE;
	pthread_mutex_lock(&wlan_mutex);
	wlan = wlan_list;
	while(wlan != NULL)
	{
		if(wlan->sec_policy == EAP_WPA_WPA2)
		{
			sprintf(cmd, "ifconfig ath.%d-%d down", wlan->rid-1, wlan->wid);
			SYSTEM(cmd);
			if(wlan->security == SECURITY_WAPI)
			{
				if(set_security_mode_wapi(wlan) == TRUE)
				{
					ret = SUCCESSED;
				}
				else
				{
					ret = FAILED;
				}
			}
			else if(wlan->security == SECURITY_802_11i)
			{
				
				if(set_security_mode(wlan) == TRUE)
				{
					ret = SUCCESSED;
				}
				else
				{
					ret = FAILED;
				}
			}
			sprintf(cmd, "ifconfig ath.%d-%d up", wlan->rid-1, wlan->wid);
			SYSTEM(cmd);
		}
		wlan = wlan->next;
	}
	pthread_mutex_unlock(&wlan_mutex);
	return ret;
}
BOOL set_downlink_traffic_limit(char *wlanname, u32 ssid_traffic_limit, u32 user_traffic_limit)
{
	char cmd[128] = {0};
	if(ssid_traffic_limit == 0)
	{
		traffic_vap_down_switch = FALSE;
		
		if(traffic_vap_up_switch==FALSE)
		{
			sprintf(cmd,"autelan traffic_limit %s set_vap_flag 0", wlanname);
			SYSTEM(cmd);
		}
		save_apctl_level_config("traffic_vap_down_switch", INT, (int*)&traffic_vap_down_switch,1);
	}
	else
	{
		traffic_vap_down_switch = TRUE;
		
		sprintf(cmd,"autelan traffic_limit %s set_vap_flag 1", wlanname);
		SYSTEM(cmd);
		sprintf(cmd,"autelan traffic_limit %s set_vap_send %d", wlanname, ssid_traffic_limit);
		SYSTEM(cmd);
		save_apctl_level_config("traffic_vap_down_switch", INT, (int*)&traffic_vap_down_switch,1);
	}
	memset(cmd, 0, 128);
	if(user_traffic_limit == 0)
	{
		traffic_node_down_switch = FALSE;
		if(traffic_node_up_switch==FALSE)
		{
			sprintf(cmd,"autelan traffic_limit %s set_every_node_flag 0", wlanname);
			SYSTEM(cmd);
		}
		save_apctl_level_config("traffic_node_down_switch", INT, (int*)&traffic_node_down_switch,1);
	}
	else
	{
		traffic_node_down_switch = TRUE;
		
		sprintf(cmd,"autelan traffic_limit %s set_every_node_flag 1", wlanname);
		SYSTEM(cmd);
		sprintf(cmd,"autelan traffic_limit %s set_every_node_send %d", wlanname, user_traffic_limit);
		SYSTEM(cmd);
		save_apctl_level_config("traffic_node_down_switch", INT, (int*)&traffic_node_down_switch,1);
	}
	return TRUE;
}
BOOL save_wlan_config_for_hot_start(WLAN_INFO *wlan)
{
	if(wlan == NULL)
		return RAISE_ERROR(ERROR_WRONG_ARG, NULL);

	int rid = wlan->rid -1, wid = wlan->wid;
	char path[128] = {0},cmd[128]={0};
	unsigned char bssid[24]={0};

	//rm old config first.
	sprintf(path,"%s%d/wlaninfo/wlan%d",CONF_RADIO_PATH, rid, wid);
	printf_d("%s,%d,save rid(%d),wid(%d) info into (%s)\n", __func__, __LINE__, rid, wid, path);
	if(access(path, R_OK) == 0){
		sprintf(cmd,"rm -rf %s",path);
		SYSTEM(cmd);
	}

	//save parameters of wlan 
	u8 radio_id = wlan->rid -1;
	printf_d("%s,%d,rid(%d),rid-1:%d\n",__func__,__LINE__,wlan->rid,radio_id);
	save_wlan_level_config(rid, wid, "rid",CHAR, (u8*)&radio_id,1);
	save_wlan_level_config(rid, wid, "wid",CHAR, (u8*)&wlan->wid,0);
	save_wlan_level_config(rid, wid, "vid",SHORT, (u16*)&wlan->vid,0);
	save_wlan_level_config(rid, wid, "data_packet_method",CHAR, (u8*)&wlan->data_packet_method,0);
	save_wlan_level_config(rid, wid, "SSID_len",CHAR, (u8*)&wlan->SSID_len,0);
	printf_d("%s,%d,SSID(%s)\n",__func__,__LINE__,wlan->SSID);
	save_wlan_level_config(rid, wid, "SSID",STR, wlan->SSID,0);
	save_wlan_level_config(rid, wid, "hide_SSID",CHAR, (u8*)&wlan->hide_SSID,0);
	save_wlan_level_config(rid, wid, "max_users",CHAR, (u8*)&wlan->max_users,0);
	save_wlan_level_config(rid, wid, "rx_interval_bytes",INT, (u32*)&wlan->rx_interval_bytes,0);
	save_wlan_level_config(rid, wid, "tx_interval_bytes",INT, (u32*)&wlan->tx_interval_bytes,0);

	sprintf(bssid,MAC_FMT"",MAC2STR(wlan->BSSID));
	printf_d("%s,%d,bssid(%s)\n",__func__,__LINE__,bssid);
	save_wlan_level_config(rid, wid, "BSSID",STR, (u8*)bssid,0);
	save_wlan_level_config(rid, wid, "security",CHAR, (u8*)&wlan->security,0);
	if(wlan->security == SECURITY_802_11i){
		save_wlan_level_config(rid, wid, "sec_policy",CHAR, (u8*)&wlan->sec_policy,0);
		if(wlan->sec_policy == WEP){
			save_wlan_level_config(rid, wid, "sec_wep.bits",INT, (u32*)&wlan->sec_wep.bits,0);
		}else if(wlan->sec_policy == EAP_WPA_WPA2 ||wlan->sec_policy == PSK_WPA_WPA2 ){
			save_wlan_level_config(rid, wid, "sec_wpa.mode",INT, (u32*)&wlan->sec_wpa.mode,0);
		}
	}

	return TRUE;
}
BOOL add_wlan_to_list(WLAN_INFO *wlan)
{
	WLAN_INFO *pwlan = NULL, *pwlan1 = NULL;
	BOOL is_exists = FALSE;
	if(wlan == NULL)
		return RAISE_ERROR(ERROR_WRONG_ARG, NULL);
	pthread_mutex_lock(&wlan_mutex);
	save_wlan_config_for_hot_start(wlan);
	if(wlan_list == NULL)
	{
		int i;
		for(i = 0; i < RADIO_NUM; i++)
		{		
			gRadioInfoValue[i].wifi_start_time = info_report_get_current_time();
			gRadioInfoValue[i].wifi_current_time = gRadioInfoValue[i].wifi_start_time;
			save_radio_level_config(i,"wifi_start_time",UL,(unsigned long*)&gRadioInfoValue[i].wifi_start_time,1);
			save_radio_level_config(i,"wifi_current_time",UL,(unsigned long*)&gRadioInfoValue[i].wifi_current_time,1);
		}
		wlan_list = wlan;
		Wlan_ReportEnable = ENABLED;
		save_apctl_level_config("Wlan_ReportEnable",CHAR,(u8 *)&Wlan_ReportEnable,1);
	}
	else
	{
		pwlan1 = pwlan = wlan_list;
		while(pwlan != NULL)
		{
			if((wlan->rid == pwlan->rid) && (wlan->wid == pwlan->wid))
			{
			
				memcpy(pwlan->BSSID, wlan->BSSID, MAC_ADDR_LEN);
				FREE_OBJ(pwlan->SSID);
				FREE_OBJ(pwlan->sec_wapi.key.key);
				FREE_OBJ(pwlan->sec_wep.wep_key[0].key);
				FREE_OBJ(pwlan->sec_wep.wep_key[1].key);
				FREE_OBJ(pwlan->sec_wep.wep_key[2].key);
				FREE_OBJ(pwlan->sec_wep.wep_key[3].key);
				FREE_OBJ(pwlan->sec_wpa.wpa_key.key);
				/* */
				pwlan->ability_info = wlan->ability_info;
				pwlan->data_packet_method = wlan->data_packet_method;
				pwlan->hide_SSID = wlan->hide_SSID;
				pwlan->is_exists = wlan->is_exists;
				pwlan->mac_mode = wlan->mac_mode;
				pwlan->mac_percolate_rule = wlan->mac_percolate_rule;
				pwlan->max_users = wlan->max_users;
				pwlan->Qos_switch = wlan->Qos_switch;
				pwlan->ret = wlan->ret;
				pwlan->security = wlan->security;
				pwlan->sec_policy = wlan->sec_policy;
				pwlan->sec_wapi.encryption_type = wlan->sec_wapi.encryption_type;
				pwlan->sec_wapi.index = wlan->sec_wapi.index;
				pwlan->sec_wapi.key_t = wlan->sec_wapi.key_t;
				pwlan->sec_wapi.key.len = wlan->sec_wapi.key.len;
				pwlan->sec_wapi.key.key = wlan->sec_wapi.key.key;
				pwlan->sec_wep.bits = wlan->sec_wep.bits;
				pwlan->sec_wep.index = wlan->sec_wep.index;
				pwlan->sec_wep.key_t = wlan->sec_wep.key_t;
				pwlan->sec_wep.wep_key[0].len = wlan->sec_wep.wep_key[0].len;
				pwlan->sec_wep.wep_key[0].key = wlan->sec_wep.wep_key[0].key;
				pwlan->sec_wep.wep_key[1].len = wlan->sec_wep.wep_key[1].len;
				pwlan->sec_wep.wep_key[1].key = wlan->sec_wep.wep_key[1].key;
				pwlan->sec_wep.wep_key[2].len = wlan->sec_wep.wep_key[2].len;
				pwlan->sec_wep.wep_key[2].key = wlan->sec_wep.wep_key[2].key;
				pwlan->sec_wep.wep_key[3].len = wlan->sec_wep.wep_key[3].len;
				pwlan->sec_wep.wep_key[3].key = wlan->sec_wep.wep_key[3].key;
				pwlan->sec_wpa.key_t = wlan->sec_wpa.key_t;
				pwlan->sec_wpa.mode = wlan->sec_wpa.mode;
				pwlan->sec_wpa.wpa_key.len = wlan->sec_wpa.wpa_key.len;
				pwlan->sec_wpa.wpa_key.key = wlan->sec_wpa.wpa_key.key;
				pwlan->SSID = wlan->SSID;
				pwlan->SSID_flow_limit = wlan->SSID_flow_limit;
				pwlan->SSID_len = wlan->SSID_len;
				pwlan->user_flow_limit = wlan->user_flow_limit;
				pwlan->vid = wlan->vid;
				is_exists = TRUE;
				break;
			}
			if(pwlan != wlan_list)
			{
				pwlan1 = pwlan;				
			}
			pwlan = pwlan->next;
			
		}
		if(pwlan == NULL)
		{
			pwlan1->next = wlan;
		}
	}
	pthread_mutex_unlock(&wlan_mutex);
	return is_exists;
}

BOOL create_sta_check_thread()
{
	if(wlan_list != NULL)
	{
		if((thread_sta_tid == 0) || (ESRCH == pthread_kill(thread_sta_tid, 0)))
		{
			if(0 != pthread_create(&thread_sta_tid, 0, check_sta_on_off_line, NULL))
			{
				printf_d("create wlan failed !\n");
				return FALSE;
			}
		}
	}
	return TRUE;
}
BOOL is_exists_same_wlan(u8 rid, u8 wid)
{
	WLAN_INFO  *wlan = NULL;
	BOOL ret = FALSE;
	pthread_mutex_lock(&wlan_mutex);
	wlan = wlan_list;
	while(wlan != NULL)
	{
		if((wlan->rid == rid) && (wlan->wid == wid))
		{
			ret = TRUE;
			break;
		}
		wlan = wlan->next;
	}
	pthread_mutex_unlock(&wlan_mutex);
	return ret;
}
/* no need wlan_mutex */
BOOL get_wlan_if_by_rid(u8 rid, char *wlan_name)
{
	WLAN_INFO *pwlan = NULL;
	if(wlan_name == NULL)
		return FALSE;

	for(pwlan = wlan_list; pwlan != NULL; pwlan = pwlan->next)
	{
		if(pwlan->rid == rid)
		{
			snprintf(wlan_name, 16, "ath.%d-%d", pwlan->rid-1, pwlan->wid);
			return TRUE;
		}
	}
	return FALSE;
}

BOOL set_radio_state_none_lock(TSTATE state, u8 rid)
{
	WLAN_INFO *pwlan = NULL;
	char cmd[64] = {0};
	if(wlan_list == NULL)
		goto err;
	if(state == DISABLED)
	{
		for(pwlan = wlan_list; pwlan != NULL; pwlan = pwlan->next)
		{
			if(pwlan->rid == rid)				
			{
				snprintf(cmd, sizeof(cmd), "ifconfig ath.%d-%d down", pwlan->rid-1, pwlan->wid);
				SYSTEM(cmd);
			}
		}
		if(ReportEnable == ENABLED)
		{
			memset(cmd,0,64);
			snprintf(cmd,sizeof(cmd),"ifconfig athscanwifi%d down",rid-1);
			SYSTEM(cmd);
			SYSTEM("killall -STOP iwlist_scan > /dev/null 2>&1");
		}
	}
	else if(state == ENABLED)
	{
		for(pwlan = wlan_list; pwlan != NULL; pwlan = pwlan->next)
		{
			if(pwlan->rid == rid)
			{
				snprintf(cmd, sizeof(cmd), "ifconfig ath.%d-%d up", pwlan->rid-1, pwlan->wid);
				SYSTEM(cmd);
			}
		}
		if(ReportEnable == ENABLED)
		{
			memset(cmd,0,64);
			snprintf(cmd,sizeof(cmd),"ifconfig athscanwifi%d up",rid-1);
			SYSTEM(cmd);
			SYSTEM("killall -CONT iwlist_scan > /dev/null 2>&1");
		}

	}
	else
		goto err;
	wireless_if_updown_count[rid-1]++;
	save_radio_level_config(rid-1,"wireless_if_updown_count",INT,(int*)&wireless_if_updown_count[rid-1],1);
	return TRUE;
err:
	return FALSE;
}

BOOL set_radio_state(TSTATE state, u8 rid)
{
	WLAN_INFO *pwlan = NULL;
	char cmd[64] = {0};
	pthread_mutex_lock(&wlan_mutex);
	if(wlan_list == NULL)
		goto err;
	if(state == DISABLED)
	{
		for(pwlan = wlan_list; pwlan != NULL; pwlan = pwlan->next)
		{
			if(pwlan->rid == rid)
			{
				snprintf(cmd, sizeof(cmd), "ifconfig ath.%d-%d down", pwlan->rid-1, pwlan->wid);
				SYSTEM(cmd);
			}
		}
		if(ReportEnable == ENABLED)
		{
			memset(cmd,0,64);
			snprintf(cmd,sizeof(cmd),"ifconfig athscanwifi%d down",rid-1);
			SYSTEM(cmd);
			SYSTEM("killall -STOP iwlist_scan > /dev/null 2>&1");
		}
	}
	else if(state == ENABLED)
	{
		for(pwlan = wlan_list; pwlan != NULL; pwlan = pwlan->next)
		{
			if(pwlan->rid == rid)
			{
				snprintf(cmd, sizeof(cmd), "ifconfig ath.%d-%d up", pwlan->rid-1, pwlan->wid);
				SYSTEM(cmd);
			}
		}
		if(ReportEnable == ENABLED)
		{
			memset(cmd,0,64);
			snprintf(cmd,sizeof(cmd),"ifconfig athscanwifi%d up",rid-1);
			SYSTEM(cmd);
			SYSTEM("killall -CONT iwlist_scan > /dev/null 2>&1");
		}

	}
	else
		goto err;
	wireless_if_updown_count[rid-1]++;
	save_radio_level_config(rid-1,"wireless_if_updown_count",INT,(int*)&wireless_if_updown_count[rid-1],1);
	pthread_mutex_unlock(&wlan_mutex);
	return TRUE;
err:
	pthread_mutex_unlock(&wlan_mutex);
	return FALSE;
}



BOOL set_mode(u8 rid, u8 wid)
{
	char cmd[128] = {0};
	int extoffset = 0, cwmmode = 0;
	char wlanname[32] = {0};
	
	sprintf(wlanname, "ath.%d-%d", rid, wid);
	/*	
		sprintf(cmd, "iwconfig %s channel %d", wlanname, 0);
		SYSTEM(cmd);
	*/
	if(gRadioInfoValue[rid].channel_width == 0)
	{
		extoffset = 0;
		cwmmode = 0;
	}
	else if(gRadioInfoValue[rid].channel_width == 1)
	{
		extoffset = 0;
		cwmmode = 1;
	}
	else if(gRadioInfoValue[rid].channel_width == 2)
	{
		extoffset = -1;
		cwmmode = 2;
	}
	else if(gRadioInfoValue[rid].channel_width == 3)
	{
		extoffset = 1;
		cwmmode = 2;
	}
	if(gRadioInfoValue[rid].radiotype == IEEE80211_PARAM_MODE_11b)	//11b(2.4G)
		sprintf(cmd,"set_mode %s 11b", wlanname);
	else if(gRadioInfoValue[rid].radiotype == IEEE80211_PARAM_MODE_11a)	//11a(5G)
		sprintf(cmd,"set_mode %s 11a", wlanname);
	else if(gRadioInfoValue[rid].radiotype == IEEE80211_PARAM_MODE_11g)	//11g(2.4G)
		sprintf(cmd,"set_mode %s 11g", wlanname);
	else if(gRadioInfoValue[rid].radiotype == IEEE80211_PARAM_MODE_11bg)	//11b(2.4G) & 11g(2.4G)
		sprintf(cmd,"set_mode %s 11bg", wlanname);
	else if(gRadioInfoValue[rid].radiotype == IEEE80211_PARAM_MODE_11n_a) //11an(5G)
		sprintf(cmd,"set_mode %s 11na %d %d", wlanname, extoffset, cwmmode);
	else if(gRadioInfoValue[rid].radiotype == IEEE80211_PARAM_MODE_11an_a)	//11a(5G) & 11an(5G)
		sprintf(cmd,"set_mode %s 11ana %d %d", wlanname, extoffset, cwmmode);
	else if(gRadioInfoValue[rid].radiotype == IEEE80211_PARAM_MODE_11n_g)	//11gn(2.4G)
		sprintf(cmd,"set_mode %s 11ng %d %d", wlanname, extoffset, cwmmode);
	else if(gRadioInfoValue[rid].radiotype == IEEE80211_PARAM_MODE_11gn_g)	//11g(2.4G) & 11gn(2.4G)
		sprintf(cmd,"set_mode %s 11gng %d %d", wlanname, extoffset, cwmmode);
	else if(gRadioInfoValue[rid].radiotype == IEEE80211_PARAM_MODE_11bgn_g)	//11b(2.4G) & 11g(2.4G) & 11gn(2.4G)
		sprintf(cmd,"set_mode %s 11bgng %d %d", wlanname, extoffset, cwmmode);
	else
	{
		printf_d("radio->radio_type = %d\n", gRadioInfoValue[rid].radiotype);
		return FALSE;
	}
	SYSTEM(cmd);
	//set_mode set channel 0,need recoverys
	sprintf(cmd, "iwconfig %s channel %d", wlanname, gRadioInfoValue[rid].channel);
	SYSTEM(cmd);
	return TRUE;
}
BOOL set_radio_mode(u8 rid)
{
	WLAN_INFO *wlan = NULL;
	wlan = wlan_list;

	while(wlan != NULL)
	{
		if (rid == 0 || rid ==  wlan->rid)
			set_mode(wlan->rid-1, wlan->wid);
		wlan = wlan->next;
	}
	
	if(ReportEnable) {		
		char cmd_str[128] = {0};
		char str_tmp[24] = {0};
		int state;

		sprintf(cmd_str, "cat /tmp/report_switch|grep rrm_report_enable|awk -F \"=\" '{print $2}'");
		popen_fgets(cmd_str, str_tmp, sizeof(str_tmp));
		state = atoi(str_tmp);

		if(state == 1) {
			if (rid == 0) {		
				set_mode2(0);
				if(Check_Interface_State("wifi1")==0)
					set_mode2(1);
			}
			else
				set_mode2(rid-1);
		}
	}
	
	return TRUE;
}

BOOL set_radio_param(u8 rid, u8 wid)
{
	char cmdbuf[128]={0};
	char wlanname[16] = {0};
	
	sta_limit(rid - 1);
	
	set_radio_state(DISABLED, rid);
	pthread_mutex_lock(&wlan_mutex);
	snprintf(wlanname, sizeof(wlanname), "ath.%d-%d", rid-1, wid);
 	//set radio mode
	set_mode(rid-1, wid);
	/* set txpower */
	if(gRadioInfoValue[rid-1].txpower <= 3)
		sprintf(cmdbuf, "settxpower %s %d", wlanname, 3);
	else
		sprintf(cmdbuf, "settxpower %s %d", wlanname, gRadioInfoValue[rid-1].txpower);
	SYSTEM(cmdbuf);
	//set channel
	sprintf(cmdbuf,"iwconfig %s channel %d", wlanname, gRadioInfoValue[rid-1].channel);
	SYSTEM(cmdbuf);
	//11N rate set
	//memset(cmdbuf, 0, 128);
	//sprintf(cmdbuf,"iwpriv wifi%d set_mcs_mask 0x%x",  rid-1, gRadioInfoValue[rid-1].MCS&0x0000FFFF);
	//SYSTEM(cmdbuf);
	//memset(cmdbuf, 0, 128);
	//sprintf(cmdbuf,"iwpriv wifi%d set_mcs_cap 0x%x",  rid-1, 0x80);
	//SYSTEM(cmdbuf); 
	//chianmask
	sprintf(cmdbuf,"iwpriv wifi%d rxchainmask %d",	rid-1, gRadioInfoValue[rid-1].chianmask);
	SYSTEM(cmdbuf);
	sprintf(cmdbuf,"iwpriv wifi%d txchainmask %d",	rid-1, gRadioInfoValue[rid-1].chianmask);
	SYSTEM(cmdbuf);
	//A-MPDU
	/* 1-open 0-close */
	sprintf(cmdbuf,"iwpriv wifi%d AMPDU %d > /dev/null",  rid-1, gRadioInfoValue[rid-1].AMPDU);
	SYSTEM(cmdbuf);
	//short GI(Guard Interval)
	sprintf(cmdbuf, "iwpriv %s shortgi %d", wlanname, gRadioInfoValue[rid-1].shortGI);
	SYSTEM(cmdbuf);
	//HT-greenfield/HT-Mixed
	sprintf(cmdbuf,"iwpriv %s puren %d",wlanname, gRadioInfoValue[rid-1].HTMixed);
	SYSTEM(cmdbuf);
	//A-MSDU
	sprintf(cmdbuf,"iwpriv wifi%d AMSDU %d > /dev/null",  rid-1, gRadioInfoValue[rid-1].AMSDU);
	SYSTEM(cmdbuf);
	//becone interval		
	sprintf(cmdbuf,"iwpriv %s bintval %d",wlanname, gRadioInfoValue[rid-1].beaconInterval);
	SYSTEM(cmdbuf);
	//DTIM
	sprintf(cmdbuf,"iwpriv %s dtim_period %d", wlanname, gRadioInfoValue[rid-1].dtim);
	SYSTEM(cmdbuf);
	//frag threshold
	sprintf(cmdbuf,"iwconfig %s frag %d",wlanname, gRadioInfoValue[rid-1].fragThreshold);
	SYSTEM(cmdbuf);
	//RTS threshold
	sprintf(cmdbuf,"iwconfig %s rts %d",wlanname, gRadioInfoValue[rid-1].rtsThreshold);
	SYSTEM(cmdbuf);
	//short preamble
	sprintf(cmdbuf,"iwpriv %s shpreamble %d", wlanname, gRadioInfoValue[rid-1].preamble);
	SYSTEM(cmdbuf);
	//rate set
	//extern rate set
	if(gRadioInfoValue[rid-1].radiotype & IEEE80211_PARAM_MODE_11g) 	//g 1-54 (contain a and b)
	{
		sprintf(cmdbuf,"iwpriv wifi%d set_rate_mask_g 0x%x", rid-1, gRadioInfoValue[rid-1].rate_mask);
	}
	else if(gRadioInfoValue[rid-1].radiotype & IEEE80211_PARAM_MODE_11b)	//b 6-54
	{
		sprintf(cmdbuf,"iwpriv wifi%d set_rate_mask_b 0x%x", rid-1, gRadioInfoValue[rid-1].rate_mask);
	}
	else if(gRadioInfoValue[rid-1].radiotype & IEEE80211_PARAM_MODE_11a)	//a 1-11
	{
		sprintf(cmdbuf,"iwpriv wifi%d set_rate_mask_a 0x%x", rid-1, gRadioInfoValue[rid-1].rate_mask);
	}
	else
	{
		goto ext;
	}
	SYSTEM(cmdbuf);
	memset(cmdbuf, 0, 128);
	sprintf(cmdbuf,"iwpriv wifi%d set_rate_cap 0x%x", rid-1, 0x80);
	SYSTEM(cmdbuf);
ext:
	pthread_mutex_unlock(&wlan_mutex);	
	set_radio_state(ENABLED, rid);
	return TRUE;
}
/*
int is_exists_wlan(u8 rid)
{
	WLAN_INFO *pwlan = NULL;
	int count = 0;
	pthread_mutex_lock(&wlan_mutex);
	for(pwlan = wlan_list; pwlan != NULL; pwlan = pwlan->next)
	{
		if(pwlan->rid == rid)
		{
			count++;
		}
	}
	pthread_mutex_unlock(&wlan_mutex);
	return count;
}
*/
BOOL add_vlan_to_bridge(WLAN_INFO *wlanconf)
{
	char cmdbuf[128]={0};
	char wlanname[16] = {0};
	char sub_eth_name[16] = {0};
	int eth_nu = 0;
	sprintf(cmdbuf, "br%d", wlanconf->vid);
	snprintf(wlanname, 16, "ath.%d-%d", wlanconf->rid-1, wlanconf->wid);

	#if (defined( AR11N ))
		eth_nu=1;
	#endif
	
	/*create bridge*/
	if(Check_Interface_State(cmdbuf)!=0)
	{
		sprintf(cmdbuf, "brctl addbr br%d", wlanconf->vid);
		SYSTEM(cmdbuf);
		sprintf(cmdbuf, "/sbin/ifconfig br%d 0.0.0.0 up", wlanconf->vid);
		SYSTEM(cmdbuf);
	}
	/*add subif*/
	sprintf(sub_eth_name, "eth%d.%d", eth_nu, wlanconf->vid);
	if(Check_Interface_State(cmdbuf)!=0)
	{
		sprintf(cmdbuf, "vconfig add eth%d %d",eth_nu, wlanconf->vid);
		SYSTEM(cmdbuf);
		sprintf(cmdbuf, "brctl addif br%d %s", wlanconf->vid, sub_eth_name);
		SYSTEM(cmdbuf);
		sprintf(cmdbuf, "/sbin/ifconfig %s up",sub_eth_name);
		SYSTEM(cmdbuf);
		sprintf(cmdbuf, "/sbin/ifconfig eth%d mtu 1504",eth_nu);
		SYSTEM(cmdbuf);
	}
	/*add vap to bridge*/
	sprintf(cmdbuf,"brctl addif br%d %s", wlanconf->vid, wlanname);
	SYSTEM(cmdbuf);
	
	set_dhcp_snooping(ENABLED);
	return TRUE;
}


BOOL manage_add_wlan_request_msg(WLAN_INFO *wlanconfig)
{
	BOOL ret = FALSE;
	char *ifname="wifi1";
	char cmd[128] = {0};
	char wlanname[16] = {0};
	int radiocount = 1;
	int i = 0;
	WLAN_INFO *wlanconf[2]={NULL,NULL};
	WLAN_INFO *wlanconf_second = NULL;
	WLAN_INFO *pwlan = NULL;
	WLAN_INFO *ptr	 = NULL;
	//BOOL is_exists = FALSE;
	AP_KEY_EVENT_INFO *ap_key_event = NULL;
	AP_KEY_EVENT_INFO *ap_key_event_tmp = NULL;		

	wlanconf[0]=wlanconfig;
	if(check_interface_state(ifname)==0)
	{
		radiocount++;
		copy_wlanconf_to_sencond_wifi(wlanconfig,&wlanconf_second);
		wlanconf[1] = wlanconf_second;
		
	}
	for(i=0;i<radiocount;i++)
	{
		pwlan = wlanconf[i];
		get_key_event_wlan_enable(&ap_key_event, pwlan);
		while(pwlan != NULL)
		{
			set_radio_state(DISABLED, pwlan->rid);
			pwlan->is_exists = is_exists_same_wlan(pwlan->rid, pwlan->wid);
			if(pwlan->is_exists == TRUE)
			{
				delete_vlan_from_bridge(pwlan->rid,pwlan->wid);
				sprintf(cmd,"wlanconfig ath.%d-%d destroy", pwlan->rid-1, pwlan->wid);
				SYSTEM(cmd);
			}
			/* add wlan */
			sprintf(cmd,"wlanconfig ath.%d-%d create wlandev wifi%d wlanmode ap > /dev/null", pwlan->rid-1, pwlan->wid, pwlan->rid-1);
			SYSTEM(cmd);
	//		sprintf(cmd, "brctl addif default ath.%d-%d",  pwlan->rid-1, pwlan->wid);
	//		SYSTEM(cmd);
		//	count = is_exists_wlan(pwlan->rid);
		//	if(count < 2)  /*del: fix bug ath.0-3... mode is always AUTO*/
			{
				set_radio_param(pwlan->rid, pwlan->wid);
			}
			/* set_radio_info_no_wlan(pwlan->rid, count); */
			sprintf(wlanname, "ath.%d-%d", pwlan->rid-1, pwlan->wid);
			/* set ssid */
			sprintf(cmd,"iwconfig %s essid \"%s\"",  wlanname, pwlan->SSID);
			SYSTEM(cmd);
			get_bssid(pwlan->rid-1, pwlan->wid, pwlan->BSSID);
			printf_d("[%s]  BSSID [%02X:%02X:%02X:%02X:%02X:%02X]\n", wlanname, pwlan->BSSID[0], pwlan->BSSID[1], pwlan->BSSID[2], pwlan->BSSID[3], pwlan->BSSID[4], pwlan->BSSID[5]);
			/* set security mode */
			if(pwlan->security == SECURITY_WAPI)
			{
				if(set_security_mode_wapi(pwlan) == TRUE)
				{
					pwlan->ret = SUCCESSED;
				}
				else
				{
					pwlan->ret = FAILED;
				}
			}
			else if(pwlan->security == SECURITY_802_11i)
			{
				if(set_security_mode(pwlan) == TRUE)
				{
					pwlan->ret = SUCCESSED;
				}
				else
				{
					pwlan->ret = FAILED;
				}
			}
			sprintf(cmd, "iwpriv %s hide_ssid %d", wlanname, pwlan->hide_SSID);
			SYSTEM(cmd);
			memset(cmd, 0, 128);
			sprintf(cmd, "iwpriv %s wmm %d", wlanname, pwlan->Qos_switch);
			SYSTEM(cmd);
			sprintf(cmd,"iwpriv %s ampdu %d > /dev/null",  wlanname, gRadioInfoValue[pwlan->rid - 1].AMPDU);
			SYSTEM(cmd);
			sprintf(cmd,"iwpriv %s amsdu %d > /dev/null",  wlanname, gRadioInfoValue[pwlan->rid - 1].AMSDU);
			SYSTEM(cmd);
			sprintf(cmd, "echo %d > /proc/sys/net/%s/maxaid", pwlan->max_users, wlanname);
			SYSTEM(cmd);
			/* set black/white list */
			snprintf(cmd, sizeof(cmd), "iwpriv %s maccmd %d",wlanname, pwlan->mac_percolate_rule);
			SYSTEM(cmd);	
			
			/* 1. When AP run, blacklist_info received earlier than wlan_info.
			** 2. When wlan has been rebuild, blacklist need to renew. */
			set_sta_mac_addr_filtration(pwlan->rid, pwlan->wid);
				
			set_downlink_traffic_limit(wlanname, pwlan->SSID_flow_limit, pwlan->user_flow_limit);
			set_radio_state(ENABLED, pwlan->rid);
			add_wlan_to_list(pwlan);

			if(debugf == TRUE)
			{
				printf("%-24s  %d\n", "RID:", pwlan->rid-1);
				printf("%-24s  %d\n", "WID:", pwlan->wid);
				printf("%-24s  %d\n", "security:", pwlan->security);
				printf("%-24s  %d\n", "sec_policy:", pwlan->sec_policy);
				printf("%-24s  %d\n", "max_users:", pwlan->max_users);
				printf("%-24s  %d\n", "SSID_flow_limit:", pwlan->SSID_flow_limit);
				printf("%-24s  %d\n", "user_flow_limit:", pwlan->user_flow_limit);
				printf("%-24s  %d\n", "SSID_len:", pwlan->SSID_len);
				printf("%-24s  %s\n", "SSID:", pwlan->SSID);
				printf("%-24s  %s\n", "data_packet_method:" ,pwlan->data_packet_method==TUNNEL_802_3?"TUNNEL_802_3":"LOCAL_BRIDGE");
				printf("%-24s  %d\n", "mac_mode:" ,pwlan->mac_mode);
				if(pwlan->security == SECURITY_802_11i)
				{
					switch(pwlan->sec_policy)
					{
						case OPEN:
							break;
						case WEP:
							printf("==================================================================\n");
							printf("								WEP\n");
							printf("%-24s  %d\n", "wep key bits:", pwlan->sec_wep.bits);
							printf("%-24s  %d\n", "wep key type:", pwlan->sec_wep.key_t);
							printf("%-24s  %d\n", "wep key index:",pwlan->sec_wep.index);
							int k = 0;
							
							printf("%-24s  %d\n", "wep key1 len:", pwlan->sec_wep.wep_key[0].len);
							printf("%s\n", "wep key1:");
							for(k = 0; k < pwlan->sec_wep.wep_key[0].len; k++)
							{
								printf("%02X ", pwlan->sec_wep.wep_key[0].key[k]);
							}
							printf("\n");
							printf("%-24s  %d\n", "wep key2 len:", pwlan->sec_wep.wep_key[1].len);
							printf("%s\n", "wep key2:");
							for(k = 0; k < pwlan->sec_wep.wep_key[1].len; k++)
							{
								printf("%02X ", pwlan->sec_wep.wep_key[1].key[k]);
							}
							printf("\n");
							printf("%-24s  %d\n", "wep key3 len:", pwlan->sec_wep.wep_key[2].len);
							printf("%s\n", "wep key3:");
							for(k = 0; k < pwlan->sec_wep.wep_key[2].len; k++)
							{
								printf("%02X ", pwlan->sec_wep.wep_key[2].key[k]);
							}
							printf("\n");
							printf("%-24s  %d\n", "wep key4 len:", pwlan->sec_wep.wep_key[3].len);
							printf("%s\n", "wep key4:");
							for(k = 0; k < pwlan->sec_wep.wep_key[3].len; k++)
							{
								printf("%02X ", pwlan->sec_wep.wep_key[3].key[k]);
							}
							printf("\n");
							printf("==================================================================\n");
							break;
						case PSK_WPA_WPA2:
							printf("==========================PSK_WPA/WPA2===========================\n");
							printf("%-24s  %d\n", "wpa psk len:", pwlan->sec_wpa.wpa_key.len);
							printf("%-24s  %s\n", "wpa psk:", pwlan->sec_wpa.wpa_key.key);
							break;
						case EAP_WPA_WPA2:
							printf("==========================EAP_WPA/WPA2============================\n");
							break;
						case PSK_WPA:
							printf("==========================PSK_WPA===========================\n");
							break;
						case PSK_WPA2:
							printf("==========================PSK_WPA2==========================\n");
							break;
						case EAP_WPA:
							printf("==========================EAP_WPA============================\n");
							break;
						case EAP_WPA2:
							printf("==========================EAP_WPA2===========================\n");
							break;
						default:
							printf("==========================DEFAULT===========================\n");
							break;
						
					}
				}
			}
			pwlan = pwlan->next;
		}


		send_message(assemble_report_key_event_wlan_enable_request, ap_key_event);
		while(ap_key_event != NULL)
		{
			ap_key_event_tmp = ap_key_event->next;
			FREE_OBJ(ap_key_event->BSSID);
			FREE_OBJ(ap_key_event);
			ap_key_event = ap_key_event_tmp;
		}
		ap_key_event = NULL;
	}
		/* response only for radio0*/
	ret = send_message(assemble_add_wlan_response, wlanconf[0]);
		
	for(i=0;i<radiocount;i++)
	{
		pwlan = wlanconf[i];
		while(pwlan != NULL)
		{
			if(pwlan->is_exists == TRUE)
			{
				ptr = pwlan->next;
				FREE_OBJ(pwlan);
				pwlan = ptr;
			}
			else
			{
				pwlan = pwlan->next;
			}
		}
	}
	
	create_sta_check_thread();
	
	return ret;
}

/* parse delete wlan message */
BOOL parse_del_wlan_requset_msg(TMSG *msg)
{
	WLAN_INFO *wlan ;//= (WLAN_INFO *)malloc(sizeof(WLAN_INFO));
	CREATE_OBJECT_ERR(wlan,WLAN_INFO,MALLOC_ERR_RET(FALSE));
	if(wlan == NULL)
		return RAISE_ERROR(ERROR_OUT_OF_MEMORY, NULL);
	wlan->rid = protocol_retrieve8(msg);
	wlan->wid = protocol_retrieve8(msg);
	/* del wlan */
	return add_cmd_to_list_tail(MSG_ELEMENT_DEL_WLAN_REQUEST_TYPE, wlan);
}

BOOL manage_del_wlan_request_msg(WLAN_INFO *wlan)
{
	BOOL ret = FALSE;
	char cmd[128] = {0};
	WLAN_INFO *ptr1 = NULL;
	WLAN_INFO *ptr2 = NULL;
	pthread_mutex_lock(&wlan_mutex);
	ptr2 = wlan_list;
	char wlanname[16] = {0};
	#if 1
	for(ptr1 = wlan_list;ptr1 != NULL; )
	{
		printf_d("rid:%d wlanid:%d\n",ptr1->rid,ptr1->wid);
		if(wlan->wid == ptr1->wid)
		{
			set_radio_state_none_lock(DISABLED, ptr1->rid);
			printf_d("found wlan in table\n");
			sprintf(wlanname, "ath.%d-%d",  ptr1->rid-1, ptr1->wid);
			//sprintf(cmd,"ifconfig %s down", wlanname);
			//SYSTEM(cmd);
			//ptr1->wlan_updown_time++;
			delete_vlan_from_bridge(ptr1->rid,ptr1->wid);
			sprintf(cmd,"wlanconfig %s destroy", wlanname);
			SYSTEM(cmd);
			
			//diaowq add for hot-start.@20121012
			sprintf(cmd,"rm -rf /tmp/conf_apctl/radio%d/wlaninfo/wlan%d", ptr1->rid-1, ptr1->wid);
			SYSTEM(cmd);
			
			kill_hostapd_by_wlanname(wlanname);
			set_radio_state_none_lock(ENABLED, ptr1->rid);
			wlan->ret = SUCCESSED;
			if(ptr1 == wlan_list)
			{
				wlan_list = wlan_list->next;
				ptr2 = wlan_list;
			}
			else
				ptr2->next = ptr1->next;
			free_wlan_info(ptr1);
			FREE_OBJ(ptr1);	
			ptr1 = ptr2;
			if(ptr2 != ptr1)
				ptr2 = ptr2->next;
		}
		else
		{
			if(ptr2 != ptr1)
				ptr2 = ptr2->next;
			ptr1 = ptr1->next;
		}
		
	}
	#endif
	
	if(ptr1 == NULL)
		wlan->ret = FAILED;
	ret = send_message(assemble_del_wlan_response, wlan);
	
	AP_KEY_EVENT_INFO *ap_key_event = NULL;
	AP_KEY_EVENT_INFO *ap_key_event_tmp = NULL;
	get_key_event_wlan_disable(&ap_key_event, wlan);
	send_message(assemble_report_key_event_wlan_disable_request, ap_key_event);
	while(ap_key_event != NULL)
	{
		ap_key_event_tmp = ap_key_event->next;
		FREE_OBJ(ap_key_event->BSSID);
		FREE_OBJ(ap_key_event);
		ap_key_event = ap_key_event_tmp;
	}
	
	FREE_OBJ(wlan);
	if(wlan_list == NULL){
		Wlan_ReportEnable = DISABLED;
		save_apctl_level_config("Wlan_ReportEnable",CHAR,(u8 *)&Wlan_ReportEnable,1);
	}
	pthread_mutex_unlock(&wlan_mutex);
	
	return ret;
}
/* parse add sta to blacklist message */
BOOL parse_add_sta_to_blacklist_request_msg(TMSG *msg)
{
	int i = 0;
	BLACK_LST *bl = NULL;
	//bl = (BLACK_LST *)malloc(sizeof(BLACK_LST));
	CREATE_OBJECT_ERR(bl,BLACK_LST,MALLOC_ERR_RET(FALSE));
	if(bl == NULL)
		return FALSE;
	bl->num = protocol_retrieve8(msg);
	//bl->maclst = (MAC_LST *)malloc(sizeof(MAC_LST)*(bl->num));
	CREATE_ARRAY_ERR(bl->maclst,bl->num,MAC_LST,MALLOC_ERR_RET(FALSE));
	if(bl->maclst == NULL)
	{
		FREE_OBJ(bl);
		return FALSE;
	}
	for(i = 0; i < bl->num; i++)
	{
		bl->maclst[i].ret = FAILED;
		bl->maclst[i].mac = (u8 *)protocol_retrieve_raw_bytes(msg, MAC_ADDR_LEN);
	}
	return add_cmd_to_list_tail(MSG_ELEMENT_ADD_STA_TO_BALCKLIST_REQUEST_TYPE, bl);
}
BOOL refresh_blacklist()
{
	FILE* fd=NULL;
	unsigned char mac [64] ={0}, filepath[256] = {0};

	macAddrFiltration *p = NULL;

	sprintf(filepath,"%s/StaBlackList",CONF_APCTL_PATH); 
	printf_d("%s,%d,StaBlackList saved in %s\n",__func__,__LINE__,filepath);
	if((fd = fopen(filepath,"w+"))==NULL)
	{ 
		printf_d("open file %s failed!\n",filepath);
		return FALSE;
	}
	else
	{
		for(p = StaBlackList;p != NULL; p = p->next)
		{
			memset(mac,0,64);
			sprintf(mac,"%02x:%02x:%02x:%02x:%02x:%02x\n",p->mac[0],p->mac[1],p->mac[2]
													,p->mac[3],p->mac[4],p->mac[5]);
			fputs(mac, fd);   
		}
		fclose(fd);
		printf_d("%s,%d,refresh the sta black list for backup ok.\n",__func__, __LINE__);
		return TRUE;
	}
}

BOOL del_macaddr_from_StaList(u8 *mac)
{
	STA_INFO *list_tmp;
	char cmd[128] = {0};
	if(sta_list != NULL)
	{
		for(list_tmp = sta_list; list_tmp != NULL; list_tmp = list_tmp->next)
		{
			if(macAddrCmp(mac, list_tmp->MAC) == 1)
			{
				sprintf(cmd, "iwpriv ath.%d-%d kickmac %02X:%02X:%02X:%02X:%02X:%02X",list_tmp->rid-1, list_tmp->wid, mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
				SYSTEM(cmd);
				sta_list = list_tmp->next;
				FREE_OBJ(list_tmp);
				return TRUE;
			}
		}
	}
	return FALSE;
}

BOOL add_macaddr_to_StaBlackList(u8 *mac)
{
	macAddrFiltration *list_tmp;
	if(StaBlackList != NULL)
	{
		for(list_tmp = StaBlackList; list_tmp != NULL; list_tmp = list_tmp->next)
		{
			if(macAddrCmp(mac, list_tmp->mac) == 1)
				return FALSE;
		}
	}
	list_tmp = NULL;
	//list_tmp = (macAddrFiltration *)malloc(sizeof(macAddrFiltration));
	CREATE_OBJECT_ERR(list_tmp,macAddrFiltration,MALLOC_ERR_RET(FALSE));
	memcpy((char *)list_tmp->mac, (char *)mac, MAC_ADDR_LEN);
	list_tmp->next = StaBlackList;
	StaBlackList = list_tmp;
	return TRUE;
}

BOOL manage_add_sta_to_blacklist_request_msg(BLACK_LST *bl)
{
	int i = 0;
	WLAN_INFO *wlan = NULL;
	BOOL ret = FALSE;
	char cmd[128] = {0};

	pthread_mutex_lock(&wlan_mutex);
	/* ioctl */
	for(i = 0; i < bl->num; i++)
	{
		printf_d("%03d  %02X:%02X:%02X:%02X:%02X:%02X\n", i, bl->maclst[i].mac[0], bl->maclst[i].mac[1], bl->maclst[i].mac[2], bl->maclst[i].mac[3], bl->maclst[i].mac[4], bl->maclst[i].mac[5]);
		wlan = wlan_list;
		/* one table enough */
		if(add_macaddr_to_StaBlackList(bl->maclst[i].mac) == TRUE)
		{
			while(wlan != NULL)
			{			
				snprintf(cmd, sizeof(cmd), "iwpriv ath.%d-%d addmac %02X:%02X:%02X:%02X:%02X:%02X", wlan->rid-1, wlan->wid, bl->maclst[i].mac[0], bl->maclst[i].mac[1], bl->maclst[i].mac[2], bl->maclst[i].mac[3], bl->maclst[i].mac[4], bl->maclst[i].mac[5]);
				SYSTEM(cmd);
				APVLog("add sta %02X:%02X:%02X:%02X:%02X:%02X to black_list!\n", bl->maclst[i].mac[0], bl->maclst[i].mac[1], bl->maclst[i].mac[2], bl->maclst[i].mac[3], bl->maclst[i].mac[4], bl->maclst[i].mac[5]);
				wlan = wlan->next;
			}
		}
		bl->maclst[i].ret = SUCCESSED;
	}
	refresh_blacklist();
	pthread_mutex_unlock(&wlan_mutex);
	pthread_mutex_lock(&sta_mutex);
	for(i = 0; i < bl->num; i++)
	{
		del_macaddr_from_StaList(bl->maclst[i].mac);
	}
	pthread_mutex_unlock(&sta_mutex);
	ret = send_message(assemble_add_del_sta_to_blacklist_response, bl);
	
	for(i = 0; i < bl->num; i++)
	{
		FREE_OBJ(bl->maclst[i].mac);
	}
	FREE_OBJ(bl->maclst);
	FREE_OBJ(bl);
	return ret;
}

/* parse add sta to blacklist message */
BOOL parse_del_sta_to_blacklist_request_msg(TMSG *msg)
{
	int i = 0;
	BLACK_LST *bl = NULL;
	//bl = (BLACK_LST *)malloc(sizeof(BLACK_LST));
	CREATE_OBJECT_ERR(bl,BLACK_LST,MALLOC_ERR_RET(FALSE));
	if(bl == NULL)
		return FALSE;
	bl->num = protocol_retrieve8(msg);
	//bl->maclst = (MAC_LST *)malloc(sizeof(MAC_LST)*(bl->num));
	CREATE_ARRAY_ERR(bl->maclst,bl->num,MAC_LST,MALLOC_ERR_RET(FALSE));
	if(bl->maclst == NULL)
	{
		FREE_OBJ(bl);
		return FALSE;
	}
	for(i = 0; i < bl->num; i++)
	{
		bl->maclst[i].ret = FAILED;
		bl->maclst[i].mac = (u8 *)protocol_retrieve_raw_bytes(msg, MAC_ADDR_LEN);
	}
	return add_cmd_to_list_tail(MSG_ELEMENT_DEL_STA_TO_BALCKLIST_REQUEST_TYPE, bl);
}

BOOL del_macaddr_to_StaBlackList(u8 *mac)
{
	macAddrFiltration *list_tmp;
	if(StaBlackList != NULL)
	{
		for(list_tmp = StaBlackList; list_tmp != NULL; list_tmp = list_tmp->next)
		{
			if(macAddrCmp(mac, list_tmp->mac) == 1)
			{
				StaBlackList = list_tmp->next;
				FREE_OBJ(list_tmp);
				return TRUE;
			}
		}
	}
	return FALSE;
}


BOOL manage_del_sta_to_blacklist_request_msg(BLACK_LST *bl)
{
	int i = 0;
	WLAN_INFO *wlan = NULL;
	BOOL ret = FALSE;
	char cmd[128] = {0};
	pthread_mutex_lock(&wlan_mutex);
	/* ioctl */
	for(i = 0; i < bl->num; i++)
	{
		printf_d("%03d  %02X:%02X:%02X:%02X:%02X:%02X\n", i, bl->maclst[i].mac[0], bl->maclst[i].mac[1], bl->maclst[i].mac[2], bl->maclst[i].mac[3], bl->maclst[i].mac[4], bl->maclst[i].mac[5]);
		wlan = wlan_list;
		while(wlan != NULL)
		{
			snprintf(cmd, sizeof(cmd), "iwpriv ath.%d-%d delmac %02X:%02X:%02X:%02X:%02X:%02X", wlan->rid-1, wlan->wid, bl->maclst[i].mac[0], bl->maclst[i].mac[1], bl->maclst[i].mac[2], bl->maclst[i].mac[3], bl->maclst[i].mac[4], bl->maclst[i].mac[5]);
			SYSTEM(cmd);
			wlan = wlan->next;
		}
		del_macaddr_to_StaBlackList(bl->maclst[i].mac);
		bl->maclst[i].ret = SUCCESSED;
	}
	refresh_blacklist();
	pthread_mutex_unlock(&wlan_mutex);
	pthread_mutex_lock(&sta_mutex);
	for(i = 0; i < bl->num; i++)
	{
		del_macaddr_from_StaList(bl->maclst[i].mac);
	}
	pthread_mutex_unlock(&sta_mutex);
	ret = send_message(assemble_add_del_sta_to_blacklist_response, bl);
	
	for(i = 0; i < bl->num; i++)
	{
		FREE_OBJ(bl->maclst[i].mac);
	}
	FREE_OBJ(bl->maclst);
	FREE_OBJ(bl);
	return ret;
}

/* echo */
void heartbeat_timer_expired(void *arg)
{
	int i =0;
	WLAN_INFO *temp = NULL;
	char wlanname[16] = {0};
	char wifiname[16] = {0};
	unsigned char  ifstate = 0;
	char cmd[64] = {0};
	send_message(assemble_echo_request, NULL);
	
	for (i=0;i<RADIO_NUM;i++)
	{
		if(gRadioInfoValue[i].radioState==DOWN)
		{
			pthread_mutex_lock(&wlan_mutex);
			for(temp = wlan_list; temp != NULL; temp = temp->next)
			{
				if(i == (temp->rid - 1))
				{
					sprintf(wlanname,"ath.%d-%d",temp->rid-1,temp->wid);
					GetInterfaceStatus(wlanname,&ifstate);
					if(ifstate!=2)
					{
						sprintf(cmd,"ifconfig %s down",wlanname);
						SYSTEM(cmd);
					}
				}
			}
			sprintf(wifiname,"wifi%d",i);
			GetInterfaceStatus(wifiname,&ifstate);
			if(ifstate!=2)
			{
				sprintf(cmd,"ifconfig wifi%d down",i);
				SYSTEM(cmd);
			}
			
			pthread_mutex_unlock(&wlan_mutex);
		}
	}
	if(overtime_count > ECHO_OVERTIME)
	{
		APLog("echo timer has expired,reboot!\n");
		SYSTEM("sysreboot");
	}
	else
	{
		start_heartbeat_timer();
	}
}

BOOL start_heartbeat_timer()
{
	if(heartbeat_timer != -1)
		timer_rem(heartbeat_timer, 0);
	heartbeat_timer = timer_add(echotime, 0, heartbeat_timer_expired, NULL);
	if(0 == access("/tmp/zt_state", F_OK))
		overtime_count++;
	if(heartbeat_timer == -1)
		return FALSE;
	return TRUE;
}

BOOL stop_heartbeat_timer()
{	
	timer_rem(heartbeat_timer,0);
	return TRUE;
}


BOOL ResetTimers()
{
	time_now();
	if(!stop_heartbeat_timer()) 
	{
		APVLog("WTP Stop HeartbeatTime error!\n");
		return FALSE;
	}
	if(!start_heartbeat_timer()) 
	{
		APVLog("WTP Start HeartbeatTime error!\n");
		return FALSE;
	}
	
	return TRUE;
}


/* UPGRADE */
/* upgrade ap fireware */
BOOL parse_upgrade_request_msg(TMSG *msg)
{
	u16      elemID = 0;
	u16      elemlen = 0;
	UPGRADE_ARG *uparg = NULL;
	uparg = (UPGRADE_ARG *)malloc(sizeof(UPGRADE_ARG));
	if(uparg == NULL)
		return RAISE_ERROR(ERROR_OUT_OF_MEMORY, NULL);
	memset(uparg, 0, sizeof(UPGRADE_ARG));
	uparg->type          = protocol_retrieve16(msg);
	uparg->len           = protocol_retrieve16(msg);
	uparg->ver           = (u8 *)protocol_retrieve_str(msg, uparg->len);
	while(msg->offset < msg->len)
	{
		elemID               = protocol_retrieve16(msg);
		elemlen              = protocol_retrieve16(msg);
		switch(elemID)
		{
			case 32:                                   /* download fireware base on protocol */
				uparg->transproto = protocol_retrieve8(msg);
				break;
			case 33:                                   /* ip address of the server that to save fireware */
				uparg->serverip = protocol_retrieve32(msg);
			break;
			case 35:                                   /* url of the server that to save fireware */
				uparg->url = (u8 *)protocol_retrieve_str(msg, elemlen);
				break;
			case 36:                                   /* user name */
				uparg->username = (u8 *)protocol_retrieve_str(msg, elemlen);
				break;
			case 37:                				   /* passwd */
				uparg->passwd = (u8 *)protocol_retrieve_str(msg, elemlen);
				break;
			default:
				msg->offset += elemlen;
				break;
		}
	}
	
	return add_cmd_to_list_tail(MSG_ELEMENT_UPGRADE_REQUEST_TYPE, uparg);
}
void upgrade_timeout(void *arg)
{
	u8 ret = (u8)UPGRADE_TIMEOUT;
	send_message(assemble_upgrade_response, &ret);
	time_upgrade = -1;
	pthread_cancel(tid_upgrade);
}
void *thread_upgrade(void *arg)
{
	char cmd[256] = {0};
	char ipstr[32] = {0};
	char fireware[64] = {0};
	char url[32] = {0};
	char username[32] = {0};
	char passwd[32] = {0};
	struct in_addr addr;
	u8 ret = (u8)UPGRADE_TIMEOUT;
	UPGRADE_ARG *uparg = (UPGRADE_ARG *)arg;
	addr.s_addr = htonl(uparg->serverip);
	pthread_detach(pthread_self());
	/* */
	time_upgrade = timer_add(UPGRADE_TIMEOUT_TIME, 0, upgrade_timeout, NULL);
	if(time_upgrade == -1)
		return NULL;
	
	inet_ntop(AF_INET, (void *)&addr, ipstr, sizeof(ipstr));
	snprintf(fireware, sizeof(fireware), "Fireware%s.img", uparg->ver);
	snprintf(url, sizeof(url), "%s", uparg->url);
	snprintf(username, sizeof(username), "%s", uparg->username);
	snprintf(passwd, sizeof(passwd), "%s", uparg->passwd);
	
	
	snprintf(cmd, sizeof(cmd), "cd /tmp && ftpget -u %s -p %s %s %s %s && touch upgrade_flag", 
		     username, passwd, ipstr, fireware, url);
	/*
	snprintf(cmd, sizeof(cmd), "cd /tmp && ftpget -u %s -p %s %s %s %s && touch upgrade_flag", 
		     username, passwd, ipstr, url, fireware);
	*/
	
	SYSTEM(cmd);
	if(0 == access("/tmp/upgrade_flag", F_OK))
	{
		ret = UPGRADE_FTP_SUCCESSED;
		send_message(assemble_upgrade_response, &ret);
		snprintf(cmd, sizeof(cmd), "cd /tmp && sysupgrade %s", fireware);
		SYSTEM(cmd);
		ret = UPGRADE_SUCCESSED;
		send_message(assemble_upgrade_response, &ret);
		timer_rem(time_upgrade, 0);
		time_upgrade = -1;
		APLog("upgrade to %s successfully,reboot!\n",uparg->ver);
		SYSTEM("sysreboot");
	}
	else
	{
		ret = UPGRADE_INVALIDED_VER;
		send_message(assemble_upgrade_response, &ret);
		timer_rem(time_upgrade, 0);
		time_upgrade = -1;
	}

	FREE_OBJ(uparg->url);
	FREE_OBJ(uparg->username);
	FREE_OBJ(uparg->passwd);
	FREE_OBJ(uparg->ver);
	FREE_OBJ(uparg);

	
	return NULL;
}
BOOL manage_upgrade_request_msg(UPGRADE_ARG *uparg)
{
	if(strcmp((char *)uparg->ver, SOFTWARE_VER) == 0)
	{
		printf_d("The current version is lastest, don't need to upgrade!\n");
		APVLog("The current version is lastest, don't need to upgrade!\n");
	}
	else
	{
		if((tid_upgrade == 0) || (ESRCH == pthread_kill(tid_upgrade, 0)))
		{
			if(pthread_create(&tid_upgrade, NULL, thread_upgrade, uparg) != 0)
			{
				printf_d("create thread failed for upgrade!\n");
				APVLog("create thread failed for upgrade!\n");
				return FALSE;
			}
		}
	}
	return TRUE;
}

/* parse set NTP message */
BOOL parse_set_ntp_request_msg(TMSG *msg)
{
	u16      elemID = 0;
	u16      elemlen = 0;
	NTP_SET *ntp = NULL;
	ntp = (NTP_SET *)malloc(sizeof(NTP_SET));
	if(ntp == NULL)
		return RAISE_ERROR(ERROR_OUT_OF_MEMORY, NULL);
	memset(ntp, 0, sizeof(NTP_SET));
	ntp->ver             = protocol_retrieve16(msg);
	ntp->seq             = protocol_retrieve8(msg);
	msg->offset         += 1;
	while(msg->offset < msg->len)
	{
		elemID               = protocol_retrieve16(msg);
		elemlen              = protocol_retrieve16(msg);
		switch(elemID)
		{
			case 1:
				ntp->serverip = protocol_retrieve32(msg);
				break;
			case 2:
				ntp->syninterval = protocol_retrieve16(msg);
				break;
			default:
				msg->offset += elemlen;
				break;
		}
	}
	if(ntp->serverip == 0)
		return FALSE;
	return add_cmd_to_list_tail(MSG_ELEMENT_SET_NTP_REQUEST_TYPE, ntp);
}
BOOL manage_set_ntp_request_msg(NTP_SET *ntp)
{
	char cmd[64] = {0};
	char ipstr[32] = {0};
	BOOL ret = FALSE;
	SYSTEM("killall -9 ntpclient");
	if(ntp->serverip == 0)
	{
		ntp->serverip = ntpserverip;
	}
	if(ntp->syninterval == 0)
	{
		snprintf(cmd, sizeof(cmd), "ntpclient -h %s -s&", inet_inttostr(ipstr, sizeof(ipstr), ntp->serverip));
	}
	else
	{
		snprintf(cmd, sizeof(cmd), "ntpclient -h %s -i %d -s&", inet_inttostr(ipstr, sizeof(ipstr), ntp->serverip), ntp->syninterval);
	}
	SYSTEM(cmd);
	ntp->ret = SUCCESSED;
	
	ret = send_message(assemble_set_ntp_response, ntp);

	FREE_OBJ(ntp);	
	
	return ret;
}

/* Begin: Add by dongzw for station isolation 2012-11-21 */
BOOL parse_set_isolation_request_msg(TMSG *msg)
{
	ISOLATION_SET *isolation = NULL;
	u8 data;
	
	CREATE_OBJECT_ERR(isolation, ISOLATION_SET, MALLOC_ERR_RET(FALSE));
	if(isolation == NULL)
		return RAISE_ERROR(ERROR_OUT_OF_MEMORY, NULL);
	
	isolation->ver = protocol_retrieve16(msg);
	isolation->seq = protocol_retrieve8(msg);
	msg->offset += 1;
	while (msg->offset < msg->len) {
		isolation->type = protocol_retrieve16(msg);
		isolation->length = protocol_retrieve16(msg);
		data = protocol_retrieve8(msg);
		switch (data >> 4) {
			case 0:
				isolation->value = 0;		/* closed isolation */
				break;
			case 1:
				isolation->value = 1;		/* opened isolation for user */
				break;
			case 2:
			case 4:
				isolation->value = 2;		/* opened isolation for multicast and broadcast */
				break;
			case 8:
				isolation->value = 4;		/* opened isolation for all */
				break;
			default:
				isolation->value = 15;		/* isolation set error */
				break;
		}
	}

	return add_cmd_to_list_tail(MSG_ELEMENT_SET_ISOLATION_REQUEST_TYPE, isolation);

}
/* End: Add by dongzw for station isolation 2012-11-21 */

/* Begin: Add by dongzw for station isolation 2012-11-21 */
BOOL manage_set_isolation_request_msg(ISOLATION_SET *isolation)
{
	WLAN_INFO *pwlan = NULL;
	char cmd[64] = {0};
	BOOL ret = FALSE;

	if (isolation->value == 0 || isolation->value == 1 || isolation->value == 2 || isolation->value == 4) {
		for (pwlan = wlan_list; pwlan != NULL; pwlan = pwlan->next) {
			snprintf(cmd, sizeof(cmd), "iwpriv ath.%d-%d switch %d", pwlan->rid - 1, pwlan->wid, isolation->value);
			SYSTEM(cmd);
		}
		isolation->ret = SUCCESSED;
	} else {
		isolation->ret = FAILED;
	}
	
	ret = send_message(assemble_set_isolation_response, isolation);

	FREE_OBJ(isolation);	
	
	return ret;
}
/* End: Add by dongzw for station isolation 2012-11-21 */

/* parse sys maintenance message */
BOOL parse_sys_maintenance_requset_msg(TMSG *msg)
{
	MAINTENANCE_OP *maintenance = NULL;
	//maintenance = (MAINTENANCE_OP *)malloc(sizeof(MAINTENANCE_OP));
	CREATE_OBJECT_ERR(maintenance,MAINTENANCE_OP,MALLOC_ERR_RET(FALSE));
	if(maintenance == NULL)
		return RAISE_ERROR(ERROR_OUT_OF_MEMORY, NULL);
	
	maintenance->ver = protocol_retrieve16(msg);
	maintenance->seq = protocol_retrieve8(msg);
	msg->offset += 1;
	while(msg->offset < msg->len)
	{
		maintenance->type = protocol_retrieve16(msg);
		maintenance->length = protocol_retrieve16(msg);
		maintenance->value = protocol_retrieve8(msg);				//maintenance opration
	}
	return add_cmd_to_list_tail(MSG_ELEMENT_SYS_MAINTENANCE_REQUEST_TYPE, maintenance);
}
BOOL manage_sys_maintenance_requset_msg(MAINTENANCE_OP *maintenance)
{
	if(maintenance->value == 1)			//reboot
	{
		maintenance->value = SUCCESSED;
		send_message(assemble_sys_maintenance_response, maintenance);
		FREE_OBJ(maintenance);
		APLog("AC set AP reboot!\n");
		SYSTEM("/usr/sbin/ac-lose");
		SYSTEM("sysreboot");
	}
	else if(maintenance->value == 2)	//restore factory
	{
		APLog("AC restored AP factory!\n");
		SYSTEM("sysfactoryreset");
	}
	maintenance->value = SUCCESSED;
	send_message(assemble_sys_maintenance_response, maintenance);
	FREE_OBJ(maintenance);
	return TRUE;
}

/* parse set radio message */
BOOL parse_set_radio_requset_msg(TMSG *msg)
{
	int i;
	SET_RADIO_PARAMETER *radio_parameter = NULL;
	RADIO_INFO *head = NULL;
	//radio_parameter = (SET_RADIO_PARAMETER *)malloc(sizeof(SET_RADIO_PARAMETER));
	CREATE_OBJECT_ERR(radio_parameter,SET_RADIO_PARAMETER,MALLOC_ERR_RET(FALSE));
	if(radio_parameter == NULL)
		return RAISE_ERROR(ERROR_OUT_OF_MEMORY, NULL);
	

	radio_parameter->ver	= protocol_retrieve16(msg);
	radio_parameter->seq	= protocol_retrieve8(msg);
	radio_parameter->radio_info = NULL;

	msg->offset 		+= 1;
	while(msg->offset < msg->len)
	{
		if(radio_parameter->radio_info == NULL)
		{
			//radio_parameter->radio_info = (RADIO_INFO *)malloc(sizeof(RADIO_INFO));		
			CREATE_OBJECT_ERR(radio_parameter->radio_info,RADIO_INFO,MALLOC_ERR_RET(FALSE));
			radio_parameter->radio_info->next = NULL;
			head = radio_parameter->radio_info;
		}
		else
		{
			//radio_parameter->radio_info->next = (RADIO_INFO *)malloc(sizeof(RADIO_INFO));
			CREATE_OBJECT_ERR(radio_parameter->radio_info->next,RADIO_INFO,MALLOC_ERR_RET(FALSE));
			radio_parameter->radio_info = radio_parameter->radio_info->next;
			radio_parameter->radio_info->next = NULL;
		}
		if(radio_parameter->radio_info == NULL)
				return RAISE_ERROR(ERROR_OUT_OF_MEMORY, NULL);
		
		radio_parameter->radio_info->type = protocol_retrieve16(msg);
		radio_parameter->radio_info->length = protocol_retrieve16(msg);
		radio_parameter->radio_info->radio_id = protocol_retrieve8(msg);
		/* radio_parameter->radio_info->radio_type = protocol_retrieve32(msg); */
		printf_d("set radio infot the type is %d\n", radio_parameter->radio_info->type);
		APVLog("set radio infot the type is %d\n", radio_parameter->radio_info->type);
		switch(radio_parameter->radio_info->type)
		{
			case 1:
				radio_parameter->radio_info->radio_type = protocol_retrieve32(msg);	//radio type
				printf_d("parse radioID:%d radio_type:%d\n",radio_parameter->radio_info->radio_id,radio_parameter->radio_info->radio_type);
				break;
			case 2:
				radio_parameter->radio_info->radio_type = protocol_retrieve8(msg);	//power type
				radio_parameter->radio_info->value = protocol_retrieve16(msg);		//power
				break;
			case 3:
				radio_parameter->radio_info->value = protocol_retrieve16(msg);		//work channel
				break;
			case 5:
				radio_parameter->radio_info->value = protocol_retrieve16(msg);		//(mode 11n) wireless rate set
				break;
			case 6:
				radio_parameter->radio_info->value = protocol_retrieve8(msg);			//space flow
				break;
			case 7:
				radio_parameter->radio_info->value = protocol_retrieve8(msg);			//(mode 11n) A-MPDU switch
				break;
			case 8:
				radio_parameter->radio_info->value = protocol_retrieve8(msg);			//(mode 11n) channel Bandwidth
				break;
			case 9:
				radio_parameter->radio_info->value = protocol_retrieve8(msg);			//(mode 11n) short Guard Interval
				break;
			case 10:
				radio_parameter->radio_info->value = protocol_retrieve8(msg);			//(mode 11n) HT-Greenfield HT-Mixed
				break;
			case 11:
				radio_parameter->radio_info->value = protocol_retrieve8(msg);			//(mode 11n) A-MSDU switch
				break;
			case 12:
				radio_parameter->radio_info->value = protocol_retrieve16(msg);		//becone interval
				break;
			case 13:
				radio_parameter->radio_info->value = protocol_retrieve8(msg);			//DTIM interval
				break;
			case 14:
				radio_parameter->radio_info->value = protocol_retrieve16(msg);		//divided threshold value
				break;
			case 15:
				radio_parameter->radio_info->value = protocol_retrieve16(msg);		//RTS threshold value
				break;
			case 16:
				radio_parameter->radio_info->value = protocol_retrieve8(msg);			//short preamble
				break;
			case 21:
			case 23:
				//radio_parameter->radio_info->rate = (u16 *)malloc((radio_parameter->radio_info->length-1)*sizeof(u16));
				//radio_parameter->radio_info->f = (u8 *)malloc((radio_parameter->radio_info->length-1)*sizeof(char));
				CREATE_ARRAY_ERR(radio_parameter->radio_info->rate,(radio_parameter->radio_info->length-1),u16,MALLOC_ERR_RET(FALSE));
				CREATE_ARRAY_ERR(radio_parameter->radio_info->f ,(radio_parameter->radio_info->length-1),u8,MALLOC_ERR_RET(FALSE));
				for(i = 0; i < (radio_parameter->radio_info->length-1); i++)
				{
					radio_parameter->radio_info->rate[i] = protocol_retrieve8(msg);			//rate set
					radio_parameter->radio_info->f[i] = radio_parameter->radio_info->rate[i] >> 7;
					printf_d("parse radio->rate[%d]:%d (with f mark)\n", i, radio_parameter->radio_info->rate[i]);
					printf_d("radio->f[%d]:%d\n", i, radio_parameter->radio_info->f[i]);
				}
				break;
			default:
				printf_d("unrecognised set radio type\n");
				msg->offset += radio_parameter->radio_info->length - 1;
				break;
		}
	}
	radio_parameter->radio_info = head;
	return add_cmd_to_list_tail(MSG_ELEMENT_SET_RADIO_REQUEST_TYPE, radio_parameter);
}

int get_txpower(int n, u8 rid)
{
	char cmd[512] = {0};
	char str[32] = {0};
	int txpower = 0;
	double m = 0.0;

	if(0 != access(DEVINFO_PATH, F_OK) || RADIO_COUNT <= 0)
		return 0;
	
	sprintf(cmd, "grep \"<wifi_total>\" -A 17 %s | grep \"<wifi%d>\" -A 6 | awk -F \"[<>]\" '/max_power/{print $3}'", DEVINFO_PATH, rid);
	if(popen_fgets(cmd, str, sizeof(str)) == NULL)
		return 0;
	
	m = (double)atoi(str)/10.0;
	txpower = 10*log(((int)pow(10, m)*n)/2000)/log(10);
	if(debugf == TRUE)
	{
		printf("max_txpower = %s dbm, max_txpower = %f mw, txpower = %d\n", str, pow(10, m), txpower);
	}
	return txpower;
}

BOOL set_radio_info(RADIO_INFO *radio)
{
	char cmdbuf[128]={0};
	int i = 0;
	BOOL bol_exists_wlan = FALSE;
	u16 *rates = NULL;
	u8 *ratef = NULL;
	char wlanname[16] = {0};
	AP_KEY_EVENT_INFO *key_event = NULL;
	BOOL bol_radio_up = TRUE;
	BOOL bol_radio_txpower = TRUE;
	WLAN_INFO *pwlan = NULL;

	sta_limit(radio->radio_id - 1);

	#ifndef AR11N
		bol_radio_txpower = FALSE;
	#endif
	if((bol_radio_txpower == FALSE) && (radio->type == 2) && ((radio->radio_type == 1) || (radio->radio_type == 2)))
	{
	}
	else
		set_radio_state(DISABLED, radio->radio_id);

	pthread_mutex_lock(&wlan_mutex);
	bol_exists_wlan = get_wlan_if_by_rid(radio->radio_id, wlanname);
	switch(radio->type)
	{
		case 1: 	//set radio mode
			dprintff("set radio mode", '=', FORMAT_STR_LEN);
			/* gRadioInfoValue[radio->radio_id-1].channel = 0; */
			gRadioInfoValue[radio->radio_id-1].radiotype = radio->radio_type;
			save_radio_level_config(radio->radio_id-1,"radiotype",INT,(int *)&gRadioInfoValue[radio->radio_id-1].radiotype,1);
			if(bol_exists_wlan == TRUE)
			{
				set_radio_mode(radio->radio_id);
			}
			break;
		case 2: 	//set txpower
			dprintff("set txpower", '=', FORMAT_STR_LEN);
			printf_d("radio->radio_type = %d\n", radio->radio_type);
			switch(radio->radio_type)
			{
				case 0:
					/* !!! auto */
					if(radio->value == 0)		//auto open
					{
						gRadioInfoValue[ radio->radio_id-1].auto_txpower = ENABLED;
						save_radio_level_config(radio->radio_id-1,"auto_txpower",CHAR,(u8*)&gRadioInfoValue[radio->radio_id-1].auto_txpower,1);
						printf_d("open txpower auto\n");
						//sprintf(cmdbuf,"/usr/sbin/auto_power2 ath%d inc", radio->radio_id-1);
					}
					else if(radio->value == 1)	//auto close
					{
						gRadioInfoValue[ radio->radio_id-1].auto_txpower = DISABLED;
						save_radio_level_config(radio->radio_id-1,"auto_txpower",CHAR,(u8*)&gRadioInfoValue[radio->radio_id-1].auto_txpower,1);
						printf_d("close txpower auto\n");
					}
					break;
				case 1:
					printf_d("set txpower pass rate\n");
					if(radio->value == 0)
					{
						printf_d("txpower is 0\n");
						gRadioInfoValue[radio->radio_id-1].txpower = 3;
						save_radio_level_config(radio->radio_id-1,"txpower",INT,(u32*)&gRadioInfoValue[radio->radio_id-1].txpower,1);
						if(bol_exists_wlan == TRUE)
						{
							sprintf(cmdbuf, "settxpower %s %d", wlanname, 3);
							SYSTEM(cmdbuf);
						}
					}
					else
					{
						gRadioInfoValue[radio->radio_id-1].txpower = get_txpower(radio->value, radio->radio_id-1);
						save_radio_level_config(radio->radio_id-1,"txpower",INT,(u32*)&gRadioInfoValue[radio->radio_id-1].txpower,1);
						if(bol_exists_wlan == TRUE)
						{
							if(gRadioInfoValue[radio->radio_id-1].txpower <= 3)
								sprintf(cmdbuf, "settxpower %s %d", wlanname, 3);
							else
								sprintf(cmdbuf, "settxpower %s %d", wlanname, gRadioInfoValue[radio->radio_id-1].txpower);
							SYSTEM(cmdbuf);
						}
					}
					break;
				case 2:
					radio->value = 10*log(radio->value)/log(10);	//mW to dBm
					gRadioInfoValue[ radio->radio_id-1].txpower = radio->value;
					if(bol_exists_wlan == TRUE)
					{
						if(radio->value <= 3)
							sprintf(cmdbuf,"settxpower %s %d", wlanname, 3);
						else
							sprintf(cmdbuf,"settxpower %s %d", wlanname, radio->value);
						SYSTEM(cmdbuf);
					}
					break;
				case 4:
					/* how to close radio */
					if(radio->value == 0)		//open
					{
						if(gRadioInfoValue[radio->radio_id-1].radioState == DOWN)
						{
							gRadioInfoValue[radio->radio_id-1].radioState = UP;					
							gRadioInfoValue[radio->radio_id-1].wifi_start_time = info_report_get_current_time();
							gRadioInfoValue[radio->radio_id-1].wifi_current_time = gRadioInfoValue[radio->radio_id-1].wifi_start_time;
							int rid =radio->radio_id-1;
							save_radio_level_config(rid,"wifi_start_time",UL,(unsigned long*)&(gRadioInfoValue[rid].wifi_start_time),1);
							save_radio_level_config(rid,"wifi_current_time",UL,(unsigned long*)&gRadioInfoValue[rid].wifi_current_time,1);
							sprintf(cmdbuf,"ifconfig wifi%d up", radio->radio_id-1);
							SYSTEM(cmdbuf);
						}				
						#if 0
						if(bol_exists_wlan == TRUE)
						{
							for(ptr=wlan_list;ptr != NULL;ptr = ptr->next)
							{
								if(ptr->rid == radio->radio_id)
								{
									ptr->wlan_updown_time++;
									sprintf(cmdbuf,"ifconfig ath.%d-%d up", ptr->rid-1, ptr->wid);
									SYSTEM(cmdbuf);
								}
							}		
						}
						#endif
					}
					else if(radio->value == 1)	//close
					{
						#if 0
						if(bol_exists_wlan == TRUE)
						{
							for(ptr=wlan_list;ptr != NULL;ptr = ptr->next)
							{
								if(ptr->rid == radio->radio_id)
								{
									ptr->wlan_updown_time++;
									sprintf(cmdbuf,"ifconfig ath.%d-%d down", ptr->rid-1, ptr->wid);
									SYSTEM(cmdbuf);
								}
							}
						}
						#endif
						if(gRadioInfoValue[radio->radio_id-1].radioState == UP)
						{
							gRadioInfoValue[radio->radio_id-1].radioState = DOWN;
							gRadioInfoValue[radio->radio_id-1].wifi_start_time = info_report_get_current_time();
							gRadioInfoValue[radio->radio_id-1].wifi_current_time = gRadioInfoValue[radio->radio_id-1].wifi_start_time;
							int rid =radio->radio_id-1;
							save_radio_level_config(rid,"wifi_start_time",UL,(unsigned long*)&gRadioInfoValue[rid].wifi_start_time,1);
							save_radio_level_config(rid,"wifi_current_time",UL,(unsigned long*)&gRadioInfoValue[rid].wifi_current_time,1);
							sprintf(cmdbuf,"ifconfig wifi%d down", radio->radio_id-1);
							SYSTEM(cmdbuf);
						}
						bol_radio_up = FALSE;
					}
					else
					{
						printf_d("set radio down/up failed!\n");
					}

					//key_event = (AP_KEY_EVENT_INFO *)malloc(sizeof(AP_KEY_EVENT_INFO));
					CREATE_OBJECT_ERR(key_event,AP_KEY_EVENT_INFO,MALLOC_ERR_RET(FALSE));
					if(key_event == NULL)
					{
						pthread_mutex_unlock(&wlan_mutex);
						return RAISE_ERROR(ERROR_OUT_OF_MEMORY, NULL);
					}
					key_event->next = NULL;
					key_event->radio_id = radio->radio_id;
					key_event->radio_frequency_switch = radio->value;
					key_event->switch_reason = 1;/* AC force up/down */
					send_message(assemble_report_key_event_radio_frequency_request, key_event);
					FREE_OBJ(key_event);
					break;
				default:
					break;
			}	
			break;
		case 3: 	//set channel
			dprintff("set channel", '=', FORMAT_STR_LEN);
			gRadioInfoValue[ radio->radio_id-1].channel = radio->value;
			save_radio_level_config(radio->radio_id-1,"channel",CHAR,(u8*)&gRadioInfoValue[radio->radio_id-1].channel,1);
			if(bol_exists_wlan == TRUE)
			{
				sprintf(cmdbuf,"iwconfig %s channel %d", wlanname, radio->value);
				SYSTEM(cmdbuf);
			}
			break;
		case 5: 	//11N rate set
			dprintff("11n rate set", '=', FORMAT_STR_LEN);
			memset(cmdbuf, 0, 128);
			if(radio->value == 0)	//auto
			{	
				sprintf(cmdbuf,"iwpriv wifi%d set_mcs_mask 0x%x",  radio->radio_id-1, 0x0000FFFF);
				gRadioInfoValue[ radio->radio_id-1].rate = 0x0000FFFF;
			}
			else
			{
				radio->value = 1<<(radio->value-1); //just one index
				gRadioInfoValue[ radio->radio_id-1].rate = radio->value&0x0000FFFF;
				sprintf(cmdbuf,"iwpriv wifi%d set_mcs_mask 0x%x",  radio->radio_id-1, radio->value&0x0000FFFF);
			}
			SYSTEM(cmdbuf);
			memset(cmdbuf, 0, 128);
			sprintf(cmdbuf,"iwpriv wifi%d set_mcs_cap 0x%x",  radio->radio_id-1, 0x80);
			SYSTEM(cmdbuf); 		
			break;
		case 6: 	//TX/RX Chianmask
			dprintff("TX/RX Chianmask", '=', FORMAT_STR_LEN);
			if(radio->value ==1 )
				gRadioInfoValue[ radio->radio_id-1].chianmask = 1;
			else if( radio->value==2 )
				gRadioInfoValue[ radio->radio_id-1].chianmask = 3;
			else if( radio->value==3 )
				gRadioInfoValue[ radio->radio_id-1].chianmask = 7;
			save_radio_level_config(radio->radio_id-1,"chainmask",CHAR,(u8*)&gRadioInfoValue[radio->radio_id-1].chianmask,1);
			sprintf(cmdbuf,"iwpriv wifi%d rxchainmask %d",	radio->radio_id-1,gRadioInfoValue[ radio->radio_id-1].chianmask); 
			SYSTEM(cmdbuf);
			sprintf(cmdbuf,"iwpriv wifi%d txchainmask %d",	radio->radio_id-1, gRadioInfoValue[ radio->radio_id-1].chianmask);
			SYSTEM(cmdbuf);
			break;
		case 7: 	//A-MPDU
			dprintff("A-MPDU", '=', FORMAT_STR_LEN);
			gRadioInfoValue[ radio->radio_id-1].AMPDU = radio->value;
			save_radio_level_config(radio->radio_id-1,"AMPDU",CHAR,(u8*)&gRadioInfoValue[radio->radio_id-1].AMPDU,1);
			/* 1-open 0-close */
			sprintf(cmdbuf,"iwpriv wifi%d AMPDU %d > /dev/null",  radio->radio_id-1, radio->value);
			SYSTEM(cmdbuf);
			if(bol_exists_wlan == TRUE) {
				sprintf(cmdbuf,"iwpriv %s ampdu %d > /dev/null",  wlanname, radio->value);
				SYSTEM(cmdbuf);
			}
			break;
		case 8: 	//channel bandwidth
			dprintff("channel bandwidth", '=', FORMAT_STR_LEN);
			gRadioInfoValue[radio->radio_id-1].channel_width = radio->value;
			save_radio_level_config(radio->radio_id-1,"channel_width",CHAR,(u8*)&gRadioInfoValue[radio->radio_id-1].channel_width,1);
			if(bol_exists_wlan == TRUE)
			{
				set_radio_mode(radio->radio_id);
			}
			break;
		case 9: 	//short GI(Guard Interval)
			dprintff("short GI(Guard Interval)", '=', FORMAT_STR_LEN);
			gRadioInfoValue[ radio->radio_id-1].shortGI = radio->value;
			save_radio_level_config(radio->radio_id-1,"shortGI",CHAR,(u8*)&gRadioInfoValue[radio->radio_id-1].shortGI,1);
			if(bol_exists_wlan == TRUE)
			{
				sprintf(cmdbuf, "iwpriv %s shortgi %d", wlanname, radio->value);
				SYSTEM(cmdbuf);
			}
			break;
		case 10:	//HT-greenfield/HT-Mixed
			dprintff("HT-greenfield/HT-Mixed", '=', FORMAT_STR_LEN);
			/* Begin: Modified by dongzw for fix HT-Greenfield/HT-Mixed configuration failure bug 2012-12-11 AZT-68 */
			gRadioInfoValue[radio->radio_id-1].HTMixed = radio->value;
			save_radio_level_config(radio->radio_id-1,"HTMixed",CHAR,(u8*)&gRadioInfoValue[radio->radio_id-1].HTMixed,1);
			if(bol_exists_wlan == TRUE) {
				for(pwlan = wlan_list; pwlan != NULL; pwlan = pwlan->next) {
					if(pwlan->rid == radio->radio_id) {
						snprintf(wlanname, 16, "ath.%d-%d", pwlan->rid-1, pwlan->wid);
						sprintf(cmdbuf, "iwpriv %s puren %d", wlanname, radio->value);
						SYSTEM(cmdbuf);
					}
				}
			}
			/* End: Modified by dongzw for fix HT-Greenfield/HT-Mixed configuration failure bug 2012-12-11 AZT-68 */
			break;
		case 11:	//A-MSDU
			dprintff("A-MSDU", '=', FORMAT_STR_LEN);
			gRadioInfoValue[ radio->radio_id-1].AMSDU = radio->value;
			save_radio_level_config(radio->radio_id-1,"AMSDU",CHAR,(u8*)&gRadioInfoValue[radio->radio_id-1].AMSDU,1);
			sprintf(cmdbuf,"iwpriv wifi%d AMSDU %d > /dev/null",  radio->radio_id-1, radio->value);
			SYSTEM(cmdbuf);
			if(bol_exists_wlan == TRUE) {
				sprintf(cmdbuf,"iwpriv %s amsdu %d > /dev/null",  wlanname, radio->value);
				SYSTEM(cmdbuf);
			}
			break;
		case 12:	//becone interval		
			dprintff("becone interval", '=', FORMAT_STR_LEN);
			gRadioInfoValue[radio->radio_id-1].beaconInterval = radio->value;
			save_radio_level_config(radio->radio_id-1,"beaconInterval",SHORT,(u16*)&gRadioInfoValue[radio->radio_id-1].beaconInterval,1);
			if(bol_exists_wlan == TRUE)
			{
				sprintf(cmdbuf,"iwpriv %s bintval %d",wlanname, radio->value);
				SYSTEM(cmdbuf);
			}
			break;
		case 13:	//DTIM
			dprintff("DTIM", '=', FORMAT_STR_LEN);
			gRadioInfoValue[radio->radio_id-1].dtim = radio->value;
			save_radio_level_config(radio->radio_id-1,"dtim",CHAR,(u8*)&gRadioInfoValue[radio->radio_id-1].dtim,1);
			if(bol_exists_wlan == TRUE)
			{
				sprintf(cmdbuf,"iwpriv %s dtim_period %d", wlanname, radio->value);
				SYSTEM(cmdbuf);
			}
			
			break;
		case 14:	//devided threshold
			dprintff("fragment threshold", '=', FORMAT_STR_LEN);
			gRadioInfoValue[radio->radio_id-1].fragThreshold = radio->value;
			save_radio_level_config(radio->radio_id-1,"fragThreshold",SHORT,(u16*)&gRadioInfoValue[radio->radio_id-1].fragThreshold,1);
			if(bol_exists_wlan == TRUE)
			{
				sprintf(cmdbuf,"iwconfig %s frag %d",wlanname, radio->value);
				SYSTEM(cmdbuf);
			}
			
			break;
		case 15:	//RTS threshold
			dprintff("RTS threshold", '=', FORMAT_STR_LEN);
			gRadioInfoValue[radio->radio_id-1].rtsThreshold = radio->value;
			save_radio_level_config(radio->radio_id-1,"rtsThreshold",SHORT,(u16*)&gRadioInfoValue[radio->radio_id-1].rtsThreshold,1);
			if(bol_exists_wlan == TRUE)
			{
				for(pwlan = wlan_list; pwlan != NULL; pwlan = pwlan->next)
				{
					if(pwlan->rid == radio->radio_id)
					{
						snprintf(wlanname, 16, "ath.%d-%d", pwlan->rid-1, pwlan->wid);
						sprintf(cmdbuf,"iwconfig %s rts %d",wlanname, radio->value);
						SYSTEM(cmdbuf);
					}
				}
			}
			break;
		case 16:	//short preamble
			dprintff("short preamble", '=', FORMAT_STR_LEN);
			gRadioInfoValue[ radio->radio_id-1].preamble = radio->value;
			save_radio_level_config(radio->radio_id-1,"preamble",CHAR,(u8*)&gRadioInfoValue[radio->radio_id-1].preamble,1);
			if(bol_exists_wlan == TRUE)
			{
				sprintf(cmdbuf,"iwpriv %s shpreamble %d", wlanname, radio->value);
				SYSTEM(cmdbuf);
			}
			break;
		case 21:	//rate set
			FREE_OBJ(rs.f);
			FREE_OBJ(rs.rate);
			rs.count = 0;
		case 23:	//extern rate set
			dprintff("rate set", '=', FORMAT_STR_LEN);
			//rates = (u16 *)malloc((rs.count +radio->length-1)*sizeof(u16));
			//ratef = malloc((rs.count +radio->length-1)*sizeof(u8));
			CREATE_ARRAY_ERR(rates,(rs.count +radio->length-1),u16,MALLOC_ERR_RET(FALSE));
			CREATE_ARRAY_ERR(ratef,(rs.count +radio->length-1),u8,MALLOC_ERR_RET(FALSE));
			for(i = 0; i < rs.count; i++)
			{
				rates[i] = rs.rate[i];
				ratef[i] = rs.f[i];
			}
			for(i = 0; i < (radio->length-1); i++)
			{
				radio->rate[i] = radio->rate[i]&0x7F;//&0x7F to remove the f mark
				radio->rate[i] = radio->rate[i]*5;	//multiply 5 to use in function rate_set(int count, RADIO_INFO *radio)
				rates[rs.count+i] = radio->rate[i];
				ratef[rs.count+i] = radio->f[i];
				printf_d("manage radio->rate[%d]:%d\n", i, radio->rate[i]);
			}	
			rs.count += radio->length-1;
			FREE_OBJ(radio->rate);
			FREE_OBJ(radio->f);
			FREE_OBJ(rs.rate);
			FREE_OBJ(rs.f);
			//rs.rate = (u16 *)malloc((rs.count)*sizeof(u16));
			//rs.f = malloc((rs.count)*sizeof(u8));
			CREATE_ARRAY_ERR(rs.rate,(rs.count),u16,MALLOC_ERR_RET(FALSE));
			CREATE_ARRAY_ERR(rs.f,(rs.count),u8,MALLOC_ERR_RET(FALSE));
			for(i = 0; i < rs.count; i++)
			{
				rs.rate[i] = rates[i];
				rs.f[i] = ratef[i];
			}
			radio->rate = rates;
			radio->f = ratef;
			rate_set(rs.count, radio);	//get radio_type and rate_mask
			
			if(gRadioInfoValue[radio->radio_id-1].radiotype & IEEE80211_PARAM_MODE_11g)		//g 1-54 (contain a and b)
			{
				sprintf(cmdbuf,"iwpriv wifi%d set_rate_mask_g 0x%x",radio->radio_id-1,radio->rate_mask);
			}
			else if(gRadioInfoValue[radio->radio_id-1].radiotype & IEEE80211_PARAM_MODE_11b)	//b 6-54
			{
				sprintf(cmdbuf,"iwpriv wifi%d set_rate_mask_b 0x%x",radio->radio_id-1,radio->rate_mask);
			}
			else if(gRadioInfoValue[radio->radio_id-1].radiotype & IEEE80211_PARAM_MODE_11a)	//a 1-11
			{
				sprintf(cmdbuf,"iwpriv wifi%d set_rate_mask_a 0x%x",radio->radio_id-1,radio->rate_mask);
			}
			else
			{
				FREE_OBJ(radio->rate);
				FREE_OBJ(radio->f);
				break;
			}
			gRadioInfoValue[radio->radio_id-1].rate_mask = radio->rate_mask;
			save_radio_level_config(radio->radio_id-1,"rate_mask",INT,(int*)&gRadioInfoValue[radio->radio_id-1].rate_mask,1);
			SYSTEM(cmdbuf);
			memset(cmdbuf, 0, 128);
			sprintf(cmdbuf,"iwpriv wifi%d set_rate_cap 0x%x", radio->radio_id-1, 0x80);
			SYSTEM(cmdbuf);

			FREE_OBJ(radio->rate);
			FREE_OBJ(radio->f);
			break;
		default:
			printf_d("unrecognized type: %d\n", radio->type);
			break;
	}
	pthread_mutex_unlock(&wlan_mutex);	
	if((bol_radio_up == FALSE) || ((bol_radio_txpower == FALSE) && (radio->type == 2) && ((radio->radio_type == 1) || (radio->radio_type == 2))))
	{
	}
	else
		set_radio_state(ENABLED, radio->radio_id);
	return TRUE;
}
BOOL manage_set_radio_requset_msg(SET_RADIO_PARAMETER *radio_para)
{
	int radio_num = 1;
	int i = 0;
	RADIO_INFO *radio = NULL;
	
	if(radio_para->radio_info->radio_id == 0)	//all radio
	{
		radio_para->radio_info->radio_id = 1;
		radio_num = RADIO_COUNT;
	}
	else
		radio_num = 1;
	
	for(radio = radio_para->radio_info; radio != NULL; radio = radio->next)
	{
		for(i = 0; i < radio_num; i++)
		{
			radio->radio_id = radio->radio_id+i;
			set_radio_info(radio);
		}
	}
	
	radio_para->result = SUCCESSED;
	send_message(assemble_set_radio_response, radio_para);
	
	radio = radio_para->radio_info;
	while(radio != NULL)
	{
		radio = radio->next;
		FREE_OBJ(radio_para->radio_info);
		radio_para->radio_info = radio;
	}
	FREE_OBJ(radio_para);
	return TRUE;
}
BOOL parse_set_sta_state_request_msg(TMSG *msg)
{
	u16      elemID = 0;
	u16      elemlen = 0;
	SET_STA_STATE *sta = NULL;
	char *p = NULL;
	sta = (SET_STA_STATE *)malloc(sizeof(SET_STA_STATE));
	if(sta == NULL)
		return RAISE_ERROR(ERROR_OUT_OF_MEMORY, NULL);
	memset(sta, 0, sizeof(SET_STA_STATE));
	sta->PMK = NULL;
	sta->ver             = protocol_retrieve16(msg);
	sta->seq             = protocol_retrieve8(msg);
	msg->offset         += 1;
	while(msg->offset < msg->len)
	{
		elemID               = protocol_retrieve16(msg);
		elemlen              = protocol_retrieve16(msg);
		switch(elemID)
		{
			case 1:                            /* add sta */
				sta->type    = elemID;
				sta->rid     = protocol_retrieve8(msg);
				sta->wid     = protocol_retrieve8(msg);
				sta->state   = protocol_retrieve16(msg);
				p            = protocol_retrieve_raw_bytes(msg, MAC_ADDR_LEN);
				memcpy(sta->mac, p, MAC_ADDR_LEN);
				FREE_OBJ(p);
				break;
			case 2:                            /* del sta */
				sta->type    = elemID;
				sta->rid     = protocol_retrieve8(msg);
				sta->wid     = protocol_retrieve8(msg);
				sta->code    = protocol_retrieve16(msg);
				p            = protocol_retrieve_raw_bytes(msg, MAC_ADDR_LEN);
				memcpy(sta->mac, p, MAC_ADDR_LEN);
				FREE_OBJ(p);
				break;
			case 5:							   /* STATID renew */
				sta->type    = elemID;
				sta->act     = (protocol_retrieve8(msg) >> 4) & 0x0F;
				msg->offset += 1;
				p            = protocol_retrieve_raw_bytes(msg, MAC_ADDR_LEN);
				print_packet((u8 *)p, MAC_ADDR_LEN);
				memcpy(sta->mac, p, MAC_ADDR_LEN);
				print_packet(sta->mac, MAC_ADDR_LEN);
				FREE_OBJ(p);
				sta->sid	 = protocol_retrieve32(msg);
				break;
			case 6:                            /* set PMK */
				sta->type    = elemID;
				sta->PMKtype = protocol_retrieve16(msg);
				p            = protocol_retrieve_raw_bytes(msg, MAC_ADDR_LEN);
				memcpy(sta->mac, p, MAC_ADDR_LEN);
				FREE_OBJ(p);
				sta->PMKlen  = protocol_retrieve16(msg);
				sta->PMK     = protocol_retrieve_str(msg, sta->PMKlen);
				break;
			default:
				msg->offset += elemlen;
				break;
		}
	}
	
	return add_cmd_to_list_tail(MSG_ELEMENT_SET_STA_STATE_REQUEST_TYPE, sta);
}
BOOL manage_set_sta_state_request_msg(SET_STA_STATE *sta)
{
	char cmd[128] = {0};
	u8 ret = FAILED;
	char ifname[32];
	STA_INFO *ptr = NULL, *h = NULL, *psta = NULL;
	WLAN_INFO *wptr = NULL;
	int s;
	BOOL is_exists_sta = FALSE;
	struct iwreq iwr;
	struct ieee80211_tunnel_base_users user;
	printf_d("sta_type = %d\n", sta->type);
	pthread_mutex_lock(&wlan_mutex);
	pthread_mutex_lock(&sta_mutex);
	switch(sta->type)
	{
		case 1:                             /* add sta */
			ptr = (STA_INFO *)malloc(sizeof(STA_INFO));
			if(ptr == NULL)
			{
				goto ext;
			}
			memset(ptr,0,sizeof(STA_INFO));
			ptr->next = NULL;
			for(wptr = wlan_list; wptr != NULL; wptr = wptr->next)
			{
				if((wptr->rid == sta->rid) && (wptr->wid == sta->wid))
				{
					CREATE_ARRAY_ERR(ptr->BSSID,MAC_ADDR_LEN,u8,MALLOC_ERR_GOTO(ext));
					memcpy(ptr->BSSID , wptr->BSSID, MAC_ADDR_LEN);
					ptr->ssid_length = wptr->SSID_len;
					CREATE_STRING_ERR(ptr->SSID,ptr->ssid_length,MALLOC_ERR_GOTO(ext));
					strcpy(ptr->SSID , wptr->SSID);
					break;
				}
			}
			ptr->rid = sta->rid;
			ptr->wid = sta->wid;
			CREATE_ARRAY_ERR(ptr->MAC,MAC_ADDR_LEN,u8,MALLOC_ERR_GOTO(ext));
			memcpy(ptr->MAC, sta->mac, MAC_ADDR_LEN);
			ptr->PMK = NULL;
			ptr->ip = 0;
			if(sta_list == NULL)
			{
				sta_list = ptr;
				Sta_ReportEnable = ENABLED;
				save_apctl_level_config("Sta_ReportEnable",CHAR,(u8 *)&Sta_ReportEnable,1);
			}
			else
			{
				psta = sta_list;
				while(psta != NULL)
				{
					if(memcmp(ptr->MAC, psta->MAC, MAC_ADDR_LEN) == 0)
					{
						memcpy(psta->BSSID, ptr->BSSID, MAC_ADDR_LEN);
						FREE_OBJ(psta->SSID);
						psta->SSID = ptr->SSID;
						psta->rid = ptr->rid;
						psta->wid = sta->wid;
						is_exists_sta = TRUE;
						FREE_OBJ(ptr->BSSID);
						FREE_OBJ(ptr->MAC);
						FREE_OBJ(ptr);
						break;
					}
					psta = psta->next;
				}
				if(is_exists_sta == FALSE)
				{
					ptr->next = sta_list;
					sta_list = ptr;
				}
			}
			break;
		case 2:                             /* del sta */
			sprintf(cmd, "iwpriv ath.%d-%d kickmac %02X:%02X:%02X:%02X:%02X:%02X", 
					sta->rid-1, sta->wid, 
					sta->mac[0], sta->mac[1], sta->mac[2], sta->mac[3], sta->mac[4], sta->mac[5]);
			SYSTEM(cmd);
			h = sta_list;
			ptr = sta_list;
			while(ptr != NULL)
			{
				
				if(ptr == sta_list)
				{
					if((ptr->rid == sta->rid) && (ptr->wid == sta->wid) && (memcmp(ptr->MAC, sta->mac, MAC_ADDR_LEN) == 0))
					{
						sta_list = ptr->next;
						h = sta_list;
						FREE_OBJ(ptr->MAC);
						FREE_OBJ(ptr->BSSID);
						FREE_OBJ(ptr->SSID);
						FREE_OBJ(ptr->PMK);
						FREE_OBJ(ptr);
						ptr = sta_list;
					}
					else
					{
						ptr = ptr->next;
					}
					
				}
				else
				{
					if((ptr->rid == sta->rid) && (ptr->wid == sta->wid) && (memcmp(ptr->MAC, sta->mac, MAC_ADDR_LEN) == 0))
					{
						h->next = ptr->next;
						FREE_OBJ(ptr->MAC);
						FREE_OBJ(ptr->BSSID);
						FREE_OBJ(ptr->SSID);
						FREE_OBJ(ptr->PMK);
						FREE_OBJ(ptr);
						ptr = sta_list;
					}
					else
					{
						h = ptr;
						ptr = ptr->next;
					}
				}
				
			}
			
			ret = SUCCESSED;
			if(sta_list == NULL){
				Sta_ReportEnable = DISABLED;
				save_apctl_level_config("Sta_ReportEnable",CHAR,(u8 *)&Sta_ReportEnable,1);
			}
			break;
		case 5:
			/* !!! */	
			s = socket(AF_INET, SOCK_DGRAM, 0);
			if (s < 0)
			{
				APVLog("create sock to get_stations error!\n");
				printf_d("create sock to get_stations error!\n");
				goto ext;
			}
			for(ptr = sta_list; ptr != NULL; ptr = ptr->next)
			{
				if(memcmp(sta->mac, ptr->MAC, MAC_ADDR_LEN) == 0)
				{
					sta->rid = ptr->rid;
					sta->wid = ptr->wid;
					break;
				}
			}
			for(wptr = wlan_list; wptr != NULL; wptr = wptr->next)
			{
				if((sta->rid == wptr->rid) && (sta->wid == wptr->wid))
				{
					memset(&iwr, 0, sizeof(iwr));
					sprintf(ifname,"ath.%d-%d", wptr->rid-1, wptr->wid);
					strncpy(iwr.ifr_name, ifname, 32);
					iwr.u.data.pointer = (void *) &user;
					iwr.u.data.length = sizeof(user);
					memcpy(user.ni_macaddr, ptr->MAC, IEEE80211_ADDR_LEN);
					if(wptr->data_packet_method == TUNNEL_802_3)
						user.ni_tunnel_flag = 1;
					else if(wptr->data_packet_method == LOCAL_BRIDGE)
						user.ni_tunnel_flag = 2;
					else
						user.ni_tunnel_flag = 0xFF;

					user.vlan_id = wptr->vid;
					user.sta_sid = sta->sid;
					if(ioctl(s, IEEE80211_IOCTL_TUNNEL_BASE_USERS, &iwr) < 0)
					{
						APVLog("ioctl to set sta tunnel failed!\n");
						printf_d("ioctl to set sta tunnel failed!\n");
						close(s);
						goto ext;
					}
					printf_d("call ioctl for set tunnel, ni_tunnel_flag = %d, user.vlan_id = %d, user.sta_sid = %d, %02X:%02X:%02X:%02X:%02X:%02X\n", 
							user.ni_tunnel_flag, user.vlan_id, user.sta_sid,
						    user.ni_macaddr[0], user.ni_macaddr[1], user.ni_macaddr[2], user.ni_macaddr[3], user.ni_macaddr[4], user.ni_macaddr[5]);
					APVLog("call ioctl for set tunnel, ni_tunnel_flag = %d, user.vlan_id = %d, user.sta_sid = %d, %02X:%02X:%02X:%02X:%02X:%02X\n", 
							user.ni_tunnel_flag, user.vlan_id, user.sta_sid,
						    user.ni_macaddr[0], user.ni_macaddr[1], user.ni_macaddr[2], user.ni_macaddr[3], user.ni_macaddr[4], user.ni_macaddr[5]);
					break;
				}
			}
			close(s);
			break;
		case 6:
			for(ptr = sta_list; ptr != NULL; ptr = ptr->next)
			{
				if(memcmp(sta->mac, ptr->MAC, MAC_ADDR_LEN) == 0)
				{
					ptr->PMK = sta->PMK;
					break;
				}
			}
			break;
		default:
			break;
	}
	sta->ret = ret;
	ret = send_message(assemble_set_sta_state_response, sta);
ext:
	pthread_mutex_unlock(&sta_mutex);
	pthread_mutex_unlock(&wlan_mutex);
	FREE_OBJ(sta);
	
	return ret;
}


/* parse set data tunnel message */
BOOL parse_set_data_tunnel_requset_msg(TMSG *msg)
{
	DATA_T *data = NULL;
	u8 I_rev_TunT;
	//data = (DATA_T *)malloc(sizeof(DATA_T));
	CREATE_OBJECT_ERR(data,DATA_T,MALLOC_ERR_RET(FALSE));
	if(data == NULL)
		return RAISE_ERROR(ERROR_OUT_OF_MEMORY, NULL);

	data->ver             = protocol_retrieve16(msg);
	data->seq             = protocol_retrieve8(msg);
	msg->offset         += 1;
	while(msg->offset < msg->len)
	{
		data->type = protocol_retrieve16(msg);
		data->length = protocol_retrieve16(msg);
		I_rev_TunT = protocol_retrieve8(msg);				//
		data->I = I_rev_TunT >> 7;
		data->TunT = I_rev_TunT&0x0F;
		data->tunnel_mode = protocol_retrieve8(msg);
		if(data->I == 0)	//IPv4
		{
			data->IPv4 = protocol_retrieve32(msg);
			data->port = protocol_retrieve16(msg);
		}
	}
	return add_cmd_to_list_tail(MSG_ELEMENT_SET_DATA_TUNNEL_REQUEST_TYPE, data);
}

BOOL manage_set_data_tunnel_requset_msg(DATA_T *data)
{
	char IPv4[16];
	struct in_addr s;
	char cmdbuf[128];
	
	s.s_addr = data->IPv4;	
	inet_ntop(AF_INET, (void *)&s, IPv4, 16);
	printf_d("AC IP Address: %s\n", IPv4);
	
	/* !!! create data tunnel */
	if(data->I == 0)	//IPv4
	{
		if(s.s_addr == 0)			//data tunnel invalid
		{
			printf_d("Invalid IP 0.0.0.0 in set data tunnel\n");
			APVLog("Invalid IP 0.0.0.0 in set data tunnel\n");
			data->result = FAILED;
			local_tunnel = FALSE;
			save_apctl_level_config("local_tunnel",INT,(u32 *)&local_tunnel,1);
			set_tunnel_for_all_wlan(local_tunnel);
			goto send_msg;
		}
		else
		{
			memset(cmdbuf, 0, 128);
			sprintf(cmdbuf, "echo %d > /proc/sys/net/capwap/tunnel8023_udp_port", data->port);//32769
			SYSTEM(cmdbuf);
			memset(cmdbuf, 0, 128);
			sprintf(cmdbuf, "echo %#x > /proc/sys/net/capwap/ac_addr", data->IPv4);
			SYSTEM(cmdbuf);
			local_tunnel = TRUE;
			save_apctl_level_config("local_tunnel",INT,(u32 *)&local_tunnel,1);
			set_tunnel_for_all_wlan(local_tunnel);
		}
	}
	if(data->TunT == 1)			//use UDP
	{
	}
	if(data->tunnel_mode == 1)	//MACBridge
	{
	}
	data->result = SUCCESSED;
send_msg:
	send_message(assemble_data_tunnel_response, data);
	FREE_OBJ(data);
	return TRUE;
}

/* parse set wlan info message */
BOOL parse_set_wlan_info_requset_msg(TMSG *msg)
{
	RENEW_WLAN_CONF *wlanconf = NULL;
	RENEW_WLAN_INFO *head = NULL;
	//wlanconf = (RENEW_WLAN_CONF *)malloc(sizeof(RENEW_WLAN_CONF));
	CREATE_OBJECT_ERR(wlanconf,RENEW_WLAN_CONF,MALLOC_ERR_RET(FALSE));
	//wlanconf->wlan = (WLAN_INFO *)malloc(sizeof(WLAN_INFO));
	if(wlanconf == NULL)
		return RAISE_ERROR(ERROR_OUT_OF_MEMORY, NULL);
	wlanconf->ver = protocol_retrieve16(msg);
	wlanconf->seq = protocol_retrieve8(msg);
	wlanconf->wlan_info = NULL;
	msg->offset += 1;
	while(msg->offset < msg->len)
	{	

		if(wlanconf->wlan_info == NULL)
		{
			//wlanconf->wlan_info = (RENEW_WLAN_INFO *)malloc(sizeof(RENEW_WLAN_INFO)); 	
			CREATE_OBJECT_ERR(wlanconf->wlan_info,RENEW_WLAN_INFO,MALLOC_ERR_RET(FALSE));
			wlanconf->wlan_info->next = NULL;
			head = wlanconf->wlan_info;
		}
		else
		{
			//wlanconf->wlan_info->next = (RENEW_WLAN_INFO *)malloc(sizeof(RENEW_WLAN_INFO));
			CREATE_OBJECT_ERR(wlanconf->wlan_info->next,RENEW_WLAN_INFO,MALLOC_ERR_RET(FALSE));
			wlanconf->wlan_info = wlanconf->wlan_info->next;
			wlanconf->wlan_info->next = NULL;
		}	
		if(wlanconf->wlan_info == NULL)
			return RAISE_ERROR(ERROR_OUT_OF_MEMORY, NULL);
		
		wlanconf->wlan_info->type = protocol_retrieve16(msg);
		wlanconf->wlan_info->length = protocol_retrieve16(msg);
		wlanconf->wlan_info->rid = protocol_retrieve8(msg);				//radio id
		wlanconf->wlan_info->wid = protocol_retrieve8(msg);				//wlan id
		if(wlanconf->wlan_info->type == 2)
		{
			wlanconf->wlan_info->tunnel = protocol_retrieve8(msg);		//tunnel
		}
		else if(wlanconf->wlan_info->type == 3)
		{
			wlanconf->wlan_info->SSID_flow_limit = protocol_retrieve32(msg);
			wlanconf->wlan_info->user_flow_limit = protocol_retrieve32(msg);
		}
		else if(wlanconf->wlan_info->type == 11)
		{
			wlanconf->wlan_info->state = protocol_retrieve8(msg);
		}
	}
	wlanconf->wlan_info = head;
	return add_cmd_to_list_tail(MSG_ELEMENT_SET_WLAN_INFO_REQUEST_TYPE, wlanconf);
}
BOOL set_tunnel(BOOL state, u8 rid, u8 wid)
{
	char cmdbuf[128] = {0};
	if(state == 0)
	{
		sprintf(cmdbuf, "echo 0 > /proc/sys/net/capwap/spfast_down");
		SYSTEM(cmdbuf);
		sprintf(cmdbuf, "echo 0 > /proc/sys/net/capwap/spfast_up");
		SYSTEM(cmdbuf);
		sprintf(cmdbuf, "echo 0 > /proc/sys/net/ath.%d-%d/vap_splitmac", rid, wid);
		SYSTEM(cmdbuf);
		sprintf(cmdbuf, "echo 1 > /proc/sys/net/ipv4/ip_forward");
		SYSTEM(cmdbuf);
	}
	else if(state == 1)
	{
		sprintf(cmdbuf, "echo 0 > /proc/sys/net/capwap/spfast_down");
		SYSTEM(cmdbuf);
		sprintf(cmdbuf, "echo 0 > /proc/sys/net/capwap/spfast_up");
		SYSTEM(cmdbuf);
		sprintf(cmdbuf, "echo 2 > /proc/sys/net/ath.%d-%d/vap_splitmac", rid, wid);
		SYSTEM(cmdbuf);
		sprintf(cmdbuf, "echo 1 > /proc/sys/net/capwap/tunnel8023_down");
		SYSTEM(cmdbuf);
		sprintf(cmdbuf, "echo 1 > /proc/sys/net/capwap/tunnel8023_up");
		SYSTEM(cmdbuf);
		sprintf(cmdbuf, "echo 1 > /proc/sys/net/ipv4/ip_forward");
		SYSTEM(cmdbuf);
	}
	return TRUE;
}
BOOL set_tunnel_for_all_wlan(BOOL state)
{
	WLAN_INFO *ptr = NULL;
	char cmdbuf[128] = {0};
	pthread_mutex_lock(&wlan_mutex);
	if(state == TRUE)
	{
		for(ptr = wlan_list; ptr != NULL; ptr = ptr->next)
		{
			if(ptr->data_packet_method == TUNNEL_802_3)
			{
				sprintf(cmdbuf, "echo 2 > /proc/sys/net/ath.%d-%d/vap_splitmac", ptr->rid-1, ptr->wid);
				SYSTEM(cmdbuf);
			}
			else if(ptr->data_packet_method == TUNNEL_802_11)
			{
				sprintf(cmdbuf, "echo 1 > /proc/sys/net/ath.%d-%d/vap_splitmac", ptr->rid-1, ptr->wid);
				SYSTEM(cmdbuf);
			}
		}
	}
	else
	{
		for(ptr = wlan_list; ptr != NULL; ptr = ptr->next)
		{
			
			sprintf(cmdbuf, "echo 0 > /proc/sys/net/ath.%d-%d/vap_splitmac", ptr->rid-1, ptr->wid);
			SYSTEM(cmdbuf);
		}
	}
	pthread_mutex_unlock(&wlan_mutex);
	return TRUE;
}

BOOL update_hostapd_and_restart(WLAN_INFO *wlan)
{
	char cmd[128] = {0}; 
	char filename[128] = {0};
	char wlanname[64] = {0};
	char briname[16]={"default"};
	sprintf(wlanname,"ath.%d-%d",wlan->rid -1,wlan->wid);	

	kill_hostapd_by_wlanname(wlanname);
	/* */
	if(wlan->sec_policy == EAP_WPA_WPA2)               /* WPA/WPA2 EAP */
	{
		//snprintf(filename,sizeof(filename), "eap_%s", wlanname);
		//kill_hostapd(filename);
		//memset(filename, 0, 128);
		snprintf(filename,sizeof(filename), "%s/eap_%s", HOSTAPD_PROFILE_PATH, wlanname);
	}
	else if(wlan->sec_policy == EAP_WPA)
	{
		//snprintf(filename,sizeof(filename), "eap_wpa_%s", wlanname);
		//kill_hostapd(filename);
		//memset(filename, 0, 128);
		snprintf(filename,sizeof(filename), "%s/eap_wpa_%s", HOSTAPD_PROFILE_PATH, wlanname);
	}
	else if(wlan->sec_policy == EAP_WPA2)
	{
		//snprintf(filename,sizeof(filename), "eap_wpa2_%s", wlanname);
		//kill_hostapd(filename);
		//memset(filename, 0, 128);
		snprintf(filename,sizeof(filename), "%s/eap_wpa2_%s", HOSTAPD_PROFILE_PATH, wlanname);
	}
	else if(wlan->sec_policy == PSK_WPA_WPA2)          /* WPA/WPA2 PSK */
	{
		//snprintf(filename,sizeof(filename), "psk_%s", wlanname);
		//kill_hostapd(filename);
		//memset(filename, 0, 128);
		snprintf(filename,sizeof(filename), "%s/psk_%s", HOSTAPD_PROFILE_PATH, wlanname);
	}
	else if(wlan->sec_policy == PSK_WPA)
	{
		//snprintf(filename,sizeof(filename), "psk_wpa_%s", wlanname);
		//kill_hostapd(filename);
		//memset(filename, 0, 128);
		snprintf(filename,sizeof(filename), "%s/psk_wpa_%s", HOSTAPD_PROFILE_PATH, wlanname);
	}
	else if(wlan->sec_policy == PSK_WPA2)
	{
		//snprintf(filename,sizeof(filename), "psk_wpa2_%s", wlanname);
		//kill_hostapd(filename);
		//memset(filename, 0, 128);
		snprintf(filename,sizeof(filename), "%s/psk_wpa2_%s", HOSTAPD_PROFILE_PATH, wlanname);
	}
	else
	{
		/* other sec_policy don't need hostpad */
		return TRUE;
	}
	if(wlan->data_packet_method==LOCAL_BRIDGE && wlan->vid != 0){
		memset(briname,0,sizeof(briname));
		sprintf(briname,"br%d",wlan->vid);
	}
	
	sprintf(cmd,"sed -i 's/bridge=.*/bridge=%s/g' %s",wlan->data_packet_method==TUNNEL_802_3?"default":briname,filename);
	SYSTEM(cmd);
	memset(cmd, 0, 128);

	snprintf(cmd, sizeof(cmd), "hostapd %s &", filename);
	SYSTEM(cmd);
	
	return TRUE;
}
	
	

BOOL manage_set_wlan_info_request_msg(RENEW_WLAN_CONF *wlanconfig)
{

	char cmdbuf[128] = {0};
	WLAN_INFO *ptr = NULL;
	char ifname[IFNAMSIZ] = "wifi1";
	char wlanname[IFNAMSIZ]={0};
	int i = 0,rid_count=1;
	RENEW_WLAN_INFO *tmp = NULL;
	RENEW_WLAN_INFO *src = wlanconfig->wlan_info;
	RENEW_WLAN_CONF*wlanconf[2]={NULL,NULL};
	RENEW_WLAN_INFO *second_wlanconf = NULL,*temp_dest=NULL;
	if(check_interface_state(ifname)==0)
	{
		temp_dest = second_wlanconf = (RENEW_WLAN_INFO *)malloc(sizeof(RENEW_WLAN_INFO));
		memset(temp_dest,0,sizeof(RENEW_WLAN_INFO));
		if(temp_dest==NULL)
			return RAISE_ERROR(ERROR_OUT_OF_MEMORY, NULL);
		
		while(src!=NULL)
		{
			memcpy(temp_dest,src,sizeof(RENEW_WLAN_INFO));
			temp_dest->rid=2;	/* second radio */
			printf_d("rid	:%d\twid:	%d\ttunnel:	%d\t type:	%d\n",temp_dest->rid,temp_dest->wid,temp_dest->tunnel,temp_dest->type);
			if(src->next!=NULL)
			{
				temp_dest->next=(RENEW_WLAN_INFO*)malloc(sizeof(RENEW_WLAN_INFO));
				if(temp_dest->next==NULL)
					return RAISE_ERROR(ERROR_OUT_OF_MEMORY, NULL);
				memset(temp_dest->next,0,sizeof(RENEW_WLAN_INFO));
				temp_dest = temp_dest->next;
			}
			else
			{
				temp_dest->next=NULL;
				break;	/* last element */
			}
			src = src->next;	
		}		
		wlanconf[1]  = (RENEW_WLAN_CONF*)malloc(sizeof(RENEW_WLAN_CONF));
		if(wlanconf[1]==NULL)
			return RAISE_ERROR(ERROR_OUT_OF_MEMORY, NULL);
		
		memset(wlanconf[1],0,sizeof(RENEW_WLAN_CONF));
		memcpy(wlanconf[1],wlanconfig,sizeof(wlanconfig));
		wlanconf[1]->wlan_info = second_wlanconf;
		
		rid_count++;
	}

	wlanconf[0] = wlanconfig;
	pthread_mutex_lock(&wlan_mutex);
	for(i = 0;i< rid_count;i++)
	{
		sprintf(wlanname,"ath.%d-%d", wlanconf[i]->wlan_info->rid-1, wlanconf[i]->wlan_info->wid);
		if(wlanconf[i]->wlan_info->type == 2)
		{
			if(wlanconf[i]->wlan_info->tunnel == 0)
			{
				set_tunnel(wlanconf[i]->wlan_info->tunnel, wlanconf[i]->wlan_info->rid-1, wlanconf[i]->wlan_info->wid);
				for(ptr = wlan_list; ptr != NULL; ptr = ptr->next)
				{
					if((ptr->rid == wlanconf[i]->wlan_info->rid) && (ptr->wid == wlanconf[i]->wlan_info->wid))
					{
						ptr->data_packet_method = LOCAL_BRIDGE;
						/* add vlan tag */
						if(ptr->vid > 0 )
							add_vlan_to_bridge(ptr);
						else if(ptr->vid == 0 ){
							char cmd[64] ={0};
							sprintf(cmd, "brctl addif default ath.%d-%d",  ptr->rid-1, ptr->wid);
							SYSTEM(cmd);
						}
						update_hostapd_and_restart(ptr);
						/* save wlan for renew data_packet_method */
						save_wlan_config_for_hot_start(ptr);
						break;
					}
				}
			}
			else
			{
				set_tunnel(wlanconf[i]->wlan_info->tunnel, wlanconf[i]->wlan_info->rid-1, wlanconf[i]->wlan_info->wid);
				for(ptr = wlan_list; ptr != NULL; ptr = ptr->next)
				{
					if((ptr->rid == wlanconf[i]->wlan_info->rid) && (ptr->wid == wlanconf[i]->wlan_info->wid))
					{
						ptr->data_packet_method = TUNNEL_802_3;
						/* delete vlan tag */
						delete_vlan_from_bridge(ptr->rid,ptr->wid);
						sprintf(cmdbuf, "brctl addif default ath.%d-%d",  ptr->rid-1, ptr->wid);
						SYSTEM(cmdbuf);
						update_hostapd_and_restart(ptr);
						/* save wlan for renew data_packet_method */
						save_wlan_config_for_hot_start(ptr);
						break;
					}
				}
			}
		}
		else if(wlanconf[i]->wlan_info->type == 3)
		{
			//wlan uplink limite set
			if(wlanconf[i]->wlan_info->SSID_flow_limit == 0)
			{
				traffic_vap_up_switch = FALSE;
				
				if(traffic_vap_down_switch==FALSE)
				{
					sprintf(cmdbuf,"autelan traffic_limit %s set_vap_flag 0", wlanname);
					SYSTEM(cmdbuf);
				}
				save_apctl_level_config("traffic_vap_up_switch", INT, (int*)&traffic_vap_up_switch,1);
			}
			else
			{
				traffic_vap_up_switch = TRUE;
				sprintf(cmdbuf,"autelan traffic_limit %s set_vap_flag 1", wlanname);
				SYSTEM(cmdbuf);
				sprintf(cmdbuf,"autelan traffic_limit %s set_vap %d", wlanname, wlanconf[i]->wlan_info->SSID_flow_limit);
				SYSTEM(cmdbuf);
				save_apctl_level_config("traffic_vap_up_switch", INT, (int*)&traffic_vap_up_switch,1);
			}
			if(wlanconf[i]->wlan_info->user_flow_limit == 0)
			{
				traffic_node_up_switch = FALSE;
				
				if(traffic_node_down_switch==FALSE)
				{
					sprintf(cmdbuf,"autelan traffic_limit %s set_every_node_flag 0", wlanname);
					SYSTEM(cmdbuf);
				}
				save_apctl_level_config("traffic_node_up_switch", INT, (int*)&traffic_node_up_switch,1);
			}
			else
			{
				traffic_node_up_switch = TRUE;
				sprintf(cmdbuf,"autelan traffic_limit %s set_every_node_flag 1", wlanname);
				SYSTEM(cmdbuf);
				sprintf(cmdbuf,"autelan traffic_limit %s set_every_node %d", wlanname, wlanconf[i]->wlan_info->user_flow_limit);
				SYSTEM(cmdbuf);
				save_apctl_level_config("traffic_node_up_switch", INT, (int*)&traffic_node_up_switch,1);
			}
		}
		else if(wlanconf[i]->wlan_info->type == 11)
		{
			if(wlanconf[i]->wlan_info->state == 0)		//enable
			{			
				sprintf(cmdbuf,"ifconfig %s up", wlanname);
			}
			else if(wlanconf[i]->wlan_info->state == 1)	//disable
			{
				sprintf(cmdbuf,"ifconfig %s down", wlanname);
			}
			SYSTEM(cmdbuf);
			for(ptr = wlan_list; ptr != NULL; ptr = ptr->next)
			{
				if(ptr->rid == wlanconf[i]->wlan_info->rid)
				{
					ptr->wlan_updown_time++;
				}
			}
		}
	}
	pthread_mutex_unlock(&wlan_mutex);
	wlanconf[0]->ret = SUCCESSED;
	send_message(assemble_set_wlan_info_response, wlanconf[0]);

	for(i=0;i<rid_count;i++)
	{
		tmp = wlanconf[i]->wlan_info;
		while(tmp != NULL)
		{
			tmp = wlanconf[i]->wlan_info->next;
			FREE_OBJ(wlanconf[i]->wlan_info);
			wlanconf[i]->wlan_info = tmp;
		}
		FREE_OBJ(wlanconf[i]);
	}
	return TRUE;
}
BOOL parse_echo_response_msg(TMSG *msg)
{
	MSG2  *echomsg = NULL;
	//echomsg = (MSG2 *)malloc(sizeof(MSG2));
	CREATE_OBJECT_ERR(echomsg,MSG2,MALLOC_ERR_RET(FALSE));
	echomsg->ver = protocol_retrieve16(msg);
	echomsg->seq = protocol_retrieve8(msg);
	msg->offset += 1;
	return add_cmd_to_list_tail(MSG_ELEMENT_AP_ALIVE_RESPONSE_TYPE, echomsg);
	return TRUE;
}
BOOL manage_echo_response_msg(MSG2 *msg2)
{
	overtime_count = 0;
	FREE_OBJ(msg2);
	return TRUE;
}

/* parse noice discovery ac result message */
BOOL parse_notice_discovery_ac_result_requset_msg(TMSG *msg)
{
	AC_RES *result;// = (AC_RES *)malloc(sizeof(AC_RES));
	CREATE_OBJECT_ERR(result,AC_RES,MALLOC_ERR_RET(FALSE));
	if(result == NULL)
		return RAISE_ERROR(ERROR_OUT_OF_MEMORY, NULL);
	
	result->ver             = protocol_retrieve16(msg);
	result->seq             = protocol_retrieve8(msg);
	msg->offset         += 1;
	while(msg->offset < msg->len)
	{
		result->type = protocol_retrieve16(msg);
		result->length = protocol_retrieve16(msg);
		result->result_code = protocol_retrieve8(msg);
	}
	return add_cmd_to_list_tail(MSG_ELEMENT_NOTICE_DISCOVERY_AC_RESULT_REQUEST_TYPE, result);
}
BOOL manage_notice_discovery_ac_result_requset_msg(AC_RES *result)
{
	printf_d("result->type:%d\n", result->type);
	printf_d("result->length:%d\n", result->length);
	printf_d("result->result_code:%d\n", result->result_code);
	printf_d("result->error_code:%d\n", result->error_code);
	FREE_OBJ(result);
	return TRUE;
}
/* */
BOOL parse_set_radius_requset_msg(TMSG *msg)
{
	MSG2 *radius_msg;// = (MSG2 *)malloc(sizeof(MSG2));
	CREATE_OBJECT_ERR(radius_msg,MSG2,MALLOC_ERR_RET(FALSE));
	RADIUS_CONF *radius = NULL, *head = NULL, *p = NULL;
	u16 elemtype = 0;
	u16 elemlen = 0;
	
	if(radius_msg == NULL)
		return RAISE_ERROR(ERROR_OUT_OF_MEMORY, NULL);
	
	radius_msg->ver      = protocol_retrieve16(msg);
	radius_msg->seq      = protocol_retrieve8(msg);
	msg->offset         += 1;
	while(msg->offset < msg->len)
	{
		//radius = (RADIUS_CONF *)malloc(sizeof(RADIUS_CONF));
		CREATE_OBJECT_ERR(radius,RADIUS_CONF,MALLOC_ERR_RET(FALSE));
		if(radius == NULL)
			goto err;
		
		radius->next = NULL;
		elemtype = protocol_retrieve16(msg);
		elemlen = protocol_retrieve16(msg);
		radius->type   = protocol_retrieve8(msg);
		radius->pri    = protocol_retrieve8(msg);
		radius->ipaddr = protocol_retrieve32(msg);
		radius->port   = protocol_retrieve16(msg);
		radius->pwd_len = elemlen - 8;
		radius->pwd     = protocol_retrieve_str(msg, radius->pwd_len);
		if(radius->pwd == NULL)
		{
			radius->pwd_len = 9;
			CREATE_OBJECT_SIZE_ERR(radius->pwd, radius->pwd_len, {raise_err(ERROR_OUT_OF_MEMORY, NULL, NULL, 0);});
			memcpy(radius->pwd,"12345678",radius->pwd_len);
			//radius->pwd = "12345678";///////////CK////////////
		}
		else
		{
			if((radius->pwd[0] == '\0') || (radius->pwd[0] == '\n'))
			{
				FREE_OBJ(radius->pwd);
				radius->pwd_len = 9;
				CREATE_OBJECT_SIZE_ERR(radius->pwd, radius->pwd_len, {raise_err(ERROR_OUT_OF_MEMORY, NULL, NULL, 0);});
				memcpy(radius->pwd,"12345678",radius->pwd_len);
				//radius->pwd = "12345678";
			}
		}
		if(head == NULL)
			p = head = radius;
		else
		{
			p->next = radius;
			p = p->next;
		}
	}
	radius_msg->element = (void *)head;
	return add_cmd_to_list_tail(MSG_ELEMENT_SET_RADIUS_REQUEST_TYPE, radius_msg);
err:
	while(head != NULL)
	{
		radius = head;
		head = head->next;
		FREE_OBJ(radius);		
	}
	return FALSE;
}
BOOL manage_set_radius_resquest_msg(MSG2 *radius_msg)
{
	MSG2 *radiusmsg = (MSG2 *)radius_msg;
	RADIUS_CONF *radius = NULL, *p = NULL;
	int ret = 0;
	if(radiusmsg == NULL)
		return RAISE_ERROR(ERROR_WRONG_ARG, NULL);
	
	radius = (RADIUS_CONF *)radiusmsg->element;
	while(radius_list != NULL)
	{
		p = radius_list->next;
		FREE_OBJ(radius_list->pwd);
		FREE_OBJ(radius_list);
		radius_list = p;
	}
	radius_list = radius;
	
	if(debugf == TRUE)
	{
		p = radius_list;
		while(p != NULL)
		{
			printf("radius->type = %d\n", p->type);
			printf("radius->pri = %d\n", p->pri);
			printf("radius->ipaddr = 0x%08X\n", p->ipaddr);
			printf("radius->port = %d\n", p->port);
			printf("radius->pwd_len = %d\n", p->pwd_len);
			printf("radius->pwd = %s\n", p->pwd);
			p = p->next;
		}
	}
	radiusmsg->ret = set_security_policy();
	radiusmsg->element = (void *)radius_list;
	ret = send_message(assemble_set_radius_response, radiusmsg);
	FREE_OBJ(radiusmsg);
	return ret;
}

/* parse noice join ac result message */
BOOL parse_notice_join_ac_result_requset_msg1(TMSG *msg)
{
	AC_RES *result;// = (AC_RES *)malloc(sizeof(AC_RES));
	CREATE_OBJECT_ERR(result,AC_RES,MALLOC_ERR_RET(FALSE));
	if(result == NULL)
		return RAISE_ERROR(ERROR_OUT_OF_MEMORY, NULL);

	while(msg->offset < msg->len)
	{
		result->result_code = protocol_retrieve16(msg);
		result->error_code = protocol_retrieve16(msg);
		result->IPv4 = protocol_retrieve32(msg);
	}
	struct in_addr s;
	s.s_addr = result->IPv4;
	char IPv4[16];
	inet_ntop(AF_INET, (void *)&s, IPv4, 16);
	printf_d("AC IP Address:  %s\n", IPv4);
	return add_cmd_to_list_tail(MSG_ELEMENT_NOTICE_JOIN_AC_RESULT_REQUEST_TYPE1, result);

}
BOOL manage_notice_join_ac_result_requset_msg1(AC_RES *result)
{
	printf_d("join result->result_code:%d\n", result->result_code);
	printf_d("join result->error_code:%d\n", result->error_code);
	printf_d("join result->IPv4:%d\n", result->IPv4);
	FREE_OBJ(result);
	return TRUE;
}

BOOL parse_notice_join_ac_result_requset_msg(TMSG *msg)
{
	AC_RES *result;//= (AC_RES *)malloc(sizeof(AC_RES));
	CREATE_OBJECT_ERR(result,AC_RES,MALLOC_ERR_RET(FALSE));
	if(result == NULL)
		return RAISE_ERROR(ERROR_OUT_OF_MEMORY, NULL);

	result->ver             = protocol_retrieve16(msg);
	result->seq             = protocol_retrieve8(msg);
	msg->offset         += 1;
	while(msg->offset < msg->len)
	{
		result->type = protocol_retrieve16(msg);
		result->length = protocol_retrieve16(msg);
		result->result_code = protocol_retrieve16(msg);
		result->error_code = protocol_retrieve16(msg);
		result->IPv4 = protocol_retrieve32(msg);
	}
	struct in_addr s;
	s.s_addr = result->IPv4;
	char IPv4[16];
	inet_ntop(AF_INET, (void *)&s, IPv4, 16);
	printf_d("AC IP Address:  %s\n", IPv4);
	return add_cmd_to_list_tail(MSG_ELEMENT_NOTICE_JOIN_AC_RESULT_REQUEST_TYPE, result);

}
BOOL manage_notice_join_ac_result_requset_msg(AC_RES *result)
{
	if(result->result_code == SUCCESSED)
	{
		APVLog("%s\n", get_format_str(" INTO RUNNING STATE ", '#', FORMAT_STR_LEN));
		SYSTEM("touch /tmp/zt_state");
		/* Begin: Add by dongzw for when ap ip address changed, restart apctl & apcomm 2013-04-16 */
		char cmd[64] = {0};
		sprintf(cmd, "echo %s > /tmp/ap-addr", hostip);
		SYSTEM(cmd);
		/* End: Add by dongzw for when ap ip address changed, restart apctl & apcomm 2013-04-16 */
		/* Begin: Add by dongzw for ap/wireless/eth information report time 2012-12-21 */
		ap_report_switch = ENABLED;
		eth_report_switch = ENABLED;
		/* Begin: Add by dongzw for ap/wireless/eth information report time 2012-12-21 */
	}
	printf_d("join result->type:%d\n", result->type);
	printf_d("join result->length:%d\n", result->length);
	printf_d("join result->result_code:%d\n", result->result_code);
	printf_d("join result->error_code:%d\n", result->error_code);
	printf_d("join result->IPv4:%d\n", result->IPv4);
	FREE_OBJ(result);
	return TRUE;
}
/*------------------------------------------------------------------------------------------------*/
static void driver_madwifi_wireless_event_wireless(char *data, int len, STA_ON_OFF_LINE *sta_state)
{
	struct iw_event iwe_buf, *iwe = &iwe_buf;
	char *pos, *end, *custom;

	pos = data;
	end = data + len;

	while (pos + IW_EV_LCP_LEN <= end) 
	{
		/* Event data may be unaligned, so make a local, aligned copy
		 * before processing. */
		memcpy(&iwe_buf, pos, IW_EV_LCP_LEN);
		if (iwe->len <= IW_EV_LCP_LEN)
			return;

		custom = pos + IW_EV_POINT_LEN;
		
		memcpy(&iwe_buf, pos, sizeof(struct iw_event));
		custom += IW_EV_POINT_OFF;

		switch (iwe->cmd) 
		{
			case IWEVREGISTERED:
				sta_state->on_off_line = IWEVREGISTERED;
				memcpy(sta_state->mac, (unsigned char *)iwe->u.addr.sa_data, MAC_ADDR_LEN);
				printf_d("recv msg from drv:station[%02X:%02X:%02X:%02X:%02X:%02X] on line!\n", sta_state->mac[0], sta_state->mac[1], sta_state->mac[2], sta_state->mac[3], sta_state->mac[4], sta_state->mac[5]);
				break;
			case IWEVEXPIRED:
				sta_state->on_off_line = IWEVEXPIRED;
				memcpy(sta_state->mac, (unsigned char *)iwe->u.addr.sa_data, MAC_ADDR_LEN);
				printf_d("recv msg from drv:station[%02X:%02X:%02X:%02X:%02X:%02X] off line!\n", sta_state->mac[0], sta_state->mac[1], sta_state->mac[2], sta_state->mac[3], sta_state->mac[4], sta_state->mac[5]);
				break;
			default:
				sta_state->on_off_line = 0;
				memset(sta_state->mac, 0, MAC_ADDR_LEN);
				break;
		}

		pos += iwe->len;
	}
}

static void driver_madwifi_wireless_event_rtm_newlink(struct nlmsghdr *h, int len, STA_ON_OFF_LINE *sta_state)
{
	struct ifinfomsg *ifi;
	int attrlen, nlmsg_len, rta_len;
	struct rtattr * attr;
	if (len < (int) sizeof(*ifi))
		return;

	ifi = NLMSG_DATA(h);

	nlmsg_len = NLMSG_ALIGN(sizeof(struct ifinfomsg));

	attrlen = h->nlmsg_len - nlmsg_len;

	if (attrlen < 0)
		return;

	attr = (struct rtattr *) (((char *) ifi) + nlmsg_len);

	rta_len = RTA_ALIGN(sizeof(struct rtattr));
	while (RTA_OK(attr, attrlen)) 
	{
		if (attr->rta_type == IFLA_WIRELESS) 
		{
			driver_madwifi_wireless_event_wireless(((char *) attr) + rta_len, attr->rta_len - rta_len, sta_state);
		}
		attr = RTA_NEXT(attr, attrlen);
	}
}

static void driver_madwifi_wireless_event_receive(int sock, STA_ON_OFF_LINE *sta_state)
{
	/*
	 * Modified by dongzw, change the buf from '256' to '8192' for 
	 * fix netlink receive data error when wlan security is wpa/wpa2, 2012-12-28
	 */
	char buf[8192] = {0};
	int left= 0;
	struct sockaddr_nl from;
	socklen_t fromlen;
	struct nlmsghdr *h;
	int ret;
	fd_set myset;

	fromlen = sizeof(from);

	FD_ZERO(&myset);
	FD_SET(sock,&myset);
	ret = select((sock+1), &myset, NULL, NULL, NULL);
	if(ret<0)
	{
		APVLog("select error in %s\n",__func__);
		return;
	}
	else
	{
		if(FD_ISSET(sock, &myset))
		{
			left = recvfrom(sock, buf, sizeof(buf), MSG_DONTWAIT, (struct sockaddr *) &from, &fromlen);
			if (left < 0) 
			{
				if (errno != EINTR && errno != EAGAIN)
					perror("recvfrom(netlink)");
				return;
			}
			h = (struct nlmsghdr *) buf;
			while (left >= (int) sizeof(struct nlmsghdr)) 
			{
				int len, plen;
				len = h->nlmsg_len;
				plen = len - sizeof(struct nlmsghdr);
				if (len > left || plen < 0) 
				{
					APVLog("Malformed netlink message: len=%d left=%d plen=%d\n", len, left, plen);
					break;
				}

				switch (h->nlmsg_type) 
				{
					case RTM_NEWLINK:
						driver_madwifi_wireless_event_rtm_newlink(h, plen, sta_state);
						break;
					default:
						break;
				}

				len = NLMSG_ALIGN(len);
				left -= len;
				h = (struct nlmsghdr *) ((char *) h + len);
			}

			if (left > 0) 
			{
				APVLog("%d extra bytes in the end of netlink message\n", left);
			}
		}
	}
	return;
}

BOOL del_sta(STA_INFO *stainfo)
{
	STA_INFO *sta = NULL, *hsta = NULL;
	if(stainfo == NULL)
		return RAISE_ERROR(ERROR_WRONG_ARG, NULL);
	sta = hsta = sta_list;
	while(sta != NULL)
	{
		if(sta == sta_list)
		{
			if(memcmp(sta->MAC, stainfo->MAC, MAC_ADDR_LEN) == 0)
			{
				sta_list = sta->next;
				hsta = sta_list;
				stainfo->rid = sta->rid;
				stainfo->wid = sta->wid;
				goto ext;
			}
			else
			{
				sta = sta->next;
			}
		}
		else
		{
			if(memcmp(sta->MAC, stainfo->MAC, MAC_ADDR_LEN) == 0)
			{
				hsta->next = sta->next;
				stainfo->rid = sta->rid;
				stainfo->wid = sta->wid;
				goto ext;
			}
			else
			{
				hsta = sta;
				sta = sta->next;
			}
		}
	}
	return FALSE;
ext:
	stainfo->BSSID = sta->BSSID;
	printf_d("sta("MAC_FMT") offline from ath.%d-%d bssid("MAC_FMT"), ssid(%s)\n",MAC2STR(sta->MAC),sta->rid-1,sta->wid,MAC2STR(sta->BSSID),sta->SSID);
	APVLog("sta("MAC_FMT") offline from ath.%d-%d bssid("MAC_FMT"), ssid(%s)\n",MAC2STR(sta->MAC),sta->rid-1,sta->wid,MAC2STR(sta->BSSID),sta->SSID);
	//printf_d("sta off line  in AP BSSID: %02X:%02X:%02X:%02X:%02X:%02X\n", 
	//		stainfo->BSSID[0], stainfo->BSSID[1], stainfo->BSSID[2], stainfo->BSSID[3], stainfo->BSSID[4], stainfo->BSSID[5]);
	stainfo->ssid_length = strlen((char *)sta->SSID)+1;
	//printf_d("ssid[%s]  ssid_length = %d\n", sta->SSID, stainfo->ssid_length);
	stainfo->SSID = sta->SSID;
	stainfo->value = 4;
	FREE_OBJ(sta->MAC);
	FREE_OBJ(sta);
	return TRUE;
}


void *check_sta_on_off_line(void *arg)
{
	int sock = 0;
	char wlanname[16] = {0};
	struct sockaddr_nl local;
	STA_ON_OFF_LINE sta_state;
	
	dprintff("in check_sta_on_off_line", '=', FORMAT_STR_LEN);
	if(pthread_detach(pthread_self())!=0)
	{
		dprintff("detach error!", '#', FORMAT_STR_LEN);
	}
	
	sock = socket(PF_NETLINK, SOCK_RAW, NETLINK_ROUTE);
	if (sock < 0) 
	{
		perror("socket(PF_NETLINK,SOCK_RAW,NETLINK_ROUTE)");
		pthread_exit(NULL);
	}

	memset(&local, 0, sizeof(local));
	local.nl_family = AF_NETLINK;
	local.nl_groups = RTMGRP_LINK;
	if (bind(sock, (struct sockaddr *) &local, sizeof(local)) < 0) 
	{
		perror("bind(netlink)");
		close(sock);
		pthread_exit(NULL);
	}
	
	while(1)
	{
		memset(sta_state.mac, 0, MAC_ADDR_LEN);
		sta_state.on_off_line = 0;
		/* dprintff("recevice packet", '-', FORMAT_STR_LEN); */
		driver_madwifi_wireless_event_receive(sock, &sta_state);
		if((sta_state.mac[0] == 0)&&(sta_state.mac[1] == 0)&&(sta_state.mac[2] == 0)&&(sta_state.mac[3] == 0)&&(sta_state.mac[4] == 0)&&(sta_state.mac[5] == 0))
		{
		}
		else
		{
			pthread_mutex_lock(&wlan_mutex);
			pthread_mutex_lock(&sta_mutex);
			if((sta_state.on_off_line == IWEVREGISTERED) || (sta_state.on_off_line == IWEVEXPIRED))
			{
				int i = 0;
				STA_INFO *values = NULL;
				//values = (STA_INFO *)malloc(sizeof(STA_INFO));//diaowq del@20120928,memory leak.
				CREATE_OBJECT_ERR(values, STA_INFO, {APVLog("create memory error in check_sta_on_off_line\n");close(sock);pthread_mutex_unlock(&sta_mutex);pthread_mutex_unlock(&wlan_mutex);pthread_exit(NULL);});

				STA_INFO *ptr = NULL;
				STA_INFO *oldptr = NULL;
				oldptr = ptr = NULL;
				values->BSSID = NULL;
				values->SSID = NULL;
				values->PMK = NULL;
				
				CREATE_OBJECT_SIZE_ERR(values->MAC, MAC_ADDR_LEN, {APVLog("create memory error in check_sta_on_off_line\n");close(sock);pthread_mutex_unlock(&sta_mutex);pthread_mutex_unlock(&wlan_mutex);pthread_exit((void *) 0);});
				for(i = 0;i<6;i++)
				{
					values->MAC[i] = sta_state.mac[i];
				}
				/* get station info */		
				if((sta_state.on_off_line == IWEVREGISTERED) && (local_tunnel == TRUE))
				{
					WLAN_INFO *ptr_wlan = NULL;
					for(ptr_wlan=wlan_list;ptr_wlan != NULL; ptr_wlan = ptr_wlan->next)
					{
						memset(wlanname, 0, 32);
						sprintf(wlanname, "ath.%d-%d", ptr_wlan->rid-1, ptr_wlan->wid);
						if(sta_compare(wlanname, values) == 0)
						{
							values->rid = ptr_wlan->rid;
							values->wid = ptr_wlan->wid;
							CREATE_OBJECT_SIZE_ERR(values->BSSID, MAC_ADDR_LEN, {APVLog("create memory error in check_sta_on_off_line\n");close(sock);pthread_mutex_unlock(&sta_mutex);pthread_mutex_unlock(&wlan_mutex);pthread_exit((void *) 0);});
							memcpy(values->BSSID, ptr_wlan->BSSID, MAC_ADDR_LEN);
							printf_d("sta("MAC_FMT") online to %s bssid("MAC_FMT"), ssid(%s)\n",MAC2STR(values->MAC),wlanname,MAC2STR(values->BSSID),ptr_wlan->SSID);
							APVLog("sta("MAC_FMT") online to %s bssid("MAC_FMT"), ssid(%s)\n",MAC2STR(values->MAC),wlanname,MAC2STR(values->BSSID),ptr_wlan->SSID);
							//values->encryption_type = ptr_wlan->sec_policy;
							values->encryption_type = 0;
							if(ptr_wlan->sec_policy == OPEN)
								values->encryption_type = 0;
							else if(ptr_wlan->sec_policy == WEP)
								values->encryption_type = 3;
							else if(ptr_wlan->sec_policy == EAP_WPA_WPA2)
								values->encryption_type = 4;
							else if(ptr_wlan->sec_policy == PSK_WPA_WPA2)
								values->encryption_type = 5;
							values->ssid_length = strlen((char *)ptr_wlan->SSID)+1;
							CREATE_OBJECT_SIZE_ERR(values->SSID, values->ssid_length, {APVLog("create memory error in check_sta_on_off_line\n");close(sock);pthread_mutex_unlock(&sta_mutex);pthread_mutex_unlock(&wlan_mutex);pthread_exit((void *) 0);});
							memcpy(values->SSID, ptr_wlan->SSID, values->ssid_length-1);
							values->value = 1;
							send_message(assemble_sta_association_request, values);
							/*if(ptr_wlan->sec_policy == EAP_WPA_WPA2)
							{
								values->value = 2;
								send_message(assemble_sta_association_request, values);
							}*/
							STA_ABILITY_INFO *sta_ability = NULL;
							//sta_ability = (STA_ABILITY_INFO *)malloc(sizeof(STA_ABILITY_INFO));
							CREATE_OBJECT_ERR(sta_ability,STA_ABILITY_INFO,{APVLog("create memory error in check_sta_on_off_line\n");close(sock);pthread_mutex_unlock(&sta_mutex);pthread_mutex_unlock(&wlan_mutex);pthread_exit((void *) 0);});
							sta_ability->mac = values->MAC;
							sta_ability->is_Qos = get_sta_wmm(wlanname, values->MAC);
							send_message(assemble_report_sta_ability_request, sta_ability);
							FREE_OBJ(sta_ability);/*do not free sta_ability->mac */
							break;//diaowq add, no need test other ath.x-y.@20120928
						}
					}
				}
				else if(sta_state.on_off_line == IWEVEXPIRED)
				{
					if(del_sta(values) == TRUE)
					{
						WLAN_INFO *ptr_wlan = NULL;
						values->encryption_type = 0;
						for(ptr_wlan=wlan_list;ptr_wlan != NULL; ptr_wlan = ptr_wlan->next)
						{
							if((ptr_wlan->rid == values->rid) && (ptr_wlan->wid == values->wid))
							{
								//values->encryption_type = ptr_wlan->sec_policy;
								if(ptr_wlan->sec_policy == OPEN)
									values->encryption_type = 0;
								else if(ptr_wlan->sec_policy == WEP)
									values->encryption_type = 3;
								else if(ptr_wlan->sec_policy == EAP_WPA_WPA2)
									values->encryption_type = 4;
								else if(ptr_wlan->sec_policy == PSK_WPA_WPA2)
									values->encryption_type = 5;
								break;
							}
						}
						values->value = 4;
						send_message(assemble_sta_association_request, values);
					}
					if(sta_list == NULL)
						Sta_ReportEnable = DISABLED;
				}
				FREE_OBJ(values->BSSID);
				FREE_OBJ(values->SSID);
				FREE_OBJ(values->MAC);
				
				FREE_OBJ(values);
			}
			pthread_mutex_unlock(&sta_mutex);
			pthread_mutex_unlock(&wlan_mutex);
		}
	}

	return NULL;
}
int set_dhcp_snooping(unsigned char state)//char *if_name
{
	return 0;//diaowq add@20120928:temp del for 8wei flow,no log reboot
	char cmdbuf[128];
	memset(cmdbuf, 0, 128);
#if 0
	char str[64];
	sprintf(cmdbuf, "ps|grep %s|awk -F \" \" '{print $1}'", if_name);

	FILE *fp =NULL;
	fp = popen(cmdbuf, "r");
	if(fp == NULL)
	{
		printf_d("cmd:%s error!%s\n", cmdbuf, strerror(errno));	   
		APVLog("cmd:%s error!%s\n", cmdbuf, strerror(errno)); 	   
		return 0;   
	}	   
	while(fgets(str, 64, fp))
	{
		if(str[strlen(str)-1] == '\n')	   
		{		   
		   str[strlen(str)-1] = '\0';	   
		}
		sprintf(cmdbuf, "kill -9 %s", str);
		SYSTEM(cmdbuf);
	}
	pclose(fp);
#else
	sprintf(cmdbuf, "killall -9 dhcpsnooping");
	SYSTEM(cmdbuf);
#endif

	if(state==ENABLED)
	{
#if 0
		sprintf(cmdbuf,"/usr/sbin/dhcpsnooping -t %s > /dev/null 2>&1 &", if_name);
#else
	#if (defined(AR11N))
		sprintf(cmdbuf,"/usr/sbin/dhcpsnooping -t eth1 > /dev/null 2>&1 &");
	#else
		sprintf(cmdbuf,"/usr/sbin/dhcpsnooping -t eth0 > /dev/null 2>&1 &");
	#endif
#endif
		SYSTEM(cmdbuf);
		APVLog("set dhcpsnooping start OK  !\n");
		printf_d("set dhcpsnooping start OK  !\n");
	}
	return 1;
}

unsigned int ip_int2str(unsigned int ipAddress, unsigned char *buff)
 {
	 unsigned int cnt;
	 unsigned char *tmpPtr = buff;
 
	 cnt = sprintf((char*)tmpPtr,"%d.%d.%d.%d",(ipAddress>>24) & 0xFF, \
			 (ipAddress>>16) & 0xFF,(ipAddress>>8) & 0xFF,ipAddress & 0xFF);
	 
	 return cnt;
 }


BOOL acw_parse_dhcpsnooping_report(TMSG *msgPtr, int len ,dhcpsnooping_report_type *valPtr)	//pei add 0722
{
	int i;
	char *tmp_subtype;
	char ip_str[24];
	void *ptr = NULL;
	
	memset(ip_str, 0, 24);
	int oldOffset;
	if(msgPtr == NULL || valPtr == NULL)
		return RAISE_ERROR(ERROR_WRONG_ARG, NULL);
	oldOffset = msgPtr->offset;

	ptr = protocol_retrieve_str(msgPtr,1);
	valPtr->type = atoi(ptr);
	FREE_OBJ(ptr);
	tmp_subtype = protocol_retrieve_str(msgPtr,2);
	/*10 means sta up line  11 means sta down line*/
	if(strcmp(tmp_subtype, "10")==0)   
		valPtr->subtype = 0;
	else if(strcmp(tmp_subtype, "11")==0)
		valPtr->subtype = 1;
	FREE_OBJ(tmp_subtype);
	
	for(i=0;i<6;i++){
		ptr = protocol_retrieve_str(msgPtr, 2);
		valPtr->mac[i] = strtol(ptr, NULL, 16);
		FREE_OBJ(ptr);
	}
	if (debugf){
		printf("dhcpsnooping report sta mac:");
		for(i=0;i<6;i++){
			printf("%02x ", valPtr->mac[i]);
		}
		printf("\n");
	}
	APVLog("dhcpsnooping report sta mac:[%02X-%02X-%02X-%02X-%02X-%02X]\n", valPtr->mac[0], valPtr->mac[1], 
		valPtr->mac[2], valPtr->mac[3], valPtr->mac[4], valPtr->mac[5]);

	ptr = protocol_retrieve_str(msgPtr, 1);
	valPtr->radio_id = atoi(ptr);
	FREE_OBJ(ptr);
	
	ptr = protocol_retrieve_str(msgPtr, 3); /*sun edited 2 to 3*/
	valPtr->wlan_id = atoi(ptr);
	FREE_OBJ(ptr);
	
	ptr = protocol_retrieve_str(msgPtr, 4);
	valPtr->vlanid = atoi(ptr);
	FREE_OBJ(ptr);
	
	valPtr->ip_length = sizeof(valPtr->ip);
//	sscanf(CWProtocolRetrieveStr(msgPtr,len-22),"%u",&(valPtr->ip));
	ptr = protocol_retrieve_str(msgPtr,len-23); /*sun edit 22 to 23*/
	sscanf(ptr, "%u", &(valPtr->ip));
	FREE_OBJ(ptr);

	ip_int2str(valPtr->ip, (unsigned char *)ip_str);
	APVLog("int_IP: %u   IP: %s\n", valPtr->ip, ip_str);
	APVLog("dhcpsnooping_report ( type = %d subtype = %d radio_id=%d wlan_id= %d vlanid= %d )\n",valPtr->type,valPtr->subtype,valPtr->radio_id,valPtr->wlan_id,valPtr->vlanid);
	if(debugf){
		printf("int_IP: %u\n IP: %s\n", valPtr->ip, ip_str);
		printf("dhcpsnooping_report ( type = %d subtype = %d radio_id=%d wlan_id= %d vlanid= %d  ) \n",valPtr->type,valPtr->subtype,valPtr->radio_id,valPtr->wlan_id,valPtr->vlanid);
	}
	return ((msgPtr->offset) - oldOffset) == len ? TRUE:RAISE_ERROR(ERROR_INVALID_FORMAT, "Message Element Malformed");
}

BOOL acw_parse_8021X_sta_online(unsigned char *sta_mac)
{

	char wlanname[32] = {0};
	pthread_mutex_lock(&wlan_mutex);
	pthread_mutex_lock(&sta_mutex);

	int i = 0;
	STA_INFO *values = NULL;
	//values = (STA_INFO *)malloc(sizeof(STA_INFO));
	CREATE_OBJECT_ERR(values, STA_INFO, {APVLog("create memory error in acw_parse_8021X_sta_online\n");pthread_mutex_unlock(&sta_mutex);pthread_mutex_unlock(&wlan_mutex);pthread_exit(NULL);});

	STA_INFO *ptr = NULL;
	STA_INFO *oldptr = NULL;
	oldptr = ptr = NULL;
	values->BSSID = NULL;
	values->SSID = NULL;
	values->PMK = NULL;
	
	CREATE_OBJECT_SIZE_ERR(values->MAC, MAC_ADDR_LEN, {APVLog("create memory error in acw_parse_8021X_sta_online\n");pthread_mutex_unlock(&sta_mutex);pthread_mutex_unlock(&wlan_mutex);pthread_exit((void *) 0);});
	for(i = 0;i<6;i++)
	{
		values->MAC[i] = sta_mac[i];
	}
	
	/* get station info */		
	WLAN_INFO *ptr_wlan = NULL;
	for(ptr_wlan=wlan_list;ptr_wlan != NULL; ptr_wlan = ptr_wlan->next)
	{
		memset(wlanname, 0, 32);
		sprintf(wlanname, "ath.%d-%d", ptr_wlan->rid-1, ptr_wlan->wid);
		if(sta_compare(wlanname, values) == 0)
		{
			values->rid = ptr_wlan->rid;
			values->wid = ptr_wlan->wid;
			CREATE_OBJECT_SIZE_ERR(values->BSSID, MAC_ADDR_LEN, {APVLog("create memory error in acw_parse_8021X_sta_online\n");pthread_mutex_unlock(&sta_mutex);pthread_mutex_unlock(&wlan_mutex);pthread_exit((void *) 0);});
			memcpy(values->BSSID, ptr_wlan->BSSID, MAC_ADDR_LEN);
			printf_d("sta in AP wlanname: ath.%d-%d  BSSID: %02X:%02X:%02X:%02X:%02X:%02X\n", values->rid-1, values->wid, 
					values->BSSID[0], values->BSSID[1], values->BSSID[2], values->BSSID[3], values->BSSID[4], values->BSSID[5]);
			APVLog("sta in AP wlanname: ath.%d-%d  BSSID: %02X:%02X:%02X:%02X:%02X:%02X\n", values->rid-1, values->wid, 
					values->BSSID[0], values->BSSID[1], values->BSSID[2], values->BSSID[3], values->BSSID[4], values->BSSID[5]);
			values->encryption_type = 0;
			if(ptr_wlan->sec_policy == OPEN)
				values->encryption_type = 0;
			else if(ptr_wlan->sec_policy == WEP)
				values->encryption_type = 3;
			else if(ptr_wlan->sec_policy == EAP_WPA_WPA2)
				values->encryption_type = 4;
			else if(ptr_wlan->sec_policy == PSK_WPA_WPA2)
				values->encryption_type = 5;
			values->ssid_length = strlen((char *)ptr_wlan->SSID)+1;
			printf_d("ssid[%s]  ssid_length = %d\n", ptr_wlan->SSID, values->ssid_length);
			CREATE_OBJECT_SIZE_ERR(values->SSID, values->ssid_length, {APVLog("create memory error in acw_parse_8021X_sta_online\n");pthread_mutex_unlock(&sta_mutex);pthread_mutex_unlock(&wlan_mutex);pthread_exit((void *) 0);});
			memcpy(values->SSID, ptr_wlan->SSID, values->ssid_length);
			values->value = 2;
			send_message(assemble_sta_association_request, values);
		}
	}

	FREE_OBJ(values->BSSID);
	FREE_OBJ(values->SSID);
	FREE_OBJ(values->MAC);
	
	FREE_OBJ(values);

	pthread_mutex_unlock(&sta_mutex);
	pthread_mutex_unlock(&wlan_mutex);
	
	return TRUE;
}

void *CheckMsgFromOtherPro(void *arg)
{
	int sock;
	int readBytes = 0;
	char s_buff[65536];
	struct sockaddr_in local;
	SOCKADDR_STORAGE server_addr;
	unsigned char sta_mac[6]={0};
	int i = 0;

	TMSG msg;
	int type;
	char *ptr = NULL;
	int seqNum;
	
	set_dhcp_snooping(ENABLED);

	sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (sock < 0)
	{
		perror("Failed to create socket");
		APVLog("Failed to create socket in CheckMsgFromOtherPro, goto exit_thread\n");
		goto  exit;
	}
	
	memset(&local, 0, sizeof(local));
	local.sin_family = AF_INET;
	local.sin_port = htons(3333);
	local.sin_addr.s_addr = htonl(INADDR_ANY);
	int on=1;
	setsockopt(sock,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on));
	if(bind(sock, (struct sockaddr *)&local, sizeof(local)) == -1)
	{
		perror("bind error");
		APVLog("bind error, goto exit_thread\n");
		close(sock);
		goto  exit;
	}
	
	while(1)
	{
		bzero(s_buff, 65536);
		network_receive_unsafe(sock, s_buff, 65536, 0, (struct sockaddr_storage *)&server_addr, &readBytes);

		dhcpsnooping_report_type *auxdhcp_report = NULL;
		msg.msg = (u8 *)s_buff;
		msg.offset = 0;
		seqNum = get_sequeue_num();
		ptr = protocol_retrieve_str(&msg,1);
		type = atoi(ptr);
		FREE_OBJ(ptr);
		switch(type)
		{
			case 1://dhcpsnooping_report
				msg.offset = 0;
				CREATE_OBJECT_ERR(auxdhcp_report, dhcpsnooping_report_type,{close(sock);APVLog("CW_ERROR_OUT_OF_MEMORY\n");printf_d("CW_ERROR_OUT_OF_MEMORY\n");return (NULL);});
				acw_parse_dhcpsnooping_report(&msg, readBytes, auxdhcp_report);
				STA_INFO *str = NULL;
				pthread_mutex_lock(&sta_mutex);
				for(str = sta_list; str != NULL; str = str->next)
				{
					if(macAddrCmp(str->MAC, auxdhcp_report->mac) == 1)
						str->ip = auxdhcp_report->ip;
					printf_d("int_IP: %u\n", str->ip);
				}
				pthread_mutex_unlock(&sta_mutex);
				FREE_OBJ(auxdhcp_report);
				break;
			case 3:
				for(i=0;i<6;i++)
				{
					ptr = protocol_retrieve_str(&msg, 2);
					sta_mac[i] = strtol(ptr, NULL, 16);
					FREE_OBJ(ptr);
				}
				printf_d("8021X_sta_online report sta mac:[%02X-%02X-%02X-%02X-%02X-%02X]\n", sta_mac[0], sta_mac[1],sta_mac[2], sta_mac[3], sta_mac[4], sta_mac[5]);
				acw_parse_8021X_sta_online(sta_mac);
				break;
			default:
				break;
		}
	}
exit:
	//pthread_exit((void *) 0);
	return NULL;
}
void * interval_count_monitor(void *arg)
{
	char str_tmp[64];
	char cmd_str[128];
	char wlanname[24];
	
	WLAN_INFO *ptr = NULL;
/* Begin: Modified by dongzw for signal strength message report 2013-04-11 */
	s8 signal_level = -96;
/* End: Modified by dongzw for signal strength message report 2013-04-11 */
	int signal_level_total = 0;
	int count = 0;
	int sampling_count = COUNT_INTERVAL/SAMPLING_INTERVAL;
	int i;
	u32 bytes[3] = {0,0,0};
	u32 rx_bytes[3][2] = {{0,0},{0,0},{0,0}}, tx_bytes[3][2] = {{0,0},{0,0},{0,0}};
	
	if(IntervalCountMonitorOnce == FALSE)
	{
		interval_count_monitor_info[0].ap_interval_bytes = 0;/* 采样时长内的流量值 */
		interval_count_monitor_info[0].wireless_rx_interval_bytes[0] = 0;
		interval_count_monitor_info[0].wireless_tx_interval_bytes[0] = 0;
		interval_count_monitor_info[0].wireless_rx_interval_bytes[1] = 0;
		interval_count_monitor_info[0].wireless_tx_interval_bytes[1] = 0;
		for(i = 0; i < sampling_count; i++)
		{
			memset(cmd_str, 0, 128);
			sprintf(cmd_str, "/usr/sbin/cpu_util_rate %d >/dev/null &", 50);
			system(cmd_str);
			GetCpuUseRatio(&(interval_count_monitor_info[i].CPU_use_rate_average));
			GetMemUseRatio(&(interval_count_monitor_info[i].MEM_use_rate_average));
			if(interval_count_monitor_info[i].CPU_use_rate_average == 0)
				interval_count_monitor_info[i].CPU_use_rate_average = 100;
			if(interval_count_monitor_info[i].MEM_use_rate_average == 0)
				interval_count_monitor_info[i].MEM_use_rate_average = 1000;
			interval_count_monitor_info[i].CPU_use_rate_average = interval_count_monitor_info[i].CPU_use_rate_average/100;
			interval_count_monitor_info[i].MEM_use_rate_average = interval_count_monitor_info[i].MEM_use_rate_average/100;
			if(CPU_use_rate_max < interval_count_monitor_info[i].CPU_use_rate_average)
				CPU_use_rate_max = interval_count_monitor_info[i].CPU_use_rate_average;
			if(MEM_use_rate_max < interval_count_monitor_info[i].MEM_use_rate_average)
				MEM_use_rate_max = interval_count_monitor_info[i].MEM_use_rate_average;
			interval_count_monitor_info[i].count = sampling_count;
			interval_count_monitor_info[i].rx_signal_average_strength = -96;
			interval_count_monitor_info[i].rx_signal_max_strength = -96;
			interval_count_monitor_info[i].rx_signal_min_strength = -96;
		}		
		IntervalCountMonitorOnce = TRUE;
	}	
	pthread_mutex_lock(&wlan_mutex);
	if(wlan_list != NULL)
	{						
		for(ptr = wlan_list; ptr!= NULL; ptr = ptr->next)
		{
			sprintf(wlanname, "ath.%d-%d", ptr->rid-1, ptr->wid);
			get_wlan_bytes(wlanname,&rx_bytes[0][ptr->rid-1], &tx_bytes[0][ptr->rid-1], &bytes[0]);
			bytes[1] += bytes[0];
			rx_bytes[1][ptr->rid-1] += rx_bytes[0][ptr->rid-1];
			tx_bytes[1][ptr->rid-1] += tx_bytes[0][ptr->rid-1];
			ptr->rx_interval_bytes = rx_bytes[0][ptr->rid-1];
			ptr->tx_interval_bytes = tx_bytes[0][ptr->rid-1];
		}
		bytes[2] = bytes[1];
		rx_bytes[2][0] = rx_bytes[1][0];
		rx_bytes[2][1] = rx_bytes[1][1];
		tx_bytes[2][0] = tx_bytes[1][0];
		tx_bytes[2][1] = tx_bytes[1][1];
	}
	pthread_mutex_unlock(&wlan_mutex);
	sleep(SAMPLING_INTERVAL);/* to sampling the bytes */
	while(1)
	{
		for(i = 0; i < sampling_count; i++)
		{
			if(ReportEnable == DISABLED)
			{
				//system("killall -9 dhcpsnooping");/* if close dhcpsnooping, we can't get sta's IP */
				system("killall -9 linkcheck");
				goto exit;
			}
			memset(cmd_str, 0, 128);
			sprintf(cmd_str, "/usr/sbin/cpu_util_rate %d >/dev/null &", 50);
			system(cmd_str);
			GetCpuUseRatio(&(interval_count_monitor_info[i].CPU_use_rate_average));
			GetMemUseRatio(&(interval_count_monitor_info[i].MEM_use_rate_average));
			interval_count_monitor_info[i].CPU_use_rate_average = interval_count_monitor_info[i].CPU_use_rate_average/100;
			interval_count_monitor_info[i].MEM_use_rate_average = interval_count_monitor_info[i].MEM_use_rate_average/100;
			if(CPU_use_rate_max < interval_count_monitor_info[i].CPU_use_rate_average)
				CPU_use_rate_max = interval_count_monitor_info[i].CPU_use_rate_average;
			if(MEM_use_rate_max < interval_count_monitor_info[i].MEM_use_rate_average)
				MEM_use_rate_max = interval_count_monitor_info[i].MEM_use_rate_average;
			pthread_mutex_lock(&wlan_mutex);
			if(wlan_list != NULL)
			{						
				for(count = 0, signal_level_total = 0, ptr = wlan_list; ptr!= NULL; ptr = ptr->next)
				{
					get_wlan_bytes(wlanname,&rx_bytes[0][ptr->rid-1], &tx_bytes[0][ptr->rid-1], &bytes[0]);
					bytes[1] += bytes[0];
					rx_bytes[1][ptr->rid-1] += rx_bytes[0][ptr->rid-1];
					tx_bytes[1][ptr->rid-1] += tx_bytes[0][ptr->rid-1];
					
					sprintf(wlanname, "ath.%d-%d", ptr->rid-1, ptr->wid);
					memset(cmd_str, 0, 128);
					sprintf(cmd_str, "iwconfig %s|awk '/%s/, /Signal level/'|grep 'Noise level'|awk -F \"Signal level=\" '{print $2}'|awk -F \" \" '{print $1}'",wlanname, wlanname);
					memset(str_tmp, 0, 64);
					popen_fgets(cmd_str, str_tmp, sizeof(str_tmp));
					signal_level = atoi(str_tmp);
					signal_level_total += signal_level;
					if(signal_level > interval_count_monitor_info[i].rx_signal_max_strength)
						interval_count_monitor_info[i].rx_signal_max_strength = signal_level;
					else if(signal_level < interval_count_monitor_info[i].rx_signal_min_strength)
						interval_count_monitor_info[i].rx_signal_min_strength = signal_level;
					
					ptr->rx_interval_bytes = rx_bytes[0][ptr->rid-1] - ptr->rx_interval_bytes;
					ptr->tx_interval_bytes = tx_bytes[0][ptr->rid-1] - ptr->tx_interval_bytes;
					count++;
				}
				interval_count_monitor_info[0].ap_interval_bytes = bytes[1] - bytes[2];/* 采样时长内的流量值 */
				interval_count_monitor_info[0].wireless_rx_interval_bytes[0] = rx_bytes[1][0] - rx_bytes[2][0];
				interval_count_monitor_info[0].wireless_rx_interval_bytes[1] = rx_bytes[1][1] - rx_bytes[2][1];
				interval_count_monitor_info[0].wireless_tx_interval_bytes[0] = tx_bytes[1][0] - tx_bytes[2][0];
				interval_count_monitor_info[0].wireless_tx_interval_bytes[1] = tx_bytes[1][1] - tx_bytes[2][1];
				bytes[2] = bytes[1];
				rx_bytes[2][0] = rx_bytes[1][0];
				rx_bytes[2][1] = rx_bytes[1][1];
				tx_bytes[2][0] = tx_bytes[1][0];
				tx_bytes[2][1] = tx_bytes[1][1];
				if(count != 0)
					interval_count_monitor_info[i].rx_signal_average_strength = signal_level_total/count;	
			}
			pthread_mutex_unlock(&wlan_mutex);
			sleep(SAMPLING_INTERVAL);
		}
	}
exit:
	APVLog("thread interval_count_monitor close\n");
	pthread_exit((void *) 0);
	return NULL;
}

BOOL restart_hostapd_for_hot_start(WLAN_INFO *wlan)
{
	int need_hostapd = 1;
	char wlanname[32] = {0},filename[64] = {0},cmd[512] = {0};

	sprintf(wlanname, "ath.%d-%d", wlan->rid-1, wlan->wid);
	if(wlan->sec_policy == EAP_WPA_WPA2)
		snprintf(filename,sizeof(filename), "%s/eap_%s", HOSTAPD_PROFILE_PATH, wlanname);
	else if(wlan->sec_policy == EAP_WPA)
		snprintf(filename,sizeof(filename), "%s/eap_wpa_%s",HOSTAPD_PROFILE_PATH, wlanname);
	else if(wlan->sec_policy == EAP_WPA2)
		snprintf(filename,sizeof(filename), "%s/eap_wpa2_%s",HOSTAPD_PROFILE_PATH, wlanname);
	else if(wlan->sec_policy == PSK_WPA_WPA2)
		snprintf(filename,sizeof(filename), "%s/psk_%s",HOSTAPD_PROFILE_PATH, wlanname);
	else if(wlan->sec_policy == PSK_WPA)
		snprintf(filename,sizeof(filename), "%s/psk_wpa_%s",HOSTAPD_PROFILE_PATH, wlanname);
	else if(wlan->sec_policy == PSK_WPA2)
		snprintf(filename,sizeof(filename), "%s/psk_wpa2_%s",HOSTAPD_PROFILE_PATH, wlanname);
	else if(wlan->sec_policy == WEP)
		need_hostapd = 0;
	else
	{	/*open mode*/
		need_hostapd = 0;
	}
	if(need_hostapd == 1){
		sprintf(cmd,"hostapd %s > /dev/null 2>&1 &",filename);
		SYSTEM(cmd);
	}
	return TRUE;
}


