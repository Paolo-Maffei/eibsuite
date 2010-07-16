#include "ConnectionStateResponse.h"

using namespace EibStack;

CConnectionStateResponse::CConnectionStateResponse(byte channelid, byte status) :
CEIBNetPacket<EIBNETIP_CONNECTIONSTATE_RESPONSE>(CONNECT_REQUEST)
{
}

CConnectionStateResponse::CConnectionStateResponse(unsigned char* data) :
CEIBNetPacket<EIBNETIP_CONNECTIONSTATE_RESPONSE>(data)
{
	_data.channelid = data[0];
    _data.status = data[1];
}

CConnectionStateResponse::~CConnectionStateResponse()
{
}

unsigned short CConnectionStateResponse::GetChannelID() const
{
	return _data.channelid;
}

unsigned char CConnectionStateResponse::GetStatus() const
{
	return _data.status;
}

CString CConnectionStateResponse::GetStatusString() const
{
	switch (_data.status)
	{
        case (E_CONNECTION_ID):
            return ("Wrong channel id");
        case (E_NO_ERROR):
            return ("Connection OK");
        default:
            return ("unknown status");
    }

	return EMPTY_STRING;
}

void CConnectionStateResponse::FillBuffer(unsigned char* buffer, int max_len)
{
	CEIBNetPacket<EIBNETIP_CONNECTIONSTATE_RESPONSE>::FillBuffer(buffer,max_len);
	unsigned char* tmp_ptr = buffer + GetHeaderSize();
	memcpy(tmp_ptr,&_data,GetDataSize());
}
