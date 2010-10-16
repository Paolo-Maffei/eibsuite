#include "Emulator-ng.h"
#include "cli.h"

JTCInitialize init;
CEIBEmulator CEIBEmulator::_instance;

CEIBEmulator::CEIBEmulator() : 
CSingletonProcess(RELAY_SERVER_PROCESS_NAME)
{
}

CEIBEmulator::~CEIBEmulator()
{
}

void CEIBEmulator::Destroy()
{
}

void CEIBEmulator::Run(void *arg)
{
	_handler.Start();
}

void CEIBEmulator::Close()
{
	LOG_INFO("Saving Configuration file...");
	_conf.Save(EMULATOR_CONF_FILE_NAME);

	//close the heart beat thread
	LOG_INFO("Closing Emulator module...");
	_handler.Close();
	
	CTime t;
	//indicate user
	LOG_INFO("EIB Emulator closed on %s",t.Format().GetBuffer());
}

bool CEIBEmulator::Init()
{
	bool res = true;

	START_TRY
		_log.SetPrinterMethod(printf);
		_log.Init(CURRENT_LOGS_FOLDER + DEFAULT_LOG_FILE_NAME);
		LOG_INFO("Initializing Log manager...Successful");
	END_TRY_START_CATCH_ANY
		LOG_ERROR("Initializing Log manager...Failed.");
		return false;
	END_CATCH
		
	START_TRY
		//load configuration from file
		_conf.Load(EMULATOR_CONF_FILE_NAME);
		_log.SetLogLevel((LogLevel)_conf.GetLogLevel());
		LOG_INFO("Reading Configuration file...Successful.");
	END_TRY_START_CATCH(e)
		LOG_ERROR("Reading Configuration file...Failed. Reason: %s",e.what());
		res = false;
	END_CATCH

	START_TRY
		_handler.Init(&_conf,&_log);
		LOG_INFO("Initializing EIB Emulator handler...Successful.");
		const CString& laddr = _handler.GetLocalCtrlAddr();
		int lport = _handler.GetLocalCtrlPort();
		LOG_INFO("EIB Emulator is listening for new KNX/IP connections on [%s:%d]",laddr.GetBuffer(), lport);
	END_TRY_START_CATCH(e)
		LOG_ERROR("Initializing EIB Emulator handler...Failed. Reason: %s",e.what());
		res = false;
	END_CATCH

	CTime t;
	CString time_str = t.Format();
	//indicate user
	LOG_INFO("EIB Emulator started on %s\n",time_str.GetBuffer());
	
	return res;
}

CEIBEmulator& CEIBEmulator::GetInstance()
{
	return _instance;
}

void CEIBEmulator::InteractiveConf()
{
	START_TRY
		_conf.Load(EMULATOR_CONF_FILE_NAME);
	END_TRY_START_CATCH_ANY
		_conf.Init();
	END_CATCH

	LOG_SCREEN("*******************************************\n");
	LOG_SCREEN("EIB Emulator Server Interactive configuration:\n");
	LOG_SCREEN("*******************************************\n");

	CString sval;
	int ival;
	
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
	LOG_SCREEN("Saving configuration to %s...", EMULATOR_CONF_FILE_NAME);
	if(!_conf.Save(EMULATOR_CONF_FILE_NAME)){
		throw CEIBException(FileError, "Cannot save configuration to file \"%s\"", EMULATOR_CONF_FILE_NAME);
	}
	LOG_SCREEN(" [OK]\n");
	_log.SetConsoleColor(GREEN);
	LOG_SCREEN("\nNow you can run EIBRelay Server. the new file will be loaded automatically.");
	_log.SetConsoleColor(WHITE);
	LOG_SCREEN("\n\n");
}
