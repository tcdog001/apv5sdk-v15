
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <net/if_arp.h>
#include <net/if.h>




char *
ip2str(uint32_t ip, char *str, size_t size)
{
	if (NULL == str) {
		return NULL;
	}

	memset(str, 0, size);
	snprintf(str, size-1, "%u.%u.%u.%u",
		(ip>>24)&0xff, (ip>>16)&0xff, (ip>>8)&0xff, ip&0xff);

	return str;
}

int do_ioctl_get_mac(char *dev,char *mac)
{
	struct ifreq ifreq;
	int fd = 0;

	if (NULL == dev) {
		ApnpDebugLog("do_ioctl_get_mac:dev error\n");
		return 0;
	}
	
	memset(&ifreq, 0, sizeof(ifreq));
	strncpy(ifreq.ifr_name, dev, sizeof(ifreq.ifr_name));
	
	if((fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0){
		ApnpDebugLog("do_ioctl_get_mac:socket error\n");
		return 0;
	}
	if (ioctl(fd, SIOCGIFHWADDR, &ifreq)) {
		ApnpDebugLog("do_ioctl_get_mac:ioctl error\n");
		return 0;
	}
	close(fd);
	snprintf (mac, 18, "%02X:%02X:%02X:%02X:%02X:%02X", 
		(unsigned char) ifreq.ifr_hwaddr.sa_data[0],
		(unsigned char) ifreq.ifr_hwaddr.sa_data[1], 
		(unsigned char) ifreq.ifr_hwaddr.sa_data[2],
		(unsigned char) ifreq.ifr_hwaddr.sa_data[3],
		(unsigned char) ifreq.ifr_hwaddr.sa_data[4],
		(unsigned char) ifreq.ifr_hwaddr.sa_data[5]);

	ApnpDebugLog("do_ioctl_get_mac:%s\n",mac);
	return 1;
}


unsigned long do_ioctl_get_ipaddress(char *dev)
{
	struct ifreq ifr;
	int fd = 0;
	unsigned long ip =0;
	struct in_addr tmp_addr;

	if (NULL == dev) {
		ApnpDebugLog("do_ioctl_get_ipaddress: dev error\n");
		return 0;
	}
	
	memset(&ifr, 0, sizeof(ifr));
	strncpy(ifr.ifr_name, dev, sizeof(ifr.ifr_name));
	
	fd = socket(AF_INET, SOCK_DGRAM, 0);
	if (ioctl(fd, SIOCGIFADDR, &ifr)) {
		ApnpDebugLog("do_ioctl_get_ipaddress:ioctl error\n");
		return 0;
	}
	close(fd);
	memcpy(&ip,ifr.ifr_addr.sa_data + 2, 4);
	tmp_addr.s_addr=ip;
	return htonl(ip);
}


