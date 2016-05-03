 
/**********************************************************************************
* Copyright (c) 2008-2011  Beijing Autelan Technology Co. Ltd.
* All rights reserved.
*
* filename: dhcp_snooping_pkt.c
* description:  implementation for  dhcp_snooping packet
* 
*
* 
************************************************************************************/
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

#include "dhcp_snooping_if.h"
#include "dhcp_snooping_options.h"
#include "dhcp_snooping_pkt.h"
#include "dhcp_snooping_tbl.h"
#include "dhcp_snooping_main.h"

int g_sockfd;

extern struct interface *int_list;


u_int16_t checksum(void *addr, int count)
{
	/* Compute Internet Checksum for "count" bytes
	*         beginning at location "addr".
	*/
	register int32_t sum = 0;
	u_int16_t *source = (u_int16_t *) addr;

	while (count > 1)  
	{
		/*  This is the inner loop */
		sum += *source++;
		count -= 2;
	}

	/*  Add left-over byte, if any */
	if (count > 0) 
	{
		/* Make sure that the left-over byte is added correctly both
		* with little and big endian hosts */
		u_int16_t tmp = 0;
		*(unsigned char *) (&tmp) = * (unsigned char *) source;
		sum += tmp;
	}
	/*  Fold 32-bit sum to 16 bits */
	while (sum >> 16)
	{
		sum = (sum & 0xffff) + (sum >> 16);
	}
	return ~sum;
}





/*
 ******************************************************************************
 *  Function   :   UINT32 dhcp_snoop_pkt_initialize(void)
 *  Input      :   void
 *  Output     :   void
 *  Return     :   OK/FAIL
 *  Description:   initialize DHCP Snooping packet receive
 ******************************************************************************
 */
u_int32_t dhcp_snoop_pkt_initialize(void)
{
	struct sock_filter BPF_code[] = {
		{ 0x28, 0, 0, 0x0000000c },
		{ 0x15, 0, 7, 0x000086dd },
		{ 0x30, 0, 0, 0x00000014 },
		{ 0x15, 0, 18, 0x00000011 },
		{ 0x28, 0, 0, 0x00000036 },
		{ 0x35, 0, 1, 0x00000043 },
		{ 0x25, 0, 14, 0x00000044 },
		{ 0x28, 0, 0, 0x00000038 },
		{ 0x35, 11, 13, 0x00000043 },
		{ 0x15, 0, 12, 0x00000800 },
		{ 0x30, 0, 0, 0x00000017 },
		{ 0x15, 0, 10, 0x00000011 },
		{ 0x28, 0, 0, 0x00000014 },
		{ 0x45, 8, 0, 0x00001fff },
		{ 0xb1, 0, 0, 0x0000000e },
		{ 0x48, 0, 0, 0x0000000e },
		{ 0x35, 0, 1, 0x00000043 },
		{ 0x25, 0, 3, 0x00000044 },
		{ 0x48, 0, 0, 0x00000010 },
		{ 0x35, 0, 2, 0x00000043 },
		{ 0x25, 1, 0, 0x00000044 },
		{ 0x6, 0, 0, 0x000005dc },
		{ 0x6, 0, 0, 0x00000000 }
	};

	struct sock_fprog filter;
	
	filter.len = 23;
	filter.filter = BPF_code;	
	if ((g_sockfd=(socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ALL)))) < 0) 
	{
	    printf("Could not open raw socket\n");
	    return DHCP_FAIL;
	}
	if(setsockopt(g_sockfd, SOL_SOCKET, SO_ATTACH_FILTER, &filter, sizeof(filter)) < 0)
	{
		printf("set option filter error\r\n");
		close(g_sockfd);
		return DHCP_FAIL;
	}
	set_all_interfaces_promisc(g_sockfd);
    
    return DHCP_OK;
}


/*
 ******************************************************************************
 *  Function   :   UINT32 dhcp_snoop_pkt_destroy(void)
 *  Input      :   void
 *  Output     :   void
 *  Return     :   OK/FAIL
 *  Description:   destroy DHCP Snooping packet receive
 ******************************************************************************
 */
u_int32_t dhcp_snoop_pkt_destroy(void)
{
    close(g_sockfd);
    return DHCP_OK;
}
/*
 ******************************************************************************
 *  Function   :   UINT32 dhcp_snoop_pkt_destroy(void)
 *  Input      :   void
 *  Output     :   void
 *  Return     :   OK/FAIL
 *  Description:   destroy DHCP Snooping packet receive
 ******************************************************************************
 */
