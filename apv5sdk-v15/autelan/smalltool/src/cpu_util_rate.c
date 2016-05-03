/**********************************************************************************
* Copyright (c) 2008-2011  Beijing Autelan Technology Co. Ltd.
* All rights reserved.
*
* filename: cpu_util_rate.c
* description:  implementation for get cpu util rate.
* 
*
* 
************************************************************************************/

#include <auteos.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
float get_cpu_util(int second_num_tmp);
int main(int argc,char *argv[])
{
	int second_num;
	char cmdbuf[64];
	if(argc<2){
		autelan_printf("Usage : cpu_util_rate 5[second num]\n");
		return 0;
	}//rbk add 20100224
	second_num = atoi(argv[1]);
//	printf("second_num : %d\n\n",second_num);
	float p = get_cpu_util(second_num);
	// printf("p-->%.1f\n",p);
 	autelan_printf("\nCpu Utilization           :%.1f %%\n",p);
	memset(cmdbuf, 0, 64);
	autelan_sprintf(cmdbuf, "echo %.2f%% > /tmp/cpu_ratio", p);
	autelan_system(cmdbuf);
	autelan_sprintf(cmdbuf, "echo %d >> /tmp/cpu_ratio", (int)(p*100));
	autelan_system(cmdbuf);
	return 0;
}

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
 
float get_cpu_util(int second_num_tmp)
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
	autelan_printf ("%s  user:%ld  nice:%ld  autelan_system:%ld  idle:%ld\n", name, user1, nice1, system1, idle1);
 	sum1=user1+ nice1+system1+idle1;
 	autelan_fclose(fp);
 
 	autelan_sleep(second_num_tmp);
 
 if( (fp=autelan_fopen("/proc/stat","r") )==NULL)
 {
 	autelan_printf("Can not open file!");
 	exit(1);
 }
 
 autelan_fgets (buffer, sizeof(buffer),fp);
 	autelan_sscanf (buffer, "%s  %ld  %ld  %ld  %ld", name, &user2,&nice2,&system2, &idle2);
 	autelan_printf ("%s  user:%ld  nice:%ld  autelan_system:%ld  idle:%ld\n", name, user2, nice2, system2, idle2);
 	sum2=user2+ nice2+system2+idle2;
 	util=(float)100.0*(user2-user1+system2-system1)/(sum2-sum1);
 	user_space = (float)100.0*(user2-user1)/(sum2-sum1);
 	kernel_space = (float)100.0*(system2-system1)/(sum2-sum1);
 	idle_space =(float)100.0*(idle2-idle1)/(sum2-sum1); 
 	autelan_printf("\nUser Space Utilization   :%0.1f %%\n",user_space);
 	autelan_printf("\nKernel Space Utilization :%0.1f %%\n",kernel_space);
 	autelan_printf("\nIdle Space Utilization   :%0.1f %%\n",idle_space);
// printf("Cpu Utilization:%.1f  %%\n",util);
 	 
 	autelan_fclose(fp);
 
 	return util;
}
