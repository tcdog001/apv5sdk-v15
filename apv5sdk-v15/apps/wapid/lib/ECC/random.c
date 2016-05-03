/*
 * CDDL HEADER START
 *
 * The contents of this file are subject to the terms of the
 * Common Development and Distribution License (the "License").
 * You may not use this file except in compliance with the License.
 *
 * You can obtain a copy of the license at usr/src/OPENSOLARIS.LICENSE
 * or http://www.opensolaris.org/os/licensing.
 * See the License for the specific language governing permissions
 * and limitations under the License.
 *
 * When distributing Covered Code, include this CDDL HEADER in each
 * file and include the License file at usr/src/OPENSOLARIS.LICENSE.
 * If applicable, add the following below this CDDL HEADER, with the
 * fields enclosed by brackets "[]" replaced with your own identifying
 * information: Portions Copyright [yyyy] [name of copyright owner]
 *
 * CDDL HEADER END
 */
/*
 * Copyright 2009 Sun Microsystems, Inc.  All rights reserved.
 * Use is subject to license terms.
 */

#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <locale.h>
#include <stdarg.h>


#define	URANDOM_DEVICE		"/dev/urandom"	/* urandom device name */
static int	urandom_fd = -1;


/*
 * Equivalent of open(2) insulated from EINTR.
 * Also sets close-on-exec.
 */
int
open_nointr(const char *path, int oflag, ...)
{
	int	fd;
	mode_t	pmode;
	va_list	alist;

	va_start(alist, oflag);
	pmode = va_arg(alist, mode_t);
	va_end(alist);

	do {
		if ((fd = open(path, oflag, pmode)) >= 0) {
			(void) fcntl(fd, F_SETFD, FD_CLOEXEC);
			break;
		}
		/* errno definitely set by failed open() */
	} while (errno == EINTR);
	return (fd);
}

/*
 * Equivalent of read(2) insulated from EINTR.
 */
ssize_t
readn_nointr(int fd, void *dbuf, size_t dlen)
{
	char	*marker = dbuf;
	size_t	left = dlen;
	ssize_t	nread = 0, err;

	for (err = 0; left > 0 && nread != -1; marker += nread, left -= nread) {
		if ((nread = read(fd, marker, left)) < 0) {
			if (errno == EINTR) {	/* keep trying */
				nread = 0;
				continue;
			}
			err = nread;		/* hard error */
			break;
		} else if (nread == 0) {
			break;
		}
	}
	return (err != 0 ? err : dlen - left);
}

/*
 * Opens the random number generator devices if not already open.
 * Always returns the opened fd of the device, or error.
 */
static int
pkcs11_open_common(int *fd, const char *dev, int oflag)
{
	if (*fd < 0) {
		if (*fd < 0)
			*fd = open_nointr(dev, oflag);
	}
	return (*fd);
}


static int
pkcs11_open_urandom(void)
{
	return (pkcs11_open_common(&urandom_fd,   URANDOM_DEVICE, O_RDONLY));
}

/*
 * Close the random number generator devices if already open.
 */
static void
pkcs11_close_common(int *fd)
{
	if (*fd < 0)
		return;
	(void) close(*fd);
	*fd = -1;
}

void
pkcs11_close_urandom(void)
{
	pkcs11_close_common(&urandom_fd);
}

/*
 * Put the requested amount of random data into a preallocated buffer.
 * Good for passphrase salts, initialization vectors.
 */
int
pkcs11_get_urandom(void *dbuf, size_t dlen)
{
	if (dbuf == NULL || dlen == 0)
		return (0);

	/* Read random data directly from /dev/urandom */
	if (pkcs11_open_urandom() < 0)
		return (-1);

	if (readn_nointr(urandom_fd, dbuf, dlen) == dlen)
		return (0);
	return (-1);
}

/*
 * Same as pkcs11_get_urandom but ensures non zero data.
 */
int
pkcs11_get_nzero_urandom(void *dbuf, size_t dlen)
{
	char	extrarand[32];
	size_t	bytesleft = 0;
	size_t	i = 0;

	/* Start with some random data */
	if (pkcs11_get_urandom(dbuf, dlen) < 0)
		return (-1);

	/* Walk through data replacing any 0 bytes with more random data */
	while (i < dlen) {
		if (((char *)dbuf)[i] != 0) {
			i++;
			continue;
		}

		if (bytesleft == 0) {
			bytesleft = sizeof (extrarand);
			if (pkcs11_get_urandom(extrarand, bytesleft) < 0)
				return (-1);
		}
		bytesleft--;

		((char *)dbuf)[i] = extrarand[bytesleft];
	}
	return (0);
}
