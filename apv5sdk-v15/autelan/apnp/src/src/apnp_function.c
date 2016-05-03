#include<string.h>
#include<stdio.h>
#include<dlfcn.h>
#include<stdarg.h>
#include "apnp_log.h"

extern void *dl_handle;

char * dlsym_function(char *function_name,...)
{
	va_list argp;
	char *error=NULL;
	char *data=NULL;
	if(!function_name){
		ApnpDebugLog("funciton_name is NULL\n");
		return NULL;
	}
	va_start(argp,function_name);
	char *(*dl_fun_handle)(va_list);
	dl_fun_handle = dlsym(dl_handle,function_name);

	if((error = dlerror()) != NULL){
		ApnpDebugLog("error:%s\n",error); 
		return NULL;
	}
	data = (*dl_fun_handle)(argp);
	va_end(argp);
	return data;
}



void get_ap_ip(char Ip[])
{
	if (NULL == Ip) {
		ApnpDebugLog("get_ap_ip null point\n");
		return;
	}

	char cmd[256] = {0};
	snprintf(cmd, sizeof(cmd)-1, 
			 "/sbin/showdevinfo AP IP");
	FILE *fp = NULL;
	if (NULL != (fp = popen(cmd, "r")))
	{
		fgets(Ip,256,fp);
		pclose(fp);
	} 
	else
	{
		ApnpDebugLog("Failed to get AP IP !\n");
	}
}

void get_ap_mac(char Mac[])
{
	if (NULL == Mac) {
		ApnpDebugLog("get_ap_mac null point\n");
		return;
	}

	char cmd[256] = {0};
	snprintf(cmd, sizeof(cmd)-1, 
			 "/sbin/showdevinfo AP MAC");
	FILE *fp = NULL;
	if (NULL != (fp = popen(cmd, "r")))
	{
		fgets(Mac,256,fp);
		pclose(fp);
	} 
	else
	{
		ApnpDebugLog("Failed to get AP MAC !\n");
	}
}

void get_ap_name(char Name[])
{
	if (NULL == Name) {
		ApnpDebugLog("get_ap_name null point\n");
		return;
	}

	char cmd[256] = {0};
	snprintf(cmd, sizeof(cmd)-1, 
			 "/sbin/showdevinfo AP NAME");
	FILE *fp = NULL;
	if (NULL != (fp = popen(cmd, "r")))
	{
		fgets(Name,256,fp);
		pclose(fp);
	} 
	else
	{
		ApnpDebugLog("Failed to get AP NAME !\n");
	}
}

void get_ap_apname(char APName[])
{
	if (NULL == APName) {
		ApnpDebugLog("get_ap_apname null point\n");
		return;
	}

	char cmd[256] = {0};
	snprintf(cmd, sizeof(cmd)-1,"/sbin/showdevinfo AP APNAME");
	FILE *fp = NULL;
	if (NULL != (fp = popen(cmd, "r")))
	{
		fgets(APName,256,fp);
		pclose(fp);
	} 
	else
	{
		ApnpDebugLog("Failed to get AP APNAME !\n");
	}
}

void get_ap_soft_version(char Version[])
{
	if (NULL == Version) {
		ApnpDebugLog("get_ap_soft_version null point\n");
		return;
	}

	char cmd[256] = {0};
	snprintf(cmd, sizeof(cmd)-1, 
			 "/sbin/showdevinfo AP SOFT_VERSION");
	FILE *fp = NULL;
	if (NULL != (fp = popen(cmd, "r")))
	{
		fgets(Version,256,fp);
		pclose(fp);
	} 
	else
	{
		ApnpDebugLog("Failed to get AP SOFT_VERSION !\n");
	}
}

void get_ap_radio_count(char RadioC[])
{
	if (NULL == RadioC) {
		ApnpDebugLog("get_ap_radio_count null point\n");
		return;
	}

	char cmd[256] = {0};
	snprintf(cmd, sizeof(cmd)-1, 
			 "/sbin/showdevinfo AP RADIO_COUNT");
	FILE *fp = NULL;
	if (NULL != (fp = popen(cmd, "r")))
	{
		fgets(RadioC,256,fp);
		pclose(fp);
	} 
	else
	{
		ApnpDebugLog("Failed to get AP RADIO_COUNT !\n");
	}
}

void get_ap_radioif(char RadioIf[])
{
	if (NULL == RadioIf) {
		ApnpDebugLog("get_ap_radioif null point\n");
		return;
	}

	char cmd[256] = {0};
	snprintf(cmd, sizeof(cmd)-1, 
			 "/sbin/showdevinfo AP RADIOIF");
	FILE *fp = NULL;
	if (NULL != (fp = popen(cmd, "r")))
	{
		fgets(RadioIf,256,fp);
		pclose(fp);
	} 
	else
	{
		ApnpDebugLog("Failed to get AP RADIOIF !\n");
	}
}

