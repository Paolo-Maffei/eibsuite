#ifndef __EIB_CONNECTION_STATE_RESPONSE_HEADER__
#define __EIB_CONNECTION_STATE_RESPONSE_HEADER__

#include "EibStdLib.h"
#include "CException.h"
#include "EIBNetIP.h"
#include "CRI_CRD.h"
#include "HPAI.h"
#include "EibNetPacket.h"

namespace EibStack
{

class EIB_STD_EXPORT CConnectionStateResponse : public CEIBNetPacket<EIBNETIP_CONNECTIONSTATE_RESPONSE>
{
public:
	/**
     * Creates a new Connectionstate Response by parsing a byte array. Pass all
     * bytes after the EIBnet/IP header to this constructor.
     * 
     * @param _Buffer
     *            The byte array starting after the EIBnet/IP header.
     */
	CConnectionStateResponse(unsigned char* data);
	
	CConnectionStateResponse(byte channelid, byte status);

	/**
	Destructor
	*/
	virtual ~CConnectionStateResponse();

	/**
     * Returns the encapsulated channel identifier
     * 
     * @return The connection channel identifier.
     */
	unsigned short GetChannelID() const;
	
	/**
     * Get the messages errorcode / status.
     * 
     * @return the statusbyte
     */
	unsigned char GetStatus() const;

	/**
     * Get a human readable representation of the statusbyte.
     * 
     * @return The status in a string representation.
     */
	CString GetStatusString() const;

	virtual void FillBuffer(unsigned char* buffer, int max_len);
};

}

#endif

