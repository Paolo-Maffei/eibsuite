#ifndef __EIB_CONNECT_REUQEST_HEADER__
#define __EIB_CONNECT_REUQEST_HEADER__

#include "EibStdLib.h"
#include "CException.h"
#include "EIBNetIP.h"
#include "CRI_CRD.h"
#include "HPAI.h"
#include "EibNetPacket.h"

namespace EibStack
{

class EIB_STD_EXPORT CConnectRequest : public CEIBNetPacket<EIBNETIP_CONNECT_REQUEST>
{

public:
	enum EIB_STD_EXPORT ConnectionType
	{
		DeviceMgmntConnection = DEVICE_MGMT_CONNECTION,
		TunnelConnection = TUNNEL_CONNECTION,
		RemoteLogConnection = REMLOG_CONNECTION,
		RemoteConfConnection = REMCONF_CONNECTION,
	    ObjSvrConnection = OBJSVR_CONNECTION
	};

	enum EIB_STD_EXPORT TunnelConnectionOption
	{
		TunnelLinkLayer = TUNNEL_LINKLAYER,
		TunnelRaw = TUNNEL_RAW,
		TunnelBusMon = TUNNEL_BUSMONITOR
	};

public:
	CConnectRequest(unsigned char* buffer);
	CConnectRequest(ConnectionType connectionType, TunnelConnectionOption option, int port, const CString& ipaddress);
	virtual ~CConnectRequest();
	void FillBuffer(unsigned char* buffer, int max_length);

	const CString& GetControlAddress() const { return _ctrl_addr; }
	int GetControlPort() const { return _ctrl_port; }	

	const CString& GetDataAddress() const { return _data_addr; }
	int GetDataPort() const { return _data_port; }	

	ConnectionType GetConnectionType() const { return _con_type; }

private:
	CString _ctrl_addr;
	int _ctrl_port;
	CString _data_addr;
	int _data_port;
	ConnectionType _con_type;
};

}

#endif
