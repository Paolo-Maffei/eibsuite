#include "Client.h"
#include "EIBServer.h"

CClient::CClient(int session_id) : 
_logged_in(false),
_client_type(0),
_session_id(session_id),
_keep_alive_thread(NULL),
_client_port(UNDEFINED_PORT),
_client_ka_port(UNDEFINED_PORT)

{
	this->setName("Client Thread");
	_keep_alive_thread = new CListenerThread();
}

CClient::~CClient()
{
}

void CClient::UnregisterClient()
{
	CEIBServer::GetInstance().GetClientsManager()->Disconnect(GetSessionID());
}

void CClient::Close()
{
	//close heartbeat thread (the heart beat thread will trigger the closing of the client thread)
	_keep_alive_thread->Close();
	_keep_alive_thread->join();
}

void CClient::HandleIncomingPktsFromBus(const CUser& user, const CString* key, CCemiFrame& msg)
{
	if(!_buffer.Read(msg) || !user.GetFilter().IsPacketAllowed(msg) || !user.IsReadPolicyAllowed()){
		return;
	}
	int len = 0;
	char buffer[52];
	InternalNetMsg* tmp_int_msg = (InternalNetMsg*)buffer;
	InternalRelayMsg* tmp_rel_msg = (InternalRelayMsg*)buffer;
	CEibAddress dst_addr = msg.GetDestAddress();
	//forward packets that been received from EIB to the current client
	switch (_client_type)
	{
	case EIB_TYPE_PM_SERVER:
	case EIB_TYPE_SMS_SERVER:
	case EIB_TYPE_AMX_SERVER:
	case EIB_TYPE_EIB_SERVER:
	case EIB_TYPE_WEB_SERVER:
		tmp_int_msg->_header._client_type = _client_type;
		//set the message type
		tmp_int_msg->_header._msg_type = EIB_MSG_TYPE_EIB_STATUS;
		//fill the data
		tmp_int_msg->_is_logical = dst_addr.IsGroupAddress();
		tmp_int_msg->_function = htons(dst_addr.ToByteArray());
		tmp_int_msg->_value_len = msg.GetValueLength();
		msg.FillBufferWithFrameData(tmp_int_msg->_value,MAX_EIB_VALUE_LEN);
		len = sizeof(InternalNetMsg);
		break;
	case EIB_TYPE_RELAY_SERVER:
		//set the message type
		tmp_rel_msg->_header._client_type = _client_type;
		tmp_rel_msg->_header._msg_type = EIB_MSG_TYPE_RELAY;
		msg.FillBuffer((unsigned char*)&(tmp_rel_msg->_cemi_l_data_msg), sizeof(tmp_rel_msg->_cemi_l_data_msg) + sizeof(tmp_rel_msg->_addil));
		len = sizeof(InternalRelayMsg);
		break;
	default:
		return;
	}
		
	CDataBuffer::Encrypt(buffer,len,key);
	_sock.SendTo(buffer,len,GetClientIP(),GetClientPort());
}

void CClient::HandleIncomingPktsFromClient(char* buffer, int max_len, const CUser& user, const CString* key, CString& s_address, CCemiFrame& msg)
{
	int len = 0, s_port = 0;
	EibNetworkHeader* header = NULL;
	START_TRY
		len = _sock.RecvFrom(buffer,max_len,s_address,s_port,100);
	END_TRY_START_CATCH_SOCKET(e)
		LOG_ERROR("Socket Exception at Client [%s] : Code %d",_client_name.GetBuffer(),e.GetErrorCode());
	END_CATCH

	if(len == 0){
		return;
	}
		
	if(s_address != GetClientIP() || s_port != GetClientPort()){
		return;
	}
	//decrypt message
	CDataBuffer::Decrypt(buffer,len,key);
	header = (EibNetworkHeader*)buffer;
	switch(header->_msg_type)
	{
	case EIB_MSG_TYPE_EIB_CMD:
		if(header->_client_type == this->_client_type && user.IsWritePolicyAllowed())
		{
			InternalNetMsg* data = (InternalNetMsg*)(buffer);// + sizeof(EibNetworkHeader));
			//build the packet
			msg.SetMessageControl(L_DATA_REQ);
			msg.SetAddilLength(0); //must be 0
			msg.SetCtrl1(0); // initialize with 0
			msg.SetCtrl2(6); //set the TTL to 6 (Default)
			msg.SetPriority(PRIORITY_NORMAL);
			msg.SetFrameFormatStandard();
			msg.SetSrcAddress(CEibAddress());
			msg.SetDestAddress(CEibAddress(data->_function,data->_is_logical != 0));
			msg.SetValue(data->_value,data->_value_len);
				
			//write the message through EIB handler
			CEIBServer::GetInstance().GetOutputHandler()->Write(msg, (BlockingMode)header->_mode, &_pkt_mon);
			//log message
			LOG_DEBUG("Received %d Bytes from client \"%s\"",len,_client_name.GetBuffer());
		}
		break;
	case EIB_MSG_TYPE_CLINET_DISCONNECT:
		if(header->_client_type == this->_client_type){
			//close the heart beat thread and leave
			this->Close();
			//disconnect (will cause the main loop the break)
			_logged_in = false;
		}
		break;
	case EIB_MSG_TYPE_RELAY:
		if(header->_client_type == this->_client_type && user.IsWritePolicyAllowed())
		{
			msg.Parse((unsigned char*)(buffer + sizeof(EibNetworkHeader)));
			//write the message through EIB handler
			CEIBServer::GetInstance().GetOutputHandler()->Write(msg,(BlockingMode)header->_mode, &_pkt_mon);
			//log message
			LOG_DEBUG("Relay raw CEMI frame from client \"%s\" to KNX bus",_client_name.GetBuffer());
		}

		break;
	default:
		break;
	}
}

