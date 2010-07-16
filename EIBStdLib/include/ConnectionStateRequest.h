#ifndef __EIB_CONNECTION_STATE_REQUEST_HEADER__
#define __EIB_CONNECTION_STATE_REQUEST_HEADER__

#include "EibStdLib.h"
#include "CException.h"
#include "EIBNetIP.h"
#include "CRI_CRD.h"
#include "HPAI.h"
#include "EibNetPacket.h"

namespace EibStack
{

class EIB_STD_EXPORT CConnectionStateRequest : public CEIBNetPacket<EIBNETIP_CONNECTIONSTATE_REQUEST>
{
public:
	/**
     * Creates a new Connectionstate_Request message.
     * 
     * @param channelid
     *            The current connection channelid.
     * @param port
     *            The clients local data end point port.
     */
	CConnectionStateRequest(unsigned char channelid, int port, const CString& ipaddress);
	/**
	 * Creates a connection state request out of a byte array.
	 * 
	 * @param data byte array containing a connection state request structure
	 */
	CConnectionStateRequest(unsigned char* data);

	virtual ~CConnectionStateRequest();

	virtual void FillBuffer(unsigned char* buffer, int max_len);

	byte GetChannelId() const { return _data.channelid; }
};

}

#endif
