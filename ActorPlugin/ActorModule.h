#pragma once
#include "IActorModule.h"

class ActorModule : public IActorModule
{
public:
	ActorModule(std::shared_ptr<IPluginManager> ptr) : IActorModule(ptr)
	{
	}

	// life cycle
	virtual void Init();
};

