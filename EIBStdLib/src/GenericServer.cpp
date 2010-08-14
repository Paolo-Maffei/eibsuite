#include "GenericServer.h"


using namespace EibStack;

CGenericServer::CGenericServer(char network_id) : 
_network_id(network_id),
_status(STATUS_DISCONNECTED),
_thread(NULL),
_log(NULL)
{
	_thread = new CHeartBeatThread();
}

CGenericServer::~CGenericServer()
{
}

void CGenericServer::Init(CLogFile* log)
{
	_log = log;
}

void CGenericServer::SetConnectionParams(const CString& network_name,
										 const CString& eib_server_adress,
										 int eib_server_port,
										 const CString& initial_key,
										 const CString& local_ip,
										 const CString& user_name,
										 const CString& password)
{
}

bool CGenericServer::IsConnected()
{
	return (_status == STATUS_CONNECTED);
}

int CGenericServer::SendEIBNetwork(const CCemiFrame& frame, BlockingMode mode)
{
	if (!IsConnected()){
		//write error to log file
		return 0;
	}
	
	InternalRelayMsg msg;
	//header
	msg._header._client_type = _network_id;
	msg._header._msg_type = EIB_MSG_TYPE_RELAY;
	msg._header._mode = (char)mode;
	//data
	int max_len = sizeof(msg._cemi_l_data_msg);
	max_len += sizeof(msg._addil);
	frame.FillBuffer((unsigned char*)&msg._cemi_l_data_msg, max_len);
	//encrypt the data using the shared key
	CDataBuffer::Encrypt(&msg,sizeof(msg),&_encryptor.GetSharedKey());
	START_TRY
		//send the data to the network
		_data_sock.SendTo(&msg,sizeof(msg),_eib_address,_eib_port);
	END_TRY_START_CATCH_ANY
		//log error
		return 0;
	END_CATCH

	return sizeof(InternalRelayMsg);
}

int CGenericServer::SendEIBNetwork(const CEibAddress& address, unsigned char* value, int value_len, BlockingMode mode)
{
	if (!IsConnected()){
		//write error to log file
		return 0;
	}

	ASSERT_ERROR(value_len <= MAX_EIB_VALUE_LEN,"Max Length Cannot be sent.");

	EIBInternalNetMsg msg;
	//local network header
	msg._header._client_type = 	_network_id;
	msg._header._msg_type = EIB_MSG_TYPE_EIB_CMD;
	msg._header._mode = (char)mode;
	//msg data
	msg._is_logical = address.IsGroupAddress();
	msg._function = address.ToByteArray();
	msg._value_len = value_len;
	memcpy(&msg._value,value,value_len);
	
	//encrypt the data using the shared key
	CDataBuffer::Encrypt(&msg,sizeof(msg),&_encryptor.GetSharedKey());
	START_TRY
		//send the data to the network
		_data_sock.SendTo(&msg,sizeof(msg),_eib_address,_eib_port);
	END_TRY_START_CATCH_ANY
		//log error
		return 0;
	END_CATCH

	return sizeof(InternalNetMsg);
}

int CGenericServer::ReceiveEIBNetwork(CCemiFrame& frame, int timeout)
{
	if (!IsConnected()){
		return 0;
	}

	int s_port;
	CString s_address;
	char buffer[100];

	START_TRY
		int len = _data_sock.RecvFrom(buffer,sizeof(buffer),s_address,s_port,timeout);

		if(len == 0){
			return 0;
		}
			
		if(s_port != _eib_port || s_address != _eib_address){
			//faked message
			return 0;
		}
		CDataBuffer::Decrypt(buffer,len,&_encryptor.GetSharedKey());
		EIBInternalRelayMsg* msg = (EIBInternalRelayMsg*)buffer;
		if (msg->_header._client_type == _network_id && msg->_header._msg_type == EIB_MSG_TYPE_RELAY )
		{
			unsigned char* temp = (unsigned char*)&(msg->_cemi_l_data_msg);
			frame.Parse(temp);
			return len;
		}
	END_TRY_START_CATCH_SOCKET(e)
		GetLog()->SetConsoleColor(RED);
		GetLog()->Log(LOG_LEVEL_ERROR,e.what());
		return 0;
	END_CATCH

	return 0;
}

