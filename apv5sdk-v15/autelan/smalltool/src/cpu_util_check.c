/**********************************************************************************
* Copyright (c) 2008-2011  Beijing Autelan Technology Co. Ltd.
* All rights reserved.
*
* filename: cpu_util_check.c
* description:  implementation for set /eth0/maxnumber after getting cpu util ratio.
* 
*
* 
************************************************************************************/

#include <auteos.h>
#include <stdio.h>
#include <stdlib.h>
//#include <unistd.h>
//#include <pthread.h>

#define STR_LENGTH 128
#define MIN_MAXNUM 14
#define MAX_MAXNUM 20
#define OFFSET 3

int second_num = 180;
char *ethmaxnum = "/proc/sys/net/eth0/maxnumber";

/*-------------------------------------------------------------------*
 *  *  function get_cpu_util();
 **
 **  Return:
 **      returns status of util(cpu utilization)
 **  Parameters:
 **    
 **  Remarks:
 **      This function mainly gets the cpu utilization .
 **/
 
float get_cpu_util(void)
{
	FILE *fp;
	char buffer[128],name[15];
	long user1, nice1, system1, idle1,user2, nice2, system2, idle2,sum1,sum2;
	float util,user_space,kernel_space,idle_space;
 
	if( (fp=autelan_fopen("/proc/stat","r") )==NULL)
	{
		autelan_printf("Can not open file!");
		exit(1);
	}
 
	autelan_fgets (buffer, sizeof(buffer),fp);
	autelan_sscanf (buffer, "%s  %ld  %ld  %ld  %ld", name, &user1,&nice1,&system1, &idle1);
//	printf ("%s  user:%ld  nice:%ld  system:%ld  idle:%ld\n ", name, user1, nice1, system1, idle1);
	sum1=user1+ nice1+system1+idle1;
	autelan_fclose(fp);

	autelan_sleep(second_num);

	if( (fp=autelan_fopen("/proc/stat","r") )==NULL)
	{
		autelan_printf("Can not open file!");
		exit(1);
	}

	autelan_fgets (buffer, sizeof(buffer),fp);
	autelan_sscanf (buffer, "%s  %ld  %ld  %ld  %ld", name, &user2,&nice2,&system2, &idle2);
//	printf ("%s  user:%ld  nice:%ld  system::%ld  idle:%ld\n ", name, user2, nice2, system2, idle2);
	sum2=user2+ nice2+system2+idle2;
	util=(float)100.0*(user2-user1+system2-system1)/(sum2-sum1);
	user_space = (float)100.0*(user2-user1)/(sum2-sum1);
	kernel_space = (float)100.0*(system2-system1)/(sum2-sum1);
	idle_space =(float)100.0*(idle2-idle1)/(sum2-sum1); 
//	printf("\nUser Space Utilization   :%0.1f %%\n",user_space);
//	printf("\nKernel Space Utilization :%0.1f %%\n",kernel_space);
//	printf("\nIdle Space Utilization   :%0.1f %%\n",idle_space);
	// printf("Cpu Utilization:%.1f  %%\n",util);

	autelan_fclose(fp);

	return util;
}

void *CWGetAPCpuUtil(void *arg)
{
	while(1)
	{
		FILE *fp = NULL;
		char str[STR_LENGTH];
		int maxnum = 0;
		float p = get_cpu_util();
//		printf("\nCpu Utilization            :%.1f %%\n", p);
		if(p > 80)
		{
			if((fp = autelan_fopen(ethmaxnum, "r")) != NULL)
			{
//				printf("file is exist!\n");
				memset(str, 0, STR_LENGTH);
				autelan_fgets(str, sizeof(str), fp);
				str[strlen(str)-1] = '\0';
				maxnum = atoi(str);
				autelan_fclose(fp);
			}
			if(maxnum > (MIN_MAXNUM+OFFSET-1))
			{
				maxnum -= OFFSET;
				memset(str, 0, STR_LENGTH);
				autelan_sprintf(str, "echo %d > %s", maxnum, ethmaxnum);
				autelan_system(str);
//				printf("\nCpu utilization is larger than 80%, decrease the weight num\n");
			}
			
		}
		else if(p < 50)
		{
			if((fp = autelan_fopen(ethmaxnum, "r")) != NULL)
			{
//				printf("file is exist!\n");
				memset(str, 0, STR_LENGTH);
				autelan_fgets(str, sizeof(str), fp);
				str[strlen(str)-1] = '\0';
				maxnum = atoi(str);
				autelan_fclose(fp);
			}
			if(maxnum < (MAX_MAXNUM-OFFSET+1))
			{
				maxnum += OFFSET;
				memset(str, 0, STR_LENGTH);
				autelan_sprintf(str, "echo %d > %s", maxnum, ethmaxnum);
				autelan_system(str);
//				printf("\nCpu utilization is smaller than 50%, increase the weight num\n");
			}
			
		}
	}
}

int main(int argc,char *argv[])
{
	pthread_t thread_cpuUtil;
	
	if(argc>2){
		autelan_printf("Usage : cpu_util_rate Or cpu_util_rate 5[second num]\n");
		return 0;
	}//rbk add 20100224
	if(argc == 2)
		second_num = atoi(argv[1]);
//	printf("second_num : %d\n\n", second_num);
	if(pthread_create(&thread_cpuUtil, NULL, CWGetAPCpuUtil, NULL) != 0) {
		autelan_printf("create thread error");
		return -1;
	}
	pthread_join (thread_cpuUtil, NULL);

	return 0;
}
