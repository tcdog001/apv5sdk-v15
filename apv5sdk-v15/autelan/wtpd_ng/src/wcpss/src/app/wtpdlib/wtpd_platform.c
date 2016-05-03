#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <errno.h>
#include "wtpd_platform.h"


#define WTP_BOARD_ID_LEN 64
#define TEMP_STR_LEN 64

extern int debug_print;
extern char WTPBoardID[WTP_BOARD_ID_LEN];
extern char * trim(char *str_org);
extern __inline__ void CWDebugLog(const char *format, ...) ;
extern __inline__ void CWWTPDebugLog(const char *format, ...) ;
extern unsigned int ip_int2str(unsigned int ipAddress, unsigned char *buff);
extern char *CWProtocolRetrieveStr(CWProtocolMessage *msgPtr, int len);
extern CWBool _CWErrorRaise(CWErrorCode code, const char *msg, const char *fileName, int line);




CWBool _system(char *cmdbuf)
{
	int stat;
	CWWTPDebugLog("cmd :%s", cmdbuf) ;
	stat = system(cmdbuf);
	return CW_TRUE;
	
	if(WIFEXITED(stat))
	{
		if(WEXITSTATUS(stat) == 0)
		{	
			CWWTPDebugLog("cmd ok:%s", cmdbuf) ;
			return CW_TRUE;
		}
		else
		{
			CWWTPDebugLog("cmd err:%s %s.", cmdbuf,strerror(errno)) ;
			return CW_FALSE;
		}
	}

	CWWTPDebugLog("cmd err:%s %s.", cmdbuf,strerror(errno)) ;
	return CW_FALSE;	
//	return CW_TRUE;
}
CWBool spf_cmd(const char *format,...)
{
	if(format == NULL)
		return CW_FALSE;
	int bRet = CW_TRUE;
	char *cmd;
	int cmd_len = strlen(format)+256;
	
	CW_CREATE_STRING_ERR(cmd, cmd_len, return CW_FALSE;);

	va_list args;

	va_start(args,format);
	vsnprintf(cmd, cmd_len, format, args);
	va_end(args);

	bRet = _system(cmd);

	CW_FREE_OBJECT(cmd);

	return bRet;
}
CWBool CWGetSN(char * sn)
{
		char temp_str[100];
		char str_tmp[64];
		char str[TEMP_STR_LEN];
		FILE *fp;
		memset(temp_str, 0, 100);
		memset(str_tmp, 0 ,64);
	//chenxf added WAG102 sn get from mac correctly 20101211
#ifdef WG102
		sprintf(temp_str, "showsysinfo | awk -F \":\" '/MAC/ {print $2\":\"$3\":\"$4\":\"$5\":\"$6\":\"$7}'");
#elif WAG102
		sprintf(temp_str, "showsysinfo | awk -F \":\" '/MAC/ {print $2\":\"$3\":\"$4\":\"$5\":\"$6\":\"$7}'");
#else
		sprintf(temp_str, "showsysinfo | awk -F \":\" '/SN/ {print $2}'");
#endif
		fp = popen(temp_str, "r");
		if(fp)
		{
			memset(str, 0, TEMP_STR_LEN);
			memset(str_tmp, 0, 64);
			fgets(str_tmp, sizeof(str_tmp), fp);
			str_tmp[strlen(str_tmp)-1] = '\0';
			strcpy(str, trim(str_tmp));
			CWWTPDebugLog("WTPBoardID:%s len:%d", str, strlen(str));
			if(debug_print)
				printf("WTPBoardID:%s len:%d\n", str, strlen(str));
			memcpy(sn, str, WTP_BOARD_ID_LEN);
			pclose(fp);
		}
	return CW_TRUE;
}
CWBool CWGetBaseMac(char * basemac)
{
	char temp_str[100];
	char str_tmp[64];
	char str[TEMP_STR_LEN];
	int a,b,c,d,e,f;
	FILE *fp;
	memset(temp_str, 0, 100);
	memset(str_tmp, 0 ,64);

get_base_mac_atheros:
	memset(temp_str, 0, 100);
	sprintf(temp_str, "showsysinfo | awk -F \":\" '/MAC/ {print $2\":\"$3\":\"$4\":\"$5\":\"$6\":\"$7}'");
	fp = popen(temp_str, "r");
	if(fp)
	{
		memset(str, 0, TEMP_STR_LEN);
		memset(str_tmp, 0, 64);
		fgets(str_tmp, sizeof(str_tmp), fp);
		str_tmp[strlen(str_tmp)-1] = '\0';
		strcpy(str, trim(str_tmp));
		sscanf(str, "%x:%x:%x:%x:%x:%x", &a,&b,&c,&d,&e,&f);
		pclose(fp);
	}
	if((0 == a)&&(0 == b)&&(0 == c))
	{
		sleep(1);
		goto get_base_mac_atheros;
	}
	
	basemac[0]=a;
	basemac[1]=b;
	basemac[2]=c;
	basemac[3]=d;
	basemac[4]=e;
	basemac[5]=f;
	
	return CW_TRUE;
}
CWBool CWGetMode(char * mode)
{
	return CW_TRUE;
}
CWBool CWGetRealMode(char * realMode)
{
	char temp_str[100];	
	char str_tmp[64];
	char str[TEMP_STR_LEN];
	FILE *fp;
	memset(temp_str, 0, 100);

	sprintf(temp_str, "showsysinfo | awk -F \":\" '/Device Type/ {print $2}'");
	fp = popen(temp_str, "r");
	if(fp)
	{
		memset(str, 0, TEMP_STR_LEN);
		memset(str_tmp, 0, 64);
		fgets(str_tmp, sizeof(str_tmp), fp);
		str_tmp[strlen(str_tmp)-1] = '\0';
		strcpy(str, trim(str_tmp));
		CWWTPDebugLog("temp_WTPRealModelNum:%s len:%d", str, strlen(str));
		if(debug_print)
			printf("temp_WTPRealModelNum:%s len:%d\n", str, strlen(str));

		memcpy(realMode, str, strlen(str));
		pclose(fp);
	}
	return CW_TRUE;
}
CWBool CWGetSoftwareVersion(char * softwareVersion)\
{
	char temp_str[100];
   	char str_tmp[64];
	char str[64];
	FILE *fp;
	int k=0;

	memset(temp_str, 0, 100);
	sprintf(temp_str, "showsysinfo | awk -F \":\" '/Software Version/ {print $2}'");
	fp = popen(temp_str, "r");
	if(fp)
	{
		memset(str, 0, 64);
		memset(str_tmp, 0, 64);
		fgets(str_tmp, sizeof(str_tmp), fp);
		str_tmp[strlen(str_tmp)-1] = '\0';
		strcpy(str, trim(str_tmp));
		CWWTPDebugLog("temp_SoftwareVersion:%s len:%d", str, strlen(str));
		if(debug_print)
			printf("temp_SoftwareVersion:%s len:%d\n", str, strlen(str));
		for(k=0;k<strlen(str);k++)
		{
			if(str[k]=='#')
				str[k] = '\0';
		}
		memcpy(softwareVersion, str, strlen(str));
		pclose(fp);
	}
	return CW_TRUE;
}
CWBool CWGetHardwareVersion(char * hardwareVersion)
{
	char hardversion_str[100];
	FILE *fh;
	memset(hardversion_str, 0, 100);
	sprintf(hardversion_str, "showsysinfo | awk -F \":\" '/Hardware Version/ {print $2}'");
	fh = popen(hardversion_str, "r");
	if(fh)
	{
		memset(hardwareVersion, 0, 64);
		fgets(hardwareVersion, sizeof(hardwareVersion), fh);
		hardwareVersion[strlen(hardwareVersion)-1] = '\0';
		CWWTPDebugLog("gWTPHardWareVersion:%s len:%d", hardwareVersion, strlen(hardwareVersion));
		if(debug_print)
			printf("gWTPHardWareVersion:%s len:%d\n", hardwareVersion, strlen(hardwareVersion));
		pclose(fh);
	}
	return CW_TRUE;
}
CWBool CWGetApCode(char * apCode)
{
	char temp_str[100];
	char str[TEMP_STR_LEN];
	memset(temp_str, 0, 100);

#ifdef WAG102
	memcpy(apCode,"AG102",5);
	CWWTPDebugLog("temp_APcode:%s len:%d","AG102",5);
	if(debug_print)
		printf("temp_APcode:%s len:%d\n","AG102",5);
#else
	memset(temp_str, 0, 100);
	sprintf(temp_str, "wrcpuutil -i | awk -F \":\" '/product type/ {print substr($2,3)}'");
	PopenFile(temp_str,str,TEMP_STR_LEN);
	memcpy(apCode, str, strlen(str));
	CWWTPDebugLog("temp_APcode:%s len:%d", str, (int)strlen(str));
#endif	
	return CW_TRUE;

}
CWBool CWGetInternalSoftwareVersion(char * internalSoftwareVersion)
{
	char codeversion_str[100];
	FILE *fc;
	memset(codeversion_str, 0, 100);
	sprintf(codeversion_str, "showver | awk -F \".\" '// {print $1\".\"$2\".\"$3}'");
	fc = popen(codeversion_str, "r");
	if(fc)
	{
		memset(internalSoftwareVersion, 0, 64);
		fgets(internalSoftwareVersion, sizeof(internalSoftwareVersion), fc);
		internalSoftwareVersion[strlen(internalSoftwareVersion)-1] = '\0';
		CWWTPDebugLog("gWTPCodeVersion:%s len:%d", internalSoftwareVersion, (int)strlen(internalSoftwareVersion));
		if(debug_print)
			printf("gWTPCodeVersion:%s len:%d\n", internalSoftwareVersion, (int)strlen(internalSoftwareVersion));
		pclose(fc);
	}
	return CW_TRUE;
}
CWBool CWGetManuOption(char WTPRealModelNum[64], unsigned int *gManuOption)
{
#ifdef AQ2000-M
		gManuOption = WTP_AQ2000M;
		CWWTPDebugLog("WTP manufacture option:%d", gManuOption);
#endif
	return CW_TRUE;
}
CWBool CWGetTxpower(int *txpower, char *wlanname)
{
	char temp_str[128];
	char str_tmp[24];
	char str[24];
	FILE *fp;
	memset(temp_str, 0, 128);
	memset(str_tmp, 0 ,24);
	sprintf(temp_str,"/usr/sbin/gettxpower %s |awk	'{print $0}'",wlanname);
	fp = popen(temp_str, "r");
	if(fp)
	{
		memset(str, 0, 24);
		memset(str_tmp, 0, 24);
		fgets(str_tmp, sizeof(str_tmp), fp);
		str_tmp[strlen(str_tmp)-1] = '\0';
		strcpy(str, trim(str_tmp));
//		if(debug_print)
//			printf("txpower:%s len:%d\n", str, (int)strlen(str));
		pclose(fp);
	}	
	*txpower=atoi(str);

	return CW_TRUE;

}
CWBool CWSetCountryCode(unsigned char countryCode[4])
{
	char cmdbuf[128];
	memset(cmdbuf,0,128);
	sprintf(cmdbuf, "/usr/sbin/set_countrycode %s >/dev/null 2>&1", countryCode);
	return _system(cmdbuf);
}
CWBool CWSetWlanTunnelMode(char *wlanname,int tunnel_mode)
{
	char cmdbuf[128];
	memset(cmdbuf,0,128);
	sprintf(cmdbuf, "echo %d > /proc/sys/net/%s/vap_splitmac",tunnel_mode,wlanname);
	return _system(cmdbuf);
}
CWBool CWEnableIpForward(int enable)
{
	char cmdbuf[128];
	memset(cmdbuf,0,128);
	sprintf(cmdbuf, "echo %d > /proc/sys/net/ipv4/ip_forward",enable);
	return _system(cmdbuf);
}
CWBool CWSetSpfastDown(int enable)
{
	char cmdbuf[128];
	memset(cmdbuf,0,128);
	sprintf(cmdbuf, "echo %d > /proc/sys/net/capwap/spfast_down",enable);
	return _system(cmdbuf);
}
CWBool CWSetSpfastUp(int enable)
{
	char cmdbuf[128];
	memset(cmdbuf,0,128);
	sprintf(cmdbuf, "echo %d > /proc/sys/net/capwap/spfast_up",enable);
	return _system(cmdbuf);
}
CWBool CWSetUdpPort()
{
	char cmdbuf[128];
	memset(cmdbuf,0,128);
	sprintf(cmdbuf, "echo 32769 > /proc/sys/net/capwap/udp_port");
	return _system(cmdbuf);
}
CWBool CWSetAC_IP(char *ac_ip)
{
	char cmdbuf[128];
	memset(cmdbuf,0,128);
	sprintf(cmdbuf, "echo %#x > /proc/sys/net/capwap/ac_addr", inet_addr(ac_ip));
	return _system(cmdbuf);
}
CWBool CWEnableIPinIP(char *wlanname,int enable)
{
	char cmdbuf[128];
	memset(cmdbuf,0,128);
	sprintf(cmdbuf, "echo %d > /proc/sys/net/%s/vap_ipinip", enable, wlanname);
	return _system(cmdbuf);
}


