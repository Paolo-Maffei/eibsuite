#include "ConnectionStateRequest.h"

using namespace EibStack;

CConnectionStateRequest::CConnectionStateRequest(unsigned char channelid, int port, const CString& ipaddress):
CEIBNetPacket<EIBNETIP_CONNECTIONSTATE_REQUEST>(CONNECTIONSTATE_REQUEST)
{
	_data.channelid = channelid;
	_data.reserved = 0;
	CHPAI controlendpoint(port,ipaddress);
	memcpy(&_data.controlendpoint,controlendpoint.ToByteArray(),controlendpoint.GetDataSize());
}

CConnectionStateRequest::CConnectionStateRequest(unsigned char* data) :
CEIBNetPacket<EIBNETIP_CONNECTIONSTATE_REQUEST>(data)
{
	_data.channelid = data[0];
	CHPAI ctrl_endpoint(&data[2]);
	memcpy(&_data.controlendpoint,ctrl_endpoint.ToByteArray(),ctrl_endpoint.GetDataSize()); 
}

CConnectionStateRequest::~CConnectionStateRequest()
{
}

void CConnectionStateRequest::FillBuffer(unsigned char* buffer, int max_length)
{
	CEIBNetPacket<EIBNETIP_CONNECTIONSTATE_REQUEST>::FillBuffer(buffer,max_length);
	unsigned char* tmp_ptr = buffer + GetHeaderSize();
	memcpy(tmp_ptr,&_data,GetDataSize());

	EIBNETIP_CONNECTIONSTATE_REQUEST* ptr = ((EIBNETIP_CONNECTIONSTATE_REQUEST*)tmp_ptr);
	ptr->controlendpoint.port = htons(_data.controlendpoint.port);
}
