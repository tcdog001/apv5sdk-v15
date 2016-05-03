
/**********************************************************************************
* Copyright (c) 2008-2011  Beijing Autelan Technology Co. Ltd.
* All rights reserved.
*
* filename: cwmp.c  
* description:  implementation for cwmp    
*  - Communicate with ACS CWMP 
*  - Create a thread server (requests from Data model repository)
*  - Subscribe to Data model repository (XML message) 
************************************************************************************/
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <sys/sysinfo.h>
#include <time.h>

#include <pthread.h>
#include <semaphore.h>

#include "Stub.nsmap"


#include "const.h"
#include "Manager.h"
#include "cwmp.h"

#include "tools.h"
#include "messages.h"
#include "FileConf.h"

#include "httpda.h"

#include "ACSInterface.h"

#include "Fault.h"

#define SPRINT_MAX_LEN 100
#ifdef __cplusplus
extern "C" {
#endif

struct Namespace *namespaces;

static sem_t S_response;
static sem_t S_notification;
static sem_t S_download;
static sem_t S_upload;

static sem_t S_cmdKey;
static sem_t S_transfercomplete;
static sem_t S_faultcode;
static sem_t S_listparamvalues;
static sem_t S_metux;

static int gflagResponse_OK = 0;
static int gDownloadResponse_status = 0;  //add by wangjr 091124
static int gId = -1;
static int gUploadResponse_status = 0;  //add by wangjr 091124
static int gAlarm = 0; //add by wangjr 091124
static int gfirmupgrade = 0; //add by wangjr 091124
static int gupdateconfig = 0; //add by wangjr 091203
static int greboot=0;  //add by wangjr 091124
static int gfactory=0;  //add by wangjr 091124

static LIST_PARAM_VALUE *gListActiveNotification=NULL;  // list of param-value with active notification
static LIST_PARAM_VALUE *gListPassiveListParamValues; //add by wangjr 091124

char  gActiveParameter[20][MAX_PARAMETER]={"","",}; //add by wangjr 091124

char EventCode[][30]    = {"0 BOOTSTRAT", "1 BOOT", "2 PERIODIC", "4 VALUE CHANGE", "6 CONNECTION REQUEST", "7 TRANSFER COMPLETE", "8 DIAGNOSTICS COMPLETE", "EventCode X _CT-COM MONITOR", "EventCode X _CT-COM ALARM"};

short bFirstTime = 1;


//char ACS_address[ARGSIZE]="http://localhost:8060";
char ACS_address[ARGSIZE]="http://sedm.cert.motive.com:8003/cwmpWeb/CPEMgt"; // default ACS


// datamodel address
static char gdatamodel_address[ARGSIZE];
static char gupgrade_filename[ARGSIZE]={0}; //add by wangjr 091124
static char gconfig_filename[ARGSIZE]={0}; //add by wangjr 091203
//static char ACS_userid[32];
//static char ACS_realm[32];

// Active Notification indication
//
// mutual exclusion
static int gflagNotification = 0;
time_t tmp_StartTime;  //add by wangjr 091124

void setActiveNotification(short nVal)
{
int ret_sem_wait;
  do {
    if ((ret_sem_wait=sem_wait(&S_notification)) != 0)  // SEMAPHORE lock
        dprintf(LOG_LEVEL1, "setActiveNotification, ERROR on sem_wait !! \n");
  }
  while ((ret_sem_wait != 0) && (errno == EINTR));

  gflagNotification = nVal;

  sem_post(&S_notification);
}

short getActiveNotification()
{
short nRet;
int ret_sem_wait;
  do {
    if ((ret_sem_wait=sem_wait(&S_notification)) != 0)  // SEMAPHORE lock
        dprintf(LOG_LEVEL1, "getActiveNotification, ERROR on sem_wait !! \n");
  }
  while ((ret_sem_wait != 0) && (errno == EINTR));
  nRet = gflagNotification;
  sem_post(&S_notification);
  return nRet;
}

// Command Key parameter
//
// mutual exclusion
static char gEventCmdKey[33] = "";

void setCommandKey(char *_szVal) // set EventCmdKey string
{
int ret_sem_wait;
  do {
    if ((ret_sem_wait=sem_wait(&S_cmdKey)) != 0)  // SEMAPHORE lock
        dprintf(LOG_LEVEL1, "setCommandKey, ERROR on sem_wait !! \n");
  }
  while ((ret_sem_wait != 0) && (errno == EINTR));

  strcpy(gEventCmdKey, _szVal);

  sem_post(&S_cmdKey);
}

void getCommandKey(char *_szVal) // get EventCmdKey string
{
int ret_sem_wait;
  do {
    if ((ret_sem_wait=sem_wait(&S_cmdKey)) != 0)  // SEMAPHORE lock
        dprintf(LOG_LEVEL1, "getCommandKey, ERROR on sem_wait !! \n");
  }
  while ((ret_sem_wait != 0) && (errno == EINTR));

  if (_szVal != NULL) strcpy(_szVal, gEventCmdKey);

  sem_post(&S_cmdKey);
}

// Transfer complete indication
//
// mutual exclusion
static short gTransferComplete = 0;

void setTransferComplete(short nVal)
{
int ret_sem_wait;
  do {
    if ((ret_sem_wait=sem_wait(&S_transfercomplete)) != 0)  // SEMAPHORE lock
        dprintf(LOG_LEVEL1, "setTransferComplete, ERROR on sem_wait !! \n");
  }
  while ((ret_sem_wait != 0) && (errno == EINTR));

  gTransferComplete = nVal;

  sem_post(&S_transfercomplete);
}

short getTransferComplete()
{
short nRet;
int ret_sem_wait;
  do {
    if ((ret_sem_wait=sem_wait(&S_transfercomplete)) != 0)  // SEMAPHORE lock
        dprintf(LOG_LEVEL1, "getTransferComplete, ERROR on sem_wait !! \n");
  }
  while ((ret_sem_wait != 0) && (errno == EINTR));

  nRet = gTransferComplete;

  sem_post(&S_transfercomplete);
  return nRet;
}

// Transfer complete fault code
//
// mutual exclusion
static unsigned int gFaultCode;
unsigned int getFaultCode()
{
unsigned int nRet;
int ret_sem_wait;
  do {
    if ((ret_sem_wait=sem_wait(&S_faultcode)) != 0)  // SEMAPHORE lock
        dprintf(LOG_LEVEL1, "getFaultCode, ERROR on sem_wait !! \n");
  }
  while ((ret_sem_wait != 0) && (errno == EINTR));

  nRet = gFaultCode;

  sem_post(&S_faultcode);
  return nRet;
}

void setFaultCode(unsigned int _nVal)
{
int ret_sem_wait;
  do {
    if ((ret_sem_wait=sem_wait(&S_faultcode)) != 0)  // SEMAPHORE lock
        dprintf(LOG_LEVEL1, "setFaultCode, ERROR on sem_wait !! \n");
  }
  while ((ret_sem_wait != 0) && (errno == EINTR));

  gFaultCode = _nVal;

  sem_post(&S_faultcode);
}

// global (param, value) list 
//
// mutual exclusion
static LIST_PARAM_VALUE *gListParamValues=NULL;

short set_globalList(LIST_PARAM_VALUE **destListParamValues, LIST_PARAM_VALUE *srcListParamValues)
{
short nRet = 0;
int ret_sem_wait;
  do {
    if ((ret_sem_wait=sem_wait(&S_listparamvalues)) != 0)  // SEMAPHORE lock
        dprintf(LOG_LEVEL1, "set_globalList, ERROR on sem_wait !! \n");
  }
  while ((ret_sem_wait != 0) && (errno == EINTR));

  if (*destListParamValues != NULL)
  {
    free_queue(*destListParamValues);
    *destListParamValues = NULL;
  }

  if (srcListParamValues != NULL)
    *destListParamValues = duplicate_queue(srcListParamValues);

  if (*destListParamValues != NULL) nRet = 1;
  sem_post(&S_listparamvalues);
  return nRet;
}

/***************************************************************************/
/***                                                                     ***/
/***                          PROGRAMME PRINCIPAL                        ***/
/***                                                                     ***/
/***                                                                     ***/
/*** ex of execution:                                                    ***/
/*** ./cwmp --ACS:http://devhdm7.cert.motive.com:7003/cwmpWeb/CPEMgt --proxy_host:141.11.159.9 --proxy_port:80 --proxy_userid:SVCRENN-APTGET --proxy_passwd:'Proxy;-)2005' --file:/var/tr-069/cwmp.conf --datamodel:http://localhost:8090 ***/
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


char szFileName[ARGSIZE];
char szAuthrealm[BUFFERSIZE];
char szAuthusername[BUFFERSIZE];
char szAuthPasswd[BUFFERSIZE];
char szWWWAuthrealm[BUFFERSIZE];
char szWWWAuthusername[BUFFERSIZE];
char szWWWAuthPasswd[BUFFERSIZE];
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
char szConfigValue[BUFFERSIZE];



int nLength;
int nParameterType;

char soap_proxy_host[ARGSIZE]="";
int soap_proxy_port=0;
char soap_proxy_userid[ARGSIZE]="";
char soap_proxy_passwd[ARGSIZE]="";

char szLocalDate[40];

int nConfPortNumber, nConfConnectionTimeout = 60;

// thread
pthread_t tid, tDownload,tUpload;

//pthread_t tid, tDownload;

SOAP_SOCKET m;
struct soap soap, soap_listen;
struct SOAP_ENV__Header header; // SOAP Header
struct Namespace empty_namespaces[] = { { NULL } };
struct InformResponse InformResponse;
struct cwmp__GetParameterValues tmp_cwmp__GetParameterValuesRequest;
struct cwmp__Reboot tmp_cwmp__RebootRequest;
struct cwmp__FactoryReset tmp_cwmp__FactroyResetRequest;  //add by wangjr 091124
struct cwmp__SetParameterValues tmp_cwmp__SetParameterValuesRequest;
struct cwmp__Download tmp_cwmp__DownloadRequest;
struct cwmp__Upload tmp_cwmp__UploadRequest;
struct cwmp__DeleteObject tmp_cwmp__DeleteObjectRequest; //add by wangjr 091124
struct cwmp__AddObject tmp_cwmp__AddObjectRequest; //add by wangjr 091124
struct cwmp__GetParameterNames tmp_cwmp__GetParameterNamesRequest; //add by wangjr 091124
struct cwmp__GetParameterAttributes tmp_cwmp__GetParameterAttributesRequest;  
struct cwmp__SetParameterAttributes tmp_cwmp__SetParameterAttributesRequest;
	struct cwmp__GetRPCMethods tmp_cwmp__GetRPCMethods;  //add by wangjr 091202

//printf("%d\n",__LINE__);
get_vendor_info( &st_vendor );
//printf("%d\n",__LINE__);

//struct cwmp__FactoryReset tmp_cwmp__FactroyResetRequest;

short bContinue;
int nError = 0;
short bReboot = 0;
short bFactoryReset=0;
int status;
char szEventCommandKey[40];
char *szFaultString;
int nPeriodicTime = 120;   // Default Periodic Time Value
int bPeriodicEnable = 1;  // Default Periodic Enable Value

time_t tLastInform_period;     // time since period inform
time_t tLastInform_monitor;    //time since monitor inform
time_t tLastInform_alarm;    //time since alarm inform  add by wangjr 091124
time_t tLastInform;            //localtime
short bInformToDo = 1; // flag indicating Inform to do
char *ptrEventCode;
int ret_sem_wait;
int ssl_flag = 0;
int digest_flag = 0;

struct http_da_info info;
struct threadDownloadargs downloadargs;
struct threadUploadargs uploadargs;
int mon_timer=0; //add by wangjr
  int alarm_timer=0; //add by wangjr

  dprintf(LOG_LEVEL1, "\nCWMP..execution\n\n");

  memset(&info, 0, sizeof(struct http_da_info));
  
  // signal handling
  signal(SIGPIPE, sigpipe_handle);
  signal(SIGINT, sigint_handle);

  sem_init(&S_response, 0, 0);      // semaphore synchronization between (request / answer)
  sem_init(&S_download, 0, 0);      // semaphore synchronization with download process
  sem_init(&S_upload, 0, 0);		// semaphore synchronization with download process

  sem_init(&S_notification, 0, 1);      // mutual exclusion on ActiveNotification flag
  sem_init(&S_cmdKey, 0, 1);            // mutual exclusion on EventCmdKey flag
  sem_init(&S_transfercomplete, 0, 1);  // mutual exclusion on transferComplete flag
  sem_init(&S_faultcode, 0, 1);         // mutual exclusion on faultcode integer
  sem_init(&S_listparamvalues, 0, 1);   // mutual exclusion on (param, value) list copy 
  sem_init(&S_metux, 0, 1);

  // Command Line Arguments
  for (nArg = 1; nArg < argc; nArg++)
  {
    ptr = argv[nArg];
    if (strncmp(argv[nArg], "--file:", strlen("--file:")) == 0)
    {
      strcpy(szFileName, &ptr[strlen("--file:")]);
      dprintf(LOG_LEVEL1, "..szFileName: %s\n", szFileName);
    }
    else if (strncmp(argv[nArg], "--datamodel:", strlen("--datamodel:")) == 0)
    {
      strcpy(gdatamodel_address, &ptr[strlen("--datamodel:")]);
      dprintf(LOG_LEVEL1, "..gdatamodel address: %s\n", gdatamodel_address);
    }
    else if (strncmp(argv[nArg], "--ACS:", strlen("--ACS:")) == 0)
    {
      strcpy(ACS_address, &ptr[strlen("--ACS:")]);
      dprintf(LOG_LEVEL1, "..ACS_address: %s\n", ACS_address);
    }
    else if (strncmp(argv[nArg], "--proxy_host:", strlen("--proxy_host:")) == 0)
    {
      strcpy(soap_proxy_host, &ptr[strlen("--proxy_host:")]);
      dprintf(LOG_LEVEL1, "..soap_proxy_host: %s\n", soap_proxy_host);
    }
    else if (strncmp(argv[nArg], "--proxy_port:", strlen("--proxy_port:")) == 0) 
    {
      soap_proxy_port = atoi(&ptr[strlen("--proxy_port:")]);
      dprintf(LOG_LEVEL1, "..proxy_port: %d\n", soap_proxy_port);
    }
    else if (strncmp(argv[nArg], "--proxy_userid:", strlen("--proxy_userid:")) == 0) 
    {
      strcpy(soap_proxy_userid, &ptr[strlen("--proxy_userid:")]);
      dprintf(LOG_LEVEL1, "..proxy_userid: %s\n", soap_proxy_userid);
    }
    else if (strncmp(argv[nArg], "--proxy_passwd:", strlen("--proxy_passwd:")) == 0) 
    {
      strcpy(soap_proxy_passwd, &ptr[strlen("--proxy_passwd:")]);
      dprintf(LOG_LEVEL1, "..proxy_passwd: %s\n", soap_proxy_passwd);
    }
  }
  
  // cwmp conf values
  fdConfig = fopen(szFileName, "r");
  if (fdConfig != NULL)
  {
    // Search port number of thread server in Config File
    memset(szConfigValue, 0x00, BUFFERSIZE);
    nLength = GetConfigValue(fdConfig, SERVER_PORT, szConfigValue,  &nParameterType);
    if (nLength > 0) nConfPortNumber = atoi(szConfigValue);

    // Search timeout Value In Config File
    memset(szConfigValue, 0x00, BUFFERSIZE);
    nLength = GetConfigValue(fdConfig, CONNECTIONTIME_OUT, szConfigValue,  &nParameterType);
    if (nLength > 0) nConfConnectionTimeout = atoi(szConfigValue);

    //user name
    memset(szAuthusername, 0x00, BUFFERSIZE);
    nLength = GetConfigValue(fdConfig, CONST_AUTH_USERNAME, szAuthusername,  &nParameterType);
    if (nLength <= 0) strcpy(szAuthusername, DEFAULT_SERIALNUMBER);

    //password
    memset(szAuthPasswd, 0x00, BUFFERSIZE);
    nLength = GetConfigValue(fdConfig, CONST_AUTH_PASSWORD, szAuthPasswd,  &nParameterType);
    if (nLength <= 0) strcpy(szAuthPasswd, DEFAULT_PASSWD);

    //realm
    memset(szAuthrealm, 0x00, BUFFERSIZE);
    nLength = GetConfigValue(fdConfig, CONST_AUTH_REALM, szAuthrealm,  &nParameterType);
    //if (nLength <= 0) strcpy(szAuthrealm, DEFAULT_REALM);

    //www-realm
    memset(szWWWAuthrealm, 0x00, BUFFERSIZE);
    nLength = GetConfigValue(fdConfig, CONST_WWW_AUTH_REALM, szWWWAuthrealm,  &nParameterType);
    if (nLength <= 0) strcpy(szWWWAuthrealm, DEFAULT_WWW_REALM);

    //www-password
    memset(szWWWAuthPasswd, 0x00, BUFFERSIZE);
    nLength = GetConfigValue(fdConfig, CONST_WWW_AUTH_PASSWORD, szWWWAuthPasswd,  &nParameterType);
    //if (nLength <= 0) strcpy(szWWWAuthPasswd, DEFAULT_WWW_PASSWORD);

    //www-username
    memset(szWWWAuthusername, 0x00, BUFFERSIZE);
    nLength = GetConfigValue(fdConfig, CONST_WWW_AUTH_USERNAME, szWWWAuthusername,  &nParameterType);
    //if (nLength <= 0) strcpy(szWWWAuthusername, DEFAULT_WWW_USERNAME);
    fclose(fdConfig);
    
  }
  else 
  {
    dprintf(LOG_LEVEL1, "..file %s not found\n", szFileName);
    goto error_label;
  }

  dprintf(LOG_LEVEL1,"username: %s\n", szAuthusername);
  dprintf(LOG_LEVEL1,"passwd: %s\n", szAuthPasswd);

  //add for cwmp syn with localmanager
  FILE *fp = NULL;
  int fd_conf = 0;
  if ( NULL == (fp=fopen(LOCK_FILE, "r")) )
  {
    return NULL;
  }
  
  fd_conf = fileno( fp );
  if( flock( fd_conf, LOCK_EX ) != 0 )
  {
    fclose( fp );
    return NULL;
  }

  flock( fd_conf, LOCK_UN );
  fclose(fp);
  remove(LOCK_FILE);

  // Create listening socket
  soap_init(&soap_listen);
  soap_listen.send_timeout = 60; // 60 seconds 
  soap_listen.recv_timeout = 60; // 60 seconds
  m = soap_bind(&soap_listen, NULL, nConfPortNumber, BACKLOG);
  if (!soap_valid_socket(m))
          exit(1);
  dprintf(LOG_LEVEL3, "..listening socket connection successful %d on port %d\n", m, nConfPortNumber);

  dprintf(LOG_LEVEL3, "..before pthread_create\n");
  // server thread creation (wait for request, response from datamodel repository)
  soap_register_plugin(&soap_listen, http_da);
  if (pthread_create(&tid, NULL, (void*(*)(void*))cwmp_server_thread, (void*)&soap_listen) != 0)
    goto error_label; // listening thread creation error

// ATTENTION : Don't use SOAP_IO_KEEPALIVE during SUBSCRIBE message
// because each SUBSCRIBE must correspond to one TCP connection
  InitSoapStruct(&soap, empty_namespaces);

//SUBSCRIBE SEQUENCE (BEGIN)
  dprintf(LOG_LEVEL3, "main process subscribe begin:\n");
  dprintf(LOG_LEVEL1, "..SUBSCRIPTION to Datamodel repository\n");
  dprintf(LOG_LEVEL3, "gdatamodel_address : %s\n", gdatamodel_address);
  if (SubscribeSequence(&soap, gdatamodel_address, nConfPortNumber) != 0)
  {
    dprintf(LOG_LEVEL1, "Subscribe error !\n");
    goto error_label;
  }
  dprintf(LOG_LEVEL3, "\ngdatamodel_address end!");
//SUBSCRIBE SEQUENCE (END)

  // wait for response message (semaphore)
  do {
    if ((ret_sem_wait=sem_wait(&S_response)) != 0)  // SEMAPHORE lock
        dprintf(LOG_LEVEL1, "ERROR on sem_wait !! \n");
  }
  while ((ret_sem_wait != 0) && (errno == EINTR));

//PERIODIC INFORM ENABLE (BEGIN)
  dprintf(LOG_LEVEL1, "..request PERIODIC INFORM ENABLE values to Datamodel repository\n");
  dprintf(LOG_LEVEL3, "gdatamodel_address : %s\n", gdatamodel_address);
  if (CreateRequest(gdatamodel_address, gId, CONST_FUNCTIONGETVALUE, "InternetGatewayDevice.ManagementServer.PeriodicInformEnable", NULL) != 0)
  {
    dprintf(LOG_LEVEL1, "CreateRequest error !\n");
    goto error_label;
  }

  // wait for response message (semaphore)
  do {
    if ((ret_sem_wait=sem_wait(&S_response)) != 0)  // SEMAPHORE lock
        dprintf(LOG_LEVEL1, "ERROR on sem_wait !! \n");
  }
  while ((ret_sem_wait != 0) && (errno == EINTR));

  if ((gflagResponse_OK == 1) && (gListParamValues != NULL))
  {
    bPeriodicEnable = atoi(gListParamValues->szValue);
    dprintf(LOG_LEVEL1, "bPeriodicEnable: %d !\n", bPeriodicEnable);
  }
  dprintf(LOG_LEVEL2, "worker_thread: enable end!\n");
//PERIODIC INFORM ENABLE (END)

//PERIODIC INFORM INTERVAL (BEGIN)
  dprintf(LOG_LEVEL1, "..request PERIODIC INFORM INTERVAL values to Datamodel repository\n");
  dprintf(LOG_LEVEL3, "gdatamodel_address : %s\n", gdatamodel_address);
  if (CreateRequest(gdatamodel_address, gId, CONST_FUNCTIONGETVALUE, "InternetGatewayDevice.ManagementServer.PeriodicInformInterval", NULL) != 0)
  {
    dprintf(LOG_LEVEL1, "CreateRequest error !\n");
    goto error_label;
  }

  // wait for response message (semaphore)
  dprintf(LOG_LEVEL2, "worker_thread: go into interval\n");
  do {
    if ((ret_sem_wait=sem_wait(&S_response)) != 0)  // SEMAPHORE lock
        dprintf(LOG_LEVEL1, "ERROR on sem_wait !! \n");
  }
  while ((ret_sem_wait != 0) && (errno == EINTR));

  if ((gflagResponse_OK == 1) && (gListParamValues != NULL))
  {
    nPeriodicTime = atoi(gListParamValues->szValue);
    dprintf(LOG_LEVEL1, "nPeriodicTime: %d !\n", nPeriodicTime);
  }
  dprintf(LOG_LEVEL2, "worker_thread: interval end!\n");
//PERIODIC INFORM VALUES (END)

// CONTEXT FOR CONNECTION TO ACS
  soap_destroy(&soap);  // dealloc data
  soap_end(&soap);      // dealloc data and cleanup
  soap_done(&soap);     // detach soap struct

  setTransferComplete(0);

/*// FB >>> TEMP
  setTransferComplete(1);
  setCommandKey("36");
// FB <<< TEMP*/
  int monitorFlag = 0;
  int nMonitorTime = 60;
  char str_montmp[8];
  int timeup = 0;
  FILE *f_monitor;

  dprintf(LOG_LEVEL1, "\n\nCWMP process...... MAIN LOOP\n");
  do
  {
    soap_init2(&soap,SOAP_IO_KEEPALIVE | SOAP_XML_INDENT,SOAP_IO_KEEPALIVE | SOAP_XML_INDENT);
    if( ssl_flag )
    {
        soap_ssl_init(); 
        //use SOAP_SSL_NO_AUTHENTICATION for test,we should use SOAP_SSL_REQUIRE_SERVER_AUTHENTICATION
        //to authenticate server
        if (soap_ssl_client_context( &soap, 
                                     SOAP_SSL_NO_AUTHENTICATION, 
                                     NULL, 
                                     NULL,
                                     NULL, 
                                     NULL,
                                     NULL )) 
        { 
            goto error_label;
        }
    }
    soap_register_plugin(&soap, http_da);
    soap_set_namespaces( &soap, Stub_namespaces);
    soap.encodingStyle = NULL;
  
  // SOAP Header
    header.cwmp__ID = "1"; 
    header.cwmp__NoMoreRequests = "1"; 
    soap.header = &header; // bind the SOAP Header for transport 
    
  // parameters for Basic HTTP Authent
    soap.userid = szAuthusername;
    soap.passwd = szAuthPasswd;
  
  // parameters if proxy
    if (strcmp(soap_proxy_host, "") != 0)
    {
      soap.proxy_host = soap_proxy_host;
      soap.proxy_port = soap_proxy_port;
      soap.proxy_userid = soap_proxy_userid;
      soap.proxy_passwd = soap_proxy_passwd;
    };
  
  // timeout parameters
    soap.send_timeout = 60; 
    soap.recv_timeout = 60;
    soap.accept_timeout = 60;
    soap.connect_timeout = 60;
	FILE *f_flag;
	int pingFlag = 0;
	int alarmFlag = 0;
    int ipchangeFlag = 0;
	memset(str_montmp, 0x00, 8);
	monitorFlag = 0;
    //ip change flag
    if(f_flag = fopen("/tmp/ipchange-flag", "r"))
    {
        bInformToDo = 1;
        ipchangeFlag = 1;
        fclose(f_flag);
        system("rm -rf /tmp/ipchange-flag");
    }
	//ping flag
	if(f_flag = fopen("/jffs/pingflag","r"))
	{
		bInformToDo = 1;
		pingFlag = 1;
		fclose(f_flag);
		system("rm -rf /jffs/pingflag");
	}
	//alarm flag  update by wangjr 091124
	 else if(f_flag = fopen("/jffs/alarmflag","r"))  //if(f_flag = fopen("/jffs/alarmflag","r"))//modify by wangjr 090527
	{
		alarmFlag = 1;
		fclose(f_flag);
		//system("rm -f /jffs/alarmflag");
	}
	//monitor flag
	else if(f_monitor = fopen("/jffs/monflag","r"))
	{
		//bInformToDo = 1;
		monitorFlag = 1;
		fclose(f_monitor);
		if(f_monitor = fopen("/jffs/mon/montime1", "r"))
		{
			fgets(str_montmp, 8, f_monitor);
			nMonitorTime = atoi(str_montmp)*60;
			fclose(f_monitor);
		}
		else
		{
			nMonitorTime = 30;
		}
	}
	
    if ( getTransferComplete()||getActiveNotification() || bInformToDo)  //update by wangjr 091210
    {
      bInformToDo = 0;
	  if(bPeriodicEnable)
      	tLastInform_period = time(NULL);//only for period
      if(monitorFlag)
	  	tLastInform_monitor = time(NULL);//only for monitor
	  if(alarmFlag)  //add by wangjr update 091124
	  	tLastInform_alarm= time(NULL);//only for alarm
      tLastInform=time(NULL);
      setActiveNotification(0);

      szFormatLocalTime(tLastInform_period, szLocalDate);
      dprintf(LOG_LEVEL1, "%s, new Inform \n", szLocalDate);

reInform:

      // Send Get Passive parameters to Data model repository before connection to ACS
      if(monitorFlag && timeup)
      	{
	  		CreateGetPassiveParametersRequest(gdatamodel_address, gId, CONST_FUNCTIONGETMONITORPARAMETERS);
      	}
	  else if(alarmFlag)
	  	{
	  		CreateGetPassiveParametersRequest(gdatamodel_address, gId, CONST_FUNCTIONGETIPCHANGEPARAMETERS);
	  	}
      else if(ipchangeFlag)
      {
            CreateRequest(gdatamodel_address, gId, CONST_FUNCTIONGETVALUE, "InternetGatewayDevice.ManagementServer.ConnectionRequestURL", 
            NULL);
            //CreateGetPassiveParametersRequest(gdatamodel_address, gId, );     
      }
	  else if(getActiveNotification())  //add by wangjr 091124
	  {

		CreateGetActiveParametersRequest(gdatamodel_address, gId, CONST_FUNCTIONGETACTIVEPARAMETERS);		
		
	  }
	  else
      	CreateGetPassiveParametersRequest(gdatamodel_address, gId, CONST_FUNCTIONGETPASSIVEPARAMETERS);
  
      do {
        if ((ret_sem_wait=sem_wait(&S_response)) != 0)  // SEMAPHORE lock
            dprintf(LOG_LEVEL1, "ERROR on sem_wait !! \n");
      }
      while ((ret_sem_wait != 0) && (errno == EINTR));
      if(ipchangeFlag)
      {
          LIST_PARAM_VALUE * listParamValues = (LIST_PARAM_VALUE *)malloc(sizeof(LIST_PARAM_VALUE));
          strcpy(listParamValues->szParameter, "InternetGatewayDevice.ManagementServer.ConnectionRequestURL");
          strcpy(listParamValues->szValue, gListParamValues->szValue);  
          CreateRequest(gdatamodel_address, gId, CONST_FUNCTIONGETVALUE, "InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.IPInterface.1.IPInterfaceIPAddress", 
                        NULL);
          do {
                if ((ret_sem_wait=sem_wait(&S_response)) != 0)  // SEMAPHORE lock
                        dprintf(LOG_LEVEL1, "ERROR on sem_wait !! \n");
          }while ((ret_sem_wait != 0) && (errno == EINTR));
          strcpy(gListParamValues->szParameter, "InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.IPInterface.1.IPInterfaceIPAddress");
          add_in_queue(gListParamValues, listParamValues);
      }
//reInform: 
      // Create Inform Request (using parameter list received from data model)
      dprintf(LOG_LEVEL1, "..call_InformRequest.\n");
	  
      if( 1 == digest_flag )
      {
          http_da_restore(&soap, &info);
      }
      if (bFirstTime) ptrEventCode = EventCode[1];//add event 6
      else if(pingFlag)
      	{
	  		ptrEventCode = EventCode[6];
			pingFlag = 0;
      	}
	  else if(alarmFlag)
	  	{
	  		ptrEventCode = EventCode[8];
			alarmFlag = 0;
	  	}
	  else if(monitorFlag && timeup)
	  	{
	  		ptrEventCode = EventCode[7];
			//timeup = 0;
	  	}
	 else if(getActiveNotification() || ipchangeFlag)  //add by wangjr 091124
	 {
	  		ptrEventCode = EventCode[3];
            ipchangeFlag = 0;
	 }
	  else
	  	ptrEventCode = EventCode[2];
/*  update TransferComplete  by wangjr 091124 */
      if (getTransferComplete() && (gfirmupgrade==0))
	  {
	  	ptrEventCode = EventCode[5];
      }
	   if (getTransferComplete() && (gfirmupgrade==1))
	  {
		ptrEventCode = "BOOT/TRANSFERCOMPLET";
	  }
	  if(bReboot)
	  {
	    ptrEventCode = "1 BOOT/M Reboot";
		greboot=1;
		bReboot=0;
	  }
	  if(bFactoryReset)
	  {
	  	ptrEventCode = "BOOTSTRAT";
	    gfactory=1;
		bFactoryReset=0;
	  }
/*  update TransferComplete  by wangjr 091124 */

      getCommandKey(szEventCommandKey);
      /*
      gListParamValues = (LIST_PARAM_VALUE *)malloc(sizeof(LIST_PARAM_VALUE));
      memset(gListParamValues , 0, sizeof(LIST_PARAM_VALUE));
      strcpy(gListParamValues->szParameter, "InternetGatewayDevice.ManagementServer.URL");
      strcpy(gListParamValues->szValue, "http://127.0.0.1:80");
      gListParamValues->next = (LIST_PARAM_VALUE *)malloc(sizeof(LIST_PARAM_VALUE));
      strcpy(gListParamValues->next->szParameter, "InternetGatewayDevice.ManagementServer.ConnectionRequestURL");
      strcpy(gListParamValues->next->szValue, "http://127.0.0.1:8060");
      gListParamValues->next->next = NULL;
      soap.header = NULL;*/
//      soap->error = 401;
//      soap_send_fault(&soap);
//	  return 0;
///////////////////////////////////update by wangjr  091124 ////////////////////////////////////////////////////////////////////////////////////////////////////////
	  LIST_PARAM_VALUE *iter_paramlist;
	  LIST_PARAM_VALUE *tListParamValues;
	  LIST_PARAM_VALUE *elt;
	  int i=0;
	  FILE *fp;
	  char szTemp[MAX_PARAMETER];
	  char szMonitorTime[5]={0};
	  char szAlarmTime[5]={0};
	  int count=0;
	  int monitortimer=0;
	  int alarmtimer=0;
	  char szTempMax[MAX_PARAMETER];
	  char szTempMin[MAX_PARAMETER];
	  char szAlarmMax[MAX_PARAMETER];
	  char szAlarmMin[MAX_PARAMETER];
	   
		  
	  ////////////////////////wangjr////////////////////////////////////////////

	  if (tListParamValues != NULL)
	  {
		free_queue(tListParamValues);
		tListParamValues = NULL;
	  }

	  if ((!bFirstTime) && (strncmp(ptrEventCode,EventCode[7],strlen(EventCode[7]))==0))
	  {
	  mon_timer++;
	  if(fp = popen("ls -l /jffs/mon/ | grep -c monpara","r"))
      {
  		fgets(szTemp, MAX_PARAMETER, fp);
		count = atoi(szTemp);
  		pclose(fp);
  	  }
  
	  dprintf(LOG_LEVEL3,"\n--------------------mon_timer=%d------------------------------\n",mon_timer);
	  for(i=1; i<=count; i++)
	  {
		sprintf(szTemp, "/jffs/mon/montime%d", i);
		if(fp=fopen(szTemp, "r"))
		{
			fgets(szMonitorTime, MAX_PARAMETER, fp);
			szMonitorTime[strlen(szMonitorTime)-1] = '\0';	
			fclose(fp); 
		}
		monitortimer=atoi(szMonitorTime);
		dprintf(LOG_LEVEL3,"\n monitorparatimer=%d\n",monitortimer);
		
		iter_paramlist=gListParamValues;
		
		int j=0;
		if(i==1)
		{
			;;   
		}
		else
		{
			for(j=1;j<i;j++)
			{
			   iter_paramlist=iter_paramlist->next;
			}
		}
		
		if((monitortimer !=0)&&((mon_timer%monitortimer)!=0))
		{

			;;
		}
		else
		{		
				if (tListParamValues == NULL)
				{
					tListParamValues = (LIST_PARAM_VALUE *)malloc(sizeof(LIST_PARAM_VALUE));
					memset(tListParamValues, 0x00, sizeof(LIST_PARAM_VALUE));
					strcpy(tListParamValues->szParameter, iter_paramlist->szParameter);
					strcpy(tListParamValues->szValue, iter_paramlist->szValue);
				}
				else
				{
					elt = (LIST_PARAM_VALUE *)malloc(sizeof(LIST_PARAM_VALUE));
					strcpy(elt->szParameter, iter_paramlist->szParameter);
					strcpy(elt->szValue,iter_paramlist->szValue);
					add_in_queue(tListParamValues, elt);
				}	
		}
		
	  }
	 // nError = call_InformRequest(&soap, ACS_address, tListParamValues, gListActiveNotification, ptrEventCode, szEventCommandKey); 
	  }
	  if ((!bFirstTime) && (strncmp(ptrEventCode,EventCode[8],strlen(EventCode[8]))==0))
	  {
		  alarm_timer++;
		  if(fp = popen("ls -l /jffs/alarm/ | grep -c alarmpara","r"))
	      {
	  		fgets(szTemp, MAX_PARAMETER, fp);
			count = atoi(szTemp);
	  		pclose(fp);
	  	  }
  
	  	  dprintf(LOG_LEVEL3,"\n--------------------alarm_timer=%d------------------------------\n",alarm_timer);

		  for(i=1; i<=count; i++)
		  {
			sprintf(szTemp, "/jffs/alarm/alarmtime%d", i);
			if(fp=fopen(szTemp, "r"))
			{
				fgets(szAlarmTime, MAX_PARAMETER, fp);
				szAlarmTime[strlen(szAlarmTime)-1] = '\0';	
				fclose(fp); 
			}	
			alarmtimer=atoi(szAlarmTime);
			dprintf(LOG_LEVEL3,"\n alarmparatimer=%d\n",alarmtimer);


				  
			sprintf(szTempMax, "/jffs/alarm/alarmmax%d", i);
			if(fp=fopen(szTempMax, "r"))
			{
				fgets(szAlarmMax, MAX_PARAMETER, fp);
				szAlarmMax[strlen(szAlarmMax)-1] = '\0';
				fclose(fp);
			}
			sprintf(szTempMin, "/jffs/alarm/alarmmin%d", i);
			if(fp=fopen(szTempMin, "r"))
			{
				fgets(szAlarmMin, MAX_PARAMETER, fp);
				szAlarmMin[strlen(szAlarmMin)-1] = '\0';
				fclose(fp);
			}
			
			iter_paramlist=gListParamValues;
			
			int j=0;
			if(i==1)
			{
				;;   
			}
			else
			{
				for(j=1;j<i;j++)
				{
				   iter_paramlist=iter_paramlist->next;
				}
			}
			
			if((alarmtimer!=0) && ((alarm_timer%alarmtimer)!=0) )
			{
				;;			 
			}
			else
			{

			   if((atoi(iter_paramlist->szValue)>atoi(szAlarmMax)) || (atoi(iter_paramlist->szValue)<atoi(szAlarmMin)))
		       {
					if (tListParamValues == NULL)
					{
						tListParamValues = (LIST_PARAM_VALUE *)malloc(sizeof(LIST_PARAM_VALUE));
						memset(tListParamValues, 0x00, sizeof(LIST_PARAM_VALUE));
						strcpy(tListParamValues->szParameter, iter_paramlist->szParameter);
						strcpy(tListParamValues->szValue, iter_paramlist->szValue);
						gAlarm=1;
					}
					else
					{
						elt = (LIST_PARAM_VALUE *)malloc(sizeof(LIST_PARAM_VALUE));
						strcpy(elt->szParameter, iter_paramlist->szParameter);
						strcpy(elt->szValue,iter_paramlist->szValue);
						add_in_queue(tListParamValues, elt);
					}
		       	}
			}
			
		  }
		  if((gAlarm==1)&&(tListParamValues == NULL))
		  {
		      ptrEventCode = EventCode[9];
		  }		  
	  }

			if((alarmFlag == 0) && (monitorFlag == 0) && (!getActiveNotification()))
			{
			   	 if (gListPassiveListParamValues != NULL)
				 {
				    free_queue(gListPassiveListParamValues);
					gListPassiveListParamValues = NULL;
				 }
				iter_paramlist=gListParamValues;
			
				while(iter_paramlist != NULL)
				{
					if (gListPassiveListParamValues == NULL)
					{

						gListPassiveListParamValues = (LIST_PARAM_VALUE *)malloc(sizeof(LIST_PARAM_VALUE));
						memset(gListPassiveListParamValues, 0x00, sizeof(LIST_PARAM_VALUE));				
						strcpy(gListPassiveListParamValues->szParameter, iter_paramlist->szParameter);
						strcpy(gListPassiveListParamValues->szValue, iter_paramlist->szValue);
					}
					else
					{
						elt = (LIST_PARAM_VALUE *)malloc(sizeof(LIST_PARAM_VALUE));
						strcpy(elt->szParameter, iter_paramlist->szParameter);
						strcpy(elt->szValue,iter_paramlist->szValue);
						add_in_queue(gListPassiveListParamValues, elt);
					}	
					iter_paramlist=iter_paramlist->next;
				}
			}

			if(getActiveNotification())
			{		
			   int gactivenum=0;
			
				iter_paramlist=gListParamValues;

			    while(iter_paramlist != NULL)
			    {
			        for(gactivenum =0 ;gactivenum <=(sizeof(gActiveParameter)/sizeof(gActiveParameter[0]));gactivenum++)
			        {  
			             if(gActiveParameter[gactivenum][0] != '\0' )
			             {

							 if(strncmp(iter_paramlist->szParameter,gActiveParameter[gactivenum],strlen(gActiveParameter[gactivenum])) == 0)
							{
									 if (tListParamValues == NULL)
									 {
										 tListParamValues = (LIST_PARAM_VALUE *)malloc(sizeof(LIST_PARAM_VALUE));
										 memset(tListParamValues, 0x00, sizeof(LIST_PARAM_VALUE));
										 strcpy(tListParamValues->szParameter, iter_paramlist->szParameter);
										 strcpy(tListParamValues->szValue, iter_paramlist->szValue); 
									 }
									 else
									 {
										 elt = (LIST_PARAM_VALUE *)malloc(sizeof(LIST_PARAM_VALUE));
										 strcpy(elt->szParameter, iter_paramlist->szParameter);
										 strcpy(elt->szValue,iter_paramlist->szValue);
										 add_in_queue(tListParamValues, elt);						 
									 }
							}
						 }
			        }	
					iter_paramlist=iter_paramlist->next;
			    }
			} 
			//ACTIVE INFORM ENABLE (END)	
	  ///////////////////////////////wangjr/////////////////////////////////////////////////

     if((alarmFlag == 1)||(monitorFlag == 1) || getActiveNotification())
     {
     	setActiveNotification(0);
		nError = call_InformRequest(&soap, ACS_address, gListPassiveListParamValues, tListParamValues,ptrEventCode, szEventCommandKey); 

	 }
	 else
	 {
	   nError = call_InformRequest(&soap, ACS_address,gListPassiveListParamValues,  tListParamValues, ptrEventCode, szEventCommandKey);     
	 }
/////////////////////update by wangjr 091124//////////////////////////////////////////////
      dprintf(LOG_LEVEL2, "---> return code : %d\n", soap.error);

      if (soap.error != SOAP_OK) goto error_label;
  
      dprintf(LOG_LEVEL2, "---> call to server OK\n");
  
      dprintf(LOG_LEVEL1, "..wait_InformResponse.\n");
      nError = wait_InformResponse(&soap, &InformResponse);
      dprintf(LOG_LEVEL1, "-> return code : %d\n", nError);

      if (soap.error != SOAP_OK)
      {
          if( 401 == soap.error )
          {
              printf("soap error 401\n");
              
              if ((szAuthrealm != NULL && !strcmp(soap.authrealm, szAuthrealm))
                || szAuthrealm[0] == '\0' && strcpy(szAuthrealm, soap.authrealm))
              {
                  digest_flag = 1;
                  http_da_save(&soap, &info, szAuthrealm, szAuthusername, szAuthPasswd); 
                  goto reInform;
              }
          }
          goto error_label;
      }        

      if (getTransferComplete())
      {
        // FaultCode <-- 0 pour l'instant
        // timeStart <-- current pour l'instant
		FILE *tmpfile;  //add by wangjr 091201
		FILE   *stream;  //add by wangjr 091203
		char tmpbuf[10]={0};  //add by wangjr 09121
		char firmbuf[100]={0};//add by wangjr 091203
		char firmfaultbuf[100]={0};//add by wangjr 091203
        dprintf(LOG_LEVEL1, "....call_TransferComplete.\n");
        getCommandKey(szEventCommandKey);
/*        add by wangjr 091201   */

		tmpfile = fopen("/tmp/tr069", "r");
		if(tmpfile != NULL)
		{
		   
		   fread( tmpbuf, sizeof( char ), sizeof(tmpbuf), tmpfile );
		}
		fclose(tmpfile);
/*        add by wangjr 091201   */

        ///getTR069ErrorString(getFaultCode(), &szFaultString); //update by wangjr

		/*		  add by wangjr 091203	 */

		if(atoi(tmpbuf) == 0 && gfirmupgrade == 1)
		{
            sprintf(firmbuf ,"{ /usr/sbin/checkimg  %s 1>/dev/null 2>&1; echo $? ; }" ,gupgrade_filename);

			dprintf(LOG_LEVEL3, "firmbuf: %s\n", firmbuf);
			
			stream = popen( firmbuf, "r" );
			fread( firmfaultbuf, sizeof(char), sizeof(firmfaultbuf),stream); 
			pclose( stream );
			firmfaultbuf[strlen(firmfaultbuf)-1]='\0';
			
			dprintf(LOG_LEVEL3, "firmfaultbuf: %s\n", firmfaultbuf);
			
			if(1==atoi(firmfaultbuf))
			{
				memset(tmpbuf, 0x00, sizeof(tmpbuf)); 
				strcpy(tmpbuf,"9019");
			}				
		}
		if(atoi(tmpbuf) == 0 && gupdateconfig == 1)
		{
            sprintf(firmbuf ,"/usr/sbin/config_check  %s" ,gconfig_filename);

			dprintf(LOG_LEVEL3, "firmbuf: %s\n", firmbuf);
			
			stream = popen( firmbuf, "r" );
			fread( firmfaultbuf, sizeof(char), sizeof(firmfaultbuf),stream); 
			pclose( stream );
			firmfaultbuf[strlen(firmfaultbuf)-1]='\0';
			
			dprintf(LOG_LEVEL3, "firmfaultbuf: %s\n", firmfaultbuf);
			
			if(strncmp(firmfaultbuf,"fault",strlen("fault") )== 0)
			{
				memset(tmpbuf, 0x00, sizeof(tmpbuf)); 
				strcpy(tmpbuf,"9019");
			}				
		}
		getTR069ErrorString(atoi(tmpbuf), &szFaultString);
		dprintf(LOG_LEVEL3, "szFaultCode: %d\n", atoi(tmpbuf));
        dprintf(LOG_LEVEL3, "szFaultString: %s\n", szFaultString);

      //  nError = call_TransferComplete(&soap, ACS_address, szEventCommandKey, getFaultCode(), szFaultString, time(NULL));

	  
	  nError = call_TransferComplete(&soap, ACS_address, szEventCommandKey, atoi(tmpbuf), szFaultString, time(NULL));

        if (szFaultString != NULL) free(szFaultString);

        dprintf(LOG_LEVEL3, "-> return code : %d\n", soap.error);
        if (soap.error != SOAP_OK) goto error_label;

        /*dprintf(LOG_LEVEL1, "....wait_TransferCompleteResponse.\n");
        wait_TransferCompleteResponse(&soap);
        dprintf(LOG_LEVEL1, "-> return code : %d\n", nError);
        if (soap.error != SOAP_OK) goto error_label;*/
      }
      else
      {
        dprintf(LOG_LEVEL1, "....send HTTP POST empty.\n"); 
        if(digest_flag)
            http_da_restore(&soap, &info);
        soap_serializeheader(&soap);
        soap_begin_count(&soap);
        soap_end_count(&soap);
        soap_connect_command(&soap, SOAP_POST, ACS_address, NULL);
        soap_end_send(&soap);
      }

      // Init Transfer complete boolean after inform request following a download request
      if (getTransferComplete())
      {
        setTransferComplete(0);
        setCommandKey("");
		gDownloadResponse_status = 0;  //add by wangjr 091124
		gUploadResponse_status = 0;  //add by wangjr 091124
      }

      bContinue = 1;
	  dprintf(LOG_LEVEL1,"bcontinue ...\n");
      while (bContinue && (nError == 0))
      {
	  	if(getTransferComplete())  //add by wangjr 091209
		          break;
	  	if(getActiveNotification())  //add by wangjr 091124 
		          break;
		
        nError = wait_RPCRequest(&soap);
        dprintf(LOG_LEVEL3, "-> return code : %d\n", nError);
        if( 1 == digest_flag )
        {
            http_da_restore(&soap, &info);
        }
        switch(nError)
        {
		  case RPC_GETPARAMETERNAMES:  //add by wangjr 091124
		  	dprintf(LOG_LEVEL1, "....GetParameterNames Request.\n");
			nError = soap_wait_cwmp__GetParameterNames(&soap, &tmp_cwmp__GetParameterNamesRequest);
			dprintf(LOG_LEVEL1, "return code : %d\n", nError);

			dprintf(LOG_LEVEL3, "....ParseGetParameterNames_and_SendReponse.\n");
			nError = ParseGetParameterNames_and_SendResponse(&soap, ACS_address, tmp_cwmp__GetParameterNamesRequest);
		  	break;
		  case RPC_GETMETHODS://add by wangjr 091202
		  	dprintf(LOG_LEVEL1, "....GetRPCMethods Request.\n");
			nError = soap_wait_cwmp__GetRPCMethods(&soap, &tmp_cwmp__GetRPCMethods);
			dprintf(LOG_LEVEL1, " return code : %d\n", nError);
			dprintf(LOG_LEVEL3, "ParseGetRPCMethods_and_SendReponse.\n");
			nError = ParseGetRPCMethods_and_SendResponse(&soap, ACS_address, tmp_cwmp__GetRPCMethods);
		  	break;
          case RPC_GETPARAMETERVALUES:
            dprintf(LOG_LEVEL1, "....GetParameterValues Request.\n");
            nError = soap_wait_cwmp__GetParameterValues(&soap, &tmp_cwmp__GetParameterValuesRequest);
            dprintf(LOG_LEVEL1, "-> return code : %d\n", nError);
  
            dprintf(LOG_LEVEL3, "....ParseGetParamValues_and_SendReponse.\n");
            nError = ParseGetParamValues_and_SendResponse(&soap, ACS_address, tmp_cwmp__GetParameterValuesRequest);
            break;
  
          case RPC_REBOOT:
            dprintf(LOG_LEVEL1, "....Reboot Request.\n");
            nError = soap_wait_cwmp__Reboot(&soap, &tmp_cwmp__RebootRequest);
            dprintf(LOG_LEVEL1, "-> return code : %d\n", nError);
  
            dprintf(LOG_LEVEL3, "....ParseReboot_and_SendReponse.\n");
            nError = ParseReboot_and_SendResponse(&soap, ACS_address, tmp_cwmp__RebootRequest);
  
            bReboot = 1;
            break;
 /*         case RPC_FACTORYRESET:
		  	#if 0
		  	dprintf(LOG_LEVEL1, "....FactoryReset Request.\n");
            nError = soap_wait_cwmp__FactoryReset(&soap, &tmp_cwmp__FactroyResetRequest);			
			dprintf(LOG_LEVEL1, "-> return code : %d\n", nError);
			nError = ParseFactoryReset_and_SendResponse(&soap, ACS_address, tmp_cwmp__FactroyResetRequest);
			dprintf(LOG_LEVEL3, "....FactoryReset_and_SendReponse.\n"); 
            #endif 
			bFactoryReset=1;
			break;*/
          case RPC_SETPARAMETERVALUES:
            dprintf(LOG_LEVEL1, "....SetParameterValues Request.\n");
            nError = soap_wait_cwmp__SetParameterValues(&soap, &tmp_cwmp__SetParameterValuesRequest);
            dprintf(LOG_LEVEL1, "-> return code : %d\n", nError);
            dprintf(LOG_LEVEL3, "....ParseSetParamValues_and_SendReponse.\n");
            status = 0;
            nError = ParseSetParamValues_and_SendResponse(&soap, ACS_address, tmp_cwmp__SetParameterValuesRequest, &status);
            break;
		  case RPC_UPLOAD:
			dprintf(LOG_LEVEL1, "....Upload Request.\n");
            nError = soap_wait_cwmp__Upload(&soap, &tmp_cwmp__UploadRequest);
            dprintf(LOG_LEVEL1, "***************soap_wait_cwmp__Upload-> return code : %d\n", nError);
			dprintf(LOG_LEVEL3, "....ParseUpload_and_SendResponse.\n");
            nError = ParseUpload_and_SendResponse(&soap, ACS_address, tmp_cwmp__UploadRequest);
			if (tmp_cwmp__UploadRequest.CommandKey != NULL)
			{
				uploadargs.szCmdKey = (char*)malloc(strlen(tmp_cwmp__UploadRequest.CommandKey) + 1);
				strcpy(uploadargs.szCmdKey, tmp_cwmp__UploadRequest.CommandKey);
			 }
			
			 uploadargs.szCWMPAddress = (char*)malloc(20);
			 sprintf(uploadargs.szCWMPAddress, "192.168.8.10:%d", nConfPortNumber);
			
			 if (tmp_cwmp__UploadRequest.URL != NULL)
			 {
			     uploadargs.szURL = (char*)malloc(strlen(tmp_cwmp__UploadRequest.URL)+1);
				 strcpy(uploadargs.szURL, tmp_cwmp__UploadRequest.URL);
			 }
			 if (tmp_cwmp__UploadRequest.FileType) 
			 {
			    uploadargs.szFileType = (char*)malloc(strlen(tmp_cwmp__UploadRequest.FileType)+1);
				strcpy(uploadargs.szFileType, tmp_cwmp__UploadRequest.FileType);
			 }
			 if (tmp_cwmp__UploadRequest.Username != NULL) 
			 {
			      uploadargs.szUsername = (char*)malloc(strlen(tmp_cwmp__UploadRequest.Username)+1);
				  strcpy(uploadargs.szUsername, tmp_cwmp__UploadRequest.Username);
			 }
			 if (tmp_cwmp__UploadRequest.Password != NULL) 
			 {
			     uploadargs.szPassword = (char*)malloc(strlen(tmp_cwmp__UploadRequest.Password)+1);
				 strcpy(uploadargs.szPassword, tmp_cwmp__UploadRequest.Password);
			 }
			 uploadargs.FileSize = tmp_cwmp__UploadRequest.FileSize;
						
			 if (tmp_cwmp__UploadRequest.TargetFileName != NULL) 
			 {
			    uploadargs.szTargetFileName = (char*)malloc(strlen(tmp_cwmp__UploadRequest.TargetFileName)+1);
				strcpy(uploadargs.szTargetFileName, tmp_cwmp__UploadRequest.TargetFileName);
			}
			uploadargs.DelaySeconds = tmp_cwmp__UploadRequest.DelaySeconds;
            if (pthread_create(&tUpload, NULL, (void*(*)(void*))cwmp_upload_thread, (void*)&uploadargs) != 0)
            {
              dprintf(LOG_LEVEL1, "....pthread_create cwmp_upload_thread error\n");
            }
			else
            {
              // wait for end of thread execution
              (void) pthread_join(tUpload, NULL);
			  bContinue = 0;
              //goto transfercomplete_label;
             }
            break;
          case RPC_DOWNLOAD:
            dprintf(LOG_LEVEL1, "....Download Request.\n");
            nError = soap_wait_cwmp__Download(&soap, &tmp_cwmp__DownloadRequest);
            dprintf(LOG_LEVEL1, "-> return code : %d\n", nError);
  
            dprintf(LOG_LEVEL3, "....ParseDownload_and_SendResponse.\n");
            status = 0;
            nError = ParseDownload_and_SendResponse(&soap, ACS_address, tmp_cwmp__DownloadRequest);

            dprintf(LOG_LEVEL3, "....pthread_create cwmp_download_thread\n");
            // download thread creation (launch download manager process and wait for response)
            // create new string for command key parameter
            if (tmp_cwmp__DownloadRequest.CommandKey != NULL)
            {
              downloadargs.szCmdKey = (char*)malloc(strlen(tmp_cwmp__DownloadRequest.CommandKey) + 1);
              strcpy(downloadargs.szCmdKey, tmp_cwmp__DownloadRequest.CommandKey);
            }

            downloadargs.szCWMPAddress = (char*)malloc(20);
            sprintf(downloadargs.szCWMPAddress, "127.0.0.1:%d", nConfPortNumber);

            if (tmp_cwmp__DownloadRequest.URL != NULL) {
              downloadargs.szURL = (char*)malloc(strlen(tmp_cwmp__DownloadRequest.URL)+1);
              strcpy(downloadargs.szURL, tmp_cwmp__DownloadRequest.URL);
            }
            if (tmp_cwmp__DownloadRequest.FileType) {
              downloadargs.szFileType = (char*)malloc(strlen(tmp_cwmp__DownloadRequest.FileType)+1);
              strcpy(downloadargs.szFileType, tmp_cwmp__DownloadRequest.FileType);
            }
            if (tmp_cwmp__DownloadRequest.Username != NULL) {
              downloadargs.szUsername = (char*)malloc(strlen(tmp_cwmp__DownloadRequest.Username)+1);
              strcpy(downloadargs.szUsername, tmp_cwmp__DownloadRequest.Username);
            }
            if (tmp_cwmp__DownloadRequest.Password != NULL) {
              downloadargs.szPassword = (char*)malloc(strlen(tmp_cwmp__DownloadRequest.Password)+1);
              strcpy(downloadargs.szPassword, tmp_cwmp__DownloadRequest.Password);
            }
            downloadargs.FileSize = tmp_cwmp__DownloadRequest.FileSize;
            if (tmp_cwmp__DownloadRequest.TargetFileName != NULL) {
              downloadargs.szTargetFileName = (char*)malloc(strlen(tmp_cwmp__DownloadRequest.TargetFileName)+1);
              strcpy(downloadargs.szTargetFileName, tmp_cwmp__DownloadRequest.TargetFileName);
            }
            downloadargs.DelaySeconds = tmp_cwmp__DownloadRequest.DelaySeconds;

            if (strcmp(soap_proxy_host, "") != 0) {
              downloadargs.szProxyHost = (char*)malloc(strlen(soap_proxy_host)+1);
              strcpy(downloadargs.szProxyHost, soap_proxy_host);

              downloadargs.ProxyPort = soap_proxy_port;
            }

            if (strcmp(soap_proxy_userid, "") != 0) {
              downloadargs.szProxyUserId = (char*)malloc(strlen(soap_proxy_userid)+1);
              strcpy(downloadargs.szProxyUserId, soap_proxy_userid);
            }

            if (strcmp(soap_proxy_passwd, "") != 0) {
              downloadargs.szProxyPasswd = (char*)malloc(strlen(soap_proxy_passwd)+1);
              strcpy(downloadargs.szProxyPasswd, soap_proxy_passwd);
            }

            if (pthread_create(&tDownload, NULL, (void*(*)(void*))cwmp_download_thread, (void*)&downloadargs) != 0)
            {
              dprintf(LOG_LEVEL1, "....pthread_create cwmp_download_thread error\n");
            }
            else
            {
              // wait for end of thread execution
              (void) pthread_join(tDownload, NULL);
			  bContinue = 0;
              //goto transfercomplete_label;
             }
            break;
/* add by wangjr 091124 */
          case RPC_ADDOBJECT:
		  	dprintf(LOG_LEVEL1, "....AddObject Request.\n");
            nError = soap_wait_cwmp__AddObject(&soap, &tmp_cwmp__AddObjectRequest);			
			dprintf(LOG_LEVEL1, "-> return code : %d\n", nError);
			nError = ParseAddObject_and_SendResponse(&soap, ACS_address, tmp_cwmp__AddObjectRequest);
			dprintf(LOG_LEVEL3, "....AddObject_and_SendReponse.\n"); 
		  break;
		  case RPC_DELETEOBJECT:
  	  	    dprintf(LOG_LEVEL1, "....DeleteObject Request.\n");
            nError = soap_wait_cwmp__DeleteObject(&soap, &tmp_cwmp__DeleteObjectRequest);			
			dprintf(LOG_LEVEL1, "-> return code : %d\n", nError);
			nError = ParseDeleteObject_and_SendResponse(&soap, ACS_address, tmp_cwmp__DeleteObjectRequest);
			dprintf(LOG_LEVEL3, "....DeleteObject_and_SendReponse.\n"); 
		  break;
          case RPC_GETPARAMETERATTRIBUTES:
            dprintf(LOG_LEVEL1, "....ParseGetParameterAttributes Request.\n");
            nError = soap_wait_cwmp__GetParameterAttributes(&soap, &tmp_cwmp__GetParameterAttributesRequest);
            dprintf(LOG_LEVEL1, "-> return code : %d\n", nError);
            dprintf(LOG_LEVEL3, "....ParseGetParameterAttributes_and_SendReponse.\n");
            nError = ParseGetParameterAttributes_and_SendResponse(&soap, ACS_address, tmp_cwmp__GetParameterAttributesRequest);
            break;
		 case RPC_SETPARAMETERATTRIBUTES:
            dprintf(LOG_LEVEL1, "....ParseSetParameterAttributes Request.\n");
            nError = soap_wait_cwmp__SetParameterAttributes(&soap, &tmp_cwmp__SetParameterAttributesRequest);
            dprintf(LOG_LEVEL1, "-> return code : %d\n", nError);
            dprintf(LOG_LEVEL3, "....ParseSetParameterAttributes_and_SendReponse.\n");
            nError = ParseSetParameterAttributes_and_SendResponse(&soap, ACS_address, tmp_cwmp__SetParameterAttributesRequest);
            break;	
/* add by wangjr 091124 */

          case SOAP_STOP:
		  //case 204:
            dprintf(LOG_LEVEL1, "-> HTTP Empty (no more exchanges)\n");
            bContinue = 0;
            if (soap_end_recv(&soap)) return soap_closesock(&soap); 
            break;
        }
      }
    }

    // Reboot STB
    #if 0  //remove by wangjr 091124
    if (bReboot)
    {
      dprintf(LOG_LEVEL1, "\n.\n.\nReboot..\n\n");
      sleep(2);
      system("/sbin/reboot");
    }
	if(bFactoryReset)
	{
	  dprintf(LOG_LEVEL1, "\n.\n.\nFactoryReset..\n\n");
      sleep(2);
	  //system("rm /etc/config/config.xml");
	}
	#endif
/*            update by wangjr 091124       */
	if(bReboot || bFactoryReset)
	{
		bInformToDo=1;
	}
	if (greboot)
    {
		dprintf(LOG_LEVEL1, "\n.\n.\nReboot..\n\n");
		sleep(2);
		//bInformToDo=1;
		system("/sbin/reboot");
		greboot=0;
    }
	if(gfactory)
	{
		dprintf(LOG_LEVEL1, "\n.\n.\nFactoryReset..\n\n");
		sleep(2);
		system("rm /etc/config/config.xml && /sbin/sysreboot");	
		gfactory=0;
	}
	
	if(gfirmupgrade && (!getTransferComplete()))
	{
		gfirmupgrade=0;
	    char szUpgrade[100]={0};
		sleep(2);
	  	dprintf(LOG_LEVEL3,"*******************************************************\n");
	  	dprintf(LOG_LEVEL3,".......................upgrade..........................\n");
	  	dprintf(LOG_LEVEL3,"********************************************************\n");
	  	sprintf(szUpgrade," /usr/sbin/sysupgrade %s && /sbin/sysreboot",gupgrade_filename);
	  	system(szUpgrade);
	}
    if(gupdateconfig && (!getTransferComplete()))
	{
		gupdateconfig=0;
	    char szConfig[100]={0};
		char configbuf[100]={0};
		char configfaultbuf[50]={0};
		FILE *fp;

		sprintf(configbuf ,"/usr/sbin/config_check  %s" ,gconfig_filename);

			dprintf(LOG_LEVEL3, "configbuf: %s\n", configbuf);
			
			fp = popen( configbuf, "r" );
			fread( configfaultbuf, sizeof(char), sizeof(configfaultbuf),fp); 
			pclose( fp );
			configfaultbuf[strlen(configfaultbuf)-1]='\0';
			
			dprintf(LOG_LEVEL3, "configfaultbuf: %s\n", configfaultbuf);
			

		sleep(2);

		if(strncmp(configfaultbuf,"fault",strlen("fault") ) != 0 )
		{		
		  	dprintf(LOG_LEVEL3,"*******************************************************\n");
		  	dprintf(LOG_LEVEL3,".......................update config file..................\n");
		  	dprintf(LOG_LEVEL3,"********************************************************\n");
		  	sprintf(szConfig," /bin/cp  %s  /etc/config/config.xml && /sbin/sysreboot",gconfig_filename);

			dprintf(LOG_LEVEL3,"********************szConfig=%s***********************************\n",szConfig);
		  	system(szConfig);
		}
	}

/*            update by wangjr 091124       */
    
    http_da_release(&soap, &info);
    soap_destroy(&soap);  // dealloc data
    soap_end(&soap);      // dealloc data and cleanup
    soap_done(&soap);     // detach soap struct

    bFirstTime = 0;       // First execution = false
    if(monitorFlag|| alarmFlag)
    {
        bPeriodicEnable = 0;  //modify by wangjr
    }
    if (bPeriodicEnable)
    {
      dprintf(LOG_LEVEL3, "period difftime  %f\n", difftime(time(NULL), tLastInform_period));

      // calculate time since last Inform Request
      if (difftime(time(NULL), tLastInform_period) >= nPeriodicTime)
      {
        bInformToDo = 1;
      }
    }
	if (monitorFlag)
	{
		dprintf(LOG_LEVEL3, "monitor difftime  %f\n", difftime(time(NULL), tLastInform_monitor));
		
		// calculate time since last Inform Request
		if (difftime(time(NULL), tLastInform_monitor) >= nMonitorTime)
		{
		  bInformToDo = 1;
		 // timeup = 1; //modify by wangjr 090527
		}
		}
	dprintf(LOG_LEVEL3,"\n monitorFlag=%d,alarmFlag=%d\n",monitorFlag,alarmFlag);
	
	if(alarmFlag)  //add by wangjr 091124
	{
	   dprintf(LOG_LEVEL3, "\n alarm difftime  %f\n", difftime(time(NULL), tLastInform_alarm));		
		// calculate time since last Inform Request
		if (difftime(time(NULL), tLastInform_alarm) >= 60)
		{
		  bInformToDo = 1;
		}
	}
    if(!bInformToDo)
        sleep(3);
  }
  while(1);

error_label:
  if (gListParamValues != NULL)
  {
    free_queue(gListParamValues);
    gListParamValues = NULL;
  }
  sleep(5);
  goto reInform;
  
  dprintf(LOG_LEVEL3, "END OF MAIN PROGRAM\n");
  return 0;
}


