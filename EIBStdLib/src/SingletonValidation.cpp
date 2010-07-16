#include "SingletonValidation.h"

#ifdef WIN32
HANDLE CSingletonProcess::_mutex = NULL;
#endif

CSingletonProcess::CSingletonProcess(const CString& process_name) :
_process_name(process_name),
_initialized(false)
{
#ifdef WIN32
    _mutex = CreateMutex(NULL, FALSE, process_name.GetBuffer()); //do early
    _last_error = GetLastError(); //save for use later...
	if(_last_error == 0){
		_initialized = true;
	}
#endif

	if(IsAnotherInstanceRunning()){
		cout << "Another Instance of \"";
		cout << _process_name.GetBuffer();
		cout << "\" is already running..." << endl;
		JTCThread::sleep(3000);
		exit(0);
	}
}

CSingletonProcess::~CSingletonProcess()
{
	ReleaseGlobalLock();
}

void CSingletonProcess::ReleaseGlobalLock()
{
#ifdef WIN32
	if (_mutex != NULL)  //Do not forget to close handles.
    {
       CloseHandle(_mutex); //Do as late as possible.
       _mutex = NULL; //Good habit to be in.
    }
#endif
}

bool CSingletonProcess::IsAnotherInstanceRunning()
{
#ifdef WIN32
	return (!_initialized && (_last_error == ERROR_ALREADY_EXISTS));
#else
	return false;//(!_initialized && (_last_error == EEXIST ));
#endif
}

