#include "GenericTemplate.h"
#include "GenericServer.h"

class Server : public CGenericServer, public JTCThread
{
public:
	Server() : CGenericServer(EIB_TYPE_PM_SERVER), _stop(false) {};
	virtual ~Server() {};
	virtual void run();
	bool _stop;
};

void Server::run()
{
	while(!_stop)
	{

	}
}

typedef JTCHandleT<Server> ServerHandle;
ServerHandle _server;

int OpenConnection()
{
	static bool initialized = false;
	if(!initialized)
		_server = new Server();

	return 0;
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
	return 0;
}
