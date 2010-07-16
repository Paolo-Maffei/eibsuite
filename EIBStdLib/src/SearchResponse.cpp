#include "SearchResponse.h"

using namespace EibStack;

CSearchResponse::CSearchResponse(const CString& ctrl_addr, int ctrl_port) :
CEIBNetPacket<EIBNETIP_SEARCH_RESPONSE>(SEARCH_RESPONSE)
{
	_control_ip = ctrl_addr;
	_control_port = ctrl_port;
	//set the data of control endpoint
	CHPAI ctrl_ep(ctrl_port,ctrl_addr);
	memcpy(&_data.endpoint,ctrl_ep.ToByteArray(),ctrl_ep.GetDataSize());
	//add the other field here...
	//DIB, Supported families, manufactor details
}

CSearchResponse::CSearchResponse(unsigned char* data):
CEIBNetPacket<EIBNETIP_SEARCH_RESPONSE>(data)
{
	CHPAI discovered(data);
	memcpy(&_data.endpoint,discovered.ToByteArray(),discovered.GetDataSize());

	_control_ip = discovered.GetAddress();
	_control_port = discovered.GetPort();
}

CSearchResponse::~CSearchResponse()
{
}

void CSearchResponse::FillBuffer(unsigned char* buffer, int max_length)
{
	CEIBNetPacket<EIBNETIP_SEARCH_RESPONSE>::FillBuffer(buffer,max_length);
	unsigned char* tmp_ptr = buffer + GetHeaderSize();
	memcpy(tmp_ptr,&_data,GetDataSize());
	EIBNETIP_SEARCH_REQUEST* ptr = ((EIBNETIP_SEARCH_REQUEST*)tmp_ptr);
	ptr->discoveryendpoint.port = htons(_data.endpoint.port);
}

