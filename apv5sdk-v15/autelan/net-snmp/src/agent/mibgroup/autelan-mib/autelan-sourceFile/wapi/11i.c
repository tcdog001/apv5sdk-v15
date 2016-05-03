/**********************************************************************************
* Copyright (c) 2008-2011  Beijing Autelan Technology Co. Ltd.
* All rights reserved.
*
* filename: 11i.c
* description:  implementation for Radius server configuration and wireless authentication and association.
* 
*
* 
************************************************************************************/


#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>
#include <net-snmp/agent/net-snmp-agent-includes.h>

#include <unistd.h>
#include <dirent.h>
#include <stdio.h>
#include <time.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <signal.h>
#include "wireless.h"
#include <string.h>
#include <dirent.h>
#include <stdlib.h>
#include <malloc.h>
#include <signal.h>
#include <sys/ioctl.h>
#include <arpa/inet.h>
#include <errno.h>

#include  <sys/param.h>
#include  <sys/stat.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#include "wapiPwlanAp.h"    //int SaveLogger( char *strPara );
#include "readKey.h"    //int GetWepPara( int iIndex, char *strPara ); 
#include "readRadius.h"
#include "11i.h"
#include "wirelessStatus.h"

//int Interface_Scan_Get_Count(void); int Interface_Index_By_Name(char *, int);
#include "../../../mibII/interfaces.h" 

#include "p80211def.h"
#include "wapiiofunc.h"
#include "wapicgifunc.h"
#include "wapiconfig.h"
#include "iwlib.h"

#include  "ieee80211.h"
 #include "ieee80211_ioctl.h"
#include  "ieee80211_crypto.h"
#include "wapidebug.h"

extern char g_strMsg[265];


oid    dot11i_variables_oid[] = { 1,3,6,1,4,1,4000,100};

