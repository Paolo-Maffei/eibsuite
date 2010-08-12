#ifndef __SEARCH_RESPONSE_HEADER__
#define __SEARCH_RESPONSE_HEADER__

#include "EIBNetIP.h"
#include "HPAI.h"
#include "EibNetPacket.h"
#include "DescriptionResponse.h"

namespace EibStack
{

class EIB_STD_EXPORT CSearchResponse : public CEIBNetPacket<EIBNETIP_SEARCH_RESPONSE>
{
public:
	CSearchResponse(const CString& ctrl_addr,
				    int ctrl_port,
				    KNXMedium knxMedium,
				    const CEibAddress& devAddr,
				    short projInstallId,
				    const char serial[],
				    const char multicatAddr[],
				    const char macAddr[],
				    const char name[],
				    int suppServices);
	CSearchResponse(unsigned char* data, int len);
	virtual ~CSearchResponse();

	const CString& GetControlIPAddress() const { return _control_ip; }
	unsigned short GetControlPort() const { return _control_port; }

	void FillBuffer(unsigned char* buffer, int max_length);

	void Dump();

private:
	CString _control_ip;
	int _control_port;
	CDescriptionResponse _desc;
};

}

#endif
