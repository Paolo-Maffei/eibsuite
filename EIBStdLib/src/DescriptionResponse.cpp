#include "DescriptionResponse.h"

using namespace EibStack;

CDescriptionResponse::CDescriptionResponse():
CEIBNetPacket<EIBNETIP_DESCRIPTION_RESPONSE>(DESCRIPTION_RESPONSE)
{
	// This constructor is used since the Description response can be constructed in 2 ways:
	// 1. By itself as a stand alone response the description request
	// 2. As part of the search response
	//Note: this version of the constructor is used ONLY in option 2 !!!
}

CDescriptionResponse::CDescriptionResponse(
		 KNXMedium knxMedium,
		 const CEibAddress& devAddr,
		 short projInstallId,
		 const char serial[],
		 const char multicatAddr[],
		 const char macAddr[],
		 const char name[]
		 ):
CEIBNetPacket<EIBNETIP_DESCRIPTION_RESPONSE>(DESCRIPTION_RESPONSE)
{
	EIBNETIP_DEVINF_DIB* dib = &_data.devicehardware;

	dib->structlength = sizeof(EIBNETIP_DEVINF_DIB);

	dib->descriptiontypecode                = DEVICE_INFO;

	dib->knxmedium                          = (unsigned char)knxMedium;

	dib->devicestatus                       = 0x01;                                 // program mode

	dib->eibaddress                         = htons( devAddr.ToByteArray() );

	dib->projectinstallationidentifier      = projInstallId;

	dib->serialnumber[0]                    = serial[0];
	dib->serialnumber[1]                    = serial[1];
	dib->serialnumber[2]                    = serial[2];
	dib->serialnumber[3]                    = serial[3];
	dib->serialnumber[4]                    = serial[4];
	dib->serialnumber[5]                    = serial[5];

	int mcast = htonl( inet_addr("224.0.23.12") );
	memcpy( dib->multicastaddress, &mcast, sizeof(mcast));

	memcpy( dib->macaddress, macAddr, 6 );

	memset( dib->name, '\0', 30 );
	snprintf( (char *)dib->name, 30, "%s", name);
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
	ASSERT_ERROR(data[0] == sizeof(EIBNETIP_DEVINF_DIB), "Wrong size in DIB (device info).");
	ASSERT_ERROR(data[1] == DEVICE_INFO, "Wrong device description code");

	_data.devicehardware.structlength = data[0];
	_data.devicehardware.descriptiontypecode = data[1];
	_data.devicehardware.knxmedium = data[2];
	_data.devicehardware.devicestatus = data[3];

	unsigned char* ptr = &data[4];

	memcpy(&_data.devicehardware.eibaddress, ptr, sizeof(_data.devicehardware.eibaddress));
	ptr += sizeof(_data.devicehardware.eibaddress);

	memcpy(&_data.devicehardware.projectinstallationidentifier, ptr, sizeof(_data.devicehardware.projectinstallationidentifier));
	ptr += sizeof(_data.devicehardware.projectinstallationidentifier);

	memcpy(_data.devicehardware.serialnumber, ptr, sizeof(_data.devicehardware.serialnumber));
	ptr += sizeof(_data.devicehardware.serialnumber);

	memcpy(_data.devicehardware.multicastaddress, ptr, sizeof(_data.devicehardware.multicastaddress));
	ptr += sizeof(_data.devicehardware.multicastaddress);

	memcpy(_data.devicehardware.macaddress, ptr, sizeof(_data.devicehardware.macaddress));
	ptr += sizeof(_data.devicehardware.macaddress);

	memcpy(_data.devicehardware.name, ptr, sizeof(_data.devicehardware.name));
}

void CDescriptionResponse::ParseSuppFamiliesDIB(unsigned char* data)
{
	ASSERT_ERROR(data[1] == SUPP_SVC_FAMILIES, "Wrong device description code (supp families)");

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

void CDescriptionResponse::Dump()
{
	//Medium
	printf("Medium: ");
	switch((KNXMedium)_data.devicehardware.knxmedium)
	{
	case MEDIUM_TP0: printf("twisted pair 0 (2400 bit/s)\n");
		break;
	case MEDIUM_TP1: printf("twisted pair 1 (9600 bit/s)\n");
		break;
	case MEDIUM_PL110: printf("power line 110 kHz (1200 bit/s)\n");
		break;
	case MEDIUM_PL132: printf("power line 132 kHz (2400 bit/s)\n");
		break;
	case MEDIUM_RF: printf("radio frequency (868 MHz)\n");
		break;
	default: printf("Unknown (%u)\n", _data.devicehardware.knxmedium);
		break;
	}
	//Status
	printf("Status: %u\n", _data.devicehardware.devicestatus);
	//EIB Address
	CEibAddress addr(_data.devicehardware.eibaddress, false);
	printf("Physical Address: %s\n", addr.ToString().GetBuffer());
	//Project installation ID
	printf("Project installation ID: %d\n", _data.devicehardware.projectinstallationidentifier);
	//Serial number
	printf("Serial number: ");
	int i;
	for(i = 0; i < 6; i++)
		printf("%x", _data.devicehardware.serialnumber[i]);
	printf("\n");
	//ip address
	unsigned char* ip = _data.devicehardware.multicastaddress;
	printf("Multicast Address: %u.%u.%u.%u\n", ip[0], ip[1], ip[2], ip[3]);
	//MAC address
	printf("MAC Address: ");
	for(i = 0; i < 6; i++){
		printf("%x", _data.devicehardware.macaddress[i]);
		if(i != 5) printf(":");
	}
	printf("\n");
	//Name
	if(_data.devicehardware.name[0] != '\0'){
		printf("Name: %s\n", _data.devicehardware.name);
	}
}