void CClient::run()
{
	CUser user;
	if(!ExchangeKeys() || !Authenticate(user)){
		//connection initialization failed. terminate connection & client
		UnregisterClient();
		return;
	}

	_keep_alive_thread->SetParent(this);
	_keep_alive_thread->start();

	CString s_address;
	const CString* key = &_encryptor.GetSharedKey();
	char buffer[256];
	CCemiFrame msg;
	while (_logged_in)
	{
		START_TRY
			//handle incoming packets from EIB Bus
			HandleIncomingPktsFromBus(user, key, msg);
			//handle incoming packets from client
			HandleIncomingPktsFromClient(buffer, 256, user, key, s_address, msg);
		END_TRY_START_CATCH_ANY
			LOG_ERROR("Unknown execption in client \"%s\"",user.GetName().GetBuffer());
		END_CATCH
	}

	_sock.Close();
	UnregisterClient();
	
	LOG_DEBUG("Client Thread [%s] Exit.",GetName().GetBuffer());
}

bool CClient::InsertToBuffer(CCemiFrame& msg)
{
	return _buffer.Write(msg);
}

void CClient::Init(int source_port,int keep_alive_port,CString& source_ip)
{	
	//allocate new socket
	SetClientPort(source_port);
	SetClientKeepAlivePort(keep_alive_port);
	SetClientIP(source_ip);
}

bool CClient::operator==(const CClient& other) const
{
	return (_session_id == other._session_id) && (_session_id != 0);
}

void CClient::CreatePublicData(CHttpReply& reply)
{
	int64 g,n,interim;
	_encryptor.CreateKeys(g,n);
	_encryptor.CreateSenderInterKey(interim);
	//request line
	reply.SetVersion(HTTP_1_0);
	reply.SetStatusCode(STATUS_OK);
	//headers
	reply.AddHeader(DIFFIE_HELLAM_MODULUS,n);
	reply.AddHeader(DIFFIE_HELLAM_INTERIM,interim);
	reply.AddHeader(DIFFIE_HELLAM_GENERATOR,g);
	reply.AddHeader(DATA_PORT_HEADER,_sock.GetLocalPort());
	reply.AddHeader(NETWORK_SESSION_ID_HEADER,_session_id);
	reply.AddHeader(KEEPALIVE_PORT_HEADER,_keep_alive_thread->GetListenPort());
}

