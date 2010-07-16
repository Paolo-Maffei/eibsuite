#ifndef __AMX_SERVER_HEADER__
#define __AMX_SERVER_HEADER__

#include "Globals.h"
#include "AMXNetwork.h"
#include "EibNetwork.h"
#include "GenericServer.h"
#include "AMXServerConfig.h"
#include "LogFile.h"
#include "MsgTable.h"
#include "AMXHandler.h"
#include "EIBListener.h"
#include "SingletonValidation.h"

using namespace std;

#define AMX_MSG_TABLE_FILE_NAME "AMXMessages.db"
#define DEFAULT_LOG_FILE_NAME "AMXServer.log"

#define AMX_SERVER_PROCESS_NAME "AMXserver"

#ifdef WIN32
#include <direct.h>
#else
#define _MAX_PATH 256
#endif

class CAMXServer : public CGenericServer, public CSingletonProcess, public JTCMonitor
{
public:
	CAMXServer();
	virtual ~CAMXServer();
	void Start();
	bool Init();
	void Close();

	/*!
		\fn static CAMXServer& GetInstance()
		\brief Returns reference to instance of CAMXServer

		Since this class is a singleton, any use of its member functions requires getting a reference to
		the single instance of CAMXServer. This is the function that allows this.
	*/
	static CAMXServer& GetInstance();

	inline CLogFile& GetLog() { return _log;}
	inline CAMXServerConfig& GetConfig() { return _conf;}
	inline CMsgTable& GetMsgsTable() { return _msgs_table;}
	inline CAMXListener& GetAMXListenr() { return *_amx_handler; }
	inline CEIBListener& GetEIBListenr() { return *_eib_listener; }
	
	static void Create();
	static void Destroy();

	friend class CEIBListener;

private:
	static CAMXServer _instance;
	CAMXServerConfig _conf;
	CLogFile _log;
	CAMXListener* _amx_handler;
	CEIBListener* _eib_listener;
	CMsgTable _msgs_table;
	bool _stop;
};

#endif
