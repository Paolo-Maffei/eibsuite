#ifndef __EIB_HPAI_HEADER__
#define __EIB_HPAI_HEADER__

#include "Socket.h"
#include "CException.h"
#include "EIBNetIP.h"


namespace EibStack
{

class EIB_STD_EXPORT CHPAI
{
public:
	CHPAI(unsigned char* data);
	CHPAI(int port, const CString& ipaddress);
	virtual ~CHPAI();

	//get methods
	CString GetAddress() const;
	int GetINETAddress() const;
	int GetPort() const { return _data.port;}
	int GetHostProtocol() const { return _data.hostprotocol;}

	int GetDataSize() const { return sizeof(_data); }
	unsigned char* ToByteArray() { return (unsigned char*)&_data;}

private:
	EIBNETIP_HPAI _data;

};

}

#endif
