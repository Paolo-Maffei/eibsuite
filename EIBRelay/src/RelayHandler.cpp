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
	_control_handler = new CRelayControlHandler();
	_data_output_handler = new CRelayDataOutputHandler();

	_control_handler->_relay = this;
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
	_control_handler->Init();
}

void CRelayHandler::Close()
{
	CGenericServer::Close();

	_control_handler->Close();
	_control_handler->join();

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
	_control_handler->start();
	_data_output_handler->start();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

CRelayHandler::CRelayControlHandler::CRelayControlHandler() :
JTCThread("CRelayControlHandler"),
_relay(NULL),
_stop(false),
_ctrl_addr(EMPTY_STRING),
_ctrl_port(0)
{

}

CRelayHandler::CRelayControlHandler::~CRelayControlHandler()
{
}

void CRelayHandler::CRelayControlHandler::Init()
{
	//set the source port to default EIB Port (3671)
	_ctrl_addr = Socket::LocalAddress(_relay->_server_conf->GetListenInterface());
	_sock.SetLocalAddressAndPort(_ctrl_addr,EIB_PORT);
	_ctrl_port = _sock.GetLocalPort();
	_sock.JoinGroup(_ctrl_addr,EIB_MULTICAST_ADDRESS);
}

void CRelayHandler::CRelayControlHandler::run()
{
	unsigned char buffer[256];
	CString src_ip;
	int len = 0, src_port, timeout_interval = 1000;
	CTime timeout;
	timeout.SetNow();

	while (!_stop)
	{
		len = _sock.RecvFrom(buffer, sizeof(buffer), src_ip, src_port, timeout_interval);

		if(len == 0){
			//reduce the timeout
			
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

void CRelayHandler::CRelayControlHandler::HandleTunnelAck(unsigned char* buffer, int max_len)
{
	START_TRY
		CTunnelingAck ack(buffer);
		JTCSynchronized sync(_relay->_state_monitor);	
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

void CRelayHandler::CRelayControlHandler::HandleDisconnectResponse(unsigned char* buffer, int max_len)
{
	START_TRY
		CDisconnectResponse resp(buffer);
		JTCSynchronized sync(_relay->_state_monitor);	
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

void CRelayHandler::CRelayControlHandler::HandleTunnelRequest(unsigned char* buffer, int max_len)
{
	START_TRY
		CTunnelingRequest req(buffer);
		JTCSynchronized sync(_relay->_state_monitor);
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
			LOG_DEBUG("[Received] [Client %d] [Tunnel Request] UNKNOWN", _relay->_state._channelid);
			break;
		}

		//req.GetcEMI().Dump();
		LOG_DEBUG("[Send] [EIB] [Raw frame from client]");
		//everything is ok. we send forward the request to EIB Sever and waiting for confirmation
		_relay->SendEIBNetwork(req.GetcEMI(), WAIT_FOR_CONFRM);
		//sending an ack back
		CTunnelingAck ack(_relay->_state._channelid, _relay->_state._recv_sequence, E_NO_ERROR);
		ack.FillBuffer(buffer, max_len);
		_sock.SendTo(buffer, ack.GetTotalSize(), _relay->_state._remote_ctrl_addr, _relay->_state._remote_ctrl_port);
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

void CRelayHandler::CRelayControlHandler::HandleDisconnectRequest(unsigned char* buffer, int max_len)
{
	START_TRY
		CDisconnectRequest req(buffer);
		JTCSynchronized sync(_relay->_state_monitor);

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

void CRelayHandler::CRelayControlHandler::HandleConnectionStateRequest(unsigned char* buffer, int max_len)
{
	START_TRY
		CConnectionStateRequest req(buffer);
		JTCSynchronized sync(_relay->_state_monitor);
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

void CRelayHandler::CRelayControlHandler::HandleDescriptionRequest(unsigned char* buffer, int max_len)
{
	START_TRY
		CDescriptionRequest req(buffer);
		//throw CEIBException(NotImplementedError,"Not implemented yet!!!");
	END_TRY_START_CATCH(e)
		LOG_ERROR("Error in description request parsing: %s",e.what());
	END_TRY_START_CATCH_SOCKET(ex)
		LOG_ERROR("Socket Error in description request parsing: %s",ex.what());
	END_TRY_START_CATCH_ANY
		LOG_ERROR("Unknown Error in description request parsing");
	END_CATCH
}

void CRelayHandler::CRelayControlHandler::HandleConnectRequest(unsigned char* buffer, int max_len)
{
	START_TRY
		CConnectRequest req(buffer);
		JTCSynchronized sync(_relay->_state_monitor);
		
		if(_relay->_state._is_connected){
			//we already have an open connection. we do not support more than one
			CConnectResponse resp(0, E_NO_MORE_CONNECTIONS, _ctrl_addr, _ctrl_port, req.GetConnectionType());
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
		//_relay->_state._remote_data_addr = req.GetDataAddress();
		//_relay->_state._remote_data_port = req.GetDataPort();
		
		//mark connection is open
		_relay->_state._is_connected = true;

		//send response back
		CConnectResponse resp(_relay->_state._channelid, E_NO_ERROR, _ctrl_addr, _ctrl_port, req.GetConnectionType());
		resp.FillBuffer(buffer, max_len);
		LOG_DEBUG("[Send] [Connect Response] --> %s:%d", req.GetControlAddress().GetBuffer(), req.GetControlPort());
		_sock.SendTo(buffer, resp.GetTotalSize(), req.GetControlAddress(), req.GetControlPort());

	END_TRY_START_CATCH(e)
		LOG_ERROR("Error in connect request parsing: %s",e.what());
	END_TRY_START_CATCH_SOCKET(ex)
		LOG_ERROR("Socket Error in connect request parsing: %s",ex.what());
	END_TRY_START_CATCH_ANY
		LOG_ERROR("Unknown Error in connect request parsing");
	END_CATCH
}

void CRelayHandler::CRelayControlHandler::HandleSearchRequest(unsigned char* buffer, int max_len)
{
	START_TRY
		CSearchRequest req(buffer);
		//send search response back to the sender
		CSearchResponse resp(_ctrl_addr,_ctrl_port);
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

void CRelayHandler::CRelayControlHandler::Close()
{
	_stop = true;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

CRelayHandler::CRelayDataOutputHandler::CRelayDataOutputHandler() :
JTCThread("CRelayDataOutputHandler"),
_relay(NULL),
_stop(false)
{
}

CRelayHandler::CRelayDataOutputHandler::~CRelayDataOutputHandler()
{
}

void CRelayHandler::CRelayDataOutputHandler::Close()
{
	_stop = true;
}

void CRelayHandler::CRelayDataOutputHandler::run()
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
		JTCSynchronized sync(_relay->_state_monitor);
		if(_relay->_state._is_connected){
			LOG_DEBUG("[send] [Client %d] [Raw frame]", _relay->_state._channelid);
			CTunnelingRequest req(_relay->_state._channelid, _relay->_state._send_sequence, frame);
			req.FillBuffer(buffer, sizeof(buffer));
			_relay->_control_handler->_sock.SendTo(buffer, req.GetTotalSize(), _relay->_state._remote_ctrl_addr, _relay->_state._remote_ctrl_port);
		}else{
			LOG_DEBUG("[Received] [EIB] Raw frame. no client connected: ignoring.");
		}
	}
}
