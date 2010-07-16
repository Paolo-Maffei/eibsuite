#ifndef __EXCEPTION_HEADER__
#define __EXCEPTION_HEADER__

#include "EibStdLib.h"
#include "CString.h"
#include "Globals.h"

using namespace std;

enum EIB_STD_EXPORT EIBErrorClass
{
	GeneralError,
	FileError,
	HttpProtocolError,
	MutexError,
	ConfigFileError,
	XmlError,
	EibPacketError,
	SystemError,
	SocketError,
	OutOfMemory,
	NumberConversionError,
	NotImplementedError
};

class EIB_STD_EXPORT CEIBException : public exception
{
public:
	CEIBException(EIBErrorClass error,const char* msg,...);
	CEIBException(EIBErrorClass error,const CString& msg);
	virtual ~CEIBException() throw() ;
	virtual const char* what() const throw();
	void Throw() { throw *this;}

private:
	virtual CString Translate() const;
	
private:
	EIBErrorClass _error_class;
	CString _what;
};

#endif
