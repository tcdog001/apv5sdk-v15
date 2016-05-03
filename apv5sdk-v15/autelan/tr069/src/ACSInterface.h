
/**********************************************************************************
* Copyright (c) 2008-2011  Beijing Autelan Technology Co. Ltd.
* All rights reserved.
*
* filename: ACSInterface.c
* description:  implementation for the header file  for SOAP RPC methods
* 
*
* 
************************************************************************************/
#ifndef _ACSINTERFACE_H
#define _ACSINTERFACE_H

// RPC methods supported
#define RPC_GETPARAMETERVALUES  4000
#define RPC_REBOOT              4001
#define RPC_SETPARAMETERVALUES  4002
#define RPC_DOWNLOAD            4003
#define RPC_GETMETHODS          4004
#define RPC_GETPARAMETERNAMES   4005
#define RPC_FACTORYRESET        4006
#define RPC_UPLOAD              4007
#define RPC_ADDOBJECT           4008 //add by wangjr 091124
#define RPC_DELETEOBJECT        4009 //add by wangjr 091124
#define RPC_GETPARAMETERATTRIBUTES        4010 //add by wangjr 091124
#define RPC_SETPARAMETERATTRIBUTES        4011 //add by wangjr 091124
// Default Inform Request forced parameters
#define DEFAULT_MANUFACTURER                                    "Autelan"
//#define DEFAULT_MANUFACTURER									"JS"
#define DEFAULT_O_U_I                                           "001F64"
//#define DEFAULT_O_U_I                                           					"001099"
#define DEFAULT_SERIALNUMBER                                    "01010049A12109300019"
//#define DEFAULT_SERIALNUMBER                                   				"01010049A12109300019"
#define DEFAULT_PASSWD                                          "password"
#define DEFAULT_REALM                                           "tr069-main"
#define DEFAULT_WWW_USERNAME                                    "admin"
#define DEFAULT_WWW_PASSWORD                                    "admin"
#define DEFAULT_WWW_REALM                                       "tr069-test"
#define DEFAULT_PRODUCTCLASS                                    "Product Class"

//get vendor info

typedef struct st_vendor_info{
    char vendor_name[32];
    char vendor_oui[32];
    char product_class[32];
    char serial_number[32];
}VENDORINFO;

extern VENDORINFO st_vendor;
int get_vendor_info(VENDORINFO *vendor_info);

// print gSoap error information
void print_gSoap_error(int _nError);

// return timezone letter
char cUTCLetter(long _TimeZone);

// Format date time in a string
void szFormatLocalTime(time_t _tDateTime, char *_szDate);

// Set InformRequest parameters
int SetInformRequestParameter(struct cwmp__Inform *_soap_Inform, LIST_PARAM_VALUE *_list_param_value, char *_EventCode, char *_EventCmdKey);

// Send an Inform Request to ACS - Encode SOAP message
int call_InformRequest(struct soap*_soap, const char *_soap_endpoint, LIST_PARAM_VALUE *_list_param_value, LIST_PARAM_VALUE *_list_notification, char *_EventCode, char *_EventCmdKey);

// Wait for an Inform Response from ACS - SOAP message
int wait_InformResponse(struct soap*_soap, struct InformResponse *_response);


// Send a Transfer Complete Request to ACS - Encode SOAP message
int call_TransferComplete(struct soap*_soap, const char *_soap_endpoint, char *_EventCmdKey, unsigned int _FaultCode, char *_szFaultString, time_t _timeStart);

// Wait for a Transfer Complete Response from ACS - SOAP message
int wait_TransferCompleteResponse(struct soap*_soap);

/**************************************************************************/
/*** GetRPCMethods Request/Response                                            ***/
/**************************************************************************/ 
int soap_wait_cwmp__GetRPCMethods(struct soap *_soap, struct cwmp__GetRPCMethods *_a);
int ParseGetRPCMethods_and_SendResponse(struct soap *_soap, const char *_endpoint, struct cwmp__GetRPCMethods _tmp_cwmp__GetParameterValuesRequest);
int send_GetRPCMethodsResponse(struct soap*_soap, const char *_soap_endpoint, struct cwmp__GetRPCMethodsResponse*_GetMethodResponse);

/**************************************************************************/
/*** GetParameterNames Request/Response                                ***/
/**************************************************************************/
int soap_wait_cwmp__GetParameterNames(struct soap *_soap, struct cwmp__GetParameterNames *_a);
int GetParameterNames_serve_request(struct soap *_soap, struct cwmp__GetParameterNames *_a);



/**************************************************************************/
/*** GetParameterValues Request/Response                                ***/
/**************************************************************************/
int soap_wait_cwmp__GetParameterValues(struct soap *_soap, struct cwmp__GetParameterValues *_a);
int GetParameterValues_serve_request(struct soap *_soap, struct cwmp__GetParameterValues *_a);
int send_GetParameterValuesResponse(struct soap*_soap, const char *_soap_endpoint, struct cwmp__GetParameterValuesResponse*_GetParamResponse);


/**************************************************************************/
/*** Reboot Request/Response                                            ***/
/**************************************************************************/ 
int soap_wait_cwmp__Reboot(struct soap *_soap, struct cwmp__Reboot *_a);
int Reboot_serve_request(struct soap *_soap, struct cwmp__Reboot *_a);
int send_RebootResponse(struct soap*_soap, const char *_soap_endpoint, struct cwmp__RebootResponse*_RebootResponse);
// Parse Reboot request and send response
int ParseReboot_and_SendResponse(struct soap *_soap, const char *_endpoint, struct cwmp__Reboot _tmp_cwmp__RebootRequest);


/**************************************************************************/
/*** SetParameterValues Request/Response                                ***/
/**************************************************************************/ 
int soap_wait_cwmp__SetParameterValues(struct soap *_soap, struct cwmp__SetParameterValues *_a);
int SetParameterValues_serve_request(struct soap *_soap, struct cwmp__SetParameterValues *_a);
int send_SetParameterValuesResponse(struct soap*_soap, const char *_soap_endpoint, struct cwmp__SetParameterValuesResponse*_SetParamResponse);


/**************************************************************************/
/*** Download Request/Response                                          ***/
/**************************************************************************/
int soap_wait_cwmp__Download(struct soap *_soap, struct cwmp__Download *_a);
int Download_serve_request(struct soap *_soap, struct cwmp__Download *_a);
int send_DownloadResponse(struct soap*_soap, const char *_soap_endpoint, struct cwmp__DownloadResponse*_DownloadResponse);

/**************************************************************************/
/*** Upload Request/Response                                          ***/
/**************************************************************************/

int soap_wait_cwmp__Upload(struct soap *_soap, struct cwmp__Upload *_a);
int Upload_serve_request(struct soap *_soap, struct cwmp__Upload *_a);
int send_UploadResponse(struct soap*_soap, const char *_soap_endpoint, struct cwmp__UploadResponse*_UploadResponse);




/**************************************************************************/
/*** Wait for method request                                            ***/
/**************************************************************************/ 
int Methods_server_request(struct soap *_soap);
int wait_RPCRequest(struct soap*_soap);

#endif // _ACSINTERFACE_H
