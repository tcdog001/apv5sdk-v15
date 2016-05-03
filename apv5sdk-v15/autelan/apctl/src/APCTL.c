#include "global.h"

#define CONFIG_FILE    "/tmp/config.wtp"
#define DHCP_OPTION43_AC_ADDR_FILE "/tmp/ac-addr"
#define DEFAULT_LOG_SIZE					1000000

extern pthread_mutex_t neighbor_apinfo_list_mutex;
extern pthread_mutex_t siocg_80211_stats;
extern pthread_mutex_t path_procnet_dev;
extern FILE *fp;
extern FILE *vfp;
extern FILE *message_report_fp;	// Add by dongzw for station association message report log 2013-04-09
extern int tmp_ln;
extern int ln;
extern int message_report_ln;	// Add by dongzw for station association message report log 2013-04-09

void *thread_report_eth_info(void *arg);
void *thread_report_ap_state(void *arg);
void *thread_manage_msg(void *arg);

pthread_t counter_monitor;
pthread_t dhcp_snooping;
pthread_t tid_report_ap_state;
pthread_t tid_report_eth_info;
pthread_t thread_apScanning;

char ReportEnable = DISABLED;
char Sta_ReportEnable = DISABLED;
char Wlan_ReportEnable = DISABLED;
u32 ap_report_interval = 180;
u32 eth_report_interval = 180;
/* Begin: Add by dongzw for ap/wireless/eth information report time 2012-12-21 */
int ap_report_switch = DISABLED;
int eth_report_switch = DISABLED;
/* End: Add by dongzw for ap/wireless/eth information report time 2012-12-21 */

unsigned long AP_START_TIME;
unsigned long AP_CURRENT_TIME;

BOOL WLanReceiveThreadRun;


sock_t    ctl_sd;
PTHREAD_SAFE_VARIABLES safe_var;
CMD_LST   *cmd_list_header, *cmd_list_tail;


pthread_mutex_t  wlan_mutex;
pthread_mutex_t  sta_mutex;

char ifname[32];
char dst[32];
char src[32];
char hostip[32];
char loip[32];
char JoinAcReasion[32];
BOOL traffic_node_up_switch;
BOOL traffic_node_down_switch;
BOOL traffic_vap_up_switch;
BOOL traffic_vap_down_switch;

AP_CONFIG *apconf;
int apconf_count;
char **ac_addr;
char *domain;
int ac_count_opt43;
int ac_count_dns;
int ac_count_static;
int ac_count_broadcast;
char *ac_list_option43;
char *ac_list_dns;
char *ac_list_static;
char *ac_list_broadcast;



char SOFTWARE_VER[32];
char SOFTWARE_FEATURES_YARDS[32];
char COMPANY[64];
u8   DEV_MAC[MAC_ADDR_LEN];
char DEV_TYPE[32];
char HARDWARE_VER[32];
u8   GET_AC_METHOD;
char BOOT_VER[32];
char SN[32];
u8   RADIO_COUNT;
int  *RADIO_MODE;
u8	isBackup = 0;


SOCKADDR_STORAGE dst_addr;
extern BOOL restart_hostapd_for_hot_start(WLAN_INFO *wlan);
extern BOOL create_sta_check_thread();

//save parameters, which apctl used to hot start, added by diaowq@20120915
BOOL init_config_backup_dir()
{
	int i = 0;
	char pathbuf[128];
	
	//make sure the dir of "conf_apctl" is exist		
	if(0 != access(CONF_APCTL_PATH, F_OK))
	{
		mkdir(CONF_APCTL_PATH,S_IRWXU);
	}
	//make sure the dir of "conf_apctl/radio" is exist
	for(i = 0 ; i < RADIO_COUNT; i++)
	{
		memset(pathbuf,0,128);
		sprintf(pathbuf,"%s%d",CONF_RADIO_PATH,i);	
		if(0 != access(pathbuf, F_OK))
		{
			mkdir(pathbuf,S_IRWXU);
		}	
	}

	return TRUE;
}
BOOL save_config_info(Conf_Type conf_type,ConfArg confArg,BOOL chkdir)
{
	BOOL bRet = TRUE;
	//chkdir = 0;//has made sure the dir is existed.so no need to check again.
	//int debug_print_dwq = 1;
	//if(debug_print_dwq)
	//	printf("conf_type:%d\t radioID:%d\t wlanID:%d\t %s\t = %s\n ",conf_type,confArg.radioID,confArg.wlanID,confArg.conf_arg,confArg.conf_value);
	
	if(CONF_APCTL == conf_type)
	{
		//make sure the dir of  "conf_apctl" is exist		
		if(chkdir == TRUE)
		{
			if(0 != access(CONF_APCTL_PATH, F_OK))
			{
				mkdir(CONF_APCTL_PATH,S_IRWXU);
			}
		}
		//save the attribute of the configuration of "apctl"
		int fd;
		char filepath[128];
		memset(filepath,0,128);
		sprintf(filepath,"%s/%s",CONF_APCTL_PATH,confArg.conf_arg);
		if((fd = open(filepath,O_CREAT|O_RDWR,S_IRWXU))== -1)
		{ 
			printf("create file %s failed!\n",filepath);			
		}
		else
		{
			write(fd,confArg.conf_value,sizeof(confArg.conf_value));
			close(fd);
		}
	}
	else if(CONF_RADIO == conf_type)
	{
		//make sure the dir of  "conf_apctl/radio" is exist
		char pathbuf[128];
		memset(pathbuf,0,128);
		sprintf(pathbuf,"%s%d",CONF_RADIO_PATH,confArg.radioID);	
		//if(debug_print_dwq)
		//	printf("CONF_RADIO pathbuf=%s \t %s=%s\n",pathbuf,confArg.conf_arg,confArg.conf_value);
		if(chkdir == TRUE)
		{
			if(0 != access(pathbuf, F_OK))
			{
				mkdir(pathbuf,S_IRWXU);
			}	
		}
		//save the attribute of the configuration of "radio"
		int fd;
		char filepath[128];
		memset(filepath,0,128);
		sprintf(filepath,"%s/%s",pathbuf,confArg.conf_arg);
		if((fd = open(filepath,O_CREAT|O_RDWR,S_IRWXU))== -1)
		{ 
			printf("create file %s failed!\n",filepath);			
		}
		else
		{
			write(fd,confArg.conf_value,sizeof(confArg.conf_value));
			close(fd);
		}
	}
	else if(CONF_WLAN ==conf_type)
	{
		//make sure the dir of "conf_wtpd/radio/wlan" is exist
		char pathbuf1[128];
		char pathbuf2[128];
		memset(pathbuf1,0,128);
		memset(pathbuf2,0,128);
		sprintf(pathbuf1,"%s%d/wlaninfo",CONF_RADIO_PATH,confArg.radioID);		
		sprintf(pathbuf2,"%s%d/wlaninfo/wlan%d",CONF_RADIO_PATH,confArg.radioID,confArg.wlanID); 			
		//if(debug_print_dwq)
		//	printf("CONF_WLAN pathbuf=%s \t %s=%s\n",pathbuf2,confArg.conf_arg,confArg.conf_value);
		if(chkdir == TRUE)
		{
			if(0 != access(pathbuf1, F_OK))
			{
				mkdir(pathbuf1,S_IRWXU);
			}			
			if(0 != access(pathbuf2, F_OK))
			{
				mkdir(pathbuf2,S_IRWXU);
			}	
		}
		//save the attribute of the configuration of "wlan"		
		int fd;
		char filepath[128];
		memset(filepath,0,128);
		sprintf(filepath,"%s/%s",pathbuf2,confArg.conf_arg);
		if((fd = open(filepath,O_CREAT|O_RDWR,S_IRWXU))== -1)
		{ 
			printf("create file %s failed!\n",filepath);			
		}
		else
		{
			write(fd,confArg.conf_value,sizeof(confArg.conf_value));
			close(fd);
		}
	}
	else
	{
		bRet = FALSE;
		if(debugf)
			printf("CONF_return_false\n");		
	}
	return bRet;
}
BOOL save_apctl_level_config(char *conf_arg, int value_type, void *conf_value, BOOL chkdir)
{
	ConfArg confArg;
	memset(&confArg,0,sizeof(ConfArg));
	strcpy(confArg.conf_arg,conf_arg);
	if(value_type == INT)
		sprintf(confArg.conf_value,"%d",*(int*)conf_value);
	else if(value_type == CHAR)
		sprintf(confArg.conf_value,"%d",*(unsigned char*)conf_value);
	else if(value_type == SHORT)
		sprintf(confArg.conf_value,"%hd",*(unsigned short*)conf_value);
	else if(value_type == UL)
		sprintf(confArg.conf_value,"%lu",*(unsigned long*)conf_value);
	else if(value_type == STR)
		sprintf(confArg.conf_value,"%s",(u8*)conf_value);
	return save_config_info(CONF_APCTL,confArg,chkdir);
}
BOOL save_radio_level_config(int radio_id, char *conf_arg, int value_type, void *conf_value, BOOL chkdir)
{
	ConfArg confArg;
	memset(&confArg,0,sizeof(ConfArg));
	confArg.radioID = radio_id;
	strcpy(confArg.conf_arg,conf_arg);
	if(value_type == INT)
		sprintf(confArg.conf_value,"%d",*(int*)conf_value);
	else if(value_type == CHAR)
		sprintf(confArg.conf_value,"%d",*(unsigned char*)conf_value);
	else if(value_type == SHORT)
		sprintf(confArg.conf_value,"%hd",*(unsigned short*)conf_value);
	else if(value_type == UL)
		sprintf(confArg.conf_value,"%lu",*(unsigned long*)conf_value);
	else if(value_type == STR)
		sprintf(confArg.conf_value,"%s",(u8*)conf_value);
	return save_config_info(CONF_RADIO,confArg,chkdir);
}
BOOL save_wlan_level_config(int radio_id, int wlan_id, char *conf_arg, int value_type, void *conf_value, BOOL chkdir)
{
	ConfArg confArg;
	memset(&confArg,0,sizeof(ConfArg));
	confArg.radioID = radio_id;
	confArg.wlanID  = wlan_id;
	strcpy(confArg.conf_arg,conf_arg);
	if(value_type == INT)
		sprintf(confArg.conf_value,"%d",*(int*)conf_value);
	else if(value_type == CHAR)
		sprintf(confArg.conf_value,"%d",*(unsigned char*)conf_value);
	else if(value_type == SHORT)
		sprintf(confArg.conf_value,"%hd",*(unsigned short*)conf_value);
	else if(value_type == UL)
		sprintf(confArg.conf_value,"%lu",*(unsigned long*)conf_value);
	else if(value_type == STR){
		sprintf(confArg.conf_value,"%s",(char*)conf_value);
		//printf_d("%s,%d,value=%s\n",__func__,__LINE__,confArg.conf_value);
	}
	return save_config_info(CONF_WLAN,confArg,chkdir);
}
BOOL get_config_from_file(char filepath[128],char filename[64],char * str)
{
	int fd;
	char file[192];
	memset(file,0,192);
	strcat(file,filepath);
	strcat(file,filename);
	if((fd = open(file,O_RDONLY))== -1)
	{
		if(debugf)
			printf("open file %s failed! \t Use Default Value.\n",file);	
		return FALSE;
	}
	else
	{
		read(fd,str,64);
		if(debugf)
			printf("from %s\tget %s = %s\n",file,filename,str);
		close(fd);
	}	
	return TRUE;
}

#if 0
BOOL get_ac_addr()
{
	char cmd[128] = {0};
	char str[32] = {0};
	while(0 != access("/tmp/ac-addr", F_OK))
	{
		printf_d("Get AC IP address failed\n");
		sleep(2);
	}
	sprintf(cmd, "cat /tmp/ac-addr");
	if(popen_fgets(cmd, str, sizeof(str)) == NULL)
		return FALSE;
	
	strncpy(AC_ADDR, trim(str), 32);	
	printf_d("%-16s: %-24s len: %d\n", "AC_ADDR", AC_ADDR, strlen(AC_ADDR));
	APVLog("%-16s: %-24s len: %d", "AC_ADDR", AC_ADDR, strlen(AC_ADDR));
	
	return TRUE;
}
#else
BOOL get_ac_addr(char **ac_addr, int ac_count, char **addr)
{
	int i = 0;
	int count = 0;
	if((ac_count < 1) || (addr == NULL))
		return FALSE;
	if(ac_count > 32)
	{
		count = 32;
	}
	else
	{
		count = ac_count;
	}
	//*ac_addr = (char *)malloc(count*17);
	CREATE_STRING_ERR(*ac_addr,count*17,MALLOC_ERR_RET(FALSE));
	if(*ac_addr == NULL)
		return FALSE;
	if(addr != NULL)
	{
		strcpy(*ac_addr, addr[0]);
		for(i = 1; i < count; i++)
		{
			strcat(*ac_addr, "%");
			strcat(*ac_addr, addr[i]);
		}
	}
	else
		return FALSE;
	return TRUE;
}

#endif
BOOL get_commpany()
{
	char cmd[128] = {0};
	char str[64] = {0};
	
	sprintf(cmd, "showsysinfo | awk -F \":\" '/Company Name/ {print $2}'");
	if(popen_fgets(cmd, str, sizeof(str)) == NULL)
		return FALSE;
	
	strncpy(COMPANY, trim(str), 64);	
	printf_d("%-16s: %-24s len: %d\n", "COMPANY", COMPANY, strlen(COMPANY));
	APVLog("%-16s: %-24s len: %d\n", "COMPANY", COMPANY, strlen(COMPANY));
	
	return TRUE;
}
BOOL get_sn()
{
	char cmd[128] = {0};
	char str[32] = {0};
	
	sprintf(cmd, "showsysinfo | awk -F \":\" '/SN/ {print $2}'");
	if(popen_fgets(cmd, str, sizeof(str)) == NULL)
		return FALSE;
	
	strncpy(SN, trim(str), 32);	
	printf_d("%-16s: %-24s len: %d\n", "SN", SN, strlen(SN));
	APVLog("%-16s: %-24s len: %d\n", "SN", SN, strlen(SN));
	
	return TRUE;
}