/***************************************************************************/
/*** THREAD SERVER                                                       ***/
/***                                                                     ***/
/*** Wait for connection requests, response from Data model repository   ***/
/***************************************************************************/ 

int ACS_REQUEST(void *_soap)
{
    dprintf(LOG_LEVEL1,"acs request \n");
    struct soap *soap = _soap;
    /*hxh: we could use soap_begin_recv to judege if this is a get_method , if return error code 14 , we could know this is a ACS connect request*/
	int ip_1 = (soap->ip >> 24)&0xFF;
	int ip_2 = (soap->ip >> 16)&0xFF;
	int ip_3 = (soap->ip >> 8)&0xFF;
	int ip_4 = soap->ip & 0xFF;


	int flag = 1;
	char tmpbuf[32];
	int fp;
	char *iter = NULL;
	char *tmp = NULL;
	int acs_list[5][4];
/*
	if( (fp = fopen("/jffs/acs_list", "r")) <= 0 )
	{
		printf("no acs_list exist\n");
	}

	int i = 0;
	while(fgets(tmpbuf,32,fp) != NULL)
	{
		printf("%s\n",tmpbuf);
		iter = tmpbuf;
		int j = 0;
		tmp = strtok(iter, ".");
		acs_list[i][j++] = atoi(tmp);
		while((tmp = strtok(NULL, ".") ) != NULL)
		{
			acs_list[i][j++] = atoi(tmp);
			if(j==4)
				break;
		}
		memset(tmpbuf, 0, 32);
		i++;
	}
	printf("get finish\n");
	i--;*/
	//for( ; i--; i>0)
	//{
		//if(ip_1 == acs_list[i][0] && ip_2 == acs_list[i][1] && ip_3 == acs_list[i][2] && ip_4 == acs_list[i][3])
		//{
		//	flag = 1;
		//	break;
		//}
	//}

		if(ip_1 == 127 && ip_2 == 0 && ip_3 == 0 && ip_4 == 1)
		{
			flag = 0;
			//break;
		}
//        printf("flag: %d\n", flag);
	return flag;
}

