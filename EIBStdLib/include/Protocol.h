#ifndef __PROTOCOL_HEADER__
#define __PROTOCOL_HEADER__

#include "EibStdLib.h"

//#define CR '\r'
//#define LF '\n'

#define CRLF "\r\n"
#define SPACE ' '
#define HTTP_METHOD_GET_STR "GET"
#define HTTP_METHOD_POST_STR "POST"
#define HTTP_1_0_STR "HTTP/1.0"
#define HTTP_1_1_STR "HTTP/1.1"

#define STATUS_OK_STR "OK"
#define STATUS_NOT_AUTHORIZED_STR "Unauthorized"
#define STATUS_FOUND_STR "Found"
#define STATUS_NOT_FOUND_STR "Not Found"
#define STATUS_INTERNAL_ERROR_STR "Internal Error"
#define STATUS_FORBIDDEN_STR "Forbidden"

//HTTP Method
enum EIB_STD_EXPORT HTTP_METHOD
{
	GET_M,
	POST_M,
	UNDEF_METHOD
};

//HTTP Version
enum EIB_STD_EXPORT HTTP_VERSION
{
	HTTP_1_0,
	HTTP_1_1,
	UNDEF_VERSION
};

//HTTP reply statis code
enum EIB_STD_EXPORT HTTP_STATUS_CODE
{
	STATUS_OK = 200,
	STATUS_FOUND = 302,
	STATUS_NOT_AUTHORIZED = 401,
	STATUS_FORBIDDEN = 403,
	STATUS_NOT_FOUND = 404,
	STATUS_INTERNAL_ERROR = 500,
	UNDEF_STATUS
};

enum EIB_STD_EXPORT HTTP_CONTENT_TYPE
{
	CT_TEXT_HTML,
	CT_TEXT_PLAIN,
	CT_TEXT_XML,
	CT_IMAGE_X_ICON,
	CT_IMAGE_PNG,
	CT_IMAGE_JPG,
	CT_IMAGE_JPEG,
	UNDEF_CT
};

//Known content types
#define CONTENT_TYPE_TEXT_HTML_STR "text/html"
#define CONTENT_TYPE_TEXT_PLAIN_STR "text/plain"
#define CONTENT_TYPE_TEXT_XML_STR "text/xml"
#define CONTENT_TYPE_IMAGE_X_ICON_STR "image/x-icon"
#define CONTENT_TYPE_IMAGE_PNG_STR "image/png"
#define CONTENT_TYPE_IMAGE_JPG_STR "image/jpg"
#define CONTENT_TYPE_IMAGE_JPEG_STR "image/jpeg"

//HTTP headers
#define SET_COOKIE_HEADER "Set-Cookie"
#define COOKIE_HEADER "Cookie"
#define CONTENT_TYPE_HEADER "Content-Type"
#define CONTENT_LENGTH_HEADER "Content-Length"
#define HEADER_SEPERATOR ':'
#define PARAMETER_SEPERATOR '&'
#define QUERY_STRING_SEPERATOR '?'
#define AUTHORIZATION_HEADER "Authorization"
#define AUTHENTICATION_HEADER "WWW-Authenticate"
#define LOCATION_HEADER "Location"

#define CONTENT_TYPE_XXX_URL_ENCODED "application/x-www-form-urlencoded"

//constants
#define MAX_HTTP_VERSION_STR_LENGTH 9
#define MAX_HTTP_STATUS_CODE_NUM_STR_LENGTH 3
#define MAX_HTTP_STATUS_CODE_DESC_STR_LENGTH 20
#define MAX_URL_LENGTH 1024
#define MAX_HEADER_LENGTH 512
#define MAX_NUM_HEADERS 10

//Utils
#define APPEND_HEADER_STR(string,header,value) string += header;\
											   string += HEADER_SEPERATOR;\
											   string += value; \
											   string += CRLF

#define CREATE_REUQEST_LINE(data_buf,method,line,version) data_buf += method;\
														  data_buf += SPACE;\
														  data_buf += line;\
														  data_buf += SPACE;\
														  data_buf += version;\
														  data_buf += CRLF;

//EIB Server General Messages
#define EIB_SERVER_FULL						"EIB_SERVER_FULL"

//Network Headers
#define DATA_PORT_HEADER					"Data-Port"
#define KEEPALIVE_PORT_HEADER				"KeepAlive-Port"
#define NETWORK_NAME_HEADER					"Network"
#define NETWORK_SESSION_ID_HEADER			"Session-ID"
#define ADDRESS_HEADER						"Address"
#define CLIENT_TYPE_HEADER					"Client-Type"
//Diffie Hellman headers
#define DIFFIE_HELLAM_GENERATOR				"Generator"
#define DIFFIE_HELLAM_INTERIM				"Interim"
#define DIFFIE_HELLAM_MODULUS				"Modulus"
//Login Headers
#define USER_NAME_HEADER					"User-Name"
#define PASSWORD_HEADER						"Password"
//EIB Server Information Headers
#define EIB_INTERFACE_MODE					"EIB-Interface-Mode"
//Console Headers
#define EIB_SESSION_ID_COOKIE_NAME			"EIBSESSIONID"

//Auto Discovery
#define EIB_SERVER_AUTO_DISCOVERY_REQ		"EIB_SERVER_AUTO_DISCOVERY_REQ"

//Diffie Hellman messages
#define DIFFIE_HELLMAN_SERVER_PUBLIC_DATA	"DIFFIE_HELLMAN_SERVER_PUBLIC_DATA"
#define DIFFIE_HELLMAN_CLIENT_PUBLIC_DATA	"DIFFIE_HELLMAN_CLIENT_PUBLIC_DATA"
#define DIFFIE_HELLMAN_CLIENT_HELLO			"DIFFIE_HELLMAN_CLIENT_HELLO"
#define DIFFIE_HELLMAN_CLIENT_EXCHANGE_OK	"DIFFIE_HELLMAN_CLIENT_EXCHANGE_OK" 

//LOGIN Messages
#define EIB_CLIENT_AUTHENTICATE				"AUTHENTICATE_USER"
#define EIB_CLIENT_AUTHENTICATE_SUCCESS		"USER_AUTHENTICATE_SUCCESS"
#define EIB_CLIENT_AUTHENTICATE_FAILED		"USER_AUTHENTICATE_FAILED"
//LOGOUT Messages
#define EIB_CLIENT_DISCONNECT				"DISCONNECT_USER"
#define EIB_CLIENT_DISCONNECT_SUCCESS		"USER_DISCONNECT_SUCCESS"
#define EIB_CLIENT_DISCONNECT_FAILED		"USER_DISCONNECT_FAILED"

#endif
