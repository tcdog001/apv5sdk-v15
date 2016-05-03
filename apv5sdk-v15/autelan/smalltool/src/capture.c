/*
*
* capture.c
*
* gcc   -W   -Wall   -Wno-unused   -ggdb   -o   capture   capture.c 
*
*******************************************************************
*copyright autelan beijing 2008 All rights reserved.
*file name capture.c
*version 1.0
*auther :***
*date : 2007.12
* ****************************************************************
*
*
*
* ************************************************************* */

#include <auteos.h>  
//#include <stdio.h>  
//#include <fcntl.h>  
#include <errno.h>  
//#include <stdlib.h>  
#include <unistd.h>  
#include <string.h>  
#include <net/if.h>  
#include <features.h>  
#include <sys/ioctl.h>  
#include <sys/types.h>  
#include <arpa/inet.h>  
#include <netinet/in.h>  
#include <netinet/ip.h>  
#include <netinet/tcp.h>  
#include <sys/socket.h>  
#include <net/ethernet.h>  
#include <netpacket/packet.h>  

//#include <signal.h>  
//#include <sys/stat.h>  
//#include <stdio.h>
#include <ctype.h>

#define N 30
#define in
#define out
#define RET_OK 1

#define IP_PROTOCOL_TCP	0x06
#define IP_PROTOCOL_UDP 0x11

#define OFFSET_DMAC	0
#define OFFSET_SMAC	6
#define OFFSET_ETHER	12
#define OFFSET_IP_PROT	23
#define OFFSET_SIP	26
#define OFFSET_DIP	30
#define OFFSET_SPORT	34
#define OFFSET_DPORT	36

#define DEFAULT_DEVICE_NAME "ath.0-1" 
/*  #define DEFAULT_RECORD_FILENAME "packet.dat" */ 
   
  typedef unsigned char uchar;   
  int Packet;  
  int PacketCount   =   0;  
/*  char *RecordFileName   =   DEFAULT_RECORD_FILENAME;  */
//  char *DeviceName   =   DEFAULT_DEVICE_NAME;  
  char DeviceName[16]   =   DEFAULT_DEVICE_NAME;
  char PacketType[16] = "";
  int packet_size = 0;

  struct ifreq ifr; 
  struct sockaddr_ll mysocket; 
  /**********from handle***************************************************/
typedef enum __ETHER_TYPE {
	ETHER_IP = 0,   	/* 0x0800 */
	ETHER_ARP,		/* 0x0806 */
	ETHER_PPP,		/* 0x880b */
	ETHER_MPLS_U,		/* 0x8847 */
	ETHER_MPLS_M,		/* 0x8848 */
	ETHER_PPPOE_DIS,	/* 0x8863 */
	ETHER_PPPOE_SESS,	/* 0x8864 */
	ETHER_EAPOL,		/* 0x888e */
	ETHER_LWAPP,		/* 0x88bb */
	ETHER_MAX 		/* 0xffff */
}ETHER_TYPE_T;
  uchar* Ether_Type_Desc[] = { \
	"IP", \
	"ARP", \
	"PPP", \
	"MPLS_UNICAST", \
	"MPLS_MULTICAST", \
	"PPPOE_DISCOVERY", \
	"PPPOE_SESSION", \
	"EAPOL", \
	"LWAPP", \
	"Unknown" \
};

typedef enum __IP_PROT_TYPE {
	IP_PROT_TCP = 0, 	/* 0x06 */
	IP_PROT_UDP,		/* 0x11 */
	IP_PROT_MAX,		/* 0xFF */
}IP_PROT_TYPE_T;

uchar* Ip_Protocol_Desc[] = { \
	"TCP", \
	"UDP", \
	"Unknown" \
};

typedef enum __UDP_PORT_TYPE {
	UDP_PORT_CAPWAP_C = 0,	/*0x8000 or 0x147e*/  /* 0x2fbf */
	UDP_PORT_CAPWAP_D,	/*0x8001 or 0x147f*/         /* 0x2fbe */
	UDP_PORT_DNS,		/* 0x35 */
	UDP_PORT_DHCPS,		/* 0x43 */
	UDP_PORT_DHCPC,		/* 0x44 */	
	UDP_PORT_MAX
}UDP_PORT_TYPE_T;

uchar* Udp_Port_Desc[] = { \
	"capwap control", \
	"capwap data", \
	"dns", \
	"dhcp server", \
	"dhcp client", \
	"Unknown" \
};

