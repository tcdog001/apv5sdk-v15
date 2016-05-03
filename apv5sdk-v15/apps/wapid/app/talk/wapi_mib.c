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

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>	 
#include <unistd.h>

#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <net/if.h>

#include "wapi_ctrl.h"
#include "dot11wapiMib.h"

#ifdef DOT11_WAPI_MIB
void wapimib_config_dump(struct Gb15629dot11wapiConfigEntry* entry, int item )
{
	int i;
	switch (item)
	{
		case WAPIMIB_CONFENTRY_VERSION:
			printf("%02d  %-16s [%d]\n",item,"Version",entry->ConfigVersion);
			break;
		case WAPIMIB_CONFENTRY_AUTHCTL:
			printf("%02d  %-16s [%d]\n",item,"AuthControl",entry->ControlledAuthControl);
			break;
		case WAPIMIB_CONFENTRY_PORTCTL:
			printf("%02d  %-16s [%d]\n",item,"PortControl",entry->ControlledPortControl);
			break;
		case WAPIMIB_CONFENTRY_OPTIONAL_IMP:
			printf("%02d  %-16s [%d]\n",item,"OptionalImpl",entry->OptionalImplemented);
			break;
		case WAPIMIB_CONFENTRY_PREAUTH_IMP:
			printf("%02d  %-16s [%d]\n",item,"PreauthImpl",entry->PreauthenticationImplemented);
			break;
		case WAPIMIB_CONFENTRY_ENABLE:
			printf("%02d  %-16s [%d]\n",item,"Enable",entry->Enabled);
			break;
		case WAPIMIB_CONFENTRY_PREAUTH_ENABLE:
			printf("%02d  %-16s [%d]\n",item,"PreauthEnable",entry->PreauthenticationEnabled);
			break;
		case WAPIMIB_CONFENTRY_UKEY_SUPPORT:
			printf("%02d  %-16s [%d]\n",item,"UKeysSupport",entry->ConfigUnicastKeysSupported);
			break;
		case WAPIMIB_CONFENTRY_UREKEY_METHOD:
			printf("%02d  %-16s [%d]\n",item,"URekeyMethod",entry->ConfigUnicastRekeyMethod);
			break;
		case WAPIMIB_CONFENTRY_UREKEY_TIME:
			printf("%02d  %-16s [%d]\n",item,"URekeyTime",entry->ConfigUnicastRekeyTime);
			break;
		case WAPIMIB_CONFENTRY_UREKEY_PACKETS:
			printf("%02d  %-16s [%d]\n",item,"URekeyPackets",entry->ConfigUnicastRekeyPackets);
			break;
		case WAPIMIB_CONFENTRY_MCAST_CIPHER:
			printf("%02d  %-16s [",item, "MCipher");
			for (i=0;i<4;i++)
				printf("%02X",entry->ConfigMulticastCipher[i]);
			printf("]\n");
			break;
		case WAPIMIB_CONFENTRY_MREKEY_METHOD:
			printf("%02d  %-16s [%d]\n",item,"MRekeyMethod",entry->ConfigMulticastRekeyMethod);
			break;
		case WAPIMIB_CONFENTRY_MREKEY_TIME:
			printf("%02d  %-16s [%d]\n",item,"MRekeyTime",entry->ConfigMulticastRekeyTime);
			break;
		case WAPIMIB_CONFENTRY_MREKEY_PACKETS:
			printf("%02d  %-16s [%d]\n",item,"MRekeyPackets",entry->ConfigMulticastRekeyPackets);
			break;
		case WAPIMIB_CONFENTRY_MREKEY_STRICT:
			printf("%02d  %-16s [%d]\n",item,"MRekeyStrict",entry->ConfigMulticastRekeyStrict);
			break;
		case WAPIMIB_CONFENTRY_PSK_VALUE:
			printf("%02d  %-16s [0] hidden\n",item,"PSKValue");
			break;
		case WAPIMIB_CONFENTRY_PSK_PASSPHRASE:
			printf("%02d  %-16s [0] hidden\n",item,"PSKPhrase");
			break;
		case WAPIMIB_CONFENTRY_CERT_UPDATECNT:
			printf("%02d  %-16s [%d]\n",item,"CertUpdateCount",entry->ConfigCertificateUpdateCount);
			break;
		case WAPIMIB_CONFENTRY_MCAST_UPDATECNT:
			printf("%02d  %-16s [%d]\n",item,"MCastUpdateCount",entry->ConfigMulticastUpdateCount);
			break;
		case WAPIMIB_CONFENTRY_UCAST_UPDATECNT:
			printf("%02d  %-16s [%d]\n",item,"UCastUpdateCount",entry->ConfigUnicastUpdateCount);
			break;
		case WAPIMIB_CONFENTRY_MCAST_CIPHER_SIZE:
			printf("%02d  %-16s [%d]\n",item,"MCastCipherSize",entry->ConfigMulticastCipherSize);
			break;
		case WAPIMIB_CONFENTRY_BK_LIFETIME:
			printf("%02d  %-16s [%d]\n",item,"BKLifetime",entry->ConfigBKLifetime);
			break;
		case WAPIMIB_CONFENTRY_BK_REAUTH_THRESH:
			printf("%02d  %-16s [%d]\n",item,"BKReauthThresh",entry->ConfigBKReauthThreshold);
			break;
		case WAPIMIB_CONFENTRY_SA_TIMEOUT:
			printf("%02d  %-16s [%d]\n",item,"SATimeout",entry->ConfigSATimeout);
			break;
		case WAPIMIB_CONFENTRY_AUTHSUITE_SEL:
			printf("%02d  %-16s [",item, "AuthSuiteSel");
			for (i=0;i<4;i++)
				printf("%02X",entry->AuthenticationSuiteSelected[i]);
			printf("]\n");
			break;
		case WAPIMIB_CONFENTRY_UCIPHER_SEL:
			printf("%02d  %-16s [",item, "UCastCipherSel");
			for (i=0;i<4;i++)
				printf("%02X",entry->UnicastCipherSelected[i]);
			printf("]\n");
			break;
		case WAPIMIB_CONFENTRY_MCIPHER_SEL:
			printf("%02d  %-16s [",item, "MCastCipherSel");
			for (i=0;i<4;i++)
				printf("%02X",entry->MulticastCipherSelected[i]);
			printf("]\n");
			break;
		case WAPIMIB_CONFENTRY_BKID_USED:
			printf("%02d  %-16s [",item, "BKID");
			for (i=0;i<16;i++)
				printf("%02X",entry->BKIDUsed[i]);
			printf("]\n");
			break;
			break;
		case WAPIMIB_CONFENTRY_AUTHSUITE_REQ:
			printf("%02d  %-16s [",item, "AuthSuiteReq");
			for (i=0;i<4;i++)
				printf("%02X",entry->AuthenticationSuiteRequested[i]);
			printf("]\n");
			break;
		case WAPIMIB_CONFENTRY_UCIPHER_REQ:
			printf("%02d  %-16s [",item, "UCastCipherReq");
			for (i=0;i<4;i++)
				printf("%02X",entry->UnicastCipherRequested[i]);
			printf("]\n");
			break;
		case WAPIMIB_CONFENTRY_MCIPHER_REQ:
			printf("%02d  %-16s [",item, "MCastCipherReq");
			for (i=0;i<4;i++)
				printf("%02X",entry->MulticastCipherRequested[i]);
			printf("]\n");
			break;
		default:
			break;

	}


	
}

