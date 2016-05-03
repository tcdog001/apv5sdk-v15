
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
#include <linux/if_arp.h>
#if((LINUX_VERSION_CODE == KERNEL_VERSION(3,0,6)) || (LINUX_VERSION_CODE == KERNEL_VERSION(2,6,35)))//liuqaing add
#include <net/route.h>
#endif
//#include "../../os/linux/src/osif_private.h"
#define CAPWAP_HEADER_LEN 8
#define AC_DATA_PORT 5247 
#define IPPROTO_UDP 17
//liuqiang modify

#ifdef PRISM2_HEADER
#define PRISM2_HEADER_LENS 144
#else
#define PRISM2_HEADER_LENS 0
#endif

//liuqiang end
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

//tunnel 802.3 packets statistics
static unsigned long long tunnel8023_bytes_send = 0;
static unsigned long long tunnel8023_bytes_recv = 0;


//
/*Begin:added by duanmingzhe wmm to ip header 2010-08-28*/
static unsigned long wmm_to_tos= 0;
/*End:added by duanmingzhe wmm to ip header 2010-08-28*/
/*<begin : transplant by caizhibang from apv5*/
/*yanggs add for tunnel mode wds 2012-01-14*/
static int wds_tunnel = 0;
/*yanggs add for tunnel mode wds end 2012-01-14*/
/*end : transplant by caizhibang from apv5>*/

extern int pro_file_sche_flag;


/*chenxf add for tunnel forwarding optimization up direction 2012-04-07*/
extern unsigned short int dest_ac_mac_flag;
extern unsigned char      dest_ac_mac[6];
/*Begin: liuqiang modifed for selecting out-device flexible in capwap module 2012-12-06*/
extern unsigned char out_dev_name[IFNAMSIZ];
/*End: liuqiang modifed for selecting out-device flexible in capwap module 2012-12-06*/

#define AC_CONTROL_PORT 5246

/*chenxf add end 2012-04-07*/

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
	int  val = 0;
	unsigned short wlan_id_bitmap = 0;
	struct iphdr *ip = NULL;
	struct udphdr * udp = NULL; 
	char buffer[10];
	struct net_device *to = NULL;
	/*Begin modified by liuqiang for local and tunnel coexistence 2012-11-22*/
	unsigned short tunnel_type = 0; //tunnel 802.11 or 802.3
	/*End modified by liuqiang for local and tunnel coexistence 2012-11-22*/
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
	
