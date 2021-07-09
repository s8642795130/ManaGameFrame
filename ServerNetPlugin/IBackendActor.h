#pragma once
#include "NetActor.h"

class IBackendActor : public NetActor
{
public:
	IBackendActor(std::shared_ptr<IPluginManager> ptr_manager, std::shared_ptr<ClientPimpl> ptr_impl, ITcpServer* ptr_sender) :
		NetActor(ptr_manager, ptr_impl, ptr_sender)
	{
	}
};