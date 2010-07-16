/*! \file Buffer.h
    \brief Buffer Class - Source file

	This is The header file for Buffer class. This buffer is NOT Syncronized.

*/

#ifndef __BUFFER_NO_SYNC_HEADER__
#define __BUFFER_NO_SYNC_HEADER__

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include "JTC.h"
#include "EibStdLib.h"
#include "EibNetwork.h"

#define MAX_BUFFER_SIZE 250

/*! \class CBuffer
	\brief Buffer Class
*/
template <class T, int MaxSize = MAX_BUFFER_SIZE>
class CBufferNoSync
{

public:
	/*!Constructor*/
	CBufferNoSync();
	/*!Destructor*/
	virtual ~CBufferNoSync();

	/*!
	\brief Check is buffer is empty
	\fn virtual bool IsEmpty()
	\return bool - true is buffer is empty
	*/
	virtual bool IsEmpty();
	/*!
	\brief Check is buffer is full
	\fn virtual bool IsFull()
	\return bool - true is buffer is full
	*/
	virtual bool IsFull();

	/*!
	\brief Write an EIB Message to the buffer
	\fn virtual bool Write(const T& data)
	\param data The message to be written to the buffer
	\return bool - true if writing success false else.
	*/
	virtual bool Write(const T& data);
	/*!
	\brief Write an EIB Message to the buffer. the method removes the EIB message after it has been read.
	\fn virtual bool Read(T& data)
	\param data The message to be filled from the buffer
	\return bool - true if writing success false else.
	*/
	virtual bool Read(T& data);

private:
	T _data[MaxSize];         //Buffer data array
	int _head;
	int _tail;          // Head & tail pointer to the cyclic buffer
	bool _empty;                   // Buffer empty flag
	bool _full;                    // Buffer full flag

};

//buffer Constructor . Init. buffer variables
template <class T, int MaxSize> CBufferNoSync<T,MaxSize>::CBufferNoSync() :
_head(0),
_tail(0),
_empty(true),
_full(false)
{
}

// buffer Destructor
template <class T, int MaxSize> CBufferNoSync<T,MaxSize>::~CBufferNoSync()
{
}


// Retruns true if buffer is empty
template <class T, int MaxSize> bool CBufferNoSync<T,MaxSize>::IsEmpty()
{
	bool ret;
	ret=_empty;
    return ret;
}

// Returns true if buffer is full
template <class T, int MaxSize> bool CBufferNoSync<T,MaxSize>::IsFull()
{
    bool ret;
    ret=_full;
    return ret;
}


//  Reads the next buffer cell.
//	Returns true if reading operation was successful, Data is read to the ReadValue string
template <class T, int MaxSize> bool CBufferNoSync<T,MaxSize>::Read(T& data)
{
	bool ret=false;
	if (!_empty) {
		data = _data[_tail];
		// Updating BufferFull Flag
		_full=false;
		// Updating BufferEmpty Flag
		_tail = (_tail+1) % MaxSize;
		if (_tail == _head)
		{
			_empty=true;
		}
		else
		{
			_empty=false;
		}
		ret=true;
	}
	return ret;
}

// Writes data to the buffer
// Returns true if writing operation was successful,
template <class T, int MaxSize> bool CBufferNoSync<T,MaxSize>::Write(const T& data)
{
	bool ret=false;
	if (!_full)
	{
		_data[_head] = data;
		// Updating BufferEmpty Flag
		_empty=false;
		// Updating BufferFull Flag
		_head= (_head+1) % MaxSize;
		if  (_tail==_head)
		{
			_full=true;
		}
		else
		{
			_full=false;
		}
		ret=true;
	}
	return ret;
}

#endif
