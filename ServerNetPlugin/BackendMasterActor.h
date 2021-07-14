#pragma once
#include "IBackendActor.h"

class BackendMasterActor : public IBackendActor, public std::enable_shared_from_this<BackendMasterActor>
{
public:
	BackendMasterActor(std::shared_ptr<IPluginManager> ptr_manager, std::shared_ptr<ClientPimpl> ptr_impl, ITcpServer* ptr_sender) :
		IBackendActor(ptr_manager, ptr_impl, ptr_sender)
	{
	}

	virtual void ProcessIO()
	{
		int majorId = m_buffer->GetMajorId();
		auto map_callback = m_client_impl->m_callback_module->GetReceiveCallbackMap();

		// check
		if (map_callback.find(majorId) != std::cend(map_callback))
		{
			auto callback = map_callback[majorId];
			callback->Trigger(m_client_impl->m_thread_pool_module, shared_from_this());
		}
	}
};