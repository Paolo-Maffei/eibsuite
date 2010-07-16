#ifndef __ROUTING_INDICATION_HEADER__
#define __ROUTING_INDICATION_HEADER__

#include "EibNetPacket.h"
#include "CemiFrame.h"

namespace EibStack
{

class EIB_STD_EXPORT CRoutingIndication : public CEIBNetPacket<EIBNETIP_ROUTING_INDICATION>
{
public:
	CRoutingIndication(const CCemiFrame& cemi);
	CRoutingIndication(unsigned char* buffer);
	virtual ~CRoutingIndication();

	virtual void FillBuffer(unsigned char* buffer, int max_length);
	const CCemiFrame& GetCemiFrame() const { return _cemi_frame; }

private:
	CCemiFrame _cemi_frame;
};

}

#endif
