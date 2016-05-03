
/**********************************************************************************
* Copyright (c) 2008-2011  Beijing Autelan Technology Co. Ltd.
* All rights reserved.
*
* filename: dhcp_snooping_main.c
* description:  implementation for  the main body of Dhcp_snooping
* 
*
* 
************************************************************************************/

#include <pthread.h>//add by wangjr 20100105  
#include "iwlib.h"		/* Header */
#include "wireless.h"		/* Header */
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
#include <string.h>
#include <getopt.h>
#include <linux/if_ether.h>
#include <netpacket/packet.h>

#include <linux/netlink.h>  //add by wangjr 20100105
#include <linux/rtnetlink.h>//add by wangjr 20100105

#include "dhcp_snooping_if.h"
#include "dhcp_snooping_options.h"
#include "dhcp_snooping_pkt.h"
#include "dhcp_snooping_tbl.h"
#include "dhcp_snooping_main.h"

dhcp_snooping_config_t g_dhcp_snooping_config;

//#if 0  //add by wangjr 091231

/* Maximum tx powers in the range struct */
#define IW_MAX_TXPOWER		8
/* Maximum number of size of encoding token available
 * they are listed in the range structure */
#define IW_MAX_ENCODING_SIZES	8

/* Maximum bit rates in the range struct */
#define IW_MAX_BITRATES		8

/* Maximum frequencies in the range struct */
#define IW_MAX_FREQUENCIES	16


/****************************** TYPES ******************************/

/*
 * Static information about wireless interface.
 * We cache this info for performance reason.
 */
typedef struct wireless_iface
{
  /* Linked list */
  struct wireless_iface *	next;

  /* Interface identification */
  int		ifindex;		/* Interface index == black magic */

  /* Interface data */
  char			ifname[IFNAMSIZ + 1];	/* Interface name */
  struct iw_range	range;			/* Wireless static data */
  int			has_range;
} wireless_iface;



/**************************** VARIABLES ****************************/

/* Cache of wireless interfaces */
struct wireless_iface *	interface_cache = NULL;


//#endif  //add by wangjr

/*
 ******************************************************************************
 *  Function   :   static void show_usage(void)
 *  Input      :   void
 *  Output     :   void
 *  Return     :   NULL
 *  Description:   show the help information to user
 ******************************************************************************
 */
static void show_usage(void)
{
	printf(
"Usage: dhcpsnooping [OPTIONS]\n\n"
"  -t, --trust=IFNAME              Trusted interface name\n"
"  -o, --option82=OPTIONSTR        Option82 string\n"
"  -h                              help\n"
	);
	exit(0);
}

static int dhcp_snoop_is_trusted(int ifindex)
{
	if((ifindex == g_dhcp_snooping_config.trusted_ifindex)||(g_dhcp_snooping_config.trusted_ifindex == 0))
	{
		return DHCP_OK;
	}
	return DHCP_FAIL;
}

int dhcp_snoop_have_circuit_policy(void)
{
	if(strlen(g_dhcp_snooping_config.option82)>0)
	{
		return DHCP_OK;
	}
	return DHCP_FAIL;
}

void* dhcp_snoop_get_circuit_str(void)
{
	return g_dhcp_snooping_config.option82;
}

