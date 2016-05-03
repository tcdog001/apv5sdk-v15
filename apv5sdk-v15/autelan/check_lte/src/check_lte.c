/****************************
 *
 * to use in the user space 
 * autelan
 * 2014.04.13
 *
 ****************************/
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>   
#include <arpa/inet.h>
#include <signal.h>
#include <sys/types.h>
#include <pthread.h>

#define DEFAULT_IP	"192.168.1.1"
#define LTETIME 	60
#define NO_THREAD	1

#ifdef LTEFI
#define WAN0_UP 	"ifconfig wan0 up"
#define CHECK_WAN0 	"ifconfig | grep wan0"
#define CHECK_IP	"ifconfig | sed -e '1,/wan0/d' |awk -F \" \" '/addr:/{print $2}'|awk -F \":\" '{print $2}'"

#define NDIS_CONNECT		"/usr/sbin/ndis_connect"
#define NDIS_DISCONNECT		"/usr/sbin/ndis_disconnect"
#define NDIS_UDHCPC_BACK	"udhcpc -i wan0 -t 3 -b"
#define NDIS_UDHCPC			"udhcpc -i wan0"
#endif

#ifdef LTEFI_V2
#define WAN0_UP         "ifconfig usb0 up"
#define CHECK_WAN0      "ifconfig | grep usb0"
#define CHECK_IP        "ifconfig | sed -e '1,/usb0/d' |awk -F \" \" '/addr:/{print $2}'|awk -F \":\" '{print $2}'"

#define QMUXD		"killall qmuxd; /bin/qmuxd &"
#define NETCONN		"killall netconn; /bin/netconn /tmp/apn_config &"
#define NDIS_UDHCPC_BACK        "udhcpc -i usb0 -t 3 -b"
#define NDIS_UDHCPC                     "udhcpc -i usb0"
#define GET_LTE_MODE	"ctrl_lte at'$'qcsysmode | sed -nr '2p'"
#endif

char lte_mode[50] = {0};

#define CHECK_RESLOVE	"cat /tmp/resolv.conf.auto 2> /dev/null | grep nameserver"
#define CHECK_BAIDU		"ping www.baidu.com -c 2| grep icmp_seq= &"
#define CHECK_SOHU		"ping www.sohu.com -c 2| grep icmp_seq= &"
#define CHECK_SINA		"ping www.sina.com -c 2| grep icmp_seq= &"
#define REBOOT_NOW		"/sbin/reboot"

enum LETFISubType {
	//0,//reserved
	AP_LEAVE=1,			//lte-fi quit reason
	LTE_MODE=2,			//2,//for lte mode atc -c "at\$qcsysmode"
};


int PopenFile (char *cmd_str,char *str,int len )
{
	FILE *fp=NULL; 

	if(cmd_str == NULL ||str == NULL)
		return -1;

	memset(str, 0, len);		   
	fp = popen(cmd_str, "r");  
//	printf("%s\n", cmd_str);
	if(fp) 
	{	   
		fgets(str, len, fp);	   
		if(str[strlen(str)-1] == '\n')	   
		{		   
		   str[strlen(str)-1] = '\0';	   
		}	   
		pclose(fp); 	   
		return 0;    
	}  
	else
	{	   
		perror("popen");	   
		str = NULL;
		return -1;   
	}
	return 0;
}

int GetStdoutDev(int proc_id,int *stdout_dev)
{
	char str_cmd[128],str[64];
	int logon_type_serial = 0;
	memset(str_cmd,0,sizeof(str_cmd));
	sprintf(str_cmd,"ls -l /proc/%d/fd/1 | awk -F\">\" '/ttyS/{print}' | wc -l",proc_id);
	PopenFile(str_cmd,str,sizeof(str));
	logon_type_serial = atoi(str);
   
	if(logon_type_serial == 0){
		//current is telnet
		printf("current logon is via telnet\n");
		memset(str_cmd,0,sizeof(str_cmd));
		sprintf(str_cmd,"ls -l /proc/%d/fd/1 | awk -F\"/\" '{print $8}'",proc_id);
		PopenFile(str_cmd,str,sizeof(str));
		*stdout_dev = atoi(str);
   //	printf("Sender's stdout_dev:%d\n",*stdout_dev);
		*stdout_dev += 10;
	}else{
		//current is serial port
		printf("current logon is via serial\n");
		memset(str_cmd,0,sizeof(str_cmd));
		sprintf(str_cmd,"ls -l /proc/%d/fd/1 | awk -F\"S\" '/ttyS/{print $2}'",proc_id);
		PopenFile(str_cmd,str,sizeof(str));
		*stdout_dev = atoi(str);
   //	printf("Sender's stdout_dev:%d\n",*stdout_dev);
	}
   return 1;
}

