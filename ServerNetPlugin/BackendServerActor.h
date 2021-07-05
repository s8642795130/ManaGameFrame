#pragma once
#include "IBackendActor.h"

class BackendServerActor : public IBackendActor
{
protected:
    EnumDefine::ClientType m_client_type;

	void ProcessServerBackendIO();
	void ProcessRPCIO();

public:
	BackendServerActor(std::shared_ptr<IPluginManager> ptr_manager, std::shared_ptr<ClientPimpl> ptr_impl, ITcpServer* ptr_sender) :
		IBackendActor(ptr_manager, ptr_impl, ptr_sender)
	{
	}

	void SetClientType(EnumDefine::ClientType client_type);
    virtual void ProcessIO();
};

