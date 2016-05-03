/**********************************************************************************
* Copyright (c) 2008-2011  Beijing Autelan Technology Co. Ltd.
* All rights reserved.
*
* filename: syslogService.c
* description:  implementation for syslog information of AP. 
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
#include "syslogService.h"

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



oid    syslog_variables_oid[] = { 1,3,6,1,4,1,4000,100};

struct variable4 syslog_variables[] = {

{SYSLOGENABLE,  ASN_INTEGER,  RWRITE,  var_syslogService, 2,  {22, 1 }},
{SYSLOGMSGSEND,  ASN_OCTET_STR,  RWRITE,  var_syslogService, 2,  {22, 2 }},
{SYSLOGMSGLEVEL,  ASN_INTEGER,  RWRITE,  var_syslogService, 2,  {22, 3 }},
{SYSLOGINDEX,  ASN_INTEGER,  RONLY,   var_wapiSyslogSettingTable, 4,  {22, 4, 1, 1 }},
{SYSLOGSERVERPATH,  ASN_OCTET_STR,  RWRITE,  var_wapiSyslogSettingTable, 4,  {22, 4, 1, 2 }},
{SYSLOGLEVEL,  ASN_INTEGER,  RWRITE,  var_wapiSyslogSettingTable, 4,  {22, 4, 1, 3 }},
{SYSLOGFACILITY,  ASN_INTEGER,  RWRITE,  var_wapiSyslogSettingTable, 4,  {22, 4, 1, 4 }},
{SYSLOGCONFINFO,  ASN_OCTET_STR,  RWRITE,  var_wapiSyslogSettingTable, 4,  {22, 4, 1, 5 }},
{SYSLOGUPDATAINTERVALTIME,  ASN_OCTET_STR,  RWRITE,  var_syslogService, 2,  {22, 5 }},
{SYSLOGSAVELOGBAKPATH,  ASN_OCTET_STR,  RONLY,  var_syslogService, 2,  {22, 6 }},
{SYSLOGSERVERPORT,  ASN_INTEGER,  RWRITE,  var_syslogService, 2,  {22, 7 }},


};


void 
init_syslogService(void)
{
	syslogConfpara.strMsg[0] = '\0';
    syslogConfpara.dMsgLevel = 0;
    loadSyslogPara();
    REGISTER_MIB("autelan-mib/autelan-sourceFile/wapi/syslog", syslog_variables, variable4,
               syslog_variables_oid);
}


unsigned char*
var_syslogService(struct variable *vp, 
                oid     *name, 
                size_t  *length, 
                int     exact, 
                size_t  *var_len, 
                WriteMethod **write_method)
{
    
    prop_data properties[KEYS_MAX];
    int prop_count=0;
    static char ap_parameter[256];
	static u_long ulong_ret;
    if( header_generic(vp,name,length,exact,var_len,write_method) == MATCH_FAILED ) 
    {
    	return NULL;
    }
  /* 
   * this is where we do the value assignments for the mib results.
   */
    switch(vp->magic) {

    #if 1
       	case SYSLOGENABLE:
        	*write_method = write_syslogEnable;
			memset(ap_parameter,0,256);
         	prop_count=load_prop(SEP_EQUAL,AG_SNMP_CONF,properties);
		 	if(get_prop("SYSLOG_FLAG",ap_parameter,properties,prop_count)==NULL){
				save_global_conf(SEP_EQUAL,AG_SNMP_CONF,"","SYSLOG_FLAG", "0");
			}
	     	free_prop(properties,prop_count) ;
			ulong_ret = atoi(ap_parameter);
         	return (u_char*) &ulong_ret;
	   	case SYSLOGMSGSEND:
        	*write_method = write_syslogMsgSend;
					*var_len = strlen( syslogConfpara.strMsg );
        	return (u_char*) syslogConfpara.strMsg ;
		case SYSLOGMSGLEVEL:
        	*write_method = write_syslogMsgLevel;
       		return (u_char*) &syslogConfpara.dMsgLevel;
		case SYSLOGUPDATAINTERVALTIME:
        	*write_method = write_syslogUpdataIntervalTime;
		 	memset(ap_parameter,0,265);
         	prop_count=load_prop(SEP_EQUAL,AG_SNMP_CONF,properties);
		 	if(get_prop("UPDATA_SYSLOG_TIME",ap_parameter,properties,prop_count)==NULL){
				save_global_conf(SEP_EQUAL,AG_SNMP_CONF,"","UPDATA_SYSLOG_TIME", "5");
			}
	     	free_prop(properties,prop_count) ;	
	     	*var_len = strlen(ap_parameter);
	     	ap_parameter[*var_len]='\0';
         	return (unsigned char *) ap_parameter ;
		case SYSLOGSAVELOGBAKPATH:
        	//*write_method = write_syslogSaveLogBakPath;
		 	memset(ap_parameter,0,265);
         	prop_count=load_prop(SEP_EQUAL,AG_SNMP_CONF,properties);
		 	if(get_prop("SYSLOG_LOG_FILE_BAK_PATH",ap_parameter,properties,prop_count)==NULL){
				save_global_conf(SEP_EQUAL,AG_SNMP_CONF,"",\
								"SYSLOG_LOG_FILE_BAK_PATH", "/etc/WAPI/auth/");
		 	}
	     	free_prop(properties,prop_count);	
	     	*var_len = strlen(ap_parameter);
	     	ap_parameter[*var_len]='\0';
         	return (unsigned char *) ap_parameter;
		case SYSLOGSERVERPORT:
			*write_method = write_syslogServerPort;
			memset(ap_parameter,0,sizeof(ap_parameter));
			prop_count=load_prop(SEP_EQUAL,AG_SNMP_CONF,properties);
			get_prop("SYSLOG_SERVER_PORT",ap_parameter,properties,prop_count);
			free_prop(properties,prop_count);	
			ulong_ret = atoi(ap_parameter);
			if (ulong_ret <= 0)
			{
				ulong_ret = 514;
				save_global_conf(SEP_EQUAL,AG_SNMP_CONF,"","SYSLOG_SERVER_PORT", "514");
			}
			return (u_char*) &ulong_ret;
#endif
        default:
      		ERROR_MSG("");
    }
  
    return NULL;
}

