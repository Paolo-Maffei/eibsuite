#include "DisconnectResponse.h"

using namespace EibStack;

CDisconnectResponse::CDisconnectResponse(unsigned char* data):
CEIBNetPacket<EIBNETIP_DISCONNECT_RESPONSE>(data)
{
	_data.channelid = data[0];
    _data.status = data[1];
}

CDisconnectResponse::CDisconnectResponse(unsigned char channelid, unsigned char status) :
CEIBNetPacket<EIBNETIP_DISCONNECT_RESPONSE>(DISCONNECT_RESPONSE)
{
	_data.channelid = channelid;
    _data.status = status;
}

CDisconnectResponse::~CDisconnectResponse()
{
}

void CDisconnectResponse::FillBuffer(unsigned char* buffer, int max_length)
{
	CEIBNetPacket<EIBNETIP_DISCONNECT_RESPONSE>::FillBuffer(buffer,max_length);
	unsigned char* tmp_ptr = buffer + GetHeaderSize();
	memcpy(tmp_ptr,&_data,GetDataSize());
}
