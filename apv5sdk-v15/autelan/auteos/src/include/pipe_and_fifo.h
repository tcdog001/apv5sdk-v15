/*************************************************************************
Copyright (c) 2013-2015, Autelan Networks. All rights reserved.

This Software is the property of Autelan Networks. 
*************************************************************************/

/*
 * File version info: 
 *
 */
#ifndef __AUTEOS_PIPE_AND_FIFO_H__
#define __AUTEOS_PIPE_AND_FIFO_H__
/******************************************************************************/
/*
* section 1:include glibc header files
*/
#include "auteos_base.h"
#include "pipe_and_fifo_glibc.h"
/******************************************************************************/

/*
* section 2:autelan api
*/
extern int      autelan_pipe (int filedes[2]);
extern FILE *   autelan_popen (const char *command, const char *mode);
extern int      autelan_pclose (FILE *stream);
extern int      autelan_mkfifo (const char *filename, __mode_t mode);

/*
* section 3:forbidden all glibc api
*/
#define pipe            autelan_replaced(pipe)
#define popen           autelan_replaced(popen)
#define pclose          autelan_replaced(pclose)
#define mkfifo          autelan_replaced(mkfifo)
/******************************************************************************/
#endif /* __AUTEOS_PIPE_AND_FIFO_H__ */