BOOL get_radio_count()
{
	char cmd[128] = {0};
	char str[32] = {0};

	if(0 != access(DEVINFO_PATH, F_OK))
		return FALSE;
	sprintf(cmd, "grep \"<wifi_total>\" -C 1 %s | awk -F '[><]' '/count/ {print $3}'", DEVINFO_PATH);
	if(popen_fgets(cmd, str,sizeof(str)) == NULL)
		return FALSE;
	RADIO_COUNT = atoi(str);
	save_apctl_level_config("RADIO_COUNT",CHAR,(u8*)&RADIO_COUNT,1);
	return TRUE;
}
int get_radio_support_mode(char *str)
{
	int rmode = 0;
	if(strcmp(str, "A") == 0)
	{
		rmode = MODE_11A;
	}
	else if(strcmp(str, "B") == 0)
	{
		rmode = MODE_11B;
	}
	else if(strcmp(str, "G") == 0)
	{
		rmode = MODE_11G;
	}
	else if(strcmp(str, "N5") == 0)
	{
		rmode = MODE_11N_5;
	}
	else if(strcmp(str, "N24") == 0)
	{
		rmode = MODE_11N_24;
	}
	else if(strcmp(str, "BG") == 0)
	{
		rmode = MODE_11B | MODE_11G;
	}
	else if(strcmp(str, "AN") == 0)
	{
		rmode = MODE_11A | MODE_11N_5;
	}
	else if(strcmp(str, "GN") == 0)
	{
		rmode = MODE_11G | MODE_11N_24;
	}
	else if((strcmp(str, "BGN") == 0) ||(strcmp(str, "N") == 0))
	{
		rmode = MODE_11B | MODE_11G | MODE_11N_24;
	}
	else
	{
		rmode = 0;
	}
	return rmode;
}
BOOL get_radio_mode()
{
	char cmd[1152] = {0};
	char *buf = NULL;
	char str[32] = {0};
	int i = 0;

	if(0 != access(DEVINFO_PATH, F_OK) || RADIO_COUNT <= 0)
		return FALSE;
	
	//buf = (char *)malloc(1024);
	CREATE_STRING_ERR(buf,1024,MALLOC_ERR_RET(FALSE));
	if(buf == NULL)
		return FALSE;
	RADIO_MODE = (int *)malloc(RADIO_COUNT);
	memset(RADIO_MODE,0,RADIO_COUNT);
	if(RADIO_MODE == NULL){
		FREE_OBJ(buf);//diaowq add@20120929 for memory leak
		return FALSE;
	}
	
	sprintf(cmd, "grep \"<wifi_total>\" -A 17 %s", DEVINFO_PATH);
	if(popen_fread(cmd, buf, 1024) == NULL){
		FREE_OBJ(buf);				//diaowq add@20120929 for memory leak
		FREE_OBJ(RADIO_MODE);	//diaowq add@20120929 for memory leak
		return FALSE;
	}
	i = 0;
	for(i = 0; i < RADIO_COUNT; i++)
	{
		memset(str, 0, sizeof(str));
		sprintf(cmd, "echo \"%s\" | grep \"<wifi%d>\" -A 5 | awk -F '[><]' '/mode/ {print $3}'", buf, i);
		if(popen_fgets(cmd, str,sizeof(str)) == NULL){
			FREE_OBJ(buf);				//diaowq add@20120929 for memory leak
			FREE_OBJ(RADIO_MODE);	//diaowq add@20120929 for memory leak
			return FALSE;
		}
		if(low_to_upper(str) == TRUE)
		{
			RADIO_MODE[i] = get_radio_support_mode(str);
		}
		else
			RADIO_MODE[i] = 0;
	}
	
	return TRUE;
}


BOOL get_boot_version()
{
	strcpy(BOOT_VER, "1.0");
	APVLog("%-16s: %-24s len: %d\n", "Hardware Version", BOOT_VER, strlen(BOOT_VER));
	printf_d("%-16s: %-24s len: %d\n", "Hardware Version", BOOT_VER, strlen(BOOT_VER));
	return TRUE;
}

BOOL get_hardware_version(G)
{
	char cmd[128] = {0};
	char str[64]  = {0};
	/* get ap software version */
	sprintf(cmd, "showsysinfo | awk -F \":\" '/Hardware Version/ {print $2}'");
	if(popen_fgets(cmd, str, sizeof(str)) == NULL)
		return FALSE;
	strcpy(HARDWARE_VER, trim(str));
	APVLog("%-16s: %-24s len: %d\n", "Hardware Version", str, strlen(str));
	printf_d("%-16s: %-24s len: %d\n", "Hardware Version", str, strlen(str));
	return TRUE;
}
BOOL get_dev_type()
{
	char cmd[128] = {0};
	char str[32] = {0};
	/* get ap real model, 090519 */
	memset(cmd, 0, 128);
	sprintf(cmd, "showsysinfo | awk -F \":\" '/Device Type/ {print $2}'");
	if(popen_fgets(cmd, str, sizeof(str)) == NULL)
		return FALSE;
	
	strncpy(DEV_TYPE, trim(str), 32);
	
	APVLog("%-16s: %-24s len: %d\n", "DEVICE TYPE", DEV_TYPE, strlen(DEV_TYPE));
	printf_d("%-16s: %-24s len: %d\n", "DEVICE TYPE", DEV_TYPE, strlen(DEV_TYPE));
	return TRUE;
}
BOOL get_bssid(u8 rid, u8 wid, u8 *bssid)
{
	char wlanname[16] = {0};
	int s = 0;
	int i = 0;
	struct ifreq ifr;
	if(bssid == NULL)
		return FALSE;
	s = socket(AF_INET, SOCK_DGRAM, 0);
	if (s < 0) 
	{
		printf_d("Error Creating Socket for ioctl"); 
		return RAISE_ERROR(ERROR_GENERAL, NULL);;
	}
	sprintf(wlanname,"ath.%d-%d", rid, wid);
	strncpy(ifr.ifr_name,wlanname,IFNAMSIZ);
	if (ioctl(s, SIOCGIFHWADDR, &ifr)==-1) 
	{
		printf_d("get wlan mAC error\n");
	}
	else
	{
		for (i=0; i<MAC_ADDR_LEN; i++)
		{
			bssid[i]=(u8)ifr.ifr_hwaddr.sa_data[i];
		}
	}
	printf_d("%s  BSSID [%02X:%02X:%02X:%02X:%02X:%02X]\n", wlanname, bssid[0], bssid[1], bssid[2], bssid[3], bssid[4], bssid[5]);
	close(s);
	return TRUE;
}
BOOL get_mac()
{
	char cmd[128] = {0};
	char str[64] = {0};
	char str1[64] = {0};
	
	memset(cmd, 0, sizeof(cmd));
	sprintf(cmd, "showsysinfo | awk -F \":\" '/MAC/ {print $2\":\"$3\":\"$4\":\"$5\":\"$6\":\"$7}'");
	while(1)
	{
		memset(str, 0, 64);
		if(popen_fgets(cmd, str1, sizeof(str1)) == NULL)
			return FALSE;
		
		strncpy(str, trim(str1), sizeof(str1));
		sscanf(str, "%02hhX:%02hhX:%02hhX:%02hhX:%02hhX:%02hhX", 
			   &DEV_MAC[0],&DEV_MAC[1],&DEV_MAC[2],&DEV_MAC[3],&DEV_MAC[4],&DEV_MAC[5]);	
		if((0 == DEV_MAC[0])&&(0 == DEV_MAC[1])&&(0 == DEV_MAC[2]))
		{
			sleep(1);
		}
		else
			break;
	}
	APVLog("%-16s: %-24s len: %d\n", "MAC", str, strlen(str));
	printf_d("%-16s: %-24s len: %d\n", "MAC", str, strlen(str));
	return TRUE;
}

BOOL get_software_version()
{
	char cmd[128] = {0};
	char str1[64] = {0};
	char str[64]  = {0};
	int i = 0;
	/* get ap software version */
	memset(cmd, 0, 128);
	memset(str1, 0, 64);
	if(0 != access("/usr/sbin/showsysinfo", F_OK))
	{
		memset(SOFTWARE_VER, 0, sizeof(SOFTWARE_VER));
		return FALSE;
	}
	sprintf(cmd, "showsysinfo | awk -F \":\" '/Software Version/ {print $2}'");
	if(popen_fgets(cmd, str1, sizeof(str1)) == NULL)
		return FALSE;
	memset(str, 0, 64);
	strcpy(str, (const char *)trim(str1));
	APVLog ("%-16s: %-24s len: %d\n", "Software Version", str, strlen(str));
	printf_d("%-16s: %-24s len: %d\n", "Software Version", str, strlen(str));
	for(i = 0; i < strlen(str); i++)
	{
		if(str[i] == '#')
			str[i] = '\0';
	}	
	memcpy(SOFTWARE_VER, str, strlen(str));
	return TRUE;
}

BOOL get_software_features_yards()
{
	char cmd[128] = {0};
	char str1[64] = {0};
	char str[64]  = {0};
	int i = 0;
	/* get ap software version */
	memset(cmd, 0, 128);
	memset(str1, 0, 64);
	if(0 != access("/usr/sbin/showver", F_OK))
	{
		memset(SOFTWARE_FEATURES_YARDS, 0, sizeof(SOFTWARE_FEATURES_YARDS));
		return FALSE;
	}
	sprintf(cmd, "showver"); 
	if(popen_fgets(cmd, str1, sizeof(str1)) == NULL)
		return FALSE;
	memset(str, 0, 64);
	strcpy(str, (const char *)trim(str1));
	APVLog ("%-16s: %-24s len: %d\n", "Software Features Yards", str, strlen(str));
	printf_d("%-16s: %-24s len: %d\n", "Software Features Yards", str, strlen(str));
	for(i = 0; i < strlen(str); i++)
	{
		if(str[i] == '#')
			str[i] = '\0';
	}	
	memcpy(SOFTWARE_FEATURES_YARDS, str, strlen(str));
	return TRUE;
}


void usage()
{
	printf("apctl [options] [values]\n");
	printf("%-4s  %-8s  %s\n", " ", "-d", "open debug switch");
	printf("%-4s  %-8s  %s\n", " ", "-i", "eth interface");
	printf("%-4s  %-8s  %s\n", " ", "-src", "source ip address");
	printf("%-4s  %-8s  %s\n", " ", "-dst", "dest ip address");
	printf("%-4s  %-8s  %s\n", " ", "-le", "open log switch");
	printf("%-4s  %-8s  %s\n", " ", "-nle", "close log switch");
	printf("%-4s  %-8s  %s\n", " ", "-lm", "set /jffs/apctl.log file max size");
	printf("%-4s  %-8s  %s\n", " ", "-tlm", "set /tmp/apctl.log file max size");
	printf("%-4s  %-8s  %s\n", " ", "-v", "apctl hot start.");
	printf("%-4s  %-8s  %s\n", " ", "-h", "help");
}
/* */
BOOL get_cmd_line_parameters(int argc, char *argv[])
{
	int i = 1;
	char opt[32] = {0};
	if(argc < 1)
		goto err;
	for(i = 1; i < argc; i++)
	{
		strncpy(opt, argv[i], sizeof(opt));
		if(low_to_upper(opt) == FALSE)
			goto err;
		if((strcmp(opt, "-V") == 0))
		{
			isBackup = 1;
		}else if((strcmp(opt, "-D") == 0) || (strcmp(opt, "-DEBUG") == 0))
		{
			debugf = TRUE;
		}
		else if(strcmp(opt, "-LE") == 0)
		{
			log_enable = TRUE;
		}
		else if(strcmp(opt, "-SRC") == 0)
		{
			i++;
			if(argv[i] == NULL)
				goto err;
			strcpy(src, argv[i]);
		}
		else if(strcmp(opt, "-DST") == 0)
		{
			i++;
			if(argv[i] == NULL)
				goto err;
			strcpy(dst, argv[i]);
		}
		else if(strcmp(opt, "-I") == 0)
		{
			i++;
			if(argv[i] == NULL)
				goto err;
			strcpy(ifname, argv[i]);
		}
		else if(strcmp(opt, "-LM") == 0)
		{
			i++;
			if(argv[i] == NULL)
				goto err;
			log_max = atoi(argv[i]);
			if(log_max == 0)
				log_max = 200;
		}
		else if(strcmp(opt, "-4") == 0)
		{
			NetWorkFamily = IPV4;
		}
		else if(strcmp(opt, "-6") == 0)
		{
			NetWorkFamily = IPV6;
		}
		else if(strcmp(opt, "-TLM") == 0)
		{
			i++;
			if(argv[i] == NULL)
				goto err;
			tmp_log_max = atoi(argv[i]);
			if(tmp_log_max == 0)
				tmp_log_max = 3000;
		}
		else if(strcmp(opt, "-NLE") == 0)
		{
			log_enable = FALSE;
		}
		else if(strcmp(opt, "-R") == 0)
		{
			i++;
			memset(JoinAcReasion,0,32);
			strcpy(JoinAcReasion, argv[i]);
		}
		else
			goto err;
	}
	return TRUE;
err:
	usage();
	return FALSE;
}
void init()
{
	sigset_t mask;
	WLanReceiveThreadRun = FALSE;
	log_enable = TRUE;
	tmp_log_max = 3000;
	log_max = 200;
	message_report_max = 3000;	// Add by dongzw for station association message report log 2013-04-09
	GET_AC_METHOD = 0;
	strcpy(COMPANY, "autelan");
	heartbeat_timer = -1;
	RADIO_MODE = NULL;
	NetWorkFamily = IPV4;
	echotime = ECHO_TIME;
	cmd_list_header = NULL;
	cmd_list_tail = NULL;
	ntpserverip = 0xC0A80101;
	ntpinterval = 0xA8C0;
	hri = NULL;
	tri = NULL;
	rs.count = 0;
	rs.rate = NULL;
	rs.f    = NULL;
	domain = NULL;
	ac_addr = NULL;
	apconf = NULL;
	wlan_list = NULL;
	sta_list = NULL;
	StaBlackList = NULL;
	radius_list = NULL;
	interval_count_monitor_info = NULL;
	local_tunnel = FALSE;
	overtime_count = 0;
	CPU_use_rate_max = 1;
	MEM_use_rate_max = 10;
	neighbor_apinfo_list = NULL;
	traffic_vap_up_switch 	= FALSE;
	traffic_vap_down_switch = FALSE;
	traffic_node_up_switch  = FALSE;
	traffic_node_down_switch= FALSE;

	strcpy(ifname, "default");

	pthread_mutex_init(&wlan_mutex, NULL);
	pthread_mutex_init(&sta_mutex, NULL);
	pthread_mutex_init(&seq_mutex, NULL);
	pthread_mutex_init(&safe_var.rcv_mutex, NULL);
	pthread_cond_init(&safe_var.rcv_cond, NULL);
	pthread_mutex_init(&neighbor_apinfo_list_mutex, NULL);
	pthread_mutex_init(&siocg_80211_stats, NULL);
	pthread_mutex_init(&path_procnet_dev, NULL);
	/* mask block SIGALRM */	
	sigemptyset(&mask);
	sigaddset(&mask,  SIGALRM);
	pthread_sigmask(SIG_BLOCK, &mask, NULL);
	AP_START_TIME = get_ap_start_time();
	signal(SIGSEGV, sigstop_handle);
	signal(SIGTERM, sigstop_handle);
	signal(SIGINT, sigstop_handle);
	fp = NULL;
	vfp = NULL;
	message_report_fp = NULL;	// Add by dongzw for station association message report log 2013-04-09
}
BOOL BackupWlanTableAdd(WLAN_INFO *wlanBackup)
{
	WLAN_INFO *ptr = NULL;
	
	for(ptr = wlan_list;ptr != NULL; ptr = ptr->next)
	{
		if((wlanBackup->rid == ptr->rid)&&(wlanBackup->wid == ptr->wid))
		{
			printf_d("wlan("MAC_FMT") is already in table\n",MAC2STR(wlanBackup->BSSID));
			return TRUE;
		}
	}
	/*forward insert*/
	wlanBackup->next = wlan_list;
	wlan_list = wlanBackup;
	
	return TRUE;
}

