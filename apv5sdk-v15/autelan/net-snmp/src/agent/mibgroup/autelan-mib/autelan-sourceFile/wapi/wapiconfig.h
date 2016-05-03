/**********************************************************************************
* Copyright (c) 2008-2011  Beijing Autelan Technology Co. Ltd.
* All rights reserved.
*
* filename: wapiconfig.h
* description:  implementation for WAPI configuration.
* 
*
* 
************************************************************************************/

/*
* 0 is bridge; 
* 1 is AP; 
* 2 is AP router  ; 
* 3 is Router->AP  ; 
* 4 is AS .
*/
#define WAPI_PRODUCT_ID  1   // NM-200v2
#define WAPI_PWLAN_ID   1
/************************************/
#define WAPI_AGENT_AP    1

#define WAPI_PRODUCT_AP  5
/************************************/
#define WAPI_HARDWARE_XSCALE  1


/*	head file	*/
#define WLANDEV_NAME       "wlan0"
#define WIFIDEV_NAME       "wifi0"
#define ETHDEV_NAME        "eth0"
#define BRDEV_NAME         "br0"
#define NAME_MAX_LEN     512

/* PWLAN madwifi  net80211 path .*/

/* trap out format */
#define OUT_TRAP_TABLE   1




