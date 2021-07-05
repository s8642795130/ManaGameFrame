#pragma once

#include <array>

#include "HPSocket.h"
#include "../Server/IPluginManager.h"
#include "../ActorPlugin/IThreadPoolModule.h"
#include "INetActor.h"

class CListenerImpl : public CTcpServerListener
{
protected:
	// manager
	std::shared_ptr<IPluginManager> m_ptr_manager;

	// module
	std::shared_ptr<IThreadPoolModule> m_thread_pool_module;
	std::shared_ptr<IClientNetModule> m_client_net_module;

public:
	CListenerImpl()
	{
		m_thread_pool_module = m_ptr_manager->GetModule<IThreadPoolModule>();
		m_client_net_module = m_ptr_manager->GetModule<IClientNetModule>();
	}

	virtual EnHandleResult OnPrepareListen(ITcpServer* pSender, SOCKET soListen) override
	{
		std::array<char, 50> szAddress;
		int iAddressLen = 50;
		USHORT usPort = 0;

		pSender->GetListenAddress(szAddress.data(), iAddressLen, usPort);

		return HR_OK;
	}

	virtual EnHandleResult OnAccept(ITcpServer* pSender, CONNID dwConnID, UINT_PTR soClient) override
	{
		std::array<char, 50> szAddress;
		int iAddressLen = 50;
		USHORT usPort;

		pSender->GetRemoteAddress(dwConnID, szAddress.data(), iAddressLen, usPort);

		return HR_OK; // HR_ERROR
	}

	virtual EnHandleResult OnHandShake(ITcpServer* pSender, CONNID dwConnID) override
	{
		return HR_OK;
	}

	virtual EnHandleResult OnReceive(ITcpServer* pSender, CONNID dwConnID, const BYTE* pData, int iLength) override
	{
		INetActor* ptr_client = nullptr;
		if (pSender->GetConnectionExtra(dwConnID, reinterpret_cast<PVOID*>(&ptr_client)) == TRUE)
		{
			ptr_client->PushData(pData, iLength);
		}

		return HR_OK; // HR_ERROR
	}

	virtual EnHandleResult OnSend(ITcpServer* pSender, CONNID dwConnID, const BYTE* pData, int iLength) override
	{
		return HR_OK;
	}

	virtual EnHandleResult OnClose(ITcpServer* pSender, CONNID dwConnID, EnSocketOperation enOperation, int iErrorCode) override
	{
		INetActor* ptr_client = nullptr;
		if (pSender->GetConnectionExtra(dwConnID, reinterpret_cast<PVOID*>(&ptr_client)) == TRUE)
		{
			m_thread_pool_module->RemoveActorFromThreadCell(ptr_client->GetUUID());
		}

		if (iErrorCode == SE_OK)
		{
			// close
		}
		else
		{
			// ::PostOnError(dwConnID, enOperation, iErrorCode);
		}

		return HR_OK;
	}

	virtual EnHandleResult OnShutdown(ITcpServer* pSender) override
	{
		return HR_OK;
	}

};