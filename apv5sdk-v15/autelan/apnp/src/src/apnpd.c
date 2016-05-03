#include <signal.h>
#include <sys/socket.h>

#include "apnp_util.h"
#include "apnp_api.h"
#include "apnp_type.h"
#include "apnp_transport.h"
#include "apnp_log.h"

static int keep_going = 1;
int apnp_running = 1;
char *ifname;


extern struct sockaddr_in ser_addr;
extern uint32_t ip;
	
UpnpDevice_Handle device_handle = -1;

static void signal_handler(int sig) 
{
	switch (sig) {
		case SIGTERM:
		case SIGINT:
		case SIGUSR2:
//			apnp_log_info("Received terminate signal %d\n", sig);
			apnp_running = 0;
			break;
		default:
			break;
	}
}

static void
apnp_singal_init(void) {	
	struct sigaction act;
	act.sa_handler = signal_handler;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;
	sigaction(SIGTERM, &act, NULL);
	sigaction(SIGINT, &act, NULL);
	sigaction(SIGUSR2, &act, NULL);
	//signal(SIGCHLD,SIG_IGN);
}



static void
apnp_service_init(){
	uint32_t tmp_ip = 0;

	tmp_ip = do_ioctl_get_ipaddress(ifname);
	if (0 == tmp_ip) {
			ApnpDebugLog("Can`t get the %s ip address",ifname);
			return ;
	}
	apnp_service_restart(tmp_ip);
	ip = tmp_ip;
}

static void 
apnp_config_init(void)
{
	config_init();
}

static void
apnp_init()

{	ApnpLogInitFile(gApnpLogFileName);

	apnp_singal_init();

	apnp_dlysm_init();
	//pthread_mutex_init();
	apnp_tdomain_init();

	apnp_session_init();

	apnp_config_init();

	

	apnp_service_init();

	

}

void udp_heartbeat(void)
{
	ApnpDebugLog("enter udp_heartbeat...\n");
	pthread_t thread;
	int th_tmp;
	if((th_tmp = pthread_create(&thread,NULL,(void *(*)(void))beat_handle,NULL)) !=0 ){
		ApnpDebugLog("udp_heartbeat: create thread ERROR!\n");
		exit(-1);
	}
	else
		ApnpDebugLog("udp_heartbeat: create heartbeat thread SUCESS!\n");

	//pthread_join(thread,NULL); 主线程阻塞直到thread退出
	ApnpDebugLog("udp_heartbeat quit!\n");
	
	return;
}


static int
apnp_receive(void) {
	int	numfds;
	fd_set readfds;
	struct timeval  timeout, *tvp;
	int	count, block;

	while(apnp_running) {

		tvp = &timeout;
		tvp->tv_sec = 2;
		tvp->tv_usec = 0;

		apnp_select_info(&numfds, &readfds, tvp, &block);
		if(block) {
			tvp = NULL;
		}
		
	reselect:	
		ApnpDebugLog("apnp select .........\n");
		count = select(numfds, &readfds, NULL, NULL, tvp);
		ApnpDebugLog("apnp select: count = %d\n", count);
		if (count > 0) {
			apnp_read(&readfds);
		} else switch(count) {
			case 0:
				apnp_timeout();
				break;
				
			case -1:
				ApnpDebugLog("apnp select , errno = %d\n", errno);
				if (errno == EINTR) {
					/*
					* likely that we got a signal. Check our special signal
					* flags before retrying select.
					*/
					if(apnp_running) {
						goto reselect;
					}
					continue;
				} 
				
				return -1;
				
			default:
				ApnpDebugLog("apnp select returned %d\n", count);
				return -1;
		}	
	}
	conf_ap_free();
	
	ApnpDebugLog("Received TERM or STOP signal...  shutting down...\n");
	return 0;
}


static void
apnp_task_process(void) {
	pthread_t thread_task;
	
	if(pthread_create(&thread_task, NULL, (void *(*) (void *))_task_thread, NULL)) {
		ApnpDebugLog("An error occurs when create task thread!");
		exit(-1);
	}
	
	ApnpDebugLog("create task thread success!");
	return ;
}

int main(int argc, char *argv[])
{
	
	char ipstr[32] = "";
	//char *ifname = "";
#if 0
	if (1 == argc) {
		ifname = "default";
	} else if (2 == argc && strlen(argv[1]) < 16) {
		ifname = argv[1];
	} else {
		ApnpDebugLog("used as:%s [ifname]\n", argv[0]);
		return 1;
	}
	
	ApnpDebugLog("cmd = %s \tifname = %s\n", argv[0], ifname);
#endif
	if(3 == argc){
		
		ifname = argv[1];
		
		memset(&ser_addr,0,sizeof(ser_addr));
		ser_addr.sin_family = AF_INET;
		ser_addr.sin_port = htons(38800);
		ser_addr.sin_addr.s_addr = inet_addr(argv[2]);
		printf("udp server port:%d~~~~~ip:%s\n",ser_addr.sin_port,inet_ntoa(ser_addr.sin_addr));
		printf("cmd = %s \tifname = %s\t server_ip = %s\n", argv[0], ifname,inet_ntoa(ser_addr.sin_addr));
	}else{
		printf("used as:%s [ifname] [server ip]\n", argv[0]);
		return 1;
	}

	apnp_init();
	udp_heartbeat();
	apnp_task_process();
	apnp_receive();

	#if 0
	ip = do_ioctl_get_ipaddress(ifname);
	if (0 == ip) {
		printf("Can`t get the %s ip address",ifname);
		return 1;
	}
	ip2str(ip, ipstr, sizeof(ipstr));
	
	ret = device_start(ipstr, 19900);

	if (UPNP_E_SUCCESS != ret) {
		return ret;
	}
	
	while (keep_going) {
		sleep(1);
	}
	device_stop();
	#endif
	return 0;
}
