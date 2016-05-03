/*************************************************************************
Copyright (c) 2013-2015, Autelan Networks. All rights reserved.

This Software is the property of Autelan Networks. 
*************************************************************************/
#include "date_and_time_glibc.h"

double autelan_difftime (time_t time1, time_t time0)
{
    return difftime(time1, time0);
}

clock_t autelan_clock (void)
{
    return clock();
}

clock_t autelan_times (struct tms *buffer)
{
    return times(buffer);
}

time_t autelan_time (time_t *result)
{
    return time(result);
}

int autelan_stime (time_t *newtime)
{
    return stime(newtime);
}

int autelan_gettimeofday (struct timeval *tp, struct timezone *tzp)
{
    return gettimeofday(tp, tzp);
}

int autelan_settimeofday (const struct timeval *tp, const struct timezone *tzp)
{
    return settimeofday(tp, tzp);
}

int autelan_adjtime (const struct timeval *delta, struct timeval *olddelta)
{
    return adjtime(delta, olddelta);
}

int autelan_adjtimex (struct timex *timex)
{
    return adjtimex(timex);
}

struct tm * autelan_localtime (const time_t *time)
{
    return localtime(time);
}

struct tm * autelan_localtime_r (const time_t *time, struct tm *resultp)
{
    return localtime_r(time, resultp);
}

struct tm * autelan_gmtime (const time_t *time)
{
    return gmtime(time);
}

struct tm * autelan_gmtime_r (const time_t *time, struct tm *resultp)
{
    return gmtime_r(time, resultp);
}

time_t autelan_mktime (struct tm *brokentime)
{
    return mktime(brokentime);
}

time_t autelan_timelocal (struct tm *brokentime)
{
    return timelocal(brokentime);
}

time_t autelan_timegm (struct tm *brokentime)
{
    return timegm(brokentime);
}

char * autelan_asctime (const struct tm *brokentime)
{
    return asctime(brokentime);
}

char * autelan_asctime_r (const struct tm *brokentime, char *buffer)
{
    return asctime_r(brokentime, buffer);
}

char * autelan_ctime (const time_t *time)
{
    return ctime(time);
}

char * autelan_ctime_r (const time_t *time, char *buffer)
{
    return ctime_r(time, buffer);
}

size_t autelan_strftime (char *s, size_t size, const char *template, const struct tm *brokentime)
{
    return strftime(s, size, template, brokentime);
}

void autelan_tzset (void)
{
    tzset();
}

int autelan_setitimer (int which, struct itimerval *new, struct itimerval *old)
{
    return setitimer(which, new, old);
}

int autelan_getitimer (int which, struct itimerval *old)
{
    return getitimer(which, old);
}

unsigned int autelan_alarm (unsigned int seconds)
{
    return alarm(seconds);
}

unsigned int autelan_sleep (unsigned int seconds)
{
    return sleep(seconds);
}

int autelan_nanosleep (const struct timespec *requested_time, struct timespec *remaining)
{
    return nanosleep(requested_time, remaining);
}


