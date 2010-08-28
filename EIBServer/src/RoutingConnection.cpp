#include "RoutingConnection.h"
#include "CemiFrame.h"
#include "EIBServer.h"

using namespace EibStack;
 
CRoutingConnection::CRoutingConnection(const CString& ipaddress) :
IConnection(),
_device_data_address(EIB_MULTICAST_ADDRESS),
_device_data_port(EIB_PORT),
_ipaddress(ipaddress)
{
}

CRoutingConnection::~CRoutingConnection()
{
}

bool CRoutingConnection::Connect()
{
	
	START_TRY
		//set the source port to default EIB Port (3671)
		_data_sock.SetLocalAddressAndPort(_ipaddress,EIB_PORT);
		_data_sock.JoinGroup(_ipaddress,EIB_MULTICAST_ADDRESS);
	END_TRY_START_CATCH_SOCKET(e)
		throw CEIBException(SocketError,e.what());
	END_CATCH
	return true;
}

bool CRoutingConnection::IsConnected() const
{
	return true;
}

void CRoutingConnection::DisConnect()
{
}

bool CRoutingConnection::SendDataFrame(const KnxElementQueue& elem)
{
	unsigned char buffer[256];
	CRoutingIndication req(elem._frame);
	req.FillBuffer(buffer,256);
	_data_sock.SendTo(buffer,req.GetTotalSize(),_device_data_address,_device_data_port);
	return true;
}

bool CRoutingConnection::ReceiveDataFrame(CCemi_L_Data_Frame& frame)
{
	unsigned char buffer[256];
	CString tmp_ip;
	int tmp_port;
	int len = _data_sock.RecvFrom(buffer,256,tmp_ip,tmp_port,2000);
	if(len == 0){
		return false;
	}
	
	LOG_DEBUG("[Received] [Routing indication]");

	EIBNETIP_HEADER* header = ((EIBNETIP_HEADER*)buffer);
	header->servicetype = htons(header->servicetype);
	header->totalsize = htons(header->totalsize);

	CRoutingIndication req(buffer);
	frame = req.GetCemiFrame();
	
	return true;
}
