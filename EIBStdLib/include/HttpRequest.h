#ifndef __HTTP_REQUEST_HEADER__
#define __HTTP_REQUEST_HEADER__


#include "EibStdLib.h"
#include "CString.h"
#include "DataBuffer.h"
#include "CException.h"
#include "HttpHeader.h"
#include "HttpParameter.h"
#include "CException.h"

using namespace std;

class EIB_STD_EXPORT CHttpRequest
{
public:
	CHttpRequest();
	CHttpRequest(HTTP_METHOD method,const CString& request_uri,HTTP_VERSION version,const CString& content);
	virtual ~CHttpRequest();

	HTTP_METHOD GetMethod() const;
	const CString& GetRequestURI() const;
	HTTP_VERSION GetVersion() const;
	
	void SetMethod(HTTP_METHOD method) { _method = method;} 
	void SetRequestURI(const CString& request_uri) { _request_uri = request_uri;}
	void SetVersion(HTTP_VERSION version) { _version = version;}
	
	int GetNumHeaders() const;
	int GetNumParams() const;

	void SetContent(const CString& content);
	void SetContent(const char* content);

	void AddHeader(const CString& name,const CString& value);
	void AddHeader(const CString& name,int value);
	void AddHeader(const CString& name,int64 value);
	void AddHeader(const CHttpHeader& header);
	
	bool IsHeaderExist(const CString& header_name) const;

	bool GetHeader(const CString& header_name,CHttpHeader& header) const;
	bool GetHeader(const CHttpHeader& header_to_find,CHttpHeader& header) const;

	void RemoveAllHeaders();
	void RemoveAllParams();

	void Reset();

	bool RemoveHeader(const CString& header_name);
	bool RemoveHeader(const CHttpHeader& header);

	bool GetCookie(const CString& name, CHttpCookie& cookie) const;

	void Finalize(CDataBuffer& raw_request);

	const CDataBuffer& GetContent() const { return _content;}

	void AddParameter(const CString& name,const CString& value);
	void AddParameter(const CString& name,int value);
	void AddParameter(const CString& name,int64 value);
	void AddParameter(const CString& name,bool value);
	void AddParameter(const CString& name,double value);
	void AddParameter(const CHttpParameter& parameter);
	
	bool GetParameter(const CString& param_name,CHttpParameter& param) const;
	bool GetParameter(const CHttpParameter& param_to_find,CHttpParameter& param) const;

	bool IsParamExist(const CString& param_name);

	friend class CHttpParser;

private:
	bool _legal;
	HTTP_METHOD _method;
	CString _request_uri;
	HTTP_VERSION _version;
	CDataBuffer _content;
#ifdef WIN32
#pragma warning( disable: 4251 )
#endif
	map<CString,CHttpHeader> _headers;
	map<CString,CHttpParameter> _params;
	map<CString,CHttpCookie> _cookies;
#ifdef WIN32
#pragma warning( default: 4251 )
#endif
};

#endif
