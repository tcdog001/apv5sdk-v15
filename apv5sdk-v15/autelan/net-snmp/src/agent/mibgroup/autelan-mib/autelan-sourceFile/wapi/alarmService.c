/**********************************************************************************
* Copyright (c) 2008-2011  Beijing Autelan Technology Co. Ltd.
* All rights reserved.
*
* filename: alarmService.c
* description:  implementation for display the alarm information.
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

#include "wapiPwlanAp.h"    
#include "alarmService.h"

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



oid    alarm_variables_oid[] = { 1,3,6,1,4,1,4000,100};

struct variable4 alarm_variables[] = {

{WAPIALARMTIMES,  ASN_INTEGER,  RWRITE,  var_alarmService, 2,  {8, 1 }},
{WAPIALARMFLAG,  ASN_INTEGER,  RWRITE,  var_alarmService, 2,  {8, 2 }},
{ALARMID,  ASN_OCTET_STR,  RWRITE,  var_wapiApAlarmAdminTable, 4,  { 8,3, 1, 1 }},
{ALARMNAME,  ASN_OCTET_STR,  RWRITE,  var_wapiApAlarmAdminTable, 4,  {8, 3, 1, 2 }},
{ALARMLEVEL,  ASN_OCTET_STR,  RWRITE,  var_wapiApAlarmAdminTable, 4,  {8, 3, 1, 3 }},
{ALARMTYPE,  ASN_OCTET_STR,  RWRITE,  var_wapiApAlarmAdminTable, 4,  { 8,3, 1, 4 }},
{ALARMREASONID,  ASN_INTEGER,  RWRITE,  var_wapiApAlarmAdminTable, 4,  {8, 3, 1, 5 }},
{ALARMREASON,  ASN_OCTET_STR,  RWRITE,  var_wapiApAlarmAdminTable, 4,  {8, 3, 1, 6 }},
{ALARMCREATTIMES,  ASN_OCTET_STR,  RWRITE,  var_wapiApAlarmAdminTable, 4,  {8, 3, 1, 7 }},
{ALARMSTATE,  ASN_INTEGER,  RWRITE,  var_wapiApAlarmAdminTable, 4,  { 8,3, 1, 8 }},
{ALARMHEADING,  ASN_OCTET_STR,  RWRITE,  var_wapiApAlarmAdminTable, 4,  {8, 3, 1, 9 }},
{ALARMCONTENT,  ASN_OCTET_STR,  RWRITE,  var_wapiApAlarmAdminTable, 4,  {8, 3, 1, 10 }},

{WAPISTARTSCANAPENABLED,  ASN_INTEGER,  RWRITE,  var_alarmService, 2,  {8, 4}},
{WAPIADDNEIGHBORHOODAPMACADDR,  ASN_OCTET_STR,  RWRITE,  var_alarmService, 2,  {8, 5}},

{WAPINEIGHBORHOODAPMACID,  ASN_INTEGER,  RONLY,  var_wapiScanApTable, 4,  {8, 6, 1, 1 }},
{WAPINEIGHBORHOODAPMACLIST,  ASN_OCTET_STR,  RWRITE,  var_wapiScanApTable, 4,  {8, 6, 1, 2 }},
};
static int alarmTimes;
static  unsigned long  alarmId;
static int alarmFlag;
int temp_start_scan_ap_enabled = 0;
void 
init_alarmService(void)
{
    prop_data properties[KEYS_MAX];
    int prop_count=0;
    char ap_parameter[256];
    long int ret ;
		 /* get basic info */
    prop_count=load_prop(SEP_EQUAL,AG_SNMP_CONF,properties);
    memset(ap_parameter,0,256);
    get_prop("ALARM_TIMES",ap_parameter,properties,prop_count);
    alarmTimes = atoi(ap_parameter);
    
    memset(ap_parameter,0,256);
    get_prop("ALARM_FLAG",ap_parameter,properties,prop_count);
		if( strcmp(ap_parameter,"")==0 ){
			strcpy(ap_parameter,"0");
		} 
    alarmFlag= atoi(ap_parameter);
    free_prop(properties,prop_count);
    
    if( alarmFlag != 0 ){	
   		if(alarmTimes>0)   
        		alarmId=snmp_alarm_register(alarmTimes,  //10  /* seconds */
                        SA_REPEAT,      /* repeat (every 30 seconds). */
                        register_callback,      /* our callback */
                        NULL );    /* no callback data needed */
    		else 
        		alarmId=snmp_alarm_register(ALARM_TET_FREQUENCY,  //10  /* seconds */
                        SA_REPEAT,      /* repeat (every 30 seconds). */
                        register_callback,      /* our callback */
                        NULL );    /* no callback data needed */
		}
    
    REGISTER_MIB("autelan-mib/autelan-sourceFile/wapi/alarm", alarm_variables, variable4,
               alarm_variables_oid);

}


