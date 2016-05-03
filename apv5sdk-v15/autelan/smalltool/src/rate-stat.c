
#include <auteos.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>


#define _PATH_PROCNET_DEV               "/proc/net/dev"
#define _PATH_INTERFACE_RATE         "/tmp/interface-rate"
#define _PATH_USER_RATE         "/tmp/user-rate"
#define isspace(c) ((((c) == ' ') || (((unsigned int)((c) - 9)) <= (13 - 9))))
#define IFNAMSIZ 16
#define SPRINT_MAX_LEN 256
#define USERNUM_OF_ONE_VAP 128
#define NUM_OF_VAP 16
#define NUM_OF_ETH 2

typedef enum {
	CW_FALSE = 0,
	CW_TRUE = 1
} CWBool;
struct user_net_device_stats {
	unsigned long long rx_packets;	/* total packets received       */
	unsigned long long tx_packets;	/* total packets transmitted    */
	unsigned long long rx_bytes;	/* total bytes received         */
	unsigned long long tx_bytes;	/* total bytes transmitted      */
	unsigned long rx_errors;	/* bad packets received         */
	unsigned long tx_errors;	/* packet transmit problems     */
	unsigned long rx_dropped;	/* no space in linux buffers    */
	unsigned long tx_dropped;	/* no space available in linux  */
	unsigned long rx_multicast;	/* multicast packets received   */
	unsigned long rx_compressed;
	unsigned long tx_compressed;
	unsigned long collisions;

	/* detailed rx_errors: */
	unsigned long rx_length_errors;
	unsigned long rx_over_errors;	/* receiver ring buff overflow  */
	unsigned long rx_crc_errors;	/* recved pkt with crc error    */
	unsigned long rx_frame_errors;	/* recv'd frame alignment error */
	unsigned long rx_fifo_errors;	/* recv'r fifo overrun          */
	unsigned long rx_missed_errors;	/* receiver missed packet     */
	/* detailed tx_errors */
	unsigned long tx_aborted_errors;
	unsigned long tx_carrier_errors;
	unsigned long tx_fifo_errors;
	unsigned long tx_heartbeat_errors;
	unsigned long tx_window_errors;
};
typedef struct {
	unsigned char wlanId;
	unsigned int rx_packets;
	unsigned int tx_packets;
	unsigned int rx_dropped;
	unsigned int tx_dropped;
	unsigned long rx_bytes;
	unsigned long tx_bytes;
	unsigned int rx_rate;
	unsigned int tx_rate;
}wlan_stats_info;
typedef struct node{
	unsigned char wlanId;
	unsigned char user_mac[20];
	unsigned char flag;
	unsigned int rx_bytes;
	unsigned int tx_bytes;
	unsigned int rx_rate;
	unsigned int tx_rate;
	//struct node *next;
}user_stats_info;