/*
*---------add by piyongping 2008-10-30----------------------
*/
#if 1
/*
 * var_syslogSettingTable():
 *   Handle this table separately from the scalar value case.
 *   The workings of this are basically the same as for var_wapiPwlanAp above.
 */
unsigned char *
var_wapiSyslogSettingTable(struct variable *vp,
    	    				oid     *name,
    	    				size_t  *length,
    	    				int     exact,
    	    				size_t  *var_len,
    	    				WriteMethod **write_method)
{

 	static long long_ret;
 	int tableSize,i;
  	static char ap_parameter[256];
	char buffer[104];
	struct syslogConf *head;
  	static struct syslogConf tmp[10],*p,*r;
	
	head = NULL;
	tableSize = loadSyslogConf1(SYSLOG_CONF_FILE,&head);
	memset(tmp,0,sizeof(tmp));
    p=head;
    r=tmp;
    while(p!=NULL){   
    	memcpy(r,p,sizeof(struct syslogConf));
    	p=p->next;
			r++;
    }
    free_Syslog_table(&head);
    head=tmp;
	
	if (header_simple_table(vp,name,length,exact,var_len,write_method,tableSize)
                                                == MATCH_FAILED )
    {
    	return NULL;
	}

    if (name[*length - 1] > tableSize)
    {       
        DEBUGMSGTL(("nb/wapiPwlanAp", "nb -- no match: %lu\n",name[*length - 1]));
        return NULL;
    }
	
	
	memset( syslogConfpara.strFacility, 0, 20 );
	memset( syslogConfpara.strLevel, 0, 20 );
	memset( syslogConfpara.strPath, 0, 64 );
	showSyslogConfInfo(head,syslogConfpara.strFacility,\
					   syslogConfpara.strLevel,syslogConfpara.strPath);
	
    for (i = 1;  i < name[*length - 1];i++)
    {	
		head++;
		DEBUGMSGTL(("nb/wapiPwlanAp", "nb -- %lu != %lu\n",i, name[*length - 1]));
		memset( syslogConfpara.strFacility, 0, 20 );
    	memset( syslogConfpara.strLevel, 0, 20 );
		memset( syslogConfpara.strPath, 0, 64 );
		showSyslogConfInfo(head,syslogConfpara.strFacility,
				   syslogConfpara.strLevel,syslogConfpara.strPath);	
    }

  /* 
   * this is where we do the value assignments for the mib results.
   */
    switch(vp->magic) {
		
    case SYSLOGINDEX:
        long_ret =i;
        return  (u_char*) &long_ret;
    case SYSLOGSERVERPATH:
        *write_method = write_syslogServerPath;
		memset(ap_parameter,'\0', 256);
		memcpy(ap_parameter,syslogConfpara.strPath,strlen(syslogConfpara.strPath));
		*var_len=strlen(syslogConfpara.strPath);
		ap_parameter[*var_len]='\0';
	    return (unsigned char *)&ap_parameter;
    case SYSLOGLEVEL:
        *write_method = write_syslogLevel;
		long_ret = getSyslogLevel(syslogConfpara.strLevel);
        return (u_char*) &long_ret ;
    case SYSLOGFACILITY:
        *write_method = write_syslogFacility;
        long_ret = getSyslogFacility(syslogConfpara.strFacility);
        return (u_char*) &long_ret ;
	 case SYSLOGCONFINFO:
        *write_method = write_syslogConfInfo;
		bzero(ap_parameter, 256);
		memset(buffer,'\0',104);
		strcpy(buffer,head->dataconf);
		sprintf(ap_parameter,"%s",buffer);
        *var_len = strlen(ap_parameter);
		ap_parameter[*var_len]='\0';
        return (unsigned char *) ap_parameter ;
    default:
      ERROR_MSG("");
    }
    return NULL;
}

