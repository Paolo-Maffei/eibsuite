#ifndef __RELAY_SERVER_HEADER__
#define __RELAY_SERVER_HEADER__

#include "Globals.h"
#include "EibNetwork.h"
#include "GenericServer.h"
#include "RelayServerConfig.h"
#include "LogFile.h"
#include "SingletonValidation.h"
#include "RelayHandler.h"

using namespace std;

#define DEFAULT_LOG_FILE_NAME "EIBRelay.log"
#define RELAY_SERVER_PROCESS_NAME "EIBRelay"

#ifdef WIN32
#define LOG(level,msg,...) CEIBRelayServer::GetInstance().GetLog().Log(level,msg,__VA_ARGS__)
#define LOG_INFO(msg,...) LOG(LOG_LEVEL_INFO,msg,##__VA_ARGS__)
#define LOG_ERROR(msg,...) LOG(LOG_LEVEL_ERROR,msg,##__VA_ARGS__)
#define LOG_DEBUG(msg,...) LOG(LOG_LEVEL_DEBUG,msg,##__VA_ARGS__)
#define LOG_SCREEN(msg,...) printf(msg,##__VA_ARGS__)
#else
#define LOG(level,msg,args...) CEIBRelayServer::GetInstance().GetLog().Log(level,msg,##args)
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

class CRelayHandler;

class CEIBRelayServer : public CSingletonProcess
{
public:
	CEIBRelayServer();
	virtual ~CEIBRelayServer();
	virtual void Run(void *arg);
	bool Init();
	void Close();

	/*!
		\fn static CWEBServer& GetInstance()
		\brief Returns reference to instance of CWEBServer

		Since this class is a singleton, any use of its member functions requires getting a reference to
		the single instance of CWEBServer. This is the function that allows this.
	*/
	static CEIBRelayServer& GetInstance();
	static void Destroy();
	inline CLogFile& GetLog() { return _log;}
	inline CRelayServerConfig& GetConfig() { return _conf;}
	
	void InteractiveConf();


private:
	static CEIBRelayServer _instance;
	CRelayServerConfig _conf;
	CLogFile _log;
	CRelayHandler _handler;
};

#endif
