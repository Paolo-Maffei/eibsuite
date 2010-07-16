// **********************************************************************
//
// Copyright (c) 2002
// IONA Technologies, Inc.
// Waltham, MA, USA
//
// All Rights Reserved
//
// **********************************************************************

#include <TSS.h>

#include <TSSManager.h>

#include <errno.h>

#ifndef HAVE_JTC_NO_IOSTREAM
#   ifdef HAVE_STD_IOSTREAM
using namespace std;
#   endif
#endif // !HAVE_JTC_NO_IOSTREAM

// ----------------------------------------------------------------------
// JTCTSS public member implementation
// ----------------------------------------------------------------------

JTCThreadKey
JTCTSS::allocate()
{
    JTCThreadKey key;
#if defined(HAVE_POSIX_THREADS)
    JTC_SYSCALL_2(pthread_key_create, &key, 0, != 0)
#endif
#if defined(HAVE_WIN32_THREADS)
    JTC_SYSCALL_0(key = TlsAlloc, == 0xFFFFFFFF)
#endif
    return key;
}

JTCThreadKey
JTCTSS::allocate(void (*cleanup)(void*))
{
    JTCThreadKey key;
#if defined(HAVE_POSIX_THREADS)
    JTC_SYSCALL_2(pthread_key_create, &key, 0, != 0)
#endif
#if defined(HAVE_WIN32_THREADS)
    JTC_SYSCALL_0(key = TlsAlloc, == 0xFFFFFFFF)
#endif

    JTCTSSManager::instance() -> allocate(key, cleanup);

    return key;
}

void
JTCTSS::release(JTCThreadKey key)
{
    //
    // It's possible that release is called after the JTCTSS Manager
    // has already been destroyed -- although this is technically
    // an error, just ignore it.
    //
    JTCTSSManager* manager = JTCTSSManager::instance();
    if(manager != 0)
	 manager -> release(key);

#if defined(HAVE_POSIX_THREADS)
    JTC_SYSCALL_1(pthread_key_delete, key, != 0)
#endif
#if defined(HAVE_WIN32_THREADS)
    JTC_SYSCALL_1(TlsFree, key, == 0)
#endif
}

void*
JTCTSS::get(JTCThreadKey key)
{
    void* v;
#if defined(HAVE_POSIX_THREADS)
    //
    // FSU Threads has bad definition for pthread_getspecific.
    //
#   if defined(HAVE_FSU_THREADS)

    pthread_getspecific(key, &v);

#   else

    v = pthread_getspecific(key);

#   endif
#endif
#if defined(HAVE_WIN32_THREADS)
    v = TlsGetValue(key);
#endif
    return v;
}

void
JTCTSS::set(JTCThreadKey key, void* value)
{
#if defined(HAVE_POSIX_THREADS)
    try
    {
	JTC_SYSCALL_2(pthread_setspecific, key, value, != 0)
    }
    catch(const JTCSystemCallException& e)
    {
	if (e.getError() == EAGAIN || e.getError() == ENOMEM)
	{
	    throw JTCOutOfMemoryError(e.getMessage());
	}
	throw;
    }
#endif
#if defined(HAVE_WIN32_THREADS)
    try
    {
	JTC_SYSCALL_2(TlsSetValue, key, value, == 0)
    }
    catch(const JTCSystemCallException& e)
    {
	if (e.getError() == ERROR_NOT_ENOUGH_MEMORY ||
	    e.getError() == ERROR_OUTOFMEMORY)
	{
	    throw JTCOutOfMemoryError(e.getMessage());
	}
	throw;
    }
#endif
}