void *cwmp_server_thread(void *_soap)
{
SOAP_SOCKET s;
struct soap *soap = _soap;
struct soap *tsoap;
// thread
pthread_t tid;

  dprintf(LOG_LEVEL1, "cwmp_server_thread !\n");

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
      dprintf(LOG_LEVEL1, "cwmp_server_thread timed out\n"); 
      break;
    }
	
    dprintf(LOG_LEVEL1, "cwmp_server_thread accepts socket %d connection from IP %ld.%ld.%ld.%ld\n", s, (soap->ip >> 24)&0xFF, (soap->ip >> 16)&0xFF, (soap->ip >> 8)&0xFF, soap->ip&0xFF);
    tsoap = soap_copy(soap); // make a safe copy
    
    if (!tsoap)
      break;
/*hxh: add for acs request*/
	if(!ACS_REQUEST(soap)){
    	pthread_create(&tid, NULL, (void*(*)(void*))reading_thread, 
    	(void*)tsoap);}
	else{
//		tsoap->socket = s;
//		tsoap->keep_alive = 1;
//		tsoap->imode |= SOAP_IO_KEEPALIVE;
//		tsoap->omode |= SOAP_IO_KEEPALIVE;
    	pthread_create(&tid, NULL, (void*(*)(void*))connestreq_thread, (void*)tsoap);
	}
  }

  pthread_detach(pthread_self());
  soap_destroy((struct soap*)soap); // dealloc C++ data
  soap_end((struct soap*)soap); // dealloc data and clean up
  soap_done((struct soap*)soap); // detach soap struct
  free(soap); // !!

  return NULL;
}

