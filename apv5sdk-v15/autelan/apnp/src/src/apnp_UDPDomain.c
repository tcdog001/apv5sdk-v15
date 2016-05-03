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
#include "apnp_log.h"

#include "apnp_UDPDomain.h"




static apnp_tdomain udpDomain;




static int 
apnp_udp_send(apnp_transport *t,void *buf,size_t size,void *opaque,size_t olength)
{
	int ret_udp =0;
	ret_udp = sendto(t->sock,buf,size,0,(struct sockaddr_in*)opaque,olength);
	if(ret_udp < 0){
		ApnpDebugLog("apnp_udp_send: sendto error\n");
	}
	ApnpDebugLog("apnp_udp_send\n");
	return 0;

}



static int 
apnp_udp_recv(apnp_transport *t, void *buf, size_t size,
						void **opaque, size_t *olength)
{
		ApnpDebugLog("apnp_udp_recv\n");
		
		int length;
		if(NULL == t ||  t->sock < 0) {
			ApnpDebugLog( "apnp_udp_recv: transport is %p, sock is %d\n", t, t ? t->sock : -1);
			return -1;
		}
		/*test addr*/
		char paddr_pre[20];
		inet_ntop(AF_INET,&(((struct sockaddr_in *)(t->addr))->sin_addr),paddr_pre,16);
		ApnpDebugLog("apnp_udp_recv :paddr_pre is %s----port is %d\n",paddr_pre,ntohs((((struct sockaddr_in *)(t->addr))->sin_port)));
	
		if(NULL == buf) {
			ApnpDebugLog("apnp_udp_recv: buf is NULL\n");
			return -1;
		}
		struct sockaddr *from_addr = (struct sockaddr  *)malloc(sizeof(struct sockaddr));
		memset(from_addr,0,sizeof(struct sockaddr));
		memcpy((void *)from_addr,t->addr,sizeof(struct sockaddr));
	
		memset(buf,0,size);
		int tmp_olength = 0;
		length = recvfrom(t->sock,buf,size,0,from_addr,&tmp_olength);
		
		
		if(length <= 0){
			ApnpDebugLog("apnp_udp_recv length:%d\n",length);
			//APNP_FREE(from_addr);
			ApnpDebugLog("apnp_udp_recv fail\n");
		}
		else{
			*opaque = (void *)(from_addr);
			*olength = tmp_olength;
			ApnpDebugLog("apnp_udp_recv:%s\n",buf);
		}
		return length;


}




int 
apnp_udp_close(struct apnp_transport_s * t)
{
	int rc = -1;
    if (t != NULL && t->sock >= 0) {
        rc = close(t->sock);
        t->sock = -1;
    }
    return rc;
}


apnp_transport *
apnp_udp_transport(struct sockaddr_in *addr ,int flags)
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


	t->sock = socket(AF_INET,SOCK_DGRAM,0);
	if(t->sock < 0){
		ApnpDebugLog("apnp_udp_transport: socket fail\n");
		return NULL;
	}
	if(setsockopt(t->sock,SOL_SOCKET,SO_REUSEADDR,&fg,len) == -1){
		ApnpDebugLog("SO_REUSEADDR\n");
		apnp_udp_close(t);
		return NULL;
	}
	
	if(setsockopt(t->sock,SOL_SOCKET,SO_SNDBUF,&sndbuf,sizeof(sndbuf)) < 0) {
		ApnpDebugLog("SO_SNDBUF\n");
		apnp_udp_close(t);
		return -1;
	}
	
	if (setsockopt(t->sock,SOL_SOCKET,SO_RCVBUF,&rcvbuf,sizeof(rcvbuf)) < 0) {
		ApnpDebugLog("SO_RCVBUF\n");
		apnp_udp_close(t);
		return -1;
	}

	if (bind(t->sock, (struct sockaddr_in*)addr, sizeof(struct sockaddr_in)) != 0) {
		ApnpDebugLog("Cannot bind udp socket\n");
		apnp_udp_close(t);
		return NULL;
	}


	
	/*test addr*/
	
	t->addr = (void *)addr;
	char paddr[20];
	inet_ntop(AF_INET,(void *) &((struct sockaddr_in *)(t->addr))->sin_addr,paddr,16);
	ApnpDebugLog("apnp_udp_transport addr:%s-----port:%d\n",paddr,ntohs(addr->sin_port));

	t->flags = flags;
	//t->flags |= APNP_FLAGS_UDP_SOCKET;
	//t->addr
	/*
	 * 16-bit length field, 8 byte UDP header, 20 byte IPv4 header  
	 */
	t->msgMaxSize 	= 0x7fffffff; //- 8 - 20;
	t->f_recv		= apnp_udp_recv;
	t->f_send     		= apnp_udp_send;
	t->f_close    		= apnp_udp_close;
	ApnpDebugLog("apnp_udp_transport t:%p\n",t);
	return t;

	
}

static apnp_transport *
apnp_udp_create_chunk(u_long flags)
{
	struct sockaddr_in *ser_addr = (struct sockaddr_in *)malloc(sizeof(struct sockaddr_in));
	bzero(ser_addr,sizeof(ser_addr));
	ApnpDebugLog("sizeof(ser_addr):%d----sizeof(struct sockaddr_in):%d\n",sizeof(ser_addr),sizeof(struct sockaddr_in));
	ser_addr->sin_family = AF_INET;
	ser_addr->sin_port = htons(29900);
	ser_addr->sin_addr.s_addr = htonl(INADDR_ANY);
	//inet_pton(AF_INET,"192.168.13.101",&ser_addr->sin_addr);
	//ser_addr->sin_addr.s_addr = inet_addr("192.168.13.101");

	char paddr[20];
	inet_ntop(AF_INET,(void *) &(ser_addr->sin_addr),paddr,16);
	ApnpDebugLog("apnp_udp_create_chunk :addr is %s-----port:%d\n",paddr,ntohs(ser_addr->sin_port));
	return apnp_udp_transport(ser_addr,flags);


}

void 
apnp_udp_ctor()
{
	udpDomain.name = APNP_UDP_DOMAIN;
	
	udpDomain.f_create_from_chunk = apnp_udp_create_chunk;

	apnp_tdomain_register(&udpDomain);
	
	return ;
}