int
write_syslogServerPath(int      action,
            u_char   *var_val,
            u_char   var_val_type,
            size_t   var_val_len,
            u_char   *statP,
            oid      *name,
            size_t   name_len)
{
    //char strTemp[256];
	int ret;
	char *key;
    switch ( action ) {
        case RESERVE1:
          if (var_val_type != ASN_OCTET_STR) {
              fprintf(stderr, "write to wapiPwlanAp not ASN_OCTET_STR\n");
              return SNMP_ERR_WRONGTYPE;
          }
          if (var_val_len >=256) {
              fprintf(stderr,"write to wapiPwlanAp: bad length\n");
              return SNMP_ERR_WRONGLENGTH;
          }
          break;
        case RESERVE2:
          break;
        case FREE:
          break;
        case ACTION:
			memset(syslogConfpara.strPath,'\0',128);
			//strncpy( syslogConfpara.strPath, var_val,128);
			if(var_val_len>=128)
				var_val_len = 127;
			memcpy(syslogConfpara.strPath, var_val,var_val_len);			
			ret = (int)name[name_len-1];
			key = "path";
			if(saveSyslogConf1(SYSLOG_CONF_FILE,syslogConfpara.strPath,key,ret)<0){
					//return SNMP_ERR_WRONGVALUE;
					return SNMP_ERR_READONLY;
			}
          break;

        case UNDO:
          break;
        case COMMIT:
          break;
    }
    return SNMP_ERR_NOERROR;
}

int
write_syslogFacility(int      action,
            u_char   *var_val,
            u_char   var_val_type,
            size_t   var_val_len,
            u_char   *statP,
            oid      *name,
            size_t   name_len)
{
	int ret;
	char *key;
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
          break;

        case RESERVE2:
          break;
        case FREE:
          break;
        case ACTION:
			bzero(syslogConfpara.strFacility,strlen(syslogConfpara.strFacility));
			strncpy( syslogConfpara.strFacility, facility_Names[* (long *) var_val],64);
			ret = (int)name[name_len-1];
			key = "fac";
			if(saveSyslogConf1(SYSLOG_CONF_FILE,syslogConfpara.strFacility,key,ret)<0){
				return SNMP_ERR_WRONGVALUE;
			}
			
          break;
        case UNDO:
          break;
        case COMMIT:
          break;
    }
    return SNMP_ERR_NOERROR;
}

int
write_syslogLevel(int      action,
				u_char   *var_val,
				u_char   var_val_type,
				size_t   var_val_len,
				u_char   *statP,
				oid      *name,
				size_t   name_len)
{
    long value;
    int ret;
	char *key;
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
          	if( value < 0 || value > 7 ){
			  	fprintf( stderr, "value out of range( 0...7 )\n" );
			  	return SNMP_ERR_WRONGVALUE;
			 }
          	break;
        case RESERVE2:
          	break;
        case FREE:
          	break;
        case ACTION:
			bzero(syslogConfpara.strLevel,strlen(syslogConfpara.strLevel));
			strncpy( syslogConfpara.strLevel, syslog_Alarm_Level[* (long *) var_val],64);	
			ret = (int)name[name_len-1];
			key = "level";
			if(saveSyslogConf1(SYSLOG_CONF_FILE,syslogConfpara.strLevel,key,ret)<0){
				return SNMP_ERR_WRONGVALUE;
			}
			
          	break;
        case UNDO:
          	break;
        case COMMIT:
          	break;
    }
    return SNMP_ERR_NOERROR;
}