void dhcpsnoop_expired(); //add by wangjr 20100105
int main(int argc, char *argv[])
{
	int c, len;
	unsigned char *packet;
	struct sockaddr_ll sll;	
	int bytes = 0;
	int ifindex;	
	unsigned char *temp, *message;
	int ret;
	struct dhcpMessage *data;
	pthread_t pexpire;  //add by wangjr 20100105
	
	if(argc == 1)
		show_usage();

	static struct option arg_options[] = {
		{"trust",	required_argument,	0, 't'},
		{"option82", required_argument,		0, 'o'},
		{"help",	no_argument,		0, '?'},
		{0, 0, 0, 0}
	};

	/* get options */
	memset(&g_dhcp_snooping_config, 0, sizeof(dhcp_snooping_config_t));
	while (1) {
	
		int option_index = 0;
		c = getopt_long(argc, argv, "t:o:", arg_options, &option_index);
		if (c == -1) break;		
		switch (c) {
		case 't':
			len = strlen(optarg) > 31 ? 31 : strlen(optarg);
			strncpy(g_dhcp_snooping_config.trusted_interface, optarg, len);

			break;
		case 'o':
			len = strlen(optarg) > 255 ? 255 : strlen(optarg);
			printf("the option82 string is %s\r\n", optarg);
			g_dhcp_snooping_config.option82[0] = DHCP_CIRCUIT_ID;
			g_dhcp_snooping_config.option82[1] = len;
			g_dhcp_snooping_config.option82[2] = '\0';
			strncpy(g_dhcp_snooping_config.option82 + 2, optarg, len);
			printf("the option82 string is %s\r\n", g_dhcp_snooping_config.option82);
			break;
		default:
			show_usage();
		}
	}
	dhcp_snoop_tbl_initialize(); /*init the dhcp snooping item hash table*/
	dhcp_snoop_pkt_initialize(); /*init the dhcp snooping packet receive module*/
	if(strlen(g_dhcp_snooping_config.trusted_interface) > 0)
	{
		if((g_dhcp_snooping_config.trusted_ifindex = get_ifindex(g_dhcp_snooping_config.trusted_interface)) == DHCP_FAIL)
		{
			printf("get ifindex error\r\n");
			return;
		}
#if 0		
		printf("the trusted interface is %s, ifindex is %d\r\n", g_dhcp_snooping_config.trusted_interface,
			g_dhcp_snooping_config.trusted_ifindex);
#endif
	}

//add by wangjr 20100105
	if (pthread_create(&pexpire, NULL, (void*(*)(void*))dhcpsnoop_expired, NULL) != 0)
	{
//	  dprintf("....dhcpsnoop_expiredr\n");//wangyu cannel dprintf for apv6
	}
//add by wangjr

	packet = malloc(2048);
	if (!packet)
	{
		printf("malloc memory error \r\n");
		return;
	}
	while (1)
	{
	
		dhcp_snoop_scan_tbl();
		memset(packet, 0, 2048);
		bytes = dhcp_snoop_pkt_receive(&sll, packet, &ifindex);
		
		if (bytes == 0)
		{
			continue;
		}
		data = (struct dhcpMessage *)(packet + sizeof(struct ethhdr) + sizeof(struct iphdr) + sizeof(struct udphdr)); 
		if ((message = (unsigned char *)get_option(data, DHCP_MESSAGE_TYPE)) == NULL) 
		{
			printf("couldnt get option from packet -- ignoring");
			continue;
		}
		if (data->op == BOOTREPLY)
		{
/*			if(dhcp_snoop_is_trusted(ifindex) == DHCP_FAIL)
			{
				printf("receive dhcp reply packet from untrusted interface %d, discard\r\n", ifindex);
				continue;
			}
*/
		}
		else if(ifindex == g_dhcp_snooping_config.trusted_ifindex)
		{
				printf("receive dhcp request packet from trusted interface %d, discard\r\n", ifindex);
				continue;
		}
		switch (*message) {
			
			case DHCPDISCOVER :
				ret = dhcp_snoop_discovery_process(ifindex, data);
				break;
			case DHCPOFFER :
				ret = dhcp_snoop_offer_process(ifindex, data);
				break;
			case DHCPREQUEST :	
				ret = dhcp_snoop_request_process(ifindex, data);
				break;
			case DHCPACK :	
				ret = dhcp_snoop_ack_process(ifindex, data);
				break;
			case DHCPNAK :
				ret = dhcp_snoop_nack_process(ifindex, data);
				break;
			case DHCPRELEASE :
				ret = dhcp_snoop_nack_process(ifindex, data);
				break;
			case DHCPINFORM :	
			case DHCPDECLINE :	
				break;
			default:
				printf("unknow message");
				break;			
		}	
		
	/*
		if ((*message) == DHCPDISCOVER)
		{
			dhcp_snoop_attach_option82(packet);
		}
		if (data->op == BOOTREPLY)
		{
			dhcp_snoop_pkt_send_all(&sll, packet, &ifindex);
		}
		else if (ifindex != g_dhcp_snooping_config.trusted_ifindex)
		{
			printf("send packet from %d to trusted_ifindex %d\n",ifindex,g_dhcp_snooping_config.trusted_ifindex);
			dhcp_snoop_pkt_send(&sll, packet, &g_dhcp_snooping_config.trusted_ifindex);
		}
	*/
		printf("\n");
	}
	free(packet);
	dhcp_snoop_tbl_destroy();
	dhcp_snoop_pkt_destroy();
	return 0;
} 

