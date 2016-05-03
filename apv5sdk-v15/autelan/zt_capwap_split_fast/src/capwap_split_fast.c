/* ****************************************************************************************************
 * Filename: capwap_splide_fast.c
 * Description: a module for thin ap capwap packet sent fast in splide mode.
 * Project: autelan ap 2010
 * Author: xmeng
 * Date: 12/03/2008
 ********************************************************************************************************/
#include <linux/ip.h>
#include <linux/udp.h>
#include <linux/inetdevice.h>
#include <asm/unaligned.h>
#include <linux/module.h>
#include <linux/skbuff.h>
#include <linux/netdevice.h>
#include <linux/init.h>
#include <linux/sysctl.h>
#include <linux/proc_fs.h>
#include <linux/route.h>
#include <net/sock.h>
#include <linux/miscdevice.h>
#include <linux/version.h>
//#include "../../os/linux/src/osif_private.h"
#define CAPWAP_HEADER_LEN 8
#define PRISM2_HEADER_LENS 144
#define AC_DATA_PORT 5247 
#define IPPROTO_UDP 17
/*Begin:added by duanmingzhe wmm to ip header 2010-08-28*/
#define MAC_80211_LEN 24
#define QOS_LEN 4
#define SNAP_LEN 6
#define TYPE_LEN 2
#define TOS_LEN 1
#define IP_HEAD 0x0800
#define IPV6_HEAD 0x86dd
#define QOSDATA_80211 0x80
/*End:added by duanmingzhe wmm to ip header 2010-08-28*/

#ifdef CONFIG_MACH_AR7100
typedef __u16 __bitwise __sum16;
#define CSUM_MANGLED_0 ((__force __sum16)0xffff)
#endif
#ifdef CONFIG_MACH_AR7240
typedef __u16 __bitwise __sum16;
#define CSUM_MANGLED_0 ((__force __sum16)0xffff)
#endif
#define MAC_LEN 6
#define MAC_IP_LIST_LEN 20

static int udp_port = 0 ;
static unsigned ac_addr = 0;
static int spfast_down = 0;
static int spfast_up = 0;
static unsigned ip_fragment = 0;
static unsigned capwap_fragment = 0;
static unsigned dest_ac_addr;
static unsigned long long bytes_send = 0;
static unsigned long long bytes_recv = 0;

//liuqiang ZT tunnel 8023
static unsigned long long tunnel8023_bytes_send = 0;
static unsigned long long tunnel8023_bytes_recv = 0;
static int tunnel8023_udp_port = 0;
static int tunnel8023_up = 0;
static int tunnel8023_down = 0;
//static unsigned int zt_id = 0;
//liuqiang ZT tunnel 8023

/*Begin:added by duanmingzhe wmm to ip header 2010-08-28*/
static unsigned long wmm_to_tos= 0;
/*End:added by duanmingzhe wmm to ip header 2010-08-28*/
/*<begin : transplant by caizhibang from apv5*/
/*yanggs add for tunnel mode wds 2012-01-14*/
static int wds_tunnel = 0;
/*yanggs add for tunnel mode wds end 2012-01-14*/
/*end : transplant by caizhibang from apv5>*/
extern int pro_file_sche_flag;
extern int create_flag_file(char *, char *);
extern struct ieee80211vap * get_vap_from_netdev(struct net_device * dev);  //Added by duanmingzhe for get vap from netdev
/*xmeng add for ac roaming eap at splitmac mode*/
unsigned char mac[MAC_LEN];
unsigned int temp_ip_addr;
int mac_ip_flag;
struct mac_ip 
{
	int index;
	int enable;
	unsigned char mac[MAC_LEN];
	unsigned ip_addr;
};

//liuqiang ZT tunnel 8023

/*struct tunnel8023_header
{
	unsigned short w:1; //
	unsigned short f:1; // fragment
	unsigned short : 14; //padding 14 bits
	unsigned int sta_sid; //sta session id
};*/
//liuqiang ZT tunnel 8023


static struct mac_ip mac_ip_list[MAC_IP_LIST_LEN];

static void init_mac_ip_list () 
{	
	int i;
	memset(mac_ip_list, 0x00, sizeof(struct mac_ip)*MAC_IP_LIST_LEN);
	for (i = 0; i < MAC_IP_LIST_LEN; i++) {

		mac_ip_list[i].index = i;		
		
	}

}

int mac_cmp (unsigned char * mac1, unsigned char * mac2)
{
	
	int i = 0;

	for (i = 0; i < MAC_LEN; i++) {
		
		if (mac1[i] != mac2[i]) {
			
			return 1;
		}
	}
	return 0;
}

static int insert_mac_ip () 
{
	int i ;
	for (i = 0; i < MAC_IP_LIST_LEN; i++) {
		
		if (mac_ip_list[i].enable == 0) {
			
			memcpy(mac_ip_list[i].mac , mac, MAC_LEN);
			mac_ip_list[i].ip_addr = temp_ip_addr;
			mac_ip_list[i].enable = 1;
			printk("\n--insert a mac-ip %#X,%#X,%#X,%#X,%#X,%#X,---ip :%#X, index %d\n", mac_ip_list[i].mac[0], mac_ip_list[i].mac[1],mac_ip_list[i].mac[2],mac_ip_list[i].mac[3],mac_ip_list[i].mac[4],mac_ip_list[i].mac[5],mac_ip_list[i].ip_addr, mac_ip_list[i].index);
			return 0;
		}
	}
	return -1;
}

int delete_mac_ip() 
{


	int i ;
	for (i = 0; i < MAC_IP_LIST_LEN; i++) {
		
		if (mac_ip_list[i].enable ==1 && mac_cmp(mac,mac_ip_list[i].mac) == 0) {
			
			mac_ip_list[i].enable = 0;
			printk("\n--delete a mac-ip %#X,%#X,%#X,%#X,%#X,%#X,---ip :%d, index:%d\n", mac_ip_list[i].mac[0], mac_ip_list[i].mac[1],mac_ip_list[i].mac[2],mac_ip_list[i].mac[3],mac_ip_list[i].mac[4], mac_ip_list[i].mac[5],mac_ip_list[i].ip_addr, mac_ip_list[i].index);
			return 0;
		}
		
	}
	return -1;
	
}
unsigned int find_mac_ip ()
{
	int i ;
	for (i = 0; i < MAC_IP_LIST_LEN; i++) {
		
		if (mac_ip_list[i].enable == 1 && mac_cmp(mac,mac_ip_list[i].mac) == 0) {

			return mac_ip_list[i].ip_addr;
		}
	}
	return ac_addr;
	
}  

//extern struct ieee80211vap *vap;
struct ieee80211vap {
	unsigned int vap_splitmac;
};

/*	 lisongbai add for reducing the times of print "can not find device ..."   ***/
static int print_cnt = 0;
static int capwap_splide_fast_recv(struct sk_buff *skb) 
{
	struct ieee80211vap *vap = NULL;
	int size = 0;
	int radioId=0;	 /* pei modified at 0327 */
        unsigned int packet_len = 0;
	unsigned * per = NULL;
	struct iphdr *ip = NULL;
	struct udphdr * udp = NULL; 
	char buffer[10];
	struct net_device *to = NULL;
//	osif_dev  *osifp = NULL;
	//printk("capwap_splide_fast_recv spfast_down : %d\n",vap->vap_spfast_down);
	if (!spfast_down) {
		
		return -1;
	}
	/*<begin : transplant by caizhibang from apv5*/
	/*yanggs add for tunnel mode wds 2012-01-14*/	
	if (wds_tunnel)
	{
		/*chech the skb dev, if it does not come from ath0,we do nothing*/
		if (strncmp(skb->dev->name, "ath", 3) == 0) {
			;

		} else {

			return -1;
		}
	}
	else
	{
	/*chech the skb dev, if it does not come from eth0, we do nothing*/
	if (strncmp(skb->dev->name, "eth", 3) == 0) {
		;

	} else {

		return -1;
	}
	}
	/*yanggs add for tunnel mode wds end 2012-01-14*/
	/*end : transplant by caizhibang from apv5>*/
	/*check this skb is for host or not*/
	if (skb->pkt_type == PACKET_HOST) {
		;
	} else {
		return -1;	
	}

	/*check this skb is udp package or not*/
	
	if (eth_hdr(skb)->h_proto == htons(0x0800)) {
		
		;
	} else {
		return -1;
	}
	#if LINUX_VERSION_CODE <= KERNEL_VERSION(2,6,20)
	ip = skb->nh.iph;
	#else
	ip = skb->network_header; //Modified for thinap
	#endif
	/*check if this packet is udp or others*/
	if (17 == ip->protocol) {

		size += ip->ihl * 4;

		
	} else {
		
		return -1;
	}
    
	udp = (struct udphdr *)((uint8_t *)ip +(ip->ihl << 2));
    
	if (udp->dest == udp_port) {
		size += 8;
	} else {
		return -1;
	}

	/*check if the ip is fragment or not*/
	if (ip->frag_off != 0) {
        
		if ( !(ip->frag_off& 0x4000)) {
			bytes_recv += ( skb->len - 62 );
			ip_fragment ++;
			//printk("the ip package fragment, trun it to ip level \n");
			return  -1;
		} 

	}
	
	/*check if capwap is fregment or not */

	per = (unsigned *)((uint8_t *)skb->data+size);
	if ( (*per) & 0x00000080) {
        bytes_recv += (skb->len - 62);
		capwap_fragment ++;
		//printk("-cap wap fragment, return it to wtpd :: %#X", *per);
		return -1;
	}

	/*check ip header*/
    //skb_len - ip - udp - capwap - 80211 - LLC
    packet_len = skb->len - 62;
    bytes_recv += packet_len;
    
	/* pei add at 0327 for 2 radio */
	per=(unsigned *)((uint8_t *)skb->data+size);
	radioId=((*per)>>14)&0x1f;
	/* end */
	/*compute the capwap header length*/
	size += 8;

	/*get the  wlan NO.*/
	per = (unsigned *)((uint8_t *)skb->data+size);
	memset(buffer, 0x00, 10);
	//sprintf(buffer, "ath%d", (*per)>>24);	
	sprintf(buffer, "ath.%d-%d", radioId, (*per)>>24);	 /* pei modified at 0327 */
	//printk("#########ath.%d-%d##########\n", radioId, (*per)>>24);
#if LINUX_VERSION_CODE <= KERNEL_VERSION(2,6,22)
	to = dev_get_by_name(buffer);
#else
	to = dev_get_by_name(&init_net,buffer);
#endif	
	if (to == NULL) {
		print_cnt++;
/*	 lisongbai add for reducing the times of print "can not find device ..."   ***/
		if (print_cnt == 500) {
			printk("Can not find device %s:: %#X\n", buffer, *per);
			print_cnt = 0;
		}
/*	 lisongbai add for reducing the times of print "can not find device ..."   ***/
		return -1;
	}

	vap = get_vap_from_netdev(to); //Added by duanmingzhe for get vap from netdev

	if(vap->vap_splitmac == 0) {

		dev_put(to);
		return -1;
	}
	
	size += 8;
	
	
	skb_pull(skb, size);
#if LINUX_VERSION_CODE <= KERNEL_VERSION(2,6,20)
	skb->nh.raw = skb->data;
#else
	skb->network_header= skb->data;
#endif
	skb->dev = to;

	skb->protocol = htons(0x0019);

	dev_queue_xmit(skb);


	if (to != NULL)
		dev_put(to);

	return 0;

}
struct ieee80211_frame {
	unsigned char i_fc[2];
	unsigned short i_dur;
	unsigned char i_addr1[MAC_LEN];
	unsigned char i_addr2[MAC_LEN];
	unsigned char i_addr3[MAC_LEN];
	unsigned char i_seq[2];
	/* possibly followed by addr4[IEEE80211_ADDR_LEN]; */
	/* see below */
};

