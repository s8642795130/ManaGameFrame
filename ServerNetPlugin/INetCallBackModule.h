#pragma once
#include "../Server/IModule.h"

class INetCallBackModule : public IModule
{
public:
	INetCallBackModule(std::shared_ptr<IPluginManager> ptr) : IModule(ptr)
	{
	}
};

