#include "BusMonConnection.h"
#include "EIBServer.h"

using namespace EibStack;

CBusMonConnection::CBusMonConnection(const CString& ipaddress):
CTunnelingConnection(ipaddress)
{
}

CBusMonConnection::~CBusMonConnection()
{

}

bool CBusMonConnection::Connect()
{
	CString tmp_ip;
	int tmp_port,len = 0,timeout;
	unsigned char buffer[256];

	CTunnelingConnection::InitConnectionParams();

	//sanity
	if(_device_control_address.GetLength() == 0 || _device_control_port == 0){
		return false;
	}

	//send the connect request
	CConnectRequest con_req(CConnectRequest::TunnelConnection ,CConnectRequest::TunnelBusMon,_data_sock.GetLocalPort(),_ipaddress);
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

bool CBusMonConnection::HandleTunnelRequest(unsigned char* buffer, int len, CCemi_L_Data_Frame& frame)
{
	CCemi_L_BusMon_Frame f(buffer, len);
	LOG_DEBUG("[Received] [Bus] Status: %d Time: %s",f.GetFrameError(), f.GetTimeStamp().Format().GetBuffer());
	return false;
}
