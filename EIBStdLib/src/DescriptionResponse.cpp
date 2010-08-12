#include "DescriptionResponse.h"

using namespace EibStack;

CDescriptionResponse::CDescriptionResponse():
CEIBNetPacket<EIBNETIP_DESCRIPTION_RESPONSE>(DESCRIPTION_RESPONSE)
{
	// This constructor is used since the Description response can be constructed in 2 ways:
	// 1. By itself as a stand alone response the description request
	// 2. As part of the search response
	//Note: this version of the constructor is used ONLY in option 2 !!!
	_data.manufacturer = NULL;
	_data.supported.data = NULL;
}

CDescriptionResponse::CDescriptionResponse(
		 KNXMedium knxMedium,
		 const CEibAddress& devAddr,
		 short projInstallId,
		 const char serial[],
		 const char multicatAddr[],
		 const char macAddr[],
		 const char name[],
		 int suppServices
		 ):
CEIBNetPacket<EIBNETIP_DESCRIPTION_RESPONSE>(DESCRIPTION_RESPONSE)
{
	EIBNETIP_DEVINF_DIB* devinfodib = &_data.devicehardware;

	devinfodib->structlength = sizeof(EIBNETIP_DEVINF_DIB);

	devinfodib->descriptiontypecode                = DEVICE_INFO;

	devinfodib->knxmedium                          = (unsigned char)knxMedium;

	devinfodib->devicestatus                       = 0x01;                                 // program mode

	devinfodib->eibaddress                         = htons( devAddr.ToByteArray() );

	devinfodib->projectinstallationidentifier      = projInstallId;

	devinfodib->serialnumber[0]                    = serial[0];
	devinfodib->serialnumber[1]                    = serial[1];
	devinfodib->serialnumber[2]                    = serial[2];
	devinfodib->serialnumber[3]                    = serial[3];
	devinfodib->serialnumber[4]                    = serial[4];
	devinfodib->serialnumber[5]                    = serial[5];

	int mcast = htonl( inet_addr("224.0.23.12") );
	memcpy( devinfodib->multicastaddress, &mcast, sizeof(mcast));

	memcpy( devinfodib->macaddress, macAddr, 6 );

	memset( devinfodib->name, '\0', 30 );
	snprintf( (char *)devinfodib->name, 30, "%s", name);

	EIBNETIP_SUPPFAM_DIB* supporteddib = &_data.supported;

	//fast algorithm to count the number of set bits in an integer.
	suppServices = suppServices - ((suppServices >> 1) & 0x55555555);
	suppServices = (suppServices & 0x33333333) + ((suppServices >> 2) & 0x33333333);
	int sum = ((suppServices + ((suppServices >> 4) & 0xF0F0F0F)) * 0x1010101) >> 24;

	if(sum > 0){
		supporteddib->structlength = (sum * 2) + 2;
		supporteddib->data = new unsigned char[(sum * 2)];
	}else{
		supporteddib->structlength = 2;
		supporteddib->data = NULL;
	}
	supporteddib->descriptiontypecode = SUPP_SVC_FAMILIES;
	sum = 0;
	if(suppServices & SERVICE_CORE){
		supporteddib->data[sum++] = EIBNETIP_CORE;
		supporteddib->data[sum++] = 0x01;
	}
	if(suppServices & SERVICE_DEV_MNGMT){
		supporteddib->data[sum++] = EIBNETIP_DEVMGMT;
		supporteddib->data[sum++] = 0x01;
	}
	if(suppServices & SERVICE_TUNNELING){
		supporteddib->data[sum++] = EIBNETIP_TUNNELING;
		supporteddib->data[sum++] = 0x01;
	}
	if(suppServices & SERVICE_ROUTING){
		supporteddib->data[sum++] = EIBNETIP_ROUTING;
		supporteddib->data[sum++] = 0x01;
	}

	_data.manufacturer = NULL;
}

CDescriptionResponse::CDescriptionResponse(unsigned char* data, int len) :
CEIBNetPacket<EIBNETIP_DESCRIPTION_RESPONSE>(data)
{
	_data.manufacturer = NULL;
	_data.supported.data = NULL;
	Parse(data, len - _header.headersize);
}

CDescriptionResponse::~CDescriptionResponse()
{
	if(_data.manufacturer != NULL){
		delete _data.manufacturer;
	}
	if(_data.supported.data != NULL){
		delete _data.supported.data;
	}
}

void CDescriptionResponse::FillBuffer(unsigned char* buffer, int max_length)
{
	CEIBNetPacket<EIBNETIP_DESCRIPTION_RESPONSE>::FillBuffer(buffer,max_length);
}

void CDescriptionResponse::Parse(unsigned char* data, int len)
{
	unsigned char* ptr = data;
	ParseDevInfoDIB(ptr);
	ptr += _data.devicehardware.structlength;
	len -= _data.devicehardware.structlength;
	ParseSuppFamiliesDIB(ptr);
	ptr += _data.supported.structlength;
	len -= _data.supported.structlength;
	if(len > 0){
		ParseManufacturerDIB(ptr);
	}
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
	if(data[0] > 2){
		_data.supported.data = new unsigned char[data[0] - 2];
		memcpy(_data.supported.data, &data[2], data[0] - 2);
	}
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
	printf("Status: %s\n", _data.devicehardware.devicestatus == 0 ? "OK" : "Unknown");
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
	//supported services
	if(_data.supported.structlength > 2){
		printf("Supported services: ");
		for(i = 0; i < _data.supported.structlength - 2; i += 2){
			switch(_data.supported.data[i])
			{
			case EIBNETIP_CORE: printf("Core");
				break;
			case EIBNETIP_DEVMGMT: printf("Device management");
				break;
			case EIBNETIP_TUNNELING: printf("Tunneling");
				break;
			case EIBNETIP_ROUTING: printf("Routing");
				break;
			case EIBNETIP_REMLOG: printf("Remote log");
				break;
			case EIBNETIP_REMCONF: printf("Remote configuration");
				break;
			case EIBNETIP_OBJSRV: printf("Object server");
				break;
			}

			if(i < _data.supported.structlength - 4){
				printf(", ");
			}
		}
		printf(".\n");
	}
}
