
/**********************************************************************************
* Copyright (c) 2008-2011  Beijing Autelan Technology Co. Ltd.
* All rights reserved.
*
* filename: DataModel.h
* description:  the header file for the declaration of Data model application implementation of Data model repository
* 
*
* 
************************************************************************************/
#ifndef _DATAMODEL_H
#define _DATAMODEL_H

#include "StubH.h"


// Datamodel column + row
// (table to store subscriber(s))

enum DataModelCol {ID, SUBSCRIBER, IP, PORT};

typedef struct structDataModelRow { 
  int Id;
  enum hostType Type;
  unsigned long Ip;
  int Port;
} STRUCT_DATAMODEL_ROW;


/* function declaration */

void *worker_thread(void *soap);

int subscribe_task(struct soap_dom_element *dom, enum methodName eMethodName, enum hostType eHostType, STRUCT_MESSAGE *message);
void *request_task(void *_threadArg);
void *request_getpassiveparameters_task(void *_threadArg);
void *request_getactiveparameters_task(void *_threadArg);  //add by wangjr 091124
void *request_getparameternames_task(void *_threadArg);
void *request_getmonitorparameter_task(void *_threadArg);
void *request_getalarmparameter_task(void *_threadArg);
void *request_deleteobject_task(void *_threadArg);  //add by wangjr 091124
void *request_addobject_task(void *_threadArg);  //add by wangjr 091124

void *request_GetParameterAttributes_task(void *_threadArg);  //add by wangjr 091124
	void *request_SetParameterAttributes_task(void *_threadArg);  //add by wangjr 091124
void *notify_task(void *_threadArg);


// Print subscriber table
//
void print_subscriber(STRUCT_DATAMODEL_ROW *tab, int nbRows);

// Look up a value in subscriber table
//
int LookUpSubscriber(STRUCT_DATAMODEL_ROW *_tab, int nbRows, enum DataModelCol _column, void *_ptrVal);

// Find Id in subscriber table
//
short getSubscriber(STRUCT_DATAMODEL_ROW *_table, int _nbElement, short _Id, char *_address, int *_portNumber);

#endif // _DATAMODEL_H
