
/**********************************************************************************
* Copyright (c) 2008-2011  Beijing Autelan Technology Co. Ltd.
* All rights reserved.
*
* filename: messages.c 
* description:  implementation for messages relative functions 
* - creation of request, response, error, notify.. messages  
* - extract of data in messages 
* 
************************************************************************************/

#include <stdio.h>

#include "const.h"
#include "messages.h"


/***************************************************************************/
/*** InitSoapStruct                                                      ***/
/*** Initiailize default soap context                                    ***/
/***************************************************************************/ 
void InitSoapStruct(struct soap *_soap, struct Namespace *_namespaces)
{
  soap_init2(_soap,SOAP_ENC_XML|SOAP_IO_LENGTH|SOAP_XML_INDENT, SOAP_ENC_XML|SOAP_IO_LENGTH|SOAP_XML_INDENT);
  _soap->accept_timeout = 60;
  _soap->max_keep_alive = 100; // max keep-alive sequence
  _soap->recv_timeout = 60; // Timeout after 5 minutes stall on recv 
  _soap->send_timeout = 60; // Timeout after 1 minute stall on send 
  soap_set_namespaces(_soap, _namespaces);
}

/***************************************************************************/
/*** CreateResponse                                                      ***/
/*** Create XML Response                                                 ***/
/***************************************************************************/ 
int CreateResponse(char *_soap_endpoint, short _Id)
{
int nRet = 0;
struct soap soap;

char szline[MAX_BUFFER];
struct Namespace empty_namespaces[] = { { NULL } };

  dprintf(LOG_LEVEL2, "CreateResponse _soap_endpoint: %s, _Id: %d\n", _soap_endpoint, _Id);

  InitSoapStruct(&soap, empty_namespaces);
  soap_connect_command(&soap, SOAP_POST, _soap_endpoint, "");

  soap_element_begin_out(&soap, CONST_MESSAGE, 0, NULL);
  soap_element_begin_out(&soap, "NAME", 0, NULL);
  sprintf(szline, "%s", CONST_RESPONSE);
  soap_send_raw(&soap, szline, strlen(szline));
  soap_element_end_out(&soap, "NAME");
  soap_element_begin_out(&soap, "Id", 0, NULL);
  sprintf(szline, "%d", _Id);
  soap_send_raw(&soap, szline, strlen(szline));
  soap_element_end_out(&soap, "Id");
  soap_element_end_out(&soap, CONST_MESSAGE);

  soap_end_send(&soap);
  soap_destroy((struct soap*)&soap); // dealloc C++ data
  soap_end((struct soap*)&soap); // dealloc data and clean up
  soap_done(&soap); // detach soap struct
  return nRet;
}


/***************************************************************************/
/*** CreateResponseWithValues                                            ***/
/*** Create XML Response with values                                     ***/
/***************************************************************************/ 
int CreateResponseWithValues(char *_soap_endpoint, short _Id, char *_szValue)
{
int nRet = 0;
struct soap soap;

char szline[MAX_BUFFER];
struct Namespace empty_namespaces[] = { { NULL } };

  dprintf(LOG_LEVEL2, "CreateResponseWithValues _soap_endpoint: %s, _Id: %d, _szValue: %s\n", _soap_endpoint, _Id, _szValue);

  InitSoapStruct(&soap, empty_namespaces);
  soap_connect_command(&soap, SOAP_POST, _soap_endpoint, "");

  soap_element_begin_out(&soap, CONST_MESSAGE, 0, NULL);
  soap_element_begin_out(&soap, "NAME", 0, NULL);
  sprintf(szline, "%s", CONST_RESPONSE);
  soap_send_raw(&soap, szline, strlen(szline));
  soap_element_end_out(&soap, "NAME");
  soap_element_begin_out(&soap, "Id", 0, NULL);
  sprintf(szline, "%d", _Id);
  soap_send_raw(&soap, szline, strlen(szline));
  soap_element_end_out(&soap, "Id");
  soap_element_begin_out(&soap, "Value", 0, NULL);
  soap_send_raw(&soap, _szValue, strlen(_szValue));
  soap_element_end_out(&soap, "Value");
  soap_element_end_out(&soap, CONST_MESSAGE);

  soap_end_send(&soap);
  soap_destroy((struct soap*)&soap); // dealloc C++ data
  soap_end((struct soap*)&soap); // dealloc data and clean up
  soap_done(&soap); // detach soap struct
  return nRet;
}