int
write_syslogConfInfo(int   action,
						u_char   *var_val,
						u_char   var_val_type,
						size_t   var_val_len,
						u_char   *statP,
						oid      *name,
						size_t   name_len)
{
    char strTemp[256];
	
    switch ( action ) {
        case RESERVE1:
           if (var_val_type != ASN_OCTET_STR) {
              fprintf(stderr, "write to wapiPwlanAp not ASN_OCTET_STR\n");
              return SNMP_ERR_WRONGTYPE;
          }
          if (var_val_len >=256) {
              fprintf(stderr,"write to wapiPwlanAp: bad length\n");
              return SNMP_ERR_WRONGLENGTH;
          }
          break;
        case RESERVE2:
          break;
        case FREE:
          break;
        case ACTION:
			memset(strTemp,'\0',128);	
			if(var_val_len>=128)
				var_val_len = 127;
			memcpy(strTemp, var_val,var_val_len);
			if(saveSyslogConf2(SYSLOG_CONF_FILE, strTemp)<0){
					return SNMP_ERR_READONLY;
			}
			
			syslogConfpara.dEnable = atoi(readSyslogFlag());
			if( syslogConfpara.dEnable == 1 ){
				syslogScript( "restart" );
			}	
          break;
        case UNDO:
          break;
        case COMMIT:
          break;
    }
    return SNMP_ERR_NOERROR;
}

int write_syslogMsgLevel(int      action,
            u_char   *var_val,
            u_char   var_val_type,
            size_t   var_val_len,
            u_char   *statP,
            oid      *name,
            size_t   name_len)
{
    u_long value;
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
   			 value = * (long *) var_val ;
			if( value < 0 || value > 7 )
				return SNMP_ERR_WRONGVALUE;
			break;	  
        case RESERVE2:
			break;
        case FREE:
			break;
        case ACTION:
			syslogConfpara.dMsgLevel = * (long *) var_val ;
 			break;
        case UNDO:
			break;
        case COMMIT:
			break;
    }
    return SNMP_ERR_NOERROR;
}


int
write_syslogEnable(int      action,
            u_char   *var_val,
            u_char   var_val_type,
            size_t   var_val_len,
            u_char   *statP,
            oid      *name,
            size_t   name_len)
{
    long value;
    char temp[256];
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
          if (value!=0 && value!=1) {
              fprintf(stderr,"value out of range: 0...1 \n");
              return SNMP_ERR_WRONGVALUE;
          }
          break;
        case RESERVE2:
          break;
        case FREE:
          break;
        case ACTION:
			value= * (long *) var_val;
			syslogConfpara.dEnable=value;
			memset(temp,0,256);	 
			sprintf(temp,"%ld",syslogConfpara.dEnable);	      
			save_global_conf(SEP_EQUAL,AG_SNMP_CONF,"","SYSLOG_FLAG",temp);
			if(syslogConfpara.dEnable==1){
				syslogScript("restart");
			}
			else{
				syslogScript("stop");
			}
			
          break;
        case UNDO:
          break;
        case COMMIT:
          break;
    }
    return SNMP_ERR_NOERROR;
}


int
write_syslogMsgSend(int      action,
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
              fprintf(stderr, "type error!\n");
              return SNMP_ERR_WRONGTYPE;
          }
          if (var_val_len > 128) {
              fprintf(stderr,"length must less than 256!\n");
              return SNMP_ERR_WRONGLENGTH;
          }
         case RESERVE2:
          break;
        case FREE:
          break;
        case ACTION:
	      memset( syslogConfpara.strMsg, 0, 128 );
	      memcpy( syslogConfpara.strMsg, var_val, var_val_len );
 	      syslogConfpara.strMsg[var_val_len] = '\0';
	      sendLoggerMsg( syslogConfpara.dMsgLevel, syslogConfpara.strMsg );
          break;
        case UNDO:
          break;
        case COMMIT:
          break;
    }
    return SNMP_ERR_NOERROR;
}


