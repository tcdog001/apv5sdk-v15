#ifndef INC_SOCKET_H
#define INC_SOCKET_H

#define MSOCKETBUFFER 4096


struct _Socket
{
	char hostname[128];
	unsigned int port_num;
	unsigned int ip_addr;
	int inHandle;
	int outHandle;
	int  sockfd;
	unsigned int sockDisconnect;
	unsigned int sockClose;
	int nbuffer;
	unsigned char buffer[MSOCKETBUFFER];
};

#ifndef LINUX
extern int SocketRead(struct _Socket *pSockInfo, char *buf, int len);

extern int SocketWrite(struct _Socket *pSockInfo, char *buf, int len);
#else
long SocketRead(struct _Socket *pSockInfo, char *buf, long len);
long SocketWrite(struct _Socket *pSockInfo, char *buf, long len);
#endif

extern void SocketClose(struct _Socket *pSockInfo);


extern struct _Socket *SocketConnect(char *pname, unsigned int port);


extern struct _Socket *SocketAccept(struct _Socket *pSockInfo, int noblock);


extern struct _Socket *SocketListen(unsigned int port);

#endif