int CGenericServer::ReceiveEIBNetwork(CEibAddress& function, unsigned char* value, unsigned char& value_len, int timeout)
{
	if (!IsConnected()){
		return 0;
	}

	int s_port;
	CString s_address;
	char buffer[100];

	START_TRY
		int len = _data_sock.RecvFrom(buffer,sizeof(buffer),s_address,s_port,timeout);

		if(len == 0){
			return 0;
		}

		if(s_port != _eib_port || s_address != _eib_address){
			//faked message
			return 0;
		}
		CDataBuffer::Decrypt(buffer,len,&_encryptor.GetSharedKey());
		EIBInternalNetMsg* msg = (EIBInternalNetMsg*)buffer;
		if (msg->_header._client_type == _network_id && msg->_header._msg_type == EIB_MSG_TYPE_EIB_STATUS )
		{
			bool is_logical = msg->_is_logical == 0 ? false : true;
			function.Set((unsigned char*)&msg->_function,is_logical);
			value_len = msg->_value_len;
			memcpy(value,msg->_value,value_len);
			return len;
		}
	END_TRY_START_CATCH_SOCKET(e)
		GetLog()->SetConsoleColor(RED);
		GetLog()->Log(LOG_LEVEL_ERROR,e.what());
		return 0;
	END_CATCH

	return 0;
}

bool CGenericServer::Authenticate(const CString& user_name,const CString& password,const CString* key)
{
	char buff[MAX_URL_LENGTH];
	CDataBuffer raw_data;
	int len,s_port;
	CString s_address;

	//create request
	CHttpRequest request;
	request.SetMethod(GET_M);
	request.SetRequestURI(EIB_CLIENT_AUTHENTICATE);
	request.SetVersion(HTTP_1_0);
	request.AddHeader(USER_NAME_HEADER,user_name);
	request.AddHeader(PASSWORD_HEADER,password);
	request.Finalize(raw_data);
	raw_data.Encrypt(key);

	//send the request
	_data_sock.SendTo(raw_data.GetBuffer(),raw_data.GetLength(),_eib_address,_eib_port);
	GetLog()->SetConsoleColor(YELLOW);
	GetLog()->Log(LOG_LEVEL_INFO,"[%s] [Send] Client Authentication",GetUserName().GetBuffer());
	//wait for reply
	len = _data_sock.RecvFrom(buff,MAX_URL_LENGTH,s_address,s_port,2000);

	CDataBuffer raw_reply(buff,len);
	raw_reply.Decrypt(key);
	CHttpReply reply;
	CHttpParser parser(reply,raw_reply);

	if(!parser.IsLegalRequest() || reply.GetStatusCode() != STATUS_OK){
		return false;
	}
	GetLog()->SetConsoleColor(YELLOW);
	GetLog()->Log(LOG_LEVEL_INFO,"[EIB] [Received] Client Authentication OK");
	return true;
}

void CGenericServer::Close()
{
	if (IsConnected()){
		//Send disconnect msg
		EIBInternalNetMsg msg;
		memset(&msg,0,sizeof(msg));
		//local network header
		msg._header._client_type = 	_network_id;
		msg._header._msg_type = EIB_MSG_TYPE_CLINET_DISCONNECT;
		//encrypt the data using the shared key
		CDataBuffer::Encrypt(&msg,sizeof(msg),&_encryptor.GetSharedKey());
		START_TRY
			//send the data to the network
			_data_sock.SendTo(&msg,sizeof(msg),_eib_address,_eib_port);
		END_TRY_START_CATCH_ANY
			//do nothing in case of socket error
		END_CATCH
	}
	if(_thread->isAlive()){
		_thread->Close();
		_thread->join();
	}
}

bool CGenericServer::OpenConnection(const CString& network_name, const CString& eib_server_adress,
							   int eib_server_port,const CString& initial_key,const CString& local_ip,
							   const CString& user_name, const CString& password)
{
	int num_tries = 3;
	bool result = false;

	if (!result && num_tries > 0){
		result = OpenConnection(network_name.GetBuffer(),eib_server_adress.GetBuffer(),eib_server_port,
			initial_key.GetBuffer(),local_ip.GetBuffer(), user_name.GetBuffer(),password.GetBuffer());
		JTCThread::sleep(500);
		num_tries--;
	}

	return result;
}