int
write_syslogUpdataIntervalTime(int      action,
            u_char   *var_val,
            u_char   var_val_type,
            size_t   var_val_len,
            u_char   *statP,
            oid      *name,
            size_t   name_len)
{	
    char temp[20];
	
    switch ( action ) {
         case RESERVE1:
          if (var_val_type != ASN_OCTET_STR) {
              fprintf(stderr, "type error!\n");
              return SNMP_ERR_WRONGTYPE;
          }
          if (var_val_len > 20) {
              fprintf(stderr,"length must less than 20!\n");
              return SNMP_ERR_WRONGLENGTH;
          }
         case RESERVE2:
          break;
        case FREE:
          break;
        case ACTION:
			memset(temp,0,20);	 
			//edit by xsj 080814
			#if 0
			sprintf(temp,"%s",var_val);	      
			#else
			sprintf(temp,"%d", atoi(var_val) );	      
			if (strlen(temp) != var_val_len)
				break; //return SNMP_ERR_WRONGVALUE;
			#endif
			save_global_conf(SEP_EQUAL,AG_SNMP_CONF,"","UPDATA_SYSLOG_TIME",temp);
			do_restart_syslog();
          break;
        case UNDO:
          break;
        case COMMIT:
          break;
    }
    return SNMP_ERR_NOERROR;
}
#if 0
int
write_syslogSaveLogBakPath(int      action,
            u_char   *var_val,
            u_char   var_val_type,
            size_t   var_val_len,
            u_char   *statP,
            oid      *name,
            size_t   name_len)
{
	 char temp[100];
	switch ( action ) {
         case RESERVE1:
          if (var_val_type != ASN_OCTET_STR) {
              fprintf(stderr, "type error!\n");
              return SNMP_ERR_WRONGTYPE;
          }
          if (var_val_len > 100) {
              fprintf(stderr,"length must less than 100!\n");
              return SNMP_ERR_WRONGLENGTH;
          }
         case RESERVE2:
          break;
        case FREE:
          break;
        case ACTION:
	     	memset(temp,0,100);	 
			sprintf(temp,"%s",var_val);	      
			save_global_conf(SEP_EQUAL,AG_SNMP_CONF,"","SYSLOG_LOG_FILE_BAK_PATH",temp);
          break;
        case UNDO:
          break;
        case COMMIT:
          break;
    }
    return SNMP_ERR_NOERROR;
}
 #endif

int
write_syslogServerPort(int      action,
            u_char   *var_val,
            u_char   var_val_type,
            size_t   var_val_len,
            u_char   *statP,
            oid      *name,
            size_t   name_len)
{
    long value;
	char temp[8];
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
          if (value <= 0) {
              return SNMP_ERR_WRONGVALUE;
          }
          break;
         case RESERVE2:
          break;
        case FREE:
          break;
        case ACTION:
			value= * (long *) var_val;
			memset(temp,0,sizeof(temp));	 
			sprintf(temp,"%ld",value);
			save_global_conf(SEP_EQUAL,AG_SNMP_CONF,"","SYSLOG_SERVER_PORT",temp);
			do_restart_syslog();
          break;
        case UNDO:
          break;
        case COMMIT:
          break;
    }
    return SNMP_ERR_NOERROR;
}

void loadSyslogPara( void ){    
    prop_data properties[KEYS_MAX];
    int prop_count=0;
    char ap_parameter[256];
	FILE *fptr;
	//before syslogd start need kill old syslogd
	killallSyslogd();
	memset( syslogConfpara.strFacility, 0, 64 );
	memset( syslogConfpara.strLevel, 0, 64 );
	memset( syslogConfpara.strPath, 0, 128 );
	if( 0 != loadSyslogConf( SYSLOG_CONF_FILE, syslogConfpara.strFacility,\
	 	                  syslogConfpara.strLevel, syslogConfpara.strPath) ||
	    				10 < getSyslogFacility(syslogConfpara.strFacility) || 
	                           7 < getSyslogLevel(syslogConfpara.strLevel))
	{
		creatSyslogConf(SYSLOG_CONF_FILE);
       }
	/*******************************************/

	memcpy(ap_parameter,readSyslogFlag(),256);
	if((0!=strcmp(ap_parameter,"1"))&&(0!=strcmp(ap_parameter,"0"))){
		memset(ap_parameter,0,256);
		strcpy(ap_parameter,"0");		
	    save_global_conf( SEP_EQUAL,AG_SNMP_CONF,"","SYSLOG_FLAG",ap_parameter );
	}
	syslogConfpara.dEnable= atoi(ap_parameter);
	if(syslogConfpara.dEnable==1)
		syslogScript( "start"); 

}


int killallSyslogd( void )
{
    int pid;
    char *argv[4];

	argv[0]="/bin/killall";
	argv[1]="-9";
	argv[2]="syslogd";
	argv[3]=NULL;
	
	pid=fork();
	if(pid==0){
		execv(argv[0],argv);
		exit(1);
	}
	else if(pid != -1)
	{
		wait(NULL);
	}
	else{

	}
	return SNMP_ERR_GENERR;	
}


