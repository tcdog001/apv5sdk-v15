/*
 *  Copyright (c) 2010 Atheros Communications Inc.
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */
 #ifdef DOT11_WAPI_MIB
 
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>	 
#include <unistd.h>
#include <stdarg.h>
#include <sys/stat.h>
#include <sys/un.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/if.h>
#include <linux/wireless.h>

#include "pack.h"
#include "auth.h"
#include "typedef.h"
#include "ctrl_iface.h"
#include "cert_info.h"
#include "debug.h"
#include "proc.h"
#include "init.h"
#include "wai_sta.h"
#include "key_neg.h"
#include "dot11wapiMib.h"
#include "ieee80211_external.h"


extern struct auth_sta_info_t * ap_get_sta_by_index(int index, apdata_info *pap);
extern int ap_get_sta_num(apdata_info *pap);

const u8 AKM_CRT[4]={0x00, 0x14, 0x72, 0x01};
const u8 AKM_PSK[4]={0x00, 0x14, 0x72, 0x02};
const u8 WPI_SMS4[4] = {0x00, 0x14, 0x72, 0x01};


void wapimib_config_init(struct Gb15629dot11wapiConfigEntry* entry)
{
	memset(entry,0,sizeof(struct Gb15629dot11wapiConfigEntry));
	entry->ConfigVersion=1;
	entry->ControlledAuthControl=1;//enable auth
	entry->ControlledPortControl=0;//auto
	entry->OptionalImplemented=1;//support wapi
	entry->PreauthenticationImplemented=0;// not support preauth currently
	entry->Enabled=1; //activate WAPI
	entry->PreauthenticationEnabled=0;
	entry->ConfigUnicastKeysSupported=32;
	entry->ConfigUnicastRekeyMethod=2;//timebased/
	entry->ConfigUnicastRekeyTime=86400;
	entry->ConfigUnicastRekeyPackets=0;
	memcpy(entry->ConfigMulticastCipher,WPI_SMS4,4);
	entry->ConfigMulticastRekeyMethod=2;//timebased
	entry->ConfigMulticastRekeyTime=86400;
	entry->ConfigMulticastRekeyPackets=0;
	entry->ConfigMulticastRekeyStrict=0;
	//entry->ConfigPSKValue
	//entry->ConfigPSKPassPhrase
	entry->ConfigCertificateUpdateCount=3;
	entry->ConfigMulticastUpdateCount=3;
	entry->ConfigUnicastUpdateCount=3;
	entry->ConfigMulticastCipherSize=256;
	entry->ConfigBKLifetime=43200;
	entry->ConfigBKReauthThreshold=70;
	entry->ConfigSATimeout=60;
	//entry->AuthenticationSuiteSelected
	memcpy(entry->UnicastCipherSelected,WPI_SMS4,4);
	memcpy(entry->MulticastCipherSelected,WPI_SMS4,4);
	//entry->BKIDUsed
	//entry->AuthenticationSuiteRequested
	memcpy(entry->UnicastCipherRequested,WPI_SMS4,4);
	memcpy(entry->MulticastCipherRequested, WPI_SMS4,4);
	
}


