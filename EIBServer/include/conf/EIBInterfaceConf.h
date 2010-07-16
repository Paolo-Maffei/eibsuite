#ifndef __SERVER_STATUS_CONF_HEADER__
#define __SERVER_STATUS_CONF_HEADER__

#include "IConfBase.h"
#include "EIBInterface.h"
#include "CTime.h"

//defs
#define EIB_INTERFACE_ADDRESS_XML "EIB_INTERFACE_ADDRESS"
#define EIB_INTERFACE_PORT_XML "EIB_INTERFACE_PORT"
#define EIB_INTERFACE_DEVICE_MODE_XML "EIB_INTERFACE_MODE"
#define EIB_INTERFACE_AUTO_DETECT_XML "EIB_INTERFACE_AUTO_DETECT"
#define EIB_INTERFACE_RUNNING_STATUS_XML "EIB_INTERFACE_RUNNING_STATUS"
#define EIB_INTERFACE_LAST_TIME_PACKET_SENT_XML "EIB_INTERFACE_LAST_TIME_PACKET_SENT"
#define EIB_INTERFACE_LAST_TIME_PACKET_RECEIVED_XML "EIB_INTERFACE_LAST_TIME_PACKET_RECEIVED"
#define EIB_INTERFACE_TOTAL_PACKETS_SENT_XML "EIB_INTERFACE_TOTAL_PACKETS_SENT"
#define EIB_INTERFACE_TOTAL_PACKETS_RECEIVED_XML "EIB_INTERFACE_TOTAL_PACKETS_RECEIVED"

class CEIBInterfaceConf : public IConfBase
{
public:
	CEIBInterfaceConf();
	virtual ~CEIBInterfaceConf();

	virtual void ToXml(CDataBuffer& xml_str);
	virtual void FromXml(const CDataBuffer& xml_str);

	bool StopInterface(CDataBuffer& xml_str);
	bool StartInterface(CDataBuffer& xml_str);

private:
	unsigned short _interface_port;
	EIB_DEVICE_MODE _device_mode;
	bool _auto_detect;
	CTime _last_time_sent;
	CTime _last_time_received;
	int _total_sent;
	int _total_received;
};

#endif