BOOL recover_sta_black_list()
{
	FILE *fd = NULL;
	unsigned char buf[64] = {0}, mac[64] ={0}, filepath[256] = {0};

	sprintf(filepath,"%s/StaBlackList",CONF_APCTL_PATH); 
	if((fd=fopen(filepath,"r"))==NULL)
	{
		printf_d("%s,%d,open file %s failed!\n", __func__, __LINE__, filepath);
		return FALSE;
	}
	else
	{
		while (fgets(buf, sizeof(buf),fd) != NULL)
		{	
			macAddrFiltration *staBlackNode;
			memset(mac,0,6);
			sscanf(buf, "%02hhx:%02hhx:%02hhx:%02hhx:%02hhx:%02hhx",&mac[0],&mac[1],&mac[2],&mac[3],&mac[4],&mac[5]);
			printf_d("%s,get mac is "MAC_FMT"\n", __func__, MAC2STR(mac));

			CREATE_OBJECT_ERR(staBlackNode,macAddrFiltration,{RAISE_ERROR(ERROR_OUT_OF_MEMORY, NULL);APVLog("create memory error\n");});

			memcpy(staBlackNode->mac,mac,6);
			/*forward insert*/
			staBlackNode->next = StaBlackList;
			StaBlackList = staBlackNode;
		}
	}
	return TRUE;
}
BOOL init_config_from_backup()
{	
	int i = 0;
	char str[64] = {0}, filepath[128] = {0};	

	//get the config parameters of apctl level
	sprintf(filepath,"%s/",CONF_APCTL_PATH);
	if(get_config_from_file(filepath,"traffic_node_down_switch",str))
		traffic_node_down_switch = atoi(str);	
	if(get_config_from_file(filepath,"traffic_node_up_switch",str))
		traffic_node_up_switch = atoi(str);	
	if(get_config_from_file(filepath,"traffic_vap_down_switch",str))
		traffic_vap_down_switch = atoi(str);	
	if(get_config_from_file(filepath,"traffic_vap_up_switch",str))
		traffic_vap_up_switch = atoi(str);	
	if(get_config_from_file(filepath,"local_tunnel",str))
		local_tunnel = atoi(str);	
	if(get_config_from_file(filepath,"Sta_ReportEnable",str))
		Sta_ReportEnable	= atoi(str);	
	if(get_config_from_file(filepath,"Wlan_ReportEnable",str))
		Wlan_ReportEnable = atoi(str);	
	if(get_config_from_file(filepath,"RADIO_COUNT",str))
		RADIO_COUNT = atoi(str);	
	//recover the sta black list
	recover_sta_black_list();

	//get the config parameters of radio level
	for(i = 0; i < RADIO_COUNT ; i++){
		memset(filepath,0,sizeof(filepath));
		sprintf(filepath,"%s/radio%d/",CONF_APCTL_PATH,i);
		if(get_config_from_file(filepath,"wifi_start_time",str))
			gRadioInfoValue[i].wifi_start_time = atol(str);	
		if(get_config_from_file(filepath,"wifi_current_time",str))
			gRadioInfoValue[i].wifi_current_time = atol(str);	
		if(get_config_from_file(filepath,"radiotype",str))
			gRadioInfoValue[i].radiotype = atoi(str);	
		if(get_config_from_file(filepath,"auto_txpower",str))
			gRadioInfoValue[i].auto_txpower = atoi(str);	
		if(get_config_from_file(filepath,"txpower",str))
			gRadioInfoValue[i].txpower = atoi(str);	
		if(get_config_from_file(filepath,"channel",str))
			gRadioInfoValue[i].channel = atoi(str);	
		if(get_config_from_file(filepath,"chainmask",str))
			gRadioInfoValue[i].chianmask = atoi(str);	
		if(get_config_from_file(filepath,"AMPDU",str))
			gRadioInfoValue[i].AMPDU = atoi(str);	
		if(get_config_from_file(filepath,"channel_width",str))
			gRadioInfoValue[i].channel_width = atoi(str);	
		if(get_config_from_file(filepath,"shortGI",str))
			gRadioInfoValue[i].shortGI = atoi(str);	
		if(get_config_from_file(filepath,"HTMixed",str))
			gRadioInfoValue[i].HTMixed = atoi(str);	
		if(get_config_from_file(filepath,"AMSDU",str))
			gRadioInfoValue[i].AMSDU = atoi(str);	
		if(get_config_from_file(filepath,"beaconInterval",str))
			gRadioInfoValue[i].beaconInterval = atoi(str);	
		if(get_config_from_file(filepath,"dtim",str))
			gRadioInfoValue[i].dtim = atoi(str);	
		if(get_config_from_file(filepath,"fragThreshold",str))
			gRadioInfoValue[i].fragThreshold = atoi(str);	
		if(get_config_from_file(filepath,"rtsThreshold",str))
			gRadioInfoValue[i].rtsThreshold = atoi(str);	
		if(get_config_from_file(filepath,"preamble",str))
			gRadioInfoValue[i].preamble = atoi(str);	
		if(get_config_from_file(filepath,"rate_mask",str))
			gRadioInfoValue[i].rate_mask = atoi(str);	
		if(get_config_from_file(filepath,"wireless_if_updown_count",str))
			wireless_if_updown_count[i] = atoi(str);	

		//get the config parameters of wlan level
		DIR *dir;
		struct dirent *ptr;
		strcat(filepath,"wlaninfo");
		if((dir = opendir(filepath)) != NULL)
		{
			printf_d("%s,%d\n",__func__,__LINE__);
			while((ptr = readdir(dir)) != NULL)
			{
				printf_d("%s,%d\n",__func__,__LINE__);
				char wlanpath[192];
				memset(wlanpath,0,192);
				printf_d("%s,%d,ptr->d_name:%s\n",__func__,__LINE__,ptr->d_name);
				if(strcmp(".",ptr->d_name)!=0&&strcmp("..",ptr->d_name)!=0)
				{
					WLAN_INFO *wlan_node;
					
					sprintf(wlanpath,"%s/%s/",filepath,ptr->d_name);
					printf_d("get wlaninfo from %s\n",wlanpath); 
					
					CREATE_OBJECT_ERR(wlan_node,WLAN_INFO,{RAISE_ERROR(ERROR_OUT_OF_MEMORY, NULL);APVLog("create memory error\n");});

					if(get_config_from_file(wlanpath,"rid",str))
						wlan_node->rid = atoi(str)+1; 
					printf_d("%s,%d,wlan rid:%d\n",__func__,__LINE__,wlan_node->rid);
					if(get_config_from_file(wlanpath,"wid",str))
						wlan_node->wid = atoi(str); 
					if(get_config_from_file(wlanpath,"vid",str))
						wlan_node->vid = atoi(str); 
					if(get_config_from_file(wlanpath,"data_packet_method",str))
						wlan_node->data_packet_method = atoi(str); 
					if(get_config_from_file(wlanpath,"hide_SSID",str))
						wlan_node->hide_SSID = atoi(str); 
					if(get_config_from_file(wlanpath,"SSID_len",str))
						wlan_node->SSID_len = atoi(str); 
					/* Begin: Modified by dongzw for fix apctl exit and restart bug 2013-04-10 */
					if(get_config_from_file(wlanpath,"SSID",str)){
						CREATE_STRING_ERR(wlan_node->SSID,wlan_node->SSID_len,{RAISE_ERROR(ERROR_OUT_OF_MEMORY, NULL);APVLog("create memory error\n");});
						//memcpy(wlan_node->SSID,str,wlan_node->SSID_len);
						strncpy(wlan_node->SSID, str, wlan_node->SSID_len + 1);
						wlan_node->SSID[wlan_node->SSID_len] = '\0';
					}
					/* End: Modified by dongzw for fix apctl exit and restart bug 2013-04-10 */
					if(get_config_from_file(wlanpath,"max_users",str))
						wlan_node->max_users = atoi(str); 
					if(get_config_from_file(wlanpath,"rx_interval_bytes",str))
						wlan_node->rx_interval_bytes = atoi(str); 
					if(get_config_from_file(wlanpath,"tx_interval_bytes",str))
						wlan_node->tx_interval_bytes = atoi(str); 
					if(get_config_from_file(wlanpath,"BSSID",str)){
						unsigned char *p = wlan_node->BSSID;
						sscanf(str,"%02hhx:%02hhx:%02hhx:%02hhx:%02hhx:%02hhx",&p[0],&p[1],&p[2],&p[3],&p[4],&p[5]);
					}
					if(get_config_from_file(wlanpath,"security",str)){
						wlan_node->security = atoi(str); 
						if(wlan_node->security == SECURITY_802_11i){
							if(get_config_from_file(wlanpath,"sec_policy",str))
								wlan_node->sec_policy = atoi(str); 
							if(wlan_node->sec_policy == WEP){
								if(get_config_from_file(wlanpath,"sec_wep.bits",str))
									wlan_node->sec_wep.bits = atoi(str); 
							}else if(wlan_node->sec_policy == EAP_WPA_WPA2 ||wlan_node->sec_policy == PSK_WPA_WPA2 ){
								if(get_config_from_file(wlanpath,"sec_wpa.mode",str))
									wlan_node->sec_wpa.mode = atoi(str); 
							}
						}

					}
					BackupWlanTableAdd(wlan_node);
					restart_hostapd_for_hot_start(wlan_node);
					printf_d("%s,%d,wlan_node->rid - 1:%d\n",__func__,__LINE__,wlan_node->rid - 1);
					get_wlan_stations(wlan_node->rid - 1,wlan_node->wid);
					create_sta_check_thread();
					printf_d("%s,%d\n",__func__,__LINE__);
				}
				printf_d("%s,%d\n",__func__,__LINE__);
				
			}
			printf_d("%s,%d\n",__func__,__LINE__);
			closedir(dir);
		}
	}

	return TRUE;
}
BOOL add_cmd_to_list_tail(u32 msgtype, void *cmd)
{
	CMD_LST *cmdlst = NULL;
	if(cmd == NULL)
		return FALSE;
	pthread_mutex_lock(&safe_var.rcv_mutex);
	
	//cmdlst = (CMD_LST *)malloc(sizeof(CMD_LST));
	CREATE_OBJECT_ERR(cmdlst,CMD_LST,MALLOC_ERR_RET(FALSE));
	if(cmdlst == NULL)
	{
		FREE_OBJ(cmd);
		pthread_mutex_unlock(&safe_var.rcv_mutex);
		return FALSE;
	}
	cmdlst->next = NULL;
	cmdlst->cmd.cmdmsg = cmd;
	cmdlst->cmd.msgtype = msgtype;
	if(cmd_list_header == NULL)
	{
		cmd_list_header = cmdlst;
		cmd_list_tail = cmd_list_header;
	}
	else
	{
		cmd_list_tail->next = cmdlst;
		cmd_list_tail = cmdlst;
	}
	pthread_cond_signal(&safe_var.rcv_cond);
	pthread_mutex_unlock(&safe_var.rcv_mutex);
	return TRUE;
}
TCMD *get_cmd_from_list_header()
{
	TCMD *cmd = NULL;
	CMD_LST *cmdlst = NULL;
	pthread_mutex_lock(&safe_var.rcv_mutex);

	while(cmd_list_header == NULL)
	{
		pthread_cond_wait(&safe_var.rcv_cond, &safe_var.rcv_mutex);
	}
	if(cmd_list_header->cmd.cmdmsg == NULL)
	{
		cmdlst = cmd_list_header->next;
		FREE_OBJ(cmd_list_header);
		cmd_list_header = cmdlst;
		pthread_mutex_unlock(&safe_var.rcv_mutex);
		return NULL;
	}
	//cmd = (TCMD *)malloc(sizeof(TCMD));
	CREATE_OBJECT_ERR(cmd ,TCMD,MALLOC_ERR_RET(NULL));
	if(cmd == NULL)
	{
		pthread_mutex_unlock(&safe_var.rcv_mutex);
		return NULL;
	}	
	cmd->msgtype = cmd_list_header->cmd.msgtype;
	cmd->cmdmsg = cmd_list_header->cmd.cmdmsg;
	cmdlst = cmd_list_header->next;
	FREE_OBJ(cmd_list_header);
	cmd_list_header = cmdlst;
	pthread_mutex_unlock(&safe_var.rcv_mutex);
	return cmd;
}
BOOL parse_msg(TMSG *msg)
{
	MSG_HEADER msgh;
	
	dprintff(" parse msg ", '=', FORMAT_STR_LEN);
	if(parse_messages_header(msg, &msgh) == FALSE)
	{
		return FALSE;
	}
	if(msg->len != (msgh.len+sizeof(u32)+sizeof(u32)))
	{
		 RAISE_SYS_ERR(ERROR_INVALID_FORMAT);
	}
	/* */
	while(msg->offset < msg->len)
	{
		switch(msgh.type)
		{
			case  MSG_ELEMENT_ADD_WLAN_REQUEST_TYPE:
				{
					printf_d("MSG_ELEMENT_ADD_WLAN_REQUEST_TYPE\n");
					parse_add_wlan_requset_msg(msg);
				}
				break;
			case MSG_ELEMENT_DEL_WLAN_REQUEST_TYPE:
				printf_d("MSG_ELEMENT_DEL_WLAN_REQUEST_TYPE\n");
				parse_del_wlan_requset_msg(msg);
				break;
			case MSG_ELEMENT_ADD_STA_TO_BALCKLIST_REQUEST_TYPE:
				printf_d("MSG_ELEMENT_ADD_STA_TO_BALCKLIST_REQUEST_TYPE\n");
				parse_add_sta_to_blacklist_request_msg(msg);
				break;
			case MSG_ELEMENT_DEL_STA_TO_BALCKLIST_REQUEST_TYPE:
				printf_d("MSG_ELEMENT_DEL_STA_TO_BALCKLIST_REQUEST_TYPE\n");
				parse_del_sta_to_blacklist_request_msg(msg);
				break;
			case MSG_ELEMENT_UPGRADE_REQUEST_TYPE:				
				printf_d("MSG_ELEMENT_UPGRADE_REQUEST_TYPE\n");
				parse_upgrade_request_msg(msg);
				break;
			case MSG_ELEMENT_STA_ASSOCIATION_REQUEST_TYPE:
				printf_d("MSG_ELEMENT_STA_ASSOCIATION_REQUEST_TYPE\n");
				break;
			case MSG_ELEMENT_SET_NTP_REQUEST_TYPE:
				printf_d("MSG_ELEMENT_SET_NTP_REQUEST_TYPE\n");
				parse_set_ntp_request_msg(msg);
				break;
			/* Begin: Add by dongzw for station isolation 2012-11-21 */
			case MSG_ELEMENT_SET_ISOLATION_REQUEST_TYPE:
				printf_d("MSG_ELEMENT_SET_ISOLATION_REQUEST_TYPE\n");
				parse_set_isolation_request_msg(msg);
				break;
			/* End: Add by dongzw for station isolation 2012-11-21 */
			case MSG_ELEMENT_SYS_MAINTENANCE_REQUEST_TYPE:
				printf_d("MSG_ELEMENT_SYS_MAINTENANCE_REQUEST_TYPE\n");
				parse_sys_maintenance_requset_msg(msg);
				break;
			case MSG_ELEMENT_SET_RADIO_REQUEST_TYPE:
				printf_d("MSG_ELEMENT_SET_RADIO_REQUEST_TYPE\n");
				parse_set_radio_requset_msg(msg);
				break;
			case MSG_ELEMENT_SET_STA_STATE_REQUEST_TYPE:
				printf_d("MSG_ELEMENT_SET_STA_STATE_REQUEST_TYPE\n");
				parse_set_sta_state_request_msg(msg);
				break;
			case MSG_ELEMENT_SET_DATA_TUNNEL_REQUEST_TYPE:
				printf_d("MSG_ELEMENT_SET_DATA_TUNNEL_REQUEST_TYPE\n");
				parse_set_data_tunnel_requset_msg(msg);
				break;
			case MSG_ELEMENT_SET_WLAN_INFO_REQUEST_TYPE:
				printf_d("MSG_ELEMENT_SET_WLAN_INFO_REQUEST_TYPE\n");
				parse_set_wlan_info_requset_msg(msg);
				break;
			case MSG_ELEMENT_REPORT_AP_STATE_REQUEST_TYPE:
				printf_d("MSG_ELEMENT_REPORT_AP_STATE_REQUEST_TYPE\n");
				break;
			case MSG_ELEMENT_AP_ALIVE_REQUEST_TYPE:
				printf_d("MSG_ELEMENT_AP_ALIVE_REQUEST_TYPE\n");
				break;
			case MSG_ELEMENT_AP_ALIVE_RESPONSE_TYPE:
				printf_d("MSG_ELEMENT_AP_ALIVE_RESPONSE_TYPE\n");

				parse_echo_response_msg(msg);
				#if 0
				ResetTimers();
				overtime_count = 0;
				#endif
				printf_d("Echo Response received\n");

				
				if(access("/tmp/report_switch", R_OK)!=0)
				{
					ReportEnable = DISABLED;
					break;
				}
				
				char cmd_str[128] = {0};
				char str_tmp[24] = {0};
				int state;
				sprintf(cmd_str, "cat /tmp/report_switch|grep total_report_enable|awk -F \"=\" '{print $2}'");
				popen_fgets(cmd_str, str_tmp, sizeof(str_tmp));
				state = atoi(str_tmp);
				if(state == 1)
					ReportEnable = ENABLED;	
				else
					ReportEnable = DISABLED;
				if(ReportEnable)
				{
					if((counter_monitor == 0) || ((counter_monitor != 0) && (ESRCH == pthread_kill(counter_monitor, 0))))
					{
						if(pthread_create(&counter_monitor, NULL, interval_count_monitor, NULL) != 0)
						{
							printf_d("create monitor thread failed!\n");
							return FALSE;
						}
					}

					int sleepinterval = 180;
					
					sprintf(cmd_str, "cat /tmp/report_switch|grep rrm_report_enable|awk -F \"=\" '{print $2}'");
					popen_fgets(cmd_str, str_tmp, sizeof(str_tmp));
					state = atoi(str_tmp);
					if(state == 1){
						if((thread_apScanning == 0) || ((thread_apScanning != 0) && (ESRCH == pthread_kill(thread_apScanning, 0))))
						{
							if(pthread_create(&thread_apScanning, NULL, GetApScanningInfo, (void *)sleepinterval) != 0)
							{
								APVLog("Error starting Thread that Scanning rogue AP\n");
								printf_d("Error starting Thread that Scanning rogue AP\n");
								return FALSE;
							}
						}					
					}
					
					#if 1
					if((tid_report_ap_state == 0) || ((tid_report_ap_state != 0) && (ESRCH == pthread_kill(tid_report_ap_state, 0))))
					{
						if(pthread_create(&tid_report_ap_state, NULL, thread_report_ap_state, (void *)sleepinterval) != 0)
						{
							printf_d("create report ap state msg thread failed!\n");
							return FALSE;
						}
					}
					//sleepinterval = 120;
					if((tid_report_eth_info == 0) || ((tid_report_eth_info != 0) && (ESRCH == pthread_kill(tid_report_eth_info, 0))))
					{
						if(pthread_create(&tid_report_eth_info, NULL, thread_report_eth_info, (void *)sleepinterval) != 0)
						{
							printf_d("create report eth info msg thread failed!\n");
							return FALSE;
						}
					}
					#endif
				}
				break;
			case MSG_ELEMENT_AP_REPORT_RADIO_INFO_REQUEST_TYPE:
				printf_d("MSG_ELEMENT_AP_REPORT_RADIO_INFO_REQUEST_TYPE\n");
				break;
			case MSG_ELEMENT_AP_REPORT_STA_INFO_REQUEST_TYPE:
				printf_d("MSG_ELEMENT_AP_REPORT_STA_INFO_REQUEST_TYPE\n");
				break;
			case MSG_ELEMENT_AP_REPORT_KEY_INFO_REQUEST_TYPE:
				printf_d("MSG_ELEMENT_AP_REPORT_KEY_INFO_REQUEST_TYPE\n");
				break;
			case MSG_ELEMENT_NOTICE_DISCOVERY_AC_RESULT_REQUEST_TYPE:
				printf_d("MSG_ELEMENT_NOTICE_DISCOVERY_AC_RESULT_REQUEST_TYPE\n");
				parse_notice_discovery_ac_result_requset_msg(msg);
				break;
			case MSG_ELEMENT_SET_RADIUS_REQUEST_TYPE:
				printf_d("MSG_ELEMENT_SET_RADIUS_REQUEST_TYPE\n");
				parse_set_radius_requset_msg(msg);
				break;
			case MSG_ELEMENT_NOTICE_JOIN_AC_RESULT_REQUEST_TYPE1:
				printf_d("MSG_ELEMENT_NOTICE_JOIN_AC_RESULT_REQUEST_TYPE1\n");
				parse_notice_join_ac_result_requset_msg1(msg);
				break;
			case MSG_ELEMENT_NOTICE_JOIN_AC_RESULT_REQUEST_TYPE:
				printf_d("MSG_ELEMENT_NOTICE_JOIN_AC_RESULT_REQUEST_TYPE\n");
				parse_notice_join_ac_result_requset_msg(msg);
				break;
			default:
				printf_d("unrecognised message type\n");
				break;
		}
		msg->offset += msgh.len;
	}
	FREE_PROTOCOL_MESSAGE(*msg);
	return TRUE;
}
BOOL sta_limit(int radio_id)
{
	char cmd[256] = {0};
#ifdef AR11N
	sprintf(cmd,"killall -9 sta_access_limit");
	SYSTEM(cmd);

	memset(cmd,0,256);
	sprintf(cmd,"sta_access_limit &");
	SYSTEM(cmd);
#else
	sprintf(cmd,"killall -9 sta_limit_rid%d ",radio_id);
	SYSTEM(cmd);

	memset(cmd,0,256);
	sprintf(cmd,"sta_limit_rid%d wifi%d &",radio_id,radio_id);
	SYSTEM(cmd);
#endif
	return TRUE;
}
BOOL manage_cmd(TCMD *cmd)
{
/*
	char wifi0mode[10]={0};
	char wifi1mode[10]={0};
	char cmdbuf[128];
*/
	
	if(cmd == NULL)
		return FALSE;
	dprintff(" manage_cmd ", '=', FORMAT_STR_LEN);

	/*
	if (ReportEnable == ENABLED){
		//SYSTEM("killall -9 iwlist_scan > /dev/null 2>&1");
		SYSTEM("killall -STOP iwlist_scan > /dev/null 2>&1");
	}
	*/
	
	switch(cmd->msgtype)
	{
		case  MSG_ELEMENT_ADD_WLAN_REQUEST_TYPE:
			printf_d("MSG_ELEMENT_ADD_WLAN_REQUEST_TYPE\n");
			sta_limit(((WLAN_INFO*)(cmd->cmdmsg))->rid - 1);
			manage_add_wlan_request_msg(cmd->cmdmsg);
			break;
		case MSG_ELEMENT_DEL_WLAN_REQUEST_TYPE:
			printf_d("MSG_ELEMENT_DEL_WLAN_REQUEST_TYPE\n");
			sta_limit(((WLAN_INFO*)(cmd->cmdmsg))->rid - 1);
			manage_del_wlan_request_msg(cmd->cmdmsg);
			break;
		case MSG_ELEMENT_ADD_STA_TO_BALCKLIST_REQUEST_TYPE:
			printf_d("MSG_ELEMENT_ADD_STA_TO_BALCKLIST_REQUEST_TYPE\n");
			manage_add_sta_to_blacklist_request_msg(cmd->cmdmsg);
			break;
		case MSG_ELEMENT_DEL_STA_TO_BALCKLIST_REQUEST_TYPE:
			printf_d("MSG_ELEMENT_DEL_STA_TO_BALCKLIST_REQUEST_TYPE\n");
			manage_del_sta_to_blacklist_request_msg(cmd->cmdmsg);
			break;
		case MSG_ELEMENT_UPGRADE_REQUEST_TYPE:
			printf_d("MSG_ELEMENT_UPGRADE_REQUEST_TYPE\n");
			manage_upgrade_request_msg(cmd->cmdmsg);
			break;
		case MSG_ELEMENT_STA_ASSOCIATION_REQUEST_TYPE:
			printf_d("MSG_ELEMENT_STA_ASSOCIATION_REQUEST_TYPE\n");
			break;
		case MSG_ELEMENT_SET_NTP_REQUEST_TYPE:
			printf_d("MSG_ELEMENT_SET_NTP_REQUEST_TYPE\n");
			manage_set_ntp_request_msg(cmd->cmdmsg);
			break;
		/* Begin: Add by dongzw for station isolation 2012-11-21 */
		case MSG_ELEMENT_SET_ISOLATION_REQUEST_TYPE:
			printf_d("MSG_ELEMENT_SET_ISOLATION_REQUEST_TYPE\n");
			manage_set_isolation_request_msg(cmd->cmdmsg);
			break;
		/* End: Add by dongzw for station isolation 2012-11-21 */
		case MSG_ELEMENT_SYS_MAINTENANCE_REQUEST_TYPE:
			printf_d("MSG_ELEMENT_SYS_MAINTENANCE_REQUEST_TYPE\n");
			manage_sys_maintenance_requset_msg(cmd->cmdmsg);
			break;
		case MSG_ELEMENT_SET_RADIO_REQUEST_TYPE:
			printf_d("MSG_ELEMENT_SET_RADIO_REQUEST_TYPE\n");
			manage_set_radio_requset_msg(cmd->cmdmsg);
			break;
		case MSG_ELEMENT_SET_STA_STATE_REQUEST_TYPE:
			printf_d("MSG_ELEMENT_SET_STA_STATE_REQUEST_TYPE\n");
			manage_set_sta_state_request_msg(cmd->cmdmsg);
			break;
		case MSG_ELEMENT_SET_DATA_TUNNEL_REQUEST_TYPE:
			printf_d("MSG_ELEMENT_SET_DATA_TUNNEL_REQUEST_TYPE\n");
			manage_set_data_tunnel_requset_msg(cmd->cmdmsg);
			break;
		case MSG_ELEMENT_SET_WLAN_INFO_REQUEST_TYPE:
			printf_d("MSG_ELEMENT_SET_WLAN_INFO_REQUEST_TYPE\n");
			manage_set_wlan_info_request_msg(cmd->cmdmsg);
			break;
		case MSG_ELEMENT_REPORT_AP_STATE_REQUEST_TYPE:
			printf_d("MSG_ELEMENT_REPORT_AP_STATE_REQUEST_TYPE\n");
			break;
		case MSG_ELEMENT_AP_ALIVE_RESPONSE_TYPE:
			printf_d("MSG_ELEMENT_AP_ALIVE_RESPONSE_TYPE\n");
			manage_echo_response_msg(cmd->cmdmsg);
			break;
		case MSG_ELEMENT_AP_REPORT_RADIO_INFO_REQUEST_TYPE:
			printf_d("MSG_ELEMENT_AP_REPORT_RADIO_INFO_REQUEST_TYPE\n");
			break;
		case MSG_ELEMENT_AP_REPORT_STA_INFO_REQUEST_TYPE:
			printf_d("MSG_ELEMENT_AP_REPORT_STA_INFO_REQUEST_TYPE\n");
			break;
		case MSG_ELEMENT_AP_REPORT_KEY_INFO_REQUEST_TYPE:
			printf_d("MSG_ELEMENT_AP_REPORT_KEY_INFO_REQUEST_TYPE\n");
			break;
		case MSG_ELEMENT_NOTICE_DISCOVERY_AC_RESULT_REQUEST_TYPE:
			printf_d("MSG_ELEMENT_NOTICE_DISCOVERY_AC_RESULT_REQUEST_TYPE\n");
			manage_notice_discovery_ac_result_requset_msg(cmd->cmdmsg);
			break;
		case MSG_ELEMENT_SET_RADIUS_REQUEST_TYPE:
			printf_d("MSG_ELEMENT_SET_RADIUS_REQUEST_TYPE\n");
			manage_set_radius_resquest_msg(cmd->cmdmsg);
			break;
		case MSG_ELEMENT_NOTICE_JOIN_AC_RESULT_REQUEST_TYPE1:
			printf_d("MSG_ELEMENT_NOTICE_JOIN_AC_RESULT_REQUEST_TYPE1\n");
			manage_notice_join_ac_result_requset_msg1(cmd->cmdmsg);
			break;
		case MSG_ELEMENT_NOTICE_JOIN_AC_RESULT_REQUEST_TYPE:
			printf_d("MSG_ELEMENT_NOTICE_JOIN_AC_RESULT_REQUEST_TYPE\n");
			manage_notice_join_ac_result_requset_msg(cmd->cmdmsg);
			break;
		default:
			printf_d("unrecognised message type\n");
			break;
	}
	FREE_OBJ(cmd);
	
	//if (ReportEnable == ENABLED){
/*
	memset(cmdbuf, 0, 128);
	if((gRadioInfoValue[0].radiotype&0x02)!=0)	//IEEE80211_PARAM_MODE_11a 0x02
		memcpy(wifi0mode,"11a",sizeof("11a"));
	else
		memcpy(wifi0mode,"11bg",sizeof("11bg"));
	if(Check_Interface_State("wifi1")==0)
	{
		
		if((gRadioInfoValue[1].radiotype&0x02)!=0)	//IEEE80211_PARAM_MODE_11a 0x02
			memcpy(wifi1mode,"11a",sizeof("11a"));
		else
			memcpy(wifi1mode,"11bg",sizeof("11bg"));
		sprintf(cmdbuf, "/usr/sbin/iwlist_scan -m %s -c %s -I %d -i %d -n 2 > /dev/null 2>&1 &",wifi0mode,wifi1mode,120,10);
	}
	else 
		sprintf(cmdbuf, "/usr/sbin/iwlist_scan -m %s  -I %d -i %d  > /dev/null 2>&1 &",wifi0mode,120,10);
	SYSTEM(cmdbuf);	
*/
	
	//	SYSTEM("killall -CONT iwlist_scan > /dev/null 2>&1");
	//}
	return TRUE;
}

