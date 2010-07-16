#ifndef __BUS_CONNECTION_HEADER__
#define __BUS_CONNECTION_HEADER__

#include "JTC.h"
#include "CString.h"
#include "Socket.h"
#include "TunnelConnection.h"
#include "Globals.h"

using namespace EibStack;

class CBusMonConnection : public CTunnelingConnection
{
public:
	CBusMonConnection(const CString& ipaddress);
	virtual ~CBusMonConnection();

	virtual bool Connect();
};

#endif
