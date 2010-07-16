// **********************************************************************
//
// Copyright (c) 2002
// IONA Technologies, Inc.
// Waltham, MA, USA
//
// All Rights Reserved
//
// **********************************************************************

#ifndef JTC_MUTEX_H
#define JTC_MUTEX_H

#include <Types.h>
#include <Syscall.h>

#include <errno.h>

//
// This class can be used to establish a critical section. Call the
// lock method to lock the mutex, and the unlock Method to unlock the
// mutex. If the mutex is currently locked the thread will be
// suspended until the mutex becomes unlocked.
//
class JTC_IMPORT_EXPORT JTCMutex
{
public:

    JTCMutex()
    {
#if defined(HAVE_POSIX_THREADS)
        JTC_SYSCALL_2(pthread_mutex_init, &m_lock, 0, != 0)
#endif
#if defined(HAVE_WIN32_THREADS)
        InitializeCriticalSection(&m_lock);
#endif
    }

    ~JTCMutex()
    {
#if defined(HAVE_POSIX_THREADS)
        pthread_mutex_destroy(&m_lock);
#endif
#if defined(HAVE_WIN32_THREADS)
        DeleteCriticalSection(&m_lock);
#endif
    }

    //
    // Lock the mutex.
    //
    bool
    lock() const
    {
#if defined(HAVE_POSIX_THREADS)
        pthread_mutex_t* lock = &((JTCMutex*)this) -> m_lock;
        JTC_SYSCALL_1(pthread_mutex_lock, lock, != 0)
#endif

#if defined(HAVE_WIN32_THREADS)
	CRITICAL_SECTION* crit = &((JTCMutex*)this) -> m_lock;
        EnterCriticalSection(crit);
#endif
	//
	// Since the lock is non-recursive its always the first
	// acquisition.
	//
	return true;
    }


    //
    // Unlock the mutex.
    //
    bool
    unlock() const
    {
#if defined(HAVE_POSIX_THREADS)
        pthread_mutex_t* lock = &((JTCMutex*)this) -> m_lock;
        JTC_SYSCALL_1(pthread_mutex_unlock, lock, != 0)
#endif
#if defined(HAVE_WIN32_THREADS)
	CRITICAL_SECTION* crit = &((JTCMutex*)this) -> m_lock;
        LeaveCriticalSection(crit);
#endif
	//
	// Since the lock is non-recursive its always the last
	// release.
	//
	return true;
    }

    //
    // Trylock the mutex.
    //
    bool
    trylock() const
    {
#if defined(HAVE_POSIX_THREADS)
        pthread_mutex_t* lock = &((JTCMutex*)this) -> m_lock;
	try
	{
	    JTC_SYSCALL_1(pthread_mutex_trylock, lock, != 0)
	}
	catch(const JTCSystemCallException& e)
	{
	    //
	    // EBUSY means that the thread couldn't obtain the lock
	    //
	    if (e.getError() == EBUSY)
		return false;
	    throw;
	}
	return true;
#endif

#if defined(HAVE_WIN32_THREADS)
#    if (_WIN32_WINNT >= 0x0400)
	CRITICAL_SECTION* crit = &((JTCMutex*)this) -> m_lock;
        return TryEnterCriticalSection(crit) ? true : false;
#    else
	return false;
#    endif
#endif

    }

    JTCThreadId
    get_owner() const
    {
	return JTCThreadId::self();
    }

    //
    // Number of time the mutex has been locked. Always 1 since the
    // mutex is not recursive.
    //
    unsigned int
    count() const
    {
	return 1;
    }

private:

    //
    // Hide copy constructor and assignment operator.
    //
    JTCMutex(const JTCMutex&);
    
    void
    operator=(const JTCMutex&);

#if defined(HAVE_POSIX_THREADS)
    pthread_mutex_t m_lock; // Pthread mutex.
#endif

#if defined(HAVE_WIN32_THREADS)
    CRITICAL_SECTION m_lock; // WIN32 critical section.
#endif

    friend class JTCCond;
};

//
// Unlike the JTCMutex class this mutex can be locked recursively.
// That is it can be locked by a thread that already has the mutex
// locked.
//
class JTC_IMPORT_EXPORT JTCRecursiveMutex
{
public:

    JTCRecursiveMutex();
    ~JTCRecursiveMutex();

    //
    // Lock the mutex. True is returned if the mutex is was locked for
    // the first time, false otherwise.
    //
    bool
    lock() const;
    
    //
    // Unlock the mutex. True is returned if the mutex is available
    // for locking by some other thread. False otherwise.
    //
    bool
    unlock() const;

    //
    // Trylock the mutex.
    //
    bool
    trylock() const;

    //
    // Get the thread id of the owning thread.
    //
    JTCThreadId
    get_owner() const;

    //
    // Number of time the mutex has been locked.
    //
    unsigned int
    count() const
    {
	return m_count;
    }

private:

    //
    // Hide copy constructor and assignment operator.
    //
    JTCRecursiveMutex(const JTCRecursiveMutex&);

    void
    operator=(const JTCRecursiveMutex&);

    //
    // Internal non-const operations since mutable isn't supported
    // on some compilers.
    //
    bool
    lock_internal(int count);

    bool
    trylock_internal();

    bool
    unlock_internal();

    //
    // Lock the mutex count times.
    //
    void
    lock(unsigned int count) const;

    unsigned int
    reset_for_condvar();

#if defined(HAVE_POSIX_THREADS)
    pthread_mutex_t m_lock; // Pthreads mutex.
#endif

#if defined(HAVE_WIN32_THREADS)
    CRITICAL_SECTION m_lock; // WIN32 critical section.
#endif

    JTCMutex m_internal; // Internal mutex.
 
    unsigned int m_count; // Number of times the mutex has been aquired.
    JTCThreadId m_owner; // Current owner of the mutex.

    friend class JTCCondHelper;
    friend class JTCCond;
};

#endif