u_int32_t dhcp_snoop_pkt_receive(struct sockaddr_ll *head, unsigned char *buf, int *ifindex)
{
	int bytes;
	struct sockaddr_ll from;
	int fromlen;
	
	unsigned char *packet;
	unsigned char *temppacket;
	struct iphdr *ip;
	struct udphdr *udp;
	struct ethhdr *eth;
	struct dhcpMessage *data;
	
	u_int32_t source,dest;
	u_int16_t check;
	struct iphdr tempIP;

	
	fd_set rdfds; 
	struct timeval tv;
	int ret;
	
	if ((buf == NULL)||(head == NULL)||(ifindex == NULL))
	{
		return DHCP_FAIL;
	}
	packet = buf;
	FD_ZERO(&rdfds); 
	FD_SET(g_sockfd, &rdfds);
	tv.tv_sec = 1;
	tv.tv_usec = 0;
	ret = select(g_sockfd+ 1, &rdfds, NULL, NULL, &tv); 
	if(ret < 0) 
	{
		printf("select error\n");
	}
	else if(ret == 0) 
	{
		//printf("recvfrom time out\n"); 
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
				printf("recvfrom error%s\n\r",strerror(errno));
				printf("g_sockfd = %d, packet = %d\r\n",g_sockfd,packet);
				return DHCP_FAIL;
			}
			//add by liuyanbo
			struct ifreq ifr1;
			ifr1.ifr_ifindex = from.sll_ifindex;
			if((ioctl(g_sockfd,SIOCGIFNAME,&ifr1) == -1))
			{
				printf("Couldn't retrive ifname for the interface index %d\r\n", ifr1.ifr_ifindex);
				return DHCP_FAIL;
			}
			//printf("The interface index %d ifname is ::%s\r\n",ifr1.ifr_ifindex,ifr1.ifr_name);
			//printf("\n");
			if (!strcmp(ifr1.ifr_name,"lo"))
			{
				return DHCP_FAIL;
			}	
			if (!strcmp(ifr1.ifr_name,"default"))
			{
				return DHCP_FAIL;
			}
			if (!strncmp(ifr1.ifr_name,"wifi",4))
			{
				return DHCP_FAIL;
			}
			if (!strncmp(ifr1.ifr_name,"br",2))
			{
				return DHCP_FAIL;
			}
			//
			//printf("the recv buff is %d,active buff is %d\n", sizeof(struct udp_dhcp_packet),bytes);
			//printf("the recvfrom interface index is %d\n", from.sll_ifindex);
			//printf("recv mac %d,%d,%d,%d,%d,%d\r\n",from.sll_addr[0],from.sll_addr[1],from.sll_addr[2],from.sll_addr[3],from.sll_addr[4],from.sll_addr[5]);
			temppacket = malloc(2048);
			memcpy(temppacket, packet, bytes);
			if (bytes < (int) (sizeof(struct ethhdr) + sizeof(struct iphdr) + sizeof(struct udphdr))) {
				printf("message too short, ignoring\n\r");
				free(temppacket);
				return DHCP_FAIL;
			}
			ip = (struct iphdr *)(packet + sizeof(struct ethhdr));
			if (bytes < ntohs(ip->tot_len)) {
				printf("Truncated packet,bytes = %d,tot-len = %d\n\r",bytes, ntohs(ip->tot_len));
				free(temppacket);
				return DHCP_FAIL;
			}
			
			/* ignore any extra garbage bytes */
			bytes = ntohs(ip->tot_len) + sizeof(struct ethhdr);
			
			/* Make sure its the right packet for us, and that it passes sanity checks */
			udp = (struct udphdr *)(packet + sizeof(struct ethhdr) + sizeof(struct iphdr));	
			if (ip->protocol != IPPROTO_UDP || ip->version != IPVERSION ||
				ip->ihl != sizeof(struct iphdr) >> 2 || 
				((udp->dest != htons(CLIENT_PORT))&&(udp->dest != htons(SERVER_PORT))) ||
				bytes > (int) sizeof(struct udp_dhcp_packet) ||
				ntohs(udp->len) != (short) (bytes - sizeof(struct iphdr) - sizeof(struct ethhdr))) 
			{
				printf("protocol %d\n\r", ip->protocol);
				printf("version %d\n\r", ip->version);
				printf("packet.ip.ihl %d\n\r", ip->ihl);
				printf("udp dest %d\n\r", udp->dest);
				printf("bytes %d,sizeof %d\n\r", bytes,sizeof(struct udp_dhcp_packet));
				printf("ntohs(packet.udp.len) %d,sizeof %d\n\r", ntohs(udp->len),bytes - sizeof(struct iphdr) - sizeof(struct ethhdr));
				printf("unrelated/bogus packet\n\r");
				free(temppacket);
				return DHCP_FAIL;
			}
			
			/* check IP checksum */
			check = ip->check;
			ip->check = 0;
#if 1
			if (check != checksum(ip, sizeof(struct iphdr))) {
				printf("bad IP header checksum, ignoring\n\r");
				free(temppacket);
				return DHCP_FAIL;
			}
#endif			
			/* verify the UDP checksum by replacing the header with a psuedo header */
			memcpy(&tempIP, ip, sizeof(struct iphdr));
			source = ip->saddr;
			dest = ip->daddr;
			check = udp->check;
			udp->check = 0;
			memset(ip, 0, sizeof(struct iphdr));
			
			ip->protocol = IPPROTO_UDP;
			ip->saddr = source;
			ip->daddr = dest;
			ip->tot_len = udp->len; /* cheat on the psuedo-header */
#if 1
			if (check && check != checksum(ip, bytes - sizeof(struct ethhdr))) 
			{
				printf("packet with bad UDP checksum received, ignoring\n\r");
				free(temppacket);
				return DHCP_FAIL;
			}
#endif
			memcpy(ip, &tempIP, sizeof(struct iphdr));
			data = (struct dhcpMessage *)((unsigned char *)udp + sizeof(struct udphdr)); 
			if (ntohl(data->cookie) != DHCP_MAGIC) {
				printf("received bogus message (bad magic) -- ignoring\n");
				free(temppacket);
				return DHCP_FAIL;
			}
			
			//printf("oooooh!!! got some!\n");
			*ifindex = from.sll_ifindex;
			memcpy(head, &from, sizeof(struct sockaddr_ll));
			memcpy(buf, temppacket, 590);
			//memcpy(buf, &(packet), bytes);
			printf("\n");
			printf("\n-------dhcp_snoop_pkt_receive:ifindex:%d-------\n", *ifindex);
			free(temppacket);
			return bytes;
		}
	}
	return DHCP_FAIL;	
}

