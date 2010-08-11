#include "EIBAddress.h"

using namespace EibStack;

CEibAddress::CEibAddress() : _group_type(GROUP_3_LEVEL_FORMAT)
{
	_high = 0;
	_low = 0;
	_is_logical = false;
}

CEibAddress::CEibAddress(const CEibAddress& rhs)
{
	_high = rhs._high;
	_low = rhs._low;
	_is_logical = rhs._is_logical;
	_group_type = rhs._group_type;
}

CEibAddress::CEibAddress(unsigned char* body, bool is_group) : _group_type(GROUP_3_LEVEL_FORMAT)
{
	_high = (unsigned short)body[0];
	_low = (unsigned short)body[1];
	_is_logical = is_group;
}

CEibAddress::CEibAddress(unsigned int address, bool is_group) : _group_type(GROUP_3_LEVEL_FORMAT)
{
    _high = (unsigned short) ((address & 0xFF00) >> 8);
    _low = (unsigned short) ((address) & 0x00FF);
    _is_logical = is_group;
}

void CEibAddress::Set(unsigned char* body, bool is_group)
{
	_group_type = GROUP_3_LEVEL_FORMAT;
	_high = (unsigned short)body[0];
	_low = (unsigned short)body[1];
	_is_logical = is_group;
}

CEibAddress::CEibAddress(const CString& address) : _group_type(GROUP_3_LEVEL_FORMAT)
{
	if(address.Find('/') != -1){
		_is_logical = true;
		StringTokenizer tmp =  StringTokenizer(address,"/");
		if(tmp.CountTokens() == 3){
			unsigned short high = tmp.NextToken().ToUShort();
			unsigned short low = tmp.NextToken().ToUShort();
			_high = ((high << 3) | (low & 0x0007));
			_low = tmp.NextToken().ToUShort();
			_group_type = GROUP_3_LEVEL_FORMAT;
		}
		else if(tmp.CountTokens() == 2){
			unsigned short high = tmp.NextToken().ToUShort();
			unsigned short low = tmp.NextToken().ToUShort();
			_high = (unsigned short) (high << 3 | low >> 8);
			_low = (unsigned short) (low & 0x00FF);
			_group_type = GROUP_2_LEVEL_FORMAT;
		}
		else{
			throw CEIBException(GeneralError,"EIB Address error. Wrong group address syntax");
		}
	}
	else if(address.Find('.') != -1){
		_is_logical = false;
		StringTokenizer tmp =  StringTokenizer(address,".");
		if(tmp.CountTokens() == 3){
			unsigned short high = tmp.NextToken().ToUShort();
			unsigned short low = tmp.NextToken().ToUShort();
			_high = ((high << 4) | (low & 0x000F));
			_low = tmp.NextToken().ToUShort();
			_group_type = GROUP_3_LEVEL_FORMAT;
		}
		else{
			throw CEIBException(GeneralError,"EIB Address error. Wrong group address syntax");
		}
	}
	else{
		throw CEIBException(GeneralError,"EIB Address error. Need . or / separator");
	}	
}

CEibAddress::~CEibAddress()
{
}

bool CEibAddress::IsGroupAddress() const
{
	return _is_logical;
}

unsigned char CEibAddress::GetZone() const
{
	if (_is_logical){
		throw CEIBException(GeneralError,"EIBAddress: Operation not appropriate for a group address");
	}
    return (unsigned char) ((_high & 0x00F0) >> 4);
}

unsigned char CEibAddress::GetLine() const
{
	if (_is_logical){
		throw CEIBException(GeneralError,"EIBAddress: Operation not appropriate for a group address");
	}
    return (unsigned char) ((_high & 0x000F));
}

unsigned char CEibAddress::GetDevice() const
{
	if (_is_logical){
		throw CEIBException(GeneralError,"EIBAddress: Operation not appropriate for a group address");
	}
    return (unsigned char) (_low & 0x00FF);
}

unsigned char CEibAddress::GetMiddleGroup() const
{
	if (!_is_logical){
        throw CEIBException(GeneralError,"EIBAddress: Operation not appropriate for a group address");
	}
    return (unsigned char) (_high & 0x0007);
}

unsigned char CEibAddress::GetMainGroup() const
{
	if (!_is_logical){
        throw CEIBException(GeneralError,"EIBAddress: Operation not appropriate for a group address");
	}
    return (unsigned char) (_high >> 3);
}

unsigned short CEibAddress::GetSubGroup11() const
{
	if (!_is_logical){
        throw CEIBException(GeneralError,"EIBAddress: Operation not appropriate for a group address");
	}
    return (unsigned short) ((_high & 0x0007) << 8 | _low);
}

unsigned char CEibAddress::GetSubGroup8() const
{
	if (!_is_logical){
        throw CEIBException(GeneralError,"EIBAddress: Operation not appropriate for a group address");
	}
    return (unsigned char) (_low & 0x00FF);
}

unsigned short CEibAddress::ToByteArray() const
{
	unsigned short res = 0;
	unsigned char* ptr = (unsigned char*)&res;
	ptr[0] = (unsigned char)_high;
	ptr[1] = (unsigned char)_low;

	return htons(res);
}

CString CEibAddress::ToString() const
{
	CString address = "";
    if (IsGroupAddress())
	{
	    if (_group_type == GROUP_2_LEVEL_FORMAT) {
			address = (int)GetMainGroup();
			address += '/';
			address += (int)GetSubGroup11();
        }
        else {
			address = (int)GetMainGroup();
			address += '/';
			address += (int)GetMiddleGroup();
			address += '/';
			address += (int)GetSubGroup8();
        }
        return address;
    }
	address = (int)GetZone();
	address += '.';
	address += (int)GetLine();
	address += '.';
	address += (int)GetDevice();
    return  address;
}

bool CEibAddress::operator==(const CEibAddress& rhs) const
{
	return (ToByteArray() == rhs.ToByteArray() && _is_logical == rhs._is_logical);
}

bool CEibAddress::operator!=(const CEibAddress& rhs) const
{
	return (ToByteArray() != rhs.ToByteArray() || _is_logical != rhs._is_logical);
}

bool CEibAddress::operator>(const CEibAddress& rhs) const
{
	if (ToByteArray() > rhs.ToByteArray()){
		return true;
	}
	
	if (ToByteArray() < rhs.ToByteArray()){
		return false;
	}

	if(_is_logical && !rhs._is_logical){
		return true;
	}
	return false;
}

bool CEibAddress::operator<(const CEibAddress& rhs) const
{
	if (ToByteArray() < rhs.ToByteArray())
	{
		return true;
	}
	
	if (ToByteArray() > rhs.ToByteArray())
	{
		return false;
	}

	if(!_is_logical && rhs._is_logical){
		return true;
	}
	return false;
}

CEibAddress& CEibAddress::operator=(const CEibAddress& rhs)
{
	_low = rhs._low;
	_high = rhs._high;
	_is_logical = rhs._is_logical;
	_group_type = rhs._group_type;

	return *this;
}