int kill_udhcpc(void)
{
	char str_cmd[128], str[32];
	int count;
	
	memset(str_cmd, 0, sizeof(str_cmd));
	sprintf(str_cmd, "ps |awk '/%s/{if($5==\"udhcpc\"){print $1}}' | wc -l", NDIS_UDHCPC);
	PopenFile(str_cmd, str, sizeof(str));
	printf("udhcpc count:%d\n", atoi(str));
	if(atoi(str) == 0){
		printf("please make sure udhcpc has been started.\n");
	}

	count = atoi(str);
	while(count > 0)
	{
		memset(str_cmd, 0, sizeof(str_cmd));
		sprintf(str_cmd, "ps | awk '/%s/{if($5==\"udhcpc\"){print $1}}' | awk '{if(NR==1){print $1}}'", NDIS_UDHCPC);
		PopenFile(str_cmd, str, sizeof(str));
		if(str == NULL){
			printf("get udhcpc process id failed!\n");
			return 1;
		}
		else if(atoi(str) != 0){
			memset(str_cmd, 0, sizeof(str_cmd));
			sprintf(str_cmd, "kill -9 %d", atoi(str));
			system(str_cmd);
			printf("kill_udhcpc\n");
		}
		count--;
	}
	return 0;
}
int Popen_ping (char *cmd_str,char *str,int len )
{
	FILE *fp=NULL; 

	if(cmd_str == NULL ||str == NULL)
		return -1;

	memset(str, 0, len);		   
	printf("----------------Popen_ping start\n");
	system("uptime");
	fp = popen(cmd_str, "r");  
//	printf("%s\n", cmd_str);
	if(fp) 
	{	   
		fgets(str, len, fp);	   
		if(str[strlen(str)-1] == '\n')	   
		{		   
		   str[strlen(str)-1] = '\0';	   
		}	   
		pclose(fp); 	   
		system("uptime");
		printf("----------------Popen_ping return\n");
		return 0;    
	}  
	else
	{	   
		perror("popen");	   
		str = NULL;
		return -1;   
	}
	return 0;
}

int GetProcID(int *proc_id)
{
	char str_cmd[128],str[32];
	memset(str_cmd,0,sizeof(str_cmd));
	sprintf(str_cmd,"ps |awk '/wtpd/{if($5==\"wtpd\"){print $1}}' | wc -l");
//	sprintf(str_cmd,"ps |awk '/wtpd/{if($5==\"./wtpd\"){print $1}}' | wc -l");
	PopenFile(str_cmd,str,sizeof(str));
	printf("wtpd count:%d\n",atoi(str));
	if(atoi(str) == 0){
		printf("get wtpd process id failed!\nplease make sure wtpd has been started.\n");
		return 0;
	}

	memset(str_cmd,0,sizeof(str_cmd));
	sprintf(str_cmd,"ps | awk '/wtpd/{if($5==\"wtpd\"){print $1}}' | awk '{if(NR==1){print $1}}'");
//	sprintf(str_cmd,"ps | awk '/wtpd/{if($5==\"./wtpd\"){print $1}}' | awk '{if(NR==1){print $1}}'");
	PopenFile(str_cmd,str,sizeof(str));
	if(str == NULL){
		printf("get wtpd process id failed!\nplease make sure wtpd has been started.\n");
		return 0;
	}
	else if(atoi(str) != 0){
		*proc_id = atoi(str);
		printf("get wtpd process id:%d\n", *proc_id);
	}
	return 1;
}

