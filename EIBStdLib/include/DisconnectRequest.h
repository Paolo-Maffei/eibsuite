#ifndef __EIB_DISCONNECT_REUQEST_HEADER__
#define __EIB_DISCONNECT_REUQEST_HEADER__

#include "EibStdLib.h"
#include "CException.h"
#include "EIBNetIP.h"
#include "CRI_CRD.h"
#include "HPAI.h"
#include "EibNetPacket.h"

namespace EibStack
{

class EIB_STD_EXPORT CDisconnectRequest : public CEIBNetPacket<EIBNETIP_DISCONNECT_REQUEST>
{
public:
	/**
     * This constructor parses a byte array. It is used for incoming requests.
     * Pass all bytes after the EIBnet/IP header to this constructor.
     * 
     * @param _Disconnect_Request
     *            The byte array starting after EIBnet/IP header
     * @throws EICLException
     *             forwards the IOException and HPAI EICLException
     * 
     * @see HPAI
     */
	CDisconnectRequest(unsigned char* data);
	/**
     * This constructor is used for outgoing requests. It creates a new Request
     * with the given parameters. Use this for outgoing requests.
     * 
     * @param _Channelid
     *            the channelid
     * @param _LocalPort
     *            the local client port
     * @throws EICLException
     *             forwards the EICLException thrown by the HPAI constructor
     * @see HPAI
     */
	CDisconnectRequest(unsigned char channelid, int port, const CString& ipaddress);
	/**
     * Destructor
     */
	virtual ~CDisconnectRequest();

	/**
     * Returns the channel ID of the connection being closed.
     * 
     * @return The connection channel ID.
     */
	unsigned  short GetChannelID() { return _data.channelid; }
	
	/**
     * Returns the reserved byte of this message (never used).
     * 
     * @return The reserved byte.
     */
    unsigned short GetReserved();

	void FillBuffer(unsigned char* buffer, int max_length);
};

}

#endif
