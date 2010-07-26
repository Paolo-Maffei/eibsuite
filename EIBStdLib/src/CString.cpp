#include "CString.h"
#include "CException.h"
#include "Utils.h"

CString::CString()
{
}

CString::CString(const char* str) : _str(str)
{
}

CString::CString(bool str, bool camel_case)
{
	if (camel_case){
		_str = str ? "True" : "False";
	}
	else{
		_str = str ? "true" : "false";
	}
}

CString::CString(const unsigned char* str) : _str((char*)str)
{
}

CString::CString(const unsigned char* str,int length) : _str((char*)str,length)
{
}

CString::CString(const char* str,int length) : _str(str,length)
{
}

CString::CString(const string& str)
{
	_str = str;
}

void CString::ToLower()
{
	for (unsigned int j=0; j <_str.length(); ++j)
	{
		_str[j]=tolower(_str[j]);
	}
}

void CString::ToUpper()
{
	for (unsigned int j=0; j <_str.length(); ++j)
	{
		_str[j]=toupper(_str[j]);
	}
}

CString::CString(const CString& rhs)
{
	_str = rhs._str;
}

CString::CString(int str)
{
	ostringstream os;
	os << str;
	_str = os.str();
}

CString::CString(long str)
{
	ostringstream os;
	os << str;
	_str = os.str();
}

CString::CString(unsigned long str)
{
	ostringstream os;
	os << str;
	_str = os.str();
}

CString::CString(unsigned int str)
{
	ostringstream os;
	os << str;
	_str = os.str();
}

CString::CString(int64 str)
{
	ostringstream os;
	os << str;
	_str = os.str();
}

CString::CString(double str)
{
	ostringstream os;
	os << str;
	_str = os.str();
}

CString::~CString()
{
}

const char& CString::operator[](const int& index) const
{
	return _str[index];
}

CString& CString::operator=(const CString& rhs)
{
	_str = rhs._str;
	return *this;
}

CString& CString::operator=(const char* str)
{
	_str = str;
	return *this;
}

CString& CString::operator=(const unsigned char* str)
{
	_str = (char*)str;
	return *this;
}

CString& CString::operator=(int val)
{
	if(_str.length() > 0){
		_str.clear();
	}
	*this += val;
	return *this;
}
CString& CString::operator=(unsigned int val)
{
	Clear();
	*this += val;
	return *this;
}
CString& CString::operator=(short val)
{
	Clear();
	*this += val;
	return *this;
}
CString& CString::operator=(unsigned short val)
{
	Clear();
	*this += val;
	return *this;
}
CString& CString::operator=(double val)
{
	Clear();
	*this += val;
	return *this;
}
CString& CString::operator=(int64 val)
{
	Clear();
	*this += val;
	return *this;
}
CString& CString::operator=(uint64 val)
{
	Clear();
	*this += val;
	return *this;
}

CString& CString::operator+=(const CString& rhs)
{
	_str += rhs._str;
	return *this;
}

CString& CString::operator+=(unsigned char c)
{
	_str += c;
	return *this;
}

CString& CString::operator+=(const int& str)
{
	ostringstream os;
	os << str;
	_str += os.str();
	return *this;
}

CString& CString::operator+=(const long& str)
{
	ostringstream os;
	os << str;
	_str += os.str();
	return *this;
}

CString& CString::operator+=(const unsigned long& str)
{
	ostringstream os;
	os << str;
	_str += os.str();
	return *this;
}


CString& CString::operator+=(const unsigned int& str)
{
	ostringstream os;
	os << str;
	_str += os.str();
	return *this;
}

CString& CString::operator+=(const int64& str)
{
	ostringstream os;
	os << str;
	_str += os.str();
	return *this;
}

CString& CString::operator+=(const uint64& str)
{
	ostringstream os;
	os << str;
	_str += os.str();
	return *this;
}

CString& CString::operator+=(const double& str)
{
	ostringstream os;
	os << str;
	_str += os.str();
	return *this;
}

bool CString::operator==(const CString& rhs) const
{
	return (_str == rhs._str);
}

bool CString::operator==(const char* rhs) const
{
	return strcasecmp(_str.c_str(),rhs) == 0;
}

bool CString::operator!=(const CString& rhs) const
{
	return (_str != rhs._str);
}

bool CString::operator!=(const char* rhs) const
{
	return strcasecmp(_str.c_str(),rhs) != 0;
}

