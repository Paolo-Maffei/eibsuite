#ifndef __HTTP_REPLY_HEADER__
#define __HTTP_REPLY_HEADER__

#include <map>
#include "EibStdLib.h"
#include "Protocol.h"
#include "CString.h"
#include "DataBuffer.h"
#include "HttpHeader.h"
#include "CException.h"

class EIB_STD_EXPORT CHttpReply
{
public:
	CHttpReply();
	virtual ~CHttpReply();

	HTTP_VERSION GetVersion() const;
	HTTP_STATUS_CODE GetStatusCode() const;
		
	void SetVersion(HTTP_VERSION version);
	void SetStatusCode(HTTP_STATUS_CODE status);
	//void SetContent(const CString& content);
	//void SetContent(const char* content);
	void SetContentType(HTTP_CONTENT_TYPE ct);

	bool GetHeader(const CString& header_name,CHttpHeader& header) const;
	bool GetHeader(const CHttpHeader& header_to_find,CHttpHeader& header) const;
	void AddHeader(const CString& name,const CString& value);
	void AddHeader(const CString& name,int value);
	void AddHeader(const CString& name,int64 value);
	void AddHeader(const CHttpHeader& header);
	
	void AddCookie(const CString& name, const CString& value);
	void AddCookie(const CHttpCookie& cookie);

	bool IsHeaderExist(const CString& header_name) const;

	void RemoveAllHeaders();

	void Finalize(CDataBuffer& raw_reply);
	
	CDataBuffer& GetContent() { return _content;}
	void Reset();
	void Redirect(const CString& url);
	friend class CHttpParser;
	
private:
	void ResolveContentType();
	void ResolveHttpVersion(CDataBuffer& raw_reply);
	void ResolveHttpStatusCode(CDataBuffer& raw_reply);

private:
	HTTP_VERSION _version;
	HTTP_STATUS_CODE _status;
	HTTP_CONTENT_TYPE _content_type;
	//CString _content;
	CDataBuffer _content;
#ifdef WIN32
#pragma warning( disable: 4251 )
#endif
	map<CString,CHttpHeader> _headers;
#ifdef WIN32
#pragma warning( default: 4251 )
#endif
};

#endif
