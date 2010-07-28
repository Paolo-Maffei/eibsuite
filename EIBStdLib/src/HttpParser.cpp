#include "HttpParser.h"

// some constants
char CR = 13;             // ASCII carriage return
char LF = 10;             // ASCII line feed

CHttpParser::CHttpParser(): _legal(false)
{
}

CHttpParser::CHttpParser(CHttpRequest& request,const CDataBuffer& data): _legal(false)
{
	_legal = ParseRequest(request,data.GetBuffer(),data.GetLength());
}

CHttpParser::CHttpParser(CHttpRequest& request, const char* data, int length): _legal(false)
{
	_legal = ParseRequest(request,data,length);
}

CHttpParser::CHttpParser(CHttpRequest& request,const char* data, int length, TCPSocket& socket)
{
	int position = 0;
	if (this->ParseRequestMethod(request,data,position) != PARSE_OK){
		_legal = false;
	}
	if (!this->ParseRequestLine(request,data,position) != PARSE_OK){
		_legal = false;
	}
	if(this->ParseRequestVersion(request,data,position) != PARSE_OK){
		_legal = false;
	}
	if(this->ParseRequestHeaders(request,data,position) != PARSE_OK){
		_legal = false;
	}
	if(!this->RequestContentHandler(request,position,data,length,socket)){
		_legal = false;
	}
	_legal = true;
}

CHttpParser::CHttpParser(CHttpReply& reply,const CString& data)
{
	_legal = ParseReply(reply,data);
}

CHttpParser::CHttpParser(CHttpReply& reply,const char* data, int length)
{
	CString rep((const unsigned char*)data,length);
	_legal = ParseReply(reply,rep);
}

CHttpParser::CHttpParser(CHttpReply& reply,const CDataBuffer& data)
{
	CString rep(data.GetBuffer(),data.GetLength());
	_legal = ParseReply(reply,rep);
}

void CHttpParser::SetData(CHttpReply& reply, const char* data,int length)
{
	CString rep((const unsigned char*)data,length);
	_legal = ParseReply(reply,rep);
}

void CHttpParser::SetData(CHttpRequest& request, const char* data,int length)
{
	_legal = ParseRequest(request,data,length);
}


void CHttpParser::SetData(CHttpReply& reply, const CDataBuffer& data)
{
	CString rep(data.GetBuffer(),data.GetLength());
	_legal = ParseReply(reply,rep);
}

void CHttpParser::SetData(CHttpRequest& request, const CDataBuffer& data)
{
	_legal = ParseRequest(request,data.GetBuffer(),data.GetLength());
}

bool CHttpParser::ParseReply(CHttpReply& reply,const CString& data)
{
	int position = 0;
	if (!this->ParseReplyVersion(reply,data,position)){
		return false;
	}
	if (!this->ParseReplyStatus(reply,data,position)){
		return false;
	}
	if (!this->ParseReplyHeaders(reply,data,position)){
		return false;
	}
	if(!this->ParseReplyContent(reply,data,position)){
		return false;
	}
	return true;
}

bool CHttpParser::ParseRequest(CHttpRequest& request,const char* data, int length, TCPSocket& socket)
{
	return true;
}

bool CHttpParser::ParseRequest(CHttpRequest& request,const unsigned char* data, int length)
{
	return ParseRequest(request,(char*)data,length);
}

bool CHttpParser::ParseRequest(CHttpRequest& request,const char* data, int length)
{
	int position = 0;
	if (this->ParseRequestMethod(request,data,position) != PARSE_OK){
		return false;
	}
	if (!this->ParseRequestLine(request,data,position) != PARSE_OK){
		return false;
	}
	if(this->ParseRequestVersion(request,data,position) != PARSE_OK){
		return false;
	}
	if(this->ParseRequestHeaders(request,data,position) != PARSE_OK){
		return false;
	}
	request._content.Clear();
	if(this->ParseRequestContent(request,data,position,length) != PARSE_OK){
		return false;
	}

	return true;
}

