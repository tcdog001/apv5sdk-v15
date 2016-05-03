/*************************************************************************
Copyright (c) 2013-2015, Autelan Networks. All rights reserved.

This Software is the property of Autelan Networks. 
*************************************************************************/
#include "io_on_streams_glibc.h"
#include "low_Level_io_glibc.h"

int autelan_open (const char *pathname, int oflag, ...)
{
	va_list args;
	int ret;

	va_start(args, oflag);
	ret = open(pathname, oflag, args);
	va_end(args);

	return ret;
}

ssize_t autelan_read (int filedes, void *buf, size_t nbytes)
{
	return read(filedes, buf, nbytes);
}

ssize_t autelan_write (int filedes, const void *buf, size_t nbytes)
{
	return write(filedes, buf, nbytes);
}

off_t autelan_lseek (int filedes, off_t offset, int whence)
{
	return lseek(filedes, offset, whence);
}

int autelan_close (int __fd)
{
	return close(__fd);
}

int autelan_dup (int filedes)
{
	return dup(filedes);
}

int autelan_dup2 (int filedes, int filedes2)
{
	return dup2(filedes, filedes2);
}

void *autelan_mmap (void *addr, size_t len, int prot, int flag, int filedes, off_t off)
{
	return mmap(addr, len, prot, flag, filedes, off);
}

int autelan_munmap (caddr_t addr, size_t len)
{
	return munmap(addr, len);
}

int autelan_select (int maxfdp1, fd_set *readfds, fd_set *writefds, \
		fd_set *exceptfds, struct timeval *tvptr)
{
	return select(maxfdp1, readfds, writefds, exceptfds, tvptr);
}