static int capwap_header_add (struct sk_buff *skb) 
{

	unsigned short *cw = NULL;
	unsigned char *type = NULL;
	struct ieee80211_frame * h;
	/*check the 80211 frame*/
	if (skb->len <= PRISM2_HEADER_LENS) {
	
		return -1;
	}
	
	//if (skb->len > 1500) { /*check if this package need fragment or not*/
		
	//	return -1;
	//} 
	skb_pull(skb, PRISM2_HEADER_LENS);

	type = skb->data;

	if (!((*type)& 0x08)) { /*check 80211 frame is data or not */
		skb_push(skb, PRISM2_HEADER_LENS);
		return -1;
	} 
	
	h = (struct ieee80211_frame *) type;
	
	if (skb->protocol == htons(0x0020)) { /*EAP to wtpd in */
		//printk("--a eap packet at capwap_splide_fast_send\n");
		skb->protocol = htons(0x0019);
		dest_ac_addr = ac_addr;
	} else { 

		dest_ac_addr = find_mac_ip(h->i_addr2);
	
	}
	
	skb_push(skb, 16);

	memset(skb->data, 0x00, 16);
	cw = (unsigned short *) skb->data;
	*cw = htons(0x0020);

	cw = cw +1;

	*cw = htons(0x0320);

	cw = cw +3;

	*cw = htons(0x06a1);

	cw = cw +1;

	*cw = htons(0xa100);

	return 0;

}

extern int ip_route_input (struct sk_buff *skb, unsigned int source, unsigned int dest, unsigned char tos,  struct net_device *dev);

/* this function used to add ip and udp header for capwap splide mac fast*/
int capwap_udp_ip (struct sk_buff *skb)
{

	//if (!skb) {
	//	printk("capwap_splide_fast_send : skb should not be NULL\n");
	//	return -1;
	//}

	int total_len, eth_len, ip_len, udp_len, len;
	struct udphdr *udph;
	struct iphdr *iph;
	struct ethhdr *eth;
	struct net_device *local = NULL ;
	int local_ip = 0;
	unsigned dest_ip = dest_ac_addr;
	len = skb->len;
	udp_len = len + sizeof(*udph);
	ip_len = eth_len = udp_len + sizeof(*iph);
	total_len = eth_len + ETH_HLEN + NET_IP_ALIGN;
	

#if LINUX_VERSION_CODE <= KERNEL_VERSION(2,6,22)
	local = dev_get_by_name("default"); 
#else	
	local = dev_get_by_name(&init_net,"default"); 
#endif
	if (!local) {
		return -1;
	}

	skb->dev = local;

	if (skb->dev->ip_ptr) {

		if(((struct in_device *)(skb->dev->ip_ptr))->ifa_list) {

			local_ip = ((struct in_device *)(skb->dev->ip_ptr))->ifa_list->ifa_local;
		}

	}
	
	if (local_ip == 0) {

		return -1;		
	}
#if LINUX_VERSION_CODE <= KERNEL_VERSION(2,6,20)
	skb->h.uh = udph = (struct udphdr *) skb_push(skb, sizeof(*udph));
#else
	skb->transport_header = udph = (struct udphdr *) skb_push(skb, sizeof(*udph));
#endif
	udph->source = htons(udp_port);
	udph->dest = htons(AC_DATA_PORT);
	udph->len = htons(udp_len);
	udph->check = 0;
	udph->check = csum_tcpudp_magic(htonl(local_ip),
					htonl(dest_ip),
					udp_len,  IPPROTO_UDP,
					csum_partial((unsigned char *)udph, udp_len, 0));
	if (udph->check == 0)
		udph->check = CSUM_MANGLED_0;

#if LINUX_VERSION_CODE <= KERNEL_VERSION(2,6,22)
	skb->nh.iph = iph = (struct iphdr *)skb_push(skb, sizeof(*iph));
#else
	skb->network_header= iph = (struct iphdr *)skb_push(skb, sizeof(*iph));
#endif
	/* iph->version = 4; iph->ihl = 5; */
	put_unaligned(0x45, (unsigned char *)iph);
	iph->tos      = 0;
	put_unaligned(htons(ip_len), &(iph->tot_len));
	//static unsigned id = 0xfffe;
	iph->id       = 0 ;//htons(id++);
	iph->frag_off = 0;// htons(0x4000);
	iph->ttl      = 64;
	iph->protocol = IPPROTO_UDP;
	iph->check    = 0;
	put_unaligned(htonl(local_ip), &(iph->saddr));
	put_unaligned(htonl(dest_ip), &(iph->daddr));
	iph->check    = ip_fast_csum((unsigned char *)iph, iph->ihl);
	
	skb->pkt_type = PACKET_HOST;
	eth = (struct ethhdr *) skb_push(skb, ETH_HLEN);
	#if LINUX_VERSION_CODE <= KERNEL_VERSION(2,6,20)
	skb->mac.raw = skb->data;
	#else
	skb->mac_header = skb->data;
	#endif
	skb->protocol = eth->h_proto = htons(ETH_P_IP);
	memcpy(eth->h_source, local->dev_addr, 6);
	memcpy(eth->h_dest, local->dev_addr, 6);
	
	skb->mac_len = ETH_HLEN;

	/*xmeng add for local ip route check, this is for capwap splide mac fast up*/
	#if LINUX_VERSION_CODE <= KERNEL_VERSION(2,6,20)
	if (skb->dst == NULL) 
	#else
	#if LINUX_VERSION_CODE != KERNEL_VERSION(2,6,35)//liuqaing add	
	if ((struct  dst_entry *)(skb->_skb_dst) == NULL)
	#endif
	#endif
	{

		unsigned sip ;
		sip = local_ip +1;
		if (sip == dest_ip) {
			sip = sip +1;
			if ((sip & ~ (((struct in_device *)(local->ip_ptr))->ifa_list->ifa_mask)) == ~(((struct in_device *)(local->ip_ptr))->ifa_list->ifa_mask)) {
				sip =  local_ip - 1;
			}
		} else if ((sip & ~ (((struct in_device *)(local->ip_ptr))->ifa_list->ifa_mask)) == ~(((struct in_device *)(local->ip_ptr))->ifa_list->ifa_mask)) {
		
			sip = local_ip -1;
			if (sip == dest_ip) {
				
				sip = sip -1;
			}
	
		}

		#if 0
		{	
			printk("===========ip_route_input before in capwap============\n");
			int i =0;
			while (i <= 48)
			{
				if ((0 != (i%8)) || (0 == i))
				{
					printk("%02x ",skb->data[i]);
					i++;
					continue;
				}

				printk("\n");
				printk("%02x ",skb->data[i]);
				i++;                     
			}
				printk("\n\n\n");
		}
		
		#endif
		ip_route_input(skb, iph->daddr, sip, iph->tos,skb->dev);

		
	#if 0
	{
	
		printk("=======%s:%d sip=%d.%d.%d.%d [used to route]========\n",
				__func__, __LINE__,((unsigned char *) &sip)[0],
				((unsigned char *) &sip)[1],((unsigned char *) &sip)[2],
				((unsigned char *) &sip)[3]);
		printk("=======%s:%d iph->daddr=%d.%d.%d.%d [used to route]========\n",
			__func__, __LINE__,((unsigned char *) &iph->daddr)[0],
			((unsigned char *) &iph->daddr)[1],((unsigned char *) &iph->daddr)[2],
			((unsigned char *) &iph->daddr)[3]);

		printk("=======%s:%d iph->saddr=%d.%d.%d.%d [used to route]========\n",
			__func__, __LINE__,((unsigned char *) &iph->saddr)[0],
			((unsigned char *) &iph->saddr)[1],((unsigned char *) &iph->saddr)[2],
			((unsigned char *) &iph->saddr)[3]);
		printk("======%s:%d print frame from wlan-driver============\n",__func__, __LINE__);
		
		int i =0;
		while (i <= 48)
		{
			if ((0 != (i%8)) || (0 == i))
			{
				printk("%02x ",skb->data[i]);
				i++;
				continue;
			}

			printk("\n");
			printk("%02x ",skb->data[i]);
			i++;                     
		}
		printk("\n\n\n");
	}
	#endif	
	}
	
	skb_pull(skb, ETH_HLEN);
	return 0;
}



static int capwap_splide_fast_send(struct sk_buff *skb) 
{
	struct ieee80211vap *vap =  NULL; //Added by duanmingzhe for get vap from netdev

	//if (skb->protocol == htons(0x0020)) { /*EAP to wtpd in */
	//	printk("--a eap packet at capwap_splide_fast_send\n");
	//	skb->protocol == htons(0x0019);
	//	return -1;
	//} 
	/*Begin:added by duanmingzhe wmm to ip header 2010-08-28*/
	unsigned char tos = 0;
	unsigned short ip_type = 0;
	/*End:added by duanmingzhe wmm to ip header 2010-08-28*/
    	unsigned int packet_len = 0;
	if (!spfast_up) {
		
		return -1;
	}

	/*chech the skb dev, if it does not come from eth0, we do nothing*/
	if (strncmp(skb->dev->name, "ath", 3) == 0) {

		;//printk("--function: capwap_splide_fast_send, dev = ath \n");

	} else {
		
		return -1;
	}
	vap = get_vap_from_netdev(skb->dev); //Added by duanmingzhe for get vap from netdev
	if (vap->vap_splitmac == 0) { 
			
			return -1;
	}	
	if (skb->protocol == htons(0x0019) || skb->protocol == htons(0x0020)) {
		
		;//printk("--function: capwap_splide_fast_send, skb->protocal == 0x0019 \n");
	} else {
		return -1;
	}
	
		
	if (ac_addr == 0) {

		return -1;
	}
        
	/*Begin:added by duanmingzhe wmm to ip header 2010-08-28*/
	if(wmm_to_tos)
	{
		unsigned char fc = 0;
		memcpy(&fc,skb->data+PRISM2_HEADER_LENS,1);
		if(fc & QOSDATA_80211)
		{
			memcpy(&ip_type,skb->data+PRISM2_HEADER_LENS + MAC_80211_LEN + QOS_LEN + SNAP_LEN ,TYPE_LEN);
		}
		else
		{
			memcpy(&ip_type,skb->data+PRISM2_HEADER_LENS + MAC_80211_LEN + SNAP_LEN ,TYPE_LEN);
		}
		
		if(IPV6_HEAD == ip_type)
		{
			unsigned char tos_1 = 0;
			if(fc & QOSDATA_80211)
			{
				memcpy(&tos,skb->data+PRISM2_HEADER_LENS + MAC_80211_LEN +QOS_LEN+ SNAP_LEN + TYPE_LEN ,TOS_LEN);
				memcpy(&tos_1,skb->data+PRISM2_HEADER_LENS + MAC_80211_LEN +QOS_LEN+ SNAP_LEN + TYPE_LEN + 1,TOS_LEN);
				tos = tos << 4;
				tos |= (tos_1 >>4);
			}
			else
			{
				memcpy(&tos,skb->data+PRISM2_HEADER_LENS + MAC_80211_LEN + SNAP_LEN + TYPE_LEN,TOS_LEN);
				memcpy(&tos_1,skb->data+PRISM2_HEADER_LENS + MAC_80211_LEN + SNAP_LEN + TYPE_LEN + 1,TOS_LEN);
				tos = tos << 4;
				tos |= (tos_1 >>4);
			}
		}
		else if(IP_HEAD == ip_type)
		{
			if(fc & QOSDATA_80211)
			{
				memcpy(&tos,skb->data+PRISM2_HEADER_LENS + MAC_80211_LEN +4+ SNAP_LEN + TYPE_LEN + 1,TOS_LEN);
			}
			else
			{
				memcpy(&tos,skb->data+PRISM2_HEADER_LENS + MAC_80211_LEN + SNAP_LEN + TYPE_LEN + 1,TOS_LEN);
			}
		}
	}
	/*End:added by duanmingzhe wmm to ip header 2010-08-28*/
	/*add capwap header to skb*/
	#if 0
	skb->zt_frame = 8023;
	skb->zt_id = ++zt_id;
	#endif
	if (capwap_header_add(skb) < 0) {
		
		return -1;
	}

	capwap_udp_ip(skb);
	/*Begin:added by duanmingzhe wmm to ip header 2010-08-28*/
	if(wmm_to_tos)
	{
		if(IP_HEAD == ip_type || IPV6_HEAD == ip_type)
		{
			struct iphdr *iph = (struct iphdr *)skb->data;
			iph->tos = tos;
			iph->check = 0;
			iph->check = ip_fast_csum((unsigned char *)iph, iph->ihl);
		}
	}
	/*End:added by duanmingzhe wmm to ip header 2010-08-28*/
    //skb_len  + eth?
    //- ip - udp - capwap - 80211 - LLC

    packet_len = skb->len - 62;
    //printk("packet_send_len:%d - %d\n",packet_len,skb->len);
    bytes_send += packet_len ;
    
	return 0;

}


