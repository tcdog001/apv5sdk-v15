/**********************************************************************************
* Copyright (c) 2008-2011  Beijing Autelan Technology Co. Ltd.
* All rights reserved.
*
* filename: ManagerDefinition.h
* description:  the headef file for the Manager type declaration and structure
* 
*
* 
************************************************************************************/
#ifndef _MANAGERDEFINITION_H
#define _MANAGERDEFINITION_H

/* type declaration */
typedef enum {INT, UINT, BOOLEAN, STRING} TYPE;
typedef enum {ACTIVE, PASSIVE, NONE} NOTIFICATION;
typedef enum {READ, WRITE, READWRITE} ACCESSCONDITION;

typedef struct structManagerInternalValues { 
  int (*SetValue)(char *,char *);          // function pointer (Set) - parameter in string form
  char* (*GetValue)(char *);              // function pointer (Get) - return parameter as string form
  TYPE type;                        // internal type (int, boolean, char *)
  char *szDataModelSyntax;          // ex. Device.DeviceInfo
  NOTIFICATION notificationType;    // notification : Active, Passive or none
  ACCESSCONDITION accessConditions; // Read Write
  int bValueChanged;                // Has value changed ?
  int isExist;         //add by wangjr 091124 for addobject and deleteobject
} STRUCT_MANAGER_INTERNALVALUES;

#endif // _MANAGERDEFINITION_H
