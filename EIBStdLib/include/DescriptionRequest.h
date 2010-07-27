#ifndef __DESCRIPTION_REQUEST_H__
#define __DESCRIPTION_REQUEST_H__

#include "EibStdLib.h"
#include "CException.h"
#include "EIBNetIP.h"
#include "EibNetPacket.h"
#include "HPAI.h"

namespace EibStack
{

class EIB_STD_EXPORT CDescriptionRequest : public CEIBNetPacket<EIBNETIP_DESCRIPTION_REQUEST>
{
public:
	CDescriptionRequest(const CString& ipaddr, int port);
	CDescriptionRequest(unsigned char* data);

	virtual ~CDescriptionRequest();

	const CString& GetAddress() const { return _ipaddr; }
	int GetPort() const { return _port; }

	void FillBuffer(unsigned char* buffer, int max_length);

private:
	CString _ipaddr;
	int _port;
};

}

#endif
