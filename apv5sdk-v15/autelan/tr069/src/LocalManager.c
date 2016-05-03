
/**********************************************************************************
* Copyright (c) 2008-2011  Beijing Autelan Technology Co. Ltd.
* All rights reserved.
*
* filename: LocalManager.c  
* description:  implementation for implementation of Local Manager 
* - Table with row (value, data model, notification type, access conditions)
* - Create a thread server (requests from Data model repository) 
* - Subscribe to Data model repository (XML message)
* - Poll changes on variables with Active notification  
************************************************************************************/

#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <sys/sysinfo.h>
#include <sys/utsname.h>

#include <pthread.h>
#include <semaphore.h>


#include "const.h"
#include "messages.h"
#include "Manager.h"
#include "LocalManager.h"
#include "LocalManagerDefinition.h"


#include "tools.h"
#include "FileConf.h"
#include "StubH.h"


#include <ap_interface.h>

#define SPRINT_MAX_LEN 100
#define DEFAULT_MANUFACTURER                                    "Autelan"  //add by wangjr 091124
#define DEFAULT_O_U_I                                           "001F64" // add by wangjr 091124

#ifdef __cplusplus
extern "C" {
#endif


struct Namespace *namespaces;

// data model repository server address
char gdatamodel_address[ARGSIZE];

// synchronization
static sem_t S_response;
static short Response_OK = 0;
static short Request_OK = 0;

static int gId = -1;
static int gnbParameters=0;

// global var
static char szHardwareVersion[20]="FR7100-LAB0-C30";
static char szConnectionRequestURL[20];

// temp value
char szVal[256];
char athnum[2]={0};
char wepkeynum[2]={0};
char gPingHost[50]={0};
int gPingNum=0;
int gPingSize=0;
static char gPingSuc[8];
static char gPingFal[8];
static char gPingMin[8];
static char gPingAvg[8];
static char gPingMax[8];
static char gPingRes[16];
static char g_alarm_mode[8] = "3";


int Get_vapnum(char * str_arg)
{
  	char *ptr=NULL;
	if((ptr=strstr(str_arg,"WLANConfiguration."))!=NULL)
	{   
		memset(athnum,0,sizeof(athnum));
		strncpy(athnum,ptr+18,1);
	}
	return atoi(athnum);

}

char* Get_wepkeynum(char * str_arg)
{
	char *ptr=NULL;
	if((ptr=strstr(str_arg,"WEPKey."))!=NULL)
	{	
		memset(wepkeynum,0,sizeof(wepkeynum));
		strncpy(wepkeynum,ptr+7,1);
	}
	return  wepkeynum;
}

void CGetChar(int *sum, char* Src)
{
	unsigned j = 0;
	long IsLeft = 1;
	char buffer[10]= {0};
    unsigned nStart= 0;
	for (nStart = 0; nStart < strlen(Src); nStart++)
	{ 
		if (Src[nStart] == ' ' || Src[nStart] == '\t' 
			|| Src[nStart] == '\r' || Src[nStart] == '\0' || Src[nStart] == '\n')
		{
			if (IsLeft)
				continue;
			else 
				break;
		}
		buffer[j++] = Src[nStart];		
		IsLeft = 0;
	}
	
	*sum += atoi(buffer);
	
}

double GetPercentage(int firstNumber, int secondNumber)
{
	return (firstNumber/(double)secondNumber);
}
char *GetEnable(char *str_arg)
{
   memset(szVal, 0x00, sizeof(szVal));
   wireless_get_enable(Get_vapnum(str_arg),szVal,sizeof(szVal));
   if(strcmp(szVal,"yes") == 0)
   {
   	   memset(szVal, 0x00, sizeof(szVal));
       strcpy(szVal,"enable");
   }
   else if(strcmp(szVal,"no") == 0)
   {
       memset(szVal, 0x00, sizeof(szVal));
       strcpy(szVal,"disable");
   }
   return szVal; //don't return local variable
}

int SetEnable(char *str_arg,char *str_szval)
{
    memset(szVal, 0x00, sizeof(szVal));
    if(strcmp(str_szval,"enable")==0)
    {
    	wireless_config_enable(Get_vapnum(str_arg),"yes");	
    }
	else if(strcmp(str_szval,"disable")==0)
	{
	    wireless_config_enable(Get_vapnum(str_arg),"no");
	}
	else
	{
	   printf("invalid argment");
	}
    return 0; //don't return local variable
}

char *GetSSID(char *str_arg)
{
   memset(szVal, 0x00, sizeof(szVal));
   wireless_get_ssid(Get_vapnum(str_arg),szVal,sizeof(szVal));
   return szVal;
}

int SetSSID(char *str_arg,char *str_szval)
{
   memset(szVal, 0x00, sizeof(szVal));
   wireless_config_ssid(Get_vapnum(str_arg),str_szval);
   return 0;
}

char *GetSSIDManage(char *str_arg)
{
	FILE   *stream; 
	int vapnum=Get_vapnum(str_arg);
	char str_cmd[SPRINT_MAX_LEN]={0};
    sprintf(str_cmd,"/usr/sbin/read_manager_conf %d",vapnum);
	memset(szVal, 0x00, sizeof(szVal));
	stream = popen( str_cmd, "r" );
	if(stream)
	{
	fread( szVal, sizeof(char), sizeof(szVal),	stream); 
		pclose( stream );
	}
	szVal[strlen(szVal)-1]='\0';
	return szVal;

}

int SetSSIDManage(char *str_arg,char *str_szval)
{
   int vapnum=Get_vapnum(str_arg);
   char str_cmd[SPRINT_MAX_LEN]={0};
   if(strcmp(str_szval,"enable") == 0)
   {
       sprintf(str_cmd,"/usr/sbin/manager_vap_conf %d  yes",vapnum);
   }
   else
   {
   	   sprintf(str_cmd,"/usr/sbin/manager_vap_conf %d  no",vapnum);
   }
   system(str_cmd);
   return 0;
}
/////////////////update by wangjr 091124//////////////////////////////////////
char * GetPowerValue(char *str_arg)
{

	FILE   *stream; 
	int vapnum=Get_vapnum(str_arg);
	memset(szVal, 0x00, sizeof(szVal));
	char str_cmd[SPRINT_MAX_LEN]={0};
	
	sprintf(str_cmd,"/usr/sbin/iwconfig ath%d |grep  -o -i -e 'Tx-Power:[0-9]\\{1,2\\}' |cut -d: -f2",vapnum-1);

	stream = popen( str_cmd, "r" );
	if(stream)
	{
	fread( szVal, sizeof(char), sizeof(szVal),	stream); 
		pclose( stream );
	}
	szVal[strlen(szVal)-1]='\0';
	return szVal;

}





char *GetPowerlevel(char *str_arg)
{
   int power=0;
   memset(szVal, 0x00, sizeof(szVal));
   wireless_get_txpower(Get_vapnum(str_arg),szVal,sizeof(szVal));
   
   power=atoi(szVal);
   
   if((power >=1) && (power <=6))
   {
		strcpy(szVal,"1");
   }
   else if((power >=7) && (power <=12)) 
   {
   		strcpy(szVal,"2");
   }
   else if((power >=13) && (power <=18)) 
   {
   		strcpy(szVal,"3");
   }
   else if((power >=19) && (power <=24)) 
   {
   		strcpy(szVal,"4");
   }
   else 
   {
   		strcpy(szVal,"5");
   }
   	
   return szVal;
}


int SetPowerlevel(char *str_arg,char *str_szval)
{
   if(strcmp(str_szval,"1")==0)
   {
		wireless_config_txpower(Get_vapnum(str_arg),"1");
   }
   else if(strcmp(str_szval,"2")==0)
   {
   		wireless_config_txpower(Get_vapnum(str_arg),"7");
   }
   else if(strcmp(str_szval,"3")==0)
   {
   		wireless_config_txpower(Get_vapnum(str_arg),"13");
   } 
   else if(strcmp(str_szval,"4")==0)
   {
   		wireless_config_txpower(Get_vapnum(str_arg),"19");
   }
   else
   {
   		wireless_config_txpower(Get_vapnum(str_arg),"26");
   }
   return 0;
}

char * GetAPModuleEnable(char *str_arg)
{

   memset(szVal, 0x00, sizeof(szVal));
   wireless_get_enable(Get_vapnum(str_arg),szVal,sizeof(szVal));
   if(strcmp(szVal,"yes")==0)
   {
   	   memset(szVal, 0x00, sizeof(szVal));
       strcpy(szVal,"enable");
   }
   else if(strcmp(szVal,"no")==0)
   {
       memset(szVal, 0x00, sizeof(szVal));
       strcpy(szVal,"disable");
   }
   return szVal; //don't return local variable


}

int SetAPModuleEnable(char *str_arg,char *str_szval)
{

   memset(szVal, 0x00, sizeof(szVal));
   if(strcmp(str_szval,"enable")==0)
   {
	   wireless_config_enable(Get_vapnum(str_arg),"yes");  
   }
   else if(strcmp(str_szval,"disable")==0)
   {
	   wireless_config_enable(Get_vapnum(str_arg),"no");
   }
   else
   {
	  printf("invalid argment");
   }
   return 0;

}

char * GetRadioEnabled(char *str_arg)
{
	memset(szVal, 0x00, sizeof(szVal));
	wireless_get_enable(Get_vapnum(str_arg),szVal,sizeof(szVal));
	if(strcmp(szVal,"yes")==0)
	{
		memset(szVal, 0x00, sizeof(szVal));
		strcpy(szVal,"enable");
	}
	else if(strcmp(szVal,"no")==0)
	{
		memset(szVal, 0x00, sizeof(szVal));
		strcpy(szVal,"disable");
	}
	return szVal; //don't return local variable

}


int SetRadioEnabled(char *str_arg,char *str_szval)
{

   memset(szVal, 0x00, sizeof(szVal));
   if(strcmp(str_szval,"enable")==0)
   {
	   wireless_config_enable(Get_vapnum(str_arg),"yes");  
   }
   else if(strcmp(str_szval,"disable")==0)
   {
	   wireless_config_enable(Get_vapnum(str_arg),"no");
   }
   else
   {
	  printf("invalid argment");
   }
   return 0;

}


//#if 0 //CNC
char * GetSSIDAdvertisementEnable(char *str_arg)
{

   memset(szVal, 0x00, sizeof(szVal));
   wireless_get_broadcastssid(Get_vapnum(str_arg),szVal,sizeof(szVal));
   return szVal;
}
//#endif

char * GetSSIDHide(char *str_arg)
{
   memset(szVal, 0x00, sizeof(szVal));
   wireless_get_broadcastssid(Get_vapnum(str_arg),szVal,sizeof(szVal));
   if(strcmp(szVal,"no") == 0)
   {
       memset(szVal, 0x00, sizeof(szVal));
       strcpy(szVal,"enable");
   }
   else if(strcmp(szVal,"yes") == 0)
   {   
        memset(szVal, 0x00, sizeof(szVal));
   		strcpy(szVal,"disable");
   }
   else
   {
       printf("valid arg");
   }
   return szVal;
}

int SetSSIDHide(char *str_arg,char *str_szval)
{
    if(strcmp(str_szval,"disable")==0)
    {
    	wireless_config_hidessid(Get_vapnum(str_arg),"0");	
    }
	else if(strcmp(str_szval,"enable")==0)
	{
	    wireless_config_hidessid(Get_vapnum(str_arg),"1");
	}
	else
	{
	   printf("invalid argment");
	}
    return 0; //don't return local variable
}
char *GetBSSID(char *str_arg)
{
    memset(szVal, 0x00, sizeof(szVal));
	//strcpy(szVal,"bssid");
    wireless_get_bssid(Get_vapnum(str_arg),szVal,sizeof(szVal));
    return szVal;
}

char *GetStatus(char *str_arg)
{
	memset(szVal, 0x00, sizeof(szVal));
    wireless_get_status(Get_vapnum(str_arg),szVal,sizeof(szVal));
    return szVal;
}

#if 0
char *GetInterfaceEnable(char *str_arg)
{
   memset(szVal, 0x00, sizeof(szVal));
   wireless_get_enable(Get_athnum(str_arg),szVal,sizeof(szVal));
   return szVal; //don't return local variable
}
#endif
char *GetChannel(char *str_arg)
{
   memset(szVal, 0x00, sizeof(szVal));
   wireless_get_channel(Get_vapnum(str_arg),szVal,sizeof(szVal));
   return szVal; //don't return local variable
}

int SetChannel(char *str_arg,char *str_szval)
{
   wireless_config_channel(Get_vapnum(str_arg),str_szval);
   return 0; 
}


char *GetChannelsInUse(char *str_arg)
{
	FILE   *stream; 
	memset(szVal, 0x00, sizeof(szVal));
	stream = popen( "/usr/sbin/iwconfig ath0 |grep  -o -i -e 'Frequency:[0-9]\\.[0-9]\\{1,3\\}'|/usr/bin/cut -d: -f2", "r" );
	if(stream)
	{
    fread( szVal, sizeof(char), sizeof(szVal),  stream); 
		pclose( stream );
	}
	szVal[strlen(szVal)-1]='\0';
    return szVal;
}
int SetSSIDAdvertisementEnable(char *str_arg,char *str_szval)
{
    if(strcmp(str_szval,"enable")==0)
    {
    	wireless_config_hidessid(Get_vapnum(str_arg),"0");	
    }
	else if(strcmp(str_szval,"disable")==0)
	{
	    wireless_config_hidessid(Get_vapnum(str_arg),"1");
	}
	else
	{
	   printf("invalid argment");
	}
    return 0; //don't return local variable
}

char * GetWEPKeyIndex(char *str_arg)
{
    memset(szVal, 0x00, sizeof(szVal));
	wireless_sec_get_wepkey_seq(Get_vapnum(str_arg),szVal,sizeof(szVal));
	return szVal;
}

int SetBeaconType(char *str_arg,char *str_szval)
{
    memset(szVal, 0x00, sizeof(szVal));
	wireless_sec_set_type(Get_vapnum(str_arg),str_szval);
    return 0; 
}

char * GetBeaconType(char *str_arg)
{
	memset(szVal, 0x00, sizeof(szVal));
	wireless_sec_get_type( Get_vapnum(str_arg),szVal, sizeof(szVal));
	return szVal; 

}

char * GetBasicAuthencationMode(char *str_arg)
{
    memset(szVal, 0x00, sizeof(szVal));
    wireless_sec_get_basicmode(Get_vapnum(str_arg),szVal,sizeof(szVal));
	if(atoi(szVal) == 1)
	{
		memset(szVal, 0x00, sizeof(szVal));
		strcpy(szVal,"OpenSystem");	
	}
	else if(atoi(szVal) == 2)
	{
	    memset(szVal, 0x00, sizeof(szVal));
		strcpy(szVal,"SharedKey");
	}
	else
	{
	    memset(szVal, 0x00, sizeof(szVal));
		strcpy(szVal,"Both");
	}
    return szVal;
}
int  SetBasicAuthencationMode(char *str_arg,char *str_szval)
{
   memset(szVal, 0x00, sizeof(szVal));
   
   if(strcmp(str_szval,"OpenSystem")==0)
   {
		wireless_sec_set_mode( Get_vapnum(str_arg), "open",0);
   }
   else if(strcmp(str_szval,"SharedKey")==0)
   {
		wireless_sec_set_mode( Get_vapnum(str_arg), "psk",0);
   }
   else
   {
		wireless_sec_set_mode( Get_vapnum(str_arg), "802.1x",0);
   }
   return 0;
}
char * GetWEPEncryptionLevel(char *str_arg)
{
   memset(szVal, 0x00, sizeof(szVal));
   wireless_sec_get_wepkey_level(Get_vapnum(str_arg),szVal,sizeof(szVal));
   return szVal;
}
int  SetWEPEncryptionLevel(char *str_arg,char *str_szval)
{
   if(strcmp(str_szval,"64")==0)
   {
       wireless_sec_set_encrypt( Get_vapnum(str_arg), "wep64", 0 );
   }
   else if(strcmp(str_szval,"128")==0)
   {
       wireless_sec_set_encrypt( Get_vapnum(str_arg), "wep128", 0 );
   }
   else
   {
       printf("valid problem \n");
   }
   return 0;
}
char * GetStandard(char *str_arg)
	
{
	memset(szVal, 0x00, sizeof(szVal));
	wireless_get_mode(Get_vapnum(str_arg),szVal,sizeof(szVal));//a,b,g
	return szVal;
}

int SetStandard(char *str_arg,char *str_szval)
{ 
   memset(szVal, 0x00, sizeof(szVal));
   wireless_config_mode(Get_vapnum(str_arg),str_szval);
   return 0; 
}
char * GetBasicEncryptionModes(char *str_arg)
{
   memset(szVal, 0x00, sizeof(szVal));
   wireless_sec_get_type(Get_vapnum(str_arg),szVal,sizeof(szVal));
   return szVal;
}
char * GetWPAEncryptionModes(char *str_arg)
{
   memset(szVal, 0x00, sizeof(szVal));
   wireless_sec_get_wpaencrypt( Get_vapnum(str_arg),szVal, sizeof(szVal) );
   return szVal; 
}
int SetWPAEncryptionModes(char *str_arg,char *str_szval)
{
   memset(szVal, 0x00, sizeof(szVal));
   wireless_sec_set_encrypt( Get_vapnum(str_arg), str_szval, 0 );                             
   return szVal; 
}

char * GetPossibleChannels(char *str_arg)
{
    memset(szVal, 0x00, sizeof(szVal));
	wireless_get_region(Get_vapnum(str_arg),szVal,sizeof(szVal));
	if((strstr(szVal,"Asia")!=NULL)||(strstr(szVal,"Europe")!=NULL))
	{
	   memset(szVal, 0x00, sizeof(szVal));
	   strcpy(szVal,"1-15");
	}
	else if(strstr(szVal,"America")!=NULL)
	{
	   memset(szVal, 0x00, sizeof(szVal));
	   strcpy(szVal,"1-13");
	}
	else
	{
	   memset(szVal, 0x00, sizeof(szVal));
	   strcpy(szVal,"auto channel");
	}
	return szVal;

}
char * GetAutoChannelEnable(char *str_arg)
{
    memset(szVal, 0x00, sizeof(szVal));
	wireless_get_channel(Get_vapnum(str_arg),szVal,sizeof(szVal));
	if(atoi(szVal)==0)
	{
	   memset(szVal, 0x00, sizeof(szVal));
	   strcpy(szVal,"enable");
	}
	else
	{
	   memset(szVal, 0x00, sizeof(szVal));
	   strcpy(szVal,"disable");
	}
	return szVal;
}
char * GetWEPKey(char *str_arg)
{
	memset(szVal, 0x00, sizeof(szVal));
	wireless_sec_get_wepkey( Get_vapnum(str_arg), Get_wepkeynum(str_arg),szVal,sizeof(szVal) );
    return szVal;
}

int SetWEPKey(char *str_arg,char *str_szval)
{
	wireless_sec_set_wepkey(Get_vapnum(str_arg),Get_wepkeynum(str_arg),"hex",str_szval,0);
}
char * GetPreSharedKey(char *str_arg)
{
    memset(szVal, 0x00, sizeof(szVal));
    return szVal;
}
char * GetKeyPassphrase(char *str_arg)
{
    memset(szVal, 0x00, sizeof(szVal));

	//strcpy(szVal,"GetKeyPassphrase");
	wireless_sec_get_wpakey(Get_vapnum(str_arg),szVal,sizeof(szVal));
    return szVal;
}
int SetPreSharedKey(char *str_arg,char *str_szval)
{
    return 0;
}
int SetKeyPassphrase(char *str_arg,char *str_szval)
{
    memset(szVal, 0x00, sizeof(szVal));
    wireless_sec_set_wpakey( Get_vapnum(str_arg), str_arg, 0);								
	return 0;
}
////////////////////////////update by wangjr 091124///////////////////////////////////////
char * GetReceiveNoise(char *str_arg)
{
	FILE *fp;
	char str_tmp_cmd[SPRINT_MAX_LEN];
	memset( str_tmp_cmd, 0, SPRINT_MAX_LEN );
	int vapnum=Get_vapnum(str_arg);
	memset(szVal, 0x00, sizeof(szVal));
	sprintf( str_tmp_cmd, "/usr/sbin/iwconfig ath%d |grep -o -i -e 'Noise level=-[0-9]\\{1,2\\}'|/usr/bin/cut -d= -f2",vapnum-1);
	fp=popen(str_tmp_cmd,"r");
	if(fp)
	{
	fgets(szVal,sizeof(szVal),fp);
		pclose(fp);
	}
	szVal[strlen(szVal)-1] = '\0';
    return szVal;
}
char * GetLANTotalBytesSent(char *str_arg)
{
	FILE *fp;
	//int sum = 0;/*update by wangjr 2010-12-03*/
	unsigned long long sum =0;
	char str_tmp_cmd[SPRINT_MAX_LEN];
	memset(szVal, 0x00, sizeof(szVal));
	memset( str_tmp_cmd, 0, SPRINT_MAX_LEN );
	sprintf( str_tmp_cmd, "%s", "/sbin/ifconfig |awk 'BEGIN{RS=\"\\n\\n\"}/ath/{print $0}'|awk -F \"[ ]+|:\" '/TX bytes/{print $9}'");
	fp=popen(str_tmp_cmd,"r");
	if(fp)
	{
	   memset(szVal, 0x00, sizeof(szVal));
	   while(fgets(szVal,sizeof(szVal),fp)) 
	   {
			 //CGetChar(&sum, szVal); /*update by wangjr 2010-12-03*/
			 sscanf(szVal,"%llu",&sum);
	   }
	   sprintf(szVal,"%llu",sum);/*update by wangjr 2010-12-03*/
	   pclose(fp);
	}
	return szVal;

}

char * GetLANTotalBytesReceived(char *str_arg)
{
	memset(szVal, 0x00, sizeof(szVal));
	FILE *fp;
	//int sum = 0; /*update by wangjr 2010-12-03*/
	unsigned long long sum =0;
	char str_tmp_cmd[SPRINT_MAX_LEN];
 	memset( str_tmp_cmd, 0, SPRINT_MAX_LEN );
	memset(szVal, 0x00, sizeof(szVal));
	sprintf( str_tmp_cmd, "%s", "/sbin/ifconfig |awk 'BEGIN{RS=\"\\n\\n\"}/ath/{print $0}'|awk -F \"[ ]+|:\" '/RX bytes/{print $4}'");
	fp=popen(str_tmp_cmd,"r");
	if(fp)
	{
	   memset(szVal, 0x00, sizeof(szVal));
		while(fgets(szVal,sizeof(szVal),fp)) 
		{
			//CGetChar(&sum, szVal);/*update by wangjr 2010-12-03*/
			sscanf(szVal,"%llu",&sum);
		}
		sprintf(szVal,"%llu",sum);/*update by wangjr 2010-12-03*/
		pclose(fp);
	}
	return szVal;

}

char * GetWANTotalBytesSent(char *str_arg)
{

	FILE *fp;
	//int sum = 0; /*update by wangjr 2010-12-03*/
	unsigned long long sum =0;
	char str_tmp_cmd[SPRINT_MAX_LEN];
	memset( str_tmp_cmd, 0, SPRINT_MAX_LEN );
	memset(szVal, 0x00, sizeof(szVal));
	sprintf( str_tmp_cmd, "%s", "/sbin/ifconfig |awk 'BEGIN{RS=\"\\n\\n\"}/eth/{print $0}'|awk -F \"[ ]+|:\" '/TX bytes/{print $9}'");
	fp=popen(str_tmp_cmd,"r");
	if(fp)
	{
		memset(szVal, 0x00, sizeof(szVal));
		while(fgets(szVal,sizeof(szVal),fp)) 
		{
			//CGetChar(&sum, szVal);/*update by wangjr 2010-12-03*/
			sscanf(szVal,"%llu",&sum);
		}
		sprintf(szVal,"%llu",sum); /*update by wangjr 2010-12-03*/
		pclose(fp);
	}
	
	return szVal;

}

char * GetWANTotalBytesReceived(char *str_arg)
{

	FILE *fp;
	//int sum = 0;   /*update by wangjr 2010-12-03*/
	unsigned long long sum =0;
	char str_tmp_cmd[SPRINT_MAX_LEN];
	memset( str_tmp_cmd, 0, SPRINT_MAX_LEN );
	memset(szVal, 0x00, sizeof(szVal));
	sprintf( str_tmp_cmd, "%s", "/sbin/ifconfig |awk 'BEGIN{RS=\"\\n\\n\"}/eth/{print $0}'|awk -F \"[ ]+|:\" '/RX bytes/{print $4}'");
	fp=popen(str_tmp_cmd,"r");
	if(fp)
	{
		memset(szVal, 0x00, sizeof(szVal));
		while(fgets(szVal,sizeof(szVal),fp)) 
		{
			//CGetChar(&sum, szVal);/*update by wangjr 2010-12-03*/
			sscanf(szVal,"%llu",&sum);
		}
		sprintf(szVal,"%llu",sum);
		pclose(fp);
	}
	return szVal;

}

char * GetLANTotalPacketsSent(char *str_arg)
{
   
	FILE *fp;
	//int sum = 0;/*update by wangjr 2010-12-03*/
	unsigned long long sum =0;
	char str_tmp_cmd[SPRINT_MAX_LEN];
	memset( str_tmp_cmd, 0, SPRINT_MAX_LEN );
	memset(szVal, 0x00, sizeof(szVal));
	sprintf( str_tmp_cmd, "%s", "/sbin/ifconfig |awk 'BEGIN{RS=\"\\n\\n\"}/ath/{print $0}'|awk -F \"[ ]+|:\" '/TX packets/{print $4}'");
	fp=popen(str_tmp_cmd,"r");
	if(fp)
	{
		memset(szVal, 0x00, sizeof(szVal));
		while(fgets(szVal,sizeof(szVal),fp)) 
		{
			//CGetChar(&sum, szVal);/*update by wangjr 2010-12-03*/
			sscanf(szVal,"%llu",&sum);		
		}
		//sprintf(szVal,"%d",sum);
		sprintf(szVal,"%llu",sum);/*update by wangjr 2010-12-03*/
		pclose(fp);
	}
	return szVal;

}


char * GetLANTotalPacketsReceived(char *str_arg)
{
	FILE *fp;
	//int sum = 0;/*update by wangjr 2010-12-03*/
	unsigned long long sum =0;
	char str_tmp_cmd[SPRINT_MAX_LEN];
	memset( str_tmp_cmd, 0, SPRINT_MAX_LEN );
	memset(szVal, 0x00, sizeof(szVal));
	sprintf( str_tmp_cmd, "%s", "/sbin/ifconfig |awk 'BEGIN{RS=\"\\n\\n\"}/ath/{print $0}'|awk -F \"[ ]+|:\" '/RX packets/{print $4}'");
	fp=popen(str_tmp_cmd,"r");
	if(fp)
	{
		memset(szVal, 0x00, sizeof(szVal));
		while(fgets(szVal,sizeof(szVal),fp)) 
		{
			//CGetChar(&sum, szVal);/*update by wangjr 2010-12-03*/
			sscanf(szVal,"%llu",&sum);	
		}
		sprintf(szVal,"%llu",sum);/*update by wangjr 2010-12-03*/
		pclose(fp);
	}
	return szVal;

}

char * GetWANTotalPacketsSent(char *str_arg)
{

	FILE *fp;
	//int sum = 0;/*update by wangjr 2010-12-03*/
	unsigned long long sum = 0;	
	char str_tmp_cmd[SPRINT_MAX_LEN];
	memset( str_tmp_cmd, 0, SPRINT_MAX_LEN );
	memset(szVal, 0x00, sizeof(szVal));
	sprintf( str_tmp_cmd, "%s", "/sbin/ifconfig |awk 'BEGIN{RS=\"\\n\\n\"}/eth/{print	$0}'|awk -F  \"[ ]+|:\" '/TX packets/{print $4}'");
	fp=popen(str_tmp_cmd,"r");
	if(fp)
	{
		memset(szVal, 0x00, sizeof(szVal));
		while(fgets(szVal,sizeof(szVal),fp)) 
		{
			//CGetChar(&sum, szVal);/*update by wangjr 2010-12-03*/
			sscanf(szVal,"%llu",&sum);	
		}
		sprintf(szVal,"%llu",sum);/*update by wangjr 2010-12-03*/
		pclose(fp);
	}
	return szVal;

}

char * GetWANTotalPacketsReceived(char *str_arg)
{
	FILE *fp;
	//int sum = 0;/*update by wangjr 2010-12-03*/
	unsigned long long sum = 0;		
	char str_tmp_cmd[SPRINT_MAX_LEN];
	memset( str_tmp_cmd, 0, SPRINT_MAX_LEN );
	memset(szVal, 0x00, sizeof(szVal));
	sprintf( str_tmp_cmd, "%s", "/sbin/ifconfig |awk 'BEGIN{RS=\"\\n\\n\"}/eth/{print $0}'|awk -F \"[ ]+|:\" '/RX packets/{print $4}'");
	fp=popen(str_tmp_cmd,"r");
	if(fp)
	{
		memset(szVal, 0x00, sizeof(szVal));
		while(fgets(szVal,sizeof(szVal),fp)) 
		{
			//CGetChar(&sum, szVal);/*update by wangjr 2010-12-03*/
			sscanf(szVal,"%llu",&sum);	
		}
		sprintf(szVal,"%llu",sum);
		pclose(fp);
	}	
	return szVal;

}

char * GetTotalAssociations(char *str_arg)
{
	FILE *fp;
	memset(szVal, 0x00, sizeof(szVal));
	fp=popen("cat /proc/sys/net/ath0/users_join_counter","r");
	if(fp)
	{
		fgets(szVal,sizeof(szVal),fp);
		szVal[strlen(szVal)-1] = '\0';
		 pclose(fp);
	 }	
	return szVal;

}
////////////////////////update by wangjr 091124///////////////////////////////////////
char * GetAssociatedDeviceMACAddress(char *str_arg)
{

	FILE *fp;
	char str_tmp_cmd[SPRINT_MAX_LEN];
	int vapnum=Get_vapnum(str_arg);
	memset( str_tmp_cmd, 0, SPRINT_MAX_LEN );
	memset(szVal, 0x00, sizeof(szVal));

	
	sprintf( str_tmp_cmd,  "/usr/sbin/wlanconfig ath%d list |/usr/bin/awk '{print $1}'|/usr/bin/head -2 |sed '1d'",vapnum-1);
	
	fp=popen(str_tmp_cmd,"r");
	if(fp)
	{
		fgets(szVal,sizeof(szVal),fp);
		szVal[strlen(szVal)-1] = '\0';
		 pclose(fp);
	 }	
	return szVal;


}
//////////////////update by wangjr 091124////////////////////////////////////////
char * GetAssociatedDeviceIPAddress(char *str_arg)
{

	FILE *fp;
	char str_cmd[SPRINT_MAX_LEN]={0};
	char str_tmp_cmd[SPRINT_MAX_LEN]={0};
	int vapnum=Get_vapnum(str_arg);
	memset( str_tmp_cmd, 0, SPRINT_MAX_LEN );
	memset(szVal, 0x00, sizeof(szVal));
	sprintf( str_tmp_cmd,  "/usr/sbin/wlanconfig ath%d list |/usr/bin/awk '{print $1}'|/usr/bin/head -2 |sed '1d'",vapnum-1);
	fp=popen(str_tmp_cmd,"r");
	if(fp)
	{
		fgets(szVal,sizeof(szVal),fp);
		szVal[strlen(szVal)-1] = '\0';
	    pclose(fp);
	}

//#if 0
	sprintf(str_cmd,"/bin/grep '%s' /tmp/dhcpsnooping.log |/usr/bin/awk '{print $4}' | /usr/bin/cut -d: -f2", szVal);
	memset(szVal, 0x00, sizeof(szVal));
	fp=popen(str_cmd,"r");
	if(fp)
	{
		fgets(szVal,sizeof(szVal),fp);
		szVal[strlen(szVal)-1] = '\0';
		pclose(fp);
	}
//#endif
   // printf("1111111111111111111111111111szVal=%s\n",szVal);
	return szVal;
}	
////////////////////////update by wangjr 091124//////////////////////////////////////
char * GetAssociatedDeviceAuthenticationState(char *str_arg)
{

	FILE *fp;
	char str_tmp_cmd[SPRINT_MAX_LEN];
	int vapnum=Get_vapnum(str_arg);
	memset( str_tmp_cmd, 0, SPRINT_MAX_LEN );
	memset(szVal, 0x00, sizeof(szVal));

	sprintf( str_tmp_cmd,  "/usr/sbin/wlanconfig ath%d list |/usr/bin/awk '{print $14}'|/usr/bin/head -2 |sed '1d'",vapnum-1);
	fp=popen(str_tmp_cmd,"r");
	
	//fp=popen("/usr/sbin/wlanconfig ath0 list |/usr/bin/awk '{print $14}'|/usr/bin/head -2 |sed '1d'","r");
	if(fp)
	{
		fgets(szVal,sizeof(szVal),fp);
		szVal[strlen(szVal)-1] = '\0';
		pclose(fp);
	 }	
	if(atoi(szVal)%2 == 0)
	{
		memset(szVal, 0x00, sizeof(szVal));   
		strcpy(szVal,"false");
	}
	else
	{
		memset(szVal, 0x00, sizeof(szVal));   
		strcpy(szVal,"true");
	}
	
	return szVal;

}
char * GetCTReceiveRate(char *str_arg)
{
	FILE *fp;
	//int sum=0;/*update by wangjr 2010-12-03*/
	unsigned long long sum = 0;
	memset(szVal, 0x00, sizeof(szVal));
	fp=popen("/bin/cat /tmp/user-rate |/usr/bin/awk '{print $3}'|/usr/bin/head -2|sed '1d'","r");
	if(fp)
	{
		fgets(szVal,sizeof(szVal),fp);
		szVal[strlen(szVal)-1] = '\0';
		 pclose(fp);
	 }	
		/*CGetChar(&sum, szVal);
		sprintf(szVal,"%d",sum);*/
		sscanf(szVal,"%llu",&sum);
		sprintf(szVal,"%llu",sum);		
		return szVal;

}	
char * GetCTSendRate(char *str_arg)
{
	FILE *fp;
	//int sum=0;/*update by wangjr 2010-12-03*/
	unsigned long long sum = 0;
	memset(szVal, 0x00, sizeof(szVal));
	fp=popen("/bin/cat /tmp/user-rate |/usr/bin/awk '{print $4}'|/usr/bin/head -2|sed '1d'","r");
	if(fp)
	{
		fgets(szVal,sizeof(szVal),fp);
		szVal[strlen(szVal)-1] = '\0';
		 pclose(fp);
	 }	
		/*CGetChar(&sum, szVal);
		sprintf(szVal,"%d",sum);*/
		sscanf(szVal,"%llu",&sum);
		sprintf(szVal,"%llu",sum);
		return szVal;

}
char * GetTotalBytes(char *str_arg)
{
	//int sum=0;/*update by wangjr 2010-12-03*/
	unsigned long long sum = 0;
	unsigned long long rx_sum = 0;
 	unsigned long long tx_sum = 0;	
   	FILE *fp;
   	memset(szVal, 0x00, sizeof(szVal));
	fp=popen("/bin/cat /tmp/user-rate |/usr/bin/awk '{print $3}'|/usr/bin/head -2|sed '1d'","r");
	if(fp)
	{
		fgets(szVal,sizeof(szVal),fp);
		szVal[strlen(szVal)-1] = '\0';
		//sum=atoi(szVal);/*update by wangjr 2010-12-03*/
		sscanf(szVal,"%llu",&rx_sum);
		pclose(fp);
	}
	fp=popen("/bin/cat /tmp/user-rate |/usr/bin/awk '{print $4}'|/usr/bin/head -2|sed '1d'","r");
	if(fp)
	{
	    memset(szVal, 0x00, sizeof(szVal));
		fgets(szVal,sizeof(szVal),fp);
		szVal[strlen(szVal)-1] = '\0';
		//sum+=atoi(szVal);/*update by wangjr 2010-12-03*/
		sscanf(szVal,"%llu",&rx_sum);		
		pclose(fp);
	 }	
	sum = rx_sum + tx_sum;
	sprintf(szVal,"%llu",sum);	
	return szVal;
}
////////////////////update by wangjr 091124/////////////////////////////////
char * GetCTReceivePower(char *str_arg)
{
	FILE *fp;
	char str_tmp_cmd[SPRINT_MAX_LEN];
	int vapnum=Get_vapnum(str_arg);
	memset( str_tmp_cmd, 0, SPRINT_MAX_LEN );
	memset(szVal, 0x00, sizeof(szVal));
	sprintf( str_tmp_cmd,  "/usr/sbin/wlanconfig ath%d list |/usr/bin/awk '{print $5}'|/usr/bin/head -2 |sed '1d'",vapnum-1);
	fp=popen(str_tmp_cmd,"r");
	//fp=popen("/usr/sbin/wlanconfig ath0 list |/usr/bin/awk '{print $5}'|/usr/bin/head -2 |sed '1d'","r");
	if(fp)
	{
		fgets(szVal,sizeof(szVal),fp);
		szVal[strlen(szVal)-1] = '\0';
		 pclose(fp);
	 }	
	return szVal;


}
////////////////update by wangjr 091124////////////////////////////////////////
char * GetCTReceiveNoise(char *str_arg)
{
	FILE *fp;
	char str_tmp_cmd[SPRINT_MAX_LEN];
	int vapnum=Get_vapnum(str_arg);
	memset( str_tmp_cmd, 0, SPRINT_MAX_LEN );
	memset(szVal, 0x00, sizeof(szVal));
	sprintf( str_tmp_cmd, "/usr/sbin/iwconfig ath%d |grep -o -i -e 'Noise level=-[0-9]\\{1,2\\}'|/usr/bin/cut -d= -f2",vapnum-1);
	fp=popen(str_tmp_cmd,"r");
	if(fp)
	{
		fgets(szVal,sizeof(szVal),fp);
		szVal[strlen(szVal)-1] = '\0';
		pclose(fp);
	}
    return szVal;

}
///////////////////update by wangjr 091124/////////////////////////////////////////
char * GetCTPacketsError(char *str_arg)
{
	FILE *fp;
	int sum = 0;
	char crc_err[SPRINT_MAX_LEN];
	char key_err[SPRINT_MAX_LEN];
	char phy_err[SPRINT_MAX_LEN];
	memset( crc_err, 0, SPRINT_MAX_LEN );
	memset( key_err, 0, SPRINT_MAX_LEN );
	memset( phy_err, 0, SPRINT_MAX_LEN );
	sprintf( crc_err, "%s", "/usr/sbin/athstats |awk '/cuz of bad CRC/{print $1}'");
	sprintf( key_err, "%s", "/usr/sbin/athstats |awk '/cuz decryption/{print $1}'");
	sprintf( phy_err, "%s", "/usr/sbin/athstats |awk '/PHY errors/{print $1}'");	
	fp=popen(crc_err,"r");
	if(fp)
	{
		memset(szVal, 0x00, sizeof(szVal));
		fgets(szVal,sizeof(szVal),fp); 
		sum += atoi(szVal);
		pclose(fp);
	}
	fp=popen(key_err,"r");
	if(fp)
	{
		memset (szVal, 0, SPRINT_MAX_LEN);
		fgets(szVal,sizeof(szVal),fp); 
		sum += atoi(szVal);
		pclose(fp);
	}
	fp=popen(phy_err,"r");
	if(fp)
	{
		memset (szVal, 0, SPRINT_MAX_LEN);
		fgets(szVal,sizeof(szVal),fp); 
		sum += atoi(szVal);
		pclose(fp);
	}
	sprintf(szVal,"%d",sum);
	return szVal;

}
////////////update by wangjr 091124/////////////////////////
char * GetCRCError(char *str_arg)
{		
	FILE *fp;
	char str_tmp_cmd[SPRINT_MAX_LEN];
	memset( str_tmp_cmd, 0, SPRINT_MAX_LEN );
	sprintf( str_tmp_cmd, "%s", "/usr/sbin/athstats |awk '/cuz of bad CRC/{print $1}'");
	memset(szVal, 0x00, sizeof(szVal));
	fp=popen(str_tmp_cmd,"r");
    if(fp)
    {
		fgets(szVal,sizeof(szVal),fp); 
		szVal[strlen(szVal)-1] = '\0';
		pclose(fp);
    }
	return szVal;

}
///////////////update by wangjr 091124//////////////////////////////////
char * GetPHYError(char *str_arg)
{
  
	FILE *fp;
	char str_tmp_cmd[SPRINT_MAX_LEN];
	memset( str_tmp_cmd, 0, SPRINT_MAX_LEN );
	sprintf( str_tmp_cmd, "%s", "/usr/sbin/athstats|awk '/PHY errors/{print $1}'");
	memset(szVal, 0x00, sizeof(szVal));
	fp=popen(str_tmp_cmd,"r");
    if(fp)
    {
		fgets(szVal,sizeof(szVal),fp);
		szVal[strlen(szVal)-1] = '\0';
		pclose(fp);
    }
	return szVal;

}
char * GetMICError(char *str_arg)
{
    memset(szVal, 0x00, sizeof(szVal));
	strcpy(szVal,"micerror");
    return szVal;

}
/////////////////////update by wangjr 091124//////////////////////////////////
char * GetKEYError(char *str_arg)
{
	FILE *fp;
	char str_tmp_cmd[SPRINT_MAX_LEN];
	memset( str_tmp_cmd, 0, SPRINT_MAX_LEN );
	sprintf( str_tmp_cmd, "%s", "/usr/sbin/athstats |awk '/cuz decryption/{print $1}'");
	memset(szVal, 0x00, sizeof(szVal));
	fp=popen(str_tmp_cmd,"r");
    if(fp)
    {
		fgets(szVal,sizeof(szVal),fp);
		szVal[strlen(szVal)-1] = '\0';
		pclose(fp);
    }
    return szVal;
}
////////////////////update by wangjr 091124//////////////////////////////
char * GetResource_using(char *str_arg)
{  
	FILE *fp;
	int sum = 0;
	int r=0,t=0;
	char str_cmd1[SPRINT_MAX_LEN];
	char str_cmd2[SPRINT_MAX_LEN];
	int vapnum=Get_vapnum(str_arg);
	memset( str_cmd1, 0, SPRINT_MAX_LEN );
	memset( str_cmd1, 0, SPRINT_MAX_LEN );
	memset(szVal, 0x00, sizeof(szVal));
	
	sprintf( str_cmd1, "cat /tmp/interface-rate |awk '/ath%d/{print $2}'",vapnum-1);
	sprintf( str_cmd2, "cat /tmp/interface-rate |awk '/ath%d/{print $3}'",vapnum-1); 
    fp=popen(str_cmd1,"r");
    if(fp)
    {
		while(fgets(szVal,sizeof(szVal),fp)) 
		{
			CGetChar(&sum, szVal);
		}
		  r=sum;
		  pclose(fp);
	}
	fp=popen(str_cmd2,"r");
	if(fp)
    {
	   memset(szVal, 0x00, sizeof(szVal));
	   while(fgets(szVal,sizeof(szVal),fp)) 
	   {
			CGetChar(&sum, szVal);
	   }
	   t=sum;
	   pclose(fp);
	}
	memset(szVal, 0x00, sizeof(szVal));
	sprintf(szVal,"%f",GetPercentage(((t+r)*8),23*1024*1024)*100);
    return szVal;

}

char * GetAuthenticationUser(char *str_arg)
{
   #if 0  //update for NG 
	FILE *fp;
	int sum=0;
	memset(szVal, 0x00, sizeof(szVal));
	fp=popen("cat /proc/sys/net/ath0/users_join_counter","r");
	if(fp)
	{
		fgets(szVal,sizeof(szVal),fp);
		szVal[strlen(szVal)-1] = '\0';
		 pclose(fp);
	 }	
	CGetChar(&sum, szVal);
	sprintf(szVal,"%d",sum);
	return szVal;
	#endif

		FILE *fp;
	int sum=0;
	char str_tmp_cmd[SPRINT_MAX_LEN];
	int vapnum=Get_vapnum(str_arg);
	memset( str_tmp_cmd, 0, SPRINT_MAX_LEN );
	memset(szVal, 0x00, sizeof(szVal));

	
	sprintf( str_tmp_cmd,  "/usr/sbin/wlanconfig ath%d list | wc -l",vapnum-1);
	
	fp=popen(str_tmp_cmd,"r");
	if(fp)
	{
		fgets(szVal,sizeof(szVal),fp);
		szVal[strlen(szVal)-1] = '\0';
		 pclose(fp);
	 }	
	CGetChar(&sum, szVal);
        if (0<sum)
            sum=sum-1;
	sprintf(szVal,"%d",sum); 
	return szVal;
	

}

char * GetCTLANTotalBytesSent(char *str_arg)
{
	FILE *fp;
	//int sum = 0;/*update by wangjr 2010-12-03*/
	unsigned long long sum = 0;
	char str_tmp_cmd[SPRINT_MAX_LEN];
	memset( str_tmp_cmd, 0, SPRINT_MAX_LEN );
	memset(szVal, 0x00, sizeof(szVal));
	sprintf( str_tmp_cmd, "%s", "/sbin/ifconfig |awk 'BEGIN{RS=\"\\n\\n\"}/eth/{print $0}'|awk -F \"[ ]+|:\" '/TX bytes/{print $9}'");
	fp=popen(str_tmp_cmd,"r");
	if(fp)
	{
		memset(szVal, 0x00, sizeof(szVal));
		while(fgets(szVal,sizeof(szVal),fp)) 
		{
			//CGetChar(&sum, szVal);/*update by wangjr 2010-12-03*/
			sscanf(szVal,"%llu",&sum);
		}
		sprintf(szVal,"%llu",sum);/*update by wangjr 2010-12-03*/	  
		pclose(fp);
	}
    return szVal;

}

char * GetCTLANTotalBytesReceived(char *str_arg)
{
	FILE *fp;
	//int sum = 0;/*update by wangjr 2010-12-03*/
	unsigned long long sum = 0;
	char str_tmp_cmd[SPRINT_MAX_LEN];
	memset( str_tmp_cmd, 0, SPRINT_MAX_LEN );
	memset(szVal, 0x00, sizeof(szVal));
	sprintf( str_tmp_cmd, "%s", "/sbin/ifconfig |awk 'BEGIN{RS=\"\\n\\n\"}/eth/{print $0}'|awk -F \"[ ]+|:\" '/RX bytes/{print $4}'");
	fp=popen(str_tmp_cmd,"r");
	if(fp)
	{
		memset(szVal, 0x00, sizeof(szVal));
		while(fgets(szVal,sizeof(szVal),fp)) 
		{
			//CGetChar(&sum, szVal);/*update by wangjr 2010-12-03*/
			sscanf(szVal,"%llu",&sum);

		}
		sprintf(szVal,"%llu",sum);
		pclose(fp);
	}
	return szVal;


}

char * GetCTWANTotalBytesSent(char *str_arg)
{
	FILE *fp;
	//int sum = 0;/*update by wangjr 2010-12-03*/
	unsigned long long sum = 0;
	char str_tmp_cmd[SPRINT_MAX_LEN];
	memset( str_tmp_cmd, 0, SPRINT_MAX_LEN );
	memset(szVal, 0x00, sizeof(szVal));
	sprintf( str_tmp_cmd, "%s", "/sbin/ifconfig |awk 'BEGIN{RS=\"\\n\\n\"}/eth/{print $0}'|awk -F \"[ ]+|:\" '/TX bytes/{print $9}'");
	fp=popen(str_tmp_cmd,"r");
	if(fp)
	{
		memset(szVal, 0x00, sizeof(szVal));
		while(fgets(szVal,sizeof(szVal),fp)) 
		{
			//CGetChar(&sum, szVal);/*update by wangjr 2010-12-03*/
			sscanf(szVal,"%llu",&sum);
		}
		sprintf(szVal,"%llu",sum);/*update by wangjr 2010-12-03*/	  
		pclose(fp);
	}
	
	return szVal;


}

char * GetCTWANTotalBytesReceived(char *str_arg)
{

	FILE *fp;
	//int sum = 0;/*update by wangjr 2010-12-03*/
	unsigned long long sum = 0;
	char str_tmp_cmd[SPRINT_MAX_LEN];
	memset( str_tmp_cmd, 0, SPRINT_MAX_LEN );
	memset(szVal, 0x00, sizeof(szVal));
	sprintf( str_tmp_cmd, "%s", "/sbin/ifconfig |awk 'BEGIN{RS=\"\\n\\n\"}/eth/{print $0}'|awk -F \"[ ]+|:\" '/RX bytes/{print $4}'");
	fp=popen(str_tmp_cmd,"r");
	if(fp)
	{
		memset(szVal, 0x00, sizeof(szVal));
		while(fgets(szVal,sizeof(szVal),fp)) 
		{
			//CGetChar(&sum, szVal);/*update by wangjr 2010-12-03*/
			sscanf(szVal,"%llu",&sum);

		}
		sprintf(szVal,"%llu",sum);
		pclose(fp);
	}
	return szVal;	

}

char * GetTotalBytesSentoper(char *str_arg)
{
	FILE *fp;
	int sum = 0;
	char str_tmp_cmd[SPRINT_MAX_LEN];
	memset( str_tmp_cmd, 0, SPRINT_MAX_LEN );
	memset(szVal, 0x00, sizeof(szVal));
	sprintf( str_tmp_cmd, "%s", "/sbin/ifconfig |awk 'BEGIN{RS=\"\\n\\n\"}/eth/{print $0}'|awk -F \"[ ]+|:\" '/TX bytes/{print $9}'");
	fp=popen(str_tmp_cmd,"r");
	if(fp)
	{
		memset(szVal, 0x00, sizeof(szVal));
		while(fgets(szVal,sizeof(szVal),fp)) 
		{
			CGetChar(&sum, szVal);
		}
		sprintf(szVal,"%d",sum);	  
		pclose(fp);
	}
	
	return szVal;


}
//////////////////update by wangjr 091124//////////////////////////////
char * GetLANPacketsError(char *str_arg)
{	
	FILE *fp;
	//int sum = 0;
	char right_rx[SPRINT_MAX_LEN];	
	char right_tx[SPRINT_MAX_LEN];
	char error_rx[SPRINT_MAX_LEN];
	char error_tx[SPRINT_MAX_LEN];
	char str_tmp_cmd[SPRINT_MAX_LEN];
	/* int iright=0;
	int ierror=0;*//*update by wangjr 2010-12-03*/
	unsigned long long sum = 0;
	unsigned long long iright = 0;
	unsigned long long ierror = 0;
	
	unsigned long long iright_rx = 0;
	unsigned long long iright_tx = 0;
	unsigned long long ierror_rx = 0;
	unsigned long long ierror_tx = 0;

	
	int vapnum=Get_vapnum(str_arg);
	memset( str_tmp_cmd, 0, SPRINT_MAX_LEN );
	memset( right_rx, 0, SPRINT_MAX_LEN );
	memset( right_tx, 0, SPRINT_MAX_LEN );
	memset( error_rx, 0, SPRINT_MAX_LEN );
	memset( error_tx, 0, SPRINT_MAX_LEN );
	memset(szVal, 0x00, sizeof(szVal));
	
	sprintf( str_tmp_cmd,  "/sbin/ifconfig |awk 'BEGIN{RS=\"\\n\\n\"}/ath%d/{print $0}'|awk -F \"[ ]+|:\" '/RX packets/{print $4}'",vapnum-1);
	fp=popen(str_tmp_cmd,"r");
	if(fp)
	{
		fgets(szVal,sizeof(szVal),fp);
		szVal[strlen(szVal)-1] = '\0';
		pclose(fp);
		strncpy(right_rx,szVal,strlen(szVal));
		memset(szVal, 0x00, sizeof(szVal));
	}	

	
	memset( str_tmp_cmd, 0, SPRINT_MAX_LEN );
	sprintf( str_tmp_cmd, "/sbin/ifconfig |awk 'BEGIN{RS=\"\\n\\n\"}/ath%d/{print $0}'|awk -F \"[ ]+|:\" '/TX packets/{print $4}'",vapnum-1);
	fp=popen(str_tmp_cmd,"r");
	if(fp)
	{
		fgets(szVal,sizeof(szVal),fp);
		szVal[strlen(szVal)-1] = '\0';
		pclose(fp);
		strncpy(right_tx,szVal,strlen(szVal));
		memset(szVal, 0x00, sizeof(szVal));
	}


	memset( str_tmp_cmd, 0, SPRINT_MAX_LEN );
	sprintf( str_tmp_cmd, "/sbin/ifconfig |awk 'BEGIN{RS=\"\\n\\n\"}/ath%d/{print $0}'|awk -F \"[ ]+|:\" '/errors/{print $6}'|sed -n '1p'",vapnum-1);
	fp=popen(str_tmp_cmd,"r");
	if(fp)
	{
		fgets(szVal,sizeof(szVal),fp);
		szVal[strlen(szVal)-1] = '\0';
		pclose(fp);
		strncpy(error_rx,szVal,strlen(szVal));
		memset(szVal, 0x00, sizeof(szVal));
	}
	memset( str_tmp_cmd, 0, SPRINT_MAX_LEN );
	sprintf( str_tmp_cmd,  "/sbin/ifconfig |awk 'BEGIN{RS=\"\\n\\n\"}/ath%d/{print $0}'|awk -F \"[ ]+|:\" '/RX packets/{print $6}'|sed -n '2p'",vapnum-1);
	fp=popen(str_tmp_cmd,"r");
	if(fp)
	{
		fgets(szVal,sizeof(szVal),fp);
		szVal[strlen(szVal)-1] = '\0';
		pclose(fp);
		strncpy(error_tx,szVal,strlen(szVal));
		memset(szVal, 0x00, sizeof(szVal));
	}

	/*iright+=atoi(right_rx);
	iright+=atoi(right_tx);
	ierror+=atoi(error_rx);
	ierror+=atoi(error_tx);*//*update by wangjr 2010-12-03*/

	sscanf(right_rx,"llu",&iright_rx);
	sscanf(right_tx,"llu",&iright_tx);
	iright = iright_rx + iright_tx;	
	sscanf(error_rx,"llu",&ierror_rx);
	sscanf(error_tx,"llu",&ierror_tx);
	ierror= ierror_rx + ierror_tx;
	if(iright == 0)
	{ 
	  sprintf(szVal,"%d %",0);
	  
	}
	else
	{
	  sprintf(szVal,"%d %",ierror*100/iright);
	}

	
	return szVal;

}
////////////////update by wangjr 091124////////////////////////
char * GetWANPacketsError(char *str_arg)
{
	FILE *fp;
	//int sum = 0;/*update by wangjr 2010-12-03*/

	char right_rx[SPRINT_MAX_LEN];	
	char right_tx[SPRINT_MAX_LEN];
	char error_rx[SPRINT_MAX_LEN];
	char error_tx[SPRINT_MAX_LEN];
	char str_tmp_cmd[SPRINT_MAX_LEN];
	/*int iright=0;
	int ierror=0;*/
	
	unsigned long long iright = 0;
	unsigned long long ierror = 0;
	
	unsigned long long iright_rx = 0;
	unsigned long long iright_tx = 0;
	unsigned long long ierror_rx = 0;
	unsigned long long ierror_tx = 0;
	
	int vapnum=Get_vapnum(str_arg);
	memset( str_tmp_cmd, 0, SPRINT_MAX_LEN );
	memset( right_rx, 0, SPRINT_MAX_LEN );
	memset( right_tx, 0, SPRINT_MAX_LEN );
	memset( error_rx, 0, SPRINT_MAX_LEN );
	memset( error_tx, 0, SPRINT_MAX_LEN );
	memset(szVal, 0x00, sizeof(szVal));
	
	sprintf( str_tmp_cmd, "%s", "/sbin/ifconfig |awk 'BEGIN{RS=\"\\n\\n\"}/eth0/{print $0}'|awk -F \"[ ]+|:\" '/RX packets/{print $4}'");
	
	fp=popen(str_tmp_cmd,"r");
	if(fp)
	{
		fgets(szVal,sizeof(szVal),fp);
		szVal[strlen(szVal)-1] = '\0';
		pclose(fp);
		strncpy(right_rx,szVal,strlen(szVal));
		memset(szVal, 0x00, sizeof(szVal));
	}	

	
	memset( str_tmp_cmd, 0, SPRINT_MAX_LEN );
	sprintf( str_tmp_cmd,"%s", "/sbin/ifconfig |awk 'BEGIN{RS=\"\\n\\n\"}/eth0/{print $0}'|awk -F \"[ ]+|:\" '/RX packets/{print $4}'");
	fp=popen(str_tmp_cmd,"r");
	if(fp)
	{
		fgets(szVal,sizeof(szVal),fp);
		szVal[strlen(szVal)-1] = '\0';
		pclose(fp);
		strncpy(right_tx,szVal,strlen(szVal));
		memset(szVal, 0x00, sizeof(szVal));
	}


	memset( str_tmp_cmd, 0, SPRINT_MAX_LEN );
	sprintf( str_tmp_cmd,"%s", "/sbin/ifconfig |awk 'BEGIN{RS=\"\\n\\n\"}/eth0/{print $0}'|awk -F \"[ ]+|:\" '/errors/{print $6}'|sed -n '2p'");
	fp=popen(str_tmp_cmd,"r");
	if(fp)
	{
		fgets(szVal,sizeof(szVal),fp);
		szVal[strlen(szVal)-1] = '\0';
		pclose(fp);
		strncpy(error_rx,szVal,strlen(szVal));
		memset(szVal, 0x00, sizeof(szVal));
	}
	memset( str_tmp_cmd, 0, SPRINT_MAX_LEN );
	sprintf( str_tmp_cmd,"%s", "/sbin/ifconfig |awk 'BEGIN{RS=\"\\n\\n\"}/eth0/{print $0}'|awk -F \"[ ]+|:\" '/errors/{print $6}'|sed -n '1p'");
	fp=popen(str_tmp_cmd,"r");
	if(fp)
	{
		fgets(szVal,sizeof(szVal),fp);
		szVal[strlen(szVal)-1] = '\0';
		pclose(fp);
		strncpy(error_tx,szVal,strlen(szVal));
		memset(szVal, 0x00, sizeof(szVal));
	}

	/*iright+=atoi(right_rx);
	iright+=atoi(right_tx);
	ierror+=atoi(error_rx);
	ierror+=atoi(error_tx);*/
	sscanf(right_rx,"llu",&iright_rx);
	sscanf(right_tx,"llu",&iright_tx);
	iright = iright_rx + iright_tx;	
	sscanf(error_rx,"llu",&ierror_rx);
	sscanf(error_tx,"llu",&ierror_tx);
	ierror= ierror_rx + ierror_tx;
	
	if(iright == 0)
	{ 
	  sprintf(szVal,"%d %",0);
	  
	}
	else
	{
	  sprintf(szVal,"%d %",ierror*100/iright);
	}

	
	return szVal;



}
//////////////////update by wangjr 091124////////////////////////////////////
char * GetBreakTime(char *str_arg)
{
	FILE *fp;
	memset(szVal, 0x00, sizeof(szVal));
	fp=popen("/usr/sbin/80211stats |/bin/grep 'node_timeout:'|/usr/bin/cut -d: -f2","r");
	if(fp)
	{
		fgets(szVal,sizeof(szVal),fp);
		szVal[strlen(szVal)-1] = '\0';
		 pclose(fp);
	 }	
	return szVal;
}
//////////////////update by wangjr 091124////////////////////////////////////

char * GetAskPass(char *str_arg)
{

 FILE *fp;
 char str_tmp_cmd[SPRINT_MAX_LEN];
 int vapnum=Get_vapnum(str_arg);
 memset( str_tmp_cmd, 0, SPRINT_MAX_LEN );
 
 memset(szVal, 0x00, sizeof(szVal));

 sprintf( str_tmp_cmd, "cat /proc/sys/net/ath%d/users_join_counter",vapnum-1);

 fp=popen(str_tmp_cmd,"r"); 
 if(fp)
 {
	 fgets(szVal,sizeof(szVal),fp);
		szVal[strlen(szVal)-1] = '\0';
		pclose(fp);
	 }  
    return szVal;

}
//////////////////update by wangjr 091124////////////////////////////////////

char * GetResponsePass(char *str_arg)
{

	FILE *fp;
	char str_tmp_cmd[SPRINT_MAX_LEN];
	int vapnum=Get_vapnum(str_arg);
 	memset( str_tmp_cmd, 0, SPRINT_MAX_LEN );
	memset(szVal, 0x00, sizeof(szVal));
	sprintf( str_tmp_cmd, "cat /proc/sys/net/ath%d/respond_users_join_cnt",vapnum-1);
	//fp=popen("cat /proc/sys/net/ath0/respond_users_join_cnt","r");
	
	fp=popen(str_tmp_cmd,"r"); 
	if(fp)
	{
		fgets(szVal,sizeof(szVal),fp);
		szVal[strlen(szVal)-1] = '\0';
		pclose(fp);
	 }
    return szVal;

}
//////////////////update by wangjr 091124////////////////////////////////////

char * GetSuccessPass(char *str_arg)
{
  
  	FILE *fp;
  	char str_tmp_cmd[SPRINT_MAX_LEN];
	int vapnum=Get_vapnum(str_arg);
 	memset( str_tmp_cmd, 0, SPRINT_MAX_LEN );
 	 memset(szVal, 0x00, sizeof(szVal));
	 
	sprintf( str_tmp_cmd, "cat /proc/sys/net/ath%d/users_join_successful_counter",vapnum-1);
	
 	 //fp=popen("cat /proc/sys/net/ath0/users_join_successful_counter","r"); 
 fp=popen(str_tmp_cmd,"r");	 
	 
  if(fp)
  {
	  fgets(szVal,sizeof(szVal),fp);
	  szVal[strlen(szVal)-1] = '\0';
	  pclose(fp);
   }
    return szVal;

}


char *GetWorkTime(char *str_arg)
{
	FILE *fp;
	memset(szVal, 0x00, sizeof(szVal));
	int time=0;
	fp=popen("/bin/cat /proc/uptime |/usr/bin/awk '{print $1}'","r"); 
	if(fp)
	{
		fgets(szVal,sizeof(szVal),fp);
		szVal[strlen(szVal)-1] = '\0';
		pclose(fp);
	}
	time=atoi(szVal);
	memset(szVal, 0x00, sizeof(szVal));
	sprintf(szVal,"%d",time*100);
	return szVal;
}
char *GetCPU(char *str_arg)
{
	memset(szVal, 0x00, sizeof(szVal));

	int cpu_use_ratio=0;
	
	unsigned int total;  
	float user;
	float nice;
	float system;
	float idle;
	
	char cpu[21];
	char text[201];
	
	FILE *fp;
	
	fp = fopen("/proc/stat", "r");
	while (fgets(text, 200, fp))
	{
	 if (strstr(text, "cpu"))
	 {
	  sscanf(text, "%s %f %f %f %f", cpu, &user, &nice, &system, &idle);
	 }
	}
	fclose(fp);
	
	total = (user + nice + system + idle);
	user = (user / total) * 100;
	nice = (nice / total) * 100;
	system = (system / total) * 100;
	idle = (idle / total) * 100;
	
	cpu_use_ratio = 10000-idle*100;
	sprintf(szVal,"%d \%",cpu_use_ratio/100);
	return szVal;

}
char *GetTemp(char *str_arg)
{
  	FILE   *stream; 
	memset(szVal, 0x00, sizeof(szVal));
	stream = popen( "/usr/bin/avrutil -T|grep Temperature |cut -d: -f2", "r" );
	if(stream)
	{
    	fread( szVal, sizeof(char), sizeof(szVal),  stream); 
		pclose( stream );
	}
	szVal[strlen(szVal)-1]='\0';
    return szVal;
}

char * GetTotalMem(char *str_arg)
{

	FILE *fp;
	memset(szVal, 0x00, sizeof(szVal));
	fp=popen("/usr/bin/free | /usr/bin/tail -1 |/usr/bin/awk '{print $2}'","r");
	if(fp)
	{
		fgets(szVal,sizeof(szVal),fp);
		szVal[strlen(szVal)-1] = '\0';
		pclose(fp);
	 }
	return szVal;

}
char * GetUseMem(char *str_arg)
{

	FILE *fp;
	memset(szVal, 0x00, sizeof(szVal));
	fp=popen("/usr/bin/free | /usr/bin/tail -1 |/usr/bin/awk '{print $3}'","r");
	if(fp)
	{
		fgets(szVal,sizeof(szVal),fp);
		szVal[strlen(szVal)-1] = '\0';
		pclose(fp);
	 }
	return szVal;

}

void CWGetFlashSizeInfo(unsigned short *flashTotal, int *flashFree) 
{  

int flashConfigFree;  char temp_str[100];  
char str[20];  
FILE *fp;
 fp = popen("/bin/df | /bin/grep 'jffs' |/usr/bin/awk '{print $4}'", "r");
 if(fp)
 {
  memset(str, 0, 20);
  fgets(str, sizeof(str), fp);
  str[strlen(str)-1] = '\0';
  flashConfigFree = atoi(str);
  pclose(fp);
 }

 *flashTotal = 32;
 *flashFree = ((*flashTotal-15)*1024-128-(1024-flashConfigFree))*8;

  //printf("flashTotal:%dMB flashFree:%dKb", *flashTotal, *flashFree);
 
 return;
}
char * GetFlash(char *str_arg)
{
   	memset(szVal, 0x00, sizeof(szVal));
	sprintf(szVal,"32 MB");
	return szVal;

}
char * GetFreeFlash(char *str_arg)
{
   	FILE *fp;
	int flashFree=0;
	int flashConfigFree=0;
	char str[20]={0};
	memset(szVal, 0x00, sizeof(szVal));
	fp = popen("/bin/df | /bin/grep 'jffs' |/usr/bin/awk '{print $4}'", "r");
	if(fp)
	{
	 memset(str, 0, 20);
	 fgets(str, sizeof(str), fp);
	 str[strlen(str)-1] = '\0';
	 flashConfigFree = atoi(str);
	 pclose(fp);
	}
	flashFree = ((32-15)*1024-128-(1024-flashConfigFree))*8;

	sprintf(szVal,"%d Kb",flashFree);
	return szVal;

}
char * GeWorkMode(char *str_arg)
{
	memset(szVal, 0x00, sizeof(szVal));
	FILE   *fp; 
	fp = popen( "/bin/cat /proc/sys/net/ipv4/ip_forward", "r" );
    if(fp)
	{
		fgets(szVal,sizeof(szVal),fp);
		szVal[strlen(szVal)-1] = '\0';
		pclose(fp);
	 }
	if(atoi(szVal) == 0)
	{
	    strcpy(szVal,"bridge");
	}
	else
	{
	    strcpy(szVal,"route");
	}
	return szVal;


}

char * GetMaxUser(char *str_arg)
{
	memset(szVal, 0x00, sizeof(szVal));
	wireless_get_maxsimultusers(Get_vapnum(str_arg),szVal,sizeof(szVal));
	return szVal; //don't return local variable
}
char * GetIsolation(char *str_arg)
{
	memset(szVal, 0x00, sizeof(szVal));
	wireless_get_isolation(Get_vapnum(str_arg),szVal,sizeof(szVal));
	return szVal; //don't return local variable
}

char * GetLogIP(char *str_arg)
{
	memset(szVal, 0x00, sizeof(szVal));
	syslog_get_ip(szVal,20);
	return szVal; //don't return local variable
}

char * GetVlanEnable(char *str_arg)
{
	memset(szVal, 0x00, sizeof(szVal));
	strcpy(szVal,"enable");
	return szVal; //don't return local variable


}
char * GetVlanVid(char *str_arg)
{
	memset(szVal, 0x00, sizeof(szVal));
	strcpy(szVal,"0");
	return szVal; 
}

//#endif

char *GetIPAddress(char *str_arg)
{
	FILE   *stream; 
	memset(szVal, 0x00, sizeof(szVal));
	stream = popen( "/sbin/ifconfig default |/bin/grep -o -i -e 'inet addr:[0-9]\\{1,3\\}\\.[0-9]\\{1,3\\}\\.[0-9]\\{1,3\\}\\.[0-9]\\{1,3\\}'| /usr/bin/cut -d: -f2", "r" );
    if(stream)
    {
		fread( szVal, sizeof(char), sizeof(szVal),	stream); 
		pclose( stream );
    }
	szVal[strlen(szVal)-1]='\0';
	return szVal;
}
char *GetSubnetMask(char *str_arg)
{
    FILE   *stream; 
    memset(szVal, 0x00, sizeof(szVal));
	stream = popen( "/sbin/ifconfig default |/bin/grep -o -i -e 'mask:[0-9]\\{1,3\\}\\.[0-9]\\{1,3\\}\\.[0-9]\\{1,3\\}\\.[0-9]\\{1,3\\}' | /usr/bin/cut -d: -f2", "r" );
	if(stream)
	{
		fread( szVal, sizeof(char), sizeof(szVal),  stream); 
		pclose( stream );
	}
    szVal[strlen(szVal)-1]='\0';
    return szVal;
}

char *GetDefaultGateway(char *str_arg)
{
    FILE   *stream; 
    memset(szVal, 0x00, sizeof(szVal));
	stream = popen( "/sbin/route |/usr/bin/awk '{print $2}'|/usr/bin/head -4 |/bin/sed '1,3d'", "r" );
	if(stream)
	{
    	fread( szVal, sizeof(char), sizeof(szVal),  stream); 
		pclose( stream );
	}
    szVal[strlen(szVal)-1]='\0';
    return szVal;
}


char *GetMACAddress(char *str_arg)
{
    FILE   *stream; 
    memset(szVal, 0x00, sizeof(szVal));
	stream = popen( "/sbin/ifconfig default | /bin/grep 'default' |/usr/bin/awk '{print $5}'", "r" );
	if(stream)
	{
    	fread( szVal, sizeof(char), sizeof(szVal),  stream); 
		pclose( stream );
	}
    szVal[strlen(szVal)-1]='\0';
    return szVal;


}

char *GetLANStatus(char *str_arg)
{
	FILE   *stream; 
	char str_tmp_cmd[100]={0};
	memset(szVal, 0x00, sizeof(szVal)); 
	strcpy(str_tmp_cmd,"{ /sbin/ifconfig default|/bin/grep  'UP' ; echo $? ; }");
	stream = popen( str_tmp_cmd, "r" );
	if(stream)
	{
		fread( szVal, sizeof(char), sizeof(szVal),	stream); 
		pclose( stream );
	}
	szVal[strlen(szVal)-1]='\0';
	if(0==atoi(szVal))
	{
		memset(szVal, 0x00, sizeof(szVal)); 
		strcpy(szVal,"Up");
	}
	else 
	{
		memset(szVal, 0x00, sizeof(szVal));
		strcpy(szVal,"Disable");
	}
	
    return szVal;
}

char *GetLANMaxBitRate(char *str_arg)
{

	FILE   *stream; 
	memset(szVal, 0x00, sizeof(szVal)); 
	stream = popen( "/usr/sbin/ethtool eth0 |grep  -i 'speed'|/usr/bin/cut -d: -f2|/usr/bin/cut -dM -f1", "r" );
	if(stream)
	{
		fread( szVal, sizeof(char), sizeof(szVal),stream); 
		pclose( stream );
	}
	szVal[strlen(szVal)-1]='\0';
	return szVal;

}

#if 0
char *GetIPValue()
{
  return "10.0.0.2";
}

char *GetDeviceSummary()
{
  return "Device:1.0(Baseline:1),STBService:1.0(Baseline:1)";
}
#endif

/*********************
******system*****
***********************/

char *GetHardwareVersion(char *str_arg)
{
	FILE   *stream; 
	memset(szVal, 0x00, sizeof(szVal));
	stream = popen( "/usr/sbin/showsysinfo |grep 'Hardware Version'|cut -d: -f2", "r" );
	if(stream)
	{
    	fread( szVal, sizeof(char), sizeof(szVal),  stream); 
		pclose( stream );
	}
	szVal[strlen(szVal)-1]='\0';
    return szVal;

}

char *GetSoftwareVersion(char *str_arg)
{
  	FILE   *stream; 
	memset(szVal, 0x00, sizeof(szVal));
	stream = popen( "/usr/sbin/showsysinfo |grep 'Software Version'|cut -d: -f2", "r" );
	if(stream)
	{
		fread( szVal, sizeof(char), sizeof(szVal),  stream); 
		pclose( stream );
	}
	szVal[strlen(szVal)-1]='\0';
    return szVal;
}
//////////////////update by wangjr 091124////////////////////////////////////

char *GetOUI(char *str_arg)
{
    FILE *fp;
    char str_conf[64]={0};
    char str_com[100]={0};
	
	memset(str_com, 0x00, sizeof(str_com));
	fp = popen( "/usr/sbin/showsysinfo |grep 'Company Name'|cut -d: -f2", "r" );
	if(fp)
	{
		fread( str_com, sizeof(char), sizeof(str_com),  fp); 
		pclose( fp );
	}
	str_com[strlen(str_com)-1]='\0';

	
   // return szVal;

    if(fp=fopen("/jffs/.OEM/.vendor_oui","r"))
    {
        fgets(str_conf, 64, fp);
        int length = strlen(str_conf);
        if(str_conf[length-1] == '\n')
            str_conf[length-1] = '\0';
        strcpy(szVal, str_conf);
        fclose(fp);
    }
    else
    {
        strcpy(szVal, DEFAULT_O_U_I);
    }

    return szVal;


}
//////////////////update by wangjr 091124////////////////////////////////////
char *GetProductClass(char *str_arg)
{
  	FILE   *stream; 
	memset(szVal, 0x00, sizeof(szVal));
	stream = popen( "/usr/sbin/showsysinfo |grep 'Device Type'|cut -d: -f2", "r" );
	if(stream)
	{
		fread( szVal, sizeof(char), sizeof(szVal),  stream); 
		pclose( stream );
	}
	szVal[strlen(szVal)-1]='\0';
    return szVal;

}

char *GetSerialNumber(char *str_arg)
{
    FILE   *stream; 
	memset(szVal, 0x00, sizeof(szVal));
	stream = popen( "/usr/sbin/showsysinfo |grep 'SN'|cut -d: -f2", "r" );
	if(stream)
	{
		fread( szVal, sizeof(char), sizeof(szVal),  stream); 
		pclose( stream );
	}
	szVal[strlen(szVal)-1]='\0';
    return szVal;
}
//////////////////update by wangjr 091124////////////////////////////////////

char *GetManufacturer(char *str_arg)
{
    FILE   *stream; 
	memset(szVal, 0x00, sizeof(szVal));
	stream = popen( "/usr/sbin/showsysinfo |grep 'Company Name'|cut -d: -f2", "r" );
	if(stream)
	{
    	fread( szVal, sizeof(char), sizeof(szVal),  stream); 
		pclose( stream );
	}
	szVal[strlen(szVal)-1]='\0';
    return szVal;
}

int SetDiagnosticsState(char *str_arg,char *str_szval)
{
	char str_cmd[BUFFERSIZE];
	//char str_tmp[BUFFERSIZE];
	FILE *fp;
	
	memset(str_cmd, 0x00, BUFFERSIZE);
	if(str_szval && !strcmp(str_szval, "Requested"))
	{
		sprintf(str_cmd, "ping -c %d -s %d %s | awk 'BEGIN{FS=\"[ ]+|/\";SUC=0;MIN=0;AVG=0;MAX=0;FLAG=0;}{if(FLAG){if($0 ~ \"packets\"){SUC=$4;}else{MIN=$6;AVG=$7;MAX=$8}}if($0 ~ \"ping statistics\"){FLAG=1}}END{print SUC\"\\n\"MIN\"\\n\"AVG\"\\n\"MAX}' && echo $? > /jffs/pingres",gPingNum, gPingSize, gPingHost);
		//system(str_cmd);
		fp=popen(str_cmd, "r");
		if(fp)
		{
			fgets(gPingSuc, 8, fp);
			gPingSuc[strlen(gPingSuc)-1]='\0';
			sprintf(gPingFal, "%d", gPingNum - atoi(gPingSuc));
			fgets(gPingMin, 8, fp);
			gPingMin[strlen(gPingMin)-1]='\0';
			fgets(gPingAvg, 8, fp);
			gPingAvg[strlen(gPingAvg)-1]='\0';
			fgets(gPingMax, 8, fp);
			gPingMax[strlen(gPingMax)-1]='\0';
			dprintf(LOG_LEVEL4, "%s\n", gPingSuc);
			dprintf(LOG_LEVEL4, "%s\n", gPingFal);
			dprintf(LOG_LEVEL4, "%s\n", gPingMin);
			dprintf(LOG_LEVEL4, "%s\n", gPingAvg);
			dprintf(LOG_LEVEL4, "%s\n", gPingMax);
			pclose(fp);
		}
		else
		{
			return 1;
		}
		sprintf(str_cmd, "echo 1 > /jffs/pingflag");
		system(str_cmd);
	}
	return 0;
}

int SetInterface(char *str_arg,char *str_szval)
{
	return 0;
}

int SetHost(char *str_arg,char *str_szval)
{
    strcpy(gPingHost,str_szval);
	   
   if(strlen(gPingHost) == 0)
   {
      strcpy(gPingHost,"192.168.100.100");
   }
	return 0;
}

int SetNumberOfRepetions(char *str_arg,char *str_szval)
{
   gPingNum=atoi(str_szval);
   if(gPingNum == 0)
   {
      gPingNum=4;
   }
   return 0;
}

int SetTimeout(char *str_arg,char *str_szval)
{
	return 0;
}

int SetDataBlockSize(char *str_arg,char *str_szval)
{
    gPingSize=atoi(str_szval);
   if(gPingSize == 0)
   {
      gPingSize=56;
   }
 	return 0;
}

int SetDSCP(char *str_arg,char *str_szval)
{
	return 0;
}

char *GetDiagnosticsState(char *str_arg)
{
	FILE *fp;
	char res[8];
	dprintf(4,"start diag\n",res);
	strcpy(res,"0");
	dprintf(4,"get diag: %s\n",res);
	if(fp=fopen("/jffs/pingres","r"))
	{
		dprintf(4,"get diag: %s\n",res);
		if(fgets(res, 8,fp))
		{
			if(atoi(res))
			{
				strcpy(gPingRes, "Error_Internal");
			}
			else
			{
				strcpy(gPingRes, "Complete");
			}
		}
		else
		{
			strcpy(gPingRes, "Error_Other");
		}
		fclose(fp);
		system("rm -rf /jffs/pingres");
	}
	else
	{
		strcpy(gPingRes,"None");
	}
	return gPingRes;
}


char *GetSuccessCount(char *str_arg)
{
/*
	FILE   *stream; 
	memset(szVal, 0x00, sizeof(szVal));
	stream = popen( "/bin/cat ping.txt |/bin/grep -o -i  -e '[0-9]\\{1,3\\} packets received'|/usr/bin/cut  -d' ' -f1", "r" );
	fread( szVal, sizeof(char), sizeof(szVal),	stream); 
	fclose( stream );
	szVal[strlen(szVal)-1]='\0';
	return szVal;*/
	if(gPingSuc[0])
		return gPingSuc;
	else
		return "0";
	
}
char *GetFailureCount(char *str_arg)
{
    /*int totalcount=0;
	int successcount=0;
	
	FILE   *stream; 
	memset(szVal, 0x00, sizeof(szVal));
	stream = popen( "/bin/cat /usr/bin/ping.txt |/bin/grep -o -i  -e '[0-9]\\{1,3\\} packets transmitted'|/usr/bin/cut  -d' ' -f1", "r" );
	fread( szVal, sizeof(char), sizeof(szVal),	stream); 
	fclose( stream );
	szVal[strlen(szVal)-1]='\0';
	totalcount=atoi(szVal);

	memset(szVal, 0x00, sizeof(szVal));
	stream = popen( "/bin/cat /usr/bin/ping.txt |/bin/grep -o -i  -e '[0-9]\\{1,3\\} packets received'|/usr/bin/cut  -d' ' -f1", "r" );
	fread( szVal, sizeof(char), sizeof(szVal),	stream); 
	fclose( stream );
	szVal[strlen(szVal)-1]='\0';
	successcount=atoi(szVal);
	
    memset(szVal, 0x00, sizeof(szVal));
	sprintf(szVal,"%d",(totalcount-successcount));
	
	return szVal;

*/
	if(gPingFal[0])
		return gPingFal;
	else
		return "0";

}

char *GetAverageResponseTime(char *str_arg)
{

	/*FILE   *stream; 
	memset(szVal, 0x00, sizeof(szVal));
	stream = popen( "/bin/cat /usr/bin/ping.txt |/bin/grep  -i 'round-trip'|/bin/grep -o -i -e '/[0-9.]\\{1,4\\}'|/usr/bin/cut -d'/' -f2", "r" );
	fread( szVal, sizeof(char), sizeof(szVal),	stream); 
	fclose( stream );
	szVal[strlen(szVal)-1]='\0';
	return szVal;*/
	if(gPingAvg[0])
		return gPingAvg;
	else
		return "0";


}
char *GetMinimumResponseTime(char *str_arg)
{
	/*FILE   *stream; 
	memset(szVal, 0x00, sizeof(szVal));
	stream = popen( "/bin/cat /usr/bin/ping.txt |/bin/grep  -i 'round-trip'|/bin/grep -o -i -e '[0-9.]\\{1,4\\}'", "r" );
	fread( szVal, sizeof(char), sizeof(szVal),	stream); 
	fclose( stream );
	szVal[strlen(szVal)-1]='\0';
	return szVal;*/
	if(gPingMin[0])
		return gPingMin;
	else
		return "0";


}
char *GetMaximumResponseTime(char *str_arg)
{
	/*FILE   *stream; 
	memset(szVal, 0x00, sizeof(szVal));
	stream = popen( "/bin/cat /usr/bin/ping.txt |/bin/grep -i 'round-trip'|/bin/grep -o -i -e '[0-9.]\\{1,4\\} ms'|/usr/bin/cut -d' ' -f1", "r" );
	fread( szVal, sizeof(char), sizeof(szVal),	stream); 
	fclose( stream );
	szVal[strlen(szVal)-1]='\0';
	return szVal;*/
	if(gPingMax[0])
		return gPingMax;
	else
		return "0";
}


/*********************
******ipinterface*****
***********************/

char *GetIPInterface1Enable()
{
    return "enable";
}


char *GetInterfaceEnable(char *str_arg)
{
   memset(szVal, 0x00, sizeof(szVal));
   wireless_get_enable(Get_vapnum(str_arg),szVal,sizeof(szVal));
   return szVal; //don't return local variable
}

char *GetIPInterface1IPAddr()
{
    FILE   *stream; 
    memset(szVal, 0x00, sizeof(szVal));
	stream = popen( "/sbin/ifconfig default |grep -o -i -e 'inet addr:[0-9]\\{1,3\\}\\.[0-9]\\{1,3\\}\\.[0-9]\\{1,3\\}\\.[0-9]\\{1,3\\}'| cut -d: -f2", "r" );
	if(stream)
	{
		fread( szVal, sizeof(char), sizeof(szVal),  stream); 
		pclose( stream );
	}
    szVal[strlen(szVal)-1]='\0';
    return szVal;
}

char *GetIPInterface1SubnetMask()
{
    FILE   *stream; 
    memset(szVal, 0x00, sizeof(szVal));
	stream = popen( "/sbin/ifconfig default |grep -o -i -e 'mask:[0-9]\\{1,3\\}\\.[0-9]\\{1,3\\}\\.[0-9]\\{1,3\\}\\.[0-9]\\{1,3\\}' | cut -d: -f2", "r" );
	if(stream)
	{
		fread( szVal, sizeof(char), sizeof(szVal),  stream); 
		pclose( stream );
	}
    szVal[strlen(szVal)-1]='\0';
    return szVal;
}

char *GetIPInterface1AddressingType()
{
   memset(szVal, 0x00, sizeof(szVal));
   network_get_dnsmode(szVal,sizeof(szVal));
   return szVal;
}

int SetIPInterface1IPAddr(char *str_arg, char *str_szval)
{
   network_set_ip("default",str_szval);
   system("touch /tmp/ipchange-flag");
   return 0;
}
int SetIPInterface1SubnetMask(char *str_arg,char *str_szval)
{
   network_set_netmask("default",str_szval);
   return 0;
}
int SetIPInterface1DefaultGateway(char *str_arg,char *str_szval)
{
	network_set_gw(str_szval);
	return 0;
}
int SetLogIP(char *str_arg,char *str_szval)
{ 
	syslog_set_ip(str_szval,1);
	return 0;
}

int SetIsolation(char *str_arg,char *str_szval)
{

	wireless_config_isolation(Get_vapnum(str_arg),str_szval);
	return 0;

}
int SetMaxUser(char *str_arg,char *str_szval)
{

	wireless_config_maxsimultusers(Get_vapnum(str_arg),str_szval);
	return 0;

}
/*********************
******ntp****************
***********************/

char *GetNTPServer1()
{
    memset(szVal, 0x00, sizeof(szVal));
    network_get_ntp(szVal,1,sizeof(szVal));
    return szVal;
}

char *GetNTPServer2()
{
    memset(szVal, 0x00, sizeof(szVal));
    network_get_ntp(szVal,2,sizeof(szVal));
    return szVal;
}

char *GetLocalTimeZone()
{
    memset(szVal, 0x00, sizeof(szVal));
    //network_get_timezone(szVal,sizeof(szVal));
    return szVal;
}

char *GetCurrentLocalTime()
{  
    FILE   *stream; 
	memset(szVal, 0x00, sizeof(szVal));
	stream = popen( "date -R", "r" );
	if(stream)
	{
		fread( szVal, sizeof(char), sizeof(szVal),	stream); 
		pclose( stream );
	}
	return szVal;
}

char * GetDNSServersMode()
{
    memset(szVal, 0x00, sizeof(szVal));
    network_get_dnsmode(szVal,sizeof(szVal));
    return szVal;
}

char * GetDNSServers1()
{
    memset(szVal, 0x00, sizeof(szVal));
    network_get_dns(szVal,1,sizeof(szVal));
    return szVal;
}
char * GetDNSServers2()
{
	memset(szVal, 0x00, sizeof(szVal));
    network_get_dns(szVal,2,sizeof(szVal));
    return szVal;
}



void SetHardwareVersion(char *_szVal)
{
  dprintf(LOG_LEVEL1, "\n!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
  dprintf(LOG_LEVEL1, "!!!!!!!!!!! SET HardwareVersion !!!!!!!!!!!\n");
  dprintf(LOG_LEVEL1, "val : %s\n", _szVal);
  dprintf(LOG_LEVEL1, "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n\n");
  sprintf(szHardwareVersion, "%s", _szVal);
}

#define TR69_PORT  "7547"
#define COL ":"
char * GetCpeURL(char *str_arg)
{
    FILE *fp;
    char szTemp[128];
    memset(szVal, 0x00, 128);
    memset(szVal, 0x00, 256);
    fp = popen("/sbin/ifconfig default | awk -F \":|[ ]+\" '/inet addr/{print $4}' ", "r");
    if(fp )
    {
		fgets(szTemp, 128, fp);
    }
	pclose(fp);
    int length = strlen(szTemp);
    if(szTemp[length-1] == '\n')
        szTemp[length-1] = '\0';
    strcpy(szVal, "http://");
    strcat(szVal, szTemp);
    strcat(szVal, COL);
    strcat(szVal, TR69_PORT);
	return szVal;
}
//////////////////update by wangjr 091124////////////////////////////////////

char * SetCpeURL(char *str_arg, char *str_szval)
{
	network_set_ip("default",str_szval);
    system("touch /tmp/ipchange-flag");
	return szVal;
}

char * GetParKey(char *str_arg)
{
	return "NULL";
}

char * GetWanIp(char *str_arg)
{
    FILE *fp;
	int length = 0;
    memset(szVal, 0x00, 256);
    fp = popen("/sbin/ifconfig default | awk -F \":|[ ]+\" '/inet addr/{print $4}' ", "r");
    if(fp)
    {
    	fgets(szVal, 256, fp);
    }
	pclose(fp);
	length = strlen(szVal);
    if(szVal[length-1] == '\n')
        szVal[length-1] = '\0';
	return szVal;    
}

char *GetTeleACEnable(char *str_arg)
{
	return "0";
}

char *GetTeleACPass(char *str_arg)
{
	return "NULL";
}
//////////////////update by wangjr 091124////////////////////////////////////

int SetMonEnable(char *str_arg,char *str_szval)
{
	if(strcmp(str_szval, "0"))
	{
		system("echo 0 > /jffs/monflag && mkdir -p /jffs/mon");
	}
	else
	{
		system("rm -f /jffs/monflag");
	}
	return 0;
}

char* GetMonEnable(char *str_arg)
{
	FILE *fp;
	if(fp = fopen("/jffs/monflag", "r"))
	{
		fclose(fp);
		return "1";
	}
	return "0";
}

char* GetMonCount(char *str_arg)
{
	FILE *fp;
	int tmp = 0;
	memset(szVal, 0x00, 256);
	if(fp = popen("ls -l /jffs/mon/ | grep -c monpara", "r"))
	{
		fgets(szVal, 256, fp);
		tmp = atoi(szVal);
		memset(szVal, 0x00, 256);
		sprintf(szVal,"%d",tmp);
		pclose(fp);
		return szVal;
	}
	return "0";
}
//////////////////update by wangjr 091124////////////////////////////////////

int SetMonParaList(char *str_arg,char *str_szval)
{
	char * tag = "InternetGatewayDevice.DeviceInfo.X_CT-COM_Monitor.MonitorConfig.";
	char * iter = str_arg+strlen(tag);
	int seq = atoi(iter);
	char syscmd[128];
	memset(syscmd, 0x00, 128);
	if(!strcmp(str_szval, "NULL"))
	{
		sprintf(syscmd, "rm -f /jffs/mon/monpara%d", seq);
	}
	else
	{
		sprintf(syscmd, "echo \"%s\" > /jffs/mon/monpara%d", str_szval, seq);
	}

	system(syscmd);
	return 0;
}

char *GetMonParaList(char *str_arg)
{
	char * tag = "InternetGatewayDevice.DeviceInfo.X_CT-COM_Monitor.MonitorConfig.";
	char * iter = str_arg+strlen(tag);
	int seq = 0;
	FILE *fp;
	char syscmd[128];
	memset(syscmd, 0x00, 128);	
	seq = atoi(iter);
	sprintf(syscmd, "/jffs/mon/monpara%d",	seq);
	if(fp = fopen(syscmd, "r"))
	{
		fgets(szVal, 256, fp);
		szVal[strlen(szVal)-1] = '\0';
		fclose(fp);
		return szVal;
	}
	return "NULL";
}

int SetMonTimeList(char *str_arg,char *str_szval)
{
	char * tag = "InternetGatewayDevice.DeviceInfo.X_CT-COM_Monitor.MonitorConfig.";
	char * iter = str_arg+strlen(tag);
	int seq = 0;
	char syscmd[128];
	memset(syscmd, 0x00, 128);
	seq = atoi(iter);
	if(!strcmp(str_szval, "NULL"))
	{
		sprintf(syscmd, "rm -f /jffs/mon/montime%d", seq);
	}
	else
	{
		sprintf(syscmd, "echo \"%s\" > /jffs/mon/montime%d", str_szval, seq);
		system(syscmd);
	}
	return 0;
}

char *GetMonTimeList(char *str_arg)
{
	char * tag = "InternetGatewayDevice.DeviceInfo.X_CT-COM_Monitor.MonitorConfig.";
	char * iter = str_arg+strlen(tag);
	int seq = atoi(iter);
	FILE *fp;
	char syscmd[128];
	memset(syscmd, 0x00, 128);
	sprintf(syscmd, "/jffs/mon/montime%d", seq);
	if(fp = fopen(syscmd, "r"))
	{
		fgets(szVal, 256, fp);
		szVal[strlen(szVal)-1] = '\0';
		fclose(fp);
		return szVal;
	}

	return "NULL";
}
//////////////////update by wangjr 091124////////////////////////////////////

int SetAlarmEnable(char *str_arg,char *str_szval)
{
	if(strcmp(str_szval, "0"))
	{
		system("echo 0 > /jffs/alarmflag && mkdir -p /jffs/alarm");
	}
	else
	{
		system("rm -f /jffs/alarmflag");
	}
	return 0;
}

char* GetAlarmEnable(char *str_arg)
{
	FILE *fp;
	if(fp = fopen("/jffs/alarmflag", "r"))
	{
		fclose(fp);
		return "1";
	}
	return "0";
}

char* GetAlarmCount(char *str_arg)
{
	FILE *fp;
	int tmp = 0;
	memset(szVal, 0x00, 256);
	if(fp = popen("ls -l /jffs/alarm/ | grep -c alarmpara", "r"))
	{
		fgets(szVal, 256, fp);
		tmp = atoi(szVal);
		memset(szVal, 0x00, 256);
		sprintf(szVal,"%d",tmp);
		pclose(fp);
		return szVal;
	}
	return "0";
}
//////////////////update by wangjr 091124////////////////////////////////////

int SetAlarmParaList(char *str_arg,char *str_szval)
{
	char * tag = "InternetGatewayDevice.DeviceInfo.X_CT-COM_Alarm.AlarmConfig.";
	char * iter = str_arg+strlen(tag);
	int seq = atoi(iter);
	char syscmd[128];
	memset(syscmd, 0x00, 128);
	if(!strcmp(str_szval, "NULL"))
	{
		sprintf(syscmd, "rm -f /jffs/alarm/alarmpara%d", seq);
	}
	else
	{
        sprintf(syscmd, "echo \"%s\" > /jffs/alarm/alarmpara%d", str_szval, seq);
		system(syscmd);
	}
	return 0;
}

char *GetAlarmParaList(char *str_arg)
{
	char * tag = "InternetGatewayDevice.DeviceInfo.X_CT-COM_Alarm.AlarmConfig.";
	char * iter = str_arg+strlen(tag);
	int seq = 0;
	FILE *fp;
	char syscmd[128];
	memset(syscmd, 0x00, 128);	
	seq = atoi(iter);
	sprintf(syscmd, "/jffs/alarm/alarmpara%d",	seq);
	if(fp = fopen(syscmd, "r"))
	{
		fgets(szVal, 256, fp);
		szVal[strlen(szVal)-1] = '\0';
		fclose(fp);
		return szVal;
	}
	return "NULL";
}

int SetAlarmTimeList(char *str_arg,char *str_szval)
{
	char * tag = "InternetGatewayDevice.DeviceInfo.X_CT-COM_Alarm.AlarmConfig.";
	char * iter = str_arg+strlen(tag);
	int seq = 0;
	char syscmd[128];
	memset(syscmd, 0x00, 128);
	seq = atoi(iter);
	if(!strcmp(str_szval, "NULL"))
	{
		sprintf(syscmd, "rm -f /jffs/alarm/alarmtime%d", seq);
	}
	else
	{
		sprintf(syscmd, "echo \"%s\" > /jffs/alarm/alarmtime%d", str_szval, seq);
		system(syscmd);
	}
	return 0;
}

char *GetAlarmTimeList(char *str_arg)
{
	char * tag = "InternetGatewayDevice.DeviceInfo.X_CT-COM_Alarm.AlarmConfig.";
	char * iter = str_arg+strlen(tag);
	int seq = atoi(iter);
	FILE *fp;
	char syscmd[128];
	memset(syscmd, 0x00, 128);
	sprintf(syscmd, "/jffs/alarm/alarmtime%d", seq);
	if(fp = fopen(syscmd, "r"))
	{
		fgets(szVal, 256, fp);
		szVal[strlen(szVal)-1] = '\0';
		fclose(fp);
		return szVal;
	}

	return "NULL";
}

int SetAlarmLMax(char *str_arg,char *str_szval)
{
	char * tag = "InternetGatewayDevice.DeviceInfo.X_CT-COM_Alarm.AlarmConfig.";
	char * iter = str_arg+strlen(tag);
	int seq = 0;
	char syscmd[128];
	memset(syscmd, 0x00, 128);
	seq = atoi(iter);
	if(!strcmp(str_szval, "NULL"))
	{
		sprintf(syscmd, "rm -f /jffs/alarm/alarmmax%d", seq);
	}
	else
	{
		sprintf(syscmd, "echo \"%s\" > /jffs/alarm/alarmmax%d", str_szval, seq);
		system(syscmd);
	}
	return 0;
}

char *GetAlarmLMax(char *str_arg)
{
	char * tag = "InternetGatewayDevice.DeviceInfo.X_CT-COM_Alarm.AlarmConfig.";
	char * iter = str_arg+strlen(tag);
	int seq = atoi(iter);
	FILE *fp;
	char syscmd[128];
	memset(syscmd, 0x00, 128);
	sprintf(syscmd, "/jffs/alarm/alarmmax%d", seq);
	if(fp = fopen(syscmd, "r"))
	{
		fgets(szVal, 256, fp);
		szVal[strlen(szVal)-1] = '\0';
		fclose(fp);
		return szVal;
	}

	return "NULL";
}

int SetAlarmLMin(char *str_arg,char *str_szval)
{
	char * tag = "InternetGatewayDevice.DeviceInfo.X_CT-COM_Alarm.AlarmConfig.";
	char * iter = str_arg+strlen(tag);
	int seq = 0;
	char syscmd[128];
	memset(syscmd, 0x00, 128);
	seq = atoi(iter);
	if(!strcmp(str_szval, "NULL"))
	{
		sprintf(syscmd, "rm -f /jffs/alarm/alarmmin%d", seq);
	}
	else
	{
		sprintf(syscmd, "echo \"%s\" > /jffs/alarm/alarmmin%d", str_szval, seq);
		system(syscmd);
	}
	return 0;
}

char *GetAlarmLMin(char *str_arg)
{
	char * tag = "InternetGatewayDevice.DeviceInfo.X_CT-COM_Alarm.AlarmConfig.";
	char * iter = str_arg+strlen(tag);
	int seq = atoi(iter);
	FILE *fp;
	char syscmd[128];
	memset(syscmd, 0x00, 128);
	sprintf(syscmd, "/jffs/alarm/alarmmin%d", seq);
	if(fp = fopen(syscmd, "r"))
	{
		fgets(szVal, 256, fp);
		szVal[strlen(szVal)-1] = '\0';
		fclose(fp);
		return szVal;
	}

	return "NULL";
}

int SetAlarmMode(char *str_arg,char *str_szval)
{
	strcpy(g_alarm_mode, str_szval);
	return 0;
}

char *GetAlarmMode(char *str_arg)
{
	return g_alarm_mode;
}
//////////////////update by wangjr 091124////////////////////////////////////

char *GetPeriodicInfInterval(char *str_arg)
{
	FILE *fp;
	memset(szVal, 0, sizeof(szVal));
	if(fp = popen("/bin/cat /usr/bin/tr-069.conf |/bin/sed -n '2p'| /usr/bin/cut -d= -f2", "r"))
	{
		fgets(szVal, 256, fp);
		pclose(fp);
	}
	return szVal;
}

char *GetPeriodicInfEnable(char *str_arg)
{
	FILE *fp;
	memset(szVal, 0, sizeof(szVal));
	if(fp = popen("/bin/cat /usr/bin/tr-069.conf |/bin/sed -n 'p'| /usr/bin/cut -d= -f2", "r"))
	{
		fgets(szVal, 256, fp);
		pclose(fp);
	}
	return szVal;
}


#if 0
char *GetConnectionRequestURL()
{
struct hostent *hptr;
struct utsname myname;
struct in_addr **ptr;
int addrtype;

  if (uname(&myname) < 0)
    return NULL;

  if ((hptr = gethostbyname(myname.nodename)) == NULL)
    return NULL;

  addrtype = hptr->h_addrtype;
  ptr = (struct in_addr **)hptr->h_addr_list;

  sprintf(szConnectionRequestURL, "%s", inet_ntoa(**ptr));
  return szConnectionRequestURL;
}

char *GetParameterKey()
{
  return " ";
}

char *GetManufacturer()
{
  return "Siemens";
}

char *GetManufacturerOUI()
{
  return "001095";
}

char *GetModelName()
{
  return "Freeman 7100";
}

char *GetDescription()
{
  return "Freeman 7100 STB";
}



char *GetSerialNumber()
{
  return "sn00";
}

char *GetProvisioningCode()
{
  return "";
}

char *GetUpTime()
{
struct sysinfo info;
  sysinfo(&info);
  sprintf(szVal, "%ld", info.uptime);
  return szVal;
}

char *GetPeriodicInfEnable()
{
FILE *fdConfig = NULL;
int nParameterType;

  // default value
  memset(szVal, 0x00, sizeof(szVal));

  // Periodic Inform Enable value
  fdConfig = fopen(TR69_CONFIG_FILE, "r");
  if (fdConfig != NULL)
  {
    GetConfigValue(fdConfig, DEVICEINFO_MANAGEMENTSERVER_PERIODICINFORMENABLE, szVal,  &nParameterType);
    fclose(fdConfig);
  }
  return szVal;
}

char *GetPeriodicInfInterval()
{
FILE *fdConfig = NULL;
int nParameterType;

  // default value
  memset(szVal, 0x00, sizeof(szVal));

  // Periodic Inform Interval value
  fdConfig = fopen(TR69_CONFIG_FILE, "r");
  if (fdConfig != NULL)
  {
    GetConfigValue(fdConfig, DEVICEINFO_MANAGEMENTSERVER_PERIODICINFORMINTERVAL, szVal,  &nParameterType);
    fclose(fdConfig);
  }
  return szVal;
}

char *GetPeriodicInfTime()
{
  sprintf(szVal, "0001-01-01T00:00:00Z");
  return szVal;
}
#endif
/***************************************************************************/
/***                                                                     ***/
/***                          PROGRAMME PRINCIPAL                        ***/
/***                                                                     ***/
/***                                                                     ***/
/*** ex of execution:                                                    ***/
/*** ./localmanager --file:/var/tr-069/localmanager.conf --datamodel:localhost:8090 ***/
/***************************************************************************/ 

void sigpipe_handle(int x) 
{ 
  dprintf(LOG_LEVEL1, "SIGPIPE caught\n");
}

void sigint_handle(int x) 
{ 
  dprintf(LOG_LEVEL1, "SIGINT caught\n");
  exit(0);
}



int main(int argc, char *argv[])
{
printf("debug_level: %d\n",DEBUG_LEVEL);
dprintf(LOG_LEVEL1,"level1\n");
dprintf(LOG_LEVEL2,"level2\n");
dprintf(LOG_LEVEL3,"level3\n");
dprintf(LOG_LEVEL4,"level4\n");    
// command line arguments
int nArg;
char *ptr;

// config file values
FILE *fdConfig = NULL;
char szFileName[ARGSIZE];
char szConfigValue[BUFFERSIZE];
int nLength;
int nParameterType;

int nConfPortNumber, nConfConnectionTimeout = 60;

// thread
pthread_t tid;

SOAP_SOCKET m;
struct Namespace empty_namespaces[] = { { NULL } };
struct soap soap, soap_listen;


  dprintf(LOG_LEVEL1, "\nLOCAL_MANAGER..execution\n\n");

  /************add wangjr**********************/
  if((argc == 4) &&(strncmp(argv[3],"-d",strlen("-d")) == 0))
  {
	  if( daemon(0,0))
	  { 	  
		  dprintf(LOG_LEVEL3, "daemon error\n");		  
		  return 1 ;  
	  }   
  }

  // Command Line Arguments
  for (nArg = 1; nArg < argc; nArg++)
  {
    ptr = argv[nArg];
    if (strncmp(argv[nArg], "--file:", strlen("--file:")) == 0)
    {
      strcpy(szFileName, &ptr[strlen("--file:")]);
      dprintf(LOG_LEVEL1, "..szFileName: %s\n", szFileName);
    }

    if (strncmp(argv[nArg], "--datamodel:", strlen("--datamodel:")) == 0)
    {
      strcpy(gdatamodel_address, &ptr[strlen("--datamodel:")]);
      dprintf(LOG_LEVEL1, "..datamodel: %s\n", gdatamodel_address);
    }
  }

  // signal handling
  signal(SIGPIPE, sigpipe_handle);
  signal(SIGINT, sigint_handle);

  // Init semaphore synchronization between (request / answer)
  sem_init(&S_response, 0, 0);

  // Local manager conf values
  fdConfig = fopen(szFileName, "r");
  if (fdConfig != NULL)
  {
    // Search port number of thread server in Config File
    memset(szConfigValue, 0x00, BUFFERSIZE);
    nLength = GetConfigValue(fdConfig, SERVER_PORT, szConfigValue,  &nParameterType);
    if (nLength > 0) nConfPortNumber = atoi(szConfigValue);

    // Search Corresponding Value In Config File
    memset(szConfigValue, 0x00, BUFFERSIZE);
    nLength = GetConfigValue(fdConfig, CONNECTIONTIME_OUT, szConfigValue,  &nParameterType);
    if (nLength > 0) nConfConnectionTimeout = atoi(szConfigValue);

    fclose(fdConfig);
  }
  else 
  {
    dprintf(LOG_LEVEL1, "..file %s not found\n", szFileName);
    goto error_label;
  }

  // Create listening socket
  soap_init(&soap_listen);
  soap_listen.send_timeout = 60; // 60 seconds 
  soap_listen.recv_timeout = 60; // 60 seconds
  m = soap_bind(&soap_listen, NULL, nConfPortNumber, BACKLOG);
  if (!soap_valid_socket(m))
          exit(1);
  dprintf(LOG_LEVEL1, "..listening socket connection successful %d on port %d\n", m, nConfPortNumber);

  // server thread creation
  if (pthread_create(&tid, NULL, (void*(*)(void*))manager_server_thread, (void*)&soap_listen) != 0)
    goto error_label; // listening thread creation error

// soap struct initialization
// ATTENTION : Don't use SOAP_IO_KEEPALIVE during SUBSCRIBE message
// because each SUBSCRIBE must correspond to one TCP connection
  InitSoapStruct(&soap, empty_namespaces);

//Number of parameters
  gnbParameters = sizeof(parameters_table) / sizeof(STRUCT_MANAGER_INTERNALVALUES);

//SUBSCRIBE SEQUENCE (BEGIN)
  dprintf(LOG_LEVEL1, "..SUBSCRIPTION to Datamodel repository\n");

  FILE *fp = NULL;
  int fd_conf = 0;
  if ( NULL == (fp=fopen(LOCK_FILE, "w+")) )
  {
    return NULL;
  }
  
  fd_conf = fileno( fp );
  if( flock( fd_conf, LOCK_EX ) != 0 )
  {
    fclose( fp );
    return NULL;
  }

  if (SubscribeSequence(&soap, gdatamodel_address, nConfPortNumber) == 0)
  {
    dprintf(LOG_LEVEL3, "..SubscribeSequence ok!\n");
//    printf("=============\nsub post subscribe\n============\n");
//    sem_post(&S_subscrib);
  }
  else
  {
    dprintf(LOG_LEVEL1, "..Error on SubscribeSequence!\n");
    goto error_label; // listening thread creation error
  }
  flock( fd_conf, LOCK_UN );
  fclose(fp);

//SUBSCRIBE SEQUENCE (END)

  // poll changes on variables with active notification
  while(1)
  {
    /*fprintf(stderr, "poll parameters changes !\n");
    for (i=0; i < gnbParameters; i++)
    {
      fprintf(stderr, "parameters_table[%d].bValueChanged: %d!\n", i, parameters_table[i].bValueChanged);
    }

    sleep(10);

    if (nNotify == 1)
    {
      CreateNotify(gdatamodel_address, gId, "Device.DeviceInfo.UpTime", "50000");

      printf("Wait on semaphore\n");
      // wait for response message (semaphore)
      // don't forget to declare ret_sem_wait --> int ret_sem_wait;
      do {
        if ((ret_sem_wait=sem_wait(&S_response)) != 0)  // SEMAPHORE lock
            dprintf(LOG_LEVEL1, "ERROR on sem_wait !! \n");
      }
      while ((ret_sem_wait != 0) && (errno == EINTR));
      
      if (Response_OK != 1)
      {
        fprintf(stderr, "Error on response !!\n");
        break;
      }

      nNotify = 0;
    }*/

    sleep(50);

  }

//  (void) pthread_join(tid, NULL);

error_label:
  soap_destroy((struct soap*)&soap); // dealloc C++ data
  soap_end((struct soap*)&soap); // dealloc data and clean up
  soap_done(&soap); // detach soap struct

  dprintf(LOG_LEVEL3, "END OF MAIN PROGRAM\n");
  return 0;
}


/***************************************************************************/
/*** SUBSCRIBE SEQUENCE                                                  ***/
/***************************************************************************/

int SubscribeSequence(struct soap *_soap, char *_soap_endpoint, int _nConfPort)
{
int nRet = -1;
int i;
char szline[ARGSIZE];
int ret_sem_wait;

  Response_OK = 0; // response flag set to 1 when a response is received following a subscribe

  // subscribe to Data model repository for all parameters
  for (i=0; i < gnbParameters; i++)
  {
    nRet = -1;
    if (soap_connect_command(_soap, SOAP_POST, _soap_endpoint, "") == SOAP_OK)
    {
      // SUBSCRIBE MESSAGE
      soap_element_begin_out(_soap, CONST_MESSAGE, 0, NULL);
      soap_element_begin_out(_soap, "NAME", 0, NULL);
      sprintf(szline, CONST_SUBSCRIBE);
      soap_send_raw(_soap, szline, strlen(szline));
      soap_element_end_out(_soap, "NAME");
      soap_element_begin_out(_soap, "Type", 0, NULL);
      sprintf(szline, CONST_TYPELOCAL);
      soap_send_raw(_soap, szline, strlen(szline));
      soap_element_end_out(_soap, "Type");
      soap_element_begin_out(_soap, "Port", 0, NULL);
      sprintf(szline, "%d", _nConfPort);
      soap_send_raw(_soap, szline, strlen(szline));
      soap_element_end_out(_soap, "Port");
      soap_element_end_out(_soap, CONST_MESSAGE);

      // PARTS OF DATAMODEL
      CreateSubscribeMessage(parameters_table[i], _soap);

      soap_end_send(_soap);
    
      dprintf(LOG_LEVEL3, "Wait on semaphore\n");
      // wait for response message (semaphore)
      do {
        if ((ret_sem_wait=sem_wait(&S_response)) != 0)  // SEMAPHORE lock
            dprintf(LOG_LEVEL1, "ERROR on sem_wait !! \n");
      }
      while ((ret_sem_wait != 0) && (errno == EINTR));

      if (Response_OK != 1)
      {
        dprintf(LOG_LEVEL3, "Error on response !!\n");
        break;
      }

      nRet = 0;
    }
    else break;
  }

  return nRet;
}



/***************************************************************************/
/*** THREAD SERVER                                                       ***/
/***                                                                     ***/
/*** Wait for connection requests from Data model repository             ***/
/***************************************************************************/ 

void *manager_server_thread(void *_soap)
{
SOAP_SOCKET s;
struct soap *soap = _soap;
struct soap *tsoap;
// thread
pthread_t tid;

  dprintf(LOG_LEVEL1, "manager_server_thread !\n");

  for (;;)
  {
    s = soap_accept(soap);
    if (!soap_valid_socket(s))
    {
      if (soap->errnum)
      {
        soap_print_fault(soap, stderr);
        exit(1);
      }
      dprintf(LOG_LEVEL2, "..server timed out\n"); 
      break;
    }
    dprintf(LOG_LEVEL3, "..accepts socket %d connection from IP %ld.%ld.%ld.%ld\n", s, (soap->ip >> 24)&0xFF, (soap->ip >> 16)&0xFF, (soap->ip >> 8)&0xFF, soap->ip&0xFF);
    tsoap = soap_copy(soap); // make a safe copy
    if (!tsoap)
      break;
    pthread_create(&tid, NULL, (void*(*)(void*))reading_thread, (void*)tsoap);

    // wait for end of thread execution
    (void) pthread_join(tid, NULL); 

    dprintf(LOG_LEVEL3, "..server sem_post\n"); 
    // unlock main thread
    sem_post(&S_response);
  }

  soap_destroy((struct soap*)soap); // dealloc C++ data
  soap_end((struct soap*)soap); // dealloc data and clean up
  soap_done((struct soap*)soap); // detach soap struct

  return NULL;
}


/***************************************************************************/
/*** READING THREAD                                                      ***/
/***                                                                     ***/
/*** Read data on socket from Data model repository                      ***/
/***************************************************************************/ 

void *reading_thread(void *_soap)
{
int nError;
struct soap *soap = _soap;
STRUCT_MESSAGE message, *dupMessage;
LIST_PARAM_VALUE *list_param_value;
int ret_sem_wait;

enum methodName eMethodName;
enum hostType eHostType;
enum functionName eFunctionName;
// thread parameters
pthread_t tid;
STRUCT_THREAD_DATA threadargs;

  dprintf(LOG_LEVEL1, "reading_thread..read data from socket\n");
  nError = soap_begin_recv(soap);
  if (nError != 0) dprintf(LOG_LEVEL3, "reading_thread.. %lu, nError: %d!\n", (unsigned long)pthread_self(), nError);

  // list of (param,value)
  list_param_value = (LIST_PARAM_VALUE *)malloc(sizeof(LIST_PARAM_VALUE));
  memset(list_param_value, 0x00, sizeof(LIST_PARAM_VALUE));

  if (ExtractXMLTagsWithMultipleParams(soap, &message, list_param_value) != 0)
  {
    dprintf(LOG_LEVEL2, "ExtractXMLTags error\n");
    goto endthread_label;
  }

  message.ListParamValue = list_param_value;
  dprintf(LOG_LEVEL3, "message.ListParamValue: %p\n", message.ListParamValue);
  dprintf(LOG_LEVEL3, "list_param_value: %p\n", list_param_value);

  // Process message to retrieve method name, host type and function name
  if (ProcessMessage(&message, &eMethodName, &eHostType, &eFunctionName) > -1)
  {
    dprintf(LOG_LEVEL3, "eMethodName: %d\n", eMethodName);
    dprintf(LOG_LEVEL3, "eHostType: %d\n", eHostType);
    dprintf(LOG_LEVEL3, "eFunctionName: %d\n", eFunctionName);

    if ((eMethodName == RESPONSE) && (message.Id != -1)) // response message
    {
      Response_OK = 1;
      if (gId == -1) gId = message.Id;
    }
    else if ((eMethodName == REQUEST) && (eFunctionName != FCT_UNKNOWN)) // request message
    {
      Request_OK = 1;

      // copy of message and copy of list of param-value
      dupMessage = (STRUCT_MESSAGE*)malloc(sizeof(STRUCT_MESSAGE));
      memcpy(dupMessage, &message, sizeof(STRUCT_MESSAGE));
      dupMessage->ListParamValue = duplicate_queue(message.ListParamValue);
	  	dprintf(4, "------------------------->\n");
		dprintf(4, "----->%s\n",dupMessage->ListParamValue->szParameter);
		dprintf(4, "----->%s\n",message.ListParamValue->szParameter);
		dprintf(4, "------------------------->\n");
      // arguments of thread
      threadargs.soap = soap;
      threadargs.msg = dupMessage;

      // create an answering thread
      pthread_create(&tid, NULL, (void*(*)(void*))response_thread, (void*)&threadargs);

      // wait for end of thread execution
      (void) pthread_join(tid, NULL); 
    }
  }

endthread_label:
  nError = soap_end_recv(soap);
  if (nError != 0) dprintf(LOG_LEVEL2, "reading_thread.. nError: %d!\n", nError);

  soap_closesock(soap);

  if (list_param_value != NULL) free_queue(list_param_value); // free list of param-value

  soap_destroy((struct soap*)soap); // dealloc C++ data
  soap_end((struct soap*)soap); // dealloc data and clean up
  soap_done((struct soap*)soap); // detach soap struct
  free(soap);

  dprintf(LOG_LEVEL3, "reading_thread, before return !\n");

  return NULL;
}

//////////////////update by wangjr 091124////////////////////////////////////

/**************************************************************************/
/***
/***wangjr  string to other
/****
/**************************************************************************/
char* sting_to_singlestring(char *src,char *dest)
{
    int i=0;
	for(i=0;i<=strlen(src);i++)
	{
	   if(src[i]>='0' && src[i]<='9')
	   {
	   	    dest[i]='1';
	   }
	   else
	   {
	        dest[i]=src[i];
	   }
	}
		
	return dest;
}

/***************************************************************************/
/*** RESPONDING THREAD                                                   ***/
/***                                                                     ***/
/*** Send reponse to a request from Data model repository                ***/
/***************************************************************************/ 
void *response_thread(void *_threadArg)
{
	short bFound = 0;
	struct soap *soap;
	int i;
	STRUCT_THREAD_DATA *args;
	STRUCT_MESSAGE *message;
	char szVal[ARGSIZE];
	char *ptr;
	char szParameterName[ARGSIZE];//wangjr  091124
char szSingleParameterName[ARGSIZE];//wangjr 091124

  dprintf(LOG_LEVEL1, "response_thread\n");

  args = (STRUCT_THREAD_DATA *) _threadArg;
  soap = args->soap;
  message = args->msg;

  // Create response to server

  if ( strcmp(message->szFunction, CONST_FUNCTIONGETVALUE) == 0)
  {
    //Get Value of parameter
    for (i=0; i < gnbParameters; i++)
    {
     //dprintf(LOG_LEVEL3, "get parametervalue \n");
	 //dprintf(LOG_LEVEL3, "get parametervalue 2 \n");
	 //dprintf(LOG_LEVEL3, "get parametervalue :%s \n", parameters_table[i].szDataModelSyntax);
	 //dprintf(LOG_LEVEL3, "get parametervalue :%s \n", message->ListParamValue->szParameter);
      if (strcmp(parameters_table[i].szDataModelSyntax, message->ListParamValue->szParameter) == 0)
      {
        ptr = parameters_table[i].GetValue(parameters_table[i].szDataModelSyntax);
        if (ptr == NULL)
        {
          dprintf(LOG_LEVEL3, "NULL \n");
          strcpy(szVal, " ");
        }
        else{
			sprintf(szVal, "%s",ptr);
        }
        dprintf(LOG_LEVEL3, "response_thread, CreateResponseWithValues szVal: %s\n", szVal);

        CreateResponseWithValues(gdatamodel_address, gId, szVal);

        bFound = 1;
        break;
      }
    }
  }
  else if ( strcmp(message->szFunction, CONST_FUNCTIONSETVALUE) == 0)
  {
    //Set Value of parameter
    for (i=0; i < gnbParameters; i++)
    {
      if (strcmp(parameters_table[i].szDataModelSyntax, message->ListParamValue->szParameter) == 0)
      {
        parameters_table[i].SetValue(parameters_table[i].szDataModelSyntax,message->ListParamValue->szValue);

        // response
        CreateResponse(gdatamodel_address, gId);
        bFound = 1;
        break;
      }
    }

  }
/* TO BE IMPLEMENTED */
//  else if ( strcmp(message->szFunction, CONST_FUNCTIONGETNAMES) == 0)
//    CreateResponseWithNames(soap_endpoint, Id, );
//  else if ( strcmp(message->szFunction, CONST_FUNCTIONGETATTRIBUTES) == 0)
//    CreateResponseWithAttributes(soap_endpoint, Id, );
//  else if ( strcmp(message->szFunction, CONST_FUNCTIONSETATTRIBUTES) == 0)
//    CreateResponse(soap_endpoint, Id);

  if (bFound == 0)
  {
    CreateErrorResponse(gdatamodel_address, gId);
  }

  dprintf(LOG_LEVEL3, "response_thread, %lu before return !\n", (unsigned long)pthread_self());
  // free copy of message param-value list
  if (message->ListParamValue != NULL)
  {
    free_queue(message->ListParamValue);
    message->ListParamValue = NULL;
  }

  // free message
  if (message != NULL) free(message);
  return NULL;
}

#ifdef __cplusplus
}
#endif
