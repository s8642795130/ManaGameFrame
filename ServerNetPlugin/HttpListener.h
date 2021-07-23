#pragma once

#include "HPSocket.h"
/*
class CHttpServerListenerImpl : public CHttpServerListener
{
private:
	virtual EnHandleResult OnPrepareListen(ITcpServer* pSender, SOCKET soListen);
	virtual EnHandleResult OnAccept(ITcpServer* pSender, CONNID dwConnID, UINT_PTR soClient);
	virtual EnHandleResult OnHandShake(ITcpServer* pSender, CONNID dwConnID);
	virtual EnHandleResult OnReceive(ITcpServer* pSender, CONNID dwConnID, const BYTE* pData, int iLength);
	virtual EnHandleResult OnSend(ITcpServer* pSender, CONNID dwConnID, const BYTE* pData, int iLength);
	virtual EnHandleResult OnClose(ITcpServer* pSender, CONNID dwConnID, EnSocketOperation enOperation, int iErrorCode);
	virtual EnHandleResult OnShutdown(ITcpServer* pSender);

	virtual EnHttpParseResult OnMessageBegin(IHttpServer* pSender, CONNID dwConnID);
	virtual EnHttpParseResult OnRequestLine(IHttpServer* pSender, CONNID dwConnID, LPCSTR lpszMethod, LPCSTR lpszUrl);
	virtual EnHttpParseResult OnHeader(IHttpServer* pSender, CONNID dwConnID, LPCSTR lpszName, LPCSTR lpszValue);
	virtual EnHttpParseResult OnHeadersComplete(IHttpServer* pSender, CONNID dwConnID);
	virtual EnHttpParseResult OnBody(IHttpServer* pSender, CONNID dwConnID, const BYTE* pData, int iLength);
	virtual EnHttpParseResult OnChunkHeader(IHttpServer* pSender, CONNID dwConnID, int iLength);
	virtual EnHttpParseResult OnChunkComplete(IHttpServer* pSender, CONNID dwConnID);
	virtual EnHttpParseResult OnMessageComplete(IHttpServer* pSender, CONNID dwConnID);
	virtual EnHttpParseResult OnUpgrade(IHttpServer* pSender, CONNID dwConnID, EnHttpUpgradeType enUpgradeType);
	virtual EnHttpParseResult OnParseError(IHttpServer* pSender, CONNID dwConnID, int iErrorCode, LPCSTR lpszErrorDesc);

	virtual EnHandleResult OnWSMessageHeader(IHttpServer* pSender, CONNID dwConnID, BOOL bFinal, BYTE iReserved, BYTE iOperationCode, const BYTE lpszMask[4], ULONGLONG ullBodyLen);
	virtual EnHandleResult OnWSMessageBody(IHttpServer* pSender, CONNID dwConnID, const BYTE* pData, int iLength);
	virtual EnHandleResult OnWSMessageComplete(IHttpServer* pSender, CONNID dwConnID);

private:
	CStringA GetHeaderSummary(IHttpServer* pSender, CONNID dwConnID, LPCSTR lpszSep = "  ", int iSepCount = 0, BOOL bWithContentLength = TRUE);

public:
	CHttpServerListenerImpl(LPCTSTR lpszName)
	{
	}
};

// ------------------------------------------------------------------------------------------------------------- //

EnHandleResult CHttpServerListenerImpl::OnPrepareListen(ITcpServer* pSender, SOCKET soListen)
{
	TCHAR szAddress[50];
	int iAddressLen = sizeof(szAddress) / sizeof(TCHAR);
	USHORT usPort;

	pSender->GetListenAddress(szAddress, iAddressLen, usPort);

	return HR_OK;
}

EnHandleResult CHttpServerListenerImpl::OnAccept(ITcpServer* pSender, CONNID dwConnID, UINT_PTR soClient)
{
	BOOL bPass = TRUE;
	TCHAR szAddress[50];
	int iAddressLen = sizeof(szAddress) / sizeof(TCHAR);
	USHORT usPort;

	pSender->GetRemoteAddress(dwConnID, szAddress, iAddressLen, usPort);

	return bPass ? HR_OK : HR_ERROR;
}

EnHandleResult CHttpServerListenerImpl::OnHandShake(ITcpServer* pSender, CONNID dwConnID)
{
	return HR_OK;
}

EnHandleResult CHttpServerListenerImpl::OnReceive(ITcpServer* pSender, CONNID dwConnID, const BYTE* pData, int iLength)
{
	if (pSender->Send(dwConnID, pData, iLength))
		return HR_OK;
	else
		return HR_ERROR;
}

EnHandleResult CHttpServerListenerImpl::OnSend(ITcpServer* pSender, CONNID dwConnID, const BYTE* pData, int iLength)
{
	return HR_OK;
}

EnHandleResult CHttpServerListenerImpl::OnClose(ITcpServer* pSender, CONNID dwConnID, EnSocketOperation enOperation, int iErrorCode)
{
	iErrorCode == SE_OK ? ::PostOnClose(dwConnID, m_strName) :
		::PostOnError(dwConnID, enOperation, iErrorCode, m_strName);

	CBufferPtr* pBuffer = nullptr;
	pSender->GetConnectionExtra(dwConnID, (PVOID*)&pBuffer);

	if (pBuffer) delete pBuffer;

	return HR_OK;
}

EnHandleResult CHttpServerListenerImpl::OnShutdown(ITcpServer* pSender)
{
	::PostOnShutdown(m_strName);
	return HR_OK;
}

// ------------------------------------------------------------------------------------------------------------- //

EnHttpParseResult CHttpServerListenerImpl::OnMessageBegin(IHttpServer* pSender, CONNID dwConnID)
{
	::PostOnMessageBegin(dwConnID, m_strName);
	return HPR_OK;
}

EnHttpParseResult CHttpServerListenerImpl::OnRequestLine(IHttpServer* pSender, CONNID dwConnID, LPCSTR lpszMethod, LPCSTR lpszUrl)
{
	::PostOnRequestLine(dwConnID, lpszMethod, pSender->GetUrlFieldSet(dwConnID), lpszUrl, m_strName);
	return HPR_OK;
}

EnHttpParseResult CHttpServerListenerImpl::OnHeader(IHttpServer* pSender, CONNID dwConnID, LPCSTR lpszName, LPCSTR lpszValue)
{
	::PostOnHeader(dwConnID, lpszName, lpszValue, m_strName);
	return HPR_OK;
}

EnHttpParseResult CHttpServerListenerImpl::OnHeadersComplete(IHttpServer* pSender, CONNID dwConnID)
{
	CStringA strSummary = GetHeaderSummary(pSender, dwConnID, "    ", 0, TRUE);
	::PostOnHeadersComplete(dwConnID, strSummary, m_strName);

	return HPR_OK;
}

EnHttpParseResult CHttpServerListenerImpl::OnBody(IHttpServer* pSender, CONNID dwConnID, const BYTE* pData, int iLength)
{
	::PostOnBody(dwConnID, pData, iLength, m_strName);
	return HPR_OK;
}

EnHttpParseResult CHttpServerListenerImpl::OnChunkHeader(IHttpServer* pSender, CONNID dwConnID, int iLength)
{
	return HPR_OK;
}

EnHttpParseResult CHttpServerListenerImpl::OnChunkComplete(IHttpServer* pSender, CONNID dwConnID)
{
	::PostOnChunkComplete(dwConnID, m_strName);
	return HPR_OK;
}

EnHttpParseResult CHttpServerListenerImpl::OnMessageComplete(IHttpServer* pSender, CONNID dwConnID)
{
	::PostOnMessageComplete(dwConnID, m_strName);

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

EnHttpParseResult CHttpServerListenerImpl::OnUpgrade(IHttpServer* pSender, CONNID dwConnID, EnHttpUpgradeType enUpgradeType)
{
	::PostOnUpgrade(dwConnID, enUpgradeType, m_strName);

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
		pSender->SetConnectionExtra(dwConnID, new CBufferPtr);
	}
	else
		ASSERT(FALSE);

	return HPR_OK;
}

EnHttpParseResult CHttpServerListenerImpl::OnParseError(IHttpServer* pSender, CONNID dwConnID, int iErrorCode, LPCSTR lpszErrorDesc)
{
	::PostOnParseError(dwConnID, iErrorCode, lpszErrorDesc, m_strName);
	return HPR_OK;
}

// ------------------------------------------------------------------------------------------------------------- //

EnHandleResult CHttpServerListenerImpl::OnWSMessageHeader(IHttpServer* pSender, CONNID dwConnID, BOOL bFinal, BYTE iReserved, BYTE iOperationCode, const BYTE lpszMask[4], ULONGLONG ullBodyLen)
{
	::PostOnWSMessageHeader(dwConnID, bFinal, iReserved, iOperationCode, lpszMask, ullBodyLen, m_strName);

	return HR_OK;
}

EnHandleResult CHttpServerListenerImpl::OnWSMessageBody(IHttpServer* pSender, CONNID dwConnID, const BYTE* pData, int iLength)
{
	CBufferPtr* pBuffer = nullptr;
	pSender->GetConnectionExtra(dwConnID, (PVOID*)&pBuffer);
	VERIFY(pBuffer);

	pBuffer->Cat(pData, iLength);

	return HR_OK;
}

EnHandleResult CHttpServerListenerImpl::OnWSMessageComplete(IHttpServer* pSender, CONNID dwConnID)
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
*/