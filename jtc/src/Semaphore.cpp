// **********************************************************************
//
// Copyright (c) 2008
// IONA Technologies, Inc.
// Waltham, MA, USA
//
// All Rights Reserved
//
// **********************************************************************

#include <JTCSemaphore.h>

#if defined(HAVE_POSIX_THREADS)
#   include <unistd.h>
#   include <sys/time.h>
#endif

#ifndef HAVE_JTC_NO_IOSTREAM
#   ifdef HAVE_STD_IOSTREAM
using namespace std;
#   endif
#endif // !HAVE_JTC_NO_IOSTREAM

// ----------------------------------------------------------------------
// JTCSemaphore constructor and destructor
// ----------------------------------------------------------------------

JTCSemaphore::JTCSemaphore(long initial_count)
{
#if defined(HAVE_POSIX_THREADS)
    JTC_SYSCALL_3(sem_init, &m_sem, 0, initial_count, == -1)
#endif
#if defined(HAVE_WIN32_THREADS)
    JTC_SYSCALL_4(m_sem = CreateSemaphore, 0, initial_count, 0x7fffffff, 0,
                  == INVALID_HANDLE_VALUE)
#endif
}

JTCSemaphore::~JTCSemaphore()
{
#if defined(HAVE_POSIX_THREADS)
    sem_destroy(&m_sem);
#endif
#if defined(HAVE_WIN32_THREADS)
    CloseHandle(m_sem);
#endif
}

// ----------------------------------------------------------------------
// JTCSemaphore public member implementation
// ----------------------------------------------------------------------
bool
JTCSemaphore::wait()
{
#if defined(HAVE_POSIX_THREADS)
    int rc;
    rc = sem_wait(&m_sem);

    if (rc != 0 && errno != ETIMEDOUT)
        JTC_THROW_EXCEPTION(errno, "Semaphore wait aborted due to error.")

    return !rc;
#endif
#if defined(HAVE_WIN32_THREADS)
    int rc;
    JTC_SYSCALL_2(rc = WaitForSingleObject, m_sem, INFINITE, == WAIT_ABANDONED);
    return rc != WAIT_TIMEOUT;
#endif
}

//
// HP-UX, AIX, and Solaris 9 do not have a timed wait semaphore so we need
// to exclude the ability to wait for a set period of time on a JTCSemaphore.
//
#if !defined(__hpux) && !defined(_AIX) && !defined(__sun)
bool
JTCSemaphore::wait(long timeout)
{
#if defined(HAVE_POSIX_THREADS)
    int rc;
    if (timeout < 0)
    {
        rc = sem_wait(&m_sem);
    }
    else
    {
        struct timeval tv;
        struct timespec abstime;
        gettimeofday(&tv, 0);
        //                       123456789 - 10^9
        const long oneBillion = 1000000000;

        abstime.tv_sec = tv.tv_sec + (timeout / 1000);
        abstime.tv_nsec = (tv.tv_usec * 1000) + ((timeout % 1000) * 1000000);
        if (abstime.tv_nsec >= oneBillion)
        {
           ++abstime.tv_sec;
           abstime.tv_nsec -= oneBillion;
        }

        rc = sem_timedwait(&m_sem, &abstime);
    }
    if (rc != 0 && errno != ETIMEDOUT)
        JTC_THROW_EXCEPTION(errno, "Semaphore wait aborted due to error.")

    //
    // Getting here implies that if rc != 0, then the errno is ETIMEDOUT.
    // This means we should return false for this method.
    //
    return !rc;
#endif
#if defined(HAVE_WIN32_THREADS)
    if (timeout < 0)
    {
        timeout = INFINITE;
    }
    int rc;
    JTC_SYSCALL_2(rc = WaitForSingleObject, m_sem, timeout, == WAIT_ABANDONED);
    return rc != WAIT_TIMEOUT;
#endif
}
#endif // __hpux, _AIX

void
JTCSemaphore::post(int count)
{
#if defined(HAVE_POSIX_THREADS)
    JTC_SYSCALL_1(sem_post, &m_sem, == -1);
#endif
#if defined(HAVE_WIN32_THREADS)
    JTC_SYSCALL_3(ReleaseSemaphore, m_sem, count, 0, == 0)
#endif
}
