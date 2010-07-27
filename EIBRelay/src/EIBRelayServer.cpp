#include "EIBRelayServer.h"
#include "cli.h"

CEIBRelayServer CEIBRelayServer::_instance;

CEIBRelayServer::CEIBRelayServer() : 
CSingletonProcess(RELAY_SERVER_PROCESS_NAME)
{
}

CEIBRelayServer::~CEIBRelayServer()
{
}

void CEIBRelayServer::Destroy()
{
}

void CEIBRelayServer::Run(void *arg)
{
	bool connected = _handler.Connect();
	if (!connected)
	{
		LOG_ERROR("\nCannot establish connection with EIB Server!\n");
		return;
	}
	else{
		LOG_INFO("\nEIB Server Connection established.\n");
		_handler.Start();
	}
}

void CEIBRelayServer::Close()
{
	LOG_INFO("Saving Configuration file...");
	_conf.Save(RELAY_CONF_FILE_NAME);

	//close the heart beat thread
	LOG_INFO("Closing Generic Server module...");
	_handler.Close();
	
	CTime t;
	//indicate user
	LOG_INFO("EIB Relay Server closed on %s",t.Format().GetBuffer());
}

bool CEIBRelayServer::Init()
{
	bool res = true;

	START_TRY
		_log.Init(CURRENT_LOGS_FOLDER + DEFAULT_LOG_FILE_NAME);
		_log.Log(LOG_LEVEL_INFO,"Initializing Log manager...Successful");
	END_TRY_START_CATCH_ANY
		cerr << "Initializing Log manager...Failed." << endl;
		return false;
	END_CATCH
		
	START_TRY
		//load configuration from file
		_conf.Load(RELAY_CONF_FILE_NAME);
		_log.SetLogLevel((LogLevel)_conf.GetLogLevel());
		LOG_INFO("Reading Configuration file...Successful.");
	END_TRY_START_CATCH(e)
		LOG_ERROR("Reading Configuration file...Failed. Reason: %s",e.what());
		res = false;
	END_CATCH

	START_TRY
		_handler.Init(&_conf,&_log);
		LOG_INFO("Initializing EIB Relay handler...Successful.");
		const CString& laddr = _handler.GetLocalCtrlAddr();
		int lport = _handler.GetLocalCtrlPort();
		LOG_INFO("EIB Relay is listening for new KNX/IP connections on [%s:%d]",laddr.GetBuffer(), lport);
	END_TRY_START_CATCH(e)
		LOG_ERROR("Initializing EIB Relay handler...Failed. Reason: %s",e.what());
		res = false;
	END_CATCH

	CTime t;
	CString time_str = t.Format();
	//indicate user
	LOG_INFO("EIB Relay Server started on %s\n",time_str.GetBuffer());
	
	return res;
}

CEIBRelayServer& CEIBRelayServer::GetInstance()
{
	return _instance;
}

void CEIBRelayServer::InteractiveConf()
{
	START_TRY
		_conf.Load(RELAY_CONF_FILE_NAME);
	END_TRY_START_CATCH_ANY
		_conf.Init();
	END_CATCH

	LOG_SCREEN("*******************************************\n");
	LOG_SCREEN("EIB Relay Server Interactive configuration:\n");
	LOG_SCREEN("*******************************************\n");

	CString sval;
	int ival;
	bool bval;
	if(ConsoleCLI::GetCString("Initial Encryption/Decryption key?",sval, _conf.GetInitialKey())){
		_conf.SetInitialKey(sval);
	}
	if(ConsoleCLI::Getbool("Auto detect EIBServer on local network?",bval,_conf.GetAutoDiscoverEibServer())){
		_conf.SetAutoDiscoverEibServer(bval);
		if(!bval){
			if(ConsoleCLI::GetCString("EIBServer IP Address?",sval,_conf.GetEibIPAddress())){
				_conf.SetEibIPAddress(sval);
			}
			if(ConsoleCLI::Getint("EIB Server port?",ival, _conf.GetEibPort())){
				_conf.SetEibPort(ival);
			}
		}
	}
	
	if(ConsoleCLI::GetCString("RELAY Server user name (used to connect to EIB Server)?",sval, _conf.GetName())){
		_conf.SetName(sval);
	}
	if(ConsoleCLI::GetCString("RELAY Server password (used to connect to EIB Server)?",sval, _conf.GetPassword())){
		_conf.SetPassword(sval);
	}

	map<int,CString> map1;
	map1.insert(map1.end(),pair<int,CString>(LOG_LEVEL_ERROR,"ERROR"));
	map1.insert(map1.end(),pair<int,CString>(LOG_LEVEL_INFO,"INFO"));
	map1.insert(map1.end(),pair<int,CString>(LOG_LEVEL_DEBUG,"DEBUG"));
	if(ConsoleCLI::GetStrOption("Program Logging Level?", map1, ival, _conf.GetLogLevel())){
		_conf.SetLogLevel(ival);
	}

	map<CString,CString> map2;
	
#ifdef WIN32
	if(CUtils::EnumNics(map2) && ConsoleCLI::GetStrOption("Choose Interface to listen for new KNX/IP connections", map2, sval, CString(_conf.GetListenInterface()))){
		_conf.SetListenInterface(sval.ToInt());
	}
#else
	if(CUtils::EnumNics(map2) && ConsoleCLI::GetStrOption("Choose Interface to listen for new KNX/IP connections", map2, sval, _conf.GetListenInterface())){
		_conf.SetListenInterface(sval);
	}
#endif
	LOG_SCREEN("Saving configuration to %s...", RELAY_CONF_FILE_NAME);
	if(!_conf.Save(RELAY_CONF_FILE_NAME)){
		throw CEIBException(FileError, "Cannot save configuration to file \"%s\"", RELAY_CONF_FILE_NAME);
	}
	LOG_SCREEN(" [OK]\n");
	_log.SetConsoleColor(GREEN);
	LOG_INFO("\nNow you can run EIBRelay Server. the new file will be loaded automatically.\n\n");
}
