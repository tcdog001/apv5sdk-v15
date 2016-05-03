

#define INCLUDE_LINUX_IF_H
#include <auteos.h>
#include "iwlist_scan.h"
static char help_msg[] = "\
Usage: iwlist_scan -b -d [device name] -I [interval every scanning] -i\
[interval scan every channel]\n\
-b\t running in background.\n\
-d\t device used to scan.\n\
-I\t intreval between every scanning.(should be greater than 15, unit: SECOND)\n\
-i\t interval in scanning every channel(should not be ZERO, unit: SECOND)\n\
-m\t the first  radio mode\n\
-c\t the second radio mode\n\
-n\t the radio count\n";

static unsigned int un_interval_scan = 250;
static unsigned int un_interval_chan = 5;
static int channel_num = 0;
static int channel_num_wifi1 = 0;
//static char if_name[DEV_LEN];
static char dev_name[][DEV_LEN]={"wifi0","wifi1"};
static char wifi0mode[10]={0}; 
static char wifi1mode[10]={0}; 
/************ zhouke add ,scan ath name *********/
#ifdef ARM_AF3220
    static char *san_if_name[]={"ath.0-7","ath.1-7"};
#else
    static char *san_if_name[]={"athscanwifi0","athscanwifi1"};
#endif 
/******************** end ***********************/
static S_LIST_HEAD *ap_list_head = NULL;
static S_LIST_HEAD *ap_list_head_wifi1 = NULL;
static pthread_mutex_t pthread_list_metux;
static pthread_mutex_t pthread_whole_scan;
static pthread_mutex_t pthread_list_metux_wifi1;
static pthread_mutex_t pthread_whole_scan_wifi1;

static int sockfd;
int mode = 0 ;
int radio_id = 0;

S_LIST_HEAD* IW_SCAN_LIST_INIT(void)
{
    S_LIST_HEAD *list_head = NULL;
    list_head = (S_LIST_HEAD*)malloc(sizeof(S_LIST_HEAD));
    if( list_head != NULL )
        memset(list_head, 0, sizeof(S_LIST_HEAD));
    return list_head;
}

S_LIST* IW_SCAN_LIST_NODE_CREATE(void)
{
    S_LIST *node = NULL;
    node = (S_LIST *)malloc(sizeof(S_LIST));
    if( node != NULL )
        memset(node, 0, sizeof(S_LIST));
    return node;
}

typedef int VALUE_CMP(S_LIST *, void *);

S_LIST* IW_SCAN_LIST_NODE_FIND( S_LIST_HEAD *list, VALUE_CMP iw_scan_cmp, void 
*value )
{
    S_LIST *iter = list->head.next;
    while( iter != NULL )
    {
        int res = iw_scan_cmp(iter, value);
        if(res != 0){
            break;
        }
        iter = iter->next;
    }
    return iter;
}

S_LIST* IW_SCAN_LIST_NODE_FIND_PRE( S_LIST_HEAD *list, VALUE_CMP iw_scan_cmp, void 
*value )
{
    S_LIST *iter = (S_LIST *)list;
    S_LIST *iter_pre = (S_LIST *)list;
    while( iter->next != NULL )
    {
        iter = iter->next;
        if(!iw_scan_cmp(iter, value))
            break;
        iter_pre = iter;
    }
    return iter_pre;
}


int IW_SCAN_LIST_NODE_ADD( S_LIST_HEAD *list, S_LIST *node )
{
    if( NULL == list || NULL == node )
    {
        return -1;
    }
    node->next = list->head.next;
    list->head.next = node;
    list->node_count ++;
    return list->node_count;
}

S_LIST* IW_SCAN_LIST_NODE_DEL( S_LIST_HEAD *list , S_LIST *pre_node )
{
    S_LIST *node_tmp = NULL;
    if( NULL == list || NULL == pre_node )
    {
        return NULL;
    }
    node_tmp = pre_node->next;
    pre_node->next = node_tmp->next;
    list->node_count--;
    return node_tmp;
}

int IW_SCAN_LIST_NODE_CLEAN( S_LIST_HEAD *list )
{
    int i = list->node_count;
    S_LIST *node_del = NULL;
    for( ; i>0; i--)
    {
        node_del = IW_SCAN_LIST_NODE_DEL( list, &(list->head) );
        if(node_del != NULL)
        {
            free(node_del);
        }
    }

    return 0;
}

/* -------------- */


/* ------ thread ------ */

#define SERV_PORT 5248
#define IW_SCAN_VERSION 0
#define P_VERSION 0
#define P_COUNT 1
#define P_DATA 2
//#define MAX_BUF_SIZE 8192
#define MAX_BUF_SIZE 20*1024


