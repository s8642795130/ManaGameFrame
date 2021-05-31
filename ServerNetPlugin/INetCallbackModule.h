#pragma once
#include "../Server/IModule.h"
#include "IClientNetActor.h"

class INetCallbackModule : public IModule
{
public:
	INetCallbackModule(std::shared_ptr<IPluginManager> ptr) : IModule(ptr)
	{
	}

	// interface
	virtual void AddReceiveCallBack(const int msgID, std::function<void(IClientNetActor&)> call_func) = 0;
	virtual const std::map<int, std::function<void(IClientNetActor&)>>& GetReceiveCallBackMap() = 0;
	virtual const std::map<int, std::string> GetGameMsgMap() = 0;
};

