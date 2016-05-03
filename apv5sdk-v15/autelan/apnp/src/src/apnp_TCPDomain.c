#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <linux/netlink.h> 
#include <linux/rtnetlink.h> 



#include "apnp_util.h"
#include "apnp_ipaddress.h"

#include "apnp_type.h"
#include "apnp_transport.h"

#include "apnp_TCPDomain.h"



#define MAX_LISTEN	5

static apnp_tdomain tcpDomain;



static int
apnp_tcp_recv(apnp_transport *t, void *buf, size_t size,
						void **opaque, size_t *olength)
{
	ApnpDebugLog("apnp_tcp_recv\n");
	
	int length;
	if(NULL == t ||  t->sock < 0) {
		ApnpDebugLog("apnp_tcp_recv: transport is %p, sock is %d\n", t, t ? t->sock : -1);
		return -1;
	}
	/*test addr*/
	char paddr_pre[20];
	inet_ntop(AF_INET,&(((struct sockaddr_in *)(t->addr))->sin_addr),paddr_pre,16);
	ApnpDebugLog( "apnp_tcp_recv :paddr_pre is %s----port is %d\n",paddr_pre,ntohs((((struct sockaddr_in *)(t->addr))->sin_port)));

	if(NULL == buf) {
		ApnpDebugLog("apnp_tcp_recv: buf is NULL\n");
		return -1;
	}
	struct sockaddr *from_addr = (struct sockaddr  *)malloc(sizeof(struct sockaddr));
	memset(from_addr,0,sizeof(struct sockaddr));
	memcpy((void *)from_addr,t->addr,sizeof(struct sockaddr));

	memset(buf,0,size);
	
	length = recv(t->sock,buf,size,0);

//	char paddr[20];
//	inet_ntop(AF_INET,&(((struct sockaddr_in *)from_addr)->sin_addr),paddr,16);
//	printf("apnp_udp_recv after recvfrom:%s----port:%d\n",paddr,ntohs(((struct sockaddr_in *)from_addr)->sin_port));

	
	
	if(length <= 0){
		ApnpDebugLog("apnp_tcp_recv length:%d\n",length);
		//APNP_FREE(from_addr);
		ApnpDebugLog("apnp_tcp_recv fail\n");
	}
	else{
		*opaque = (void *)(from_addr);
		*olength = sizeof(struct sockaddr);
		ApnpDebugLog("apnp_tcp_recv:%s\n",buf);
	}
	return length;
}



static int
apnp_tcp_send(struct apnp_transport_s *t, void *buf,
						size_t size, void *opaque, size_t olength )
{
	ApnpDebugLog("apnp_tcp_send\n");
	
	int nsend;
	nsend=send(t->sock,(char *)buf,size,0);
	if(nsend <= 0 ){
		ApnpDebugLog("nsend error\n");
	}
	ApnpDebugLog("send sucess\n");
	return nsend;
}

static int 
apnp_tcp_accept(struct apnp_transport_s * t)
{
	ApnpDebugLog("enter apnp_tcp_accept\n");

	
	int data_len=sizeof(struct sockaddr);
	int acpt_sock = -1;
	int block_flags = 0;

	struct sockaddr *from_addr = (struct sockaddr  *)malloc(sizeof(struct sockaddr));
	memset(from_addr,0,sizeof(struct sockaddr));
	

	if(t->sock >= 0 && t != NULL){
		if((acpt_sock = accept(t->sock,from_addr,&data_len)) < 0){
			ApnpDebugLog("apnp_tcp_accept: accept error\n");
			APNP_FREE(from_addr);
			return acpt_sock;
		}
		/*
		if(t->addr != NULL ){  
			APNP_FREE(from_addr);
		}*/


		/*test addr*/
		char paddr[20];
		inet_ntop(AF_INET,&(((struct sockaddr_in *)from_addr)->sin_addr),paddr,16);
		ApnpDebugLog("apnp_tcp_accept :paddr is %s----port is %d\n",paddr,ntohs(((struct sockaddr_in *)from_addr)->sin_port));

		
			/*set non-block*/
		if((block_flags = fcntl(t->sock,F_GETFL,0)) < 0){
			ApnpDebugLog("apnp_tcp_accept:get fcntl error\n");
		}
		if(fcntl(t->sock,F_SETFL,block_flags|O_NONBLOCK) < 0){
			ApnpDebugLog("apnp_tcp_accept:set fcntl error\n");
		} 
		return acpt_sock;
	}
	else{
		return -1;
	}
}
int 
apnp_tcp_close(struct apnp_transport_s * t)
{
	int rc = -1;
    if (t != NULL && t->sock >= 0) {
        rc = close(t->sock);
        t->sock = -1;
    }
    return rc;
}

