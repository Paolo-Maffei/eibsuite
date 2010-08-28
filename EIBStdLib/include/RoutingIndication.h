#ifndef __ROUTING_INDICATION_HEADER__
#define __ROUTING_INDICATION_HEADER__

#include "EibNetPacket.h"
#include "CCemi_L_Data_Frame.h"

namespace EibStack
{

class EIB_STD_EXPORT CRoutingIndication : public CEIBNetPacket<EIBNETIP_ROUTING_INDICATION>
{
public:
	CRoutingIndication(const CCemi_L_Data_Frame& cemi);
	CRoutingIndication(unsigned char* buffer);
	virtual ~CRoutingIndication();

	virtual void FillBuffer(unsigned char* buffer, int max_length);
	const CCemi_L_Data_Frame& GetCemiFrame() const { return _cemi_frame; }

private:
	CCemi_L_Data_Frame _cemi_frame;
};

}

#endif
