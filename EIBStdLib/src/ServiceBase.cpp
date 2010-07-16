#ifdef WIN32

#include "ServiceBase.h"

const int CServiceBase::STARTUP_TIMEOUT = 30000;


CServiceBase::CServiceBase(const CString& name):_name(name),_svcHandle(0)
{
	_scmHandle = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (!_scmHandle){
		throw CEIBException(SystemError,"cannot open Service Control Manager");
	}
}


CServiceBase::~CServiceBase()
{
	Close();
	CloseServiceHandle(_scmHandle);
}


const CString& CServiceBase::Name() const
{
	return _name;
}


CString CServiceBase::DisplayName() const
{
	LPQUERY_SERVICE_CONFIG pSvcConfig = Config();
	CString dispName(pSvcConfig->lpDisplayName);
	LocalFree(pSvcConfig);
	return dispName;
}


CString CServiceBase::Path() const
{
	LPQUERY_SERVICE_CONFIG pSvcConfig = Config();
	CString path(pSvcConfig->lpBinaryPathName);
	LocalFree(pSvcConfig);
	return path;
}


void CServiceBase::RegisterService(const CString& path, const CString& displayName)
{
	Close();
	_svcHandle = CreateService(
		_scmHandle,
		_name.GetBuffer(),
		displayName.GetBuffer(),
		SERVICE_ALL_ACCESS,
		SERVICE_WIN32_OWN_PROCESS,
		SERVICE_DEMAND_START,
		SERVICE_ERROR_NORMAL,
		path.GetBuffer(),
		NULL, NULL, NULL, NULL, NULL);
	if (!_svcHandle){
		int error = GetLastError();
		throw CEIBException(SystemError,"cannot register service");
	}
}


void CServiceBase::RegisterService(const CString& path)
{
	RegisterService(path, _name);
}


void CServiceBase::UnRegisterService()
{
	this->Open();
	if (!DeleteService(_svcHandle)){
		throw CEIBException(SystemError,"cannot unregister service");
	}
}


bool CServiceBase::IsRegistered() const
{
	return TryOpen();
}


bool CServiceBase::IsRunning() const
{
	Open();
	SERVICE_STATUS ss;
	if (!QueryServiceStatus(_svcHandle, &ss)){
		throw CEIBException(SystemError,"cannot query service status");
	}
	return ss.dwCurrentState == SERVICE_RUNNING;
}


void CServiceBase::Start()
{
	Open();
	if (!StartService(_svcHandle, 0, NULL)){
		throw CEIBException(SystemError,"cannot start service");
	}

	SERVICE_STATUS svcStatus;
	long msecs = 0;
	while (msecs < STARTUP_TIMEOUT)
	{
		if (!QueryServiceStatus(_svcHandle, &svcStatus)) break;
		if (svcStatus.dwCurrentState != SERVICE_START_PENDING) break;
		//CThread::Sleep(250);
		msecs += 250;
	}
	if (!QueryServiceStatus(_svcHandle, &svcStatus)){
		throw CEIBException(SystemError,"cannot query status of starting service");
	}
	else if (svcStatus.dwCurrentState != SERVICE_RUNNING){
		throw CEIBException(SystemError,"service failed to start within a reasonable time");
	}
 }


void CServiceBase::Stop()
{
	Open();
	SERVICE_STATUS svcStatus;
	if (!ControlService(_svcHandle, SERVICE_CONTROL_STOP, &svcStatus)){
		throw CEIBException(SystemError,"cannot stop service");
	}
}


void CServiceBase::SetStartup(CServiceBase::Startup startup)
{
	Open();
	DWORD startType;
	switch (startup)
	{
	case SVC_AUTO_START:
		startType = SERVICE_AUTO_START;
		break;
	case SVC_MANUAL_START:
		startType = SERVICE_DEMAND_START;
		break;
	case SVC_DISABLED:
		startType = SERVICE_DISABLED;
		break;
	default:
		startType = SERVICE_NO_CHANGE;
	}
	if (!ChangeServiceConfig(_svcHandle, SERVICE_NO_CHANGE, startType, SERVICE_NO_CHANGE, NULL, NULL, NULL, NULL, NULL, NULL, NULL))
	{
		throw CEIBException(SystemError,"cannot change service startup mode");
	}
}


CServiceBase::Startup CServiceBase::GetStartup() const
{
	LPQUERY_SERVICE_CONFIG pSvcConfig = Config();
	Startup result;
	switch (pSvcConfig->dwStartType)
	{
	case SERVICE_AUTO_START:
	case SERVICE_BOOT_START:
	case SERVICE_SYSTEM_START:
		result = SVC_AUTO_START;
		break;
	case SERVICE_DEMAND_START:
		result = SVC_MANUAL_START;
		break;
	case SERVICE_DISABLED:
		result = SVC_DISABLED;
		break;
	default:
		result = SVC_MANUAL_START;
	}
	LocalFree(pSvcConfig);
	return result;
}


void CServiceBase::Open() const
{
	if (!TryOpen()){
		throw CEIBException(GeneralError,"service does not exist");
	}
}


bool CServiceBase::TryOpen() const
{
	_svcHandle = OpenService(_scmHandle, _name.GetBuffer(), SERVICE_ALL_ACCESS);
	return _svcHandle != 0;
}


void CServiceBase::Close() const
{
	if (_svcHandle)
	{
		CloseServiceHandle(_svcHandle);
	}
}


LPQUERY_SERVICE_CONFIG CServiceBase::Config() const
{
	Open();
	int size = 4096;
	DWORD bytesNeeded;
	LPQUERY_SERVICE_CONFIG pSvcConfig = (LPQUERY_SERVICE_CONFIG) LocalAlloc(LPTR, size);
	if (!pSvcConfig){
		throw CEIBException(OutOfMemory,"cannot allocate service config buffer");
	}
	try
	{
		while (!QueryServiceConfig(_svcHandle, pSvcConfig, size, &bytesNeeded))
		{
			if (GetLastError() == ERROR_INSUFFICIENT_BUFFER)
			{
				LocalFree(pSvcConfig);
				size = bytesNeeded;
				pSvcConfig = (LPQUERY_SERVICE_CONFIG) LocalAlloc(LPTR, size);
			}
			else
			{
				throw CEIBException(SystemError,"cannot query service configuration");
			}
		}
	}
	catch (...)
	{
		LocalFree(pSvcConfig);
		throw;
	}
	return pSvcConfig;
}

#endif
