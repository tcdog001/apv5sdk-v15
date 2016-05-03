#include "global.h"

extern SOCKADDR_STORAGE dst_addr;

u16  NetWorkFamily;


__inline__ int get_network_addr_struct_size(SOCKADDR_STORAGE *addrPtr) 
{
	if(addrPtr == NULL)
		return 0;
	switch ( ((SOCKADDR *)(addrPtr))->sa_family ) 
	{
		/* IPV6 is defined in Stevens' library */
		case AF_INET6:
			return sizeof(SOCKADDR_IN6);
			break;
		case AF_INET:
		default:
			return sizeof(SOCKADDR_IN4);
	}
}
/* send buf on an unconnected UDP socket. Unsafe means that we don't use DTLS */
BOOL network_send_unsafe_unconnected(sock_t sock, SOCKADDR_STORAGE *addrPtr, const char *buf, int len) 
{
	if((buf == NULL) || (addrPtr == NULL)) 
		return FALSE;

	while(sendto(sock, buf, len, 0, (SOCKADDR *)addrPtr, get_network_addr_struct_size(addrPtr)) < 0) 
	{
		if(errno == EINTR) 
			continue;
		RAISE_SYS_ERR(ERROR_SENDING);
	}
	
	return TRUE;
}
BOOL send_message(BOOL (*assemble_func)(void *arg, TMSG *msg), void *arg1)
{
	TMSG msg;
	msg.len = 0;
	msg.offset = 0;
	msg.msg = NULL;
	if(assemble_func(arg1, &msg) == TRUE)
	{
		//print_packet(msg.msg, msg.offset);
		//printf("0x%08X\n", ntohl(((SOCKADDR_IN4 *)&dst_addr)->sin_addr.s_addr));
		//printf("0x%08X\n", ntohl(((SOCKADDR_IN4 *)&dst_addr)->sin_port));
		if(network_send_unsafe_unconnected(ctl_sd,  &dst_addr, (const char *)msg.msg, msg.offset) == FALSE)
		{
			FREE_PROTOCOL_MESSAGE(msg);//diaowq add@20120929 for memory leak.
			return FALSE;
		}
		FREE_PROTOCOL_MESSAGE(msg);
	}
	else
		return FALSE;
	return TRUE;
}
/* receive a datagram on an unconnected UDP socket (blocking). Unsafe means that we don't use DTLS */
BOOL network_receive_unsafe(sock_t sock, char *buf, int len, int flags, SOCKADDR_STORAGE *addrPtr, int *rcvbytes) 
{
	socklen_t addrLen = sizeof(SOCKADDR_STORAGE);
	
	if((buf == NULL) || (addrPtr == NULL) || (rcvbytes == NULL)) 
		return RAISE_ERROR(ERROR_WRONG_ARG, NULL);
	
	while((*rcvbytes = recvfrom(sock, buf, len, flags, (SOCKADDR *)addrPtr, &addrLen)) < 0) 
	{
		if(errno == EINTR) 
			continue;
		RAISE_SYS_ERR(ERROR_RECEIVING);
	}
	
	return TRUE;
}
BOOL get_host_IP(char *hostip)
{
	char cmd[128] = {0};
	char str[32]  = {0};
	if(hostip == NULL)
		return FALSE;

	memset(cmd, 0, sizeof(cmd));
	memset(str, 0, sizeof(str));
	#if 1
	sprintf(cmd, "ifconfig %s | awk -F \" \" '/inet addr/ {print $2}' | awk -F \":\" '{print $2}'", ifname);
	if(popen_fgets(cmd, str, sizeof(str)) == NULL)
		return FALSE;
	#else
		strcpy(str, "127.0.0.1");
	#endif
	strcpy(hostip, str);
	APVLog("%-16s: %-24s len: %d\n", "HostIP", hostip, (int)strlen(hostip));
	printf_d("%-16s: %-24s len: %d\n", "HostIP", hostip, (int)strlen(hostip));

	return 1;
}

BOOL sock_cpy_addr_port(struct sockaddr *sa1, const struct sockaddr *sa2)
{
	sa1->sa_family = sa2->sa_family;
	switch (sa1->sa_family) 
	{
		case AF_INET: 
		{
			(memcpy( &((SOCKADDR_IN4 *) sa1)->sin_addr,&((SOCKADDR_IN4 *) sa2)->sin_addr,sizeof(struct in_addr)));
			((SOCKADDR_IN4 *)sa1)->sin_port = ((SOCKADDR_IN4 *)sa2)->sin_port;
		}
		break;
		case AF_INET6: 
		{
			memcpy( &((SOCKADDR_IN6 *) sa1)->sin6_addr, &((SOCKADDR_IN6 *) sa2)->sin6_addr, sizeof(struct in6_addr));						   
			((SOCKADDR_IN6 *)sa1)->sin6_port = ((SOCKADDR_IN6 *)sa2)->sin6_port;
		}
		break;
		case AF_UNIX: 
		{
			strcpy( ((struct sockaddr_un *)sa1)->sun_path, ((struct sockaddr_un *)sa2)->sun_path);
		}
		break;
		#if 0
		case AF_LINK: 
		{
			return FALSE;		/* no idea what to copy here ? */
		}
		#endif
		default:
			return FALSE;
		break;
	}
    return TRUE;
}

