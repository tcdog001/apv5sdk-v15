/**********************************************************************************
* Copyright (c) 2008-2011  Beijing Autelan Technology Co. Ltd.
* All rights reserved.
*
* filename: alarmService.h
* description:  implementation for the header file of alarmService.c.
* 
*
* 
************************************************************************************/

#ifndef ALARMSERVICE_H
#define ALARMSERVICE_H

/*
*---------add by piyongping 2008-10-30---------------------------------
*/
#define WAPIALARMTIMES		1
#define WAPIALARMFLAG		2
#define ALARMID		3
#define ALARMNAME		4
#define ALARMLEVEL		5
#define ALARMTYPE		6
#define ALARMREASONID		7
#define ALARMREASON		8
#define ALARMCREATTIMES		9
#define ALARMSTATE		10
#define ALARMHEADING		11
#define ALARMCONTENT		12

//modify by piyp 2008-11-28
#define WAPISTARTSCANAPENABLED		13
#define WAPIADDNEIGHBORHOODAPMACADDR		14

#define WAPINEIGHBORHOODAPMACID	15
#define WAPINEIGHBORHOODAPMACLIST		16


// function declarations 
    void init_alarmService(void);
    
    FindVarMethod var_alarmService;
		FindVarMethod var_wapiApAlarmAdminTable ;
		FindVarMethod var_wapiScanApTable;
#if 1
//add by piyongping 2008-10-30
		WriteMethod write_wapialarmflag;
		WriteMethod write_wapialarmtimes;
		WriteMethod write_wapiStartScanApEnabled;
		WriteMethod write_wapiAddNeighborhoodApMacAddr;
		WriteMethod write_wapiNeighborhoodApMacList;
#endif

#endif // ALARMSERVICE_H 

