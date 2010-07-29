#include "ConnectResponse.h"

using namespace EibStack;

CConnectResponse::CConnectResponse(int channel_id, int status,
								   const CString& data_addr, int data_port,
								   CConnectRequest::ConnectionType connection_type) :
CEIBNetPacket<EIBNETIP_CONNECT_RESPONSE>(CONNECT_RESPONSE)
{
	ASSERT_ERROR(channel_id <= 0xFF && channel_id >= 0, "Channel id must be in range [0,256]");
	ASSERT_ERROR(status <= 0xFF && status >= 0, "Status flag must be in range [0,256]");
	
	_data.channelid = channel_id;
	_data.status = status;
	
	CHPAI data_endpoint(data_port,data_addr);
	memcpy(&_data.dataendpoint, data_endpoint.ToByteArray(), data_endpoint.GetDataSize());

	unsigned char opts[2] = { 0, 0 };
	if(connection_type == CConnectRequest::TunnelConnection){
		opts[0] = TUNNEL_LINKLAYER;
	}
	CCRI_CRD crd(connection_type, opts);
	memcpy(&_data.crd, crd.ToByteArray(), crd.GetDataSize());

	_data_address = data_addr;
	_data_port = data_port;
}

CConnectResponse::CConnectResponse(unsigned char* data):
CEIBNetPacket<EIBNETIP_CONNECT_RESPONSE>(data)
{
	_data.channelid = data[0];
    _data.status = data[1];

	if(_data.status != E_NO_ERROR){
		throw CEIBException(EibPacketError,GetStatusString());
	}

	CHPAI data_endpoint(&data[2]);
	memcpy(&_data.dataendpoint,data_endpoint.ToByteArray(),data_endpoint.GetDataSize());

	_data_address = data_endpoint.GetAddress();
	_data_port = data_endpoint.GetPort();

	CCRI_CRD crd(&data[10]);
	memcpy(&_data.crd,crd.ToByteArray(),crd.GetDataSize());
	_dev_addr = CEibAddress(((crd.GetProtocolIndependentData() << 8) | (crd.GetProtocolDependentData())), false);
}

void CConnectResponse::FillBuffer(unsigned char* buffer, int max_len)
{
	CEIBNetPacket<EIBNETIP_CONNECT_RESPONSE>::FillBuffer(buffer,max_len);
	unsigned char* tmp_ptr = buffer + GetHeaderSize();
	memcpy(tmp_ptr,&_data,GetDataSize());

	EIBNETIP_CONNECT_RESPONSE* ptr = ((EIBNETIP_CONNECT_RESPONSE*)tmp_ptr);
	ptr->dataendpoint.port = htons(_data.dataendpoint.port);
}

CConnectResponse::~CConnectResponse()
{
}

CString CConnectResponse::GetStatusString()
{
	switch (_data.status)
	{
        case (E_CONNECTION_TYPE):
            return ("Connection type not supported");
        case (E_CONNECTION_OPTION):
            return ("Connection option not supported");
        case (E_NO_MORE_CONNECTIONS):
            return ("No more Connections supported");
        case (E_NO_ERROR):
            return ("Everything ok..... go on");
        default:
            return ("unknown status");
    }

	return EMPTY_STRING;
}
