#ifndef __SINGLETON_VALIDATION__
#define __SINGLETON_VALIDATION__

#include "EibStdLib.h"
#include "CString.h"
#include "CException.h"
#include "LogFile.h"
#include "JTC.h"

class EIB_STD_EXPORT CSingletonProcess
{
public:

	CSingletonProcess(const CString& process_name);
	virtual ~CSingletonProcess();
	
	const CString& GetProcessName() { return _process_name;}
	static void ReleaseGlobalLock();

protected:
	CString _process_name;
	bool _initialized;

#ifdef WIN32
	DWORD  _last_error;
	static HANDLE _mutex;
#endif

private:
	bool IsAnotherInstanceRunning();
};

#endif