bool CClient::ExchangeKeys()
{
	CDataBuffer raw_data;
	CHttpReply reply;
	CHttpRequest request;
	CLogFile& log = CEIBServer::GetInstance().GetLog();
	log.SetConsoleColor(YELLOW);
	LOG_INFO("[Clients Manager] Exchanging Keys with new client.");

	_sock.SetLocalPort(0);

	CreatePublicData(reply);
	reply.Finalize(raw_data);
	
	//send public keys to client
	START_TRY
		raw_data.Encrypt(&CEIBServer::GetInstance().GetConfig().GetInitialKey());
		
		//send server public key
		_sock.SendTo(raw_data.GetBuffer(),raw_data.GetLength(),GetClientIP(),GetClientPort());
		log.SetConsoleColor(YELLOW);
		LOG_INFO("[Clients Manager] Send Server public key.");
		//wait for client interim key
		char buffer[1024];
		CString s_address;
		int len = 0,s_port =0;

		//waiting for client to reply
		len = _sock.RecvFrom(buffer,sizeof(buffer),s_address,s_port,5000);

		if(len == 0){
			LOG_ERROR("[Clients Manager]  Client is not responding. Terminating connection");
			//client not responding - terminate session
			return false;
		}

		if(s_address != GetClientIP() || s_port != GetClientPort()){
			LOG_ERROR("[Clients Manager]  Client origin is fake...");
			//client not responding - terminate session
			return false;
		}

		CDataBuffer::Decrypt(buffer,len,&CEIBServer::GetInstance().GetConfig().GetInitialKey());
		CHttpParser parser(request,buffer,len);
		if(!parser.IsLegalRequest() || request.GetRequestURI() != DIFFIE_HELLMAN_CLIENT_PUBLIC_DATA){
			LOG_ERROR("[Clients Manager] Illegal http reply from client. Terminating connection");
			return false;
		}

		CHttpHeader header;
		if(!request.GetHeader(CLIENT_TYPE_HEADER,header)){
			log.SetConsoleColor(YELLOW);
			LOG_ERROR("[Clients Manager] Illegal http reply from client. Terminating connection");
			return false;
		}
		_client_type = header.GetValue().ToInt();
		if(!request.GetHeader(DIFFIE_HELLAM_INTERIM,header)){
			log.SetConsoleColor(YELLOW);
			LOG_ERROR("[Clients Manager] Illegal http reply from client. Terminating connection");
			return false;
		}
		int64 r_interim = header.GetValue().ToInt64(),key;
		_encryptor.CreateSenderEncryptionKey(key,r_interim);

		log.SetConsoleColor(YELLOW);
		LOG_INFO("[Clients Manager] Recevied Client public key.");
		//send reply back
		reply.SetStatusCode(STATUS_OK);
		reply.SetVersion(HTTP_1_0);
		reply.RemoveAllHeaders();
		reply.Finalize(raw_data);
		raw_data.Encrypt(&_encryptor.GetSharedKey());

		_sock.SendTo(raw_data.GetBuffer(),raw_data.GetLength(),GetClientIP(),GetClientPort());
		log.SetConsoleColor(YELLOW);
		LOG_INFO("[Clients Manager] Keys exchanged succesfuly.");
		
		return true;
	END_TRY_START_CATCH_SOCKET(e)
		log.SetConsoleColor(YELLOW);
		LOG_ERROR("[Clients Manager] Exchange keys error : %s",e.what());
		return false;
	END_CATCH
}