int wapimib_config_get(struct Gb15629dot11wapiConfigEntry* entry, int index ,char* buf)
{
	int datalen=sizeof(int);
	switch (index)
	{	case 0:
			datalen=sizeof(struct Gb15629dot11wapiConfigEntry);
			memcpy( buf,entry,datalen);
			break;
		case WAPIMIB_CONFENTRY_VERSION:
			memcpy(buf,&entry->ConfigVersion, sizeof(int));
			break;
		case WAPIMIB_CONFENTRY_AUTHCTL:
			memcpy(buf,&entry->ControlledAuthControl, sizeof(int));
			break;
		case WAPIMIB_CONFENTRY_PORTCTL:
			memcpy(buf,&entry->ControlledPortControl, sizeof(int));
			break;
		case WAPIMIB_CONFENTRY_OPTIONAL_IMP:
			memcpy(buf,&entry->OptionalImplemented, sizeof(int));
			break;
		case WAPIMIB_CONFENTRY_PREAUTH_IMP:
			memcpy(buf,&entry->PreauthenticationImplemented, sizeof(int));
			break;
		case WAPIMIB_CONFENTRY_ENABLE:
			memcpy(buf,&entry->Enabled, sizeof(int));
			break;
		case WAPIMIB_CONFENTRY_PREAUTH_ENABLE:
			memcpy(buf,&entry->PreauthenticationEnabled, sizeof(int));
			break;
		case WAPIMIB_CONFENTRY_UKEY_SUPPORT:
			memcpy(buf,&entry->ConfigUnicastKeysSupported, sizeof(int));
			break;
		case WAPIMIB_CONFENTRY_UREKEY_METHOD:
			memcpy(buf,&entry->ConfigUnicastRekeyMethod, sizeof(int));
			break;
		case WAPIMIB_CONFENTRY_UREKEY_TIME:
			memcpy(buf,&entry->ConfigUnicastRekeyTime, sizeof(int));
			break;
		case WAPIMIB_CONFENTRY_UREKEY_PACKETS:
			memcpy(buf,&entry->ConfigUnicastRekeyPackets, sizeof(int));
			break;
		case WAPIMIB_CONFENTRY_MCAST_CIPHER:
			memcpy(buf,entry->ConfigMulticastCipher, 4);
			break;
		case WAPIMIB_CONFENTRY_MREKEY_METHOD:
			memcpy(buf,&entry->ConfigMulticastRekeyMethod, sizeof(int));
			break;
		case WAPIMIB_CONFENTRY_MREKEY_TIME:
			memcpy(buf,&entry->ConfigMulticastRekeyTime, sizeof(int));
			break;
		case WAPIMIB_CONFENTRY_MREKEY_PACKETS:
			memcpy(buf,&entry->ConfigMulticastRekeyPackets, sizeof(int));
			break;
		case WAPIMIB_CONFENTRY_MREKEY_STRICT:
			memcpy(buf,&entry->ConfigMulticastRekeyStrict, sizeof(int));
			break;
		case WAPIMIB_CONFENTRY_PSK_VALUE:
			datalen=32;
			memcpy(buf,entry->ConfigPSKValue, 32);
			break;
		case WAPIMIB_CONFENTRY_PSK_PASSPHRASE:
			datalen=128;
			memcpy(buf,entry->ConfigPSKPassPhrase, 128);
			break;
		case WAPIMIB_CONFENTRY_CERT_UPDATECNT:
			memcpy(buf,&entry->ConfigCertificateUpdateCount, sizeof(int));
			break;
		case WAPIMIB_CONFENTRY_MCAST_UPDATECNT:
			memcpy(buf,&entry->ConfigMulticastUpdateCount, sizeof(int));
			break;
		case WAPIMIB_CONFENTRY_UCAST_UPDATECNT:
			memcpy(buf,&entry->ConfigUnicastUpdateCount, sizeof(int));
			break;
		case WAPIMIB_CONFENTRY_MCAST_CIPHER_SIZE:
			memcpy(buf,&entry->ConfigMulticastCipherSize, sizeof(int));
			break;
		case WAPIMIB_CONFENTRY_BK_LIFETIME:
			memcpy(buf,&entry->ConfigBKLifetime, sizeof(int));
			break;
		case WAPIMIB_CONFENTRY_BK_REAUTH_THRESH:
			memcpy(buf,&entry->ConfigBKReauthThreshold, sizeof(int));
			break;
		case WAPIMIB_CONFENTRY_SA_TIMEOUT:
			memcpy(buf,&entry->ConfigSATimeout, sizeof(int));
			break;
		case WAPIMIB_CONFENTRY_AUTHSUITE_SEL:
			memcpy(buf,entry->AuthenticationSuiteSelected, 4);
			break;
		case WAPIMIB_CONFENTRY_UCIPHER_SEL:
			memcpy(buf,entry->UnicastCipherSelected, 4);
			break;
		case WAPIMIB_CONFENTRY_MCIPHER_SEL:
			memcpy(buf,entry->MulticastCipherSelected, 4);
			break;
		case WAPIMIB_CONFENTRY_BKID_USED:
			datalen=16;
			memcpy(buf,entry->BKIDUsed, 16);
			break;
		case WAPIMIB_CONFENTRY_AUTHSUITE_REQ:
			memcpy(buf,entry->AuthenticationSuiteRequested, 4);
			break;
		case WAPIMIB_CONFENTRY_UCIPHER_REQ:
			memcpy(buf,entry->UnicastCipherRequested, 4);
			break;
		case WAPIMIB_CONFENTRY_MCIPHER_REQ:
			memcpy(buf,entry->MulticastCipherRequested, 4);
			break;
		default:
			return 0;

	}

	return datalen;
}



void wapimib_ucipher_init(struct Gb15629dot11wapiConfigUnicastCiphersEntry* entry)
{
	memset(entry,0,sizeof(struct Gb15629dot11wapiConfigUnicastCiphersEntry));
	entry->ConfigUnicastCipherIndex=1;
	memcpy(entry->ConfigUnicastCipher,WPI_SMS4,4);
	entry->ConfigUnicastCipherEnabled=0;
	entry->ConfigUnicastCipherSize=256;
}


