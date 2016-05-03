/*
 *
 *       Filename:  daemon.h
 *
 *    Description:  Export for daemon function 
 *
 *        Version:  1.0
 *        Created:  Thursday 20 August 2009 11:38:32  IST
 *       Revision:  none
 *       Compiler:  gcc
 *
 *        Company:  Atheros Communications
 *
 *
 *         Credit:
 *                  UNIX Daemon Server Programming Sample Program
 *                  Levent Karakas <levent at mektup dot at> May 2001
 */


#ifndef     _DAEMON_H
#define     _DAEMON_H

extern  void log_message(char* filename, char *message);
extern  void signal_handler(int sig);
extern  void daemonize();

#endif      /* _DAEMON_H */





