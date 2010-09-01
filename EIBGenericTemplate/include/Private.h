#ifndef __PRIVATE_HEADER__
#define __PRIVATE_HEADER__

#include "GenericServer.h"

class Server;

typedef JTCHandleT<Server> ServerHandle;

class Server : public CGenericServer, public JTCThread
{
public:
	Server();
	virtual ~Server();

	virtual void run();
	int _OpenConnection();
	void _Close();
	int _SendKnxMessage(unsigned short to, char* data, int data_len);

	static ServerHandle GetInstance() { return _instance; }

private:
	static ServerHandle _instance;
	bool _stop;
	CLogFile* _log;
};

#endif