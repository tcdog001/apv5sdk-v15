/*************************************************************************
Copyright (c) 2013-2015, Autelan Networks. All rights reserved.

This Software is the property of Autelan Networks. 
*************************************************************************/

/*
 * File version info: 
 *
 */
#ifndef __AUTEOS_PROCESSES_H__
#define __AUTEOS_PROCESSES_H__
/*****************************************************************/
/*---------------------------------------------------------*/
/*-------------------Low-Level I/O functions------------------*/
/*---------------------------------------------------------*/

/*
* section 1:include glibc header files
*/
#include "auteos_base.h"
#include "processes_glibc.h"
/*
* section 2:autelan api
*/
extern int autelan_system (const char *command);
extern __pid_t autelan_getpid (void);
extern __pid_t autelan_getppid (void);
extern __pid_t autelan_fork (void);
extern __pid_t autelan_vfork (void);
extern int autelan_execv (const char *filename, char *const argv[]);
extern int autelan_execve (const char *filename, char *const argv[], char *const env[]);
extern int autelan_execvp (const char *filename, char *const argv[]);
extern __pid_t autelan_waitpid (__pid_t pid, int *status_ptr, int options);
extern __pid_t autelan_wait (int *status_ptr);
extern __pid_t autelan_wait4 (__pid_t pid, int *status_ptr, int options, struct rusage *usage);

/*
* section 3: Macro define
*/
/*Below are the functions that are forbidden by auteos*/
#define wait3 autelan_forbidden(wait3)

/*Below are the functions that are replaced by auteos*/
#define system autelan_replaced(system)
#define getpid autelan_replaced(getpid)
#define getppid autelan_replaced(getppid)
#define fork autelan_replaced(fork)
#define vfork autelan_replaced(vfork)
#define execv autelan_replaced(execv)
#define execl autelan_forbidden(execl)
#define execve autelan_replaced(execve)
#define execle autelan_forbidden(execle)
#define execvp autelan_replaced(execvp)
#define execlp autelan_forbidden(execlp)
#define waitpid autelan_replaced(waitpid)
#define wait autelan_replaced(wait)
#define wait4 autelan_replaced(wait4)

#endif /* __AUTEOS_PIPE_AND_FIFO_GLIBC_H__ */
