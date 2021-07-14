#pragma once

#include "IListenerImpl.h"

class CMasterListenerImpl : public IListenerImpl
{
public:

	virtual EnHandleResult OnAccept(ITcpServer* ptr_sender, CONNID dwConnID, UINT_PTR soClient) override
	{
		std::array<char, 50> szAddress;
		int iAddressLen = 50;
		USHORT usPort;

		ptr_sender->GetRemoteAddress(dwConnID, szAddress.data(), iAddressLen, usPort);

		//
		auto ptr_client = m_client_net_module->CreateSocketClientNet(ptr_sender);
		ptr_sender->SetConnectionExtra(dwConnID, ptr_client.get());
		m_thread_pool_module->AddActorToThreadCell(ptr_client);
		//

		return HR_OK; // HR_ERROR
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
};