char *readSyslogFlag(void)
{
	prop_data properties[KEYS_MAX];
    int prop_count=0;
    static char ap_parameter[256];
	memset(ap_parameter,0,256);
	prop_count=load_prop(SEP_EQUAL, AG_SNMP_CONF, properties);//piyongping add
	get_prop("SYSLOG_FLAG",ap_parameter,properties,prop_count);
	free_prop(properties,prop_count);
	return ap_parameter;
}

int 
showSyslogConfInfo( struct syslogConf *head,char *fac,
				   char *level,char *path )
{ 
	char *pos =NULL;
	char *prev_pos=NULL;
	pos = head->dataconf;
	prev_pos=head->dataconf;
	//find 'fac'
	while(((*pos)!='\0') && ((*pos)!='\n') && ((*pos)!='.')){
		pos++;		   		   
	}
	memcpy( fac, prev_pos, pos-prev_pos );
	fac[pos-prev_pos] = '\0';
	//find 'level'
    prev_pos = ++pos;
	while(((*pos)!='\0') &&((*pos)!='\n') && ((*pos)!=' ') &&((*pos)!='\t')){
		pos++;
	}
	memcpy( level, prev_pos, pos-prev_pos );
	level[pos-prev_pos] = '\0';
		   
	//find 'path' "@" @@ "/"
	while(((*pos)!='\0' )&& ((*pos)!='\n') && ((*pos)!='@') && ((*pos)!='/')){
		pos++;		       
	}	
	if((*pos)=='@'){
		prev_pos = ++pos;
	}
	if((*pos)=='/'){
		prev_pos = pos++;
	}
	//prev_pos = ++pos;
	while(((*pos)!='\0') &&((*pos)!='\n')&&((*pos) !=' ') &&((*pos)!= '\t')){
		pos++;		
	}	   		   
	memcpy( path,prev_pos, pos-prev_pos );
	path[pos-prev_pos] = '\0';
	return 1;
}

int
saveSyslogConf1(const char *strFileName,char *syslogType,char *key,int index)
{
	
    FILE *fp;
	char *head,*prev,*pos,*pt;
	char strline[128],buffer[512];
	char headstr[64],laststr[64],newstr[128];
	
	int countlen =0;
	int reslen = 0;
	int flag = 0;
	memset( buffer, '\0', 512 );

	if((fp=fopen(strFileName, "r"))==NULL){
		printf("Open read config file error!\n");
		return -1; //return NULL;
	}
	
	while(!feof(fp)){
		pos =NULL;
		prev=NULL;
		++flag;
		memset( strline, '\0', 128 );
		if( fgets(strline, 128, fp) !=NULL) {

			if(index == flag){
				countlen=strlen(strline);
				reslen=pos-prev;
				//find 'fac'
				if(0==strcmp(key,"fac")){
					pos = prev = strline;
					if((pos!=NULL) && (prev!=NULL)){
						while(((*pos)!='\0') && ((*pos)!='\n') && ((*pos)!='.')){
						  pos++;	
						}
						memset(laststr,'\0',64);
						memset(newstr,'\0',128);
						memmove(laststr,pos,(countlen-reslen));
						pos = laststr;
						while(((*pos)!='\0') && ((*pos)!='\n') && ((*pos)!='\r')){
						  pos++;	
						}
						*pos = '\0';
						sprintf(newstr,"%s%s\n",syslogType,laststr);
						strcat( buffer, newstr);
					}
				}
				//find 'level'
				if(0==strcmp(key,"level")){
					pos = prev = strstr(strline,".");
					if((pos!=NULL) && (prev!=NULL)){
						head = strline;
						prev = ++pos;
						while(((*pos)!='\0') &&((*pos)!='\n') && ((*pos)!=' ') &&((*pos)!='\t')){
							pos++;
						}
						memset(headstr,'\0',64);
						memset(laststr,'\0',64);
						memset(newstr,'\0',128);
						memmove(headstr,head,(prev-head));
						memmove(laststr,pos,(countlen-reslen));
						pos = laststr;
						while(((*pos)!='\0') && ((*pos)!='\n') && ((*pos)!='\r')){
						  pos++;	
						}
						*pos = '\0';
						sprintf(newstr,"%s%s%s\n",headstr,syslogType,laststr);
						strcat( buffer, newstr);
					}
				}
				//find 'path'
				if(0==strcmp(key,"path")){
					if(pos = strstr(strline,"@")){
						prev = ++pos;
					}
					if(pt = strstr(strline,"/")){
						//prev = pt++;
						fclose(fp);
						return -1;
					}
					if(pos!=NULL || pt!=NULL){
						head = strline;
						memset(headstr,'\0',64);
						memset(newstr,'\0',128);
						memmove(headstr,head,(prev-head));
						sprintf(newstr,"%s%s\n",headstr,syslogType);
						strcat( buffer, newstr);
					}
				}
			}else{
				strcat( buffer, strline);
			}
		}
	}
	fclose(fp);
	fp = NULL;
	if( fp = fopen( strFileName, "w" )){
		if(fwrite(buffer, strlen(buffer),1,fp)<1){
			printf("write file error");
			return -1;
		}
    }
	fclose(fp);
	return 1;
}