/*
 * Deal with the sysctl handler api changing.
 */
/* zhengkun add kernel kernel(2.6.36) proc interface*/
//#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,36)
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,34)
#define	CAPWAP_SYSCTL_DECL(f, ctl, write, filp, buffer, lenp, ppos) \
	f(ctl_table *ctl, int write, struct file *filp, void *buffer,\
		size_t *lenp, loff_t *ppos)
#define	CAPWAP_SYSCTL_PROC_DOINTVEC(ctl, write, filp, buffer, lenp, ppos) \
	proc_dointvec(ctl, write, filp, buffer, lenp, ppos)
//chenxf added for bytes_recv & bytes_send supporting 64bit 2011-11-16
#if defined(PC018)
const unsigned long long capwap_zero = 0, capwap_infinity = ~0ULL;
#define	CAPWAP_SYSCTL_PROC_DODOULONGVEC_MINMAX(ctl, write, filp, buffer, lenp, ppos) \
	proc_doulongvec_minmax(ctl, write, filp, buffer, lenp, ppos)
#endif
//chenxf added end
#else
#define	CAPWAP_SYSCTL_DECL(f, ctl, write, filp, buffer, lenp, ppos) \
	f(ctl_table *ctl, int write, void *buffer,\
		size_t *lenp, loff_t *ppos)
#define	CAPWAP_SYSCTL_PROC_DOINTVEC(ctl, write, filp, buffer, lenp, ppos) \
	proc_dointvec(ctl, write, buffer, lenp, ppos)
const unsigned long long capwap_zero = 0, capwap_infinity = ~0ULL;
#define	CAPWAP_SYSCTL_PROC_DODOULONGVEC_MINMAX(ctl, write, filp, buffer, lenp, ppos) \
	proc_doulongvec_minmax(ctl, write, buffer, lenp, ppos)
#endif //end for #if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,36)

static struct ctl_table_header * capwap_sys_ctl;

static struct ctl_table * capwap_sysctls;

#define	CTL_AUTO	-2	/* cannot be CTL_ANY or CTL_NONE */
static int
CAPWAP_SYSCTL_DECL(udp_port_set, ctl, write, filp, buffer,
		lenp, ppos)
{
	
	u_int val;
	int ret;
	ctl->data = &val;
	ctl->maxlen = sizeof(val);
	if (write) {
		ret = CAPWAP_SYSCTL_PROC_DOINTVEC(ctl, write, filp, buffer,
			lenp, ppos);
		if (ret == 0)
			udp_port = val;
	} else {
			val =  udp_port;
			ret =CAPWAP_SYSCTL_PROC_DOINTVEC(ctl, write, filp, buffer,
					lenp, ppos);
	}

	return ret;
}

//liuqiang ZT tunnel 8023
static int
CAPWAP_SYSCTL_DECL(tunnel8023_udp_port_set, ctl, write, filp, buffer,
		lenp, ppos)
{
	
	u_int val;
	int ret;
	ctl->data = &val;
	ctl->maxlen = sizeof(val);
	if (write) {
		ret = CAPWAP_SYSCTL_PROC_DOINTVEC(ctl, write, filp, buffer,
			lenp, ppos);
		if (ret == 0)
			tunnel8023_udp_port = val;
	} else {
			val =  tunnel8023_udp_port;
			ret =CAPWAP_SYSCTL_PROC_DOINTVEC(ctl, write, filp, buffer,
					lenp, ppos);
	}

	return ret;
}



//

static int
CAPWAP_SYSCTL_DECL(spfast_down_set, ctl, write, filp, buffer,
		lenp, ppos)
{
	
	u_int val;
	int ret;
	ctl->data = &val;
	ctl->maxlen = sizeof(val);
	if (write) {
		ret = CAPWAP_SYSCTL_PROC_DOINTVEC(ctl, write, filp, buffer,
			lenp, ppos);
		if (ret == 0)
			spfast_down = val;
	} else {
			val =  spfast_down;
			ret =CAPWAP_SYSCTL_PROC_DOINTVEC(ctl, write, filp, buffer,
					lenp, ppos);
	}

	return ret;
}

//liuqiang ZT tunnel 8023
static int
CAPWAP_SYSCTL_DECL(tunnel8023_down_set, ctl, write, filp, buffer,
		lenp, ppos)
{
	
	u_int val;
	int ret;
	ctl->data = &val;
	ctl->maxlen = sizeof(val);
	if (write) {
		ret = CAPWAP_SYSCTL_PROC_DOINTVEC(ctl, write, filp, buffer,
			lenp, ppos);
		if (ret == 0)
			tunnel8023_down = val;
	} else {
			val =  tunnel8023_down;
			ret =CAPWAP_SYSCTL_PROC_DOINTVEC(ctl, write, filp, buffer,
					lenp, ppos);
	}

	return ret;
}


//liuqiang ZT tunnel 8023

static int
CAPWAP_SYSCTL_DECL(spfast_up_set, ctl, write, filp, buffer,
		lenp, ppos)
{
	
	u_int val;
	int ret;
	ctl->data = &val;
	ctl->maxlen = sizeof(val);
	if (write) {
		ret = CAPWAP_SYSCTL_PROC_DOINTVEC(ctl, write, filp, buffer,
			lenp, ppos);
		if (ret == 0)
			spfast_up = val;
	} else {
			val =  spfast_up;
			ret =CAPWAP_SYSCTL_PROC_DOINTVEC(ctl, write, filp, buffer,
					lenp, ppos);
	}

	return ret;
}

//liuqiang ZT tunnel 8023
static int
CAPWAP_SYSCTL_DECL(tunnel8023_up_set, ctl, write, filp, buffer,
		lenp, ppos)
{
	
	u_int val;
	int ret;
	ctl->data = &val;
	ctl->maxlen = sizeof(val);
	if (write) {
		ret = CAPWAP_SYSCTL_PROC_DOINTVEC(ctl, write, filp, buffer,
			lenp, ppos);
		if (ret == 0)
			tunnel8023_up = val;
	} else {
			val =  tunnel8023_up;
			ret =CAPWAP_SYSCTL_PROC_DOINTVEC(ctl, write, filp, buffer,
					lenp, ppos);
	}

	return ret;
}

//liuqiang ZT tunnel 8023

static int
CAPWAP_SYSCTL_DECL(ip_fragment_set, ctl, write, filp, buffer,
		lenp, ppos)
{
	
	u_int val;
	int ret;
	ctl->data = &val;
	ctl->maxlen = sizeof(val);
	if (write) {
		ret = CAPWAP_SYSCTL_PROC_DOINTVEC(ctl, write, filp, buffer,
			lenp, ppos);
		if (ret == 0)
			ip_fragment = val;
	} else {
			val =  ip_fragment;
			ret =CAPWAP_SYSCTL_PROC_DOINTVEC(ctl, write, filp, buffer,
					lenp, ppos);
	}

	return ret;
}


static int
CAPWAP_SYSCTL_DECL(capwap_fragment_set, ctl, write, filp, buffer,
		lenp, ppos)
{
	
	u_int val;
	int ret;
	ctl->data = &val;
	ctl->maxlen = sizeof(val);
	if (write) {
		ret = CAPWAP_SYSCTL_PROC_DOINTVEC(ctl, write, filp, buffer,
			lenp, ppos);
		if (ret == 0)
			capwap_fragment = val;
	} else {
			val =  capwap_fragment;
			ret =CAPWAP_SYSCTL_PROC_DOINTVEC(ctl, write, filp, buffer,
					lenp, ppos);
	}

	return ret;
}

static int
CAPWAP_SYSCTL_DECL(ac_addr_set, ctl, write, filp, buffer,
		lenp, ppos)
{
	
	unsigned val;
	int ret;
	ctl->data = &val;
	ctl->maxlen = sizeof(val);
	if (write) {
		ret = CAPWAP_SYSCTL_PROC_DOINTVEC(ctl, write, filp, buffer,
			lenp, ppos);
		if (ret == 0)
			ac_addr = val;
	} else {
			val =  ac_addr;
			ret =CAPWAP_SYSCTL_PROC_DOINTVEC(ctl, write, filp, buffer,
					lenp, ppos);
	}

	return ret;
}

static int
CAPWAP_SYSCTL_DECL(mac_ip_flag_set, ctl, write, filp, buffer,
		lenp, ppos)
{
	
	unsigned val;
	int ret;
	ctl->data = &val;
	ctl->maxlen = sizeof(val);
	if (write) {
		ret = CAPWAP_SYSCTL_PROC_DOINTVEC(ctl, write, filp, buffer,
			lenp, ppos);
		if (ret == 0)
			mac_ip_flag = val;
			if (mac_ip_flag) {

				delete_mac_ip();
				insert_mac_ip();

			} else {
				
				delete_mac_ip();
			}				
	} else {	
			printk("----find ip_addr: %d", find_mac_ip(mac));
			val =  mac_ip_flag;
			ret =CAPWAP_SYSCTL_PROC_DOINTVEC(ctl, write, filp, buffer,
					lenp, ppos);
	}

	return ret;
}

