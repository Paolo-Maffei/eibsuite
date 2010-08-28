#include "TunnelRequest.h"

using namespace EibStack;

CTunnelingRequest::CTunnelingRequest(unsigned char channelid, unsigned char sequencecounter, const CCemi_L_Data_Frame &cemi):
CEIBNetPacket<EIBNETIP_TUNNELING_REUQEST>(TUNNELLING_REQUEST),
_cemi_frame(cemi)
{
	_header.totalsize = HEADER_SIZE_10 + sizeof(EIBNETIP_COMMON_CONNECTION_HEADER) + cemi.GetTotalSize();

	_data.header.channelid = channelid;
	_data.header.sequencecounter = sequencecounter;
	_data.header.typespecific = 0; //YGYG - this byte is reserved
	_data.header.structlength = 4;
}

CTunnelingRequest::CTunnelingRequest(unsigned char *data):
CEIBNetPacket<EIBNETIP_TUNNELING_REUQEST>(data),
_cemi_frame(&data[4])
{
	_data.header.structlength = data[0];
	_data.header.channelid = data[1];
	_data.header.sequencecounter = data[2];
	_data.header.typespecific = data[3]; //YGYG - this byte is reserved and should be 0
}

CTunnelingRequest::~CTunnelingRequest()
{
}


void CTunnelingRequest::FillBuffer(unsigned char* buffer, int max_length)
{
	//sanity
	ASSERT_ERROR(max_length >= this->GetTotalSize(), "Buffer size is too small");
	//fill header
	CEIBNetPacket<EIBNETIP_TUNNELING_REUQEST>::FillBuffer(buffer,max_length);

	unsigned char* tmp_ptr = buffer + GetHeaderSize();
	//fill the common connection header
	memcpy(tmp_ptr,&_data.header,_data.header.structlength);
	//fill the cemi frame itself
	tmp_ptr += _data.header.structlength;
	_cemi_frame.FillBuffer(tmp_ptr,max_length - _data.header.structlength - GetHeaderSize());
}
