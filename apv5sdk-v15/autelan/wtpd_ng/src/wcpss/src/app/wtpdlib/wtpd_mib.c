#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <unistd.h>
#include "common.h"
#include "wtpd_mib.h"


extern int debug_print;
extern CWWTPSta *wtp_sta_list;
extern CWWTPWlan *wtp_wlan_list;
extern unsigned char gFloodingDetectEnable;
extern unsigned char gSpoofingDetectEnable;
extern unsigned char gWeakIVDetectEnable;

extern char * trim(char *str_org);
extern int inet_atoi(char *cp, unsigned int *ap);
extern CWErrorCode CWErrorGetLastErrorCode();
extern __inline__ void CWDebugLog(const char *format, ...);
extern __inline__ void CWWTPDebugLog(const char *format, ...);
extern int macAddrCmp (unsigned char* addr1, unsigned char* addr2);
extern CWBool _CWErrorHandleLast(const char *fileName, int line);
extern unsigned short CWProtocolRetrieve16(CWProtocolMessage *msgPtr);
extern CWBool _CWErrorRaise(CWErrorCode code, const char *msg, const char *fileName, int line) ;
extern void CWWTPGetWlanName(char *wlanname,unsigned char radioId, unsigned char wlanId);
extern CWBool CWNetworkReceiveUnsafe(CWSocket sock, char *buf, int len, int flags, CWNetworkLev4Address *addrPtr, int *readBytesPtr);
CWBool get_wifi_info(char *wifiname, wlan_stats_info *wlanStatsInfo)
{
	int s;
	struct ifreq ifr;
	struct ath_stats total = { 0 };
//	unsigned long itot, otot;

	s = socket(AF_INET, SOCK_DGRAM, 0);
	if (s < 0)
	{
		CWWTPDebugLog("create sock to get_wifi_info error!");
		if(debug_print)
			printf("create sock to get_wifi_info error!\n");
		return CW_FALSE;
	}

    /**** zhouke add ,for marvell ioctl ****/
#ifdef MARVELL_PLATFORM
    memset(&ifr, 0, sizeof(struct ifreq));
#endif
    /************ end ************/

	strncpy(ifr.ifr_name, wifiname, sizeof (ifr.ifr_name));

	ifr.ifr_data = (caddr_t) &total;
	if (ioctl(s, SIOCGATHSTATS, &ifr) < 0)
	{
		CWWTPDebugLog("ioctl sock to get_wifi_info error!");
		if(debug_print)
			printf("ioctl sock to get_wifi_info error!\n");
		close(s);
		return CW_FALSE;
	}

	wlanStatsInfo->ast_rx_crcerr = total.ast_rx_crcerr;
	wlanStatsInfo->ast_rx_badcrypt = total.ast_rx_badcrypt;
	wlanStatsInfo->ast_rx_badmic = total.ast_rx_badmic;
	wlanStatsInfo->ast_rx_phyerr = total.ast_rx_phyerr;
	
/*	if (!getifstats(ifr.ifr_name, &itot, &otot))
	{
		if(debug_print)
			printf("getifstats to get_wifi_info error!\n");
		close(s);
		return CW_FALSE;
		err(1, ifr.ifr_name);
	}
	printf("%8u %8u %7u %7u %7u %6u %6u %6u %7u\n"
		, itot - total.ast_rx_mgt
		, otot
		, total.ast_tx_altrate
		, total.ast_tx_shortretry
		, total.ast_tx_longretry
		, total.ast_tx_xretries
		, total.ast_rx_crcerr
		, total.ast_rx_badcrypt
		, total.ast_rx_phyerr
	);*/
	close(s);
	return CW_TRUE;
}
CWBool CWGetExtraInfo(char *wifiname, extra_info *extraInfoValues)
{
	int s;
	struct ifreq ifr;
	struct ath_stats total;
//	unsigned long icur, ocur;
//	unsigned long itot, otot;

	s = socket(AF_INET, SOCK_DGRAM, 0);
	if (s < 0)
	{
		CWWTPDebugLog("create sock to get extra info error!");
		if(debug_print)
			printf("create sock to get extra info error!\n");
		return CW_FALSE;
	}

    /**** zhouke add ,for marvell ioctl ****/
#ifdef MARVELL_PLATFORM
    memset(&ifr, 0, sizeof(struct ifreq));
#endif
    /************ end ************/
	strncpy(ifr.ifr_name, wifiname, sizeof (ifr.ifr_name));

	ifr.ifr_data = (caddr_t) &total;
	if (ioctl(s, SIOCGATHSTATS, &ifr) < 0)
	{
		CWWTPDebugLog("ioctl to get extra info error!");
		if(debug_print)
			printf("ioctl to get extra info error!\n");
		close(s);
		return CW_FALSE;
		//err(1, ifr.ifr_name);
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
	extraInfoValues->tx_mgmt += total.ast_tx_mgmt;
	extraInfoValues->rx_mgmt += total.ast_rx_mgt;
	extraInfoValues->tx_packets += total.ast_tx_packets;
	extraInfoValues->tx_errors += total.ast_tx_xretries;
	extraInfoValues->tx_retry += total.ast_tx_xretries;
		
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
	return CW_TRUE;
}
CWBool get_athinfo_from_80211stats(char *ifname, wlan_stats_info *wlanStatsInfo)
{
	struct ieee80211_stats stats;
	struct ifreq ifr;
	int s;
	
	s = socket(AF_INET, SOCK_DGRAM, 0);
	if (s < 0)
	{
		CWWTPDebugLog("create sock to get 80211stats error!");
		if(debug_print)
			printf("create sock to get 80211stats error!\n");
		return CW_FALSE;
	}

    /**** zhouke add ,for marvell ioctl ****/
#ifdef MARVELL_PLATFORM
    memset(&ifr, 0, sizeof(struct ifreq));
#endif
    /************ end ************/

	strncpy(ifr.ifr_name, ifname, sizeof (ifr.ifr_name));
	ifr.ifr_data = (caddr_t) &stats;
	if (ioctl(s, SIOCG80211STATS, &ifr) < 0)
	{
		CWWTPDebugLog("ioctl to get 80211stats error!");
		if(debug_print)
			printf("ioctl to get 80211stats error!\n");
		close(s);
		return CW_FALSE;
	}
	
	wlanStatsInfo->rx_mgmt = stats.is_rx_mgmt;
	wlanStatsInfo->tx_mgmt = stats.is_tx_mgmt;
	wlanStatsInfo->rx_mgmt_bytes = stats.is_rx_mgmtbytes;
	wlanStatsInfo->tx_mgmt_bytes = stats.is_tx_mgmtbytes;

	close(s);
	return CW_TRUE;
}
CWBool CWGetWpiErrors(char *ifname, wpi_errors *wpi_error_values)
{
	struct ieee80211_stats stats;
	struct ifreq ifr;
	int s;
	
	s = socket(AF_INET, SOCK_DGRAM, 0);
	if (s < 0)
	{
		CWWTPDebugLog("create sock to get 80211stats error!");
		if(debug_print)
			printf("create sock to get 80211stats error!\n");
		return CW_FALSE;
	}

    /**** zhouke add ,for marvell ioctl ****/
#ifdef MARVELL_PLATFORM
    memset(&ifr, 0, sizeof(struct ifreq));
#endif
    /************ end ************/

	strncpy(ifr.ifr_name, ifname, sizeof (ifr.ifr_name));
	ifr.ifr_data = (caddr_t) &stats;
	if (ioctl(s, SIOCG80211STATS, &ifr) < 0)
	{
		CWWTPDebugLog("ioctl to get 80211stats error!");
		if(debug_print)
			printf("ioctl to get 80211stats error!\n");
		close(s);
		return CW_FALSE;
	}
	wpi_error_values->wpi_replay_error = stats.is_re_wpi;
	wpi_error_values->wpi_decryp_error = stats.is_wpi_no_key_error;
	wpi_error_values->wpi_mic_error = stats.is_wpi_mic;

	close(s);
	return CW_TRUE;
}
CWBool CWGetDisassocErrors(char *ifname, disassoc_errors *disassoc_error_values)
{
	struct ieee80211_stats stats;
	struct ifreq ifr;
	int s;
	
	s = socket(AF_INET, SOCK_DGRAM, 0);
	if (s < 0)
	{
		CWWTPDebugLog("create sock to get 80211stats error!");
		if(debug_print)
			printf("create sock to get 80211stats error!\n");
		return CW_FALSE;
	}

    /**** zhouke add ,for marvell ioctl ****/
#ifdef MARVELL_PLATFORM
    memset(&ifr, 0, sizeof(struct ifreq));
#endif
    /************ end ************/

	strncpy(ifr.ifr_name, ifname, sizeof (ifr.ifr_name));
	ifr.ifr_data = (caddr_t) &stats;
	if (ioctl(s, SIOCG80211STATS, &ifr) < 0)
	{
		CWWTPDebugLog("ioctl to get 80211stats error!");
		if(debug_print)
			printf("ioctl to get 80211stats error!\n");
		close(s);
		return CW_FALSE;
	}
//	printstats(stdout, &stats);
#if (defined(MODE11N)||defined(AR11N))
	disassoc_error_values->disassoc_unnormal = stats.is_disassoc_unnormal;
#else
	disassoc_error_values->disassoc_unnormal = stats.is_disassoc_unnormal+stats.is_node_fdisassoc;
#endif
	disassoc_error_values->rx_assoc_norate = stats.is_rx_assoc_norate;
	disassoc_error_values->rx_assoc_capmismatch = stats.is_rx_assoc_capmismatch;
	disassoc_error_values->assoc_invalid = stats.is_rx_bad_assoc;
#if (defined(MODE11N)||defined(AR11N))
	disassoc_error_values->reassoc_deny = stats.is_rx_assoc_bss + stats.is_rx_assoc_notauth + stats.is_rx_assoc_capmismatch + stats.is_rx_assoc_norate 
					+ stats.is_rx_assoc_badwpaie ;
#else
	disassoc_error_values->reassoc_deny = stats.is_rx_assoc_bss + stats.is_rx_assoc_notauth + stats.is_rx_assoc_capmismatch + stats.is_rx_assoc_norate 
					+ stats.is_rx_assoc_badwpaie + stats.is_rx_assoc_badscie;
#endif

	close(s);
	return CW_TRUE;
}
void get_sta_stats( StationInfo *staInfoValues,int sta_count)
{
	int s,i ;
	struct iwreq iwr;
	struct ieee80211req_sta_stats stats;
	const struct ieee80211_nodestats *ns = &stats.is_stats;
	char ifname[IFNAMSIZ];
	
	memset(&stats,0,sizeof(struct ieee80211req_sta_stats));//yuanding add
	s = socket(AF_INET, SOCK_DGRAM, 0);
	if (s < 0)
	{
		CWWTPDebugLog("create sock to get_stations error!");
		if(debug_print)
			printf("create sock to get_stations error!\n");
		return;
	}
	for(i=0;i<sta_count;i++){
		(void) memset(&iwr, 0, sizeof(iwr));
		CWWTPGetWlanName(ifname, staInfoValues[i].radioId, staInfoValues[i].wlanId);
		(void) strncpy(iwr.ifr_name, ifname,IFNAMSIZ);
		iwr.u.data.pointer = (void *) &stats;
		iwr.u.data.length = sizeof(stats);

		memcpy(stats.is_u.macaddr, staInfoValues[i].mac, IEEE80211_ADDR_LEN);
		if (ioctl(s, IEEE80211_IOCTL_STA_STATS, &iwr) < 0){
			CWWTPDebugLog("ioctl to get_sta_stats error!");
			if(debug_print)
				printf("ioctl to get_sta_stats error!\n");
			close(s);
			return;
		}
		staInfoValues[i].rx_bytes = ns->ns_rx_bytes;
		staInfoValues[i].tx_bytes = ns->ns_tx_bytes;
		staInfoValues[i].rx_data_bytes = ns->ns_rx_bytes;
		staInfoValues[i].tx_data_bytes = ns->ns_tx_bytes;
		staInfoValues[i].rx_data_frames = ns->ns_rx_data;
		staInfoValues[i].tx_data_frames = ns->ns_tx_data;
		staInfoValues[i].rx_frames = ns->ns_rx_data+ns->ns_rx_mgmt+ns->ns_rx_ctrl;
		staInfoValues[i].tx_frames = ns->ns_tx_data+ns->ns_tx_mgmt+ns->ns_tx_ctrl;
/*		if(debug_print)
			printf("sta_state %s rx_bytes=%d tx_bytes= %d rx_data_bytes=%llu  tx_data_bytes=%llu  rx_data_frames=%d  tx_data_frames=%d  rx_frames=%d  tx_frames=%d rx_frag_packets=%d  tx_frag_packets=%d  \n",
				ifname,staInfoValues[i].rx_bytes, staInfoValues[i].tx_bytes,staInfoValues[i].rx_data_bytes,
				staInfoValues[i].tx_data_bytes,staInfoValues[i].rx_data_frames,staInfoValues[i].tx_data_frames,
				staInfoValues[i].rx_frames,staInfoValues[i].tx_frames,staInfoValues[i].rx_frag_packets,
				staInfoValues[i].tx_frag_packets);*/
	}
	close(s);
	return;
}
void get_stations_ctrl(wlan_stats_info *wlanStatsInfo)
{
	int s;
	unsigned int tx_ctrl =0;
	unsigned int rx_ctrl =0;
	
	struct iwreq iwr;
	struct ieee80211req_sta_stats stats;
	const struct ieee80211_nodestats *ns = &stats.is_stats;
	char ifname[IFNAMSIZ];
	CWWTPSta *ptr = NULL;
	
	s = socket(AF_INET, SOCK_DGRAM, 0);
	if (s < 0)
	{
		CWWTPDebugLog("create sock to get_stations error!");
		if(debug_print)
			printf("create sock to get_stations error!\n");
		return;
	}

	for(ptr = wtp_sta_list;ptr != NULL;ptr = ptr->next){
		if(ptr->radio_id==wlanStatsInfo->radioId&&ptr->wlan_id==wlanStatsInfo->wlanId){
			(void) memset(&iwr, 0, sizeof(iwr));
			CWWTPGetWlanName(ifname,  ptr->radio_id, ptr->wlan_id);
			(void) strncpy(iwr.ifr_name, ifname,IFNAMSIZ);
			iwr.u.data.pointer = (void *) &stats;
			iwr.u.data.length = sizeof(stats);

			memcpy(stats.is_u.macaddr, ptr->sta_mac, IEEE80211_ADDR_LEN);
			if (ioctl(s, IEEE80211_IOCTL_STA_STATS, &iwr) < 0){
				CWWTPDebugLog("ioctl to get_sta_ctrl error!");
				if(debug_print)
					printf("ioctl to get_sta_ctrl error!\n");
				close(s);
				return;
			}
			rx_ctrl += ns->ns_rx_ctrl;
			tx_ctrl += ns->ns_tx_ctrl;
		}
		
		wlanStatsInfo->rx_ctrl=rx_ctrl ;
		wlanStatsInfo->tx_ctrl=tx_ctrl ;
	}
	
	close(s);
	return;
}
void getStaWapiInfos(int staCount, StationWapiInfo *StaWapiInfo)
{
	int s, i = 0;
	char ifname[IFNAMSIZ];
	struct iwreq iwr;
	struct ieee80211req_sta_stats stats;

	for(i = 0; i<staCount; i++)
	{
		StaWapiInfo[i].version = 1;
		StaWapiInfo[i].controlledPortStatus = 1;
		StaWapiInfo[i].selectedUnicastCipher = 1339905;  /* OUI(24bit):00-14-72, type(8bit):1*/
	}
	s = socket(AF_INET, SOCK_DGRAM, 0);
	if (s < 0)
	{
		if(debug_print)
			printf("create sock to getStaWapiInfos error!\n");
		return;
	}
	for(i = 0; i<staCount; i++)
	{
		(void) memset(&iwr, 0, sizeof(iwr));
		(void) memset(&stats, 0, sizeof(stats));
		CWWTPGetWlanName(ifname, StaWapiInfo[i].radioId, StaWapiInfo[i].wlanId);
		(void) strncpy(iwr.ifr_name, ifname, sizeof(iwr.ifr_name));
		iwr.u.data.pointer = (void *) &stats;
		iwr.u.data.length = sizeof(stats);
		memcpy(stats.is_u.macaddr, &(StaWapiInfo[i].mac), IEEE80211_ADDR_LEN);

		if (ioctl(s, IEEE80211_IOCTL_STA_STATS, &iwr) < 0)
		{
			if(debug_print)
				printf("unable to get station stats for [%02x-%02x-%02x-%02x-%02x-%02x]\n", StaWapiInfo[i].mac[0], 
				StaWapiInfo[i].mac[1], StaWapiInfo[i].mac[2], StaWapiInfo[i].mac[3], StaWapiInfo[i].mac[4], StaWapiInfo[i].mac[5]);
				//printf("unable to get station stats for %s\n", ether_ntoa((const struct ether_addr*) &(StaWapiInfo[i].mac)));

			continue;
		}
#if (defined(MODE11N)||defined(AR11N))
#else
		StaWapiInfo[i].wpiReplayCounters = stats.is_stats.ns_re_wpi;
		StaWapiInfo[i].wpiDecryptableErrors = stats.is_stats.ns_wpi_no_key_error;
		StaWapiInfo[i].wpiMICErrors = stats.is_stats.ns_wpi_mic;
#endif
	}
	
	close(s);
	return;
}

/*
 * Convert MHz frequency to IEEE channel number.
 */
u_int32_t
ieee80211_mhz2ieee(u_int32_t freq)
{
	return freq;
	if (freq == 2484)
		return 14;
	if (freq < 2484)
		return (freq - 2407) / 5;
	if (freq < 5000)
		return 15 + ((freq - 2512) / 20);
	return (freq - 5000) / 5;
}

void get_stations(char *ifname, int staCount, StationInfo *staInfoValues)
{
	static unsigned char buf[24*1024];
	struct iwreq iwr;
	unsigned char *cp;
	int s, len;
	int i=0;

	s = socket(AF_INET, SOCK_DGRAM, 0);
	if (s < 0)
	{
		CWWTPDebugLog("create sock to get_stations error!");
		if(debug_print)
			printf("create sock to get_stations error!\n");
		return;
	}

	(void) memset(&iwr, 0, sizeof(iwr));
	(void) strncpy(iwr.ifr_name, ifname, sizeof(iwr.ifr_name));
	iwr.u.data.pointer = (void *) buf;
	iwr.u.data.length = sizeof(buf);
	if (ioctl(s, IEEE80211_IOCTL_STA_INFO, &iwr) < 0)
	{
		CWWTPDebugLog("ioctl to get_stations error!");
		if(debug_print)
			printf("ioctl to get_stations error!\n");
		close(s);
		return;
	}
	len = iwr.u.data.length;
	if (len < sizeof(struct ieee80211req_sta_info))
	{
		CWWTPDebugLog("get sta info for get_stations error!");
		if(debug_print)
			printf("get sta info for get_stations error!\n");
		close(s);
		return;
	}

	cp = buf;
	do {
		struct ieee80211req_sta_info *si;
		u_int8_t *vp;

		si = (struct ieee80211req_sta_info *) cp;
		vp = (u_int8_t *)(si+1);
		for(i=0;i<staCount;i++)
		{
			if(macAddrCmp(si->isi_macaddr, staInfoValues[i].mac))
			{
				staInfoValues[i].channel = ieee80211_mhz2ieee(si->isi_freq);
				staInfoValues[i].rssi = si->isi_rssi;
				staInfoValues[i].tx_Rate = (si->isi_rates[si->isi_txrate]&IEEE80211_RATE_VAL)*10/2;
				if(si->isi_freq>2000&&si->isi_freq<3000){
					if(si->isi_nrates==4)
						staInfoValues[i].mode = IEEE80211_PARAM_MODE_11b;
					else if(si->isi_nrates==12)
						staInfoValues[i].mode = IEEE80211_PARAM_MODE_11b|IEEE80211_PARAM_MODE_11g;
					else
						staInfoValues[i].mode = IEEE80211_PARAM_MODE_11g;
				}
				else
					staInfoValues[i].mode = IEEE80211_PARAM_MODE_11a;
#if (defined(MODE11N)||defined(AR11N))
				if(si->isi_txratekbps != 0)
					staInfoValues[i].tx_Rate = si->isi_txratekbps *10/1000;
				else
					staInfoValues[i].tx_Rate = (si->isi_rates[si->isi_txrate] & IEEE80211_RATE_VAL)*10/2;
				if(si->isi_rxratekbps != 0)
					staInfoValues[i].rx_Rate = si->isi_rxratekbps *10/1000;
				else
					staInfoValues[i].rx_Rate =0;

				if(si->isi_htcap!=0){
					if(si->isi_freq>2000&&si->isi_freq<3000)
						staInfoValues[i].mode = IEEE80211_PARAM_MODE_11b|IEEE80211_PARAM_MODE_11g|IEEE80211_PARAM_MODE_11n;
					else
						staInfoValues[i].mode = IEEE80211_PARAM_MODE_11a|IEEE80211_PARAM_MODE_11n;//11b-0x01,11a-0x02,11g-0x04,11n-0x08,
				}
				else{
					if(si->isi_freq>2000&&si->isi_freq<3000){
						if(si->isi_nrates==4)
							staInfoValues[i].mode = IEEE80211_PARAM_MODE_11b;
						else if(si->isi_nrates==12)
							staInfoValues[i].mode = IEEE80211_PARAM_MODE_11b|IEEE80211_PARAM_MODE_11g;
						else
							staInfoValues[i].mode = IEEE80211_PARAM_MODE_11g;
					}
					else
						staInfoValues[i].mode = IEEE80211_PARAM_MODE_11a;
				}
#endif
				staInfoValues[i].isPowerSave = 0;
				staInfoValues[i].isQos = 0;
				if(si->isi_state & IEEE80211_NODE_PWR_MGT)
					staInfoValues[i].isPowerSave = 1;
				if(si->isi_state & IEEE80211_NODE_QOS)
					staInfoValues[i].isQos = 1;
			}
		}
		
		cp += si->isi_len;
		len -= si->isi_len;
	} while (len >= sizeof(struct ieee80211req_sta_info));

	close(s);
	return;
}
CWBool CWGetInterfaceStatus(char *ifname, unsigned char *status)
{
	int sockfd;
	struct ifreq ifr;

    /**** zhouke add ,for marvell ioctl ****/
#ifdef MARVELL_PLATFORM
    memset(&ifr, 0, sizeof(struct ifreq));
#endif
    /************ end ************/
    
	*status = 0;
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	strncpy(ifr.ifr_name,ifname, sizeof(ifr.ifr_name));
	if(ioctl(sockfd, SIOCGIFINDEX, &ifr) == -1){//bind to a interface 
		CWDebugLog("SIOCGIFINDEX error\n");
		CWWTPDebugLog("%s is not exit !", ifname);
		if(debug_print)
			printf("%s is not exit !\n", ifname);
		close(sockfd);
		*status = 3;
		return CW_TRUE;
	}
	
	if (ioctl(sockfd, SIOCGIFFLAGS, &ifr) == -1){
		CWWTPDebugLog("SIOCGIFFLAGS error");
		if(debug_print)
			printf("SIOCGIFFLAGS error\n");
		close(sockfd);
		*status = 3;
		return CW_TRUE;
	}
	if(ifr.ifr_flags & IFF_UP){
		if(debug_print)
			printf("%s is UP\n",ifname);
		*status = 1;
	}else{
		if(debug_print)
			printf("%s is DOWN\n",ifname);
		*status = 2;
	}

	close(sockfd);
	return CW_TRUE;
}
CWBool CWGetWifiStatus(extra_info *extraInfoValues)
{
	int i = 0;
	int sockfd;
	struct ifreq ifr;
	char ifname[IFNAMSIZ];
	
	for(i=0;i<extraInfoValues->wifi_count;i++)
	{
		extraInfoValues->wifi_status[i].id = i;
		extraInfoValues->wifi_status[i].status = 0;
		sprintf(ifname, "wifi%d", i);
		sockfd = socket(AF_INET, SOCK_DGRAM, 0);

        /**** zhouke add ,for marvell ioctl ****/
#ifdef MARVELL_PLATFORM
        memset(&ifr, 0, sizeof(struct ifreq));
#endif
        /************ end ************/

		strncpy(ifr.ifr_name,ifname, sizeof(ifr.ifr_name));
		if(ioctl(sockfd, SIOCGIFINDEX, &ifr) == -1){//bind to a interface 
			CWDebugLog("SIOCGIFINDEX error\n");
			CWWTPDebugLog("%s is not exit !", ifname);
			if(debug_print)
				printf("%s is not exit !\n", ifname);
			close(sockfd);
			extraInfoValues->wifi_status[i].status = 3;
			continue;
		}
		
		if (ioctl(sockfd, SIOCGIFFLAGS, &ifr) == -1){
			CWWTPDebugLog("SIOCGIFFLAGS error");
			if(debug_print)
				printf("SIOCGIFFLAGS error\n");
			close(sockfd);
			extraInfoValues->wifi_status[i].status = 3;
			continue;
		}
		if(ifr.ifr_flags & IFF_UP){
			if(debug_print)
				printf("interface UP\n");
			extraInfoValues->wifi_status[i].status = 1;
			close(sockfd);
		}else{
			if(debug_print)
				printf("interface DOWN\n");
			extraInfoValues->wifi_status[i].status = 2;
			close(sockfd);
		}
	}

	return CW_TRUE;
}

Neighbor_AP_INFO * CWGetApScanningResult(CWBool *bRet,unsigned int *neighborApCount)
{
	Neighbor_AP_INFO *neighborApInfo;

	char buf[24*1024];
	int sockfd = -1;
	int readBytes = 0;
	int type = 0;
	struct timeval timeout;
	fd_set readfds;
	CWProtocolMessage msgPtr;
	struct sockaddr_in addr_from;
	struct sockaddr_in serv_addr;
	
	sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if(sockfd < 0)
	{
		CWWTPDebugLog("Failed to create socket in ap scanning, continue");
		*bRet = CW_TRUE;//continue;
	}

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(5248);
	serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

	if(sendto(sockfd, &buf[0], 1, 0,(struct sockaddr *)&serv_addr, sizeof(struct sockaddr_in)) == -1)
	{
		CWWTPDebugLog("send error\n");
		close(sockfd);
		*bRet = CW_TRUE;//continue;
	}
	if(debug_print)
		printf("send request ok....................\n");
	
	FD_ZERO(&readfds);
	FD_SET(sockfd, &readfds);
	timeout.tv_sec = 15;
	timeout.tv_usec = 0;
	int ret = select(sockfd+1, &readfds, NULL, NULL, &timeout);
	if(ret < 0)
	{
		close(sockfd);
		if(debug_print)
			printf("select_ret=%d,select error\n",ret);
		CWWTPDebugLog("select error");
	}
	else if(ret == 0)
	{
		close(sockfd);
		if(debug_print)
			printf("select_ret=%d,receive time out\n",ret);
		CWWTPDebugLog("receive time out");
	}
	else
	{
		if(!CWErr(CWNetworkReceiveUnsafe(sockfd, buf, 24*1024, 0, (CWNetworkLev4Address *)&addr_from, &readBytes))) 
		{
			if (CWErrorGetLastErrorCode() == CW_ERROR_INTERRUPTED)
			{
				close(sockfd);
				*bRet = CW_TRUE;//continue;
			}
		}
		close(sockfd);
		if(debug_print)
			printf("receive a msg....................\n");
//			CWCaptrue(readBytes, buf);
		
		msgPtr.msg = buf;
		msgPtr.offset = 0;
		unsigned int ApCount = 0;
		int i;
		type= CWProtocolRetrieve8(&msgPtr);
		ApCount = CWProtocolRetrieve8(&msgPtr);
		if(debug_print)
			printf("type:%d   neighborapcount:%d\n", type, ApCount);
		*neighborApCount = ApCount;
		if( ApCount!= 0)
		{
			CW_CREATE_ARRAY_ERR(neighborApInfo, ApCount, Neighbor_AP_INFO, {CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL);printf("---6---goto exit_thread\n");*bRet = CW_FALSE;/*goto exit_thread;*/});
			for(i=0;i<ApCount;i++)
			{
				memset(&neighborApInfo[i], 0, sizeof(Neighbor_AP_INFO));
				
				//memcpy(neighborApInfo[i].BSSID, (unsigned char *)CWProtocolRetrieveRawBytes(&msgPtr,6), 6);
				/*
				 * BUG ML09
				 * 19/10/2009 - Donato Capitella
				 * 14/3/2011 - Pei Wenhui
				 */
				void *ptr = NULL;
				ptr =  (unsigned char *)CWProtocolRetrieveRawBytes(&msgPtr,6);
				CW_COPY_MEMORY(neighborApInfo[i].BSSID, ptr, 6);
				CW_FREE_OBJECT(ptr);
				neighborApInfo[i].Channel = CWProtocolRetrieve8(&msgPtr);
				neighborApInfo[i].RSSI = CWProtocolRetrieve8(&msgPtr);
				neighborApInfo[i].NOISE = CWProtocolRetrieve8(&msgPtr);
				neighborApInfo[i].BEACON_INT = CWProtocolRetrieve8(&msgPtr);
				neighborApInfo[i].Opstatus = NEW;
				msgPtr.offset +=1;
				neighborApInfo[i].EssidLen = CWProtocolRetrieve8(&msgPtr);

				CW_CREATE_STRING_ERR(neighborApInfo[i].ESSID, neighborApInfo[i].EssidLen, {CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL);CWWTPDebugLog("create memory error");*bRet = CW_FALSE;/*goto exit_thread;*/});
				memset(neighborApInfo[i].ESSID, 0 , neighborApInfo[i].EssidLen+1);
				//memcpy(neighborApInfo[i].ESSID, CWProtocolRetrieveRawBytes(&msgPtr, neighborApInfo[i].EssidLen), neighborApInfo[i].EssidLen);
				/*
				 * BUG ML09
				 * 19/10/2009 - Donato Capitella
				 * 14/3/2011 - Pei Wenhui
				 */
				ptr = NULL;
				ptr =  CWProtocolRetrieveRawBytes(&msgPtr, neighborApInfo[i].EssidLen);
				CW_COPY_MEMORY(neighborApInfo[i].ESSID, ptr, neighborApInfo[i].EssidLen);
				CW_FREE_OBJECT(ptr);
				//printf("%d---ESSID:%s\n", i, neighborApInfo[i].ESSID);
				
				neighborApInfo[i].IeLen = CWProtocolRetrieve8(&msgPtr);
				CW_CREATE_STRING_ERR(neighborApInfo[i].IEs_INFO, neighborApInfo[i].IeLen, {CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL);CWWTPDebugLog("create memory error");*bRet = CW_FALSE;/*goto exit_thread;*/});
				memset(neighborApInfo[i].IEs_INFO, 0 , neighborApInfo[i].IeLen+1);
				//memcpy(neighborApInfo[i].IEs_INFO, CWProtocolRetrieveRawBytes(&msgPtr, neighborApInfo[i].IeLen), neighborApInfo[i].IeLen);
				/*
				 * BUG ML09
				 * 19/10/2009 - Donato Capitella
				 * 14/3/2011 - Pei Wenhui
				 */
				ptr = NULL;
				ptr =  CWProtocolRetrieveRawBytes(&msgPtr, neighborApInfo[i].IeLen);
				CW_COPY_MEMORY(neighborApInfo[i].IEs_INFO, ptr, neighborApInfo[i].IeLen);
				CW_FREE_OBJECT(ptr);
				//printf("%d---IEs_INFO:%s\n", i, neighborApInfo[i].IEs_INFO);
				
				neighborApInfo[i].Rate = CWProtocolRetrieve16(&msgPtr);
				neighborApInfo[i].CapabilityInfo = CWProtocolRetrieve16(&msgPtr);
				neighborApInfo[i].next = NULL;
				printf("i=%d,neighborApInfo[i].Channel=%d\n",i,neighborApInfo[i].Channel);
			}
		}
	}
	*bRet = CW_TRUE;
	return neighborApInfo;
}

