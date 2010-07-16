#include "HttpRequest.h"
#include "Globals.h"

CHttpRequest::CHttpRequest() :
_legal(true),
_method(UNDEF_METHOD),
_request_uri(EMPTY_STRING),
_version(UNDEF_VERSION)
{
}

CHttpRequest::CHttpRequest(HTTP_METHOD method,const CString& request_uri,HTTP_VERSION version,const CString& content) :
_legal(true),
_method(method),
_request_uri(request_uri),
_version(version)
{
	if(content.GetLength() > 0){
		_content.Add(content.GetBuffer(),content.GetLength());
	}
}

CHttpRequest::~CHttpRequest()
{
}

HTTP_METHOD CHttpRequest::GetMethod() const
{
	return _method;
}

const CString& CHttpRequest::GetRequestURI() const
{
	return _request_uri;
}

HTTP_VERSION CHttpRequest::GetVersion() const
{
	return _version;
}

void CHttpRequest::Reset()
{
	RemoveAllHeaders();
	RemoveAllParams();
	_content.Clear();
	_method = UNDEF_METHOD;
	_version = UNDEF_VERSION;
}

void CHttpRequest::AddHeader(const CString& name,const CString& value)
{
	if(!IsHeaderExist(name)){
		CHttpHeader header;
		header.SetName(name);
		header.SetValue(value);
		_headers.insert(pair<CString,CHttpHeader>(name,header));
	}
	else{
		throw CEIBException(HttpProtocolError,"Http Header already exist.");
	}
}

void CHttpRequest::AddHeader(const CString& name,int value)
{
	CString val(value);
	AddHeader(name,val);
}

void CHttpRequest::AddHeader(const CString& name,int64 value)
{
	CString val(value);
	AddHeader(name,val);
}

void CHttpRequest::AddHeader(const CHttpHeader& header)
{
	AddHeader(header.GetName(),header.GetValue());
}

bool CHttpRequest::IsParamExist(const CString& param_name)
{
	if(!_legal){
		return false;
	}

	map<CString,CHttpParameter>::const_iterator it;
	it = _params.find(param_name);
	if(it == _params.end()){
		return false;
	}
	return true;
}

void CHttpRequest::AddParameter(const CString& name,const CString& value)
{
	if(!IsParamExist(name)){
		CHttpParameter param;
		param.SetName(name);
		param.SetValue(value);
		_params.insert(pair<CString,CHttpParameter>(name,param));
	}
	else{
		throw CEIBException(HttpProtocolError,"Http Parameter already exist.");
	}
}

void CHttpRequest::AddParameter(const CString& name,int value)
{
	CString val(value);
	AddParameter(name,val);
}

void CHttpRequest::AddParameter(const CString& name,int64 value)
{
	CString val(value);
	AddParameter(name,val);
}

void CHttpRequest::AddParameter(const CString& name,bool value)
{
	CString val(value);
	AddParameter(name,val);
}

void CHttpRequest::AddParameter(const CString& name,double value)
{
	CString val(value);
	AddParameter(name,val);
}

void CHttpRequest::AddParameter(const CHttpParameter& parameter)
{
	AddParameter(parameter.GetName(),parameter.GetValue());
}

bool CHttpRequest::GetCookie(const CString& name, CHttpCookie& cookie) const
{
	if(!_legal){
		return false;
	}
	map<CString,CHttpCookie>::const_iterator it;
	it = _cookies.find(name);
	if(it != _cookies.end()){
		cookie = it->second;
		return true;
	}
	return false;
}

void CHttpRequest::RemoveAllHeaders()
{
	_headers.clear();
	_cookies.clear();
}

void CHttpRequest::RemoveAllParams()
{
	_params.clear();
}

bool CHttpRequest::IsHeaderExist(const CString& header_name) const
{
	if(!_legal){
		return false;
	}

	map<CString,CHttpHeader>::const_iterator it;
	it = _headers.find(header_name);
	if(it == _headers.end()){
		return false;
	}
	return true;
}

