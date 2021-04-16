#pragma once
#include <memory>

#include "../Server/IModule.h"
#include "../Server/ClientDescriptor.h"
#include "MasterActor.h"

class MasterModule : public IModule
{
private:
	std::shared_ptr<MasterActor> m_master_actor;
public:
	MasterModule(std::shared_ptr<IPlugin> ptr) : IModule(ptr)
	{
	}

	virtual void Init();
	virtual void AfterInit();

	//
	void OnServerOnlineCallback(ClientDescriptor* ptr_client);
};