struct variable4 dot11i_variables[] = {


{magic_radiusAuthPrimaryServerIP, ASN_IPADDRESS,  RWRITE, var_11i, 4, { 21, 1, 1, 1 }}, 
{magic_radiusAuthPrimaryServerPort, ASN_INTEGER,  RWRITE, var_11i, 4, { 21, 1, 1, 2 }},
{magic_radiusAuthPrimaryServerSecret, ASN_OCTET_STR, RWRITE,  var_11i, 4, { 21, 1, 1, 3 }},

{magic_radiusAuthSecondaryServerIP, ASN_IPADDRESS,  RWRITE, var_11i, 4, { 21, 1, 2, 1 }}, 
{magic_radiusAuthSecondaryServerPort, ASN_INTEGER,  RWRITE, var_11i, 4, { 21, 1, 2, 2 }},
{magic_radiusAuthSecondaryServerSecret, ASN_OCTET_STR,  RWRITE, var_11i, 4, { 21, 1, 2, 3 }},

{magic_radiusAccountPrimaryServerIP, ASN_IPADDRESS,  RWRITE, var_11i, 4, { 21, 1, 3, 1 }},
{magic_radiusAccountPrimaryServerPort, ASN_INTEGER,  RWRITE, var_11i, 4, { 21, 1, 3, 2 }},
{magic_radiusAccountPrimaryServerSecret, ASN_OCTET_STR,  RWRITE, var_11i, 4, { 21, 1, 3, 3 }},

{magic_radiusAccountSecondaryServerIP, ASN_IPADDRESS,  RWRITE, var_11i, 4, { 21, 1, 4, 1 }},
{magic_radiusAccountSecondaryServerPort, ASN_INTEGER,  RWRITE, var_11i, 4, { 21, 1, 4, 2 }},
{magic_radiusAccountSecondaryServerSecret, ASN_OCTET_STR,  RWRITE, var_11i, 4, { 21, 1, 4, 3 }},

{magic_wepKeyType, ASN_INTEGER,  RWRITE, var_wepTable, 4,  { 21, 2, 1, 1 }},
{magic_key1, ASN_OCTET_STR,  RWRITE, var_wepTable, 4,  { 21, 2, 1, 2 }},
{magic_key2, ASN_OCTET_STR,  RWRITE, var_wepTable, 4,  { 21, 2, 1, 3 }},
{magic_key3, ASN_OCTET_STR,  RWRITE, var_wepTable, 4,  { 21, 2, 1, 4 }},
{magic_key4, ASN_OCTET_STR,  RWRITE, var_wepTable, 4,  { 21, 2, 1, 5 }},
{magic_wepPassPhrase, ASN_OCTET_STR,  RWRITE, var_wepTable, 4,  { 21, 2, 1, 6 }},
{magic_wpaReauthTime, ASN_INTEGER,  RWRITE, var_wepTable, 4,  { 21, 2, 1, 7 }},
{magic_wpaGroupKeyUpdateCondition, ASN_INTEGER,  RWRITE, var_wepTable, 4,  { 21, 2, 1, 8 }},
{magic_wpaGroupKeyUpdateIntervalSecond, ASN_INTEGER,  RWRITE, var_wepTable, 4,  { 21, 2, 1, 9 }},
{magic_wpaGroupKeyUpdateIntervalPacket, ASN_INTEGER,  RWRITE, var_wepTable, 4,  { 21, 2, 1, 10 }},


{magic_wirelessReqAuthCount, ASN_INTEGER, RONLY, var_wirelessStatsTable, 4,  { 21, 3, 1, 1 }},
{magic_wirelessSucceedAuthCount, ASN_INTEGER, RONLY, var_wirelessStatsTable, 4,  { 21, 3, 1, 2 }},
{magic_wirelessAssocTrySum, ASN_INTEGER, RONLY, var_wirelessStatsTable, 4,  { 21, 3, 1, 3 }},
{magic_wirelessAssocSucceedSum, ASN_INTEGER, RONLY, var_wirelessStatsTable, 4,  { 21, 3, 1, 4 }},
{magic_wirelessAssocFailInval, ASN_INTEGER, RONLY, var_wirelessStatsTable, 4,  { 21, 3, 1, 5 }},
{magic_wirelessAssocFailTimeout, ASN_INTEGER, RONLY, var_wirelessStatsTable, 4,  { 21, 3, 1, 6 }},
{magic_wirelessAssocFailReject, ASN_INTEGER, RONLY, var_wirelessStatsTable, 4,  { 21, 3, 1, 7 }},
{magic_wirelessReassocTrySum, ASN_INTEGER, RONLY, var_wirelessStatsTable,  4,  { 21, 3, 1, 8 }},
{magic_wirelessReassocFailInval, ASN_INTEGER, RONLY, var_wirelessStatsTable,  4,  { 21, 3, 1, 9 }},
{magic_wirelessReassocFailTimeout, ASN_INTEGER, RONLY, var_wirelessStatsTable,  4,  { 21, 3, 1, 10 }},
{magic_wirelessReassocFailReject, ASN_INTEGER, RONLY, var_wirelessStatsTable,  4,  { 21, 3, 1, 11 }},
{magic_wirelessReassocFailOther, ASN_INTEGER, RONLY, var_wirelessStatsTable,  4,  { 21, 3, 1, 12 }},
{magic_wirelessAuthTrySum, ASN_INTEGER, RONLY, var_wirelessStatsTable,  4,  { 21, 3, 1, 13 }},
{magic_wirelessAuthSucceedSum, ASN_INTEGER, RONLY, var_wirelessStatsTable,  4,  { 21, 3, 1, 14 }},
{magic_wirelessAuthFailPassword, ASN_INTEGER, RONLY, var_wirelessStatsTable,  4,  { 21, 3, 1, 15 }},
{magic_wirelessAuthFailInval, ASN_INTEGER, RONLY, var_wirelessStatsTable,  4,  { 21, 3, 1, 16 }},
{magic_wirelessAuthFailTimeout, ASN_INTEGER, RONLY, var_wirelessStatsTable,  4,  { 21, 3, 1, 17 }},
{magic_wirelessAuthFailReject, ASN_INTEGER, RONLY, var_wirelessStatsTable,  4,  { 21, 3, 1, 18 }},
{magic_wirelessAuthFailOther, ASN_INTEGER, RONLY, var_wirelessStatsTable,  4,  { 21, 3, 1, 19 }},
{magic_wirelessDisassocSum, ASN_INTEGER, RONLY, var_wirelessStatsTable,  4,  { 21, 3, 1, 20 }},
{magic_wirelessDisassocUSERLeaving, ASN_INTEGER, RONLY, var_wirelessStatsTable,  4,  { 21, 3, 1, 21 }},
{magic_wirelessDisassocAPNORec, ASN_INTEGER, RONLY, var_wirelessStatsTable,  4,  { 21, 3, 1, 22 }},
{magic_wirelessDisassocExceptional, ASN_INTEGER, RONLY, var_wirelessStatsTable,  4,  { 21, 3, 1, 23 }},
{magic_wirelessDisassocOther, ASN_INTEGER, RONLY, var_wirelessStatsTable,  4,  { 21, 3, 1, 24 }},

};




long GetWirelessParaVal( int iIndex, char *strIfName, char *strParaName )
{
    char strPara[256];    
    unsigned long ulRet;
    char strDev[100];

    strcpy( strPara, strParaName );
    if( strcmp(strPara, "") == 0 )
    {
	return 0;
    }

    strcpy( strDev, strIfName+(iIndex-1)*IFNAME_LEN );
    return GetWirelessStatus( strDev, strPara );;
}

