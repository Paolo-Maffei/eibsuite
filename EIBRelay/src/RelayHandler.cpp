#include "EIBRelayServer.h"
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

CRelayHandler::CRelayHandler() :
CGenericServer(EIB_TYPE_RELAY_SERVER),
_server_conf(NULL),
_log_file(NULL)
{
	_input_handler = new CRelayInputHandler();
	_data_output_handler = new CRelayOutputHandler();

	_input_handler->_relay = this;
	_data_output_handler->_relay = this;

	InitState();
}

CRelayHandler::~CRelayHandler()
{
}

void CRelayHandler::InitState()
{
	_state._is_connected = false;
	_state._channelid = 0;
	_state._recv_sequence = 0;
	_state._send_sequence = 0;
}

void CRelayHandler::Init(CRelayServerConfig* server_conf, CLogFile* log_file)
{
	InitState();

	_log_file = log_file;
	_server_conf = server_conf;

	CGenericServer::Init(log_file);
	_input_handler->Init();
}

void CRelayHandler::Close()
{
	CGenericServer::Close();

	_input_handler->Close();
	_input_handler->join();

	_data_output_handler->Close();
	_data_output_handler->join();
}

bool CRelayHandler::Connect()
{
	//DEBUG
	//return true;
	
	bool established = false;
	if(_server_conf->GetAutoDiscoverEibServer())
	{
		_log_file->Log(LOG_LEVEL_INFO,"Searching EIB Server on local network...");
		established = this->OpenConnection(_server_conf->GetNetworkName().GetBuffer(),
											_server_conf->GetInitialKey().GetBuffer(),
											Socket::LocalAddress(_server_conf->GetListenInterface()).GetBuffer(),
											_server_conf->GetName().GetBuffer(),
											_server_conf->GetPassword().GetBuffer());
	}
	else
	{
		established = this->OpenConnection(_server_conf->GetNetworkName().GetBuffer(),
											_server_conf->GetEibIPAddress(),
											_server_conf->GetEibPort(),
											_server_conf->GetInitialKey().GetBuffer(),
											Socket::LocalAddress(_server_conf->GetListenInterface()).GetBuffer(),
											_server_conf->GetName().GetBuffer(),
											_server_conf->GetPassword().GetBuffer());
	}

	return established;
}

void CRelayHandler::Start()
{
	_input_handler->start();
	_data_output_handler->start();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

CRelayHandler::CRelayInputHandler::CRelayInputHandler() :
JTCThread("CRelayControlHandler"),
_relay(NULL),
_stop(false),
_local_addr(EMPTY_STRING),
_local_port(0)
{

}

CRelayHandler::CRelayInputHandler::~CRelayInputHandler()
{
}

void CRelayHandler::CRelayInputHandler::Init()
{
	//set the source port to default EIB Port (3671)
	_local_addr = Socket::LocalAddress(_relay->_server_conf->GetListenInterface());
	_sock.SetLocalAddressAndPort(_local_addr,EIB_PORT);
	_local_port = _sock.GetLocalPort();
	_sock.JoinGroup(_local_addr,EIB_MULTICAST_ADDRESS);
}

void CRelayHandler::CRelayInputHandler::run()
{
	unsigned char buffer[256];
	CString src_ip;
	int len = 0, src_port, timeout_interval = 1000;

	while (!_stop)
	{
		if(_relay->_state._is_connected && _relay->_state._timeout.secTo() == 0){
			// Connection time out...
			// force close connection
			LOG_ERROR("[Connection timeout] Closing connection with client %d.", _relay->_state._channelid);
		}

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
			LOG_DEBUG("[Received] [Client %d] [Connection state Request]", _relay->_state._channelid);
			HandleConnectionStateRequest(buffer,sizeof(buffer));
			break;
		case DISCONNECT_REQUEST:
			LOG_DEBUG("[Received] [Client %d] [Disconnect Request]", _relay->_state._channelid);
			HandleDisconnectRequest(buffer,sizeof(buffer));
			break;
		case DISCONNECT_RESPONSE:
			LOG_DEBUG("[Received] [Client %d] [Disconnect Response]", _relay->_state._channelid);
			HandleDisconnectResponse(buffer,sizeof(buffer));
			break;
		case SEARCH_REQUEST:
			LOG_DEBUG("[Received] [Search Request]");
			HandleSearchRequest(buffer,sizeof(buffer));
			break;
		case CONNECT_REQUEST:
			LOG_DEBUG("[Received] [Connect Request]");
			HandleConnectRequest(buffer,sizeof(buffer));
			break;
		case DESCRIPTION_REQUEST:
			LOG_DEBUG("[Received] [Description Request]");
			HandleDescriptionRequest(buffer,sizeof(buffer));
			break;
		case TUNNELLING_REQUEST:
			HandleTunnelRequest(buffer, sizeof(buffer));
			break;
		case TUNNELLING_ACK:
			HandleTunnelAck(buffer, sizeof(buffer));
			LOG_DEBUG("[Received] [Client %d] [Tunnel Ack]", _relay->_state._channelid);
			break;
		default:
			LOG_ERROR("[Received] [Client %d] [Unknown Core-service message]", _relay->_state._channelid);
			break;
		}
	}
}