unsigned char*
var_alarmService(struct variable *vp, 
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
	static long long_ret;
    if( header_generic(vp,name,length,exact,var_len,write_method) == MATCH_FAILED ) 
    {
    	return NULL;
    }
  /* 
   * this is where we do the value assignments for the mib results.
   */
    switch(vp->magic) {
			 case WAPIALARMTIMES:
            *write_method = write_wapialarmtimes;
	    	memset(ap_parameter,0,256);
            prop_count=load_prop(SEP_EQUAL,AG_SNMP_CONF,properties);
            get_prop("ALARM_TIMES",ap_parameter,properties,prop_count);    
	    	free_prop(properties,prop_count) ;
	    	long_ret=atoi(ap_parameter);
	    	//printf("alarm times=%d\n",long_ret);
            return (u_char*) &long_ret ;
  		 case WAPIALARMFLAG:
            *write_method = write_wapialarmflag;
            prop_count=load_prop(SEP_EQUAL,AG_SNMP_CONF,properties);
            get_prop("ALARM_FLAG",ap_parameter,properties,prop_count);
            free_prop(properties,prop_count) ;
            long_ret=atoi(ap_parameter);
            //printf("alarm flag = %d \n ",long_ret);
            return (u_char*) &long_ret ;
       case WAPISTARTSCANAPENABLED:
            *write_method = write_wapiStartScanApEnabled;	  
			long_ret=temp_start_scan_ap_enabled;
            return (u_char*) &long_ret ;
       case WAPIADDNEIGHBORHOODAPMACADDR:
           *write_method = write_wapiAddNeighborhoodApMacAddr;
			memset(ap_parameter,0,256);
			strcpy(ap_parameter,"00:00:00:00:00:00");
			*var_len = strlen(ap_parameter);
	    	ap_parameter[*var_len]='\0'; 
            return (u_char*) ap_parameter ;	
        default:
      		ERROR_MSG("");
    }
  
    return NULL;
}


/*
 * var_wapiApAlarmAdminTable():
 *   Handle this table separately from the scalar value case.
 *   The workings of this are basically the same as for var_wapiPwlanAp above.
 */
unsigned char *
var_wapiApAlarmAdminTable(struct variable *vp,
    	    oid     *name,
    	    size_t  *length,
    	    int     exact,
    	    size_t  *var_len,
    	    WriteMethod **write_method)
{

    /* variables we may use later */
    static long long_ret;
    static u_long ulong_ret;
    static unsigned char string[256];
    static oid objid[MAX_OID_LEN];
    int ret,i;
 
   if (header_simple_table(vp,name,length,exact,var_len,write_method, 1)
                                                == MATCH_FAILED )
    return NULL;
    for(i=0;i<name[*length-1];i++) {
    }
    memset(string,0,256);
    switch(vp->magic) {
    case ALARMID:
	*var_len=0;
        return (u_char*) string;
    case ALARMNAME:
	*var_len=0;
        return (u_char*) string;
    case ALARMLEVEL:
	*var_len=0;
        return (u_char*) string;
    case ALARMTYPE:
	*var_len=0;
        return (u_char*) string;
	case ALARMREASONID:
	long_ret=0;
        return (u_char*) &long_ret;
    case ALARMREASON:
	long_ret=0;
        return (u_char*) &long_ret;
    case ALARMCREATTIMES:
        long_ret=0;
        return (u_char*) &long_ret;
    case ALARMSTATE:
        long_ret=0;
        return (u_char*) &long_ret;
    case ALARMHEADING:
	*var_len=0;
        return (u_char*) string;
    case ALARMCONTENT:
	*var_len=0;
        return (u_char*) string;
    default:
      ERROR_MSG("");
    }
    return NULL;
}