unsigned char* var_wirelessStatsTable(struct variable *vp,
    	    oid     *name,
    	    size_t  *length,
    	    int     exact,
    	    size_t  *var_len,
    	    WriteMethod **write_method)
{
    static char strPara[256];    
    static unsigned long ulRet;
    int tableSize=4;  
    char strIfName[IFNAME_MAX];
    int iIndex;
    char strDev[100];
    int i;
 
    memset( strIfName, 0, IFNAME_MAX );
    tableSize = iw_enum_wlan_interface( strIfName ); 

    if( header_simple_table(vp,name,length,exact,var_len,write_method,tableSize) == MATCH_FAILED ) 
    {
	return NULL;  
    }

    if( (iIndex=name[*length-1]) > tableSize)
    {
	return NULL;
    }

    switch(vp->magic) {

	case magic_wirelessReqAuthCount:

	    ulRet = GetWirelessParaVal( iIndex, strIfName, "wirelessReqAuthCount" );
	    return (u_char *)&ulRet;
					
	case magic_wirelessSucceedAuthCount:
	    ulRet = GetWirelessParaVal( iIndex, strIfName, "wirelessSucceedAuthCount" );
	    return (u_char *)&ulRet;
	
	case magic_wirelessAssocTrySum:
       	    
	    ulRet = GetWirelessParaVal( iIndex, strIfName, "wirelessAssocTrySum" );
	    return (u_char *)&ulRet;

	case magic_wirelessAssocSucceedSum:
	    
	    ulRet = GetWirelessParaVal( iIndex, strIfName, "wirelessAssocSucceedSum" );
	    return (u_char *)&ulRet;
					
	case magic_wirelessAssocFailInval:
 	    
	    ulRet = GetWirelessParaVal( iIndex, strIfName, "" );
	    return (u_char *)&ulRet;
	
	case magic_wirelessAssocFailTimeout:
 	    
	    ulRet = GetWirelessParaVal( iIndex, strIfName, "" );
	    return (u_char *)&ulRet;

	case magic_wirelessAssocFailReject:
 	    
	    ulRet = GetWirelessParaVal( iIndex, strIfName, "" );
	    return (u_char *)&ulRet;

	case magic_wirelessReassocTrySum:
 	    
	    ulRet = GetWirelessParaVal( iIndex, strIfName, "" );
	    return (u_char *)&ulRet;

	case magic_wirelessReassocFailInval:
 	    
	    ulRet = GetWirelessParaVal( iIndex, strIfName, "" );
	    return (u_char *)&ulRet;

	case magic_wirelessReassocFailTimeout:
 	    
	    ulRet = GetWirelessParaVal( iIndex, strIfName, "" );
	    return (u_char *)&ulRet;

	case magic_wirelessReassocFailReject:
 	    
	    ulRet = GetWirelessParaVal( iIndex, strIfName, "" );
	    return (u_char *)&ulRet;

	case magic_wirelessReassocFailOther:
 	    
	    ulRet = GetWirelessParaVal( iIndex, strIfName, "" );
	    return (u_char *)&ulRet;

	case magic_wirelessAuthTrySum:
 	    
	    ulRet = GetWirelessParaVal( iIndex, strIfName, "" );
	    return (u_char *)&ulRet;

	case magic_wirelessAuthSucceedSum:
 	    
	    ulRet = GetWirelessParaVal( iIndex, strIfName, "" );
	    return (u_char *)&ulRet;

	case magic_wirelessAuthFailPassword:
 	    
	    ulRet = GetWirelessParaVal( iIndex, strIfName, "" );
	    return (u_char *)&ulRet;

	case magic_wirelessAuthFailInval:

 	    ulRet = GetWirelessParaVal( iIndex, strIfName, "" );
	    return (u_char *)&ulRet;

	case magic_wirelessAuthFailTimeout:

  	    ulRet = GetWirelessParaVal( iIndex, strIfName, "" );
	    return (u_char *)&ulRet;

	case magic_wirelessAuthFailReject:

 	    ulRet = GetWirelessParaVal( iIndex, strIfName, "" );
	    return (u_char *)&ulRet;

	case magic_wirelessAuthFailOther:
 	    
 	    ulRet = GetWirelessParaVal( iIndex, strIfName, "" );
	    return (u_char *)&ulRet;

	case magic_wirelessDisassocSum:
 	    
	    ulRet = GetWirelessParaVal( iIndex, strIfName, "" );
	    return (u_char *)&ulRet;

	case magic_wirelessDisassocUSERLeaving:
 	    
	    ulRet = GetWirelessParaVal( iIndex, strIfName, "" );
	    return (u_char *)&ulRet;

	case magic_wirelessDisassocAPNORec:
 	    
	    ulRet = GetWirelessParaVal( iIndex, strIfName, "wirelessDisassocAPNORec" );
	    return (u_char *)&ulRet;

	case magic_wirelessDisassocExceptional:
 	    
	    ulRet = GetWirelessParaVal( iIndex, strIfName, "" );
	    return (u_char *)&ulRet;

	case magic_wirelessDisassocOther:
 	    
	    ulRet = GetWirelessParaVal( iIndex, strIfName, "" );
	    return (u_char *)&ulRet;

        default:
      		ERROR_MSG("");
    }

    return NULL; 
}
						    
