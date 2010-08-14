#include "SMSServer.h"
#include "cli.h"

JTCInitialize init;
CSMSServer CSMSServer::_instance;

CSMSServer::CSMSServer() :
CSingletonProcess(SMS_SERVER_PROCESS_NAME)
{
	_listener = new CSMSListener();
	_agent = new CEIBAgent();
}

CSMSServer::~CSMSServer()
{
}

bool CSMSServer::Init()
{
	
	bool res = true;
	START_TRY
		//initialize log file
		_log.Init(CURRENT_LOGS_FOLDER + CString(DEFAULT_LOG_FILE_NAME));
		LOG_INFO("Initializing Log manager...Successful.");
	END_TRY_START_CATCH(e)
		cerr << "Initializing Log manager...Failed: " << e.what() << endl;
		return false;
	END_CATCH

	START_TRY
		//load configuration from file
		_conf.Load(SMS_CONF_FILE_NAME);
		_log.SetLogLevel((LogLevel)_conf.GetLogLevel());
		LOG_INFO("Reading Configuration file...Successful.");
	END_TRY_START_CATCH(e)
		LOG_ERROR("Reading Configuration file...Failed: %s",e.what());
		res = false;
	END_CATCH

	START_TRY
		_db.Init(CURRENT_CONF_FOLDER + SMS_DB_FILE);
		_db.Load();
		LOG_INFO("Initializing SMS Messages Database...Successful");
	END_TRY_START_CATCH(e)
		LOG_ERROR("Initializing SMS Messages Database...Failed: %s",e.what());
		res = false;
	END_CATCH
	

	START_TRY
		LOG_INFO("Initializing Cellular Modem connection...");
		CString device = _conf.GetDevice();
		_meta = new MeTa(new SerialPort(device.GetSTDString(),_conf.GetDeviceBaudRate(),DEFAULT_INIT_STRING, false));
		if(_meta == NULL){
			throw CEIBException(GeneralError,"dummy");
		}
		//DeleteAllMessages();
		_listener->start();
		LOG_INFO("Starting SMS Listener... Successful.");
	END_TRY_START_CATCH_GSM(e)
		LOG_ERROR("Cellular Modem connection Failed: %s",e.what());
		res = false;
	END_CATCH
	
	if(res){
		LOG_INFO("Cellular Modem connection OK");
	}

	CTime t;
	//indicate user
	LOG_INFO("SMS Server started on %s",t.Format().GetBuffer());
	
	return res;
}

void CSMSServer::Close()
{
	LOG_INFO("Saving Configuration file...");
	_conf.Save(SMS_CONF_FILE_NAME);

	//close agent
	LOG_INFO("Closing connection to EIB Server...");
	_agent->Close();
	_agent->join();

	LOG_INFO("Closing the cellular port...");
	_listener->Close();
	_listener->join();
}

void CSMSServer::Run()
{
	if (!_agent->ConnectToEIB())
	{
		LOG_INFO("\nCannot establish connection with EIB Server!\n");
		exit(1);
	}

	LOG_INFO("\nEIB Server Connection established.\n");

	_agent->start();
}

void CSMSServer::DeleteAllMessages()
{
	if(_meta == NULL){
		return;
	}

	vector<std::string> stores = _meta->getSMSStoreNames();
	vector<std::string>::iterator it;
	for(it = stores.begin(); it != stores.end(); ++it)
	{
		SMSStoreRef st = _meta->getSMSStore(*it);
		st->erase(st->begin(),st->end());
	}
}

bool CSMSServer::SendSMS(const CString& phone_number,const CString& text)
{
	START_TRY
		if(_meta == NULL){
			LOG_ERROR("EIB Error during Send SMS: %s", "No Me/Ta Device connected.");
			return false;
		}

		//send the sms
		SMSSubmitMessage* submitSMS = new SMSSubmitMessage();
		submitSMS->setStatusReportRequest(false);
		Address destAddr(phone_number.GetBuffer());
		submitSMS->setDestinationAddress(destAddr);
		_meta->sendSMSs(submitSMS, text.GetBuffer(), true);
	END_TRY_START_CATCH(e)
		LOG_ERROR("EIB Error during Send SMS: %s", e.what());
	END_TRY_START_CATCH_GSM(ex)
		LOG_ERROR("GSM Error during Send SMS: %s", ex.what());
	END_TRY_START_CATCH_ANY
		LOG_ERROR("General Error during Send SMS.");
	END_CATCH

	return true;
}

void CSMSServer::InteractiveConf()
{
	START_TRY
		_conf.Load(SMS_CONF_FILE_NAME);
	END_TRY_START_CATCH_ANY
		_conf.Init();
	END_CATCH

	LOG_SCREEN("************************************\n");
	LOG_SCREEN("SMSServer Interactive configuration:\n");
	LOG_SCREEN("************************************\n");

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
	if(ConsoleCLI::GetCString("SMS Server user name (used to connect to EIB Server)?",sval, _conf.GetName())){
		_conf.SetName(sval);
	}
	if(ConsoleCLI::GetCString("SMS Server password (used to connect to EIB Server)?",sval, _conf.GetPassword())){
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
		if(CUtils::EnumNics(map2) && ConsoleCLI::GetStrOption("Choose Interface to connect through to EIBServer", map2, sval, CString(_conf.GetListenInterface()))){
			_conf.SetListenInterface(sval.ToInt());
		}
	#else
		if(CUtils::EnumNics(map2) && ConsoleCLI::GetStrOption("Choose Interface to connect through to EIBServer", map2, sval, _conf.GetListenInterface())){
			_conf.SetListenInterface(sval);
		}
	#endif

	if(ConsoleCLI::GetCString("SMS Network device port?",sval, _conf.GetDevice())){
		_conf.SetDevice(sval);
	}
	LOG_SCREEN("Saving configuration to %s...", SMS_CONF_FILE_NAME);
	if(!_conf.Save(SMS_CONF_FILE_NAME)){
		throw CEIBException(FileError, "Cannot save configuration to file \"%s\"", SMS_CONF_FILE_NAME);
	}
	LOG_SCREEN(" [OK]\n");
	_log.SetConsoleColor(GREEN);
	LOG_INFO("\nNow you can run SMS Server. the new file will be loaded automatically.\n\n");
}
