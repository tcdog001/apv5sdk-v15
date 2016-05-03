/*************************************************************************
Copyright (c) 2013-2015, Autelan Networks. All rights reserved.

This Software is the property of Autelan Networks. 
*************************************************************************/

/*
 * File version info: 
 *
 */
#ifndef __AUTEOS_MEMORY_H__
#define __AUTEOS_MEMORY_H__
/*****************************************************************/
/*---------------------------------------------------------*/
/*-------------------Low-Level I/O functions------------------*/
/*---------------------------------------------------------*/

/*
* section 1:include glibc header files
*/
#include "auteos_base.h"
#include "resource_glibc.h"
/*
* section 2:autelan api
*/
extern int  autelan_getrusage (int processes, struct rusage *rusage);
extern int  autelan_setrlimit (int resource, const struct rlimit *rlp);
extern int  autelan_sched_setscheduler (__pid_t pid, int policy, const struct sched_param *param);
extern int  autelan_sched_getscheduler (__pid_t pid);
extern int  autelan_sched_setparam (__pid_t pid, struct sched_param *param);
extern int  autelan_sched_getparam (__pid_t pid, struct sched_param *param);
extern int  autelan_sched_get_priority_min (int policy);
extern int  autelan_sched_get_priority_max (int policy);
extern int  autelan_sched_rr_get_interval (__pid_t pid, struct timespec *interval);
extern int  autelan_sched_yield (void);
extern int  autelan_getpriority (int class, int id);
extern int  autelan_setpriority (int class, int id, int niceval);
extern int  autelan_nice (int increment);
extern int  autelan_getpagesize (void);


/*
* section 3: Macro define
*/
/*Below are the functions that are forbidden by auteos*/


/*Below are the functions that are replaced by auteos*/
#define getrusage autelan_replaced(getrusage)
#define vtimes autelan_forbidden(vtimes)
#define getrlimit autelan_forbidden(getrlimit)
#define getrlimit64 autelan_forbidden(getrlimit64)
#define setrlimit autelan_replaced(setrlimit)
#define ulimit autelan_forbidden(ulimit)
#define vlimit autelan_forbidden(vlimit)
#define sched_setscheduler autelan_replaced(sched_setscheduler)
#define sched_getscheduler autelan_replaced(sched_getscheduler)
#define sched_getparam autelan_replaced(sched_getparam)
#define sched_get_priority_min  autelan_replaced(sched_get_priority_min)
#define sched_get_priority_max autelan_replaced(sched_get_priority_max)
#define sched_rr_get_interval autelan_replaced(sched_rr_get_interval)
#define sched_yield autelan_replaced(sched_yield)
#define getpriority autelan_replaced(getpriority)
#define setpriority autelan_replaced(setpriority)
#define nice autelan_replaced(nice)
#define sched_setaffinity autelan_forbidden(sched_setaffinity)
#define sched_getaffinity autelan_forbidden(sched_getaffinity)
#define getpagesize autelan_replaced(getpagesize)
#define get_phys_pages autelan_forbidden(get_phys_pages)
#define get_avphys_pages autelan_forbidden(get_avphys_pages)
#define get_nprocs_conf autelan_forbidden(get_nprocs_conf)
#define get_nprocs autelan_forbidden(get_nprocs)
#define getloadavg autelan_forbidden(getloadavg)

#endif /* __AUTEOS_PIPE_AND_FIFO_GLIBC_H__ */