//#if 0
/*   --------------------------------------add by wangjr 2009-12-15 ---------------------------------------*/
struct rtnl_handle
{
	int			fd;
	struct sockaddr_nl	local;
	struct sockaddr_nl	peer;
	__u32			seq;
	__u32			dump;
};


static inline void rtnl_close(struct rtnl_handle *rth)
{
	close(rth->fd);
}

static inline int rtnl_open(struct rtnl_handle *rth, unsigned subscriptions)
{
	int addr_len;

	memset(rth, 0, sizeof(rth));

	rth->fd = socket(PF_NETLINK, SOCK_RAW, NETLINK_ROUTE);
	if (rth->fd < 0) {
		perror("Cannot open netlink socket");
		return -1;
	}

	memset(&rth->local, 0, sizeof(rth->local));
	rth->local.nl_family = AF_NETLINK;
	rth->local.nl_groups = subscriptions;

	if (bind(rth->fd, (struct sockaddr*)&rth->local, sizeof(rth->local)) < 0) {
		perror("Cannot bind netlink socket");
		return -1;
	}
	addr_len = sizeof(rth->local);
	if (getsockname(rth->fd, (struct sockaddr*)&rth->local,
			(socklen_t *) &addr_len) < 0) {
		perror("Cannot getsockname");
		return -1;
	}
	if (addr_len != sizeof(rth->local)) {
		fprintf(stderr, "Wrong address length %d\n", addr_len);
		return -1;
	}
	if (rth->local.nl_family != AF_NETLINK) {
		fprintf(stderr, "Wrong address family %d\n", rth->local.nl_family);
		return -1;
	}
	rth->seq = time(NULL);
	return 0;
}

//add by wangjr 20100104

int offline_deleteItem(const struct ether_addr * ether_mac)
{
	dhcp_snooping_user_item_t user;
	dhcp_snooping_tbl_item_t *item;  
	user.chaddr[0]=ether_mac->ether_addr_octet[0];
	user.chaddr[1]=ether_mac->ether_addr_octet[1];
	user.chaddr[2]=ether_mac->ether_addr_octet[2];
	user.chaddr[3]=ether_mac->ether_addr_octet[3];
	user.chaddr[4]=ether_mac->ether_addr_octet[4];
	user.chaddr[5]=ether_mac->ether_addr_octet[5];

	item = (dhcp_snooping_tbl_item_t *)dhcp_snoop_tbl_item_find(&user);

	if (item == NULL)
	{
		return DHCP_OK; /*from trust to trust interface*/
	}	
	printf("\ndhcp_snoop_tbl_item_delete ..item->ip_addr:%d  item->ifindex:%d",item->ip_addr,item->ifindex);
	if (dhcp_snoop_tbl_item_delete(item) == DHCP_FAIL)
	{
		return DHCP_FAIL;
	}
}


/*------------------------------------------------------------------*/
/*
 * Print one element from the scanning results
 */