void wapimib_ucipher_dump(struct Gb15629dot11wapiConfigUnicastCiphersEntry* entry)
{
	int i;
	printf("%-16s [%d]\n","Index",entry->ConfigUnicastCipherIndex);
	printf("%-16s [", "Cipher");
	for (i=0;i<4;i++)
		printf("%02X",entry->ConfigUnicastCipher[i]);
	printf("]\n");
	printf("%-16s [%d]\n","CipherEnabled",entry->ConfigUnicastCipherEnabled);
	printf("%-16s [%d]\n","CipherSize",entry->ConfigUnicastCipherSize);

}

void wapimib_asuite_dump(struct Gb15629dot11wapiConfigAuthenticationSuitesEntry* entry)
{
	int i;
	printf("%-16s [%d]\n","Index",entry->ConfigAuthenticationSuiteIndex);
	printf("%-16s [","AuthSuite");
	for (i=0;i<4;i++)
		printf("%02X",entry->ConfigAuthenticationSuite[i]);
	printf("]\n");
	printf("%-16s [%d]\n","SuiteEnabled",entry->ConfigAuthenticationSuiteEnabled);

}
void wapimib_stats_dump(struct Gb15629dot11wapiStatsEntry* entry)
{
	int i;
	int item;

	for (item=WAPIMIB_STATS_INDEX;item<=WAPIMIB_STATS_WAI_MULTICASTFAILURE;item++)
	    switch (item)
	    {
		case WAPIMIB_STATS_INDEX:
			printf("%02d  %-16s [%d]\n",item,"Index",entry->StatsIndex);
			break;
		case WAPIMIB_STATS_STAADDRESS:
			printf("%02d  %-16s [",item, "Address");
			for (i=0;i<6;i++)
				printf("%02X",entry->StatsSTAAddress[i]);
			printf("]\n");
			break;
		case WAPIMIB_STATS_VERSION:
			printf("%02d  %-16s [%d]\n",item,"Version",entry->StatsVersion);
			break;
		case WAPIMIB_STATS_CTLPORT_STATUS:
			printf("%02d  %-16s [%d]\n",item,"CTLPortStatus",entry->StatsControlledPortStatus);
			break;
		case WAPIMIB_STATS_UNICAST_CIPHER:
			printf("%02d  %-16s [",item, "UCipter");
			for (i=0;i<4;i++)
				printf("%02X",entry->StatsSelectedUnicastCipher[i]);
			printf("]\n");
			break;
		case WAPIMIB_STATS_WPI_REPLAYCNT:
			printf("%02d  %-16s [%d]\n",item,"WPIReply",entry->StatsWPIReplayCounters);
			break;
		case WAPIMIB_STATS_WPI_DECRYPTERROR:
			printf("%02d  %-16s [%d]\n",item,"WPIDecryptError",entry->StatsWPIDecryptableErrors);
			break;
		case WAPIMIB_STATS_WPI_MICERROR:
			printf("%02d  %-16s [%d]\n",item,"WPIMICError",entry->StatsWPIMICErrors);
			break;
		case WAPIMIB_STATS_WAI_SIGERROR:
			printf("%02d  %-16s [%d]\n",item,"WAISIGError",entry->StatsWAISignatureErrors);
			break;
		case WAPIMIB_STATS_WAI_HMACERROR:
			printf("%02d  %-16s [%d]\n",item,"WAIHMACError",entry->StatsWAIHMACErrors);
			break;
		case WAPIMIB_STATS_WAI_AUTHFAILURE:
			printf("%02d  %-16s [%d]\n",item,"WAIAUTHFail",entry->StatsWAIAuthenticationResultFailures);
			break;
		case WAPIMIB_STATS_WAI_DISCARDCNT:
			printf("%02d  %-16s [%d]\n",item,"WAIDiscard",entry->StatsWAIDiscardCounters);
			break;
		case WAPIMIB_STATS_WAI_TIMEOUTCNT:
			printf("%02d  %-16s [%d]\n",item,"WAITimeout",entry->StatsWAITimeoutCounters);
			break;
		case WAPIMIB_STATS_WAI_FORMATERROR:
			printf("%02d  %-16s [%d]\n",item,"WAIFMTError",entry->StatsWAIFormatErrors);
			break;
		case WAPIMIB_STATS_WAI_CERTFAILURE:
			printf("%02d  %-16s [%d]\n",item,"WAICERTFail",entry->StatsWAICertificateHandshakeFailures);
			break;
		case WAPIMIB_STATS_WAI_UNICASTFAILURE:
			printf("%02d  %-16s [%d]\n",item,"WAIUKeyFail",entry->StatsWAIUnicastHandshakeFailures);
			break;
		case WAPIMIB_STATS_WAI_MULTICASTFAILURE:
			printf("%02d  %-16s [%d]\n",item,"WAIMKeyFail",entry->StatsWAIMulticastHandshakeFailures);
			break;
		default:
			break;
	    }


}



