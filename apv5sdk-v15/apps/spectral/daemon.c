/*
 *
 *       Filename:  daemon.c
 *
 *    Description:  Daemonize functions
 *
 *        Version:  1.0
 *        Created:  Thursday 20 August 2009 11:44:49  IST
 *       Revision:  none
 *       Compiler:  gcc
 *
 *        Company:  Atheros Communications
 *
 *         Credit:
 *                  UNIX Daemon Server Programming Sample Program
 *                  Levent Karakas <levent at mektup dot at> May 2001
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <signal.h>
#include <unistd.h>
#include "daemon.h"

/*
 * The following variable needs to tuned as per the application
 * and requirement, currently this daemonize feature is used only 
 * for the spectraldemo application and these are defined for that
 * feature
 */
#define RUNNING_DIR "/tmp"
#define LOCK_FILE   "sd.lock"
#define LOG_FILE    "sd.log"



void log_message(char *filename, char *message)
{ 
    FILE *logfile;

	logfile=fopen(filename,"a");

	if(!logfile) 
        return;

	fprintf(logfile,"%s\n",message);
	fclose(logfile);

}


void signal_handler(int sig)
{
	switch(sig) {
	case SIGHUP:
		log_message(LOG_FILE,"hangup signal catched");
		break;
	case SIGTERM:
		log_message(LOG_FILE,"terminate signal catched");
		exit(0);
		break;
	}

}

void daemonize()
{
    int i,lfp;
    char str[10];

	if(getppid()==1) 
        return; /* already a daemon */

	i=fork();

	if (i<0) 
        exit(1); /* fork error */
	if (i>0) 
        exit(0); /* parent exits */

	/* child (daemon) continues */
	setsid(); /* obtain a new process group */

	for (i=getdtablesize();i>=0;--i) 
        close(i); /* close all descriptors */

	i=open("/dev/null",O_RDWR); dup(i); dup(i); /* handle standart I/O */

	umask(027); /* set newly created file permissions */

	chdir(RUNNING_DIR); /* change running directory */

	lfp=open(LOCK_FILE,O_RDWR|O_CREAT,0640);

	if (lfp<0) 
        exit(1); /* can not open */

	if (lockf(lfp,F_TLOCK,0)<0) 
        exit(0); /* can not lock */

	/* first instance continues */
	sprintf(str,"%d\n",getpid());
	write(lfp,str,strlen(str)); /* record pid to lockfile */

	signal(SIGCHLD,SIG_IGN); /* ignore child */
	signal(SIGTSTP,SIG_IGN); /* ignore tty signals */
	signal(SIGTTOU,SIG_IGN);
	signal(SIGTTIN,SIG_IGN);
	signal(SIGHUP,signal_handler); /* catch hangup signal */
	signal(SIGTERM,signal_handler); /* catch kill signal */

}