int iw_scan_get_single_list( S_LIST_HEAD *list_head, char *buf, 
                             unsigned int buf_len, unsigned int *node_count )
{
    int i = 0;
    S_LIST *iter_node = list_head->head.next;
    AP_NODE *node = NULL;
    char *iter_buf = buf;
    *node_count = 0;

    if( 0 == list_head->node_count || NULL == iter_node )
    {
        return 0;
    }

    while( iter_node != NULL )
    {
        unsigned short val = 0;
        char *p_start = iter_buf;
        if(buf_len < 100)
            break;
        node = (AP_NODE *)iter_node;
        
        memcpy(iter_buf, &(node->st_ap_stat.st_ap_info.BSSID), SSID_LEN);
        iter_buf += SSID_LEN;
        memcpy(iter_buf, &(node->st_ap_stat.st_ap_info.Channel), 1);
        iter_buf += 1;
        memcpy(iter_buf, &(node->st_ap_stat.st_ap_info.RSSI), 1);
        iter_buf += 1;
        memcpy(iter_buf, &(node->st_ap_stat.st_ap_info.NOISE), 1);
        iter_buf += 1;
        memcpy(iter_buf, &(node->st_ap_stat.st_ap_info.BEACON_INT), 1);
        iter_buf += 1;
        memcpy(iter_buf, &(node->st_ap_stat.st_ap_info.Opstatus), 1);
        iter_buf += 1;
        memcpy(iter_buf, &(node->st_ap_stat.st_ap_info.EssidLen), 1);
        iter_buf += 1;
        memcpy(iter_buf, node->st_ap_stat.st_ap_info.ESSID, node->st_ap_stat.st_ap_info.EssidLen);
        iter_buf += node->st_ap_stat.st_ap_info.EssidLen;
        memcpy(iter_buf, &(node->st_ap_stat.st_ap_info.IeLen), 1);
        iter_buf += 1;
        memcpy(iter_buf, node->st_ap_stat.st_ap_info.IEs_INFO, node->st_ap_stat.st_ap_info.IeLen);
        iter_buf += node->st_ap_stat.st_ap_info.IeLen;
        val = autelan_htons(node->st_ap_stat.st_ap_info.Rate);
        memcpy(iter_buf, &val, sizeof(unsigned short));
        iter_buf += sizeof(unsigned short);
        val = autelan_htons(node->st_ap_stat.st_ap_info.CapabilityInfo);
        memcpy(iter_buf, &val, sizeof(unsigned short)); 
        iter_buf += sizeof(unsigned short);
	memcpy(iter_buf,&(node->st_ap_stat.st_ap_info.radio_id),1); 
	iter_buf += 1;												
        buf_len = buf_len - (iter_buf - p_start); 
        *node_count = *node_count + 1;
        iter_node = iter_node->next;
    }

    return iter_buf-buf;
}

int iw_scan_get_ap_info( char *tmp_buf )
{
    char *iter = 0;
    unsigned int data_len = 0;
    unsigned int list_len = 0;
    unsigned int buf_size = MAX_BUF_SIZE;
    int i = 0;
    int node_count = 0;
    int temp_node_count = 0;
    
    tmp_buf[P_VERSION] = IW_SCAN_VERSION;
    tmp_buf[P_COUNT] = 0;
    data_len += 2;
    iter = tmp_buf+P_DATA;
    buf_size -= data_len;

    /*get ap info from list*/
    pthread_mutex_lock(&pthread_list_metux);
    for(; i<channel_num; i++)
    {	
        list_len = iw_scan_get_single_list( ap_list_head+i, iter, buf_size, &node_count );
	if(tmp_buf[P_COUNT] + node_count > 255)
	{
		autelan_printf("packet data count bigger than 255,break.\n");
		break;
	}
        data_len += list_len;
        iter += list_len;
        buf_size -= list_len;
        tmp_buf[P_COUNT] += node_count;
	temp_node_count  += node_count;
    }
   autelan_printf("wifi0 get count:%d,  Capable buf_size:%d,   Allcount: %d\n",temp_node_count,buf_size,tmp_buf[P_COUNT]);
   pthread_mutex_unlock(&pthread_list_metux);


   if (radio_id>1)
   {	
	   pthread_mutex_lock(&pthread_list_metux_wifi1); 
//	   if ( ap_list_head_wifi1->head.next != NULL )
	   temp_node_count = 0;
	    for(i=0;i<channel_num_wifi1;i++)
		{
	
			list_len = iw_scan_get_single_list( ap_list_head_wifi1+i, iter, buf_size, &node_count );
			if(tmp_buf[P_COUNT] + node_count > 255)
			{
				autelan_printf("packet data count bigger than 255,break.\n");
				break;
			}
			data_len += list_len;
			iter += list_len;
			buf_size -= list_len;
			tmp_buf[P_COUNT] += node_count;
			temp_node_count += node_count;
		}
		autelan_printf("wifi1 Get node Count:%d,   Capable buf_size: %d,   Allcount:%d\n",temp_node_count ,buf_size,tmp_buf[P_COUNT]);
	     pthread_mutex_unlock(&pthread_list_metux_wifi1);
	}
    return data_len;
}