/***************************************************************************/
/*** CreateResponseWithMultipleValues                                    ***/
/*** Create XML Response with multiple values                            ***/
/***************************************************************************/ 
int CreateResponseWithMultipleValues(char *_soap_endpoint, short _Id, LIST_PARAM_VALUE *_list_param_value)
{
int nRet = 0;
struct soap soap;

char szline[MAX_BUFFER];
struct Namespace empty_namespaces[] = { { NULL } };
LIST_PARAM_VALUE *elt = _list_param_value;

  dprintf(LOG_LEVEL2, "CreateResponseWithMultipleValues _soap_endpoint: %s, _Id: %d, _list_param_value: %p\n", _soap_endpoint, _Id, _list_param_value);

  print(_list_param_value);

  InitSoapStruct(&soap, empty_namespaces);
  soap_connect_command(&soap, SOAP_POST, _soap_endpoint, "");

  soap_element_begin_out(&soap, CONST_MESSAGE, 0, NULL);
  soap_element_begin_out(&soap, "NAME", 0, NULL);
  sprintf(szline, "%s", CONST_RESPONSE);
  soap_send_raw(&soap, szline, strlen(szline));
  soap_element_end_out(&soap, "NAME");
  soap_element_begin_out(&soap, "Id", 0, NULL);
  sprintf(szline, "%d", _Id);
  soap_send_raw(&soap, szline, strlen(szline));
  soap_element_end_out(&soap, "Id");
  while (elt != NULL) // list of (param, value)
  {
    soap_element_begin_out(&soap, "Parameter", 0, NULL);
    soap_send_raw(&soap, elt->szParameter, strlen(elt->szParameter));
    soap_element_end_out(&soap, "Parameter");

    soap_element_begin_out(&soap, "Value", 0, NULL);
    soap_send_raw(&soap, elt->szValue, strlen(elt->szValue));
    soap_element_end_out(&soap, "Value");
    elt = elt->next;
  }
  soap_element_end_out(&soap, CONST_MESSAGE);

  soap_end_send(&soap);
  soap_destroy((struct soap*)&soap); // dealloc C++ data
  soap_end((struct soap*)&soap); // dealloc data and clean up
  soap_done(&soap); // detach soap struct
  return nRet;
}


/***************************************************************************/
/*** CreateErrorResponse                                                 ***/
/*** Create XML Error Response                                           ***/
/***************************************************************************/ 
int CreateErrorResponse(char *_soap_endpoint, short _Id)
{
int nRet = 0;
struct soap soap;

char szline[MAX_BUFFER];
struct Namespace empty_namespaces[] = { { NULL } };

  dprintf(LOG_LEVEL2, "CreateErrorResponse _soap_endpoint: %s, _Id: %d\n", _soap_endpoint, _Id);

  InitSoapStruct(&soap, empty_namespaces);
  soap_connect_command(&soap, SOAP_POST, _soap_endpoint, "");

  soap_element_begin_out(&soap, CONST_MESSAGE, 0, NULL);
  soap_element_begin_out(&soap, "NAME", 0, NULL);
  sprintf(szline, "%s", CONST_ERROR);
  soap_send_raw(&soap, szline, strlen(szline));
  soap_element_end_out(&soap, "NAME");
  soap_element_begin_out(&soap, "Id", 0, NULL);
  sprintf(szline, "%d", _Id);
  soap_send_raw(&soap, szline, strlen(szline));
  soap_element_end_out(&soap, "Id");
  soap_element_end_out(&soap, CONST_MESSAGE);

  soap_end_send(&soap);
  soap_destroy((struct soap*)&soap); // dealloc C++ data
  soap_end((struct soap*)&soap); // dealloc data and clean up
  soap_done(&soap); // detach soap struct
  return nRet;
}


