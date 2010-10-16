#include "Emulator-ng.h"
#include "SearchRequest.h"
#include "SearchResponse.h"
#include "ConnectRequest.h"
#include "ConnectResponse.h"
#include "ConnectionStateRequest.h"
#include "ConnectionStateResponse.h"
#include "DisconnectRequest.h"
#include "DisconnectResponse.h"
#include "TunnelRequest.h"
#include "TunnelAck.h"
#include "DescriptionRequest.h"

using namespace EibStack;

CEmulatorHandler::CEmulatorHandler() :
_server_conf(NULL),
_log_file(NULL)
{
	_input_handler = new CEmulatorInputHandler();
	_data_output_handler = new CEmulatorOutputHandler();

	_input_handler->SetParent(this);
	_data_output_handler->SetParent(this);
	memset(_states, 0, sizeof(_states));
}

CEmulatorHandler::~CEmulatorHandler()
{
	for(int i = 0; i < MAX_CONNS; i++){
		if(_states[i] != NULL){
			delete _states[i];
		}
	}
}

void CEmulatorHandler::InitState(CEmulatorHandler::ConnectionState* s)
{
	if(s == NULL)
		return;

	s->id = 0;
	s->is_connected = false;
	s->channelid = 0;
	s->recv_sequence = 0;
	s->send_sequence = 0;
	s->_timeout.SetNow();
	s->_timeout += HEARTBEAT_REQUEST_TIME_OUT;
}

void CEmulatorHandler::Init(CEmulatorConfig* server_conf, CLogFile* log_file)
{
	_log_file = log_file;
	_server_conf = server_conf;

	_input_handler->Init();
}

void CEmulatorHandler::Close()
{
	_input_handler->Close();
	_input_handler->join();

	_data_output_handler->Close();
	_data_output_handler->join();
}

void CEmulatorHandler::Start()
{
	_input_handler->start();
	_data_output_handler->start();
}

void CEmulatorHandler::Broadcast(const CCemi_L_Data_Frame& frame)
{
	//lock this object so if cleanup occured it will wait
	JTCSynchronized s(*this);

	for(int i = 0; i < MAX_CONNS; i++)
	{
		if(_states[i] != NULL){
			SendTunnelToClient(frame, _states[i]);
		}
	}
}

void CEmulatorHandler::CheckConnectionsCleanup()
{
	//lock this object so if broadcast occured it will wait
	JTCSynchronized s(*this);
	
	//check if any connection has expired
	for(int i = 0; i < MAX_CONNS; i++)
	{
		if(_states[i] != NULL && _states[i]->_timeout.secTo() == 0){
			// Connection timeout. force close connection
			LOG_ERROR("[Connection timeout] Closing connection with client %d.", _states[i]->channelid);
			FreeConnection(_states[i]);
		}
	}
}

CEmulatorHandler::ConnectionState* CEmulatorHandler::GetState(int channel)
{
	for(int i = 0; i < MAX_CONNS; i++)
	{
		if(_states[i] != NULL && channel == _states[i]->channelid){
			return _states[i];
		}
	}
	return NULL;	
}

CEmulatorHandler::ConnectionState* CEmulatorHandler::AllocateNewState(const CString& source_ip, int sourc_port)
{
	CEmulatorHandler::ConnectionState* s = NULL;
	for(int i = 0; i < MAX_CONNS; i++)
	{
		if(_states[i] == NULL){
			//allocate memory
			s = new CEmulatorHandler::ConnectionState();
			InitState(s);
			_states[i] = s;
			//set the connection id
			s->id = i;
			srand((unsigned)time(0));
			unsigned char channelid = 0;
			do
			{
				//assign new channel id (make sure its unique)
				channelid = rand() % 0xFF;
			}while(GetState(channelid) != NULL);
			s->channelid = channelid;
			break;
		}
	}
	if(s == NULL){
		LOG_ERROR("Error: EIB Relay is already connected to max number of clients.");
	}
	return s;
}

