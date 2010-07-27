#include "DescriptionRequest.h"

using namespace EibStack;

CDescriptionRequest::CDescriptionRequest(const CString& ipaddr, int port) :
CEIBNetPacket<EIBNETIP_DESCRIPTION_REQUEST>(DESCRIPTION_REQUEST)
{
	CHPAI ep(port, ipaddr);
	memcpy(&_data.endpoint, ep.ToByteArray(), ep.GetDataSize());

	_ipaddr = ipaddr;
	_port = port;
}

CDescriptionRequest::CDescriptionRequest(unsigned char* data) :
CEIBNetPacket<EIBNETIP_DESCRIPTION_REQUEST>(data)
{
	unsigned char* ptr = data;
	CHPAI ep(ptr);
	memcpy(&_data.endpoint, ep.ToByteArray(), ep.GetDataSize());

	_ipaddr = ep.GetAddress();
	_port = ep.GetPort();
}

CDescriptionRequest::~CDescriptionRequest()
{
}

void CDescriptionRequest::FillBuffer(unsigned char* buffer, int max_length)
{
	CEIBNetPacket<EIBNETIP_DESCRIPTION_REQUEST>::FillBuffer(buffer,max_length);

	unsigned char* tmp_ptr = buffer + GetHeaderSize();
	memcpy(tmp_ptr,&_data,GetDataSize());

	EIBNETIP_DESCRIPTION_REQUEST* ptr = ((EIBNETIP_DESCRIPTION_REQUEST*)tmp_ptr);
	ptr->endpoint.port = htons(_data.endpoint.port);
}
