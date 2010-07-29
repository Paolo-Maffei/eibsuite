#ifndef __DESCRIPTION_RESPONSE_H__
#define __DESCRIPTION_RESPONSE_H__

#include "EibStdLib.h"
#include "CException.h"
#include "EIBNetIP.h"
#include "EibNetPacket.h"
#include "HPAI.h"
#include "EIBAddress.h"

namespace EibStack
{

class EIB_STD_EXPORT CDescriptionResponse : public CEIBNetPacket<EIBNETIP_DESCRIPTION_RESPONSE>
{
public:
	CDescriptionResponse();
	CDescriptionResponse(unsigned char knxMedium,
						 const CEibAddress& devAddr,
						 short projInstallId,
						 unsigned char serial[],
						 unsigned char multicatAddr[],
						 unsigned char macAddr[],
						 unsigned char name[]
						 );
	CDescriptionResponse(unsigned char* data);

	virtual ~CDescriptionResponse();

	void Parse(unsigned char* data);
	void FillBuffer(unsigned char* buffer, int max_length);

private:
	void ParseDevInfoDIB(unsigned char* data);
	void ParseSuppFamiliesDIB(unsigned char* data);
	void ParseManufacturerDIB(unsigned char* data);
};

}

#endif