BOOL circle_recv_msg()
{
	char *buf = NULL;
	int rcvBytes = 0;
	TMSG msg;
	msg.len = 0;
	msg.msg = NULL;
	msg.offset = 0;
	SOCKADDR_STORAGE addr;
	//buf = (char *)malloc(BUFFER_SIZE);
	CREATE_STRING_ERR(buf,BUFFER_SIZE,MALLOC_ERR_RET(FALSE));
	if(buf == NULL)
		return FALSE;
	start_heartbeat_timer();
	while(1)
	{
		memset(buf, 0, BUFFER_SIZE);
		if(network_receive_unsafe(ctl_sd, buf, BUFFER_SIZE, 0, &addr, &rcvBytes) == FALSE)
		{
			continue;
		}
		if(rcvBytes == 0)
			continue;
		print_packet((u8 *)buf, rcvBytes);
		CREATE_PROTOCOL_MESSAGE(msg, rcvBytes, return RAISE_ERROR(ERROR_OUT_OF_MEMORY, NULL););
		msg.len = rcvBytes;
		msg.offset = 0;
		memcpy(msg.msg, buf, rcvBytes);
		parse_msg(&msg);
	}
	FREE_OBJ(buf);
	return TRUE;
}

void *thread_manage_msg(void *arg)
{
	TCMD *cmd = NULL;
	while(1)
	{
		/* */
		cmd = get_cmd_from_list_header();
		if(cmd == NULL)
		{
			continue;
		}
		manage_cmd(cmd);
	}
	return NULL;
}

