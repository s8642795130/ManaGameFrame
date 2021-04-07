#pragma once
#include <memory>

#include "../Server/IModule.h"

class MasterModule : public IModule
{
public:
	MasterModule(std::shared_ptr<IPlugin> ptr) : IModule(ptr)
	{
	}

	virtual void Init();
	virtual void AfterInit();
	virtual void ReadyExecute();
	virtual void Execute();
};
