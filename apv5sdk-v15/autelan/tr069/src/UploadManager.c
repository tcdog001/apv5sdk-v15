
/**********************************************************************************
* Copyright (c) 2008-2011  Beijing Autelan Technology Co. Ltd.
* All rights reserved.
*
* filename:  uploadmanager.c
* description:  implementation for uploadmanager application, use curl library for uploading file ,answers to cwmp process  
* 
*
* 
************************************************************************************/

#include <stdio.h>
#include <signal.h>

#include <curl/curl.h>

#include "const.h"
#include "UploadManager.h"

#include "StubH.h"
#include "Stub.nsmap"
#include "Fault.h"


struct Namespace *namespaces;

/***************************************************************************/
/***                                                                     ***/
/***                          PROGRAMME PRINCIPAL                        ***/
/***                                                                     ***/
/***                                                                     ***/
/*** ex. of execution:                                                   ***/
/*** ./uploadmanager --FileType:'1 Firmware Upgrade Image' --URL:'http://host:port'  --Username: --Password: --FileSize:1000 --TargetFileName: --DelaySeconds:1 --CWMPAddress:localhost:8093                            ***/
/***************************************************************************/ 

int nErrorCode; //add by wangjr 091202

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
// command line arguments
int nArg;
char *ptr;
char szFileType[64]="";
char szURL[256];
char szUsername[256]="";
char szPassword[256]="";
unsigned int FileSize=0;
char szTargetFileName[256]="";
unsigned int DelaySeconds;
char szCWMPAddress[256]="";

char szProxyHost[256]="";
unsigned int ProxyPort=0;
char szProxyUserId[256]="";
char szProxyPasswd[256]="";
char szTemp[256];
double UploadSize;
FILE *tmpfile;  //add by wangjr 091202