static int wapimib_config_parse(struct wapimib_msg_hdr* hdr, char*buf)
{
	int i;
	if(hdr->subEntry==0)
	{
		for (i=WAPIMIB_CONFENTRY_VERSION;i<=WAPIMIB_CONFENTRY_MCIPHER_REQ;i++)
			wapimib_config_dump((struct Gb15629dot11wapiConfigEntry * )buf, i);
	}
	else
		wapimib_config_dump((struct Gb15629dot11wapiConfigEntry * )buf, hdr->subEntry);

	return 0;
}

static int  wapimib_ucipher_parse(struct wapimib_msg_hdr* hdr, char*buf)
{
	wapimib_ucipher_dump((struct Gb15629dot11wapiConfigUnicastCiphersEntry *) buf);
	return 0;
}

static int wapimib_asuite_parse(struct wapimib_msg_hdr* hdr, char*buf)
{
	wapimib_asuite_dump((struct Gb15629dot11wapiConfigAuthenticationSuitesEntry *) buf);
	return 0;
}

static int  wapimib_stats_parse(struct wapimib_msg_hdr* hdr, char*buf)
{
	wapimib_stats_dump((struct Gb15629dot11wapiStatsEntry *) buf);
	return 0;
}

static int wapimib_stanum_parse(struct wapimib_msg_hdr* hdr, char*buf)
{

	int *num;
	num=(int*)buf;
	printf("StaNum: [%d]\n", *num);
	return *num;
}

struct wapimib_cmd{
	const char *mibName;
	int mibEntry;
	int (*mibParse)(struct wapimib_msg_hdr* hdr, char*buf);
};