int 
saveSyslogConf2(const char * strFileName, char *concent){
	FILE *fp;
	char *head,*prev,*pos,*pt;
	char strline[128],laststr[128],buffer[512];
	
	memset( buffer, '\0', 512 );

	if((fp=fopen(strFileName, "r"))==NULL){
		printf("Open read config file error!\n");
		return -1; //return NULL;
	}
	while(!feof(fp)){
		memset( strline, '\0', 128 );
		if( fgets(strline, 128, fp) !=NULL) {
			strcat( laststr, strline);	
		}
		sprintf(buffer,"%s\n%s\n",laststr,concent);
	}
	fclose(fp);
	fp = NULL;
	if( fp = fopen( strFileName, "w" )){
		if(fwrite(buffer, strlen(buffer),1,fp)<1){
			printf("write file error");
			return -1;
		}
    }
	fclose(fp);
	return 1;


}


int
loadSyslogConf1( const char *strFileName, struct syslogConf **head)
{ 
    char strLine[128];
    FILE *fptr;
	char *pt;
	int count=0;
	struct syslogConf *tmp,*r;
	fptr=fopen(strFileName, "r");
	if(fptr==NULL){
		printf("Open read config file error!\n");
		return -1;
	}
	while(!feof(fptr)){
		memset( strLine, '\0', 128 );
		if( fgets(strLine, 128, fptr)!= NULL) {
			if(strlen(strLine)>1 &&(strLine!=" ")){
				tmp = (syslogConf *) malloc (sizeof (syslogConf));
				tmp->next =NULL;
				pt=tmp->dataconf;
	  	  		memcpy(pt, strLine,strlen(strLine));
				pt[strlen(strLine)-1]='\0';
				++count;
				if((*head)==NULL){
					(*head)=tmp;
				}
				else{
					r = (*head);
					while(r->next!=NULL){
						r = r->next;
					}
					r->next = tmp;
		  		}
			}
		}
	} 
    fclose(fptr); 
    return count;
}

void  free_Syslog_table( struct syslogConf **head)
{
	struct syslogConf *sp,*ep;
	sp=*head;
	while(sp!=NULL){
	ep=sp;
	sp=sp->next;
	free(ep);
	}
}

int 
loadSyslogConf( const char *strFileName, char *fac , char *level,char *path)
{ 
    char strLine[512];
    FILE *fptr;
    memset( strLine, '\0', 512 );
	fptr=fopen( strFileName, "r");
    if( fptr!=NULL ) {
		if( fgets(strLine, 512, fptr) > 0 ) {
	       char *pos = strLine;
	       char *prev_pos = strLine;
	      //find 'fac'
	       while(( (*pos)!='\0') && ((*pos)!='\n') && ((*pos)!='.') ){
				pos++;		   		   
	       }
			memcpy( fac, prev_pos, pos-prev_pos );
			fac[pos-prev_pos] = '\0';
			//find 'level'
    	   	prev_pos = ++pos;
	       while(  ((*pos)!='\0') &&( (*pos)!='\n') && ((*pos)!=' ') &&( (*pos)!='\t') ){
				pos++;
	       }
			memcpy( level, prev_pos, pos-prev_pos );
			level[pos-prev_pos] = '\0';
		   
			//find 'path' "@" @@ "/"
			while(((*pos)!='\0' )&& ((*pos)!='\n') && ((*pos)!='@') && ((*pos)!='/')){
				pos++;		       
			}
			if((*pos)=='@'){
			prev_pos = ++pos;
			}
			if((*pos)=='/'){
				prev_pos = pos++;
			}
			//prev_pos = ++pos;
			while(((*pos)!='\0') &&((*pos)!='\n')&&((*pos) !=' ') &&((*pos)!= '\t')){
				pos++;		
			}	   		   
			memcpy( path,prev_pos, pos-prev_pos );
			path[pos-prev_pos] = '\0';
		}
    }
    else{
		printf("open syslog config file error\n\n\n");
    return -1;
    }
    fclose( fptr ); 
    return 0;
}


