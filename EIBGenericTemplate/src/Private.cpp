#include "Private.h"
#include "GenericTemplate.h"

JTCInitialize init;
ServerHandle Server::_instance = new Server;

Server::Server() :
CGenericServer(EIB_TYPE_GENERIC),
_stop(false)
{
	this->setName("Server");
	_log = new CLogFile(); 
}

Server::~Server()
{
	delete _log; 
	join();
}

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
		START_TRY
			OnReceiveKNXMessage(addr.ToByteArray(), (char*)val, val_len);
		END_TRY_START_CATCH_ANY
			continue;
		END_CATCH
	}
}

int Server::_OpenConnection()
{
	//set the log dest
	_log->Init(EMPTY_STRING);
	_log->SetLogLevel(LOG_LEVEL_DEBUG);
	_log->SetPrinterMethod(printf);
	
	Init(_log);

	const char* userName = GetEIBUserName();
	const char* userPassword = GetPassword();
	const char* encryptKey = GetEncryptKey();
	
	CString local_ip;

	START_TRY
		local_ip = Socket::LocalAddress(GetLocalNetworkIf());
	END_TRY_START_CATCH_SOCKET(e)
		_log->Log(LOG_LEVEL_ERROR, e.what());
		return ERR_NETWORK_ERROR;
	END_CATCH

	_log->Log(LOG_LEVEL_DEBUG, "Trying to connect to EIBServer...");

	
	CString serverIP = GetEIBServerAddress();
	int serverPort = GetEIBServerPort();

	DiscoverEIBServer(local_ip,
					  encryptKey,
					  serverIP,
					  serverPort);

	ConnectionResult res;
	res = OpenConnection("Generic",
						userName,
						serverPort,
						encryptKey,
						local_ip.GetBuffer(),
						userName,
						userPassword);

	
	if(res == STATUS_CONN_OK) {
		start();
	}

	return (int)res;
}

void Server::_Close()
{
	if(isAlive()){
		_stop = true;
		join();
	}
}

int Server::_SendKnxMessage(unsigned short to, char* data, int data_len)
{
	EibStack::CEibAddress addr(to, true);
	return SendEIBNetwork(addr, (unsigned char*)data, data_len, NON_BLOCKING);
}

ServerHandle _server;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int OpenConnection()
{
	return Server::GetInstance()->_OpenConnection();
}

void CloseConnection()
{
	Server::GetInstance()->_Close();
}

int SendKnxMessage(unsigned short to, char* data, int data_len)
{
	return Server::GetInstance()->_SendKnxMessage(to, data, data_len);
}