static inline int
print_event_token(char *sysmsg, struct iw_event *	event,		/* Extracted token */
		  struct iw_range *	iw_range,	/* Range info */
		  int			has_range)
{
  char		buffer[128];	/* Temporary buffer */
  char		buffer2[30];	/* Temporary buffer */
  char      buffer3[128];
  char *	prefix = (IW_IS_GET(event->cmd) ? "New" : "Set");

  /* Now, let's decode the event */
  switch(event->cmd)
    {

	case IWEVREGISTERED:
		  printf("\n ------------------Registered node------------------------------- \n");        
	break;

    case IWEVEXPIRED:
		printf("\n --------wangjr---IWEVEXPIRED-------------- \n");
		sprintf(buffer3, "Expired node:%s\n",
		iw_saether_ntop(&event->u.addr, buffer));
		#if 0
		printf("\n  888888888888888888%d \n ",((const struct ether_addr *)(&event->u.addr.sa_data))->ether_addr_octet[0]);
		printf("\n  888888888888888888%d \n ",((const struct ether_addr *)(&event->u.addr.sa_data))->ether_addr_octet[1]);
		printf("\n  888888888888888888%d \n",((const struct ether_addr *)(&event->u.addr.sa_data))->ether_addr_octet[2]);
		printf("\n  888888888888888888%d \n",((const struct ether_addr *)(&event->u.addr.sa_data))->ether_addr_octet[3]);
		printf("\n  888888888888888888%d \n",((const struct ether_addr *)(&event->u.addr.sa_data))->ether_addr_octet[4]);
		printf("\n  888888888888888888%d \n",((const struct ether_addr *)(&event->u.addr.sa_data))->ether_addr_octet[5]);
		#endif
		offline_deleteItem(&event->u.addr.sa_data);
		strcat(sysmsg, buffer3);
      break;
    default:
      sprintf(buffer3, "(Unknown Wireless event 0x%04X)\n", event->cmd);
      strcat(sysmsg, buffer3);
  }	/* switch(event->cmd) */

  return(0);
}

/******************* WIRELESS INTERFACE DATABASE *******************/
/*
 * We keep a few information about each wireless interface on the
 * system. This avoid to query this info at each event, therefore
 * reducing overhead.
 *
 * Each interface is indexed by the 'ifindex'. As opposed to interface
 * names, 'ifindex' are never reused (even if you reactivate the same
 * hardware), so the data we cache will never apply to the wrong
 * interface.
 * Because of that, we are pretty lazy when it come to purging the
 * cache...
 */

/*------------------------------------------------------------------*/
/*
 * Get name of interface based on interface index...
 */
static inline int
index2name(int		skfd,
	   int		ifindex,
	   char *	name)
{
  struct ifreq	irq;
  int		ret = 0;

  memset(name, 0, IFNAMSIZ + 1);

  /* Get interface name */
  irq.ifr_ifindex = ifindex;
  if(ioctl(skfd, SIOCGIFNAME, &irq) < 0)
    ret = -1;
  else
    strncpy(name, irq.ifr_name, IFNAMSIZ);

  return(ret);
}

/*------------------------------------------------------------------*/
/*
 * Get interface data from cache or live interface
 */
static struct wireless_iface *
iw_get_interface_data(int	ifindex)
{
  struct wireless_iface *	curr;
  int				skfd = -1;	/* ioctl socket */

  /* Search for it in the database */
  curr = interface_cache;
  while(curr != NULL)
    {
      /* Match ? */
      if(curr->ifindex == ifindex)
	{
	  //printf("Cache : found %d-%s\n", curr->ifindex, curr->ifname);

	  /* Return */
	  return(curr);
	}
      /* Next entry */
      curr = curr->next;
    }

  /* Create a channel to the NET kernel. Doesn't happen too often, so
   * socket creation overhead is minimal... */
  if((skfd = iw_sockets_open()) < 0)
    {
      perror("iw_sockets_open");
      return(NULL);
    }

  /* Create new entry, zero, init */
  curr = calloc(1, sizeof(struct wireless_iface));
  if(!curr)
    {
      fprintf(stderr, "Malloc failed\n");
      return(NULL);
    }
  curr->ifindex = ifindex;

  /* Extract static data */
  if(index2name(skfd, ifindex, curr->ifname) < 0)
    {
      perror("index2name");
      free(curr);
      return(NULL);
    }
  curr->has_range = (iw_get_range_info(skfd, curr->ifname, &curr->range) >= 0);
  //printf("Cache : create %d-%s\n", curr->ifindex, curr->ifname);

  /* Done */
  iw_sockets_close(skfd);

  /* Link it */
  curr->next = interface_cache;
  interface_cache = curr;

  return(curr);
}


/*------------------------------------------------------------------*/
/*
 * Print out all Wireless Events part of the RTNetlink message
 * Most often, there will be only one event per message, but
 * just make sure we read everything...
 */
