#pragma once
#include "../Server/IModule.h"

class ILoginModule : public IModule
{
public:
	ILoginModule(std::shared_ptr<IPluginManager> ptr) : IModule(ptr)
	{
	}

	// life cycle
	virtual void Init() = 0;
	virtual void AfterInit() = 0;
};