unsigned char* var_wepTable(struct variable *vp,
    	    oid     *name,
    	    size_t  *length,
    	    int     exact,
    	    size_t  *var_len,
    	    WriteMethod **write_method)
{
      int iTableSize=0;
      int iLoop;
      char *pPos=NULL;
      static unsigned char strPara[256];
      char strMsg[50];
      static unsigned long ulRet;
      int iIfNum; 
      int iIndex; 
      int tableSize;
      char strIfName[IFNAME_MAX];
      
      //tableSize = Interface_Scan_Get_Count();
      tableSize = ReadKeyConf();

      if( header_simple_table(vp,name,length,exact,var_len,write_method,tableSize) == MATCH_FAILED )
      {
	  return NULL;  
      }

      if( (iIndex=name[*length-1]) > tableSize)
      {
	  return NULL;
      }
    
    switch(vp->magic) {

	case magic_wepKeyType:
	    *write_method = write_wepKeyType;
	    strcpy( strPara, "type" );
	    ulRet =GetWepPara( strPara );
	    return (unsigned char *)&ulRet;
					
	case magic_key1:
		*write_method = write_key1;
		strcpy( strPara, "key1" );
		GetWepPara( strPara );
		*var_len = strlen( strPara );
        	return strPara;
	
	case magic_key2:
		*write_method = write_key2;
		strcpy( strPara, "key2" );
		GetWepPara( strPara );
		*var_len = strlen( strPara );
        	return strPara;
	
	case magic_key3:
		*write_method = write_key3;
		strcpy( strPara, "key3" );
		GetWepPara( strPara );
		*var_len = strlen( strPara );
        	return strPara;
	
	case magic_key4:
		*write_method = write_key4;
		strcpy( strPara, "key4" );
		GetWepPara( strPara );
		*var_len = strlen( strPara );
        	return strPara;

	case magic_wepPassPhrase:
		*write_method = write_wepPassPhrase;
                if( iIndex == 1 ){
		    strcpy( strPara, "" );
		}
		else{		
		    strcpy( strPara, "wepPassPhrase" );                
		    GetRadiusPara( strPara );
		}
                *var_len = strlen( strPara );
                return strPara;		

	case magic_wpaReauthTime:
		*write_method = write_wpaReauthTime;
		ulRet=0;
        	return (u_char*)&ulRet;

	case magic_wpaGroupKeyUpdateCondition:
		*write_method = write_wpaGroupKeyUpdateCondition;
		ulRet=0;
        	return (u_char*)&ulRet;

	case magic_wpaGroupKeyUpdateIntervalSecond:
		*write_method = write_wpaGroupKeyUpdateIntervalSecond;
                if( iIndex == 1 ){
		    strcpy( strPara, "" );
		}
		else{		
    		    strcpy( strPara, "wpaGroupKeyUpdateIntervalSecond" );    
		    GetRadiusPara( strPara );
		}
                ulRet = atoi(strPara);
                return (u_char*)&ulRet; 	

	case magic_wpaGroupKeyUpdateIntervalPacket:
		*write_method = write_wpaGroupKeyUpdateIntervalPacket;
		ulRet=0;
        	return (u_char*)&ulRet;
        
	default:
      		ERROR_MSG("");
    }
    return NULL; 
 
}


void init_11i(void)
{

    prop_data properties[KEYS_MAX];
    int prop_count=0;
    static char ap_parameter[256];
    int iRet;

    REGISTER_MIB("autelan-mib/autelan-sourceFile/wapi/11i", dot11i_variables, variable4,
               dot11i_variables_oid);
    
}