bool CGenericServer::FirstPhaseConnection(const CString& key,const char* local_ip,
										  char* buff, int buf_len,int& reply_length)
{
	CString s_address;
	int s_port;
	CDataBuffer request;

	//request line
	CHttpRequest http_request(GET_M,DIFFIE_HELLMAN_CLIENT_HELLO,HTTP_1_0,EMPTY_STRING);
	//headers
	http_request.AddHeader(NETWORK_NAME_HEADER,_network_name);
	http_request.AddHeader(DATA_PORT_HEADER,_data_sock.GetLocalPort());
	http_request.AddHeader(KEEPALIVE_PORT_HEADER,_thread->GetHeartBeatPort());
	http_request.AddHeader(ADDRESS_HEADER,local_ip);
	//end
	http_request.Finalize(request);

	CDataBuffer::Encrypt(request.GetBuffer(),request.GetLength(),&key);
	

	START_TRY
		
		GetLog()->SetConsoleColor(YELLOW);
		GetLog()->Log(LOG_LEVEL_INFO,"[%s] [Send] Client Hello [%s:%d --> %s:%d]",GetUserName().GetBuffer(),
						local_ip,_data_sock.GetLocalPort(),_eib_address.GetBuffer(),_eib_port);
		_data_sock.SendTo(request.GetBuffer(),request.GetLength(),_eib_address,_eib_port);
		reply_length = _data_sock.RecvFrom(buff,buf_len,s_address,s_port,5000);

	END_TRY_START_CATCH(ex)
		GetLog()->SetConsoleColor(RED);
		GetLog()->Log(LOG_LEVEL_ERROR,"[%s] Cannot connect to eib server: %s",GetUserName().GetBuffer(),ex.what());
 		return false;
	END_TRY_START_CATCH_SOCKET(e)
		GetLog()->SetConsoleColor(RED);
		GetLog()->Log(LOG_LEVEL_ERROR,"[%s] Socket Error: Reason: %s",GetUserName().GetBuffer(),e.what());
 		return false;
	END_TRY_START_CATCH_ANY
		GetLog()->SetConsoleColor(RED);
		GetLog()->Log(LOG_LEVEL_ERROR,"[%s] Cannot connect to eib server... Unknown Exception.",GetUserName().GetBuffer());
		return false;
	END_CATCH

	if (reply_length > 0){
		return true;
	}
	//no reply from EIB...
	GetLog()->SetConsoleColor(RED);
	GetLog()->Log(LOG_LEVEL_ERROR,"No reply from EIB Server. maybe EIBServer is down?");
	return false;
}

const CString& CGenericServer::GetSharedKey()
{
	return _encryptor.GetSharedKey();
}

//function to open connection using the auto discovery service
bool CGenericServer::OpenConnection(const char* network_name, const char* initial_key,
									const char* local_ip, const char* user_name,
									const char* password)
{
	if(this->IsConnected())
		return false;

	_status = STATUS_DURING_CONNECT;
	CString ini_key(initial_key);

	//Send discovery reuqest
	UDPSocket discovery_sock;
	//Bind to some random port
	discovery_sock.SetLocalAddressAndPort(local_ip,0);
	//Generate the actual request
	CHttpRequest discovery_req(GET_M, EIB_SERVER_AUTO_DISCOVERY_REQ, HTTP_1_0, EMPTY_STRING);
	discovery_req.AddHeader(ADDRESS_HEADER,local_ip);
	discovery_req.AddHeader(DATA_PORT_HEADER,discovery_sock.GetLocalPort());
	CDataBuffer raw_req;
	discovery_req.Finalize(raw_req);
	//encrypt request using the key
	raw_req.Encrypt(&ini_key);
	//send the request
	discovery_sock.SendTo(raw_req.GetBuffer(), raw_req.GetLength(), AUTO_DISCOVERY_SERVICE_ADDRESS, AUTO_DISCOVERY_SERVICE_PORT);
	//wait (max 10 seconds) to response
	char buffer[MAX_URL_LENGTH];
	CString tmp_addr;
	int tmp_port;
	int len = discovery_sock.RecvFrom(buffer, sizeof(buffer),tmp_addr,tmp_port,5000);
	if(len == 0){
		GetLog()->Log(LOG_LEVEL_ERROR,"No data received. timeout.");
		return false;
	}

	CDataBuffer dbf(buffer,len);
	dbf.Decrypt(&ini_key);
	CHttpReply reply;
	CHttpParser parser(reply,dbf);
	if(!parser.IsLegalRequest() || reply.GetStatusCode() != STATUS_OK){
		return false;
	}

	CHttpHeader header;
	if(!reply.GetHeader(ADDRESS_HEADER,header)){
		return false;
	}
	CString eibserveraddr = header.GetValue();

	if(!reply.GetHeader(DATA_PORT_HEADER,header)){
		return false;
	}
	int eibserverport =  header.GetValue().ToInt();

	return OpenConnection(network_name, eibserveraddr.GetBuffer(), eibserverport, initial_key, local_ip, user_name, password);
}

