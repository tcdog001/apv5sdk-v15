
/**********************************************************************************
* Copyright (c) 2008-2011  Beijing Autelan Technology Co. Ltd.
* All rights reserved.
*
* filename: cwmp.h
* description:  the header file for cwmp application  implementation of cwmp client
* 
*
* 
************************************************************************************/
#ifndef _CWMP_H
#define _CWMP_H

#include "StubH.h"


struct threadDownloadargs{
  char *szCmdKey;
  char *szURL;
  char *szUsername;
  char *szPassword;
  char *szFileType;
  uint FileSize;
  char *szTargetFileName;
  uint DelaySeconds;

  char *szProxyHost;
  uint ProxyPort;
  char *szProxyUserId;
  char *szProxyPasswd;

  char *szCWMPAddress;
};

struct threadUploadargs{
  char *szCmdKey;
  char *szURL;
  char *szUsername;
  char *szPassword;
  char *szFileType;
  uint FileSize;
  char *szTargetFileName;
  uint DelaySeconds;

  char *szProxyHost;
  uint ProxyPort;
  char *szProxyUserId;
  char *szProxyPasswd;

  char *szCWMPAddress;
};
/* function declaration */

// thread functions
void *cwmp_server_thread(void *soap);
void *reading_thread(void *soap);
void *connestreq_thread(void *_soap);
void *cwmp_download_thread(void *Downloadargs);
void *cwmp_upload_thread(void *Uploadargs);

// Notification : mutual exclusion functions
void setActiveNotification(short nVal);
short getActiveNotification();


int SubscribeSequence(struct soap *_soap, char *_soap_endpoint, int _nConfPort);


// Parse GetParameterValues request and send response
int ParseGetParameterNames_and_SendResponse(struct soap *_soap, char *_soap_endpoint, struct cwmp__GetParameterNames tmp_cwmp__GetParameterNamesRequest);

// Parse GetParameterValues request and send response
int ParseGetParamValues_and_SendResponse(struct soap *_soap, const char *_endpoint, struct cwmp__GetParameterValues _tmp_cwmp__GetParameterValuesRequest);

// Parse SetParameterValues request and send response
int ParseSetParamValues_and_SendResponse(struct soap *_soap, const char *_endpoint, struct cwmp__SetParameterValues _tmp_cwmp__SetParameterValuesRequest, int *_Status);

// Parse Download request and send response
int ParseDownload_and_SendResponse(struct soap *_soap, const char *_endpoint, struct cwmp__Download _tmp_cwmp__DownloadRequest);

// Parse Upload request and send response

int ParseUpload_and_SendResponse(struct soap *_soap, const char *_endpoint, struct cwmp__Upload _tmp_cwmp__UploadRequest);

//soap_wait_cwmp__FactoryReset
int soap_wait_cwmp__FactoryReset(struct soap *_soap, struct cwmp__FactoryReset *_a);			

//ParseFactoryReset_and_SendResponse
int ParseFactoryReset_and_SendResponse(struct soap *_soap, const char *_endpoint, struct cwmp__FactoryReset _tmp_cwmp__FactoryResetRequest);

time_t download_endtime;//add by wangjr 091124

time_t download_endover;//add by wangjr 091124

time_t download_starttime;//add by wangjr 091124

#endif // _CWMP_H
