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
	virtual void EventLoop() = 0;
};