struct soap soap;
struct Namespace empty_namespaces[] = { { NULL } };

  dprintf(LOG_LEVEL1, "\nUPLOAD MANAGER ..execution\n\n");

  // signal handling
  signal(SIGPIPE, sigpipe_handle);
  signal(SIGINT, sigint_handle);

  // Command Line Arguments
  for (nArg = 1; nArg < argc; nArg++)
  {
    ptr = argv[nArg];
    if (strncmp(argv[nArg], "--CWMPAddress:", strlen("--CWMPAddress:")) == 0) {
      strcpy(szCWMPAddress, &ptr[strlen("--CWMPAddress:")]);
      dprintf(LOG_LEVEL1, "....CWMPAddress: %s\n", szCWMPAddress);
    }
    else if (strncmp(argv[nArg], "--FileType:", strlen("--FileType:")) == 0) {
      strcpy(szFileType, &ptr[strlen("--FileType:")]);
      dprintf(LOG_LEVEL1, "....FileType: %s\n", szFileType);
    }
    else if (strncmp(argv[nArg], "--URL:", strlen("--URL:")) == 0) {
      strcpy(szURL, &ptr[strlen("--URL:")]);
      dprintf(LOG_LEVEL1, "....URL: %s\n", szURL);
    }
    else if (strncmp(argv[nArg], "--Username:", strlen("--Username:")) == 0) {
      strcpy(szUsername, &ptr[strlen("--Username:")]);
      dprintf(LOG_LEVEL1, "....Username: %s\n", szUsername);
    }
    else if (strncmp(argv[nArg], "--Password:", strlen("--Password:")) == 0) {
      strcpy(szPassword, &ptr[strlen("--Password:")]);
      dprintf(LOG_LEVEL1, "....Password: %s\n", szPassword);
    }
    else if (strncmp(argv[nArg], "--FileSize:", strlen("--FileSize:")) == 0) {
      strcpy(szTemp, &ptr[strlen("--FileSize:")]);
      FileSize = atoi(szTemp);
      dprintf(LOG_LEVEL1, "....FileSize: %d\n", FileSize);
    }
    else if (strncmp(argv[nArg], "--TargetFileName:", strlen("--TargetFileName:")) == 0) {
      strcpy(szTargetFileName, &ptr[strlen("--TargetFileName:")]);
      dprintf(LOG_LEVEL1, "....TargetFileName: %s\n", szTargetFileName);
    }
    else if (strncmp(argv[nArg], "--DelaySeconds:", strlen("--DelaySeconds:")) == 0) {
      strcpy(szTemp, &ptr[strlen("--DelaySeconds:")]);
      DelaySeconds = atoi(szTemp);
      dprintf(LOG_LEVEL1, "....DelaySeconds: %d\n", DelaySeconds);
    }
    else if (strncmp(argv[nArg], "--ProxyHost:", strlen("--ProxyHost:")) == 0) {
      strcpy(szProxyHost, &ptr[strlen("--ProxyHost:")]);
      dprintf(LOG_LEVEL1, "....szProxyHost: %s\n", szProxyHost);
    }
    else if (strncmp(argv[nArg], "--ProxyPort:", strlen("--ProxyPort:")) == 0) {
      strcpy(szTemp, &ptr[strlen("--ProxyPort:")]);
      ProxyPort = atoi(szTemp);
      dprintf(LOG_LEVEL1, "....ProxyPort: %d\n", ProxyPort);
    }
    else if (strncmp(argv[nArg], "--ProxyUserId:", strlen("--ProxyUserId:")) == 0) {
      strcpy(szProxyUserId, &ptr[strlen("--ProxyUserId:")]);
      dprintf(LOG_LEVEL1, "....szProxyUserId: %s\n", szProxyUserId);
    }
    else if (strncmp(argv[nArg], "--ProxyPasswd:", strlen("--ProxyPasswd:")) == 0) {
      strcpy(szProxyPasswd, &ptr[strlen("--ProxyPasswd:")]);
      dprintf(LOG_LEVEL1, "....szProxyPasswd: %s\n", szProxyPasswd);
    }
  }

  UploadSize = upload_funct(szURL,FileSize, szTargetFileName, szUsername, szPassword, szProxyHost, ProxyPort, szProxyUserId, szProxyPasswd);

		if ((FileSize > 0) && ((unsigned long)UploadSize != FileSize))  //update by wangjr  091202 
		  nErrorCode = 9010;
		//else 
		  //nErrorCode = 0;
	  /*		add by wangjr 091202  */
		  tmpfile = fopen("/tmp/tr069", "w");
		  if(tmpfile != NULL)
		  {
			  fprintf(tmpfile, "%d",nErrorCode);
		  }
		  fclose(tmpfile);
	  /*		add by wangjr 091202   */


  if (szCWMPAddress && (strcmp(szCWMPAddress, "") != 0))
  {
  // RESPONSE to CWMP server
  // ATTENTION : Don't use SOAP_IO_KEEPALIVE during SUBSCRIBE message
  // because each SUBSCRIBE must correspond to one TCP connection
    soap_init2(&soap,SOAP_ENC_XML|SOAP_IO_LENGTH|SOAP_XML_INDENT, SOAP_ENC_XML|SOAP_IO_LENGTH|SOAP_XML_INDENT);
    soap.accept_timeout = 60;
    soap.max_keep_alive = 100; // max keep-alive sequence
    soap.recv_timeout = 60; // Timeout after 5 minutes stall on recv 
    soap.send_timeout = 60; // Timeout after 1 minute stall on send 
    soap_set_namespaces(&soap, empty_namespaces);
  
    soap_connect_command(&soap, SOAP_POST, szCWMPAddress, "");
  
    soap_element_begin_out(&soap, CONST_MESSAGE, 0, NULL);
    soap_element_begin_out(&soap, "NAME", 0, NULL);
    sprintf(szTemp, "%s", CONST_UPLOADCOMPLETE);
    soap_send_raw(&soap, szTemp, strlen(szTemp));
    soap_element_end_out(&soap, "NAME");
    soap_element_begin_out(&soap, "Value", 0, NULL);
    sprintf(szTemp, "%d", nErrorCode);
    soap_send_raw(&soap, szTemp, strlen(szTemp));
    soap_element_end_out(&soap, "Value");
    soap_element_end_out(&soap, CONST_MESSAGE);
  
    soap_end_send(&soap);
    soap_closesock(&soap);
  
    soap_destroy((struct soap*)&soap); // dealloc C++ data
    soap_end((struct soap*)&soap); // dealloc data and clean up
    soap_done((struct soap*)&soap); // detach soap struct
  }

  dprintf(LOG_LEVEL1, "size of data uploaded: %f\n", UploadSize);

  dprintf(LOG_LEVEL2, "UPLOAD MANAGER, END OF MAIN PROGRAM\n");
  return 0;
}



/* functions using CURL */

int read_to_file(void *buffer, size_t size, size_t nmemb, void *stream)
{
  FILE *infile = (FILE *)stream;
  if (!infile) {
    return -1;
  }
  return fread(buffer, size, nmemb, infile);
}