#if (defined( CAVIUM_PLATFORM )|| defined(AR11N))
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

#else

#if INT_MAX == LONG_MAX
static const char * const ss_fmt[] = {
	"%n%llu%u%u%u%u%n%n%n%llu%u%u%u%u%u",
	"%llu%llu%u%u%u%u%n%n%llu%llu%u%u%u%u%u",
	"%llu%llu%u%u%u%u%u%u%llu%llu%u%u%u%u%u%u"
};
#else
static const char * const ss_fmt[] = {
	"%n%llu%lu%lu%lu%lu%n%n%n%llu%lu%lu%lu%lu%lu",
	"%llu%llu%lu%lu%lu%lu%n%n%llu%llu%lu%lu%lu%lu%lu",
	"%llu%llu%lu%lu%lu%lu%lu%lu%llu%llu%lu%lu%lu%lu%lu%lu"
};
#endif
#endif
void get_dev_fields(char *bp, struct user_net_device_stats *wlan_stats, int procnetdev_vsn)
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
#if (defined( CAVIUM_PLATFORM )|| defined(AR11N))
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
#endif
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
unsigned long long  CWGet_tunnel_bytes_recv()
{
	char str[32];
	char temp_str[100];
	unsigned long long bytes_recv;

	memset(str,0,32);
	memset(temp_str, 0, 100);
	system("echo 0 > /proc/sys/net/capwap/bytes_recv");
	sprintf(temp_str, "cat /tmp/bytes_recv");
	PopenFile(temp_str, str, sizeof(str));
	bytes_recv = atoll(str);

	return bytes_recv;

}
unsigned long long  CWGet_tunnel_bytes_send()
{
	char str[32];
	char temp_str[100];
	unsigned long long bytes_send;

	memset(str,0,32);
	memset(temp_str, 0, 100);
	system("echo 0 > /proc/sys/net/capwap/bytes_send");
	sprintf(temp_str, "cat /tmp/bytes_send");
	PopenFile(temp_str,str, sizeof(str));
	bytes_send = atoll(str);

	return bytes_send;

}
CWBool CWGetCpuUseRatio(int *cpu_use_ratio)
{
	char temp_str[128];
	char str[24];
#if 0
	unsigned int total;
	float cpuratio;
   
	float user,user_old;
	float nice,nice_old;
	float system,system_old;
	float idle,idle_old;

	char cpu[21];
	char text[201];
	
	fp = fopen("/proc/stat", "r");
	while (fgets(text, 200, fp))
	{
		if (strstr(text, "cpu"))
		{
			sscanf(text, "%s %f %f %f %f", cpu, &user_old, &nice_old, &system_old, &idle_old);
		}
	}
	fclose(fp);
	sleep(2);
	fp = fopen("/proc/stat", "r");
	while (fgets(text, 200, fp))
	{
		if (strstr(text, "cpu"))
		{
			sscanf(text, "%s %f %f %f %f", cpu, &user, &nice, &system, &idle);
		}
	}
	fclose(fp);

	user -= user_old;
	nice -= nice_old;
	system -= system_old;
	idle -= idle_old;
	total = (user + nice + system + idle);
	if(total == 0)  //pei add 0416
	{
		*cpu_use_ratio = 0;
		return CW_TRUE;
	}
	user = (user / total) * 100;
	nice = (nice / total) * 100;
	system = (system / total) * 100;
	idle = (idle / total) * 100;

	cpuratio = 100-idle;
	*cpu_use_ratio = 10000-idle*100;
	
	if(debug_print)
		printf("cpu use ratio:%d     %4.2f%%\n", *cpu_use_ratio, cpuratio);
	//snprintf(cpu, 21, "%4.2f %4.2f %3.2f %4.2f", user, nice, system, idle);
//#else
        FILE *fp_cpu_info = NULL;
        char cmd_str[128];
	 char str_tmp[32];
        int cpu_use_ratio_tmp;
        memset(cmd_str, 0, 128);
	 sprintf(cmd_str,  "/usr/sbin/cpu_util_rate 3 |  awk -F  \"[:]\" '/Cpu Utilization/{print $2}'");
	 fp_cpu_info = popen(cmd_str, "r");
	 if(fp_cpu_info){
		memset(str_tmp, 0, 32);
		fgets(str_tmp, sizeof(str_tmp), fp_cpu_info);
		str_tmp[strlen(str_tmp)-1] = '\0';
		cpu_use_ratio_tmp=atoi(str_tmp);
              *cpu_use_ratio=cpu_use_ratio_tmp * 100;
		if(debug_print)
			printf("cpu_util_rate get cpu_use_ratio: %d  \n", *cpu_use_ratio);
		pclose(fp_cpu_info);
	 }
#else
	if(access("/tmp/cpu_ratio", 0)!=0)
	{
		system("/usr/sbin/cpu_util_rate 5");
	}
	memset(temp_str, 0, 128);
	sprintf(temp_str, "cat /tmp/cpu_ratio |awk 'NR==2 {print $0}'");
	PopenFile(temp_str,str, sizeof(str));	
	*cpu_use_ratio = atoi(str);
//		if(debug_print)
//			printf("AP get IP mode: %s len:%d\n", str, (int)strlen(str));
//	CWWTPDebugLog("cpu use ratio:%d", *cpu_use_ratio);
	if(debug_print)
		printf("cpu use ratio:%d\n", *cpu_use_ratio);
#endif

	return CW_TRUE;
}
int CWGetIPMode(void)
{
	char temp_str[100];
	char str[20];
	
	memset(temp_str, 0, 100);
	sprintf(temp_str, "awk -F \"[<>]\" '/proto/ {print $3}' /jffs/config.xml");
	PopenFile(temp_str,str, sizeof(str));
//		if(debug_print)
//			printf("AP get IP mode: %s len:%d\n", str, (int)strlen(str));
	if(!strcmp(str, "static"))
		return 0;

	return 1;
}
int CWGetTotalMem()
{
	char str[20];
	char temp_str[100];
	int mem_Total = 0;

	memset(str,0,20);
	memset(temp_str, 0, 100);
	sprintf(temp_str, "/usr/sbin/get_hardinfo | awk -F \"[:M]\" '/mem_size/ {print $2}'");
	PopenFile(temp_str, str, sizeof(str));	
	mem_Total = atoi(str);

	return mem_Total;

}
int CWGetFreeMem()
{
	char str[20];
	char temp_str[100];
	int memFree = 0;

	memset(str,0,20);
	memset(temp_str, 0, 100);
	sprintf(temp_str, "cat /proc/meminfo | awk -F \" \" '/MemFree/ {print $2}'");
	PopenFile(temp_str,str, sizeof(str));	
	memFree = atoi(str);

	return memFree;
}
int CWGetFlashConfigSize()
{
	char str[20];
	char temp_str[100];	
	int flashConfigFree = 0;	

	memset(str,0,20);
	memset(temp_str, 0, 100);
	sprintf(temp_str, "df | awk -F \" \" '/jffs/ {print $4}'");
	PopenFile(temp_str, str, sizeof(str));	
	flashConfigFree = atoi(str);

	return flashConfigFree;
}
int CWGetFlashTotalSize()
{
	char str[20];
	char temp_str[100];	
	int flashTotal = 0;

	memset(str,0,20);
	memset(temp_str, 0, 100);
	sprintf(temp_str, "/usr/sbin/get_hardinfo | awk -F \"[:M]\" '/flash_size/ {print $2}'");
	PopenFile(temp_str,str, sizeof(str));	
	flashTotal = atoi(str);

	return flashTotal;
}
unsigned char CWGetSnr(void)
{
	unsigned char snr = 0;
	CWWTPWlan *ptr = NULL;
	CWWTPWlan *oldptr = NULL;
	unsigned char radioId = 0;
	unsigned char wlanId = 0;
	unsigned char wlanFound = 0;
	char ifname[IFNAMSIZ];
	char temp_str[100];
	char str[20];
	
	for(ptr=wtp_wlan_list;ptr != NULL;oldptr = ptr,ptr = ptr->next)
	{
		if(ptr->wlan_id != 0)
		{
			radioId = ptr->radio_id;
			wlanId = ptr->wlan_id;
			wlanFound = 1;
			break;
		}
	}
	if(!wlanFound)
		return snr;
	
	CWWTPGetWlanName(ifname,radioId, wlanId);
	
	memset(temp_str, 0, 100);
	sprintf(temp_str, "iwconfig %s | awk -F \"[=/]\" '/Link Quality/ {print $2}'", ifname);
	PopenFile(temp_str, str, sizeof(str));	
	snr = atoi(str);
	if(debug_print)
		printf("AP get Snr:%d\n", snr);
	
	return snr;
}
char CWGetTemperature(void)
{
	char temperature = 0;
	char temp_str[100];
	char str[20];
	
	memset(temp_str, 0, 100);
	sprintf(temp_str, "avrutil -T | awk -F \" \" '/Temperature/ {print $3}'");
	PopenFile(temp_str,str,sizeof(str));
	temperature = atoi(str);
	if(debug_print)
		printf("AP get temperature:%d\n", temperature);
	
	return temperature;
}
CWBool CWGetInterfaceCount(int *count, char *type)
{
	char cmd_n[100];
	char bfr[10];
	
	memset(cmd_n, 0, 100);
	sprintf(cmd_n, "cat /proc/net/dev | awk -F \" \" '/%s:/' | wc -l", type);
	PopenFile(cmd_n,bfr, sizeof(bfr));
	*count = atoi(bfr);
	
	return CW_TRUE;
}
CWBool CWGetEthUpTime(int * ifUpTime,int eth_num)
{
	char temp_str[100];
	FILE *fp;
	memset(temp_str, 0, 100);

	sprintf(temp_str, "cat /tmp/link_count/link_up_count_eth%d",eth_num);
	fp = popen(temp_str, "r");
	if(fp)
	{
		char str[32];
		memset(str, 0, 32);
		fgets(str, sizeof(str), fp);
		//str[strlen(str)-1] = '\0';
		*ifUpTime = atoi(str);
		pclose(fp);
		return CW_TRUE;
	}

	return CW_FALSE;
}
CWBool CWGetEthdownTime(int * ifDownTime,int eth_num)
{
	char temp_str[100];
	FILE *fp;
	memset(temp_str, 0, 100);

	sprintf(temp_str, "cat /tmp/link_count/link_down_count_eth%d",eth_num);
	fp = popen(temp_str, "r");
	if(fp)
	{
		char str[32];
		memset(str, 0, 32);
		fgets(str, sizeof(str), fp);
		//str[strlen(str)-1] = '\0';
		*ifDownTime = atoi(str);
		pclose(fp);
		return CW_TRUE;
	}

	return CW_FALSE;
}
int CWGetRadioMaxCount()
{
	int radioMaxCount = 1;
	char temp_str[100];
	char str[20];
	FILE *fp;
	
	memset(temp_str, 0, 100);
	sprintf(temp_str, "grep \"<wifi_total>\" -C 1 /tmp/device_info.xml | awk -F '[><]' '/count/ {print $3}'");
	fp = popen(temp_str, "r");
	if(fp)
	{
		memset(str, 0, 20);
		fgets(str, sizeof(str), fp);
		str[strlen(str)-1] = '\0';
		radioMaxCount = atoi(str);
		pclose(fp);
	}
	if(radioMaxCount == 0)
		radioMaxCount = 1;
	
//	if(debug_print)
//		printf("AP get radioMaxCount:%d\n", radioMaxCount);
	
	return radioMaxCount;
}
CWBool CWGetAPOnlineTime(unsigned short *OnTime) 
{	
	char cmd_time[128];
	char str_time[24];
	*OnTime = 0;
/*
	memset(cmd_time, 0, 128);
	sprintf(cmd_time, "date | awk '{print $4}' | awk -F \":\" '{print $1}'");
	fp_time = popen(cmd_time, "r");
	if(fp_time)
	{
		memset(str_time, 0, 10);
		fgets(str_time, sizeof(str_time), fp_time);
		*OnTime += atoi(str_time)*60*60;
		pclose(fp_time);
	}
	memset(cmd_time, 0, 128);
	sprintf(cmd_time, "date | awk '{print $4}' | awk -F \":\" '{print $2}'");
	fp_time = popen(cmd_time, "r");
	if(fp_time)
	{
		memset(str_time, 0, 10);
		fgets(str_time, sizeof(str_time), fp_time);
		*OnTime += atoi(str_time)*60;
		pclose(fp_time);
	}
	memset(cmd_time, 0, 128);
	sprintf(cmd_time, "date | awk '{print $4}' | awk -F \":\" '{print $3}'");
	fp_time = popen(cmd_time, "r");
	if(fp_time)
	{
		memset(str_time, 0, 10);
		fgets(str_time, sizeof(str_time), fp_time);
		*OnTime += atoi(str_time);
		pclose(fp_time);
	}
*/
	
	memset(cmd_time, 0, 128);
	sprintf(cmd_time, "cat /proc/uptime | awk -F ' ' '{print $1}' | awk -F '.' '{print $1}'");
	PopenFile(cmd_time,str_time, sizeof(str_time));
	*OnTime = atoi(str_time);
	//printf("sum of *OnTime:%d\n", *OnTime);
	
	return CW_TRUE;
}
CWBool CWGetAPIsColdBoot(unsigned char *IsColdBoot)
{
	if(0 ==access("/jffs/sysrebootflag", 0))
	{
		//printf("The file is exit!\n");
		*IsColdBoot = 0;
		system("rm /jffs/sysrebootflag");
	}
	//else
		//printf("The file is not exit!\n");
	
	return CW_TRUE;
}
CWBool CWGetAPIPInfo(APIPInfoValues *ipInfo)
{
	char cmd_str[128];
	char str[24];
	char str_tmp[24];
	unsigned int netmask, gw, dnsNum, dns;
	
	memset(cmd_str, 0, 128);
	sprintf(cmd_str, "ifconfig default | awk -F \" \" '/Mask/ {print $4}' | awk -F \":\" '{print $2}'");
	PopenFile(cmd_str,str_tmp, sizeof(str_tmp));
	memset(str, 0, 24);
	strcpy(str, trim(str_tmp));
	if(inet_atoi(str, &netmask) == 1)
		ipInfo->netmask = netmask;
	CWWTPDebugLog("netmask: %s len:%d", str, (int)strlen(str));

	memset(cmd_str, 0, 128);
	sprintf(cmd_str, "route -n | awk -F \" \" '/UG/ {print $2}'");
	PopenFile(cmd_str,str_tmp, sizeof(str_tmp));
	memset(str, 0, 24);
	strcpy(str, trim(str_tmp));
	if(inet_atoi(str, &gw) == 1)
		ipInfo->gateway = gw;
	CWWTPDebugLog("gw: %s len:%d", str, (int)strlen(str));

	memset(cmd_str, 0, 128);
	sprintf(cmd_str, "cat /etc/resolv.conf | grep nameserver | wc -l");
	PopenFile(cmd_str,str_tmp, sizeof(str_tmp));
	memset(str, 0, 24);
	strcpy(str, trim(str_tmp));
	dnsNum = atoi(str);
//		if(debug_print)
//			printf("dnsNum: %s len:%d    dnsNum:%u\n", str, (int)strlen(str), dnsNum);
	
	dns = 0;
	if(dnsNum >= 1)
	{
		memset(str, 0, 24);
		memset(cmd_str, 0, 128);
		sprintf(cmd_str, "cat /etc/resolv.conf | grep nameserver | awk '{if(NR==1){print $2}}'");
		PopenFile(cmd_str,str_tmp, sizeof(str_tmp));
		
		strcpy(str, trim(str_tmp));
		if(inet_atoi(str, &dns) == 1)
			ipInfo->dns = dns;
		CWWTPDebugLog("dns: %s len:%d", str, (int)strlen(str));
		if(debug_print)
			printf("dns: %s len:%d\n", str, (int)strlen(str));
	}
	dns = 0;
	if(dnsNum == 2)
	{
		memset(str, 0, 24);
		memset(cmd_str, 0, 128);
		sprintf(cmd_str, "cat /etc/resolv.conf | grep nameserver | awk '{if(NR==2){print $2}}'");
		PopenFile(cmd_str,str_tmp, sizeof(str_tmp));
		strcpy(str, trim(str_tmp));
		if(inet_atoi(str, &dns) == 1)
			ipInfo->vice_dns = dns;
		CWWTPDebugLog("vice_dns: %s len:%d", str, (int)strlen(str));
		if(debug_print)
			printf("vice_dns: %s len:%d\n", str, (int)strlen(str));
	}
	return CW_TRUE;
}
CWBool CWGetAPhardInfo(ApHardInfoValues *hardInfo)
{
	char cmd_str[128];
	char str_tmp[32];
	hardInfo->eth_num=1;
	hardInfo->eth_up_speed=100;
	
	memset(cmd_str, 0, 128);
	sprintf(cmd_str, "/usr/sbin/get_hardinfo | awk -F \":\" '/cpu_type/ {print $2}'");
	PopenFile(cmd_str,str_tmp, sizeof(str_tmp));
	memcpy(hardInfo->WTPCPUTYPE,str_tmp,strlen(str_tmp));
	if(debug_print)
		printf("cpu_type: %s len:%d   \n", str_tmp, (int)strlen(str_tmp));
	memset(cmd_str, 0, 128);
	sprintf(cmd_str, "/usr/sbin/get_hardinfo | awk -F \":\" '/flash_type/ {print $2}'");
	PopenFile(cmd_str,str_tmp, sizeof(str_tmp));
	memcpy(hardInfo->WTPFLASHTYPE,str_tmp,strlen(str_tmp));
	if(debug_print)
		printf("fp_flash_type: %s len:%d  \n", str_tmp, (int)strlen(str_tmp));
#if 0
		memset(cmd_str, 0, 128);
	sprintf(cmd_str,  "/usr/sbin/get_hardinfo | awk -F \"[:M]\" '/flash_size/ {print $2}'");
	fp_flash_info = popen(cmd_str, "r");
	if(fp_flash_info)
		memset(str_tmp, 0, 32);
		fgets(str_tmp, sizeof(str_tmp), fp_flash_info);
		str_tmp[strlen(str_tmp)-1] = '\0';
		hardInfo->flashsize=atoi(str_tmp);
		if(debug_print)
			printf("flash_size: %d  \n", hardInfo->flashsize);
		pclose(fp_flash_info);
#endif

	memset(cmd_str, 0, 128);
	sprintf(cmd_str,  "/usr/sbin/get_hardinfo | awk -F \":\" '/mem_type/ {print $2}'");
	PopenFile(cmd_str,str_tmp, sizeof(str_tmp));
	memcpy(hardInfo->WTPMEMTYPE,str_tmp,strlen(str_tmp));
	if(debug_print)
		printf("fp_mem_type: %s len:%d    \n", str_tmp, (int)strlen(str_tmp));
#if 0	
	memset(cmd_str, 0, 128);
	sprintf(cmd_str,  "/usr/sbin/get_hardinfo | awk -F \"[:M]\" '/mem_size/ {print $2}'");
	fp_mem_info = popen(cmd_str, "r");
	if(fp_mem_info)
		memset(str_tmp, 0, 32);
		fgets(str_tmp, sizeof(str_tmp), fp_mem_info);
		str_tmp[strlen(str_tmp)-1] = '\0';
		hardInfo->memsize=atoi(str_tmp);
		if(debug_print)
			printf("mem_size: %d \n", hardInfo->memsize);
		pclose(fp_mem_info);
#endif
	memset(cmd_str, 0, 128);
	sprintf(cmd_str,  "/usr/sbin/get_hardinfo | awk -F \":\" '/eth_num/ {print $2}'");
	PopenFile(cmd_str,str_tmp, sizeof(str_tmp));
	hardInfo->eth_num=atoi(str_tmp);
	if(debug_print)
		printf("eth_num: %d  \n", hardInfo->eth_num);

	memset(cmd_str, 0, 128);
	sprintf(cmd_str,  "/usr/sbin/get_eth_speed| awk -F \"[:M]\" '{print $2}'");
	PopenFile(cmd_str,str_tmp, sizeof(str_tmp));
	hardInfo->eth_up_speed=atoi(str_tmp);
	if(debug_print)
		printf("eth_up_speed: %d  \n", hardInfo->eth_up_speed);
#ifdef AP2400_IFM
	int speed_tmp=0;
	memset(cmd_str, 0, 128);
	sprintf(cmd_str,  "/usr/bin/ethreg eth0 |awk -F \"x\" '{print $3}'");
	PopenFile(cmd_str,str_tmp, sizeof(str_tmp));
	speed_tmp=atoi(str_tmp);
	switch(speed_tmp){
		case 0:
			hardInfo->eth_up_speed=10;
			break;
		case 1:
			hardInfo->eth_up_speed=100;
			break;
		case 2:
			hardInfo->eth_up_speed=1000;
			break;
		default:
			hardInfo->eth_up_speed=1000;
			break;
	}
	if(debug_print)
		printf("eth_up_speed: %d  \n", hardInfo->eth_up_speed);
#endif	
#if (defined(AQ2000N)||defined(APXJ))
	int speed_tmp=0;
	memset(cmd_str, 0, 128);
	sprintf(cmd_str,  "/usr/bin/ethreg -p 2 offset |awk -F \"x\" '{print $3}'");
	PopenFile(cmd_str,str_tmp, sizeof(str_tmp));
	speed_tmp=atoi(str_tmp);
	switch(speed_tmp){
		case 1000:
			hardInfo->eth_up_speed=100;
			break;
		default:
			hardInfo->eth_up_speed=10;
			break;
	}
	if(debug_print)
		printf("eth_up_speed: %d  \n", hardInfo->eth_up_speed);
#endif	
	return CW_TRUE;
}