static inline int
print_event_stream(int		ifindex,
		   char *	data,
		   int		len)
{

//#if 0
  struct iw_event	iwe;
  struct stream_descr	stream;
  int			i = 0;
  int			ret;
  char			buffer[64];
  char          sysmsg[128];
  struct timeval	recv_time;
  struct timezone	tz;
  struct wireless_iface *	wireless_data;

  /* Get data from cache */
  wireless_data = iw_get_interface_data(ifindex);
  if(wireless_data == NULL)
    return(-1);

  /* Print received time in readable form */
  gettimeofday(&recv_time, &tz);
  iw_print_timeval(buffer, sizeof(buffer), &recv_time, &tz);
  iw_init_event_stream(&stream, data, len);
  do
    {
      /* Extract an event and print it */
      ret = iw_extract_event_stream(&stream, &iwe,
				    wireless_data->range.we_version_compiled);
      if(ret != 0)
	{
	  //if(i++ == 0)
	    sprintf(sysmsg, "%s %s ", buffer, wireless_data->ifname);
	  //else
	   // sprintf("                           ");
	  if(ret > 0)
	    print_event_token(sysmsg, &iwe,
			             &wireless_data->range, wireless_data->has_range);
	  //else
	    sprintf(sysmsg, "    (Invalid event)\n");
	  /* Push data out *now*, in case we are redirected to a pipe */
      //deal_with_event(sysmsg, &iwe);     
      fflush(stdout);
	}
    }
  while(ret > 0);
//#endif
  return(0);
}

/*------------------------------------------------------------------*/
/*
 * Remove interface data from cache (if it exist)
 */
static void
iw_del_interface_data(int	ifindex)
{
  struct wireless_iface *	curr;
  struct wireless_iface *	prev = NULL;
  struct wireless_iface *	next;

  /* Go through the list, find the interface, kills it */
  curr = interface_cache;
  while(curr)
    {
      next = curr->next;

      /* Got a match ? */
      if(curr->ifindex == ifindex)
	{
	  /* Unlink. Root ? */
	  if(!prev)
	    interface_cache = next;
	  else
	    prev->next = next;
	  //printf("Cache : purge %d-%s\n", curr->ifindex, curr->ifname);

	  /* Destroy */
	  free(curr);
	}
      else
	{
	  /* Keep as previous */
	  prev = curr;
	}

      /* Next entry */
      curr = next;
    }
}




/*********************** RTNETLINK EVENT DUMP***********************/
/*
 * Dump the events we receive from rtnetlink
 * This code is mostly from Casey
 */

/*------------------------------------------------------------------*/
/*
 * Respond to a single RTM_NEWLINK event from the rtnetlink socket.
 */
static int
LinkCatcher(struct nlmsghdr *nlh)
{
  struct ifinfomsg* ifi;


  printf("\n ------------------LinkCatcher-----------------------\n");  //add by wangjr

#if 0
  fprintf(stderr, "nlmsg_type = %d.\n", nlh->nlmsg_type);
#endif

  ifi = NLMSG_DATA(nlh);

  /* Code is ugly, but sort of works - Jean II */

  /* If interface is getting destoyed */
  if(nlh->nlmsg_type == RTM_DELLINK)
    {
      /* Remove from cache (if in cache) */
      iw_del_interface_data(ifi->ifi_index);
      return 0;
    }

  /* Only keep add/change events */
  if(nlh->nlmsg_type != RTM_NEWLINK)
    return 0;

  /* Check for attributes */
  if (nlh->nlmsg_len > NLMSG_ALIGN(sizeof(struct ifinfomsg)))
    {
      int attrlen = nlh->nlmsg_len - NLMSG_ALIGN(sizeof(struct ifinfomsg));
      struct rtattr *attr = (void *) ((char *) ifi +
				      NLMSG_ALIGN(sizeof(struct ifinfomsg)));

      while (RTA_OK(attr, attrlen))
	{
	  /* Check if the Wireless kind */
	  if(attr->rta_type == IFLA_WIRELESS)
	    {
	      /* Go to display it */
	      print_event_stream(ifi->ifi_index,
				 (char *) attr + RTA_ALIGN(sizeof(struct rtattr)),
				 attr->rta_len - RTA_ALIGN(sizeof(struct rtattr)));
	    }
	  attr = RTA_NEXT(attr, attrlen);
	}
    }

  return 0;
}