unsigned char* var_11i(struct variable *vp, 
                oid     *name, 
                size_t  *length, 
                int     exact, 
                size_t  *var_len, 
                WriteMethod **write_method)
{
    static char strPara[256];
    static unsigned long ulRet;


    memset( strPara, 0, 256 );
    memset( strPara, 0, 256 );
    
///* 
    if( header_generic(vp,name,length,exact,var_len,write_method) == MATCH_FAILED ) 
    {
    	return NULL;
    }
    
    switch(vp->magic) {

	case magic_radiusAuthPrimaryServerIP:
	    *write_method = write_authPrimaryServerIP;
	    strcpy( strPara, "authPrimaryServerIP" );
	    GetRadiusPara( strPara );
	    ulRet = inet_addr( strPara );    	  
    	        memcpy(strPara, (char *)&ulRet, 4);
    	    	*var_len = 4;
	    	return (unsigned char *) strPara;
		
	case magic_radiusAuthPrimaryServerPort:
		*write_method = write_authPrimaryServerPort;
    		strcpy( strPara, "authPrimaryServerPort" );
    		GetRadiusPara( strPara );
		ulRet = atoi(strPara);
        	return (u_char*)&ulRet;
	
        case magic_radiusAuthPrimaryServerSecret:
       		*write_method = write_authPrimaryServerSecret;
    		strcpy( strPara, "authPrimaryServerSecret" );
    		GetRadiusPara( strPara );
		*var_len = strlen( strPara );
           	return (u_char*)strPara;


	case magic_radiusAuthSecondaryServerIP:
	    *write_method = write_authSecondaryServerIP;
                strcpy( strPara, "authSecondaryServerIP" );
                GetRadiusPara( strPara );
                ulRet = inet_addr( strPara );
    	        memcpy(strPara, (char *)&ulRet, 4);
    	    	*var_len = 4;
	    	return (unsigned char *) strPara;
		
	case magic_radiusAuthSecondaryServerPort:
		*write_method = write_authSecondaryServerPort;
                strcpy( strPara, "authSecondaryServerPort" );
                GetRadiusPara( strPara );
                ulRet = atoi(strPara);
                return (u_char*)&ulRet;		

        case magic_radiusAuthSecondaryServerSecret:
       		*write_method = write_authSecondaryServerSecret;
                strcpy( strPara, "authSecondaryServerSecret" );
                GetRadiusPara( strPara );
                *var_len = strlen( strPara );
                return (u_char*)strPara;	       

	case magic_radiusAccountPrimaryServerIP:
	    *write_method = write_accountPrimaryServerIP;
                strcpy( strPara, "accountPrimaryServerIP" );
                GetRadiusPara( strPara );
                ulRet = inet_addr( strPara );
                memcpy(strPara, (char *)&ulRet, 4);
                *var_len = 4;
                return (unsigned char *) strPara;
	
	case magic_radiusAccountPrimaryServerPort:
		*write_method = write_accountPrimaryServerPort;
                strcpy( strPara, "accountPrimaryServerPort" );
                GetRadiusPara( strPara );
                ulRet = atoi(strPara);
                return (u_char*)&ulRet;

        case magic_radiusAccountPrimaryServerSecret:
       		*write_method = write_accountPrimaryServerSecret;
                strcpy( strPara, "accountPrimaryServerSecret" );
                GetRadiusPara( strPara );
                *var_len = strlen( strPara );
                return (u_char*)strPara;      

	case magic_radiusAccountSecondaryServerIP:
	    *write_method = write_accountSecondaryServerIP;
                strcpy( strPara, "accountSecondaryServerIP" );
                GetRadiusPara( strPara );
                ulRet = inet_addr( strPara );
                memcpy(strPara, (char *)&ulRet, 4);
                *var_len = 4;
                return (unsigned char *) strPara; 

	case magic_radiusAccountSecondaryServerPort:
		*write_method = write_accountSecondaryServerPort;
                strcpy( strPara, "accountSecondaryServerPort" );
                GetRadiusPara( strPara );
                ulRet = atoi(strPara);
                return (u_char*)&ulRet;

        case magic_radiusAccountSecondaryServerSecret:
       		*write_method = write_accountSecondaryServerSecret;
                strcpy( strPara, "accountSecondaryServerSecret" );
                GetRadiusPara( strPara );
                *var_len = strlen( strPara );
                return (u_char*)strPara;      		

	case magic_wepPassPhrase:
		*write_method = write_wepPassPhrase;
                strcpy( strPara, "wepPassPhrase" );
                GetRadiusPara( strPara );
                *var_len = strlen( strPara );
                return (u_char*)strPara;		

	case magic_wpaReauthTime:
		*write_method = write_wpaReauthTime;
        	return (u_char*)0;

	case magic_wpaGroupKeyUpdateCondition:
		*write_method = write_wpaGroupKeyUpdateCondition;
        	return (u_char*)0;

	case magic_wpaGroupKeyUpdateIntervalSecond:
		*write_method = write_wpaGroupKeyUpdateIntervalSecond;
                strcpy( strPara, "wpaGroupKeyUpdateIntervalSecond" );
                GetRadiusPara( strPara );
                ulRet = atoi(strPara);
                return (u_char*)&ulRet; 	

	case magic_wpaGroupKeyUpdateIntervalPacket:
		*write_method = write_wpaGroupKeyUpdateIntervalPacket;
        	return (u_char*)0;

        default:
      		ERROR_MSG("");
    }

//*/
    
    return NULL;
}

int Do11iScript() 
{		
    int pid;
    char scr_file[256];
    char * aargv[2];

    strcpy( scr_file, "/etc/WAPI/11i/restart_hostapd" );
    aargv[0] = scr_file;
    aargv[1] = NULL;
    
		pid =fork();
          	if(pid == 0)
          	{
          	   execv(aargv[0],aargv);
          	   _exit(0);
          	}
          	else if(pid > 0)
		{
             		wait(NULL);
		}
          	else
		{
		}
}

