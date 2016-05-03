/*************************************************************************
Copyright (c) 2013-2015, Autelan Networks. All rights reserved.

This Software is the property of Autelan Networks. 
*************************************************************************/

/*
 * File version info: 
 *
 */
#ifndef __AUTEOS_SIGNAL_H__
#define __AUTEOS_SIGNAL_H__
/******************************************************************************/
/*
* section 1:include glibc header files
*/
#include "auteos_base.h"
#include "signal_glibc.h"
/******************************************************************************/

/*
* section 2:autelan api
*/
extern void     autelan_psignal (int signum, const char *message);
extern __sighandler_t autelan_signal (int signum, __sighandler_t action);
extern int      autelan_sigaction (int signum, const struct sigaction *__restrict action, struct sigaction *__restrict old_action);
extern int      autelan_raise (int signum);
extern int      autelan_kill (pid_t pid, int signum);
extern int      autelan_killpg (int pgid, int signum);
extern int      autelan_sigemptyset (sigset_t *set);
extern int      autelan_sigfillset (sigset_t *set);
extern int      autelan_sigaddset (sigset_t *set, int signum);
extern int      autelan_sigdelset (sigset_t *set, int signum);
extern int      autelan_sigismember (const sigset_t *set, int signum);
extern int      autelan_sigprocmask (int how, const sigset_t *__restrict set, sigset_t *__restrict oldset);
extern int      autelan_sigpending (sigset_t *set);
extern int      autelan_pause ();
extern int      autelan_sigsuspend (const sigset_t *set);
extern int      autelan_sigaltstack (const stack_t *__restrict stack, stack_t *__restrict oldstack);
extern int      autelan_siginterrupt (int signum, int failflag);
extern int      autelan_sigmask (int signum);

/*
* section 3:forbidden all glibc api
*/
#define strsignal       autelan_forbidden(strsignal)
#define psignal         autelan_replaced(psignal)
#define signal          autelan_replaced(signal)
#define sysv_signal     autelan_forbidden(sysv_signal)
#define ssignal         autelan_forbidden(ssignal)
//#define sigaction       autelan_replaced(sigaction)
#define raise           autelan_replaced(raise)
#define gsignal         autelan_forbidden(gsignal)
#define kill            autelan_replaced(kill)
#define killpg          autelan_replaced(killpg)
#define sigemptyset     autelan_replaced(sigemptyset)
#define sigfillset      autelan_replaced(sigfillset)
#define sigaddset       autelan_replaced(sigaddset)
#define sigdelset       autelan_replaced(sigdelset)
#define sigismember     autelan_replaced(sigismember)
#define sigprocmask     autelan_replaced(sigprocmask)
#define sigpending      autelan_replaced(sigpending)
#define pause           autelan_replaced(pause)
#define sigsuspend      autelan_replaced(sigsuspend)
#define sigaltstack     autelan_replaced(sigaltstack)
#define sigstack        autelan_forbidden(sigstack)
#define sigvec          autelan_forbidden(sigvec)
#define siginterrupt    autelan_replaced(siginterrupt)
#define sigblock        autelan_forbidden(sigblock)
#define sigsetmask      autelan_forbidden(sigsetmask)
#define sigpause        autelan_forbidden(sigpause)

#if 0
#define sigmask         autelan_replaced(sigmask)
#endif

/******************************************************************************/
#endif /* __AUTEOS_SIGNAL_H__ */
