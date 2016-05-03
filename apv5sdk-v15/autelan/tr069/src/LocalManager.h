
/**********************************************************************************
* Copyright (c) 2008-2011  Beijing Autelan Technology Co. Ltd.
* All rights reserved.
*
* filename: LocalManager.h
* description:  the header file for the declaration of Manager function
* 
*
* 
************************************************************************************/

#ifndef _LOCALMANAGER_H
#define _LOCALMANAGER_H

#include "StubH.h"

#define DEVICEINFO_MANAGEMENTSERVER_PERIODICINFORMTIME      "InternetGatewayDevice.ManagementServer.PeriodicInformTime"
#define DEVICEINFO_MANAGEMENTSERVER_PERIODICINFORMENABLE    "InternetGatewayDevice.ManagementServer.PeriodicInformEnable"
#define DEVICEINFO_MANAGEMENTSERVER_PERIODICINFORMINTERVAL  "InternetGatewayDevice.ManagementServer.PeriodicInformInterval"

/* function declaration */

// thread functions
void *manager_server_thread(void *soap);
void *reading_thread(void *soap);
void *response_thread(void *_threadArg);

int SubscribeSequence(struct soap *_soap, char *_soap_endpoint, int _nConfPort);

#endif // _LOCALMANAGER_H
