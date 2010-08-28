#include "CCemi_L_BusMon_Frame.h"

using namespace EibStack;

#define MIN_ADDINFO_LENGTH 7
#define TYPEID_TIMESTAMP 0x04
#define TYPEID_TIMESTAMP_EXT 0x06
#define TYPEID_STATUSINFO 0x03
/*
CCemi_L_BusMon_Frame::CCemi_L_BusMon_Frame() : CEIBNetPacket<CEMI_L_BUSMON_IND>(L_BUSMON_IND)
{
	memset(&_data, 0, sizeof(_data));
}
*/
CCemi_L_BusMon_Frame::CCemi_L_BusMon_Frame(unsigned char* data, int len) : CEIBNetPacket<CEMI_L_BUSMON_IND>(data)
{
	Parse(data, len - GetHeaderSize());
}

CCemi_L_BusMon_Frame::~CCemi_L_BusMon_Frame() 
{
	if(_data.rawlen > 0){
		delete _data.raw;
	}
}

CCemi_L_BusMon_Frame::CCemi_L_BusMon_Frame(const CCemi_L_BusMon_Frame& rhs) : CEIBNetPacket<CEMI_L_BUSMON_IND>(rhs)
{
	*this = rhs;
}

CCemi_L_BusMon_Frame& CCemi_L_BusMon_Frame::operator=(const CCemi_L_BusMon_Frame& rhs)
{
	if(rhs._data.rawlen > 0){
		_data.raw = new unsigned char[rhs._data.rawlen];
		memcpy(_data.raw, rhs._data.raw, rhs._data.rawlen);
		_data.rawlen = rhs._data.rawlen;
	}
	_data.timestamp = rhs._data.timestamp;
	_data.status = rhs._data.status;
	return *this;
}

void CCemi_L_BusMon_Frame::Parse(unsigned char* data, int len)
{
	ASSERT_ERROR(len >= 14, "bus monitor frame length too short");

	unsigned int i = 0;
	_data.cch_structlength = data[i++];
	_data.cch_channelid = data[i++];
	_data.cch_sequencecounter = data[i++];
	_data.cch_typespecific = data[i++];
	
	_data.mc = data[i++];
	ASSERT_ERROR(_data.mc == L_BUSMON_IND, "msg code indicates no bus monitor frame");
	
	_data.ailen = data[i++];
	ASSERT_ERROR(_data.ailen >= MIN_ADDINFO_LENGTH, "bus monitor add.info length too short");

	unsigned int end_addil_idx = i + _data.ailen;
	
	bool statusRead = false;
	bool timeRead = false;
	while(end_addil_idx > i)
	{
		int typeID = data[i++];
		int typeLen = data[i++];
		
		if (!statusRead)
			statusRead = ReadStatus(typeID, typeLen, &data[i]);
		if (!timeRead)
			timeRead = ReadTimestamp(typeID, typeLen, &data[i]);

		i += typeLen;
	}

	_data.rawlen = len - end_addil_idx;
	if(_data.rawlen > 0){
		_data.raw = new unsigned char[_data.rawlen];
		memcpy(_data.raw, &data[end_addil_idx], _data.rawlen);
	}
}

bool CCemi_L_BusMon_Frame::ReadStatus(int typeID, int len, unsigned char* addInfo)
{
	if (typeID != TYPEID_STATUSINFO)
		return false;
	if (len != 1)
		throw CEIBException(EibPacketError, "wrong status info length");
	_data.status = (unsigned char) (addInfo[0] & 0xff);
		return true;
}

bool CCemi_L_BusMon_Frame::ReadTimestamp(int typeID, int len, unsigned char* addInfo)
{
	if (typeID != TYPEID_TIMESTAMP && typeID != TYPEID_TIMESTAMP_EXT)
		return false;
	if (len != 2 && len != 4)
		throw CEIBException(EibPacketError, "wrong timestamp info length");
	
	for (int i = 0; i < len; ++i){
		_data.timestamp = _data.timestamp << 8 | addInfo[i] & 0xff;
	}
	//if (len == 4)
	//	tstampType = TYPEID_TIMESTAMP_EXT;
	return true;
}