void CEmulatorHandler::FreeConnection(CEmulatorHandler::ConnectionState* s)
{
	if(s == NULL){
		return;
	}
	CEmulatorHandler::ConnectionState* ptr = GetState(s->channelid);
	if(ptr != NULL){
		int id = ptr->id;
		_states[id] = NULL;
		free(ptr);
		LOG_DEBUG("Resources for Connection %d Cleaned successfuly.",id);
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

CEmulatorHandler::CEmulatorInputHandler::CEmulatorInputHandler() :
JTCThread("CRelayControlHandler"),
_relay(NULL),
_stop(false),
_local_addr(EMPTY_STRING),
_local_port(0)
{

}

CEmulatorHandler::CEmulatorInputHandler::~CEmulatorInputHandler()
{
}

void CEmulatorHandler::CEmulatorInputHandler::Init()
{
	START_TRY
		//set the source port to default EIB Port (3671)
		_local_addr = Socket::LocalAddress(_relay->_server_conf->GetListenInterface());
		_sock.SetLocalAddressAndPort(_local_addr,EIB_PORT);
		_local_port = _sock.GetLocalPort();
		_sock.JoinGroup(_local_addr,EIB_MULTICAST_ADDRESS);
	END_TRY_START_CATCH_SOCKET(e)
		throw CEIBException(GeneralError, e.what());
	END_CATCH
}

void CEmulatorHandler::CEmulatorInputHandler::run()
{
	unsigned char buffer[256];
	CString src_ip;
	int len = 0, src_port, timeout_interval = 1000;

	while (!_stop)
	{
		_relay->CheckConnectionsCleanup();
		
		len = _sock.RecvFrom(buffer, sizeof(buffer), src_ip, src_port, timeout_interval);

		if(len == 0){
			//check if timeout expired
			continue;
		}

		EIBNETIP_HEADER* header = ((EIBNETIP_HEADER*)buffer);
		header->servicetype = htons(header->servicetype);
		header->totalsize = htons(header->totalsize);

		switch (header->servicetype)
		{
		case CONNECTIONSTATE_REQUEST:
			HandleConnectionStateRequest(buffer,sizeof(buffer));
			break;
		case DISCONNECT_REQUEST:
			HandleDisconnectRequest(buffer,sizeof(buffer));
			break;
		case DISCONNECT_RESPONSE:
			HandleDisconnectResponse(buffer,sizeof(buffer));
			break;
		case SEARCH_REQUEST:
			HandleSearchRequest(buffer,sizeof(buffer));
			break;
		case CONNECT_REQUEST:
			HandleConnectRequest(buffer,sizeof(buffer));
			break;
		case DESCRIPTION_REQUEST:
			HandleDescriptionRequest(buffer,sizeof(buffer));
			break;
		case TUNNELLING_REQUEST:
			HandleTunnelRequest(buffer, sizeof(buffer));
			break;
		case TUNNELLING_ACK:
			HandleTunnelAck(buffer, sizeof(buffer));
			break;
		case ROUTING_INDICATION:
		case ROUTING_LOST_MESSAGE:
			break;
		default:
			LOG_ERROR("[Received] [Unknown service message code] Source: %s:%d", src_ip.GetBuffer(), src_port);
			break;
		}
	}
}

void CEmulatorHandler::CEmulatorInputHandler::HandleTunnelAck(unsigned char* buffer, int max_len)
{
	START_TRY
		CTunnelingAck ack(buffer);
		
		CEmulatorHandler::ConnectionState* s = _relay->GetState(ack.GetChannelId());
		if(s == NULL){
			LOG_ERROR("Error: Wrong channel id in tunnel ack (ignoring)");
			return;
		}else{
			LOG_DEBUG("[Received] [Client %d] [Tunnel Ack]", s->channelid);
		}

		JTCSynchronized sync(s->state_monitor);
		if(s->send_sequence == ack.GetSequenceNumber()){
			//increment the send sequence
			s->send_sequence++;
		}else{
			LOG_ERROR("Error: Incorrect Sequnece number in Tunnel Ack. Ignore Ack.");
		}

	END_TRY_START_CATCH(e)
		LOG_ERROR("Error in tunnel ack parsing: %s",e.what());
	END_TRY_START_CATCH_SOCKET(ex)
		LOG_ERROR("Socket Error in tunnel ack parsing: %s",ex.what());
	END_TRY_START_CATCH_ANY
		LOG_ERROR("Unknown Error in tunnel ack parsing");
	END_CATCH
}

void CEmulatorHandler::CEmulatorInputHandler::HandleDisconnectResponse(unsigned char* buffer, int max_len)
{
	START_TRY
		CDisconnectResponse resp(buffer);
		CEmulatorHandler::ConnectionState* s = _relay->GetState(resp.GetChannelID());
		if(s == NULL){
			LOG_ERROR("Error: Wrong channel id in disconnect response (ignoring)");
			return;
		}else{
			LOG_DEBUG("[Received] [Client %d] [Disconnect Response]", s->channelid);
		}

		//reset the connection state
		_relay->FreeConnection(s);

	END_TRY_START_CATCH(e)
		LOG_ERROR("Error in disconnect response parsing: %s",e.what());
	END_TRY_START_CATCH_SOCKET(ex)
		LOG_ERROR("Socket Error in disconnect response parsing: %s",ex.what());
	END_TRY_START_CATCH_ANY
		LOG_ERROR("Unknown Error in disconnect response parsing");
	END_CATCH
}

void CEmulatorHandler::CEmulatorInputHandler::HandleTunnelRequest(unsigned char* buffer, int max_len)
{
	START_TRY
		
		CTunnelingRequest req(buffer);
		CEmulatorHandler::ConnectionState* s = _relay->GetState(req.GetChannelId());
		
		if(s == NULL){
			LOG_ERROR("[Received] [Client %d] [Tunnel Request] Error: cannot find connection with channel id: %d", req.GetChannelId());
			return;
		}

		JTCSynchronized sync(s->state_monitor);
		if(req.GetChannelId() != s->channelid){
			//wrong channel -> send error ack
			CTunnelingAck ack(s->channelid, 0, E_CONNECTION_ID);
			ack.FillBuffer(buffer, max_len);
			_sock.SendTo(buffer, ack.GetTotalSize(), s->_remote_ctrl_addr, s->_remote_ctrl_port);
			LOG_ERROR("[Received] [Client %d] [Tunnel Request] Error: Wrong channel id (sending error ack)", s->channelid);
			return;
		}
		if(req.GetSequenceNumber() != s->recv_sequence){
			//wrong sequence number -> send error ack
			CTunnelingAck ack(s->channelid, 0, E_SEQUENCE_NUMBER);
			ack.FillBuffer(buffer, max_len);
			_sock.SendTo(buffer, ack.GetTotalSize(), s->_remote_ctrl_addr, s->_remote_ctrl_port);
			LOG_ERROR("[Received] [Client %d] [Tunnel Request] Error: Wrong sequence id (sending error ack)", s->channelid);
			return;
		}
		
		switch(req.GetcEMI().GetMessageCode())
		{
		case L_DATA_REQ:
			LOG_DEBUG("[Received] [Client %d] [Tunnel Request] Data Request", s->channelid);
			break;
		case L_DATA_CON:
			LOG_DEBUG("[Received] [Client %d] [Tunnel Request] Data Confirmation", s->channelid);
			break;
		case L_DATA_IND:
			LOG_DEBUG("[Received] [Client %d] [Tunnel Request] Data Indication", s->channelid);
			break;
		default:
			LOG_DEBUG("[Received] [Client %d] [Tunnel Request] UNKNOWN message code!!!", s->channelid);
			break;
		}

		//sending an ack back
		CTunnelingAck ack(s->channelid, s->recv_sequence , E_NO_ERROR);
		//increment the recv sequence
		s->recv_sequence++;
		ack.FillBuffer(buffer, max_len);
		
		//everything is ok. we send forward the request to EIB Sever and waiting for confirmation
		//if(_relay->SendEIBNetwork(req.GetcEMI(), WAIT_FOR_CONFRM)){
		//	LOG_DEBUG("[Send] [EIB] [Raw frame from client]");
		//}
		//We send the ACK back over the Data channel (the channel that the request was received from)
		_sock.SendTo(buffer, ack.GetTotalSize(), s->_remote_data_addr,s->_remote_data_port);

	END_TRY_START_CATCH(e)
		LOG_ERROR("Error in tunnel request parsing: %s",e.what());
	END_TRY_START_CATCH_SOCKET(ex)
		LOG_ERROR("Socket Error in tunnel request parsing: %s",ex.what());
	END_TRY_START_CATCH_ANY
		LOG_ERROR("Unknown Error in tunnel request parsing");
	END_CATCH
}

void CEmulatorHandler::CEmulatorInputHandler::HandleDisconnectRequest(unsigned char* buffer, int max_len)
{
	START_TRY
		CDisconnectRequest req(buffer);
		CEmulatorHandler::ConnectionState* s = _relay->GetState(req.GetChannelID());
		if(s == NULL){
			CDisconnectResponse resp(req.GetChannelID(), E_CONNECTION_ID);
			resp.FillBuffer(buffer, max_len);
			_sock.SendTo(buffer, resp.GetTotalSize(), req.GetControlAddr(), req.GetControlPort());
			LOG_ERROR("Error: Wrong channel id in disconnect request (sending error disconnect response)");
			return;
		}else{
			LOG_DEBUG("[Received] [Client %d] [Disconnect Request]", s->channelid);
		}

		do
		{
			JTCSynchronized sync(s->state_monitor);
		
			if(!s->is_connected){
				LOG_ERROR("Error: Recevied disconnect request when no connection is open. (ignoring)");
				return;
			}

			LOG_DEBUG("[Send] [Client %d] [Disconnect Response]", s->channelid);

			CDisconnectResponse resp(s->channelid, E_NO_ERROR);
			resp.FillBuffer(buffer, max_len);
			CString remote_ip = s->_remote_ctrl_addr;
			int remote_port = s->_remote_ctrl_port;
			_sock.SendTo(buffer, resp.GetTotalSize(), remote_ip, remote_port);
		}while(0);

		_relay->FreeConnection(s);

	END_TRY_START_CATCH(e)
		LOG_ERROR("Error in search disconnect request parsing: %s",e.what());
	END_TRY_START_CATCH_SOCKET(ex)
		LOG_ERROR("Socket Error in disconnect request parsing: %s",ex.what());
	END_TRY_START_CATCH_ANY
		LOG_ERROR("Unknown Error in disconnect request parsing");
	END_CATCH
}

void CEmulatorHandler::CEmulatorInputHandler::HandleConnectionStateRequest(unsigned char* buffer, int max_len)
{
	START_TRY
		CConnectionStateRequest req(buffer);
		CEmulatorHandler::ConnectionState* s =	_relay->GetState(req.GetChannelId());
		
		if(s == NULL){
			LOG_ERROR("Error: Wrong channel id in connection state request (sending error connection state response)");
			return;
		}

		LOG_DEBUG("[Received] [Client %d] [Connection state Request]", s->channelid);

		JTCSynchronized sync(s->state_monitor);
		if(!s->is_connected){
			return;
		}
		if(s->channelid != req.GetChannelId()){
			//send connection state response
			CConnectionStateResponse resp(s->channelid, E_CONNECTION_ID);
			resp.FillBuffer(buffer, max_len);
			_sock.SendTo(buffer, resp.GetTotalSize(),s->_remote_ctrl_addr,	s->_remote_ctrl_port);
			LOG_ERROR("Error: Wrong channel id in connection state request (sending error connection state response)");
			return;
		}

		//reset the timeout
		s->_timeout.SetNow();
		s->_timeout += HEARTBEAT_REQUEST_TIME_OUT;

		CConnectionStateResponse resp(s->channelid, E_NO_ERROR);
		resp.FillBuffer(buffer, max_len);
		LOG_DEBUG("[Send] [Connection State Response]");
		_sock.SendTo(buffer, resp.GetTotalSize(), s->_remote_ctrl_addr, s->_remote_ctrl_port);

	END_TRY_START_CATCH(e)
		LOG_ERROR("Error in search connection state request parsing: %s",e.what());
	END_TRY_START_CATCH_SOCKET(ex)
		LOG_ERROR("Socket Error in connection state request parsing: %s",ex.what());
	END_TRY_START_CATCH_ANY
		LOG_ERROR("Unknown Error in connection state request parsing");
	END_CATCH
}

void CEmulatorHandler::CEmulatorInputHandler::HandleDescriptionRequest(unsigned char* buffer, int max_len)
{
	START_TRY
		CDescriptionRequest req(buffer);
		LOG_DEBUG("[Received] [Description Request]");
		EIBNETIP_DEVINF_DIB dib;
		dib.structlength = sizeof(EIBNETIP_DEVINF_DIB);
		dib.descriptiontypecode = DEVICE_INFO;
		dib.knxmedium = TP1; //YGYG - get this info from real device
		dib.devicestatus = 0x1;


		throw CEIBException(NotImplementedError,"Not implemented yet!!!");
	END_TRY_START_CATCH(e)
		LOG_ERROR("Error in description request parsing: %s",e.what());
	END_TRY_START_CATCH_SOCKET(ex)
		LOG_ERROR("Socket Error in description request parsing: %s",ex.what());
	END_TRY_START_CATCH_ANY
		LOG_ERROR("Unknown Error in description request parsing");
	END_CATCH
}

void CEmulatorHandler::CEmulatorInputHandler::HandleConnectRequest(unsigned char* buffer, int max_len)
{
	START_TRY
		CConnectRequest req(buffer);
		LOG_DEBUG("[Received] [Connect Request]");
		//TODO: here need to check that we do not have another client with the same ip:port since this will create many many errors.
		ConnectionState* state = _relay->AllocateNewState(req.GetControlAddress(), req.GetControlPort());
		if(state == NULL){
			//we already have max open connections. we do not support more than that.
			LOG_ERROR("[Send] [Client] Connect Response Error. No more connections.");
			CConnectResponse resp(0, E_NO_MORE_CONNECTIONS, _local_addr, _local_port, req.GetConnectionType());
			resp.FillBuffer(buffer, max_len);
			_sock.SendTo(buffer, resp.GetTotalSize(), req.GetControlAddress(), req.GetControlPort());
			return;
		}

		switch(req.GetConnectionType())
		{
		case CConnectRequest::TunnelConnection:
			break;
		case CConnectRequest::RemoteLogConnection:
		case CConnectRequest::RemoteConfConnection:
		case CConnectRequest::ObjSvrConnection:
		case CConnectRequest::DeviceMgmntConnection:
			CConnectResponse resp(0, E_CONNECTION_TYPE, _local_addr, _local_port, req.GetConnectionType());
			resp.FillBuffer(buffer, max_len);
			_sock.SendTo(buffer, resp.GetTotalSize(), req.GetControlAddress(), req.GetControlPort());
			LOG_ERROR("Error: Connection type is not implemented");
			return;
		}

		//validate host protocol type here

		//set the connection params in the state object :
		//control channel
		state->_remote_ctrl_addr = req.GetControlAddress();
		state->_remote_ctrl_port = req.GetControlPort();
		
		//data channel
		state->_remote_data_addr = req.GetDataAddress();
		state->_remote_data_port = req.GetDataPort();
		
		//mark connection is open
		state->is_connected = true;
		
		//send response back (we state the our Data endpoint is the same as our control endpoint)
		CConnectResponse resp(state->channelid, E_NO_ERROR, _local_addr, _local_port, req.GetConnectionType());
		resp.FillBuffer(buffer, max_len);
		_sock.SendTo(buffer, resp.GetTotalSize(), state->_remote_ctrl_addr, state->_remote_ctrl_port);
		LOG_DEBUG("[Send] [Connect Response] [%s:%d] Channel ID: %d", state->_remote_ctrl_addr.GetBuffer(), state->_remote_ctrl_port, state->channelid);

	END_TRY_START_CATCH(e)
		LOG_ERROR("Error in connect request parsing: %s",e.what());
	END_TRY_START_CATCH_SOCKET(ex)
		LOG_ERROR("Socket Error in connect request parsing: %s",ex.what());
	END_TRY_START_CATCH_ANY
		LOG_ERROR("Unknown Error in connect request parsing");
	END_CATCH
}

void CEmulatorHandler::CEmulatorInputHandler::HandleSearchRequest(unsigned char* buffer, int max_len)
{
	START_TRY
		CSearchRequest req(buffer);
		LOG_DEBUG("[Received] [Search Request]");	
		//send search response back to the sender
		char serial[6] = { 0 };
		unsigned long mcast = inet_addr(EIB_MULTICAST_ADDRESS);
		const char* name = "EIB Relay Device";
		CSearchResponse resp(_local_addr,_local_port, MEDIUM_TP1, CEibAddress((unsigned int)0, false),
				0, serial, mcast, serial, name, (SERVICE_CORE | SERVICE_DEV_MNGMT | SERVICE_TUNNELING));
		resp.FillBuffer(buffer, max_len);
		_sock.SendTo(buffer,resp.GetTotalSize(),req.GetRemoteIPAddress(),req.GetRemotePort());
		LOG_DEBUG("[Send] [Search Response]");
	END_TRY_START_CATCH(e)
		LOG_ERROR("Error in search request parsing: %s",e.what());
	END_TRY_START_CATCH_SOCKET(ex)
		LOG_ERROR("Socket Error in search request parsing: %s",ex.what());
	END_TRY_START_CATCH_ANY
		LOG_ERROR("Unknown Error in search request parsing");
	END_CATCH
}

void CEmulatorHandler::CEmulatorInputHandler::SendTunnelToClient(const CCemi_L_Data_Frame& frame, ConnectionState* s)
{
	START_TRY
		JTCSynchronized sync(s->state_monitor);
		if(s->is_connected){
			unsigned char buffer[256];
			LOG_DEBUG("[Send] [Client %d] [Raw frame]", s->channelid);
			CTunnelingRequest req(s->channelid, s->send_sequence, frame);
			req.FillBuffer(buffer, sizeof(buffer));
			_sock.SendTo(buffer, req.GetTotalSize(), s->_remote_data_addr, s->_remote_data_port);
		}else{
			LOG_ERROR("[Received] [EIB] Raw frame. no client connected: ignoring.");
		}
	END_TRY_START_CATCH(e)
		LOG_ERROR("Error in SendTunnelToClient: %s",e.what());
	END_TRY_START_CATCH_SOCKET(ex)
		LOG_ERROR("Socket Error in SendTunnelToClient: %s",ex.what());
	END_TRY_START_CATCH_ANY
		LOG_ERROR("Unknown Error in SendTunnelToClient");
	END_CATCH
}

void CEmulatorHandler::CEmulatorInputHandler::Close()
{
	_stop = true;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

CEmulatorHandler::CEmulatorOutputHandler::CEmulatorOutputHandler() :
JTCThread("CRelayDataOutputHandler"),
_relay(NULL),
_stop(false)
{
}

CEmulatorHandler::CEmulatorOutputHandler::~CEmulatorOutputHandler()
{
}

void CEmulatorHandler::CEmulatorOutputHandler::Close()
{
	_stop = true;
}

void CEmulatorHandler::CEmulatorOutputHandler::run()
{
	CCemi_L_Data_Frame frame;
	
	//while(!_stop)
	//{
		//int len = _relay->ReceiveEIBNetwork(frame,2000);
		//if(len == 0){
		//	continue;
		//}
		//LOG_DEBUG("[Received] [EIB] [Raw frame]");
		//_relay->Broadcast(frame);
	//}
}
