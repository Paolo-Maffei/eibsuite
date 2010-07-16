#ifndef __CONSOLE_MANAGER_HEADER__
#define __CONSOLE_MANAGER_HEADER__

#include <iostream>
#include "JTC.h"
#include "Socket.h"
#include "HttpRequest.h"
#include "HttpParser.h"
#include "HttpSession.h"

using namespace std;

//commands
#define EIB_SERVER_GET_USERS_CONF_CMD "EIB_SERVER_GET_USERS_CONF_CMD"
#define EIB_SERVER_SET_USERS_CONF_CMD "EIB_SERVER_SET_USERS_CONF_CMD"
#define EIB_SERVER_GET_INTERFACE_CONF_CMD "EIB_SERVER_GET_INTERFACE_CONF_CMD"
#define EIB_SERVER_SET_INTERFACE_CONF_CMD "EIB_SERVER_SET_INTERFACE_CONF_CMD"
#define EIB_CONSOLE_KEEP_ALIVE_CMD "EIB_CONSOLE_KEEP_ALIVE_CMD"
#define EIB_SERVER_INTERFACE_STOP_CMD "EIB_SERVER_INTERFACE_STOP_CMD"
#define EIB_SERVER_INTERFACE_START_CMD "EIB_SERVER_INTERFACE_START_CMD"
#define EIB_BUS_MON_GET_ADDRESSES_CMD "EIB_BUS_MON_GET_ADDRESSES_CMD"
#define EIB_BUS_MON_SEND_CMD_TO_ADDR_CMD "EIB_BUS_MON_SEND_CMD_TO_ADDR_CMD"

enum CONSOLE_API_REQUEST
{
	//interface conf
	GetConfEIBInterface,
	SetConfEIBInterface,
	//users list conf
	GetConfUsersList,
	SetConfUsersList,
	//keep alive msg
	GetKeepAliveStatus,
	//stop & start EIB Interface
	GetEIBInterfaceStop,
	GetEIBInterfaceStart,
	//bus monitor
	GetAddressList,
	SendAddrCommand,
	UNDEF_REQUEST //keep it last
};

class CConsoleManager : public JTCThread, public JTCMonitor
{
public:
	CConsoleManager();
	virtual ~CConsoleManager();

	virtual void run();
	void Init(int port);
	void Close();
	void HandleRequest(TCPSocket& socket,const CHttpRequest& request,CONSOLE_API_REQUEST type,CHttpReply& reply,CDataBuffer& buffer);
	
protected:
	virtual CONSOLE_API_REQUEST ParseRequestLine(const CString& request_uri);
	
	bool RequestAuthenticated(const CHttpRequest& reuqest,CString& uname);
	bool IsAuthenticationRequest(const CHttpRequest& reuqest);
	bool IsLegalSession(const CHttpRequest& request);
	bool IsLegalDisconnectRequest(const CHttpRequest& request);
	
private:
	void SetReplyAndSend(HTTP_STATUS_CODE sc, HTTP_VERSION version, CHttpReply& reply,CDataBuffer& buf, TCPSocket* socket);
	void GenerateSessionCookie(CHttpReply& reply,const CString& session_id);

private:
	//Interface Configuration
	void GetEIBInterfaceConf(const CHttpRequest& request, CHttpReply& reply);
	void SetEIBInterfaceConf(const CHttpRequest& request, CHttpReply& reply);
	
	//Users configuration
	void GetUsersList(const CHttpRequest& request, CHttpReply& reply);
	void SetUsersList(const CHttpRequest& request, CHttpReply& reply);

	//keep alive
	void GetKeepAlive(const CHttpRequest& request, CHttpReply& reply);
	//Stop & start EIB Interface
	void StopEIBInterface(const CHttpRequest& request, CHttpReply& reply);
	void StartEIBInterface(const CHttpRequest& request, CHttpReply& reply);
	//bus monitor
	void GetEibAddressList(const CHttpRequest& request, CHttpReply& reply);
	void SendCommandToAddress(const CHttpRequest& request, CHttpReply& reply);

private:
	bool _initialized;
	bool _stop;
	TCPServerSocket* _sock;
	CSessionManager _session_mgr;
};

#endif