void CRelayHandler::CRelayInputHandler::HandleTunnelAck(unsigned char* buffer, int max_len)
{
	START_TRY
		CTunnelingAck ack(buffer);
		JTCSynchronized sync(_relay->_state._state_monitor);
		if(ack.GetChannelId()  != _relay->_state._channelid){
			LOG_ERROR("Error: Wrong channel id in tunnel ack (ignoring)");
			return;
		}
		//increment the send sequence
		++_relay->_state._send_sequence;

	END_TRY_START_CATCH(e)
		LOG_ERROR("Error in disconnect response parsing: %s",e.what());
	END_TRY_START_CATCH_SOCKET(ex)
		LOG_ERROR("Socket Error in disconnect response parsing: %s",ex.what());
	END_TRY_START_CATCH_ANY
		LOG_ERROR("Unknown Error in disconnect response parsing");
	END_CATCH
}

void CRelayHandler::CRelayInputHandler::HandleDisconnectResponse(unsigned char* buffer, int max_len)
{
	START_TRY
		CDisconnectResponse resp(buffer);
		JTCSynchronized sync(_relay->_state._state_monitor);
		if(resp.GetChannelID() != _relay->_state._channelid){
			LOG_ERROR("Error: Wrong channel id in disconnect response (ignoring)");
			return;
		}
		//reset the connection state
		_relay->InitState();

	END_TRY_START_CATCH(e)
		LOG_ERROR("Error in disconnect response parsing: %s",e.what());
	END_TRY_START_CATCH_SOCKET(ex)
		LOG_ERROR("Socket Error in disconnect response parsing: %s",ex.what());
	END_TRY_START_CATCH_ANY
		LOG_ERROR("Unknown Error in disconnect response parsing");
	END_CATCH
}

