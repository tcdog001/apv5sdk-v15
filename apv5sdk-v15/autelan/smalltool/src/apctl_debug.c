/* Begin: Add by dongzw for apctl debug 2013-01-18 */

#include <auteos.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <signal.h>
#include <errno.h>

#define SIG_DEBUG_SWITCH SIGUSR1+1

int PopenFile(char *cmd_str,char *str,int len);
int GetProcID(int *proc_id);
int GetStdoutDev(int proc_id,int *stdout_dev);

int main(int argc, char **argv)
{
	int proc_id = 0;
	int stdout_dev = 0;
	int self_pid = autelan_getpid();

	union sigval mysigval;

	autelan_printf("Sender self process id: %d.\n", self_pid);
	GetStdoutDev(self_pid, &stdout_dev);
	mysigval.sival_int = stdout_dev;

	if(argc > 1)
		proc_id = atoi(argv[1]);
	else
		if(!GetProcID(&proc_id)) return;
	autelan_printf("Sender gets apctl process id: %d.\n\n", proc_id);

	if(sigqueue(proc_id, SIG_DEBUG_SWITCH, mysigval) == -1)
		autelan_printf("Sender sent signal error!\n\n");
	else
	    	autelan_printf("Sender sent signal ok!\n\n");

	return 1;
}

int PopenFile(char *cmd_str, char *str, int len)
{
	FILE *fp = NULL; 

	if(cmd_str == NULL || str == NULL)
		return 0;
	   
	memset(str, 0, len);		   
	
	fp = autelan_popen(cmd_str, "r");  
	if(fp) {	   
		autelan_fgets(str, len, fp);	   
		if(str[strlen(str)-1] == '\n')	   
		   	str[strlen(str)-1] = '\0';	   
		autelan_pclose(fp); 	   

		return 1;    
	} else {	   
		autelan_printf("cmd: %s error! %s\n", cmd_str, strerror(errno)); 	   

		return 0;   
	}
}

int GetProcID(int *proc_id)
{
	char str_cmd[128] = {0}, str[32] = {0};

	memset(str_cmd, 0, sizeof(str_cmd));
	autelan_sprintf(str_cmd, "ps |awk '/apctl/{if($5==\"apctl\"){print $1}}' | wc -l");
	PopenFile(str_cmd, str, sizeof(str));
	autelan_printf("apctl count:%d\n", atoi(str));

	if(atoi(str) == 0) {
		autelan_printf("get apctl process id failed!\nplease make sure apctl has been started.\n");

		return 0;
	}

	if(PopenFile("ps | awk '/apctl/{if($5==\"apctl\"){print $1}}' | awk '{if(NR==1){print $1}}'", str, sizeof(str))) {
		*proc_id = atoi(str);
	} else {
		autelan_printf("get apctl process id failed!\nplease make sure apctl has been started.\n");

		return 0;
	}

	return 1;
}

int GetStdoutDev(int proc_id, int *stdout_dev)
{
	 char str_cmd[128] = {0}, str[64] = {0};
	 int logon_type_serial = 0;

	 memset(str_cmd, 0, sizeof(str_cmd));
	 autelan_sprintf(str_cmd, "ls -l /proc/%d/fd/1 | awk -F\">\" '/ttyS/{print}' | wc -l", proc_id);
	 PopenFile(str_cmd, str, sizeof(str));
	 logon_type_serial = atoi(str);
	
	 if(logon_type_serial == 0) {
		 //current is telnet
		 autelan_printf("current logon is via telnet.\n");
		 memset(str_cmd, 0, sizeof(str_cmd));
		 autelan_sprintf(str_cmd, "ls -l /proc/%d/fd/1 | awk -F\"/\" '{print $8}'", proc_id);
		 PopenFile(str_cmd, str, sizeof(str));
		 *stdout_dev = atoi(str);
		 *stdout_dev += 10;
	 } else {
		 //current is serial port
		 autelan_printf("current logon is via serial.\n");
		 memset(str_cmd, 0, sizeof(str_cmd));
		 autelan_sprintf(str_cmd, "ls -l /proc/%d/fd/1 | awk -F\"S\" '/ttyS/{print $2}'", proc_id);
		 PopenFile(str_cmd, str, sizeof(str));
		 *stdout_dev = atoi(str);
	 }

	return 1;
}

/* End: Add by dongzw for apctl debug 2013-01-18 */

