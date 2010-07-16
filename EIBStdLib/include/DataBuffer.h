/*! \file DataBuffer.h
    \brief Data Buffer Class - Source file
    
	This is The header file for CDataBuffer class. This Class serves as the low level buffer to hold data arrived from/
	goes to the Network. this class is responsible to Encrypt and decrypt data using key supplied.

*/
#ifndef __DATA_BUFFER_HEADER__
#define __DATA_BUFFER_HEADER__

#include "EibStdLib.h"
#include "CString.h"
#include "CException.h"

#define DATA_BUFFER_DEFAULT_SIZE 256

/*! \class CDataBuffer
	\brief Data Buffer Class. This class  is the basic Buffer in this library. the data buffer used to send and
		   to receive data from the network. in addition, the data buffer is responsible to encrypt and decrypt 
		   it's contnet using specified private key.
*/
class EIB_STD_EXPORT CDataBuffer
{
public:
	/*!Default Constructor*/
	CDataBuffer();
	/*!Constructor*/
	CDataBuffer(const char* data,int length);
	/*!Destructor*/
	virtual ~CDataBuffer();

	/*!
	\brief Method Encrypt the data withing the DataBuffer object using specified key
	\fn void Encrypt(const CString* key = NULL)
	\param key = NULL - the key to use
	*/
	void Encrypt(const CString* key = NULL);
	/*!
	\brief Method Decrypt the data withing the DataBuffer object using specified key
	\fn void Decrypt(const CString* key = NULL)
	\param key = NULL - the key to use
	*/
	void Decrypt(const CString* key = NULL);

	/*!
	\brief Set method
	\fn void SetLength(int length)
	\param length - the desired length of the data buffer
	*/
	void SetLength(int length);
	/*!
	\brief Get method
	\fn int GetLength() const
	\return int - the length of the data buffer (number of bytes)
	*/
	int GetLength() const;
	/*!
	\brief Get method
	\fn int GetAllocated() const
	\return int - the number of bytes currently allocated for this data buffer. (not all these bytes are neccessarily used)
	*/
	int GetAllocated() const;
	/*!
	\brief Get method
	\fn unsigned char * GetBuffer() const
	\return unsigned char* - internal buffer
	*/
	unsigned char * GetBuffer() const;
	/*!
	\brief Method to clear the contents of the data buffer.
	\fn void Clear()
	*/
	void Clear();

	/*!
	\brief Index operator
	\fn char operator[](const int& index) const
	\return the byte at the desired index
	*/
	char operator[](const int& index) const;

	/*!
	\fn CDataBuffer& operator+=(const CString& str)
	\param str the string to add to the data buffer
	\return the Current DataBuffer after adding the str data
	*/
	CDataBuffer& operator+=(const CString& str);
	/*!
	\fn CDataBuffer& operator+=(const char* str)
	\param str the string to add to the data buffer
	\return the Current DataBuffer after adding the str data
	*/
	CDataBuffer& operator+=(const char* str);
	/*!
	\fn CDataBuffer& operator+=(const int& str)
	\param str the string to add to the data buffer
	\return the Current DataBuffer after adding the str data
	*/
	CDataBuffer& operator+=(const int& str);
	/*!
	\fn CDataBuffer& operator+=(const char& str)
	\param str the string to add to the data buffer
	\return the Current DataBuffer after adding the str data
	*/
	CDataBuffer& operator+=(const char& str);
	/*!
	\fn CDataBuffer& operator+=(const int64& str)
	\param str the string to add to the data buffer
	\return the Current DataBuffer after adding the str data
	*/
	CDataBuffer& operator+=(const int64& str);
	
	/*!
	\fn void Set(const CDataBuffer& data)
	\param data the string to add to the data buffer
	\return the Current DataBuffer after adding the str data
	*/
	void Set(const CDataBuffer& data);

	/*!
	\fn void Add(char c)
	\param c the character to add to the data buffer
	*/
	void Add(char c);
	/*!
	\fn void Add(const char* data)
	\param data the character to add to the data buffer
	*/
	void Add(const char* data);
	/*!
	\fn void Add(const char* data, int length)
	\param data the character to add to the data buffer
	\param length the length of the data to add
	*/
	void Add(const char* data, int length);
	/*!
	\fn void Add(const unsigned char* data, int length)
	\param data the character to add to the data buffer
	\param length the length of the data to add
	*/
	void Add(const unsigned char* data, int length);

	/*!
	\brief Method Encrypt the data withing the DataBuffer object using specified key
	\fn static void Encrypt(void* data, int len,const CString* key)
	\param data pointer to the clear data
	\param len the length of the clear data
	\param key = NULL - the key to use
	*/
	static void Encrypt(void* data, int len,const CString* key);
	/*!
	\brief Method Decrypt the data withing the DataBuffer object using specified key
	\fn static void Decrypt(void* data, int len,const CString* key)
	\param data pointer to the encrypted data
	\param len the length of the encrypted data
	\param key = NULL - the key to use
	*/
	static void Decrypt(void* data, int len,const CString* key);

	void Read(void* dst, int size);

private:
	void ReAllocate(int factor = 2);

private:
	unsigned char _data[DATA_BUFFER_DEFAULT_SIZE];
	int _length;
	int _allocated;
	int _cursor;
	bool _destroy;
	unsigned char* _data_ptr;
};

#endif