/***************************************************************************/
/*** CreateRequest                                                       ***/
/*** Create XML Request                                                  ***/
/***************************************************************************/
int CreateRequest(char *_soap_endpoint, short _Id, const char *_szFunctionName, const char *_szParameter, const char *_szValue)
{
int nRet = 0;
struct soap soap;

char szline[MAX_BUFFER];
struct Namespace empty_namespaces[] = { { NULL } };

// ATTENTION : Don't use SOAP_IO_KEEPALIVE during message
// because each message must correspond to one TCP connection

  dprintf(LOG_LEVEL2, "CreateRequest _soap_endpoint: %s, _Id: %d, _szFunctionName: %s, _szParameter: %s, _szValue: %s\n", _soap_endpoint, _Id,  _szFunctionName, _szParameter, _szValue);

  InitSoapStruct(&soap, empty_namespaces);
  soap_connect_command(&soap, SOAP_POST, _soap_endpoint, "");

  soap_element_begin_out(&soap, CONST_MESSAGE, 0, NULL);
  soap_element_begin_out(&soap, "NAME", 0, NULL);
  sprintf(szline, "%s", CONST_REQUEST);
  soap_send_raw(&soap, szline, strlen(szline));
  soap_element_end_out(&soap, "NAME");
  soap_element_begin_out(&soap, "Id", 0, NULL);
  sprintf(szline, "%d", _Id);
  soap_send_raw(&soap, szline, strlen(szline));
  soap_element_end_out(&soap, "Id");
  soap_element_begin_out(&soap, "Function", 0, NULL);
  soap_send_raw(&soap, _szFunctionName, strlen(_szFunctionName));
  soap_element_end_out(&soap, "Function");
  soap_element_begin_out(&soap, "Parameter", 0, NULL);
  soap_send_raw(&soap, _szParameter, strlen(_szParameter));
  soap_element_end_out(&soap, "Parameter");
  if (_szValue != NULL)
  {
    soap_element_begin_out(&soap, "Value", 0, NULL);
    if (strcmp(_szValue, "") != 0) soap_send_raw(&soap, _szValue, strlen(_szValue));
    soap_element_end_out(&soap, "Value");
  }
  soap_element_end_out(&soap, CONST_MESSAGE);

  soap_end_send(&soap);
  soap_destroy((struct soap*)&soap); // dealloc C++ data
  soap_end((struct soap*)&soap); // dealloc data and clean up
  soap_done(&soap); // detach soap struct
  return nRet;
}


/***************************************************************************/
/*** CreateGetNamesRequest                                   ***/
/*** Create XML Request                                                  ***/
/***************************************************************************/

