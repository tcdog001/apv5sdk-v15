#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <linux/netlink.h> 
#include <linux/rtnetlink.h> 



#include "apnp_util.h"
#include "apnp_ipaddress.h"

#include "apnp_type.h"
#include "apnp_transport.h"

#include "apnp_netlink.h"
#include "apnp_log.h"



static apnp_tdomain netlinkDomain;




static int
apnp_netlink_recv(apnp_transport *t, void *buf, size_t size,
						void **opaque, size_t *olength) {

	

	 ApnpDebugLog("apnp_netlink_recv\n");
	
	struct msghdr n_msg;	
	struct iovec n_iov;
	struct sockaddr_nl *snl;
	struct nlmsghdr *n_nlh = NULL;
	int length;

	if(NULL == t ||  t->sock < 0) {
		 ApnpDebugLog("apnp_netlink_recv: transport is %p, sock is %d\n", t, t ? t->sock : -1);
		return -1;
	}

	if(NULL == buf) {
		 ApnpDebugLog("apnp_netlink_recv: buf is NULL\n");
		return -1;
	}

	if((n_nlh = (struct nlmsghdr*)malloc(NLMSG_SPACE(MAX_PAYLOAD))) == NULL) {
		 ApnpDebugLog(stderr,"Failed malloc\n");
		return -1;
	}

	
	snl =(struct sockaddr_nl *)malloc(sizeof(struct sockaddr_nl));
	
	if(NULL == snl){
		ApnpDebugLog( "apnp_netlink_snl: malloc snl fail\n");
		return -1;
	}

	memset(&n_msg, 0, sizeof(n_msg));
	memset(&n_iov, 0, sizeof(n_iov));	
	memset(n_nlh, 0, NLMSG_SPACE(MAX_PAYLOAD));

	n_iov.iov_base = (void *)n_nlh;
	n_iov.iov_len = NLMSG_SPACE(MAX_PAYLOAD);
	n_msg.msg_name = (void *)snl;
	n_msg.msg_namelen = sizeof(snl);
	n_msg.msg_iov = &n_iov;
	n_msg.msg_iovlen = 1;

	length = recvmsg(t->sock, &n_msg, 0);
	if(length <= 0){
			ApnpDebugLog( "__recv_message: netlink recvmsg fail\n");
			APNP_FREE(snl);
			return -1;
	}			
		
	if (n_msg.msg_namelen != sizeof(struct sockaddr_nl)) {
			ApnpDebugLog( "netlink recv address length error: length %d \n", n_msg.msg_namelen);
			APNP_FREE(snl);
			return -1;
	}
		
	if(NULL == opaque)	{
		ApnpDebugLog("netlink snl is faild\n");
		APNP_FREE(snl);
		return -1;
	}	
	else{
		*opaque = snl;
	  	 memcpy(buf,&n_msg,length);
		 //length = n_iov.iov_len;
	}
	return length;

}

static int
apnp_netlink_send(apnp_transport *t, void *buf, size_t size,
						void **opaque, size_t *olength) {

	ApnpDebugLog("apnp_netlink_send\n");

}

static int
apnp_netlink_close(apnp_transport *t, void *buf, size_t size,
						void **opaque, size_t *olength) {

	ApnpDebugLog("apnp_netlink_close\n");

}



apnp_transport *
apnp_netlink_transport(struct sockaddr_nl *addr, int flags)
{

	socklen_t addr_len;
	apnp_transport *t = NULL;
	int sndbuf = 32768;
	int rcvbuf = 32768;

	if(addr == NULL ) {
        return NULL;
   	}
	
	t = (apnp_transport *) malloc(sizeof(apnp_transport));
	   if (t == NULL) {
		   return NULL;
	   }
	
	memset(t, 0, sizeof(apnp_transport));

	t->sock = socket(AF_NETLINK,SOCK_RAW,NETLINK_ROUTE);
	if(t->sock < 0){
		ApnpDebugLog("apnp_netlink_transport: socket fail\n");
		return NULL;
	}
	if(setsockopt(t->sock,SOL_SOCKET,SO_SNDBUF,&sndbuf,sizeof(sndbuf)) < 0) {
			ApnpDebugLog("SO_SNDBUF");
			return -1;
	}
	
	if (setsockopt(t->sock,SOL_SOCKET,SO_RCVBUF,&rcvbuf,sizeof(rcvbuf)) < 0) {
			ApnpDebugLog("SO_RCVBUF");
			return -1;
	}

	if (bind(t->sock, (struct sockaddr*)addr, sizeof(struct sockaddr_nl)) < 0) {
		ApnpDebugLog("Cannot bind netlink socket");
		return -1;
	}
	addr_len = sizeof(struct sockaddr_nl);
	if (getsockname(t->sock, (struct sockaddr*)addr, &addr_len) < 0) {
		ApnpDebugLog("Cannot getsockname\n");
		return -1;
	}
	if (addr_len != sizeof(struct sockaddr_nl)) {
		ApnpDebugLog("Wrong address length %d\n", addr_len);
		return -1;
	}
	if (addr->nl_family != AF_NETLINK) {
		ApnpDebugLog("Wrong address family %d\n", addr->nl_family);
		return -1;
	}

	t->flags = flags;
	//t->addr
	/*
	 * 16-bit length field, 8 byte UDP header, 20 byte IPv4 header  
	 */
	t->msgMaxSize 	= 0xffff - 8 - 20;
	t->f_recv		= apnp_netlink_recv;	
	t->f_send     		= apnp_netlink_send;
	t->f_close    		= apnp_netlink_close;
	t->f_accept   	= NULL;
	ApnpDebugLog("apnp_netlink_transport t:%p\n",t);
	return t;

}



int
apnp_sockaddr_in2(struct sockaddr_nl *addr,unsigned subscriptions)
{
	if(addr == NULL){
		return 0;
	}
	memset(addr, 0, sizeof(struct sockaddr_nl));
	addr->nl_family = AF_NETLINK;
	addr->nl_groups = subscriptions;

}


static apnp_transport *
apnp_netlink_create_chunk( u_long flags) {
	 struct sockaddr_nl addr;
	 unsigned groups = 0;
	 groups |= RTMGRP_IPV4_IFADDR;

	//if(chunk && sizeof(apnp_netlink_addr) == chunk_size) {
	//	apnp_netlink_addr *addr = (apnp_netlink_addr *)chunk;
		//return apnp_netlink_transport(addr, flags);
	//}
	if (apnp_sockaddr_in2(&addr, groups)) {
        return apnp_netlink_transport(&addr, flags);
    } else {
        return NULL;
    }
	return NULL;
}





void
apnp_netlink_ctor(void)
{
	netlinkDomain.name = APNP_NETLINK_DOMAIN;
	
	netlinkDomain.f_create_from_chunk = apnp_netlink_create_chunk;

	apnp_tdomain_register(&netlinkDomain);
	
	return ;
}
