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

void CEmulatorHandler::SendIndication(const CGroupEntry& ge)
{
	_data_output_handler->EnqueueFrame(ge);
}

void CEmulatorHandler::Init(CEmulatorConfig* server_conf, CLogFile* log_file)
{
	_log_file = log_file;
	_server_conf = server_conf;

	_input_handler->Init();
}

void CEmulatorHandler::Close()
{
	DisconnectClients();

	_input_handler->Close();
	_input_handler->join();

	_data_output_handler->Close();
	_data_output_handler->join();
}

void CEmulatorHandler::DisconnectClients()
{
	JTCSynchronized s(*this);
	for(int i = 0; i < MAX_CONNS; i++)
	{
		if(_states[i] != NULL){
			_input_handler->DisconnectClient(_states[i]);
		}
	}
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
	bool sent = false;
	for(int i = 0; i < MAX_CONNS; i++)
	{
		if(_states[i] != NULL){
			SendTunnelToClient(frame, _states[i], true);
			sent = true;
		}
	}
	if(!sent){
		LOG_ERROR("\nNo client connected.");
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
_emulator(NULL),
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
		_local_addr = Socket::LocalAddress(_emulator->_server_conf->GetListenInterface());
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
		_emulator->CheckConnectionsCleanup();
		
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
		
		CEmulatorHandler::ConnectionState* s = _emulator->GetState(ack.GetChannelId());
		if(s == NULL){
			LOG_ERROR("Error: Wrong channel id in tunnel ack (ignoring)");
			return;
		}else{
			LOG_DEBUG("[Received] [Tunnel Ack]");
		}

		JTCSynchronized sync(s->state_monitor);
		if(s->send_sequence == ack.GetSequenceNumber()){
			//increment the send sequence
			s->send_sequence++;
		}else{
			LOG_ERROR("Error: Incorrect Sequence number in Tunnel Ack. Ignore Ack.");
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
		CEmulatorHandler::ConnectionState* s = _emulator->GetState(resp.GetChannelID());
		if(s == NULL){
			LOG_ERROR("Error: Wrong channel id in disconnect response (ignoring)");
			return;
		}else{
			LOG_DEBUG("[Received] [Disconnect Response]");
		}

		//reset the connection state
		_emulator->FreeConnection(s);

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
		CEmulatorHandler::ConnectionState* s = _emulator->GetState(req.GetChannelId());
		
		if(s == NULL){
			LOG_ERROR("[Received] [Tunnel Request] Error: cannot find connection with channel id: %d", req.GetChannelId());
			return;
		}

		JTCSynchronized sync(s->state_monitor);
		if(req.GetChannelId() != s->channelid){
			//wrong channel -> send error ack
			CTunnelingAck ack(s->channelid, 0, E_CONNECTION_ID);
			ack.FillBuffer(buffer, max_len);
			_sock.SendTo(buffer, ack.GetTotalSize(), s->_remote_ctrl_addr, s->_remote_ctrl_port);
			LOG_ERROR("[Received] [Tunnel Request] Error: Wrong channel id (sending error ack)");
			return;
		}
		if(req.GetSequenceNumber() != s->recv_sequence){
			//wrong sequence number -> send error ack
			CTunnelingAck ack(s->channelid, 0, E_SEQUENCE_NUMBER);
			ack.FillBuffer(buffer, max_len);
			_sock.SendTo(buffer, ack.GetTotalSize(), s->_remote_ctrl_addr, s->_remote_ctrl_port);
			LOG_ERROR("[Received] [Tunnel Request] Error: Wrong sequence id (sending error ack)");
			return;
		}
		
		switch(req.GetcEMI().GetMessageCode())
		{
		case L_DATA_REQ:
			if(req.GetcEMI().GetValueLength() == 1 && req.GetcEMI().GetAPCI() == 0 && req.GetcEMI().GetTPCI() == 0){
				LOG_DEBUG("[Received] [GROUP_READ Request] Read Value for %s", req.GetcEMI().GetDestAddress().ToString().GetBuffer());
			}
			break;
		case L_DATA_CON:
			LOG_DEBUG("[Received] [Tunnel Request] Data Confirmation");
			break;
		case L_DATA_IND:
			LOG_DEBUG("[Received] [Tunnel Request] Data Indication");
			break;
		default:
			LOG_DEBUG("[Received] [Tunnel Request] UNKNOWN message code!!!");
			break;
		}

		//sending an ack back
		CTunnelingAck ack(s->channelid, s->recv_sequence , E_NO_ERROR);
		//increment the recv sequence
		s->recv_sequence++;
		ack.FillBuffer(buffer, max_len);
		//We send the ACK back over the Data channel (the channel that the request was received from)
		_sock.SendTo(buffer, ack.GetTotalSize(), s->_remote_data_addr,s->_remote_data_port);

		//now we are going to check the contents of this tunnel request
		//1. Is it read request
		//2. Is it write request
		const CCemi_L_Data_Frame& cemi = req.GetcEMI();
		CEibAddress dst = cemi.GetDestAddress();
		const CEibAddress& src = CEIBEmulator::GetInstance().GetDB().GetPhyForGroup(dst);

		CCemi_L_Data_Frame con(cemi);
		con.SetMessageControl(L_DATA_CON);
		con.SetDestAddress(dst);
		con.SetSrcAddress(src);
		LOG_DEBUG("[Send] [Frame Confirmation]");
		_emulator->Broadcast(con);

		bool is_read_req = false;
		if(cemi.GetValueLength() == 1 && cemi.GetAPCI() == 0 && cemi.GetTPCI() == 0){
			//this is group read request. we now look in the db
			is_read_req = true;

			int len;
			unsigned char* result = CEIBEmulator::GetInstance().GetDB().GetValueForGroup(dst, len);
			if(len > 0 && result != NULL){

				CCemi_L_Data_Frame ind(L_DATA_IND,
						src,
						dst,
						result,
						len);
				ind.SetAPCI(GROUP_RESPONSE | ind.GetAPCI());
				LOG_DEBUG("[Send] [GROUP_RESPONE Indication] Value is: %s",CString::ToHexFormat((char*)result, len, true).GetBuffer());
				_emulator->Broadcast(ind);

			}
		}

		//this is a write request, we shall update the db and return confirmation
		if(!is_read_req){
			unsigned char data[MAX_EIB_VAL];
			data[0] = cemi.GetAPCI();
			if(cemi.GetValueLength() > 1){
				memcpy(&data[1], cemi.GetAddilData(), cemi.GetValueLength() - 1);
			}
			LOG_DEBUG("[Received] [GROUP_WRITE Request] Write Value for %s. Value: %s", cemi.GetDestAddress().ToString().GetBuffer(),
				CString::ToHexFormat((char*)data,cemi.GetValueLength(),true).GetBuffer());
			
			CEIBEmulator::GetInstance().GetDB().SetValueForGroup(dst, cemi);
		}

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
		CEmulatorHandler::ConnectionState* s = _emulator->GetState(req.GetChannelID());
		if(s == NULL){
			CDisconnectResponse resp(req.GetChannelID(), E_CONNECTION_ID);
			resp.FillBuffer(buffer, max_len);
			_sock.SendTo(buffer, resp.GetTotalSize(), req.GetControlAddr(), req.GetControlPort());
			LOG_ERROR("Error: Wrong channel id in disconnect request (sending error disconnect response)");
			return;
		}else{
			LOG_DEBUG("[Received] [Disconnect Request]");
		}

		do
		{
			JTCSynchronized sync(s->state_monitor);
		
			if(!s->is_connected){
				LOG_ERROR("Error: Received disconnect request when no connection is open. (ignoring)");
				return;
			}

			LOG_DEBUG("[Send] [Disconnect Response] [%s:%d] Channel ID: %d", s->_remote_ctrl_addr.GetBuffer(), s->_remote_ctrl_port, s->channelid);

			CDisconnectResponse resp(s->channelid, E_NO_ERROR);
			resp.FillBuffer(buffer, max_len);
			CString remote_ip = s->_remote_ctrl_addr;
			int remote_port = s->_remote_ctrl_port;
			_sock.SendTo(buffer, resp.GetTotalSize(), remote_ip, remote_port);
		}while(0);

		_emulator->FreeConnection(s);

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
		CEmulatorHandler::ConnectionState* s =	_emulator->GetState(req.GetChannelId());
		
		if(s == NULL){
			LOG_ERROR("Error: Wrong channel id in connection state request (sending error connection state response)");
			return;
		}

		LOG_DEBUG("[Received] [Connection state Request]");

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
		LOG_ERROR("Error in connection state request parsing: %s",e.what());
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
		ConnectionState* state = _emulator->AllocateNewState(req.GetControlAddress(), req.GetControlPort());
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
		LOG_DEBUG("[Received] [Search Request] [From: %s:%d]", req.GetRemoteIPAddress().GetBuffer(), req.GetRemotePort());	
		//send search response back to the sender
		char serial[6] = { 0 };
		unsigned long mcast = inet_addr(EIB_MULTICAST_ADDRESS);
		const char* name = "EIB Relay Device";
		CSearchResponse resp(_local_addr,_local_port, MEDIUM_TP1, CEibAddress((unsigned int)0, false),
				0, serial, mcast, serial, name, (SERVICE_CORE | SERVICE_DEV_MNGMT | SERVICE_TUNNELING));
		resp.FillBuffer(buffer, max_len);
		_sock.SendTo(buffer,resp.GetTotalSize(),req.GetRemoteIPAddress(),req.GetRemotePort());
		LOG_DEBUG("[Send] [Search Response] [To: %s:%d]", req.GetRemoteIPAddress().GetBuffer(), req.GetRemotePort());
	END_TRY_START_CATCH(e)
		LOG_ERROR("Error in search request parsing: %s",e.what());
	END_TRY_START_CATCH_SOCKET(ex)
		LOG_ERROR("Socket Error in search request parsing: %s",ex.what());
	END_TRY_START_CATCH_ANY
		LOG_ERROR("Unknown Error in search request parsing");
	END_CATCH
}

bool CEmulatorHandler::CEmulatorInputHandler::SendTunnelToClient(const CCemi_L_Data_Frame& frame, ConnectionState* s, bool wait4ack)
{
	START_TRY
		JTCSynchronized sync(s->state_monitor);
		if(s->is_connected){
			unsigned char buffer[256];
			CTunnelingRequest req(s->channelid, s->send_sequence, frame);
			req.FillBuffer(buffer, sizeof(buffer));
			_sock.SendTo(buffer, req.GetTotalSize(), s->_remote_data_addr, s->_remote_data_port);
		}else{
			LOG_ERROR("[Received] [EIB] Raw frame. no client connected: ignoring.");
		}

		if(wait4ack){
			unsigned char buffer[256];
			int len = 0, src_port, timeout_interval = 1000;
			CString src_ip;
			len = _sock.RecvFrom(buffer, sizeof(buffer), src_ip, src_port, timeout_interval);
			if(len == 0){
				return false;
			}
			EIBNETIP_HEADER* header = ((EIBNETIP_HEADER*)buffer);
			header->servicetype = htons(header->servicetype);
			header->totalsize = htons(header->totalsize);
			if(header->servicetype != TUNNELLING_ACK){
				return false;
			}
			HandleTunnelAck(buffer, sizeof(buffer));
		}

	END_TRY_START_CATCH(e)
		LOG_ERROR("Error in SendTunnelToClient: %s",e.what());
	END_TRY_START_CATCH_SOCKET(ex)
		LOG_ERROR("Socket Error in SendTunnelToClient: %s",ex.what());
	END_TRY_START_CATCH_ANY
		LOG_ERROR("Unknown Error in SendTunnelToClient");
	END_CATCH
	return true;
}

void CEmulatorHandler::CEmulatorInputHandler::DisconnectClient(ConnectionState* s)
{
	//YGYG: need to send disconnect request to all connected clients.
	START_TRY
		JTCSynchronized sync(s->state_monitor);
		if(s->is_connected){
			unsigned char buffer[256];
			CDisconnectRequest req(s->channelid, GetLocalCtrlPort(), GetLocalCtrlAddr());
			req.FillBuffer(buffer, sizeof(buffer));
			_sock.SendTo(buffer, req.GetTotalSize(), s->_remote_data_addr, s->_remote_data_port);
			LOG_DEBUG("[Send] [Disconnect Request]");
			JTCThread::sleep(200);
			_emulator->FreeConnection(s);
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
_emulator(NULL),
_stop(false)
{
}

CEmulatorHandler::CEmulatorOutputHandler::~CEmulatorOutputHandler()
{
}

void CEmulatorHandler::CEmulatorOutputHandler::Close()
{
	JTCSynchronized sync(_mon);
	_stop = true;
	_mon.notify();
}

void CEmulatorHandler::CEmulatorOutputHandler::EnqueueFrame(const CGroupEntry& ge)
{
	JTCSynchronized sync(_mon);
	_q.push(ge);
	_mon.notify();
}

void CEmulatorHandler::CEmulatorOutputHandler::run()
{
	CCemi_L_Data_Frame frame;
	JTCSynchronized sync(_mon);
	while(!_stop)
	{
		_mon.wait();
		while(_q.size() > 0){
			const CGroupEntry& ge = _q.front();

			CCemi_L_Data_Frame ind(L_DATA_IND,
									ge.GetPhyAddress(),
									ge.GetAddress(),
									(const unsigned char*)ge.GetValue(),
									ge.GetValueLen());
			_emulator->Broadcast(ind);
			_q.pop();
		}
	}
}
