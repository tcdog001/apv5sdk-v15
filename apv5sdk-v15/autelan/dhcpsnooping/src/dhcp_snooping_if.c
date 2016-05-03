/**********************************************************************************
* Copyright (c) 2008-2011  Beijing Autelan Technology Co. Ltd.
* All rights reserved.
*
* filename: Dhcp_snooping_if.c
* description:  implementation for the Dhcp_snooping for interface 
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
#include <string.h>
#include <linux/if_ether.h>




#include "dhcp_snooping_if.h"
#include "dhcp_snooping_options.h"
#include "dhcp_snooping_pkt.h"
#include "dhcp_snooping_tbl.h"
#include "dhcp_snooping_main.h"


int procnetdev_vsn = 1;

int ife_short; 

extern int g_sockfd;


struct interface *int_list, *int_last;

static int if_readlist_proc(char *);

extern int dhcp_snoop_set_promisc(char *ifname,int sock);

static struct interface *add_interface(char *name)
{
    struct interface *ife, **nextp, *new;

    for (ife = int_last; ife; ife = ife->prev) {
	    int n = strcmp(ife->name, name); 
	    if (n == 0) 
		    return ife; 
	    if (n < 0) 
		    break; 
    }
    new = (struct interface *)malloc(sizeof(struct interface)); 
    if (new == NULL )
            return NULL;
    strncpy(new->name, name, IFNAMSIZ); 
	
    nextp = ife ? &ife->next : &int_list;
    new->prev = ife;
    new->next = *nextp; 
    if (new->next) 
	    new->next->prev = new; 
    else
	    int_last = new; 
    printf("add new interface %s \n", new->name);

    *nextp = new; 
    return new; 
}

struct interface *lookup_interface(char *name)
{
    struct interface *ife = NULL;

    if (if_readlist_proc(name) < 0) 
	    return NULL; 
    ife = add_interface(name); 
    return ife;
}

int dhcp_snoop_set_promisc(char *ifname,int sock)
{
	struct ifreq ifr;
	strncpy(ifr.ifr_name,ifname,strlen(ifname)+1);
	if((ioctl(sock,SIOCGIFFLAGS,&ifr) == -1))
	{
		printf("Couldn't retrive flags for the interface %s\n", ifname);
		return DHCP_FAIL;
	}
	//printf("The interface is ::%s\n",ifname);
	//printf("Retrieved flags from the interface successly %s\n",ifname);
	ifr.ifr_flags &= ~IFF_PROMISC;
	if(ioctl(sock,SIOCSIFFLAGS,&ifr) == -1)
	{
		printf("Could not set the no PROMISC flag\n");
		return DHCP_FAIL;
	}
	//printf("Setting interface::%s::not to promisc\n",ifname);
	return DHCP_OK;
}

int dhcp_snoop_get_ifindex(int *ifindex, char *ifname,int sock)
{
	struct ifreq ifr;
	strncpy(ifr.ifr_name,ifname,strlen(ifname)+1);
	if((ioctl(sock,SIOCGIFINDEX,&ifr) == -1))
	{
		printf("Couldn't retrive ifindex for the interface %s\r\n", ifname);
		return DHCP_FAIL;
	}
	*ifindex = ifr.ifr_ifindex;
	printf("The interface %s ifindex is ::%d\r\n",ifname, *ifindex);
	return DHCP_OK;
}

int get_ifindex(char *ifname)
{
	int ifindex;
	
	if(dhcp_snoop_get_ifindex(&ifindex, ifname, g_sockfd)== DHCP_FAIL)
	{
		return DHCP_FAIL;
	}
	return ifindex;
}

int set_all_interfaces_promisc(int sock)
{
    struct interface *ife;
	int err;

 //   if (!int_list && (if_readlist() < 0)) //note by sundaolian
    if ((if_readlist() < 0))
	return -1;
	
    for (ife = int_list; ife; ife = ife->next) 
	{
		err = dhcp_snoop_set_promisc(ife->name, sock);
		if (err == DHCP_FAIL)
		    return err;
		err = dhcp_snoop_get_ifindex(&(ife->ifindex),ife->name, sock);		
		if (err == DHCP_FAIL)
		    return err;
    }
    return DHCP_OK;
}
 


int free_interface_list(void)
{
    struct interface *ife;
    while ((ife = int_list) != NULL) {
	int_list = ife->next;
	free(ife);
    }
    return 0;
}
#if 0
static int if_readconf(void)
{
    int numreqs = 30;
    struct ifconf ifc;
    struct ifreq *ifr;
    int n, err = -1;
    int skfd;

    /* SIOCGIFCONF currently seems to only work properly on AF_INET sockets
       (as of 2.1.128) */ 
    skfd = get_socket_for_af(AF_INET);
    if (skfd < 0) {
	printf("warning: no inet socket available: %s\n",skfd);
	/* Try to soldier on with whatever socket we can get hold of.  */
	skfd = sockets_open(0);
	if (skfd < 0)
	    return -1;
    }

    ifc.ifc_buf = NULL;
    for (;;) {
	ifc.ifc_len = sizeof(struct ifreq) * numreqs;
	ifc.ifc_buf = xrealloc(ifc.ifc_buf, ifc.ifc_len);

	if (ioctl(skfd, SIOCGIFCONF, &ifc) < 0) {
	    perror("SIOCGIFCONF");
	    goto out;
	}
	if (ifc.ifc_len == sizeof(struct ifreq) * numreqs) {
	    /* assume it overflowed and try again */
	    numreqs += 10;
	    continue;
	}
	break;
    }

    ifr = ifc.ifc_req;
    for (n = 0; n < ifc.ifc_len; n += sizeof(struct ifreq)) {
	add_interface(ifr->ifr_name);
	ifr++;
    }
    err = 0;