static int
CAPWAP_SYSCTL_DECL(temp_ip_addr_set, ctl, write, filp, buffer,
		lenp, ppos)
{
	
	unsigned val;
	int ret;
	ctl->data = &val;
	ctl->maxlen = sizeof(val);
	if (write) {
		ret = CAPWAP_SYSCTL_PROC_DOINTVEC(ctl, write, filp, buffer,
			lenp, ppos);
		if (ret == 0)
			temp_ip_addr = val;
	} else {
			val =  temp_ip_addr;
			ret =CAPWAP_SYSCTL_PROC_DOINTVEC(ctl, write, filp, buffer,
					lenp, ppos);
	}

	return ret;
}

static int
CAPWAP_SYSCTL_DECL(mac0_set, ctl, write, filp, buffer,
		lenp, ppos)
{
	
	unsigned val;
	int ret;
	ctl->data = &val;
	ctl->maxlen = sizeof(val);
	if (write) {
		ret = CAPWAP_SYSCTL_PROC_DOINTVEC(ctl, write, filp, buffer,
			lenp, ppos);
		if (ret == 0)
			mac[0] = val;
	} else {
			val =  mac[0];
			ret =CAPWAP_SYSCTL_PROC_DOINTVEC(ctl, write, filp, buffer,
					lenp, ppos);
	}

	return ret;
}

static int
CAPWAP_SYSCTL_DECL(mac1_set, ctl, write, filp, buffer,
		lenp, ppos)
{
	
	unsigned val;
	int ret;
	ctl->data = &val;
	ctl->maxlen = sizeof(val);
	if (write) {
		ret = CAPWAP_SYSCTL_PROC_DOINTVEC(ctl, write, filp, buffer,
			lenp, ppos);
		if (ret == 0)
			mac[1] = val;
	} else {
			val =  mac[1];
			ret =CAPWAP_SYSCTL_PROC_DOINTVEC(ctl, write, filp, buffer,
					lenp, ppos);
	}

	return ret;
}

static int
CAPWAP_SYSCTL_DECL(mac2_set, ctl, write, filp, buffer,
		lenp, ppos)
{
	
	unsigned val;
	int ret;
	ctl->data = &val;
	ctl->maxlen = sizeof(val);
	if (write) {
		ret = CAPWAP_SYSCTL_PROC_DOINTVEC(ctl, write, filp, buffer,
			lenp, ppos);
		if (ret == 0)
			mac[2] = val;
	} else {
			val =  mac[2];
			ret =CAPWAP_SYSCTL_PROC_DOINTVEC(ctl, write, filp, buffer,
					lenp, ppos);
	}

	return ret;
}

static int
CAPWAP_SYSCTL_DECL(mac3_set, ctl, write, filp, buffer,
		lenp, ppos)
{
	
	unsigned val;
	int ret;
	ctl->data = &val;
	ctl->maxlen = sizeof(val);
	if (write) {
		ret = CAPWAP_SYSCTL_PROC_DOINTVEC(ctl, write, filp, buffer,
			lenp, ppos);
		if (ret == 0)
			mac[3] = val;
	} else {
			val =  mac[3];
			ret =CAPWAP_SYSCTL_PROC_DOINTVEC(ctl, write, filp, buffer,
					lenp, ppos);
	}

	return ret;
}

static int
CAPWAP_SYSCTL_DECL(mac4_set, ctl, write, filp, buffer,
		lenp, ppos)
{
	
	unsigned val;
	int ret;
	ctl->data = &val;
	ctl->maxlen = sizeof(val);
	if (write) {
		ret = CAPWAP_SYSCTL_PROC_DOINTVEC(ctl, write, filp, buffer,
			lenp, ppos);
		if (ret == 0)
			mac[4] = val;
	} else {
			val =  mac[4];
			ret =CAPWAP_SYSCTL_PROC_DOINTVEC(ctl, write, filp, buffer,
					lenp, ppos);
	}

	return ret;
}

static int
CAPWAP_SYSCTL_DECL(mac5_set, ctl, write, filp, buffer,
		lenp, ppos)
{
	
	unsigned val;
	int ret;
	ctl->data = &val;
	ctl->maxlen = sizeof(val);
	if (write) {
		ret = CAPWAP_SYSCTL_PROC_DOINTVEC(ctl, write, filp, buffer,
			lenp, ppos);
		if (ret == 0)
			mac[5] = val;
	} else {
			val =  mac[5];
			ret =CAPWAP_SYSCTL_PROC_DOINTVEC(ctl, write, filp, buffer,
					lenp, ppos);
	}

	return ret;
}

//chenxf added for bytes_recv & bytes_send supporting 64bit 2011-11-16
#if (!defined(PC018) && !defined(APM82181))
static int
CAPWAP_SYSCTL_DECL(bytes_send_set, ctl, write, filp, buffer,
		lenp, ppos)
{
	unsigned val;
	int ret;
    char buf[32];

    memset(buf, 0, 32);
    
	ctl->data = &val;
	ctl->maxlen = sizeof(val);
	if (write) {
		ret = CAPWAP_SYSCTL_PROC_DOINTVEC(ctl, write, filp, buffer,
			lenp, ppos);
        //bytes_send = 64294968295;
		if (ret == 0)
		{
            sprintf(buf, "%llu", bytes_send);
            pro_file_sche_flag = 1;
            create_flag_file("/tmp/bytes_send", buf);
        }
            //printk("%llu\n",bytes_send);
			//bytes_send = val;
	} else {
	        //bytes_send = 64294968295;
			val = bytes_send;
			ret =CAPWAP_SYSCTL_PROC_DOINTVEC(ctl, write, filp, buffer,
					lenp, ppos);
	}

	return ret;
}

//liuqiang ZT tunnel 8023
static int
CAPWAP_SYSCTL_DECL(tunnel8023_bytes_send_set, ctl, write, filp, buffer,
		lenp, ppos)
{
	unsigned val;
	int ret;
    char buf[32];

    memset(buf, 0, 32);
    
	ctl->data = &val;
	ctl->maxlen = sizeof(val);
	if (write) {
		ret = CAPWAP_SYSCTL_PROC_DOINTVEC(ctl, write, filp, buffer,
			lenp, ppos);
        //bytes_send = 64294968295;
		if (ret == 0)
		{
            sprintf(buf, "%llu", tunnel8023_bytes_send);
            pro_file_sche_flag = 1;
            create_flag_file("/tmp/tunnel8023_bytes_send", buf);
        }
            //printk("%llu\n",bytes_send);
			//bytes_send = val;
	} else {
	        //bytes_send = 64294968295;
			val = tunnel8023_bytes_send;
			ret =CAPWAP_SYSCTL_PROC_DOINTVEC(ctl, write, filp, buffer,
					lenp, ppos);
	}

	return ret;
}
static int
CAPWAP_SYSCTL_DECL(tunnel8023_bytes_recv_set, ctl, write, filp, buffer,
		lenp, ppos)
{
	unsigned val;
	int ret;
    char buf[32];

    memset(buf, 0, 32);
    
	ctl->data = &val;
	ctl->maxlen = sizeof(val);
	if (write) {
		ret = CAPWAP_SYSCTL_PROC_DOINTVEC(ctl, write, filp, buffer,
			lenp, ppos);
        //bytes_recv = 64294968295;	
		if (ret == 0)
		{
            sprintf(buf, "%llu", tunnel8023_bytes_recv);
            pro_file_sche_flag = 1;
            create_flag_file("/tmp/tunnel8023_bytes_recv", buf);
        }
			//bytes_recv = val;
	} else {
	        //bytes_recv = 64294968295;
			val = tunnel8023_bytes_recv;
            //printk("---|%lu|---\n", val);
	    ret =CAPWAP_SYSCTL_PROC_DOINTVEC(ctl, write, filp, buffer,
                                        lenp, ppos);

	}

	return ret;
}


//liuqiang ZT tunnel 8023

static int
CAPWAP_SYSCTL_DECL(bytes_recv_set, ctl, write, filp, buffer,
		lenp, ppos)
{
	unsigned val;
	int ret;
    char buf[32];

    memset(buf, 0, 32);
    
	ctl->data = &val;
	ctl->maxlen = sizeof(val);
	if (write) {
		ret = CAPWAP_SYSCTL_PROC_DOINTVEC(ctl, write, filp, buffer,
			lenp, ppos);
        //bytes_recv = 64294968295;	
		if (ret == 0)
		{
            sprintf(buf, "%llu", bytes_recv);
            pro_file_sche_flag = 1;
            create_flag_file("/tmp/bytes_recv", buf);
        }
			//bytes_recv = val;
	} else {
	        //bytes_recv = 64294968295;
			val = bytes_recv;
            //printk("---|%lu|---\n", val);
	    ret =CAPWAP_SYSCTL_PROC_DOINTVEC(ctl, write, filp, buffer,
                                        lenp, ppos);

	}

	return ret;
}
#else  /*chenxf, the following deal with apv5 pc018 for support 64bit*/
static int
CAPWAP_SYSCTL_DECL(bytes_send_set, ctl, write, filp, buffer,
		lenp, ppos)
{
	unsigned long long val;
	int ret;
  
	ctl->data = &val;
	ctl->maxlen = sizeof(val);
	if (write) {
		ret = CAPWAP_SYSCTL_PROC_DODOULONGVEC_MINMAX(ctl, write, filp, buffer, lenp, ppos);
		if (ret == 0)
			bytes_send = val;
	} else {
		val = bytes_send;
		ret = CAPWAP_SYSCTL_PROC_DODOULONGVEC_MINMAX(ctl, write, filp, buffer, lenp, ppos);
	}

	return ret;
}

//liuqiang ZT tunnel 8023
static int
CAPWAP_SYSCTL_DECL(tunnel8023_bytes_send_set, ctl, write, filp, buffer,
		lenp, ppos)
{
	unsigned long long val;
	int ret;
  
	ctl->data = &val;
	ctl->maxlen = sizeof(val);
	if (write) {
		ret = CAPWAP_SYSCTL_PROC_DODOULONGVEC_MINMAX(ctl, write, filp, buffer, lenp, ppos);
		if (ret == 0)
			tunnel8023_bytes_send = val;
	} else {
		val = tunnel8023_bytes_send;
		ret = CAPWAP_SYSCTL_PROC_DODOULONGVEC_MINMAX(ctl, write, filp, buffer, lenp, ppos);
	}

	return ret;
}

static int
CAPWAP_SYSCTL_DECL(tunnel8023_bytes_recv_set, ctl, write, filp, buffer,
		lenp, ppos)
{
	unsigned long long val;
	int ret;
    
	ctl->data = &val;
	ctl->maxlen = sizeof(val);
	if (write) {
		ret = CAPWAP_SYSCTL_PROC_DODOULONGVEC_MINMAX(ctl, write, filp, buffer, lenp, ppos);
		if (ret == 0)
			tunnel8023_bytes_recv = val;
	} else {
		val = tunnel8023_bytes_recv;
		ret = CAPWAP_SYSCTL_PROC_DODOULONGVEC_MINMAX(ctl, write, filp, buffer, lenp, ppos);
	}

	return ret;
}

//liuqiang ZT tunnel 8023