void *iw_scan_ap_info_report(void *arg)
{    
    
     struct sockaddr_in servaddr;
     struct sockaddr_in cliaddr;
     fd_set fd_read;
     int rcvlen = 0;
     int clilen;
     int res = 0;
     int fromlen = sizeof(struct sockaddr_in);
     int on;
     
     char buf[MAX_BUF_SIZE];

     pthread_detach(pthread_self());

     memset( buf, 0, MAX_BUF_SIZE );     
     if( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 )
     {
         perror("create socket error");
         exit(1);
     }
     
     memset( &servaddr, 0, sizeof(servaddr) );
     servaddr.sin_family = AF_INET;
     servaddr.sin_addr.s_addr = autelan_inet_addr("127.0.0.1");
     servaddr.sin_port = autelan_htons(SERV_PORT);
    
     on = 1;
     res = autelan_setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on));

     if( autelan_bind( sockfd, (struct sockaddr *)&servaddr, sizeof(struct sockaddr_in)) == -1)
     {
        perror("autelan_bind error");
        exit(1);
     }

    FD_ZERO(&fd_read);
    FD_SET(sockfd, &fd_read);
    while(1)
    {
        if(autelan_select(sockfd + 1, &fd_read, NULL, NULL, NULL) > 0)
        {
            memset( &cliaddr, 0, sizeof(cliaddr) );
            clilen = sizeof(struct sockaddr_in);
            rcvlen = autelan_recvfrom( sockfd, buf, MAX_BUF_SIZE, 0, (struct sockaddr *)&cliaddr, &fromlen);
        }
        if(rcvlen >= 0)
        {
            pthread_mutex_lock(&pthread_whole_scan);
            res = iw_scan_get_ap_info( buf );
            pthread_mutex_unlock(&pthread_whole_scan);
        }
        autelan_sendto( sockfd, (void *)buf, (size_t)res, 0, (struct sockaddr*)&cliaddr, sizeof(struct sockaddr_in) );
    }
 
}

int iw_scan_info_pthread(void)
{
    pthread_t pid;
    
    pthread_mutex_init( &pthread_list_metux, NULL );
    pthread_mutex_init( &pthread_whole_scan, NULL );
    if( pthread_create( &pid, NULL, iw_scan_ap_info_report, NULL ) != 0 )
    {
        autelan_printf("CREATE THREAD ERROR !\n");
        return -1;
    }
	autelan_printf("create iw_scan_ap_info_report OK.\n");
    return 0;
}

/* -------------------- */

static void iw_usage(void)
{
    autelan_printf("%s", help_msg);
    exit(1);
}

static int iw_scan_get_channel_num(int i,unsigned radio_id)
{
    char buf_cmd[CMD_LEN];
    char buf_res[RES_LEN];
    FILE *fp = NULL;
    
    memset(buf_cmd, 0, CMD_LEN);
    /************ zhouke add ,scan ath name *********/
    if (i==mode_11bg)
    {
#ifdef ARM_AF3220
        autelan_sprintf(buf_cmd, "/sbin/iwlist %s channel |grep '2.4' |wc -l", san_if_name[radio_id]);
#else
        autelan_sprintf(buf_cmd, "/usr/sbin/iwlist %s channel |grep '2.4' |wc -l", san_if_name[radio_id]);
#endif 
    }
    else if (i==mode_11a)
    {
#ifdef ARM_AF3220
        autelan_sprintf(buf_cmd, "/sbin/iwlist %s channel |grep '5.[0-9]' | wc -l", san_if_name[radio_id]);
#else
        autelan_sprintf(buf_cmd, "/usr/sbin/iwlist %s channel |grep '5.[0-9]' | wc -l", san_if_name[radio_id]);
#endif
    }
    /*********************** end ********************/
    fp = autelan_popen(buf_cmd, "r");
    if(fp)
    {  
        memset( buf_res, 0, RES_LEN);
        autelan_fgets(buf_res, sizeof(RES_LEN), fp);
        buf_res[strlen(buf_res)-1] = '\0';
        autelan_pclose(fp);
        return atoi(buf_res)-1; 
    }
    return channel_num;
}


void sigstop_handle(int x) 
{ 
  char buf_cmd[CMD_LEN];
  memset(buf_cmd, 0, CMD_LEN);
  
  if(strcmp(wifi0mode,"11bg")==0)
 	 autelan_sprintf(buf_cmd, "echo 0 > /proc/sys/dev/%s/iwlist_scan_onebyone && echo 0 >/proc/sys/dev/%s/iwlist_scan_current_chan_ng",  dev_name[0], dev_name[0]);
  else
 	 autelan_sprintf(buf_cmd, "echo 0 > /proc/sys/dev/%s/iwlist_scan_onebyone && echo 0 >/proc/sys/dev/%s/iwlist_scan_current_chan_na ",  dev_name[0], dev_name[0]);
  
  autelan_system(buf_cmd);
  if(radio_id>1)
  {
	  memset(buf_cmd, 0, CMD_LEN);
 if(strcmp(wifi1mode,"11bg")==0)
	  autelan_sprintf(buf_cmd, "echo 0 > /proc/sys/dev/%s/iwlist_scan_onebyone && echo 0 >/proc/sys/dev/%s/iwlist_scan_current_chan_ng",  dev_name[1], dev_name[1]);
 else
 	  autelan_sprintf(buf_cmd, "echo 0 > /proc/sys/dev/%s/iwlist_scan_onebyone && echo 0 >/proc/sys/dev/%s/iwlist_scan_current_chan_na",  dev_name[1], dev_name[1]);
	  autelan_system(buf_cmd);
  	}
  autelan_close(sockfd);
  exit(0);
}