void CRelayHandler::CRelayInputHandler::HandleTunnelRequest(unsigned char* buffer, int max_len)
{
	START_TRY
		CTunnelingRequest req(buffer);
		JTCSynchronized sync(_relay->_state._state_monitor);
		if(req.GetChannelId() != _relay->_state._channelid){
			//wrong channel -> send error ack
			CTunnelingAck ack(_relay->_state._channelid, 0, E_CONNECTION_ID);
			ack.FillBuffer(buffer, max_len);
			_sock.SendTo(buffer, ack.GetTotalSize(), _relay->_state._remote_ctrl_addr, _relay->_state._remote_ctrl_port);
			LOG_ERROR("[Received] [Client %d] [Tunnel Request] Error: Wrong channel id (sending error ack)", _relay->_state._channelid);
			return;
		}
		if(req.GetSequenceNumber() != _relay->_state._recv_sequence){
			//wrong sequence number -> send error ack
			CTunnelingAck ack(_relay->_state._channelid, 0, E_SEQUENCE_NUMBER);
			ack.FillBuffer(buffer, max_len);
			_sock.SendTo(buffer, ack.GetTotalSize(), _relay->_state._remote_ctrl_addr, _relay->_state._remote_ctrl_port);
			LOG_ERROR("[Received] [Client %d] [Tunnel Request] Error: Wrong sequence id (sending error ack)", _relay->_state._channelid);
			return;
		}
		
		switch(req.GetcEMI().GetMessageCode())
		{
		case L_DATA_REQ:
			LOG_DEBUG("[Received] [Client %d] [Tunnel Request] Data Request", _relay->_state._channelid);
			break;
		case L_DATA_CON:
			LOG_DEBUG("[Received] [Client %d] [Tunnel Request] Data Confirmation", _relay->_state._channelid);
			break;
		case L_DATA_IND:
			LOG_DEBUG("[Received] [Client %d] [Tunnel Request] Data Indication", _relay->_state._channelid);
			break;
		default:
			LOG_DEBUG("[Received] [Client %d] [Tunnel Request] UNKNOWN message code!!!", _relay->_state._channelid);
			break;
		}

		//req.GetcEMI().Dump();
		LOG_DEBUG("[Send] [EIB] [Raw frame from client]");
		//everything is ok. we send forward the request to EIB Sever and waiting for confirmation
		_relay->SendEIBNetwork(req.GetcEMI(), WAIT_FOR_CONFRM);
		//sending an ack back
		CTunnelingAck ack(_relay->_state._channelid, _relay->_state._recv_sequence, E_NO_ERROR);
		ack.FillBuffer(buffer, max_len);
		//We send the ACK back over the Data channel (the channel that the request was received from)
		_sock.SendTo(buffer, ack.GetTotalSize(), _relay->_state._remote_data_addr, _relay->_state._remote_data_port);
		//last thing - we increment the recv sequence
		++_relay->_state._recv_sequence;

	END_TRY_START_CATCH(e)
		LOG_ERROR("Error in tunnel request parsing: %s",e.what());
	END_TRY_START_CATCH_SOCKET(ex)
		LOG_ERROR("Socket Error in tunnel request parsing: %s",ex.what());
	END_TRY_START_CATCH_ANY
		LOG_ERROR("Unknown Error in tunnel request parsing");
	END_CATCH
}

void CRelayHandler::CRelayInputHandler::HandleDisconnectRequest(unsigned char* buffer, int max_len)
{
	START_TRY
		CDisconnectRequest req(buffer);
		JTCSynchronized sync(_relay->_state._state_monitor);

		if(!_relay->_state._is_connected){
			LOG_ERROR("Error: Recevied disconnect request when no connection is open. (ignoring)");
			return;
		}

		if(req.GetChannelID() != _relay->_state._channelid){
			CDisconnectResponse resp(_relay->_state._channelid, E_CONNECTION_ID);
			resp.FillBuffer(buffer, max_len);
			_sock.SendTo(buffer, resp.GetTotalSize(), _relay->_state._remote_ctrl_addr, _relay->_state._remote_ctrl_port);
			LOG_ERROR("Error: Wrong channel id in disconnect request (sending error disconnect response)");
			return;
		}

		LOG_DEBUG("[Send] [Client %d] [Disconnect Response]", _relay->_state._channelid);

		CDisconnectResponse resp(_relay->_state._channelid, E_NO_ERROR);
		resp.FillBuffer(buffer, max_len);
		_sock.SendTo(buffer, resp.GetTotalSize(), _relay->_state._remote_ctrl_addr, _relay->_state._remote_ctrl_port);
		//reset the connection state
		_relay->InitState();

	END_TRY_START_CATCH(e)
		LOG_ERROR("Error in search disconnect request parsing: %s",e.what());
	END_TRY_START_CATCH_SOCKET(ex)
		LOG_ERROR("Socket Error in disconnect request parsing: %s",ex.what());
	END_TRY_START_CATCH_ANY
		LOG_ERROR("Unknown Error in disconnect request parsing");
	END_CATCH
}

