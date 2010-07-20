#include "SMSServer.h"
#include "cli.h"

CSMSServer CSMSServer::_instance;

CSMSServer::CSMSServer() : 
CGenericServer(EIB_TYPE_SMS_SERVER),
CSingletonProcess(SMS_SERVER_PROCESS_NAME),
_stop(false)
{
}

CSMSServer::~CSMSServer()
{
	if (_cell_port != NULL){
		delete _cell_port;
	}

	if (_meta != NULL){
		delete _meta;
	}
}

bool CSMSServer::Init()
{
	
	bool res = true;
	START_TRY
		//initialize log file
		_log.Init(CURRENT_LOGS_FOLDER + CString(DEFAULT_LOG_FILE_NAME));
		_log.Log(LOG_LEVEL_INFO,"Initializing Log manager...Successful.");
	END_TRY_START_CATCH(e)
		cerr << "Initializing Log manager...Failed: " << e.what() << endl;
		return false;
	END_CATCH

	START_TRY
		//load configuration from file
		_conf.Load(SMS_CONF_FILE_NAME);
		_log.Log(LOG_LEVEL_INFO,"Reading Configuration file...Successful.");
	END_TRY_START_CATCH(e)
		_log.Log(LOG_LEVEL_ERROR,"Reading Configuration file...Failed: %s",e.what());
		res = false;
	END_CATCH

	START_TRY
		_db.Init(CURRENT_CONF_FOLDER + SMS_DB_FILE);
		_db.Load();
		_log.Log(LOG_LEVEL_INFO,"Initializing SMS Messages Database...Successful");
	END_TRY_START_CATCH(e)
		_log.Log(LOG_LEVEL_ERROR,"Initializing SMS Messages Database...Failed: %s",e.what());
		res = false;
	END_CATCH
	

	START_TRY
		_log.Log(LOG_LEVEL_INFO,"Initializing Cellular Modem connection...");
		CString device = _conf.GetDevice();
		_cell_port = new SerialPort(device.GetSTDString(),_conf.GetDeviceBaudRate(),DEFAULT_INIT_STRING, false);
		_meta = new MeTa(_cell_port);
		//DeleteAllMessages();
		_listener.start();
		_log.Log(LOG_LEVEL_INFO,"Starting SMS Listener... Successful.");
	END_TRY_START_CATCH_GSM(e)
		_log.Log(LOG_LEVEL_ERROR,"Cellular Modem connection Failed: %s",e.what());
		_cell_port = NULL;
		res = false;
	END_CATCH
	
	if(res){
		_log.Log(LOG_LEVEL_INFO,"Cellular Modem connection OK");
	}

	CTime t;
	//indicate user
	_log.Log(LOG_LEVEL_INFO,"SMS Server started on %s",t.Format().GetBuffer());
	
	return res;
}

void CSMSServer::Close()
{
	_log.Log(LOG_LEVEL_INFO,"Saving Configuration file...");
}

void CSMSServer::run()
{
	CGenericServer::Init(&_log);

	bool established = false;
	if(_conf.GetAutoDiscoverEibServer())
	{
		_log.Log(LOG_LEVEL_INFO,"Searching EIB Server on local network...");
		established = this->OpenConnection(_conf.GetNetworkName().GetBuffer(),
											_conf.GetInitialKey().GetBuffer(),
											Socket::LocalAddress(_conf.GetListenInterface()).GetBuffer(),
											_conf.GetName().GetBuffer(),
											_conf.GetPassword().GetBuffer());
	}
	else
	{
		established = this->OpenConnection(_conf.GetNetworkName().GetBuffer(),
											_conf.GetEibIPAddress(),
											_conf.GetEibPort(),
											_conf.GetInitialKey().GetBuffer(),
											Socket::LocalAddress(_conf.GetListenInterface()).GetBuffer(),
											_conf.GetName().GetBuffer(),
											_conf.GetPassword().GetBuffer());
	}

	if (!established)
	{
		cout << endl << "Cannot establish connection with EIB Server!" << endl;
		cout.flush();
		return;
	}
	else{
		cout << endl << "EIB Server Connection established." << endl;
		cout.flush();
	}

	CEibAddress func;
	unsigned char val[5];
	unsigned char val_len = 0;
	int length;
	while (!_stop)
	{
		length = ReceiveEIBNetwork(func,val,val_len);

		if (length > 0)
		{
			cout << endl;
			cout << "Received " << length << " Bytes from EIB Server." << endl;
			
			list<CUserAlertRecord> result;
			if(_cell_port != NULL && _db.FindSmsMesaages(func.ToByteArray(),val[0],result)){
				list<CUserAlertRecord>::iterator it;
				for(it = result.begin(); it != result.end(); ++it)
				{
					CSMSServer::GetInstance().GetMetaLock().Lock();
					SendSMS(it->GetPoneNumber(),it->GetTextMessage());
					CSMSServer::GetInstance().GetMetaLock().Release();
				}
			}
		}
	}
}

void CSMSServer::DeleteAllMessages()
{
	if (_cell_port == NULL){
		return;
	}
	
	MeTa* m = CSMSServer::GetInstance().GetMeTa();
	
	if(m == NULL){
		return;
	}

	vector<std::string> stores = m->getSMSStoreNames();
	vector<std::string>::iterator it;
	for(it = stores.begin(); it != stores.end(); ++it)
	{
		SMSStoreRef st = m->getSMSStore(*it);
		st->erase(st->begin(),st->end());
	}
}

bool CSMSServer::SendSMS(const CString& phone_number,const CString& text)
{
	try
	{
		if (_cell_port == NULL){
			return false;
		}
		MeTa* m = CSMSServer::GetInstance().GetMeTa();

		if(m == NULL){
			return false;
		}

		//send the sms
		SMSSubmitMessage* submitSMS = new SMSSubmitMessage();
		submitSMS->setStatusReportRequest(false);

		submitSMS->setStatusReportRequest(false);
		Address destAddr(phone_number.GetBuffer());
		submitSMS->setDestinationAddress(destAddr);
		m->sendSMSs(submitSMS, text.GetBuffer(), true);
	}
	catch (GsmException &e)
	{
		cout << e.what() << endl;
		return false;
	}

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
	if(ConsoleCLI::GetCString("RELAY Server user name (used to connect to EIB Server)?",sval, _conf.GetName())){
		_conf.SetName(sval);
	}
	if(ConsoleCLI::GetCString("RELAY Server password (used to connect to EIB Server)?",sval, _conf.GetPassword())){
		_conf.SetPassword(sval);
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
