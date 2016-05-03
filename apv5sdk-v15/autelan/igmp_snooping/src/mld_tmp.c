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
#include <netinet/icmp6.h>



#define MAX_VAP 2

//#define _DEBUG_	
#ifdef _DEBUG_
#define DEBUG_OUT(format,arg...)   {	printf(format,##arg);}
#else
#define DEBUG_OUT(format,arg...)	
#endif

#define MLD_LEVEL 130

#define ICMP6_MEMBERSHIP_QUERY  130
#define ICMP6_MEMBERSHIP_REPORT 131
#define ICMP6_MEMBERSHIP_REDUCTION 132


#define ADD_PORT 1
#define DEL_PORT 2

#define CMD_LEN 256
#define SIZE_OF_IPV6_ADDR 8


#define MLD_IFINDEX "/proc/sys/net/br_igmp/if_index"
#define MLD_GROUPID "/proc/sys/net/br_igmp/mcast_addr"
#define MLD_OPER "/proc/sys/net/br_igmp/operation"

//#define MUCAST_ALL 0x4f000001 // 244.0.0.1
//#define MUCAST_LEVEL 0x4f000002 // 244.0.0.2 

#define MUCAST_ALL 0x4f000001 // 244.0.0.1
#define MUCAST_LEVEL 0x4f000002 // 244.0.0.2 

#define IGMP_MASK 0x7fffff 

#define MLD_MUCAST_ALL 0x00000001 // FF02::1
#define MLD_MUCAST_LEVEL 0x00000002  //FF02::2



/*
 *	IPv6 address structure
 */
struct v6_addr
{
	union 
	{
		unsigned char	u6_addr8[16];
		unsigned short	u6_addr16[8];
		unsigned int	u6_addr32[4];
	} in6_u;
#define s6_addr			in6_u.u6_addr8
#define s6_addr16		in6_u.u6_addr16
#define s6_addr32		in6_u.u6_addr32
};

/*
 *	IPv6 fixed header
 *
 *	BEWARE, it is incorrect. The first 4 bits of flow_lbl
 *	are glued to priority now, forming "class".
 */
struct ipv6hdr {
//#if defined(__LITTLE_ENDIAN_BITFIELD)
//	unsigned char	priority:4,
				//    version:4;
//#elif defined(__BIG_ENDIAN_BITFIELD)
	unsigned char	version:4,
				    priority:4;
//#else
//#error	"Please fix <asm/byteorder.h>"
//#endif
	unsigned char	flow_lbl[3];

	unsigned short	payload_len;
	unsigned char	nexthdr;
	unsigned char	hop_limit;

	struct	v6_addr	saddr;
	struct	v6_addr	daddr;
};

struct mldhdr {
	unsigned char type; /* type of mld message */
	unsigned char code; /* mld code */
	unsigned short cksum; /* IP-style checksum */
	unsigned short maxresp;	/*max response delay*/
	unsigned short reserved;  /*reserved*/
	unsigned short group[SIZE_OF_IPV6_ADDR]; /* IPV6 group address*/
};


int vap_count = MAX_VAP;
int vap_index[MAX_VAP];
int  set_hw_tab(  unsigned int group_id, unsigned int ifindex, unsigned opt_type)
{
	int br_opt = 0;
	int ret = 0;
	unsigned char sys_cmd[CMD_LEN];
    switch(opt_type){
			case ICMP6_MEMBERSHIP_REPORT:
			{
					printf("add group\n");
					br_opt = ADD_PORT;
					break;
			} 
			case ICMP6_MEMBERSHIP_REDUCTION:
			{
				    printf("leave group\n");
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
			   group_id, MLD_GROUPID,
			   ifindex, MLD_IFINDEX,
	   	       br_opt, MLD_OPER);
	DEBUG_OUT("system command : %s\n", sys_cmd);
	ret = system(sys_cmd);
    return ret;
}

void main(void)
{
	 int g_sockfd;


		 struct sock_filter BPF_code[] = {
			 { 0x28, 0, 0, 0x0000000c },
			 { 0x15, 0, 4, 0x000086dd },
			 { 0x30, 0, 0, 0x0000003e },
			 { 0x15, 1, 0, 0x00000083 },
			 { 0x15, 0, 1, 0x00000084 },
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

	unsigned char tmp[130]={0};
    u_int32_t source,dest;
	u_int16_t check;
	struct ipv6hdr *ipv6;
	struct mldhdr *mld;
		  unsigned int  mcast_addr;
	unsigned char *packet = (
	char *)malloc(2048*sizeof(char));
	struct ifreq ifr;

	struct sock_fprog filter;
	filter.len = 7;
	filter.filter = BPF_code;	

	
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
    	DEBUG_OUT("error: not valid vap\n");
    	return;
    }
   for(i=0; i< vap_count; i++)
    {
       set_hw_tab( MLD_MUCAST_ALL , vap_index[i], ICMP6_MEMBERSHIP_REPORT );
    	set_hw_tab( MLD_MUCAST_LEVEL , vap_index[i], ICMP6_MEMBERSHIP_REPORT );
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
			ipv6= (struct ipv6hdr *)(packet + sizeof(struct ethhdr));
		    DEBUG_OUT("from index is %d,\n",from.sll_ifindex);
            for(i=0; i<vap_count; i++)
            {
            	DEBUG_OUT("vap index is %d\n",vap_index[i]);
            	if(vap_index[i] == from.sll_ifindex)
            	    flag = 1;	
            }
			DEBUG_OUT("version = %0x\n",ipv6->version);
			DEBUG_OUT("payload_len = %0x\n",ipv6->payload_len);
            DEBUG_OUT("nexthdr = %0x\n",ipv6->nexthdr);
			DEBUG_OUT("ipv6->hop_limit = %0x\n",ipv6->hop_limit);
            DEBUG_OUT("flag = %d\n", flag);
           if(  1 == ipv6->hop_limit  && 1 ==  flag) // if(  1 == ipv6->hop_limit && 1 ==  flag)
            {
			 DEBUG_OUT("the recvfrom interface index is %d\n", from.sll_ifindex);
			 DEBUG_OUT("recv mac %x,%x,%x,%x,%x,%x\r\n",from.sll_addr[0],from.sll_addr[1],from.sll_addr[2],from.sll_addr[3],from.sll_addr[4],from.sll_addr[5]);
			if (bytes < (int) (sizeof(struct ethhdr) + sizeof(struct ipv6hdr) + 8)) {
				printf("message too short, ignoring\n\r");
				free(packet);
				return;
			}
			
			DEBUG_OUT("size of eth : %d\n",sizeof(struct ethhdr));
		   mld = (struct mldhdr *)(packet + sizeof(struct ethhdr)+sizeof(struct ipv6hdr) + 8);
		   memcpy(&mcast_addr, &mld->group[6], sizeof(unsigned int)); 
		   DEBUG_OUT("mld type: %x\n", mld->type);
		    DEBUG_OUT("mld  mcast_addr:  %d\n",mcast_addr);
		        set_hw_tab( mcast_addr, from.sll_ifindex, mld->type ); // set_hw_tab( mcast_addr, from.sll_ifindex, mld->type );
		}
		}	
	}
}

}