void *connestreq_thread(void *_soap)
{
	struct soap *soap= _soap;
	struct SOAP_ENV__Header header; // SOAP Header
	int digest_flag = 0;
	char *ptrEventCode;
	int nError = 0;
	int bContinue = 0;
	
	short bReboot = 0;
	short bFactoryReset = 0;
	
	int status = 1;
	int ret_sem_wait;

	char szEventCommandKey[40];
	char *szFaultString;

	char soap_proxy_host[ARGSIZE]="";
	int soap_proxy_port=0;
	char soap_proxy_userid[ARGSIZE]="";
	char soap_proxy_passwd[ARGSIZE]="";

	pthread_t tDownload,tUpload;

	
	struct http_da_info info;
    memset(&info, 0, sizeof(struct http_da_info));

	struct Namespace empty_namespaces[] = { { NULL } };
	struct InformResponse InformResponse;
	struct cwmp__GetParameterValues tmp_cwmp__GetParameterValuesRequest;
	struct cwmp__GetRPCMethods tmp_cwmp__GetRPCMethods;
	struct cwmp__Reboot tmp_cwmp__RebootRequest;
	struct cwmp__SetParameterValues tmp_cwmp__SetParameterValuesRequest;
	struct cwmp__Download tmp_cwmp__DownloadRequest;
	struct threadDownloadargs downloadargs;
	struct cwmp__Upload tmp_cwmp__UploadRequest;
	struct threadUploadargs uploadargs;
	struct cwmp__GetParameterNames tmp_cwmp__GetParameterNamesRequest;
	struct cwmp__FactoryReset tmp_cwmp__FactroyResetRequest;
	struct cwmp__DeleteObject tmp_cwmp__DeleteObjectRequest; //add by wangjr 091124
	struct cwmp__AddObject tmp_cwmp__AddObjectRequest; //add by wangjr 091124
	struct cwmp__GetParameterAttributes tmp_cwmp__GetParameterAttributesRequest;//add by wangjr 091124  
	struct cwmp__SetParameterAttributes tmp_cwmp__SetParameterAttributesRequest;//add by wangjr 091124

	int nConfPortNumber, nConfConnectionTimeout = 60;


    sem_wait(&S_metux);

	dprintf(LOG_LEVEL1, "\n\nACS connect request thread ... \n");

    if(bFirstTime)
    {
        dprintf(LOG_LEVEL1,"be first time\n`");
    	pthread_detach(pthread_self());
		sem_post(&S_metux);
		return;
	}

//  soap_register_plugin(soap, http_da);
 // printf("register_plugin\n");
  //soap->error = 401;
  //soap->authrealm = "GDI";
  //printf("send error\n");
  //soap_send_fault(soap);
  
//  dprintf(LOG_LEVEL1, "acs ....send HTTP POST empty.\n");

//  soap_destroy(soap);  // dealloc data
//  soap_end(soap);   // dealloc data and cleanup
//  soap_done(soap);	   // detach soap struct
//  soap_init2(soap,SOAP_IO_KEEPALIVE | SOAP_XML_INDENT,SOAP_IO_KEEPALIVE | SOAP_XML_INDENT);
  
//  soap_register_plugin(soap, http_da);
//  soap_connect_command(soap, SOAP_POST, ACS_address, NULL);
//  soap->error = 401;
//  soap->authrealm = "GDI";
//  soap_send_fault(soap);
 //  printf("====\n\n port : %d \n ====\n\n",soap->port);
//   soap_begin_count(soap);
//   soap_end_count(soap);
//  soap_connect_command(soap, SOAP_POST, ACS_address, NULL);

 //  soap_register_plugin(soap, http_da);
 //  soap_response(soap, SOAP_HTML);
 //soap_begin_recv(soap);
 //soap_end_recv(soap);
// dprintf(LOG_LEVEL1,"www-username: %s\n", szWWWAuthusername);
// dprintf(LOG_LEVEL1,"www-password: %s\n", szWWWAuthPasswd);
// dprintf(LOG_LEVEL1,"username: %s\n", soap->userid);
// dprintf(LOG_LEVEL1,"authrealm: %s\n", soap->authrealm);

 if( szWWWAuthusername[0] != '\0' )
 {
    int auth_flag = 0;
    printf("go into 401\n");

    soap->fparse(soap);
//    printf("================\n");
//    printf("buf:%s\n",soap->tmpbuf);
//    printf("userid:%s\n",soap->userid);
//    printf("realm:%s\n",soap->authrealm);
//    printf("================\n");

    if (soap->userid && soap->passwd) // Basic authentication  
    { 
        if (!strcmp(soap->userid, szWWWAuthusername) && !strcmp(soap->passwd, szWWWAuthPasswd))    
        {   
            auth_flag = 1;
        }  
    }  
    else if (soap->authrealm && soap->userid) // Digest authentication  
    { 
  //      printf("%s-%s\n",soap->authrealm, soap->userid);
  //      printf("%s\n",szWWWAuthrealm);
  //      printf("%s\n",szWWWAuthusername);
        if(!strcmp(soap->authrealm, szWWWAuthrealm) && !strcmp(soap->userid, szWWWAuthusername))
        {
  //          printf("gogogo\n");
  //          printf("%s\n",szWWWAuthPasswd);
            if (!http_da_verify_get(soap, szWWWAuthPasswd))
            {
  //              printf("right right right \n");
                auth_flag = 1;
            }
        }
    }
    
    if(! auth_flag)
    {
        //printf("111\n");
        if( szWWWAuthrealm[0] != '\0' )
        {
            //printf("222\n");
            soap->authrealm = szWWWAuthrealm;
        }
        else
        {
            //printf("333\n");
            soap->authrealm = DEFAULT_WWW_REALM;
        }
   soap->keep_alive = 1;
   //soap_send_fault(soap);
//   soap_serializeheader(soap);
//   soap_begin_count(soap);
//   soap_end_count(soap);
   soap_response(soap, 401);
   soap_end_send(soap);
       soap_closesock(soap);
   soap_destroy(soap);  // dealloc data
   soap_end(soap);	  // dealloc data and cleanup
   soap_done(soap);	  // detach soap struct
   sem_post(&S_metux);
   pthread_detach(pthread_self());
   return 0;
   }
 }
//   soap_closesock(soap);
//   soap_begin_recv(soap);
//   soap_end_recv(soap);
  //printf("port: %d\n",soap->port);
  //soap_begin_recv(soap);
  //soap_end_recv(soap);


   soap_response(soap, 200);
   soap_end_send(soap);
   
//   soap_begin_count(soap);
//   soap_end_count(soap);
//   soap_response(soap, SOAP_HTML);
//   soap_send(soap, "");
//   soap_end_send(soap);

Inform_agagin:

 	soap_destroy(soap);  // dealloc data
 	soap_end(soap);	  // dealloc data and cleanup
 	soap_done(soap);	  // detach soap struct
	setTransferComplete(0);

	soap = soap_new();
    soap_init2(soap,SOAP_IO_KEEPALIVE | SOAP_XML_INDENT,SOAP_IO_KEEPALIVE | SOAP_XML_INDENT);
    soap_register_plugin(soap, http_da);
    soap_set_namespaces(soap, Stub_namespaces);
    soap->encodingStyle = NULL;

  // SOAP Header
    header.cwmp__ID = "1"; 
    header.cwmp__NoMoreRequests = "1"; 
    soap->header = &header; // bind the SOAP Header for transport 
    
  //hxh: parameters for Basic HTTP Authent
  //  soap->userid = "hgw";
  //  soap->passwd = "hgw";
  
  // timeout parameters
    soap->send_timeout = 60; 
    soap->recv_timeout = 60;
    soap->accept_timeout = 60;
    soap->connect_timeout = 60;
    
      setActiveNotification(0);

      // Send Get Passive parameters to Data model repository before connection to ACS
      CreateGetPassiveParametersRequest(gdatamodel_address, gId, CONST_FUNCTIONGETPASSIVEPARAMETERS);
  
      do {
        if ((ret_sem_wait=sem_wait(&S_response)) != 0)  // SEMAPHORE lock
            dprintf(LOG_LEVEL1, "ERROR on sem_wait !! \n");
      }
      while ((ret_sem_wait != 0) && (errno == EINTR));
	  
      // Create Inform Request (using parameter list received from data model)
      dprintf(LOG_LEVEL1, "acs ..call_InformRequest.\n");
      
      if( 1 == digest_flag )
      {
          http_da_restore(soap, &info);
      }

	  ptrEventCode = EventCode[4];
      getCommandKey(szEventCommandKey);
      /*
      gListParamValues = (LIST_PARAM_VALUE *)malloc(sizeof(LIST_PARAM_VALUE));
      memset(gListParamValues , 0, sizeof(LIST_PARAM_VALUE));
      strcpy(gListParamValues->szParameter, "InternetGatewayDevice.ManagementServer.URL");
      strcpy(gListParamValues->szValue, "http://127.0.0.1:80");
      gListParamValues->next = (LIST_PARAM_VALUE *)malloc(sizeof(LIST_PARAM_VALUE));
      strcpy(gListParamValues->next->szParameter, "InternetGatewayDevice.ManagementServer.ConnectionRequestURL");
      strcpy(gListParamValues->next->szValue, "http://127.0.0.1:8060");
      gListParamValues->next->next = NULL;
      soap.header = NULL;*/


      nError = call_InformRequest(soap, ACS_address, gListParamValues, gListActiveNotification, ptrEventCode, szEventCommandKey);
      dprintf(LOG_LEVEL2, "acs ---> return code : %d\n", soap->error);
      
      if (soap->error != SOAP_OK) goto error_label2;
  
      dprintf(LOG_LEVEL2, "acs ---> call to server OK\n");
  
      dprintf(LOG_LEVEL1, "..wait_InformResponse.\n");
      nError = wait_InformResponse(soap, &InformResponse);
      dprintf(LOG_LEVEL1, "acs -> return code : %d\n", nError);

      if (soap->error != SOAP_OK)
      {
          if( 401 == soap->error )
          {
            //we need get autename and password before this
            //  if (!strcmp(soap->authrealm, szAuthrealm))
            if ((szAuthrealm != NULL && !strcmp(soap->authrealm, szAuthrealm))
              || szAuthrealm[0] == '\0' && strcpy(szAuthrealm, soap->authrealm))
              {
                  digest_flag = 1;
                  soap_closesock(soap);
                  http_da_save(soap, &info, szAuthrealm, szAuthusername, szAuthPasswd); 
        		  goto Inform_agagin;
              }
          }
          goto error_label2;
      }        
trans_complete:

      if (getTransferComplete())
      {
        FILE *tmpfile;  //add by wangjr 091201
		FILE   *stream;  //add by wangjr 091203
		char tmpbuf[10]={0};  //add by wangjr 091201
		char firmbuf[100]={0};//add by wangjr 091203
		char firmfaultbuf[100]={0};//add by wangjr 091203
        // FaultCode <-- 0 pour l'instant
        // timeStart <-- current pour l'instant
        dprintf(LOG_LEVEL1, "acs ....call_TransferComplete.\n");
        getCommandKey(szEventCommandKey);
/*        add by wangjr 091201   */

		tmpfile = fopen("/tmp/tr069", "r");
		if(tmpfile != NULL)
		{
		   
		   fread( tmpbuf, sizeof( char ), sizeof(tmpbuf), tmpfile );
		}
		fclose(tmpfile);
/*        add by wangjr 091201   */
/*		  add by wangjr 091203	 */

		if(atoi(tmpbuf) == 0 && gfirmupgrade == 1)
		{
            sprintf(firmbuf ,"{ /usr/sbin/checkimg  %s 1>/dev/null 2>&1; echo $? ; }" ,gupgrade_filename);

			dprintf(LOG_LEVEL3, "firmbuf: %s\n", firmbuf);
			stream = popen( firmbuf, "r" );
			fread( firmfaultbuf, sizeof(char), sizeof(firmfaultbuf),	stream); 
			pclose( stream );
			firmfaultbuf[strlen(firmfaultbuf)-1]='\0';

			dprintf(LOG_LEVEL3, "firmfaultbuf: %s\n", firmfaultbuf);
			
			if(1==atoi(firmfaultbuf))
			{
				memset(tmpbuf, 0x00, sizeof(tmpbuf)); 
				strcpy(tmpbuf,"9019");
			}				
		}
		getTR069ErrorString(atoi(tmpbuf), &szFaultString); 
		
		dprintf(LOG_LEVEL3, "szFaultCode: %d\n", atoi(tmpbuf));
        dprintf(LOG_LEVEL3, "szFaultString: %s\n", szFaultString);

	   nError = call_TransferComplete(soap, ACS_address, szEventCommandKey, atoi(tmpbuf), szFaultString, time(NULL));

        if (szFaultString != NULL) free(szFaultString);

        dprintf(LOG_LEVEL3, "-> return code : %d\n", soap->error);
        if (soap->error != SOAP_OK) goto error_label2;

        /*dprintf(LOG_LEVEL1, "....wait_TransferCompleteResponse.\n");
        wait_TransferCompleteResponse(&soap);
        dprintf(LOG_LEVEL1, "-> return code : %d\n", nError);
        if (soap.error != SOAP_OK) goto error_label;*/
      }
      else
      {
        dprintf(LOG_LEVEL1, "acs ....send HTTP POST empty.\n");
        if(digest_flag)
            http_da_restore(soap, &info);
        soap_begin_count(soap);
        soap_end_count(soap);
        soap_connect_command(soap, SOAP_POST, ACS_address, NULL);
        soap_end_send(soap);
      }

      // Init Transfer complete boolean after inform request following a download request
      if (getTransferComplete())
      {
        setTransferComplete(0);
        setCommandKey("");
		gDownloadResponse_status = 0;  //add by wangjr 091124
		gUploadResponse_status = 0;  //add by wangjr 091124
      }

      bContinue = 1;
	  dprintf(LOG_LEVEL1,"bcontinue ...\n");
      while (bContinue && (nError == 0))
      {
        nError = wait_RPCRequest(soap);
        if( szWWWAuthusername[0] != '\0' )
        {
           int auth_flag = 0;
           printf("go into 401\n");
           if (soap->userid && soap->passwd) // Basic authentication  
           { 
               if (!strcmp(soap->userid, szWWWAuthusername) && !strcmp(soap->passwd, szWWWAuthPasswd))    
               {   
                   auth_flag = 1;
               }  
           }  
           else if (soap->authrealm && soap->userid) // Digest authentication  
           { 
               if(!strcmp(soap->authrealm, szWWWAuthrealm) && !strcmp(soap->userid, szWWWAuthusername))
               {
                   if (!http_da_verify_get(soap, szWWWAuthPasswd))
                   {
                       auth_flag = 1;
                   }
                   if (!http_da_verify_post(soap, szWWWAuthPasswd))
                   {
                       auth_flag = 1;
                   }
               }
           }
           
           if(! auth_flag)
           {
               if( szWWWAuthrealm[0] != '\0' )
               {
                   strcpy(soap->authrealm, szWWWAuthrealm);
               }
               else
               {
                   strcpy(soap->authrealm, DEFAULT_WWW_REALM);
               }
               soap->keep_alive = 1;
              //soap_send_fault(soap);
              //soap_serializeheader(soap);
              //soap_begin_count(soap);
              //soap_end_count(soap);
              soap_response(soap, 401);
              soap_end_send(soap);
              soap_destroy(soap);  // dealloc data
              soap_end(soap);    // dealloc data and cleanup
              soap_done(soap);   // detach soap struct
              sem_post(&S_metux);
              pthread_detach(pthread_self());
              return 0;
          }
        }
        dprintf(LOG_LEVEL3, "acs -> return code : %d\n", nError);
        if( 1 == digest_flag )
        {
            printf("restore\n");
            http_da_restore(soap, &info);
            printf("restore end\n");
        }
        printf("%d\n",__LINE__);
        switch(nError)
        {
          case RPC_GETPARAMETERVALUES:
            dprintf(LOG_LEVEL1, "....GetParameterValues Request.\n");
            nError = soap_wait_cwmp__GetParameterValues(soap, &tmp_cwmp__GetParameterValuesRequest);
            dprintf(LOG_LEVEL1, "acs -> return code : %d\n", nError);
  
            dprintf(LOG_LEVEL3, "acs ....ParseGetParamValues_and_SendReponse.\n");
            nError = ParseGetParamValues_and_SendResponse(soap, ACS_address, tmp_cwmp__GetParameterValuesRequest);
            break;
          case RPC_GETMETHODS:
		  	dprintf(LOG_LEVEL1, "....GetRPCMethods Request.\n");
			nError = soap_wait_cwmp__GetRPCMethods(soap, &tmp_cwmp__GetRPCMethods);
			dprintf(LOG_LEVEL1, "acs -> return code : %d\n", nError);

			dprintf(LOG_LEVEL3, "acs ....ParseGetRPCMethods_and_SendReponse.\n");
			nError = ParseGetRPCMethods_and_SendResponse(soap, ACS_address, tmp_cwmp__GetRPCMethods);
		  	break;
		  case RPC_GETPARAMETERNAMES:
		  	dprintf(LOG_LEVEL1, "....GetParameterNames Request.\n");
			nError = soap_wait_cwmp__GetParameterNames(soap, &tmp_cwmp__GetParameterNamesRequest);
			dprintf(LOG_LEVEL1, "acs -> return code : %d\n", nError);

			dprintf(LOG_LEVEL3, "acs ....ParseGetParameterNames_and_SendReponse.\n");
			nError = ParseGetParameterNames_and_SendResponse(soap, ACS_address, tmp_cwmp__GetParameterNamesRequest);
		  	break;
          case RPC_REBOOT:
            dprintf(LOG_LEVEL1, "acs ....Reboot Request.\n");
            nError = soap_wait_cwmp__Reboot(soap, &tmp_cwmp__RebootRequest);
            dprintf(LOG_LEVEL1, "acs -> return code : %d\n", nError);
  
            dprintf(LOG_LEVEL3, "acs ....ParseReboot_and_SendReponse.\n");
            nError = ParseReboot_and_SendResponse(soap, ACS_address, tmp_cwmp__RebootRequest);
  
            bReboot = 1;
            break;
          case RPC_FACTORYRESET:
		  	dprintf(LOG_LEVEL1, "....FactoryReset Request.\n");
            nError = soap_wait_cwmp__FactoryReset(soap, &tmp_cwmp__FactroyResetRequest);			
			dprintf(LOG_LEVEL1, "-> return code : %d\n", nError);
			nError = ParseFactoryReset_and_SendResponse(soap, ACS_address, tmp_cwmp__FactroyResetRequest);
			dprintf(LOG_LEVEL3, "....FactoryReset_and_SendReponse.\n"); 
 
			bFactoryReset=1;
			break;
          case RPC_SETPARAMETERVALUES:
            dprintf(LOG_LEVEL1, "acs ....SetParameterValues Request.\n");
            nError = soap_wait_cwmp__SetParameterValues(soap, &tmp_cwmp__SetParameterValuesRequest);
            dprintf(LOG_LEVEL1, "acs -> return code : %d\n", nError);
            dprintf(LOG_LEVEL3, "acs ....ParseSetParamValues_and_SendReponse.\n");
            status = 0;
            nError = ParseSetParamValues_and_SendResponse(soap, ACS_address, tmp_cwmp__SetParameterValuesRequest, &status);
            break;
			case RPC_UPLOAD:
			  dprintf(LOG_LEVEL1, "....Upload Request.\n");
			  nError = soap_wait_cwmp__Upload(soap, &tmp_cwmp__UploadRequest);
			  dprintf(LOG_LEVEL1, "***************soap_wait_cwmp__Upload-> return code : %d\n", nError);
			  dprintf(LOG_LEVEL3, "....ParseUpload_and_SendResponse.\n");
			  nError = ParseUpload_and_SendResponse(soap, ACS_address, tmp_cwmp__UploadRequest);
			  if (tmp_cwmp__UploadRequest.CommandKey != NULL)
			  {
				  uploadargs.szCmdKey = (char*)malloc(strlen(tmp_cwmp__UploadRequest.CommandKey) + 1);
				  strcpy(uploadargs.szCmdKey, tmp_cwmp__UploadRequest.CommandKey);
			   }
			  
			   uploadargs.szCWMPAddress = (char*)malloc(20);
			   sprintf(uploadargs.szCWMPAddress, "127.0.0.1:%d", nConfPortNumber);
			  
			   if (tmp_cwmp__UploadRequest.URL != NULL)
			   {
				   uploadargs.szURL = (char*)malloc(strlen(tmp_cwmp__UploadRequest.URL)+1);
				   strcpy(uploadargs.szURL, tmp_cwmp__UploadRequest.URL);
			   }
			   if (tmp_cwmp__UploadRequest.FileType) 
			   {
				  uploadargs.szFileType = (char*)malloc(strlen(tmp_cwmp__UploadRequest.FileType)+1);
				  strcpy(uploadargs.szFileType, tmp_cwmp__UploadRequest.FileType);
			   }
			   if (tmp_cwmp__UploadRequest.Username != NULL) 
			   {
					uploadargs.szUsername = (char*)malloc(strlen(tmp_cwmp__UploadRequest.Username)+1);
					strcpy(uploadargs.szUsername, tmp_cwmp__UploadRequest.Username);
			   }
			   if (tmp_cwmp__UploadRequest.Password != NULL) 
			   {
				   uploadargs.szPassword = (char*)malloc(strlen(tmp_cwmp__UploadRequest.Password)+1);
				   strcpy(uploadargs.szPassword, tmp_cwmp__UploadRequest.Password);
			   }
			   uploadargs.FileSize = tmp_cwmp__UploadRequest.FileSize;
						  
			   if (tmp_cwmp__UploadRequest.TargetFileName != NULL) 
			   {
				  uploadargs.szTargetFileName = (char*)malloc(strlen(tmp_cwmp__UploadRequest.TargetFileName)+1);
				  strcpy(uploadargs.szTargetFileName, tmp_cwmp__UploadRequest.TargetFileName);
			  }
			  uploadargs.DelaySeconds = tmp_cwmp__UploadRequest.DelaySeconds;
			  if (pthread_create(&tUpload, NULL, (void*(*)(void*))cwmp_upload_thread, (void*)&uploadargs) != 0)
			  {
				dprintf(LOG_LEVEL1, "....pthread_create cwmp_upload_thread error\n");
			  }
			  else
			  {
				// wait for end of thread execution
				(void) pthread_join(tUpload, NULL);
				//sleep(5);
				bContinue = 0;
				//goto transfercomplete_label;
			   }
			  break;
			case RPC_DOWNLOAD:
			  dprintf(LOG_LEVEL1, "....Download Request.\n");
			  nError = soap_wait_cwmp__Download(soap, &tmp_cwmp__DownloadRequest);
			  dprintf(LOG_LEVEL1, "-> return code : %d\n", nError);
			
			  dprintf(LOG_LEVEL3, "....ParseDownload_and_SendResponse.\n");
			  status = 0;
			  nError = ParseDownload_and_SendResponse(soap, ACS_address, tmp_cwmp__DownloadRequest);
			
			  dprintf(LOG_LEVEL3, "....pthread_create cwmp_download_thread\n");
			  // download thread creation (launch download manager process and wait for response)
			  // create new string for command key parameter
			  if (tmp_cwmp__DownloadRequest.CommandKey != NULL)
			  {
				downloadargs.szCmdKey = (char*)malloc(strlen(tmp_cwmp__DownloadRequest.CommandKey) + 1);
				strcpy(downloadargs.szCmdKey, tmp_cwmp__DownloadRequest.CommandKey);
			  }
			
			  downloadargs.szCWMPAddress = (char*)malloc(20);
              sprintf(downloadargs.szCWMPAddress, "127.0.0.1:%d", nConfPortNumber);
			
			  if (tmp_cwmp__DownloadRequest.URL != NULL) {
				downloadargs.szURL = (char*)malloc(strlen(tmp_cwmp__DownloadRequest.URL)+1);
				//strcpy(downloadargs.szURL, "ftp://192.168.17.60:21/");
				//strcat(downloadargs.szURL, tmp_cwmp__DownloadRequest.URL);
				strcpy(downloadargs.szURL, tmp_cwmp__DownloadRequest.URL);
				//strcpy(downloadargs.szURL, "ftp://192.168.17.60:21/FTPROOT/devtypever/conf/905/474/config.xml");
			  }
			  if (tmp_cwmp__DownloadRequest.FileType) {
				downloadargs.szFileType = (char*)malloc(strlen(tmp_cwmp__DownloadRequest.FileType)+1);
				strcpy(downloadargs.szFileType, tmp_cwmp__DownloadRequest.FileType);
			  }
			  if(strcmp("1 Firmware Upgrade Image", downloadargs.szFileType) == 0)
			  {//hxh:upgrade  image
			    dprintf(LOG_LEVEL3, "=====================================\n is image file\n===============================\n");
			  	//sleep(20);
				//bReboot = 1;
			  }
			  if (tmp_cwmp__DownloadRequest.Username != NULL) {
				downloadargs.szUsername = (char*)malloc(strlen(tmp_cwmp__DownloadRequest.Username)+1);
				strcpy(downloadargs.szUsername, tmp_cwmp__DownloadRequest.Username);
				//strcpy(downloadargs.szUsername, "itms");
			  }
			  if (tmp_cwmp__DownloadRequest.Password != NULL) {
				downloadargs.szPassword = (char*)malloc(strlen(tmp_cwmp__DownloadRequest.Password)+1);
				strcpy(downloadargs.szPassword, tmp_cwmp__DownloadRequest.Password);
				//strcpy(downloadargs.szPassword, "itms");
			  }
			  downloadargs.FileSize = tmp_cwmp__DownloadRequest.FileSize;
			  if (tmp_cwmp__DownloadRequest.TargetFileName != NULL) {
				downloadargs.szTargetFileName = (char*)malloc(strlen(tmp_cwmp__DownloadRequest.TargetFileName)+1);
				strcpy(downloadargs.szTargetFileName, tmp_cwmp__DownloadRequest.TargetFileName);
				//strcpy(downloadargs.szTargetFileName, "config.xml");
			  }
			  downloadargs.DelaySeconds = tmp_cwmp__DownloadRequest.DelaySeconds;
			
			  if (strcmp(soap_proxy_host, "") != 0) {
				downloadargs.szProxyHost = (char*)malloc(strlen(soap_proxy_host)+1);
				strcpy(downloadargs.szProxyHost, soap_proxy_host);
			
				downloadargs.ProxyPort = soap_proxy_port;
			  }
			
			  if (strcmp(soap_proxy_userid, "") != 0) {
				downloadargs.szProxyUserId = (char*)malloc(strlen(soap_proxy_userid)+1);
				strcpy(downloadargs.szProxyUserId, soap_proxy_userid);
			  }
			
			  if (strcmp(soap_proxy_passwd, "") != 0) {
				downloadargs.szProxyPasswd = (char*)malloc(strlen(soap_proxy_passwd)+1);
				strcpy(downloadargs.szProxyPasswd, soap_proxy_passwd);
			  }
			
			  if (pthread_create(&tDownload, NULL, (void*(*)(void*))cwmp_download_thread, (void*)&downloadargs) != 0)
			  {
				dprintf(LOG_LEVEL1, "....pthread_create cwmp_download_thread error\n");
			  }
			  else
			  {
			    dprintf(LOG_LEVEL1, "download start\n");
				// wait for end of thread execution
				//sleep(5);
				(void) pthread_join(tDownload, NULL);
				dprintf(LOG_LEVEL1, "download end\n");
				bContinue = 0;
				
				//goto transfercomplete_label;
			   }
			  break;
			  /* add by wangjr 091124 */
						case RPC_ADDOBJECT:
						  dprintf(LOG_LEVEL1, "acs....AddObject Request.\n");
						  nError = soap_wait_cwmp__AddObject(soap, &tmp_cwmp__AddObjectRequest);		  
						  dprintf(LOG_LEVEL1, "acs-> return code : %d\n", nError);
						  nError = ParseAddObject_and_SendResponse(soap, ACS_address, tmp_cwmp__AddObjectRequest);
						  dprintf(LOG_LEVEL3, "acs....AddObject_and_SendReponse.\n"); 
						break;
						case RPC_DELETEOBJECT:
						  dprintf(LOG_LEVEL1, "acs....DeleteObject Request.\n");
						  nError = soap_wait_cwmp__DeleteObject(soap, &tmp_cwmp__DeleteObjectRequest); 		  
						  dprintf(LOG_LEVEL1, "acs-> return code : %d\n", nError);
						  nError = ParseDeleteObject_and_SendResponse(soap, ACS_address, tmp_cwmp__DeleteObjectRequest);
						  dprintf(LOG_LEVEL3, "acs....DeleteObject_and_SendReponse.\n"); 
						break;
						case RPC_GETPARAMETERATTRIBUTES:
						  dprintf(LOG_LEVEL1, "acs....ParseGetParameterAttributes Request.\n");
						  nError = soap_wait_cwmp__GetParameterAttributes(soap, &tmp_cwmp__GetParameterAttributesRequest);
						  dprintf(LOG_LEVEL1, "acs-> return code : %d\n", nError);
						  dprintf(LOG_LEVEL3, "acs....ParseGetParameterAttributes_and_SendReponse.\n");
						  nError = ParseGetParameterAttributes_and_SendResponse(soap, ACS_address, tmp_cwmp__GetParameterAttributesRequest);
						  break;
					   case RPC_SETPARAMETERATTRIBUTES:
						  dprintf(LOG_LEVEL1, "acs....ParseSetParameterAttributes Request.\n");
						  nError = soap_wait_cwmp__SetParameterAttributes(soap, &tmp_cwmp__SetParameterAttributesRequest);
						  dprintf(LOG_LEVEL1, "acs-> return code : %d\n", nError);
						  dprintf(LOG_LEVEL3, "acs....ParseSetParameterAttributes_and_SendReponse.\n");
						  nError = ParseSetParameterAttributes_and_SendResponse(soap, ACS_address, tmp_cwmp__SetParameterAttributesRequest);
						  break;  
			  /* add by wangjr 091124 */

          case SOAP_STOP:
            dprintf(LOG_LEVEL1, "acs -> HTTP Empty (no more exchanges)\n");
            bContinue = 0;
            if (soap_end_recv(soap)){
				return soap_closesock(soap);
				sem_post(&S_metux);
			}
            break; 
        }
      }


    // Reboot STB
    if (bReboot)
    {
      dprintf(LOG_LEVEL1, "\n.\n.\nReboot..\n\n");
      sleep(1);
      system("/sbin/sysreboot");
    }
	if(bFactoryReset)
	{
	  dprintf(LOG_LEVEL1, "\n.\n.\nFactoryReset..\n\n");
      sleep(1);
      system("rm -f /jffs/config.xml && /sbin/sysreboot");
	
	}
    
    soap_destroy(soap);  // dealloc data
    soap_end(soap);      // dealloc data and cleanup
    soap_done(soap);     // detach soap struct

error_label2:
  if (gListParamValues != NULL)
  {
    free_queue(gListParamValues);
    gListParamValues = NULL;
  }

  dprintf(LOG_LEVEL3, "END OF ACS PROGRAM\n");
  sem_post(&S_metux);
  return 0;
	
}


/***************************************************************************/
/*** READING THREAD                                                      ***/
/***                                                                     ***/
/*** Read data on socket from Data model repository                      ***/
/*** or Download Manager                                                 ***/
/***************************************************************************/ 

