#include <sys/types.h>
#include <inttypes.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <net/if.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <netpacket/packet.h>
#include <net/ethernet.h>
#include <linux/if_ether.h>
#include <linux/filter.h>

#define MAX_VAP 2

//#define _DEBUG_	
#ifdef _DEBUG_
#define DEBUG_OUT(format,arg...)   {	printf(format,##arg);}
#else
#define DEBUG_OUT(format,arg...)	
#endif

#define IGMP_REPORT_V2 0X16
#define IGMP_REPORT_V1 0x12
#define IGMP_LEVEL 0X17

#define ADD_PORT 1
#define DEL_PORT 2

#define CMD_LEN 256

#define IGMP_IFINDEX "/proc/sys/net/br_igmp/if_index"
#define IGMP_GROUPID "/proc/sys/net/br_igmp/mcast_addr"
#define IGMP_OPER "/proc/sys/net/br_igmp/operation"

#define MUCAST_ALL 0x4f000001 // 244.0.0.1
#define MUCAST_LEVEL 0x4f000002 // 244.0.0.2 
#define IGMP_MASK 0x7fffff 

typedef struct igmphdr{
	 __u8 type;
	 __u8 MaxResTime;
	 __u16 checksum;
	 __be32 gaddr;
}igmp_head;

int vap_count = MAX_VAP;
int vap_index[MAX_VAP];
int  set_hw_tab( unsigned group_id, unsigned int ifindex, unsigned opt_type)
{
	int br_opt = 0;
	int ret = 0;
	unsigned char sys_cmd[CMD_LEN];
    switch(opt_type){
			case IGMP_REPORT_V2:
			case IGMP_REPORT_V1:
			{
					printf("add group\n");
					br_opt = ADD_PORT;
					break;
			}
			case IGMP_LEVEL:
			{
				    printf("level group\n");
				    br_opt = DEL_PORT;
				    break;
			}	 
			default:
			{
					printf("unknow opt\n");
					return -1;	
			}
    }	
    
    DEBUG_OUT("group id :%d\n ifindex : %d\n option: %d\n", group_id, ifindex, br_opt);
    sprintf(sys_cmd, "( echo %d > %s && echo %d > %s && echo %d > %s ; )", 
			   group_id, IGMP_GROUPID,
			   ifindex, IGMP_IFINDEX,
	   	       br_opt, IGMP_OPER);
	DEBUG_OUT("system command : %s\n", sys_cmd);
	ret = system(sys_cmd);
    return ret;
}

