
/**********************************************************************************
* Copyright (c) 2008-2011  Beijing Autelan Technology Co. Ltd.
* All rights reserved.
*
* filename: threads.c
* description:  implementation for Emulation of POSIX condition variables for WIN32
* 
*
* 
************************************************************************************/

#include "threads.h"

#ifdef WIN32

int emulate_pthread_cond_init(COND_TYPE *cv)
{
  cv->waiters_count_ = 0;
  cv->signal_event_ = CreateEvent(NULL, FALSE, FALSE, NULL);

  return 0;
}

int emulate_pthread_cond_destroy(COND_TYPE *cv)
{
  CloseHandle(cv->signal_event_);

  return 0;
}

int emulate_pthread_cond_signal(COND_TYPE *cv)
{
  int have_waiters;

  EnterCriticalSection(&cv->waiters_count_lock_);
  have_waiters = cv->waiters_count_ > 0;
  LeaveCriticalSection(&cv->waiters_count_lock_);

  if (have_waiters)
    return SetEvent(cv->signal_event_) == 0;

  return 0;
}

int emulate_pthread_cond_wait(COND_TYPE *cv, MUTEX_TYPE *cs)
{
  int result;

  EnterCriticalSection(&cv->waiters_count_lock_);
  cv->waiters_count_++;
  LeaveCriticalSection(&cv->waiters_count_lock_);

  LeaveCriticalSection(cs);

  result = (WaitForSingleObject(cv->signal_event_, INFINITE) == WAIT_FAILED);

  EnterCriticalSection(&cv->waiters_count_lock_);
  cv->waiters_count_--;
  LeaveCriticalSection(&cv->waiters_count_lock_);

  EnterCriticalSection(cs, INFINITE);

  return result;
}

#endif 
