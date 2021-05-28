#pragma once
#include "../Server/IModule.h"
#include "IClientNetActor.h"

class INetCallBackModule : public IModule
{
public:
	INetCallBackModule(std::shared_ptr<IPluginManager> ptr) : IModule(ptr)
	{
	}

	virtual void AddReceiveCallBack(const int msgID, std::function<void(std::shared_ptr<IClientNetActor>)> call_func) = 0;
};

