/**********************************************************************************
* Copyright (c) 2008-2011  Beijing Autelan Technology Co. Ltd.
* All rights reserved.
*
* filename: ACSInterface.c
* description:  implementation for  using gSoap to perform SOAP RPC methods
* 
*
* 
************************************************************************************/
#include <stdio.h>

#include <pthread.h>
#include <semaphore.h>

#include "StubH.h"
#include "const.h"
#include "tools.h"
#include "ACSInterface.h"
#include "FileConf.h"
#include "messages.h"
#include "cwmp.h"  //add by wangjr

/*********************************************/
/*** print_gSoap_error                     ***/
/*** print error information               ***/
/*** argument :                            ***/
/***  Error code                           ***/
/*********************************************/
extern time_t download_endtime;//add by wangjr 091113
extern time_t download_endover;//add by wangjr 091113
void print_gSoap_error(int _nError)
{
  printf("\nERROR !! ");
  switch(_nError)
  {
    case SOAP_CLI_FAULT: printf("The service returned a client fault (SOAP 1.2 Sender fault)\n"); break;
    case SOAP_SVR_FAULT:  printf("The service returned a server fault (SOAP 1.2 Receiver fault)\n"); break;
    case SOAP_TAG_MISMATCH:  printf("An XML element didn't correspond to anything expected\n"); break;
    case SOAP_TYPE:  printf("An XML Schema type mismatch\n"); break;
    case SOAP_SYNTAX_ERROR:  printf("An XML syntax error occurred on the input\n"); break;
    case SOAP_NO_TAG:  printf("Begin of an element expected, but not found\n"); break;
    case SOAP_IOB:  printf("Array index out of bounds\n"); break;
    case SOAP_MUSTUNDERSTAND:  printf("An element needs to be ignored that need to be understood\n"); break;
    case SOAP_NAMESPACE:  printf("Namespace name mismatch (validation error)\n"); break;
    case SOAP_FATAL_ERROR:  printf("Internal error\n"); break;
    case SOAP_FAULT:  printf("An exception raised by the service\n"); break;
    case SOAP_NO_METHOD:  printf("Skeleton error: the skeleton cannot serve the method\n"); break;
    case SOAP_GET_METHOD:  printf("Unsupported HTTP GET\n"); break;
    case SOAP_EOM:  printf("Out of memory\n"); break;
    case SOAP_NULL:  printf("An element was null, while it is not supposed to be null\n"); break;
    case SOAP_DUPLICATE_ID:  printf("Multiple occurrences of the same element ID on the input\n"); break;
    case SOAP_MISSING_ID:  printf("Element ID missing for an HREF on the input\n"); break;
    case SOAP_HREF:  printf("Reference to object is incompatible with the object refered to\n"); break;
    case SOAP_UDP_ERROR:  printf("Message too large to store in UDP packet\n"); break;
    case SOAP_TCP_ERROR:  printf("A connection error occured\n"); break;
    case SOAP_HTTP_ERROR:  printf("An HTTP error occured\n"); break;
    case SOAP_SSL_ERROR:  printf("An SSL error occured\n"); break;
    case SOAP_ZLIB_ERROR:  printf("A Zlib error occured\n"); break;
    case SOAP_PLUGIN_ERROR:  printf("Failed to register plugin\n"); break;
    case SOAP_MIME_ERROR:  printf("MIME parsing error\n"); break;
    case SOAP_MIME_HREF:  printf("MIME has no href from SOAP body\n"); break;
    case SOAP_MIME_END:  printf("End of DIME error\n"); break;
    case SOAP_DIME_ERROR:  printf("DIME parsing error\n"); break;
    case SOAP_DIME_END:  printf("End of DIME error\n"); break;
    case SOAP_DIME_HREF:  printf("DIME attachment has no href from SOAP body \n(and no DIME callbacks were defined to save the attachment)\n"); break;
    case SOAP_DIME_MISMATCH:  printf("DIME version/transmission error\n"); break;
    case SOAP_VERSIONMISMATCH:  printf("SOAP version mismatch or no SOAP message\n"); break;
    case SOAP_DATAENCODINGUNKNOWN:  printf("SOAP 1.2 DataEncodingUnknown fault\n"); break;
    case SOAP_REQUIRED:  printf("Attributed required validation error\n"); break;
    case SOAP_PROHIBITED:  printf("Attributed prohibited validation error\n"); break;
    case SOAP_OCCURS:  printf("Element minOccurs/maxOccurs validation error\n"); break;
    case SOAP_LENGTH:  printf("Element length validation error\n"); break;
    case SOAP_EOF:  printf("Unexpected end of file, no input, or timeout while receiving data\n"); break;
  }
}


/*********************************************/
/*** cUTCLetter                            ***/
/*** argument :                            ***/
/***  timezone value                       ***/
/*** return value :                        ***/
/***  timezone letter                      ***/
/*********************************************/ 
char cUTCLetter(long _TimeZone)
{
  char cResult = 'Z'; // default value for 0

  if (_TimeZone < 0)   cResult = 64 - _TimeZone; /* from A to I */
  if (_TimeZone < -10) cResult = 65 - _TimeZone; /* from K to M */
  if (_TimeZone > 0)   cResult = 77 + _TimeZone; /* from N to X */

  return cResult;
}


/*********************************************/
/*** szFormatLocalTime                     ***/
/*** Format date time in a string          ***/
/*********************************************/ 
void szFormatLocalTime(time_t _tDateTime, char *_szDate)
{
struct tm *m;
long nTimezone;

  m = localtime(&_tDateTime);

  nTimezone = timezone / 3600; // diff timezone with Greenwitch

  sprintf(_szDate, "%04d-%02d-%02dT%02d:%02d:%02d%c%02d:00",
    m->tm_year+1900, 
    m->tm_mon + 1, 
    m->tm_mday,
    //cUTCLetter(nTimezone), /* timezone letter */
    m->tm_hour, m->tm_min, m->tm_sec,
    (nTimezone<=0)?'+':'-',
    abs(nTimezone));
}


/*********************************************/
/*** SetInformRequestParameter             ***/
/*** Send an Inform Request to ACS         ***/
/*********************************************/ 
#define Company_Name "Company Name"
#define SN           "SN"
#define Device_Type  "Device Type"

VENDORINFO st_vendor;

char *skip_space(char *str)
{
    char *ptmp = str;
    alarm(NULL == str);
    while(*(ptmp++) == ' ');
    return ptmp-1;
}

int get_vendor_info(VENDORINFO *vendor_info)
{
    FILE *fp;
    char str_conf[64];
    char *ptemp = NULL;
    char *ptemp2 = NULL;
    fp = popen("/usr/sbin/showsysinfo", "r");
    while(fgets(str_conf, 64, fp))
    {
        ptemp2 = skip_space(str_conf);
        ptemp = strtok(ptemp2, ":\n");
        if(!strncmp(ptemp, Company_Name, strlen(Company_Name)))
        {
            ptemp = strtok(NULL, ":\n");
            if(ptemp != NULL)
                strncpy(st_vendor.vendor_name, ptemp, strlen(DEFAULT_MANUFACTURER));
        }
        else if(!strncmp(ptemp, SN, strlen(SN)))
        {            
            ptemp = strtok(NULL, ":\n");
            if(ptemp != NULL)
                strcpy(st_vendor.serial_number, ptemp);
        }
        else if(!strncmp(ptemp, Device_Type, strlen(Device_Type)))
        {
            ptemp = strtok(NULL, ":\n");
            if(ptemp != NULL)
                strcpy(st_vendor.product_class, ptemp);
        }
    }
    pclose(fp);

//hxh: if not define oui, use default oui instead
    if(fp=fopen("/jffs/.OEM/.vendor_oui","r"))
    {
        fgets(str_conf, 64, fp);
        int length = strlen(str_conf);
        if(str_conf[length-1] == '\n')
            str_conf[length-1] = '\0';
        strcpy(st_vendor.vendor_oui, str_conf);
        fclose(fp);
    }
    else
    {
        strcpy(st_vendor.vendor_oui, DEFAULT_O_U_I);
    }
//hxh:modify end
        
    return 0;
}

