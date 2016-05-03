/*************************************************************************
Copyright (c) 2013-2015, Autelan Networks. All rights reserved.

This Software is the property of Autelan Networks. 
*************************************************************************/

/*
 * File version info: 
 *
 */
#ifndef __AUTEOS_SOCKET_LIB_H__
#define __AUTEOS_SOCKET_LIB_H__
/*****************************************************************/
/*---------------------------------------------------------*/
/*-------------------Low-Level I/O functions------------------*/
/*---------------------------------------------------------*/

/*
* section 1:include glibc header files
*/
#include "auteos_base.h"
#include "socket_lib_glibc.h"
/*
* section 2:autelan api
*/
extern int autelan_bind (int socket, struct sockaddr *addr, socklen_t length);
extern int autelan_getsockname (int socket, struct sockaddr *addr, socklen_t *length_ptr);
extern unsigned int autelan_if_nametoindex (const char *ifname);
extern char * autelan_if_indextoname (unsigned int ifindex, char *ifname);
extern struct if_nameindex * autelan_if_nameindex (void);
extern void autelan_if_freenameindex (struct if_nameindex *ptr);
extern int autelan_inet_aton (const char *name, struct in_addr *addr);
extern unsigned int  autelan_inet_addr (const char *name);
extern unsigned int  autelan_inet_network (const char *name);
extern char * autelan_inet_ntoa (struct in_addr addr);
extern struct in_addr autelan_inet_makeaddr (unsigned int  net, unsigned int  local);
extern unsigned int  autelan_inet_lnaof (struct in_addr addr);
extern unsigned int  autelan_inet_netof (struct in_addr addr);
extern int autelan_inet_pton (int af, const char *cp, void *buf);
extern const char * autelan_inet_ntop (int af, const void *cp, char *buf, size_t len);
extern struct hostent * autelan_gethostbyname (const char *name);
extern struct hostent * autelan_gethostbyname2 (const char *name, int af);
extern struct hostent * autelan_gethostbyaddr (const char *addr, size_t length, int format);
extern int autelan_gethostbyname_r (const char *__restrict name, struct hostent *__restrict result_buf, char *__restrict buf, size_t buflen, struct hostent **__restrict result, int *__restrict h_errnop);
extern int autelan_gethostbyname2_r (const char *name, int af, struct hostent *__restrict result_buf, char *__restrict buf, size_t buflen, struct hostent **__restrict result, int *__restrict h_errnop);
extern int autelan_gethostbyaddr_r (const char *addr, size_t length, int format, struct hostent *__restrict result_buf, char *__restrict buf, size_t buflen, struct hostent **__restrict result, int *__restrict h_errnop);
extern void autelan_sethostent (int stayopen);
extern struct hostent * autelan_gethostent (void);
extern void autelan_endhostent (void);
extern struct servent * autelan_getservbyname (const char *name, const char *proto);
extern struct servent * autelan_getservbyport (int port, const char *proto);
extern void autelan_setservent (int stayopen);
extern struct servent * autelan_getservent (void);
extern void autelan_endservent (void);
extern uint16_t autelan_htons (uint16_t hostshort);
extern uint16_t autelan_ntohs (uint16_t netshort);
extern unsigned int  autelan_htonl (unsigned int  hostlong);
extern unsigned int  autelan_ntohl (unsigned int  netlong);
extern struct protoent * autelan_getprotobyname (const char *name);
extern struct protoent * autelan_getprotobynumber (int protocol);
extern void autelan_setprotoent (int stayopen);
extern struct protoent * autelan_getprotoent (void);
extern void autelan_endprotoent (void);
extern int autelan_shutdown (int socket, int how);
extern int autelan_connect (int socket, struct sockaddr *addr, socklen_t length);
extern int autelan_listen (int socket, int n);
extern int autelan_accept (int socket, struct sockaddr *addr, socklen_t *length_ptr);
extern int autelan_getpeername (int socket, struct sockaddr *addr, socklen_t *length_ptr);
extern int autelan_send (int socket, void *buffer, size_t size, int flags);
extern int autelan_recv (int socket, void *buffer, size_t size, int flags);
extern int autelan_sendto (int socket, void *buffer, size_t size, int flags, struct sockaddr *addr, socklen_t length);
extern int autelan_recvfrom (int socket, void *buffer, size_t size, int flags, struct sockaddr *addr, socklen_t *length_ptr);
extern int autelan_getsockopt (int socket, int level, int optname, void *optval, socklen_t *optlen_ptr);
extern int autelan_setsockopt (int socket, int level, int optname, void *optval, socklen_t optlen);
extern struct netent * autelan_getnetbyname (const char *name);
extern struct netent * autelan_getnetbyaddr (unsigned long int net, int type);
extern void autelan_setnetent (int stayopen);
extern struct netent * autelan_getnetent (void);
extern void autelan_endnetent (void);


