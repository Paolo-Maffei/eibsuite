#include "EIBServer.h"
#include "Globals.h"
#include "cli.h"

CEIBServer* CEIBServer::_instance = NULL;

CEIBServer::CEIBServer():
CSingletonProcess(EIB_SERVER_PROCESS_NAME),
_enabled(true),
_input_handler(NULL),
_output_handler(NULL),
_clients_mgr(NULL),
_console_mgr(NULL),
_interface(NULL)
{
	_input_handler = new CEIBHandler(INPUT_HANDLER);
	_output_handler = new CEIBHandler(OUTPUT_HANDLER);
	_interface = new CEIBInterface();
	_console_mgr = new CConsoleManager();
	_clients_mgr = new CClientsMgr();
}

CEIBServer::~CEIBServer()
{
	if (_interface != NULL){
		delete _interface;
	}
}

void CEIBServer::Close()
{
	_enabled = false;
	
	if(_conf.GetLoadOK()) {
		LOG_INFO("Saving Configuration file...");
		//save configuration to file
		_conf.Save(DEFAULT_CONF_FILE_NAME);
	}
	
	LOG_INFO("Saving Users database...");
	//save users database
	_users_db.Save();

	//close console manager
	LOG_INFO("Closing Console manager...");
	_console_mgr->Close();
	_console_mgr->join();

	//close clients manager
	LOG_INFO("Closing Clients manager...");
	_clients_mgr->Close();
	_clients_mgr->join();

	LOG_INFO("Closing EIB Interface...");
	_interface->Close();
	
	LOG_INFO("Closing EIB Input Handler...");
	//close the open connection
	_input_handler->Close();
	_input_handler->join();

	LOG_INFO("Closing EIB Output Handler...");
	//close EIB handlers
	_output_handler->Close();	
	_output_handler->join();

	CTime t;
	//indicate user
	LOG_INFO("EIB Server closed on %s",t.Format().GetBuffer());
}

bool CEIBServer::IsEnabled()
{
	return _enabled;
}

CEIBServer& CEIBServer::GetInstance()
{
	return *CEIBServer::_instance;
}

void CEIBServer::Destroy()
{
	if(_instance != NULL){
		delete _instance;
		_instance = NULL;
	}
}

void CEIBServer::Create()
{
	static bool created = false;
	if(!created){
		_instance = new CEIBServer();
		created = true;
	}
}

void CEIBServer::Start()
{
	//start EIB handlres
	_input_handler->start();
	_output_handler->start();

	//start the clients manager
	_clients_mgr->start();
	//start console manager
	_console_mgr->start();

	CTime t;
	CString time_str = t.Format();

	//indicate user
	LOG_INFO("EIB Server started on %s",time_str.GetBuffer());
}

bool CEIBServer::Init()
{
	bool res = true;
	START_TRY
		//initialize log file
		_log.SetPrinterMethod(printf);
		_log.Init(CURRENT_LOGS_FOLDER + CString(DEFAULT_LOG_FILE_NAME));
		LOG_INFO("Initializing Log manager...Successful.");
	END_TRY_START_CATCH(e)
		cerr << "Initializing Log manager...Failed: " << e.what() << endl;
		return false;
	END_CATCH
	
	START_TRY
		//load configuration from file
		_conf.Load(DEFAULT_CONF_FILE_NAME);
		_log.SetLogLevel((LogLevel)_conf.GetLogLevel());
		LOG_INFO("Reading Configuration file...Successful.");
	END_TRY_START_CATCH(e)
		LOG_ERROR("Reading Configuration file... Failed: %s", e.what());
		return false;
	END_CATCH

	START_TRY
		//initialize eib interface
		_interface->Init();
		LOG_INFO("Initializing EIB Interface...Successful.");
	END_TRY_START_CATCH(e)
		LOG_ERROR("Initializing EIB Interface...Failed: %s",e.what());
		return false;
	END_TRY_START_CATCH_SOCKET(ex)
		LOG_ERROR("Initializing EIB Interface...Failed: %s",ex.what());
		return false;
	END_CATCH

	START_TRY
		//initializing users db
		CString file_name(CURRENT_CONF_FOLDER);
		file_name += DEFAULT_USERS_DB_FILE;
		_users_db.Init(file_name);
		_users_db.Load();
		LOG_INFO("Loading Users database...Successful.");
	END_TRY_START_CATCH(e)
		LOG_ERROR("Loading Users database...Failed: %s",e.what());
		return false;
	END_CATCH

	START_TRY
		_users_db.Validate();
	END_TRY_START_CATCH(e)
		_log.SetConsoleColor(YELLOW);
		LOG_INFO("Warning: %s",e.what());
	END_CATCH

	START_TRY
		//initialize clients manager module
		_clients_mgr->Init();
		LOG_INFO("Initializing Clients manager...Successful.");
		LOG_INFO("Listening for new clients on [%s:%d]", _clients_mgr->GetListeningAddress().GetBuffer(),_conf.GetListeningPort());
	END_TRY_START_CATCH(e)
		LOG_ERROR("Initializing Clients manager...Failed: %s",e.what());
		return false;
	END_TRY_START_CATCH_SOCKET(ex)
		LOG_ERROR("Initializing Clients manager...Failed: %s",ex.what());
		return false;
	END_CATCH

	START_TRY
		_console_mgr->Init(_conf.GetConsoleManagerPort());
		LOG_INFO("Initializing Console manager...Successful.");
	END_TRY_START_CATCH(e)
		LOG_ERROR("Initializing Console manager...Failed: %s",e.what());
		return false;
	END_CATCH

	return res;
}

