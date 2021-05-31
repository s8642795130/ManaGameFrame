#pragma once
#include "../Server/IModule.h"

class IMasterModule : public IModule
{
public:
	IMasterModule(std::shared_ptr<IPluginManager> ptr) : IModule(ptr)
	{
	}

	// life cycle
	virtual void Init() = 0;
	virtual void AfterInit() = 0;
};