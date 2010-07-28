/*! \file HttpParser.h
    \brief Http Parser - Source file
    
	This is The header file for Http Parser class. This Class can parse data in HTTP form, and then can be used to
	determine the HTTP propeties of the data (i.e. request line, headers etc.)

*/
#ifndef __HTTP_PARSER__HEADER__
#define __HTTP_PARSER__HEADER__

#include "StringTokenizer.h"
#include "Protocol.h"
#include "HttpHeader.h"
#include "HttpRequest.h"
#include "HttpReply.h"
#include "EibStdLib.h"
#include "CString.h"
#include "DataBuffer.h"
#include "Socket.h"
#include <map>
#include <exception>

#define IS_LOWER_CASE_LATIN_CHAR(c) ((int)c >= 97 && (int)c <= 122))
#define IS_UPPER_CASE_LATIN_CHAR(c) ((int)c >= 65 && (int)c <= 90))
#define IS_LEGAL_URL_CHAR(c) ((int)c >= 32 && (int)c <= 126)

enum INTERNAL_PARSER_STATUS
{
	WAITING_FOR_MORE_DATA,
	UNKNOWN_METHOD,
	UNKNOWN_HTTP_VERSION,
	MISSING_CRLF_SEPARAOTR,
	MISSING_CONTENT_TYPE_HEADER,
	MALFORMED_URL,
	MALFORMED_HEADER,
	PARSE_OK,
};

/*! \class CHttpParser
	\brief HttpParser class. the class and interpert string data and determine whether the data forms a legal Http 
		   Request. if the data forms an legal Http Request, then the class can be used to query any property of 
		   the Http req	uest such as : Http Method, Http Version, Headers, Request line etc.
*/
class EIB_STD_EXPORT CHttpParser
{
public:
	
	/*!Constructor*/
	CHttpParser();
	/*!Constructor*/
	CHttpParser(CHttpRequest& request,const char* data, int length);
	/*!Constructor*/
	CHttpParser(CHttpRequest& request,const CDataBuffer& data);
	/*!Constructor*/
	CHttpParser(CHttpRequest& request,const char* data, int length, TCPSocket& socket);
	

	/*!Constructor*/
	CHttpParser(CHttpReply& reply,const CString& data);
	/*!Constructor*/
	CHttpParser(CHttpReply& reply,const char* data, int length);
	/*!Constructor*/
	CHttpParser(CHttpReply& reply,const CDataBuffer& data);


	/*!Destructor*/
	virtual ~CHttpParser(){};

	bool IsLegalRequest() { return _legal;}
	
	void SetData(CHttpReply& reply, const char* data,int length);
	void SetData(CHttpRequest& request, const char* data,int length);
	void SetData(CHttpReply& reply, const CDataBuffer& data);
	void SetData(CHttpRequest& request, const CDataBuffer& data);

private:
	bool ParseRequest(CHttpRequest& request,const unsigned char* data, int length);
	bool ParseRequest(CHttpRequest& request,const char* data, int length);
	bool ParseRequest(CHttpRequest& request,const char* data, int length, TCPSocket& socket);

	bool ParseReply(CHttpReply& reply,const CString& data);

	//request
	INTERNAL_PARSER_STATUS ParseRequestMethod(CHttpRequest& request,const char* data, int& position);
	INTERNAL_PARSER_STATUS ParseRequestLine(CHttpRequest& request,const char* data,int& position);
	INTERNAL_PARSER_STATUS ParseRequestVersion(CHttpRequest& request,const char* data,int& position);
	INTERNAL_PARSER_STATUS ParseRequestHeaders(CHttpRequest& request,const char* data, int& position);
	INTERNAL_PARSER_STATUS ParseRequestContent(CHttpRequest& request,const char* data, int& position, int length);
	bool ParseRequestCookie(const CHttpHeader& header,CHttpCookie& cookie);
	bool RequestContentHandler(CHttpRequest& request,int& position,const char* data, int length, TCPSocket& socket);

	//reply
	bool ParseReplyVersion(CHttpReply& reply,const CString& data, int& position);
	bool ParseReplyStatus(CHttpReply& reply,const CString& data, int& position);
	bool ParseReplyHeaders(CHttpReply& reply,const CString& data, int& position);
	bool ParseReplyContent(CHttpReply& reply,const CString& data, int& position);


private:
	bool _legal;
};

#endif
