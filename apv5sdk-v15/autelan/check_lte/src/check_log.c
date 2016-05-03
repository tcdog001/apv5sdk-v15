/****************************
 *
 * to use in the user space 
 * with the module wrcpu.o
 * autelan
 * 2014.03.23
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
#include <time.h>

#define UP_TIME 	15
#define KEEP_TIME 	7

#define DAY_HOUR_MIN 	"cat /proc/uptime| awk -F '.' '{print $1}'"
//#define DAY_HOUR_MIN 	"cat /proc/uptime| awk -F '.' '{run_days=$1 / 86400;run_hour=($1 % 86400)/3600;run_minute=($1 % 3600)/60;run_second=$1 % 60;printf(\"%d:%d:%d\",run_days,run_hour,run_minute)}'"
//#define UP_TIME_NOW 	"uptime | awk -F ':' '{print $2}'"
//#define UP_HOUR_NOW 	"uptime | awk -F ':' '{print $1}'"
#define MINIHTTP_STOP 	"/etc/init.d/mini_httpd_udisk stop"
#define MINIHTTP_START  "/etc/init.d/mini_httpd_udisk start"

#define MINIHTTP_LOG_DEF	"/tmp/udisk/www/minihttp.log"
#define MINIHTTP_LOG_PATH 	"/tmp/udisk/log/http"

int up_time = UP_TIME;
char* up_user;
char* up_passwd;
char* up_ip;

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
int file_num(void)
{
	char str[256];
	char temp_str[256] = {0};

	memset(temp_str, 0, sizeof(temp_str));
	sprintf(temp_str, "ls %s | wc -l", MINIHTTP_LOG_PATH);
	PopenFile(temp_str, str, sizeof(str));
//	printf("-file_num----%d-------%s\n", atoi(str), temp_str);
	return atoi(str);
}
void ftp_upload(void)
{
	char str[256], str1[256];
	char temp_str[256] = {0};
	int num = 0, ret = 0;

	printf("-------ftp_upload-------\n");
	num = file_num();
	while(num){
		memset(temp_str, 0, sizeof(temp_str));
		sprintf(temp_str, "ls -l %s| awk -F ' ' '{print $9}'|sed -n '%dp'", MINIHTTP_LOG_PATH, num);
//		printf("------------%s\n", temp_str);
		PopenFile(temp_str, str, sizeof(str));

		memset(temp_str, 0, sizeof(temp_str));
		sprintf(temp_str, "ls -l %s| awk -F ' ' '{print $9}'|sed -n '%dp'| awk -F '.' '{print $3}'", MINIHTTP_LOG_PATH, num);
//		printf("------------%s\n", temp_str);
		PopenFile(temp_str, str1, sizeof(str1));

		if(strlen(str)&& strlen(str1)) {

			memset(temp_str, 0, sizeof(temp_str));
			sprintf(temp_str, "ftpput -u %s -p %s %s /%s/%s %s/%s", up_user, up_passwd, up_ip, str1, str, MINIHTTP_LOG_PATH, str);
//			printf("------------%s\n", temp_str);

			ret = system(temp_str);
			if(0 == ret)
			{
				memset(temp_str, 0, sizeof(temp_str));
				sprintf(temp_str, "rm -rf %s/%s", MINIHTTP_LOG_PATH, str);
//				printf("------------%s\n", temp_str);
				system(temp_str);
			}
		}
		num--;
	}	
	return;
}
void * uptime_now(void *arg)
{
	int uptime_min = 0, uptime_hour = 0, uptime_day = 0;
	int up_hour = 0, up_min = 0;
	char str[256];

	while(1)
	{
		PopenFile(DAY_HOUR_MIN, str, sizeof(str));
		if(strlen(str)) {
			uptime_day = (atoi(str) / 86400);
			uptime_hour = (atoi(str)% 86400)/3600;
			uptime_min = (atoi(str)% 3600)/60;
			printf("----uptime is %d:%d:%d--------\n", uptime_day, uptime_hour, uptime_min);
			if(up_time < 60){
				up_min = up_time;
				if(up_min <= uptime_min) {
					ftp_upload();
					return 0;
				}
			}
			else{
				up_hour = up_time / 60;
				up_min = up_time % 60;
				if(up_hour == uptime_hour) {
					uptime_min = strtoul(str, NULL, 10);
					if(up_min <= uptime_min) {
						ftp_upload();
						return 0;
					}
				}
				if(up_hour < uptime_hour) {
					ftp_upload();
					return 0;					
				}
			}
		}
		sleep(60);
	}
}
int main(int argc, char **argv) 
{
	int opt, keep_time = KEEP_TIME, check_flag = 0, filenum, flag_today;
	char str[256], str_host[30];
	char temp_str[256] = {0};
	time_t now;
	struct tm* t;
	char year_month_date[100] , date_def[100], date_now[100];
	int date_def_num, date_now_num, keep_date= 0; 
	int err;
	pthread_t uptime_tid;

	while ((opt = getopt(argc,argv,"t:d:u:p:I:h")) != -1)
	{
		switch (opt) {
			case 'h':
				printf("check_log\n");
				printf("          -h (get help, exist immediately)\n");
				printf("          -I 192.168.1.2 (ftpput IP)\n");
				printf("          -t 15 (ftpput start after ap running about 5~180 min)\n");
				printf("          -u test2 (ftpput user)\n");
				printf("          -p logitech (ftpput passwd)\n");
				printf("          -d 7 (keep mini_httpd log for 7 days at most)\n");
				return 0;				
			case 'I':
				up_ip = optarg;
				break;
			case 't':
				up_time = strtoul(optarg,NULL,10);
				if(up_time < 5 || up_time >180){
					printf("		  -t 15 (ftpput start after ap running about 5~180 min)\n");
					up_time = 15;
				}
				break;
			case 'u':
				up_user = optarg;
				break;
			case 'p':
				up_passwd = optarg;
				break;
			case 'd':
				keep_time = strtoul(optarg,NULL,10);
				break;
			default :
				printf("wrong input\n");
		}		
	}
	printf("check_log -u %s -p %s -d %d -t %d -I %s\n", up_user, up_passwd, keep_time, up_time, up_ip);
//	printf("upload running after ap start in %d min\n", up_time);

//	printf("pthread_create---------------\n");
	err = pthread_create(&uptime_tid, NULL, uptime_now, NULL);
	if (err != 0)
		printf("can't create thread: %s\n", strerror(err));

	sprintf(temp_str, "showsysinfo | awk -F 'MAC:' '/MAC/{print $2}'| sed 's/:/-/g'"); 
	PopenFile(temp_str, str_host, sizeof(str_host));	

	now = time( (time_t*) 0 );
	t = localtime( &now );
	(void) strftime( date_def, sizeof(date_def), "%j", t );
//	printf("date is %s of the year\n", date_def);
	date_def_num = atoi(date_def);

	while(1)
	{
		sleep(5*60);

		now = time( (time_t*) 0 );
		t = localtime( &now );
		(void) strftime( year_month_date, sizeof(year_month_date), "%Y%m%d", t );
		(void) strftime( date_now, sizeof(date_now), "%j", t );
//		printf("%s of the year, start at %s of the year\n", date_now, date_def);
		date_now_num = atoi(date_now);

		if(date_now_num != date_def_num)
		{
			system(MINIHTTP_STOP);
			memset(temp_str, 0, sizeof(temp_str));
			sprintf(temp_str, "mv %s %s/%s.http.%s.log", MINIHTTP_LOG_DEF, MINIHTTP_LOG_PATH, str_host, year_month_date);
			system(temp_str);
			system(MINIHTTP_START);

			date_def_num = date_now_num;
		}
		else{
			memset(temp_str, 0, sizeof(temp_str));
			sprintf(temp_str, "cp %s %s/%s.http.%s.log", MINIHTTP_LOG_DEF, MINIHTTP_LOG_PATH, str_host, year_month_date);
			system(temp_str);
		}		

		filenum = file_num();
		while(filenum > 7)
		{
			memset(temp_str, 0, sizeof(temp_str));
			sprintf(temp_str, "ls -l %s | awk -F ' ' '{print $9}'|sed -n '%dp'", MINIHTTP_LOG_PATH, 1);
//			printf("----%d--------%s\n", filenum, temp_str);
			PopenFile(temp_str, str, sizeof(str));
			
			sprintf(temp_str, "rm -rf %s/%s", MINIHTTP_LOG_PATH,str);
//			printf("------------%s\n", temp_str);
			system(temp_str);
			filenum--;
		}
//		sleep(50*60);
	}
	pthread_join(uptime_tid, NULL);

	return 0;
}