void *thread_transmit_arg(void *arg)
{
	char cmd[640] = {0};
	int i = 0;
	DISCOVER_AC_METHOD method  = METHOD_OPTION43;
	
	sprintf(cmd, "ap_param_parse -a %s", hostip);
	SYSTEM(cmd);
	memset(cmd, 0 , sizeof(cmd));
	sprintf(cmd, "ap_param_parse -m %02X:%02X:%02X:%02X:%02X:%02X", 
			DEV_MAC[0], DEV_MAC[1], DEV_MAC[2], DEV_MAC[3], DEV_MAC[4], DEV_MAC[5]);
	SYSTEM(cmd);
	memset(cmd, 0 , sizeof(cmd));
	sprintf(cmd, "ap_param_parse -h %s", HARDWARE_VER);
	SYSTEM(cmd);
	memset(cmd, 0 , sizeof(cmd));
	sprintf(cmd, "ap_param_parse -H %s", DEV_TYPE);
	SYSTEM(cmd);
	
	memset(cmd, 0 , sizeof(cmd));
	sprintf(cmd, "ap_param_parse -M \"%s\"", COMPANY);
	SYSTEM(cmd);
	memset(cmd, 0 , sizeof(cmd));
	sprintf(cmd, "ap_param_parse -P %d", VENDOR_ID);
	SYSTEM(cmd);
	memset(cmd, 0 , sizeof(cmd));
	sprintf(cmd, "ap_param_parse -b %s", BOOT_VER);
	SYSTEM(cmd);
	memset(cmd, 0 , sizeof(cmd));
	sprintf(cmd, "ap_param_parse -r %d", RADIO_COUNT);
	SYSTEM(cmd);
	memset(cmd, 0 , sizeof(cmd));
	sprintf(cmd, "ap_param_parse -R %d", RADIO_COUNT);
	SYSTEM(cmd);
	memset(cmd, 0 , sizeof(cmd));
	sprintf(cmd, "ap_param_parse -i %s", SN);
	SYSTEM(cmd);
	/* ---------------------------------------------------- */
	/* need to  communicate */
	for(i = 0; i < RADIO_COUNT; i++)
	{
		memset(cmd, 0 , sizeof(cmd));
		sprintf(cmd, "ap_param_parse -o %d%%0x%02X", i+1, RADIO_MODE[i]);
		SYSTEM(cmd);
	}
	memset(cmd, 0 , sizeof(cmd));
	sprintf(cmd, "ap_param_parse -u %s", SOFTWARE_FEATURES_YARDS);
	SYSTEM(cmd);
	memset(cmd, 0 , sizeof(cmd));
	sprintf(cmd, "ap_param_parse -K %d:%d", 10, 0);
	SYSTEM(cmd);
	/* ---------------------------------------------------- */
	memset(cmd, 0 , sizeof(cmd));
	sprintf(cmd, "ap_param_parse -S %s",  SOFTWARE_VER);
	SYSTEM(cmd);
	memset(cmd, 0 , sizeof(cmd));
	sprintf(cmd, "ap_param_parse -s %s",  JoinAcReasion);
	SYSTEM(cmd);
	memset(cmd, 0 , sizeof(cmd));

	pthread_detach(pthread_self());

	/* Begin: Modified by dongzw for fix ap discovery ac time is too long 2013-2-16 */
	while(1)
	{
		/* */
		memset(cmd, 0 , sizeof(cmd));
		switch(method)
		{
			case METHOD_OPTION43:
				if(ac_list_option43 != NULL)
				{
					sprintf(cmd, "ap_param_parse -G %d", METHOD_OPTION43-1);
					SYSTEM(cmd);
					sprintf(cmd, "ap_param_parse -A %s",  ac_list_option43);
					SYSTEM(cmd);
					
					sleep(20);
				}
				method = METHOD_DNS;
				break;
			case METHOD_DNS:
				if(ac_list_dns != NULL)
				{
					sprintf(cmd, "ap_param_parse -G %d", METHOD_DNS-1);
					SYSTEM(cmd);
					sprintf(cmd, "ap_param_parse -A %s",  ac_list_dns);
					SYSTEM(cmd);
					
					sleep(20);
				}
				method = METHOD_STATIC;
				break;
			case METHOD_STATIC:
				if(ac_list_static != NULL)
				{
					sprintf(cmd, "ap_param_parse -G %d", METHOD_STATIC-1);
					SYSTEM(cmd);
					sprintf(cmd, "ap_param_parse -A %s",  ac_list_static);
					SYSTEM(cmd);
					
					sleep(20);
				}
				method = METHOD_BOARDCAST;
				break;
			case METHOD_BOARDCAST:
				if(ac_list_broadcast != NULL)
				{
					sprintf(cmd, "ap_param_parse -G %d", METHOD_BOARDCAST-1);
					SYSTEM(cmd);
					sprintf(cmd, "ap_param_parse -A %s",  ac_list_broadcast);
					SYSTEM(cmd);
					
					sleep(20);
				}
				method = METHOD_MULTICAST;
				break;
			case METHOD_MULTICAST:
				method = METHOD_OPTION43;
				break;
			default:
				method = METHOD_OPTION43;
				break;
		}
		//sleep(20);
		if(0 == access("/tmp/zt_state", F_OK))
			break;
	}
	/* End: Modified by dongzw for fix ap discovery ac time is too long 2013-2-16 */

	return NULL;
}

/* Begin: Modified by dongzw for ap/wireless/eth information report time 2012-12-21 */
void *thread_report_eth_info(void *arg)
{
	if(arg!=NULL)
	{
		sleep((int)arg);
	}

	AP_ETH_ABILITY_INFO *ability_info = NULL;
	AP_ETH_STATE_COUNT_INFO *state_count_info = NULL;
	char cmd_str[128] = {0};
	char str_tmp[24] = {0};
	int state = 0;

	while(1) {
		if (eth_report_switch == ENABLED) {
			get_ap_eth_ability_info(&ability_info);
			send_message(assemble_report_ap_eth_ability_request, ability_info);
			if(ability_info != NULL) {
				FREE_OBJ(ability_info->if_info);
				FREE_OBJ(ability_info->if_mac);
			}
			FREE_OBJ(ability_info);
			printf_d("========Report eth ability information!========\n");
			APVLog("========Report eth ability information!========\n");

			eth_report_switch = DISABLED;
		}
	
		if(ReportEnable == DISABLED)
			break;

		sprintf(cmd_str, "cat /tmp/report_switch|grep eth_report_enable|awk -F \"=\" '{print $2}'");
		popen_fgets(cmd_str, str_tmp, sizeof(str_tmp));
		state = atoi(str_tmp);
		if(state == 1)
		{
			get_ap_eth_state_count_info(&state_count_info);
			send_message(assemble_report_ap_eth_state_count_request, state_count_info);
			FREE_OBJ(state_count_info);
		}
		
		sprintf(cmd_str, "cat /tmp/report_switch|grep eth_report_interval|awk -F \"=\" '{print $2}'");
		popen_fgets(cmd_str, str_tmp, sizeof(str_tmp));
		eth_report_interval = atoi(str_tmp);
		if(eth_report_interval == 0)
			eth_report_interval = (int)arg;
		sleep(eth_report_interval);
	}
	APVLog("thread_report_eth_info close\n");
	pthread_exit((void *) 0);
	return NULL;
}
/* End: Modified by dongzw for ap/wireless/eth information report time 2012-12-21 */