/* ---------------------------------------------------------------- */
/*
 * We must watch the rtnelink socket for events.
 * This routine handles those events (i.e., call this when rth.fd
 * is ready to read).
 */
static inline void
handle_netlink_events(struct rtnl_handle *	rth)
{
  while(1)
    {
      struct sockaddr_nl sanl;
      socklen_t sanllen = sizeof(struct sockaddr_nl);

      struct nlmsghdr *h;
      int amt;
      char buf[8192];

      amt = recvfrom(rth->fd, buf, sizeof(buf), MSG_DONTWAIT, (struct sockaddr*)&sanl, &sanllen);
      if(amt < 0)
	{
	  if(errno != EINTR && errno != EAGAIN)
	    {
	      fprintf(stderr, "%s: error reading netlink: %s.\n",
		      __PRETTY_FUNCTION__, strerror(errno));
	    }
	  return;
	}

      if(amt == 0)
	{
	  fprintf(stderr, "%s: EOF on netlink??\n", __PRETTY_FUNCTION__);
	  return;
	}

      h = (struct nlmsghdr*)buf;
      while(amt >= (int)sizeof(*h))
	{
	  int len = h->nlmsg_len;
	  int l = len - sizeof(*h);

	  if(l < 0 || len > amt)
	    {
	      fprintf(stderr, "%s: malformed netlink message: len=%d\n", __PRETTY_FUNCTION__, len);
	      break;
	    }

	  switch(h->nlmsg_type)
	    {
	    case RTM_NEWLINK:
	    case RTM_DELLINK:
	      LinkCatcher(h);
	      break;
	    default:
#if 0
	      fprintf(stderr, "%s: got nlmsg of type %#x.\n", __PRETTY_FUNCTION__, h->nlmsg_type);
#endif
	      break;
	    }

	  len = NLMSG_ALIGN(len);
	  amt -= len;
	  h = (struct nlmsghdr*)((char*)h + len);
	}

      if(amt > 0)
	fprintf(stderr, "%s: remnant of size %d on netlink\n", __PRETTY_FUNCTION__, amt);
    }
}



/**************************** MAIN LOOP ****************************/

/* ---------------------------------------------------------------- */
/*
 * Wait until we get an event
 */
static inline int
wait_for_event(struct rtnl_handle *	rth)
{
#if 0
  struct timeval	tv;	/* Select timeout */
#endif

  /* Forever */
  while(1)
    {
      fd_set		rfds;		/* File descriptors for select */
      int		last_fd;	/* Last fd */
      int		ret;

      /* Guess what ? We must re-generate rfds each time */
      FD_ZERO(&rfds);
      FD_SET(rth->fd, &rfds);
      last_fd = rth->fd;

      /* Wait until something happens */
      ret = select(last_fd + 1, &rfds, NULL, NULL, NULL);

      /* Check if there was an error */
      if(ret < 0)
	{
	  if(errno == EAGAIN || errno == EINTR)
	    continue;
	  fprintf(stderr, "Unhandled signal - exiting...\n");
	  break;
	}

      /* Check if there was a timeout */
      if(ret == 0)
	{
	  continue;
	}

      /* Check for interface discovery events. */
      if(FD_ISSET(rth->fd, &rfds))
	handle_netlink_events(rth);
    }

  return(0);
}

void dhcpsnoop_expired()
{
	struct rtnl_handle	  rth;
	int opt;
	FILE *file = NULL;
	char tmp_buf[64];
	
	/* Open netlink channel */
	if(rtnl_open(&rth, RTMGRP_LINK) < 0)
	  {
		perror("Can't initialize rtnetlink socket");
		return(1);
	  }
	
	fprintf(stderr, "Waiting for Wireless Events from interfaces...\n");
	
	/* Do what we have to do */
	wait_for_event(&rth);
	
	/* Cleanup - only if you are pedantic */
	rtnl_close(&rth);
	
	return(0);


}
/*   --------------------------------------add by wangjr 2009-12-15 ---------------------------------------*/
//#endif
