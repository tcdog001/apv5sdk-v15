#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/socket.h>

#include "apnp_type.h"
#include "apnp_transport.h"
#include "apnp_log.h"


static apnp_tdomain *domain_list = NULL;


apnp_transport * app_transport_copy(apnp_transport * t)
{
	apnp_transport *n=NULL;
	
	int addr_len = sizeof(struct sockaddr);

	n = (apnp_transport *)malloc(sizeof(apnp_transport));
	if(n == NULL){
		return NULL;
	}
	memset(n, 0, sizeof(apnp_transport));
	if(t->addr != NULL){
		n->addr = malloc(addr_len);/////	
		if(n->addr == NULL){
			apnp_transport_free(n);
			return NULL;
		}
		memcpy(n->addr,t->addr,addr_len);
	}else {
		n->addr = NULL;
	}
    n->msgMaxSize = t->msgMaxSize;
    n->f_accept = t->f_accept;
    n->f_recv = t->f_recv;
    n->f_send = t->f_send;
    n->f_close = t->f_close;
    n->sock = t->sock;
    n->flags = t->flags;	

	return n;
}


static apnp_tdomain *
_find_tdomain(const char *name) {
	apnp_tdomain *d = domain_list;
	for(; d; d = d->next) {
		if(0 == strcmp(d->name, name)) {
			ApnpDebugLog("_find_tdomain: Found domain \"%s\" from name \"%s\"\n", 
													d->name, name);
			return d;
		}		
	}
	ApnpDebugLog("_find_tdomain: Found no domain from name \"%s\"\n", name);
	return NULL;
}

int
apnp_tdomain_register(apnp_tdomain *n) {
	apnp_tdomain **prevNext = &domain_list, *d = domain_list;

	if(NULL != n) {
		for(; d; d = d->next) {
			if(0 == strcmp(n->name, d->name)) {
				/*
				* Already registered.  
				*/
				ApnpDebugLog("apnp_tdomain_register: %s is already registered\n", n->name);
				return -1;
			}
			prevNext = &(d->next);
		}
		n->next = NULL;
		*prevNext = n;
		return 0;
	} 

	ApnpDebugLog("manage_tdomain_register: input para n is NULL\n");
	return -1;
}

void
apnp_transport_free(apnp_transport *t) {
	if (NULL == t) {
		return ;
	}
	
	APNP_FREE(t->addr);
	APNP_FREE(t);

	return ;
}

apnp_transport *
apnp_transport_open(const char *name, u_long flags) {
	ApnpDebugLog("enter apnp_transport_open\n");
	
	if(NULL == name) {
		return NULL;
	}
	
	apnp_tdomain *match = _find_tdomain(name);
	if(NULL != match) {
		apnp_transport *t = match->f_create_from_chunk(flags);
		ApnpDebugLog( "create apnp_transport %p\n", t);
		return t;	
	}
	return NULL;
}



void
apnp_tdomain_init(void){
	apnp_netlink_ctor();
	apnp_tcp_ctor();
	apnp_udp_ctor();

}