void CRelayHandler::CRelayInputHandler::HandleConnectionStateRequest(unsigned char* buffer, int max_len)
{
	START_TRY
		CConnectionStateRequest req(buffer);
		JTCSynchronized sync(_relay->_state._state_monitor);
		if(!_relay->_state._is_connected){
			return;
		}
		if(_relay->_state._channelid != req.GetChannelId()){
			//send connection state response
			CConnectionStateResponse resp(_relay->_state._channelid, E_CONNECTION_ID);
			resp.FillBuffer(buffer, max_len);
			_sock.SendTo(buffer, resp.GetTotalSize(), _relay->_state._remote_ctrl_addr,	_relay->_state._remote_ctrl_port);
			LOG_ERROR("Error: Wrong channel id in connection state request (sending error connection state response)");
			return;
		}

		_relay->_state._timeout.SetNow();
		//reset the timeout
		_relay->_state._timeout += 60;

		CConnectionStateResponse resp(_relay->_state._channelid, E_NO_ERROR);
		resp.FillBuffer(buffer, max_len);
		LOG_DEBUG("[Send] [Connection State Response]");
		_sock.SendTo(buffer, resp.GetTotalSize(), _relay->_state._remote_ctrl_addr, _relay->_state._remote_ctrl_port);

	END_TRY_START_CATCH(e)
		LOG_ERROR("Error in search connection state request parsing: %s",e.what());
	END_TRY_START_CATCH_SOCKET(ex)
		LOG_ERROR("Socket Error in connection state request parsing: %s",ex.what());
	END_TRY_START_CATCH_ANY
		LOG_ERROR("Unknown Error in connection state request parsing");
	END_CATCH
}

void CRelayHandler::CRelayInputHandler::HandleDescriptionRequest(unsigned char* buffer, int max_len)
{
	START_TRY
		CDescriptionRequest req(buffer);

		EIBNETIP_DEVINF_DIB dib;
		dib.structlength = sizeof(EIBNETIP_DEVINF_DIB);
		dib.descriptiontypecode = DEVICE_INFO;
		dib.knxmedium = TP1; //YGYG - get this info from real device
		dib.devicestatus = 0x1;


		//throw CEIBException(NotImplementedError,"Not implemented yet!!!");
	END_TRY_START_CATCH(e)
		LOG_ERROR("Error in description request parsing: %s",e.what());
	END_TRY_START_CATCH_SOCKET(ex)
		LOG_ERROR("Socket Error in description request parsing: %s",ex.what());
	END_TRY_START_CATCH_ANY
		LOG_ERROR("Unknown Error in description request parsing");
	END_CATCH
}

