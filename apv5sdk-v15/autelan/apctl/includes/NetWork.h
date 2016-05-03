#ifndef __NETWORK_H__
#define __NETWORK_H__

/* ------------------------------------------------------------------------------------------------------------------ */
/* include header files */
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netdb.h>

/* ------------------------------------------------------------------------------------------------------------------ */
/* define macro */
#define SERVER_PORT            9008
#define CLIENT_PORT            9007


typedef struct 
{
	pthread_mutex_t  rcv_mutex;
	pthread_cond_t   rcv_cond;
}PTHREAD_SAFE_VARIABLES;
/* ------------------------------------------------------------------------------------------------------------------ */
/* define data type */
typedef struct sockaddr_storage SOCKADDR_STORAGE;
typedef struct sockaddr         SOCKADDR;
typedef struct sockaddr_in      SOCKADDR_IN4;
typedef struct sockaddr_in6     SOCKADDR_IN6;


/* ------------------------------------------------------------------------------------------------------------------ */
/* declare variables */
extern u16  NetWorkFamily;

/* ------------------------------------------------------------------------------------------------------------------ */
/* declare functions */
extern BOOL network_send_unsafe_unconnected(sock_t sock, SOCKADDR_STORAGE *addrPtr, const char *buf, int len);
extern BOOL network_receive_unsafe(sock_t sock, char *buf, int len, int flags, SOCKADDR_STORAGE *addrPtr, int *rcvbytes);
extern BOOL create_socket(sock_t *sock, u16 port ,char * hostip);
extern BOOL get_dst_sockaddr_storage(char *host, SOCKADDR_STORAGE *addrPtr) ;
extern int Check_Interface_State(char *ifname);

extern BOOL get_host_IP(char *hostip);

#endif