int 
creatSyslogConf( const char *strFileName)
{
	FILE *fptr;
	char buffer0[128];
	char buffer1[128];
	char strBuffer[256];
	memset( syslogConfpara.strFacility, 0, 20 );
	memset( syslogConfpara.strLevel, 0, 20 );
	memset( syslogConfpara.strPath, 0, 64 );
	strcpy( syslogConfpara.strFacility, "*" );
	strcpy( syslogConfpara.strLevel, "notice" );
	strcpy( syslogConfpara.strPath, "192.168.14.254" );
	sprintf( buffer0, "%s.%s     @%s\n",syslogConfpara.strFacility,\
								syslogConfpara.strLevel,syslogConfpara.strPath );
	strcat(strBuffer,buffer0);
	memset( syslogConfpara.strFacility, 0, 20 );
	memset( syslogConfpara.strLevel, 0, 20 );
	memset( syslogConfpara.strPath, 0, 64 );
	strcpy( syslogConfpara.strFacility, "*" );
	strcpy( syslogConfpara.strLevel, "err" );
	strcpy( syslogConfpara.strPath, "/var/log/syslog.log" );
	sprintf( buffer1, "%s.%s     %s",syslogConfpara.strFacility,\
								syslogConfpara.strLevel,syslogConfpara.strPath );
	strcat(strBuffer,buffer1);
	fptr = fopen( strFileName, "w+" );

	
    	if(fptr!=NULL){
		if(fwrite(strBuffer, strlen(strBuffer),1,fptr)<0){
			printf("Write syslog config file error");
			return -1;	
		}
    }
    fclose( fptr );
    return 1;
	
}

int copySyslogConf( char *srcPath,  char *desPath )
{
    int pid;
    char *argv[4];

	argv[0]="/bin/cp";
	argv[1]=srcPath;
	argv[2]=desPath;
	argv[3]=NULL;
	
	pid=fork();
	if(pid==0){
		execv(argv[0],argv);
		exit(1);
	}
	else if(pid != -1)
	{
		wait(NULL);
	}
	else{

	}
	return SNMP_ERR_GENERR;	
}

int syslogScript( char *strType ) //0:stop; 1:start; 2:restart
{		
    int pid;
	char * argv[10 ];
   	argv[0]="/etc/WAPI/snmp/syslog.sh";
	argv[1]=strType;
   	argv[2]=NULL;   			
		pid =fork();
          	if(pid == 0)
          	{
			   execv(argv[0],argv);
          	   exit(1);
          	}
          	else if(pid > 0)
			{
			   wait(NULL);
			
			}
          	else
			return SNMP_ERR_GENERR;	
		return 0;
}

void do_restart_syslog()
{
	char temp[4];
	int prop_count=0;
	prop_data properties[KEYS_MAX];
	prop_count=load_prop(SEP_EQUAL,AG_SNMP_CONF,properties);
	get_prop("SYSLOG_FLAG",temp,properties,prop_count);
	free_prop(properties,prop_count);
	if (atoi(temp) == 1)
		syslogScript("restart");
}
long getSyslogLevel( const char *strLevel )
{
    long dLevel = 0;
    for( dLevel=0; dLevel<8; dLevel++ ){
		if( 0 == strcmp(strLevel, syslog_Alarm_Level[dLevel]) )
		{
		    break;
		}
    }
    return dLevel;
}
int getSyslogFacility( const char *strIndex )
{
    int dindex;
    for( dindex=0; dindex<10; dindex++ ){
		if( 0 == strcmp(strIndex, facility_Names[dindex]) )
		{
		    break;
		}
    }
    return dindex;
}
int sendLoggerMsg( int iLevel, char *strMsg )
{
    int pid;
    char scr_file[256];
    char * aargv[5];
    char strLevel[256];

    strcpy( scr_file, "/bin/logger" );
    strcpy( strLevel, "mail." );
    strcat( strLevel, syslog_Alarm_Level[iLevel] );
    aargv[0] = scr_file;
    aargv[1] = "-p";
    aargv[2] = strLevel;
    aargv[3] = strMsg;
    aargv[4] = NULL;

		pid =fork();
          	if(pid == 0)
          	{
          	  	execv(aargv[0],aargv);
          	  	exit(1);
          	}
			//此处会导致每次syslogd在系统重起后，首次启动syslogd后，测试发送消息不能正常发出。使接收不到日志消息
			#if 0
          		else if(pid != -1)
			#else
			  else if(pid > 0)//by piyp midify 2008-11-18
			#endif
			{
               wait(NULL);
			}
          	else
			{
				return SNMP_ERR_GENERR;	
			}
		return 0;
}


#endif


//bottom
