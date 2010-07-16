#ifndef __WEB_SERVER_HEADER__
#define __WEB_SERVER_HEADER__

#include "Globals.h"
#include "WEBNetwork.h"
#include "EibNetwork.h"
#include "GenericServer.h"
#include "WEBServerConfig.h"
#include "LogFile.h"
#include "SingletonValidation.h"
#include "Dispatcher.h"
#include "UsersDB.h"
#include "WEBCollector.h"

using namespace std;

#define DEFAULT_LOG_FILE_NAME "WEBServer.log"
#define DEFAULT_USERS_DB_FILE_NAME "WEBUsers.db"

#define WEB_SERVER_PROCESS_NAME "WEBserver"
#define DEFAULT_WEB_PORT 80

#ifdef WIN32
#define LOG_SCREEN(msg,...) printf(msg,##__VA_ARGS__)
#else
#define LOG_SCREEN(msg, args...) printf(msg,##args)
#endif

#ifdef WIN32
#include <direct.h>
#include "XGetopt.h"
#else
#define _MAX_PATH 256
#endif

typedef JTCHandleT<CDispatcher> CDispatcherHandle;
typedef JTCHandleT<CWebCollector> CWebCollectorHandle;

class CWEBServer : public CGenericServer, public CSingletonProcess
{
public:
	CWEBServer();
	virtual ~CWEBServer();
	virtual void Run(void *arg);
	bool Init();
	void Close();

	/*!
		\fn static CWEBServer& GetInstance()
		\brief Returns reference to instance of CWEBServer

		Since this class is a singleton, any use of its member functions requires getting a reference to
		the single instance of CWEBServer. This is the function that allows this.
	*/
	static CWEBServer& GetInstance();
	inline const CString& GetDomain() const { return _domain;};
	inline CLogFile& GetLog() { return _log;}
	inline CWEBServerConfig& GetConfig() { return _conf;}
	inline CDispatcherHandle& GetDispatcher() { return _dispatcher;}
	inline CUsersDB& GetUsersDB() { return _users;}
	inline CWebCollectorHandle& GetCollector() { return _collector; }

	//bool GetHisotryFromEIB(CDataBuffer& buffer);
	void SetConfFile(const CString& file_name) { _conf_file = file_name; }

	void InteractiveConf();

private:
	static CWEBServer _instance;
	CWEBServerConfig _conf;
	CLogFile _log;
	CDispatcherHandle _dispatcher;
	CUsersDB _users;
	CString _domain;
	CWebCollectorHandle _collector;
	CString _conf_file;
};

#endif
