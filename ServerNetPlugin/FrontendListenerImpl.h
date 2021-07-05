#pragma once

#include "SocketListenerImpl.h"
#include "INetActor.h"

class CFrontendListenerImpl : public CListenerImpl
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
};