static int iw_scan_create_wlan(void)
{
    char buf_cmd[CMD_LEN];
    autelan_sleep(4);
    memset(buf_cmd, 0, CMD_LEN);
    autelan_sprintf(buf_cmd, "echo 1 > /proc/sys/dev/%s/iwlist_scan_onebyone", dev_name[0]);
    autelan_system(buf_cmd);
    if(radio_id>1)
    {

		memset(buf_cmd, 0, CMD_LEN);
		autelan_sprintf(buf_cmd, "echo 1 > /proc/sys/dev/%s/iwlist_scan_onebyone", dev_name[1]);
		autelan_system(buf_cmd);
    }
   // memset(if_name, 0, sizeof(if_name));
   // sprintf(if_name, "athscan%s", dev_name[0]);
    autelan_signal(SIGTERM, sigstop_handle);
    autelan_signal(SIGINT, sigstop_handle);
}

static int iw_scan_init(void)
{ 
    int res = 0;
    ap_list_head = (S_LIST_HEAD *)malloc(channel_num * sizeof(S_LIST_HEAD));
    memset(ap_list_head, 0, channel_num * sizeof(S_LIST_HEAD));

    res = iw_scan_info_pthread();
    if( res != 0)
    {
        autelan_printf("CREATE PTHREAD ERROR !");
        exit(0);
    }
    iw_scan_create_wlan();
    autelan_signal(SIGTERM, sigstop_handle);
    autelan_signal(SIGINT, sigstop_handle);
}

static int
getsocket(void)
{
	int s = -1;

	if (s < 0) {
		s = socket(AF_INET, SOCK_DGRAM, 0);
		if (s < 0)
			err(1, "socket(SOCK_DRAGM)");
	}
	return s;
}

static int
do80211priv(struct iwreq *iwr, const char *ifname, int op, void *data, 
unsigned int len)
{
#define	N(a)	(sizeof(a)/sizeof(a[0]))

	memset(iwr, 0, sizeof(iwr));
	strncpy(iwr->ifr_name, ifname, IFNAMSIZ);
	if (len < IFNAMSIZ) {
		/*
		 * Argument data fits inline; put it there.
		 */
		memcpy(iwr->u.name, data, len);
	} else {
		/*
		 * Argument data too big for inline transfer; setup a
		 * parameter block instead; the kernel will transfer
		 * the data for the driver.
		 */
		iwr->u.data.pointer = data;
		iwr->u.data.length = len;
	}

	int s = getsocket();
	if(s<0)
		return -1;
	if (ioctl(s, op, iwr) < 0) {
		static const char *opnames[] = {
			"ioctl[IEEE80211_IOCTL_SETPARAM]",
			"ioctl[IEEE80211_IOCTL_GETPARAM]",
			"ioctl[IEEE80211_IOCTL_SETKEY]",
			"ioctl[SIOCIWFIRSTPRIV+3]",
			"ioctl[IEEE80211_IOCTL_DELKEY]",
			"ioctl[SIOCIWFIRSTPRIV+5]",
			"ioctl[IEEE80211_IOCTL_SETMLME]",
			"ioctl[SIOCIWFIRSTPRIV+7]",
			"ioctl[IEEE80211_IOCTL_SETOPTIE]",
			"ioctl[IEEE80211_IOCTL_GETOPTIE]",
			"ioctl[IEEE80211_IOCTL_ADDMAC]",
			"ioctl[SIOCIWFIRSTPRIV+11]",
			"ioctl[IEEE80211_IOCTL_DELMAC]",
			"ioctl[SIOCIWFIRSTPRIV+13]",
			"ioctl[IEEE80211_IOCTL_CHANLIST]",
			"ioctl[SIOCIWFIRSTPRIV+15]",
			"ioctl[IEEE80211_IOCTL_GETRSN]",
			"ioctl[SIOCIWFIRSTPRIV+17]",
			"ioctl[IEEE80211_IOCTL_GETKEY]",
		};
		op -= SIOCIWFIRSTPRIV;
		if (0 <= op && op < N(opnames))
			perror(opnames[op]);
		else
			perror("ioctl[unknown???]");
		autelan_close(s);
		return -1;
	}
	autelan_close(s);
	return 0;
#undef N
}


static int
get80211priv(const char *ifname, int op, void *data, u_int32_t len)
{
	struct iwreq iwr;

	if (do80211priv(&iwr, ifname, op, data, len) < 0)
		return -1;
	if (len < IFNAMSIZ)
		memcpy(data, iwr.u.name, len);
	return iwr.u.data.length;
}