int SetInformRequestParameter(struct cwmp__Inform *_soap_Inform, LIST_PARAM_VALUE *_list_param_value, char *_EventCode, char *_EventCmdKey)
{
LIST_PARAM_VALUE *ptr;  // pointer on current element
int nbDefaultParam, nbParam, nbParameter;
short i;

  dprintf(LOG_LEVEL2, "SetInformRequestParameter\n");
  // Inform parameters
  _soap_Inform->DeviceId.Manufacturer = st_vendor.vendor_name;//DEFAULT_MANUFACTURER;
  _soap_Inform->DeviceId.OUI          = st_vendor.vendor_oui;//DEFAULT_O_U_I;
  _soap_Inform->DeviceId.ProductClass = st_vendor.product_class;//DEFAULT_PRODUCTCLASS;
  _soap_Inform->DeviceId.SerialNumber = st_vendor.serial_number;//DEFAULT_SERIALNUMBER;

  // Default parameters
  nbParam = nbDefaultParam = 0;

  ptr = _list_param_value;

  if (ptr != NULL)
  {
    do
    {
      nbParam++;
      dprintf(LOG_LEVEL4, "parameter: %s\t\tvalue: %s\n", ptr->szParameter, ptr->szValue);
      if ((strcmp(ptr->szParameter, "Device.DeviceInfo.Manufacturer") == 0) ||
          (strcmp(ptr->szParameter, "InternetGatewayDevice.DeviceInfo.Manufacturer") == 0))
      {
        _soap_Inform->DeviceId.Manufacturer = ptr->szValue;
        nbDefaultParam++;
      }
      else if ((strcmp(ptr->szParameter, "Device.DeviceInfo.ManufacturerOUI") == 0) ||
          (strcmp(ptr->szParameter, "InternetGatewayDevice.DeviceInfo.ManufacturerOUI") == 0))
      {
        _soap_Inform->DeviceId.OUI = ptr->szValue;
        nbDefaultParam++;
      }
      else if ((strcmp(ptr->szParameter, "Device.DeviceInfo.ProductClass") == 0) ||
          (strcmp(ptr->szParameter, "InternetGatewayDevice.DeviceInfo.ProductClass") == 0))
      {
        _soap_Inform->DeviceId.ProductClass = ptr->szValue;
        nbDefaultParam++;
      }
      else if ((strcmp(ptr->szParameter, "Device.DeviceInfo.SerialNumber") == 0) ||
          (strcmp(ptr->szParameter, "InternetGatewayDevice.DeviceInfo.SerialNumber") == 0))
      {
        _soap_Inform->DeviceId.SerialNumber = ptr->szValue;
        nbDefaultParam++;
      }

      ptr = ptr->next;
    }
    while(ptr != NULL);
  }

  nbParameter = nbParam-nbDefaultParam;

  _soap_Inform->Event = (struct __EventList *)malloc(sizeof(struct __EventList));
  _soap_Inform->Event->__ptrEventStruct = (struct cwmp__EventStruct *)malloc(1*sizeof(struct cwmp__EventStruct));
  _soap_Inform->Event->__size = 1;
  _soap_Inform->Event->__ptrEventStruct[0].EventCode = _EventCode;
  _soap_Inform->Event->__ptrEventStruct[0].CommandKey = _EventCmdKey;

  _soap_Inform->RetryCount = 0;
  _soap_Inform->MaxEnvelopes = 1;

  _soap_Inform->ParameterList = (struct __ParameterList *)malloc(sizeof(struct __ParameterList));
  _soap_Inform->ParameterList->__ptrParameterValueStruct = (struct cwmp__ParameterValueStruct *)malloc(nbParameter*sizeof(struct cwmp__ParameterValueStruct));
  _soap_Inform->ParameterList->__size = nbParameter;

  ptr = _list_param_value;
  if (ptr != NULL)
  {
    i = 0;
    do
    {
      if ((strcmp(ptr->szParameter, "Device.DeviceInfo.Manufacturer") != 0) &&
          (strcmp(ptr->szParameter, "InternetGatewayDevice.DeviceInfo.Manufacturer") != 0) &&
          (strcmp(ptr->szParameter, "Device.DeviceInfo.ManufacturerOUI") != 0) &&
          (strcmp(ptr->szParameter, "InternetGatewayDevice.DeviceInfo.ManufacturerOUI") != 0) &&
          (strcmp(ptr->szParameter, "Device.DeviceInfo.ProductClass") != 0) &&
          (strcmp(ptr->szParameter, "InternetGatewayDevice.DeviceInfo.ProductClass") != 0) &&
          (strcmp(ptr->szParameter, "Device.DeviceInfo.SerialNumber") != 0) &&
          (strcmp(ptr->szParameter, "InternetGatewayDevice.DeviceInfo.SerialNumber") != 0))
      {
        _soap_Inform->ParameterList->__ptrParameterValueStruct[i].Name = ptr->szParameter;
        _soap_Inform->ParameterList->__ptrParameterValueStruct[i].Value.Value = (void*)ptr->szValue;
        _soap_Inform->ParameterList->__ptrParameterValueStruct[i].Value.__type = SOAP_TYPE_string;
        i++;
      }
      ptr = ptr->next;
    }
    while(ptr != NULL);
  }

  return 0;
}



/*********************************************/
/*** call_InformRequest                    ***/
/*** Send an Inform Request to ACS         ***/
/*** Encode SOAP message                   ***/
/*********************************************/ 
int call_InformRequest(struct soap*_soap, const char *_soap_endpoint, LIST_PARAM_VALUE *_list_param_value, LIST_PARAM_VALUE *_list_notification, char *_EventCode, char *_EventCmdKey)
{
// Inform Request arguments
struct cwmp__Inform soap_tmp_cwmp__Inform;

LIST_PARAM_VALUE *new_list, *ptr;
LIST_PARAM_VALUE *elt;  //add by wangjr 091124

time_t timeCurrent;
char szLocalDate[40];

  dprintf(LOG_LEVEL2, "call_InformRequest\n");
  // create new list (concatenation of 2 lists)
  new_list = duplicate_queue(_list_param_value);

  ptr = _list_notification;
/////////////////////////////////update by wangjr///////////////////////////////////////////
  while(ptr != NULL)
  {
	  elt = (LIST_PARAM_VALUE *)malloc(sizeof(LIST_PARAM_VALUE));
	  strcpy(elt->szParameter, ptr->szParameter);
	  strcpy(elt->szValue, ptr->szValue);
    add_in_queue(new_list, elt);
    ptr = ptr->next;
  }
/////////////////////////////////update by wangjr///////////////////////////////////////////

  ptr = new_list;

  SetInformRequestParameter(&soap_tmp_cwmp__Inform, ptr, _EventCode, _EventCmdKey);

  dprintf(LOG_LEVEL3, "Manufacturer : %s\n", soap_tmp_cwmp__Inform.DeviceId.Manufacturer);

// Format date/time (format has changed, cf. wt121v6 !!)
  time(&timeCurrent);
  szFormatLocalTime(timeCurrent, szLocalDate);
  dprintf(LOG_LEVEL3, "..local Date/hour : %s\n", szLocalDate);
  soap_tmp_cwmp__Inform.CurrentTime = timeCurrent; // time has changed with wt121v6
//  soap_tmp_cwmp__Inform.CurrentTime = szLocalDate;

  dprintf(LOG_LEVEL3, "..begin serialization\n");
  soap_begin(_soap);
  soap_serializeheader(_soap);
  soap_serialize_cwmp__Inform(_soap, &soap_tmp_cwmp__Inform);
  if (soap_begin_count(_soap))
    return _soap->error;
  if (_soap->mode & SOAP_IO_LENGTH)
  { if (soap_envelope_begin_out(_soap)
     || soap_putheader(_soap)
     || soap_body_begin_out(_soap)
     || soap_put_cwmp__Inform(_soap, &soap_tmp_cwmp__Inform, "cwmp:Inform", "")
     || soap_body_end_out(_soap)
     || soap_envelope_end_out(_soap))
       goto error_label;
  }
  if (soap_end_count(_soap))
    goto error_label;
  dprintf(LOG_LEVEL3, "..end serialization\n");

  dprintf(LOG_LEVEL3, "..send SOAP message\n");
  if (soap_connect(_soap, _soap_endpoint, NULL)
   || soap_envelope_begin_out(_soap)
   || soap_putheader(_soap)
   || soap_body_begin_out(_soap)
   || soap_put_cwmp__Inform(_soap, &soap_tmp_cwmp__Inform, "cwmp:Inform", "")
   || soap_body_end_out(_soap)
   || soap_envelope_end_out(_soap)
   || soap_end_send(_soap))
    goto error_terminate_label;

  dprintf(LOG_LEVEL3, "..free allocated resources\n");
  // free resources of sent arguments
  if (soap_tmp_cwmp__Inform.ParameterList != NULL)
    free(soap_tmp_cwmp__Inform.ParameterList);
  
  if (soap_tmp_cwmp__Inform.Event != NULL)
    free(soap_tmp_cwmp__Inform.Event);

  free_queue(new_list);

error_label:
  return _soap->error;
error_terminate_label:
  return soap_closesock(_soap);
}


