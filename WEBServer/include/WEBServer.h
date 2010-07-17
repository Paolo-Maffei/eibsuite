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

//some useful MACROS
#ifdef WIN32
#define LOG(level,msg,...) CWEBServer::GetInstance().GetLog().Log(level,msg,__VA_ARGS__)
#define LOG_INFO(msg,...) LOG(LOG_LEVEL_INFO,msg,##__VA_ARGS__)
#define LOG_ERROR(msg,...) LOG(LOG_LEVEL_ERROR,msg,##__VA_ARGS__)
#define LOG_DEBUG(msg,...) LOG(LOG_LEVEL_DEBUG,msg,##__VA_ARGS__)
#define LOG_SCREEN(msg,...) printf(msg,##__VA_ARGS__)
#else
#define LOG(level,msg,args...) CWEBServer::GetInstance().GetLog().Log(level,msg,##args)
#define LOG_INFO(msg,args...) LOG(LOG_LEVEL_INFO,msg,##args)
#define LOG_ERROR(msg,args...) LOG(LOG_LEVEL_ERROR,msg,##args)
#define LOG_DEBUG(msg,args...) LOG(LOG_LEVEL_DEBUG,msg,##args)
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

	void InteractiveConf();

private:
	static CWEBServer _instance;
	CWEBServerConfig _conf;
	CLogFile _log;
	CDispatcherHandle _dispatcher;
	CUsersDB _users;
	CString _domain;
	CWebCollectorHandle _collector;
};

#endif