void *reading_thread(void *_soap)
{
int nError;
struct soap *soap = _soap;
STRUCT_MESSAGE message;
LIST_PARAM_VALUE *list_param_value;
enum methodName eMethodName;
enum hostType eHostType;
enum functionName eFunctionName;
int i = 0;
  dprintf(LOG_LEVEL2, "reading_thread\n");
  nError = soap_begin_recv(soap);
  if (nError != 0) dprintf(LOG_LEVEL1, "reading_thread.. nError: %d!\n", nError);

  // list of (param,value)
  list_param_value = (LIST_PARAM_VALUE *)malloc(sizeof(LIST_PARAM_VALUE));
  memset(list_param_value, 0x00, sizeof(LIST_PARAM_VALUE));

  if (ExtractXMLTagsWithMultipleParams(soap, &message, list_param_value) != 0)
  //if (ExtractXMLTags(soap, &message) != 0) // extract data from XML message
  {
    dprintf(LOG_LEVEL1, "ExtractXMLTags error\n");
    goto endthread_label;
  }
  message.ListParamValue = list_param_value;

  // Process message to retrieve method name, host type and function name
  if (ProcessMessage(&message, &eMethodName, &eHostType, &eFunctionName) > -1)
  {
    dprintf(LOG_LEVEL3, "eMethodName: %d\n", eMethodName);
    dprintf(LOG_LEVEL3, "eHostType: %d\n", eHostType);
    dprintf(LOG_LEVEL3, "eFunctionName: %d\n", eFunctionName);

    if ((eMethodName == RESPONSE) && (message.Id != -1)) // response message
    {
      gflagResponse_OK = 1;
      if (gId == -1) gId = message.Id;

      // copy list received in global list
      set_globalList(&gListParamValues, list_param_value);

      // SEMAPHORE unlock
      dprintf(LOG_LEVEL3, "reading_thread %lu, unlock S_response\n", (unsigned long)pthread_self());
      sem_post(&S_response);
    }
    if (eMethodName == ERROR)
    {
      gflagResponse_OK = 0;

      // free global list
      set_globalList(&gListParamValues, NULL);

      // SEMAPHORE unlock
      dprintf(LOG_LEVEL3, "reading_thread %lu, unlock S_response\n", (unsigned long)pthread_self());
      sem_post(&S_response);
    }
    else if (eMethodName == NOTIFY) // notify message
    {
      dprintf(LOG_LEVEL3, "NOTIFY message\n");

      if (gListActiveNotification != NULL)
      {
        free_queue(gListActiveNotification);
        gListActiveNotification = NULL;
      }

      gListActiveNotification = duplicate_queue(list_param_value);

      CreateResponse(gdatamodel_address, gId);

      // Informs main thread for notification
      setActiveNotification(1);
    }
    else if (eMethodName == DOWNLOADCOMPLETE) // Download complete message
    {
      dprintf(LOG_LEVEL3, "DOWNLOADCOMPLETE message\n");

      // copy list received in global list
      if ((list_param_value != NULL) && (list_param_value->szValue != NULL))
      {
        setFaultCode(atoi(list_param_value->szValue));
      }

      // SEMAPHORE unlock
      dprintf(LOG_LEVEL3, "reading_thread %lu, unlock S_download\n", (unsigned long)pthread_self());
      //sem_post(&S_download);  //remove by wangjr 091130
    }
	 else if (eMethodName == UPLOADCOMPLETE) // Download complete message
    {
      dprintf(LOG_LEVEL3, "UPLOADCOMPLETE message\n");

      // copy list received in global list
      if ((list_param_value != NULL) && (list_param_value->szValue != NULL))
      {
        setFaultCode(atoi(list_param_value->szValue));
      }

      // SEMAPHORE unlock
      dprintf(LOG_LEVEL3, "reading_thread %lu, unlock S_upload\n", (unsigned long)pthread_self());
     // sem_post(&S_upload); //remove by wangjr 091202
    }
  }

endthread_label:
  nError = soap_end_recv(soap);
  if (nError != 0) dprintf(LOG_LEVEL1, "reading_thread.. nError: %d!\n", nError);

  soap_closesock(soap);

  free_queue(list_param_value); // free list of param-value

  soap_destroy((struct soap*)soap); // dealloc C++ data
  soap_end((struct soap*)soap); // dealloc data and clean up
  soap_done((struct soap*)soap); // detach soap struct
  free(soap);   // !!

  dprintf(LOG_LEVEL3, "reading_thread, before return !\n");

  return NULL;
}



/***************************************************************************/
/*** CWMP DOWNLOAD THREAD                                                ***/
/***                                                                     ***/
/*** Executes Download manager process      update by wangjr 091124                             ***/
/*** Wait for response (download complete response) from Download manager***/
/*** Set parameters for next inform to inform ACS of download            ***/
/***************************************************************************/

void *cwmp_download_thread(void *_downloadargs)
{
struct threadDownloadargs *args;
char szExecDownload[2*ARGSIZE]={0};
char szUpgrade[ARGSIZE];
char szTemp[ARGSIZE];
int ret_sem_wait;
char szFileName[ARGSIZE];
char *ptrLastSlash;


  // arg of thread
  args = (struct threadDownloadargs*)_downloadargs;

  dprintf(LOG_LEVEL1, "cwmp_download_thread\n");

  dprintf(LOG_LEVEL3, "......cwmp_download_thread, launch download manager process\n");
  sprintf(szExecDownload, "/usr/bin/DownloadManager --CWMPAddress:%s", args->szCWMPAddress);

  if (args->szURL)  {
    sprintf(szTemp, " --URL:'%s'", args->szURL);
    strcat(szExecDownload, szTemp);
  }
  if (args->szFileType)  {
    sprintf(szTemp, " --FileType:'%s'", args->szFileType);
    strcat(szExecDownload, szTemp);
  }
  if (args->FileSize > 0)  {
    sprintf(szTemp, " --FileSize:%d", args->FileSize);
    strcat(szExecDownload, szTemp);
  }
  if (args->szUsername)  {
    sprintf(szTemp, " --Username:'%s'", args->szUsername);
    strcat(szExecDownload, szTemp);
  }
  if (args->szPassword)  {
    sprintf(szTemp, " --Password:'%s'", args->szPassword);
    strcat(szExecDownload, szTemp);
  }
  if (args->szTargetFileName)  {
    sprintf(szTemp, " --TargetFileName:'%s'", args->szTargetFileName);
    strcat(szExecDownload, szTemp);
  }

  sprintf(szTemp, " --DelaySeconds:%d", args->DelaySeconds);
  strcat(szExecDownload, szTemp);

#if 0  //remove by wangjr 
  if (args->szProxyHost && (args->ProxyPort > 0))  {
    sprintf(szTemp, " --ProxyHost:'%s'", args->szProxyHost);
    strcat(szExecDownload, szTemp);
    sprintf(szTemp, " --ProxyPort:%d", args->ProxyPort);
    strcat(szExecDownload, szTemp);

    if (args->szProxyUserId && args->szProxyPasswd)  {
      sprintf(szTemp, " --ProxyUserId:'%s'", args->szProxyUserId);
      strcat(szExecDownload, szTemp);
      sprintf(szTemp, " --ProxyPasswd:'%s'", args->szProxyPasswd);
      strcat(szExecDownload, szTemp);
    }
  }
#endif

  //dprintf(LOG_LEVEL3, "CommandLine: %s\n", szExecDownload);
  time(&download_endtime); //add by wangjr 091124
 dprintf(LOG_LEVEL1, "------------------CommandLine: %s\n", szExecDownload);
  system(szExecDownload);

  // wait for download complete (semaphore) received from Download manager
  dprintf(LOG_LEVEL3, "......cwmp_download_thread, wait for download complete message from Download manager\n");
  #if 0 //remove by wangjr
  do {
    if ((ret_sem_wait=sem_wait(&S_download)) != 0)  // SEMAPHORE lock
        dprintf(LOG_LEVEL1, "ERROR on sem_wait !! \n");
  }while ((ret_sem_wait != 0) && (errno == EINTR));
  #endif
     time(&download_endover);  //add by wangjr 091124
  setTransferComplete(1);  
  setCommandKey(args->szCmdKey);
  free(args->szCmdKey);

   if(strcmp(args->szURL, "") != 0)
    {
      if ((ptrLastSlash = strrchr(args->szURL, '/')) != NULL)
      {
        if (++ptrLastSlash != NULL)
          strcpy(szFileName, ptrLastSlash);
      }
    }
    else
    {
      // set target filename
      strcpy(szFileName, args->szURL);
    }
/* update by wangjr 091124  */	
  if(strstr(args->szFileType,"Firmware Upgrade Imag")!=NULL)
  {
	strcpy(gupgrade_filename,szFileName);
	gfirmupgrade=1;
  }  
  else if (strstr(args->szFileType,"Vendor Configuration File")!=NULL)
  {
	strcpy(gconfig_filename,szFileName);
     gupdateconfig=1;
  }
  else
  {
      ;;
     
  }
  
  pthread_detach(pthread_self());
  dprintf(LOG_LEVEL3, "cwmp_download_thread, before return !\n");
  return NULL;
}
 
 /***************************************************************************/
 /*** CWMP UPLOAD THREAD												***/
 /***																	  ***/
 /*** Executes Upload manager process									***/
 /*** Wait for response (upload complete response) from Upload manager***/
 /*** Set parameters for next inform to inform ACS of download			  ***/
 /***************************************************************************/
 
 void *cwmp_upload_thread(void *_uploadargs)
 {
	 struct threadUploadargs *args;
	 char szExecUpload[2*ARGSIZE];
	 char szTemp[ARGSIZE];
	 int ret_sem_wait;
 
   // arg of thread
   args = (struct threadUploadargs*)_uploadargs;
 
   dprintf(LOG_LEVEL1, "cwmp_upload_thread\n");
 
   dprintf(LOG_LEVEL3, "......cwmp_upload_thread, launch upload manager process\n");
 
   sprintf(szExecUpload, "/usr/bin/UploadManager --CWMPAddress:%s", args->szCWMPAddress);
 
   if (args->szURL)  
   {
 
	 sprintf(szTemp, " --URL:'%s'", args->szURL);
	 strcat(szExecUpload, szTemp);
   }
   if (args->szFileType)  {
	 sprintf(szTemp, " --FileType:'%s'", args->szFileType);
	 strcat(szExecUpload, szTemp);
   }
   if (args->FileSize > 0)	{
	 sprintf(szTemp, " --FileSize:%d", args->FileSize);
	 strcat(szExecUpload, szTemp);
   }
   if (args->szUsername)  {
	 sprintf(szTemp, " --Username:'%s'", args->szUsername);
	 strcat(szExecUpload, szTemp);
   }
   if (args->szPassword)  {
	 sprintf(szTemp, " --Password:'%s'", args->szPassword);
	 strcat(szExecUpload, szTemp);
   }
   #if 0
   if (args->szTargetFileName)	{
	 sprintf(szTemp, " --TargetFileName:'%s'", args->szTargetFileName);
	 strcat(szExecUpload, szTemp);
   }
   #endif
   
   sprintf(szTemp, " --DelaySeconds:%d", args->DelaySeconds);
   strcat(szExecUpload, szTemp);
 
   time(&download_endtime); //add by wangjr 091202
   dprintf(LOG_LEVEL3, "CommandLine: %s\n", szExecUpload);
   printf("CommandLine: %s\n", szExecUpload);   
  if(strstr(args->szFileType,"Vendor Log File"))  //update by wangjr 091124
   {
       system("cp /var/log/messages  log");
   }
   else if(strstr(args->szFileType,"Vendor Configuration File"))  //update by wangjr 091204
   {
       system("cp /etc/config/config.xml  config.xml");
   }
   else
   {
		;;
   }
   system( szExecUpload);
	time(&download_endover); //add by wangjr 091202
   // wait for upload complete (semaphore) received from Upload manager
   dprintf(LOG_LEVEL3, "......cwmp_upload_thread, wait for upload complete message from Upload manager\n");
   // Set parameters for next inform

#if 0
   do{
	  if ((ret_sem_wait=sem_wait(&S_upload)) != 0)	// SEMAPHORE lock
		  dprintf(LOG_LEVEL1, "ERROR on sem_wait !! \n");
	}while ((ret_sem_wait != 0) && (errno == EINTR));
#endif


   setTransferComplete(1);
   setCommandKey(args->szCmdKey); 
   // free memory of arg passed
   #if 0
   free(args->szCmdKey);
   if((args->szCWMPAddress)!=NULL )
	   free(args->szCWMPAddress);
   if((args->szURL)!=NULL )
	   free(args->szURL);
   if((args->szFileType)!=NULL )
	   free(args->szFileType);
   if((args->szUsername)!=NULL )
	   free(args->szUsername);
   if((args->szPassword)!=NULL )
	   free(args->szPassword);
   if((args->szTargetFileName)!=NULL )
	   free(args->szTargetFileName);
   if((args->szProxyHost)!=NULL )
	   free(args->szProxyHost);
   if((args->szProxyUserId)!=NULL )
	   free(args->szProxyUserId);
   if((args->szProxyPasswd)!=NULL )
	   free(args->szProxyPasswd);
  #endif
   
   pthread_detach(pthread_self());
   dprintf(LOG_LEVEL3, "cwmp_upload_thread, before return !\n");
   return NULL;
 }



/***************************************************************************/
/*** SUBSCRIBE SEQUENCE                                                  ***/
/***************************************************************************/

int SubscribeSequence(struct soap *_soap, char *_soap_endpoint, int _nConfPort)
{
int nRet = -1;
char szline[ARGSIZE];
struct Namespace empty_namespaces[] = { { NULL } };
struct soap *soap;

  gflagResponse_OK = 0; // response flag set to 1 when a response is received following a subscribe

  // subscribe to Data model repository for all parameters

// ATTENTION : Don't use SOAP_IO_KEEPALIVE during message
// because each message must correspond to one TCP connection
  if (_soap == NULL)
    InitSoapStruct(soap, empty_namespaces);
  else soap = _soap;

  nRet = -1;
  dprintf(LOG_LEVEL3, "subscribe : call soap_connect_command\n");
  if (soap_connect_command(soap, SOAP_POST, _soap_endpoint, "") == SOAP_OK)
  {
    dprintf(LOG_LEVEL3, "subscribe : success \n");
    // SUBSCRIBE MESSAGE
    soap_element_begin_out(soap, CONST_MESSAGE, 0, NULL);
    soap_element_begin_out(soap, "NAME", 0, NULL);
    sprintf(szline, CONST_SUBSCRIBE);
    soap_send_raw(soap, szline, strlen(szline));
    soap_element_end_out(soap, "NAME");
    
    soap_element_begin_out(soap, "Type", 0, NULL);
    sprintf(szline, CONST_TYPECLIENT);
    soap_send_raw(soap, szline, strlen(szline));
    soap_element_end_out(soap, "Type");
    
    soap_element_begin_out(soap, "Port", 0, NULL);
    sprintf(szline, "%d", _nConfPort);
    soap_send_raw(soap, szline, strlen(szline));
    soap_element_end_out(soap, "Port");
    
    soap_element_end_out(soap, CONST_MESSAGE);
    
    soap_end_send(soap);
    nRet = 0;
  }
  return nRet;
}



/*********************************************/
/* ParseGetParamValues_and_SendResponse      */
/* Parse GetParameterValues request and send */
/* response                                  */
/*                                           */
/*********************************************/
//# if 0 add by wangjr for ng test

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
char szVal[256];
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


char * GetLANTotalBytesReceived(char *str_arg)
{
	memset(szVal, 0x00, sizeof(szVal));
	FILE *fp;
	//int sum = 0;/*update by wangjr 2010-12-03*/
	unsigned long long sum=0;
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
		sprintf(szVal,"%llu",sum);
		pclose(fp);
	}
	return szVal;

}


char * GetLANTotalBytesSent(char *str_arg)
{
	FILE *fp;
	//int sum = 0;/*update by wangjr 2010-12-03*/
	unsigned long long sum=0;
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
			 ///CGetChar(&sum, szVal);/*update by wangjr 2010-12-03*/
				sscanf(szVal,"%llu",&sum);		 
	   }
	   sprintf(szVal,"%llu",sum);/*update by wangjr 2010-12-03*/
	   pclose(fp);
	}
	return szVal;

}

char * GetWANTotalBytesReceived(char *str_arg)
{

	FILE *fp;
	//int sum = 0;/*update by wangjr 2010-12-03*/
	unsigned long long sum=0;
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

char * GetWANTotalBytesSent(char *str_arg)
{

	FILE *fp;
	//int sum = 0;/*update by wangjr 2010-12-03*/
	unsigned long long sum=0;	
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
			//CGetChar(&sum, szVal);	/*update by wangjr 2010-12-03*/
			sscanf(szVal,"%llu",&sum);				
		}
		sprintf(szVal,"%llu",sum);/*update by wangjr 2010-12-03*/	  
		pclose(fp);
	}
	
	return szVal;

}


char * GetLANTotalPacketsSent(char *str_arg)
{
   
	FILE *fp;
	//int sum = 0;/*update by wangjr 2010-12-03*/
	unsigned long long sum=0;	
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
		sprintf(szVal,"%llu",sum);/*update by wangjr 2010-12-03*/
		pclose(fp);
	}
	return szVal;

}


char * GetLANTotalPacketsReceived(char *str_arg)
{
	FILE *fp;
	//int sum = 0;/*update by wangjr 2010-12-03*/
	unsigned long long sum=0;
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
			//CGetChar(&sum, szVal);	/*update by wangjr 2010-12-03*/
			sscanf(szVal,"%llu",&sum);			
		}
		sprintf(szVal,"%llu",sum);
		pclose(fp);
	}
	return szVal;

}

char *GetSSID(char *str_arg)
{
   memset(szVal, 0x00, sizeof(szVal));
   wireless_get_ssid(1,szVal,sizeof(szVal));
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

char * GetReceiveNoise(char *str_arg)
{
	FILE *fp;
	char str_tmp_cmd[SPRINT_MAX_LEN];
	memset( str_tmp_cmd, 0, SPRINT_MAX_LEN );
	int vapnum=1;
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



char * GetWANTotalPacketsSent(char *str_arg)
{

	FILE *fp;
	//int sum = 0;
	unsigned long long sum = 0;/*update by wangjr 2010-12-03*/
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
	//int sum = 0;
	unsigned long long sum = 0;/*update by wangjr 2010-12-03*/
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
		sprintf(szVal,"%llu",sum);/*update by wangjr 2010-12-03*/
		pclose(fp);
	}	
	return szVal;

}


char * GetCTLANTotalBytesSent(char *str_arg)
{
	FILE *fp;
	//int sum = 0;/*update by wangjr 2010-12-03*/
	unsigned long long sum = 0;/*update by wangjr 2010-12-03*/
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
	//int sum = 0;
	unsigned long long sum = 0;/*update by wangjr 2010-12-03*/
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
	//int sum = 0;
	unsigned long long sum = 0;/*update by wangjr 2010-12-03*/
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
		sprintf(szVal,"%llu",sum);	  
		pclose(fp);
	}
	
	return szVal;


}