INTERNAL_PARSER_STATUS CHttpParser::ParseRequestMethod(CHttpRequest& request,const char* data, int& position)
{
	while(IS_UPPER_CASE_LATIN_CHAR(data[position]) { ++position; }
	
	if(data[position] == SPACE){
		CString method(data,position);
		method.ToUpper();
		++position;
		if (method == HTTP_METHOD_GET_STR){
			request.SetMethod(GET_M);
			return PARSE_OK;
		}
		if (method == HTTP_METHOD_POST_STR){
			request.SetMethod(POST_M);
			return PARSE_OK;
		}
		return UNKNOWN_METHOD;
	}
	return UNKNOWN_METHOD;
}

INTERNAL_PARSER_STATUS CHttpParser::ParseRequestLine(CHttpRequest& request,const char* data,int& position)
{
	int new_pos = position;
	bool include_params = false;

	while( IS_LEGAL_URL_CHAR(data[new_pos]) && new_pos < MAX_URL_LENGTH){
		if (data[new_pos] == QUERY_STRING_SEPERATOR || data[new_pos] == SPACE){
			//include query string
			if(data[new_pos] == QUERY_STRING_SEPERATOR){
				include_params = true;
				++new_pos;
				break;
			}
			//no query string exist
			else{
				CString uri(&data[position],new_pos - position);
				request.SetRequestURI(uri);
				position = ++new_pos;
				return PARSE_OK;
			}
		}
		++new_pos; 
	}

	if (include_params){
		int current = new_pos;
		while(data[current] != SPACE){
			if(data[current] == PARAMETER_SEPERATOR)
			{
				CString param(&data[new_pos],current - new_pos);
				request.AddParameter(CHttpParameter(param));
				new_pos = current + 1;
			}
			++current;
		}
		CString param(&data[new_pos],current - new_pos);
		request.AddParameter(CHttpParameter(param));
		position = current + 1;
		return PARSE_OK;
	}

	return MALFORMED_URL;
}

INTERNAL_PARSER_STATUS CHttpParser::ParseRequestVersion(CHttpRequest& request,const char* data,int& position)
{
	bool legal = false;
	char tmp[MAX_HTTP_VERSION_STR_LENGTH + 1];
	int len = sscanf(&data[position],"%s ",tmp);
	
	if(!len == 1){
		return UNKNOWN_HTTP_VERSION;
	}

	if (!strcmp(tmp,HTTP_1_0_STR)){
		request.SetVersion(HTTP_1_0);
		position += 8;
		legal = true;
	}
	if (!strcmp(tmp,HTTP_1_1_STR)){
		request.SetVersion(HTTP_1_1);
		position += 8;
		legal = true;
	}
	
	if(legal && data[position++] == CR && data[position++] == LF){
		return PARSE_OK;
	}	
	return UNKNOWN_HTTP_VERSION;
}

INTERNAL_PARSER_STATUS CHttpParser::ParseRequestHeaders(CHttpRequest& request,const char* data, int& position)
{
	CString name,value;
	request._cookies.clear();
	int current = position;
	while(true)
	{
		while (true){
			if (data[current] == CR){
				if (data[current + 1] == LF){
					break;
				}
			}
			++current;
		}
		CString header(&data[position],current-position);
		int index = header.FindFirstOf(':');
		name = header.SubString(0,index);
		value = header.SubString(index + 1,header.GetLength() - index - 1);
		
		if(name.GetLength() == 0 || value.GetLength() == 0){
			return MALFORMED_HEADER;
		}

		name.Trim();
		value.Trim();
		if(name != COOKIE_HEADER){
			request.AddHeader(name,value);
		}
		else{
			CHttpCookie cookie;
			CHttpHeader header_val;
			header_val.SetValue(value);
			if (ParseRequestCookie(header_val,cookie)){
				request._cookies.insert(pair<CString,CHttpCookie>(cookie.GetName(),cookie));
			}
		}
		
		current += 2;
		position = current;

		if(data[current] == CR && data[current + 1] == LF){
			break;
		}
	}
	
	return PARSE_OK;
}


bool CHttpParser::ParseRequestCookie(const CHttpHeader& cookie,CHttpCookie& c)
{
	int index = cookie.GetValue().FindFirstOf('=');
	if(index == -1){
		return false;
	}
	
	c.SetName(cookie.GetValue().SubString(0,index));
	int end = cookie.GetValue().FindFirstOf(';');
	if (end == -1){
		c.SetValue(cookie.GetValue().SubString(index + 1,cookie.GetValue().GetLength() - index - 1));
	}
	else{
		c.SetValue(cookie.GetValue().SubString(index + 1,end - index - 1));
	}
	return true;
}

INTERNAL_PARSER_STATUS CHttpParser::ParseRequestContent(CHttpRequest& request,const char* data, int& position, int length)
{
	CHttpHeader cl;
	if (!request.GetHeader(CONTENT_LENGTH_HEADER,cl) || cl.GetValue().ToInt() == 0){
		return PARSE_OK;
	}

	length -= position;

	if(length < cl.GetValue().ToInt())
	{
		request._content.Add(&data[position],length - request._content.GetLength());
		return WAITING_FOR_MORE_DATA;
	}else
	{
		request._content.Add(&data[position],length - request._content.GetLength());
	}

	int end = position + cl.GetValue().ToInt() + 1;
	int current = position;	
	
	CHttpHeader content_type;
	CString ct_str;
	if(request.GetHeader(CONTENT_TYPE_HEADER,content_type))
	{
		ct_str = content_type.GetValue();
		ct_str.ToLower();
	}

	if(ct_str == CONTENT_TYPE_XXX_URL_ENCODED)
	{
		//remever to move the pointer 2 bytes in respect to the HTTP \r\n between headers and content
		if(data[position] == CR && data[position + 1] == LF)
			position += 2;
		//read content as parameters
		while(end >= current)
		{
			if(data[current] == PARAMETER_SEPERATOR){
				CString param(&data[position],current-position);
				request.AddParameter(CHttpParameter(param));
				position = current + 1;
			}
			++current;
		}
		CString param(&data[position],current-position);
		request.AddParameter(CHttpParameter(param));
		position = current + 1;	
		request._content.Clear();
	}
	
	return PARSE_OK;
}


bool CHttpParser::ParseReplyStatus(CHttpReply& reply,const CString& data, int& position)
{
	char status_code[MAX_HTTP_STATUS_CODE_NUM_STR_LENGTH + 1];
	char status_desc[MAX_HTTP_STATUS_CODE_DESC_STR_LENGTH];
	int len = sscanf(&data[position],"%s ",status_code);
	
	if(!len == 1){
		return false;
	}

	if (!strcmp(status_code,"200")){
		reply.SetStatusCode(STATUS_OK);
	}
	else if (!strcmp(status_code,"404")){
		reply.SetStatusCode(STATUS_NOT_FOUND);
	}
	else if (!strcmp(status_code,"500")){
		reply.SetStatusCode(STATUS_INTERNAL_ERROR);
	}
	else{
		throw CEIBException(HttpProtocolError,"Error in parse of Reply status code");
	}

	position += 4;
	len = sscanf(&data[position],"%s ",status_desc);
	
	if(len != 1){
		return false;
	}

	if (!strcmp(status_desc,"OK")){
		position += 3;
	}
	else if (!strcmp(status_desc,"Not Found")){
		position += 10;
	}
	else if (!strcmp(status_desc,"Internal Error")){
		position += 14;
	}
	else{
		throw CEIBException(HttpProtocolError,"Error in parse of Reply status code");
	}
	
	if(data[position++] == CR && data[position++] == LF){
		return true;
	}

	return false;
}

bool CHttpParser::ParseReplyHeaders(CHttpReply& reply,const CString& data, int& position)
{
	char header_str[MAX_HEADER_LENGTH];
	CString name,value;
	
	int current = position;
	while(true)
	{
		while (true){
			if (data[current] == CR){
				if (data[current + 1] == LF){
					break;
				}
			}
			++current;
		}
		header_str[0] = '\0';
		memcpy(header_str,&data[position],current - position);
		CString header((const unsigned char*)header_str,current-position);
		int index = header.FindFirstOf(':');
		name = header.SubString(0,index);
		value = header.SubString(index + 1,header.GetLength() - index - 1);
		
		if(name.GetLength() == 0 || value.GetLength() == 0){
			return false;
		}

		name.Trim();
		value.Trim();
		reply.AddHeader(name,value);

		current += 2;
		position = current;

		if(data[current] == CR && data[current + 1] == LF){
			break;
		}
	}
	
	return true;
}

bool CHttpParser::ParseReplyContent(CHttpReply& reply,const CString& data, int& position)
{
	return true;
}

bool CHttpParser::ParseReplyVersion(CHttpReply& reply,const CString& data, int& position)
{
	char tmp[MAX_HTTP_VERSION_STR_LENGTH + 1];
	int len = sscanf(&data[position],"%s ",tmp);
	
	if(len != 1){
		return false;
	}

	if (!strcmp(tmp,HTTP_1_0_STR)){
		reply.SetVersion(HTTP_1_0);
		position += 8;
		return true;
	}
	if (!strcmp(tmp,HTTP_1_1_STR)){
		reply.SetVersion(HTTP_1_1);
		position += 8;
		return true;
	}
	return false;
}

bool CHttpParser::RequestContentHandler(CHttpRequest& request,int& position,const char* data, int length, TCPSocket& socket)
{
	//parse the content (if exists)
	request._content.Clear();
	if (request._method != POST_M){
		return true;
	}
	if(!(data[position++] == CR && data[position++] == LF)){
		return false;
	}
	
	INTERNAL_PARSER_STATUS status = this->ParseRequestContent(request,data,position,length);
	if(status == WAITING_FOR_MORE_DATA)
	{
		char buf[5000];
		int len = 0,total_len = 0;
		do
		{
			len = socket.Recv(buf,5000,3000);
			if(len == 0){
				break;
			}
			position = 0;
			total_len += len;
		}while((status = ParseRequestContent(request,buf,position,total_len)) == WAITING_FOR_MORE_DATA);
		return (status == PARSE_OK);
	}
	else{
		return (status == PARSE_OK);
	}
}
