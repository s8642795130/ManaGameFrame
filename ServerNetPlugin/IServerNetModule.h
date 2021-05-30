#pragma once
#include "../Server/IModule.h"

class IClientNetActor;

class IServerNetModule : public IModule
{
public:
	IServerNetModule(std::shared_ptr<IPluginManager> ptr) : IModule(ptr)
	{
	}

	// interface
	virtual bool AddFD(std::shared_ptr<IClientNetActor> ptr_client) = 0;
	virtual void EventLoop() = 0;
};

