
/**********************************************************************************
* Copyright (c) 2008-2011  Beijing Autelan Technology Co. Ltd.
* All rights reserved.
*
* filename: messages.h
* description:  implementation for messages relative functions
* - creation of request, response, error, notify.. messages
* - extract of data in messages
* 
************************************************************************************/

#ifndef _MESSAGES_H
#define _MESSAGES_H

#include "StubH.h"
#include "const.h"
#include "tools.h"

typedef struct structThreadData {
  struct soap *soap;
  STRUCT_MESSAGE *msg;
} STRUCT_THREAD_DATA;

char szTR_69_ERROR[MAX_BUFFER];

void InitSoapStruct(struct soap *_soap, struct Namespace *_namespaces);

int CreateResponse(char *_soap_endpoint, short _Id);

int CreateResponseWithValues(char *_soap_endpoint, short _Id, char *_szValue);

int CreateResponseWithMultipleValues(char *_soap_endpoint, short _Id, LIST_PARAM_VALUE *_list_param_value);

int CreateErrorResponse(char *_soap_endpoint, short _Id);

int CreateRequest(char *_soap_endpoint, short _Id, const char *_szFunctionName, const char *_szParameter, const char *_szValue);

int CreateGetPassiveParametersRequest(char *_soap_endpoint, short _Id, const char *_szFunctionName);
int CreateGetActiveParametersRequest(char *_soap_endpoint, short _Id, const char *_szFunctionName);  //add by wangjr 091124
int CreateGetParameterNamesRequest( char *_soap_endpoint, short _Id, const char *_szFunctionName, char *_very_path, char *_level);


int CreateNotify(char *_soap_endpoint, short _Id, const char *_szParameter, const char *_szValue);

int ExtractXMLTags(struct soap *_soap, STRUCT_MESSAGE *_message);
int ExtractXMLTagsWithMultipleParams(struct soap *_soap, STRUCT_MESSAGE *_message, LIST_PARAM_VALUE *_list_param_value);

int ProcessMessage(STRUCT_MESSAGE *_message, enum methodName *_methodName, enum hostType *_type, enum functionName *_fction);

ACCESSCONDITION getAccessCondition(char *_szValue);

char *getErrorString(TR69_ERROR _error);

#endif // _MESSAGES_H
