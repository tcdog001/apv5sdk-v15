/*************************************************************************
Copyright (c) 2013-2015, Autelan Networks. All rights reserved.

This Software is the property of Autelan Networks. 
*************************************************************************/

/*
 * File version info: 
 *
 */
#ifndef __AUTEOS_SOCKET_LIB_GLIBC_H__
#define __AUTEOS_SOCKET_LIB_GLIBC_H__


/******************************************************************************/
#include <stdlib.h>
#include <sys/socket.h>
#ifdef INCLUDE_LINUX_IF_H
#include <linux/if.h>
#else
#include <net/if.h>
#endif
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
/******************************************************************************/
#endif
