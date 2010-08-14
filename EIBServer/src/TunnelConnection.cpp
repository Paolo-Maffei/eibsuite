#include "TunnelConnection.h"
#include "EIBServer.h"

using namespace EibStack;

CTunnelingConnection::CTunnelingConnection(const CString& ipaddress) :
IConnection(),
_device_data_address(EMPTY_STRING),
_device_data_port(0),
_connection_status(DISCONNECTED),
_heartbeat(NULL),
_ipaddress(ipaddress),
_num_out_of_sync_pkts(0)
{
	_heartbeat = new CTunnelHeartBeat();
}

CTunnelingConnection::~CTunnelingConnection()
{
	delete _heartbeat;
	_heartbeat = NULL;
}

bool CTunnelingConnection::IsConnected() const
{
	return (_connection_status == CONNECTED);
}

bool CTunnelingConnection::Connect()
{
	CString tmp_ip;
	int tmp_port,len = 0,timeout;
	unsigned char buffer[256];
	
	START_TRY
		InitConnectionParams();
	END_TRY_START_CATCH(e)
		LOG_ERROR("Error: %s", e.what());
		return false;
	END_CATCH

	//sanity
	if(_device_control_address.GetLength() == 0 || _device_control_port == 0){
		return false;
	}

	//send the connect request
	//(we send both HPAI  identical - means the control channel & data channel will be the same on the EIB Server side
	CConnectRequest con_req(CConnectRequest::TunnelConnection ,CConnectRequest::TunnelLinkLayer,_data_sock.GetLocalPort(),_ipaddress);
	con_req.FillBuffer(buffer,256);
	_data_sock.SendTo(buffer,con_req.GetTotalSize(),_device_control_address,_device_control_port);

	//get the connect response
	timeout = CONNECT_REQUEST_TIME_OUT * 1000;
	START_TRY
		len = _data_sock.RecvFrom(buffer,256,tmp_ip,tmp_port,timeout);
	END_TRY_START_CATCH_ANY
		return false;
	END_CATCH
	
	//timeout expired
	if (len == 0){
		return false;
	}
	
	CConnectResponse con_resp(buffer);
	//set the channel id & data end point
	_state._channelid = con_resp.GetChannelID();
	_device_data_address = con_resp.GetDataIPAddress();
	_device_data_port = con_resp.GetDataPort();

	if(_heartbeat == NULL){
		_heartbeat = new CTunnelHeartBeat();
	}
	_heartbeat->start();

	_connection_status = CONNECTED;
	
	return true;
}

void CTunnelingConnection::InitConnectionParams()
{
	CString tmp_ip;
	int tmp_port, port;
	unsigned char buffer[256];

	//set the source port to random port selected by the OS
	START_TRY
		port = _data_sock.GetLocalPort();
	END_TRY_START_CATCH_ANY
		port = 0;
	END_CATCH

	if(!port){
		_data_sock.SetLocalAddressAndPort(_ipaddress);
		_data_sock.SetNonBlocking();
	}

	bool auto_discover = CEIBServer::GetInstance().GetConfig().GetAutoDetectEibDeviceAddress();

	if(auto_discover)
	{
		//send search request to multicast address
		CSearchRequest search_req(_data_sock.GetLocalPort(),_ipaddress);
		search_req.FillBuffer(buffer,256);
		_data_sock.SendTo(buffer,search_req.GetTotalSize(),EIB_MULTICAST_ADDRESS,EIB_PORT);
		//get the search response
		int len =_data_sock.RecvFrom(buffer,256,tmp_ip,tmp_port,3000);
		if(len == 0){
			//if we didn't got the search response in 3 seconds - lets used the configured params (ip address of the device)
			_device_control_address = CEIBServer::GetInstance().GetConfig().GetEibDeviceAddress();
			_device_control_port = EIB_PORT;
		}
		else{
			CSearchResponse search_resp(buffer, len);
			_device_control_address = search_resp.GetControlIPAddress();
			_device_control_port = search_resp.GetControlPort();
			LOG_DEBUG("Searching for KNX/IP on local network... Device found!");
			search_resp.Dump();

			EIBInterfaceInfo info;
			info.IsValid = true;
			info.KNXAddress = search_resp.GetDeviceDescription().GetAddress();
			info.MACAddr = search_resp.GetDeviceDescription().GetMACAddress();
			info.MulticastAddr = search_resp.GetDeviceDescription().GetMulticastAddress();
			info.Name = search_resp.GetDeviceDescription().GetName();
			info.SerialNumber = search_resp.GetDeviceDescription().GetSerialNum();
			info.SupportedServices = search_resp.GetDeviceDescription().GetSupportedServicesMask();
			CEIBServer::GetInstance().GetEIBInterface().SetInterfaceInfo(info);
		}
	}
	else
	{
		_device_control_address = CEIBServer::GetInstance().GetConfig().GetEibDeviceAddress();
		_device_control_port = EIB_PORT;
	}
}