bool CString::operator>(const CString& rhs) const
{
	return _str > rhs._str;
}

bool CString::operator>(const char* rhs) const
{
	CString tmp(rhs);
	return *this > tmp;
}

bool CString::operator<(const CString& rhs) const
{
	return _str > rhs._str;
}

bool CString::operator<(const char* rhs) const
{
	CString tmp(rhs);
	return *this > tmp;
}

const CString CString::operator+(const char* rhs) const
{
	CString tmp(*this);
	tmp += rhs;
	return tmp;
}

const CString CString::operator+(char* rhs) const
{
	CString tmp(*this);
	tmp += rhs;
	return tmp;
}

const CString CString::operator+(const CString& rhs) const
{
	CString tmp(*this);
	tmp += rhs;
	return tmp;
}


int CString::FindFirstOf(const char& c) const
{
	return (int)_str.find_first_of(c);
}

int CString::FindFirstOf(const CString& c) const
{
	int ret = (int)_str.find_first_of(c.GetBuffer());
	return ret;
}

int CString::Find(char c, int pos) const
{
	return (int)_str.find(c,pos);
}

CString CString::SubString(int start,int length)  const
{
	ASSERT(start >= 0 && length >= 0 && (unsigned)(start + length) <= _str.length());
	CString ret(_str.substr(start,length));
	return ret;
}

int CString::Find(const CString& value, int pos) const
{
	return (int)_str.find(value._str.c_str(),pos);
}

int CString::RFind(const CString& value, int pos) const
{
	return (int)_str.rfind(value._str.c_str(),pos);
}


void CString::Erase(int start, int length)
{
	*this = _str.erase(start,length);
}

bool CString::IsEmpty() const
{
	return _str.empty();
}

const char* CString::GetBuffer() const
{
	return _str.c_str();
}

void CString::Clear()
{
	_str.clear();
}

int CString::GetLength() const
{
	return (int)_str.length();
}

void CString::TrimStart(char delim)
{
	int pos = (int)_str.find_first_not_of(delim);
	if(pos == 0)
	{
		return;
	}

	if(pos != static_cast<int>(string::npos)){
		_str.erase(0, pos);
	}
	else{
		_str.erase(_str.begin(), _str.end());
	}
}

void CString::Trim(char delim)
{
	int pos = (int)_str.find_last_not_of(delim);
	if(pos != static_cast<int>(string::npos)) {
		_str.erase(pos + 1);
		pos = (int)_str.find_first_not_of(delim);
		if(pos != static_cast<int>(string::npos)){
			_str.erase(0, pos);
		}
	}
	else{
		_str.erase(_str.begin(), _str.end());
	}
}

CString& CString::operator+=(const char& str)
{
	_str += str;
	return *this;
}

char CString::ToChar() const
{
	char val;
	istringstream is(_str);
	is >> val;
	return val;
}

unsigned char CString::ToUChar() const
{
	unsigned char val;
	istringstream is(_str);
	is >> val;
	return val;
}

int CString::ToInt() const
{
	int val;
	istringstream is(_str);
	is >> val;
	return val;
}

unsigned int CString::ToUInt() const
{
	unsigned int val;
	istringstream is(_str);
	is >> val;
	return val;
}

int64 CString::ToInt64() const
{
	int64 val;
	istringstream is(_str);
	is >> val;
	return val;
}
uint64 CString::ToUInt64() const
{
	uint64 val;
	istringstream is(_str);
	is >> val;
	return val;
}

double CString::ToDouble() const
{
	double val;
	istringstream is(_str);
	is >> val;
	return val;
}

bool CString::ToBool()  const
{
	if(strcmp(_str.c_str(),"true") == 0){
		return true;
	}
	return false;
}

short CString::ToShort() const
{
	short val;
	istringstream is(_str);
	is >> val;
	return val;
}

unsigned short CString::ToUShort() const
{
	unsigned short val;
	istringstream is(_str);
	is >> val;
	return val;
}

long CString::ToLong() const
{
	long val;
	istringstream is(_str);
	is >> val;
	return val;
}
unsigned long CString::ToULong() const
{
	unsigned long val;
	istringstream is(_str);
	is >> val;
	return val;
}

