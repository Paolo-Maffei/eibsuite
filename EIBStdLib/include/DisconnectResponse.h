#ifndef __EIB_DISCONNECT_RESPONSE_HEADER__
#define __EIB_DISCONNECT_RESPONSE_HEADER__

#include "EibStdLib.h"
#include "CException.h"
#include "EIBNetIP.h"
#include "CRI_CRD.h"
#include "HPAI.h"
#include "EibNetPacket.h"

namespace EibStack
{

class EIB_STD_EXPORT CDisconnectResponse : public CEIBNetPacket<EIBNETIP_DISCONNECT_RESPONSE>
{
public:
	CDisconnectResponse(unsigned char* data);
	CDisconnectResponse(unsigned char channelid, unsigned char status);
	/**
     * Destructor
     */
	virtual ~CDisconnectResponse();

	/**
     * Returns the channel ID of the connection being closed.
     * 
     * @return The connection channel ID.
     */
	unsigned  short GetChannelID() const { return _data.channelid; }
	/**
     * Get the messages errorcode / status.
     * 
     * @return the statusbyte
     */
	unsigned char GetStatus() const { return _data.status; }
	/**
     * Get a human readable representation of the statusbyte.
     * 
     * @return The status in a string representation.
     */
	CString GetStatusString() const;

	void FillBuffer(unsigned char* buffer, int max_length);
};

}

#endif

