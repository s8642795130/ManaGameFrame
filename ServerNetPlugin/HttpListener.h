#pragma once

#include "HPSocket.h"
#include "Helper.h"

#include "../Server/IPluginManager.h"
#include "../ActorPlugin/IThreadPoolModule.h"
#include "IClientNetModule.h"
#include "INetActor.h"

class CHttpServerListenerImpl : public CHttpServerListener
{
protected:
	// manager
	std::shared_ptr<IPluginManager> m_ptr_manager;

	// module
	std::shared_ptr<IThreadPoolModule> m_thread_pool_module;
	std::shared_ptr<IClientNetModule> m_client_net_module;

private:
	virtual EnHandleResult OnPrepareListen(ITcpServer* pSender, SOCKET soListen)
	{
		TCHAR szAddress[50];
		int iAddressLen = sizeof(szAddress) / sizeof(TCHAR);
		USHORT usPort;

		pSender->GetListenAddress(szAddress, iAddressLen, usPort);

		return HR_OK;
	}

	virtual EnHandleResult OnAccept(ITcpServer* pSender, CONNID dwConnID, UINT_PTR soClient)
	{
		BOOL bPass = TRUE;
		TCHAR szAddress[50];
		int iAddressLen = sizeof(szAddress) / sizeof(TCHAR);
		USHORT usPort;

		pSender->GetRemoteAddress(dwConnID, szAddress, iAddressLen, usPort);

		std::cout << "OnAccept " << szAddress << std::endl;

		return bPass ? HR_OK : HR_ERROR;
	}

	virtual EnHandleResult OnHandShake(ITcpServer* pSender, CONNID dwConnID)
	{
		return HR_OK;
	}

	virtual EnHandleResult OnReceive(ITcpServer* pSender, CONNID dwConnID, const BYTE* pData, int iLength)
	{
		// if (pSender->Send(dwConnID, pData, iLength))
		// 	return HR_OK;
		// else
		// 	return HR_ERROR;

		std::cout << "http OnReceive" << std::endl;

		return HR_OK;
	}

	virtual EnHandleResult OnSend(ITcpServer* pSender, CONNID dwConnID, const BYTE* pData, int iLength)
	{
		return HR_OK;
	}

	virtual EnHandleResult OnClose(ITcpServer* pSender, CONNID dwConnID, EnSocketOperation enOperation, int iErrorCode)
	{
		INetActor* ptr_client = nullptr;
		if (pSender->GetConnectionExtra(dwConnID, reinterpret_cast<PVOID*>(&ptr_client)) == TRUE)
		{
			m_thread_pool_module->RemoveActorFromThreadCell(ptr_client->GetUUID());
		}

		if (iErrorCode == SE_OK)
		{
			return HR_ERROR;
		}

		return HR_OK;
	}

	virtual EnHandleResult OnShutdown(ITcpServer* pSender)
	{
		return HR_OK;
	}

	// ------------------------------------------------------------------------------------------------------------- //

	virtual EnHttpParseResult OnMessageBegin(IHttpServer* pSender, CONNID dwConnID)
	{
		return HPR_OK;
	}

	virtual EnHttpParseResult OnRequestLine(IHttpServer* pSender, CONNID dwConnID, LPCSTR lpszMethod, LPCSTR lpszUrl)
	{
		return HPR_OK;
	}

	virtual EnHttpParseResult OnHeader(IHttpServer* pSender, CONNID dwConnID, LPCSTR lpszName, LPCSTR lpszValue)
	{
		std::cout << "http OnHeader" << std::endl;

		return HPR_OK;
	}

	virtual EnHttpParseResult OnHeadersComplete(IHttpServer* pSender, CONNID dwConnID)
	{
		return HPR_OK;
	}

	virtual EnHttpParseResult OnBody(IHttpServer* pSender, CONNID dwConnID, const BYTE* pData, int iLength)
	{
		std::cout << "http OnBody" << std::endl;

		return HPR_OK;
	}