void wapimib_asuites_init(struct Gb15629dot11wapiConfigAuthenticationSuitesEntry* entry)
{
	memset(entry,0,sizeof(struct Gb15629dot11wapiConfigAuthenticationSuitesEntry));
	entry->ConfigAuthenticationSuiteIndex=1;
	memcpy(entry->ConfigAuthenticationSuite,AKM_CRT,4);
	entry->ConfigAuthenticationSuiteEnabled=0;

	entry++;
	entry->ConfigAuthenticationSuiteIndex=2;
	memcpy(entry->ConfigAuthenticationSuite,AKM_PSK,4);
	entry->ConfigAuthenticationSuiteEnabled=0;
	
}

void wapimib_stats_init(struct Gb15629dot11wapiStatsEntry* entry)
{
	memset(entry,0,sizeof(struct Gb15629dot11wapiStatsEntry));
	//config->StatsSTAAddress
	entry->StatsVersion=1;
	entry->StatsControlledPortStatus=0;
	memcpy(entry->StatsSelectedUnicastCipher,WPI_SMS4,4);
	//config->StatsWPIReplayCounters
	//config->StatsWPIDecryptableErrors
	//config->StatsWPIMICErrors
}


//China Mobile Trap definition
/*5.1.3.1*/
void wapimib_trap_auth_invalid_ASUE_cert()
{
	fprintf(stderr,"WAPI TRAP :%s\n",__FUNCTION__);
}

/*5.1.3.2*/
void wapimib_trap_keyneg_invalid_challenge()
{
	fprintf(stderr,"WAPI TRAP :%s\n",__FUNCTION__);
}

/*5.1.3.3*/
void wapimib_trap_keyneg_invalid_MIC()
{
	fprintf(stderr,"WAPI TRAP :%s\n",__FUNCTION__);
}

/*5.1.3.4*/
void wapimib_trap_keyneg_invalid_WIE()
{
	fprintf(stderr,"WAPI TRAP :%s\n",__FUNCTION__);
}


#ifdef notyet
/*todo: sta based statistics need to be implemented in wlan driver*/
static int wapimib_get_stastats_from_driver(char* ifname, char* macaddr, struct ieee80211req_sta_stats* stats)
{
	struct iwreq iwr;
	
	int sock = socket(AF_INET, SOCK_DGRAM, 0);
	if(sock<0)
	{	
		return -1;
	}

	(void) memset(&iwr, 0, sizeof(iwr));
	(void) strncpy(iwr.ifr_name, ifname, sizeof(iwr.ifr_name));
	iwr.u.data.pointer = (void *) stats;
	iwr.u.data.length = sizeof(struct ieee80211req_sta_stats);
	memcpy(stats->is_u.macaddr, macaddr, 6);

	DPrintf("[WAPID]::get stastats from %s, chars %d, ioctl 0x%x: 0x%x\n",
			iwr.ifr_name, strlen(iwr.ifr_name),IEEE80211_IOCTL_STA_STATS, SIOCDEVPRIVATE);
	if (ioctl(sock, IEEE80211_IOCTL_STA_STATS, &iwr) < 0)
	{

		close(sock);
		return -1;
	}

	close(sock);
	return 0;
}
#endif

static int wapimib_get_80211stats_from_driver(char* ifname, void* dot11stats)
{
	struct ifreq ifr;
	int sock = socket(AF_INET, SOCK_DGRAM, 0);
	if(sock<0)
	{	
		return -1;
	}	

	memset(&ifr, 0, sizeof(ifr));
	strncpy(ifr.ifr_name, ifname, sizeof (ifr.ifr_name));
	ifr.ifr_data = (caddr_t) dot11stats;
	if (ioctl(sock, SIOCG80211STATS, &ifr) < 0)
	{
		close(sock);
		return -1;
	}

	close(sock);
	return 0;
}



/*command GETMIB

	config  index[1-32]
		1-32: single mib
		0: total config table
		others: invalid
	ucipher  index[1-1]
             1: SMS4
             others: invalid
	asuite   index[1-2]
		1: cert
		2: psk
		others: invalid
	stats     index[1-X]
		1  total statistics(for ff:ff:ff:ff:ff:ff), it's always available
		others                 single sta statistics it existed     
		
	stanum
		no other paramter
		return station number(not include ff:ff:ff:ff:ff:ff)
*/