void get_ap_stacount(char StaC[])
{
	if (NULL == StaC) {
		ApnpDebugLog("get_ap_stacount null point\n");
		return;
	}

	char cmd[256] = {0};
	snprintf(cmd, sizeof(cmd)-1, 
			 "/sbin/showdevinfo AP STACOUNT");
	FILE *fp = NULL;
	if (NULL != (fp = popen(cmd, "r")))
	{
		fgets(StaC,256,fp);
		pclose(fp);
	} 
	else
	{
		ApnpDebugLog("Failed to get AP STACOUNT !\n");
	}
}




void get_radio_id(const char* Radio,char RadioId[])
{
	if (NULL == Radio || NULL == RadioId) {
		ApnpDebugLog("get_radio_id null point\n");
		return;
	}

	char cmd[256] = {0};
	snprintf(cmd, sizeof(cmd)-1, 
			 "/sbin/showdevinfo RADIO %s ID",Radio);
	FILE *fp = NULL;
	if (NULL != (fp = popen(cmd, "r")))
	{
		fgets(RadioId,256,fp);
		pclose(fp);
	} 
	else
	{
		ApnpDebugLog("Failed to get RADIO %s ID !\n",Radio);
	}
}

void get_radio_channel(const char* Radio,char RadioCh[])
{
	if (NULL == Radio || NULL == RadioCh) {
		ApnpDebugLog("get_radio_channel null point\n");
		return;
	}

	char cmd[256] = {0};
	snprintf(cmd, sizeof(cmd)-1, 
			 "/sbin/showdevinfo RADIO %s CHANNEL",Radio);
	FILE *fp = NULL;
	if (NULL != (fp = popen(cmd, "r")))
	{
		fgets(RadioCh,256,fp);
		pclose(fp);
	} 
	else
	{
		ApnpDebugLog("Failed to get RADIO %s CHANNEL !\n",Radio);
	}
}

void get_radio_mode(const char* Radio,char RadioMode[])
{
	if (NULL == Radio || NULL == RadioMode) {
		ApnpDebugLog("get_radio_mode null point\n");
		return;
	}

	char cmd[256] = {0};
	snprintf(cmd, sizeof(cmd)-1, 
			 "/sbin/showdevinfo RADIO %s MODE",Radio);
	FILE *fp = NULL;
	if (NULL != (fp = popen(cmd, "r")))
	{
		fgets(RadioMode,256,fp);
		pclose(fp);
	} 
	else
	{
		ApnpDebugLog("Failed to get RADIO %s MODE !\n",Radio);
	}
}

void get_radio_rate(const char* Radio,char RadioRate[])
{
	if (NULL == Radio || NULL == RadioRate) {
		ApnpDebugLog("get_radio_rate null point\n");
		return;
	}

	char cmd[256] = {0};
	snprintf(cmd, sizeof(cmd)-1, 
			 "/sbin/showdevinfo RADIO %s RATE",Radio);
	FILE *fp = NULL;
	if (NULL != (fp = popen(cmd, "r")))
	{
		fgets(RadioRate,256,fp);
		pclose(fp);
	} 
	else
	{
		ApnpDebugLog("Failed to get RADIO %s RATE !\n",Radio);
	}
}

void get_radio_power(const char* Radio,char RadioPower[])
{
	if (NULL == Radio || NULL == RadioPower) {
		ApnpDebugLog("get_radio_power null point\n");
		return;
	}
	
	char cmd[256] = {0};
	snprintf(cmd, sizeof(cmd)-1, 
			 "/sbin/showdevinfo RADIO %s POWER",Radio);
	FILE *fp = NULL;
	if (NULL != (fp = popen(cmd, "r")))
	{
		fgets(RadioPower,256,fp);
		pclose(fp);
	} 
	else
	{
		ApnpDebugLog("Failed to get RADIO %s POWER !\n",Radio);
	}
}

void get_radio_wlancount(const char* Radio,char RadioWlanC[])
{
	if (NULL == Radio || NULL == RadioWlanC) {
		ApnpDebugLog("get_radio_wlancount null point\n");
		return;
	}

	char cmd[256] = {0};
	snprintf(cmd, sizeof(cmd)-1, 
			 "/sbin/showdevinfo RADIO %s WLANCOUNT",Radio);
	FILE *fp = NULL;
	if (NULL != (fp = popen(cmd, "r")))
	{
		fgets(RadioWlanC,256,fp);
		pclose(fp);
	} 
	else
	{
		ApnpDebugLog("Failed to get RADIO %s WLANCOUNT !\n",Radio);
	}
}

