#ifndef __PRIVATE_HEADER__
#define __PRIVATE_HEADER__

#include "GenericServer.h"

class Server : public CGenericServer, public JTCThread
{
public:
	Server();
	virtual ~Server();

	virtual void run();
	int _OpenConnection();
	void _Close();
	int _SendKnxMessage(unsigned short to, char* data, int data_len);

	static Server& GetInstance() { return _instance; }

private:
	static Server _instance;
	bool _stop;
	CLogFile* _log;
};

typedef JTCHandleT<Server> ServerHandle;

#endif