CWBool CWSet_dhcp_snooping(unsigned char state)
{
	CWBool bRet=CW_TRUE;
	char cmdbuf[128];
	sprintf(cmdbuf,"killall dhcpsnooping");
	system(cmdbuf);
	if(state==1)
	{
#if (defined( AQ2000N )|| defined(XHPB44)||defined(XJAP93V1))
		sprintf(cmdbuf,"/usr/sbin/dhcpsnooping -t eth1 > /dev/null 2>&1 &");
#else
		sprintf(cmdbuf,"/usr/sbin/dhcpsnooping -t eth0 > /dev/null 2>&1 &");
#endif
		bRet=_system(cmdbuf);
		CWWTPDebugLog("set dhcpsnooping start OK  !\n");
		if(debug_print)
			printf("set dhcpsnooping start OK  !\n");
	}
	return bRet;
}
CWBool CWEnable_linkcheck(int eth_num)
{
	char cmdbuf[128];
	memset(cmdbuf,0,128);
	sprintf(cmdbuf,"/usr/sbin/linkcheck eth%d >/dev/null 2>&1 &",eth_num);
	return _system(cmdbuf);
}
CWBool CWSaveACAddrFromDNS(char *ACDomainName)
{
	char cmdbuf[128];
	memset(cmdbuf,0,128);
	sprintf(cmdbuf, "nslookup6 -q=aaaa %s > /tmp/dns-ac-addr6", ACDomainName);
	return _system(cmdbuf);
}
int CWGetACCountFromDNS(char *ACDomainName)
{
	char str[64];	
	char tmp_str[128];
	memset(str,0,64);
	memset(tmp_str, 0, 128);
	
	sprintf(tmp_str,"cat /tmp/dns-ac-addr6 |grep %s| awk -F \" \" '{if($1==\"%s\")print $5}'| wc -l", ACDomainName,ACDomainName);
	PopenFile(tmp_str, str, sizeof(str));
	return atoi(str);
}
CWBool CWGetACAddrFromDNS(char *ACDomainName,int rowNum,char *ACAddr)
{
	char str[64];	
	char tmp_str[128];
	memset(str,0,64);
	memset(tmp_str, 0, 128);

	sprintf(tmp_str, "cat /tmp/dns-ac-addr6 |grep %s| awk -F \" \" '{if($1==\"%s\"&&NR==%d)print $5}'",ACDomainName,ACDomainName,rowNum);
	PopenFile(tmp_str,str,sizeof(str));
	strcpy(ACAddr,str);
	return CW_TRUE;
}
CWBool CWWTPSetStaticIP(WTPStaticIPValues *wtpStaticIPValues)
{
	char cmdbuf[128];
	unsigned char ip_str[24];
	unsigned char netmask_str[24];
	unsigned char gateway_str[24];
	
	if(wtpStaticIPValues == NULL) {return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);}

	if(!wtpStaticIPValues->static_IP_enable)
	{
		CWWTPDebugLog("WTP Static IP disable");
		if(debug_print)
			printf("WTP Static IP disable\n");
		return CW_TRUE;
	}
	CWWTPDebugLog("WTP Static IP enable");
	if(debug_print)
		printf("WTP Static IP enable\n");
	
	memset(ip_str, 0, 24);
	memset(netmask_str, 0, 24);
	memset(gateway_str, 0, 24);

	ip_int2str(wtpStaticIPValues->IP_addr, ip_str);
	ip_int2str(wtpStaticIPValues->netmask, netmask_str);
	ip_int2str(wtpStaticIPValues->gateway, gateway_str);

