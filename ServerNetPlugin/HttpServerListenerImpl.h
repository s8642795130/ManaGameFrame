#pragma once

#include "HPSocket.h"
#include "Helper.h"

#include "../Server/IPluginManager.h"
#include "../ActorPlugin/IThreadPoolModule.h"
#include "IClientNetModule.h"
#include "INetActor.h"

#include "BufferPtr.h"
#include "StringT.h"
#include "HttpCookie.h"

class CHttpServerListenerImpl : public CHttpServerListener
{
protected:
	// manager
	std::shared_ptr<IPluginManager> m_ptr_manager;

	// module
	std::shared_ptr<IThreadPoolModule> m_thread_pool_module;
	std::shared_ptr<IClientNetModule> m_client_net_module;

private:

	EnHandleResult OnClose(ITcpServer* pSender, CONNID dwConnID, EnSocketOperation enOperation, int iErrorCode)
	{
		// iErrorCode == SE_OK ? ::PostOnClose(dwConnID, m_strName) :
		//	::PostOnError(dwConnID, enOperation, iErrorCode, m_strName);

		CBufferPtr* pBuffer = nullptr;
		pSender->GetConnectionExtra(dwConnID, (PVOID*)&pBuffer);

		if (pBuffer) delete pBuffer;

		return HR_OK;
	}

	EnHandleResult OnShutdown(ITcpServer* pSender)
	{
		return HR_OK;
	}

	// ------------------------------------------------------------------------------------------------------------- //

	EnHttpParseResult OnHeadersComplete(IHttpServer* pSender, CONNID dwConnID)
	{
		CStringA strSummary = GetHeaderSummary(pSender, dwConnID, "    ", 0, TRUE);

		return HPR_OK;
	}

	EnHttpParseResult OnBody(IHttpServer* pSender, CONNID dwConnID, const BYTE* pData, int iLength)
	{
		return HPR_OK;
	}

	EnHttpParseResult OnMessageComplete(IHttpServer* pSender, CONNID dwConnID)
	{

		if (pSender->IsUpgrade(dwConnID))
			return HPR_OK;

		CStringA strBody = GetHeaderSummary(pSender, dwConnID, "    ", 0, FALSE);
		int iBodyLength = strBody.GetLength();
		BOOL bSkipBody = FALSE;

		if (strcmp(pSender->GetMethod(dwConnID), HTTP_METHOD_HEAD) == 0)
			bSkipBody = TRUE;

		CStringA strContentLength;
		strContentLength.Format("%u", iBodyLength);

		DWORD dwSeq = 1;
		LPCSTR lpszReqSequence = nullptr;
		CStringA strSeq;

		if (pSender->GetCookie(dwConnID, "__reqSequence_1", &lpszReqSequence))
			dwSeq += atoi(lpszReqSequence);

		strSeq.Format("%u", dwSeq);

		CStringA strSeqCookie1 = CCookie::ToString("__reqSequence_1", strSeq, nullptr, nullptr, -1, TRUE, TRUE, CCookie::SS_LAX);

		dwSeq = 1;
		lpszReqSequence = nullptr;

		if (pSender->GetCookie(dwConnID, "__reqSequence_2", &lpszReqSequence))
			dwSeq += atoi(lpszReqSequence);

		strSeq.Format("%u", dwSeq);

		CStringA strSeqCookie2 = CCookie::ToString("__reqSequence_2", strSeq, nullptr, "/", 300, FALSE, FALSE, CCookie::SS_NONE);

		THeader header[] = { {"Content-Type", "text/plain"}, {"Content-Length", strContentLength}, {"Set-Cookie", strSeqCookie1}, {"Set-Cookie", strSeqCookie2} };
		int iHeaderCount = sizeof(header) / sizeof(THeader);

		if (bSkipBody)
		{
			strBody.Empty();
			iBodyLength = 0;
		}

		pSender->SendResponse(dwConnID,
			HSC_OK,
			"HP Http Server OK",
			header, iHeaderCount,
			(const BYTE*)(LPCSTR)strBody,
			iBodyLength);

		if (!pSender->IsKeepAlive(dwConnID))
			pSender->Release(dwConnID);

		return HPR_OK;
	}

	EnHttpParseResult OnParseError(IHttpServer* pSender, CONNID dwConnID, int iErrorCode, LPCSTR lpszErrorDesc)
	{
		return HPR_OK;
	}

	// ------------------------------------------------------------------------------------------------------------- //

