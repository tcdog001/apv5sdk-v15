/*************************************************************************
Copyright (c) 2013-2015, Autelan Networks. All rights reserved.

This Software is the property of Autelan Networks. 
*************************************************************************/
#include "signal_glibc.h"

void autelan_psignal (int signum, const char *message)
{
    psignal(signum, message);
}

__sighandler_t autelan_signal (int signum, __sighandler_t action)
{
    return signal(signum, action);
}

int autelan_sigaction (int signum, const struct sigaction *__restrict action, struct sigaction *__restrict old_action)
{
    return sigaction(signum, action, old_action);
}

int autelan_raise (int signum)
{
    return raise(signum);
}

int autelan_kill (__pid_t pid, int signum)
{
    return kill(pid, signum);
}

int autelan_killpg (int pgid, int signum)
{
    return killpg(pgid, signum);
}

int autelan_sigemptyset (sigset_t *set)
{
    return sigemptyset(set);
}

int autelan_sigfillset (sigset_t *set)
{
    return sigfillset(set);
}

int autelan_sigaddset (sigset_t *set, int signum)
{
    return sigaddset(set, signum);
}

int autelan_sigdelset (sigset_t *set, int signum)
{
    return sigdelset(set, signum);
}

int autelan_sigismember (const sigset_t *set, int signum)
{
    return sigismember(set, signum);
}

int autelan_sigprocmask (int how, const sigset_t *__restrict set, sigset_t *__restrict oldset)
{
    return sigprocmask(how, set, oldset);
}

int autelan_sigpending (sigset_t *set)
{
    return sigpending(set);
}

int autelan_pause ()
{
    return pause();
}

int autelan_sigsuspend (const sigset_t *set)
{
    return sigsuspend(set);
}

int autelan_sigaltstack (const stack_t *__restrict stack, stack_t *__restrict oldstack)
{
    return sigaltstack(stack, oldstack);
}

int autelan_siginterrupt (int signum, int failflag)
{
    return siginterrupt(signum, failflag);
}

int autelan_sigmask (int signum)
{
    return sigmask(signum);
}
