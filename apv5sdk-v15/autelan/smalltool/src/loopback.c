#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "server.h"

static int sockfd;


int main(int argc, char **argv)
{
    struct sockaddr_in servaddr;
    struct sockaddr_in cliaddr;
	int new_fd;
    fd_set fd_read;

	if( argc != 3 )
	{
		printf("usage: loopback {ipaddr} {port}");
		exit(1);	
	}

    if((sockfd = socket(AF_INET,SOCK_STREAM,0)) < 0){
		perror("socket is error\n");
		exit(1);
	}
	 
	memset( &servaddr, 0, sizeof(servaddr) );
	servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr(argv[1]);
    servaddr.sin_port = htons(atoi(argv[2]));

	if(bind(sockfd,(struct sockaddr *)&servaddr,sizeof(servaddr)) < 0){
		perror("bind is error\n");
		exit(1);
	}
	if(listen(sockfd,10) == -1){
		perror("listen is error\n");
		exit(1);
	}
    printf("start to listen\n");

	
	while(1){
	   int sin_size = sizeof(struct sockaddr_in);
	   if((new_fd = accept(sockfd,(struct sockaddr *)&cliaddr,&sin_size)) == -1){
	    	perror("accept is error\n");
	    	continue;
	   }
       printf("server:got connection from %s\n",inet_ntoa(cliaddr.sin_addr));

	   char* p;
       char sock_buf[1024];
       memset( &sock_buf, 0, sizeof(sock_buf) );
       p = sock_buf;
       int rval=0;
	   
	   if ((rval = recv(new_fd, p, 1024, 0)) < 0)
	   {
           printf("recv errror\n");
	   }
	   else
	   {
		   printf("recv %s\n",p);
	   }
   
	   if(!fork()){           
		 if(send(new_fd,p,rval,0) == -1)
		 { 
		    perror("send is error\n");
			autelan_close(new_fd);
			exit(0);
		 }
		 printf("send %s\n",p);
	   }
	   autelan_close(new_fd);
	   waitpid(-1,NULL,0);
	}
	
    return 0;
}
