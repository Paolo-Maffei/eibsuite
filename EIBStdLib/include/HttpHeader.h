#ifndef __HTTP_HEADER__
#define __HTTP_HEADER__

#include "CString.h"
#include "Protocol.h"

/*! \class CHttpHeader
	\brief HttpHeader class. Class to represend a Http Header. an Http Header consists of Name and value.
*/
class EIB_STD_EXPORT CHttpHeader
{
public:
	/*!Constructor*/
	CHttpHeader(void);
	/*!Constructor*/
	CHttpHeader(const CString& val);
	/*!Destructor*/
	virtual ~CHttpHeader(void);

	const CString& GetName(void) const;
	const CString& GetValue(void) const;
	
	void SetName(const CString& name) { _name = name;}
	void SetValue(const CString& value) { _value = value;}

private:
	void Parse(const CString& val);

protected:
	CString _name;
	CString _value;
};

class EIB_STD_EXPORT CHttpCookie : public CHttpHeader
{
public:
	/*!Constructor*/
	CHttpCookie(void){};
	/*!Destructor*/
	virtual ~CHttpCookie(void){};

	void SetPath(const CString& path) { _path = path;}
	void SetHttpOnly(bool http_only) { _http_only = http_only;}

	bool IsHttpOnly() const { return _http_only;}
	const CString& GetPath() const { return _path;}

protected:
	CString _path;
	bool _http_only;
	//TODO: add the epiration time of cookie
};

#endif
