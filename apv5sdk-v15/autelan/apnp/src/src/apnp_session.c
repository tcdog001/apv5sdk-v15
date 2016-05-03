#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <syslog.h>

#include <sys/types.h>


/** include manage lib **/

#include "apnp_type.h"
#include "apnp_api.h"

#include "apnp_session.h"
#include "apnp_task_handler.h"


int
sess_netlink_init()
{
		apnp_session apnp_sess_netlink,*sp_net;
		//apnp_netlink_addr_group local_netlink;
		//unsigned int is_active_master = 0;
		
		memset(&apnp_sess_netlink, 0, sizeof(apnp_session));
		//memset(&local_netlink, 0, sizeof(manage_tipc_addr));
	
		apnp_sess_netlink.flags |= APNP_FLAGS_NETLINK_SOCKET;	
		apnp_sess_netlink.callback= apnp_task_ip_change;
	
		if(NULL == (sp_net = apnp_open(&apnp_sess_netlink))) {
			ApnpDebugLog("_session_netlink_init: s_apnp_errno = %d, s_errno = %d\n",
							apnp_sess_netlink.s_manage_errno, apnp_sess_netlink.s_errno);
			return APNP_ERR_OPEN_FAIL;
		}
	
		ApnpDebugLog("_session_netlink_init: netlink session is %p\n", sp_net);
		
		return APNP_SUCCESS;


}

int 
sess_tcp_init()
{
	apnp_session apnp_sess_tcp,*sp_net;
	memset(&apnp_sess_tcp,0,sizeof(apnp_session));

	apnp_sess_tcp.flags |= APNP_FLAGS_STREAM_SOCKET;
	//apnp_sess_tcp.message_type = APNP_MESSAGE_TYPE_RET;
	apnp_sess_tcp.callback = apnp_task_tcp_send;

	
	if(NULL == (sp_net = apnp_open(&apnp_sess_tcp))) {
		ApnpDebugLog("apnp_sess_tcp: s_apnp_errno = %d, s_errno = %d\n",
						apnp_sess_tcp.s_manage_errno, apnp_sess_tcp.s_errno);
		return APNP_ERR_OPEN_FAIL;
	}
	
	ApnpDebugLog("sess_tcp_init: tcp session is %p\n", sp_net);
	
	return APNP_SUCCESS;

		

}

int 
sess_udp_init()
{
	apnp_session apnp_sess_udp,*sp_net;
	memset(&apnp_sess_udp,0,sizeof(apnp_session));

	apnp_sess_udp.flags |= APNP_FLAGS_UDP_SOCKET;
	apnp_sess_udp.message_type = APNP_MESSAGE_TYPE_RET;

	apnp_sess_udp.callback = apnp_task_udp_send;

	
	if(NULL == (sp_net = apnp_open(&apnp_sess_udp))) {
		ApnpDebugLog("apnp_sess_udp: s_apnp_errno = %d, s_errno = %d\n",
						apnp_sess_udp.s_manage_errno, apnp_sess_udp.s_errno);
		return APNP_ERR_OPEN_FAIL;
	}
	
	ApnpDebugLog("sess_udp_init: udp session is %p\n", sp_net);
	
	return APNP_SUCCESS;

}

void 
apnp_session_init()
{
	sess_netlink_init();
	sess_tcp_init();
	sess_udp_init();
}