int CreateGetParameterNamesRequest( char *_soap_endpoint, short _Id, const char *_szFunctionName,
											 char *_very_path, char *_level)
{
	int nRet = 0;
	struct soap soap;

	char szline[MAX_BUFFER];
	struct Namespace empty_namespaces[] = { { NULL } };

	// ATTENTION : Don't use SOAP_IO_KEEPALIVE during message
	// because each message must correspond to one TCP connection
	dprintf(LOG_LEVEL2, " CreateGetParameterNamesRequest _soap_endpoint: %s, _Id: %d, _szFunctionName: %s\n", _soap_endpoint, _Id,  _szFunctionName);

	InitSoapStruct(&soap, empty_namespaces);
	soap_connect_command(&soap, SOAP_POST, _soap_endpoint, "");

	soap_element_begin_out(&soap, CONST_MESSAGE, 0, NULL);
	soap_element_begin_out(&soap, "NAME", 0, NULL);
	sprintf(szline, "%s", CONST_REQUEST);
	soap_send_raw(&soap, szline, strlen(szline));
	soap_element_end_out(&soap, "NAME");
	soap_element_begin_out(&soap, "Id", 0, NULL);
	sprintf(szline, "%d", _Id);
	soap_send_raw(&soap, szline, strlen(szline));
	soap_element_end_out(&soap, "Id");
	soap_element_begin_out(&soap, "Function", 0, NULL);
	soap_send_raw(&soap, _szFunctionName, strlen(_szFunctionName));
	soap_element_end_out(&soap, "Function");
	soap_element_begin_out(&soap, "Parameter", 0, NULL);
	soap_send_raw(&soap, _very_path, strlen(_very_path));
	soap_element_end_out(&soap, "Parameter");
	soap_element_begin_out(&soap, "Value", 0, NULL);
	soap_send_raw(&soap, _level, strlen(_level));
	soap_element_end_out(&soap, "Value");

	soap_element_end_out(&soap, CONST_MESSAGE);

	soap_end_send(&soap);
	soap_destroy((struct soap*)&soap); // dealloc C++ data
	soap_end((struct soap*)&soap); // dealloc data and clean up
	soap_done(&soap); // detach soap struct
	return nRet;
}


/***************************************************************************/
/*** CreateGetPassiveParametersRequest                                   ***/
/*** Create XML Request                                                  ***/
/***************************************************************************/
int CreateGetPassiveParametersRequest(char *_soap_endpoint, short _Id, const char *_szFunctionName)
{
int nRet = 0;
struct soap soap;

char szline[MAX_BUFFER];
struct Namespace empty_namespaces[] = { { NULL } };

// ATTENTION : Don't use SOAP_IO_KEEPALIVE during message
// because each message must correspond to one TCP connection
  dprintf(LOG_LEVEL2, " Create special Request _soap_endpoint: %s, _Id: %d, _szFunctionName: %s\n", _soap_endpoint, _Id,  _szFunctionName);

  InitSoapStruct(&soap, empty_namespaces);
  soap_connect_command(&soap, SOAP_POST, _soap_endpoint, "");

  soap_element_begin_out(&soap, CONST_MESSAGE, 0, NULL);
  soap_element_begin_out(&soap, "NAME", 0, NULL);
  sprintf(szline, "%s", CONST_REQUEST);
  soap_send_raw(&soap, szline, strlen(szline));
  soap_element_end_out(&soap, "NAME");
  soap_element_begin_out(&soap, "Id", 0, NULL);
  sprintf(szline, "%d", _Id);
  soap_send_raw(&soap, szline, strlen(szline));
  soap_element_end_out(&soap, "Id");
  soap_element_begin_out(&soap, "Function", 0, NULL);
  soap_send_raw(&soap, _szFunctionName, strlen(_szFunctionName));
  soap_element_end_out(&soap, "Function");
  soap_element_end_out(&soap, CONST_MESSAGE);
  soap_end_send(&soap);
  soap_destroy((struct soap*)&soap); // dealloc C++ data
  soap_end((struct soap*)&soap); // dealloc data and clean up
  soap_done(&soap); // detach soap struct
  return nRet;
}