/*
 ******************************************************************************
 *  Function   :   UINT32 dhcp_snoop_pkt_send(void)
 *  Input      :   void
 *  Output     :   void
 *  Return     :   OK/FAIL
 *  Description:   destroy DHCP Snooping packet receive
 ******************************************************************************
 */

u_int32_t dhcp_snoop_pkt_send(struct sockaddr_ll *head, unsigned char *buf, int *ifindex)
{
	int bytes;
	int fromlen;
    	struct interface *ife;
	
	//printf("origen interface is %d\r\n", *ifindex);
	
	for (ife = int_list; ife; ife = ife->next) 
	{
		if (!strcmp(ife->name,"lo"))
		{
			continue;
		}	
		if (!strcmp(ife->name,"default"))
		{
			continue;
		}
		if (!strncmp(ife->name,"wifi",4))
		{
			continue;
		}
		if (!strncmp(ife->name,"br",2))
		{
			continue;
		}
		if (!strncmp(ife->name,"aplocator",9))
		{
			continue;
		}
		
		//printf("the interface %s ifindex is %d\r\n", ife->name, ife->ifindex);
		//printf("the trusted interface ifindex is %d\n",*ifindex);
		if (ife->ifindex == *ifindex)
		{
			head->sll_ifindex = ife->ifindex;
			if(sendto(g_sockfd, buf, sizeof(udp_dhcp_packet_t)-2, 0, (struct sockaddr*)head, sizeof(struct sockaddr_ll)) < 0)
			{
				printf("send packet to interface %s error\n\r", ife->name);
				break;
			}
			printf("send packetto interface %s success\r\n", ife->name);
			break;
		}
		
    }
	return DHCP_OK;	
}

