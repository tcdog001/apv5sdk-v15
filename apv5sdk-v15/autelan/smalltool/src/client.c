/************************************************************************
peiwenhui created at 090603.

for fatAP load balance of user on two different AP

************************************************************************/
#include <auteos.h>
//#include <stdio.h>
//#include <stdlib.h>
#include <string.h>
//#include <signal.h>
#include <sys/types.h>
//#include <sys/stat.h>
//#include <fcntl.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <errno.h>
//#include <unistd.h>
 

#define TRANSFER_PORT  9997
#define BUF_LEN     255
#define CMD_LEN    50
#define IP_LEN	   20
#define PARA_PATH "/proc/sys/net/ath0/refuse_join"
#define SERVER_IP_FILE "/jffs/server-ip"
#define INTERFACE "default"

#define ALLOW 1
#define FORBID 2
#define KICK_A_MAC 3


int send_data(int sock, unsigned char b, struct sockaddr_in *addr)
{
	int l = 0;
	char buf[128];

	memset(buf, 0, 128);
	buf[0] = b;
	autelan_printf("========buf:%d\n", b);
	
	l = autelan_sendto(sock, buf, 128, 0, (struct sockaddr*)addr, sizeof(struct sockaddr_in));
	if(0 > l)
	{
		perror("send error:");
		return -1;
	}

	autelan_printf("send %d data, more to be send бн \n", l);

	return l;
}

int getHostIP(char *ipaddr, char *interface)
{
	int                 sockfd;
	struct ifreq        req;

	if ((sockfd = socket(PF_INET, SOCK_DGRAM, 0)) < 0) 
	{
		perror ("socket");
		return -1;
	}

	memset(&req, 0, sizeof(struct ifreq));
//	sprintf(req.ifr_name, "eth%d", interface);
	strncpy(req.ifr_name, interface, strlen(interface));
	autelan_printf("===============interface:%s\n", req.ifr_name);
	// get current ip addr 
	if (ioctl(sockfd, SIOCGIFADDR, (char*)&req)) 
	{
		perror(req.ifr_name);
		autelan_close(sockfd);
		return -1;
	}
	else
	{
		struct in_addr ip_addr;
		char * tmpstr;
		ip_addr.s_addr = *((int*) &req.ifr_addr.sa_data[2]);
		tmpstr = autelan_inet_ntoa(ip_addr);
		strcpy(ipaddr, tmpstr);
	}

	autelan_close(sockfd);

	return 0;
}

void CWCaptrue(int n ,unsigned char *buffer){
	int t=0;
	while((n-t)>=16)
	{
		int i;
		autelan_printf("[");
		for(i=0;i<16;i++)
			autelan_printf("%02x ",buffer[t+i]);
		autelan_printf("]\t[");
		for(i=0;i<16;i++)
		{
			char ch=buffer[t+i];
			if(isalnum(ch))
				autelan_printf("%c",ch);
			else
				autelan_printf(".");
		}
		autelan_printf("]\n");
		t+=16;
	}

	if(n>t)
	{
		int i=t;
		autelan_printf("[");
		while(i<n)
			autelan_printf("%02x ",buffer[i++]);
		autelan_printf("]");
		i=n-t;
		i=16-i;
		while(i--)
			autelan_printf("   ");
		autelan_printf("\t[");
		i=t;
		while(i<n)
		{
			char ch=buffer[i++];
			if(isalnum(ch))
				autelan_printf("%c",ch);
			else
				autelan_printf(".");
		}
		autelan_printf("]\n");
	}
	autelan_printf("\n\n");
}

void set_para_allow(unsigned char value)
{
	char cmdbuf[CMD_LEN];
	
	autelan_sprintf(cmdbuf, "echo %d > %s", value, PARA_PATH);
	autelan_system(cmdbuf);
}

int main(int argc, char **argv)
{
	int fd;
	int size;
	char serverIP[IP_LEN];
	char ownIP[IP_LEN];
	int s;
	int len;
	struct sockaddr_in client_addr;
	int sockfd = -1;
	struct sockaddr_in addr;
	char s_buf[BUF_LEN];
	int ret_no;
	unsigned char cmd;

	memset(serverIP, 0, 20);
	fd = autelan_open (SERVER_IP_FILE, O_RDONLY);
	size = autelan_read(fd, serverIP, sizeof(serverIP));
	autelan_close(fd);
	
	if(size < 0)
		strcpy(serverIP, "192.168.100.1");
	else
		serverIP[size-1] = '\0';

	autelan_printf("serverIP:%s\n", serverIP);

	autelan_printf("init transfer бнбн\n");

	sockfd = socket(AF_INET, SOCK_DGRAM, 0);

	if(-1 == sockfd)
	{
		perror("Failed to create socket");
		return -1;
	}


	len = sizeof(struct sockaddr_in);
	memset(&client_addr, 0, len);
	client_addr.sin_family=AF_INET;
	client_addr.sin_port=autelan_htons(TRANSFER_PORT);
	client_addr.sin_addr.s_addr = autelan_inet_addr(serverIP);
/*	
	s = connect(sockfd, (struct sockaddr *)&client_addr, len);
	if(0 > s)
	{
		perror("connect failed!");
		close(sockfd);
		exit(-1);
	}

	if(getHostIP(ownIP, INTERFACE) < 0)
	{
		perror("get host ip error!");
		close(sockfd);
		exit(-1);
	}
*/
	if(send_data(sockfd, 1, &client_addr) < 0)
	{
		perror("Send register info failed!");
		autelan_close(sockfd);
		exit(-1);
	}
	
	autelan_printf("Enter Cmd: "); 
	while (1) { 
		fd_set readfds; 
		bzero(s_buf, BUF_LEN); 
		FD_ZERO(&readfds); 
		FD_SET(sockfd, &readfds); 
		autelan_select(sockfd + 1, &readfds, NULL, NULL, NULL); 

		if (FD_ISSET(sockfd, &readfds)) { /* receive from remote */ 
			struct sockaddr_in server_addr;
			socklen_t addrLen = sizeof(struct sockaddr_in);
			
			ret_no = autelan_recvfrom(sockfd, s_buf, BUF_LEN, 0, (struct sockaddr *)&server_addr, &addrLen); 
			if (ret_no < 0) { 
				autelan_printf("recv error, code: %d\nAbort!\n", errno); 
				exit(-1); 
			}
			autelan_printf("\n\nserverIP:%x    port:%d\n", server_addr.sin_addr.s_addr, server_addr.sin_port);
			
			autelan_printf("\n-*-*-*-*-*-*-\nReceived: %d\n-*-*-*-*-*-*-\n", s_buf[0]);
		}
		//CWCaptrue(BUF_LEN, s_buf);

		cmd = (unsigned char)s_buf[0];
		switch(cmd)
		{
			case ALLOW:
				set_para_allow(0);
			break;
			case FORBID:
				set_para_allow(1);
			break;
			case KICK_A_MAC:
				autelan_system("/usr/sbin/kicksta");
			break;
			default:
				autelan_printf("unknown cmd type!\n");
			break;
		}
	} 

	autelan_close(sockfd);

	return 0;
}


