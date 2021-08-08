#pragma once
#include <memory>

#include "HPSocket.h"
#include "Helper.h"

#include "../Server/IPluginManager.h"
#include "../ActorPlugin/IThreadPoolModule.h"
#include "IClientNetModule.h"
#include "FrontendHttpActor.h"
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

		if (iErrorCode != SE_OK)
		{
			std::cerr << "http close error: " << iErrorCode << std::endl;
		}

		std::cout << __func__ << std::endl;

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

	EnHttpParseResult OnHeadersComplete(IHttpServer* pSender, CONNID dwConnID)
	{
		return HPR_OK;
	}

	EnHttpParseResult OnBody(IHttpServer* pSender, CONNID dwConnID, const BYTE* pData, int iLength)
	{
		return HPR_OK;
	}

	EnHttpParseResult OnMessageComplete(IHttpServer* pSender, CONNID dwConnID)
	{
		std::cout << __func__ << std::endl;

		if (pSender->IsUpgrade(dwConnID))
		{
			return HPR_OK;
		}

		// create client
		auto ptr_client = m_client_net_module->CreateHttpClientNet(pSender);

		// add client actor
		ptr_client->SetSid(dwConnID);
		pSender->SetConnectionExtra(dwConnID, ptr_client.get());
		m_thread_pool_module->AddActorToThreadCell(ptr_client);
		ptr_client->ProcessIO();

		if (!pSender->IsKeepAlive(dwConnID))
			pSender->Release(dwConnID);

		return HPR_OK;
	}

	EnHttpParseResult OnParseError(IHttpServer* pSender, CONNID dwConnID, int iErrorCode, LPCSTR lpszErrorDesc)
	{
		return HPR_OK;
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