bool CString::ShortFromHexString(short& val) const
{
	if (GetLength() != 6 || _str[0] != '0' || (_str[1] != 'x' && _str[1] != 'X')){
		return false;
	}

	for (int counter = 2; counter < GetLength(); ++counter){
		if (!IS_HEX_DIGIT(_str[counter])){
			return false;
		}
	}

	istringstream is(_str);
	is >> hex >> val;
	return true;
}

bool CString::UShortFromHexString(unsigned short& val) const
{
	if (GetLength() > 6 || _str[0] != '0' || (_str[1] != 'x' && _str[1] != 'X')){
		return false;
	}

	for (int counter = 2; counter < GetLength(); ++counter){
		if (!IS_HEX_DIGIT(_str[counter])){
			return false;
		}
	}

	istringstream is(_str);
	is >> hex >> val;
	return true;
}

bool CString::BitFromHexString(unsigned char& val) const
{
	if (!UCharFromHexString(val) || (val != 0 && val != 1)){
		return false;
	}
	return true;
}

bool CString::UCharFromHexString(unsigned char& val) const
{
	if (GetLength() > 4 || _str[0] != '0' || (_str[1] != 'x' && _str[1] != 'X')){
		return false;
	}

	for (int counter = 2; counter < GetLength(); ++counter){
		if (!IS_HEX_DIGIT(_str[counter])){
			return false;
		}
	}
	val = (unsigned char)strtol(_str.c_str(),NULL,16);
	return true;
}

bool CString::IntFromHexString(int& val) const
{
	if (GetLength() != 10 || _str[0] != '0' || (_str[1] != 'x' && _str[1] != 'X')){
		return false;
	}

	for (int counter = 2; counter < GetLength(); ++counter){
		if (!IS_HEX_DIGIT(_str[counter])){
			return false;
		}
	}

	istringstream is(_str);
	is >> hex >> val;
	return true;
}

bool CString::UIntFromHexString(unsigned int& val) const
{
	if (GetLength() != 10 || _str[0] != '0' || (_str[1] != 'x' && _str[1] != 'X')){
		return false;
	}

	for (int counter = 2; counter < GetLength(); ++counter){
		if (!IS_HEX_DIGIT(_str[counter])){
			return false;
		}
	}

	istringstream is(_str);
	is >> hex >> val;
	return true;
}

unsigned int CString::HashCode() const
{
	unsigned int c,hash = 0;
	for (unsigned int i=0; i<_str.length();++i)
	{
		c = (int)_str[i];
		hash = c + (hash << 6) + (hash << 16) - hash;
	}

	hash &= 0x7FFFFFFF;
	return hash;
}

CString CString::ToHexFormat(char val)
{
	return ToHexFormat(sizeof(char) * 2,ToHex((int)val));
}

CString CString::ToHexFormat(unsigned char val)
{
	return ToHexFormat(sizeof(unsigned char) * 2,ToHex((unsigned int)val));
}

CString CString::ToHexFormat(short val)
{
	return ToHexFormat(sizeof(short) * 2,ToHex(val));
}

CString CString::ToHexFormat(unsigned short val)
{
	return ToHexFormat(sizeof(unsigned short) * 2,ToHex(val));
}

CString CString::ToHexFormat(int val)
{
	return ToHexFormat(sizeof(unsigned int) * 2,ToHex(val));
}

CString CString::ToHexFormat(unsigned int val)
{
	return ToHexFormat(sizeof(unsigned int) * 2,ToHex(val));
}

CString CString::ToHexFormat(int wanted_length, const CString& current)
{
	if (wanted_length > current.GetLength()){
		CString res = "0x";
		for(int i=0; i < wanted_length - current.GetLength(); ++i){
			res += '0';
		}
		res += current;
		return res;
	}
	else{
		return (CString("0x") + current);
	}
}

bool CString::EndsWith(const CString& str) const
{
	return EndsWith(str.GetBuffer());
}

bool CString::EndsWith(const char* str) const
{
    unsigned int lastMatchPos = _str.rfind(str); // Find the last occurrence of ending
    bool isEnding = (lastMatchPos != std::string::npos); // Make sure it's found at least once

    /*
    // If the string was found, make sure that any characters that follow it are the ones we're trying to ignore
    for( int i = lastMatchPos + strlen(str); (i < _str.length()) && isEnding; i++)
    {
        if( (_str[i] != '\n') &&
            (_str[i] != '\r') )
        {
            isEnding = false;
        }
    }
	*/
    return isEnding;
}