void * CWParseNetLinkBuffer( NetLink_Type *netLink_Type,char * data,int len,unsigned char *mac)
{
	struct iw_event iwe_buf, *iwe = &iwe_buf;
	char *pos, *end, *custom;
	int i = 0;
	unsigned char vap_mac[MAC_ADDR_LEN];
	terminalDisturbInfo *terminalDisturbValues;
	unsigned char wireless_type = 0;
	static unsigned char tbuf[128];
	
	/* pei add start, for wapi attack addr redirection trap */
	CWWTPWlan *ptr = NULL;
	asso_mt *asso_mt_info ;
	attackAddrRedirectionInfo *attackAddrRdValues;
	staThroughputsendtoAc *staThpvalues;
	challengeReplayInfo *challengeReplayValues;
	const u16 ADDR_RD_TRAP = 0x00F8;  /* 地址重定向*/
	const u16 CHALLENGE_REPLAY_TRAP = 0x00FF;
	asso_mt_info = (asso_mt *)(data+IW_EV_POINT_LEN);

	for(ptr=wtp_wlan_list;ptr != NULL;ptr = ptr->next)
	{
		if ((0 == memcmp(ptr->wlan_bssid, asso_mt_info->ap_mac, 6))&&(asso_mt_info->type == ADDR_RD_TRAP))
		{
			if(debug_print)
				printf("attack addr redirection [%02x-%02x-%02x-%02x-%02x-%02x]\n", asso_mt_info->mac[0], 
				asso_mt_info->mac[1], asso_mt_info->mac[2], asso_mt_info->mac[3], asso_mt_info->mac[4],
				asso_mt_info->mac[5]);
			CW_CREATE_OBJECT_ERR(attackAddrRdValues,attackAddrRedirectionInfo, CWErrorRaise(CW_ERROR_OUT_OF_MEMORY,NULL);)
			memset(attackAddrRdValues,0,sizeof(attackAddrRedirectionInfo));
			(*attackAddrRdValues).value = ATTACK_ADDR_RD;
			(*attackAddrRdValues).count = 1;
			(*attackAddrRdValues).radio_id = ptr->radio_id;
			(*attackAddrRdValues).wlan_id = ptr->wlan_id;
			(*attackAddrRdValues).state = 1;
			memcpy((*attackAddrRdValues).sta_mac, asso_mt_info->mac, MAC_ADDR_LEN);
			*netLink_Type = NT_T_AttackAddrRedirection;
			return attackAddrRdValues;
			//CWWTPGetAttackAddrRedirection(&attackAddrRdValues);
		}
		if ((0 == memcmp(ptr->wlan_bssid, asso_mt_info->ap_mac, 6))&&(asso_mt_info->type == CHALLENGE_REPLAY_TRAP))
		{
			if(debug_print)
				printf("challenge replay [%02x-%02x-%02x-%02x-%02x-%02x]\n", asso_mt_info->mac[0], 
				asso_mt_info->mac[1], asso_mt_info->mac[2], asso_mt_info->mac[3], asso_mt_info->mac[4],
				asso_mt_info->mac[5]);
			CW_CREATE_OBJECT_ERR(challengeReplayValues,challengeReplayInfo, CWErrorRaise(CW_ERROR_OUT_OF_MEMORY,NULL);)
			memset(challengeReplayValues,0,sizeof(challengeReplayInfo));
			(*challengeReplayValues).value = CHALLENGE_REPLAY;
			(*challengeReplayValues).count = 1;
			(*challengeReplayValues).radio_id = ptr->radio_id;
			(*challengeReplayValues).wlan_id = ptr->wlan_id;
			(*challengeReplayValues).state = 1;
			memcpy((*challengeReplayValues).sta_mac, asso_mt_info->mac, MAC_ADDR_LEN);
			*netLink_Type = NT_T_ChallengeReplay;
			return challengeReplayValues;
			//CWWTPGetChallengeReplay(&challengeReplayValues);
		}
	}
	pos = data;
	end = data + len;

//	printf("-----entering the driver_madwifi_wireless_event_wireless()\n");
	while (pos + IW_EV_LCP_LEN <= end) {
		/* Event data may be unaligned, so make a local, aligned copy
		 * before processing. */
		memcpy(&iwe_buf, pos, IW_EV_LCP_LEN);
		CWDebugLog("Wireless event: "
				  "cmd=0x%x len=%d\n", iwe->cmd, iwe->len);
		if (iwe->len <= IW_EV_LCP_LEN)
			return NULL;

		custom = pos + IW_EV_POINT_LEN;
		
		memcpy(&iwe_buf, pos, sizeof(struct iw_event));
		custom += IW_EV_POINT_OFF;

		switch (iwe->cmd) {
			case IWEVEXPIRED:
				CWDebugLog("station aging!\n");
				memcpy(mac, (unsigned char *)iwe->u.addr.sa_data, 6);
				*netLink_Type = NT_T_StationAging;
				return NULL ;
//				madwifi_del_sta(drv, (u8 *) iwe->u.addr.sa_data);
			break;
			case IWEVTRAFFIC:
				CW_CREATE_OBJECT_ERR(staThpvalues,staThroughputsendtoAc, CWErrorRaise(CW_ERROR_OUT_OF_MEMORY,NULL);)
				CWWTPDebugLog("station up/down line");
				memset(staThpvalues,0,sizeof(staThpvalues));
				(*staThpvalues).optype = STATHROUGHPUT_OPTYPE;
				(*staThpvalues).len = 33;
				memcpy((*staThpvalues).sta_mac, (unsigned char *)iwe->u.addr.sa_data, 6);
				dpf(MAC_FMT"up/down line\n",MAC2STR((*staThpvalues).sta_mac));
				if(!(get_sta_throughput_info(staThpvalues)))
					break;
				*netLink_Type = NT_T_Sta_Traffic_Limit;
				return staThpvalues;
			case 0x8c0a:  /* pei add for attack detect, at 090625 */
				if(debug_print)
				{
					printf("iwe->len:%d   data-len:%d\n", iwe->len, iwe->u.data.length);
					printf("data:");
					for(i=0;i<iwe->len;i++)
						printf("%02x ", (unsigned char)*(pos+i));
					printf("\n");
				}
				wireless_type = (unsigned char)*(pos+IW_EV_POINT_LEN);
				if(debug_print)
					printf("wireless_type:%d\n", wireless_type);
				switch(wireless_type)
				{
					case IEEE80211_STA_INTERFER_DETECT:
						memset(vap_mac, 0, MAC_ADDR_LEN);
						CW_CREATE_OBJECT_ERR(terminalDisturbValues,terminalDisturbInfo, CWErrorRaise(CW_ERROR_OUT_OF_MEMORY,NULL);)
						memset(terminalDisturbValues, 0, sizeof(terminalDisturbInfo));
						(*terminalDisturbValues).value = TERMINAL_DISTURB;
						(*terminalDisturbValues).state = 1;
						(*terminalDisturbValues).sta_count = (unsigned char)*(pos+IW_EV_POINT_LEN+2);
						CW_CREATE_ARRAY_ERR((*terminalDisturbValues).sta_mac, MAC_ADDR_LEN*(*terminalDisturbValues).sta_count, unsigned char, CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
						memcpy((*terminalDisturbValues).sta_mac, pos+IW_EV_POINT_LEN+3, MAC_ADDR_LEN*(*terminalDisturbValues).sta_count);
						memcpy(vap_mac, pos+IW_EV_POINT_LEN+3+MAC_ADDR_LEN*(*terminalDisturbValues).sta_count, MAC_ADDR_LEN);
						for(ptr=wtp_wlan_list;ptr != NULL;ptr = ptr->next)
						{
							if (0 == memcmp(ptr->wlan_bssid, vap_mac, 6))
							{
								(*terminalDisturbValues).radio_id = ptr->radio_id;
								(*terminalDisturbValues).wlan_id = ptr->wlan_id;
							}
						}
						memset(tbuf, 0, 128);
						tbuf[0] = (*terminalDisturbValues).sta_count;
						memcpy(tbuf+1, pos+IW_EV_POINT_LEN+3, MAC_ADDR_LEN*(*terminalDisturbValues).sta_count);
						for(i=0;i<MAC_ADDR_LEN*tbuf[0]+1;i++)
						{
							printf("%02x", tbuf[i]);
						}
						printf("\n");
						*netLink_Type = NT_T_TerminalDisturbDetect;
						return terminalDisturbValues;
//diao commet						CWWTPGetTerminalDisturb(&terminalDisturbValues);
					//	CW_FREE_OBJECT(terminalDisturbValues.sta_mac);
					break;
					case IEEE80211_STA_INTERFER_CLEAR:
						memset(vap_mac, 0, MAC_ADDR_LEN);
						CW_CREATE_OBJECT_ERR(terminalDisturbValues,terminalDisturbInfo, CWErrorRaise(CW_ERROR_OUT_OF_MEMORY,NULL);)
						memset(terminalDisturbValues, 0, sizeof(terminalDisturbInfo));
						(*terminalDisturbValues).value = TERMINAL_DISTURB;
						(*terminalDisturbValues).state = 0;
						for(i=0;i<MAC_ADDR_LEN*tbuf[0]+1;i++)
						{
							printf("%02x", tbuf[i]);
						}
						printf("\n");
						(*terminalDisturbValues).sta_count = tbuf[0];
						CW_CREATE_ARRAY_ERR((*terminalDisturbValues).sta_mac, MAC_ADDR_LEN*(*terminalDisturbValues).sta_count, unsigned char, CWErrorRaise(CW_ERROR_OUT_OF_MEMORY, NULL););
						memcpy((*terminalDisturbValues).sta_mac, tbuf+1, MAC_ADDR_LEN*(*terminalDisturbValues).sta_count);
						memcpy(vap_mac, pos+IW_EV_POINT_LEN+2, MAC_ADDR_LEN);
						for(ptr=wtp_wlan_list;ptr != NULL;ptr = ptr->next)
						{
							if (0 == memcmp(ptr->wlan_bssid, vap_mac, 6))
							{
								(*terminalDisturbValues).radio_id = ptr->radio_id;
								(*terminalDisturbValues).wlan_id = ptr->wlan_id;
							}
						}
						*netLink_Type = NT_T_TerminalDisturbClear;
						return terminalDisturbValues;
				//		CWWTPGetTerminalDisturb(&terminalDisturbValues);
				//		CW_FREE_OBJECT(terminalDisturbValues.sta_mac);
					break;
					default:
					break;
				}

				if(debug_print)
				{
					printf("iwe->len:%d   data-len:%d\n", iwe->len, iwe->u.data.length);
					printf("data:");
					//for(i=0;i<iwe->len;i++)
					//	printf("%02x ", (unsigned char)*(pos+i));
					//printf("\n");
				}
				unsigned char buf[16];
				for(i=0;i<16;i++)
				{
					buf[i] = (unsigned char)*(pos+iwe->len-16+i);
					if(debug_print)
						printf("%02x ", (unsigned char)*(pos+iwe->len-16+i));
				}
				if(debug_print)
					printf("\n");
				CWWTPDebugLog("data-len:%d", iwe->u.data.length);
				CWWTPDebugLog("data:%02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x",
					buf[0], buf[1], buf[2], buf[3], buf[4], buf[5], buf[6], buf[7], buf[8], buf[9], buf[10], buf[11], buf[12], buf[13], buf[14], buf[15]);
				

				{
					attackDetectInfo *attackDetectInfoValues;
					CW_CREATE_OBJECT_ERR(attackDetectInfoValues,attackDetectInfo, CWErrorRaise(CW_ERROR_OUT_OF_MEMORY,NULL);)
					memset(attackDetectInfoValues,0,sizeof(attackDetectInfo));
					(*attackDetectInfoValues).value = ATTACK_DETECT_INFO;
					(*attackDetectInfoValues).count = 0;
					(*attackDetectInfoValues).is_flood_attack = 0;
					(*attackDetectInfoValues).is_spoof_attack = 0;
					(*attackDetectInfoValues).is_weakiv_attack = 0;
					//if(gFloodingDetectEnable||gSpoofingDetectEnable||gWeakIVDetectEnable)
					if(gFloodingDetectEnable&&(buf[0]) == 1)
					{
						(*attackDetectInfoValues).count += 1;
						(*attackDetectInfoValues).is_flood_attack = 1;
						(*attackDetectInfoValues).flood_detect_type = IEEE80211_WIDS_FLOOD_DETECT;
						memcpy((*attackDetectInfoValues).flood_sta_mac, &buf[1], 6);
						memcpy((*attackDetectInfoValues).flood_vap_mac, &buf[7], 6);
						(*attackDetectInfoValues).flood_frame_type = buf[13];
						(*attackDetectInfoValues).flood_channel = buf[14];
						(*attackDetectInfoValues).flood_rssi = buf[15];
					}
					if(gSpoofingDetectEnable&&(buf[0]) == 2)
					{
						(*attackDetectInfoValues).count += 1;
						(*attackDetectInfoValues).is_spoof_attack = 1;
						(*attackDetectInfoValues).spoof_detect_type = IEEE80211_WIDS_SPOOF_DETECT;
						memcpy((*attackDetectInfoValues).spoof_sta_mac, &buf[1], 6);
						memcpy((*attackDetectInfoValues).spoof_vap_mac, &buf[7], 6);
						(*attackDetectInfoValues).spoof_frame_type = buf[13];
						(*attackDetectInfoValues).spoof_channel = buf[14];
						(*attackDetectInfoValues).spoof_rssi = buf[15];
					}
					if(gWeakIVDetectEnable&&(buf[0]) == 3)
					{
						(*attackDetectInfoValues).count += 1;
						(*attackDetectInfoValues).is_weakiv_attack = 1;
						(*attackDetectInfoValues).weakiv_detect_type = IEEE80211_WIDS_WEAKIV_DETECT;
						memcpy((*attackDetectInfoValues).weakiv_sta_mac, &buf[1], 6);
						memcpy((*attackDetectInfoValues).weakiv_vap_mac, &buf[7], 6);
						(*attackDetectInfoValues).weakiv_payload = buf[13];
						(*attackDetectInfoValues).weakiv_channel = buf[14];
						(*attackDetectInfoValues).weakiv_rssi = buf[15];
					}
					*netLink_Type = NT_T_AttackDetect;
					return attackDetectInfoValues;
					//if(attackDetectInfoValues.count != 0)
					//	CWWTPGetAttackDetectInfo(&attackDetectInfoValues);
				}
				
			break;
			default:
			break;
		}

		pos += iwe->len;
	}
	return NULL;
}