/* Begin: Modified by dongzw for ap/wireless/eth information report time 2012-12-21 */
void *thread_report_ap_state(void *arg)
{
	int i = 0;

	if(arg!=NULL)
	{
		sleep((int)arg);
	}

	AP_ABILITY_INFO *ap_ability_info = NULL;
	WIRELESS_ABILITY_INFO *wireless_ability_info = NULL;
	WIRELESS_ABILITY_INFO *wireless_ability_tmp = NULL; 
	AP_STATE_COUNT_INFO *ap_state_count_info = NULL;
	WIRELESS_STATE_COUNT_INFO *wireless_state_count_info = NULL;
	WIRELESS_STATE_COUNT_INFO *wireless_state_count_tmp = NULL;
	STA_STATE_COUNT_INFO *sta_state_count_info = NULL;
	AP_ATH_STATE_COUNT_INFO *ath_state_count_info = NULL;
	AP_ATH_STATE_COUNT_INFO *ath_state_count_info_tmp = NULL;
	char cmd_str[128] = {0};
	char str_tmp[24] = {0};
	int state = 0;
	int report_num = 0;
	int count = 0;
	u32 count_tmp1 = 0;
	u32 count_tmp2 = 0;
	int size = 0;

	while(1) {
		if (ap_report_switch == ENABLED) {
			get_ap_ability_info(&ap_ability_info);
			send_message(assemble_report_ap_ability_request, ap_ability_info);
			if(ap_ability_info != NULL) {
				FREE_OBJ(ap_ability_info->net_code);
				FREE_OBJ(ap_ability_info->CPU_INFO);
				FREE_OBJ(ap_ability_info->MEM_TYPE);
			}
			FREE_OBJ(ap_ability_info);
			printf_d("========Report ap ability information!========\n");
			APVLog("========Report ap ability information!========\n");

			get_wireless_ability_info(&wireless_ability_info);
			if(wireless_ability_info != NULL) {
				send_message(assemble_report_wireless_ability_request, wireless_ability_info);
				for(wireless_ability_tmp = wireless_ability_info->next; wireless_ability_tmp != NULL;wireless_ability_tmp = wireless_ability_tmp->next) {
					FREE_OBJ(wireless_ability_info->if_info);
					FREE_OBJ(wireless_ability_info->if_mac);
					FREE_OBJ(wireless_ability_info);
					wireless_ability_info = wireless_ability_tmp;
				}
				FREE_OBJ(wireless_ability_info->if_info);
				FREE_OBJ(wireless_ability_info->if_mac);
				FREE_OBJ(wireless_ability_info);
			}
			printf_d("========Report wireless ability information!========\n");
			APVLog("========Report wireless ability information!========\n");

			ap_report_switch = DISABLED;
		}
		
		if(ReportEnable == DISABLED)
		{
			system("killall -9 iwlist_scan");
			break;
		}
		
		sprintf(cmd_str, "cat /tmp/report_switch|grep ap_report_enable|awk -F \"=\" '{print $2}'");
		popen_fgets(cmd_str, str_tmp, sizeof(str_tmp));
		state = atoi(str_tmp);
		if(state == 1)
		{
			get_ap_state_count_info(&ap_state_count_info);
			send_message(assemble_report_ap_state_count_request, ap_state_count_info);
			FREE_OBJ(ap_state_count_info);
		}
		
		sprintf(cmd_str, "cat /tmp/report_switch|grep radio_report_enable|awk -F \"=\" '{print $2}'");
		popen_fgets(cmd_str, str_tmp, sizeof(str_tmp));
		state = atoi(str_tmp);
		if(state == 1)
		{
			if(Wlan_ReportEnable == ENABLED)
			{
				get_wireless_state_count_info(&wireless_state_count_info);
				if(wireless_state_count_info != NULL)
				{				
					WIRELESS_STATE_COUNT_INFO *wireless_ptr = NULL;
					count = 0;
					for(wireless_ptr = wireless_state_count_info; wireless_ptr!= NULL; wireless_ptr = wireless_ptr->next)
						count++;
					size = 9*22+6*10+13*2+5+64;
					wireless_state_count_info->count = count;
					report_num = (MAX_PACKET_SIZE-4)/size;
					report_num = 1;
					if(count > report_num)
					{
						count_tmp1 = count;
						count_tmp2 = count;
						wireless_ptr = wireless_state_count_info;
						
						while(count_tmp2 > report_num)
						{				
							wireless_ptr->count = report_num;
							send_message(assemble_report_wireless_state_count_request, wireless_ptr);
							for(i = report_num; i > 0; i--)
								wireless_ptr = wireless_ptr->next;
							count_tmp2 = count_tmp2 - report_num;
						}
						wireless_ptr->count = count_tmp2;
						send_message(assemble_report_wireless_state_count_request, wireless_ptr);
					}
					else
						send_message(assemble_report_wireless_state_count_request, wireless_state_count_info);
					for(wireless_state_count_tmp = wireless_state_count_info->next; wireless_state_count_tmp != NULL;wireless_state_count_tmp = wireless_state_count_tmp->next)
					{
						FREE_OBJ(wireless_state_count_info->SSID_list_around);//diaowq add@20120929
						FREE_OBJ(wireless_state_count_info->current_rate);
						FREE_OBJ(wireless_state_count_info);
						wireless_state_count_info = wireless_state_count_tmp;
					}
					FREE_OBJ(wireless_state_count_info->SSID_list_around);//diaowq add@20120929
					FREE_OBJ(wireless_state_count_info->current_rate);
					FREE_OBJ(wireless_state_count_info);
				}
			}
		}

		sprintf(cmd_str, "cat /tmp/report_switch|grep sta_report_enable|awk -F \"=\" '{print $2}'");
		popen_fgets(cmd_str, str_tmp, sizeof(str_tmp));
		state = atoi(str_tmp);
		if(state == 1)
		{
			if(Sta_ReportEnable == ENABLED)
			{
				get_sta_state_count_info(&sta_state_count_info);
				if(sta_state_count_info != NULL)
				{
					size = 11*3+14*7+18*2;
					report_num = (MAX_PACKET_SIZE-4)/size;
					report_num = 1;
					if(sta_state_count_info->count > report_num)/* may lead to message divided */
					{
						count_tmp1 = sta_state_count_info->count;
						count_tmp2 = sta_state_count_info->count;
						while(count_tmp2 > report_num)
						{
							sta_state_count_info[count_tmp1-count_tmp2].count = report_num;
							send_message(assemble_report_sta_count_request, &(sta_state_count_info[count_tmp1-count_tmp2]));
							count_tmp2 = count_tmp2 - report_num;
						}
						sta_state_count_info[count_tmp1-count_tmp2].count = count_tmp2;
						send_message(assemble_report_sta_count_request, &(sta_state_count_info[count_tmp1-count_tmp2]));
					}
					else
						send_message(assemble_report_sta_count_request, sta_state_count_info);
					for(i = 0; i < sta_state_count_info->count; i++)
					{
						FREE_OBJ(sta_state_count_info[i].mac);
					}
					FREE_OBJ(sta_state_count_info);
				}
			}
		}
		
		sprintf(cmd_str, "cat /tmp/report_switch|grep ath_report_enable|awk -F \"=\" '{print $2}'");
		popen_fgets(cmd_str, str_tmp, sizeof(str_tmp));
		state = atoi(str_tmp);
		if(state == 1)
		{
			if(Wlan_ReportEnable == ENABLED)
			{
				get_ap_ath_state_count_info(&ath_state_count_info);
				if(ath_state_count_info != NULL)
				{
					AP_ATH_STATE_COUNT_INFO *ath_ptr = NULL;
					count = 0;
					for(ath_ptr = ath_state_count_info; ath_ptr != NULL; ath_ptr = ath_ptr->next)
						count++;
					ath_state_count_info->count = count;
					size = 6+64+10*7+7*7+14*2+12;
					report_num = (MAX_PACKET_SIZE-4)/size;
					report_num = 1;
					if(count > report_num)
					{
						count_tmp1 = count;
						count_tmp2 = count;
						ath_ptr = ath_state_count_info;
						while(count_tmp2 > report_num)
						{				
							ath_ptr->count = report_num;
							send_message(assemble_report_ap_ath_state_count_request, ath_ptr);
							for(i = report_num; i > 0; i--)
								ath_ptr = ath_ptr->next;
							count_tmp2 = count_tmp2 - report_num;
						}
						ath_ptr->count = count_tmp2;
						send_message(assemble_report_ap_ath_state_count_request, ath_ptr);
					}
					else
						send_message(assemble_report_ap_ath_state_count_request, ath_state_count_info);
					for(ath_state_count_info_tmp = ath_state_count_info->next; ath_state_count_info_tmp != NULL;ath_state_count_info_tmp = ath_state_count_info_tmp->next)
					{
						FREE_OBJ(ath_state_count_info->SSID);
						FREE_OBJ(ath_state_count_info);
						ath_state_count_info = ath_state_count_info_tmp;
					}
					FREE_OBJ(ath_state_count_info->SSID);
					FREE_OBJ(ath_state_count_info);
				}
			}
		}

		sprintf(cmd_str, "cat /tmp/report_switch|grep ap_report_interval|awk -F \"=\" '{print $2}'");
		popen_fgets(cmd_str, str_tmp, sizeof(str_tmp));
		ap_report_interval = atoi(str_tmp);
		if(ap_report_interval == 0)
			ap_report_interval = (int)arg;
		sleep(ap_report_interval);
	}
	APVLog("thread_report_ap_state close\n");
	pthread_exit((void *) 0);
	return NULL;
}
/* End: Modified by dongzw for ap/wireless/eth information report time 2012-12-21 */

BOOL init_config_variables()
{
	apconf_count = 11;

	CREATE_ARRAY_ERR(apconf, apconf_count, AP_CONFIG, return RAISE_ERROR(ERROR_OUT_OF_MEMORY, NULL););
	
	apconf[0].type = T_STRING_ARRAY;
	apconf[0].code = "<AC_ADDRESSES>";
	apconf[0].endCode = "</AC_ADDRESSES>";
	apconf[0].value.str_array_value = NULL;
	apconf[0].count = 0;
	
	apconf[1].type = T_INTEGER;
	apconf[1].code = "</WTP_FORCE_MTU> ";
	apconf[1].value.int_value = 0;
	
	apconf[2].type = T_STRING;
	apconf[2].code = "</WTP_LEV3_PROTOCOL> ";
	apconf[2].value.str_value = NULL;
	
	apconf[3].type = T_STRING;
	apconf[3].code = "</WTP_NAME> ";
	apconf[3].value.str_value = NULL;
	
	apconf[4].type = T_STRING;
	apconf[4].code = "</WTP_LOCATION> ";
	apconf[4].value.str_value = NULL;
	
	apconf[5].type = T_STRING;
	apconf[5].code = "</WTP_FORCE_AC_ADDRESS> ";
	apconf[5].value.str_value = NULL;
	
	apconf[6].type = T_STRING;
	apconf[6].code = "</WTP_FORCE_SECURITY> ";
	apconf[6].value.str_value = NULL;

	apconf[7].type = T_INTEGER;
	apconf[7].code = "</AC_LOG_FILE_ENABLE> ";
	apconf[7].value.int_value = 0;

	apconf[8].type = T_INTEGER;
	apconf[8].code = "</AC_LOG_FILE_SIZE> ";
	apconf[8].value.int_value = DEFAULT_LOG_SIZE;

	apconf[9].type = T_STRING;
	apconf[9].code = "</WTP_AC_DOMAIN_NAME> ";
	apconf[9].value.str_value = NULL;

	apconf[10].type = T_STRING;
	apconf[10].code = "</WTP_SOFTWARE_VERSION> ";
	apconf[10].value.str_value = NULL;
	
	return TRUE;
}
//get one "useful" (not a comment, not blank) line from the config file
char *get_config_file_content(FILE *configFile) 
{
	char *buff = NULL;
	char *command = NULL;
	char *ret = NULL;
	CREATE_STRING_ERR(buff, MAX_BUF_SIZE, return NULL;);
	
	while (((ret = fgetsc(buff, MAX_BUF_SIZE, configFile)) != NULL) && 
		   ((buff[0] == '\n') || (buff[0] == '\r') || (buff[0] == '#')))
	{
		//printf_d("the config file is error! ++buff[0]=%c++\n", buff[0]);
		//sleep(1); // skip comments and blank lines
	}
	
	if((buff != NULL) && (ret != NULL)) 
	{
		int len = strlen(buff);
		buff[len-1] = '\0'; // remove new line
		
		CREATE_STRING_ERR(command, len, return NULL;);
		memset(command,0, len);  //sdl fix a bug for pc-apv6
		strcpy(command, buff);
	}
	
	FREE_OBJ(buff);
	
	return command;
}

