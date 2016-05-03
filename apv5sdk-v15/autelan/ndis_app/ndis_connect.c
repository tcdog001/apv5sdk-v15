#include     <stdio.h>      
#include     <stdlib.h>     
#include     <unistd.h>     
#include     <sys/types.h>  
#include     <sys/stat.h>   
#include     <fcntl.h>  
#include     <sys/ioctl.h>
#include     <net/if.h>
#include     <sys/socket.h>

#include     "ndis_qmi_service.h"

#define PC_DEBUG
#ifdef PC_DEBUG
    #define cprintf  printf
#else
    #define cprintf(fmt, args...) do { \
        FILE *fp = fopen("/dev/console", "w"); \
        if (fp) { \
            fprintf(fp, fmt, ## args); \
            fclose(fp); \
        } \
    } while (0)  
#endif
#define CONNECT_TIME_OUT  30    //15 seconds
/********************************************************************
 *
 *         Name:  trim_str
 *  Description:  trim the string with a-z,0-9 A-Z,=,.,/ only
 *        Input:  in_out_str:input string
 *       Output:   in_out_str:trimed string
 *       Return:  0/-1
 *        Notes:  
 ********************************************************************/
static void trim_str(char* in_out_str)
{
    char buff[128]="";
    char* ptr=in_out_str;
    int i=0;
    while(*ptr)
    {    
        if((*ptr>='0'&&*ptr<='9')||(*ptr>='a'&&*ptr<='z')||(*ptr>='A'&&*ptr<='Z')||(*ptr=='_')||(*ptr=='=')||(*ptr=='.')||(*ptr=='/')||(*ptr==','))
            buff[i++]=*ptr;
        ptr++;        
    }
    strcpy(in_out_str,buff);
}

/********************************************************************
 *
 *         Name:  read_ini_config
 *  Description:  read from a ini file the distinct zone distinct 
 *                configure name value
 *        Input:  file_name,ini file name,its example:
 *                ....[conf_zone]
 *                .... conf_name = conf_value
 *       Output:  conf_value
 *       Return:  0/-1
 *        Notes:  
 ********************************************************************/
int read_ini_config(char* file_name,char* conf_zone,char* conf_name, char* conf_value)
{
    FILE* ini_file=fopen(file_name,"r");
    if(ini_file==NULL)
    {
        return -1;
    }
    
    char buff[256];
    int  tag=0;
    while(fgets(buff,sizeof(buff),ini_file))//find zone 
    {
        if(strstr(buff,"[")&&strstr(buff,conf_zone))
        {
            tag=1;
            break;
        }
    }
    if(tag==0)//can not found
    {
        fclose(ini_file);
        return -1;
    }
    while((fgets(buff,sizeof(buff),ini_file)))
    {
        if(strstr(buff,conf_name))
        {
            tag=1;
            break;
        }
    }
    if(tag==0)//can not found
    {
        fclose(ini_file);
        return -1;
    }
    char* ptr = strstr(buff,"=");
    ptr++;
    strcpy(conf_value,ptr);    
    trim_str(conf_value);//remove dirty ascii code
    
    fclose(ini_file);
    return 0;
}
int main()
{
	char version[64] = {0};
	int ret = -1;
	ndis_ipinfo pipinfo;
	int connecting_timeout = CONNECT_TIME_OUT;
	int fd = ndis_open();	
	
	if(fd == -1)
	{
		cprintf("ndis_open failed.\n");
		return -1;
	}
	
	//if get version failed, indicated initliaze error.
	ret = ndis_get_lib_version(fd,version,sizeof(version));
	if(ret!=0||strlen(version)<3)
	{
		cprintf("ndis_get_lib_version failed,ret=%d.\n",ret);	
        ndis_close(fd);
		return -1;		
	}else
	{
		cprintf("ndis_get_lib_version success,version=%s.\n",version);		
	}

	cprintf("ndis connect test! \n");
	// connect to internet
	{
		char apn[128] = "UNINET";
		char usrname[128] = "";
		char pwd[128] = "";
		char authtype[10] = "0";
		FILE* m_fd = fopen("/jffs/profile.ini","r");
		if(m_fd)
		{
			fclose(m_fd);
			read_ini_config("/jffs/profile.ini","profile","apn",apn);
			read_ini_config("/jffs/profile.ini","profile","usrname",usrname);
			read_ini_config("/jffs/profile.ini","profile","pwd",pwd);
			read_ini_config("/jffs/profile.ini","profile","auth",authtype);								
		}
		cprintf("connection use apn:%s,usrname:%s,pwd:%s,auth:%s.\n",apn,usrname,pwd,authtype);	
		ret = ndis_connect(fd,apn,usrname,pwd, atoi(authtype));			
		if(ret!=0)
		{
			cprintf("ndis_connect failed,ret=%d.\n",ret);	
			goto EXIT_PROGRAM;
		}else
		{
			cprintf("ndis_connect success.\n");
		}
		
	}
	
	while(connecting_timeout--)
	{
		ret = ndis_get_status(fd,&pipinfo);
		if(ret!=0)
		{
			cprintf("ndis_get_status failed,ret=%d.\n",ret);					
		}else
		{
			cprintf("ndis_get_status success.\n");
			if(NDIS_CONNECTED==pipinfo.i32status){
			    cprintf("connected to internet success, .\n");
				system("udhcpc -i wan0 -t 3 -b");//to get ip/dns/gatway 
				break;
			}
		}
		usleep(500000);
	}

	if(NDIS_CONNECTING==pipinfo.i32status)
	{
		cprintf("connecting to internet time out,disconnect from internet.\n");	
		ret = ndis_disconnect(fd);
		if(ret!=0)
		{
			cprintf("ndis_disconnect failed,ret=%d.\n",ret);	
            goto EXIT_PROGRAM;		
		}else
		{
			cprintf("ndis_disconnect success.\n");
		}	
		ret = ndis_get_status(fd,&pipinfo);
	}
EXIT_PROGRAM:	
	ndis_close(fd);
	return 0;
}

