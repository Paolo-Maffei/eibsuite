/*! \file Buffer.h
    \brief Buffer Class - Source file
    
	This is The header file for Buffer class. This buffer is Syncronized.

*/

#ifndef __BUFFER_HEADER__
#define __BUFFER_HEADER__

#include "BufferNoSync.h"

#define MAX_BUFFER_SIZE 250

/*! \class CBuffer
	\brief Buffer Class
*/
template <class T, int MaxSize = MAX_BUFFER_SIZE>
class CBuffer : public CBufferNoSync<T, MaxSize> , public JTCMonitor
{
	
public:
	/*!Constructor*/
	CBuffer();
	/*!Destructor*/
	virtual ~CBuffer();
	
	/*!
	\brief Check is buffer is empty
	\fn bool IsEmpty()
	\return bool - true is buffer is empty
	*/
	bool IsEmpty();
	/*!
	\brief Check is buffer is full
	\fn bool IsFull()
	\return bool - true is buffer is full
	*/
	bool IsFull();
	
	/*!
	\brief Write an EIB Message to the buffer
	\fn bool bool Write(const EibMsg& data)
	\param data The message to be written to the buffer
	\return bool - trye if writing success flase else.
	*/
	bool Write(const T& data);
	/*!
	\brief Write an EIB Message to the buffer. the method removes the EIB message after it has been read.
	\fn bool Read(EibMsg& data)
	\param data The message to be filled from the buffer
	\return bool - trye if writing success flase else.
	*/
	bool Read(T& data);
		
private:
	T _data[MaxSize];         //Buffer data array
	int _head;
	int _tail;          // Head & tail pointer to the cyclic buffer
	bool _empty;                   // Buffer empty flag   
	bool _full;                    // Buffer full flag  
    
};

//buffer Constructor . Init. buffer variables
template <class T, int MaxSize> CBuffer<T,MaxSize>::CBuffer() :
CBufferNoSync<T,MaxSize>::CBufferNoSync()
{
}

// buffer Destructor
template <class T, int MaxSize> CBuffer<T,MaxSize>::~CBuffer()
{
}


// Retruns true if buffer is empty
template <class T, int MaxSize> bool CBuffer<T,MaxSize>::IsEmpty()
{
	JTCSynchronized sync(*this);
	return CBufferNoSync<T,MaxSize>::IsEmpty();
}

// Returns true if buffer is full
template <class T, int MaxSize> bool CBuffer<T,MaxSize>::IsFull()
{
    JTCSynchronized sync(*this);
    return CBufferNoSync<T,MaxSize>::IsFull();
}


//  Reads the next buffer cell. 
//	Returns true if reading operation was successful, Data is read to the ReadValue string
template <class T, int MaxSize> bool CBuffer<T,MaxSize>::Read(T& data)
{
	JTCSynchronized sync(*this);
	return CBufferNoSync<T,MaxSize>::Read(data);
}

// Writes data to the buffer
// Returns true if writing operation was successful,
template <class T, int MaxSize> bool CBuffer<T,MaxSize>::Write(const T& data)
{
	JTCSynchronized sync(*this);
	return CBufferNoSync<T,MaxSize>::Write(data);
}


#endif

