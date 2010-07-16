#ifndef __SEARCH_REQUEST_HEADER__
#define __SEARCH_REQUEST_HEADER__

#include "EIBNetIP.h"
#include "HPAI.h"
#include "EibNetPacket.h"

namespace EibStack
{

class EIB_STD_EXPORT CSearchRequest : public CEIBNetPacket<EIBNETIP_SEARCH_REQUEST>
{
public:
	CSearchRequest(unsigned char* data);
	CSearchRequest(int port, const CString& ipaddress);
	virtual ~CSearchRequest();

	const CString& GetRemoteIPAddress() const { return _remote_ip; }
	unsigned short GetRemotePort() const { return _remote_port; }

	virtual void FillBuffer(unsigned char* buffer, int max_length);

private:
	CString _remote_ip;
	int _remote_port;
};

}
#endif