/*********************************************/
/*** wait_InformResponse                   ***/
/*** Wait for an Inform Response from ACS  ***/
/*** Decode SOAP message                   ***/
/*********************************************/ 
int wait_InformResponse(struct soap*_soap, struct InformResponse *_response)
{
// Inform Response argument
unsigned int MaxEnvelopesResponse;

  dprintf(LOG_LEVEL2, "wait_InformResponse\n");
  if (soap_begin_recv(_soap)
   || soap_envelope_begin_in(_soap)
   || soap_recv_header(_soap)
   || soap_body_begin_in(_soap))
    return soap_closesock(_soap);
  soap_get_InformResponse(_soap, _response, "InformResponse", "");

  dprintf(LOG_LEVEL3, "---->soap.error: %d\n", _soap->error);
  if (_soap->error)
  { if (_soap->error == SOAP_TAG_MISMATCH && _soap->level == 2)
      return soap_recv_fault(_soap);
    return soap_closesock(_soap);
  }

//  soap_default_InformReponse(_soap, Response);
  soap_default_unsignedInt(_soap, &MaxEnvelopesResponse);

  MaxEnvelopesResponse = _response->MaxEnvelopes;
  dprintf(LOG_LEVEL2, "..MaxEnvelopes: %d\n", MaxEnvelopesResponse);

  dprintf(LOG_LEVEL3, "..end of SOAP message\n");
  if (soap_body_end_in(_soap)
   || soap_envelope_end_in(_soap)
   || soap_end_recv(_soap))
    return soap_closesock(_soap);
// FB fermeture connexion TCP !!!   return soap->error;

  return soap_closesock(_soap);
}


/*********************************************/
/*** call_TransferComplete                 ***/
/*** Send a TransferComplete to ACS        ***/
/*** Encode SOAP message                   ***/
/*********************************************/ 
int call_TransferComplete(struct soap*_soap, const char *_soap_endpoint, char *_EventCmdKey, unsigned int _FaultCode, char *_szFaultString, time_t _timeStart)
{
// Transfer Complete arguments
struct cwmp__TransferComplete soap_tmp_cwmp__TransferComplete;

time_t timeCurrent;
char szLocalDate[40];

  dprintf(LOG_LEVEL2, "call_TransferComplete\n");
// Format date/time (format has changed, cf. wt121v6 !!)
  time(&timeCurrent);
  szFormatLocalTime(timeCurrent, szLocalDate);
  dprintf(LOG_LEVEL3, "..local Date/hour : %s\n", szLocalDate);
  soap_tmp_cwmp__TransferComplete.CommandKey = _EventCmdKey;
  soap_tmp_cwmp__TransferComplete.FaultStruct.FaultCode = _FaultCode;
  soap_tmp_cwmp__TransferComplete.FaultStruct.FaultString  = _szFaultString;
  //soap_tmp_cwmp__TransferComplete.StartTime = _timeStart;   //update by wangjr 091124
  //soap_tmp_cwmp__TransferComplete.CompleteTime = timeCurrent;
  soap_tmp_cwmp__TransferComplete.StartTime = download_endtime;
  soap_tmp_cwmp__TransferComplete.CompleteTime = download_endover;


  dprintf(LOG_LEVEL3, "..begin serialization\n");
  soap_begin(_soap);
  soap_serializeheader(_soap);
  soap_serialize_cwmp__TransferComplete(_soap, &soap_tmp_cwmp__TransferComplete);
  if (soap_begin_count(_soap))
    return _soap->error;
  if (_soap->mode & SOAP_IO_LENGTH)
  { if (soap_envelope_begin_out(_soap)
     || soap_putheader(_soap)
     || soap_body_begin_out(_soap)
     || soap_put_cwmp__TransferComplete(_soap, &soap_tmp_cwmp__TransferComplete, "cwmp:TransferComplete", "")
     || soap_body_end_out(_soap)
     || soap_envelope_end_out(_soap))
       goto error_label;
  }
  if (soap_end_count(_soap))
    goto error_label;
  dprintf(LOG_LEVEL3, "..end serialization\n");

  dprintf(LOG_LEVEL3, "..send SOAP message\n");
  if (soap_connect(_soap, _soap_endpoint, NULL)
   || soap_envelope_begin_out(_soap)
   || soap_putheader(_soap)
   || soap_body_begin_out(_soap)
   || soap_put_cwmp__TransferComplete(_soap, &soap_tmp_cwmp__TransferComplete, "cwmp:TransferComplete", "")
   || soap_body_end_out(_soap)
   || soap_envelope_end_out(_soap)
   || soap_end_send(_soap))
    goto error_terminate_label;

error_label:
  return _soap->error;
error_terminate_label:
  return soap_closesock(_soap);
}


/*********************************************/
/*** wait_TransferCompleteResponse         ***/
/*** Wait for an Transfer complete Response***/
/*** from ACS                              ***/
/*** Decode SOAP message                   ***/
/*********************************************/ 
int wait_TransferCompleteResponse(struct soap*_soap)
{
struct cwmp__TransferCompleteResponse *out;

  dprintf(LOG_LEVEL2, "wait_TransferCompleteResponse\n");
  soap_default_cwmp__TransferCompleteResponse(_soap, out);

  if (soap_begin_recv(_soap)
   || soap_envelope_begin_in(_soap)
   || soap_recv_header(_soap)
   || soap_body_begin_in(_soap))
    return soap_closesock(_soap);
  soap_get_cwmp__TransferCompleteResponse(_soap, out, "cwmp:TransferCompleteResponse", "");

  dprintf(LOG_LEVEL3, "---->soap.error: %d\n", _soap->error);
  if (_soap->error)
  { if (_soap->error == SOAP_TAG_MISMATCH && _soap->level == 2)
      return soap_recv_fault(_soap);
    return soap_closesock(_soap);
  }

  dprintf(LOG_LEVEL3, "..end of SOAP message\n");
  if (soap_body_end_in(_soap)
   || soap_envelope_end_in(_soap)
   || soap_end_recv(_soap))
    return soap_closesock(_soap);
// FB fermeture connexion TCP !!!   return soap->error;

  return soap_closesock(_soap);
}


/**************************************************************************/
/*** GetRPCMethods Request/Response                                            ***/
/**************************************************************************/ 

#define RPCMETHODS_COUNT 16
#define METHOD_NAME_LEN 64

static xsd__string g_str_RPCMethodList[RPCMETHODS_COUNT]={
	"cwmp:GetRPCMethods",
	"cwmp:ScheduleInform",
	"cwmp:GetParameterNames",
	"cwmp:GetParametervalues",
	"cwmp:GetParameterAttributes",
	"cwmp:SetParametervalues",
	"cwmp:SetParameterAttributes",
	"cwmp:AddObject",
	"cwmp:DeleteObject",
	"cwmp:Download",
	"cwmp:Upload",
	"cwmp:Reboot",
	"cwmp:InformResponse",
	"cmwp:FactorySet",
	"cwmp:TransferCompleteResponse",
	"cwmp:GetRPCMethodsResponse"
};