#if defined(LTEFI) || defined(LTEFI_V2)
	if (strncmp(skb->dev->name, "eth", 3) == 0||strncmp(skb->dev->name, "wan", 3) == 0||strncmp(skb->dev->name, "usb", 3) == 0) {
#else
	if (strncmp(skb->dev->name, "eth", 3) == 0) {
#endif
		;

	} else {

		return -1;
	}
	}
	/*yanggs add for tunnel mode wds end 2012-01-14*/
	/*end : transplant by caizhibang from apv5>*/
	/*check this skb is for host or not*/
	if (skb->pkt_type == PACKET_HOST || skb->pkt_type == PACKET_OTHERHOST) {
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
    
	if (udp->dest == htons(udp_port)) {
		size += 8;
	} else {
		return -1;
	}

	/*check if the ip is fragment or not*/
	if (ip->frag_off != 0) {
        
		if ( !(ip->frag_off& 0x4000)) {
		/*begin:wangyu add these lines for capwap test*/
			if ( skb->len > 62 ){
				bytes_recv += ( skb->len - 62 );
				}
			else{
				printk("%d ip_frag ::bytes_recv:the skb-> len is %d.....\n",__LINE__,skb->len);
				bytes_recv += skb->len;
			}
		/*end:wangyu add these lines for capwap test*/
			ip_fragment ++;
			//printk("the ip package fragment, trun it to ip level \n");
			return  -1;
		} 

	}
	
	/*check if capwap is fregment or not */

	per = (unsigned *)((uint8_t *)skb->data+size);// per point to capwap header
	if ( (*per) & 0x00000080) { //is "F" filed 0 ?
		/*begin:wangyu add these lines for capwap test*/
			if ( skb->len > 62 ){
				bytes_recv += ( skb->len - 62 );
				}
			else{
				printk("%d ip_frag ::bytes_recv:the skb-> len is %d.....\n",__LINE__,skb->len);
				bytes_recv += skb->len;
			}
		/*end:wangyu add these lines for capwap test*/
		capwap_fragment ++;
		//printk("-cap wap fragment, return it to wtpd :: %#X", *per);
		return -1;
	}

	/*check ip header*/
	
	per=(unsigned *)((uint8_t *)skb->data+size);//per point to capwap header
	
	#if 0
	{
		
		unsigned char * tmp = (unsigned char *)per;
		int i =0;
		printk("udp->dest = %d,udp_port = %d\n",udp->dest,htons(udp_port));
		printk("==== %s:%d capwap header ==============\n", __func__, __LINE__);
		while (i <= 16)
		{
			if ((0 != (i%8)) || (0 == i))
			{
				printk("%02x ",tmp[i]);
				i++;
				continue;
			}

			printk("\n");
			printk("%02x ",tmp[i]);
			i++;                     

		}
			printk("\n\n\n");
	}

	#endif

        val = *(unsigned int *)per;
        val = ntohl(val);//endian issue.
	if((val) & 0x0100U) //  "T" is not 0 that stand for 802.11 tunnel 
	{
		//printk("====%s:%d walk through 802.11 tunnel ======\n",__func__, __LINE__);
		//skb_len - ip - udp - capwap - 80211 - LLC
		/*begin:wangyu add these lines for capwap test*/
		if ( skb->len > 62 ){
			packet_len = ( skb->len - 62 );
			}
		else{
			printk("%d default ::bytes_recv:the skb->len is %d.....\n",__LINE__,skb->len);
			packet_len = skb->len;
		}
		/*end:wangyu add these lines for capwap test*/

	    bytes_recv += packet_len;
		/*Begin modified by liuqiang for local and tunnel coexistence 2012-11-22*/
		tunnel_type = htons(0x0019);
		/*End modified by liuqiang for local and tunnel coexistence 2012-11-22*/
	}
	else //802.3 tunnel
	{
		//printk("====%s:%d walk through 802.3 tunnel ======\n",__func__, __LINE__);
		/*Begin modified by liuqiang for local and tunnel coexistence 2012-11-22*/
		tunnel_type = htons(0x0030);
		/*End modified by liuqiang for local and tunnel coexistence 2012-11-22*/
		/*begin:wangyu add these lines for capwap test*/
		if ( skb->len > 44 ){
			packet_len = ( skb->len - 44 );
			}
		else{
			printk("%d default ::bytes_recv:the skb->len is %d.....\n",__LINE__,skb->len);
			packet_len = skb->len;
		}
		/*end:wangyu add these lines for capwap test*/
	    tunnel8023_bytes_recv += packet_len;
		
	}

	radioId=((val)>>14)&0x1f; /* pei add at 0327 for 2 radio */
	/* end */
	/*compute the capwap header length*/
	size += 8;
	/*get the  wlan NO.*/
	per = (unsigned *)((uint8_t *)skb->data+size); // per point to "Radio MAC" in capwap
	memset(buffer, 0x00, 10);
	//sprintf(buffer, "ath%d", (*per)>>24);
	wlan_id_bitmap = *(unsigned char *)(per);
	//sprintf(buffer, "ath.%d-%d", radioId, (*per)>>24);	 /* pei modified at 0327 */
        sprintf(buffer, "ath.%d-%d", radioId, wlan_id_bitmap);	 /* pei modified at 0327 */
	#if 0
		printk("====%s:%d : @athX name@ is : %s ======\n", __func__, __LINE__, buffer);
	#endif
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
	
	#if LINUX_VERSION_CODE <= KERNEL_VERSION(2,6,22)
		vap = skb->dev->priv;
	#else
		vap = get_vap_from_netdev(to); //Added by duanmingzhe for get vap from netdev
	#endif
	if(vap->vap_splitmac == 0) {

		dev_put(to);
		return -1;
	}
	
	size += 8;
	skb_pull(skb, size); //skb->data point to capwap payload

#if LINUX_VERSION_CODE <= KERNEL_VERSION(2,6,20)
	skb->nh.raw = skb->data;
#else
	skb->network_header= skb->data;
#endif
	skb->dev = to;
	/*Begin modified by liuqiang for local and tunnel coexistence 2012-11-22*/
	skb->protocol = tunnel_type;
	/*End modified by liuqiang for local and tunnel coexistence 2012-11-22*/
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
	unsigned char *radio_mac = NULL;
	struct ieee80211_frame * h;

	if((skb->protocol == htons(0x0019)) || (skb->protocol == htons(0x0020))) //add capwap header for 802.11 tunnel
	{

		/*check the 80211 frame*/
		#ifdef PRISM2_HEADER
		if (skb->len <= PRISM2_HEADER_LENS) 
		{
			return -1;
		}
		#endif
		//if (skb->len > 1500) { /*check if this package need fragment or not*/
			
		//	return -1;
		//} 
		#ifdef PRISM2_HEADER
		skb_pull(skb, PRISM2_HEADER_LENS);
		//printk("====skb_pull(skb, PRISM2_HEADER_LENS)====\n");
		#endif
	
		type = skb->data;

		if (!((*type)& 0x08)) { /*check 80211 frame is data or not */
			#ifdef PRISM2_HEADER
			skb_push(skb, PRISM2_HEADER_LENS);
			//printk("====skb_push(skb, PRISM2_HEADER_LENS)====\n");
			#endif
			return -1;
		} 
		
		h = (struct ieee80211_frame *) type; //skb->data point to 802.11 MAC header, type = skb->data
		
		if (skb->protocol == htons(0x0020)) { /*EAP to wtpd in */
			printk("===a eap packet at capwap_splide_fast_send tunnel 802.11 =====\n");
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
	}
	else //add capwap header for 802.3 tunnel
	{
		struct ethhdr *eth = NULL;
		eth = skb_pull(skb, 6); // remove BSSID
		if( skb->protocol == htons(0x0031)) /*EAP to wtpd in */
		{
			printk("===a eap packet at capwap_splide_fast_send tunnel 802.3 =====\n");
			skb->protocol = htons(0x0030);
			dest_ac_addr = ac_addr;
		}
		else
		{
			dest_ac_addr = find_mac_ip(eth->h_source);
		}
		 
		skb_push(skb, 16);
		memset(skb->data, 0x00, 16);
		cw = (unsigned short *) skb->data;
		*cw = htons(0x0020);

		cw = cw +1;

		*cw = htons(0x0010); // "T" feild is  "0"  that indicate that capwap payload is 802.3 frame.
							//"M" is 1, "W" is 0
		cw =cw + 3; //cw point to Radio MAC
		radio_mac = (unsigned char *)cw; 
		*radio_mac = htons(0x06);// Radio Mac length is 6;
		radio_mac = radio_mac + 1;
		memcpy(radio_mac, skb->dev->dev_addr, 6); //copy athX  MAC to "Radio MAC" optional in capwap header
	
	}

	return 0;

}

extern int ip_route_input (struct sk_buff *skb, unsigned int source, unsigned int dest, unsigned char tos,  struct net_device *dev);

/* this function used to add ip and udp header for capwap splide mac fast*/
int capwap_udp_ip (struct sk_buff *skb,unsigned char tos,	unsigned short ip_type)
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
	udph->check = csum_tcpudp_magic((local_ip),
					(dest_ip),
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
	//put_unaligned(htonl(local_ip), &(iph->saddr));
	//put_unaligned(htonl(dest_ip), &(iph->daddr));
	put_unaligned(local_ip, &(iph->saddr));
	put_unaligned(dest_ip, &(iph->daddr));  
	
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
	#if !((LINUX_VERSION_CODE == KERNEL_VERSION(3,0,6)) || (LINUX_VERSION_CODE == KERNEL_VERSION(2,6,35)))//liuqaing add	
	if ((struct  dst_entry *)(skb->_skb_dst) == NULL)
	#endif
	#endif
	if((skb->len - 14) > 1500 || dest_ac_mac_flag != 2)
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
				
		ip_route_input(skb, iph->daddr, sip, iph->tos,skb->dev);
		skb_pull(skb, ETH_HLEN);
		
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
		return 0;
		
	}
	else
	{
		struct net_device *eth_dev = NULL ;
		#if 0	
		printk("====%s:%d AC MAC is : %2x:%2x:%2x:%2x:%2x:%2x ====\n", __func__, __LINE__,
				dest_ac_mac[0],dest_ac_mac[1],dest_ac_mac[2],dest_ac_mac[3],
				dest_ac_mac[4],dest_ac_mac[5]);
		#endif
		memcpy(eth->h_dest, dest_ac_mac, 6);
		/*Begin: liuqiang modifed for selecting out-device flexible in capwap module 2012-12-06*/
		#if LINUX_VERSION_CODE <= KERNEL_VERSION(2,6,22)
			eth_dev = dev_get_by_name(out_dev_name);
		#else			
			eth_dev = dev_get_by_name(&init_net,out_dev_name);
		#endif
		/*End: liuqiang modifed for selecting out-device flexible in capwap module 2012-12-06*/
		skb->dev = eth_dev;
		skb_pull(skb, ETH_HLEN);
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
		
		skb_push(skb, ETH_HLEN);
		dev_queue_xmit(skb);
		return 50;   //representation directly send to eth
	}

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
	int re_val = 0;
	if (!spfast_up) {
		
		return -1;
	}
	if (skb->protocol == htons(0x0019) || skb->protocol == htons(0x0020)
		 || skb->protocol == htons(0x0030) || skb->protocol == htons(0x0031)) { //0x300 stand for 802.3 tunnel
		
		;
	} else {
			return -1;
	}
	/*chech the skb dev, if it does not come from eth0, we do nothing*/
	if (strncmp(skb->dev->name, "ath", 3) == 0) {

		;//printk("--function: capwap_splide_fast_send, dev = ath \n");

	} else {
		
		return -1;
	}
	#if LINUX_VERSION_CODE <= KERNEL_VERSION(2,6,22)
		vap = skb->dev->priv;
	#else
	/*added by duanmingzhe for get vap from netdev*/
	    vap = get_vap_from_netdev(skb->dev);
	
	#endif
	/*Begin: yanggongsheng modify for wds client vlan 2013-01-07 PCAPVXNN-86*/
	if(vap == NULL)
	{
		return -1;
	}
	/*End: yanggongsheng modify for wds client vlan 2013-01-07 PCAPVXNN-86*/
	if (vap->vap_splitmac == 0) { 
			
			return -1;
	}	
		
	if (ac_addr == 0) {

		return -1;
	}
    if(wmm_to_tos && (skb->protocol == htons(0x0019)))
    {
	/*Begin:added by duanmingzhe wmm to ip header 2010-08-28*/
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

	//Begin:liuqiand add for 802.3 tunnel
	if(wmm_to_tos && (skb->protocol == htons(0x0030)))
	{
		memcpy(&ip_type, skb->data + 18, 2); //18 = BSSID + Dest_MAC + Src_MAC
		memcpy(&tos, skb->data + ETH_HLEN + 6 + 1, 1); 
	}

	//END:liuqiand add for 802.3 tunnel
	
	/*add capwap header to skb*/
	if (capwap_header_add(skb) < 0) {
		
		return -1;
	}
	if(skb->protocol == htons(0x0019))
	{
		/*begin:wangyu add these lines for capwap test*/
		if ( skb->len > 62 ){
	    	packet_len = skb->len - 62;
			}
		else{
			printk("%d ::bytes_send:the skb-> len is less 62.....\n",__LINE__);
			packet_len = skb->len;
		}
		/*end:wangyu add these lines for capwap test*/
	    bytes_send += packet_len ;
	}
	else //skb->protocol = 0x0030
	{
		/*begin:wangyu add these lines for capwap test*/
		if ( skb->len > 44 ){
			packet_len = skb->len - 44; // skb->len - ip - udp - capwap
			}
		else{
			printk("%d ::bytes_send:the skb-> len is less 62.....\n",__LINE__);
			packet_len = skb->len;
		}
		/*end:wangyu add these lines for capwap test*/

		tunnel8023_bytes_send += packet_len ;
	}
	
	re_val = capwap_udp_ip(skb,tos,ip_type);
	return re_val;

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
		{
			ac_addr = val;
			/*chenxf add for tunnel forwarding optimization up direction 2012-04-07*/
			dest_ac_mac_flag = 0;
			memset(dest_ac_mac,0,6);
			/*chenxf add end 2012-04-07*/
		}
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
	#if !((LINUX_VERSION_CODE == KERNEL_VERSION(3,0,6)) || (LINUX_VERSION_CODE == KERNEL_VERSION(2,6,35)))//liuqaing add
	  .ctl_name	= CTL_AUTO,
	#endif
	  .procname	= "udp_port",
	  .mode		= 0644,
	  .proc_handler	= udp_port_set
	},
	{ 
	#if !((LINUX_VERSION_CODE == KERNEL_VERSION(3,0,6)) || (LINUX_VERSION_CODE == KERNEL_VERSION(2,6,35)))//liuqaing add	
	  .ctl_name	= CTL_AUTO,
	#endif	  
	  .procname	= "spfast_down",
	  .mode		= 0644,
	  .proc_handler	= spfast_down_set
	},

	{ 
	#if !((LINUX_VERSION_CODE == KERNEL_VERSION(3,0,6)) || (LINUX_VERSION_CODE == KERNEL_VERSION(2,6,35)))//liuqaing add	
	  .ctl_name	= CTL_AUTO,
	#endif
	  .procname	= "spfast_up",
	  .mode		= 0644,
	  .proc_handler	= spfast_up_set
	},

	{ 
	#if !((LINUX_VERSION_CODE == KERNEL_VERSION(3,0,6)) || (LINUX_VERSION_CODE == KERNEL_VERSION(2,6,35)))//liuqaing add
	  .ctl_name	= CTL_AUTO,
	#endif
	  .procname	= "ip_fragment",
	  .mode		= 0644,
	  .proc_handler	= ip_fragment_set
	},
	{ 
	#if !((LINUX_VERSION_CODE == KERNEL_VERSION(3,0,6)) || (LINUX_VERSION_CODE == KERNEL_VERSION(2,6,35)))//liuqaing add
	  .ctl_name	= CTL_AUTO,
	#endif
	  .procname	= "capwap_fragment",
	  .mode		= 0644,
	  .proc_handler	= capwap_fragment_set
	},
	{ 
	#if !((LINUX_VERSION_CODE == KERNEL_VERSION(3,0,6)) || (LINUX_VERSION_CODE == KERNEL_VERSION(2,6,35)))//liuqaing add
	  .ctl_name	= CTL_AUTO,
	#endif
	  .procname	= "ac_addr",
	  .mode		= 0644,
	  .proc_handler	= ac_addr_set
	},
	{ 
   #if !((LINUX_VERSION_CODE == KERNEL_VERSION(3,0,6)) || (LINUX_VERSION_CODE == KERNEL_VERSION(2,6,35)))//liuqaing add
	  .ctl_name	= CTL_AUTO,
	#endif
	  .procname	= "mac_ip_flag",
	  .mode		= 0644,
	  .proc_handler	= mac_ip_flag_set
	},
	{ 
	#if !((LINUX_VERSION_CODE == KERNEL_VERSION(3,0,6)) || (LINUX_VERSION_CODE == KERNEL_VERSION(2,6,35)))//liuqaing add	
	  .ctl_name	= CTL_AUTO,
	#endif
	  .procname	= "temp_ip_addr",
	  .mode		= 0644,
	  .proc_handler	= temp_ip_addr_set
	},
	{ 
	#if !((LINUX_VERSION_CODE == KERNEL_VERSION(3,0,6)) || (LINUX_VERSION_CODE == KERNEL_VERSION(2,6,35)))//liuqaing add
	  .ctl_name	= CTL_AUTO,
	#endif
	  .procname	= "mac0",
	  .mode		= 0644,
	  .proc_handler	= mac0_set
	},
	{ 
	#if !((LINUX_VERSION_CODE == KERNEL_VERSION(3,0,6)) || (LINUX_VERSION_CODE == KERNEL_VERSION(2,6,35)))//liuqaing add
	  .ctl_name	= CTL_AUTO,
	#endif
	  .procname	= "mac1",
	  .mode		= 0644,
	  .proc_handler	= mac1_set
	},
	{ 
	#if !((LINUX_VERSION_CODE == KERNEL_VERSION(3,0,6)) || (LINUX_VERSION_CODE == KERNEL_VERSION(2,6,35)))//liuqaing add
		.ctl_name	= CTL_AUTO,
	#endif
	  .procname	= "mac2",
	  .mode		= 0644,
	  .proc_handler	= mac2_set
	},
	{ 
	#if !((LINUX_VERSION_CODE == KERNEL_VERSION(3,0,6)) || (LINUX_VERSION_CODE == KERNEL_VERSION(2,6,35)))//liuqaing add
		.ctl_name	= CTL_AUTO,
	#endif
	  .procname	= "mac3",
	  .mode		= 0644,
	  .proc_handler	= mac3_set
	},
	{ 
	#if !((LINUX_VERSION_CODE == KERNEL_VERSION(3,0,6)) || (LINUX_VERSION_CODE == KERNEL_VERSION(2,6,35)))//liuqaing add
		.ctl_name	= CTL_AUTO,
	#endif
	  .procname	= "mac4",
	  .mode		= 0644,
	  .proc_handler	= mac4_set
	},
	{ 
	#if !((LINUX_VERSION_CODE == KERNEL_VERSION(3,0,6)) || (LINUX_VERSION_CODE == KERNEL_VERSION(2,6,35)))//liuqaing add
		.ctl_name	= CTL_AUTO,
	#endif
	  .procname	= "mac5",
	  .mode		= 0644,
	  .proc_handler	= mac5_set
	},
//chenxf added for bytes_recv & bytes_send supporting 64bit 2011-11-16
#if (!defined(PC018) && !defined(APM82181))
	{
	#if !((LINUX_VERSION_CODE == KERNEL_VERSION(3,0,6)) || (LINUX_VERSION_CODE == KERNEL_VERSION(2,6,35)))//liuqaing add
	  .ctl_name = CTL_AUTO,
	#endif
	  .procname = "bytes_send",
	  .mode = 0664,
	  .proc_handler = bytes_send_set
	},
	{
	#if !((LINUX_VERSION_CODE == KERNEL_VERSION(3,0,6)) || (LINUX_VERSION_CODE == KERNEL_VERSION(2,6,35)))//liuqaing add	
	  .ctl_name = CTL_AUTO,
	#endif
	  .procname = "tunnel8023_bytes_send",
	  .mode = 0664,
	  .proc_handler = tunnel8023_bytes_send_set
	},
	{
	#if !((LINUX_VERSION_CODE == KERNEL_VERSION(3,0,6)) || (LINUX_VERSION_CODE == KERNEL_VERSION(2,6,35)))//liuqaing add
		.ctl_name = CTL_AUTO,
	#endif
      .procname = "tunnel8023_bytes_recv",
      .mode = 0664,
      .proc_handler = tunnel8023_bytes_recv_set
    },
	{
	#if !((LINUX_VERSION_CODE == KERNEL_VERSION(3,0,6)) || (LINUX_VERSION_CODE == KERNEL_VERSION(2,6,35)))//liuqaing add
		.ctl_name = CTL_AUTO,
	#endif
      .procname = "bytes_recv",
      .mode = 0664,
      .proc_handler = bytes_recv_set
    },
#else
	{
	#if !((LINUX_VERSION_CODE == KERNEL_VERSION(3,0,6)) || (LINUX_VERSION_CODE == KERNEL_VERSION(2,6,35)))//liuqaing add
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
    {
	#if !((LINUX_VERSION_CODE == KERNEL_VERSION(3,0,6)) || (LINUX_VERSION_CODE == KERNEL_VERSION(2,6,35)))//liuqaing add
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
	#if !((LINUX_VERSION_CODE == KERNEL_VERSION(3,0,6)) || (LINUX_VERSION_CODE == KERNEL_VERSION(2,6,35)))//liuqaing add
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
	{
	#if !((LINUX_VERSION_CODE == KERNEL_VERSION(3,0,6)) || (LINUX_VERSION_CODE == KERNEL_VERSION(2,6,35)))//liuqaing add
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
	#if !((LINUX_VERSION_CODE == KERNEL_VERSION(3,0,6)) || (LINUX_VERSION_CODE == KERNEL_VERSION(2,6,35))) //liuqaing add
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
	#if !((LINUX_VERSION_CODE == KERNEL_VERSION(3,0,6)) || (LINUX_VERSION_CODE == KERNEL_VERSION(2,6,35)))//liuqaing add
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

#if ((LINUX_VERSION_CODE == KERNEL_VERSION(3,0,6)) || (LINUX_VERSION_CODE == KERNEL_VERSION(2,6,35)))
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
		|| LINUX_VERSION_CODE == KERNEL_VERSION(3,0,6) || LINUX_VERSION_CODE == KERNEL_VERSION(2,6,35))
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
#if (LINUX_VERSION_CODE != KERNEL_VERSION(2,6,31) && LINUX_VERSION_CODE != KERNEL_VERSION(2,6,36)) 
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

inline void capwap_get_uplink_mac(struct sk_buff *skb)
{
/*chenxf add for tunnel forwarding optimization up direction 2012-04-07*/
	if ( 0 == dest_ac_mac_flag )
	{
		if (skb->protocol == htons(ETH_P_IP)) 
		{
			struct iphdr *ip;
			ip = ip_hdr(skb);
			if (ip && 17 == ip->protocol)
			{
				struct udphdr * udp = (struct udphdr *)((uint8_t *)ip +(ip->ihl << 2));
				if (udp->dest == AC_CONTROL_PORT)
				{
					struct ethhdr *eth;
					eth = eth_hdr(skb);
					memcpy(dest_ac_mac, eth->h_dest, 6);
					/*Begin: liuqiang modifed for selecting out-device flexible in capwap module 2012-12-06*/
					memcpy(out_dev_name, skb->dev->name, IFNAMSIZ);
					/*End: liuqiang modifed for selecting out-device flexible in capwap module 2012-12-06*/
					dest_ac_mac_flag = 2;  //2 representation eth get the dest_ac_mac
					printk("%s: dest_ac_mac  = %02x:%02x:%02x:%02x:%02x:%02x\n",__func__,dest_ac_mac[0],\
						dest_ac_mac[1],dest_ac_mac[2],dest_ac_mac[3],dest_ac_mac[4],dest_ac_mac[5]);
				}
			}
		}
	}
/*chenxf add end 2012-04-07*/	
}

static int __init
init_capwap_splide_fast(void)
{	
	capwap_splide_fast_send_hook = capwap_splide_fast_send;
	capwap_splide_fast_recv_hook = capwap_splide_fast_recv;
	capwap_get_uplink_mac_hook = capwap_get_uplink_mac;
	#if 1
	printk("======capwap sport for tunel 80211 and tunnel 8023======\n");
	#endif
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
	capwap_get_uplink_mac_hook = NULL;	/*lijiyong add to avoid kernel panic when rmmod*/
	capwap_sysctl_detach();

}
module_exit(exit_capwap_splide_fast);


MODULE_AUTHOR("xmeng@autelan");
MODULE_DESCRIPTION("Support for thinap splide mac mode ");
#ifdef MODULE_LICENSE
MODULE_LICENSE("GPL");
#endif