/*	if(wtpStaticIPValues->netmask == 8)
		strcpy(netmask_str, "255.0.0.0");
	else if(wtpStaticIPValues->netmask == 16)
		strcpy(netmask_str, "255.255.0.0");
	else if(wtpStaticIPValues->netmask == 24)
		strcpy(netmask_str, "255.255.255.0");
	if(0 == ip_int2str(wtpStaticIPValues->gateway, gateway_str))
		return CW_FALSE;
*/
	CWWTPDebugLog("ip:%s netmask:%s gateway:%s", ip_str, netmask_str, gateway_str);
	if(debug_print)
		printf("ip:%s netmask:%s gateway:%s\n", ip_str, netmask_str, gateway_str);

	memset(cmdbuf, 0, 128);
	sprintf(cmdbuf, "/usr/sbin/changeip static %s %s %s", ip_str, netmask_str, gateway_str);
	system(cmdbuf);
	CWSysReboot();
	return CW_TRUE;
}

CWBool CWWTPSetStaticIPV2(WTPStaticIPValues *wtpStaticIPValues)
{
	char cmdbuf[128];
	unsigned char ip_str[24];
	unsigned char netmask_str[24];
	unsigned char gateway_str[24];
	unsigned char firstdns_str[24];
	unsigned char secdns_str[24];
	
	if(wtpStaticIPValues == NULL) {return CWErrorRaise(CW_ERROR_WRONG_ARG, NULL);}
	
	memset(cmdbuf, 0, 128);
	memset(ip_str, 0, 24);
	memset(netmask_str, 0, 24);
	memset(gateway_str, 0, 24);
	memset(firstdns_str, 0, 24);
	memset(secdns_str, 0, 24);
	
	if(!wtpStaticIPValues->static_IP_enable){
		CWWTPDebugLog("WTP  use DHCP get IP");
		if(debug_print)
			printf("WTP  use DHCP get IP\n");
		sprintf(cmdbuf, "/usr/sbin/changeip2 dhcp");
		system(cmdbuf);
		CWSysReboot();
		return CW_TRUE;
	}
	CWWTPDebugLog("WTP Static IP enable");
	if(debug_print)
		printf("WTP Static IP enable\n");
	ip_int2str(wtpStaticIPValues->IP_addr, ip_str);
	ip_int2str(wtpStaticIPValues->netmask, netmask_str);
	ip_int2str(wtpStaticIPValues->gateway, gateway_str);
	ip_int2str(wtpStaticIPValues->firstdns, firstdns_str);
	ip_int2str(wtpStaticIPValues->secdns, secdns_str);

	CWWTPDebugLog("ip:%s netmask:%s gateway:%s firstdns_str:%s secdns_str:%s", ip_str, netmask_str, gateway_str, firstdns_str, secdns_str);
	if(debug_print)
		printf("ip:%s netmask:%s gateway:%s firstdns_str:%s secdns_str:%s\n", ip_str, netmask_str, gateway_str, firstdns_str, secdns_str);

	sprintf(cmdbuf, "/usr/sbin/changeip2 static %s %s %s %s %s", ip_str, netmask_str, gateway_str, firstdns_str, secdns_str);
	system(cmdbuf);
	CWSysReboot();
	return CW_TRUE;
}

