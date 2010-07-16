#include "HttpReply.h"

CHttpReply::CHttpReply()
{
}

CHttpReply::~CHttpReply()
{
}

void CHttpReply::SetVersion(HTTP_VERSION version)
{
	_version = version;
}

void CHttpReply::SetStatusCode(HTTP_STATUS_CODE status)
{
	_status = status;
}

HTTP_VERSION CHttpReply::GetVersion() const
{
	return _version;
}

HTTP_STATUS_CODE CHttpReply::GetStatusCode() const
{
	return _status;
}

void CHttpReply::SetContentType(HTTP_CONTENT_TYPE ct)
{
	_content_type = ct;
}

void CHttpReply::Redirect(const CString& url)
{
	CHttpHeader header;
	header.SetName(LOCATION_HEADER);
	header.SetValue(url);
	AddHeader(header);

	_status = STATUS_FOUND;
}

void CHttpReply::RemoveAllHeaders()
{
	_headers.clear();
}

void CHttpReply::AddHeader(const CString& name,const CString& value)
{
	CHttpHeader header;
	header.SetName(name);
	header.SetValue(value);
	_headers.insert(pair<CString,CHttpHeader>(name,header));
}

void CHttpReply::AddHeader(const CString& name,int value)
{
	CString val(value);
	AddHeader(name,val);
}

void CHttpReply::AddHeader(const CString& name,int64 value)
{
	CString val(value);
	AddHeader(name,val);
}

void CHttpReply::AddHeader(const CHttpHeader& header)
{
	AddHeader(header.GetName(),header.GetValue());
}

void CHttpReply::AddCookie(const CString& name, const CString& value)
{
	CString cookie(name);
	cookie += '=';
	cookie += value;
	AddHeader(SET_COOKIE_HEADER,cookie);
}

void CHttpReply::AddCookie(const CHttpCookie& cookie)
{
	CString c(cookie.GetName());
	c += '=';
	c += cookie.GetValue();

	if (cookie.GetPath() != EMPTY_STRING){
		c += ';';
		c += "path=";
		c += cookie.GetPath();
	}

	AddHeader(SET_COOKIE_HEADER,c);
}

bool CHttpReply::GetHeader(const CString& header_name,CHttpHeader& header) const
{
	map<CString,CHttpHeader>::const_iterator it;
	it = _headers.find(header_name);
	if(it == _headers.end()){
		return false;
	}
	header = it->second;
	return true;
}

bool CHttpReply::GetHeader(const CHttpHeader& header_to_find,CHttpHeader& header) const
{
	return GetHeader(header_to_find.GetName(),header);
}

bool CHttpReply::IsHeaderExist(const CString& header_name) const
{
	map<CString,CHttpHeader>::const_iterator it;
	it = _headers.find(header_name);
	if(it == _headers.end()){
		return false;
	}
	return true;
}

void CHttpReply::Reset()
{
	RemoveAllHeaders();
	_content.Clear();
	_version = UNDEF_VERSION;
	_status = UNDEF_STATUS;
}

void CHttpReply::ResolveContentType()
{
	CHttpHeader content_type;
	content_type.SetName(CONTENT_TYPE_HEADER);
	switch (_content_type)
	{
	case CT_TEXT_HTML: content_type.SetValue(CONTENT_TYPE_TEXT_HTML_STR);
		break;
	case CT_TEXT_PLAIN: content_type.SetValue(CONTENT_TYPE_TEXT_PLAIN_STR);
		break;
	case CT_TEXT_XML: content_type.SetValue(CONTENT_TYPE_TEXT_XML_STR);
		break;
	case CT_IMAGE_X_ICON: content_type.SetValue(CONTENT_TYPE_IMAGE_X_ICON_STR);
		break;
	default: content_type.SetValue(CONTENT_TYPE_TEXT_PLAIN_STR);
		break;
	}
	AddHeader(content_type);
}

void CHttpReply::ResolveHttpVersion(CDataBuffer& raw_reply)
{
	switch(_version)
	{
	case HTTP_1_0: raw_reply += HTTP_1_0_STR;
		break;
	case HTTP_1_1: raw_reply += HTTP_1_1_STR;
		break;
	default: throw CEIBException(HttpProtocolError,"Http version not defined.");
		break;
	}
}

void CHttpReply::ResolveHttpStatusCode(CDataBuffer& raw_reply)
{
	switch (_status)
	{
	case STATUS_OK:
		raw_reply += STATUS_OK;
		raw_reply += SPACE;
		raw_reply += STATUS_OK_STR;
		break;
	case STATUS_FOUND:
		raw_reply += STATUS_FOUND;
		raw_reply += SPACE;
		raw_reply += STATUS_FOUND_STR;
		break;
	case STATUS_NOT_AUTHORIZED:
		raw_reply += STATUS_NOT_AUTHORIZED;
		raw_reply += SPACE;
		raw_reply += STATUS_NOT_AUTHORIZED_STR;
		break;
	case STATUS_NOT_FOUND:
		raw_reply += STATUS_NOT_FOUND;
		raw_reply += SPACE;
		raw_reply += STATUS_NOT_FOUND_STR;
		break;
	case STATUS_INTERNAL_ERROR:
		raw_reply += STATUS_INTERNAL_ERROR;
		raw_reply += SPACE;
		raw_reply += STATUS_INTERNAL_ERROR_STR;
		break;
	case STATUS_FORBIDDEN:
		raw_reply += STATUS_FORBIDDEN;
		raw_reply += SPACE;
		raw_reply += STATUS_FORBIDDEN_STR;
		break;
	default: throw CEIBException(HttpProtocolError,"Http status code not defined.");
		break;	
	}
}

void CHttpReply::Finalize(CDataBuffer& raw_reply)
{
	raw_reply.Clear();
	ResolveContentType();

	if(!IsHeaderExist(CONTENT_LENGTH_HEADER)){
		CString size(_content.GetLength());
		AddHeader(CONTENT_LENGTH_HEADER,size);
	}

	//HTTP Version
	ResolveHttpVersion(raw_reply);
	raw_reply += SPACE;

	//Status Code
	ResolveHttpStatusCode(raw_reply);
	raw_reply += CRLF;

	//Headers
	map<CString,CHttpHeader>::iterator it;
	for(it = _headers.begin(); it != _headers.end(); ++it)
	{
		raw_reply += it->second.GetName();
		raw_reply += HEADER_SEPERATOR;
		raw_reply += SPACE;
		raw_reply += it->second.GetValue();
		raw_reply += CRLF;
	}

	//content
	if (_content.GetLength() > 0){
		raw_reply += CRLF;
		raw_reply.Add(_content.GetBuffer(),_content.GetLength());
	}

	raw_reply += CRLF;
	raw_reply += CRLF;
}
