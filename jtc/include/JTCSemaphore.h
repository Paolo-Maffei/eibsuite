// **********************************************************************
//
// Copyright (c) 2008
// IONA Technologies, Inc.
// Waltham, MA, USA
//
// All Rights Reserved
//
// **********************************************************************

#ifndef JTC_SEMAPHORE_H
#define JTC_SEMAPHORE_H

#include <Types.h>
#include <Syscall.h>

#if defined(HAVE_POSIX_THREADS)
#    include <semaphore.h>
#endif
#include <errno.h>

class JTC_IMPORT_EXPORT JTCSemaphore
{
public:

    JTCSemaphore(long initial_count = 0);
    ~JTCSemaphore();

    //
    // Wait for the semaphore to be signaled.  Waits for an infinite
    // period of time
    //
    bool
    wait();

    //
    // Can not use a timed wait on a semaphore in HP-UX, AIX, or Solaris
    //
#if !defined(__hpux) && !defined(_AIX) && !defined(__sun)
    //
    // Wait for the semaphore to be signaled for timeout msec. A
    // negative number means wait indefinitely. Return true if wait
    // terminated successfully (that is no timeout).
    //
    bool
    wait(long timeout);
#endif

    //
    // Increment the semaphore count times.
    //
    void
    post(int count = 1);

private:

#if defined(HAVE_POSIX_THREADS)
	sem_t m_sem; // The semaphore object
#endif
#if defined(HAVE_WIN32_THREADS)
    HANDLE m_sem; // The semaphore handle
#endif
};

#endif // JTC_SEMAPHORE_H
