// **********************************************************************
//
// Copyright (c) 2002
// IONA Technologies, Inc.
// Waltham, MA, USA
//
// All Rights Reserved
//
// **********************************************************************

#ifndef JTC_RW_MUTEX_H
#define JTC_RW_MUTEX_H

#include <Mutex.h>
#include <Cond.h>

class JTC_IMPORT_EXPORT JTCRWMutex
{
public:

    JTCRWMutex();
    ~JTCRWMutex();

    void
    read_lock() const;

    void
    write_lock() const;

    void
    unlock() const;

private:

    //
    // Hide copy constructor and assignment operator.
    //
    JTCRWMutex(const JTCRWMutex &);
    
    void
    operator=(const JTCRWMutex &);

    void
    read_lock_internal();

    void
    write_lock_internal();

    void
    unlock_internal();
    
    int	m_count;
    unsigned int m_waiting_writers;
    JTCMutex m_mutex;
    JTCCond m_ok_to_read;
    JTCCond m_ok_to_write;
};

#endif
