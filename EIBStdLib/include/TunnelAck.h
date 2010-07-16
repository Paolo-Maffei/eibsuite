#ifndef __TUNNELING_ACK_HEADER__
#define __TUNNELING_ACK_HEADER__

#include "EIBNetIP.h"
#include "cEMI.h"
#include "CString.h"
#include "EibNetPacket.h"

namespace EibStack
{

class EIB_STD_EXPORT CTunnelingAck : public CEIBNetPacket<EIBNETIP_TUNNELING_ACK>
{
public:
	CTunnelingAck(unsigned char channelid, unsigned char sequencecounter, unsigned char status);
	CTunnelingAck(unsigned char* data);
	virtual ~CTunnelingAck();

	unsigned char GetChannelId()const { return _data.channelid; };
	unsigned char GetSequenceNumber() const { return _data.sequencecounter; }
	unsigned char GetStatus() const{ return _data.typespecific; }

	CString GetStatusString() const;

	void FillBuffer(unsigned char* buffer, int max_length);
};

}

#endif