/*
* section 3: Macro define
*/

/*Below are the functions that are forbidden to be called by auteos*/
#define dup autelan_forbidden(dup)

/*Below are the functions that are replaced by auteos*/
#define bind autelan_replaced(bind)
#define getsockname autelan_replaced(getsockname)
#define if_nametoindex autelan_replaced(if_nametoindex)
#define if_indextoname autelan_replaced(if_indextoname)
#define if_nameindex  autelan_replaced(if_nameindex)
#define if_freenameindex autelan_replaced(if_freenameindex)
#define inet_aton autelan_replaced(inet_aton)
#define inet_network autelan_replaced(inet_network)
#define inet_addr autelan_replaced(inet_addr)
#define inet_ntoa autelan_replaced(inet_ntoa)
#define inet_makeaddr autelan_replaced(inet_makeaddr)
#define inet_lnaof autelan_replaced(inet_lnaof)
#define inet_netof autelan_replaced(inet_netof)
#define inet_pton autelan_replaced(inet_pton)
#define inet_ntop autelan_replaced(inet_ntop)
#define gethostbyname autelan_replaced(gethostbyname)
#define gethostbyname2 autelan_replaced(gethostbyname2)
#define gethostbyaddr autelan_replaced(gethostbyaddr)
#define gethostbyname_r autelan_replaced(gethostbyname_r)
#define gethostbyname2_r autelan_replaced(gethostbyname2_r)
#define gethostbyaddr_r autelan_replaced(gethostbyaddr_r)
#define sethostent autelan_replaced(sethostent)
#define gethostent autelan_replaced(gethostent)
#define endhostent autelan_replaced(endhostent)
#define getservbyname autelan_replaced(getservbyname)
#define getservbyport autelan_replaced(getservbyport)
#define setservent autelan_replaced(setservent)
#define getservent autelan_replaced(getservent)
#define endservent autelan_replaced(endservent)
#define htons autelan_replaced(htons)
#define ntohs autelan_replaced(ntohs)
#define htonl autelan_replaced(htonl)
#define ntohl autelan_replaced(ntohl)
#define getprotobyname autelan_replaced(getprotobyname)
#define getprotobynumber autelan_replaced(getprotobynumber)
#define setprotoent autelan_replaced(setprotoent)
#define getprotoent autelan_replaced(getprotoent)
#define endprotoent autelan_replaced(endprotoent)
#define shutdown autelan_replaced(shutdown)
#define socketpair autelan_forbidden(socketpair)
#define connect autelan_replaced(connect)
#define listen autelan_replaced(listen)
#define accept autelan_replaced(accept)
#define getpeername autelan_replaced(getpeername)
#define send autelan_replaced(send)
#define recv autelan_replaced(recv)
#define sendto autelan_replaced(sendto)
#define recvfrom autelan_replaced(recvfrom)
#define getsockopt autelan_replaced(getsockopt)
#define setsockopt autelan_replaced(setsockopt)
#define getnetbyname autelan_replaced(getnetbyname)
#define getnetbyaddr autelan_replaced(getnetbyaddr)
#define setnetent autelan_replaced(setnetent)
#define getnetent autelan_replaced(getnetent)
#define endnetent autelan_replaced(endnetent)
#endif /* __AUTEOS_PIPE_AND_FIFO_GLIBC_H__ */
