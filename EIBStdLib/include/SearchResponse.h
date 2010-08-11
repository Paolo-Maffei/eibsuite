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
				    unsigned char serial[],
				    unsigned char multicatAddr[],
				    unsigned char macAddr[],
				    unsigned char name[]);
	CSearchResponse(unsigned char* data);
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