BOOL parase_config_file_get_values()
{
	char *content = NULL;
	char *value = NULL;
	int i = 0, j = 0;
	FILE *fp = NULL;
	
	for(i = 0; i < apconf_count; i++) 
	{
		if(apconf[i].type == T_STRING_ARRAY) 
		{
			/* avoid to allocate 0 bytes */  /* according gzm 0817 */
			if (apconf[i].count) 
			{
				CREATE_ARRAY_ERR((apconf[i].value.str_array_value), apconf[i].count, char*, return RAISE_ERROR(ERROR_OUT_OF_MEMORY, NULL););
			}
		}
	}
	/* */
	fp = fopen(CONFIG_FILE, "rb");
	if(fp == NULL)
	{
		RAISE_SYS_ERR(ERROR_GENERAL);
	}

	while((content = get_config_file_content(fp)) != NULL)
	{		
		for(i = 0; i < apconf_count; i++) 
		{
			if(!strncmp(content, apconf[i].code, strlen(apconf[i].code))) 
			{
				value = content + strlen(apconf[i].code);
				
				switch(apconf[i].type) 
				{
					case T_INTEGER:
						apconf[i].value.int_value = atoi(value);
						break;
					case T_STRING:
						/*
						 * BUG - LE02
						 * If this function was called just to count ACAddresses and
						 * paths, we MUST NOT allocate a string value; the actual allocation
						 * will be performed when the function is called with the isCount
						 * argument = CW_FALSE.  
						 *
						 * 19/10/2009 - Donato Capitella
						 */
						CREATE_STRING_FROM_STRING_ERR(apconf[i].value.str_value, value, return RAISE_ERROR(ERROR_OUT_OF_MEMORY, NULL););
						break;
					case T_STRING_ARRAY:
						FREE_OBJ(content);
						while((content = get_config_file_content(fp)) != NULL && strcmp(content, apconf[i].endCode)) 
						{
							CREATE_STRING_FROM_STRING_ERR((apconf[i].value.str_array_value)[j], content, return RAISE_ERROR(ERROR_OUT_OF_MEMORY, NULL););
							j++;
							FREE_OBJ(content);
						}
						break;
				}
				break;
			}
		}		
		FREE_OBJ(content);
	}
	printf_d("Config File Parsed get value\n");
	fclose(fp);
	
	return TRUE;	
}
BOOL parase_config_file_get_count()
{
	char *content = NULL;
	char *value = NULL;
	int i = 0;
	FILE *fp = NULL;
	
	for(i = 0; i < apconf_count; i++) 
	{
		if(apconf[i].type == T_STRING_ARRAY) 
		{
			apconf[i].count = 0;
		}
	}
	fp = fopen (CONFIG_FILE, "rb");
	if(fp == NULL)
	{
		RAISE_SYS_ERR(ERROR_GENERAL);
	}
	while((content = get_config_file_content(fp)) != NULL) 
	{
		for(i = 0; i < apconf_count; i++) 
		{
			if(!strncmp(content, apconf[i].code, strlen(apconf[i].code))) 
			{
				value = content + strlen(apconf[i].code);
				switch(apconf[i].type) 
				{
					case T_INTEGER:
						//apconf[i].value.int_value = atoi(value);
						break;
					case T_STRING:
						/*
						 * BUG - LE02
						 * If this function was called just to count ACAddresses and
						 * paths, we MUST NOT allocate a string value; the actual allocation
						 * will be performed when the function is called with the isCount
						 * argument = CW_FALSE.  
						 *
						 * 19/10/2009 - Donato Capitella
						 */
						break;
					case T_STRING_ARRAY:
						FREE_OBJ(content);
						while((content = get_config_file_content(fp)) != NULL && strcmp(content, apconf[i].endCode)) 
						{
							apconf[i].count++;
							FREE_OBJ(content);
						}
						break;
				}
				
				break;
			}
		}

		FREE_OBJ(content);
	}
	
	printf_d("Config File Parsed get count\n");

	fclose(fp);
	
	return TRUE;
}
BOOL save_config_from_config_file() 
{
	int  i = 0;
	// save the preferences we read
	CREATE_ARRAY_ERR(ac_addr, apconf[0].count, char*, return RAISE_ERROR(ERROR_OUT_OF_MEMORY, NULL););
	for(i = 0; i < apconf[0].count; i++) 
	{
		CREATE_STRING_FROM_STRING_ERR(ac_addr[i], (apconf[0].value.str_array_value)[i], return RAISE_ERROR(ERROR_OUT_OF_MEMORY, NULL););
	}
	ac_count_static = apconf[0].count;
	if(apconf[9].value.str_value != NULL) 
	{
		CREATE_STRING_FROM_STRING_ERR(domain, (apconf[9].value.str_value), return RAISE_ERROR(ERROR_OUT_OF_MEMORY, NULL););
		printf_d("DNS DOMAIN NAME: %s\n", domain);
	}
	
	log_enable = apconf[7].value.int_value;
	tmp_log_max    = apconf[8].value.int_value;

	for(i = 0; i < apconf_count; i++) 
	{
		if(apconf[i].type == T_STRING) 
		{
			FREE_OBJ(apconf[i].value.str_value);
		} 
		else if(apconf[i].type == T_STRING_ARRAY) 
		{
			FREE_OBJS_ARRAY((apconf[i].value.str_array_value), apconf[i].count);
		}
	}

	FREE_OBJ(apconf);
	
	return TRUE;
}

BOOL parase_config_file()
{
	if(!(init_config_variables())) 
		return FALSE;
	if(parase_config_file_get_count() == FALSE)
		return FALSE;	
	if(parase_config_file_get_values() == FALSE) 
		return FALSE;
	return save_config_from_config_file();
}
int get_ip_method()
{
	char cmd[128] = {0};
	char str[32]  = {0};
	
	memset(cmd, 0, 128);
	sprintf(cmd, "awk -F \"[<>]\" '/proto/ {print $3}' /jffs/config.xml");
	if(popen_fgets(cmd, str, sizeof(str)) == NULL)
		return -1;
	
	if(0 == strcmp(str, "dhcp"))
		return 1;

	return 0;
}
BOOL get_ac_addr_by_dns_ipv4(char *domainanme, char ***DNSACAddr, int *DNSACCount)
{
	struct hostent *host = NULL;
	char **h_addr_list = NULL;
	int i = 0;
	char charip[32] = {0};
	char str[32] = {0};

	if(domainanme == NULL)
		return FALSE;
	
	host = gethostbyname(domainanme);
	if(host==NULL) 
	{	
		printf_d("DNS No Information found\n");
	}
	else
	{
		printf_d("host find, the hostname: %s\n", host->h_name);
		APVLog("host find, the hostname: %s\n", host->h_name);
		h_addr_list = host->h_addr_list;
		for (i = 0; h_addr_list[i]; i++)
		{
			(*DNSACCount)++;
		}
		if(*DNSACCount < 1)
			return FALSE;
		
		CREATE_ARRAY_ERR(*DNSACAddr, *DNSACCount, char*, return RAISE_ERROR(ERROR_OUT_OF_MEMORY, NULL););
		
		for (i = 0;i<*DNSACCount; i++)
		{
			memset(str, 0, 32);
			sprintf(charip, "%s", inet_ntop(host->h_addrtype, h_addr_list[i], str, sizeof(str)));
			APVLog("dns-ACAddress:%s\n", charip);
			printf_d("dns-ACAddress:%s\n", charip);
			CREATE_STRING_FROM_STRING_ERR((*DNSACAddr)[i], charip, return RAISE_ERROR(ERROR_OUT_OF_MEMORY, NULL););
		}		
	}
	return TRUE;
}
int get_ac_ip_addr_count_of_dhcp_option43(char *filename)
{
	FILE *ip_fp = NULL;
	int i = 0;
	char tempIp[64] = {0};
	if(filename == NULL)
		return 0;
	if(0 == access(filename, F_OK))
	{
		if((ip_fp = fopen(filename, "r")) == NULL)
		{
			APVLog("fopen Tmp/Ip file is wrong or not exist!\n");
			printf_d("fopen Tmp/Ip file is wrong or not exist!\n");
			return 0;
		}
		memset(tempIp, 0, 64);
		while(fgets(tempIp,sizeof(tempIp),ip_fp))
		{
			i++;
		}
		fclose(ip_fp);
	}
	else
		return 0;
	
	return i;
}

BOOL get_ac_addr_by_dhcp_option43(char *filename, char ***DHCPACAddr, int *DHCPACCount)
{
	int i = 0;
	char charip[64] = {0};
	FILE *ip_fp = NULL;
	*DHCPACCount = get_ac_ip_addr_count_of_dhcp_option43(filename);
	if(*DHCPACCount > 0)
	{
		CREATE_ARRAY_ERR(*DHCPACAddr, *DHCPACCount, char*, return RAISE_ERROR(ERROR_OUT_OF_MEMORY, NULL););

		if((ip_fp = fopen(filename, "r")) == NULL)
		{
			APVLog("fopen Tmp/Ip file is wrong or not exist!\n");
 			return FALSE;
		}
		i = 0;
		memset(charip, 0, 64);
		while(fgets(charip, sizeof(charip), ip_fp))
		{
			if(charip[strlen(charip)-1] == '\n')
			{
				charip[strlen(charip)-1] = '\0';
			}
			if(strcmp(charip, "255.255.255.255") == 0)
				continue;
			CREATE_STRING_FROM_STRING_ERR((*DHCPACAddr)[i], charip, return RAISE_ERROR(ERROR_OUT_OF_MEMORY, NULL););
			memset(charip, 0, 64);
			i++;
		}
		fclose(ip_fp);
	}
	else
		return FALSE;
	
	return TRUE;
}
BOOL get_ac_addr_by_config_file(char ***StaticACAddr_IPv4, int *StaticACCount_IPv4)
{
	int i = 0, k = 0;

	*StaticACCount_IPv4 = ac_count_static;
	if(ac_count_static > 0)
	{
		CREATE_ARRAY_ERR(*StaticACAddr_IPv4, *StaticACCount_IPv4, char*, return RAISE_ERROR(ERROR_OUT_OF_MEMORY, NULL););
		
		for(i = 0; i < ac_count_static; i++)
		{
			if(strcmp(ac_addr[i], "255.255.255.255") == 0)
				*StaticACCount_IPv4 -= 1;
			else
			{
				CREATE_STRING_FROM_STRING_ERR((*StaticACAddr_IPv4)[k], ac_addr[i], return RAISE_ERROR(ERROR_OUT_OF_MEMORY, NULL););
				k++;
				
			}
		}
	}
	else
		return FALSE;
	
	return TRUE;
}
BOOL get_boardcast_addr(char *bcast)
{
	char cmd[128];
	char str[24];
	char str_tmp[24];

	memset(cmd, 0, 128);
	sprintf(cmd, "/sbin/ifconfig default | awk -F \" \" '/Bcast/ {print $3}' | awk -F \":\" '{print $2}'");
	if(popen_fgets(cmd,str_tmp,sizeof(str_tmp)) == NULL)
		return FALSE;
	
	memset(str, 0, 24);
	strcpy(str, trim(str_tmp));
	strcpy(bcast, str);

	return TRUE;
}

BOOL load_configuration()
{
	char **DNSACAddr_IPv4 = NULL;
	char **DHCPACAddr_IPv4 = NULL;
	char **StaticACAddr_IPv4 = NULL;
	int DNSACCount_IPv4 = 0;
	int DHCPACCount_IPv4 = 0;
	int StaticACCount_IPv4 = 0;
	int i = 0, n = 0;
	char Bcast[64] = {0};
	
	if(get_ip_method() == TRUE)
	{
		if(!isBackup)
		{
			while(1)
			{
				if(0 == access("/tmp/dhcpflag", F_OK))
				{
					break;
				}
				else
				{
					printf_d("Dhcp hasn't return!Waiting...\n");
					sleep(2);
				}
			}
		}
	}
	if(get_ac_addr_by_dhcp_option43(DHCP_OPTION43_AC_ADDR_FILE, &DHCPACAddr_IPv4, &DHCPACCount_IPv4) == TRUE)
	{
		ac_count_opt43 = DHCPACCount_IPv4;
	}
	if(get_ac_addr_by_dns_ipv4(domain, &DNSACAddr_IPv4, &DNSACCount_IPv4) == TRUE)
	{
		ac_count_dns = DNSACCount_IPv4;
	}
	if(get_ac_addr_by_config_file(&StaticACAddr_IPv4, &StaticACCount_IPv4) == TRUE)
	{
		ac_count_static = StaticACCount_IPv4;
	}
	APVLog("Init configuration  get AC amount:  %d\n", ac_count_opt43+ac_count_dns+ac_count_static+2);
	printf_d("Init configuration get AC amount:  %d\n", ac_count_opt43+ac_count_dns+ac_count_static+2);
	printf_d("================================================================\n");
	APVLog("================================================================\n");
	printf_d("%-4s  %-8s  %-4s %-8s %-32s\n", "NO.", "FAMILY", "PRI", "TYPE",  "IP ADDRESS");
	APVLog("%-4s  %-8s  %-4s %-8s %-32s\n", "NO.", "FAMILY", "PRI", "TYPE",  "IP ADDRESS");
	if(DHCPACAddr_IPv4 != NULL)
	{
		get_ac_addr(&ac_list_option43, DHCPACCount_IPv4, DHCPACAddr_IPv4);
		for(i = 0; i < DHCPACCount_IPv4; i++)
		{
			printf_d("%-4d  %-8s  %-4s %-8s %-32s\n", n+i, "IPv4", "0", "DHCPv4", DHCPACAddr_IPv4[i]);
			APVLog("%-4d  %-8s  %-4s %-8s %-32s\n", n+i, "IPv4", "0", "DHCPv4", DHCPACAddr_IPv4[i]);
			FREE_OBJ(DHCPACAddr_IPv4[i]);
		}
		FREE_OBJ(DHCPACAddr_IPv4);
		n += DHCPACCount_IPv4;
	}
	if(DNSACAddr_IPv4 != NULL)
	{
		get_ac_addr(&ac_list_dns, DNSACCount_IPv4, DNSACAddr_IPv4);
		for(i = 0; i < DNSACCount_IPv4; i++)
		{
			printf_d("%-4d  %-8s  %-4s %-8s %-32s\n", n+i, "IPv4", "0", "DNSv4", DNSACAddr_IPv4[i]);
			APVLog("%-4d  %-8s  %-4s %-8s %-32s\n", n+i, "IPv4", "0", "DNSv4", DNSACAddr_IPv4[i]);
			FREE_OBJ(DNSACAddr_IPv4[i]);
		}
		FREE_OBJ(DNSACAddr_IPv4);
		n += DNSACCount_IPv4;
	}
	if(StaticACAddr_IPv4 != NULL)
	{
		get_ac_addr(&ac_list_static, StaticACCount_IPv4, StaticACAddr_IPv4);
		for(i = 0; i < StaticACCount_IPv4; i++)
		{
			printf_d("%-4d  %-8s  %-4s %-8s %-32s\n", n+i, "IPv4", "0", "STATIC", StaticACAddr_IPv4[i]);
			APVLog("%-4d  %-8s  %-4s %-8s %-32s\n", n+i, "IPv4", "0", "STATIC", StaticACAddr_IPv4[i]);
			FREE_OBJ(StaticACAddr_IPv4[i]);
		}
		FREE_OBJ(StaticACAddr_IPv4);
		n += StaticACCount_IPv4;
	}
	if(get_boardcast_addr(Bcast) == TRUE)
	{
		printf_d("%-4d  %-8s  %-4s %-8s %-32s\n", n, "IPv4", "0", "LBCAST", Bcast);
		APVLog("%-4d  %-8s  %-4s %-8s %-32s\n", n, "IPv4", "0", "LBCAST", Bcast);
		ac_count_broadcast = 2;
		strcat(Bcast, "%");
	}
	else
		ac_count_broadcast = 1;
	n += 1;
	printf_d("%-4d  %-8s  %-4s %-8s %-32s\n", n, "IPv4", "0", "GBCAST", "255.255.255.255");
	APVLog("%-4d  %-8s  %-4s %-8s %-32s\n", n, "IPv4", "0", "GBCAST", "255.255.255.255");
	strcat(Bcast, "255.255.255.255");
	//ac_list_broadcast = (char *)malloc(strlen(Bcast)+1);
	CREATE_STRING_ERR(ac_list_broadcast,strlen(Bcast),MALLOC_ERR_RET(FALSE));
	strcpy(ac_list_broadcast, Bcast);
	return TRUE;
}
BOOL get_wlan_name(char *wlanname,unsigned char radioId, unsigned char wlanId)
{
	sprintf(wlanname,"ath.%d-%d", radioId, wlanId);
	return TRUE;
}