void CEIBServer::ReloadConfiguration()
{
	START_TRY
		//load configuration from file
		_conf.Load(DEFAULT_CONF_FILE_NAME);
		_log.SetLogLevel((LogLevel)_conf.GetLogLevel());
		LOG_INFO("Reloading Configuration file...Successful.");
	END_TRY_START_CATCH(e)
		LOG_ERROR("Reloading Configuration file...Failed: %s",e.what());
	END_CATCH

	START_TRY
		//initializing users db
		CString file_name(CURRENT_CONF_FOLDER);
		file_name += DEFAULT_USERS_DB_FILE;
		_users_db.Init(file_name);
		_users_db.Clear();
		_users_db.Load();
		_users_db.Validate();
		LOG_INFO("Reloading Users database...Successful.");
	END_TRY_START_CATCH(e)
		LOG_ERROR("Reloading Users database...Failed: %s",e.what());
	END_CATCH
}

void CEIBServer::InteractiveConf()
{
	START_TRY
		_conf.Load(DEFAULT_CONF_FILE_NAME);
	END_TRY_START_CATCH_ANY
		_conf.Init();
	END_CATCH

	LOG_SCREEN("************************************\n");
	LOG_SCREEN("EIBServer Interactive configuration:\n");
	LOG_SCREEN("************************************\n");

	CString sval;
	int ival;
	bool bval;
	if(ConsoleCLI::GetCString("Initial Encryption/Decryption key?",sval, _conf.GetInitialKey())){
		_conf.SetInitialKey(sval);
	}
	if(ConsoleCLI::Getint("EIB Server listening port?",ival, _conf.GetListeningPort())){
		_conf.SetListeningPort(ival);
	}
	if(ConsoleCLI::Getint("Max concurrent clients connected to EIBServer?",ival, _conf.GetMaxConcurrentClients())){
		_conf.SetMaxConcurrentClients(ival);
	}
	if(ConsoleCLI::Getint("EIB Console listening port?",ival, _conf.GetConsoleManagerPort())){
		_conf.SetConsoleManagerPort(ival);
	}

	map<int,CString> map1;
	map1.insert(map1.end(),pair<int,CString>(LOG_LEVEL_ERROR,"ERROR"));
	map1.insert(map1.end(),pair<int,CString>(LOG_LEVEL_INFO,"INFO"));
	map1.insert(map1.end(),pair<int,CString>(LOG_LEVEL_DEBUG,"DEBUG"));
	if(ConsoleCLI::GetStrOption("Program Logging Level?", map1, ival, _conf.GetLogLevel())){
		_conf.SetLogLevel(ival);
	}

	map<CString,CString> map2;
	map2.insert(map2.end(),pair<CString,CString>("MODE_ROUTING","MODE_ROUTING"));
	map2.insert(map2.end(),pair<CString,CString>("MODE_TUNNELING","MODE_TUNNELING"));
	map2.insert(map2.end(),pair<CString,CString>("MODE_BUSMONITOR","MODE_BUSMONITOR"));
	if(ConsoleCLI::GetStrOption("EIBNetIP Device working mode?", map2, sval, _conf.GetEibDeviceMode())){
		_conf.SetEibDeviceMode(sval);
	}

	if(ConsoleCLI::Getbool("Auto detect EIBNet/IP on local network?",bval,_conf.GetAutoDetectEibDeviceAddress())){
		_conf.SetAutoDetectEibDeviceAddress(bval);
		if(!bval){
			if(ConsoleCLI::GetCString("EIBNet/IP Device IP Address?",sval,_conf.GetEibDeviceAddress())){
				_conf.SetEibDeviceAddress(sval);
			}
		}
	}

#ifdef WIN32
	if(CUtils::EnumNics(map2) && ConsoleCLI::GetStrOption("Choose Interface to listen for new clients", map2, sval, CString(_conf.GetClientsListenInterface()))){
		_conf.SetClientsListenInterface(sval.ToInt());
	}
	if(ConsoleCLI::GetStrOption("Choose Interface to connect EIBNet/IP device", map2, sval, CString(_conf.GetEibLocalInterface()))){
		_conf.SetEibLocalInterface(sval.ToInt());
	}
#else
	if(CUtils::EnumNics(map2) && ConsoleCLI::GetStrOption("Choose Interface to listen for new clients", map2, sval, _conf.GetClientsListenInterface())){
		_conf.SetClientsListenInterface(sval);
	}
	if(ConsoleCLI::GetStrOption("Choose Interface to connect EIBNet/IP device", map2, sval, _conf.GetEibLocalInterface())){
		_conf.SetEibLocalInterface(sval);
	}
#endif

	_conf.SetLoadOK(true);
	LOG_SCREEN("Saving configuration to %s...", DEFAULT_CONF_FILE_NAME);
	if(!_conf.Save(DEFAULT_CONF_FILE_NAME)){
		throw CEIBException(FileError, "Cannot save configuration to file \"%s\"",DEFAULT_CONF_FILE_NAME);
	}
	LOG_SCREEN(" [OK]\n");
	_log.SetConsoleColor(GREEN);
	LOG_SCREEN("\nNow you can run EIB Server. the new file will be loaded automatically.");
	_log.SetConsoleColor(WHITE);
	LOG_SCREEN("\n\n");
}
