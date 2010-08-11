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

enum KNXMedium
{
	/* KNX medium code for twisted pair 0 (2400 bit/s), inherited from BatiBUS.	*/
	MEDIUM_TP0 = 0x01,
	/*KNX medium code for twisted pair 1 (9600 bit/s). */
	MEDIUM_TP1 = 0x02,
	/* KNX medium code for power line 110 kHz (1200 bit/s).	*/
	MEDIUM_PL110 = 0x04,
	/* KNX medium code for power line 132 kHz (2400 bit/s), inherited from EHS.	*/
	MEDIUM_PL132 = 0x08,
	/* KNX medium code for radio frequency (868 MHz). */
	MEDIUM_RF = 0x10
};

class EIB_STD_EXPORT CDescriptionResponse : public CEIBNetPacket<EIBNETIP_DESCRIPTION_RESPONSE>
{
public:
	CDescriptionResponse();
	CDescriptionResponse(KNXMedium knxMedium,
						 const CEibAddress& devAddr,
						 short projInstallId,
						 const char serial[],
						 const char multicatAddr[],
						 const char macAddr[],
						 const char name[]
						 );
	CDescriptionResponse(unsigned char* data);

	virtual ~CDescriptionResponse();

	void Parse(unsigned char* data);
	void FillBuffer(unsigned char* buffer, int max_length);

	void Dump();

private:
	void ParseDevInfoDIB(unsigned char* data);
	void ParseSuppFamiliesDIB(unsigned char* data);
	void ParseManufacturerDIB(unsigned char* data);
};

}

#endif
