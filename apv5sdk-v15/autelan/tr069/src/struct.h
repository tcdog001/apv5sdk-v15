
/**********************************************************************************
* Copyright (c) 2008-2011  Beijing Autelan Technology Co. Ltd.
* All rights reserved.
*
* filename: Struct.h
* description:  implementation for gsoap cwmp service encoding
* 
*
* 
************************************************************************************/
#import "dom.h"

//gsoap cwmp service style: rpc
//gsoap cwmp service encoding: encoded
//gsoap cwmp service location: http://localhost:8060
//gsoap cwmp service namespace: urn:dslforum-org:cwmp-1-0
//gsoap cwmp schema namespace: urn:dslforum-org:cwmp-1-0
typedef char *xsd__string; // encode xsd__string value as the xsd:string schema type 
enum xsd__boolean {false_, true_};
typedef time_t xsd__dateTime;

struct SOAP_ENV__Header 
 { 
    char *cwmp__ID; 
    char *cwmp__NoMoreRequests; 
 };

struct DeviceIdStruct {
  char* Manufacturer;
  char* OUI;
  char* ProductClass;
  char* SerialNumber;
} ;

struct cwmp__EventStruct {
  xsd__string EventCode;
  xsd__string CommandKey;
} ;

struct cwmp__FaultStruct  //add by wangjr for download Fault 091124
{
  unsigned int FaultCode;
  xsd__string FaultString;
};

struct __anytype {
int __type; // the SOAP_TYPE pointed to by p 
void *Value;
} ; 

struct cwmp__ParameterValueStruct {
  xsd__string Name; /* required element of type xsd:string */
  struct __anytype Value;
};

/*add by wangjr for SetParamValuesFault and  GetParamValuesFault 091124 */
struct cwmp__SetParameterValuesFault
{
  xsd__string ParameterName;
  unsigned int FaultCode;
  xsd__string FaultString;
};


struct cwmp__GetParameterValuesFault
{
  xsd__string ParameterName;
  unsigned int FaultCode;
  xsd__string FaultString;
};

struct cwmp__SetFault
{
  unsigned int FaultCode;
  xsd__string FaultString;
  struct cwmp__SetParameterValuesFault SetParameterValuesFault;
};



struct cwmp__Fault
{
  unsigned int FaultCode;
  xsd__string FaultString;
  struct cwmp__GetParameterValuesFault GetParameterValuesFault;
};


struct cwmp__SetDetail
{
  struct cwmp__SetFault cwmp__Fault;
};


struct cwmp__Detail
{
  struct cwmp__Fault cwmp__Fault;
};


struct cwmp__SetFaultResponse {
	               xsd__string faultcode;
				   xsd__string faultstring;
				   struct cwmp__SetDetail detail;
};


struct cwmp__FaultResponse {
	               xsd__string faultcode;
				   xsd__string faultstring;
				   struct cwmp__Detail detail;
};
/*add by wangjr for SetParamValuesFault and  GetParamValuesFault 091124 */

struct __ParameterList {
    struct cwmp__ParameterValueStruct *__ptrParameterValueStruct; // pointer to array 
    int   __size; // number of elements pointed to 
};


struct cwmp__ParameterAttributeStruct {  //add by wangjr 091124
  xsd__string Name; /* required element of type xsd:string */
  int Notification;
  xsd__string AccessList;
};

struct __ParameterAttributeList {  //add by wangjr 091124
    struct cwmp__ParameterAttributeStruct *__ptrParameterAttributeStruct; // pointer to array 
    int   __size; // number of elements pointed to 
};

struct cwmp__SetParameterAttributesStruct { //add by wangjr 091124
  xsd__string Name; /* required element of type xsd:string */
  enum xsd__boolean NotificationChange;
  int Notification;
  enum xsd__boolean AccessListChange;
  xsd__string AccessList;
};


struct __SetParameterAttributesList {  //add by wangjr 091124
    struct cwmp__SetParameterAttributesStruct *__ptrSetParameterAttributesStruct; // pointer to array 
    int   __size; // number of elements pointed to 
};

struct __MethodList {
    xsd__string*__ptrstring; // pointer to array 
    int   __size; // number of elements pointed to 
};

struct __EventList {
    struct cwmp__EventStruct *__ptrEventStruct; // pointer to array 
    int   __size; // number of elements pointed to 
};

