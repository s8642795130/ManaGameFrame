
#include "Helper.h"
#include "GlobalDef.h"
#include "Crypto.h"

#include <iostream>

/*
LPCSTR HTTP_WEB_SOCKET_CLOSE_FLAG = "$close";
const BYTE HTTP_WEB_SOCKET_MASK_KEY[] = { 0x1, 0x2, 0x3, 0x4 };

std::string HttpVersionToString(EnHttpVersion enVersion)
{
	// strResult.Format("HTTP/%d.%d", LOBYTE(enVersion), HIBYTE(enVersion));
	std::string ret = std::string("HTTP/") + std::to_string(LOBYTE(enVersion)) + "." + std::to_string(HIBYTE(enVersion));
	return ret;
}

std::vector<BYTE> MakeSecWebSocketAccept(LPCSTR lpszKey)
{
	std::string strKey(lpszKey);
	strKey += HTTP_WEB_SOCKET_SEC_SALT;

	_SHA1_CTX ctx;
	BYTE buf[SHA1_BLOCK_SIZE];

	::sha1_init(&ctx);
	::sha1_update(&ctx, (BYTE*)(LPCSTR)strKey.c_str(), strKey.length());
	::sha1_final(&ctx, buf);

	std::vector<BYTE> strAccept(SHA1_BLOCK_SIZE * 4 / 3 + 4);
	// BYTE* lpszAccept = (BYTE*)strAccept.GetBuffer(SHA1_BLOCK_SIZE * 4 / 3 + 4);

	// int len = (int)::base64_encode(buf, strAccept.data(), SHA1_BLOCK_SIZE, FALSE);
	::base64_encode(buf, strAccept.data(), SHA1_BLOCK_SIZE, FALSE);
	// strAccept.ReleaseBufferSetLength(len);

	return strAccept;
}
*/

CStringA& HttpVersionToString(EnHttpVersion enVersion, CStringA& strResult)
{
	strResult.Format("HTTP/%d.%d", LOBYTE(enVersion), HIBYTE(enVersion));
	return strResult;
}

CStringA& MakeSecWebSocketAccept(LPCSTR lpszKey, CStringA& strAccept)
{
	CStringA strKey(lpszKey);
	strKey.Append(HTTP_WEB_SOCKET_SEC_SALT);

	_SHA1_CTX ctx;
	BYTE buf[SHA1_BLOCK_SIZE];

	::sha1_init(&ctx);
	::sha1_update(&ctx, (BYTE*)(LPCSTR)strKey, strKey.GetLength());
	::sha1_final(&ctx, buf);

	BYTE* lpszAccept = (BYTE*)strAccept.GetBuffer(SHA1_BLOCK_SIZE * 4 / 3 + 4);

	int len = (int)::base64_encode(buf, lpszAccept, SHA1_BLOCK_SIZE, FALSE);
	strAccept.ReleaseBufferSetLength(len);

	return strAccept;
}