int WriteRadiusIP( int action, u_char *var_val, u_char var_val_type, size_t var_val_len, char *strPara )
{
    char strVal[256];
    
    switch ( action ) {
        case RESERVE1:
          if (var_val_type != ASN_IPADDRESS) {
              fprintf(stderr, "write to 11i not ASN_IPADDRESS\n");
              return SNMP_ERR_WRONGTYPE;
          }

          if (var_val_len != 4){
              fprintf(stderr,"write to wapiPwlanAp: bad length\n");
              return SNMP_ERR_WRONGLENGTH;
          }
          break;

        case RESERVE2:
          //size  = var_val_len;
          //value = * (u_long *) var_val;

          break;

        case FREE:
             /* Release any resources that have been allocated */
          break;

        case ACTION:
             /*
              * The variable has been stored in 'value' for you to use,
              * and you have just been asked to do something with it.
              * Note that anything done here must be reversable in the UNDO case
              */
         
        	sprintf( strVal, "%d.%d.%d.%d", var_val[0], var_val[1], var_val[2], var_val[3] );
		if( var_val[3] == 255 ){
			return SNMP_ERR_WRONGVALUE;
		}
		
		SaveRadiusPara( strPara, strVal );


          break;

        case UNDO:
             /* Back out any changes made in the ACTION case */
          break;

        case COMMIT:
             /*
              * Things are working well, so it's now safe to make the change
              * permanently.  Make sure that anything done here can't fail!
              */
          break;
    }
    return SNMP_ERR_NOERROR;
}

int WriteRadiusPort( int action, u_char *var_val, u_char var_val_type, size_t var_val_len, char *strPara )
{
    char strVal[256];
    long value;

    switch ( action ) {
        case RESERVE1:
          if (var_val_type != ASN_INTEGER) {
              fprintf(stderr, "write to wapiPwlanAp not ASN_INTEGER\n");
              return SNMP_ERR_WRONGTYPE;
          }
          if (var_val_len > sizeof(long)) {
              fprintf(stderr,"write to wapiPwlanAp: bad length\n");
              return SNMP_ERR_WRONGLENGTH;
          }
          value = * (long *) var_val;
          if (value<0) {
              fprintf(stderr,"value out of range: 0...1 \n");
              return SNMP_ERR_WRONGVALUE;
          }
          break;

        case RESERVE2:
          break;

        case FREE:
          break;

        case ACTION:
	  	value = * (long *) var_val;
	  	sprintf( strVal, "%d", value );
		SaveRadiusPara( strPara, strVal );
		Do11iScript();
	  break;

        case UNDO:
          break;

        case COMMIT:
          break;
    }
    return SNMP_ERR_NOERROR;
}

int WriteRadiusSecret( int action, u_char *var_val, u_char var_val_type, size_t var_val_len, char *strPara )
{
    char strVal[256];

    switch ( action ) {
        case RESERVE1:
          if (var_val_type != ASN_OCTET_STR ) {
              fprintf(stderr, "write to wapiPwlanAp not ASN_INTEGER\n");
              return SNMP_ERR_WRONGTYPE;
          }
         break;

        case RESERVE2:
          break;

        case FREE:
          break;

        case ACTION:
	  	memcpy( strVal, var_val, var_val_len );
		strVal[var_val_len] = '\0';
		SaveRadiusPara( strPara, strVal );
		Do11iScript();

          break;

        case UNDO:
          break;

        case COMMIT:
          break;
    }
    return SNMP_ERR_NOERROR;
}

int write_authPrimaryServerIP(int      action,
            u_char   *var_val,
            u_char   var_val_type,
            size_t   var_val_len,
            u_char   *statP,
            oid      *name,
            size_t   name_len)
{ 
    return WriteRadiusIP( action, var_val, var_val_type, var_val_len, "authPrimaryServerIP" );
}


int write_authPrimaryServerPort(int      action,
            u_char   *var_val,
            u_char   var_val_type,
            size_t   var_val_len,
            u_char   *statP,
            oid      *name,
            size_t   name_len)
{
    return WriteRadiusPort( action, var_val, var_val_type, var_val_len, "authPrimaryServerPort" ); 
}

int write_authPrimaryServerSecret(int      action,
            u_char   *var_val,
            u_char   var_val_type,
            size_t   var_val_len,
            u_char   *statP,
            oid      *name,
            size_t   name_len)
{
    return WriteRadiusSecret( action, var_val, var_val_type, var_val_len, "authPrimaryServerSecret" );
}

int write_authSecondaryServerIP(int      action,
            u_char   *var_val,
            u_char   var_val_type,
            size_t   var_val_len,
            u_char   *statP,
            oid      *name,
            size_t   name_len)
{
    return WriteRadiusIP( action, var_val, var_val_type, var_val_len, "authSecondaryServerIP" );
}

int write_authSecondaryServerPort(int      action,
            u_char   *var_val,
            u_char   var_val_type,
            size_t   var_val_len,
            u_char   *statP,
            oid      *name,
            size_t   name_len)
{
   return WriteRadiusPort( action, var_val, var_val_type, var_val_len, "authSecondaryServerPort" ) ;   
}