int soap_wait_cwmp__GetRPCMethods(struct soap *_soap, struct cwmp__GetRPCMethods *_a)
{
	dprintf(LOG_LEVEL2, "soap_wait_cwmp__GetRPCMethods\n");
	soap_default_cwmp__GetRPCMethods(_soap, _a);
	_soap->encodingStyle = "";
	printf("aaa\n\n%s\naaa\n\n",_soap->tmpbuf);
	if (!soap_get_cwmp__GetRPCMethods(_soap, _a, "cwmp:GetRPCMethods", NULL))
	  return _soap->error;
	printf("aaa\n\n%s\naaa\n\n",_soap->tmpbuf);
	if (soap_body_end_in(_soap)
	 || soap_envelope_end_in(_soap)
	 || soap_end_recv(_soap))
	  return _soap->error;
	
	return _soap->error;
}

int ParseGetRPCMethods_and_SendResponse(struct soap *_soap, const char *_endpoint, struct cwmp__GetRPCMethods _tmp_cwmp__GetParameterValuesRequest)
{	
    int nNbValue = 0;
	int nRet = 0;
	int nNbParam;
	struct cwmp__GetRPCMethodsResponse tmp_cwmp__GetRPCMethodsResponse;
	int nLength;

  dprintf(LOG_LEVEL3, "---> allocates memory for GetRPCMethods Response\n");
  // memory allocation for response

  dprintf(LOG_LEVEL3, "---> analyze GetRPCMethods Request\n");
  // test if nb parameter(s) higher than 0

  tmp_cwmp__GetRPCMethodsResponse.MethodList = (struct __MethodList*)malloc(sizeof(struct __MethodList));

  // Parse GetParameterValues Request And Fill Response
  tmp_cwmp__GetRPCMethodsResponse.MethodList->__size = RPCMETHODS_COUNT;
  printf("--->size:%d\n",tmp_cwmp__GetRPCMethodsResponse.MethodList->__size);
  tmp_cwmp__GetRPCMethodsResponse.MethodList->__ptrstring = g_str_RPCMethodList;
  for (nNbValue=0; nNbValue < RPCMETHODS_COUNT; nNbValue++)
  {
    printf("-->%s\n",tmp_cwmp__GetRPCMethodsResponse.MethodList->__ptrstring[nNbValue]);
	
  		// Fill response structure
  	//	tmp_cwmp__GetRPCMethodsResponse.MethodList.string[nNbValue] = (xsd__string)malloc(METHOD_NAME_LEN*sizeof(xsd__string));
	//    strcpy(tmp_cwmp__GetRPCMethodsResponse.MethodList.string, g_str_RPCMethodList[nNbValue]);
  }
  
  // send GetParameterValues response
    dprintf(LOG_LEVEL3, "......send GetRPCMethods Response.\n");
    nRet = send_GetRPCMethodsResponse(_soap, _endpoint, &tmp_cwmp__GetRPCMethodsResponse);

    // Free String Resources
/*    for (nNbValue=0; nNbValue < tmp_cwmp__GetRPCMethodsResponse.MethodList.__size; nNbValue++)
    {
      //if (tmp_cwmp__GetParameterValuesResponse.ParameterList->__ptrParameterValueStruct[nNbValue].Name != NULL)
      //    free(tmp_cwmp__GetParameterValuesResponse.ParameterList->__ptrParameterValueStruct[nNbValue].Name);
      //if ((nParameterType == SOAP_TYPE_string) && (tmp_cwmp__GetParameterValuesResponse.ParameterList->__ptrParameterValueStruct != NULL) && (tmp_cwmp__GetParameterValuesResponse.ParameterList->__ptrParameterValueStruct[nNbValue].__AnytypeStruct.Value != NULL))
      if ((tmp_cwmp__GetRPCMethodsResponse.MethodList.string != NULL) && (tmp_cwmp__GetRPCMethodsResponse.MethodList.string[nNbValue] != NULL))
      {
        free(tmp_cwmp__GetRPCMethodsResponse.MethodList.string[nNbValue]);
        tmp_cwmp__GetRPCMethodsResponse.MethodList.string[nNbValue] = NULL;
      }
    }

    if (tmp_cwmp__GetRPCMethodsResponse.MethodList.string != NULL)
    {
      free(tmp_cwmp__GetRPCMethodsResponse.MethodList.string);
    }*/
    return nRet;
}

int send_GetRPCMethodsResponse(struct soap*_soap, const char *_soap_endpoint, struct cwmp__GetRPCMethodsResponse*_GetMethodResponse)
{
  printf("%d\n", __LINE__);
  soap_serializeheader(_soap);
  printf("%d\n", __LINE__);
  soap_serialize_cwmp__GetRPCMethodsResponse(_soap, _GetMethodResponse);
  printf("%d\n", __LINE__);
  if (soap_begin_count(_soap))
    return _soap->error;
  printf("%d\n", __LINE__);
  //printf("%x\n", _soap->header);
  if (_soap->mode & SOAP_IO_LENGTH)
  { if (soap_envelope_begin_out(_soap)
     || soap_putheader(_soap)
     || soap_body_begin_out(_soap)
     || soap_put_cwmp__GetRPCMethodsResponse(_soap, _GetMethodResponse, "cwmp:GetRPCMethodsResponse", "")
     || soap_body_end_out(_soap)
     || soap_envelope_end_out(_soap))
       return _soap->error;
  };
  printf("%d\n", __LINE__);
  if (soap_end_count(_soap)
   || soap_connect(_soap,_soap_endpoint, NULL)
   || soap_envelope_begin_out(_soap)
   || soap_putheader(_soap)
   || soap_body_begin_out(_soap)
   || soap_put_cwmp__GetRPCMethodsResponse(_soap, _GetMethodResponse, "cwmp:GetRPCMethodsResponse", "")
   || soap_body_end_out(_soap)
   || soap_envelope_end_out(_soap)
   || soap_end_send(_soap))
    return _soap->error;
   printf("%d\n", __LINE__);
  return soap_closesock(_soap);
}


/**************************************************************************/
/*** GetParameterNames Request/Response                                ***/
/**************************************************************************/
int soap_wait_cwmp__GetParameterNames(struct soap *_soap, struct cwmp__GetParameterNames *_a)
{ 
  dprintf(LOG_LEVEL2, "soap_wait_cwmp__GetParameterNames\n");
  soap_default_cwmp__GetParameterNames(_soap, _a);
  _soap->encodingStyle = "";
  if (!soap_get_cwmp__GetParameterNames(_soap, _a, "cwmp:GetParameterNames", NULL))
    return _soap->error;
  if (soap_body_end_in(_soap)
   || soap_envelope_end_in(_soap)
   || soap_end_recv(_soap))
    return _soap->error;

  return _soap->error;
}

int GetParameterNames_serve_request(struct soap *_soap, struct cwmp__GetParameterNames *_a)
{
  soap_peek_element(_soap);
  if (!soap_match_tag(_soap, _soap->tag, "cwmp:GetParameterNames"))
    return soap_wait_cwmp__GetParameterNames(_soap, _a);
  return _soap->error = SOAP_NO_METHOD;
}

int send_GetParameterNamesResponse(struct soap*_soap, const char *_soap_endpoint, struct cwmp__GetParameterNamesResponse*_GetParamResponse)
{
  soap_serializeheader(_soap);
  soap_serialize_cwmp__GetParameterNamesResponse(_soap, _GetParamResponse);
  if (soap_begin_count(_soap))
    return _soap->error;
  if (_soap->mode & SOAP_IO_LENGTH)
  { if (soap_envelope_begin_out(_soap)
     || soap_putheader(_soap)
     || soap_body_begin_out(_soap)
     || soap_put_cwmp__GetParameterNamesResponse(_soap, _GetParamResponse, "cwmp:GetParameterNamesResponse", "")
     || soap_body_end_out(_soap)
     || soap_envelope_end_out(_soap))
       return _soap->error;
  };
  if (soap_end_count(_soap)
   || soap_connect(_soap,_soap_endpoint, NULL)
   || soap_envelope_begin_out(_soap)
   || soap_putheader(_soap)
   || soap_body_begin_out(_soap)
   || soap_put_cwmp__GetParameterNamesResponse(_soap, _GetParamResponse, "cwmp:GetParameterNamesResponse", "")
   || soap_body_end_out(_soap)
   || soap_envelope_end_out(_soap)
   || soap_end_send(_soap))
    return _soap->error;

  return soap_closesock(_soap);
}

