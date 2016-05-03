/*************************************************************************
Copyright (c) 2013-2015, Autelan Networks. All rights reserved.

This Software is the property of Autelan Networks. 
*************************************************************************/
#ifndef __AUTEOS_DATA_AND_TIME_H__
#define __AUTEOS_DATA_AND_TIME_H__
/******************************************************************************/
/*
* section 1:include glibc header files
*/
#include "auteos_base.h"
#include "date_and_time_glibc.h"
/******************************************************************************/

/*
* section 2:autelan api
*/
extern double   autelan_difftime (time_t time1, time_t time0);
extern clock_t  autelan_clock (void);
extern clock_t  autelan_times (struct tms *buffer);
extern time_t   autelan_time (time_t *result);
extern int      autelan_stime (time_t *newtime);
extern int      autelan_gettimeofday (struct timeval *tp, struct timezone *tzp);
extern int      autelan_settimeofday (const struct timeval *tp, const struct timezone *tzp);
extern int      autelan_adjtime (const struct timeval *delta, struct timeval *olddelta);
extern int      autelan_adjtimex (struct timex *timex);
extern struct tm * autelan_localtime (const time_t *time);
extern struct tm * autelan_localtime_r (const time_t *time, struct tm *resultp);
extern struct tm * autelan_gmtime (const time_t *time);
extern struct tm * autelan_gmtime_r (const time_t *time, struct tm *resultp);
extern time_t   autelan_mktime (struct tm *brokentime);
extern time_t   autelan_timelocal (struct tm *brokentime);
extern time_t   autelan_timegm (struct tm *brokentime);
extern char *   autelan_asctime (const struct tm *brokentime);
extern char *   autelan_asctime_r (const struct tm *brokentime, char *buffer);
extern char *   autelan_ctime (const time_t *time);
extern char *   autelan_ctime_r (const time_t *time, char *buffer);
extern size_t   autelan_strftime (char *s, size_t size, const char *template, const struct tm *brokentime);
extern void     autelan_tzset (void);
extern int      autelan_setitimer (int which, struct itimerval *new, struct itimerval *old);
extern int      autelan_getitimer (int which, struct itimerval *old);
extern unsigned int autelan_alarm (unsigned int seconds);
extern unsigned int autelan_sleep (unsigned int seconds);
extern int      autelan_nanosleep (const struct timespec *requested_time, struct timespec *remaining);


/*
* section 3:forbidden all glibc api
*/
#define difftime        autelan_replaced(difftime)
#define clock           autelan_replaced(clock)
#define times           autelan_replaced(times)
#define time            autelan_replaced(time)
#define stime           autelan_replaced(stime)
#define gettimeofday    autelan_replaced(gettimeofday)
#define settimeofday    autelan_replaced(settimeofday)
#define adjtime         autelan_replaced(adjtime)
#define adjtimex        autelan_replaced(adjtimex)
#define localtime       autelan_replaced(localtime)
#define localtime_r     autelan_replaced(localtime_r)
#define gmtime          autelan_replaced(gmtime)
#define gmtime_r        autelan_replaced(gmtime_r)
#define mktime          autelan_replaced(mktime)
#define timelocal       autelan_replaced(timelocal)
#define timegm          autelan_replaced(timegm)
#define ntp_gettime     autelan_forbidden(ntp_gettime)
#define ntp_adjtime     autelan_forbidden(ntp_adjtime)
#define asctime         autelan_replaced(asctime)
#define asctime_r       autelan_replaced(asctime_r)
#define ctime           autelan_replaced(ctime)
#define ctime_r         autelan_replaced(ctime_r)
#define strftime        autelan_replaced(strftime)
#define strptime        autelan_forbidden(strptime)
#define getdate         autelan_forbidden(getdate)
#define getdate_r       autelan_forbidden(getdate_r)
#define tzset           autelan_replaced(tzset)
#define setitimer       autelan_replaced(setitimer)
#define getitimer       autelan_replaced(getitimer)
#define alarm           autelan_replaced(alarm)
#define sleep           autelan_replaced(sleep)
#define nanosleep       autelan_replaced(nanosleep)

/******************************************************************************/
#endif /* __AUTEOS_DATA_AND_TIME_H__ */