int write_authSecondaryServerSecret(int      action,
            u_char   *var_val,
            u_char   var_val_type,
            size_t   var_val_len,
            u_char   *statP,
            oid      *name,
            size_t   name_len)
{

    return WriteRadiusSecret( action, var_val, var_val_type, var_val_len, "authSecondaryServerSecret" );
}

int write_accountPrimaryServerIP(int      action,
            u_char   *var_val,
            u_char   var_val_type,
            size_t   var_val_len,
            u_char   *statP,
            oid      *name,
            size_t   name_len)
{
    return WriteRadiusIP( action, var_val, var_val_type, var_val_len, "accountPrimaryServerIP" );
}

int write_accountPrimaryServerPort(int      action,
            u_char   *var_val,
            u_char   var_val_type,
            size_t   var_val_len,
            u_char   *statP,
            oid      *name,
            size_t   name_len)
{
    return WriteRadiusPort( action, var_val, var_val_type, var_val_len, "accountPrimaryServerPort" ) ;
}


int write_accountPrimaryServerSecret(int      action,
            u_char   *var_val,
            u_char   var_val_type,
            size_t   var_val_len,
            u_char   *statP,
            oid      *name,
            size_t   name_len)
{

    return WriteRadiusSecret( action, var_val, var_val_type, var_val_len, "accountPrimaryServerSecret" );
}

int write_accountSecondaryServerIP(int      action,
            u_char   *var_val,
            u_char   var_val_type,
            size_t   var_val_len,
            u_char   *statP,
            oid      *name,
            size_t   name_len)
{
    return WriteRadiusIP( action, var_val, var_val_type, var_val_len, "accountSecondaryServerIP" );
}


int write_accountSecondaryServerPort(int      action,
            u_char   *var_val,
            u_char   var_val_type,
            size_t   var_val_len,
            u_char   *statP,
            oid      *name,
            size_t   name_len)
{
       return WriteRadiusPort( action, var_val, var_val_type, var_val_len, "accountSecondaryServerPort" ) ; 
}


int write_accountSecondaryServerSecret(int      action,
            u_char   *var_val,
            u_char   var_val_type,
            size_t   var_val_len,
            u_char   *statP,
            oid      *name,
            size_t   name_len)
{

    return WriteRadiusSecret( action, var_val, var_val_type, var_val_len, "accountSecondaryServerSecret" );
}

int write_wepKeyType(int      action,
            u_char   *var_val,
            u_char   var_val_type,
            size_t   var_val_len,
            u_char   *statP,
            oid      *name,
            size_t   name_len)
{
    char strVal[256];
    int iIndex = name[name_len-1];

    switch ( action ) {
        case RESERVE1:
          return SNMP_ERR_READONLY;
          if (var_val_type != ASN_INTEGER) {
              fprintf(stderr, "write to wapiPwlanAp not ASN_INTEGER\n");
              return SNMP_ERR_WRONGTYPE;
          }
          if (var_val_len > sizeof(long)) {
              fprintf(stderr,"write to wapiPwlanAp: bad length\n");
              return SNMP_ERR_WRONGLENGTH;
          }
         break;

        case RESERVE2:
          break;

        case FREE:
          break;

        case ACTION:

	  {
	      long value = * (long *) var_val;
	  	sprintf( strVal, "%d", value );
		SaveWepPara( "type", strVal );
		Do11iScript();
	  }
         break;

        case UNDO:
          break;

        case COMMIT:
          break;
    }
    return SNMP_ERR_NOERROR;
}

int WriteWepKey( int action, 
	u_char *var_val, u_char var_val_type, size_t var_val_len, 
	oid *name, size_t name_len, 
	char *strPara )
{
    char strVal[256];
    int iIndex = name[name_len-1];

    switch ( action ) {
        case RESERVE1:
          if (var_val_type != ASN_OCTET_STR) {
              fprintf(stderr, "write to wapiPwlanAp not ASN_OCTET_STR\n");
              return SNMP_ERR_WRONGTYPE;
          }
          if (  var_val_len !=5 && var_val_len !=10 ) {
              fprintf(stderr,"write to 11i: bad length\n");
              return SNMP_ERR_WRONGLENGTH;
          }
  	  break;

        case RESERVE2:
          break;

        case FREE:
          break;

        case ACTION:
	  	memcpy( strVal, var_val, var_val_len );
		strVal[var_val_len] = '\0';
		SaveWepPara( strPara, strVal );
		Do11iScript();	
          break;

        case UNDO:
          break;

        case COMMIT:
          break;
    }
    return SNMP_ERR_NOERROR;
}

int write_key1(int      action,
            u_char   *var_val,
            u_char   var_val_type,
            size_t   var_val_len,
            u_char   *statP,
            oid      *name,
            size_t   name_len)
{
    return WriteWepKey( action, var_val, var_val_type, var_val_len, name, name_len, "key1" );
}


