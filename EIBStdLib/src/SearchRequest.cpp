#include "SearchRequest.h"

using namespace EibStack;

CSearchRequest::CSearchRequest(unsigned char *data) :
CEIBNetPacket<EIBNETIP_SEARCH_REQUEST>(data)
{
	CHPAI discovery(data);
	memcpy(&_data.discoveryendpoint,discovery.ToByteArray(),discovery.GetDataSize());

	_remote_ip = discovery.GetAddress();
	_remote_port = discovery.GetPort();
}

CSearchRequest::CSearchRequest(int port, const CString& ipaddress):
CEIBNetPacket<EIBNETIP_SEARCH_REQUEST>(SEARCH_REQUEST)
{
	CHPAI hpai(port,ipaddress);
	memcpy(&_data,hpai.ToByteArray(),hpai.GetDataSize());
}

CSearchRequest::~CSearchRequest()
{
}

void CSearchRequest::FillBuffer(unsigned char* buffer, int max_length)
{
	CEIBNetPacket<EIBNETIP_SEARCH_REQUEST>::FillBuffer(buffer,max_length);
	unsigned char* tmp_ptr = buffer + GetHeaderSize();
	memcpy(tmp_ptr,&_data,GetDataSize());
	EIBNETIP_SEARCH_REQUEST* ptr = ((EIBNETIP_SEARCH_REQUEST*)tmp_ptr);
	ptr->discoveryendpoint.port = htons(_data.discoveryendpoint.port);
}