/*
*---------add by piyongping 2008-12-1----------------------
*          扫描相邻的不安全的AP
*/


unsigned char *
var_wapiScanApTable(struct variable *vp,
    	    				oid     *name,
    	    				size_t  *length,
    	    				int     exact,
    	    				size_t  *var_len,
    	    				WriteMethod **write_method)
{

 	static unsigned  long  ulong_ret;
	static long long_ret;
    static unsigned char string[SPRINT_MAX_LEN];
 	int tableSize,i,id;
    prop_data properties[KEYS_MAX];
    int prop_count=0;
	static char ap_parameter[256];
	
	tableSize = get_Mac_All_list();
    if(tableSize==0)  return 0;
    
    if (header_simple_table(vp,name,length,exact,var_len,write_method, tableSize)
                                                == MATCH_FAILED )
    {
    	return NULL;
    }
	//get index
	id = (name[*length - 1]<2)?1:name[*length - 1];
    if (name[*length - 1] > tableSize)
    {       
        DEBUGMSGTL(("nb/wapiPwlanAp", "nb -- no match: %lu\n",i));
        return NULL;
    }
	
	for (i=1;i<name[*length-1];i++)
    {
        DEBUGMSGTL(("autelan-mib/autelan-sourceFile/wapi/wapiPwlanAp", "wapi -- %lu != %lu\n",i, name[*length - 1]));
		
    }
  

  /* 
   * this is where we do the value assignments for the mib results.
   */
    switch(vp->magic) {
   
    	case WAPINEIGHBORHOODAPMACID:
			ulong_ret = id;
	    	return (u_char *)&ulong_ret;
		case WAPINEIGHBORHOODAPMACLIST:
        	*write_method = write_wapiNeighborhoodApMacList;
			memset(ap_parameter,0,256);
			ulong_ret = get_Mac_list(id,ap_parameter);
			printf("ulong_ret= %d\n",ulong_ret);
			if(ulong_ret==0){
				memset(ap_parameter,0,256);
			}
			*var_len = strlen(ap_parameter);
	    	ap_parameter[*var_len]='\0'; 
	    return (u_char *)ap_parameter;
   
    default:
      ERROR_MSG("");
    }
    return NULL;
}

//// add by piyp 2008-12-1
int
get_Mac_All_list()
{
	char strLine[64];
    FILE *fptr;
	int count=0;
	
	fptr=fopen(WIRELESS_CONF, "r");
	if(fptr==NULL){
		/*ljy--test start*/
		printf("%s: test.\n", __func__);
		/*ljy--test end*/
		printf("Open read config file error!\n");
		return -1;
	}
	while(!feof(fptr)){
		memset( strLine, '\0', 64 );
		if( fgets(strLine, 64, fptr)!= NULL) {
			if(strstr(strLine,"NEIGHBORHOOD_AP_MAC")){
				count++;
			}
		}
	} 

    fclose(fptr); 
    return count;

}

// add by piyp 2008-12-1
int
get_Mac_list(int index,char *mac)
{
	prop_data properties[KEYS_MAX];
    int prop_count=0;
	char macAddrName[25],temp[5];
	char mac_addr[32];
	memset(mac_addr,0,32);
	memset(macAddrName,0,25);
	memset(temp,0,5);
	strcpy(macAddrName,"NEIGHBORHOOD_AP_MAC_");
	sprintf(temp,"%d",index);
	strcat(macAddrName,temp);
	
    prop_count=load_prop(SEP_EQUAL,WIRELESS_CONF,properties);
	if(get_prop(macAddrName,mac_addr,properties,prop_count)==NULL){
		/*ljy--test start*/
		printf("%s: prop_count %d.\n", __func__, prop_count);
		/*ljy--test end*/
		return 0;
	}
	free_prop(properties,prop_count);
	
	//printf("%s: mac_addr = %s.\n", __func__, mac_addr);
	memcpy(mac,mac_addr,32);
	//sprintf(mac,"%2.2x:%2.2x:%2.2x:%2.2x:%2.2x:%2.2x",mac_addr[0],mac_addr[1],\
	//									mac_addr[2],mac_addr[3],\
	//			                 		mac_addr[4],mac_addr[5]);
	
	return 1;
	
}



