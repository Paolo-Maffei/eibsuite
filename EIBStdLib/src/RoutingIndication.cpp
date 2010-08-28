#include "RoutingIndication.h"

using namespace EibStack;


CRoutingIndication::CRoutingIndication(const CCemi_L_Data_Frame& cemi) :
CEIBNetPacket<EIBNETIP_ROUTING_INDICATION>(ROUTING_INDICATION),
_cemi_frame(cemi)
{
	_header.totalsize = HEADER_SIZE_10 + cemi.GetTotalSize();
}

CRoutingIndication::CRoutingIndication(unsigned char* buffer) :
CEIBNetPacket<EIBNETIP_ROUTING_INDICATION>(buffer),
_cemi_frame(buffer)
{
}

CRoutingIndication::~CRoutingIndication()
{
}

void CRoutingIndication::FillBuffer(unsigned char* buffer, int max_length)
{
	CEIBNetPacket<EIBNETIP_ROUTING_INDICATION>::FillBuffer(buffer,max_length);
	unsigned char* tmp_ptr = buffer + GetHeaderSize();
	_cemi_frame.FillBuffer(tmp_ptr,max_length - GetHeaderSize());
}