bool CClient::Authenticate(CUser& user)
{
	int len,s_port;
	CHttpReply reply;
	CHttpRequest request;
	char buf[MAX_URL_LENGTH];
	CString s_address;

	CLogFile& log = CEIBServer::GetInstance().GetLog();

	log.SetConsoleColor(YELLOW);
	LOG_INFO("[Clients Manager] Authenticating...");

	len = _sock.RecvFrom(buf,sizeof(buf),s_address,s_port);

	if(len == 0 || s_address != GetClientIP() || s_port != GetClientPort()){
		//client not responding OR faked client - terminate session
		return false;
	}

	CDataBuffer::Decrypt(buf,len,&_encryptor.GetSharedKey());
	CHttpParser parser(request,buf,len);

	if(!parser.IsLegalRequest() || request.GetRequestURI() != EIB_CLIENT_AUTHENTICATE){
		return false;
	}
	CHttpHeader header;
	if(!request.GetHeader(USER_NAME_HEADER,header)){
		return false;
	}
	else
	{
		if(!CEIBServer::GetInstance().GetUsersDB().GetRecord(header.GetValue(),user)){
			LOG_ERROR("[Clients Manager] User name \"%s\" not found. login failed.",header.GetValue().GetBuffer());
			return false;
		}
	}

	_client_name = user.GetName();

	if(!request.GetHeader(PASSWORD_HEADER,header)){
		return false;
	}
	else{
		if(user.GetPassword() != header.GetValue()){
			LOG_ERROR("[Clients Manager] Password is not correct. login failed.");
			return false;
		}
	}
	

	log.SetConsoleColor(YELLOW);
	LOG_INFO("[Clients Manager] User \"%s\" Logged in successfully.",user.GetName().GetBuffer());
	_logged_in = true;

	_policy._read = user.IsReadPolicyAllowed();
	_policy._write = user.IsWritePolicyAllowed();
		
	CDataBuffer raw_data;
	reply.SetStatusCode(STATUS_OK);
	reply.SetVersion(HTTP_1_0);
	reply.RemoveAllHeaders();
	reply.Finalize(raw_data);
	raw_data.Encrypt(&_encryptor.GetSharedKey());
	_sock.SendTo(raw_data.GetBuffer(),raw_data.GetLength(),GetClientIP(),GetClientPort());

	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define HEART_BEAT_TIMEOUT 20000

CListenerThread::~CListenerThread() 
{
	//this destructor is called when the thread exits his OnRun() function
}

CListenerThread::CListenerThread() : _sock(0),_run(true)
{
	this->setName("Client Heartbeat Thread");
}

void CListenerThread::Close()
{
	CClient& client = *_parent;
	const CString* key = &client.GetSharedKey();
	//Send disconnect message
	ClientHeartBeatMsg msg;
	memset(&msg,0,sizeof(msg));
	//local network header
	msg._header._client_type = 	EIB_TYPE_EIB_SERVER;
	msg._header._msg_type = EIB_MSG_TYPE_SERVER_DISCONNECT;
	msg._session_id = client.GetSessionID();
	//encrypt the data using the shared key
	CDataBuffer::Encrypt(&msg, sizeof(msg), key);
	START_TRY
		//send the data to the network
		_sock.SendTo(&msg, sizeof(msg), client.GetClientIP(), client.GetClientKeepAlivePort());
	END_TRY_START_CATCH_ANY
		//do nothing in case of socket error
	END_CATCH
	_run = false;
}

void CListenerThread::run()
{
	ClientHeartBeatMsg hearbeat_msg;
	CClient& client = *_parent;
	CString s_address;
	const CString* key = &client.GetSharedKey();
	int session_id = client.GetSessionID(),len = 0,s_port;
	const CString& client_name = client.GetName();
	double time_out = HEART_BEAT_TIMEOUT;
	CLogFile& log = CEIBServer::GetInstance().GetLog();

	_run = true;
	
	while(_run)
	{
		time_t start = time(NULL);
		START_TRY
			len = _sock.RecvFrom(&hearbeat_msg,sizeof(hearbeat_msg),s_address,s_port,1000);
		END_TRY_START_CATCH_SOCKET(e)
			LOG_ERROR("Socket Exception at Client [%s] Heartbeat Thread: %s",client_name.GetBuffer(),e.what());
		END_CATCH
		// in case socket received message that is not the expected keep-alive,
		// substract time difference from time_out for next RecvFrom
		time_t end = time(NULL);
		
		double diff = difftime(end,start);
		time_out -= (diff * 1000);

		if(time_out <= 0 && len == 0)
		{
			LOG_ERROR("Heartbeat timeout expired, user name [%s]. Disconnecting...",client.GetName().GetBuffer());
			time_out = HEART_BEAT_TIMEOUT;
			_run = false;
			break;
		}

		if(len ==0){
			continue;
		}

		if (len > 0 && len != sizeof(hearbeat_msg))
		{
			LOG_ERROR("Received Keep alive packet with wrong size. Ignoring packet");
			continue;
		}

		if(s_port != client.GetClientKeepAlivePort()){
			LOG_ERROR("Received Keep alive packet with wrong source port. Ignoring packet");
			continue;
		}

		if(s_address != client.GetClientIP()){
			LOG_ERROR("Received Keep alive packet with wrong source ip address. Ignoring packet");
			continue;
		}

		CDataBuffer::Decrypt(&hearbeat_msg,sizeof(ClientHeartBeatMsg),key);
		
		if(hearbeat_msg._session_id != session_id){
			LOG_ERROR("Incorrect session ID received. Disconnecting...");
			_run = false;
			break;
		}
		else if(hearbeat_msg._header._client_type != client.GetClientType()){
			LOG_ERROR("Incorrect Client Type received. Disconnecting...");
			_run = false;
			break;
		}
		else if(hearbeat_msg._header._msg_type != EIB_MSG_TYPE_KEEP_ALIVE){
			LOG_ERROR("Incorrect Message Type received. Disconnecting...");
			_run = false;
			break;
		}
		else
		{
			log.SetConsoleColor(YELLOW);
			LOG_DEBUG("[%s] [Received] Heart Beat",client_name.GetBuffer());
			hearbeat_msg._header._client_type = EIB_TYPE_EIB_SERVER;
			hearbeat_msg._header._msg_type = EIB_MSG_TYPE_KEEP_ALIVE_ACK;
			hearbeat_msg._session_id = session_id;
			CDataBuffer::Encrypt(&hearbeat_msg,sizeof(hearbeat_msg),key);
			_sock.SendTo(&hearbeat_msg,sizeof(hearbeat_msg),client.GetClientIP(),client.GetClientKeepAlivePort());
			//log.SetConsoleColor(GREEN);
			//LOG_DEBUG("[EIB] [Send] Heart Beat Ack");
			//reset timeout
			time_out = HEART_BEAT_TIMEOUT;
		}
	}

	_sock.Close();

	client.SetLoggedIn(false);
	LOG_ERROR("Client HeartBeat Thread: [%s] Now stopped.",client_name.GetBuffer());
}