static int
CAPWAP_SYSCTL_DECL(bytes_recv_set, ctl, write, filp, buffer,
		lenp, ppos)
{
	unsigned long long val;
	int ret;
    
	ctl->data = &val;
	ctl->maxlen = sizeof(val);
	if (write) {
		ret = CAPWAP_SYSCTL_PROC_DODOULONGVEC_MINMAX(ctl, write, filp, buffer, lenp, ppos);
		if (ret == 0)
			bytes_recv = val;
	} else {
		val = bytes_recv;
		ret = CAPWAP_SYSCTL_PROC_DODOULONGVEC_MINMAX(ctl, write, filp, buffer, lenp, ppos);
	}

	return ret;
}
#endif
//chenxf added end

/*Begin:added by duanmingzhe wmm to ip header 2010-08-28*/
static int
CAPWAP_SYSCTL_DECL(wmm_to_tos_set, ctl, write, filp, buffer,
		lenp, ppos)
{
	unsigned val;
	int ret;
	ctl->data = &val;
	ctl->maxlen = sizeof(val);
	if (write) {
		ret = CAPWAP_SYSCTL_PROC_DOINTVEC(ctl, write, filp, buffer,
			lenp, ppos);
		if (ret == 0)
			wmm_to_tos = val;
	} else {
			val = wmm_to_tos;
			ret =CAPWAP_SYSCTL_PROC_DOINTVEC(ctl, write, filp, buffer,
					lenp, ppos);
	}

	return ret;
}
/*End:added by duanmingzhe wmm to ip header 2010-08-28*/
/*<begin : transplant by caizhibang from apv5*/
/*yanggs add for tunnel mode wds 2012-01-14*/
static int
CAPWAP_SYSCTL_DECL(wds_tunnel_set, ctl, write, filp, buffer,
		lenp, ppos)
{
	
	u_int val;
	int ret;
	ctl->data = &val;
	ctl->maxlen = sizeof(val);
	if (write) {
		ret = CAPWAP_SYSCTL_PROC_DOINTVEC(ctl, write, filp, buffer,
			lenp, ppos);
		if (ret == 0)
			wds_tunnel = val;
	} else {
			val = wds_tunnel;
			ret =CAPWAP_SYSCTL_PROC_DOINTVEC(ctl, write, filp, buffer,
					lenp, ppos);
	}

	return ret;
}
/*yanggs add for tunnel mode wds end 2012-01-14*/
/*end : transplant by caizhibang from apv5 >*/
static const ctl_table CAPWAP_sysctl_template[] = {

	{ 
	#if LINUX_VERSION_CODE != KERNEL_VERSION(2,6,35)//liuqaing add
	  .ctl_name	= CTL_AUTO,
	#endif
	  .procname	= "udp_port",
	  .mode		= 0644,
	  .proc_handler	= udp_port_set
	},
	//liuqiang ZT tunnel 8023
	{ 
	#if LINUX_VERSION_CODE != KERNEL_VERSION(2,6,35)//liuqaing add
	  .ctl_name	= CTL_AUTO,
	#endif
	  .procname	= "tunnel8023_udp_port",
	  .mode		= 0644,
	  .proc_handler	= tunnel8023_udp_port_set
	},
	//liuqiang ZT tunnel 8023
	{ 
	#if LINUX_VERSION_CODE != KERNEL_VERSION(2,6,35)//liuqaing add	
	  .ctl_name	= CTL_AUTO,
	#endif	  
	  .procname	= "spfast_down",
	  .mode		= 0644,
	  .proc_handler	= spfast_down_set
	},
	//liuqiang ZT tunnel 8023
	{ 
	#if LINUX_VERSION_CODE != KERNEL_VERSION(2,6,35)//liuqaing add	
	  .ctl_name	= CTL_AUTO,
	#endif	  
	  .procname	= "tunnel8023_down",
	  .mode		= 0644,
	  .proc_handler	= tunnel8023_down_set
	},
	//liuqiang ZT tunnel 8023
	
	{ 
	#if LINUX_VERSION_CODE != KERNEL_VERSION(2,6,35)//liuqaing add	
	  .ctl_name	= CTL_AUTO,
	#endif
	  .procname	= "spfast_up",
	  .mode		= 0644,
	  .proc_handler	= spfast_up_set
	},
	//liuqiang ZT tunnel 8023
	{
	#if LINUX_VERSION_CODE != KERNEL_VERSION(2,6,35)//liuqiang add	
	  .ctl_name	= CTL_AUTO,
	#endif
	  .procname	= "tunnel8023_up",
	  .mode		= 0644,
	  .proc_handler	= tunnel8023_up_set
	},
	//liuqiang ZT tunnel 8023

	{ 
	#if LINUX_VERSION_CODE != KERNEL_VERSION(2,6,35)//liuqaing add
	  .ctl_name	= CTL_AUTO,
	#endif
	  .procname	= "ip_fragment",
	  .mode		= 0644,
	  .proc_handler	= ip_fragment_set
	},
	{ 
	#if LINUX_VERSION_CODE != KERNEL_VERSION(2,6,35)//liuqaing add
	  .ctl_name	= CTL_AUTO,
	#endif
	  .procname	= "capwap_fragment",
	  .mode		= 0644,
	  .proc_handler	= capwap_fragment_set
	},
	{ 
	#if LINUX_VERSION_CODE != KERNEL_VERSION(2,6,35)//liuqaing add
	  .ctl_name	= CTL_AUTO,
	#endif
	  .procname	= "ac_addr",
	  .mode		= 0644,
	  .proc_handler	= ac_addr_set
	},
	{ 
	#if LINUX_VERSION_CODE != KERNEL_VERSION(2,6,35)//liuqaing add
	  .ctl_name	= CTL_AUTO,
	#endif
	  .procname	= "mac_ip_flag",
	  .mode		= 0644,
	  .proc_handler	= mac_ip_flag_set
	},
	{ 
	#if LINUX_VERSION_CODE != KERNEL_VERSION(2,6,35)//liuqaing add
	  .ctl_name	= CTL_AUTO,
	#endif
	  .procname	= "temp_ip_addr",
	  .mode		= 0644,
	  .proc_handler	= temp_ip_addr_set
	},
	{ 
	#if LINUX_VERSION_CODE != KERNEL_VERSION(2,6,35)//liuqaing add
	  .ctl_name	= CTL_AUTO,
	#endif
	  .procname	= "mac0",
	  .mode		= 0644,
	  .proc_handler	= mac0_set
	},
	{ 
	#if LINUX_VERSION_CODE != KERNEL_VERSION(2,6,35)//liuqaing add
	  .ctl_name	= CTL_AUTO,
	#endif
	  .procname	= "mac1",
	  .mode		= 0644,
	  .proc_handler	= mac1_set
	},
	{ 
	#if LINUX_VERSION_CODE != KERNEL_VERSION(2,6,35)//liuqaing add
		.ctl_name	= CTL_AUTO,
	#endif
	  .procname	= "mac2",
	  .mode		= 0644,
	  .proc_handler	= mac2_set
	},
	{ 
	#if LINUX_VERSION_CODE != KERNEL_VERSION(2,6,35)//liuqaing add
		.ctl_name	= CTL_AUTO,
	#endif
	  .procname	= "mac3",
	  .mode		= 0644,
	  .proc_handler	= mac3_set
	},
	{ 
	#if LINUX_VERSION_CODE != KERNEL_VERSION(2,6,35)//liuqaing add
		.ctl_name	= CTL_AUTO,
	#endif
	  .procname	= "mac4",
	  .mode		= 0644,
	  .proc_handler	= mac4_set
	},
	{ 
	#if LINUX_VERSION_CODE != KERNEL_VERSION(2,6,35)//liuqaing add
		.ctl_name	= CTL_AUTO,
	#endif
	  .procname	= "mac5",
	  .mode		= 0644,
	  .proc_handler	= mac5_set
	},
//chenxf added for bytes_recv & bytes_send supporting 64bit 2011-11-16
#if (!defined(PC018) && !defined(APM82181))
	{
	#if LINUX_VERSION_CODE != KERNEL_VERSION(2,6,35)//liuqaing add	
	  .ctl_name = CTL_AUTO,
	#endif
	  .procname = "bytes_send",
	  .mode = 0664,
	  .proc_handler = bytes_send_set
	},
	//liuqiang ZT tunnel 8023
	{
	#if LINUX_VERSION_CODE != KERNEL_VERSION(2,6,35)//liuqaing add	
	  .ctl_name = CTL_AUTO,
	#endif
	  .procname = "tunnel8023_bytes_send",
	  .mode = 0664,
	  .proc_handler = tunnel8023_bytes_send_set
	},
	{
	#if LINUX_VERSION_CODE != KERNEL_VERSION(2,6,35)//liuqaing add
		.ctl_name = CTL_AUTO,
	#endif
      .procname = "tunnel8023_bytes_recv",
      .mode = 0664,
      .proc_handler = tunnel8023_bytes_recv_set
    },

	//liuqiang ZT tunnel 8023
	{
	#if LINUX_VERSION_CODE != KERNEL_VERSION(2,6,35)//liuqaing add
		.ctl_name = CTL_AUTO,
	#endif
      .procname = "bytes_recv",
      .mode = 0664,
      .proc_handler = bytes_recv_set
    },
#else
	{
	#if LINUX_VERSION_CODE != KERNEL_VERSION(2,6,35)//liuqaing add
		.ctl_name = CTL_AUTO,
	#endif
      .procname = "bytes_send",
      .data		= NULL,
	  .maxlen	= sizeof(unsigned long long),
	  .mode = 0664,
      .proc_handler = bytes_send_set,
	  .extra1		= &capwap_zero,
	  .extra2		= &capwap_infinity,
    },

	//liuqiang ZT tunnel 8023
	{
	#if LINUX_VERSION_CODE != KERNEL_VERSION(2,6,35)//liuqaing add
		.ctl_name = CTL_AUTO,
	#endif
      .procname = "tunnel8023_bytes_send",
      .data		= NULL,
	  .maxlen	= sizeof(unsigned long long),
	  .mode = 0664,
      .proc_handler = tunnel8023_bytes_send_set,
	  .extra1		= &capwap_zero,
	  .extra2		= &capwap_infinity,
    },
    {
	#if LINUX_VERSION_CODE != KERNEL_VERSION(2,6,35)//liuqaing add
		.ctl_name = CTL_AUTO,
	#endif
      .procname = "tunnel8023_bytes_recv",
      .data		= NULL,
	  .maxlen	= sizeof(unsigned long long),
      .mode = 0664,
      .proc_handler = tunnel8023_bytes_recv_set,
	  .extra1		= &capwap_zero,
	  .extra2		= &capwap_infinity,
    },
	//liuqiang ZT tunnel 8023

	{
	#if LINUX_VERSION_CODE != KERNEL_VERSION(2,6,35)//liuqaing add
		.ctl_name = CTL_AUTO,
	#endif
      .procname = "bytes_recv",
      .data		= NULL,
	  .maxlen	= sizeof(unsigned long long),
      .mode = 0664,
      .proc_handler = bytes_recv_set,
	  .extra1		= &capwap_zero,
	  .extra2		= &capwap_infinity,
    },
#endif
//chenxf added end 
	/*Begin:added by duanmingzhe wmm to ip header 2010-08-28*/
    {
	#if LINUX_VERSION_CODE != KERNEL_VERSION(2,6,35)//liuqaing add
			.ctl_name = CTL_AUTO,
	#endif
      .procname = "wmm_to_tos",
      .mode = 0664,
      .proc_handler = wmm_to_tos_set
    },
    /*End:added by duanmingzhe wmm to ip header 2010-08-28*/
	/*<begin : transplant by caizhibang from apv5*/
    /*yanggs add for tunnel mode wds 2012-01-14*/
    { 
	#if LINUX_VERSION_CODE != KERNEL_VERSION(2,6,35)//liuqaing add
		.ctl_name	= CTL_AUTO,
	#endif
      .procname	= "wds_tunnel", 
      .mode	= 0644,
      .proc_handler = wds_tunnel_set
    },
    /*yanggs add for tunnel mode wds end 2012-01-14*/
	/*end : transplant by caizhibang from apv5>*/
	{ 0 }
};

