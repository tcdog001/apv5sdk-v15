/*
 * simple logging functions that can be expanded into nothing
 *
 * Copyright (C) 2003-2004 Greg Kroah-Hartman <greg@kroah.com>
 * Copyright (C) 2004-2006 Kay Sievers <kay.sievers@vrfy.org>
 *
 *	This program is free software; you can redistribute it and/or modify it
 *	under the terms of the GNU General Public License as published by the
 *	Free Software Foundation version 2 of the License.
 * 
 *	This program is distributed in the hope that it will be useful, but
 *	WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *	General Public License for more details.
 * 
 *	You should have received a copy of the GNU General Public License along
 *	with this program; if not, write to the Free Software Foundation, Inc.,
 *	51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

 * Notes:
 *   LOG_EMERG           0      system is unusable 
 *   LOG_ERR             3      error conditions 
 *   LOG_INFO            6      informational 
 *   LOG_DEBUG   	 7      debug-level messages
 */

#ifndef LOGGING_H
#define LOGGING_H

#include <sys/stat.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

#include <stdarg.h>
#include <unistd.h>
#include <syslog.h>
#include <pthread.h>

#ifdef __cplusplus
extern "C" {
#endif

#define USE_LOG /* Eaddy */
#ifdef USE_LOG

extern void log_message(int priority, const char *format, ...);
extern void logging_init(const char *program_name);
extern void logging_close(void);

/*howard add on 20110407, get system uptime*/
#include <sys/sysinfo.h>
#define GET_SYS_UPTIME(x)     { \
                struct sysinfo info ; \
                sysinfo(&info); \
                *x=info.uptime;\
                }
/*howard end*/

#define err(format, arg...)							\
	do {									\
		log_message(LOG_ERR ,"%s: " format ,__FUNCTION__ ,## arg);	\
	} while (0)

#define info(format, arg...)							\
	do {									\
		log_message(LOG_INFO ,"%s: " format ,__FUNCTION__ ,## arg);	\
	} while (0)

#define dbg(format, arg...)							\
	do {									\
		log_message(LOG_DEBUG ,"%s: " format ,__FUNCTION__ ,## arg);	\
	} while (0)

#else

#define err(format, arg...)		do { } while (0)
#define info(format, arg...)		do { } while (0)
#define dbg(format, arg...)		do { } while (0)
#define logging_init(foo)		do { } while (0)
#define logging_close(foo)		do { } while (0)

#endif	/* USE_LOG */

extern int create_socket(const char *name, int type, mode_t perm, uid_t uid, gid_t gid);
extern int create_socket_addr(const char *name,struct sockaddr_un *addr ,int type, mode_t perm, uid_t uid, gid_t gid);
extern int connect_socket_server(const char *fileName, int type);
extern int writen(int fd, const void *vptr, size_t n);
struct _wnc_queue;
struct wnc_queue
{
	pthread_mutex_t mutex;
	pthread_cond_t cond;
	struct _wnc_queue *head;
	struct _wnc_queue *tail;
};

int wnc_queue_init(struct wnc_queue *q);
int wnc_queue_push(struct wnc_queue *q, void *input, void *output);
void *wnc_queue_pop(struct wnc_queue *q, void **output, void **data);
void wnc_queue_pop_done(void *data);

/*
	strlen_utf8(): return the length of utf8 string
	utf8_str: the utf8 string
	return: the length of the utf8 string
*/
int strlen_utf8(const char *utf8_str);

#ifdef __cplusplus
}
#endif
#endif /* LOGGING_H */