int
write_wapialarmtimes(int      action,
            u_char   *var_val,
            u_char   var_val_type,
            size_t   var_val_len,
            u_char   *statP,
            oid      *name,
            size_t   name_len)
{
    long value;
    int size;
    char temp[16];

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
	   	if (value< MIN_ALARM_TIMES || value >MAX_ALARM_TIMES) {
              fprintf(stderr,"write to wapiPwlanAp: times error \n");
              return SNMP_ERR_WRONGVALUE;
          }
          break;

        case RESERVE2:
          break;
        case FREE:
          break;
        case ACTION:
	     /* write config file .*/
	      value = * (long *) var_val ;
	      memset(temp,0,sizeof(temp));
	      sprintf(temp,"%d",value);
              save_global_conf(SEP_EQUAL,AG_SNMP_CONF,"","ALARM_TIMES",temp);

	      /* write alarm time value.*/
				if( alarmTimes != value ){
					if( alarmFlag != 0 ){
						snmp_alarm_unregister(alarmId);	   
		         			
			     			alarmId=snmp_alarm_register(value,  //10  /* seconds */
							SA_REPEAT,      /* repeat (every 30 seconds). */
		                        		register_callback,      /* our callback */
		                	        	NULL );    /* no callback data needed */
			     		}
			
					alarmTimes=value;
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
write_wapialarmflag(int      action,
            u_char   *var_val,
            u_char   var_val_type,
            size_t   var_val_len,
            u_char   *statP,
            oid      *name,
            size_t   name_len)
{
    long value;
    int size;
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
	  			value = * (long *) var_val ;
	  			if ( value != 0 && value != 1 ) {
              fprintf(stderr,"write to wapiPwlanAp: flag error \n");
              return SNMP_ERR_WRONGVALUE;
          }

          break;
        case RESERVE2:
          break;
        case FREE:   
          break;
        case ACTION:
       
          value = * (long *) var_val;
	     /* write config file .*/
	      memset(temp,0,10);
	      sprintf(temp,"%d",value);
              save_global_conf(SEP_EQUAL,AG_SNMP_CONF,"","ALARM_FLAG",temp);

	      /* write alarm time value.*/
				if( alarmFlag != value ){
		
					if( alarmFlag != 0 && value == 0 ){
						snmp_alarm_unregister(alarmId);	   
		          		}
					else if( alarmFlag == 0 && value != 0 ){
						alarmId=snmp_alarm_register(alarmTimes,  //10  /* seconds */
		        	                SA_REPEAT,      /* repeat (every 30 seconds). */
		                	        register_callback,      /* our callback */
		                        	NULL );    /* no callback data needed */
			     		}
			     		
					alarmFlag=value;
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
write_wapiStartScanApEnabled(int      action,
								u_char   *var_val,
								u_char   var_val_type,
								size_t   var_val_len,
								u_char   *statP,
								oid      *name,
								size_t   name_len){
    long tmpvalue;
    u_char value;
    prop_data properties[KEYS_MAX];
    int prop_count = 0;
   	char temp[5];
	struct start_scan_ap scanap;
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
   	      tmpvalue = * (long *) var_val ;
	        if((tmpvalue <0)|| (tmpvalue >1) )
		        return SNMP_ERR_WRONGVALUE;
          break;
        case RESERVE2:
          break;
        case FREE:
            
          break;
        case ACTION:
   	     	memset(temp,0,5);
			sprintf(temp,"%d",* (long *) var_val);
			int isStart= * (long *) var_val;
			temp_start_scan_ap_enabled = isStart;
			int i, count = 0;
			char mac_addr[512],mac[20];
			char *p;

			memset(&scanap,0,sizeof(scanap));
			memset(mac_addr,0,512);
			memset(mac,0,20);
		
			p=mac_addr;
			//得到所有MAC列表
			count = get_Mac_All_list();
			/*ljy--test start*/
			printf("%s: count %d.\n", __func__, count);
			/*ljy--test end*/
			for(i=1;i<=count;i++){
				get_Mac_list(i, mac);
				memcpy(p,mac,20);
				printf("%s: Before point change. mac_addr = %s.\n", __func__, mac_addr);
				p += 20;
				printf("%s: After point change. mac = %s, mac_addr = %s.\n", __func__, mac, mac_addr);
			}
			/*ljy--test start*/
			printf("%s: macaddr\n", __func__);
			for (i = 0; i < 512; i++)
			{
				printf("%c", mac_addr[i]);
				if (((i + 1) % 64) == 0)
					printf("\n");
			}
			printf("\n");
			printf("%s: isStart %d.\n", __func__, isStart);
			/*ljy--test end*/
			
			if(isStart==1){
				scanap.symbol=WHITE_SCAN_AP;
				scanap.data_len= (count * 20);
   	    		memcpy(scanap.data,mac_addr,(count * 20));
				/*ljy--test start*/
				printf("%s: scanap.data %s.\n", __func__, scanap.data);
				/*ljy--test end*/
				set_ioctl_param(&scanap);
				/*ljy--test start*/
				printf("%s: test.\n", __func__);
				/*ljy--test end*/
				
			}else{
				scanap.symbol=END_SCAN_AP;
				scanap.data_len= (count * 20);
   	    		memcpy(scanap.data,mac_addr,(count * 20));
				set_ioctl_param(&scanap);

			}
                      
          break;
        case UNDO:
          break;
        case COMMIT:
          break;
    }
    return SNMP_ERR_NOERROR;
}

// add by piyp 2008-12-1
int set_ioctl_param(struct start_scan_ap *scanap)
{
        int socketfd;
		struct iwreq iwr;
		int mode=0,dev_num=0,i;
    	struct wlan_info_t  *wlan_list ;
		static struct  wlan_info_t  wlantable[MSSID_NUMBER],*wlp,*wtp;
		//GET MODE
		mode=get_ap_work_mode();
		if(mode==1){
			dev_num=get_wlan_table(WLAN_CONFIG, WLAN_BEGIN, WLAN_END, &wlan_list); 
		}else if(mode==2){
			dev_num=get_wlan_table(WLAN_11ITEMP_CONFIG, WLAN_BEGIN, WLAN_END, &wlan_list); 
		}
		// modify by piyp 2008-12-17
		bzero(wlantable,sizeof(wlantable));
    	wlp=wlan_list;
    	wtp=wlantable;
    	while(wlp!=NULL){   
    		memcpy(wtp,wlp,sizeof(struct wlan_info_t)); 
    		wlp=wlp->next;
   			wtp++;
    	}
    	free_wlan_table(& wlan_list);
    	wlan_list=wlantable;
		
 		memset(&iwr, 0, sizeof(iwr));
		/*ljy--test start*/
		printf("%s: dev_num %d.\n", __func__, dev_num);
		/*ljy--test end*/
		for(i=1;i<=dev_num;i++){	
			/* ifname为设备名， 如WLAN0, awlan20等等 */	
 			strncpy(iwr.ifr_name, wlan_list->dev_name, IFNAMSIZ);  
			wlan_list++;
			iwr.u.data.pointer =(caddr_t) scanap;
 			iwr.u.data.length = sizeof(*scanap);
        	socketfd = socket(AF_INET, SOCK_DGRAM, 0);

			if (socketfd < 0)
        	{
            	printf(" socket error in set_ioctl_parameter().\n");
            	return -1;
        	}

			/*ljy--test start*/
			printf("%s: socketfd %d.\n", __func__, socketfd);
			/*ljy--test end*/
	    	if (ioctl(socketfd, SIOC_SNMPDAGENT_SCAN_AP, &iwr) < 0)
	    	{
		        printf("ioctl error setioctl_parameter().\n");
			    close(socketfd);
		        return -2;
	    	}
       	
		}
		
        close(socketfd);
        return 1;
}


int
write_wapiAddNeighborhoodApMacAddr(int      action,
								u_char   *var_val,
								u_char   var_val_type,
								size_t   var_val_len,
								u_char   *statP,
								oid      *name,
								size_t   name_len){

    char temp[20];
    int index,value;
	char macAddrName[25],indextemp[5];
	int count=0;
    switch ( action ) {
        case RESERVE1:
          if (var_val_type != ASN_OCTET_STR) {
              fprintf(stderr, "write to wapiPwlanAp not ASN_OCTET_STR\n");
              return SNMP_ERR_WRONGTYPE;
          }
          if (var_val_len > 20 || var_val_len < 17) {
              fprintf(stderr,"write to wapiPwlanAp: bad length\n");
              return SNMP_ERR_WRONGLENGTH;
          }
          
          	break;
        case RESERVE2:
          	break;
        case FREE:
          	break;
        case ACTION:
			memset(temp,0,20);
			strcpy(temp,var_val);
			memset(macAddrName,0,25);
			memset(indextemp,0,5);
			count = get_Mac_All_list();
			char *pos;
			int j=0;
			pos=temp;
			while(((*pos)!='\0') && ((*pos)!='\n')){
				pos++;	
				if((*pos)==':'){
					j++;
				}
			}	
			printf("temp=%s\n",temp);
			printf("j=%d\n",j);
			if(j==5){
				if(count!=0){
					index = count+1;
					if(index<=16){
						strcpy(macAddrName,"NEIGHBORHOOD_AP_MAC_");
						sprintf(indextemp,"%d",index);
						strcat(macAddrName,indextemp);
						save_global_conf(SEP_EQUAL,WIRELESS_CONF,"",macAddrName,temp);
					}
				}else{
					save_global_conf(SEP_EQUAL,WIRELESS_CONF,"","NEIGHBORHOOD_AP_MAC_1",temp);
				}
			}else{
              	return SNMP_ERR_WRONGLENGTH;
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
write_wapiNeighborhoodApMacList(int      action,
				u_char   *var_val,
				u_char   var_val_type,
				size_t   var_val_len,
				u_char   *statP,
				oid      *name,
				size_t   name_len)
{
    char temp[20];
    int index,value;
	char macAddrName[25],indextemp[5];
    switch ( action ) {
        case RESERVE1:
          if (var_val_type != ASN_OCTET_STR) {
              fprintf(stderr, "write to wapiPwlanAp not ASN_OCTET_STR\n");
              return SNMP_ERR_WRONGTYPE;
          }
          if (var_val_len > 20 || var_val_len <17) {
              fprintf(stderr,"write to wapiPwlanAp: bad length\n");
              return SNMP_ERR_WRONGLENGTH;
          }
          
          	break;
        case RESERVE2:
          	break;
        case FREE:
          	break;
        case ACTION:
			memset(temp,0,20);
			strcpy(temp,var_val);								
			index = (int)name[name_len-1];
			printf("index=%d\n",index);
			memset(macAddrName,0,25);
			memset(indextemp,0,5);
			char *pos;
			int j=0;
			pos=temp;
			while(((*pos)!='\0') && ((*pos)!='\n')){
				pos++;	
				if((*pos)==':'){
					j++;
				}
			}	
			//EDIT
			if(j==5){
				if(index<=16){
					strcpy(macAddrName,"NEIGHBORHOOD_AP_MAC_");
					sprintf(indextemp,"%d",index);
					strcat(macAddrName,indextemp);
	            	save_global_conf(SEP_EQUAL,WIRELESS_CONF,"",macAddrName, temp);
				}
			}else{
				return SNMP_ERR_WRONGLENGTH;
			}
          	break;
        case UNDO:
          	break;
        case COMMIT:
          	break;
    }
    return SNMP_ERR_NOERROR;
}

//bottom