//chenxf added for apv5 being compatiable with 2.6.31 2011-11-17
/*zhengkun  added for apv6 being compatiable with 2.6.36 2011-11-18*/
#if (LINUX_VERSION_CODE == KERNEL_VERSION(2,6,36))
static struct ctl_path capwap_path[] = {
	{ .procname = "net",},
	{ .procname = "capwap",},
	{ }
};
#endif

#if (LINUX_VERSION_CODE == KERNEL_VERSION(2,6,35))
static struct ctl_path capwap_path[] = {
	{ .procname = "net",},
	{ .procname = "capwap",},
	{ }
};
#endif


#if (LINUX_VERSION_CODE == KERNEL_VERSION(2,6,31))
static struct ctl_path capwap_path[] = {
	{ .procname = "net", .ctl_name = CTL_NET, },
	{ .procname = "capwap", .ctl_name = CTL_AUTO, },
	{ }
};
#endif
//chenxf added end

static void capwap_sysctl_attach(void)
{
/*chenxf added for apv5 being compatiable with 2.6.31 2011-11-17*/
/*zhengkun  added for apv6 being compatiable with 2.6.36 2011-11-18*/
#if (LINUX_VERSION_CODE == KERNEL_VERSION(2,6,31)|| LINUX_VERSION_CODE == KERNEL_VERSION(2,6,36) \
		|| LINUX_VERSION_CODE == KERNEL_VERSION(2,6,35))
#ifdef CONFIG_SYSCTL
		capwap_sys_ctl = register_sysctl_paths(capwap_path, CAPWAP_sysctl_template);
		if (capwap_sys_ctl == NULL) {
			printk(KERN_WARNING "capwap: can't register to sysctl.\n");
		}
#endif
#else /*#if (LINUX_VERSION_CODE == KERNEL_VERSION(2,6,31)|| LINUX_VERSION_CODE == KERNEL_VERSION(2,6,36)) chenxf added end*/

	int i, space;

	space = 9*sizeof(struct ctl_table) + sizeof(CAPWAP_sysctl_template);
	capwap_sysctls = kmalloc(space, GFP_KERNEL);
	if (capwap_sysctls == NULL) {
		printk("%s: no memory for sysctl table!\n", __func__);
		return;
	}

	/* setup the table */
	memset(capwap_sysctls, 0, space);
	capwap_sysctls[0].ctl_name = CTL_NET;
	capwap_sysctls[0].procname = "net";
	capwap_sysctls[0].mode = 0555;
	capwap_sysctls[0].child = &capwap_sysctls[2];
	/* [1] is NULL terminator */
	capwap_sysctls[2].ctl_name = CTL_AUTO;
	capwap_sysctls[2].procname = "capwap";/* XXX bad idea? */
	capwap_sysctls[2].mode = 0555;
	capwap_sysctls[2].child = &capwap_sysctls[4];
	/* [3] is NULL terminator */
	/* copy in pre-defined data */
	memcpy(&capwap_sysctls[4], CAPWAP_sysctl_template,
		sizeof(CAPWAP_sysctl_template));

	/* add in dynamic data references */
	for (i = 4; capwap_sysctls[i].ctl_name; i++)
		if (capwap_sysctls[i].extra1 == NULL)
			capwap_sysctls[i].extra1 = capwap_sysctls; // sp;

	/* tack on back-pointer to parent device */
	capwap_sysctls[i-1].data = "capwap" ;	

	/* and register everything */
#ifdef CONFIG_MACH_AR7100
	capwap_sys_ctl = register_sysctl_table(capwap_sysctls,0);
#else
#ifdef CONFIG_MACH_AR7240
	capwap_sys_ctl = register_sysctl_table(capwap_sysctls,0);
#else
	capwap_sys_ctl = register_sysctl_table(capwap_sysctls);
#endif
#endif

	if (!capwap_sys_ctl) {
		printk("%s: failed to register sysctls!\n", "capwap");
		kfree(capwap_sysctls);
		capwap_sysctls = NULL;
	}
#endif /*#if (LINUX_VERSION_CODE == KERNEL_VERSION(2,6,31))*/ 
}

static void capwap_sysctl_detach(void)
{

	if (capwap_sys_ctl) {
		unregister_sysctl_table(capwap_sys_ctl);
		capwap_sys_ctl = NULL;
	}
/*chenxf added for apv5 being compatiable with 2.6.31 2011-11-17*/
#if (LINUX_VERSION_CODE != KERNEL_VERSION(2,6,31) &&  LINUX_VERSION_CODE != KERNEL_VERSION(2,6,36))
	if (capwap_sysctls) {
		kfree(capwap_sysctls);
		capwap_sysctls = NULL;
	}
#endif
//chenxf added end
}
extern int (*capwap_splide_fast_send_hook)(struct sk_buff *skb);
extern int (*capwap_splide_fast_recv_hook)(struct sk_buff *skb);
extern void (*capwap_get_uplink_mac_hook)(struct sk_buff *skb);

//liuqiang ZT tunnel 8023
extern int (*tunnel8023_fast_send_hook)(struct sk_buff *skb);
extern int (*tunnel8023_fast_recv_hook)(struct sk_buff *skb);
#define IP_MF		0x2000		/* Flag: "More Fragments"	*/
#define IP_OFFSET	0x1FFF		/* "Fragment Offset" part	*/


/* this function used to add ip and udp header for  tunnel8023*/
int tunnel8023_udp_ip (struct sk_buff *skb)
{

	int total_len, eth_len, ip_len, udp_len, len;
	struct udphdr *udph;
	struct iphdr *iph;
	struct ethhdr *eth;
	struct net_device *local = NULL ;
	int local_ip = 0;
	unsigned dest_ip = ac_addr;
	len = skb->len;
	udp_len = len + sizeof(*udph);
	ip_len = eth_len = udp_len + sizeof(*iph);
	total_len = eth_len + ETH_HLEN + NET_IP_ALIGN;
	

#if LINUX_VERSION_CODE <= KERNEL_VERSION(2,6,22)
	local = dev_get_by_name("default"); 
#else	
	local = dev_get_by_name(&init_net,"default"); 
#endif
	if (!local) {
		return -1;
	}

	skb->dev = local;

	if (skb->dev->ip_ptr) {

		if(((struct in_device *)(skb->dev->ip_ptr))->ifa_list) {

			local_ip = ((struct in_device *)(skb->dev->ip_ptr))->ifa_list->ifa_local;
		}

	}
	
	if (local_ip == 0) {

		return -1;		
	}
#if LINUX_VERSION_CODE <= KERNEL_VERSION(2,6,20)
	skb->h.uh = udph = (struct udphdr *) skb_push(skb, sizeof(*udph));
#else
	skb->transport_header = udph = (struct udphdr *) skb_push(skb, sizeof(*udph));//add upd-header
#endif
	//udph->source = htons(tunnel8023_udp_port);
	//udph->dest = htons(AC_DATA_PORT);
	udph->source = htons(AC_DATA_PORT + 1);
	udph->dest = htons(tunnel8023_udp_port);
	udph->len = htons(udp_len);
	udph->check = 0;
	udph->check = csum_tcpudp_magic(htonl(local_ip),
					htonl(dest_ip),
					udp_len,  IPPROTO_UDP,
					csum_partial((unsigned char *)udph, udp_len, 0));
	if (udph->check == 0)
		udph->check = CSUM_MANGLED_0;

#if LINUX_VERSION_CODE <= KERNEL_VERSION(2,6,22)
	skb->nh.iph = iph = (struct iphdr *)skb_push(skb, sizeof(*iph));
#else
	skb->network_header= iph = (struct iphdr *)skb_push(skb, sizeof(*iph)); //add network-header
#endif
	/* iph->version = 4; iph->ihl = 5; */
	put_unaligned(0x45, (unsigned char *)iph);
	iph->tos      = 0;
	put_unaligned(htons(ip_len), &(iph->tot_len));
	//static unsigned id = 0xfffe;
	iph->id       = 0 ;//htons(id++);
	iph->frag_off = 0;// htons(0x4000);
	iph->ttl      = 64;
	iph->protocol = IPPROTO_UDP;
	iph->check    = 0;
	put_unaligned(htonl(local_ip), &(iph->saddr));
	put_unaligned(htonl(dest_ip), &(iph->daddr));
	iph->check    = ip_fast_csum((unsigned char *)iph, iph->ihl);
	
	skb->pkt_type = PACKET_HOST;
	eth = (struct ethhdr *) skb_push(skb, ETH_HLEN); // add eth-header
	#if LINUX_VERSION_CODE <= KERNEL_VERSION(2,6,20)
	skb->mac.raw = skb->data;
	#else
	skb->mac_header = skb->data;
	#endif
	skb->protocol = eth->h_proto = htons(ETH_P_IP);
	memcpy(eth->h_source, local->dev_addr, 6);
	memcpy(eth->h_dest, local->dev_addr, 6);
	
	skb->mac_len = ETH_HLEN;

	/*xmeng add for local ip route check, this is for capwap splide mac fast up*/
	#if LINUX_VERSION_CODE <= KERNEL_VERSION(2,6,20)
	if (skb->dst == NULL) 
	#else
	#if LINUX_VERSION_CODE != KERNEL_VERSION(2,6,35)//liuqaing add	
	if ((struct  dst_entry *)(skb->_skb_dst) == NULL)
	#endif
	#endif
	{

		unsigned sip ;
		sip = local_ip +1;
		if (sip == dest_ip) {
			sip = sip +1;
			if ((sip & ~ (((struct in_device *)(local->ip_ptr))->ifa_list->ifa_mask)) == ~(((struct in_device *)(local->ip_ptr))->ifa_list->ifa_mask)) {
				sip =  local_ip - 1;
			}
		} else if ((sip & ~ (((struct in_device *)(local->ip_ptr))->ifa_list->ifa_mask)) == ~(((struct in_device *)(local->ip_ptr))->ifa_list->ifa_mask)) {
		
			sip = local_ip -1;
			if (sip == dest_ip) {
				
				sip = sip -1;
			}
	
		}
		/*
	   The function ip_route_input() is invoked for each IP packet arriving over a network interface. 
	   The parameters are a pointer to the socket-buffer structure, the destination and 
	   source addresses, the TOS value, and a pointer to the net_device structure of the receiving network interface.
		*/

		if (ip_route_input(skb, iph->daddr, sip, iph->tos,skb->dev) == 0)
		{
			;
		}
		else
		{
			printk("============find route Failed=========\n");
		}
		
		#if 0
		printk("=======%s:%d sip=%d.%d.%d.%d [used to route]========\n",
				__func__, __LINE__,((unsigned char *) &sip)[0],
				((unsigned char *) &sip)[1],((unsigned char *) &sip)[2],
				((unsigned char *) &sip)[3]);
		#endif
		#if 0
		printk("=======%s:%d iph->daddr=%d.%d.%d.%d ========\n",
			__func__, __LINE__,((unsigned char *) &iph->daddr)[0],
			((unsigned char *) &iph->daddr)[1],((unsigned char *) &iph->daddr)[2],
			((unsigned char *) &iph->daddr)[3]);
		printk("=======%s:%d iph->saddr=%d.%d.%d.%d ========\n",
			__func__, __LINE__,((unsigned char *) &iph->saddr)[0],
			((unsigned char *) &iph->saddr)[1],((unsigned char *) &iph->saddr)[2],
			((unsigned char *) &iph->saddr)[3]);
		#endif
		
	}
	#if 0
	printk("=======%s:%d frame_id[%u] print frame after add tunnel-8023 header==========\n",
			__func__, __LINE__,skb->zt_id);
	
	int i =0;
	while (i <= 65)
	{
		if ((0 != (i%8)) || (0 == i))
		{
			printk("%02x ",skb->data[i]);
			i++;
			continue;
		}

		printk("\n");
		printk("%02x ",skb->data[i]);
		i++;                     
	}
	printk("\n\n\n");
	#endif
	skb_pull(skb, ETH_HLEN);
	return 0;
}

