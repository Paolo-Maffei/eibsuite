#ifndef __TUNNELING_CONNECTION_HEADER__
#define __TUNNELING_CONNECTION_HEADER__

#include "JTC.h"
#include "CString.h"
#include "Socket.h"
#include "IConnection.h"
#include "Globals.h"

#include "SearchRequest.h"
#include "SearchResponse.h"
#include "ConnectRequest.h"
#include "ConnectResponse.h"
#include "ConnectionStateRequest.h"
#include "ConnectionStateResponse.h"
#include "TunnelAck.h"
#include "TunnelRequest.h"
#include "DisconnectRequest.h"
#include "DisconnectResponse.h"

using namespace EibStack;

enum CONNECTION_STATUS
{
	DURING_OPEN,
	WAITING_FOR_ACK,
	WAITING_FOR_DATA_CONFIRMATION,
	CONNECTED,
	WAITING_DISCONNECT_RESPONSE,
	DISCONNECTED
};

class CTunnelHeartBeat : public JTCThread, public JTCMonitor
{
public:
	CTunnelHeartBeat();
	virtual ~CTunnelHeartBeat();
	virtual void run();
	void Close();
	void DecrementCounter() { --_counter;}

private:
	int _counter;
	UDPSocket _sock;
};

class CTunnelingConnection : public IConnection, public JTCMonitor
{
public:
	CTunnelingConnection(const CString& ipaddress);
	virtual ~CTunnelingConnection();

	virtual bool Connect();
	virtual bool IsConnected() const;
	virtual void DisConnect();

	virtual bool SendDataFrame(const KnxElementQueue& elem);
	virtual bool ReceiveDataFrame(CCemiFrame& frame); 

	virtual const CString& GetDeviceDataAddress() { return _device_data_address;}
	virtual int GetDeviceDataPort() { return _device_data_port;}
	virtual const CString& GetDeviceControlAddress() { return _device_control_address;}
	virtual int GetDeviceControlPort() { return _device_control_port;}


	void SetStatusDisconnected() { _connection_status = DISCONNECTED;}

	void InitConnectionParams();

private:
	class CConnectionState
	{
	public:
		CConnectionState(): _channelid(0),_recv_sequence(0),_send_sequence(0){};
		virtual ~CConnectionState(){};
		unsigned char _channelid;
		unsigned char _recv_sequence;
		unsigned char _send_sequence;
	};


private:
	void HandleCoreServices(unsigned char* recvdData);
	bool HandleTunnelingServices(unsigned char* recvdData, CCemiFrame &frame);
	void HandleConnectionStateResponse(unsigned char* buffer);
	void HandleTunnelingAck(unsigned char* buffer);
	bool HandleTunnelRequest(unsigned char* buffer,CCemiFrame &frame);
	void HandleDisconnectResponse(unsigned char* buffer);
	void HandleDisconnectRequest(unsigned char* buffer);
	void Reconnect();

public:
	const CConnectionState& GetConnectionState() { return _state;}

	virtual int GetLocalPort(){ return _data_sock.GetLocalPort(); }
	const CString& GetLocalIPAddress() { return _ipaddress;}

protected:
	CString _device_data_address;
	int _device_data_port;
	UDPSocket _data_sock;

	CString _device_control_address;
	int _device_control_port;

	CConnectionState _state;
	CONNECTION_STATUS _connection_status;
	CTunnelHeartBeat* _heartbeat;
	CString _ipaddress;
	int _num_out_of_sync_pkts;
	map<int, JTCMonitor*> _waiting_for_acks;
	map<int, JTCMonitor*> _waiting_for_confirms;
};

#endif
