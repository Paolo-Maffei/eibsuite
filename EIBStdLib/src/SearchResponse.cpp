#include "SearchResponse.h"

using namespace EibStack;

CSearchResponse::CSearchResponse(const CString& ctrl_addr,
								 int ctrl_port,
								 KNXMedium knxMedium,
								 const CEibAddress& devAddr,
								 short projInstallId,
								 const char serial[],
								 unsigned long multicatAddr,
								 const char macAddr[],
								 const char name[],
								 int suppServices) :
CEIBNetPacket<EIBNETIP_SEARCH_RESPONSE>(SEARCH_RESPONSE),
_desc(knxMedium, devAddr, projInstallId, serial, multicatAddr, macAddr, name, suppServices)
{
	_control_ip = ctrl_addr;
	_control_port = ctrl_port;
	//set the data of control endpoint
	CHPAI ctrl_ep(ctrl_port,ctrl_addr);
	memcpy(&_data.endpoint,ctrl_ep.ToByteArray(),ctrl_ep.GetDataSize());
}

CSearchResponse::CSearchResponse(unsigned char* data, int len):
CEIBNetPacket<EIBNETIP_SEARCH_RESPONSE>(data)
{
	CHPAI discovered(data);
	memcpy(&_data.endpoint,discovered.ToByteArray(),discovered.GetDataSize());

	_control_ip = discovered.GetAddress();
	_control_port = discovered.GetPort();

	if((unsigned)(_header.totalsize - _header.headersize) > sizeof(EIBNETIP_HPAI)){
		_desc.Parse(data + _data.endpoint.structlength, len - _header.headersize - _data.endpoint.structlength);
	}
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

void CSearchResponse::Dump()
{
	printf("****************************************************************\n");
	_desc.Dump();
	printf("****************************************************************\n");
}