static int tunnel8023_add_zt_header(struct sk_buff *skb);

//extern void ip_copy_metadata_for_zt(struct sk_buff *to, struct sk_buff *from);
//extern void ip_options_fragment(struct sk_buff * skb);
extern int netif_receive_skb(struct sk_buff *skb);
static int ip_fragment_tunnel8023(struct sk_buff * skb)
{
	struct iphdr *iph;
	int ptr = 0;
	struct sk_buff *skb2;
	unsigned int mtu, hlen, left, len;
	int offset;
	__be16 not_last_frag;
	unsigned char * sta_mac_header = NULL;
	const unsigned int tunnel_header_len = 128; //Sta_MAC = 18, Tunnel_header = 48 , reverse = 8 

	sta_mac_header = skb->data;
	/*
	 *	Point into the IP datagram header.
	 */
	 
	iph = skb_pull(skb,18); //remove sta MAC + VLAN-tag

	/*
	 *	Setup starting values.
	 */

	hlen = iph->ihl * 4;
	mtu = 1500 - 512;	/* Size of data space  */

	left = skb->len - hlen; 	/* Space per frame */
	ptr =  hlen;		/* Where to start from */

	/*
	 *	Fragment the datagram.
	 */

	offset = (ntohs(iph->frag_off) & IP_OFFSET) << 3;
	not_last_frag = iph->frag_off & htons(IP_MF);

	/*
	 *	Keep copying data until we run out.
	 */
	//printk("=====%s : %d left = %u=========\n", __func__, __LINE__,left);
	while (left > 0) {
		len = left;
		/* IF: it doesn't fit, use 'mtu' - the data space left */
		if (len > mtu)
			len = mtu;
		/* IF: we are not sending upto and including the packet end
		   then align the next start on an eight byte boundary */
		if (len < left) {
			len &= ~7;
		}
		/*
		 *	Allocate buffer.
		 */
		//printk("=====%s : %d len = %u=========\n", __func__, __LINE__,len);
		if ((skb2 = alloc_skb(len+hlen+tunnel_header_len, GFP_ATOMIC)) == NULL) {
			printk(KERN_INFO "IP: frag: no memory for new fragment!\n");
			return -1;
		}

		/*
		 *	Set up data on packet
		 */

		//ip_copy_metadata_for_zt(skb2, skb);
		skb_reserve(skb2, tunnel_header_len);
		skb_put(skb2, len + hlen);
		skb_reset_network_header(skb2);
		skb2->transport_header = skb2->network_header + hlen;

		/*
		 *	Charge the memory for the fragment to any owner
		 *	it might possess
		 */

		if (skb->sk)
			skb_set_owner_w(skb2, skb->sk);

		/*
		 *	Copy the packet header into the new buffer.
		 */

		skb_copy_from_linear_data(skb, skb_network_header(skb2), hlen);

		/*
		 *	Copy a block of the IP datagram.
		 */
		memcpy(skb2->transport_header, skb->data+ptr, len);
		
		ptr += len;
		left -= len;

		/*
		 *	Fill in the new header fields.
		 */
		iph = ip_hdr(skb2);
		iph->frag_off = htons((offset >> 3));

		/* ANK: dirty, but effective trick. Upgrade options only if
		 * the segment to be fragmented was THE FIRST (otherwise,
		 * options are already fixed) and make it ONCE
		 * on the initial skb, so that all the following fragments
		 * will inherit fixed options.
		 */
		//if (offset == 0)
		//	ip_options_fragment(skb);

		/*
		 *	Added AC : If we are fragmenting a fragment that's not the
		 *		   last fragment then keep MF on each bit
		 */
		if (left > 0 || not_last_frag)
			iph->frag_off |= htons(IP_MF);
		
		
		offset += len;

		/*
		 *	Put this fragment into the sending queue.
		 */
		iph->tot_len = htons(len + hlen);

		//ip_send_check(iph);
		iph->check = 0;
		iph->check = ip_fast_csum((unsigned char *)iph, iph->ihl);

		skb_push(skb2, 18);
		skb2->sta_sid = skb->sta_sid;
		memcpy(skb2->data, sta_mac_header, 18); //add sta MAC
		skb2->protocol = htons(0x0030);
		skb2->dev = skb->dev;
		#if 0
		int i =0;
		printk("======%s:%d frament ==========\n", __func__, __LINE__);
		while (i <= 37)
		{
			if ((0 != (i%8)) || (0 == i))
			{
				printk("%02x ",skb2->data[i]);
				i++;
				continue;
			}

			printk("\n");
			printk("%02x ",skb2->data[i]);
			i++;                     
		}
		printk("=====%s:%d skb2 head room is %d ====\n",__func__, __LINE__, (skb2->data - skb2->head));
		printk("\n\n\n");
		
		#endif
		capwap_fragment++;	
		netif_receive_skb(skb2);
	
	}
	//kfree_skb(skb);

}


static int tunnel8023_add_zt_header(struct sk_buff *skb)
{
	
	struct iphdr * ipheader = NULL;
	//struct tunnel8023_header * thdr = NULL;
	unsigned short * thdr = NULL;
	__be16 not_last_frag = 0;
	unsigned int packet_len = 0;

    /*Begin:added  by zhanghu to kill the bug of the system handled up 2012-10-30 AZT-31*/
    unsigned short * packet_type = NULL;
    packet_type = (unsigned short *)(skb->data + 16); //16 = mac(12 Bytes) + tag(4 Bytes)
    /*End:added  by zhanghu to kill the bug of the system handled up 2012-10-30 AZT-31*/
    
		//ipheader = (struct tunnel8023_header *)(skb->data + ETH_HLEN + 4);
    if(*packet_type == 0x0800U)  /*added  by zhanghu to kill the bug of the system handled up 2012-10-30 AZT-31*/
    
    {
	#if LINUX_VERSION_CODE <= KERNEL_VERSION(2,6,22)
		ipheader = (struct iphdr *)(skb->data + 18); //mac + vlan = 18 from vap 
	#else
		ipheader = (struct iphdr *)(skb->data + 18);
	#endif
	
	#if 0
	{
		printk("====skb->dev->name[%s] skb->protocol[%x] tunnel8023_udp_port[%d]=======\n",
			skb->dev->name, skb->protocol,tunnel8023_udp_port);
		skb->zt_frame = 8023;
		skb->zt_id = ++zt_id ;
		printk("======%s:%d  frame_id[%u] print frame from wlan-driver============\n",
			__func__, __LINE__,skb->zt_id);
		
		/*int i =0;
		while (i <= 18)
		{
			if ((0 != (i%8)) || (0 == i))
			{
				printk("%02x ",skb->data[i]);
				i++;
				continue;
			}

			printk("\n");
			printk("%02x ",skb->data[i]);
			i++;                     
		}
		printk("\n\n\n");*/
	}
	#endif
	
	//add tunnel 8023-header and the lenth is 6 Bytes
	if((skb->data - skb->head) < 48)
	{
		printk("=====skb header room is not enough==========\n");
		return -1;

	}
	thdr =  (unsigned short *)skb_push(skb, 6);
	memset((void *)thdr , 0, 6);
	
	not_last_frag = ipheader->frag_off & htons(IP_MF); //tunel 8023 header

	if((ipheader->frag_off != 0) && (not_last_frag != 0))
	{
		*thdr = htons(0x4000);  

	}
	else
	{
		*thdr=0x0000;
	}
	// pading STASID
    }
    
    /*Begin:added  by zhanghu to kill the bug of the system handled up 2012-10-30 AZT-31*/
    else
    {
        thdr =  (unsigned short *)skb_push(skb, 6);
    	memset((void *)thdr , 0, 6); 
        *thdr=0x0000;
    }
    /*End:added  by zhanghu to kill the bug of the system handled up 2012-10-30 AZT-31*/

	*((unsigned int *)(thdr+1)) = skb->sta_sid;
	#if 0
		if ((ipheader->frag_off != 0) && (not_last_frag != 0))
		{
			printk("======%s:%d tunnel header========\n", __func__, __LINE__);
			int i =0;
			while (i <= 24)
			{
				if ((0 != (i%8)) || (0 == i))
				{
					printk("%02x ",skb->data[i]);
					i++;
					continue;
				}
			printk("\n");
			printk("%02x ",skb->data[i]);
			i++;
			}
			printk("\n\n\n");
		}		
	#endif

	tunnel8023_udp_ip(skb);
    packet_len = skb->len;
    tunnel8023_bytes_send += packet_len;
	return 0;
}

static int tunnel8023_fast_send(struct sk_buff *skb) 
{
 	unsigned char tos = 0;
	unsigned short ip_type = 0;
	struct ieee80211vap *vap =  NULL;
	
	if (!tunnel8023_up) 
	{   // is the tunnel8023 open ?  provided by capwap
		return -1;
	}
	
	if (ac_addr == 0) {  // Do i get  address of AC ? provided by capwap
		//printk(KERN_ALERT "tunnel8023: AC ip-addr is null\n");
		return -1;
	}
	#if 0
	
	((unsigned char *)&ac_addr)[0] = htons(0x0a);
	((unsigned char *)&ac_addr)[1] = htons(0x0a);
	((unsigned char *)&ac_addr)[2] = htons(0x0a);
	((unsigned char *)&ac_addr)[3] = htons(0x01);
	tunnel8023_udp_port = 5248;
	#endif
	if (strncmp(skb->dev->name, "ath", 3) == 0) {

		;

	} else {
		//printk("=====%s : %d skb->dev->name = %s========\n", __func__,__LINE__,skb->dev->name);
		return -1;
	}
	if (skb->protocol == htons(0x0030))
	{
		;
	}
	else
	{
		//printk(KERN_ALERT "===tunnel8023: skb->protocol != 0x30 [%x]===\n",
		//		skb->protocol);
		return -1;
	}

	#if LINUX_VERSION_CODE <= KERNEL_VERSION(2,6,22)
		vap = skb->dev->priv;
	#else
		vap = get_vap_from_netdev(skb->dev); 
	#endif	
	if (vap != NULL && vap->vap_splitmac != 2) 
	{ 
																	
		return -1;
	}

	if(tunnel8023_udp_port == 0)
	{
		return -1;
	}

	if((skb->len + 34) > 1500)
	{
		//printk("======%s : %d Get a big packet ==========\n",__func__, __LINE__);
		
        /*Begin:added  by zhanghu to kill the bug of the system handled up 2012-10-30 AZT-31*/
        unsigned short * packet_type = NULL;
        packet_type = (unsigned short *)(skb->data + 16); //16 = mac(12 Bytes) + tag(4 Bytes)
        if( 0x0800U != *packet_type){
            return -1;
        }
        /*End:added  by zhanghu to kill the bug of the system handled up 2012-10-30 AZT-31*/
        
		ip_fragment_tunnel8023(skb);
		skb->protocol = htons(0x0040); //it will free in netif_receive_skb()
	}
	else
	{
		
	//	printk("======%s : %d Get a small packet ==========\n",__func__, __LINE__);
		tunnel8023_add_zt_header(skb);
	}

	return 0;

}