static int getmaxrate(u_int8_t rates[15], u_int8_t nrates)
{
	int i, maxrate = -1;

	for (i = 0; i < nrates; i++) {
		int rate = rates[i] & IEEE80211_RATE_VAL;
		if (rate > maxrate)
			maxrate = rate;
	}
	return maxrate / 2;
}
/*<Begin : caizhibang modify for WBS-4 */
static int 
get_max_mcs_rate(uint16_t rates[36],uint8_t nrates)
{
     int i,maxrate = -1;
     for(i = 0; i < nrates; i++) {
		int rate = rates[i];
		if (rate > maxrate)
			maxrate = rate;
	}
	return maxrate / 100;
}
/*End : caizhibang modify for WBS-4> */

static unsigned int ieee80211_mhz2ieee(u_int32_t freq)
{
	autelan_printf("freq:%d\n",freq);
	if (freq == 2484)
		return 14;
	if (freq < 2484)
		return (freq - 2407) / 5;
	if (freq < 5000)
		return 15 + ((freq - 2512) / 20);
	return (freq - 5000) / 5;
}

static const char *getcaps(int capinfo)
{
	static char capstring[32];
	char *cp = capstring;

	if (capinfo & IEEE80211_CAPINFO_ESS)
		*cp++ = 'E';
	if (capinfo & IEEE80211_CAPINFO_IBSS)
		*cp++ = 'I';
	if (capinfo & IEEE80211_CAPINFO_CF_POLLABLE)
		*cp++ = 'c';
	if (capinfo & IEEE80211_CAPINFO_CF_POLLREQ)
		*cp++ = 'C';
	if (capinfo & IEEE80211_CAPINFO_PRIVACY)
		*cp++ = 'P';
	if (capinfo & IEEE80211_CAPINFO_SHORT_PREAMBLE)
		*cp++ = 'S';
	if (capinfo & IEEE80211_CAPINFO_PBCC)
		*cp++ = 'B';
	if (capinfo & IEEE80211_CAPINFO_CHNL_AGILITY)
		*cp++ = 'A';
	if (capinfo & IEEE80211_CAPINFO_SHORT_SLOTTIME)
		*cp++ = 's';
	if (capinfo & IEEE80211_CAPINFO_RSN)
		*cp++ = 'R';
	if (capinfo & IEEE80211_CAPINFO_DSSSOFDM)
		*cp++ = 'D';
	*cp = '\0';
	return capstring;
}

/* unaligned little endian access */
#define LE_READ_4(p)					\
	((u_int32_t)					\
	 ((((const u_int8_t *)(p))[0]      ) |		\
	  (((const u_int8_t *)(p))[1] <<  8) |		\
	  (((const u_int8_t *)(p))[2] << 16) |		\
	  (((const u_int8_t *)(p))[3] << 24)))

static __inline int
iswpaoui(const u_int8_t *frm)
{
	return frm[1] > 3 && LE_READ_4(frm + 2) == ((WPA_OUI_TYPE << 24) | WPA_OUI);
}

static void getencryption(const u_int8_t *vp, int ielen, char *buf, int *buf_len)
{
	while (ielen > 0) {
		switch (vp[0]) {
		case IEEE80211_ELEMID_VENDOR:
			if (iswpaoui(vp))
			{
				*buf_len = 3;
				memcpy(buf, "WPA", strlen("WPA"));
				return;
			}
			else
				*buf_len = 0;
			break;
		case IEEE80211_ELEMID_RSN:
			*buf_len = 3;
			memcpy(buf, "RSN", strlen("RSN"));
			return;
		default:
			*buf_len = 0;
			break;
		}
		ielen -= 2 + vp[1];
		vp += 2 + vp[1];
	}
	return;
}

int node_ssid_cmp(S_LIST *node, void *val)
{
    AP_NODE *ap_node = (AP_NODE *)node;
    char *test = (char *)val;
    unsigned char *tmp = ap_node->st_ap_stat.st_ap_info.BSSID;
    return !memcmp((void *)tmp, val, SSID_LEN);
}

