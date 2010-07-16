#ifndef __DUMMY_THREAD_HEADER__
#define __DUMMY_THREAD_HEADER__

#include "JTC.h"

class EIB_STD_EXPORT CDummyThread :  public JTCThread
{
public:
	CDummyThread() {};
	virtual ~CDummyThread() {};

protected:

	virtual void run(){};

};

typedef EIB_STD_EXPORT JTCHandleT<CDummyThread> CDummyThreadHandle;

#endif