void CTunnelingConnection::DisConnect()
{
	if(!IsConnected()){
		return;
	}
	unsigned char buffer[256];

	CDisconnectRequest dis_req(_state._channelid,GetLocalPort(),_ipaddress);
	dis_req.FillBuffer(buffer,256);

	UDPSocket sock(0);
	sock.SendTo(buffer,dis_req.GetTotalSize(),_device_control_address,_device_control_port);
	LOG_DEBUG("[Send] [BUS] [Disconnect Request]");
	_connection_status = WAITING_DISCONNECT_RESPONSE;

	_heartbeat->Close();
	while(_connection_status != DISCONNECTED){
		JTCThread::sleep(100);
	}
	_heartbeat = NULL;
}

//receives frames from the EIB Router
//Returns true if the frame is data (tunneling frame)
//Handles the frame If the frame is control-oriented
bool CTunnelingConnection::ReceiveDataFrame(CCemiFrame &frame)
{
	unsigned char buffer[256];
	int len;
	
	CString d_add;
	int d_port;

	len = _data_sock.RecvFrom(buffer,256,d_add,d_port,1000);
	
	if(len == 0){
		return false;
	}
	
	EIBNETIP_HEADER* header = ((EIBNETIP_HEADER*)buffer);
	header->servicetype = htons(header->servicetype);
	header->totalsize = htons(header->totalsize);

	bool res = false;

	//Validate that the service type in the packet is TUNNELING
	switch(header->servicetype >> 8)
	{
	case EIBNETIP_CORE:
		HandleCoreServices(buffer);
		break;
	case EIBNETIP_TUNNELING:
		res = HandleTunnelingServices(buffer,frame);
		break;
	case EIBNETIP_ROUTING:
	case EIBNETIP_REMLOG:
	case EIBNETIP_REMCONF:
	case EIBNETIP_OBJSRV:
		LOG_ERROR("[Received] [BUS] [Unsupported service type message] Service type: %d",header->servicetype >> 8);
		break;
	default:
		LOG_ERROR("[Received] [BUS] [Unknown service type message]");
		break;
	}

	return res;
}

void CTunnelingConnection::HandleCoreServices(unsigned char* recvdData)
{
	EIBNETIP_HEADER* header = ((EIBNETIP_HEADER*)recvdData);

	switch (header->servicetype)
	{
	case CONNECTIONSTATE_RESPONSE:
		LOG_DEBUG("[Received] [BUS] [Connection state response]");
		HandleConnectionStateResponse(recvdData);
		break;
	case DISCONNECT_REQUEST:
		LOG_DEBUG("[Received] [BUS] [Disconnect Request]");
		HandleDisconnectRequest(recvdData);
		break;
	case DISCONNECT_RESPONSE:
		LOG_DEBUG("[Received] [BUS] [Disconnect Response]");
		HandleDisconnectResponse(recvdData);
		break;
	default:
		LOG_ERROR("[Received] [BUS] [Unknown Core-service message]");
		break;
	}
}

bool CTunnelingConnection::HandleTunnelingServices(unsigned char* recvdData, CCemiFrame &frame)
{
	JTCSynchronized s(*this);
	EIBNETIP_HEADER* header = ((EIBNETIP_HEADER*)recvdData);
	bool res = false;
	switch (header->servicetype)
	{
	case TUNNELLING_REQUEST:
		res = HandleTunnelRequest(recvdData,frame);
		break;
	case TUNNELLING_ACK:
		HandleTunnelingAck(recvdData);
		break;
	default:
		LOG_ERROR("[Received] [BUS] [Unknown Tunnel service message]");
		break;
	}
	return res;
}


void CTunnelingConnection::HandleDisconnectRequest(unsigned char* buffer)
{	
	CDisconnectRequest dis_req(buffer);
	if(dis_req.GetChannelID() != _state._channelid){
		return;
	}

	CDisconnectResponse dis_resp(_state._channelid,E_NO_ERROR);
	unsigned char buf[256];
	dis_resp.FillBuffer(buf,256);
	UDPSocket sock;

	sock.SendTo(buf,dis_resp.GetTotalSize(),_device_control_address,_device_control_port);
	
	_heartbeat->Close();
}