void signal_f(int signum)
{
	printf("check_lte got a signal=%d\n", signum);

	int stdout_dev = 0, proc_id = 0;
  	int self_pid = getpid();
	
   	union sigval mysigval;
	printf("Sender self process id:%d\n",self_pid);
#if 1
	if(SIGIO == signum)//SIGIO 22; SIGUSR1 16; SIGUSR1+1=SIGUSR2 17 for wtpd_debug;
		mysigval.sival_int = AP_LEAVE;
	else
		mysigval.sival_int = LTE_MODE;
#else	
	GetStdoutDev(self_pid,&stdout_dev);
	mysigval.sival_int = stdout_dev;
#endif
	if(!GetProcID(&proc_id)) return;
//	if(sigqueue(proc_id, signum, mysigval) == -1)
	if(sigqueue(proc_id, SIGIO, mysigval) == -1)
		printf("Sender sent signal=%d sival_int=%d error!\n", signum, mysigval.sival_int);
	else
		printf("Sender sent signal=%d sival_int=%d ok!\n", signum, mysigval.sival_int);
	return;
}

void printids(const char *s)
{
    pid_t pid;
    pthread_t tid;
    pid = getpid();
    tid = pthread_self();
    printf("%s pid %u tid %u (0x%x)\n", s, (unsigned int) pid,
            (unsigned int) tid, (unsigned int) tid);
}

