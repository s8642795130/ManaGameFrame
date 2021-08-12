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
		std::cout << __func__ << std::endl;
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
		std::cout << __func__ << std::endl;
		return HPR_OK;
	}

	EnHttpParseResult OnHeadersComplete(IHttpServer* pSender, CONNID dwConnID)
	{
		CStringA strSummary = GetHeaderSummary(pSender, dwConnID, "    ", 0, TRUE);
		std::cout << __func__ << std::endl;

		return HPR_OK;
	}

	EnHttpParseResult OnBody(IHttpServer* pSender, CONNID dwConnID, const BYTE* pData, int iLength)
	{
		std::cout << __func__ << std::endl;
		return HPR_OK;
	}

	EnHttpParseResult OnMessageComplete(IHttpServer* pSender, CONNID dwConnID)
	{
		std::cout << __func__ << std::endl;

		if (pSender->IsUpgrade(dwConnID))
		{
			std::cout << "Upgrade" << std::endl;
			return HPR_OK;
		}

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
		INetActor* ptr_client = nullptr;
		if (pSender->GetConnectionExtra(dwConnID, reinterpret_cast<PVOID*>(&ptr_client)) == TRUE)
		{
			ptr_client->PushData(pData, iLength);
		}

		return HR_OK;
	}

	EnHandleResult OnWSMessageComplete(IHttpServer* pSender, CONNID dwConnID)
	{
		INetActor* ptr_client = nullptr;
		if (pSender->GetConnectionExtra(dwConnID, reinterpret_cast<PVOID*>(&ptr_client)) == TRUE)
		{
		}

		BOOL bFinal;
		BYTE iReserved, iOperationCode;

		VERIFY(pSender->GetWSMessageState(dwConnID, &bFinal, &iReserved, &iOperationCode, nullptr, nullptr, nullptr));

		std::array<BYTE, 20> buffer{ 1 };

		pSender->SendWSMessage(dwConnID, bFinal, iReserved, iOperationCode, buffer.data(), (int)buffer.size());

		if (iOperationCode == 0x8)
			pSender->Disconnect(dwConnID);

		return HR_OK;
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