/////////////////////////////add by wangjr for  SetParameterValuesFault 091124///////////////////////////////////////////////////////
int send_SetFaultResponse(struct soap*_soap, const char *_soap_endpoint, struct cwmp__SetFaultResponse*_SetFaultResponse)
{
  soap_serializeheader(_soap);
  soap_serialize_cwmp__SetFaultResponse(_soap, _SetFaultResponse);
  if (soap_begin_count(_soap))
    return _soap->error;
  if (_soap->mode & SOAP_IO_LENGTH)
  { if (soap_envelope_begin_out(_soap)
     || soap_putheader(_soap)
     || soap_body_begin_out(_soap)
     || soap_put_cwmp__SetFaultResponse(_soap, _SetFaultResponse, "soap:Fault", "")
     || soap_body_end_out(_soap)
     || soap_envelope_end_out(_soap))
       return _soap->error;
  };
  if (soap_end_count(_soap)
   || soap_connect(_soap,_soap_endpoint, NULL)
   || soap_envelope_begin_out(_soap)
   || soap_putheader(_soap)
   || soap_body_begin_out(_soap)
   || soap_put_cwmp__SetFaultResponse(_soap, _SetFaultResponse, "soap:Fault", "")
   || soap_body_end_out(_soap)
   || soap_envelope_end_out(_soap)
   || soap_end_send(_soap))
    return _soap->error;

  return soap_closesock(_soap);
}
/////////////////////////////add by wangjr for  SetParameterValuesFault 091124///////////////////////////////////////////////////////
//////////////////////////////////add by wangjr for GetParameterValuesFault  091124///////////////////////////////////////////////////////
int send_FaultResponse(struct soap*_soap, const char *_soap_endpoint, struct cwmp__FaultResponse*_FaultResponse)
{
  soap_serializeheader(_soap);
  soap_serialize_cwmp__FaultResponse(_soap, _FaultResponse);
  if (soap_begin_count(_soap))
    return _soap->error;
  if (_soap->mode & SOAP_IO_LENGTH)
  { if (soap_envelope_begin_out(_soap)
     || soap_putheader(_soap)
     || soap_body_begin_out(_soap)
     || soap_put_cwmp__FaultResponse(_soap, _FaultResponse, "soap:Fault", "")
     || soap_body_end_out(_soap)
     || soap_envelope_end_out(_soap))
       return _soap->error;
  };
  if (soap_end_count(_soap)
   || soap_connect(_soap,_soap_endpoint, NULL)
   || soap_envelope_begin_out(_soap)
   || soap_putheader(_soap)
   || soap_body_begin_out(_soap)
   || soap_put_cwmp__FaultResponse(_soap, _FaultResponse, "soap:Fault", "")
   || soap_body_end_out(_soap)
   || soap_envelope_end_out(_soap)
   || soap_end_send(_soap))
    return _soap->error;

  return soap_closesock(_soap);
}
//////////////////////////////////add by wangjr for GetParameterValuesFault  091124///////////////////////////////////////////////////////


/**************************************************************************/
/*** GetParameterValues Request/Response                                ***/
/**************************************************************************/
int soap_wait_cwmp__GetParameterValues(struct soap *_soap, struct cwmp__GetParameterValues *_a)
{ 
  dprintf(LOG_LEVEL2, "soap_wait_cwmp__GetParameterValues\n");
  soap_default_cwmp__GetParameterValues(_soap, _a);
  _soap->encodingStyle = "";
  if (!soap_get_cwmp__GetParameterValues(_soap, _a, "cwmp:GetParameterValues", NULL))
    return _soap->error;
  if (soap_body_end_in(_soap)
   || soap_envelope_end_in(_soap)
   || soap_end_recv(_soap))
    return _soap->error;

  return _soap->error;
}

int GetParameterValues_serve_request(struct soap *_soap, struct cwmp__GetParameterValues *_a)
{
  soap_peek_element(_soap);
  if (!soap_match_tag(_soap, _soap->tag, "cwmp:GetParameterValues"))
    return soap_wait_cwmp__GetParameterValues(_soap, _a);
  return _soap->error = SOAP_NO_METHOD;
}

int send_GetParameterValuesResponse(struct soap*_soap, const char *_soap_endpoint, struct cwmp__GetParameterValuesResponse*_GetParamResponse)
{
  soap_serializeheader(_soap);
  soap_serialize_cwmp__GetParameterValuesResponse(_soap, _GetParamResponse);
  if (soap_begin_count(_soap))
    return _soap->error;
  if (_soap->mode & SOAP_IO_LENGTH)
  { if (soap_envelope_begin_out(_soap)
     || soap_putheader(_soap)
     || soap_body_begin_out(_soap)
     || soap_put_cwmp__GetParameterValuesResponse(_soap, _GetParamResponse, "cwmp:GetParameterValuesResponse", "")
     || soap_body_end_out(_soap)
     || soap_envelope_end_out(_soap))
       return _soap->error;
  };
  if (soap_end_count(_soap)
   || soap_connect(_soap,_soap_endpoint, NULL)
   || soap_envelope_begin_out(_soap)
   || soap_putheader(_soap)
   || soap_body_begin_out(_soap)
   || soap_put_cwmp__GetParameterValuesResponse(_soap, _GetParamResponse, "cwmp:GetParameterValuesResponse", "")
   || soap_body_end_out(_soap)
   || soap_envelope_end_out(_soap)
   || soap_end_send(_soap))
    return _soap->error;

  return soap_closesock(_soap);
}


//**************************************************************************/
/*** FactoryReset Request/Response                                            ***/
/**************************************************************************/ 
int soap_wait_cwmp__FactoryReset(struct soap *_soap, struct cwmp__FactoryReset *_a)
{
  dprintf(LOG_LEVEL2, "soap_wait_cwmp__FactoryReset\n");
  soap_default_cwmp__FactoryReset(_soap, _a);
  printf("acif:%d\n",__LINE__);
  _soap->encodingStyle = "";
  printf("acif:%d\n",__LINE__);
  if (!soap_get_cwmp__FactoryReset(_soap, _a, "cwmp:FactoryReset", NULL))
    return _soap->error;
  printf("acif:%d\n",__LINE__);
  if (soap_body_end_in(_soap)
   || soap_envelope_end_in(_soap)
   || soap_end_recv(_soap))
    return _soap->error;
  printf("acif:%d\n",__LINE__);
  return _soap->error;
}

int send_FactoryResetResponse(struct soap*_soap, const char *_soap_endpoint, struct cwmp__FactoryResetResponse* _FactoryResetResponse)
{
  soap_serializeheader(_soap);
  soap_serialize_cwmp__FactoryResetResponse(_soap, _FactoryResetResponse);
  if (soap_begin_count(_soap))
    return _soap->error;
  if (_soap->mode & SOAP_IO_LENGTH)
  { if (soap_envelope_begin_out(_soap)
     || soap_putheader(_soap)
     || soap_body_begin_out(_soap)
     || soap_put_cwmp__FactoryResetResponse(_soap, _FactoryResetResponse, "cwmp:FactoryResetResponse", "")
     || soap_body_end_out(_soap)
     || soap_envelope_end_out(_soap))
       return _soap->error;
  };
  if (soap_end_count(_soap)
   || soap_connect(_soap,_soap_endpoint, NULL)
   || soap_envelope_begin_out(_soap)
   || soap_putheader(_soap)
   || soap_body_begin_out(_soap)
   || soap_put_cwmp__FactoryResetResponse(_soap, _FactoryResetResponse, "cwmp:FactoryResetResponse", "")
   || soap_body_end_out(_soap)
   || soap_envelope_end_out(_soap)
   || soap_end_send(_soap))
    return _soap->error;

  return soap_closesock(_soap);
}

