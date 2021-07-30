#pragma once
#include <string>
#include <vector>

#include "HPTypeDef.h"

#define HTTP_NAME					_T("http")
#define HTTPS_NAME					_T("https")
#define STR_HTTP_SCHEMA				"http://"
#define STR_HTTPS_SCHEMA			"https://"
#define CRLF						"\r\n"
#define NV_SEPARATOR_CHAR			'='
#define HEADER_SEPARATOR			": "
#define COOKIE_TOKENIZE				"; "
#define STR_HTTP_1_0				"HTTP/1.0"
#define STR_HTTP_1_1				"HTTP/1.1"
#define HOST_HEADER					"Host"
#define COOKIE_HEADER				"Cookie"
#define SET_COOKIE_HEADER			"Set-Cookie"
#define CONTENT_TYPE_HEADER			"Content-Type"
#define CONTENT_LENGTH_HEADER		"Content-Length"
#define TRANSFER_ENCODING_HEADER	"Transfer-Encoding"
#define UPGRADE_HEADER				"Upgrade"
#define WEB_SOCKET_HEADER_VALUE		"WebSocket"

#define HTTP_METHOD_POST			"POST"
#define HTTP_METHOD_PUT				"PUT"
#define HTTP_METHOD_PATCH			"PATCH"
#define HTTP_METHOD_GET				"GET"
#define HTTP_METHOD_DELETE			"DELETE"
#define HTTP_METHOD_HEAD			"HEAD"
#define HTTP_METHOD_TRACE			"TRACE"
#define HTTP_METHOD_OPTIONS			"OPTIONS"
#define HTTP_METHOD_CONNECT			"CONNECT"

#define HTTP_WEB_SOCKET_SEC_SALT	"258EAFA5-E914-47DA-95CA-C5AB0DC85B11"

// std::string HttpVersionToString(EnHttpVersion enVersion);
// std::vector<BYTE> MakeSecWebSocketAccept(LPCSTR lpszKey);

#include "StringT.h"

CStringA& HttpVersionToString(EnHttpVersion enVersion, CStringA& strResult);
CStringA& MakeSecWebSocketAccept(LPCSTR lpszKey, CStringA& strAccept);