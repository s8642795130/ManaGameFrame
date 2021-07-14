#pragma once
#include "../Server/IModule.h"

class IServerNetModule : public IModule
{
public:
	IServerNetModule(std::shared_ptr<IPluginManager> ptr) : IModule(ptr)
	{
	}

	virtual void Init() = 0;
	virtual void AfterInit() = 0;
};

