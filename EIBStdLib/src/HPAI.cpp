#include "HPAI.h"

using namespace EibStack;

CHPAI::CHPAI(unsigned char* data)
{
	_data.structlength = data[0];
	_data.hostprotocol = data[1];
	_data.ip1 = data[2];
    _data.ip2 = data[3];
    _data.ip3 = data[4];
    _data.ip4 = data[5];

	int high = data[6] & 0x000000FF;
    int low = data[7] & 0x000000FF;
	_data.port = (high << 8 | low);

	//if (_data.hostprotocol != IPV4_UDP){
	//	throw CEIBException(NotImplementedError,"Unsupported protocol found in HPAI");
	//}
}

CHPAI::CHPAI(int port, const CString& ipaddress)
{
	int address = inet_addr(ipaddress.GetBuffer());
	unsigned char* ptr = (unsigned char*)&address;
	_data.ip1 = ptr[0];
	_data.ip2 = ptr[1];
	_data.ip3 = ptr[2];
	_data.ip4 = ptr[3];
	_data.structlength = 8;
	_data.hostprotocol = IPV4_UDP;
	_data.port = port;
}

CHPAI::~CHPAI()
{
}

CString CHPAI::GetAddress() const
{
	CString res = EMPTY_STRING;
	res += (int)_data.ip1;
	res += '.';
	res += (int)_data.ip2;
	res += '.';
	res += (int)_data.ip3;
	res += '.';
	res += (int)_data.ip4;
	return res;
}

int CHPAI::GetINETAddress() const
{
	int res = 0;
	unsigned char* ptr = (unsigned char*)&res;
	ptr[0] = _data.ip1;
	ptr[1] = _data.ip2;
	ptr[2] = _data.ip3;
	ptr[3] = _data.ip4;
	return res;
}