void CTunnelingConnection::HandleDisconnectResponse(unsigned char* buffer)
{
	CDisconnectResponse dis_resp(buffer);
	if(dis_resp.GetStatus() != E_NO_ERROR ||
		dis_resp.GetChannelID() != _state._channelid){
		//error
		LOG_ERROR("[Received] [BUS] [Disconnect response with wrong channel id. disconnecting anyway.]");
	}

	JTCSynchronized s(*this);
	_state._channelid = 0;
	_state._recv_sequence = 0;
	_state._send_sequence = 0;

	SetStatusDisconnected();
}

void CTunnelingConnection::HandleTunnelingAck(unsigned char* buffer)
{
	CTunnelingAck ack(buffer);
	LOG_DEBUG("[Received] [BUS] [Tunnel Ack] Sequence: %d", ack.GetSequenceNumber());

	if(ack.GetChannelId() == _state._channelid && ack.GetSequenceNumber() == _state._send_sequence)
	{
		++_state._send_sequence;
	}

	map<int,JTCMonitor*>::iterator it = _waiting_for_acks.find(_state._send_sequence);
	if(it != _waiting_for_acks.end()){
		JTCSynchronized(*it->second);
		it->second->notify();
		_waiting_for_acks.erase(_state._send_sequence);
	}
}

void CTunnelingConnection::HandleConnectionStateResponse(unsigned char* buffer)
{
	CConnectionStateResponse cs_resp(buffer);
	if(cs_resp.GetStatus() != E_NO_ERROR){
		//error
		LOG_ERROR("[Received] [BUS] Connection state response contains error flag: %d",cs_resp.GetStatus());
		return;
	}

	_heartbeat->DecrementCounter();
}

bool CTunnelingConnection::HandleTunnelRequest(unsigned char* buffer,CCemiFrame &frame)
{
	CTunnelingRequest req(buffer);

	if(req.GetChannelId() != _state._channelid){
		//error
		LOG_ERROR("[Received] [BUS] CEMI frame with invalid channel id. [ignored]");
		return false;
	}

	if(req.GetSequenceNumber() == _state._recv_sequence ||
		req.GetSequenceNumber() + 1 == _state._recv_sequence)
	{
		if(req.GetSequenceNumber() == _state._recv_sequence){
			_num_out_of_sync_pkts = 0;
			const CCemiFrame& tmpfrm = req.GetcEMI();
			LOG_DEBUG("[Received] [BUS] [Tunnel request] Sequence: %d Dest Address: %s",req.GetSequenceNumber(),
					tmpfrm.GetDestAddress().ToString().GetBuffer());
		}else{
			LOG_ERROR("[Received] [BUS] [Tunnel request] Packet has invalid sequence number (%d). [sending ack but ignoring frame]",req.GetSequenceNumber());
		}
		unsigned char buf[20];
		CTunnelingAck ack(req.GetChannelId(),req.GetSequenceNumber(),E_NO_ERROR);
		ack.FillBuffer(buf,20);
		LOG_DEBUG("[Send] [BUS] [Tunnel Ack] Sequence: %d",req.GetSequenceNumber());
		_data_sock.SendTo(buf,ack.GetTotalSize(),_device_data_address,_device_data_port);
	}else{
		_num_out_of_sync_pkts++;
		//we have many pkts out of sync consecutively
		if(_num_out_of_sync_pkts > 3){
			//we are totally out of sync. reconnect.
			LOG_ERROR("[Received] [BUS] [Tunnel request] Packet has invalid sequence number (%d). reconnecting is 3 seconds.",req.GetSequenceNumber());
			Reconnect();
		}
	}

	if(req.GetSequenceNumber() != _state._recv_sequence)
	{
		LOG_ERROR("[Received] [BUS] [Tunnel request] Packet has invalid sequence number (%d). [ignoring]",req.GetSequenceNumber());
		return false;
	}

	++_state._recv_sequence;

	unsigned char mc = req.GetcEMI().GetMessageCode();
	//Data indication
	if(mc == L_DATA_IND || mc == L_BUSMON_IND)
	{
		//data indication or bus monitor indication should be processed
		frame = req.GetcEMI();
	}
	//Data confirmation
	else if (mc == L_DATA_CON)
	{
		if(req.GetcEMI().IsPositiveConfirmation()){
			//positive confirmation was received
			frame = req.GetcEMI();
			LOG_DEBUG("[Received] [BUS] [Positive confirmation] Sequence: %d", _state._recv_sequence);
			map<int,JTCMonitor*>::iterator it = _waiting_for_confirms.find(_state._recv_sequence);
			if(it != _waiting_for_confirms.end()){
				JTCMonitor* monitor = it->second;
				JTCSynchronized s(*monitor);
				_waiting_for_confirms.erase(_state._recv_sequence);
				monitor->notify();
			}		
			return true;
		}
		else
		{
			LOG_ERROR("[Received] [BUS] [Not supported message control field in cEMI frame (negative confirmation)]");
			return false;
		}
	}else{
		LOG_ERROR("[Received] [BUS] [Not supported message control field in cEMI frame]");
	}

	return true;
}


