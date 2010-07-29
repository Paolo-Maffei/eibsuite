#include "DescriptionResponse.h"

using namespace EibStack;

CDescriptionResponse::CDescriptionResponse():
CEIBNetPacket<EIBNETIP_DESCRIPTION_RESPONSE>(DESCRIPTION_RESPONSE)
{
	// This constructor is used since the Description response can be constructor in 2 ways:
	// 1. By itself as stand alone response the description request
	// 2. as part of the search response

	//Note: this version of the constructor is used in option 2 !!!
}

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
	ParseDevInfoDIB(ptr);
	ptr += _data.devicehardware.structlength;
	ParseSuppFamiliesDIB(ptr);
	ptr += _data.supported.structlength;
	ParseManufacturerDIB(ptr);
}

void CDescriptionResponse::ParseDevInfoDIB(unsigned char* data)
{
	ASSERT_ERROR(data[0] != sizeof(EIBNETIP_DEVINF_DIB), "Wrong size in DIB (device info).");

	_data.devicehardware.structlength = data[0];
	_data.devicehardware.descriptiontypecode = data[1];
	_data.devicehardware.knxmedium = data[2];
	_data.devicehardware.devicestatus = data[3];

	unsigned char* ptr = &data[4];

	memcpy(&_data.devicehardware.eibaddress, ptr, sizeof(_data.devicehardware.eibaddress));
	ptr += sizeof(_data.devicehardware.eibaddress);

	memcpy(&_data.devicehardware.projectinstallationidentifier, ptr, sizeof(_data.devicehardware.projectinstallationidentifier));
	ptr += sizeof(_data.devicehardware.projectinstallationidentifier);

	memcpy(&_data.devicehardware.serialnumber, ptr, sizeof(_data.devicehardware.serialnumber));
	ptr += sizeof(_data.devicehardware.serialnumber);

	memcpy(&_data.devicehardware.multicastaddress, ptr, sizeof(_data.devicehardware.multicastaddress));
	ptr += sizeof(_data.devicehardware.multicastaddress);

	memcpy(&_data.devicehardware.macaddress, ptr, sizeof(_data.devicehardware.macaddress));
	ptr += sizeof(_data.devicehardware.macaddress);

	memcpy(&_data.devicehardware.name, ptr, sizeof(_data.devicehardware.name));
}

void CDescriptionResponse::ParseSuppFamiliesDIB(unsigned char* data)
{
	_data.supported.structlength = data[0];
	_data.supported.descriptiontypecode = data[1];
	_data.supported.data = new unsigned char[data[0] - 2];
	memcpy(&_data.supported.data, &data[2], data[0] - 2);
}

void CDescriptionResponse::ParseManufacturerDIB(unsigned char* data)
{
	//YGYG - Manufacturer DIB is optional and there's no documentation available - leave it out
	_data.manufacturer = NULL;
}