/***************************************************************************/
/*** CreateGetActiveParametersRequest                                   ***/
/*** Create XML Request   add by wangjr 091124                                                 ***/
/***************************************************************************/
int CreateGetActiveParametersRequest(char *_soap_endpoint, short _Id, const char *_szFunctionName)
{
int nRet = 0;
struct soap soap;

char szline[MAX_BUFFER];
struct Namespace empty_namespaces[] = { { NULL } };

// ATTENTION : Don't use SOAP_IO_KEEPALIVE during message
// because each message must correspond to one TCP connection
  dprintf(LOG_LEVEL2, " Create special Request _soap_endpoint: %s, _Id: %d, _szFunctionName: %s\n", _soap_endpoint, _Id,  _szFunctionName);

  InitSoapStruct(&soap, empty_namespaces);
  soap_connect_command(&soap, SOAP_POST, _soap_endpoint, "");

  soap_element_begin_out(&soap, CONST_MESSAGE, 0, NULL);
  soap_element_begin_out(&soap, "NAME", 0, NULL);
  sprintf(szline, "%s", CONST_REQUEST);
  soap_send_raw(&soap, szline, strlen(szline));
  soap_element_end_out(&soap, "NAME");
  soap_element_begin_out(&soap, "Id", 0, NULL);
  sprintf(szline, "%d", _Id);
  soap_send_raw(&soap, szline, strlen(szline));
  soap_element_end_out(&soap, "Id");
  soap_element_begin_out(&soap, "Function", 0, NULL);
  soap_send_raw(&soap, _szFunctionName, strlen(_szFunctionName));
  soap_element_end_out(&soap, "Function");
  soap_element_end_out(&soap, CONST_MESSAGE);
  soap_end_send(&soap);
  soap_destroy((struct soap*)&soap); // dealloc C++ data
  soap_end((struct soap*)&soap); // dealloc data and clean up
  soap_done(&soap); // detach soap struct
  return nRet;
}
/*** CreateNotify                                                        ***/
/*** Create XML Notify                                                   ***/
/***************************************************************************/
int CreateNotify(char *_soap_endpoint, short _Id, const char *_szParameter, const char *_szValue)
{
int nRet = 0;
struct soap soap;

char szline[MAX_BUFFER];
struct Namespace empty_namespaces[] = { { NULL } };

// ATTENTION : Don't use SOAP_IO_KEEPALIVE during message
// because each message must correspond to one TCP connection

  dprintf(LOG_LEVEL2, "CreateNotify _soap_endpoint: %s, _Id: %d, _szParameter: %s, _szValue: %s\n", _soap_endpoint, _Id, _szParameter, _szValue);

  InitSoapStruct(&soap, empty_namespaces);
  soap_connect_command(&soap, SOAP_POST, _soap_endpoint, "");

  soap_element_begin_out(&soap, CONST_MESSAGE, 0, NULL);
  soap_element_begin_out(&soap, "NAME", 0, NULL);
  sprintf(szline, "%s", CONST_NOTIFY);
  soap_send_raw(&soap, szline, strlen(szline));
  soap_element_end_out(&soap, "NAME");
  soap_element_begin_out(&soap, "Id", 0, NULL);
  sprintf(szline, "%d", _Id);
  soap_send_raw(&soap, szline, strlen(szline));
  soap_element_end_out(&soap, "Id");
  soap_element_begin_out(&soap, "Parameter", 0, NULL);
  soap_send_raw(&soap, _szParameter, strlen(_szParameter));
  soap_element_end_out(&soap, "Parameter");
  if (_szValue != NULL)
  {
    soap_element_begin_out(&soap, "Value", 0, NULL);
    if (strcmp(_szValue, "") != 0) soap_send_raw(&soap, _szValue, strlen(_szValue));
    soap_element_end_out(&soap, "Value");
  }
  soap_element_end_out(&soap, CONST_MESSAGE);

  soap_end_send(&soap);
  soap_destroy((struct soap*)&soap); // dealloc C++ data
  soap_end((struct soap*)&soap); // dealloc data and clean up
  soap_done(&soap); // detach soap struct
  return nRet;
}



