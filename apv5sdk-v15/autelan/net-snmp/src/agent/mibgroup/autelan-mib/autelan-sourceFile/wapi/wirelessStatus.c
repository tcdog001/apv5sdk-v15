/**********************************************************************************
* Copyright (c) 2008-2011  Beijing Autelan Technology Co. Ltd.
* All rights reserved.
*
* filename: wirelessStatus.c
* description:  implementation for wireless status information.
* 
*
* 
************************************************************************************/

#include <sys/un.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/time.h>

#include "readRadius.h"

#include "ieee80211_ioctl.h"
extern int get_ioctl_parameter( char * ifname , struct ieee80211_stats *s); // "wapiPwlanAp.c"

#define MSG_ARRARY_LEN 2048
#define HOSTAPD_CONF_PATH "/var/run/hostapd/"

static int  init_send_socket(struct sockaddr_un *saddr,char *path)
{
    int sockfd, len;
    sockfd = socket(PF_UNIX, SOCK_DGRAM, 0);
    if (sockfd == -1)
    {
	return -1;
    }
    bzero(saddr,sizeof(struct sockaddr_un));
    saddr->sun_family = AF_UNIX;
    strcpy(saddr->sun_path,path);
    unlink(path);
    len =strlen(saddr->sun_path)+sizeof(saddr->sun_family);
    if(bind(sockfd,(struct sockaddr *)saddr,len)<0)
    {
	return -1;
    }
    return sockfd;
}

static int receive_from_socket(int sockfd, char msg[])
{
    int len,fromlen;
    memset(msg,0,MSG_ARRARY_LEN);
    len =recv(sockfd,msg,MSG_ARRARY_LEN,0);
    if(len <=0) return -1;
    msg[len]=0;
    return len;

}																							
static int send_to_socket(int sockfd, char msg[],struct sockaddr_un *addr)	
{
    int len,len1;

    len =strlen(addr->sun_path)+sizeof(addr->sun_family);
    len1 = sendto(sockfd,msg,strlen(msg),0,(struct sockaddr*)addr,len);
    return len1;
}

static int fd_timed_read(int fd, char *buf,int microseconds)
{
    fd_set readfds;
    struct timeval tv;
    int bytes_read;

    /* First, setup a select() statement to poll for the data comming in */
    FD_ZERO(&readfds);
    FD_SET(fd,&readfds);

    tv.tv_sec = microseconds;
    tv.tv_usec = 0;


    select(fd+1, &readfds, NULL, NULL, &tv);

    if (FD_ISSET(fd,&readfds))
    {
	bytes_read= receive_from_socket(fd, buf);
	return(bytes_read);
    }
    else
    {
    /* No bytes read */
        return 0;
    }
}



static void print_string(void *_str,int len)
{
    int i;
    unsigned char *str = (unsigned char *)_str;

    for(i=0;i<len;i++)
    {
	if((i+1)%16==0) {}  //printf("\n");
    }			
}

static int FindValue( char *strRecv, char *strPara )
{
    int i;
    char *pStart=NULL;
    char *pEnd=NULL;

    if( (pStart=strstr(strRecv,strPara)) != NULL ){
	for( ; *pStart != '\n'; pStart++ )
	{
	    if(*pStart == '=')
	    {
		break;
	    }
	}

	//find start
	if( *pStart == '=' ){

	    pStart++;
	    pEnd = pStart;
	    //find end
	    for( pEnd = pStart; *pEnd != '\n'; pEnd++ )
	    {
	    }
            
	    *pEnd = '\0';

	    strcpy( strPara, pStart );
	    
	    return 0;
	}
    }
    	
    strcpy( strPara, "" );
    return 0;
}


static void GetLine( FILE* fp, char *strLine )
{
    memset( strLine, 0, 256 );
    fgets( strLine, 256, fp);
    if( '\n' == strLine[strlen(strLine)-1] ){
	strLine[strlen(strLine)-1] = '\0';	
    }
}



