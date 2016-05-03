#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include <stdlib.h>
#include <signal.h>
#include "CWWTP.h"
#include "WTPDriverInteraction.h"


#define WATCHDOG_INTERVAL 50

CWTimerID gCWWatchDogTimerID;
#if 0
int count = 0;
void set_timer()
{
	struct itimerval itv, oldtv;
	itv.it_interval.tv_sec = 1;
	itv.it_interval.tv_usec = 0;
	itv.it_value.tv_sec = 1;
	itv.it_value.tv_usec = 0;

	setitimer(ITIMER_REAL, &itv, &oldtv);
}

void sigalrm_handler(int sig)
{
	count++;
//	printf("timer signal.. %d\n", count);
}

void *CWWTPWatchdog(void *arg)
{
	int retval;
	printf("----------Enter WTP Watchdog!----------\n");
	signal(SIGALRM, sigalrm_handler);
	set_timer();
	while (1)
	{
		if(count == WATCHDOG_INTERVAL){
			printf("watchdog\n");
	/*		retval = ioctl (leddev,WRCPU_WATCHDOG,NULL);*/
			if(retval !=0)
				CWLog("watchdog write failed");
			count =0;
		}
	}
}
#endif

CWBool CWStartWatchDogTimer(){
	if ((gCWWatchDogTimerID = timer_add(WATCHDOG_INTERVAL, 0, &CWWTPWatchDogTimerExpiredHandler, NULL)) == -1 ) {
		return CW_FALSE;
	}
	return CW_TRUE;
}

/*______________________________________________________________*/
/*	*******************___TIMER HANDLERS___*******************	*/
void CWWTPWatchDogTimerExpiredHandler(void *arg) 
{
	int retval;

 	retval = ioctl (leddev,WRCPU_WATCHDOG,NULL);/*down the blind*/

	if(!CWStartWatchDogTimer()) {
		return;
	}
}

CWBool CWWTPStartWatchdog()
{
	autelan_printf("CWWTPStartWatchdog\n");
	if (!CWErr(CWStartWatchDogTimer())) 
	{
	}
	return CW_TRUE;
}