/***************************************************************************/
/*** Extract XML tags from received message                              ***/
/*** - fill STRUCT_MESSAGE var                                           ***/
/*** - create list of (param,value)                                      ***/
/***************************************************************************/ 
int ExtractXMLTagsWithMultipleParams(struct soap *_soap, STRUCT_MESSAGE *_message, LIST_PARAM_VALUE *_list_param_value)
{
short flag_Name = 1, flag_Id = 1, flag_Function = 1, flag_Parameter = 1, flag_Value = 1, flag_Type = 1, flag_Port = 1, flag_Active = 1;
char **s;
LIST_PARAM_VALUE *elt_param_value = _list_param_value;

  dprintf(LOG_LEVEL2, "ExtractXMLTagsWithMultipleParams\n"); 
  memset(_message, 0x00, sizeof(STRUCT_MESSAGE));
  _message->Id = -1;

  if (soap_element_begin_in(_soap, CONST_MESSAGE, 0))
    return SOAP_ERR;

  _message->Ip = _soap->ip;

  for (;;)
  { _soap->error = SOAP_TAG_MISMATCH;
    if (flag_Name && _soap->error == SOAP_TAG_MISMATCH)
      if ((s = soap_in_xsd__string(_soap, "NAME", NULL, NULL)) != NULL)
      { flag_Name--;
        sprintf(_message->szName, "%s", *s);
        continue;
      }
    if (flag_Id && _soap->error == SOAP_TAG_MISMATCH)
      if (soap_in_int(_soap, "Id", &_message->Id, NULL))
      { flag_Id--;
        continue;
      }
    if (flag_Function && _soap->error == SOAP_TAG_MISMATCH)
      if ((s = soap_in_xsd__string(_soap, "Function", NULL, NULL)) != NULL)
      { flag_Function--;
        sprintf(_message->szFunction, "%s", *s);
        continue;
      }
    if (flag_Parameter && _soap->error == SOAP_TAG_MISMATCH)
      if ((s = soap_in_xsd__string(_soap, "Parameter", NULL, NULL)) != NULL)
      {
        // add new element of (param, value) list if (param, value) already filled
        if ((strcmp(elt_param_value->szParameter, "") != 0) && (strcmp(elt_param_value->szValue, "") != 0))
        {
          elt_param_value->next = (LIST_PARAM_VALUE*)malloc(sizeof(LIST_PARAM_VALUE));
          elt_param_value = elt_param_value->next;
          memset(elt_param_value, 0x00, sizeof(LIST_PARAM_VALUE));
        }

        sprintf(elt_param_value->szParameter, "%s", *s);
		dprintf(4, "------------------------->\n");
		dprintf(4, "----->%s\n",elt_param_value->szParameter);
		dprintf(4, "------------------------->\n");
        //flag_Parameter--;
        //sprintf(_message->szParameter, "%s", *s);
        continue;
      }
    if (flag_Value && _soap->error == SOAP_TAG_MISMATCH)
      if ((s = soap_in_xsd__string(_soap, "Value", NULL, NULL)) != NULL)
      {
        // add new element of (param, value) list if (param, value) already filled
        if ((strcmp(elt_param_value->szParameter, "") != 0) && (strcmp(elt_param_value->szValue, "") != 0))
        {
          elt_param_value->next = (LIST_PARAM_VALUE*)malloc(sizeof(LIST_PARAM_VALUE));
          elt_param_value = elt_param_value->next;
          memset(elt_param_value, 0x00, sizeof(LIST_PARAM_VALUE));
        }

        sprintf(elt_param_value->szValue, "%s", *s);
        //flag_Value--;
        //sprintf(_message->szValue, "%s", *s);
        continue;
      }
    if (flag_Type && _soap->error == SOAP_TAG_MISMATCH)
      if ((s = soap_in_xsd__string(_soap, "Type", NULL, NULL)) != NULL)
      { flag_Type--;
        sprintf(_message->szType, "%s", *s);
        continue;
      }
    if (flag_Port && _soap->error == SOAP_TAG_MISMATCH)
      if (soap_in_int(_soap, "Port", &_message->Port, NULL))
      { flag_Port--;
        continue;
      }

    if (flag_Active && _soap->error == SOAP_TAG_MISMATCH)
      if ((s = soap_in_xsd__string(_soap, "Active", NULL, NULL)) != NULL)
      { flag_Active--;
        sprintf(_message->szActive, "%s", *s);
        continue;
      }

    if (_soap->error == SOAP_TAG_MISMATCH)
      _soap->error = soap_ignore_element(_soap);
    if (_soap->error == SOAP_NO_TAG)
      break;
    if (_soap->error) return _soap->error;
  }

  dprintf(LOG_LEVEL3, "ExtractXMLTagsWithMultipleParams before print param-value list\n");
  print(_list_param_value);
  return soap_element_end_in(_soap, CONST_MESSAGE);
}