/*********************************************/
/* ParseFactoryReset_and_SendResponse              */
/* Parse FactoryReset request and send response    */
/*********************************************/
int ParseFactoryReset_and_SendResponse(struct soap *_soap, const char *_endpoint, struct cwmp__FactoryReset _tmp_cwmp__FactoryResetRequest)
{
	int nRet = 0;
	struct cwmp__FactoryResetResponse tmp_cwmp__FactoryResetResponse;

  	dprintf(LOG_LEVEL2, "ParseFactoryReset_and_SendResponse.\n");

  	soap_default_cwmp__FactoryResetResponse(_soap, &_tmp_cwmp__FactoryResetRequest);
  	nRet = send_FactoryResetResponse(_soap, _endpoint, &_tmp_cwmp__FactoryResetRequest);

  	return nRet;
}
//**************************************************************************/
/*** AddObject Request/Response                                            ***/
/**************************************************************************/ 
int soap_wait_cwmp__AddObject(struct soap *_soap, struct cwmp__AddObject *_a)
{
  dprintf(LOG_LEVEL2, "soap_wait_cwmp__AddObject\n");
  soap_default_cwmp__AddObject(_soap, _a);
  _soap->encodingStyle = "";
  if (!soap_get_cwmp__AddObject(_soap, _a, "cwmp:AddObject", NULL))
    return _soap->error;
  if (soap_body_end_in(_soap)
   || soap_envelope_end_in(_soap)
   || soap_end_recv(_soap))
    return _soap->error;
  dprintf(LOG_LEVEL3,"acif:%d\n",__LINE__);
  return _soap->error;
}

int send_AddObjectResponse(struct soap*_soap, const char *_soap_endpoint, struct cwmp__DeleteObjectResponse* _AddObjectResponse)
{
  soap_serializeheader(_soap);
  soap_serialize_cwmp__AddObjectResponse(_soap, _AddObjectResponse);
  if (soap_begin_count(_soap))
    return _soap->error;
  if (_soap->mode & SOAP_IO_LENGTH)
  { if (soap_envelope_begin_out(_soap)
     || soap_putheader(_soap)
     || soap_body_begin_out(_soap)
     || soap_put_cwmp__AddObjectResponse(_soap, _AddObjectResponse, "cwmp:AddObjectResponse", "")
     || soap_body_end_out(_soap)
     || soap_envelope_end_out(_soap))
       return _soap->error;
  };
  if (soap_end_count(_soap)
   || soap_connect(_soap,_soap_endpoint, NULL)
   || soap_envelope_begin_out(_soap)
   || soap_putheader(_soap)
   || soap_body_begin_out(_soap)
   || soap_put_cwmp__AddObjectResponse(_soap, _AddObjectResponse, "cwmp:AddObjectResponse", "")
   || soap_body_end_out(_soap)
   || soap_envelope_end_out(_soap)
   || soap_end_send(_soap))
    return _soap->error;

  return soap_closesock(_soap);

}



//**************************************************************************/
/*** DeleteObject Request/Response    by wangjr 091124                                         ***/
/**************************************************************************/ 
int soap_wait_cwmp__DeleteObject(struct soap *_soap, struct cwmp__DeleteObject *_a)
{
  dprintf(LOG_LEVEL2, "soap_wait_cwmp__DeleteObject\n");
  soap_default_cwmp__DeleteObject(_soap, _a);
  _soap->encodingStyle = "";
  if (!soap_get_cwmp__DeleteObject(_soap, _a, "cwmp:DeleteObject", NULL))
    return _soap->error;
  if (soap_body_end_in(_soap)
   || soap_envelope_end_in(_soap)
   || soap_end_recv(_soap))
    return _soap->error;
  dprintf(LOG_LEVEL3,"acif:%d\n",__LINE__);
  return _soap->error;
}

int send_DeleteObjectResponse(struct soap*_soap, const char *_soap_endpoint, struct cwmp__DeleteObjectResponse* _DeleteObjectResponse)
{
  soap_serializeheader(_soap);
  soap_serialize_cwmp__DeleteObjectResponse(_soap, _DeleteObjectResponse);
  if (soap_begin_count(_soap))
    return _soap->error;
  if (_soap->mode & SOAP_IO_LENGTH)
  { if (soap_envelope_begin_out(_soap)
     || soap_putheader(_soap)
     || soap_body_begin_out(_soap)
     || soap_put_cwmp__DeleteObjectResponse(_soap, _DeleteObjectResponse, "cwmp:DeleteObjectResponse", "")
     || soap_body_end_out(_soap)
     || soap_envelope_end_out(_soap))
       return _soap->error;
  };
  if (soap_end_count(_soap)
   || soap_connect(_soap,_soap_endpoint, NULL)
   || soap_envelope_begin_out(_soap)
   || soap_putheader(_soap)
   || soap_body_begin_out(_soap)
   || soap_put_cwmp__DeleteObjectResponse(_soap, _DeleteObjectResponse, "cwmp:DeleteObjectResponse", "")
   || soap_body_end_out(_soap)
   || soap_envelope_end_out(_soap)
   || soap_end_send(_soap))
    return _soap->error;

  return soap_closesock(_soap);
}
//**************************************************************************/
/*** GetParameterAttributes Request/Response             			***/
/**************************************************************************/ 
int soap_wait_cwmp__GetParameterAttributes(struct soap *_soap, struct cwmp__GetParameterAttributes *_a)
{
  dprintf(LOG_LEVEL2, "soap_wait_cwmp__GetParameterAttributes\n");
  soap_default_cwmp__GetParameterAttributes(_soap, _a);
  _soap->encodingStyle = "";
  if (!soap_get_cwmp__GetParameterAttributes(_soap, _a, "cwmp:GetParameterAttributes", NULL))
    return _soap->error;
  if (soap_body_end_in(_soap)
   || soap_envelope_end_in(_soap)
   || soap_end_recv(_soap))
    return _soap->error;
  dprintf(LOG_LEVEL3,"acif:%d\n",__LINE__);
  return _soap->error;
}

int send_GetParameterAttributesResponse(struct soap*_soap, const char *_soap_endpoint, struct cwmp__GetParameterAttributesResponse* _GetParameterAttributesResponse)
{
  soap_serializeheader(_soap);
  soap_serialize_cwmp__GetParameterAttributesResponse(_soap, _GetParameterAttributesResponse);
  if (soap_begin_count(_soap))
    return _soap->error;
  if (_soap->mode & SOAP_IO_LENGTH)
  { if (soap_envelope_begin_out(_soap)
     || soap_putheader(_soap)
     || soap_body_begin_out(_soap)
     || soap_put_cwmp__GetParameterAttributesResponse(_soap, _GetParameterAttributesResponse, "cwmp:GetParameterAttributesResponse", "")
     || soap_body_end_out(_soap)
     || soap_envelope_end_out(_soap))
       return _soap->error;
  };
  if (soap_end_count(_soap)
   || soap_connect(_soap,_soap_endpoint, NULL)
   || soap_envelope_begin_out(_soap)
   || soap_putheader(_soap)
   || soap_body_begin_out(_soap)
   || soap_put_cwmp__GetParameterAttributesResponse(_soap, _GetParameterAttributesResponse, "cwmp:GetParameterAttributesResponse", "")
   || soap_body_end_out(_soap)
   || soap_envelope_end_out(_soap)
   || soap_end_send(_soap))
    return _soap->error;

  return soap_closesock(_soap);

}



//**************************************************************************/
/*** SetParameterAttributes Request/Response    add by wangjr 091124         			***/
/**************************************************************************/ 
//#if 0
int soap_wait_cwmp__SetParameterAttributes(struct soap *_soap, struct cwmp__SetParameterAttributes *_a)
{
  dprintf(LOG_LEVEL2, "soap_wait_cwmp__SetParameterAttributes\n");
  soap_default_cwmp__SetParameterAttributes(_soap, _a);
  _soap->encodingStyle = "";
  if (!soap_get_cwmp__SetParameterAttributes(_soap, _a, "cwmp:SetParameterAttributes", NULL))
    return _soap->error;
  if (soap_body_end_in(_soap)
   || soap_envelope_end_in(_soap)
   || soap_end_recv(_soap))
    return _soap->error;
  dprintf(LOG_LEVEL3,"acif:%d\n",__LINE__);
  return _soap->error;
}

