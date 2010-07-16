#ifndef __SEARCH_RESPONSE_HEADER__
#define __SEARCH_RESPONSE_HEADER__

#include "EIBNetIP.h"
#include "HPAI.h"
#include "EibNetPacket.h"

namespace EibStack
{

class EIB_STD_EXPORT CSearchResponse : public CEIBNetPacket<EIBNETIP_SEARCH_RESPONSE>
{
public:
	CSearchResponse(const CString& ctrl_addr, int ctrl_port);
	CSearchResponse(unsigned char* data);
	virtual ~CSearchResponse();

	const CString& GetControlIPAddress() const { return _control_ip; }
	unsigned short GetControlPort() const { return _control_port; }

	void FillBuffer(unsigned char* buffer, int max_length);

private:
	CString _control_ip;
	int _control_port;
};

}

#endif
