/** include glibc **/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <syslog.h>
#include <errno.h>

#include <time.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <linux/netlink.h> 
#include <linux/rtnetlink.h> 

#include "apnp_util.h"
#include "apnp_log.h"

#include "apnp_type.h"


extern char* ifname;
uint32_t ip;


int
apnp_task_udp_send(void *data, size_t data_len,void **data_addr, size_t *data_addr_len)
{

		ApnpDebugLog("apnp_task_udp_send.....\n");

}
int
apnp_task_tcp_send(void *data, size_t data_len,void **data_addr, size_t *data_addr_len)
{

/*	printf("recvfrom client data:%s,data_length:%d\n",data,data_len);


	char paddr[20];
	inet_ntop(AF_INET,&(((struct sockaddr_in *)(*data_addr))->sin_addr),paddr,16);
	//send();
	
	printf("form_addr:%s----from_addr length:%d\n",paddr,*data_addr_len);
	*/
	ApnpDebugLog("apnp_task_tcp_send.....\n");
	

}



int
apnp_task_ip_change(void *data, size_t data_len,void **data_addr, size_t *data_addr_len)
{
	ApnpDebugLog("enter apnp_task_ip_change,ifname:%s\n",ifname);
	uint32_t tmp_ip = 0;
	
	tmp_ip = do_ioctl_get_ipaddress(ifname);
	if (0 >= tmp_ip) {
		ApnpDebugLog("apnp_task_ip_change Deleted ip from %s\n",ifname);
		return 1;
	}else if(ip == tmp_ip){
		ApnpDebugLog("apnp_task_ip_change %s ip not changed,do nothing...\n",ifname);
		return 1;
	}else{
		//ip has changed,restart service
		ApnpDebugLog("apnp_task_ip_change %s ip has changed,do restart service...\n",ifname);
		apnp_service_restart(tmp_ip);
		ip = tmp_ip;	
	}
	
	#if 0
	struct nlmsghdr *h;
	//char   buf[16384];
	int status = data_len;


	struct sockaddr_nl nladdr;
	struct iovec iov;
	struct msghdr msg = {
		.msg_name = &nladdr,
		.msg_namelen = sizeof(nladdr),
		.msg_iov = &iov,
		.msg_iovlen = 1,
	};
	char   buf[8192];
	
	memset(&nladdr, 0, sizeof(nladdr));
	nladdr.nl_family = AF_NETLINK;
	nladdr.nl_pid = 0;
	nladdr.nl_groups = 0;
	
	iov.iov_base = buf;
	iov.iov_len = sizeof(buf);


	if(data == NULL || data_len <= 0){
		fprintf(stderr,"data is NULL or data_len is 0.\n");
		return -1;
	}

	memcpy(&msg,data,data_len);

	if (msg.msg_namelen != sizeof(nladdr)) {
			fprintf(stderr, "Sender address length == %d\n", msg.msg_namelen);
			return -1;
		}
	
	for (h = (struct nlmsghdr*)buf; status >= sizeof(*h); ){
			fprintf(stdout,"apnp_task_ip_change.....\n");
			int err;
			int len = h->nlmsg_len;
			int l = len - sizeof(*h);

			if (h->nlmsg_type == RTM_NEWADDR || h->nlmsg_type == RTM_DELADDR) {
					fprintf(stdout,"apnp_task_ip_change:h->nlmsg_type:%d\n",h->nlmsg_type);
	
					//print_addrinfo(who, n, arg);
					return 0;
			}
			
			
			status -= NLMSG_ALIGN(len);
			h = (struct nlmsghdr*)((char*)h + NLMSG_ALIGN(len));
		}
	#endif

	
	
	ApnpDebugLog("exit apnp_task_ip_change\n");

}