u_int32_t dhcp_snoop_pkt_send_all(struct sockaddr_ll *head, unsigned char *buf, int *ifindex)
{
	int bytes;
	int fromlen;
    	struct interface *ife;
	
	//printf("origen interface is %d\r\n", *ifindex);
	
	for (ife = int_list; ife; ife = ife->next) 
	{
		//printf("the interface %s ifindex is %d\r\n", ife->name, ife->ifindex);
		if (ife->ifindex == *ifindex)
		{
			continue;
		}
		if (!strcmp(ife->name,"lo"))
		{
			continue;
		}	
		if (!strcmp(ife->name,"default"))
		{
			continue;
		}
		if (!strncmp(ife->name,"wifi",4))
		{
			continue;
		}
		if (!strncmp(ife->name,"br",2))
		{
			continue;
		}
		if (!strncmp(ife->name,"aplocator",9))
		{
			continue;
		}
		head->sll_ifindex = ife->ifindex;
		if(sendto(g_sockfd, buf, sizeof(udp_dhcp_packet_t)-2, 0, (struct sockaddr*)head, sizeof(struct sockaddr_ll)) < 0)
		{
			//printf("send packet to interface %s error\n\r", ife->name);
			continue;
		}
		//printf("send packetto interface %s success\r\n", ife->name);
    }
	return DHCP_OK;	
}

#if 0

/*
 ******************************************************************************
 *  Function   :   UINT32 dhcp_snoop_pkt_process(void)
 *  Input      :   void
 *  Output     :   void
 *  Return     :   OK/FAIL
 *  Description:   destroy DHCP Snooping packet receive
 ******************************************************************************
 */
u_int32_t dhcp_snoop_pkt_process()
{
    return DHCP_OK;
}

#endif

/*
 ******************************************************************************
 *  Function   :   UINT32 dhcp_snoop_pkt_initialize(void)
 *  Input      :   void
 *  Output     :   void
 *  Return     :   OK/FAIL
 *  Description:   initialize DHCP Snooping packet receive
 ******************************************************************************
 */
u_int32_t dhcp_snooping_get_item_from_pkt(u_int32_t ifindex, dhcpMessage_t *packet, dhcp_snooping_user_item_t *user)
{

    unsigned char *temp;
	
    if((packet == NULL) || (user == NULL))
    {
        return DHCP_FAIL;
    }
    /* info->bind_type = ;*/
    
    user->vlanId        = 0;
    user->haddr_len     = packet->hlen;

    memcpy(user->chaddr,packet->chaddr,MAX_BOOTP_MAC_LEN);
	printf("\npacket->chaddr:%u",user->chaddr);

    /* 为了支持从RELAY到本地SERVER申请IP地址，同时在本地支持SNOOPING的处理
     * 添加对没有通过用户MAC+VLAN定位到用户接口时从数据包取接口的处理，同时
     * 支持对响应数据包不带接口的处理
     */
    user->ifindex = ifindex;
    if(packet->op == BOOTREPLY)
    {
		user->ifindex = 0;
		printf("*******enter the boot reply sub way\r\n"); 
		printf("\npacket->yiaddr:%u",packet->yiaddr);
		user->ip_addr       = ntohl(packet->yiaddr);		
		if (!(temp = (unsigned char *)get_option(packet, DHCP_LEASE_TIME)))
		{
			return DHCP_FAIL;
			
		} 
		else 
		{
			memcpy(&(user->lease_time), temp, 4);
			user->lease_time = ntohl(user->lease_time);
		}
    }
    else
    {
        user->ip_addr       = 0;
        user->lease_time    = DHCP_SNOOPING_REQUEST_TIMEOUT;
    }
    return DHCP_OK;
}




/*
 ******************************************************************************
 *  Function   :   UINT32 dhcp_snoop_discovery_process(void)
 *  Input      :   void
 *  Output     :   void
 *  Return     :   OK/FAIL
 *  Description:   destroy DHCP Snooping packet receive
 ******************************************************************************
 */
u_int32_t dhcp_snoop_discovery_process(u_int32_t ifindex, dhcpMessage_t *dhcp)
{
	dhcp_snooping_user_item_t user;
	dhcp_snooping_tbl_item_t *item = NULL;
	printf("receive discovery packet\r\n");
    if((dhcp == NULL) || (ifindex == 0))
    {
	printf("error packet pointer and ifindex\r\n");
        return DHCP_FAIL;
    }
	if(dhcp_snooping_get_item_from_pkt(ifindex, dhcp, &user)== DHCP_FAIL)
	{
		printf("get item value from packet error\r\n");		
		return DHCP_FAIL;
	}
	item = (dhcp_snooping_tbl_item_t *)dhcp_snoop_tbl_item_find(&user);
	if (item == NULL)
	{
		user.state = DHCP_SNOOPING_BIND_STATE_REQUEST;
		if (!dhcp_snoop_tbl_item_insert(&user))
		{
			printf("insert item to table error\r\n");			
			return DHCP_FAIL;
		}
	}	
	return DHCP_OK;
}


