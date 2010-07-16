#include "DataBuffer.h"

CDataBuffer::CDataBuffer() :
_length(0),
_allocated(DATA_BUFFER_DEFAULT_SIZE),
_cursor(0),
_destroy(false),
_data_ptr(NULL)
{
}

CDataBuffer::~CDataBuffer()
{
	if(_destroy && _data != NULL)
	{
		delete _data_ptr;
		_data_ptr = NULL;
	}
}

CDataBuffer::CDataBuffer(const char* data,int length) : _length(0),_allocated(DATA_BUFFER_DEFAULT_SIZE),
_destroy(false),_data_ptr(NULL)
{
	if (length > _allocated){
		ReAllocate((length / _allocated) + 1);
	}
	memcpy(&GetBuffer()[0],data,length);
	_length = length;
}

void CDataBuffer::SetLength(int length)
{
	_length = length;
}

void CDataBuffer::Set(const CDataBuffer& data)
{
	Clear();
	Add((char*)data.GetBuffer(),data.GetLength());
}

void CDataBuffer::Read(void* dst, int size)
{
	ASSERT(_cursor + size <= _length)

	memcpy(dst,&this->GetBuffer()[_cursor],size);
	_cursor += size;
}

char CDataBuffer::operator[](const int& index) const
{
	return GetBuffer()[index];
}

CDataBuffer& CDataBuffer::operator+=(const char* str)
{
	this->Add(str);
	return *this;
}
CDataBuffer& CDataBuffer::operator+=(const int& str)
{
	CString s(str);
	this->Add(s.GetBuffer(),s.GetLength());
	return *this;
}

CDataBuffer& CDataBuffer::operator+=(const CString& str)
{
	this->Add(str.GetBuffer(),str.GetLength());
	return *this;
}

CDataBuffer& CDataBuffer::operator+=(const int64& str)
{
	CString s(str);
	this->Add(s.GetBuffer(),s.GetLength());
	return *this;
}

CDataBuffer& CDataBuffer::operator+=(const char& str)
{
	this->Add(str);
	return *this;
}


int CDataBuffer::GetLength() const
{
	return _length;
}

int CDataBuffer::GetAllocated() const
{
	return _allocated;
}

unsigned char* CDataBuffer::GetBuffer() const
{
	if (_data_ptr != NULL){
		return _data_ptr;
	}
	return (unsigned char*)&_data[0];
}

void CDataBuffer::ReAllocate(int factor)
{
	unsigned char* tmp = new unsigned char[_allocated * factor];
	if(_data_ptr != NULL){
		memcpy(tmp,_data_ptr,_length);
	}
	else{
		memcpy(tmp,&_data[0],_length);
	}
	_allocated = _allocated * factor;
	_destroy = true;
	if (_data_ptr != NULL){
		delete _data_ptr;
		_data_ptr = NULL;
	}
	_data_ptr = tmp;
}

void CDataBuffer::Add(char c)
{
	if (_allocated == _length){
		ReAllocate();
	}
	GetBuffer()[_length++] = (unsigned char)c;
}

void CDataBuffer::Add(const char* data)
{
	int length = (int)strlen(data);
	if (length + _length > _allocated){
		int factor = ((length + _length) / (_allocated)) + 1;
		ReAllocate(factor);
	}
	memcpy(&GetBuffer()[_length],data,length);
	_length += length;
}

void CDataBuffer::Clear()
{
	_length =  0;
}
void CDataBuffer::Add(const unsigned char* data, int length)
{
	Add((const char*)data,length);
}

void CDataBuffer::Add(const char* data, int length)
{
	if (length + _length > _allocated){
		int factor = ((length + _length) / (_allocated)) + 1;
		ReAllocate(factor);
	}
	memcpy(&GetBuffer()[_length],data,length);
	_length += length;
}


void CDataBuffer::Encrypt(const CString* key)
{
	ASSERT(key != NULL && key->GetLength() > 0);

	const CString& key_str = *key;

	for(int i=0;i < _length;++i)
	{
		GetBuffer()[i] = GetBuffer()[i] ^ key_str[i % key->GetLength()];
	}
}

void CDataBuffer::Decrypt(const CString* key)
{
	ASSERT(key != NULL && key->GetLength() > 0);

	const CString& key_str = *key;
	//current buffer.
	unsigned char* buf = GetBuffer();

	for(int i=0;i < _length;++i)
	{
		buf[i] = buf[i] ^ key_str[i % key->GetLength()];
	}
}

void CDataBuffer::Encrypt(void* data, int len,const CString* key)
{
	ASSERT(key != NULL && key->GetLength() > 0);

	const CString& key_str = *key;
	char* _data = (char*)data;
	for(int i=0;i < len;++i)
	{
		_data[i] = _data[i] ^ key_str[i % key->GetLength()];
	}
}

void CDataBuffer::Decrypt(void* data, int len,const CString* key)
{
	ASSERT(key != NULL && key->GetLength() > 0);
	
	const CString& key_str = *key;
	char* _data = (char*)data;
	for(int i=0;i < len;++i)
	{
		_data[i] = _data[i] ^ key_str[i % key->GetLength()];
	}
}
