#include "ClientsMgr.h"
#include "ConfigFile.h"
#include "EIBServer.h"

CClientsMgr::CClientsMgr():
_stop(false),_auto_discovery_enabled(false)
{
}

CClientsMgr::~CClientsMgr()
{
}

int CClientsMgr::GetNumConnectedClients()
{
	JTCSynchronized sync(*this);
	int ret = 0;
	ret = _clients.size();
	return ret;
}

void CClientsMgr::Init()
{
	this->setName("Clients Manager Thread");
	CServerConfig& conf = CEIBServer::GetInstance().GetConfig();
	//Initialize the socket
	_local_address = Socket::LocalAddress(conf.GetClientsListenInterface());
	_server_sock.SetLocalAddressAndPort(_local_address,conf.GetListeningPort());
	START_TRY
		//register to multicast address for auto discovery service
		_broadcast_sock.JoinGroup(_local_address, AUTO_DISCOVERY_SERVICE_ADDRESS);
		_broadcast_sock.SetMulticastLoopBack(true);
		_broadcast_sock.SetLocalPort(AUTO_DISCOVERY_SERVICE_PORT);
		_auto_discovery_enabled = true;
	END_TRY_START_CATCH_ANY
		CEIBServer::GetInstance().GetLog().SetConsoleColor(YELLOW);
		LOG_INFO("WARNINIG: Error in Auto-Discovery service. Auto-Discovery is disabled.");
		_auto_discovery_enabled = false;
	END_CATCH
}

bool CClientsMgr::IsClientConnected(const CString& client_name, CString& client_ip, int& session_id)
{
	JTCSynchronized sync(*this);

	map<int,CClientHandle>::iterator it;
	for(it = _clients.begin(); it != _clients.end(); ++it)
	{
		if(it->second->GetName() == client_name){
			client_ip = it->second->GetClientIP();
			session_id = it->first;
			return true;
		}
	}
	
	return false;
}

void CClientsMgr::run()
{
	CServerConfig& conf = CEIBServer::GetInstance().GetConfig();
	char buffer[1024];
	CString source_address;
	int source_port,keep_alive_port;
	
	START_TRY

		while(!_stop)
		{
			
			int len = _server_sock.RecvFrom(buffer,sizeof(buffer),source_address,source_port,500);
			
			if (len == 0 || _stop){
				if(_auto_discovery_enabled){
					HandleServiceDiscovery(buffer,1024);
				}
				continue;
			}

			if(conf.GetMaxConcurrentClients() == GetNumConnectedClients()){
				//write error to client
				//write to log file
				LOG_ERROR("[Clients Manager] Max clients exceeded. Refusing new client.");
				//keep listening
				continue;
			}

			if(IsOpenConnectionMessage(buffer,len,source_address,source_port,keep_alive_port)){
				//initialize client with session id
				InitClient(source_address,source_port,keep_alive_port);
				//write here to log file
			}
			else{
				LOG_ERROR("[Clients Manager] Unknown request");
			}
		}

	END_TRY_START_CATCH_SOCKET(e)
		LOG_ERROR("[Clients manager] disptacher unknown exception: %s",e.what());
	END_CATCH
}

void CClientsMgr::HandleServiceDiscovery(char* buffer, int maxlen)
{
	CString saddr;
	int sport = 0;
	int len = _broadcast_sock.RecvFrom(buffer,maxlen,saddr,sport,0);

	if(len == 0){
		return;
	}

	CDataBuffer raw_request(buffer,len);
	raw_request.Decrypt(&CEIBServer::GetInstance().GetConfig().GetInitialKey());
	CHttpRequest request;

	CHttpParser parser(request,raw_request);
	if(!parser.IsLegalRequest() || request.GetRequestURI() != EIB_SERVER_AUTO_DISCOVERY_REQ){
		return;
	}

	CHttpHeader header;
	if(!request.GetHeader(ADDRESS_HEADER,header)){
		return;
	}
	saddr = header.GetValue();

	if(!request.GetHeader(DATA_PORT_HEADER,header)){
		return;
	}

	LOG_DEBUG("[Received] Auto-Discovery search request");

	sport =  header.GetValue().ToInt();

	//return reply with discovery details
	CHttpReply reply;
	reply.SetStatusCode(STATUS_OK);
	reply.SetContentType(CT_TEXT_HTML);
	reply.SetVersion(HTTP_1_0);
	//add the data
	reply.AddHeader(ADDRESS_HEADER,_local_address);
	CServerConfig& conf = CEIBServer::GetInstance().GetConfig();
	reply.AddHeader(DATA_PORT_HEADER,conf.GetListeningPort());

	CDataBuffer raw_reply;
	reply.Finalize(raw_reply);
	raw_reply.Encrypt(&CEIBServer::GetInstance().GetConfig().GetInitialKey());
	//send to reply over the network
	LOG_DEBUG("[Send] Auto-Discovery search response to [%s:%d]", saddr.GetBuffer(), sport);
	_broadcast_sock.SendTo(raw_reply.GetBuffer(), raw_reply.GetLength(), saddr, sport);
}

