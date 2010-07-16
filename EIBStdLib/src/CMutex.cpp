/*! \file Mutex.cpp
    \brief Mutex Class - Source file.
    
	Ex 3 - Master Mind Telnet Server. 
	Created By: Yosi Gabay, ID 040293953			Creation Date: March 16th. 2007

	This is The header file for the Mutex Class

*/
#include "CMutex.h"

CMutex::CMutex()
{
#ifdef WIN32	
	this->Handle = CreateMutex(NULL,false,NULL);
#else
	if(pthread_mutex_init(&this->Handle,NULL) != 0){
		throw CEIBException(MutexError,"Error in initialize mutex.");
	}
#endif
}

void CMutex::Lock()
{
#ifdef WIN32
	if (WaitForSingleObject(this->Handle,INFINITE) == WAIT_OBJECT_0){
		return;
	}
#else
	if (pthread_mutex_lock(&this->Handle) != 0){
		throw CEIBException(MutexError,"Error in aquire mutex lock.");
	}
	return;
#endif
}

void CMutex::Release()
{
	bool error = false;
	
#ifdef WIN32
	if (!ReleaseMutex(this->Handle)){
		error = true;
	}
#else
	if(pthread_mutex_unlock(&this->Handle) != 0){
		error = true;	
	}
#endif
	if(error){
		throw CEIBException(MutexError,"Error in release mutex lock.");
	}
}

bool CMutex::TryLock()
{
	bool ret = true;
#ifdef WIN32	
	if (WaitForSingleObject(this->Handle,0) == WAIT_TIMEOUT){
		ret = false;
	}
#else
	if(pthread_mutex_trylock(&this->Handle) != 0){
		ret = false;
	}
#endif
	return ret;
}

CMutex::~CMutex()
{
#ifdef WIN32
	CloseHandle(this->Handle);
#else
	if(pthread_mutex_destroy(&this->Handle) != 0){
		throw CEIBException(MutexError,"Error in destroying mutex.");
	}
#endif
}


