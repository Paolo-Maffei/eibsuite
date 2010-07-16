#include "ConnectRequest.h"

using namespace EibStack;

CConnectRequest::CConnectRequest(unsigned char* data):
CEIBNetPacket<EIBNETIP_CONNECT_REQUEST>(data)
{
	unsigned char* ptr = data;
	CHPAI control_ep(ptr);
	ptr += control_ep.GetDataSize();
	CHPAI data_ep(ptr);
	ptr += data_ep.GetDataSize();
	CCRI_CRD crd(ptr);

	_ctrl_addr = control_ep.GetAddress();
	_ctrl_port = control_ep.GetPort();

	_data_addr = data_ep.GetAddress();
	_data_port = data_ep.GetPort();

	_con_type = (ConnectionType)crd.GetConnectionTypeCode();
}

CConnectRequest::CConnectRequest(ConnectionType connectionType, TunnelConnectionOption option, int port, const CString& ipaddress):
CEIBNetPacket<EIBNETIP_CONNECT_REQUEST>(CONNECT_REQUEST)
{
	CHPAI control(port,ipaddress);
	CHPAI data(port,ipaddress);

	_ctrl_addr = ipaddress;
	_ctrl_port = port;

	_data_addr = ipaddress;
	_data_port = port;

	_con_type = connectionType;

	unsigned char conType = (unsigned char)connectionType;
	unsigned char crioptions[2] = {0,0};
	if(conType == TunnelConnection){
		crioptions[0] = (unsigned char)option;
	}
	CCRI_CRD crd(conType,crioptions);

	memcpy(&_data.controlendpoint,control.ToByteArray(),control.GetDataSize());
	memcpy(&_data.dataendpoint,data.ToByteArray(),data.GetDataSize());
	memcpy(&_data.crd,crd.ToByteArray(),crd.GetDataSize());
}

CConnectRequest::~CConnectRequest()
{
}

void CConnectRequest::FillBuffer(unsigned char* buffer, int max_length)
{
	CEIBNetPacket<EIBNETIP_CONNECT_REQUEST>::FillBuffer(buffer,max_length);
	unsigned char* tmp_ptr = buffer + GetHeaderSize();
	memcpy(tmp_ptr,&_data,GetDataSize());

	EIBNETIP_CONNECT_REQUEST* ptr = ((EIBNETIP_CONNECT_REQUEST*)tmp_ptr);
	ptr->controlendpoint.port = htons(_data.controlendpoint.port);
	ptr->dataendpoint.port = htons(_data.dataendpoint.port);
}