int iw_scan_signle_channel( S_LIST_HEAD *list_head ,char radio_id)
{
    char cmd_buf[CMD_LEN]={0};
    char cmd_buf_temp[CMD_LEN]={0};
    char buf_res[RES_LEN]={0};
    AP_NODE *ap_node = NULL;
    unsigned char res_buf[24*1024];
    unsigned char *cp = NULL;
    int len = 0;
    int buf_size = 0;
   int nCount = 0;
   FILE *fp=NULL;

    S_LIST *test_iter = &list_head->head;
    int j = 0;
    test_iter = test_iter->next;
    autelan_sprintf(cmd_buf, "iwlist %s scanning >/dev/null", san_if_name[radio_id]);
    autelan_system(cmd_buf);
    autelan_printf("\n\n------iwlist %s scanning-------\n",san_if_name[radio_id]);
    autelan_sleep(SCAN_INTERVAL);

    buf_size = get80211priv(san_if_name[radio_id], IEEE80211_IOCTL_SCAN_RESULTS,
                res_buf, sizeof(res_buf));
    autelan_printf("buf_size[%d] = %d\n", radio_id,buf_size);
    if (buf_size == -1)
    {
        autelan_fprintf(stderr, "unable to get scan results\n");
        return -1;
    }
    if (buf_size < sizeof(struct ieee80211req_scan_result))
    {
        autelan_fprintf(stderr, "get no vap in scanning\n");
        return -1;
    }
    len = buf_size;
    cp = res_buf;
    while (len >= sizeof(struct ieee80211req_scan_result))
    {
           char temp_buf[32];
           int buf_len = 0;
           struct ieee80211req_scan_result *sr;
           unsigned char *vp;
           sr = (struct ieee80211req_scan_result *)cp;
           vp = (unsigned char *)(sr+1);
           ap_node = (AP_NODE *)IW_SCAN_LIST_NODE_FIND( list_head, node_ssid_cmp, sr->isr_bssid );

           if( NULL == ap_node )
           {
                ap_node = (AP_NODE *)malloc(sizeof(AP_NODE));
                memset(ap_node, 0, sizeof(AP_NODE));
                memcpy(ap_node->st_ap_stat.st_ap_info.BSSID, sr->isr_bssid, SSID_LEN);
                IW_SCAN_LIST_NODE_ADD( list_head, (S_LIST *)ap_node );                
           }
           /*<Begin : caizhibang modify for WBS-4 */
        //  ap_node->st_ap_stat.st_ap_info.Rate = (unsigned short)getmaxrate(sr->isr_rates, sr->isr_nrates);
           ap_node->st_ap_stat.st_ap_info.Rate = (unsigned short)get_max_mcs_rate(sr->isr_rates, sr->isr_nrates);
           /*End : caizhibang modify for WBS-4> */
		   ap_node->st_ap_stat.st_ap_info.Channel = (unsigned char)(sr->isr_freq);
           ap_node->st_ap_stat.st_ap_info.RSSI = (unsigned char) sr->isr_rssi;
           ap_node->st_ap_stat.st_ap_info.NOISE = sr->isr_noise;
           ap_node->st_ap_stat.st_ap_info.BEACON_INT = sr->isr_intval;
           ap_node->st_ap_stat.st_ap_info.Opstatus = NEW;
           ap_node->st_ap_stat.st_ap_info.CapabilityInfo = sr->isr_capinfo;
		  
		   ap_node->st_ap_stat.st_ap_info.radio_id = radio_id;
           if( sr->isr_ssid_len < ESSID_LEN )
           {
               ap_node->st_ap_stat.st_ap_info.EssidLen = sr->isr_ssid_len;
               memset(ap_node->st_ap_stat.st_ap_info.ESSID, 0, ESSID_LEN);
               memcpy(ap_node->st_ap_stat.st_ap_info.ESSID, vp, ap_node->st_ap_stat.st_ap_info.EssidLen);
           }

           int tmp_len = strlen(getcaps(sr->isr_capinfo));
           getencryption(vp + sr->isr_ssid_len, sr->isr_ie_len, temp_buf, &buf_len);
           tmp_len += buf_len;
           if( ap_node->st_ap_stat.st_ap_info.IeLen < IES_INFO_LEN )
           {
                ap_node->st_ap_stat.st_ap_info.IeLen = tmp_len;
                memset(ap_node->st_ap_stat.st_ap_info.IEs_INFO, 0, IES_INFO_LEN);
                memcpy(ap_node->st_ap_stat.st_ap_info.IEs_INFO, getcaps(sr->isr_capinfo), ap_node->st_ap_stat.st_ap_info.IeLen);
                strcat(ap_node->st_ap_stat.st_ap_info.IEs_INFO, temp_buf);
           }
           ap_node->st_ap_stat.node_stat = STAT_NEW;
           cp += sr->isr_len;
           len -= sr->isr_len;
		   nCount ++;
    }

/*
    printf("-----------[0]-----------\n");
    test_iter = &list_head->head;
    j = 0;
    test_iter = test_iter->next;
    printf("node_count: %d\n", list_head->node_count);
    for( ; j<list_head->node_count; j++ )
    {
        AP_NODE *p_test = (AP_NODE *)test_iter;
        printf("node[%d]:\n", j);
        printf("   node_stat: %d\n", p_test->st_ap_stat.node_stat);
        printf("   node_essid: %s\n", p_test->st_ap_stat.st_ap_info.ESSID);
        printf("   node_bssid: %02x:%02x:%02x:%02x:%02x:%02x\n", 
                   p_test->st_ap_stat.st_ap_info.BSSID[0],
                   p_test->st_ap_stat.st_ap_info.BSSID[1],
                   p_test->st_ap_stat.st_ap_info.BSSID[2],
                   p_test->st_ap_stat.st_ap_info.BSSID[3],
                   p_test->st_ap_stat.st_ap_info.BSSID[4],
                   p_test->st_ap_stat.st_ap_info.BSSID[5]);

        printf("   node_channel: %d\n", p_test->st_ap_stat.st_ap_info.Channel);
        printf("   node_next: 0x%x\n", test_iter->next);
        test_iter = test_iter->next;
    }
    printf("--------------------------\n");
*/
    //system("wlanconfig athscanwifi0 list ap");
    S_LIST *iter = list_head->head.next;
    S_LIST *pre_iter = (S_LIST *)list_head;

    while(NULL != iter)
    {
        AP_NODE *node_tmp = (AP_NODE *)iter;
        node_tmp->st_ap_stat.node_stat--;
        if(node_tmp->st_ap_stat.node_stat <= STAT_EPT)
        {
            node_tmp = (AP_NODE *)IW_SCAN_LIST_NODE_DEL(list_head, pre_iter);
            if(node_tmp != NULL){
                free(node_tmp); 
                node_tmp = NULL;
            }
        }
        else
        {
            pre_iter = pre_iter->next;
        }
        iter = pre_iter->next;
    }
}

