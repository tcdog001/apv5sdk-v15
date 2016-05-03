/**********************************************************************************
* Copyright (c) 2008-2011  Beijing Autelan Technology Co. Ltd.
* All rights reserved.
*
* filename: readKey.h
* description:  implementation for the header file of readKey.c.
* 
*
* 
************************************************************************************/

//output interface:
//	int GetWepKey( int iIndex, char *strPara ); 
//		strPara: IN_OUT	in: para name; out:para value

#ifndef READKEY_H
#define READKEY_H

#define WEPKEY_CONF_FILE	"/etc/WAPI/11i/wep_exec_conf"

int ReadKeyConf(); //ret if have wep ssid

int GetWepPara( char *strPara );

int SaveWepPara( char *strPara, char *strVal );


#endif

