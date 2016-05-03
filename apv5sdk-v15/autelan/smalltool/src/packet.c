/****************************************************************
 *	packet.c
 *	
 *	send a frame frome the interface binded 
 *	the frame is writted manually.
 *	
 *	auther: caosicong 
 *	
 *	time :2007.12.10
 *
 *	modifier: lijiyong 
 *	
 *	time :2009.05.20
 *
 * **************************************************************/
#include <stdio.h>  
#include <fcntl.h>  
#include <errno.h>  
#include <stdlib.h>  
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
#include <signal.h>  
#include <sys/stat.h>  
#include <stdio.h>
#include <ctype.h>
#include <sys/stat.h>

#define IFNAME "ath0"
int main(int argc, char *argv[])
{
	
	struct sockaddr_ll mysocket,to,from;
	int fromlen,tolen;
	long num = 0;
	long pksz = 0;
	int flags = 1;
	int sockd, i,n,sd,on = 1;
	char ifname[12];
	
	if(argc < 4){
		printf("error input\n");
		printf("Usage: packet number size ifname\n");
		return 0;
	}
	
	long times = strtol(argv[1],NULL,10);
	pksz = strtol(argv[2],NULL,10);
	strncpy(ifname,argv[3],12);
#if 0
unsigned char packet[]={
//		0x08,0x02,0x2c,0x00,/*ap->sta*/	
		0x50,0x00,0x2c,0x00,
		0x00,0x19,0xe0,0x81,0x42,0xa0,/*da*/
		0x00,0x03,0x7f,0x07,0xd8,0x5d,/*sa*/
        0x00,0x03,0x7f,0x07,0xd8,0x5d,/*bssid*/
		0x00,0x02,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x19,
		0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0xa0,0x00,0x00,0x00,
		0x00,0x00,0xa0,0x00,0x00,0x00,
		0x00,0x00,0xa0,0x00,0x00,0x00,
		0x00,0x00,0xa0,0x00,0x00,0x00,
		0x00,0x00,0xa0,0x00,0x00,0x00,
		0x00,0x00,0xa0,0x00,0x00,0x00
	};/*this is a 802.11 frame to my wireless interface */
#endif
#if 0
unsigned char packet[60]={
		0x00,0x19,0xe0,0x81,0x42,0xa0,
		0x00,0x1b,0x2f,0x6a,0xf8,0x08,
		0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,
		0x19,0xe0,0x81,0x42,0xa0,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x19,
		0xe0,0x81,0x42,0xa0,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x19,0xe0
	}; 
#endif

	if (pksz < 60){
		printf("the size of packet is too small\n");
		return 1;
	}
	unsigned char *packet = malloc(pksz);
	if (packet == NULL){
		printf("malloc error\n");
		return 1;
	}
	memset(packet, 0xFF, 6);
	memset((packet + 6), 0, pksz-6);
	
	struct ifreq ifr;

	if ((sockd = socket(PF_PACKET,SOCK_RAW,htons(ETH_P_ALL))) < 0){
		printf("socket init error\n");
		return 1;
	}
	printf("via %s\n", ifname);
	strncpy(ifr.ifr_name, ifname, sizeof(ifr.ifr_name));
	if (ioctl(sockd, SIOCGIFINDEX, &ifr) == -1){	//bind to a interface
		printf("SIOCGIFINDEX  failed\n");
		return 1;
	}
#if 0
	ioctl(sockd, FIONBIO, &flags);
#endif
	memset(&mysocket, '\0', sizeof(mysocket));
	mysocket.sll_family = AF_PACKET;
/*ETH_P_HDLC 0x0019 802.11 frame*/
#if 1
	mysocket.sll_protocol = htons(ETH_P_ALL);	
#else
	mysocket.sll_protocol = htons(0x0019);
#endif
	mysocket.sll_ifindex = ifr.ifr_ifindex;

	if (bind(sockd,(struct sockaddr *)&mysocket, sizeof(mysocket)) < 0){
		printf("Bind error\n");
		return 1;
	}
	printf("Bind succecced \n");
		
	while (times > 0){
		n = sendto(sockd, packet, pksz, 0x0, NULL, NULL);
		if (n > 0){
			num++;
			times--;
		}
		else {
			printf("send error! %s maybe down\n", ifname);
			return 1;
		}
	}
	autelan_close(sockd);	
	printf("send %d packets\n",num);
	return 0;
} 
