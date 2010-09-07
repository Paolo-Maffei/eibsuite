#include "AMXServer.h"

JTCInitialize init;
CAMXServer CAMXServer::_instance;

CAMXServer::CAMXServer() : 
CGenericServer(EIB_TYPE_AMX_SERVER),
CSingletonProcess(AMX_SERVER_PROCESS_NAME),
_amx_handler(NULL),
_stop(false)
{
	_amx_handler = new CAMXListener();
	_eib_listener = new CEIBListener();
}

CAMXServer::~CAMXServer()
{
}

void CAMXServer::Start()
{
	CGenericServer::Init(&_log);
	
	bool established = this->OpenConnection(_conf.GetNetworkName(),
										    _conf.GetEibIPAddress(),
											_conf.GetEibPort(),
											_conf.GetInitialKey(),
											_conf.GetLocalIPAddress(),
											_conf.GetName(),
											_conf.GetPassword());

	if (!established)
	{
		LOG_ERROR("\nCannot establish connection with EIB Server!\n");
		exit(1);
	}
	else{
		LOG_INFO("\nEIB Server Connection established.\n");
	}

	LOG_INFO("Starting AMX/NET Listener...");
	_amx_handler->start();
	LOG_INFO("Starting EIB/NET Listener...");
	_eib_listener->start();
}

void CAMXServer::Close()
{
	LOG_INFO("Saving Configuration file...");
	_conf.Save(CURRENT_CONF_FOLDER + AMX_CONF_FILE_NAME);
	
	LOG_INFO("Closing EIB Listener...");
	_eib_listener->Close();

	LOG_INFO("Closing AMX Interface...");
	_amx_handler->Close();

	LOG_INFO("Closing Generic Server module...");
	CGenericServer::Close();
}

bool CAMXServer::Init()
{
	bool res = true;
	START_TRY
		//initialize log file
		_log.SetPrinterMethod(printf);
		_log.Init(CURRENT_LOGS_FOLDER + DEFAULT_LOG_FILE_NAME);
		LOG_INFO("Initializing Log manager...Successful");
	END_TRY_START_CATCH(e)
		cerr << "Initializing Log manager...Failed: " << e.what() << endl;
		return false;
	END_CATCH

	START_TRY
		//load configuration from file
		_conf.Load(AMX_CONF_FILE_NAME);
		CAMXServer::GetInstance().GetLog().Log(LOG_LEVEL_INFO,"Reading Configuration file...Successful.");
	END_TRY_START_CATCH(e)
		LOG_ERROR("Reading Configuration file...Failed: %s",e.what());
		res = false;
	END_CATCH
	
	////Messages Database
	
	START_TRY
		CMsgsDB db;
		db.Init(CURRENT_CONF_FOLDER + AMX_MSG_TABLE_FILE_NAME);
		//this fills the AMX server itself using the singleton instance
		db.Load();
		LOG_INFO("Initializing Messages DB...Successful.");
	END_TRY_START_CATCH(e)
		LOG_ERROR("Initializing Messages DB...Failed. Reason: %s",e.what());
		res = false;
	END_CATCH
	
	START_TRY
		_amx_handler->Init();
		LOG_INFO("Initializing AMX/NET Gateway...Successful");
	END_TRY_START_CATCH(e)
		LOG_ERROR("Initializing AMX/NET Gateway...Failed. Reason: %s",e.what());
		res = false;
	END_CATCH

	CTime t;
	//indicate user
	LOG_INFO("AMX Server started on %s",t.Format().GetBuffer());
	
	return res;
}

CAMXServer& CAMXServer::GetInstance()
{
	return _instance;
}
