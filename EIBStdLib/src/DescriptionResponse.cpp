#include "DescriptionResponse.h"

using namespace EibStack;

CDescriptionResponse::CDescriptionResponse(unsigned char* data) :
CEIBNetPacket<EIBNETIP_DESCRIPTION_RESPONSE>(data)
{
	Parse(data);
}

CDescriptionResponse::~CDescriptionResponse()
{
}

void CDescriptionResponse::FillBuffer(unsigned char* buffer, int max_length)
{
	CEIBNetPacket<EIBNETIP_DESCRIPTION_RESPONSE>::FillBuffer(buffer,max_length);
}

void CDescriptionResponse::Parse(unsigned char* data)
{
	unsigned char* ptr = data;

	ASSERT_ERROR(ptr[0] != sizeof(EIBNETIP_DEVINF_DIB), "Wrong size in DIB (device info).");

	_data.devicehardware.structlength = *(ptr++);
	_data.devicehardware.descriptiontypecode = *(ptr++);
	_data.devicehardware.knxmedium = *(ptr++);
	_data.devicehardware.devicestatus = *(ptr++);

	word eibaddress;
	word projectinstallationidentifier;

	memcpy(_data.devicehardware.serialnumber, ptr, sizeof(_data.devicehardware.serialnumber));
	ptr += sizeof(_data.devicehardware.serialnumber);
	memcpy(_data.devicehardware.multicastaddress, ptr, sizeof(_data.devicehardware.multicastaddress));
	ptr += sizeof(_data.devicehardware.multicastaddress);
	memcpy(_data.devicehardware.macaddress, ptr, sizeof(_data.devicehardware.macaddress));
	ptr += sizeof(_data.devicehardware.macaddress);
	memcpy(_data.devicehardware.name, ptr, sizeof(_data.devicehardware.name));
}