void *thr_dhcp(void *arg)
{
	printf("check_lte: udhcpc\n");
	kill_udhcpc();
	system(NDIS_UDHCPC_BACK);
    return NULL;
}
int lte_ping(char *cmd_str)
{
	int err;
	pthread_t ping_tid;
	char str_tmp[256] = {0};

	if(cmd_str == NULL)
		return -1;

	printf("check_lte: ping_udhcpc\n");
	Popen_ping(cmd_str, str_tmp, sizeof(str_tmp));
	if(!strlen(str_tmp)) {
#if NO_THREAD
		thr_dhcp(NULL);
#else
		err = pthread_create(&ping_tid, NULL, thr_dhcp, NULL);
		if (err != 0)
			printf("can't create thread: %s\n", strerror(err));
		printids("main thread:");
		pthread_join(ping_tid,NULL);
#endif
		return 1;
	}
	return 0;
}
#ifdef LTEFI_V2
void *get_lte_mode(void *arg)
{
	char temp_str[100];
	char str[50];
	
	printf("pthread get_lte_mode\n");
	while(1){
		memset(temp_str, 0, 100);
		sprintf(temp_str, GET_LTE_MODE);
		printf("get_lte_mode %s\n", temp_str);
		system("atc -c at");
		PopenFile(temp_str, str, sizeof(str));
		if(strlen(str)) {
//			printf("\n--------0--------get_lte_mode is nowmode=%d,%s oldmode=%s--------1--------\n", strlen(str), str, lte_mode);
			str[strlen(str) - 1] = '\0';
			printf("\n--------0--------get_lte_mode is nowmode=%d,%s oldmode=%s--------1--------\n", strlen(str), str, lte_mode);
			if(strcmp(lte_mode, str) != 0){
				signal_f(0);
			}
			memset(lte_mode, 0, sizeof(lte_mode));
			memcpy(lte_mode, str, strlen(str));
			printf("\n--------2--------oldmode=%s--------3--------\n", lte_mode);
		}
		sleep(20);
	}
}
#endif
void *thr_dail(void *arg)
{
	printf("check_lte: dailing\n");
	kill_udhcpc();
#ifdef LTEFI
	system(NDIS_DISCONNECT);    
	system(NDIS_CONNECT);    
#endif
#ifdef LTEFI_V2
//	system(QMUXD);
	system(NETCONN);
	system(NDIS_UDHCPC_BACK);
#endif
    return NULL;
}
int lte_ping_dail(char *cmd_str)
{
	int err;
	pthread_t ping_tid;
	char str_tmp[256] = {0};

	if(cmd_str == NULL)
		return -1;

	printf("check_lte: ping_dail\n");
	Popen_ping(cmd_str, str_tmp, sizeof(str_tmp));
	if(!strlen(str_tmp)) {
#if NO_THREAD
		thr_dail(NULL);
#else
		err = pthread_create(&ping_tid, NULL, thr_dail, NULL);
		if (err != 0)
			printf("can't create thread: %s\n", strerror(err));
		printids("main thread:");
		pthread_join(ping_tid,NULL);
#endif
		return 1;
	}
	return 0;
}
int main(int argc, char **argv) 
{
	int opt, check_interval = LTETIME, check_flag = 0;
	char *default_IP = DEFAULT_IP;
	char cmd_str[128] = {0};
	char str_tmp[256] = {0};

	int err;
	pthread_t dail_tid, mode_tid;

	while ((opt = getopt(argc,argv,"s:t:D:I:hA")) != -1)
	{
		switch (opt) {
			case 'h':
				printf("check_lte\n");
				printf("          -h (get help, exist immediately)\n");
				printf("          -I 192.168.1.1 (set wan0 default_IP, for check_lte)\n");
				printf("          -t 60 (set check_interval (sec), for check_lte)\n");
				return 0;				
			case 'I':
				default_IP = optarg;
				break;
			case 't':
				check_interval = strtoul(optarg,NULL,10);
				break;
			default :
				printf("wrong input\n");
		}		
	}
	system(WAN0_UP);
#ifdef LTEFI_V2
	system(QMUXD);
	system(NETCONN);
	err = pthread_create(&mode_tid, NULL, get_lte_mode, NULL);
	if (err != 0)
		printf("can't create thread: %s\n", strerror(err));
	printf("check_lte: open acc\n");
#endif
	printf("check_lte running\ndefault_IP:%s\ncheck_interval:%d\n", default_IP, check_interval);

	while (1)
	{	
		PopenFile(CHECK_WAN0, str_tmp, sizeof(str_tmp));
		if(!strlen(str_tmp))
		{
			printf("check_lte: sleep for %d(sec)\n", check_interval*5);
			sleep(check_interval*5);
		}
		else
		{
			PopenFile(CHECK_IP, str_tmp, sizeof(str_tmp));
			if(0 == strlen(str_tmp))
			{
				check_flag ++;
#if NO_THREAD
				thr_dail(NULL);
#else
				err = pthread_create(&dail_tid, NULL, thr_dail, NULL);
				if (err != 0)
					printf("can't create thread: %s\n", strerror(err));
				printids("--------no ip.main thread:");
				pthread_join(dail_tid,NULL);
#endif
			}
			else if(0 == strcmp(str_tmp, default_IP))
			{
				check_flag ++;
#if NO_THREAD
				thr_dhcp(NULL);
#else
				err = pthread_create(&dail_tid, NULL, thr_dhcp, NULL);
				if (err != 0)
					printf("can't create thread: %s\n", strerror(err));
				printids("--------192.168.1.1.main thread:");
				pthread_join(dail_tid,NULL);
#endif
			}
			else
			{
				PopenFile(CHECK_RESLOVE, str_tmp, sizeof(str_tmp));
				if(!strlen(str_tmp))
				{
					check_flag ++;
#if NO_THREAD
					thr_dhcp(NULL);
#else
					err = pthread_create(&dail_tid, NULL, thr_dhcp, NULL);
					if (err != 0)
						printf("can't create thread: %s\n", strerror(err));
					printids("--------no resolve.main thread:");
					pthread_join(dail_tid,NULL);
#endif
				}
				else
				{
					check_flag = 0;						
					printf("ping baidu\n");
					if(lte_ping(CHECK_BAIDU) == 1)	
					{
						check_flag ++;
						printf("check_lte: sleep for %d(sec), check_flag=%d\n", check_interval*2, check_flag);
						sleep(check_interval*2);
					}
					else
					{
						check_flag = 0;
					}
					printf("ping sohu\n");
					if(lte_ping(CHECK_SOHU) == 1)
					{
						check_flag ++;
						printf("check_lte: sleep for %d(sec), check_flag=%d\n", check_interval*2, check_flag);
						sleep(check_interval*2);
					}
					else
					{
						check_flag = 0;
					}		
					printf("ping baidu\n");
                    if(lte_ping_dail(CHECK_BAIDU) == 1)
					{
						check_flag ++;
						printf("check_lte: sleep for %d(sec), check_flag=%d\n", check_interval*3, check_flag);
						sleep(check_interval*3);
					}
					else
					{
						check_flag = 0;
					}
					printf("ping sina\n");
					if(lte_ping(CHECK_SINA) == 1)
					{
						check_flag ++;
					}
					else
					{		
						check_flag = 0;
					}
				}
			}			
			if(check_flag >= 4) {
				printf("check_lte: redail time = %d, thr_dail now\n", check_flag);
				thr_dail(NULL);
				//printf("check_lte: redail time = %d, sysreboot now\n", check_flag);
				//system(REBOOT_NOW);
			}
			printf("check_lte: sleep for %d(sec), check_flag=%d\n", check_interval, check_flag);
			sleep(check_interval);
		}
	}
	pthread_join(mode_tid,NULL);
	return 0;
}
