/**
 *    Name       : proxy.h
 *    
 *    Purpose    : Host proxy header file.
 *
 *    Copyright (c) 2009 Sierra Wireless, Inc.  All rights reserved
 *
 */
#ifndef PROXY_H_
#define PROXY_H_

/* Linux definitions */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <syslog.h>
#include <termios.h>
#include <signal.h>
#include <stdbool.h>


#define PORTNAME_LENGTH 20
#define MAX_PROXY_CLIENTS 5
#define MAX_MODEM_RETRIES 5
#define DEFAULT_TIMEOUT 10000
#define MAX_BUF_SIZE 2048
#define BAUDRATE B115200    /* preferred baud rate */

#define DIAG_PORT "/dev/ttyUSB0"
#define MODM_PORT "/dev/ttyUSB1"

#define PXY_LOG_MSG(log_level, format, args...)  if(g_verbose){printf("[%s:%d]"format"\n",__FILE__,__LINE__,##args);} else{printf("[%s:%d]"format"\n",__FILE__,__LINE__,##args);}



/*function declaration used by proxy*/
void PrintUsage();
int32_t CreateServerSocket(struct sockaddr_in *serv_addr);
int32_t OpenDevice(char* ttyport);
void logerror(const char *errorp, int32_t llerrno);
void logmsg(const char *errorp);
void HandleNewClient();
void* ReadClientSocketThread( void* param );
void* WriteClientSocketThread( void* param );
void* ReadDeviceThread(void* param);
void CloseAllClients();
void WriteToClient(int32_t* s, void* buffer , size_t length );
void delay(uint32_t timeout);
void ExitProxy(int sig);

#endif /*PROXY_H_*/
