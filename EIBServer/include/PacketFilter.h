#ifndef __PACKET_FILTER_HEADER__
#define __PACKET_FILTER_HEADER__

#include "Globals.h"
#include <map>
#include "EIBNetIP.h"
#include "CCemi_L_Data_Frame.h"

using namespace EibStack;

class CUsersDB;

class CPacketFilter
{
public:
	CPacketFilter();
	virtual ~CPacketFilter();

	void SetAllowedSourceAddressMask(unsigned short mask) { _allowed_sa_mask = mask;}
	void SetAllowedDestAddressMask(unsigned short mask) { _allowed_da_mask = mask;}
	bool IsPacketAllowed(const CCemi_L_Data_Frame& msg) const;

	//void AddAllowedSourceAddress(unsigned short source);
	//void AddAllowedDestAddress(unsigned short dest);

	unsigned short GetSrcMask() const { return _allowed_sa_mask; }
	unsigned short GetDstMask() const { return _allowed_da_mask; }

	void ClearAllFilters();

	friend class CUsersDB;

private:
	//void AddAddress(map<unsigned short,bool>& filters, unsigned short address, bool allowed);

private:
	//map<unsigned short,bool> _allowed_sa_filters;
	//map<unsigned short,bool> _allowed_da_filters;

	unsigned short _allowed_sa_mask;
	unsigned short _allowed_da_mask;
};

#endif
