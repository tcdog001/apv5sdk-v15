/*************************************************************************
Copyright (c) 2013-2015, Autelan Networks. All rights reserved.

This Software is the property of Autelan Networks. 
*************************************************************************/
#include "socket_lib_glibc.h"
#include <net/if.h>

int autelan_bind(int socket, struct sockaddr *addr, socklen_t length)
{
    return bind(socket, addr, length);
}

int autelan_getsockname(int socket, struct sockaddr *addr, socklen_t *length_ptr)
{
    return getsockname(socket, addr, length_ptr);
}

unsigned int autelan_if_nametoindex(const char *ifname)
{
    return if_nametoindex(ifname);
}

char * autelan_if_indextoname(unsigned int ifindex, char *ifname)
{
    return if_indextoname(ifindex,ifname);
}

struct if_nameindex * autelan_if_nameindex(void)
{
    return if_nameindex();
}

void autelan_if_freenameindex(struct if_nameindex *ptr)
{
    if_freenameindex(ptr);
	return;
}

int autelan_inet_aton(const char *name, struct in_addr *addr)
{
    return inet_aton(name, addr);
}

unsigned int autelan_inet_addr(const char *name)
{
    return inet_addr(name);
}

unsigned int  autelan_inet_network(const char *name)
{
    return inet_network(name);
}

char * autelan_inet_ntoa(struct in_addr addr)
{
    return inet_ntoa(addr);
}

struct in_addr autelan_inet_makeaddr(unsigned int  net, unsigned int local)
{
    return inet_makeaddr(net, local);
}

unsigned int  autelan_inet_lnaof(struct in_addr addr)
{
    return inet_lnaof(addr);
}

unsigned int  autelan_inet_netof(struct in_addr addr)
{
    return inet_netof(addr);
}

int autelan_inet_pton(int af, const char *cp, void *buf)
{
    return inet_pton(af, cp, buf);
}

const char * autelan_inet_ntop(int af, const void *cp, char *buf, size_t len)
{
    return inet_ntop(af, cp, buf, len);
}

struct hostent * autelan_gethostbyname(const char *name)
{
    return gethostbyname(name);
}

struct hostent * autelan_gethostbyname2(const char *name, int af)
{
    return gethostbyname2(name, af);
}

struct hostent * autelan_gethostbyaddr(const char *addr, size_t length, int format)
{
    return gethostbyaddr(addr, length, format);
}

int autelan_gethostbyname_r(const char *__restrict name, struct hostent *__restrict result_buf, char *__restrict buf, size_t buflen, struct hostent **__restrict result, int *__restrict h_errnop)
{
    return gethostbyname_r(name,result_buf, buf, buflen, result, h_errnop);
}

int autelan_gethostbyname2_r(const char *name, int af, struct hostent *__restrict result_buf, char *__restrict buf, size_t buflen, struct hostent **__restrict result, int *__restrict h_errnop)
{
    return gethostbyname2_r(name, af,result_buf,buf,  buflen,  result, h_errnop);
}

int autelan_gethostbyaddr_r(const char *addr, size_t length, int format, struct hostent *__restrict result_buf, char *__restrict buf, size_t buflen, struct hostent **__restrict result, int *__restrict h_errnop)
{
    return gethostbyaddr_r(addr, length, format, result_buf, buf, buflen, result, h_errnop);
}

void autelan_sethostent(int stayopen)
{
    sethostent(stayopen);
	return;
}

struct hostent * autelan_gethostent(void)
{
    return gethostent();
}

void autelan_endhostent(void)
{
    endhostent();
	return;
}

struct servent * autelan_getservbyname(const char *name, const char *proto)
{
    return getservbyname(name, proto);
}

struct servent * autelan_getservbyport(int port, const char *proto)
{
    return getservbyport(port,proto);
}

void autelan_setservent(int stayopen)
{
    setservent(stayopen);
	return;
}

struct servent * autelan_getservent(void)
{
    return getservent();
}

void autelan_endservent(void)
{
    endservent();
	return;
}

unsigned short autelan_htons(unsigned short hostshort)
{
    return htons(hostshort);
}

unsigned short autelan_ntohs(unsigned short netshort)
{
    return ntohs(netshort);
}

unsigned int  autelan_htonl(unsigned int  hostlong)
{
    return htonl(hostlong);
}

unsigned int  autelan_ntohl(unsigned int  netlong)
{
    return ntohl(netlong);
}

struct protoent * autelan_getprotobyname(const char *name)
{
    return getprotobyname(name);
}

struct protoent * autelan_getprotobynumber(int protocol)
{
    return getprotobynumber(protocol);
}

void autelan_setprotoent(int stayopen)
{
    setprotoent(stayopen);
	return;
}

struct protoent * autelan_getprotoent(void)
{
    return getprotoent();
}

void autelan_endprotoent(void)
{
    endprotoent();
	 return;
}

int autelan_shutdown(int socket, int how)
{
    return shutdown(socket, how);
}

int autelan_connect(int socket, struct sockaddr *addr, socklen_t length)
{
    return connect(socket, addr, length);
}

int autelan_listen(int socket, int n)
{
    return listen(socket,n);
}

int autelan_accept(int socket, struct sockaddr *addr, socklen_t *length_ptr)
{
    return accept(socket, addr, length_ptr);
}

int autelan_getpeername(int socket, struct sockaddr *addr, socklen_t *length_ptr)
{
    return getpeername(socket, addr,length_ptr);
}

int autelan_send(int socket, void *buffer, size_t size, int flags)
{
    return send(socket,buffer,size, flags);
}

int autelan_recv(int socket, void *buffer, size_t size, int flags)
{
    return recv(socket,buffer,size, flags);
}

int autelan_sendto(int socket, void *buffer, size_t size, int flags, struct sockaddr *addr, socklen_t length)
{
    return sendto(socket, buffer, size, flags, addr, length);
}

int autelan_recvfrom(int socket, void *buffer, size_t size, int flags, struct sockaddr *addr, socklen_t *length_ptr)
{
    return recvfrom(socket, buffer, size, flags, addr, length_ptr);
}

int autelan_getsockopt(int socket, int level, int optname, void *optval, socklen_t *optlen_ptr)
{
    return getsockopt(socket, level, optname, optval, optlen_ptr);
}

int autelan_setsockopt(int socket, int level, int optname, void *optval, socklen_t optlen)
{
    return setsockopt(socket, level, optname, optval, optlen);
}

struct netent * autelan_getnetbyname(const char *name)
{
    return getnetbyname(name);
}

struct netent * autelan_getnetbyaddr(unsigned long int net, int type)
{
    return getnetbyaddr(net, type);
}

void autelan_setnetent(int stayopen)
{
    setnetent(stayopen);
	return;
}

struct netent * autelan_getnetent (void)
{
    return getnetent();
}

void autelan_endnetent (void)
{
    endnetent();
	return;
}