double upload_funct(const char *_url, 
	const unsigned int FileSize,
                            const char *_targetfilename, 
                            const char *_user,
                            const char *_passwd,
                            const char *_proxyhost,
                            const unsigned int _proxyport,
                            const char *_proxyuserid, 
                            const char *_proxyuserpwd)
{
	CURL *curl;
	CURLcode res;
	FILE *infile;
	char szFileName[ARGSIZE];
	char *ptrLastSlash;
	char szTemp[ARGSIZE];
	double UploadSize = 0;

	dprintf(LOG_LEVEL1, "upload_funct\n");
	dprintf(LOG_LEVEL1, "_url: %s\n", _url);
	dprintf(LOG_LEVEL1, "_targetfilename: %s\n", _targetfilename);

  curl_global_init(CURL_GLOBAL_DEFAULT);
  curl = curl_easy_init();
  if(curl)
  {
    if (!_targetfilename || (strcmp(_targetfilename, "") == 0))
    {
      if ((ptrLastSlash = strrchr(_url, '/')) != NULL)
      {
        if (++ptrLastSlash != NULL)
          strcpy(szFileName, ptrLastSlash);
      }
    }
    else
    {
      // set target filename
      strcpy(szFileName, _targetfilename);
    }

    dprintf(LOG_LEVEL1, "szFileName: %s\n", szFileName);
    dprintf(LOG_LEVEL1, "CURLE_OK: %d\n", CURLE_OK);

   infile = fopen(szFileName, "r");
    res = curl_easy_setopt(curl, CURLOPT_URL, _url);
	res = curl_easy_setopt(curl, CURLOPT_INFILESIZE , FileSize);
    dprintf(LOG_LEVEL1, "after curl_easy_setopt(curl, CURLOPT_URL, res: %d\n", res);

    if ((strcmp(_user, "") != 0) && (strcmp(_passwd, "") != 0))
	{
	  sprintf(szTemp, "%s:%s", _user, _passwd);
      res = curl_easy_setopt(curl, CURLOPT_USERPWD, szTemp);
      dprintf(LOG_LEVEL1, "after curl_easy_setopt(curl, CURLOPT_USERPWD, res: %d\n", res);
    }
    res = curl_easy_setopt(curl, CURLOPT_UPLOAD, 1);
    
    #if 0
    if (strcmp(_proxyhost, "") != 0)
	{
      res = curl_easy_setopt(curl, CURLOPT_PROXY, _proxyhost);
      dprintf(LOG_LEVEL1, "after curl_easy_setopt(curl, CURLOPT_PROXY, res: %d\n", res);

      res = curl_easy_setopt(curl, CURLOPT_PROXYPORT, _proxyport);
      dprintf(LOG_LEVEL1, "after curl_easy_setopt(curl, CURLOPT_PROXYPORT, res: %d\n", res);

      if ((strcmp(_proxyuserid, "") != 0) && (strcmp(_proxyuserpwd, "") != 0))
      {
        sprintf(szTemp, "%s:%s", _proxyuserid, _proxyuserpwd);
        res = curl_easy_setopt(curl, CURLOPT_PROXYUSERPWD, szTemp);
        dprintf(LOG_LEVEL1, "after curl_easy_setopt(curl, CURLOPT_PROXYUSERPWD, res: %d\n", res);
      }
    }
	#endif

    /* Define our callback to get called when there's data to be written */
    res = curl_easy_setopt(curl, CURLOPT_READFUNCTION , read_to_file);
    dprintf(LOG_LEVEL1, "after curl_easy_setopt(curl, CURLOPT_READFUNCTION , res: %d\n", res);

    /* Set a pointer to our struct to pass to the callback */
    res = curl_easy_setopt(curl, CURLOPT_READDATA , infile);
    dprintf(LOG_LEVEL1, "after curl_easy_setopt(curl, CURLOPT_READDATA , res: %d\n", res);

    res = curl_easy_perform(curl);
    dprintf(LOG_LEVEL1, "after curl_easy_perform, res: %d\n", res);
    if(res == 1)  //add by wangjr 091202
    {
		nErrorCode = 9013;

	}
	else if(res == 6 )
	{
		nErrorCode = 9015;
	}
	else if (res == 67)
	{
		nErrorCode = 9012;
	}
	else if (res == 68 || res == 19|| res == 7)
	{
	    dprintf(LOG_LEVEL3, " \n res == 68 || res == 19 \n");
		nErrorCode = 9016;
	}
	else
	{
	   ;;
	}
	
   // res = curl_easy_setopt(curl, CURLOPT_UPLOAD, 0);
	fclose(infile);

    res = curl_easy_getinfo(curl, CURLINFO_SIZE_UPLOAD, &UploadSize);
    dprintf(LOG_LEVEL1, "after curl_easy_getinfo(curl, CURLINFO_SIZE_UPLOAD, res: %d\n", res);

    curl_easy_cleanup(curl);
  }

  return UploadSize;
}
