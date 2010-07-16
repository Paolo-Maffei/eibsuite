#ifndef __EIB_CONNECT_RESPONSE_HEADER__
#define __EIB_CONNECT_RESPONSE_HEADER__

#include "EibStdLib.h"
#include "EIBNetIP.h"
#include "HPAI.h"
#include "CRI_CRD.h"
#include "CString.h"
#include "EibNetPacket.h"
#include "ConnectRequest.h"

namespace EibStack
{

class EIB_STD_EXPORT CConnectResponse : public CEIBNetPacket<EIBNETIP_CONNECT_RESPONSE>
{
public:
	CConnectResponse(int channel_id, int status,
				     const CString& ctrl_addr, int ctrl_port,
					 CConnectRequest::ConnectionType connection_type);
	
	CConnectResponse(unsigned char* data);
	
	virtual ~CConnectResponse();

	/**
     * Returns a human readable representation of the error codes.
     *
     * @return The error in a human readable form.
     */
	CString GetStatusString();
	/**
     * Extracts the message status byte. The status byte gives information about
     * the request success.
     *
     * @return Returns the status byte
     */
    unsigned char GetStatus() {return _data.status;}

	/**
     * Returns the connection channel ID (1 byte) if the connection was
     * established successfully.
     *
     * @return The connection channel identifier.
     */
	unsigned char GetChannelID() { return _data.channelid;}

	const CString& GetDataIPAddress() const { return _data_address; }
	unsigned short GetDataPort() const { return _data_port; }

	void FillBuffer(unsigned char* buffer, int max_len);

private:
	CString _data_address;
	unsigned short _data_port;
};

}

#endif