void CRelayHandler::CRelayInputHandler::HandleConnectRequest(unsigned char* buffer, int max_len)
{
	START_TRY
		CConnectRequest req(buffer);
		JTCSynchronized sync(_relay->_state._state_monitor);
		
		if(_relay->_state._is_connected){
			//we already have an open connection. we do not support more than one
			CConnectResponse resp(0, E_NO_MORE_CONNECTIONS, _local_addr, _local_port, req.GetConnectionType());
			resp.FillBuffer(buffer, max_len);
			_sock.SendTo(buffer, resp.GetTotalSize(), req.GetControlAddress(), req.GetControlPort());
			LOG_ERROR("Error: EIB Relay is already connected. only one connection is supported. (sending error connect response)");
			return;
		}

		//validate host protocol type here

		//assign new channel id
		srand((unsigned)time(0)); 
		_relay->_state._channelid = rand() % 0xFF;
		//set the connection params in the state object :
		//control channel
		_relay->_state._remote_ctrl_addr = req.GetControlAddress();
		_relay->_state._remote_ctrl_port = req.GetControlPort();
		
		//data channel
		_relay->_state._remote_data_addr = req.GetDataAddress();
		_relay->_state._remote_data_port = req.GetDataPort();
		
		//mark connection is open
		_relay->_state._is_connected = true;
		_relay->_state._timeout.SetNow();
		_relay->_state._timeout += 60; // the connection will expire in 60 secs from now (if no connection state request will be received)

		//send response back (we state the our Data endpoint is the same as our control endpoint)
		CConnectResponse resp(_relay->_state._channelid, E_NO_ERROR, _local_addr, _local_port, req.GetConnectionType());
		resp.FillBuffer(buffer, max_len);
		LOG_DEBUG("[Send] [Connect Response] [%s:%d]", _relay->_state._remote_ctrl_addr.GetBuffer(), _relay->_state._remote_ctrl_port);
		_sock.SendTo(buffer, resp.GetTotalSize(), _relay->_state._remote_ctrl_addr, _relay->_state._remote_ctrl_port);

	END_TRY_START_CATCH(e)
		LOG_ERROR("Error in connect request parsing: %s",e.what());
	END_TRY_START_CATCH_SOCKET(ex)
		LOG_ERROR("Socket Error in connect request parsing: %s",ex.what());
	END_TRY_START_CATCH_ANY
		LOG_ERROR("Unknown Error in connect request parsing");
	END_CATCH
}

void CRelayHandler::CRelayInputHandler::HandleSearchRequest(unsigned char* buffer, int max_len)
{
	START_TRY
		CSearchRequest req(buffer);
		//send search response back to the sender
		char serial[6] = { 0 };
		char mcast[4] = { 224, 0, 23, 12 };
		const char* name = "EIB Relay Device";
		CSearchResponse resp(_local_addr,_local_port, MEDIUM_TP1, CEibAddress((unsigned int)0, false),
				0, serial, mcast, serial, name);
		resp.FillBuffer(buffer, max_len);
		_sock.SendTo(buffer,resp.GetTotalSize(),req.GetRemoteIPAddress(),req.GetRemotePort());
	END_TRY_START_CATCH(e)
		LOG_ERROR("Error in search request parsing: %s",e.what());
	END_TRY_START_CATCH_SOCKET(ex)
		LOG_ERROR("Socket Error in search request parsing: %s",ex.what());
	END_TRY_START_CATCH_ANY
		LOG_ERROR("Unknown Error in search request parsing");
	END_CATCH
}

void CRelayHandler::CRelayInputHandler::Close()
{
	_stop = true;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

CRelayHandler::CRelayOutputHandler::CRelayOutputHandler() :
JTCThread("CRelayDataOutputHandler"),
_relay(NULL),
_stop(false)
{
}

CRelayHandler::CRelayOutputHandler::~CRelayOutputHandler()
{
}

void CRelayHandler::CRelayOutputHandler::Close()
{
	_stop = true;
}

void CRelayHandler::CRelayOutputHandler::run()
{
	CCemiFrame frame;
	unsigned char buffer[256];

	while(!_stop)
	{
		int len = _relay->ReceiveEIBNetwork(frame,2000);
		if(len == 0){
			continue;
		}
		LOG_DEBUG("[Received] [EIB] [Raw frame]");
		JTCSynchronized sync(_relay->_state._state_monitor);
		if(_relay->_state._is_connected){
			LOG_DEBUG("[send] [Client %d] [Raw frame]", _relay->_state._channelid);
			CTunnelingRequest req(_relay->_state._channelid, _relay->_state._send_sequence, frame);
			req.FillBuffer(buffer, sizeof(buffer));
			_relay->_input_handler->_sock.SendTo(buffer, req.GetTotalSize(), _relay->_state._remote_data_addr, _relay->_state._remote_data_port);
		}else{
			LOG_DEBUG("[Received] [EIB] Raw frame. no client connected: ignoring.");
		}
	}
}