char * GetCTWANTotalBytesReceived(char *str_arg)
{

	FILE *fp;
	//int sum = 0;
	unsigned long long sum = 0;/*update by wangjr 2010-12-03*/
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

//////////////////update by wangjr 091124//////////////////////////////
char * GetLANPacketsError(char *str_arg)
{	
	FILE *fp;
	//int sum = 0;
	unsigned long long sum = 0;/*update by wangjr 2010-12-03*/
	char right_rx[SPRINT_MAX_LEN];	
	char right_tx[SPRINT_MAX_LEN];
	char error_rx[SPRINT_MAX_LEN];
	char error_tx[SPRINT_MAX_LEN];
	char str_tmp_cmd[SPRINT_MAX_LEN];
	//int iright=0;/*update by wangjr 2010-12-03*/
	//int ierror=0;
	unsigned long long  iright = 0;
	unsigned long long  ierror = 0;
	unsigned long long  iright_rx = 0;
	unsigned long long  iright_tx = 0;
	unsigned long long  ierror_rx = 0;
	unsigned long long  ierror_tx = 0;
	
	int vapnum=1;
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
	int sum = 0;

	char right_rx[SPRINT_MAX_LEN];	
	char right_tx[SPRINT_MAX_LEN];
	char error_rx[SPRINT_MAX_LEN];
	char error_tx[SPRINT_MAX_LEN];
	char str_tmp_cmd[SPRINT_MAX_LEN];
	/*int iright=0;
	int ierror=0;*//*update by wangjr 2010-12-03*/
	unsigned long long  iright = 0;
	unsigned long long  ierror = 0;
	unsigned long long  iright_rx = 0;
	unsigned long long  iright_tx = 0;
	unsigned long long  ierror_rx = 0;
	unsigned long long  ierror_tx = 0;
	
	int vapnum=1;
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
	ierror+=atoi(error_tx);*/ /*update by wangjr 2010-12-03*/
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


char * GetAskPass(char *str_arg)
{

 FILE *fp;
 int sum=0;
 char str_tmp_cmd[SPRINT_MAX_LEN];
 int vapnum=1;
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
 		CGetChar(&sum, szVal);
	sprintf(szVal,"%d",sum);
    return szVal;

}
//////////////////update by wangjr 091124////////////////////////////////////

char * GetResponsePass(char *str_arg)
{

	FILE *fp;
	int sum=0;
	char str_tmp_cmd[SPRINT_MAX_LEN];
	int vapnum=1;
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
		CGetChar(&sum, szVal);
	sprintf(szVal,"%d",sum);
    return szVal;

}
//////////////////update by wangjr 091124////////////////////////////////////

char * GetSuccessPass(char *str_arg)
{
  
  	FILE *fp;
	int sum=0;
  	char str_tmp_cmd[SPRINT_MAX_LEN];
	int vapnum=1;
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
  	CGetChar(&sum, szVal);
	sprintf(szVal,"%d",sum);
    return szVal;

}


char * GetCTReceiveRate(char *str_arg)
{
	FILE *fp;
	//int sum = 0;/*update by wangjr 2010-12-03*/
	unsigned long long sum =0 ;
	memset(szVal, 0x00, sizeof(szVal));
	fp=popen("/bin/cat /tmp/user-rate |/usr/bin/awk '{print $3}'|/usr/bin/head -2|sed '1d'","r");
	if(fp)
	{
		fgets(szVal,sizeof(szVal),fp);
		szVal[strlen(szVal)-1] = '\0';
		 pclose(fp);
	 }	
	//CGetChar(&sum, szVal);/*update by wangjr 2010-12-03*/
	sscanf(szVal,"%llu",&sum);
	sprintf(szVal,"%llu",sum);
	
	return szVal;

}	

char * GetCTSendRate(char *str_arg)
{
	FILE *fp;
	//int sum=0;/*update by wangjr 2010-12-03*/
	unsigned long long sum =0 ;
	memset(szVal, 0x00, sizeof(szVal));
	fp=popen("/bin/cat /tmp/user-rate |/usr/bin/awk '{print $4}'|/usr/bin/head -2|sed '1d'","r");
	if(fp)
	{
		fgets(szVal,sizeof(szVal),fp);
		szVal[strlen(szVal)-1] = '\0';
		 pclose(fp);
	 }	
	//CGetChar(&sum, szVal);
	sscanf(szVal,"%llu",&sum);/*update by wangjr 2010-12-03*/
	sprintf(szVal,"%llu",sum);
	return szVal;

}
char * GetTotalBytes(char *str_arg)
{
  // int sum=0;/*update by wangjr 2010-12-03*/
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
		sscanf(szVal,"llu",&rx_sum);
		pclose(fp);
	}
	fp=popen("/bin/cat /tmp/user-rate |/usr/bin/awk '{print $4}'|/usr/bin/head -2|sed '1d'","r");
	if(fp)
	{
	    memset(szVal, 0x00, sizeof(szVal));
		fgets(szVal,sizeof(szVal),fp);
		/*szVal[strlen(szVal)-1] = '\0';
			sum+=atoi(szVal);*//*update by wangjr 2010-12-03*/
		sscanf(szVal,"llu",&tx_sum);
		sum = rx_sum + tx_sum;
		pclose(fp);
	 }	
	sprintf(szVal,"%llu",sum);	
	return szVal;
}

char * GetAuthenticationUser(char *str_arg)
{
   #if 0 // for NG
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
	memset(szVal, 0x00, sizeof(szVal));
	fp=popen("/usr/sbin/wlanconfig ath0 list | wc -l","r");
	if(fp)
	{
		fgets(szVal,sizeof(szVal),fp);
		szVal[strlen(szVal)-1] = '\0';
		 pclose(fp);
	 }	
	CGetChar(&sum, szVal);
	if (0 < sum )
                sum=sum-1;
	sprintf(szVal,"%d",sum);
	return szVal;
}

//endif 
int ParseGetParamValues_and_SendResponse(struct soap *_soap, const char *_endpoint, struct cwmp__GetParameterValues _tmp_cwmp__GetParameterValuesRequest)
{
	int nNbValue = 0;
	int nCountValue = 0; //add by wangjr
	int nRet = 0;
	int nNbParam;
	char tmp_parameter[100]={0}; //add by wangjr
	struct cwmp__GetParameterValuesResponse tmp_cwmp__GetParameterValuesResponse;
	struct cwmp__FaultResponse tmp_cwmp__FaultResponse = {"Client","CWMP fault",};//add by wangjr 20091112
	int nLength;
	int ret_sem_wait;
	int FaultFlag=0; //add by wangjr

  dprintf(LOG_LEVEL3, "---> allocates memory for GetParameterValues Response\n");
  // memory allocation for response
  tmp_cwmp__GetParameterValuesResponse.ParameterList = (struct __ParameterList *)malloc(sizeof(struct __ParameterList));
  memset(tmp_cwmp__GetParameterValuesResponse.ParameterList, 0x00, sizeof(struct __ParameterList));

  dprintf(LOG_LEVEL3, "---> analyze GetParameterValues Request\n");
  // test if nb parameter(s) higher than 0
  nNbParam = _tmp_cwmp__GetParameterValuesRequest.ParameterNames.__size;
  if (nNbParam > 0)
  {
    tmp_cwmp__GetParameterValuesResponse.ParameterList->__ptrParameterValueStruct = (struct cwmp__ParameterValueStruct *)malloc(nNbParam*sizeof(struct cwmp__ParameterValueStruct));

    // Parse GetParameterValues Request And Fill Response
    tmp_cwmp__GetParameterValuesResponse.ParameterList->__size = nNbParam;
    for (nNbValue=0; nNbValue < nNbParam; nNbValue++)
    {
      dprintf(LOG_LEVEL3, "......send request for %s to data model\n", _tmp_cwmp__GetParameterValuesRequest.ParameterNames.__ptrstring[nNbValue]);
    // #if 0 //add by wangjr 2010 

	 if(strstr(_tmp_cwmp__GetParameterValuesRequest.ParameterNames.__ptrstring[nNbValue ] ,"InternetGatewayDevice.DeviceInfo.X_CT-COM_WorkTime")!= 0)	 	 
	 {

        tmp_cwmp__GetParameterValuesResponse.ParameterList->__ptrParameterValueStruct[nNbValue].Name = _tmp_cwmp__GetParameterValuesRequest.ParameterNames.__ptrstring[nNbValue];

        nLength = 20;
  
        tmp_cwmp__GetParameterValuesResponse.ParameterList->__ptrParameterValueStruct[nNbValue].Value.Value = (char*)malloc(nLength+1);
        memset(tmp_cwmp__GetParameterValuesResponse.ParameterList->__ptrParameterValueStruct[nNbValue].Value.Value, 0x00, nLength+1);
        strncpy(tmp_cwmp__GetParameterValuesResponse.ParameterList->__ptrParameterValueStruct[nNbValue].Value.Value, GetWorkTime(""), nLength);
        tmp_cwmp__GetParameterValuesResponse.ParameterList->__ptrParameterValueStruct[nNbValue].Value.__type = SOAP_TYPE_string;	 

	 }
	 else if(strstr(_tmp_cwmp__GetParameterValuesRequest.ParameterNames.__ptrstring[nNbValue ] ,"InternetGatewayDevice.LANDevice.1.WLANConfiguration.1.X_CT-COM_LAN-TotalBytesReceived")!= 0)
	 {
        tmp_cwmp__GetParameterValuesResponse.ParameterList->__ptrParameterValueStruct[nNbValue].Name = _tmp_cwmp__GetParameterValuesRequest.ParameterNames.__ptrstring[nNbValue];

        nLength = 20;
  
        tmp_cwmp__GetParameterValuesResponse.ParameterList->__ptrParameterValueStruct[nNbValue].Value.Value = (char*)malloc(nLength+1);
        memset(tmp_cwmp__GetParameterValuesResponse.ParameterList->__ptrParameterValueStruct[nNbValue].Value.Value, 0x00, nLength+1);
        strncpy(tmp_cwmp__GetParameterValuesResponse.ParameterList->__ptrParameterValueStruct[nNbValue].Value.Value, GetLANTotalBytesReceived(""), nLength);
        tmp_cwmp__GetParameterValuesResponse.ParameterList->__ptrParameterValueStruct[nNbValue].Value.__type = SOAP_TYPE_string;
	 }
	 else if(strstr(_tmp_cwmp__GetParameterValuesRequest.ParameterNames.__ptrstring[nNbValue ] ,"InternetGatewayDevice.LANDevice.1.WLANConfiguration.1.X_CT-COM_LAN-TotalBytesSent")!= 0)
	 {
        tmp_cwmp__GetParameterValuesResponse.ParameterList->__ptrParameterValueStruct[nNbValue].Name = _tmp_cwmp__GetParameterValuesRequest.ParameterNames.__ptrstring[nNbValue];

        nLength = 20;
  
        tmp_cwmp__GetParameterValuesResponse.ParameterList->__ptrParameterValueStruct[nNbValue].Value.Value = (char*)malloc(nLength+1);
        memset(tmp_cwmp__GetParameterValuesResponse.ParameterList->__ptrParameterValueStruct[nNbValue].Value.Value, 0x00, nLength+1);
        strncpy(tmp_cwmp__GetParameterValuesResponse.ParameterList->__ptrParameterValueStruct[nNbValue].Value.Value, GetLANTotalBytesSent(""), nLength);
        tmp_cwmp__GetParameterValuesResponse.ParameterList->__ptrParameterValueStruct[nNbValue].Value.__type = SOAP_TYPE_string;
	 }
	else if(strstr(_tmp_cwmp__GetParameterValuesRequest.ParameterNames.__ptrstring[nNbValue ] ,"InternetGatewayDevice.LANDevice.1.WLANConfiguration.1.X_CT-COM_WAN-TotalBytesReceived")!= 0)
	 {
        tmp_cwmp__GetParameterValuesResponse.ParameterList->__ptrParameterValueStruct[nNbValue].Name = _tmp_cwmp__GetParameterValuesRequest.ParameterNames.__ptrstring[nNbValue];

        nLength = 20;
  
        tmp_cwmp__GetParameterValuesResponse.ParameterList->__ptrParameterValueStruct[nNbValue].Value.Value = (char*)malloc(nLength+1);
        memset(tmp_cwmp__GetParameterValuesResponse.ParameterList->__ptrParameterValueStruct[nNbValue].Value.Value, 0x00, nLength+1);
        strncpy(tmp_cwmp__GetParameterValuesResponse.ParameterList->__ptrParameterValueStruct[nNbValue].Value.Value, GetWANTotalBytesReceived(""), nLength);
        tmp_cwmp__GetParameterValuesResponse.ParameterList->__ptrParameterValueStruct[nNbValue].Value.__type = SOAP_TYPE_string;
	 }
	else if(strstr(_tmp_cwmp__GetParameterValuesRequest.ParameterNames.__ptrstring[nNbValue ] ,"InternetGatewayDevice.LANDevice.1.WLANConfiguration.1.X_CT-COM_WAN-TotalBytesSent")!= 0)
	 {
        tmp_cwmp__GetParameterValuesResponse.ParameterList->__ptrParameterValueStruct[nNbValue].Name = _tmp_cwmp__GetParameterValuesRequest.ParameterNames.__ptrstring[nNbValue];

        nLength = 20;
  
        tmp_cwmp__GetParameterValuesResponse.ParameterList->__ptrParameterValueStruct[nNbValue].Value.Value = (char*)malloc(nLength+1);
        memset(tmp_cwmp__GetParameterValuesResponse.ParameterList->__ptrParameterValueStruct[nNbValue].Value.Value, 0x00, nLength+1);
        strncpy(tmp_cwmp__GetParameterValuesResponse.ParameterList->__ptrParameterValueStruct[nNbValue].Value.Value, GetWANTotalBytesSent(""), nLength);
        tmp_cwmp__GetParameterValuesResponse.ParameterList->__ptrParameterValueStruct[nNbValue].Value.__type = SOAP_TYPE_string;
	 }
	else if(strstr(_tmp_cwmp__GetParameterValuesRequest.ParameterNames.__ptrstring[nNbValue ] ,"InternetGatewayDevice.LANDevice.1.WLANConfiguration.1.X_CT-COM_Stat.AuthenticationUser")!= 0)
	 {
        tmp_cwmp__GetParameterValuesResponse.ParameterList->__ptrParameterValueStruct[nNbValue].Name = _tmp_cwmp__GetParameterValuesRequest.ParameterNames.__ptrstring[nNbValue];

        nLength = 20;
  
        tmp_cwmp__GetParameterValuesResponse.ParameterList->__ptrParameterValueStruct[nNbValue].Value.Value = (char*)malloc(nLength+1);
        memset(tmp_cwmp__GetParameterValuesResponse.ParameterList->__ptrParameterValueStruct[nNbValue].Value.Value, 0x00, nLength+1);
        strncpy(tmp_cwmp__GetParameterValuesResponse.ParameterList->__ptrParameterValueStruct[nNbValue].Value.Value, GetAuthenticationUser(""), nLength);
        tmp_cwmp__GetParameterValuesResponse.ParameterList->__ptrParameterValueStruct[nNbValue].Value.__type = SOAP_TYPE_string;
	 }
	else if(strstr(_tmp_cwmp__GetParameterValuesRequest.ParameterNames.__ptrstring[nNbValue ] ,"InternetGatewayDevice.LANDevice.1.WLANConfiguration.1.X_CT-COM_LAN-TotalPacketsReceived")!= 0)
	{
 	tmp_cwmp__GetParameterValuesResponse.ParameterList->__ptrParameterValueStruct[nNbValue].Name = _tmp_cwmp__GetParameterValuesRequest.ParameterNames.__ptrstring[nNbValue];

        nLength = 20;
  
        tmp_cwmp__GetParameterValuesResponse.ParameterList->__ptrParameterValueStruct[nNbValue].Value.Value = (char*)malloc(nLength+1);
        memset(tmp_cwmp__GetParameterValuesResponse.ParameterList->__ptrParameterValueStruct[nNbValue].Value.Value, 0x00, nLength+1);
        strncpy(tmp_cwmp__GetParameterValuesResponse.ParameterList->__ptrParameterValueStruct[nNbValue].Value.Value, GetLANTotalPacketsReceived(""),nLength);
        tmp_cwmp__GetParameterValuesResponse.ParameterList->__ptrParameterValueStruct[nNbValue].Value.__type = SOAP_TYPE_string;
	
	}
	else if(strstr(_tmp_cwmp__GetParameterValuesRequest.ParameterNames.__ptrstring[nNbValue ] ,"InternetGatewayDevice.LANDevice.1.WLANConfiguration.1.X_CT-COM_LAN-TotalPacketsSent")!= 0)
	{
 	tmp_cwmp__GetParameterValuesResponse.ParameterList->__ptrParameterValueStruct[nNbValue].Name = _tmp_cwmp__GetParameterValuesRequest.ParameterNames.__ptrstring[nNbValue];

        nLength = 20;
  
        tmp_cwmp__GetParameterValuesResponse.ParameterList->__ptrParameterValueStruct[nNbValue].Value.Value = (char*)malloc(nLength+1);
        memset(tmp_cwmp__GetParameterValuesResponse.ParameterList->__ptrParameterValueStruct[nNbValue].Value.Value, 0x00, nLength+1);
        strncpy(tmp_cwmp__GetParameterValuesResponse.ParameterList->__ptrParameterValueStruct[nNbValue].Value.Value, GetLANTotalPacketsSent(""),nLength);
        tmp_cwmp__GetParameterValuesResponse.ParameterList->__ptrParameterValueStruct[nNbValue].Value.__type = SOAP_TYPE_string;
	
	}
	else if(strstr(_tmp_cwmp__GetParameterValuesRequest.ParameterNames.__ptrstring[nNbValue ] ,"InternetGatewayDevice.LANDevice.1.WLANConfiguration.1.X_CT-COM_WAN-TotalPacketsReceived")!= 0)
	{
 	tmp_cwmp__GetParameterValuesResponse.ParameterList->__ptrParameterValueStruct[nNbValue].Name = _tmp_cwmp__GetParameterValuesRequest.ParameterNames.__ptrstring[nNbValue];

        nLength = 20;
  
        tmp_cwmp__GetParameterValuesResponse.ParameterList->__ptrParameterValueStruct[nNbValue].Value.Value = (char*)malloc(nLength+1);
        memset(tmp_cwmp__GetParameterValuesResponse.ParameterList->__ptrParameterValueStruct[nNbValue].Value.Value, 0x00, nLength+1);
        strncpy(tmp_cwmp__GetParameterValuesResponse.ParameterList->__ptrParameterValueStruct[nNbValue].Value.Value, GetWANTotalPacketsReceived(""), nLength);
        tmp_cwmp__GetParameterValuesResponse.ParameterList->__ptrParameterValueStruct[nNbValue].Value.__type = SOAP_TYPE_string;
	
	}
	else if(strstr(_tmp_cwmp__GetParameterValuesRequest.ParameterNames.__ptrstring[nNbValue ] ,"InternetGatewayDevice.LANDevice.1.WLANConfiguration.1.X_CT-COM_WAN-TotalPacketsSent")!= 0)
	{
 	tmp_cwmp__GetParameterValuesResponse.ParameterList->__ptrParameterValueStruct[nNbValue].Name = _tmp_cwmp__GetParameterValuesRequest.ParameterNames.__ptrstring[nNbValue];

        nLength = 20;
  
        tmp_cwmp__GetParameterValuesResponse.ParameterList->__ptrParameterValueStruct[nNbValue].Value.Value = (char*)malloc(nLength+1);
        memset(tmp_cwmp__GetParameterValuesResponse.ParameterList->__ptrParameterValueStruct[nNbValue].Value.Value, 0x00, nLength+1);
        strncpy(tmp_cwmp__GetParameterValuesResponse.ParameterList->__ptrParameterValueStruct[nNbValue].Value.Value, GetWANTotalPacketsSent(""), nLength);
        tmp_cwmp__GetParameterValuesResponse.ParameterList->__ptrParameterValueStruct[nNbValue].Value.__type = SOAP_TYPE_string;
	
	}
		else if(strstr(_tmp_cwmp__GetParameterValuesRequest.ParameterNames.__ptrstring[nNbValue ] ,"InternetGatewayDevice.DeviceInfo.SoftwareVersion")!= 0)
	{
 	tmp_cwmp__GetParameterValuesResponse.ParameterList->__ptrParameterValueStruct[nNbValue].Name = _tmp_cwmp__GetParameterValuesRequest.ParameterNames.__ptrstring[nNbValue];

        nLength = 20;
  
        tmp_cwmp__GetParameterValuesResponse.ParameterList->__ptrParameterValueStruct[nNbValue].Value.Value = (char*)malloc(nLength+1);
        memset(tmp_cwmp__GetParameterValuesResponse.ParameterList->__ptrParameterValueStruct[nNbValue].Value.Value, 0x00, nLength+1);
        strncpy(tmp_cwmp__GetParameterValuesResponse.ParameterList->__ptrParameterValueStruct[nNbValue].Value.Value, GetSoftwareVersion(""), nLength);
        tmp_cwmp__GetParameterValuesResponse.ParameterList->__ptrParameterValueStruct[nNbValue].Value.__type = SOAP_TYPE_string;
	
	}
	else if(strstr(_tmp_cwmp__GetParameterValuesRequest.ParameterNames.__ptrstring[nNbValue ] ,"InternetGatewayDevice.LANDevice.1.WLANConfiguration.1.SSID")!= 0)
	{
 	tmp_cwmp__GetParameterValuesResponse.ParameterList->__ptrParameterValueStruct[nNbValue].Name = _tmp_cwmp__GetParameterValuesRequest.ParameterNames.__ptrstring[nNbValue];

        nLength = 20;
  
        tmp_cwmp__GetParameterValuesResponse.ParameterList->__ptrParameterValueStruct[nNbValue].Value.Value = (char*)malloc(nLength+1);
        memset(tmp_cwmp__GetParameterValuesResponse.ParameterList->__ptrParameterValueStruct[nNbValue].Value.Value, 0x00, nLength+1);
        strncpy(tmp_cwmp__GetParameterValuesResponse.ParameterList->__ptrParameterValueStruct[nNbValue].Value.Value, GetSSID(""), nLength);
        tmp_cwmp__GetParameterValuesResponse.ParameterList->__ptrParameterValueStruct[nNbValue].Value.__type = SOAP_TYPE_string;
	
	}
	else if(strstr(_tmp_cwmp__GetParameterValuesRequest.ParameterNames.__ptrstring[nNbValue ] ,"InternetGatewayDevice.LANDevice.1.WLANConfiguration.1.X_CT-COM_ReceiveNoise")!= 0)
	{
 	tmp_cwmp__GetParameterValuesResponse.ParameterList->__ptrParameterValueStruct[nNbValue].Name = _tmp_cwmp__GetParameterValuesRequest.ParameterNames.__ptrstring[nNbValue];

        nLength = 20;
  
        tmp_cwmp__GetParameterValuesResponse.ParameterList->__ptrParameterValueStruct[nNbValue].Value.Value = (char*)malloc(nLength+1);
        memset(tmp_cwmp__GetParameterValuesResponse.ParameterList->__ptrParameterValueStruct[nNbValue].Value.Value, 0x00, nLength+1);
        strncpy(tmp_cwmp__GetParameterValuesResponse.ParameterList->__ptrParameterValueStruct[nNbValue].Value.Value, GetReceiveNoise(""), nLength);
        tmp_cwmp__GetParameterValuesResponse.ParameterList->__ptrParameterValueStruct[nNbValue].Value.__type = SOAP_TYPE_string;
	
	}
	else if(strstr(_tmp_cwmp__GetParameterValuesRequest.ParameterNames.__ptrstring[nNbValue ] ,"InternetGatewayDevice.DeviceInfo.X_CT-COM_CPU")!= 0)
	{
 	tmp_cwmp__GetParameterValuesResponse.ParameterList->__ptrParameterValueStruct[nNbValue].Name = _tmp_cwmp__GetParameterValuesRequest.ParameterNames.__ptrstring[nNbValue];

        nLength = 20;
  
        tmp_cwmp__GetParameterValuesResponse.ParameterList->__ptrParameterValueStruct[nNbValue].Value.Value = (char*)malloc(nLength+1);
        memset(tmp_cwmp__GetParameterValuesResponse.ParameterList->__ptrParameterValueStruct[nNbValue].Value.Value, 0x00, nLength+1);
        strncpy(tmp_cwmp__GetParameterValuesResponse.ParameterList->__ptrParameterValueStruct[nNbValue].Value.Value, GetCPU(""), nLength);
        tmp_cwmp__GetParameterValuesResponse.ParameterList->__ptrParameterValueStruct[nNbValue].Value.__type = SOAP_TYPE_string;
	
	}
	else if(strstr(_tmp_cwmp__GetParameterValuesRequest.ParameterNames.__ptrstring[nNbValue ] ,"InternetGatewayDevice.DeviceInfo.X_CT-COM_Temp")!= 0)
	{
 	tmp_cwmp__GetParameterValuesResponse.ParameterList->__ptrParameterValueStruct[nNbValue].Name = _tmp_cwmp__GetParameterValuesRequest.ParameterNames.__ptrstring[nNbValue];

        nLength = 20;
  
        tmp_cwmp__GetParameterValuesResponse.ParameterList->__ptrParameterValueStruct[nNbValue].Value.Value = (char*)malloc(nLength+1);
        memset(tmp_cwmp__GetParameterValuesResponse.ParameterList->__ptrParameterValueStruct[nNbValue].Value.Value, 0x00, nLength+1);
        strncpy(tmp_cwmp__GetParameterValuesResponse.ParameterList->__ptrParameterValueStruct[nNbValue].Value.Value, GetTemp(""), nLength);
        tmp_cwmp__GetParameterValuesResponse.ParameterList->__ptrParameterValueStruct[nNbValue].Value.__type = SOAP_TYPE_string;
	
	}
	else if(strstr(_tmp_cwmp__GetParameterValuesRequest.ParameterNames.__ptrstring[nNbValue ] ,"InternetGatewayDevice.LANDevice.1.WLANConfiguration.1.X_CT-COM_Stat.LAN-TotalBytesReceived")!= 0)
	{
 	tmp_cwmp__GetParameterValuesResponse.ParameterList->__ptrParameterValueStruct[nNbValue].Name = _tmp_cwmp__GetParameterValuesRequest.ParameterNames.__ptrstring[nNbValue];

        nLength = 20;
  
        tmp_cwmp__GetParameterValuesResponse.ParameterList->__ptrParameterValueStruct[nNbValue].Value.Value = (char*)malloc(nLength+1);
        memset(tmp_cwmp__GetParameterValuesResponse.ParameterList->__ptrParameterValueStruct[nNbValue].Value.Value, 0x00, nLength+1);
        strncpy(tmp_cwmp__GetParameterValuesResponse.ParameterList->__ptrParameterValueStruct[nNbValue].Value.Value, GetCTLANTotalBytesReceived(""), nLength);
        tmp_cwmp__GetParameterValuesResponse.ParameterList->__ptrParameterValueStruct[nNbValue].Value.__type = SOAP_TYPE_string;
	
	}
	else if(strstr(_tmp_cwmp__GetParameterValuesRequest.ParameterNames.__ptrstring[nNbValue ] ,"InternetGatewayDevice.LANDevice.1.WLANConfiguration.1.X_CT-COM_Stat.LAN-TotalBytesSent")!= 0)
	{
 	tmp_cwmp__GetParameterValuesResponse.ParameterList->__ptrParameterValueStruct[nNbValue].Name = _tmp_cwmp__GetParameterValuesRequest.ParameterNames.__ptrstring[nNbValue];

        nLength = 20;
  
        tmp_cwmp__GetParameterValuesResponse.ParameterList->__ptrParameterValueStruct[nNbValue].Value.Value = (char*)malloc(nLength+1);
        memset(tmp_cwmp__GetParameterValuesResponse.ParameterList->__ptrParameterValueStruct[nNbValue].Value.Value, 0x00, nLength+1);
        strncpy(tmp_cwmp__GetParameterValuesResponse.ParameterList->__ptrParameterValueStruct[nNbValue].Value.Value, GetCTLANTotalBytesSent(""), nLength);
        tmp_cwmp__GetParameterValuesResponse.ParameterList->__ptrParameterValueStruct[nNbValue].Value.__type = SOAP_TYPE_string;
	
	}
	else if(strstr(_tmp_cwmp__GetParameterValuesRequest.ParameterNames.__ptrstring[nNbValue ] ,"InternetGatewayDevice.LANDevice.1.WLANConfiguration.1.X_CT-COM_Stat.WAN-TotalBytesReceived")!= 0)
	{
 	tmp_cwmp__GetParameterValuesResponse.ParameterList->__ptrParameterValueStruct[nNbValue].Name = _tmp_cwmp__GetParameterValuesRequest.ParameterNames.__ptrstring[nNbValue];

        nLength = 20;
  
        tmp_cwmp__GetParameterValuesResponse.ParameterList->__ptrParameterValueStruct[nNbValue].Value.Value = (char*)malloc(nLength+1);
        memset(tmp_cwmp__GetParameterValuesResponse.ParameterList->__ptrParameterValueStruct[nNbValue].Value.Value, 0x00, nLength+1);
        strncpy(tmp_cwmp__GetParameterValuesResponse.ParameterList->__ptrParameterValueStruct[nNbValue].Value.Value, GetCTWANTotalBytesReceived(""), nLength);
        tmp_cwmp__GetParameterValuesResponse.ParameterList->__ptrParameterValueStruct[nNbValue].Value.__type = SOAP_TYPE_string;
	
	}
		else if(strstr(_tmp_cwmp__GetParameterValuesRequest.ParameterNames.__ptrstring[nNbValue ] ,"InternetGatewayDevice.LANDevice.1.WLANConfiguration.1.X_CT-COM_Stat.WAN-TotalBytesSent")!= 0)
	{
 	tmp_cwmp__GetParameterValuesResponse.ParameterList->__ptrParameterValueStruct[nNbValue].Name = _tmp_cwmp__GetParameterValuesRequest.ParameterNames.__ptrstring[nNbValue];

        nLength = 20;
  
        tmp_cwmp__GetParameterValuesResponse.ParameterList->__ptrParameterValueStruct[nNbValue].Value.Value = (char*)malloc(nLength+1);
        memset(tmp_cwmp__GetParameterValuesResponse.ParameterList->__ptrParameterValueStruct[nNbValue].Value.Value, 0x00, nLength+1);
        strncpy(tmp_cwmp__GetParameterValuesResponse.ParameterList->__ptrParameterValueStruct[nNbValue].Value.Value, GetCTWANTotalBytesSent(""), nLength);
        tmp_cwmp__GetParameterValuesResponse.ParameterList->__ptrParameterValueStruct[nNbValue].Value.__type = SOAP_TYPE_string;
	
	}
	else if(strstr(_tmp_cwmp__GetParameterValuesRequest.ParameterNames.__ptrstring[nNbValue ] ,"InternetGatewayDevice.LANDevice.1.WLANConfiguration.1.X_CT-COM_Stat.LAN-PacketsError")!= 0)
	{
 	tmp_cwmp__GetParameterValuesResponse.ParameterList->__ptrParameterValueStruct[nNbValue].Name = _tmp_cwmp__GetParameterValuesRequest.ParameterNames.__ptrstring[nNbValue];

        nLength = 20;
  
        tmp_cwmp__GetParameterValuesResponse.ParameterList->__ptrParameterValueStruct[nNbValue].Value.Value = (char*)malloc(nLength+1);
        memset(tmp_cwmp__GetParameterValuesResponse.ParameterList->__ptrParameterValueStruct[nNbValue].Value.Value, 0x00, nLength+1);
        strncpy(tmp_cwmp__GetParameterValuesResponse.ParameterList->__ptrParameterValueStruct[nNbValue].Value.Value, GetLANPacketsError(""), nLength);
        tmp_cwmp__GetParameterValuesResponse.ParameterList->__ptrParameterValueStruct[nNbValue].Value.__type = SOAP_TYPE_string;
	
	}
	else if(strstr(_tmp_cwmp__GetParameterValuesRequest.ParameterNames.__ptrstring[nNbValue ] ,"InternetGatewayDevice.LANDevice.1.WLANConfiguration.1.X_CT-COM_Stat.WAN-PacketsError")!= 0)
	{
 	tmp_cwmp__GetParameterValuesResponse.ParameterList->__ptrParameterValueStruct[nNbValue].Name = _tmp_cwmp__GetParameterValuesRequest.ParameterNames.__ptrstring[nNbValue];

        nLength = 20;
  
        tmp_cwmp__GetParameterValuesResponse.ParameterList->__ptrParameterValueStruct[nNbValue].Value.Value = (char*)malloc(nLength+1);
        memset(tmp_cwmp__GetParameterValuesResponse.ParameterList->__ptrParameterValueStruct[nNbValue].Value.Value, 0x00, nLength+1);
        strncpy(tmp_cwmp__GetParameterValuesResponse.ParameterList->__ptrParameterValueStruct[nNbValue].Value.Value, GetWANPacketsError(""), nLength);
        tmp_cwmp__GetParameterValuesResponse.ParameterList->__ptrParameterValueStruct[nNbValue].Value.__type = SOAP_TYPE_string;
	
	}
	else if(strstr(_tmp_cwmp__GetParameterValuesRequest.ParameterNames.__ptrstring[nNbValue ] ,"InternetGatewayDevice.LANDevice.1.WLANConfiguration.1.X_CT-COM_Stat.AskPass")!= 0)
	{
 	tmp_cwmp__GetParameterValuesResponse.ParameterList->__ptrParameterValueStruct[nNbValue].Name = _tmp_cwmp__GetParameterValuesRequest.ParameterNames.__ptrstring[nNbValue];

        nLength = 20;
  
        tmp_cwmp__GetParameterValuesResponse.ParameterList->__ptrParameterValueStruct[nNbValue].Value.Value = (char*)malloc(nLength+1);
        memset(tmp_cwmp__GetParameterValuesResponse.ParameterList->__ptrParameterValueStruct[nNbValue].Value.Value, 0x00, nLength+1);
        strncpy(tmp_cwmp__GetParameterValuesResponse.ParameterList->__ptrParameterValueStruct[nNbValue].Value.Value, GetAskPass(""), nLength);
        tmp_cwmp__GetParameterValuesResponse.ParameterList->__ptrParameterValueStruct[nNbValue].Value.__type = SOAP_TYPE_string;
	
	}
	else if(strstr(_tmp_cwmp__GetParameterValuesRequest.ParameterNames.__ptrstring[nNbValue ] ,"InternetGatewayDevice.LANDevice.1.WLANConfiguration.1.X_CT-COM_Stat.ResponsePass")!= 0)
	{
 	tmp_cwmp__GetParameterValuesResponse.ParameterList->__ptrParameterValueStruct[nNbValue].Name = _tmp_cwmp__GetParameterValuesRequest.ParameterNames.__ptrstring[nNbValue];

        nLength = 20;
  
        tmp_cwmp__GetParameterValuesResponse.ParameterList->__ptrParameterValueStruct[nNbValue].Value.Value = (char*)malloc(nLength+1);
        memset(tmp_cwmp__GetParameterValuesResponse.ParameterList->__ptrParameterValueStruct[nNbValue].Value.Value, 0x00, nLength+1);
        strncpy(tmp_cwmp__GetParameterValuesResponse.ParameterList->__ptrParameterValueStruct[nNbValue].Value.Value, GetResponsePass(""), nLength);
        tmp_cwmp__GetParameterValuesResponse.ParameterList->__ptrParameterValueStruct[nNbValue].Value.__type = SOAP_TYPE_string;
	
	}
	else if(strstr(_tmp_cwmp__GetParameterValuesRequest.ParameterNames.__ptrstring[nNbValue ] ,"InternetGatewayDevice.LANDevice.1.WLANConfiguration.1.X_CT-COM_Stat.SuccessPass")!= 0)
	{
 	tmp_cwmp__GetParameterValuesResponse.ParameterList->__ptrParameterValueStruct[nNbValue].Name = _tmp_cwmp__GetParameterValuesRequest.ParameterNames.__ptrstring[nNbValue];

        nLength = 20;
  
        tmp_cwmp__GetParameterValuesResponse.ParameterList->__ptrParameterValueStruct[nNbValue].Value.Value = (char*)malloc(nLength+1);
        memset(tmp_cwmp__GetParameterValuesResponse.ParameterList->__ptrParameterValueStruct[nNbValue].Value.Value, 0x00, nLength+1);
        strncpy(tmp_cwmp__GetParameterValuesResponse.ParameterList->__ptrParameterValueStruct[nNbValue].Value.Value, GetSuccessPass(""), nLength);
        tmp_cwmp__GetParameterValuesResponse.ParameterList->__ptrParameterValueStruct[nNbValue].Value.__type = SOAP_TYPE_string;
	
	}
	else if(strstr(_tmp_cwmp__GetParameterValuesRequest.ParameterNames.__ptrstring[nNbValue ] ,"InternetGatewayDevice.LANDevice.1.WLANConfiguration.1.AssociatedDevice.1.X_CT-COM_ReceiveRate")!= 0)
	{
 	tmp_cwmp__GetParameterValuesResponse.ParameterList->__ptrParameterValueStruct[nNbValue].Name = _tmp_cwmp__GetParameterValuesRequest.ParameterNames.__ptrstring[nNbValue];

        nLength = 20;
  
        tmp_cwmp__GetParameterValuesResponse.ParameterList->__ptrParameterValueStruct[nNbValue].Value.Value = (char*)malloc(nLength+1);
        memset(tmp_cwmp__GetParameterValuesResponse.ParameterList->__ptrParameterValueStruct[nNbValue].Value.Value, 0x00, nLength+1);
        strncpy(tmp_cwmp__GetParameterValuesResponse.ParameterList->__ptrParameterValueStruct[nNbValue].Value.Value, GetCTReceiveRate(""), nLength);
        tmp_cwmp__GetParameterValuesResponse.ParameterList->__ptrParameterValueStruct[nNbValue].Value.__type = SOAP_TYPE_string;
	
	}
	else if(strstr(_tmp_cwmp__GetParameterValuesRequest.ParameterNames.__ptrstring[nNbValue ] ,"InternetGatewayDevice.LANDevice.1.WLANConfiguration.1.AssociatedDevice.1.X_CT-COM_SendRate")!= 0)
	{
 	tmp_cwmp__GetParameterValuesResponse.ParameterList->__ptrParameterValueStruct[nNbValue].Name = _tmp_cwmp__GetParameterValuesRequest.ParameterNames.__ptrstring[nNbValue];

        nLength = 20;
  
        tmp_cwmp__GetParameterValuesResponse.ParameterList->__ptrParameterValueStruct[nNbValue].Value.Value = (char*)malloc(nLength+1);
        memset(tmp_cwmp__GetParameterValuesResponse.ParameterList->__ptrParameterValueStruct[nNbValue].Value.Value, 0x00, nLength+1);
        strncpy(tmp_cwmp__GetParameterValuesResponse.ParameterList->__ptrParameterValueStruct[nNbValue].Value.Value,  GetCTSendRate(""), nLength);
        tmp_cwmp__GetParameterValuesResponse.ParameterList->__ptrParameterValueStruct[nNbValue].Value.__type = SOAP_TYPE_string;
	
	}
	else if(strstr(_tmp_cwmp__GetParameterValuesRequest.ParameterNames.__ptrstring[nNbValue ] ,"InternetGatewayDevice.LANDevice.1.WLANConfiguration.1.AssociatedDevice.1.X_CT-COM_TotalBytes")!= 0)
	{
 	tmp_cwmp__GetParameterValuesResponse.ParameterList->__ptrParameterValueStruct[nNbValue].Name = _tmp_cwmp__GetParameterValuesRequest.ParameterNames.__ptrstring[nNbValue];

        nLength = 20;
  
        tmp_cwmp__GetParameterValuesResponse.ParameterList->__ptrParameterValueStruct[nNbValue].Value.Value = (char*)malloc(nLength+1);
        memset(tmp_cwmp__GetParameterValuesResponse.ParameterList->__ptrParameterValueStruct[nNbValue].Value.Value, 0x00, nLength+1);
        strncpy(tmp_cwmp__GetParameterValuesResponse.ParameterList->__ptrParameterValueStruct[nNbValue].Value.Value, GetTotalBytes(""), nLength);
        tmp_cwmp__GetParameterValuesResponse.ParameterList->__ptrParameterValueStruct[nNbValue].Value.__type = SOAP_TYPE_string;
	
	}
	 else 
	 {
	  // create and send request to data model repository
      CreateRequest(gdatamodel_address, gId, CONST_FUNCTIONGETVALUE, _tmp_cwmp__GetParameterValuesRequest.ParameterNames.__ptrstring[nNbValue], NULL);

      // wait for response message (semaphore) from data model
      dprintf(LOG_LEVEL3, "......ParseGetParamValues_and_SendResponse, wait for response from data model\n");
      do {
        if ((ret_sem_wait=sem_wait(&S_response)) != 0)  // SEMAPHORE lock
            dprintf(LOG_LEVEL1, "ERROR on sem_wait !! \n");
      }
      while ((ret_sem_wait != 0) && (errno == EINTR));

      // Fill response structure
      dprintf(LOG_LEVEL1, "--->%d\n",gflagResponse_OK);
      if ((gflagResponse_OK == 1) && (gListParamValues != NULL))
      {
        tmp_cwmp__GetParameterValuesResponse.ParameterList->__ptrParameterValueStruct[nNbValue].Name = _tmp_cwmp__GetParameterValuesRequest.ParameterNames.__ptrstring[nNbValue];

        nLength = strlen(gListParamValues->szValue);
  
        tmp_cwmp__GetParameterValuesResponse.ParameterList->__ptrParameterValueStruct[nNbValue].Value.Value = (char*)malloc(nLength+1);
        memset(tmp_cwmp__GetParameterValuesResponse.ParameterList->__ptrParameterValueStruct[nNbValue].Value.Value, 0x00, nLength+1);
        strncpy(tmp_cwmp__GetParameterValuesResponse.ParameterList->__ptrParameterValueStruct[nNbValue].Value.Value, gListParamValues->szValue, nLength);
        tmp_cwmp__GetParameterValuesResponse.ParameterList->__ptrParameterValueStruct[nNbValue].Value.__type = SOAP_TYPE_string;
      }
      else {
        dprintf(LOG_LEVEL3, "---> Value not found\n");
		FaultFlag=1;  /* update by wangjr 091124*/
		strcpy(tmp_parameter,_tmp_cwmp__GetParameterValuesRequest.ParameterNames.__ptrstring[nNbValue]);
		tmp_cwmp__FaultResponse.detail.cwmp__Fault.FaultCode=9003;
		tmp_cwmp__FaultResponse.detail.cwmp__Fault.FaultString="Invalid arguments";
		tmp_cwmp__FaultResponse.detail.cwmp__Fault.GetParameterValuesFault.ParameterName=tmp_parameter;
		tmp_cwmp__FaultResponse.detail.cwmp__Fault.GetParameterValuesFault.FaultCode=9005;
		tmp_cwmp__FaultResponse.detail.cwmp__Fault.GetParameterValuesFault.FaultString="Not a valid arguments";	
        tmp_cwmp__GetParameterValuesResponse.ParameterList->__ptrParameterValueStruct[nNbValue].Value.Value = (char*)malloc(2);
        memset(tmp_cwmp__GetParameterValuesResponse.ParameterList->__ptrParameterValueStruct[nNbValue].Value.Value, 0x00, 2);
        sprintf(tmp_cwmp__GetParameterValuesResponse.ParameterList->__ptrParameterValueStruct[nNbValue].Value.Value, " ");
        tmp_cwmp__GetParameterValuesResponse.ParameterList->__ptrParameterValueStruct[nNbValue].Value.__type = SOAP_TYPE_string;
      }
      }
    }

    // send GetParameterValues response   //update by wangjr 091124 
	if(FaultFlag == 0)
	{
		dprintf(LOG_LEVEL3, "......send GetParameterValues Response.\n");
		nRet = send_GetParameterValuesResponse(_soap, _endpoint, &tmp_cwmp__GetParameterValuesResponse);
	}
	else
	{
		dprintf(LOG_LEVEL3, "......send send_FaultResponse .\n");

		nRet = send_FaultResponse(_soap, _endpoint, &tmp_cwmp__FaultResponse);//add by wangjr20091112
	}

    // Free String Resources
    for (nNbValue=0; nNbValue < nNbParam; nNbValue++)  //update by wangjr 091124
    {
      /*if (tmp_cwmp__GetParameterValuesResponse.ParameterList->__ptrParameterValueStruct[nNbValue].Name != NULL)
          free(tmp_cwmp__GetParameterValuesResponse.ParameterList->__ptrParameterValueStruct[nNbValue].Name);*/
      //if ((nParameterType == SOAP_TYPE_string) && (tmp_cwmp__GetParameterValuesResponse.ParameterList->__ptrParameterValueStruct != NULL) && (tmp_cwmp__GetParameterValuesResponse.ParameterList->__ptrParameterValueStruct[nNbValue].__AnytypeStruct.Value != NULL))
      if ((tmp_cwmp__GetParameterValuesResponse.ParameterList->__ptrParameterValueStruct != NULL) && (tmp_cwmp__GetParameterValuesResponse.ParameterList->__ptrParameterValueStruct[nNbValue].Value.Value != NULL))
      {
        free(tmp_cwmp__GetParameterValuesResponse.ParameterList->__ptrParameterValueStruct[nNbValue].Value.Value);
        tmp_cwmp__GetParameterValuesResponse.ParameterList->__ptrParameterValueStruct[nNbValue].Value.Value = NULL;
      }
    }

    // free GetParameterValuesResponse.ParameterList->__ptrParameterValueStruct resources
    if (tmp_cwmp__GetParameterValuesResponse.ParameterList->__ptrParameterValueStruct != NULL)
    {
      free(tmp_cwmp__GetParameterValuesResponse.ParameterList->__ptrParameterValueStruct);
      tmp_cwmp__GetParameterValuesResponse.ParameterList->__ptrParameterValueStruct = NULL;
    }
  }
  FaultFlag=0; //add by wangjr
  if (tmp_cwmp__GetParameterValuesResponse.ParameterList != NULL)
  {
    free(tmp_cwmp__GetParameterValuesResponse.ParameterList);
    tmp_cwmp__GetParameterValuesResponse.ParameterList = NULL;
  }
  return nRet;
}

/*********************************************/
/* ParseDownload_and_SendResponse            */
/* Parse Download request and send           */
/* response                                  */
/*********************************************/
int ParseDownload_and_SendResponse(struct soap *_soap, const char *_endpoint, struct cwmp__Download _tmp_cwmp__DownloadRequest)
{
int nRet = 0;
struct cwmp__DownloadResponse tmp_cwmp__DownloadResponse;

  dprintf(LOG_LEVEL3, "......parse Download Request.\n");

  dprintf(LOG_LEVEL3, "......Download request  CommandKey: %s\n", _tmp_cwmp__DownloadRequest.CommandKey);
  dprintf(LOG_LEVEL3, "......Download request  FileType: %s\n", _tmp_cwmp__DownloadRequest.FileType);
  dprintf(LOG_LEVEL3, "......Download request  URL: %s\n", _tmp_cwmp__DownloadRequest.URL);
  dprintf(LOG_LEVEL3, "......Download request  Username: %s\n", _tmp_cwmp__DownloadRequest.Username);
  dprintf(LOG_LEVEL3, "......Download request  Password: %s\n", _tmp_cwmp__DownloadRequest.Password);
  dprintf(LOG_LEVEL3, "......Download request  FileSize : %d\n", _tmp_cwmp__DownloadRequest.FileSize);
  dprintf(LOG_LEVEL3, "......Download request  TargetFileName : %s\n", _tmp_cwmp__DownloadRequest.TargetFileName);
  dprintf(LOG_LEVEL3, "......Download request  DelaySeconds : %d\n", _tmp_cwmp__DownloadRequest.DelaySeconds);
  dprintf(LOG_LEVEL3, "......Download request  SuccessURL : %s\n", _tmp_cwmp__DownloadRequest.SuccessURL);
  dprintf(LOG_LEVEL3, "......Download request  FailureURL : %s\n", _tmp_cwmp__DownloadRequest.FailureURL);

  dprintf(LOG_LEVEL3, "......send Download Response.\n");
  soap_default_cwmp__DownloadResponse(_soap, &tmp_cwmp__DownloadResponse);

/*  update by wangjr 091124 */
#if 0
  tmp_cwmp__DownloadResponse.Status = 0;   // indicate that TransferComplete will be sent later !!
  tmp_cwmp__DownloadResponse.StartTime = time(NULL);
  tmp_cwmp__DownloadResponse.CompleteTime = time(NULL);
#endif

  if (getTransferComplete())
  {
	  tmp_cwmp__DownloadResponse.Status = 0;
	  tmp_cwmp__DownloadResponse.StartTime = download_endtime;
      tmp_cwmp__DownloadResponse.CompleteTime = download_endover;
  }
  else
  { 
	  tmp_cwmp__DownloadResponse.Status = 1;
	  tmp_cwmp__DownloadResponse.StartTime = time(NULL);
      tmp_cwmp__DownloadResponse.CompleteTime = time(NULL);
	  gDownloadResponse_status=1;
  }
/*  update by wangjr 091124 */
  nRet = send_DownloadResponse(_soap, _endpoint, &tmp_cwmp__DownloadResponse);

  return nRet;
}

/*********************************************/
/* ParseGetParamNames_and_SendResponse      */
/* Parse GetParameterNames request and send */
/* response                                  */
/*********************************************/

//#define MAX_STRNAME 256
/*
static char *parameter_names_level0 = "InternetGatewayDevice.";

#define LEVEL2_COUNT 4
static char *parameter_names_level1[]={
	"ManagementServer.",
	"DeviceInfo.",
	"ManagementServer.",
	"LANDevice.",
};

#define LEVEL3_COUNT 1
static char *parameter_names_level2 = "SoftwareVersion";*/

int ParseGetParameterNames_and_SendResponse(struct soap *_soap, char *_endpoint, struct cwmp__GetParameterNames tmp_cwmp__GetParameterNamesRequest)
{
	//printf("%s\n", tmp_cwmp__GetParameterNamesRequest.ParameterPath);
	//return -1;
	int nRet = 0;
	int nNbParam = 0;
	int nCount = 0;
	char szParamValue[BUFFERSIZE];
	struct cwmp__GetParameterNamesResponse tmp_cwmp__GetParameterNamesResponse;
	int ret_sem_wait;
	LIST_PARAM_VALUE *iter_paramlist;

	tmp_cwmp__GetParameterNamesResponse.ParameterList = (struct __ParameterInfoList *)malloc(sizeof(struct __ParameterInfoList));
	tmp_cwmp__GetParameterNamesResponse.ParameterList->__size = 0;
	tmp_cwmp__GetParameterNamesResponse.ParameterList->__ptrParameterInfoStruct = NULL;
	
	dprintf(LOG_LEVEL3, "......parse GetParameterNames Request.\n");

	/*hxh:  create and send request to data model repository */
	dprintf(LOG_LEVEL1,"create get request to datamodel\n");

    if(strlen(tmp_cwmp__GetParameterNamesRequest.ParameterPath)!= 0)
    {

		CreateGetParameterNamesRequest( gdatamodel_address, gId, CONST_FUNCTIONGETNAME,
									    tmp_cwmp__GetParameterNamesRequest.ParameterPath,
									    (tmp_cwmp__GetParameterNamesRequest.NextLevel==1)? "TRUE":"FALSE" );
		
		dprintf(LOG_LEVEL3, "......ParseGetParamValues_and_SendResponse, wait for response from data model\n");
	    do {
	      if ((ret_sem_wait=sem_wait(&S_response)) != 0)  // SEMAPHORE lock
	          dprintf(LOG_LEVEL1, "ERROR on sem_wait !! \n");
	    }
	    while ((ret_sem_wait != 0) && (errno == EINTR));

    }
	else
	{
		if (gListParamValues != NULL)
		{
			free_queue(gListParamValues);
			gListParamValues = NULL;
		}

       if(tmp_cwmp__GetParameterNamesRequest.NextLevel==1)
       {
				if (gListParamValues == NULL)
			{
				gListParamValues = (LIST_PARAM_VALUE *)malloc(sizeof(LIST_PARAM_VALUE));
				memset(gListParamValues, 0x00, sizeof(LIST_PARAM_VALUE));
				strcpy(gListParamValues->szParameter, "InternetGatewayDevice.");
				strcpy(gListParamValues->szValue, "FALSE");
			}
       	}
	}
	dprintf(LOG_LEVEL3, "===============================\n");
	print(gListParamValues);
	dprintf(LOG_LEVEL3, "--->%d\n", gflagResponse_OK);
	dprintf(LOG_LEVEL3, "===============================\n");
	// Fill response structure
	if ((gflagResponse_OK == 1) && (gListParamValues != NULL))
	{
		for(iter_paramlist=gListParamValues; iter_paramlist; iter_paramlist=iter_paramlist->next)
		{	
			nNbParam++;
		}
		tmp_cwmp__GetParameterNamesResponse.ParameterList->__ptrParameterInfoStruct = (struct cwmp__ParameterInfoStruct *)malloc(nNbParam*sizeof(struct cwmp__ParameterInfoStruct));
		tmp_cwmp__GetParameterNamesResponse.ParameterList->__size = nNbParam;
		nCount = 0;
		for(iter_paramlist=gListParamValues; iter_paramlist; iter_paramlist = iter_paramlist->next)
		{
			tmp_cwmp__GetParameterNamesResponse.ParameterList->__ptrParameterInfoStruct[nCount].Name = (char *)malloc(BUFFERSIZE*sizeof(char));
			strcpy(tmp_cwmp__GetParameterNamesResponse.ParameterList->__ptrParameterInfoStruct[nCount].Name, iter_paramlist->szParameter);
			if(!strcmp(iter_paramlist->szValue, "FALSE"))
				tmp_cwmp__GetParameterNamesResponse.ParameterList->__ptrParameterInfoStruct[nCount++].Writable = 0;
			else
				tmp_cwmp__GetParameterNamesResponse.ParameterList->__ptrParameterInfoStruct[nCount++].Writable = 1;
		}
	}

	// send GetParameterNames response
	nRet = send_GetParameterNamesResponse(_soap, _endpoint, &tmp_cwmp__GetParameterNamesResponse);

	//free tmp
	if( tmp_cwmp__GetParameterNamesResponse.ParameterList )
	{
		if(tmp_cwmp__GetParameterNamesResponse.ParameterList->__ptrParameterInfoStruct)
		{
			for(nCount=0; nCount<nNbParam; nCount++)
			{
				if(tmp_cwmp__GetParameterNamesResponse.ParameterList->__ptrParameterInfoStruct[nCount].Name)
					free(tmp_cwmp__GetParameterNamesResponse.ParameterList->__ptrParameterInfoStruct[nCount].Name);
				tmp_cwmp__GetParameterNamesResponse.ParameterList->__ptrParameterInfoStruct[nCount].Name = NULL;
			}
			free(tmp_cwmp__GetParameterNamesResponse.ParameterList->__ptrParameterInfoStruct);
			tmp_cwmp__GetParameterNamesResponse.ParameterList->__ptrParameterInfoStruct = NULL;
		}
		free(tmp_cwmp__GetParameterNamesResponse.ParameterList);
		tmp_cwmp__GetParameterNamesResponse.ParameterList = NULL;
	}

	return nRet;
}

/*********************************************/
/* ParseUpload_and_SendResponse            */
/* Parse Upload request and send           */
/* response                                  */
/*********************************************/
int ParseUpload_and_SendResponse(struct soap *_soap, const char *_endpoint, struct cwmp__Upload _tmp_cwmp__UploadRequest)
{
	int nRet = 0;
	struct cwmp__UploadResponse tmp_cwmp__UploadResponse;

  dprintf(LOG_LEVEL3, "......parse Upload Request.\n");

  dprintf(LOG_LEVEL3, "......Upload request  CommandKey: %s\n", _tmp_cwmp__UploadRequest.CommandKey);
  dprintf(LOG_LEVEL3, "......Upload request  FileType: %s\n", _tmp_cwmp__UploadRequest.FileType);
  dprintf(LOG_LEVEL3, "......Upload request  URL: %s\n", _tmp_cwmp__UploadRequest.URL);
  dprintf(LOG_LEVEL3, "......Upload request  Username: %s\n", _tmp_cwmp__UploadRequest.Username);
  dprintf(LOG_LEVEL3, "......Upload request  Password: %s\n", _tmp_cwmp__UploadRequest.Password);
  dprintf(LOG_LEVEL3, "......Upload request  FileSize : %d\n", _tmp_cwmp__UploadRequest.FileSize);
  dprintf(LOG_LEVEL3, "......Upload request  TargetFileName : %s\n", _tmp_cwmp__UploadRequest.TargetFileName);
  //dprintf(LOG_LEVEL3, "......Download request  DelaySeconds : %d\n", _tmp_cwmp__DownloadRequest.DelaySeconds);
  //dprintf(LOG_LEVEL3, "......Download request  SuccessURL : %s\n", _tmp_cwmp__DownloadRequest.SuccessURL);
  //dprintf(LOG_LEVEL3, "......Download request  FailureURL : %s\n", _tmp_cwmp__DownloadRequest.FailureURL);

  dprintf(LOG_LEVEL3, "......send Upload Response.\n");
  //soap_default_cwmp__UploadResponse(_soap, &tmp_cwmp__UploadResponse);
/* add by wangjr 091124  */
  #if 0  //add by wangjr
  if(strstr(_tmp_cwmp__UploadRequest.FileType,"Vendor Configuration File")!=NULL)
  {
    gUploadResponse_status=1;
  	tmp_cwmp__UploadResponse.Status = 1;   // indicate that TransferComplete will be sent later !!
  }
  else
  {
    gUploadResponse_status=0;
  	tmp_cwmp__UploadResponse.Status = 0;   // indicate that TransferComplete will be sent later !!
  }
/* add by wangjr 091124  */

  tmp_cwmp__UploadResponse.StartTime = time(NULL);
  tmp_cwmp__UploadResponse.CompleteTime = time(NULL);
#endif 
/*************************update by wangjr 091203************************************/
  if (getTransferComplete())
  {
	  tmp_cwmp__UploadResponse.Status = 0;
	  tmp_cwmp__UploadResponse.StartTime = download_endtime;//download_endtime;
      tmp_cwmp__UploadResponse.CompleteTime = download_endtime;//download_endover;
  }
  else
  { 
	  tmp_cwmp__UploadResponse.Status = 1;
	  tmp_cwmp__UploadResponse.StartTime = time(NULL);
      tmp_cwmp__UploadResponse.CompleteTime = time(NULL);
	  gUploadResponse_status=1;
  }

  nRet = send_UploadResponse(_soap, _endpoint, &tmp_cwmp__UploadResponse);

  return nRet;
}

/*********************************************/
/* ParseSetParamValues_and_SendResponse      */
/* Parse SetParameterValues request and send */
/* response                                  */
/*********************************************/
int ParseSetParamValues_and_SendResponse(struct soap *_soap, const char *_endpoint, struct cwmp__SetParameterValues _tmp_cwmp__SetParameterValuesRequest, int *_Status)
{
int nRet = 0;
int nNbParam = 0;
int nNbValue;
char szParamValue[BUFFERSIZE];
int *ptrI;
char *ptrC;
struct cwmp__SetParameterValuesResponse tmp_cwmp__SetParameterValuesResponse;
int ret_sem_wait;
struct cwmp__SetFaultResponse tmp_cwmp__SetFaultResponse = {"Client","CWMP fault",};//add by wangjr 20091123

int FaultFlag=0;  //add by wangjr
  dprintf(LOG_LEVEL3, "......parse SetParameterValues Request.\n");
  nNbParam = _tmp_cwmp__SetParameterValuesRequest.ParameterList.__size;
  dprintf(LOG_LEVEL3, "......nNbParam : %d\n", nNbParam);
  if (nNbParam > 0)
  {
    // Parse SetParameterValues Request
    for (nNbValue=0; nNbValue < nNbParam; nNbValue++)
    {
      memset(szParamValue, 0x00, BUFFERSIZE);
      if ((_tmp_cwmp__SetParameterValuesRequest.ParameterList.__ptrParameterValueStruct[nNbValue].Value.__type == SOAP_TYPE_unsignedInt) || (_tmp_cwmp__SetParameterValuesRequest.ParameterList.__ptrParameterValueStruct[nNbValue].Value.__type == SOAP_TYPE_int))
      {
        ptrI = (int*)_tmp_cwmp__SetParameterValuesRequest.ParameterList.__ptrParameterValueStruct[nNbValue].Value.Value;
        sprintf(szParamValue, "%d", (int) *ptrI);
      }
      else if (_tmp_cwmp__SetParameterValuesRequest.ParameterList.__ptrParameterValueStruct[nNbValue].Value.__type == SOAP_TYPE_xsd__boolean)
      {
        ptrC = (char*)_tmp_cwmp__SetParameterValuesRequest.ParameterList.__ptrParameterValueStruct[nNbValue].Value.Value;
        sprintf(szParamValue, "%d", (int) *ptrC);
      }
      else if (_tmp_cwmp__SetParameterValuesRequest.ParameterList.__ptrParameterValueStruct[nNbValue].Value.__type == SOAP_TYPE_xsd__dateTime)
      {
        ptrI = (int*)_tmp_cwmp__SetParameterValuesRequest.ParameterList.__ptrParameterValueStruct[nNbValue].Value.Value;
        dprintf(LOG_LEVEL3, "......dateTime value : %d", (int) *ptrI);
        szFormatLocalTime((int)*ptrI, szParamValue);
      }
      else
      {
        sprintf(szParamValue, "%s", (char*) _tmp_cwmp__SetParameterValuesRequest.ParameterList.__ptrParameterValueStruct[nNbValue].Value.Value);
         strncpy(szParamValue,(char*) _tmp_cwmp__SetParameterValuesRequest.ParameterList.__ptrParameterValueStruct[nNbValue].Value.Value,strlen((char*) _tmp_cwmp__SetParameterValuesRequest.ParameterList.__ptrParameterValueStruct[nNbValue].Value.Value)); //add by wangjr 091124
      }

      dprintf(LOG_LEVEL3, "......SetParameterValues request param %d : %s, val : %s\n", nNbValue+1, _tmp_cwmp__SetParameterValuesRequest.ParameterList.__ptrParameterValueStruct[nNbValue].Name, szParamValue);

      dprintf(LOG_LEVEL3, "......send request for %s to data model\n", _tmp_cwmp__SetParameterValuesRequest.ParameterList.__ptrParameterValueStruct[nNbValue].Name);

    if( _tmp_cwmp__SetParameterValuesRequest.ParameterList.__ptrParameterValueStruct[nNbValue].Name != NULL)
    {
      // create and send request to data model repository
      CreateRequest(gdatamodel_address, gId, CONST_FUNCTIONSETVALUE, _tmp_cwmp__SetParameterValuesRequest.ParameterList.__ptrParameterValueStruct[nNbValue].Name, szParamValue);

      // wait for response message (semaphore) from data model
      dprintf(LOG_LEVEL3, "......ParseSetParamValues_and_SendResponse, wait for response from data model\n");
      do {
        if ((ret_sem_wait=sem_wait(&S_response)) != 0)  // SEMAPHORE lock
            dprintf(LOG_LEVEL1, "ERROR on sem_wait !! \n");
      }
      while ((ret_sem_wait != 0) && (errno == EINTR));
	    //add by wangjr 091117 
		strncpy(gActiveParameter[nNbValue],_tmp_cwmp__SetParameterValuesRequest.ParameterList.__ptrParameterValueStruct[nNbValue].Name,strlen(_tmp_cwmp__SetParameterValuesRequest.ParameterList.__ptrParameterValueStruct[nNbValue].Name));
		dprintf(LOG_LEVEL3, "......send SetParameterValues Response. gActiveParameter[nNbValue] =%s\n",gActiveParameter[nNbValue]);
    }
	else
	{
        dprintf(LOG_LEVEL1 ,"---> Parameter not found\n");
		FaultFlag=1;
		tmp_cwmp__SetFaultResponse.detail.cwmp__Fault.FaultCode=9003;
		tmp_cwmp__SetFaultResponse.detail.cwmp__Fault.FaultString="Invalid arguments";
		tmp_cwmp__SetFaultResponse.detail.cwmp__Fault.SetParameterValuesFault.ParameterName="The Parameter is NULL";
		tmp_cwmp__SetFaultResponse.detail.cwmp__Fault.SetParameterValuesFault.FaultCode=9005;
		tmp_cwmp__SetFaultResponse.detail.cwmp__Fault.SetParameterValuesFault.FaultString="Not a valid arguments";	
	}
    	}
  }
  else
  {
       dprintf(LOG_LEVEL1 ,"---> Parameter not found\n");
		FaultFlag=1;
		tmp_cwmp__SetFaultResponse.detail.cwmp__Fault.FaultCode=9003;
		tmp_cwmp__SetFaultResponse.detail.cwmp__Fault.FaultString="Invalid arguments";
		tmp_cwmp__SetFaultResponse.detail.cwmp__Fault.SetParameterValuesFault.ParameterName="The Parameter is NULL";
		tmp_cwmp__SetFaultResponse.detail.cwmp__Fault.SetParameterValuesFault.FaultCode=9005;
		tmp_cwmp__SetFaultResponse.detail.cwmp__Fault.SetParameterValuesFault.FaultString="Not a valid arguments";	
       FaultFlag =1;
  }
/* update by wangjr 091124 */
 if(FaultFlag == 0)
 {
	dprintf(LOG_LEVEL3, "......send SetParameterValues Response.\n");
	soap_default_cwmp__SetParameterValuesResponse(_soap, &tmp_cwmp__SetParameterValuesResponse);

	tmp_cwmp__SetParameterValuesResponse.Status = _Status; 
  
	nRet = send_SetParameterValuesResponse(_soap, _endpoint, &tmp_cwmp__SetParameterValuesResponse);

  setActiveNotification(1);
 }
 else
 {
 	dprintf(LOG_LEVEL3, "......send send_FaultResponse .\n");
	nRet = send_SetFaultResponse(_soap, _endpoint, &tmp_cwmp__SetFaultResponse);//add by wangjr20091112
 }
  
  return nRet;
}

/*********************************************/
/* ParseDeleteObject_and_SendResponse  add by wangjr 091124             */
/* Parse DeleteObject request and send response    */
/*********************************************/
int ParseDeleteObject_and_SendResponse(struct soap *_soap, const char *_endpoint, struct cwmp__DeleteObject _tmp_cwmp__DeleteObjectRequest)
{
	int nRet = 0;
	struct cwmp__DeleteObjectResponse tmp_cwmp__DeleteObjectResponse;

	dprintf(LOG_LEVEL3,"\n\n_tmp_cwmp__DeleteObjectRequest.ParameterKey=%s\n\n",_tmp_cwmp__DeleteObjectRequest.ParameterKey);

	dprintf(LOG_LEVEL3,"\n\n_tmp_cwmp__DeleteObjectRequest.ObjectName=%s\n\n",_tmp_cwmp__DeleteObjectRequest.ObjectName );
	if (_tmp_cwmp__DeleteObjectRequest.ObjectName !=NULL)
	{
		  CreateRequest(gdatamodel_address, gId, CONST_FUNCTIONDELETEOBJECT, _tmp_cwmp__DeleteObjectRequest.ObjectName, NULL);
	}
  	dprintf(LOG_LEVEL2, "ParseDeleteObject_and_SendResponse.\n");

	tmp_cwmp__DeleteObjectResponse.Status= 0;   // indicate that TransferComplete will be sent later !!

  	nRet = send_DeleteObjectResponse(_soap, _endpoint, &tmp_cwmp__DeleteObjectResponse);
  	return nRet;
}
/*********************************************/
/* ParseAddObject_and_SendResponse       add by wangjr 091124        */
/* Parse AddObject request and send response    */
/*********************************************/
int ParseAddObject_and_SendResponse(struct soap *_soap, const char *_endpoint, struct cwmp__AddObject _tmp_cwmp__AddObjectRequest)
{	
	int nRet = 0;
	LIST_PARAM_VALUE *iter_list;
	struct cwmp__AddObjectResponse tmp_cwmp__AddObjectResponse;
	LIST_PARAM_VALUE *iter_tmplist;
	int ret_sem_wait;
	int tmpnum=0;

	 dprintf(LOG_LEVEL3,"\n\n_tmp_cwmp__AddObjectRequest.ParameterKey=%s\n\n",_tmp_cwmp__AddObjectRequest.ParameterKey);
	 
	 dprintf(LOG_LEVEL3,"\n\n_tmp_cwmp__AddObjectRequest.ObjectName=%s\n\n",_tmp_cwmp__AddObjectRequest.ObjectName );

	 if (_tmp_cwmp__AddObjectRequest.ObjectName !=NULL)
	 {
		   CreateRequest(gdatamodel_address, gId, CONST_FUNCTIONADDOBJECT, _tmp_cwmp__AddObjectRequest.ObjectName, NULL);
	 }
  	dprintf(LOG_LEVEL2, "ParseAddObject_and_SendResponse.\n");

      do {
        if ((ret_sem_wait=sem_wait(&S_response)) != 0)  // SEMAPHORE lock
            dprintf(LOG_LEVEL1, "ERROR on sem_wait !! \n");
      }
      while ((ret_sem_wait != 0) && (errno == EINTR));

    if(gListParamValues !=NULL)
    {
		for(iter_tmplist=gListParamValues;iter_tmplist;iter_tmplist=iter_tmplist->next)
		{		
			tmpnum=atoi(iter_tmplist->szValue);
		}
    }


	tmp_cwmp__AddObjectResponse.InstanceNumber=tmpnum;
    tmp_cwmp__AddObjectResponse.Status= 0;   // indicate that TransferComplete will be sent later !!

  	nRet = send_AddObjectResponse(_soap, _endpoint, &tmp_cwmp__AddObjectResponse);

  	return nRet;
}



int ParseSetParameterAttributes_and_SendResponse(struct soap *_soap, const char *_endpoint, struct cwmp__SetParameterAttributes _tmp_cwmp__SetParameterAttributesRequest)
{	
	int nRet = 0;
	LIST_PARAM_VALUE *iter_list;
	struct cwmp__SetParameterAttributesResponse tmp_cwmp__SetParameterAttributesResponse;
	LIST_PARAM_VALUE *iter_tmplist;
	int ret_sem_wait;
	char tmpvalue[5]={0};
	int nNbParam=0;
	int nNbValue=0;
	struct cwmp__SetFaultResponse tmp_cwmp__SetFaultResponse = {"Client","CWMP fault",};//add by wangjr 20091209
	int FaultFlag=0;  //add by wangjr 20091209
   nNbParam = _tmp_cwmp__SetParameterAttributesRequest.ParameterList.__size;

  dprintf(LOG_LEVEL3, "......nNbParam : %d\n", nNbParam);  
  if(nNbParam>0)
  {
	for (nNbValue=0; nNbValue < nNbParam; nNbValue++)
   
		{	
			dprintf(LOG_LEVEL3, "_tmp_cwmp__SetParameterAttributesRequest.ParameterList.__ptrSetParameterAttributesStruct[nNbValue].Name =%s \n",_tmp_cwmp__SetParameterAttributesRequest.ParameterList.__ptrSetParameterAttributesStruct[nNbValue].Name);		
			dprintf(LOG_LEVEL3, "_tmp_cwmp__SetParameterAttributesRequest.ParameterList.__ptrSetParameterAttributesStruct[nNbValue].Notification =%d\n",_tmp_cwmp__SetParameterAttributesRequest.ParameterList.__ptrSetParameterAttributesStruct[nNbValue].Notification);    
			dprintf(LOG_LEVEL3, "_tmp_cwmp__SetParameterAttributesRequest.ParameterList.__ptrSetParameterAttributesStruct[nNbValue].NotificationChange =%d\n",_tmp_cwmp__SetParameterAttributesRequest.ParameterList.__ptrSetParameterAttributesStruct[nNbValue].NotificationChange);    
			dprintf(LOG_LEVEL3, "_tmp_cwmp__SetParameterAttributesRequest.ParameterList.__ptrSetParameterAttributesStruct[nNbValue].AccessListChange =%d\n",_tmp_cwmp__SetParameterAttributesRequest.ParameterList.__ptrSetParameterAttributesStruct[nNbValue].AccessListChange);    

	        if(_tmp_cwmp__SetParameterAttributesRequest.ParameterList.__ptrSetParameterAttributesStruct[nNbValue].Name != NULL)
			{
				sprintf(tmpvalue,"%d",_tmp_cwmp__SetParameterAttributesRequest.ParameterList.__ptrSetParameterAttributesStruct[nNbValue].Notification);

				if(_tmp_cwmp__SetParameterAttributesRequest.ParameterList.__ptrSetParameterAttributesStruct[nNbValue].NotificationChange != 0)
		        {
					CreateRequest(gdatamodel_address, gId, CONST_FUNCTIONSETPARAMETERATTRIBUTES,_tmp_cwmp__SetParameterAttributesRequest.ParameterList.__ptrSetParameterAttributesStruct[nNbValue].Name,tmpvalue);

				}
			}
			else
			{
					dprintf(LOG_LEVEL1 ,"---> Parameter not found\n");
					FaultFlag=1;
					tmp_cwmp__SetFaultResponse.detail.cwmp__Fault.FaultCode=9003;
					tmp_cwmp__SetFaultResponse.detail.cwmp__Fault.FaultString="Invalid arguments";
					tmp_cwmp__SetFaultResponse.detail.cwmp__Fault.SetParameterValuesFault.ParameterName="The Parameter is NULL";
					tmp_cwmp__SetFaultResponse.detail.cwmp__Fault.SetParameterValuesFault.FaultCode=9005;
					tmp_cwmp__SetFaultResponse.detail.cwmp__Fault.SetParameterValuesFault.FaultString="Not a valid arguments";	
			}

		}
	}
  else
  {

		   dprintf(LOG_LEVEL1 ,"---> Parameter not found\n");
			FaultFlag=1;
			tmp_cwmp__SetFaultResponse.detail.cwmp__Fault.FaultCode=9003;
			tmp_cwmp__SetFaultResponse.detail.cwmp__Fault.FaultString="Invalid arguments";
			tmp_cwmp__SetFaultResponse.detail.cwmp__Fault.SetParameterValuesFault.ParameterName="The Parameter is NULL";
			tmp_cwmp__SetFaultResponse.detail.cwmp__Fault.SetParameterValuesFault.FaultCode=9005;
			tmp_cwmp__SetFaultResponse.detail.cwmp__Fault.SetParameterValuesFault.FaultString="Not a valid arguments";	
		   FaultFlag =1;

  }

   if(FaultFlag == 0)
 {
	   dprintf(LOG_LEVEL2, "-------------SendResponse--------------------------\n");
	   nRet = send_SetParameterAttributesResponse(_soap, _endpoint, &tmp_cwmp__SetParameterAttributesResponse);

 }
 else
 {
 	dprintf(LOG_LEVEL3, "......send send_FaultResponse .\n");
	nRet = send_SetFaultResponse(_soap, _endpoint, &tmp_cwmp__SetFaultResponse);//add by wangjr20091112
 }

  	return nRet;
}
/*********************************************/
/* ParseAddObject_and_SendResponse     add by wangjr 091124         */
/* Parse AddObject request and send response    */
/*********************************************/
int ParseGetParameterAttributes_and_SendResponse(struct soap *_soap, const char *_endpoint, struct cwmp__GetParameterAttributes _tmp_cwmp__GetParameterAttributesRequest)
{	
	int nNbValue = 0;
	int nRet = 0;
	int nNbParam;
	struct cwmp__GetParameterAttributesResponse tmp_cwmp__GetParameterAttributesResponse;
	int nLength;
	int ret_sem_wait;
	// test if nb parameter(s) higher than 0
	
	nNbParam = _tmp_cwmp__GetParameterAttributesRequest.ParameterNames.__size;

	dprintf(LOG_LEVEL3, "---> allocates memory for GetParameterAttributes Response\n");
	// memory allocation for response
	tmp_cwmp__GetParameterAttributesResponse.ParameterList = (struct __ParameterList *)malloc(sizeof(struct __ParameterList));
	memset(tmp_cwmp__GetParameterAttributesResponse.ParameterList, 0x00, sizeof(struct __ParameterList));
	dprintf(LOG_LEVEL1, "\n------------nNbParam=%d--->\n",nNbParam);

	 tmp_cwmp__GetParameterAttributesResponse.ParameterList->__size = nNbParam;
	 
	if (nNbParam > 0)
	{
		tmp_cwmp__GetParameterAttributesResponse.ParameterList->__ptrParameterAttributeStruct = (struct cwmp__ParameterAttributeStruct *)malloc(nNbParam*sizeof(struct cwmp__ParameterAttributeStruct));
		for (nNbValue=0; nNbValue < nNbParam; nNbValue++)
		{
			dprintf(LOG_LEVEL3, "......send request for %s to data model\n", _tmp_cwmp__GetParameterAttributesRequest.ParameterNames.__ptrstring[nNbValue]);
			// create and send request to data model repository
			CreateRequest(gdatamodel_address, gId, CONST_FUNCTIONGETPARAMETERATTRIBUTES, _tmp_cwmp__GetParameterAttributesRequest.ParameterNames.__ptrstring[nNbValue], NULL);
		
			// wait for response message (semaphore) from data model
			dprintf(LOG_LEVEL3, "......ParseGetParameterAttributes_and_SendResponse, wait for response from data model\n");
			do {
			  if ((ret_sem_wait=sem_wait(&S_response)) != 0)  // SEMAPHORE lock
				  dprintf(LOG_LEVEL1, "ERROR on sem_wait !! \n");
			}
			while ((ret_sem_wait != 0) && (errno == EINTR));
			// Fill response structure
			dprintf(LOG_LEVEL3, "--->%d\n",gflagResponse_OK);
			if ((gflagResponse_OK == 1) && (gListParamValues != NULL))
			{
				
			  tmp_cwmp__GetParameterAttributesResponse.ParameterList->__ptrParameterAttributeStruct[nNbValue].Name = _tmp_cwmp__GetParameterAttributesRequest.ParameterNames.__ptrstring[nNbValue];	
			  if(strstr(gListParamValues->szValue,"ACTIVE")!=NULL)
			  {
				tmp_cwmp__GetParameterAttributesResponse.ParameterList->__ptrParameterAttributeStruct[nNbValue].Notification=2;
			  }
			  else if(strstr(gListParamValues->szValue,"PASSIVE")!=NULL)
			  {
			  	tmp_cwmp__GetParameterAttributesResponse.ParameterList->__ptrParameterAttributeStruct[nNbValue].Notification=1;
			  }
			  else
			  {
			  	tmp_cwmp__GetParameterAttributesResponse.ParameterList->__ptrParameterAttributeStruct[nNbValue].Notification=0;
			  }
			  //strncpy(tmp_cwmp__GetParameterAttributesResponse.ParameterList->__ptrParameterAttributeStruct[nNbValue].notification, gListParamValues->szValue, nLength);
				tmp_cwmp__GetParameterAttributesResponse.ParameterList->__ptrParameterAttributeStruct[nNbValue].AccessList = "Subscriber";				
				dprintf(LOG_LEVEL3, "\n--->tmp_cwmp__GetParameterAttributesResponse.ParameterList->__ptrParameterAttributeStruct[nNbValue].Name=%s\n",tmp_cwmp__GetParameterAttributesResponse.ParameterList->__ptrParameterAttributeStruct[nNbValue].Name);
				dprintf(LOG_LEVEL3, "\n--->tmp_cwmp__GetParameterAttributesResponse.ParameterList->__ptrParameterAttributeStruct[nNbValue].Notification=%d\n",tmp_cwmp__GetParameterAttributesResponse.ParameterList->__ptrParameterAttributeStruct[nNbValue].Notification);
				dprintf(LOG_LEVEL3, "\n--->tmp_cwmp__GetParameterAttributesResponse.ParameterList->__ptrParameterAttributeStruct[nNbValue].AccessList=%s\n",tmp_cwmp__GetParameterAttributesResponse.ParameterList->__ptrParameterAttributeStruct[nNbValue].AccessList);

			}
			else 
			{
			 ;; //tmp_cwmp__GetParameterAttributesResponse.ParameterList->__ptrParameterAttributeStruct[nNbValue].Notification=0;
			  //tmp_cwmp__GetParameterAttributesResponse.ParameterList->__ptrParameterAttributeStruct[nNbValue].AccessList ="Subscriber";
			}
	  }

		// send GetParameterValues response
		dprintf(LOG_LEVEL3, "......send GetParameterAttributes Response.\n");
		nRet = send_GetParameterAttributesResponse(_soap, _endpoint, &tmp_cwmp__GetParameterAttributesResponse);

		  // free GetParameterValuesResponse.ParameterList->__ptrParameterValueStruct resources
		if (tmp_cwmp__GetParameterAttributesResponse.ParameterList->__ptrParameterAttributeStruct != NULL)
		{
			free(tmp_cwmp__GetParameterAttributesResponse.ParameterList->__ptrParameterAttributeStruct);
			tmp_cwmp__GetParameterAttributesResponse.ParameterList->__ptrParameterAttributeStruct = NULL;
		}
	}
	
	if (tmp_cwmp__GetParameterAttributesResponse.ParameterList != NULL)
	{
		free(tmp_cwmp__GetParameterAttributesResponse.ParameterList);
		tmp_cwmp__GetParameterAttributesResponse.ParameterList = NULL;
	}
	return nRet;

}

#ifdef __cplusplus
}
#endif