int wapid_ctrl_getmib(struct eloop_data *eloop, const char *req, char *buf, int buf_len)
{

	apdata_info *pap;
	struct wapid_interfaces *user;
	struct auth_sta_info_t *sta_info;
	struct wapimib_msg_hdr *req_hdr, *reply_hdr;
#ifdef notyet
	struct ieee80211req_sta_stats staStats;
	const struct ieee80211_nodestats *ns = &staStats.is_stats;
#endif
	struct {
		struct ieee80211_stats          iv_stats; 
		struct ieee80211_mac_stats   iv_unicast_stats;   
		struct ieee80211_mac_stats   iv_multicast_stats; 
	}dot11Stats;

	
	
	int reply_len=sizeof(struct wapimib_msg_hdr);
	struct Gb15629dot11wapiStatsEntry *stats;
	int stanum;
	int datalen=0;

	if (buf_len <reply_len)
	{
		goto error;
	}
	req_hdr=(struct wapimib_msg_hdr*)req;
	reply_hdr=(struct wapimib_msg_hdr*)buf;
	memcpy(reply_hdr,req_hdr,reply_len);
	reply_hdr->type=WAPI_MIB_CMD_REPLY;

		
	if (req_hdr->type!=WAPI_MIB_CMD_GET )
	{
		DPrintf("[WAPID]::req type %d",req_hdr->type);
		goto error;
	}

	for (user = eloop->vap_user;user; user = user->next)
	{
		if(user->wapid && user->wapid->g_ifr== req_hdr->ifindex)
			break;

	}

	if(!user)
	{
		DPrintf("[WAPID]::Cann't find VAP\n");
		goto error;

	}

	pap=user->wapid;

	switch(req_hdr->mibEntry)
	{
		case WAPI_MIB_ENTRY_CONFIG:
			if (req_hdr->subEntry ==0)
			{
				datalen=sizeof(struct Gb15629dot11wapiConfigEntry);
				memcpy(reply_hdr->data,&pap->mib_config,datalen);
			}
			else if (req_hdr->subEntry>=1 && req_hdr->subEntry <=32)
			{
				datalen=wapimib_config_get(&pap->mib_config,req_hdr->subEntry,reply_hdr->data);
			}
			else
			{
				goto error;
			}
				
			break;
			
		case WAPI_MIB_ENTRY_UCIPHER:
			if (req_hdr->subEntry !=1)
			{
				goto error;
				return -1;
			}
			datalen=sizeof(struct Gb15629dot11wapiConfigUnicastCiphersEntry);
			memcpy(reply_hdr->data,&pap->mib_uciphers,datalen);
			break;
			
		case WAPI_MIB_ENTRY_ASUITE:
			if (req_hdr->subEntry<1 ||req_hdr->subEntry>2)
			{
				goto error;
			}
			datalen=sizeof(struct Gb15629dot11wapiConfigAuthenticationSuitesEntry);
			memcpy(reply_hdr->data,&pap->mib_asuites[req_hdr->subEntry-1],datalen);
			break;
			
		case WAPI_MIB_ENTRY_STATS:
			stats=(struct Gb15629dot11wapiStatsEntry *)reply_hdr->data;
			if(req_hdr->subEntry==1)/*for ff:ff:ff:ff:ff:ff*/
			{
				datalen=sizeof(struct Gb15629dot11wapiStatsEntry);
				memcpy(reply_hdr->data,&pap->mib_stats,datalen);
				if(wapimib_get_80211stats_from_driver(pap->iface, &dot11Stats) == 0)
				{
				stats->StatsWPIReplayCounters =(u32)(dot11Stats.iv_unicast_stats.ims_rx_wpireplay 
					+ dot11Stats.iv_multicast_stats.ims_rx_wpireplay);
				stats->StatsWPIDecryptableErrors =(u32)(dot11Stats.iv_unicast_stats.ims_rx_badkeyid 
					+ dot11Stats.iv_multicast_stats.ims_rx_badkeyid);
				stats->StatsWPIMICErrors =(u32)(dot11Stats.iv_unicast_stats.ims_rx_wpimic 
					+ dot11Stats.iv_multicast_stats.ims_rx_wpimic);

				}
			}
			else/*single sta with index from [2,# )*/
			{
				sta_info=ap_get_sta_by_index(req_hdr->subEntry-1,  pap);
				if (!sta_info)
				{
					goto error;
				}
				datalen=sizeof(struct Gb15629dot11wapiStatsEntry);
				memcpy(reply_hdr->data,&sta_info->mib_stats,datalen);

#ifdef notyet
				if(wapimib_get_stastats_from_driver(pap->iface, sta_info->mac, &staStats)==0)
				{
					stats->StatsWPIReplayCounters=0;
					stats->StatsWPIDecryptableErrors=ns->ns_rx_decryptcrc;
					stats->StatsWPIMICErrors=ns->ns_rx_demicfail;
				}	
#endif
			}

			stats->StatsIndex=req_hdr->subEntry;
			break;
			
		case WAPI_MIB_ENTRY_STANUM:
			stanum=ap_get_sta_num(pap);
			datalen=sizeof(int);
			memcpy(reply_hdr->data,&stanum,datalen);
			break;
			
		default:
			goto error;
			

	}

	reply_hdr->result=WAPI_MIB_RESULT_OK;
	reply_hdr->datalen=datalen;
	reply_len+=datalen;
	return reply_len;

error:	
	DPrintf("[WAPID]::Mib command fails\n");
	reply_hdr->result=WAPI_MIB_RESULT_FAIL;
	reply_hdr->datalen=0;
	return reply_len;
}



#endif

