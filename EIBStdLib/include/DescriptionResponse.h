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
	MEDIUM_RF = 0x10,

	MEDIUM_UNKNOWN = 0xFF
};

enum SupportedServices
{
	SERVICE_CORE = 		0x01,
	SERVICE_DEV_MNGMT = 0x02,
	SERVICE_TUNNELING = 0x04,
	SERVICE_ROUTING = 	0x08,
	SERVICE_REMLOG = 	0x10,
	SERVICE_REMCONF =	0x20,
	SERVICE_OBJSRV = 	0x40
};

class EIB_STD_EXPORT CDescriptionResponse : public CEIBNetPacket<EIBNETIP_DESCRIPTION_RESPONSE>
{
public:
	CDescriptionResponse();
	CDescriptionResponse(KNXMedium knxMedium,
						 const CEibAddress& devAddr,
						 short projInstallId,
						 const char serial[],
						 unsigned long multicatAddr,
						 const char macAddr[],
						 const char name[],
						 int suppServices
						 );
	CDescriptionResponse(unsigned char* data, int len);

	virtual ~CDescriptionResponse();

	void Parse(unsigned char* data, int len);
	void FillBuffer(unsigned char* buffer, int max_length);

	static void KNXMediumToString(KNXMedium medium, CString& str);
	static void SupportedServicesToString(int mask, CString& str);

	void Dump();

	short GetProjectInstallID() const { return _proj_install_id; }
	int GetSupportedServicesMask() const {return _supported_services; }
	int GetStatus() const { return _status; }
	KNXMedium GetMedium() const { return _medium; }
	const CString& GetName() const { return _name; }
	const CString& GetMulticastAddress() const { return _mcast_address; }
	const CString& GetMACAddress() const {return _mac_address; }
	const CString& GetSerialNum() const { return _serial_num; }
	const CEibAddress& GetAddress() const { return _knx_address; }

private:
	void ParseDevInfoDIB(unsigned char* data);
	void ParseSuppFamiliesDIB(unsigned char* data);
	void ParseManufacturerDIB(unsigned char* data);

private:
	short _proj_install_id;
	int _supported_services;
	int _status;
	KNXMedium _medium;
	CString _name;
	CString _mac_address;
	CString _mcast_address;
	CString _serial_num;
	CEibAddress _knx_address;
};

}

#endif
