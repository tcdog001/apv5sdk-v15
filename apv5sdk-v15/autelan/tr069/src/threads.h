
/**********************************************************************************
* Copyright (c) 2008-2011  Beijing Autelan Technology Co. Ltd.
* All rights reserved.
*
* filename: threads.h
* description:  the header file for the declaration of function
* 
*
* 
************************************************************************************/
#ifndef THREADS_H
#define THREADS_H

#include <unistd.h>	/* defines _POSIX_THREADS if pthreads are available */
#ifdef _POSIX_THREADS
# include <pthread.h>
#endif

/******************************************************************************\
 *
 *	Threads
 *
\******************************************************************************/

#if defined(WIN32)
# define THREAD_TYPE		HANDLE
# define THREAD_ID		GetCurrentThreadId()
# define THREAD_CREATE(x,y,z)	*(x) = _beginthread((y), NULL, 8*4096, (z))
# define THREAD_DETACH(x)	
# define THREAD_JOIN(x)		WaitForSingleObject((x), INFINITE)
# define THREAD_EXIT		_endthread()
# define MUTEX_TYPE		HANDLE
# define MUTEX_SETUP(x)		(x) = CreateMutex(NULL, FALSE, NULL)
# define MUTEX_CLEANUP(x)	CloseHandle(x)
# define MUTEX_LOCK(x)		WaitForSingleObject((x), INFINITE)
# define MUTEX_UNLOCK(x)	ReleaseMutex(x)
# define COND_SETUP(x)		emulate_pthread_cond_init(&(x))
# define COND_CLEANUP(x)	emulate_pthread_cond_destroy(&(x))
# define COND_SIGNAL(x)		emulate_pthread_signal(&(x))
# define COND_WAIT(x,y)		emulate_pthread_wait(&(x), &(y))
typedef struct
{ u_int waiters_count_;
  CRITICAL_SECTION waiters_count_lock_;
  HANDLE signal_event_;
} COND_TYPE;
int emulate_pthread_cond_init(COND_TYPE*);
int emulate_pthread_cond_destroy(COND_TYPE*);
int emulate_pthread_signal(COND_TYPE*);
int emulate_pthread_wait(COND_TYPE*, MUTEX_TYPE*);
#elif defined(_POSIX_THREADS)
# define THREAD_TYPE		pthread_t
# define THREAD_ID		pthread_self()
# define THREAD_CREATE(x,y,z)	pthread_create((x), NULL, (y), (z))
# define THREAD_DETACH(x)	pthread_detach((x))
# define THREAD_JOIN(x)		pthread_join((x), NULL)
# define THREAD_EXIT		pthread_exit(NULL)
# define MUTEX_TYPE		pthread_mutex_t
# define MUTEX_SETUP(x)		pthread_mutex_init(&(x), NULL)
# define MUTEX_CLEANUP(x)	pthread_mutex_destroy(&(x))
# define MUTEX_LOCK(x)		pthread_mutex_lock(&(x))
# define MUTEX_UNLOCK(x)	pthread_mutex_unlock(&(x))
# define COND_TYPE		pthread_cond_t
# define COND_SETUP(x)		pthread_cond_init(&(x), NULL)
# define COND_CLEANUP(x)	pthread_cond_destroy(&(x))
# define COND_SIGNAL(x)		pthread_cond_signal(&(x))
# define COND_WAIT(x,y)		pthread_cond_wait(&(x), &(y))
#else
# error "No POSIX threads detected: we need thread and mutex operations. See for example OpenSSL /threads/th-lock.c on how to implement mutex on your platform"
#endif

#endif