void CClientsMgr::Close()
{
	//aquire lock
	JTCSynchronized sync(*this);
	
	//close any active clients
	map<int,CClientHandle>::iterator it;
	for(it = _clients.begin(); it != _clients.end(); ++it)
	{
		it->second->Close();
	}

	_stop = true;
}

void CClientsMgr::InitClient(CString& source_address,int source_port,int keep_alive_port)
{
	JTCSynchronized sync(*this);

	CClientHandle Client = NULL;
	START_TRY
		Client = new CClient(GetSessionID());
	END_TRY_START_CATCH_ANY
		LOG_ERROR("Error during client init. insufficient memory");
		return;
	END_CATCH
	
	_clients.insert(pair<int,CClientHandle>(Client->GetSessionID(),Client));

	Client->Init(source_port,keep_alive_port,source_address);
	CEIBServer::GetInstance().GetLog().SetConsoleColor(YELLOW);
	LOG_INFO("[Clients Manager] New Client Initialized.");
	Client->start();
}

bool CClientsMgr::IsOpenConnectionMessage(char* data, int length,CString& source_address,int& source_port,int& keep_alive_port)
{
	CDataBuffer raw_request(data,length);
	raw_request.Decrypt(&CEIBServer::GetInstance().GetConfig().GetInitialKey());
	CHttpRequest request;

	CHttpParser parser(request,raw_request);
	if(!parser.IsLegalRequest() || request.GetRequestURI() != DIFFIE_HELLMAN_CLIENT_HELLO){
		return false;
	}
	
	CHttpHeader header;
	if(!request.GetHeader(ADDRESS_HEADER,header)){
		return false;
	}
	source_address = header.GetValue();

	if(!request.GetHeader(DATA_PORT_HEADER,header)){
		return false;
	}
	source_port =  header.GetValue().ToInt();
	if(!request.GetHeader(KEEPALIVE_PORT_HEADER,header)){
		return false;
	}
	keep_alive_port = header.GetValue().ToInt();
	return true;
}

void CClientsMgr::Disconnect(int session_id)
{
	JTCSynchronized sync(*this);
	
	map<int,CClientHandle>::iterator it = _clients.find(session_id);
	if(it == _clients.end()){
		//log error
		throw CEIBException(GeneralError,"Client not found");
	}
	_clients.erase(it);
}

void CClientsMgr::Brodcast(CCemiFrame& msg)
{
	JTCSynchronized sync(*this);

	if(_clients.size() == 0){
		return;
	}

	map<int,CClientHandle>::iterator it;
	
	for(it = _clients.begin(); it != _clients.end(); ++it)
	{
		CClientHandle client = it->second;
		if (client->CanRead()){
			if(msg.GetMessageCode() == L_DATA_IND || msg.GetMessageCode() == L_BUSMON_IND){
				client->InsertToBuffer(msg);
				continue;
			}
			//positive confirmations will be forwarded only to "Relay" clients
			if(client->GetClientType() == EIB_TYPE_RELAY_SERVER){
				printf("YGYG2\n");
				client->InsertToBuffer(msg);
				continue;
			}
		}
	}
}

int CClientsMgr::GetSessionID()
{
	while(true)
	{
		int session_id = rand();
		
		map<int,CClientHandle>::iterator it = _clients.find(session_id);
				
		if(it == _clients.end()){
			return session_id;
		}
	}
	return -1;
}