	CStringA GetHeaderSummary(IHttpServer* pSender, CONNID dwConnID, LPCSTR lpszSep, int iSepCount, BOOL bWithContentLength)
	{
		CStringA strSEP1;

		for (int i = 0; i < iSepCount; i++)
			strSEP1 += lpszSep;

		CStringA strSEP2(strSEP1);
		strSEP2 += lpszSep;

		LPCSTR SEP1 = (LPCSTR)strSEP1;
		LPCSTR SEP2 = (LPCSTR)strSEP2;

		CStringA strResult;

		//USHORT usUrlFieldSet = pSender->GetUrlFieldSet(dwConnID);

		strResult.AppendFormat("%s[URL Fields]%s", SEP1, CRLF);
		strResult.AppendFormat("%s%8s: %s%s", SEP2, "SCHEMA", pSender->GetUrlField(dwConnID, HUF_SCHEMA), CRLF);
		strResult.AppendFormat("%s%8s: %s%s", SEP2, "HOST", pSender->GetUrlField(dwConnID, HUF_HOST), CRLF);
		strResult.AppendFormat("%s%8s: %s%s", SEP2, "PORT", pSender->GetUrlField(dwConnID, HUF_PORT), CRLF);
		strResult.AppendFormat("%s%8s: %s%s", SEP2, "PATH", pSender->GetUrlField(dwConnID, HUF_PATH), CRLF);
		strResult.AppendFormat("%s%8s: %s%s", SEP2, "QUERY", pSender->GetUrlField(dwConnID, HUF_QUERY), CRLF);
		strResult.AppendFormat("%s%8s: %s%s", SEP2, "FRAGMENT", pSender->GetUrlField(dwConnID, HUF_FRAGMENT), CRLF);
		strResult.AppendFormat("%s%8s: %s%s", SEP2, "USERINFO", pSender->GetUrlField(dwConnID, HUF_USERINFO), CRLF);

		DWORD dwHeaderCount = 0;
		pSender->GetAllHeaders(dwConnID, nullptr, dwHeaderCount);

		strResult.AppendFormat("%s[Request Headers]%s", SEP1, CRLF);

		if (dwHeaderCount == 0)
			strResult.AppendFormat("%s(no header)%s", SEP2, CRLF);
		else
		{
			unique_ptr<THeader[]> headers(new THeader[dwHeaderCount]);
			VERIFY(pSender->GetAllHeaders(dwConnID, headers.get(), dwHeaderCount));

			for (DWORD i = 0; i < dwHeaderCount; i++)
				strResult.AppendFormat("%s%s: %s%s", SEP2, headers[i].name, headers[i].value, CRLF);
		}

		DWORD dwCookieCount = 0;
		pSender->GetAllCookies(dwConnID, nullptr, dwCookieCount);

		strResult.AppendFormat("%s[Cookies]%s", SEP1, CRLF);

		if (dwCookieCount == 0)
			strResult.AppendFormat("%s(no cookie)%s", SEP2, CRLF);
		else
		{
			unique_ptr<TCookie[]> cookies(new TCookie[dwCookieCount]);
			VERIFY(pSender->GetAllCookies(dwConnID, cookies.get(), dwCookieCount));

			for (DWORD i = 0; i < dwCookieCount; i++)
				strResult.AppendFormat("%s%s: %s%s", SEP2, cookies[i].name, cookies[i].value, CRLF);
		}

		CStringA strVersion;
		::HttpVersionToString((EnHttpVersion)pSender->GetVersion(dwConnID), strVersion);
		EnHttpUpgradeType enUpgType = pSender->GetUpgradeType(dwConnID);
		LPCSTR lpszUpgrade = enUpgType != HUT_NONE ? "true" : "false";
		LPCSTR lpszKeepAlive = pSender->IsKeepAlive(dwConnID) ? "true" : "false";

		strResult.AppendFormat("%s[Basic Info]%s", SEP1, CRLF);
		strResult.AppendFormat("%s%13s: %s%s", SEP2, "Version", (LPCSTR)strVersion, CRLF);
		strResult.AppendFormat("%s%13s: %s%s", SEP2, "Method", pSender->GetMethod(dwConnID), CRLF);
		strResult.AppendFormat("%s%13s: %s%s", SEP2, "IsUpgrade", lpszUpgrade, CRLF);
		if (enUpgType != HUT_NONE)
			strResult.AppendFormat("%s%13s: %d%s", SEP2, "UpgradeType", enUpgType, CRLF);
		strResult.AppendFormat("%s%13s: %s%s", SEP2, "IsKeepAlive", lpszKeepAlive, CRLF);
		if (bWithContentLength)
			strResult.AppendFormat("%s%13s: %lld%s", SEP2, "ContentLength", pSender->GetContentLength(dwConnID), CRLF);
		strResult.AppendFormat("%s%13s: %s%s", SEP2, "ContentType", pSender->GetContentType(dwConnID), CRLF);

		return strResult;
	}

public:
	CHttpServerListenerImpl()
	{
	}

	void SetManagerPtr(std::shared_ptr<IPluginManager> ptr)
	{
		m_ptr_manager = ptr;
	}

	void Init()
	{
		m_thread_pool_module = m_ptr_manager->GetModule<IThreadPoolModule>();
		m_client_net_module = m_ptr_manager->GetModule<IClientNetModule>();
	}
};