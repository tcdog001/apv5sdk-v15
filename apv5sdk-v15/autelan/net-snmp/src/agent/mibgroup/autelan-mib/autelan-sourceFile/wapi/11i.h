/**********************************************************************************
* Copyright (c) 2008-2011  Beijing Autelan Technology Co. Ltd.
* All rights reserved.
*
* filename: 11i.h
* description:  implementation for the header file of 11i.c.
* 
*
* 
************************************************************************************/



#ifndef DOT11I_H
#define DOT11I_H

#define magic_radiusAuthPrimaryServerIP			1 
#define magic_radiusAuthPrimaryServerPort		2
#define magic_radiusAuthPrimaryServerSecret		3
#define magic_radiusAuthSecondaryServerIP		4
#define magic_radiusAuthSecondaryServerPort		5
#define magic_radiusAuthSecondaryServerSecret		6
#define magic_radiusAccountPrimaryServerIP		7
#define magic_radiusAccountPrimaryServerPort		8
#define magic_radiusAccountPrimaryServerSecret		9
#define magic_radiusAccountSecondaryServerIP		10
#define magic_radiusAccountSecondaryServerPort		11
#define magic_radiusAccountSecondaryServerSecret	12

#define magic_wepKeyType				13
#define magic_key1					14
#define magic_key2					15
#define magic_key3					16
#define magic_key4					17
#define magic_wepPassPhrase				18
#define magic_wpaReauthTime				19
#define magic_wpaGroupKeyUpdateCondition		20
#define magic_wpaGroupKeyUpdateIntervalSecond		21
#define magic_wpaGroupKeyUpdateIntervalPacket		22


#define magic_wirelessReqAuthCount			23
#define magic_wirelessSucceedAuthCount			24
#define magic_wirelessAssocTrySum			29
#define magic_wirelessAssocSucceedSum			30
#define magic_wirelessAssocFailInval			31
#define magic_wirelessAssocFailTimeout			32
#define magic_wirelessAssocFailReject			33
#define magic_wirelessReassocTrySum			34
#define magic_wirelessReassocFailInval			35
#define magic_wirelessReassocFailTimeout		36
#define magic_wirelessReassocFailReject			37
#define magic_wirelessReassocFailOther			38
#define magic_wirelessAuthTrySum			39
#define magic_wirelessAuthSucceedSum			40
#define magic_wirelessAuthFailPassword			41
#define magic_wirelessAuthFailInval			42
#define magic_wirelessAuthFailTimeout			43
#define magic_wirelessAuthFailReject			44
#define magic_wirelessAuthFailOther			45
#define magic_wirelessDisassocSum			46
#define magic_wirelessDisassocUSERLeaving		47
#define magic_wirelessDisassocAPNORec			48
#define magic_wirelessDisassocExceptional		49
#define magic_wirelessDisassocOther			50


#define TESTTABLE	233	

// function declarations 
    void init_11i(void);
    
    FindVarMethod var_testTable;
    FindVarMethod var_11i;
    FindVarMethod var_wirelessStatsTable;
    FindVarMethod var_wepTable;

    WriteMethod write_authPrimaryServerIP;
    WriteMethod write_authPrimaryServerPort;
    WriteMethod write_authPrimaryServerSecret;

    WriteMethod write_authSecondaryServerIP;
    WriteMethod write_authSecondaryServerPort;
    WriteMethod write_authSecondaryServerSecret;

    WriteMethod write_accountPrimaryServerIP;
    WriteMethod write_accountPrimaryServerPort;
    WriteMethod write_accountPrimaryServerSecret;

    WriteMethod write_accountSecondaryServerIP;
    WriteMethod write_accountSecondaryServerPort;
    WriteMethod write_accountSecondaryServerSecret;

    WriteMethod write_wepKeyType;
    WriteMethod write_key1;
    WriteMethod write_key2;
    WriteMethod write_key3;
    WriteMethod write_key4;
    WriteMethod write_wepPassPhrase;
    WriteMethod write_wpaReauthTime;
    WriteMethod write_wpaGroupKeyUpdateCondition;
    WriteMethod write_wpaGroupKeyUpdateIntervalSecond;
    WriteMethod write_wpaGroupKeyUpdateIntervalPacket;




#endif // DOT11I_H 