BOOL STATableAdd(STA_INFO *sta)
{/*head insert*/
	STA_INFO *ptr = NULL, *psta = NULL;
	BOOL is_exists_sta = FALSE;
	
	//ptr = (STA_INFO *)malloc(sizeof(STA_INFO));
	CREATE_OBJECT_ERR(ptr,STA_INFO,MALLOC_ERR_RET(FALSE));
	if(ptr == NULL)
	{
		return FALSE;
	}
	ptr->next = NULL;
	//ptr->BSSID = malloc(MAC_ADDR_LEN);
	//ptr->SSID = malloc(ptr->ssid_length+1);
	CREATE_ARRAY_ERR(ptr->BSSID,MAC_ADDR_LEN,u8,MALLOC_ERR_RET(FALSE));
	CREATE_STRING_ERR(ptr->SSID,ptr->ssid_length,MALLOC_ERR_RET(FALSE));
	
	ptr->rid = sta->rid;
	ptr->wid = sta->wid;
	//ptr->MAC = malloc(MAC_ADDR_LEN);
	CREATE_ARRAY_ERR(ptr->MAC,MAC_ADDR_LEN,u8,MALLOC_ERR_RET(FALSE));
	memcpy(ptr->MAC, sta->MAC, MAC_ADDR_LEN);
	ptr->PMK = NULL;
	ptr->ip = 0;
	if(sta_list == NULL)
	{
		sta_list = ptr;
		Sta_ReportEnable = ENABLED;
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
#if 0
	CWWTPSta *wtp_sta_node;
	CWWTPSta *ptr = NULL;
	CWWTPSta *oldptr = NULL;
	unsigned char *mac = addSTAValues->mac_addr;
	printf_d("sta table add\n");
	for(ptr = wtp_sta_list;ptr != NULL; oldptr = ptr ,ptr = ptr->next)
	{
		if(macAddrCmp(ptr->sta_mac,addSTAValues->mac_addr) == 1)
		{
			printf_d("sta is already in table\n");
			printf_d("sta(%02x:%02x:%02x:%02x:%02x:%02x) is already in table",mac[0],mac[1],mac[2],mac[3],mac[4],mac[5]);
			return TRUE;
		}
	}
	CREATE_OBJECT_ERR(wtp_sta_node, CWWTPSta, {return raise_err(ERROR_OUT_OF_MEMORY, NULL, NULL, 0);});
	wtp_sta_node->radio_id = addSTAValues->radio_id;
	wtp_sta_node->wlan_id = addSTAValues->wlan_id;
	memcpy(wtp_sta_node->sta_mac,addSTAValues->mac_addr,MAC_ADDR_LEN);
	wtp_sta_node->next = wtp_sta_list;
	wtp_sta_list = wtp_sta_node;
	wtp_sta_count ++;
	printf_d("wtp_sta_count=%d",wtp_sta_count);
	printf_d("add sta(%02x:%02x:%02x:%02x:%02x:%02x) to table successfully.",mac[0],mac[1],mac[2],mac[3],mac[4],mac[5]);
	return TRUE;
#endif
	return TRUE;
}
BOOL get_wlan_stations(unsigned char radioId, unsigned char wlanId)
{
	unsigned char buf[24*1024];
	struct iwreq iwr;
	unsigned char *cp;
	int s, len;
	char ifname[IFNAMSIZ];

	s = socket(AF_INET, SOCK_DGRAM, 0);
	if (s < 0)
	{
		printf_d("bakup sta info: create sock error!\n");
		APVLog("bakup sta info: create sock error!\n");
		return TRUE;
	}

	get_wlan_name(ifname, radioId, wlanId);
	printf_d("%s,%d,wlanname:%s\n",__func__,__LINE__,ifname);
	(void) memset(&iwr, 0, sizeof(iwr));
	(void) strncpy(iwr.ifr_name, ifname, sizeof(iwr.ifr_name));
	iwr.u.data.pointer = (void *) buf;
	iwr.u.data.length = sizeof(buf);
	if (ioctl(s, IEEE80211_IOCTL_STA_INFO, &iwr) < 0)
	{
		printf_d("bakup sta info: ioctl error!\n");
		APVLog("bakup sta info: ioctl error!\n");
		close(s);
		return TRUE;
	}
	len = iwr.u.data.length;
	if (len < sizeof(struct ieee80211req_sta_info))
	{
		printf_d("bakup sta info: get sta info error!\n");
		APVLog("bakup sta info: get sta info error!\n");
		close(s);
		return TRUE;
	}

	cp = buf;
	do {
		struct ieee80211req_sta_info *si;
		u8 *vp;
		STA_INFO addStaValues;
		
		si = (struct ieee80211req_sta_info *)cp;
		vp = (u8 *)(si+1);
		if(si->isi_state & IEEE80211_NODE_AUTH)
		{
			memset(&addStaValues, 0, sizeof(addStaValues));
			addStaValues.rid = radioId +1;
			addStaValues.wid = wlanId;
			CREATE_OBJECT_SIZE_ERR(addStaValues.MAC, MAC_ADDR_LEN, {close(s); raise_err(ERROR_OUT_OF_MEMORY, NULL, NULL, 0);});
			memcpy(addStaValues.MAC, si->isi_macaddr, MAC_ADDR_LEN);
			printf_d("%s,%d,bakup sta("MAC_FMT") to ath.%d-%d.\n",__func__,__LINE__,MAC2STR(addStaValues.MAC),radioId,wlanId);
			STATableAdd(&addStaValues);
			FREE_OBJ(addStaValues.MAC);
		}
		cp += si->isi_len;
		len -= si->isi_len;
	} while (len >= sizeof(struct ieee80211req_sta_info));

	close(s);
	return TRUE;
	
}
BOOL get_ap_is_cold_boot(void)
{
	BOOL isColdBoot = TRUE;
	if(0 == access("/jffs/sysrebootflag", 0))
	{
		isColdBoot = FALSE;
		system("rm /jffs/sysrebootflag");
	}
	
	return isColdBoot;
}
void sigstop_handle(int x) 
{ 
/* Begin: Modified by dongzw for fix apctl exit and restart bug 2013-04-10 */
	SYSTEM("killall -9 dhcpsnooping  >  /dev/null 2>&1 &");
	SYSTEM("killall -9 linkcheck  >  /dev/null 2>&1 &");
	SYSTEM("killall -9 ntpclient >  /dev/null 2>&1 &");
	SYSTEM("killall -9 iwlist_scan >  /dev/null 2>&1 &");
	SYSTEM("killall -9 cpu_util_rate >  /dev/null 2>&1 &");
	SYSTEM("killall -9 hostapd >  /dev/null 2>&1 &");
	SYSTEM("touch /tmp/apctl_flag_v");
	printf("catch signal:%d[SIGINT(2),SIGSEGV(11),SIGTERM(15)]\n",x);
	APVLog("catch signal:%d[SIGINT(2),SIGSEGV(11),SIGTERM(15)]\n",x);
	exit(0);
/* End: Modified by dongzw for fix apctl exit and restart bug 2013-04-10 */
}
/* Begin: Add by dongzw for apctl debug 2013-01-18 */
#define SIG_DEBUG_SWITCH SIGUSR1 + 1

void change_debug_print_status(int sig_num, siginfo_t *info,void *myact)
{
	char str_dev[32] = {0};
	int sigval_int = info->si_int;
	int logon_type_serial = sigval_int >= 10 ? 0: 1;

	sigval_int = sigval_int >= 10 ? sigval_int - 10: sigval_int;

	if (debugf != 0) {
		memset(str_dev, 0, sizeof(str_dev));
		if (logon_type_serial)
			sprintf(str_dev, "/dev/ttyS%d", sigval_int);
		else
			sprintf(str_dev, "/dev/pts/%d", sigval_int);
		freopen(str_dev, "w", stdout);

		printf("apctl get signal to change debug_print status.\n");
		printf("old debugf = %d\n", debugf);
		debugf = 0;
		printf("new debugf = %d\n", debugf);
		printf("debugf switch is closed.\n\n");
	} else {
		memset(str_dev, 0, sizeof(str_dev));
		if (logon_type_serial)
			sprintf(str_dev, "/dev/ttyS%d", sigval_int);
		else
			sprintf(str_dev, "/dev/pts/%d", sigval_int);
		freopen(str_dev, "w", stdout);

		printf("apctl get signal to change debug_print status.\n");
		printf("old debugf = %d\n", debugf);
		debugf = 1;
		printf("new debugf = %d\n", debugf);
		printf("debugf switch is opened,\n\n");
	}
}
/* End: Add by dongzw for apctl debug 2013-01-18 */

/* */
int main(int argc, char *argv[])
{
	memset(hostip, 0, sizeof(hostip));
	memset(loip, 0, sizeof(loip));
	strcpy(loip, "127.0.0.1");
	memset(JoinAcReasion,0,sizeof(JoinAcReasion));
	strcpy(JoinAcReasion,"reboot");
	
	init();

	/* Begin: Add by dongzw for apctl debug 2013-01-18 */
	struct sigaction sig_act;
	sigemptyset(&sig_act.sa_mask);
	sig_act.sa_sigaction = change_debug_print_status;
	sig_act.sa_flags = SA_SIGINFO;
	if(sigaction(SIG_DEBUG_SWITCH, &sig_act, NULL) < 0) {
		printf("install signal failed.\n");
	}
	/* End: Add by dongzw for apctl debug 2013-01-18 */

	if(!isBackup){//hot start no need
		if(parase_config_file() == FALSE)
			return FALSE;
	}
	
	if(get_cmd_line_parameters(argc, argv) == FALSE)
		return FALSE;

	fp=LogFileInit(&ln,LOG_FILE_NAME);
	vfp=LogFileInit(&tmp_ln,TMP_LOG_FILE_NAME);
	message_report_fp=LogFileInit(&message_report_ln,MESSAGE_REPORT_FILE_NAME);	// Add by dongzw for station association message report log 2013-04-09

	if(access("/jffs/report_switch", R_OK) != 0)
	{	
		TouchReportSwitchFile();
	}
	else
	{
/* Bebing: Modified by dongzw for update report_switch file when system upgrade 2013-04-10 */
#if 1
		char cmd_str[128] = {0};
		char str_tmp[32] = {0};
		int line_num = 0;
		sprintf(cmd_str, "cat /jffs/report_switch | grep report_switch_line | awk -F \"=\" '{print $2}'");
		popen_fgets(cmd_str, str_tmp, sizeof(str_tmp));
		line_num = atoi(str_tmp);
		if(line_num != REPORT_SWITCH_LINE)
		{
			SYSTEM("rm /jffs/report_switch");
			TouchReportSwitchFile();
		}
#endif
/* End: Modified by dongzw for update report_switch file when system upgrade 2013-04-10 */
	}
	/* */
	if(isBackup)
	{
		APVLog("apctl hot restart for a time\n");
	}
	else
	{
		SYSTEM("cp /jffs/report_switch /tmp/report_switch");
		if(get_ap_is_cold_boot())
		{
			APLog("THE AP Cold started for a time\n");
			APVLog("THE AP Cold started for a time\n");
		}
		else
		{
			APLog("THE AP Hot started for a time\n");
			APVLog("THE AP Hot started for a time\n");
		}
	}
	dprintff(" bootting apctl ", '#', FORMAT_STR_LEN);
	APVLog("%s\n", get_format_str(" bootting apctl ", '#', FORMAT_STR_LEN));

	//apctl hot start, wo need recover some variables from config file saved before
	if(isBackup && !init_config_from_backup()){
		;//may be need restart ap here.
	}
	
	if(load_configuration() == FALSE)
		return FALSE;
	/* */
	/* init timer thread, use for keep alive and so on */
	if(timer_init() == 0)
		return FALSE;
	
	/* info report need*/
	get_commpany();
	get_software_version();
	get_hardware_version();
	get_radio_count();
	
	if(!isBackup){//hot start no need
		get_software_features_yards();
		get_mac();
		get_dev_type();
		get_boot_version();
		get_radio_mode();
		get_sn();
		//make sure dir of /tmp/conf_apctl is exist, should after get radio_count;
		init_config_backup_dir();
	}
	/* save the date of every SAMPLING_INTERVAL in a COUNT_INTERVAL */
	//interval_count_monitor_info = (INTERVAL_COUNT_MONITOR_INFO *)malloc((COUNT_INTERVAL/SAMPLING_INTERVAL)*sizeof(INTERVAL_COUNT_MONITOR_INFO));
	CREATE_ARRAY_ERR(interval_count_monitor_info, (COUNT_INTERVAL/SAMPLING_INTERVAL), INTERVAL_COUNT_MONITOR_INFO,MALLOC_ERR_RET(FALSE));
	if(get_host_IP(hostip) == FALSE)
	{
		return FALSE;
	}
	if(get_dst_sockaddr_storage(loip, &dst_addr) == FALSE)
	{
		return FALSE;
	}
	
	if(create_socket(&ctl_sd, CLIENT_PORT,loip) == TRUE)
	{		
		pthread_t tid_manage_msg;
		if(!isBackup)
		{
			pthread_t tid_transmit;
			if(pthread_create(&tid_transmit, NULL, thread_transmit_arg, NULL) != 0)
			{
				printf_d("create thread_transmit_arg thread failed!\n");
				return FALSE;
			}
		}
		/* */
		if(pthread_create(&tid_manage_msg, NULL, thread_manage_msg, NULL) != 0)
		{
			printf_d("create thread_manage_msg thread failed!\n");
			return FALSE;
		}
		//if((dhcp_snooping == 0) || ((dhcp_snooping != 0) && (ESRCH == pthread_kill(dhcp_snooping, 0))))
		if(pthread_create(&dhcp_snooping, NULL, CheckMsgFromOtherPro, NULL) != 0)
		{
			printf_d("create dhcp snooping thread failed!\n");
			return FALSE;
		}				
		circle_recv_msg();
		close(ctl_sd);
	}
	CloseLogFile();
	return TRUE;
}
