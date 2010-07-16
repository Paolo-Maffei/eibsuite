// **********************************************************************
//
// Copyright (c) 2002
// IONA Technologies, Inc.
// Waltham, MA, USA
//
// All Rights Reserved
//
// **********************************************************************

#ifndef JTC_RUNNABLE_H
#define JTC_RUNNABLE_H

class JTC_IMPORT_EXPORT JTCRunnable : public virtual JTCRefCount
{
public:

    virtual void run() = 0;
};

#endif
