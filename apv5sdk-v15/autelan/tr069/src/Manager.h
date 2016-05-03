
/**********************************************************************************
* Copyright (c) 2008-2011  Beijing Autelan Technology Co. Ltd.
* All rights reserved.
*
* filename: Manager.h
* description:  implementation for used by managers (local manager, component manager ..)
* 
*
* 
************************************************************************************/


#ifndef _MANAGER_H
#define _MANAGER_H

#include "const.h"
#include "StubH.h"

typedef struct structManagerInternalValues { 
  int (*SetValue)(char *,char *);          // function pointer (Set) - parameter in string form
  char* (*GetValue)(char *);              // function pointer (Get) - return parameter as string form
  TYPE type;                        // internal type (int, boolean, char *)
  char *szDataModelSyntax;          // ex. Device.DeviceInfo
  NOTIFICATION notificationType;    // notification : Active, Passive or none
  ACCESSCONDITION accessConditions; // Read Write
  int bValueChanged;                // Has value changed ?
  int isExist;         //add by wangjr 091124
} STRUCT_MANAGER_INTERNALVALUES;


char *CreateDataModelPart(struct soap *_soap, char *_szDataModelSyntax, NOTIFICATION notificationType, ACCESSCONDITION accessConditions,int isExist); //update by wangjr 091124
int CreateSubscribeMessage(STRUCT_MANAGER_INTERNALVALUES row, struct soap *_soap);

#endif // _MANAGER_H
