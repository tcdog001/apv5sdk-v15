/**********************************************************************************
* Copyright (c) 2008-2011  Beijing Autelan Technology Co. Ltd.
* All rights reserved.
*
* filename: readRadius.h
* description:  implementation for the header file of readRadius.c.
* 
*
* 
************************************************************************************/

//output:
//	int GetRadiusPara( char *strPara ); 
//strPara in:
/*  authPrimaryServerIP,
    authPrimaryServerPort,
    authPrimaryServerSecret,
    authSecondaryServerIP,
    authSecondaryServerPort,
    authSecondaryServerSecret,
    accountPrimaryServerIP,
    accountPrimaryServerPort,
    accountPrimaryServerSecret,
    accountSecondaryServerIP,
    accountSecondaryServerPort,
    accountSecondaryServerSecret,
    wpaPassPhrase
    int wpaReauthTime;
    int wpaGroupKeyUpdateCondition;
    int wpaGroupKeyUpdateIntervalSecond;
    int wpaGroupKeyUpdateIntervalPacket;    
*/
//strPara out: value    

#ifndef READRADIUS_H
#define READRADIUS_H

#define RADIUS_CONF_FILE	"/etc/WAPI/11i/eap_11i_0.conf"


int GetRadiusPara( char *strPara );

int SaveRadiusPara( char *strPara, char *strVal );



#endif