int send_SetParameterAttributesResponse(struct soap*_soap, const char *_soap_endpoint, struct cwmp__SetParameterAttributesResponse* _SetParameterAttributesResponse)
{
  soap_serializeheader(_soap);
  soap_serialize_cwmp__SetParameterAttributesResponse(_soap, _SetParameterAttributesResponse);
  if (soap_begin_count(_soap))
    return _soap->error;
  if (_soap->mode & SOAP_IO_LENGTH)
  { if (soap_envelope_begin_out(_soap)
     || soap_putheader(_soap)
     || soap_body_begin_out(_soap)
     || soap_put_cwmp__SetParameterAttributesResponse(_soap, _SetParameterAttributesResponse, "cwmp:SetParameterAttributesResponse", "")
     || soap_body_end_out(_soap)
     || soap_envelope_end_out(_soap))
       return _soap->error;
  };
  if (soap_end_count(_soap)
   || soap_connect(_soap,_soap_endpoint, NULL)
   || soap_envelope_begin_out(_soap)
   || soap_putheader(_soap)
   || soap_body_begin_out(_soap)
   || soap_put_cwmp__SetParameterAttributesResponse(_soap, _SetParameterAttributesResponse, "cwmp:SetParameterAttributesResponse", "")
   || soap_body_end_out(_soap)
   || soap_envelope_end_out(_soap)
   || soap_end_send(_soap))
    return _soap->error;

  return soap_closesock(_soap);

}
//#endif
//**************************************************************************/
/*** FactoryReset Request/Response                                            ***/
/**************************************************************************/ 
int soap_wait_cwmp__Hello(struct soap *_soap, struct cwmp__Hello *_a)
{
  dprintf(LOG_LEVEL2, "soap_wait_cwmp__Hello\n");
  soap_default_cwmp__Hello(_soap, _a);
  dprintf(LOG_LEVEL3,"acif:%d\n",__LINE__);
  _soap->encodingStyle = "";
  dprintf(LOG_LEVEL3,"acif:%d\n",__LINE__);
  if (!soap_get_cwmp__Hello(_soap, _a, "cwmp:Hello", NULL))
    return _soap->error;
  dprintf(LOG_LEVEL3,"acif:%d\n",__LINE__);
  if (soap_body_end_in(_soap)
   || soap_envelope_end_in(_soap)
   || soap_end_recv(_soap))
    return _soap->error;
  dprintf(LOG_LEVEL3,"acif:%d\n",__LINE__);
  return _soap->error;
}

int send_HelloResponse(struct soap*_soap, const char *_soap_endpoint, struct cwmp__HelloResponse* _HelloResponse)
{
  soap_serializeheader(_soap);
  soap_serialize_cwmp__HelloResponse(_soap, _HelloResponse);
  if (soap_begin_count(_soap))
    return _soap->error;
  if (_soap->mode & SOAP_IO_LENGTH)
  { if (soap_envelope_begin_out(_soap)
     || soap_putheader(_soap)
     || soap_body_begin_out(_soap)
     || soap_put_cwmp__HelloResponse(_soap, _HelloResponse, "cwmp:HelloResponse", "")
     || soap_body_end_out(_soap)
     || soap_envelope_end_out(_soap))
       return _soap->error;
  };
  if (soap_end_count(_soap)
   || soap_connect(_soap,_soap_endpoint, NULL)
   || soap_envelope_begin_out(_soap)
   || soap_putheader(_soap)
   || soap_body_begin_out(_soap)
   || soap_put_cwmp__HelloResponse(_soap, _HelloResponse, "cwmp:HelloResponse", "")
   || soap_body_end_out(_soap)
   || soap_envelope_end_out(_soap)
   || soap_end_send(_soap))
    return _soap->error;

  return soap_closesock(_soap);
}


/**************************************************************************/
/*** Reboot Request/Response                                            ***/
/**************************************************************************/ 
int soap_wait_cwmp__Reboot(struct soap *_soap, struct cwmp__Reboot *_a)
{
  dprintf(LOG_LEVEL2, "soap_wait_cwmp__Reboot\n");
  soap_default_cwmp__Reboot(_soap, _a);
  _soap->encodingStyle = "";
  if (!soap_get_cwmp__Reboot(_soap, _a, "cwmp:Reboot", NULL))
    return _soap->error;
  if (soap_body_end_in(_soap)
   || soap_envelope_end_in(_soap)
   || soap_end_recv(_soap))
    return _soap->error;
  return _soap->error;
}

int Reboot_serve_request(struct soap *_soap, struct cwmp__Reboot *_a)
{
  soap_peek_element(_soap);
  if (!soap_match_tag(_soap, _soap->tag, "cwmp:Reboot"))
    return soap_wait_cwmp__Reboot(_soap, _a);
  return _soap->error = SOAP_NO_METHOD;
}

int send_RebootResponse(struct soap*_soap, const char *_soap_endpoint, struct cwmp__RebootResponse*_RebootResponse)
{
  soap_serializeheader(_soap);
  soap_serialize_cwmp__RebootResponse(_soap, _RebootResponse);
  if (soap_begin_count(_soap))
    return _soap->error;
  if (_soap->mode & SOAP_IO_LENGTH)
  { if (soap_envelope_begin_out(_soap)
     || soap_putheader(_soap)
     || soap_body_begin_out(_soap)
     || soap_put_cwmp__RebootResponse(_soap, _RebootResponse, "cwmp:RebootResponse", "")
     || soap_body_end_out(_soap)
     || soap_envelope_end_out(_soap))
       return _soap->error;
  };
  if (soap_end_count(_soap)
   || soap_connect(_soap,_soap_endpoint, NULL)
   || soap_envelope_begin_out(_soap)
   || soap_putheader(_soap)
   || soap_body_begin_out(_soap)
   || soap_put_cwmp__RebootResponse(_soap, _RebootResponse, "cwmp:RebootResponse", "")
   || soap_body_end_out(_soap)
   || soap_envelope_end_out(_soap)
   || soap_end_send(_soap))
    return _soap->error;

  return soap_closesock(_soap);
}

/*********************************************/
/* ParseReboot_and_SendResponse              */
/* Parse Reboot request and send response    */
/*********************************************/
int ParseReboot_and_SendResponse(struct soap *_soap, const char *_endpoint, struct cwmp__Reboot _tmp_cwmp__RebootRequest)
{
int nRet = 0;
FILE *fdCommand;
struct cwmp__RebootResponse tmp_cwmp__RebootResponse;

  dprintf(LOG_LEVEL2, "ParseReboot_and_SendResponse.\n");
  fdCommand = fopen(COMMANDKEY_FILE, "w");

  soap_default_cwmp__RebootResponse(_soap, &tmp_cwmp__RebootResponse);
  nRet = send_RebootResponse(_soap, _endpoint, &tmp_cwmp__RebootResponse);

  if (nRet == SOAP_OK)
  {
    // Write Boot CommandKey value in config file
    WriteBootCommandKey(fdCommand, _tmp_cwmp__RebootRequest.CommandKey);
  }

  if (fdCommand != NULL) fclose(fdCommand);

  return nRet;
}



/**************************************************************************/
/*** SetParameterValues Request/Response                                ***/
/**************************************************************************/ 
int soap_wait_cwmp__SetParameterValues(struct soap *_soap, struct cwmp__SetParameterValues *_a)
{ 
  dprintf(LOG_LEVEL2, "soap_wait_cwmp__SetParameterValues.\n");
  soap_default_cwmp__SetParameterValues(_soap, _a);
  _soap->encodingStyle = "";
  if (!soap_get_cwmp__SetParameterValues(_soap, _a, "cwmp:SetParameterValues", NULL))
    return _soap->error;
  if (soap_body_end_in(_soap)
   || soap_envelope_end_in(_soap)
   || soap_end_recv(_soap))
    return _soap->error;

  return _soap->error;
}

int SetParameterValues_serve_request(struct soap *_soap, struct cwmp__SetParameterValues *_a)
{
  soap_peek_element(_soap);
  if (!soap_match_tag(_soap, _soap->tag, "cwmp:SetParameterValues"))
    return soap_wait_cwmp__SetParameterValues(_soap, _a);
  return _soap->error = SOAP_NO_METHOD;
}