bool CGenericServer::OpenConnection(const char* network_name, const char* eib_server_adress,
							   int eib_server_port,const char* initial_key,const char* local_ip,
							   const char* user_name, const char* password)
{
	_status = STATUS_DURING_CONNECT;
	
	CString ini_key(initial_key);
	
	CString s_address;
	int len = 0,s_port;
	CDataBuffer raw_data;
	char buff[MAX_URL_LENGTH];
	_network_name = network_name;
	_eib_address = eib_server_adress;
	_eib_port = eib_server_port;
	_user_name = user_name;

	_data_sock.SetLocalAddressAndPort(local_ip,0);

	int num_tries = 1;

	while (!FirstPhaseConnection(ini_key,local_ip, buff,MAX_URL_LENGTH,len) && num_tries > 0){
		--num_tries;
	}

	if(num_tries == 0){
		return false;
	}

	CDataBuffer raw_reply(buff,len);
	raw_reply.Decrypt(&ini_key);

	CHttpReply reply;
	CHttpParser parser(reply,raw_reply);

	if(!parser.IsLegalRequest() || reply.GetStatusCode() != STATUS_OK){
		_status = STATUS_DISCONNECTED;
		return false;
	}
	CHttpHeader header;
	int64 s_interim,g,n,r_interim,key;
	if(!reply.GetHeader(NETWORK_SESSION_ID_HEADER,header)){
		_status = STATUS_DISCONNECTED;
		return false;
	}
	_session_id = header.GetValue().ToInt();
	if(!reply.GetHeader(DIFFIE_HELLAM_MODULUS,header)){
		_status = STATUS_DISCONNECTED;
		return false;
	}
	n = header.GetValue().ToInt64();
	if(!reply.GetHeader(DIFFIE_HELLAM_INTERIM,header)){
		_status = STATUS_DISCONNECTED;
		return false;
	}
	s_interim = header.GetValue().ToInt64();
	if(!reply.GetHeader(DIFFIE_HELLAM_GENERATOR,header)){
		_status = STATUS_DISCONNECTED;
		return false;
	}
	g = header.GetValue().ToInt64();

	if(!reply.GetHeader(DATA_PORT_HEADER,header)){
		_status = STATUS_DISCONNECTED;
		return false;
	}
	_eib_port = header.GetValue().ToInt();
	if(!reply.GetHeader(KEEPALIVE_PORT_HEADER,header)){
		_status = STATUS_DISCONNECTED;
		return false;
	}
	
	GetLog()->SetConsoleColor(YELLOW);
	GetLog()->Log(LOG_LEVEL_INFO,"[EIB] [Received] Server Public key");

	int eib_ka_port = header.GetValue().ToInt();

	_encryptor.CreateRecipientInterKey(r_interim,g,n);
	_encryptor.CreateRecipientEncryptionKey(key,s_interim);

	CHttpRequest http_request;

	http_request.SetMethod(GET_M);
	http_request.SetRequestURI(DIFFIE_HELLMAN_CLIENT_PUBLIC_DATA);
	http_request.SetVersion(HTTP_1_0);
	http_request.AddHeader(DIFFIE_HELLAM_INTERIM,r_interim);
	http_request.AddHeader(CLIENT_TYPE_HEADER,(int)GetNetworkID());
	http_request.Finalize(raw_data);
	
	raw_data.Encrypt(&ini_key);

	_data_sock.SendTo(raw_data.GetBuffer(),raw_data.GetLength(),_eib_address,_eib_port);
	GetLog()->SetConsoleColor(YELLOW);
	GetLog()->Log(LOG_LEVEL_INFO,"[%s] [Send] Client Public key",GetUserName().GetBuffer());

	len = _data_sock.RecvFrom(buff,sizeof(buff),s_address,s_port);
	raw_data.Clear();
	raw_data.Add(buff,len);
	raw_data.Decrypt(&_encryptor.GetSharedKey());
	parser.SetData(reply,raw_data);
	if(!parser.IsLegalRequest() || reply.GetStatusCode() != STATUS_OK){
		_status = STATUS_DISCONNECTED;
		return false;
	}
	
	GetLog()->SetConsoleColor(YELLOW);
	GetLog()->Log(LOG_LEVEL_INFO,"[EIB] [Received] Keys exchanged succesfuly.");

	if(!Authenticate(user_name,password,&_encryptor.GetSharedKey())){
		_status = STATUS_DISCONNECTED;
		return false;
	}

	//set marker
	_status = STATUS_CONNECTED;

	//start keep alive thread here
	_thread->Init(eib_ka_port,_eib_address, this);
	_thread->start();
	return true;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

CHeartBeatThread::CHeartBeatThread() :
_sock(0),
_parent(NULL),
_stop(false)
{
}

CHeartBeatThread::~CHeartBeatThread()
{
}

void CHeartBeatThread::Init(int server_port,const CString& server_address, CGenericServer* parent)
{
	_server_port = server_port;
	_server_address = server_address;
	_heartbeat_interval = 10 * 1000; //[YGYG] Default heart beat interval - 10 seconds

	//Parent
	_parent = parent;

	this->setName("HeartBeat Thread");
}

void CHeartBeatThread::Close()
{
	JTCSynchronized sync(*this);
	_stop = true;
	this->notify();
}

void CHeartBeatThread::run()
{
	ASSERT_ERROR(_parent != NULL,"Parent pointer is NULL!");

	JTCSynchronized sync(*this);

	CGenericServer& server = *_parent;
	const CString* key = &server.GetSharedKey();
	ClientHeartBeatMsg msg;
	while (!_stop)
	{
		msg._header._client_type = server.GetNetworkID();
		msg._header._msg_type = EIB_MSG_TYPE_KEEP_ALIVE;
		msg._session_id = server.GetSessionID();

		CDataBuffer::Encrypt(&msg,sizeof(ClientHeartBeatMsg),key);

		START_TRY
			_sock.SendTo(&msg,sizeof(ClientHeartBeatMsg),_server_address,_server_port);
			//server.GetLog()->SetConsoleColor(BLUE);
			//server.GetLog()->Log(LOG_LEVEL_DEBUG,"[%s] Heart Beat sent.",server.GetUserName().GetBuffer());	

			time_t start = time(NULL);
			int len = _sock.RecvFrom(&msg,sizeof(ClientHeartBeatMsg),_server_address,_server_port, _heartbeat_interval);
			time_t end = time(NULL);

			if(len == 0){
				server.GetLog()->Log(LOG_LEVEL_ERROR,"[EIB] Heart beat Ack Timeout. EIB Server is probably dead. Exit");
				break;
			}

			CDataBuffer::Decrypt(&msg,sizeof(ClientHeartBeatMsg),key);
			if(msg._header._msg_type == EIB_MSG_TYPE_KEEP_ALIVE_ACK && msg._header._client_type == EIB_TYPE_EIB_SERVER){
				server.GetLog()->SetConsoleColor(YELLOW);
				server.GetLog()->Log(LOG_LEVEL_DEBUG,"[EIB] Heart beat --> [OK].");
				this->wait((int)(_heartbeat_interval - difftime(end,start)));
			}
			else if(msg._header._msg_type == EIB_MSG_TYPE_SERVER_DISCONNECT && msg._header._client_type == EIB_TYPE_EIB_SERVER){
				server.GetLog()->Log(LOG_LEVEL_ERROR,"[EIB] EIB Server closed the connection. Exit.");
				break;
			}
			else{
				server.GetLog()->Log(LOG_LEVEL_ERROR,"[EIB] Incorrect Heart beat Ack received. Exit.");
				break;
			}
		END_TRY_START_CATCH_SOCKET(e)
			server.GetLog()->Log(LOG_LEVEL_ERROR,"Socket Error at Client Heartbeat Thread: %s",e.what());
			break;
		END_CATCH
	}

	server.SetStatus(STATUS_DISCONNECTED);
}
