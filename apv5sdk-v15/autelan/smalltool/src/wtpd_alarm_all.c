


#define DHCP_SNOOPING  	1
#define  USER_REGISTER  10
#define  USER_EXPIRED 11

#include<stdio.h>
#include<stdlib.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#define SERVPORT 3333
#define MAXDATASIZE 1024  /*每次最大数据传输量 */

usage() 


{
 printf("\n Usage: wtpd_alarm_all [lOCALIP] [TYPE] [SUBTYPE] [MAC] [Radio_id] [vapnum] [vlanid] [IP]\n");
 printf("Usage:wtpd_alarm_all [LOCALIP] [TYPE] [0|1]\n");
}



int main(int argc, char **argv){
	
	int i ;
	int flag=0;
	char buf[MAXDATASIZE] ={0};
	int sockfd, recvbytes;
	struct hostent *host;
	struct sockaddr_in serv_addr;
	//buf=malloc( MAXDATASIZE);
	if( argc < 8 && argc != 4 ){
	usage() ;
			return 0;
	} 	
 if(strncmp(argv[2],"1",1) == 0){
	memcpy(buf,  argv[2],1); //type					1 byte 
	memcpy(buf+1,argv[3],2);  //subtype				2 byte	
	memcpy(buf+3,argv[4],strlen(argv[4])); //MAC   12 byte	
	memcpy(buf+15,argv[5],strlen(argv[5])); //radioid 1 byte
	memcpy(buf+16,argv[6],strlen(argv[6])); //wlanid  3 byte 
    memcpy(buf+19,argv[7],strlen(argv[7])); //vlanid  4 byte
    memcpy(buf+23,argv[8],strlen(argv[8])); //IP
    printf("\nwtpd_alarm_all:buf is:    %s",buf);
 }
 else if(strncmp(argv[2],"2",1) == 0){
	memcpy(buf,argv[2],1);
        memcpy(buf+1,argv[3],strlen(argv[3]));
 }
    
 if((host=gethostbyname(argv[1]))==NULL) {

	herror("gethostbyname error");
	exit(1);
}


 if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1){
	perror("socket error");
	exit(1);
}

 serv_addr.sin_family=AF_INET;
 serv_addr.sin_port=htons(SERVPORT);
 serv_addr.sin_addr = *((struct in_addr *)host->h_addr);
 bzero(&(serv_addr.sin_zero),8);
 
       //  if(bind(sockfd,(struct sockaddr *)&serv_addr,sizeof(struct sockaddr_in))<0)
       // {
       // printf("connect");
       //// return 1;
        //}

 
// if (connect(sockfd, (struct sockaddr *)&serv_addr, \
// sizeof(struct sockaddr)) == -1) {

	//perror("connect error");
	//exit(1);
	//}
 	if(sendto(sockfd, buf, strlen(buf), 0,(struct sockaddr *)&serv_addr,sizeof(struct sockaddr_in)) == -1)
 	perror("send error");
 	autelan_close(sockfd);                 
	return 0 ;
}