int iw_scan_whole_channel(unsigned int interval)
{
    int i;  
    int cur_chan_num = 0;
    char cmd_buf[CMD_LEN];

    memset(cmd_buf, 0, CMD_LEN);
    if(strcmp(wifi0mode,"11bg")==0)
   	 autelan_sprintf(cmd_buf, "echo 0 >/proc/sys/dev/%s/iwlist_scan_current_chan_ng", dev_name[0]);
   else
   	 autelan_sprintf(cmd_buf, "echo 0 >/proc/sys/dev/%s/iwlist_scan_current_chan_na", dev_name[0]);
    autelan_system(cmd_buf);
    for(i=0; i<channel_num; i++)
    {
       cur_chan_num = iw_scan_get_channel_num(wifimodetoint(wifi0mode),0);
       if(cur_chan_num != channel_num)
       {
            autelan_printf("return -1\n");
            return -1;
       }
       pthread_mutex_lock(&pthread_list_metux);
       iw_scan_signle_channel(ap_list_head+i,0);
       pthread_mutex_unlock(&pthread_list_metux);
       autelan_sleep(interval);
    }
    return 0;
}

void  iw_scan_wifi1(void )
{
	int i =0;
    pthread_detach(pthread_self());
	while(1)
	{
	    autelan_sleep(un_interval_scan);
	    if( -1 == iw_scan_whole_channel_wifi1(un_interval_chan) )
	    {
	        for(; i<channel_num_wifi1; i++)
	        {
	            IW_SCAN_LIST_NODE_CLEAN( ap_list_head_wifi1 + i );
	        }
	        channel_num_wifi1 = iw_scan_get_channel_num(wifimodetoint(wifi1mode),1);
	    }
	}
  	return;
}


int iw_scan_init_wifi1(void)
{
	int res = 0;
	pthread_t pid;
	channel_num_wifi1 = iw_scan_get_channel_num(wifimodetoint(wifi1mode),1);
	ap_list_head_wifi1 = (S_LIST_HEAD *)malloc(channel_num_wifi1 * sizeof(S_LIST_HEAD));
	memset(ap_list_head_wifi1, 0, channel_num_wifi1 * sizeof(S_LIST_HEAD));
	pthread_mutex_init( &pthread_list_metux_wifi1, NULL );
	pthread_mutex_init( &pthread_whole_scan_wifi1, NULL );

	if( pthread_create( &pid, NULL, iw_scan_wifi1, NULL ) != 0 )
	{
		autelan_printf("CREATE THREAD ERROR !\n");
		return -1;
	}
	autelan_printf("create iw_scan_wifi1 OK\n");
	return 0;

}


int iw_scan_whole_channel_wifi1(unsigned int interval)
{
    int cur_chain_num = 0;
	int i=0;  
    int cur_chan_num = 0;
    char cmd_buf[CMD_LEN];
    memset(cmd_buf, 0, CMD_LEN);
    if(strcmp(wifi1mode,"11bg")==0)
   	 autelan_sprintf(cmd_buf, "echo 0 >/proc/sys/dev/%s/iwlist_scan_current_chan_ng", dev_name[1]);
   else
    	autelan_sprintf(cmd_buf, "echo 0 >/proc/sys/dev/%s/iwlist_scan_current_chan_na", dev_name[1]);
    autelan_system(cmd_buf);
	
	for(;i<channel_num_wifi1;i++)
	{
		cur_chain_num = iw_scan_get_channel_num(wifimodetoint(wifi1mode),1);
		if (cur_chain_num !=channel_num_wifi1)
			{autelan_printf("cur_chain_num is equal channel_num_wifi1,return.\n");return -1;}
		
		pthread_mutex_lock(&pthread_list_metux_wifi1);
		iw_scan_signle_channel(ap_list_head_wifi1+i,1);
		pthread_mutex_unlock(&pthread_list_metux_wifi1);
		autelan_sleep(interval);
	}
	 return 0;
}