	virtual EnHttpParseResult OnChunkHeader(IHttpServer* pSender, CONNID dwConnID, int iLength)
	{
		return HPR_OK;
	}

	virtual EnHttpParseResult OnChunkComplete(IHttpServer* pSender, CONNID dwConnID)
	{
		return HPR_OK;
	}

	virtual EnHttpParseResult OnMessageComplete(IHttpServer* pSender, CONNID dwConnID)
	{
		if (pSender->IsUpgrade(dwConnID))
			return HPR_OK;

		/*
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
		*/

		/*
		pSender->SendResponse(dwConnID,
			HSC_OK,
			"HP Http Server OK",
			header, iHeaderCount,
			(const BYTE*)(LPCSTR)strBody,
			iBodyLength);
		*/

		if (!pSender->IsKeepAlive(dwConnID))
			pSender->Release(dwConnID);

		return HPR_OK;
	}

	virtual EnHttpParseResult OnUpgrade(IHttpServer* pSender, CONNID dwConnID, EnHttpUpgradeType enUpgradeType)
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

			// CStringA strAccept;
			// ::MakeSecWebSocketAccept(lpszAccept, strAccept);
			auto strAccept = ::MakeSecWebSocketAccept(lpszAccept);

			header[2].name = "Sec-WebSocket-Accept";
			header[2].value = (LPCSTR)strAccept.data();
			++iHeaderCount;

			// CStringA strFirst;
			LPCSTR lpszProtocol = nullptr;

			if (pSender->GetHeader(dwConnID, "Sec-WebSocket-Protocol", &lpszProtocol))
			{
				/*
				int i = 0;
				CStringA strProtocol(lpszProtocol);
				strFirst = strProtocol.Tokenize(", ", i);

				if (!strFirst.IsEmpty())
				{
					header[3].name = "Sec-WebSocket-Protocol";
					header[3].value = strFirst;
					++iHeaderCount;
				}
				*/
			}

			pSender->SendResponse(dwConnID, HSC_SWITCHING_PROTOCOLS, nullptr, header, iHeaderCount);
			// pSender->SetConnectionExtra(dwConnID, new CBufferPtr);

			//
			auto ptr_client = m_client_net_module->CreateHttpClientNet(pSender);
			ptr_client->SetSid(dwConnID);
			pSender->SetConnectionExtra(dwConnID, ptr_client.get());
			m_thread_pool_module->AddActorToThreadCell(ptr_client);
			//
		}
		else
		{
			std::cerr << "OnUpgrade Error" << std::endl;
			std::exit(1);
		}

		return HPR_OK;
	}

	virtual EnHttpParseResult OnParseError(IHttpServer* pSender, CONNID dwConnID, int iErrorCode, LPCSTR lpszErrorDesc)
	{
		return HPR_OK;
	}

	// ------------------------------------------------------------------------------------------------------------- //

	virtual EnHandleResult OnWSMessageHeader(IHttpServer* pSender, CONNID dwConnID, BOOL bFinal, BYTE iReserved, BYTE iOperationCode, const BYTE lpszMask[4], ULONGLONG ullBodyLen)
	{
		return HR_OK;
	}

	virtual EnHandleResult OnWSMessageBody(IHttpServer* pSender, CONNID dwConnID, const BYTE* pData, int iLength)
	{
		INetActor* ptr_client = nullptr;
		if (pSender->GetConnectionExtra(dwConnID, reinterpret_cast<PVOID*>(&ptr_client)) == TRUE)
		{
			ptr_client->PushData(pData, iLength);
		}

		return HR_OK;
	}

	virtual EnHandleResult OnWSMessageComplete(IHttpServer* pSender, CONNID dwConnID)
	{
		BOOL bFinal;
		BYTE iReserved, iOperationCode;

		pSender->GetWSMessageState(dwConnID, &bFinal, &iReserved, &iOperationCode, nullptr, nullptr, nullptr);

		if (iOperationCode == 0x8)
		{
			pSender->Disconnect(dwConnID);
		}

		return HR_OK;
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