void get_radio_maxassoc(const char* Radio,char RadioMaxAC[])
{
	if (NULL == Radio || NULL == RadioMaxAC) {
		ApnpDebugLog("get_radio_maxassoc null point\n");
		return;
	}
	char cmd[256] = {0};
	snprintf(cmd, sizeof(cmd)-1, 
			 "/sbin/showdevinfo RADIO %s MAXASSOC",Radio);
	FILE *fp = NULL;
	if (NULL != (fp = popen(cmd, "r")))
	{
		fgets(RadioMaxAC,256,fp);
		pclose(fp);
	} 
	else
	{
		ApnpDebugLog("Failed to get RADIO %s MAXASSOC !\n",Radio);
	}
}

void get_radio_wlanif(const char* Radio,char RadioWlanIf[])
{
	if (NULL == Radio || NULL == RadioWlanIf) {
		ApnpDebugLog("get_radio_wlanif null point\n");
		return;
	}
	
	char cmd[256] = {0};
	ApnpDebugLog(cmd, sizeof(cmd)-1, 
			 "/sbin/showdevinfo RADIO %s WLANIF",Radio);
	FILE *fp = NULL;
	if (NULL != (fp = popen(cmd, "r")))
	{
		fgets(RadioWlanIf,256,fp);
		pclose(fp);
	} 
	else
	{
		ApnpDebugLog("Failed to get RADIO %s WLANIF !\n",Radio);
	}
}



void get_wlan_name(const char* Wlan,char WlanName[])
{
	

	char cmd[256] = {0};
	FILE *fp = NULL;

	if (NULL == Wlan || NULL == WlanName) {
		ApnpDebugLog("get_wlan_name null point\n");
		return;
	}

	
	snprintf(cmd, sizeof(cmd)-1, 
			 "/sbin/showdevinfo WLAN %s NAME ", Wlan);
	fp = popen(cmd, "r");
	
	if (NULL != fp)
	{
		fgets(WlanName,256,fp);
		pclose(fp);
	} 
	else
	{
		ApnpDebugLog("Failed to get WLAN %s NAME !\n",Wlan);
	}

}

void get_wlan_essid(const char* Wlan,char WlanEssid[])
{
	if (NULL == Wlan || NULL == WlanEssid) {
		ApnpDebugLog("get_wlan_essid null point\n");
		return;
	}
	
	char cmd[256] = {0};
	snprintf(cmd, sizeof(cmd)-1, 
			 "/sbin/showdevinfo WLAN %s ESSID",Wlan);
	FILE *fp = NULL;
	if (NULL != (fp = popen(cmd, "r")))
	{
		fgets(WlanEssid,256,fp);
		pclose(fp);
	} 
	else
	{
		ApnpDebugLog("Failed to get WLAN %s ESSID !\n",Wlan);
	}
}

void get_wlan_securitytype(const char* Wlan,char WlanSeType[])
{
	if (NULL == Wlan || NULL == WlanSeType) {
		printf("get_wlan_securitytype null point\n");
		return;
	}
	
	char cmd[256] = {0};
	snprintf(cmd, sizeof(cmd)-1, 
			 "/sbin/showdevinfo WLAN %s SECURITYTYPE",Wlan);
	FILE *fp = NULL;
	if (NULL != (fp = popen(cmd, "r")))
	{
		fgets(WlanSeType,256,fp);
		fgets(WlanSeType,256,fp);
		pclose(fp);
	} 
	else
	{
		ApnpDebugLog("Failed to get WLAN %s SECURITYTYPE !\n",Wlan);
	}
}

void get_wlan_securitykey(const char* Wlan,char WlanSeKey[])
{
	if (NULL == Wlan || NULL == WlanSeKey) {
		ApnpDebugLog("get_wlan_securitykey null point\n");
		return;
	}
	
	char cmd[256] = {0};
	snprintf(cmd, sizeof(cmd)-1, 
			 "/sbin/showdevinfo WLAN %s SECURITYKEY",Wlan);
	FILE *fp = NULL;
	if (NULL != (fp = popen(cmd, "r")))
	{
		fgets(WlanSeKey,256,fp);
		fgets(WlanSeKey,256,fp);
		pclose(fp);
	} 
	else
	{
		ApnpDebugLog("Failed to get WLAN %s SECURITYKEY !\n",Wlan);
	}
}



