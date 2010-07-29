#ifndef __EIB_NET_IP_PACKET_HEADER__
#define __EIB_NET_IP_PACKET_HEADER__

#include "EibStdLib.h"
#include "EIBNetIP.h"
#include "CException.h"

template <class T>
class EIB_STD_EXPORT CEIBNetPacket
{
public:
	//c'tor to create packet
	CEIBNetPacket(unsigned short service_type)
	{
		 _header.headersize = (HEADER_SIZE_10);
	     _header.version = (EIBNETIP_VERSION_10);
		 _header.servicetype = service_type;
		 _header.totalsize = HEADER_SIZE_10 + sizeof(_data);
	}

	//c'tor to parse packet
	CEIBNetPacket(unsigned char*& data)
	{
		size_t size = sizeof(_header);// + sizeof(_data);
		memcpy(&this->_header,data,size);
		_header.totalsize = htons(_header.totalsize);
		data += HEADER_SIZE_10;
		ASSERT_ERROR(_header.headersize == HEADER_SIZE_10,"Incorrect header size in packet");
		ASSERT_ERROR(_header.version == EIBNETIP_VERSION_10,"Incorrect version in packet");
	}

	virtual ~CEIBNetPacket(){};

	virtual int GetServiceType() const {return _header.servicetype; }
	virtual int GetDataSize() const { return _header.totalsize - _header.headersize;}
	virtual int GetHeaderSize() const { return _header.headersize;}
	virtual int GetVersion() const { return _header.version;}
	virtual int GetTotalSize() const { return _header.totalsize;}
	virtual unsigned char* ToByteArray() const { return (unsigned char*)&_header; }
	virtual unsigned char* GetData() const{ return (unsigned char*)&_data; }

	virtual void FillBuffer(unsigned char* buffer, int max_length)
	{
		ASSERT(max_length >= GetTotalSize());
		EIBNETIP_HEADER* ptr = ((EIBNETIP_HEADER*)buffer);
		ptr->version = _header.version;
		ptr->headersize = _header.headersize;
		ptr->servicetype = htons(_header.servicetype);
		ptr->totalsize = htons(_header.totalsize);
	}

private:
	CEIBNetPacket();

protected:
	EIBNETIP_HEADER _header;

protected:
	T _data;
};

#endif
