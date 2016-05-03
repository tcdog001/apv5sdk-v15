/*************************************************************************
Copyright (c) 2013-2015, Autelan Networks. All rights reserved.

This Software is the property of Autelan Networks. 
*************************************************************************/

/*
 * File version info: 
 *
 */
#ifndef __AUTEOS_LOWIO_H__
#define __AUTEOS_LOWIO_H__

/*****************************************************************/
/*---------------------------------------------------------*/
/*-------------------Low-Level I/O functions------------------*/
/*---------------------------------------------------------*/

/*
* section 1:include glibc header files
*/
#include "auteos_base.h"
#include "low_Level_io_glibc.h"
/*
* section 2:autelan api
*/
extern int autelan_open (const char *pathname, int oflag, ...);
extern ssize_t autelan_read (int filedes, void *buf, size_t nbytes);
extern ssize_t autelan_write (int filedes, const void *buf, size_t nbytes);
extern off_t autelan_lseek (int filedes, off_t offset, int whence);
extern int autelan_close (int __fd);
extern int autelan_dup (int filedes);
extern int autelan_dup2 (int filedes, int filedes2);
extern void *autelan_mmap (void *addr, size_t len, int prot, int flag, int filedes, off_t off);
extern int autelan_munmap (caddr_t addr, size_t len);
extern int autelan_select (int maxfdp1, fd_set *readfds, fd_set *writefds, \
		fd_set *exceptfds, struct timeval *tvptr);

/*
* section 3: Macro define
*/
/*Below are the functions that are forbidden by auteos*/
#define open autelan_replaced(open)
#define open64 autelan_forbidden(open64)
#define creat autelan_forbidden(creat)
#define creat64 autelan_forbidden(creat64)
#define close autelan_replaced(close)
#define read autelan_replaced(read)
#define pread autelan_forbidden(pread)
#define pread64 autelan_forbidden(pread64)
#define write autelan_replaced(write )
#define pwrite autelan_forbidden(pwrite)
#define pwrite64 autelan_forbidden(pwrite64)
#define lseek autelan_replaced(lseek)
#define lseek64 autelan_forbidden(lseek64)
#define fdopen autelan_forbidden(fdopen)
#define fileno autelan_forbidden(fileno)
#define fileno_unlocked autelan_forbidden(fileno_unlocked)
#define readv autelan_forbidden(readv)
#define writev autelan_forbidden(writev)
#define mmap autelan_replaced(mmap)
#define mmap64 autelan_forbidden(mmap64)
#define munmap autelan_replaced(munmap)
#define msync autelan_forbidden(msync)
#define mremap autelan_forbidden(mremap)
#define madvise autelan_forbidden(madvise)
#define sync autelan_forbidden(sync)
#define fsync autelan_forbidden(fsync)
#define fdatasync autelan_forbidden(fdatasync)
#define aio_read64 autelan_forbidden(aio_read64)
#define aio_write autelan_forbidden(aio_write)
#define aio_write64 autelan_forbidden(aio_write64)
#define aio_read autelan_forbidden(aio_read)
#define lio_listio autelan_forbidden(lio_listio)
#define lio_listio64 autelan_forbidden(lio_listio64)
#define aio_error autelan_forbidden(aio_error)
#define aio_error64 autelan_forbidden(aio_error64)
#define aio_return autelan_forbidden(aio_return)
#define aio_return64 autelan_forbidden(aio_return64)
#define aio_fsync autelan_forbidden(aio_fsync)
#define aio_fsync64 autelan_forbidden(aio_fsync64)
#define aio_suspend autelan_forbidden(aio_suspend)
#define aio_suspend64 autelan_forbidden(aio_suspend64)
#define aio_cancel autelan_forbidden(aio_cancel)
#define aio_init autelan_forbidden(aio_init)
#define fcntl autelan_forbidden(fcntl)
#define dup2  autelan_replaced(dup2)
#define select autelan_replaced(select)

/*Below are the functions that are replaced by auteos*/

#if 0
#define dup autelan_replaced(dup)
#endif


#endif