void main(void)
{
	 int g_sockfd;
	 struct sock_filter BPF_code[] = {
		{ 0x28, 0, 0, 0x0000000c },
		{ 0x15, 0, 3, 0x00000800 },
		{ 0x30, 0, 0, 0x00000017 },
		{ 0x15, 0, 1, 0x00000002 },
		{ 0x6, 0, 0, 0x00000060 },
		{ 0x6, 0, 0, 0x00000000 }
	};	
	
	
	int i = 0;
	char *vap_name[]={
    	{"ath0"},
    	{"ath1"}
    };
	
    fd_set rdfds; 
	struct timeval tv;
	int ret,flag;
    int bytes;
    struct sockaddr_ll from;
	int fromlen;
	
    u_int32_t source,dest;
	u_int16_t check;
	struct iphdr *ip;
	struct igmphdr *igmp;
	
	struct sock_fprog filter;
	filter.len = 6;
	filter.filter = BPF_code;	
	
    unsigned char *packet = (
     char *)malloc(2048*sizeof(char));
	
	if ((g_sockfd=(socket(PF_PACKET, SOCK_RAW, htons(0x003)))) < 0) 
	{
	    printf("Could not open raw socket\n");
	    return;
	}
	if(setsockopt(g_sockfd, SOL_SOCKET, SO_ATTACH_FILTER, &filter, sizeof(filter)) < 0)
	{
		printf("set option filter error\r\n");
		close(g_sockfd);
		return;
	}

    FD_ZERO(&rdfds); 
	FD_SET(g_sockfd, &rdfds);
	tv.tv_sec = 60;
	tv.tv_usec = 0;
	
	struct ifreq ifr;
    int j = 0;
    for(i=0; i<MAX_VAP; i++)
    {
    	 strncpy(ifr.ifr_name,vap_name[i],strlen(vap_name[i])+1);
    	 if((ioctl(g_sockfd,SIOCGIFINDEX,&ifr) == -1))
	     {
		      vap_count--;
	     }
	     DEBUG_OUT("%s 's index is %d\n",vap_name[i], ifr.ifr_ifindex);
	     vap_index[j++] = ifr.ifr_ifindex;
    }
    if(0 == vap_count)
    {
    	printf("error: not valid vap\n");
    	return;
    }
    for(i=0; i< vap_count; i++)
    {
        set_hw_tab( MUCAST_ALL & IGMP_MASK, vap_index[i], IGMP_REPORT_V2 );
    	set_hw_tab( MUCAST_LEVEL & IGMP_MASK, vap_index[i], IGMP_REPORT_V2 );
    }
    
	while(1){
	ret = select(g_sockfd+ 1, &rdfds, NULL, NULL, NULL); 
    if(ret < 0) 
	{
		printf("select error\n");
		return;
	}
	else if(ret == 0) 
	{
		printf("recvfrom time out\n"); 
		return;
	}
	else 
	{
		//printf("select function ret=%d\n", ret);
		if(FD_ISSET(g_sockfd, &rdfds)) 
		{
			memset(&from, 0, sizeof(struct sockaddr_ll));
			fromlen = sizeof(struct sockaddr_ll);
			bytes = recvfrom(g_sockfd,packet,2048, 0,(struct sockaddr *) &from, &fromlen);
			if(bytes<=0) 
			{
				DEBUG_OUT("recvfrom error%s\n\r",strerror(errno));
				DEBUG_OUT("g_sockfd = %d, packet = %d\r\n",g_sockfd,packet);
				return;
			}
			
			flag = 0;
			ip = (struct iphdr *)(packet + sizeof(struct ethhdr));
		    DEBUG_OUT("from index is %d,\n",from.sll_ifindex);
            for(i=0; i<vap_count; i++)
            {
            	DEBUG_OUT("vap index is %d\n",vap_index[i]);
            	if(vap_index[i] == from.sll_ifindex)
            	    flag = 1;	
            }
            DEBUG_OUT("protocol = %d\n",ip->protocol);
            DEBUG_OUT("flag = %d\n", flag);
            if( 2 == ip->protocol  && 1 ==  flag)
            {
			 DEBUG_OUT("the recvfrom interface index is %d\n", from.sll_ifindex);
			 DEBUG_OUT("recv mac %x,%x,%x,%x,%x,%x\r\n",from.sll_addr[0],from.sll_addr[1],from.sll_addr[2],from.sll_addr[3],from.sll_addr[4],from.sll_addr[5]);
			if (bytes < (int) (sizeof(struct ethhdr) + sizeof(struct iphdr) + 8)) {
				printf("message too short, ignoring\n\r");
				free(packet);
				return;
			}
			if (bytes < ntohs(ip->tot_len)) {
				printf("Truncated packet,bytes = %d,tot-len = %d\n\r",bytes, ntohs(ip->tot_len));
				free(packet);
				return;
			}
			
			DEBUG_OUT("size of eth : %d\n",sizeof(struct ethhdr));
			DEBUG_OUT("total len of ip head : %d\n", ip->ihl);
		   igmp = (struct igmphdr *)(packet + sizeof(struct ethhdr) + (ip->ihl)*4);
		   DEBUG_OUT("igmp type: %x\n", igmp->type);
		   unsigned char *test = &(igmp->gaddr);
		   DEBUG_OUT("igmp type: %x.%x.%x.%x\n", test[0],test[1],test[2],test[3]);
		   DEBUG_OUT("igmp group id : %x --- %x\n",igmp->gaddr, igmp->gaddr&0x7fffff);
		   set_hw_tab( (igmp->gaddr&IGMP_MASK), from.sll_ifindex, igmp->type );
		}
		}	
	}
}
}