apnp_transport *
apnp_tcp_transport(struct sockaddr_in *addr ,int flags)
{
	apnp_transport *t = NULL;
	int sndbuf = 32768;
	int rcvbuf = 32768;
	int block_flags;
	int fg=1;
	int len = sizeof(int);
	
	if(addr == NULL ) {
        return NULL;
   	}
	
	t = (apnp_transport *) malloc(sizeof(apnp_transport));
	if (t == NULL) {
		return NULL;
	}

	memset(t, 0, sizeof(apnp_transport));


	t->sock = socket(AF_INET,SOCK_STREAM,0);
	if(t->sock < 0){
		ApnpDebugLog("apnp_tcp_transport: socket fail\n");
		return NULL;
	}
	if(setsockopt(t->sock,SOL_SOCKET,SO_REUSEADDR,&fg,len) == -1){
		ApnpDebugLog("SO_REUSEADDR");
		apnp_tcp_close(t);
		return NULL;
	}
	
	if(setsockopt(t->sock,SOL_SOCKET,SO_SNDBUF,&sndbuf,sizeof(sndbuf)) < 0) {
		ApnpDebugLog("SO_SNDBUF");
		apnp_tcp_close(t);
		return -1;
	}
	
	if (setsockopt(t->sock,SOL_SOCKET,SO_RCVBUF,&rcvbuf,sizeof(rcvbuf)) < 0) {
		ApnpDebugLog("SO_RCVBUF");
		apnp_tcp_close(t);
		return -1;
	}

	if (bind(t->sock, (struct sockaddr_in*)addr, sizeof(struct sockaddr_in)) != 0) {
		ApnpDebugLog("Cannot bind tcp socket");
		apnp_tcp_close(t);
		return NULL;
	}

	if(listen(t->sock,MAX_LISTEN) == SOCKET_ERROR){
		apnp_tcp_close(t);
		ApnpDebugLog("listen error\n");
		return NULL;
	}
	
	//

	
	/*test addr*/
	
	t->addr = (void *)addr;
	char paddr[20];
	inet_ntop(AF_INET,(void *) &((struct sockaddr_in *)(t->addr))->sin_addr,paddr,16);
	ApnpDebugLog("apnp_tcp_transport addr:%s-----port:%d\n",paddr,ntohs(addr->sin_port));

	t->flags = flags;
	t->flags |= APNP_FLAGS_LISTEN_SOCKET;
	//t->addr
	/*
	 * 16-bit length field, 8 byte UDP header, 20 byte IPv4 header  
	 */
	t->msgMaxSize 	= 0x7fffffff; //- 8 - 20;
	t->f_recv		= apnp_tcp_recv;
	t->f_send     		= apnp_tcp_send;
	t->f_close    		= apnp_tcp_close;
	t->f_accept   	= apnp_tcp_accept;
	ApnpDebugLog("apnp_tcp_transport t:%p\n",t);
	return t;

	
}

static apnp_transport *
apnp_tcp_create_chunk(u_long flags)
{
	struct sockaddr_in *ser_addr = (struct sockaddr_in *)malloc(sizeof(struct sockaddr_in));
	bzero(ser_addr,sizeof(ser_addr));
	ser_addr->sin_family = AF_INET;
	ser_addr->sin_port = htons(10008);
	ser_addr->sin_addr.s_addr = htonl(INADDR_ANY);
	//inet_pton(AF_INET,"192.168.13.101",&ser_addr->sin_addr);
	//ser_addr->sin_addr.s_addr = inet_addr("192.168.13.101");

	char paddr[20];
	inet_ntop(AF_INET,(void *) &(ser_addr->sin_addr),paddr,16);

	ApnpDebugLog("apnp_tcp_create_chunk :addr is %s-----port:%d\n",paddr,ntohs(ser_addr->sin_port));
	return apnp_tcp_transport(ser_addr,flags);


}

void 
apnp_tcp_ctor()
{
	tcpDomain.name = APNP_TCP_DOMAIN;
	
	tcpDomain.f_create_from_chunk = apnp_tcp_create_chunk;

	apnp_tdomain_register(&tcpDomain);
	
	return ;
}
