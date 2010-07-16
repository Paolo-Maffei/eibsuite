#include "TunnelAck.h"
#include "Globals.h"

using namespace EibStack;

CTunnelingAck::CTunnelingAck(unsigned char channelid, unsigned char sequencecounter, unsigned char status):
CEIBNetPacket<EIBNETIP_TUNNELING_ACK>(TUNNELLING_ACK)
{
	_data.structlength = 4;
	_data.channelid = channelid;
	_data.sequencecounter = sequencecounter;
    _data.typespecific = status;
}

CTunnelingAck::CTunnelingAck(unsigned char* data) :
CEIBNetPacket<EIBNETIP_TUNNELING_ACK>(data)
{
	_data.structlength = data[0];
	_data.channelid = data[1];
	_data.sequencecounter = data[2];
	_data.typespecific = data[4];
}

CTunnelingAck::~CTunnelingAck()
{
}

CString CTunnelingAck::GetStatusString() const
{
	switch (_data.typespecific)
	{
        case E_CONNECTION_ID:  return CString("Wrong channelid");
        case E_CONNECTION_OPTION: return CString("Connection option not supported");
        case E_CONNECTION_TYPE: return CString("Connection type not supported");
        case E_DATA_CONNECTION: return CString("Problem with data connection");
        case E_HOST_PROTOCOL_TYPE: return CString("Hostprotocol type not supported");
        case E_KNX_CONNECTION: return CString("KNX connection error");
        case E_NO_ERROR: return CString("No error");
        case E_NO_MORE_CONNECTIONS: return CString("No more connections");
        case E_SEQUENCE_NUMBER: return CString("Wrong Sequencenumber");
        case E_TUNNELING_LAYER: return CString("Tunneling problem");
        case E_VERSION_NOT_SUPPORTED: return CString("EIBNETIP version not supported");
        default: return CString("Unknown error");
    }

	return EMPTY_STRING;
}

void CTunnelingAck::FillBuffer(unsigned char* buffer, int max_length)
{
	CEIBNetPacket<EIBNETIP_TUNNELING_ACK>::FillBuffer(buffer,max_length);
	unsigned char* tmp_ptr = buffer + GetHeaderSize();
	memcpy(tmp_ptr,&_data,GetDataSize());
}
