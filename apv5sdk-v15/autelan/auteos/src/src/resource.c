/*************************************************************************
Copyright (c) 2013-2015, Autelan Networks. All rights reserved.

This Software is the property of Autelan Networks. 
*************************************************************************/
#include "resource_glibc.h"



int autelan_getrusage(int processes, struct rusage *rusage)
{
    return getrusage(processes,rusage);
}

int autelan_setrlimit(int resource, const struct rlimit *rlp)
{
    return setrlimit(resource, rlp);
}


int autelan_sched_setscheduler(__pid_t pid, int policy, const struct sched_param *param)
{
    return sched_setscheduler(pid, policy,param);
}

int autelan_sched_getscheduler(__pid_t pid)
{
    return sched_getscheduler(pid);
}

int autelan_sched_setparam (__pid_t pid, struct sched_param *param)
{
    return sched_setparam(pid, param);
}

int autelan_sched_getparam(__pid_t pid, struct sched_param *param)
{
    return sched_getparam(pid,param);
}

int autelan_sched_get_priority_min(int policy)
{
    return sched_get_priority_min(policy);
}

int autelan_sched_get_priority_max(int policy)
{
    return sched_get_priority_max(policy);
}

int autelan_sched_rr_get_interval(__pid_t pid, struct timespec *interval)
{
    return sched_rr_get_interval(pid, interval);
}

int autelan_sched_yield(void)
{
    return sched_yield();
}

int autelan_getpriority(int class, int id)
{
    return getpriority(class,id);
}

int autelan_setpriority(int class, int id, int niceval)
{
    return setpriority(class, id,niceval);
}

int autelan_nice(int increment)
{
    return nice(increment);
}

int autelan_getpagesize(void)
{
    return getpagesize();
}