bool CHttpRequest::GetHeader(const CString& header_name,CHttpHeader& header) const
{
	if(!_legal){
		return false;
	}

	map<CString,CHttpHeader>::const_iterator it;
	it = _headers.find(header_name);
	if(it == _headers.end()){
		return false;
	}
	header = it->second;
	return true;
}

bool CHttpRequest::GetHeader(const CHttpHeader& header_to_find,CHttpHeader& header) const
{
	return GetHeader(header_to_find.GetName(),header);
}

bool CHttpRequest::GetParameter(const CString& param_name,CHttpParameter& param) const
{
	if(!_legal){
		return false;
	}

	map<CString,CHttpParameter>::const_iterator it;
	it = _params.find(param_name);
	if(it == _params.end()){
		return false;
	}
	param = it->second;
	return true;
}

bool CHttpRequest::GetParameter(const CHttpParameter& param_to_find,CHttpParameter& param) const
{
	return GetParameter(param_to_find.GetName(),param);
}

bool CHttpRequest::RemoveHeader(const CString& header_name)
{
	if(!_legal){
		return false;
	}

	map<CString,CHttpHeader>::iterator it;
	it = _headers.find(header_name);
	if(it == _headers.end()){
		return false;
	}
	_headers.erase(it);
	return true;
}

bool CHttpRequest::RemoveHeader(const CHttpHeader& header)
{
	return RemoveHeader(header.GetName());
}

void CHttpRequest::SetContent(const CString& content)
{
	_content.Add(content.GetBuffer(),content.GetLength());
}

void CHttpRequest::SetContent(const char* content)
{
	_content.Add(content);
}

int CHttpRequest::GetNumHeaders() const
{
	return (int)_headers.size();
}

int CHttpRequest::GetNumParams() const
{
	return (int)_params.size();
}

void CHttpRequest::Finalize(CDataBuffer& raw_request)
{
	raw_request.Clear();

	if(!IsHeaderExist(CONTENT_LENGTH_HEADER)){
		CHttpHeader content_length;
		content_length.SetName(CONTENT_LENGTH_HEADER);
		content_length.SetValue(CString(_content.GetLength()));
		AddHeader(content_length);
	}

	if (_method == GET_M){
		raw_request += HTTP_METHOD_GET_STR;
	}
	else if (_method == POST_M){
		raw_request += HTTP_METHOD_POST_STR;
	}
	else{
		throw CEIBException(HttpProtocolError,"Http Method not defined.");
	}
	raw_request += SPACE;
	raw_request += _request_uri;
	
	//parameters section
	if(_params.size() > 0){
		raw_request += QUERY_STRING_SEPERATOR;
	}

	map<CString,CHttpParameter>::iterator params_it;
	for(params_it = _params.begin(); params_it != _params.end(); ++params_it)
	{
		raw_request += params_it->second.GetName();
		raw_request += PARAMETER_SEPERATOR;
		raw_request += params_it->second.GetValue();
	}

	raw_request += SPACE;

	if (_version == HTTP_1_0){
		raw_request += HTTP_1_0_STR;
	}
	else if (_version == HTTP_1_1){
		raw_request += HTTP_1_0_STR;
	}
	else{
		throw CEIBException(HttpProtocolError,"Http version not defined.");
	}
	raw_request += CRLF;

	map<CString,CHttpHeader>::iterator it;
	for(it = _headers.begin(); it != _headers.end(); ++it)
	{
		raw_request += it->second.GetName();
		raw_request += HEADER_SEPERATOR;
		raw_request += SPACE;
		raw_request += it->second.GetValue();
		raw_request += CRLF;
	}

	if (_content.GetLength() > 0){
		raw_request += CRLF;
		raw_request.Add(_content.GetBuffer(),_content.GetLength());
	}

	raw_request += CRLF;
	raw_request += CRLF;
}
