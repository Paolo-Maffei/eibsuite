/*! \file EIBServer.h
    \brief Defines CEIBServer

	This is the header file for CEIBServer.

*/
#ifndef __EIB_SERVER_HEADER__
#define __EIB_SERVER_HEADER__

#include <stdlib.h>
#include <iostream>
#include "CException.h"
#include "EIBInterface.h"
#include "ServerConfig.h"
#include "LogFile.h"
#include "UsersDB.h"
#include "ClientsMgr.h"
#include "ConsoleManager.h"
#include "SingletonValidation.h"
#include "StatsDB.h"
#include "EIBInterface.h"
#include "Handle.h"
#include "DummyThread.h"

#ifdef WIN32
#include "XGetopt.h"
#endif

#ifndef _MAX_PATH
#define _MAX_PATH 1024
#endif

#define DEFAULT_CONF_FILE_NAME "EIB.conf"
#define DEFAULT_USERS_DB_FILE "Users.db"
#define DEFAULT_LOG_FILE_NAME "Eib.log"

#define EIB_SERVER_PROCESS_NAME "EIBserver"

typedef JTCHandleT<CConsoleManager> CConsoleManagerHandle;
typedef JTCHandleT<CClientsMgr> CClientsMgrHandle;

//some useful MACROS
#ifdef WIN32
#define LOG(level,msg,...) CEIBServer::GetInstance().GetLog().Log(level,msg,__VA_ARGS__)
#define LOG_INFO(msg,...) LOG(LOG_LEVEL_INFO,msg,##__VA_ARGS__)
#define LOG_ERROR(msg,...) LOG(LOG_LEVEL_ERROR,msg,##__VA_ARGS__)
#define LOG_DEBUG(msg,...) LOG(LOG_LEVEL_DEBUG,msg,##__VA_ARGS__)
#define LOG_SCREEN(msg,...) printf(msg,##__VA_ARGS__)
#else
#define LOG(level,msg,args...) CEIBServer::GetInstance().GetLog().Log(level,msg,##args)
#define LOG_INFO(msg,args...) LOG(LOG_LEVEL_INFO,msg,##args)
#define LOG_ERROR(msg,args...) LOG(LOG_LEVEL_ERROR,msg,##args)
#define LOG_DEBUG(msg,args...) LOG(LOG_LEVEL_DEBUG,msg,##args)
#define LOG_SCREEN(msg, args...) printf(msg,##args)
#endif

//forward class declaration
class CEIBHandler;
class CClientsMgr;
class CString;

/*! \class CEIBServer
	\brief Provides communication to/from the EIB System and to/from any clients

	This class is the focal point of the whole project. It provides a channel for all the clients that need to
	communicate with the Instabus. Also, it provides logging and user database management.
*/
class CEIBServer : public CSingletonProcess
{
private:
	/*!
		constructor
	*/
	CEIBServer();

public:
	/*!
		destructor
	*/
	virtual ~CEIBServer();
	/*!
		\fn static CEIBServer& GetInstance()
		\brief Returns reference to instance of CEIBServer

		Since this class is a singleton, any use of its member functions requires getting a reference to
		the single instance of CEIBServer. This is the function that allows this.
	*/
	static CEIBServer& GetInstance();

	static void Destroy();
	static void Create();
	
	/*!
		\fn inline CServerConfig& GetConfig()
		Returns reference to the server's configuration file
	*/
	inline CServerConfig& GetConfig() { return _conf;}
	/*!
		\fn inline CUsersDB& GetUsersDB()
		Returns reference to the server's user database
	*/
	inline CUsersDB& GetUsersDB() { return _users_db;}
	/*!
		\fn inline CLogFile& GetLog()
		Returns reference to the server's log file
	*/
	inline CLogFile& GetLog() { return _log;}
	/*!
		\fn inline CClientsMgr& GetClientsManager()
		Returns reference to the server's client manager
	*/
	inline CClientsMgrHandle& GetClientsManager() { return _clients_mgr;}
	/*!
		\brief Initializing the server
		\fn virtual void Run(void* arg)
	*/
	bool Init();
	/*!
		\fn void Close()
		\brief Closes the server
	*/
	void Close();
	/*!
		\fn void Start()
		\brief Starts the server threads
	*/
	void Start();
	/*!
		\fn inline CStatsDB& GetStatsDB()
		Returns reference to Statistics database
	*/
	inline CStatsDB& GetStatsDB() { return _stats;}
	/*!
		\fn inline CEIBInterface& GetEIBInterface()
		Returns reference to EIB Interface
	*/
	inline CEIBInterface& GetEIBInterface() { return *_interface;}

	void ReloadConfiguration();
	void InteractiveConf();

private:
	static CEIBServer* _instance;
	CClientsMgrHandle _clients_mgr;
	CConsoleManagerHandle _console_mgr;
	CEIBInterface* _interface;
	CUsersDB _users_db;
	CServerConfig _conf;
	CLogFile _log;
	CStatsDB _stats;
};
#endif