extern __be16 eth_type_trans(struct sk_buff *skb, struct net_device *dev);


static int tunnel8023_fast_recv(struct sk_buff *skb) 
{

	struct iphdr * ip = NULL;
	struct udphdr * udp = NULL;
	__be16 srcPort = 0;
	unsigned char * protocol = NULL;
	unsigned char * sub_frame_header = NULL;
	unsigned char * sub_frame_eth = NULL;
	if (!tunnel8023_down)
	{
		return -1;
	}

	if (strncmp(skb->dev->name, "eth", 3) == 0) 
	{
		;

	} 
	else 
	{
		//printk(" ===%s : %d skb->dev->name = %s ====\n",
		//		__func__, __LINE__, skb->dev->name);
		return -1;
	}
	#if LINUX_VERSION_CODE <= KERNEL_VERSION(2,6,22)
	protocol = (((uint8_t *)skb->mac.raw) + 12); //get TPID of vlan 
	#else
	protocol = (((uint8_t *)skb->mac_header) + 12);
	#endif
	
	#if 0
	if( *((unsigned short *) protocol) == 0x8100) //The frame from AC  include vlan-tag
	{
		#if LINUX_VERSION_CODE <= KERNEL_VERSION(2,6,22)
		if((skb->tail - skb->mac.raw) < 52)
		#else
		if((skb->tail - skb->mac_header) < 52)
		#endif
		{
			return -1;
		}
		#if LINUX_VERSION_CODE <= KERNEL_VERSION(2,6,22)
		sub_frame_header = (uint8_t *)( (uint8_t *)skb->mac.raw + 52);
		ip = (struct iphdr *) (( uint8_t *)skb->mac.raw + 18);
		#else
		sub_frame_header = (uint8_t *)( (uint8_t *)skb->mac_header + 52);//mac+tag+ip+udp+ tunnel(6)
		ip = (struct iphdr *) (( uint8_t *)skb->mac_header + 18);
		#endif
		
		if(ip->protocol == 17)
		{
			udp = (struct udphdr *)((uint8_t *)ip +(ip->ihl << 2));
			srcPort = udp->source;
		}
		else
		{
			return -1;
		}

		if((srcPort == tunnel8023_udp_port) && (ip->saddr == ac_addr))
		{
			;
		}
		else
		{
			return -1;
		}
		#if 0
		{
		printk("=====print frame header(18 bytes) from AC :udp-port[%d]=====\n",srcPort);
		int i =0;
		while (i <= 17)
		{
			if ((0 != (i%8)) || (0 == i))
			{
				#if LINUX_VERSION_CODE <= KERNEL_VERSION(2,6,22)
				printk("%02x ",skb->mac.raw[i]);
				#else
				printk("%02x ",skb->mac_header[i]);
				#endif
				i++;
				continue;
			}

			printk("\n");
			#if LINUX_VERSION_CODE <= KERNEL_VERSION(2,6,22)
			printk("%02x ",skb->mac.raw[i]);
			#else
			printk("%02x ",skb->mac_header[i]);
			#endif
			i++;                     
		}
		printk("\n\n\n");
		}
		#endif
		
		//copy tag  to sub-frame
		#if LINUX_VERSION_CODE <= KERNEL_VERSION(2,6,22)
		memcpy((uint8_t *)((uint8_t *)sub_frame_header+12), (uint8_t *)((uint8_t *)skb->mac.raw+12), 4);
		#else
		memcpy((uint8_t *)((uint8_t *)sub_frame_header+12), (uint8_t *)((uint8_t *)skb->mac_header+12), 4);
		#endif
		//skb->head = sub_frame_header;
		skb->data = sub_frame_header;
		skb->len = skb->tail - skb->data;
		skb->protocol = eth_type_trans(skb, skb->dev);

		#if 0
		{
		printk("=====print sub-frame header(18 bytes) =====\n");
		int i =0;
		while (i <= 17)
		{
			if ((0 != (i%8)) || (0 == i))
			{
				#if LINUX_VERSION_CODE <= KERNEL_VERSION(2,6,22)
				printk("%02x ",skb->mac.raw[i]);
				#else
				printk("%02x ",skb->mac_header[i]);
				#endif
				i++;
				continue;
			}

			printk("\n");
			#if LINUX_VERSION_CODE <= KERNEL_VERSION(2,6,22)
			printk("%02x ",skb->mac.raw[i]);
			#else
			printk("%02x ",skb->mac_header[i]);
			#endif
			i++;                     
		}
		printk("\n\n\n");
		}
		#endif
		
		

		
		

	}
	//else //The frame from AC donnt include vlan-tag
	#endif
	{
		#if LINUX_VERSION_CODE <= KERNEL_VERSION(2,6,22)
		if((skb->tail - skb->mac.raw) < 48)
		#else
		if((skb->tail - skb->mac_header) < 48)
		#endif
		{
			return -1;
		}
		#if LINUX_VERSION_CODE <= KERNEL_VERSION(2,6,22)
		sub_frame_header = (uint8_t *)( (uint8_t *)skb->mac.raw + 48);//mac+ip+udp+ tunnel(6)
		ip = (struct iphdr *) (( uint8_t *)skb->mac.raw + 14);
		#else
		sub_frame_header = (uint8_t *)( (uint8_t *)skb->mac_header + 48);//mac+ip+udp+ tunnel(6)
		ip = (struct iphdr *) (( uint8_t *)skb->mac_header + 14);
		#endif
		
		if(ip->protocol == 17)
		{
			udp = (struct udphdr *)((uint8_t *)ip +(ip->ihl << 2));
			srcPort = udp->source;
			
		}
		else
		{
			return -1;
		}
		
		if(srcPort == tunnel8023_udp_port)
		{
			;
		}
		else
		{
			return -1;
		}

		#if 0
		{
		printk("=====%s:%d print frame header(18 bytes) from AC :udp-port[%d]=====\n",__func__, __LINE__,srcPort);
		int i =0;
		while (i <= 17)
		{
			if ((0 != (i%8)) || (0 == i))
			{
				#if LINUX_VERSION_CODE <= KERNEL_VERSION(2,6,22)
				printk("%02x ",skb->mac.raw[i]);
				#else
				printk("%02x ",skb->mac_header[i]);
				#endif
				i++;
				continue;
			}

			printk("\n");
			#if LINUX_VERSION_CODE <= KERNEL_VERSION(2,6,22)
			printk("%02x ",skb->mac.raw[i]);
			#else
			printk("%02x ",skb->mac_header[i]);
			#endif
			i++;                     
		}
		printk("\n\n\n");
		}
		#endif
		
		
		sub_frame_eth = sub_frame_header - 16; // store sub-frame mac
		//printk("=========%s:%d===============\n",__func__,__LINE__);
		memcpy(sub_frame_eth, sub_frame_header, 12);
		//skb->data = sub_frame_header;
		//printk("=========%s:%d===============\n",__func__, __LINE__);
		//skb->data = skb->head = sub_frame_header + 4; //remove tag in sub-frame
		skb->data = sub_frame_header + 4; //remove tag in sub-frame
		skb->len = skb->tail - skb->data;
		memcpy(skb->data, sub_frame_eth, 12);
		//printk("=========%s:%d===============\n",__func__, __LINE__);
		skb->protocol = eth_type_trans(skb, skb->dev);
		//printk("=========%s:%d===============\n",__func__, __LINE__);

		#if 0
			{
		printk("=====print sub-frame header(18 bytes) from AC=====\n");
		int i =0;
		while (i <= 17)
		{
			if ((0 != (i%8)) || (0 == i))
			{
				#if LINUX_VERSION_CODE <= KERNEL_VERSION(2,6,22)
				printk("%02x ",skb->mac.raw[i]);
				#else
				printk("%02x ",skb->mac_header[i]);
				#endif
				i++;
				continue;
			}

			printk("\n");
			#if LINUX_VERSION_CODE <= KERNEL_VERSION(2,6,22)
			printk("%02x ",skb->mac.raw[i]);
			#else
			printk("%02x ",skb->mac_header[i]);
			#endif
			i++;                     
		}
		printk("\n\n\n");
			}
		#endif
			
	}
		

	#if 0
	printk("%s : %d :print frame included station mac+valn from AC \n",__func__, __LINE__);
	int i =0;
	while (i <= 17)
	{
		if ((0 != (i%8)) || (0 == i))
		{
			printk("%02x ",skb->data[i]);
			i++;
			continue;
		}

		printk("\n");
		printk("%02x ",skb->data[i]);
		i++;                     
	}
	printk("\n\n\n");
	#endif

	tunnel8023_bytes_recv += skb->len;

}

//liuqiang ZT tunnel 8023

static int __init
init_capwap_splide_fast(void)
{	
	capwap_splide_fast_send_hook = NULL;//= capwap_splide_fast_send;
	capwap_splide_fast_recv_hook = NULL;//= capwap_splide_fast_recv;
	//liuqiang ZT tunnel 8023
	tunnel8023_fast_send_hook = tunnel8023_fast_send;
	tunnel8023_fast_recv_hook = tunnel8023_fast_recv;
	/*Begin: added by liuqiang for get AC mac address [2012-9-18]*/
	capwap_get_uplink_mac_hook = NULL; 
	/*End: added by liuqiang for get AC mac address [2012-9-18]*/
	printk("==============capwap_forward.ko is OK=============\n");
	//liuqiang  ZT tunnel 8023
	capwap_sysctl_attach();
	init_mac_ip_list();
	return 0;
}
module_init(init_capwap_splide_fast);

static void __exit
exit_capwap_splide_fast(void)
{	
	capwap_splide_fast_send_hook = NULL;
	capwap_splide_fast_recv_hook = NULL;
	//liuqiang  ZT tunnel 8023
	tunnel8023_fast_send_hook = NULL;
	tunnel8023_fast_recv_hook = NULL;
	//liuqiang  ZT tunnel 8023
	capwap_sysctl_detach();

}
module_exit(exit_capwap_splide_fast);


MODULE_AUTHOR("xmeng@autelan");
MODULE_DESCRIPTION("Support for thinap splide mac mode ");
#ifdef MODULE_LICENSE
MODULE_LICENSE("GPL");
#endif