static int  GetDevPath( char *strPath )
{
    char strLine[256];

    strcpy( strPath, "/var/run/hostapd" ); //defult
    
    FILE *fp = fopen( RADIUS_CONF_FILE , "r" );
    if( fp == NULL )
    {
	return -1;
    }


    while( !feof(fp) )
    {
	GetLine( fp, strLine );
	if( strstr(strLine, "ctrl_interface") != NULL )
	{
	    char *pos = strLine;
	    for( ; *pos!='\0'; pos++ )
	    {
		if( *pos == '=' )
		{
		    break;
		}
	    }
	    if( *pos == '=' )
	    {
		pos++;
		strcpy( strPath, pos ); //ok
	    }
	}
    }

    fclose( fp );
    return 0;
}

#if 0
int GetWirelessStatus( char *strDev, char *strPara )
{
    int fd,counter=0;
    char send_buffer[MSG_ARRARY_LEN];
    char recv_buffer[MSG_ARRARY_LEN];
    struct sockaddr_un addr,server;
    char strPath[256];

    if( strPara[0] == '\0' )
    {
	return 0;
    }

    GetDevPath( strPath );

    bzero(&server, sizeof(struct sockaddr_in));
    server.sun_family = AF_UNIX;
    strcpy(server.sun_path, HOSTAPD_CONF_PATH);
//    strcpy(server.sun_path, strPath);
/*    if( strPath[strlen(strPath)-1] == '/' )
    {
	strPath[strlen(strPath)-1] = '\0'; 
    }
    strcat(server.sun_path, "/"); //zpz
*/
    strcat(server.sun_path, strDev); //zpz

    //if((fd=init_send_socket(&addr,"/var/run/hostapd/wlan2"))>0)
    
    if((fd=init_send_socket(&addr,"/tmp/vv"))>0)

#if 1
//	while(1)
//	{
	    memset(send_buffer,0,MSG_ARRARY_LEN);
//	    printf("%s\n",addr.sun_path);
	    sprintf(send_buffer, "MIB");
//	    strcpy(send_buffer, strPara);
	    send_to_socket(fd, send_buffer, &server);
//	    printf("socket=%d,sender:%s\n", fd, send_buffer);
	    memset(recv_buffer,0,2048);
	    fd_timed_read(fd, recv_buffer, 5);
	    //receive_from_socket(fd, recv_buffer,&addr);
//	    printf("recv:%s\n\n", recv_buffer);
	    FindValue( recv_buffer, strPara ); //zpz
//	    sleep(1);
//	}
#endif	

    return 0;
}
#endif

/*
int main(int argc, char* argv[])
{
    char strPara[256];
    char strDev[100];

    strcpy( strDev, "wlan0" );
if(argc>1)
{
    strcpy( strPara, argv[1] );
}
else
{
    strcpy( strPara, "" );
}
    GetWirelessStatus( strDev, strPara );

    printf( "%s\n", strPara );

    return 0;
}
//*/



long GetWirelessStatus( char *strDev, char *strPara )
{
    struct ieee80211_stats stats;

    memset( &stats, 0, sizeof(stats) );   
    get_ioctl_parameter( strDev, &stats );
    if( strcmp( strPara, "wirelessReqAuthCount") == 0 )
    {
	return stats.is_rx_auth_try;
    }
    else if( strcmp( strPara, "wirelessSucceedAuthCount") == 0 )
    {
	return stats.is_rx_auth_ok;
    }
    else if( strcmp( strPara, "wirelessAssocTrySum") == 0 )
    {
	return stats.is_rx_assoc_try;
    }
    else if( strcmp( strPara, "wirelessAssocSucceedSum") == 0 )
    {
	return stats.is_assocs;
    }
    else if( strcmp( strPara, "wirelessDisassocAPNORec") == 0 )
    {
	return stats.is_rx_ap_too_many_sta;
    }

}