static struct wapimib_cmd wapimib_commands[]=
{
	{"Config Entry",WAPI_MIB_ENTRY_CONFIG, wapimib_config_parse},
	{"Unicast Cipher Entry",WAPI_MIB_ENTRY_UCIPHER,wapimib_ucipher_parse},
	{"Auth Suites Entry",WAPI_MIB_ENTRY_ASUITE,wapimib_asuite_parse},
	{"Stats Entry",WAPI_MIB_ENTRY_STATS,wapimib_stats_parse},
	{"Sta Num",WAPI_MIB_ENTRY_STANUM,wapimib_stanum_parse},
	{NULL,0,NULL},
};


static int _wapimib_request(struct wpa_ctrl *ctrl, char *req, int reqlen, char* buf, int* len )
{

	int ret;
	
	ret = wpa_ctrl_request(ctrl, req, reqlen, buf, len, NULL);
	if (ret == -2) {
		printf("mib command timed out.\n");
		return -2;
	} else if (ret < 0) {
		printf("mib command failed.\n");
		return -1;
	}

	return 0;
}


static int wapimib_request(struct wpa_ctrl *ctrl, int ifindex, int mibEntry , int subEntry )
{
	char buf[1024];
	int len=sizeof(buf);
	char req[32];
	struct wapimib_msg_hdr *req_hdr, *reply_hdr;
	int reqlen=sizeof(struct wapimib_msg_hdr )+6;
	struct wapimib_cmd *cmd;
	int ret=0;


	memcpy(req,"GETMIB",6);
	req_hdr=(struct wapimib_msg_hdr *)(req+6);
	memset(req_hdr,0,sizeof(struct wapimib_msg_hdr));
	req_hdr->type=WAPI_MIB_CMD_GET;
	req_hdr->ifindex=ifindex;


	cmd=wapimib_commands;
	while (cmd->mibName) {
		if(cmd->mibEntry==mibEntry)
			break;
		cmd++;
	}

	if(!cmd->mibName)
		return -1;
	
	req_hdr->mibEntry=mibEntry;
	req_hdr->subEntry=subEntry;


	if(_wapimib_request(ctrl,req,reqlen,buf,&len)!=0)
	{
		return -1;
	}

	reply_hdr=(struct wapimib_msg_hdr *)buf;
	if(reply_hdr->result!=WAPI_MIB_RESULT_OK)
	{
		return -1;
	}

	if(cmd->mibParse)
	{
		printf("Get MIB: %s\n",cmd->mibName);			
		ret=cmd->mibParse(reply_hdr, reply_hdr->data);
		printf("\n");	
	}

	
	
	return ret;
}


int get_device_index_by_name(char* dev_name)
{
	int sock;

	struct ifreq ifr;
	int ifindex;

	if (strlen (dev_name)>= sizeof (ifr.ifr_name)) {
		return -1;
	}
	sock = socket(PF_INET, SOCK_DGRAM, 0);
	if(sock<0)
	{	
		return -1;
	}
	
	memcpy (ifr.ifr_name, dev_name, strlen (dev_name) +1);
	if (ioctl (sock, SIOCGIFINDEX, &ifr) != 0) {
		return -1;
	}

	close(sock);
	return ifr.ifr_ifindex;

}


int wapitalk_cmd_getmib(struct wpa_ctrl *ctrl, int argc, char *argv[])
{

	char buf[1024];
	int len=sizeof(buf);
	char req[32];
	struct wapimib_msg_hdr *req_hdr, *reply_hdr;
	int reqlen=sizeof(struct wapimib_msg_hdr )+6;

	int ifindex;
	struct wapimib_cmd *cmd;
	int ret;
	int i;

	if (argc < 1) {
		printf("Invalid 'getmib' command - interface paramters  is required %d.\n");
		return -1;
	}

	ifindex=get_device_index_by_name(argv[0]);

	if(ifindex<0){
		printf("Invalid interface name.\n");
		return -1;
	}

	wapimib_request(ctrl, ifindex, WAPI_MIB_ENTRY_CONFIG, 0);
	wapimib_request(ctrl, ifindex, WAPI_MIB_ENTRY_UCIPHER, 1);
	wapimib_request(ctrl, ifindex, WAPI_MIB_ENTRY_ASUITE, 1);
	wapimib_request(ctrl, ifindex, WAPI_MIB_ENTRY_ASUITE, 2);

	ret=wapimib_request(ctrl, ifindex, WAPI_MIB_ENTRY_STANUM, 0);

	if (ret >=0)
	{
		for (i=1;i<=ret+1;i++)
		{
			wapimib_request(ctrl, ifindex, WAPI_MIB_ENTRY_STATS, i);
		}
	}
	return 0;
}

#endif