/*
 ******************************************************************************
 *  Function   :   UINT32 dhcp_snoop_discovery_process(void)
 *  Input      :   void
 *  Output     :   void
 *  Return     :   OK/FAIL
 *  Description:   destroy DHCP Snooping packet receive
 ******************************************************************************
 */
u_int32_t dhcp_snoop_attach_option82(unsigned char* packet)
{
	struct dhcpMessage* dhcp;
	struct iphdr* ip;
	struct iphdr tempip;
	struct udphdr* udp;
	
	u_int32_t source,dest;
	u_int16_t check;
	u_int32_t headlen;
	u_int32_t originlen;
	u_int32_t endposition;

	ip = (struct iphdr *)(packet + sizeof(struct ethhdr));
	udp = (struct udphdr *)(packet + sizeof(struct ethhdr) + sizeof(struct iphdr)); 
	dhcp = (struct dhcpMessage *)(packet + sizeof(struct ethhdr) + sizeof(struct iphdr) + sizeof(struct udphdr));
	
	if( dhcp_snoop_have_circuit_policy()== DHCP_OK )
	{
		add_option_string(dhcp->options, dhcp_snoop_get_circuit_str());
		headlen = sizeof(struct udphdr) + sizeof(struct dhcpMessage) - 308; 
		originlen = ntohs(udp->len);
		endposition = end_option(dhcp->options);
		//printf("endpositon is %d\r\n", endposition);
		//printf("originlen is %d\r\n", originlen);
		//printf("headlen is %d\r\n", headlen);
		if ((headlen + endposition + 1) > originlen)
		{
			ip->tot_len = htons(ip->tot_len + strlen((unsigned char *)dhcp_snoop_get_circuit_str()));
			udp->len = htons(udp->len + strlen((unsigned char *)dhcp_snoop_get_circuit_str()));
		}
		memcpy(&tempip, ip, sizeof(struct iphdr));

		source = ip->saddr;
		dest = ip->daddr;
		check = udp->check;
		udp->check = 0;
		memset(ip, 0, sizeof(struct iphdr));
		
		ip->protocol = IPPROTO_UDP;
		ip->saddr = source;
		ip->daddr = dest;
		ip->tot_len = udp->len; /* cheat on the psuedo-header */
		if (check)			
		{
			udp->check = checksum(ip, ntohs(udp->len)); 
		}		
		memcpy(ip, &tempip, sizeof(struct iphdr));
		ip->check = 0;
		ip->check = checksum(ip, sizeof(struct iphdr));
	}
	return DHCP_OK;
}



/*
 ******************************************************************************
 *  Function   :   UINT32 dhcp_snoop_request_process(void)
 *  Input      :   void
 *  Output     :   void
 *  Return     :   OK/FAIL
 *  Description:   destroy DHCP Snooping packet receive
 ******************************************************************************
 */
u_int32_t dhcp_snoop_request_process(u_int32_t ifindex, dhcpMessage_t *dhcp)
{
	dhcp_snooping_user_item_t user;
	dhcp_snooping_tbl_item_t *item = NULL;
	char char49[49]={0};
	printf("receive request packet\r\n");
    if((dhcp == NULL) || (ifindex == 0))
    {
        return DHCP_FAIL;
    }
	if(dhcp_snooping_get_item_from_pkt(ifindex, dhcp, &user)== DHCP_FAIL)
	{
		return DHCP_FAIL;
	}
	
	item = (dhcp_snooping_tbl_item_t *)dhcp_snoop_tbl_item_find(&user);
	memcpy(char49,user.chaddr,48);
	if (item == NULL)
	{
		user.state = DHCP_SNOOPING_BIND_STATE_REQUEST;
		if (!dhcp_snoop_tbl_item_insert(&user))
		{
			return DHCP_FAIL;
		}
	}	
	return DHCP_OK;
}

/*
 ******************************************************************************
 *  Function   :   UINT32 dhcp_snoop_offer_process(void)
 *  Input      :   void
 *  Output     :   void
 *  Return     :   OK/FAIL
 *  Description:   destroy DHCP Snooping packet receive
 ******************************************************************************
 */
u_int32_t dhcp_snoop_offer_process(u_int32_t ifindex, dhcpMessage_t *dhcp)
{
	printf("receive offer packet\r\n");		
	return DHCP_OK;
}

