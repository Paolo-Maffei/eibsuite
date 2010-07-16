#ifndef __DISPATCHER_HEADER__
#define __DISPATCHER_HEADER__

#include "JTC.h"
#include "Socket.h"
#include "WEBServerConfig.h"
#include "WebHandler.h"

#define NUM_HANDLERS 2

class CDispatcher : public JTCThread, public JTCMonitor
{
public:
	CDispatcher();
	virtual ~CDispatcher();

	void Init();
	void Close();
	virtual void run();
	inline const CString& GetServerAddress() const{ return _server_address;};
	inline int GetServerPort() const{ return _server_sock->GetLocalPort();};

private:
	TCPServerSocket* _server_sock;
	CWebHandler* _handlers[NUM_HANDLERS];
	int _num_handlers;
	CString _server_address;
	bool _stop;
};

#endif

