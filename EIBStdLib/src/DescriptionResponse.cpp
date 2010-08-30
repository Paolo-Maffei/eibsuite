#include "DescriptionResponse.h"

using namespace EibStack;

CDescriptionResponse::CDescriptionResponse():
CEIBNetPacket<EIBNETIP_DESCRIPTION_RESPONSE>(DESCRIPTION_RESPONSE),
_proj_install_id(0),
_supported_services(0),
_status(-1),
_medium(MEDIUM_UNKNOWN)
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
		 unsigned long multicatAddr,
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

	memcpy( devinfodib->multicastaddress, &multicatAddr, 4);

	memcpy( devinfodib->macaddress, macAddr, 6 );

	memset( devinfodib->name, '\0', 30 );
	memcpy(devinfodib->name, name, strlen(name));

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

	//set the locals
	_medium = knxMedium;
	_proj_install_id = projInstallId;
	_status = 0; //OK
	_supported_services = suppServices;

	int i;
	for (i = 0; i < 4; i++){
		_mcast_address += CString(_data.devicehardware.multicastaddress[i]);
		if(i < 3) _mcast_address += '.';
	}
	for (i = 0; i < 6; i++){
		_mac_address += CString::ToHexFormat(_data.devicehardware.macaddress[i], false);
		if(i < 5) _mac_address += ':';
	}
	for (i = 0; i < 6; i++){
		_serial_num += CString::ToHexFormat(_data.devicehardware.serialnumber[i], false);
		if(i < 5) _serial_num += '-';
	}
	_knx_address = devAddr;
	_name.Assign(name);
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

	//set the locals
	_medium = (KNXMedium)_data.devicehardware.knxmedium;
	_proj_install_id = _data.devicehardware.projectinstallationidentifier;
	_status = _data.devicehardware.devicestatus;


	int i;
	for (i = 0; i < 4; i++){
		_mcast_address += CString(_data.devicehardware.multicastaddress[i]);
		if(i < 3) _mcast_address += '.';
	}
	for (i = 0; i < 6; i++){
		_mac_address += CString::ToHexFormat(_data.devicehardware.macaddress[i], false);
		if(i < 5) _mac_address += ':';
	}
	for (i = 0; i < 6; i++){
		_serial_num += CString::ToHexFormat(_data.devicehardware.serialnumber[i], false);
		if(i < 5) _serial_num += '-';
	}
	_knx_address.Set(_data.devicehardware.eibaddress, false);
	_name.Assign((char*)_data.devicehardware.name);
}

void CDescriptionResponse::ParseSuppFamiliesDIB(unsigned char* data)
{
	ASSERT_ERROR(data[1] == SUPP_SVC_FAMILIES, "Wrong device description code (supp families)");
	_data.supported.structlength = data[0];
	_data.supported.descriptiontypecode = data[1];
	if(data[0] > 2){
		_data.supported.data = new unsigned char[data[0] - 2];
		memcpy(_data.supported.data, &data[2], data[0] - 2);

		for(int i = 0; i < _data.supported.structlength - 2; i += 2){
			_supported_services |= 1 << (_data.supported.data[i] - 2);
		}
	}
}

void CDescriptionResponse::ParseManufacturerDIB(unsigned char* data)
{
	//YGYG - Manufacturer DIB is optional and there's no documentation available - leave it out
	_data.manufacturer = NULL;
}

void CDescriptionResponse::KNXMediumToString(KNXMedium medium, CString& str)
{
	switch(medium)
	{
	case MEDIUM_TP0: str = "twisted pair 0 (2400 bit/s)";
		break;
	case MEDIUM_TP1: str = "twisted pair 1 (9600 bit/s)";
		break;
	case MEDIUM_PL110: str = "power line 110 kHz (1200 bit/s)";
		break;
	case MEDIUM_PL132: str = "power line 132 kHz (2400 bit/s)";
		break;
	case MEDIUM_RF: str = "radio frequency (868 MHz)";
		break;
	default: str = "Unknown (";
		str += medium;
		str += ")";
		break;
	}
}

void CDescriptionResponse::SupportedServicesToString(int mask, CString& str)
{
	str.Clear();
	if((mask & SERVICE_CORE) != 0){
		str += "Core";
	}
	if((mask & SERVICE_DEV_MNGMT) != 0){
		str += " Device-management";
	}
	if((mask & SERVICE_TUNNELING) != 0){
		str += " Tunneling";
	}
	if((mask & SERVICE_ROUTING) != 0){
		str += " Routing";
	}
	if((mask & SERVICE_REMLOG) != 0){
		str += " Remote-Log";
	}
	if((mask & SERVICE_REMCONF) != 0){
		str += " Remote-Configuration";
	}
	if((mask & SERVICE_OBJSRV) != 0){
		str += " Object-Server";
	}
}

void CDescriptionResponse::Dump()
{
	CString str;
	KNXMediumToString(_medium ,str);
	//Medium
	printf("Medium: %s\n", str.GetBuffer());
	//Status
	printf("Status: %s\n", _status == 0 ? "OK" : "Unknown");
	//EIB Address
	printf("Physical Address: %s\n", _knx_address.ToString().GetBuffer());
	//Project installation ID
	printf("Project installation ID: %d\n", _proj_install_id);
	//Serial number
	printf("Serial number: %s\n", _serial_num.GetBuffer());
	printf("Multicast Address: %s\n", _mcast_address.GetBuffer());
	//MAC address
	printf("MAC Address: %s\n", _mac_address.GetBuffer());
	//Name
	if(_name.GetLength() > 0){
		printf("Name: %s\n", _name.GetBuffer());
	}
	//supported services
	if(_supported_services != 0){
		SupportedServicesToString(_supported_services, str);
		printf("Supported services: %s.\n", str.GetBuffer());
	}
}