typedef enum __TCP_PORT_TYPE {
	TCP_PORT_FTP_C = 0, 	/* 0x15 */
	TCP_PORT_FTP_D,		/* 0x16 */
	TCP_PORT_TELNET,	/* 0x17 */
	TCP_PORT_SMTP,		/* 0x19 */
	TCP_PORT_HTTP,		/* 0x50 */
	TCP_PORT_MAX
}TCP_PORT_TYPE_T;

uchar* Tcp_Port_Desc[] = { \
	"ftp control", \
	"ftp data", \
	"telnet", \
	"smtp", \
	"http", \
	"Unknown" \
};
static char help_msg[] = "\
Usage: capture -i [interface name] -l [min length of capture packet] -t [packet type of capture]\n";
static void iw_usage(void)
{
    autelan_printf("%s", help_msg);
    exit(1);
}
int decap(in void * packet,in int totalBytes)
{
	uchar cur;
	uchar cByte;
	ETHER_TYPE_T eType;
	IP_PROT_TYPE_T iType;
	UDP_PORT_TYPE_T uType;
	TCP_PORT_TYPE_T tType;
	int protocol;
	int number = 0;
	
	autelan_printf("%-12s:","DST MAC");
	while(number<totalBytes)
	{
		cur = ((uchar*) packet)[number];

		if(number<6)
		{	
			autelan_printf("%02x%s", \
					cur, \
					number<5?":":"");
			number++;
		}
		else if(number<12)	
		{
			if(number==6)
				autelan_printf("\n%-12s:","SRC MAC");
			autelan_printf("%02x%s", \
					cur, \
					number<11?":":"");
			number++;
		}
		else if(number==12)
		{
			char tmp =((char*) packet)[number+1];

			if(cur==0x08 && tmp == 0x00) eType = ETHER_IP;
			else if (cur==0x08 && tmp == 0x06) eType = ETHER_ARP;
			else eType = ETHER_MAX;
			number += 2; 
			autelan_printf("\n%-12s:[%s][0x%02x%02x]","ETH TYPE",Ether_Type_Desc[eType],cur,tmp);		
		}
		else
		{
		switch(eType)
		{
		case ETHER_IP:
			if(number==OFFSET_IP_PROT)	
			{
				protocol = (int)cur;
				switch(protocol)
				{
				case 0x06:
					iType = IP_PROT_TCP;
					break;
				case 0x11:
					iType = IP_PROT_UDP;
					break;
				default:
					iType = IP_PROT_MAX;
				}
				autelan_printf("\n%-12s:[%s][%d]"," IP PROT",Ip_Protocol_Desc[iType],protocol);
			}
			else if(number>=OFFSET_SIP && number<OFFSET_DIP)
			{			
				if(number==OFFSET_SIP)
					autelan_printf("\n%-12s:","SRC IP");
				autelan_printf("%d%s", \
					cur,(number<OFFSET_DIP-1)?".":"");
			}
			else if(number>=OFFSET_DIP && number<OFFSET_DIP+4)
			{
				if(number==OFFSET_DIP)
					autelan_printf("\n%-12s:","DST IP");
				autelan_printf("%d%s", \
					cur,(number<OFFSET_DIP+3)?".":"");
			}
			else if(number>=OFFSET_SPORT && number<OFFSET_DPORT)
			{
				switch(iType)
				{
				case IP_PROT_TCP:
					number += 1;
					cByte = ((uchar*)packet)[number];

					if(0x00==cur&&0x15==cByte) tType = TCP_PORT_FTP_C;
					else if(0x00==cur&&0x16==cByte) tType = TCP_PORT_FTP_D;
					else if(0x00==cur&&0x17==cByte) tType = TCP_PORT_TELNET;
					else if(0x00==cur&&0x50==cByte) tType = TCP_PORT_HTTP;
					else tType = TCP_PORT_MAX;

					autelan_printf("\n%-12s:[%s][0x%02x%02x]","SRC PORT", \
						(tType==TCP_PORT_MAX)? "???" : Tcp_Port_Desc[tType],cur,cByte);
					number += 1;
					cur = ((uchar*)packet)[number];
					number += 1;
					cByte = ((uchar*)packet)[number];
					
					if(0x00==cur&&0x15==cByte) tType = TCP_PORT_FTP_C;
					else if(0x00==cur&&0x16==cByte) tType = TCP_PORT_FTP_D;
					else if(0x00==cur&&0x17==cByte) tType = TCP_PORT_TELNET;
					else if(0x00==cur&&0x50==cByte) tType = TCP_PORT_HTTP;
					else tType = TCP_PORT_MAX;
					
					autelan_printf("\n%-12s:[%s][0x%02x%02x]","DST PORT", \
						(tType==TCP_PORT_MAX) ? "???" : Tcp_Port_Desc[tType],cur,cByte);
					break;
				case IP_PROT_UDP:
					number += 1;
					cByte = ((uchar*)packet)[number];

					if((0x80==cur&&0x00==cByte)||(0x14==cur&&0x7e==cByte)) uType = UDP_PORT_CAPWAP_C;
					else if((0x80==cur&&0x01==cByte)||(0x14==cur&&0x7f==cByte)) uType = UDP_PORT_CAPWAP_D;
					else if(0x00==cur&&0x35==cByte) uType = UDP_PORT_DNS;
					else if(0x00==cur&&0x43==cByte) uType = UDP_PORT_DHCPS;
					else if(0x00==cur&&0x44==cByte) uType = UDP_PORT_DHCPC;
					else tType = UDP_PORT_MAX;

					autelan_printf("\n%-12s:[%s][0x%02x%02x]","SRC PORT", \
						uType==UDP_PORT_MAX?"???":Udp_Port_Desc[uType],cur,cByte);
					
					number += 1;
					cur = ((uchar*)packet)[number];
					number += 1;
					cByte = ((uchar*)packet)[number];

					if((0x80==cur&&0x00==cByte)||(0x14==cur&&0x7e==cByte)) uType = UDP_PORT_CAPWAP_C;
					else if((0x80==cur&&0x01==cByte)||(0x14==cur&&0x7f==cByte)) uType = UDP_PORT_CAPWAP_D;
					else if(0x00==cur&&0x35==cByte) uType = UDP_PORT_DNS;
					else if(0x00==cur&&0x43==cByte) uType = UDP_PORT_DHCPS;
					else if(0x00==cur&&0x44==cByte) uType = UDP_PORT_DHCPC;
					else uType = UDP_PORT_MAX;
					
					autelan_printf("\n%-12s:[%s][0x%02x%02x]","DST PORT", \
						uType==UDP_PORT_MAX?"???":Udp_Port_Desc[uType],cur,cByte);
					break;
				default:
					;
				}
			}
			else {}
		break;
	
		case ETHER_ARP:
		break;

		case ETHER_MAX:
		break;

		default:
	 	break;	
		}
			number++;
		}	
	}
	return RET_OK;
}
/*****************************************************************************/  
   
  void ArgInit(   int   argc,   char   **argv   )  
  {  
  }  
   
  void Shutdown(   int   signum   )  
  {  
  	autelan_close(   Packet   );  
  	autelan_fprintf(   stdout,   "Capture   %d   packets\n",   PacketCount   );  
  	exit(   0   );  
  }  
   
  void AppInit(   void   )  
  {  
  	strcpy(   ifr.ifr_name,   DeviceName   );  
  	autelan_signal(   SIGINT,   Shutdown   );  
  	autelan_signal(   SIGTERM,   Shutdown   );  
  } 
   
  void PacketInit(   void   )  
  {  
  	Packet = socket( PF_PACKET,SOCK_RAW,autelan_htons(ETH_P_ALL));  
  	if   (Packet < 0)  
  	{	  
  		autelan_fprintf(   stderr,   "socket   failed   :   [%s]\n",   strerror(errno)   );  
  		exit(   1   );  
  	}  
   
  strncpy(ifr.ifr_name,DeviceName, sizeof(ifr.ifr_name));
	if (ioctl(Packet, SIOCGIFINDEX, &ifr) == -1)//bind to a interface
  	{	
    		autelan_fprintf(stderr, "SIOCGIFINDEX on %s failed: %s\n","lo",strerror(errno));
   		autelan_printf("my error\n");
  	}
  	memset(&mysocket,'\0',sizeof(mysocket));
  	mysocket.sll_family = PF_PACKET;
  	mysocket.sll_protocol =autelan_htons(ETH_P_ALL);
  	mysocket.sll_ifindex = ifr.ifr_ifindex;
  
  	if (autelan_bind(Packet,(struct sockaddr *)&mysocket, sizeof(mysocket)) <0)
  	{
 		 	perror("autelan_bind");
		  
		}  
 }
  void CaptureLoop(   void   )  
  {  
  	int n;  
	int t;
	unsigned short sport = 0;
/*	int i=N;*/
  	unsigned char buf[2048];  
   //	char tempt[]="000000000000";
  	while( 1 )  
  	{  
  		t=0;
  		n = autelan_recvfrom(Packet,buf, sizeof(buf),0,NULL,NULL);  
  		if(n<=0)  
  		{  
  			autelan_fprintf(stderr,"autelan_recvfrom error:[%s]\n",strerror(errno));  
  			break;  
  		}
		//printf("test buf[34-35]:%02x %02x-----------\n", buf[34],buf[35]);
//		if((strstr(DeviceName, "eth")!=0)&&((buf[34]!=0x80)||(buf[35]!=0x01)))
//			continue;
//		if((strstr(DeviceName, "eth")!=0)&&(buf[OFFSET_IP_PROT]!=0x11)) /* filter UDP */
//			continue;
		if(n<=packet_size)
			continue;
		if(0==strcmp(PacketType, "udp"))
		{
			if((n>OFFSET_IP_PROT)&&(buf[OFFSET_IP_PROT]!=0x11))
				continue;
		}
		if(0==strcmp(PacketType, "capwapc"))
		{
			sport = *(unsigned short *)(buf+OFFSET_SPORT);
//			printf("PacketType:%s  sport:0x%02x%02x   sport:%d\n", PacketType, buf[OFFSET_SPORT], buf[OFFSET_SPORT+1], sport);
			if((n>OFFSET_SPORT+1)&&(sport!=0x8000)&&(sport!=0x147e))
				continue;
		}
		if(0==strcmp(PacketType, "capwapd"))
		{
			sport = *(unsigned short *)(buf+OFFSET_SPORT);
			if((n>OFFSET_SPORT+1)&&(sport!=0x8001)&&(sport!=0x147f))
				continue;
		}
	 while((n-t)>=16){
			int i;
			autelan_printf("[");
			for(i=0;i<16;i++)
				autelan_printf("%02x ",buf[t+i]);
			autelan_printf("]\t[");
			for(i=0;i<16;i++)
			{
				char ch=buf[t+i];
				if(isalnum(ch))
					autelan_printf("%c",ch);
				else
					autelan_printf(".");
			}
			autelan_printf("]\n");
			t+=16;
	}

	if(n>t){
		int i=t;
		autelan_printf("[");
		
		while(i<n)
			autelan_printf("%02x ",buf[i++]);
		
		autelan_printf("]");
			
		i=n-t;
		i=16-i;
		while(i--)
		autelan_printf("   ");
		autelan_printf("\t[");
		i=t;
		while(i<n){
			char ch=buf[i++];
			if(isalnum(ch))
			autelan_printf("%c",ch);
			else
			autelan_printf(".");
		}
			autelan_printf("]\n");
	}
/*		strncpy(tempt,buf,6);*/
		decap(buf,n);
		autelan_printf("\n\n");
		PacketCount++;  
		
	}
  		
  	autelan_close(   Packet   );  
}  
   
  int main(int argc, char**argv   )  
 {
//	 int *a=NULL,*b=3;
//	 memcpy(a, b, 4);
#if 0
	 if(argc == 2)
	 {
		memset(DeviceName, 0, sizeof(DeviceName));
		memcpy(DeviceName, argv[1], strlen(argv[1]));
	 }
	else if(argc == 4)
	{
		memset(DeviceName, 0, sizeof(DeviceName));
		memcpy(DeviceName, argv[1], strlen(argv[1]));
		if(0 == memcmp(argv[2], "-l", 2))
		{
			packet_size = atoi(argv[3]);
			autelan_printf("capture packet larger than size:%d\n", packet_size);
		}
	}
#endif
	int opt = 0;
//	int is_back = 0;
//	int res = 0;
//	int i = 0;

	while ((opt = getopt(argc,argv,"i:l:t:")) != -1) {
		switch (opt)
		{
			case 'i':
				memset(DeviceName, 0, sizeof(DeviceName));
				memcpy(DeviceName, optarg, strlen(optarg));
				autelan_printf("capture interface:%s\n", DeviceName);
			break;
			case 'l':
				packet_size = atoi(optarg);
				autelan_printf("capture packet larger than size:%d\n", packet_size);
			break;
			case 't':
				memset(PacketType, 0, sizeof(PacketType));
				memcpy(PacketType, optarg, strlen(optarg));
				autelan_printf("capture packet type:%s\n", PacketType);
			break;
			default: 
			iw_usage();
		}
	}
	if( 0 == strcmp(DeviceName, ""))
	{
		autelan_printf("DeviceName is NULL.\n");
		iw_usage();
	}
	
  	ArgInit(argc,argv);  
  	AppInit();  
  	PacketInit();  
  	CaptureLoop();  
  	return 0;  
 }
