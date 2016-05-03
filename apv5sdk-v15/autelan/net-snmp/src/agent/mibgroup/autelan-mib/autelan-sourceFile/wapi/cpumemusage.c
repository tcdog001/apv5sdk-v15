/**********************************************************************************
* Copyright (c) 2008-2011  Beijing Autelan Technology Co. Ltd.
* All rights reserved.
*
* filename: cpumemusage.c
* description:  implementation for the usage information of CPU and memrory.
* 
*
* 
************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <sys/types.h>

//#ifndef __u16
#define __u16 unsigned short 
//#endif
#define   SET_IF_DESIRED(x,y)     if(x)   *(x)   =   (y)   
#define   _ULL   unsigned   long   long

 static   int   
  five_cpu_numbers(_ULL   *   uret,   _ULL   *   nret,   _ULL   *   sret,   _ULL   *   iret,   _ULL   *   iowait)   
  {   
  static   _ULL   u   =   0,   m   =   0,   s   =   0,   i   =   0,   iw   =   0;   
  _ULL   user_j,   nice_j,   sys_j,   idle_j,   iowait_j   =   0;   
  FILE   *fp;   
  __u16   byte_read;   
  char   buffer[100];   
    
  fp   =   fopen("/proc/stat",   "r");   
  byte_read   =   fread(buffer,   1,   sizeof(buffer)-1,   fp);   
  fclose(fp);   
    
  if   (byte_read==0   ||   byte_read==sizeof(buffer))   
  return   -1;   
  buffer[byte_read]   =   '\0';   
    
  sscanf(buffer,   "cpu   %Lu   %Lu   %Lu   %Lu   %Lu",   &user_j,   &nice_j,   &sys_j,   &idle_j,   &iowait_j);   
  SET_IF_DESIRED(uret,   user_j   -   u);   
  SET_IF_DESIRED(nret,   nice_j   -   m);   
  SET_IF_DESIRED(sret,   sys_j   -   s);   
  /*   Idle   can   go   backwards   one   tick   due   to   kernel   calculation   issues   */   
  SET_IF_DESIRED(iret,   (idle_j   >   i)   ?   (idle_j   -   i)   :   0);   
  SET_IF_DESIRED(iowait,   iowait_j   -   iw);   
  u   =   user_j;   
  m   =   nice_j;   
  s   =   sys_j;   
  i   =   idle_j;   
  iw   =   iowait_j;   
    
  return   0;   
  }   
  /*   
    *   get   the   cpu   usage   
    */   
static int ncm_cab_read_cpu(float   *cpu_usage)   
  {
  _ULL   user_j,   nice_j,   sys_j,   idle_j,   iowait_j   =   0;   
    
  if(five_cpu_numbers(&user_j,   &nice_j,   &sys_j,   &idle_j,   &iowait_j)!=0)   
  return   -1;   
  usleep(500000);   
  if(five_cpu_numbers(&user_j,   &nice_j,   &sys_j,   &idle_j,   &iowait_j)!=0)   
  return   -1;   
  usleep(500000);   
  if(five_cpu_numbers(&user_j,   &nice_j,   &sys_j,   &idle_j,   &iowait_j)!=0)   
  return   -1;   
    
  *cpu_usage   =   (idle_j   *   100.0)   /   (user_j   +   nice_j   +   sys_j   +   idle_j   +   iowait_j);   
    
  if   (*cpu_usage>100)   
  *cpu_usage   =   100.0;   
    
  /*   change   to   useage   */   
  *cpu_usage   =   100.0   -   (*cpu_usage);   
    
          return   0;   
  }   
#undef   _ULL   
  /*   
    *   get   the   mm   usage   
    */   
 
static int getOnceMemUsage(float   *memusage)   
  {   
  FILE   *fp;   
  char   buffer[1024];   
  __u16   byte_read;   
  char   *pos;   
  int   totalmem;   
  int   freemem;   
    
  fp   =   fopen("/proc/meminfo",   "r");   
  byte_read   =   fread(buffer,   1,   sizeof(buffer),   fp);   
  fclose(fp);   
    
  if   (byte_read==0   ||   byte_read==sizeof(buffer))   
  return   -1;   
  buffer[byte_read]   =   '\0';   
  pos   =   strstr(buffer,   "MemTotal:");   
  if   (NULL==pos)     
  return   -1;   
  sscanf(pos,   "MemTotal:               %d   kB",   &totalmem);   
  pos   =   strstr(pos,   "MemFree:");   
  if   (NULL==pos)     
  return   -1;   
  sscanf(pos,   "MemFree:               %d   kB",   &freemem);   
    
  *memusage   =   (totalmem-freemem)     *   100.0   /   totalmem;   
    
	//printf("MemUsage(total:%d   free:%d   MEM:%3.2f)\n",   totalmem,   freemem,   *memusage);   
            
          return   0;   
  }  

#if 0
int main(void)
{
	float mem_usage=0.001;
	float cpu_usage=0.001;


	getOnceMemUsage(&mem_usage);
	
	printf("memusage=%f\n", mem_usage);

  	ncm_cab_read_cpu(&cpu_usage);

	printf("cpuusage=%f\n", cpu_usage);
	
	return 0;
}

#endif 