void CWSetLastConnectLocalIP(void)
{
	char *local_ip_file = "/jffs/last_connect_ip";
	char temp_str[256];
	char cmdbuf[128];
	char str[64];
	
	memset(temp_str, 0, 256);
	sprintf(temp_str, "ifconfig default | awk -F \" \" '/inet addr/ {print $2}' | awk -F \":\" '{print $2}'");
	PopenFile(temp_str,str, sizeof(str));
//	if(debug_print)
//		printf("Last Connect Local IP: %s  len:%d\n", str, (int)strlen(str));
	
	if(strlen(str))
	{
		memset(cmdbuf, 0, 128);
		sprintf(cmdbuf, "echo %s > %s", str, local_ip_file);
		system(cmdbuf);
	}

	return;
}
CWBool CWSysReboot()
{
	return _system("sysreboot");
}
CWBool CWClearMemory()
{
	return _system("/usr/sbin/clearmemory");
}
CWBool CWGetUpgradeImage(unsigned char *image_name,unsigned char *ip,char *successfile)
{
	char cmdbuf[128];
	memset(cmdbuf,0,128);
	sprintf(cmdbuf, "cd /tmp && tftp -g -r %s %s && touch %s",image_name,ip,successfile);
	return _system(cmdbuf);
}
CWBool CWac_get()
{
	return _system("/usr/sbin/ac-get");
}
CWBool CWac_lose()
{
	return _system("/usr/sbin/ac-lose");
}
CWBool CWSetThinapState(int thinap_state)
{
	char cmdbuf[128];
	memset(cmdbuf,0,128);
	sprintf(cmdbuf, "echo %d > /proc/sys/dev/wifi0/thinap_state",thinap_state);
	return _system(cmdbuf);
}
CWBool CWSetThinapCheckTimer()
{
	char cmdbuf[128];
	memset(cmdbuf,0,128);
	sprintf(cmdbuf, "echo 0 > /proc/sys/dev/wifi0/thinap_check_timer");
	return system(cmdbuf);
}
CWBool CWSetNeighborDeadInterval(int neighborDeadInterval)
{
	char cmdbuf[128];
	memset(cmdbuf,0,128);
	sprintf(cmdbuf, "echo %d > /proc/sys/dev/wifi0/thinap_check_threshold",neighborDeadInterval);
	return _system(cmdbuf);
}
CWBool CWSysUpgrade(char *imageName)
{
	char cmdbuf[128];
	memset(cmdbuf,0,128);
	system("cp /usr/sbin/sysupgrade /tmp/");
	sprintf(cmdbuf, "cd /tmp && /tmp/sysupgrade %s",imageName);
	return _system(cmdbuf);
}
CWBool CWTime_Now(char *nowReadable,time_t now)
{
	char cmdbuf[128];
	memset(cmdbuf,0,128);
	sprintf(cmdbuf, "echo %s [%lu] >/tmp/echo_time",nowReadable, now);
	return system(cmdbuf);
}
CWBool CWSetAntennaNone(char *wlanname)
{
#ifdef WNAP350
		char cmdbuf[128];
		memset(cmdbuf,0,128);
		sprintf(cmdbuf, "/usr/sbin/setantenna350 %s antenna none", wlanname);
		system(cmdbuf);
#endif
	return CW_TRUE;
}
CWBool CWPingTestAndSave(char *ip,char *filename)
{
	char cmdbuf[128];
	
	memset(cmdbuf,0,128);
	sprintf(cmdbuf, "date > %s && echo >> %s", filename, filename);
	system(cmdbuf);
	
	memset(cmdbuf,0,128);
	sprintf(cmdbuf, "/usr/sbin/myping %s >> %s 2>&1", ip, filename);
	system(cmdbuf);

	memset(cmdbuf,0,128);
	sprintf(cmdbuf, "echo >> %s && date >> %s", filename, filename);
	system(cmdbuf);

	return CW_TRUE;
}
CWBool CWSta_Access_limit()
{
	system("killall sta_access_limit");
	return _system("sta_access_limit &");
}