int write_key2(int      action,
            u_char   *var_val,
            u_char   var_val_type,
            size_t   var_val_len,
            u_char   *statP,
            oid      *name,
            size_t   name_len)
{
    return WriteWepKey( action, var_val, var_val_type, var_val_len, name, name_len, "key2" );    
}


int write_key3(int      action,
            u_char   *var_val,
            u_char   var_val_type,
            size_t   var_val_len,
            u_char   *statP,
            oid      *name,
            size_t   name_len)
{
    return WriteWepKey( action, var_val, var_val_type, var_val_len, name, name_len, "key3" );    
}


int write_key4(int      action,
            u_char   *var_val,
            u_char   var_val_type,
            size_t   var_val_len,
            u_char   *statP,
            oid      *name,
            size_t   name_len)
{
    return WriteWepKey( action, var_val, var_val_type, var_val_len, name, name_len, "key4" );
}

int write_wepPassPhrase(int      action,
            u_char   *var_val,
            u_char   var_val_type,
            size_t   var_val_len,
            u_char   *statP,
            oid      *name,
            size_t   name_len)
{
    switch ( action ) {
        case RESERVE1:
          if (var_val_type != ASN_OCTET_STR) {
              fprintf(stderr, "write to wapiPwlanAp not ASN_INTEGER\n");
              return SNMP_ERR_WRONGTYPE;
          }
          if (var_val_len > 255) {
              fprintf(stderr,"write to wapiPwlanAp: bad length\n");
              return SNMP_ERR_WRONGLENGTH;
          }
         break;

        case RESERVE2:
          break;

        case FREE:
          break;

        case ACTION:
	  {
	      char strVal[256];
	      memcpy( strVal, var_val, var_val_len );
	      strVal[var_val_len] = '\0';
	      SaveRadiusPara( "wepPassPhrase", strVal );
	      Do11iScript();

	  }

          break;

        case UNDO:
          break;

        case COMMIT:
          break;
    }
    return SNMP_ERR_NOERROR;
}


int write_wpaReauthTime(int      action,
            u_char   *var_val,
            u_char   var_val_type,
            size_t   var_val_len,
            u_char   *statP,
            oid      *name,
            size_t   name_len)
{
    switch ( action ) {
        case RESERVE1:
          if (var_val_type != ASN_INTEGER) {
              fprintf(stderr, "write to wapiPwlanAp not ASN_INTEGER\n");
              return SNMP_ERR_WRONGTYPE;
          }
          if (var_val_len > sizeof(long)) {
              fprintf(stderr,"write to wapiPwlanAp: bad length\n");
              return SNMP_ERR_WRONGLENGTH;
          }
//          value = * (long *) var_val;
         break;

        case RESERVE2:
          break;

        case FREE:
          break;

        case ACTION:
	  {
	      char strVal[256];
	      sprintf( strVal, "%d", *(long*)var_val );
	      SaveRadiusPara( "wpaReauthTime", strVal );
              Do11iScript();
	  }
          break;

        case UNDO:
          break;

        case COMMIT:
          break;
    }
    return SNMP_ERR_NOERROR;
}


int write_wpaGroupKeyUpdateCondition(int      action,
            u_char   *var_val,
            u_char   var_val_type,
            size_t   var_val_len,
            u_char   *statP,
            oid      *name,
            size_t   name_len)
{
    return SNMP_ERR_NOERROR;
}


int write_wpaGroupKeyUpdateIntervalSecond(int      action,
            u_char   *var_val,
            u_char   var_val_type,
            size_t   var_val_len,
            u_char   *statP,
            oid      *name,
            size_t   name_len)
{
    switch ( action ) {
        case RESERVE1:
          if (var_val_type != ASN_INTEGER) {
              fprintf(stderr, "write to wapiPwlanAp not ASN_INTEGER\n");
              return SNMP_ERR_WRONGTYPE;
          }
          if (var_val_len > sizeof(long)) {
              fprintf(stderr,"write to wapiPwlanAp: bad length\n");
              return SNMP_ERR_WRONGLENGTH;
          }
//          value = * (long *) var_val;
         break;

        case RESERVE2:
          break;

        case FREE:
          break;

        case ACTION:
            {
		char strVal[256];
		sprintf( strVal, "%d", *(long*)var_val );
		SaveRadiusPara( "wpaGroupKeyUpdateIntervalSecond", strVal );
		Do11iScript();
	    }
          break;

        case UNDO:
          break;

        case COMMIT:
          break;
    }
    return SNMP_ERR_NOERROR;
}


int write_wpaGroupKeyUpdateIntervalPacket(int      action,
            u_char   *var_val,
            u_char   var_val_type,
            size_t   var_val_len,
            u_char   *statP,
            oid      *name,
            size_t   name_len)
{
   return SNMP_ERR_NOERROR;
}



//bottom
