/****************************
 *
 * to use in the user space 
 * with the module acc
 * autelan
 * 2014.05.21
 *
 ****************************/
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include "check_acc.h"
acc_op_args acc_u_op_args;

#ifdef LTEFI
#define MODDEV				"/dev/acc"
#endif

#ifdef LTEFI_V2
#define MODDEV				"/dev/acc"
#endif

enum LETFISubType {
	//0,//reserved
	AP_LEAVE=1,			//lte-fi quit reason
	LTE_MODE=2,			//2,//for lte mode atc -c "at\$qcsysmode"
};

int set_acc_gpio_timer(acc_op_args * ops)
{
	int fd;
	int retval;
	fd = open(MODDEV, 0);
	retval = ioctl(fd, SET_ACC_GPIO_TIMER, ops);
	if (0 ==retval) {
		printf("\nioctl success\n");
		return 0;
	}	
	else {
		printf("Read failed return [%d]\n",retval);
		return -1;
	}		
	close(fd);
	return retval;	
}
int get_acc_gpio_timer(acc_op_args * ops)
{
	int fd;
	int retval;
	fd = open(MODDEV, 0);
	retval = ioctl(fd, GET_ACC_GPIO_TIMER, ops);
	if (0 ==retval) {
		printf("\nioctl success\n");
		return 0;
	}	
	else {
		printf("Read failed return [%d]\n",retval);
		return -1;
	}		
	close(fd);
	return retval;	
}

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

int main(int argc, char **argv) 
{
	int ret, opt;

	while ((opt = getopt(argc,argv,"s:D:hA")) != -1)
	{
		switch (opt) {
			case 'h':
				printf("check_lte\n");
				printf("          -h (get help, exist immediately)\n");
				printf("          -D 300 (set acc_gpio timer (sec))\n");
				printf("          -A(get acc_gpio timer (sec))\n");
				return 0;				
			case 'D':
				acc_u_op_args.num = strtoul(optarg,NULL,10);
				ret == set_acc_gpio_timer(&acc_u_op_args);
				return 0;
			case 'A':
				ret == get_acc_gpio_timer(&acc_u_op_args);
				return 0;
			case 's':
				signal_f(strtoul(optarg,NULL,10));
				return 0;
			default :
				printf("wrong input\n");
		}		
	}
	printf("check_lte: open acc\n");
	int fd_wrcpu = open(MODDEV, O_RDWR);
	if (fd_wrcpu < 0) 
	{
		printf("check_lte: can't open acc mode dev!\n"); 	
	}
	else		
	{
		signal(SIGIO, signal_f);
		printf("fcntl(fd_wrcpu)\n");
		fcntl(fd_wrcpu, F_SETOWN, getpid());  
		fcntl(fd_wrcpu, F_SETFL, fcntl(fd_wrcpu, F_GETFL) | FASYNC); 
	}

	while (1)
	{
		sleep(1000);
	}
	close(fd_wrcpu);
	return 0;
}

