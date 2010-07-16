#ifndef __SERVICE_BASE_HEADER__
#define __SERVICE_BASE_HEADER__

#ifdef WIN32

#include "EibStdLib.h"

#include "CException.h"
#include <windows.h>

class EIB_STD_EXPORT CServiceBase
	/// This class provides an object-oriented interface to
	/// the Windows Service Control Manager for registering,
	/// unregistering, configuring, starting and stopping
	/// services.
	///
	/// This class is only available on Windows platforms.
{
public:
	enum Startup
	{
		SVC_AUTO_START,
		SVC_MANUAL_START,
		SVC_DISABLED
	};

	CServiceBase(const CString& name);
		/// Creates the CServiceBase, using the given service name.

	~CServiceBase();
		/// Destroys the CServiceBase.

	const CString& Name() const;
		/// Returns the service name.

	CString DisplayName() const;
		/// Returns the service's display name.

	CString Path() const;
		/// Returns the path to the service executable.
		///
		/// Throws a NotFoundException if the service has not been registered.

	void RegisterService(const CString& path, const CString& displayName);
		/// Creates a Windows service with the executable specified by path
		/// and the given displayName.
		///
		/// Throws a ExistsException if the service has already been registered.

	void RegisterService(const CString& path);
		/// Creates a Windows service with the executable specified by path
		/// and the given displayName. The service name is used as display name.
		///
		/// Throws a ExistsException if the service has already been registered.

	void UnRegisterService();
		/// Deletes the Windows service.
		///
		/// Throws a NotFoundException if the service has not been registered.

	bool IsRegistered() const;
		/// Returns true if the service has been registered with the Service Control Manager.

	bool IsRunning() const;
		/// Returns true if the service is currently running.

	void Start();
		/// Starts the service.
		/// Does nothing if the service is already running.
		///
		/// Throws a NotFoundException if the service has not been registered.

	void Stop();
		/// Stops the service.
		/// Does nothing if the service is not running.
		///
		/// Throws a NotFoundException if the service has not been registered.

	void SetStartup(Startup startup);
		/// Sets the startup mode for the service.

	Startup GetStartup() const;
		/// Returns the startup mode for the service.

	static const int STARTUP_TIMEOUT;

private:
	void Open() const;
	bool TryOpen() const;
	void Close() const;
	LPQUERY_SERVICE_CONFIG Config() const;

	CServiceBase();
	CServiceBase(const CServiceBase&);
	CServiceBase& operator = (const CServiceBase&);

	CString       _name;
	SC_HANDLE         _scmHandle;
	mutable SC_HANDLE _svcHandle;
};

#endif // WIN32

#endif