int wifimodetoint(char *wifimode)
{
	if(wifimode==NULL)
		return 0;
	if(strcmp(wifimode,"11a")==0)
		return 1;
	if(strcmp(wifimode,"11bg")==0)
		return 2;
	autelan_printf("wifimode is error,return\n");
	return 0;
}

int main(int argc, char** argv)
{
    int opt = 0;
    int is_back = 0;
    int res = 0;
    int i = 0;
    char cmd_buf[CMD_LEN];
	
    radio_id = 1;
	while ((opt = getopt(argc,argv,"bd:I:i:m:c:n:")) != -1) {
    	switch (opt) 
        {
    		case 'b':
    			is_back = 1;
    			break;
    		case 'd':
#if 0
                strcpy(dev_name, optarg);
                printf("dev name: %s\n",dev_name);
                memset(if_name, 0, sizeof(if_name));
                sprintf(if_name, "athscan%s", dev_name);
#endif
    			break;
    		case 'I':
	                un_interval_scan = atoi(optarg);
	                autelan_printf("interval_scan: %u\n", un_interval_scan);
    			break;
    		case 'i':
	                un_interval_chan = atoi(optarg);
	                autelan_printf("interval_chan: %u\n", un_interval_chan);
    			break;
		case 'n':
			radio_id = atoi(optarg);
			autelan_printf("radio count: %d\n",radio_id);
			break;
		case 'c':
			  if(strcmp(optarg,"11bg")==0){autelan_sprintf(wifi1mode,"%s",optarg);autelan_printf("the second wifi mode is 11bg\n");}
			  	else if (strcmp(optarg,"11a")==0){autelan_sprintf(wifi1mode,"%s",optarg);autelan_printf("the second wifi  mode is 11a\n");}
			  else {autelan_printf("the parameter following the c is wrong ,it must be '11bg'or '11a'\n");iw_usage();}
			  break;
		case 'm':
			  if(strcmp(optarg,"11bg")==0){autelan_sprintf(wifi0mode,"%s",optarg);autelan_printf("the first wifi mode is 11bg\n");}
			  else if (strcmp(optarg,"11a")==0){autelan_sprintf(wifi0mode,"%s",optarg);autelan_printf("the first wifi mode is 11a\n");}
			  else {autelan_printf("the parameter following the m is wrong ,it must be '11bg'or '11a'\n");iw_usage();}
			  	break;
    		default: 
    			iw_usage();
    	}
	}
    if( 0 == un_interval_scan || 0 == un_interval_chan )
    {
        iw_usage();
    }
	autelan_sleep(5);
#ifdef AQ2010
	{
		struct sigaction sa;
		sa.sa_handler = SIG_IGN;
			#ifdef SA_NOCLDWAIT
		sa.sa_flags = SA_NOCLDWAIT;
			#else
			sa.sa_flags = 0;
			#endif
		sigemptyset(&sa.sa_mask);
		sigaction(SIGCHLD, &sa, NULL);
	}
#endif
    if(is_back)
    {
        if( daemon(0,0) ){
            autelan_fprintf(stderr, "daemon error") ;
            return 1;
        }
    }
	if(radio_id>1)
		iw_scan_init_wifi1();
	
    pthread_mutex_lock(&pthread_whole_scan);
    channel_num = iw_scan_get_channel_num(wifimodetoint(wifi0mode),0);
    iw_scan_init();
    
    if( channel_num > 13 )
    {
      //  ap_list_head = (S_LIST_HEAD *)realloc( ap_list_head, channel_num * sizeof(S_LIST_HEAD));
    }

    while( -1 == iw_scan_whole_channel(1) )
    {
        autelan_printf("return -1\n");
        for(; i<channel_num; i++)
        {
            IW_SCAN_LIST_NODE_CLEAN( ap_list_head+i );
        }
        channel_num = iw_scan_get_channel_num(wifimodetoint(wifi0mode),0);
        if( channel_num > 13 )
        {
            //ap_list_head = (S_LIST_HEAD *)realloc( ap_list_head, channel_num * sizeof(S_LIST_HEAD));
        }
        autelan_sleep(2);
    }
    pthread_mutex_unlock(&pthread_whole_scan);

    while(1)
    {
        autelan_sleep(un_interval_scan);
        if( -1 == iw_scan_whole_channel(un_interval_chan))
        {
            autelan_printf("return -1\n");
            for(; i<channel_num; i++)
            {
                IW_SCAN_LIST_NODE_CLEAN(ap_list_head + i);
            }
            channel_num = iw_scan_get_channel_num(wifimodetoint(wifi0mode),0);
            autelan_printf("channel num: %d\n", channel_num);
            if( channel_num > 13 )
            {
                //ap_list_head = (S_LIST_HEAD *)realloc( ap_list_head, channel_num * sizeof(S_LIST_HEAD));
            }
        }
  }
}
