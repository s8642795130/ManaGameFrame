#pragma once
#include <memory>

#include "../Server/IModule.h"

class IActorModule : public IModule
{
public:
	IActorModule(std::shared_ptr<IPluginManager> ptr) : IModule(ptr)
	{
	}

	// life cycle
	virtual void Init() = 0;
};