/***************************************************************************/
/*** process received message                                            ***/
/*** parse string values                                                 ***/
/***************************************************************************/ 
int ProcessMessage(STRUCT_MESSAGE *_message, enum methodName *_methodName, enum hostType *_type, enum functionName *_fction)
{
int nRet = -1;

  dprintf(LOG_LEVEL2, "ProcessMessage, message received\n");
  dprintf(LOG_LEVEL2, "Name method : %s\n", _message->szName);
  if ( strcmp(_message->szName, CONST_SUBSCRIBE) == 0)
    *_methodName = SUBSCRIBE;
  else if ( strcmp(_message->szName, CONST_REQUEST) == 0)
    *_methodName = REQUEST;
  else if ( strcmp(_message->szName, CONST_NOTIFY) == 0)
    *_methodName = NOTIFY;
  else if ( strcmp(_message->szName, CONST_RESPONSE) == 0)
    *_methodName = RESPONSE;
  else if ( strcmp(_message->szName, CONST_ERROR) == 0)
    *_methodName = ERROR;
  else if ( strcmp(_message->szName, CONST_DOWNLOADCOMPLETE) == 0)
    *_methodName = DOWNLOADCOMPLETE;
  else if ( strcmp(_message->szName, CONST_UPLOADCOMPLETE) == 0)
    *_methodName = UPLOADCOMPLETE;
  else *_methodName = UNKNOWN;

  dprintf(LOG_LEVEL3, "Id : %d\n", _message->Id);

  dprintf(LOG_LEVEL3, "Type : %s\n", _message->szType);
  if ( strcmp(_message->szType, CONST_TYPELOCAL) == 0)
    *_type = TYPE_LOCAL;
  else if ( strcmp(_message->szType, CONST_TYPECLIENT) == 0)
    *_type = TYPE_CLIENT;
  else if ( strcmp(_message->szType, CONST_TYPECMPMGR) == 0)
    *_type = TYPE_CMP_MGR;
  else *_type = TYPE_UNKNOWN;

  dprintf(LOG_LEVEL3, "Function : %s\n", _message->szFunction);
  if ( strcmp(_message->szFunction, CONST_FUNCTIONGETVALUE) == 0)
    *_fction = GET_VALUE;
  else if ( strcmp(_message->szFunction, CONST_FUNCTIONSETVALUE) == 0)
    *_fction = SET_VALUE;
  else if ( strcmp(_message->szFunction, CONST_FUNCTIONDELETEOBJECT) == 0) //add  by wangjr 091124
    *_fction = DELETEOBJECT;
  else if ( strcmp(_message->szFunction, CONST_FUNCTIONADDOBJECT) == 0)
    *_fction = ADDOBJECT;
  else if ( strcmp(_message->szFunction, CONST_FUNCTIONGETPARAMETERATTRIBUTES) == 0)
    *_fction = GET_PARAMETERATTRIBUTES;
  else if ( strcmp(_message->szFunction, CONST_FUNCTIONSETPARAMETERATTRIBUTES) == 0)
    *_fction = SET_PARAMETERATTRIBUTES;
  else if ( strcmp(_message->szFunction, CONST_FUNCTIONGETNAME) == 0)
    *_fction = GET_NAME;
  else if ( strcmp(_message->szFunction, CONST_FUNCTIONGETATTRIBUTES) == 0)
    *_fction = GET_ATTRIBUTES;
  else if ( strcmp(_message->szFunction, CONST_FUNCTIONSETATTRIBUTES) == 0)
    *_fction = SET_ATTRIBUTES;
  else if ( strcmp(_message->szFunction, CONST_FUNCTIONGETPASSIVEPARAMETERS) == 0)
    *_fction = GET_PASSIVEPARAMETERS;
  else if ( strcmp(_message->szFunction, CONST_FUNCTIONGETACTIVEPARAMETERS) == 0) //add by wangjr 091124
    *_fction = GET_ACTIVEPARAMETERS;
  else if ( strcmp(_message->szFunction, CONST_FUNCTIONGETMONITORPARAMETERS) == 0)
  	*_fction = GET_MONITORPARAMETERS;
  else if ( strcmp(_message->szFunction, CONST_FUNCTIONGETALARMPARAMETERS) == 0)
  	*_fction = GET_ALARMPARAMETERS;
  else if ( strcmp(_message->szFunction, CONST_FUNCTIONGETIPCHANGEPARAMETERS) == 0)
    *_fction = GET_IPPARAMETERS;
  else *_fction = FCT_UNKNOWN;

  if (*_methodName == SUBSCRIBE)
    dprintf(LOG_LEVEL3, "@subscriber: %ld.%ld.%ld.%ld:%d\n", (_message->Ip >> 24)&0xFF, (_message->Ip >> 16)&0xFF, (_message->Ip >> 8)&0xFF, _message->Ip&0xFF, _message->Port);

  dprintf(LOG_LEVEL3, "Active : %s\n\n", _message->szActive);

  if (*_methodName != UNKNOWN)
    nRet = 0;
  return nRet;
}