out:
    free(ifc.ifc_buf);
    return err;
}

#endif

static char *get_name(char *name, char *p)
{
    while (isspace(*p))
	p++;
    while (*p) {
	if (isspace(*p))
	    break;
	if (*p == ':') {	/* could be an alias */
	    char *dot = p, *dotname = name;
	    *name++ = *p++;
	    while (isdigit(*p))
		*name++ = *p++;
	    if (*p != ':') {	/* it wasn't, backup */
		p = dot;
		name = dotname;
	    }
	    if (*p == '\0')
		return NULL;
	    p++;
	    break;
	}
	*name++ = *p++;
    }
    *name++ = '\0';
    return p;
}

static int procnetdev_version(char *buf)
{
    if (strstr(buf, "compressed"))
	return 3;
    if (strstr(buf, "bytes"))
	return 2;
    return 1;
}

static int if_readlist_proc(char *target)
{
    static int proc_read; 
    FILE *fh;
    char buf[512];
    struct interface *ife;
    int err;

    if (proc_read) 
	    return 0; 
#if 0   /*add by sundaolian*/ 	
    if (!target) 
	    proc_read = 1;
#endif /*add by sundaolian*/

    fh = fopen(_PATH_PROCNET_DEV, "r");
    if (!fh) {
		printf("Warning: cannot open %s (%s). Limited output.\n");
		return 0 /*if_readconf()*/;
	}	
    fgets(buf, sizeof buf, fh);	/* eat line */
    fgets(buf, sizeof buf, fh);

#if 0				/* pretty, but can't cope with missing fields */
    fmt = proc_gen_fmt(_PATH_PROCNET_DEV, 1, fh,
		       "face", "",	/* parsed separately */
		       "bytes", "%lu",
		       "packets", "%lu",
		       "errs", "%lu",
		       "drop", "%lu",
		       "fifo", "%lu",
		       "frame", "%lu",
		       "compressed", "%lu",
		       "multicast", "%lu",
		       "bytes", "%lu",
		       "packets", "%lu",
		       "errs", "%lu",
		       "drop", "%lu",
		       "fifo", "%lu",
		       "colls", "%lu",
		       "carrier", "%lu",
		       "compressed", "%lu",
		       NULL);
    if (!fmt)
	return -1;
#else
    procnetdev_vsn = procnetdev_version(buf);
#endif

    err = 0;
    while (fgets(buf, sizeof buf, fh)) {
	char *s, name[IFNAMSIZ];
	s = get_name(name, buf);    
	ife = add_interface(name);
#if 0	
	get_dev_fields(s, ife);
	ife->statistics_valid = 1;
#endif
	if (target && !strcmp(target,name))
		break;
    }
    if (ferror(fh)) {
	perror(_PATH_PROCNET_DEV);
	err = -1;
	proc_read = 0; 
    }

#if 0
    free(fmt);
#endif
    fclose(fh);
    return err;
}

int if_readlist(void) 
{ 
    int err = if_readlist_proc(NULL); 
#if 0
    if (!err)
	    err = if_readconf();
#endif
    return err;
} 
/*
 ******************************************************************************
 *  Function   :   int dhcp_snooping_interface_initialize(void)
 *  Input      :   void
 *  Output     :   void
 *  Return     :   OK/FAIL
 *  Description:   initialize DHCP Snooping interface table
 ******************************************************************************
 */

int dhcp_snooping_interface_initialize(void)
{
	return if_readlist();
}

/*
 ******************************************************************************
 *  Function   :   UINT32 dhcp_snooping_interface_destroy(void)
 *  Input      :   void
 *  Output     :   void
 *  Return     :   OK/FAIL
 *  Description:   release DHCP Snooping interface table
 ******************************************************************************
 */
int dhcp_snooping_interface_destroy(void)
{
	return free_interface_list();
}