struct InformResponse { 
  int MaxEnvelopes; 
} ;

int cwmp__Inform(struct DeviceIdStruct DeviceId,
                 struct __EventList *Event,
                 int MaxEnvelopes,
                 time_t CurrentTime,
                 unsigned int RetryCount,
                 struct __ParameterList *ParameterList,
                 struct InformResponse *Response);


/*struct cwmp__GetRPCMethodsResponse{
				 struct __MethodList MethodList;
};*/

/*
int cwmp__GetRPCMethods( void *_, 
//				 struct cwmp__GetRPCMethodsResponse *Response); 
				 struct __MethodList *MethodList );
*/

int cwmp__GetRPCMethods(void *_,
                 struct __MethodList *MethodList);

struct cwmp__ParameterInfoStruct{
	xsd__string Name;
	int Writable;
};

struct __ParameterInfoList{
	struct cwmp__ParameterInfoStruct *__ptrParameterInfoStruct;
	int __size;
};

int cwmp__GetParameterNames(xsd__string ParameterPath, enum xsd__boolean NextLevel, 
				struct __ParameterInfoList *ParameterList);

struct __ParameterNames {
    xsd__string *__ptrstring; // pointer to array 
    int   __size; // number of elements pointed to 
};

int cwmp__GetParameterValues(struct __ParameterNames ParameterNames,
                 struct __ParameterList *ParameterList);

int cwmp__GetParameterAttributes(struct __ParameterNames ParameterNames,
                 struct __ParameterAttributeList *ParameterList);  //add by wangjr 091124

int cwmp__SetParameterAttributes(struct __SetParameterAttributesList ParameterList, struct cwmp__SetParameterAttributesResponse { } *out);  //add by wangjr 091124
int cwmp__Hello(xsd__string CommandKey, struct cwmp__HelloResponse { } *out); //add by wangjr 091124
// Reboot Response with empty struct !
int cwmp__Reboot(xsd__string CommandKey, struct cwmp__RebootResponse { } *out);

// FactoryReset Response with empty struct !
int cwmp__FactoryReset( void *_ , struct cwmp__FactoryResetResponse { } *out);
struct cwmp__DeleteObjectResponse {
                  int Status;
};  //add by wangjr 091124

// DeleteObject Response with empty struct !
int cwmp__DeleteObject( xsd__string ObjectName,xsd__string ParameterKey , struct cwmp__DeleteObjectResponse *out);//modify by wangjr 091124

//int cwmp__DeleteObject( void *_ , struct cwmp__DeleteObjectResponse { } *out);//modify by wangjr

struct cwmp__AddObjectResponse {
                  int InstanceNumber;
                  int Status;
};

// AddObject Response with empty struct !
int cwmp__AddObject(xsd__string ObjectName,xsd__string ParameterKey , struct cwmp__AddObjectResponse *out);

// SetParameterValues
int cwmp__SetParameterValues(struct __ParameterList ParameterList, xsd__string ParameterKey, int *Status);


// Download
//
// Download response
struct cwmp__DownloadResponse {
                  int Status;
                  time_t StartTime;
                  time_t CompleteTime;
};

struct cwmp__UploadResponse {
                  int Status;
                  time_t StartTime;
                  time_t CompleteTime;
};


int cwmp__Download(xsd__string CommandKey,
                  xsd__string FileType,
                  xsd__string URL,
                  xsd__string Username,
                  xsd__string Password,
                  unsigned int FileSize,
                  xsd__string TargetFileName,
                  unsigned int DelaySeconds,
                  xsd__string SuccessURL,
                  xsd__string FailureURL,
                  struct cwmp__DownloadResponse *Response);

int cwmp__Upload(xsd__string CommandKey,
                  xsd__string FileType,
                  xsd__string URL,
                  xsd__string Username,
                  xsd__string Password,
                  unsigned int FileSize,
                  xsd__string TargetFileName,
                  unsigned int DelaySeconds,
                  xsd__string SuccessURL,
                  xsd__string FailureURL,
                  struct cwmp__UploadResponse *Response);



// TransferComplete


int cwmp__TransferComplete(xsd__string CommandKey, 
                          struct cwmp__FaultStruct FaultStruct,
                          time_t StartTime,
                          time_t CompleteTime,
                          struct cwmp__TransferCompleteResponse { } *out);

//xsd__anyType*     any;
