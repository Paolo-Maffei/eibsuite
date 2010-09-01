#include "GenericTemplate.h"
#include "GenericServer.h"

class Server : public CGenericServer, public JTCThread
{
public:
	Server() : CGenericServer(EIB_TYPE_GENERIC), _stop(false) { _log = new CLogFile(); }
	virtual ~Server() { delete _log; }
	virtual void run();
	bool _stop;
	CLogFile* _log;
};

void Server::run()
{
	EibStack::CEibAddress addr;
	unsigned char val[15];
	unsigned char val_len;
	int len = 0;

	while(!_stop)
	{
		len = this->ReceiveEIBNetwork(addr, val, val_len, 2000);
		if(len == 0)
			continue;
		OnReceiveKNXMessage(addr.ToByteArray(), (char*)val, val_len);
	}
}

typedef JTCHandleT<Server> ServerHandle;
ServerHandle _server;

int OpenConnection()
{
	static bool initialized = false;
	if(!initialized)
		_server = new Server();

	//set the log dest
	_server->_log->Init(EMPTY_STRING);
	_server->_log->SetLogLevel(LOG_LEVEL_DEBUG);
	_server->_log->SetPrinterMethod(printf);

	_server->Init(_server->_log);

	const char* userName = GetEIBUserName();
	const char* userPassword = GetPassword();
	const char* encryptKey = GetEncryptKey();

	CString local_ip = Socket::LocalAddress(GetLocalNetworkIf());

	_server->GetLog()->Log(LOG_LEVEL_DEBUG, "Trying to connect to EIBServer...");

	bool res  = _server->OpenConnection("Generic", encryptKey, local_ip.GetBuffer(), userName, userPassword);
	if(!res)
		return CONN_SUCCESS;

	return ERR_NO_EIB_SERVER_PRESENT;
}

void CloseConnection()
{
	if(_server->isAlive()){
		_server->_stop = true;
		_server->join();
	}
}

int SendKnxMessage(unsigned short to, char* data, int data_len)
{
	EibStack::CEibAddress addr(to, true);
	return _server->SendEIBNetwork(addr, (unsigned char*)data, data_len, NON_BLOCKING);
}