/*
 ******************************************************************************
 *  Function   :   UINT32 dhcp_snoop_ack_process(void)
 *  Input      :   void
 *  Output     :   void
 *  Return     :   OK/FAIL
 *  Description:   destroy DHCP Snooping packet receive
 ******************************************************************************
 */
u_int32_t dhcp_snoop_ack_process(u_int32_t ifindex, dhcpMessage_t *dhcp)
{
	dhcp_snooping_user_item_t user;
	dhcp_snooping_tbl_item_t *item = NULL;
	
    if((dhcp == NULL) || (ifindex == 0))
    {
        return DHCP_FAIL;
    }
	if(dhcp_snooping_get_item_from_pkt(ifindex, dhcp, &user)== DHCP_FAIL)
	{
		return DHCP_FAIL;
	}
	item = (dhcp_snooping_tbl_item_t *)dhcp_snoop_tbl_item_find(&user);

	if (item == NULL)
	{
		return DHCP_OK; /*from trust to trust interface*/
	}	
	
    if( item->bind_type == DHCP_SNOOPING_BIND_TYPE_STATIC)
    {
        return DHCP_OK;
    }
	user.state = DHCP_SNOOPING_BIND_STATE_BOUND;
	if (dhcp_snoop_tbl_refresh_bind(&user, item) == DHCP_FAIL)
	{
		return DHCP_FAIL;
	}
	return DHCP_OK;
}

/*
 ******************************************************************************
 *  Function   :   UINT32 dhcp_snoop_release_process(void)
 *  Input      :   void
 *  Output     :   void
 *  Return     :   OK/FAIL
 *  Description:   destroy DHCP Snooping packet receive
 ******************************************************************************
 */
u_int32_t dhcp_snoop_release_process(u_int32_t ifindex, dhcpMessage_t *dhcp)
{
	dhcp_snooping_user_item_t user;
	dhcp_snooping_tbl_item_t *item = NULL;

	printf("receive release packet\r\n");
	if((dhcp == NULL) || (ifindex == 0))
	{
		return DHCP_FAIL;
	}
	if(dhcp_snooping_get_item_from_pkt(ifindex, dhcp, &user)== DHCP_FAIL)
	{
		return DHCP_FAIL;
	}
	
	item = (dhcp_snooping_tbl_item_t *)dhcp_snoop_tbl_item_find(&user);
	if (item == NULL)
	{
		return DHCP_OK; /*from trust to trust interface*/
	}	
	
	if( item->bind_type == DHCP_SNOOPING_BIND_TYPE_STATIC)
	{
		return DHCP_OK;
	}
	if(dhcp_snoop_tbl_identity_item(item, &user) == DHCP_FAIL)
	{
		return DHCP_OK;
	}
	if (item->bind_type == DHCP_SNOOPING_BIND_TYPE_DYNAMIC)
	{
		if (dhcp_snoop_tbl_item_delete(item) == DHCP_FAIL)
		{
			return DHCP_FAIL;
		}
	}
	return DHCP_OK;
}




/*
 ******************************************************************************
 *  Function   :   UINT32 dhcp_snoop_nack_process(void)
 *  Input      :   void
 *  Output     :   void
 *  Return     :   OK/FAIL
 *  Description:   destroy DHCP Snooping packet receive
 ******************************************************************************
 */
u_int32_t dhcp_snoop_nack_process(u_int32_t ifindex, dhcpMessage_t *dhcp)
{
	dhcp_snooping_user_item_t user;
	dhcp_snooping_tbl_item_t *item = NULL;


	printf("receive nack packet\r\n");
	if((dhcp == NULL) || (ifindex == 0))
	{
		return DHCP_FAIL;
	}
	if(dhcp_snooping_get_item_from_pkt(ifindex, dhcp, &user)== DHCP_FAIL)
	{
		return DHCP_FAIL;
	}
	
	item = (dhcp_snooping_tbl_item_t *)dhcp_snoop_tbl_item_find(&user);
	if (item == NULL)
	{
		return DHCP_OK; /*from trust to trust interface*/
	}	
	
	if( item->bind_type == DHCP_SNOOPING_BIND_TYPE_STATIC)
	{
		return DHCP_OK;
	}
	if(dhcp_snoop_tbl_identity_item(item, &user) == DHCP_FAIL)
	{
		return DHCP_OK;
	}
     
	if (item->bind_type == DHCP_SNOOPING_BIND_TYPE_DYNAMIC)
	{
		if (dhcp_snoop_tbl_item_delete(item) == DHCP_FAIL)
		{
			return DHCP_FAIL;
		}
	}
	return DHCP_OK;
}


