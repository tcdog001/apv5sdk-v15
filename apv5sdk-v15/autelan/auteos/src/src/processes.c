/*************************************************************************
Copyright (c) 2013-2015, Autelan Networks. All rights reserved.

This Software is the property of Autelan Networks. 
*************************************************************************/
#include "processes_glibc.h"

int autelan_system(const char *command)
{
    return system(command);
}

__pid_t autelan_getpid(void)
{
    return getpid();
}

__pid_t autelan_getppid(void)
{
    return getppid();
}

__pid_t autelan_fork(void)
{
    return fork();
}

__pid_t autelan_vfork(void)
{
    return vfork();
}

int autelan_execv(const char *filename, char *__const argv[])
{
    return execv(filename,argv);
}

int autelan_execve(const char *filename, char *const argv[], char *const env[])
{
    return execve(filename,argv,env);
}

int autelan_execvp(const char *filename, char *const argv[])
{
    return execvp(filename,argv);
}

__pid_t autelan_waitpid(__pid_t pid, int *status_ptr, int options)
{
    return waitpid(pid,status_ptr,options);
}

__pid_t autelan_wait(int *status_ptr)
{
    return wait(status_ptr);
}

__pid_t autelan_wait4(__pid_t pid, int *status_ptr, int options, struct rusage *usage)
{
    return wait4(pid, status_ptr, options, usage);
}