/***************************************************************************/
/*** getAccessCondition function                                         ***/
/*** return access condition                                             ***/
/*** (i) access condition in string value                                ***/
/***************************************************************************/
ACCESSCONDITION getAccessCondition(char *_szValue)
{
ACCESSCONDITION eRet = READ;

  if (strcmp(_szValue, "READ") == 0)
  {
    eRet = READ;
  }
  else if (strcmp(_szValue, "WRITE") == 0)
  {
    eRet = WRITE;
  }
  else if (strcmp(_szValue, "READWRITE") == 0)
  {
    eRet = READWRITE;
  }
  return eRet;
}


/***************************************************************************/
/*** getErrorString function                                             ***/
/*** return string error                                                 ***/
/*** (i) error value                                                     ***/
/***************************************************************************/
char *getErrorString(TR69_ERROR _error)
{
  memset(szTR_69_ERROR, 0x00, sizeof(szTR_69_ERROR));
  switch(_error)
  {
    case OK:
      sprintf(szTR_69_ERROR, "OK");
      break;
    case SYNTAX_ERROR:
      sprintf(szTR_69_ERROR, "SYNTAX ERROR ON XML MESSAGE");
      break;
    case FUNCTION_ERROR:
      sprintf(szTR_69_ERROR, "FUNCTION NOT RECOGNIZED IN XML MESSAGE");
      break;
    case PARAMETER_ERROR:
      sprintf(szTR_69_ERROR, "PARAMETER ERROR OR MISSING IN XML MESSAGE");
      break;
    case VARIABLE_ERROR:
      sprintf(szTR_69_ERROR, "VARIABLE ERROR OR UNKNOWN");
      break;
    case DOM_ERROR:
      sprintf(szTR_69_ERROR, "ERROR IN DOM TREE");
      break;
    case ACCESS_ERROR:
      sprintf(szTR_69_ERROR, "ACCESS CONDITION (READ OR WRITE) ERROR");
      break;
    case ID_ERROR:
      sprintf(szTR_69_ERROR, "ID ERROR");
      break;
  }
  return szTR_69_ERROR;
}
