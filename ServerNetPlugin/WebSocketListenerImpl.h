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

class CWebSocketListenerImpl : public CHttpServerListener
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

		INetActor* ptr_client = nullptr;
		if (pSender->GetConnectionExtra(dwConnID, reinterpret_cast<PVOID*>(&ptr_client)) == TRUE)
		{
			m_thread_pool_module->RemoveActorFromThreadCell(ptr_client->GetUUID());
		}

		return HR_OK;
	}

	EnHandleResult OnShutdown(ITcpServer* pSender)
	{
		return HR_OK;
	}

	// ------------------------------------------------------------------------------------------------------------- //

	EnHttpParseResult OnHeader(IHttpServer* pSender, CONNID dwConnID, LPCSTR lpszName, LPCSTR lpszValue)
	{
		return HPR_OK;
	}

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

	EnHttpParseResult OnUpgrade(IHttpServer* pSender, CONNID dwConnID, EnHttpUpgradeType enUpgradeType)
	{

		if (enUpgradeType == HUT_HTTP_TUNNEL)
		{
			pSender->SendResponse(dwConnID, HSC_OK, "Connection Established");
		}
		else if (enUpgradeType == HUT_WEB_SOCKET)
		{
			int iHeaderCount = 2;
			THeader header[] = { {"Connection", UPGRADE_HEADER},
								{UPGRADE_HEADER, WEB_SOCKET_HEADER_VALUE},
								{nullptr, nullptr},
								{nullptr, nullptr} };


			LPCSTR lpszAccept = nullptr;

			if (!pSender->GetHeader(dwConnID, "Sec-WebSocket-Key", &lpszAccept))
				return HPR_ERROR;

			CStringA strAccept;
			::MakeSecWebSocketAccept(lpszAccept, strAccept);

			header[2].name = "Sec-WebSocket-Accept";
			header[2].value = strAccept;
			++iHeaderCount;

			CStringA strFirst;
			LPCSTR lpszProtocol = nullptr;

			if (pSender->GetHeader(dwConnID, "Sec-WebSocket-Protocol", &lpszProtocol))
			{
				int i = 0;
				CStringA strProtocol(lpszProtocol);
				strFirst = strProtocol.Tokenize(", ", i);

				if (!strFirst.IsEmpty())
				{
					header[3].name = "Sec-WebSocket-Protocol";
					header[3].value = strFirst;
					++iHeaderCount;
				}
			}

			pSender->SendResponse(dwConnID, HSC_SWITCHING_PROTOCOLS, nullptr, header, iHeaderCount);
			// pSender->SetConnectionExtra(dwConnID, new CBufferPtr);

			auto ptr_client = m_client_net_module->CreateWsClientNet(pSender);

			ptr_client->SetSid(dwConnID);
			pSender->SetConnectionExtra(dwConnID, ptr_client.get());
			m_thread_pool_module->AddActorToThreadCell(ptr_client);
		}
		else
			ASSERT(FALSE);

		return HPR_OK;
	}

	EnHttpParseResult OnParseError(IHttpServer* pSender, CONNID dwConnID, int iErrorCode, LPCSTR lpszErrorDesc)
	{
		return HPR_OK;
	}

	// ------------------------------------------------------------------------------------------------------------- //

	EnHandleResult OnWSMessageHeader(IHttpServer* pSender, CONNID dwConnID, BOOL bFinal, BYTE iReserved, BYTE iOperationCode, const BYTE lpszMask[4], ULONGLONG ullBodyLen)
	{
		return HR_OK;
	}

	EnHandleResult OnWSMessageBody(IHttpServer* pSender, CONNID dwConnID, const BYTE* pData, int iLength)
	{
		CBufferPtr* pBuffer = nullptr;
		pSender->GetConnectionExtra(dwConnID, (PVOID*)&pBuffer);
		VERIFY(pBuffer);

		pBuffer->Cat(pData, iLength);

		return HR_OK;
	}

	EnHandleResult OnWSMessageComplete(IHttpServer* pSender, CONNID dwConnID)
	{

		CBufferPtr* pBuffer = nullptr;
		pSender->GetConnectionExtra(dwConnID, (PVOID*)&pBuffer);
		VERIFY(pBuffer);

		BOOL bFinal;
		BYTE iReserved, iOperationCode;

		VERIFY(pSender->GetWSMessageState(dwConnID, &bFinal, &iReserved, &iOperationCode, nullptr, nullptr, nullptr));

		pSender->SendWSMessage(dwConnID, bFinal, iReserved, iOperationCode, pBuffer->Ptr(), (int)pBuffer->Size());
		pBuffer->Free();

		if (iOperationCode == 0x8)
			pSender->Disconnect(dwConnID);

		return HR_OK;
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
	CWebSocketListenerImpl()
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