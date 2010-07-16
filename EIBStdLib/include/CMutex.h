/*! \file Mutex.h
    \brief Mutex Class - Header file.

	Ex 3 - Master Mind Telnet Server.
	Created By: Yosi Gabay, ID 040293953			Creation Date: March 16th. 2007

	This is The header file for the Mutex Class

*/

#ifndef __MUTEX_HEADER__
#define __MUTEX_HEADER__

#include "EibStdLib.h"
#include "CException.h"
using namespace std;

#ifdef WIN32
typedef HANDLE MutexHandle;
#else
#include <pthread.h>
typedef pthread_mutex_t MutexHandle;
#endif


/************************************************************************/
/*		Include Library files                                           */
/************************************************************************/

/*! \class CMutex
	\brief Mutex Class
*/
class EIB_STD_EXPORT CMutex
{

private:
		/*! Handle to the mutex */
	MutexHandle Handle;

public:
		/*!
		  Constructor
		*/
		CMutex();

		/*!
		  Destructor
		*/
		~CMutex();

		/*!
		\brief Function used to Lock Mutex. if mutex is already owned , the thread will sleep untill other thread realse the lock.
		\fn Lock()
		\return void
		*/
		void Lock();

		/*!
		\brief Function used to Try Lock Mutex. if mutex is already owned by other thread, function return false else true.
		\fn bool TryLock()
		\return True - if Lock succeeded. False - Else.
		*/
		bool TryLock();

		/*!
		\brief Function used to Release Mutexby the thread that own the mutex.
		\fn Lock()
		\return void
		*/
		void Release();

};

#endif
