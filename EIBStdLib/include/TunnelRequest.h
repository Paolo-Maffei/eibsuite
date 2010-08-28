#ifndef __TUNNELING_REUQEST_HEADER__
#define __TUNNELING_REUQEST_HEADER__

#include "EIBNetIP.h"
#include "cEMI.h"
#include "EibNetPacket.h"
#include "CCemi_L_Data_Frame.h"

namespace EibStack
{

class EIB_STD_EXPORT CTunnelingRequest : public CEIBNetPacket<EIBNETIP_TUNNELING_REUQEST>
{
public:
	CTunnelingRequest(unsigned char channelid, unsigned char sequencecounter, const CCemi_L_Data_Frame& cemi);
	CTunnelingRequest(unsigned char* data);
	virtual ~CTunnelingRequest();

	unsigned char GetChannelId()const { return _data.header.channelid; };
	unsigned char GetSequenceNumber() const { return _data.header.sequencecounter; }
	const CCemi_L_Data_Frame& GetcEMI() const{ return _cemi_frame; }
	void FillBuffer(unsigned char* buffer, int max_length);

private:
	CCemi_L_Data_Frame _cemi_frame;
};

}

#endif