/* given an host int hte form of C string (e.g. "192.168.1.2" or "localhost"), returns the address */
BOOL get_dst_sockaddr_storage(char *host, SOCKADDR_STORAGE *addrPtr) 
{
	struct addrinfo hints, *res = NULL, *ressave = NULL;
	char serviceName[8] = {0};
	sock_t sock = 0;
	
	if((host == NULL) || (addrPtr == NULL)) 
		return RAISE_ERROR(ERROR_WRONG_ARG, NULL);
	
	bzero(&hints, sizeof(struct addrinfo));
	
	if(NetWorkFamily == IPV6) 
	{
		hints.ai_family = AF_INET6;
		hints.ai_flags = AI_V4MAPPED;
	} 
	else 
	{
		hints.ai_family = AF_INET;
	}
	hints.ai_socktype = SOCK_DGRAM;
	snprintf(serviceName, sizeof(serviceName), "%d", SERVER_PORT); // endianness will be handled by getaddrinfo
	if (getaddrinfo(host, serviceName, &hints, &res) !=0 ) 
	{
		return RAISE_ERROR(ERROR_GENERAL, "Can't resolve hostname");
	}
	
	ressave = res;
	do 
	{
		if((sock = socket(res->ai_family, res->ai_socktype, res->ai_protocol)) < 0) 
		{
			continue;                                  /* try next address */
		}
		
		break;
	} while ((res = res->ai_next) != NULL);
	
	close(sock);
	
	if(res == NULL)   /* error on last iteration */
	{ 
		RAISE_SYS_ERR(ERROR_CREATING);
	}
	
	sock_cpy_addr_port((SOCKADDR *)addrPtr, (res->ai_addr));
	
	freeaddrinfo(ressave);
	
	return TRUE;
}
BOOL create_socket(sock_t *sock, u16 port ,char * hostip)
{
	SOCKADDR_IN4 addr4;
	SOCKADDR_IN6 addr6;
	struct in6_addr in_addr6;
	struct in_addr in_addr4;
	if(NetWorkFamily == IPV6)
	{
		*sock = socket(AF_INET6, SOCK_DGRAM, IPPROTO_UDP);
		if(*sock < 0)
		{
			printf_d("create sock failed!\n");
			return FALSE;
		}
		memset(&addr6, 0, sizeof(SOCKADDR_IN6));
		if(inet_pton(AF_INET6, hostip, &in_addr6) <= 0)
			return FALSE;
		addr6.sin6_family=AF_INET6;
		addr6.sin6_port=htons(port);
		addr6.sin6_addr.s6_addr32[0] = in_addr6.s6_addr32[0]; /* structure assignment */
		addr6.sin6_addr.s6_addr32[1] = in_addr6.s6_addr32[1]; /* structure assignment */
		addr6.sin6_addr.s6_addr32[2] = in_addr6.s6_addr32[2]; /* structure assignment */
		addr6.sin6_addr.s6_addr32[3] = in_addr6.s6_addr32[3]; /* structure assignment */
		if(bind(*sock, (SOCKADDR *)&addr6, sizeof(addr6))==-1)
		{
			APVLog("bind error!%s\n", strerror(errno));
			printf_d("bind error!%s\n", strerror(errno));
			return FALSE;
		}
	}
	else
	{
		*sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
		if(*sock < 0)
		{
			printf_d("create sock failed!\n");
			return FALSE;
		}
		memset(&addr4, 0, sizeof(SOCKADDR_IN4));
		if(inet_pton(AF_INET, hostip, &in_addr4) <= 0)
			return FALSE;
		addr4.sin_family=AF_INET;
		addr4.sin_port=htons(port);
		addr4.sin_addr.s_addr = in_addr4.s_addr; /* structure assignment */
		if(bind(*sock, (SOCKADDR *)&addr4, sizeof(addr4)) == -1)
		{
			APVLog("bind error!%s\n", strerror(errno));
			printf_d("bind error!%s\n", strerror(errno));
			return FALSE;
		}
	}
	return TRUE;
}

int Check_Interface_State(char *ifname)
{
	int sockfd;
	struct ifreq ifr;
	
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	
	strncpy(ifr.ifr_name,ifname, sizeof(ifr.ifr_name));	
	
	if(ioctl(sockfd, SIOCGIFINDEX, &ifr) == -1)
	{//bind to a interface 
		printf_d("%s is not exist !\n", ifname);
		close(sockfd);
		return -1;
	 }

	printf_d("%s is already exist !\n", ifname);
	close(sockfd);
	return 0;
}

