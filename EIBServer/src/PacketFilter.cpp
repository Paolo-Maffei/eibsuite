#include "PacketFilter.h"

CPacketFilter::CPacketFilter() : _allowed_sa_mask(0xffff),_allowed_da_mask(0xffff)
{
}

CPacketFilter::~CPacketFilter()
{
}

/*
void CPacketFilter::AddAllowedSourceAddress(unsigned short source)
{
	AddAddress(_allowed_sa_filters,source,true);
}

void CPacketFilter::AddAllowedDestAddress(unsigned short dest)
{
	AddAddress(_allowed_da_filters,dest,true);
}

void CPacketFilter::AddAddress(map<unsigned short,bool>& filters, unsigned short address,bool allowed)
{
	map<unsigned short,bool>::iterator it;
	it = filters.find(address);
	if (it == filters.end()){
		filters.insert(pair<unsigned short,bool>(address,allowed));
	}
}
*/

bool CPacketFilter::IsPacketAllowed(const CCemi_L_Data_Frame& msg) const
{
	unsigned short saddr = msg.GetSourceAddress().ToByteArray();
	unsigned short daddr = msg.GetDestAddress().ToByteArray();
	
	//ALLOWED
	if((_allowed_sa_mask & saddr) == 0 && _allowed_sa_mask != 0xFFFF){
		//find source address rule
		//if (_allowed_sa_filters.count(saddr) == 0){
		//	return false;
		//}
		return false;
	}

	//check mask here
	if((_allowed_da_mask & daddr) == 0 && _allowed_da_mask != 0xFFFF){
		//find source address rule
		//if (_allowed_da_filters.count(daddr) == 0){
		//	return false;
		//}
		return false;
	}

	return true;
}

void CPacketFilter::ClearAllFilters()
{
	_allowed_sa_mask = 0;
	_allowed_da_mask = 0;
}
