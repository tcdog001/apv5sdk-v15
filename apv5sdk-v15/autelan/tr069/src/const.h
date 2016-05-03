
/**********************************************************************************
* Copyright (c) 2008-2011  Beijing Autelan Technology Co. Ltd.
* All rights reserved.
*
* filename: const.h
* description:  the header file  for the constant definition
* 
*
* 
************************************************************************************/

#ifndef _CONST_H
#define _CONST_H

#define ARGSIZE 256
#define BACKLOG (100) // Max. request backlog
#define MAX_PARAMETER 256
#define MAX_VALUE     128
#define MAX_BUFFER    256

#define SERVER_ADDRESS                     "Address"
#define SERVER_PORT                        "Port"
#define CONNECTIONTIME_OUT                 "ConnectionTimeout"
#define CONST_AUTH_USERNAME                "UserName"
#define CONST_AUTH_PASSWORD                "Password"
#define CONST_AUTH_REALM                   "Realeam"
#define CONST_WWW_AUTH_USERNAME            "WWW-UserName"
#define CONST_WWW_AUTH_PASSWORD            "WWW-Password"
#define CONST_WWW_AUTH_REALM               "WWW-Realeam"

#define CONST_MESSAGE                      "MESSAGE"
#define CONST_REQUEST                      "REQUEST"
#define CONST_NOTIFY                       "NOTIFY"
#define CONST_SUBSCRIBE                    "SUBSCRIBE"
#define CONST_RESPONSE                     "RESPONSE"
#define CONST_ERROR                        "ERROR"
#define CONST_DOWNLOADCOMPLETE             "DOWNLOADCOMPLETE"
#define CONST_UPLOADCOMPLETE               "UPLOADCOMPLETE"


#define CONST_FUNCTIONSETVALUE             "setValue"
#define CONST_FUNCTIONGETVALUE             "getValue"
#define CONST_FUNCTIONGETNAME              "getName"
#define CONST_FUNCTIONSETATTRIBUTES        "setAttributes"
#define CONST_FUNCTIONGETATTRIBUTES        "getAttributes"
#define CONST_FUNCTIONGETPASSIVEPARAMETERS "getPassiveParameters"
#define CONST_FUNCTIONGETACTIVEPARAMETERS  "getActiveParameters"  //add by wangjr 091124
#define CONST_FUNCTIONGETMONITORPARAMETERS "getMoniterParameters"
#define CONST_FUNCTIONGETALARMPARAMETERS   "getAlarmParameters"
#define CONST_FUNCTIONGETIPCHANGEPARAMETERS "getIpchangeParameters"
#define  CONST_FUNCTIONDELETEOBJECT        "deleteobject" //add by wangjr
#define  CONST_FUNCTIONADDOBJECT           "addobject"  
#define  CONST_FUNCTIONGETPARAMETERATTRIBUTES "GetParameterAttributes" 
#define  CONST_FUNCTIONSETPARAMETERATTRIBUTES  "SetParameterAttributes"

#define CONST_TYPELOCAL                    "TYPE_LOCAL"
#define CONST_TYPECLIENT                   "TYPE_CLIENT"
#define CONST_TYPECMPMGR                   "TYPE_CMP_MGR"

#define SZ_ACCESSCONDITION                 "READ"
#define SZ_NOTIFICATION                    "NOTIFICATION"
#define SZ_ACTIVE                          "ACTIVE"
#define SZ_PASSIVE                         "PASSIVE"
#define SZ_NONE                            "NONE"
#define SZ_READ                            "READ"
#define SZ_WRITE                           "WRITE"
#define SZ_READWRITE                       "READWRITE"
#define SZ_INSTANCE                        "INSTANCE"
#define SZ_NUMBEROFENTRIES                 "NumberOfEntries"
#define SZ_ISEXIST                         "ISEXIST"  //add by wngjr


// DEBUG TRACE
#ifdef DEBUG

#define LOG_NOLOG  0
#define LOG_LEVEL1 1 // trace with main functions + errors
#define LOG_LEVEL2 2 // level1 + secondary functions
#define LOG_LEVEL3 3 // level2 + values
#define LOG_LEVEL4 4 // all traces

#ifndef DEBUG_LEVEL
#define DEBUG_LEVEL LOG_LEVEL4
#endif

#define dprintf(debug_level, fmt...) \
    do { \
      if (debug_level <= DEBUG_LEVEL) fprintf(stderr, fmt); \
    } while(0)
#else
#define dprintf(debug_level, fmt...) \
    do { } while(0)

#endif


/* LIST PARAMETER-VALUE DEFINITION */
typedef struct list_param_value {
  struct list_param_value *next;
  char szParameter[MAX_PARAMETER];  // parameter
  char szValue[MAX_VALUE];          // value (string)
} LIST_PARAM_VALUE;


// message definition
typedef struct structMessage { 
  char szName[10];                  // name of method
  int  Id;                          // Id of sender/receiver
  char szFunction[22];              // function
  LIST_PARAM_VALUE *ListParamValue; // list of (param, value)
  char szType[14];                  // type (TYPE_LOCAL, TYPE_CLIENT, TYPE_CMP_MGR)
  unsigned long Ip;                 // IP address
  int  Port;                        // Port value
  char szActive[8];                 // Notification (Active, Passive, None)
} STRUCT_MESSAGE;


/* type declaration */
typedef enum {INT, UINT, BOOLEAN, STRING} TYPE;
typedef enum {ACTIVE, PASSIVE, NONE} NOTIFICATION;
typedef enum {READ, WRITE, READWRITE} ACCESSCONDITION;
typedef enum {OK=0, SYNTAX_ERROR, FUNCTION_ERROR, PARAMETER_ERROR, VARIABLE_ERROR, DOM_ERROR, ACCESS_ERROR, ID_ERROR} TR69_ERROR;

/* type declaration */
enum methodName {UNKNOWN=-1, REQUEST, NOTIFY, SUBSCRIBE, RESPONSE, ERROR, DOWNLOADCOMPLETE,UPLOADCOMPLETE};
enum functionName {FCT_UNKNOWN, GET_VALUE, SET_VALUE, GET_NAME, GET_ATTRIBUTES, SET_ATTRIBUTES, GET_PASSIVEPARAMETERS,GET_ACTIVEPARAMETERS, GET_MONITORPARAMETERS, 
GET_ALARMPARAMETERS,DELETEOBJECT,ADDOBJECT,GET_PARAMETERATTRIBUTES,SET_PARAMETERATTRIBUTES,GET_IPPARAMETERS}; //update by wangjr 091124
enum hostType { TYPE_UNKNOWN, TYPE_LOCAL, TYPE_CLIENT, TYPE_CMP_MGR};

#endif //_CONST_H