CWBool acw_parse_dhcpsnooping_report(CWProtocolMessage *msgPtr, int len ,dhcpsnooping_report_type *valPtr)	
{
	int i;
	char *tmp_subtype;
	char ip_str[24];
	void *ptr = NULL;
	
	memset(ip_str, 0, 24);
	CWDebugLog("Parse dhcpsnooping_report message values");
	CWParseMessageElementStart();
//	valPtr->type= atoi(CWProtocolRetrieveStr(msgPtr,1));
	/*
         * BUG ML09
         * 19/10/2009 - Donato Capitella
         * 14/3/2011 - Pei Wenhui
         */
	ptr = CWProtocolRetrieveStr(msgPtr,1);
	valPtr->type = atoi(ptr);
	CW_FREE_OBJECT(ptr);
	tmp_subtype = CWProtocolRetrieveStr(msgPtr,2);
	/*10 means sta up line  11 means sta down line*/
	if(strcmp(tmp_subtype, "10")==0)   
		valPtr->subtype = 0;
	else if(strcmp(tmp_subtype, "11")==0)
		valPtr->subtype = 1;
	CW_FREE_OBJECT(tmp_subtype);
	
	for(i=0;i<6;i++){
		ptr = CWProtocolRetrieveStr(msgPtr, 2);
		valPtr->mac[i] = strtol(ptr, NULL, 16);
		CW_FREE_OBJECT(ptr);
	}
	if (debug_print){
		printf("dhcpsnooping report sta mac:");
		for(i=0;i<6;i++){
			printf("%02x ", valPtr->mac[i]);
		}
		printf("\n");
	}
	CWWTPDebugLog("dhcpsnooping report sta mac:[%02x-%02x-%02x-%02x-%02x-%02x]", valPtr->mac[0], valPtr->mac[1], 
		valPtr->mac[2], valPtr->mac[3], valPtr->mac[4], valPtr->mac[5]);
//	valPtr->radio_id = atoi(CWProtocolRetrieveStr(msgPtr,1));
//	valPtr->wlan_id = atoi(CWProtocolRetrieveStr(msgPtr,2));
//	valPtr->vlanid = atoi(CWProtocolRetrieveStr(msgPtr,4));
	ptr = CWProtocolRetrieveStr(msgPtr, 1);
	valPtr->radio_id = atoi(ptr);
	CW_FREE_OBJECT(ptr);
	
	ptr = CWProtocolRetrieveStr(msgPtr, 3); /*sun edited 2 to 3*/
	valPtr->wlan_id = atoi(ptr);
	CW_FREE_OBJECT(ptr);
	

	/*convert wlan id from local id(0~7 for marvel ap) to remote id (uncertain at AC) */
	valPtr->wlan_id = CWGetRemoteWlanID(valPtr->radio_id, valPtr->wlan_id);
	
	ptr = CWProtocolRetrieveStr(msgPtr, 4);
	valPtr->vlanid = atoi(ptr);
	CW_FREE_OBJECT(ptr);
	
	valPtr->ip_length = sizeof(valPtr->ip);
//	sscanf(CWProtocolRetrieveStr(msgPtr,len-22),"%u",&(valPtr->ip));
	ptr = CWProtocolRetrieveStr(msgPtr,len-23); /*sun edit 22 to 23*/
	sscanf(ptr, "%u", &(valPtr->ip));
	CW_FREE_OBJECT(ptr);

	ip_int2str(valPtr->ip, (unsigned char*)ip_str);
	CWWTPDebugLog("int_IP: %u   IP: %s", valPtr->ip, ip_str);
	CWWTPDebugLog("dhcpsnooping_report ( type = %d subtype = %d radio_id=%d wlan_id= %d vlanid= %d )",valPtr->type,valPtr->subtype,valPtr->radio_id,valPtr->wlan_id,valPtr->vlanid);
	if(debug_print){
		printf("int_IP: %u\n IP: %s\n", valPtr->ip, ip_str);
		printf("dhcpsnooping_report ( type = %d subtype = %d radio_id=%d wlan_id= %d vlanid= %d  ) \n",valPtr->type,valPtr->subtype,valPtr->radio_id,valPtr->wlan_id,valPtr->vlanid);
	}
	CWParseMessageElementEnd();
}
CWBool CWNtp_TimeSync(char *ip)
{
	char cmdbuf[128];
	memset(cmdbuf,0,128);
	sprintf(cmdbuf, "ntpclient -h %s -s &", ip);
	return _system(cmdbuf);
}




