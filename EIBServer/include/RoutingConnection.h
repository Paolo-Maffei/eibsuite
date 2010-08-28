#ifndef __ROUTING_CONNECTION_HEADER__
#define __ROUTING_CONNECTION_HEADER__

#include "CString.h"
#include "Socket.h"
#include "IConnection.h"
#include "EIBNetIP.h"
#include "RoutingIndication.h"

class CRoutingConnection : public IConnection
{
public:
	CRoutingConnection(const CString& ipaddress);
	virtual ~CRoutingConnection();

	virtual bool Connect();
	virtual bool IsConnected() const;
	virtual void DisConnect();

	virtual bool SendDataFrame(const KnxElementQueue& elem);
	virtual bool ReceiveDataFrame(CCemi_L_Data_Frame& frame);

	virtual const CString& GetDeviceDataAddress() { return _device_data_address; }
	virtual int GetDeviceDataPort() { return _device_data_port; }
	virtual const CString& GetDeviceControlAddress() { return _device_data_address; }
	virtual int GetDeviceControlPort() { return _device_data_port; }
	
	virtual int GetLocalPort(){ return _data_sock.GetLocalPort(); }

private:
	CString _device_data_address;
	int _device_data_port;
	UDPSocket _data_sock;
	CString _ipaddress;
};

#endif