bool CTunnelingConnection::SendDataFrame(const KnxElementQueue& elem)
{
	unsigned char buffer[256];
	if(!IsConnected()){
		return false;
	}

	JTCSynchronized s(*this);
	CTunnelingRequest req(_state._channelid,_state._send_sequence,elem._frame);
	ASSERT_ERROR(!req.GetcEMI().IsExtendedFrame(),"Only standard frames are supported");
	ASSERT_ERROR(!req.GetcEMI().GetSourceAddress().IsGroupAddress(),"Only physical source address allowed");
	req.FillBuffer(buffer,256);
	if(elem._mode == WAIT_FOR_ACK && elem._optional_mon != NULL){
		_waiting_for_acks.insert(pair<int,JTCMonitor*>(_state._send_sequence + 1,elem._optional_mon));
	}
	else if(elem._mode == WAIT_FOR_CONFRM && elem._optional_mon != NULL){
		_waiting_for_confirms.insert(pair<int,JTCMonitor*>(_state._recv_sequence + 1,elem._optional_mon));
	}
	LOG_DEBUG("[Send] [BUS] [Tunnel Request] Sequence: %d. Dest Address: %s", req.GetSequenceNumber(), elem._frame.GetDestAddress().ToString().GetBuffer());
	_data_sock.SendTo(buffer,req.GetTotalSize(),_device_data_address,_device_data_port);
	return true;
}

void CTunnelingConnection::Reconnect()
{
	JTCThread::sleep(3000);
	this->DisConnect();
	this->Connect();
}

////////////////////////////////////////////////////////////////////////////////////

CTunnelHeartBeat::CTunnelHeartBeat() : _counter(0)
{
}

CTunnelHeartBeat::~CTunnelHeartBeat()
{
}

void CTunnelHeartBeat::Close()
{
	if(this->isAlive()){
		JTCSynchronized sync(*this);
		notify();
	}
}

void CTunnelHeartBeat::run()
{
	_sock.SetLocalPort(0);
	unsigned char buffer[256];

	CTunnelingConnection* ptr = ((CTunnelingConnection*)CEIBServer::GetInstance().GetEIBInterface().GetConnection());
	
	if (ptr == NULL){
		throw CEIBException(GeneralError,"Connection state is NULL");
	}

	CString device_address = ptr->GetDeviceControlAddress();
	int device_port = ptr->GetDeviceControlPort();
	unsigned char channelid = ptr->GetConnectionState()._channelid;
	
	CConnectionStateRequest cs_req(channelid,ptr->GetLocalPort(),ptr->GetLocalIPAddress());
	cs_req.FillBuffer(buffer,256);
	
	JTCSynchronized sync(*this);

	while (ptr->IsConnected())
	{
		if(_counter == 3){
			LOG_ERROR("[EIB Interface] Heartbeat to Device timeout. Stopping heartbeat thread.");
			break;
		}
		
		LOG_DEBUG("[Send] [BUS] [Connection State Request]");
		_sock.SendTo(buffer,cs_req.GetTotalSize(),device_address,device_port);
		++_counter;

		try
		{
			//1 Minute wait before sending new "connection state request..." packet
			wait(60000);
		}
		catch (const JTCInterruptedException& e)
		{
			LOG_ERROR("[JTC ERROR][%s]",e.getMessage());
			break;
		}
		catch (...)
		{
			LOG_ERROR("UnKnown Exception in Tunnel heartbit thread.");
		}
	}

	LOG_DEBUG("Tunnel connection heartbit closed.");
	ptr->SetStatusDisconnected();
	LOG_DEBUG("Tunnel connection with EIB Device is now closed.");
}

