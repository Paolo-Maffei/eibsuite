#include "DisconnectRequest.h"

using namespace EibStack;

CDisconnectRequest::CDisconnectRequest(unsigned char* data) :
CEIBNetPacket<EIBNETIP_DISCONNECT_REQUEST>(data)
{
	_data.channelid = data[0];
	_data.reserved = data[1];

	CHPAI control(&data[2]);
	memcpy(&_data.controlendpoint,control.ToByteArray(),control.GetDataSize());

	_ctrl_addr = control.GetAddress();
	_ctrl_port = control.GetPort();
}

CDisconnectRequest::CDisconnectRequest(unsigned char channelid, int port, const CString& ipaddress):
CEIBNetPacket<EIBNETIP_DISCONNECT_REQUEST>(DISCONNECT_REQUEST)
{
	_data.channelid = channelid;
	_data.reserved = 0;

	CHPAI hpai(port,ipaddress);
	memcpy(&_data.controlendpoint,hpai.ToByteArray(),hpai.GetDataSize());

	_ctrl_addr = ipaddress;
	_ctrl_port = port;
}

CDisconnectRequest::~CDisconnectRequest()
{
}

void CDisconnectRequest::FillBuffer(unsigned char* buffer, int max_length)
{
	CEIBNetPacket<EIBNETIP_DISCONNECT_REQUEST>::FillBuffer(buffer,max_length);
	unsigned char* tmp_ptr = buffer + GetHeaderSize();
	memcpy(tmp_ptr,&_data,GetDataSize());

	EIBNETIP_DISCONNECT_REQUEST* ptr = ((EIBNETIP_DISCONNECT_REQUEST*)tmp_ptr);
	ptr->controlendpoint.port = htons(_data.controlendpoint.port);
}