int send_SetParameterValuesResponse(struct soap*_soap, const char *_soap_endpoint, struct cwmp__SetParameterValuesResponse*_SetParamResponse)
{
  soap_serializeheader(_soap);
  soap_serialize_cwmp__SetParameterValuesResponse(_soap, _SetParamResponse);
  if (soap_begin_count(_soap))
    return _soap->error;
  if (_soap->mode & SOAP_IO_LENGTH)
  { if (soap_envelope_begin_out(_soap)
     || soap_putheader(_soap)
     || soap_body_begin_out(_soap)
     || soap_put_cwmp__SetParameterValuesResponse(_soap, _SetParamResponse, "cwmp:SetParameterValuesResponse", "")
     || soap_body_end_out(_soap)
     || soap_envelope_end_out(_soap))
       return _soap->error;
  };
  if (soap_end_count(_soap)
   || soap_connect(_soap,_soap_endpoint, NULL)
   || soap_envelope_begin_out(_soap)
   || soap_putheader(_soap)
   || soap_body_begin_out(_soap)
   || soap_put_cwmp__SetParameterValuesResponse(_soap, _SetParamResponse, "cwmp:SetParameterValuesResponse", "")
   || soap_body_end_out(_soap)
   || soap_envelope_end_out(_soap)
   || soap_end_send(_soap))
    return _soap->error;

  return soap_closesock(_soap);
}


/**************************************************************************/
/*** Download Request/Response                                          ***/
/**************************************************************************/
int soap_wait_cwmp__Download(struct soap *_soap, struct cwmp__Download *_a)
{ 
  dprintf(LOG_LEVEL2, "soap_wait_cwmp__Download.\n");
  soap_default_cwmp__Download(_soap, _a);
  _soap->encodingStyle = "";
  if (!soap_get_cwmp__Download(_soap, _a, "cwmp:Download", NULL))
    return _soap->error;
  if (soap_body_end_in(_soap)
   || soap_envelope_end_in(_soap)
   || soap_end_recv(_soap))
    return _soap->error;

  return _soap->error;
}

int Download_serve_request(struct soap *_soap, struct cwmp__Download *_a)
{
  soap_peek_element(_soap);
  if (!soap_match_tag(_soap, _soap->tag, "cwmp:Download"))
    return soap_wait_cwmp__Download(_soap, _a);
  return _soap->error = SOAP_NO_METHOD;
}

int send_DownloadResponse(struct soap*_soap, const char *_soap_endpoint, struct cwmp__DownloadResponse*_DownloadResponse)
{
  soap_serializeheader(_soap);
  soap_serialize_cwmp__DownloadResponse(_soap, _DownloadResponse);
  if (soap_begin_count(_soap))
    return _soap->error;
  if (_soap->mode & SOAP_IO_LENGTH)
  { if (soap_envelope_begin_out(_soap)
     || soap_putheader(_soap)
     || soap_body_begin_out(_soap)
     || soap_put_cwmp__DownloadResponse(_soap, _DownloadResponse, "cwmp:DownloadResponse", "")
     || soap_body_end_out(_soap)
     || soap_envelope_end_out(_soap))
       return _soap->error;
  };
  if (soap_end_count(_soap)
   || soap_connect(_soap,_soap_endpoint, NULL)
   || soap_envelope_begin_out(_soap)
   || soap_putheader(_soap)
   || soap_body_begin_out(_soap)
   || soap_put_cwmp__DownloadResponse(_soap, _DownloadResponse, "cwmp:DownloadResponse", "")
   || soap_body_end_out(_soap)
   || soap_envelope_end_out(_soap)
   || soap_end_send(_soap))
    return _soap->error;

  return soap_closesock(_soap);
}
/**************************************************************************/
/*** Upload Request/Response                                          ***/
/**************************************************************************/
int soap_wait_cwmp__Upload(struct soap *_soap, struct cwmp__Upload *_a)
{ 
  dprintf(LOG_LEVEL2, "soap_wait_cwmp__Upload.\n");
  soap_default_cwmp__Upload(_soap, _a);
  _soap->encodingStyle = "";
  if (!soap_get_cwmp__Upload(_soap, _a, "cwmp:Upload", NULL))
    return _soap->error;
  if (soap_body_end_in(_soap)
   || soap_envelope_end_in(_soap)
   || soap_end_recv(_soap))
    return _soap->error;

  return _soap->error;
}


int Upload_serve_request(struct soap *_soap, struct cwmp__Upload *_a)
{
  soap_peek_element(_soap);
  if (!soap_match_tag(_soap, _soap->tag, "cwmp:Upload"))
    return soap_wait_cwmp__Upload(_soap, _a);
  return _soap->error = SOAP_NO_METHOD;
}


int send_UploadResponse(struct soap*_soap, const char *_soap_endpoint, struct cwmp__UploadResponse*_UploadResponse)
{
  soap_serializeheader(_soap);
  soap_serialize_cwmp__UploadResponse(_soap, _UploadResponse);
  if (soap_begin_count(_soap))
    return _soap->error;
  if (_soap->mode & SOAP_IO_LENGTH)
  { if (soap_envelope_begin_out(_soap)
     || soap_putheader(_soap)
     || soap_body_begin_out(_soap)
     || soap_put_cwmp__UploadResponse(_soap, _UploadResponse, "cwmp:UploadResponse", "")
     || soap_body_end_out(_soap)
     || soap_envelope_end_out(_soap))
       return _soap->error;
  };
  if (soap_end_count(_soap)
   || soap_connect(_soap,_soap_endpoint, NULL)
   || soap_envelope_begin_out(_soap)
   || soap_putheader(_soap)
   || soap_body_begin_out(_soap)
   || soap_put_cwmp__UploadResponse(_soap, _UploadResponse, "cwmp:UploadResponse", "")
   || soap_body_end_out(_soap)
   || soap_envelope_end_out(_soap)
   || soap_end_send(_soap))
    return _soap->error;

  return soap_closesock(_soap);
}




/**************************************************************************/
/*** Wait for method request                                            ***/
/**************************************************************************/ 

int Methods_server_request(struct soap *_soap)
{
  soap_peek_element(_soap);
  if (!soap_match_tag(_soap, _soap->tag, "cwmp:GetParameterValues"))
    return RPC_GETPARAMETERVALUES;
  if (!soap_match_tag(_soap, _soap->tag, "cwmp:Reboot"))
    return RPC_REBOOT;
  if (!soap_match_tag(_soap, _soap->tag, "cwmp:SetParameterValues"))
    return RPC_SETPARAMETERVALUES;
  if (!soap_match_tag(_soap, _soap->tag, "cwmp:Download"))
    return RPC_DOWNLOAD;
  if (!soap_match_tag(_soap, _soap->tag, "cwmp:GetRPCMethods"))
  	return RPC_GETMETHODS;
  if (!soap_match_tag(_soap, _soap->tag, "cwmp:GetParameterNames"))
  	return RPC_GETPARAMETERNAMES;
  if (!soap_match_tag(_soap, _soap->tag, "cwmp:FactoryReset"))
  	return RPC_FACTORYRESET;
  if (!soap_match_tag(_soap, _soap->tag, "cwmp:Upload"))
    return RPC_UPLOAD;
  if (!soap_match_tag(_soap, _soap->tag, "cwmp:AddObject"))  //add by wangjr 091124
    return RPC_ADDOBJECT;
  if (!soap_match_tag(_soap, _soap->tag, "cwmp:DeleteObject")) //add by wangjr 091124
    return RPC_DELETEOBJECT;
  if (!soap_match_tag(_soap, _soap->tag, "cwmp:GetParameterAttributes"))  //add by wangjr 091124
    return RPC_GETPARAMETERATTRIBUTES;
  if (!soap_match_tag(_soap, _soap->tag, "cwmp:SetParameterAttributes"))  //add by wangjr 091124
    return RPC_SETPARAMETERATTRIBUTES;
  return _soap->error = SOAP_NO_METHOD;
}

int wait_RPCRequest(struct soap*_soap)
{
int nRet = _soap->error;

  if (soap_begin_recv(_soap) == SOAP_OK)
  {
    if (_soap->length == 0) return SOAP_STOP;

    if (soap_envelope_begin_in(_soap)
      || soap_recv_header(_soap)
      || soap_body_begin_in(_soap))
    {
      return soap_send_fault(_soap);
    }

    nRet = Methods_server_request(_soap);

    if (_soap->fserveloop && _soap->fserveloop(_soap))
    {
      return soap_send_fault(_soap);
    }

    return nRet;
  }
  return _soap->error;
}