int g_rateStatInterval = 10;
wlan_stats_info g_wlanStatInfo[NUM_OF_VAP+NUM_OF_ETH];
user_stats_info g_userStatInfo[USERNUM_OF_ONE_VAP*NUM_OF_VAP];

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
	while (isspace(p[namestart]))
		namestart++;
	nameend=namestart;
	while (p[nameend] && p[nameend]!=':' && !isspace(p[nameend]))
		nameend++;
	if (p[nameend]==':') {
		if ((nameend-namestart)<IFNAMSIZ) {
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


#if INT_MAX == LONG_MAX
static const char * const ss_fmt[] = {
	"%n%llu%u%u%u%u%n%n%n%llu%u%u%u%u%u",
	"%llu%llu%u%u%u%u%n%n%llu%llu%u%u%u%u%u",
	"%llu%llu%lu%lu%lu%lu%lu%lu%llu%llu%lu%lu%lu%lu%lu%lu"
//	"%llu%llu%u%u%u%u%u%u%llu%llu%u%u%u%u%u%u"
};
#else
static const char * const ss_fmt[] = {
	"%n%llu%lu%lu%lu%lu%n%n%n%llu%lu%lu%lu%lu%lu",
	"%llu%llu%lu%lu%lu%lu%n%n%llu%llu%lu%lu%lu%lu%lu",
	"%llu%llu%lu%lu%lu%lu%lu%lu%llu%llu%lu%lu%lu%lu%lu%lu"
};
#endif

static void get_dev_fields(char *bp, struct user_net_device_stats *wlan_stats, int procnetdev_vsn)
{
	memset(wlan_stats, 0, sizeof(struct user_net_device_stats));

	autelan_sscanf(bp, ss_fmt[procnetdev_vsn],
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

static int get_wlan_throughput_fields(char *wlanname, wlan_stats_info *wlanStatsInfo)
{
	FILE *fh;
	char buf[512];
	struct user_net_device_stats wlan_stats;
	int procnetdev_vsn;
	char cmdbuf[SPRINT_MAX_LEN];
	unsigned char findwlanflag = 0;

	fh = autelan_fopen(_PATH_PROCNET_DEV, "r");
	if (!fh) {
		autelan_printf("warning: cannot open %s, limiting output", _PATH_PROCNET_DEV);
		return 1;
	}
	autelan_fgets(buf, sizeof buf, fh);	/* eat line */
	autelan_fgets(buf, sizeof buf, fh);

	procnetdev_vsn = procnetdev_version(buf);

	while (autelan_fgets(buf, sizeof buf, fh)) {
		char *s, name[128];

		s = get_name(name, buf);
		if(strstr(buf, wlanname))
		{
			//printf("222name:%s\n", name);
			if(strncmp(name, wlanname, strlen(name)) == 0)
			{
				findwlanflag = 1;
				get_dev_fields(s, &wlan_stats, procnetdev_vsn);
				wlanStatsInfo->rx_rate = (wlan_stats.rx_bytes/1024 - wlanStatsInfo->rx_bytes)*1024/g_rateStatInterval;
				wlanStatsInfo->tx_rate = (wlan_stats.tx_bytes/1024 - wlanStatsInfo->tx_bytes)*1024/g_rateStatInterval;

				if(wlan_stats.rx_packets != 0)
					wlanStatsInfo->rx_rate = wlanStatsInfo->rx_rate-wlanStatsInfo->rx_rate *wlan_stats.rx_dropped/wlan_stats.rx_packets;
				if(wlan_stats.tx_packets != 0)
					wlanStatsInfo->tx_rate = wlanStatsInfo->tx_rate -wlanStatsInfo->tx_rate *wlan_stats.tx_dropped/wlan_stats.tx_packets;
				
				wlanStatsInfo->rx_bytes = wlan_stats.rx_bytes/1024;
				wlanStatsInfo->tx_bytes = wlan_stats.tx_bytes/1024;

/*
				if((wlan_stats.rx_packets-wlanStatsInfo->rx_packets) != 0)
				{
					printf("111rx_rate:%d   rx_drop_ratio:%d\n", wlanStatsInfo->rx_rate, ((wlan_stats.rx_dropped-wlanStatsInfo->rx_dropped)*100/(wlan_stats.rx_packets-wlanStatsInfo->rx_packets)));
					wlanStatsInfo->rx_rate = wlanStatsInfo->rx_rate-wlanStatsInfo->rx_rate*(wlan_stats.rx_dropped-wlanStatsInfo->rx_dropped)/(wlan_stats.rx_packets-wlanStatsInfo->rx_packets);
				}
				if((wlan_stats.tx_packets-wlanStatsInfo->tx_packets) != 0)
				{
					printf("222tx_rate:%d   tx_drop_ratio:%d\n", wlanStatsInfo->tx_rate, ((wlan_stats.tx_dropped-wlanStatsInfo->tx_dropped)*100/(wlan_stats.tx_packets-wlanStatsInfo->tx_packets)));
					wlanStatsInfo->tx_rate = wlanStatsInfo->tx_rate -wlanStatsInfo->tx_rate *(wlan_stats.tx_dropped-wlanStatsInfo->tx_dropped)/(wlan_stats.tx_packets-wlanStatsInfo->tx_packets);
				}
				wlanStatsInfo->rx_packets = wlan_stats.rx_packets;
				wlanStatsInfo->tx_packets = wlan_stats.tx_packets;
				wlanStatsInfo->rx_dropped = wlan_stats.rx_dropped;
				wlanStatsInfo->tx_dropped = wlan_stats.tx_dropped;
*/
				//sprintf(cmdbuf, "echo \"%s	 %d		   %d\" >> %s", wlanname, wlanStatsInfo->rx_rate, wlanStatsInfo->tx_rate, _PATH_INTERFACE_RATE);
				autelan_sprintf(cmdbuf, "echo \"%s	%d		%d\" >> %s", wlanname, wlanStatsInfo->rx_rate, wlanStatsInfo->tx_rate, _PATH_INTERFACE_RATE);
				autelan_system(cmdbuf);
			}
		}
	}
	autelan_fclose(fh);
	if(findwlanflag == 0)
		return -1;
	
	return 0;
}
int CWGetWlanId(char *wlanname)
{
	int wlan_id = 0;
	wlan_id = atoi(&wlanname[3]);
	return wlan_id;
}
void CWGetUserRate(char *wlanname)
{
	char cmd_n[SPRINT_MAX_LEN];
	char cmd_t[SPRINT_MAX_LEN];
	char cmd_r[SPRINT_MAX_LEN];
	char cmd_mac[SPRINT_MAX_LEN];
	char cmdbuf[SPRINT_MAX_LEN];

	char bfr[SPRINT_MAX_LEN];
	char bytes_str[SPRINT_MAX_LEN];
	int  num_row = 0;
	int user_rx_byte;
	int  i = 0;
	int  j = 0;
	unsigned char wlan_id;
	FILE *fp1;
	FILE *fp2;
	user_stats_info temp;
	
	memset(cmd_n, 0, SPRINT_MAX_LEN);
	autelan_sprintf(cmd_n, "%s %s %s", "80211stats -a -i", wlanname, "|wc -l");
	fp1=autelan_popen(cmd_n, "r");
	if(fp1)
	{
		for(j=wlan_id*USERNUM_OF_ONE_VAP;j<(wlan_id+1)*USERNUM_OF_ONE_VAP;j++)
		{
			g_userStatInfo[j].flag = 0;
		}
		wlan_id = CWGetWlanId(wlanname);
		memset(bfr, 0, SPRINT_MAX_LEN);
		autelan_fgets(bfr, sizeof(bfr), fp1);
		bfr[strlen(bfr)-1] = '\0';
		//printf("bfr:%s\n", bfr);
		num_row=atoi(bfr);
		//printf("num_row:%d\n", num_row);
		for(i=0;i<num_row/4;i++)
		{
			//userStatsInfo = (user_stats_info*)malloc(sizeof(user_stats_info));
			//userStatsInfo->flag = 0;
			temp.flag = 0;
			temp.wlanId = wlan_id;
			temp.rx_rate = 0;
			temp.tx_rate = 0;
			
			memset(cmd_mac, 0, SPRINT_MAX_LEN);
			autelan_sprintf(cmd_mac,"%s %s %s %d %s","80211stats -a -i",wlanname,"|awk 'NR==",i*4+1,"{print $1}'");
			fp2=autelan_popen(cmd_mac, "r");
			if(fp2)
			{
				memset(bytes_str, 0, SPRINT_MAX_LEN );
				autelan_fgets(bytes_str, sizeof(bytes_str), fp2);
				bytes_str[strlen(bytes_str)-2] = '\0';
				//printf("11bytes_str:%s\n", bytes_str);
				memset(temp.user_mac, 0, sizeof(temp.user_mac));
				strcpy(temp.user_mac, bytes_str);
//				printf("user_mac:%s\n", temp.user_mac);
				autelan_pclose(fp2);
			}
			
			memset(cmd_r, 0, SPRINT_MAX_LEN);
			autelan_sprintf(cmd_r,"%s %s %s %d %s","80211stats -a -i",wlanname,"|awk 'NR==",i*4+2,"{print $6}'");
			fp2=autelan_popen(cmd_r, "r");
			if(fp2)
			{
				memset( bytes_str, 0, SPRINT_MAX_LEN);
				autelan_fgets(bytes_str,sizeof(bytes_str),fp2);
				bytes_str[strlen(bytes_str)-1] = '\0';
				//printf("22bytes_str:%s\n", bytes_str);
				temp.rx_bytes = atoi(bytes_str);
				autelan_pclose(fp2);
			}

			memset(cmd_t, 0, SPRINT_MAX_LEN);
			autelan_sprintf(cmd_t,"%s %s %s %d %s","80211stats -a -i",wlanname,"|awk 'NR==",i*4+3,"{print $6}'");
			fp2=autelan_popen(cmd_t,"r");
			if(fp2)
			{
				memset(bytes_str, 0, SPRINT_MAX_LEN );
				autelan_fgets(bytes_str, sizeof(bytes_str), fp2);
				bytes_str[strlen(bytes_str)-1] = '\0';
				//printf("11bytes_str:%s\n", bytes_str);
				temp.tx_bytes = atoi(bytes_str);
				autelan_pclose(fp2);
			}
#if 0
			temp = g_userStatInfo;
			while(temp != NULL)
			{
				temp->flag = 1;
				if(strcmp(temp->user_mac, userStatsInfo->user_mac) == 0)
				{
					temp->flag = 0;
					temp->rx_rate = (userStatsInfo->rx_bytes-temp->rx_bytes)/g_rateStatInterval;
					temp->tx_rate = (userStatsInfo->tx_bytes-temp->tx_bytes)/g_rateStatInterval;
					temp->rx_bytes = userStatsInfo->rx_bytes;
					temp->tx_bytes = userStatsInfo->tx_bytes;
					break;
				}
				temp = temp->next;
			}
			temp = g_userStatInfo;
			while(temp != NULL)
			{
				if(temp->flag == 1)
				{
					if(temp == g_userStatInfo)
					{
						if(temp->next!=NULL)
						{
							g_userStatInfo = g_userStatInfo->next;
							temp->next = NULL;
							free(temp);
						}
						else
						{
							temp->next=NULL;
							free(temp);
							break;
						}
					}
					else
					{
						if(temp->next!=NULL)
						{
							last->next = temp->next;
							temp->next = NULL;
							free(temp);
							temp = last;
						}
						else
						{
							last->next = NULL;
							temp->next = NULL;
							free(temp);
							break;
						}
					}
					
				}
				last = temp;
				temp = temp->next;
			}
#endif
			
			for(j=wlan_id*USERNUM_OF_ONE_VAP;j<(wlan_id+1)*USERNUM_OF_ONE_VAP;j++)
			{
				if(strcmp(temp.user_mac, g_userStatInfo[j].user_mac) == 0)
				{
					temp.flag = 1;
					temp.rx_rate = (temp.rx_bytes - g_userStatInfo[j].rx_bytes)/g_rateStatInterval;
					temp.tx_rate = (temp.tx_bytes - g_userStatInfo[j].tx_bytes)/g_rateStatInterval;
					memcpy(&g_userStatInfo[j], &temp, sizeof(temp));
				}
			}
			if(temp.flag == 0)
			{
				for(j=wlan_id*USERNUM_OF_ONE_VAP;j<(wlan_id+1)*USERNUM_OF_ONE_VAP;j++)
				{
					if((g_userStatInfo[j].flag == 0)&&(strcmp(g_userStatInfo[j].user_mac, "")==0))
					{
						temp.flag = 1;
						memcpy(&g_userStatInfo[j], &temp, sizeof(temp));
					}
				}
			}
			//sprintf(cmdbuf, "echo \"%s  %s	 %d  		    %d\" >> %s", wlanname, temp.user_mac, temp.rx_rate, temp.tx_rate, _PATH_USER_RATE);
			autelan_sprintf(cmdbuf, "echo \"%s	%s	%d		%d\" >> %s", wlanname, temp.user_mac, temp.rx_rate, temp.tx_rate, _PATH_USER_RATE);
			autelan_system(cmdbuf);
		}
		for(j=wlan_id*USERNUM_OF_ONE_VAP;j<(wlan_id+1)*USERNUM_OF_ONE_VAP;j++)
		{
			if(g_userStatInfo[j].flag == 0)
				memset(&g_userStatInfo[j], 0, sizeof(user_stats_info));
		}
	}
	autelan_pclose(fp1);

    return;
}

void *CWGetAPRateStat(void *arg)
{
	int i = 0,k = 0;
	char wlanname[IFNAMSIZ];
	int wlan_id = 0;
	char cmdbuf[SPRINT_MAX_LEN];
	memset(g_userStatInfo, 0, sizeof(user_stats_info)*(USERNUM_OF_ONE_VAP*NUM_OF_VAP));
	memset(g_wlanStatInfo, 0, sizeof(wlan_stats_info)*(NUM_OF_VAP+NUM_OF_ETH));
	
	while(1)
	{
		//sleep(g_rateStatInterval);

		//sprintf(cmdbuf, "echo \"ath/eth  rx-rate(Bytes/s)  tx-rate(Bytes/s)\" > %s", _PATH_INTERFACE_RATE);
		autelan_sprintf(cmdbuf, "echo \"ath/eth	rx-rate(Bytes/s)	tx-rate(Bytes/s)\" > %s", _PATH_INTERFACE_RATE);
		autelan_system(cmdbuf);
		//sprintf(cmdbuf, "echo \"ath   user-mac           rx-rate(Bytes/s)   tx-rate(Bytes/s)\" > %s", _PATH_USER_RATE);
		autelan_sprintf(cmdbuf, "echo \"ath	user-mac		rx-rate(Bytes/s)	tx-rate(Bytes/s)\" > %s", _PATH_USER_RATE);
		autelan_system(cmdbuf);
		
		if(arg != NULL)
		{
			if(strncmp(arg, "ath", 3) == 0)
			{
				memset(wlanname, 0, IFNAMSIZ);
				strcpy(wlanname, arg);
				//printf("wlanname:%s\n", wlanname);

				/* get interface-rate info */
				wlan_id = CWGetWlanId(wlanname);
				get_wlan_throughput_fields(wlanname, &g_wlanStatInfo[wlan_id]);

				/* get user-rate info */
				CWGetUserRate(wlanname);
			}
			else if(strncmp(arg, "eth", 3) == 0)
			{
				memset(wlanname, 0, IFNAMSIZ);
				strcpy(wlanname, arg);
				//printf("wlanname:%s\n", wlanname);

				wlan_id = CWGetWlanId(wlanname);
				get_wlan_throughput_fields(wlanname, &g_wlanStatInfo[wlan_id+NUM_OF_VAP]);
			}
			
		}
		else
		{
			for(i=0;i<NUM_OF_VAP;i++)
			{
				autelan_sprintf(wlanname, "ath%d", i);
				if(-1 == get_wlan_throughput_fields(wlanname, &g_wlanStatInfo[i]))
					continue;
				/* get user-rate info */
				CWGetUserRate(wlanname);
			}
			for(i=0;i<NUM_OF_ETH;i++)
			{
				autelan_sprintf(wlanname, "eth%d", i);
				get_wlan_throughput_fields(wlanname, &g_wlanStatInfo[i+NUM_OF_VAP]);
			}
		}
		autelan_sleep(g_rateStatInterval);
	}
	
}

int main(int argc, char *argv[])
{
	char ifname[10];
	char *wlanname = NULL;
	int interval = 10;
	pthread_t thread_rateStat;
	
	if(argc == 2)
	{
		if((strncmp(argv[1], "ath", 3) == 0)||(strncmp(argv[1], "eth", 3) == 0))
		{
			memset(ifname, 0, 10);
			strcpy(ifname, argv[1]);
			//printf("ifname:%s\n", ifname);
			wlanname = ifname;
		}
		else
		{
			interval = atoi(argv[1]);
			g_rateStatInterval = interval;
		}
	}
	if(argc == 3)
	{
		interval = atoi(argv[1]);
		g_rateStatInterval = interval;
		//printf("interval:%d\n", interval);

		strcpy(ifname, argv[2]);
		//printf("ifname:%s\n", ifname);
		wlanname = ifname;
	}
	if(argc > 3)
	{
		autelan_printf("num of argument invalid\n");
		return -1;
	}
	//printf("create thread...\n");
	if(pthread_create(&thread_rateStat, NULL, CWGetAPRateStat, (void *)wlanname) != 0) {
		autelan_printf("create thread error");
		return -1;
	}
	pthread_join (thread_rateStat, NULL);

	return 0;
}



