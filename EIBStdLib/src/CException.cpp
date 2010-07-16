#include "CException.h"

CEIBException::CEIBException(EIBErrorClass error,const CString& msg) : _error_class(error),_what(msg)
{
}

CEIBException::CEIBException(EIBErrorClass error,const char* msg,...) : _error_class(error)
{
	char buffer[512];
	va_list arglist;
	va_start(arglist,msg);
	vsprintf(buffer,msg,arglist);
	va_end(arglist);
	_what = buffer;
}

CEIBException::~CEIBException() throw()
{
}

const char* CEIBException::what() const throw()
{
	return _what.GetBuffer();
}

CString CEIBException::Translate() const
{
	CString tmp;
	EIBErrorClass error = _error_class;

	switch (error)
	{
	case FileError:
		tmp += "File Error";
		break;
	case HttpProtocolError:
		tmp += "Http Protocol Error";
		break;
	case MutexError:
		tmp += "Mutex Error";
		break;
	case XmlError:
		tmp += "XML Error";
		break;
	case GeneralError:
		tmp += "General Error";
		break;
	case ConfigFileError:
		tmp += "Config File Error";
		break;
	case EibPacketError:
		tmp += "Packet Error";
		break;
	case SystemError:
		tmp += "System error";
		break;
	case OutOfMemory:
		tmp += "Out of memory error";
		break;
	case NotImplementedError:
		tmp += "Not implemented";
		break;
	case NumberConversionError:
		tmp += "Number conversion error";
		break;
	case SocketError:
		tmp += "Socket error";
		break;
	};

	return tmp;
}
